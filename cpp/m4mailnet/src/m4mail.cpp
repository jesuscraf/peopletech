//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4mail
// File:             m4mail.cpp
// Project:          Mail support
// Author:           Meta Software M.S. , S.A
// Date:             30/8/99
// Language:         C++
// Operating System: WIN32, UNIX 
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


// Includes de otras librerias de M4

#ifdef _UNIX
	#include <unistd.h>
#endif

// Para manejar el contexto
#include "execontx.hpp"
#include "access.hpp"
#include "m4objreg.hpp"
#include "syncro.hpp"

// Includes de las librerias de Netscape
#include "nsstream.h"
#include "smtp.h"
#include "pop3.h"
#include "mime.h"
#include "mimeparser.h"

// Includes de nuestro fuentes
#include "errors.hpp"
#include "m4mail.hpp"
#include "sinksmtp.h"
#include "sinkpop3.h"
#include "globals.h"
#include "m4unicode.hpp"
#include "_base64.h"
#include <time.h>



#ifndef _WIN64

static m4pcchar_t s_vcMimeTypes[] = {"text","audio","image","video","application","multipart","messagepart",NULL};

static mime_content_type s_viMimeTypes[] = {MIME_CONTENT_TEXT,MIME_CONTENT_AUDIO,MIME_CONTENT_IMAGE,MIME_CONTENT_VIDEO,MIME_CONTENT_APPLICATION,MIME_CONTENT_MULTIPART,MIME_CONTENT_MESSAGEPART};

static mime_content_type ConvertType(m4pcchar_t ai_pcType) {
	int i=0;
	m4pcchar_t pcMimeType = NULL;
	for (i=0; (pcMimeType = s_vcMimeTypes[i]) != NULL; ++i) {
		if (stricmp(ai_pcType,pcMimeType) == 0) {
			return s_viMimeTypes[i];
		}
	}

	return MIME_CONTENT_UNINITIALIZED;
}

// Buscar el charset de las cabeceras SMTP.
// Puede ser: Subject: =?iso-8859-1?Q?.....?=
m4pchar_t getHeaderCharset(m4pcchar_t ai_rfc822_header)
{
	m4int32_t	iLen = 0;
	m4char_t	*pHeaderValue = NULL;
	m4char_t	*pCharsetStart = NULL;
	m4char_t	*pCharsetEnd = NULL;
	m4char_t	*pCharset = NULL;
	
	if (ai_rfc822_header == NULL)
	{
		return NULL;
	}
	
	iLen = strlen(ai_rfc822_header);
	pHeaderValue = new m4char_t[iLen + 1];
	strncpy(pHeaderValue, ai_rfc822_header, iLen);
	pHeaderValue[iLen] = '\0';
	
	pCharsetStart = strstr(pHeaderValue, "=?");
	if (pCharsetStart == NULL)
	{
		delete [] pHeaderValue;
		return NULL;
	}

	*pCharsetStart = '\0';
	pCharsetStart = pCharsetStart + 2;

	pCharsetEnd = strchr(pCharsetStart, '?');
	if (pCharsetEnd == NULL)
	{
		delete [] pHeaderValue;
		return NULL;
	}
	
	*pCharsetEnd = '\0';
	
	iLen = pCharsetEnd - pCharsetStart;
	pCharset= new char[iLen + 1];
	strncpy(pCharset, pCharsetStart, iLen);
	pCharset[iLen] = '\0';
	
	delete [] pHeaderValue;

	return pCharset;
}

// soporte para rfc2047. bugid: 0085667
// defined in mailutils/rfc2047.cpp
int rfc2047_decode (const char *tocode, const char *input, char **ptostr);

m4pchar_t decode_rfc2047_header(m4pcchar_t ai_rfc822_header, m4pcchar_t ai_pcHeaderCharset)
{
	int		muerror = 0;
	char*	buffer = NULL;
	char*	decoded_header = NULL;
	int		iLength = -1;
	
	muerror = rfc2047_decode ("iso-8859-1", ai_rfc822_header, &buffer);
	if (muerror == 0)
	{
		/* ok */
		if (ai_pcHeaderCharset != NULL)
		{
			if (strcmpi(ai_pcHeaderCharset, "iso-8859-1") == 0 || strcmpi(ai_pcHeaderCharset, "windows-1252") == 0)
			{
				// Unicode.
				iLength = -1;
				decoded_header = M4ANSIToCpp(buffer, iLength);
			}
			else if (strcmpi(ai_pcHeaderCharset, "UTF-8") == 0)
			{
				// Unicode.
				iLength = -1;
				decoded_header = M4Utf8ToCpp(buffer, iLength);
			}
			else
			{
				decoded_header = mailstrdup(ai_rfc822_header);
			}
		}
		else
		{
			decoded_header = mailstrdup(ai_rfc822_header);
		}
		
		free(buffer);
	}
	else
	{
		/* error */
		decoded_header = mailstrdup(ai_rfc822_header);
	}

	return decoded_header;
}

m4pchar_t EncodeHeader(const char * const ai_pccHeader, char ai_cEncoding)
{
	size_t		iSize = 0;
	m4char_t	* pcHeader = NULL;

	iSize = strlen( ai_pccHeader ) * 3 ; // En el peor de los casos hay que encodear todo el subject.
	iSize += strlen( M4CharSet() ) ;
	iSize += 8 ; // Caracteres adicionales: =?charset?Q?subject?=

	pcHeader = new m4char_t[ iSize ] ;
	memset( pcHeader, 0, iSize ) ;

	mime_encodeHeaderString( (char*)ai_pccHeader, (char*)M4CharSet(), ai_cEncoding, &pcHeader );

	return pcHeader;
}

// bugid: 0069775. Returns a safe filename.
void GetSafeFileName(m4char_t*& ai_pcFileName)
{
	// check input arguments.
	if (NULL == ai_pcFileName)
	{
		return;
	}
	
	// try to open input file.
	FILE* fd = NULL;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	fd = M4Fopen (ai_pcFileName, M4UniRead, eEncoding);
	if (NULL == fd)
	{
		// file does not exist.
		return;
	}

	// search for a valid filename.
	int i=1;
	m4pchar_t pcTmpFileName = NULL;
	m4char_t pcFileIndex[32];
	do
	{
		fclose (fd);
		if (NULL != pcTmpFileName) delete pcTmpFileName;

		pcTmpFileName = mailstrdup(ai_pcFileName);
		sprintf(pcFileIndex, "(%d)", i);
		ConcatString (pcTmpFileName, pcFileIndex);

		// UNICODE FILE
		 eEncoding = M4UniANSI ;
		fd = M4Fopen (pcTmpFileName, M4UniRead, eEncoding);
	} while ((i++<1024) && (NULL != fd));
	
	if (NULL != fd)
	{
		// I find dificult that i>1024. Someone is not deleting old files.
		delete pcTmpFileName;
		fclose(fd);
		return;
	}
	
	// I find dificult that i>1024. Someone is not deleting old files.
	delete ai_pcFileName;
	ai_pcFileName = pcTmpFileName;
}



// Function to send a mail from Mind
//
// Params:
//		0. MAIL SERVER
//		1. USER
//		2. PASSWORD
//		3. FROM
//		4. TO
//		5. CC
//		6. SUBJECT
//		7. BODY
//		8. ATTACHMENTS
//		9. HELLO STRING
//		10.MAIL PORT
//		11.MAIL Format sub-type, p.ej. html or plain
//		12.MAIL Format type, p.ej. text
//		13.TIMEOUT in milliseconds
//		14.SENDER
//		15.REPLY-TO
//		16.BCC
//		17.RETURN-PATH
//		18.RETURN-RECEIPT
//		19.DISPOSITION-NOTIFICATION-TO
//		20.IMPORTANCE
//		21.MESSAGE-ID
// Returns:
//		M4_ERROR, M4_SUCCESS

