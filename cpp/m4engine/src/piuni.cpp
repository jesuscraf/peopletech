//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             piuni.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             18/09/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    PI information container for PDF unicode generation
//
//
//==============================================================================

#include "eng.h"
#include "piuni.h"

//================================================================StPiUnicode_t

void StPiUnicode_t::Init( StPiUnicode_t *ai_poFrom ) 
{
	m_Handler	= ai_poFrom->m_Handler ;
	m_oIdPI.StrCpy( ai_poFrom->m_oIdPI ) ;
	m_NPage		= ai_poFrom->m_NPage ;
	m_NNumPage	= ai_poFrom->m_NNumPage ;
	m_X			= ai_poFrom->m_X ;
	m_Y			= ai_poFrom->m_Y ;
	m_oFont		= ai_poFrom->m_oFont;
	m_Color		= ai_poFrom->m_Color;
	m_NChars	= ai_poFrom->m_NChars ;
	m_oHyperlink.StrCpy( ai_poFrom->m_oHyperlink ) ;
}

//================================================================ClENG_PIUnicode_Manager

void ClENG_PIUnicode_Manager::End() 
{	
	PI_List_t::iterator it ;

	for (it=m_oPI_List.begin(); it!=m_oPI_List.end(); ++it)
	{
		if ((*it).second)
		{
			delete (*it).second;
			(*it).second = NULL;
		}
	}

	m_oPI_List.clear() ;
}

void ClENG_PIUnicode_Manager::Init( ) 
{
	ClENG_PIUnicode_Manager::End();

	m_HandlerAux=0 ;
}

StPiUnicode_t * ClENG_PIUnicode_Manager::AddNewPi(
	const char *ai_poIdPI, m4int16_t ai_iPage,  m4int_t ai_iNumPage, DvcUnit_t ai_iX,
	DvcUnit_t ai_iY, ClFONT_M_Font *ai_oFont, m4uint32_t ai_iColor,
	size_t ai_szNChar, const char *ai_pcHyperlink)
{
	StPiUnicode_t *Pi;
	PI_List_t::iterator it ;

	Pi = new StPiUnicode_t;

	Pi->m_Handler = m_HandlerAux ;
	Pi->m_oIdPI.StrCpy(ai_poIdPI) ;
	Pi->m_NPage = ai_iPage;
	Pi->m_NNumPage = ai_iNumPage;
	Pi->m_X = ai_iX;
	Pi->m_Y = ai_iY;
	Pi->m_oFont = ai_oFont;
	Pi->m_Color = ai_iColor;
	Pi->m_NChars = ai_szNChar;
	Pi->m_oHyperlink.StrCpy(ai_pcHyperlink);
	
	m_oPI_List.insert(PI_List_t::value_type(Pi->m_oIdPI,Pi));

	++m_HandlerAux ;

	return Pi ;
}

m4bool_t ClENG_PIUnicode_Manager::ExtractPi( const char *ai_pcIdPI, StPiUnicode_t *ao_poData ) 
{
	PI_List_t::iterator it;
	ClMIT_Str key ;
	
	key.StrCpy(ai_pcIdPI) ;

	it=m_oPI_List.find(key);
	if ( it != m_oPI_List.end() )
	{
		ao_poData->Init( (*it).second ) ;
		m_oPI_List.erase(it);

		return M4_TRUE;	

	}
	else
	{ 
		return M4_FALSE;
	} ;
 }


StPiUnicode_t * ClENG_PIUnicode_Manager::FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound) 
{
	PI_List_t::iterator it;
	ClMIT_Str key ;
	
	key.StrCpy(ai_pcIdPI) ;

	it=m_oPI_List.find(key);
	if (it == m_oPI_List.end() )
	{
	    if(ai_bSwErrorIfNotFound)
		{
		}
		return NULL ;        
	}
	else
	{	
		return (*it).second ;	
	}
}