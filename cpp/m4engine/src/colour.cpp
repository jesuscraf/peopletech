
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             colour.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/21/97
// Language:         C++
// Operating System: WINDOWS
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
#include "colour.h"
#include "engdf.inl"

#include "clstr.h"
#include "colour.hmg"

//==============================================================================ClENG_Colour

ClENG_Colour::ClENG_Colour() 
{
	m_poInsp=GET_INSP();
}

void ClENG_Colour::End() 
{
	m_oName.End() ;
}

void ClENG_Colour::Init( 
	m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
	m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B ) 
{
	ClENG_Colour::End();

	m_uiIdColour = ai_uiIdColour ;
	m_oName.StrCpy(ai_pcName) ;
	m_R = ai_R ; 
	m_G = ai_G ;
	m_B = ai_B ;
}

m4int16_t ClENG_Colour::GetReductionIndexTo256() 
{
	//Utiliza un algoritmo parecido a los modos OpenGL 3,3,2. 
	//Estructura del indice, de MSB a LSB: 2 bits blue, 2 bits green, 2 bits red

	return (m_B & 0x0C00) | ( (m_G & 0x0E0) >> 2  ) | ( (m_R & 0x0E0) >> 5 )  ; 	
}


//==============================================================================ClENG_Palette


void ClENG_Palette::End() {
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_OTHERS, m_poColour, ClENG_Colour, PALETTE_MAX_COLORS ) ;
	m_oColourIdIndex.clear() ;
	m_iColourSize=0 ;
}

ClENG_Palette::ClENG_Palette() {
	m_poInsp=GET_INSP();
	m_poColour=NULL ;
}


void ClENG_Palette::Init(ClENG_Palette *ai_poSource) 
{
	m4int16_t B ;

	m_poInsp			= ai_poSource->m_poInsp ;	
	m_iColourSize		= ai_poSource->m_iColourSize ;
	m_iRealColourSize	= ai_poSource->m_iRealColourSize ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_OTHERS, m_poColour, ClENG_Colour, PALETTE_MAX_COLORS ) ;
	for (B=0;B<GetSize();++B) {
		m_poColour[B].Init( 
			ai_poSource->m_poColour[B].GetId(), ai_poSource->m_poColour[B].GetName(),
			ai_poSource->m_poColour[B].GetR(), ai_poSource->m_poColour[B].GetG(), 
			ai_poSource->m_poColour[B].GetB() ) ;
	} ;

	m_oColourIdIndex = ai_poSource->m_oColourIdIndex ;	

}


m4bool_t ClENG_Palette::Init(
	m4int16_t ai_iSize,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE;
	m4int16_t B ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Palette::Init(), size %0:s", ClMIT_Msg::ENTRY_FORMAT) 
				<< ai_iSize << SEND_MSG ;
	};
	#endif

	ClENG_Palette::End();

	if ( ! M4_IN_RANGE( ai_iSize, 0, GetMaxSize() ) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162984, "The size %0:s is not valid for a colour palette. The valid range is from %1:s to %2:s", ClMIT_Msg::MIXED_OUT)
			<< ai_iSize << (m4int16_t)1 << GetMaxSize() << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_iColourSize=ai_iSize ;
	m_iRealColourSize=PALETTE_MAX_COLORS;
	if (!m_iColourSize) m_iColourSize=1 ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_OTHERS, m_poColour, ClENG_Colour, PALETTE_MAX_COLORS ) ;

	for (B=0;B<GetSize();++B) {
		m_poColour[B].Init(0,"Dummy colour",0,0,0) ;
	} ;

exit:
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClPalette::Init()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG ;
	};
	#endif

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;
}

