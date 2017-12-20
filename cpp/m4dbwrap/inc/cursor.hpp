//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cursor.hpp   
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

#ifndef  _CURSOR_HPP_
#define  _CURSOR_HPP_

#include "m4stl.hpp"
#include "m4dbwrap_dll.hpp"
#include "m4dbwrapdef.hpp"
#include "cursorba.hpp"


class ClCursor_Base;
class ClDBConnection;



enum eLastError_t
{
	M4DBW_UNDEFINED_ERROR = 1,
	M4DBW_CONNECTION_ERROR,
	M4DBW_CURSOR_ERROR,
	M4DBW_TRANSLATION_ERROR,
	M4DBW_CONEX_CLONE_ERROR
};



// UNICODE DB
struct StParamData
{
	m4int32_t m_lCol;
	m4pchar_t m_pData;
	m4pchar_t *m_ppData;
	m4int32_t m_lPrec;
	m4int32_t m_iM4Type;
	m4int32_t m_lScale;
	m4pint32_t m_plLength;
	m4int32_t m_lPrecFld;

	StParamData (void)
	{
		m_lCol = M4LDB_NULL_INDEX;
		m_pData = NULL;
		m_ppData = NULL;
		m_lPrec = 0;
		m_iM4Type = M4CL_M4_TYPE_FIXED_STRING;
		m_lScale = 0;
		m_plLength = NULL;
		m_lPrecFld = -1;
	}
	
	StParamData (m4int32_t ai_lCol, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale , m4pint32_t ai_plLength, m4int32_t ai_lPrecFld = -1)
	{
		m_lCol = ai_lCol;
		m_pData = ai_pValue;
		m_ppData = ai_ppValue;
		m_lPrec = ai_lPrec;
		m_iM4Type = ai_lM4Type;
		m_lScale = ai_lScale;
		m_plLength = ai_plLength;
		m_lPrecFld = ai_lPrecFld;
	}
};


typedef vector<struct StParamData>			VcParamData;
typedef VcParamData*						PVcParamData;
typedef VcParamData::const_iterator			itVcParamData;


struct StResultData
{
	m4int32_t m_lCol;
	m4pchar_t *m_ppData;
	m4int32_t *m_plPrec;
	m4int32_t m_iM4Type;
	m4int32_t m_lScale;
	m4pint32_t m_plLength;
	m4int32_t m_lPrecFld;

	StResultData (void)
	{
		m_lCol = M4LDB_NULL_INDEX;
		m_ppData = NULL;
		m_plPrec = NULL;
		m_iM4Type = M4CL_M4_TYPE_FIXED_STRING;
		m_lScale = 0;
		m_plLength = NULL;
		m_lPrecFld = -1;
	}
	
	StResultData (m4int32_t ai_lCol, m4pchar_t *ai_ppValue, m4int32_t *ai_plPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale , m4pint32_t ai_plLength, m4int32_t ai_lPrecFld = -1)
	{
		m_lCol = ai_lCol;
		m_ppData = ai_ppValue;
		m_plPrec = ai_plPrec;
		m_iM4Type = ai_lM4Type;
		m_lScale = ai_lScale;
		m_plLength = ai_plLength;
		m_lPrecFld = ai_lPrecFld;
	}
};


typedef vector<struct StResultData>			VcResultData;
typedef VcResultData*						PVcResultData;
typedef VcResultData::const_iterator		itVcResultData;



class ClMetaCursor
{
	m4int32_t m_lReferences;

public:

	m4pchar_t m_pstrSql;
	m4int32_t m_lstrSql;

	PVcParamData m_pvcParam;
	PVcResultData m_pvcData;

	m4bool_t m_bMultiRow;
	m4bool_t m_bPartialLoad;

	ClMetaCursor (void)
	{
		m_pvcParam = NULL;
		m_pvcData = NULL;
		m_bMultiRow = M4_TRUE;
		m_bPartialLoad = M4_FALSE;
		m_pstrSql = NULL;
		m_lstrSql = 0;
		m_lReferences = 0;
	}

	~ClMetaCursor (void)
	{
		if (m_pvcParam) {
			delete m_pvcParam;
		}
		m_pvcParam = NULL;
		if (m_pvcData) {
			delete m_pvcData;
		}
		m_pvcData = NULL;
		if (m_pstrSql) {
			delete [] m_pstrSql;
		}
		m_pstrSql = NULL;
	}

	void Attach (void)
	{
		m_lReferences++;
	}


	void Detach (void)
	{
		m_lReferences--;
	}

	m4int32_t GetReferences (void)
	{
		return (m_lReferences);
	}
};
typedef ClMetaCursor*						PClMetaCursor;


class M4_DECL_M4DBWRAP ClCursor
{

public:
	ClCursor (void);
	~ClCursor (void);

	m4return_t Open (void);

	m4return_t Close (void);

	m4return_t Destroy (void);

	m4return_t Defuse (void);

	m4return_t BindParam (m4int32_t ai_lCol, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale , m4pint32_t ao_lLength);

	m4return_t BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld = -1);

	m4return_t GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass = M4_FALSE);

	m4return_t Prepare (m4pchar_t ai_pStmt, eDDBBType_t ai_eDDBB, m4bool_t ai_bPartialLoad = M4_FALSE);

	m4return_t Execute (m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled);

	m4return_t Fetch (void);

	m4return_t DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type);

	m4int32_t GetNumCol (void) ;

	m4return_t GetErrorString (m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString);

	m4return_t GetStmt (string &ao_stStmt);

	m4return_t GetStmt (m4pcchar_t &ao_pStmt);

	m4return_t Clone (ClCursor *ai_pCursor);

	m4bool_t AllowCursorOpened (void);

	m4return_t InitCursor (ClDBConnection *ai_pConn, m4bool_t &ao_bReallyInitialized);

	m4bool_t IsCursorActivated (void);

	m4return_t SetBlockingMode (m4uint32_t ai_iMode);

private :

	ClCursor_Base *m_pCurB;

	ClDBConnection *m_pConn;
	ClDBConnection *m_pConnPartialLoad;

	eLastError_t m_eLastError;

	PClMetaCursor m_pMtCursor;

	ClCursor * m_pBonusUpdate;
}; 


// Clase traductora para Informix. ( Separa el For Update del Order By, en caso de que lo haya ).

class ClTrnSQL{
public:
	m4return_t          Translate( string& ao_stStmSql , string& ao_stStmBonusUpdate );
protected:
	m4pchar_t			_FindToken(m4pcchar_t ai_pstring,m4pcchar_t ai_pToken);
};


#endif
