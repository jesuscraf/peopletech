#ifndef _CLDMCONTEXT_
#define _CLDMCONTEXT_

#include "m4dm_dll.hpp"

#include "cldefine.hpp"

 
class M4_DECL_M4DM ClDMContext {

protected:
	m4uint32_t	m_uiCSType;	// Tipo CS
	m4bool_t	m_bIsSM;	// SingleMachine S/N

	ClDMContext(m4bool_t ai_bIsSM) {
		m_uiCSType	= M4CL_CSTYPE_FRONT;
		m_bIsSM		= ai_bIsSM; 
	}

public:
	virtual ~ClDMContext() {}

	virtual m4bool_t	ExecutingInClient(void) = 0 ;
	m4uint32_t			GetCSType()		{ return m_uiCSType; }
	m4bool_t			IsSM(void)		{ return m_bIsSM; }
};


class M4_DECL_M4DM ClDMContextServer : public ClDMContext {

public:
	ClDMContextServer(m4bool_t ai_bIsSM) 
		: ClDMContext(ai_bIsSM) { 
		m_uiCSType = M4CL_CSTYPE_BACK;
	}

	virtual m4bool_t	ExecutingInClient(void) { return M4_FALSE; }
};


class M4_DECL_M4DM ClDMContextClient : public ClDMContext {

public:
	ClDMContextClient(m4bool_t ai_bIsSM) 
		: ClDMContext(ai_bIsSM) { 
		m_uiCSType = M4CL_CSTYPE_FRONT;
	}

	virtual m4bool_t	ExecutingInClient(void) { return M4_TRUE; }
};

#endif

