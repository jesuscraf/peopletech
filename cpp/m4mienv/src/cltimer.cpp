//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cltimer.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:8/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "mit.h"

#include "cltimer.h"

void ClMIT_Timer::GetTimer( m4int16_t * ao_iHours, m4int16_t * ao_iMins, m4int16_t * ao_iSeconds, m4int16_t * ao_iMiliSeconds ) 
{
	*ao_iHours=GetTimerHour() ;
	*ao_iMins=GetTimerMin() ;
	*ao_iSeconds=GetTimerSec() ;
	*ao_iMiliSeconds=GetTimerMiliSec() ;
}

m4int16_t ClMIT_Timer::GetTimerHour()
{
	return (m4int16_t)( m_uiTimer/3600000 ) ;
}

m4int16_t ClMIT_Timer::GetTimerMin() 
{
	return (m4int16_t)( (m_uiTimer%3600000)/60000 ) ;
}

m4int16_t ClMIT_Timer::GetTimerSec() 
{
	return (m4int16_t)( (m_uiTimer%60000)/1000 ) ;
}

m4int16_t ClMIT_Timer::GetTimerMiliSec() 
{
	return (m4int16_t)( (m_uiTimer%60000)%1000 ) ;
}

