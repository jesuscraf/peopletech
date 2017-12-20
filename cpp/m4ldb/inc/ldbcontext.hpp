
#ifndef	_LDBCONTEXT_HPP_
#define _LDBCONTEXT_HPP_
#include "m4ldb.hpp"
#include "syncrowait.hpp"


class ClMutex;
class ClSyncroWait;


class M4_DECL_M4LDB ClLdbContextEmul : public ILdbContext
{
};



class M4_DECL_M4LDB ClLdbContextServer : public ILdbContext
{
public:
	ClLdbContextServer(void);
	virtual m4return_t Init (m4int32_t ai_iNumConn);
	virtual ~ClLdbContextServer(void);

	virtual m4int16_t	GetMaxNumConn(ClVMBaseEnv *);
	virtual m4int16_t	GetNumConn(void);

	virtual m4bool_t	IsFreeSystemConnection(void){return M4_FALSE;}

	virtual m4return_t	Lock_LDBSWLConn(m4int32_t ai_time);
	virtual void		UnLock_LDBSWLConn(void);

	virtual m4bool_t	IsCriticalDBConnection(m4bool_t ai_bIsCritical);
	virtual m4bool_t	MustAttachConnection(m4int32_t ai_indLogConn);

	virtual m4int32_t	IncrementUsedLogicConn(void){return 1;}
	virtual m4int32_t	DecrementUsedLogicConn(void){return 1;}

	virtual m4pcchar_t	GetVersionCheckingKey(void){return TECH_SRV_VERSION_KEY;}
	
protected:
	ClSyncroWait	m_oLDBSWLConn;
	m4int16_t		m_numConn;
};

#endif
