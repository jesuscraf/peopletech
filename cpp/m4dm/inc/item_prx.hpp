//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itnd_prx.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             08/09/97
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
#include "m4dm_dll.hpp"
#include "handles.hpp"
#include "m4types.hpp"
#include "conitinf.hpp"

//memoria:
#include "slices_fixed.hpp"
#include "slices_variable.hpp"
#include "gm_intrf.hpp"

// Seguridad
#include "val_read.hpp"
#include "val_write.hpp"

//## end module.includes preserve=yes

#ifndef __ITEM_PRX_HPP__
#define __ITEM_PRX_HPP__
//## begin module.additionalDeclarations preserve=yes

class _ClRegister_Base;
class ClRecordWithTransactions;
//class ClBlock_Channel_Proxy;

class ClConnectItemInfo;
class ClColItemDef;
class _ClItemProxy_Nihil;
class _ClVirtual_Value_W;
class ClChannel_Kernel;
class ClCompiledMCR;

class StKernelContext;
//class ClNodeDirectory_Channel;

extern M4_DECL_M4DM _ClItemProxy_Nihil s_oClItemProxy_Nihil;

//## end module.additionalDeclarations

struct StItemProxyData
{

    ClHandle    Handle;
    m4uint16_t  iPos;

    ClConnectItemInfo ConnectItemInfo;

    // y más información
    _ClVirtual_Value_W *pKernel;
    _ClValue_Write     *pWrite;
    _ClValue_Read      *pRead;

	m4return_t GetSize(ClSize &ao_size);
};

///////////////////////////////////////////////////////////////////////////////
//
// _AClItemProxy
//

class _AClItemProxy
{
public:
    // utilizamos un pool, con una gestión un poco especial:
    void *operator new (size_t, void *ai_pbuffer);
    void operator delete (void *);

    _AClItemProxy () {}

    m4return_t Init (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem);
    m4return_t InitValue (ClCompiledMCR *ai_pcmcr, m4bool_t ai_bsenable);
	m4return_t InitSecurity (ClCompiledMCR *ai_pcmcr);
    void SetPos (m4uint16_t ai_ipos)
    {
        M.iPos = ai_ipos;
    }

    virtual m4return_t Validation (const StKernelContext &ai_pcontext, m4uint32_t ai_Offset) { return M4_ERROR; }

    virtual m4return_t GetAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const { return M4_ERROR; }
    virtual m4return_t GetOldAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const { return M4_ERROR; }
    virtual m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const { return M4_ERROR; }

    virtual m4return_t GetDescription (const StKernelContext &ai_pcontext, const void *&oi_Item) const { return M4_ERROR; }
    virtual m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* oi_Item) const { return M4_ERROR; }

    virtual m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const { return M4_ERROR; }
    virtual m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize) const { return M4_ERROR; }
    virtual m4return_t GetOldSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const { return M4_ERROR; }

    virtual m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull) { return M4_ERROR; }
    virtual m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus) { return M4_ERROR; }
    virtual m4return_t OldIsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull) { return M4_ERROR; }

    virtual m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices) { return M4_ERROR; }
    virtual m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ai_hasslices) { return M4_ERROR; }
    virtual m4return_t OldHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices) { return M4_ERROR; }

    virtual m4return_t GetPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority) { return M4_ERROR; }
    virtual m4return_t SetPriority (const StKernelContext &ai_pcontext, m4char_t ai_priority) { return M4_ERROR; }
    virtual m4return_t OldPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority) { return M4_ERROR; }

    virtual m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser) { return M4_ERROR; }

    virtual m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension) { return M4_ERROR; }
    virtual m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask) { return M4_ERROR; }

    m4uint16_t GetPos (void) const
    {
        return M.iPos;
    }

    ClHandle GetHandle (void) const
    {
        return M.Handle;
    }
    
    _ClVirtual_Value_W *GetpKernel (void) const
    {
        return M.pKernel;
    }

    _ClValue_Write     *GetpWrite (void) const
    {
        return M.pWrite;
    }

    _ClValue_Read      *GetpRead (void) const
    {
        return M.pRead;
    }

    ClConnectItemInfo  *GetpConnectItemInfo (void) const
    {
        return (ClConnectItemInfo *)&(M.ConnectItemInfo);
    }

    StItemProxyData *GetpData (void) const
    {
        return (StItemProxyData *)&M;
    }

    enum Type_e {Nihil, Register, Block, Node, Method};

    virtual Type_e Type (void) const = 0;

	m4return_t GetSize(ClSize &ao_size);

protected:
    StItemProxyData M;

//public para los estáticos
public:
    //el kernel para los valores:
    static _ClSlices_Fixed ms_pFLValue_Kernel;
    static _ClSlices_Variable ms_pVLValue_Kernel;

    static ClValue_Write ms_pClValueWrite;
    static ClValue_Read ms_pClValueRead;

	static ClValue_Write_Nihil ms_pClValueWriteNihil;
	static ClValue_Read_Nihil ms_pClValueReadNihil;

