//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             channel.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (28/05/98) - Tipo CURRENCY
//
// Definition:
//
//    API con la capa de canales
//
//
//==============================================================================

#ifndef __APICHNEN__H__
#define __APICHNEN__H__

#include "cltimer.h"

class ClChannelManager ;
class ClVMBaseEnv ;
class ClAccess ;
class ClMIT_Chn ;
class ClMIT_SessionChn ; 

struct StMIT_APIChn_TaskEnv
{
	//-------------->Propietaria:ClMIT_Chn

	// Esta clase lo pone a NULL

	ClMIT_MVC *m_poMVC ;	

	//Temporizaciones de aceso a canal. El constructor los resetea.

	ClMIT_Timer Chn_m_oTimerChannelAccess, 
		Chn_m_oTimerBuildFromId, Chn_m_oTimerChannelDestroy,
		Chn_m_oTimerStartVM, Chn_m_oTimerShutDownVM, Chn_m_oTimerExecuteMethod,
		Chn_m_oTimerLoad,    Chn_m_oTimerUnload,     Chn_m_oTimerStartUpCurrency ;

	ClAccess* ItemV_m_poCurrChn;     //Simple link, este constructor lo pone a NULL
	ClMIT_Chn *Chn_m_poChnCurrency ;	//Simple link,este constructor lo pone a NULL

	//-------------->Fin Propietaria:ClMIT_Chn

	//-------------->Propietaria:ClMIT_ChnItemValue
	
	enum { ItemV_SIZE_STR_FORMAT=2000, ItemV_CURR_SIZE=100, ItemV_TYPE_SIZE=100  } ;
	m4char_t ItemV_m_pcBuffFormat[ItemV_SIZE_STR_FORMAT] ;

	
	//Por defecto cadenas vacias
	
	m4char_t ItemV_m_pcExchCurr[ItemV_CURR_SIZE], ItemV_m_pcExchType[ItemV_TYPE_SIZE] ;
	m4date_t ItemV_m_ExchDate ;

	m4bool_t bFromWrapper ;

	//-------------->Fin Propietaria:ClMIT_ChnItemValue

	StMIT_APIChn_TaskEnv() ;
} ;

//------------------------- Temporizaciones ----------------------------------------------
/*		Se mantiene a nivel de thread una serie de temporizadores, que van a estimar los 
	tiempos empleados por la enginedll en realizar las operaciones que en principio van
	a ser mas costosas, principalmente las llamadas a la capa de canal. Otro conjunto de 
	temporizadores no se conserva a nivel de thread, sino que son definidos a nivel de la 
	capa loader.
		La mayoria de los temporizadores se han implementado para funcionar en modo DEBUG 
	y en modo RELEASE. Sin embargo, algunos de ellos solo funcionan en modo DEBUG, puesto
	que puede accederse en repetidas ocasiones y puede provocar perdidas de tiempo inne-
	cesarias y considerables.

		TEMPORIZADOR				NIVEL		MODO		ESTIMACION DE TIEMPOS
		========================	========	=======		============================
		Chn_m_oTimerStartVM			APIChn		DBG+RLS		Start Up de la MVC
		Chn_m_oTimerShutDownVM		APIChn		DBG+RLS		Shut Down de la MVC
		Chn_m_oTimerBuildFromId		APIChn		DBG+RLS		BuildFromId desde la MVC
		Chn_m_oTimerStartUpCurrency	APIChn		DBG+RLS		Levantar canal moneda
		Chn_m_oTimerLoad			APIChn		DBG+RLS		Hacer el Load de un canal
		Chn_m_oTimerUnload			APIChn		DBG+RLS		Unload de un canal de datos
		Chn_m_oTimerChannelAccess	APIChn		DEBUG		Acesos a capa de canal
		Chn_m_oTimerExecuteMethod	APIChn		DEBUG		Ejecucion de metodos por MVC
		Chn_m_oTimerUpDesign		Loader		DBG+RLS		Load del REPORT_CHANNEL_PT
		Chn_m_oTimerUpData			Loader		DBG+RLS		Load inicial del Data Channel
		m_oTimerLoadDesign			Loader		DBG+RLS		Preparar diseño de report
		m_oTimerPrintDesign			Loader		DBG+RLS		Diseño de impresion
		m_oTimerDownPhase			Loader		DBG+RLS		Limpiar diseño de report

		Los temporizadores siempre van a ser volcados a la traza, segun sea el modo de 
	compilacion del engine.

*/

#endif


