// RFNparser.y

/* 
*************************************************************************************************
*										DECLARATION SECTION										*
************************************************************************************************* 
*/

%{

#include "rfn.h"
#include "rfndf.cpp"
#include "flexlexer.h"	//Para acceder a yylex()
#include "rfntrans.h"
#include "rfnstrt.h"
#include "rfnparser.h"

%}

//---------------->Tokens basicos

%token RFN_LITERAL_INTEGER RFN_LITERAL_FLOAT RFN_LITERAL_STR RFN_LITERAL_BOOLEAN

//---------------->Keywords

%token RFN_NOT RFN_HEADER RFN_OUTPUT_NAME RFN_FONT_NAME RFN_FIXED RFN_PCL RFN_PSC RFN_RVW RFN_ESCP 
%token RFN_MAP_NAME RFN_DVC_FONT_ID RFN_DVC_CHARSET RFN_DVC_PITCH RFN_DVC_2Y RFN_DVC_2X 
%token RFN_DVC_HEIGHT RFN_SELECTION_HEIGHT
%token RFN_DVC_SOFT_FONT RFN_DVC_SOFT_FONT_TRANS
%token RFN_DVC_COMPR RFN_HEIGHT RFN_MAP_HEIGHT RFN_ALL RFN_BOLD_ON RFN_BOLD_OFF
%token RFN_BOLD_ALL RFN_ITALIC_ON RFN_ITALIC_OFF RFN_ITALIC_ALL RFN_UNDERLINE_ON RFN_UNDERLINE_OFF 
%token RFN_UNDERLINE_ALL RFN_DESIGN_HEIGHT RFN_DESIGN_WIDTH RFN_FOR_ALL

//---------------->Operadores especiales

%token RFN_OPEN_BLOCK RFN_CLOSE_BLOCK RFN_OPE_LET

%start program


%%

/* 
*************************************************************************************************
*										RULES SECTION											*
************************************************************************************************* 
	
	NOTA: en mayusculas estan los simbolos terminales, y ademas tienen tambien esta consideracion
los simbolos no terminales compuestos solo por simbolos terminales, cuya unica finalidad es 
agrupar tokens para hacer mas legible y menos extensa la gramatica.

*/

//=====================================================>Almacenar una variable en una lista


VBLE	: RFN_OUTPUT_NAME			{ PAR->m_oVbleName.StrCpy("OutputName") ; }
		| RFN_FONT_NAME				{ PAR->m_oVbleName.StrCpy("FontName") ; }
		| RFN_MAP_NAME				{ PAR->m_oVbleName.StrCpy("MapName") ; }
		| RFN_DVC_FONT_ID			{ PAR->m_oVbleName.StrCpy("DvcFontId") ; }
		| RFN_DVC_CHARSET			{ PAR->m_oVbleName.StrCpy("DvcCharSet") ; }
		| RFN_DVC_PITCH				{ PAR->m_oVbleName.StrCpy("DvcPitch") ; }
		| RFN_MAP_HEIGHT			{ PAR->m_oVbleName.StrCpy("MapHeight") ; }
		| RFN_DESIGN_HEIGHT			{ PAR->m_oVbleName.StrCpy("DesignHeight") ; }
		| RFN_DESIGN_WIDTH			{ PAR->m_oVbleName.StrCpy("DesignWidth") ; }
		| RFN_FIXED					{ PAR->m_oVbleName.StrCpy("Fixed") ; }
		| RFN_DVC_2Y				{ PAR->m_oVbleName.StrCpy("DvcDY") ; }
		| RFN_DVC_2X				{ PAR->m_oVbleName.StrCpy("DvcDX") ; }
		| RFN_DVC_COMPR				{ PAR->m_oVbleName.StrCpy("DvcCompr") ; }
		| RFN_DVC_HEIGHT			{ PAR->m_oVbleName.StrCpy("DvcHeight") ; }
		| RFN_SELECTION_HEIGHT		{ PAR->m_oVbleName.StrCpy("SelectionHeight") ; }
		| RFN_DVC_SOFT_FONT			{ PAR->m_oVbleName.StrCpy("DvcSoftFont") ; }
		| RFN_DVC_SOFT_FONT_TRANS	{ PAR->m_oVbleName.StrCpy("DvcSoftFontTrans") ; }
		;
				
let	: VBLE RFN_OPE_LET RFN_LITERAL_INTEGER
		{
			//Asignar INTEGER

			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::NUM_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		}
	| VBLE RFN_OPE_LET RFN_LITERAL_FLOAT
		{
			//Asignar FLOAT
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::NUM_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			};
		}
	| VBLE RFN_OPE_LET RFN_LITERAL_STR
		{
			//Asignar STRING
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::STR_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		}
	| VBLE RFN_OPE_LET RFN_LITERAL_BOOLEAN
		{
			//Asignar BOOLEAN
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::BOOL_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		}
	;

let_nelist	: let
			| let_nelist let

let_list	: let_nelist
			| /*nothing*/
			;

//=====================================================>Almacenar un bloque selector 

