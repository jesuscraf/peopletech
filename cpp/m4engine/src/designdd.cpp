
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designdd.cpp
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

#include "clargs.h"

#include "designdd.h"
//#include "designdd.hmg"

#define GROUP_ID    ENG_TGI_LOADER


#define ENG_DD_COPY_STR(Value,Flag)   Flag=ai_poSource->Flag; if(Flag) Value.StrCpy(ai_poSource->Value)
#define ENG_DD_COPY_VALUE(Value,Flag) Flag=ai_poSource->Flag; if(Flag) Value = ai_poSource->Value

//=============================================================================StENG_DD_Defaults


void StENG_DD_Defaults::Reset()
{
	m_SwIdFontLoaded = 	m_SwIdFormatLoaded = m_SwAlignYLoaded = m_SwAlignXLoaded = 
		m_SwClipLoaded = m_SwGraphicsLoaded = m_SwFormatNULLLoaded = m_SwDefDisplayNULLLoaded =
		m_SwIdColourLoaded = M4_FALSE ;
}

void StENG_DD_Defaults::Init(const StENG_DD_Defaults *ai_poSource) 
{

	ENG_DD_COPY_VALUE(	m_uiIdFont,			m_SwIdFontLoaded	) ;

	ENG_DD_COPY_VALUE(	m_uiIdFormat,		m_SwIdFormatLoaded	) ;

	ENG_DD_COPY_VALUE(	m_AlignY,			m_SwAlignYLoaded	) ;

	ENG_DD_COPY_VALUE(	m_AlignX,			m_SwAlignXLoaded	) ;

	ENG_DD_COPY_VALUE(	m_bSwClip ,			m_SwClipLoaded	) ;

	ENG_DD_COPY_VALUE(	m_bSwGraphics,		m_SwGraphicsLoaded	) ;

	ENG_DD_COPY_VALUE(	m_bSwFormatNULL,	m_SwFormatNULLLoaded	) ;

	ENG_DD_COPY_STR(	m_oDefDisplayNULL,	m_SwDefDisplayNULLLoaded) ;

	ENG_DD_COPY_VALUE(	m_uiIdColour,		m_SwIdColourLoaded	) ;

}


//=========================================================================StENG_DD_GlobalReport

void StENG_DD_GlobalReport::Reset()
{
	m_bReportNameLoaded = m_bDataChnLoaded = m_bDataChnReprinterLoaded = m_bModeMetricLoaded =
		m_bIdRootSectionLoaded = m_bSwRootSyncLoaded = m_bBreakModeLoaded =
		m_bIdBreakSectionLoaded = m_bTotalizeListLoaded = m_bCTListLoaded = 
		m_bExecutionParsLoaded = m_bIgnoreTxtWarningLoaded = M4_FALSE ;

	m_oReportDefaults.Reset() ;

	m_stDefaultFileParams.Init() ;
}

void StENG_DD_GlobalReport::Init( const StENG_DD_GlobalReport *ai_poSource ) 
{
	ENG_DD_COPY_STR(	m_oReportName,			m_bReportNameLoaded) ;

	ENG_DD_COPY_STR(	m_oDataChn,				m_bDataChnLoaded) ;

	ENG_DD_COPY_STR(	m_oDataChnReprinter,	m_bDataChnReprinterLoaded) ;

	ENG_DD_COPY_VALUE(	m_ModeMetric,			m_bModeMetricLoaded	) ;
	
	ENG_DD_COPY_VALUE(	m_iIdRootSection,		m_bIdRootSectionLoaded	) ;

	ENG_DD_COPY_VALUE(	m_bSwRootSync,			m_bSwRootSyncLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_BreakMode,			m_bBreakModeLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_iIdBreakSection,		m_bIdBreakSectionLoaded 	) ;

	ENG_DD_COPY_STR(	m_oTotalizeList,		m_bTotalizeListLoaded ) ;

	ENG_DD_COPY_STR(	m_oCTList,				m_bCTListLoaded ) ;

	ENG_DD_COPY_STR(	m_oExecutionPars,		m_bExecutionParsLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bIgnoreTxtWarning,	m_bIgnoreTxtWarningLoaded 	) ;

	m_oReportDefaults.Init(&ai_poSource->m_oReportDefaults) ;

	m_stDefaultFileParams.Init(&ai_poSource->m_stDefaultFileParams) ;
}


//=========================================================================StENG_DD_Colour


void StENG_DD_Colour::Reset() 
{
	m_bIdColourLoaded = m_bNameLoaded = m_b_R_Loaded = m_b_G_Loaded = m_b_B_Loaded = M4_FALSE ;

}

