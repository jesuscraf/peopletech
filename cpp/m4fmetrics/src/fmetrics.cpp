//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             fmetrics.cpp
// Project:			 fmetrics.dll
// Author:           Meta Software M.S. , S.A
// Date:			 13/05/1999
// Language:         C++
// Operating System: WINDOWS. 
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	Who && Date
//
// Definition:
//
//==============================================================================


#include "fontm.h"
#define _INCLUDING_FROM_FONT_M_
#include "fontmdf.cpp"
#include "fmetrics.h"
#include "m4var.hpp"



m4int16_t WINAPI FONT_M_StartSession(m4uint32_t* ao_TaskId)
{
	m4bool_t SwOk;
	m4uint32_t OkCount;
	MIT_Id_t NewTask ; 

	SPAWN_IN(NewTask,g_FONT_M_pstPlugIn) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	OkCount = Insp->m_poTrace->GetOkCount();

	//Comprobacion del path del registry
	ClMIT_Str RFNStr;
	Insp->m_poSysInfo->GetValue(FONT_M_RK_RFN_PATH,&RFNStr);
	if(!strcmp("ERROR!!!!",RFNStr.InChar(0))){
		SwOk=M4_FALSE;
	    goto exit;
	};

	//Creacion de la Base de Datos de Fuentes
	M4_NEW_ITEM( Insp->m_poMemProf, FONT_M_MGI_FONT, TaskEnv->m_poDBFont, ClFONT_M_DBFont ) ;
	TaskEnv->m_poDBFont->Init();

	*ao_TaskId = NewTask;
	SwOk = (OkCount!=Insp->m_poTrace->GetOkCount())? M4_FALSE : M4_TRUE;
	
exit: 

	;
	SPAWN_OUT() ;

	return SwOk;
} 


m4return_t StartSessionLN4(
	m4VariantType * ai_poParam, m4int32_t ai_iNParam, 
	m4VariantType * ao_poReturn, void * ai_poDataContext)
{
	m4int16_t R;
	m4uint32_t  ao_TaskId ;

	// necesitamos siempre un argumento por referencia
	if (ai_iNParam < 1) {
		return M4_ERROR;
	}

	R=FONT_M_StartSession(&ao_TaskId) ;
			
	// argumento por referencia
	ai_poParam[0].Type = M4CL_CPP_TYPE_NUMBER;
	ai_poParam[0].Data.DoubleData = ao_TaskId ;	

	// valor de salida
	ao_poReturn->Type = M4CL_CPP_TYPE_NUMBER;
	ao_poReturn->Data.DoubleData = R ;	
	
	return M4_SUCCESS;
}
	


m4int16_t WINAPI FONT_M_KillSession(m4uint32_t ai_TaskId)
{
	m4bool_t SwOk;
	m4uint32_t OkCount;
	KILL_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	OkCount = Insp->m_poTrace->GetOkCount();

	TaskEnv->m_poDBFont->End();
	M4_DELETE_ITEM( Insp->m_poMemProf, FONT_M_MGI_FONT, TaskEnv->m_poDBFont, ClFONT_M_DBFont ) ;

	SwOk = (OkCount!=Insp->m_poTrace->GetOkCount())? M4_FALSE : M4_TRUE;

	KILL_OUT(ai_TaskId) ;

	return SwOk;
} 


m4return_t KillSessionLN4(
	m4VariantType * ai_poParam, m4int32_t ai_iNParam, 
	m4VariantType * ao_poReturn, void * ai_poDataContext) 
{

	m4int16_t R;

	if (ai_iNParam < 1) {
		return M4_ERROR;
	}

	R=FONT_M_KillSession(ai_poParam[0].Data.DoubleData) ;

	// valor de salida
	ao_poReturn->Type = M4CL_CPP_TYPE_NUMBER;
	ao_poReturn->Data.DoubleData = R ;	
	
	return M4_SUCCESS;


}


m4int16_t WINAPI FONT_M_AddFont(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic)
{
	m4bool_t SwOk=M4_TRUE;
	SWAP_IN(ai_TaskId);
	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;


	ClMIT_IdNumDesc IdDesc;
	ClMIT_Str RFNStr;
	ClFONT_M_Font *F ;

	IdDesc.Init(ai_uiFontId, ai_pcFontName);

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		if(!ai_bIsAbsolute){
			Insp->m_poSysInfo->GetValue(FONT_M_RK_RFN_PATH,&RFNStr);
			Insp->m_poSysInfo->AppendToPath(&RFNStr,ai_pcRFNFileName);
			SwOk = TaskEnv->m_poDBFont->SaveFont(ClFONT_M_Font::RVW_INFO,&IdDesc,RFNStr.InChar(0),
											 ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic,ClMIT_Trace::PROPAGATE_ERROR);
		} else {
			SwOk = TaskEnv->m_poDBFont->SaveFont(ClFONT_M_Font::RVW_INFO,&IdDesc,ai_pcRFNFileName,
											 ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic,ClMIT_Trace::PROPAGATE_ERROR);
		};
	} else {
		SwOk=M4_FALSE ;
	} ;
