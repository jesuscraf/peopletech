//## begin module%39F71BD70231.cm preserve=no
//## end module%39F71BD70231.cm

//## begin module%39F71BD70231.cp preserve=no
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
//## end module%39F71BD70231.cp

//## Module: appctlnodeinfo%39F71BD70231; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: D:\Work\v600\m4dispatcher\src\appctlnodeinfo.cpp

//## begin module%39F71BD70231.additionalIncludes preserve=no
//## end module%39F71BD70231.additionalIncludes

//## begin module%39F71BD70231.includes preserve=yes
#include "m4stl.hpp"
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"
//## end module%39F71BD70231.includes

// m4string
#include <m4string.hpp>
// appctlnodeinfo
#include <appctlnodeinfo.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
//## begin module%39F71BD70231.declarations preserve=no
//## end module%39F71BD70231.declarations

//## begin module%39F71BD70231.additionalDeclarations preserve=yes
//## end module%39F71BD70231.additionalDeclarations


// Class ClAppctlNodeInfo 



ClAppctlNodeInfo::ClAppctlNodeInfo (m4string_t ai_szHost, m4uint32_t ai_iPort)
  //## begin ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.hasinit preserve=no
  //## end ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.hasinit
  //## begin ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.initialization preserve=yes
  //## end ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.initialization
{
  //## begin ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.body preserve=yes
	int len = ai_szHost.length() + 1;
	m4pchar_t temp = new m4char_t[len];
	strcpy(temp,ai_szHost.c_str());

	m_szSiteNodeHost = ToLower(temp) ;
	m_iAppctlPort = ai_iPort ;

	delete [] temp;
  //## end ClAppctlNodeInfo::ClAppctlNodeInfo%972463875.body
}



//## Other Operations (implementation)
ClAppctlNodeInfo & ClAppctlNodeInfo::operator = (const ClAppctlNodeInfo &ai_oAppctlNodeInfo)
{
  //## begin ClAppctlNodeInfo::operator =%972463876.body preserve=yes
	if (&ai_oAppctlNodeInfo!= this)
	{
		m_szSiteNodeHost = ai_oAppctlNodeInfo.GetSiteNodeHost () ;
		m_iAppctlPort = ai_oAppctlNodeInfo.GetAppctlPort () ;
	}

	return *this;
  //## end ClAppctlNodeInfo::operator =%972463876.body
}

m4bool_t operator == (const ClAppctlNodeInfo &ai_oRAppctlInfo, const ClAppctlNodeInfo &ai_oLAppctlinfo)
{
  //## begin ClAppctlNodeInfo::operator ==%972463881.body preserve=yes
	return stricmp (ai_oRAppctlInfo.GetSiteNodeHost().c_str() , ai_oLAppctlinfo.GetSiteNodeHost ().c_str() ) ?  M4_FALSE : M4_TRUE ;
  //## end ClAppctlNodeInfo::operator ==%972463881.body
}

// Additional Declarations
  //## begin ClAppctlNodeInfo%39F71A290373.declarations preserve=yes
  //## end ClAppctlNodeInfo%39F71A290373.declarations

// Class ClAppctlNodeRepository 



ClAppctlNodeRepository::ClAppctlNodeRepository ()
  //## begin ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.hasinit preserve=no
  //## end ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.hasinit
  //## begin ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.initialization preserve=yes
  //## end ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.initialization
{
  //## begin ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.body preserve=yes
  //## end ClAppctlNodeRepository::ClAppctlNodeRepository%972463877.body
}



//## Other Operations (implementation)
m4return_t ClAppctlNodeRepository::AddNode (const ClAppctlNodeInfo &ai_oAppctlNodeInfo)
{
  //## begin ClAppctlNodeRepository::AddNode%972463878.body preserve=yes
	TAppMap ::value_type				mapEntry (ai_oAppctlNodeInfo.GetSiteNodeHost () , ai_oAppctlNodeInfo );
	pair<TAppMap::iterator, bool>		mapres;


	mapres = m_oAppctlMap.insert ( mapEntry ) ;

	if (M4_FALSE == mapres.second)
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADDSITENODE,
			"Error adding site node. Cannot add the new site node %0:s.",
			ai_oAppctlNodeInfo.GetSiteNodeHost().c_str() );

		return M4_ERROR;
	}

	return M4_SUCCESS ;
  //## end ClAppctlNodeRepository::AddNode%972463878.body
}

m4return_t ClAppctlNodeRepository::GetNodeInfo (ClAppctlNodeInfo &ai_oAppctlNodeInfo)
{
  //## begin ClAppctlNodeRepository::GetNodeInfo%972463882.body preserve=yes
	TAppMap::iterator					mapIter;

	// fix bug 0088880
	// converts the host name to the real local host name
	ClAppctlNodeInfo oConvertedInfo = ConvertToNodeHost(ai_oAppctlNodeInfo);

	if ( m_oAppctlMap.end() == (mapIter = m_oAppctlMap.find(oConvertedInfo.GetSiteNodeHost() )))
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOGETAPPCTLNODEINFO,
			"Error getting site node information. No Application Controler node in host '%0:s'." , ai_oAppctlNodeInfo.GetSiteNodeHost().c_str() );
		
		return M4_ERROR;
	}

	ai_oAppctlNodeInfo = (*mapIter).second ;

	return M4_SUCCESS ;
  //## end ClAppctlNodeRepository::GetNodeInfo%972463882.body
}

ClAppctlNodeInfo ClAppctlNodeRepository::ConvertToNodeHost (ClAppctlNodeInfo &ai_oInfo)
{
  //## begin ClAppctlNodeRepository::ConvertToNodeHost%1070463940.body preserve=yes

	m4string_t strHost = ai_oInfo.GetSiteNodeHost();

	ClSiteNodeRepository *Rep = ClSiteRepositorySingleton::GetInstance() ;
	ClSiteNodeContainer List ;
	ClSiteNodeInformation Info;
	
	Rep->GetNodeList(List) ;
	
	while (  M4_ERROR != List.GetNextNode(Info) ) 
	{
		if (Info.GetLocalHost() == strHost) {
			return ClAppctlNodeInfo(Info.GetNodeHost(),ai_oInfo.GetAppctlPort());
		}
		
	} ;

	return ai_oInfo;

  //## end ClAppctlNodeRepository::ConvertToNodeHost%1070463940.body
}

// Additional Declarations
  //## begin ClAppctlNodeRepository%39F71C1400F9.declarations preserve=yes
  //## end ClAppctlNodeRepository%39F71C1400F9.declarations

//## begin module%39F71BD70231.epilog preserve=yes
//## end module%39F71BD70231.epilog