void StENG_DD_Colour::Init( const StENG_DD_Colour *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_uiIdColour,		m_bIdColourLoaded 	) ;

	ENG_DD_COPY_STR(	m_oName,			m_bNameLoaded ) ;
	
	ENG_DD_COPY_VALUE(	m_R,				m_b_R_Loaded 	) ;

	ENG_DD_COPY_VALUE(	m_G,				m_b_G_Loaded 	) ;

	ENG_DD_COPY_VALUE(	m_B,				m_b_B_Loaded 	) ;
}

//=========================================================================StENG_DD_FontData


void StENG_DD_FontData::Reset() 
{
	m_bIdFontLoaded = m_bDesignHeightLoaded = m_bFixedSpaceLoaded = m_bRFNFileLoaded  =
		m_bTraceNameLoaded = m_bUnderlineLoaded = m_bBoldLoaded =  m_bItalicLoaded =
		M4_FALSE ;

}

void StENG_DD_FontData::Init( const StENG_DD_FontData *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_uiIdFont,				m_bIdFontLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_DesignHeight,			m_bDesignHeightLoaded 	) ;
	
	ENG_DD_COPY_VALUE(	m_bFixedSpace,			m_bFixedSpaceLoaded 	) ;

	ENG_DD_COPY_STR(	m_oRFNFile,				m_bRFNFileLoaded ) ;
	
	ENG_DD_COPY_STR(	m_oTraceName,			m_bTraceNameLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bUnderline,			m_bUnderlineLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_bBold,				m_bBoldLoaded 	) ;
	
	ENG_DD_COPY_VALUE(	m_bItalic,				m_bItalicLoaded 	) ;

}

//===========================================================================StENG_DD_FormatData

void StENG_DD_FormatData::Reset() 
{
	m_bIdFormatLoaded = m_bFormatNameLoaded = m_bIdCurrencyLoaded = M4_FALSE;
	m_oProperties.clear() ;
}

void StENG_DD_FormatData::Init( const StENG_DD_FormatData *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_uiIdFormat,			m_bIdFormatLoaded 	) ;

	ENG_DD_COPY_STR(	m_oFormatName,			m_bFormatNameLoaded ) ;

	ENG_DD_COPY_STR(	m_oIdCurrency,			m_bIdCurrencyLoaded ) ;

	m_oProperties = ai_poSource->m_oProperties ;

}

//=============================================================================StENG_DD_PageData

void StENG_DD_PageData::Reset() 
{
	m_bPageLoaded = m_bOffsetXLoaded = m_bOffsetYLoaded = m_bPageSizeLoaded  = 
		m_bOrientationLoaded = m_bSizeXLoaded = m_bSizeYLoaded = m_bCntLoaded = M4_FALSE ;

	m_stDefaultData.Reset() ;
	m_oCnt.Reset() ;
}


void StENG_DD_PageData::Init( const StENG_DD_PageData *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_iPage,			m_bPageLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_dOffsetX,			m_bOffsetXLoaded 	) ;
	
	ENG_DD_COPY_VALUE(	m_dOffsetY,			m_bOffsetYLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_PageSize,			m_bPageSizeLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_Orientation,		m_bOrientationLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_dSizeX,			m_bSizeXLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_dSizeY,			m_bSizeYLoaded 	) ;

	m_stDefaultData.Init(&ai_poSource->m_stDefaultData) ;

	m_bCntLoaded=ai_poSource->m_bCntLoaded;

	if ( m_bCntLoaded ) {
		m_oCnt.Init( &ai_poSource->m_oCnt ) ;
	} else {
		m_oCnt.Reset() ;
	} ;
}

//=============================================================================StENG_DD_CTData

void StENG_DD_CTData::Reset() 
{
	m_bCTIdLoaded = m_bNameLoaded = m_bTotListLoaded = m_bTreeListLoaded = 
		m_bFillTreeLoaded = m_bIdParamSecLoaded = m_bMergePageLoaded  = 
		m_bXColumnLeftLoaded = m_bXColumnRightLoaded = m_bTogLevelLoaded = m_bDerivedListLoaded =
		m_bFirstCrossRowLoaded = m_bWizardDataLoaded = m_bIsModified = M4_FALSE ;

	m_oParamSec.clear() ;
	m_oCellList.clear() ;
}

