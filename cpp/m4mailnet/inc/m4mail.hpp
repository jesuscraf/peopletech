//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail.dll
// File:             m4mail.hpp
// Project:          Mail support
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


#include "m4types.hpp"
#include "m4var.hpp"
#include "m4mailnet_dll.hpp"


extern "C" M4_DECL_M4MAILNET m4return_t SendMail		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4MAILNET m4return_t ReceiveMail		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4MAILNET m4return_t NumberOfMails	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4MAILNET m4return_t DeleteMail		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4MAILNET m4return_t LoadMailData	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

#ifndef _WIN64

// general definitions
// default port for sendmail
#define SMTP_DEFAULT_PORT 25

// default port for reading mails
#define POP3_DEFAULT_PORT 110

// timeout in seconds
#define M4_MAIL_DEFAULT_TIMEOUT 10


// Types
typedef map< string,string,less <string> > headersMap_t;

//Exported functions

//Not exported functions

m4void_t ConcatString (m4char_t * &ao_pcCad1, const m4char_t * ai_pcCad2);

m4pchar_t mailstrdup (m4pcchar_t ai_pcString);

// funciones para Send
m4void_t setSinkSMTP( smtpSink_t * pSink );

m4char_t * getFileExt (m4char_t * filePath);

m4int_t CreateMIME( m4pchar_t ai_pcFrom,
					m4pchar_t ai_pcSender,
					m4pchar_t *ai_ppcTo,
					m4pchar_t *ai_ppcReplyTo,
					m4pchar_t *ai_ppcCc,
					m4pchar_t ai_pcDispositionNotificationTo,
					m4pchar_t ai_pcSubject,
					m4pchar_t ai_pcText,
					m4pchar_t *ai_ppcAttachments,
					m4pcchar_t ai_pcSubType,
					mime_content_type ai_iType,
					m4pchar_t ai_pcImportance,
					m4pchar_t ai_pcMessageId);

m4int_t SendSMTP(	m4pchar_t ai_pcServer,
					m4uint32_t ai_iPort,
					m4pchar_t ai_pcFrom,
					m4pchar_t ai_pcReturnPath,
					m4bool_t ai_bReturnReceipt,
					m4pchar_t *ai_ppcTo,
					m4pchar_t *ai_ppcCc,
					m4pchar_t *ai_ppcBcc,
					m4pchar_t ai_pcUser,
					m4pchar_t ai_pcPassword,
					m4pchar_t ai_pcHelloString,
					m4double_t ai_dTimeOut);

m4pchar_t* Parse( m4char_t * pcCad, const m4pchar_t pcSep, m4bool_t ai_bToANSI = M4_FALSE);

m4bool_t CheckAccounts(const m4pcchar_t* ai_ppcAccounts);

// funciones para receive

m4int_t ReceivePOP3( m4char_t* ai_pcServer,
					 m4char_t* ai_pcUser,
					 m4char_t* ai_pcPwd,
					 m4int_t ai_iNumMsg,
					 m4uint32_t	ai_uiPort);

m4void_t setSinkPOP3( pop3Sink_t * pSink );

m4char_t * getFileName (m4char_t * filePath, m4char_t cSep);
m4char_t * getFileName (m4char_t * filePath);

m4int_t walkMessage ( mime_message_t * pMessage,
					  m4char_t * & ao_pcFrom, 
					  m4char_t * & ao_pcTo,
					  m4char_t * & ao_pcCc,
					  m4char_t * & ao_pcSubject,
					  m4char_t * & ao_pcText,
					  m4char_t * & ao_pcAttachments,
					  headersMap_t & ao_oHeaders );

m4int_t ParseMIME ( m4char_t * & ao_pcFrom, 
				    m4char_t * & ao_pcTo,
					m4char_t * & ao_pcCc,
				    m4char_t * & ao_pcSubject,
				    m4char_t * & ao_pcText,
					m4char_t * & ao_pcAttachments,
					headersMap_t & ao_oHeaders );

m4int_t walkBodyPart  ( m4void_t * pBody, 
					    mime_content_type contentType,
					    m4char_t * & ao_pcFrom, 
						m4char_t * & ao_pcTo,
						m4char_t * & ao_pcCc,
						m4char_t * & ao_pcSubject,
						m4char_t * & ao_pcText,
						m4char_t * & ao_pcAttachments,
						headersMap_t & ao_oHeaders );

// Funciones para obtener el número de mails en el buzón

m4int_t NumberPOP3( m4char_t* ai_pcServer,
					m4char_t* ai_pcUser,
					m4char_t* ai_pcPwd,
					m4uint32_t	ai_uiPort);


// Funciones para eliminar un mensaje del buzón

m4int_t DeletePOP3( m4char_t* ai_pcServer,
					m4char_t* ai_pcUser,
					m4char_t* ai_pcPwd,
					m4int_t   ai_iNum,
					m4uint32_t	ai_uiPort);


#endif // _WIN64
