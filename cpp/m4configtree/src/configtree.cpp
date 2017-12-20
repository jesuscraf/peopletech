//## begin module%36792AA6022E.cm preserve=no
//## end module%36792AA6022E.cm

//## begin module%36792AA6022E.cp preserve=no
//	=============================================================================
//	=
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
//	=============================================================================
//## end module%36792AA6022E.cp

//## Module: ConfigTree%36792AA6022E; Package body
//## Subsystem: M4ConfigTree::src%379C72840108
//## Source file: F:\integration\M4ConfigTree\src\configtree.cpp

//## begin module%36792AA6022E.additionalIncludes preserve=no
//## end module%36792AA6022E.additionalIncludes

//## begin module%36792AA6022E.includes preserve=yes
#include <stdio.h>
//## end module%36792AA6022E.includes

// syncro
#include <syncro.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// properties
#include <properties.hpp>
// ConfigTree
#include <configtree.hpp>
//## begin module%36792AA6022E.declarations preserve=no
//## end module%36792AA6022E.declarations

//## begin module%36792AA6022E.additionalDeclarations preserve=yes
//## end module%36792AA6022E.additionalDeclarations


// Class ClM4ConfigTree 






ClM4ConfigTree::ClM4ConfigTree (ClConfiguration *ai_poConfiguration)
  //## begin ClM4ConfigTree::ClM4ConfigTree%917549077.hasinit preserve=no
  //## end ClM4ConfigTree::ClM4ConfigTree%917549077.hasinit
  //## begin ClM4ConfigTree::ClM4ConfigTree%917549077.initialization preserve=yes
  //## end ClM4ConfigTree::ClM4ConfigTree%917549077.initialization
{
  //## begin ClM4ConfigTree::ClM4ConfigTree%917549077.body preserve=yes
	m4return_t	iRet = M4_SUCCESS;
	m4char_t	szProperty [MAXNAMEPROP], szValue[MAXNAMEPROP] ;
	m4uint32_t	iPropLen , iValueLen , iHndProp;
	m4void_t *	iHndMatch;
	
	m_oSynchro.Init ( ) ;

	( (ClM4ConfigTree*)ai_poConfiguration) -> m_oSynchro.Lock () ;

	iRet = ai_poConfiguration -> GetFirstProp ( "" , szProperty , MAXNAMEPROP , iPropLen , szValue , MAXNAMEPROP , iValueLen , iHndProp , iHndMatch ) ;

	if ( M4_SUCCESS == iRet )
	{
		iRet = SetStringValue( "" , szProperty  , szValue ) ;
	}

	while ( iRet == M4_SUCCESS )
	{
		iRet = ai_poConfiguration -> GetNextProp ( "" , szProperty , MAXNAMEPROP , iPropLen , szValue , MAXNAMEPROP , iValueLen , iHndProp , iHndMatch ) ;

		if ( M4_SUCCESS == iRet )
		{
			iRet = SetStringValue( "" , szProperty  , szValue ) ;
		}
	}

	( (ClM4ConfigTree*)ai_poConfiguration) -> m_oSynchro.Unlock () ;

  //## end ClM4ConfigTree::ClM4ConfigTree%917549077.body
}

ClM4ConfigTree::ClM4ConfigTree (m4pchar_t ai_szProperties)
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982492.hasinit preserve=no
  //## end ClM4ConfigTree::ClM4ConfigTree%913982492.hasinit
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982492.initialization preserve=yes
  //## end ClM4ConfigTree::ClM4ConfigTree%913982492.initialization
{
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982492.body preserve=yes
	m_oSynchro.Init ( ) ;

	m_oSynchro.Lock () ;
	m4return_t iRet = (m4return_t) m_oProperties.SetStringProperties ( ai_szProperties ) ;
	m_oSynchro.Unlock () ;
  //## end ClM4ConfigTree::ClM4ConfigTree%913982492.body
}

