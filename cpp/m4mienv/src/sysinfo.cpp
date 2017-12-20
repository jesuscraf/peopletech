//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sysinfo.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:18/11/97
// Language:         C++
// Operating System: WINDOWS (Usa el registry)
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

#include "mit.h"
#include "clstr.h"
#include "mitdefs.cpp"
#include "sysinfo.h"
#include "regedit.hpp"
#include "m4regdef.hpp"
#include "m4buildversion.hpp"
#include "clfactm4objservice.hpp"
#include "m4unicode.hpp"


#define GROUP_ID MIT_TGI_TEMPORAL

ClMIT_SysInfo::ClMIT_SysInfo()
{
	m_poInsp=GET_INSP();
}

void ClMIT_SysInfo::End()
{
	m_oVList.clear() ;
}

void ClMIT_SysInfo::Init()
{
	ClMIT_SysInfo::End() ;

	m_BinaryStorage=ClMIT_BinaryAdapter::TestEndian() ;
}

m4bool_t ClMIT_SysInfo::PrepareRegistryPath(ClMIT_Str* ao_poBuff)
{
	m4bool_t SwOk=M4_TRUE;
	m4uint32_t iSize=0;
	m4pcchar_t pccSide=NULL;
	m4char_t acPath[ M4CH_MAX_TEMP_BUFFER + 1 ]="";

	if ( ClFactM4ObjService::GetM4ObjConfig() == ClFactM4ObjService::M4ObjServerType ) {
		pccSide = M4_NEW_REGEDIT_KEY_SERVER ;
	} else {
		pccSide = M4_NEW_REGEDIT_KEY_CLIENT ;
	} ;

	if (M4ComposeRegistryPath(pccSide, "TOOLS\\REPORTS", acPath, M4CH_MAX_TEMP_BUFFER, iSize)==M4_SUCCESS) {
		ao_poBuff->StrCpy(acPath) ;
	}
	return SwOk;
}

// Gesti�n concurrente de directorios temporales (Bug 169941)
// Se utiliza un directorio por thread
// Elementos necesarios: Mutex de concurrencia, Mapa de Id Thread con n�mero de directorio
ClCriticalSection ClMIT_SysInfo::s_oMutexMap(M4_TRUE);
ClMIT_SysInfo::M4ThreadDirectoryMap_t ClMIT_SysInfo::s_MapThread;

m4bool_t ClMIT_SysInfo::NewValue( MIT_Id_t ai_KeyId, m4char_t *ai_pcKeyDir, m4char_t *ai_pcKey ) 
{
	m4bool_t SwOk=M4_TRUE ;
	const MIT_Id_t KeyWorkDir = 0x500;
	pair<ValueList_t::iterator,bool> It ;
	ClMIT_Str Str ;
	
	Str.Accommodate(300);

	Str.StrCpy("");
	if ( !GetRegKeyData(ai_pcKeyDir, ai_pcKey, Str) ) {
		SwOk=M4_FALSE ;
	} else {
		// Si estamos en server se comprueba si se est� leyendo la propiedad WorkDir del report
		// para concatenar un directorio asociado al thread (y de esta forma se evitan choques
		// en la generaci�n concurrente de temporales)
		if ((ClFactM4ObjService::GetM4ObjConfig() == ClFactM4ObjService::M4ObjServerType ) &&
			(ai_KeyId == KeyWorkDir) &&
			(strnicmp(ai_pcKey, "WorkDir", 7) == 0) &&
			(Str.StrLen() != 0))
		{
			// Bug 176355
			// Se realiza s�lo la expansi�n de directorio en server s� as� lo indica la propiedad
			// ExpandWorkDir (situada al mismo nivel que WorkDir)
			m4bool_t bExpand = M4_TRUE; // Default value
			ClMIT_Str StrExpand;
			StrExpand.Accommodate(300);
			StrExpand.StrCpy("");
			if (GetRegKeyData(ai_pcKeyDir, "ExpandWorkDir", StrExpand))
			{
				if (StrExpand.StrLen() != 0)
				{
					bExpand = (atoi(StrExpand) == 1 ? M4_TRUE : M4_FALSE);
				}
			}

			if (bExpand == M4_TRUE)
			{
				ClMIT_Str TempDir;
				m4uint_t iThreadNumber;
				m4iden_t IdThread = M4Thread::GetCurrentIdThread();
				// Se va a manipular el mapa => Bloqueamos el Mutex
				s_oMutexMap.Lock();
				M4ThreadDirectoryMap_t::iterator ItMap = s_MapThread.find(IdThread);
				if (ItMap == s_MapThread.end())
				{
					// No est� registrado el Thread. Obtenemos su n�mero correlativo y el nombre del directorio
					iThreadNumber = s_MapThread.size() + 1;
					m4char_t cDir[16];
					sprintf(cDir, "Thread_%d", iThreadNumber);
					TempDir.StrCat(cDir);
					// y se a�ade al mapa
					s_MapThread.insert(M4ThreadDirectoryMap_t::value_type(IdThread, TempDir));
				}
				else
				{
					TempDir = (*ItMap).second;
				}
				// Fin de la manipulaci�n => Desbloqueamos el Mutex
				s_oMutexMap.Unlock();

				// Se concatena al directorio de trabajo el directorio asociado al Thread
				AppendToPath(&Str, TempDir.InChar(0));
			}
		}

		It = m_oVList.insert(ClMIT_SysInfo::ValueList_t::value_type(ai_KeyId,Str)) ;
		if ( !It.second ) SwOk=M4_FALSE ;
	} ;
	Str.OverStringValid() ;

	return SwOk ;
}


