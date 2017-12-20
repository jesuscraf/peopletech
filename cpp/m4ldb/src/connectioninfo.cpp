#include "connectioninfo.hpp"
#include "access.hpp"
#include "defmain.hpp"
#include "ldbglobals.hpp"
//#include "m4regdef.hpp"
#include "m4objreg.hpp"
#include "ldbcontext.hpp"	// ILdbContext

m4int32_t
ClConnectionInfo::GetNumRConn(ClAccess *ai_pDictAccess){
	// Contamos las conexiones que haya en el nodo de conexiones
	if (ai_pDictAccess==0){
		return 0;
	}
	return ai_pDictAccess->Node[(ClNodeIndex)M4LDB_SYS_DICT_CONNECTIONS].RecordSet.Count();
}
m4return_t
ClConnectionInfo::GetInfoByPos(ClAccess *ai_pDictAccess,m4int32_t ai_Pos,m4int32_t &ao_iId,stConexionProperties &ao_stConProp){
// Nos posicionamos en el registro que nos piden.
	ClRegister *pReg = &(ai_pDictAccess->Node[(ClNodeIndex)M4LDB_SYS_DICT_CONNECTIONS].RecordSet.Register);
	if (pReg->MoveTo(ai_Pos)==M4_FALSE){
		// No hay tantos registros
		return M4_ERROR;
	}
	
	// Obtenemos el Id_Conn
	ao_iId = M4LDB_NUMBER(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_CONN].Value.Get());
	
// Obtenemos el resto de información de la conexion.

	// Cadena de conexion.
	m4pcchar_t pConnStr = M4LDB_PCSTR(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_CONN_STR].Value.Get());
	// Password
	m4pcchar_t pPassword = M4LDB_PCSTR (pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_PASSWORD].Value.GetWithOutSec ());
	// Tipo de base de datos.
	eDDBBType_t eDBType = (eDDBBType_t)M4LDB_NUMBER(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_DDBB].Value.Get());
	// Nivel de isolacion
	eIsolationLevel_t eILevel = (eIsolationLevel_t )M4LDB_NUMBER(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_ISOLATION_LEVEL].Value.Get());
	// Numero maximo de cursores por conexion fisica.
	m4int16_t iMaxNumCursor = (m4int16_t)M4LDB_NUMBER(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_MAX_NUM_CURSOR].Value.Get());
	// Cursores abiertos.
	m4bool_t bCursorOpened = M4_TRUE;
	// ConExec
	m4pcchar_t pConExec = M4LDB_PCSTR(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_CON_EXEC].Value.Get());
	m4bool_t bIsCritical = M4LDB_BOOL(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_IS_CRITICAL].Value.Get());

	//Obtenemos el tipo de conexión y se lo indicamos a la estructura!.
	eConnectionType_t iConnectionType = (eConnectionType_t) M4LDB_NUMBER(pReg->Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_CONN_TYPE].Value.Get());
	ao_stConProp.m_eConnectionType = iConnectionType;