void StENG_DD_CTData::Init( const StENG_DD_CTData *ai_poSource ) 
{
	ENG_DD_COPY_STR(	m_oCTId,			m_bCTIdLoaded 	) ;

	ENG_DD_COPY_STR(	m_oName,			m_bNameLoaded 	) ;

	ENG_DD_COPY_STR(	m_oTotList,			m_bTotListLoaded 	) ;

	ENG_DD_COPY_STR(	m_oTreeList,		m_bTreeListLoaded 	) ;

	ENG_DD_COPY_STR(	m_oFillTree,		m_bFillTreeLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_uiIdParamSec,		m_bIdParamSecLoaded ) ;

	ENG_DD_COPY_VALUE(	m_MergePage,		m_bMergePageLoaded ) ;

	ENG_DD_COPY_VALUE(	m_dXColumnLeft,		m_bXColumnLeftLoaded ) ;
	
	ENG_DD_COPY_VALUE(	m_dXColumnRight,	m_bXColumnRightLoaded ) ;

	ENG_DD_COPY_VALUE(	m_iTogLevel,		m_bTogLevelLoaded ) ;

	ENG_DD_COPY_STR(	m_oWizardData,		m_bWizardDataLoaded ) ;

	ENG_DD_COPY_STR(	m_oDerivedList,		m_bDerivedListLoaded 	) ;

	ENG_DD_COPY_VALUE(	m_iFirstCrossRow,	m_bFirstCrossRowLoaded ) ;
		
	m_oCellList = ai_poSource->m_oCellList ;
	m_oParamSec = ai_poSource->m_oParamSec ;

	m_bIsModified = ai_poSource->m_bIsModified;
}

StENG_DD_CTCell *StENG_DD_CTData::FindCell(ENG_FIND_ACTION ai_Action, 
		m4int16_t ai_iLevel, ENG_CT_CELL_TYPE ai_Type, CellList_t::iterator *ao_It) 
{
	CellList_t::iterator It ;
	StENG_DD_CTCell *R ;
		
	for ( It=m_oCellList.begin(), R=NULL; It != m_oCellList.end(); ++It ) {
		if ( (*It).m_bLevelLoaded && (*It).m_bTypeLoaded ) {
			if ( (*It).m_iLevel == ai_iLevel && (*It).m_Type == ai_Type ) {
				R = &(*It) ;
				break ;
			} ;
		} ;
	} ;
	
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Cell %0:s, level %1:s,  not found", ClMIT_Msg::MIXED_OUT) 
				<< (m4int16_t)ai_Type << ai_iLevel << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Cell %0:s, level %1:s exist", ClMIT_Msg::MIXED_OUT) 
				<< (m4int16_t)ai_Type << ai_iLevel << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

m4bool_t StENG_DD_CTData::GetParamSec(m4int16_t ai_Level, m4uint32_t *ao_poParamSec) 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_IdSectionList_t::iterator It ;

	for ( It = m_oParamSec.begin(); ai_Level && It != m_oParamSec.end() ; --ai_Level, ++It ) ;
	if ( It==m_oParamSec.end() ) {
		/*##Num error*/
		*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"ParamSec for level %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_Level << SEND_MSG ;
		return M4_FALSE ;
	} else {
		*ao_poParamSec = (*It) ;
		return M4_TRUE ;
	} ;
}

//=====================================================================StENG_DD_SonSectionData

void StENG_DD_SonSectionData::Reset() 
{
	m_bIdSonSectionLoaded = m_bExeOrderLoaded = 
		m_bIsSyncLoaded = M4_FALSE ;
}

void StENG_DD_SonSectionData::Init( const StENG_DD_SonSectionData *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_uiIdSonSection,		m_bIdSonSectionLoaded ) ;
	ENG_DD_COPY_VALUE(	m_iExeOrder,			m_bExeOrderLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bIsSync,				m_bIsSyncLoaded ) ;
}

//=====================================================================StENG_DD_SectionData 

void StENG_DD_SectionData::Reset() 
{
	m_bIdSectionLoaded = m_bSectionNameLoaded = m_bNoPrintEmptyLoaded = 
		m_bBeforePBreakLoaded = m_bAfterPBreakLoaded = m_bHeaderModeLoaded =
		m_bFooterModeLoaded = m_bGoTLoaded = m_bRangeModeLoaded = m_bGroupKeyLoaded =
		m_bNextRecNodeLoaded = m_bSwBreakVRLoaded = m_bGuideNodeLoaded = 
		m_bGuideItemNameLoaded = m_bNullOnSonsLoaded = m_bOneOneStrLoaded = 	
		m_bCancelSectionLoaded =  m_bResetBeforeTotalLoaded =  m_bPrepareRecordsetLoaded = 
		m_bResetHeaderTotalsLoaded = m_bCancelVRLoaded = m_bSetPostVRTotalLoaded =
		m_bTogetherLoaded = m_bIsModified = M4_FALSE ;

	m_oSonList.clear() ;

	m_oCnt.clear() ;

	m_stDefaultData.Reset() ;
}