m4bool_t  ClMIT_SysInfo::GetValue( MIT_Id_t ai_KeyId, ClMIT_Str *ao_poBuff ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ValueList_t::iterator It ;

	It = m_oVList.find(ai_KeyId) ;
	if ( It==m_oVList.end() ) {
		SwOk=M4_FALSE ;
		if (ao_poBuff) ao_poBuff->StrCpy( "ERROR!!!!" ) ;
	} else {
		if (ao_poBuff) ao_poBuff->StrCpy( (*It).second ) ;
	} ;
	return SwOk ;
}

void ClMIT_SysInfo::AppendToPath( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr ) 
{
	size_t index = ao_poPath->StrLen()-1 ;	
	#ifdef _UNIX
	m4char_t CDir='/' ;
	#else
	m4char_t CDir='\\' ;
	#endif

	if ( *ao_poPath->InChar(index) != CDir ) {
		ao_poPath->StrInsertChar(ClMIT_Str::STR_CAT, 1, CDir) ;
	} ;
	ao_poPath->StrCat(ai_pcStr) ;
}

void ClMIT_SysInfo::AppendToFile( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr, 
		m4bool_t ai_bTestExt ) 
{
	m4bool_t SwCat=M4_TRUE ;
	m4char_t *Pos ;

	if ( ai_bTestExt ) {
		Pos = strrchr( ao_poPath->InChar(0),'.' ) ;
		if (Pos) {
			ao_poPath->StrInsertStr( Pos-ao_poPath->InChar(0), ai_pcStr ) ;
			SwCat=M4_FALSE ;
		} ;
	} ;
	if (SwCat) ao_poPath->StrCat(ai_pcStr) ;

}

void ClMIT_SysInfo::ExtractFromPath( const m4char_t *ai_pcPath, ClMIT_Str *ao_poPath, ClMIT_Str *ao_poName )
{
	m4pcchar_t pcPos = NULL;
	#ifdef _UNIX
	m4char_t cDirSep='/' ;
	#else
	m4char_t cDirSep='\\' ;
	#endif

	pcPos = strrchr( ai_pcPath, cDirSep );
	
	if(pcPos){
		ao_poName->StrCpy( pcPos+1 );
		ao_poPath->ToASCIZ( ai_pcPath, (int)(pcPos - ai_pcPath +1) );
	}else{
		ao_poName->StrCpy(ai_pcPath) ;
		ao_poPath->StrCpy("") ;
	};

}


