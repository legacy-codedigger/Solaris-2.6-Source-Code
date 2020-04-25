/*
 * Copyright (c) 1990-1996, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)mp_machdep.c	1.18	96/10/17 SMI"

#include <sys/smp_impldefs.h>
#include <sys/psm.h>
#include <sys/psm_modctl.h>
#include <sys/pit.h>
#include <sys/cmn_err.h>
#include <sys/strlog.h>
#include <sys/clock.h>
#include <sys/debug.h>
#include <sys/rtc.h>
#include <sys/sysmacros.h>

/* pointer to array of frame pointers for other processors */
/* temporary till debugger gives us commands to do so */
int	i_fparray_ptr;

/*
 *	Local function prototypes
 */
static int mp_disable_intr(processorid_t cpun);
static void mp_enable_intr(processorid_t cpun);

static void mach_init();
static void mach_picinit();
static void mach_clkinit();
static void mach_smpinit(void);
static void mach_set_softintr(int ipl);
#if MP
static void mach_cpu_start(int cpun);
#endif
static int mach_softlvl_to_vect(int ipl);

static void mach_get_platform(int owner);
static void mach_construct_info();

static int mach_translate_irq(dev_info_t *dip, int irqno);
static timestruc_t mach_tod_get(void);
static void mach_tod_set(timestruc_t ts);
static void mach_notify_error(int level, char *errmsg);
static void m48t18_check(void);

/*
 *	External reference functions
 */
extern void return_instr();
extern timestruc_t (*todgetf)(void);
extern void (*todsetf)(timestruc_t);
extern char *panicstr;
extern long gmt_lag;

/*
 *	PSM functions initialization
 */
void (*psm_shutdownf)()		= return_instr;
void (*psm_set_idle_cpuf)(int)	= return_instr;
void (*psm_unset_idle_cpuf)(int) = return_instr;
void (*psminitf)()		= mach_init;
void (*picinitf)() 		= return_instr;
void (*clkinitf)() 		= return_instr;
void (*cpu_startf)() 		= return_instr;
int (*ap_mlsetup)() 		= (int (*)(void))return_instr;
void (*send_dirintf)() 		= return_instr;
void (*setspl)(int)		= return_instr;
int (*addspl)(int, int, int, int) = (int (*)(int, int, int, int))return_instr;
int (*delspl)(int, int, int, int) = (int (*)(int, int, int, int))return_instr;
void (*setsoftint)(int)		= (void (*)(int))return_instr;
int (*slvltovect)(int)		= (int (*)(int))return_instr;
int (*setlvl)(int, int *)	= (int (*)(int, int *))return_instr;
void (*setlvlx)(int, int)	= (void (*)(int, int))return_instr;
int (*psm_disable_intr)(int)	= mp_disable_intr;
void (*psm_enable_intr)(int)	= mp_enable_intr;
hrtime_t (*gethrtimef)(void)	= (hrtime_t (*)(void))return_instr;
int (*psm_translate_irq)(dev_info_t *, int) = mach_translate_irq;
int (*psm_todgetf)(todinfo_t *) = (int (*)(todinfo_t *))return_instr;
int (*psm_todsetf)(todinfo_t *) = (int (*)(todinfo_t *))return_instr;
void (*psm_notify_error)(int, char *) = (void (*)(int, char *))NULL;
void (*notify_error)(int, char *) = (void (*)(int, char *))return_instr;

/*
 * Local Static Data
 */
static struct psm_ops mach_ops;
static struct psm_ops *mach_set[4] = {&mach_ops, NULL, NULL, NULL};
static ushort mach_ver[4] = {0, 0, 0, 0};

static int
mp_disable_intr(int cpun)
{
	/*
	 * switch to the offline cpu
	 */
	affinity_set(cpun);
	/*
	 *raise ipl to just below cross call
	 */
	(void) splx(XC_MED_PIL-1);
	/*
	 *	set base spl to prevent the next swtch to idle from
	 *	lowering back to ipl 0
	 */
	CPU->cpu_intr_actv |= (1 << (XC_MED_PIL-1));
	set_base_spl();
	affinity_clear();
	return (DDI_SUCCESS);
}

