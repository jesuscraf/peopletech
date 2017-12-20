// M4LogSys.cpp : Implementation of CM4logsysvbApp and DLL registration.

#include "stdafx.h"
#include "m4logsysvb.h"
#include "m4logsys.h"

#include "m4types.hpp"
#include "m4unicode.hpp"
#include "m4logsys.hpp"
#include "m4variant.hpp"
#include "apichannelwrapper.h"
#include "m4log.hpp"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP M4LogSys::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IM4LogSys,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// Exportación COM de métodos de la m4logsys

STDMETHODIMP M4LogSys::FormatString(BSTR ai_psInString, BSTR ai_psParams, short ai_iByIdParamPos, BSTR *ao_psOutString, short *ao_iRetVal)
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcInString = NULL ;
	m4pchar_t	pcParams = NULL ;
	m4pchar_t	pcOut = NULL ;


	*ao_iRetVal = M4_ERROR ;

	if( *ao_psOutString != NULL )
	{
		SysFreeString( *ao_psOutString ) ;
		*ao_psOutString = NULL ;
	}

	pcInString = M4VBToCpp( ai_psInString ) ;
	pcParams = M4VBToCpp( ai_psParams ) ;

	iResult = FormatErrorString( pcInString, pcParams, ai_iByIdParamPos, NULL, 0, &iSize, LOGSYS_INVALID_M4UINT32 ) ;

	if( iResult != M4_SUCCESS )
	{
		delete [] pcInString ;
		delete [] pcParams ;
		return( S_OK ) ;
	}

	pcOut = new m4char_t[ iSize ] ;

	if( pcOut == NULL )
	{
		delete [] pcInString ;
		delete [] pcParams ;
		return( S_OK ) ;
	}

	iResult = FormatErrorString( pcInString, pcParams, ai_iByIdParamPos, pcOut, iSize, &iSize ) ;

	delete [] pcInString ;
	delete [] pcParams ;

	if( iResult != M4_SUCCESS )
	{
		delete [] pcOut ;
		return( S_OK ) ;
	}

	*ao_psOutString = M4CppToVB( pcOut ) ;
	delete [] pcOut ;

	*ao_iRetVal = M4_SUCCESS ;
	return( S_OK ) ;
}

STDMETHODIMP M4LogSys::FormatStringByParams(BSTR ai_psInString, SAFEARRAY **ai_saParams, short ai_iByIdParamPos, BSTR *ao_psOutString, short *ao_iRetVal)
{

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4uint32_t		iSize = 0 ;
	m4pchar_t		pcInString = NULL ;
	m4pchar_t		pcOut = NULL ;
	m4VariantType	*pvParams = NULL ;
	VARIANT			*pvTemp = NULL ;


	*ao_iRetVal = M4_ERROR ;

	if( *ao_psOutString != NULL )
	{
		SysFreeString( *ao_psOutString ) ;
		*ao_psOutString = NULL ;
	}

	if( *ai_saParams != NULL )
	{
		// Get number of params
		iLength = (**ai_saParams).rgsabound[0].cElements ;

		// VARIANT to m4VariantType
		pvParams = new m4VariantType[ iLength ] ;

		if( pvParams == NULL )
		{
			return( S_OK ) ;
		}

		hResult = SafeArrayAccessData( *ai_saParams, (void HUGEP* FAR*) &pvTemp ) ;

		if( hResult != S_OK )
		{
			delete [] pvParams ;
			return( S_OK ) ;
		}

		for( i = 0 ; i < iLength ; i++ )
		{
			pvParams[ i ] = pvTemp[ i ] ;
		}
		SafeArrayUnaccessData( *ai_saParams ) ;
	}

	pcInString = M4VBToCpp( ai_psInString ) ;

	iResult = FormatErrorStringByParams( pcInString, pvParams, iLength, ai_iByIdParamPos, NULL, 0, &iSize, LOGSYS_INVALID_M4UINT32 ) ;

	if( iResult != M4_SUCCESS )
	{
		if( pvParams != NULL )
		{
			delete [] pvParams ;
		}
		delete [] pcInString ;
		return( S_OK ) ;
	}

	pcOut = new m4char_t[ iSize ] ;

	if( pcOut == NULL )
	{
		if( pvParams != NULL )
		{
			delete [] pvParams ;
		}
		delete [] pcInString ;
		return( S_OK ) ;
	}

	iResult = FormatErrorStringByParams( pcInString, pvParams, iLength, ai_iByIdParamPos, pcOut, iSize, &iSize, LOGSYS_INVALID_M4UINT32 ) ;

	if( pvParams != NULL )
	{
		delete [] pvParams ;
	}
	delete [] pcInString ;

	if( iResult != M4_SUCCESS )
	{
		delete [] pcOut ;
		return( S_OK ) ;
	}

	*ao_psOutString = M4CppToVB( pcOut ) ;
	delete [] pcOut ;

	*ao_iRetVal = M4_SUCCESS ;
	return( S_OK ) ;
}

