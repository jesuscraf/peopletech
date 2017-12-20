//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             field.cpp
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

#include <math.h>

#include "m4string.hpp"
#include "eng.h"
#include "engdf.inl"

#include "xengval.h"

#include "field.hmg"
#include "field.h"


#include "m4unicode.hpp"

//==============================================================================

#define MULTI_LINE_WORD_SEPARATOR " \t"
#define MULTI_LINE_SPACE " "
#define MULTI_LINE_SPACE_CHAR ' '

#define MULTILINE_TAG_OPEN '['
#define MULTILINE_TAG_CLOSE ']'
#define MULTILINE_CRLF_TAG "[CRLF]"
#define MULTILINE_CRLF_TAG_LEN 6

#define MULTILINE_CRLF "\r\n"
#define MULTILINE_LF "\n"
#define MULTILINE_CR_CHAR '\r'
#define MULTILINE_CRLF_LEN 2
#define MULTILINE_LF_LEN 1


//=======================================================================StENG_FieldEnv

void StENG_FieldEnv::End() 
{
	m_poFieldsManager=NULL ;
	m_poDvc=NULL ;		
	m_poDBFormat=NULL ;
}

//=======================================================================ClENG_FieldExternProp

void ClENG_FieldExternProp::Init() 
{
	m_oFlag.Off(RESET_DUPLICATE) ;
}

//=================================================================================ClENG_Field


ClENG_Field::ClENG_Field() 
{
	m_poInsp=GET_INSP();
	m_poFieldEnv=NULL ;
	m_State=NOT_INITIALIZED ;
}

void ClENG_Field::Init(	
	StENG_FieldEnv * ai_poFieldEnv,
	ClMIT_IdNumDesc *ai_poLocation,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
	DvcUnit_t ai_Height,  const m4char_t *ai_pcExpr, 
	const m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
	m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
	m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
	m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
	m4bool_t ai_bSwVariableHeight, m4bool_t ai_bPartialPrint,
	m4char_t *ai_pcIdPI)
{
	m4bool_t bPIAdd;
	ClMIT_Str strAux ;

	ClENG_Field::End();

	m_poFieldEnv = ai_poFieldEnv ;
	m_oDesc.StrCpy(ai_poLocation->GetDesc()) ;
	m_oLocation.Init( ai_poLocation->GetIdNum(), m_oDesc ) ;

	if (!ai_bSwStatic) ai_bSwOptimize=M4_FALSE;

	m_Top=ai_Top; m_Left=ai_Left; m_Width=ai_Width; m_Height=m_HeightBackup=ai_Height;
	m_oFlag.Switch( STATIC, ai_bSwStatic );
	m_oFlag.Switch( OPTIMIZE, ai_bSwOptimize );
	m_oFlag.Switch( DUPLICATED, ai_bSwDuplicated );
	m_oFlag.Switch( VARIABLE_HEIGHT, ai_bSwVariableHeight ) ;
	m_oFlag.Switch( PARTIAL_PRINT, ai_bPartialPrint  ) ;

	m_oFlag.On(DUPLICATE_RESET) ;
	
	m_poExternProp = NULL ;
	SetSave(NULL) ;

	strAux.StrCpy("Field ") ;
	strAux.StrCat(m_oDesc) ;
	m_oExpr.Init(ai_pcExpr,strAux) ;

	strAux.StrCpy("Index expr of field ") ;
	strAux.StrCat(m_oDesc) ;
	m_oIndexExpr.Init(ai_pcIndexExpr,strAux);
	m_iIndexLevel = ai_iIndexLevel;

	strAux.StrCpy("Hyperlink expr of field ") ;
	strAux.StrCat(m_oDesc) ;
	m_oHyperlink.Init(ai_pcHyperlink,strAux);
	m_oHyperlinkStr.StrCpy("") ;

	m_oIdPI.StrCpy("");

	if (ai_pcIdPI) {
		if (ClMIT_Str::StrLenTrim(ai_pcIdPI)>0) {
			m_oIdPI.StrCpy(ai_pcIdPI) ;
			m_oFlag.On( HAS_PI)  ;
		} else m_oFlag.Off( HAS_PI ) ;
	} else m_oFlag.Off( HAS_PI ) ;

	//Y 0 para controlar la altura con YField
	m_oRegion.Init( m_poFieldEnv->m_poDvc, /*m_Top*/0, m_Left, m_Width, m_Height );
	m_oRegion.SetFrame(ai_pcFrame) ;
	m_oRegion.SetFillColour(ai_FrameFillColour) ;

	m_State=INITIALIZED ;

}


void ClENG_Field::ResetExternProp( ClENG_FieldExternProp * ai_poExternProp ) 
{
	m_poExternProp = ai_poExternProp ;
}

void ClENG_Field::End() 
{
	m_oLocation.Init(0,"?") ;
	m_oRegion.End() ;
	m_State=NOT_INITIALIZED ;
}

m4bool_t ClENG_Field::PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) 
{
	m4bool_t SwOk = M4_TRUE;
	ClENG_Analex Analex ; 
	ClMIT_Str StrAux ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_Field::PreAnalex() field %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< m_oDesc.InChar(0) << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ FIELD_MSG_30 ] ) 
		<< m_oLocation << END_LOC ;

	StrAux.StrCpy( m_oExpr.GetExpressionStr() ) ;
	ClENG_Analex::PrepareForAnalexField(&StrAux,ai_pstEnv) ;
	m_oExpr.ChangeExpression(StrAux) ;

	SwOk = Analex.Init(ai_pstEnv,&m_oExpr,ClMIT_Trace::PROPAGATE_ERROR) ;	
	ResetExternProp(ai_pstEnv->m_poFieldExternProp) ;

	StrAux.StrCpy( m_oIndexExpr.GetExpressionStr() ) ;
	ClENG_Analex::PrepareForAnalexField(&StrAux,ai_pstEnv) ;
	m_oIndexExpr.ChangeExpression(StrAux) ;

	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pstEnv,&m_oIndexExpr,ClMIT_Trace::PROPAGATE_ERROR) );	

	StrAux.StrCpy( m_oHyperlink.GetExpressionStr() ) ;
	ClENG_Analex::PrepareForAnalexField(&StrAux,ai_pstEnv) ;
	m_oHyperlink.ChangeExpression(StrAux) ;

	SwOk = M4_BOOL( SwOk && Analex.Init(ai_pstEnv,&m_oHyperlink,ClMIT_Trace::PROPAGATE_ERROR) );	

	if ( IsStatic() ) LoadValue() ;

	*m_poInsp->m_poTrace << POP_LOC ;

	m_State= PRINTED ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_Field::PreAnalex() field %0:s returning %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< m_oDesc.InChar(0) << SwOk << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}

void ClENG_Field::SetSave( ClMIT_ChnItem *ai_poSave ) 
{
	if (ai_poSave) {
		m_oSave.Init( ai_poSave ) ;
	} else m_oSave.End()  ;
}

m4bool_t ClENG_Field::IsDuplicatedControl() 
{
	return M4_BOOL( m_oFlag.True(DUPLICATED) && m_oFlag.False(DUPLICATE_RESET) && 
					( ! m_poExternProp ? M4_TRUE : ! m_poExternProp->IsResetDuplicate() ) 
				  );
}


void ClENG_Field::NewPrint()
{
	m_State=START_PRINTING ;
	m_HeightPrinted=0 ;
	if ( !IsStatic()  ) {  
		LoadValue() ;
	};
	NewPrintCycle() ;
}


