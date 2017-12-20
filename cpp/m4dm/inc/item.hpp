//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           item.hpp
// File:             item.hpp
// Project:          CVM-DM
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

#include "itemsimp.hpp"
#include "m4dm_dll.hpp"


#ifndef __ITEM__HPP__
#define __ITEM__HPP__


class ClAccess;
class IExecutor;

///////////////////////////////////////////////////////////////////////////////
//
// ClItem
//

class M4_DECL_M4DM ClItem : public ClNoCallableItem
{
public:
    ClItem (ClAccess *ai_paccess, m4uint8_t ai_iscope = M4DM_SCOPE_ALL) : 
        ClNoCallableItem (ai_iscope),
        m_pClAccess (ai_paccess)
    {
        Value._Attach27 (this, ai_paccess);
        OldValue._Attach27 (this, ai_paccess);
    }

    ClItem (const ClItem &ai_item);

    void _NotifyChange (void);

    void Attach (_ClRegister_Base *ai_pregister = 0) { ClNoCallableItem::Attach (ai_pregister); }
    void Attach (ClNode_Data *ai_pnode_data) { ClNoCallableItem::Attach (ai_pnode_data); }
    void Attach (ClMMIndex ai_index) { ClNoCallableItem::Attach (ai_index); }
    void Attach (ClNode_Data *ai_pnode_data, _ClRegister_Base *ai_pregister) { ClNoCallableItem::Attach (ai_pnode_data, ai_pregister); }
    void Attach (ClNode_Data *ai_pnode_data, ClMMIndex ai_index) { ClNoCallableItem::Attach (ai_pnode_data, ai_index); }

    void Attach (ClAccess *ai_paccess)
    {
        //permitimos que sea cero:
        m_pClAccess = ai_paccess;
    }
    
    void Detach (void)
    {
        m_pClAccess = 0;
        ClNoCallableItem::Detach ();
    }
    
    
    ClItem &     operator= (const ClItem &ai_item);

    ClItem &     operator[] (const ClItemIndex ai_itemindex)    //random
    {
        MoveTo (ai_itemindex);
        return *this;
    }

    ClItem &     operator[] (const ClHandle ai_itemhandle)    //random    
    {
        MoveTo (ai_itemhandle);
        return *this;
    }

    ClItem &     operator[] (m4pcchar_t ai_itemid)    //random    
    {
        MoveTo (ai_itemid);
        return *this;
    }

    ClItem &     operator++ ();           //pre - next
    ClItem &     operator-- ();           //pre - prev
    ClItem &     Begin (void);
    ClItem &     End (void);    
    
    ClNode *     GetpNodeAccess (void); //Se usa, Edu, no la quites por dios, que hace falta.

// Execution
    m4return_t	Call (IExecutor *ai_pexecutor = 0,m4int32_t ai_iArgs=-1, m4bool_t ai_bFromWrapper=M4_FALSE, m4bool_t ai_bNoInsiteRec=M4_FALSE );
    m4return_t	StackPush (const m4VariantType &ai_var);
    m4return_t	StackPop  (m4VariantType &ao_var);

    void		StackReset(void);

	ClAccess   * GetpAccess (void) const; 
	m4return_t GetSize(ClSize &ao_size);

private:
    ClAccess *m_pClAccess;    
    
    void _Copy (const ClItem &ai_item);
};


#endif  //__ITEM__HPP__
