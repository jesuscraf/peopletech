#ifndef _EXECONT_H
#define _EXECONT_H

#include "m4vm_dll.hpp"
#include "m4types.hpp"
#include "index.hpp"
#include "chlog.hpp"


class ClVMachine1;
class ClAccess;
class ClAccessRecordSet;
class ClLogonAdaptor;

//contexto para pasar a objectos externos
struct ClVMRunContext
{
    ClVMachine1			*m_pVM;
    ClAccess			*m_pAccess;
    ClAccessRecordSet	*m_pRecordSet;
    ClRegisterIndex		m_oRegIndex; 
    m4uint32_t			m_hItem;
	ClLogonAdaptor		*m_pLA;
};

M4_DECL_M4VM m4return_t	GetExecutionLine( ClVMRunContext* ai_pvContext, m4int32_t &ao_riLine, m4pcchar_t &ao_rpccItemId, m4pcchar_t &ao_rpccItemName, m4pcchar_t &ao_rpccNodeId, m4pcchar_t &ao_rpccNodeName, m4pcchar_t &ao_rpccM4ObjId, m4pcchar_t &ao_rpccM4ObjName ) ;

typedef	BasicLogManip1<ChLog, ClVMRunContext*>	ChLogManip_vmcontext_dump_t ;

M4_DECL_M4VM ChLogManip_vmcontext_dump_t	DumpVMContext( ClVMRunContext* ai_pvContext ) ;


#endif	// _EXECONT_H
