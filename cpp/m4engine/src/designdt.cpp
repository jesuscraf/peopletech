
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designdt.cpp
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
#include "engdf.inl"

#include "sysinfo.h"
#include "clargs.h"
#include "vbserial.h"

#include "designdt.h"

#include "loader.hmg"
//#include "designdthmg"


//============================================================================================

#define ENG_DT_OBLIG( Vble, VbleS, FlagS, TxtVble) SwOk = M4_BOOL(SwOk&&ClENG_DesignTree::TestOblig(ai_poSource->FlagS,TxtVble)); Vble=ai_poSource->VbleS 
#define ENG_DT_OPT(   Vble, VbleS, FlagS, Default)    Vble= (ai_poSource->FlagS) ? ai_poSource->VbleS : Default

#define ENG_DT_OBLIG_STR( Vble, VbleS, FlagS, TxtVble) SwOk=M4_BOOL(SwOk&&ClENG_DesignTree::TestOblig(ai_poSource->FlagS,TxtVble)); Vble.StrCpy(ai_poSource->VbleS) 
#define ENG_DT_OPT_STR(   Vble, VbleS, FlagS, Default)    Vble.StrCpy( ai_poSource->FlagS ? m4pchar_t(ai_poSource->VbleS) : Default ) 

#define ENG_DT_OBLIG_DVC( Vble, VbleS, FlagS, TxtVble) SwOk=M4_BOOL(SwOk&&ClENG_DesignTree::TestOblig(ai_poSource->FlagS,TxtVble)); Vble=ai_poDT->GetDvcUnit(ai_poSource->VbleS) 
#define ENG_DT_OPT_DVC(   Vble, VbleS, FlagS, DefaultDvc)    Vble= (ai_poSource->FlagS) ? ai_poDT->GetDvcUnit(ai_poSource->VbleS) : DefaultDvc 

#define ENG_DT_COPY_FROM_DD(   Vble, VbleFlag )    m_stFromValues.VbleFlag = ai_poSource->VbleFlag ; if (m_stFromValues.VbleFlag) m_stFromValues.Vble = ai_poSource->Vble ;
#define ENG_DT_COPY_FROM_DD_2(   Vble, VbleFlag )    m_stFromValues.VbleFlag = ai_poSource->m_stFromValues.VbleFlag ; if (m_stFromValues.VbleFlag) m_stFromValues.Vble = ai_poSource->m_stFromValues.Vble ;


//======================================================================StENG_DT_Defaults


StENG_DT_Defaults::StENG_DT_Defaults()
{
	m_poInsp=GET_INSP();
}

void StENG_DT_Defaults::End()
{
	m_oNULLDef.End() ;
	m_oNULLFormatDef.End() ;
	m_oDefDisplayNULL.End() ;
}

m4bool_t StENG_DT_Defaults::ResetGlobalDefaults(ClENG_DesignTree *ai_poDT, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t SwOk = M4_TRUE;
	ClFONT_M_Font *Font ;

	StENG_DT_Defaults::End() ;

	m_bIdFontInherit = m_bIdFormatInherit = m_bAlignYInherit = m_bAlignXInherit =
		m_bClipInherit = m_bGraphicsInherit = m_bIdColourInherit = m_bSwFormatNULLInherit =
		m_bDefDisplayNULLInherit = M4_FALSE ;

	Font= ai_poDT->m_oDBFont.GetAddressFontByIndex(0,M4_FALSE) ;	//Primera font
	if (!Font) {
		SwOk = M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"DBFont doesn't have fonts", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
	} else {
		m_uiIdFont = Font->GetId() ;
		m_FontDesignHeight			= Font->GetDvcDesignHeight() ;
		m_FontDesignMaxWidthChar	= Font->GetDvcDesignMaxWidthChar() ;
	} ;

	m_uiIdFormat = ClENG_Format::ID_NULL ;

	m_AlignY= m_AlignX = ClENG_FieldStr::LEFT ;
	m_bSwClip =	M4_FALSE ;
	m_bSwGraphics = M4_FALSE ;

	m_bSwFormatNULL = M4_FALSE ;
	m_uiIdColour=0 ; 		

	m_oDefDisplayNULL.StrCpy("") ;

	m_oNULLDef.m_oStr[ClMIT_ChnValue::NUMERIC].StrCpy("") ;
	m_oNULLDef.m_oStr[ClMIT_ChnValue::STRING].StrCpy("") ;
	m_oNULLDef.m_oStr[ClMIT_ChnValue::DATE].StrCpy("") ;
	m_oNULLDef.m_oStr[ClMIT_ChnValue::BOOLEAN].StrCpy("") ;
	m_oNULLDef.m_oStr[ClMIT_ChnValue::CURRENCY].StrCpy("") ;
	m_oNULLDef.m_oStr[ClMIT_ChnValue::NO_INIT].StrCpy("") ;

	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::NUMERIC].StrCpy("") ;
	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::STRING].StrCpy("") ;
	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::DATE].StrCpy("    \\  \\  ") ;
	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::BOOLEAN].StrCpy("") ;
	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::CURRENCY].StrCpy("") ;
	m_oNULLFormatDef.m_oStr[ClMIT_ChnValue::NO_INIT].StrCpy("") ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk;
}

void StENG_DT_Defaults::CopyFrom(StENG_DT_Defaults *ai_poFrom) 
{
	StENG_DT_Defaults::End() ;

	m_uiIdFont					= ai_poFrom->m_uiIdFont ;
	m_FontDesignHeight			= ai_poFrom->m_FontDesignHeight ;
	m_FontDesignMaxWidthChar	= ai_poFrom->m_FontDesignMaxWidthChar ;
	m_uiIdFormat				= ai_poFrom->m_uiIdFormat ;
	m_AlignY					= ai_poFrom->m_AlignY ;
	m_AlignX					= ai_poFrom->m_AlignX ;
	m_bSwClip					= ai_poFrom->m_bSwClip ;
	m_bSwGraphics				= ai_poFrom->m_bSwGraphics ;
	m_bSwFormatNULL				= ai_poFrom->m_bSwFormatNULL ;
	m_uiIdColour				= ai_poFrom->m_uiIdColour ;
	m_bIdFontInherit			= ai_poFrom->m_bIdFontInherit ;			
	m_bIdFormatInherit			= ai_poFrom->m_bIdFormatInherit ; 
	m_bAlignYInherit			= ai_poFrom->m_bAlignYInherit ; 
	m_bAlignXInherit			= ai_poFrom->m_bAlignXInherit ;
	m_bClipInherit				= ai_poFrom->m_bClipInherit ; 
	m_bGraphicsInherit			= ai_poFrom->m_bGraphicsInherit ; 
	m_bIdColourInherit			= ai_poFrom->m_bIdColourInherit ; 
	m_bSwFormatNULLInherit		= ai_poFrom->m_bSwFormatNULLInherit ;
	m_bDefDisplayNULLInherit	= ai_poFrom->m_bDefDisplayNULLInherit  ;

	m_oDefDisplayNULL.StrCpy(ai_poFrom->m_oDefDisplayNULL) ;
	m_oNULLDef.Init(&ai_poFrom->m_oNULLDef) ;
	m_oNULLFormatDef.Init(&ai_poFrom->m_oNULLFormatDef) ;

}

void StENG_DT_Defaults::InheritAllFrom(StENG_DT_Defaults *ai_poFrom) 
{
	StENG_DT_Defaults::End() ;

	m_bIdFontInherit = m_bIdFormatInherit = m_bAlignYInherit = m_bAlignXInherit =
		m_bClipInherit = m_bGraphicsInherit = m_bIdColourInherit = m_bSwFormatNULLInherit =
		m_bDefDisplayNULLInherit = M4_TRUE ;

	m_uiIdFont					= ai_poFrom->m_uiIdFont ;
	m_FontDesignHeight			= ai_poFrom->m_FontDesignHeight ;
	m_FontDesignMaxWidthChar	= ai_poFrom->m_FontDesignMaxWidthChar ;
	m_uiIdFormat				= ai_poFrom->m_uiIdFormat ;
	m_AlignY					= ai_poFrom->m_AlignY ;
	m_AlignX					= ai_poFrom->m_AlignX ;
	m_bSwClip					= ai_poFrom->m_bSwClip ;
	m_bSwGraphics				= ai_poFrom->m_bSwGraphics ;
	m_bSwFormatNULL				= ai_poFrom->m_bSwFormatNULL ;
	m_uiIdColour				= ai_poFrom->m_uiIdColour ;

	m_oDefDisplayNULL.StrCpy(ai_poFrom->m_oDefDisplayNULL) ;
	m_oNULLDef.Init(&ai_poFrom->m_oNULLDef) ;
	m_oNULLFormatDef.Init(&ai_poFrom->m_oNULLFormatDef) ;
}



m4bool_t StENG_DT_Defaults::InheritReset( 
	StENG_DD_Defaults *ai_poSource, ClENG_DesignTree *ai_poDT, m4bool_t ai_bSwInherit,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return=M4_TRUE ;
	ClFONT_M_Font *Font ;

	if (ai_poSource->m_SwIdFontLoaded) {
		m_bIdFontInherit			= ai_bSwInherit;
		m_uiIdFont					= ai_poSource->m_uiIdFont ;
		Font= ai_poDT->m_oDBFont.GetAddressFontById(m_uiIdFont,M4_FALSE) ;	//Primera font
		if (!Font) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_10 ] ) 
				<< m_uiIdFont << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} else {
			m_FontDesignHeight			= Font->GetDvcDesignHeight() ;
			m_FontDesignMaxWidthChar	= Font->GetDvcDesignMaxWidthChar() ;
		} ;
	} ;

	if (ai_poSource->m_SwIdFormatLoaded) {
		m_bIdFormatInherit			= ai_bSwInherit;
		m_uiIdFormat				= ai_poSource->m_uiIdFormat  ;
	} ;

	if (ai_poSource->m_SwAlignYLoaded) {
		m_bAlignYInherit	= ai_bSwInherit;
		m_AlignY			= ai_poSource->m_AlignY ;
		Return=ClENG_DesignTree::TestFieldAlign(m_AlignY) ;
		if (!Return) goto exit ;
	} ;

	if (ai_poSource->m_SwAlignXLoaded) {
		m_bAlignXInherit	= ai_bSwInherit;
		m_AlignX			=ai_poSource->m_AlignX ;
		Return=ClENG_DesignTree::TestFieldAlign(m_AlignX) ;
		if (!Return) goto exit ;
	} ;

	if (ai_poSource->m_SwClipLoaded) {
		m_bClipInherit	= ai_bSwInherit;
		m_bSwClip		=ai_poSource->m_bSwClip ; 
	} ;

	if (ai_poSource->m_SwIdColourLoaded) {
		m_bIdColourInherit	= ai_bSwInherit;
		m_uiIdColour		= ai_poSource->m_uiIdColour ; 
	} ;

	if (ai_poSource->m_SwGraphicsLoaded) {
		m_bGraphicsInherit	= ai_bSwInherit;
		m_bSwGraphics		= ai_poSource->m_bSwGraphics ; 
	} ;

	if (ai_poSource->m_SwFormatNULLLoaded) {
		m_bSwFormatNULLInherit	= ai_bSwInherit;
		m_bSwFormatNULL			= ai_poSource->m_bSwFormatNULL ; 
	} ;

	if (ai_poSource->m_SwDefDisplayNULLLoaded) {
		m_bDefDisplayNULLInherit	= ai_bSwInherit;
		m_oDefDisplayNULL.StrCpy(ai_poSource->m_oDefDisplayNULL) ;
		Return=DecodeDisplayNULL(ai_poSource->m_oDefDisplayNULL) ;
		if (!Return) goto exit ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return Return ;
}


m4bool_t StENG_DT_Defaults::DecodeDisplayNULL(m4char_t *ai_pcDisplayNULL ) 
{
	ClMIT_Args Args ;
	m4int16_t B ;
	m4bool_t SwOk=M4_TRUE ;
	const m4char_t *Pos ;
	ClMIT_Str *Str ;
	ClMIT_ChnValue::TYPE Type ;

	Args.Init(ai_pcDisplayNULL,(m4char_t)255,", \t") ;

	for (B=0;B<Args.GetArgCount();++B) {
		Pos=Args.GetArg(B) ;

		switch (*Pos) {
		case 'N': case 'n':
			Type=ClMIT_ChnValue::NUMERIC ;
			break;
		case 'S': case 's':
			Type=ClMIT_ChnValue::STRING ;
			break;
		case 'D': case 'd':
			Type=ClMIT_ChnValue::DATE ;
			break;
		case 'B': case 'b':
			Type=ClMIT_ChnValue::BOOLEAN ;
			break;
		case 'C': case 'c':
			Type=ClMIT_ChnValue::CURRENCY ;
			break;
		default:
			SwOk=M4_FALSE ;
			goto exit ;
			break;
		} ;
		++Pos ;

		if (*Pos=='F' || *Pos=='f' ) {
			++Pos ;
			Str=&m_oNULLFormatDef.m_oStr[Type] ;
		} else {
			Str=&m_oNULLDef.m_oStr[Type] ;
		} ;

		if (*Pos!=':') goto exit ;
		else ++Pos ;

		Str->StrCpy( Pos ) ;

	} ;
	
exit:
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_40 ] ) 
			<< ai_pcDisplayNULL << SEND_MSG ;
	} ;
	return SwOk ;
}

void StENG_DT_Defaults::IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) 
{
	if ( aoi_puiFontCount ) {
		if ( ! m_bIdFontInherit && ai_uiIdFont==m_uiIdFont ) ++(*aoi_puiFontCount) ;
	} ;
	if ( aoi_puiFormatCount ) {
		if ( ! m_bIdFormatInherit && ai_uiIdFormat==m_uiIdFormat ) ++(*aoi_puiFormatCount) ;
	} ;
	if ( aoi_puiColourCount ) {
		if ( ! m_bIdColourInherit && ai_uiIdColour==m_uiIdColour ) ++(*aoi_puiColourCount) ;
	} ;
}

//======================================================================StENG_DT_GlobalReport

m4bool_t StENG_DT_GlobalReport::Init( 
	StENG_DD_GlobalReport *ai_poSource, m4bool_t ai_bGlobalLoaded,
	ClENG_DesignTree *ai_poDT) 
{
	m4bool_t SwOk=M4_TRUE ;
	StMIT_Inspector* Insp = GET_INSP() ;


	if ( ! ai_bGlobalLoaded ) {
		SwOk=M4_FALSE;
		*Insp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Global Report Data not loaded", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		goto exit;
	} ;


	ENG_DT_OPT_STR(   m_oReportName,	m_oReportName,		m_bReportNameLoaded,	"?"  ) ;
	ENG_DT_OBLIG_STR( m_oDataChn,		m_oDataChn,			m_bDataChnLoaded,		"DataChn") ; 
	ENG_DT_OPT_STR(	  m_oDataChnReprinter,m_oDataChnReprinter,m_bDataChnReprinterLoaded, "") ; 
	ENG_DT_OPT(       m_ModeMetric,		m_ModeMetric,		m_bModeMetricLoaded,	ENG_MILIM  ) ;
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestReportModeMetric(m_ModeMetric));
	ENG_DT_OBLIG(     m_iIdRootSection,	 m_iIdRootSection,  m_bIdRootSectionLoaded,	"IdRootSection") ; 
	ENG_DT_OPT(       m_bSwRootSync,	m_bSwRootSync,		m_bSwRootSyncLoaded,	M4_TRUE  ) ;
	ENG_DT_OPT(       m_BreakMode,		m_BreakMode,		m_bBreakModeLoaded,		ClENG_Report::NO_BREAK  ) ;
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestReportBreakMode(m_BreakMode));
	ENG_DT_OPT_STR(   m_oTotalizeList,	m_oTotalizeList,	m_bTotalizeListLoaded,	""  ) ;
	ENG_DT_OPT_STR(   m_oCTList,		m_oCTList,			m_bCTListLoaded,		""  ) ;
	ENG_DT_OPT_STR(   m_oExecutionPars,	m_oExecutionPars,	m_bExecutionParsLoaded,	""  ) ;
	ENG_DT_OPT(       m_bIgnoreTxtWarning, m_bIgnoreTxtWarning,m_bIgnoreTxtWarningLoaded,	M4_FALSE  ) ;

	ENG_DT_OPT(       m_iIdBreakSection, m_iIdBreakSection,	m_bIdBreakSectionLoaded,	0  ) ;
	if (m_BreakMode != ClENG_Report::NO_BREAK) {
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig(ai_poSource->m_bIdBreakSectionLoaded,"IdBreakSection"));
	} ;
	if ( m_BreakMode != ClENG_Report::NO_BREAK && !m_iIdBreakSection ) {
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_30 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
	} ;
	
	m_stDefaultFileParams.Init( &ai_poSource->m_stDefaultFileParams ) ; 

	SwOk = M4_BOOL(SwOk && m_oReportDefaults.ResetGlobalDefaults( ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));
	SwOk = M4_BOOL(SwOk && m_oReportDefaults.UpdateFrom( 
		&ai_poSource->m_oReportDefaults, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));

	if ( m_iIdRootSection==ENG_SECION_ID_EMPTY ) {
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162994, "Error en diseño de report, debe haber por lo menos una sección.", 
			ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		SwOk=M4_FALSE ;
	} ;

	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(
		&m_stDefaultFileParams.m_oPath,	&ai_poDT->m_oLE));