void ClENG_Field::Print( 
	m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, DvcUnit_t ai_PrintInterval,
	DvcUnit_t ai_HeightCntPrinted) 
{
	m4bool_t SwShow, SwAllPrinted ;
	DvcUnit_t YField, FieldInterval, ToBePrinted ;

	//En estado PRINTED no hace nada

	if ( !( m_State==START_PRINTING || m_State==PARTIAL_PRINTED )  ) return ;

	//Si no puede imprimirse nada en primera impresion, se sale sin tocar el estado.

	if ( GetState()==START_PRINTING ) {
		//Primera impresión, el Top del field cuenta
		YField			= ai_YBase+m_Top-ai_HeightCntPrinted ;
		FieldInterval	= ai_PrintInterval- (m_Top-ai_HeightCntPrinted) ;	
		ToBePrinted		= M4_MIN(FieldInterval,m_Height) ;
	} else {
		//No es primera impresion, el Top no cuenta porque se imprime desde el principio
		YField			=  ai_YBase ;
		FieldInterval	= ai_PrintInterval ;	
		ToBePrinted		= M4_MIN(FieldInterval,m_Height-m_HeightPrinted) ;
	} ;
	if (FieldInterval<=0) return ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ FIELD_MSG_40 ] ) 
		<< m_oLocation << END_LOC ;

	//Si el field es PI se añade una nueva entrada a la lista con el m_iIdPI

	if ( m_State==START_PRINTING && IsPi()  )
	{
		if ( ! m_poFieldEnv->m_poFieldsManager->AddNewPi(&m_oIdPI,this, &m_oValue) ) {
			m_oFlag.Off( HAS_PI ) ;
		} ;
	};

	//Actualiza SwShow según control de duplicados

	if ( m_State==START_PRINTING ) { 
		if ( IsDuplicatedControl() ) {
			ClMIT_ChnValue R ;

			m_oValue.Evaluate( ClMIT_ChnValue::NOT_EQUAL, &m_oPrevValue, &R ) ;
			SwShow = R.GetBoolean() ;

		} else SwShow=M4_TRUE ;
	} else SwShow=M4_TRUE ;

	if (SwShow) {

		//Si hay control de duplicados, salva actual en previo para pasada posterior

		if ( IsDuplicated() ) m_oPrevValue.Init(&m_oValue) ;
			
		//Impresión Región. Actualiza m_HeightPrinted.

		m_oRegion.Show( ai_iNumPage, ai_XBase, YField, m_HeightPrinted, ToBePrinted ) ;
		m_HeightPrinted += ToBePrinted ;

		//FlushValue con coordenadas de device

		SwAllPrinted = FlushValue( 
			ai_iNumPage, ai_XBase+m_Left, YField, FieldInterval ) ;
		
		//Actualiza estado según lo impreso

		m_State = SwAllPrinted ? PRINTED : PARTIAL_PRINTED  ;
		
		//Desactiva flag 'no hay valor previo para comparar'

		m_oFlag.Off(DUPLICATE_RESET) ;


	} else {
	 
		m_State = PRINTED ;
	} ;

	//Salva valor

	if ( m_oSave.IsLinked() )  {
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped() && 
			m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL )
		{
			ClMIT_Str strAux ;
			m_oValue.GetTraceValue(&strAux) ;
			*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
				"Saving Item %0:s : %1:s, pos %2:s with value %3:s", 
				ClMIT_Msg::MIXED_OUT) 
				<< m_oSave.GetNodeIdDesc().GetId() 
				<< m_oSave.GetIdDesc().GetId()
				<< m_oSave.GetNode()->GetNumRecord()
				<< strAux.InChar(0)
				<< DETAILED_MSG << SEND_MSG ;
		} ;
		#endif
		m_oSave.m_oValue.Init(&m_oValue) ;
		m_oSave.Save() ;
	} ;

	*m_poInsp->m_poTrace << POP_LOC ;

}

void ClENG_Field::LoadValue(ClMIT_ChnValue *ai_poValue) 
{
	//Carga m_oValue o bien por ejecución de expresión o bien por copia de valor pasado.

	if (!ai_poValue) {
		m_oExpr.Evaluate(&m_oValue,ClMIT_Trace::PROPAGATE_ERROR) ;
	} else {
		m_oValue.Init(ai_poValue) ;
	} ;

	//Deposita en m_oIndexStr el valor de indice.

	if(m_iIndexLevel!=NO_INDEX_ENTRY){
		ClMIT_ChnValue indexValue ;

		m_oIndexExpr.Evaluate(&indexValue, ClMIT_Trace::PROPAGATE_ERROR);
		indexValue.GetInputFormat(&m_oIndexStr);
	};

	//Deposita en m_oHyperlinkStr el valor de link (si no hay, siempre esta a "" )

	if ( ! m_oHyperlink.IsEmpty() ) {
		ClMIT_ChnValue hyperValue;

		m_oHyperlink.Evaluate(&hyperValue, ClMIT_Trace::PROPAGATE_ERROR);
		hyperValue.GetInputFormat(&m_oHyperlinkStr);
	} ;

	if ( ! IsVariableHeight() ) {

		//Campo no admite altura variable. NO hay reajustes.

		PostLoadValue() ;
	
	} else {
		
		//Campo admite altura variable, se reajusta.

		m_Height = PostLoadValue() ;
		m_Height = M4_MAX( m_Height, m_HeightBackup ) ;
		if ( m_oRegion.GetHeight() != m_Height ) m_oRegion.SetHeight(m_Height) ;
	} ;
}

//==============================================================================ClENG_FieldStr

ClENG_FieldStr::ClENG_FieldStr() {
	m_poFont=NULL;
}

void ClENG_FieldStr::End() {
	m_poFont=NULL;
	ClENG_Field::End();
	m_oNULLDef.End() ;
	m_oNULLFormatDef.End() ;
}



void ClENG_FieldStr::Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,

		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,

		ALIGN ai_AlignX, ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, ClENG_Format *ai_poFormat, m4bool_t ai_bSwClip,
		FILL_MODE ai_FillMode, m4char_t ai_cFillChar,

		StMIT_NULLDisplayDef *ai_poNULLDef, StMIT_NULLDisplayDef * ai_poNULLFormatDef, 
		m4bool_t ai_bSwFormatNULL,
		
		m4char_t *ai_pcIdPi)
{
	ClENG_FieldStr::End() ;

	m_AlignX=ai_AlignX ; m_AlignY=ai_AlignY ; 
	m_poFormat=ai_poFormat ;
	m_poFont=ai_poFont ;
	m_FillMode = ai_FillMode ;
	m_cFillChar = ai_cFillChar ;
	m_uiIdFGColour = ai_uiIdFGColour ;
	if ( ai_Height<0 )  ai_Height=m_poFont->GetDvcDesignHeight() * (-ai_Height);
	if (ai_Width<0) ai_Width=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Width) ;
	if ( ai_Top<0 )  ai_Top=m_poFont->GetDvcDesignHeight() * (-ai_Top);
	if (ai_Left<0) ai_Left=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Left) ;
	m_oFlag.Switch(CLIP,ai_bSwClip);

	if (ai_poNULLDef) m_oNULLDef.Init(ai_poNULLDef) ; 
	if (ai_poNULLFormatDef) m_oNULLFormatDef.Init(ai_poNULLFormatDef) ; 
	m_oFlag.Switch( FORMAT_NULL,ai_bSwFormatNULL) ;
	
	m_OffsetX=m_OffsetY=0 ;

	ClENG_Field::Init(ai_poFieldEnv,ai_poLocation,ai_Top, ai_Left, ai_Width, ai_Height, ai_pcExpr, 
		ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize,
		ai_pcFrame, ai_FrameFillColour,
		M4_FALSE, M4_FALSE, ai_pcIdPi);
	
}


