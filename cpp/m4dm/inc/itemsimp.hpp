//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itemsimp.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             20/10/97
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
#include "m4types.hpp"

#include "mmindex.hpp"

#include "val.hpp"

#include "operation.hpp"
#include "handles.hpp"
#include "index.hpp"
#include "iteminfo.hpp"
#include "itemdef.hpp"

#include "checkmem.hpp"
#include "wproxy.hpp"

#ifndef __ITEMSIMP__HPP__
#define __ITEMSIMP__HPP__

class _ClRegister_Base;
class ClCompiledMCR;
class ClRecordSet;
class ClRecordWithTransactions;
class ClNode_Data;
class ClNodeDef;
class ClBlock_Channel_Proxy;
class ClNode_Channel;
class ClItemProxy_Array;

class ClBlockReference;

//#define M4DM_ITEM_ALL_ITEMS        17   // quien ha definido esto ??
//#define M4DM_ITEM_REGISTER_ONLY    42

/*
struct StItem
{
    StItem ();
    
    _ClRegister_Base  *pClRegister;
    //ClNode_Kernel    *pClNode_Kernel;
};
*/

///////////////////////////////////////////////////////////////////////////////
//
// ClNoCallableItem
//

class M4_DECL_M4DM ClNoCallableItem : public _withitemproxy
{
public:
    ClNoCallableItem (m4uint8_t ai_iscope = M4DM_SCOPE_ALL) :
        m_pClNode_Data (0),
        m_pClRTItemIterator (0),    //importante!
        m_iScope (ai_iscope),
        Value (&ItemDef),
        OldValue (&ItemDef),
        Index (M4DM_EOF_INDEX)
    {
        #ifdef _DEBUG
            g_StMemInfo.iItem++;
        #endif
    }

    ClNoCallableItem (const ClNoCallableItem &ai_item) :
        m_pClRTItemIterator (0),    //importante!
        Value (&ItemDef),
        OldValue (&ItemDef)
    {
        #ifdef _DEBUG
            g_StMemInfo.iItem++;
        #endif

        _Copy (ai_item);
    }

    ~ClNoCallableItem ()
    {
        #ifdef _DEBUG
            g_StMemInfo.iItem--;
        #endif

        _SetRTII (0);
    }

    ClNoCallableItem& operator= (const ClNoCallableItem &ai_item);

    //m4return_t Attach (_ClRegister_Base *ai_pregister, m4uint8_t ai_imode);
    //m4return_t Attach (ClMMIndex ai_index, m4uint8_t ai_imode);

//esto cubre todas las posibilidades:
    void Attach (_ClRegister_Base *ai_pregister = 0);
    void Attach (ClNode_Data *ai_pnode_data);
    void Attach (ClMMIndex ai_index);
    void Attach (ClNode_Data *ai_pnode_data, _ClRegister_Base *ai_pregister)
    {
        Attach (ai_pregister);
        Attach (ai_pnode_data);
    }

    void Attach (ClNode_Data *ai_pnode_data, ClMMIndex ai_index)
    {
        Attach (ai_index);
        Attach (ai_pnode_data);
    }

    virtual void Detach (void);
    
// Values
    ClValue             Value;
    ClOldValue          OldValue;

// Iterator Interface    
    ClNoCallableItem &     operator[] (const ClItemIndex ai_itemindex)    //random    
    {
        MoveTo (ai_itemindex);
        return *this;
    }

    ClNoCallableItem &     operator[] (const ClHandle ai_itemhandle)    //random    
    {
        MoveTo (ai_itemhandle);
        return *this;
    }

    ClNoCallableItem &     operator[] (m4pcchar_t ai_itemid)    //random    
    {
        MoveTo (ai_itemid);
        return *this;
    }

    ClNoCallableItem &     operator++ ();           //pre - next
    ClNoCallableItem &     operator-- ();           //pre - prev
    ClNoCallableItem &     Begin (void);
    ClNoCallableItem &     End (void);
    m4bool_t     Next (void);
    m4bool_t     Prev (void);

    m4bool_t     MoveTo (const ClItemIndex ai_itemindex);
    m4bool_t     MoveTo (const ClHandle ai_itemhandle);
    m4bool_t     MoveTo (m4pcchar_t ai_itemid);
    m4return_t   MoveToEOF (void);

    ClItemIndex  TryMove (m4pcchar_t ai_itemid, m4bool_t ai_bIsUpper = M4_FALSE);
    ClItemIndex  TryMove (ClHandle ai_itemhandle);


    m4uint32_t   Count (void);

    ClNoCallableItem Parent (void);
    m4return_t InitAsMyParent (ClNoCallableItem &ai_item);

    ClItemIndex GetIndex(void) const
    {
	    return Index;
    }

    m4return_t   SetMode (m4uint8_t ai_newscope, m4uint8_t ai_newtype);

// Metadata
    ClItemDef    ItemDef;

// gets
	//ClCompiledMCR *GetpCMCR (void);
    
    ClNodeDef *GetpNodeDef (void) const;
    ClNode_Kernel *GetpNode_ExtInfo (void) const;

    ClNode_Data *GetpNode_Data (void) const
    {
        return m_pClNode_Data;
    }

    //desde item y el ejecutor, para saber a que index está apuntando el item
    ClRegisterIndex GetRegisterIndex (void)
    {
        return Value.GetRegisterIndex ();
    }

    /*
    ClNode_Kernel *GetpNode_Kernel (void) const
    {
        return M.pClNode_Kernel;
    }
    */    

    //_ClRegister_Base  *GetpRegister (void) const;

// ColInf_Interface, para la implementación de tema 'Value':
    m4return_t GetUnitDate (m4date_t &ao_date);
    m4uint32_t GetDefaultStartDate (m4date_t &ao_date);
    m4return_t GetDefaultEndDate (m4uint32_t &ao_date);

    // para el value, de momento público:
//    void _NotifyChange (void);
    void _NotifyOperation (m4uint8_t ai_operation);

    
	//----------------
	//a extinguir
	void SetCurrentBlock (ClBlock_Channel_Proxy *ai_pblock);
	ClBlock_Channel_Proxy*  GetCurrentBlock (void) const;

	//nuevas:
	void SetCurrentBlock (const ClBlockReference &ai_br);
	const ClBlockReference &GetCurrentBlockRef (void) const;
	//----------------


    void SetpItemProxy (_AClItemProxy *ai_p);    

	m4return_t GetSize(ClSize &ao_size);

protected:
    m4uint8_t         m_iScope;
    ClRTItemIterator* m_pClRTItemIterator;
    ClNode_Data*      m_pClNode_Data;

    ClItemIndex       Index;
    
    //void _AttachReg (_ClRegister_Base *ai_pregister, const ClNoCallableItem &ai_item);
    //void _Attach (_ClValue_Read_Slices &ai_value);
    void _Copy (const ClNoCallableItem &ai_item);    

private:
    void _SetRTII (ClRTItemIterator *ai_pnewrtii);

    /** Por compatibilidad con los antiguos bookMark. Sólo se utilizan desde m4vm. */
    friend class ClOldItemBookMark;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClNoCallableItem &ai_item);

#endif  //__ITEMSIMP__HPP__
