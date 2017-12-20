//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                ldbdebug.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
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

#ifndef __LDBDEBUG_HPP__
#define __LDBDEBUG_HPP__



#include "m4types.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "cllconn.hpp"
#include "m4stl.hpp"


class ClLConn;
class ClRecordSet;
class ClCursor;
class ClAccessRecordSet;
class ILdbContext;


//Detalle de trazas fichero.
const	m4uint8_t		M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED	= 0;
const	m4uint8_t		M4LDB_OBJECT_TYPE_IN_TRACE_FILE_ROLE		= 1;
const	m4uint8_t		M4LDB_OBJECT_TYPE_IN_TRACE_FILE_USER		= 2;
const	m4uint8_t		M4LDB_OBJECT_TYPE_IN_TRACE_FILE_M4O			= 3;
const	m4uint8_t		M4LDB_OBJECT_TYPE_IN_TRACE_FILE_NODE		= 4;

//Es un mapa donde la clave es el Tipo (M4O, User, Role) y el Id
typedef set <string, less<string> >		SetIncDbgDetail;
typedef SetIncDbgDetail::iterator		itSetIncDbgDetail;

//Tenemos el tipo de traza de fichero que se va leyendo del fichero.
typedef set <string, less<string> >		SetTypeToTrace;
typedef SetIncDbgDetail::iterator		itSetTypeToTrace;



class ClLDBInspector
{
private:

	m4bool_t		m_bInit;
	ILdbContext		*m_pContext;
	m4int32_t		m_lReferences;

	FILE			*m_pFile1;
	FILE			*m_pFile2;
	FILE			*m_pFile;

	m4pchar_t		m_pcPathFile1;
	m4pchar_t		m_pcPathFile2;

	ClLConn			*m_poLastLConn;
	m4pchar_t		m_pcLastUser;

	m4int32_t		m_iActualPos;
	m4int32_t		m_iMaxPos;
	m4int32_t		m_iTotSize;

	//Detalle de trazas fichero.
	SetIncDbgDetail *m_psetIncDbgDetail;
	SetTypeToTrace	*m_psetTypeToTrace;
	m4int32_t		m_iMaskTypeToTrace;

public:
	
	ClLDBInspector(m4pcchar_t ai_pccFile1, m4pcchar_t ai_pccFile2, m4int32_t ai_iSize);
	~ClLDBInspector(void);

	void Init(ILdbContext *ai_pContext)
	{
		m_pContext = ai_pContext;
	}

	void Attach(void)
	{
		m_lReferences++;
	}

	void Detach(void)
	{
		m_lReferences--;
	}

	m4int32_t GetReferences(void)
	{
		return(m_lReferences);
	}

	void PrintFileError(ClLConn *ai_poLConn, m4int32_t ai_lErrorCode, m4pcchar_t ai_pMessage)
	{
		_PrintFileInfo(ai_poLConn, "ERROR", ai_lErrorCode, ai_pMessage);
	}

	void PrintFileWarning(ClLConn *ai_poLConn, m4int32_t ai_lErrorCode, m4pcchar_t ai_pMessage)
	{
		_PrintFileInfo(ai_poLConn, "WARNING", ai_lErrorCode, ai_pMessage);
	}

	void PrintFileDebug(ClLConn *ai_poLConn, m4int32_t ai_lErrorCode, m4pcchar_t ai_pMessage)
	{
		_PrintFileInfo(ai_poLConn, "INFO", ai_lErrorCode, ai_pMessage);
	}

	void PrintFileDebugDetailDate(m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...);
	void PrintFileDebugDetail    (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...);
	void PrintFileDebugDetail    (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData = NULL, m4int32_t ai_iFirstData = 0);
	void PrintFileDebugDetail    (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, ClCursor *ai_pCursor);

	void PrintFileDebugDate(ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...);
	void PrintFileDebug    (ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...);
	void PrintFileDebug    (ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData = NULL, m4int32_t ai_iFirstData = 0);
	void PrintFileDebug    (ClLConn *ai_poLConn, ClCursor *ai_pCursor);

private:

	m4bool_t	_BeginPrinting(ClLConn *ai_poLConn);
	void		_EndPrinting(void);
	m4return_t  _CreateLDBFile(void);
	void		_ChangeLDBInspector(void);

	void _PrintFileDebug(m4bool_t ai_bWithDate, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, va_list ai_vaListVariableArguments);
	void _PrintFileDebug(ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData = NULL, m4int32_t ai_iFirstData = 0);
	void _PrintFileDebug(ClLConn *ai_poLConn, ClCursor *ai_pCursor);
	void _PrintFileInfo (ClLConn *ai_poLConn, m4pcchar_t ai_pLabel, m4int32_t ai_lErrorCode, m4pcchar_t ai_pMessage);

	m4bool_t	_IsReadyToDumpDebugInfo (ClLConn *ai_poLConn);
	m4bool_t	_FindObjectDebugDetail (ClLConn *ai_poLConn);	
	m4bool_t	_FindObjectInListDebugDetail (ClLConn *ai_poLConn);
	m4bool_t	_FindInSetInclusionDebugDetail (string ai_sTypeToTrace, string ai_sIdObjectToTrace);
	void		_ReadFromFileDebugDetail (void);
	m4return_t	_TranslateMaskTypeToTrace (string ai_sMaskTypeToTrace);
	m4bool_t	_IsDebuggingFromFileDebugDetail (ClLConn *ai_poLConn);
};

#endif

