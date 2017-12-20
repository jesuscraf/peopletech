#ifndef __RFNSTR__H__
#define __RFNSTR__H__

#include "m4types.hpp"

#include "m4clflag.h"


class M4_DECL_M4FMETRICS ClFONT_M_RFNStruct
{
	// Esta clase simplemente contine enums de utilidad general respecto a los ficheros RFN

MIT_PUBLIC:

	// Vale 0.6, constante del factor de compresión de ESCP
	static const m4double_t ESC_FACTOR_COMPR; 

	// "Report Viewer Fonts Meta4", cadena identificadora
	static const m4char_t *RFN_FILE_ID ;

	enum OTHERS {
		SELECTION_COUNT=8,		//Numero de combinaciones bold/italic/underline posibles
		CURRENT_VERSION=8,
		METRIC_ARRAY_SIZE=256
	} ;

	enum DVC { PCL, PSC, RVW, ESCP, DVC_COUNT } ;

	enum STR_LENGHTS {	//Lenghts de cadenas de la estructura
		HEADER_FILE_PAR=256,
		HEADER_FILE_ID=25,
		HEADER_TYPE_FACE=40,
		PCL_SYMBOL_SET=8,
		RVW_FACE_NAME=60,
		PSC_FONT_ID=40,
		SOFT_FONT_NAME=40
	} ;

	enum HEADER_OFFSETS {	//Ofsets del header utiles
		FILE_STORAGE_OFF=0,
		FILE_ID_OFF=261,
		VERSION_OFF=290
	} ;

	enum BLOCK_SIZES {	//Tamaños de bloque utiles
		HEADER_SIZE=340,
		INDEX_SIZE=40
	} ;

MIT_PRIVATE:
		
	enum INDEX_FLAGS {
		UNDERLINE_FLAG	=ClMIT_Flag::BIT0,
		ITALIC_FLAG		=ClMIT_Flag::BIT1,
		BOLD_FLAG		=ClMIT_Flag::BIT2
	} ;

	enum DVC_FLAG	//Flag que indica si la metrica se permite o no para el Dvc
					//Hacer directamente un & con el valor leido para preguntar por el
	{
		PCL_FLAG = ClMIT_Flag::BIT0,
		PSC_FLAG = ClMIT_Flag::BIT1,
		RVW_FLAG = ClMIT_Flag::BIT2 ,
		ESCP_FLAG = ClMIT_Flag::BIT3
	} ;

	enum ESCP_FLAGS {
		ESCP_2Y_FLAG = ClMIT_Flag::BIT0,
		ESCP_2X_FLAG = ClMIT_Flag::BIT1,
		ESCP_COMPR_FLAG = ClMIT_Flag::BIT2
	} ;

MIT_PUBLIC:

	static m4int16_t PackSIndex( m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline ) ;
	static void UnpackSIndex( m4int16_t ai_Index, m4bool_t *ai_bSwBold, m4bool_t *ai_bSwItalic, m4bool_t *ai_bSwUnderline ) ;

	static m4int16_t PackDvcFlags( 
		m4bool_t ai_bSwPCL, m4bool_t ai_bSwPSC, 
		m4bool_t ai_bSwRVW, m4bool_t ai_bSwESCP ) ;

	static m4bool_t IsPCL(m4int16_t ai_DvcFlags) { return M4_BOOL( ai_DvcFlags & PCL_FLAG ); }
	static m4bool_t IsPSC(m4int16_t ai_DvcFlags) { return M4_BOOL( ai_DvcFlags & PSC_FLAG ); }
	static m4bool_t IsRVW(m4int16_t ai_DvcFlags) { return M4_BOOL( ai_DvcFlags & RVW_FLAG ); }
	static m4bool_t IsESCP(m4int16_t ai_DvcFlags) { return M4_BOOL( ai_DvcFlags & ESCP_FLAG ); }

	static m4int16_t PackESCP_Flags( m4bool_t ai_bSw2Y, m4bool_t ai_bSw2X, m4bool_t ai_bSwCompr ) ;
	static void UnpackESCP_Flags( m4int16_t ai_Value, m4bool_t *ai_bSw2Y, m4bool_t *ai_bSw2X, m4bool_t *ai_bSwCompr ) ;

	static m4char_t *GetDvcTraceName(m4int16_t ai_Dvc) ;

} ;   

#endif
