
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtprestool.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                16-03-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene las funciones de la tool de presentaciones
//
//
//==============================================================================



#include "m4mdtprestool.hpp"
#include "m4mdtres.hpp"
#include "m4mdtdef.hpp"

#include "crtdbg.h"
#include "m4prop.h"
#include "m4anle_i.c"
#include "m4prop_i.c"
#include "definitions_i.c"
#include "_base64.h"
#include "m4unicode.hpp"



#include <atlbase.h>


//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa los miembros a null y rellena la tabla de idiomas
// Se necesita que se inicialicen los miembros, porque el orden es importante
// Primero debe inicializarse la anle
//
//=================================================================================

ClMDTPresentationTool::ClMDTPresentationTool( void )
{
	m_bComInitialized = M4_FALSE ;
	m_pIAnle202 = NULL ;
	m_pIAnle203 = NULL ;
	m_pITraductProp = NULL ;
	m_pIPersist = NULL ;
	m_pIConverter = NULL ;
	m_pIParser = NULL ;

	m_asLanguages[ 0 ] = SysAllocString( L"BRA" ) ;
	m_asLanguages[ 1 ] = SysAllocString( L"ENG" ) ;
	m_asLanguages[ 2 ] = SysAllocString( L"ESP" ) ;
	m_asLanguages[ 3 ] = SysAllocString( L"FRA" ) ;
	m_asLanguages[ 4 ] = SysAllocString( L"GEN" ) ;
	m_asLanguages[ 5 ] = SysAllocString( L"GER" ) ;
	m_asLanguages[ 6 ] = SysAllocString( L"ITA" ) ;

	m_pIAnle202 = _GetAnle202( "" ) ;
	m_pIAnle203 = _GetAnle203( "" ) ;
	m_pITraductProp = _GetTraductProp( "" ) ;
	m_pIPersist = _GetPersist( "" ) ;
	m_pIConverter = _GetConverter( "" ) ;
	m_pIParser = _GetParser( "" ) ;
}


//=================================================================================
//
// Destructor
//
// Libera los miembros y la tabla de idiomas
//
//=================================================================================

ClMDTPresentationTool::~ClMDTPresentationTool( void )
{

	m4uint16_t	i = 0 ;

	
	if( m_pIParser != NULL )
	{
		m_pIParser->Release() ;
		m_pIParser = NULL ;
	}

	if( m_pIConverter != NULL )
	{
		m_pIConverter->Release() ;
		m_pIConverter = NULL ;
	}

	if( m_pIPersist != NULL )
	{
		m_pIPersist->Release() ;
		m_pIPersist = NULL ;
	}

	if( m_pITraductProp != NULL )
	{
		m_pITraductProp->Release() ;
		m_pITraductProp = NULL ;
	}

	if( m_pIAnle202 != NULL )
	{
		m_pIAnle202->Release() ;
		m_pIAnle202 = NULL ;
	}

	if( m_pIAnle203 != NULL )
	{
		m_pIAnle203->Release() ;
		m_pIAnle203 = NULL ;
	}

	for( i = 0 ; i < M4MDT_LANGUAGE_NUMBER ; i++ )
	{
		if( m_asLanguages[ i ] != NULL )
		{
			SysFreeString( m_asLanguages[ i ] ) ;
			m_asLanguages[ i ] = NULL ;
		}
	}
}




//=================================================================================
// Funciones del interfaz
//=================================================================================


//=================================================================================
//
// Genera una presentación en texto a partir de sus campos en base de datos
//
// Los pasos son
//
//		Se inicializan los parámetros de salida a NULL
//		Se obtienen los miembros de la clase que se van a utilizar
//		Se crea un array de variants con los campos de base de datos de la
//			presentación. Los variants son del tipo cadena BSTR y contienen
//			cada uno un campo de la presentación
//		A partir del array se pide al objeto IConvertPropBag que convierta ese
//			array en un objeto del tipo ICPropBagDef, que es una presentación
//			en formato bolsa de propiedades. En esa conversión se necesita usar
//			el objeto Iveranle que es el que sabe construir los objetos visuales
//			que se añaden a la bolsa de propiedades
//			Si falla con el Iveranle de metalanguage 202 se intenta con el de 203
//		A partir de la bolsa de propiedades se pide al objeto IConvertPropBag que
//			convierta esa bolsa en una presentación en formato texto. Ese texto se
//			devuelve en un objeto cadena BSTR
//		Se convierte la cadena BSTR a una cadena C++ y se devuelve
//
//		Nota (28/06/2007):	Para generar la presentación en formato texto, sólo se van
//							a tener en cuenta las propiedades de la bolsa que se correspondan
//							a configuraciones activas. Esto significa que si el usuario ha 
//							activado la configuración de español (ESP) y la configuración de
//							inglés (ENG), se atenderá solo al valor de las propiedades en este
//							idioma para generar la presentación.
//=================================================================================

