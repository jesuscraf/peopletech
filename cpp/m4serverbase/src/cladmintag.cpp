//## begin module%35AF75D10376.cm preserve=no
//## end module%35AF75D10376.cm

//## begin module%35AF75D10376.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              cladmintag.cpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines classes to proccess the attributes in the 
//		administration commands.
//
//
//	=============================================================================
//## end module%35AF75D10376.cp

//## Module: ClAdminTag%35AF75D10376; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: e:\source\m4serverbase\version\src\cladmintag.cpp

//## begin module%35AF75D10376.additionalIncludes preserve=no
//## end module%35AF75D10376.additionalIncludes

//## begin module%35AF75D10376.includes preserve=yes
#include "m4trace.hpp"
//## end module%35AF75D10376.includes

// ClAdminTag
#include <cladmintag.hpp>


//## begin module%35AF75D10376.declarations preserve=no
//## end module%35AF75D10376.declarations

//## begin module%35AF75D10376.additionalDeclarations preserve=yes
//## end module%35AF75D10376.additionalDeclarations


// Class search_by_tag_id 


// Additional Declarations
  //## begin search_by_tag_id%35BDB1DC00EC.declarations preserve=yes
  //## end search_by_tag_id%35BDB1DC00EC.declarations

// Class ClAdminTag 







ClAdminTag::ClAdminTag (const ClAdminTag &ai_tag)
  //## begin ClAdminTag::ClAdminTag%900494633.hasinit preserve=no
      : m_tagStringId(NULL), m_tagType(invalidType), m_tagValue(NULL), m_tagWithParameters(M4_FALSE), m_tagRetValue(M4_FALSE)
  //## end ClAdminTag::ClAdminTag%900494633.hasinit
  //## begin ClAdminTag::ClAdminTag%900494633.initialization preserve=yes
  //## end ClAdminTag::ClAdminTag%900494633.initialization
{
  //## begin ClAdminTag::ClAdminTag%900494633.body preserve=yes
	//--------------------------------------------------------------------
	// set m_tagId
	//--------------------------------------------------------------------
	m_tagId = ai_tag.m_tagId;
	
	//--------------------------------------------------------------------
	// set m_tagStringId
	//--------------------------------------------------------------------
	if (ai_tag.m_tagStringId != NULL)
	{
		m_tagStringId = new m4char_t[strlen(ai_tag.m_tagStringId)+1];
		M4_ASSERT(m_tagStringId != NULL);
		if (m_tagStringId != NULL)
		{
			strcpy(m_tagStringId, ai_tag.m_tagStringId);
		}
		else
		{
			m_tagStringId = NULL;
		}
	}
	else
	{
		m_tagStringId = NULL;
	}
	
	//--------------------------------------------------------------------
	// set m_tagType
	//--------------------------------------------------------------------
	m_tagType = ai_tag.m_tagType;

	//--------------------------------------------------------------------
	// set m_tagValue
	//--------------------------------------------------------------------
	if (ai_tag.m_tagValue != NULL)
	{
		m_tagValue = new m4char_t[strlen(ai_tag.m_tagValue)+1];
		M4_ASSERT(m_tagValue != NULL);
		if (m_tagValue != NULL)
		{
			strcpy(m_tagValue, ai_tag.m_tagValue);
		}
		else
		{
			m_tagValue = NULL;
		}
	}
	else
	{
		m_tagValue = NULL;
	}

	//--------------------------------------------------------------------
	// set m_tagWithParameters
	//--------------------------------------------------------------------
	m_tagWithParameters = ai_tag.m_tagWithParameters;
	m_tagRetValue=ai_tag.m_tagRetValue;
  //## end ClAdminTag::ClAdminTag%900494633.body
}

ClAdminTag::ClAdminTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, m4bool_t ai_tagWithParams, m4bool_t ai_bRetValue)
  //## begin ClAdminTag::ClAdminTag%901005788.hasinit preserve=no
      : m_tagStringId(NULL), m_tagType(invalidType), m_tagValue(NULL), m_tagWithParameters(M4_FALSE), m_tagRetValue(M4_FALSE)
  //## end ClAdminTag::ClAdminTag%901005788.hasinit
  //## begin ClAdminTag::ClAdminTag%901005788.initialization preserve=yes
  //## end ClAdminTag::ClAdminTag%901005788.initialization
{
  //## begin ClAdminTag::ClAdminTag%901005788.body preserve=yes
	//--------------------------------------------------------------------
	// set m_tagId
	//--------------------------------------------------------------------
	m_tagId = ai_tagId;
	
	//--------------------------------------------------------------------
	// set m_tagStringId
	//--------------------------------------------------------------------
	if (ai_tagStringId != NULL)
	{
		m_tagStringId = new m4char_t[strlen(ai_tagStringId)+1];
		M4_ASSERT(m_tagStringId != NULL);
		if (m_tagStringId != NULL)
		{
			strcpy(m_tagStringId, ai_tagStringId);
		}
		else
		{
			m_tagStringId = NULL;
		}
	}
	else
	{
		m_tagStringId = NULL;
	}
	//--------------------------------------------------------------------
	// set m_tagWithParameters
	//--------------------------------------------------------------------
	m_tagWithParameters = ai_tagWithParams;
	m_tagRetValue=ai_bRetValue;
  //## end ClAdminTag::ClAdminTag%901005788.body
}