DvcUnit_t ClENG_FieldStr::PostLoadValue() 
{
	DvcUnit_t SobraDvc, DvcLen ;
	size_t Len;
	FORMAT_DATA_TYPE Type ;
	ClENG_Format *Format ;
	ClMIT_Str Aux;
	m4uint16_t iIndex;
	size_t FillLeft, FillRight ;
	size_t iChars = 0;
	size_t iBytes = 0;
	size_t iszNChar = 0;
	
	//--------------Obtencion de la cadena formateada, debo dejarla en m_oSourceStr

	Format=m_poFormat ;
	if( !Format && m_oValue.GetType()==ClMIT_ChnValue::CURRENCY && 
		(!m_oValue.IsNULL() || ( m_oValue.IsNULL() && IsFormatNULL() )  ) ) 
	{
		//Sera NULL si no hay formato
		Format= m_poFieldEnv->m_poDBFormat->GetCurDefaultFormat(
			m_oValue.GetCurrencyType(), M4_FALSE, ClMIT_Trace::PROPAGATE_ERROR);
	};

	if ( m_oValue.IsNULL() ) {

		if (!IsFormatNULL() || !Format ) { 
			m_oSourceStr.StrCpy(m_oNULLDef.m_oStr[m_oValue.GetType()]) ;
		} else {
			m_oValue.GetInputFormat( &m_oSourceStr, &Type ) ;	//Para obtener tipo
			Format->Apply(m_oNULLFormatDef.m_oStr[m_oValue.GetType()], 
				Type, &m_oSourceStr) ;
		} ;

	} else if(m_oValue.GetType()==ClMIT_ChnValue::NO_INIT){

		m_oSourceStr.StrCpy("") ;

	} else {
	
		if ( m_oValue.GetInputFormat( &m_oSourceStr, &Type ) ) {
			//Tipo de dato formateable
			if (Format) {
				Format->Apply(m_oSourceStr.InChar(0), Type, &m_oFormatedStr) ;
				m_oSourceStr.StrCpy( m_oFormatedStr.InChar(0) ) ;
			} ;
		} ; 
	} ;

	//---------Alinear eje X
		
	if(m_oValue.GetType()!=ClMIT_ChnValue::NUMERIC && m_AlignX==POINT) {
		m_AlignX=RIGHT;
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FIELD_MSG_50 ] ) 
			<< m_oLocation << SEND_MSG ;
	}
	Len=m_oSourceStr.StrLen();
	DvcLen=m_poFont->GetDvcLenStr(m_oSourceStr.InChar(0));
	SobraDvc=GetWidth()-DvcLen;
	if (SobraDvc>0) {
			
		//Si sobra espacio, muestra todo y alinea. 
		//Unico caso en el que es posible el relleno de caracter
		if ( GetFillMode()==NO_FILL || ( GetFillMode()==NO_EMPTY_FILL && 
			m_oSourceStr.StrLenTrim()==0 ) ) 
		{
			m_szIndStr=0 ; m_szNChar=Len ;
			if (m_AlignX==LEFT) {
				m_OffsetX=0;
			} else if (m_AlignX==CENTER) {
				m_OffsetX=SobraDvc/2;
			} else if (m_AlignX==RIGHT) {
				m_OffsetX=SobraDvc;
			} else if (m_AlignX==POINT) {
				if(strchr((m4char_t*)m_oSourceStr,'.')){
					Aux.StrCat(m_oSourceStr);
					Aux.Trim(strchr((m4char_t*)m_oSourceStr,'.')-m_oSourceStr.InChar(0));

					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(Aux.InChar(0)));
					Aux.Accommodate(0);
				}
				else if(strchr((m4char_t*)m_oSourceStr,',')){
					Aux.StrCat(m_oSourceStr);
					Aux.Trim(strchr((m4char_t*)m_oSourceStr,',')-m_oSourceStr.InChar(0));

					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(Aux.InChar(0)));
					Aux.Accommodate(0);
				}
				else{
					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(m_oSourceStr.InChar(0)));
				}
			};

		} else {

			m_szIndStr=0 ; 
			m_OffsetX=0;
			FillLeft=FillRight=0;
			if (m_AlignX==LEFT) {
				FillRight=SobraDvc/m_poFont->GetDvcWidthChar(GetFillChar()) ;
			} else if (m_AlignX==CENTER) {
				FillLeft = FillRight = (SobraDvc)/(2*m_poFont->GetDvcWidthChar(GetFillChar())) ;
				m_OffsetX = (SobraDvc-
					( 
					(FillLeft+FillRight)*m_poFont->GetDvcWidthChar(GetFillChar()) 
					))/2 ;	//Ajuste por caracter entero
			} else if (m_AlignX==RIGHT) {
				FillLeft=SobraDvc/m_poFont->GetDvcWidthChar(GetFillChar()) ;
				m_OffsetX = SobraDvc-
					( 
					(FillLeft+FillRight)*m_poFont->GetDvcWidthChar(GetFillChar()) 
					) ;	//Ajus<te por caracter entero
			} else if (m_AlignX==POINT) {
				if(strchr((m4char_t*)m_oSourceStr,'.')){
					Aux.StrCat(m_oSourceStr);
					Aux.Trim(strchr((m4char_t*)m_oSourceStr,'.')-m_oSourceStr.InChar(0));

					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(Aux.InChar(0)));
					Aux.Accommodate(0);
				}
				else if(strchr((m4char_t*)m_oSourceStr,',')){
					Aux.StrCat(m_oSourceStr);
					Aux.Trim(strchr((m4char_t*)m_oSourceStr,',')-m_oSourceStr.InChar(0));

					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(Aux.InChar(0)));
					Aux.Accommodate(0);
				}
				else{
					m_OffsetX=(GetWidth()/2)-(m_poFont->GetDvcLenStr(m_oSourceStr.InChar(0)));
				}
				
			} ;
			m_oSourceStr.StrInsertChar( 0, FillLeft, GetFillChar() ) ;
			m_oSourceStr.StrInsertChar( ClMIT_Str::STR_CAT, FillRight,  GetFillChar() ) ;
			m_szNChar=Len+FillRight+FillLeft ;
		} ;

	} else if (SobraDvc==0) {
	
		//Si el espacio es justo, muestra todo
		
		m_OffsetX=0;
		m_szIndStr=0 ; m_szNChar=Len ;

	} else {
	
		//Si falta espacio, si IsClip(), recorta, si no muestra todo.
		m_OffsetX=0;
		if ( IsClip() )
		{
			// UNICODE REPORTS.
			// Bug 0125699. Como la cadena de entrada viene en UTF8, algún carácter
			// puede ocupar más de un byte. Es por esta razón que necesitamos saber
			// el número de caracteres de la cadena (no el número de bytes) y 
			// recortar a esta cantidad.
			m_szIndStr=0 ; 
			m_szNChar=m_poFont->TrimRightStr(m_oSourceStr.InChar(0),GetWidth()) ;

			// Averiguar los bytes a los que corresponde m_szNChar (carácteres).
			// Por ejemplo, para "página" en UTF8, m_szNChar = 6 caracteres, 
			// pero realmente queremos recortar a 7 bytes.
			iszNChar = m_szNChar;
			iChars = M4StrLen(m_oSourceStr.InChar(0), iszNChar);
			iszNChar += iszNChar - iChars;

			// Ahora ya tenemos los bytes a los que se corresponde los m_szNChar 
			// caracteres originales.
			iChars = M4StrLen(m_oSourceStr.InChar(0), iszNChar);
			m_szNChar += iszNChar - iChars;

			// Verificar que el caracter en el que cortamos no es un caracter compuesto
			// por más de un byte.
			size_t iSize = 0;
			size_t iPosition = 0;
			iPosition = M4CharPosition(m_oSourceStr.InChar(0), m_oSourceStr.StrLen(), m_szNChar - 1, iSize);
			if (iSize > 1)
			{
				m_szNChar += iSize - 1;
			}

			m_oSourceStr.Trim(m_szNChar);
		}
		else
		{
			m_szIndStr=0;
			m_szNChar=Len;
		} ;
	} ;

	//---------Alinear eje Y

	DvcLen=m_poFont->GetDvcHeight();
	SobraDvc=GetHeight()-DvcLen;
	if (SobraDvc>=0) {
			
		//Si sobra espacio o va justo, muestra todo y alinea
		if (m_AlignY==LEFT) {
			m_OffsetY=0;
		} else if (m_AlignY==CENTER) {
			m_OffsetY=SobraDvc/2;
		} else if (m_AlignY==RIGHT) {
			m_OffsetY=SobraDvc;
		} else if (m_AlignY==POINT) {
			m_OffsetY=SobraDvc/2;   //Cubre posible error alineando a CENTER

			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FIELD_MSG_60 ] ) 
				<< m_oLocation << SEND_MSG ;
		};

	} else {

		//Si falta espacio, lo ignora, y muestra.

		m_OffsetY=0;
	} ;

	m_oFormatedStr.StrCpy(m_oSourceStr.InChar(m_szIndStr)) ;

	return 0 ;
}



