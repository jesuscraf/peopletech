#ifndef _CONN_PROVIDER_DIC_HPP_
#define _CONN_PROVIDER_DIC_HPP_

#include "conn_provider.hpp"
#include "chlog.hpp"
#include "csres.hpp"
#include "m4oeinit_dll.hpp"

class M4_DECL_M4OEINIT ClLConn_Provider_DIC : public ClLConn_Provider{
	virtual ClLConn_Interface	*GetConnection(ClChannel *ao_pChannel=0, m4bool_t ai_NewConn=M4_TRUE){
		if (ai_NewConn){
			DUMP_CHLOG_ERROR (M4_CH_MCR_GET_CONNECT_IN_DIC);
		}
		return 0;
	}

};

#endif // _CONN_PROVIDER_DIC_HPP_