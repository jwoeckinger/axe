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

#if !defined(_AXE_Timer_h)
#define _AXE_Timer_h

#include "axe_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*----------------------------------------------------------------
							  Macros
  ----------------------------------------------------------------*/
#define AxeMilliSecToTicksPerSec(ms)	(AXE_TICKS_PER_SECOND * (ms) / 1000)
#define AxeTicksPerSecToMilliSec(tps)	((tps) * 1000 / AXE_TICKS_PER_SECOND)
/*----------------------------------------------------------------
							  Types
  ----------------------------------------------------------------*/

typedef UINT32 AxeTimerVal;

typedef void (*AxeTimeOutHdl)(AxeAppHdl app,AxeTimer *timer);

struct AxeTimerStruct {
 	UINT autotimer				: 1;	// autotimer will be restarted automatically
	UINT active					: 1;
	UINT stopped				: 1;
	AxeTimer *next;						// internal, init with 0
	AxeAppHdl app;
	AxeTimerVal timeout;				// timeout value in ticks (see AXE_TICKS_PER_SECOND)
	AxeTimerVal expire;					// internal, init with 0
	AxeTimeOutHdl TimeOutHdl;			// function will be called on timeout
	void *user_data;					// data to pass to the timeout handler
};


/*----------------------------------------------------------------
							 Prototypes
  ----------------------------------------------------------------*/

extern void AxeTimerInit(AxeTimer *timer,AxeAppHdl app,AxeTimerVal timeout,
						 AxeTimeOutHdl hdl,BOOLEAN autotimer,void *userdata);

extern BOOLEAN AxeTimerIsActive(AxeTimer *timer);
extern void AxeTimerStart(AxeTimer *timer);
extern void AxeTimerStop(AxeTimer *timer);
extern void AxeTimerResume(AxeTimer *timer);
extern void AxeTimerRestart(AxeTimer *timer);

#if defined(__cplusplus)
}	// extern "C"
#endif

#endif	// _AXE_Timer_h