//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             arecset.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    At last the ClRecordSet !!
//
//
//==============================================================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "operation.hpp"
#include "item.hpp"
#include "register.hpp"
#include "regtot.hpp"
#include "not_cli.hpp"
#include "m4stl.hpp"


//## end module.includes preserve=yes

#ifndef __ARECSET_HPP__
#define __ARECSET_HPP__

//## begin module.additionalDeclarations preserve=yes

class ClBlock_Channel_Proxy;
class ClRecordSet;
class _ClRecordSet_Base;
class ClNode_Data;
class ClNodeDef;
class ClBSearchIndex;
class ClNotifier_Channel;
class ClRegTotalize;
class ClBlockReference;

//## end module.additionalDeclarations

///////////////////////////////////////////////////////////////////////////////
//
// ClFlagPersist_RecordSet
//

class ClFlagPersist_RecordSet
{
public:
    ClFlagPersist_RecordSet ();

    m4bool_t Get (void);
    void Set (m4bool_t ai_bool);
    
    void Attach (_ClRecordSet_Base *ai_prs);

	m4return_t GetSize(ClSize &ao_size);

private:

    _ClRecordSet_Base *m_pClRecordSet;
};


///////////////////////////////////////////////////////////////////////////////
//
// _ClRecordSet_Base
//

class M4_DECL_M4DM _ClRecordSet_Base
{
public:
    _ClRecordSet_Base (ClAccess *ai_paccess);
    virtual ~_ClRecordSet_Base ()
    {
        if (m_pFooter)
            delete m_pFooter;
    }

    virtual _ClRegister_Base *GetpRegister (void) const = 0;

    virtual void Attach (ClNode_Data *ai_pClNode_Channel);
    virtual void Detach (void);
    m4bool_t IsAttached (void);

    virtual m4bool_t   IsEOF (void);

    ClItem                     Item;

    //---------------
    ClOperation_RecordSet      Operation;
    ClFlagPersist_RecordSet    FlagPersist;

    m4return_t IsModified (m4bool_t &ao_is) const;
    m4return_t IsNew (m4bool_t &ao_is) const;
    m4return_t IsUpdated (m4bool_t &ao_is) const;
    m4return_t IsDeleted (m4bool_t &ao_is) const;
    m4return_t IsModifiedByDDBB (m4bool_t &ao_is) const;
    //---------------

    m4return_t        SetCheckPointMode (m4bool_t ai_operation);
    m4return_t        GetCheckPointMode (m4bool_t &ao_operation);
    
    m4bool_t          IsValid (void);

    m4uint32_t        Count (void);

    m4return_t        CheckPoint (void);
    m4return_t        CheckPoint (m4uint32_t iMMindex); // NO refresca el canal
    m4return_t        Undo (void);

    m4return_t        CommitPersisted (VersionMark &ai_version);
    m4return_t        RollbackPersisted (void);

    m4return_t        Destroy (void);

    ClNode_Data*         GetpNode_Data (void) const
    {
        return m_pClNode_Data;
    }

    ClNode * GetpNode (void) const;

    ClNodeDef*           GetpNodeDef (void) const;
    ClNode_Kernel*       GetpNode_ExtInfo (void) const;
    ClChannel_Kernel*    GetpChannel_ExtInfo (void) const;

	//--------------------
	//a extinguir:
    ClBlock_Channel_Proxy*  GetChildBlock (ClHandle ai_handle);
    ClBlock_Channel_Proxy*  GetChildBlock (ClChildIndex ai_childindex);
    ClBlock_Channel_Proxy*  GetCurrentBlock (void) const;

	//las nuevas:
    m4return_t GetChildBlock (ClHandle ai_handle, ClBlockReference &aobr);
    m4return_t GetChildBlock (ClChildIndex ai_childindex, ClBlockReference &ao_br);
    const ClBlockReference &GetCurrentBlockRef (void) const
    {
        return Item.GetCurrentBlockRef ();
    }
	virtual m4bool_t SetCurrentBlock (const ClBlockReference &ai_br);
	m4return_t GetParentBlock (ClBlockReference &ai_br);
	//--------------------
    
    
    void FollowSimple (const _ClRecordSet_Base &);    //no virtual

