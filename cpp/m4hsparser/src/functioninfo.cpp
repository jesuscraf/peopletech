//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                functioninfo.cpp
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
//    Definición de una función en una sentencia select
//
//
//==============================================================================

#include "functioninfo.hpp"
#include <malloc.h>
#include <stdlib.h>


FunctionInfo::FunctionInfo(m4pcchar_t ai_pccName)
{
	m_pcName = strdup (ai_pccName);

	m_eType = FUNCTIONINFO;
}


FunctionInfo::~FunctionInfo()
{
	free( m_pcName ) ;
}


m4void_t FunctionInfo::SetArgument ( ElementInfo * ai_poArgument )
{
	m_poArgument = ai_poArgument;
}


m4void_t FunctionInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";
	ai_oStream << "#" << m_pcName << " ";
	ai_oStream << "(";
	
	if (m_poArgument != 0)
		m_poArgument->Dump (ai_oStream);

	ai_oStream << ")";

}


m4void_t FunctionInfo::MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{
	if (m_poArgument != 0)
		m_poArgument->ChangeAlias (ai_pccOldAlias, ai_pccNewAlias);
}