exit:
	if (!SwOk) {
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_210 ] ) << SEND_MSG ;
	} ;

	return SwOk ;
}

//======================================================================Datos Font

m4bool_t ClENG_DesignTree::TestFontData( StENG_DD_FontData *ai_poThis, StENG_DD_FontData *ai_poSource  ) 
{
	m4bool_t SwOk=M4_TRUE ;

	
	ENG_DT_OBLIG(     ai_poThis->m_uiIdFont,	 m_uiIdFont,		m_bIdFontLoaded,	"IdFont") ; 
	ENG_DT_OBLIG(     ai_poThis->m_DesignHeight, m_DesignHeight,	m_bDesignHeightLoaded, "Font DesignHeight") ; 
	ENG_DT_OBLIG(     ai_poThis->m_bFixedSpace,	 m_bFixedSpace,		m_bFixedSpaceLoaded, "Font FixedSpace") ; 
	ENG_DT_OBLIG_STR( ai_poThis->m_oRFNFile,	 m_oRFNFile,		m_bRFNFileLoaded, "Font RFNFile") ; 
	ENG_DT_OBLIG_STR( ai_poThis->m_oTraceName,	 m_oTraceName,		m_bTraceNameLoaded, "Font TraceName") ; 
	ENG_DT_OPT(		  ai_poThis->m_bUnderline,	 m_bUnderline,		m_bUnderlineLoaded, M4_FALSE) ; 
	ENG_DT_OPT(		  ai_poThis->m_bBold,		 m_bBold,			m_bBoldLoaded, M4_FALSE) ; 
	ENG_DT_OPT(		  ai_poThis->m_bItalic,		 m_bItalic,			m_bItalicLoaded, M4_FALSE) ; 	

	return SwOk ;
}

//======================================================================Datos Colour

m4bool_t ClENG_DesignTree::TestColourData( StENG_DD_Colour *ai_poThis, StENG_DD_Colour *ai_poSource  ) 
{
	m4bool_t SwOk=M4_TRUE ;

	ENG_DT_OBLIG(     ai_poThis->m_uiIdColour,	 m_uiIdColour,		m_bIdColourLoaded,	"IdColour") ; 
	ENG_DT_OPT_STR(	  ai_poThis->m_oName,		 m_oName,			m_bNameLoaded,		"Colour Name") ; 
	ENG_DT_OBLIG(     ai_poThis->m_R,			 m_R,		        m_b_R_Loaded,		"Colour, R component") ; 
	ENG_DT_OBLIG(     ai_poThis->m_G,			 m_G,		        m_b_G_Loaded,		"Colour, G component") ; 
	ENG_DT_OBLIG(     ai_poThis->m_B,			 m_B,		        m_b_B_Loaded,		"Colour, B component") ; 

	return SwOk ;
}


//======================================================================StENG_DT_FormatData

m4bool_t StENG_DT_FormatData::Init( StENG_DD_FormatData *ai_poSource) 
{
	m4bool_t SwOk=M4_TRUE ;
	StMIT_Inspector* Insp = GET_INSP() ;

	m_oProperties.clear() ;

	ENG_DT_OBLIG(     m_uiIdFormat,		m_uiIdFormat,		m_bIdFormatLoaded,	 "IdFormat") ; 
	ENG_DT_OBLIG_STR( m_oFormatName,	m_oFormatName,		m_bFormatNameLoaded, "Format Name") ; 
	ENG_DT_OPT_STR(	  m_oIdCurrency,	m_oIdCurrency,		m_bIdCurrencyLoaded, "") ; 

	m_oProperties = ai_poSource->m_oProperties ;

	return SwOk ;
}

//======================================================================StENG_DT_PageData

m4bool_t StENG_DT_PageData::Init( StENG_DD_PageData *ai_poSource,ClENG_DesignTree *ai_poDT) 
{
	m4bool_t SwOk=M4_TRUE ;
	StMIT_Inspector* Insp = GET_INSP() ;
	m4int16_t Page ;
	ClENG_PagePrinter PageAux ;	//Para calculo ancho/largo

	ENG_DT_OBLIG(	Page,			m_iPage,		m_bPageLoaded,	 "Page Number (0-n)") ; 
	ENG_DT_OPT_DVC(	m_OffsetX,		m_dOffsetX,		m_bOffsetXLoaded,	 0 ) ; 
	ENG_DT_OPT_DVC(	m_OffsetY,		m_dOffsetY,		m_bOffsetYLoaded,	 0 ) ; 
	ENG_DT_OPT(		m_PageSize,		m_PageSize,		m_bPageSizeLoaded,	 ClENG_PagePrinter::A4 ) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestPagePaperSize(m_PageSize));
	ENG_DT_OPT(		m_Orientation,	m_Orientation,	m_bOrientationLoaded, ClENG_PagePrinter::PORTRAIT ) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestPageOrientation(m_Orientation));

	ENG_DT_OPT_DVC(		m_SizeX,	m_dSizeX,	m_bSizeXLoaded, 0 ) ; 
	ENG_DT_OPT_DVC(		m_SizeY,	m_dSizeY,	m_bSizeYLoaded, 0 ) ; 
	if ( m_PageSize == ClENG_PagePrinter::PERSONAL ) {
		SwOk=M4_BOOL(SwOk && ClENG_DesignTree::TestOblig(ai_poSource->m_bSizeXLoaded,"Page SizeX") && 
			         ClENG_DesignTree::TestOblig(ai_poSource->m_bSizeYLoaded,"Page SizeY"));
	} ;

	if (SwOk) {
		PageAux.Init( m_Orientation, m_PageSize, ClENG_PagePrinter::PRINTER_DEFAULT_BIN, m_SizeX, m_SizeY ) ;
		m_SizeX = PageAux.GetSizeX() ;
		m_SizeY = PageAux.GetSizeY() ;
	} ;

	m_stDefaultData.InheritAllFrom( &ai_poDT->m_oGlobal.m_oReportDefaults ) ;
	SwOk = M4_BOOL(SwOk && m_stDefaultData.UpdateFrom( 
		           &ai_poSource->m_stDefaultData, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));

	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bCntLoaded, "Page container" ));

	return SwOk ;
}

void StENG_DT_PageData::IncrCountFontFormatColourInstances( 
	m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
	m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
	m4uint32_t *aoi_puiColourCount ) 
{
	//Def de pag

	m_stDefaultData.IncrCountFontFormatColourInstances(
		ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

	//Cnt de pag layout (def+fields)

	m_oCnt.IncrCountFontFormatColourInstances(
		ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;
}


//======================================================================StENG_DT_CTData

m4bool_t StENG_DT_CTData::Init( StENG_DD_CTData *ai_poSource, ClENG_DesignTree *ai_poDT ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Args Arg ;
	StMIT_Inspector* Insp = GET_INSP() ;
	//StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	
	ENG_DT_OBLIG_STR(	m_oCTId,		m_oCTId,		m_bCTIdLoaded,		"CTId") ; 
	ENG_DT_OPT_STR(		m_oName,		m_oName,		m_bNameLoaded,		"") ; 
	ENG_DT_OBLIG_STR(	m_oTotList,		m_oTotList,		m_bTotListLoaded,	"TotList") ; 
	ENG_DT_OBLIG_STR(	m_oTreeList,	m_oTreeList,	m_bTreeListLoaded,	"TreeList") ; 
	ENG_DT_OBLIG_STR(	m_oFillTree,	m_oFillTree,	m_bFillTreeLoaded,	"FillTree") ; 
	ENG_DT_OBLIG(		m_uiIdParamSec,	m_uiIdParamSec,	m_bIdParamSecLoaded,"IdParamSec") ; 
	ENG_DT_OPT(			m_MergePage,	m_MergePage,	m_bMergePageLoaded,  ENG_CT_ALL_PAGES) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestMergePage(m_MergePage));
	ENG_DT_OPT(			m_iTogLevel,	m_iTogLevel,	m_bTogLevelLoaded,  0) ; 
	ENG_DT_OPT_STR(		m_oWizardData,	m_oWizardData,	m_bWizardDataLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oDerivedList,	m_oDerivedList,	m_bDerivedListLoaded,"") ; 
	ENG_DT_OBLIG(		m_iFirstCrossRow,m_iFirstCrossRow,	m_bFirstCrossRowLoaded,"FirstCrossRow") ; 

	ENG_DT_OPT_DVC(		m_XColumnLeft,	m_dXColumnLeft,	m_bXColumnLeftLoaded, ClMetric::MilimToDvc(15) ) ; 
	ENG_DT_OPT_DVC(		m_XColumnRight,	m_dXColumnRight,m_bXColumnRightLoaded, 
		ai_poDT->m_oPageData[0].m_SizeX - ClMetric::MilimToDvc(15) ) ; 

	m_bIsModified = ai_poSource->m_bIsModified;

	m_iBreakLevel = 0 ;

	Arg.Init( m_oFillTree, '.', " ," ) ; 
	m_iLevelCount=Arg.GetArgCount() ;
	if ( m_iLevelCount<=0 ) {
		/*##Cambiar NUMERO de error! */
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, /*4325472, */
			"Un Crosstab debe tener al menos un nivel", ClMIT_Msg::MIXED_OUT) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_oTemplate.End() ;

	m_oParamSec = ai_poSource->m_oParamSec ;

	if ( m_oParamSec.size() != m_iLevelCount ) {
		/*##Cambiar NUMERO de error! */
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, /*4325472, */
			"Un Crosstab debe tener tantas secciones de parametrizacion como niveles", ClMIT_Msg::MIXED_OUT) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( *(m_oParamSec.begin()) != m_uiIdParamSec ) {
		/*##Cambiar NUMERO de error! */
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, /*4325472, */
			"Crosstab con datos inconsistentes, el primer elemento de m_oParamSec no es "
			"igual a m_uiIdParamSec", ClMIT_Msg::MIXED_OUT) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
exit:
	return SwOk ;
}

m4bool_t StENG_DT_CTData::LoadTemplate( StENG_DD_CTData *ai_poSource, ClENG_DesignTree *ai_poDT ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SwOk = M4_BOOL( SwOk && m_oTemplate.Init( this, ai_poSource, ai_poDT,
		ClMIT_Trace::PROPAGATE_ERROR ) ) ;

	return SwOk ;
}


void StENG_DT_CTData::IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) 
{
	m_oTemplate.IncrCountFontFormatColourInstances( 
		ai_uiIdFont, ai_uiIdFormat, ai_uiIdColour, aoi_puiFontCount, aoi_puiFormatCount, aoi_puiColourCount ) ;
}


//======================================================================StENG_DT_CTCell

m4bool_t StENG_DT_CTCell::Init( StENG_DD_CTCell *ai_poSource, ClENG_DesignTree *ai_poDT) 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_DD_FieldList_t::iterator FieldIt ; 
	StENG_DT_FieldData Field ;

	ENG_DT_OBLIG(		m_uiIdParamSec ,	m_uiIdParamSec ,	m_bIdParamSecLoaded, "IdParamSec") ; 
	ENG_DT_OBLIG(		m_Type ,			m_Type ,			m_bTypeLoaded,		"Cell Type") ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestCellType(m_Type));
	ENG_DT_OBLIG(		m_iLevel,			m_iLevel,			m_bLevelLoaded,		"Cell Level") ; 

	if (!ai_poDT) goto exit ;

	ENG_DT_OBLIG_DVC( m_Height, m_dHeight, m_bHeightLoaded, "Height") ;
	ENG_DT_OBLIG_DVC( m_Width,  m_dWidth,  m_bWidthLoaded,  "Width") ;

	ENG_DT_OPT_STR(	m_oRegionBorder,	m_oRegionBorder,	m_bRegionBorderLoaded,	"" ) ; 
	ENG_DT_OPT(		m_uiRegionColour,	m_uiRegionColour,	m_bRegionColourLoaded,	ClENG_Colour::ID_TRANSPARENT ) ; 

	ENG_DT_OPT(		m_AlignParamCellY,	m_AlignParamCellY,	m_bAlignParamCellYLoaded,	ENG_CT_TOP_DISPLAY  ) ; 
	ENG_DT_OPT(		m_AlignParamCellX,	m_AlignParamCellX,	m_bAlignParamCellXLoaded,	ENG_CT_LEFT_DISPLAY ) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestAlignParamCellY(m_AlignParamCellY));
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestAlignParamCellX(m_AlignParamCellX));
	
	//Carga de Fields

	m_oFieldList.clear() ;

	for ( FieldIt = ai_poSource->m_oFieldList.begin() ; 
		FieldIt != ai_poSource->m_oFieldList.end() ; ++FieldIt  )
	{
		SwOk = M4_BOOL(SwOk && Field.Init( &(*FieldIt), ENG_CT_CELL, 
			&ai_poDT->m_oGlobal.m_oReportDefaults, ai_poDT ) );
		if (!SwOk) break ;

		m_oFieldList.push_back(Field) ;
	} ;

	m_bLoaded = SwOk ;
	
exit:
	return SwOk ;
}

StENG_DT_FieldData	* StENG_DT_CTCell::FindIdCTField( 
	ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField, ENG_DT_FieldList_t::iterator *ao_It ) 
{
	StENG_DT_FieldData	*R ;
	ENG_DT_FieldList_t::iterator It ;

	for ( It=m_oFieldList.begin(), R=NULL; It!=m_oFieldList.end(); ++It ) {
		if ( ClMIT_Str::StrICmpTrim( (*It).m_oIdCtField,ai_pcIdCtField)==0 ) {
			R = &(*It) ;
			break ;
		} ;
	} ;

	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Field whith IdCT %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Field whith IdCT %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

void StENG_DT_CTCell::IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) 
{
	ENG_DT_FieldList_t::iterator Field ;
	m4uint32_t IdColour;
	ClMIT_Str StrAux ;
	m4int16_t B;

	//Def de los fields del cnt

	for ( Field=m_oFieldList.begin(); Field!=m_oFieldList.end(); ++Field )  {
		(*Field).m_stDefaultData.IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount,aoi_puiColourCount) ;

		if ( aoi_puiColourCount ) {
			if ( (*Field).m_uiRegionColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==(*Field).m_uiRegionColour ) ++(*aoi_puiColourCount) ;

			if((*Field).m_oRegionBorder.StrLen()>0) {
				for(B=0; B<4; B++) {
					StrAux.ToASCIZ((*Field).m_oRegionBorder.InChar(7+(12*B)),5);
					IdColour = atol(StrAux);
					if ( IdColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==IdColour ) ++(*aoi_puiColourCount) ;
				};
			};
		} ;
	} ;

	if ( aoi_puiColourCount ) {
		if ( m_uiRegionColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==m_uiRegionColour ) ++(*aoi_puiColourCount) ;

		if(m_oRegionBorder.StrLen()>0) {
			for(B=0; B<4; B++) {
				StrAux.ToASCIZ(m_oRegionBorder.InChar(7+(12*B)),5);
				IdColour = atol(StrAux);
				if ( IdColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==IdColour ) ++(*aoi_puiColourCount) ;
			};
		};
	} ;
}

