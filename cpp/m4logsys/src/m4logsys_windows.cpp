//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4logsys_win32.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           19 jan 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Error Format (specific for Win32)
////
//==============================================================================

#ifdef _WINDOWS

#include "m4var.hpp"

#include "m4logsys.hpp"
#include "m4logsys_windows.hpp"
#include "m4unicode.hpp"


///////////////////////////////////////////////////////////////////////
/////////////////                                //////////////////////
///////////////// Visual Basic Wrapper Functions //////////////////////
/////////////////                                //////////////////////
///////////////////////////////////////////////////////////////////////


m4int16_t WINAPI FormatErrorString_VB (
	char *ai_pszErrorString,			
	char *ai_pszParams,
	m4uchar_t ai_byIdParamPos,		
	BSTR *ao_pszOutString)
{

	m4return_t res, res2;
	m4uint32_t uBufferLen, uBufferLen2;

	res=FormatErrorString(ai_pszErrorString, ai_pszParams,
											ai_byIdParamPos, NULL, 0, &uBufferLen);

	if (res==M4_ERROR) {
		return M4_ERROR;
	}

	// Output string
	SysFreeString(*ao_pszOutString);
	*ao_pszOutString = SysAllocStringByteLen(NULL, uBufferLen-1); // It includes NULL

	if (*ao_pszOutString == NULL) { // Not enough memory
		return M4_ERROR;
	}

	res2=FormatErrorString(ai_pszErrorString, ai_pszParams,
											ai_byIdParamPos, (m4pchar_t)(*ao_pszOutString), uBufferLen, &uBufferLen2);

	M4_ASSERT(res==res2);
	M4_ASSERT(res==M4_SUCCESS);
	M4_ASSERT(uBufferLen==uBufferLen2);

	if (res==M4_ERROR) {
		return M4_ERROR;
	}

	return M4_SUCCESS;
}



m4int16_t WINAPI FormatErrorStringByParams_VB (
	char *ai_pszErrorString,			
	SAFEARRAY **ai_ppParams,
	m4uchar_t ai_byIdParamPos,		
	BSTR *ao_pszOutString)
{
	m4return_t res, res2;
	m4uint32_t cCont;
	m4uint32_t uNumParams;
	m4uint32_t uBufferLen, uBufferLen2;
	m4VariantType *pvParams;
	VARIANT *pvariantTemp;

	if (*ai_ppParams == NULL) { // Array uninitialized
		uNumParams=0;
		pvParams=NULL;
	} else {

		// Get number of params
		uNumParams = (**ai_ppParams).rgsabound[0].cElements;

		// VARIANT to m4VariantType
		if ((pvParams = new m4VariantType[uNumParams]) == NULL)
			return M4_ERROR;
		if (SafeArrayAccessData(*ai_ppParams, (void HUGEP* FAR*)&pvariantTemp)) {
			delete [] pvParams;
			return M4_ERROR;
		}
		for (cCont=0; cCont<uNumParams;cCont++) {
			pvParams[cCont] = pvariantTemp[cCont];
		}
		SafeArrayUnaccessData(*ai_ppParams);
	}

	/* Bug 0129409
	Hay que pasar la cadena de entrada a CPP
	y la de salida a ANSI para que le llegue bien a VB
	*/
	// UNICODE VB
	int iLength = -1 ;
	m4pchar_t pcErrorString = NULL ;
	m4pchar_t pcBuffer = NULL ;
	m4pchar_t pcMessage = NULL ;

	iLength = -1 ;
	pcErrorString = M4ANSIToCpp( ai_pszErrorString, iLength ) ;

	res=FormatErrorStringByParams(pcErrorString, pvParams, uNumParams,
			ai_byIdParamPos, NULL, 0, &uBufferLen, LOGSYS_INVALID_M4UINT32);

	if (res==M4_ERROR)
	{
		delete [] pcErrorString ;
		if (pvParams) delete [] pvParams;
		return M4_ERROR;
	}

	pcBuffer = new m4char_t[ uBufferLen ] ;

	if( pcBuffer == NULL )
	{
		delete [] pcErrorString ;
		if (pvParams) delete [] pvParams;
		return M4_ERROR;	// Not enough memory
	}

	res2=FormatErrorStringByParams(pcErrorString, pvParams, uNumParams,
			ai_byIdParamPos, pcBuffer, uBufferLen, &uBufferLen2, LOGSYS_INVALID_M4UINT32);

	delete [] pcErrorString ;
	if (pvParams) delete [] pvParams;

	if (res2==M4_ERROR) {
		delete [] pcBuffer ;
		return M4_ERROR;
	}

	iLength = uBufferLen2 - 1 ;
	pcMessage = M4CppToANSI( pcBuffer, iLength ) ;
	delete [] pcBuffer ;

	// Output string
	SysFreeString(*ao_pszOutString);
	*ao_pszOutString = SysAllocStringByteLen(pcMessage, iLength); // It includes NULL
	delete [] pcMessage ;

	return M4_SUCCESS;
}