static void
mp_enable_intr(int cpun)
{
	/*
	 * switch to the online cpu
	 */
	affinity_set(cpun);
	/*
	 * clear the interrupt active mask
	 */
	CPU->cpu_intr_actv &= ~(1 << (XC_MED_PIL-1));
	set_base_spl();
	(void) spl0();
	affinity_clear();
}

static void
mach_get_platform(int owner)
{
	long *srv_opsp;
	long *clt_opsp;
	int	i;
	int	total_ops;

	/* fix up psm ops						*/
	srv_opsp = (long *) mach_set[0];
	clt_opsp = (long *) mach_set[owner];

	if (mach_ver[owner] == (ushort) PSM_INFO_VER01)
		total_ops = sizeof (struct psm_ops_ver01) /
				sizeof (void (*)(void));
	else
		total_ops = sizeof (struct psm_ops) / sizeof (void (*)(void));

	for (i = 0; i < total_ops; i++) {
		if (*clt_opsp != (long) NULL)
			*srv_opsp = *clt_opsp;
		srv_opsp++;
		clt_opsp++;
	}
}

static void
mach_construct_info()
{
	register struct psm_sw *swp;
	int	mach_cnt[PSM_OWN_OVERRIDE+1] = {0};
	int	conflict_owner = 0;

	mutex_enter(&psmsw_lock);
	for (swp = psmsw->psw_forw; swp != psmsw; swp = swp->psw_forw) {
		if (!(swp->psw_flag & PSM_MOD_IDENTIFY))
			continue;
		mach_set[swp->psw_infop->p_owner] = swp->psw_infop->p_ops;
		mach_ver[swp->psw_infop->p_owner] = swp->psw_infop->p_version;
		mach_cnt[swp->psw_infop->p_owner]++;
	}
	mutex_exit(&psmsw_lock);

	mach_get_platform(PSM_OWN_SYS_DEFAULT);

	/* check to see are there any conflicts */
	if (mach_cnt[PSM_OWN_EXCLUSIVE] > 1)
		conflict_owner = PSM_OWN_EXCLUSIVE;
	if (mach_cnt[PSM_OWN_OVERRIDE] > 1)
		conflict_owner = PSM_OWN_OVERRIDE;
	if (conflict_owner) {
		/* remove all psm modules except uppc */
		cmn_err(CE_WARN,
			"Conflicts detected on the following PSM modules:");
		mutex_enter(&psmsw_lock);
		for (swp = psmsw->psw_forw; swp != psmsw; swp = swp->psw_forw) {
			if (swp->psw_infop->p_owner == conflict_owner)
				cmn_err(CE_WARN, "%s ",
					swp->psw_infop->p_mach_idstring);
		}
		mutex_exit(&psmsw_lock);
		cmn_err(CE_WARN,
			"Setting the system back to SINGLE processor mode!");
		cmn_err(CE_WARN,
		    "Please edit /etc/mach to remove the invalid PSM module.");
		return;
	}

	if (mach_set[PSM_OWN_EXCLUSIVE])
		mach_get_platform(PSM_OWN_EXCLUSIVE);

	if (mach_set[PSM_OWN_OVERRIDE])
		mach_get_platform(PSM_OWN_OVERRIDE);

}

static void
mach_init()
{
	register struct psm_ops  *pops;

	mach_construct_info();

	pops = mach_set[0];

	/* register the interrupt and clock initialization rotuines	*/
	picinitf = mach_picinit;
	clkinitf = mach_clkinit;
	gethrtimef = pops->psm_gethrtime;

	/* register the interrupt setup code				*/
	slvltovect = mach_softlvl_to_vect;
	addspl	= pops->psm_addspl;
	delspl	= pops->psm_delspl;

	if (pops->psm_translate_irq)
		psm_translate_irq = pops->psm_translate_irq;
	/*
	 * This check is temporary.  When m48t18 support is deliver
	 * in PS, this check should be taken out.
	 */
	if ((! pops->psm_tod_get) && (! pops->psm_tod_set))
		m48t18_check();
	else {
		if (pops->psm_tod_get) {
			todgetf = mach_tod_get;
			psm_todgetf = pops->psm_tod_get;
		}
		if (pops->psm_tod_set) {
			todsetf = mach_tod_set;
			psm_todsetf = pops->psm_tod_set;
		}
	}
	if (pops->psm_notify_error) {
		psm_notify_error = mach_notify_error;
		notify_error = pops->psm_notify_error;
	}

	(*pops->psm_softinit)();

	mach_smpinit();
}

