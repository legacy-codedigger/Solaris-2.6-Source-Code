/*
 * Copyright (c) 1991-1994, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _SYS_IOMMU_H
#define	_SYS_IOMMU_H

#pragma ident	"@(#)iommu.h	1.20	96/09/03 SMI"

#if defined(_KERNEL) && !defined(_ASM)
#include <sys/sunddi.h>
#include <sys/sysiosbus.h>
#include <sys/ddi_impldefs.h>
#endif /* defined(_KERNEL) && !defined(_ASM) */

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _ASM
/* constants for DVMA */
/*
 * It takes an 8byte TSB entry to map i an 8k page, so the conversion
 * from tsb size to dvma mapping is to multiply by 1000 or 0x400
 * left shift by 10 does this
 */
#define	IOMMU_TSB_TO_RNG	0xa
#define	SBUSMAP_MAXRESERVE	0x1000		/* Max fastdvma is 32M */
#define	SBUSMAP_FRAG(mapsize)	((mapsize) >> 3)
#define	IOMMU_TSB_SIZE_8M	0x2000
#define	IOMMU_TSB_SIZE_16M	0x4000
#define	IOMMU_TSB_SIZE_32M	0x8000
#define	IOMMU_TSB_SIZE_64M	0x10000
#define	IOMMU_TSB_SIZE_128M	0x20000
#define	IOMMU_TSB_SIZE_256M	0x40000
#define	IOMMU_TSB_SIZE_512M	0x80000
#define	IOMMU_TSB_SIZE_1G	0x100000

#define	IOMMU_PAGESIZE		0x2000		/* 8k page */
#define	IOMMU_PAGEMASK		0x1fff
#define	IOMMU_PAGEOFFSET	(IOMMU_PAGESIZE - 1)
#define	IOMMU_N_TTES		(IOMMU_DVMA_RANGE/IOMMU_PAGESIZE)
#define	IOMMU_TSB_TBL_SIZE	(IOMMU_N_TTES << 3)	/* 8B for each entry */
#define	IOMMU_PAGESHIFT		13

#define	OFF_IOMMU_CTRL_REG	0x2400
#define	IOMMU_CTRL_REG_SIZE	(NATURAL_REG_SIZE)
#define	OFF_TSB_BASE_ADDR	0x2408
#define	TSB_BASE_ADDR_SIZE	(NATURAL_REG_SIZE)
#define	OFF_IOMMU_FLUSH_REG	0x2410
#define	IOMMU_FLUSH_REG		(NATURAL_REG_SIZE)
#define	OFF_IOMMU_TLB_TAG	0x4580
#define	OFF_IOMMU_TLB_DATA	0x4600

#define	TSB_SIZE		3		/* 64M of DVMA */
#define	TSB_SIZE_SHIFT		16
#define	IOMMU_TLB_ENTRIES	16

#define	IOMMU_ENABLE		1		/* Turns on the IOMMU */
#define	IOMMU_TLB_VALID		0x40000000ull
#define	IOMMU_DIAG_ENABLE	0x2ull

/* define IOPTEs */
#define	IOTTE_PFN_MSK	0x1ffffffe000ull
#define	IOTTE_CACHE	0x10ull
#define	IOTTE_WRITE	0x2ull
#define	IOTTE_STREAM	0x1000000000000000ull
#define	IOTTE_INTRA	0x800000000000000ull
#define	IOTTE_64K_PAGE	0x2000000000000000ull
#endif	/* _ASM */
#define	IOTTE_VALID	0x8000000000000000ull
#define	IOTTE_PFN_SHIFT 13

/*
 * IOMMU pages to bytes, and back (with and without rounding)
 */
#define	iommu_ptob(x)	((x) << IOMMU_PAGESHIFT)
#define	iommu_btop(x)	(((unsigned)(x)) >> IOMMU_PAGESHIFT)
#define	iommu_btopr(x)	((((unsigned)(x) + IOMMU_PAGEOFFSET) \
				>>  IOMMU_PAGESHIFT))
#if defined(_KERNEL) && !defined(_ASM)

/* sbus nexus private dma mapping structure. */
struct dma_impl_priv {
	ddi_dma_impl_t mp;
	struct sbus_soft_state *softsp;
	volatile int sync_flag;
	u_ll_t phys_sync_flag;
};

extern int iommu_init(struct sbus_soft_state *, caddr_t);
extern int iommu_resume_init(struct sbus_soft_state *);
extern int iommu_dma_mctl(dev_info_t *, dev_info_t *, ddi_dma_handle_t,
	enum ddi_dma_ctlops, off_t *, u_int *, caddr_t *, u_int);
extern int iommu_dma_map(dev_info_t *, dev_info_t *, struct ddi_dma_req *,
	ddi_dma_handle_t *);
extern int iommu_dma_allochdl(dev_info_t *, dev_info_t *, ddi_dma_attr_t *,
	int (*waitfp)(caddr_t), caddr_t arg, ddi_dma_handle_t *);
extern int iommu_dma_freehdl(dev_info_t *, dev_info_t *, ddi_dma_handle_t);
extern int iommu_dma_bindhdl(dev_info_t *, dev_info_t *, ddi_dma_handle_t,
	struct ddi_dma_req *, ddi_dma_cookie_t *, u_int *);
extern int iommu_dma_unbindhdl(dev_info_t *, dev_info_t *, ddi_dma_handle_t);
extern int iommu_dma_flush(dev_info_t *, dev_info_t *, ddi_dma_handle_t,
	off_t, u_int, u_int);
extern int iommu_dma_win(dev_info_t *, dev_info_t *, ddi_dma_handle_t,
	uint_t, off_t *, uint_t *, ddi_dma_cookie_t *, uint_t *);

void
iommu_dvma_kaddr_load(ddi_dma_handle_t h, caddr_t a, u_int len, u_int index,
    ddi_dma_cookie_t *cp);

void
iommu_dvma_unload(ddi_dma_handle_t h, u_int objindex, u_int view);

void
iommu_dvma_sync(ddi_dma_handle_t h, u_int objindex, u_int view);

#endif /* _KERNEL && !_ASM */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_IOMMU_H */
