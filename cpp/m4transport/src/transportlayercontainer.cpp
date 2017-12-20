//## begin module%38EB1AB20304.cm preserve=no
//## end module%38EB1AB20304.cm

//## begin module%38EB1AB20304.cp preserve=no
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
//## end module%38EB1AB20304.cp

//## Module: TransportLayerContainer%38EB1AB20304; Package body
//## Subsystem: m4transport::M4CTL::src%38D1FCC40071
//## Source file: F:\integration\m4transport\src\transportlayercontainer.cpp

//## begin module%38EB1AB20304.additionalIncludes preserve=no
//## end module%38EB1AB20304.additionalIncludes

//## begin module%38EB1AB20304.includes preserve=yes
//## end module%38EB1AB20304.includes

// CommComun
#include <commcomun.hpp>
// TransportLayerContainer
#include <transportlayercontainer.hpp>
// TransportLayerBase
#include <transportlayerbase.hpp>
// tsap
#include <tsap.hpp>
// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// RecycleableCollector
#include <recycleablecollector.hpp>
//## begin module%38EB1AB20304.declarations preserve=no
//## end module%38EB1AB20304.declarations

//## begin module%38EB1AB20304.additionalDeclarations preserve=yes
#include <m4trace.hpp>
//## end module%38EB1AB20304.additionalDeclarations


// Class ClTransportLayerContainer 


//## begin ClTransportLayerContainer::m_lTSAPId%3C7BB6BA01DA.attr preserve=no  private: static m4uint32_t {UA} 5
m4uint32_t ClTransportLayerContainer::m_lTSAPId = 5;
//## end ClTransportLayerContainer::m_lTSAPId%3C7BB6BA01DA.attr










ClTransportLayerContainer::ClTransportLayerContainer ()
  //## begin ClTransportLayerContainer::ClTransportLayerContainer%956912723.hasinit preserve=no
      : m_bWantStop(M4_FALSE)
  //## end ClTransportLayerContainer::ClTransportLayerContainer%956912723.hasinit
  //## begin ClTransportLayerContainer::ClTransportLayerContainer%956912723.initialization preserve=yes
  ,m_ListTransportAction(5000)
  //## end ClTransportLayerContainer::ClTransportLayerContainer%956912723.initialization
{
  //## begin ClTransportLayerContainer::ClTransportLayerContainer%956912723.body preserve=yes
	m_SyncGarbageableCollector.Init();
	m_SyncListAction.Init();
  //## end ClTransportLayerContainer::ClTransportLayerContainer%956912723.body
}


ClTransportLayerContainer::~ClTransportLayerContainer ()
{
  //## begin ClTransportLayerContainer::~ClTransportLayerContainer%958576517.body preserve=yes
// Se ha cambiado el index del mapa por un id
	ClTransportLayerIdentifier * pTransportLayerIdentifier ;
	ClMapTransportLayerIdentifier::iterator ite = m_MapTransportLayerIdentifier.begin();
	while (ite != m_MapTransportLayerIdentifier.end())
	{
		pTransportLayerIdentifier = ((*ite).second);

		if ( pTransportLayerIdentifier  )
		{
		
			delete (ClTSAP *)(pTransportLayerIdentifier -> GetTSAP());
			delete pTransportLayerIdentifier ;
		}

		m_MapTransportLayerIdentifier.erase(ite);
		ite = m_MapTransportLayerIdentifier.begin();

	}
  //## end ClTransportLayerContainer::~ClTransportLayerContainer%958576517.body
}



