//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             px_block.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             07/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Proxy block, the famous block 'seed'
//
//
//==============================================================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "dm_types.hpp"
#include "dstypes.hpp"

#include "index.hpp"
#include "mmindex.hpp"
#include "handles.hpp"
#include "changelistener.hpp"
//## end module.includes preserve=yes

#ifndef __PX_BLOCK__HPP__
#define __PX_BLOCK__HPP__

class _ClBlock_Channel_Base;
class _ClRecordSet_Base;
class ClAccess;
class ClBlock_Channel;
class ClBlockReference;
class ClBSearchIndex;
class ClChannel;
class ClChannelProcess;
class ClFileName;
class ClGenericIODriver;
class ClItem;
class ClNode;
class ClNode_Data;
class ClNode_Kernel;
class ClNotifier_Channel_Data;
class ClPartialLoadData;
class ClRegister_Delete;
class ClRegister_Insert;
class ClRegister_Pipe;
class ClCsTraceSize;

struct ClVectorRecordBaseMemoryInfo;


// Valores que puede tomar m_bNeedSerial
// Solo tienen sentido cuando trabajamos con serializacion avanzada
#define M4_PXB_NO_SEND           0  // El bloque no esta marcado, no hay que enviarlo
#define M4_PXB_SEND_SOME_RECORDS 1  // Hay que enviar los registros que estan marcados
#define M4_PXB_SEND_ALL_RECORDS  2  // Hay que enviar todos los registros


///////////////////////////////////////////////////////////////////////////////
//
// StBlockId
//

struct StBlockId
{
	StBlockId () :
        BlockUniqueKey (0),
        BlockId (0),
        RegisterId (0),
        Parent_BlockId (0),
        Parent_RegisterIndex (0)
	{
    }

	StBlockId (m4uint32_t bid, m4uint32_t rid, m4uint32_t pbid, m4uint32_t prid) :
        BlockUniqueKey (0),
        BlockId (bid),
        RegisterId (rid),
        Parent_BlockId (pbid),
        Parent_RegisterIndex (prid)
	{
    }

	m4return_t GetSize(ClSize &ao_size);

    m4uint32_t BlockUniqueKey;          //lo necesito para poder ir a mis hijos
    m4uint32_t BlockId;                 //lo necesito para localizarme a mi mismo->por ejemplo cuando me elimino del store o cuando creo un bloque hijo que le tengo que pasar el que será parent_blockid
    m4uint32_t RegisterId;              //lo necesito sin falta para poder ir justo al registro al que estoy asociado en mi padre, por ejemplo para pedir un valor
    m4uint32_t Parent_BlockId;          //lo necesito para localizar mi bloque padre
    m4uint32_t Parent_RegisterIndex;    //lo necesito para localizar mi bloque padre
};


///////////////////////////////////////////////////////////////////////////////
//
// ClBlockSearchInfo
//

class ClBlockSearchInfo
{
public:
    m4uint32_t GetSearchIndexId (void)
    { 
        return m_CurrentSearchIndexId;
    }
    
    void SetSearchIndexId (m4uint32_t ai_siid)   
    { 
        m_CurrentSearchIndexId = ai_siid;
    }
    
    void SetNotSorted (void)
    { 
        m_CurrentSearchIndexId = (m4uint32_t)-1; 
    }

    ClBlockSearchInfo &operator= (const ClBlockSearchInfo &ai_bsi)
    {
        m_CurrentSearchIndexId = ai_bsi.m_CurrentSearchIndexId;
        return *this;
    }

    ClBlockSearchInfo () :
        m_CurrentSearchIndexId ((m4uint32_t)-1)
    { 
    }

	m4return_t GetSize(ClSize &ao_size);

protected:
    //ClBSearchIndex *m_pCurrentSearchIndex;
    m4uint32_t m_CurrentSearchIndexId;
};


///////////////////////////////////////////////////////////////////////////////
//
// ClBlock_Channel_Proxy
//

class M4_DECL_M4DM ClBlock_Channel_Proxy : public ClBlockSearchInfo, public ClChangeListener
{
public:
    //nueva y excitante componente:
    StBlockId ID;

    ClBlock_Channel_Proxy ();// CONSTRUCTOR POR DEFECTO    

	void DestroyProxy (m4bool_t ai_bremovefromstore);

