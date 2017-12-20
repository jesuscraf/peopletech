//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             drvAPI.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
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


#include "rfn.h"
#include "rfndf.inl"
#include "rfntrans.h"
#include "rfnfilew.h"
#include "fontm.h"
#include "rfnstrt.h"

#define GROUP_ID RFN_TGI_OTHERS
#define RFN ClFONT_M_RFNStruct		//Para abreviar!

//==============================================================================ClRFN_ParsePAR

#include "flexlexer.h"
#include "rfnparser.h"
#include "m4unicode.hpp"


ClRFN_ParsePAR::ClRFN_ParsePAR() 
{
	m_poInsp=GET_INSP();
}

void ClRFN_ParsePAR::End() 
{
	m_oVbleDesc.clear() ;
	m_oVbleStack.clear() ;
	m_oVbleName.End() ; m_oLiteralData.End() ;
	m_oSelection.Reset() ;
	m_oBlockData.Reset() ;
	m_oHeaderData.Reset() ;
	m_oHeightData.clear() ;
}


void ClRFN_ParsePAR::Init( )
{
	StDescVbleData  Data ; 
	m4int16_t B ;
	ClMIT_Str Vble ;

	ClRFN_ParsePAR::End() ;

	//Crear entradas m_oVbleDesc

	//***Variables solo HEADER y FOR_ALL, obligatorias

	Data.m_SuitInHeader = REQUIRED_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE ;
	Data.m_Type			= STR_VBLE ;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=NOT_SUIT_VBLE ;
	} ;

	Vble.StrCpy("OutputName") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	Vble.StrCpy("FontName") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	Vble.StrCpy("Fixed") ;
	Data.m_Type			= BOOL_VBLE ;

	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//***Variables solo HEIGHT y FOR_ALL, obligatorias
	
	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = REQUIRED_VBLE ;
	Data.m_Type			= NUM_VBLE ;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=NOT_SUIT_VBLE ;
	} ;

	Vble.StrCpy("DesignHeight") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	Vble.StrCpy("DesignWidth") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	
	//***Variables HEADER/HEIGHT y Dvc, obligatorias HEADER, opcionales HEIGHT
	
	//Tipo igual en todos Dvcs

	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=REQUIRED_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=OPT_DI_VBLE ;
		Data.m_DvcSuit[B].m_oInheritFrom.StrCpy("MapName") ;
		Data.m_DvcSuit[B].m_Type		=STR_VBLE ;
	} ;
 
	Vble.StrCpy("MapName") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 
	
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_oInheritFrom.StrCpy("DvcPitch") ;
		Data.m_DvcSuit[B].m_Type		 =NUM_VBLE ;
	} ;

	Vble.StrCpy("DvcPitch") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//***Variables HEIGHT y Dvc, opcionales e igual tipo en todos Dvc
	
	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=OPT_FAI_VBLE ;
		Data.m_DvcSuit[B].m_oInheritFrom.StrCpy("DesignHeight") ;
		Data.m_DvcSuit[B].m_Type		=NUM_VBLE ;
	} ;
 
	Vble.StrCpy("MapHeight") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//***Variables HEIGHT, FOR_ALL y Dvc, obligatorias  FOR_ALL, opcionales Dvc e igual tipo en todos Dvc
	
	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = REQUIRED_VBLE;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=OPT_FAI_VBLE ;
		Data.m_DvcSuit[B].m_oInheritFrom.StrCpy("DvcHeight") ;
		Data.m_DvcSuit[B].m_Type		=NUM_VBLE ;
	} ;
 
	Vble.StrCpy("DvcHeight") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//**

	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_oInheritFrom.StrCpy("SelectionHeight") ;
	} ;
 
	Vble.StrCpy("SelectionHeight") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 


	//***Variables HEADER/HEIGHT y Dvc, obligatorias HEADER, opcionales HEIGHT, varios Dvc

	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE;

	Data.m_DvcSuit[RFN::PCL].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_oInheritFrom.StrCpy("DvcFontId") ;
	Data.m_DvcSuit[RFN::PCL].m_Type		=NUM_VBLE ;

	Data.m_DvcSuit[RFN::PSC].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_oInheritFrom.StrCpy("DvcFontId") ;
	Data.m_DvcSuit[RFN::PSC].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::RVW].m_SuitInHeader=NOT_SUIT_VBLE ;
	Data.m_DvcSuit[RFN::RVW].m_SuitInHeight=NOT_SUIT_VBLE ;

	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::ESCP].m_oInheritFrom.StrCpy("DvcFontId") ;
	Data.m_DvcSuit[RFN::ESCP].m_Type		 =NUM_VBLE ;

	Vble.StrCpy("DvcFontId") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//**

	Data.m_DvcSuit[RFN::PCL].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_oInheritFrom.StrCpy("DvcSoftFont") ;
	Data.m_DvcSuit[RFN::PCL].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::PSC].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_oInheritFrom.StrCpy("DvcSoftFont") ;
	Data.m_DvcSuit[RFN::PSC].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::RVW].m_SuitInHeader=NOT_SUIT_VBLE ;
	Data.m_DvcSuit[RFN::RVW].m_SuitInHeight=NOT_SUIT_VBLE ;

	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeader=NOT_SUIT_VBLE ;
	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeight=NOT_SUIT_VBLE ;

	Vble.StrCpy("DvcSoftFont") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 


	//**

	Data.m_DvcSuit[RFN::PCL].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_oInheritFrom.StrCpy("DvcSoftFontTrans") ;
	Data.m_DvcSuit[RFN::PCL].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::PSC].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PSC].m_oInheritFrom.StrCpy("DvcSoftFontTrans") ;
	Data.m_DvcSuit[RFN::PSC].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::RVW].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::RVW].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::RVW].m_oInheritFrom.StrCpy("DvcSoftFontTrans") ;
	Data.m_DvcSuit[RFN::RVW].m_Type		=STR_VBLE ;

	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeader=NOT_SUIT_VBLE ;
	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeight=NOT_SUIT_VBLE ;

	Vble.StrCpy("DvcSoftFontTrans") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 


	//***Variables HEADER/HEIGHT y Dvc, obligatorias HEADER, opcionales HEIGHT, solo PCL

	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=NOT_SUIT_VBLE ;
	} ;
	Data.m_DvcSuit[RFN::PCL].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_SuitInHeight=OPT_DI_VBLE ;
	Data.m_DvcSuit[RFN::PCL].m_oInheritFrom.StrCpy("DvcCharSet") ;
	Data.m_DvcSuit[RFN::PCL].m_Type		=STR_VBLE ;

	Vble.StrCpy("DvcCharSet") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	//***Variables HEADER/HEIGHT y Dvc, obligatorias HEADER, opcionales HEIGHT, solo ESCP
	
	Data.m_SuitInHeader = NOT_SUIT_VBLE ;
	Data.m_SuitInHeight = NOT_SUIT_VBLE;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		Data.m_DvcSuit[B].m_SuitInHeader=NOT_SUIT_VBLE ;
		Data.m_DvcSuit[B].m_SuitInHeight=NOT_SUIT_VBLE ;
	} ;
	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeader=REQUIRED_VBLE ;
	Data.m_DvcSuit[RFN::ESCP].m_SuitInHeight=OPT_DI_VBLE ;

	Data.m_DvcSuit[RFN::ESCP].m_Type		 =BOOL_VBLE ;

	Vble.StrCpy("DvcDY") ;
	Data.m_DvcSuit[RFN::ESCP].m_oInheritFrom.StrCpy("DvcDY") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	Vble.StrCpy("DvcDX") ;
	Data.m_DvcSuit[RFN::ESCP].m_oInheritFrom.StrCpy("DvcDX") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	Vble.StrCpy("DvcCompr") ;
	Data.m_DvcSuit[RFN::ESCP].m_oInheritFrom.StrCpy("DvcCompr") ;
	m_oVbleDesc.insert( VbleDesc_t::value_type( Vble,Data) ) ; 

	return ;
}

