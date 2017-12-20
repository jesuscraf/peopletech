//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             regi_smp.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter declaration
//
//
//==============================================================================

#ifndef __REGI_SMP_HPP__
#define __REGI_SMP_HPP__
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "mmindex.hpp"

#include "clfuvar.hpp"
#include "filter_i.hpp"

class ClBlock_Channel_Proxy;
class m4VariantType;
class ClAccessProvider;

class ClBlockReference;

class M4_DECL_M4DM _ClRegisterFilter
{
public:
    _ClRegisterFilter ();
    virtual ~_ClRegisterFilter ();

    enum Type_e {Simple, AutoLoad, PartialLoad, StaticFilter, DynamicFilter, BlockWalker};

    virtual m4return_t InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs) = 0;

    virtual m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;

    virtual m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;

    virtual _ClRegisterFilter * Destroy(void) = 0;

    virtual _ClRegisterFilter * SubFilter(void) = 0;
    virtual m4bool_t IsTerminator(void) = 0;
    virtual m4return_t AttachSubFilter(_ClRegisterFilter * ai_pSubFilter) = 0;

    virtual void Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual void Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual void Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual void Notify_ReleaseAll_Before (ClAccessProvider *ai_pap) = 0;

    virtual m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;

    virtual _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal = M4_FALSE) = 0;

	virtual m4return_t UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;
    virtual m4return_t UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) = 0;

    virtual _ClFilterUtility * operator -> (void) const = 0;

    virtual m4return_t AttachFather(_ClRegisterFilter * ai_pFather)
    {
        //no hacemos nada
        return M4_SUCCESS;
    }

    virtual m4return_t AttachInterface(ClFilterInterface * ai_pClFilter) = 0;

    virtual m4return_t Delete(void) = 0;    

    virtual m4return_t GetName(m4VariantType & ao_pVar) = 0;

    virtual m4return_t UpdateThisFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
    {
        return M4_SUCCESS;
    }

    virtual m4return_t PrepareIndexPL(m4bool_t ai_bUp, const ClBlockReference &ai_br)
    {
        return M4_SUCCESS;
    }
	virtual m4return_t GetSize(ClSize &ao_size) = 0;


    virtual Type_e Type (void) const = 0;

    //Le paso el register a los filtros por si alguno lo necesita
    virtual void AttachRegister(_ClRegister_Base * ai_pReg)
    {
        return;
    }
};

class M4_DECL_M4DM _ClRegisterFilter_Terminator : public _ClRegisterFilter
{
public:
    //por compatibilidad con los demas filtros 
    _ClRegisterFilter *SubFilter (void) { return 0; }
    m4bool_t IsTerminator (void) { return M4_TRUE; }
    m4return_t AttachSubFilter(_ClRegisterFilter * ai_pSubFilter) { return M4_ERROR; }

    void Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_ReleaseAll_Before (ClAccessProvider *ai_pap) { return; }

    _ClRegisterFilter * Destroy(void) { return this; }
    _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal = M4_FALSE)
    {
        return this;
    }
	m4return_t UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return M4_SUCCESS; }
    m4return_t UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap){ return M4_SUCCESS; }

    m4return_t InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs){ return M4_SUCCESS; }

    _ClFilterUtility * operator -> (void) const
    {
        return & m_ClFU_Nihil;
    }

    m4return_t AttachInterface(ClFilterInterface * ai_pClFilter) { return M4_ERROR; };

    m4return_t Delete (void) { return M4_ERROR; };

    m4return_t GetName(m4VariantType & ao_pVar) { return M4_ERROR; };

	m4return_t GetSize(ClSize &ao_size)
	{
		return M4_SUCCESS;
	}
protected:

    static ClFU_Nihil m_ClFU_Nihil;
};

class M4_DECL_M4DM ClRegisterFilter_Simple : public _ClRegisterFilter_Terminator
{
public:
    ClRegisterFilter_Simple ();
   ~ClRegisterFilter_Simple ();

    m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    Type_e Type (void) const { return _ClRegisterFilter::Simple; }

private:

};

class M4_DECL_M4DM _ClRegisterFilter_Generic : public _ClRegisterFilter
{
public:
    _ClRegisterFilter_Generic (m4pchar_t ai_pName)
    {
        m_pClFilterUtility = 0;
        m_pSubFilter = 0;
        m_pFather = 0;
        if (ai_pName)
        {
            m4uint32_t iLen = strlen (ai_pName) + 1;
            m_pName = new m4char_t[iLen];
            strcpy (m_pName, ai_pName);
        }
        else
            m_pName = 0;
    }

    _ClRegisterFilter_Generic (void)
    {
        m_pClFilterUtility = 0;
        m_pSubFilter = 0;
        m_pFather = 0;
        m_pName = 0;
    }
    ~_ClRegisterFilter_Generic ()
    {
        if (m_pClFilterUtility)
            delete m_pClFilterUtility;
        if (m_pName)
            delete [] m_pName;
    }


    _ClRegisterFilter * SubFilter(void)
    {
	    return m_pSubFilter;
    }

    //Tiene que ser publica por si queremos cambiar el filtro que attachamos
    m4return_t AttachSubFilter(_ClRegisterFilter * ai_pSubFilter)
    {
	    m_pSubFilter = ai_pSubFilter;

	    return M4_SUCCESS;
    }

    m4return_t AttachInterface(ClFilterInterface * ai_pClFilter)
    {
	    m_pClFilter = ai_pClFilter;

	    return M4_SUCCESS;
    }

    _ClRegisterFilter * Destroy(void)
    {
        _ClRegisterFilter * AuxFilter = m_pSubFilter->Destroy();
        delete this;
        return AuxFilter;
    }

    m4return_t Delete(void)
    {
        if (m_pFather)
        {
            m_pFather->AttachSubFilter(m_pSubFilter);
            m_pSubFilter->AttachFather(m_pFather);
            delete this;
            return M4_SUCCESS;
        }
        else
            m_pSubFilter->AttachFather(0); //ahora lo que era el padre es nulo
            return m_pClFilter->DeleteLast();

    }

    m4bool_t IsTerminator (void) { return M4_FALSE; }

    _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal = M4_FALSE)
    {
        return m_pSubFilter->Copy(ai_pClFI, ai_bTotal);
    }

    m4return_t GetName(m4VariantType & ao_pVar)
    {
        if (m_pName)
        {
            m4VariantType vaux(m_pName, M4_TRUE);
            ao_pVar = vaux;
            if ( (m4pchar_t)ao_pVar )
                return M4_SUCCESS;
        }
        return M4_ERROR;
    }

    m4return_t AttachFather(_ClRegisterFilter * ai_pFather)
    {
        m_pFather = ai_pFather;
        return M4_SUCCESS;
    }
	m4return_t GetSize(ClSize &ao_size);

    //Le paso el register a los filtros por si alguno lo necesita
    void AttachRegister(_ClRegister_Base * ai_pReg)
    {
        m_pSubFilter->AttachRegister(ai_pReg);
    }

protected:

    m4pchar_t m_pName;

    _ClFilterUtility *m_pClFilterUtility;
    _ClRegisterFilter * m_pSubFilter;
    _ClRegisterFilter * m_pFather;
    ClFilterInterface * m_pClFilter;
};

#endif // __REGI_SMP_HPP__
