//## begin module%37A5559D033C.cm preserve=no
//## end module%37A5559D033C.cm

//## begin module%37A5559D033C.cp preserve=no
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
//## end module%37A5559D033C.cp

//## Module: clssinterfaz%37A5559D033C; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: z:\m4subsystems\src\clssinterfaz.cpp

//## begin module%37A5559D033C.additionalIncludes preserve=no
//## end module%37A5559D033C.additionalIncludes

//## begin module%37A5559D033C.includes preserve=yes
#include <clssimplementacion.hpp>
//## end module%37A5559D033C.includes

// ClAdminTag
#include <cladmintag.hpp>
// NavigatorF
#include <navigatorf.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// Statistics
#include <statistics.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// serverevent
#include <serverevent.hpp>
// DataStorages
#include <datastorages.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>


//## begin module%37A5559D033C.declarations preserve=no
//## end module%37A5559D033C.declarations

//## begin module%37A5559D033C.additionalDeclarations preserve=yes
//## end module%37A5559D033C.additionalDeclarations


// Class ClSSInterfaz 




ClSSInterfaz::ClSSInterfaz (ClSSImplementation *ai_poImplementation)
  //## begin ClSSInterfaz::ClSSInterfaz%933665016.hasinit preserve=no
  //## end ClSSInterfaz::ClSSInterfaz%933665016.hasinit
  //## begin ClSSInterfaz::ClSSInterfaz%933665016.initialization preserve=yes
  //## end ClSSInterfaz::ClSSInterfaz%933665016.initialization
{
  //## begin ClSSInterfaz::ClSSInterfaz%933665016.body preserve=yes
   m_poImplementation = ai_poImplementation ;
  //## end ClSSInterfaz::ClSSInterfaz%933665016.body
}


ClSSInterfaz::~ClSSInterfaz ()
{
  //## begin ClSSInterfaz::~ClSSInterfaz%960286622.body preserve=yes
	if ( m_poImplementation )
	{
		if ( ! m_poImplementation ->HasOwner ( ) )
		{
			delete m_poImplementation ;

			if ( m_poConfiguration )
				delete m_poConfiguration ;
		}
	}

  //## end ClSSInterfaz::~ClSSInterfaz%960286622.body
}



//## Other Operations (implementation)
m4return_t ClSSInterfaz::ShutDown ()
{
  //## begin ClSSInterfaz::ShutDown%933578409.body preserve=yes
	return m_poImplementation -> ShutDown ( ) ;

  //## end ClSSInterfaz::ShutDown%933578409.body
}

m4return_t ClSSInterfaz::Synchronize ()
{
  //## begin ClSSInterfaz::Synchronize%933578410.body preserve=yes
	return m_poImplementation -> Synchronize () ;
  //## end ClSSInterfaz::Synchronize%933578410.body
}

m4return_t ClSSInterfaz::Initialize ()
{
  //## begin ClSSInterfaz::Initialize%933578411.body preserve=yes
	return m_poImplementation -> Initialize () ;
  //## end ClSSInterfaz::Initialize%933578411.body
}

void ClSSInterfaz::SetConfiguration (ClConfiguration *ai_poConfiguration)
{
  //## begin ClSSInterfaz::SetConfiguration%933578412.body preserve=yes
	m_poConfiguration = ai_poConfiguration ;
	m_poImplementation -> SetConfiguration (ai_poConfiguration) ;
  //## end ClSSInterfaz::SetConfiguration%933578412.body
}

void * ClSSInterfaz::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClSSInterfaz::GetElement%933578413.body preserve=yes
	return m_poImplementation -> GetElement (ai_szElement) ;
  //## end ClSSInterfaz::GetElement%933578413.body
}

m4return_t ClSSInterfaz::GetCommandsId (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList)
{
  //## begin ClSSInterfaz::GetCommandsId%933578414.body preserve=yes
	return m_poImplementation -> GetCommandsId (ai_szPath , aio_oCommandList) ;
  //## end ClSSInterfaz::GetCommandsId%933578414.body
}

m4return_t ClSSInterfaz::GetCommandParams (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClSSInterfaz::GetCommandParams%933578415.body preserve=yes
	return m_poImplementation -> GetCommandParams (ai_szPath , ai_cCommand, aio_oCommnadList) ;
  //## end ClSSInterfaz::GetCommandParams%933578415.body
}