// Validamos la informacion y aplicamos valores por defecto si hiciera falta.
	if (ValidateConnectionProperties(pConnStr,pPassword,eDBType,iMaxNumCursor,eILevel,bCursorOpened,pConExec,bIsCritical,ao_stConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::GetInfoSystemConnection(stConexionProperties &ao_stSysConProp){
	if (m_pObjReg==0){
		// No nos han inicializado.
		return M4_ERROR;
	}

// Leemos la informacion del Registry.
	// Cadena de conexion.
	m4pcchar_t pConnStr = m_pObjReg->GetConnectionString();
	// Password
	m4pcchar_t pPassword = m_pObjReg->GetDBPassword();
	// Tipo de base de datos.
	eDDBBType_t eDBType = (eDDBBType_t)m_pObjReg->GetDataBase();
	// Nivel de isolacion
	eIsolationLevel_t eILevel = (eIsolationLevel_t)m_pObjReg->GetIsolationLevel();
	// Numero maximo de cursores por conexion fisica.
	m4int16_t iMaxNumCursor = m_pObjReg->GetMaxNumCursor();
	// Cursores abiertos.
	m4bool_t bCursorOpened = M4_TRUE;
	// ConExec
	m4pcchar_t pConExec = m_pObjReg->GetConExec();
	m4bool_t  bIsCritical = M4_TRUE;
	
// Validamos la informacion y aplicamos valores por defecto si hiciera falta.
	if (ValidateConnectionProperties(pConnStr,pPassword,eDBType,iMaxNumCursor,eILevel,bCursorOpened,pConExec,bIsCritical,ao_stSysConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ValidateConnectionProperties(m4pcchar_t ai_pConnStr,m4pcchar_t ai_pPwdEncryp,eDDBBType_t ai_eDBType,m4int16_t ai_MaxNumCursor,eIsolationLevel_t ai_eILevel,m4bool_t ai_bCurOpen,m4pcchar_t ai_pConnExec,m4bool_t ai_bIsCritical,stConexionProperties &ao_stConProp){
// Validamos una a una todas las propiedades.
	// Cadena de conexion
	if (ProcessConnectionString(ai_pConnStr,ai_pPwdEncryp,ao_stConProp.m_stConnStr)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Tipo de base de datos.
	if (ProcessDBType(ai_eDBType,ao_stConProp.m_eDBW)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Numero maximo de cursores por conexion fisica.
	if (ProcessMaxNumCursor(ai_MaxNumCursor,ao_stConProp.m_numMaxCursor)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Nivel de isolacion
	if (ProcessIsolationLevel(ai_eILevel,ao_stConProp.m_eILevel)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Cursores abiertos.
	if (ProcessCursorOpened(ai_bCurOpen,ao_stConProp.m_bCursorOpened)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// ConExec
	if (ProcessConnExec(ai_pConnExec,ao_stConProp.m_stConExec)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// IsCritical
	if (ProcesIsCritical(ai_bIsCritical,ao_stConProp.m_bIsCritical)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t
ClConnectionInfo::ProcessConnectionString(m4pcchar_t ai_pConnStr,m4pcchar_t ai_pPwdEncryp,string &ao_ConnStr){
	
	m4return_t	iResult = M4_SUCCESS;
	m4int_t		iLength = 0;
	m4char_t	szBuffer[M4CH_MAX_CONNECTION_STRING];
	m4pchar_t	pcDBPwd = NULL;

	iResult = M4DeConvert(ai_pPwdEncryp, pcDBPwd);

	if (iResult != M4_SUCCESS)
	{
		// si hay error devolvemos la que nos viniera.
		ao_ConnStr = ai_pConnStr;
		return M4_SUCCESS;
	}
	
	sprintf(szBuffer, ai_pConnStr, pcDBPwd);

	delete pcDBPwd;

	ao_ConnStr = szBuffer;

	return M4_SUCCESS;
}

m4return_t
ClConnectionInfo::ProcessDBType(eDDBBType_t ai_eDBType,eDDBBType_t &ao_eDBType){

	// Si está fuera de rango [1-5], le damos el de Oracle.
	if (ai_eDBType<M4DBW_SQL_SERVER_DDBB||ai_eDBType>M4DBW_SYBASE_DDBB){
		ao_eDBType = M4DBW_ORACLE_DDBB;
	}
	else{
		ao_eDBType = ai_eDBType;
	}

	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ProcessMaxNumCursor(m4int16_t	ai_MaxNumCursor,m4int16_t &ao_MaxNumCursor){
	if (ai_MaxNumCursor<=0){
		ao_MaxNumCursor = M4LDB_MAX_NUM_CURSOR;
	}
	else{
		ao_MaxNumCursor = ai_MaxNumCursor;
	}
	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ProcessIsolationLevel	(eIsolationLevel_t ai_eILevel,eIsolationLevel_t &ao_eILevel){
	ao_eILevel = ai_eILevel;
	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ProcessCursorOpened		(m4bool_t ai_pCurOpen,m4bool_t &ao_pCurOpen){
	ao_pCurOpen = M4_TRUE;
	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ProcessConnExec			(m4pcchar_t ai_pConnExec,string &ao_ConnExec){
	ao_ConnExec = ai_pConnExec;
	return M4_SUCCESS;
}
m4return_t
ClConnectionInfo::ProcesIsCritical			(m4bool_t ai_bIsCritical,m4bool_t &ao_bIsCritical){
	ao_bIsCritical = m_pContext->IsCriticalDBConnection(ai_bIsCritical);
	return M4_SUCCESS;
}
