//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             region.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/18/97
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

#include "designdt.h"

#include "region.h"

#define GROUP_ID    ENG_TGI_FIELD

//==============================================================================ClENG_Line

ClENG_Line::ClENG_Line() 
{
	m_poInsp=GET_INSP();
	m_poDvc =NULL;
}

void ClENG_Line::End() 
{
	m_poDvc =NULL;
}

void ClENG_Line::Init(
	ClENG_OutDevice * ai_poDvc,
	DvcUnit_t ai_Top, DvcUnit_t  ai_Left, DvcUnit_t ai_Top2, DvcUnit_t ai_Left2,
	m4bool_t ai_bAdjustLineToContainer,
	DvcUnit_t ai_Width, ClENG_OutDevice::STYLE ai_Style, m4uint32_t ai_uiIdColour, 
	m4bool_t ai_bSwVisible )
{
	ClENG_Line::End() ;

	m_poDvc = ai_poDvc ;
	m_Top = ai_Top ;
	m_Left = ai_Left ;
	m_Top2 = ai_Top2 ;
	m_Left2 = ai_Left2 ;
	m_Width = ai_Width ;
	m_Style = ai_Style ;
	m_uiIdColour = ai_uiIdColour ;
	m_bSwVisible = ai_bSwVisible ;
	m_bAdjustLineToContainer = ai_bAdjustLineToContainer ;
}

void ClENG_Line::Init( ClENG_Line *ai_poSource ) 
{
	Init( 
		ai_poSource->m_poDvc,
		ai_poSource->GetTop(), ai_poSource->GetLeft(), 
		ai_poSource->GetTop2(), ai_poSource->GetLeft2(),
		ai_poSource->GetAdjustLineToContainer(),
		ai_poSource->GetWidth(), ai_poSource->GetStyle(), ai_poSource->GetIdColour(), 
		ai_poSource->IsVisible() ) ;
}


void ClENG_Line::Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) 
{
	if ( IsVisible() ) {

		m_poDvc->SelectColour(ai_iNumPage, m_uiIdColour); 
		m_poDvc->PrintLine(	ai_iNumPage, ai_XBase+m_Left, ai_YBase+m_Top, 
			ai_XBase+m_Left2, ai_YBase+m_Top2, m_Width, m_Style ) ;
	} ;
}

void ClENG_Line::Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) 
{
	m4double_t PendX, K ;
	m4bool_t Inf, SwShow ;
	DvcUnit_t X1,Y1,X2,Y2 ;	//Puntos de la linea ordenados de < a > por eje Y
	DvcUnit_t YLim1, YLim2 ;	//Limites Y de impresion


	if ( IsVisible() ) {

		//Ordeno puntos por eje Y, para que el signo de la pendiente sea correcto
		//Opero sobre coordenadas ya reales  

		if (m_Top<m_Top2) {
			Y1=m_Top ; X1=m_Left; Y2=m_Top2; X2=m_Left2 ;
		} else {
			Y2=m_Top ; X2=m_Left; Y1=m_Top2; X1=m_Left2 ;
		} ;
		Y1+=ai_YBase - ai_HeightPrinted  ; Y2+=ai_YBase - ai_HeightPrinted ; X1+=ai_XBase ;X2+=ai_XBase ;

		//Hayo f(x)=PendX*x+k. 

		if ( Y1==Y2 ) {
			Inf=M4_TRUE ;		//Pendiente infinita, linea horizontal
		} else {
			Inf=M4_FALSE ;
			PendX=(X2-X1)*1.0/(Y2-Y1) ;	
			K = X1-PendX*Y1 ;					
		} ;

		//Recalculo puntos teniendo en cuenta la intersección

		YLim1=ai_YBase ;
		YLim2=ai_YBase+ai_HeightToPrint-1 ;
		if (Inf) {
			SwShow = M4_BOOL(M4_IN_RANGE(Y1,YLim1,YLim2));
		} else {
			SwShow = M4_BOOL(! ( Y1>YLim2 || Y2<YLim1 ));
			if (SwShow) {
				if (Y1<YLim1) {
					Y1=YLim1 ;
					X1=Y1*PendX+K ;
				} ;
				if (Y2>YLim2) {
					Y2=YLim2 ;
					X2=Y2*PendX+K ;
				} ;
			} ;
		} ;
		if (SwShow) {

			m_poDvc->SelectColour(ai_iNumPage, m_uiIdColour); 
			m_poDvc->PrintLine(	ai_iNumPage, X1, Y1, X2, Y2, m_Width, m_Style ) ;
		} ;
	} ;

}