static	m4int32_t	_SendMail( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg )
{

	m4pchar_t pcUser = NULL;
	m4pchar_t pcPassword = NULL;
	m4pchar_t pcServer = NULL;
	m4pchar_t pcFrom = NULL;
	m4pchar_t pcSender = NULL;
	m4pchar_t pcSubject = NULL;
	m4pchar_t pcBody = NULL;
	m4pchar_t pcHelloString = NULL;
	m4pchar_t pcMailSubType = "plain";
	m4pchar_t pcReturnPath = NULL;
	m4pchar_t pcDispositionNotificationTo = NULL;
	m4pchar_t pcImportance = "normal";
	m4pchar_t pcMessageId = NULL;
	m4pchar_t *ppcTo = NULL;
	m4pchar_t *ppcReplyTo = NULL;
	m4pchar_t *ppcCc = NULL;
	m4pchar_t *ppcBcc = NULL;
	m4pchar_t *ppcAttachments = NULL;

	m4bool_t	bReturnReceipt = M4_FALSE;
	m4int_t		iRes = 0;
	size_t		i = 0;
	m4uint32_t	uiPort = SMTP_DEFAULT_PORT;
	m4double_t	dTimeOut = M4_MAIL_DEFAULT_TIMEOUT;

	mime_content_type iMailType = MIME_CONTENT_TEXT;


	// fix bug 76198 
	// el argumento 11 (puerto) es opcional (compatibilidad 5.0)
	
	// se añade soporte a formatos , parámetro 12, 13 y 14, opcional (compatibilidad 7.0)
	// se añaden sender, reply-to, bcc, return-path, return-receipt, disposition-notification-to, importance
	if( ai_iLongArg < 10 || ai_iLongArg > 22 )
	{
		// Error numero de parametros incorrecto
		SETCODEF( M4_MAIL_E_INVALID_NUM_ARG, M4_ERROR, "#*s1*#%s#", "SendMail" );
		return M4_ERROR;
	}

	// Comprobamos el SERVER
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[0].Data.PointerChar != NULL && ai_pvArg[0].Data.PointerChar[0] != '\0' )
	{
		pcServer = ai_pvArg[0].Data.PointerChar;
	}
	else
	{
		// Error no se ha pasado el server
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MAIL SERVER" );
		return M4_ERROR;
	}
	
	// Comprobamos el USER
	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[1].Data.PointerChar != NULL && ai_pvArg[1].Data.PointerChar[0] != '\0' )
	{
		pcUser = ai_pvArg[1].Data.PointerChar;
	}

	// Comprobamos el PASSWORD
	if( ai_pvArg[2].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[2].Data.PointerChar != NULL && ai_pvArg[2].Data.PointerChar[0] != '\0' )
	{
		pcPassword = ai_pvArg[2].Data.PointerChar;
	}

	// Comprobamos el FROM
	if( ai_pvArg[3].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[3].Data.PointerChar != NULL && ai_pvArg[3].Data.PointerChar[0] != '\0' )
	{
		pcFrom = ai_pvArg[3].Data.PointerChar;
	}
	else
	{
		// Error no se ha pasado el from
		SETCODEF( M4_MAIL_E_INVALID_PROPERTY, M4_ERROR, "#*s1*#%s#", "FROM" );
		return M4_ERROR;
	}

	// Comprobamos el TO
	if( ai_pvArg[4].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[4].Data.PointerChar != NULL && ai_pvArg[4].Data.PointerChar[0] != '\0' )
	{
		ppcTo = Parse( ai_pvArg[4].Data.PointerChar, ";" );

		m4bool_t bCheck = CheckAccounts( ppcTo );

		if( bCheck == M4_FALSE )
		{
			// Alguna dirección no es correcta
			return M4_ERROR;
		}
	}

	// Comprobamos el CC
	if( ai_pvArg[5].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[5].Data.PointerChar != NULL && ai_pvArg[5].Data.PointerChar[0] != '\0' )
	{
		ppcCc = Parse( ai_pvArg[5].Data.PointerChar, ";" );

		m4bool_t bCheck = CheckAccounts( ppcCc );

		if( bCheck == M4_FALSE )
		{
			// Alguna dirección no es correcta
			return M4_ERROR;
		}
	}

	// Comprobamos el subject
	if( ai_pvArg[6].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[6].Data.PointerChar != NULL && ai_pvArg[6].Data.PointerChar[0] != '\0' )
	{
		pcSubject = ai_pvArg[6].Data.PointerChar;
	}

	// Comprobamos el body
	if( ai_pvArg[7].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[7].Data.PointerChar != NULL && ai_pvArg[7].Data.PointerChar[0] != '\0' )
	{
		pcBody = ai_pvArg[7].Data.PointerChar;
	}

	// Los attachments pueden ser NULL
	if( ai_pvArg[8].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[8].Data.PointerChar != NULL && ai_pvArg[8].Data.PointerChar[0] != '\0' )
	{
		ppcAttachments = Parse( ai_pvArg[8].Data.PointerChar, "*");
	}
	else
	{
		ppcAttachments = NULL;
	}

	if( ai_pvArg[9].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[9].Data.PointerChar != NULL && ai_pvArg[9].Data.PointerChar[0] != '\0' )
	{
		pcHelloString = ai_pvArg[9].Data.PointerChar;
	}
	else
	{
		// Error no se ha pasado nada en la cadena a enviar
		// Utilizamos una cadena vacía en ese caso
		pcHelloString = "";
	}

	/* Bug 0053284
	Se usa el puerto
	*/
	if( ai_iLongArg > 10 && ai_pvArg[10].Type != M4CL_CPP_TYPE_NULL )
	{
		if( ai_pvArg[ 10 ].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[10].Data.PointerChar [0] == '\0' )
		{
			// default value
		} 
		else if( ai_pvArg[ 10 ].Type != M4CL_CPP_TYPE_NUMBER || ai_pvArg[ 10 ].Data.DoubleData <= 0 )
		{
			SETCODEF( M4_MAIL_E_INVALID_PROPERTY, M4_ERROR, "#*s1*#%s#", "PORT" );
			return M4_ERROR;
		}
		else
		{
			uiPort = (m4uint32_t) ai_pvArg[ 10 ].Data.DoubleData; 
		}
	}

	// Comprueba el formato sub type de correo
	if( ai_iLongArg > 11 && ai_pvArg[11].Type != M4CL_CPP_TYPE_NULL )
	{
		if( ai_pvArg[ 11 ].Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			// Error no se ha pasado el server
			SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "SUBTYPE" );
			return M4_ERROR;
		} 
		else if( ai_pvArg[11].Data.PointerChar [0] == '\0' )
		{
			// default value
		}
		else
		{
			pcMailSubType = ai_pvArg[11].Data.PointerChar;
		}
	}

	// Comprueba el formato type de correo
	if( ai_iLongArg > 12 && ai_pvArg[12].Type != M4CL_CPP_TYPE_NULL )
	{
		if( ai_pvArg[ 12 ].Type != M4CL_CPP_TYPE_STRING_VAR || 
			( ai_pvArg[ 12 ].Data.PointerChar[0] != '\0' && ConvertType(ai_pvArg[ 12 ].Data.PointerChar) == MIME_CONTENT_UNINITIALIZED ) )
		{
			// Error no se ha pasado el server
			SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "TYPE" );
			return M4_ERROR;
		} 
		else if( ai_pvArg[ 12 ].Data.PointerChar[0] == '\0' )
		{
			// default value
		}
		else
		{
			iMailType = ConvertType(ai_pvArg[12].Data.PointerChar);
		}
	}

	// Fix bug 0183909: adds support to timeout
	if( ai_iLongArg > 13 && ai_pvArg[13].Type != M4CL_CPP_TYPE_NULL )
	{
		if( ai_pvArg[ 13 ].Type != M4CL_CPP_TYPE_NUMBER )
		{
			// Error invalid timeout value
			SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "TIMEOUT");
			return M4_ERROR;
		} 

		dTimeOut = ai_pvArg[13].Data.DoubleData;
	}

	if( ai_iLongArg > 14 && ai_pvArg[14].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos el sender
		if( ai_pvArg[14].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[14].Data.PointerChar != NULL && ai_pvArg[14].Data.PointerChar[0] != '\0' )
		{
			pcSender = ai_pvArg[14].Data.PointerChar;
		}
	}

	if( ai_iLongArg > 15 && ai_pvArg[15].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos el reply-to
		if( ai_pvArg[15].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[15].Data.PointerChar != NULL && ai_pvArg[15].Data.PointerChar[0] != '\0' )
		{
			ppcReplyTo = Parse( ai_pvArg[15].Data.PointerChar, ";" );

			if( ppcReplyTo == NULL )
			{
				// Error el ReplyTo no tiene ni uno válido
				SETCODEF( M4_MAIL_E_INVALID_PROPERTY, M4_ERROR, "#*s1*#%s#", "REPLY-TO" );
				return M4_ERROR;
			}

			m4bool_t bCheck = CheckAccounts( ppcReplyTo );

			if( bCheck == M4_FALSE )
			{
				// Alguna dirección no es correcta
				return M4_ERROR;
			}
		}
	}

	if( ai_iLongArg > 16 && ai_pvArg[16].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos el Bcc
		if( ai_pvArg[16].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[16].Data.PointerChar != NULL && ai_pvArg[16].Data.PointerChar[0] != '\0' )
		{
			ppcBcc = Parse( ai_pvArg[16].Data.PointerChar, ";" );

			if( ppcBcc == NULL )
			{
				// Error el Bcc no tiene ni uno válido
				SETCODEF( M4_MAIL_E_INVALID_PROPERTY, M4_ERROR, "#*s1*#%s#", "BCC" );
				return M4_ERROR;
			}

			m4bool_t bCheck = CheckAccounts( ppcBcc );

			if( bCheck == M4_FALSE )
			{
				// Alguna dirección no es correcta
				return M4_ERROR;
			}
		}
	}

	if( ai_iLongArg > 17 && ai_pvArg[17].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos el return-path
		if( ai_pvArg[17].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[17].Data.PointerChar != NULL && ai_pvArg[17].Data.PointerChar[0] != '\0' )
		{
			pcReturnPath = ai_pvArg[17].Data.PointerChar;
		}
	}

	if( ai_iLongArg > 18 && ai_pvArg[18].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos return-receipt
		if( ai_pvArg[18].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[18].Data.DoubleData == 1 )
		{
			bReturnReceipt = M4_TRUE;
		}
	}

	if( ai_iLongArg > 19 && ai_pvArg[19].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos el disposition-notification-to
		if( ai_pvArg[19].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[19].Data.PointerChar != NULL && ai_pvArg[19].Data.PointerChar[0] != '\0' )
		{
			pcDispositionNotificationTo = ai_pvArg[19].Data.PointerChar;
		}
	}

	if( ai_iLongArg > 20 && ai_pvArg[20].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos la importancia
		if( ai_pvArg[20].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[20].Data.PointerChar != NULL && ai_pvArg[20].Data.PointerChar[0] != '\0' )
		{
			if( strcmpi( ai_pvArg[20].Data.PointerChar, "low" ) == 0 )
			{
				pcImportance = "low" ;
			}
			else if( strcmpi( ai_pvArg[20].Data.PointerChar, "high" ) == 0 )
			{
				pcImportance = "high" ;
			}
		}
	}

	if( ai_iLongArg > 21 && ai_pvArg[21].Type != M4CL_CPP_TYPE_NULL )
	{
		// Comprobamos si viene la cabecera Message-ID.
		if( ai_pvArg[21].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[21].Data.PointerChar != NULL && ai_pvArg[21].Data.PointerChar[0] != '\0' )
		{
			pcMessageId = ai_pvArg[21].Data.PointerChar;
		}
	}

	if( ppcTo == NULL && ppcCc == NULL && ppcBcc == NULL )
	{
		// Error el To no tiene ni uno válido
		SETCODEF( M4_MAIL_E_INVALID_PROPERTY, M4_ERROR, "#*s1*#%s#", "DESTINATION" );
		return M4_ERROR;
	}

	// Construimos el mime
	iRes = CreateMIME(	pcFrom,
						pcSender,
						ppcTo,
						ppcReplyTo,
						ppcCc,
						pcDispositionNotificationTo,
						pcSubject,
						pcBody,
						ppcAttachments,
						pcMailSubType,
						iMailType,
						pcImportance,
						pcMessageId);

	if( ppcAttachments != NULL )
	{
		for( i = 0; ppcAttachments[i] != NULL; i++ )
		{
			delete( ppcAttachments[i] ) ;
		}
		delete( ppcAttachments ) ;
		ppcAttachments = NULL ;
	}

	// Controlamos el error al crear el mime
	if( iRes != 0 )
	{
		SETCODE( M4_MAIL_E_CREATEMIME, M4_ERROR, "#*s1*#");
		return iRes;
	}

	iRes = SendSMTP(	pcServer,
						uiPort,
						pcFrom,
						pcReturnPath,
						bReturnReceipt,
						ppcTo,
						ppcCc,
						ppcBcc,
						pcUser,
						pcPassword,
						pcHelloString,
						dTimeOut);

	if( ppcTo != NULL )
	{
		for( i = 0; ppcTo[i] != NULL; i++ )
		{
			delete( ppcTo[i] ) ;
		}
		delete( ppcTo ) ;
		ppcTo = NULL ;
	}

	if( ppcReplyTo != NULL )
	{
		for( i = 0; ppcReplyTo[i] != NULL; i++ )
		{
			delete( ppcReplyTo[i] ) ;
		}
		delete( ppcReplyTo ) ;
		ppcReplyTo = NULL ;
	}

	if( ppcCc != NULL )
	{
		for( i = 0; ppcCc[i] != NULL; i++ )
		{
			delete( ppcCc[i] ) ;
		}
		delete( ppcCc ) ;
		ppcCc = NULL ;
	}

	if( ppcBcc != NULL )
	{
		for( i = 0; ppcBcc[i] != NULL; i++ )
		{
			delete( ppcBcc[i] ) ;
		}
		delete( ppcBcc ) ;
		ppcBcc = NULL ;
	}

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Send  realizado: " << iRes << "\n";
	#endif

	if( iRes != 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", pcServer );
				break;
			case -20:
				// bg 139211
				SETCODEF( M4_MAIL_E_SENDDATA, M4_ERROR, "#*s1*#%s#", "Data send failed. Recoverable error." );
				break;			
			default:
				SETCODEF( M4_MAIL_E_SENDSMTP, M4_ERROR, "#*s1*#%d#", iRes );
		}
		return iRes;
	}

	return M4_SUCCESS;
}


// Function to receive a mail from Mind
//
// Params:
//		0.  MAIL SERVER		in
//		1.  USER			in
//		2.  PASSWORD		in
//		3.  MSG NUMBER		in
//		4.  FROM			out
//		5.  TO				out
//		6.  CC				out
//		7.  SUBJECT			out
//		8.  BODY			out
//		9.  ATTACHMENTS		out
//		10. PORT			in
// Returns:
//		M_4ERROR, M4_SUCCESS

static	m4int32_t	_ReceiveMail( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg )
{

	m4int_t			iRes = 0;
	m4uint32_t		uiPort = POP3_DEFAULT_PORT;
	headersMap_t	oHeadersMap;


	if (ai_iLongArg != 11) {
		// Error numero de parametros incorrecto
		SETCODEF ( M4_MAIL_E_INVALID_NUM_ARG, M4_ERROR, "#*s1*#%s#", "ReceiveMail");
		return M4_ERROR;
	}

	// Comprobamos el SERVER
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0') ) {

		// Error no se ha pasado el server
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MAIL SERVER");
		return M4_ERROR;
	}

	// Comprobamos el USER
	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar [0] == '\0' ) ) {

		// Error no se ha pasado el user
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "USER");
		return M4_ERROR;
	}

	// Comprobamos el PASSWD
	if ( (ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].Data.PointerChar [0] == '\0' ) ) {
		
		// Error no se ha pasado el Passwd
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "PASSWORD");
		return M4_ERROR;
	}
	
	// Comprobamos el MSG NUM
	// Si es negativo devolvemos error
	if (( ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER) ||
		( ai_pvArg[3].Data.DoubleData <= 0 ) ) {

		// Error no se ha pasado el numero de mensaje
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}

	// Comprobamos el puerto
	if (( ai_pvArg[10].Type != M4CL_CPP_TYPE_NUMBER) || ( ai_pvArg[3].Data.DoubleData <= 0 ) )
	{
		// Error no se ha pasado el puerto
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}
	else
	{
		uiPort = (m4uint32_t) ai_pvArg[10].Data.DoubleData;
	}

	// Chapuza metida por un bug, en un principio se pensaba tener 
	// las conexiones cacheadas, por ahora así de cutre.
	// Vamos a ver si el número de mensaje existe
	iRes = NumberPOP3 ( ai_pvArg[0].Data.PointerChar,
						ai_pvArg[1].Data.PointerChar,
						ai_pvArg[2].Data.PointerChar,
						uiPort);

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Número de mensajes disponibles: " << iRes << "\n";
	#endif

	if( iRes < 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "ReceiveMail" );
		}
		return iRes;
	}

	if ( ai_pvArg[3].Data.DoubleData > iRes ) {

		// Error pues el mensaje no existe
		SETCODEF( M4_MAIL_E_MESSAGE_NOT_FOUND, M4_ERROR, "#*s1*#%d#", (m4int_t)(ai_pvArg[3].Data.DoubleData));
		return M4_ERROR;
	}

	// Pues ala, a recibirlo
	iRes = ReceivePOP3( ai_pvArg[0].Data.PointerChar,
						ai_pvArg[1].Data.PointerChar,
						ai_pvArg[2].Data.PointerChar,
						(m4int_t)ai_pvArg[3].Data.DoubleData,
						uiPort);

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "POP3 recibido: " << iRes << "\n";
	#endif

	if( iRes != 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "ReceiveMail" );
		}
		return iRes;
	}

	m4char_t * pcFrom = NULL;
	m4char_t * pcTo = NULL;
	m4char_t * pcCc = NULL;
	m4char_t * pcSubject = NULL;
	m4char_t * pcBody = NULL;
	m4char_t * pcAttachments = NULL;

	iRes =  ParseMIME ( pcFrom,
						pcTo,
						pcCc,
						pcSubject,
						pcBody,
						pcAttachments,
						oHeadersMap );

	// Not used.
	oHeadersMap.clear();

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Parse del mime realizado: " << iRes << "\n";
	#endif

	if( iRes != 0 )
	{
		// Controlar el tipo de error
		SETCODEF ( M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "ReceiveMail");
		return iRes;
	}

	if (pcFrom)
		ai_pvArg[4].SetSysString (pcFrom);
	if (pcTo)
		ai_pvArg[5].SetSysString (pcTo);
	if (pcCc)
		ai_pvArg[6].SetSysString (pcCc);
	if (pcSubject)
		ai_pvArg[7].SetSysString (pcSubject);
	if (pcBody)
		ai_pvArg[8].SetSysString (pcBody);
	if (pcAttachments)
		ai_pvArg[9].SetSysString (pcAttachments);
	
	delete [] pcFrom;
	delete [] pcTo;
	delete [] pcCc;
	delete [] pcSubject;
	delete [] pcBody;
	delete [] pcAttachments;
	
	return M4_SUCCESS;
}


