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

#include "eng.h"
#include "engdf.inl"

#include "barcode.h"

#define GROUP_ID ENG_TGI_FIELD


#define BAR_CODE_DIGITS	"0123456789"

//====================================================================ClENG_BarCodeField

ClENG_BarCodeField::ClENG_BarCodeField() 
{
	m_poFont=NULL;
}

void ClENG_BarCodeField::End() 
{
	m_poFont=NULL;
	ClENG_Field::End();
	m_oNULLDef.End() ;
}

void ClENG_BarCodeField::Init( 
	StENG_FieldEnv * ai_poFieldEnv,
	ClMIT_IdNumDesc *ai_poLocation,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
	DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
	const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
	m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
	m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
	m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
	ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
	m4uint32_t ai_uiIdFGColour, 
	ClFONT_M_Font * ai_poFont, m4bool_t ai_bSwClip,
	StMIT_NULLDisplayDef *ai_poNULLDef, m4char_t *ai_pcIdPi)
{
	ClENG_BarCodeField::End() ;
	ClMIT_IdNumDesc IdDesc;	
	IdDesc.Init(1, "code");

	m_AlignX = ai_AlignX ; 
	m_AlignY = ai_AlignY ; 
	m_poFont=ai_poFont ;
		
	m_uiIdFGColour = ai_uiIdFGColour ;
	if ( ai_Height<0 )  ai_Height=m_poFont->GetDvcDesignHeight() * (-ai_Height);
	if (ai_Width<0) ai_Width=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Width) ;
	if ( ai_Top<0 )  ai_Top=m_poFont->GetDvcDesignHeight() * (-ai_Top);
	if (ai_Left<0) ai_Left=m_poFont->GetDvcDesignMaxWidthChar()*(-ai_Left) ;
	m_oFlag.Switch(CLIP,ai_bSwClip);

	if (ai_poNULLDef) m_oNULLDef.Init(ai_poNULLDef) ; 

	if (ai_uiIndexLevel!=ClENG_Field::NO_INDEX_ENTRY) {
		//ERROR LOCAL
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163014, "Fields of type Barcode can't be an entry of the Table of Contents", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		ai_uiIndexLevel = ClENG_Field::NO_INDEX_ENTRY ;
	};
	
	ClENG_Field::Init(ai_poFieldEnv,ai_poLocation,ai_Top, ai_Left, ai_Width, ai_Height, ai_pcExpr, 
		ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		M4_FALSE, M4_FALSE, ai_pcIdPi);
	m_OffsetX=m_OffsetY=0 ;

}


DvcUnit_t ClENG_BarCodeField::PostLoadValue() 
{
	DvcUnit_t SobraDvc, DvcLen ;
	size_t Len;
	ClMIT_Str Aux;
	m4uint16_t iIndex;
	size_t FillLeft, FillRight ;

	//El GetString ya lanza error si el tipo es incorrecto
	//Tras esta llamada, todo se debe hacer respecto a m_oCodedStr

	Encode( m_oValue.GetString(), &m_oCodedStr ) ;

	m_szIndStr=0;
	m_szNChar=m_oCodedStr.StrLen();

	Len=m_oCodedStr.StrLen();
	DvcLen=m_poFont->GetDvcLenStr(m_oCodedStr.InChar(0));
	SobraDvc=GetWidth()-DvcLen;
	
	if (SobraDvc>0) {

		m_szIndStr=0 ; m_szNChar=Len ;
		if (m_AlignX==ClENG_FieldStr::LEFT) {
			m_OffsetX=0;
		} else if (m_AlignX==ClENG_FieldStr::CENTER) {
			m_OffsetX=SobraDvc/2;
		} else if (m_AlignX==ClENG_FieldStr::RIGHT) {
			m_OffsetX=SobraDvc;
		};

	} else if (SobraDvc==0) {
	
		//Si el espacio es justo, muestra todo
		
		m_OffsetX=0;
		m_szIndStr=0 ; m_szNChar=Len ;

	} else {
	
		//Si falta espacio, si IsClip(), recorta, si no muestra todo.
		m_OffsetX=0;
		if ( IsClip() ) {
			m_szIndStr=0 ; 
			m_szNChar=m_poFont->TrimRightStr(m_oCodedStr.InChar(0),GetWidth()) ;
			m_oCodedStr.Trim(m_szNChar) ;
		} else {
			m_szIndStr=0 ; m_szNChar=Len ;
		} ;
	} ;

	//---------Alinear eje Y

	DvcLen=m_poFont->GetDvcHeight();
	SobraDvc=GetHeight()-DvcLen;
	if (SobraDvc>=0) {
			
		//Si sobra espacio o va justo, muestra todo y alinea
		if (m_AlignY==ClENG_FieldStr::LEFT) {
			m_OffsetY=0;
		} else if (m_AlignY==ClENG_FieldStr::CENTER) {
			m_OffsetY=SobraDvc/2;
		} else if (m_AlignY==ClENG_FieldStr::RIGHT) {
			m_OffsetY=SobraDvc;
		};

	} else {
		//Si falta espacio, lo ignora, y muestra.
		m_OffsetY=0;
	} ;

	return 0 ;
}



