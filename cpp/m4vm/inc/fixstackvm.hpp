
#ifndef _FIXED_STACKVM_H
#define _FIXED_STACKVM_H


#include "fixstack.hpp"

#include "vmtypes.hpp"

#include "vmres.hpp"

#include "access.hpp"


//ESPECIALIZACION PARA el tipo m4VMState_t->Para sacar los mensajes de error apropiados
template<>
inline m4return_t tpFixedStack<m4VMState_t>::Push(const m4VMState_t &ai_tElement){
	
	m4return_t ret;
	
	if(m_iTop==m_iMax){
		ret=Resize(m_iMax*2);
		if (ret!=M4_SUCCESS){
			if (ai_tElement.m_hItem!=-1){
				g_oChLog<<BeginError ( M4_CH_VM_CANT_RESIZE_STATE_STACK )<<M4ObjNodeItemI(ai_tElement.m_poAccess->GetpCMCR(),ai_tElement.m_hItem) <<EndLog;
			}
			else{
				g_oChLog<<BeginError ( M4_CH_VM_CANT_RESIZE_STACK )<<EndLog;
			}
			return ret;
		}
	}

	
	m_tData[m_iTop]=ai_tElement;
	m_iTop++;
	ret=M4_SUCCESS;
	
	return ret;
}

#endif