m4bool_t ClENG_Palette::SetColour( 
	m4int16_t ai_PaletteIndex,
	m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
	m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B )
{
	m4bool_t SwOk=M4_TRUE ;
	ColourId_PaletteIndex_t::iterator It ;

	if ( ! M4_IN_RANGE( ai_PaletteIndex, 0, GetSize()-1 ) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162985, "Invalid index for a colour palette. The valid range is from %1:s to %2:s.", ClMIT_Msg::MIXED_OUT)
			<< ai_PaletteIndex << (m4int16_t)0 << (m4int16_t)(GetSize()-1) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	It=m_oColourIdIndex.find(ai_uiIdColour) ;
	if ( It != m_oColourIdIndex.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162986, "Invalid colour identifier: %0:s.", ClMIT_Msg::MIXED_OUT)
			<< ai_uiIdColour << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_poColour[ai_PaletteIndex].Init(ai_uiIdColour,ai_pcName,ai_R,ai_G,ai_B) ;
	m_oColourIdIndex.insert(ColourId_PaletteIndex_t::value_type(ai_uiIdColour,ai_PaletteIndex)) ;

exit:

	return SwOk ;
}


m4bool_t ClENG_Palette::AddColour( 
	m4int16_t ai_PaletteIndex,
	m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
	m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B )
{
	m4bool_t SwOk=M4_TRUE ;
	ColourId_PaletteIndex_t::iterator It ;

	if ( ai_PaletteIndex!=GetSize() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162985, "Invalid index for a colour palette. The valid range is from %1:s to %2:s.", ClMIT_Msg::MIXED_OUT)
			<< ai_PaletteIndex << (m4int16_t)0 << (m4int16_t)(GetSize()-1) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	It=m_oColourIdIndex.find(ai_uiIdColour) ;
	if ( It != m_oColourIdIndex.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162986, "Invalid colour identifier: %0:s.", ClMIT_Msg::MIXED_OUT)
			<< ai_uiIdColour << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_poColour[ai_PaletteIndex].Init(ai_uiIdColour,ai_pcName,ai_R,ai_G,ai_B) ;
	m_oColourIdIndex.insert(ColourId_PaletteIndex_t::value_type(ai_uiIdColour,ai_PaletteIndex)) ;

exit:

	return SwOk ;
}


m4int16_t ClENG_Palette::GetIndex(
	m4uint32_t ai_ColourId, 
	m4bool_t *ai_bSwFound, m4bool_t ai_bThrowError ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ColourId_PaletteIndex_t::iterator It ;

	It=m_oColourIdIndex.find(ai_ColourId) ;
	if ( It == m_oColourIdIndex.end() ) {
		if (ai_bThrowError) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162986, "El identificador de color %0:s no es válido.", ClMIT_Msg::MIXED_OUT)
				<< ai_ColourId << SEND_MSG ;
		} ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
exit:
	if (ai_bSwFound) *ai_bSwFound=SwOk ;
	return SwOk ?  (*It).second : 0 ;
}


ClENG_Colour * ClENG_Palette::GetColourById(
	m4uint32_t ai_ColourId,
	m4bool_t *ai_bSwFound, m4bool_t ai_bThrowError ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ColourId_PaletteIndex_t::iterator It ;

	It=m_oColourIdIndex.find(ai_ColourId) ;
	if ( It == m_oColourIdIndex.end() ) {
		if (ai_bThrowError) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162986, "Invalid colour identifier: %0:s.", ClMIT_Msg::MIXED_OUT)
				<< ai_ColourId << SEND_MSG ;
		} ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
exit:
	if (ai_bSwFound) *ai_bSwFound=SwOk ;
	return &m_poColour[ SwOk ? (*It).second : 0 ] ;
}


ClENG_Colour * ClENG_Palette::GetColourByIndex(
	m4int16_t ai_PaletteIndex,
	m4bool_t *ai_bSwFound, m4bool_t ai_bThrowError ) 
{
	m4bool_t SwOk=M4_TRUE ;

	
	if ( ! M4_IN_RANGE( ai_PaletteIndex, 0, GetSize()-1 ) ) {
		if (ai_bThrowError) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162985, "Invalid index for a colour palette. The valid range is from %1:s to %2:s.", ClMIT_Msg::MIXED_OUT)
				<< ai_PaletteIndex << (m4int16_t)0 << (m4int16_t)(GetSize()-1) << SEND_MSG ;
		} ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
exit:
	if (ai_bSwFound) *ai_bSwFound=SwOk ;
	return &m_poColour[ SwOk ? ai_PaletteIndex : 0 ] ;
}




