//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clstatem.hpp   
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

#ifndef __CLSTATEM_HPP__
#define __CLSTATEM_HPP__

#include "defmain.hpp"
#include "clglbvar.hpp"
//#include "cursor.hpp"



	
	
class ClCursor;
class ClAccessRecordSet;
class ClCompiledMCR;
class ClMetaSentence;
class ClDinArr;


typedef ClAccessRecordSet* PClAccessRecordSet;
#ifdef _M4LDB_DEBUG
	#include "clldb.hpp"
	#include "ldbdebug.hpp"
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DATE
	#define M4LDB_PRINT_DEBUG_DATE	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDate

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDetail

	#undef M4LDB_PRINT_DEBUG_DETAIL_DATE
	#define M4LDB_PRINT_DEBUG_DETAIL_DATE	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDetailDate

	#undef M4LDB_PRINTV_DEBUG
	#define M4LDB_PRINTV_DEBUG	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) if (!m_pGlobVars->GetpLConn ()->m_pLdb->GetShowData ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebug (m_pGlobVars->GetpLConn (), "*** ; "); else m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebug

	/*
	Bug 0114888
	Se cambia el M4LDB_DETAIL_LEVEL_SHOW_ALWAYS por M4LDB_DETAIL_LEVEL_SHOW_DATA
	para que no se muestre siempre, sólo cuando deben salir datos
	*/
	#undef M4LDB_PRINTV_DEBUG_DETAIL
	#define M4LDB_PRINTV_DEBUG_DETAIL	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) if (!m_pGlobVars->GetpLConn ()->m_pLdb->GetShowData ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDetail (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "*** ; "); else m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDetail

	#undef M4LDB_BEGIN_DEBUG
	#define M4LDB_BEGIN_DEBUG	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) {

	#undef M4LDB_END_DEBUG
	#define M4LDB_END_DEBUG		}


#else
	#define M4LDB_BEGIN_DEBUG
#endif


struct StIndPar 
{
	m4int32_t m_indPar;
	m4id_t m_idForFld;
	m4bool_t m_bIsPK;

	StIndPar (void) {
		m_indPar = M4LDB_NULL_INDEX;
		m_idForFld = M4LDB_EMPTY;
		m_bIsPK = M4_FALSE;
	}
};

typedef StIndPar*						PStIndPar;

typedef vector<PStIndPar>				VcIndParVal;
typedef VcIndParVal*					PVcIndParVal;
typedef VcIndParVal::const_iterator		itVcIndParVal;


typedef map<m4int32_t, ClCursor *, less<m4int32_t> >	MpPClCursor;
typedef MpPClCursor*									PMpPClCursor;
typedef MpPClCursor::iterator							itMpPClCursor;


class ClMetaSentence;

class ClStmt : public ICancelStatusProvider
{
protected:

	MpPClCursor m_mpCursor;

	ClCursor *m_pCursor;

	ClGlobVars *m_pGlobVars;

	m4int32_t m_idRConn;

	ClMetaSentence *m_pMtSent;
	m4bool_t m_bMtSentOwner;

	// Me lo llevo al ClGlobVars
	ClDinArr *m_pParam;	
	VcClParam * m_pVcClParam;
	VcVirtualParam		m_oVParam;

	//-----------------------------------------

	m4bool_t	m_bIsInternal;

	m4bool_t	m_bSttInit;

	m4return_t DisplayError (m4return_t ai_retcode, m4bool_t ai_bWarningLikeDebug = M4_FALSE);

	// Serializamos el acceso al mapa de cursores por causa de las funciones
	// de limpieza de cursores.
	ClCursor *FindCursor (m4int32_t ai_idLConn);
	ClCursor *GetFirstCursor (void);
	void EraseCursor (m4int32_t ai_idLConn);
	void InsertCursor (m4int32_t ai_idLConn, ClCursor *ai_pCursor);

	virtual m4bool_t HasOrdinal (void){
		return (M4_FALSE);
	}

	virtual m4bool_t IsPartialLoad (void){
		return (M4_FALSE);
	}

	virtual m4int32_t GetIndLgcTbl (void) {
		return (M4LDB_NULL_INDEX);
	}

	virtual m4return_t ComposePK (string &ai_onPKString, string &ai_oiPKString) {
		return (M4_SUCCESS);
	}

	m4return_t _ReadChannelPropertyOrSessionItem(m4int32_t ai_hItem, m4VariantType &ao_vValue);

	void DisplayData (m4int32_t ai_indParam);

public:
	eStmType_t m_eStmType;

	ClStmt (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal);
	virtual ~ClStmt (void);

	m4bool_t	GetCancelStatus( void );

	m4return_t Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, m4bool_t ai_bMtSentOwner, ClDinArr *ai_pParam);
  	m4return_t Destroy (void);
  	m4return_t Close (void);
    m4return_t Prepare (m4pchar_t ai_pStmSQL);
	m4return_t Execute (m4uint32_t ai_iTimeOut);
 	m4return_t BindParam (m4int32_t ai_lPar, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ai_pLength);
	m4return_t BindParam (m4int32_t ai_lPar, m4int32_t ai_lParam, m4bool_t ai_bUseOldValue = M4_FALSE, m4bool_t ai_bBlobAsBinary = M4_FALSE);
  	m4return_t BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld = -1);
  	m4return_t GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass = M4_FALSE);
    m4return_t Fetch (void);
  	m4return_t DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type);
    m4int32_t GetNumCol (void);
	
	ClCursor *GetCurrentCursor (void) {
		return (m_pCursor);
	}

	ClDinArr *GetpParams (void);
	virtual ClMetaSentence *GetMetaSentence (void);

	virtual void InitStt (void);
	
	m4return_t SetCursor (ClLConn *ai_pLConn);
	m4int32_t GetReferences (void);

	virtual m4return_t Defuse (ClLConn *ai_pLConn);

	m4bool_t AllowCursorOpened (void);

	ClGlobVars *GetpGlobVars (void);

	virtual m4id_t GetIdLgcTbl (void) {
		return (M4LDB_EMPTY);
	}
	// No usadas pero necesarias.
	friend m4bool_t operator < (const ClStmt &lhc, const ClStmt &rhc);
	friend m4bool_t operator ==(const ClStmt &lhc, const ClStmt &rhc);

	ClParam * ClStmt::GetClParamByType( eFldType_t ai_eFldType );
};

#endif