void StENG_DD_SectionData ::Init( const StENG_DD_SectionData  *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_iIdSection,		m_bIdSectionLoaded ) ;

	ENG_DD_COPY_STR(	m_oSectionName,		m_bSectionNameLoaded ) ;

	ENG_DD_COPY_VALUE(	m_NoPrintEmpty,		m_bNoPrintEmptyLoaded ) ;

	ENG_DD_COPY_VALUE(	m_BeforePBreak,		m_bBeforePBreakLoaded ) ;

	ENG_DD_COPY_VALUE(	m_AfterPBreak,		m_bAfterPBreakLoaded ) ;

	ENG_DD_COPY_VALUE(	m_HeaderMode,		m_bHeaderModeLoaded ) ;

	ENG_DD_COPY_VALUE(	m_FooterMode,		m_bFooterModeLoaded ) ;

	ENG_DD_COPY_VALUE(	m_GoT,				m_bGoTLoaded ) ;

	ENG_DD_COPY_VALUE(	m_RangeMode,		m_bRangeModeLoaded ) ;

	ENG_DD_COPY_STR(	m_oGroupKey,		m_bGroupKeyLoaded ) ;

	ENG_DD_COPY_STR(	m_RecordsetNodeName,		m_bNextRecNodeLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bSwBreakVR,		m_bSwBreakVRLoaded ) ;

	ENG_DD_COPY_STR(	m_GuideNode,		m_bGuideNodeLoaded ) ;

	ENG_DD_COPY_STR(	m_GuideItemName,	m_bGuideItemNameLoaded ) ;
	
	ENG_DD_COPY_VALUE(	m_bNullOnSons,		m_bNullOnSonsLoaded ) ;

	ENG_DD_COPY_STR(	m_oOneOneStr,		m_bOneOneStrLoaded ) ;

	ENG_DD_COPY_VALUE(	m_oCancelSection,		m_bCancelSectionLoaded ) ;
	ENG_DD_COPY_VALUE(	m_oResetBeforeTotal,	m_bResetBeforeTotalLoaded ) ;
	ENG_DD_COPY_VALUE(	m_oPrepareRecordset,	m_bPrepareRecordsetLoaded ) ;
	ENG_DD_COPY_VALUE(	m_oResetHeaderTotals,	m_bResetHeaderTotalsLoaded ) ;
	ENG_DD_COPY_VALUE(	m_oCancelVR,			m_bCancelVRLoaded ) ;
	ENG_DD_COPY_VALUE(	m_oSetPostVRTotal,		m_bSetPostVRTotalLoaded ) ;

	ENG_DD_COPY_STR(	m_oTogether,			m_bTogetherLoaded ) ;
	
	m_oSonList	= ai_poSource->m_oSonList ;

	m_oCnt		= ai_poSource->m_oCnt ;

	m_bIsModified = ai_poSource->m_bIsModified;

	m_stDefaultData.Init(&ai_poSource->m_stDefaultData) ;
}

StENG_DD_CntData *StENG_DD_SectionData::FindCnt(ENG_FIND_ACTION ai_Action, ENG_TYPE_CONTAINER ai_CntId, 
		CntList_t::iterator *ao_It) 
{
	CntList_t::iterator It ;
	StENG_DD_CntData *R ;
		
	It = m_oCnt.find(ai_CntId) ;
	R = It!=m_oCnt.end() ? &(*It).second : NULL ;
 
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Cnt %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< (m4int16_t)ai_CntId << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*(GET_INSP())->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Cnt %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< (m4int16_t)ai_CntId << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

void StENG_DD_SectionData::DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) 
{
	CntList_t::iterator It ;

	for ( It=m_oCnt.begin(); It!=m_oCnt.end(); ++It ) {
		(*It).second.DeleteReferencesToIdCTField( ai_pcCT, ai_pcIdCTField ) ;
	} ;
}

//=====================================================================StENG_DD_CntData

