//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             access.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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
#ifndef __ACCESS__HPP__
#define __ACCESS__HPP__

#include "m4dm_dll.hpp"
#include "dm_types.hpp"
#include "acno_dir.hpp"
#include "acc_prop.hpp"
#include "chanprop.hpp"
#include "autoload.hpp"
#include "channel.hpp"


class ClRegister;
class ClChannel;
class ClAccess;
class ClDataBlock;
class ClCompiledMCR;
class ClRegister_Current;
class ClStackInterface;
class ClVMBaseEnv;

class ClAccessRecordSet;
class ClRecordSet;
class IExecutor;
class ClVMachine2;

class ClSize;
class ClNotifier;

class ClLConn;

//---------------------------------->>
//totalmente provisional:

#define M4CMPROP_ACCESS_EXECUTION_DATE       "EXECUTION_DATE"
#define M4CMPROP_ACCESS_END_DATE             "END_DATE"
#define M4CMPROP_ACCESS_START_DATE           "START_DATE"
#define M4CMPROP_ACCESS_RUN_SLICE            "RUN_SLICE"

// Notificaciones
#define M4_NOTIF_ACCESS_DESTROY				0x00000001+M4_NOTIF_ACCESS_BASE
///////////////////////////////////////////////////////////////////////////////
//
// ClAccess
//

class M4_DECL_M4DM ClAccess : public ClAutoLoad_PlugIn
{
public:
    m4return_t Destroy (void);

    m4return_t Reset (void);  //para evitar problemas

    //----------------------------------------
    //propiedades, provisional aquí:
	// Fixed properties
    ClGenericProperty  ExecutionDate;
    ClGenericProperty  ExecutionStartDate;
    ClGenericProperty  ExecutionEndDate;
    ClGenericProperty  RunSliceMode;

	m4return_t			CopyProperties(ClAccess &ai_access);

    ClColProp                Property;
    //----------------------------------------

    ClNodeAccessDirectory    Node;

//autoload
    m4return_t SetAutoLoadMode (m4uint8_t ai_mode);

    ClChannel*          GetpChannel (void) const
    {
        return m_pClChannel;
    }
    
        //a extinguir:
    ClCompiledMCR*      GetpCMCR (void) const;

    ClVMBaseEnv*        GetEnvironment (void) const
	{
		if (m_pClChannel)
		{
			return m_pClChannel->GetEnvironment(); 
		}
		return NULL;
	}

	IExecutor*			GetpExecutor (void) const;

	m4uint32_t GetHandle ( void ) const;

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);

    void Follow (ClAccess &ai_access);
	
	
    m4return_t Load (m4bool_t ai_bFromWrapper=M4_FALSE);
	m4return_t UnLoad (void);
    m4return_t Persist (m4bool_t ai_bFromWrapper=M4_FALSE);
	m4return_t ReLoad (void);

	m4return_t LoadFromFile (m4pchar_t ai_pcFileName);
	m4return_t LoadFromSttFile (m4pchar_t ai_pcFileName);
	m4return_t PersistToFile (m4pchar_t ai_pcFileName, m4bool_t ai_bSerializeMCR);
	m4return_t PersistToTempFile (m4pchar_t ao_pcFileName, m4uint32_t ai_iSize);

	/** Geter para el modo de depuración */
	ClChannel::eDebugMode_t GetDebugMode(void) const
	{
		return m_eDebugMode;
	}

	/** Seter para el modo de depuración */
	void SetDebugMode(ClChannel::eDebugMode_t ai_eDebugMode)
	{
		m_eDebugMode = ai_eDebugMode;
	}

    void SetRemoteId(m4uint32_t ai_iRemoteId)
	{
		m_iRemoteId = ai_iRemoteId;
	}

	m4uint32_t GetRemoteId() const
	{
		return m_iRemoteId;
	}

	m4return_t GetSize(ClSize &ao_size);

	// Notificadores.
	ClNotifier	*m_pDestroy_Notifier;	// Destrucción

private:    
    ClChannel*     m_pClChannel;    // referencia al canal al que pertenece el access    

    //ClChannel:
    m4return_t Init (ClChannel *ai_pClChannel);

    //ClChannel:
    // void *operator new (unsigned int ai_size, m4uint16_t ai_nodescount);
	void *operator new (size_t ai_size, m4uint16_t ai_nodescount);
    ClAccess (m4uint32_t ai_iHandle);
   ~ClAccess ();    

	// for proxy's
	/** Para identificar el acceso en el servidor. */
	m4uint32_t m_iRemoteId;

	/** Indica el estado de depuración del acceso. */
	ClChannel::eDebugMode_t m_eDebugMode;

protected:
    m4return_t _RootConnectedExecution( m4pcchar_t ai_pccMethod, m4uint8_t ai_iPrecedence, m4bool_t ai_bFromWrapper ) ;
    m4return_t _RootExecution( m4bool_t ai_bLoad, m4bool_t ai_bFromWrapper ) ;

	// Identificador único.
	m4uint32_t	m_iHandle;

//---------------
//lane:
public:
    void IncLaneCount (void)
    {
        m_iLaneCount++;
    }

    void DecLaneCount (void)
    {
        m_iLaneCount--;
    }

    m4bool_t IsConnected (void) const
    {
        return M4_BOOL(m_iLaneCount != 0);
    }

private:
    m4uint32_t m_iLaneCount;

friend class ClChannel;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClAccess &ai_access);

#endif  //__ACCESS__HPP__ 