// Function to read all mail from a given user.
//
// Params:
//		0. MAIL SERVER		in
//		1. USER				in
//		2. PASSWORD			in
//		3. MSG_NUM			in
//		4. PORT				in
// Returns:
//		M4_ERROR, M4_SUCCESS

static	m4int32_t	_LoadMailData( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,	ClVMRunContext* ai_pvContext )
{

	m4int_t					iRes = 0;
	m4uint32_t				uiPort = POP3_DEFAULT_PORT;
	m4return_t				iResult = 0;
	m4VariantType			vPropertyName;
	m4VariantType			vPropertyValue;
	string					sPropertyName;
	string					sPropertyValue;	
	headersMap_t			oHeadersMap;
	headersMap_t::iterator	iterator;
	m4char_t *				pcFrom = NULL;
	m4char_t *				pcTo = NULL;
	m4char_t *				pcCc = NULL;
	m4char_t *				pcSubject = NULL;
	m4char_t *				pcBody = NULL;
	m4char_t *				pcAttachments = NULL;


	if (ai_iLongArg < 5)
	{
		// Error numero de parametros incorrecto
		SETCODEF(M4_MAIL_E_INVALID_NUM_ARG, M4_ERROR, "#*s1*#%s#", "LoadMailData");
		return M4_ERROR;
	}
	
	// Comprobamos el SERVER
	if ((ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) || (ai_pvArg[0].Data.PointerChar[0] == '\0'))
	{
		// Error no se ha pasado el server
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MAIL SERVER");
		return M4_ERROR;
	}
	
	// Comprobamos el USER
	if ((ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) || (ai_pvArg[1].Data.PointerChar[1] == '\0'))
	{
		// Error no se ha pasado el user
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "USER");
		return M4_ERROR;
	}
	
	// Comprobamos el PASSWD
	if ((ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) || (ai_pvArg[2].Data.PointerChar[2] == '\0'))
	{	
		// Error no se ha pasado el Passwd
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "PASSWORD");
		return M4_ERROR;
	}
	
	// Comprobamos el MSG NUM
	// Si es negativo devolvemos error
	if (( ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER) || (ai_pvArg[3].Data.DoubleData <= 0))
	{
		// Error no se ha pasado el numero de mensaje
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}
	
	// Comprobamos el puerto.
	if (( ai_pvArg[4].Type != M4CL_CPP_TYPE_NUMBER) || ( ai_pvArg[4].Data.DoubleData <= 0 ) )
	{
		// Error no se ha pasado el puerto
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}
	else
	{
		uiPort = (m4uint32_t) ai_pvArg[4].Data.DoubleData;
	}

	// Obtener número de mensajes.
	iRes = NumberPOP3(ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, ai_pvArg[2].Data.PointerChar, uiPort);
	
	if( iRes < 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "LoadMailData" );
		}
		return iRes;
	}
	
	if (ai_pvArg[3].Data.DoubleData > iRes)
	{
		// Error pues el mensaje no existe
		SETCODEF(M4_MAIL_E_MESSAGE_NOT_FOUND, M4_ERROR, "#*s1*#%d#", (m4int_t)(ai_pvArg[3].Data.DoubleData));
		return M4_ERROR;
	}

	// Leer correo.
	iRes = ReceivePOP3(ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, ai_pvArg[2].Data.PointerChar, ai_pvArg[3].Data.DoubleData, uiPort);

	if( iRes != 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "LoadMailData" );
		}
		return iRes;
	}
	
	iRes =  ParseMIME (pcFrom,	pcTo, pcCc, pcSubject, pcBody, pcAttachments, oHeadersMap);

	if( iRes != 0 )
	{
		oHeadersMap.clear();

		SETCODEF(M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "LoadMailData");
		return iRes;
	}
	
	// Not used.
	delete [] pcFrom;
	delete [] pcTo;
	delete [] pcCc;
	delete [] pcSubject;
	delete [] pcBody;

	// Guardar las cabeceras del mail y los attachments en el nodo SRTC_MAIL_PROPERTIES
	ClNode * poNode = &(ai_pvContext->m_pAccess->Node["SRTC_MAIL_PROPERTIES"]);

	if (poNode == NULL)
	{
		delete [] pcAttachments;
		SETCODEF(M4_MAIL_E_NO_SUCH_NODE, M4_ERROR, "#*S1*#%s#%s#", "SRTC_MAIL_PROPERTIES", "LoadMailData");
		return M4_ERROR;
	}

	// Borrar los registros.
	poNode->RecordSet.Current.Destroy();

	iterator = oHeadersMap.begin();
	while (iterator != oHeadersMap.end())
	{
		sPropertyName = (*iterator).first;
		sPropertyValue = (*iterator).second;
		
		vPropertyName.Type = M4CL_CPP_TYPE_STRING_VAR;
		vPropertyName.Data.PointerChar = (m4pchar_t) sPropertyName.c_str();
	
		vPropertyValue.Type = M4CL_CPP_TYPE_STRING_VAR;
		vPropertyValue.Data.PointerChar = (m4pchar_t) sPropertyValue.c_str();
	
		// Añadir un registro nuevo.
		iRes = poNode->RecordSet.Current.Add();
		if (iRes != M4_SUCCESS)
		{
			delete [] pcAttachments;
			SETCODEF(M4_MAIL_E_ADD_REGISTER, M4_ERROR, "#*S1*#%s#%s#", "SRTC_MAIL_PROPERTIES", "LoadMailData");
			return M4_ERROR;
		}

		if (poNode->RecordSet.Current.Item["NAME"].Value.Set(vPropertyName) == M4_ERROR)
		{
			delete [] pcAttachments;
			SETCODEF(M4_MAIL_E_SET_ITEM_VALUE, M4_ERROR, "#*S1*#%s#%s#%s#", "NAME", "SRTC_MAIL_PROPERTIES", "LoadMailData");
			return M4_ERROR;
		}
		
		if (poNode->RecordSet.Current.Item["VALUE"].Value.Set(vPropertyValue) == M4_ERROR)
		{
			delete [] pcAttachments;
			SETCODEF(M4_MAIL_E_SET_ITEM_VALUE, M4_ERROR, "#*S1*#%s#%s#%s#", "VALUE", "SRTC_MAIL_PROPERTIES", "LoadMailData");
			return M4_ERROR;
		}
		
		iterator++;
	}
	
	oHeadersMap.clear();
	
	// Por último, añadir los ficheros adjuntos.
	sPropertyName = "Attachments";
	sPropertyValue = "";
	if (pcAttachments != NULL)
	{
		sPropertyValue.assign(pcAttachments);
	}
	vPropertyName.Data.PointerChar = (m4pchar_t) sPropertyName.c_str();
	vPropertyValue.Data.PointerChar = (m4pchar_t) sPropertyValue.c_str();

	delete [] pcAttachments;

	iResult = poNode->RecordSet.Current.Add();
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_MAIL_E_ADD_REGISTER, M4_ERROR, "#*S1*#%s#%s#", "SRTC_MAIL_PROPERTIES", "LoadMailData");
		return M4_ERROR;
	}
	
	if (poNode->RecordSet.Current.Item["NAME"].Value.Set(vPropertyName) == M4_ERROR)
	{
		SETCODEF(M4_MAIL_E_SET_ITEM_VALUE, M4_ERROR, "#*S1*#%s#%s#%s#", "NAME", "SRTC_MAIL_PROPERTIES", "LoadMailData");
		return M4_ERROR;
	}
	
	if (poNode->RecordSet.Current.Item["VALUE"].Value.Set(vPropertyValue) == M4_ERROR)
	{
		SETCODEF(M4_MAIL_E_SET_ITEM_VALUE, M4_ERROR, "#*S1*#%s#%s#%s#", "VALUE", "SRTC_MAIL_PROPERTIES", "LoadMailData");
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}


// Function to get the number of mesages from Mind
//
// Params:
//		0. MAIL SERVER		in
//		1. USER				in
//		2. PASSWORD			in
//		2. PORT				in
// Returns:
//		M_4ERROR, M4_SUCCESS

static	m4int32_t	_NumberOfMails( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg )
{
	
	m4int_t		iRes = 0;
	m4uint32_t	uiPort = POP3_DEFAULT_PORT;

	if (ai_iLongArg != 4) {
		// Error numero de parametros incorrecto
		SETCODEF ( M4_MAIL_E_INVALID_NUM_ARG, M4_ERROR, "#*s1*#%s#", "NumberOfMail");
		return M4_ERROR;
	}

	// Comprobamos el SERVER
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0') ) {

		// Error no se ha pasado el server
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "SERVER");
		return M4_ERROR;
	}

	// Comprobamos el USER
	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar [0] == '\0' ) ) {

		// Error no se ha pasado el user
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "USER");
		return M4_ERROR;
	}

	// Comprobamos el PASSWD
	if (( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[2].Data.PointerChar [0] == '\0' ) ) {
		
		// Error no se ha pasado el Passwd
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "PASSWORD");
		return M4_ERROR;
	}
	
	// Comprobamos el puerto.
	if (ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER || ai_pvArg[3].Data.DoubleData <= 0)
	{
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "PORT" );
		return M4_ERROR;
	}
	else
	{
		uiPort = (m4uint32_t) ai_pvArg[3].Data.DoubleData;
	}

	iRes = NumberPOP3 ( ai_pvArg[0].Data.PointerChar,
						ai_pvArg[1].Data.PointerChar,
						ai_pvArg[2].Data.PointerChar,
						uiPort);

	// Hay error si iRes es negativo, en otro caso será el número de mensajes
	if( iRes < 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "NumberOfMails" );
		}
		return iRes;
	}

	// La función devolverá el número de mensajes si no ha habido error
	return iRes;
}


// Function to delete a mesage from Mind
//
// Params:
//		0. MAIL SERVER		in
//		1. USER				in
//		2. PASSWORD			in
//		3. MSG NUMBER		in
//		4. PORT				in
// Returns:
//		M_4ERROR, M4_SUCCESS

