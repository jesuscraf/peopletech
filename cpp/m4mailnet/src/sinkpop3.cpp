//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail
// File:             sinkpop3.cpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             30/8/99
// Language:         C++
// Operating System: UNIX, WIN32
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#if defined( _WINDOWS ) || defined ( WINDOWS )
#	include <windows.h>
#else
#	include <sys/select.h>
#endif


// este fichero tiene que ser el primero.
// Tiene problemas con el fichero nsmail.h que define también ERRORLOG
#include "m4log.hpp"
#include "errors.hpp"
#include "m4unicode.hpp"


//Notification sink for POP3 commands.
#include "pop3.h"

// Variables globales del globals.h
extern char * g_pcFile;

// includes de nuestros fuentes
#include "sinkpop3.h"

//Notification for the response to the connection to the server
void POP3_connect( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%s\n", in_responseMessage );
	#endif
}

//Notification for the response to the DELE command.
void POP3_delete( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%s\n", in_responseMessage );
	#endif	
}


//Notification for an error response
void POP3_error( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	in_pPOP3Sink->pOpaqueData = (void*)-1;
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "ERROR:%s\n", in_responseMessage );
	#endif
}

//Notification for the start of the LIST command
void POP3_listStart( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "LIST Start\n" );
	#endif
}

//Notification for the response to the LIST command
void POP3_list( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%d %d\n", in_messageNumber, in_octetCount );
	#endif
}

//Notification for the completion of the LIST command
void POP3_listComplete( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "LIST Complete\n" );
	#endif
}

//Notification for the response to the NOOP command
void POP3_noop( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "NOOP\n" );
	#endif
}

//Notification for the response to the PASS command
void POP3_pass(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%s\n", in_responseMessage );
	#endif
}

//Notification for the response to the QUIT command
void POP3_quit(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%s\n", in_responseMessage );
	#endif
}

//Notification for the response to the RSET command
void POP3_reset(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
	    printf( "%s\n", in_responseMessage );
	#endif
}

//Notification for the start of a message from the RETR command
void POP3_retrieveStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount )
{
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	FILE * l_file = M4Fopen( g_pcFile, M4UniWriteBinary, eEncoding );
	if (NULL == l_file)
	{
		SETCODEF (M4_MAIL_E_WRITETEMP, -1, "#*s1*#%s#", g_pcFile);
	}
	in_pPOP3Sink->pOpaqueData = (void*)l_file;
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "RETR Start: %d %d\n", in_messageNumber, in_octetCount );
	#endif
}

//Notification for raw message from the RETR command
void POP3_retrieve( pop3Sink_t * in_pPOP3Sink, const char * in_messageChunk )
{
	FILE * l_file = (FILE*)in_pPOP3Sink->pOpaqueData;
	if (NULL == l_file)
	{
		return;
	}
	fputs( in_messageChunk, l_file );
//	#ifdef	_M4MAIL_PRINT_DEBUG
//		printf( "%s", in_messageChunk );
//	#endif
}

//Notification for the completion of the RETR command
void POP3_retrieveComplete( pop3Sink_t * in_pPOP3Sink )
{
	FILE * l_file = (FILE*)in_pPOP3Sink->pOpaqueData;
	if (NULL == l_file)
	{
		return;
	}
   	fclose( l_file );
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%RETR Complete\n" );
	#endif
}

//Notification for the start of the extended command
void POP3_sendCommandStart( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "SENDCOMMAND Start\n" );
	#endif
}

//Notification for the response to sendCommand() method
void POP3_sendCommand( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%s\n", in_responseMessage );
	#endif	
}

//Notification for the completion of the extended command
void POP3_sendCommandComplete( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "SENDCOMMAND Complete\n" );
	#endif
}

//Notification for the response to the STAT command
void POP3_stat( pop3Sink_t * in_pPOP3Sink, int in_messageCount, int in_octetCount )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %d\n", in_messageCount, in_octetCount );
	#endif
	// Asignamos el número de mensajes
	// Metemos el entero en el pOpaqueData
	in_pPOP3Sink->pOpaqueData = (void *)in_messageCount;
}

//Notification for the start of a message from the TOP command
void POP3_topStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "TOP Start: %d\n", in_messageNumber );
	#endif
}

//Notification for a line of the message from the TOP command
void POP3_top( pop3Sink_t * in_pPOP3Sink, const char * in_responseLine )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%s\n", in_responseLine );
	#endif
}

//Notification for the completion of the TOP command
void POP3_topComplete( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%TOP Complete\n" );
	#endif
}

//Notification for the start of the UIDL command
void POP3_uidListStart( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "UIDL Start\n" );
	#endif
}

//Notification for the response to the UIDL command
void POP3_uidList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_uid )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_messageNumber, in_uid );
	#endif
}

//Notification for the completion of the UIDL command
void POP3_uidListComplete( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "UIDL Complete\n" );
	#endif
}

//Notification for the response to the USER command.
void POP3_user( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%s\n", in_responseMessage );
	#endif
}

//Notification for the start of the XAUTHLIST command
void POP3_xAuthListStart( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "XAUTHLIST Start\n" );
	#endif
}

//Notification for the response to the XAUTHLIST command
void POP3_xAuthList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_messageNumber, in_responseMessage );
	#endif
}

//Notification for the completion of the XAUTHLIST command
void POP3_xAuthListComplete( pop3Sink_t * in_pPOP3Sink )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "XAUTHLIST Complete\n" );
	#endif
}

//Notification for the response to the XSENDER command
void POP3_xSender( pop3Sink_t * in_pPOP3Sink, const char * in_emailAddress )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%s\n", in_emailAddress );
	#endif
}