m4return_t ClSSInterfaz::ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSSInterfaz::ExecuteCommand%933578416.body preserve=yes
	return m_poImplementation -> ExecuteCommand (ai_szPath , ai_cCommand, ai_oParamList, ao_oResultList) ;
  //## end ClSSInterfaz::ExecuteCommand%933578416.body
}

m4objid_t ClSSInterfaz::GetId ()
{
  //## begin ClSSInterfaz::GetId%933578417.body preserve=yes
	return m_poImplementation -> GetId () ;
  //## end ClSSInterfaz::GetId%933578417.body
}

void ClSSInterfaz::SetId (m4objid_t ai_lId, m4pchar_t ai_szFatherPath)
{
  //## begin ClSSInterfaz::SetId%933578418.body preserve=yes
	m_lId = ai_lId ;
	m_poImplementation -> SetId (ai_lId , ai_szFatherPath ) ;
  //## end ClSSInterfaz::SetId%933578418.body
}

void ClSSInterfaz::Activate ()
{
  //## begin ClSSInterfaz::Activate%933578419.body preserve=yes
	m_poImplementation -> Activate () ;
  //## end ClSSInterfaz::Activate%933578419.body
}

ClSSInterfaz* ClSSInterfaz::FindSubsystemByType (m4uint32_t ai_iType)
{
  //## begin ClSSInterfaz::FindSubsystemByType%933665004.body preserve=yes
	return m_poImplementation -> FindSubsystemByType (ai_iType) ;
  //## end ClSSInterfaz::FindSubsystemByType%933665004.body
}

ClSSInterfaz* ClSSInterfaz::FindSubsystemById (m4objid_t ai_lId)
{
  //## begin ClSSInterfaz::FindSubsystemById%933665005.body preserve=yes
	return m_poImplementation -> FindSubsystemById (ai_lId) ;
  //## end ClSSInterfaz::FindSubsystemById%933665005.body
}

ClSSInterfaz* ClSSInterfaz::FindSubsystemByName (m4pchar_t ai_szName)
{
  //## begin ClSSInterfaz::FindSubsystemByName%933665006.body preserve=yes
		return m_poImplementation -> FindSubsystemByName (ai_szName) ;
  //## end ClSSInterfaz::FindSubsystemByName%933665006.body
}

m4return_t ClSSInterfaz::Start ()
{
  //## begin ClSSInterfaz::Start%933665008.body preserve=yes
	return m_poImplementation -> Start () ;
  //## end ClSSInterfaz::Start%933665008.body
}

m4return_t ClSSInterfaz::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSSInterfaz::Stop%933665009.body preserve=yes
	return m_poImplementation -> Stop (ai_lDeferedTime) ;
  //## end ClSSInterfaz::Stop%933665009.body
}

m4bool_t ClSSInterfaz::IsReadyToClose ()
{
  //## begin ClSSInterfaz::IsReadyToClose%933665010.body preserve=yes
	return m_poImplementation -> IsReadyToClose () ;
  //## end ClSSInterfaz::IsReadyToClose%933665010.body
}

m4return_t ClSSInterfaz::Init (ClConfiguration *ai_poConfiguration)
{
  //## begin ClSSInterfaz::Init%933665015.body preserve=yes
	return m_poImplementation -> Init(ai_poConfiguration) ;
  //## end ClSSInterfaz::Init%933665015.body
}

m4pchar_t ClSSInterfaz::GetName ()
{
  //## begin ClSSInterfaz::GetName%933760131.body preserve=yes
	return m_poImplementation -> GetName () ;
  //## end ClSSInterfaz::GetName%933760131.body
}

void ClSSInterfaz::SetName (m4pchar_t ai_szName)
{
  //## begin ClSSInterfaz::SetName%933760132.body preserve=yes
	m_poImplementation -> SetName (ai_szName) ;
  //## end ClSSInterfaz::SetName%933760132.body
}

m4return_t ClSSInterfaz::GetSubsystemsId (m4pchar_t ai_szPath, vector <m4objid_t > &ao_poTags)
{
  //## begin ClSSInterfaz::GetSubsystemsId%936281637.body preserve=yes
	return m_poImplementation -> GetSubsystemsId (ai_szPath , ao_poTags) ;
  //## end ClSSInterfaz::GetSubsystemsId%936281637.body
}

