//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             v_contxt.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             03/12/97
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

#include "cltypes.hpp"

#include "mmindex.hpp"
#include "index.hpp"

#include "blockref.hpp"


#ifndef __V_CONTXT__HPP__
#define __V_CONTXT__HPP__

class ClBlock_Channel_Proxy;
class _ClRegister_Base;
class ClRecordWithTransactions;
class _AClItemProxy;
class ClAccess;
class _ClGM_Interface;
class ClItemDef;
class ClChannel_Kernel;
class ClNode_Data;

class _AStGMContext
{
public:
    void *operator new (size_t, void *ai_pbuffer)
    {
        return ai_pbuffer;
    }

    void operator delete (void *)
    {
    }

#if (_MSC_VER >= 1200)
    //sólo para el compilador VC 6.0 o superior
    //...para evitar un warning bastante curioso que da
    void operator delete (void *, void *)
    {
    }
#endif

    virtual ClMMIndex GetMMIndex (void) const = 0;
    virtual ClRegisterIndex GetRegisterIndex (void) const = 0;

    virtual _ClRegister_Base *GetpRegister () const = 0;

	virtual m4return_t GetSize(ClSize &ao_size);
protected:
    //truco vil para que las dos clases ocupen lo mismo
    union reg_union_u {
        ClMMIndex         m_mmindex;
        _ClRegister_Base *m_pRegister;
    } reg_union;

};

class StGMContext_Register : public _AStGMContext
{
public:
    StGMContext_Register (_ClRegister_Base *ai_1)
    {
        reg_union.m_pRegister = ai_1;
    }

    //necesito acceder al index desde el item:
    ClRegisterIndex GetRegisterIndex (void) const;
    ClMMIndex GetMMIndex (void) const;

    _ClRegister_Base *GetpRegister () const
    {
        return reg_union.m_pRegister;
    }
	
};

class StGMContext_Index : public _AStGMContext
{
public:
    StGMContext_Index (ClMMIndex ai_index)
    {
        reg_union.m_mmindex = ai_index;
    }

    ClMMIndex GetMMIndex (void) const
    {
        return reg_union.m_mmindex;
    }

    //necesito acceder al index desde el item:
    ClRegisterIndex GetRegisterIndex (void) const
    {
        return M4DM_EOF_INDEX;
    }

    _ClRegister_Base *GetpRegister () const
    {
        return 0;
    }
};


class StKernelContext
{
public:
    ClItemDef        * pItemDef_Interface;
    ClChannel_Kernel * m_poChannel;
    ClAccess         * m_poAccess;
    _ClGM_Interface  * pMemory_Interface;
	ClBlockReference   BlockReference;
    ClNode_Data      * pNode_Data;
    _AClItemProxy    * pClItemProxy;
    ClMMIndex        * m_piMMIndex,
                       m_iMMIndex; //Solo se utiliza en caso de no haber register

    StKernelContext (ClBlock_Channel_Proxy *ai_pb, _AClItemProxy *ai_pClItemProxy, _ClGM_Interface *ai_pMemory_Interface, ClItemDef *ai_pitemdef) :
        BlockReference (ai_pb),
        pClItemProxy (ai_pClItemProxy),
        pMemory_Interface (ai_pMemory_Interface),
        pItemDef_Interface (ai_pitemdef),
        m_poChannel (0),
        m_poAccess (0),
        pNode_Data (0),
        m_xxx (0), 
        m_iMMIndex(0),
        m_piMMIndex(&m_iMMIndex)
    {
    }

    void Init0 (ClItemDef *ai_pidi)
    {
        pItemDef_Interface = ai_pidi;
        Init1 ((_ClRegister_Base *)0);
    }

    void Init1 (_ClRegister_Base *ai_pr);

    void Init1 (ClMMIndex ai_index)
    {
        M4_ASSERT (sizeof (StGMContext_Index) == sizeof (StGMContext_Register));
        _AStGMContext *p = new (&m_xxx) StGMContext_Index (ai_index);
        m_piMMIndex = &m_iMMIndex;
        m_iMMIndex = ai_index;
    }

    void Init2 (ClChannel_Kernel *ai_pdii, ClNode_Data *ai_prwt)
    {
        pNode_Data = ai_prwt;
        m_poChannel = ai_pdii;
    }

    ClRegisterIndex GetRegisterIndex (void) const
    {
		const _AStGMContext *p = &m_xxx;	//no usar m_xxx directamente
        return p->GetRegisterIndex ();
    }

    _ClRegister_Base *GetpRegister () const
    {
		const _AStGMContext *p = &m_xxx;	//no usar m_xxx directamente
        return p->GetpRegister ();
    }

    ClRecordWithTransactions *GetpRWT (void) const;

// Funciones para obtener las fechas de comienzo y fin para el caso sin tramos.
    void GetStartDate(m4double_t &ao_dDate) const;
    void GetEndDate  (m4double_t &ao_dDate) const;
    void GetUnitDate (m4double_t &ao_dDate) const;

	m4return_t GetSize(ClSize &ao_size);

private:

    StGMContext_Register m_xxx;
};

#endif //__V_CONTXT__HPP__