void StENG_DD_CntData::Reset() 
{
	m_bPageLoaded = m_bTopLoaded =  m_bLeftLoaded = m_bHeightLoaded = m_bWidthLoaded =
		m_bIsFixedLoaded = m_bSwTogetherLoaded = m_bLowerLimitLoaded = m_bCTMergeLoaded =
		m_bCancelCntLoaded = m_bIsModified = 
		m_bRegionColourLoaded = m_bRegionBorderLoaded = M4_FALSE ;

	m_oFieldList.clear() ;

	m_stDefaultData.Reset() ;
}

void StENG_DD_CntData::Init( const StENG_DD_CntData *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_iPage,		m_bPageLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dTop,			m_bTopLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dLeft,		m_bLeftLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dHeight,		m_bHeightLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dWidth,		m_bWidthLoaded ) ;
	ENG_DD_COPY_VALUE(	m_IsFixed,		m_bIsFixedLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bSwTogether,	m_bSwTogetherLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dLowerLimit,	m_bLowerLimitLoaded ) ;
	ENG_DD_COPY_STR(	m_oCTMerge,		m_bCTMergeLoaded ) ;
	ENG_DD_COPY_STR(	m_oCancelCnt,	m_bCancelCntLoaded ) ;
	ENG_DD_COPY_VALUE(	m_uiRegionColour,		m_bRegionColourLoaded ) ;
	ENG_DD_COPY_STR(	m_oRegionBorder,		m_bRegionBorderLoaded ) ;

	m_oFieldList =  ai_poSource->m_oFieldList ;

	m_bIsModified = ai_poSource->m_bIsModified;

	m_stDefaultData.Init( &ai_poSource->m_stDefaultData ) ;
}

void StENG_DD_CntData::GetCTId(ClMIT_Str *ao_CTId) 
{
	ClMIT_Args Arg ;

	if ( m_bCTMergeLoaded ) {
		Arg.Init(m_oCTMerge, ClMIT_Args::NO_SUBARG, ".") ;
		if ( Arg.GetArgCount()==0 ) {
			ao_CTId->StrCpy("") ;
		} else {
			Arg.GetString(0,0,ao_CTId) ;
		} ;

	} else {
		ao_CTId->StrCpy("") ;
	} ;
}

void StENG_DD_CntData::DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) 
{
	ENG_DD_FieldList_t::iterator It, ItNext ;
	ClMIT_Str StrAux ;
	
	GetCTId(&StrAux) ;

	if ( ClMIT_Str::StrICmpTrim(ai_pcCT,StrAux) ==0 ) {

		for ( It=m_oFieldList.begin() ; It!=m_oFieldList.end(); It=ItNext ) {
		
			ItNext = It ; ++ItNext ;	//It puede ser borrado

			if ( ! (*It).m_bIdCtFieldLoaded ) continue ;

			if ( ClMIT_Str::StrICmpTrim( ai_pcIdCTField, (*It).m_oIdCtField ) ==0 ) {
				m_oFieldList.erase( It ) ;
			} ;
		} ;
	} ;
}

m4uint32_t StENG_DD_CntData::GenerateNewIdField( ENG_DD_FieldList_t *ai_poFieldList ) 
{
	m4uint32_t Key ;
	ENG_DD_FieldList_t::iterator It ;

	Key = ai_poFieldList->size() ; 
	do {
		for ( It=ai_poFieldList->begin(); It!=ai_poFieldList->end(); ++It ) {
			if ( ! (*It).m_bIdFieldLoaded ) continue ;
			if ( (*It).m_uiIdField == Key ) break ;
		} ;
		if ( It!=ai_poFieldList->end() ) ++Key ;
	} while ( It!=ai_poFieldList->end() )  ;

	return Key ;
}


void StENG_DD_CntData::GetMinimalHeight(m4double_t *ai_dMinHeight)
{
	ENG_DD_FieldList_t::iterator FieldIt;

	//Controlamos la anchura
	for ( FieldIt=m_oFieldList.begin(); FieldIt!=m_oFieldList.end(); ++FieldIt ) {
		if(*ai_dMinHeight<((*FieldIt).m_dHeight + (*FieldIt).m_dTop)) {
			*ai_dMinHeight = (*FieldIt).m_dHeight + (*FieldIt).m_dTop;
		};
	} ;
}


//=====================================================================StENG_DD_CTCell

void StENG_DD_CTCell::Reset() 
{
	m_bHeightLoaded = m_bWidthLoaded = m_bTypeLoaded = m_bLevelLoaded = 
		m_bIsModified = m_bIdParamSecLoaded = 
		m_bRegionColourLoaded = m_bRegionBorderLoaded = 
		m_bAlignParamCellYLoaded = m_bAlignParamCellXLoaded = M4_FALSE ;
	m_oFieldList.clear() ;
}

