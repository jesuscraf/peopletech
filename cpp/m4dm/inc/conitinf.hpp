//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             conitinf.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             27/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Información acerca de los items conectados a un cierto item
//
//
//==============================================================================

#include "m4stl.hpp"
#include "handles.hpp"
#include "index.hpp"

//#include "itemconn.hpp"

#ifndef __CONITINF_HPP__
#define __CONITINF_HPP__

//class ClChannel;
class ClCompiledMCR;
class ClNode_Data;
class ClItemDef;
class ClNodeDirectory_Data;
class _AClConnectorImplementation;


class ClConnectItemInfo
{
public:
    ClConnectItemInfo ();
   ~ClConnectItemInfo ();

    m4return_t Init (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem);
    m4return_t InitParent (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem);

    m4uint16_t Count (void) const;
    m4return_t Add (m4uint8_t ai_ichildscope, m4uint8_t ai_ifatherscope,
        m4uint8_t ai_iconnectiontype, m4uint8_t ai_iconnectionrelation,
        _AClConnectorImplementation *&ao_ppp);

    _AClConnectorImplementation *operator [] (m4uint16_t ai_index) const;

    void Refresh (ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem);

    class ClParent 
    {
    private:
        ClNode_Data    *m_pClNode_Data;
        ClItemIndex     m_ItemIndex;

    public:
        ClParent ()
		: m_pClNode_Data(0)
		, m_ItemIndex(M4DM_EOF_INDEX)
        {
        }

        ClNode_Data *GetAddress (void) const
        {
            return m_pClNode_Data;
        }

/*
        ClNodeIndex GetIndex () const
        {
            return m_Index;
        }
*/
        ClItemIndex GetItemIndex () const
        {
            return m_ItemIndex;
        }

        void Init (ClNode_Data *ai_pn, ClItemIndex ai_ii)
        {
            m_pClNode_Data = ai_pn;
            m_ItemIndex = ai_ii;
        }
		m4return_t GetSize(ClSize &ao_size);

    } Parent;

    m4bool_t IsConnected (void) const
    {
        return (Parent.GetAddress ()) ? M4_TRUE : M4_FALSE;
    }
	m4return_t GetSize(ClSize &ao_size);

private:
    _AClConnectorImplementation *m_pool;
    m4uint16_t m_iCount;

//auxiliar:
    struct StInfo
    {
        m4uint32_t hchildnode;
        m4uint32_t hchilditem;

        m4uint8_t  ichildscope;
        m4uint8_t  ifatherscope;
        m4uint8_t  iconnectiontype;
        m4uint8_t  iconnectionrelation;

        //m4uint16_t ichildnodepos;
    };

    static void s_f (StInfo &ao_info, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem, m4uint16_t ai_i);
};

#endif // __CONITINF_HPP__
