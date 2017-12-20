//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4srvtrace.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/23/97
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
//==============================================================================

#ifdef _UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <io.h>
#include <direct.h>
#endif

#include <stdio.h>

#include "m4define.hpp"
#include "m4types.hpp"

#include "m4srvtrace.h"
#include "m4unicode.hpp"


ClM4_SrvTrace g_oM4SrvTrace ; 

m4bool_t g_bM4SwSrvCoutOn=M4_FALSE ; 
m4bool_t g_bM4SwSrvM4LogOn=M4_TRUE ; 



ClM4_SrvTrace::ClM4_SrvTrace() 
{
	//No hace falta sincronizacion: constructor de un unico objeto global.

	m_oMutex.Init() ;

	m_iIndentCount = 0 ;
	m_iCurrentLevel = 1 ;
	m_poFile=NULL ;
}

ClM4_SrvTrace::~ClM4_SrvTrace() 
{
	//No hace falta sincronizacion: destructor de un unico objeto global.

	if (m_poFile) fclose (m_poFile) ;
	m_poFile=NULL ;
	m_bFlush=M4_FALSE ;
}

m4bool_t ClM4_SrvTrace::SetTraceFile( const m4char_t *ai_pcFile ) 
{
	m4bool_t R ;

	m_oMutex.Lock() ;

	if ( m_poFile ) fclose(m_poFile) ;
	m_poFile=NULL ;
	if (ai_pcFile) {
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		m_poFile=M4Fopen(ai_pcFile, M4UniWrite, eEncoding)  ;
		if (!m_poFile) {
			ClM4_SrvTrace::CreateRecursivePath(ai_pcFile) ;
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniNative ;

			m_poFile=M4Fopen(ai_pcFile, M4UniWrite, eEncoding)  ;
		} ;
	} ;
	R = m_poFile ? M4_TRUE : M4_FALSE ;

	m_oMutex.Unlock() ; 

	return R ;	
}



