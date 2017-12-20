//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                iteminfo.cpp
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
//    Definición de un item en una sentencia select
//
//
//==============================================================================

#include "iteminfo.hpp"
#include <malloc.h>
#include <stdlib.h>


ItemInfo::ItemInfo(m4pcchar_t ai_pccItemName)
{
	m_pcItemName = strdup (ai_pccItemName);
	m_bIsSessionChannel = M4_FALSE;

	m_eType = ITEMINFO;
}


ItemInfo::~ItemInfo()
{
	free( m_pcItemName ) ;
}


m4void_t ItemInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";
	
	if (m_bIsSessionChannel)
		ai_oStream << "$"; 
	else
		ai_oStream << "@";
	
	ai_oStream << m_pcItemName;
}


m4void_t ItemInfo::SetSessionChannel (m4bool_t ai_bIsSessionChannel )
{
	m_bIsSessionChannel = ai_bIsSessionChannel;
}

