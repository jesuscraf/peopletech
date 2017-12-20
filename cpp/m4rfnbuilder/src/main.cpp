//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             main.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
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

#include "rfn.h"

#define _INCLUDING_FROM_RFN_
#include "rfndf.inl"

#include "rfntrans.h"
#include "rfnfiler.h"
#include "rfnfilew.h"

#define GROUP_ID RFN_TGI_OTHERS


void main(m4int16_t ai_iArgC, m4char_t **ai_pcArgV) 
{
	MIT_Id_t NewTask ;
	m4bool_t SwOk=M4_TRUE ;
	m4char_t *SintaxMsg= 
		"\nSintax is \n"
		"RFNBuilder -B FilePAR DriverPCL DriverPSC DriverRVW DriverESCP. To Build a RFN file\n"
		"RFNBuilder -I FileRFN FileTrace. To generate a information file about the RFN file\n"
		"RFNBuilder -U FilePRN FileUSP CharSetOffset. To build a USP file\n" 
		"RFNBuilder -P FilePRN FilePFA. To build a PFA file\n"
		"RFNBuilder -T FileTTF FontName. To convert a TTF file to a PFA file\n"
		;

	//==============================================>Fase de ...
	SPAWN_IN(NewTask,g_RFN_pstPlugIn) ;

	StRFN_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	SPAWN_OUT() ;
	//<==============================================SPAWN


	//==============================================>Fase de ...

	SWAP_IN(NewTask) ;

	ClRFN_ParsePAR Parser ;
	ClRFN_RFN_R RFNList ;

	if ( ai_iArgC < 2  ) {
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
		printf( SintaxMsg ) ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( stricmp(ai_pcArgV[1], "-B")==0 ) { 

		if (  ai_iArgC != 7 ) {

			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
			printf( SintaxMsg ) ;
			SwOk=M4_FALSE ;
	
		} else  {

			Parser.Init() ;
			if ( ! Parser.ParsePAR(ai_pcArgV[2],ai_pcArgV[3],ai_pcArgV[6],ai_pcArgV[5],ai_pcArgV[3]) ) {
				SwOk=M4_FALSE ;
			} ;
		} ;

	} else if ( stricmp(ai_pcArgV[1], "-I")==0 ) { 

		if (  ai_iArgC != 4 ) {

			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
			printf( SintaxMsg ) ;
			SwOk=M4_FALSE ;
	
		} else  {

			if ( ! RFNList.List(ai_pcArgV[2],ai_pcArgV[3]) ) {
				SwOk=M4_FALSE ;
				goto exit ;
			};
		} ;
		
	} else if ( stricmp(ai_pcArgV[1], "-U")==0 ) { 

		m4int16_t FCount ;

		if (  ai_iArgC != 5 ) {

			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
			printf( SintaxMsg ) ;
			SwOk=M4_FALSE ;
	
		} else  {

			if ( ! ClRFN_RFN_W::ExtractFromPRN_To_USP(ai_pcArgV[2],ai_pcArgV[3], 
				atoi(ai_pcArgV[4]), &FCount  ) ) 
			{
				SwOk=M4_FALSE ;
				goto exit ;
			};

			printf("\n%i Fonts found and created !!!!\n", (int)FCount ) ;
		} ;
		

	} else if ( stricmp(ai_pcArgV[1], "-P")==0 ) { 

		m4int16_t FCount ;

		if (  ai_iArgC != 4 ) {

			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
			printf( SintaxMsg ) ;
			SwOk=M4_FALSE ;
	
		} else  {

			if ( ! ClRFN_RFN_W::ExtractFromPRN_To_PFA(ai_pcArgV[2],ai_pcArgV[3], &FCount  ) ) 
			{
				SwOk=M4_FALSE ;
				goto exit ;
			};

			printf("\n%i Fonts found and created !!!!\n", (int)FCount ) ;
		} ;

	} else if ( stricmp(ai_pcArgV[1], "-T")==0 ) { 

		m4int16_t FCount ;

		if (  ai_iArgC != 4 ) {

			*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
			printf( SintaxMsg ) ;
			SwOk=M4_FALSE ;
	
		} else  {

			if ( ! ClRFN_RFN_W::ConvertFromTTF_To_PFA(ai_pcArgV[2],ai_pcArgV[3] ) ) 
			{
				SwOk=M4_FALSE ;
				goto exit ;
			};

			printf("\nConversion ended !!!!\n") ;
		} ;

	} else {

		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, SintaxMsg ) << SEND_MSG ;
		printf( SintaxMsg ) ;
		SwOk=M4_FALSE ;


	} ;


	SWAP_OUT() ;
	
	//==============================================>Ejecución 

exit:

	//==============================================>Fase de ...
	KILL_IN(NewTask) ;


	if (SwOk) SwOk = Insp->m_poTrace->IsAllOK() ;

	KILL_OUT(NewTask) ;
	//<==============================================KILL


	if ( SwOk ) {
		printf( "\n\nParsing succesfully\n" ) ;
	} else {
		printf( "\n\nParsing with ERRORS !!!!!!!!!!!\n" ) ;
	} ;


} 