m4bool_t ClENG_FieldStr::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) 
{
	size_t PiSize;
	m4uint16_t IndexTargetID;
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ;
	

	ai_Y += m_OffsetY ;
	ai_X += m_OffsetX ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		m_oValue.GetTraceValue(&strAux) ;

		if ( !IsPi() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
				"Printing STANDARD Field %0:s at (y=%1:s mm,x=%2:s mm), value %3:s from Expression %4:s, font %5:s size %6:s points", 
				ClMIT_Msg::MIXED_OUT) 
				<< m_oDesc.InChar(0)
				<< ClMetric::DvcToMilim(ai_Y)
				<< ClMetric::DvcToMilim(ai_X)
				<< strAux.InChar(0)
				<< m_oExpr.GetExpressionStr()
				<< m_poFont->RVW_GetFaceName()
				<< m_poFont->GetMapHeight()
				<< SEND_MSG ;
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
				"Printing PRE PRINT field %0:s, pi id %1:s, at (y=%2:s mm,x=%3:s mm), value %4:s from Expression %5:s, font %6:s size %7:s points", 
				ClMIT_Msg::MIXED_OUT) 
				<< m_oDesc.InChar(0)
				<< m_oIdPI.InChar(0)
				<< ClMetric::DvcToMilim(ai_Y)
				<< ClMetric::DvcToMilim(ai_X)
				<< strAux.InChar(0)
				<< m_oExpr.GetExpressionStr()
				<< m_poFont->RVW_GetFaceName()
				<< m_poFont->GetMapHeight()
				<< SEND_MSG ;
		} ;
	}
	#endif

	Dvc->SelectColour(ai_iNumPage, m_uiIdFGColour); 
	Dvc->SelectFont(ai_iNumPage, m_poFont); 

	if(m_iIndexLevel!=ClENG_Field::NO_INDEX_ENTRY) {
		Dvc->NewIndexEntry(ai_iNumPage, m_iIndexLevel, m_oIndexStr, &IndexTargetID);
	} else {
		IndexTargetID = ClENG_OutDevice::DVC_NO_INDEX_TARGET ;
	} ;
	
	if (IsPi())  {

		// Calcular el size en funcion de lo que tenemos  y lo que mide la font. 

		PiSize = (GetWidth()/m_poFont->GetDvcDesignMaxWidthChar());
		Dvc->PrintNCharPi(&m_oIdPI,ai_iNumPage, ai_X, ai_Y, 
			PiSize, M4_FALSE, m_oHyperlinkStr, IndexTargetID) ;

	} else  {

		Dvc->PrintNChar( 
			ai_iNumPage, ai_X, ai_Y, m_oFormatedStr.InChar(m_szIndStr),
			m_szNChar, M4_FALSE, m_oHyperlinkStr,IndexTargetID) ;
	}

	return M4_TRUE ;
}


void ClENG_FieldStr::ResolvePi(ClMIT_ChnValue *ai_poValue)
{
	//Formatear la cadena que llega
	LoadValue(ai_poValue);
	// Resolucion a nivel de dispositivo
	m_poFieldEnv->m_poDvc->ResolvePi(&m_oIdPI,m_oFormatedStr.InChar(m_szIndStr));
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		ai_poValue->GetTraceValue(&strAux) ;
	
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
			"Resolving PRE PRINT field %0:s, pi id %1:s with value %2:s", 
			ClMIT_Msg::MIXED_OUT) 
			<< m_oDesc.InChar(0)
			<< m_oIdPI.InChar(0)
			<< strAux.InChar(0)
			<< SEND_MSG ;
	} ;
	#endif

	//Salva valor
	if ( m_oSave.IsLinked() ) {
		m_oSave.m_oValue.Init(ai_poValue) ;
		m_oSave.Save() ;
	} ;
}


//==============================================================================ClENG_FieldStrBox

ClENG_FieldStrBox::ClENG_FieldStrBox() {
	m_poFont=NULL;
}

void ClENG_FieldStrBox::End() {
	m_poFont=NULL;
	m_oNULLDef.End() ;
	m_oNULLFormatDef.End() ;
	ClENG_Field::End();
}


void ClENG_FieldStrBox::Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, const m4char_t *ai_pcExpr,
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel, 
		m4char_t* ai_pcHyperlink, size_t ai_szNBoxes,
		m4bool_t ai_bSwStatic,m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		ClENG_FieldStr::ALIGN ai_AlignX, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, ClENG_Format *ai_poFormat,
		ClENG_FieldStr::FILL_MODE ai_FillMode,
		m4char_t ai_cFillChar,
		StMIT_NULLDisplayDef *ai_poNULLDef, StMIT_NULLDisplayDef * ai_poNULLFormatDef, 
		m4bool_t ai_bSwFormatNULL )		
{
	DvcUnit_t ai_Height = ai_poFont->GetDvcDesignHeight();	
	
	ClENG_FieldStrBox::End() ;

	m_AlignX=ai_AlignX ; 
	m_poFormat=ai_poFormat ;
	m_poFont=ai_poFont ;
	m_FillMode = ai_FillMode ;
	m_cFillChar=ai_cFillChar ;
	m_uiIdFGColour = ai_uiIdFGColour ;

	if (ai_Width<0) ai_Width=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Width) ;
	if ( ai_Top<0 )  ai_Top=m_poFont->GetDvcDesignHeight() * (-ai_Top);
	if (ai_Left<0) ai_Left=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Left) ;
	m_szNBoxes=ai_szNBoxes ;
	m_SizeBox=ai_Width/m_szNBoxes ;

	if (ai_poNULLDef) m_oNULLDef.Init(ai_poNULLDef) ; 
	if (ai_poNULLFormatDef) m_oNULLFormatDef.Init(ai_poNULLFormatDef) ; 
	m_bSwFormatNULL=ai_bSwFormatNULL ;

	if (ai_uiIndexLevel!=ClENG_Field::NO_INDEX_ENTRY) {
		//ERROR LOCAL
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163011, "Fields of type Box can't be an entry of the Table of Contents", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
	};
	
	ClENG_Field::Init(ai_poFieldEnv,ai_poLocation,ai_Top, ai_Left, ai_Width, ai_Height, ai_pcExpr, 
		ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		M4_FALSE, M4_FALSE, NULL);
	
}


DvcUnit_t ClENG_FieldStrBox::PostLoadValue() 
{
	size_t Len, SobraChar;
	FORMAT_DATA_TYPE Type ;
	ClENG_Format *Format ;
	m4uint16_t iIndex;
	size_t FillLeft, FillRight ;

	//--------------Obtencion de la cadena formateada, la dejo en m_oFormatedStr
	
	Format=m_poFormat ;
	if( !Format && m_oValue.GetType()==ClMIT_ChnValue::CURRENCY && 
		(!m_oValue.IsNULL() || ( m_oValue.IsNULL() && IsFormatNULL() )  ) ) 
	{
		//Sera NULL si no hay formato
		Format= m_poFieldEnv->m_poDBFormat->GetCurDefaultFormat(
			m_oValue.GetCurrencyType(), M4_FALSE, ClMIT_Trace::PROPAGATE_ERROR);
	};

	if ( m_oValue.IsNULL() ) {

		if (!IsFormatNULL() || !Format ) { 
			m_oFormatedStr.StrCpy(m_oNULLDef.m_oStr[m_oValue.GetType()]) ;
		} else {
			m_oValue.GetInputFormat( &m_oFormatedStr, &Type ) ;	//Para obtener tipo
			Format->Apply(m_oNULLFormatDef.m_oStr[m_oValue.GetType()], 
				Type, &m_oFormatedStr) ;
		} ;

	} else if(m_oValue.GetType()==ClMIT_ChnValue::NO_INIT){

		m_oFormatedStr.StrCpy("") ;

	} else {
	
		if ( m_oValue.GetInputFormat( &m_oFormatedStr, &Type ) ) {
			//Tipo de dato formateable
			if (Format) {
				Format->Apply(m_oFormatedStr, Type, &m_oSourceStr) ;
				m_oFormatedStr.StrCpy(&m_oSourceStr) ;
			} ;
		} ; 
	} ;

	//---------Alinear eje X
		
	Len=m_oFormatedStr.StrLen();
	if (m_szNBoxes>Len) {		
			
		//Si sobra espacio, muestra todo y alinea. 
		//Relleno de char solo posible en este caso

		SobraChar=m_szNBoxes-Len;		//No se resta antes porque SobraChar es unsigned.

		if ( GetFillMode()==ClENG_FieldStr::NO_FILL || ( GetFillMode()==ClENG_FieldStr::NO_EMPTY_FILL && 
			m_oFormatedStr.StrLenTrim()==0 ) ) 
		{
			m_szNChar=Len ;
			if (m_AlignX==ClENG_FieldStr::LEFT) {
				m_szSkipBox=0;
			} else if (m_AlignX==ClENG_FieldStr::CENTER) {
				m_szSkipBox=SobraChar/2;
			} else if (m_AlignX==ClENG_FieldStr::RIGHT) {
				m_szSkipBox=SobraChar;
			} ;
		} else {
			m_szSkipBox=0;
			FillLeft=FillRight=0;
			if (m_AlignX==ClENG_FieldStr::LEFT) {
				FillRight=SobraChar ;
			} else if (m_AlignX==ClENG_FieldStr::CENTER) {
				FillLeft=SobraChar/2;
				FillRight=SobraChar-FillLeft ;	//Ajuste perfecto, no importan decimales
			} else if (m_AlignX==ClENG_FieldStr::RIGHT) {
				FillLeft=SobraChar ;
			} ;
			m_oFormatedStr.StrInsertChar( 0, FillLeft, GetFillChar()) ;
			m_oFormatedStr.StrInsertChar( ClMIT_Str::STR_CAT, FillRight,  GetFillChar() ) ;
			m_szNChar=Len+FillRight+FillLeft ; 
		} ;

	} else if (m_szNBoxes==Len) {

		//Si el espacio es justo, muestra todo
			
		m_szSkipBox=0; m_szNChar=Len ;

	}
	else
	{
		// Si falta espacio, recorta.

		m_szSkipBox = 0;
		m_szNChar = m_szNBoxes;
		m_oFormatedStr.Trim(m_szNChar);
	} ;

	return 0 ;
}