static void
mach_smpinit(void)
{
	register struct psm_ops  *pops;
	register processorid_t cpu_id;
	int	 cnt;
	int	 cpumask;

	pops = mach_set[0];

	for (cpu_id = -1, cnt = 0, cpumask = 0; cnt >= 0; cnt++) {
		cpu_id = (*pops->psm_get_next_processorid)(cpu_id);
		if (cpu_id == -1)
			break;
		cpumask |= 1 << cpu_id;
	}

	/* check for multiple cpu's					*/
	if (cnt < 2)
		return;

	/* check for MP platforms					*/
#if MP
	if (pops->psm_cpu_start == NULL)
		return;

	mp_cpus = cpumask;

	/* MP related routines						*/
	cpu_startf = mach_cpu_start;
	ap_mlsetup = pops->psm_post_cpu_start;
	send_dirintf = pops->psm_send_ipi;

	/* optional MP related routines					*/
	if (pops->psm_shutdown)
		psm_shutdownf = pops->psm_shutdown;
	if (pops->psm_set_idlecpu)
		psm_set_idle_cpuf = pops->psm_set_idlecpu;
	if (pops->psm_unset_idlecpu)
		psm_unset_idle_cpuf = pops->psm_unset_idlecpu;
	if (pops->psm_disable_intr)
		psm_disable_intr = pops->psm_disable_intr;
	if (pops->psm_enable_intr)
		psm_enable_intr  = pops->psm_enable_intr;

	(void) add_avintr((void *)NULL, XC_HI_PIL, xc_serv, "xc_hi_intr",
		(*pops->psm_get_ipivect)(XC_HI_PIL, PSM_INTR_IPI_HI),
		(caddr_t)X_CALL_HIPRI, 0);
	(void) add_avintr((void *)NULL, XC_MED_PIL, xc_serv, "xc_med_intr",
		(*pops->psm_get_ipivect)(XC_MED_PIL, PSM_INTR_IPI_LO),
		(caddr_t)X_CALL_MEDPRI, 0);
	(void) (*pops->psm_get_ipivect)(XC_CPUPOKE_PIL, PSM_INTR_POKE);
#endif
}

static void
mach_picinit()
{
	register struct psm_ops  *pops;

	pops = mach_set[0];

	/* register the interrupt handlers				*/
	setlvl = pops->psm_intr_enter;
	setlvlx = pops->psm_intr_exit;

	/* initialize the interrupt hardware				*/
	(*pops->psm_picinit)();

	/* set interrupt mask for current ipl 				*/
	setspl = pops->psm_setspl;
	setspl(CPU->cpu_pri);
}

static void
mach_clkinit()
{
	register struct psm_ops  *pops;

	pops = mach_set[0];
	clock_vector = (*pops->psm_get_clockirq)(CLOCK_LEVEL);
	clksetup();
	if (pops->psm_hrtimeinit)
		(*pops->psm_hrtimeinit)();
	(*pops->psm_clkinit)(hz);
}

static int
mach_softlvl_to_vect(register int ipl)
{
	register int softvect;
	register struct psm_ops  *pops;

	pops = mach_set[0];

	/* check for null handler for set soft interrupt call		*/
	if (pops->psm_set_softintr == NULL) {
		setsoftint = set_pending;
		return (PSM_SV_SOFTWARE);
	}

	softvect = (*pops->psm_softlvl_to_irq)(ipl);
	/* check for hardware scheme					*/
	if (softvect > PSM_SV_SOFTWARE) {
		setsoftint = pops->psm_set_softintr;
		return (softvect);
	}

	if (softvect == PSM_SV_SOFTWARE)
		setsoftint = set_pending;
	else	/* hardware and software mixed scheme			*/
		setsoftint = mach_set_softintr;

	return (PSM_SV_SOFTWARE);
}

static void
mach_set_softintr(register int ipl)
{
	register struct psm_ops  *pops;

	/* set software pending bits					*/
	set_pending(ipl);

	/*	check if dosoftint will be called at the end of intr	*/
	if ((CPU->cpu_on_intr) || (curthread->t_intr))
		return;

	/* invoke hardware interrupt					*/
	pops = mach_set[0];
	(*pops->psm_set_softintr)(ipl);
}

