//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             PLUGIN.CPP
// Project:			 M4FMT32.DLL	
// Author:           Meta Software M.S. , S.A
// Date:			 06/06/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez 
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines template struct for the Language Plug-Ins
//
//==============================================================================

#include "fmt.h"
#include "fmtdf.cpp"

#include "plugin.hpp"

#include "clargs.h"
#include "clfile.h"
#include "clstr.h"
#include "m4define.hpp"
#include "m4types.hpp"
#include "m4string.hpp"
#include "rctables.h"
#include "regedit.hpp"

#define FROM_MULTITH

#include "usa.hpp"
#include "german.hpp"
#include "french.hpp"
#include "english.h"
#include "castella.hpp"


#define M4_STANDARD_LANG_NUMBER		5

//------------------------ Defines de las listas de palabras ---------------------

#define M4_WORDS_TO_SUBSTITUTE		"Words="
#define M4_PREFIXES					"Prefixes="
#define M4_SEPARATORS				"Separators="
#define M4_NEW_LINE					"\r\n"
#define M4_INI_FILE					"m4fmt32.ini"

//--------------------------------------------------------------------------------

FMT_ClPlugInManager::FMT_ClPlugInManager()
{
	m_StPlugInDataEnv=NULL;
	FMT_ClPlugInManager::End();
};


void FMT_ClPlugInManager::Init()
{
	LoadAllPlugIns();
};

void FMT_ClPlugInManager::End()
{
	m4uint16_t iCount;
	
	//Cierra las DLLs del plug-in abiertas
	if(m_StPlugInDataEnv){
		for (iCount=M4_STANDARD_LANG_NUMBER; iCount<m_oPlugInList.size(); iCount++) {
			if(m_StPlugInDataEnv[iCount].m_bIsLoadedDLL) {
				m_StPlugInDataEnv[iCount].m_oDll.Close();
			};
		};
	};
	
	//Se funde la lista de Plug-Ins
	m_oPlugInList.clear() ;	

	if(m_StPlugInDataEnv){		
		delete []m_StPlugInDataEnv;
	};
};

m4int16_t FMT_ClPlugInManager::LoadAllPlugIns()
{
	m4return_t iError; //M4_ERROR, M4_SUCCESS
	m4char_t pcLanguage[3];
	ClMIT_Args oArgs;
	m4int16_t iCounter ;
	ClMIT_Str FilePathStr;
	StMIT_Inspector* Insp = GET_INSP() ;

	Insp->m_poSysInfo->GetValue(FMT_RK_LANGUAGE_PATH,&FilePathStr);
	oArgs.Init(FilePathStr.InChar(0),ClMIT_Args::NO_SUBARG," \t,-*");

	m_StPlugInDataEnv = new StPlugInData[oArgs.GetArgCount()+M4_STANDARD_LANG_NUMBER];

	//Primero insetamos los Plug-In internos en la lista
	AttachUSALanguage(&(m_StPlugInDataEnv[0].m_StPlugInEnv), pcLanguage);
	LoadLists(&m_StPlugInDataEnv[0], pcLanguage);
	iError = AddPlugIn(pcLanguage,0);

	AttachGERLanguage(&(m_StPlugInDataEnv[1].m_StPlugInEnv), pcLanguage);
	LoadLists(&m_StPlugInDataEnv[1], pcLanguage);
	iError = AddPlugIn(pcLanguage,1);

	AttachFRALanguage(&(m_StPlugInDataEnv[2].m_StPlugInEnv), pcLanguage);
	LoadLists(&m_StPlugInDataEnv[2], pcLanguage);
	iError = AddPlugIn(pcLanguage,2);

	AttachENGLanguage(&(m_StPlugInDataEnv[3].m_StPlugInEnv), pcLanguage);
	LoadLists(&m_StPlugInDataEnv[3], pcLanguage);
	iError = AddPlugIn(pcLanguage,3);

	AttachESPLanguage(&(m_StPlugInDataEnv[4].m_StPlugInEnv), pcLanguage);
	LoadLists(&m_StPlugInDataEnv[4], pcLanguage);
	iError = AddPlugIn(pcLanguage,4);
	//Fin de insercion de los Plug-In internos

	
	//ABRIMOS LAS DLLs INDICADAS EN EL REGISTRY
	for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
		iError = m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_oDll.Open(oArgs.GetArg(iCounter));
		if(iError==M4_ERROR) {
			m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_bIsLoadedDLL = M4_FALSE;
			iError = ERROR_FORMATONOCREADO;
		} else {
			m4typefunreturn  ao_AddressAux;
			AttachLanguage_t ao_NewLanguage ;
			iError = m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_oDll.GetFunc("AttachNewLanguage", ao_AddressAux);
			if(iError==M4_ERROR){
				//Tratar el error. Cerramos la DLL
				m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_oDll.Close();
				m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_bIsLoadedDLL = M4_FALSE;
				iError = ERROR_INVALIDHANDLE;
			} else {
				m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_bIsLoadedDLL = M4_TRUE;
				ao_NewLanguage = (AttachLanguage_t)ao_AddressAux ;
				(*ao_NewLanguage)(&(m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER].m_StPlugInEnv), pcLanguage);
				LoadLists(&m_StPlugInDataEnv[iCounter+M4_STANDARD_LANG_NUMBER], pcLanguage);
				iError = AddPlugIn(pcLanguage,iCounter+M4_STANDARD_LANG_NUMBER);			
			};
		};
	};

	return iError;
}