ClM4ConfigTree::ClM4ConfigTree ()
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982493.hasinit preserve=no
  //## end ClM4ConfigTree::ClM4ConfigTree%913982493.hasinit
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982493.initialization preserve=yes
  //## end ClM4ConfigTree::ClM4ConfigTree%913982493.initialization
{
  //## begin ClM4ConfigTree::ClM4ConfigTree%913982493.body preserve=yes
	m_oSynchro.Init ( ) ;
  //## end ClM4ConfigTree::ClM4ConfigTree%913982493.body
}



//## Other Operations (implementation)
int ClM4ConfigTree::GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClM4ConfigTree::GetNumericValue%913910690.body preserve=yes
	m4int32_t iNumber ;
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, iNumber ) ;

		if (iRet == M4_SUCCESS)
			ao_rdNumber = iNumber ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}


	m_oSynchro.Unlock () ;

	return iRet;
  //## end ClM4ConfigTree::GetNumericValue%913910690.body
}

int ClM4ConfigTree::GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClM4ConfigTree::GetNumericValue%913910691.body preserve=yes
	m4return_t iRet ;
	m4int32_t iNumber ;

	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, iNumber ) ;

		if ( iRet == M4_SUCCESS )
			ao_rdNumber = iNumber ;

	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::GetNumericValue%913910691.body
}

int ClM4ConfigTree::GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClM4ConfigTree::GetStringValue%913910692.body preserve=yes
	m4return_t iRet ;

	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, ao_pszString, ai_iStringSize) ;

		if ( iRet == M4_SUCCESS )
			ao_riStringLength = strlen ( ao_pszString ) ;

	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}

	m_oSynchro.Unlock () ;

	return iRet;
  //## end ClM4ConfigTree::GetStringValue%913910692.body
}

int ClM4ConfigTree::GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClM4ConfigTree::GetStringValue%913910693.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, ao_pszString, ai_iStringSize) ;

		if ( iRet == M4_SUCCESS )
			ao_riStringLength = strlen ( ao_pszString ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::GetStringValue%913910693.body
}

int ClM4ConfigTree::GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClM4ConfigTree::GetDateValue%913910694.body preserve=yes
	m4return_t iRet ;
	m4int32_t iDate ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, iDate ) ;

		if ( iRet == M4_SUCCESS )
			ao_rdDate = iDate ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet;
  //## end ClM4ConfigTree::GetDateValue%913910694.body
}

int ClM4ConfigTree::GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClM4ConfigTree::GetDateValue%913910695.body preserve=yes
	m4return_t iRet ;
	m4int32_t iDate ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.GetProperty ( ai_pszName, iDate ) ;

		if ( iRet == M4_SUCCESS )
			ao_rdDate = iDate ;

	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet;
  //## end ClM4ConfigTree::GetDateValue%913910695.body
}

int ClM4ConfigTree::SetNumericValue (char* ai_pszPath, char* ai_pszName, double ai_dNumber)
{
  //## begin ClM4ConfigTree::SetNumericValue%913910696.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, (m4int32_t) ai_dNumber ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::SetNumericValue%913910696.body
}

int ClM4ConfigTree::SetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double ai_dNumber)
{
  //## begin ClM4ConfigTree::SetNumericValue%913910697.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, (m4int32_t) ai_dNumber ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::SetNumericValue%913910697.body
}

int ClM4ConfigTree::SetStringValue (char* ai_pszPath, char* ai_pszName, char* ai_pszString)
{
  //## begin ClM4ConfigTree::SetStringValue%913910698.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszString , strlen ( ai_pszString ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::SetStringValue%913910698.body
}

