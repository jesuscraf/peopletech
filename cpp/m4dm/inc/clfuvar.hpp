//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clfuvar.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             5/05/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class FilterUtilityVariableArguments
//
//==============================================================================

#ifndef __CLFUVAR_HPP__
#define __CLFUVAR_HPP__

//#include "aclfltrt.h"
#include "m4types.hpp"
#include "mmindex.hpp"
#include "m4var.hpp"
#include "clargume.hpp"

#include "index.hpp"

class _ClRegister_Base;
class _ClRegisterFilter;
class ClAccess;
class ClAccessProvider;
class ClBlock_Channel_Proxy;
class ClBlockReference;
class ClBSearchIndex;
class ClChannel;
class ClMMArray;
class ClNode_Kernel;
class ClSmartHandle;
class ClSmartSearchIndex;


class _ClFilterUtility
{
public:

    enum UtilityType {LN4, Operation, Nihil, Sort, Fast};

    virtual ~_ClFilterUtility(void);

    virtual m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs) = 0;
    virtual m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;

    virtual UtilityType Type(void) = 0;

    virtual _ClArgumentList_Base & Arguments (void)
    {
        return m_oArgumentNihil;
    }

    virtual ClBSearchIndex * GetSearchIndex(void)
    {
        return 0;
    };

    virtual ClChannel * GetpChannel (void)
    {
        return 0;
    }

    virtual m4return_t CreateArray(const ClBlockReference &ai_br, _ClRegisterFilter * ai_pClSubFilter, ClMMArray * ai_pmmarray, ClAccessProvider *ai_pap);

	virtual m4return_t GetSize(ClSize &ao_size)=0;

    virtual void AttachRegister(_ClRegister_Base * ai_pReg)
    {
        return;
    }

    virtual void SetSorted(void)
    {
        return;
    }

    virtual void SetNotSorted(void)
    {
        return;
    }

    virtual m4bool_t IsSorted(void)
    {
        return M4_FALSE;
    }

protected:

    static ClArgumentList_Nihil m_oArgumentNihil;

};

class ClFilterUtility : public _ClFilterUtility
{
public:

    ClFilterUtility(ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex);
    ClFilterUtility(ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4int32_t ai_iHandleForm);
    ClFilterUtility(_ClFilterUtility * ai_pClFU);
   ~ClFilterUtility(void);

    m4return_t Init (ClChannel *ai_pchannel);
    m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs);
    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    _ClArgumentList_Base & Arguments (void)
    {
        return m_ClArguments;
    }

    ClChannel * GetpChannel (void);

    _ClFilterUtility::UtilityType Type(void) { return _ClFilterUtility::LN4; };

	m4return_t GetSize(ClSize &ao_size);


protected:

    ClArgumentList m_ClArguments;

    ClNodeIndex m_iNodeIndex;
    ClSmartHandle * m_pClHandleForm;
};

class ClFiltUtilOp: public _ClFilterUtility
{
public:

    UtilityType Type(void) { return _ClFilterUtility::Operation; };

    ClFiltUtilOp(m4uint8_t ai_OpType);
    ClFiltUtilOp(_ClFilterUtility * ai_pClFU);

    m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs) { return M4_ERROR; };
    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

	m4return_t GetSize(ClSize &ao_size);

protected:

    m4uint8_t m_OpType;
};

class ClFiltSort: public _ClFilterUtility
{
public:

    UtilityType Type(void) { return _ClFilterUtility::Sort; };

    ClFiltSort(_ClRegister_Base * ai_pClRegister, ClBSearchIndex * ai_pBSearchIndex);
    ClFiltSort(_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders);
    ClFiltSort(_ClFilterUtility * ai_pClFU);
   ~ClFiltSort(void);
    m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs)
    {
        return M4_ERROR;
    }

    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
    {
        return M4_TRUE;
    }

    m4return_t CreateArray(const ClBlockReference &ai_br, _ClRegisterFilter * ai_pClSubFilter, ClMMArray * ai_pmmarray, ClAccessProvider *ai_pap);

    ClBSearchIndex * GetSearchIndex(void);

	m4return_t GetSize(ClSize &ao_size);

    void AttachRegister(_ClRegister_Base * ai_pReg)
    {
        m_pClRegister = ai_pReg;
    }

    void SetSorted(void)
    {
        m_bSorted = M4_TRUE;
    }

    void SetNotSorted(void)
    {
        m_bSorted = M4_FALSE;
    }

    m4bool_t IsSorted(void)
    {
        return m_bSorted;
    }

protected:

    _ClRegister_Base * m_pClRegister;
    ClSmartSearchIndex * m_pBSearchIndex;
    m4bool_t m_bSorted;
};

class ClFU_Nihil : public _ClFilterUtility
{
public:

    UtilityType Type(void) { return _ClFilterUtility::Nihil; };

    m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs) { return M4_ERROR; };
    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return M4_FALSE; }
	m4return_t GetSize(ClSize &ao_size);

};

#endif //__CLFUVAR_HPP__
