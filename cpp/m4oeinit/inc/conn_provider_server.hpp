#ifndef _CONN_PROVIDER_SERVER_HPP_
#define _CONN_PROVIDER_SERVER_HPP_

#include "conn_provider.hpp"
#include "m4oeinit_dll.hpp"

class M4_DECL_M4OEINIT ClLConn_Provider_Server : public ClLConn_Provider{
	virtual ClLConn_Interface	*GetConnection(ClChannel *ao_pChannel=0, m4bool_t ai_NewConn=M4_TRUE);
	virtual m4return_t	SetConnection  (ClLConn_Interface	 *ai_poClConn = 0, m4bool_t ai_bIgnoreReferences=M4_FALSE);
	virtual m4return_t End(void);
};

#endif // _CONN_PROVIDER_SERVER_HPP_