m4bool_t ClENG_BarCodeField::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) 
{
	size_t PiSize;
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ;
	m4uint16_t IndexTargetID;
	
	ai_X += m_OffsetX ;
	ai_Y += m_OffsetY ;

	Dvc->SelectColour(ai_iNumPage, m_uiIdFGColour); 
	Dvc->SelectFont(ai_iNumPage, m_poFont); 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		m_oValue.GetTraceValue(&strAux) ;
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, 
			"Printing BAR CODE Field %0:s at (y=%1:s mm,x=%2:s mm), value %3:s from Expression %4:s, font %5:s size %6:s points", 
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

	//No se permite TOC 
	IndexTargetID = ClENG_OutDevice::DVC_NO_INDEX_TARGET ;
	
	if (IsPi())  {

		// Calcular el size en funcion de lo que tenemos y lo que mide la font. 

		PiSize = (GetWidth()/m_poFont->GetDvcDesignMaxWidthChar());
		Dvc->PrintNCharPi(&m_oIdPI, ai_iNumPage, ai_X, ai_Y, 
			PiSize, M4_TRUE, m_oHyperlinkStr, IndexTargetID) ;
	} else {

		Dvc->PrintNChar( ai_iNumPage, ai_X, ai_Y, 
			m_oCodedStr.InChar(m_szIndStr), m_szNChar, M4_TRUE, m_oHyperlinkStr) ;
	}

	return M4_TRUE ;
}


void ClENG_BarCodeField::ResolvePi(ClMIT_ChnValue *ai_poValue)
{
	//Formatear la cadena que llega
	LoadValue(ai_poValue);
	// Resolucion a nivel de dispositivo
	m_poFieldEnv->m_poDvc->ResolvePi(&m_oIdPI,m_oCodedStr.InChar(m_szIndStr));
	
	//Salva valor
	if ( m_oSave.IsLinked() ) {
		m_oSave.m_oValue.Init(ai_poValue) ;
		m_oSave.Save() ;
	} ;
}

//===================================================================ClENG_BarCode39

// Bug 0152909. Añadir carácter "-", válido para Code39c
#define BAR_CODE_39_SYMBOLS		"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$%-"
#define BAR_CODE_39_START_STOP  "*"

ClENG_BarCode39::ClENG_BarCode39() 
{
	ClENG_BarCode39::End() ;
}

void ClENG_BarCode39::Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) 
{
	m4bool_t SwOk=M4_TRUE;
	size_t szLocateChar;
	ClMIT_Str oStrAux;

	ao_CodedValue->StrCpy(ai_pcInValue) ;

	szLocateChar=strspn(ao_CodedValue->InChar(0),BAR_CODE_39_SYMBOLS);
	if(szLocateChar!=ao_CodedValue->StrLen()){
		//error
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162991, 
			"Caracter %0:s no soportado por Codigo de Barras (Code 39).", ClMIT_Msg::MIXED_OUT)
			<< ao_CodedValue->InChar(0)[szLocateChar] << SEND_MSG ;
		goto exit;
	};

	//Codifica inicio y final de secuencia
	if(ao_CodedValue->InChar(0)[0]!=BAR_CODE_39_START_STOP[0]) {
		oStrAux.StrCpy(BAR_CODE_39_START_STOP);
		oStrAux.StrCat(ao_CodedValue->InChar(0));
		oStrAux.StrCat(BAR_CODE_39_START_STOP);
		ao_CodedValue->StrCpy(oStrAux.InChar(0));	
	};

exit:

	return ;
}

//===================================================================ClENG_BarCodeI25

#define BAR_CODE_25I_COMPLEMENT	'0'
#define BAR_CODE_25I_GROUP		2
  
ClENG_BarCodeI25::ClENG_BarCodeI25() 
{
	ClENG_BarCodeI25::End() ;
}

