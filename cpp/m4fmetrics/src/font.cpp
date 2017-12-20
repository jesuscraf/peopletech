
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             font.cpp
// Project:			 fmetrics.dll
// Author:           Meta Software M.S. , S.A
// Date:			 7/21/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano Perez (18/06/98) - FileStorage
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4string.hpp"

#include "fontm.h"
#include "font.h"

#include "fontmdf.cpp"

#include "font.hmg"
#include "rfnfiler.h"

#include "m4unicode.hpp"


//==============================================================================ClFONT_M_Font

ClFONT_M_Font::ClFONT_M_Font() 
{
	m_poInsp=GET_INSP();
	Width=NULL ;
	m_piTrans=NULL ;
	m_piPSC_AFM_Widths=NULL;
}

void ClFONT_M_Font::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, Width, DvcUnit_t, SIZE_WIDTH_ARRAY ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, m_piTrans, m4int16_t, 
		ENG_TRANS_ARRAY_SIZE ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, m_piPSC_AFM_Widths, m4int16_t, 
		ENG_PSC_AFM_WIDTHS_ARRAY_SIZE ) ;
	m_oLocation.Init(0,"?");
	m_oFRN_File.End();	
}



m4bool_t ClFONT_M_Font::Init( 
		LOAD_INFO ai_LoadInfo,
		ClMIT_IdNumDesc *ai_poIdDesc, const m4char_t *ai_pcRFNFileName,
		Point_t ai_MapHeight, 
		m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, 
		m4bool_t ai_bBold, m4bool_t ai_bItalic,
		m4int32_t * ao_SoftFontOffset, m4int32_t *ao_SoftFontSize,
		ClMIT_Str *ao_poSofFontFile,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{

	m4int16_t B ;
	m4bool_t SwOk=M4_TRUE ;
	ClRFN_RFN_R Read ;
	m4int16_t Trans[ENG_TRANS_ARRAY_SIZE] ;
	m4int16_t PSC_AFM_W[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE] ;

	ClFONT_M_Font::End();

	m_oFRN_File.StrCpy(ai_pcRFNFileName);	
	m_MapHeight = ai_MapHeight ;

	m_oDesc.StrCpy(ai_poIdDesc->GetDesc()) ;
	m_oLocation.Init( ai_poIdDesc->GetIdNum(), m_oDesc ) ;
	m_SoftFontIndex=-1 ;

	if (!ai_bFixedSpace) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, Width, DvcUnit_t, SIZE_WIDTH_ARRAY ) ;
	} ;

	//Valores dummy por si hay problemas al cargar RVW

	m_stRVW.m_oFaceName.StrCpy("?") ;
	m_stRVW.m_Height = 10 ;
	m_stRVW.m_Pitch = 2 ;

	//Cuidado: Si existe la info de RVW, se cargan sus datos siempre
	SwOk = Read.ReadFont( 	
		ai_pcRFNFileName, ai_MapHeight,
		ai_bFixedSpace,ai_bBold,ai_bItalic,ai_bUnderline, 
		&m_oFaceName, 
		&m_DesignHeight, &m_DesignMaxWidth, &m_Height, &m_MaxWidth,
		Width, 	ai_LoadInfo, m_bImplemented, 
		&m_stPCL, &m_stPSC, &m_stESC, &m_stRVW,
		ao_poSofFontFile, ao_SoftFontOffset, ao_SoftFontSize, Trans, PSC_AFM_W,
		ClMIT_Trace::PROPAGATE_ERROR) ;

	if (!SwOk) goto exit ;

	m_uiIdFont=ai_poIdDesc->GetIdNum() ;

	m_oFlag.Switch(FIXED_SPACE, ai_bFixedSpace);
	m_oFlag.Switch(UNDERLINE, ai_bUnderline);
	m_oFlag.Switch(BOLD, ai_bBold);
	m_oFlag.Switch(ITALIC, ai_bItalic);
	m_oFlag.Switch(SOFT_FONT, M4_BOOL( *ao_SoftFontSize!=0 ));
	if ( Trans[ENG_TRANS_TYPE_POS] != ENG_SOFT_TRANS_TYPE_EMPTY ) 
	{
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, m_piTrans, m4int16_t, 
			ENG_TRANS_ARRAY_SIZE ) ;
		for (B=0; B<ENG_TRANS_ARRAY_SIZE; ++ B) {
			m_piTrans[B] = Trans[B] ;
		} ;
	} ;
	if ( m_oFlag.True(SOFT_FONT) && ai_LoadInfo==PSC_INFO ) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, m_piPSC_AFM_Widths, m4int16_t, 
			ENG_PSC_AFM_WIDTHS_ARRAY_SIZE ) ;
		for (B=0; B<ENG_PSC_AFM_WIDTHS_ARRAY_SIZE; ++ B) {
			m_piPSC_AFM_Widths[B] = PSC_AFM_W[B] ;
		} ;
	} ;

	