    m4return_t GetFlagPersisted (m4bool_t &ao_is);
    m4return_t SetFlagPersisted (m4bool_t ai_bool);
    m4return_t GetFlagReLoad (m4bool_t &ao_rl);
    m4return_t SetFlagReLoad (m4bool_t ai_rl);


    m4return_t Load_Block ();
    m4return_t Load_Prg ();
    m4return_t Persist_Tree ();
    m4return_t Execute (m4pcchar_t ai_pMethodId, m4bool_t ai_bFromWrapper);

	virtual m4return_t GetSize(ClSize &ao_size);
protected:
    m4return_t _Execute (m4pcchar_t ai_pMethodId);
    virtual void _SetCurrentBlock (const ClBlockReference &ai_br);

    ClNode_Data *m_pClNode_Data;

//-------------------------
//access:
    public:
	    ClAccess* GetpAccess (void) const
	    {
		    return Item.GetpAccess ();
	    }
//-------------------------

//--------------------
//totalizaciones:
    public:
        virtual ClRegTotalize & GetFooter(void) = 0;

    protected:
        ClRegTotalize *m_pFooter;
//--------------------

//-------------------------
//sort:
    public:
        ClBSearchIndex *GetCurrentSearchIndex (void) const;
        m4return_t SetCurrentSearchIndex (ClBSearchIndex *psi);
        m4return_t SetCurrentSearchIndexByPos (m4uint16_t ai_ipos);
        m4return_t SetCurrentSearchIndexById  (m4uint32_t ai_id);
        m4return_t Arrange (void);

    private:
        void _UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n);
//-------------------------

    public:

        m4return_t FindRegisterByID(m4uint32_t ai_iUniqueRowId, ClRegisterIndex &ao_iRegIndex);
};


M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClRecordSet &ai_recset);


class M4_DECL_M4DM ClRecordSet : public _ClRecordSet_Base, public _AClClient_Notifier
{
public:
    ClRecordSet (ClAccess *ai_paccess, m4bool_t ai_bne = M4_FALSE) : _ClRecordSet_Base (ai_paccess), Register (ai_paccess, ai_bne) {}
   ~ClRecordSet ();

    ClRegister    Register;

    _ClRegister_Base *GetpRegister (void) const
    {
        return (_ClRegister_Base *)&Register;
    }

    void Attach (ClAccess *ai_paccess);

    void Attach (ClNode_Data *ai_pClNode_Data);
    void Detach (void);

    void FollowSimple (const ClRecordSet &ai_rs);

    ClRegTotalize &GetFooter(void);

    void _SetCurrentBlock (const ClBlockReference &ai_br);
    
	//nuevas
	virtual m4bool_t SetCurrentBlock (const ClBlockReference &ai_br);
	//-----------------

//----------------------------
//interface _AClClient_Notifier:
    void DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
    {
        Register.DestroyRegister_Before (ai_pnode, ai_br, ai_index);
    }

    void DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
    {
        Register.DestroyRegister_After (ai_pnode, ai_br, ai_index);
    }

    void AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
    {
        Register.AddRegister_After (ai_pnode, ai_br, ai_index);
    }
    
    void InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
    {
        Register.InsertRegister_After (ai_pnode, ai_br, ai_index);
    }

    void DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
    {
        Register.DestroyAllRegisters_After (ai_pnode, ai_br);
    }

    void DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
    {
        Register.DestroyBlock_After (ai_pnode, ai_br);
    }

    void DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void ReleaseAll_Before (void);
    void DestroyChannelData_Before (void);
    void DestroyChannel_Before (ClChannel * ai_poCh);

    void Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc)
    {
        Register.Arrange_After (ai_pnode, ai_br, ai_pindex_qtc);
    }

    void ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh);
    ClChannel * GetpChannel (void);

//----------------------------
    void NotifierDetach (void);
    void NotifierAttach (ClNotifier_Channel_Data *ai_pnotifier);
    ClNotifier_Channel_Data *GetpNotifier (void) const;
	m4return_t GetSize(ClSize &ao_size);

};

#endif // __ARECSET_HPP__ 