m4int16_t FMT_ClPlugInManager::LoadLists(StPlugInData* ai_stDataInfo, m4char_t* ai_pcLanguage)
{
	m4return_t iError=EXITO;
	ClMIT_File oFile;
	ClMIT_Str  oStrAux, oStrAux2, FilePathStr;
	ClMIT_Args oArgs;
	m4char_t *Pos, *End, AuxBuff[2];
	m4int16_t iCounter;
	StMIT_Inspector* Insp = GET_INSP() ;

	Insp->m_poSysInfo->GetValue(FMT_RK_PARAMS_DIR,&FilePathStr);
	Insp->m_poSysInfo->AppendToPath(&FilePathStr,M4_INI_FILE);

	oStrAux.StrCpy("[");
	oStrAux.StrCat(ai_pcLanguage);
	oStrAux.StrCat("]");

	if(oFile.Open( FilePathStr.InChar(0), ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::SILENT_TRY_OPEN, M4_FALSE ) ){
 		m4char_t *upcBuffer = new m4char_t [oFile.GetSize()+1];
		
		oFile.R(upcBuffer, oFile.GetSize());
		upcBuffer[oFile.GetSize()] = M4_END_STR;

		//Busco bloque de idioma
		Pos = strstr(upcBuffer,oStrAux.InChar(0));

		if(Pos){
			End = strstr(Pos+1,"[");
			if(End){
				oStrAux.StrNCpy(Pos, End-Pos);
			} else {
				oStrAux.StrCpy(Pos);
			};
		} else {
			goto exit;
		};

		//Busco primera entrada: sFWordsToSubstitute
		Pos=strstr(oStrAux.InChar(0),M4_WORDS_TO_SUBSTITUTE);
		End=strstr(Pos,M4_NEW_LINE);
		oStrAux2.ToASCIZ(Pos+strlen(M4_WORDS_TO_SUBSTITUTE),End-Pos-strlen(M4_WORDS_TO_SUBSTITUTE));
		oArgs.Init(oStrAux2.InChar(0),ClMIT_Args::NO_SUBARG, ", ");

		for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
			oStrAux2.ToASCIZ(oArgs.GetArg(iCounter),strlen(oArgs.GetArg(iCounter)));
			ai_stDataInfo->WordList_t.insert(StPlugInData::List_t::value_type(oStrAux2,2));
		};
		//Fin primera entrada

		//Busco segunda entrada: sFPrefixes
		Pos=strstr(oStrAux.InChar(0),M4_PREFIXES);
		End=strstr(Pos,M4_NEW_LINE);
		oStrAux2.ToASCIZ(Pos+strlen(M4_PREFIXES),End-Pos-strlen(M4_PREFIXES));
		oArgs.Init(oStrAux2.InChar(0),ClMIT_Args::NO_SUBARG, ", ");

		for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
			oStrAux2.ToASCIZ(oArgs.GetArg(iCounter),strlen(oArgs.GetArg(iCounter)));
			ai_stDataInfo->PrefixList_t.insert(StPlugInData::List_t::value_type(oStrAux2,2));
		};
		//Fin segunda entrada

		//Busco tercera entrada: sFSeparators
		Pos=strstr(oStrAux.InChar(0),M4_SEPARATORS);
		End=strstr(Pos,M4_NEW_LINE);
		if(!End) End=Pos+strlen(Pos);
		oStrAux2.ToASCIZ(Pos+strlen(M4_SEPARATORS),End-Pos-strlen(M4_SEPARATORS));
		oArgs.Init(oStrAux2.InChar(0),ClMIT_Args::NO_SUBARG, ", ");

		for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
			sprintf(AuxBuff,"%c",atol(oArgs.GetArg(iCounter)));
			oStrAux2.ToASCIZ(AuxBuff,strlen(AuxBuff));
			ai_stDataInfo->SepList_t.insert(StPlugInData::List_t::value_type(oStrAux2,2));
		};
		//Fin tercera entrada
	};

exit:

	return iError;
}

m4int16_t FMT_ClPlugInManager::AddPlugIn(m4char_t* ai_pcLanguage, m4uint16_t ai_uiNumber)
{
	m4uint16_t Key ;
	KeyList_t::iterator FoundIt ;
	
	Key=ai_pcLanguage[0]*0x100+ai_pcLanguage[1];
	FoundIt = m_oPlugInList.find(Key);
	if (FoundIt == m_oPlugInList.end() ) {
		m_oPlugInList.insert( KeyList_t::value_type(Key, &m_StPlugInDataEnv[ai_uiNumber]) );
		return EXITO;
	} else {
		return ERROR_FORMATONOCREADO;
	};
}

m4int16_t FMT_ClPlugInManager::GetPlugInData(m4uint32_t ai_uiIdiomaDestino, StPlugInData** ao_StPlugInDataInfo)
{
	m4uint16_t Key ;
	KeyList_t::iterator FoundIt ;

	Key=ai_uiIdiomaDestino ;

	FoundIt=m_oPlugInList.find(Key);
	if (FoundIt == m_oPlugInList.end() ) 
	{	 
		//Not Found
		return ERROR_IDIOMANOSOPORTADO;
	} else {
		*ao_StPlugInDataInfo = (*FoundIt).second;
		return EXITO;	
	};
};