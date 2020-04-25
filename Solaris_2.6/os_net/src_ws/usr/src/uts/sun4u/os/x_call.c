
/*
 * Copyright (c) 1994, by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)x_call.c	1.45	96/08/15 SMI"

#include <sys/systm.h>
#include <sys/cpuvar.h>
#include <sys/intreg.h>
#include <sys/x_call.h>
#include <sys/cmn_err.h>
#include <sys/membar.h>
#include <sys/disp.h>
#include <sys/debug.h>
#include <sys/privregs.h>
#include <sys/xc_impl.h>

#ifdef DEBUG
u_int	x_dstat[NCPU][XC_LOOP_EXIT+1];
u_int	x_rstat[NCPU][4];
#endif DEBUG

u_longlong_t xc_tick_limit;		/* send_mondo() tick limit value */

/*
 * xc_init()
 *	initialize x-call related locks
 */
void
xc_init(void)
{
#ifdef DEBUG
	int pix;
#endif DEBUG
	extern int add_softintr();

	mutex_init(&xc_sys_mutex, "xc sys lock", MUTEX_SPIN,
	    (void *)ipltospl(XCALL_PIL));

#ifdef DEBUG
	for (pix = 0; pix < NCPU; pix++) {
		if (cpunodes[pix].nodeid) {
			XC_STAT_INIT(pix);
		};
	}
#endif DEBUG

	xc_serv_inum = add_softintr(XCALL_PIL, xc_serv, 0, 0);
	xc_loop_inum = add_softintr(XCALL_PIL, xc_loop, 0, 0);

	/*
	 * Initialize the calibrated tick limit for send_mondo.
	 * The value represents the maximum tick count to wait.
	 */
	xc_tick_limit = ((u_longlong_t)cpu_tick_freq * XC_SEND_MONDO_MSEC) /
			1000;
}

/*
 * The following routines basically provide callers with two kinds of
 * inter-processor interrupt services:
 *	1. cross calls (x-calls) - requests are handled at target cpu's TL=0
 *	2. cross traps (c-traps) - requests are handled at target cpu's TL>0
 *
 * Although these routines protect the services from migrating to other cpus
 * "after" they are called, it is the caller's choice or responsibility to
 * prevent the cpu migration "before" calling them.
 *
 * X-call routines:
 *
 *	xc_one()  - send a request to one processor
 *	xc_some() - send a request to some processors
 *	xc_all()  - send a request to all processors
 *
 *	Their common parameters:
 *		func - a TL=0 handler address
 *		arg1 and arg2  - optional
 *
 *	The services provided by x-call routines allow callers
 *	to send a request to target cpus to execute a TL=0
 *	handler.
 *	The interface of the registers of the TL=0 handler:
 *		%o0: arg1
 *		%o1: arg2
 *
 * X-trap routines:
 *
 *	xt_one()  - send a request to one processor
 *	xt_some() - send a request to some processors
 *	xt_all()  - send a request to all processors
 *
 *	Their common parameters:
 *		func - a TL>0 handler address or an interrupt number
 *		arg1, arg2, arg3, and arg4 -
 *		       optional when "func" is an address;
 *		       0        when "func" is an interrupt number
 *
 *	If the request of "func" is a kernel address, then
 *	the target cpu will execute the request of "func" with
 *	args at "TL>0" level.
 *	The interface of the registers of the TL>0 handler:
 *		%g1: arg1
 *		%g2: arg2
 *		%g3: arg3
 *		%g4: arg4
 *
 *
 *	If the request of "func" is not a kernel address, then it has
 *	to be an assigned interrupt number through add_softintr().
 *	An interrupt number is an index to the interrupt vector table,
 *	which entry contains an interrupt handler address with its
 *	corresponding interrupt level and argument.
 *	The target cpu will arrange the request to be serviced according
 *	to its pre-registered information.
 *	args are assumed to be zeros in this case.
 *
 * In addition, callers are allowed to capture and release cpus by
 * calling the routines: xc_attention() and xc_dismissed().
 */

/*
 * xt_one()
 *	send a "x-trap" to a cpu
 */