void ClENG_BarCodeI25::Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) 
{
	m4bool_t   SwOk=M4_TRUE;
	ClMIT_Str  oStrAux, oStrCodeAux;
	m4uint16_t uiLength;
	size_t szLocateChar;

	ao_CodedValue->StrCpy(ai_pcInValue) ;

	szLocateChar=strspn(ao_CodedValue->InChar(0),BAR_CODE_DIGITS);
	if(szLocateChar!=ao_CodedValue->StrLen()){
		//error
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162992, "Caracter %0:s no soportado por Codigo de Barras (Code 25Interleaved).", ClMIT_Msg::MIXED_OUT)
				<< ao_CodedValue->InChar(0)[szLocateChar] << SEND_MSG ;
		goto exit;
	};

	uiLength=ao_CodedValue->StrLen();
	if(uiLength & 1) {
		ao_CodedValue->StrInsertChar( 0, 1, BAR_CODE_25I_COMPLEMENT );
	};
	oStrAux.StrCpy(ao_CodedValue->InChar(0)) ;	//Cadena sin Start/Stop para recorrer y traducir
	ao_CodedValue->StrCpy("") ;
	ao_CodedValue->StrInsertChar( 0, 1, 
		GetFont()->GetTranslation(ENG_TRANS_I_2_5_START) );

	for( uiLength=0 ; *oStrAux.InChar(uiLength); uiLength+=2 ) {
		oStrCodeAux.ToASCIZ(oStrAux.InChar(uiLength),BAR_CODE_25I_GROUP);
		ao_CodedValue->StrInsertChar(ClMIT_Str::STR_CAT, 1, 
			GetFont()->GetTranslation( ENG_TRANS_I_2_5_ZERO + 
				atoi(oStrCodeAux.InChar(0)) )  );
	};
	ao_CodedValue->StrInsertChar( ClMIT_Str::STR_CAT, 1, 
		GetFont()->GetTranslation(ENG_TRANS_I_2_5_STOP) );

exit:

	return ;
}

//===================================================================ClENG_BarCode128

#define ENG_128_FORMAT_FIRST_CHARS "ABC"
#define ENG_128_FORMAT_CHARS ENG_128_FORMAT_FIRST_CHARS "abcFfZzAas*owhzr0123456789"

void ClENG_BarCode128::End() { 
	m_oFormat.End() ;
	ClENG_BarCodeField::End(); 
}

ClENG_BarCode128::ClENG_BarCode128() 
{
	ClENG_BarCode128::End() ;
}

void ClENG_BarCode128::Init(	
	StENG_FieldEnv * ai_poFieldEnv,
	ClMIT_IdNumDesc *ai_poLocation,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
	DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
	const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
	m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
	m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
	m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
	ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
	m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, 
	m4bool_t ai_bSwClip,
	StMIT_NULLDisplayDef *ai_poNULLDef,
	m4char_t *ai_pcIdPi, 
		
	m4char_t *ai_pc128Format, 
	DEFAULT_MODE ai_DefaultMode, m4bool_t ai_bSwAllowC,  
	m4bool_t ai_bSwFillZeroC ) 
{
	ClENG_BarCode128::End() ;

	ClENG_BarCodeField::Init(
		ai_poFieldEnv, ai_poLocation, ai_Top, ai_Left, ai_Width, ai_Height, 
		ai_pcExpr, ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		ai_AlignX, ai_AlignY, ai_uiIdFGColour, ai_poFont, ai_bSwClip, ai_poNULLDef,
		ai_pcIdPi ) ;

	m_oFormat.StrCpy(ai_pc128Format);
	m_DefaultMode = ai_DefaultMode ;
	m_bSwAllowC = ai_bSwAllowC ;
	m_bSwFillZeroC = ai_bSwFillZeroC ;
}


void ClENG_BarCode128::ChangeToCharSet(ClENG_BarCode128::RT_Data *ai_RT, CURRENT_CHARSET ai_NewCharSet) 
{
	m4int16_t iaux ;
	if (ai_RT->m_CCh!=ai_NewCharSet) {
		if (ai_RT->m_CCh==CHARSET_NO_INIT) {
			switch(ai_NewCharSet) {
			case CHARSET_A:
				iaux=ENG_TRANS_128_START_A ;
				break ;
			case CHARSET_B:
				iaux=ENG_TRANS_128_START_B ;
				break ;
			case CHARSET_C:
				iaux=ENG_TRANS_128_START_C ;
				break ;
			} ;
		} else {
			switch(ai_NewCharSet) {
			case CHARSET_A:
				iaux=  ai_RT->m_CCh==CHARSET_B ? 
					ENG_TRANS_128_B_TO_A : ENG_TRANS_128_C_TO_A ;
				break ;
			case CHARSET_B:
				iaux=  ai_RT->m_CCh==CHARSET_A ? 
					ENG_TRANS_128_A_TO_B : ENG_TRANS_128_C_TO_B ;
				break ;
			case CHARSET_C:
				iaux=  ai_RT->m_CCh==CHARSET_A ? 
					ENG_TRANS_128_A_TO_C : ENG_TRANS_128_B_TO_C ;
				break ;
			} ;
		} ;
		ai_RT->m_Out.StrCatChar(  ai_RT->m_poFont->GetTranslation(iaux) ) ;
	} ;
	ai_RT->m_CCh=ai_NewCharSet ;

}


