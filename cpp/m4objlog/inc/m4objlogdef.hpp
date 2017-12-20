//## begin module%391285880343.cm preserve=no
//## end module%391285880343.cm

//## begin module%391285880343.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%391285880343.cp

//## Module: m4objlogdef%391285880343; Package specification
//## Subsystem: M4ObjLog::inc%3912860B01D9
//## Source file: d:\new\m4objlog\inc\m4objlogdef.hpp

#ifndef m4objlogdef_h
#define m4objlogdef_h 1

//## begin module%391285880343.additionalIncludes preserve=no
//## end module%391285880343.additionalIncludes

//## begin module%391285880343.includes preserve=yes

#include "chlog.hpp"
#include "m4log.hpp"


//============== MACROS PARA CODIGOS DE ERROR ===============
//general
//Solo comprueba y existe en DEBUG
#ifdef _DEBUG
	#define M4_DEBUG_CHECK_RES(x,code, range, msg)	if (!(x)) { if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} return M4_ERROR; }
#else
    #define M4_DEBUG_CHECK_RES(x,code, range, msg)	
#endif

//esta muestra y compureba en debug y release
#ifdef _DEBUG
	#define M4_CHECK_RES(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} return M4_ERROR; }
#else
#define M4_CHECK_RES(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} return M4_ERROR; }
#endif

//esta macro comprueba en ambas y muestra en debug
#ifdef _DEBUG
	#define M4_CHECK_DEBUGINFO(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} return M4_ERROR; }
#else
	#define M4_CHECK_DEBUGINFO(x,code, range, msg)	if (!(x)){ return M4_ERROR; }
#endif

//## end module%391285880343.includes

//## begin module%391285880343.declarations preserve=no
//## end module%391285880343.declarations

//## begin module%391285880343.additionalDeclarations preserve=yes
//## end module%391285880343.additionalDeclarations


//## begin module%391285880343.epilog preserve=yes
//## end module%391285880343.epilog


#endif