m4return_t	ClMDTPresentationTool::Presentation2OBL
								(
									m4pcchar_t	ai_pccPackage,		size_t ai_iPackageSize,
									m4pcchar_t	ai_pccBrazilian,	size_t ai_iBrazilianSize,
									m4pcchar_t	ai_pccEnglish,		size_t ai_iEnglishSize,
									m4pcchar_t	ai_pccSpanish,		size_t ai_iSpanishSize,
									m4pcchar_t	ai_pccFrench,		size_t ai_iFrenchSize,
									m4pcchar_t	ai_pccGeneric,		size_t ai_iGenericSize,
									m4pcchar_t	ai_pccGerman,		size_t ai_iGermanSize,
									m4pcchar_t	ai_pccItalian,		size_t ai_iItalianSize,
									m4pchar_t&	ao_rpcObl,			size_t& ao_riOblSize,
									m4pcchar_t	ai_pccPresentation,
									vector<string> ai_vEnableConfigIds
								)
{

	short			sResult = 0 ;
	size_t			lSize = 0 ;
	long			lIndex = 0 ;
	long			lDecoded = 0 ;
	HRESULT			hr = 0 ;
	BSTR			sOBL = NULL ;
	VARIANT			vData ;
	VARIANT			vArray ;
	m4pchar_t		pcPresentation = NULL ;
	char			*pcDecoded = NULL ;
	ICPropBagDef	*pIPBDef = NULL ;
	Iveranle		*pIAnle202 = NULL ;
	Iveranle		*pIAnle203 = NULL ;
	IConvertPropBag	*pIConverter = NULL ;
	size_t			aiPresentationsSize[ M4MDT_LANGUAGE_NUMBER + 1 ] ;
	m4pcchar_t		apccPresentations  [ M4MDT_LANGUAGE_NUMBER + 1 ] ;

	static C_Base64	s_oBase64 ;


	ao_rpcObl = NULL ;
	ao_riOblSize = 0 ;


	pIAnle202 = _GetAnle202( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIAnle202 == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetAnle202", ai_pccPresentation, "%s#%s#%s" ) ;

	pIAnle203 = _GetAnle203( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIAnle203 == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetAnle203", ai_pccPresentation, "%s#%s#%s" ) ;

	pIConverter = _GetConverter( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIConverter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetConverter", ai_pccPresentation, "%s#%s#%s" ) ;

	// Establecer los valores de los packages de la presentación, para cada idioma, dependiendo
	// de las configuraciones que el usuario haya activado.
	apccPresentations[ 0 ] = ai_pccPackage ;
	apccPresentations[ 1 ] = NULL ;
	apccPresentations[ 2 ] = NULL ;
	apccPresentations[ 3 ] = NULL ;
	apccPresentations[ 4 ] = NULL ;
	apccPresentations[ 5 ] = NULL ;
	apccPresentations[ 6 ] = NULL ;
	apccPresentations[ 7 ] = NULL ;
	
	aiPresentationsSize[ 0 ] = ai_iPackageSize ;
	aiPresentationsSize[ 1 ] = 0 ;
	aiPresentationsSize[ 2 ] = 0 ;
	aiPresentationsSize[ 3 ] = 0 ;
	aiPresentationsSize[ 4 ] = 0 ;
	aiPresentationsSize[ 5 ] = 0 ;
	aiPresentationsSize[ 6 ] = 0 ;
	aiPresentationsSize[ 7 ] = 0 ;
		
	vector<string>::iterator it = ai_vEnableConfigIds.begin();
	while (it != ai_vEnableConfigIds.end())
	{
		string sConfigId = *it;
		
		if ((sConfigId.compare("BRA")) == 0)
		{
			apccPresentations[ 1 ] = ai_pccBrazilian ;
			aiPresentationsSize[ 1 ] = ai_iBrazilianSize ;
		}
		else if ((sConfigId.compare("ENG")) == 0)
		{
			apccPresentations[ 2 ] = ai_pccEnglish ;
			aiPresentationsSize[ 2 ] = ai_iEnglishSize ;
		}
		else if ((sConfigId.compare("ESP")) == 0)
		{
			apccPresentations[ 3 ] = ai_pccSpanish ;
			aiPresentationsSize[ 3 ] = ai_iSpanishSize ;
		}
		else if ((sConfigId.compare("FRA")) == 0)
		{
			apccPresentations[ 4 ] = ai_pccFrench ;
			aiPresentationsSize[ 4 ] = ai_iFrenchSize ;
		}
		else if ((sConfigId.compare("GEN")) == 0)
		{
			apccPresentations[ 5 ] = ai_pccGeneric ;
			aiPresentationsSize[ 5 ] = ai_iGenericSize ;
		}
		else if ((sConfigId.compare("GER")) == 0)
		{
			apccPresentations[ 6 ] = ai_pccGerman ;
			aiPresentationsSize[ 6 ] = ai_iGermanSize ;
		}
		else if ((sConfigId.compare("ITA")) == 0)
		{
			apccPresentations[ 7 ] = ai_pccItalian ;
			aiPresentationsSize[ 7 ] = ai_iItalianSize ;
		}
		
		it++;
	}

	vArray.vt = VT_ARRAY | VT_VARIANT ;
	vArray.parray = SafeArrayCreateVector( VT_VARIANT, 0, 8 ) ;

	for( lIndex = 0 ; lIndex < M4MDT_LANGUAGE_NUMBER + 1 ; lIndex++ )
	{
		/* Bug 0100834
		Si la presentación está grabada en base64 hay que pasarla a binario
		*/
		lSize = aiPresentationsSize[ lIndex ] ;
		pcPresentation = (m4pchar_t) apccPresentations[ lIndex ] ;
		pcDecoded = NULL ;

		if( pcPresentation != NULL && *pcPresentation == 'M' )
		{
			sResult = s_oBase64.Decode( pcPresentation, pcDecoded, &lDecoded ) ;
			M4MDT_CLEAN_ERROR1( sResult != 1 || pcDecoded == NULL, M4_ERROR, M4MDTTERROR_NO_BASE64_TO_BINARY, SafeArrayDestroy( vArray.parray ), ai_pccPresentation, "%s" ) ;

			lSize = lDecoded ;
			pcPresentation = pcDecoded ;
		}

		// UNICODE VB
		vData.vt = VT_BSTR ;
		vData.bstrVal = M4CppToVBBinary( pcPresentation, lSize ) ;
		
		if( pcDecoded != NULL )
		{
			s_oBase64.FreeBuffer( pcDecoded ) ;
			pcDecoded = NULL ;
		}

		hr = SafeArrayPutElement( vArray.parray, &lIndex, (void *) &vData ) ;
		M4MDT_CLEAN_ERROR2( hr != S_OK, M4_ERROR, M4MDTTERROR_NO_PUT_ARRAY_ELEMENT, SafeArrayDestroy( vArray.parray ), hr, ai_pccPresentation, "%d#%s" ) ;

		SysFreeString( vData.bstrVal ) ;
	}

	hr = pIConverter->ConstructPresentationFromArray( vArray, pIAnle202, &pIPBDef ) ;

	if( pIPBDef == NULL )
	{
		hr = pIConverter->ConstructPresentationFromArray( vArray, pIAnle203, &pIPBDef ) ;
	}

	SafeArrayDestroy( vArray.parray ) ;
	M4MDT_RETURN_ERROR2( hr != S_OK || pIPBDef == NULL, M4_ERROR, M4MDTTERROR_NO_PRES_FROM_ARRAY, hr, ai_pccPresentation, "%d#%s" ) ;

	hr = pIConverter->PBDef2StringOBLHide( pIPBDef, &sOBL ) ;
	pIPBDef->Release() ;
	pIPBDef = NULL ;
	M4MDT_RETURN_ERROR2( hr != S_OK || sOBL == NULL, M4_ERROR, M4MDTTERROR_NO_OBL_FROM_PRES, hr, ai_pccPresentation, "%d#%s" ) ;

	// UNICODE VB
	/* Bug 0178924
	El obl se da en el formato del entorno
	*/
	ao_rpcObl = M4VBToCpp( sOBL ) ;
	ao_riOblSize = strlen( ao_rpcObl ) ;
	SysFreeString( sOBL ) ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera los campos de base de datos de una presentación a partir de su texto
//
// Los pasos son
//
//		Se inicializan los parámetros de salida a NULL
//		Se obtienen los miembros de la clase que se van a utilizar
//		Se convierte la presentación en formato texto que viene en una cadena de
//			C++ a una cadena BSTR
//		A partir de esa cadena BSTR se llama a una función que obtiene el
//			objeto ICPropBagDef que representa la presentación en su formato
//			de bolsa de propiedades
//		A partir de la bolsa de propiedades se pide al objeto IConvertPropBag
//			que convierta esa presentación en sus campos de base de datos.
//			Este objeto deja esa conversión en un objeto de tipo IPersistObject
//		A partir del objeto IPersistObject se obtienen los campos en base
//			de datos de la presentación como cadenas BSTR
//		Se convierte las cadenas BSTR a cadenas C++ y se devuelven
//
// 
//		Nota (28/06/2007):	cuando se va a generar los campos de base de datos a partir
//							de la presentación, se hará atendiendo a las configuraciones
//							de idiomas que el usuario haya activado. Por ejemplo, si el 
//							usuario ha activado la configuración de español (ESP) y la 
//							configuración de inglés (ENG), sólo se van a rellenar estos 
//							dos campos de base de datos en este método. En un proceso 
//							posterior (External2PresentationPackage) se rellenará el resto.
//=================================================================================

m4return_t	ClMDTPresentationTool::OBL2Presentation
								(
									m4pcchar_t	ai_pccObl,			size_t	ai_iOblSize,
									m4pchar_t&	ao_rpcPackage,		size_t&	ai_riPackageSize,
									m4pchar_t&	ao_rpcBrazilian,	size_t&	ai_riBrazilianSize,
									m4pchar_t&	ao_rpcEnglish,		size_t&	ai_riEnglishSize,
									m4pchar_t&	ao_rpcSpanish,		size_t&	ai_riSpanishSize,
									m4pchar_t&	ao_rpcFrench,		size_t&	ai_riFrenchSize,
									m4pchar_t&	ao_rpcGeneric,		size_t&	ai_riGenericSize,
									m4pchar_t&	ao_rpcGerman,		size_t&	ai_riGermanSize,
									m4pchar_t&	ao_rpcItalian,		size_t&	ai_riItalianSize,
									m4pcchar_t	ai_pccPresentation,
									vector<string> ai_vEnableConfigIds
								)

{

	m4return_t		iResult = M4_SUCCESS ;
	size_t			i = 0 ;
	HRESULT			hr = 0 ;
	BSTR			sOBL = NULL ;
	VARIANT			vLanguage ;
	ICPropBagDef	*pIPBDef = NULL ;
	IConvertPropBag	*pIConverter = NULL ;
	ITraductProp	*pITraductProp = NULL ;
	IPersistObject	*pIPesist = NULL ;
	BSTR			asPresentations[ M4MDT_LANGUAGE_NUMBER + 1 ] ;


	ao_rpcPackage   = NULL ;
	ao_rpcBrazilian = NULL ;
	ao_rpcEnglish   = NULL ;
	ao_rpcSpanish   = NULL ;
	ao_rpcFrench    = NULL ;
	ao_rpcGeneric   = NULL ;
	ao_rpcGerman    = NULL ;
	ao_rpcItalian   = NULL ;

	ai_riPackageSize   = 0 ;
	ai_riBrazilianSize = 0 ;
	ai_riEnglishSize   = 0 ;
	ai_riSpanishSize   = 0 ;
	ai_riFrenchSize    = 0 ;
	ai_riGenericSize   = 0 ;
	ai_riGermanSize    = 0 ;
	ai_riItalianSize   = 0 ;

	
	//	Inicializar asPresentations[].
	for (i = 0; i <= M4MDT_LANGUAGE_NUMBER; i++)
	{
		asPresentations[i] = L"";
	}
	
	pIConverter = _GetConverter( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIConverter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetConverter", ai_pccPresentation, "%s#%s#%s" ) ;

	pITraductProp = _GetTraductProp( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pITraductProp == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetTraductProp", ai_pccPresentation, "%s#%s#%s" ) ;

	pIPesist = _GetPersist( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIPesist == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetPersist", ai_pccPresentation, "%s#%s#%s" ) ;


	// UNICODE VB
	/* Bug 0178924
	El obl viene en el formato del entorno
	*/
	sOBL = M4CppToVB( ai_pccObl ) ;
	M4MDT_RETURN_ERROR3( sOBL == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "M4CppToVBBinary", ai_pccPresentation, "%s#%s#%s" ) ;


	iResult = _OBL2PropBagDef( sOBL, pIPBDef, ai_pccPresentation ) ;
	SysFreeString( sOBL ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_OBL2PropBagDef", ai_pccPresentation, "%s#%s#%s" ) ;

	hr = pIConverter->PersistPBDProccess( pIPBDef, pIPesist ) ;
	pIPBDef->Release() ;
	pIPBDef = NULL ;
	M4MDT_RETURN_ERROR2( hr != S_OK, M4_ERROR, M4MDTTERROR_NO_ARRAY_FROM_PRESS, hr, ai_pccPresentation, "%d#%s" ) ;


	hr = pIPesist->get_PersistMain( &asPresentations[ 0 ] ) ;
	M4MDT_RETURN_ERROR2( hr != S_OK, M4_ERROR, M4MDTTERROR_NO_GET_ARRAY_PRESENTATION, hr, ai_pccPresentation, "%d#%s" ) ;

	vLanguage.vt = VT_BSTR ;

	for( i = 0 ; i < M4MDT_LANGUAGE_NUMBER ; i++ )
	{
		vLanguage.bstrVal = m_asLanguages[ i ] ;

		// Convertir BSTR a cadena.
		unsigned uLength = SysStringLen(m_asLanguages[i]);
		char * pszLanguage = new char[uLength + 1];
		WideCharToMultiByte(CP_ACP, 0, m_asLanguages[i], uLength, pszLanguage, uLength, NULL, NULL);
		pszLanguage[uLength] = '\0';

		// Verificar configuraciones activas y rellenar sólo los campos correspondientes.
		m4bool_t bFound = M4_FALSE;
		vector<string>::iterator it = ai_vEnableConfigIds.begin();
		while (it != ai_vEnableConfigIds.end())
		{
			string sConfigId = *it;
			if ((sConfigId.compare(pszLanguage) == 0))
			{
				bFound = M4_TRUE;
				break;
			}
			it++;
		}

		delete [] pszLanguage;

		if (bFound == M4_TRUE)
		{
			hr = pITraductProp->get_ValueByLng( vLanguage, &asPresentations[ i + 1 ] ) ;
			M4MDT_RETURN_ERROR2( hr != S_OK, M4_ERROR, M4MDTTERROR_NO_GET_ARRAY_PRESENTATION, hr, ai_pccPresentation, "%d#%s" ) ;
		}
	}

	// UNICODE VB
	ai_riPackageSize   = SysStringLen( asPresentations[ 0 ] ) ;
	ai_riBrazilianSize = SysStringLen( asPresentations[ 1 ] ) ;
	ai_riEnglishSize   = SysStringLen( asPresentations[ 2 ] ) ;
	ai_riSpanishSize   = SysStringLen( asPresentations[ 3 ] ) ;
	ai_riFrenchSize    = SysStringLen( asPresentations[ 4 ] ) ;
	ai_riGenericSize   = SysStringLen( asPresentations[ 5 ] ) ;
	ai_riGermanSize    = SysStringLen( asPresentations[ 6 ] ) ;
	ai_riItalianSize   = SysStringLen( asPresentations[ 7 ] ) ;

	ao_rpcPackage   = M4VBToCppBinary( asPresentations[ 0 ], ai_riPackageSize    ) ;
	ao_rpcBrazilian = M4VBToCppBinary( asPresentations[ 1 ], ai_riBrazilianSize  ) ;
	ao_rpcEnglish   = M4VBToCppBinary( asPresentations[ 2 ], ai_riEnglishSize    ) ;
	ao_rpcSpanish   = M4VBToCppBinary( asPresentations[ 3 ], ai_riSpanishSize    ) ;
	ao_rpcFrench    = M4VBToCppBinary( asPresentations[ 4 ], ai_riFrenchSize     ) ;
	ao_rpcGeneric   = M4VBToCppBinary( asPresentations[ 5 ], ai_riGenericSize    ) ;
	ao_rpcGerman    = M4VBToCppBinary( asPresentations[ 6 ], ai_riGermanSize     ) ;
	ao_rpcItalian   = M4VBToCppBinary( asPresentations[ 7 ], ai_riItalianSize    ) ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Funciones auxiliares
//=================================================================================


//=================================================================================
//
// Crea un nuevo interfaz del tipo pedido
// Primero inicializa la librería COM
// Si se ha inicializado bien, o ya estaba inicializa con el mismo modo
// u otro distinto no se da error
// Después crea la instancia del interfaz con el API COM
//
//=================================================================================

void*	ClMDTPresentationTool::_NewInterface( REFCLSID ai_ClassId, REFIID ai_InterfaceId, m4pcchar_t ai_pccInterface, m4pcchar_t ai_pccPresentation )
{

	void*	pvResult = NULL ;
	HRESULT	hr = S_OK ;

	
	if( m_bComInitialized == M4_FALSE )
	{
		hr = CoInitializeEx( NULL, COINIT_MULTITHREADED ) ;
		M4MDT_RETURN_ERROR2( hr != S_OK && hr != S_FALSE && hr != RPC_E_CHANGED_MODE, NULL, M4MDTTERROR_NO_INIT_COM, hr, ai_pccPresentation, "%d#%s" ) ;

		m_bComInitialized = M4_TRUE ;
	}

	hr = CoCreateInstance( ai_ClassId, NULL, CLSCTX_INPROC_SERVER, ai_InterfaceId, (void**) &pvResult ) ;
	M4MDT_RETURN_ERROR3( hr != S_OK || pvResult == NULL, NULL, M4MDTTERROR_NO_CREATE_INSTANCE, ai_pccInterface, hr, ai_pccPresentation, "%s#%d#%s" ) ;

	return( pvResult ) ;
}



//=================================================================================
//
// Devuelve el miembro Iveranle para el metalenguaje 202
// Lo crea e inicializa si es necesario
// 
// Si el objeto ya existe lo devuelve
// Si no existe lo crea y después lo inicializa
// Después se crea un array con la lista de idiomas y se inicializa el objeto
// con ella
//
//=================================================================================

Iveranle*	ClMDTPresentationTool::_GetAnle( Iveranle* &ao_rpVeranle, long ai_lMetaLanguage, m4pcchar_t ai_pccPresentation )
{

	HRESULT		hr = S_OK ;
	long		lIndex = 0 ;
	VARIANT		vArray ;


	if( ao_rpVeranle != NULL )
	{
		return( ao_rpVeranle ) ;
	}

	ao_rpVeranle = (Iveranle *) _NewInterface( CLSID_veranle, IID_Iveranle, "Iveranle", ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR4( ao_rpVeranle == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetAnle", "_NewInterface", "Iveranle", ai_pccPresentation, "%s#%s Interface = <%s>#%s" ) ;

	hr = ao_rpVeranle->put_MetalengResourceID( ai_lMetaLanguage ) ;
	M4MDT_RETURN_ERROR3( hr != S_OK, NULL, M4MDTTERROR_NO_INIT_COM_OBJECT, "Iveranle", hr, ai_pccPresentation, "%s#%d#%s" ) ;

	vArray.vt = VT_ARRAY | VT_BSTR ;
	vArray.parray = SafeArrayCreateVector( VT_BSTR, 0, M4MDT_LANGUAGE_NUMBER ) ;

	for( lIndex = 0 ; lIndex < M4MDT_LANGUAGE_NUMBER; lIndex++ )
	{
		hr = SafeArrayPutElement( vArray.parray, &lIndex, (void *) m_asLanguages[ lIndex ] ) ;
		M4MDT_CLEAN_ERROR2( hr != S_OK, NULL, M4MDTTERROR_NO_PUT_ARRAY_ELEMENT, SafeArrayDestroy( vArray.parray ), hr, ai_pccPresentation, "%d#%s" ) ;
	}

	hr = ao_rpVeranle->InitLangFromArray( vArray ) ;
	SafeArrayDestroy( vArray.parray ) ;
	M4MDT_RETURN_ERROR3( hr != S_OK, NULL, M4MDTTERROR_NO_INIT_COM_OBJECT, "Iveranle", hr, ai_pccPresentation, "%s#%d#%s" ) ;

	
	return( ao_rpVeranle ) ;
}



//=================================================================================
//
// Devuelve el miembro Iveranle para el metalenguaje 202
// Delegan en la función _GetAnle
// 
//=================================================================================

Iveranle*	ClMDTPresentationTool::_GetAnle202( m4pcchar_t ai_pccPresentation )
{

	m_pIAnle202 = _GetAnle( m_pIAnle202, 202, ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( m_pIAnle202 == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetAnle202", "_GetAnle", ai_pccPresentation, "%s#%s#%s" ) ;
	
	return( m_pIAnle202 ) ;
}



//=================================================================================
//
// Devuelve el miembro Iveranle para el metalenguaje 203
// Delegan en la función _GetAnle
// 
//=================================================================================

Iveranle*	ClMDTPresentationTool::_GetAnle203( m4pcchar_t ai_pccPresentation )
{

	m_pIAnle203 = _GetAnle( m_pIAnle203, 203, ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( m_pIAnle203 == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetAnle203", "_GetAnle", ai_pccPresentation, "%s#%s#%s" ) ;
	
	return( m_pIAnle203 ) ;
}



//=================================================================================
//
// Devuelve el miembro ITraductProp. Lo crea e inicializa si es necesario
// 
// Si el objeto ya existe lo devuelve
// Si no existe lo crea y después lo inicializa
// Para inicializarlo se crea un array con la lista de idiomas y se
// inicializa el objeto con ella
//
//=================================================================================

ITraductProp*	ClMDTPresentationTool::_GetTraductProp( m4pcchar_t ai_pccPresentation )
{

	HRESULT		hr = S_OK ;
	long		lIndex = 0 ;
	VARIANT		vArray ;


	if( m_pITraductProp != NULL )
	{
		return( m_pITraductProp ) ;
	}

	m_pITraductProp = (ITraductProp *) _NewInterface( CLSID_TraductProp, IID_ITraductProp, "ITraductProp", ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR4( m_pITraductProp == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetTraductProp", "_NewInterface", "ITraductProp", ai_pccPresentation, "%s#%s Interface = <%s>#%s" ) ;

	vArray.vt = VT_ARRAY | VT_BSTR ;
	vArray.parray = SafeArrayCreateVector( VT_BSTR, 0, M4MDT_LANGUAGE_NUMBER ) ;

	for( lIndex = 0 ; lIndex < M4MDT_LANGUAGE_NUMBER; lIndex++ )
	{
		hr = SafeArrayPutElement( vArray.parray, &lIndex, (void *) m_asLanguages[ lIndex ] ) ;
		M4MDT_CLEAN_ERROR2( hr != S_OK, NULL, M4MDTTERROR_NO_PUT_ARRAY_ELEMENT, SafeArrayDestroy( vArray.parray ), hr, ai_pccPresentation, "%d#%s" ) ;
	}

	hr = m_pITraductProp->InitFromBSTRArray( vArray ) ;
	SafeArrayDestroy( vArray.parray ) ;
	M4MDT_RETURN_ERROR3( hr != S_OK, NULL, M4MDTTERROR_NO_INIT_COM_OBJECT, "ITraductProp", hr, ai_pccPresentation, "%s#%d#%s" ) ;

	
	return( m_pITraductProp ) ;
}



//=================================================================================
//
// Devuelve el miembro IPersistObject. Lo crea e inicializa si es necesario
// 
// Si el objeto ya existe lo devuelve
// Si no existe lo crea y después lo inicializa
// Para inicializarlo se obtiene el objeto de propiedades traducidas y se le añade
// como referencia
//
//=================================================================================

IPersistObject*	ClMDTPresentationTool::_GetPersist( m4pcchar_t ai_pccPresentation )
{

	HRESULT			hr = 0 ;
	ITraductProp	*pITraductProp = NULL ;

	
	if( m_pIPersist != NULL )
	{
		return( m_pIPersist ) ;
	}

	m_pIPersist = (IPersistObject *) _NewInterface( CLSID_PersistObject, IID_IPersistObject, "IPersistObject", ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR4( m_pIPersist == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetPersist", "_NewInterface", "IPersistObject", ai_pccPresentation, "%s#%s Interface = <%s>#%s" ) ;

	pITraductProp = _GetTraductProp( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pITraductProp == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetPersist", "_GetTraductProp", ai_pccPresentation, "%s#%s#%s" ) ;

	hr = m_pIPersist->putref_PersistTraductProp( pITraductProp ) ;
	M4MDT_RETURN_ERROR3( hr != S_OK, NULL, M4MDTTERROR_NO_INIT_COM_OBJECT, "IPersistObject", hr, ai_pccPresentation, "%s#%d#%s" ) ;


	return( m_pIPersist ) ;
}



//=================================================================================
//
// Devuelve el miembro IConvertPropBag. Lo crea e inicializa si es necesario
// 
// Si el objeto ya existe lo devuelve
// Si no existe lo crea
//
//=================================================================================

IConvertPropBag*	ClMDTPresentationTool::_GetConverter( m4pcchar_t ai_pccPresentation )
{
	if( m_pIConverter != NULL )
	{
		return( m_pIConverter ) ;
	}

	m_pIConverter = (IConvertPropBag *) _NewInterface( CLSID_ConvertPropBag, IID_IConvertPropBag, "IConvertPropBag", ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR4( m_pIConverter == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetConverter", "_NewInterface", "IConvertPropBag", ai_pccPresentation, "%s#%s Interface = <%s>#%s" ) ;

	return( m_pIConverter ) ;
}



//=================================================================================
//
// Devuelve el miembro IOBLParser. Lo crea e inicializa si es necesario
// 
// Si el objeto ya existe lo devuelve
// Si no existe lo crea
//
//=================================================================================

IOBLParser*	ClMDTPresentationTool::_GetParser( m4pcchar_t ai_pccPresentation )
{
	if( m_pIParser != NULL )
	{
		return( m_pIParser ) ;
	}

	m_pIParser = (IOBLParser *) _NewInterface( CLSID_OBLParser, IID_IOBLParser, "IOBLParser", ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR4( m_pIParser == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_GetParser", "_NewInterface", "IOBLParser", ai_pccPresentation, "%s#%s Interface = <%s>#%s" ) ;

	return( m_pIParser ) ;
}



//=================================================================================
//
// Obtiene la presentación en formato bolsa de propiedades a partir de la
// presentación en formato texto
//
// Los pasos son
//
//		Se inicializa el parámetro de salida a NULL
//		Se obtienen los miembros de la clase que se van a utilizar
//		A partir de la presentación en formato cadena BSTR se pide al objeto
//			IOBLParser que convierta esa presentación en un objeto del tipo
//			ICPropBagDef, que es una presentación en formato bolsa de propiedades.
//			En esa conversión se necesita usar el objeto Iveranle que es el que
//			sabe construir los objetos visuales que se añaden a la bolsa de
//			propiedades
//		La presentación obtenida no está directamente en el nivel 0 de la bolsa
//			de propiedades, sino que viene dentro de una propiedad de tipo texto,
//			de modo que para recuperar la presentación hay que pedir el primer
//			hijo a la bolsa de propiedades. Para ello se pide a la bolsa de
//			propiedades sus columnas dadas por un objeto de tipo ICChildColDef
//			al cual se pide su primer elemento, que es la presentación en si,
//			de tipo ICPropBagDef
//		Si resulta que no existe un hijo para esa bolsa de propiedades puede ser
//			porque el metalenguaje no sea el adecuado y se vuelve a intentar el
//			proceso con con el metalenguaje 203
//		Se asigna la variable de salida a la presentación obtenida
//
//=================================================================================

m4return_t	ClMDTPresentationTool::_OBL2PropBagDef( const BSTR ai_pcsOBL, ICPropBagDef* &ao_rpIPBDef, m4pcchar_t ai_pccPresentation )
{

	HRESULT			hr = 0 ;
	long			lCount = 0 ;
	VARIANT			vIndex ;
	ICPropBagDef	*pIPBDef = NULL ;
	ICPropBagDef	*pIPBDefChild = NULL ;
	ICChildColDef	*pIChildColDef = NULL ;
	Iveranle		*pIAnle202 = NULL ;
	Iveranle		*pIAnle203 = NULL ;
	IOBLParser		*pIParser = NULL ;


	ao_rpIPBDef = NULL ;

	pIAnle202 = _GetAnle202( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIAnle202 == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_OBL2PropBagDef", "_GetAnle202", ai_pccPresentation, "%s#%s#%s" ) ;

	pIAnle203 = _GetAnle203( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIAnle203 == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::_OBL2PropBagDef", "_GetAnle203", ai_pccPresentation, "%s#%s#%s" ) ;

	pIParser = _GetParser( ai_pccPresentation ) ;
	M4MDT_RETURN_ERROR3( pIParser == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTPresentationTool::Presentation2OBL", "_GetParser", ai_pccPresentation, "%s#%s#%s" ) ;


	hr = pIParser->PropBagDefFromOBL( pIAnle202, ai_pcsOBL, &pIPBDef ) ;
	M4MDT_RETURN_ERROR2( hr != S_OK || pIPBDef == NULL, M4_ERROR, M4MDTTERROR_NO_PRES_FROM_OBL, hr, ai_pccPresentation, "%d#%s" ) ;

	hr = pIPBDef->get_ChildColDef( &pIChildColDef ) ;
	pIPBDef->Release() ;
	pIPBDef = NULL ;
	M4MDT_RETURN_ERROR2( hr != S_OK || pIChildColDef == NULL, M4_ERROR, M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES, hr, ai_pccPresentation, "%d#%s" ) ;

	hr = pIChildColDef->get_Count( &lCount ) ;
	M4MDT_CLEAN_ERROR2( hr != S_OK, M4_ERROR, M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES, pIChildColDef->Release(), hr, ai_pccPresentation, "%d#%s" ) ;

	if( lCount == 0 )
	{
		pIChildColDef->Release() ;
		pIChildColDef = NULL ;

		hr = pIParser->PropBagDefFromOBL( pIAnle203, ai_pcsOBL, &pIPBDef ) ;
		M4MDT_RETURN_ERROR2( hr != S_OK || pIPBDef == NULL, M4_ERROR, M4MDTTERROR_NO_PRES_FROM_OBL, hr, ai_pccPresentation, "%d#%s" ) ;

		hr = pIPBDef->get_ChildColDef( &pIChildColDef ) ;
		pIPBDef->Release() ;
		pIPBDef = NULL ;
		M4MDT_RETURN_ERROR2( hr != S_OK || pIChildColDef == NULL, M4_ERROR, M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES, hr, ai_pccPresentation, "%d#%s" ) ;

		hr = pIChildColDef->get_Count( &lCount ) ;
		M4MDT_CLEAN_ERROR2( hr != S_OK || lCount != 1, M4_ERROR, M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES, pIChildColDef->Release(), hr, ai_pccPresentation, "%d#%s" ) ;
	}

	vIndex.vt = VT_I4 ;
	vIndex.lVal = 1 ;
	hr = pIChildColDef->get_Item( vIndex, &pIPBDefChild ) ;
	pIChildColDef->Release() ;
	pIChildColDef = NULL ;
	M4MDT_RETURN_ERROR2( hr != S_OK || pIPBDefChild == NULL, M4_ERROR, M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES, hr, ai_pccPresentation, "%d#%s" ) ;

	ao_rpIPBDef = pIPBDefChild ;

	return( M4_SUCCESS ) ;
}