static	m4int32_t	_DeleteMail( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg )
{

	m4int_t		iRes = 0;
	m4uint32_t	uiPort = POP3_DEFAULT_PORT;

	if (ai_iLongArg != 5) {
		// Error numero de parametros incorrecto
		SETCODEF ( M4_MAIL_E_INVALID_NUM_ARG, M4_ERROR, "#*s1*#%s#", "NumberOfMail");
		return M4_ERROR;
	}

	// Comprobamos el SERVER
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0') ) {

		// Error no se ha pasado el server
		SETCODEF(M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "SERVER");
		return M4_ERROR;
	}

	// Comprobamos el USER
	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar [0] == '\0' ) ) {

		// Error no se ha pasado el user
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "USER");
		return M4_ERROR;
	}

	// Comprobamos el PASSWD
	if (( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[2].Data.PointerChar [0] == '\0' ) ) {
		
		// Error no se ha pasado el Passwd
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "PASSWORD");
		return M4_ERROR;
	}

	// Comprobamos el MSG NUM
	// Si es negativo devolvemos error
	if (( ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER) ||
		( ai_pvArg[3].Data.DoubleData <= 0 ) ) {

		// Error no se ha pasado el numero de mensaje
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}

	// Comprobamos el puerto
	if (( ai_pvArg[4].Type != M4CL_CPP_TYPE_NUMBER) || ( ai_pvArg[4].Data.DoubleData <= 0 ) )
	{
		// Error no se ha pasado el puerto
		SETCODEF( M4_MAIL_E_INVALID_ARGUMENT, M4_ERROR, "#*s1*#%s#", "MESSAGE NUMBER");
		return M4_ERROR;
	}
	else
	{
		uiPort = (m4uint32_t) ai_pvArg[4].Data.DoubleData;
	}

	// Vamos a ver si el número de mensaje existe
	iRes = NumberPOP3 ( ai_pvArg[0].Data.PointerChar,
						ai_pvArg[1].Data.PointerChar,
						ai_pvArg[2].Data.PointerChar,
						uiPort);

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Número de mensajes disponibles: " << iRes << "\n";
	#endif

	if( iRes < 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "DeleteMail" );
		}
		return iRes;
	}

	if ( ai_pvArg[3].Data.DoubleData > iRes ) {

		// Error pues el mensaje no existe
		SETCODEF( M4_MAIL_E_MESSAGE_NOT_FOUND, M4_ERROR, "#*s1*#%d#", (m4int_t)(ai_pvArg[3].Data.DoubleData));
		return M4_ERROR;
	}

	// Pues como está, ala a borrarlo
	iRes = DeletePOP3 ( ai_pvArg[0].Data.PointerChar,
						ai_pvArg[1].Data.PointerChar,
						ai_pvArg[2].Data.PointerChar,
						(m4int_t)ai_pvArg[3].Data.DoubleData,
						uiPort);

	if( iRes != 0 )
	{
		// Controlar el tipo de error
		switch( iRes )
		{
			case -10:
				SETCODEF( M4_MAIL_E_INVALID_SERVER, M4_ERROR, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
				break;
			case -6:
			case -12:
				SETCODEF( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#%s#", "" );
				break;
			default:
				SETCODEF( M4_MAIL_E_RECEIVEPOP3, M4_ERROR, "#*s1*#%s#", "DeleteMail" );
		}
		return iRes;
	}

	return M4_SUCCESS;
}




//////////////////////////////////////////////////////////////////
///////// NUMBER OF MAILS
//////////////////////////////////////////////////////////////////

m4int_t NumberPOP3( m4char_t*	ai_pcServer,
					m4char_t*	ai_pcUser,
					m4char_t*	ai_pcPwd,
					m4uint32_t	ai_uiPort)
{

	// Resultado de las operaciones
    m4int_t iRes;
	pop3Client_t * pClient = NULL;
    pop3Sink_t * pSink = NULL;
	
	// Número de mensajes en el buzón
	m4int_t iNumMessages = 0;

	// Initialize the response sink
    iRes = pop3Sink_initialize( &pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Set the function pointers on the response sink
    setSinkPOP3( pSink );

    // Initialize the client passing in the response sink
    iRes = pop3_initialize( &pClient, pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Sets the default timeout
    iRes = pop3_setTimeout( pClient, M4_MAIL_DEFAULT_TIMEOUT );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Connect to the POP3 server
    iRes = pop3_connect( pClient, ai_pcServer, ai_uiPort );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
	
    // Specify a user name
    iRes = pop3_user( pClient, ai_pcUser );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Specify a password
    iRes = pop3_pass( pClient, ai_pcPwd );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }


    // Retrieve the message number
    iRes = pop3_stat( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
	// Sacamos el número de mensajes
	// Se utiliza pOpaqueData para intercambiar datos entre la función 
	// y la callback
	iNumMessages = (m4int_t) (pSink->pOpaqueData);
	
    // Quit
    iRes = pop3_quit( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Free the client structure
    pop3_free( &pClient );
    // Free the sink structure
    pop3Sink_free( &pSink );

	return iNumMessages;
}



//////////////////////////////////////////////////////////////////
///////// DELETE
//////////////////////////////////////////////////////////////////


m4int_t DeletePOP3( m4char_t*	ai_pcServer,
					m4char_t*	ai_pcUser,
					m4char_t*	ai_pcPwd,
					m4int_t		ai_iNum,
					m4uint32_t  ai_uiPort)
{

	// Resultado de las operaciones
    m4int_t iRes;
	pop3Client_t * pClient = NULL;
    pop3Sink_t * pSink = NULL;

    // Initialize the response sink
    iRes = pop3Sink_initialize( &pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Set the function pointers on the response sink
    setSinkPOP3( pSink );

    // Initialize the client passing in the response sink
    iRes = pop3_initialize( &pClient, pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Connect to the POP3 server
    iRes = pop3_connect( pClient, ai_pcServer, ai_uiPort );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Specify a user name
    iRes = pop3_user( pClient, ai_pcUser );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Specify a password
    iRes = pop3_pass( pClient, ai_pcPwd );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Delete the message
    iRes = pop3_delete( pClient, ai_iNum );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Quit
    iRes = pop3_quit( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Free the client structure
    pop3_free( &pClient );
    // Free the sink structure
    pop3Sink_free( &pSink );

	return 0;

}


//////////////////////////////////////////////////////////////////
///////// SEND
//////////////////////////////////////////////////////////////////

// Variable global para controlar los errores de sink
static int s_iSendError = 0;

// Función de callback para capturar los errores de envio de correo
// De momento sólo para la autentificación
void SendError( smtpSinkPtr_t in_psmtpSink, int in_responseCode, const char * in_errorMessage )
{
	s_iSendError = in_responseCode;

	// El de login se trata de manera especial
	if( in_responseCode == 535 )
	{
		SETCODE( M4_MAIL_E_INVALID_USER, M4_ERROR, "#*s1*#" ) ;
	}
	else
	{
		SETCODEF( M4_MAIL_E_SENDDATA, M4_ERROR, "#*s1*#%s#", in_errorMessage ) ;
	}
}



// Funcion que crea un archivo con el mensaje contruido y codificado en formato mime
// Lee los ficheros de los attachments 
// Params:
//		0. FROM
//		1. SENDER
//		2. TO
//		3. REPLY-TO
//		4. CC
//		5. DISPOSITION-NOTIFICATION-TO
//		6. SUBJECT
//		7. BODY
//		8. ATTACHMENTS
//		9. FORMATO SUBTYPE DE CORREO
//		10. FORMATO TYPE DE CORREO
//		11. IMPORTANCE
//		12. MESSAGE-ID
// Returns:
//		0	when OK
//		1	when there is any problem to set an attachment
//		int when ERROR

m4int_t CreateMIME(	m4pchar_t ai_pcFrom,
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
					m4pchar_t ai_pcMessageId)
{ 
	// Resultado de las operaciones
	m4int_t iRes = 0;
	// Índice de cada parte que vamos añadiendo al mensaje
	m4int_t iIndex = 0;
	// Bucles
	m4int_t i = 0;

	// Declaramos el mensaje
	mime_message_t * pMimeMessage = NULL; 
	
	// Contenedor multipart y las partes del mensaje
	mime_multiPart_t * pMultiPart = NULL;
	// Parte básica de texto
	mime_basicPart_t * pBasicPart = NULL;
	
	// SI AÑADIERAMOS LOS FICHEROS A PELO NO NOS HARIAN FALTA:
	// Una parte básica por cada fichero
	mime_basicPart_t ** ppBasicPartFile = NULL;
	
	// Declaramos un stream de entrada por cada fichero
	nsmail_inputstream_t  ** ppIStream = NULL;

	// Número de attachments
	m4int_t		ai_iNumAttachments = 0;
	m4pchar_t	pcAttachment = NULL;

	if (ai_ppcAttachments)
	{
		// Contamos el número de attachments
		for( ai_iNumAttachments = 0; ai_ppcAttachments[ai_iNumAttachments] != NULL; ai_iNumAttachments++);

		ppBasicPartFile = new mime_basicPart_t* [ai_iNumAttachments];
		ppIStream = new nsmail_inputstream_t* [ai_iNumAttachments];
	}

	// y uno de salida para el temporal
	nsmail_outputstream_t *pOStream = NULL;

	// Cabeceras que usaremos
	mime_header_t  * pRFC822_hdrs, * pHdr, *pTmpHdr;
		
	// Asignamos memoria al mensaje
	pMimeMessage = (mime_message_t *) mime_malloc (sizeof (mime_message_t));
	memset (pMimeMessage, 0, sizeof (mime_message_t));
	pMimeMessage->pInternal = NULL;
	
	// Asignamos memoria y Configuramos el contenedor multipart
	pMultiPart = (mime_multiPart_t *) mime_malloc (sizeof (mime_multiPart_t));
	memset (pMultiPart, 0, sizeof (mime_multiPart_t));
	// Lo parametrizamos
	pMultiPart->content_type = MIME_CONTENT_MULTIPART;
	// fix bug 0080713
 	pMultiPart->content_subtype = strdup ("mixed");
	pMultiPart->content_disposition = MIME_DISPOSITION_INLINE;

	// Añadimos las cabeceras al mensaje

	// FROM
	if( ai_pcFrom != NULL )
	{
		pTmpHdr = mime_header_new( "From", ai_pcFrom );
		pRFC822_hdrs = pTmpHdr;
		pHdr = pRFC822_hdrs;
	}
	else
	{
		return -1;
	}

	// SENDER
	if( ai_pcSender != NULL )
	{
		pTmpHdr = mime_header_new( "Sender", ai_pcSender );
		pHdr->next = pTmpHdr;
		pHdr = pHdr->next;
	}

	// TO
	if( ai_ppcTo != NULL )
	{
		for( i = 0; ai_ppcTo[i] != NULL; i++ )
		{
			pTmpHdr = mime_header_new( "To", ai_ppcTo[i] );
			pHdr->next = pTmpHdr;
			pHdr = pHdr->next;
		}
	}

	// REPLY-TO
	if( ai_ppcReplyTo != NULL )
	{
		for( i = 0; ai_ppcReplyTo[i] != NULL; i++ )
		{
			pTmpHdr = mime_header_new( "Reply-To", ai_ppcReplyTo[i] );
			pHdr->next = pTmpHdr;
			pHdr = pHdr->next;
		}
	}

	// CC
	if( ai_ppcCc != NULL )
	{
		for( i = 0; ai_ppcCc[i] != NULL; i++ )
		{
			pTmpHdr = mime_header_new( "Cc", ai_ppcCc[i] );
			pHdr->next = pTmpHdr;
			pHdr = pHdr->next;
		}
	}

	// DISPOSITION-NOTIFICATION-TO
	if( ai_pcDispositionNotificationTo != NULL )
	{
		pTmpHdr = mime_header_new( "Disposition-Notification-To", ai_pcDispositionNotificationTo );
		pHdr->next = pTmpHdr;
		pHdr = pHdr->next;
	}

	// IMPORTANCE
	if( ai_pcImportance != NULL )
	{
		pTmpHdr = mime_header_new( "Importance", ai_pcImportance );
		pHdr->next = pTmpHdr;
		pHdr = pHdr->next;
	}

	// Y el SUBJECT
	if( ai_pcSubject != NULL && ai_pcSubject[0] != '\0' )
	{
		m4pchar_t	pcSubject = NULL;

		// bg 180948
		pcSubject = EncodeHeader(ai_pcSubject, 'Q');
		
		pTmpHdr = mime_header_new ("Subject", pcSubject);
		pHdr->next = pTmpHdr;
		pHdr = pHdr->next;

		delete [] pcSubject ;
	}

	// Metemos la cabecera con la fecha de envío
	time_t tTime;
	time (&tTime);

	/* Bug 0106312
	La hora debe ser en gmt
	*/
	m4char_t * pcTime = mailstrdup (asctime(gmtime( &tTime )));

	/* Bug 0163879
	Se reordena la información de la fecha para que cumpla la especificación RFC2822
	[ day-name "," ] 1*2DIGIT month-name 4*DIGIT 2DIGIT":"2DIGIT[":"2DIGIT] "+"/"-"4DIGIT
		Por ejemplo Sat, 20 May 2000 15:21:51 +0000
	La asctime devuelve en formato Www Mmm dd hh:mm:ss yyyy
		Por ejemplo Sat May 20 15:21:51 2000
	*/
	m4char_t	acDate[ 31 + 1 ];
	size_t		iLength = 0;

	iLength = strlen( pcTime );

	if( iLength >= 24 )
	{
		memset( acDate, ' ', 31 );
		acDate[ 31 ] = '\0';

		memcpy( acDate + 0,  pcTime, 3 );		// Day name
		acDate[ 3 ] = ',';						// Comma
		memcpy( acDate + 5,  pcTime + 8,  2 );	// Day
		memcpy( acDate + 8,  pcTime + 4,  3 );	// Month name
		memcpy( acDate + 12, pcTime + 20, 4 );	// Year
		memcpy( acDate + 17, pcTime + 11, 8 );	// Time
		strcpy( acDate + 26, "+0000" );			// Zone
	}
	else
	{
		// En caso de duda se hace como antes
		pcTime[ iLength - 1 ] = '\0';
		memcpy( acDate, pcTime, iLength );
	}

	delete [] pcTime;

	pTmpHdr = mime_header_new ("Date", acDate);
	pHdr->next = pTmpHdr;
	pHdr = pHdr->next;
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Date añadido\n";
	#endif

	// Cabecera Message-ID.
	if( ai_pcMessageId != NULL )
	{
		pTmpHdr = mime_header_new ("Message-ID", ai_pcMessageId);
		pHdr->next = pTmpHdr;
		pHdr = pHdr->next;
	}
	
	// y se acabaron las cabeceras
	pHdr->next = NULL;

	// Asignamos las cabeceras al mensaje
	pMimeMessage->rfc822_headers = pRFC822_hdrs;

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Cabeceras añadidas al mensaje\n";
	#endif

	// Vamos por el TEXTO del mensaje si hay
	if (( ai_pcText ) && ( ai_pcText[0] != '\0' ))
	{
		// Asignamos memoria a la parte basica mime del texto
		pBasicPart = (mime_basicPart_t *) mime_malloc (sizeof (mime_basicPart_t));
		memset (pBasicPart, 0, sizeof (mime_basicPart_t));
		// La parametrizamos
		pBasicPart->content_disposition = MIME_DISPOSITION_INLINE;
		pBasicPart->content_type = ai_iType;
		// fix bug 0080713

		// sets the format
		pBasicPart->content_subtype = strdup (ai_pcSubType);

		/* Bug 0132041
		Se establece el charset según se esté en ANSI o unicode
		*/
		size_t		iLength = 0 ;
		m4pcchar_t	pccCharSet = NULL ;

		pccCharSet = M4CharSet() ;
		iLength = strlen( "charset=" ) + strlen( pccCharSet ) ;

		pBasicPart->content_type_params = (char*) mime_malloc( iLength + 1 ) ;
		strcpy( pBasicPart->content_type_params, "charset=" ) ;
		strcat( pBasicPart->content_type_params, pccCharSet ) ;

		pBasicPart->encoding_type = MIME_ENCODING_QP;

		// Le asignamos el contenido
		iRes = mime_basicPart_setDataBuf ( pBasicPart, strlen(ai_pcText), ai_pcText, TRUE);

		#ifdef	_M4MAIL_PRINT_DEBUG
			cout << "Texto asignado: " << iRes << "\n";
		#endif

		// Si hay error salimos
		if (iRes != NSMAIL_OK) {
			return iRes;
		}

		// Añadimos la parte mime del texto al contenedor
		iRes = mime_multiPart_addBasicPart( pMultiPart, pBasicPart, TRUE, &iIndex ); 

		#ifdef	_M4MAIL_PRINT_DEBUG
			cout << "BasicPart de texto añadida a MultiPart: " << iRes << "\n";
		#endif

		// Si hay error salimos
		if (iRes != NSMAIL_OK) {
			return iRes;
		}
	}


	// Venga, ahora a por los ATTACHMENTS si hay
	if ( ai_ppcAttachments )
	{
		// Vamos a por uno de los ficheros
		for( m4int_t i=0; ai_ppcAttachments[i] != NULL; i++ )
		{
			// Sacamos su extensión
			file_mime_type fmt;
			m4char_t *pcExt = getFileExt(ai_ppcAttachments[i]);
			iRes = getFileMIMEType (pcExt, &fmt);
			delete [] pcExt;

			if (iRes)
			{
				return iRes;
			}

			/* 
			// PODRÍAMOS AÑADIR LOS ATACHMENTS CON ESTO EN LUGAR
			// DE A PELO, PERO EXISTEN PROBLEMAS CON DETERMINADAS 
			// EXTENSIONES MIME
			iRes = mime_multiPart_addFile (pMultiPart, ai_ppcAttachments[i], fmt.mime_encoding,	&iIndex );
			
			if (iRes) {
				return 1;
			}
			*/

			// Asignamos memoria a la parte mime del fichero
			ppBasicPartFile[i] = (mime_basicPart_t *) mime_malloc (sizeof (mime_basicPart_t));
			memset (ppBasicPartFile[i], 0, sizeof (mime_basicPart_t));
			
			// UNICODE FILE. Bug 0267063
			m4pchar_t pcAttachment = EncodeHeader(ai_ppcAttachments[i], 'Q');

			// Comenzamos a rellenar los parámetros
			// fix bug 0080713
			ppBasicPartFile[i]->content_description = strdup (pcAttachment);
			ppBasicPartFile[i]->content_disposition = MIME_DISPOSITION_ATTACHMENT;
			
			delete [] pcAttachment;

			// Sacamos el nombre del fichero y configuramos con ese nombre el attachment
			// allocates for filename= and the filename itself
			// fix bug 0069273, 0080713 	
			// fix bug 0074679, 0177947
			m4char_t *pcName = getFileName(ai_ppcAttachments[i]);

			m4pchar_t pcDispParam = (m4pchar_t) malloc ( 9 + strlen (pcName) + 1 );
			sprintf(pcDispParam,"%s%s","filename=",pcName);
			ppBasicPartFile[i]->content_disp_params = pcDispParam;
			delete [] pcName;

			// Configuramos el resto de parametros
			ppBasicPartFile[i]->content_type = fmt.content_type;
			ppBasicPartFile[i]->content_subtype = fmt.content_subtype;
			
			// Si la codificacioón que sacamos es 7 bits o 8 bits pasamos de ella
			// y usamos QP
			if (fmt.mime_encoding == MIME_ENCODING_7BIT || fmt.mime_encoding == MIME_ENCODING_8BIT)
			{
				ppBasicPartFile[i]->encoding_type = MIME_ENCODING_QP;
			}
			else 
			{
			// Si no usamos la que se nos de
				ppBasicPartFile[i]->encoding_type =  fmt.mime_encoding;
			}

			ppBasicPartFile[i]->content_description = NULL;
			ppBasicPartFile[i]->content_type_params = NULL;
			ppBasicPartFile[i]->contentMD5 = NULL;
			
			// Creamos un stream de entrada para leer el fichero
			// UNICODE FILE. Bug 0267063
			int iLength = -1;
			char * pcAttachmentFile = M4CppToANSI(ai_ppcAttachments[i], iLength);
			iRes = file_inputStream_create( pcAttachmentFile, &ppIStream[i] );

			if (iRes != NSMAIL_OK) 
			{
				SETCODEF (M4_MAIL_E_READATTACHMENT,M4_ERROR,"#*s1*#%s#", pcAttachmentFile);
				delete [] pcAttachmentFile;
				return iRes;
			}

			delete [] pcAttachmentFile;

			// Leemos los datos del fichero y los metemos en la parte mime
			iRes = mime_basicPart_setDataStream( ppBasicPartFile[i], ppIStream[i], TRUE); 

			if (iRes) {
				return iRes;
			}

			// Añadimos esta parte mime al contenedor
			iRes = mime_multiPart_addBasicPart( pMultiPart, ppBasicPartFile[i], TRUE, &iIndex ); 

			if (iRes) {
				return iRes;
			}
				
			#ifdef	_M4MAIL_PRINT_DEBUG
				cout << "Attachment añadido al mime: " << iRes << "\n";
			#endif
		}
	}
	
	// Asignamos el contenedor al mensaje
	iRes = mime_message_addMultiPart( pMimeMessage, pMultiPart, TRUE );

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Multipart añadido a Message: " << iRes << "\n";
	#endif

	if (iRes) 
		return iRes;


	// Sacamos todo a un fichero temporal
	iRes = file_outputStream_create (g_pcFile, &pOStream);

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Output stream creado: " << iRes << "\n";
	#endif

	if (iRes) {
		SETCODEF (M4_MAIL_E_WRITETEMP, M4_ERROR, "#*s1*#%s#", g_pcFile);
		return iRes;
	}

	// Codificamos el mensaje en el fichero de salida
	iRes = mime_message_putByteStream ( pMimeMessage,
										pOStream);

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Stream de salida relleno: " << iRes << "\n";
	#endif

	if (iRes) 
		return iRes;
	
	// Cerramos los stream y liberamos todo lo usado
	pOStream->close (pOStream->rock);
	nsStream_free (pOStream); 


	/* 
	// SI NO LOS AÑADIERAMOS A PELO NO NOS HARÍA FALTA ESTA LIBERACIÓN
	*/
	for (i=0; i < ai_iNumAttachments; i++ ) {
		mime_basicPart_free_all (ppBasicPartFile[i]);
		ppIStream[i]->close (ppIStream[i]->rock);
		nsStream_free (ppIStream[i]);
	}

	mime_basicPart_free_all (pBasicPart);
	mime_multiPart_free_all  (pMultiPart);
	mime_message_free_all (pMimeMessage);

	return 0;
}


// Funcion para enviar mediante SMTP un mensaje que se encuentra ya
// codificado en mime en un fichero
// Params:
//		1. MAIL SERVER
//		2. MAIL PORT
//		3. CUENTA
//		4. TO
//		5. NUMERO DE ELEMENTOS TO
// Returns:
//		0	when OK
//		int when ERROR
//
m4int_t SendSMTP (  m4pchar_t ai_pcServer,
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
					m4double_t ai_dTimeOut)
{
    
	// Resultado de las operacioones
	m4int_t iRes = 0;
	// Estructura cliente
    smtpClient_t * pClient = NULL;
	// Estructura Sink, para controlar las callbacks
    smtpSink_t * pSink = NULL;
	// Stream del fichero de entrada
	nsmail_inputstream_t *pIStream;

	m4int_t i=0;
	m4char_t pcHelloString[80];
	char *pcBuffer = NULL;
	m4pchar_t pcNotifications = NULL;
	C_Base64 oEncoder;
	

	// Initialize the response sink
    iRes = smtpSink_initialize( &pSink );
    if ( iRes != NSMAIL_OK )
            return iRes;
    
    // Set the function pointers on the response sink
    setSinkSMTP( pSink );

    // Initialize the client passing in the response sink
    iRes = smtp_initialize( &pClient, pSink );
    if ( iRes != NSMAIL_OK )
            return iRes;
    
    // Set the internal buffer chunk size
    iRes = smtp_setChunkSize( pClient, 1024 );
    if ( iRes != NSMAIL_OK )
            return iRes;
    

	// Fix bug 0183909: adds support to timeout
	iRes = smtp_setTimeout( pClient, ai_dTimeOut );
    if ( iRes != NSMAIL_OK )
            return iRes;

    // Connect to the SMTP server
    iRes = smtp_connect( pClient, ai_pcServer, ai_iPort );
    if ( iRes != NSMAIL_OK )
			return iRes;
    
    iRes = smtp_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
	{	
		goto RETURN;
	}


    // Send the EHLO command
//  iRes = smtp_ehlo( pClient, ai_pcHelloString );
//  if ( iRes != NSMAIL_OK )
//	{
//		smtp_quit( pClient );
//		smtp_disconnect( pClient );
//		return iRes;
//	}
    // Send the EHLO command
	sprintf (pcHelloString, "%s %s", "EHLO", ai_pcHelloString);
    iRes = smtp_sendCommand( pClient, pcHelloString );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}
     
    iRes = smtp_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}


	// Se hace la autentificación si viene usuario y password
	if( ai_pcUser != NULL && ai_pcPassword != NULL )
	{
		// Registramos la función de errores
		s_iSendError = 0 ;
		pSink->error = SendError;

		iRes = smtp_sendCommand( pClient, "AUTH LOGIN" );
		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}
     
		iRes = smtp_processResponses( pClient );
		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}

		oEncoder.Code( ai_pcUser, strlen( ai_pcUser ), pcBuffer ) ;
		iRes = smtp_sendCommand( pClient, pcBuffer );
		delete pcBuffer;
		pcBuffer = NULL;

		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}
     
		iRes = smtp_processResponses( pClient );
		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}

		oEncoder.Code( ai_pcPassword, strlen( ai_pcPassword ), pcBuffer ) ;

		iRes = smtp_sendCommand( pClient, pcBuffer );
		delete pcBuffer;
		pcBuffer = NULL;

		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}
     
		iRes = smtp_processResponses( pClient );
		if ( iRes != NSMAIL_OK || s_iSendError != 0 )
		{
			goto RETURN;
		}
	}


    // Send the MAIL FROM command
	if( ai_pcReturnPath != NULL )
	{
		ai_pcFrom = ai_pcReturnPath;
	}

    iRes = smtp_mailFrom( pClient, ai_pcFrom, NULL );
    if ( iRes != NSMAIL_OK ) 
	{
		goto RETURN;
	}
    
    iRes = smtp_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}
    
	if( ai_bReturnReceipt == M4_TRUE )
	{
		// Bug 0262096. Enviar correo de notifiación de éxito o fallo siempre.
		pcNotifications = " NOTIFY=SUCCESS,FAILURE";
	}

	// Send the RCPT TO command
	// Primero los TO
	if ( ai_ppcTo != NULL )
	{
		for( i = 0; ai_ppcTo[i] != NULL; i++ )
		{
			iRes = smtp_rcptTo( pClient, ai_ppcTo[i], pcNotifications );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
			
			iRes = smtp_processResponses( pClient );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
		}
	}
    
	// Ahora los CC
	if ( ai_ppcCc != NULL )
	{
		for( i = 0; ai_ppcCc[i] != NULL; i++ )
		{
			iRes = smtp_rcptTo( pClient, ai_ppcCc[i], pcNotifications );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
			
			iRes = smtp_processResponses( pClient );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
		}
	}

	// Ahora los BCC
	if ( ai_ppcBcc != NULL )
	{
		for( i = 0; ai_ppcBcc[i] != NULL; i++ )
		{
			iRes = smtp_rcptTo( pClient, ai_ppcBcc[i], pcNotifications );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
			
			iRes = smtp_processResponses( pClient );
			if ( iRes != NSMAIL_OK )
			{
				goto RETURN;
			}
		}
	}

	// //Send the EXPN command
    // iRes = smtp_expand( pClient, ai_pcTo );
	// if ( iRes != NSMAIL_OK )
	//	{
	//		goto RETURN;
	//	}
    //         return iRes;
    // iRes = smtp_processResponses( pClient );
    // if ( iRes != NSMAIL_OK )
	//	{
	//		goto RETURN;
	//	}    

    // Send the DATA command
    iRes = smtp_data( pClient );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}

    iRes = smtp_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}

    
    // Creamos el stream
	iRes = file_inputStream_create (g_pcFile, &pIStream);
	if (iRes != NSMAIL_OK) 
	{	
		goto RETURN;
	}
		

    // Send the message
    iRes = smtp_sendStream( pClient, pIStream );

	// Bg 0131917
    // Cerramos y liberamos el stream
	pIStream->close (pIStream->rock);
	nsStream_free (pIStream); 

	if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}

	iRes = smtp_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
	{
		goto RETURN;
	}

