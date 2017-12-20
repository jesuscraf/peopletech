//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mail.dll
// File:                errors.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                30/8/99
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    This module defines the error codes for the channel- core
//
//
//==============================================================================


#ifndef __ERRORS_HPP__
#define __ERRORS_HPP__

//#include "m4res.hpp"

//==============================================================================
//  Errores de la m4mail
//==============================================================================


#define M4_MAIL_E_INVALID_NUM_ARG				2359701
#define M4_MAIL_E_INVALID_PROPERTY				2359702
#define M4_MAIL_E_CONCURRENCY					2359703								
// Ojo con el orden de los códigos
#define M4_MAIL_E_INVALID_ARGUMENT				2359713

#define M4_MAIL_E_CREATEMIME					2359704								
#define M4_MAIL_E_SENDSMTP						2359705

#define M4_MAIL_E_RECEIVEPOP3					2359706								
#define M4_MAIL_E_PARSEMIME						2359707								

#define M4_MAIL_E_MESSAGE_NOT_FOUND				2359708

#define M4_MAIL_E_INVALID_SERVER				2359709
#define M4_MAIL_E_INVALID_USER					2359710

#define M4_MAIL_E_WRITEATTACHMENT				2359711
#define M4_MAIL_E_READATTACHMENT				2359712
#define M4_MAIL_E_WRITETEMP						2359714

#define M4_MAIL_E_ADDRESSERROR					2359715
//2359715="M4mail: La direccion de correo indicada es sintácticamente incorrecta ("%0:s")."

#define M4_MAIL_E_SENDDATA						2359716
//2359716="M4mail: Error durante el envío del correo. El mensaje no se ha podido enviar correctamente por la razón "%0:s"."

#define M4_MAIL_E_NO_SUCH_NODE					2359719
// M4mail: No existe el nodo.\n \nDebe definir el nodo "%0:s" o no utilizarlo.\nEsto sucedió en la función "%1:s".

#define M4_MAIL_E_ADD_REGISTER					2359720
// M4mail: No se puede añadir registro.\n \nNo se puede añadir registro en el nodo "%0:s".\nEsto sucedió en la función "%1:s".

#define M4_MAIL_E_SET_ITEM_VALUE				2359721
// M4mail: No se puede establecer el valor del elemento.\n \nDebe definir el elemento "%0:s" del nodo "%1:s".\nEsto sucedió en la función "%2:s".


