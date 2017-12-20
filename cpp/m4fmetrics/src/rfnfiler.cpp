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

#include "fontm.h"

#include "fontmdf.cpp"

#include "rfnstrt.h"
#include "sysinfo.h"
#include "clmetric.h"

#include "rfnfiler.h"

#include "readrfn.hmg"

#define RFN ClFONT_M_RFNStruct		//Para abreviar!

//==========================================================================ClRFN_RFN_R

ClRFN_RFN_R::ClRFN_RFN_R() 
{
	m_poInsp=GET_INSP();
}

void ClRFN_RFN_R::End() 
{	
	m_oFile.Close() ;
}

void ClRFN_RFN_R::RInt(m4int16_t &ai_Int) 
{
	m_oFile.R(&ai_Int,sizeof(m4int16_t)) ;

	ai_Int = m_BAdapter.EncodeInt(ai_Int) ;
}

void ClRFN_RFN_R::RLong(m4int32_t &ai_Long) 
{
	m_oFile.R(&ai_Long,sizeof(m4int32_t)) ;

	ai_Long = m_BAdapter.EncodeLong(ai_Long) ;
}

void ClRFN_RFN_R::RChar(m4uint8_t &ai_Char) 
{
	m_oFile.R(&ai_Char,sizeof(m4uint8_t)) ;
}


void ClRFN_RFN_R::RStr(ClMIT_Str &ai_Str) 
{
	m4int32_t L ;

	m_oFile.R(&L,sizeof(m4int32_t)) ;
	L = m_BAdapter.EncodeLong(L) ;

	ai_Str.Accommodate(L+1) ;
	m_oFile.R( ai_Str.InChar(0), sizeof(m4char_t)*L ) ;
	*ai_Str.InChar(L) = M4_END_STR ;
	ai_Str.Trim() ;
}

inline void ClRFN_RFN_R::RLongArray( m4int32_t *ai_poLong, size_t ai_NItems) 
{
	m4int16_t B ;

	m_oFile.R(ai_poLong, sizeof(m4int32_t)*ai_NItems) ;

	for (B=ai_NItems;B;--B, ++ai_poLong) {
		*ai_poLong = m_BAdapter.EncodeLong(*ai_poLong) ;
	} ;
}

inline void ClRFN_RFN_R::RIntArray(  m4int16_t *ai_poInt, size_t ai_NItems) 
{
	m4int16_t B ;

	m_oFile.R(ai_poInt, sizeof(m4int16_t)*ai_NItems) ;

	for (B=ai_NItems;B;--B, ++ai_poInt) {
		*ai_poInt = m_BAdapter.EncodeInt(*ai_poInt) ;
	} ;
}

m4bool_t ClRFN_RFN_R::OpenFile(m4char_t *ai_pcFileName) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int32_t LAux ;


	SwOk=m_oFile.Open(ai_pcFileName,ClMIT_File::EXIST,ClMIT_File::READ,ClMIT_File::TRY_OPEN, M4_FALSE) ;
	if (!SwOk) goto exit ;

	//Primero, actualizar el m_BAdapter para que las lecturas de fichero sean correctas

	m_oFile.SetPos(RFN::FILE_STORAGE_OFF) ;
	RChar( Header.FileStorage ) ; 
	m_BAdapter.SetFileStorage((ClMIT_BinaryAdapter::ENDIAN_ORDER)Header.FileStorage);

	//Cuidado con el Id: RStr provoca GP si es no es un fichero RFN y lenght esta mal

	m_oFile.SetPos(RFN::FILE_ID_OFF) ;
	RLong(LAux) ;
	if (LAux != RFN::HEADER_FILE_ID ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ READ_RFN_MSG_10 ] ) 
			<< ai_pcFileName  << SEND_MSG ;
	} ;
	m_oFile.SetPos(RFN::FILE_ID_OFF) ;
	RStr( Header.IdFormat) ;

	if (strcmp(Header.IdFormat,RFN::RFN_FILE_ID)) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ READ_RFN_MSG_10 ] ) 
			<< ai_pcFileName  << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ; 

	//Y ahora filtro de version

	m_oFile.SetPos(RFN::VERSION_OFF) ;
	RInt( Header.Version) ;

	if (Header.Version!=RFN::CURRENT_VERSION) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ READ_RFN_MSG_20 ] ) 
			<< ai_pcFileName  << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//Todo correcto, releemos todo el Header

	m_oFile.SetPos(0) ;

	RChar( Header.FileStorage ) ; 
	RStr( Header.FilePAR) ;
	RStr( Header.IdFormat) ;
	RInt( Header.Version) ;
	RStr( Header.Typeface) ;
	RInt( Header.Variations) ;
	RChar( Header.SwFixed) ;
	RChar( Header.DvcFlag) ;