RETURN:
    
	// Cerramos la conexión con el servidor
	smtp_quit( pClient );
	smtp_disconnect( pClient );
    
	// Free the client structure
    smtp_free( &pClient );
    // Free the sink structure
    smtpSink_free( &pSink );

    return iRes;
}



// Funcion para obtener la extension de un fichero
// Params:
//		0. CADENA CON EL FICHERO
// Returns:
//		CADENA CON LA EXTENSION
//
m4char_t * getFileExt (m4char_t * filePath)
{
        m4char_t* pCh = NULL;
        m4int_t i = 0;
		m4int_t len = 0;
 
        if ( filePath )
        {
                len = strlen (filePath);
                if (len <= 2)
                     return NULL;
 
                pCh = filePath + len;
 
                for (i = len-1; i >= 2; i--)
                {
                        if (filePath [i] == '.')
                        {
                           return mailstrdup (pCh);
                        }
 
                        pCh--;
                }
 
        }
        return NULL;
}


// Function to set the SMTP sink pointers

void setSinkSMTP( smtpSink_t * pSink )
{
    pSink->bdat = SMTP_bdat;
    pSink->connect = SMTP_connect;
    pSink->data = SMTP_data;
    pSink->ehlo = SMTP_ehlo;
    pSink->ehloComplete = SMTP_ehloComplete;
    pSink->expand = SMTP_expand;
    pSink->expandComplete = SMTP_expandComplete;
    pSink->help = SMTP_help;
    pSink->helpComplete = SMTP_helpComplete;
    pSink->mailFrom = SMTP_mailFrom;
    pSink->noop = SMTP_noop;
    pSink->quit = SMTP_quit;
    pSink->rcptTo = SMTP_rcptTo;
    pSink->reset = SMTP_reset;
    pSink->send = SMTP_send;
    pSink->sendCommand = SMTP_sendCommand;
    pSink->sendCommandComplete = SMTP_sendCommandComplete;
    pSink->verify = SMTP_verify;
}