    //
    // Serializacion >>>>>>>>>>>>>>>>>>>>>
    //
    /**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
    m4return_t SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount);

    m4return_t Serialize(ClGenericIODriver&      IOD,
                         const m4uint8_t         ai_iRecSerMod,
                         const m4uint8_t         ai_iIteSerMod,
			             ClChannelProcess* const ai_pChannelProcess,
			             ClCsTraceSize*    const ai_pCsTraceSize,
			             const m4uint8_t         ai_iVectorDelta,
						 m4bool_t                ai_bWritePLFileMap);
    
    m4return_t DeSerialize(ClGenericIODriver& IOD, const m4uint8_t ai_iIteSerMod, ClCsTraceSize* const ai_pCsTraceSize, const m4uint8_t ai_iVectorDeltaCount);

    // Serializacion avanzada: nos permite marcar concretamente que es lo que se va a a serializar
    void ResetSerialMarks(void);

    void GetNeedSerial(m4uint8_t& ai_iNeedSerial, m4bool_t& ai_bBranchMode) const;

    void SetNeedSerial(const m4uint8_t ai_iNeedSerial, const m4bool_t ai_bBranchMode = M4_FALSE);

    void TestAndSetNeedSerial(m4uint8_t&      ao_iBeforeNeedSerial,
                              m4bool_t&      ao_bBeforeBranchMode,
                              const m4uint8_t ai_iNeedSerial,
                              const m4bool_t ai_bBranchMode = M4_FALSE);

    m4return_t MarkForPersistSerial(void);

    void MarkBlockAndParentRecordsForSerialize(const m4bool_t  ai_bBranchMode = M4_FALSE, const m4uint8_t ai_iNeedSerial = M4_PXB_SEND_ALL_RECORDS);

    void MarkRecordAndParentRecordsForSerialize(const regindex_t ai_iRecordToMark, const m4bool_t ai_bBranchMode = M4_FALSE);

    // Copy/Paste de una rama de una canal utilizando serializacion
    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClGenericIODriver& IOD);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
                                                  const m4bool_t     ai_bKeepMarks,
                                                  const m4bool_t     ai_bDestroyRecords,
                                                  const m4char_t*    ai_vpExclusionNodeName[],
                                                  const m4uint16_t   ai_iNumExclusionNodes,
                                                  ClGenericIODriver& IOD);

    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClFileName& ao_oFileName);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
                                                  const m4bool_t    ai_bKeepMarks,
                                                  const m4bool_t    ai_bDestroyRecords,
                                                  const m4char_t*   ai_vpExclusionNodeName[],
                                                  const m4uint16_t  ai_iNumExclusionNodes,
                                                  const ClFileName& ai_oFileName,
                                                  const m4bool_t    ai_bDeleteFile);
    //
    // Serializacion <<<<<<<<<<<<<<<<<<<<<
    //

    void FindRecordAndSetVersion(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion, m4bool_t& ao_bContinue);

    virtual m4bool_t IsProxy (void) const;
    virtual m4bool_t IsNihil (void) const { return M4_FALSE; }


    virtual m4bool_t IsValid (void) const;    

    void _SetIsLoaded (m4bool_t);
    virtual m4bool_t IsLoaded (void) const;

    virtual m4return_t B_Undo(void);
    virtual m4bool_t R_Undo(ClMMIndex ai_regindex);

    virtual m4return_t CheckPoint (void);
    virtual m4return_t CheckPoint (ClMMIndex ai_regindex); //checkpoints de registro

    virtual m4return_t SetCheckPointMode(m4bool_t ai_operation);
    virtual m4return_t GetCheckPointMode (m4bool_t &ai_operation);
    virtual m4return_t CommitPersisted (VersionMark &ai_version);
    virtual m4return_t RollbackPersisted (void);

    virtual m4bool_t R_GetNewNotPersist(ClMMIndex ai_regindex) const;
    virtual m4return_t R_SetNewNotPersist(ClMMIndex ai_regindex, const m4bool_t ai_bNewNotPersist);

// jcr
    virtual	m4return_t CheckModify( m4bool_t &ao_bModified ) ;

    m4bool_t ExistParentBlock(void) const;
    virtual void GetParentBlock (ClBlockReference &ao_br);

    //void NotifyDestroyAllRegisters_Before ();

    virtual size_t     Count (void);

    virtual ClPartialLoadData * GetpPL_Data(void);
    //Ordenacion de indices para la carga parcial
	virtual m4return_t R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg);

    virtual m4return_t Arrange (void);    

    //virtual ClBlockSearchInfo *GetpSearchInfo (void);


    m4return_t ResetProxy (void);
    //void ResetProxyUnconditional (void);

    
    virtual m4return_t DeleteAll (void);

    

    virtual void UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n);
    //virtual void UpdateParentInfoIndexOfMyRBs (void);

    virtual m4return_t DestroyBlock (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyBlockNoUpdateAccesses(void);
    virtual m4return_t DestroyAllRegisters (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyAllRegistersNoUpdateAccesses (void);

//*******************************************************************
// NUEVAS FUNCIONES DE OPERACION
//*******************************************************************

    virtual m4return_t R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus);
    virtual m4return_t R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus);

    virtual m4bool_t R_IsModified		(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNew			(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdated		(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeleted		(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNewAndDeleted	(const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModifiedByDDBB	(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNewByDDBB		(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdatedByDDBB	(const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeletedByDDBB	(const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModified    (void) const;
    virtual m4bool_t R_IsNew         (void) const;
    virtual m4bool_t R_IsUpdated     (void) const;
    virtual m4bool_t R_IsDeleted     (void) const;

    virtual m4bool_t R_IsModifiedByDDBB (void) const;
    virtual m4bool_t R_IsNewByDDBB      (void) const;
    virtual m4bool_t R_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t R_IsDeletedByDDBB  (void) const;

    virtual m4bool_t B_IsModified    (void) const;
    virtual m4bool_t B_IsNew         (void) const;
    virtual m4bool_t B_IsUpdated     (void) const;
    virtual m4bool_t B_IsDeleted     (void) const;

    virtual m4bool_t B_IsModifiedByDDBB (void) const;
    virtual m4bool_t B_IsNewByDDBB      (void) const;
    virtual m4bool_t B_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t B_IsDeletedByDDBB  (void) const;

    virtual m4return_t RefreshFlags (m4uint8_t &ao_Operation);

//*******************************************************************
//*******************************************************************

    virtual m4bool_t    R_IsValid           (ClMMIndex ai_index);
    virtual m4return_t  R_SetUpdated        (ClMMIndex ai_index);

    virtual m4return_t  R_GetVersion        (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t  R_SetVersion        (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t  R_GetRecordPersist  (ClMMIndex ai_index, m4bool_t &ai_persist);
    virtual m4return_t  R_SetRecordPersist  (ClMMIndex ai_index, m4bool_t ai_persist);

    virtual m4return_t  B_GetPersist        (m4bool_t &ai_persist);
    virtual m4return_t  B_SetPersist        (m4bool_t ai_persist);
    virtual m4return_t  B_GetReLoad         (m4bool_t &ao_rl);
    virtual m4return_t  B_SetReLoad         (m4bool_t ai_rl);

    virtual m4return_t  R_AddRecordAndSetLDBItems   (ClMMIndex & ao_Index,
                                                     ClRegister_Pipe* ai_pRegPipe,
                                                     m4bool_t ai_bAddRegister = M4_TRUE,
                                                     m4bool_t ai_bAuxLoad = M4_FALSE);
    virtual m4return_t  B_AddRegister               (ClMMIndex &ao_newindex);
    virtual m4return_t  R_InsertRegister            (ClMMIndex ai_regindex);
    virtual m4return_t  R_DeleteRegister            (ClMMIndex ai_regindex);
    virtual m4return_t  R_Destroy                   (ClMMIndex ai_regindex);
    virtual m4return_t  R_GetRecordID               (ClMMIndex ai_regindex,
                                                     m4uint32_t &ao_iUniqueRowId);
    virtual m4return_t  R_SetRecordID               (const ClMMIndex ai_regindex,
                                                     const m4uint32_t ai_iUniqueRowId);
    virtual m4return_t  B_FindByRecordID            (ClMMIndex &ao_regindex,
                                                     m4uint32_t ai_iUniqueRowId);
	virtual m4return_t	Reserve						(const regindex_t ai_iSize);

    void R_DestroyRBChilds  (ClMMIndex ai_regindex);

public:
	static m4return_t GetNewProxy (ClNode_Data *pnode, m4uint32_t blockid, m4uint32_t registerid, m4uint32_t ai_Parent_BlockId, m4uint32_t ai_Parent_RegisterIndex, ClBlockReference &ao_br);
    
    virtual ClBlock_Channel* _CreateBlock (void);

    void DestroyFast (void)
    {
        delete this;    //suicidio
    }

    void DestroySafe (void)
    {
        DestroyProxy (M4_TRUE);
    }

	virtual m4uint32_t IncRef (void)
	{
		return ++m_iRefCount;
	}

	virtual m4uint32_t DecRef (void)
	{
		return --m_iRefCount;
	}

	virtual m4bool_t IsValidProxy (void) const
	{
		return (m_pClBlock || m_bLoaded)? M4_TRUE : M4_FALSE;
	}

    ClNode_Data *GetpNode_Data (void) const
    {
        return m_pClNode_Data;    
    }

	m4uint32_t	GetRangeSize( void ) const;
	m4uint32_t	GetRangeStart( void ) const;
	m4return_t	SetRangeSize( m4uint32_t ai_iRangeSize );
	m4return_t	SetRangeStart( m4uint32_t ai_iRangeStart );
	m4return_t	SetRangeStartNext( void );

	m4uint32_t	GetTimeOut( void ) const;
	m4return_t	SetTimeOut( m4uint32_t ai_iTimeOut );

	m4return_t	GetLastLoadStatus( m4uint8_t &ao_riLastLoadStatus, m4uint32_t &ao_riLastLoadRows, m4uint32_t &ao_riLastLoadFetchs ) const ;
	m4return_t	SetLastLoadStatus( m4uint8_t ai_iLastLoadStatus, m4uint32_t ai_iLastLoadRows, m4uint32_t ai_iLastLoadFetchs ) ;

	m4uint64_t	GetBlockTimeStamp( void ) const ;
	m4uint64_t	GetRecordTimeStamp( void ) const ;

	m4return_t GetSize(ClSize &ao_size);

    ClBlock_Channel * m_pClBlock;

	// Hay que hacerlo público para la carga parcial con serialización
    m4return_t _SerializeRecord(ClGenericIODriver&      IOD,
                                const m4uint8_t         ai_iRecSerMod,
                                const m4uint8_t         ai_iIteSerMod,
                                ClChannelProcess* const ai_pChannelProcess,
								ClCsTraceSize*    const ai_pCsTraceSize,
                                const regindex_t        ai_RecordIndex,
                                const m4bool_t          ai_bThereIsAnyChange,
                                const m4uint16_t        ai_iNumChildNodesRB,
								const m4uint8_t         ai_iVectorDelta,
								m4bool_t                ai_bWritePLFileMap);

    m4return_t _DeSerializeRecord(ClGenericIODriver&             IOD,
                                  const m4uint8_t                ai_iIteSerMod,
                                  const regindex_t               ai_RecordIndex,
                                  const m4bool_t                 ai_bThereIsAnyChange,
                                  const m4uint16_t               ai_iNumChildNodesRB,
                                  const m4bool_t                 ai_bDeSerializingDeltas,
                                  ClNotifier_Channel_Data* const ai_pNotifier,
								  ClCsTraceSize*           const ai_pCsTraceSize,
								  const m4uint8_t                ai_iVectorDeltaCount);

	void SetConnectedChange( void )
	{
		m_oConnectedChangeListener.SetChange() ;
		m_bConnectedChanged = M4_TRUE ;
	}

protected:
        ClNode_Data * m_pClNode_Data;
        
		m4uint32_t	m_iRefCount;

        m4bool_t    m_bCheckPointMode           : 1;  // : 1 --> bitfield
        m4bool_t    m_bLoaded                   : 1;
        m4bool_t    m_bBlockNotSentBecauseMarks : 1;
        m4bool_t    m_bBranchMode               : 1;  // Serializacion avanzada

        m4uint8_t	m_iNeedSerial;  // Serializacion avanzada

		ClChangeListener	m_oConnectedChangeListener ;
		m4bool_t			m_bConnectedChanged;

		//protegidos !        
        ClBlock_Channel_Proxy (ClNode_Data *ai_pnodech, m4uint32_t bid, m4uint32_t rid, m4uint32_t pbid, m4uint32_t prid) :
			m_pClNode_Data (ai_pnodech),
			m_bCheckPointMode (M4_TRUE),
			m_pClBlock (0),
			m_bLoaded (M4_FALSE),

			ID (bid, rid, pbid, prid),

			m_iNeedSerial (M4_PXB_NO_SEND),
			m_bBranchMode (M4_FALSE),
			m_bBlockNotSentBecauseMarks (M4_FALSE),

            m_iRefCount (0)
		{
		}

		virtual ~ClBlock_Channel_Proxy();

        //cañero:
        void *operator new (size_t, void *ai_pbuffer);
        void operator delete (void *);

private:
    void _UpdateConnectedItems (void/*ClItem &ai_pfatheritem*/);
    
    void _R_InitChildren (ClMMIndex ao_Index);
	//void _B_InitChildren (void);

    void _UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n);    

    //
    // Serializacion >>>>>>>>>>>>>>>>>>>>>
    //
    m4return_t _SerializeNT(ClGenericIODriver&      IOD,
                            const m4uint8_t         ai_iRecSerMod,
                            const m4uint8_t         ai_iIteSerMod,
							ClChannelProcess* const ai_pChannelProcess,
							ClCsTraceSize*    const ai_pCsTraceSize,
							const m4uint8_t         ai_iVectorDelta,
							m4bool_t				ai_bWritePLFileMap);

    m4return_t _DeSerializeNT (ClGenericIODriver&  IOD,
                              const m4uint8_t      ai_iIteSerMod,
							  ClCsTraceSize* const ai_pCsTraceSize,
							  const m4uint8_t      ai_iVectorDeltaCount);
};

#endif  // __PX_BLOCK__HPP__ 
