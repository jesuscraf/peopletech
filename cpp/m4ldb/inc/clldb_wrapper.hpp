
#ifndef _CLLDB_WRAPPER_HPP_
#define _CLLDB_WRAPPER_HPP_

#include "m4ldb.hpp"


class ClLdb;

class M4_DECL_M4LDB ClLdb_Wrapper : public ILdb
{
public:

	ClLdb_Wrapper (void);
	virtual ~ClLdb_Wrapper(void);

	// Inicialización.
	virtual m4return_t	Init(ClVMBaseEnv *ai_pObjReg, ClCMCRFactory *ai_pFactory, ILdbContext *ai_pContext);
	virtual m4return_t	End(void);
	virtual m4return_t	SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory);

	// Gestión de conexiones.
	virtual ClLConn_Interface * GetConnection(m4int32_t &ao_indConnection);
	virtual ClLConn_Interface * GetConnection28(m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword);
	virtual m4return_t	FreeConnection (m4int32_t ai_indConnection,  m4bool_t ai_bIgnoreChannel = M4_FALSE);
	virtual m4return_t	FreeConnection28 (ClLConn_Interface *&ao_pLConn);
	virtual m4return_t	RefreshInvalidConnections(m4int32_t ai_indDBConn,m4int32_t ai_indLConn);

	// Notificaciones.
	virtual void Attach_DBRyptureConnection(ClNotifiable *);

	// Limpieza del diccionario
	virtual m4return_t  ClearAll (void);
	virtual m4return_t  ClearTables (void);
	virtual m4return_t  ClearCache (void);
	virtual m4return_t  ClearTrans (void);

	virtual m4pcchar_t	GetDictConnStr();
	virtual m4int16_t	GetNumConn();		
	virtual m4int16_t	GetNumCursor();
	virtual m4pcchar_t	GetConExec();
	virtual m4int16_t	GetNumStmtCache();
	virtual m4int16_t	GetNumObject();	
	virtual m4pcchar_t	GetIsolationLevel();	
	virtual m4pcchar_t	GetDataBaseType();
	virtual m4pcchar_t	GetDebugLevel();
	virtual m4uint32_t	GetNumConnInUse();
	virtual m4uint32_t	GetNumLogicObjectInUse();
	virtual m4uint32_t	GetNumStatmntCacheInUse();

	virtual m4return_t	SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel);

	//Detalle de trazas en ldbinsp.
	virtual m4return_t	SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel);
	virtual m4pcchar_t	GetStrSystemDebugDetailLevel(void);
	virtual	m4uint32_t	GetSystemDebugDetailLevel (void);	

	//Notificacion de deslogado de un usuario.
	//Con fecha 24/02/2004 hemos buscado en todo el código (*.hpp, *.cpp) qué componentes utilizan ClLdb_Wrapper, 
	//y deberan ser recompiladas:
	//m4ldb, m4oeinit.
	//Pero como tambien se ha modificado ILdb (que engloba a estas dos), finalmente recompilamos las componentes que utilizan
	//el interfaz ILdb (ver en la definicion de ILdb que se indica las componentes afectadas).
	void	UserSessionClosedByIdSession( m4uint64_t ai_iIdSession );

protected:
	ILdbContext	*m_poContext;
	ClLdb			*m_pLdb;
	m4bool_t m_bInit;
};

#endif