exit:

	if (!SwOk) m_oFile.Close() ;
	
	return SwOk ;

}

void ClRFN_RFN_R::ReadIndex(m4int16_t ai_Index) 
{
	m_oFile.SetPos( RFN::HEADER_SIZE+ai_Index*RFN::INDEX_SIZE  ) ;

	RLong( Index.Height ) ;
	RLong( Index.MaxWidth ) ;
	
	RLongArray( Index.Selection, RFN::SELECTION_COUNT ) ;
}

void ClRFN_RFN_R::ReadSoftFont(m4int32_t ai_Offset) 
{
	m4int32_t PosAux ;
	m4int16_t B ;

	if (ai_Offset!=-1) {
	
		PosAux = m_oFile.GetPos() ;

		m_oFile.SetPos( ai_Offset  ) ;

		RStr(SoftFont.m_oFileName) ;
		RIntArray(SoftFont.m_iTrans, ENG_TRANS_ARRAY_SIZE) ; 
		RIntArray(SoftFont.m_iPSC_PFA_Widths, ENG_PSC_AFM_WIDTHS_ARRAY_SIZE) ; 
		RLong(SoftFont.m_FileSize );
		SoftFont.m_Offset = m_oFile.GetPos() ;

		m_oFile.SetPos( PosAux  ) ;

	} else {
		
		SoftFont.m_oFileName.StrCpy("") ;
		SoftFont.m_iTrans[ENG_TRANS_TYPE_POS]=ENG_SOFT_TRANS_TYPE_EMPTY ;
		for (B=0;B<ENG_PSC_AFM_WIDTHS_ARRAY_SIZE;++B){
			SoftFont.m_iPSC_PFA_Widths[B]=0 ;
		} ;
		SoftFont.m_FileSize=0 ;
		SoftFont.m_Offset=-1 ;
	} ;
}


m4bool_t ClRFN_RFN_R::LocateIndex( DvcUnit_t ai_DesignHeight ) 
{
	m4int16_t B ;
	m4bool_t SwOk=M4_FALSE ;

	for (B=0;B<Header.Variations; ++B) {
		ReadIndex(B) ;
		if ( Index.Height==ai_DesignHeight ) {
			SwOk=M4_TRUE ;
			break ;
		} ;
	} ;
	return SwOk ;
}