int ClM4ConfigTree::SetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszString)
{
  //## begin ClM4ConfigTree::SetStringValue%913910699.body preserve=yes
	m4return_t iRet ;
	
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszString , strlen ( ai_pszString ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::SetStringValue%913910699.body
}

int ClM4ConfigTree::SetDateValue (char* ai_pszPath, char* ai_pszName, char* ai_pszDate)
{
  //## begin ClM4ConfigTree::SetDateValue%913910700.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszDate , strlen ( ai_pszDate ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::SetDateValue%913910700.body
}

int ClM4ConfigTree::SetDateValue (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszDate)
{
  //## begin ClM4ConfigTree::SetDateValue%913910701.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszDate , strlen ( ai_pszDate ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::SetDateValue%913910701.body
}

int ClM4ConfigTree::AddNumericProperty (char* ai_pszPath, char* ai_pszName, double ai_dNumber)
{
  //## begin ClM4ConfigTree::AddNumericProperty%913910702.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, (m4int32_t) ai_dNumber ) ;
	}else
	{
		m_oSynchro.Unlock () ;

		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::AddNumericProperty%913910702.body
}

int ClM4ConfigTree::AddNumericProperty (UINT ai_hBlockHandle, char* ai_pszName, double ai_dNumber)
{
  //## begin ClM4ConfigTree::AddNumericProperty%913910703.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName , (m4int32_t) ai_dNumber ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::AddNumericProperty%913910703.body
}

int ClM4ConfigTree::AddStringProperty (char* ai_pszPath, char* ai_pszName, char* ai_pszString)
{
  //## begin ClM4ConfigTree::AddStringProperty%913910704.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszString , strlen ( ai_pszString ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::AddStringProperty%913910704.body
}

int ClM4ConfigTree::AddStringProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszString)
{
  //## begin ClM4ConfigTree::AddStringProperty%913910705.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszString , strlen ( ai_pszString ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::AddStringProperty%913910705.body
}