m4bool_t ClENG_FieldStrBox::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) 
{
	size_t B ; 
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ;
	m4char_t *Str=m_oFormatedStr.InChar(0) ;

	Dvc->SelectColour(ai_iNumPage, m_uiIdFGColour); 
	Dvc->SelectFont(ai_iNumPage, m_poFont); 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		m_oValue.GetTraceValue(&strAux) ;
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
			"Printing BOX Field %0:s at (y=%1:s mm,x=%2:s mm), value %3:s from Expression %4:s, font %5:s size %6:s points", 
			ClMIT_Msg::MIXED_OUT) 
			<< m_oDesc.InChar(0)
			<< ClMetric::DvcToMilim(ai_Y)
			<< ClMetric::DvcToMilim(ai_X)
			<< strAux.InChar(0)
			<< m_oExpr.GetExpressionStr()
			<< m_poFont->RVW_GetFaceName()
			<< m_poFont->GetMapHeight()
			<< SEND_MSG ;
	}
	#endif

	ai_X += m_szSkipBox*m_SizeBox ;
	for (B=0; B<m_szNChar; ++B ) {
		Dvc->PrintNChar( ai_iNumPage, ai_X, ai_Y, Str, 1 , M4_FALSE, m_oHyperlinkStr) ;
		++Str ;
		ai_X+=m_SizeBox ;
	} ;

	return M4_TRUE ;
}

//==========================================================================ClENG_FieldMultiL

ClENG_FieldMultiL::ClENG_FieldMultiL() 
{
	m_poFont=NULL;
}

void ClENG_FieldMultiL::End() 
{
	m_poFont=NULL;
	ClENG_Field::End();
}



void ClENG_FieldMultiL::Init(	
	StENG_FieldEnv * ai_poFieldEnv,
	ClMIT_IdNumDesc *ai_poLocation,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
	DvcUnit_t ai_Height, DvcUnit_t ai_InterLine, m4bool_t ai_bIsVarHeight,
	m4bool_t ai_bIsJustified, const m4char_t *ai_pcStr, 
	const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel, 
	m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
	m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize, 
	m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
	ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
	m4uint32_t ai_uiIdFGColour, 
	ClFONT_M_Font * ai_poFont, m4bool_t ai_bSwClip  ) 
{

	ClENG_FieldMultiL::End() ;

	if(ai_bIsVarHeight) ai_bSwClip=M4_FALSE;

	m_InterLine=ai_InterLine;

	m_uiIdFGColour = ai_uiIdFGColour ;
	m_poFont=ai_poFont ;

	if ( ai_Height<0 )  ai_Height=(m_poFont->GetDvcDesignHeight()+ai_InterLine)*(-ai_Height);
	if (ai_Width<0) ai_Width=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Width) ;
	if ( ai_Top<0 ) {
		ai_Top = m_poFont->GetDvcDesignHeight() * (-ai_Top);
	};

	if (ai_Left<0) ai_Left=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Left) ;

	m_oFlag.Switch(CLIP,ai_bSwClip);
	m_oFlag.Switch(JUSTIFIED,ai_bIsJustified);

	if (ai_uiIndexLevel!=ClENG_Field::NO_INDEX_ENTRY) {
		//ERROR LOCAL
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163012, "Fields of type MultiLine can't be an entry of the Table of Contents", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
	};

	m_LineHeight = m_poFont->GetDvcDesignHeight()+m_InterLine ;

	// Altura total de las líneas a imprimir.
	m_iTotalLineHeight = 0;

	// Alineamiento.
	m_AlignX = ai_AlignX;
	m_AlignY = ai_AlignY;
	m_OffsetX = 0;
	m_OffsetY = 0;

	ClENG_Field::Init(ai_poFieldEnv,ai_poLocation, ai_Top, ai_Left, ai_Width, ai_Height, ai_pcStr, 
		ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		ai_bIsVarHeight, M4_TRUE, NULL );

}

void ClENG_FieldMultiL::NewPrintCycle() 
{
	m_CurrentPos=0 ;
	m_HeightInLines=GetHeight()/m_LineHeight ;
	m_LinesPrinted=0 ;
}


DvcUnit_t ClENG_FieldMultiL::PostLoadValue() 
{
	DvcUnit_t r=0 ;

	if ( m_oValue.GetType()!=ClMIT_ChnValue::STRING ) {
		//Error local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FIELD_MSG_10 ] ) 
			<< m_oExpr.GetExpressionStr() << SEND_MSG ;
		goto exit ;
	} ;

	r = PreCalculateLinesToPrint() * m_LineHeight;
	m_iTotalLineHeight = r;

exit:
	return r ;
}

