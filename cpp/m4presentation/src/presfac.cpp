//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presfac.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    
//
//
//==============================================================================

#include "presfac.hpp"
#include "cldates.hpp"
#include "clpresen.hpp"
#include "prescache.hpp"
#include "m4objreg.hpp"
#include "cachedir.hpp"
#include "m4presres.hpp"
#include "chlog.hpp"
#include "sttwrap.hpp"
#include "sttlib.hpp"
#include "m4cstrace.hpp"
#include "asndef.h"
#include "attributes.hpp"
#include "dm.hpp"
#include "m4mdfac.hpp"


// Descomentar estos includes para usar la funcion _debugPresentationToFile
// Se debe insertar ../../../m4prop en el directorio de INCLUDE del Visual Studio. 
// Y m4prop.lib en las librerias

// #include "..\..\m4prop\inc\stdafx.h"
// #include "crtdbg.h"
// #include "m4prop.h"
// #include "m4prop_i.c"
// #include "definitions_i.c"
// #include "_base64.h"
// #include "m4unicode.hpp"
// #include "definitions.h"

// factoria ===================================================

ClPresentationFactory::ClPresentationFactory (void)
{
	m_poChannelManager = NULL;
	m_poPresCache = 0;
	m_poSttWrapper = 0;
	m_poCacheDir = 0;
	m_poCsTrace = 0;
	m_bInit = M4_FALSE;	
}

ClPresentationFactory::~ClPresentationFactory (void)
{
	_End();
}


m4return_t ClPresentationFactory::InitCache (ClCache * ai_poPresCache, ClVMBaseEnv * ai_poRegistry)
{
	m4return_t ret = M4_SUCCESS;
	m4uint8_t iMode;

	if (!ai_poRegistry)
		return M4_ERROR;

	iMode = ai_poRegistry->GetMDDiskCache() ;
	
	//Ya esta inicializada
	if (m_poPresCache)
		return M4_SUCCESS;

	// No caches
	if (iMode == M4CL_CACHE_NONE)
		return M4_SUCCESS;

	if (!ai_poPresCache)
		return M4_SUCCESS;

	m_poPresCache = new ClPresentationCache( ai_poPresCache, ai_poRegistry->GetCacheDirectory(), iMode, ai_poRegistry->GetMDCacheMaxMemory(), ai_poRegistry->GetMDCacheMaxNumObjects(), ai_poRegistry->GetMDCacheRefreshRatio(), 0, 0);

	if (m_poCacheDir)
	{
		m_poCacheDir->Attach(m_poPresCache);
	}
	
	return M4_SUCCESS;
}

m4return_t ClPresentationFactory::_End()
{
	m_poChannelManager = NULL;

	if (m_poPresCache)
	{
		if (m_poCacheDir)
		{
			m_poCacheDir->Detach(m_poPresCache);
		}
		delete m_poPresCache;
		m_poPresCache = 0;
	}

	m_poSttWrapper = 0;
	m_poCacheDir = 0;
	m_poCsTrace = 0;

	return M4_SUCCESS;
}