exit:

	;

	SWAP_OUT() ;
	return SwOk;
}

m4int16_t WINAPI FONT_M_WindowsMetricsById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, m4char_t *ao_pcFaceName,
											     m4int16_t * ao_NameSize, m4int32_t* ao_WindowsHeight, m4int32_t *ao_MaxDesignHeight, 
												 m4int32_t* ao_MaxDesignWidth)
{
	m4uint32_t OkCount;
	m4bool_t SwOk;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	OkCount = Insp->m_poTrace->GetOkCount();	

	if(*ao_NameSize==0) {
		SwOk=M4_FALSE;
		goto exit;
	};

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if( F ){
		*ao_MaxDesignWidth = F->GetDvcDesignMaxWidthChar();
		ClMIT_Str::SafeStrCpy(ao_pcFaceName,*ao_NameSize, F->RVW_GetFaceName());
		*ao_NameSize = strlen(ao_pcFaceName) ;
		*ao_WindowsHeight = F->RVW_GetHeight();
		*ao_MaxDesignHeight = F->GetDvcDesignHeight();
	};

exit:
	SwOk = (OkCount!=Insp->m_poTrace->GetOkCount())? M4_FALSE : M4_TRUE;
	
	SWAP_OUT() ;
	return SwOk;

};

m4int16_t WINAPI FONT_M_WindowsMetrics(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
										 const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									     m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic,
										 m4char_t *ao_pcFaceName, m4int16_t * ao_NameSize,m4int32_t* ao_WindowsHeight, 
										 m4int32_t *ao_MaxDesignHeight, m4int32_t* ao_MaxDesignWidth)
{
	m4uint32_t OkCount, CountAfter;
	m4bool_t SwOk=M4_TRUE;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	OkCount = Insp->m_poTrace->GetOkCount();

	if(*ao_NameSize==0) {
		SwOk=M4_FALSE;
		goto exit;
	};

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;
	if(SwOk && F ) {
			*ao_MaxDesignWidth = F->GetDvcDesignMaxWidthChar();
			ClMIT_Str::SafeStrCpy(ao_pcFaceName,*ao_NameSize,F->RVW_GetFaceName());
			*ao_NameSize = strlen(ao_pcFaceName) ;
			*ao_WindowsHeight = F->RVW_GetHeight();
			*ao_MaxDesignHeight = F->GetDvcDesignHeight();
	};

exit:
	CountAfter = Insp->m_poTrace->GetOkCount();
	SwOk = (OkCount!=CountAfter)? M4_FALSE : M4_TRUE;
	
	SWAP_OUT() ;
	return SwOk; 
}


m4return_t WindowsMetricsLN4(
	m4VariantType * ai_poParam, m4int32_t ai_iNParam, 
	m4VariantType * ao_poReturn, void * ai_poDataContext) 
{

	m4int16_t R;

	// fix bug 0096471 && 0098405.
	m4int16_t aio_NameSize = 256;
	m4char_t *ao_pcFaceName = new m4char_t[aio_NameSize];
    *ao_pcFaceName = '\0';  // hay que inicializar porque sino la función StrSafeCopy = core.

	m4int32_t ao_WindowsHeight;
	m4int32_t ao_MaxDesignHeight;
	m4int32_t ao_MaxDesignWidth;

	// comprobamos número de argumentos
	if (ai_iNParam < 15) {
		return M4_ERROR;
	}

	// comprueba tipos
	if (ai_poParam[0].Type != M4CL_CPP_TYPE_NUMBER		|| 
		ai_poParam[1].Type != M4CL_CPP_TYPE_NUMBER		|| 
		ai_poParam[2].Type != M4CL_CPP_TYPE_STRING_VAR	|| 
		ai_poParam[3].Type != M4CL_CPP_TYPE_STRING_VAR	|| 
		ai_poParam[4].Type != M4CL_CPP_TYPE_NUMBER		||  
		ai_poParam[5].Type != M4CL_CPP_TYPE_NUMBER		||  
		ai_poParam[6].Type != M4CL_CPP_TYPE_NUMBER		||  
		ai_poParam[7].Type != M4CL_CPP_TYPE_NUMBER		||  
		ai_poParam[8].Type != M4CL_CPP_TYPE_NUMBER		||  
		ai_poParam[9].Type != M4CL_CPP_TYPE_NUMBER )
	{
		return M4_ERROR;
	}

	
	R=FONT_M_WindowsMetrics(ai_poParam[0].Data.DoubleData, ai_poParam[1].Data.DoubleData, ai_poParam[2].Data.PointerChar,
										 ai_poParam[3].Data.PointerChar, ai_poParam[4].Data.DoubleData, ai_poParam[5].Data.DoubleData, 
									     ai_poParam[6].Data.DoubleData, ai_poParam[7].Data.DoubleData, ai_poParam[8].Data.DoubleData, ai_poParam[9].Data.DoubleData,
										 ao_pcFaceName, &aio_NameSize,&ao_WindowsHeight, 
										 &ao_MaxDesignHeight, &ao_MaxDesignWidth) ;

	
	
	// establecer parámetros de salida
	ai_poParam[10].Set(ao_pcFaceName,M4_TRUE);

	// avoids memory leak
	delete [] ao_pcFaceName;
	ao_pcFaceName = NULL;
	
	ai_poParam[11].Type = M4CL_CPP_TYPE_NUMBER;
	ai_poParam[11].Data.DoubleData = aio_NameSize ;	

	ai_poParam[12].Type = M4CL_CPP_TYPE_NUMBER;
	ai_poParam[12].Data.DoubleData = ao_WindowsHeight ;	

	ai_poParam[13].Type = M4CL_CPP_TYPE_NUMBER;
	ai_poParam[13].Data.DoubleData = ao_MaxDesignHeight ;	

	ai_poParam[14].Type = M4CL_CPP_TYPE_NUMBER;
	ai_poParam[14].Data.DoubleData = ao_MaxDesignWidth ;	


	// valor de salida
	ao_poReturn->Type = M4CL_CPP_TYPE_NUMBER;
	ao_poReturn->Data.DoubleData = R ;	
	
	return M4_SUCCESS;
}


