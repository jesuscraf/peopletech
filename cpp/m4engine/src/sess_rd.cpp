//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sess_rd.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
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

#include "clargs.h"
#include "analex.h"	//Solo para uso de funcs estaticas!

#include "sess_rd.h"

#define GROUP_ID    ENG_TGI_LOADER

#define ENG_VB_STR_CPY(pcTo,Size,pcFrom) ClMIT_Str::SafeStrCpy( pcTo, *Size, pcFrom ) ; *Size = strlen(pcTo) ;

//====================================================================ClENG_SessReportDesigner

ClENG_SessReportDesigner::ClENG_SessReportDesigner()
{
	m_poInsp=GET_INSP();
}

void ClENG_SessReportDesigner::End()
{
	m_oDesignChn.End() ;	
	m_oDD.End() ;
	m_oDT.End() ;	
}

m4bool_t ClENG_SessReportDesigner::Init(
		m4char_t *ai_pcReportId, m4int16_t ai_iExecutionType,
		ENG_LINK_DESIGN_MODE ai_LinkDesign, ClAccess *ai_poAccess,
		ClMIT_Chn::AUTOLOAD_MODE ai_AutoLoadMode,
		m4bool_t ai_bPack,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t iMetadataLang=-1;
	ClMIT_Str oChannelID;

	oChannelID.StrCpy("");
	ClENG_SessReportDesigner::End() ;

	SwOk = M4_BOOL ( SwOk && m_oDesignChn.Init( 
		ai_pcReportId, ai_iExecutionType, ai_LinkDesign, iMetadataLang, oChannelID.InChar(0), ai_poAccess, ai_AutoLoadMode,
		ai_bPack, ClMIT_Trace::PROPAGATE_ERROR ) ) ; 
	if (!SwOk) goto exit ;

	m_oDD.Init() ;
	m_oDesignChn.LoadAll(&m_oDD) ;

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) goto exit ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 
	return SwOk ;
}

StENG_DT_CTCell * ClENG_SessReportDesigner::FindCT_T_CellInDT(
	m4char_t *ai_pcCTId, m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CellType, 
	StENG_DT_CTData **ao_poCT) 
{
	StENG_DT_CTData * CTAux ;
	StENG_DT_CTCell * R ;

	if (!ao_poCT) ao_poCT= &CTAux ;

	*ao_poCT = m_oDT.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( ! *ao_poCT )  {
		R=NULL ;
	} else {
		R = (*ao_poCT)->m_oTemplate.GetCell(ai_Level,ai_CellType);
	} ;

	return R ;
}

StENG_DD_CTCell *ClENG_SessReportDesigner::FindCT_T_CellInDD(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CellType,
		StENG_DD_CTData **ao_poCT, StENG_DD_CTData::CellList_t::iterator *ao_It) 
{
	StENG_DD_CTData * CTAux ;
	StENG_DD_CTCell * R ;

	if (!ao_poCT) ao_poCT= &CTAux ;

	*ao_poCT = m_oDD.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( ! *ao_poCT )  {
		R=NULL ;
	} else {
		R = (*ao_poCT)->FindCell(ENG_EXIST_REQUIRED,ai_Level,ai_CellType,ao_It);
	} ;
	return R ;
}


