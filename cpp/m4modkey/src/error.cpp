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

#include "error.hpp"

//----------------------------------------------
//Clase ClErrorManager
//----------------------------------------------

void ClErrorManager::DumpError( m4pcchar_t ai_pError, m4char_t * ai_pFile, m4int32_t ai_iLine )
{
	printf(ai_pError, ai_pFile, ai_iLine);
}

//----------------------------------------------
//FIN Clase ClErrorManager
//----------------------------------------------
