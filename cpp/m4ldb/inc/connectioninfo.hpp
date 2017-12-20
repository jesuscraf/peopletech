#ifndef _CONNECTIONINFO_HPP_
#define _CONNECTIONINFO_HPP_

#include "defmain.hpp"
#include "m4stl.hpp"
#include "m4types.hpp"

class ClAccess;
class ClVMBaseEnv;
class ILdbContext;

struct stConexionProperties;


class ClConnectionInfo{
public:
	ClConnectionInfo():m_pObjReg(0){}
	
	m4return_t Init(ClVMBaseEnv *ai_pObjReg,ILdbContext *ai_pContext){m_pObjReg=ai_pObjReg;m_pContext=ai_pContext;return M4_SUCCESS;}
	m4return_t End(ClVMBaseEnv *ai_pEnv);

	m4int32_t	GetNumRConn(ClAccess *ai_pDictAccess);	// sin contar la del sistema.
	m4return_t	GetInfoSystemConnection(stConexionProperties &ao_stConProp);	// del registry
	m4return_t	GetInfoByPos(ClAccess *ai_pDictAccess,m4int32_t ai_Pos,m4int32_t &ao_iId,stConexionProperties &ao_stConProp);	// del diccionario
	
	m4return_t	GetDBConnectionInfoById(m4int32_t ai_idRConn,ClAccess *ai_pDictAccess,stConexionProperties &ao_stConProp);	// de donde sea
	m4return_t	GetBackUpInfoById(m4int32_t ai_idRConn,ClAccess *ai_pDictAccess,stConexionProperties &ao_stConProp);	// de donde sea

protected:
	m4return_t ValidateConnectionProperties(m4pcchar_t ai_pConnStr,m4pcchar_t ai_pPwdEncryp,eDDBBType_t ai_eDBType,m4int16_t ai_MaxNumCursor,eIsolationLevel_t ai_eILevel,m4bool_t ai_bCurOpen,m4pcchar_t ai_pConnExec,m4bool_t ai_bIsCritical,stConexionProperties &ao_stConProp);
	
	m4return_t ProcessConnectionString	(m4pcchar_t ai_pConnStr,m4pcchar_t ai_pPwdEncryp,string &ao_ConnStr);
	m4return_t ProcessDBType			(eDDBBType_t ai_eDBType,eDDBBType_t &ao_eDBType);
	m4return_t ProcessMaxNumCursor		(m4int16_t	ai_MaxNumCursor,m4int16_t	&ao_MaxNumCursor);
	m4return_t ProcessIsolationLevel	(eIsolationLevel_t ai_eILevel,eIsolationLevel_t &ao_eILevel);
	m4return_t ProcessCursorOpened		(m4bool_t ai_bCurOpen,m4bool_t &ao_bCurOpen);
	m4return_t ProcessConnExec			(m4pcchar_t ai_pConnExec,string &ao_ConnExec);
	m4return_t ProcesIsCritical			(m4bool_t ai_bIsCritical,m4bool_t &ao_bIsCritical);

	ClVMBaseEnv *m_pObjReg;
	ILdbContext *m_pContext;
private:

};

#endif  // _CONNECTIONINFO_HPP_
