//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             executor.hpp
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
//    class executor, a vmachine wrapper
//
//
//==============================================================================

#ifndef __IEXECUTOR_HPP__
#define __IEXECUTOR_HPP__

#include "stack.hpp"
#include "m4vm_dll.hpp"

#include "vmcscontext.hpp" 


class ClGenericIODriver;
class ClItem;
class ClChannelManager;
class ClExecutorRuntimeInfo;
class ClStatus;

class ClStackInterface;

class M4_DECL_M4VM IExecutor
{
public:
	IExecutor(IStackInterface &ai_roStack):Stack(ai_roStack){}


	virtual ~IExecutor() {};
    virtual m4return_t Init (ClVMCSContext *ai_poContext1,ClVMCSContext *ai_poContext2=0) =0;   

	
 
   
    // Functions for serialization 
    virtual m4return_t Serialize(ClGenericIODriver& IOD) = 0;
    virtual m4return_t DeSerialize(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager) = 0;

    IStackInterface        &Stack;

	//Para la ejecución
	virtual	m4bool_t	IsRunning	(void) = 0;
	virtual m4return_t	Call		(ClItem *ao_poItem,m4int32_t ai_iArgs=-1, m4bool_t ai_bEnableDebug=M4_FALSE, m4bool_t ai_bNoInsiteRec=M4_FALSE) = 0;
	virtual m4return_t	Execute		(void) = 0;
	
	virtual m4return_t	RequestAbort(m4bool_t ai_bRequest=M4_TRUE) = 0; 
	virtual m4return_t	SetStatusLabel(m4pcchar_t ai_pszLabel) = 0;
	virtual m4return_t	GetStatusLabel(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize) = 0;
	virtual m4uint32_t	GetSize (void) const = 0;

	virtual void Reset			(void) = 0;
	virtual	void ResetStack		(void) = 0;
	virtual	void ResetDebugging	(void) = 0;

	virtual string		DumpCallStack	(void) = 0;//Volcar pila de llamadas de la VM
	virtual m4return_t	GetRTInfo		(ClExecutorRuntimeInfo *ai_poRTInfo)= 0 ;
	virtual void		SetpCancelStatus(ClStatus *ai_poCancelStatus = NULL) = 0;
	virtual m4bool_t	GetCancelStatus		(void) = 0;

	virtual ClVMStatisticsWrapper *GetSttWrap(void)=0;

	/**
	 * Petición para abortar una ejecución.
	 * Es distinto del RequestToAbort porque tiene en cuenta si tenemos asignado un ClStatus
	 * (por ejemplo en el caso de estar ejecutando en el servidor). En tal caso la petición para
	 * abortar se hace a través de este objeto.
	 *
	 * @param ai_bRequestToAbort    M4_TRUE  --> se pide que se aborte la ejecución.
	 *								M4_FALSE --> se pide que la ejecución continue con normalidad.
	 */
	virtual void RequestToAbortWithClStatus(const m4bool_t ai_bRequestToAbort) = 0;	

	/**
	 * Nos devuelve si ha sido abortada una ejecución.
	 */
	virtual m4bool_t GetRequestToAbortWithClStatus(void) const = 0;	
};

M4_DECL_M4VM inline ostream & operator << (ostream &ao_Output, IExecutor &ai_oClExecutor)
{
	ao_Output << endl << "Starting executor dump info." << endl;
	ao_Output << ai_oClExecutor.DumpCallStack();
	ao_Output << endl << "Finishing executor dump info." << endl;
	return ao_Output;
}


#endif // __IEXECUTOR_HPP__