void ClENG_BarCode128::EncodeC(ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	ClMIT_Str strAux, codeAux ;
	size_t length ;

	if (ai_NChar<0) {
		ai_NChar =  strlen(ai_RT->m_InputPos) ;
	} ;
	if (ai_NChar==0) return ;

	ChangeToCharSet(ai_RT, CHARSET_C) ;

	strAux.ToASCIZ(ai_RT->m_InputPos,ai_NChar) ;
	if ( ai_RT->m_bSwFillZeroC && (ai_NChar&0x0001) ) {
		strAux.StrInsertChar( 0, 1, '0') ;
	} ;

	for( length=0 ; *strAux.InChar(length); length+=2 ) {
		codeAux.ToASCIZ(strAux.InChar(length), 2 );
		ai_RT->m_Out.StrCatChar(
			ai_RT->m_poFont->GetTranslation( ENG_TRANS_128_C_ZERO_POS + 
				atoi(codeAux.InChar(0)) )  );
	};

	ai_RT->m_InputPos += ai_NChar ;
}

size_t ClENG_BarCode128::IsAEncodable(const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) 
{
	size_t R ;
	m4int16_t top ;

	if (ai_NChar<0) {
		ai_NChar =  strlen(ai_pcStr) ;
	} ;
	top = ai_bExclusive ? 31 : 95 ;
	for (R=0 ; ai_NChar; --ai_NChar, ++ai_pcStr, ++R  ) {
		if ( *ai_pcStr>top ) break ;
	} ;
	return R ;
}

size_t ClENG_BarCode128::IsBEncodable(const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) 
{
	size_t R ;
	m4int16_t topMin, topMax ;

	if (ai_NChar<0) {
		ai_NChar =  strlen(ai_pcStr) ;
	} ;
	if (ai_bExclusive) {
		topMin =96;
		topMax =127;
	} else {
		topMin =32;
		topMax =127;
	} ;
	for (R=0 ; ai_NChar; --ai_NChar, ++ai_pcStr, ++R  ) {
		if ( *ai_pcStr<topMin || *ai_pcStr>topMax ) break ;
	} ;
	return R ;
}
size_t ClENG_BarCode128::IsABEncodable(
	CURRENT_CHARSET ai_CharSet,
	const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) 
{
	if (ai_CharSet==CHARSET_A) {
		return IsAEncodable(ai_pcStr, ai_NChar, ai_bExclusive) ;
	} else {
		return IsBEncodable(ai_pcStr, ai_NChar, ai_bExclusive) ;
	} ;
}


size_t ClENG_BarCode128::NextCEncodableBlock(
 const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4int32_t *ao_BeginIndex) 
{
	size_t R ;
	size_t AuxL ;

	if (ai_NChar<0) {
		ai_NChar =  strlen(ai_pcStr) ;
	} ;

	for ( *ao_BeginIndex=0, R=0 ; ai_NChar; ) {
		
		//Salta secuencias SIN digitos
		
		AuxL = strcspn(ai_pcStr, BAR_CODE_DIGITS ) ;
		AuxL = M4_MIN(AuxL, ai_NChar) ;
		*ao_BeginIndex += AuxL ;
		ai_pcStr += AuxL ;
		ai_NChar -= AuxL ;
		if (!ai_NChar) break ;

		//El actual char es digito; ver si hay como minimo n digitos seguidos

		AuxL =  strspn(ai_pcStr, BAR_CODE_DIGITS ) ;
		AuxL = M4_MIN(AuxL, ai_NChar) ;
		if ( AuxL >= 4 ) {
			//Lo encontramos, salir !
			R = AuxL ;
			break ;
		} else {
			//No cumple el minimo, saltar los caracteres y continuar
			*ao_BeginIndex += AuxL ;
			ai_pcStr += AuxL ;
			ai_NChar -= AuxL ;
		} ;
	} ;
	return R ;
}

