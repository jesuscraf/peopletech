//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             rerror.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             10/11/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Definition of the raise error functions
//
//==============================================================================

//
// Include files
//
#include "rerror.h"
#include "m4log.hpp"

HRESULT RaiseError(gstring ai_st)
{
	ICreateErrorInfo *pcerrinfo;
	IErrorInfo *perrinfo;
 
	//
	// Create the create error info interface
	//
    HRESULT hr = CreateErrorInfo(&pcerrinfo);

	//
	// Add the only additional field we want to define
	// The textual description of the error
	//
/*	BSTR bstrSource;
	BSTR bstrDescription;
 
	bstrSource = SysAllocString(L"JIM");
	bstrDescription = SysAllocString(L"You are ugly");


	if (SUCCEEDED(hr))
	{
//		hr = pcerrinfo->SetGUID(IID_IJames);
		hr = pcerrinfo->SetHelpContext(343);
		hr = pcerrinfo->SetSource(bstrSource);
		hr = pcerrinfo->SetDescription(bstrDescription);
	}
*/
	if (SUCCEEDED(hr))
		hr = pcerrinfo->SetDescription(ai_st);
	//
	// Ask for the ErrorInfo interface from the
	// CreateErrorInfo interface
	//
	hr = pcerrinfo->QueryInterface(IID_IErrorInfo,
		(void**)&perrinfo);

	//
	// Set this as the current error info
	//
	if (SUCCEEDED(hr))
		SetErrorInfo(0, perrinfo);

	perrinfo->Release();
	pcerrinfo->Release();

	return E_UNEXPECTED;
}

HRESULT FormatError(m4int32_t ai_error, m4pcchar_t ai_sz, ...)
{
	va_list marker;
	m4char_t buffer[BUFSIZ];

	//
	// Construct one string from the
	// Format string
	//

	if (ai_sz)
	{
		va_start(marker, ai_sz);
		vsprintf(buffer, ai_sz, marker);
		va_end(marker);
	}

	//
	// Log the error
	//

	SETCODE(ai_error, ERRORLOG, buffer);

	// If we are debugging, display the message in the
	// debug window

	ATLTRACE(buffer);

	//
	// Then raise it for a client
	//

	return RaiseError(buffer);

	// This could be done much more simply with a call to
	//   return Error(buffer);
	// if only these functions were part of a base class!!!
}
