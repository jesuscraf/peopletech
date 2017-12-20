//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                ldb_interface.hpp   
// Project:             mind 4.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-02-00
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module defines the interface of the Logical DataBase.
//
//
//==============================================================================

#ifndef	_M4LDB_HPP_
#define _M4LDB_HPP_

#include "m4ldb_dll.hpp"

#include "m4types.hpp"
#include "lconn_interface.hpp"
#include "conn_provider.hpp"

class ClNotifiable;
class ClVMBaseEnv;
class ClCMCRFactory;
class ClCacheDirectory;
class ClServerEvent;
class ILdbContext;
class ClCursor;

class ClMutex;

const m4pchar_t ERROR_LDB_NIHIL = "ERROR";

const m4pcchar_t TECH_VERSION_KEY		= "TECH_VERSION";
const m4pcchar_t TECH_SRV_VERSION_KEY	= "TECH_SRV_VERSION";

class ILdb
{
public:

	ILdb (void){}
	virtual ~ILdb(void){}
	
	// Inicialización.
	virtual m4return_t	Init(ClVMBaseEnv *ai_pObjReg, ClCMCRFactory *ai_pFactory, ILdbContext *ai_pContext)= 0;
	virtual m4return_t	End(void) = 0;
	virtual m4return_t	SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory) = 0;
	
	// Gestión de conexiones.
	virtual ClLConn_Interface * GetConnection(m4int32_t &ao_indConnection) = 0;
	virtual ClLConn_Interface * GetConnection28(m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword) = 0;
	virtual m4return_t	FreeConnection (m4int32_t ai_indConnection, m4bool_t ai_bIgnoreChannel = M4_FALSE) = 0;
	virtual m4return_t	FreeConnection28 (ClLConn_Interface *&ao_pLConn) = 0;
	virtual m4return_t	RefreshInvalidConnections(m4int32_t ai_indDBConn,m4int32_t ai_indLConn)=0;

	// Notificaciones.
	virtual void Attach_DBRyptureConnection(ClNotifiable *) = 0;
	
	// Limpieza del diccionario
	virtual m4return_t  ClearAll (void) = 0;
	virtual m4return_t  ClearTables (void) = 0;
	virtual m4return_t  ClearCache (void) = 0;
	virtual m4return_t  ClearTrans (void) = 0;

	virtual m4pcchar_t	GetDictConnStr() = 0;
	virtual m4int16_t	GetNumConn() = 0;		
	virtual m4int16_t	GetNumCursor() = 0;
	virtual m4pcchar_t	GetConExec() = 0;
	virtual m4int16_t	GetNumStmtCache() = 0;
	virtual m4int16_t	GetNumObject() = 0;	
	virtual m4pcchar_t	GetIsolationLevel() = 0;	
	virtual m4pcchar_t	GetDataBaseType() = 0;
	virtual m4pcchar_t	GetDebugLevel() = 0;
	virtual m4uint32_t	GetNumConnInUse() = 0;
	virtual m4uint32_t	GetNumLogicObjectInUse() = 0;
	virtual m4uint32_t	GetNumStatmntCacheInUse() = 0
		;
	virtual m4return_t	SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel) = 0;
	//Detalle de trazas en ldbinsp.
	virtual m4return_t	SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel) = 0;
	virtual m4pcchar_t	GetStrSystemDebugDetailLevel(void) = 0;
	virtual	m4uint32_t	GetSystemDebugDetailLevel (void) = 0;

	virtual void		UserSessionClosedByIdSession( m4uint64_t ai_iIdSession ) = 0;
};


class ClLdb_Nihil : public ILdb
{
public:

