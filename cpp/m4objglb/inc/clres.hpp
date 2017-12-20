//## begin module%3912A6BB03D1.cm preserve=no
//## end module%3912A6BB03D1.cm

//## begin module%3912A6BB03D1.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A6BB03D1.cp

//## Module: clres%3912A6BB03D1; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clres.hpp

#ifndef __CLRES_HPP__
#define __CLRES_HPP__ 1

//## begin module%3912A6BB03D1.additionalIncludes preserve=no
//## end module%3912A6BB03D1.additionalIncludes

//## begin module%3912A6BB03D1.includes preserve=yes

#include "m4res.hpp"
#include "m4log.hpp"

//vale tio, ya lo sabemos: que es una version preeliminar mientras modifias .cpp's
//#pragma message("clres: Version preeliminar mientras modifico .cpp's")
//#include "dsres.hpp"
//#include "dmres.hpp"
//#include "vmres.hpp"
//#include "mcrres.hpp"
//#include "ldbres.hpp"
//#include "wores.hpp"

//==============================================================================
//  SM-CS Messages
//==============================================================================

#define M4_SMCS_REQUEST_SEND  0x0001 + M4_SMCS_BASE
//Petición cliente/servidor enviada.\n \nSolicitud de emulación cliente/servidor en "%0:s" : método "%1:s" del nodo "%2:s" en el Meta4Object "%3:s" enviada.
#define M4_SMCS_RESPONSE_RECEIVED  0x0002 + M4_SMCS_BASE
//Petición cliente/servidor recibida.\n \nSolicitud de emulación cliente/servidor en "%0:s" : método "%1:s" del nodo "%2:s" en el Meta4Object "%3:s" recibida.

//==============================================================================
//  Server API client messages
//==============================================================================

// ESTO DEBERIA ESTAR EN UN RES COMUN ENTRE SERVER Y CLIENT (CRG) !!!
#define  M4_API_COMMS_TIMEOUT_REMOTE_EXEC 0x0001 + M4_SRV_API_CLIENTE_ERROR
#define  M4_API_COMMS_TIMEOUT_LOGON 0x0002 + M4_SRV_API_CLIENTE_ERROR
#define  M4_API_COMMS_TIMEOUT_LOGOUT 0x0003 + M4_SRV_API_CLIENTE_ERROR

//## end module%3912A6BB03D1.includes

//## begin module%3912A6BB03D1.declarations preserve=no
//## end module%3912A6BB03D1.declarations

//## begin module%3912A6BB03D1.additionalDeclarations preserve=yes
//## end module%3912A6BB03D1.additionalDeclarations


//## begin module%3912A6BB03D1.epilog preserve=yes
//## end module%3912A6BB03D1.epilog


#endif
