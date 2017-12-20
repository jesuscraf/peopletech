//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             pi.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"

#include "pi.h"

#define GROUP_ID    ENG_TGI_CLDVCFI

//================================================================StPi_t

void StPi_t::Init( StPi_t *ai_poFrom ) 
{
	m_Handler	= ai_poFrom->m_Handler ;
	m_oIdPI.StrCpy( ai_poFrom->m_oIdPI )  ;
	m_Offset	= ai_poFrom->m_Offset ; 
	m_NBytes	= ai_poFrom->m_NBytes ; 
	m_Place		= ai_poFrom->m_Place ;  
	m_NPage		= ai_poFrom->m_NPage ;   
	m_poFile	= ai_poFrom->m_poFile ;	
}

void StPi_t::ChangeToIN_FILE( ClENG_PhysicalFile *ai_poFile, m4uint32_t ai_Offset ) 
{
	m_Place = PI_IN_FILE ;
	m_poFile = ai_poFile ;	
	m_Offset = ai_Offset ;

}

//================================================================ClENG_PI_Manager

ClENG_PI_Manager::ClENG_PI_Manager()
{
	m_poInsp=GET_INSP();
}


void ClENG_PI_Manager::End() 
{	
	PI_List_t::iterator it ;

	for (it=m_oPI_List.begin(); it!=m_oPI_List.end(); ++it) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, (*it).second, StPi_t ) ;
	} ;

	m_oPI_List.clear() ;
}

void ClENG_PI_Manager::Init( ) 
{
	ClENG_PI_Manager::End();

	m_HandlerAux=0 ;
} 


StPi_t * ClENG_PI_Manager::AddNewPi(
	const char *ai_poIdPI, m4int16_t ai_iPage,  m4uint32_t ai_Offset, size_t ai_NBytes)
{
	StPi_t *Pi;
	PI_List_t::iterator it ;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, Pi, StPi_t ) ;


	Pi->m_Handler = m_HandlerAux ;
	Pi->m_oIdPI.StrCpy(ai_poIdPI) ;
	Pi->m_NBytes = ai_NBytes;
	Pi->m_NPage = ai_iPage;
	Pi->m_Place = StPi_t::PI_IN_RAM;   
	Pi->m_Offset = ai_Offset;
	Pi->m_poFile = NULL ;
	
	m_oPI_List.insert(PI_List_t::value_type(Pi->m_oIdPI,Pi));

	++m_HandlerAux ;

	return Pi ;
}

void ClENG_PI_Manager::MoveAllPiToFile(m4int16_t ai_iPage, ClENG_PhysicalFile *ai_File, m4uint32_t ai_FilePos)
{
	PI_List_t::iterator It;
	StPi_t *Pi;

	for (It= m_oPI_List.begin(); It!=m_oPI_List.end(); It++) {
		Pi = (*It).second ;
		if ( Pi->m_Place == StPi_t::PI_IN_RAM &&  Pi->m_NPage == ai_iPage )  {
			Pi->ChangeToIN_FILE(ai_File, Pi->m_Offset + ai_FilePos ) ;
		} ;
	} ;
}

m4bool_t ClENG_PI_Manager::ExtractPi( const char *ai_pcIdPI, StPi_t *ao_poData ) 
{
	PI_List_t::iterator it;
	ClMIT_Str key ;
	
	key.StrCpy(ai_pcIdPI) ;

	it=m_oPI_List.find(key);
	if ( it != m_oPI_List.end() ) {

		ao_poData->Init( (*it).second ) ;
		m_oPI_List.erase(it);

		return M4_TRUE;	

	} else  { 

		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162894, 
			"Pre-printing not found. Maybe you wrote a wrong identifier.", 
			ClMIT_Msg::MIXED_OUT) << SEND_MSG ;

		return M4_FALSE;
	} ;
 }


StPi_t * ClENG_PI_Manager::FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound) 
{
	PI_List_t::iterator it;
	ClMIT_Str key ;
	
	key.StrCpy(ai_pcIdPI) ;

	it=m_oPI_List.find(key);
	if (it == m_oPI_List.end() ) {	 
	    if(ai_bSwErrorIfNotFound) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162894, 
				"Pre-printing not found. Maybe you wrote a wrong identifier.", 
				ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		} ;
		return NULL ;        
	} else  {	
		return (*it).second ;	
	} ;
}


m4bool_t ClENG_PI_Manager::HasPendingPIs(ClENG_PhysicalFile *ai_File) 
{
	PI_List_t::iterator it ;
	StPi_t *pi ;

	for ( it=m_oPI_List.begin() ; it!=m_oPI_List.end(); ++it ) {
		pi = (*it).second ;
		if ( pi->m_Place==StPi_t::PI_IN_FILE ) {
			if ( pi->m_poFile==ai_File ) return M4_TRUE ;
		} ;
	} ;
	return M4_FALSE ;
}
