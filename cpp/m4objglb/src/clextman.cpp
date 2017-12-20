//## begin module%3912A6BD02B0.cm preserve=no
//## end module%3912A6BD02B0.cm

//## begin module%3912A6BD02B0.cp preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             clextman.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
//## end module%3912A6BD02B0.cp

//## Module: clextman%3912A6BD02B0; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\clextman.cpp

//## begin module%3912A6BD02B0.additionalIncludes preserve=no
//## end module%3912A6BD02B0.additionalIncludes

//## begin module%3912A6BD02B0.includes preserve=yes

#include "clextman.hpp"
#include "blocksynchronization.hpp"
//#include "syncro.hpp"
//## end module%3912A6BD02B0.includes

// clextobj
#include <clextobj.hpp>
#include "m4unicode.hpp"
//## begin module%3912A6BD02B0.declarations preserve=no
//## end module%3912A6BD02B0.declarations

//## begin module%3912A6BD02B0.additionalDeclarations preserve=yes
long	M4ClExternObjectManager::m_lfCallBack = 0 ;

static ClMutex soMutexDll(M4_TRUE);
	
//## end module%3912A6BD02B0.additionalDeclarations


// Class M4ClExternObjectManager 

//## begin M4ClExternObjectManager::m_ExtObjMap%3912A6BA018F.role preserve=no  private: static M4ClExternObjectMap {1 -> 1VHAPN}
M4ClExternObjectMap M4ClExternObjectManager::m_ExtObjMap;
//## end M4ClExternObjectManager::m_ExtObjMap%3912A6BA018F.role

M4ClExternObjectManager::M4ClExternObjectManager (void )
  //## begin M4ClExternObjectManager::M4ClExternObjectManager%1757527291.hasinit preserve=no
  //## end M4ClExternObjectManager::M4ClExternObjectManager%1757527291.hasinit
  //## begin M4ClExternObjectManager::M4ClExternObjectManager%1757527291.initialization preserve=yes
  //## end M4ClExternObjectManager::M4ClExternObjectManager%1757527291.initialization
{
  //## begin M4ClExternObjectManager::M4ClExternObjectManager%1757527291.body preserve=yes
  //## end M4ClExternObjectManager::M4ClExternObjectManager%1757527291.body
}


M4ClExternObjectManager::~M4ClExternObjectManager ()
{
  //## begin M4ClExternObjectManager::~M4ClExternObjectManager%-526924627.body preserve=yes
  //## end M4ClExternObjectManager::~M4ClExternObjectManager%-526924627.body
}



//## Other Operations (implementation)
m4return_t M4ClExternObjectManager::LoadObject (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType)
{
  //## begin M4ClExternObjectManager::LoadObject%-355168904.body preserve=yes
	ClMutBlock oBlock(soMutexDll);

	m4return_t result;

	itM4ClExternObjectMap it = m_ExtObjMap.find (ai_pccObjectName);

	if (it == m_ExtObjMap.end ())
	{
		// Por si estamos en rich client y hay que cargar la librería
		// No se comprueba el retorno y se sigue adelante
		result = RunCallBack( ai_pccObjectName ) ;

		//Lo creamos
		M4ClExternObjectMapEntry * pExtObjMapEntry = new M4ClExternObjectMapEntry ( );

		result = pExtObjMapEntry->Init ( ai_pccObjectName, ai_iObjectType );

		if (result == M4_ERROR)
		{
			return M4_ERROR;	
		}
		m_ExtObjMap.insert ( M4ClExternObjectMap::value_type ( ai_pccObjectName, pExtObjMapEntry ) );

	}
	else
	{
		//Aumentamos el numero de enlaces.
		((*it).second) -> AddLink ();
	}

	return M4_SUCCESS;

  //## end M4ClExternObjectManager::LoadObject%-355168904.body
}