int ClM4ConfigTree::AddDateProperty (char* ai_pszPath, char* ai_pszName, char* ai_pszDate)
{
  //## begin ClM4ConfigTree::AddDateProperty%913910706.body preserve=yes
	m4return_t iRet ;
	
	m_oSynchro.Lock () ;
	if ( ! strcmp ( ai_pszPath , "" ) )
	{
		iRet = (m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszDate , strlen ( ai_pszDate ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;


	return iRet ;
  //## end ClM4ConfigTree::AddDateProperty%913910706.body
}

int ClM4ConfigTree::AddDateProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszDate)
{
  //## begin ClM4ConfigTree::AddDateProperty%913910707.body preserve=yes
	m4return_t iRet ;
	
	
	m_oSynchro.Lock () ;
	if ( ai_hBlockHandle == m_hBlock ) 
	{
		iRet = ( m4return_t) m_oProperties.SetProperty ( ai_pszName, ai_pszDate , strlen ( ai_pszDate ) ) ;
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::AddDateProperty%913910707.body
}

int ClM4ConfigTree::AddBlock (char* ai_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle)
{
  //## begin ClM4ConfigTree::AddBlock%913910708.body preserve=yes
	return M4_ERROR ;
  //## end ClM4ConfigTree::AddBlock%913910708.body
}

int ClM4ConfigTree::WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut)
{
  //## begin ClM4ConfigTree::WriteOut%913910709.body preserve=yes
	m4int32_t i , iValue ;
	m4uint32_t iContlevels = 0 ;
	m4uint32_t  iLenLine , iLenFile , iLenProp , iLenValue , iHndProp , iLenBuf;
	m4void_t * iHndMatch;
	m4pchar_t pAux ;
	m4char_t szProperty [MAXNAMEPROP], szValue[MAXNAMEPROP] , szComillas [2] = { '\"' , 0 } , szAux[MAXNAMEPROP];
	m4return_t iRet ;
	ClPropertyBlocks::iterator Iterator ;


	iLenFile = strlen ( ao_szTextOut ) ;


	pAux = ao_szTextOut + iLenFile -1 ;

	//contar el nº de tabulaciones que tiene el bloque
	while ( (  pAux[0] == '\t') && ( iContlevels < iLenFile) )
	{
		iContlevels ++ ;
		pAux -- ;
	}

	if ( ! iContlevels )
		iLenFile = 0;


	pAux = ao_szTextOut + iLenFile ;

	if ( iContlevels == 0 )
	{
		ao_szTextOut [0] = 0 ;

		sprintf ( ao_szTextOut , "%s\n" , "VERSION 02.00" ) ;

		iLenFile = 14 ;

		pAux += 14 ;
	}

	iLenBuf = ai_iLenBuf - iLenFile ;
	
	//Escribir la cabecera del bloque: clase e instancia

	iLenLine = 9 + m_szClass.size () -1 + m_szHinst.size () -1 ;

	if ( iLenBuf < iLenLine )
	{
		return M4_ERROR ;
	}


	sprintf ( pAux , "%s %s %s\n" , "BEGIN" ,  m_szClass.begin() , m_szHinst.begin() ) ;

	//avanza el puntero
	pAux += iLenLine -1;
	iLenBuf -= iLenLine -1;

	//Esta llamada se hace porque está desactivada la sincronización en las funciones getnextprop y getfirstprop
	m_oSynchro.Lock () ;

	iRet = GetFirstProp ("", szProperty, MAXNAMEPROP, iLenProp, szValue, MAXNAMEPROP, iLenValue, iHndProp, iHndMatch) ;

	while ( M4_SUCCESS == iRet )
	{
	
		iLenLine = iContlevels + 1 + strlen (szProperty) + strlen(szValue) + 4  ;

		if ( iLenLine > iLenBuf )
		{
			m_oSynchro.Unlock () ;

			return M4_ERROR ;
		}

		for  ( i = 0 ; i < iContlevels + 1; i++ )
		{
			sprintf ( pAux + i , "%s" , "\t" ) ;
		}

		//Cuadno la propiedad es noo es numérica, lleva las comillas
//	OSCAR sscanf falla cuando son fechas o campos con numéricos y carateres
		//por ese motivo hay que hacer una triple comparación
		if ( ( sscanf ( szValue , "%d\0" , &iValue )  ) 
			&& ( 2 != sscanf ( szValue , "%d%s" , &iValue , szAux )  ) 
			&& (  strcmp ( "" , szValue ) ) )
		{
			szComillas [0] = 0 ;
		}
		else 
		{
			szComillas [0] = '\"' ;

			iLenLine += 2 ;
		}
		
		sprintf ( pAux +i, "%s = %s%s%s\n" , szProperty , szComillas , szValue , szComillas ) ;

		pAux += iLenLine ;
	
		iRet = GetNextProp ("", szProperty, MAXNAMEPROP, iLenProp, szValue, MAXNAMEPROP, iLenValue, iHndProp, iHndMatch) ;
		
	}

	m_oSynchro.Unlock () ;


		//Dejar el puntero al siguiente bloque
	for  ( i = 0 ; i < iContlevels + 1; i++ )
	{
		sprintf ( pAux , "%s" , "\t" ) ;

		pAux++ ;
	}



	for ( Iterator = m_oPropertyBlocks.begin ( ) ;
		Iterator != m_oPropertyBlocks.end ( ) ;
		Iterator ++ )
		{
			(*Iterator).second -> WriteOut (ao_szTextOut, ai_iLenBuf, ao_iLenOut) ;
		}

		pAux = ao_szTextOut + strlen ( ao_szTextOut ) ;

		while ( (*(pAux -1)) =='\t' )
			pAux -- ;

		//Poner END al bloque
		for  ( i = 0 ; i < iContlevels ; i++ )
		{
			sprintf ( pAux + i , "%s" , "\t" ) ;
		}

		sprintf ( pAux + i , "%s\n" , "END" ) ;

		pAux += i + 4 ;

		for  ( i = 0 ; i < iContlevels ; i++ )
		{
			sprintf ( pAux + i , "%s" , "\t" ) ;
		}

		ao_iLenOut = strlen ( ao_szTextOut) ;

	return M4_SUCCESS ;
  //## end ClM4ConfigTree::WriteOut%913910709.body
}

int ClM4ConfigTree::GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength)
{
  //## begin ClM4ConfigTree::GetAllMatchingBlocks%913910710.body preserve=yes
	ClPropertyBlocks::iterator Iterator ;
	
	m_oSynchro.Lock () ;

	ao_riHandleLength = 0 ;
	if ( ( ! strcmp ( ai_pszPath , "*.*" ) ) || ( ! strcmp ( ai_pszPath , "" ) ) )
	{
		for ( Iterator = m_oPropertyBlocks.begin ( ) ;
			Iterator != m_oPropertyBlocks.end ( ) ;
			Iterator ++ )
			{
			ao_auiBlockHandles [ao_riHandleLength++] = (*Iterator).first ;
			}
	}else
	{
		m_oSynchro.Unlock () ;
		return M4_ERROR ;
	}
	m_oSynchro.Unlock () ;
	return M4_SUCCESS ;
  //## end ClM4ConfigTree::GetAllMatchingBlocks%913910710.body
}

int ClM4ConfigTree::GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq)
{
  //## begin ClM4ConfigTree::GetPathBlq%913910711.body preserve=yes
	return M4_ERROR ;
  //## end ClM4ConfigTree::GetPathBlq%913910711.body
}

