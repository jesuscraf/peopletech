#include "m4types.hpp"

#ifndef __GM_VALUE__HPP__
#define __GM_VALUE__HPP__

class StKernelContext;
class ClSize;

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_Interface
//

class _ClGM_Interface
{
public:
    m4return_t Validation (const StKernelContext &ai_pcontext, m4uint32_t ai_Offset);

    virtual m4return_t GetAddress (const StKernelContext &ai_pcontext, void **ao_address) = 0;

    virtual m4return_t GetDescription (const StKernelContext &ai_pcontext, void **ao_address) = 0;

    virtual m4return_t GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) = 0;

    virtual m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size) = 0;
    virtual m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_bisnull) = 0;
    virtual m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_bhasslices) = 0;
    virtual m4return_t GetPriority(const StKernelContext &ai_pcontext, m4char_t &prior) = 0;
    virtual m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser) = 0;

// mismo interface de momento, hasta que los kernel se separen en lectura/escritura
    virtual m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* ai_item, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) = 0;
    virtual m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* ai_item) = 0;

    virtual m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize) = 0;
    virtual m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus) = 0;
    virtual m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ao_bhasslices) = 0;
    virtual m4return_t SetPriority(const StKernelContext &ai_pcontext, m4char_t prior) = 0;
	
    virtual m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension) = 0;
    virtual m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask) = 0;

	virtual m4return_t GetSize(ClSize &ao_size)=0;

};

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_OldValue
//

class _ClGM_OldValue : public _ClGM_Interface
{
public:
    //m4return_t Validation (const StKernelContext &ai_pcontext);
    m4return_t GetAddress (const StKernelContext &ai_pcontext, void **ao_address);

    m4return_t GetDescription (const StKernelContext &ai_pcontext, void **ao_address)
    {
        return M4_ERROR;
    }

        m4return_t GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices);

    m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size);
    m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_bisnull);
    m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_bhasslices);
    m4return_t GetPriority(const StKernelContext &ai_pcontext, m4char_t &prior);
    m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser);

// mismo interface de momento, hasta que los kernel se separen en lectura/escritura
    m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* ai_item, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser)
    {
        return M4_ERROR; 
    }
    m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* ai_item)
    {
        return M4_ERROR; 
    }
    m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize)
    {
        return M4_ERROR; 
    }
    m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus)
    {
        return M4_ERROR; 
    }
    m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ao_bhasslices)
    {
        return M4_ERROR; 
    }
    m4return_t SetPriority(const StKernelContext &ai_pcontext, m4char_t prior)
    {
        return M4_ERROR; 
    }

    m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
    {
        return M4_ERROR; 
    }

    m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask)
    {
        return M4_ERROR; 
    }


	m4return_t GetSize(ClSize &ao_size);
};

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_Value
//

class _ClGM_Value : public _ClGM_Interface
{
    //m4return_t Validation (const StKernelContext &ai_pcontext);
    m4return_t GetAddress (const StKernelContext &ai_pcontext, void **ao_address);

    m4return_t GetDescription (const StKernelContext &ai_pcontext, void **ao_address);

        m4return_t GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices);

    m4return_t GetSize (const StKernelContext &ai_pcontext, m4uint32_t &ao_size);
    m4return_t IsNull (const StKernelContext &ai_pcontext, m4bool_t &ao_bisnull);
    m4return_t GetHasSlices (const StKernelContext &ai_pcontext, m4bool_t &ao_bhasslices);
    m4return_t GetPriority(const StKernelContext &ai_pcontext, m4char_t &prior);
    m4return_t GetIsUser (const StKernelContext &ai_pcontext, m4bool_t &ao_bIsUser);

    m4return_t SetValueByOffset (const StKernelContext &ai_pcontext, const void* ai_item, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser);
    m4return_t SetDescription (const StKernelContext &ai_pcontext, const void* ai_item);

    m4return_t SetSize (const StKernelContext &ai_pcontext, m4uint32_t ai_newsize);
    m4return_t SetNull (const StKernelContext &ai_pcontext, m4bool_t ai_bChangeStatus);
    m4return_t SetHasSlices (const StKernelContext &ai_pcontext, m4bool_t ao_bhasslices);
    m4return_t SetPriority(const StKernelContext &ai_pcontext, m4char_t prior);

    m4return_t CreateTempFile(const StKernelContext &ai_pcontext, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);
    m4return_t ChangedMask(const StKernelContext &ai_pcontext, m4uint32_t & ao_oChangedMask);

    m4return_t GetSize(ClSize &ao_size);
};
 
#endif //__GM_VALUE__HPP__

