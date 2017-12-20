//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================


#ifndef	_M4MODKEY_FILEUTIL_HPP_
#define _M4MODKEY_FILEUTIL_HPP_

#include "globdec.hpp"

//----------------------------------------------
//Clase ClFileUtil
//----------------------------------------------
class M4_DECL_M4MODKEY ClFileUtil {
public:
	static m4return_t ReadFile( const m4pchar_t ai_pFileName, m4pchar_t & ao_pFileBuffer, m4int32_t & ao_iSize );
	static m4return_t WriteFile( const m4pchar_t ai_pFileName, const m4pchar_t ai_pFileBuffer, m4int32_t ai_iSize, m4bool_t ai_bReWrite );
};
//----------------------------------------------
//FIN Clase ClFileUtil
//----------------------------------------------

#endif


