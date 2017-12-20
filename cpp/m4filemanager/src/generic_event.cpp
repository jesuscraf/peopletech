//## begin module%374E96100131.cm preserve=no
//## end module%374E96100131.cm

//## begin module%374E96100131.cp preserve=no
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
//## end module%374E96100131.cp

//## Module: generic_event%374E96100131; Package body
//## Subsystem: M4FileManager::src%374E96100087
//	d:\fuentesservidor\m4filemanager\version\src
//## Source file: d:\evol\m4filemanager\version\src\generic_event.cpp

//## begin module%374E96100131.additionalIncludes preserve=no
//## end module%374E96100131.additionalIncludes

//## begin module%374E96100131.includes preserve=yes
#include <syncro.hpp>
//## end module%374E96100131.includes

// m4types
#include <m4types.hpp>
// generic_event
#include <generic_event.hpp>
//## begin module%374E96100131.declarations preserve=no
//## end module%374E96100131.declarations

//## begin module%374E96100131.additionalDeclarations preserve=yes
ClMutex oMutextEventBroker(M4_TRUE);
//## end module%374E96100131.additionalDeclarations


// Class ClEventDescription 



ClEventDescription::ClEventDescription (ClEventType ai_oType, void* ai_pDetails)
  //## begin ClEventDescription::ClEventDescription%-2107733181.hasinit preserve=no
  //## end ClEventDescription::ClEventDescription%-2107733181.hasinit
  //## begin ClEventDescription::ClEventDescription%-2107733181.initialization preserve=yes
  //## end ClEventDescription::ClEventDescription%-2107733181.initialization
{
  //## begin ClEventDescription::ClEventDescription%-2107733181.body preserve=yes
	SetType(ai_oType);
	SetDetails(ai_pDetails);
  //## end ClEventDescription::ClEventDescription%-2107733181.body
}


ClEventDescription::~ClEventDescription ()
{
  //## begin ClEventDescription::~ClEventDescription%1169785350.body preserve=yes
  //## end ClEventDescription::~ClEventDescription%1169785350.body
}



//## Other Operations (implementation)
ClEventType ClEventDescription::GetType ()
{
  //## begin ClEventDescription::GetType%-274530691.body preserve=yes
	return m_oType;
  //## end ClEventDescription::GetType%-274530691.body
}

void ClEventDescription::SetType (ClEventType ai_oNewType)
{
  //## begin ClEventDescription::SetType%1411540414.body preserve=yes
	m_oType = ai_oNewType;
  //## end ClEventDescription::SetType%1411540414.body
}

void* ClEventDescription::GetDetails () const
{
  //## begin ClEventDescription::GetDetails%404087045.body preserve=yes
	return m_pDetails;
  //## end ClEventDescription::GetDetails%404087045.body
}

void ClEventDescription::SetDetails (void* ai_pNewDetails)
{
  //## begin ClEventDescription::SetDetails%2057788636.body preserve=yes
	m_pDetails = ai_pNewDetails;
  //## end ClEventDescription::SetDetails%2057788636.body
}

// Additional Declarations
  //## begin ClEventDescription%374E960F03F8.declarations preserve=yes
  //## end ClEventDescription%374E960F03F8.declarations

// Class ClEventBroker 

//## begin ClEventBroker::m_poInstance%374E96100069.attr preserve=no  private: static ClEventBroker* {VA} NULL
ClEventBroker* ClEventBroker::m_poInstance = NULL;
//## end ClEventBroker::m_poInstance%374E96100069.attr

ClEventBroker::ClEventBroker ()
  //## begin ClEventBroker::ClEventBroker%-247254367.hasinit preserve=no
  //## end ClEventBroker::ClEventBroker%-247254367.hasinit
  //## begin ClEventBroker::ClEventBroker%-247254367.initialization preserve=yes
  //## end ClEventBroker::ClEventBroker%-247254367.initialization
{
  //## begin ClEventBroker::ClEventBroker%-247254367.body preserve=yes
  //## end ClEventBroker::ClEventBroker%-247254367.body
}


ClEventBroker::~ClEventBroker ()
{
  //## begin ClEventBroker::~ClEventBroker%109429335.body preserve=yes
  //## end ClEventBroker::~ClEventBroker%109429335.body
}



//## Other Operations (implementation)
ClEventBroker* ClEventBroker::Instance ()
{
  //## begin ClEventBroker::Instance%625172185.body preserve=yes
	// Check if instance doesn't exist (it must be unique).
	if (m_poInstance == NULL) {
		// Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutextEventBroker.Lock();
		if (m_poInstance == NULL) {
			//Si no existia, se crea
			if ( (m_poInstance=new ClEventBroker) == NULL)
				m_poInstance = NULL; // Not enough memory
		}
		oMutextEventBroker.Unlock();
	}
	return m_poInstance;
  //## end ClEventBroker::Instance%625172185.body
}

m4return_t ClEventBroker::SetInterestInEventType (ClEventType ai_oEventType, ClEventsClientId* ai_oClientId)
{
  //## begin ClEventBroker::SetInterestInEventType%9126463.body preserve=yes
	return M4_ERROR;
  //## end ClEventBroker::SetInterestInEventType%9126463.body
}

m4return_t ClEventBroker::UnSetInterestInEventType (ClEventType ai_oEventType, ClEventsClientId* ai_oClientId)
{
  //## begin ClEventBroker::UnSetInterestInEventType%999267762.body preserve=yes
	return M4_ERROR;
  //## end ClEventBroker::UnSetInterestInEventType%999267762.body
}

m4return_t ClEventBroker::SetInterestInEvent (ClEventDescription ai_oEventType, ClEventsClientId* ai_oClientId)
{
  //## begin ClEventBroker::SetInterestInEvent%-2032400336.body preserve=yes
	return M4_ERROR;
  //## end ClEventBroker::SetInterestInEvent%-2032400336.body
}

m4return_t ClEventBroker::UnSetInterestInEvent (ClEventDescription ai_oEventType, ClEventsClientId* ai_oClientId)
{
  //## begin ClEventBroker::UnSetInterestInEvent%-1530773485.body preserve=yes
	return M4_ERROR;
  //## end ClEventBroker::UnSetInterestInEvent%-1530773485.body
}

// Additional Declarations
  //## begin ClEventBroker%374E9610004C.declarations preserve=yes
  //## end ClEventBroker%374E9610004C.declarations

//## begin module%374E96100131.epilog preserve=yes
//## end module%374E96100131.epilog