const m4uint8_t  ClMIT_SysInfo::m_pcANSIToASCII[ClMIT_SysInfo::COUNT_EXTEND_ANSI] = {

	//128 al 145, sin definir

	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,

	// �(146?) (147?) (148?)  �(149)  
		146,	147,	148,	249, 
		
	// 150 a 160 sin definir
	150,151,152,153,154,155,156,157,158,159,160,

	//�(161) �(162) �(163) �(164?) �(165) �(166) �(167) �(168?) �(169?) �(170) 
	173,     155,	156,	164,	157,	124,	15,	168,	169,	166,
	
	//�(171) �(172) �(173) �(174?) �(175?) �(176) �(177) �(178) �(179?) �(180)
	174,	170,	196,	174,	175,	248,	241,	253,	179, 96,

	//�(181) �(182) �(183) �(184?) �(185?) �(186) �(187) �(188) �(189) �(190?) 
	230,	20,		250,	184,	185,	167,  175,	 172,	171,	190,
	
	//�(191) �(192) �(193) �(194) �(195) �(196) �(197) �(198) �(199) �(200)
	168,	 'A',	'A',	131,	'A', 142,   143,	146,	128,	144,

	//�(201) �(202) �(203) �(204) �(205) �(206) �(207) �(208?) �(209) �(210)
	144,	136,	137,	'I',	'I',  140,	139,	208,	165,	'O',

	//�(211) �(212) �(213) �(214) �(215?) �(216) �(217) �(218) �(219)  �(220) 
	'O',	147,	'O',	153,	215,	237,	'U', 'U',	150,	154,
	
	//�(221) �(222?) �(223) �(224) �(225) �(226) �(227) �(228) �(229) �(230)
	'Y',	222,	225,	133,	160,	131,   'a', 132,	134,	145,

	//�(231) �(232) �(233) �(234) �(235) �(236) �(237) �(238) �(239) �(240?) 
	135,	138,	130,	136,	137,  141,	161,	140,	139,	240,

	//�(241) �(242) �(243) �(244) �(245) �(246) �(247) �(248) �(249) �(250)
	164,	149,	162,	147,	'o',  148,	246,	237,	151,	163,
	
	//�(251) �(252) �(253) �(254?) �(255)
	150,	129,	'y',	254,	152
} ;



//!!!!!!!!!!!!!!!!!!!!!!!!!! CUIDADO, VALORES EN OCTAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

const m4uint16_t  ClMIT_SysInfo::m_pcANSIToISO[ClMIT_SysInfo::COUNT_EXTEND_ANSI] = {

	//128 al 145, sin definir

	200,201,202,203,204,205,206,207,210,211,212,213,214,215,216,217,220,221,
	
	// �(146?) (147?) (148?)  �(149)  
	222,		223,	224,	267, 
		
	// 150 a 160 sin definir

	226,227,230,231,232,233,234,235,236,237,240,

	//�(161) �(162) �(163) �(164?) �(165) �(166) �(167) �(168?) �(169?) �(170) 
	241,     242,	243,	244,	245,	246,	247,	250,	251,	252,
	
	//�(171) �(172) �(173) �(174?) �(175?) �(176) �(177) �(178) �(179?) �(180)
	253,	254,	255,	256,	257,	260,	261,	262,	263,	264,

	//�(181) �(182) �(183) �(184?) �(185?) �(186) �(187) �(188) �(189) �(190?) 
	265,	266,		267,	270,	271,	272,  273,	 274,	275,	276,
	
	//�(191) �(192) �(193) �(194) �(195) �(196) �(197) �(198) �(199) �(200)
	277,	 300,	301,	302,	303, 304,   305,	306,	307,	310,

	//�(201) �(202) �(203) �(204) �(205) �(206) �(207) �(208?) �(209) �(210)
	311,	312,	313,	314,	315,  316,	317,	320,	321,	322,

	//�(211) �(212) �(213) �(214) �(215?) �(216) �(217) �(218) �(219)  �(220) 
	323,	324,	325,	326,	327,	330,	331, 332,	333,	334,
	
	//�(221) �(222?) �(223) �(224) �(225) �(226) �(227) �(228) �(229) �(230)
	335,	336,	337,	340,	341,	342,   343, 344,	345,	346,

	//�(231) �(232) �(233) �(234) �(235) �(236) �(237) �(238) �(239) �(240?) 
	347,	350,	351,	352,	353,  354,	355,	356,	357,	360,

	//�(241) �(242) �(243) �(244) �(245) �(246) �(247) �(248) �(249) �(250)
	361,	362,	363,	364,	365,  366,	367,	370,	371,	372,
	
	//�(251) �(252) �(253) �(254?) �(255)
	373,	374,	375,	376,	377
} ;


