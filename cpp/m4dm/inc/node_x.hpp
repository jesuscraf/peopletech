//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             node.hpp
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
//    Este archivo se introduce para evitar una dependencia circular de archivos
//
//==============================================================================

#include "m4types.hpp"
#include "dm_types.hpp"
#include "operation.hpp"
#include "recorset.hpp"
#include "index.hpp"
#include "handles.hpp"
#include "childnod.hpp"
#include "m4dm_dll.hpp"

#include "chanprop.hpp"

#include "autoload.hpp"

#ifndef __NODE_X__HPP__
#define __NODE_X__HPP__

class ClAccess;
class ClNode_Data;
class ClNode;
class ClCompiledMCR;
class _ClRegister_Base;
class ClRTItemIterator;
class ClRegister_Current;

class ClLaneConnection;
class ClNodeAccessDirectory;

class ClNodeDef;

class ClBlockReference;
class ClLane_Info;


///////////////////////////////////////////////////////////////////////////////
//
// ClNode
//

class M4_DECL_M4DM ClNode : public ClAutoLoad_PlugIn
{
public:

    void *operator new (size_t ai_size, void *ai_pbuffer);
    void operator delete (void *);

    ClNode (ClAccess *ai_paccess);
   ~ClNode ();

    ClItem               Item;
    ClAccessRecordSet    RecordSet;

    m4bool_t IsFree (void) const
    {
        return m_bIsFree;
    }

    ClAccess*        GetpAccess (void) const;
    ClNode_Data*     GetpNode_Data (void) const
    {
        return m_pClNode_Data;
    }

    ClNodeDef*       GetpNodeDef (void) const;
    ClNode_Kernel*   GetpExtInfo (void) const;

    // Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);    

    void SendNodeMark(const m4bool_t ai_bBranchMode);
    // Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    void Reset (void);

    /**
     * Hace un reset de verdad. Es decir, deja los BB de "this" apuntatndo a un proxy nil.
     * Este método se mete para arreglar el bug 0047176.
     * Por ahora sólo se hace la corrección del proceso del Reset del access sobre la propagación
     * a los BB, pero no se descarta que exista el mismo problema para los RB.
     * Este método sólo debe ser llamado desde el Reset del access.
     */
    void RealReset(void);

//reload:
		m4return_t ReLoad ();

	m4return_t GetSize(ClSize &ao_size);
	m4return_t GetTotalSize(ClSize &ao_size);

    
    ClLane_Info * GetpLane_Info(void)
    {
        return m_poLane_Info;
    }

//-------------------------
//autoload:
    public:
        m4return_t  SetAutoLoadMode (m4uint8_t ai_mode);
		m4return_t  SetPlugInAutoLoadMode (m4uint8_t ai_mode);
        m4uint8_t   GetAutoLoadRealMode (void);
        m4return_t  SetAutoLoadMode_fromAccess (m4uint8_t ai_mode);

    private:
        m4return_t _SetAutoLoadMode (m4uint8_t ai_mode);
//-------------------------

private:
    ClAccess *m_pClAccess;              // access al que pertenece el nodo
    ClNode_Data *m_pClNode_Data;  // node_channel asociado

    m4bool_t m_bIsFree;
    
    void Attach (ClNode_Data *ai_pnode_data);

    //ClAccess:
    void _ResetCurrentBlock (void);

    //ClNode:
    void _UpdateAccess_BB (ClChildIndex ai_childindex, const ClBlockReference &ai_br);

    //ClRegister_Current:
    void _UpdateAccess_RB (ClChildIndex ai_childindex, const ClBlockReference &ai_br);

    //ClNode:
    void _UpdateAccessNode     (ClChildNodes<ClNode> &ai_pchildarray,
        ClChildIndex ai_childindex, const ClBlockReference &ai_br);

    void _UpdateAccessNodeData (ClChildNodes<ClNode_Data> &ai_pchildarray,
        ClChildIndex ai_childindex, const ClBlockReference &ai_br);

    //ClLaneConnection:
    m4return_t _LaneConnectToNode (ClNode *ai_pnodedoc);
    m4return_t _LaneDisConnectToNode (ClNode *ai_pnodedoc);

    m4return_t LaneConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon);

    m4return_t LaneDisConnectToNode_Doc (ClLaneConnection &ai_lanecon);
    m4return_t LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon);

private:

    ClLane_Info * m_poLane_Info;


friend class ClAccess;
friend class ClNodeAccessDirectory;
friend class ClRegister_Current;
friend class ClLaneConnection;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClNode &ai_node);

#endif  //__NODE_X__HPP__ 