m4bool_t ClENG_FieldMultiL::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) 
{
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ; 
	char *nextL, *prevL ;
	char *BegLine ;
	
	size_t CharsToPrint ;
	m4bool_t SwCRLF, SwAllPrinted=M4_TRUE ;
	DvcUnit_t LeftLines; 
	m4double_t auxD1, auxD2 ;

	m4bool_t	bIsJustified = M4_FALSE;
	DvcUnit_t	iXPos = 0;
	DvcUnit_t	iYPos = 0;
	
	if ( m_oValue.GetType()!=ClMIT_ChnValue::STRING ) {
		//Error local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FIELD_MSG_10 ] ) 
			<< m_oExpr.GetExpressionStr() << SEND_MSG ;
		goto exit ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		m_oValue.GetTraceValue(&strAux) ;
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
			"Printing MULTILINE Field %0:s at (y=%1:s mm,x=%2:s mm), value %3:s from Expression %4:s, font %5:s size %6:s points", 
			ClMIT_Msg::MIXED_OUT) 
			<< m_oDesc.InChar(0)
			<< ClMetric::DvcToMilim(ai_Y)
			<< ClMetric::DvcToMilim(ai_X)
			<< strAux.InChar(0)
			<< m_oExpr.GetExpressionStr()
			<< m_poFont->RVW_GetFaceName()
			<< m_poFont->GetMapHeight()
			<< SEND_MSG ;
	}
	#endif

	Dvc->SelectColour(ai_iNumPage, m_uiIdFGColour); 
	Dvc->SelectFont(ai_iNumPage, m_poFont); 

	//Haya cuantas lineas caben en el intervalo. Si una pilla entre medio, se la asignara
	//	a la impresion parcial con mayor trozo.

	LeftLines = ai_PrintInterval / m_LineHeight  ;
	auxD1 = modf( ai_PrintInterval/(m4double_t)m_LineHeight, &auxD2 ) ;
	if (auxD1>0.5) ++LeftLines ; 

	//Ajusta LeftLines para Clip
	if( IsClip() ) {
		LeftLines = M4_MIN(LeftLines, m_HeightInLines-m_LinesPrinted ) ;
	} ;

	// Verificar si está justificado.
	bIsJustified = IsJustified();

	nextL = m_oValue.GetString() +  m_CurrentPos ;
	while (M4_TRUE) 
	{
		prevL = nextL ;
		nextL = ExtractLine( nextL, &BegLine, &CharsToPrint, &SwCRLF ) ;

		//>>>>>>>>>>>>>>>>>>>>>>>>PROCESO LINEA BegLine-CharsToPrint

		//Test salidas por impresiones parciales o clip

		if ( LeftLines <=0 ) {
			if (  IsVariableHeight() ) {

				//En los de altura variable, no hay concepto de clip o sobreescritura

				SwAllPrinted =  M4_FALSE ;
				break ;

			} else {

				//En los que no son de altura variable, puede sobrescribirse
				
				if ( m_LinesPrinted >= m_HeightInLines ) {

					//Se sobrepasa el campo, siempre es como si se imprimiese todo, 
					//	solo que si IsClip() no se imprime, y si no se imprime.
				
					if ( IsClip() ) {
						break ;		//break, pero AllPrinted !
					} else {
						; //AllPrinted, se deja acabar machacando
					} ;

				} else {

					//Aun no se sobrepasa el campo, impresion parcial
		
					SwAllPrinted =  M4_FALSE ;
					break ;
				} ;

			} ;
		} ;

		//**La linea cabe seguro, imprimirla !


		//Impresión
		if ( ! bIsJustified || SwCRLF ) 
		{

			//Sin justificación, o porque no la tiene o por CRLF o por ultima linea
			//Simplemente, imprimr todos los char

			// Recorta separadores iniciales.
			// Quitar los espacios a la izquierda.
			while ( CharsToPrint && strchr(MULTI_LINE_SPACE, *BegLine) ) 
			{
				++BegLine; --CharsToPrint;
			} ;
			
			// Quitar los espacios del final.
			while (CharsToPrint > 0)
			{
				if (BegLine[CharsToPrint - 1] == MULTI_LINE_SPACE_CHAR)
				{
					CharsToPrint--;
				}
				else
				{
					break;
				}
			}
			
			// Bug 0165349: calcular el alineamiento en el eje X y en el eje Y.
			// El alineamiento solo aplica en el caso de que el campo
			// no tenga justificación.
			if (bIsJustified == M4_FALSE)
			{
				SetAligment(BegLine, CharsToPrint);
				
				// Aumentar las coordenadas X, Y como consecuencia del alineamineto.
				iXPos = ai_X + m_OffsetX;
				iYPos = ai_Y + m_OffsetY;
			}
			else
			{
				iXPos = ai_X;
				iYPos = ai_Y;
			}

			if (CharsToPrint)
			{

				Dvc->PrintNChar( ai_iNumPage, iXPos, iYPos, BegLine, CharsToPrint, M4_FALSE,
					m_oHyperlinkStr ) ;
			} ;

		} else {

			//Varias impresiones parciales, para distribuir blancos

			PrintJustified( ai_iNumPage, ai_Y, ai_X, BegLine, CharsToPrint ) ;

		} ;

		//Computos de linea impresa
		
		m_CurrentPos += (nextL-prevL) ;
		--LeftLines ;
		++m_LinesPrinted ;
		ai_Y += m_LineHeight ; 

		//SE PROCESO ULTIMA LINEA, TODO IMPRESO !
		if ( ! *nextL )  break ;

		//<<<<<<<<<<<<<<<<<<<<<<<<
	} ;

exit:

	return SwAllPrinted ;
}

m4uint32_t ClENG_FieldMultiL::PreCalculateLinesToPrint()
{ 
	m4uint32_t lcount ;
	char *nextL ;
	char *BegLine ;
	size_t CharsToPrint ;
	m4bool_t SwCRLF ;

	
	lcount=0, nextL=m_oValue.GetString() ;
	while (M4_TRUE) {
		nextL = ExtractLine( nextL, &BegLine, &CharsToPrint, &SwCRLF ) ;
		++lcount ;
		if ( ! *nextL )  break ;
	} ;
	return lcount ;
}

char * ClENG_FieldMultiL::ExtractLine( 
	char * ai_Pos, char **ao_BegLine, size_t *ao_CharsToPrint, 
	m4bool_t *ao_SwCRLF ) 
{
	char		*tk = NULL;
	size_t		lenTk = 0;
	size_t		lenAux = 0;
	size_t		iDefaultLen = 0;
	DvcUnit_t	DvcQuedan = 0;
	DvcUnit_t	DvcTk = 0;
	char		* nextLine = NULL;
	char		auxChr;
	char		* auxPos = NULL;
	m4int32_t	iWidth = 0;
	m4bool_t	bCRLFFound = M4_FALSE;
	
	for (tk = ai_Pos, DvcQuedan = GetWidth(), *ao_BegLine = ai_Pos, nextLine = ai_Pos, *ao_CharsToPrint = 0, *ao_SwCRLF = M4_FALSE; 
		*tk; tk += lenTk) 
	{	
		// Token indicado por tk-lentk es una palabra o bloque de separadores.
		// Si token vacio, se sale, no hay nada para procesar.
		lenTk = strcspn(tk, MULTI_LINE_WORD_SEPARATOR);
		if (!lenTk)
		{
			lenTk = strspn(tk, MULTI_LINE_WORD_SEPARATOR);
		}

		if (!lenTk)
		{
			break;
		}

		// Testea apariciones de [CRLF] en medio de palabras.
		auxPos = strstr(tk, MULTILINE_CRLF_TAG);
		if (auxPos != NULL)
		{
			bCRLFFound = M4_TRUE;
			iDefaultLen = MULTILINE_CRLF_TAG_LEN;

			// Len de la cadena antes del tag [CRLF].
			lenAux = auxPos - tk;

			// Buscar si antes del tag [CRLF] hay un retorno de carro (0A).
			m4pchar_t pcBuffer = new m4char_t[lenAux + 1];
			strncpy(pcBuffer, tk, lenAux);
			pcBuffer[lenAux] = '\0';
			
			auxPos = strstr(pcBuffer, MULTILINE_LF);
			if (auxPos != NULL)
			{
				// Hemos encontrado un retorno de carro (0A). Tenemos que tratarlo
				// igual que el tag [CRLF].
				bCRLFFound = M4_TRUE;
				lenAux = auxPos - pcBuffer;
				iDefaultLen = MULTILINE_LF_LEN;
				if (lenAux > 0 && tk[lenAux - 1] == MULTILINE_CR_CHAR)
				{
					// Si encontramos el \r justo antes de \n, siginifica que 
					// tenemos que imprimir un caracter menos.
					lenAux--;
					iDefaultLen = MULTILINE_CRLF_LEN;
				}
			}

			delete [] pcBuffer;
		}
		else
		{
			// No se ha encontrado tag [CRLF], pero aún puede haber retornos de carro 0A.
			auxPos = strstr(tk, MULTILINE_LF);
			if (auxPos != NULL)
			{
				// Hemos encontrado un retorno de carro (0A). Tenemos que tratarlo
				// igual que el tag [CRLF].
				bCRLFFound = M4_TRUE;
				lenAux = auxPos - tk;
				iDefaultLen = MULTILINE_LF_LEN;
				if (lenAux > 0 && tk[lenAux - 1] == MULTILINE_CR_CHAR)
				{
					// Si encontramos el \r justo antes de \n, siginifica que 
					// tenemos que imprimir un caracter menos.
					lenAux--;
					iDefaultLen = MULTILINE_CRLF_LEN;
				}
			}
		}

		// Hay ocurrencia del tag [CRLF] o de retorno de carro (0A).
		if (bCRLFFound == M4_TRUE && lenAux < lenTk)
		{
			// Ocurre al principio, recortar lenTk por si no hay separador luego.
			if (lenAux == 0)
			{
				lenTk = iDefaultLen;
			} 
			else 
			{
				// No ocurre al principio, el propio tag es un separador.
				lenTk = lenAux;
			}
		}

		DvcTk = m_poFont->GetDvcLenStr(tk, lenTk);
		iWidth = GetWidth();

		if ((strnicmp(tk, MULTILINE_CRLF_TAG, lenTk) == 0) || (strnicmp(tk, MULTILINE_CRLF, lenTk) == 0) || (strnicmp(tk, MULTILINE_LF, lenTk) == 0))
		{
			// Tag CRLF o retorno de carro.
			nextLine = tk + lenTk;
			*ao_SwCRLF = M4_TRUE;
			break;
		}
		else if (DvcTk <= DvcQuedan)
		{
			// Token cabe en la misma linea.
			*ao_CharsToPrint += lenTk;
			DvcQuedan -= DvcTk;
			nextLine = tk + lenTk;
		} 
		else if (DvcTk < iWidth)
		{
			// Token no cabe en esta linea, pero si cabrá en la siguiente; NO imprimir
			nextLine = tk;
			break ;
		}
		else
		{
			// Token es mas ancho que el field. Imprimir en esta linea lo que se pueda y salir.
			auxChr = tk[lenTk];
			tk[lenTk] = M4_END_STR;
			auxPos = &tk[lenTk];
			lenTk = m_poFont->TrimLeftStr(tk, DvcQuedan);

			// Si no cabe ninguno, hay dos posibiliades:
			//	-En esta linea ya se imprimio algo: no sacar nada de este token, hacer como 
			//		si no cupiese en esta linea pero si en la siguiente.
			//  -No se imprimio nada en esta linea. Fuerzar la impresion de uno o caeremos
			//		en un bucle recursivo.
			*auxPos = auxChr;
			if (lenTk == 0)
			{
				if (*ao_CharsToPrint > 0)
				{
					nextLine = tk;
					break;
				}
				else
				{
					lenTk = 1;
				}
			}

			DvcTk = m_poFont->GetDvcLenStr(tk, lenTk);
			*ao_CharsToPrint += lenTk;
			DvcQuedan -= DvcTk;
			DvcQuedan = M4_MAX(DvcQuedan, 0);
			nextLine = tk + lenTk;
			break;
		}
	}

	if (! *nextLine) 
	{
		*ao_SwCRLF = M4_TRUE;
	}

	return nextLine;
}


