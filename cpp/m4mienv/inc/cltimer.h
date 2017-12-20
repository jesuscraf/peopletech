//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cltimer.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:8/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (14/10/98) - GetTimerHour
//
// Definition:
//
//    Envuelve a la M4ClTimer para tener algun metodo mas.
//	  Hay que tener cuidado de no incluir nada del MemProfiler en
//	  este modulo, puesto que se utilizan objetos de la M4ClTimer
//	  para hacer el StartUp del Environment
//
//==============================================================================

#ifndef __CLTIMER__H__
#define __CLTIMER__H__

#include "m4time.hpp"

class M4_DECL_M4MIENV ClMIT_Timer: public ClM4Clock 
{
	//ESTA CLASE NO USA NADA REFERENTE A TRATAMIENTO MIT

MIT_PRIVATE:
	m4uint32_t m_uiTimer ;

MIT_PUBLIC:

	/*
	---Para temporizar ininterrumpidamente: 

	Timer.StartReset() ; ( ó Reset(); Start(); ) 
	***Bloque a temporizar
	Timer.Stop() ;

	---Para temporizar con interrupciones:

	Timer.Reset() ;
	
	***Bloque NO se temporiza

	Timer.Start() ; 
	***Bloque a temporizar
	Timer.Stop() ;

	***Bloque NO se temporiza

	...Más bloques Start/Stop.

	*/
	void Reset(m4uint32_t ai_uiValue=0) { m_uiTimer=ai_uiValue; } 

	void Start() { ClM4Clock::Start(); } 
	void StartReset() { ClM4Clock::Start(); m_uiTimer=0; }	//Equivale a Reset(); Start();
	void Stop() { m_uiTimer += ClM4Clock::GetDifference(); } 
	
	//--Tienen sentido tras un Stop.

	m4uint32_t GetTimer() { return m_uiTimer ; } 
	void GetTimer( m4int16_t * ao_iHours, m4int16_t * ao_iMins, m4int16_t * ao_iSeconds, m4int16_t * ao_iMiliSeconds ) ;

	// Ojo, no transforman todo el valor timer a otra unidad, si no que dan solo la parte 
	//	correspondiente. Osea, para un valor 30:20:1500, un GetTimerSec retorna 20, NO
	//	todo el valor de temporizacion convertido a segundos.

	m4int16_t GetTimerHour() ;
	m4int16_t GetTimerMin() ;
	m4int16_t GetTimerSec() ;
	m4int16_t GetTimerMiliSec() ;
} ;


#endif