void StENG_DD_CTCell::GetMinimalWidth(m4double_t *ai_dMinWidth)
{
	ENG_DD_FieldList_t::iterator FieldIt;

	//Controlamos la anchura
	for ( FieldIt=m_oFieldList.begin(); FieldIt!=m_oFieldList.end(); ++FieldIt ) {
		if(*ai_dMinWidth<((*FieldIt).m_dWidth + (*FieldIt).m_dLeft)) {
			*ai_dMinWidth = (*FieldIt).m_dWidth + (*FieldIt).m_dLeft;
		};
	} ;
}

void StENG_DD_CTCell::Init( const StENG_DD_CTCell *ai_poSource ) 
{
	ENG_DD_COPY_VALUE(	m_uiIdParamSec,	m_bIdParamSecLoaded ) ;
	ENG_DD_COPY_VALUE(	m_Type,			m_bTypeLoaded ) ;
	ENG_DD_COPY_VALUE(	m_iLevel,		m_bLevelLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dWidth,		m_bWidthLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dHeight,		m_bHeightLoaded ) ;
	ENG_DD_COPY_VALUE(	m_uiRegionColour,		m_bRegionColourLoaded ) ;
	ENG_DD_COPY_STR(	m_oRegionBorder,		m_bRegionBorderLoaded ) ;
	ENG_DD_COPY_VALUE(	m_AlignParamCellY,		m_bAlignParamCellYLoaded ) ;
	ENG_DD_COPY_VALUE(	m_AlignParamCellX,		m_bAlignParamCellXLoaded ) ;

	m_oFieldList =  ai_poSource->m_oFieldList ;

	m_bIsModified = ai_poSource->m_bIsModified;

}

//=====================================================================StENG_DD_FieldData

void StENG_DD_FieldData::Reset() 
{
	m_bIdFieldLoaded = m_bFieldNameLoaded = m_bFldTypeLoaded = m_bTopLoaded = m_bLeftLoaded =
		m_bWidthLoaded = m_bHeightLoaded = m_bSaveNodeLoaded = m_bSaveItemNameLoaded = 
		m_bSwStaticLoaded = m_bIdPiLoaded = m_bSwOptimizeLoaded = m_bExprLoaded = 
		m_bLiteralValueLoaded = m_bInterlineLoaded = m_bIsVarHeightLoaded = 
		m_bIsJustifiedLoaded = m_bMaxBoxesLoaded = m_bFillModeLoaded = m_bFillCharLoaded = 
		m_bSwDuplicatedLoaded = m_bIdCtFieldLoaded = m_bBarCodeTypeLoaded =
		m_bEmbebedRasterLoaded = m_bBNRasterLoaded = m_bColourRasterLoaded = 
		m_bRegionColourLoaded = m_bRegionBorderLoaded= m_bIsModified = 
		m_bIndexExprLoaded = m_bIndexLevelLoaded = m_bTempWebImageLoaded = 
		m_bHyperlinkLoaded = m_bX1Loaded = m_bX2Loaded = m_bY1Loaded = m_bY2Loaded =
		m_bAdjustLineToContainerLoaded = M4_FALSE  ;
	m_stDefaultData.Reset() ;
}