// JUAN RAMON DIAZ POL
//
// Funcion que desglosa una cadena con elementos separados por un token en un array con 
// cada elemento en una posición y acabado en NULL
//	
// Argumentos:
//		1. Cadena con las palabras separadas por un caracter
//		separador definido en el segundo parametro. (char *)
//		2. Elemento de separacion (char *).
//			Debe ser de longitud 1,
//			sólo se reconocen separadores de longitud 1.
//
// Salida:
//		3. Array donde se guardan los elementos acabado 
//		en NULL (char**)

m4pchar_t* Parse( m4pchar_t pcCad, const m4pchar_t pcSep, m4bool_t ai_bToANSI)
{	
	m4pchar_t vacia = " ";
	
	// Array resultado
	m4pchar_t* ppcRes;

	// Contamos el número de elementos
	m4int_t iNumElem = 1;  //Al menos tenemos uno
	m4int_t cont_nulos;
	m4int_t id_valido = 0;

	if (strlen(pcCad) != 0) 
		for (m4uint_t i=0; i<strlen(pcCad); i++){
			
			if ((pcCad[i]!=' ')&&(pcCad[i]!= pcSep[0] )) {
				id_valido=1;
			}
			
			if ( (pcCad[i]== pcSep[0] ) && 
			   ( (pcCad[i+1]!= pcSep[0] ) && (id_valido==1) &&
					((i+1)!=strlen(pcCad) )	) ) {
					
					iNumElem++;
			}
		}
		
	if (id_valido==1) {
		// Asignamos memoria al array
		ppcRes = new m4pchar_t [iNumElem + 1];

		m4pchar_t pcToken;

		pcToken = strtok( pcCad, pcSep );
	
	
		for (m4int_t j = 0; pcToken != NULL; j++ )	{

			cont_nulos=0;		
			if (pcToken[0]==' ') {
				cont_nulos++;			
				for (m4int_t k = 1; k<=(int)strlen(pcToken); k++ ) {
					if (pcToken[k]==' '){
						cont_nulos++;
					}
					else k=strlen(pcToken)+1;
				}
			
				if (cont_nulos==(int)strlen(pcToken)){
					j--;
					iNumElem--;
				}
			
			}
		
			if (cont_nulos!=(int)strlen(pcToken)){
			
				// Asignamos memoria a la cadena
				int iLength = strlen(pcToken) ;

				/* Bug 0190403
				Los ficheros deben ser ANSI
				*/
				if( ai_bToANSI == M4_TRUE )
				{
					ppcRes[j] = M4CppToANSI( pcToken, iLength ) ;
				}
				else
				{
					ppcRes[j] = new m4char_t[ iLength + 1 ];
					memcpy( ppcRes[j], pcToken, iLength + 1 ) ;
				}
			}	
			
			pcToken = strtok( NULL, pcSep );
	
		}

		// metemos el NULL al final
		ppcRes[iNumElem] = NULL;
	}
	
	else {

		ppcRes = NULL;
	}

	return ppcRes;

}