void
xt_one(int cix, u_int func, u_int arg1, u_int arg2, u_int arg3, u_int arg4)
{
	int lcx;
	int opl;
	cpuset_t tset;

	/*
	 * send to nobody; just return
	 */
	if (!CPU_IN_SET(cpu_ready_set, cix))
		return;

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);			/* lcx set by the macro */

	CPUSET_ADD(tset, cix);

	if (cix == lcx) {
		/*
		 * same cpu - use software fast trap
		 */
		send_self_xcall(CPU, arg1, arg2, arg3, arg4, func);
		XC_STAT_INC(x_dstat[lcx][XT_ONE_SELF]);
		XC_TRACE(XT_ONE_SELF, tset, func, arg1, arg2, arg3);
	} else {	/* other cpu - send a mondo to the target cpu */
		/*
		 * other cpu - send a mondo to the target cpu
		 */
		XC_TRACE(XT_ONE_OTHER, tset, func, arg1, arg2, arg3);
		init_mondo(func, arg1, arg2, arg3, arg4);
		send_mondo(cix);
		fini_mondo();
		XC_STAT_INC(x_dstat[lcx][XT_ONE_OTHER]);
	}
	XC_SPL_EXIT(lcx, opl);
}

/*
 * xt_some()
 *	send a "x-trap" to some cpus
 */
void
xt_some(cpuset_t cpuset, u_int func, u_int arg1, u_int arg2,
    u_int arg3, u_int arg4)
{
	int lcx;
	int opl;
	cpuset_t xc_cpuset, tset;

	/*
	 * send to nobody; just return
	 */
	if (CPUSET_ISNULL(cpuset))
		return;

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);		/* lcx set by the macro */

	CPUSET_ADD(tset, lcx);

	/*
	 * only send to the CPU_READY ones
	 */
	xc_cpuset = cpu_ready_set;
	CPUSET_AND(xc_cpuset, cpuset);

	/*
	 * don't send mondo to self
	 */
	if (CPU_IN_SET(xc_cpuset, lcx)) {
		/*
		 * same cpu - use software fast trap
		 */
		send_self_xcall(CPU, arg1, arg2, arg3, arg4, func);
		XC_STAT_INC(x_dstat[lcx][XT_SOME_SELF]);
		XC_TRACE(XT_SOME_SELF, tset, func, arg1, arg2, arg3);
		CPUSET_DEL(xc_cpuset, lcx);
		if (CPUSET_ISNULL(xc_cpuset)) {
			XC_SPL_EXIT(lcx, opl);
			return;
		}
	}
	XC_TRACE(XT_SOME_OTHER, xc_cpuset, func, arg1, arg2, arg3);
	init_mondo(func, arg1, arg2, arg3, arg4);
	SEND_MONDO_ONLY(xc_cpuset);
	fini_mondo();
	XC_STAT_INC(x_dstat[lcx][XT_SOME_OTHER]);

	XC_SPL_EXIT(lcx, opl);
}

/*
 * xt_all()
 *	send a "x-trap" to all cpus
 */
void
xt_all(u_int func, u_int arg1, u_int arg2, u_int arg3, u_int arg4)
{
	int lcx;
	int opl;
	cpuset_t xc_cpuset, tset;

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);		/* lcx set by the macro */

	CPUSET_ADD(tset, lcx);

	/*
	 * same cpu - use software fast trap
	 */
	if (CPU_IN_SET(cpu_ready_set, lcx))
		send_self_xcall(CPU, arg1, arg2, arg3, arg4, func);

	XC_TRACE(XT_ALL_OTHER, cpu_ready_set, func, arg1, arg2, arg3);

	/*
	 * don't send mondo to self
	 */
	xc_cpuset = cpu_ready_set;
	CPUSET_DEL(xc_cpuset, lcx);

	if (CPUSET_ISNULL(xc_cpuset)) {
		XC_STAT_INC(x_dstat[lcx][XT_ALL_SELF]);
		XC_TRACE(XT_ALL_SELF, tset, func, arg1, arg2, arg3);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	init_mondo(func, arg1, arg2, arg3, arg4);
	SEND_MONDO_ONLY(xc_cpuset);
	fini_mondo();

	XC_STAT_INC(x_dstat[lcx][XT_ALL_OTHER]);
	XC_SPL_EXIT(lcx, opl);
}

/*
 * xt_sync()
 *	wait for previous x-traps to finish
 *
 */