ClSSInterfaz* ClSSInterfaz::FindSubsystem (m4objid_t ai_lId)
{
  //## begin ClSSInterfaz::FindSubsystem%936613835.body preserve=yes
	return m_poImplementation -> FindSubsystem (ai_lId);
  //## end ClSSInterfaz::FindSubsystem%936613835.body
}

ClSSInterfaz* ClSSInterfaz::GetSubsystem ()
{
  //## begin ClSSInterfaz::GetSubsystem%936613836.body preserve=yes
	return this;
  //## end ClSSInterfaz::GetSubsystem%936613836.body
}

m4return_t ClSSInterfaz::AddSubsystem (ClSSInterfaz *ai_poSubsystem)
{
  //## begin ClSSInterfaz::AddSubsystem%936613837.body preserve=yes
	return m_poImplementation -> AddSubsystem ( (ClSSInterfaz*) ai_poSubsystem);
  //## end ClSSInterfaz::AddSubsystem%936613837.body
}

m4return_t ClSSInterfaz::RemoveSubsystem (m4objid_t ai_lSubsystemId)
{
  //## begin ClSSInterfaz::RemoveSubsystem%936613838.body preserve=yes
	return m_poImplementation -> RemoveSubsystem (ai_lSubsystemId);
  //## end ClSSInterfaz::RemoveSubsystem%936613838.body
}

m4uint16_t ClSSInterfaz::GetType ()
{
  //## begin ClSSInterfaz::GetType%936613839.body preserve=yes
	return m_poImplementation -> GetType ();
  //## end ClSSInterfaz::GetType%936613839.body
}

void ClSSInterfaz::SetType (m4uint16_t ai_iType)
{
  //## begin ClSSInterfaz::SetType%936613842.body preserve=yes
	m_poImplementation -> SetType (ai_iType);
  //## end ClSSInterfaz::SetType%936613842.body
}

void ClSSInterfaz::AttachInterfaceImplementation (void *ai_poImlementation)
{
  //## begin ClSSInterfaz::AttachInterfaceImplementation%936688276.body preserve=yes
	m_poImplementation = (ClSSImplementation*)  ai_poImlementation ;
	((ClSSImplementation*)ai_poImlementation) -> SetInterfaz (this);
  //## end ClSSInterfaz::AttachInterfaceImplementation%936688276.body
}

ClStatistic * ClSSInterfaz::GetStats ()
{
  //## begin ClSSInterfaz::GetStats%937209551.body preserve=yes
	return m_poImplementation -> GetStats ()  ;
  //## end ClSSInterfaz::GetStats%937209551.body
}

void ClSSInterfaz::SetStats (ClStatistic *ai_poStats)
{
  //## begin ClSSInterfaz::SetStats%937209552.body preserve=yes
	m_poImplementation -> SetStats (ai_poStats ) ;
  //## end ClSSInterfaz::SetStats%937209552.body
}

m4return_t ClSSInterfaz::SetState (eStatus ai_iStatus, m4pchar_t ai_szState)
{
  //## begin ClSSInterfaz::SetState%937811412.body preserve=yes
	m_poImplementation -> SetState (  (ClSSImplementation::eStatus) ai_iStatus , ai_szState) ;
	 return M4_SUCCESS ;
  //## end ClSSInterfaz::SetState%937811412.body
}

void ClSSInterfaz::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClSSInterfaz::Update%942826988.body preserve=yes
	m_poImplementation -> Update(  ai_iEvent, ai_oSubject);
  //## end ClSSInterfaz::Update%942826988.body
}

void ClSSInterfaz::AttachObserver (ClServerEvent *ai_poObserver)
{
  //## begin ClSSInterfaz::AttachObserver%942850406.body preserve=yes
	m_poImplementation -> AttachObserver (  ai_poObserver );
  //## end ClSSInterfaz::AttachObserver%942850406.body
}

void ClSSInterfaz::GetState (eStatus &ao_eState)
{
  //## begin ClSSInterfaz::GetState%954231002.body preserve=yes
	m_poImplementation -> GetState ( (ClSSImplementation::eStatus &) ao_eState ) ;
  //## end ClSSInterfaz::GetState%954231002.body
}

m4return_t ClSSInterfaz::GetConfigNumericValue (m4pchar_t ai_pszName, m4double_t &ao_rdNumber)
{
  //## begin ClSSInterfaz::GetConfigNumericValue%954231000.body preserve=yes
	return m_poImplementation -> GetConfigNumericValue (ai_pszName, ao_rdNumber);
  //## end ClSSInterfaz::GetConfigNumericValue%954231000.body
}

