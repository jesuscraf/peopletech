//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             RFNfileR.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __RFNFILER__H__
#define __RFNFILER__H__

#include "clstr.h"
#include "clmetric.h"
#include "font.h"
#include "rfnstrt.h"
#include "softtrans.h"


//==============================================================================ClRFN_RFN_R

class M4_DECL_M4FMETRICS ClRFN_RFN_R
{

	//Interpreta ficheros RFN. Todas las operaciones abren y cierran el fichero, 
	//	no lo mantienen abierto entre llamadas.

MIT_PRIVATE:

	StMIT_Inspector  *m_poInsp ;	//Simple link

	// Adaptador para archivos binarios

	ClMIT_BinaryAdapter m_BAdapter ;

	ClMIT_File m_oFile ;

	struct StAuxGroup {
		m4int16_t Group ; //-1, no imprime, 0-9 max, grupos por orden.
		ClMIT_Str TraceText ;
		m4int32_t Offset ;
	} Group[ClFONT_M_RFNStruct::SELECTION_COUNT] ;	

	struct StHeader {
		m4uint8_t FileStorage ;
		ClMIT_Str FilePAR ;
		ClMIT_Str IdFormat ;
		m4int16_t Version ;
		ClMIT_Str Typeface ;
		m4int16_t Variations ;
		m4uint8_t SwFixed ;
		m4uint8_t DvcFlag ;
	} Header ; 

	struct StIndex {
		m4int32_t Height ;
		m4int32_t MaxWidth ;
		m4int32_t Selection[ClFONT_M_RFNStruct::SELECTION_COUNT] ;
	} Index ;

	struct StMetric {

		m4int16_t PCL_Height ;
		m4int16_t PCL_MaxWidth ;
		m4int16_t PCL_SelectionHeight ;

		m4int16_t PCL_TypeFace ;
		ClMIT_Str PCL_SymbolSet ;	
		m4int32_t PCL_SoftFontOff ;

		m4int16_t PSC_Height ;
		m4int16_t PSC_MaxWidth ;
		m4int16_t PSC_SelectionHeight ;
		ClMIT_Str PSC_TypeFace ;
		m4int32_t PSC_SoftFontOff ;

		m4int16_t ESCP_Height ;
		m4int16_t ESCP_MaxWidth ;
		m4int16_t ESCP_SelectionHeight ;
		m4uint8_t ESCP_Flags ;
		m4int16_t ESCP_TypeFace ;

		ClMIT_Str RVW_FaceName ;	
		m4int16_t RVW_Height ;
		m4int16_t RVW_MaxWidth ;
		m4int16_t RVW_SelectionHeight ;
		m4int16_t RVW_iTrans[ENG_TRANS_ARRAY_SIZE] ;

		m4int32_t Metric[ClFONT_M_RFNStruct::DVC_COUNT]
						[ClFONT_M_RFNStruct::METRIC_ARRAY_SIZE] ;
	
	}  Metric ;

	struct StSoftFont {
		ClMIT_Str m_oFileName ;
		m4int16_t m_iTrans[ENG_TRANS_ARRAY_SIZE] ;
		m4int16_t m_iPSC_PFA_Widths[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE] ;
		m4int32_t m_FileSize, m_Offset ;
	}  SoftFont ;


MIT_PUBLIC:

	ClRFN_RFN_R() ;  

	virtual ~ClRFN_RFN_R() { ClRFN_RFN_R::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClRFN_RFN_R) ; }

	virtual void End() ;

	//Da false si tiene problemas. Saca a traza información sobre el fichero RFN
	//Si se pasa ai_pcSaveFile, resetea los niveles de traza para que en el fichero salga SOLO 
	//	la informacion referente al RFN, y se hace un MoveFile al fichero indicado.

	m4bool_t List(m4char_t *ai_pcFileName, m4char_t *ai_pcSaveFile=NULL) ;


	//	Localiza una Font, la abre, busca la combinación pedida y rellena los datos referentes
	//al device solicitado.
	//  Aunque se pidan datos no RVW, si estos existen, se carga siempre ao_pstRVW
	//	Da false si tiene problemas.

	m4bool_t ReadFont( 	

		//Fichero que contiene la Font

		const m4char_t *ai_pcFileName,

		//Datos seleccionadores de la font

		Point_t ai_MapHeight,
		m4bool_t ai_bSwFixed, m4bool_t ai_bBold, m4bool_t ai_bItalic, m4bool_t ai_bUnderline, 

		//Datos leidos del fichero
		ClMIT_Str *ao_poNameFont,
		DvcUnit_t *ao_DesignHeight, DvcUnit_t *ao_DesignMaxWidth, 
		DvcUnit_t *ao_Height, DvcUnit_t *ao_MaxWidth, 
		DvcUnit_t  ao_piMetric[ClFONT_M_Font::SIZE_WIDTH_ARRAY],//	SOLO es usado si !ai_bSwFixed

		ClFONT_M_Font::LOAD_INFO ai_Device, 
		m4bool_t ai_bImplemented[ClFONT_M_Font::LOAD_INFO_COUNT], 


		ClFONT_M_Font::StPCL *ao_pstPCL, ClFONT_M_Font::StPSC *ao_pstPSC, 
		ClFONT_M_Font::StESC *ao_pstESC, ClFONT_M_Font::StRVW *ao_pstRVW, 
		
		//Info SoftFont. Si ao_SoftFontSize=0, no es SoftFont. Si no, la informacion esta
		//guardada en el mismo ai_pcFileName, en el Offset y bytes indicados.
		//ao_piTrans se guarda siempre. Si no hay datos, pone el tipo a TRANS_TYPE_EMPTY. 

		ClMIT_Str *ao_poSofFontFile,
		m4int32_t * ao_SoftFontOffset, m4int32_t *ao_SoftFontSize,
		m4int16_t ao_piTrans[ENG_TRANS_ARRAY_SIZE],
		m4int16_t ao_piPSC_AFM_Widths[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE],

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) ;

MIT_PRIVATE:

	//============================================================>Auxiliares lectura estructura

	// Pasa todos los test de integridad, carga el Header y actualiza el m_BAdpater

	m4bool_t OpenFile(m4char_t *ai_pcFileName) ;

	// Lee el bloque Index n. Pasar indice correcto (de 0 a Header.Variations-1).

	void ReadIndex(m4int16_t ai_Index) ;

	// Localiza y carga el Index correspondiente a la ai_DesignHeight solicitada.
	// Da false si no lo localiza (no saca mensajes). 

	m4bool_t LocateIndex( DvcUnit_t ai_DesignHeight ) ;
	

	// Lee bloque Metric. Pasar Offset correcto (leido de Index Block, y no es -1)
	// Solo lee array de metricas si Font es de paso variable

	void ReadMetric(m4int32_t ai_Offset) ;

	// Lee bloque SoftFont. NO lee la informacion del fichero, pero si almacena los offsets.
	// Salva y recupera la posicion actual.
	//Si ai_Offset es -1, no lee y reseta todos los valores

	void ReadSoftFont(m4int32_t ai_Offset) ;

	//============================================================>Auxiliares lectura fichero

	void RInt(m4int16_t &ai_Int) ;
	void RLong(m4int32_t &ai_Long) ;
	void RChar(m4uint8_t &ai_Char) ;
	void RStr(ClMIT_Str &ai_Str) ; 
	void RLongArray( m4int32_t *ai_poLong, size_t ai_NItems) ; 
	void RIntArray(  m4int16_t *ai_poInt, size_t ai_NItems) ; 
} ;

#endif