m4return_t ClPresentationFactory::BuildPresentationFromId (m4pchar_t ai_pcIdPresentation, m4pcchar_t ai_pcOrganization, m4pcchar_t ai_pcRole, m4uint8_t ai_iLanguage, m4bool_t ai_bMappingAllowed, m4bool_t ai_bDefinition, ClPresentation* &ao_rpPresentation)
{

	m4return_t iResult = M4_ERROR;
	ClPresentation* pPresentationAux = NULL ;


	ao_rpPresentation = 0;

	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	if (ai_pcIdPresentation == NULL) {
		return M4_ERROR;
	}

	// Se calculan los mapeos
	m4pchar_t	pcMappingId = NULL ;
	m4pchar_t	pcPresentation = NULL ;

	pcPresentation = ai_pcIdPresentation ;

	// Si estamos en tiempo de diseño no usamos mapeos
	if( ai_bDefinition == M4_FALSE && ai_pcOrganization != NULL )
	{
		pcMappingId = m_poChannelManager->GetpFactory()->MDFac_GetMappingById( "M4PRES_MAPPINGS", ai_pcOrganization, pcPresentation, M4_TRUE ) ;

		if( pcMappingId != NULL )
		{
			/* Bug 0109799
			Si hay mapeo y hay que dar error se da error y se corta
			*/
			CHECK_CHLOG_ERRORF( ai_bMappingAllowed == M4_FALSE, M4_ERROR, M4_CH_PRES_MAPPING_NOT_ALLOWED, pcPresentation << pcMappingId << ai_pcOrganization ) ;

			/* Bug 0106669
			No se puede limitar el tamaño del id de la presentación a 30
			*/
			pcPresentation = pcMappingId ;
		}
	}


	//--------------------------------------------------
	//Aqui se inicializa el objeto que se ira pasando como argumento a los diferente metodos.
	//El objeto se crea con el id presentacion LOGICA (sin tener en cuenta la organizacion).	
	//La presentación real se calcula en la parte server!.
	//Por lo tanto el cliente maneja presentaciones lógicas y el server presentaciones reales.
	ClPresentationAttributes oClPresentationAttributes(pcPresentation, ai_pcOrganization, ai_pcRole, ai_iLanguage);
	//--------------------------------------------------

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer(m_poSttWrapper->GetStatistics(), 
								   m_poSttWrapper->GetStatisticsLevel(), 
								   M4_VM_STT_SPECIAL_ITEM_CREATE_PRESENTATION);

	if ( M4_TRUE == oSttStateTimer.GetSttEnabled() )
	{
		//Ponemos el ID como general data a las Stt
		oSttStateTimer.GetStatistics()->sttAddGeneralData(pcPresentation);
	}

	// Si estamos en tiempo de diseño no usamos cachés
	if (ai_bDefinition == M4_FALSE && m_poPresCache)
	{
		iResult = m_poPresCache->GetObject (oClPresentationAttributes, ao_rpPresentation);

		if ( M4_TRUE == oSttStateTimer.GetSttEnabled() )
		{
			//Ponemos el cachehit a las Stt
			m_poCsTrace->PrePresentationTrace(); // From Cache
			m_poCsTrace->PostPresentationTrace(0,
											   0,
											   pcPresentation,
											   oClPresentationAttributes,
											   M4_TRUE);

			oSttStateTimer.GetStatistics()->sttAddCacheHit( (iResult == M4_SUCCESS) ? M4_TRUE : M4_FALSE );
		}
	}

	if ( M4_TRUE == oSttStateTimer.GetSttEnabled() )
	{
		if (oSttStateTimer.GetStatistics()->sttGetInitState() & M4_VM_STATISTICS_ENABLED_PARAMS)
		{
			oSttStateTimer.GetStatistics()->sttAddExecParam((m4pchar_t)pcPresentation);
			oSttStateTimer.GetStatistics()->sttAddExecParam((m4pchar_t)oClPresentationAttributes.m_pOrganization);
			oSttStateTimer.GetStatistics()->sttAddExecParam((m4pchar_t)oClPresentationAttributes.m_pRole);
			oSttStateTimer.GetStatistics()->sttAddExecParam((m4double_t)oClPresentationAttributes.m_iLanguage);
		}
	}

	if (iResult == M4_ERROR)
	{	

		iResult = _InternalBuildPresentationFromId(	oClPresentationAttributes, ai_bDefinition, ao_rpPresentation );

		// Descomentar cuando se esté depurando la construcción de la presentación
		// #ifdef _DEBUG
		//	_debugPresentationToFile (ao_rpPresentation, oClPresentationAttributes) ; 
		// #endif

		// En tiempo de diseño, no cachés
		if (ai_bDefinition == M4_FALSE && m_poPresCache)
		{
			if(iResult == M4_SUCCESS )
			{			
				// Borramos de la caché todas las presentaciones con el mismo ID y versión anterior.
				m_poPresCache->RemoveObjectByIdAndVersion(pcPresentation, ao_rpPresentation->GetVersion(), ClBaseCacheInterface::MAIN);

				// Hacemos el Put de la caché si no está
				iResult = m_poPresCache->GetObject (oClPresentationAttributes, pPresentationAux);

				if( iResult == M4_SUCCESS )
				{
					if( pPresentationAux != NULL )
					{
						pPresentationAux->Destroy() ;
						pPresentationAux = NULL ;
					}
				}
				else
				{
					iResult = m_poPresCache->PutObject (oClPresentationAttributes, ao_rpPresentation);

					if (iResult == M4_ERROR)
					{
						g_oChLog<<BeginWarning(M4_CH_PRES_CACHE_PUT_PRESENTATION)<<pcPresentation<<oClPresentationAttributes.m_pRole<<EndLog;
						iResult = M4_SUCCESS;
					}
				}
			}
			else
			{
				// Si no fue bien hacer UndoPut.	
			}
		}
	}

	if( pcMappingId != NULL )
	{
		/* Bug 0106669
		No se puede limitar el tamaño del id de la presentación a 30
		*/
		delete [] pcMappingId ;
	}

	return iResult;
}