const m4char_t * ClMIT_SysInfo::m_pcHTML_Escape[ClMIT_SysInfo::COUNT_HTML_ESCAPE] = 
{
	" &nbsp;", ">&gt;", "<&lt;", "&&amp;", "\"&quot;",
	
	"�&aacute;", "�&Aacute;",
	"�&eacute;", "�&Eacute;",
	"�&iacute;", "�&Iacute;",
	"�&oacute;", "�&Oacute;",
	"�&uacute;", "�&Uacute;",
	
	/* Bug 0119866
	Hay que poner la coma
	*/
	"�&ntilde;", "�&Ntilde;",

	"�&agrave;", "�&Agrave;",
	"�&egrave;", "�&Egrave;",
	"�&igrave;", "�&Igrave;",
	"�&ograve;", "�&Ograve;",
	"�&ugrave;", "�&Ugrave;",

	"�&auml;",	"�&Auml;",
	"�&euml;",	"�&Euml;",
	"�&iuml;",	"�&Iuml;",
	"�&ouml;",	"�&Ouml;",
	"�&uuml;",	"�&Uuml;",

	"�&acirc;",	"�&Acirc;",
	"�&ecirc;",	"�&Ecirc;",
	"�&icirc;",	"�&Icirc;",
	"�&ocirc;",	"�&Ocirc;",
	"�&ucirc;",	"�&Ucirc;",

	"�&ccedil;",	"�&Ccedil;",
	
	"�&copy;", "�&reg;",
	
	NULL

} ;


void ClMIT_SysInfo::ANSIToASCII(m4char_t *ai_pcStr, size_t ai_szNChar ) 
{
	for ( ;ai_szNChar; --ai_szNChar, ++ai_pcStr ) {
		if ( *(m4uchar_t *)ai_pcStr >= FIRST_EXTEND_ANSI ) {
			*ai_pcStr =  m_pcANSIToASCII[ *(m4uchar_t *)ai_pcStr-FIRST_EXTEND_ANSI ] ;
		} ;
	} ;
}

void ClMIT_SysInfo::ANSIToASCII(m4char_t *ai_pcStr) 
{
	ANSIToASCII(ai_pcStr,strlen(ai_pcStr)) ;
}


void ClMIT_SysInfo::ANSIToISO(m4char_t *ai_pcStr, size_t ai_szNChar ) 
{
	for ( ;ai_szNChar; --ai_szNChar, ++ai_pcStr ) {
		if ( *(m4uchar_t *)ai_pcStr >= FIRST_EXTEND_ANSI ) {
			*ai_pcStr =  m_pcANSIToISO[ *(m4uchar_t *)ai_pcStr-FIRST_EXTEND_ANSI ] ;
		} ;
	} ;
}

void ClMIT_SysInfo::ANSIToISO(m4char_t *ai_pcStr) 
{
	ANSIToASCII(ai_pcStr,strlen(ai_pcStr)) ;
}

void ClMIT_SysInfo::ConvertToIsoWithEscape(ClMIT_Str* ai_poStr, size_t *ai_szNChar)
{
    size_t szNChar;
	m4char_t pcAux[8];
	m4int16_t i;

	szNChar = *ai_szNChar;

	for ( i=0 ;szNChar; --szNChar, i++ ) {
		if ( *(m4uchar_t *)ai_poStr->InChar(i) >= FIRST_EXTEND_ANSI ) {
			sprintf(pcAux,"\\%3.3i",(int)m_pcANSIToISO[ *(m4uchar_t *)ai_poStr->InChar(i)-FIRST_EXTEND_ANSI ]);
			ClMIT_Str::StrDeleteSegment(ai_poStr->InChar(0), i, 1, ai_poStr->StrLen());
			ai_poStr->StrInsertStr(i,pcAux) ;
			i+=3;
			*ai_szNChar+=3;
		} ;
	} ;
}

