//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail
// File:             sinksmtp.cpp
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

#include "smtp.h"
#include "sinksmtp.h"

//Notification sink for SMTP commands.

//Notification for the response to the BDAT command
void SMTP_bdat( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the connection to the server
void SMTP_connect( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the DATA command
void SMTP_data( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the EHLO command
void SMTP_ehlo( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the completion of the EHLO command
void SMTP_ehloComplete(smtpSink_t * in_pSink)
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "EHLO complete\n" );
	#endif
}

//Notification for the response to a server error
void SMTP_error(smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the EXPN command
void SMTP_expand( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the completion of the EXPN command
void SMTP_expandComplete(smtpSink_t * in_pSink)
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "EXPAND complete\n" );
	#endif
}

//Notification for the response to the HELP command
void SMTP_help( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the completion of the HELP command
void SMTP_helpComplete(smtpSink_t * in_pSink)
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "HELP complete\n" );
	#endif
}

//Notification for the response to the MAIL FROM command
void SMTP_mailFrom( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the NOOP command
void SMTP_noop( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the QUIT command
void SMTP_quit( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the RCPT TO command
void SMTP_rcptTo( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to the RSET command
void SMTP_reset( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to sending the message
void SMTP_send( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the response to sending a generic command
void SMTP_sendCommand( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}

//Notification for the completion of send a generic command
void SMTP_sendCommandComplete(smtpSink_t * in_pSink)
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "SENDCOMMAND complete\n" );
	#endif
}

//Notification for the response to the VRFY command
void SMTP_verify( smtpSink_t * in_pSink, int in_responseCode, const char * in_responseMessage )
{
	#ifdef	_M4MAIL_PRINT_DEBUG
		printf( "%d %s\n", in_responseCode, in_responseMessage );
	#endif
}