ClAdminTag::~ClAdminTag ()
{
  //## begin ClAdminTag::~ClAdminTag%900494632.body preserve=yes
	//--------------------------------------------------------------------
	// liberar memoria
	//--------------------------------------------------------------------
	if (m_tagStringId != NULL)
	{
		delete m_tagStringId;
		m_tagStringId = NULL;
	}

	if (m_tagValue != NULL)
	{
		delete m_tagValue;
		m_tagValue = NULL;
	}
  //## end ClAdminTag::~ClAdminTag%900494632.body
}



//## Other Operations (implementation)
ClAdminTag & ClAdminTag::operator = (const ClAdminTag &ai_tag)
{
  //## begin ClAdminTag::operator=%900494634.body preserve=yes
	//--------------------------------------------------------------------
	// set m_tagId
	//--------------------------------------------------------------------
	m_tagId = ai_tag.m_tagId;
		
	//--------------------------------------------------------------------
	// set m_tagStringId
	//--------------------------------------------------------------------
	if (m_tagStringId != NULL)
	{
		delete m_tagStringId;
		m_tagStringId = NULL;
	}
	if (ai_tag.m_tagStringId != NULL)
	{
		m_tagStringId = new m4char_t[strlen(ai_tag.m_tagStringId)+1];
		M4_ASSERT(m_tagStringId != NULL);
		if (m_tagStringId != NULL)
		{
			strcpy(m_tagStringId, ai_tag.m_tagStringId);
		}
		else
		{
			m_tagStringId = NULL;
		}
	}
	else
	{
		m_tagStringId = NULL;
	}
	
	//--------------------------------------------------------------------
	// set m_tagType
	//--------------------------------------------------------------------
	m_tagType = ai_tag.m_tagType;

	//--------------------------------------------------------------------
	// set m_tagValue
	//--------------------------------------------------------------------
	if (m_tagValue != NULL)
	{
		delete m_tagValue;
		m_tagValue = NULL;
	}
	if (ai_tag.m_tagValue != NULL)
	{
		m_tagValue = new m4char_t[strlen(ai_tag.m_tagValue)+1];
		M4_ASSERT(m_tagValue != NULL);
		if (m_tagValue != NULL)
		{
			strcpy(m_tagValue, ai_tag.m_tagValue);
		}
		else
		{
			m_tagValue = NULL;
		}
	}
	else
	{
		m_tagValue = NULL;
	}
	m_tagRetValue = ai_tag.m_tagRetValue;
	//--------------------------------------------------------------------
	// normal return
	//--------------------------------------------------------------------
	return *this;
  //## end ClAdminTag::operator=%900494634.body
}

ClAdminTag * ClAdminTag::GetNewAdminTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, m4bool_t ai_tagWithParams, m4bool_t ai_bRetValue)
{
  //## begin ClAdminTag::GetNewAdminTag%901526259.body preserve=yes
	return new ClAdminTag(ai_tagId, ai_tagStringId, ai_tagWithParams);
  //## end ClAdminTag::GetNewAdminTag%901526259.body
}

// Additional Declarations
  //## begin ClAdminTag%35AB7B4C00BC.declarations preserve=yes
  //## end ClAdminTag%35AB7B4C00BC.declarations

// Class ClAdminCommandTag 



ClAdminCommandTag::ClAdminCommandTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, ClAdminParamType ai_tagType, const m4pchar_t ai_tagValue, m4bool_t ai_bRetValue)
  //## begin ClAdminCommandTag::ClAdminCommandTag%901005790.hasinit preserve=no
  //## end ClAdminCommandTag::ClAdminCommandTag%901005790.hasinit
  //## begin ClAdminCommandTag::ClAdminCommandTag%901005790.initialization preserve=yes
  :ClAdminTag(ai_tagId, ai_tagStringId,M4_FALSE,ai_bRetValue)
  //## end ClAdminCommandTag::ClAdminCommandTag%901005790.initialization
{
  //## begin ClAdminCommandTag::ClAdminCommandTag%901005790.body preserve=yes
	
	//--------------------------------------------------------------------
	// set m_tagType
	//--------------------------------------------------------------------
	m_tagType = ai_tagType;

	//--------------------------------------------------------------------
	// set m_tagValue
	//--------------------------------------------------------------------
	if (ai_tagValue != NULL)
	{
		m_tagValue = new m4char_t[strlen(ai_tagValue)+1];
		M4_ASSERT(m_tagValue != NULL);
		if (m_tagValue != NULL)
		{
			strcpy(m_tagValue, ai_tagValue);
		}
		else
		{
			m_tagValue = NULL;
		}
	}
	else
	{
		m_tagValue = NULL;
	}
  //## end ClAdminCommandTag::ClAdminCommandTag%901005790.body
}



//## Other Operations (implementation)
ClAdminCommandTag * ClAdminCommandTag::GetNewAdminCommandTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, ClAdminParamType ai_tagType, const m4pchar_t ai_tagValue, m4bool_t ai_bRetValue)
{
  //## begin ClAdminCommandTag::GetNewAdminCommandTag%901526260.body preserve=yes
	return new ClAdminCommandTag(ai_tagId, ai_tagStringId, ai_tagType, ai_tagValue,ai_bRetValue);
  //## end ClAdminCommandTag::GetNewAdminCommandTag%901526260.body
}

// Additional Declarations
  //## begin ClAdminCommandTag%35AC83EC002B.declarations preserve=yes
  //## end ClAdminCommandTag%35AC83EC002B.declarations

// Parameterized Class ClAdminList 


// Additional Declarations
  //## begin ClAdminList%35B59A2D02F2.declarations preserve=yes
  //## end ClAdminList%35B59A2D02F2.declarations

//## begin module%35AF75D10376.epilog preserve=yes
//## end module%35AF75D10376.epilog
