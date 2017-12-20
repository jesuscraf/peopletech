//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	=====================

#ifndef cljsexternlaunch_h
#define cljsexternlaunch_h 1

#include "m4jsutils_dll.hpp"
#include "m4types.hpp"
#include "m4variant.hpp"


extern "C" M4_DECL_M4JSUTILS m4return_t ExecuteProgram (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t UpdateLog (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t PrepareResourceForFTP(m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t ExistFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t PropertiesFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t EraseFile  (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4JSUTILS m4return_t GenerateNumber (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);

void ArrayBorrar ( m4pchar_t * ppszArray, m4uint32_t uiLongitud);
m4pchar_t * ArrayInsertarString (m4pchar_t * ppszAnterior, m4uint32_t & shLongitudAnterior, m4pchar_t pszCadena);
m4pchar_t ArrayConcatenar ( m4pchar_t *ppszArray, m4uint32_t shLongitudArray);

#endif