void ClENG_BarCode128::EncodeAB(ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	m4bool_t SwOk=M4_TRUE ;

	//0, el preferente, 1 el modo auxiliar
	size_t l[2] ;	
	CURRENT_CHARSET mode[2] ; 
	m4uchar_t shift ;
	
	if (ai_NChar<0) {
		ai_NChar =  strlen(ai_RT->m_InputPos) ;
	} ;

	if ( ai_RT->m_DefaultMode==DEFAULT_A ) {
		mode[0]=CHARSET_A ; mode[1]=CHARSET_B ; 
		shift=ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_SHIFT) ;
	} else {
		mode[0]=CHARSET_B ; mode[1]=CHARSET_A ;
		shift=ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_SHIFT) ;
	} ;

	for ( ; ai_NChar ; ) {
		l[0] = IsABEncodable(mode[0],ai_RT->m_InputPos, ai_NChar, M4_FALSE ) ;
		l[1] = IsABEncodable(mode[1],ai_RT->m_InputPos, ai_NChar, M4_TRUE ) ;

		if (l[0]==0 && l[1]==0) {
			//ERROR LOCAL
			*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"BarCode 128 error, char %0:s in string %1:s is not printable in 128 coding", ClMIT_Msg::MIXED_OUT) 
				<< *ai_RT->m_InputPos << ai_RT->m_InputPos << SEND_MSG ;
			SwOk=M4_FALSE ;
			//Copiar char tal cual e ir a siguiente
			ai_RT->m_Out.StrCatChar(*ai_RT->m_InputPos) ;
			++ai_RT->m_InputPos ;
			--ai_NChar ;
			continue ;
		} ;

		//Solo paso a modo auxiliar si hay chars seguidos representables 
		//	exclusivamente en ese modo

		if (l[1]>0) {
			if (l[1]>1) {
				//Mas de 1, modo TO_AUX
				ChangeToCharSet(ai_RT,mode[1]) ; 
			} else {
				//Solo 1, modo SHIFT, no hay cambio real
				ChangeToCharSet(ai_RT,mode[0]) ; //Cambio a preferente por si acaso
				ai_RT->m_Out.StrCatChar( shift ) ; //Inserto shift
			} ;
			EncodeABAux(mode[1],ai_RT,l[1]) ; //Codifico en auxiliar	
			ai_NChar -= l[1] ;

		}  else {

			//Paso los n char al modo preferente

			ChangeToCharSet(ai_RT,mode[0]) ; //Cambio a preferente por si acaso
			EncodeABAux(mode[0],ai_RT,l[0]) ; //Codifico en preferente
			ai_NChar -= l[0] ;
		} ;
	} ;
}

void ClENG_BarCode128::EncodeABAux( 
	CURRENT_CHARSET ai_CharSet,
	ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	if (ai_CharSet==CHARSET_A) {
		EncodeAAux(ai_RT, ai_NChar) ;
	} else {
		EncodeBAux(ai_RT, ai_NChar) ;
	} ;
}

void ClENG_BarCode128::EncodeAAux(ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	m4uchar_t charAux, fromChar ;

	//Ojo, los +fromchar van fuera del GetTranslation porquue la Font almacena el 
	//	comienzo del rango, y no el rangio en si

	for (; ai_NChar; --ai_NChar) {
		fromChar = *ai_RT->m_InputPos ;
		if (fromChar<=31) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_CTRL)+fromChar ;
		} else if (fromChar<=47) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_BLANK)+fromChar-32 ;
		} else if (fromChar<=57) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_DIGIT)+fromChar-48 ;
		} else if (fromChar<=64) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_PUNCT)+fromChar-58 ;
		} else if (fromChar<=90) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_MAY)+fromChar-65 ;
		} else if (fromChar<=95) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_A_PUNCT2)+fromChar-91 ;
		} ;

		ai_RT->m_Out.StrCatChar(charAux) ;
		ai_RT->m_InputPos++ ;
	} ;

}

void ClENG_BarCode128::EncodeBAux(ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	m4uchar_t charAux, fromChar ;

	//Ojo, los +fromchar van fuera del GetTranslation porquue la Font almacena el 
	//	comienzo del rango, y no el rangio en si

	for (; ai_NChar; --ai_NChar) {
		fromChar = *ai_RT->m_InputPos ;
		if (fromChar<=47) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_BLANK)+fromChar-32 ;
		} else if (fromChar<=57) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_DIGIT)+fromChar-48 ;
		} else if (fromChar<=64) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_PUNCT)+fromChar-58 ;
		} else if (fromChar<=90) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_MAY)+fromChar-65 ;
		} else if (fromChar<=95) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_PUNCT2)+fromChar-91 ;
		} else if (fromChar<=96) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_ASC96) ;
		} else if (fromChar<=122) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_MIN)+fromChar-97 ;
		} else if (fromChar<=127) {
			charAux = ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_B_123)+fromChar-123 ;
		} ;
		ai_RT->m_Out.StrCatChar(charAux) ;
		ai_RT->m_InputPos++ ;
	} ;
}


