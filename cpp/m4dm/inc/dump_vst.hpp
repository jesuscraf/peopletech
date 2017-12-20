//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             dump_vst.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             10/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Memory Dump 'Visitor'
//
//
//==============================================================================


#ifndef __DUMP_VST_HPP__
#define __DUMP_VST_HPP__
#include "m4dm_dll.hpp"

#include "m4types.hpp"

#include "m4stl.hpp"

class ClChannelManager;
class ClChannel;
class ClChannel_Data;
class ClAccess;
class ClNode;
//class ClNode_Channel;
class ClNode_Data;
class ClNode_Kernel;
class ClRegister;
class ClBlockDirectory;
class ClNodeDirectory_Data;
class ClRecordSet;
class ClBlock_Channel_Proxy;
class _ClRegister_Base;
class ClNoCallableItem;
class _ClValue_Read_Slices;
class _ClOperation_Base;
class ClNodeDef;
class ClItemDef;

class ClBlockReference;

class M4_DECL_M4DM _AClDump_Visitor
{
public:
    //_AClDump_Visitor ();
    _AClDump_Visitor (ostream &ai_osf);
    //_AClDump_Visitor (m4pcchar_t ai_path);
    virtual ~_AClDump_Visitor ();

    //void Open (m4pcchar_t ai_path);
    //void Close (void);

protected:
    ostream *m_pofs;
    //m4bool_t m_bIsStreamOwner;
};

class M4_DECL_M4DM ClSimpleDump_Visitor : public _AClDump_Visitor
{
public:
    //ClSimpleDump_Visitor ()                   : _AClDump_Visitor () {}
    ClSimpleDump_Visitor (ostream &ai_osf)    : _AClDump_Visitor (ai_osf) {}
    //ClSimpleDump_Visitor (m4pcchar_t ai_path) : _AClDump_Visitor (ai_path) {}

    m4return_t Visit_Channel        (ClChannel &ai_pcm);
    m4return_t Visit_ChannelData (ClChannel_Data &ai_pcm);
    m4return_t Visit_Access         (ClAccess &ai_access);
    m4return_t Visit_NodeDirChannel (ClNodeDirectory_Data &ai_nodedirchannel);
    //m4return_t Visit_NodeChannel   (ClNode_Channel &ai_nc);
    m4return_t Visit_NodeData    (ClNode_Data &ai_nc);
    m4return_t Visit_NodeAccess     (ClNode &ai_node);
    m4return_t Visit_BlockDir       (ClNode_Data &ai_nc);
    m4return_t Visit_RecordSet      (ClRecordSet &ai_recordset);
    m4return_t Visit_Block          (const ClBlockReference &ai_block);
    m4return_t Visit_Register       (_ClRegister_Base &ai_register);
    m4return_t Visit_ItemColInfo    (ClNoCallableItem &ai_item, m4uint16_t ai_scope);
    m4return_t Visit_ItemValues     (ClNoCallableItem &ai_item, m4uint16_t ai_scope);
    m4return_t Visit_Value          (_ClValue_Read_Slices &ai_value, ClItemDef &ai_itemdef);
    m4return_t Visit_Operation      (_ClOperation_Base &ai_operation);

    m4return_t Visit_Methods (ClNode_Kernel *ai_pnc);

    void _dump_type (m4uint16_t ai_type);
    void _dump_scope (m4uint16_t ai_scope);
};

class ClTreeDump_Visitor : public _AClDump_Visitor
{
public:
    //ClTreeDump_Visitor ()                   : _AClDump_Visitor () {}
    ClTreeDump_Visitor (ostream &ai_osf)    : _AClDump_Visitor (ai_osf) {}
    //ClTreeDump_Visitor (m4pcchar_t ai_path) : _AClDump_Visitor (ai_path) {}

    m4return_t Visit_Channel        (ClChannel_Data &ai_pcm);
    m4return_t Visit_Block          (const ClBlockReference &ai_br, m4uint32_t ai_k = 0, m4uint32_t ai_level = 0);
};

#endif // __MEM_VST_HPP__ 