StENG_DT_FieldData * ClENG_SessReportDesigner::FindFieldInDT(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber ) 
{
	StENG_DT_FieldData *R=NULL ;
	ENG_DT_FieldList_t *List=NULL ;
	ENG_DT_FieldList_t::iterator It;
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;

	switch (ai_Owner) {
	case 'T':
		{
			StENG_DT_CTCell *Cell ;

			SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_iLoc2 ) ) ;
			if (SwOk) {
				Cell = FindCT_T_CellInDT( ai_pcCTId, ai_uiLoc1, (ENG_CT_CELL_TYPE)ai_iLoc2) ;
				if ( ! Cell )  {
					List=NULL;
				} else {
					List=&Cell->m_oFieldList ;
				} ; 
			} ;
		} ;

		break ;
	case 'S':
		{
			StENG_DT_SectionData *Sect ;
			StENG_DT_CntData *Cnt ;

			SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestLoaderCntType( (ENG_TYPE_CONTAINER)ai_iLoc2 ) ) ;
			if (SwOk) {
				Sect = m_oDT.FindSection(ENG_EXIST_REQUIRED,ai_uiLoc1) ;
				if (!Sect) {
					List=NULL ;
				} else {
					Cnt = Sect->FindCnt( ENG_EXIST_REQUIRED, (ENG_TYPE_CONTAINER)ai_iLoc2 ) ;
					if ( ! Cnt )  {
						List=NULL;
					} else {
						List=&Cnt->m_oFieldList ;
					} ; 
				} ;
			} ;
		} ;
		break ;

	default:
		/*##Cambiar num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Invalid Field Owner value %0:s ", ClMIT_Msg::MIXED_OUT) 
			<< ai_Owner << SEND_MSG ;
		List=NULL ;
		break ;
	} ;

	if ( List ) {
		for ( It = List->begin(), B=ai_iFieldNumber ; It != List->end() && B ; ++It, --B ) ;
		if ( It==List->end() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Field %0:s  not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_iFieldNumber << SEND_MSG ;
			R=NULL ;
		} else {
			R=&(*It) ;
		} ;
	} ;
		
	return R ;
}

StENG_DD_FieldData * ClENG_SessReportDesigner::FindFieldInDD(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber,
		ENG_DD_FieldList_t **ao_poList, ENG_DD_FieldList_t::iterator *ao_poIt ) 
{
	StENG_DD_FieldData *R=NULL ;
	ENG_DD_FieldList_t *List=NULL ;
	ENG_DD_FieldList_t::iterator It;
	m4int16_t B ;

	List = FindFieldListInDD(ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2) ;

	if ( List ) {
		for ( It = List->begin(), B=ai_iFieldNumber ; It != List->end() && B ; ++It, --B ) ;
		if ( It==List->end() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Field %0:s  not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_iFieldNumber << SEND_MSG ;
			R=NULL ;
		} else {
			if ( ao_poIt )  *ao_poIt=It ;
			R=&(*It) ;
		} ;
	} ;
	
	if (ao_poList) *ao_poList = List ;

	return R ;
}

ENG_DD_FieldList_t * ClENG_SessReportDesigner::FindFieldListInDD(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2 ) 
{
	ENG_DD_FieldList_t *List=NULL ;

	switch (ai_Owner) {
	case 'T':
		{
			StENG_DD_CTCell *Cell ;

			if ( ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_iLoc2 ) ) 
			{
				Cell = FindCT_T_CellInDD( ai_pcCTId, ai_uiLoc1, (ENG_CT_CELL_TYPE)ai_iLoc2) ;
				if ( Cell )  {
					List=&Cell->m_oFieldList ;
				} ; 
			} ;
		} ;

		break ;
	case 'S':
		{
			StENG_DD_SectionData *Sect ;
			StENG_DD_CntData *Cnt ;

			if ( ClENG_DesignTree::TestLoaderCntType( (ENG_TYPE_CONTAINER)ai_iLoc2 ) )
			{
				Sect = m_oDD.FindSection(ENG_EXIST_REQUIRED,ai_uiLoc1) ;
				if (Sect) {
					Cnt = Sect->FindCnt( ENG_EXIST_REQUIRED, (ENG_TYPE_CONTAINER)ai_iLoc2 ) ;
					if ( Cnt )  {
						List=&Cnt->m_oFieldList ;
					} ; 
				} ;
			} ;
		} ;
		break ;

	default:
		/*##Cambiar num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Invalid Field Owner value %0:s ", ClMIT_Msg::MIXED_OUT) 
			<< ai_Owner << SEND_MSG ;
		break ;
	} ;

	return List ;
}

m4bool_t ClENG_SessReportDesigner::GetDefaultValues(
	StENG_DT_Defaults *ai_poDefaults,
	m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize )
{
	ClENG_VB_Serialize Serial ;

	Serial.Init() ;

	Serial.SerializeULong(		ai_poDefaults->GetIdFont() ) ; 
	Serial.SerializeULong(		ai_poDefaults->GetIdFormat() ) ;
	Serial.SerializeInt(		ai_poDefaults->GetAlignY() ) ; 
	Serial.SerializeInt(		ai_poDefaults->GetAlignX() ) ; 
	Serial.SerializeBool(		ai_poDefaults->IsClip() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsGraphicPath() ) ; 
	Serial.SerializeBool(		ai_poDefaults->IsFormatNULL() )  ; 
	Serial.SerializeStr(		ai_poDefaults->GetDefDisplayNULL() ) ;
	Serial.SerializeULong(		ai_poDefaults->GetIdColour() ) ; 

	Serial.SerializeBool(		ai_poDefaults->IsFontInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsFormatInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsAlignYInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsAlignXInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsClipInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsGraphicsInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsIdColourInherit() ) ; 
	Serial.SerializeBool(		ai_poDefaults->IsSwFormatNULLInherit() ) ;
	Serial.SerializeBool(		ai_poDefaults->IsDefDisplayNULLInherit() ) ;

	return Serial.SaveToStr( ao_pcDefaultSerialOut, aoi_puiDefaultSerialOutSize ) ;
}


void ClENG_SessReportDesigner::SetDefaultValues(

	StENG_DD_Defaults *ao_poDefaults,
	m4char_t *ai_pcDefaultSerialIn )
{
	m4uint32_t ai_uiIdFont, ai_uiIdFormat ;
	m4int16_t ai_iAlignY, ai_iAlignX ;
	m4bool_t ai_iSwClip, ai_iSwGraphics, ai_iSwFormatNULL ;
	ClMIT_Str m_DefDisplayNULL ;
	m4uint32_t ai_uiDefIdColour ; 

	m4bool_t ai_iIdFontInherit, ai_iIdFormatInherit, ai_iAlignYInherit, ai_iAlignXInherit,
		ai_iClipInherit, ai_iGraphicsInherit, ai_iIdColourInherit, ai_iSwFormatNULLInherit,
		ai_iDefDisplayNULLInherit ;

	ClENG_VB_Deserialize Serial ;
	
	Serial.Init(ai_pcDefaultSerialIn) ;

	ai_uiIdFont			= Serial.DeserializeULong() ; 
	ai_uiIdFormat		= Serial.DeserializeULong() ;
	ai_iAlignY			= Serial.DeserializeInt() ; 
	ai_iAlignX			= Serial.DeserializeInt() ; 
	ai_iSwClip			= Serial.DeserializeBool() ;
	ai_iSwGraphics		= Serial.DeserializeBool() ; 
	ai_iSwFormatNULL	= Serial.DeserializeBool()  ; 
	Serial.DeserializeStr(	&m_DefDisplayNULL ) ;
	ai_uiDefIdColour	= Serial.DeserializeULong() ; 

	ai_iIdFontInherit	= Serial.DeserializeBool() ;
	ai_iIdFormatInherit	= Serial.DeserializeBool() ;
	ai_iAlignYInherit	= Serial.DeserializeBool() ;
	ai_iAlignXInherit	= Serial.DeserializeBool() ;
	ai_iClipInherit		= Serial.DeserializeBool() ;
	ai_iGraphicsInherit	= Serial.DeserializeBool() ;
	ai_iIdColourInherit	= Serial.DeserializeBool() ; 
	ai_iSwFormatNULLInherit	= Serial.DeserializeBool() ;
	ai_iDefDisplayNULLInherit = Serial.DeserializeBool() ;

	if ( (ao_poDefaults->m_SwIdFontLoaded = !ai_iIdFontInherit )  ) {
		ao_poDefaults->m_uiIdFont = ai_uiIdFont ;
	} ;

	if ( (ao_poDefaults->m_SwIdFormatLoaded = !ai_iIdFormatInherit )  ) {
		if(ai_uiIdFormat==ClENG_Format::ID_NULL) {
			ao_poDefaults->m_SwIdFormatLoaded = M4_FALSE;
		} else {
			ao_poDefaults->m_uiIdFormat = ai_uiIdFormat ;
		};
	} ;

	if ( (ao_poDefaults->m_SwAlignYLoaded = !ai_iAlignYInherit )  ) {
		ao_poDefaults->m_AlignY = (ClENG_FieldStr::ALIGN)ai_iAlignY ;
	} ;

	if ( (ao_poDefaults->m_SwAlignXLoaded = !ai_iAlignXInherit )  ) {
		ao_poDefaults->m_AlignX = (ClENG_FieldStr::ALIGN)ai_iAlignX ;
	} ;

	if ( (ao_poDefaults->m_SwClipLoaded = !ai_iClipInherit )  ) {
		ao_poDefaults->m_bSwClip = ai_iSwClip ;
	} ;

	if ( (ao_poDefaults->m_SwGraphicsLoaded = !ai_iGraphicsInherit )  ) {
		ao_poDefaults->m_bSwGraphics = ai_iSwGraphics ;
	} ;

	if ( (ao_poDefaults->m_SwFormatNULLLoaded = !ai_iSwFormatNULLInherit )  ) {
		ao_poDefaults->m_bSwFormatNULL = ai_iSwFormatNULL ;
	} ;

	if ( (ao_poDefaults->m_SwDefDisplayNULLLoaded = !ai_iDefDisplayNULLInherit )  ) {
		ao_poDefaults->m_oDefDisplayNULL.StrCpy(m_DefDisplayNULL) ;
	} ;

	if ( (ao_poDefaults->m_SwIdColourLoaded = !ai_iIdColourInherit )  ) {
		ao_poDefaults->m_uiIdColour = ai_uiDefIdColour ;
	} ;
}



m4bool_t ClENG_SessReportDesigner::GetCT_Props(
		m4char_t *ai_pcCTId,
		m4char_t *ao_pcName,		m4uint32_t * ao_NameSize, 
		m4char_t *ao_pcTotList,		m4uint32_t * ao_TotListSize, 
		m4char_t *ao_pcTreeList,	m4uint32_t * ao_TreeListSize, 
		m4char_t *ao_pcFillTree,	m4uint32_t * ao_FillTreeSize, 
		m4int16_t *ao_piLevelCount, m4int16_t *ao_piMergerPage, 
		m4int32_t *ao_piColumnLeft, m4int32_t *ao_piColumnRight,
		m4int16_t *ao_poTogLevel,	m4int16_t *ao_poBreakLevel, 
		m4uint32_t *ao_piIdParamSec,m4int32_t *ao_piIdFirstCrossRow,
		m4char_t *ao_pcWizardData,	m4uint32_t * ao_WizardDataSize) 
{
	m4bool_t SwOk=M4_TRUE ;

	StENG_DT_CTData *CT ;

	CT = m_oDT.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId) ;
	if ( ! CT )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 


	ENG_VB_STR_CPY(ao_pcName,		ao_NameSize,		CT->m_oName) ;
	ENG_VB_STR_CPY(ao_pcTotList,	ao_TotListSize,		CT->m_oTotList) ;
	ENG_VB_STR_CPY(ao_pcTreeList,	ao_TreeListSize,	CT->m_oTreeList ) ;
	ENG_VB_STR_CPY(ao_pcFillTree,	ao_FillTreeSize,	CT->m_oFillTree ) ;
	*ao_piLevelCount	= CT->m_iLevelCount ;
	*ao_piMergerPage	= CT->m_MergePage ;
	*ao_piColumnLeft	= CT->m_XColumnLeft ;
	*ao_piColumnRight	= CT->m_XColumnRight ;
	*ao_poTogLevel		= CT->m_iTogLevel ;	
	*ao_poBreakLevel	= CT->m_iBreakLevel ; 
	*ao_piIdParamSec	= CT->m_uiIdParamSec ;
	*ao_piIdFirstCrossRow = CT->m_iFirstCrossRow ;
	//ENG_VB_STR_CPY(ao_pcWizardData,	ao_WizardDataSize,	CT->m_oWizardData ) ;

	//Get CT Props
	SwOk = M4_BOOL ( SwOk && m_oDesignChn.GetCTProps(ai_pcCTId, ao_pcWizardData) );
	*ao_WizardDataSize = strlen(ao_pcWizardData);

exit:

	return SwOk ;
}


m4bool_t ClENG_SessReportDesigner::GetCT_CellProps(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, m4int16_t ai_CellType,
		/*ENG_CT_CELL_Y_ALIGN*/m4int16_t *ao_CellAlignY, 
		/*ENG_CT_CELL_X_ALIGN*/m4int16_t *ao_CellAlignX,
		m4int16_t *ao_piFieldCount,
		m4int32_t *ao_piWidth,  m4int32_t *ao_piHeight, m4uint32_t *ao_puiIdFillColour,
		m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTCell *Cell ;
	StENG_DT_CTData *CT ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_CellType ) ) ;
	if (!SwOk) goto exit ;

	Cell = FindCT_T_CellInDT( ai_pcCTId, ai_Level, (ENG_CT_CELL_TYPE)ai_CellType, &CT ) ;
	if ( ! Cell )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	*ao_CellAlignY		= Cell->m_AlignParamCellY ;
	*ao_CellAlignX		= Cell->m_AlignParamCellX ;
	*ao_piFieldCount	= Cell->m_oFieldList.size() ;
	*ao_piWidth			= Cell->m_Width ;
	*ao_piHeight		= Cell->m_Height ;
	*ao_puiIdFillColour	= Cell->m_uiRegionColour ;
	ENG_VB_STR_CPY(ao_pcFrame,	aoi_puiFrameSize,	Cell->m_oRegionBorder ) ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::GetCntProps(
		m4uint32_t ai_uiIdSection, ENG_TYPE_CONTAINER ai_CntType,
		m4int16_t *ao_piPage,
		m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
		m4int16_t *ao_piSwFixed, m4int16_t *ao_piSwTogether,
		m4int32_t *ao_piLowerLimit,
		m4char_t *ao_pcCancelCnt,	m4uint32_t * ao_CancelCntSize, 
		m4char_t *ao_pcCTMerge,		m4uint32_t * ao_CTMergeSize, 
		m4int16_t *ao_piFieldCount,
		m4uint32_t *ao_puiIdFillColour,
		m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize,

		//Datos Param auxiliares

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4uint32_t *ao_puiLowerLimit,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,
		m4int16_t *ao_piSwLowerLimitLoaded,

		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CntData		*Cnt ;
	StENG_DT_SectionData	*Sect ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestLoaderCntType( (ENG_TYPE_CONTAINER)ai_CntType ) ) ;
	if (!SwOk) goto exit ;

	Sect=m_oDT.FindSection(ENG_EXIST_REQUIRED,ai_uiIdSection) ;
	if ( ! Sect )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 
	Cnt = Sect->FindCnt( ENG_EXIST_REQUIRED, ai_CntType ) ;
	if ( ! Cnt )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	*ao_piPage			= Cnt->m_iPage ;
	*ao_piTop			= Cnt->m_Top ;
	*ao_piLeft			= Cnt->m_Left ;
	*ao_piWidth			= Cnt->m_Width ;
	*ao_piHeight		= Cnt->m_Height ;
	*ao_piSwFixed		= Cnt->m_IsFixed ? 1: 0 ;
	*ao_piSwTogether	= Cnt->m_bSwTogether ? 1: 0 ;
	*ao_piLowerLimit	= Cnt->m_LowerLimit ;
	ENG_VB_STR_CPY(ao_pcCancelCnt,	ao_CancelCntSize,	Cnt->m_oCancelCnt ) ;
	ENG_VB_STR_CPY(ao_pcCTMerge,		ao_CTMergeSize,		Cnt->m_oCTMerge ) ;
	*ao_piFieldCount	= Cnt->m_oFieldList.size() ;
	*ao_puiIdFillColour	= Cnt->m_uiRegionColour ;
	ENG_VB_STR_CPY(ao_pcFrame,		aoi_puiFrameSize,	Cnt->m_oRegionBorder ) ;

	SwOk = M4_BOOL( SwOk && GetDefaultValues( 
		&Cnt->m_stDefaultData, ao_pcDefaultSerialOut, aoi_puiDefaultSerialOutSize )  ) ;

	*ao_puiTop				= Cnt->m_stFromValues.m_dTop*1000 ;
	*ao_puiLeft				= Cnt->m_stFromValues.m_dLeft*1000 ; 
	*ao_puiWidth			= Cnt->m_stFromValues.m_dWidth*1000 ;
	*ao_puiHeight			= Cnt->m_stFromValues.m_dHeight*1000 ;
	*ao_puiLowerLimit		= Cnt->m_stFromValues.m_dLowerLimit*1000 ;
	*ao_piSwTopLoaded		= Cnt->m_stFromValues.m_bTopLoaded ? 1: 0 ;
	*ao_piSwLeftLoaded		= Cnt->m_stFromValues.m_bLeftLoaded ? 1: 0 ;
	*ao_piSwWidthLoaded		= Cnt->m_stFromValues.m_bWidthLoaded ? 1: 0 ;
	*ao_piSwHeightLoaded	= Cnt->m_stFromValues.m_bHeightLoaded ? 1: 0 ;
	*ao_piSwLowerLimitLoaded= Cnt->m_stFromValues.m_bLowerLimitLoaded ? 1: 0 ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::GetFieldProps(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber, 
		m4char_t *ao_pcFieldName, m4uint32_t *aoi_puiFieldNameSize,
		m4int16_t *ao_piFieldType,
		m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
		m4char_t *ao_pcSaveNode, m4uint32_t *aoi_puiSaveNodeSize,
		m4char_t *ao_pcSaveItem, m4uint32_t *aoi_puiSaveItemSize,
		m4int16_t *ao_piSwStatic, 
		m4char_t *ao_pcIdPI, m4uint32_t *aoi_puiIdPISize,
		m4int16_t *ao_piSwOptimize, 
		m4char_t *ao_pcExpr, m4uint32_t *aoi_puiExprSize,
		m4char_t *ao_pcLiteralValue, m4uint32_t *aoi_puiLiteralValueSize,
		m4int16_t *ao_piSwEmbebedRaster, 
		m4char_t *ao_pcBNRaster, m4uint32_t *aoi_puiBNRasterSize,
		m4char_t *ao_pcColourRaster, m4uint32_t *aoi_puiColourRasterSize,
		m4int32_t *ao_piInterline, m4int16_t *ao_piSwVarHeight, m4int16_t *ao_piSwJustified,
		m4int16_t *ao_piMaxBoxes, m4int16_t *ao_piFillMode, m4int16_t *ao_piFillChar,
		m4int16_t *ao_piSwDuplicated,
		m4char_t *ao_pcIdCTField, m4uint32_t *aoi_puiIdCTFieldSize,
		m4int16_t *ao_piBarCodeType,
		m4uint32_t *ao_uiIdFillColour, m4char_t *ao_pcFrame, m4uint32_t *ao_puiFrameSize,

		m4char_t *ao_pcIndexExpr, m4uint32_t *ao_puiIndexExprSize,
		m4int32_t *ao_piIndexLevel,
		m4char_t *ai_pcHyperlink, m4uint32_t *ao_puiHyperlinkSize,
		m4int16_t *ao_piTempWebImage,

		//Datos Param auxiliares

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,

		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize )
{

	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_FieldData * Field ;
	
	
	Field = FindFieldInDT( ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber ) ;
	if (!Field) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	ENG_VB_STR_CPY(ao_pcFieldName,	aoi_puiFieldNameSize,	Field->m_oFieldName ) ;

	*ao_piFieldType		= Field->m_FldType ;
	*ao_piTop			= Field->m_Top ;
	*ao_piLeft			= Field->m_Left ;
	*ao_piWidth			= Field->m_Width ;
	*ao_piHeight		= Field->m_Height ;

	ENG_VB_STR_CPY(ao_pcSaveNode,	aoi_puiSaveNodeSize,	Field->m_oSaveNode ) ;
	ENG_VB_STR_CPY(ao_pcSaveItem,	aoi_puiSaveItemSize,	Field->m_oSaveItemName ) ;
	
	*ao_piSwStatic		= Field->m_bSwStatic ; 

	ENG_VB_STR_CPY(ao_pcIdPI,		aoi_puiIdPISize,		Field->m_oIdPi ) ;
	
	*ao_piSwOptimize	= Field->m_bSwOptimize ; 

	ENG_VB_STR_CPY(ao_pcExpr,			aoi_puiExprSize,			Field->m_oExpr) ;
	ENG_VB_STR_CPY(ao_pcLiteralValue,	aoi_puiLiteralValueSize,	Field->m_oLiteralValue) ;

	*ao_piSwEmbebedRaster	=Field->m_bEmbebedRaster ;
	
	ENG_VB_STR_CPY(ao_pcBNRaster,		aoi_puiBNRasterSize,		Field->m_oBNRaster) ;
	ENG_VB_STR_CPY(ao_pcColourRaster,	aoi_puiColourRasterSize,	Field->m_oColourRaster) ;

	*ao_piInterline		= Field->m_Interline ;
		
	*ao_piSwVarHeight	= Field->m_bIsVarHeight ;
	*ao_piSwJustified	= Field->m_bIsJustified ;

	*ao_piMaxBoxes		= Field->m_iMaxBoxes ;
	*ao_piFillMode		= Field->m_FillMode ;
	*ao_piFillChar		= Field->m_cFillChar ;
	*ao_piSwDuplicated	= Field->m_bSwDuplicated ;

	ENG_VB_STR_CPY(ao_pcIdCTField,	aoi_puiIdCTFieldSize,	Field->m_oIdCtField ) ;
	
	*ao_piBarCodeType	= Field->m_BarCodeType ;	
	*ao_uiIdFillColour	= Field->m_uiRegionColour ;
	ENG_VB_STR_CPY(ao_pcFrame,		ao_puiFrameSize,		Field->m_oRegionBorder) ;
	ENG_VB_STR_CPY(ao_pcIndexExpr,	ao_puiIndexExprSize,	Field->m_oIndexExpr) ;
	*ao_piIndexLevel	= Field->m_iIndexLevel ;
	ENG_VB_STR_CPY(ai_pcHyperlink,	ao_puiHyperlinkSize,	Field->m_oHyperlink) ;
	*ao_piTempWebImage	= Field->m_bTempWebImage ? 1: 0 ;


	SwOk = M4_BOOL( SwOk && GetDefaultValues(
		&Field->m_stDefaultData, ao_pcDefaultSerialOut, aoi_puiDefaultSerialOutSize ) ) ;



	*ao_puiTop				= Field->m_stFromValues.m_dTop*1000 ;
	*ao_puiLeft				= Field->m_stFromValues.m_dLeft*1000 ; 
	*ao_puiWidth			= Field->m_stFromValues.m_dWidth*1000 ;
	*ao_puiHeight			= Field->m_stFromValues.m_dHeight*1000 ;
	*ao_piSwTopLoaded		= Field->m_stFromValues.m_bTopLoaded ? 1: 0 ;
	*ao_piSwLeftLoaded		= Field->m_stFromValues.m_bLeftLoaded ? 1: 0 ;
	*ao_piSwWidthLoaded		= Field->m_stFromValues.m_bWidthLoaded ? 1: 0 ;
	*ao_piSwHeightLoaded	= Field->m_stFromValues.m_bHeightLoaded ? 1: 0 ;

exit:
	return SwOk ;
}


