//## begin module%3507C1D40395.cm preserve=no
//## end module%3507C1D40395.cm

//## begin module%3507C1D40395.cp preserve=no
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
//## end module%3507C1D40395.cp

//## Module: cllist%3507C1D40395; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\cllist.cpp

//## begin module%3507C1D40395.additionalIncludes preserve=no
//## end module%3507C1D40395.additionalIncludes

//## begin module%3507C1D40395.includes preserve=yes
#include "m4stl.hpp"
#include <m4srvres.hpp>
#include <m4log.hpp>
#include <clbaseobject.hpp>

#include "tracerequest.hpp"
//## end module%3507C1D40395.includes

// syncronization
#include <m4syncronization.hpp>
// clarraylibres
#include <clarraylibres.hpp>
// cllist
#include <cllist.hpp>

class ClLocalMonitor;

//## begin module%3507C1D40395.declarations preserve=no
//## end module%3507C1D40395.declarations

//## begin module%3507C1D40395.additionalDeclarations preserve=yes
//## end module%3507C1D40395.additionalDeclarations


// Class ClList 














ClList::ClList (ClActiveList *ai_poActiveList)
  //## begin ClList::ClList%880628514.hasinit preserve=no
  //## end ClList::ClList%880628514.hasinit
  //## begin ClList::ClList%880628514.initialization preserve=yes
  :m_poList ( ai_poActiveList )
  //## end ClList::ClList%880628514.initialization
{
  //## begin ClList::ClList%880628514.body preserve=yes
  //## end ClList::ClList%880628514.body
}


ClList::~ClList ()
{
  //## begin ClList::~ClList%880628515.body preserve=yes
	M4Thread * poItem ;
	ActiveIterator Iterator ;


	EnterWriter ( ) ;

	while ( ( Iterator =  m_oActiveMap.begin () )   !=   m_oActiveMap.end ()  )
	{

		poItem = (*Iterator).first ;

		poItem -> Terminate ( ) ;

//		delete ( poItem );

		poItem = NULL ;

		m_oActiveMap.erase ( Iterator ) ;
	}

	LeaveWriter () ;
  //## end ClList::~ClList%880628515.body
}



//## Other Operations (implementation)
m4return_t ClList::AddItem (M4Thread *ai_item)
{
  //## begin ClList::AddItem%875610307.body preserve=yes
	m4uint64_t lIdItem ;
	
	
	
	if (! ai_item )
	{
		SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "Trying to insert a null object in the list of threads" );

		return M4_ERROR ;
	}

	EnterWriter();

	lIdItem = ClIdGenerator::GetNewId ( ) ; 

	m_oActiveMap.insert ( ActiveMap::value_type( ai_item , lIdItem ) );

	LeaveWriter ( ) ;

	return M4_SUCCESS;

  //## end ClList::AddItem%875610307.body
}

m4return_t ClList::RemoveItem (M4Thread *ai_pItem)
{
  //## begin ClList::RemoveItem%875610308.body preserve=yes
	ActiveIterator Iterator ;


	EnterWriter ( ) ;

	Iterator = m_oActiveMap.find ( ai_pItem ) ;

	if ( Iterator == m_oActiveMap.end ( ) )
	{
		SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "Trying to remove a thread that is not in the list of threads" );

		LeaveWriter ( ) ;

		return M4_ERROR ;
	}

	m_oActiveMap.erase ( Iterator ) ;

	LeaveWriter ( ) ;

	return M4_SUCCESS;
  //## end ClList::RemoveItem%875610308.body
}

M4Thread * ClList::GetItem ()
{
  //## begin ClList::GetItem%877008212.body preserve=yes
	M4Thread* poItem;
	ActiveIterator Iterator ;

	
	EnterReader ( ) ;

	Iterator = m_oActiveMap.begin ( ) ;

	if ( Iterator == m_oActiveMap.end ( ) ) 
	{
		SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "Trying to get a thread from an empty list of threads" );

		LeaveReader () ;

		return NULL ;
	}

	poItem = ( * Iterator).first ;

	LeaveReader () ;

	return poItem ;
  //## end ClList::GetItem%877008212.body
}

m4return_t ClList::ShutDown ()
{
  //## begin ClList::ShutDown%877943017.body preserve=yes
	M4Thread * poItem ;
	ActiveIterator Iterator ;

	
	EnterWriter ( ) ;

	//while ( ( poThread = (M4Thread * ) (* begin () ).second )  != ( * end () ).second )
	while ( ( Iterator =  m_oActiveMap.begin () )   !=   m_oActiveMap.end ()  )
	{

		poItem = ( * Iterator ).first ;

		poItem -> Terminate () ;

		m_poList -> AddItem (  poItem ) ;

		m_oActiveMap.erase ( Iterator ) ;
	}

	LeaveWriter () ;

	return M4_SUCCESS ;
  //## end ClList::ShutDown%877943017.body
}

m4return_t ClList::ShutDownItem (M4Thread *ai_pItem)
{
  //## begin ClList::ShutDownItem%878114669.body preserve=yes
	ActiveIterator Iterator ;

	
	EnterWriter ( ) ;


	Iterator = m_oActiveMap.find ( ai_pItem ) ;
	
	if ( Iterator == m_oActiveMap.end ( ) )
	{
		LeaveWriter ( ) ;

		return M4_ERROR; 
	}

	ai_pItem -> Terminate ();

	m_poList -> AddItem (  ai_pItem ) ;

	m_oActiveMap.erase ( Iterator ) ;

	LeaveWriter ( ) ;

	return M4_SUCCESS ;
  //## end ClList::ShutDownItem%878114669.body
}

