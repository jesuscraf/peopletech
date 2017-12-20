//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             execntxt.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef _EXECONTXT_HPP_
#define _EXECONTXT_HPP_


#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "clsrlzbl.hpp"

class IExecutor;
class ClAccess;
class ClChannelManager;


// Este objeto se va a identifacar de la siguiente manera:
// ObjectType       = OLTPExecutionContextType
// EnvironmentId    = NULL
// TypedId          = "OLTPExecutionContext"
// UniqueKey        = Numero de OLTPExecutionContext (0, 1, 2, ...)

class M4_DECL_M4OEINIT ClOLTPExecutionContext: public ClSerializableObject
{
public:
	ClOLTPExecutionContext(ClChannelManager& ai_channelManager, ClAccess* const ai_pAccess);
	
	IExecutor* const GetExecutor(void) const;
	
	ClAccess* const GetAccess(void) const;


	/** Interface de serializacion. */
	virtual m4return_t Serialize(ClGenericIODriver& IOD);
	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);
	static  m4uint16_t GetSerializationVersion(void);


	m4bool_t GetbSendAccessPointer(void) {
		return m_bSendAccessPointer;
	}
	void SetbSendAccessPointer(m4bool_t ai_bSendAccessPointer) {
		m_bSendAccessPointer = ai_bSendAccessPointer;
	}

private:
	/** Ejecutor, ahora solo hay uno por VM (antes uno por Access). */
	IExecutor* m_poExecutor; 

	/** Access que hay que ejecutar. */
	ClAccess* m_poAccess;     
	
	/** Para extraer el ejecutor y para poder hacer la deserializacion. */
	ClChannelManager* m_poChannelManager;

	m4bool_t m_bSendAccessPointer;
};

#endif