void ClMIT_SysInfo::ConvertToEscape(ClMIT_Str *ai_poStr, size_t *ai_szNChar)
{
    size_t szNChar;
	m4char_t pcAux[8];
	m4int16_t i;

	szNChar = *ai_szNChar;

	for ( i=0 ;szNChar; --szNChar, i++ ) {
		if ( *(m4uchar_t *)ai_poStr->InChar(i) >= FIRST_EXTEND_ANSI ) {
			sprintf(pcAux,"\\%3.3o", (int)(*(m4uchar_t *)ai_poStr->InChar(i)) );
			ClMIT_Str::StrDeleteSegment(ai_poStr->InChar(0), i, 1, ai_poStr->StrLen());
			ai_poStr->StrInsertStr(i,pcAux) ;
			i+=3;
			*ai_szNChar+=3;
		} ;
	} ;
}

void ClMIT_SysInfo::EscapeToHTML(m4char_t *ai_pcStr, ClMIT_Str *ao_Str) 
{
	EscapeToHTML(ai_pcStr, strlen(ai_pcStr), ao_Str)  ;
}

// UNICODE REPORTS
void ClMIT_SysInfo::EscapeToHTML(m4char_t *ai_pcStr, size_t ai_szNChar, ClMIT_Str *ao_Str) 
{
	size_t	i = 0;
	m4char_t c = '\0';
	m4pcchar_t pccScaped = NULL;
	m4pcchar_t	pccInString = NULL ;

	pccInString = ai_pcStr;
	for (i = 0; i < ai_szNChar; i++)
	{
		c = pccInString[i];
		pccScaped = M4HTMLScapeChar(c);
		if (pccScaped != NULL)
		{
			ao_Str->StrCat(pccScaped) ;
		}
		else
		{
			ao_Str->StrCatChar(c) ;
		}
	}
}
		
//===========================================================================ClMIT_BinaryAdapter

void ClMIT_BinaryAdapter::Init() 
{
	StMIT_Inspector *ai_poInsp=GET_INSP();

	ClMIT_BinaryAdapter::End() ;

	SetFileStorage( ai_poInsp->m_poSysInfo->GetEndian() ) ;
	SetRAMStorage( ai_poInsp->m_poSysInfo->GetEndian() ) ;
}


m4int16_t ClMIT_BinaryAdapter::EncodeInt( m4int16_t ai_iInt) 
{
	if (m_FileStorage!=m_RAMStorage) {
		Flip(&ai_iInt,sizeof(ai_iInt)) ;
	} ;
	return ai_iInt ;
}

m4int16_t ClMIT_BinaryAdapter::DecodeInt( m4int16_t ai_iInt) 
{
	return EncodeInt(ai_iInt) ;
}

m4int32_t ClMIT_BinaryAdapter::EncodeLong( m4int32_t ai_iLong) 
{
	if (m_FileStorage!=m_RAMStorage) {
		Flip(&ai_iLong,sizeof(ai_iLong)) ;
	} ;
	return ai_iLong ;
}

m4int32_t ClMIT_BinaryAdapter::DecodeLong( m4int32_t ai_iLong) 
{
	return EncodeLong(ai_iLong) ;
}

void ClMIT_BinaryAdapter::Flip(void *ai_Value, size_t ai_szBytes) 
{
	m4int8_t *Beg=(m4int8_t *)ai_Value,*End=&Beg[ai_szBytes-1], Aux ;

	while (End>Beg) {
		Aux=*Beg ;
		*Beg=*End ;
		*End=Aux;
		++Beg ;
		--End ;
	} ;
} 


ClMIT_BinaryAdapter::ENDIAN_ORDER ClMIT_BinaryAdapter::TestEndian() 
{
	union {
		m4uint8_t  Byte[2] ;
		m4uint16_t Int ;
	} ;
	m4uint16_t const TestConst = 0x86CA ;

	Int = TestConst ;
	return (Byte[1]*0x0100+Byte[0])==TestConst ? LITLE_ENDIAN : BIG_ENDIAN  ;
}