void ClENG_BarCode128::Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) 
{
	m4uint32_t acu ;
	m4int16_t B ;
	ClENG_BarCode128::RT_Data rt ;

	//******Init de estructura RT, a partir de aqui siempre se opera sobre ella

	rt.m_poFont = GetFont() ;
	rt.m_DefaultMode = m_DefaultMode ;
	rt.m_CCh = CHARSET_NO_INIT; 
	rt.m_bSwAllowC = m_bSwAllowC ;
	rt.m_bSwFillZeroC = m_bSwFillZeroC ;
	rt.m_bSwFreMode = M4_FALSE ;
	rt.m_InputPos = ai_pcInValue ; 
	rt.m_Out.StrCpy("") ;

	if ( m_oFormat.StrLenTrim()==0 ) {
		
		//******Proceso sin formato

		EncodeFreeMode(&rt, -1) ;

	} else {

		//******Proceso con formato

		EncodeFormatMode(&rt, m_oFormat.InChar(0) )  ;

	} ;

	//******Añade secuencias de comienzo / final / checksum obligatorios para 128
	//El StartC/A/B YA esta insertado !

	//Quiet zone de comeiznom y final no requerido ??? no forma parte del charset ???

	//Insercion checksum.
	//Calculo modulo 103, no debe incluir el stop

	for (B=0, acu=0; B<rt.m_Out.StrLen(); ++B ) {
		acu += B * (*rt.m_Out.InChar(B)) ;
	} ;
	acu %= 103 ;
	rt.m_Out.StrInsertChar(ClMIT_Str::STR_CAT, 1, (m4uchar_t)acu ) ;

	//Insercion Stop
	rt.m_Out.StrInsertChar(ClMIT_Str::STR_CAT, 1, rt.m_poFont->
		GetTranslation(ENG_TRANS_128_STOP) ) ;


	////******Copia el valor codificado en la salida

	ao_CodedValue->StrCpy(rt.m_Out.InChar(0)) ;

}

void ClENG_BarCode128::EncodeFreeMode(ClENG_BarCode128::RT_Data *ai_RT, size_t ai_NChar) 
{
	size_t lAux ;
	m4int32_t begIdx ;

	if (ai_NChar<0) {
		ai_NChar = strlen(ai_RT->m_InputPos) ;
	} ;

	for ( ; ai_NChar; ) {

		//***Solo si modo C permitido, intentamos pasar a este modo

		if (ai_RT->m_bSwAllowC)  {
		
			lAux = NextCEncodableBlock(ai_RT->m_InputPos, ai_NChar, &begIdx) ;
			if (lAux>0 ) {
				
				//****Hay bloque C

				if (begIdx==0) {
					
					//Comienza en la actual pos, codificar y seguir

					if (!ai_RT->m_bSwFillZeroC && lAux&1 ) {
						//Si no permito 0 de relleno e impar, uno menos
						--lAux ;
					} ;

					EncodeC(ai_RT,lAux);
					ai_NChar -= lAux ;

					continue ;

				} else {
					
					//No comienza en actual , codificar en modo AB desde el actual hasta 
					//	el CBlock y seguir

					lAux = begIdx ;
					EncodeAB(ai_RT, lAux) ;					
					ai_NChar -= lAux ;

					continue ;


				} ;
			} ;
		} ;

		//***No hay bloque C, codificar hasta el final en modo AB

		EncodeAB(ai_RT, -1) ;					
	
	} ;

}