// Bg 139211
m4bool_t CheckAccounts(const m4pcchar_t* ai_ppcAccounts)
{
	m4bool_t bRet = M4_TRUE;
	int iAccIdx = 0;
	int iLength = 0;
	int iAtSymbolPos = 0;
	int iDotSymbolPos = 0;

	/* Bug 0254063
	Si no hay cuentas no hay que fallar
	*/
	if( ai_ppcAccounts == NULL )
	{
		return M4_TRUE;
	}

	while(ai_ppcAccounts[iAccIdx] != NULL)
	{
		iLength = strlen(ai_ppcAccounts[iAccIdx]);
		iAtSymbolPos = 0;
		iDotSymbolPos = 0;

		for(int iPos = 0; iPos < iLength; iPos++)
		{
			if(ai_ppcAccounts[iAccIdx][iPos] == '@')
			{
				if(iAtSymbolPos != 0)
				{
					// Mas de una arroba y nos vamos
					SETCODEF (M4_MAIL_E_ADDRESSERROR, M4_ERROR, "#*s1*#%s#", ai_ppcAccounts[iAccIdx]);
					return M4_FALSE;
				}

				iAtSymbolPos = iPos;
			}
			if(ai_ppcAccounts[iAccIdx][iPos] == '.')
			{
				iDotSymbolPos = iPos;
			}
		}

		if(iAtSymbolPos == 0 || (iAtSymbolPos > iDotSymbolPos))
		{
			// No hay punto después de la arroba y nos vamos
			SETCODEF (M4_MAIL_E_ADDRESSERROR, M4_ERROR, "#*s1*#%s#", ai_ppcAccounts[iAccIdx]);
			return M4_FALSE;
		}

		iAccIdx++;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////
///////// RECEIVE 
//////////////////////////////////////////////////////////////////


//Function to set the sink pointers.
void setSinkPOP3( pop3Sink_t * pSink )
{
    pSink->connect = POP3_connect;
    pSink->dele = POP3_delete;
    pSink->error = POP3_error;
    pSink->listStart = POP3_listStart;
    pSink->list = POP3_list;
    pSink->listComplete = POP3_listComplete;
    pSink->noop = POP3_noop;
    pSink->pass = POP3_pass;
    pSink->quit = POP3_quit;
    pSink->reset = POP3_reset;
    pSink->retrieveStart = POP3_retrieveStart;
    pSink->retrieve = POP3_retrieve;
    pSink->retrieveComplete = POP3_retrieveComplete;
    pSink->sendCommandStart = POP3_sendCommandStart;
    pSink->sendCommand = POP3_sendCommand;
    pSink->sendCommandComplete = POP3_sendCommandComplete;
    pSink->stat = POP3_stat;
    pSink->topStart = POP3_topStart;
    pSink->top = POP3_top;
    pSink->topComplete = POP3_topComplete;
    pSink->uidListStart = POP3_uidListStart;
    pSink->uidList = POP3_uidList;
    pSink->uidListComplete = POP3_uidListComplete;
    pSink->user = POP3_user;
    pSink->xAuthListStart = POP3_xAuthListStart;
    pSink->xAuthList = POP3_xAuthList;
    pSink->xAuthListComplete = POP3_xAuthListComplete;
    pSink->xSender = POP3_xSender;
}



// Funcion para obtener la extension de un fichero
// Params:
//		0. CADENA CON EL FICHERO
// Returns
//		CADENA CON EL NOMBRE
//
m4char_t * getFileName (m4char_t * filePath, m4char_t cSep)
{
	m4pchar_t	pcName = NULL;

	// Quitamos las posibles \" de la cadena
	for (m4int_t j = strlen (filePath); j >= 0; j--)
		if ( filePath[j] == '\"' )
			strcpy (filePath + j, filePath + j + 1);
		
	// Sacamos el nombre			
	m4char_t * pCh = NULL;
    m4int_t len = 0;
	m4int_t i = 0;
		
	if ( filePath )
    {
		len = strlen (filePath);
			
		pCh = filePath + len;
		
		for (i =len-1; i >= 0; i--)
		{
			if (filePath [i] == cSep)
			{
				// UNICODE.
				pcName = EncodeHeader(pCh, 'Q');
				return pcName;
			}	
			
			pCh--;
		}
		
		// UNICODE.
		pcName = EncodeHeader(filePath, 'Q');
		return pcName;
	}
	
	return NULL;
}

m4char_t * getFileName (m4char_t * filePath)
{
	m4pchar_t	pcName = NULL;

	// Quitamos las posibles \" de la cadena
	for (m4int_t j = strlen (filePath); j >= 0; j--)
		if ( filePath[j] == '\"' )
			strcpy (filePath + j, filePath + j + 1);
		
		// Sacamos el nombre			
		m4char_t * pCh = NULL;
        m4int_t len = 0;
		m4int_t i = 0;
		
        if ( filePath )
        {
			len = strlen (filePath);
			
			pCh = filePath + len;
			
			for (i =len-1; i >= 0; i--)
			{
				if (filePath [i] == '\\' || filePath [i] == '/')
				{
					// UNICODE.
					pcName = EncodeHeader(pCh, 'Q');
					return pcName;
				}
				
				pCh--;
			}
			
			// UNICODE.
			pcName = EncodeHeader(filePath, 'Q');
			return pcName;
        }

		return NULL;
}


// Funcion para recibir un mensaje mediante POP3
// Params:
//		0. MAIL SERVER
//		1. USER
//		2. PASSWORD
//		3. NUM MSG
//		4. PORT
// Returns
//		0 when OK
//		int when ERROR
//
m4int_t ReceivePOP3( m4char_t*	ai_pcServer,
					 m4char_t*	ai_pcUser,
					 m4char_t*	ai_pcPwd,
					 m4int_t	ai_iNumMsg,
					 m4uint32_t ai_iPort)
{

    m4int_t iRes;
    pop3Client_t * pClient = NULL;
    pop3Sink_t * pSink = NULL;

    // Initialize the response sink
    iRes = pop3Sink_initialize( &pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Set the function pointers on the response sink
    setSinkPOP3( pSink );

    // Initialize the client passing in the response sink
    iRes = pop3_initialize( &pClient, pSink );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Initialize the default timeout
    iRes = pop3_setTimeout( pClient, M4_MAIL_DEFAULT_TIMEOUT );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Connect to the POP3 server
    iRes = pop3_connect( pClient, ai_pcServer, ai_iPort );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Specify a user name
    iRes = pop3_user( pClient, ai_pcUser );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Specify a password
    iRes = pop3_pass( pClient, ai_pcPwd );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Retrieve a message
    iRes = pop3_retrieve( pClient, ai_iNumMsg );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Quit
    iRes = pop3_quit( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }
    iRes = pop3_processResponses( pClient );
    if ( iRes != NSMAIL_OK )
    {
        return iRes;
    }

    // Free the client structure
    pop3_free( &pClient );
    // Free the sink structure
    pop3Sink_free( &pSink );

	return 0;
}



void free_this_body_part (void * pBodyPart, mime_content_type contentType)
{
	#ifdef	_M4MAIL_PRINT_DEBUG
			cout << "Parte del tipo: " << contentType << " liberada\n";
	#endif

	switch (contentType)
	{
		case MIME_CONTENT_TEXT:
		case MIME_CONTENT_AUDIO:
		case MIME_CONTENT_IMAGE:
		case MIME_CONTENT_VIDEO:
		case MIME_CONTENT_APPLICATION:
			mime_basicPart_free_all ((mime_basicPart_t *)pBodyPart);
			break;
		case MIME_CONTENT_MULTIPART:
			mime_multiPart_free_all ((mime_multiPart_t *)pBodyPart);
			break;
		case MIME_CONTENT_MESSAGEPART:
			mime_messagePart_free_all ((mime_messagePart_t *)pBodyPart);
			break;
	}

}

m4return_t _InsertHeader(headersMap_t & ao_oHeaders, const char * ai_pcHeaderName, const char * ai_pcHeaderValue, const char * ai_pcHeaderCharset)
{
	headersMap_t::iterator		iterator;
	char*						pHeaderValue = NULL;
	
	// Decode header value.
	pHeaderValue = decode_rfc2047_header(ai_pcHeaderValue, ai_pcHeaderCharset);
	
	// Se busca si ya hemos obtenido valor para esta cabecera.
	iterator = ao_oHeaders.find(ai_pcHeaderName);
	if (iterator == ao_oHeaders.end())
	{
		ao_oHeaders.insert(headersMap_t::value_type(ai_pcHeaderName, pHeaderValue));
	}
	else
	{
		(*iterator).second.append(";");
		(*iterator).second.append(pHeaderValue);
	}
	
	delete[] pHeaderValue;

	return M4_SUCCESS;
}


m4int_t walkBodyPart (  m4void_t * pBody, 
					    mime_content_type contentType,
						m4char_t * & ao_pcFrom, 
						m4char_t * & ao_pcTo,
						m4char_t * & ao_pcCc,
						m4char_t * & ao_pcSubject,
						m4char_t * & ao_pcText,
						m4char_t * & ao_pcAttachments,
						headersMap_t & ao_oHeaders )
{
	mime_basicPart_t *		pBasicPart = NULL;
	mime_multiPart_t *		pMultiPart = NULL;
	mime_messagePart_t *	pMessagePart = NULL;
	mime_message_t *		pLocalMessage = NULL;
	m4uint_t				body_len = 0;
	m4int_t i,				iRes = 0, part_count=0;
	m4void_t *				pBodyPart = NULL;
	static m4int_t			iCount = 0;
	m4uint_t				iLen = 0;
	int						iLength = -1;

	mime_content_type		cType;
	m4char_t				*pcBuf = NULL;
	m4char_t				*pContentTypeParams = NULL;
	m4char_t				*pCharsetStart = NULL;
	m4char_t				*pCharsetEnd = NULL;
	m4char_t				*pCharset = NULL;
	FILE					* fd = NULL;

	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Parseo de una parte del tipo: " << contentType << "\n";
	#endif

	switch (contentType)
	{
		// content type devuelve TEXT para todos estos tipos
		case MIME_CONTENT_TEXT:
		case MIME_CONTENT_AUDIO:
		case MIME_CONTENT_IMAGE:
		case MIME_CONTENT_VIDEO:
		case MIME_CONTENT_APPLICATION:

			pBasicPart = (mime_basicPart_t *) pBody;
			iCount++;

			/*
			 * bug: 0068407
			 * En algunos casos, ver bug 0068407, no se ha parseado bien el body.
			 * Se obtiene que mime=MIME_CONTENT_APPLICATION, pero la estructura no
			 * se ha inicializado bien y tenemos null values en el objeto pBasicPart,
			 * lo que nos provoca un core.
			 */
			if (pBasicPart == NULL)
			{
				SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart=null.");				
				return 1;
			}

			iRes = mime_basicPart_getDataBuf (
						(mime_basicPart_t *) pBody,
						&iLen, 
						&pcBuf); 
			#ifdef	_M4MAIL_PRINT_DEBUG
				cout << "Sacamos los datos de la parte: " << iRes << "\n";
			#endif

						
			if ((pBasicPart->content_type == MIME_CONTENT_TEXT) &&
				(pBasicPart->content_disposition != MIME_DISPOSITION_ATTACHMENT))
			{
				// Quitamos los dos retornos de carro que llegan al final del mensaje
				// siempre
				if (pcBuf != 0)
				{
					// En ciertos casos, con Notes está pasando, aunque me imagino que en UNIX
					// también pasará, 2 retornos de carro no son 4 caracteres.
					//		pcBuf[strlen(pcBuf)-4] = '\0';
					// Obtener charset del Body, por ejemplo: charset="iso-8859-1"
					if (pBasicPart->content_type_params != NULL)
					{
						pContentTypeParams = strstr(pBasicPart->content_type_params, "charset=");
						if (pContentTypeParams == NULL)
						{
							return NULL;
						}
						
						pCharsetStart = strchr(pContentTypeParams, '"');
						if (pCharsetStart == NULL)
						{
							return NULL;
						}
						
						*pCharsetStart = '\0';
						pCharsetStart++;
						
						pCharsetEnd = strchr(pCharsetStart, '"');
						if (pCharsetEnd == NULL)
						{
							return NULL;
						}
						
						*pCharsetEnd = '\0';
						
						iLen = pCharsetEnd - pCharsetStart;
						pCharset= new char[iLen + 1];
						strncpy(pCharset, pCharsetStart, iLen);
						pCharset[iLen] = '\0';
					}

					_InsertHeader(ao_oHeaders, "Body", pcBuf, pCharset);
					
					ao_pcText = mailstrdup (pcBuf);
					
					delete [] pCharset;
					pCharset = NULL;

					mime_memfree(pcBuf);
					pcBuf = NULL;
				}
			}
			else 
			{
				// A ver si encontramos el nombre del fichero
				m4char_t *	pcFileNameTmpEncoded = NULL;
				m4char_t *	pcFileNameTmp = NULL;
				
				/* ver otros comentarios para bug: 0068407 */
				// Bug 0266556, 0266553
				if (pBasicPart->content_type == MIME_CONTENT_IMAGE)
				{
					// En las partes que son de tipo imagen, a veces viene relleno el nombre en content_type_params y 
					// a veces en content_type_params.
					if (pBasicPart->content_disp_params == NULL && pBasicPart->content_type_params == NULL)
					{
						SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_disp_params=null and mime_basicPart.content_type_params=null.");
						return 1;
					}
				}
				else if (pBasicPart->content_disp_params == NULL)
				{
					// Los partes que son de tipo MIME_CONTENT_APPLICATION pueden venir vacias.
					if (pBasicPart->content_type != MIME_CONTENT_APPLICATION)
					{
						SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_disp_params=null.");
						return 1;
					}
				}

				/* Bug 0149888
				Hay que seleccionar la parte del nombre del fichero
				*/
				m4pchar_t pcFileNameStart = NULL;
				if (pBasicPart->content_disp_params != NULL)
				{
					pcFileNameStart = strstr( pBasicPart->content_disp_params, "filename=" ) ;
				}
				else if (pBasicPart->content_type_params != NULL)
				{
					pcFileNameStart = strstr( pBasicPart->content_type_params, "name=" ) ;
				}

				if (pcFileNameStart == NULL)
				{
					SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_disp_params.filename=null.");
					return 1;
				}
				
				// Buscar las comillas dobles que delimitan el inicio y final del nombre de fichero.
				// Content-Disposition: attachment;
				// filename="midocumento.doc"; size=54412;
				// creation-date="Thu, 26 Mar 2009 08:43:50 GMT";
				// modification-date="Thu, 26 Mar 2009 08:43:50 GMT"
				m4pchar_t	pcFileStart = strchr( pcFileNameStart, '"' ) ;
				if (pcFileStart == NULL)
				{
					SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_disp_params.filename=missing doble quotes.");
					return 1;
				}
				
				*pcFileStart = '\0';
				pcFileStart++;

				m4pchar_t pcFileEnd = strchr( pcFileStart, '"' ) ;				
				if (pcFileEnd == NULL)
				{
					SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_disp_params.filename=missing doble quotes.");
					return 1;
				}

				*pcFileEnd = '\0';

				int iFileNameLen = pcFileEnd - pcFileStart;
				
				m4pchar_t pcFileName = new char[iFileNameLen + 1];
				strncpy(pcFileName, pcFileStart, iFileNameLen);
				pcFileName[iFileNameLen] = '\0';
				
				// Bug 0266551. En elgunos casos llegan retornos de carro en el nombre del documento.
				// Se reemplazan por "_".
				for (i = 0; i < iFileNameLen; i++)
				{
					if (pcFileName[i] == '\r' || pcFileName[i] == '\n')
					{
						pcFileName[i] = '_';
					}
				}

				if (pcFileName == NULL || strcmp(pcFileName, "") == 0)
				{
					// si no lo encontramos usamos la descripción
					pcFileNameTmpEncoded = mailstrdup ( g_pcTempDir );
					ConcatString (pcFileNameTmpEncoded, M4_BACK_SLASH);

					/* ver otros comentarios para bug: 0068407 */
					if (pBasicPart->content_description == NULL)
					{
						SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_basicPart.content_description=null.");
						
						delete pcFileName;
						pcFileName = NULL;
						
						delete pcFileNameTmpEncoded;
						pcFileNameTmpEncoded = NULL;

						return 1;
					}

					ConcatString (pcFileNameTmpEncoded, pBasicPart->content_description);
				}
				else
				{
					pcFileNameTmpEncoded = mailstrdup ( g_pcTempDir );
					ConcatString (pcFileNameTmpEncoded, M4_BACK_SLASH);
					ConcatString (pcFileNameTmpEncoded, pcFileName);
				}

				// Obtener el charset con el que fue codificado el nombre del fichero.
				pCharset = getHeaderCharset(pcFileName);
				
				/* bug 0082181. Hay que borrar con delete, ya que se alloca con new. */
				delete pcFileName;
				pcFileName = NULL;
				
				// Bug 0266546. Decodificar el nombre del fichero.
				iLength = strlen(pcFileNameTmpEncoded);
				pcFileNameTmp = new m4char_t[iLength + 1];
				memset(pcFileNameTmp, 0, iLength + 1);
				mime_decodeHeaderString(pcFileNameTmpEncoded, &pcFileNameTmp);

				delete pcFileNameTmpEncoded;
				pcFileNameTmpEncoded = NULL;
					
				// Bug 0267063. Convertir el nombre del fichero atendiendo al charset.
				if (pCharset != NULL)
				{
					if (strcmpi(pCharset, "iso-8859-1") == 0 || strcmpi(pCharset, "windows-1252") == 0)
					{
						// Unicode.
						iLength = -1;
						pcFileName = M4ANSIToCpp(pcFileNameTmp, iLength);
					}
					else if (strcmpi(pCharset, "UTF-8") == 0)
					{
						// Unicode.
						iLength = -1;
						pcFileName = M4Utf8ToCpp(pcFileNameTmp, iLength);
					}
					else
					{
						pcFileName = mailstrdup(pcFileNameTmp);
					}
				}
				else
				{
					pcFileName = mailstrdup(pcFileNameTmp);
				}
				
				delete pCharset;
				pCharset = NULL;

				delete pcFileNameTmp;
				pcFileNameTmp = NULL;

				// bugid: 0069775.
				// Tengo que comprobar si el fichero ya existe. Si es así, lo renombro.
				GetSafeFileName(pcFileName);

				#ifdef	_M4MAIL_PRINT_DEBUG
					cout << "Nombre del fichero temporal creado: " << pcFileName << "\n";
				#endif
				
				// UNICODE FILE
				eUniFileEncodingType_t eEncoding = M4UniANSI ;
				fd = M4Fopen ( pcFileName, M4UniWriteBinary, eEncoding);
				if (fd) {
					m4uint_t i = 0;
					for ( i = 0; i < iLen; i++) 
						iRes  = fputc (pcBuf[i], fd);
					fclose (fd);

					if ( ! ao_pcAttachments ) {
						ao_pcAttachments = mailstrdup (pcFileName);
					}
					else {
						ConcatString (ao_pcAttachments, "*");
						ConcatString (ao_pcAttachments, pcFileName);
					}
				}
				else {
					// Error: No podemos guardar uno de los attachments a recibir
					SETCODEF ( M4_MAIL_E_WRITEATTACHMENT, M4_ERROR, "#*s1*#%s#", pcFileName);
					/* bug 0082181. Hay que borrar con delete, ya que se alloca con new. */
					delete pcFileName;
					pcFileName = NULL;
					return 1;
				}
					
				delete pcFileName;
				pcFileName = NULL;
			}

			// When done with the basicPart itself, free it also 
//			mime_basicPart_free_all (pBasicPart);
//			pBasicPart = NULL;

			break;
	
		case MIME_CONTENT_MULTIPART:
	
			pMultiPart = (mime_multiPart_t *) pBody;

			/* ver otros comentarios para bug: 0068407 */
			if (pMultiPart == NULL)
			{
				SETCODEF (M4_MAIL_E_PARSEMIME, M4_ERROR, "#*s1*#%s#", "Incomming mail with mime_multiPart=null.");				
				return 1;
			}
			
			iRes = mime_multiPart_getPartCount (pMultiPart, &part_count);
			if (iRes != MIME_OK)
			{
				return iRes;
			}

			for (i = 1; i <= part_count; i++)
			{
				iRes = mime_multiPart_getPart (pMultiPart, i, &cType, &pBodyPart);
				if (iRes != MIME_OK)
				{
					return iRes;
				}

				iRes = walkBodyPart (	pBodyPart, cType,
									ao_pcFrom, 
									ao_pcTo,
									ao_pcCc,
									ao_pcSubject,
									ao_pcText,
									ao_pcAttachments,
									ao_oHeaders );
				if (iRes != MIME_OK) {
					return iRes;
				}


				// Free the part when done 
				free_this_body_part (pBodyPart, cType);
				pBodyPart = NULL;

			}

			// free the multi-part 
//			mime_multiPart_free_all (pMultiPart);
//			pMultiPart = NULL;
			break;

		case MIME_CONTENT_MESSAGEPART:
	
			pMessagePart = (mime_messagePart_t *) pBody;
			
			// Los mensajes de este tipo cuyo subtipo son delivery-status no tienen
			// pLocalMessage.
			if (pMessagePart != 0)
			{
				if (strcmp(pMessagePart->content_subtype, "delivery-status") != 0)
				{
					// get the message that is the body of this message-part 
					iRes = mime_messagePart_getMessage (pMessagePart, &pLocalMessage);
					
					if (iRes) 
					{
						return iRes;
					}
					
					walkMessage (	pLocalMessage,
						ao_pcFrom, 
						ao_pcTo,
						ao_pcCc,
						ao_pcSubject,
						ao_pcText,
						ao_pcAttachments,
						ao_oHeaders );

					// free up the message and messagePart when done 
					mime_message_free_all (pLocalMessage);
					pLocalMessage = NULL;
				}				
			}
			// handle other sub-types as needed 
	
			// free the message-part when done 
//			mime_messagePart_free_all (pMessagePart);
//			pMessagePart = NULL;
	
			break;
	}

	// Todo bien
	return 0;
}

m4int_t walkMessage ( mime_message_t * pMessage,
					  m4char_t * & ao_pcFrom, 
					  m4char_t * & ao_pcTo,
					  m4char_t * & ao_pcCc,
					  m4char_t * & ao_pcSubject,
					  m4char_t * & ao_pcText,
					  m4char_t * & ao_pcAttachments, 
					  headersMap_t & ao_oHeaders )
{
	m4int_t				iRes = 0;
	m4void_t *			pBody;
	mime_content_type	contentType;
	mime_header_t  *	pRFC822_hdrs;
	char*				pHeaderValue = NULL;
	char*				pCharset = NULL;
	
	// Walk through the headers as needed
	pRFC822_hdrs = pMessage->rfc822_headers;

	// Now simply walk through the list of headers 
	while (pRFC822_hdrs != NULL)
	{
		pCharset = getHeaderCharset(pRFC822_hdrs->value);

		if ( ! strcmp (pRFC822_hdrs->name, "Subject"))
		{
			pHeaderValue = decode_rfc2047_header(pRFC822_hdrs->value, pCharset);
			ao_pcSubject = pHeaderValue;
		}
		
		if ( ! strcmp (pRFC822_hdrs->name, "From"))
		{
			pHeaderValue = decode_rfc2047_header(pRFC822_hdrs->value, pCharset);
			ao_pcFrom = pHeaderValue;
		}
		
		if ( ! strcmp (pRFC822_hdrs->name, "To"))
		{
			pHeaderValue = decode_rfc2047_header(pRFC822_hdrs->value, pCharset);
			if (!ao_pcTo )
			{
				ao_pcTo = pHeaderValue;
			}
			else 
			{
				ConcatString (ao_pcTo, ";");
				ConcatString (ao_pcTo, pHeaderValue);
				delete pHeaderValue;
			}
		}

		if ( ! strcmp (pRFC822_hdrs->name, "Cc"))
		{
			pHeaderValue = decode_rfc2047_header(pRFC822_hdrs->value, pCharset);
			if (!ao_pcCc)
			{
				ao_pcCc = pHeaderValue;
			}
			else 
			{
				ConcatString (ao_pcCc, ";");
				ConcatString (ao_pcCc, pHeaderValue);
				delete pHeaderValue;
			}
		}

		_InsertHeader(ao_oHeaders, pRFC822_hdrs->name, pRFC822_hdrs->value, pCharset);

		delete [] pCharset;
		pCharset = NULL;

		pRFC822_hdrs = pRFC822_hdrs->next;
	}
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Cabeceras del mensaje parseadas: " << iRes << "\n";
	#endif

	// Walk through the Body of the message
	iRes = mime_message_getBody (pMessage, &contentType, &pBody);
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Body obtenido: " << iRes << "\n";
	#endif
	if (iRes != MIME_OK)
	{
		return iRes;
	}


 	iRes = walkBodyPart (	pBody, contentType,
						ao_pcFrom, 
						ao_pcTo,
						ao_pcCc,
						ao_pcSubject,
						ao_pcText,
						ao_pcAttachments,
						ao_oHeaders );
	if (iRes != MIME_OK) {
		return iRes;
	}
	
	// free the body-part 
	free_this_body_part (pBody, contentType);
	pBody = NULL;

	return 0;
}

m4int_t ParseMIME ( m4char_t * & ao_pcFrom, 
				    m4char_t * & ao_pcTo,
					m4char_t * & ao_pcCc,
				    m4char_t * & ao_pcSubject,
				    m4char_t * & ao_pcText,
					m4char_t * & ao_pcAttachments,
					headersMap_t & ao_oHeaders )
{

	mime_message_t *		pMessage;
	m4int_t					iRes;
	nsmail_inputstream_t *	pInStream;
	
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Iniciando parse del mime\n";
	#endif

	iRes = file_inputStream_create (g_pcFile, &pInStream);
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Input stream creado: " << iRes << "\n";
	#endif
	if (iRes != MIME_OK)
	{
		return iRes;
	}

	iRes = parseEntireMessageInputstream (pInStream, &pMessage);
	#ifdef	_M4MAIL_PRINT_DEBUG
		cout << "Comprobación del mensaje: " << iRes << "\n";
	#endif
	if (iRes != MIME_OK)
	{
		return iRes;
	}
	else
	{
		iRes = walkMessage (pMessage,
							ao_pcFrom, 
							ao_pcTo,
							ao_pcCc,
							ao_pcSubject,
							ao_pcText,
							ao_pcAttachments,
							ao_oHeaders);
		if (iRes)
			return iRes;
	}
	
	pInStream->close(pInStream->rock);
	nsStream_free (pInStream);

	return 0;
}

// Función que concatena dos cadenas reservando y liberando memoria.
//
// Devuelve en el primer parámetro una cadena que es resultado de 
// la concatenación de las dos que recibe como argumento. 
// Reserva la memoria necesaria para la cadena que devuelve.
// Libera la memoria de la primera cadena que recibe.

m4void_t ConcatString (m4char_t * &ao_pcCad1, const m4char_t * ai_pcCad2){

	m4char_t * pcRet = NULL;
	pcRet = new m4char_t[strlen(ao_pcCad1) + strlen(ai_pcCad2) + 1];
	strcpy (pcRet, ao_pcCad1);
	delete [] ao_pcCad1;
	pcRet = strcat (pcRet, ai_pcCad2);
	ao_pcCad1 = pcRet;	
}


// replaces strdup standard API
// internally we use delete instead of free. This causes error on some UNIX's
m4pchar_t mailstrdup (m4pcchar_t ai_pcString)
{
	m4pchar_t pcOut = new m4char_t[strlen(ai_pcString)+1];
	strcpy(pcOut,ai_pcString);
	return pcOut;
}


static	m4return_t	_ExecuteThreadSafe( m4pcchar_t ai_pccFunction, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4int32_t		iResult = M4_ERROR ;
	char*			pcTempFile = NULL ;
	static	ClMutex s_oMutex( M4_TRUE ) ;


	ai_pvReturn.SetDouble( M4_ERROR ) ;

	// Ponemos el mutex
	if( s_oMutex.Lock() == M4_ERROR )
	{
		SETCODEF( M4_MAIL_E_CONCURRENCY, M4_ERROR, "#*s1*#%s#", ai_pccFunction ) ;
		return M4_SUCCESS ;
	}

	if( g_pcTempDir == NULL )
	{
		g_pcTempDir = (m4char_t*) ((ClVMRunContext*)ai_pvContext)->m_pAccess->GetEnvironment()->GetTempDirectory();
	}

	if( g_pcFile == NULL )
	{
		pcTempFile = tmpnam( (char*)NULL ) ;

		if( pcTempFile == NULL )
		{
			SETCODEF( M4_MAIL_E_WRITETEMP, M4_ERROR, "#*s1*#%s#", "NULL" ) ;
			s_oMutex.Unlock() ;
			return M4_SUCCESS ;
		}

		g_pcFile = mailstrdup( pcTempFile ) ;
	}

	if( strcmp( ai_pccFunction, "SendMail" ) == 0 )
	{
		iResult = _SendMail( ai_pvArg, ai_iLongArg ) ;
	}
	else if( strcmp( ai_pccFunction, "ReceiveMail" ) == 0 )
	{
		iResult = _ReceiveMail( ai_pvArg, ai_iLongArg ) ;
	}
	else if( strcmp( ai_pccFunction, "LoadMailData" ) == 0 )
	{
		iResult = _LoadMailData( ai_pvArg, ai_iLongArg, (ClVMRunContext*)ai_pvContext ) ;
	}
	else if( strcmp( ai_pccFunction, "NumberOfMails" ) == 0 )
	{
		iResult = _NumberOfMails( ai_pvArg, ai_iLongArg ) ;
	}
	else if( strcmp( ai_pccFunction, "DeleteMail" ) == 0 )
	{
		iResult = _DeleteMail( ai_pvArg, ai_iLongArg ) ;
	}

	if( g_pcFile != NULL )
	{
		remove( g_pcFile ) ;
	}

	ai_pvReturn.SetDouble( iResult ) ;

	// Quitamos el mutex
	if( s_oMutex.Unlock() == M4_ERROR )
	{
		SETCODEF( M4_MAIL_E_CONCURRENCY, M4_ERROR, "#*s1*#%s#", ai_pccFunction ) ;
		return M4_SUCCESS ;
	}

	return M4_SUCCESS ;
}

#else

static	m4return_t	_ExecuteThreadSafe( m4pcchar_t ai_pccFunction, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	SETCODEF( M4_MAIL_NO_64_BITS, M4_ERROR, "#*s1*#%s#", ai_pccFunction );
	return M4_ERROR ;
}

#endif


m4return_t SendMail( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	return _ExecuteThreadSafe( "SendMail", ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ;
}


m4return_t ReceiveMail( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	return _ExecuteThreadSafe( "ReceiveMail", ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ;
}


m4return_t LoadMailData( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,	m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	return _ExecuteThreadSafe( "LoadMailData", ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ;
}


m4return_t NumberOfMails( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	return _ExecuteThreadSafe( "NumberOfMails", ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ;
}


m4return_t DeleteMail (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	return _ExecuteThreadSafe( "DeleteMail", ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ;
}