#if MP
static void
mach_cpu_start(register int cpun)
{
	register struct psm_ops  *pops;
	int	i;

	pops = mach_set[0];

	(*pops->psm_cpu_start)(cpun, rm_platter_va);

	/* wait for the auxillary cpu to be ready			*/
	for (i = 20000; i; i--) {
		if (cpu[cpun]->cpu_flags & CPU_READY)
			return;
		drv_usecwait(100);
	}
}
#endif

static int
/* LINTED: first argument dip is not used */
mach_translate_irq(dev_info_t *dip, register int irqno)
{
	return (irqno);		/* default to NO translation */
}

static timestruc_t
mach_tod_get(void)
{
	timestruc_t ts;
	todinfo_t tod;

	ASSERT(MUTEX_HELD(&tod_lock));

	/* The year returned from is the last 2 digit only */
	if ((*psm_todgetf)(&tod)) {
		ts.tv_sec = 0;
		ts.tv_nsec = 0;
		return (ts);
	}

	/* assume that we wrap the rtc year back to zero at 2000 */
	if (tod.tod_year < 70)
		tod.tod_year += 100;

	/* tod_to_utc uses 1900 as base for the year */
	ts.tv_sec = tod_to_utc(tod) + gmt_lag;
	ts.tv_nsec = 0;

	return (ts);
}

static void
mach_tod_set(timestruc_t ts)
{
	todinfo_t tod = utc_to_tod(ts.tv_sec - gmt_lag);

	ASSERT(MUTEX_HELD(&tod_lock));

	if (tod.tod_year >= 100)
		tod.tod_year -= 100;

	(*psm_todsetf)(&tod);
}

static void
mach_notify_error(int level, char *errmsg)
{
	/*
	 * SL_FATAL is pass in once panicstr is set, deliver it
	 * as CE_PANIC.  Also, translate SL_ codes back to CE_
	 * codes for the psmi handler
	 */
	if (level & SL_FATAL)
		(*notify_error)(CE_PANIC, errmsg);
	else if (level & SL_WARN)
		(*notify_error)(CE_WARN, errmsg);
	else if (level & SL_NOTE)
		(*notify_error)(CE_NOTE, errmsg);
	else if (level & SL_CONSOLE)
		(*notify_error)(CE_CONT, errmsg);
}

/*
 * All the codes below are temporary.  When m48t18 support is deliver
 * in PS, the codes below should be deleted.
 */

/*
 *
 * tod_get/tod_set functions for SGS M48T18 rtc/nvram chip.
 *
 */
#define	RTC_ADDR_PROBE0		0x74
#define	RTC_ADDR_PROBE1		0x75
#define	RTC_CTRL_ADDR		0x1ff8
#define	RTC_DATA_ADDR_SEC	0x1ff9
#define	RTC_DATA_ADDR_MIN	0x1ffa
#define	RTC_DATA_ADDR_HOUR	0x1ffb
#define	RTC_DATA_ADDR_DAY	0x1ffc
#define	RTC_DATA_ADDR_DATE	0x1ffd
#define	RTC_DATA_ADDR_MONTH	0x1ffe
#define	RTC_DATA_ADDR_YEAR	0x1fff
#define	RTC_DATA_PORT		0x77
#define	RTC_PROBE(addr) \
	outb(RTC_ADDR_PROBE0, (u_long)addr & 0xff); \
	outb(RTC_ADDR_PROBE1, ((u_long)addr & 0xff00) >> 8);
#define	RTC_READ_DATA() 	inb(RTC_DATA_PORT)
#define	RTC_WRITE_DATA(val) 	outb(RTC_DATA_PORT, (u_char)(val))

#define	RTC_CTRL_READ	0x40
#define	RTC_CTRL_WRITE	0x80