STDMETHODIMP M4LogSys::SerializeParameters(SAFEARRAY **ai_asParams, long ai_iSource, BSTR *ao_psOutString, short *ao_iRetVal)
{

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4uint32_t		iSize = 0 ;
	m4pchar_t		pcOut = NULL ;
	m4VariantType	*pvParams = NULL ;
	VARIANT			*pvTemp = NULL ;


	*ao_iRetVal = M4_ERROR ;

	if( *ao_psOutString != NULL )
	{
		SysFreeString( *ao_psOutString ) ;
		*ao_psOutString = NULL ;
	}

	if( *ai_asParams != NULL )
	{
		// Get number of params
		iLength = (**ai_asParams).rgsabound[0].cElements ;

		// VARIANT to m4VariantType
		pvParams = new m4VariantType[ iLength ] ;

		if( pvParams == NULL )
		{
			return( S_OK ) ;
		}

		hResult = SafeArrayAccessData( *ai_asParams, (void HUGEP* FAR*) &pvTemp ) ;

		if( hResult != S_OK )
		{
			delete [] pvParams ;
			return( S_OK ) ;
		}

		for( i = 0 ; i < iLength ; i++ )
		{
			pvParams[ i ] = pvTemp[ i ] ;
		}
		SafeArrayUnaccessData( *ai_asParams ) ;
	}

	// Get serialized length
	iResult = SerializeParams( pvParams, iLength, ai_iSource, NULL, 0, &iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		if( pvParams != NULL )
		{
			delete [] pvParams ;
		}
		return( S_OK ) ;
	}

	pcOut = new m4char_t[ iSize ] ;

	if( pcOut == NULL )
	{
		if( pvParams != NULL )
		{
			delete [] pvParams ;
		}
		return( S_OK ) ;
	}

	iResult = SerializeParams( pvParams, iLength, ai_iSource, pcOut, iSize, &iSize ) ;

	if( pvParams != NULL )
	{
		delete [] pvParams ;
	}

	if( iResult != M4_SUCCESS )
	{
		delete [] pcOut ;
		return( S_OK ) ;
	}

	*ao_psOutString = M4CppToVB( pcOut ) ;
	delete [] pcOut ;

	*ao_iRetVal = M4_SUCCESS ;
	return( S_OK ) ;
}

