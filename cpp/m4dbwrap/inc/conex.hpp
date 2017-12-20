//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conex.hpp   
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

#ifndef  _CONEX_HPP_
#define  _CONEX_HPP_

#include "m4dbwrap_dll.hpp"
#include "m4dbwrapdef.hpp"
#include "cursor.hpp"


typedef vector<ClCursor *>				VcClCursor;
typedef VcClCursor*						PVcClCursor;
typedef VcClCursor::iterator			itVcClCursor;

class ClDBConnection_Base;
class ClTrnSQL;
struct stConexionProperties;

class M4_DECL_M4DBWRAP ClDBConnection
{

	m4return_t ExecuteSQL (m4pcchar_t ai_pSQL);

	m4return_t Clone (ClDBConnection *&ao_pConn);

public:
	ClDBConnection();
	~ClDBConnection ();  

	m4return_t Init (m4int32_t ai_idConn, stConexionProperties *ai_pstConProp){
		m_pstConProp = ai_pstConProp;
		m_idConn = ai_idConn;
		return M4_SUCCESS;
	}
	m4return_t End(void); 
	
	m4return_t Disconnect (void);

	m4return_t Transact (eTransType_t ai_eTransType);

	m4return_t GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString);

	m4return_t IncNumCursor (ClCursor *ai_pCursor);
	
	m4return_t DecNumCursor (ClCursor *ai_pCursor = NULL);

	eDDBBType_t GetDDBBType (void);

	m4return_t InitConnection (void);

	m4return_t InitTransaction (void);

	m4int32_t  GetIdConn (void);

	void	   GetConnProp(stConexionProperties &ao_stConProp){
		if (m_pstConProp){
			ao_stConProp = *m_pstConProp;
		}
	}

	void AllowOperationsWhenDDBBIsDisabled (m4bool_t ai_bAllowOperationsWhenDDBBIsDisabled)
	{
		m_bAllowOperationsWhenDDBBIsDisabled = ai_bAllowOperationsWhenDDBBIsDisabled;
	}

private :

	m4int32_t m_idConn;

	ClDBConnection_Base* m_pConB;

	stConexionProperties *m_pstConProp;
	
	eLastError_t m_eLastError;
	
	VcClCursor m_vcCursor;

	m4bool_t m_bAllowOperationsWhenDDBBIsDisabled;

	friend class ClCursor;
}; 

M4_DECL_M4DBWRAP void DisableDDBB (void);
M4_DECL_M4DBWRAP void EnableDDBB (void);
M4_DECL_M4DBWRAP m4bool_t IsDDBBDisabled (void);


#endif