friend class ClColItemDef;   // en el papel de factoria
};

///////////////////////////////////////////////////////////////////////////////
//
// _ClItemProxy_Nihil
//

class _ClItemProxy_Nihil : public _AClItemProxy
{
public:
    _ClItemProxy_Nihil () : _AClItemProxy ()
    {
        M.Handle = 0;
        M.pKernel = &ms_vvnihil;
        M.pWrite = &_AClItemProxy::ms_pClValueWriteNihil;
        M.pRead = &_AClItemProxy::ms_pClValueReadNihil;
        M.iPos = (m4uint16_t)-1;
    }

    //constructor especial TRUQUI_TRUQUI:
    _ClItemProxy_Nihil (ClHandle ai_handle) : _AClItemProxy ()
    {
        M.Handle = ai_handle;
        M.pKernel = &ms_vvnihil;
        M.pWrite = &_AClItemProxy::ms_pClValueWriteNihil;
        M.pRead = &_AClItemProxy::ms_pClValueReadNihil;
        M.iPos = (m4uint16_t)-1;
    }


    Type_e Type (void) const { return _AClItemProxy::Nihil; }

	m4return_t GetSize(ClSize &ao_size);

private:
    static _ClVirtual_Value_Nihil ms_vvnihil;
};

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Register
//

class ClItemProxy_Register : public _AClItemProxy
{
public:
    ClItemProxy_Register () {}

    m4return_t Validation (const StKernelContext &ai_pcontext, m4uint32_t ai_Offset);

    m4return_t GetAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t GetOldAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const;

    m4return_t GetDescription(const StKernelContext &ai_pcontext, const void *&oi_Item) const;
    m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* oi_Item) const;

    m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;
    m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize) const;
    m4return_t GetOldSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;

    m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);
    m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus);
    m4return_t OldIsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);

    m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);
    m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ai_hasslices);
    m4return_t OldHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);

    m4return_t GetPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);
    m4return_t SetPriority (const StKernelContext &ai_pcontext, m4char_t ai_priority);
    m4return_t OldPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);

    m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser);

    m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);
    m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask);

    Type_e Type (void) const { return _AClItemProxy::Register; }

	m4return_t GetSize(ClSize &ao_size);
private:
};

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Block
//

class ClItemProxy_Block : public _AClItemProxy
{
public:
    ClItemProxy_Block () {}

    m4return_t Validation (const StKernelContext &ai_pcontext, m4uint32_t ai_Offset);

    m4return_t GetAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t GetOldAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const;

    m4return_t GetDescription(const StKernelContext &ai_pcontext, const void *&oi_Item) const;
    m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* oi_Item) const;

    m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;
    m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize) const;
    m4return_t GetOldSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;

    m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);
    m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus);
    m4return_t OldIsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);

    m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);
    m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ai_hasslices);
    m4return_t OldHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);

    m4return_t GetPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);
    m4return_t SetPriority (const StKernelContext &ai_pcontext, m4char_t ai_priority);
    m4return_t OldPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);

    m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser);

    m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);
    m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask);

    Type_e Type (void) const { return _AClItemProxy::Block; }

	m4return_t GetSize(ClSize &ao_size);
private:
};

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Node
//

class ClItemProxy_Node : public _AClItemProxy
{
public:
    ClItemProxy_Node () {}

    m4return_t Validation (const StKernelContext &ai_pcontext, m4uint32_t ai_Offset);

    m4return_t GetAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t GetOldAddress_andhasslices (const StKernelContext &ai_pcontext, void **ao_address, m4bool_t &ao_hasslices) const;
    m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const;

    m4return_t GetDescription(const StKernelContext &ai_pcontext, const void *&oi_Item) const;
    m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* oi_Item) const;

    m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize) const;
    m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;
    m4return_t GetOldSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) const;

    m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);
    m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus);
    m4return_t OldIsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_IsNull);

    m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);
    m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ai_hasslices);
    m4return_t OldHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_hasslices);

    m4return_t GetPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);
    m4return_t SetPriority (const StKernelContext &ai_pcontext, m4char_t ai_priority);
    m4return_t OldPriority (const StKernelContext &ai_pcontext, m4char_t &ao_priority);

    m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser);

    m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);
    m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask);

    Type_e Type (void) const { return _AClItemProxy::Node; }

	m4return_t GetSize(ClSize &ao_size);
private:
};

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Method
//

class ClItemProxy_Method : public _AClItemProxy
{
public:
    ClItemProxy_Method () {}

    Type_e Type (void) const { return _AClItemProxy::Method; }

	m4return_t GetSize(ClSize &ao_size);
private:
};

#endif // __ITEM_PRX_HPP__ 