//======================================================================StENG_DT_SonSectionData

m4bool_t StENG_DT_SonSectionData::Init( StENG_DD_SonSectionData *ai_poSource ) 
{
	m4bool_t SwOk=M4_TRUE ;

	ENG_DT_OBLIG( m_uiIdSonSection,		m_uiIdSonSection,	m_bIdSonSectionLoaded, "IdSonSection") ; 
	ENG_DT_OPT(   m_iExeOrder,			m_iExeOrder,		m_bExeOrderLoaded,  1 ) ; 
	ENG_DT_OPT(   m_bIsSync ,			m_bIsSync ,			m_bIsSyncLoaded,	M4_TRUE ) ; 
	m_oSection.End() ;

	return SwOk ;
}



//======================================================================StENG_DT_CntData

m4bool_t StENG_DT_CntData::Init( 
	StENG_DD_CntData *ai_poSource, ENG_FIELD_OWNER ai_Owner,
	StENG_DD_Defaults *ai_poSectionDef, 
	ClENG_DesignTree *ai_poDT ) 
{
	m4bool_t SwOk=M4_TRUE ;

	StMIT_Inspector* Insp = GET_INSP() ;
	m4double_t TopAux, LeftAux, HeightAux, WidthAux ;
	ENG_DD_FieldList_t::iterator FieldIt ; 
	StENG_DT_FieldData Field ;

	// Test m_iPage 

	if ( ai_poDT->m_iNumPageLayout>1 ) {
		ENG_DT_OBLIG( m_iPage,		m_iPage,	m_bPageLoaded, "Page") ; 
	} else {
		m_iPage=0 ;
	} ;

	if ( !M4_IN_RANGE(m_iPage, 0, ai_poDT->m_iNumPageLayout-1) ) {
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_RANGE ] ) 
				<< "m_iPage" << m_iPage << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;		//No sigue porque el valor se usa despues como indice
	} ;

	//Copia valores DD 

	ENG_DT_COPY_FROM_DD(m_dTop,m_bTopLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dLeft,m_bLeftLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dHeight,m_bHeightLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dWidth,m_bWidthLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dLowerLimit,m_bLowerLimitLoaded) ;

	// Debo calcular primero defectos: se usan los valores de font luego.

	if ( ai_Owner != ENG_PAGE_CONTAINER ) {
		m_stDefaultData.InheritAllFrom( & ai_poDT->m_oPageData[m_iPage].m_stDefaultData ) ;
		SwOk = M4_BOOL(SwOk && m_stDefaultData.InheritFrom( 
					   ai_poSectionDef, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));
		SwOk = M4_BOOL(SwOk && m_stDefaultData.UpdateFrom( 
			           &ai_poSource->m_stDefaultData, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));
	} else {
		m_stDefaultData.InheritAllFrom( & ai_poDT->m_oPageData[m_iPage].m_stDefaultData ) ;
		SwOk = M4_BOOL(SwOk && m_stDefaultData.UpdateFrom( 
					   &ai_poSource->m_stDefaultData, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));
	} ;

	// Ahora, a calcular, con medidas de fonts disponibles

	if ( ai_Owner != ENG_PAGE_CONTAINER ) {
	
		ENG_DT_OBLIG( TopAux,			m_dTop,			m_bTopLoaded,	"Top") ; 
		ENG_DT_OBLIG( LeftAux,			m_dLeft,		m_bLeftLoaded,	"Left") ; 
		ENG_DT_OBLIG( HeightAux,		m_dHeight,		m_bHeightLoaded,"Height") ; 
		ENG_DT_OBLIG( WidthAux,			m_dWidth,		m_bWidthLoaded,	"Width") ; 

		m_Top   = TopAux < 0 ?  
			(-TopAux)*m_stDefaultData.GetFontDesignHeight() : 
			ai_poDT->GetDvcUnit(TopAux) ;
		m_Left   = LeftAux < 0 ?  
			(-LeftAux)*m_stDefaultData.GetFontDesignWidth() : 
			ai_poDT->GetDvcUnit(LeftAux) ;
		m_Height  = HeightAux < 0 ?  
			(-HeightAux)*m_stDefaultData.GetFontDesignHeight() : 
			ai_poDT->GetDvcUnit(HeightAux) ;
		m_Width  = WidthAux < 0 ?  
			(-WidthAux)*m_stDefaultData.GetFontDesignWidth() : 
			ai_poDT->GetDvcUnit(WidthAux) ;

		ENG_DT_OBLIG(	  m_IsFixed,		m_IsFixed,		m_bIsFixedLoaded,"IsFixed") ; 
		ENG_DT_OPT(		  m_bSwTogether,	m_bSwTogether,	m_bSwTogetherLoaded, M4_FALSE) ; 
		ENG_DT_OPT_STR(	  m_oCancelCnt,		m_oCancelCnt,	m_bCancelCntLoaded,"") ; 

	} else {
		m_Top=0 ;
		m_Left=0 ;
		m_Height = ai_poDT->m_oPageData[m_iPage].m_SizeY ;
		m_Width =  ai_poDT->m_oPageData[m_iPage].m_SizeX ;
		m_IsFixed = M4_TRUE ;
		m_bSwTogether=M4_TRUE ;
		m_oCancelCnt.StrCpy("") ;
	} ;

	if ( ! m_IsFixed ) {

		if ( ! ai_poSource->m_bLowerLimitLoaded ) {
			//Top debe estar ya en DvcUnit
			m_LowerLimit = ai_poDT->m_oPageData[m_iPage].m_SizeY - m_Top - 
					ClMetric::MilimToDvc(20)  ;	
		} else  {
			m_LowerLimit =  ai_poSource->m_dLowerLimit < 0 ? 
				(-ai_poSource->m_dLowerLimit) * m_stDefaultData.GetFontDesignHeight() : 
				ai_poDT->GetDvcUnit(ai_poSource->m_dLowerLimit) ;
		} ;
	
	} else {
		m_LowerLimit=-1 ;
	} ;


	ENG_DT_OPT_STR(	m_oRegionBorder,	m_oRegionBorder,	m_bRegionBorderLoaded,	"" ) ; 
	ENG_DT_OPT(		m_uiRegionColour,	m_uiRegionColour,	m_bRegionColourLoaded,	ClENG_Colour::ID_TRANSPARENT ) ; 

	//--->Analisis m_oCTMerge

	if (!SwOk) goto exit ;

	ENG_DT_OPT_STR(	  m_oCTMerge,		m_oCTMerge,	m_bCTMergeLoaded,	"") ; 

	if ( m_oCTMerge.StrLenTrim() >0 ) {

		ClMIT_Args Arg ;
		ClMIT_Str StrAux ;

		Arg.Init(m_oCTMerge, ClMIT_Args::NO_SUBARG, ".") ;
	
		if ( Arg.GetArgCount()!=4 && Arg.GetArgCount()!=2 ) {
			*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_130 ] ) 
				<< m_oCTMerge.InChar(0) << SEND_MSG ;
			m_MergeMode=ENG_CT_NO_MERGE ;
			SwOk=M4_FALSE ;	
			goto exit ;
		} ;

		SwOk= M4_BOOL(SwOk && Arg.GetString(0,0,&m_oCTId,M4_FALSE));

		SwOk= M4_BOOL(SwOk && Arg.GetString(1,0,&StrAux,M4_FALSE));
		switch ( *StrAux.InChar(0) ) {
		case 'H': case 'h':
			m_MergeMode=ENG_CT_HEADER_MERGE ;			
			break;
		case 'D': case 'd':
			m_MergeMode=ENG_CT_ROW_MERGE ;			
			break;
		default:
			*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_130 ] ) 
				<< m_oCTMerge.InChar(0) << SEND_MSG ;
			m_MergeMode=ENG_CT_NO_MERGE ;			
			SwOk=M4_FALSE ;
			break ;
		} ;

		if ( Arg.GetArgCount()==4 ) {
			SwOk= M4_BOOL(SwOk && Arg.GetString(2,0,&StrAux,M4_FALSE));
			switch (*StrAux.InChar(0)) {
			case 'U': case 'u':
				m_MergeYMode=ENG_CT_OVER_STC ;
				break ;
			case 'D': case 'd':
				m_MergeYMode=ENG_CT_DOWN_STC ;
				break ;
			case 'A': case 'a':
			default:
				m_MergeYMode=ENG_CT_ALIGN_STC ;
				break ;
			} ;

			SwOk = M4_BOOL(SwOk && Arg.GetString(3,0,&StrAux,M4_FALSE));
			switch (*StrAux.InChar(0)) {
			case 'R': case 'r':
			default:
				m_MergeXMode=ENG_CT_RIGHT_STC ;
				m_OffsetLeft=ai_poDT->GetDvcUnit(atol(StrAux)) ;
				if (m_OffsetLeft<0) m_OffsetLeft=0 ;
				break ;
			} ;

		} else {

			m_MergeYMode=ENG_CT_ALIGN_STC ;
			m_MergeXMode=ENG_CT_RIGHT_STC ;
			m_OffsetLeft=0 ;

		} ;
		if (m_MergeMode==ENG_CT_ROW_MERGE) m_MergeYMode=ENG_CT_ALIGN_STC ;


	} else m_MergeMode=ENG_CT_NO_MERGE ;

	//<---Analisis CTMerge


	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oCancelCnt,	&ai_poDT->m_oLE));

	m_bIsModified = ai_poSource->m_bIsModified;

	//Carga fields

	m_oFieldList.clear() ;

	for ( FieldIt = ai_poSource->m_oFieldList.begin() ; 
		FieldIt != ai_poSource->m_oFieldList.end() ; ++FieldIt  )
	{
		SwOk = M4_BOOL(SwOk && Field.Init( &(*FieldIt), ai_Owner, &m_stDefaultData, ai_poDT ));
		if (!SwOk) break ;

		m_oFieldList.push_back(Field) ;
	} ;

exit:

	return SwOk ;

} 

void StENG_DT_CntData::Init( StENG_DT_CntData *ai_poSource ) 
{
	//Copia valores DD 


	ENG_DT_COPY_FROM_DD_2(m_dTop,m_bTopLoaded) ;
	ENG_DT_COPY_FROM_DD_2(m_dLeft,m_bLeftLoaded) ;
	ENG_DT_COPY_FROM_DD_2(m_dHeight,m_bHeightLoaded) ;
	ENG_DT_COPY_FROM_DD_2(m_dWidth,m_bWidthLoaded) ;
	ENG_DT_COPY_FROM_DD_2(m_dLowerLimit,m_bLowerLimitLoaded) ;

	//Resto valores

	m_iPage				=ai_poSource->m_iPage ;
	m_Top				=ai_poSource->m_Top ;
	m_Left				=ai_poSource->m_Left ;
	m_Height			=ai_poSource->m_Height ;
	m_Width				=ai_poSource->m_Width ;
	m_IsFixed			=ai_poSource->m_IsFixed ;
	m_bSwTogether		=ai_poSource->m_bSwTogether ;
	m_LowerLimit		=ai_poSource->m_LowerLimit ;
	m_oCancelCnt.StrCpy( ai_poSource->m_oCancelCnt ) ;
	m_uiRegionColour	=ai_poSource->m_uiRegionColour ;				
	m_oRegionBorder		=ai_poSource->m_oRegionBorder; 				
	m_bIsModified		=ai_poSource->m_bIsModified ;
	m_oFieldList		=ai_poSource->m_oFieldList ;
	m_stDefaultData		=ai_poSource->m_stDefaultData ;
	m_oCTMerge.StrCpy( ai_poSource->m_oCTMerge) ;
	m_oCTId.StrCpy( ai_poSource->m_oCTId) ;
	m_MergeMode			=ai_poSource->m_MergeMode ;
	m_MergeYMode		=ai_poSource->m_MergeYMode ;
	m_MergeXMode		=ai_poSource->m_MergeXMode ;
	m_OffsetLeft		=ai_poSource->m_OffsetLeft ;

}


StENG_DT_FieldData	* StENG_DT_CntData::FindIdCTField( 
	ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField, ENG_DT_FieldList_t::iterator *ao_It ) 
{
	StENG_DT_FieldData	*R ;
	ENG_DT_FieldList_t::iterator It ;

	for ( It=m_oFieldList.begin(), R=NULL; It!=m_oFieldList.end(); ++It ) {
		if ( ClMIT_Str::StrICmpTrim( (*It).m_oIdCtField,ai_pcIdCtField)==0 ) {
			R = &(*It) ;
			break ;
		} ;
	} ;

	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Field with IdCT %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Field with IdCT %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

void StENG_DT_CntData::IncrCountFontFormatColourInstances( 
	m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
	m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
	m4uint32_t *aoi_puiColourCount ) 
{
	ENG_DT_FieldList_t::iterator Field ;
	m4uint32_t IdColour;
	ClMIT_Str StrAux ;
	m4int16_t B;

	//Def de cnt 

	m_stDefaultData.IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount ) ;

	//Def de los fields del cnt

	for ( Field=m_oFieldList.begin(); Field!=m_oFieldList.end(); ++Field )  {
		(*Field).m_stDefaultData.IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

		if ( aoi_puiColourCount ) {
			if ( (*Field).m_uiRegionColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==(*Field).m_uiRegionColour ) ++(*aoi_puiColourCount) ;

			if((*Field).m_oRegionBorder.StrLen()>0) {
				for(B=0; B<4; B++) {
					StrAux.ToASCIZ((*Field).m_oRegionBorder.InChar(7+(12*B)),5);
					IdColour = atol(StrAux);
					if ( IdColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==IdColour ) ++(*aoi_puiColourCount) ;
				};
			};
		} ;
	} ;

	if ( aoi_puiColourCount ) {
		if ( m_uiRegionColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==m_uiRegionColour ) ++(*aoi_puiColourCount) ;

		if(m_oRegionBorder.StrLen()>0) {
			for(B=0; B<4; B++) {
				StrAux.ToASCIZ(m_oRegionBorder.InChar(7+(12*B)),5);
				IdColour = atol(StrAux);
				if ( IdColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==IdColour ) ++(*aoi_puiColourCount) ;
			};
		};
	} ;
}

//======================================================================StENG_DT_SectionData

StENG_DT_SectionData::StENG_DT_SectionData()
{
	m_oSonSection=NULL ;
}

void StENG_DT_SectionData::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_oSonSection, 
			StENG_DT_SonSectionData, m_iSonSectionCount ) ;
	m_oCnt.clear() ;
	m_stDefaultData.End() ;
} ;



