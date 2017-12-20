//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clrepreq.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:21/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"

#include "font.h"
#include "format.h"
#include "cldevice.h"
#include "xapichn.h"
#include "clreparg.h"
#include "field.h"

#include "reportenv.h"

StENG_ReportEnv::StENG_ReportEnv()
{
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poDBFont,		ClFONT_M_DBFont ) ;
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poDBFormat,		ClENG_DBFormat ) ;
	m_poDvc=NULL ;		
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poChnData,		ClMIT_Chn ) ;
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poExeParams,		ClENG_ReportArgs ) ;
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poFieldManager,	ClENG_FieldsManager ) ;
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poTrLit,			TrLitMap_t ) ;
} 

StENG_ReportEnv::~StENG_ReportEnv()
{
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CORE,		m_poDBFont )  ;
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CORE,		m_poDBFormat )  ;
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CORE,		m_poChnData )  ;
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CORE,		m_poExeParams )  ;
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CORE,		m_poFieldManager )  ;
	M4_DELETE_VIRTUAL_ITEM( Insp->m_poMemProf, ENG_MGI_CLDEVICE,	m_poDvc )  ;
	M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poTrLit,	TrLitMap_t ) ;
}


