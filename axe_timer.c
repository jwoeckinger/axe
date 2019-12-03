/*
** =============================================================================
** Copyright (C) 2000 COMNEON GmbH & Co. OHG. All rights reserved.
** =============================================================================
**
** =============================================================================
**
** This document contains proprietary information belonging to COMNEON.
** Passing on and copying of this document, use and communication of its
** contents is not permitted without prior written authorisation.
**
** =============================================================================
*/


#if !defined(_AXE_Types_h)
#include "axe_types.h"
#endif
#if !defined(_AXE_Timer_h)
#include "axe_timer.h"
#endif
#if !defined(_AXE_Applic_h)
#include "axe_applic.h"
#endif
#if !defined(_AXE_Os_h)
#include "axe_os.h"
#endif

#include "mmi_trap.h"

#ifdef EXTRA_TRAP_HANDLING

BOOLEAN FindTimer( AxeTimer *timer,AxeAppHdl app )
{
	AxeTimer *t = app->timer_list;
	while(t != 0 ) {
		if( t == timer )
			return TRUE;
		t = t->next;
	}
	return FALSE;
}

#endif
// ---------------------------------------------------------------
// AxeTimerInit
// ---------------------------------------------------------------
extern void AxeTimerInit(AxeTimer *timer,AxeAppHdl app,
						AxeTimerVal timeout,AxeTimeOutHdl hdl,
						BOOLEAN autotimer,void *userdata)
{
	ASSERT(timer != 0);

#ifdef EXTRA_TRAP_HANDLING
	if( FindTimer( timer, app ) )
	#ifdef TRAP_HANDLING
		MS_exit(AXE_TIMER_INIT_ALREADY_INIT);
	#else
		exit(0);               
	#endif

#endif
	timer->autotimer = autotimer;
	timer->active = FALSE;
	timer->expire = 0;
	timer->next = 0;
	timer->app = app;
	timer->timeout = timeout;
	timer->TimeOutHdl = hdl;
	timer->user_data = userdata;
}	// AxeTimerInit


// ---------------------------------------------------------------
// AxeTimerResume
// ---------------------------------------------------------------
extern void AxeTimerResume(AxeTimer *timer)
{
	AxeAppHdl app = timer->app;
	AxeTimer *prev = 0;
	AxeTimer *t;

	timer->stopped = FALSE;

	if (AxeTimerIsActive(timer))
		return;

	t = app->timer_list;
	while (t != 0 && t->expire < timer->expire) {
		prev = t;
		t = t->next;
		}

	timer->active = TRUE;
	timer->next = t;
	if (prev == 0) {
		app->timer_list = timer;
		AxeOsTriggerTimerEvent(app,timer->expire);
		}
	else
		prev->next = timer;
}	// AxeTimerResume


// ---------------------------------------------------------------
// AxeTimerIsActive
// ---------------------------------------------------------------
extern BOOLEAN AxeTimerIsActive(AxeTimer *timer)
{
	return(timer->active);
}	// AxeTimerIsActive


// ---------------------------------------------------------------
// AxeTimerStart
// ---------------------------------------------------------------
extern void AxeTimerStart(AxeTimer *timer)
{
	timer->expire = AxeOsGetTickCount() + timer->timeout;
	AxeTimerResume(timer);
}	// AxeTimerStart


// ---------------------------------------------------------------
// AxeTimerRestart
// ---------------------------------------------------------------
extern void AxeTimerRestart(AxeTimer *timer)
{
	if (AxeTimerIsActive(timer))
		AxeTimerStop(timer);
	AxeTimerStart(timer);
}	// AxeTimerRestart


// ---------------------------------------------------------------
// AxeTimerStop
// ---------------------------------------------------------------
extern void AxeTimerStop(AxeTimer *timer)
{
	AxeAppHdl app = timer->app;
	AxeTimer *t;
	AxeTimer *prev = 0;

	timer->stopped = TRUE;

	if (! AxeTimerIsActive(timer))
		return;

	t = app->timer_list;
	while (t != 0 && t != timer) {
		prev = t;
		t = t->next;
		}

	ASSERT(t != 0);

	if (t != 0) {
		if (prev == 0)
			app->timer_list = t->next;
		else
			prev->next = t->next;
		t->next = 0;
		}

	timer->active = FALSE;
}	// AxeTimerStop