m4bool_t StENG_DT_SectionData::Init( StENG_DD_SectionData *ai_poSource, ClENG_DesignTree *ai_poDT ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str Together ;
	ENG_TYPE_CONTAINER CntType ;
	ClMIT_Args Arg ;
	m4int16_t B ;
	StMIT_Inspector* Insp = GET_INSP() ;
	StENG_DD_SectionData::CntList_t::iterator CntDDIt ;
	StENG_DD_SectionData::SonList_t::iterator SonIt ;
	StENG_DT_CntData Cnt ;

	StENG_DT_SectionData::End() ;
	
	ENG_DT_OBLIG(		m_iIdSection ,		m_iIdSection ,		m_bIdSectionLoaded, "IdSection") ; 
	ENG_DT_OPT_STR(		m_oSectionName,		m_oSectionName,		m_bSectionNameLoaded,"?") ; 

	ENG_DT_OPT(			m_NoPrintEmpty,		m_NoPrintEmpty,		m_bNoPrintEmptyLoaded,  ClENG_VR::NEVER_NULL) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestSecNullIfEmpty(m_NoPrintEmpty));
	
	ENG_DT_OPT(			m_BeforePBreak,		m_BeforePBreak,		m_bBeforePBreakLoaded,  ClENG_Section::NONE_BREAK) ; 
	ENG_DT_OPT(			m_AfterPBreak,		m_AfterPBreak,		m_bAfterPBreakLoaded,  ClENG_Section::NONE_BREAK) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestSectionBreakMode(m_BeforePBreak) 
					    && ClENG_DesignTree::TestSectionBreakMode(m_AfterPBreak));
	
	ENG_DT_OPT(			m_HeaderMode,		m_HeaderMode,		m_bHeaderModeLoaded,  ClENG_Section::H_ALL_PAGES) ; 
	
	ENG_DT_OPT(			m_FooterMode,		m_FooterMode,		m_bFooterModeLoaded,  ClENG_Section::F_ALL_PAGES) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestSectionHeaderMode(m_HeaderMode) 
				        && ClENG_DesignTree::TestSectionFooterMode(m_FooterMode));
	
	ENG_DT_OPT(			m_GoT,				m_GoT,				m_bGoTLoaded,		ClENG_VR::RECORD_GOT) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestSecGoT(m_GoT));
	
	ENG_DT_OPT(			m_RangeMode,		m_RangeMode,		m_bRangeModeLoaded,	ClENG_VR::ALL_VR) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestSecRangeMode(m_RangeMode));
	
	ENG_DT_OPT_STR(		m_oGroupKey,		m_oGroupKey,		m_bGroupKeyLoaded,"") ; 
	ENG_DT_OBLIG_STR(	m_RecordsetNodeName,m_RecordsetNodeName,m_bNextRecNodeLoaded,"NodeName") ; 
	ENG_DT_OPT(			m_bSwBreakVR,		m_bSwBreakVR,		m_bSwBreakVRLoaded,	 M4_FALSE) ; 
	ENG_DT_OPT(			m_bNullOnSons,		m_bNullOnSons,		m_bNullOnSonsLoaded,	 M4_FALSE) ; 
	ENG_DT_OPT_STR(		m_oOneOneStr,		m_oOneOneStr,		m_bOneOneStrLoaded,"") ; 

	ENG_DT_OPT_STR(		m_oCancelSection,	m_oCancelSection,	m_bCancelSectionLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oResetBeforeTotal,m_oResetBeforeTotal,m_bResetBeforeTotalLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oPrepareRecordset,m_oPrepareRecordset,m_bPrepareRecordsetLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oResetHeaderTotals,m_oResetHeaderTotals,m_bResetHeaderTotalsLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oCancelVR,		m_oCancelVR,		m_bCancelVRLoaded,"") ; 
	ENG_DT_OPT_STR(		m_oSetPostVRTotal,	m_oSetPostVRTotal,	m_bSetPostVRTotalLoaded,"") ; 

	ENG_DT_OPT_STR(		m_GuideNode,		m_GuideNode,		m_bGuideNodeLoaded, m4pchar_t(m_RecordsetNodeName)) ; 
	ENG_DT_OPT_STR(		m_GuideItemName,	m_GuideItemName,	m_bGuideItemNameLoaded, "" ) ; 
	if ( m_GoT==ClENG_VR::SLICE_GOT ) {
		SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bGuideItemNameLoaded, "GuideItemName" ));
	} ;
	
	//Carga Together

	ENG_DT_OPT_STR(		Together,			m_oTogether,		m_bTogetherLoaded,"") ; 

	Arg.Init( Together, ':', " ," ) ;
	m_MaskTogether=ClMIT_Flag::NONE_BITS ;
	for ( B=0; B<Arg.GetArgCount(); ++B ) {
		CntType=(ENG_TYPE_CONTAINER)Arg.GetLong(B,0) ;
		if (!ClENG_DesignTree::TestLoaderCntType(CntType)) {
			SwOk=M4_FALSE ;
			break;
		} ;
		switch (CntType) {
		case ENG_CNT_BEFORE:
			m_MaskTogether = m_MaskTogether | ClENG_Section::TOG_CNT_BEFORE ;
			break ;
		case ENG_CNT_PRERECORD:
			m_MaskTogether = m_MaskTogether | ClENG_Section::TOG_CNT_PRE_VR ;
			break ;
		case ENG_CNT_RECORD:
			m_MaskTogether = m_MaskTogether | ClENG_Section::TOG_CNT_VR ;
			break ;
		case ENG_CNT_POSTRECORD:
			m_MaskTogether = m_MaskTogether | ClENG_Section::TOG_CNT_POST_VR ;
			break ;
		case ENG_CNT_AFTER:
			m_MaskTogether = m_MaskTogether | ClENG_Section::TOG_CNT_AFTER ;
			break ;
		default:
			*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_120 ] ) 
				<< (m4int16_t)CntType << SEND_MSG ;
			SwOk=M4_FALSE ;
			break ;
		} ;
	} ;


	// Resolución LE en expresiones 

	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oCancelSection,		&ai_poDT->m_oLE));
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oResetBeforeTotal,	&ai_poDT->m_oLE));
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oPrepareRecordset,	&ai_poDT->m_oLE));
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oResetHeaderTotals,	&ai_poDT->m_oLE));
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oCancelVR,			&ai_poDT->m_oLE));
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oSetPostVRTotal,		&ai_poDT->m_oLE));
	
	// Defectos.
	// Deben calcularse antes de los containers: estos lo usan

	m_stDefaultData.InheritAllFrom( &ai_poDT->m_oGlobal.m_oReportDefaults ) ;
	SwOk = M4_BOOL(SwOk && m_stDefaultData.UpdateFrom( 
		           &ai_poSource->m_stDefaultData, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));

	// Carga y verificación de Cnt

	m_oCnt.clear() ;

	for ( CntDDIt=ai_poSource->m_oCnt.begin() ; CntDDIt!=ai_poSource->m_oCnt.end(); ++CntDDIt ) {

		SwOk = M4_BOOL( SwOk && Cnt.Init( &(*CntDDIt).second, 
				        ENG_REPORT_CONTAINER, &ai_poSource->m_stDefaultData, ai_poDT ));
		if (!SwOk) break ;

		if ( FindCnt( ENG_NOT_EXIST_REQUIRED, (*CntDDIt).first ) ) {
			SwOk=M4_FALSE ;
			break ;
		} ;

		SwOk = M4_BOOL(SwOk &&  ClENG_DesignTree::TestLoaderCntType( (*CntDDIt).first ));  
		if (!SwOk) break ;

		m_oCnt.insert( StENG_DT_SectionData::CntList_t::value_type((*CntDDIt).first,Cnt) ) ;
	} ;

	//Carga SonSection (no carga seccion)

	m_iSonSectionCount	= ai_poSource->m_oSonList.size() ;
	if ( m_iSonSectionCount>0 ) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_oSonSection, 
				StENG_DT_SonSectionData, m_iSonSectionCount ) ;

		for ( SonIt = ai_poSource->m_oSonList.begin(), B=0; B<m_iSonSectionCount; ++B, ++SonIt ) {
			SwOk = M4_BOOL(SwOk && m_oSonSection[B].Init( &(*SonIt)  ));
		} ;
	} ;

	m_bIsModified = ai_poSource->m_bIsModified;

	return SwOk ;
}

StENG_DT_SectionData * StENG_DT_SectionData::FindSection(m4uint32_t ai_uiIdSection) 
{
	if ( m_iIdSection==ai_uiIdSection ) {
		return this ; 
	} else {
		StENG_DT_SectionData *R ;
		m4int16_t B ;

		for (B=0, R=NULL; B<m_iSonSectionCount; ++B ) {
			R = m_oSonSection[B].m_oSection.FindSection(ai_uiIdSection) ;
			if (R) break ;
		} ;
		return R ;
	} ;
}

StENG_DT_CntData *StENG_DT_SectionData::FindCnt(ENG_FIND_ACTION ai_Action, ENG_TYPE_CONTAINER ai_CntId, 
		CntList_t::iterator *ao_It) 
{
	CntList_t::iterator It ;
	StENG_DT_CntData *R ;
		
	It = m_oCnt.find(ai_CntId) ;
	R = It!=m_oCnt.end() ? &(*It).second : NULL ;
 
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Cnt %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ClENG_DesignTree::GetCntTraceStr(ai_CntId) << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Cnt %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ClENG_DesignTree::GetCntTraceStr(ai_CntId) << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;

}

void StENG_DT_SectionData::IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) 
{
	m4int16_t B ;
	CntList_t::iterator Cnt ;
	
	//Procesa defectos de seccion

	m_stDefaultData.IncrCountFontFormatColourInstances(
			ai_uiIdFont, ai_uiIdFormat, ai_uiIdColour, aoi_puiFontCount, aoi_puiFormatCount, aoi_puiColourCount ) ;

	//Procesa containers

	for ( Cnt=m_oCnt.begin(); Cnt != m_oCnt.end(); ++Cnt ) {
		(*Cnt).second.IncrCountFontFormatColourInstances(
			ai_uiIdFont, ai_uiIdFormat, ai_uiIdColour, aoi_puiFontCount, aoi_puiFormatCount, aoi_puiColourCount ) ;
	} ;

	//Procesa hijas

	for (B=0;B<m_iSonSectionCount;++B) {
		m_oSonSection[B].m_oSection.IncrCountFontFormatColourInstances(
			ai_uiIdFont, ai_uiIdFormat, ai_uiIdColour, aoi_puiFontCount, aoi_puiFormatCount, aoi_puiColourCount ) ;
	} ;

}

//==========================================================================StENG_DT_FieldData

