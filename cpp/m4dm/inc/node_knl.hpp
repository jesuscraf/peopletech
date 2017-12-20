//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             node_knl.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             04/09/97
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

#include "m4dm_dll.hpp"
#include "handles.hpp"
#include "dm_types.hpp"
#include "operation.hpp"
#include "nodedef.hpp"
#include "childnod.hpp"
#include "itempx_c.hpp"
#include "ch_nod_s.hpp"
#include "colitdef2.hpp"
#include "aregwrite.hpp"
#include "m4clock.hpp"


#ifndef __NODE_KNL__HPP__
#define __NODE_KNL__HPP__

class ClAccess;
class ClNode;
class ClNode_Kernel;
class ClNode_Data;
class ClRTItemIterator;
class ClBlock_Channel;
class ClNodeParent;
class ClLaneConnection;
class ClBSearchIndex;
class ClChannel_Data;
class _AClComponentStore;
class ClGenericIODriver;
class ClBlockReference;
class ClRecordWithTransactions;
class ClSize;
class ClMutex;
class ClChannelProcess;
class EMDStaticData;
class ClCsTraceSize;


///////////////////////////////////////////////////////////////////////////////
//
// ClNode_Connector
//

class M4_DECL_M4DM ClNode_Connector
{
public:
    ClNode_Data *GetAddress (void) const
    {
        return m_pParentNode;
    }

    ClHandle GetHandle (void) const
    {
        return m_Handle;
    }

    m4uint8_t GetType (void) const
    {
        return m_iType;
    }

    ClNode_Connector (ClNode_Data *ai_pnode, ClHandle ai_handle, m4uint8_t ai_iType)
    {
        m_pParentNode = ai_pnode;
        m_Handle = ai_handle;
        m_iType = ai_iType;
    }

	m4return_t GetSize(ClSize &ao_size);

private:
    ClNode_Data *m_pParentNode;
    ClNodeIndex m_Index;
    ClHandle  m_Handle;
    m4uint8_t m_iType;
};

///////////////////////////////////////////////////////////////////////////////
//
// ClNodeParent
//

class M4_DECL_M4DM ClNodeParent
{
public:
   ClNodeParent ();
  ~ClNodeParent ();

   ClNode_Connector &operator [] (m4uint32_t ai_index);

   m4uint32_t Count (void);

   ClNode_Connector *GetpNotFree (void) const
   {
        return m_pNotFree;
   }

   void Add (ClNode_Data *ai_pnode, ClHandle ai_handle, m4uint8_t ai_itype);

   void Remove (ClNode_Data *ai_pParentNode);

   m4return_t GetSize(ClSize &ao_size);

private:
    typedef vector<ClNode_Connector *>::iterator parentvectorit;
    vector<ClNode_Connector *> m_vpparents;
    

    ClNode_Connector *m_pNotFree;

//friend class ClNode_Kernel;
};

///////////////////////////////////////////////////////////////////////////////
//
// ClNode_Data
//

class M4_DECL_M4DM ClNode_Data : public ClSearchIndex_Current
{
public:
    ClNode_Data ();
    virtual ~ClNode_Data ();

    _AClComponentStore* GetpBlockStore (void) const
    {
        return m_pBlockStore;
    }

    //función un poco fea:
    void SetpBlockStore (_AClComponentStore *ai_pbs)
    {
        m_pBlockStore = ai_pbs;
    }

	// Esta función es para el canal del diccionario de la LDB 
	// que es el único que escribe y lee a la vez.
	m4return_t Reserve (m4uint32_t ai_lSize);
    m4return_t ForceLock( void );

//************************************************************
// FUNCIONES DE MARCAS A NIVEL DE NODO
//************************************************************

    ClOperation_Node                Operation;

    void       _NotifyOperation     (m4uint8_t ai_operation);
    m4return_t SetCheckPointMode    (m4bool_t);    
    m4return_t CheckPoint           (void);
    m4return_t Undo                 (void);
    m4return_t RefreshFlags         (m4uint8_t &ao_operation);
    m4return_t CommitPersisted      (VersionMark *ai_version);
    m4return_t RollbackPersisted    (void);    

//************************************************************

// jcr
    m4return_t CheckModify( m4bool_t &ao_bModified ) ;

