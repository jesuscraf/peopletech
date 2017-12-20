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
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//	====================
//## end module.cp

//## Module: ClPConfiguration; Package body
//## Subsystem: CPM4Configuration
//## Source file: D:\eduardoma\M4Configuration\version\src\clpconfiguration.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <cloblconfiguration.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module.includes

// Cryptstore
#include <m4cryptstore.hpp>
// ClPConfiguration
#include <clpconfiguration.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClPropertiesConfiguration 


ClPropertiesConfiguration::ClPropertiesConfiguration (m4pchar_t ai_szProperties)
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.hasinit preserve=no
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.hasinit
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.initialization preserve=yes
  :ClM4ConfigTree ( ai_szProperties )
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.initialization
{
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.body preserve=yes
	// Bug 0217974. Inicializar miembro.
	m_poCryptStore = NULL;
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913835325.body
}

ClPropertiesConfiguration::ClPropertiesConfiguration (ClOblConfiguration *ai_poConfiguration, UINT ai_hHandle, ClCryptStore *ai_poCriptFile)
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.hasinit preserve=no
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.hasinit
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.initialization preserve=yes
  :	m_poCryptStore(ai_poCriptFile)
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.initialization
{
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.body preserve=yes
	UINT hHandle ;
	UINT hProp ;
	UINT hMatch ;
	m4return_t iRet ;
	m4char_t szName [MAXNAMEPROP] = "";
	m4char_t szValue [MAXNAMEPROP] = "";
	m4char_t szPathBlock [1000] = "" ;
	UINT iLenValue , iLenName , auiBlockHandles[100] ;
	size_t riHandleLength ;
	m4uint32_t iBlock;
	m4char_t szClass [MAXNAMEPROP] ={0};
	UINT iLenClass  ;
	m4char_t szHinst [MAXNAMEPROP] = {0};
	UINT iLenHinst  ;


	m_hBlock = ai_hHandle;

	//Recupera el handle del fichero OBL
	hHandle = ai_poConfiguration-> GetHandle ( ) ;

	iRet = M4AOblGetClassBlqHandle ( hHandle , ai_hHandle , szClass , MAXNAMEPROP, &iLenClass ) ;

	iRet = M4AOblGetHinstBlqHandle ( hHandle , ai_hHandle , szHinst , MAXNAMEPROP , &iLenHinst ) ;

	SetClassHinst ( szClass , szHinst ) ;
	//Consigue el path del bloque que estamos convirtiendo a 
	ai_poConfiguration-> GetPathBlq ( ai_hHandle , szPathBlock ) ;

	//se hace una primera búsqueda para localizar la primera propiedad
	iRet = M4AOblFirstHndProp ( hHandle , szPathBlock , "*" , & hProp , &hMatch ) ;

	if (iRet != OBL_SUCCESS )
	{
		return ;
	}

	iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , MAXNAMEPROP , &iLenName , szValue , MAXNAMEPROP ,&iLenValue ) ;

	while (iRet == OBL_SUCCESS )
	{
		m_oProperties.SetProperty ( szName ,szValue , strlen ( szValue ) ) ;

		iRet = M4AOblNextHndProp ( hHandle , hMatch , & hProp ) ;
		

		if ( iRet == OBL_SUCCESS )
		{

			iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , MAXNAMEPROP , &iLenName , szValue , MAXNAMEPROP ,&iLenValue ) ;
			
		}
	}

	if ( szPathBlock [0] )
		strcat ( szPathBlock , "." ) ;

	strcat ( szPathBlock , "*.*" ) ;

	iRet = ai_poConfiguration -> GetAllMatchingBlocks (szPathBlock, auiBlockHandles, 100, riHandleLength) ;

	if ( iRet == M4_SUCCESS )
	{
		for ( iBlock = 0 ; iBlock < riHandleLength ; iBlock ++ )
		{
			ClPropertiesConfiguration * poPropertiesConfiguration = new ClPropertiesConfiguration ( ai_poConfiguration , auiBlockHandles [ iBlock] ) ;

			m_oPropertyBlocks.insert ( ClPropertyBlocks::value_type ( auiBlockHandles [ iBlock] , poPropertiesConfiguration ) ) ;

		}
	}

  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%913888151.body
}

ClPropertiesConfiguration::ClPropertiesConfiguration (ClConfiguration *ai_poConfiguration)
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.hasinit preserve=no
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.hasinit
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.initialization preserve=yes
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.initialization
{
  //## begin ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.body preserve=yes
	// Bug 0217974. Inicializar miembro.
	m_poCryptStore = NULL;
  //## end ClPropertiesConfiguration::ClPropertiesConfiguration%917867896.body
}


ClPropertiesConfiguration::~ClPropertiesConfiguration ()
{
  //## begin ClPropertiesConfiguration::~ClPropertiesConfiguration%913888152.body preserve=yes
  //## end ClPropertiesConfiguration::~ClPropertiesConfiguration%913888152.body
}



//## Other Operations (implementation)
int ClPropertiesConfiguration::GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClPropertiesConfiguration::GetStringValue%917867897.body preserve=yes
	m4return_t iRet = ClM4ConfigTree::GetStringValue (ai_pszPath, ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength);

	if ( M4_ERROR == iRet )
		return M4_ERROR ;

	// Bug 0217974. Verificar si tenemos cryptstore inicializado.
	if (m_poCryptStore == NULL)
	{
		return M4_SUCCESS;
	}

	if ( ao_pszString[0] == '@' )
	{
		m4char_t szValue [1024];
		m4int_t lvalor;
		
		iRet = m_poCryptStore -> ExtractData ( ao_pszString +1 , ao_riStringLength -2, szValue, 1024,lvalor);
		
		if ( M4_ERROR == iRet )
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR , ERRORLOG , "Property %s is expected to be into crypt file, but it couldn't be found" , ao_pszString ) ;
		}
	}

	return iRet ;
  //## end ClPropertiesConfiguration::GetStringValue%917867897.body
}

// Additional Declarations
  //## begin ClPropertiesConfiguration.declarations preserve=yes
  //## end ClPropertiesConfiguration.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