m4int32_t WINAPI FONT_M_DesignMaxWidthById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId)
{
	DvcUnit_t DesignMaxWidth;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;

	if( F ) {
		DesignMaxWidth = F->GetDvcDesignMaxWidthChar();
	};

exit:
	;
	SWAP_OUT() ;
	return DesignMaxWidth;
}

m4int32_t WINAPI FONT_M_DesignMaxWidth(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
											  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
											  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic) 
{
	DvcUnit_t DesignMaxWidth = 0;
	m4bool_t SwOk=M4_TRUE;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;

	if(SwOk && F ) {
		DesignMaxWidth = F->GetDvcDesignMaxWidthChar();
	};

exit:
	;
	SWAP_OUT() ;
	return DesignMaxWidth;
}

m4int32_t WINAPI FONT_M_MaxWidthById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId)
{
	DvcUnit_t MaxWidth;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if( F ) {
		MaxWidth = F->GetDvcMaxWidthChar();
	};

exit:
	;
	SWAP_OUT() ;
	return MaxWidth;
}

m4int32_t WINAPI FONT_M_MaxWidth(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									    const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
										m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic) 
{
	DvcUnit_t MaxWidth = 0;
	m4bool_t SwOk=M4_TRUE;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;

	if(SwOk && F ) {
		MaxWidth = F->GetDvcMaxWidthChar();
	};

exit:
	;
	SWAP_OUT() ;
	return MaxWidth;
}

m4int32_t WINAPI FONT_M_DesignHeightById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId)
{
	DvcUnit_t DesignHeight = 0;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;
	
	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;

	if(F) {
		DesignHeight = F->GetDvcDesignHeight();
	};
	
exit:
	;
	SWAP_OUT() ;
	return DesignHeight;
}

m4int32_t WINAPI FONT_M_DesignHeight(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
											const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
											m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic) 
{
	m4bool_t SwOk=M4_TRUE;
	DvcUnit_t DesignHeight;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;
	
	if (SwOk && F ) {
		DesignHeight = F->GetDvcDesignHeight();
	};

exit:
	;
	SWAP_OUT() ;
	return DesignHeight;
}


m4int32_t WINAPI FONT_M_HeightById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId)
{
	DvcUnit_t Height=0;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;

	if(F) {
		Height = F->GetDvcHeight();
	};

exit:
	;
	SWAP_OUT() ;
	return Height;
}

m4int32_t WINAPI FONT_M_Height(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic) 
{
	m4bool_t SwOk=M4_TRUE;
	DvcUnit_t Height;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;

	if(SwOk && F){
		Height = F->GetDvcHeight();
	};

exit:
	;
	SWAP_OUT() ;
	return Height;
}


m4int32_t WINAPI FONT_M_WidthCharById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, m4char_t ai_Char)
{
	DvcUnit_t WidthChar;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if(F) {
		WidthChar = F->GetDvcWidthChar(ai_Char);
	};

exit:
	;
	SWAP_OUT() ;
	return WidthChar;
}

m4int32_t WINAPI FONT_M_WidthChar(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
										 const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
										 m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic,
										 m4char_t ai_Char) 
{
	m4bool_t SwOk=M4_TRUE;
	DvcUnit_t WidthChar;
	SWAP_IN(ai_TaskId) ;

	StFONT_M_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClFONT_M_Font *F ;

	F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	if ( !F ) {
		SwOk = M4_BOOL( FONT_M_AddFont(ai_TaskId,ai_uiFontId,ai_pcFontName,ai_pcRFNFileName,ai_bIsAbsolute,ai_MapHeight,ai_bFixedSpace,ai_bUnderline,ai_bBold,ai_bItalic));
		F = TaskEnv->m_poDBFont->GetAddressFontById(ai_uiFontId, M4_FALSE) ;
	} ;

	if(SwOk && F) {
		WidthChar = F->GetDvcWidthChar(ai_Char);
	};

exit:
	;
	SWAP_OUT() ;
	return WidthChar;
}