    m4return_t Init1 (ClChannel_Data *ai_pClChannel_Data, ClNodeIndex ai_nodeindex);
    m4return_t Init2 (ClNodeDirectory_Data &ai_nodedir);
    
    m4return_t LaneConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon);
    
    m4return_t LaneDisConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon);

    m4return_t RefreshRWT(void);

    //se accede desde itemconn.cpp:
    void GetpUniqueBlock (ClBlockReference &ai_br);

    ClBlock_Channel *CreateBlock (void/*ClBlock_Channel_Proxy *ai_pproxy*/);

    m4return_t ReleaseAll (m4bool_t ai_bresetnodeitems);
    m4return_t ReleaseDoc (m4bool_t ai_bresetnodeitems);

    ClRecordWithTransactions *GetpRWT (void);

    const EMDStaticData* GetpSMDataNodo (void) const
    {
        return m_pSMDataNodo;
    }

    const EMDStaticData* GetpSMDataBlock (void) const
    {
        return m_pSMDataBlock;
    }

    const EMDStaticData* GetpSMDataRecord (void) const
    {
        return m_pSMDataRecord;
    }

    // Serializacion >>>>>>>>>>>>>>>>>>>>>
    /**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
    m4return_t SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount);

    m4bool_t SerializationRequired(const ClNode_Data* const ai_pParentNode, const m4uint8_t ai_iIteSerMod) const;

    m4uint8_t CheckBranchMode(const m4uint8_t ai_iRecSerMod) const;

	/**
	 * Serializa los items de nodo.
	 *
	 * @param IOD					IODriver donde serializamos.
	 * @param ai_iRecSerMod			Modo de serialización de los registros.
	 * @param ai_iIteSerMod			Modo de serialización de los ítems.
	 * @param ai_pChannelProcess	Por si hay que hacer algún proceso antes o despues de serializar.
	 * @param ai_pCsTraceChannel	Por si hay que guardar información sobre los tamaños de serialización.
	 * @param ai_iVectorDelta		Índice del vector de deltas que se tiene que utilizar.
	 * @return m4return_t.
	 */
    m4return_t Serialize(ClGenericIODriver&      IOD,
                         const m4uint8_t         ai_iRecSerMod,
                         const m4uint8_t         ai_iIteSerMod,
						 ClChannelProcess* const ai_pChannelProcess,
						 ClCsTraceSize*    const ai_pCsTraceSize,
						 const m4uint8_t         ai_iVectorDelta);
    
    m4return_t DeSerialize(ClGenericIODriver&   IOD,
                           const m4uint8_t      ai_iIteSerMod,
						   ClCsTraceSize* const ai_pCsTraceSize,
						   const m4uint8_t      ai_iVectorDeltaCount);
    
    m4return_t InheritConnectedData(ClNode_Data* ai_poParentNode);

    // Serializacion avanzada: nos permite marcar concretamente que es lo que se va a a serializar
    void ResetSerialMarks(void);

    void GetNeedSerial(m4bool_t& ao_bNeedSerial,
                       m4bool_t& ao_bBranchMode) const;

    void SetNeedSerial(const m4bool_t ai_bNeedSerial,
                       const m4bool_t ai_bBranchMode = M4_FALSE);

    void TestAndSetNeedSerial(m4bool_t&      ao_bBeforeNeedSerial,
                              m4bool_t&      ao_bBeforeBranchMode,
                              const m4bool_t ai_bNeedSerial,
                              const m4bool_t ai_bBranchMode = M4_FALSE);

    void MarkNodeAndParentRecordsForSerialize(const m4bool_t ai_bBranchMode);
    // Serializacion <<<<<<<<<<<<<<<<<<<<<

    void FindRecordAndSetVersion(const m4uint32_t ai_iRecordId,
                                 const m4date_t   ai_dNewRecordVersion,
                                 m4bool_t&        ao_bContinue);

    ClNode_Kernel *GetpExtInfo (void ) const
    {
        return m_pClNode_Kernel;
    }

    ClNode_Kernel *GetpNodeDef (void ) const
    {
        return m_pClNode_Kernel;
    }

    ClChannel_Data *GetpChannel_Data (void ) const
    {
        return m_pClChannel_Data;
    }

    void InitBlockStore (void);
    void ResetBlockStore (void);

    m4bool_t IsLaned (void);

	m4uint64_t GetNodeTimeStamp( void ) const ;

	m4return_t GetSize(ClSize &ao_size);