void StENG_DD_FieldData::Init( const StENG_DD_FieldData *ai_poSource ) 
{

	ENG_DD_COPY_VALUE(	m_uiIdField ,		m_bIdFieldLoaded ) ;
	ENG_DD_COPY_STR(	m_oFieldName,		m_bFieldNameLoaded ) ;
	ENG_DD_COPY_VALUE(	m_FldType ,			m_bFldTypeLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dTop ,			m_bTopLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dLeft ,			m_bLeftLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dWidth,			m_bWidthLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dHeight ,			m_bHeightLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dX1 ,				m_bX1Loaded ) ;
	ENG_DD_COPY_VALUE(	m_dX2 ,				m_bX2Loaded ) ;
	ENG_DD_COPY_VALUE(	m_dY1 ,				m_bY1Loaded ) ;
	ENG_DD_COPY_VALUE(	m_dY2 ,				m_bY2Loaded ) ;
	ENG_DD_COPY_STR(	m_oSaveNode,		m_bSaveNodeLoaded ) ;
	ENG_DD_COPY_STR(	m_oSaveItemName,	m_bSaveItemNameLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bSwStatic ,		m_bSwStaticLoaded ) ;
	ENG_DD_COPY_STR(	m_oIdPi,			m_bIdPiLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bSwOptimize ,		m_bSwOptimizeLoaded ) ;
	ENG_DD_COPY_STR(	m_oExpr,			m_bExprLoaded ) ;
	ENG_DD_COPY_STR(	m_oLiteralValue,	m_bLiteralValueLoaded ) ;
	ENG_DD_COPY_VALUE(	m_dInterline ,		m_bInterlineLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bIsVarHeight ,	m_bIsVarHeightLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bIsJustified ,	m_bIsJustifiedLoaded ) ;
	ENG_DD_COPY_VALUE(	m_iMaxBoxes ,		m_bMaxBoxesLoaded ) ;
	ENG_DD_COPY_VALUE(	m_FillMode ,		m_bFillModeLoaded ) ;
	ENG_DD_COPY_VALUE(	m_cFillChar ,		m_bFillCharLoaded ) ;
	ENG_DD_COPY_VALUE(	m_bSwDuplicated,	m_bSwDuplicatedLoaded ) ;
	ENG_DD_COPY_STR(	m_oIdCtField,		m_bIdCtFieldLoaded ) ;
	ENG_DD_COPY_VALUE(	m_BarCodeType,		m_bBarCodeTypeLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bEmbebedRaster,	m_bEmbebedRasterLoaded ) ;
	ENG_DD_COPY_STR(	m_oBNRaster,		m_bBNRasterLoaded ) ;
	ENG_DD_COPY_STR(	m_oColourRaster,	m_bColourRasterLoaded ) ;

	ENG_DD_COPY_VALUE(	m_uiRegionColour,	m_bRegionColourLoaded ) ;
	ENG_DD_COPY_STR(	m_oRegionBorder,	m_bRegionBorderLoaded ) ;
	
	ENG_DD_COPY_STR(	m_oIndexExpr,		m_bIndexExprLoaded ) ;
	ENG_DD_COPY_VALUE(	m_iIndexLevel,		m_bIndexLevelLoaded ) ;

	ENG_DD_COPY_STR(	m_oHyperlink,		m_bHyperlinkLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bTempWebImage,	m_bTempWebImageLoaded ) ;

	ENG_DD_COPY_VALUE(	m_bAdjustLineToContainer, m_bAdjustLineToContainerLoaded ) ;

	m_bIsModified = ai_poSource->m_bIsModified;

	m_stDefaultData.Init( &ai_poSource->m_stDefaultData ) ;
}


//=============================================================================ClENG_DesignData

ClENG_DesignData::ClENG_DesignData() 
{
	m_poInsp=GET_INSP();
}

void ClENG_DesignData::End() 
{
	m_bGlobalLoaded = M4_FALSE ;

	DeleteFontList() ;
	DeleteFormatList() ;
	DeleteFGPalette() ;
	DeletePageData() ;
	DeleteCTList() ;
	DeleteSectionList() ;
	DeleteLEList() ;
	DeleteTrLitList() ;
	m_oFileInterest.End() ;
}


void ClENG_DesignData::Init( ) 
{
	ClENG_DesignData::End();
}

void ClENG_DesignData::AddLE(m4char_t *ai_pcId, m4char_t *ai_pcExpr) 
{
	ClMIT_Str Id, Expr ;

	Id.StrCpy(ai_pcId) ;
	Expr.StrCpy(ai_pcExpr) ;

	m_oLE.insert( LEList_t::value_type(Id,Expr) ) ;
}

void ClENG_DesignData::AddCT(StENG_DD_CTData *ai_poData) 
{
	m_oCT.insert( CTList_t::value_type( ai_poData->m_oCTId,*ai_poData) ); 
}


void ClENG_DesignData::AddTrLit(m4char_t *ai_pcId, m4char_t *ai_pcLit) 
{
	ClMIT_Str Id, Lit ;

	Id.StrCpy(ai_pcId) ;
	Lit.StrCpy(ai_pcLit) ;

	m_oTrLit.insert( TrLitList_t::value_type(Id,Lit) ) ;
}


void ClENG_DesignData::AddSection(StENG_DD_SectionData *ai_poData) 
{ 
	m_oSection.insert( SectionList_t::value_type( ai_poData->m_iIdSection,*ai_poData) ); 
}


StENG_DD_SectionData *ClENG_DesignData::FindSection(
	ENG_FIND_ACTION ai_Action, m4uint32_t ai_uiIdSection, SectionList_t::iterator *ao_It) 
{
	SectionList_t::iterator It ;
	StENG_DD_SectionData *R ;
		
	It = m_oSection.find(ai_uiIdSection) ;
	R = It!=m_oSection.end() ? &(*It).second : NULL ;
 
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Section %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_uiIdSection << SEND_MSG ;
		} ;
		break ;
	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Section %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_uiIdSection << SEND_MSG ;
		} ;
		break ;
	} ;
	if (ao_It) *ao_It = It ;
	return R ;
}

