//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              adminmoncache.cpp
//	 Project:           M4AdminClient
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines source code for admin monitor cache
//		classes.
//
//	=============================================================================
//## end module.cp

//## Module: MonitorCache; Package body
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\src\adminmoncache.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// MonitorCache
#include <adminmoncache.hpp>
// AdminMonClient
#include <adminmonclient.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClAdminInfoTagCache 







ClAdminInfoTagCache::ClAdminInfoTagCache ()
  //## begin ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.hasinit preserve=no
      : m_IsValid(M4_FALSE), m_InfoId(-1)
  //## end ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.hasinit
  //## begin ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.initialization preserve=yes
  //## end ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.initialization
{
  //## begin ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.body preserve=yes
  //## end ClAdminInfoTagCache::ClAdminInfoTagCache%917339823.body
}



//## Other Operations (implementation)
m4bool_t ClAdminInfoTagCache::Set (const M4ClString &ai_Info, int ai_InfoId)
{
  //## begin ClAdminInfoTagCache::Set%917339807.body preserve=yes
	m_Info = ai_Info;
	m_InfoId = ai_InfoId;
	m_IsValid = M4_TRUE;
	
	return M4_TRUE;
  //## end ClAdminInfoTagCache::Set%917339807.body
}

m4bool_t ClAdminInfoTagCache::Get (M4ClString &ao_Info)
{
  //## begin ClAdminInfoTagCache::Get%917339808.body preserve=yes
	if (M4_FALSE == m_IsValid)
	{
		return M4_FALSE;
	}
	ao_Info = m_Info;
	m_IsValid = M4_FALSE;

	return M4_TRUE;
  //## end ClAdminInfoTagCache::Get%917339808.body
}

void ClAdminInfoTagCache::Reset ()
{
  //## begin ClAdminInfoTagCache::Reset%917339815.body preserve=yes
	m_IsValid = M4_FALSE;
  //## end ClAdminInfoTagCache::Reset%917339815.body
}

int ClAdminInfoTagCache::GetId ()
{
  //## begin ClAdminInfoTagCache::GetId%917426673.body preserve=yes
	return m_InfoId;
  //## end ClAdminInfoTagCache::GetId%917426673.body
}

// Additional Declarations
  //## begin ClAdminInfoTagCache.declarations preserve=yes
  //## end ClAdminInfoTagCache.declarations



// Class ClAdminInfoCache 














//## Other Operations (implementation)
m4return_t ClAdminInfoCache::SetIds (const M4ClString &ai_Path, const M4ClString &ai_Ids)
{
  //## begin ClAdminInfoCache::SetIds%917339816.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}
	m_Ids.Set(ai_Ids);

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetIds%917339816.body
}

m4return_t ClAdminInfoCache::SetAttributes (const M4ClString &ai_Path, const M4ClString &ai_Attributes)
{
  //## begin ClAdminInfoCache::SetAttributes%917339817.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}
	m_Attributes.Set(ai_Attributes);

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetAttributes%917339817.body
}

m4return_t ClAdminInfoCache::SetStatistics (const M4ClString &ai_Path, const M4ClString &ai_Statistics)
{
  //## begin ClAdminInfoCache::SetStatistics%917339818.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}
	m_Statistics.Set(ai_Statistics);

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetStatistics%917339818.body
}

m4return_t ClAdminInfoCache::SetCommands (const M4ClString &ai_Path, const M4ClString &ai_Commands)
{
  //## begin ClAdminInfoCache::SetCommands%917339819.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}
	m_Commands.Set(ai_Commands);

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetCommands%917339819.body
}

m4return_t ClAdminInfoCache::SetAttributeValues (const M4ClString &ai_Path, int ai_nAttributes, const M4ClString *ai_Attributes, const int *ai_AttributesId)
{
  //## begin ClAdminInfoCache::SetAttributeValues%917339820.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}

	for (int i=0; i<ai_nAttributes; i++)
	{
		ClAdminInfoTagCache item;
		item.Set(ai_Attributes[i], ai_AttributesId[i]);
		m_AttributeValues.push_back(item);
	}

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetAttributeValues%917339820.body
}

m4return_t ClAdminInfoCache::SetCommandParams (const M4ClString &ai_Path, int ai_nCommandParams, const M4ClString *ai_CommandParams, const int *ai_ParamsId)
{
  //## begin ClAdminInfoCache::SetCommandParams%917339821.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}

	for (int i=0; i<ai_nCommandParams; i++)
	{
		ClAdminInfoTagCache item;
		item.Set(ai_CommandParams[i], ai_ParamsId[i]);
		m_CommandParams.push_back(item);
	}

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetCommandParams%917339821.body
}