//## Other Operations (implementation)
m4return_t ClTransportLayerContainer::AddTransportLayerIdentifier (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier)
{
  //## begin ClTransportLayerContainer::AddTransportLayerIdentifier%954835578.body preserve=yes
// Se ha cambiado el index del mapa por un id
	pair< m4uint32_t , ClTransportLayerIdentifier* > p = 
		make_pair((m4uint32_t)(ai_pTransportLayerIdentifier -> GetTSAP() ), ai_pTransportLayerIdentifier);
	pair< ClMapTransportLayerIdentifier::iterator, bool > ret; 

	ret = m_MapTransportLayerIdentifier.insert(p);
	if (!ret.second)
	{
		if (ai_pTransportLayerIdentifier->GetTSAP() != NULL)
		{
			m4Trace(fprintf(stderr, "Error insertando ClTransportLayerIdentifier* con socket %d\n", (ai_pTransportLayerIdentifier->GetTSAP())->GetSystemDescriptor()));
		}
		else
		{
			m4Trace(fprintf(stderr, "Error insertando ClTransportLayerIdentifier* \n"));
		}

		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClTransportLayerContainer::AddTransportLayerIdentifier%954835578.body
}

m4return_t ClTransportLayerContainer::RemoveTransportLayerIdentifier (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier)
{
  //## begin ClTransportLayerContainer::RemoveTransportLayerIdentifier%954835579.body preserve=yes
// Se ha cambiado el index del mapa por un id
	ClMapTransportLayerIdentifier::iterator Ite;

	Ite = m_MapTransportLayerIdentifier.find((m4uint32_t)ai_pTransportLayerIdentifier->GetTSAP() );
	if ( Ite == m_MapTransportLayerIdentifier.end() ) 
		return M4_ERROR;

	m_MapTransportLayerIdentifier.erase( Ite );

	return M4_SUCCESS;
  //## end ClTransportLayerContainer::RemoveTransportLayerIdentifier%954835579.body
}

m4return_t ClTransportLayerContainer::AddSelect (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier, m4bool_t ai_bRead, m4bool_t ai_bWrite)
{
  //## begin ClTransportLayerContainer::AddSelect%954835580.body preserve=yes
// Se ha cambiado el index del mapa por un id
	if (m_MapTransportLayerIdentifier.find((m4uint32_t)ai_pTransportLayerIdentifier->GetTSAP()) == m_MapTransportLayerIdentifier.end())
		return M4_ERROR;
	m_TSAPSelect.Add(ai_pTransportLayerIdentifier->GetTSAP(), ai_bRead, ai_bWrite, M4_FALSE);

	return M4_SUCCESS;
  //## end ClTransportLayerContainer::AddSelect%954835580.body
}

m4return_t ClTransportLayerContainer::RemoveSelect (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier, m4bool_t ai_bRead, m4bool_t ai_bWrite)
{
  //## begin ClTransportLayerContainer::RemoveSelect%954835581.body preserve=yes
// Se ha cambiado el index del mapa por un id
	if (m_MapTransportLayerIdentifier.find((m4uint32_t)ai_pTransportLayerIdentifier->GetTSAP()) == m_MapTransportLayerIdentifier.end())
		return M4_ERROR;
	m_TSAPSelect.Remove(ai_pTransportLayerIdentifier->GetTSAP(), ai_bRead, ai_bWrite, M4_FALSE);

	return M4_SUCCESS;
  //## end ClTransportLayerContainer::RemoveSelect%954835581.body
}

int ClTransportLayerContainer::Select (unsigned long ai_Seconds, unsigned long ai_Useconds)
{
  //## begin ClTransportLayerContainer::Select%954835582.body preserve=yes
	return m_TSAPSelect.Select(ai_Seconds, ai_Useconds, M4_TRUE, M4_TRUE, M4_FALSE);

  //## end ClTransportLayerContainer::Select%954835582.body
}

m4return_t ClTransportLayerContainer::AddAction (ClTransportAction *ai_pAction)
{
  //## begin ClTransportLayerContainer::AddAction%954835583.body preserve=yes
	//m_SyncListAction.Lock();
	m_ListTransportAction.Push(ai_pAction);
	//m_SyncListAction.Unlock();

	return M4_SUCCESS;
  //## end ClTransportLayerContainer::AddAction%954835583.body
}

ClTransportAction * ClTransportLayerContainer::GetAction ()
{
  //## begin ClTransportLayerContainer::GetAction%954835584.body preserve=yes
	ClTransportAction *pAction = NULL;

	//m_SyncListAction.Lock();
	if (m_ListTransportAction.Size() > 0)
		m_ListTransportAction.Pop(pAction);
	//m_SyncListAction.Unlock();

	return pAction;
  //## end ClTransportLayerContainer::GetAction%954835584.body
}

ClTransportLayerIdentifier * ClTransportLayerContainer::NextRead ()
{
  //## begin ClTransportLayerContainer::NextRead%954835585.body preserve=yes
	ClTSAP *pTSAP =  m_TSAPSelect.NextRead();

	if (pTSAP == NULL)
		return NULL;

	ClMapTransportLayerIdentifier::iterator Ite;
	Ite = m_MapTransportLayerIdentifier.find((m4uint32_t)pTSAP );
	if ( Ite == m_MapTransportLayerIdentifier.end() ) 
		return NULL;

	return (*Ite).second;
  //## end ClTransportLayerContainer::NextRead%954835585.body
}

ClTransportLayerIdentifier * ClTransportLayerContainer::NextWrite ()
{
  //## begin ClTransportLayerContainer::NextWrite%954835586.body preserve=yes
	ClTSAP *pTSAP = m_TSAPSelect.NextWrite();
	if (pTSAP == NULL)
		return NULL;

	ClMapTransportLayerIdentifier::iterator Ite;
	Ite = m_MapTransportLayerIdentifier.find((m4uint32_t)pTSAP );
	if ( Ite == m_MapTransportLayerIdentifier.end() ) 
		return NULL;

	return (*Ite).second;
  //## end ClTransportLayerContainer::NextWrite%954835586.body
}

ClTransportLayerIdentifier * ClTransportLayerContainer::GetTransportLayerIdentifier (ClTSAP *ai_pTSAP)
{
  //## begin ClTransportLayerContainer::GetTransportLayerIdentifier%955466403.body preserve=yes
	ClMapTransportLayerIdentifier::iterator Ite;
	Ite = m_MapTransportLayerIdentifier.find((m4uint32_t)ai_pTSAP );
	if ( Ite == m_MapTransportLayerIdentifier.end() ) 
		return NULL;

	return (*Ite).second;
  //## end ClTransportLayerContainer::GetTransportLayerIdentifier%955466403.body
}

m4bool_t ClTransportLayerContainer::WantStop ()
{
  //## begin ClTransportLayerContainer::WantStop%956912721.body preserve=yes
	return m_bWantStop;
  //## end ClTransportLayerContainer::WantStop%956912721.body
}

void ClTransportLayerContainer::SetWantStop (ClIOEventHandler *ai_iIOEventHandler)
{
  //## begin ClTransportLayerContainer::SetWantStop%956912722.body preserve=yes
	m_bWantStop = M4_TRUE;
	m_pWantStopClIOEventHandler=ai_iIOEventHandler;
  //## end ClTransportLayerContainer::SetWantStop%956912722.body
}

ClIOEventHandler * ClTransportLayerContainer::GetWantStopIOEventHandler ()
{
  //## begin ClTransportLayerContainer::GetWantStopIOEventHandler%956912725.body preserve=yes
	return m_pWantStopClIOEventHandler;
  //## end ClTransportLayerContainer::GetWantStopIOEventHandler%956912725.body
}

IGarbageable * ClTransportLayerContainer::RecoverAction (int ai_iType)
{
  //## begin ClTransportLayerContainer::RecoverAction%961597592.body preserve=yes
	IGarbageable *pGarbageable;
	m_SyncGarbageableCollector.Lock();
	pGarbageable = m_RecycleableCollector.Recover(ai_iType);
	m_SyncGarbageableCollector.Unlock();

	return pGarbageable;
  //## end ClTransportLayerContainer::RecoverAction%961597592.body
}

m4return_t ClTransportLayerContainer::KeepAction (IGarbageable &ai_oGarbageable)
{
  //## begin ClTransportLayerContainer::KeepAction%961597593.body preserve=yes
	m4return_t ret;
	m_SyncGarbageableCollector.Lock();
	ret =  m_RecycleableCollector.DisposeOf(ai_oGarbageable);
	m_SyncGarbageableCollector.Unlock();

	return ret;
  //## end ClTransportLayerContainer::KeepAction%961597593.body
}

// Additional Declarations
  //## begin ClTransportLayerContainer%38EB0E6D0177.declarations preserve=yes
  //## end ClTransportLayerContainer%38EB0E6D0177.declarations

//## begin module%38EB1AB20304.epilog preserve=yes
//## end module%38EB1AB20304.epilog