//-----------------------------------------------------------------------
//------------------- EXPLICACION ---------------------------------------
//-----------------------------------------------------------------------
//Los GetObject buscan presentaciones en la cache, del cliente o del server. Se puede buscar la básica o la real;
//la básica no hay role (es la que se obtiene de BD en funcion del idioma), se cachea para no tener que lanzar
//la select. La real es la que se ha construido definitivamente.
//
//	-- Cliente --										-- Server --
//	BuildPresentationFromId
//		GetObject (No básica)
//		Client::_InternalBuildPresentationFromId
//			RemoteGetPresentation
//			.										BuildPresentationFromId
//			.											GetObject (No Básica)
//			.											Server::_InternalBuildPresentationFromId
//			.												GetObject (Básica)
//			.												BuildBasicPresentationFromId (Básica, construye si no estaba)
//			.												PutObject (Básica, inserta en cache)
//			.													Serialize
//			.												BuildPresentation (Analizador..., crea la No básica)
//			.											GetObject (No básica)
//
//** En esta punto, el server analizara la presentacion y los includes. El problema es que el PuObject que se ha hecho
//** sobre la presentacion básica y no tiene la información de includes, canales... en la cache.
//** Cuando se ha analizado la presentacion y sus includes continuamos, dejando en la cache la presentacion.

//			.											PutObject (No básica)
//			.												Serialize
//			.											Serialize (BuildFromResponse, pasamos al cliente).
//				Deserialize (Deserializa la respuesta)				.
//		GetObject (No básica)										.
//		PutObject (No básica, si no estaba en cache)				.
//
//** Las presentaciones se habrán quedado cacheadas en cliente y en server.

//Tenemos dos problemas:
//1.- Cuando la presentacion no existe (en el repositorio) el BufferASN es "NOT_FOUND" (porque se trata de Run Time).
//Al detectarse "NOT_FOUND" no se analiza la presentación y por lo tanto no hay presentaciones en el vector que
//se emplea para comprobar obsolescencia.
//La segunda vez que se cargue, la encontraremos en la cache e intentaremos comprobar la obsolescencia; sin embargo 
//NO hay presentacion, ni si quiera la principal, esto provoca un error en la ejecución del metodo pq se asume que 
//por lo menos tendria que haber una presentacion (la propia).
//	-Solucion-: Detectarlo y retornar que es obsoleta!.

//2.- Cuando una principal incluye dos veces a un include, en la segunda ocasión se detecta que está en cache
//y se intenta comprobar la obsolescencia.
//En la cache de disco, el include NO guarda los id subincludes, los argumentos estan sin resolver por lo tanto
//un include SIEMPRE tiene que construirse pq los argumentos se resuelven en Run Time, por lo tanto
//podemos asumir que siempre estará obsoleta!.
//Es importante tener en cuenta que aunque la presentacion final es obsoleta, la básica no lo será y si estará
//cacheada correctamente!.
//	-Solucion-: Detectarlo y retornar que es obsoleta!.

//-----------------------------------------------------------------------
//------------------- FIN EXPLICACION ---------------------------------------
//-----------------------------------------------------------------------

/*
m4return_t ClPresentationFactory::_debugPresentationToFile (  ClPresentation * ai_rpPresentation, ClPresentationAttributes ai_oClPresentationAttributes)
{		
		
		// initialize file variables
		m4char_t *filename = new m4char_t[256];			
		BSTR *pstrOBL = NULL; 
		BSTR pFileName = NULL; 

		// initialize propbag/convertpropbag
		ICPropBag * ppPBRet = NULL;	
		IConvertPropBag *oPropBag = NULL;
		void*	pvResult = NULL ;

		// obtain the presentation ASN string, size and id
		char *	poBuffer				= (char*)ai_rpPresentation->GetBuffer() ;
		m4int32_t	cElems				= ai_rpPresentation->GetSize() ;
		const char * pcPresentationID	= ai_oClPresentationAttributes.m_pIdPresentation;  

		// binary file
		sprintf(filename, "c:\\temp\\prestrace_%s.bin", pcPresentationID);		

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		FILE*	pfFile = M4Fopen( filename, M4UniWriteBinary, eEncoding ) ;
		fwrite( poBuffer, 1, cElems, pfFile ) ;
		fclose( pfFile ) ;
	
		// load the filename into a BSTR, filename uses the presentation id
		sprintf(filename, "c:\\temp\\prestrace_%s.txt", pcPresentationID);		
		pFileName = M4CppToVB ( filename ) ; 
		
		// load the presentation ASN string into a BSTR
		BSTR bStrIn = M4CppToVBBinary ( poBuffer , cElems ) ; 

		// create the propbag (object tree) from the ASN string and dump to a file if successful
		HRESULT	hr = CoInitializeEx( NULL, COINIT_MULTITHREADED ) ;
		hr = CoCreateInstance( CLSID_ConvertPropBag, NULL, CLSCTX_INPROC_SERVER, IID_IConvertPropBag, (void**) &pvResult ) ;
		oPropBag = (IConvertPropBag *) pvResult ;	
		hr = oPropBag->PBFromASNString(bStrIn, &ppPBRet);  
		
		if (ppPBRet != NULL) {
			hr = oPropBag->PB2FileOBL(ppPBRet, pFileName);
		}
		
		delete pstrOBL;
		delete filename; 

		if (hr == 0) 
		{
			return M4_SUCCESS; 
		}
		
		return M4_ERROR; 
}

*/