m4return_t ClList::AddItem (m4objid_t ai_lIdItem, M4Thread *ai_item)
{
  //## begin ClList::AddItem%880449708.body preserve=yes
	if (! ai_item )
	{
		SETCODEF ( M4_SRV_LM_NULL_ARGUMENT , ERRORLOG , "Trying to insert a null object in the list of threads" );

		return M4_ERROR ;
	}

	EnterWriter();

	m_oActiveMap.insert ( ActiveMap::value_type( ai_item  , ai_lIdItem ) );

	LeaveWriter ( ) ;

	return M4_SUCCESS;
  //## end ClList::AddItem%880449708.body
}

void ClList::SetActiveList (ClActiveList  *ai_poActiveList)
{
  //## begin ClList::SetActiveList%880628516.body preserve=yes
	m_poList = ai_poActiveList ;
  //## end ClList::SetActiveList%880628516.body
}

m4return_t ClList::StartUp ()
{
  //## begin ClList::StartUp%894962382.body preserve=yes
	M4Thread * poItem ;
	ActiveIterator Iterator ;

	
	EnterWriter ( ) ;

	//while ( ( poThread = (M4Thread * ) (* begin () ).second )  != ( * end () ).second )
	for (  Iterator =  m_oActiveMap.begin () ; Iterator  !=   m_oActiveMap.end () ; Iterator ++ )
	{

		poItem = ( * Iterator ).first ;

		poItem -> Start () ;

	}

	LeaveWriter () ;

	return M4_SUCCESS ;
  //## end ClList::StartUp%894962382.body
}

// Additional Declarations
  //## begin ClList%3430BFB102DB.declarations preserve=yes
  //## end ClList%3430BFB102DB.declarations

// Class ClActiveList 



ClActiveList::ClActiveList (ClLocalMonitor *ai_poLocalMonitor)
  //## begin ClActiveList::ClActiveList%879781126.hasinit preserve=no
  //## end ClActiveList::ClActiveList%879781126.hasinit
  //## begin ClActiveList::ClActiveList%879781126.initialization preserve=yes
  :ClList ( this )
  //## end ClActiveList::ClActiveList%879781126.initialization
{
  //## begin ClActiveList::ClActiveList%879781126.body preserve=yes
  //## end ClActiveList::ClActiveList%879781126.body
}

ClActiveList::ClActiveList ()
  //## begin ClActiveList::ClActiveList%880628524.hasinit preserve=no
  //## end ClActiveList::ClActiveList%880628524.hasinit
  //## begin ClActiveList::ClActiveList%880628524.initialization preserve=yes
  //## end ClActiveList::ClActiveList%880628524.initialization
{
  //## begin ClActiveList::ClActiveList%880628524.body preserve=yes
  //## end ClActiveList::ClActiveList%880628524.body
}



//## Other Operations (implementation)
m4return_t ClActiveList::ShutDown ()
{
  //## begin ClActiveList::ShutDown%879431912.body preserve=yes
 	M4Thread * poThread ;
	ActiveIterator Iterator ;

	
	EnterWriter ( ) ;

	while ( ( Iterator = m_oActiveMap.begin () )  !=  m_oActiveMap.end ()  )
	{
		poThread = ( * Iterator ).first ;
		

//		delete poThread ;

		m_oActiveMap.erase ( Iterator ) ;
	}

	LeaveWriter () ;

	return M4_SUCCESS ;


  //## end ClActiveList::ShutDown%879431912.body
}

m4return_t ClActiveList::ShutDownItem (M4Thread *ai_pItem)
{
  //## begin ClActiveList::ShutDownItem%881748018.body preserve=yes
	ActiveIterator Iterator ;

	EnterWriter ( ) ;

	Iterator = m_oActiveMap.find ( ai_pItem ) ;


	 if ( Iterator == m_oActiveMap.end( ) )
	 {
		LeaveWriter ( ) ;

		return M4_ERROR ;
	 }


	ai_pItem -> Terminate ();

	LeaveWriter ( ) ;

	return M4_SUCCESS;
  //## end ClActiveList::ShutDownItem%881748018.body
}

void ClActiveList::Synchronize ()
{
  //## begin ClActiveList::Synchronize%887880514.body preserve=yes
	M4Thread * poItem ;
	ActiveIterator Iterator ;
	ActiveIterator IterAux ;


    ClTraceRequest tRequest("ActiveList.Synchronize");
    m4uint32_t iItems = 0;

    tRequest.StartCount();
	EnterWriter ( ) ;

	Iterator =  IterAux = m_oActiveMap.begin () ;

	while (  Iterator  !=   m_oActiveMap.end ()  )
	{

		IterAux ++ ;
        iItems++;

		poItem = ( * Iterator ).first ;

		if ( poItem -> IsExit ( ) )
		{
			delete poItem ;

			m_oActiveMap.erase ( Iterator ) ;
		}

		Iterator = IterAux ;

	}

	LeaveWriter () ;

    tRequest.StopCount();

    if (iItems > 0)
    {
        char msg[256];
        sprintf (msg, "\tItems = %lu", iItems);
        tRequest.SetMsg(msg);
        tRequest.Trace();
    }

  //## end ClActiveList::Synchronize%887880514.body
}

// Additional Declarations
  //## begin ClActiveList%346AFED603DE.declarations preserve=yes
  //## end ClActiveList%346AFED603DE.declarations

//## begin module%3507C1D40395.epilog preserve=yes
//## end module%3507C1D40395.epilog
