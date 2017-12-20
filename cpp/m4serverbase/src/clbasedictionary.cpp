//## begin module%342240660362.cm preserve=no
//## end module%342240660362.cm

//## begin module%342240660362.cp preserve=no
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
//## end module%342240660362.cp

//## Module: clbasedictionary%342240660362; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\clbasedictionary.cpp

//## begin module%342240660362.additionalIncludes preserve=no
//## end module%342240660362.additionalIncludes

//## begin module%342240660362.includes preserve=yes
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <clarraylibres.hpp>

#include <m4srvtrace.h>
#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%342240660362.includes

// syncro
#include <syncro.hpp>
// clbasedictionary
#include <clbasedictionary.hpp>
// clbaseobject
#include <clbaseobject.hpp>


//## begin module%342240660362.declarations preserve=no
//## end module%342240660362.declarations

//## begin module%342240660362.additionalDeclarations preserve=yes
//## end module%342240660362.additionalDeclarations


// Class ClBaseDictionary 




ClBaseDictionary::ClBaseDictionary ()
  //## begin ClBaseDictionary::ClBaseDictionary%-2015032989.hasinit preserve=no
  //## end ClBaseDictionary::ClBaseDictionary%-2015032989.hasinit
  //## begin ClBaseDictionary::ClBaseDictionary%-2015032989.initialization preserve=yes
  //## end ClBaseDictionary::ClBaseDictionary%-2015032989.initialization
{
  //## begin ClBaseDictionary::ClBaseDictionary%-2015032989.body preserve=yes
		m_iNumItems = 0;
		m_oMutex.Init () ;
  //## end ClBaseDictionary::ClBaseDictionary%-2015032989.body
}



//## Other Operations (implementation)

m4int64_t ClBaseDictionary::AddItem (ClBaseObject* ai_poBaseObject)
{
  //## begin ClBaseDictionary::AddItem%-769986080.body preserve=yes
		m4int32_t Id;

		
		Lock ();

		Id = ClIdGenerator::GetNewId ( ) ; 

		

	if ( m_mapDictionary.find ( Id ) != m_mapDictionary.end() )
	{
	  //		M4_SRV_ERROR( 1005 , "Trying to insert an item with the Id %0:s and there is one element in the dictionary with this Id " , Id ) ;

		Unlock ( ) ;

		return M4_ERROR;
	}


		m_mapDictionary.insert ( ClDictionary::value_type ( Id , ai_poBaseObject ) ) ;

		ai_poBaseObject -> SetId ( Id ) ;

		m_iNumItems ++;

		Unlock () ;
			
		return Id;
	
  //## end ClBaseDictionary::AddItem%-769986080.body
}

m4return_t ClBaseDictionary::AddItem (m4int64_t ai_Id, ClBaseObject* ai_poBaseObject)
{
  //## begin ClBaseDictionary::AddItem%-179606071.body preserve=yes

	Lock () ;


	if ( m_mapDictionary.find ( ai_Id ) != m_mapDictionary.end() )
	{
//	  	M4_SRV_ERROR( M4_SRV_LM_DICTIONARY_ERROR , "Trying to insert an item with the Id %0:s and there is one element in the dictionary with this Id " , (m4int32_t)ai_Id ) ;

		Unlock () ;

		return M4_ERROR;
	}



	m_mapDictionary.insert ( ClDictionary::value_type ( ai_Id , ai_poBaseObject ) ) ;

	m_iNumItems ++;

	Unlock () ;

	return M4_SUCCESS;
  //## end ClBaseDictionary::AddItem%-179606071.body
}

m4return_t ClBaseDictionary::RemoveItem (m4int64_t ai_Id)
{
  //## begin ClBaseDictionary::RemoveItem%1696311704.body preserve=yes
		IterDictionary Iterator ;
		
	
		Lock () ;

		Iterator = m_mapDictionary.find ( ai_Id ) ;


		if  ( Iterator == m_mapDictionary.end() )
		{
//	  		M4_SRV_ERROR( M4_SRV_LM_DICTIONARY_ERROR , "Trying to remove an item with the Id %0:s and there is no one element in the dictionary with this Id " , (m4int32_t)ai_Id ) ;
 
			Unlock();

			return M4_ERROR ;
		}

		m_mapDictionary.erase ( Iterator ) ;

		m_iNumItems -- ;

		Unlock ();

	return M4_SUCCESS;
	
  //## end ClBaseDictionary::RemoveItem%1696311704.body
}

m4return_t ClBaseDictionary::DownItem (m4int64_t ai_Id)
{
  //## begin ClBaseDictionary::DownItem%-1498754811.body preserve=yes
		ClBaseObject * poObject ;


		poObject = GetItem ( ai_Id ) ;


		if  ( ! poObject )
		{
			return M4_ERROR ;
		}

		return RemoveItem ( ai_Id ) ;
	
  //## end ClBaseDictionary::DownItem%-1498754811.body
}

ClBaseObject* ClBaseDictionary::GetItem (m4int64_t ai_Id)
{
  //## begin ClBaseDictionary::GetItem%1028276320.body preserve=yes
		IterDictionary	 Iterator;
		ClBaseObject * poObject ;

		Lock();
		
		Iterator = m_mapDictionary.find ( ai_Id ) ;


	if ( (Iterator) == m_mapDictionary.end() )
	{
//	  	M4_SRV_ERROR( M4_SRV_LM_DICTIONARY_ERROR , "Trying to get an item with the Id %0:s and there is one element in the dictionary with this Id " , (m4int32_t)ai_Id ) ;

		Unlock ( ) ;

		return NULL;
	}


	poObject = ( * Iterator).second ;

	Unlock( );

	return poObject ;
  //## end ClBaseDictionary::GetItem%1028276320.body
}

m4uint64_t ClBaseDictionary::GetNumItems ()
{
  //## begin ClBaseDictionary::GetNumItems%-1250346923.body preserve=yes
	m4uint32_t iNumItems;

	Lock();

		iNumItems = m_iNumItems ;

		Unlock() ;

		return iNumItems ;
	
  //## end ClBaseDictionary::GetNumItems%-1250346923.body
}

void ClBaseDictionary::Lock ()
{
  //## begin ClBaseDictionary::Lock%893077207.body preserve=yes
	m_oMutex.Lock ( ) ;
  //## end ClBaseDictionary::Lock%893077207.body
}

void ClBaseDictionary::Unlock ()
{
  //## begin ClBaseDictionary::Unlock%893077208.body preserve=yes
	m_oMutex.Unlock ( ) ;
  //## end ClBaseDictionary::Unlock%893077208.body
}

m4return_t ClBaseDictionary::GetObjectList (IdList &ai_oObjList)
{
  //## begin ClBaseDictionary::GetObjectList%977242950.body preserve=yes
		IterDictionary	Iterator;

	Lock();

	Iterator = m_mapDictionary.begin () ;

	while ( Iterator != m_mapDictionary.end() )
	{
		ai_oObjList.push_back ( (*Iterator).first ) ;
		Iterator ++ ;
	}


	Unlock( );

	return M4_SUCCESS ;
  //## end ClBaseDictionary::GetObjectList%977242950.body
}

// Additional Declarations
  //## begin ClBaseDictionary%34223FFE0258.declarations preserve=yes
  //## end ClBaseDictionary%34223FFE0258.declarations

//## begin module%342240660362.epilog preserve=yes
//## end module%342240660362.epilog