void ClRFN_RFN_R::ReadMetric(m4int32_t ai_Offset) 
{
	m_oFile.SetPos(ai_Offset) ;

	RInt( Metric.PCL_Height ) ;
	RInt( Metric.PCL_MaxWidth ) ;
	RInt( Metric.PCL_SelectionHeight ) ;
	RInt( Metric.PCL_TypeFace ) ;
	RStr( Metric.PCL_SymbolSet ) ;
	RLong( Metric.PCL_SoftFontOff ) ;
			
	RInt( Metric.PSC_Height ) ;
	RInt( Metric.PSC_MaxWidth ) ;
	RInt( Metric.PSC_SelectionHeight ) ;
	RStr( Metric.PSC_TypeFace ) ;
	RLong( Metric.PSC_SoftFontOff ) ;

	RInt( Metric.ESCP_Height ) ;
	RInt( Metric.ESCP_MaxWidth ) ;
	RInt( Metric.ESCP_SelectionHeight ) ;
	RChar( Metric.ESCP_Flags ) ;
	RInt( Metric.ESCP_TypeFace ) ;

	RStr( Metric.RVW_FaceName ) ;
	RInt( Metric.RVW_Height ) ;
	RInt( Metric.RVW_MaxWidth ) ;
	RInt( Metric.RVW_SelectionHeight ) ;
	RIntArray(Metric.RVW_iTrans, ENG_TRANS_ARRAY_SIZE) ; 

	if ( !Header.SwFixed ) {
		RLongArray( Metric.Metric[RFN::PCL],  RFN::METRIC_ARRAY_SIZE) ;
		RLongArray( Metric.Metric[RFN::PSC],  RFN::METRIC_ARRAY_SIZE) ;
		RLongArray( Metric.Metric[RFN::ESCP], RFN::METRIC_ARRAY_SIZE) ;
		RLongArray( Metric.Metric[RFN::RVW],  RFN::METRIC_ARRAY_SIZE) ;
	} ;
} 