	ClLdb_Nihil (void){}
	// Inicialización.fe
	virtual m4return_t	Init(ClVMBaseEnv *ai_pObjReg, ClCMCRFactory *ai_pFactory, ILdbContext *ai_pContext){return M4_ERROR;}
	virtual m4return_t	End(void){return M4_ERROR;}
	virtual m4return_t	SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory){return M4_ERROR;}
		
	// Gestión de conexiones.
	virtual ClLConn_Interface * GetConnection(m4int32_t &ao_indConnection) {return 0;}
	virtual ClLConn_Interface* GetConnection28(m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword) {return 0;}
	virtual m4return_t	FreeConnection (m4int32_t ai_indConnection, m4bool_t ai_bIgnoreChannel = M4_FALSE) {return M4_ERROR;}
	virtual m4return_t	FreeConnection28 (ClLConn_Interface *&ao_pLConn) {return M4_ERROR;}
	virtual m4return_t	RefreshInvalidConnections(m4int32_t ai_indDBConn,m4int32_t ai_indLConn){return M4_ERROR;}

	// Notificaciones.
	virtual void Attach_DBRyptureConnection(ClNotifiable *) {}

	// Limpieza del diccionario
	virtual m4return_t  ClearAll (void) {return M4_ERROR;}
	virtual m4return_t  ClearTables (void) {return M4_ERROR;}
	virtual m4return_t  ClearCache (void) {return M4_ERROR;}
	virtual m4return_t  ClearTrans (void) {return M4_ERROR;}

	virtual m4pcchar_t	GetDictConnStr(){return ERROR_LDB_NIHIL;}
	virtual m4int16_t	GetNumConn(){return 0;}
	virtual m4int16_t	GetNumCursor(){return 0;}
	virtual m4pcchar_t	GetConExec(){return ERROR_LDB_NIHIL;}
	virtual m4int16_t	GetNumStmtCache(){return 0;}
	virtual m4int16_t	GetNumObject(){return 0;}
	virtual m4pcchar_t	GetIsolationLevel(){return ERROR_LDB_NIHIL;}
	virtual m4pcchar_t	GetDataBaseType(){return ERROR_LDB_NIHIL;}
	virtual m4pcchar_t	GetDebugLevel(){return ERROR_LDB_NIHIL;}
	virtual m4uint32_t	GetNumConnInUse(){return 0;}
	virtual m4uint32_t	GetNumLogicObjectInUse(){return 0;}
	virtual m4uint32_t	GetNumStatmntCacheInUse(){return 0;}

	virtual m4return_t	SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel){return M4_ERROR;}

	virtual m4return_t	SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel) {return M4_ERROR;}
	virtual m4pcchar_t	GetStrSystemDebugDetailLevel(void) { return ERROR_LDB_NIHIL; }	
	virtual	m4uint32_t	GetSystemDebugDetailLevel (void) { return 0; }

	virtual void		UserSessionClosedByIdSession( m4uint64_t ai_iIdSession ) {return;}

protected:
	virtual ~ClLdb_Nihil(void){}
};

class M4_DECL_M4LDB ILdbContext {
public:
	virtual m4return_t Init (m4int32_t ai_iNumConn){return M4_SUCCESS;}

	// Indica el numero de conexiones logicas que se van a crear.
	// En SM solo una y en server las quq vengan del Registry.
	virtual m4int16_t	GetMaxNumConn(ClVMBaseEnv *){return 1;}
	// Indica el numero de conexiones logicas que se supone que tenemos.
	// En SM 2 ( una para cliente y otra para server) y en server las que vengan del Registry.
	virtual m4int16_t	GetNumConn(void){return 2;}
	
	// Indica si hay que marcar la conexion del wsistema como libre (SM) para que la coja cualquiera
	// o como cogida (Server) para no dársela a nadie externo.
	virtual m4bool_t	IsFreeSystemConnection(void){return M4_TRUE;}

	// Indica si las conexiones logicas se tiene que atachar a la Ldb.
	// En SM hay que hacer Attach, para que se puedan limpiar las caches. En Server hay un Mutex, y no podemos hacer Attach si es
	// la del sistema. ( si tuviera que hacer Clear bloquearáa dos veces el mismo Mutex)
	virtual m4bool_t	MustAttachConnection(m4int32_t ai_indLogConn){return M4_TRUE;}

	virtual m4return_t	Lock_LDBSWLConn(m4int32_t ai_time){return M4_SUCCESS;}
	virtual void		UnLock_LDBSWLConn(void){}

	// Indica si una conexion a base de datos es critica o no. En SM ninguna es critica, de modo que se reintenta 
	// la conexion siempre. En server la criticidad viene definida en el diccionario.
	virtual m4bool_t	IsCriticalDBConnection(m4bool_t ai_bIsCritical){return M4_FALSE;}
	// El numero de conexiones usadas. En SM debe permanecer constante, ya que tenemos una sola,
	virtual m4int32_t	IncrementUsedLogicConn(void){return 0;}
	virtual m4int32_t	DecrementUsedLogicConn(void){return 0;}

	virtual m4pcchar_t	GetVersionCheckingKey(void){return TECH_VERSION_KEY;}

	ClMutex			*m_poMutexConnect;
	ClMutex			*m_poMutexSystem;
	ClMutex			*m_poMutexLoadDict;
	ClMutex			*m_poMutexObjects;
	ClMutex			*m_poMutexObjectTrans;
	ClMutex			*m_poMutexForeigns;
	ClMutex			*m_poMutexRSMs;
	ClMutex			*m_poMutexOrgTree;
	ClMutex			*m_poMutexPartialLoad;
	ClMutex			*m_poMutexSentences;
	ClMutex			*m_poMutexSysSentences;
	ClMutex			*m_poMutexDebug;

	virtual ~ILdbContext(void){}

protected:
	ILdbContext(void)
	{
		m_poMutexConnect = NULL;
		m_poMutexSystem = NULL;
		m_poMutexLoadDict = NULL;
		m_poMutexObjects = NULL;
		m_poMutexObjectTrans = NULL;
		m_poMutexForeigns = NULL;
		m_poMutexRSMs = NULL;
		m_poMutexOrgTree = NULL;
		m_poMutexPartialLoad = NULL;
		m_poMutexSentences = NULL;
		m_poMutexSysSentences = NULL;
		m_poMutexDebug = NULL;
	}
};
#endif