M4ClExternObject* M4ClExternObjectManager::GetObject (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType)
{
  //## begin M4ClExternObjectManager::GetObject%1000563077.body preserve=yes
	ClMutBlock oBlock(soMutexDll);

	m4return_t result;

	itM4ClExternObjectMap it = m_ExtObjMap.find (ai_pccObjectName);

	if (it == m_ExtObjMap.end ())
	{
		// Por si estamos en rich client y hay que cargar la librería
		// No se comprueba el retorno y se sigue adelante
		result = RunCallBack( ai_pccObjectName ) ;

		M4ClExternObjectMapEntry * pExtObjMapEntry = new M4ClExternObjectMapEntry ( );

		result = pExtObjMapEntry->Init ( ai_pccObjectName, ai_iObjectType );

		if (result == M4_ERROR)
		{
			return NULL;	
		}
		m_ExtObjMap.insert ( M4ClExternObjectMap::value_type ( ai_pccObjectName, pExtObjMapEntry ) );

		//Devolvemos el Objeto
		return pExtObjMapEntry -> GetExtObj ();

	}
	else
	{
		//devolvemos el Objeto
		return((*it).second) -> GetExtObj ();
	}

  //## end M4ClExternObjectManager::GetObject%1000563077.body
}

m4return_t M4ClExternObjectManager::FreeObject (m4pcchar_t ai_pccObjectName)
{
  //## begin M4ClExternObjectManager::FreeObject%932442653.body preserve=yes
	ClMutBlock oBlock(soMutexDll);

	itM4ClExternObjectMap it = m_ExtObjMap.find (ai_pccObjectName);

	if (it == m_ExtObjMap.end ())
	{
		return M4_SUCCESS;  //o warning

	}
	else
	{
		if ( ( ( (*it).second ) -> SubstractLink () ) == 0)
		{
			delete ((*it).second);
			m_ExtObjMap.erase (it);
		}
		return M4_SUCCESS;
	}

  //## end M4ClExternObjectManager::FreeObject%932442653.body
}

m4return_t M4ClExternObjectManager::FreeObject (M4ClExternObject* ai_poObject)
{
  //## begin M4ClExternObjectManager::FreeObject%-1687646626.body preserve=yes
	ClMutBlock oBlock(soMutexDll);

	itM4ClExternObjectMap it;
	M4ClExternObject * pExtObj;
	
	for ( it = m_ExtObjMap.begin (); it != m_ExtObjMap.end(); it++ )
	{
		pExtObj = ( ( (*it).second)->GetExtObj () );
		if ( pExtObj == ai_poObject )
		{
			if ( ( ( (*it).second ) -> SubstractLink () ) == 0)
			{
				delete ((*it).second);
				m_ExtObjMap.erase (it);
				return M4_SUCCESS;
			}
		}
	}

	return M4_SUCCESS; //o warning
  //## end M4ClExternObjectManager::FreeObject%-1687646626.body
}

void	M4ClExternObjectManager::SetCallBack( long ai_lfCallBack )
{
	ClMutBlock oBlock(soMutexDll);
	m_lfCallBack = ai_lfCallBack ;
}


// En windows, si hay un callback hay que invocar a VB para que descarge la dll
m4return_t	M4ClExternObjectManager::RunCallBack( m4pcchar_t ai_pccObjectName )
{

	m4return_t	iResult = M4_SUCCESS ;


#ifdef _WINDOWS

	BSTR	psObjectName = NULL ;

	if( m_lfCallBack != NULL )
	{
		// UNICODE VB
		psObjectName = M4CppToVB( ai_pccObjectName ) ;
		iResult = (* (long (CALLBACK *)(BSTR)) m_lfCallBack )( psObjectName ) ;
		SysFreeString( psObjectName ) ;

		// Mensaje de error
	}
#endif

	return( iResult ) ;
}

// Additional Declarations
  //## begin M4ClExternObjectManager%3912A6B90156.declarations preserve=yes
  //## end M4ClExternObjectManager%3912A6B90156.declarations

//## begin module%3912A6BD02B0.epilog preserve=yes
//## end module%3912A6BD02B0.epilog
