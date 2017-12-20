//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail.dll
// File:             sinkpop3.h
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

#ifndef SINK_H_POP3
#define SINK_H_POP3

#include <stdio.h>


void POP3_connect( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_delete( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_error( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_listStart( pop3Sink_t * in_pPOP3Sink );
void POP3_list( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount );
void POP3_listComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_noop( pop3Sink_t * in_pPOP3Sink );
void POP3_pass(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_quit(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_reset(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_retrieveStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount );
void POP3_retrieve( pop3Sink_t * in_pPOP3Sink, const char * in_messageChunk );
void POP3_retrieveComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_sendCommandStart( pop3Sink_t * in_pPOP3Sink );
void POP3_sendCommand( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_sendCommandComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_stat( pop3Sink_t * in_pPOP3Sink, int in_messageCount, int in_octetCount );
void POP3_topStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber );
void POP3_top( pop3Sink_t * in_pPOP3Sink, const char * in_responseLine );
void POP3_topComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_uidListStart( pop3Sink_t * in_pPOP3Sink );
void POP3_uidList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_uid );
void POP3_uidListComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_user( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
void POP3_xAuthListStart( pop3Sink_t * in_pPOP3Sink );
void POP3_xAuthList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_responseMessage );
void POP3_xAuthListComplete( pop3Sink_t * in_pPOP3Sink );
void POP3_xSender( pop3Sink_t * in_pPOP3Sink, const char * in_emailAddress );

#endif /* SINK_H_POP3 */
