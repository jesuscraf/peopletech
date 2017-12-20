//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             M4FMT.HPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the exported functiones of the DLL
//
//
//==============================================================================

#include "m4fmt32_dll.hpp"
#include "m4types.hpp"
#include "access.hpp"

////////////////////////////////////////////////////////////////////////////////


//Para que un thread (o proceso unico) pueda usar la dll, debe de hacer un AttachThreadEnv.
//Asi mismo, cuando se deje de usar la DLL, se debe hacer un Dettach.
//Lo que hacen es mantener una estructura de datos globales por thread. Attach la crea en
//	caso de que no este creada previamente, y Dettach la destruira solo cuando ya no este
//	nadie conectado (se lleva un contador para ello). 

//Cambio en el API de la DLL. Se le pasa el identificador de tarea, para poder
// mantener varias DLLs levantadas. Es responsabilidad del cliente mantener los
// identificadores de tarea.
extern "C" M4_DECL_M4FMT32 m4bool_t WINAPI M4FMTAttachM4Fmt(m4uint32_t* ao_TaskId) ;
extern "C" M4_DECL_M4FMT32 m4bool_t WINAPI M4FMTDettachM4Fmt(m4uint32_t ai_TaskId) ;

//Hay que pasarle a todas las funciones del API el identificador de tarea
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTCreateNewFormat (m4uint32_t ai_TaskId, m4char_t*  a_pcNombre,  m4int16_t * a_piH);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTSetProperty (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t*  a_pcNombre, m4char_t*  a_pcValor);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTDestroyFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandle);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTCreateAddFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t*  a_pcNombre,  m4int16_t * a_piHandleOut);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertFmtToFmt (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4,  m4int16_t a_iHandleOut, m4char_t*  a_pcOut,   m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTApplyFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertIsoToWin (m4uint32_t ai_TaskId, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertWinToIso (m4uint32_t ai_TaskId, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertFmtToIso (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertUserToFmt (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t*  a_pcIn,  m4int16_t a_iTipo,  m4int16_t a_iIsSQLM4,  m4int16_t a_iHandleOut, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t * a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTConvertSQLToM4 (m4uint32_t ai_TaskId, m4int16_t a_iTipoSql,  m4int16_t *a_piTipoM4);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTGetFormatName (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t*  a_pcOut,  m4int16_t a_iLonOut,  m4int16_t *a_piLonEscrita);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTGetFormatHandle (m4uint32_t ai_TaskId, m4int16_t *a_piHandle, m4char_t*  a_pcIn);
extern "C" M4_DECL_M4FMT32 m4int16_t WINAPI M4FMTGetErrorName (m4uint32_t ai_TaskId, m4int16_t a_iError, m4char_t* a_pcErrorName, m4int16_t a_iLonErrorName, m4int16_t  *a_iErrorNamel) ;
extern "C" M4_DECL_M4FMT32 m4bool_t  WINAPI M4FMTSetDesignAccess(m4uint32_t ai_TaskId, ClAccess* ai_poDesignAccess);