SELECTOR_ITEM_BOLD	: RFN_BOLD_ON	{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ON ; }
					| RFN_BOLD_OFF	{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_OFF ; }
					| RFN_BOLD_ALL	{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ALL ; }
					;
SELECTOR_ITEM_ITALIC	: RFN_ITALIC_ON		{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ON ; }
						| RFN_ITALIC_OFF	{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_OFF ; }
						| RFN_ITALIC_ALL	{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ALL ; }
						;
SELECTOR_ITEM_UNDERLINE	: RFN_UNDERLINE_ON	{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ON ; }
						| RFN_UNDERLINE_OFF	{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_OFF ; }
						| RFN_UNDERLINE_ALL	{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ALL ; }
						;

MODE_SELECTOR	: SELECTOR_ITEM_BOLD		SELECTOR_ITEM_ITALIC    SELECTOR_ITEM_UNDERLINE
				| SELECTOR_ITEM_BOLD		SELECTOR_ITEM_UNDERLINE SELECTOR_ITEM_ITALIC
				| SELECTOR_ITEM_ITALIC		SELECTOR_ITEM_BOLD		SELECTOR_ITEM_UNDERLINE
				| SELECTOR_ITEM_ITALIC		SELECTOR_ITEM_UNDERLINE	SELECTOR_ITEM_BOLD
				| SELECTOR_ITEM_UNDERLINE	SELECTOR_ITEM_BOLD		SELECTOR_ITEM_ITALIC
				| SELECTOR_ITEM_UNDERLINE   SELECTOR_ITEM_ITALIC	SELECTOR_ITEM_BOLD
				| RFN_ALL	
					{ 
						PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ALL ; 
						PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ALL ;
						PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ALL ;
					}
				;

selector_block	: MODE_SELECTOR  RFN_OPEN_BLOCK let_list  RFN_CLOSE_BLOCK 
					{
						if ( ! PAR->ReduceSelection( PAR->m_BoldSelect, PAR->m_ItalicSelect, PAR->m_UnderlineSelect ) )
						{
							YYERROR ;
						} ;
					} 
					;
				

//=====================================================>Almacenar n bloques selector consecutivos


selector_nelist	: selector_block
				| selector_nelist selector_block
				;

selector_list	: selector_nelist
				| /*nothing*/
				;

//=====================================================>Almacenar 1 bloque Dvc (calificado o no)


DVC_TYPE	: RFN_PCL	{ PAR->m_oBlockType=ClFONT_M_RFNStruct::PCL; }
			| RFN_PSC	{ PAR->m_oBlockType=ClFONT_M_RFNStruct::PSC; }
			| RFN_RVW	{ PAR->m_oBlockType=ClFONT_M_RFNStruct::RVW; }
			| RFN_ESCP	{ PAR->m_oBlockType=ClFONT_M_RFNStruct::ESCP; }
			;

DvcBlock	: DVC_TYPE  RFN_OPEN_BLOCK selector_list RFN_CLOSE_BLOCK
				{
					if ( ! PAR->ReduceForDvcBlock() ) {
						YYERROR ;
					}  ;
				}
			;


DvcQualifiedBlock	: DvcBlock
					| RFN_NOT DvcBlock
						{
							PAR->ClearDvcActivatedFlag() ;
						}
					;


//=====================================================>Almacenar n bloques Dvc consecutivos


DvcBlock_nelist	: DvcBlock
				| DvcBlock_nelist DvcBlock
				;

DvcBlock_list	: DvcBlock_nelist
				| /*nothing*/
				;


DvcQualifiedBlock_nelist	: DvcQualifiedBlock
							| DvcQualifiedBlock_nelist DvcQualifiedBlock
							;


DvcQualifiedBlock_list	: DvcQualifiedBlock_nelist
						| /*nothing*/
						;

//=====================================================>Almacenar bloque FOR_ALL

ForAllBlock	: RFN_FOR_ALL RFN_OPEN_BLOCK let_list RFN_CLOSE_BLOCK
				{
					if ( ! PAR->ReduceForAllBlock() ) {
						YYERROR ;
					}  ;
				}
			;

//=====================================================>Almacenar bloque HEADER

Header	: RFN_HEADER  RFN_OPEN_BLOCK ForAllBlock DvcQualifiedBlock_list  RFN_CLOSE_BLOCK
			{
				if ( ! PAR->ReduceHeaderBlock() ) {
					YYERROR ;
				}  ;
			} 
		;

//=====================================================>Almacenar bloque Height

HeightBlock	: RFN_HEIGHT  RFN_OPEN_BLOCK ForAllBlock DvcBlock_list  RFN_CLOSE_BLOCK
				{
					if ( ! PAR->ReduceHeightBlock() ) {
						YYERROR ;
					}  ;
				} 
			;

//=====================================================>Almacenar lista de bloques Height

Height_nelist	: HeightBlock
				| Height_nelist HeightBlock
				;

//=====================================================> INPUT de la gramatica


program : Header Height_nelist				
		;

%%

/* 
*************************************************************************************************
*										PROGRAM SECTION											*
************************************************************************************************* 
*/

//#include "parser.h"