static void
m48t18_check(void)
{
	timestruc_t ts1, ts2;

	/*
	 * Switch RTC tod_get/tod_put functions if the standard
	 * pc_tod_get/pc_tod_set functions do not work. There
	 * is no simple probing method that can find out which RTC
	 * we have. One simple check used here is to check the return
	 * values from tod_get() and if the values do not appear to
	 * increment after a second or so, then assume it is not the
	 * standard RTC (DS1385) and switch the functions
	 * to m4818_tod_get/m48t18_tod_set.  If those functions don't
	 * work, we're probably hosed.
	 */
	mutex_enter(&tod_lock);
	ts1 = pc_tod_get();
	usec_delay(1500000); /* wait for 1.5 sec */
	ts2 = pc_tod_get();
	if (ts2.tv_sec != ts1.tv_sec + 1 && ts2.tv_sec != ts1.tv_sec + 2) {
		extern timestruc_t m48t18_tod_get(void);
		extern void m48t18_tod_set(timestruc_t);

		/* No change in time value, assume we have M48T18 device */
		todgetf = m48t18_tod_get;
		todsetf = m48t18_tod_set;
	}
	mutex_exit(&tod_lock);
}

/*
 * Read the current time from the clock chip and convert to UNIX form.
 * Assumes that the year in the clock chip is valid.
 * Must be called with tod_lock held.
 */
timestruc_t
m48t18_tod_get(void)
{
	timestruc_t ts;
	todinfo_t tod;

	ASSERT(MUTEX_HELD(&tod_lock));

	/* set READ mode in the control register */
	RTC_PROBE(RTC_CTRL_ADDR);
	RTC_WRITE_DATA(RTC_CTRL_READ);

	RTC_PROBE(RTC_DATA_ADDR_YEAR);
	tod.tod_year	= BCD_TO_BYTE(RTC_READ_DATA());
	if (tod.tod_year < 70)
		tod.tod_year += 100;

	RTC_PROBE(RTC_DATA_ADDR_MONTH);
	tod.tod_month	= BCD_TO_BYTE(RTC_READ_DATA() & 0x1f);
	RTC_PROBE(RTC_DATA_ADDR_DATE);
	tod.tod_day	= BCD_TO_BYTE(RTC_READ_DATA() & 0x3f);
	RTC_PROBE(RTC_DATA_ADDR_DAY);
	tod.tod_dow	= BCD_TO_BYTE(RTC_READ_DATA() & 0x7);
	RTC_PROBE(RTC_DATA_ADDR_HOUR);
	tod.tod_hour	= BCD_TO_BYTE(RTC_READ_DATA() & 0x3f);
	RTC_PROBE(RTC_DATA_ADDR_MIN);
	tod.tod_min	= BCD_TO_BYTE(RTC_READ_DATA() & 0x7f);
	RTC_PROBE(RTC_DATA_ADDR_SEC);
	tod.tod_sec	= BCD_TO_BYTE(RTC_READ_DATA() & 0x7f);

	/* reset READ mode in the control register */
	RTC_PROBE(RTC_CTRL_ADDR);
	RTC_WRITE_DATA(0);

	ts.tv_sec = tod_to_utc(tod);
	ts.tv_nsec = 0;

	return (ts);
}

/*
 * Write the specified time into the clock chip.
 * Must be called with tod_lock held.
 */
void
m48t18_tod_set(timestruc_t ts)
{
	todinfo_t tod = utc_to_tod(ts.tv_sec);

	ASSERT(MUTEX_HELD(&tod_lock));

	/* set WRITE mode in the control register */
	RTC_PROBE(RTC_CTRL_ADDR);
	RTC_WRITE_DATA(RTC_CTRL_WRITE);

	if (tod.tod_year >= 100)
		tod.tod_year -= 100;

	RTC_PROBE(RTC_DATA_ADDR_YEAR);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_year));
	RTC_PROBE(RTC_DATA_ADDR_MONTH);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_month));
	RTC_PROBE(RTC_DATA_ADDR_DATE);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_day));
	RTC_PROBE(RTC_DATA_ADDR_DAY);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_dow));
	RTC_PROBE(RTC_DATA_ADDR_HOUR);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_hour));
	RTC_PROBE(RTC_DATA_ADDR_MIN);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_min));
	RTC_PROBE(RTC_DATA_ADDR_SEC);
	RTC_WRITE_DATA(BYTE_TO_BCD(tod.tod_sec));

	/* reset WRITE mode in the control register */
	RTC_PROBE(RTC_CTRL_ADDR);
	RTC_WRITE_DATA(0);
}
