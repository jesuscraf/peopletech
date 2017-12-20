//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_block.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             01/09/97
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


#ifndef _CH_BLOCK_HPP_
#define _CH_BLOCK_HPP_


#ifndef _M4CHCOMMON_HPP_
#include "m4types.hpp"
#endif
#include "dm_types.hpp"

#include "handles.hpp"
#include "mmindex.hpp"
#include "rec_tran.hpp"
#include "gds.hpp"
#include "pl_data.hpp"

#ifdef _DEBUG
#include "checkmem.hpp"
#endif

class ClRegister_Pipe;
class ClAccess;
class ClNode;
class ClNodeDef;
class ClRecordSet;
class _ClRegister_Base;
class ClItemProxy_Register;
class ClItemProxy_Block;
class ClItemProxy_Node;
class ClBlock_Channel;
class ClBlock_Channel_Proxy;
class ClBlock_Nihil;
class ClOperation_Register;
class ClBSearchIndex;
class ClPartialLoadData; //datos para la carga parcial
class ClNode_Data;
class _ClMemoryManager;


///////////////////////////////////////////////////////////////////////////////
//
// ClBlock_Channel
//

class ClBlock_Channel : public ClChangeListener
{
private:
        m4bool_t			m_bFlagReLoad;
        m4bool_t			m_bFlagPersist;
        ClPartialLoadData	*m_pClPL_Data;

		m4uint32_t			m_iRangeSize;
		m4uint32_t			m_iRangeStart;

		m4uint32_t			m_iTimeOut;

		m4uint8_t			m_iLastLoadStatus;
		m4uint32_t			m_iLastLoadRows;
		m4uint32_t			m_iLastLoadFetchs;
	
public:
	/** Items a nivel de bloque. */
	ClRecordWithTransactions Block;
	
	/** Items a nivel de registro. */
	ClGenericDataSet Records;     


	/** Constructor. */
	ClBlock_Channel (_ClMemoryManager* const ai_poMemoryManager)
		: m_pClPL_Data (0)
		, m_bFlagReLoad(M4_FALSE)
		, m_bFlagPersist(M4_FALSE)
		, m_iRangeSize(0)
		, m_iRangeStart(0)
		, m_iTimeOut(0)
		, m_iLastLoadStatus(0)
		, m_iLastLoadRows(0)
		, m_iLastLoadFetchs(0)
	{
#ifdef _DEBUG
		g_StMemInfo.iBlock_Channel++;
#endif
		Records.SetpMemoryManager(ai_poMemoryManager);
		Block.SetpMemoryManager(ai_poMemoryManager);
	}
	
	/** Destructor. */
	~ClBlock_Channel ()
	{
#ifdef _DEBUG
		g_StMemInfo.iBlock_Channel--;
#endif
		
		if (m_pClPL_Data) delete m_pClPL_Data;
	}
	
	
	// Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	/**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
	m4return_t SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
	{
		m4return_t res = Block.SetKeepStatus(ai_bKeepIt, ai_iVectorCount);
		if (res != M4_SUCCESS) return res;
		
		return Records.SetKeepStatus(ai_bKeepIt, ai_iVectorCount);
	}
	
	void ResetSerialMarks(void)
	{
		// ??? Por ahora la informacion a nivel de bloque viaja siempre, no se le ponen marcas
		// m_oDataBlock.Block.ResetSerialMarks(ai_bNewMark);
		Records.ResetSerialMarks();
	}
	
	// Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	void FindRecordAndSetVersion(const m4uint32_t ai_iRecordId, const VersionMark ai_iNewVersion, m4bool_t& ao_bContinue)
	{
		Block.FindRecordAndSetVersion(ai_iRecordId, ai_iNewVersion, ao_bContinue);
		if (ao_bContinue == M4_TRUE) {
			Records.FindRecordAndSetVersion(ai_iRecordId, ai_iNewVersion, ao_bContinue);
		}
	}
	
	size_t Count (void)
	{
		return Records.GetCount ();
	}
	