void ClENG_BarCode128::EncodeFormatMode(ClENG_BarCode128::RT_Data *ai_RT, const m4char_t *ai_pcFormat) 
{
	m4bool_t SwOk=M4_TRUE, SwFormatError=M4_FALSE ; ;
	size_t lAux ;
	m4uchar_t charAux ;
	ClMIT_Str strAux ;

	//****Test, formato correcto
	
	//Test caracteres de formateo no permitidos
	lAux = strspn(ai_pcFormat,ENG_128_FORMAT_CHARS) ;
	if (lAux!=strlen(ai_pcFormat)) {
		SwFormatError=M4_TRUE ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Test primer caracter A/B/C, y que no salen mas
	lAux = strspn(ai_pcFormat, ENG_128_FORMAT_FIRST_CHARS) ;
	if (lAux!=1) {
		SwFormatError=M4_TRUE ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	lAux = strspn(&ai_pcFormat[1], ENG_128_FORMAT_FIRST_CHARS) ;
	if (lAux!=0) {
		SwFormatError=M4_TRUE ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	//****Proceso formato


	for (  ; *ai_pcFormat;  ) {
		switch (*ai_pcFormat) {
		
		//----START y TO_CODE

		case 'a':
		case 'A':
			ChangeToCharSet(ai_RT, CHARSET_A) ;
			ai_RT->m_DefaultMode = DEFAULT_A ;
			++ai_pcFormat ;
			break ;
		case 'b':
		case 'B':
			ChangeToCharSet(ai_RT, CHARSET_B) ;
			ai_RT->m_DefaultMode = DEFAULT_B ;
			++ai_pcFormat ;
			break ;
		case 'c':
		case 'C':
			ChangeToCharSet(ai_RT, CHARSET_C) ;
			++ai_pcFormat ;
			break ;
		
		//----Flags algoritmo

		case 'F':
			ai_RT->m_bSwFreMode=M4_TRUE ;
			++ai_pcFormat ;
			break ;
		case 'f':
			ai_RT->m_bSwFreMode=M4_FALSE ;
			++ai_pcFormat ;
			break ;

		case 'Z':
			ai_RT->m_bSwFillZeroC=M4_TRUE ;
			++ai_pcFormat ;
			break ;
		case 'z':
			ai_RT->m_bSwFillZeroC=M4_FALSE ;
			++ai_pcFormat ;
			break ;

		case 'W':
			ai_RT->m_bSwAllowC=M4_TRUE ;
			++ai_pcFormat ;
			break ;
		case 'w':
			ai_RT->m_bSwAllowC=M4_FALSE ;
			++ai_pcFormat ;
			break ;

		//---Insercion chars especiales

		case 'o':
			ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(ENG_TRANS_128_FNC1)) ;
			++ai_pcFormat ;
			break ;

		case 't':
			if ( ai_RT->m_CCh==CHARSET_C ) {
				*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Barcode 128 error aplyng format %0:s, FNC2 not allowed in charset C", 
					ClMIT_Msg::MIXED_OUT) << ai_pcFormat << SEND_MSG ;
				SwOk=M4_FALSE;
			} else {
				ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(
					ai_RT->m_CCh==CHARSET_A ? ENG_TRANS_128_A_FNC2 : ENG_TRANS_128_B_FNC2 )) ;
			} ;
			++ai_pcFormat ;
			break ;

		case 'h':
			if ( ai_RT->m_CCh==CHARSET_C ) {
				*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Barcode 128 error aplyng format %0:s, FNC3 not allowed in charset C", 
					ClMIT_Msg::MIXED_OUT) << ai_pcFormat << SEND_MSG ;
				SwOk=M4_FALSE;
			} else {
				ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(
					ai_RT->m_CCh==CHARSET_A ? ENG_TRANS_128_A_FNC3 : ENG_TRANS_128_B_FNC3 )) ;
			} ;
			++ai_pcFormat ;
			break ;

		case 'r':
			if ( ai_RT->m_CCh==CHARSET_C ) {
				*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Barcode 128 error aplyng format %0:s, FNC4 not allowed in charset C", 
					ClMIT_Msg::MIXED_OUT) << ai_pcFormat << SEND_MSG ;
				SwOk=M4_FALSE;
			} else {
				ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(
					ai_RT->m_CCh==CHARSET_A ? ENG_TRANS_128_A_FNC4 : ENG_TRANS_128_B_FNC4 )) ;
			} ;
			++ai_pcFormat ;
			break ;

		//---Shift

		case 's':
			if ( ai_RT->m_CCh==CHARSET_C ) {
				*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Barcode 128 error aplyng format %0:s, SHIFT not allowed in charset C", 
					ClMIT_Msg::MIXED_OUT) << ai_pcFormat << SEND_MSG ;
				SwOk=M4_FALSE;
			} else {
				if (*ai_RT->m_InputPos==M4_END_STR) {
					//ERROR LOCAL
					*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
						"Barcode 128 error aplyng format %0:s, can not apply shift "
						"because there are not more data", ClMIT_Msg::MIXED_OUT) 
						<< ai_pcFormat 	<< SEND_MSG ;
					SwOk=M4_FALSE;
				} else {
					charAux = *ai_RT->m_InputPos ;		
					++ai_RT->m_InputPos ;
					if (ai_RT->m_CCh==CHARSET_A) {
						ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(
							ENG_TRANS_128_A_SHIFT)) ;
						EncodeBAux(ai_RT, 1) ;
					} else {
						ai_RT->m_Out.StrCatChar(ai_RT->m_poFont->GetTranslation(
							ENG_TRANS_128_B_SHIFT)) ;
						EncodeAAux(ai_RT, 1) ;
					} ;
				} ;
			}  ;
			++ai_pcFormat ;
			break ;

		//---Consumir caracteres

		case '0': case '1': case '2': case '3': case '4': case '5': case '6': 
		case '7': case '8': case '9':
		case '*':
			if (*ai_pcFormat=='*') {
				
				lAux = strlen(ai_RT->m_InputPos) ;
				++ai_pcFormat ;

			} else {
				lAux = strspn(ai_RT->m_InputPos, BAR_CODE_DIGITS) ;
				strAux.ToASCIZ(ai_pcFormat,lAux) ;
				ai_pcFormat += lAux ;
				lAux = atoi(strAux.InChar(0)) ;
			} ;
			
			//Test longitud datos
			if ( lAux > strlen(ai_RT->m_InputPos) ) {
				//ERROR LOCAL
				*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Barcode 128 error aplyng format %0:s, there are not enough input data", ClMIT_Msg::MIXED_OUT) 
					<< ai_pcFormat
					<< SEND_MSG ;
				SwOk=M4_FALSE;
				lAux = strlen(ai_RT->m_InputPos) ;
			} ;

			if (ai_RT->m_bSwFreMode) {
				EncodeFreeMode(ai_RT, lAux) ;
			} else if (ai_RT->m_CCh==CHARSET_C) {
				if (!ai_RT->m_bSwFillZeroC && lAux&1 ) {
					//Si no permito 0 de relleno e impar, uno menos
					--lAux ;
				} ;
				//Si todos no sun numeros ,error
				if ( strspn(ai_RT->m_InputPos, BAR_CODE_DIGITS)!=lAux) {
					*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
						"Barcode 128 error aplyng format %0:s, there are not-digit chars in a mode C block", ClMIT_Msg::MIXED_OUT) 
						<< ai_pcFormat
						<< SEND_MSG ;
					SwOk=M4_FALSE;
				} ;
				EncodeC(ai_RT,lAux) ;
			} else {
				EncodeAB(ai_RT,lAux) ;
			} ;

			break ;
		} ;
	
	} ;

	//****Test, no deben quedar char de entrada por procesar

	if (*ai_RT->m_InputPos!=M4_END_STR) {
		//ERROR LOCAL
		*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Barcode 128 error aplyng format %0:s, there are unprocessed input data %1:s", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcFormat << ai_RT->m_InputPos
			<< SEND_MSG ;
		SwOk=M4_FALSE;
	} ;