void ClENG_FieldMultiL::PrintJustified( 
	m4int16_t ai_iNumPage, DvcUnit_t ai_Y, DvcUnit_t ai_X, char *ai_BegLine, size_t ai_CharsToPrint ) 
{
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ; 
	char *posAux, *posBackup, charBackup ;

	char *tk ;
	size_t lenTk, blankBlocks ;
	size_t  blanksToInsert, blanksByBlock, blanksFosLastBlock ;
	size_t	iRemainingBlanks = 0;

	//Recorta separadores iniciales
	while ( ai_CharsToPrint && strchr(MULTI_LINE_SPACE,*ai_BegLine) ) {
		++ai_BegLine; --ai_CharsToPrint ;
	} ;
	if (!ai_CharsToPrint) return ;

	//Recorta separadores finales
	posAux=&ai_BegLine[ai_CharsToPrint-1] ;
	while ( ai_CharsToPrint && strchr(MULTI_LINE_SPACE,*posAux) ) {
		--posAux; --ai_CharsToPrint ;
	} ;
	if (!ai_CharsToPrint) return ;

	//Hacemos a ai_BegLine una cadena ASCIIZ para facilitarnos la vida
	posBackup=&ai_BegLine[ai_CharsToPrint] ;
	charBackup=*posBackup ;	*posBackup = M4_END_STR ;

	//Se haya blanksToInsert, que es el número de blancos a distribuir en la cadena 
	blanksToInsert = ( GetWidth() - m_poFont->GetDvcLenStr(ai_BegLine,ai_CharsToPrint) )
		/ m_poFont->GetDvcWidthChar( ' ' ) ;

	if (! blanksToInsert ) {

		//Si no hay blancos que distribuir, imprimimos todo tal cual y listos

		Dvc->PrintNChar( ai_iNumPage, ai_X, ai_Y, ai_BegLine, ai_CharsToPrint, M4_FALSE,
			m_oHyperlinkStr ) ;

	} else {

		ClMIT_Str strLine ;

		//Ahora hay que contar cuantos bloques de separadores intermedios quedan.
		//Los que queden son intermedios porque los de los lados nos los hemos cargado

		for ( tk=ai_BegLine, blankBlocks=0; *tk; tk += lenTk ) {
			lenTk=strcspn( tk, MULTI_LINE_WORD_SEPARATOR ) ;
			if (!lenTk) {
				lenTk=strspn( tk, MULTI_LINE_WORD_SEPARATOR ) ;
				++blankBlocks ;
			} ;
		} ;
		
		//Si no hay bloques intermedios para distribuir, imprimimos todo tal cual como un 
		//	bloque, pero distribuimos blancos al principio y al final (equivalente a solo al 
		//	principio).
		 
		if (!blankBlocks) {

			strLine.StrCpy(ai_BegLine) ;
			strLine.StrInsertChar(0, blanksToInsert/2, ' ') ;
			
			Dvc->PrintStr( ai_iNumPage, ai_X, ai_Y, strLine.InChar(0), M4_FALSE,
				m_oHyperlinkStr ) ;

		} else {

			size_t insertPos, blankBlockPrinted, insertAux ;
	
			//Hay que distribuir los blancos entre los bloques; en todos se inserta lo mismo,
			// y en el ultimo el resto.
	
			blanksByBlock = blanksToInsert / blankBlocks ;
			iRemainingBlanks = blanksToInsert % blankBlocks ;
			blanksFosLastBlock = blanksByBlock + 
				(blanksToInsert-(blanksByBlock*blankBlocks))  ;

			for ( 
				tk=ai_BegLine, strLine.StrCpy(""), insertPos=0, blankBlockPrinted=0 ; 
				*tk; 
				tk += lenTk ) 
			{
				lenTk=strcspn( tk, MULTI_LINE_WORD_SEPARATOR ) ;
				if (!lenTk) {

					lenTk=strspn( tk, MULTI_LINE_WORD_SEPARATOR ) ;
					
					// Bloque separadores
					if (blankBlockPrinted == blankBlocks - 1)
					{
						insertAux = blanksFosLastBlock;
					}
					else
					{
						if (iRemainingBlanks != 0)
						{
							insertAux = blanksByBlock + 1;
							blanksFosLastBlock = blanksFosLastBlock - 1;
							iRemainingBlanks = iRemainingBlanks - 1;
						}
						else
						{
							insertAux = blanksByBlock;
						}
					}

					strLine.StrInsertStr( insertPos, tk, lenTk )  ;
					insertPos += lenTk ;
				
					strLine.StrInsertChar(insertPos, insertAux, ' ') ;
					insertPos += insertAux ;

					++blankBlockPrinted ;

				} else {

					//Bloque palabra

					strLine.StrInsertStr( insertPos, tk, lenTk )  ;
					insertPos += lenTk ;

				} ;
			} ;
			Dvc->PrintNChar( ai_iNumPage, ai_X, ai_Y, strLine.InChar(0), insertPos, M4_FALSE,
				m_oHyperlinkStr ) ;
		} ;
	} ;

	//Restauramos el caracter alterado

	*posBackup = charBackup  ;

}

DvcUnit_t ClENG_FieldMultiL::AdjustCntInterval( DvcUnit_t ai_PrintInterval) 
{
	return ((m4uint32_t)ai_PrintInterval/(m4uint32_t)m_LineHeight)*m_LineHeight ;
}


m4void_t ClENG_FieldMultiL::SetAligment(char * ai_pLine, size_t ai_iCharNum)
{
	m4int32_t	iWidth = 0;
	m4int32_t	iHeight = 0;
	DvcUnit_t	iSobraDvcX = 0;
	DvcUnit_t	iSobraDvcY = 0;
	DvcUnit_t	iDvcX = 0;

	// Calcular dvc units para los caracteres que se quieren imprimir.
	iDvcX = m_poFont->GetDvcLenStr(ai_pLine, ai_iCharNum);

	iWidth = GetWidth();
	iHeight = GetHeight();
	iSobraDvcX = iWidth - iDvcX;
	iSobraDvcY = iHeight - m_iTotalLineHeight;
	
	// Alinear eje X.
	if (iSobraDvcX > 0)
	{
		if (m_AlignX == ClENG_FieldStr::LEFT)
		{
			m_OffsetX = 0;
		}
		else if (m_AlignX == ClENG_FieldStr::CENTER)
		{
			m_OffsetX = iSobraDvcX / 2;
		}
		else if (m_AlignX == ClENG_FieldStr::RIGHT)
		{
			m_OffsetX = iSobraDvcX;
		}
	}
	else
	{
		// Si falta espacio, lo ignora, y muestra.
		m_OffsetX = 0;
	}

	// Alinear eje Y.
	if (iSobraDvcY > 0)
	{
		// Si sobra espacio o va justo, muestra todo y alinea.
		if (m_AlignY == ClENG_FieldStr::LEFT) 
		{
			m_OffsetY = 0;
		}
		else if (m_AlignY == ClENG_FieldStr::CENTER)
		{
			m_OffsetY = iSobraDvcY / 2;
		}
		else if (m_AlignY == ClENG_FieldStr::RIGHT)
		{
			m_OffsetY = iSobraDvcY;
		}
	}
	else
	{
		// Si falta espacio, lo ignora, y muestra.
		m_OffsetY = 0;
	}	
}