ClConfiguration * ClM4ConfigTree::GetBloqConfiguration (m4uint32_t ai_hBlq)
{
  //## begin ClM4ConfigTree::GetBloqConfiguration%913910712.body preserve=yes
	ClPropertyBlocks::iterator Iterator ;
	ClConfiguration * poConfiguration ;
	m_oSynchro.Lock () ;

	Iterator = m_oPropertyBlocks.find ( ai_hBlq ) ;

	if ( Iterator == m_oPropertyBlocks.end ( ) )
	{
		poConfiguration = NULL ;
	}
	else
	{
		poConfiguration = (*Iterator).second ;
	}

	m_oSynchro.Unlock () ;

	return poConfiguration ;
  //## end ClM4ConfigTree::GetBloqConfiguration%913910712.body
}

m4return_t ClM4ConfigTree::GetType (m4pchar_t ao_szType, m4uint32_t ai_iLenBuffer)
{
  //## begin ClM4ConfigTree::GetType%913910713.body preserve=yes
	PropertyName::iterator			it;
	m4return_t						iRet;
	m4uint32_t						isize ;
	
	m_oSynchro.Lock () ;
	iRet =m_oProperties.GetProperty ( "TYPE", ao_szType , ai_iLenBuffer ) ;

	if ( iRet == M4_ERROR )
	{
		isize = m_szClass.size () ;

		if (( ai_iLenBuffer >= isize ) && (isize > 1) )
		{
			it = m_szClass.begin();
			memcpy ( ao_szType , &(*it), isize ) ;

			iRet = M4_SUCCESS ;
		}
	}

	m_oSynchro.Unlock () ;
	return iRet ;
  //## end ClM4ConfigTree::GetType%913910713.body
}

int ClM4ConfigTree::Init (const char *ai_szFile, size_t ai_iReadSize, size_t ai_iWriteSize)
{
  //## begin ClM4ConfigTree::Init%916218711.body preserve=yes
	return M4_ERROR ;
  //## end ClM4ConfigTree::Init%916218711.body
}

m4return_t ClM4ConfigTree::GetFirstProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &ao_iHndProp, void * & ao_iHndMatch)
{
  //## begin ClM4ConfigTree::GetFirstProp%917598967.body preserve=yes
	m4return_t iRet;

//Hay que buscar una mejor solución para el bloqueo de estas dos funciones
//Al cambiar esto hay que buscar y quitar la sincronización en las funciones que llaman a estas	
//	m_oSynchro.Lock () ;

	iRet = m_oProperties.MatchFirstProp(ai_szMatchProp, ao_szProperty, ai_iTamMaxProp , ao_szValue, ai_iTamMaxValue , ao_iHndProp , (void*&)ao_iHndMatch);
	 

	if (M4_SUCCESS == iRet )
	{
		ao_iTamProp = strlen ( ao_szProperty ) ;

		ao_iTamValu = strlen ( ao_szValue ) ;
	}

//Hay que buscar una mejor solución para el bloqueo de estas dos funciones
//Al cambiar esto hay que buscar y quitar la sincronización en las funciones que llaman a estas	
//	m_oSynchro.Unlock () ;

	return iRet ;
  //## end ClM4ConfigTree::GetFirstProp%917598967.body
}