exit:

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		

}

DvcUnit_t ClFONT_M_Font::GetDvcLenStr(size_t ai_szNumChar) {
	return ai_szNumChar*GetDvcMaxWidthChar();
}

DvcUnit_t ClFONT_M_Font::GetDvcLenStr(m4pchar_t ai_pcStr, size_t ai_szNumChar){
	DvcUnit_t R ;
	m4pchar_t pcStrANSI ;
	m4pchar_t pcStrAux ;
	int iNumCharANSI ;

	// AntonioADN. Bug 164100
	// Aproximación para el cálculo correcto de longitudes en entorno Unicode.
	// Aseguramos que se calculan el número de caracteres adecuado. Si un caracter no es respresentable
	// en código de página (ANSI) se toma la longitud de ?
	iNumCharANSI = ai_szNumChar ;
	pcStrANSI = M4CppToANSI( ai_pcStr, iNumCharANSI ) ;
	pcStrAux = pcStrANSI ;

	if ( IsFixedSpace() ) {

		R= iNumCharANSI*GetDvcMaxWidthChar() ;

	} else {

		R=0 ;
		for ( ;iNumCharANSI; --iNumCharANSI, ++pcStrAux ) {
			R += Width[*(m4uint8_t *)pcStrAux] ;
		} ;
	} ;

	delete [] pcStrANSI ;

	return R ;
}

DvcUnit_t ClFONT_M_Font::GetDvcLenStr(m4pchar_t ai_pcStr){
	return GetDvcLenStr(ai_pcStr, strlen(ai_pcStr) );
} ;


DvcUnit_t ClFONT_M_Font::GetDvcWidthChar( m4char_t ai_cChar){
	
	return IsFixedSpace() ? GetDvcMaxWidthChar() : Width[(m4uint8_t)ai_cChar] ;

}

size_t ClFONT_M_Font::TrimRightStr(m4pchar_t ai_pcStr, DvcUnit_t ai_UnitLen){
	size_t Len, Conta ;
	m4pchar_t pcStrANSI ;
	m4pchar_t pcStrAux ;
	int iNumCharANSI ;

	iNumCharANSI = strlen( ai_pcStr ) ;
	
	if ( IsFixedSpace() ) {
		Len=ai_UnitLen/GetDvcMaxWidthChar() ;
		Len=M4_MIN(iNumCharANSI,Len);
	} else {
		// AntonioADN. Bug 164100
		// Aproximación para el cálculo correcto de longitudes en entorno Unicode.
		// Aseguramos que se calculan el número de caracteres adecuado. Si un caracter no es respresentable
		// en código de página (ANSI) se toma la longitud de ?
		pcStrANSI = M4CppToANSI( ai_pcStr, iNumCharANSI ) ;
		pcStrAux = pcStrANSI ;

		Conta=iNumCharANSI ;
		for ( Len=0, pcStrAux +=Conta-1; ai_UnitLen>0 && Conta ; --pcStrAux, ++Len, --Conta ) {
			ai_UnitLen -=  Width[ *(m4uint8_t *)pcStrAux ] ;
		} ;
		if (ai_UnitLen<0) --Len ;

		delete [] pcStrANSI ;
	} ;

	return Len ;
}

size_t ClFONT_M_Font::TrimLeftStr(m4pchar_t ai_pcStr, DvcUnit_t ai_UnitLen){
	size_t Len ;
	m4pchar_t pcStrANSI ;
	m4pchar_t pcStrAux ;
	int iNumCharANSI ;

	iNumCharANSI = strlen( ai_pcStr ) ;
	
	if ( IsFixedSpace() ) {
		Len=ai_UnitLen/GetDvcMaxWidthChar() ;
		Len=M4_MIN(iNumCharANSI,Len);
	} else {
		// AntonioADN. Bug 164100
		// Aproximación para el cálculo correcto de longitudes en entorno Unicode.
		// Aseguramos que se calculan el número de caracteres adecuado. Si un caracter no es respresentable
		// en código de página (ANSI) se toma la longitud de ?
		pcStrANSI = M4CppToANSI( ai_pcStr, iNumCharANSI ) ;
		pcStrAux = pcStrANSI ;

		for ( Len=0; ai_UnitLen>0 && *pcStrAux ; ++pcStrAux, ++Len ) {
			ai_UnitLen -=  Width[ *(m4uint8_t *)pcStrAux ]  ;
		} ;
		if (ai_UnitLen<0) --Len ;

		delete [] pcStrANSI ;
	} ;

	return Len ;
}

