//## begin module%3422406A0258.cm preserve=no
//## end module%3422406A0258.cm

//## begin module%3422406A0258.cp preserve=no
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
//## end module%3422406A0258.cp

//## Module: clserviceinfo%3422406A0258; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clserviceinfo.cpp

//## begin module%3422406A0258.additionalIncludes preserve=no
//## end module%3422406A0258.additionalIncludes

//## begin module%3422406A0258.includes preserve=yes
#include <request.hpp>
#include <clserviceinterface.hpp>
#include <exvariables.hpp>
#include <m4log.hpp>
#include <fifoq.hpp>
#include <serviceq.hpp>
#include <cllist.hpp>
#include <clserviceinfo.hpp>
#include <cllocalmonitor.hpp>
#include <subsystemids.hpp>
//## end module%3422406A0258.additionalDeclarations


class ClServiceVisitor;






ClServiceInfo::ClServiceInfo (ClLocalMonitor *ai_poLocalMonitor, ClStatistic *ai_poStatistic, m4uint16_t ai_iMaxNumQItems)
  //## begin ClServiceInfo::ClServiceInfo%876496985.hasinit preserve=no
  //## end ClServiceInfo::ClServiceInfo%876496985.hasinit
  //## begin ClServiceInfo::ClServiceInfo%876496985.initialization preserve=yes
:   m_oLog(), m_poOutputQueue(NULL) 
  //## end ClServiceInfo::ClServiceInfo%876496985.initialization
{
  //## begin ClServiceInfo::ClServiceInfo%876496985.body preserve=yes
	m_poStatistic = ai_poStatistic ;

	m_poServiceQueue = new ClServiceQ ( ai_iMaxNumQItems ) ;

	m_oLauncherList.SetActiveList ( & ( ai_poLocalMonitor -> m_oActiveList ) ) ;

  //## end ClServiceInfo::ClServiceInfo%876496985.body
}


ClServiceInfo::~ClServiceInfo ()
{
  //## begin ClServiceInfo::~ClServiceInfo%876496986.body preserve=yes
/*	if ( m_poServiceQueue )
	{
		delete m_poServiceQueue ;

		m_poServiceQueue = NULL ;
	}
	*/
  //## end ClServiceInfo::~ClServiceInfo%876496986.body
}



//## Other Operations (implementation)
m4return_t ClServiceInfo::Remove ()
{
  //## begin ClServiceInfo::Remove%876496989.body preserve=yes
	

	return M4_SUCCESS ;
  //## end ClServiceInfo::Remove%876496989.body
}

m4return_t ClServiceInfo::Down ()
{
  //## begin ClServiceInfo::Down%876496990.body preserve=yes

	Disable();

	return M4_SUCCESS ;
  //## end ClServiceInfo::Down%876496990.body
}

m4return_t ClServiceInfo::Up ()
{
  //## begin ClServiceInfo::Up%876496991.body preserve=yes

	return M4_SUCCESS ;
  //## end ClServiceInfo::Up%876496991.body
}

void ClServiceInfo::Accept (ClServiceVisitor &ai_oVisitor)
{
  //## begin ClServiceInfo::Accept%878029378.body preserve=yes
  //## end ClServiceInfo::Accept%878029378.body
}

ClServiceQ * ClServiceInfo::GetServiceQ ()
{
  //## begin ClServiceInfo::GetServiceQ%884338124.body preserve=yes
	return  m_poServiceQueue ;
  //## end ClServiceInfo::GetServiceQ%884338124.body
}

ClStatNumericalType ClServiceInfo::GetQueuedItems ()
{
  //## begin ClServiceInfo::GetQueuedItems%889521619.body preserve=yes

	return m_poServiceQueue->Size();

  //## end ClServiceInfo::GetQueuedItems%889521619.body
}

ClOutputQ * ClServiceInfo::GetOutputQ ()
{
  //## begin ClServiceInfo::GetOutputQ%927559636.body preserve=yes
	return  m_poOutputQueue ;
  //## end ClServiceInfo::GetOutputQ%927559636.body
}

void ClServiceInfo::SetOutputQ (ClOutputQ *ai_pOutputQ)
{
  //## begin ClServiceInfo::SetOutputQ%927564337.body preserve=yes
	m_poOutputQueue = ai_pOutputQ;
  //## end ClServiceInfo::SetOutputQ%927564337.body
}

void ClServiceInfo::SetServiceQ (ClServiceQ *ai_poService)
{
  //## begin ClServiceInfo::SetServiceQ%936606376.body preserve=yes
  //## end ClServiceInfo::SetServiceQ%936606376.body
}

m4return_t ClServiceInfo::Recycle ()
{
  //## begin ClServiceInfo::Recycle%941721559.body preserve=yes
	return M4_ERROR ;
  //## end ClServiceInfo::Recycle%941721559.body
}

m4bool_t ClServiceInfo::CanRecycle ()
{
  //## begin ClServiceInfo::CanRecycle%941721560.body preserve=yes
	return M4_FALSE ;
  //## end ClServiceInfo::CanRecycle%941721560.body
}

m4return_t ClServiceInfo::Destroy ()
{
  //## begin ClServiceInfo::Destroy%941721561.body preserve=yes
	return M4_ERROR ;
  //## end ClServiceInfo::Destroy%941721561.body
}

m4bool_t ClServiceInfo::CanDestroy ()
{
  //## begin ClServiceInfo::CanDestroy%941721562.body preserve=yes
	return M4_FALSE ;
  //## end ClServiceInfo::CanDestroy%941721562.body
}

// Additional Declarations
  //## begin ClServiceInfo%3422406402B6.declarations preserve=yes
  //## end ClServiceInfo%3422406402B6.declarations

//## begin module%3422406A0258.epilog preserve=yes
//## end module%3422406A0258.epilog
