//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                elementinfo.cpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Definición de una elemento en una sentencia select
//
//
//==============================================================================

#include "elementinfo.hpp"


ElementInfo::ElementInfo()
{
	m_poElementInfoLeft = 0;
	m_poElementInfoRight = 0;

	m_eType = ELEMENTINFO;
}

ElementInfo::~ElementInfo()
{
}


m4void_t ElementInfo::AddLeft (ElementInfo * ai_poElementInfo)
{
	m_poElementInfoLeft = ai_poElementInfo;
}


m4void_t ElementInfo::AddRight (ElementInfo * ai_poElementInfo)
{
	m_poElementInfoRight = ai_poElementInfo;
}


m4void_t ElementInfo::PushLeft (ElementInfo * ai_poElementInfo)
{
	if ( m_poElementInfoLeft == 0 )
		AddLeft (ai_poElementInfo);
	else
		m_poElementInfoLeft->PushLeft (ai_poElementInfo);
}


m4void_t ElementInfo::PushRight (ElementInfo * ai_poElementInfo)
{
	if ( m_poElementInfoRight == 0 )
		AddRight (ai_poElementInfo);
	else
		m_poElementInfoRight->PushRight (ai_poElementInfo);
}


m4void_t ElementInfo::Dump (ostream & ai_oStream)
{
	if (m_poElementInfoLeft != 0)
		m_poElementInfoLeft->Dump (ai_oStream);

	MyDump (ai_oStream);

	if (m_poElementInfoRight != 0)
		m_poElementInfoRight->Dump (ai_oStream);
}


m4void_t ElementInfo::ChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{
	if (m_poElementInfoLeft != 0)
		m_poElementInfoLeft->ChangeAlias(ai_pccOldAlias, ai_pccNewAlias);

	MyChangeAlias (ai_pccOldAlias, ai_pccNewAlias);

	if (m_poElementInfoRight != 0)
		m_poElementInfoRight->ChangeAlias(ai_pccOldAlias, ai_pccNewAlias);
}


m4void_t ElementInfo::MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{
}


M4HSPARSER_ELEMENT_T ElementInfo::GetType ()
{
	return m_eType;
}


ElementInfo * ElementInfo::GetLeft ()
{
	return m_poElementInfoLeft;
}


ElementInfo * ElementInfo::GetRight ()
{
	return m_poElementInfoRight;
}

