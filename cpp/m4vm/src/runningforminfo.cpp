//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             ExecutionPoint.cpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Execution point
//
//
//==============================================================================

#include "runningforminfo.hpp"
ClRunningFormInfo::ClRunningFormInfo(): m_avArgs(0),
										m_iNumberofArguments(0),
										m_dRunningStart(0),
										m_dRunningEnd(0),
										m_dRunningCut(0),
										m_bVariableArgs(M4_FALSE),
										m_hChannel(0),
										m_hAccess(0),
										m_bIsClient(M4_TRUE)
{
}
ClRunningFormInfo::~ClRunningFormInfo(){ 
	delete []m_avArgs;
}

ClRunningFormInfo	&ClRunningFormInfo::operator=(const ClRunningFormInfo	&rhs){
	
	//la base
	*((ClExecutionPoint*)this)=*((ClExecutionPoint*)&rhs);
	m_dRunningStart=rhs.m_dRunningStart ;
	m_dRunningEnd=rhs.m_dRunningEnd;
	m_dRunningCut=rhs.m_dRunningCut;
	m_bVariableArgs=rhs.m_bVariableArgs;
	SetNumberofArguments(rhs.m_iNumberofArguments);
	m_hChannel=rhs.m_hChannel;
	m_hAccess=rhs.m_hAccess;
	m_bIsClient=rhs.m_bIsClient;
	for (m4uint32_t i=0;i<rhs.m_iNumberofArguments;++i){
		m_avArgs[i]=rhs.m_avArgs[i];
	}
	return *this;
}

//--------------------------------
//Setters
//--------------------------------

void ClRunningFormInfo::SetNumberofArguments(m4uint32_t ai_iNumArgs){
	if (ai_iNumArgs>m_iNumberofArguments){
		delete []m_avArgs;
		m_avArgs= new m4VariantType [ai_iNumArgs];
	}
	m_iNumberofArguments=ai_iNumArgs;
}

m4return_t ClRunningFormInfo::SetArgument	(const m4VariantType &ai_vArg,m4uint32_t ai_iPos){
	if (ai_iPos >=m_iNumberofArguments) {
		return M4_ERROR;
	}
	m_avArgs[ai_iPos]=ai_vArg;
	return M4_SUCCESS;
}

void ClRunningFormInfo::SetIsVariable	(m4bool_t	ai_bIsVariable) {
	m_bVariableArgs=ai_bIsVariable;
}

void ClRunningFormInfo::SetDates (m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun){
	m_dRunningStart=ai_dStart;
	m_dRunningEnd=ai_dEnd;
	m_dRunningCut=ai_dRun;
}
void ClRunningFormInfo::SetChannelHandle		(m4int32_t ai_hHandle){
	m_hChannel=ai_hHandle;
}

void ClRunningFormInfo::SetAccessHandle		(m4int32_t ai_hHandle){
	m_hAccess=ai_hHandle;
}
void ClRunningFormInfo::SetIsClientSide	(m4bool_t	ai_bIsClient) {
	m_bIsClient=ai_bIsClient;
}

//--------------------------------
//Getters
//--------------------------------
void ClRunningFormInfo::GetDates	(m4date_t &ao_dStart, m4date_t &ao_dEnd, m4date_t &ao_dRun){
	ao_dStart=m_dRunningStart;
	ao_dEnd=m_dRunningEnd;
	ao_dRun=m_dRunningCut;
}

m4uint32_t	ClRunningFormInfo::GetNumberofArguments	(void)  {
	return m_iNumberofArguments;
}

m4return_t	ClRunningFormInfo::GetArgument	(m4uint32_t ai_iPos, m4VariantType &ao_vValue){
	if (ai_iPos >=m_iNumberofArguments) {
		return M4_ERROR;
	}
	ao_vValue=m_avArgs[ai_iPos];
	return M4_SUCCESS;

}

m4return_t	ClRunningFormInfo::GetArguments	(m4VariantType	*ao_pvArgs,m4uint32_t ai_iSize){
	if (ai_iSize <m_iNumberofArguments) {
		return M4_ERROR;
	}
	for (m4uint32_t i = 0; i<m_iNumberofArguments; ++i){
		ao_pvArgs[i]=m_avArgs[i];
	}
	return M4_SUCCESS;
}

m4bool_t	ClRunningFormInfo::GetIsVariable(void) {
	return m_bVariableArgs;
}

m4int32_t	ClRunningFormInfo::GetChannelHandle		(void){
	return m_hChannel;
}

m4int32_t	ClRunningFormInfo::GetAccessHandle		(void){
	return m_hAccess;
}

m4bool_t	ClRunningFormInfo::GetIsClientSide		(void){
	return m_bIsClient;
}
