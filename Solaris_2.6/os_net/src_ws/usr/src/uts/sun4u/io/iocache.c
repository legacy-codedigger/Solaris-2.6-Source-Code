/*
 * Copyright (c) 1995 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)iocache.c 1.30	96/10/17 SMI"

#include <sys/types.h>
#include <sys/conf.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/ddi_impldefs.h>
#include <sys/cmn_err.h>
#include <vm/hat_sfmmu.h>

#include <sys/iommu.h>
#include <sys/iocache.h>
#include <sys/sysiosbus.h>

#include <sys/nexusdebug.h>
#include <sys/debug.h>
#define	IOCACHE_REGISTERS_DEBUG		0x1
#define	IOCACHE_SYNC_DEBUG		0x2
#define	IOCACHE_DIAG_REG_DEBUG		0x4
#define	IOCACHE_SYNC_FAIL_DEBUG		0x8

#define	MAX_RETRY			10

/* Flag which enables the streaming buffer */
int stream_buf_on = 1;
/*
 * This is the number of pages that a mapping request needs before we force
 * the steaminf buffer sync  code to use diagnostic registers.  This value
 * was determined through a series of test runs measuring dma mapping
 * setup performance.
 */
int stream_buf_sync_using_diag = 36;

int
stream_buf_init(struct sbus_soft_state *softsp, caddr_t address)
{
	u_char version;
#ifdef DEBUG
	debug_info = 1;
	debug_print_level = 0;
#endif
	version = (u_char) (*softsp->sysio_ctrl_reg >> SYSIO_VER_SHIFT);
	version &= 0xf;

	if (stream_buf_on == 0 || version == 0) {
		softsp->stream_buf_off = STREAM_BUF_OFF;
		if (version == 0)
			cmn_err(CE_CONT, "Disabling streaming buffer due to "
			    "SYSIO Rev %d.\n", version);
		return (DDI_SUCCESS);
	}

	/*
	 * Simply add each registers offset to the base address
	 * to calculate the already mapped virtual address of
	 * the device register...
	 *
	 * define a macro for the pointer arithmetic; all registers
	 * are 64 bits wide and are defined as u_ll_t's.
	 */

#define	REG_ADDR(b, o)	(u_ll_t *)((caddr_t)(b) + (o))

	softsp->str_buf_ctrl_reg = REG_ADDR(address, OFF_STR_BUF_CTRL_REG);
	softsp->str_buf_flush_reg = REG_ADDR(address, OFF_STR_BUF_FLUSH_REG);
	softsp->str_buf_sync_reg = REG_ADDR(address, OFF_STR_BUF_SYNC_REG);
	softsp->str_buf_pg_tag_diag = REG_ADDR(address, STR_BUF_PAGE_TAG_DIAG);

#undef	REG_ADDR

	DPRINTF(IOCACHE_REGISTERS_DEBUG, ("Streaming buffer control reg: 0x%x, "
	    "Streaming buffer flush reg: 0x%x, Streaming buffer sync reg: 0x%x",
	    softsp->str_buf_ctrl_reg, softsp->str_buf_flush_reg,
	    softsp->str_buf_sync_reg));

	/* Turn on per instance streaming buffer flag */
	softsp->stream_buf_off = 0;

	/* Set the hardware registers */
	(void) stream_buf_resume_init(softsp);

	return (DDI_SUCCESS);
}

/*
 * Initialize stream buf hardware when the system is being resumed.
 * (Subset of stream_buf_init())
 */
int
stream_buf_resume_init(struct sbus_soft_state *softsp)
{
	u_char version;

	version = (u_char) (*softsp->sysio_ctrl_reg >> SYSIO_VER_SHIFT);
	version &= 0xf;

	if (stream_buf_on == 0 || version == 0) {
		softsp->stream_buf_off = STREAM_BUF_OFF;
		return (DDI_SUCCESS);
	}

	/* Turn on the streaming buffer */
	*softsp->str_buf_ctrl_reg = STREAM_BUF_ENABLE;

	return (DDI_SUCCESS);
}

void
sync_stream_buf(struct sbus_soft_state *softsp, u_long addr, u_int npages,
	int *sync_flag, u_ll_t phys_sync_flag)
{
#ifndef lint
	volatile u_ll_t tmp;
#endif
#if DEBUG
	extern volatile clock_t lbolt;
	clock_t start_bolt;
#endif /* DEBUG */

	if (softsp->stream_buf_off != 0)
		return;

	*sync_flag = 0;

	DPRINTF(IOCACHE_SYNC_DEBUG, ("sync_stream_buf: ioaddr 0x%x, page cnt "
	    "0x%x, sync flag 0x%x, sync flag pf 0x%xll\n", addr, npages,
	    sync_flag, phys_sync_flag));

	ASSERT(npages > (u_int)0);
	if (npages > stream_buf_sync_using_diag) {
		int i;
		volatile u_ll_t *reg_addr;
		u_ll_t reg;
		u_long ioaddr;
		u_long hiaddr = addr + (npages * IOMMU_PAGESIZE);
		int do_sync = 0;

		for (i = 0, reg_addr = softsp->str_buf_pg_tag_diag;
		    i < STREAM_CACHE_LINES; i++, reg_addr++) {

			/* Read the page tag diag reg */
			reg = *reg_addr;

#if DEBUG
			{
			u_int hi, lo;
			hi = (u_int)(reg >> 32);
			lo = (u_int)(reg & 0xffffffff);
			DPRINTF(IOCACHE_DIAG_REG_DEBUG, ("IO cache line diag "
			    "reg addr 0x%x, hi0x%x lo0x%x\n", reg_addr,
			    hi, lo));
			}
#endif /* DEBUG */
			/* Check for a valid line */
			if (reg & STR_PG_VALID) {
				ioaddr = (u_long) reg << STR_PG_SHIFT;

				DPRINTF(IOCACHE_DIAG_REG_DEBUG, ("ioaddr 0x%x, "
				    "range base 0x%x, range extent 0x%x\n",
				    ioaddr, addr,
				    addr + (npages * IOMMU_PAGESIZE)));
				if (ioaddr >= addr && ioaddr <= hiaddr) {
					*softsp->str_buf_flush_reg = (u_ll_t)
					    ioaddr;
					do_sync = 1;
				}
			}
		}

		if (!do_sync)
			return;
	} else {
		do {
			*softsp->str_buf_flush_reg = (u_ll_t) addr;
			addr += IOMMU_PAGESIZE;
			npages--;
		} while (npages > (u_int)0);
	}

	/* Ask the hardware to flag when the flush is complete */
	*softsp->str_buf_sync_reg = phys_sync_flag;

#ifndef lint
	/*
	 * Due to the sun4u memory models, this noncached load will sync the
	 * order of all prior loads and stores regardless of cacheability.
	 * No membar_stst() is needed after zeroing the flush sync flag.
	 */
	tmp = *softsp->sbus_ctrl_reg;
#endif

#if DEBUG
	start_bolt = lbolt;
#endif /* DEBUG */

	/*
	 * spin till the hardware is done.  If the hardware is broke, we
	 * spin forever.
	 */
	while (!*sync_flag) {

#if DEBUG
		if ((lbolt - start_bolt) > STREAM_BUF_SYNC_WAIT) {
			cmn_err(CE_PANIC, "Streaming buffer timed out.\n");

		}
#endif /* DEBUG */

	}
}