exit:
	
	if (SwFormatError) {
		//ERROR LOCAL
		*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Barcode 128 error, syntax error in format %0:s", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcFormat
			<< SEND_MSG ;
	} ;

}


//===================================================================ClENG_PostNet

ClENG_PostNet::ClENG_PostNet()
{
}

void ClENG_PostNet::Init(	
	StENG_FieldEnv * ai_poFieldEnv,
	ClMIT_IdNumDesc *ai_poLocation,
	DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
	DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
	const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
	m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
	m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
	m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
	ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
	m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, 
	m4bool_t ai_bSwClip,
	StMIT_NULLDisplayDef *ai_poNULLDef,
	m4char_t *ai_pcIdPi, 
	
	m4int16_t ai_FixedLength ) 
{
	ClENG_PostNet::End() ;

	ClENG_BarCodeField::Init(
		ai_poFieldEnv, ai_poLocation, ai_Top, ai_Left, ai_Width, ai_Height, 
		ai_pcExpr, ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		ai_AlignX, ai_AlignY, ai_uiIdFGColour, ai_poFont, ai_bSwClip, ai_poNULLDef,
		ai_pcIdPi ) ;

	m_FixedLength = ai_FixedLength ;
}

void ClENG_PostNet::Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) 
{
	m4uint16_t uiLength;
	size_t lAux;
	const m4char_t *sPos;
	m4char_t  *tPos ;
	m4uint32_t acu ;
	m4int16_t digit ;
	m4bool_t SwOk=M4_TRUE ;

	ao_CodedValue->StrCpy("") ;

	uiLength = strlen(ai_pcInValue) ;
	if (m_FixedLength>=0 &&  uiLength!=m_FixedLength ) {
		//ERROR LOCAL
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"A PostNet barcode field has a fixed length of %0:s and the value %1:s doesn't match", ClMIT_Msg::MIXED_OUT) 
			<< m_FixedLength << ai_pcInValue
			<< SEND_MSG ;
	} ;

	//Test todos digitos

	lAux=strspn(ai_pcInValue,BAR_CODE_DIGITS);
	if(lAux!=uiLength){
		//error
		SwOk=M4_FALSE;
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Caracter %0:s no soportado por Codigo de Barras PostNet en valor %1:s", ClMIT_Msg::MIXED_OUT)
				<< ai_pcInValue[lAux] <<  ai_pcInValue << SEND_MSG ;
		goto exit;
	};

	//Inserto en coded value la cadena numerica convertida, y de paso tengo el sumatorio
	ao_CodedValue->Accommodate(uiLength+10) ; //+4 bastaria : Digitos + start + stop + check + STR_END
	for (
		sPos=ai_pcInValue, tPos=ao_CodedValue->InChar(0), acu=0; 
		*sPos; 
		++sPos, ++tPos ) 
	{
		digit = *sPos-'0' ;
		acu += digit ;
		*tPos = GetFont()->GetTranslation(ENG_TRANS_POSTNET_ZERO+digit) ;
	} ;
	*tPos=M4_END_STR ;
	
	//check sum; numero que + al acu, da un multiplo de 10
	digit = 10-(acu%10) ;
	if (digit==10) {
		digit = 0 ;
	} ;
	ao_CodedValue->StrCatChar( GetFont()->GetTranslation(ENG_TRANS_POSTNET_ZERO+digit) ) ;

	//Inserto start / stop

	ao_CodedValue->StrInsertChar( 0, 1, GetFont()->GetTranslation(ENG_TRANS_POSTNET_START) ) ;
	ao_CodedValue->StrCatChar( GetFont()->GetTranslation(ENG_TRANS_POSTNET_STOP) ) ;

exit:
	;

}



