//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clldb.hpp   
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

#ifndef __CLLDB_HPP__
#define __CLLDB_HPP__


#include "defmain.hpp"
#include "clsentcc.hpp"
#include "cllconn.hpp"
#include "connmanager.hpp"
#include "clptload.hpp"
#include "m4objreg.hpp"
#include "cacheitfz.hpp"
#include "m4cachetypes.hpp"
#include "ldb_utils.hpp"


#ifdef _UNIX
	#define	M4_LDB_MAX_PATH				PATH_MAX
#else
	#define	M4_LDB_MAX_PATH				MAX_PATH
#endif


#define			LDB_SEM_TIMEOUT				30


class ClServerEvent;
class ClSyncroWait;
class ClVMBaseEnv;
class ClCMCRFactory;
class ClCacheDirectory;

class ClLConn;
class ILdbContext;



//-------------------------
//Bug 0090288, 0092128
//Definicion del objeto y del vector.
class ClRefreshInvalidConnections
{
public:
	m4int32_t m_indDBConn;
	m4int32_t m_indLConn;

	ClRefreshInvalidConnections ( m4int32_t ai_indDBConn, m4int32_t ai_indLConn )
	{
		m_indDBConn = ai_indDBConn;
		m_indLConn = ai_indLConn;
	}
};
typedef vector<class ClRefreshInvalidConnections>					VcClRefreshInvalidConnections;
typedef VcClRefreshInvalidConnections::iterator						itVcClRefreshInvalidConnections;
//-------------------------


class ClLdb : public ClBaseCacheInterface
{

public:

	ClLdb(void);
	~ClLdb(void);
	m4return_t Init(ClVMBaseEnv *ai_pObjReg, ClCMCRFactory *ai_pFactory, ILdbContext *ai_pContext);
	m4return_t End(void);
	m4return_t SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory);
	
	// Gestión de conexiones.
	ClLConn * GetConnection(m4int32_t &ao_indConnection);
	ClLConn * GetConnection28(m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword);
	m4return_t FreeConnection (m4int32_t ai_indConnection, m4bool_t ai_bIgnoreChannel = M4_FALSE);
	m4return_t FreeConnection28 (ClLConn *&ao_pLConn);

	// Notificaciones.
	virtual void Attach_DBRyptureConnection(ClNotifiable *);

	m4int32_t	FindIndexByIdObject (m4id_t ai_idLgcTbl);
	void		InsertObject (m4id_t ai_idLgcTbl, m4uint32_t ai_indLgcTbl);
	m4int32_t	FindIndexTransByIdObject (m4id_t ai_idLgcTbl);
	void		InsertTransObject (m4id_t ai_idLgcTbl, m4uint32_t ai_indLgcTbl);

	// Limpieza del diccionario
	m4return_t  ClearAll (void)
	{
		m_bClearAll = M4_TRUE;
		return M4_SUCCESS;
	}

	m4return_t  ClearTables (void)
	{
		m_bClearTables = M4_TRUE;
		return M4_SUCCESS;
	}

	m4return_t  ClearCache (void)
	{
		m_bClearCache = M4_TRUE;
		return M4_SUCCESS;
	}

	m4return_t ClearTrans (void)
	{
		m_bClearTrans = M4_TRUE;
		return M4_SUCCESS;
	}

	m4bool_t WarningsAreErrors (void)
	{
		return m_bWarningsAreErrors;
	}

	m4bool_t UseSecurityView (void) const
	{
		return m_bUseSecurityView;
	}

#ifdef _M4LDB_DEBUG
	m4int32_t GetSysDebugLevel (void)
	{
		return (m_pObjReg->GetSystemDebugEnable());
	}
	m4int32_t GetShowData (void)
	{
		return (m_pObjReg->GetShowData ());
	}
