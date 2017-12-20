//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail.dll
// File:             sinksmtp.h
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


#ifndef SINK_H_SMTP
#define SINK_H_SMTP

#include <stdio.h>

void SMTP_bdat( smtpSink_t *, int, const char * );
void SMTP_connect( smtpSink_t *, int, const char * );
void SMTP_data( smtpSink_t *, int, const char * );
void SMTP_ehlo( smtpSink_t *, int, const char * );
void SMTP_ehloComplete(smtpSink_t *);
void SMTP_expand( smtpSink_t *, int, const char * );
void SMTP_expandComplete(smtpSink_t *);
void SMTP_help( smtpSink_t *, int, const char * );
void SMTP_helpComplete(smtpSink_t *);
void SMTP_mailFrom( smtpSink_t *, int, const char * );
void SMTP_noop( smtpSink_t *, int, const char * );
void SMTP_quit( smtpSink_t *, int, const char * );
void SMTP_rcptTo( smtpSink_t *, int, const char * );
void SMTP_reset( smtpSink_t *, int, const char * );
void SMTP_send( smtpSink_t *, int, const char * );
void SMTP_sendCommand( smtpSink_t *, int, const char * );
void SMTP_sendCommandComplete(smtpSink_t *);
void SMTP_verify( smtpSink_t *, int, const char * );

#endif /* SINK_H_SMTP */