m4bool_t ClRFN_RFN_R::List(m4char_t *ai_pcFileName, m4char_t *ai_pcSaveFile) 
{
	m4bool_t SwOk=M4_TRUE, SwBold, SwItalic, SwUnderline ;
	m4int16_t BIndex, B, B2 ;
	m4int32_t PosCurrMetric ;
	m4char_t StrAux[3000] ;
	ClMIT_Str Path, File ;
	
	ClRFN_RFN_R::End() ;

	if ( ai_pcSaveFile ) {
		m_poInsp->m_poTrace->SetGroupDetail(0,m_poInsp->m_poTrace->GetGroupCount(), ClMIT_Msg::IGNORE_ALL) ;
		m_poInsp->m_poTrace->SetGroupDetail(FONT_M_TGI_RFN_LIST, ClMIT_Msg::CRAZY) ;
		m_poInsp->m_poTrace->SetMove(ClMIT_Trace::MOVE_ON_SUCCESS,ai_pcSaveFile) ;
		m_poInsp->m_poMemProf->SetCheckPointAtEnd(M4_FALSE) ;
	} ;

	
	ClMIT_SysInfo::ExtractFromPath( ai_pcFileName, &Path, &File );

	*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, "Info of %0:s RFN file", ClMIT_Msg::ENTRY_FORMAT ) 
		<< File.InChar(0) << SEND_MSG ;

	//--------------------------------------->Lectura bloque Header

	SwOk=OpenFile(ai_pcFileName) ;
	if (!SwOk) goto exit ;

	*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
		"HEADER info TypeFace %0:s variations %1:s Fixed %2:s PCL %3:s PSC %4:s RVW %5:s ESCP %6:s FilePAR %7:s" ) 
		<< Header.Typeface.InChar(0) << Header.Variations << (m4int16_t)Header.SwFixed
		<< RFN::IsPCL( Header.DvcFlag ) 
		<< RFN::IsPSC( Header.DvcFlag )
		<< RFN::IsRVW( Header.DvcFlag ) 
		<< RFN::IsESCP( Header.DvcFlag ) 
		<< Header.FilePAR.InChar(0)
		<< SEND_MSG ;
	
	//--------------------------------------->Ahora a recorrer bloques indice

	for ( BIndex=0 ; BIndex < Header.Variations ; ++BIndex)
	{	
		//--------------------------------------->Lectura Index
		
		ReadIndex(BIndex) ;

		*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
			"HEIGHT %0:s DvcUnit (%1:s Points) MaxWidth %2:s DvcUnit (%3:s CPI)" , ClMIT_Msg::ENTRY_FORMAT ) 
			<< Index.Height   << ClMetric::DvcToPoint(Index.Height) 
			<< Index.MaxWidth << ClMetric::DvcToCPIWidthChar(Index.MaxWidth)
			<< SEND_MSG ;


		//--------------------------------------->Agrupacion selections

		for (B=0;B<RFN::SELECTION_COUNT; ++B) {
			RFN::UnpackSIndex( B, &SwBold, &SwItalic, &SwUnderline ) ;
			Group[B].Offset=Index.Selection[B] ;
			Group[B].TraceText.StrCpy( SwBold ? " <    BOLD> " : " <Not BOLD> "  ) ;
			Group[B].TraceText.StrCat( SwItalic ? "<    ITALIC> " : " <Not ITALIC> "  ) ;
			Group[B].TraceText.StrCat( SwUnderline ? "<    Underline> " : "<NOT Underline> " ) ;
			if ( Group[B].Offset==-1 ) {
				Group[B].Group = -1 ;
			} else {
				Group[B].Group = B ;
				for (B2=0;B2<B;++B2) {
					if ( Group[B].Offset==Group[B2].Offset) {
						Group[B].Group = B2 ;
						break ;
					} ;
				} ;
			} ;

		} ;
		
		m_poInsp->m_poTrace->Indent() ;

		//--------------------------------------->Impresion selections iguales

		for (B=0;B<RFN::SELECTION_COUNT; ++B) {

			PosCurrMetric=0 ;
			for (B2=0;B2<RFN::SELECTION_COUNT; ++B2) {
				if ( Group[B2].Group == -1 ) continue ;
				if ( Group[B2].Group == B ) {
					*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, "%0:s" ) 
						<< Group[B2].TraceText.InChar(0) << SEND_MSG ;
					PosCurrMetric=Group[B2].Offset ;
				} ;
			} ;
			if (PosCurrMetric==0) continue ;

			//-------------------------------------->Lectura Metric Block

			ReadMetric(PosCurrMetric) ;

			m_poInsp->m_poTrace->Indent() ;

			if ( RFN::IsPCL( Header.DvcFlag ) ) {

				ReadSoftFont(Metric.PCL_SoftFontOff) ;

				*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
					"PCL  info: Height %0:s Points MaxWidth %1:s DvcUnit (%2:s CPI) "
					"TypeFace %3:s SymbolSet %4:s "
					"SoftFont %5:s TranslationType %6:s" ) 
					<< (Metric.PCL_Height/100.0) 
					<< Metric.PCL_MaxWidth
					<< (ClMetric::DvcToCPIWidthChar(Metric.PCL_MaxWidth))
					<< Metric.PCL_TypeFace
					<< Metric.PCL_SymbolSet.InChar(0)
					<< SoftFont.m_oFileName.InChar(0)
					<< (char)SoftFont.m_iTrans[ENG_TRANS_TYPE_POS]
					<< SEND_MSG ;
	
				if ( !Header.SwFixed ) {
					m_poInsp->m_poTrace->Indent() ;
					strcpy(StrAux,"") ;
					for (B2=0;B2<RFN::METRIC_ARRAY_SIZE;++B2) {
						sprintf( &StrAux[strlen(StrAux)], "%li ", Metric.Metric[RFN::PCL][B2] ) ;
					} ;
					ClMIT_Str::OverStringValid(StrAux,3000) ;

					*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST,	"%0:s" ) 
						<< StrAux << SEND_MSG ;
					m_poInsp->m_poTrace->Unindent() ;
				} ;

			} ;

			if ( RFN::IsPSC( Header.DvcFlag ) ) {

				ReadSoftFont(Metric.PSC_SoftFontOff) ;

				*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
					"PSC  info: Height %0:s Points MaxWidth %1:s DvcUnit (%2:s CPI) "
					"TypeFace %3:s "
					"SoftFont %4:s TranslationType %5:s" ) 
					<< (Metric.PSC_Height/100.0)
					<< Metric.PSC_MaxWidth 
					<< (ClMetric::DvcToCPIWidthChar(Metric.PSC_MaxWidth))
					<< (char*)Metric.PSC_TypeFace
					<< SoftFont.m_oFileName.InChar(0)
					<< (char)SoftFont.m_iTrans[ENG_TRANS_TYPE_POS]
					<< SEND_MSG ;
	
				if ( !Header.SwFixed ) {
					m_poInsp->m_poTrace->Indent() ;
					strcpy(StrAux,"") ;
					for (B2=0;B2<RFN::METRIC_ARRAY_SIZE;++B2) {
						sprintf( &StrAux[strlen(StrAux)], "%li ", Metric.Metric[RFN::PSC][B2] ) ;
					} ;
					ClMIT_Str::OverStringValid(StrAux,3000) ;

					*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST,	"%0:s" ) 
						<< StrAux << SEND_MSG ;
					m_poInsp->m_poTrace->Unindent() ;
				} ;

			} ;

			if ( RFN::IsESCP( Header.DvcFlag ) ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
					"ESCP info: Height %0:s Points MaxWidth %1:s DvcUnit (%2:s CPI) "
					"Flags %3:s TypeFace %4:s" ) 
					<< (Metric.ESCP_Height/100.0)
					<< Metric.ESCP_MaxWidth 
					<< (ClMetric::DvcToCPIWidthChar(Metric.ESCP_MaxWidth))
					<< (m4int16_t)Metric.ESCP_Flags
					<< Metric.ESCP_TypeFace
					<< SEND_MSG ;
				if ( !Header.SwFixed ) {
					strcpy(StrAux,"") ;
					m_poInsp->m_poTrace->Indent() ;
					for (B2=0;B2<RFN::METRIC_ARRAY_SIZE;++B2) {
						sprintf( &StrAux[strlen(StrAux)], "%li ", Metric.Metric[RFN::ESCP][B2] ) ;
					} ;
					ClMIT_Str::OverStringValid(StrAux,3000) ;

					*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST,	"%0:s" ) 
						<< StrAux << SEND_MSG ;
					m_poInsp->m_poTrace->Unindent() ;
				} ;

			} ;

			if ( RFN::IsRVW( Header.DvcFlag ) ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
					"RVW  info: Height %0:s Points MaxWidth %1:s DvcUnit (%2:s CPI) "
					"FaceName %3:s TranslationType %4:s" ) 
					<< (Metric.RVW_Height/100.0) 
					<< Metric.RVW_MaxWidth 
					<< ClMetric::DvcToCPIWidthChar(Metric.RVW_MaxWidth)
					<< (char*)Metric.RVW_FaceName
					<< (char)Metric.RVW_iTrans[ENG_TRANS_TYPE_POS]
					<< SEND_MSG ;
				if ( !Header.SwFixed ) {
					m_poInsp->m_poTrace->Indent() ;
					strcpy(StrAux,"") ;
					for (B2=0;B2<RFN::METRIC_ARRAY_SIZE;++B2) {
						sprintf( &StrAux[strlen(StrAux)], "%li ", Metric.Metric[RFN::RVW][B2] ) ;
					} ;
					ClMIT_Str::OverStringValid(StrAux,3000) ;

					*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST,	"%0:s" ) 
						<< StrAux << SEND_MSG ;
					m_poInsp->m_poTrace->Unindent() ;
				} ;
			} ;

			m_poInsp->m_poTrace->Unindent() ;

		} ;

		m_poInsp->m_poTrace->Unindent() ;
		

		*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, 
			"End variation block" , ClMIT_Msg::EXIT_FORMAT ) 
			<< SEND_MSG ;
	
	} ;



	//--------------------------------------->