#endif

	ILdbContext * GetContext(void)
	{
		return m_pContext;
	}

	m4uint32_t GetDefaultLanguage (void)
	{
		return (m_lDefaultLanguage);
	}

	m4uint32_t DynamicLanguages (void)
	{
		return (m_iDynamicLanguages);
	}

	ClLConn *GetSysLConn (void)
	{
		return (m_pSysLConn);
	}

	m4uint32_t GetCounterOperation (void)
	{
		return (++m_nOperation);
	}

	m4bool_t HasSecurityOptimization (void)
	{
		return ((m_lCompatibility & M4LDB_CLTY_SECURITY_OPTIMIZATION) ? M4_TRUE : M4_FALSE);
	}

	VcPartialLoad &GetVectorPartialLoad (void)
	{
		return (m_vcPartialLoad);
	}

	void Attach (void);
	void Detach (void);
	
	// Cuando lanzamos el evento de error de conexion nos deberian llamar aqui para que intentaramos reconectarnos.
	m4return_t	RefreshInvalidConnections(m4int32_t ai_indDBConn,m4int32_t ai_indLConn);

	//Se invocara cuando haya un deslogado de algun usuario.
	void		UserSessionClosedByIdSession( m4uint64_t ai_iIdSession );

	m4int16_t	GetNumConn();
	m4int16_t	GetNumCursor()			{ return m_pObjReg->GetMaxNumCursor ();}
	m4pcchar_t	GetDictConnStr()		{ return m_pObjReg->GetConnectionString();}
	m4pcchar_t	GetConExec()			{ return m_pObjReg->GetConExec();}
	m4uint32_t	GetExecutionTimeOut()	{ return m_iExecutionTimeOut;}
	m4uint8_t	GetSQLAccessLevel()		{ return m_iSQLAccessLevel;}
	m4uint8_t	GetMultiOrgAccessLevel(){ return m_iMultiOrgAccessLevel;}
	m4uint32_t	GetAuditTime()			{ return m_iAuditTime;}
	m4uint32_t	GetAuditRows()			{ return m_iAuditRows;}
	m4int16_t	GetNumStmtCache()		{ return m_numStmtCache;}
	m4int16_t	GetNumObject()			{ return m_numObject;}
	m4int16_t	GetNumObjectTrans()		{ return m_numObjectTrans;}
	
	m4pcchar_t	GetIsolationLevel();	
	m4pcchar_t	GetDataBaseType();
	m4pcchar_t	GetDebugLevel();

	m4uint32_t	GetNumConnInUse();
	m4uint32_t	GetNumLogicObjectInUse();
	m4uint32_t	GetNumStatmntCacheInUse();

	m4return_t	SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel);

	//Detalle de trazas en ldbinsp.
	//Se utiliza como comando del monitor para poder modificar el valor del registro
	m4return_t	SetSystemDebugDetailLevel (m4uint32_t ai_uiSystemDebugDetailLevel);
	m4pcchar_t	GetStrSystemDebugDetailLevel(void);
	m4uint32_t	GetSystemDebugDetailLevel (void);	

	m4uint32_t	GetSystemDebugDetailLevelMask (m4uint32_t ai_uiSystemDebugDetailLevel);

	//Enh 0116664:
	m4bool_t  GetApplyOrganizationFilters(void) const
	{ 
		return m_bApplyOrganizationFilters;
	}

	m4bool_t  GetTranslationAuditory(void) const
	{ 
		return m_bTranslationAuditory;
	}

	m4int32_t  GetMinOrgLevel(void) const
	{ 
		return( m_iMinOrgLevel );
	}

	m4return_t RefreshOrgTree(void);

	// Api de caché
	void Init (void) {};
	m4return_t PersistCache (void){ return M4_SUCCESS; }
	m4return_t DePersistCache (void) { return M4_SUCCESS; }
	m4return_t DeleteCache (void) { return M4_SUCCESS; }
	m4return_t RemoveObjectById (m4pchar_t ai_pcId, m4puint32_t ai_piRemovedCount = 0) { return M4_SUCCESS; }
	m4return_t GetNextId (m4pchar_t& ao_pcId, void*& ai_poIterator)	{ return M4_SUCCESS; }
	m4return_t Dump (ClDumpCacheVisitor& ai_oVisitor) { return M4_SUCCESS; }

	m4uint16_t GetType (void)
	{
		return M4_CACHE_TABLE_CACHE_TYPE;
	}

	m4return_t GetNumObjects (m4uint32_t& ao_iNumObjects)
	{
		ao_iNumObjects = 0;
		return M4_SUCCESS;
	}

	m4return_t RemoveAll (m4date_t ai_dDate = 0);
	m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

	void Reload (void) {};