ENG_SOFT_TRANS_TYPE ClFONT_M_Font::GetTranslationType() 
{
	return m_piTrans ? (ENG_SOFT_TRANS_TYPE)m_piTrans[ENG_TRANS_TYPE_POS] : 
						ENG_SOFT_TRANS_TYPE_EMPTY ;
}

m4char_t ClFONT_M_Font::GetTranslation(m4int16_t ai_Char) 
{
	if (m_piTrans) {
		if ( M4_IN_RANGE(ai_Char,0, ENG_TRANS_ARRAY_SIZE-1) ) {
			return (m4uchar_t)m_piTrans[ai_Char] ;
		} else {
			return ai_Char ;
		} ;
	} else {
		return ai_Char ;
	} ;

}

m4int16_t ClFONT_M_Font::GetAFM_Width(m4int16_t ai_Pos) 
{
	if (!m_piPSC_AFM_Widths || ai_Pos<0 || ai_Pos>=ENG_PSC_AFM_WIDTHS_ARRAY_SIZE ) {
		return 0 ;
	} else {
		return m_piPSC_AFM_Widths[ai_Pos] ;
	} ;
}



//==============================================================================ClFONT_M_DBFont


void ClFONT_M_DBFont::End(){

	FontList_t::iterator It ;

	for ( It=m_oFontList.begin(); It!=m_oFontList.end(); ++It ) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, (*It).second, ClFONT_M_Font) ;
	};
	
	m_oFontList.clear();
	m_oIndexOrder.clear() ;
	m_poMonoFont=NULL ;
	m_oSoftFonts.clear() ;
}

ClFONT_M_DBFont::ClFONT_M_DBFont(void){
	m_poInsp = GET_INSP();
}

void ClFONT_M_DBFont::Init(){

	ClFONT_M_DBFont::End();
	
	SetMultiFontMode() ;
}

m4bool_t ClFONT_M_DBFont::SaveFont( 
		ClFONT_M_Font::LOAD_INFO ai_LoadInfo,
		ClMIT_IdNumDesc *ai_poIdDesc, const m4char_t *ai_pcRFNFileName,
		Point_t ai_MapHeight,
		m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, 
		m4bool_t ai_bBold, m4bool_t ai_bItalic,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	ClFONT_M_Font* poFont=NULL;
	FontList_t::iterator FoundIt ;
	m4uint32_t Key;
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str File, Path ;
	SoftFontList_t::iterator SFIt ;
	StSoftFontData SFD ;


	M4_NEW_ITEM(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, poFont, ClFONT_M_Font) ;
	
	SwOk = M4_BOOL( SwOk && M4_VIRTUAL_OBJECT(poFont,ClFONT_M_Font)->
			Init( 
				ai_LoadInfo, ai_poIdDesc, ai_pcRFNFileName, 
				ai_MapHeight,ai_bFixedSpace, ai_bUnderline, 
				ai_bBold, ai_bItalic, 
				&SFD.m_Offset, &SFD.m_Size, &SFD.m_oSofFontFile,
				ClMIT_Trace::PROPAGATE_ERROR) );

	if (!SwOk) goto exit ;

	Key=ai_poIdDesc->GetIdNum();
	FoundIt = m_oFontList.find(Key);
	if (FoundIt == m_oFontList.end() ) {
		m_oFontList.insert(FontList_t::value_type(Key, poFont) );
		m_oIndexOrder.insert( FontInsertOrder_t::value_type(m_oIndexOrder.size(),poFont) ) ;
	} else {
		SwOk=M4_FALSE;
		goto exit;
	};

	if (SFD.m_Size) 
	{
		SFIt = m_oSoftFonts.find(SFD.m_oSofFontFile) ;
		if ( SFIt==m_oSoftFonts.end() )
		{
			SFD.m_oPathName.StrCpy(ai_pcRFNFileName) ;
			SFD.m_poFont = poFont ;
			SFD.m_Index = m_oSoftFonts.size() ;
			m_oSoftFonts.insert(SoftFontList_t::value_type(SFD.m_oSofFontFile,SFD)) ;
			SFIt = m_oSoftFonts.find(SFD.m_oSofFontFile) ;
		} ;
		if ( ai_LoadInfo==ClFONT_M_Font::PCL_INFO ) {
			poFont->SetPCLSoftFontTypeFace( (*SFIt).second.m_Index ) ;
		} ;
		poFont->SetSoftFontIndex( (*SFIt).second.m_Index ) ;
	} ;

exit:
	
	if (!SwOk && poFont) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, FONT_M_MGI_FONT, poFont, ClFONT_M_Font) ;
	} ;

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}