private:
    ClNode_Kernel *m_pClNode_Kernel;
    ClChannel_Data *m_pClChannel_Data;

    _AClComponentStore *m_pBlockStore;  //almacen de bloques (proxys) del nodo

    //ClBlock_Channel_Proxy *m_pProxyMonoBlock;   //para el tema monobloque
    //ClBlock_Channel_Proxy *m_pProxy;    
    ClRecordWithTransactions * m_poRecordWithTransaction;

    // Creamos los StaticMetaData. Uno para los items a nivel de nodo,
    // otro para los items a nivel bloque y otro para los items a nivel de registro
    EMDStaticData* m_pSMDataNodo;
    EMDStaticData* m_pSMDataBlock;
    EMDStaticData* m_pSMDataRecord;

    // Serializacion >>>>>>>>>>>>>>>>>>>>>
    m4bool_t m_bNeedSerial;  // Serializacion avanzada
    m4bool_t m_bBranchMode;  // Serializacion avanzada
    // Serializacion <<<<<<<<<<<<<<<<<<<<<
};

ostream M4_DECL_M4DM & operator<< (ostream &ai_ofs, ClNode_Data &ai_nodedata);

class M4_DECL_M4DM ClNode_Kernel : /*public ClNode_Data,*/ public ClSearchIndex_List , public ClNodeDef
{
public:
    ClNode_Kernel ();
   ~ClNode_Kernel ();

    //ClNodeDef                   NodeDef;    
    
    ClNodeIndex                 Index;       // hay que ponerlo bonito, para que esté protegido

    ClNodeParent                Parent;
    ClChildNodes<ClNode_Data>   ChildNodeBB;
    ClChildNodes<ClNode_Data>   ChildNodeRB;

    m4return_t LaneConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon);    

    m4return_t LaneDisConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon);    

    m4bool_t IsFree (void);
    m4bool_t IsFree_ForgetingLanes (void);    

/*
    ClChannel_Kernel *GetpChannel_Kernel (void) const
    {
        return m_pClChannel_Kernel;
    }
*/


    m4return_t Init1 (ClCompiledMCR *ai_pCMCR, ClNodeIndex ai_index);
    m4return_t Init2 (ClNodeDirectory_Data &ai_NodeDir);
    m4return_t InitItemProxy_Array (void);

    m4return_t InitSecurity (void);

    _ClRegister_Insert * GetpRegInsert(void);
    _ClRegister_Delete * GetpRegDelete(void);

    ClColItemDef_Kernel ColItemDef_Kernel;
    ClItemProxy_Array   ItemProxy_Array;
	ClMutex*			ItemProxyMutex;

	m4return_t GetSize(ClSize &ao_size);

// is laned
	m4bool_t IsLaned()
    {
        return m_bIsLaned;
    }
	void SetIsLaned(m4bool_t ai_bIsLaned)
    {
        m_bIsLaned = ai_bIsLaned;
    }

private:
    //ClChannel_Kernel       *m_pClChannel_Kernel;

    _ClRegister_Insert * m_pRegInsert;
    _ClRegister_Delete * m_pRegDelete;

//Implementaciones para la seguridad:
    static ClRegister_Insert        ms_ClRegInsert;
    static ClRegister_InsertNihil   ms_ClRegInsertNihil;

    static ClRegister_Delete        ms_ClRegDelete;
    static ClRegister_DeleteNihil   ms_ClRegDeleteNihil;

// is laned
	m4bool_t m_bIsLaned;
};

#endif //__NODE_KNL__HPP__
