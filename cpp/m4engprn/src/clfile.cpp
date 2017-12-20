//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clfile.cpp
// Project:			 enginedll.dll
// Author:           Meta Software M.S. , S.A
// Date:			 7/28/97
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

#ifdef _UNIX
#include <unistd.h>
#else
#include <io.h>
#endif

#include "m4string.hpp"

#include "clfile.h"
#include "m4unicode.hpp"


M4ClFile::M4ClFile() 
{
	m_pstF=NULL;
	m_StPath.Str=NULL ;
	//m_poEnv=g_oTManager.GetEnv();
}

//NOTA: Open NO debe contabilizar ninguna alocacion en Trace que se desaloque en Close, 
//	porque los test de salida en Trace deben ejecutarse ANTES del Close(), y se provocaria
//	un error.

m4bool_t M4ClFile::Open( const m4char_t *ai_pcPath, MODE_EXIST ai_Exist, MODE_WRITE ai_Write ) 
{
	char *Mode ;
	int iLength = -1 ;
	char* pcFileName = NULL ;
	m4bool_t Return=M4_TRUE ;
	
	if (IsOpen()) {
		//Ya esta abierto
	} ;

	if ( (ai_Exist == EXIST &&  ai_Write==WRITE) || (ai_Exist == NEW &&  ai_Write==READ) 
		|| (ai_Exist == OVERWRITE &&  ai_Write==READ) ) 
	{
		Return=M4_FALSE ;
		goto exit;
	} ;
	if ( ai_Exist == EXIST ) {
		if ( !Exist(ai_pcPath) )  {
			Return=M4_FALSE ;
			goto exit;
		} ;
		if (ai_Write == READ ) {
			Mode="rb";
		} else if (ai_Write == READ_WRITE ) {
			Mode="rb+";
		} ;
	} else {
		if ( ai_Exist == NEW ) {
			if ( Exist(ai_pcPath) )  {
				Return=M4_FALSE ;
				goto exit;
			} ;
		} ;
		if (ai_Write == WRITE ) {
			Mode="wb";
		} else if (ai_Write == READ_WRITE ) {
			Mode="wb+";
		} ;
	} ;

	// UNICODE FILE
	iLength = -1 ;
	pcFileName = M4CppToANSI( ai_pcPath, iLength ) ;
	m_pstF=fopen(pcFileName, Mode);
	delete [] pcFileName ;
	
	if (! IsOpen() ) {
		Return=M4_FALSE ;
		goto exit;
	} ;

exit:
	if (Return) {
		m_StPath.Size = strlen(ai_pcPath)+1 ;
		//M4_NEW_ARRAY(g_oMIMemProf, MEM_GRP_OTHERS, 
		//	m_StPath.Str, m4char_t, m_StPath.Size
		//) ;
		m_StPath.Str = new m4char_t[m_StPath.Size] ;
		strcpy(m_StPath.Str, ai_pcPath) ;		
	} ;

	
	return Return ;		
}

void M4ClFile::Close() {
	if (IsOpen()) {
		fclose(m_pstF);
		m_pstF=NULL;
	} ;
	//M4_DELETE_ARRAY(g_oMIMemProf, MEM_GRP_OTHERS, 
	//	m_StPath.Str, m4char_t, m_StPath.Size
	//) ;
	delete m_StPath.Str ; m_StPath.Str=NULL ;
}

void M4ClFile::W( const void * ai_pvBuff, size_t ai_szSize, size_t ai_szNItems ) 
{
	if ( fwrite( ai_pvBuff, ai_szSize, ai_szNItems, m_pstF ) != ai_szNItems ) {
		
		//Efecto local
	} ;
}

void M4ClFile::WStr( const m4char_t * ai_pcStr ) 
{
	W(ai_pcStr, strlen(ai_pcStr)) ;
}

size_t M4ClFile::R( void * ai_pvBuff, size_t ai_szSize, size_t ai_szNItems ) 
{
	return fread(ai_pvBuff,ai_szSize,ai_szNItems,m_pstF);
}

m4uint32_t M4ClFile::GetPos() 
{
	return ftell(m_pstF) ;
}

void M4ClFile::SetPos( m4uint32_t ai_uiOff ) 
{
	fseek(m_pstF, ai_uiOff, SEEK_SET ) ;

}

m4bool_t M4ClFile::IsEOF() 
{
	return (feof(m_pstF)!=M4_FALSE) ? M4_TRUE : M4_FALSE ;
}


m4bool_t M4ClFile::Exist(const m4char_t *ai_pcStr) 
{
	#ifdef _UNIX
	return (access(ai_pcStr,0) == M4_FALSE) ? M4_TRUE : M4_FALSE ;
	#else
	return _access(ai_pcStr,0) == 0 ;
	#endif
}

m4bool_t M4ClFile::Remove(const m4char_t *ai_pcStr) 
{
	m4bool_t R ;

	R= ( remove(ai_pcStr) == M4_FALSE ) ? M4_TRUE : M4_FALSE;

	if ( ! R ) {
		
		//Efecto local
	} ;
	return R ;
}

void M4ClFile::Flush() 
{
	fflush(m_pstF) ;
}

m4uint32_t M4ClFile::GetSize() 
{
	m4uint32_t Pos, Size ;

	Pos=GetPos() ;
	fseek(m_pstF, 0, SEEK_END ) ;
	Size=GetPos()+1 ;
	SetPos(Pos) ;
	return Size ;
}