	//Ordenacion de indices para la carga parcial
	m4return_t R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg)
	{
		return Records.PrepareIndexPL(ai_bUp, ai_PrevReg);
	}

	void Attach (const ClNode_Data* const ai_pNodeData);
	
	m4return_t DeleteAll (void)
	{
		Records.Zap();
		return M4_SUCCESS;
	}
	
	m4return_t B_Undo(void);
	
	m4return_t R_Undo(ClMMIndex ai_regindex)
	{
		regindex_t aux = (m4uint32_t) ai_regindex;
		return Records.Undo(aux);
	}
	
	m4return_t CheckPoint (ClMMIndex ai_regindex, m4bool_t& ai_bDeleted)
	{
		regindex_t aux = (m4uint32_t) ai_regindex;
		return Records.CommitRecord(aux, ai_bDeleted);
	}
	
	m4return_t SetCheckPointMode (m4bool_t ai_operation)
	{
		return Records.UseCheckPoints(ai_operation);
	}
	
	m4bool_t GetCheckPointMode(void) const
	{
		return Records.UsingCheckPoints();
	}
	
	m4return_t RollbackPersisted(void);
        
	void UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n)
	{
		// Aquí hay que poner un semáforo como un burro: el bloque no se queda consistente
		// hasta la salida de la función.
		// Lo último es llamar al gestor de memoria para que cambie los índices internamente:
		Records.UpdateIndex(ai_pindex_qtc);
	}
	
	
	//*******************************************************************
	// NUEVAS FUNCIONES DE OPERACION
	//*******************************************************************
	
	m4return_t R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus);
	m4return_t R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus);
	
	m4bool_t R_IsModified		(const ClMMIndex ai_index) const;
	m4bool_t R_IsNew		(const ClMMIndex ai_index) const;
	m4bool_t R_IsUpdated		(const ClMMIndex ai_index) const;
	m4bool_t R_IsDeleted		(const ClMMIndex ai_index) const;
	m4bool_t R_IsNewAndDeleted	(const ClMMIndex ai_index) const;
	
	// Funcion implementada en DS
	m4bool_t R_IsUpdatedByDDBB  (const ClMMIndex ai_index) const;
	
	m4bool_t R_IsModified    (void) const;
	m4bool_t R_IsNew         (void) const;
	m4bool_t R_IsUpdated     (void) const;
	m4bool_t R_IsDeleted     (void) const;
	
	// Funcion implementada en DS
	m4bool_t R_IsUpdatedByDDBB  (void) const;
	
	m4bool_t B_IsModified    (void) const;
	m4bool_t B_IsNew         (void) const;
	m4bool_t B_IsUpdated     (void) const;
	m4bool_t B_IsDeleted     (void) const;
	
	// Funcion implementada en DS
	m4bool_t B_IsUpdatedByDDBB  (void) const;
	
	m4return_t B_RefreshFlags   (void);
	
	//************************************************************
	//************************************************************
	
	m4bool_t R_IsValid    (ClMMIndex ai_index)
	{
		return Records.IsRecordValid(ai_index);
	}
	
	m4return_t R_SetUpdated (ClMMIndex ai_index)
	{
		return Records.SetStatus(ai_index, M4_RS_NEW);
	}
	
	m4return_t R_GetVersion (ClMMIndex ai_iIndex, VersionMark& ao_Version);
	
	m4return_t R_SetVersion (ClMMIndex ai_index, VersionMark& ai_Version);
	
	m4bool_t R_GetRecordPersist (ClMMIndex ai_Iindex) const
	{
		return Records.GetRecordPersistLDB(ai_Iindex);
	}
	
	m4bool_t GetBlockPersistLDB(void) const
	{
		return Block.GetRecordPersistLDB(0);
	}
	
	m4return_t R_SetRecordPersist(const ClMMIndex ai_iIndex, const m4bool_t ai_persist)
	{
		m4return_t iRet = Records.SetRecordPersistLDB(ai_iIndex, ai_persist);
		if (iRet != M4_SUCCESS) return iRet;
		
		// I assume the responsability of assigning the block flag
		// ??? (Alex) ojo que esto ya se utiliza en CASI todas las funciones, se esta haciendo dos veces
		return SetBlockPersistLDB(ai_persist);
	}
	
	m4return_t SetBlockPersistLDB(const m4bool_t ai_Dirty)
	{
		return Block.SetRecordPersistLDB(0, ai_Dirty);
	}
	
	m4return_t B_GetPersist (m4bool_t& ai_persist) const
	{
		ai_persist = m_bFlagPersist;
		return M4_SUCCESS;
	}
	
	m4return_t B_SetPersist (const m4bool_t ai_persist)
	{
		m_bFlagPersist = ai_persist;
		return M4_SUCCESS;
	}
	
	m4return_t B_GetReLoad (m4bool_t& ao_rl) const
	{
		ao_rl = m_bFlagReLoad;
		return M4_SUCCESS;
	}
	
	m4return_t B_SetReLoad (const m4bool_t ai_rl)
	{
		m_bFlagReLoad = ai_rl;
		return M4_SUCCESS;
	}
	
	m4return_t R_AddRecordAndSetLDBItems(ClMMIndex & ao_Index, ClRegister_Pipe* ai_pRegPipe, m4uint32_t ai_iUniqueRowId, m4bool_t ai_bAddRegister = M4_TRUE, m4bool_t ai_bAuxLoad = M4_FALSE)
	{
		regindex_t aux = ao_Index;
		
		m4return_t res = Records.AddRecordAndSetLDBItems(aux, ai_pRegPipe, ai_bAddRegister, ai_iUniqueRowId, ai_bAuxLoad);
		
		if ( (res == M4_SUCCESS) && (ai_bAddRegister) ) {
			ao_Index = aux;
		}
		
		return res;
	}
	
	m4return_t B_AddRegister (ClMMIndex &ao_newindex, m4uint32_t ai_iUniqueRowId)
	{
		regindex_t aux;
		
		m4return_t res = Records.AddRecord(aux, ai_iUniqueRowId);
		
		if (res == M4_SUCCESS) {
			ao_newindex = aux;
		}
		
		return res;
	}
	
	m4return_t R_InsertRegister (ClMMIndex ai_regindex, m4uint32_t ai_iUniqueRowId)
	{
		return Records.InsertRecord(ai_regindex, ai_iUniqueRowId, 1);
	}
	
	m4return_t R_DeleteRegister (ClMMIndex ai_regindex)
	{
		return Records.DeleteRecord(ai_regindex);
	}
	
	m4return_t R_Destroy (ClMMIndex ai_regindex)
	{
		m4return_t	res = Records.DestroyRecord(ai_regindex);

		if( m_iLastLoadStatus != 0 && Count() == 0 )
		{
			SetLastLoadStatus( 0, 0, 0 ) ;
		}
		return res;
	}
	
	m4uint32_t R_GetRecordID (const ClMMIndex ai_regindex) const
	{
		return Records.GetRecordID(ai_regindex);
	}
	
	void R_SetRecordID (const ClMMIndex ai_regindex, const m4uint32_t ai_iRecordID)
	{
		Records.SetRecordID(ai_regindex, ai_iRecordID);
	}

	m4return_t Reserve(const regindex_t ai_iSize)
	{
		return Records.Reserve(ai_iSize);
	}
	
	ClPartialLoadData * GetpPL_Data(void)
	{
		if (!m_pClPL_Data) {
			m_pClPL_Data = new ClPartialLoadData(0);
		}
		return m_pClPL_Data;
	}

	m4uint32_t	GetRangeSize( void ) const
	{
		return m_iRangeSize;
	}

	m4uint32_t	GetRangeStart( void ) const
	{
		return m_iRangeStart;
	}

	void	SetRangeSize( m4uint32_t ai_iRangeSize )
	{
		m_iRangeSize = ai_iRangeSize;
	}

	void	SetRangeStart( m4uint32_t ai_iRangeStart )
	{
		m_iRangeStart = ai_iRangeStart;
	}
	
	void	SetRangeStartNext( void )
	{
		m_iRangeStart += m_iRangeSize;
	}
	
	m4uint32_t	GetTimeOut( void ) const
	{
		return m_iTimeOut;
	}

	void	SetTimeOut( m4uint32_t ai_iTimeOut )
	{
		m_iTimeOut = ai_iTimeOut;
	}

	void	GetLastLoadStatus( m4uint8_t &ao_riLastLoadStatus, m4uint32_t &ao_riLastLoadRows, m4uint32_t &ao_riLastLoadFetchs ) const
	{
		ao_riLastLoadStatus = m_iLastLoadStatus;
		ao_riLastLoadRows = m_iLastLoadRows;
		ao_riLastLoadFetchs = m_iLastLoadFetchs;
	}

	void	SetLastLoadStatus( m4uint8_t ai_iLastLoadStatus, m4uint32_t ai_iLastLoadRows, m4uint32_t ai_iLastLoadFetchs )
	{
		if( ai_iLastLoadStatus != m_iLastLoadStatus )
		{
			m_iLastLoadStatus = ai_iLastLoadStatus;
			SetChange() ;
		}

		if( ai_iLastLoadRows != m_iLastLoadRows )
		{
			m_iLastLoadRows = ai_iLastLoadRows;
			SetChange() ;
		}

		if( ai_iLastLoadFetchs != m_iLastLoadFetchs )
		{
			m_iLastLoadFetchs = ai_iLastLoadFetchs;
			SetChange() ;
		}
	}
	
	m4uint64_t GetBlockTimeStamp( void ) const
	{
		m4uint64_t	iTimeStamp = Block.GetTimeStamp() ;

		if( m_iTimeStamp > iTimeStamp )
		{
			iTimeStamp = m_iTimeStamp ;
		}
		return( iTimeStamp ) ;
	}

	m4uint64_t GetRecordTimeStamp( void ) const
	{
		return( Records.GetTimeStamp() ) ;
	}

	m4return_t GetSize(ClSize &ao_size);

	friend class ClBlock_Channel_Proxy;
};

#endif  // __CH_BLOCK__HPP__ 
