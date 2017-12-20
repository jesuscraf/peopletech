//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ExeRTInfo.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    class RT info
//
//
//==============================================================================
#include "exertinfo.hpp"
#include "m4mdrt.hpp"
#include "cldefine.hpp"

ClExecutorRuntimeInfo::ClExecutorRuntimeInfo(){
	m_avArgs=0;
	Reset();
}

ClExecutorRuntimeInfo::~ClExecutorRuntimeInfo(){
	delete [] m_avArgs;
}

void ClExecutorRuntimeInfo::Reset(void){
	m_hItem=0;
	delete [] m_avArgs; m_avArgs=0;
	m_iNumArgs=0;
	m_poCMCR=0;
}
//--------------------------------------------
//Formateo
//--------------------------------------------
string ClExecutorRuntimeInfo::GetInfoAsString(void){

	string str;
	m4uint32_t i;
	m4VariantType vTemp;
	m4uchar_t cAfter, cBefore;

	if (m_poCMCR){
		str.append(m_poCMCR->GetChannelId() );
		str.append("!");
		str.append(m_poCMCR->iNodeI(m_hItem) );
		str.append(".");
		str.append(m_poCMCR->iItemI(m_hItem) );
		str.append(" (");
		//parámetros
		for (i=0;i<m_iNumArgs; ++i){
			vTemp=m_avArgs[i];
			switch (vTemp.Type){
			case M4CL_CPP_TYPE_STRING_VAR:
			case M4CL_CPP_TYPE_STRING_SYS:
				{
					cAfter=cBefore='\"';
					break;
				}
			case M4CL_CPP_TYPE_DATE:
				{
					cAfter='}';
					cBefore='{';
					break;
				}
			default:
				{
					cAfter=cBefore=0;
				}

			}

			m_oMediator.ToCharString(vTemp);
			str.append(" ");
			if (cBefore){
				str.append(allocator<char>::size_type(1),cBefore);
			}
			str.append(m4pchar_t (vTemp) );
			
			if (cAfter){
				str.append(allocator<char>::size_type(1),cAfter);
			}

			if (i<m_iNumArgs-1){
				str.append(",");
			}
		}
		str.append(" )");
		
	}
	return str;
}

//--------------------------------------------
//Set/Get Item
//--------------------------------------------
m4uint32_t	ClExecutorRuntimeInfo::GetItem(void){
	return m_hItem;
}

ClCompiledMCR *ClExecutorRuntimeInfo::GetpCMCR(void){
	return m_poCMCR;
}

void ClExecutorRuntimeInfo::SetItem(ClCompiledMCR *ai_poCMCR, m4uint32_t  ai_hItem){
	m_poCMCR=ai_poCMCR;
	m_hItem=ai_hItem;
}

//--------------------------------------------
//Número de argumentos
//--------------------------------------------
void ClExecutorRuntimeInfo::SetArgNum(m4uint32_t ai_iNumArg){
	delete[] m_avArgs;
	m_avArgs=new m4VariantType [ai_iNumArg];
	m_iNumArgs=ai_iNumArg;
}

m4return_t ClExecutorRuntimeInfo::SetArg   (const m4VariantType &ai_vArg, m4uint32_t ai_iPos){
	if (!m_avArgs || ai_iPos>=m_iNumArgs ) return M4_ERROR;
	m_avArgs[ai_iPos]=ai_vArg;
	return M4_SUCCESS;
}

//--------------------------------------------
//Argumentos
//--------------------------------------------
m4uint32_t	ClExecutorRuntimeInfo::GetArgNum(void){
	return m_iNumArgs;
}

m4return_t	ClExecutorRuntimeInfo::GetArg   (m4VariantType &ai_vArg, m4uint32_t ai_iPos){
	if (!m_avArgs || ai_iPos>=m_iNumArgs ) return M4_ERROR;
	ai_vArg=m_avArgs[ai_iPos];
	return M4_SUCCESS;

}