m4return_t ClSSInterfaz::GetConfigStringValue (m4pchar_t ai_pszName, m4pchar_t ao_pszString, m4size_t ai_iStringSize, m4size_t& ao_riStringLength)
{
  //## begin ClSSInterfaz::GetConfigStringValue%954231001.body preserve=yes
	return m_poImplementation ->  GetConfigStringValue (ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength);
  //## end ClSSInterfaz::GetConfigStringValue%954231001.body
}

m4return_t ClSSInterfaz::SetConfigNumericProperty (m4pchar_t ai_szProperty, m4double_t ai_iValue)
{
  //## begin ClSSInterfaz::SetConfigNumericProperty%954231005.body preserve=yes
	return m_poImplementation -> SetConfigNumericProperty (ai_szProperty, ai_iValue);
  //## end ClSSInterfaz::SetConfigNumericProperty%954231005.body
}

m4return_t ClSSInterfaz::SetConfigStringProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue)
{
  //## begin ClSSInterfaz::SetConfigStringProperty%954231006.body preserve=yes
	return m_poImplementation -> SetConfigStringProperty (ai_szProperty, ai_szValue);
  //## end ClSSInterfaz::SetConfigStringProperty%954231006.body
}

int ClSSInterfaz::GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClSSInterfaz::GetInstance%954231008.body preserve=yes
	return m_poImplementation -> GetInstance (ao_pszString, ai_iStringSize, ao_riOutput);
  //## end ClSSInterfaz::GetInstance%954231008.body
}

m4return_t ClSSInterfaz::SetElement (m4pchar_t ai_szElement, void *ai_poElement)
{
  //## begin ClSSInterfaz::SetElement%955100218.body preserve=yes
	return m_poImplementation -> SetElement (ai_szElement, ai_poElement);
  //## end ClSSInterfaz::SetElement%955100218.body
}

m4return_t ClSSInterfaz::GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSInterfaz::GetStatisticAttribValue%994763177.body preserve=yes
	return m_poImplementation ->   GetStatisticAttribValue (ai_szPath, ai_iStatId, ao_poDataStorage) ;
  //## end ClSSInterfaz::GetStatisticAttribValue%994763177.body
}

m4return_t ClSSInterfaz::GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSInterfaz::GetStatisticAttribsId%994763178.body preserve=yes
	return m_poImplementation ->   GetStatisticAttribsId (ai_szPath, ao_poDataStorage) ;

  //## end ClSSInterfaz::GetStatisticAttribsId%994763178.body
}

m4return_t ClSSInterfaz::GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSInterfaz::GetSubsystemAttribValue%994763179.body preserve=yes
	return m_poImplementation ->   GetSubsystemAttribValue (ai_szPath, ai_iAttribId, ao_poDataStorage) ;

  //## end ClSSInterfaz::GetSubsystemAttribValue%994763179.body
}

m4return_t ClSSInterfaz::GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSInterfaz::GetSubsystemAttribsId%994763180.body preserve=yes
	return m_poImplementation ->   GetSubsystemAttribsId (ai_szPath, ao_poDataStorage) ;

  //## end ClSSInterfaz::GetSubsystemAttribsId%994763180.body
}

m4pchar_t ClSSInterfaz::GetssId ()
{
  //## begin ClSSInterfaz::GetssId%994922160.body preserve=yes
	return m_poImplementation -> GetssId () ;
  //## end ClSSInterfaz::GetssId%994922160.body
}

m4pchar_t ClSSInterfaz::GetPath ()
{
  //## begin ClSSInterfaz::GetPath%994922161.body preserve=yes
	return m_poImplementation -> GetPath () ;
  //## end ClSSInterfaz::GetPath%994922161.body
}

M4ISnmpObject* ClSSInterfaz::GetSnmpObject ()
{
  //## begin ClSSInterfaz::GetSnmpObject%1067856734.body preserve=yes
	return m_poImplementation -> GetSnmpObject();
  //## end ClSSInterfaz::GetSnmpObject%1067856734.body
}

// Additional Declarations
  //## begin ClSSInterfaz%37A5533500A2.declarations preserve=yes
  //## end ClSSInterfaz%37A5533500A2.declarations

//## begin module%37A5559D033C.epilog preserve=yes
//## end module%37A5559D033C.epilog