ClM4_SrvTrace & ClM4_SrvTrace::operator << (M4_SRV_TRACE_MANIP ai_Manip) 
{
	m4char_t *PAux ;

	switch(ai_Manip) {
	
	case  M4_SRV_MSG_B:
		m_oMutex.Lock() ;
		m_iParamCount = 0 ;
		m_MsgType = M4_SRV_TRACE ; 
		m_bSwIndent = M4_FALSE ;
		break ;

	case  M4_SRV_ERROR:
		m_MsgType = M4_SRV_ERROR ; 
		break ;

	case  M4_SRV_WARNING:
		m_MsgType = M4_SRV_WARNING ; 
		break ;

	case  M4_SRV_TRACE:
		m_MsgType = M4_SRV_TRACE ; 
		break ;

	case  M4_SRV_INDENT:
		m_bSwIndent = M4_TRUE ;
		break ;

	case  M4_SRV_MSG_E:
	
		AddIndent( m_bSwIndent ? BLOCK_ENTRY : NO_STYLE ) ;

		if (m_MsgType==M4_SRV_ERROR) {
			SafeStrCat( m_pcStrOut, SIZE_STR_OUT, "ERROR   !!!! " ) ;
		} else if (m_MsgType==M4_SRV_WARNING) {
			SafeStrCat( m_pcStrOut, SIZE_STR_OUT, "WARNING !!!! " ) ;
		} ;

		CatPrintF() ;

		//Limpia los posibles saltos de linea de los mensajes

		for(PAux = m_pcStrOut ; *PAux; ++PAux ) {
			if ( *PAux == '\n' ) *PAux = ' ' ;
		} ;

		WrapOutput(m_pcStrOut, m_bSwIndent ? BLOCK_ENTRY : NO_STYLE  ) ;

		if ( m_bSwIndent ) {
			++m_iIndentCount ;
		} ;

		m_oMutex.Unlock() ; 

		break ;
	} ;

	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4int16_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {
	
		sprintf( m_pcParams[m_iParamCount], "%i", ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4int_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {
	
		sprintf( m_pcParams[m_iParamCount], "%i", ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

#ifdef _WIN64
ClM4_SrvTrace & ClM4_SrvTrace::operator << (uintptr_t ai_Par)
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf(m_pcParams[m_iParamCount], M4_PRIuPTR, ai_Par);	//+ que suficiente para este tipo

		++m_iParamCount;
	};
	return *this;
}
#endif

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4uint_t ai_Par)
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf(m_pcParams[m_iParamCount], "%u", ai_Par);	//+ que suficiente para este tipo

		++m_iParamCount;
	};
	return *this;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4uint16_t ai_Par)
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf( m_pcParams[m_iParamCount], "%u", ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4int32_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf( m_pcParams[m_iParamCount], "%li", ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4uint32_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf( m_pcParams[m_iParamCount], "%lu", ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4int64_t ai_Par)
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf(m_pcParams[m_iParamCount], M4_PRIuPTR, ai_Par);	//+ que suficiente para este tipo

		++m_iParamCount;
	};
	return *this;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4double_t ai_Par)
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf( m_pcParams[m_iParamCount], "%.*f", (int)2, ai_Par ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4bool_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {

		sprintf( m_pcParams[m_iParamCount], "%c", ai_Par ? 'Y' : 'N' ) ;	//+ que suficiente para este tipo

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (m4char_t ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {

		m_pcParams[m_iParamCount][0] =  ai_Par ; 
		m_pcParams[m_iParamCount][1] =  0x0 ; 

		++m_iParamCount ;
	} ;
	return *this ;
}

ClM4_SrvTrace & ClM4_SrvTrace::operator << (const m4char_t *ai_Par) 
{
	if (m_iParamCount<MAX_PARAMS) {

		if (ai_Par) {
			SafeStrCpy( m_pcParams[m_iParamCount], SIZE_PARAM, ai_Par ) ;
		} else {
			SafeStrCpy( m_pcParams[m_iParamCount], SIZE_PARAM, "**NULL**" ) ;
		} ;

		++m_iParamCount ;
	} ;
	return *this ;
}


void ClM4_SrvTrace::EndBlock() 
{
	m_oMutex.Lock() ;

	AddIndent(BLOCK_EXIT) ;
	WrapOutput( m_pcStrOut, BLOCK_EXIT ) ;
	
	m_oMutex.Unlock() ; 
}

void ClM4_SrvTrace::SafeStrCpy( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom ) 
{
	ai_pcTo[ SafeStrNCpy( ai_pcTo, ai_SizeTo, ai_pcFrom, strlen(ai_pcFrom) ) ] = 0x00 ;
}

size_t  ClM4_SrvTrace::SafeStrNCpy( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom, size_t ai_Count ) 
{
	size_t L = strlen(ai_pcFrom), W ;

	if (ai_SizeTo==0) return 0 ;

	L = L < ai_Count ? L : ai_Count ; 

	if ( L>ai_SizeTo-1 ) {
		size_t C ;
		m4char_t *P ;
		W = ai_SizeTo-1 ;
		strncpy(ai_pcTo, ai_pcFrom, W ) ;
		C = W > 3 ? 3 : W  ;
		for (P=&ai_pcTo[W-1] ; C ; --C, --P ) *P = '.' ;
	} else {
		W=L ;
		strncpy(ai_pcTo,ai_pcFrom,W) ;
	} ;
	return W ;
}

void ClM4_SrvTrace::SafeStrCat( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom ) 
{
	size_t L = strlen(ai_pcTo) ;

	SafeStrCpy( &ai_pcTo[L], ai_SizeTo-L, ai_pcFrom ) ;

	return  ;
}

size_t ClM4_SrvTrace::SafeStrNCat( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom, size_t ai_Count ) 
{
	size_t L = strlen(ai_pcTo), R ;

	R = SafeStrNCpy( &ai_pcTo[L], ai_SizeTo-L, ai_pcFrom, ai_Count ) + L ;
	ai_pcTo[R] = 0x00 ;
	return R ;
}

void ClM4_SrvTrace::AddIndent(STYLE ai_Style) 
{
	size_t C = m_iIndentCount*4 ;
	size_t  L = (C < SIZE_STR_OUT-2) ? C : (SIZE_STR_OUT-2) ;

	memset(m_pcStrOut,' ',  L ) ;

	m_pcStrOut[L] = 0x0 ;

	switch (ai_Style) {
	case BLOCK_ENTRY:
		SafeStrCat( m_pcStrOut, SIZE_STR_OUT, ">->->->->-> " ) ;
		break;
	case BLOCK_EXIT:
		SafeStrCat( m_pcStrOut, SIZE_STR_OUT, "<-<-<-<-<-< " ) ; 
		break;
	} ;
	
	SafeStrCat( m_pcStrOut, SIZE_STR_OUT, "[" ) ; 
	SafeStrCat( m_pcStrOut, SIZE_STR_OUT, &m_pcParams[0][0] ) ; 
	SafeStrCat( m_pcStrOut, SIZE_STR_OUT, "] " ) ; 
}


void ClM4_SrvTrace::CatPrintF() 
{
	const m4char_t  *Pos, *Beg, *End ;
	m4int16_t ArgIndex ;

	for (Pos=&m_pcParams[1][0]; *Pos;) {

		if ( ( Beg=strchr(Pos,'%') ) ) { //Hay parametros a traducir
			
			//Copia lo que hay hasta el momento (% excluido)

			SafeStrNCat(m_pcStrOut, SIZE_STR_OUT, Pos,Beg-Pos) ;	
	
			End=strchr(Beg,':') ;			//Ahora a hayar el ordinal de parametro
			if (End) {	
				ArgIndex = atoi(Beg+1) ; //El : para la conversion, es correcto. +1 salta %
				if (*(End+1)) Beg=End+2 ;	//+2 para saltarse el s (d o lo que sea)
				else Beg=End+1 ;
				Pos=Beg ;

				ArgIndex += 2;	//Ajuste: arg 0 y 1 ocupados 

				if ( ArgIndex < m_iParamCount ) {
					SafeStrCat(m_pcStrOut, SIZE_STR_OUT, "<" ) ;	
					SafeStrCat(m_pcStrOut, SIZE_STR_OUT, &m_pcParams[ArgIndex][0] ) ;	
					SafeStrCat(m_pcStrOut, SIZE_STR_OUT, ">" ) ;	
				} else {
					//Error, no hay arg
					SafeStrCat(m_pcStrOut, SIZE_STR_OUT, "<???>") ;	
				} ;

			} else {
				Pos=Beg+1 ;	//Salta el %

				//Si hay problemas, copia un ???. La siguiente pasada copiara lo de demas.
				SafeStrCat(m_pcStrOut, SIZE_STR_OUT, "<???>") ;	
			} ;
			

		} else {

			//No hay mas %

			SafeStrCat( m_pcStrOut, SIZE_STR_OUT, Pos ) ;
			break ;
		} ;
	} ;
}



void ClM4_SrvTraceBlock::End() 
{
	if ( m_iLevel >= 0 && g_bM4SwSrvCoutOn && g_oM4SrvTrace.GetCurrentLevel()>=m_iLevel ) {
		g_oM4SrvTrace.UnIndent() ;
		g_oM4SrvTrace.EndBlock() ;
	} ;
	m_iLevel = -1 ;
}


#define  M4_SRV_TXT_WRAP 80 
#define  M4_SRV_INDENT_AUX_SIZE (M4_SRV_TXT_WRAP-20) 
void ClM4_SrvTrace::WrapOutput(m4char_t *ai_pcStr, STYLE ai_Style)
{
	m4char_t *PosB, *PosAux, SaveChar ;
	size_t SizeSource, IndentCount, FreeSize ;
	m4bool_t SwFirst ;
	m4char_t * Sep= " -_" ;
	m4int16_t m_iWrap=M4_SRV_TXT_WRAP - 5 ;
	m4char_t IndentStr[M4_SRV_INDENT_AUX_SIZE] ;
	m4int16_t BlanksPerIndent=4 ;

	IndentCount = (m_iIndentCount+1)*BlanksPerIndent ;
	IndentCount =  IndentCount>M4_SRV_INDENT_AUX_SIZE-1 ? M4_SRV_INDENT_AUX_SIZE-1 : IndentCount ;

	memset( IndentStr, ' ' , IndentCount ) ;
	IndentStr[IndentCount]=0x0 ;


	if ( ai_Style == BLOCK_ENTRY ) {
		cout << endl ;
		if (m_poFile) fwrite("\n",1,1,m_poFile) ;
	} ;

	//La primera vez ya estan metidos los blancos de indentacion.
	for( PosB=ai_pcStr, SwFirst=M4_TRUE ; *PosB;  ) {
		if ( IndentCount+BlanksPerIndent+1 < m_iWrap ) {
			FreeSize = SwFirst ? m_iWrap : m_iWrap - IndentCount ; 
		} else {
			FreeSize = IndentCount+m_iWrap ; 
		} ;

		//SizeSource tiene los char a imprimir de la cadena original
		SizeSource = strlen(PosB) ;
		SizeSource= SizeSource < FreeSize ? SizeSource : FreeSize ;

		//Test para no partir palabras
		PosAux=PosB+SizeSource-1 ;
		if ( !strchr(Sep,PosAux[1]) && PosAux[1]!=0x0 ) {	
			//Entra si siguiente char al que hay que sacar NO es un separador ni fin de cadena
			for ( ; PosAux > PosB; --PosAux )  {
				if ( strchr(Sep,*PosAux) ) break ;
			} ;
			//PosAux contiene PosB o el char que separa palabra. Solo si es separador podemos 
			//	recortar (si no hay separadores, va toda la palabra).
			if ( strchr(Sep,PosAux[0]) ) {
				SizeSource=PosAux-PosB+1 ;
			} ;
		} ;

		if ( !SwFirst ) {
			//Debe insertar blancos antes, si no blancos ya vienen en la cadena
			cout  << IndentStr ;
			if (m_poFile) fwrite(IndentStr,1,IndentCount,m_poFile) ;
		} ;
		
		SaveChar = PosB[SizeSource] ;
		PosB[SizeSource] = 0x0 ;
		cout << PosB ; 
		if (m_poFile) fwrite(PosB,1,SizeSource,m_poFile) ;
		PosB[SizeSource] = SaveChar ;
		SwFirst=M4_FALSE ;

		PosB+=SizeSource ; //Paso next del bucle. Aqui por el if siguiente

		if ( *PosB ) {
			cout << "..." ;	//Marca de ruptura de linea
			if (m_poFile) fwrite("...",1,3,m_poFile) ;
		} ;

		cout << endl ;
		if (m_poFile) fwrite("\n",1,1,m_poFile) ;

	} ;

	if ( ai_Style == BLOCK_EXIT ) {
		cout << endl ;
		if (m_poFile) fwrite("\n",1,1,m_poFile) ;
	} ;
	
	if (m_poFile && IsFlush() ) fflush(m_poFile) ;
}



#ifdef _UNIX
#define M4MEMDBG_PATH_SEPARATOR '/' 
#else
#define M4MEMDBG_PATH_SEPARATOR '\\'
#endif


void ClM4_SrvTrace::CreateRecursivePath(const m4char_t *ai_pcPath)
{
	ptrdiff_t	iPos;
	ptrdiff_t	iOldPos;
	m4char_t	*pcPos, *buffAux=NULL;
	

	buffAux=new char[strlen(ai_pcPath)+10] ;

	for ( iOldPos=0; M4_TRUE ; ){
		pcPos = strchr((char*)&ai_pcPath[iOldPos], M4MEMDBG_PATH_SEPARATOR);
		if (!pcPos) break ;
		iPos = pcPos - &ai_pcPath[iOldPos] + 1;
		sprintf(buffAux, "%.*s", (int)(iPos+iOldPos), ai_pcPath ) ;
		M4MakeDirectory(buffAux) ;
		iOldPos+=iPos;
	} ;

	if (buffAux) delete buffAux ;
}