private:

	ClVMBaseEnv			*m_pObjReg;
	ClCMCRFactory		*m_pFactory;
	ClCacheDirectory	*m_poCacheDirectory;
	ILdbContext			*m_pContext;

	VcLogicConn m_vcLogicConn;

	m4int16_t m_numConn;
	ClLConn *m_pSysLConn;
	MpObject m_mpObject;
	MpObject m_mpObjectTrans;

	m4bool_t m_bClearAll;
	m4bool_t m_bClearTables;
	m4bool_t m_bClearTrans;
	m4bool_t m_bClearCache;

	m4int32_t m_iSizeLdbInspector;

	m4int32_t m_ulNumConnInUse;

	VcPartialLoad m_vcPartialLoad;
	
	ClConnectionManager	m_oConnManager;
	m4int16_t m_numStmtCache;
	m4int16_t m_numObject;
	m4int16_t m_numObjectTrans;

	m4bool_t m_bMultFiles; 
	m4uint32_t m_lDefaultLanguage;
	m4uint32_t m_iDynamicLanguages;
	m4uint32_t m_nOperation;
	m4int32_t m_lReferences;
	m4int32_t m_lCompatibility;
	m4int32_t m_lBufferSize;
	ClLdbCacheContainer m_oLDBDiskCache;

	m4bool_t m_bWarningsAreErrors;
	m4bool_t m_bUseSecurityView;
	m4uint32_t m_iExecutionTimeOut;
	m4uint8_t m_iSQLAccessLevel;
	m4uint8_t m_iMultiOrgAccessLevel;
	m4uint32_t m_iAuditTime;
	m4uint32_t m_iAuditRows;

	//Bug 0090288, 0092128
	m4bool_t m_bRefreshInvalidConnections;
	VcClRefreshInvalidConnections m_vRefreshInvalidConnections;
	
#ifdef _M4LDB_DEBUG
	m4pcchar_t m_pDebugDirectory;
#endif

	ClSentCache m_oStmtCache;

	friend class ClLConn;
	friend class ClPersistBase;
	friend class ClPersist;
	friend class ClStmt;
	friend class ClLoad;
	friend class ClMetaSentence;

	m4return_t SysLoadMetaDataDDBB (m4int32_t ai_indDDBB);
	m4return_t	_InitCommon(void);
	m4return_t	_CreateLogicalConnection(m4int32_t ai_index,m4bool_t ai_bIncludeDBConns);
	m4return_t	_InitSystemConnection(void);
	m4return_t  _ExecuteInternalCommand (m4pcchar_t ai_pCommand);
	m4return_t _Clear (void);
	m4return_t _ClearAll (void);
	m4return_t _ClearTables (void);
	m4return_t _ClearCache (m4bool_t ai_bAll);
	m4return_t _ClearTrans (void);

	//Bug 0090288, 0092128
	m4return_t _RefreshInvalidConnections( void );	
	m4return_t _RefreshInvalidConnections( m4int32_t ai_indDBConn,m4int32_t ai_indLConn );

	m4return_t SysLoadOrgTree(ClAccess *pAcsDict);
	m4return_t SysLoadDictio(void);
	m4return_t RefreshObjectMapFromDictionary(void);
	m4return_t _SetUserKey(m4pcchar_t ai_pUserKey);
	m4bool_t LoadDictionaryFromCache(void);
	m4return_t _UpdateParamInfo(ClAccess *pAcsDict);
	m4int32_t m_contLConn28;

	//Enh 0116664:
	//M4_FALSE--> las tablas multisocietarias se consideran mono
	//M4_TRUE--> Como siempre
	m4bool_t  m_bApplyOrganizationFilters;
	m4bool_t  m_bTranslationAuditory;
	m4int32_t m_iMinOrgLevel;

	m4return_t	_CheckRepositoryVersion() ;
};

#endif