StENG_DD_CTData *ClENG_DesignData::FindCT(ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCT, 
		CTList_t::iterator *ao_It) 
{
	CTList_t::iterator It ;
	StENG_DD_CTData *R ;
		
	It = m_oCT.find(ai_pcIdCT) ;
	R = It!=m_oCT.end() ? &(*It).second : NULL ;
 
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


m4bool_t ClENG_DesignData::DeleteSection(m4uint32_t ai_uiIdSection) 
{
	SectionList_t::iterator It ;
		
	if ( FindSection( ENG_EXIST_REQUIRED, ai_uiIdSection, &It)  ) {
		m_oSection.erase(It) ;
		return M4_TRUE ;
	} else {
		return M4_FALSE ;
	} ;
}


void ClENG_DesignData::DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) 
{
	SectionList_t::iterator It ;

	for ( It=m_oSection.begin(); It!=m_oSection.end(); ++It ) {
		(*It).second.DeleteReferencesToIdCTField(ai_pcCT,ai_pcIdCTField) ;
	} ;
}

m4bool_t ClENG_DesignData::FindFont(m4uint32_t ai_uiIdFont, ClENG_DesignData::FontList_t *ai_poFontList, ClENG_DesignData::FontList_t::iterator &ao_It) 
{
	for (ao_It=ai_poFontList->begin(); ao_It!=ai_poFontList->end(); ++ao_It) {
		if ( ! (*ao_It).m_bIdFontLoaded ) continue ;
		if ( (*ao_It).m_uiIdFont == ai_uiIdFont ) {
			break ;
		} ;
	} ;
	return ao_It!=ai_poFontList->end() ;
}
	
m4bool_t ClENG_DesignData::FindFormat(m4uint32_t ai_uiIdFormat, ClENG_DesignData::FormatList_t *ai_poFormatList, ClENG_DesignData::FormatList_t::iterator &ao_It) 
{
	for (ao_It=ai_poFormatList->begin(); ao_It!=ai_poFormatList->end(); ++ao_It) {
		if ( ! (*ao_It).m_bIdFormatLoaded ) continue ;
		if ( (*ao_It).m_uiIdFormat == ai_uiIdFormat ) {
			break ;
		} ;
	} ;
	return ao_It!=ai_poFormatList->end() ;
}

m4bool_t ClENG_DesignData::FindColour(m4uint32_t ai_uiIdColour, ClENG_DesignData::ColourList_t *ai_poColourList, ClENG_DesignData::ColourList_t::iterator &ao_It) 
{
	for (ao_It=ai_poColourList->begin(); ao_It!=ai_poColourList->end(); ++ao_It) {
		if ( ! (*ao_It).m_bIdColourLoaded ) continue ;
		if ( (*ao_It).m_uiIdColour == ai_uiIdColour ) {
			break ;
		} ;
	} ;
	return ao_It!=ai_poColourList->end() ;
}

m4bool_t ClENG_DesignData::DeleteFont( m4uint32_t ai_uiIdFont ) 
{
	m4bool_t SwOk=M4_TRUE ;
	FontList_t::iterator It ;

	if ( ! FindFont( ai_uiIdFont, It)  ) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Can not Delete, Font %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_uiIdFont << SEND_MSG ;
		SwOk=M4_FALSE ;
	} else {
		m_oFontList.erase(It) ;
	} ;
	return SwOk ;
} 

m4bool_t ClENG_DesignData::DeleteFormat( m4uint32_t ai_uiIdFormat ) 
{
	m4bool_t SwOk=M4_TRUE ;
	FormatList_t::iterator It ;

	if ( ! FindFormat( ai_uiIdFormat, It)  ) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Can not Delete, Format %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_uiIdFormat << SEND_MSG ;
		SwOk=M4_FALSE ;
	} else {
		m_oFormatList.erase(It) ;
	} ;
	return SwOk ;
}

m4bool_t ClENG_DesignData::DeleteColour( m4uint32_t ai_uiIdColour ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ColourList_t::iterator It ;

	if ( ! FindColour( ai_uiIdColour, It)  ) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Can not Delete, Colour %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_uiIdColour << SEND_MSG ;
		SwOk=M4_FALSE ;
	} else {
		m_oFGPalette.erase(It) ;
	} ;
	return SwOk ;
} 