m4bool_t StENG_DT_FieldData::Init( 
	StENG_DD_FieldData *ai_poSource, ENG_FIELD_OWNER ai_Owner,
	StENG_DT_Defaults *ai_poCntDef,
	ClENG_DesignTree *ai_poDT ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StMIT_Inspector* Insp = GET_INSP() ;
	ClMIT_IdNumDesc IdDesc ;
	size_t L1, L2 ;
	ClMIT_Str Str ;

	//Copia valores DD 

	ENG_DT_COPY_FROM_DD(m_dTop,m_bTopLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dLeft,m_bLeftLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dHeight,m_bHeightLoaded) ;
	ENG_DT_COPY_FROM_DD(m_dWidth,m_bWidthLoaded) ;

	//Resto valores
	
	IdDesc.Init(ai_poSource->m_uiIdField,ai_poSource->m_oFieldName) ;

	*Insp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1090 ] ) 
		<< IdDesc << END_LOC ;

	ENG_DT_OPT(			m_uiIdField,	m_uiIdField,	m_bIdFieldLoaded,	0) ; 
	ENG_DT_OPT_STR(		m_oFieldName,	m_oFieldName,	m_bFieldNameLoaded,	"?") ; 
	ENG_DT_OPT(			m_FldType,		m_FldType,		m_bFldTypeLoaded,	ENG_FLD_STRING ) ; 
	SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestFieldType(m_FldType));
	if (!SwOk) goto exit ;		//Salir si falla, se usa el valor en ifs de carga

	ENG_DT_OPT_STR(		m_oSaveNode,		m_oSaveNode,		m_bSaveNodeLoaded,	"") ; 
	ENG_DT_OPT_STR(		m_oSaveItemName,	m_oSaveItemName,	m_bSaveItemNameLoaded,	"") ; 
	if ( m_oSaveNode.StrLenTrim() != 0 ) {
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bSaveItemNameLoaded, "Save Item name" ));
	} ;

	if ( ai_Owner == ENG_CT_CELL ) {
		m_oIdPi.StrCpy("") ;
	} else {
		ENG_DT_OPT_STR(		m_oIdPi,		m_oIdPi,		m_bIdPiLoaded,	"") ; 
	} ;

	ENG_DT_OPT(			m_bSwOptimize,		m_bSwOptimize,	m_bSwOptimizeLoaded, 
		M4_BOOL(ai_Owner == ENG_PAGE_CONTAINER)) ; 
	ENG_DT_OPT(			m_bSwStatic,		m_bSwStatic,	m_bSwStaticLoaded, 
		M4_BOOL(ai_Owner == ENG_PAGE_CONTAINER)) ;	//Luego en Tratamiento Exp-Literal puede resetearse


	ENG_DT_OPT(	m_bEmbebedRaster,	m_bEmbebedRaster,	m_bEmbebedRasterLoaded, M4_FALSE ) ; 
	ENG_DT_OPT_STR(		m_oBNRaster,		m_oBNRaster,		m_bBNRasterLoaded,	"") ; 
	ENG_DT_OPT_STR(		m_oColourRaster,	m_oColourRaster,	m_bColourRasterLoaded,	"") ; 
	
	//---->Tratamiento ENG_FLD_RASTERG con m_bEmbebedRaster

	if (  m_FldType==ENG_FLD_RASTERG ) {
		if ( m_bEmbebedRaster && m_oBNRaster.StrLenTrim()==0 &&  m_oColourRaster.StrLenTrim()==0 ) {
			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID,  
				"Embebed Bitmap not specified for this graphic field", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		} ;
	} ;

	
	//---->Tratamiento Expr-Literal. Necesita de m_oIdPi y de m_bEmbebedRaster

	ENG_DT_OPT_STR(		m_oExpr,		m_oExpr,		m_bExprLoaded,	"") ; 
	ENG_DT_OPT_STR(		m_oLiteralValue,m_oLiteralValue,m_bLiteralValueLoaded,	"") ; 

	//---->Tratamiento Indices
	ENG_DT_OPT_STR(		m_oIndexExpr,	m_oIndexExpr,	m_bIndexExprLoaded,	"\"\"") ; 
	ENG_DT_OPT(	m_iIndexLevel,	m_iIndexLevel,	m_bIndexLevelLoaded, ClENG_Field::NO_INDEX_ENTRY ) ; 

	//---->Tratamiento Hiperenlaces
	ENG_DT_OPT_STR(		m_oHyperlink,	m_oHyperlink,	m_bHyperlinkLoaded,	"" ) ; 
	
	//---->Imagenes temporales en el Web Server
	ENG_DT_OPT(	m_bTempWebImage, m_bTempWebImage, m_bTempWebImageLoaded, M4_FALSE ) ; 

 	L1 = m_oExpr.StrLenTrim() ;
	L2 = m_oLiteralValue.StrLenTrim() ;
	if ( ( (L1==0 && L2==0) || (L1>0 && L2>0) ) && m_oIdPi.StrLenTrim()==0) {
		if ( m_FldType==ENG_FLD_RASTERG  ) {
			if ( ! m_bEmbebedRaster ) {
				*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162993, 
					"Bitmap not specified for this graphic field", ClMIT_Msg::MIXED_OUT)
					<< SEND_MSG ;
				SwOk=M4_FALSE ;
			} ;
		} else {
			
			*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[LOADER_MSG_110] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
		} ;
		
	} ;
	if (L2>0) {
		m_bSwStatic=M4_TRUE ;
		Str.Accommodate(L2+3) ; 
		* Str.InChar(0) = '"' ;
		strncpy( Str.InChar(1), m_oLiteralValue, L2 ) ;
		strcpy( Str.InChar(L2+1), "\"" ) ;
		m_oExpr.StrCpy( Str ) ;
	} ;
	SwOk = M4_BOOL(SwOk && ClENG_Analex::PrepareForLongExpr(&m_oExpr,	&ai_poDT->m_oLE));

	//<----Tratamiento Expr-Literal

	ENG_DT_OPT(		m_bIsVarHeight,		m_bIsVarHeight,	m_bIsVarHeightLoaded,	M4_TRUE ) ; 
	ENG_DT_OPT(		m_bIsJustified,		m_bIsJustified,	m_bIsJustifiedLoaded,	M4_FALSE ) ; 

	if (m_FldType==ENG_FLD_BOX) {
		ENG_DT_OBLIG(		m_iMaxBoxes,		m_iMaxBoxes,	m_bMaxBoxesLoaded, "Max Boxes" ) ; 
	} else {
		m_iMaxBoxes=0;
	};

	if ( m_FldType==ENG_FLD_STRING || m_FldType==ENG_FLD_BOX) {
		ENG_DT_OPT(		m_FillMode,		m_FillMode,	m_bFillModeLoaded,	
			ai_poSource->m_bFillCharLoaded ? ClENG_FieldStr::NO_EMPTY_FILL : 
			ClENG_FieldStr::NO_FILL   ) ; 
		SwOk = M4_BOOL(SwOk && ClENG_DesignTree::TestFieldFillMode(m_FillMode));
		if ( m_FillMode != ClENG_FieldStr::NO_FILL ) {
			ENG_DT_OBLIG(		m_cFillChar,m_cFillChar, m_bFillCharLoaded, "FillChar" ) ; 
		} ;
	} ;

	ENG_DT_OPT(		m_bSwDuplicated,	m_bSwDuplicated,	m_bSwDuplicatedLoaded,	M4_FALSE ) ; 

	ENG_DT_OPT_STR(	m_oIdCtField,	m_oIdCtField,	m_bIdCtFieldLoaded,	"" ) ; 
	if ( M4_BOOL(ai_Owner==ENG_CT_CELL)) {
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bIdCtFieldLoaded, "Id CrossTab" ));
	} ;

	ENG_DT_OPT(		m_BarCodeType,	m_BarCodeType,	m_bBarCodeTypeLoaded, ENG_NO_BAR_CODE ) ; 
	SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestFieldBarCodeType( m_BarCodeType ));

	ENG_DT_OPT_DVC(	m_Interline,	m_dInterline,	m_bInterlineLoaded, 0) ; 

	//Defectos

	m_stDefaultData.InheritAllFrom( ai_poCntDef ) ;
	SwOk = M4_BOOL(SwOk && m_stDefaultData.UpdateFrom( 
				   &ai_poSource->m_stDefaultData, ai_poDT, ClMIT_Trace::PROPAGATE_ERROR ));

	//Calculos Top, ect.Necesitan de los defectos cargados y de m_Interline cargado.

	if ( ai_poSource->m_bTopLoaded ) {
		if (ai_poSource->m_dTop < 0) {
			m_Top=(-ai_poSource->m_dTop)*m_stDefaultData.GetFontDesignHeight()  ;
		} else {
			m_Top=ai_poDT->GetDvcUnit(ai_poSource->m_dTop) ;
		} ;
	} else {
		m_Top = 0 ;
	} ;

	if ( ai_poSource->m_bLeftLoaded ) {
		if (ai_poSource->m_dLeft < 0) {
			m_Left=(-ai_poSource->m_dLeft)*m_stDefaultData.GetFontDesignWidth()  ;
		} else {
			m_Left=ai_poDT->GetDvcUnit(ai_poSource->m_dLeft) ;
		} ;
	} else {
		m_Left = 0 ;
	} ;

	if ( ai_poSource->m_bWidthLoaded ) {
		if (ai_poSource->m_dWidth < 0) {
			m_Width=(-ai_poSource->m_dWidth)*m_stDefaultData.GetFontDesignWidth()  ;
		} else {
			m_Width=ai_poDT->GetDvcUnit(ai_poSource->m_dWidth) ;
		} ;
	} else {
		m_Width = ClMetric::MilimToDvc(20) ;
	} ;

	if ( ai_poSource->m_bHeightLoaded ) {
		if (ai_poSource->m_dHeight < 0) {
			m_Height=(-ai_poSource->m_dHeight) * ( m_stDefaultData.GetFontDesignHeight() +
				(m_FldType==ENG_FLD_MULTIL ? m_Interline : 0) ) ;
		} else {
			m_Height=ai_poDT->GetDvcUnit(ai_poSource->m_dHeight) ;
		} ;
	} else {
		m_Height=(-1) * ( m_stDefaultData.GetFontDesignHeight() +
			(m_FldType==ENG_FLD_MULTIL ? m_Interline : 0) ) ;
	} ;

	if (m_bEmbebedRaster) {
		m_bSwStatic = M4_TRUE ;
		m_stDefaultData.m_bSwGraphics=M4_FALSE ;
	} ;

	ENG_DT_OPT_STR(	m_oRegionBorder,	m_oRegionBorder,	m_bRegionBorderLoaded,	"" ) ; 
	ENG_DT_OPT(		m_uiRegionColour,	m_uiRegionColour,	m_bRegionColourLoaded,	ClENG_Colour::ID_TRANSPARENT ) ; 
	
	//---->Tratamiento linea

	if (  m_FldType==ENG_FLD_LINE ) {
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bX1Loaded, "Line X1" ));
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bX2Loaded, "Line X2" ));
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bY1Loaded, "Line Y1" ));
		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig( ai_poSource->m_bY2Loaded, "Line Y2" ));
		if (!SwOk) goto exit ;

		if (ai_poSource->m_dX1 < 0) {
			m_X1=(-ai_poSource->m_dX1)*m_stDefaultData.GetFontDesignWidth()  ;
		} else {
			m_X1=ai_poDT->GetDvcUnit(ai_poSource->m_dX1) ;
		} ;

		if (ai_poSource->m_dX2 < 0) {
			m_X2=(-ai_poSource->m_dX2)*m_stDefaultData.GetFontDesignWidth()  ;
		} else {
			m_X2=ai_poDT->GetDvcUnit(ai_poSource->m_dX2) ;
		} ;

		if (ai_poSource->m_dY1 < 0) {
			m_Y1=(-ai_poSource->m_dY1) * m_stDefaultData.GetFontDesignHeight() ;
		} else {
			m_Y1=ai_poDT->GetDvcUnit(ai_poSource->m_dY1) ;
		} ;

		if (ai_poSource->m_dY2 < 0) {
			m_Y2=(-ai_poSource->m_dY2) * m_stDefaultData.GetFontDesignHeight() ;
		} else {
			m_Y2=ai_poDT->GetDvcUnit(ai_poSource->m_dY2) ;
		} ;
	} ;

	ENG_DT_OPT(	m_bAdjustLineToContainer, m_bAdjustLineToContainer, m_bAdjustLineToContainerLoaded, M4_FALSE );
	
	m_bIsModified = ai_poSource->m_bIsModified;

exit:
	
	*Insp->m_poTrace << POP_LOC ;
								 
	return SwOk ;
}


void StENG_DT_FieldData::InitFieldStrDefault(ClENG_DesignTree *ai_poDT) 
{
	m_uiIdField			= 0 ;
	m_oFieldName.StrCpy("Default STR field") ;
	m_FldType			= ENG_FLD_STRING ;
	m_Top				= 0 ;
	m_Left				= 0 ;
	m_Width				= 0 ;
	m_Height			= 0 ;
	m_oSaveNode.StrCpy("") ;
	m_oSaveItemName.StrCpy("") ;
	m_bSwStatic			= M4_FALSE ;
	m_oIdPi.StrCpy("") ;
	m_bSwOptimize		= M4_FALSE ;
	m_oIndexExpr.StrCpy("\"\"") ;
	m_oHyperlink.StrCpy("");
	m_iIndexLevel		= ClENG_Field::NO_INDEX_ENTRY ;
	m_oExpr.StrCpy("\"\"") ;	
	m_oLiteralValue.StrCpy("") ;
	m_bTempWebImage		= M4_FALSE ;
	m_bEmbebedRaster	= M4_FALSE ;
	m_Interline			= 0 ;
	m_bIsVarHeight		= M4_FALSE ;
	m_bIsJustified		= M4_FALSE ;
	m_iMaxBoxes			= 0 ;
	m_FillMode			= ClENG_FieldStr::NO_FILL ;
	m_cFillChar			= '*' ;
	m_bSwDuplicated		= M4_FALSE ;
	m_oIdCtField.StrCpy("") ;
	m_uiRegionColour	= ClENG_Colour::ID_TRANSPARENT ;
	m_oRegionBorder.StrCpy("") ;
	m_bIsModified		= M4_TRUE ;
	m_BarCodeType		= ENG_NO_BAR_CODE ;
	m_bAdjustLineToContainer = M4_FALSE ;
	m_stDefaultData.ResetGlobalDefaults( ai_poDT, ClMIT_Trace::PROPAGATE_ERROR  ) ;

	m_stFromValues.m_bTopLoaded		= M4_FALSE ;
	m_stFromValues.m_bLeftLoaded 	= M4_FALSE ;
	m_stFromValues.m_bHeightLoaded	= M4_FALSE ;
	m_stFromValues.m_bWidthLoaded	= M4_FALSE ;
}


void StENG_DT_FieldData::IncrCountColourInstances( m4uint32_t ai_uiIdColour, m4uint32_t *aoi_puiColourCount )
{
	m4uint32_t IdColour;
	ClMIT_Str StrAux ;
	m4int16_t B;

	if ( aoi_puiColourCount ) {
		if ( m_uiRegionColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==m_uiRegionColour ) ++(*aoi_puiColourCount) ;

		if(m_oRegionBorder.StrLen()>0) {
			for(B=0; B<4; B++) {
				StrAux.ToASCIZ(m_oRegionBorder.InChar(7+(12*B)),5);
				IdColour = atol(StrAux);
				if ( IdColour!=ClENG_Colour::ID_TRANSPARENT && ai_uiIdColour==IdColour ) ++(*aoi_puiColourCount) ;
			};
		};
	} ;
}

//=============================================================================ClENG_DesignTree

//----------------------------------Para testear los enums y obligatorios

m4bool_t ClENG_DesignTree::TestOblig(m4bool_t ai_bSwLoaded, const m4char_t *ai_VariableName) 
{
	if (!ai_bSwLoaded) {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_80 ] ) 
			<< ai_VariableName << SEND_MSG ;
		return M4_FALSE ;	
	} else return M4_TRUE ;
}