exit:
	
	m_oFile.Close() ;

	*m_poInsp->m_poTrace << ClMIT_Msg(FONT_M_TGI_RFN_LIST, "Info of %0:s RFN file", ClMIT_Msg::EXIT_FORMAT ) 
		<< File.InChar(0) << SEND_MSG ;

	return SwOk ;
}


m4bool_t ClRFN_RFN_R::ReadFont( 	

		//Fichero que contiene la Font

		const m4char_t *ai_pcFileName,

		//Datos seleccionadores de la font

		Point_t ai_MapHeight,
		m4bool_t ai_bSwFixed, m4bool_t ai_bBold, m4bool_t ai_bItalic, m4bool_t ai_bUnderline, 

		//Datos leidos del fichero
		ClMIT_Str *ao_poNameFont,
		DvcUnit_t *ao_DesignHeight, DvcUnit_t *ao_DesignMaxWidth, 
		DvcUnit_t *ao_Height, DvcUnit_t *ao_MaxWidth, 
		DvcUnit_t  ao_piMetric[ClFONT_M_Font::SIZE_WIDTH_ARRAY],

		ClFONT_M_Font::LOAD_INFO ai_Device, 
		m4bool_t ai_bImplemented[ClFONT_M_Font::LOAD_INFO_COUNT], 

		ClFONT_M_Font::StPCL *ao_pstPCL, ClFONT_M_Font::StPSC *ao_pstPSC, 
		ClFONT_M_Font::StESC *ao_pstESC, ClFONT_M_Font::StRVW *ao_pstRVW, 
		ClMIT_Str *ao_poSofFontFile, m4int32_t * ao_SoftFontOffset, 
		m4int32_t *ao_SoftFontSize,
		m4int16_t ao_piTrans[ENG_TRANS_ARRAY_SIZE],
		m4int16_t ao_piPSC_AFM_Widths[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE],

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t SwOk=M4_TRUE, Sw2Y, Sw2X, SwCompr ;
	m4int16_t SelectIndex, B ;
	CPI_t CPIAux ;


	ClRFN_RFN_R::End() ;


	//--------------------------------------->Lectura bloque Header

	SwOk=OpenFile((m4char_t *)ai_pcFileName) ;
	if (!SwOk) goto exit ;

	if ( (Header.SwFixed == 0 && ai_bSwFixed) || (Header.SwFixed != 0 && !ai_bSwFixed) ) {	
		//Si datos Fixed no coinciden, error
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	for (B=0; B< ClFONT_M_Font::LOAD_INFO_COUNT; ++B) ai_bImplemented[B]=M4_FALSE ;

	ai_bImplemented[ClFONT_M_Font::PCL_INFO]	= RFN::IsPCL(Header.DvcFlag) ;
	ai_bImplemented[ClFONT_M_Font::PSC_INFO]	= RFN::IsPSC(Header.DvcFlag) ;
	ai_bImplemented[ClFONT_M_Font::ESCP_INFO]	= RFN::IsESCP(Header.DvcFlag) ;
	ai_bImplemented[ClFONT_M_Font::RVW_INFO]	= RFN::IsRVW(Header.DvcFlag) ;
	
	//Si las metricas para el Dvc no existen, error
	switch (ai_Device) {
	case ClFONT_M_Font::PCL_INFO:
		if ( ! RFN::IsPCL(Header.DvcFlag)  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		}
		break;

	case ClFONT_M_Font::PSC_INFO:
		if ( ! RFN::IsPSC(Header.DvcFlag)  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		}
		break;

	case ClFONT_M_Font::ESCP_INFO:
		if ( ! RFN::IsESCP(Header.DvcFlag)  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		}
		break;

	case ClFONT_M_Font::RVW_INFO:
		if ( ! RFN::IsRVW(Header.DvcFlag)  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		}
		break;

	default :
			
		SwOk=M4_FALSE ;
		goto exit ;

	} ;
	

	//--------------------------------------->Localización Indice y combinacion

	if ( ! LocateIndex( ClMetric::PointToDvc(ai_MapHeight) ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	SelectIndex = RFN::PackSIndex(ai_bBold,ai_bItalic,ai_bUnderline)  ;
	if ( Index.Selection[SelectIndex]==-1 ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	ReadMetric(Index.Selection[SelectIndex]) ;

	*ao_DesignHeight   = Index.Height ;
	*ao_DesignMaxWidth  = Index.MaxWidth ;

	//--------------------------------------->Lectura de datos 

	ao_piTrans[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_EMPTY ;

	*ao_SoftFontSize=0 ;	//Por defecto, no es SoftFont

	switch (ai_Device) {

	case ClFONT_M_Font::PCL_INFO:

		ao_pstPCL->Height		= Metric.PCL_SelectionHeight / 100.0;
		*ao_MaxWidth			= Metric.PCL_MaxWidth;
		ao_pstPCL->iTypeFace	= Metric.PCL_TypeFace ;
		ao_pstPCL->m_pcSymbolSet.StrCpy(Metric.PCL_SymbolSet) ;
		*ao_Height				= ClMetric::PointToDvc(Metric.PCL_Height / 100.0) ;
		ao_pstPCL->Pitch		= ClMetric::DvcToCPIWidthChar( Metric.PCL_MaxWidth )  ;
		if ( ! ai_bSwFixed ) {
			for (B=0; B<ClFONT_M_Font::SIZE_WIDTH_ARRAY; B++) {
				ao_piMetric[B]=Metric.Metric[RFN::PCL][B] ;
			} ;
		} ;

		if ( Metric.PCL_SoftFontOff != -1 ) {
			ReadSoftFont(Metric.PCL_SoftFontOff) ;
			*ao_SoftFontOffset  = SoftFont.m_Offset ;
			*ao_SoftFontSize = SoftFont.m_FileSize ;
			ao_poSofFontFile->StrCpy(SoftFont.m_oFileName) ;
			for (B=0; B<ENG_TRANS_ARRAY_SIZE; B++) {
				ao_piTrans[B]=SoftFont.m_iTrans[B] ;
			} ;
		} ;
		
		break;

	case ClFONT_M_Font::PSC_INFO:
		ao_pstPSC->Height		= Metric.PSC_SelectionHeight / 100.0;
		*ao_MaxWidth			= Metric.PSC_MaxWidth;
		*ao_Height				= ClMetric::PointToDvc(Metric.PSC_Height / 100.0) ;
		ao_pstPSC->Pitch		= ClMetric::DvcToCPIWidthChar( Metric.PSC_MaxWidth )  ;
		if ( ! ai_bSwFixed ) {
			for (B=0; B<ClFONT_M_Font::SIZE_WIDTH_ARRAY; B++) {
				ao_piMetric[B]=Metric.Metric[RFN::PSC][B] ;
			} ;
		} ;
		ao_pstPSC->pcTypeFace.StrCpy(Metric.PSC_TypeFace) ;	

		if ( Metric.PSC_SoftFontOff != -1 ) {
			ReadSoftFont(Metric.PSC_SoftFontOff) ;
			*ao_SoftFontOffset  = SoftFont.m_Offset ;
			*ao_SoftFontSize = SoftFont.m_FileSize ;
			ao_poSofFontFile->StrCpy(SoftFont.m_oFileName) ;
			for (B=0; B<ENG_TRANS_ARRAY_SIZE; B++) {
				ao_piTrans[B]=SoftFont.m_iTrans[B] ;
			} ;
			for (B=0; B<ENG_PSC_AFM_WIDTHS_ARRAY_SIZE; B++) {
				ao_piPSC_AFM_Widths[B]=SoftFont.m_iPSC_PFA_Widths[B] ;
			} ;
			
		} ;

		break;

	case ClFONT_M_Font::RVW_INFO:
		ao_pstRVW->m_Height		= Metric.RVW_SelectionHeight / 100.0;
		*ao_MaxWidth			= Metric.RVW_MaxWidth;
		*ao_Height				= ClMetric::PointToDvc(Metric.RVW_Height / 100.0) ;
		ao_pstRVW->m_oFaceName.StrCpy( Metric.RVW_FaceName ) ;
		ao_pstRVW->m_Pitch		= ClMetric::DvcToCPIWidthChar( Metric.RVW_MaxWidth )  ;
		if ( ! ai_bSwFixed ) {
			for (B=0; B<ClFONT_M_Font::SIZE_WIDTH_ARRAY; B++) {
				ao_piMetric[B]=Metric.Metric[RFN::RVW][B] ;
			} ;
		} ;
		for (B=0; B<ENG_TRANS_ARRAY_SIZE; B++) {
			ao_piTrans[B]=Metric.RVW_iTrans[B] ;
		} ;


		break;

		
	case ClFONT_M_Font::ESCP_INFO:

		ao_pstESC->Height		= Metric.ESCP_SelectionHeight / 100.0;
		*ao_MaxWidth			= Metric.ESCP_MaxWidth ;
		ao_pstESC->iFont		= Metric.ESCP_TypeFace ;
		*ao_Height				= ClMetric::PointToDvc(Metric.ESCP_Height / 100.0) ;

		CPIAux  =  ClMetric::DvcToCPIWidthChar( *ao_MaxWidth )  ;
		if ( CPIAux <=11.00 ) {
			ao_pstESC->Pitch =  ClFONT_M_Font::ESC_PITCH_10 ;
		} else {
			ao_pstESC->Pitch = ClFONT_M_Font::ESC_PITCH_12 ;
		} ;

		RFN::UnpackESCP_Flags( Metric.ESCP_Flags, &Sw2Y, &Sw2X, &SwCompr ) ;
		ao_pstESC->oFlag.Reset( 0 | 
			(Sw2Y ? ClFONT_M_Font::StESC::DOUBLEY : 0 ) |
			(Sw2X ? ClFONT_M_Font::StESC::DOUBLEX : 0 ) |
			(SwCompr ? ClFONT_M_Font::StESC::COMPR : 0 ) 
		); 
		if (Sw2Y) *ao_Height *= 2 ;
		if (Sw2X) *ao_MaxWidth *= 2 ;
		if (SwCompr) *ao_MaxWidth *= RFN::ESC_FACTOR_COMPR ;

		if ( ! ai_bSwFixed ) {
			for (B=0; B<ClFONT_M_Font::SIZE_WIDTH_ARRAY; B++) {
				ao_piMetric[B]=Metric.Metric[RFN::ESCP][B] ;
				if (Sw2X) ao_piMetric[B] *= 2 ;
				if ( SwCompr ) ao_piMetric[B] *= RFN::ESC_FACTOR_COMPR ;
			} ;
		} ;
		
		break;


	default :
			
		SwOk=M4_FALSE ;
		goto exit ;

	} ;

	//Las propiedades RVW Siempre se cargan si hay datos
	if (RFN::IsRVW(Header.DvcFlag) ) {
		ao_pstRVW->m_Height		= Metric.RVW_SelectionHeight / 100.0;
		ao_pstRVW->m_oFaceName.StrCpy( Metric.RVW_FaceName ) ;
		ao_pstRVW->m_Pitch		= ClMetric::DvcToCPIWidthChar( Metric.RVW_MaxWidth )  ;
	} ;

exit:
	
	m_oFile.Close() ;

	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ READ_RFN_MSG_40 ] ) 
			<< ai_pcFileName << ai_MapHeight << ai_bBold << ai_bItalic << ai_bUnderline
			<< SEND_MSG ;
	} ;

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk ;


}




