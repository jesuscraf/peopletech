//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc
// File:             errors.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             26-January-1999
// Language:         C++
// Operating System: Windows 32 , UNIX - Solaris 
// Design Document:  
//
//
// Definition:
// 
//	This module defines the error handling functions of the M4DOC.DLL library
//==============================================================================


#include "m4docerrors.hpp"
#include "m4log.hpp"





/*********************************************************************************** */
// Function: M4DOCErrorHandling  														
// Arguments:																		
//	IN		m4int_t error_type		: Id of the error type							
//			m4pchar_t add_error_message	: Additional error message					
//	OUT		None																
// Description:																		
//			This function handles the errors produced in the M4DOC.DLL		    
/*********************************************************************************** */
m4void_t M4DOCErrorHandling(m4int_t ai_ierror_type, m4pcchar_t ai_pccadd_error_message)
{
	string txt = "#*s0*#";

	txt = txt + ai_pccadd_error_message + "#";
	SETCODE (ai_ierror_type, ERRORLOG, txt.c_str());
	return;
}