m4return_t ClM4ConfigTree::GetNextProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &aio_iHndProp, void * & aio_iHndMatch)
{
  //## begin ClM4ConfigTree::GetNextProp%917598970.body preserve=yes
	m4return_t iRet;
	
//Hay que buscar una mejor solución para el bloqueo de estas dos funciones
//Al cambiar esto hay que buscar y quitar la sincronización en las funciones que llaman a estas	
//	m_oSynchro.Lock () ;
	
	iRet = m_oProperties.MatchNextProp(ai_szMatchProp, ao_szProperty, ai_iTamMaxProp , ao_szValue, ai_iTamMaxValue , aio_iHndProp , (void*&)aio_iHndMatch);


	if (M4_SUCCESS == iRet )
	{
		ao_iTamProp = strlen ( ao_szProperty ) ;

		ao_iTamValu = strlen ( ao_szValue );
	}

//Hay que buscar una mejor solución para el bloqueo de estas dos funciones
//Al cambiar esto hay que buscar y quitar la sincronización en las funciones que llaman a estas	
//	m_oSynchro.Unlock () ;
	return iRet ;
  //## end ClM4ConfigTree::GetNextProp%917598970.body
}

void ClM4ConfigTree::SetClassHinst (m4pchar_t ai_szClass, m4pchar_t ai_szHist)
{
  //## begin ClM4ConfigTree::SetClassHinst%918583520.body preserve=yes
	m4uint32_t iLenProp = 0;
	m4uint32_t i ;
	
	iLenProp =  strlen ( ai_szClass ) +1;

	m_szClass.resize ( iLenProp ) ; 

	//copy the property name to a vector
	for ( i = 0 ; i < iLenProp ; i ++ ) 
	{
		m_szClass [i] = ai_szClass [i] ;
	}

	iLenProp =  strlen ( ai_szHist ) +1;


	m_szHinst.resize ( iLenProp  ) ;

	//copy the property name to a vector
	for ( i = 0 ; i < iLenProp ; i ++ ) 
	{
		m_szHinst [i] = ai_szHist [i] ;
	}

  //## end ClM4ConfigTree::SetClassHinst%918583520.body
}

int ClM4ConfigTree::GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClM4ConfigTree::GetInstance%942315721.body preserve=yes
	PropertyName::iterator			it;
	m4uint_t						len ;

	len = m_szHinst.size();

	if ( ( 0 == len ) || (ai_iStringSize <= len ) )
		return M4_ERROR ;

	it = m_szHinst.begin();
	memcpy ( ao_pszString , &(*it), len ) ;

	ao_riOutput = len -1 ;

	return M4_SUCCESS ;
  //## end ClM4ConfigTree::GetInstance%942315721.body
}

int ClM4ConfigTree::GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClM4ConfigTree::GetClass%959065914.body preserve=yes
	PropertyName::iterator			it;
	m4uint_t						len ;

	len = m_szClass.size();

	if ( ( 0 == len ) || (ai_iStringSize <= len ) )
		return M4_ERROR ;

	it = m_szClass.begin();
	memcpy ( ao_pszString , &(*it), len ) ;

	ao_riOutput = len -1 ;

	return M4_SUCCESS ;
  //## end ClM4ConfigTree::GetClass%959065914.body
}

// Additional Declarations
  //## begin ClM4ConfigTree%3678E58F01E6.declarations preserve=yes
  //## end ClM4ConfigTree%3678E58F01E6.declarations

//## begin module%36792AA6022E.epilog preserve=yes
//## end module%36792AA6022E.epilog
