#ifndef _CONN_PROVIDER_HPP_
#define _CONN_PROVIDER_HPP_

#include "m4oeinit_dll.hpp"

#include "m4types.hpp"

class ILdb;
class ClLConn_Interface;
class ClChannel;
class ClLogonAdaptor;

class M4_DECL_M4OEINIT ClLConn_Provider {
public:
	virtual ClLConn_Interface	*GetConnection(ClChannel *ao_pChannel=0, m4bool_t ai_NewConn=M4_TRUE){return 0;};
//	virtual m4return_t FreeConnect(){return M4_ERROR;}
	virtual m4return_t	SetConnection  (ClLConn_Interface	 *ai_poClConn = 0, m4bool_t ai_bIgnoreReferences=M4_FALSE){return M4_ERROR;}
	virtual m4return_t Init(ILdb *ai_pLdb){
		m_pLdb=ai_pLdb;
		m_pConnection = 0;
		return M4_SUCCESS;
	}
	virtual m4return_t End(void){
		m_pLdb=0;
		m_pConnection = 0;
		return M4_SUCCESS;
	}


protected:
	ClLConn_Provider(void):m_pLdb(0),m_pConnection(0),m_indConnection(0){}
	ILdb *m_pLdb;
	ClLConn_Interface *m_pConnection;
	m4int32_t m_indConnection;
};


#endif