void
xt_sync(cpuset_t cpuset)
{
	extern void xt_sync_tl1();

	kpreempt_disable();
	CPUSET_DEL(cpuset, CPU->cpu_id);
	CPUSET_AND(cpuset, cpu_ready_set);
	xt_some(cpuset, (u_int)xt_sync_tl1, 0, 0, 0, 0);
	kpreempt_enable();
}

/*
 * xc_one()
 *	send a "x-call" to a cpu
 */
void
xc_one(int cix, u_int (*func)(), u_int arg1, u_int arg2)
{
	int lcx;
	int opl;
	int loop_cnt = 0;
	cpuset_t tset;

	/*
	 * send to nobody; just return
	 */
	if (!CPU_IN_SET(cpu_ready_set, cix))
		return;

	ASSERT((u_int)func > KERNELBASE);
	ASSERT(((u_int)func % PC_ALIGN) == 0);

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);		/* lcx set by the macro */

	CPUSET_ADD(tset, cix);

	if (cix == lcx) {	/* same cpu just do it */
		XC_TRACE(XC_ONE_SELF, tset, (u_int)func, arg1, arg2, NULL);
		(*func)(arg1, arg2);
		XC_STAT_INC(x_dstat[lcx][XC_ONE_SELF]);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	if (xc_holder == lcx) {		/* got the xc_sys_mutex already */
		ASSERT(MUTEX_HELD(&xc_sys_mutex));
		ASSERT(CPU_IN_SET(xc_mbox[lcx].xc_cpuset, lcx));
		ASSERT(CPU_IN_SET(xc_mbox[cix].xc_cpuset, cix));
		ASSERT(xc_mbox[cix].xc_state == XC_WAIT);
		XC_TRACE(XC_ONE_OTHER_H, tset, (u_int)func,
			arg1, arg2, NULL);
		/*
		 * target processor's xc_loop should be waiting
		 * for the work to do; just set up the xc_mbox
		 */
		XC_SETUP(cix, func, arg1, arg2);
		membar_stld();

		while (xc_mbox[cix].xc_state != XC_WAIT) {
			if (loop_cnt++ > XC_FUNCTIME) {
				panic("xc_one() timeout, xc_state[%d] != "
					"XC_WAIT", cix);
			}
		}
		XC_STAT_INC(x_dstat[lcx][XC_ONE_OTHER_H]);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	/*
	 * Avoid dead lock if someone has sent us a xc_loop request while
	 * we are trying to grab xc_sys_mutex.
	 */
	XC_SPL_EXIT(lcx, opl);

	/*
	 * At this point, since we don't own xc_sys_mutex,
	 * our pil shouldn't run at or above the XCALL_PIL.
	 */
	ASSERT(getpil() < XCALL_PIL);

	/*
	 * Since xc_holder is not owned by us, it could be that
	 * no one owns it, or we are not informed to enter into
	 * xc_loop(). In either case, we need to grab the
	 * xc_sys_mutex before we write to the xc_mbox, and
	 * we shouldn't release it until the request is finished.
	 */

	mutex_enter(&xc_sys_mutex);
	xc_spl_enter[lcx] = 1;

	/*
	 * Since we own xc_sys_mutex now, we are safe to
	 * write to the xc_mobx.
	 */
	ASSERT(xc_mbox[cix].xc_state == XC_IDLE);
	XC_TRACE(XC_ONE_OTHER, tset, (u_int)func, arg1, arg2, NULL);
	XC_SETUP(cix, func, arg1, arg2);
	init_mondo((u_int)xc_serv_inum, 0, 0, 0, 0);
	send_mondo(cix);		/* does membar_sync */
	fini_mondo();

	/* xc_serv does membar_stld */
	while (xc_mbox[cix].xc_state != XC_IDLE) {
		if (loop_cnt++ > XC_FUNCTIME)
			panic("xc_one() timeout, xc_state[%d] != XC_IDLE", cix);
	}
	xc_spl_enter[lcx] = 0;
	XC_STAT_INC(x_dstat[lcx][XC_ONE_OTHER]);
	mutex_exit(&xc_sys_mutex);

}
/*
 * xc_some()
 *	send a "x-call" to some cpus; sending to self is excluded
 */
void
xc_some(cpuset_t cpuset, u_int (*func)(), u_int arg1, u_int arg2)
{
	int lcx;
	int opl;
	cpuset_t xc_cpuset, tset;

	/*
	 * send to nobody; just return
	 */
	if (CPUSET_ISNULL(cpuset))
		return;

	ASSERT((u_int)func > KERNELBASE);
	ASSERT(((u_int)func % PC_ALIGN) == 0);

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);			/* lcx set by the macro */

	CPUSET_ADD(tset, lcx);

	/*
	 * only send to the CPU_READY ones
	 */
	xc_cpuset = cpu_ready_set;
	CPUSET_AND(xc_cpuset, cpuset);

	if (CPU_IN_SET(xc_cpuset, lcx)) {
		/*
		 * same cpu just do it
		 */
		(*func)(arg1, arg2);
		CPUSET_DEL(xc_cpuset, lcx);
		if (CPUSET_ISNULL(xc_cpuset)) {
			XC_STAT_INC(x_dstat[lcx][XC_SOME_SELF]);
			XC_TRACE(XC_SOME_SELF, tset, (u_int)func,
				arg1, arg2, NULL);
			XC_SPL_EXIT(lcx, opl);
			return;
		}
	}

	if (xc_holder == lcx) {		/* got the xc_sys_mutex already */
		cpuset_t mset = xc_mbox[lcx].xc_cpuset;

		CPUSET_AND(mset, cpuset);
		ASSERT(MUTEX_HELD(&xc_sys_mutex));
		ASSERT(CPUSET_ISEQUAL(mset, cpuset));
		SEND_MBOX_ONLY(xc_cpuset, func, arg1, arg2, lcx, XC_WAIT);
		WAIT_MBOX_DONE(xc_cpuset, lcx, XC_WAIT);
		XC_STAT_INC(x_dstat[lcx][XC_SOME_OTHER_H]);
		XC_TRACE(XC_SOME_OTHER_H, xc_cpuset, (u_int)func, arg1,
			arg2, NULL);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	/*
	 * Avoid dead lock if someone has sent us a xc_loop request while
	 * we are trying to grab xc_sys_mutex.
	 */
	XC_SPL_EXIT(lcx, opl);

	/*
	 * At this point, since we don't own xc_sys_mutex,
	 * our pil shouldn't run at or above the XCALL_PIL.
	 */
	ASSERT(getpil() < XCALL_PIL);

	/*
	 * grab xc_sys_mutex before writing to the xc_mbox
	 */
	mutex_enter(&xc_sys_mutex);
	xc_spl_enter[lcx] = 1;

	XC_TRACE(XC_SOME_OTHER, xc_cpuset, (u_int)func, arg1, arg2, NULL);
	init_mondo(xc_serv_inum, 0, 0, 0, 0);
	SEND_MBOX_MONDO(xc_cpuset, func, arg1, arg2, XC_IDLE);
	fini_mondo();
	WAIT_MBOX_DONE(xc_cpuset, lcx, XC_IDLE);

	xc_spl_enter[lcx] = 0;
	XC_STAT_INC(x_dstat[lcx][XC_SOME_OTHER]);
	mutex_exit(&xc_sys_mutex);
}
/*
 * xc_all()
 *	send a "x-call" to all cpus
 */
void
xc_all(u_int (*func)(), u_int arg1, u_int arg2)
{
	int lcx;
	int opl;
	cpuset_t xc_cpuset, tset;

	ASSERT((u_int)func > KERNELBASE);
	ASSERT(((u_int)func % PC_ALIGN) == 0);

	CPUSET_ZERO(tset);

	XC_SPL_ENTER(lcx, opl);			/* lcx set by the macro */

	CPUSET_ADD(tset, lcx);

	/*
	 * same cpu just do it
	 */
	(*func)(arg1, arg2);
	xc_cpuset = cpu_ready_set;
	CPUSET_DEL(xc_cpuset, lcx);

	if (CPUSET_ISNULL(xc_cpuset)) {
		XC_STAT_INC(x_dstat[lcx][XC_ALL_SELF]);
		XC_TRACE(XC_ALL_SELF, tset, (u_int)func, arg1,
			arg2, NULL);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	if (xc_holder == lcx) {		/* got the xc_sys_mutex already */
		cpuset_t mset = xc_mbox[lcx].xc_cpuset;

		CPUSET_AND(mset, xc_cpuset);
		ASSERT(MUTEX_HELD(&xc_sys_mutex));
		ASSERT(CPUSET_ISEQUAL(mset, xc_cpuset));
		XC_TRACE(XC_ALL_OTHER_H, xc_cpuset, (u_int)func, arg1,
			arg2, NULL);
		SEND_MBOX_ONLY(xc_cpuset, func, arg1, arg2, lcx, XC_WAIT);
		WAIT_MBOX_DONE(xc_cpuset, lcx, XC_WAIT);
		XC_STAT_INC(x_dstat[lcx][XC_ALL_OTHER_H]);
		XC_SPL_EXIT(lcx, opl);
		return;
	}

	/*
	 * Avoid dead lock if someone has sent us a xc_loop request while
	 * we are trying to grab xc_sys_mutex.
	 */
	XC_SPL_EXIT(lcx, opl);

	/*
	 * At this point, since we don't own xc_sys_mutex,
	 * our pil shouldn't run at or above the XCALL_PIL.
	 */
	ASSERT(getpil() < XCALL_PIL);

	/*
	 * grab xc_sys_mutex before writing to the xc_mbox
	 */
	mutex_enter(&xc_sys_mutex);
	xc_spl_enter[lcx] = 1;

	XC_TRACE(XC_ALL_OTHER, xc_cpuset, (u_int)func, arg1, arg2, NULL);
	init_mondo(xc_serv_inum, 0, 0, 0, 0);
	SEND_MBOX_MONDO(xc_cpuset, func, arg1, arg2, XC_IDLE);
	fini_mondo();
	WAIT_MBOX_DONE(xc_cpuset, lcx, XC_IDLE);

	xc_spl_enter[lcx] = 0;
	XC_STAT_INC(x_dstat[lcx][XC_ALL_OTHER]);
	mutex_exit(&xc_sys_mutex);
}
/*
 * xc_attention()
 *	paired with xc_dismissed()
 *	xt_attention() holds the xc_sys_mutex and xc_dismissed() releases it
 *	called when an initiator wants to capture some/all cpus for a critical
 *	session
 */
void
xc_attention(cpuset_t cpuset)
{
	int pix, lcx;
	cpuset_t xc_cpuset, tmpset;
	cpuset_t recv_cpuset;
	int loop_cnt = 0;

	CPUSET_ZERO(recv_cpuset);

	/*
	 * don't migrate the cpu until xc_dismissed() is finished
	 */
	ASSERT(getpil() < XCALL_PIL);
	mutex_enter(&xc_sys_mutex);
	lcx = (int)(CPU->cpu_id);
	ASSERT(x_dstat[lcx][XC_ATTENTION] ==
	    x_dstat[lcx][XC_DISMISSED]);
	ASSERT(xc_holder == -1);
	xc_mbox[lcx].xc_cpuset = cpuset;
	xc_holder = lcx; /* no membar; only current cpu needs the right lcx */

	/*
	 * only send to the CPU_READY ones
	 */
	xc_cpuset = cpu_ready_set;
	CPUSET_AND(xc_cpuset, cpuset);

	/*
	 * don't send mondo to self
	 */
	CPUSET_DEL(xc_cpuset, lcx);

	XC_STAT_INC(x_dstat[lcx][XC_ATTENTION]);
	XC_TRACE(XC_ATTENTION, xc_cpuset, NULL, NULL, NULL, NULL);

	if (CPUSET_ISNULL(xc_cpuset))
		return;

	xc_spl_enter[lcx] = 1;
	/*
	 * inform the target processors to enter into xc_loop()
	 */
	tmpset = xc_cpuset;
	init_mondo(xc_loop_inum, 0, 0, 0, 0);
	for (pix = 0; pix < NCPU; pix++) {
		if (CPU_IN_SET(tmpset, pix)) {
			ASSERT(xc_mbox[pix].xc_state == XC_IDLE);
			xc_mbox[pix].xc_state = XC_ENTER;
			send_mondo(pix);	/* does membar_sync */
			CPUSET_DEL(tmpset, pix);
			CPU_STAT_ADDQ(CPU, cpu_sysinfo.xcalls, 1);
			if (CPUSET_ISNULL(tmpset)) {
				break;
			}
		}
	}
	fini_mondo();
	xc_spl_enter[lcx] = 0;

	/*
	 * make sure target processors have entered into xc_loop()
	 */
	while (!CPUSET_ISEQUAL(recv_cpuset, xc_cpuset)) {
		tmpset = xc_cpuset;
		for (pix = 0; pix < NCPU; pix++) {
			if (CPU_IN_SET(tmpset, pix)) {
				/*
				 * membar_stld() is done in xc_loop
				 */
				if (xc_mbox[pix].xc_state == XC_WAIT) {
					CPUSET_ADD(recv_cpuset, pix);
				}
				CPUSET_DEL(tmpset, pix);
				if (CPUSET_ISNULL(tmpset)) {
					break;
				}
			}
		}
		if (loop_cnt++ > XC_MONDOTIME) {
			panic("xc_attention() timeout, recv_cpuset 0x%x, "
				"xc_cpuset 0x%x", recv_cpuset, xc_cpuset);
		}
	}

	/*
	 * keep locking xc_sys_mutex until xc_dismissed() is finished
	 */
}

/*
 * xc_dismissed()
 *	paired with xc_attention()
 *	called after the critical session is finished
 */
void
xc_dismissed(cpuset_t cpuset)
{
	int pix;
	int lcx = (int)(CPU->cpu_id);
	cpuset_t xc_cpuset, tmpset;
	cpuset_t recv_cpuset;
	int loop_cnt = 0;

	ASSERT(lcx == xc_holder);
	ASSERT(CPUSET_ISEQUAL(xc_mbox[lcx].xc_cpuset, cpuset));
	ASSERT(getpil() >= XCALL_PIL);
	CPUSET_ZERO(xc_mbox[lcx].xc_cpuset);
	CPUSET_ZERO(recv_cpuset);
	membar_stld();

	XC_STAT_INC(x_dstat[lcx][XC_DISMISSED]);
	ASSERT(x_dstat[lcx][XC_DISMISSED] ==
	    x_dstat[lcx][XC_ATTENTION]);

	/*
	 * only send to the CPU_READY ones
	 */
	xc_cpuset = cpu_ready_set;
	CPUSET_AND(xc_cpuset, cpuset);

	/*
	 * exclude itself
	 */
	CPUSET_DEL(xc_cpuset, lcx);
	XC_TRACE(XC_DISMISSED, xc_cpuset, NULL, NULL, NULL, NULL);
	if (CPUSET_ISNULL(xc_cpuset)) {
		xc_holder = -1;
		mutex_exit(&xc_sys_mutex);
		return;
	}

	/*
	 * inform other processors to get out of xc_loop()
	 */
	tmpset = xc_cpuset;
	for (pix = 0; pix < NCPU; pix++) {
		if (CPU_IN_SET(tmpset, pix)) {
			xc_mbox[pix].xc_state = XC_EXIT;
			membar_stld();
			CPUSET_DEL(tmpset, pix);
			if (CPUSET_ISNULL(tmpset)) {
				break;
			}
		}
	}

	/*
	 * make sure target processors have exited from xc_loop()
	 */
	while (!CPUSET_ISEQUAL(recv_cpuset, xc_cpuset)) {
		tmpset = xc_cpuset;
		for (pix = 0; pix < NCPU; pix++) {
			if (CPU_IN_SET(tmpset, pix)) {
				/*
				 * membar_stld() is done in xc_loop
				 */
				if (xc_mbox[pix].xc_state == XC_IDLE) {
					CPUSET_ADD(recv_cpuset, pix);
				}
				CPUSET_DEL(tmpset, pix);
				if (CPUSET_ISNULL(tmpset)) {
					break;
				}
			}
		}
		if (loop_cnt++ > XC_FUNCTIME)
			panic("xc_dismissed() timeout, recv_cpuset 0x%x, "
				"xc_cpuset 0x%x", recv_cpuset, xc_cpuset);
	}
	xc_holder = -1;
	mutex_exit(&xc_sys_mutex);
}

/*
 * xc_serv()
 *	"x-call" handler at TL=0; serves only one x-call request
 *	runs at XCALL_PIL level
 */
int
xc_serv()
{
	int lcx = (int)(CPU->cpu_id);
	struct xc_mbox *xmp;
	u_int (*func)();
	u_int arg1, arg2;
	cpuset_t tset;

	ASSERT(getpil() == XCALL_PIL);
	CPUSET_ZERO(tset);
	CPUSET_ADD(tset, lcx);
	flush_windows();
	xmp = &xc_mbox[lcx];
	ASSERT(lcx != xc_holder);
	ASSERT(xmp->xc_state == XC_DOIT);
	func = xmp->xc_func;
	XC_TRACE(XC_SERV, tset, (u_int)func, xmp->xc_arg1,
	    xmp->xc_arg2, NULL);
	if (func != NULL) {
		arg1 = xmp->xc_arg1;
		arg2 = xmp->xc_arg2;
		(*func)(arg1, arg2);
	}
	XC_STAT_INC(x_rstat[lcx][XC_SERV]);
	XC_TRACE(XC_SERV, tset, (u_int)func, arg1, arg2, NULL);
	xmp->xc_state = XC_IDLE;
	membar_stld();
	return (1);
}

/*
 * if == 1, an xc_loop timeout will cause a panic
 * otherwise print a warning
 */
u_int	xc_loop_panic = 0;

/*
 * xc_loop()
 *	"x-call" handler at TL=0; capture the cpu for a critial session,
 *	or serve multiple x-call requests
 *	runs at XCALL_PIL level
 */
/*ARGSUSED0*/
int
xc_loop()
{
	int lcx = (int)(CPU->cpu_id);
	struct xc_mbox *xmp;
	u_int (*func)();
	u_int arg1, arg2;
	int loop_cnt = 0;
	cpuset_t tset;


	ASSERT(getpil() == XCALL_PIL);
	CPUSET_ZERO(tset);
	flush_windows();
	/*
	 * Some one must have owned the xc_sys_mutex;
	 * no further interrupt (at XCALL_PIL or below) can
	 * be taken by this processor until xc_loop exits.
	 *
	 * The owner of xc_sys_mutex (or xc_holder) can expect
	 * its xc/xt requests are handled as follows:
	 * 	xc requests use xc_mbox's handshaking for their services
	 * 	xt requests at TL>0 will be handled immediately
	 * 	xt requests at TL=0:
	 *		if their handlers'pils are <= XCALL_PIL, then
	 *			they will be handled after xc_loop exits
	 *			(so, they probably should not be used)
	 *		else they will be handled immediately
	 *
	 * For those who are not informed to enter xc_loop, if they
	 * send xc/xt requests to this processor at this moment,
	 * the requests will be handled as follows:
	 *	xc requests will be handled after they grab xc_sys_mutex
	 *	xt requests at TL>0 will be handled immediately
	 * 	xt requests at TL=0:
	 *		if their handlers'pils are <= XCALL_PIL, then
	 *			they will be handled after xc_loop exits
	 *		else they will be handled immediately
	 */
	xmp = &xc_mbox[lcx];
	ASSERT(lcx != xc_holder);
	ASSERT(xmp->xc_state == XC_ENTER);
	xmp->xc_state = XC_WAIT;
	CPUSET_ADD(tset, lcx);
	membar_stld();
	XC_STAT_INC(x_rstat[lcx][XC_LOOP]);
	XC_TRACE(XC_LOOP_ENTER, tset, NULL, NULL, NULL, NULL);
	while (xmp->xc_state != XC_EXIT) {
		if (xmp->xc_state == XC_DOIT) {
			func = xmp->xc_func;
			arg1 = xmp->xc_arg1;
			arg2 = xmp->xc_arg2;
			XC_TRACE(XC_LOOP_DOIT, tset, (u_int)func,
				arg1, arg2, NULL);
			if (func != NULL) {
				(*func)(arg1, arg2);
			} else {
				/* EMPTY */
			}
			xmp->xc_state = XC_WAIT;
			membar_stld();
			/*
			 * reset the timeout counter
			 * since some work was done
			 */
			loop_cnt = 0;
		} else {
			/*
			 * patience is a virtue...
			 */
			loop_cnt++;
		}
		if (loop_cnt > XC_FUNCTIME) {
			cmn_err(xc_loop_panic ? CE_PANIC : CE_WARN,
				"xc_loop() timeout");
			/*
			 * if the above displayed a warning,
			 * reset the timeout counter and be patient
			 */
			loop_cnt = 0;
		}
	}
	ASSERT(xmp->xc_state == XC_EXIT);
	ASSERT(xc_holder != -1);
	XC_TRACE(XC_LOOP_EXIT, tset, NULL, NULL, NULL, NULL);
	xmp->xc_state = XC_IDLE;
	membar_stld();
	return (1);
}
