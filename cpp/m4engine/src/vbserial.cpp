//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             vbserial.cpp
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

#include "vbserial.h"

#define GROUP_ID    ENG_TGI_LOADER

//====================================================================ClENG_VB_Serialize

ClENG_VB_Serialize::ClENG_VB_Serialize()
{
	m_poInsp=GET_INSP();
}

void ClENG_VB_Serialize::End()
{
	m_oBuff.End() ;	
}

void ClENG_VB_Serialize::Init() 
{
	ClENG_VB_Serialize::End() ;

	m_oBuff.Init(BUFF_INTERBLOCK) ;
} ;

void ClENG_VB_Serialize::SerializeInt(m4int16_t ai_Par) 
{
	m4char_t Str[20] ;	//De sobra

	if (ai_Par<0)	sprintf(Str,"%0*.*i",5,5,ai_Par) ;	//Signo - consume un char
	else			sprintf(Str,"%0*.*i",6,6,ai_Par) ;
	m_oBuff.AppendStr(Str) ;
}

void ClENG_VB_Serialize::SerializeLong(m4int32_t ai_Par) 
{
	m4char_t Str[20] ;	//De sobra

	if (ai_Par<0)	sprintf(Str,"%0*.*li",11,11,ai_Par) ;	//Signo - consume un char
	else			sprintf(Str,"%0*.*li",12,12,ai_Par) ;
	m_oBuff.AppendStr(Str) ;
}

void ClENG_VB_Serialize::SerializeChar(m4char_t ai_Par) 
{
	m_oBuff.Append( &ai_Par, sizeof(m4char_t));
}


void ClENG_VB_Serialize::SerializeStr(m4char_t *ai_Par) 
{
	SerializeLong(strlen(ai_Par)) ;
	m_oBuff.AppendStr(ai_Par) ;
}

m4bool_t ClENG_VB_Serialize::SaveToStr(m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) 
{
	m4bool_t SwOk=M4_TRUE ;
	M4ClBuffer::StCopy CopyAux ;

	CopyAux.Data =  ao_pcSerialOut ;
	CopyAux.Count = m_oBuff.GetTotal() ;

	if ( CopyAux.Count >=  *aoi_puiSerialOutSize ) {

		/*##Cambiar num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Can not serialize, buffer too small", ClMIT_Msg::MIXED_OUT) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;

	} else {

		ao_pcSerialOut[0] = M4_END_STR ;

		*aoi_puiSerialOutSize = CopyAux.Count ;		
	
		m_oBuff.Flush( M4ClBuffer::BuffToMemManip, &CopyAux );	//Ojo, inutiliza CopyAux
	} ;

	return SwOk ;
}


//====================================================================ClENG_VB_Deserialize

ClENG_VB_Deserialize::ClENG_VB_Deserialize()
{
	m_poInsp=GET_INSP();
}

void ClENG_VB_Deserialize::End()
{
	m_oStr.End() ;	
}

void ClENG_VB_Deserialize::Init(m4char_t *ai_pcSerializeIn) 
{
	ClENG_VB_Deserialize::End() ;

	m_oStr.StrCpy( ai_pcSerializeIn ) ;
	m_pcPos=m_oStr.InChar(0) ;
} ;

m4int16_t ClENG_VB_Deserialize::DeserializeInt() 
{
	m4char_t Str[20] ;	//De sobra

	strncpy(Str,m_pcPos,6) ;
	Str[6]=M4_END_STR ;
	m_pcPos += 6 ;
	return atoi(Str) ;
}

m4int32_t ClENG_VB_Deserialize::DeserializeLong() 
{
	m4char_t Str[20] ;	//De sobra

	strncpy(Str,m_pcPos,12) ;
	Str[12]=M4_END_STR ;
	m_pcPos += 12 ;
	return atol(Str) ;
}

m4char_t  ClENG_VB_Deserialize::DeserializeChar() 
{
	m4char_t R ;
	
	R = *m_pcPos ; ++m_pcPos ;
	return R ;
}


void ClENG_VB_Deserialize::DeserializeStr(ClMIT_Str *ao_poStr) 
{
	m4uint32_t L ;

	L = DeserializeLong() ;

	ao_poStr->Accommodate(L+1) ;
	ao_poStr->ToASCIZ( m_pcPos, L ) ;
	m_pcPos += L ;
}