m4return_t ClAdminInfoCache::SetStatisticValues (const M4ClString &ai_Path, int ai_nStatistics, const M4ClString *ai_Statistics, const int *ai_StatisticsId)
{
  //## begin ClAdminInfoCache::SetStatisticValues%917339822.body preserve=yes
	if (m_Path != ai_Path)
	{
		Reset();
		m_Path = ai_Path;
	}

	for (int i=0; i<ai_nStatistics; i++)
	{
		ClAdminInfoTagCache item;
		item.Set(ai_Statistics[i], ai_StatisticsId[i]);
		m_StatisticValues.push_back(item);
	}

 	return M4_SUCCESS;
  //## end ClAdminInfoCache::SetStatisticValues%917339822.body
}

m4bool_t ClAdminInfoCache::GetIds (const M4ClString &ai_Path, M4ClString &ao_Ids)
{
  //## begin ClAdminInfoCache::GetIds%917426675.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	return m_Ids.Get(ao_Ids);
  //## end ClAdminInfoCache::GetIds%917426675.body
}

m4bool_t ClAdminInfoCache::GetAttributes (const M4ClString &ai_Path, M4ClString &ao_Attributes)
{
  //## begin ClAdminInfoCache::GetAttributes%917426666.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	return m_Attributes.Get(ao_Attributes);
  //## end ClAdminInfoCache::GetAttributes%917426666.body
}

m4bool_t ClAdminInfoCache::GetStatistics (const M4ClString &ai_Path, M4ClString &ao_Statistics)
{
  //## begin ClAdminInfoCache::GetStatistics%917426667.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	return m_Statistics.Get(ao_Statistics);
  //## end ClAdminInfoCache::GetStatistics%917426667.body
}

m4bool_t ClAdminInfoCache::GetCommands (const M4ClString &ai_Path, M4ClString &ao_Commands)
{
  //## begin ClAdminInfoCache::GetCommands%917426668.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	return m_Commands.Get(ao_Commands);
  //## end ClAdminInfoCache::GetCommands%917426668.body
}

m4bool_t ClAdminInfoCache::GetAttributeValue (const M4ClString &ai_Path, int ai_AttributeId, M4ClString &ao_Attribute)
{
  //## begin ClAdminInfoCache::GetAttributeValue%917426669.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	deque<ClAdminInfoTagCache>::iterator iter;
	for (iter = m_AttributeValues.begin(); iter != m_AttributeValues.end(); iter++)
	{
		ClAdminInfoTagCache& item = *iter;
		if (item.GetId() == ai_AttributeId)
		{
			return item.Get(ao_Attribute);
		}
	}

	return M4_FALSE;
  //## end ClAdminInfoCache::GetAttributeValue%917426669.body
}

m4bool_t ClAdminInfoCache::GetStatisticValue (const M4ClString &ai_Path, int ai_StatisticId, M4ClString &ao_Statistic)
{
  //## begin ClAdminInfoCache::GetStatisticValue%917426670.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	deque<ClAdminInfoTagCache>::iterator iter;
	for (iter = m_StatisticValues.begin(); iter != m_StatisticValues.end(); iter++)
	{
		ClAdminInfoTagCache& item = *iter;
		if (item.GetId() == ai_StatisticId)
		{
			return item.Get(ao_Statistic);
		}
	}

	return M4_FALSE;
  //## end ClAdminInfoCache::GetStatisticValue%917426670.body
}

m4bool_t ClAdminInfoCache::GetCommandParams (const M4ClString &ai_Path, int ai_CommandId, M4ClString &ao_Params)
{
  //## begin ClAdminInfoCache::GetCommandParams%917426671.body preserve=yes
	if (m_Path != ai_Path)
	{
		return M4_FALSE;
	}

	deque<ClAdminInfoTagCache>::iterator iter;
	for (iter = m_CommandParams.begin(); iter != m_CommandParams.end(); iter++)
	{
		ClAdminInfoTagCache& item = *iter;
		if (item.GetId() == ai_CommandId)
		{
			return item.Get(ao_Params);
		}
	}

	return M4_FALSE;
  //## end ClAdminInfoCache::GetCommandParams%917426671.body
}

void ClAdminInfoCache::Reset ()
{
  //## begin ClAdminInfoCache::Reset%917426674.body preserve=yes
	// reset all values.
	m_Ids.Reset();
	m_Attributes.Reset();
	m_Statistics.Reset();
	m_Commands.Reset();

	// empty deque values.
	m_AttributeValues.clear();
	m_StatisticValues.clear();
	m_CommandParams.clear();
  //## end ClAdminInfoCache::Reset%917426674.body
}

// Additional Declarations
  //## begin ClAdminInfoCache.declarations preserve=yes
  //## end ClAdminInfoCache.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