#define M4_MAIL_NO_64_BITS						2359727
// M4mail: Esta funcionalidad no está disponible en 64 bits.\n \nLa función "%0:s" no está disponible en 64 bits".


		/* Resultados posibles de las funciones que tratan con MIME

MIME_OK						0
MIME_ERR_UNINITIALIZED		-1 // Uninitialized parameter.
MIME_ERR_INVALIDPARAM		-2 // Invalid parameters.
MIME_ERR_OUTOFMEMORY		-3 // Out of memory.
MIME_ERR_UNEXPECTED			-4 // Unexpected element.
MIME_ERR_IO					-5 // Error in input/output operation. 
MIME_ERR_IO_READ			-6 // Error in reading input stream.
MIME_ERR_IO_WRITE			-7 // Error in writing to output stream.
MIME_ERR_IO_SOCKET			-8 // Socket connection error.
MIME_ERR_IO_SELECT			-9 // Error in selecting message.
MIME_ERR_IO_CONNECT			-10 // Error in connecting.
MIME_ERR_IO_CLOSE			-11 // Error in closing.
MIME_ERR_PARSE				-12 // Internal parsing error occurred.
MIME_ERR_TIMEOUT			-13 // Timeout occurred. Recoverable error. Wait and call processResponses later. 
MIME_ERR_INVALID_INDEX		-14 //Invalid index.
MIME_ERR_CANTOPENFILE		-15 // Cannot open file.
MIME_ERR_CANT_SET			-16 // Cannot set MIME text.
MIME_ERR_ALREADY_SET		-17 // Item already set.
MIME_ERR_CANT_DELETE		-18 // Cannot delete item.
MIME_ERR_CANT_ADD			-19 // Cannot add item.
MIME_ERR_EOF				-1	// End of file reached.
MIME_ERR_EMPTY_DATASINK		-83	// Data sink missing.
MIME_ERR_ENCODE				-84 // Encoding error.
MIME_ERR_NO_SUCH_HEADER		-85 // Header type does not exist.
MIME_ERR_NO_HEADERS			-86 // Header missing.
MIME_ERR_NOT_SET			-87 // Item not set.
MIME_ERR_NO_BODY			-88 // Message body missing.
MIME_ERR_NOT_FOUND			-89 // Cannot find search item.
MIME_ERR_NO_CONTENT_SUBTYPE -90 //  Content subtype missing.
MIME_ERR_INVALID_ENCODING	-91 //  Invalid encoding type.
MIME_ERR_INVALID_BASICPART  -92 // Invalid basic message part.
MIME_ERR_INVALID_MULTIPART  -93 // Invalid multipart.
MIME_ERR_INVALID_MESSAGEPART-94 // Invalid message part.
MIME_ERR_INVALID_MESSAGE	-95 //  Invalid message.
MIME_ERR_INVALID_CONTENTTYPE-96 // Invalid content type.
MIME_ERR_INVALID_CONTENTID  -97 //  Invalid content identifier.
MIME_ERR_NO_DATA			-98 // Data missing.
MIME_ERR_NOTIMPL			-99 // Function or feature not implemented

*/ 


/* Resultados posibles de las funciones SMTP

NSMAIL_OK					0 // Successful completion of function.
NSMAIL_ERR_UNINTIALIZED		-1 // Uninitialized value.
NSMAIL_ERR_INVALIDPARAM		-2 // Invalid parameters.
NSMAIL_ERR_OUTOFMEMORY		-3 // Out of memory.
NSMAIL_ERR_UNEXPECTED		-4 //  Unexpected element.
NSMAIL_ERR_IO				-5 // Error in input/output operation.
NSMAIL_ERR_IO_READ			-6 //  Error in reading input stream.
NSMAIL_ERR_IO_WRITE			-7 //  Error in writing to output stream.
NSMAIL_ERR_IO_SOCKET		-8 //  Socket connection error.
NSMAIL_ERR_IO_SELECT		-9 // Error in selecting message.
NSMAIL_ERR_IO_CONNECT		-10 //  Error in connecting.
NSMAIL_ERR_IO_CLOSE			-11 // Error in closing.
NSMAIL_ERR_PARSE			-12 // Internal parsing error occurred.
NSMAIL_ERR_TIMEOUT			-13 // Timeout occurred. Recoverable error. Wait and call processResponses later. 
NSMAIL_ERR_INVALID_INDEX	-14 // Index is invalid.
NSMAIL_ERR_CANTOPENFILE		-15 // Cannot open file.
NSMAIL_ERR_CANT_SET			-16 // Value cannot be set.
NSMAIL_ERR_ALREADY_SET		-17 //  Value already set.
NSMAIL_ERR_CANT_DELETE		-18	//  Cannot delete item.
NSMAIL_ERR_CANT_ADD					-19 // Cannot add item.
NSMAIL_ERR_SENDDATA					-20 // Data send failed. Recoverable error.
NSMAIL_ERR_MUSTPROCESSRESPONSES		-21 //  Response not processed. Recoverable error.
NSMAIL_ERR_PIPELININGNOTSUPPORTED	-22 //  Server does not support pipelining. Recoverable error.
NSMAIL_ERR_ALREADYCONNECTEDD		-23 // Already connected.
NSMAIL_ERR_NOTCONNECTED				-24 //  Not connected.
NSMAIL_ERR_EOF						-1 // End of file reached.
NSMAIL_ERR_NOTIMPL					-99 //  Function or feature not implemented.
*/ 


#endif