m4bool_t ClENG_SessReportDesigner::LoadCT_Sample(
		m4char_t *ai_pcCTId, 
		m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt,
		m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_CntSample Sample ;
	ClENG_VB_Serialize Serial ;

	SwOk = M4_BOOL( SwOk && Sample.Init(
		&m_oDT, ai_pcCTId, NULL, ai_pcLevelInfo, ai_bSwIncr, ai_iTogLevel, ai_bSwAllCnt,
		ClMIT_Trace::PROPAGATE_ERROR )
	) ;
	if (!SwOk) goto exit ;

	Sample.Trace() ;

	Sample.Serialize( &Serial ) ;

	SwOk = M4_BOOL( SwOk && Serial.SaveToStr(ao_pcSerialOut, aoi_puiSerialOutSize ) ) ;

exit:
	return SwOk ;
}



m4bool_t ClENG_SessReportDesigner::SetCT_Props(
		m4char_t *ai_pcCTId,
		m4int16_t ai_iMergePage, m4double_t ai_dColumnLeft, m4double_t ai_dColumnRight,
		m4int16_t ai_iTogLevel,	m4char_t *ai_pcWizardData ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CTData *CT, Backup ;
	ClMIT_Args Args;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && 
		ClENG_DesignTree::TestMergePage( (ENG_CT_MERGE_PAGE)ai_iMergePage ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	CT = m_oDD.FindCT( ENG_EXIST_REQUIRED, ai_pcCTId ) ;
	if (!CT) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Set ...

	Backup = *CT ;

	CT->m_MergePage			= (ENG_CT_MERGE_PAGE)ai_iMergePage ;	CT->m_bMergePageLoaded = M4_TRUE ;
	CT->m_iTogLevel			= ai_iTogLevel ;	CT->m_bTogLevelLoaded = M4_TRUE ;
	CT->m_dXColumnLeft		= ai_dColumnLeft; 	CT->m_bXColumnLeftLoaded = M4_TRUE ;
	CT->m_dXColumnRight		= ai_dColumnRight; 	CT->m_bXColumnRightLoaded = M4_TRUE ;
	//CT->m_oWizardData.StrCpy( ai_pcWizardData ) ;	CT->m_bWizardDataLoaded = M4_TRUE ;

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		*CT = Backup ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

	//Set CT Props
	SwOk = M4_BOOL ( SwOk && m_oDesignChn.SetCTProps(ai_pcCTId, ai_pcWizardData) );

exit:
	return SwOk ;

}

m4bool_t ClENG_SessReportDesigner::SetWizardData(
		m4char_t *ai_pcCTId, m4char_t *ai_pcWizardData ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CTData *CT, Backup ;
	ClMIT_Args Args;

	//Set Wizard Data Props
	SwOk = M4_BOOL ( SwOk && m_oDesignChn.SetCTProps(ai_pcCTId, ai_pcWizardData) );

exit:
	return SwOk ;

}

m4bool_t ClENG_SessReportDesigner::SetCT_CellProps(
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame ) 
{
	m4bool_t SwOk=M4_TRUE ;

	StENG_DD_CTCell *Cell, Backup ;
	StENG_DD_CTData *CT=NULL;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_CellType ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestAlignParamCellY( (ENG_CT_CELL_Y_ALIGN)ai_CellAlignY ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestAlignParamCellX( (ENG_CT_CELL_X_ALIGN)ai_CellAlignX ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	Cell = FindCT_T_CellInDD( ai_pcCTId, ai_Level, (ENG_CT_CELL_TYPE)ai_CellType, &CT ) ;
	if ( ! Cell )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	//Set ...

	Backup = *Cell ;

	Cell->GetMinimalWidth(&ai_dWidth);

	Cell->m_AlignParamCellX = (ENG_CT_CELL_X_ALIGN)ai_CellAlignX	; Cell->m_bAlignParamCellXLoaded = M4_TRUE ;
	Cell->m_AlignParamCellY = (ENG_CT_CELL_Y_ALIGN)ai_CellAlignY	; Cell->m_bAlignParamCellYLoaded = M4_TRUE ;
	Cell->m_uiRegionColour  = ai_uiIdFillColour	;	Cell->m_bRegionColourLoaded  = M4_TRUE ;
	Cell->m_dWidth  = ai_dWidth ;					Cell->m_bWidthLoaded		= M4_TRUE ;
	Cell->m_dHeight = ai_dHeight ;					Cell->m_bHeightLoaded	= M4_TRUE ;

	Cell->m_oRegionBorder.StrCpy(ai_pcFrame);
	if(Cell->m_oRegionBorder.StrLen()) {
		Cell->m_bRegionBorderLoaded	 = M4_TRUE;
	};

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		*Cell = Backup ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClENG_SessReportDesigner::SetFieldProps(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber, 
		m4char_t *ai_pcFieldName, m4int16_t ai_iFieldType,
		m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
		m4char_t *ai_pcSaveNode, m4char_t *ai_pcSaveItem, m4int16_t ai_iSwStatic, 
		m4char_t *ai_pcIdPI, m4int16_t ai_iSwOptimize, 
		m4char_t *ai_pcExpr, m4char_t *ai_pcLiteralValue, 
	
		m4int16_t ai_iSwEmbebedRaster, 
		m4char_t *ai_pcBNRaster, m4char_t *ai_pcColourRaster, 
		m4double_t ai_dInterline, m4int16_t ai_iSwVarHeight, m4int16_t ai_iSwJustified,
		m4int16_t ai_iMaxBoxes, m4int16_t ai_iFillMode, m4int16_t ai_iFillChar,
		m4int16_t ai_iSwDuplicated, m4char_t *ai_pcIdCTField, m4int16_t ai_iBarCodeType,
		m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame, 
		m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
		m4char_t *ai_pcHyperlink, m4int16_t ai_iTempWebImage,

		m4char_t *ai_pcDefaultSerialIn )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_FieldData * Field, Backup ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldType( (ENG_FIELD_TYPE)ai_iFieldType ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldFillMode( (ClENG_FieldStr::FILL_MODE)ai_iFillMode ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldBarCodeType( (ENG_BAR_CODE_TYPE)ai_iBarCodeType ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	Field = FindFieldInDD( ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber ) ;
	if (!Field) {
		SwOk=M4_FALSE ;
		goto exit ;
	};

	//Set ...

	Backup = *Field ;

	Field->m_oFieldName.StrCpy(ai_pcFieldName) ;		Field->m_bFieldNameLoaded=M4_TRUE;
		
	Field->m_FldType=(ENG_FIELD_TYPE)ai_iFieldType ;	Field->m_bFldTypeLoaded=M4_TRUE ;

	Field->m_dTop=ai_dTop ;								Field->m_bTopLoaded=M4_TRUE ;
		
	Field->m_dLeft=ai_dLeft ;							Field->m_bLeftLoaded=M4_TRUE ; 

	Field->m_dWidth=ai_dWidth;							Field->m_bWidthLoaded=M4_TRUE ;

	Field->m_dHeight=ai_dHeight ;						Field->m_bHeightLoaded=M4_TRUE ;

	Field->m_oSaveNode.StrCpy(ai_pcSaveNode) ;			Field->m_bSaveNodeLoaded=M4_TRUE ;

	Field->m_oSaveItemName.StrCpy(ai_pcSaveItem) ;		Field->m_bSaveItemNameLoaded=M4_TRUE ;

	Field->m_bSwStatic = M4_BOOL(ai_iSwStatic)  ;		Field->m_bSwStaticLoaded=M4_TRUE ;

	Field->m_oIdPi.StrCpy(ai_pcIdPI);					Field->m_bIdPiLoaded=M4_TRUE ;

	Field->m_bSwOptimize = M4_BOOL(ai_iSwOptimize) ;	Field->m_bSwOptimizeLoaded=M4_TRUE ;
	
	Field->m_oExpr.StrCpy(ai_pcExpr) ;					Field->m_bExprLoaded=M4_TRUE ;

	Field->m_oLiteralValue.StrCpy(ai_pcLiteralValue) ;	Field->m_bLiteralValueLoaded=M4_TRUE ;

	Field->m_oIndexExpr.StrCpy(ai_pcIndexExpr) ;		Field->m_bIndexExprLoaded=M4_TRUE;

	Field->m_iIndexLevel = ai_iIndexLevel;				Field->m_bIndexLevelLoaded=M4_TRUE;

	Field->m_oHyperlink.StrCpy(ai_pcHyperlink) ;		Field->m_bHyperlinkLoaded=M4_TRUE;

	Field->m_bTempWebImage= M4_BOOL(ai_iTempWebImage); 	Field->m_bTempWebImageLoaded=M4_TRUE;

	Field->m_bEmbebedRaster = M4_BOOL(ai_iSwEmbebedRaster) ;	Field->m_bEmbebedRasterLoaded=M4_TRUE ;

	Field->m_oBNRaster.StrCpy(ai_pcBNRaster) ;			Field->m_bBNRasterLoaded=M4_TRUE ;

	Field->m_oColourRaster.StrCpy(ai_pcColourRaster) ;	Field->m_bColourRasterLoaded=M4_TRUE  ;

	Field->m_dInterline=ai_dInterline ;					Field->m_bInterlineLoaded=M4_TRUE ;

	Field->m_bIsVarHeight=M4_BOOL(ai_iSwVarHeight);		Field->m_bIsVarHeightLoaded=M4_TRUE;

	Field->m_bIsJustified=M4_BOOL(ai_iSwJustified);		Field->m_bIsJustifiedLoaded=M4_TRUE ;

	Field->m_iMaxBoxes=ai_iMaxBoxes;					Field->m_bMaxBoxesLoaded=M4_TRUE ;

	Field->m_FillMode=(ClENG_FieldStr::FILL_MODE)ai_iFillMode;	Field->m_bFillModeLoaded=M4_TRUE ;
		
	Field->m_cFillChar=ai_iFillChar;					Field->m_bFillCharLoaded=M4_TRUE ;

	Field->m_bSwDuplicated=M4_BOOL(ai_iSwDuplicated) ;	Field->m_bSwDuplicatedLoaded=M4_TRUE ;

	Field->m_oIdCtField.StrCpy(ai_pcIdCTField) ;		Field->m_bIdCtFieldLoaded=M4_TRUE ;

	Field->m_BarCodeType=(ENG_BAR_CODE_TYPE)ai_iBarCodeType;	Field->m_bBarCodeTypeLoaded=M4_TRUE ;

	Field->m_uiRegionColour =ai_uiIdFillColour;			Field->m_bRegionColourLoaded=M4_TRUE ;

	Field->m_oRegionBorder.StrCpy(ai_pcFrame);
	if(Field->m_oRegionBorder.StrLen()) {
		Field->m_bRegionBorderLoaded = M4_TRUE;
	};
	
	SetDefaultValues( & Field->m_stDefaultData, ai_pcDefaultSerialIn ) ;

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		*Field = Backup ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::AddField(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4char_t *ai_pcFieldName, m4int16_t ai_iFieldType,
		m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
		m4char_t *ai_pcSaveNode, m4char_t *ai_pcSaveItem, m4int16_t ai_iSwStatic, 
		m4char_t *ai_pcIdPI, m4int16_t ai_iSwOptimize, 
		m4char_t *ai_pcExpr, m4char_t *ai_pcLiteralValue, 

		m4int16_t ai_iSwEmbebedRaster, 
		m4char_t *ai_pcBNRaster, m4char_t *ai_pcColourRaster, 
		m4double_t ai_dInterline, m4int16_t ai_iSwVarHeight, m4int16_t ai_iSwJustified,

		m4int16_t ai_iMaxBoxes, m4int16_t ai_iFillMode, m4int16_t ai_iFillChar,
		m4int16_t ai_iSwDuplicated, m4char_t *ai_pcIdCTField, m4int16_t ai_iBarCodeType,
		m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame, 
		m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
		m4char_t *ai_pcHyperlink, m4int16_t ai_iTempWebImage,

		//Defectos
		m4char_t *ai_pcDefaultSerialIn  ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_DD_FieldList_t *FieldList ;
	ENG_DD_FieldList_t::iterator It ;
	StENG_DD_FieldData	FieldAux, *Field ;
	m4uint32_t NewKey ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldType( (ENG_FIELD_TYPE)ai_iFieldType ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldFillMode( (ClENG_FieldStr::FILL_MODE)ai_iFillMode ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestFieldBarCodeType( (ENG_BAR_CODE_TYPE)ai_iBarCodeType ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	FieldList = FindFieldListInDD( ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2 ) ;
	if (! FieldList ) {
		SwOk=M4_FALSE ;
		goto exit ;
	};

	//Set ...
	
	NewKey = StENG_DD_CntData::GenerateNewIdField(FieldList) ; 

	FieldAux.Reset() ;
	It = FieldList->insert( FieldList->end(), FieldAux ) ;
	Field = &(*It) ;

	Field->m_uiIdField= NewKey	;						Field->m_bIdFieldLoaded=M4_TRUE ;
	
	Field->m_oFieldName.StrCpy(ai_pcFieldName) ;		Field->m_bFieldNameLoaded=M4_TRUE;
		
	Field->m_FldType=(ENG_FIELD_TYPE)ai_iFieldType ;	Field->m_bFldTypeLoaded=M4_TRUE ;

	Field->m_dTop=ai_dTop ;								Field->m_bTopLoaded=M4_TRUE ;
		
	Field->m_dLeft=ai_dLeft ;							Field->m_bLeftLoaded=M4_TRUE ; 

	Field->m_dWidth=ai_dWidth;							Field->m_bWidthLoaded=M4_TRUE ;

	Field->m_dHeight=ai_dHeight ;						Field->m_bHeightLoaded=M4_TRUE ;

	Field->m_oSaveNode.StrCpy(ai_pcSaveNode) ;			Field->m_bSaveNodeLoaded=M4_TRUE ;

	Field->m_oSaveItemName.StrCpy(ai_pcSaveItem) ;		Field->m_bSaveItemNameLoaded=M4_TRUE ;

	Field->m_bSwStatic = M4_BOOL(ai_iSwStatic)  ;		Field->m_bSwStaticLoaded=M4_TRUE ;

	Field->m_oIdPi.StrCpy(ai_pcIdPI);					Field->m_bIdPiLoaded=M4_TRUE ;

	Field->m_bSwOptimize = M4_BOOL(ai_iSwOptimize) ;	Field->m_bSwOptimizeLoaded=M4_TRUE ;
	
	Field->m_oExpr.StrCpy(ai_pcExpr) ;					Field->m_bExprLoaded=M4_TRUE ;

	Field->m_oLiteralValue.StrCpy(ai_pcLiteralValue) ;	Field->m_bLiteralValueLoaded=M4_TRUE ;

	Field->m_oIndexExpr.StrCpy(ai_pcIndexExpr) ;		Field->m_bIndexExprLoaded=M4_TRUE;

	Field->m_iIndexLevel = ai_iIndexLevel;				Field->m_bIndexLevelLoaded=M4_TRUE;

	Field->m_oHyperlink.StrCpy(ai_pcHyperlink) ;		Field->m_bHyperlinkLoaded=M4_TRUE;

	Field->m_bTempWebImage= M4_BOOL(ai_iTempWebImage); 	Field->m_bTempWebImageLoaded=M4_TRUE;

	Field->m_bEmbebedRaster = M4_BOOL(ai_iSwEmbebedRaster) ;	Field->m_bEmbebedRasterLoaded=M4_TRUE ;

	Field->m_oBNRaster.StrCpy(ai_pcBNRaster) ;			Field->m_bBNRasterLoaded=M4_TRUE ;

	Field->m_oColourRaster.StrCpy(ai_pcColourRaster) ;	Field->m_bColourRasterLoaded=M4_TRUE  ;

	Field->m_dInterline=ai_dInterline ;					Field->m_bInterlineLoaded=M4_TRUE ;

	Field->m_bIsVarHeight=M4_BOOL(ai_iSwVarHeight);		Field->m_bIsVarHeightLoaded=M4_TRUE;

	Field->m_bIsJustified=M4_BOOL(ai_iSwJustified);		Field->m_bIsJustifiedLoaded=M4_TRUE ;

	Field->m_iMaxBoxes=ai_iMaxBoxes;					Field->m_bMaxBoxesLoaded=M4_TRUE ;

	Field->m_FillMode=(ClENG_FieldStr::FILL_MODE)ai_iFillMode;	Field->m_bFillModeLoaded=M4_TRUE ;
		
	Field->m_cFillChar=ai_iFillChar;					Field->m_bFillCharLoaded=M4_TRUE ;

	Field->m_bSwDuplicated=M4_BOOL(ai_iSwDuplicated) ;	Field->m_bSwDuplicatedLoaded=M4_TRUE ;

	Field->m_oIdCtField.StrCpy(ai_pcIdCTField) ;		Field->m_bIdCtFieldLoaded=M4_TRUE ;

	Field->m_BarCodeType=(ENG_BAR_CODE_TYPE)ai_iBarCodeType;	Field->m_bBarCodeTypeLoaded=M4_TRUE ;

	Field->m_uiRegionColour =ai_uiIdFillColour;			Field->m_bRegionColourLoaded=M4_TRUE ;

	Field->m_oRegionBorder.StrCpy(ai_pcFrame);
	if(Field->m_oRegionBorder.StrLen()) {
		Field->m_bRegionBorderLoaded = M4_TRUE;
	};
	
	SetDefaultValues( & Field->m_stDefaultData, ai_pcDefaultSerialIn ) ;

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		FieldList->erase( It ) ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClENG_SessReportDesigner::DeleteField(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_DD_FieldList_t *List;
	ENG_DD_FieldList_t::iterator It ;
	StENG_DD_FieldData	*Field ;

	//Find ...

	Field = FindFieldInDD( ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber,
		&List, &It ) ;
	if (!Field) {
		SwOk=M4_FALSE ;
		goto exit ;
	};

	// Borra referencias

	if ( ai_Owner=='T' ) {
		m_oDD.DeleteReferencesToIdCTField(ai_pcCTId,(*It).m_oIdCtField) ;
	} ;

	// Erase 

	List->erase( It ) ;

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;

	//No hago nada, borrar un field no deberia dar error jamas

exit:
	return SwOk ;

}



m4bool_t ClENG_SessReportDesigner::SetCntProps(
		m4uint32_t ai_uiIdSection, /*ENG_TYPE_CONTAINER*/ m4int16_t ai_CntType,
		m4int16_t ai_iPage,
		m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
		m4int16_t ai_iSwFixed, m4int16_t ai_iSwTogether,
		m4double_t ai_dLowerLimit,
		m4char_t *ai_pcCancelCnt,	 
		m4char_t *ai_pcCTMerge,
		m4uint32_t ai_uiIdFillColour,
		m4char_t *ai_pcFrame, 

		m4char_t *ai_pcDefaultSerialIn )
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CntData		*Cnt, Backup ;
	StENG_DD_SectionData	*Sect ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestLoaderCntType( (ENG_TYPE_CONTAINER)ai_CntType ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	Sect=m_oDD.FindSection(ENG_EXIST_REQUIRED,ai_uiIdSection) ;
	if ( ! Sect )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 
	Cnt = Sect->FindCnt( ENG_EXIST_REQUIRED, (ENG_TYPE_CONTAINER)ai_CntType ) ;
	if ( ! Cnt )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	//Set ...

	Backup = *Cnt ;

	Cnt->GetMinimalHeight(&ai_dHeight);

	Cnt->m_iPage = ai_iPage ;					Cnt->m_bPageLoaded = M4_TRUE ;
	
	Cnt->m_dTop  = ai_dTop   ;					Cnt->m_bTopLoaded = M4_TRUE ;
	
	Cnt->m_dLeft = ai_dLeft ;					Cnt->m_bLeftLoaded = M4_TRUE ;

	Cnt->m_dHeight = ai_dHeight ;				Cnt->m_bHeightLoaded = M4_TRUE ;

	Cnt->m_dWidth = ai_dWidth ;					Cnt->m_bWidthLoaded = M4_TRUE ;

	Cnt->m_IsFixed = M4_BOOL(ai_iSwFixed) ;		Cnt->m_bIsFixedLoaded = M4_TRUE ;

	Cnt->m_bSwTogether = M4_BOOL(ai_iSwTogether) ;	Cnt->m_bSwTogetherLoaded = M4_TRUE ;

	Cnt->m_dLowerLimit = ai_dLowerLimit;		Cnt->m_bLowerLimitLoaded = M4_TRUE ;

	Cnt->m_oCTMerge.StrCpy(ai_pcCTMerge) ;		Cnt->m_bCTMergeLoaded = M4_TRUE ;
		
	Cnt->m_oCancelCnt.StrCpy(ai_pcCancelCnt) ;	Cnt->m_bCancelCntLoaded = M4_TRUE ;

	Cnt->m_uiRegionColour=ai_uiIdFillColour ;	Cnt->m_bRegionColourLoaded = M4_TRUE ;

	Cnt->m_oRegionBorder.StrCpy(ai_pcFrame);
	if(Cnt->m_oRegionBorder.StrLen()) {
		Cnt->m_bRegionBorderLoaded = M4_TRUE;
	};


	SetDefaultValues( & Cnt->m_stDefaultData, ai_pcDefaultSerialIn ) ;

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		*Cnt = Backup ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::DeleteCnt(
	m4uint32_t ai_uiIdSection, /*ENG_TYPE_CONTAINER*/ m4int16_t ai_CntType ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_SectionData::CntList_t::iterator		CntIt ;
	StENG_DD_SectionData	*Sect ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestLoaderCntType( (ENG_TYPE_CONTAINER)ai_CntType ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	Sect=m_oDD.FindSection(ENG_EXIST_REQUIRED,ai_uiIdSection) ;
	if ( ! Sect )  {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 
	if ( ! Sect->FindCnt( ENG_EXIST_REQUIRED, (ENG_TYPE_CONTAINER)ai_CntType, &CntIt ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	//Borrado 

	Sect->m_oCnt.erase(CntIt) ;
	m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;	//Borrar un Cnt siempre OK

exit:
	return SwOk ;

}

m4uint32_t ClENG_SessReportDesigner::CountFontInstances( m4uint32_t ai_uiIdFont ) 
{
	return m_oDT.CountFontInstances(ai_uiIdFont) ;
}

m4uint32_t ClENG_SessReportDesigner::CountFormatInstances( m4uint32_t ai_uiIdFormat ) 
{
	return m_oDT.CountFormatInstances(ai_uiIdFormat) ;
}


m4bool_t ClENG_SessReportDesigner::DeleteCT_Cell(
	m4char_t *ai_pcCTId, m4int16_t ai_Level, /*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CTData *CT ;
	StENG_DD_CTData::CellList_t::iterator CellIt  ;
	StENG_DD_CTCell *Cell ;
	ENG_DD_FieldList_t::iterator FieldIt ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_CellType ) ) ;
	if (!SwOk) goto exit ;

	//Find ...

	Cell = FindCT_T_CellInDD( ai_pcCTId, ai_Level, (ENG_CT_CELL_TYPE)ai_CellType, &CT ,&CellIt ) ;
	if ( !Cell ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	//Borrado de fields referenciados

	for ( FieldIt=Cell->m_oFieldList.begin(); FieldIt!=Cell->m_oFieldList.end(); ++FieldIt ) {
		m_oDD.DeleteReferencesToIdCTField( ai_pcCTId, (*FieldIt).m_oIdCtField ) ;
	} ;

	//Borrado de cell

	CT->m_oCellList.erase( CellIt ) ;

	//Update DT ...

	//Borrar un celda siempre OK

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;

exit:
	return SwOk ;	
}


m4bool_t ClENG_SessReportDesigner::AddCT_Cell(
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CTData *CT ;
	StENG_DD_CTData::CellList_t::iterator CellIt  ;
	StENG_DD_CTCell CellAux, *Cell ;
	m4uint32_t IdParamSec ;

	//Test enums ...

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestCellType( (ENG_CT_CELL_TYPE)ai_CellType ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestAlignParamCellY( (ENG_CT_CELL_Y_ALIGN)ai_CellAlignY ) ) ;
	if (!SwOk) goto exit ;

	SwOk = M4_BOOL( SwOk && ClENG_DesignTree::TestAlignParamCellX( (ENG_CT_CELL_X_ALIGN)ai_CellAlignX ) ) ;
	if (!SwOk) goto exit ;

	//Find CT

	CT = m_oDD.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( !CT ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! CT->GetParamSec( ai_Level, &IdParamSec  ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Set ...

	CellAux.Reset() ;
	CT->m_oCellList.push_back( CellAux ) ;
	CellIt = CT->m_oCellList.end() ;	--CellIt ;
	Cell = &(*CellIt) ;

	Cell->m_uiIdParamSec = IdParamSec ;						Cell->m_bIdParamSecLoaded = M4_TRUE ;
	Cell->m_Type		= (ENG_CT_CELL_TYPE)ai_CellType ;	Cell->m_bTypeLoaded = M4_TRUE ;
	Cell->m_iLevel		= ai_Level;							Cell->m_bLevelLoaded  = M4_TRUE ;
	Cell->m_dHeight		= ai_dHeight ;						Cell->m_bHeightLoaded = M4_TRUE ;
	Cell->m_dWidth		= ai_dWidth;						Cell->m_bWidthLoaded = M4_TRUE ;
	Cell->m_uiRegionColour = ai_uiIdFillColour ;			Cell->m_bRegionColourLoaded  = M4_TRUE ;
	Cell->m_AlignParamCellY = (ENG_CT_CELL_Y_ALIGN)ai_CellAlignY ;	Cell->m_bAlignParamCellYLoaded = M4_TRUE ;
	Cell->m_AlignParamCellX = (ENG_CT_CELL_X_ALIGN)ai_CellAlignX ;	Cell->m_bAlignParamCellXLoaded  = M4_TRUE ; ;
	
	Cell->m_oRegionBorder.StrCpy(ai_pcFrame);
	if(Cell->m_oRegionBorder.StrLen()) {
		Cell->m_bRegionBorderLoaded	 = M4_TRUE;
	};

	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {
		CT->m_oCellList.erase( CellIt ) ;
		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::SynchronizeCT(m4char_t *ai_pcCTId) 
{
	return  m_oDesignChn.SynchronizeCT( ai_pcCTId, &m_oDD ) ; 
}

void  ClENG_SessReportDesigner::SynchronizeFontsUsedByReport() 
{
	m_oDesignChn.SynchronizeFontsUsedByReport(&m_oDD) ;
}

void  ClENG_SessReportDesigner::SynchronizeFormatsUsedByReport() 
{
	m_oDesignChn.SynchronizeFormatsUsedByReport(&m_oDD) ;
}

void  ClENG_SessReportDesigner::SynchronizeColoursUsedByReport() 
{
	m_oDesignChn.SynchronizeColoursUsedByReport(&m_oDD) ;
}

m4bool_t ClENG_SessReportDesigner::IncrCTHeightLevel(m4char_t *ai_pcCTId,m4int16_t ai_Level, m4double_t ai_dHeightIncrement) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_CTData *CT_DD ;
	StENG_DT_CTData *CT_DT ;
	StENG_DD_CTData::CellList_t::iterator CellIt ;
	StENG_DD_CTCell *Cell ;
	m4int16_t BBeg, BEnd ;
	m4double_t LevelHeight ;


	//Find CT

	CT_DD = m_oDD.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( !CT_DD ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	CT_DT = m_oDT.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( !CT_DT ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//A recorrerse las celdas en cuestion

	if (ai_Level<0) {
		BBeg = 0 ;
		BEnd = CT_DT->m_oTemplate.GetLevelCount() ;
	} else {
		BBeg = BEnd = ai_Level ;
		if ( ! M4_IN_RANGE( ai_Level, 0, CT_DT->m_oTemplate.GetLevelCount()-1 ) ) {
			goto exit ;
		} ;
	} ;

	for ( CellIt=CT_DD->m_oCellList.begin(); CellIt!=CT_DD->m_oCellList.end(); ++CellIt ) {
		Cell = &(*CellIt) ;
		if ( ! Cell->m_bLevelLoaded ) continue ;
		if ( ! M4_IN_RANGE( Cell->m_iLevel, BBeg, BEnd ) ) continue ;
		LevelHeight = m_oDT.GetDesignUnit( CT_DT->m_oTemplate.GetLevelHeight(Cell->m_iLevel) ) ;
		if ( ai_dHeightIncrement+LevelHeight>0 ) {
			Cell->m_dHeight = ai_dHeightIncrement+LevelHeight ;
			Cell->m_bHeightLoaded = M4_TRUE ;
		} ;
	} ;


	//Update DT ...

	SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) {

		//A deshacer los cambios !

		for ( CellIt=CT_DD->m_oCellList.begin(); CellIt!=CT_DD->m_oCellList.end(); ++CellIt ) {
			Cell = &(*CellIt) ;
			if ( ! Cell->m_bLevelLoaded ) continue ;
			if ( ! M4_IN_RANGE( Cell->m_iLevel, BBeg, BEnd ) ) continue ;
			LevelHeight = m_oDT.GetDesignUnit( CT_DT->m_oTemplate.GetLevelHeight(Cell->m_iLevel) ) ;
			if ( ai_dHeightIncrement-LevelHeight>0 ) {
				Cell->m_dHeight  -= ai_dHeightIncrement ;
				Cell->m_bHeightLoaded = M4_TRUE ;
			} ;
		} ;
		
		//Y a actualizar DD

		m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ;
		goto exit ;
	} ;


exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::GenerateNewCTIdField(m4char_t *ai_pcCTId,
	m4char_t *ao_pcKey, m4uint32_t *aoi_puiKeySize) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData *CT ;
	ClMIT_Str StrAux ;

	//Find CT

	CT = m_oDT.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( !CT ) {
		SwOk=M4_FALSE ;
	} else {
		CT->m_oTemplate.GenerateNewCTIdField(&StrAux) ;
		ENG_VB_STR_CPY(ao_pcKey, aoi_puiKeySize, StrAux ) ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::EnsureFont(m4uint32_t ai_uiIdFont) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::FontList_t::iterator It ;

	if ( ! m_oDD.FindFont(ai_uiIdFont,It) ) {

		//Carga en DD desde Chn

		if ( ! m_oDesignChn.LoadFont(&m_oDD,ai_uiIdFont) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//Añade en DT

		if ( ! m_oDT.NewFont(&m_oDD,ai_uiIdFont) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::EnsureFormat(m4uint32_t ai_uiIdFormat) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::FormatList_t::iterator It ;

	if ( ! m_oDD.FindFormat(ai_uiIdFormat,It) ) {

		//Carga en DD desde Chn

		if ( ! m_oDesignChn.LoadFormat(&m_oDD,ai_uiIdFormat) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//Añade en DT

		if ( ! m_oDT.NewFormat(&m_oDD,ai_uiIdFormat) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} ;

exit:
	return SwOk ;

}

m4bool_t ClENG_SessReportDesigner::EnsureColour(m4uint32_t ai_uiIdColour) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DesignData::ColourList_t::iterator It ;

	if ( ! m_oDD.FindColour(ai_uiIdColour,It) ) {

		//Carga en DD desde Chn

		if ( ! m_oDesignChn.LoadColour(&m_oDD,ai_uiIdColour) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//Añade en DT

		if ( ! m_oDT.NewColour(&m_oDD,ai_uiIdColour) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::ClearFontsNotUsed() 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_ULongList_t L ;
	ENG_ULongList_t::iterator It ;

	m_oDT.GetFontsNotUsed(&L) ;

	for( It=L.begin(); It!=L.end(); ++It ) {
		if ( ! m_oDD.DeleteFont( (*It) )  ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

	if ( L.size()>0 ) {
		SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::ClearFormatsNotUsed() 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_ULongList_t L ;
	ENG_ULongList_t::iterator It ;

	m_oDT.GetFormatsNotUsed(&L) ;

	for( It=L.begin(); It!=L.end(); ++It ) {
		if ( ! m_oDD.DeleteFormat( (*It) )  ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

	if ( L.size()>0 ) {
		SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_SessReportDesigner::ClearColoursNotUsed() 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_ULongList_t L ;
	ENG_ULongList_t::iterator It ;

	m_oDT.GetColoursNotUsed(&L) ;

	for( It=L.begin(); It!=L.end(); ++It ) {
		if ( ! m_oDD.DeleteColour( (*It) )  ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

	if ( L.size()>0 ) {
		SwOk = M4_BOOL ( SwOk && m_oDT.Init( &m_oDD, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	} ;

	return SwOk ;
}


m4bool_t ClENG_SessReportDesigner::GetCT_WizardData( m4char_t *ai_pcCTId, m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData *CT ;
	StENG_DT_SectionData *Root ;
	ClENG_VB_Serialize Serial ;
	ClMIT_Args Arg ;
	ClMIT_Str Str ;
	TotalMap_t TotalMap ;
	TotalMap_t::iterator TotIt ;
	m4int16_t B; 

	//**********Find CT

	CT = m_oDT.FindCT(ENG_EXIST_REQUIRED,ai_pcCTId)  ;
	if ( !CT ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//**********Haya  datos Serial

	Serial.Init() ;
	
	//***>Lista columnas

	Arg.Init( CT->m_oFillTree, ClMIT_Args::NO_SUBARG, " ," ) ; //No queremos subargs!
	
	for (B=0; B<Arg.GetArgCount(); ++B ) {
		Arg.GetString(B,0,&Str) ;
		Serial.SerializeBool(M4_FALSE) ;	//Not EOF
		Serial.SerializeStr( Str ) ;	//Data
	} ;
	
	Serial.SerializeBool(M4_TRUE) ;	//Marca final de EOF

	//***>Lista totalizadores

	Root = m_oDT.FindSection( ENG_TRY_FIND, CT->m_iFirstCrossRow  )  ;

	GetCT_WizardDataTotalize( &TotalMap, ai_pcCTId, Root ) ;

	for ( TotIt=TotalMap.begin(); TotIt!=TotalMap.end(); ++TotIt ) {
		Serial.SerializeBool(M4_FALSE) ;	//Not EOF
		Str.ToASCIZ( (*TotIt).first, (*TotIt).second  ) ;
		Serial.SerializeStr( Str ) ;	//Data, IdTot
		Str.StrCpy( &(*TotIt).first[(*TotIt).second+1] ) ;
		Serial.SerializeStr( Str ) ;	//Data, Func
	} ;

	Serial.SerializeBool(M4_TRUE) ;	//Marca final de EOF

	//***>Lista filas

	GetCT_WizardDataRow( &Serial, ai_pcCTId, Root ) ;

	Serial.SerializeBool(M4_TRUE) ;	//Marca final de EOF

	//**********Salva a cadena

	SwOk = M4_BOOL( SwOk && Serial.SaveToStr(ao_pcSerialOut, aoi_puiSerialOutSize ) ) ;

exit:

	return SwOk ;
}


void ClENG_SessReportDesigner::GetCT_WizardDataTotalize( 
	TotalMap_t *ao_poMap, m4char_t *ai_pcCTId, StENG_DT_SectionData *ai_poSection ) 
{
	m4int16_t B ;
	StENG_DT_SectionData::CntList_t::iterator CntIt ;
	StENG_DT_CntData *Cnt ;
	ENG_DT_FieldList_t::iterator FieldIt ;
	StENG_DT_FieldData *Field ;
	ClMIT_Str TotId, Func, Key, CTId, Tree ;
	TotalMap_t::iterator TotIt ;
	m4uint32_t Incr, Index ;

	if (!ai_poSection) return ;

	//Recorre Cnt
	for ( CntIt = ai_poSection->m_oCnt.begin(); CntIt != ai_poSection->m_oCnt.end(); ++CntIt ) {

		Cnt = &(*CntIt).second ;

		//Selecciona solo Cnt fusionados 
		if ( ClMIT_Str::StrICmpTrim(Cnt->m_oCTId,ai_pcCTId)!=0 ) continue ;

		//Recorre Fields

		for ( FieldIt = Cnt->m_oFieldList.begin(); FieldIt != Cnt->m_oFieldList.end(); ++FieldIt ) {

			Field =  &(*FieldIt) ;

			//Selecciona solo Fields fusionados
			
			if (Field->m_oIdCtField.StrLenTrim()==0) continue ;

			//Hayar info a meter, se van detectando funciones %CT ...

			Index=Incr=0 ;
			do {
				Index += Incr ;
				Incr=ClENG_TkCTTot::GetNextCTInfo(Field->m_oExpr.InChar(Index), &CTId, &Tree, &TotId, &Func ) ;
				if (Incr==0) break ;
			
				//Filtros; solo si CTId correcto, y func ni RESET ni ENTRY

				if ( ClMIT_Str::StrICmpTrim(CTId,ai_pcCTId) != 0 || 
					 ClMIT_Str::StrICmpTrim(Func,"RESET")==0 || 
					 ClMIT_Str::StrICmpTrim(Func,"ENTRY")==0 || 
					 ClMIT_Str::StrICmpTrim(Func,"HEADER")==0 || 
					 ClMIT_Str::StrICmpTrim(Func,"KEY")==0 ) 
				{
					continue ;
				} ;
			
				//Monta clave
	
				TotId.Trim() ;	Func.Trim() ; Func.ToUpper() ;
				Key.StrCpy(TotId) ;  Key.StrCat(":") ; Key.StrCat(Func) ;
			
				//Solo la mete si no existe

				TotIt = ao_poMap->find(Key) ;
				if ( TotIt==ao_poMap->end() ) {
					ao_poMap->insert( TotalMap_t::value_type(Key, TotId.StrLen() ) ) ;
				} ;
			} while (M4_TRUE) ;

		} ;


	} ;


	//Procesa hijos

	for ( B = 0; B < ai_poSection->m_iSonSectionCount; ++B ) {
		GetCT_WizardDataTotalize( ao_poMap, ai_pcCTId, &ai_poSection->m_oSonSection[B].m_oSection ) ;
	} ;
}


void ClENG_SessReportDesigner::GetCT_WizardDataRow( 
	ClENG_VB_Serialize *ao_poSerial, m4char_t *ai_pcCTId, StENG_DT_SectionData *ai_poSection ) 
{
	m4int16_t B ;
	StENG_DT_SectionData::CntList_t::iterator CntIt ;
	StENG_DT_CntData *Cnt ;
	m4int16_t CountMerged ;

	if (!ai_poSection) return ;

	//Recorre Cnt
	for ( CntIt = ai_poSection->m_oCnt.begin(), CountMerged=0; CntIt != ai_poSection->m_oCnt.end(); ++CntIt ) {

		Cnt = &(*CntIt).second ;

		//Selecciona solo Cnt fusionados 
		if ( ClMIT_Str::StrICmpTrim(Cnt->m_oCTId,ai_pcCTId)!=0 ) continue ;

		++CountMerged ;

	} ;

	//Procesa solo si hay alguno fusionado

	if (CountMerged>0) {
		ao_poSerial->SerializeBool(M4_FALSE) ;	//Not EOF
		ao_poSerial->SerializeStr( ai_poSection->m_GoT != ClENG_VR::SLICE_GOT ?
				ai_poSection->m_RecordsetNodeName : ai_poSection->m_GuideNode ) ;	
		if ( ai_poSection->m_oGroupKey.StrLenTrim()>0 ) {
			ao_poSerial->SerializeStr(ai_poSection->m_oGroupKey) ;	
		} else {
			ao_poSerial->SerializeStr("") ;	
		} ;
	} ;


	//Procesa hijos

	for ( B = 0; B < ai_poSection->m_iSonSectionCount; ++B ) {
		GetCT_WizardDataRow( ao_poSerial, ai_pcCTId, &ai_poSection->m_oSonSection[B].m_oSection ) ;
	} ;
}
