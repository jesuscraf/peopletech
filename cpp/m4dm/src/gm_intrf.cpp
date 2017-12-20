#include "gm_intrf.hpp"

#include "v_contxt.hpp"
#include "item_prx.hpp"
#include "px_block.hpp"
#include "sizer.hpp"

// comentario general:
// si estamos en eof, ai_context.pClItemProxy = nihil, que devuelve error
// en todas las funciones => ai_context.pClItemProxy siempre es not null.

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_Interface
//

m4return_t _ClGM_Interface::Validation (const StKernelContext &ai_context, m4uint32_t ai_Offset)
{
    return ai_context.pClItemProxy->Validation (ai_context, ai_Offset);
}

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_OldValue
//

m4return_t _ClGM_OldValue::GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices)
{
    m4return_t res = ai_context.pClItemProxy->GetOldAddress_andhasslices (ai_context, ao_address, ao_hasslices);
    if (res != M4_SUCCESS)
    {
        *ao_address = 0;
        ao_hasslices = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::GetAddress (const StKernelContext &ai_context, void **ao_address)
{
    M4_ASSERT(0);

    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::GetSize (const StKernelContext &ai_context, m4uint32_t &ao_size)
{
    m4return_t res = ai_context.pClItemProxy->GetOldSize (ai_context, ao_size);
    if (res != M4_SUCCESS)
    {
        ao_size = 0;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::IsNull (const StKernelContext &ai_context, m4bool_t &ao_bisnull)
{
    m4return_t res = ai_context.pClItemProxy->OldIsNull (ai_context, ao_bisnull);
    if (res != M4_SUCCESS)
    {
        ao_bisnull = M4_TRUE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::GetHasSlices (const StKernelContext &ai_context, m4bool_t &ao_bhasslices)
{
    m4return_t res = ai_context.pClItemProxy->OldHasSlices (ai_context, ao_bhasslices);
    if (res != M4_SUCCESS)
    {
        ao_bhasslices = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::GetPriority(const StKernelContext &ai_context, m4char_t &prior)
{
    m4return_t res = ai_context.pClItemProxy->OldPriority (ai_context, prior);
    if (res != M4_SUCCESS)
    {
        prior = 3;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_OldValue::GetIsUser (const StKernelContext &ai_context, m4bool_t &ao_bIsUser)
{
    m4return_t res = ai_context.pClItemProxy->GetIsUser (ai_context, ao_bIsUser);
    if (res != M4_SUCCESS)
    {
        ao_bIsUser = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//
// _ClGM_Value
//

/*
m4return_t _ClGM_Value::Validation (const StKernelContext &ai_context)
{
    return ai_context.pClItemProxy->Validation (ai_context);
}
*/

m4return_t _ClGM_Value::GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices)
{
    m4return_t res;
    res = ai_context.pClItemProxy->GetAddress_andhasslices (ai_context, ao_address, ao_hasslices);
    if (res != M4_SUCCESS)
    {
        *ao_address = 0;
        ao_hasslices = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetAddress (const StKernelContext &ai_context, void **ao_address)
{
    M4_ASSERT(0);

    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetDescription (const StKernelContext &ai_context, void **ao_address)
{
    m4return_t res;
    
	// se cambia esta linea por las dos siguientes porque en Hp el segundo parámetro espera que 
	// sea un const void * y no lo es.
	//res = ai_context.pClItemProxy->GetDescription (ai_context, *ao_address);
    
	const void * pvAuxAdress;
	res = ai_context.pClItemProxy->GetDescription (ai_context, pvAuxAdress);
	
	if (res != M4_SUCCESS)
    {
        *ao_address = 0;
        return res;
    }

    *ao_address = (void *) pvAuxAdress;
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetSize (const StKernelContext &ai_context, m4uint32_t &ao_size)
{
    m4return_t res = ai_context.pClItemProxy->GetSize (ai_context, ao_size);
    if (res != M4_SUCCESS)
    {
        ao_size = 0;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::IsNull (const StKernelContext &ai_context, m4bool_t &ao_bisnull)
{
    m4return_t res = ai_context.pClItemProxy->IsNull (ai_context, ao_bisnull);
    if (res != M4_SUCCESS)
    {
        ao_bisnull = M4_TRUE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetHasSlices (const StKernelContext &ai_context, m4bool_t &ao_bhasslices)
{
    m4return_t res = ai_context.pClItemProxy->GetHasSlices (ai_context, ao_bhasslices);
    if (res != M4_SUCCESS)
    {
        ao_bhasslices = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetPriority(const StKernelContext &ai_context, m4char_t &prior)
{
    m4return_t res = ai_context.pClItemProxy->GetPriority (ai_context, prior);
    if (res != M4_SUCCESS)
    {
        prior = 3;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::GetIsUser (const StKernelContext &ai_context, m4bool_t &ao_bIsUser)
{
    m4return_t res = ai_context.pClItemProxy->GetIsUser (ai_context, ao_bIsUser);
    if (res != M4_SUCCESS)
    {
        ao_bIsUser = M4_FALSE;
        return res;
    }
    return M4_SUCCESS;
}

m4return_t _ClGM_Value::SetValueByOffset (const StKernelContext &ai_context, const void* ai_item, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser)
{
    return ai_context.pClItemProxy->SetValueByOffset (ai_context, ai_item, ai_size, ai_offset, ao_pRetSize, ai_bChangeStatus, ai_bIsUser);
}

m4return_t _ClGM_Value::SetDescription (const StKernelContext &ai_context, const void* ai_item)
{
    return ai_context.pClItemProxy->SetDescription (ai_context, ai_item);
}

m4return_t _ClGM_Value::SetSize (const StKernelContext &ai_context, m4uint32_t ai_newsize)
{
    return ai_context.pClItemProxy->SetSize (ai_context, ai_newsize);
}

m4return_t _ClGM_Value::SetNull (const StKernelContext &ai_context, m4bool_t ai_bChangeStatus)
{
    return ai_context.pClItemProxy->SetNull (ai_context, ai_bChangeStatus);
}

// ojo !! todavía no implementada:

m4return_t _ClGM_Value::SetHasSlices (const StKernelContext &ai_context, m4bool_t ai_bhasslices)
{
    return ai_context.pClItemProxy->SetHasSlices (ai_context, ai_bhasslices);
}

m4return_t _ClGM_Value::SetPriority(const StKernelContext &ai_context, m4char_t prior)
{
    return ai_context.pClItemProxy->SetPriority (ai_context, prior);
}

m4return_t _ClGM_Value::CreateTempFile(const StKernelContext &ai_context, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
{
    return ai_context.pClItemProxy->CreateTempFile(ai_context, ai_pDirectory, ai_pExtension);
}

m4return_t _ClGM_Value::ChangedMask(const StKernelContext &ai_context, m4uint32_t & ao_oChangedMask)
{
    return ai_context.pClItemProxy->ChangedMask(ai_context, ao_oChangedMask);
}


m4return_t 
_ClGM_Value::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClGM_Value);
	return M4_SUCCESS;
}

m4return_t 
_ClGM_OldValue::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClGM_OldValue);
	return M4_SUCCESS;
}