//==========================================================================ClENG_FieldsManager	

ClENG_FieldsManager::ClENG_FieldsManager()
{
	m_poInsp=GET_INSP();
}

void ClENG_FieldsManager::End() 
{
	
	PIList_t::iterator itListPi;	

	//  En este punto si hay algun Pi en la lista es que se ha quedado sin resolver
	//	Sacar mensaje por cada uno que aparezca
	for (itListPi = m_oPIList.begin (); itListPi!=m_oPIList.end(); itListPi++)
	{		
		//Efecto Local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ FIELD_MSG_20 ] ) 
			<< (*itListPi).first.InChar(0) << SEND_MSG ;
		//No lo destruyo pues es una relación de uso
	}
    
	// Borrado de la Lista de Pi
	m_oPIList.clear();	
		
} 

// Se le pasa el Device con el que trabaja
void ClENG_FieldsManager::Init()
{   	
	ClENG_FieldsManager::End();

	m_bValueActivated = M4_FALSE ;
}

//añadir el Pi a la lista con clave ai_iIdPI
m4bool_t ClENG_FieldsManager::AddNewPi  (ClMIT_Str *ai_poIdPi,ClENG_Field * ai_opClField,const ClMIT_ChnValue* ai_poValue)
{
	PIList_t::iterator itListPi; // iterador
	stFieldPi NewStPi ;  // estructura de datos a guardar
	
	NewStPi.pField = ai_opClField ;
	NewStPi.ItValue.Init(ai_poValue) ;

	m_oPIList.insert(PIList_t::value_type(*ai_poIdPi,NewStPi));
	return M4_TRUE;	
}

m4bool_t ClENG_FieldsManager::ExtractPi( ClMIT_Str *ai_poIdPi, ClENG_Field * &ao_opClField, ClMIT_ChnValue &ao_poValue)
{
	PIList_t::iterator itListPi;
	
	itListPi=m_oPIList.find(*ai_poIdPi);
	if (itListPi == m_oPIList.end() ) //Not Found
	{	 
		return M4_FALSE;
	
	}
	else
	{
		ao_opClField = ((*itListPi).second).pField;
		ao_poValue.Init( &((*itListPi).second).ItValue );
		m_oPIList.erase(itListPi);
		return M4_TRUE;	
	}
    
}

//==============================================================================ClENG_FieldLine

ClENG_FieldLine::ClENG_FieldLine() {
}

void ClENG_FieldLine::End() {

	m_oLine.End() ;
	m_oLineBackup.End() ;

	ClENG_Field::End();
}


void ClENG_FieldLine::Init( 
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation, 
		m4bool_t ai_bSwStatic, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineTop, DvcUnit_t  ai_LineLeft, DvcUnit_t ai_LineTop2, DvcUnit_t ai_LineLeft2,
		m4bool_t ai_bAdjustLineToContainer,
		DvcUnit_t ai_LineWidth, ClENG_OutDevice::STYLE ai_Style, m4uint32_t ai_uiIdColour, 
		m4bool_t ai_bSwVisible )
{

	ClENG_FieldLine::End() ;

	m_oLine.Init( 
		ai_poFieldEnv->m_poDvc, ai_LineTop, ai_LineLeft, ai_LineTop2, ai_LineLeft2, ai_bAdjustLineToContainer,
		ai_LineWidth, ai_Style, ai_uiIdColour, ai_bSwVisible ) ;

	if (ai_bAdjustLineToContainer)
	{
		m_oLineBackup = m_oLine ;
		m_HeightBackup = ai_Height ;
		m_TopBackup = ai_Top ;
	}

	// Se asocia que la línea sea divisible con que se extienda con el contenedor
	ClENG_Field::Init(ai_poFieldEnv,ai_poLocation, ai_Top, ai_Left, ai_Width, ai_Height, "", 
		"\"\"", ClENG_Field::NO_INDEX_ENTRY,
		"\"\"", ai_bSwStatic, M4_FALSE, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		M4_FALSE, ai_bAdjustLineToContainer, NULL) ;
}

m4bool_t ClENG_FieldLine::SetLine( m4char_t *ai_pcLineProps )
{
	m4bool_t retValue = m_oLine.SetLine(ai_pcLineProps) ;
	if (IsAdjustedToContainer()) m_oLineBackup = m_oLine ;
	return retValue ;
}

void ClENG_FieldLine::NewPrintCycle()
{
	if (IsAdjustedToContainer())
	{
		// Se restaura la configuración de la línea
		m_oLine = m_oLineBackup ;
		SetHeight( m_HeightBackup ) ;
		SetTop ( m_TopBackup ) ;
	}

	m_HeightPrinted = 0 ;
}

DvcUnit_t ClENG_FieldLine::PostLoadValue() 
{
	return 0 ; 
}

m4bool_t ClENG_FieldLine::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) 
{
	DvcUnit_t iLeft = 0;
	DvcUnit_t iTop = 0;
	DvcUnit_t iXPos = 0;
	DvcUnit_t iYPos = 0;

	iLeft = GetLeft();
	iTop = GetTop();
	iXPos = ai_X - iLeft;
	iYPos = ai_Y - iTop;
	
	if (IsPartialPrintAllowed())
	{
		// Se debe incluir el Top como rango de impresión
		// (se añade a la posición vertical y al rango ya impreso)
		m_oLine.Show(ai_iNumPage, iXPos, iYPos + iTop, m_HeightPrinted+ iTop, ai_PrintInterval);
		
		// Se incrementa el intervalo de impresión
		m_HeightPrinted += ai_PrintInterval ;

		// Se devuelve True si la línea ya se ha impreso completamente
		return (GetHeight() <= m_HeightPrinted ? M4_TRUE : M4_FALSE );
	}
	else
	{
		m_oLine.Show(ai_iNumPage, iXPos, iYPos) ;

		return M4_TRUE ;
	}
}

// Determina si el dibujado del campo está condicionado al tamaño final del contendor
m4bool_t ClENG_FieldLine::IsAdjustedToContainer()
{
	return m_oLine.GetAdjustLineToContainer();
}

// Sólo actua en las líneas que tienen definida la propiedad de ajustar su tamaño al contenedor.
// Se invoca en el momento en el que ya está determinado el tamaño del contenedor
void ClENG_FieldLine::EvaluateContainerHeight( DvcUnit_t ai_DesignHeight, DvcUnit_t ai_RealHeight)
{
	if (IsAdjustedToContainer())
	{
		DvcUnit_t iOldX1, iOldX2, iNewX2, iNewY2;
		DvcUnit_t iOldY1 = m_oLine.GetTop();
		DvcUnit_t iOldY2 = m_oLine.GetTop2();

		if (iOldY1 == iOldY2)
		{
			iNewY2 = GetTop() + ai_RealHeight - ai_DesignHeight - 2;
			// Se trata de una línea horizontal. Se ajusta el top del contenedor
			SetTop( iNewY2 ) ;
			// También se deben ajustar los valores Y
			m_oLine.SetTop( iNewY2 );
			m_oLine.SetTop2( iNewY2 );
		}
		else
		{
			// Se ajusta la altura del contenedor
			SetHeight( GetHeight() + ai_RealHeight - ai_DesignHeight ) ;
			// Se mantiene la pendiente de la línea => Se debe ajustar el X del punto inferior de la línea
			iOldX1 = m_oLine.GetLeft();
			iOldX2 = m_oLine.GetLeft2();

			iNewY2 = iOldY2 + ai_RealHeight - ai_DesignHeight;			
			iNewX2 = iOldX1 + (DvcUnit_t)((m4double_t)(iOldX2 - iOldX1) * ((m4double_t)(iNewY2 - iOldY1) / (m4double_t)(iOldY2 - iOldY1)));

			m_oLine.SetLeft2(iNewX2);
			// se ajusta el Y del punto inferior de la línea
			m_oLine.SetTop2(iNewY2);

		}
	}
}