m4bool_t ClENG_Line::SetLine( m4char_t *ai_pcLineProps ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClMIT_Str StrAux ;

	if ( ClMIT_Str::StrLenTrim(ai_pcLineProps)==0 ) {
		SetVisible(M4_FALSE) ;
		goto exit ;
	} 

	if ( ClMIT_Str::StrLenTrim(ai_pcLineProps)!=GetSizeStrLine() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	StrAux.ToASCIZ( &ai_pcLineProps[0],4) ;
	SetWidth( ClMetric::DPointToDvc( atol(StrAux)/10.0 ) ) ;

	StrAux.ToASCIZ( &ai_pcLineProps[4],2) ;
	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestLineStyle((ClENG_OutDevice::STYLE)atol(StrAux)) ) ;
	if (!SwOk) goto exit ;
	SetStyle( (ClENG_OutDevice::STYLE)atol(StrAux) ) ;

	StrAux.ToASCIZ( &ai_pcLineProps[6],1) ;
	SetVisible( M4_BOOL(atol(StrAux)==1) ) ;

	StrAux.ToASCIZ( &ai_pcLineProps[7],5) ;
	SetIdColour( atol(StrAux)) ;

exit:
	if (!SwOk) {
		/*##Cambiar num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Sintax error in SetLine: parameter %0:s", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcLineProps << SEND_MSG ;
	} ;

	return SwOk ;
}

//==============================================================================ClENG_Region

ClENG_Region::ClENG_Region() 
{
	m_poInsp=GET_INSP();
	m_poDvc =NULL;
}

void ClENG_Region::End() 
{
	m_poDvc =NULL;
}

void ClENG_Region::Init( 
	ClENG_OutDevice * ai_poDvc,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	m4char_t *ai_pcBordeProps, m4uint32_t ai_uiFillColour
	)
{
	m4int16_t B ;

	ClENG_Region::End() ;

	m_poDvc = ai_poDvc ;
	m_Top = ai_Top ;
	m_Left = ai_Left ;
	m_Width = ai_Width ;
	m_Height = ai_Height ;
	m_uiFillColour = ai_uiFillColour ;

	for (B=0; B<LINE_COUNT; ++B ) m_Line[B].Init(m_poDvc,0,0,0,0,M4_FALSE,0,ClENG_OutDevice::SOLID,0,M4_FALSE) ;

	AdjustFrameAndFillZone() ;

	SetFrame(ai_pcBordeProps) ;
}

void ClENG_Region::Init( ClENG_Region *ai_poSource ) 
{
	m4int16_t B ;

	Init( 
		ai_poSource->m_poDvc,
		ai_poSource->GetTop(), ai_poSource->GetLeft(), ai_poSource->GetWidth(), 
		ai_poSource->GetHeight(), "", ai_poSource->GetFillColour() ) ;
	
	for (B=0;B<LINE_COUNT;++B) m_Line[B].Init( &ai_poSource->m_Line[B] ) ;
	
}


void ClENG_Region::LineIndex(LINE_ID ai_Line, m4int16_t *ao_piBeg,m4int16_t *ao_piEnd) 
{
	if (ai_Line==FRAME) {
		*ao_piBeg=0 ; *ao_piEnd=LINE_COUNT-1 ;
	} else {
		*ao_piBeg=ai_Line; *ao_piEnd=ai_Line;
	} ;
}

void ClENG_Region::SetStyle( LINE_ID ai_Line, ClENG_OutDevice::STYLE ai_Style) 
{
	m4int16_t B, Beg, End ;

	LineIndex(ai_Line,&Beg,&End) ;
	for (B=Beg;B<=End;++B) m_Line[B].SetStyle(ai_Style) ;
}

void ClENG_Region::SetIdColour( LINE_ID ai_Line, m4uint32_t ai_uiIdColour) 
{
	m4int16_t B, Beg, End ;

	LineIndex(ai_Line,&Beg,&End) ;
	for (B=Beg;B<=End;++B) m_Line[B].SetIdColour(ai_uiIdColour) ;
}

void ClENG_Region::SetVisible(  LINE_ID ai_Line, m4bool_t ai_bSw) 
{
	m4int16_t B, Beg, End ;

	LineIndex(ai_Line,&Beg,&End) ;
	for (B=Beg;B<=End;++B) m_Line[B].SetVisible(ai_bSw) ;
}

void ClENG_Region::SetWidth(    LINE_ID ai_Line, DvcUnit_t ai_Width) 
{
	m4int16_t B, Beg, End ;

	LineIndex(ai_Line,&Beg,&End) ;
	for (B=Beg;B<=End;++B) m_Line[B].SetWidth(ai_Width) ;
	AdjustFrameAndFillZone() ;
}

void ClENG_Region::SetTop(DvcUnit_t ai_Top) 
{
	m_Top=ai_Top ;
	AdjustFrameAndFillZone() ;
}

void ClENG_Region::SetLeft(DvcUnit_t ai_Left) 
{
	m_Left=ai_Left ;
	AdjustFrameAndFillZone() ;
}

void ClENG_Region::SetWidth(DvcUnit_t ai_Width) 
{
	m_Width=ai_Width ;
	AdjustFrameAndFillZone() ;
}

void ClENG_Region::SetHeight(DvcUnit_t ai_Height) 
{
	m_Height=ai_Height ;
	AdjustFrameAndFillZone() ;
}


m4bool_t ClENG_Region::IsHomogeneousFrame() 
{
	m4bool_t SwRet=M4_TRUE ;
	m4int16_t B ;

	for (B=0;B<LINE_COUNT && SwRet ; ++B) {
		if ( ! m_Line[B].IsVisible() ) {
			SwRet=M4_FALSE ;
		} else {
			if ( 
				m_Line[B].GetStyle() != m_Line[0].GetStyle() ||  
				m_Line[B].GetIdColour() != m_Line[0].GetIdColour() ||  
				m_Line[B].GetWidth() != m_Line[0].GetWidth() 
				) 
			{
				SwRet=M4_FALSE ;
			}
		} ;
	} ;
	
	return SwRet ;
}

void ClENG_Region::AdjustFrameAndFillZone() 
{
	DvcUnit_t Int[LINE_COUNT],Ext[LINE_COUNT] ;	
	m4int16_t B ;
	
	for (B=0; B<LINE_COUNT;++B) {
		if ( m_Line[B].IsVisible() ) {
			Int[B] =  m_Line[B].GetWidth()/2 ;
			Ext[B] =  m_Line[B].GetWidth()-Int[B]*2 ;
		} else {
			Ext[B] = Int[B] = 0 ;
		} ;

	} ;

	m_Line[TOP_LINE].SetTop(   m_Top ) ;
	m_Line[TOP_LINE].SetLeft(  m_Left-Ext[LEFT_LINE]) ;
	m_Line[TOP_LINE].SetTop2(  m_Top ) ;
	m_Line[TOP_LINE].SetLeft2( m_Left+m_Width+Ext[RIGHT_LINE] ) ;

	m_Line[BOTTOM_LINE].SetTop(   m_Top+m_Height-1 ) ;
	m_Line[BOTTOM_LINE].SetLeft(  m_Line[TOP_LINE].GetLeft() ) ;
	m_Line[BOTTOM_LINE].SetTop2(  m_Top+m_Height-1 ) ;
	m_Line[BOTTOM_LINE].SetLeft2( m_Line[TOP_LINE].GetLeft2() ) ;

	m_Line[LEFT_LINE].SetTop(   m_Top-Ext[TOP_LINE] ) ;
	m_Line[LEFT_LINE].SetLeft(  m_Left ) ;
	m_Line[LEFT_LINE].SetTop2(  m_Top+m_Height-1+Ext[BOTTOM_LINE]  ) ;
	m_Line[LEFT_LINE].SetLeft2( m_Left ) ;

	m_Line[RIGHT_LINE].SetTop(   m_Top-Ext[TOP_LINE]  ) ;
	m_Line[RIGHT_LINE].SetLeft(  m_Left+m_Width-1 ) ;
	m_Line[RIGHT_LINE].SetTop2(  m_Top+m_Height-1+Ext[BOTTOM_LINE] ) ;
	m_Line[RIGHT_LINE].SetLeft2( m_Left+m_Width-1 ) ;

	m_FillTop=m_Top+Int[TOP_LINE] ;
	m_FillLeft=m_Left+Int[LEFT_LINE] ;
	m_FillWidth=m_Width-Int[LEFT_LINE]-Int[RIGHT_LINE] ;
	m_FillHeight=m_Height-Int[TOP_LINE]-Int[BOTTOM_LINE] ;
	if( m_FillWidth<=0 || m_FillHeight<=0 ) {
		m_FillTop=m_FillLeft=m_FillWidth=m_FillHeight=0;
	} ;
}

void ClENG_Region::Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) 
{
	m_poDvc->ShowRegion(this,ai_iNumPage,ai_XBase,ai_YBase) ;
}

void ClENG_Region::Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
						DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) 
{
	m_poDvc->ShowRegion(this,ai_iNumPage,ai_XBase,ai_YBase,ai_HeightPrinted,ai_HeightToPrint) ;
}

void ClENG_Region::ShowOnDvc( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) 
{
	m4int16_t B ;

	if ( m_uiFillColour!=ClENG_Colour::ID_TRANSPARENT && m_FillWidth>0) {

		m_poDvc->SelectColour(ai_iNumPage, m_uiFillColour ); 
		m_poDvc->FillZone( 
			ai_iNumPage, ai_XBase+m_FillLeft, ai_YBase+m_FillTop, m_FillWidth, m_FillHeight ) ;
	} ;

	if ( IsHomogeneousFrame() ) {
		m_poDvc->SelectColour(ai_iNumPage, m_Line[0].GetIdColour() ); 
		m_poDvc->PrintFrame( 
			ai_iNumPage, ai_XBase+m_Left, ai_YBase+m_Top, m_Width, m_Height,
			m_Line[0].GetWidth(), m_Line[0].GetStyle() ) ;

	} else {
		for (B=0;B<LINE_COUNT;++B) m_Line[B].Show(ai_iNumPage,ai_XBase,ai_YBase);
	} ;
}

void ClENG_Region::ShowOnDvc( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
						DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) 
{
	m4int16_t B ;

	if ( ai_HeightPrinted==0 && ai_HeightToPrint>=m_Height ) {
		ShowOnDvc( ai_iNumPage, ai_XBase, ai_YBase ) ;
		return ;
	} ;

	if ( m_uiFillColour!=ClENG_Colour::ID_TRANSPARENT && m_FillWidth>0) {
		m_poDvc->SelectColour(ai_iNumPage, m_uiFillColour ); 
		m_poDvc->FillZone( 
			ai_iNumPage, ai_XBase+m_FillLeft, 
			ai_HeightPrinted==0 ? ai_YBase+m_FillTop : ai_YBase, 
			m_FillWidth, ai_HeightToPrint ) ;

	} ;

	for (B=0;B<LINE_COUNT;++B) m_Line[B].Show(
		ai_iNumPage,ai_XBase,ai_YBase,ai_HeightPrinted,ai_HeightToPrint);
}

m4bool_t ClENG_Region::SetFrame( m4char_t *ai_pcBordeProps ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClMIT_Str StrAux ;

	if ( ClMIT_Str::StrLenTrim(ai_pcBordeProps)==0 ) {
		SetVisible(FRAME,M4_FALSE) ;
		goto exit ;
	} 

	if ( ClMIT_Str::StrLenTrim(ai_pcBordeProps)!= ClENG_Line::GetSizeStrLine()*4 ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	for ( B=0; B<LINE_COUNT; ++B ) {

		StrAux.ToASCIZ( &ai_pcBordeProps[
			ClENG_Line::GetSizeStrLine()*B], ClENG_Line::GetSizeStrLine() ) ;
		if ( ! m_Line[ (LINE_ID)B ].SetLine( StrAux ) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} ;
exit:
	if (!SwOk) {
		/*##Cambiar num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Sintax error in SetFrame: parameter %0:s", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcBordeProps << SEND_MSG ;
	} ;

	return SwOk ;
}

