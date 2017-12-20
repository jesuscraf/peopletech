//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             iteminfo.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/07/97
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

//## begin module.includes preserve=yes
//------------------------------------------------------------------------------
#include "m4define.hpp"
#include "m4types.hpp"
//------------------------------------------------------------------------------

//#include "item_prx.hpp"
#include "handles.hpp"
#include "index.hpp"
//#include "itemdef.hpp"
#include "colitemdefit.hpp"

#include "smartp.hpp"
//## end module.includes preserve=yes

#ifndef __ITEMINFO__HPP__
#define __ITEMINFO__HPP__

#define M4DM_SCOPE_NONE         0
#define M4DM_SCOPE_NODE         1
#define M4DM_SCOPE_BLOCK        2
#define M4DM_SCOPE_REGISTER     4
#define M4DM_SCOPE_ALL          7

#define M4DM_TYPE_NONE          0
#define M4DM_TYPE_METHOD        1
#define M4DM_TYPE_PROPERTY      2
#define M4DM_TYPE_FIELD         4
#define M4DM_TYPE_CONCEPT       8
#define M4DM_TYPE_ALL          15

class _ClRegister_Base;
class ClNode;
class ClNode_Kernel;
class ClNodeDef;
class ClColItem_RTInfo;
class ClColItemDef_Kernel;
class ClItemHash;


////////////////////////////////////////////////////////////////////////////////
//
// ClRTItemIterator
//

class ClRTItemIterator : public _ClSmartPointer
{
public:
    ClRTItemIterator ();
   ~ClRTItemIterator ();

    ClRTItemIterator &operator= (const ClRTItemIterator &ai_ClRTItemIterator);


    void Attach (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pcolitemdef);
    m4return_t SetModeMandatory (m4bool_t ai_bIsCacheable, m4uint8_t ai_newscope, m4uint8_t ai_newtype, m4uint16_t ai_count = 0xffff);

    m4return_t Get (ClItemIndex ai_itemindex, _AClItemProxy * &ao_prtitem);
    m4return_t Get (ClHandle ai_itemhandle, _AClItemProxy * &ao_prtitem, ClItemIndex &ao_itemindex);
    m4return_t Get (m4pcchar_t ai_itemid, _AClItemProxy * &ao_prtitem, ClItemIndex &ao_itemindex,
        m4bool_t ai_bIsUpper = M4_FALSE);

    m4uint32_t Count (void) const
    {
        return m_iCount;
    }

    m4uint8_t GetScope (void) const 
    {
        return m_iScope;
    }

    m4uint8_t GetType (void) const 
    {
        return m_iType;
    }

    m4bool_t IsInit (void) const
    {
        return m_ppItemProxy? M4_TRUE: M4_FALSE;
    }

    static int s_compare (const void *ai_elem1, const void *ai_elem2);
    
	m4return_t GetSize(ClSize &ao_size);

private:
    _AClItemProxy **m_ppItemProxy;
    m4uint32_t m_iCount;

    m4uint8_t  m_iScope;
    m4uint8_t  m_iType;

    ClItemHash * m_pHash;

    ClItemDefIt_Random m_itemdefit; // utilizado para las busquedas por ID

    m4return_t _DestroyPool (void);
    m4return_t _AllocPool (m4uint16_t ai_n);
    
    m4uint16_t _CalcCount (m4uint8_t ai_newscope, m4uint8_t ai_newtype);

    m4bool_t _test (m4uint8_t ai_type, m4uint8_t ai_scope);
};


#endif // __ITEMINFO__HPP__ 