m4int16_t WINAPI SerializeParams_VB(
	SAFEARRAY **ai_ppParams,
	m4uint32_t uSource,
	BSTR *ao_pszOutString)
{
	m4return_t res, res2;
	m4uint32_t uNumParams;
	m4uint32_t uBufferLen, uBufferLen2;
	m4uint32_t cCont;
	m4VariantType *pvParams;
	VARIANT *pvariantTemp;

	if (*ai_ppParams == NULL) { // Array uninitialized
		uNumParams=0;
		pvParams=NULL;
	} else {
		// Get number of params
		uNumParams = (**ai_ppParams).rgsabound[0].cElements;
	
		// VARIANT to m4VariantType
		if ((pvParams = new m4VariantType[uNumParams]) == NULL)
			return M4_ERROR;
		if (SafeArrayAccessData(*ai_ppParams, (void HUGEP* FAR*)&pvariantTemp)) {
			delete [] pvParams;
			return M4_ERROR;
		}
		for (cCont=0; cCont<uNumParams;cCont++) {
			pvParams[cCont] = pvariantTemp[cCont];
		}
		SafeArrayUnaccessData(*ai_ppParams);
	}


	// Get serialized length
	res = SerializeParams (pvParams, uNumParams, uSource,
									NULL, 0, &uBufferLen);

	if (res == M4_ERROR) {
		if (pvParams)
			delete [] pvParams;
		return M4_ERROR;
	}

	// Output string
	SysFreeString(*ao_pszOutString);
	*ao_pszOutString = SysAllocStringByteLen(NULL, uBufferLen-1); // It includes NULL

	if (*ao_pszOutString == NULL) { // Not enough memory
		if (pvParams)
			delete [] pvParams;
		return M4_ERROR;
	}

	// Set serialized params
	res2 = SerializeParams (pvParams, uNumParams, uSource,
									(m4pchar_t)(*ao_pszOutString), uBufferLen, &uBufferLen2);

	M4_ASSERT(res==res2);
	M4_ASSERT(res==M4_SUCCESS);
	M4_ASSERT(uBufferLen==uBufferLen2);

	if (res2 == M4_ERROR) {
		if (pvParams)
			delete [] pvParams;
		return M4_ERROR;
	}

	if (pvParams)
		delete [] pvParams;
	return M4_SUCCESS;
}



m4int16_t WINAPI DeserializeParams_VB(
	char *ai_pszInString,
	m4puint32_t ai_puInState,
	m4uchar_t ai_byIdParamPos,		
	SAFEARRAY **ai_ppParamsOut,
	m4puint32_t ao_puSource,
	m4puint32_t ao_puError,
	m4puint32_t ao_puExtra,
	m4uchar_t ai_byParseOption)
{

	m4VariantType vDeserializedParam[LOGSYS_MAX_PARAM_LEN];
	m4pchar_t pszParamString;
	m4uint32_t uNumParams;
	m4uint32_t cCont;
	BSTR *pbstrTemp;
	m4pchar_t pszTemp;

	if (*ai_puInState == LOGSYS_INVALID_M4UINT32)
		return M4_ERROR;

	if (ai_pszInString==NULL)
		ai_pszInString="";


	pszParamString=ai_pszInString + (*ai_puInState);

	DeserializeParams(&pszParamString, ai_byIdParamPos, vDeserializedParam, 
								LOGSYS_MAX_PARAM_LEN, &uNumParams,
								ao_puSource, ao_puError, ao_puExtra,ai_byParseOption);

	// Create output array

	if (SafeArrayDestroy(*ai_ppParamsOut)!=S_OK) // Error freeing array
		return M4_ERROR;

	if ((*ai_ppParamsOut=SafeArrayCreateVector(VT_BSTR,0,uNumParams))==NULL)  
		return M4_ERROR;

	// Fill the strings
	if (SafeArrayAccessData(*ai_ppParamsOut, (void HUGEP* FAR*)&pbstrTemp)) 
		return M4_ERROR;

	for (cCont=0; cCont<uNumParams; cCont++) {
		pszTemp=vDeserializedParam[cCont].Data.PointerChar;
		pbstrTemp[cCont] = SysAllocStringByteLen(pszTemp,strlen(pszTemp));
	}	
	SafeArrayUnaccessData(*ai_ppParamsOut);

	// Output control state
	if (pszParamString==NULL)
		*ai_puInState=LOGSYS_INVALID_M4UINT32;
	else
		*ai_puInState=pszParamString-ai_pszInString;	

	return M4_SUCCESS;

}



void WINAPI LogsysSetWorkingDir_VB(
	char *ai_pszParams)
{
	LogsysSetWorkingDir(ai_pszParams);
}


void WINAPI LogsysSetLangId_VB(
	m4uint32_t ai_uLangId)
{
	LogsysSetLangId(ai_uLangId);
}



#endif // _WINDOWS