m4bool_t ClENG_DesignTree::TestFieldAlign( ClENG_FieldStr::ALIGN ai_iValue ) 
{
	if ( ai_iValue==ClENG_FieldStr::CENTER || ai_iValue==ClENG_FieldStr::LEFT || 
		ai_iValue==ClENG_FieldStr::RIGHT || ai_iValue==ClENG_FieldStr::POINT )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestFieldType( ENG_FIELD_TYPE ai_iValue ) 
{
	if ( ai_iValue==ENG_FLD_STRING || ai_iValue==ENG_FLD_MULTIL || ai_iValue==ENG_FLD_BOX || 
		ai_iValue==ENG_FLD_RASTERG || ai_iValue==ENG_FLD_BARCODE || ai_iValue==ENG_FLD_LINE )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestMergePage( ENG_CT_MERGE_PAGE ai_iValue ) 
{
	if ( ai_iValue==ENG_CT_NONE_PAGE || ai_iValue==ENG_CT_FIRST_PAGE || 
		ai_iValue==ENG_CT_ALL_PAGES )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestReportModeMetric( ENG_MODE_METRIC ai_iValue ) 
{
	if ( ai_iValue==ENG_MILIM || ai_iValue==ENG_DINCHES ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestReportBreakMode( ClENG_Report::BREAK_MODE ai_iValue ) 
{
	if ( ai_iValue==ClENG_Report::NO_BREAK || ai_iValue==ClENG_Report::RECORD_BREAK || 
		ai_iValue==ClENG_Report::RECORD_SLICE_BREAK ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestLineStyle( ClENG_OutDevice::STYLE ai_iValue ) 
{
	if ( ai_iValue==ClENG_OutDevice::SOLID || ai_iValue==ClENG_OutDevice::DASH || 
		ai_iValue==ClENG_OutDevice::DOT || ai_iValue==ClENG_OutDevice::DASH_DOT ||
		ai_iValue==ClENG_OutDevice::DASH_DOT_DOT ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestAlignParamCellY( ENG_CT_CELL_Y_ALIGN ai_iValue ) 
{
	if ( ai_iValue==ENG_CT_TOP_DISPLAY || ai_iValue==ENG_CT_BOTTOM_DISPLAY || 
		ai_iValue==ENG_CT_CENTER_Y_DISPLAY  )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestAlignParamCellX( ENG_CT_CELL_X_ALIGN ai_iValue ) 
{
	if ( ai_iValue==ENG_CT_LEFT_DISPLAY || ai_iValue==ENG_CT_RIGHT_DISPLAY  
		|| ai_iValue==ENG_CT_CENTER_X_DISPLAY ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}


m4bool_t ClENG_DesignTree::TestFontESCPitch( ClFONT_M_Font::ESC_PITCH ai_iValue ) 
{
	if ( ai_iValue==ClFONT_M_Font::ESC_PITCH_10 || ai_iValue==ClFONT_M_Font::ESC_PITCH_12 ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestPagePaperSize( ClENG_PagePrinter::PAPER_SIZE ai_iValue ) 
{
	if ( ai_iValue==ClENG_PagePrinter::A4 || ai_iValue==ClENG_PagePrinter::A3 ||
		ai_iValue==ClENG_PagePrinter::LEGAL || ai_iValue==ClENG_PagePrinter::LETTER ||
		ai_iValue==ClENG_PagePrinter::EXECUTIVE || ai_iValue==ClENG_PagePrinter::PERSONAL 
	)
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestPageOrientation( ClENG_PagePrinter::ORIENTATION ai_iValue ) 
{
	if ( ai_iValue==ClENG_PagePrinter::PORTRAIT || ai_iValue==ClENG_PagePrinter::LANDSCAPE ) {
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestLogicOperator( ClMIT_ChnValue::OPERATOR ai_iValue )
{
	if ( ai_iValue==ClMIT_ChnValue::LESS ||  ai_iValue==ClMIT_ChnValue::LESS_EQUAL || 
		ai_iValue==ClMIT_ChnValue::EQUAL ||  ai_iValue==ClMIT_ChnValue::NOT_EQUAL ||
		ai_iValue==ClMIT_ChnValue::GREATER || ai_iValue==ClMIT_ChnValue::GREATER_EQUAL )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestCellType( ENG_CT_CELL_TYPE ai_iValue ) 
{
	if ( ai_iValue==ENG_CT_PRE_T ||  ai_iValue==ENG_CT_PRE_S || 
		ai_iValue==ENG_CT_SUB ||  ai_iValue==ENG_CT_POST_S ||
		ai_iValue==ENG_CT_POST_T )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

/*
m4bool_t ClENG_DesignTree::TestFieldType( FIELD_TYPE ai_iValue )
{
	if ( ai_iValue==FLD_STRING || ai_iValue==FLD_MULTIL || ai_iValue==FLD_BOX || 
		ai_iValue==FLD_RASTERG || ai_iValue==FLD_BARCODE )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}
*/

m4bool_t ClENG_DesignTree::TestItemValueType(  ClMIT_ChnValue::TYPE  ai_iValue )
{
	if ( ai_iValue==ClMIT_ChnValue::NUMERIC ||  ai_iValue==ClMIT_ChnValue::STRING ||  
		ai_iValue==ClMIT_ChnValue::DATE ||  ai_iValue==ClMIT_ChnValue::BOOLEAN )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

/*
m4bool_t ClENG_DesignTree::TestSectionType(  SECTION_TYPE ai_iValue )
{
	if ( ai_iValue==END_SECTION || ai_iValue==NORMAL_SECTION || 
		ai_iValue==END_SLICE_SECTION )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}
*/

m4bool_t ClENG_DesignTree::TestSectionBreakMode(  ClENG_Section::BREAK_MODE ai_iValue )
{
	if ( ai_iValue==ClENG_Section::NONE_BREAK ||  ai_iValue==ClENG_Section::BEFORE_CONTAINER_BREAK ||  
		ai_iValue==ClENG_Section::AFTER_CONTAINER_BREAK ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSectionHeaderMode(  ClENG_Section::HEADER_MODE ai_iValue )
{
	if ( ai_iValue==ClENG_Section::ONLY_FIRST_PAGE || ai_iValue==ClENG_Section::H_ALL_PAGES || 
		ai_iValue==ClENG_Section::EXCEPT_FIRST_PAGE )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSectionFooterMode(  ClENG_Section::FOOTER_MODE ai_iValue )
{
	if ( ai_iValue==ClENG_Section::ONLY_LAST_PAGE || ai_iValue==ClENG_Section::F_ALL_PAGES || 
		ai_iValue==ClENG_Section::EXCEPT_LAST_PAGE )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

/*
m4bool_t ClENG_DesignTree::TestSecSlcTotalLink(  SEC_SLC_TOTAL_LINK_MODE ai_iValue )
{
	if ( ai_iValue==NONE_TOTAL_LINK || ai_iValue==SLICE_TOTAL_LINK ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSecSlcMergeLink(  SEC_SLC_MERGE_LINK_MODE ai_iValue )
{
	if ( ai_iValue==NONE_MERGE_LINK || ai_iValue==SLICE_MERGE_LINK || 
		ai_iValue==TOTAL_MERGE_LINK ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}
*/

m4bool_t ClENG_DesignTree::TestSecNullIfEmpty(  ClENG_VR::NULL_IF_EMPTY ai_iValue ) 
{
	if ( ai_iValue==ClENG_VR::NEVER_NULL || ai_iValue==ClENG_VR::THIS_SECTION || 
		ai_iValue==ClENG_VR::SON_OR_SECTIONS || ai_iValue==ClENG_VR::SON_AND_SECTIONS ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestFieldFillMode( ClENG_FieldStr::FILL_MODE ai_iValue ) 
{
	if ( ai_iValue==ClENG_FieldStr::NO_FILL || ai_iValue==ClENG_FieldStr::ALWAYS_FILL || 
		ai_iValue==ClENG_FieldStr::NO_EMPTY_FILL ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSecGoT( ClENG_VR::GO_THROUGH_MODE ai_iValue ) 
{
	if ( ai_iValue==ClENG_VR::RECORD_GOT || ai_iValue==ClENG_VR::SLICE_GOT )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSecRangeMode( ClENG_VR::RANGE_MODE ai_iValue ) 
{
	if ( ai_iValue==ClENG_VR::ALL_VR || ai_iValue==ClENG_VR::SUBGROUP_VR )
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestLoaderCntType( ENG_TYPE_CONTAINER ai_iValue ) 
{
	if ( 
		ai_iValue==ENG_CNT_BEFORE || ai_iValue==ENG_CNT_AFTER || 
		ai_iValue==ENG_CNT_HEADER || ai_iValue==ENG_CNT_FOOTER || 
		ai_iValue==ENG_CNT_PRERECORD || ai_iValue==ENG_CNT_POSTRECORD || 
		ai_iValue==ENG_CNT_RECORD
	) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestFieldBarCodeType( ENG_BAR_CODE_TYPE ai_iValue ) 
{
	if ( 
		ai_iValue==ENG_NO_BAR_CODE || 
		ai_iValue==ENG_BAR_CODE_39 || 
		ai_iValue==ENG_BAR_CODE_25_INTERLEAVED ||
		ai_iValue==ENG_BAR_CODE_128 || 
		ai_iValue==ENG_BAR_CODE_POSTNET ) 
	{
		return M4_TRUE ;
	} else {
		*(TM_GET_INSP(GET_TM())->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_ENUM ] ) 
			<< (m4int16_t)ai_iValue << SEND_MSG ;
		return M4_FALSE ;
	};
}

m4bool_t ClENG_DesignTree::TestSectionMultimap(ClENG_DesignData::SectionList_t *ai_poList) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4uint32_t PrevPk = 0;
	ClENG_DesignData::SectionList_t::iterator It ;

	for ( It=ai_poList->begin(); It != ai_poList->end(); ++It ) {
		if ( It!=ai_poList->begin() ) {
			if ( (*It).first == PrevPk ) {
				SwOk=M4_FALSE ;
				break ;
			} ;
		} ;
		PrevPk = (*It).first ;

		SwOk= M4_BOOL(SwOk && ClENG_DesignTree::TestOblig((*It).second.m_bIdSectionLoaded,"IdSection"));
	} ;

	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"SectionId %0:s isn't unique", ClMIT_Msg::MIXED_OUT) 
			<< PrevPk  << SEND_MSG ;
	} ;

	return SwOk ;
}

const m4char_t * ClENG_DesignTree::GetCntTraceStr( ENG_TYPE_CONTAINER ai_Enum ) 
{
	m4char_t *Str ; 

	switch (ai_Enum) 
	{
	case ENG_CNT_BEFORE:
		Str="Before" ;
		break ;
	case ENG_CNT_AFTER:
		Str="After" ;
		break ;
	case ENG_CNT_HEADER:
		Str="Header" ;
		break ;
	case ENG_CNT_FOOTER:
		Str="Footer" ;
		break ;
	case ENG_CNT_PRERECORD:
		Str="Prerecord" ;
		break ;
	case ENG_CNT_POSTRECORD:
		Str="Postrecord" ;
		break ;
	case ENG_CNT_RECORD:
		Str="Record" ;
		break ;
	//case ENG_CNT_PAGE:
	//	Str="Page" ;
	//	break ;
	default :
		Str=" " ;
		break ;
	} ;
	return Str ;
}

const m4char_t * ClENG_DesignTree::GetCellTypeTraceStr( ENG_CT_CELL_TYPE ai_Enum ) 
{
	m4char_t *Str ; 

	switch (ai_Enum) 
	{
	case ENG_CT_PRE_T:
		Str="PreTotal" ;
		break ;
	case ENG_CT_PRE_S:
		Str="PreSubtotal" ;
		break ;
	case ENG_CT_SUB:
		Str="Main" ;
		break ;
	case ENG_CT_POST_S:
		Str="PostSubtotal" ;
		break ;
	case ENG_CT_POST_T:
		Str="PostTotal" ;
		break ;
	default :
		Str=" " ;
		break ;
	} ;
	return Str ;
}


const m4char_t * ClENG_DesignTree::GetCellFieldTypeTraceStr( ENG_CT_CELL_FIELDS_TYPE ai_Enum ) 
{
	m4char_t *Str ; 
	switch (ai_Enum) 
	{
	case ENG_TEMPLATE_H_FIELDS:
		Str="Template, Header" ;
		break ;
	case ENG_TEMPLATE_R_FIELDS:
		Str="Template, Row" ;
		break ;
	case ENG_STATIC_FIELDS:
		Str="Static cell" ;
		break ;
	default :
		Str=" " ;
		break ;
	} ;
	return Str ;
}

//----------------------------------Resto de métodos

ClENG_DesignTree::ClENG_DesignTree() 
{
	m_poInsp=GET_INSP();
	m_oPageData=NULL ;
}

void ClENG_DesignTree::End() 
{
	m_oDBFont.End() ;

	m_oFormatList.clear() ;

	m_oFGPalette.End() ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_oPageData, 
			StENG_DT_PageData, m_iNumPageLayout ) ;

	m_oCTList.clear();

	m_oLE.clear() ;
	m_oTrLit.clear() ;

	m_oRootSection.End() ;

	m_oFileInterest.End() ;
}

m4bool_t ClENG_DesignTree::LoadDBFont( ClENG_DesignData *ai_poDD )
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::FontList_t::iterator It ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadDBFont(), NumFonts %0:s",
			ClMIT_Msg::ENTRY_FORMAT) 
			<< (m4uint32_t)ai_poDD->m_oFontList.size()	
			<< SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1000 ] ) << END_LOC ;

	m_oDBFont.Init( ) ;

	for ( It = ai_poDD->m_oFontList.begin() ; It != ai_poDD->m_oFontList.end(); ++It ) {

		SwOk = M4_BOOL( SwOk && NewFont( ai_poDD, (*It).m_uiIdFont ) ) ;
		if ( !SwOk ) break ;

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadDBFont()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	if (!SwOk) m_oDBFont.End() ;

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

m4bool_t ClENG_DesignTree::NewFont( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdFont )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_FontData FontInfo ;
	ClMIT_Str FontPath ;
	ClMIT_IdNumDesc IdDesc ;
	ClENG_DesignData::FontList_t::iterator It ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	//Buscar Info

	if ( ! ai_poDD->FindFont(ai_uiIdFont,It) ) {
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Font %0:s not found", ClMIT_Msg::MIXED_OUT) << ai_uiIdFont << SEND_MSG ;
		goto exit;
	} ;

	// Crear FontInfo

	SwOk = M4_BOOL( SwOk && TestFontData( &FontInfo, &(*It) ) ) ;
	if ( !SwOk ) goto exit ;
	
	IdDesc.Init( (*It).m_uiIdFont, (*It).m_oTraceName) ;
	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1010 ] ) 
		<< IdDesc  << END_LOC ;

	// Hayar path RFN

	m_poInsp->m_poSysInfo->GetValue(ENG_RK_RFN_PATH, &FontPath) ;
	ClMIT_SysInfo::AppendToPath( &FontPath, FontInfo.m_oRFNFile ) ;

	// Cargar Font

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){

		m_poInsp->m_poTrace->Indent() ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"IdFont %0:s TraceName %1:s DesignHeight %2:s points"
			"FixedSpace %3:s Underline %4:s Bold %5:s Italic %6:s RFN File in %7:s" ) 
			
			<< FontInfo.m_uiIdFont << FontInfo.m_oTraceName.InChar(0)
			<< FontInfo.m_DesignHeight << FontInfo.m_bFixedSpace << FontInfo.m_bUnderline
			<< FontInfo.m_bBold << FontInfo.m_bItalic << FontPath.InChar(0)
			
			<< DETAILED_MSG << SEND_MSG   ;

		m_poInsp->m_poTrace->Unindent(1) ;
	};
	#endif

	SwOk=M4_BOOL(SwOk&&m_oDBFont.SaveFont( 
		ClFONT_M_Font::RVW_INFO, &IdDesc, FontPath, FontInfo.m_DesignHeight, 
		FontInfo.m_bFixedSpace, FontInfo.m_bUnderline, FontInfo.m_bBold, 
		FontInfo.m_bItalic, ClMIT_Trace::PROPAGATE_ERROR)
	) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if(SwOk){
			ClFONT_M_Font *Font = m_oDBFont.GetAddressFontById((*It).m_uiIdFont) ; 
			m_poInsp->m_poTrace->Indent() ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"DesignHeight %0:s MaxHeight %1:s DesignMaxWidth %2:s MaxWidth %3:s" )   
				<< (m4uint32_t)Font->GetDvcDesignHeight()
				<< (m4uint32_t)Font->GetDvcHeight()
				<< (m4uint32_t)Font->GetDvcDesignMaxWidthChar()
				<< (m4uint32_t)Font->GetDvcMaxWidthChar()
				<< DETAILED_MSG << SEND_MSG ;
			m_poInsp->m_poTrace->Unindent() ;
		};
	};
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;

exit:
	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

m4bool_t ClENG_DesignTree::LoadFormatList( ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::FormatList_t::iterator It ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadFormatList(), NumFormats %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< (m4uint32_t)ai_poDD->m_oFormatList.size() 	
			<< SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1020 ] ) << END_LOC ;

	m_oFormatList.clear( ) ;

	for ( It = ai_poDD->m_oFormatList.begin() ; It != ai_poDD->m_oFormatList.end(); ++It ) {

		// Crear FormatInfo

		SwOk = M4_BOOL( SwOk && NewFormat(ai_poDD,(*It).m_uiIdFormat) ) ;
		if (!SwOk) break ;

	} ;

	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadFormatList()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	if (!SwOk) m_oFormatList.clear() ;

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

m4bool_t ClENG_DesignTree::NewFormat( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdFormat ) 
{
	ClENG_DesignData::FormatList_t::iterator It ;
	ClMIT_IdNumDesc IdDesc ;
	StENG_DT_FormatData FormatInfo ;
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	//Buscar Info

	if ( ! ai_poDD->FindFormat(ai_uiIdFormat,It) ) {
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Format %0:s not found", ClMIT_Msg::MIXED_OUT) << ai_uiIdFormat << SEND_MSG ;
		goto exit;
	} ;

	// Crear FormatInfo

	IdDesc.Init((*It).m_uiIdFormat,(*It).m_oFormatName) ;
	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1030 ] ) 
		<< IdDesc  << END_LOC ;
	SwOk = M4_BOOL( SwOk && FormatInfo.Init( &(*It) ) ) ;
	if ( !SwOk ) goto exit ;

	m_oFormatList.push_back(FormatInfo) ;

exit:
	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

m4bool_t ClENG_DesignTree::NewColour( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdColour )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_Colour ColourInfo;
	ClENG_DesignData::ColourList_t::iterator It ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	//Buscar Info

	if ( ! ai_poDD->FindColour(ai_uiIdColour,It) ) {
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Colour %0:s not found", ClMIT_Msg::MIXED_OUT) << ai_uiIdColour << SEND_MSG ;
		goto exit;
	} ;

	// Crear FontInfo

	SwOk = M4_BOOL( SwOk && TestColourData( &ColourInfo, &(*It) ) ) ;
	if ( !SwOk ) goto exit ;
	
	// Cargar Colour

	SwOk=M4_BOOL(SwOk&&m_oFGPalette.AddColour( 
		m_oFGPalette.GetSize(),
		ColourInfo.m_uiIdColour, ColourInfo.m_oName,
		ColourInfo.m_R, ColourInfo.m_G, ColourInfo.m_B)
	) ;

	*m_poInsp->m_poTrace << POP_LOC ;

exit:
	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}


m4bool_t ClENG_DesignTree::LoadFGPalette( ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	StENG_DD_Colour ColourData ;
	ClENG_DesignData::ColourList_t::iterator It ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadFGPalette()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	//En B deja size; testea si en la paleta esta el 0. Si lo esta, no cuenta.
	//NO testea valores, se hace luego

	for ( It = ai_poDD->m_oFGPalette.begin(), B=ai_poDD->m_oFGPalette.size()+1; 
		  It != ai_poDD->m_oFGPalette.end() ; ++It ) 
	{
		if ( (*It).m_uiIdColour==0 ) {
			--B ;
			break ;
		} ;
	} ;
	
	//Crea paleta

	if ( ! m_oFGPalette.Init(B, ClMIT_Trace::PROPAGATE_ERROR) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Mete valor negro, reservado

	m_oFGPalette.SetColour(0,0,"Black",0,0,0) ;	

	
	//Carga y testea resto de valores

	for ( B=1, It = ai_poDD->m_oFGPalette.begin(); It != ai_poDD->m_oFGPalette.end(); ++It ) {

		SwOk = M4_BOOL( SwOk && TestColourData( &ColourData, &(*It) ) ) ;
		if (!SwOk) goto exit ;

		if ( ColourData.m_uiIdColour!=0 ) {
			if ( ! m_oFGPalette.SetColour( 
				B, ColourData.m_uiIdColour, ColourData.m_oName,
				ColourData.m_R, ColourData.m_G, ColourData.m_B ) )
			{
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			++B ;
		} ;

	} ;
	
exit:
	if (!SwOk) m_oFGPalette.End() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadFGPalette()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}


m4bool_t  ClENG_DesignTree::LoadPageData( ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClENG_DesignData::PageList_t::iterator It ;

	m_iNumPageLayout = ai_poDD->m_oPages.size() ;

	if (m_iNumPageLayout==0) {
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"There isn't data for pages", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		goto exit;
	} ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_oPageData, 
		StENG_DT_PageData, m_iNumPageLayout ) ;

	for (B=0;B<m_iNumPageLayout;++B) {
		for ( It=ai_poDD->m_oPages.begin(); It != ai_poDD->m_oPages.end(); ++It ) {
			if ( (*It).m_iPage==B ) break ;
		} ;
		if ( It == ai_poDD->m_oPages.end() ) {
			SwOk=M4_FALSE;
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"There isn't page data for page %0:s", ClMIT_Msg::MIXED_OUT) << B << SEND_MSG ;
			goto exit;
		} ;
		SwOk= M4_BOOL( SwOk && m_oPageData[B].Init( &(*It), this ) ) ;
		if (!SwOk) goto exit ;
	} ;
	
exit:
	if (!SwOk) 	{
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_oPageData, 
			StENG_DT_PageData, m_iNumPageLayout ) ;
	} ;


	return SwOk ;
} 

m4bool_t ClENG_DesignTree::LoadCTData(  ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData CTData ;
	ClENG_DesignData::CTList_t::iterator It ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadCTData()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_oCTList.clear() ;	

	for ( It=ai_poDD->m_oCT.begin() ; It!=ai_poDD->m_oCT.end(); ++It ) {
		SwOk = M4_BOOL( SwOk && CTData.Init( &(*It).second, this ) ) ;
		if (!SwOk) break ;

		if ( FindCT(ENG_NOT_EXIST_REQUIRED,CTData.m_oCTId) ) {
			SwOk=M4_FALSE ;
			break ;
		} ;
		m_oCTList.insert( ClENG_DesignTree::CTList_t::value_type(CTData.m_oCTId,CTData) ) ;
	} ;

	if (!SwOk) m_oCTList.clear() ;	

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadCTData()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}

m4bool_t ClENG_DesignTree::LoadCT_T_Data(  ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData CTData ;
	ClENG_DesignData::CTList_t::iterator ItDD ;
	StENG_DT_CTData *CT_DT ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadCT_T_Data()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	for ( ItDD=ai_poDD->m_oCT.begin() ; ItDD!=ai_poDD->m_oCT.end(); ++ItDD ) {

		CT_DT = FindCT( ENG_EXIST_REQUIRED, (*ItDD).second.m_oCTId) ;
		if ( !CT_DT ) {
			SwOk=M4_FALSE ;
			break ;
		} ;
		SwOk = M4_BOOL( SwOk &&  CT_DT->LoadTemplate( &(*ItDD).second, this ) ) ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadCT_T_Data()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}

m4bool_t ClENG_DesignTree::LoadLE(  ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::LEList_t::iterator DDIt ;
	ClENG_Analex::LEMap_t::iterator DTIt ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadLE()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_oLE.clear() ;

	for ( DDIt = ai_poDD->m_oLE.begin() ; DDIt != ai_poDD->m_oLE.end(); ++DDIt ) {
		DTIt = m_oLE.find( (*DDIt).first ) ;
		if ( DTIt != m_oLE.end() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Long expression %0:s duplicated", ClMIT_Msg::MIXED_OUT) 
				<< (*DDIt).first.InChar(0) << SEND_MSG ;
			break ;
		} ;
		m_oLE.insert( ClENG_Analex::LEMap_t::value_type( (*DDIt).first, (*DDIt).second ) ) ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadLE()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;

}

m4bool_t ClENG_DesignTree::LoadTrLit(  ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::TrLitList_t::iterator DDIt ;
	StENG_ReportEnv::TrLitMap_t::iterator DTIt ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadTrLit()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_oTrLit.clear() ;

	for ( DDIt = ai_poDD->m_oTrLit.begin() ; DDIt != ai_poDD->m_oTrLit.end(); ++DDIt ) {
		DTIt = m_oTrLit.find( (*DDIt).first ) ;
		if ( DTIt != m_oTrLit.end() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"TrLit %0:s duplicated", ClMIT_Msg::MIXED_OUT) 
				<< (*DDIt).first.InChar(0) << SEND_MSG ;
			break ;
		} ;
		m_oTrLit.insert( StENG_ReportEnv::TrLitMap_t::value_type( (*DDIt).first, (*DDIt).second ) ) ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadTrLit()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}

m4bool_t ClENG_DesignTree::LoadPageContainers(  ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;
	ClENG_DesignData::PageList_t::iterator It ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadPageContainers()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	for (B=0; B < m_iNumPageLayout ; ++B ) {

		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1060 ] ) 
			<< (m4int16_t)(B+1) << END_LOC ;

		for ( It=ai_poDD->m_oPages.begin(); It != ai_poDD->m_oPages.end(); ++It ) {
			if ( (*It).m_iPage==B ) break ;
		} ;
		if ( It == ai_poDD->m_oPages.end() ) {
			SwOk=M4_FALSE;
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"There isn't page data for page %0:s", ClMIT_Msg::MIXED_OUT) << B << SEND_MSG ;
			goto exit;
		} ;
		
		SwOk = M4_BOOL( SwOk && m_oPageData[B].m_oCnt.Init( &(*It).m_oCnt, ENG_PAGE_CONTAINER, 
			NULL, this ) ) ;
		if (!SwOk) break ;

		*m_poInsp->m_poTrace << POP_LOC ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::LoadPageContainers()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}


m4bool_t ClENG_DesignTree::LoadTreeSection( 
		ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdSection,
		StENG_DT_SectionData *ao_poSection, m4bool_t ai_bSwSync, m4int16_t ai_iExeOrder ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;
	m4int16_t  B ;
	StENG_DD_SectionData *Section_DD ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadTreeSection Sec %0:s Sync %1:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_uiIdSection << ai_bSwSync << SEND_MSG   ;
	};
	#endif

	// Carga datos de la propia seccion

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< ai_uiIdSection << END_LOC ;

	Section_DD = ai_poDD->FindSection(ENG_EXIST_REQUIRED,ai_uiIdSection) ;
	if ( ! Section_DD  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	SwOk = M4_BOOL( SwOk && ao_poSection->Init( Section_DD, this) ) ;
	if ( !SwOk ) goto exit ;

	*m_poInsp->m_poTrace << POP_LOC << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< ai_uiIdSection << END_LOC ;

	// Carga datos de secciones hijas.

	for ( B=0; B<ao_poSection->m_iSonSectionCount; ++B ) {
		SwOk = M4_BOOL( SwOk && LoadTreeSection( ai_poDD, 
			ao_poSection->m_oSonSection[B].m_uiIdSonSection,
			&ao_poSection->m_oSonSection[B].m_oSection,
			ao_poSection->m_oSonSection[B].m_bIsSync,
			ao_poSection->m_oSonSection[B].m_iExeOrder ) ) ;
		if (!SwOk) break ;
	} ;
	
exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadTreeSection Sec %0:s Sync %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< ai_uiIdSection << ai_bSwSync << SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}



DvcUnit_t ClENG_DesignTree::GetDvcUnit( m4double_t ai_Metric ) 
{
	return m_oGlobal.m_ModeMetric==ENG_MILIM ? ClMetric::MilimToDvc(ai_Metric) :
		ClMetric::InchToDvc(ai_Metric/10) ;
}

m4double_t ClENG_DesignTree::GetDesignUnit(DvcUnit_t ai_Metric) 
{
	return m_oGlobal.m_ModeMetric==ENG_MILIM ? ClMetric::DvcToMilim(ai_Metric) :
		ClMetric::DvcToInch(ai_Metric/10) ;
}


m4bool_t  ClENG_DesignTree::Init( 	ClENG_DesignData *ai_poDD, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::Init(), creating a design tree", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	ClENG_DesignTree::End() ;

	//******Hereda interes

	m_oFileInterest.Init( &ai_poDD->m_oFileInterest ) ;
	
	//******Test unicidad SectionId

	SwOk = M4_BOOL( SwOk && TestSectionMultimap(&ai_poDD->m_oSection) ) ;
	if (!SwOk) goto exit ;

	//******Monta la DBFont

	SwOk = M4_BOOL( SwOk && LoadDBFont( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Crea la lista de formatos

	SwOk = M4_BOOL( SwOk && LoadFormatList( ai_poDD ) ) ;
	if (!SwOk) goto exit ;
	
	//******Crea la paleta de colores

	SwOk = M4_BOOL( SwOk && LoadFGPalette( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Carga la lista LE 

	SwOk = M4_BOOL( SwOk && LoadLE( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Carga la lista TrLit
	
	SwOk = M4_BOOL( SwOk && 	LoadTrLit( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//****** A partir de aqui DBFont y FormatListdeben estar disponibles para Defaults.

	//******Datos globales report 
	
	SwOk = M4_BOOL( SwOk && m_oGlobal.Init( &ai_poDD->m_oGlobal, 
		ai_poDD->m_bGlobalLoaded, this ) ) ;
	if (!SwOk) goto exit ;
	
	//******Crea los datos de pagina
	// Ya deben estar disponibles defectos de report

	SwOk = M4_BOOL( SwOk && LoadPageData( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Carga los datos generales de CT
	//Se cargan antes de cualquier container (o field), para que esten disponibles en ese 
	//	contexto.

	SwOk = M4_BOOL( SwOk && LoadCTData( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Carga los containers de pagina
	// Ya deben estar creados los datos de pagina.

	SwOk = M4_BOOL( SwOk && LoadPageContainers( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

	//******Carga arbol de secciones de report

	SwOk = M4_BOOL( SwOk && LoadTreeSection( ai_poDD, m_oGlobal.m_iIdRootSection,  &m_oRootSection,
		m_oGlobal.m_bSwRootSync, 0  ) ) ;
	if (!SwOk) goto exit ;

	//******Carga template de CT
	//Deben cargarse cuando las secciones ya esten cargadas

	SwOk = M4_BOOL( SwOk && LoadCT_T_Data( ai_poDD ) ) ;
	if (!SwOk) goto exit ;

exit:

	if (!SwOk) End() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::Init(), return %0:s ", 
			ClMIT_Msg::EXIT_FORMAT) <<  SwOk << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk ;
}


StENG_DT_CTData *ClENG_DesignTree::FindCT(ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCT, 
		CTList_t::iterator *ao_It) 
{
	CTList_t::iterator It ;
	StENG_DT_CTData *R ;
		
	It = m_oCTList.find(ai_pcIdCT) ;
	R = It!=m_oCTList.end() ? &(*It).second : NULL ;
 
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"CT %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCT << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"CT %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCT << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

StENG_DT_SectionData *ClENG_DesignTree::FindSection(ENG_FIND_ACTION ai_Action, m4uint32_t ai_uiIdSection ) 
{
	StENG_DT_SectionData *R ;
		
	R = m_oRootSection.FindSection(ai_uiIdSection) ;
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Section %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_uiIdSection << SEND_MSG ;
		};
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Section %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_uiIdSection << SEND_MSG ;
		} ;
		break ;
	} ;
	return R ;
}

m4uint32_t ClENG_DesignTree::CountFontInstances( m4uint32_t ai_uiIdFont ) 
{
	m4uint32_t R ;

	CountFontFormatColourInstances( ai_uiIdFont, 0, 0, &R, NULL, NULL ) ;
	return R ;
}

m4uint32_t ClENG_DesignTree::CountFormatInstances( m4uint32_t ai_uiIdFormat ) 
{
	m4uint32_t R ;

	CountFontFormatColourInstances( 0, ai_uiIdFormat, 0, NULL, &R, NULL ) ;
	return R ;
}

m4uint32_t ClENG_DesignTree::CountColourInstances( m4uint32_t ai_uiIdColour ) 
{
	m4uint32_t R ;

	CountFontFormatColourInstances( 0, 0, ai_uiIdColour, NULL, NULL, &R ) ;
	return R ;
}

void ClENG_DesignTree::CountFontFormatColourInstances( 
	m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
	m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
	m4uint32_t *aoi_puiColourCount ) 
{
	m4int16_t B ;
	CTList_t::iterator CT ;

	if ( ! aoi_puiFontCount && ! aoi_puiFormatCount && ! aoi_puiColourCount ) return ;

	if ( aoi_puiFontCount )		*aoi_puiFontCount=0 ;
	if ( aoi_puiFormatCount )	*aoi_puiFormatCount=0 ;
	if ( aoi_puiColourCount )	*aoi_puiColourCount=0;

	//***Contabiliza globales de report

	m_oGlobal.m_oReportDefaults.IncrCountFontFormatColourInstances(
		ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

	//***Contabiliza Layout Pages

	for (B=0; B<m_iNumPageLayout; ++B) {

		m_oPageData[B].IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

	} ;

	//***Contabiliza arbol de secciones

	m_oRootSection.IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

	//***Contabiliza CTs

	for( CT=m_oCTList.begin(); CT!=m_oCTList.end(); ++CT ) {
		(*CT).second.IncrCountFontFormatColourInstances(
			ai_uiIdFont,ai_uiIdFormat,ai_uiIdColour,aoi_puiFontCount,aoi_puiFormatCount, aoi_puiColourCount) ;

	} ;
}

void ClENG_DesignTree::GetFontsNotUsed(  ENG_ULongList_t *ai_poFont ) 
{
	m4int16_t B ;
	ClFONT_M_Font *F ;

	ai_poFont->clear() ;

	for ( B=0; B < m_oDBFont.GetNumFonts(); ++B ) {
		F = m_oDBFont.GetAddressFontByIndex(B) ;
		if ( CountFontInstances( F->GetId() ) == 0 ) {
			ai_poFont->push_back( F->GetId() ) ;
		} ;
	} ;
}

void ClENG_DesignTree::GetFormatsNotUsed(ENG_ULongList_t *ai_poFormat ) 
{
	m4int16_t B ;
	FormatList_t::iterator It ;

	ai_poFormat->clear() ;

	for ( It=m_oFormatList.begin(); It!=m_oFormatList.end(); ++It ) {
		if ( CountFormatInstances( (*It).m_uiIdFormat ) == 0 ) {
			ai_poFormat->push_back( (*It).m_uiIdFormat ) ;
		} ;
	} ;
}

void ClENG_DesignTree::GetColoursNotUsed(  ENG_ULongList_t *ai_poColour ) 
{
	m4int16_t B ;
	ClENG_Colour *Colour ;

	ai_poColour->clear() ;

	for ( B=0; B < m_oFGPalette.GetSize(); ++B ) {
		Colour = m_oFGPalette.GetColourByIndex(B) ;
		if ( CountColourInstances( Colour->GetId() ) == 0 ) {
			ai_poColour->push_back( Colour->GetId() ) ;
		} ;
	} ;
}


//================================= GESTION DEPENDENCIAS ==================

m4bool_t ClENG_DesignTree::ManageDependencies(m4bool_t *ao_bSwError, m4bool_t ai_bSwRebuildDep, ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData)
{
	m4bool_t SwOk=M4_TRUE;
	ClENG_Section *Root;
	StENG_TaskEnv *TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_DesignTree::ManageDependencies()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	SwOk=ReportDependenciesGo(ai_poDep, ai_poChnData, ai_bSwRebuildDep);
	//Comprobamos que hay RootSection
	if(m_oRootSection.m_iIdSection!=ENG_SECION_ID_EMPTY) {
		SwOk=TreeSectionDependencies(&m_oRootSection, ai_poDep, ai_poChnData, ai_bSwRebuildDep);
	};
	
exit:
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::ManageDependencies()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	*ao_bSwError = M4_BOOL(!SwOk);
	return SwOk  ;

}


m4bool_t ClENG_DesignTree::ReportDependenciesGo(ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData, m4bool_t ai_bSwRebuildDep)
{
	m4int16_t PushState=m_poInsp->m_poTrace->GetPushState();
	m4bool_t SwOk=M4_TRUE;
	ClENG_Analex::StExpressionEnv StEnv;
	
	StEnv.m_poDependencies = ai_poDep;
	StEnv.m_poChn = ai_poChnData;

    #ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::ReportDependenciesGo", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	//DEPENDENCIAS DE REPORT
	ai_poDep->UpdatePosition(ClENG_StorageDep::REPORT, 0, 0, 0, 0);
	ai_poDep->DeleteAllDependencies();
	SwOk = M4_BOOL( SwOk && ReportDependencies(ai_poDep, &StEnv) ) ;
	if(!SwOk) goto exit;
	SwOk = M4_BOOL( SwOk && CrosstabDependencies(ai_poDep, ai_poChnData) ) ;
	if(!SwOk) goto exit;
	//FIN DE DEPENDENCIAS DE REPORT

	//DEPENDENCIAS DE FIELDS DE PAGINA
	SwOk = M4_BOOL( SwOk && PageFieldDependencies(ai_poDep, &StEnv, ai_bSwRebuildDep) );
	//FIN DE DEPENDENCIAS DE FIELDS DE PAGINA

exit:

    #ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::ReportDependenciesGo", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState);

	return SwOk;
};


m4bool_t ClENG_DesignTree::ReportDependencies(ClENG_StorageDep *ai_poDep, ClENG_Analex::StExpressionEnv *ai_pStEnv)
{
	ClENG_Analex Analex;
	ClENG_Expression oExpr;
	m4bool_t SwOk=M4_TRUE;

	//FILE_NAME && FILE_PATH

	oExpr.Init(m_oGlobal.m_stDefaultFileParams.m_oPath.InChar(0), "Path expression");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) ) ;

	return SwOk;
}


m4bool_t ClENG_DesignTree::CrosstabDependencies(ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData)
{
	m4bool_t SwOk=M4_TRUE;
	ClMIT_Args FillArg;
	m4int32_t iCounter;
	ClMIT_Str StrNode, StrItem;
	ClMIT_ChnNode *Node, *Node2;
	ClMIT_ChnItem Item;
	CTList_t::iterator CTIt;

	//DEPENDENCIAS DE CROSSTAB
	for(CTIt=m_oCTList.begin(); CTIt!=m_oCTList.end(); ++CTIt){
		
		FillArg.Init((*CTIt).second.m_oFillTree, '.', " ," ) ;

		for (iCounter=0; iCounter<FillArg.GetArgCount(); ++iCounter) {
		FillArg.GetString(iCounter, 0, &StrNode) ;
				
			Node = ai_poChnData->GetNode(StrNode,ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!Node) {
				SwOk=M4_FALSE ;
				goto exit ;
			};
				
			SwOk = M4_BOOL( SwOk && ai_poDep->StoreNodeDependency(Node->GetM4ObjId(), StrNode.InChar(0)) ) ;
			if (!SwOk) goto exit ;
				
			SwOk=M4_BOOL( SwOk&&FillArg.GetString(iCounter, 1, &StrItem, M4_FALSE) ) ;
			if (!SwOk) goto exit ;
			ai_poDep->StoreItemDependency(Node->GetTiId(), StrItem.InChar(0));
			SwOk= M4_BOOL( SwOk && (Item.Init(Node, StrItem, ClMIT_Trace::PROPAGATE_ERROR ) == ClMIT_ChnItem::ITEM_OK ) ) ;
			if (!SwOk) goto exit ;

			if ( FillArg.GetSubArgCount(iCounter)!=2  ) {
				if (FillArg.GetSubArgCount(iCounter)>3) {
					SwOk= M4_BOOL( SwOk&&FillArg.GetString(iCounter, 2, &StrNode, M4_FALSE) ) ;
					Node2=ai_poChnData->GetNode(StrNode, ClMIT_Trace::PROPAGATE_ERROR) ;
					if (!Node2) {
						SwOk=M4_FALSE ;
						goto exit ;
					};

					SwOk = M4_BOOL( SwOk && ai_poDep->StoreNodeDependency(Node2->GetM4ObjId(), StrNode.InChar(0)));
					if (!SwOk) goto exit ;
		
					SwOk= M4_BOOL( SwOk&&FillArg.GetString(iCounter,3,&StrItem,M4_FALSE) ) ;
				} else {
					Node2 = Node;
					SwOk= M4_BOOL( SwOk&&FillArg.GetString(iCounter,2,&StrItem,M4_FALSE) ) ;
				} ;		
				if (!SwOk) goto exit ;

				ai_poDep->StoreItemDependency(Node2->GetTiId(), StrItem.InChar(0));
				SwOk= M4_BOOL( SwOk && (Item.Init(Node2, StrItem, ClMIT_Trace::PROPAGATE_ERROR ) == ClMIT_ChnItem::ITEM_OK ) ) ;
				if (!SwOk) goto exit;
			} ;
		} ;

	};
	//FIN DE DEPENDENCIAS DE CROSSTAB*/

exit:

	return SwOk;
};

m4bool_t ClENG_DesignTree::PageFieldDependencies(ClENG_StorageDep *ai_poDep, ClENG_Analex::StExpressionEnv *ai_pStEnv, m4bool_t ai_bSwRebuildDep)
{
	ClENG_Analex Analex;
	ClENG_Expression oExpr;
	ClMIT_Str ExprOrigen;
	m4bool_t SwOk=M4_TRUE, SwFieldFirst, SwFieldEnd, SwFirst, SwEnd;
	m4int16_t iCounter;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	ENG_DT_FieldList_t::iterator FieldIt; 
	
	for(iCounter=0; iCounter<m_iNumPageLayout; ++iCounter) {

		if(!m_oPageData->m_oCnt.m_oFieldList.size()) continue;
 		
		for(FieldIt=m_oPageData->m_oCnt.m_oFieldList.begin(); FieldIt!=m_oPageData->m_oCnt.m_oFieldList.end(); ++FieldIt){
			
			//Borra todos los registros de dependencias si el Field ha sido modificado o la
			// opcion de lanzamiento es REBUILD_DEPENDENCES
			ai_poDep->UpdatePosition(ClENG_StorageDep::PAGE_FLD, iCounter+1, iCounter, ENG_CNT_BEFORE, (*FieldIt).m_uiIdField);
			
			if (ai_bSwRebuildDep || (*FieldIt).m_bIsModified || m_oLE.size()>0){
				ai_poDep->DeleteAllDependencies();
				ClENG_Analex::PrepareForAnalexCTAndField( 
					(*FieldIt).m_oExpr, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
				oExpr.Init(ExprOrigen.InChar(0), "PageField");
				SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );
			};
		};
	};

	return SwOk;
}


m4bool_t ClENG_DesignTree::TreeSectionDependencies(
	StENG_DT_SectionData *ai_poSection, ClENG_StorageDep *ai_poDep, 
	ClMIT_Chn *ai_poChnData, m4bool_t ai_bSwRebuildDep)
{
	m4int16_t PushState=m_poInsp->m_poTrace->GetPushState();
	m4bool_t SwOk=M4_TRUE;
	m4int16_t iCounter;
	ClENG_Analex::StExpressionEnv StEnv;
	ClENG_FieldsManager FieldManager;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	
	FieldManager.Init();
	
	StEnv.m_poFieldManager = &FieldManager;
	StEnv.m_poDependencies = ai_poDep;
	StEnv.m_poChn = ai_poChnData;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< (m4uint32_t)ai_poSection->m_iIdSection << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::TreeSectionDependencies Sec %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_poSection->m_iIdSection << SEND_MSG   ;
	};
	#endif

	//Get Node por defecto
	StEnv.m_poDefaultNode = ai_poChnData->GetNode(ai_poSection->m_RecordsetNodeName, ClMIT_Trace::PROPAGATE_ERROR);
	if (!StEnv.m_poDefaultNode) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Borra todos los registros de dependencias si el Field ha sido modificado o la
	// opcion de lanzamiento es REBUILD_DEPENDENCES
	ai_poDep->UpdatePosition(ClENG_StorageDep::SECTION, 0, ai_poSection->m_iIdSection, 0, 0);
	if (ai_bSwRebuildDep || ai_poSection->m_bIsModified || m_oLE.size()>0){
		ai_poDep->DeleteAllDependencies();
		SwOk = M4_BOOL( SwOk && SectionDependencies(ai_poSection, StEnv.m_poDefaultNode->GetM4ObjId(), ai_poDep, &StEnv, ai_poChnData) );
	};
	//FIN DE DEPENDENCIAS DE SECCION

		
	//DEPENDENCIAS DE CONTAINER Y CONTAINER FIELDS
	SwOk = M4_BOOL( SwOk && CntFieldDependencies (ai_poSection, ai_poDep, &StEnv, ai_bSwRebuildDep) ) ;
	//FIN DE DEPENDENCIAS DE CONTAINER Y CONTAINER FIELDS


	//DEPENDENCIAS RECURSIVAS DE LAS SECCIONES HIJAS
	for (iCounter=0; iCounter < ai_poSection->m_iSonSectionCount; ++iCounter) {
		SwOk = M4_BOOL( SwOk && TreeSectionDependencies(&ai_poSection->m_oSonSection[iCounter].m_oSection, 
			ai_poDep, ai_poChnData, ai_bSwRebuildDep) ) ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DesignTree::TreeSectionDependencies Sec %0:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< ai_poSection->m_iIdSection << SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState);

	return SwOk;
}

m4bool_t ClENG_DesignTree::SectionDependencies(
	StENG_DT_SectionData *ai_poSection,
	const m4char_t* ai_pcIdT3, ClENG_StorageDep *ai_poDep, 
	ClENG_Analex::StExpressionEnv *ai_pStEnv, ClMIT_Chn * ai_poChnData)
{
	ClENG_Analex Analex;
	ClENG_Expression oExpr;
	ClENG_VR::OneOneList_t::iterator Item;
	ClMIT_Args Arg;
	m4int16_t iCounter;
	m4bool_t SwOk=M4_TRUE;
	ClMIT_Str ExprOrigen, StrAux;
	ClMIT_ChnNode * pNode = NULL;
	m4char_t * pNodeId = NULL;

	//ID_NODE
	SwOk = ai_poDep->StoreNodeDependency(ai_pcIdT3, ai_poSection->m_RecordsetNodeName);
	if (!SwOk) goto exit ;

	//CANCEL_SECTION
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oCancelSection, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"CancelSection");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) ) ;

	//RESET_BEF_TOTALS
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oResetBeforeTotal, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"ResetBeforeTotal");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//PREPARE_RECORDSET
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oPrepareRecordset, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"PrepareRecordset");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//RESET_HEA_TOTALS
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oResetHeaderTotals, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"ResetHeaderTotals");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//CANCEL_RECORD
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oCancelVR, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"CancelVR");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//SET_TOTALS
	ClENG_Analex::PrepareForAnalexCTAndField(ai_poSection->m_oSetPostVRTotal, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	oExpr.Init(ExprOrigen.InChar(0),"PostVRTota");
	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//SEC_SLC_GUIDE_ITEM
	//ClENG_Analex::PrepareForAnalexCTAndField(StrAux, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
	//oExpr.Init(ExprOrigen.InChar(0));
	//SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );

	//GROUP
	Arg.Init(ai_poSection->m_oGroupKey, ':', " ,");
	for (iCounter=0; iCounter<Arg.GetArgCount(); ++iCounter) {
		ExprOrigen.StrCpy(Arg.GetArg(iCounter));
		ClENG_Analex::PrepareForAnalexCTAndField(StrAux, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
		oExpr.Init(ExprOrigen.InChar(0),"GroupKey");
		SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );
	} ;

	//SEC_ONE_ONE
	Arg.Init(ai_poSection->m_oOneOneStr, ':', " ,");
	for (iCounter=0; iCounter<Arg.GetArgCount(); ++iCounter)
	{
		ExprOrigen.StrCpy(Arg.GetArg(iCounter));

		// Bug 0160252: Obtener el M4O padre real del nodo.
		pNodeId = ExprOrigen.InChar(0);
		pNode = ai_poChnData->GetNode(pNodeId, ClMIT_Trace::PROPAGATE_ERROR);
		if (pNode == NULL)
		{
			SwOk = M4_FALSE;
			goto exit;
		}
		
		SwOk = M4_BOOL(SwOk && ai_poDep->StoreNodeDependency(pNode->GetM4ObjId(), pNodeId));
		if (!SwOk) goto exit;
	} ;
	
exit:

	return SwOk;
}


m4bool_t ClENG_DesignTree::CntFieldDependencies(StENG_DT_SectionData *ai_poSection, ClENG_StorageDep *ai_poDep, 
												ClENG_Analex::StExpressionEnv *ai_pStEnv, m4bool_t ai_bSwRebuildDep)
{
	StENG_DT_SectionData::CntList_t::iterator CntIt;
	ENG_DT_FieldList_t::iterator FldIt;
	ClENG_Analex Analex;
	ClENG_Expression oExpr;
	ClMIT_Str ExprOrigen;
	m4bool_t SwOk=M4_TRUE;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	//Para todos los containers
	for(CntIt=ai_poSection->m_oCnt.begin(); CntIt!=ai_poSection->m_oCnt.end(); ++CntIt){
		if(!(*CntIt).second.m_oFieldList.size()) continue;
	
		//Borra todos los registros de dependencias si el Container ha sido modificado o la
		// opcion de lanzamiento es REBUILD_DEPENDENCES
		ai_poDep->UpdatePosition(ClENG_StorageDep::CONTAIN, 0, ai_poSection->m_iIdSection, (*CntIt).first, 0);
		if (ai_bSwRebuildDep || (*CntIt).second.m_bIsModified || m_oLE.size()>0){
			//DEPENDENCIAS DE CONTAINER
			ai_poDep->DeleteAllDependencies();
			ClENG_Analex::PrepareForAnalexCTAndField((*CntIt).second.m_oCancelCnt, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
			oExpr.Init(ExprOrigen.InChar(0),"CancelCnt");
			SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) );
			//FIN DEPENDENCIAS DE CONTAINER
		};

		for(FldIt=(*CntIt).second.m_oFieldList.begin(); FldIt!=(*CntIt).second.m_oFieldList.end(); ++FldIt){
		
			//Borra todos los registros de dependencias si el Field ha sido modificado o la
			// opcion de lanzamiento es REBUILD_DEPENDENCES
			ai_poDep->UpdatePosition(ClENG_StorageDep::FIELD, 0, ai_poSection->m_iIdSection, (*CntIt).first, (*FldIt).m_uiIdField);
			if (ai_bSwRebuildDep || (*FldIt).m_bIsModified || m_oLE.size()>0){
				//DEPENDENCIAS DE CONTAINER FIELDS
				ai_poDep->DeleteAllDependencies();
				ClENG_Analex::PrepareForAnalexCTAndField((*FldIt).m_oExpr, "0", ai_pStEnv, &ExprOrigen, M4_TRUE ) ;
				oExpr.Init(ExprOrigen.InChar(0),"Field");
				SwOk = M4_BOOL( SwOk && Analex.Init(ai_pStEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR) ) ;
				//FIN DEPENDENCIAS DE CONTAINER FIELDS
			};
		};
	};

exit:

	return SwOk;
}

//============================= FIN GESTION DEPENDENCIAS ==================

//========================================================================Testeos pendientes

/*
	...
	
	Item.Init(m_poNodeSec,"ID_SECTION") ;
	if ( ! Item.FindDouble(ai_uiIdSection) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADERCN_MSG_30 ] ) 
			<< (m4int32_t)ai_uiIdSection << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	
	  CONTAINER: 


  DATOS CT:

	Ver en DT Container, datos CT


  NO, parametro auxiliar
	//D:M4_FALSE.
	//Si true, todas las paginas m_iNumPagLayout son clones de la primera.
	//Es un valor auxiliar de jecución, puede cambiar si un CT necesita más páginas Layout.

	m4bool_t m_bReplicatePages ;				m4bool_t m_bReplicatePagesLoaded ;

Intanciar report desde un DT



Levantar MVC

 */
 


//================================= ClENG_StorageDep ======================

ClENG_StorageDep::ClENG_StorageDep()
{
	m_poInsp=GET_INSP();
}

void ClENG_StorageDep::UpdatePosition(DEP_TYPE ai_DepType, m4int16_t ai_IdPage, m4uint32_t ai_uIdSection, m4uint32_t ai_uIdContainer, m4uint32_t ai_uIdField)
{
	m_DepType = ai_DepType;
	m_uIdPage = ai_IdPage;
	m_uIdSection = ai_uIdSection;
	m_uIdContainer = ai_uIdContainer;
	m_uIdField = ai_uIdField;
}