STDMETHODIMP M4LogSys::DeserializeParameters(BSTR ai_psInString, long *aio_piInState, short ai_iByIdParamPos, SAFEARRAY **ai_asParams, long *ao_piSource, long *ao_piError, long *ao_piExtra, long *ai_iByParseOption, short *ao_iRetVal)
{

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4pchar_t		pcInString = NULL ;
	m4pchar_t		pcParamString = NULL ;
	BSTR			*psTemp = NULL ;
	m4VariantType	vDeserializedParam[ LOGSYS_MAX_PARAM_LEN ] ;


	*ao_iRetVal = M4_ERROR ;

	if( *ai_asParams != NULL )
	{
		SafeArrayDestroy( *ai_asParams ) ;
		*ai_asParams = NULL ;
	}

	if( *aio_piInState == LOGSYS_INVALID_M4UINT32 )
	{
		return( S_OK ) ;
	}


	pcInString = M4VBToCpp( ai_psInString ) ;
	pcParamString = pcInString + *aio_piInState ;

	iResult = DeserializeParams( &pcParamString, ai_iByIdParamPos, vDeserializedParam, LOGSYS_MAX_PARAM_LEN, &iLength, (m4puint32_t) ao_piSource, (m4puint32_t) ao_piError, (m4puint32_t) ao_piExtra, (m4uint32_t) ai_iByParseOption ) ;

	if( pcParamString == NULL )
	{
		*aio_piInState = LOGSYS_INVALID_M4UINT32 ;
	}
	else
	{
		*aio_piInState = pcParamString - pcInString ;
	}

	delete [] pcInString ;

	if( iResult != M4_SUCCESS )
	{
		return( S_OK ) ;
	}


	// Create output array
	*ai_asParams = SafeArrayCreateVector( VT_BSTR, 0, iLength ) ;

	if( *ai_asParams == NULL )
	{
		return( S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ai_asParams, (void HUGEP* FAR*) &psTemp ) ;

	if( hResult != S_OK )
	{
		SafeArrayDestroy( *ai_asParams ) ;
		*ai_asParams = NULL ;
		return( S_OK ) ;
	}

	for( i = 0 ; i < iLength ; i++ )
	{
		psTemp[ i ] = M4CppToVB( vDeserializedParam[ i ].Data.PointerChar ) ;
	}

	hResult = SafeArrayUnaccessData( *ai_asParams ) ;

	if( hResult != S_OK )
	{
		SafeArrayDestroy( *ai_asParams ) ;
		*ai_asParams = NULL ;
		return( S_OK ) ;
	}

	*ao_iRetVal = M4_SUCCESS ;
	return( S_OK ) ;
}

// Exportación COM de métodos de la m4chwrap

STDMETHODIMP M4LogSys::M4ChWrapErrorLookup(IDispatch *oWrapCVM, SAFEARRAY **ErrorCodes)
{
	M4ChannelWrapperErrorLookup((void*)oWrapCVM, ErrorCodes);

	return S_OK;
}

STDMETHODIMP M4LogSys::M4ChWrapGetErrorString(long ErrorCode, BSTR strParams, unsigned char pos_params, IDispatch *oWrapCVM, BSTR *strError, short *ao_iRetVal)
{
	char * CppStrParam = M4VBToCpp (strParams);
	*ao_iRetVal = M4ChannelWrapperGetErrorString( ErrorCode, CppStrParam, pos_params, oWrapCVM, strError);
	delete [] CppStrParam;

	return S_OK;
}

STDMETHODIMP M4LogSys::M4ChWrapGetErrorStringByLang(long ErrorCode, BSTR strParams, unsigned char pos_params, IDispatch *oWrapCVM, BSTR *strError, long lLang, short *ao_iRetVal)
{
	char * CppStrParam = M4VBToCpp (strParams);
	*ao_iRetVal = M4ChannelWrapperGetErrorStringByLang( ErrorCode, CppStrParam, pos_params, oWrapCVM, strError, lLang);
	delete [] CppStrParam;

	return S_OK;
}

STDMETHODIMP M4LogSys::M4ChWrapGetErrorStringByParams(long ErrorCode, SAFEARRAY** ParamsArray, unsigned char pos_params, IDispatch* oWrapCVM, BSTR* strError, short *ao_iRetVal)
{
	*ao_iRetVal = M4ChannelWrapperGetErrorStringByParams( ErrorCode, ParamsArray, pos_params, oWrapCVM, strError);

	return S_OK;
}

STDMETHODIMP M4LogSys::M4ChWrapGetErrorStringByParamsByLang(long ErrorCode, SAFEARRAY** ParamsArray, unsigned char pos_params, IDispatch* oWrapCVM, BSTR* strError, long lLang, short *ao_iRetVal)
{
	*ao_iRetVal = M4ChannelWrapperGetErrorStringByParamsByLang(ErrorCode, ParamsArray, pos_params, oWrapCVM, strError, lLang);

	return S_OK;
}

// Exportación COM de métodos de la m4logdll

STDMETHODIMP M4LogSys::GetH_DecimalLastErrorCode(long* ErrorCode, short* ao_iRetVal)
{
	*ao_iRetVal = GETH_DECIMAL_LASTERRORCODE(*ErrorCode);

	return S_OK;
}

STDMETHODIMP M4LogSys::GetH_LastErrorType(BSTR *ErrorType, short *ao_iRetVal)
{
	m4pchar_t pszErrorType = new m4char_t[256];
	m4int32_t ai_RealSize;

	*ao_iRetVal = GETH_LASTERRORTYPE(pszErrorType, 255, ai_RealSize);
	
	if (*ao_iRetVal != M4_ERROR)
	{
		pszErrorType[ai_RealSize] = '\0';
		*ErrorType = M4CppToVB(pszErrorType);
	}

	delete [] pszErrorType;

	return S_OK;
}

STDMETHODIMP M4LogSys::GetH_LastErrorTime(BSTR *ErrorTime, short *ao_iRetVal)
{
	m4pchar_t pszErrorTime = new m4char_t[256];
	m4int32_t ai_RealSize;

	*ao_iRetVal = GETH_LASTERRORTIME(pszErrorTime, 255, ai_RealSize);
	
	if (*ao_iRetVal != M4_ERROR)
	{
		pszErrorTime[ai_RealSize] = '\0';
		*ErrorTime = M4CppToVB(pszErrorTime);
	}

	delete [] pszErrorTime;

	return S_OK;
}

STDMETHODIMP M4LogSys::GetH_LastErrorText(BSTR *ErrorText, short *ao_iRetVal)
{
	m4pchar_t pszErrorText = new m4char_t[8196];
	m4int32_t ai_RealSize;

	*ao_iRetVal = GETH_LASTERRORTEXT(pszErrorText, 8195, ai_RealSize);
	
	if (*ao_iRetVal != M4_ERROR)
	{
		pszErrorText[ai_RealSize] = '\0';
		*ErrorText = M4CppToVB(pszErrorText);
	}

	delete [] pszErrorText;

	return S_OK;
}

STDMETHODIMP M4LogSys::Enable_TraceInfo_Logs(short *ao_iRetVal)
{
	*ao_iRetVal = ENABLE_TRACEINFO_LOGS();

	return S_OK;
}

STDMETHODIMP M4LogSys::Disable_TraceInfo_Logs(short *ao_iRetVal)
{
	*ao_iRetVal = DISABLE_TRACEINFO_LOGS();

	return S_OK;
}

STDMETHODIMP M4LogSys::Log_Pop_Back(short *ao_iRetVal)
{
	*ao_iRetVal = POP_BACK();

	return S_OK;
}

STDMETHODIMP M4LogSys::IsEmptyLogStack(short *ao_iRetVal)
{
	*ao_iRetVal = EMPTY();

	return S_OK;
}

STDMETHODIMP M4LogSys::InitializeLogManager(BSTR iniFileName, short *ao_iRetVal)
{
	m4pchar_t pszFileName = M4VBToCpp(iniFileName);
	*ao_iRetVal = INITIALIZELOGMANAGER(pszFileName);
	delete [] pszFileName;

	return S_OK;
}

STDMETHODIMP M4LogSys::DestroyLogManager(short *ao_iRetVal)
{
	*ao_iRetVal = DESTROYLOGMANAGER();

	return S_OK;
}

STDMETHODIMP M4LogSys::LogSetCode(long icode, short irango, BSTR pmessage, short *ao_iRetVal)
{
	m4pchar_t pszMessage = M4VBToCpp(pmessage);
	*ao_iRetVal = SETCODE(icode, irango, pszMessage);
	delete [] pszMessage;

	return S_OK;
}
