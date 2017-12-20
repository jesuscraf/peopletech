//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conexba.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13 (Friday) -03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================


//
// Clase ClCursor_Base
// Eliminara la dependencia de la Chdll de las clases Cursor nativas
// de cada BBDD
//
//

#ifndef _CURSORBA_HPP_
#define _CURSORBA_HPP_

#include "m4stl.hpp"
#include "m4dbwrap_dll.hpp"
#include "m4dbwrapdef.hpp"
#include "conexba.hpp"


class ClDBConnection;



class M4_DECL_M4DBWRAP ICancelStatusProvider
{
public:
	virtual m4bool_t	GetCancelStatus( void ) = 0;
};




class ClCursor_Base
{

public:
	ClCursor_Base (void) {}

	virtual ~ClCursor_Base (void) {}

	virtual m4return_t Open (ClDBConnection_Base *) = 0;

	virtual m4return_t Close (void) = 0;

	virtual m4return_t Destroy (void) = 0;

	// binda una variable para que coloque ahi los resultados
	// posicion de la columna, variable donde se deja el resultado, longitud de esta)
	virtual m4return_t BindParam (m4int32_t ai_lCol, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ao_lLength) = 0;

	// binda una variable para que coloque ahi los resultados
	// posicion de la columna, variable donde se deja el resultado, longitud de esta)
	virtual m4return_t BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld = -1) = 0;

	virtual m4return_t GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass = M4_FALSE) = 0;

	virtual m4return_t Prepare (m4pcchar_t ai_pStmt, m4int32_t ai_numPar = 0, m4int32_t ai_numCol = -1, m4bool_t ai_bMultiRow = M4_TRUE) = 0;

	virtual m4return_t Execute (m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled) = 0;

	virtual m4return_t Fetch (void) = 0;

	virtual m4return_t DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type) = 0;

	virtual m4int32_t GetNumCol (void) = 0;

	virtual m4return_t GetErrorString( m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString ) = 0;

	virtual m4return_t SetBlockingMode( m4uint32_t ai_iMode ) = 0;
}; 

#endif