m4bool_t ClRFN_ParsePAR::ParsePAR(
	m4char_t *ai_pcFileName,
	m4char_t * ai_pcPCL_Driver, m4char_t * ai_pcESCP_Driver, 
	m4char_t *ai_pcRVW_Driver, 	m4char_t * ai_pcPSC_Driver ) 
{
	m4bool_t SwOk = M4_TRUE ;
	struct StBlockData * Block ;
	VbleList_t *List ;
	m4int16_t B ;
	m4bool_t SwBold, SwItalic, SwUnderline ;

	ifstream InStream ;
	ClRFN_FlexLexer Lexer(&InStream,&cout) ;
	ClRFN_Parser    Parser ;
	ClRFN_RFN_W     RFN ;
	BlockDataList_t::iterator It ;

	#ifdef RFN_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClRFN_ParsePAR::ParsePAR(), File %0:s", ClMIT_Msg::ENTRY_FORMAT ) 
			<< ai_pcFileName << SEND_MSG ;
	#endif

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    M4Open( InStream, ai_pcFileName, M4UniRead, eEncoding );

	if ( ! InStream.is_open() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error openning file %0:s" ) 
				<< ai_pcFileName << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	Lexer.LinkParser(&Parser) ;
	Lexer.LinkParserPAR(this) ;

	Parser.LinkLexer(&Lexer) ;
	Parser.LinkParserPAR(this) ;
	Parser.SetDebug(M4_FALSE) ;

	//=================>Parseo

	Reset() ;
	SwOk = Parser.yyparse()==0 ;

	//<========================

	if (!SwOk) goto exit ;

	RFN.Init() ;

	if ( ! RFN.PrepareFile( 
		ai_pcPCL_Driver, ai_pcESCP_Driver, ai_pcRVW_Driver, ai_pcPSC_Driver,
		GetStrVble(&m_oHeaderData.m_oVble,"OutputName"),
		GetStrVble(&m_oHeaderData.m_oVble,"FontName"),
		ai_pcFileName,
		m_oHeightData.size(), 
		GetBoolVble(&m_oHeaderData.m_oVble,"Fixed"),
		m_oHeaderData.m_oDvcBlock[RFN::PCL].m_SwActivated,
		m_oHeaderData.m_oDvcBlock[RFN::PSC].m_SwActivated,
		m_oHeaderData.m_oDvcBlock[RFN::RVW].m_SwActivated,
		m_oHeaderData.m_oDvcBlock[RFN::ESCP].m_SwActivated 
		) )
	{
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	for ( It=m_oHeightData.begin(); It!=m_oHeightData.end(); ++It ) {

		Block=&(*It).second ;

		RFN.BeginHeightBlock( 
			GetLongVble(&Block->m_oVble,"DesignHeight"),
			GetLongVble(&Block->m_oVble,"DesignWidth")
		) ;

		//Datos PCL

		if ( RFN.m_oHeaderData.m_bSwOn[RFN::PCL] ) 
		{
			for (B=0; B<RFN::SELECTION_COUNT; ++B) {
				if ( !Block->m_oDvcBlock[RFN::PCL].m_oDvcSelection.m_SwItemLoaded[B]) continue ;
				List=&Block->m_oDvcBlock[RFN::PCL].m_oDvcSelection.m_oDvcVble[B] ;
				RFN::UnpackSIndex(B, &SwBold, &SwItalic, &SwUnderline ) ;
				if ( ! RFN.SelectionDataPCL( 
					SwBold, SwItalic, SwUnderline,
					GetStrVble(List,"MapName"), 
					GetLongVble(List,"MapHeight")/100.0, GetLongVble(List,"DvcPitch")/100.0,
					GetLongVble(List,"DvcHeight")/100.0, GetLongVble(List,"SelectionHeight")/100.0, 
					GetLongVble(List,"DvcFontId"),	GetStrVble(List,"DvcCharSet"),
					GetStrVble(List,"DvcSoftFont"), GetStrVble(List,"DvcSoftFontTrans")
					) ) 
				{
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

			} ;
	
		} ;

		//Datos PSC

		if ( RFN.m_oHeaderData.m_bSwOn[RFN::PSC] ) 
		{
			for (B=0; B<RFN::SELECTION_COUNT; ++B) {
				if ( !Block->m_oDvcBlock[RFN::PSC].m_oDvcSelection.m_SwItemLoaded[B]) continue ;
				List=&Block->m_oDvcBlock[RFN::PSC].m_oDvcSelection.m_oDvcVble[B] ;
				RFN::UnpackSIndex(B, &SwBold, &SwItalic, &SwUnderline ) ;
				if ( ! RFN.SelectionDataPSC( 
					SwBold, SwItalic, SwUnderline,
					GetStrVble(List,"MapName"), 
					GetLongVble(List,"MapHeight")/100.0, GetLongVble(List,"DvcPitch")/100.0,
					GetLongVble(List,"DvcHeight")/100.0, GetLongVble(List,"SelectionHeight")/100.0, 
					GetStrVble(List,"DvcFontId"), GetStrVble(List,"DvcSoftFont"), 
					GetStrVble(List,"DvcSoftFontTrans") 
					) ) 
				{
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

			} ;
	
		} ;

		//Datos RVW

		if ( RFN.m_oHeaderData.m_bSwOn[RFN::RVW] ) 
		{
			for (B=0; B<RFN::SELECTION_COUNT; ++B) {
				if ( !Block->m_oDvcBlock[RFN::RVW].m_oDvcSelection.m_SwItemLoaded[B]) continue ;
				List=&Block->m_oDvcBlock[RFN::RVW].m_oDvcSelection.m_oDvcVble[B] ;
				RFN::UnpackSIndex(B, &SwBold, &SwItalic, &SwUnderline ) ;
				if ( ! RFN.SelectionDataRVW( 
					SwBold, SwItalic, SwUnderline,
					GetStrVble(List,"MapName"), 
					GetLongVble(List,"MapHeight")/100.0, GetLongVble(List,"DvcPitch")/100.0,
					GetLongVble(List,"DvcHeight")/100.0, GetLongVble(List,"SelectionHeight")/100.0, 
					GetStrVble(List,"DvcSoftFontTrans")
					) ) 
				{
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

			} ;
	
		} ;

		
		//Datos ESCP

		if ( RFN.m_oHeaderData.m_bSwOn[RFN::ESCP] ) 
		{
			for (B=0; B<RFN::SELECTION_COUNT; ++B) {
				if ( !Block->m_oDvcBlock[RFN::ESCP].m_oDvcSelection.m_SwItemLoaded[B]) continue ;
				List=&Block->m_oDvcBlock[RFN::ESCP].m_oDvcSelection.m_oDvcVble[B] ;
				RFN::UnpackSIndex(B, &SwBold, &SwItalic, &SwUnderline ) ;
				if ( ! RFN.SelectionDataESCP( 
					SwBold, SwItalic, SwUnderline,
					GetStrVble(List,"MapName"), 
					GetLongVble(List,"MapHeight")/100.0, GetLongVble(List,"DvcPitch")/100.0,
					GetLongVble(List,"DvcHeight")/100.0, GetLongVble(List,"SelectionHeight")/100.0, 
					GetLongVble(List,"DvcFontId"), 
					GetBoolVble(List,"DvcDY"), 
					GetBoolVble(List,"DvcDX"),
					GetBoolVble(List,"DvcCompr")
					) ) 
				{
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

			} ;
	
		} ;

	
		if ( ! RFN.EndHeightBlock() ) {
			SwOk=M4_FALSE ;
			goto exit ;
		};
	} ;
	

	RFN.EndFile() ;

exit:

	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error parsing file %0:s" ) 
				<< ai_pcFileName << SEND_MSG ;
	} ;

	#ifdef RFN_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClAPISQL_SQLToPT::Translate(), Ok ? %0:s", 
		ClMIT_Msg::EXIT_FORMAT ) 
			<< SwOk << SEND_MSG ;
	#endif

	InStream.close() ;

	return SwOk ;
}





//=============================================================================================

//******************

void ClRFN_ParsePAR::StDescVbleData::Init(const ClRFN_ParsePAR::StDescVbleData& ai_oSource) 
{
	m4int16_t B ;

	m_SuitInHeader = ai_oSource.m_SuitInHeader ;
	m_SuitInHeight = ai_oSource.m_SuitInHeight ;
	m_Type         = ai_oSource.m_Type ;
	m_oInheritFrom.Init(&ai_oSource.m_oInheritFrom) ;

	for (B=0;B<RFN::DVC_COUNT;++B) {
		m_DvcSuit[B].m_SuitInHeader =  ai_oSource.m_DvcSuit[B].m_SuitInHeader ;
		m_DvcSuit[B].m_SuitInHeight =  ai_oSource.m_DvcSuit[B].m_SuitInHeight ;
		m_DvcSuit[B].m_Type         =  ai_oSource.m_DvcSuit[B].m_Type ; 
		m_DvcSuit[B].m_oInheritFrom.Init(&ai_oSource.m_DvcSuit[B].m_oInheritFrom) ;
	} ;
}

//******************


void ClRFN_ParsePAR::StVbleValue::Init(const ClRFN_ParsePAR::StVbleValue& ai_oSource) 
{
	m_Type = ai_oSource.m_Type ;
	m_oValue.Init(&ai_oSource.m_oValue) ;
}


//******************

void ClRFN_ParsePAR::StSelectionBlock::Init(const ClRFN_ParsePAR::StSelectionBlock& ai_oSource) 
{
	m4int16_t B ;

	for (B=0;B<RFN::SELECTION_COUNT;++B) {
		m_SwItemLoaded[B] = ai_oSource.m_SwItemLoaded[B] ;
		m_oDvcVble[B] = ai_oSource.m_oDvcVble[B] ;
	} ;
}

void ClRFN_ParsePAR::StSelectionBlock::Reset() 
{
	m4int16_t B ;

	for (B=0;B<RFN::SELECTION_COUNT;++B) {
		m_SwItemLoaded[B] = M4_FALSE ;
		m_oDvcVble[B].clear() ;
	} ;
}

//******************

void ClRFN_ParsePAR::StBlockData::Init(const ClRFN_ParsePAR::StBlockData& ai_oSource) 
{
	m4int16_t B ;

	m_oVble = ai_oSource.m_oVble ;
	m_SwForAllBlockLoaded =  ai_oSource.m_SwForAllBlockLoaded;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		m_oDvcBlock[B].m_SwDvcBlockLoaded = ai_oSource.m_oDvcBlock[B].m_SwDvcBlockLoaded ;
		m_oDvcBlock[B].m_SwActivated	  = ai_oSource.m_oDvcBlock[B].m_SwActivated ;
		m_oDvcBlock[B].m_oDvcSelection	  = ai_oSource.m_oDvcBlock[B].m_oDvcSelection ;
	} ;
}

void ClRFN_ParsePAR::StBlockData::Reset() 
{
	m4int16_t B ;

	m_oVble.clear() ;
	m_SwForAllBlockLoaded = M4_FALSE ;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		m_oDvcBlock[B].m_SwDvcBlockLoaded = M4_FALSE ;
		m_oDvcBlock[B].m_SwActivated	  = M4_TRUE ;
		m_oDvcBlock[B].m_oDvcSelection.Reset() ;
	} ;
}

//=============================================================================================

void ClRFN_ParsePAR::Reset() 
{
	m4int16_t B ;

	m_oVbleStack.clear() ;

	m_oSelection.Reset() ;

	m_oBlockData.Reset() ;
	
	m_oHeaderData.Reset() ;
	m_oHeightData.clear() ;

	m_bSwHeaderLoaded=M4_FALSE ;
}

m4bool_t ClRFN_ParsePAR::PushVbleOnStack( m4char_t *ai_pcVbleName, VBLE_TYPE ai_Type, m4char_t *ai_pcValue ) 
{
	m4bool_t SwOk=M4_TRUE ;
	pair<VbleList_t::iterator,m4bool_t> It ;
	StVbleValue Value ;
	ClMIT_Str VbleName ;

	if ( ! CheckVbleName(ai_pcVbleName) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	VbleName.StrCpy(ai_pcVbleName) ;
	Value.m_Type=ai_Type ;
	Value.m_oValue.StrCpy(ai_pcValue) ;

	It = m_oVbleStack.insert( VbleList_t::value_type( VbleName,Value) ) ; 
	if ( ! It.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Error, can not push variable %0:s. Probably it's duplicated." ) 
				<< VbleName.InChar(0) << SEND_MSG ;
		SwOk = M4_FALSE ;
	}  else {
		SwOk = M4_TRUE ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClRFN_ParsePAR::ReduceSelection( BOLD_SELECT ai_Bold, ITALIC_SELECT ai_Italic, UNDERLINE_SELECT ai_Underline ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	m4bool_t IndexBold, IndexItalic, IndexUnderline ;

	for ( B=0; B<RFN::SELECTION_COUNT; ++B ) {
		RFN::UnpackSIndex(B, &IndexBold, &IndexItalic, &IndexUnderline ) ;
		if ( ai_Bold != BOLD_ALL ) {
			if ( IndexBold ) {
				if (ai_Bold==BOLD_OFF ) continue ;
			} else {
				if (ai_Bold==BOLD_ON ) continue ;
			} ;
		} ;
		if ( ai_Italic != ITALIC_ALL ) {
			if ( IndexItalic ) {
				if (ai_Italic==ITALIC_OFF ) continue ;
			} else {
				if (ai_Italic==ITALIC_ON ) continue ;
			} ;
		} ;
		if ( ai_Underline != UNDERLINE_ALL ) {
			if ( IndexUnderline ) {
				if (ai_Underline==UNDERLINE_OFF ) continue ;
			} else {
				if (ai_Underline==UNDERLINE_ON ) continue ;
			} ;
		} ;
		if ( m_oSelection.m_SwItemLoaded[B] ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Selection duplicated" ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			break ;
		} ;
		m_oSelection.m_SwItemLoaded[B] = M4_TRUE ;
		m_oSelection.m_oDvcVble[B] = m_oVbleStack ;
	} ;

	m_oVbleStack.clear() ;

	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::ReduceForAllBlock() 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( m_oBlockData.m_SwForAllBlockLoaded ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"FOR ALL block duplicated" ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	m_oBlockData.m_SwForAllBlockLoaded = M4_TRUE ;
	m_oBlockData.m_oVble = m_oVbleStack ;

exit:

	m_oVbleStack.clear() ;

	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::ReduceForDvcBlock() 
{
	m4bool_t SwOk=M4_TRUE ;


	if ( m_oBlockData.m_oDvcBlock[m_oBlockType].m_SwDvcBlockLoaded ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"DVC block %0:s duplicated" ) << RFN::GetDvcTraceName(m_oBlockType) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	m_oBlockData.m_oDvcBlock[m_oBlockType].m_SwDvcBlockLoaded = M4_TRUE ;
	m_oBlockData.m_oDvcBlock[m_oBlockType].m_oDvcSelection = m_oSelection ;


exit:

	m_oSelection.Reset() ;

	return SwOk ;
}


void ClRFN_ParsePAR::ClearDvcActivatedFlag() 
{
	m_oBlockData.m_oDvcBlock[m_oBlockType].m_SwActivated = M4_FALSE ;
}


m4bool_t ClRFN_ParsePAR::ReduceHeaderBlock() 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B,B2 ;

	if ( m_bSwHeaderLoaded ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"HEDER block duplicated" ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	m_bSwHeaderLoaded = M4_TRUE ;
	m_oHeaderData = m_oBlockData ;

	//Test de todos bloques cargados

	if ( ! m_oHeaderData.m_SwForAllBlockLoaded ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"FOR ALL block missing in HEADER block" ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	for (B=0;B<RFN::DVC_COUNT;++B) {
		if ( ! m_oHeaderData.m_oDvcBlock[B].m_SwDvcBlockLoaded ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"DVC block %0:s missing in HEADER block" ) 
				<< RFN::GetDvcTraceName(B) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	
	//Test CheckVbleList bloque FOR ALL

	if ( ! CheckVbleList(  &m_oHeaderData.m_oVble, M4_TRUE, M4_TRUE) ) {
		SwOk=M4_FALSE ;
	} ;

	//Test CheckVbleList bloques DVC

	for (B=0;B<RFN::DVC_COUNT;++B) {
		if ( m_oHeaderData.m_oDvcBlock[B].m_SwActivated ) {
			for (B2=0;B2<RFN::SELECTION_COUNT;++B2) {
				if ( m_oHeaderData.m_oDvcBlock[B].m_oDvcSelection.m_SwItemLoaded[B2] ) {
					if ( ! CheckVbleList( &m_oHeaderData.m_oDvcBlock[B].
							m_oDvcSelection.m_oDvcVble[B2], M4_TRUE, M4_FALSE, 
							(RFN::DVC)B) ) 
					{
						SwOk=M4_FALSE ;
					} ;
				} ;
			} ;
		} ;
	} ;


	//SetDefaults
	
	if ( ! SetBlockDefaults( NULL, &m_oHeaderData ) ) {
		SwOk=M4_FALSE ;
	} ;


exit:
	m_oBlockData.Reset() ;

	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::ReduceHeightBlock() 
{
	m4bool_t SwOk=M4_TRUE ;
	pair<BlockDataList_t::iterator,m4bool_t> It ;
	m4int16_t B, B2 ;
	m4int32_t DesignHeight ;
	
	//Test de bloques FOR ALL cargado

	if ( ! m_oBlockData.m_SwForAllBlockLoaded ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"FOR ALL block missing in HEIGHT block" ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	
	//Bloques Dvc en HEADER cargados y activados exigen lo mismo en HEIGHT. Los demas se 
	//	desactivan.

	for (B=0;B<RFN::DVC_COUNT;++B) {
		if ( m_oHeaderData.m_oDvcBlock[B].m_SwDvcBlockLoaded && 
			m_oHeaderData.m_oDvcBlock[B].m_SwActivated ) 
		{
			if ( ! m_oBlockData.m_oDvcBlock[B].m_SwDvcBlockLoaded || 
				 ! m_oBlockData.m_oDvcBlock[B].m_SwActivated )
			{
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Dvc block %0:s missing in HEIGHT block and exist in HEADER block" ) 
					<< RFN::GetDvcTraceName(B) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} else {
			m_oBlockData.m_oDvcBlock[B].m_SwActivated=M4_FALSE ;
		} ;
	} ;


	//Test CheckVbleList bloque FOR ALL

	if ( ! CheckVbleList(  &m_oBlockData.m_oVble, M4_FALSE, M4_TRUE ) ) {
		SwOk=M4_FALSE ;
	} ;

	//Test CheckVbleList bloques DVC

	for (B=0;B<RFN::DVC_COUNT;++B) {
		if ( m_oBlockData.m_oDvcBlock[B].m_SwDvcBlockLoaded && 
			 m_oBlockData.m_oDvcBlock[B].m_SwActivated ) 
		{
			for (B2=0;B2<RFN::SELECTION_COUNT;++B2) {
				if ( m_oBlockData.m_oDvcBlock[B].m_oDvcSelection.m_SwItemLoaded[B2] ) {
					if ( ! CheckVbleList( &m_oBlockData.m_oDvcBlock[B].
							m_oDvcSelection.m_oDvcVble[B2], M4_FALSE, M4_FALSE, 
							(RFN::DVC)B) ) 
					{
						SwOk=M4_FALSE ;
					} ;
				} ;
			} ;
		} ;
	} ;
	
	
	//SetDefaults
	
	if ( ! SetBlockDefaults( &m_oHeaderData, &m_oBlockData ) ) {
		SwOk=M4_FALSE ;
	} ;

	if (!SwOk) goto exit ;	//No pasar a insertar si hay errores

	
	// Necesita DesignHeight para insertar

	DesignHeight = GetLongVble(&m_oBlockData.m_oVble,"DesignHeight") ;

	It = m_oHeightData.insert( BlockDataList_t::value_type( DesignHeight,m_oBlockData) ) ; 
	if ( ! It.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"HEIGHT block duplicated" ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;



exit:

	m_oBlockData.Reset() ;

	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::CheckVbleName(m4char_t *ai_pcVbleName,StDescVbleData **ao_poData) 
{
	m4bool_t SwOk=M4_TRUE ;
	VbleDesc_t::iterator It ;
	ClMIT_Str VbleName ;

	VbleName.StrCpy(ai_pcVbleName) ;

	It = m_oVbleDesc.find(VbleName) ;
	if (It==m_oVbleDesc.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Variable Name %0:s unknow" ) << ai_pcVbleName << SEND_MSG ;
		SwOk=M4_FALSE ;
	} else {
		if ( ao_poData ) *ao_poData = &(*It).second ;
		SwOk=M4_TRUE ;
	} ;
	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::CheckVbleList(  
	VbleList_t *ai_poVbleList, 
	m4bool_t ai_bSwInHeader, m4bool_t ai_bSwForAll, RFN::DVC ai_Dvc) 
{
	m4bool_t SwOk=M4_TRUE ;
	VbleList_t::iterator It ;
	VbleDesc_t::iterator DescIt ;
	StDescVbleData *VbleDesc ;
	VBLE_SUITABLE Suitable ;
	VBLE_TYPE     TypeSuitable ;
	
	
	//Chequeo de que los que existen deben existir y son de tipo correcto

	for( It=ai_poVbleList->begin(); It != ai_poVbleList->end() ; ++It ) {

		if ( ! CheckVbleName(((*It).first).InChar(0), &VbleDesc) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( ai_bSwForAll ) {
			Suitable     = ai_bSwInHeader ? VbleDesc->m_SuitInHeader : VbleDesc->m_SuitInHeight  ;
			TypeSuitable = VbleDesc->m_Type ;
		} else {
			Suitable     = ai_bSwInHeader ? VbleDesc->m_DvcSuit[ai_Dvc].m_SuitInHeader : 
											VbleDesc->m_DvcSuit[ai_Dvc].m_SuitInHeight  ;
			TypeSuitable = VbleDesc->m_DvcSuit[ai_Dvc].m_Type ;
		} ;
		
		if ( Suitable==NOT_SUIT_VBLE ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Variable %0:s not suitable in this context. HEADER %1:s FOR ALL %2:s DVC %3:s" ) 
				<< (*It).first.InChar(0) << ai_bSwInHeader << ai_bSwForAll 
				<< RFN::GetDvcTraceName(ai_Dvc)  << SEND_MSG ;
			SwOk=M4_FALSE ;
		} else {
			if ( (*It).second.m_Type != TypeSuitable ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Variable %0:s , incorrect type in this context. HEADER %1:s FOR ALL %2:s DVC %3:s" ) 
					<< (*It).first.InChar(0) << ai_bSwInHeader << ai_bSwForAll 
					<< RFN::GetDvcTraceName(ai_Dvc)  << SEND_MSG ;
				SwOk=M4_FALSE ;
			} ;
		} ;
	} ;

	//Chequear que existen las variables obligatorias.

	for( DescIt=m_oVbleDesc.begin(); DescIt != m_oVbleDesc.end() ; ++DescIt ) {
		if ( ai_bSwForAll ) {
			Suitable     = ai_bSwInHeader ? (*DescIt).second.m_SuitInHeader : 
											(*DescIt).second.m_SuitInHeight  ;
		} else {
			Suitable     = ai_bSwInHeader ? (*DescIt).second.m_DvcSuit[ai_Dvc].m_SuitInHeader : 
											(*DescIt).second.m_DvcSuit[ai_Dvc].m_SuitInHeight  ;
		} ;
		if ( Suitable==REQUIRED_VBLE ) {
			It=ai_poVbleList->find( (*DescIt).first ) ;
			if ( It==ai_poVbleList->end() ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Variable %0:s not found, required in this context. HEADER %1:s FOR ALL %2:s DVC %3:s" ) 
					<< (*DescIt).first.InChar(0) << ai_bSwInHeader << ai_bSwForAll 
					<< RFN::GetDvcTraceName(ai_Dvc)  << SEND_MSG ;
				SwOk=M4_FALSE ;
			} ;
		} ;
	} ;


exit:

	return SwOk ;	
}


m4bool_t ClRFN_ParsePAR::SetBlockDefaults( 
	ClRFN_ParsePAR::StBlockData *ai_poHeaderBlock, ClRFN_ParsePAR::StBlockData *ai_poBlockData ) 
{
	m4bool_t SwOk=M4_TRUE ;
	VbleList_t::iterator It ;
	VbleList_t *VbleList ;
	VbleDesc_t::iterator DescIt ;
	StDescVbleData *VbleDesc ;
	VBLE_SUITABLE Suitable ;
	VBLE_TYPE     TypeSuitable ;
	m4bool_t SwHeader=!ai_poHeaderBlock;

	m4int16_t B, B2 ;

	//---->Defectos de bloques FOR ALL

	//Por ahora, no pueden tener 

	//---->Defectos de bloques Dvc

	for (B=0;B<RFN::DVC_COUNT;++B) {
		if ( ! ai_poBlockData->m_oDvcBlock[B].m_SwDvcBlockLoaded || 
			 ! ai_poBlockData->m_oDvcBlock[B].m_SwActivated ) 
		{
			continue ;
		} ;

		for (B2=0;B2<RFN::SELECTION_COUNT;++B2) {

			if ( ! ai_poBlockData->m_oDvcBlock[B].m_oDvcSelection.m_SwItemLoaded[B2] ) {
				continue ;
			} ;

			//Si llega aqui, Item Selector cargado en un block Dvc cargado y activo

			VbleList=&ai_poBlockData->m_oDvcBlock[B].m_oDvcSelection.m_oDvcVble[B2] ;

			//Ahora buscamos variables opcionales no cargadas explicitamente

			for( DescIt=m_oVbleDesc.begin(); DescIt != m_oVbleDesc.end() ; ++DescIt ) 
			{
				Suitable     = SwHeader ? 
								(*DescIt).second.m_DvcSuit[B].m_SuitInHeader : 
								(*DescIt).second.m_DvcSuit[B].m_SuitInHeight  ;

				if( Suitable != OPT_DI_VBLE && Suitable != OPT_FAI_VBLE ) continue ;

				It=VbleList->find( (*DescIt).first ) ;
				if ( It != VbleList->end() ) continue ;

				//Y si llega aqui, en DescIt vble opcional no cargada

				if ( Suitable==OPT_DI_VBLE ) {

					//Copiar del Dvc homologo en HEADER. No permitido en HEADER

					if ( SwHeader ) {
						//No permitido en HEADER
						*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
							"Optional variable %0:s not specified and default unavalable "
							"HEADER %1:s FOR ALL %2:s Dvc %3:s" ) 
							<< (*DescIt).first.InChar(0) << SwHeader
							<< (m4bool_t)M4_FALSE << RFN::GetDvcTraceName(B) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					}  ;

					It=ai_poHeaderBlock->m_oDvcBlock[B].m_oDvcSelection.
							m_oDvcVble[B2].find( (*DescIt).second.m_DvcSuit[B].m_oInheritFrom ) ;
					if ( It==ai_poHeaderBlock->m_oDvcBlock[B].m_oDvcSelection.
						m_oDvcVble[B2].end() ) 
					{
						*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
							"Optional variable %0:s not specified and default unavalable "
							"HEADER %1:s FOR ALL %2:s Dvc %3:s" ) 
							<< (*DescIt).first.InChar(0) << SwHeader
							<< (m4bool_t)M4_FALSE << RFN::GetDvcTraceName(B) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} else {
						VbleList->insert( 
							VbleList_t::value_type( (*DescIt).first, (*It).second ) ) ;
					} ;


				} else if ( Suitable==OPT_FAI_VBLE ) {

					//Copiar del bloque FOR ALL del bloque propio. Permitido HEADER/HEIGHT

					It=ai_poBlockData->m_oVble.find( (*DescIt).second.m_DvcSuit[B].m_oInheritFrom ) ;
					if ( It==ai_poBlockData->m_oVble.end() ) {
						*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
							"Optional variable %0:s not specified and default unavalable "
							"HEADER %1:s FOR ALL %2:s Dvc %3:s" ) 
							<< (*DescIt).first.InChar(0) << SwHeader 
							<< (m4bool_t)M4_FALSE << RFN::GetDvcTraceName(B) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} else {
						VbleList->insert( 
							VbleList_t::value_type( (*DescIt).first, (*It).second ) ) ;
					} ;

				} ;
				
			} ;
		} ;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClRFN_ParsePAR::GetBoolVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) 
{
	VbleList_t::iterator It;
	ClMIT_Str VbleName ;

	VbleName.StrCpy(ai_pcVbleName) ;
	It = ai_poVbleList->find(VbleName) ;
	if ( It==ai_poVbleList->end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Bool Vble requested %0:s not found" ) << ai_pcVbleName << SEND_MSG ;
		return M4_FALSE ;
	} else {
		if ( (*It).second.m_Type != BOOL_VBLE ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Bool Vble requested %0:s has incorrect type" ) << ai_pcVbleName << SEND_MSG ;
			return M4_FALSE ;
		} else {
			return atol( (*It).second.m_oValue ) == 0 ? M4_FALSE : M4_TRUE ;
		} ;

	} ;
}

m4char_t *	ClRFN_ParsePAR::GetStrVble(		VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) 
{
	VbleList_t::iterator It;
	ClMIT_Str VbleName ;

	VbleName.StrCpy(ai_pcVbleName) ;
	It = ai_poVbleList->find(VbleName) ;
	if ( It==ai_poVbleList->end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"String Vble requested %0:s not found" ) << ai_pcVbleName << SEND_MSG ;
		return "ERROR !!!" ;
	} else {
		if ( (*It).second.m_Type != STR_VBLE ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"String Vble requested %0:s has incorrect type" ) << ai_pcVbleName << SEND_MSG ;
			return "ERROR !!!";
		} else {
			return (*It).second.m_oValue.InChar(0)  ;
		} ;

	} ;
}

m4int32_t ClRFN_ParsePAR::GetLongVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) 
{
	VbleList_t::iterator It;
	ClMIT_Str VbleName ;

	VbleName.StrCpy(ai_pcVbleName) ;
	It = ai_poVbleList->find(VbleName) ;
	if ( It==ai_poVbleList->end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Long Vble requested %0:s not found" ) << ai_pcVbleName << SEND_MSG ;
		return 0 ;
	} else {
		if ( (*It).second.m_Type != NUM_VBLE ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Long Vble requested %0:s has incorrect type" ) << ai_pcVbleName << SEND_MSG ;
			return 0 ;
		} else {
			return atol( (*It).second.m_oValue ) ;
		} ;

	} ;
}

m4double_t	ClRFN_ParsePAR::GetDoubleVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) 
{
	VbleList_t::iterator It;
	ClMIT_Str VbleName ;

	VbleName.StrCpy(ai_pcVbleName) ;
	It = ai_poVbleList->find(VbleName) ;
	if ( It==ai_poVbleList->end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Double Vble requested %0:s not found" ) << ai_pcVbleName << SEND_MSG ;
		return 0 ;
	} else {
		if ( (*It).second.m_Type != NUM_VBLE ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Double Vble requested %0:s has incorrect type" ) << ai_pcVbleName << SEND_MSG ;
			return 0 ;
		} else {
			return atof( (*It).second.m_oValue ) ;
		} ;

	} ;
}