void ClFONT_M_DBFont::SetMonoFontMode(ClFONT_M_Font*  ai_poFont) 
{ 
	if (! ai_poFont ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FONT_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_poMonoFont=ai_poFont ; 
}




ClFONT_M_Font * ClFONT_M_DBFont::GetAddressFontByIndex(m4int16_t ai_iIndexFont,m4bool_t ai_bSwOblig)
{
	if ( m_poMonoFont ) {
		return m_poMonoFont ;
	} else {

		FontInsertOrder_t::iterator It ;

		It = m_oIndexOrder.find( ai_iIndexFont ) ;
		if ( It==m_oIndexOrder.end() && ai_bSwOblig ) 
		{
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FONT_MSG_20 ] ) << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;
		} ;
		return It==m_oIndexOrder.end() ? NULL : (*It).second ;
	} ;
}

ClFONT_M_Font * ClFONT_M_DBFont::GetAddressFontById(m4uint32_t ai_uiIdFont,m4bool_t ai_bSwOblig)
{
	if ( m_poMonoFont ) {
		return m_poMonoFont ;
	} else {

		FontList_t::iterator It ;

		It = m_oFontList.find( ai_uiIdFont ) ;
		if ( It==m_oFontList.end() && ai_bSwOblig ) 
		{
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FONT_MSG_20 ] ) << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;
		} ;
		return It==m_oFontList.end() ? NULL : (*It).second ;
	} ;

}

ClFONT_M_Font * ClFONT_M_DBFont::GetAddressSmallestFont()
{
	if ( m_poMonoFont ) {

		return m_poMonoFont ;

	} else {

		FontList_t::iterator It ;
		DvcUnit_t  Min=0 ;
		ClFONT_M_Font *R=NULL ; 

		for ( It = m_oFontList.begin() ;It!=m_oFontList.end() ; ++It ) 
		{
			if ( (*It).second->GetDvcDesignHeight() < Min || !R ) {
				R=(*It).second ;
				Min=R->GetDvcDesignHeight() ;
			} ;

		} ;
		return R ;
	} ;
}

ClFONT_M_Font * ClFONT_M_DBFont::GetAddressThinestFont() 
{
	if ( m_poMonoFont ) {

		return m_poMonoFont ;

	} else {

		FontList_t::iterator It ;
		DvcUnit_t  Min=0 ;
		ClFONT_M_Font *R=NULL ; 

		for ( It = m_oFontList.begin() ;It!=m_oFontList.end() ; ++It ) 
		{
			if ( (*It).second->GetDvcDesignMaxWidthChar() < Min || !R ) {
				R=(*It).second ;
				Min=R->GetDvcDesignMaxWidthChar() ;
			} ;

		} ;
		return R ;
	} ;
}


m4int16_t ClFONT_M_DBFont::FindIndexFont( m4uint32_t ai_uiIdFont, m4bool_t ai_bSwOblig ) 
{
	FontInsertOrder_t::iterator It ;
	m4int16_t Index=-1 ;

	for ( It=m_oIndexOrder.begin(); It!=m_oIndexOrder.end(); ++It ) {
		if ( (*It).second->GetId()==ai_uiIdFont ) {
			Index=(*It).first ;
			break ;
		} ;

	} ;
	
	if (Index==-1 && ai_bSwOblig) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FONT_MSG_30 ] ) 
			<< ai_uiIdFont << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	return Index ;
}


ClFONT_M_DBFont::StSoftFontData * ClFONT_M_DBFont::GetSoftFontDataByIndex(m4int16_t ai_iIndexFont,m4bool_t ai_bSwOblig)
{
	StSoftFontData * R=NULL ;
	SoftFontList_t::iterator It ;

	for ( It=m_oSoftFonts.begin(); It != m_oSoftFonts.end(); ++It ) {
		if ( (*It).second.m_Index==ai_iIndexFont) {
			break ;
		} ;
	} ;

	if ( It==m_oSoftFonts.end() && ai_bSwOblig ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FONT_MSG_40 ] ) 
			<< ai_iIndexFont << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	return It==m_oSoftFonts.end() ? NULL : &(*It).second ; 
}

m4bool_t ClFONT_M_DBFont::Init( 
		ClFONT_M_DBFont *ai_poFrom, ClFONT_M_Font::LOAD_INFO ai_LoadInfo,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClFONT_M_Font *Font ;

	ClFONT_M_DBFont::End() ;

	for (B=0; B < ai_poFrom->GetNumFonts(); ++B ) {
		Font = ai_poFrom->GetAddressFontByIndex(B) ;
		SwOk = M4_BOOL( SwOk && SaveFont( ai_LoadInfo,
			Font->GetIdNumDesc(), 
			Font->GetRFN_FileName(),Font->GetMapHeight(),
			Font->IsFixedSpace(), Font->IsUnderline(), Font->IsBold(), Font->IsItalic(),
			ClMIT_Trace::PROPAGATE_ERROR ) );
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}


