//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_ge.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             28/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//	Keep the Status and Control of "Value"
//
//==============================================================================

#include "val_ge.hpp"

#ifndef _M4_USING_INLINE
#include "val_ge.inl"
#endif

#include "virt_val.hpp"

#include "medvarin.hpp"
#include "item.hpp"
#include "itemdef.hpp"
#include "chan_knl.hpp"

#include "blockref.hpp"

#include "node_knl.hpp"
#include "nodedef.hpp"
#include "bnihil.hpp"
#include "px_block.hpp"
#include "recorset.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"

#include "m4objglb.hpp" //Definición de las funciones: M4ClCppType y M4VMRound

m4uint32_t ClValue_State::s_idefault;




//*************************************************************************
// OPERADORES BASICOS
//*************************************************************************

ClValue_State &ClValue_State::operator= (const ::ClValue_State &ai_value)
{
    _Copy (ai_value);

    return *this;
}

ClValue_State &ClValue_State::operator[] (ClSliceIndex ai_index)
{
    MoveTo (ai_index);

    return *this;
}

ClValue_State &ClValue_State::operator[] (m4date_t ai_date)
{
    MoveTo (ai_date);

    return *this;
}

ClValue_State &ClValue_State::operator++ ()     //pre
{
    Next ();

    return *this;
}

ClValue_State &ClValue_State::operator-- ()     //pre
{
    Prev ();

    return *this;
}

m4bool_t ClValue_State::Begin (void)
{
	if (m_oContext.pMemory_Interface->Validation(m_oContext, 0) == M4_SUCCESS)
	{
		Index = 0;
		return M4_TRUE;
	}
	
	Index = M4DM_EOF_INDEX;
	return M4_FALSE;
}

m4bool_t ClValue_State::End (void)
{
    return MoveTo (GetLastIndex ());
}

m4bool_t ClValue_State::Prev (void)
{
    if (Index != M4DM_EOF_INDEX)
	{
        MoveTo ((ClSliceIndex)(Index - 1));
    }

    return Index != M4DM_EOF_INDEX;
}

m4bool_t ClValue_State::Next (void)
{
    if (Index != M4DM_EOF_INDEX)
	{
        MoveTo ((ClSliceIndex)(Index + 1));
    }

    return Index != M4DM_EOF_INDEX;
}

// Si no tiene tramos pasamos de todos
// Con tramos: sólo permitimos o una posición válida o EOF
// return: !IsEOF

m4bool_t ClValue_State::MoveTo (ClSliceIndex ai_index)
{
	if (Index != ai_index)
	{
		m4uint32_t n = Count ();

		if (ai_index < n || !ai_index)
		{
			Index = ai_index;
		}
		else
		{
			// EOF
			Index = M4DM_EOF_INDEX;
		}
	}
	
	return Index != M4DM_EOF_INDEX;
}

// busca dentro de los tramos
// Sólo permitimos o una posición válida o EOF
// Como antes
// return: !iseof

m4bool_t ClValue_State::MoveTo (m4date_t ai_date)
{
    m4uint32_t index;
	m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            res = GetpKernelR ()->FindByDate (m_oContext, ai_date, index, &pvalueadd);
		}
        else
		{
            index = 0;
		}

        return MoveTo((ClSliceIndex)index);
    }

    return M4_FALSE;
}

// busca dentro de los tramos de forma exacta, por fecha de inicio y de fin
// Sólo permitimos o una posición válida o EOF
// Como antes, si no hay tramos, no hacemos nada.

m4bool_t ClValue_State::MoveTo (m4date_t ai_startdate, m4date_t ai_enddate)
{
    return MoveTo (ai_startdate);
}

void ClValue_State::MoveToEOF (void)
{
    Index = M4DM_EOF_INDEX;
    m_oContext.pClItemProxy = &s_oClItemProxy_Nihil;
}

//*************************************************************************
// NUEVAS FUNCIONES QUE SE SALTAN LA SEGURIDAD
//*************************************************************************

void * ClValue_State::GetWithOutSec (m4return_t * ao_pdone)
{
    return m_oContext.pClItemProxy->ms_pClValueRead.Get((_ClValue_Read_Slices *) this);
}

m4return_t ClValue_State::GetWithOutSec (m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone)
{
    return m_oContext.pClItemProxy->ms_pClValueRead.Get((_ClValue_Read_Slices *) this, ao_buffer, ai_size, ai_offset, ao_retSize);
}

m4return_t ClValue_State::GetWithOutSec (m4VariantType &ao_var, m4return_t * ao_pdone)
{
    return m_oContext.pClItemProxy->ms_pClValueRead.Get((_ClValue_Read_Slices *) this, ao_var);
}

m4return_t ClValue_State::SetWithOutSec (const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus)
{
    return m_oContext.pClItemProxy->ms_pClValueWrite.Set((_ClValue_ReadWrite_Slices *) this, ai_var, ai_prior, done, ai_bChangeStatus);
}

m4return_t ClValue_State::SetWithOutSec (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus)
{
    return m_oContext.pClItemProxy->ms_pClValueWrite.Set((_ClValue_ReadWrite_Slices *) this, ai_var, ai_bChangeStatus);
}

m4return_t ClValue_State::SetNULLWithOutSec (void)
{
    return m_oContext.pClItemProxy->ms_pClValueWrite.SetNULL((_ClValue_ReadWrite_Slices *) this, M4_TRUE);
}

m4return_t ClValue_State::SetDefaultWithOutSec (void)
{
    return m_oContext.pClItemProxy->ms_pClValueWrite.SetDefault((_ClValue_ReadWrite_Slices *) this, M4_TRUE);
}

m4return_t ClValue_State::SetPrioritySafe (priority_t ai_prior)
{
    return m_oContext.pClItemProxy->ms_pClValueWrite.SetPriority((_ClValue_ReadWrite_Slices *) this, ai_prior);
}

m4return_t ClValue_State::SetNotUpdated (const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t *ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser)
{
    m4return_t            res;
	m4bool_t              hasslices;
    void                * pvalueadd;
	m4uint32_t            size;
	m4uint8_t             cpp;
	m4char_t            * Buffer;

	m_oContext.pItemDef_Interface->CppType (cpp);

	if ( cpp == M4CL_CPP_TYPE_STRING)
	{
        res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

        if (res == M4_SUCCESS)
        {
            res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

            if (hasslices)
            {
		        res = GetpKernelRW ()->SetValue(m_oContext, Index, ai_paddress, ai_size, ai_offset, *ao_retSize, &pvalueadd);

		        if (res == M4_ERROR)
				{
			        return res;
				}

                if (res == M4_WARNING)
                {
                    GetpKernelRW ()->GetAddress (m_oContext, Index, (void **) &Buffer, &pvalueadd);
		            GetpKernelRW ()->Size (m_oContext, Index, size, &pvalueadd);

			        if (Buffer)
					{
				        Buffer[size - 1] = '\0';
					}
		        }
            }
            else
            {
                if ((m_oContext.pItemDef_Interface->IsVariableLength ())&&(ai_offset == 0))
				{
                    m_oContext.pMemory_Interface->SetNull(m_oContext, ai_bChangeStatus);
				}

                res = m_oContext.pMemory_Interface->SetValueByOffset (m_oContext, ai_paddress, ai_size, ai_offset, ao_retSize, ai_bChangeStatus, ai_bIsUser);

		        if (res == M4_ERROR)
				{
			        return res;
				}

                if (res == M4_WARNING)
                {
                    m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, (void **) &Buffer, hasslices);
		            m_oContext.pMemory_Interface->GetSize(m_oContext, size);

			        if (Buffer)
					{
				        Buffer[size - 1] = '\0';
					}
		        }
            }
        }
	}
	else
    {
        res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

        if (res == M4_SUCCESS)
        {
            res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

            if (hasslices)
			{
                res = GetpKernelRW ()->SetValue (m_oContext, Index, ai_paddress, ai_size, ai_offset, *ao_retSize, &pvalueadd);
			}
            else
			{
                res = m_oContext.pMemory_Interface->SetValueByOffset (m_oContext, ai_paddress, ai_size, ai_offset, ao_retSize, ai_bChangeStatus, ai_bIsUser);
			}
        }
    }

    return res;
}

//*************************************************************************
// FUNCIONES DE OPERACION INDEPENDIENTES DE LA SEGURIDAD
//*************************************************************************

priority_t ClValue_State::GetPriority (void)
{
	priority_t prior = 3;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
        	GetpKernelR ()->GetPriority (m_oContext, Index, prior, &pvalueadd);
		}
        else
		{
            m_oContext.pMemory_Interface->GetPriority (m_oContext, (m4char_t &)prior);
		}
    }

	return prior;
}

// Si no tiene tramos, devuelve la startdate del Access

m4date_t ClValue_State::GetStartDate (void)
{
    m4date_t sd = 0.0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->GetStartDate (m_oContext, Index, sd, &pvalueadd);
		}
        else
		{
            m_oContext.GetStartDate(sd);
		}
    }

    return sd;
}

// Si no tiene tramos, devuelve la enddate del Access
// Si estamos en el último tramo y no tiene asignada enddate, se devuelve también la del Access

m4date_t ClValue_State::GetEndDate (void)
{
    m4date_t ed = 0.0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->GetEndDate (m_oContext, Index, ed, &pvalueadd);
		}
        else
		{
            m_oContext.GetEndDate(ed);
		}
    }

    return ed;
}

m4bool_t ClValue_State::HasSlices (void)
{
    m4bool_t hasslices = M4_FALSE;
    m4return_t res;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);
    if (res == M4_SUCCESS)
	{
        m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);
	}

    return hasslices;
}

m4date_t ClValue_State::GetFirstStartDate (void)
{
    m4date_t fsd = 0.0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->GetFirstStartDate (m_oContext, fsd, &pvalueadd);
		}
        else
		{
            m_oContext.GetStartDate(fsd);
		}
    }

    return fsd;
}

m4date_t ClValue_State::GetLastEndDate (void)
{
    m4date_t led = 0.0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->GetLastEndDate (m_oContext, led, &pvalueadd);
		}
        else
		{
            m_oContext.GetEndDate(led);
		}
    }

    return led;
}

m4uint32_t ClValue_State::Count (void)
{
    m4uint32_t count = 0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->Count (m_oContext, count, &pvalueadd);
		}
    }

    return count;
}

m4uint32_t ClValue_State::Size (void)
{
    m4uint32_t size = 0;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->Size (m_oContext, Index, size, &pvalueadd);
		}
        else
		{
            m_oContext.pMemory_Interface->GetSize(m_oContext, size);
		}
    }

    return size;
}

m4return_t ClValue_State::IsNull (m4bool_t &ao_isnull)
{
	ao_isnull = M4_TRUE;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->IsNull (m_oContext, Index, ao_isnull, &pvalueadd);
		}
        else
		{
            m_oContext.pMemory_Interface->IsNull(m_oContext, ao_isnull);
		}
    }

    return ao_isnull;
}

ClSliceIndex ClValue_State::GetFirstIndex (void)
{
    m4return_t res;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
	{
        return 0;
	}
    else
	{
        return -1;
	}

}

ClSliceIndex ClValue_State::GetLastIndex (void)
{
    m4uint32_t li = -1;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
		{
            GetpKernelR ()->GetLastIndex (m_oContext, li, &pvalueadd);
		}
		else
		{
			li = 0;
		}
    }

    return li;
}

m4return_t ClValue_State::FillDateStack (ClDateStack *ai_stack)
{
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
        {
            m4uint32_t  n, i;
            m4date_t    dSliceDate,
                        dUnitDate;

            GetpKernelR ()->Count(m_oContext, n, &pvalueadd);
	        m_oContext.GetUnitDate (dUnitDate);

	        for (i = 0; i < n; i ++)
            {
                GetpKernelR ()->GetStartDate(m_oContext, i, dSliceDate, &pvalueadd);
		        ai_stack->AddElement (dSliceDate);
	        }

	        GetpKernelR ()->GetLastEndDate(m_oContext, dSliceDate, &pvalueadd);
	        ai_stack->AddElement (dSliceDate + dUnitDate);

            return M4_SUCCESS;
        }
        else
		{
            return M4_SUCCESS;
		}
    }

    return M4_ERROR;
}

m4bool_t ClValue_State::CheckSlice (m4date_t ai_dstart, m4date_t ai_dend)
{
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;
    m4double_t dstart, dend;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
        {
            m4uint32_t  index;

            GetpKernelR ()->FindByDate (m_oContext, ai_dstart, index, &pvalueadd);
            if (index != M4DM_EOF_INDEX)
            {
                GetpKernelR ()->GetStartDate(m_oContext, index, dstart, &pvalueadd);
                GetpKernelR ()->GetEndDate(m_oContext, index, dend, &pvalueadd);

                if ((ai_dstart == dstart)&&(ai_dend == dend))
				{
                    return M4_TRUE;
				}
            }

            return M4_FALSE;
        }
        else
        {
            return M4_FALSE;
        }
    }

    return M4_FALSE;
}

//*************************************************************************
// FUNCIONES PARA EL MANTENIMIENTO DEL ESTADO y VOLCADO DE ERRORES
//*************************************************************************

void ClValue_State::_Attach1 (_ClRegister_Base *ai_pr)
{
    m_oContext.Init1 (ai_pr);
}

void ClValue_State::_Attach1 (ClMMIndex ai_index)
{
    m_oContext.Init1 (ai_index);
}

void ClValue_State::_Attach2 (ClChannel_Kernel *ai_pdii, ClNode_Data *ai_prwt)
{
    m_oContext.Init2 (ai_pdii, ai_prwt);
}

void ClValue_State::_AttachX (const _ClValue_Read_Slices &ai_value, ClItem *ai_pii)
{
    m_pItem_Interface = ai_pii;
    Index = ai_value.Index;

    m_oContext.BlockReference = ai_value.m_oContext.BlockReference;
        //importante, para que se incremente el contador de referencias del proxy

    //copia bestial
    ClItemDef *ppp = m_oContext.pItemDef_Interface;
    memcpy (& m_oContext, &ai_value.m_oContext, sizeof (m_oContext));
    m_oContext.pItemDef_Interface = ppp; //ojo, si no se hace esto, problemas !

    // Ojo con esto, si el puntero apunta al miembro, no puedo apuntar al miembro de
    // el objeto origen.

    if (ai_value.m_oContext.m_piMMIndex == &ai_value.m_oContext.m_iMMIndex)
	{
        m_oContext.m_piMMIndex = &m_oContext.m_iMMIndex;
	}
}

void ClValue_State::_Copy (const ClValue_State &ai_value)
{
    // copia integra
    m_pItem_Interface = ai_value.m_pItem_Interface;
    Index = ai_value.Index;

    //copia bestial
    memcpy (& m_oContext, &ai_value.m_oContext, sizeof (m_oContext));

    // Ojo con esto, si el puntero apunta al miembro, no puedo apuntar al miembro de
    // el objeto origen.

    if (ai_value.m_oContext.m_piMMIndex == &ai_value.m_oContext.m_iMMIndex)
	{
        m_oContext.m_piMMIndex = &m_oContext.m_iMMIndex;
	}
}

ClBlock_Channel_Proxy *ClValue_State::GetCurrentBlock (void) const
{	
    return m_oContext.BlockReference.GetpProxy ();
}

void ClValue_State::_SetCurrentBlock (const ClBlockReference &ai_br)
{
    m_oContext.BlockReference = ai_br;	//operator = overloaded
}

StKernelContext * ClValue_State::_GetContext(void)
{
    return & m_oContext;
}

ClItem * ClValue_State::_GetpItem_Interface(void)
{
    return m_pItem_Interface;
}

//*************************************************************************
// INTERFACE PARA EL TOTALIZADOR DE TRAMOS
//*************************************************************************

ClValue_State * ClValue_State::GetValueAddr(void)
{
	return this;
}

m4uint8_t ClValue_State::GetTotType ( void ) const
{
	return m_oContext.pItemDef_Interface->SliceTotalize ();
}

//*************************************************************************
// FUNCIONES AUXILIARES DE UTILIDAD PARA OTRAS PUBLICAS
//*************************************************************************

m4return_t ClValue_State::_Get_OneSlice (m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, ClSliceIndex ai_i1)
{
    m4date_t pDates[4];

    m4VariantType vV;
    m4uint32_t auxindex;
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (hasslices)
        {
            GetpKernelR ()->GetStartDate (m_oContext, ai_i1, pDates[0], &pvalueadd);
            GetpKernelR ()->GetEndDate (m_oContext, ai_i1, pDates[1], &pvalueadd);
        }
        else
        {
            m_oContext.GetStartDate(pDates[0]);
            m_oContext.GetEndDate(pDates[1]);
        }

        auxindex = Index;
        Index = ai_i1;
        GetWithOutSec(vV);
        Index = auxindex;

        pDates[2] = ai_date1;
        pDates[3] = ai_date2;

        res = _GetWithDates(pDates, vV, ao_var, M4_FALSE);
    }

    return res;
}

//Si ai_size == 0 se considera que se ha de devolver todo el tamaño posible del valor.
//Si ai_offset == 0 y ai_size == 0 se devuelve el valor por referencia, en otro caso se copia a un nuevo buffer.
m4return_t ClValue_State::_GetSubString ( m4pchar_t &ao_pstring, void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize)
{
	m4return_t res = M4_SUCCESS;
    void * pvalueadd;
	m4uint32_t iRealSize;
    m4bool_t hasslices;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);
        if (hasslices)
		{
            res = GetpKernelRW ()->Size (m_oContext, Index, iRealSize, &pvalueadd);
		}
        else
		{
            res = m_oContext.pMemory_Interface->GetSize(m_oContext, iRealSize);
		}

	    if ((ai_size  + ai_offset) > iRealSize) 
	    {
			//El size es demasiado grande, devolvemos warnig y copiamos lo que podamos.
		    res = M4_WARNING;
		    ai_size = iRealSize - ai_offset;

		    if ( ai_offset > iRealSize )
			{
			    return M4_ERROR;
		    }
	    }
	    memcpy ( ao_pstring, ((m4pchar_t) ai_paddress + ai_offset ), ai_size );

	    if ( ao_retSize != NULL )
		{
			*ao_retSize = ai_size;
		}
    }

	return res;
}


/*
Devuelve si hay que redondear a la precisión.
Si hay que hacerlo además devuelve la precisión correcta.
Tiene en cuenta si el item es de tipo moneda o numérico. Sino no se redondea.
Según eso consulta la propiedad del canal "ROUND_CURRENCY" o "ROUND_NUMBER"
Por defecto el valor es false. Si puede leer la propiedad, es numérica
y contiene un 1 el valor es true. En cualquier otro caso es false, es decir,
si no se puede leer la propiedad, si no es numérica (por ejemplo null)
o si tiene un valor distinto de 1.0
Después, si hay que redondear se busca la precisión.
Según sea de tipo fecha o numérico se busca la precisión en la propiedad
de canal "CURRENCY_PRECISION" o "NUMBER_PRECISION"
Por defecto el valor es -1. Si puede leer la propiedad y es numérica
se usa su valor como precisión.
En cualquier otro caso se devuelve la escala del item.
Si el valor no está entre 0 y 9 se utiliza la escala del item.
Al final se acota la precisión entre 0 y 9.
*/
m4bool_t   ClValue_State::_GetRoundToPrecision( m4int32_t &ao_riPrecision )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4bool_t		bCurrency = M4_TRUE ;
	m4uint8_t		iType = M4CL_M4_TYPE_NULL ;
	m4uint8_t		iInternalType = M4CL_INTERNAL_TYPE_NULL ;
	m4int32_t		iScale = -1 ;
	m4uint16_t		iAuxItem = m4uint16_t( -1 ) ;
	m4VariantType	vValue ;


	ao_riPrecision = -1 ;


	// Primero se calcula si es número o moneda.
	iType = m_oContext.pItemDef_Interface->M4Type() ;

	if( iType == M4CL_M4_TYPE_NUM_VARIANT )
	{
		// Si es un variant numérico se mira su tipo de verdad usando su item auxiliar
		iAuxItem = m_oContext.pItemDef_Interface->AuxiliarItemIndex() ;

		if( iAuxItem != m4uint16_t( -1 ) )
		{
			iInternalType = m_oContext.pNode_Data->GetpNodeDef()->GetNodeItemInternalType( iAuxItem ) ;

			// Si el tipo interno del item auxiliar es de variant su contenido indica el tipo real
			if( iInternalType == M4CL_INTERNAL_TYPE_VARIANT_TYPE )
			{
				// Hay que usar otro item para no desposicionar el del contexto
				ClItem	oAuxItem( m_oContext.GetpRegister()->Item ) ;
				iResult = oAuxItem[ ClItemIndex( iAuxItem ) ].Value.Get( vValue ) ;

				if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
				{
					iType = m4uint8_t( vValue.Data.DoubleData ) ;
				}
			}
		}
	}


	if( iType == M4CL_M4_TYPE_CURRENCY )
	{
		bCurrency = M4_TRUE ;
	}
	else if( iType == M4CL_M4_TYPE_NUMBER )
	{
		bCurrency = M4_FALSE ;
	}
	else
	{
		// Si no es moneda o número se sale sin redondear
		return( M4_FALSE ) ;
	}


	// Se mira si hay que hacer redondeo o no
	if( bCurrency == M4_TRUE )
	{
		iResult = m_oContext.m_poAccess->GetpChannel()->RoundCurrency.Get( vValue ) ;
	}
	else
	{
		iResult = m_oContext.m_poAccess->GetpChannel()->RoundNumber.Get( vValue ) ;
	}

	if( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != 1.0 )
	{
		// Si no se puede leer la propiedad, no es numérica o no vale 1 no hay que redondear
		return( M4_FALSE ) ;
	}


	// Se obtiene la precisión
	if( bCurrency == M4_TRUE )
	{
		iResult = m_oContext.m_poAccess->GetpChannel()->CurrencyPrecision.Get( vValue ) ;
	}
	else
	{
		iResult = m_oContext.m_poAccess->GetpChannel()->NumberPrecision.Get( vValue ) ;
	}

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
	{
		ao_riPrecision = m4int32_t( vValue.Data.DoubleData ) ;
	}


	/*
	Si se ha indicado un valor negativo	en la propiedad se va al item
	También si la que se indica es mayor que la del item
	*/
	iScale = m_oContext.pItemDef_Interface->Scale() ;

	if( ao_riPrecision < 0 || ao_riPrecision > iScale )
	{
		ao_riPrecision = iScale ;
	}


	// No se admiten precisiones mayores de 9 decimales o negativas
	if( ao_riPrecision < 0 )
	{
		ao_riPrecision = 0 ;
	}
	else if( ao_riPrecision > 9 )
	{
		ao_riPrecision = 9 ;
	}


	return( M4_TRUE ) ;
}



m4return_t ClValue_State::_GetWithDates(m4date_t *ai_pDates, const m4VariantType &ai_var, m4VariantType &ao_var, m4bool_t ai_bOwner)
{
// Las fechas las doy en el siguiente orden:
// [0] = fecha inicio tramo
// [1] = fecha fin tramo
// [2] = fecha inicio para hacer el get
// [3] = fecha fin para hacer el get

    m4uint8_t slcbhvr;
	m4date_t unitdate;

    ao_var.SetNull();
	ao_var = ai_var;

	slcbhvr = m_oContext.pItemDef_Interface->SliceBhrv ();

	if( ai_var.Type == M4CL_CPP_TYPE_NUMBER && ( slcbhvr == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE || slcbhvr == M4CL_ITEM_SLICE_BHRV_UNIT ) )
	{
		m4date_t	dToCheck = 0;
		m4date_t	dCorrection = 0;

		m4uint16_t	iAuxItem = m4uint16_t( -1 ) ;

		// Se busca el item auxiliar si es unidad, incidencia o valor final
		// Primero se busca el de prorateo
		iAuxItem = m_oContext.pItemDef_Interface->AuxiliarItemProratIndex() ;

		if( iAuxItem == m4uint16_t( -1 ) )
		{
			// Si no tiene de prorateo se busca el normal para unidades o incidencias
			if( slcbhvr == M4CL_ITEM_SLICE_BHRV_UNIT ||	( slcbhvr == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE && m_oContext.pItemDef_Interface->GenerateSlices() == 0 ) )
			{
				iAuxItem = m_oContext.pItemDef_Interface->AuxiliarItemIndex() ;
			}
		}

		if( iAuxItem != m4uint16_t( -1 ) )
		{
			m4VariantType	vTemp;
			ClItem			oItemAuxiliarWalker(0);
			ClNode			*poNodeAuxiliarWalker;
			_ClRegister_Base *poRegister;
			
			//nos creamos un item que vaya al auxiliar->Ojo el index que tenemos es por registro
			poRegister = m_oContext.GetpRegister();

			if( poRegister )
			{ //si no hay register->no hay access, y no podemos leer los items auxiliares

				poNodeAuxiliarWalker = poRegister->GetpAccess()->Node.Get(m_oContext.pNode_Data->GetpNodeDef()->GetHandle() );
				oItemAuxiliarWalker  = poNodeAuxiliarWalker->RecordSet.Current.Item;
				
				if( oItemAuxiliarWalker[ClItemIndex(iAuxItem)].Value[ai_pDates[0]].Get(vTemp) == M4_SUCCESS && vTemp.Type == M4CL_CPP_TYPE_DATE )
				{
					dToCheck = vTemp.Data.DoubleData;
					
					//comprobamso si el tramo del que vamo a leer incluye dToCheck
					if( dToCheck >= ai_pDates[0] && dToCheck <= ai_pDates[1] )
					{
						//leemos el item auxiliar de este
						iAuxItem = oItemAuxiliarWalker.ItemDef.AuxiliarItemIndex();

						if( iAuxItem != (m4uint16_t)-1 && oItemAuxiliarWalker[ClItemIndex(iAuxItem)].Value[ai_pDates[0]].Get(vTemp) == M4_SUCCESS && vTemp.Type == M4CL_CPP_TYPE_DATE )
						{
							//si está en el tramos pedida sumamos a la fecha fin de tramo (dEndSliceDate)
							//la diferencia entre las fechas a tratar y tratada
							dCorrection = vTemp.Data.DoubleData - dToCheck;
						}
					}
				}
			}
		}

		// Promedio se halla la proporción
		m_oContext.GetUnitDate (unitdate);

		/* Bug 0077639
		Si el dividendo es 0 no se puede prorratear y se devuelve el valor original
		*/
		m4date_t dDivider = unitdate + ai_pDates[1] - ai_pDates[0] + dCorrection;

		if( dDivider != 0 )
		{
			m4int32_t iPrecision = 0 ;

			if( _GetRoundToPrecision( iPrecision ) == M4_TRUE )
			{
				/* Bug 0196707
				Se aplica la corrección a 30 en el caso de redondeo
				*/
				m4date_t dInterval1 = unitdate + ai_pDates[3] - ai_pDates[0];
				m4date_t dInterval2 = ai_pDates[2] - ai_pDates[0];

				if( dToCheck >= ai_pDates[0] && dToCheck <= ai_pDates[3] )
				{
					dInterval1 += dCorrection;
				}

				if( dToCheck >= ai_pDates[0] && dToCheck < ai_pDates[2] )
				{
					dInterval2 += dCorrection;
				}

				m4double_t dPorcion1 = ( ai_var.Data.DoubleData * dInterval1 ) / dDivider;
				m4double_t dPorcion2 = ( ai_var.Data.DoubleData * dInterval2 ) / dDivider;
				ao_var.Data.DoubleData = M4VMRound(dPorcion1, iPrecision) - M4VMRound(dPorcion2, iPrecision);
			}
			else
			{
				m4date_t dInterval = unitdate + ai_pDates[3] - ai_pDates[2];

				if( dToCheck >= ai_pDates[2] && dToCheck <= ai_pDates[3] )
				{
					dInterval += dCorrection;
				}

				ao_var.Data.DoubleData = ( ai_var.Data.DoubleData * dInterval ) / dDivider;
			}
		}
    }

    if( ai_bOwner && ai_var.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
        ao_var.SetString(ai_var.Data.PointerChar, M4_TRUE);
	}

    return M4_SUCCESS;
}


/* Bug 0077473
Obtiene el valor por defecto, pero prorrateado entre las fechas de trabajo.
El prorrateo se hace si el item es numérico y si el item valor final, unidad o incidencia. En cualquier otro caso se deja el valor por defecto normal
Se tienen en cuenta las correcciones debidas a meses de 30 días, es decir,
que el prorrateo puede no ser lineal
Si las fechas pedidas están dentro de las fechas de trabajo el valor prorrateado será
menor que el original
Si las fechas pedidas están fuera (o parte fuera) de las fechas de trabajo, el valor
puede llegar a ser mayor que el original
Si las fechas de trabajo tienen amplitud negativa, el valor resultante se
cambiará de signo, es decir, se tiene en cuenta el valor absoluto del perido.
Lo mismo ocurre si la amplitud de las fechas pedidas es negativo.
*/
m4return_t ClValue_State::_GetDefaultWithDates( m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4VariantType &ao_rvDefault )
{

	m4return_t		iResult ;
	m4uint8_t		iSliceBehaviour ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;
	m4date_t		dSwapDate ;
	m4date_t		adDates[ 4 ] ;
    m4VariantType	vDefaulValue ;


	m_oContext.pItemDef_Interface->GetItemDefaultValue( vDefaulValue ) ;

	// Si es numérico
    if( vDefaulValue.Type == M4CL_CPP_TYPE_NUMBER )
    {
		iSliceBehaviour = m_oContext.pItemDef_Interface->SliceBhrv() ;
	
		// Si es valor final (o incidencia) o unidad
		if( iSliceBehaviour == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE || iSliceBehaviour == M4CL_ITEM_SLICE_BHRV_UNIT )
		{
			m_oContext.GetStartDate( dStartDate ) ;
			m_oContext.GetEndDate( dEndDate ) ;

			// Si las fechas coinciden no se hace nada
			if( ai_dStartDate != dStartDate || ai_dEndDate != dEndDate )
			{
				// No se permiten numeradores negativos
				if( ai_dStartDate > ai_dEndDate )
				{
					dSwapDate = ai_dStartDate ;
					ai_dStartDate = ai_dEndDate ;
					ai_dEndDate = dSwapDate ;
				}

				// No se permiten denominadores negativos ni 0
				if( dStartDate > dEndDate )
				{
					dSwapDate = dStartDate ;
					dStartDate = dEndDate ;
					dEndDate = dSwapDate ;
				}

				adDates[ 0 ] = dStartDate ;
				adDates[ 1 ] = dEndDate ;
				adDates[ 2 ] = ai_dStartDate ;
				adDates[ 3 ] = ai_dEndDate ;

				// Se prorratea
				iResult = _GetWithDates( adDates, vDefaulValue, ao_rvDefault ) ;

				return( iResult ) ;
			}
		}
	}


	// En cualquier otro caso se asigna sin más
	ao_rvDefault = vDefaulValue ;

	return( M4_SUCCESS ) ;
}


m4return_t ClValue_State::_GetToVariant(void * ai_pAddress, m4VariantType &ao_var)
{
    m4uint8_t itype;

    if (ai_pAddress)
    {
        itype = m_oContext.pItemDef_Interface->CppType ();
        switch (itype) {
            case M4CL_CPP_TYPE_STRING:
                ao_var.Type = M4CL_CPP_TYPE_STRING_VAR;
				ao_var.Data.PointerChar = (m4pchar_t)ai_pAddress;
                break;

            case M4CL_CPP_TYPE_NUMBER:
            case M4CL_CPP_TYPE_DATE:
                ao_var.Type = itype;
                ao_var.Data.DoubleData = *((m4double_t *)ai_pAddress);
                break;

            default:
                // tipo no válido
                return M4_ERROR;
        }
    }
    else
	{
        // se trata de un 'null'
        ao_var.Type = M4CL_CPP_TYPE_NULL;
        ao_var.Data.DoubleData = 0;
    }

    return M4_SUCCESS;
}

m4return_t ClValue_State::_GetToVoid (const m4VariantType &ai_var, m4uint32_t &ao_size, void *& ao_paddress)
{
    ao_size = 0;
    ao_paddress = 0;

    switch (ai_var.Type)
    {
        case M4CL_CPP_TYPE_NUMBER:
        case M4CL_CPP_TYPE_DATE:

            ao_size = sizeof(m4VariantData);
            ao_paddress = (void *) &ai_var.Data.DoubleData;
            break;

		/* Bug 0089297
		Hay que considerar el tipo M4CL_CPP_TYPE_STRING_SYS para la genconn
		*/
        case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
            if (ai_var.Data.PointerChar)
            {
                ao_size = strlen ((char *)ai_var.Data.PointerChar) + 1;
                ao_paddress = ai_var.Data.PointerChar;
            }
            break;

        case M4CL_CPP_TYPE_NULL:
            break;

        default:
            return M4_ERROR; // tipo no válido
    }

    return M4_SUCCESS;
}

m4return_t ClValue_State::_Convert (m4VariantType &ai_var)
{
    ClVariantConversor s_oVC;

//<<pipo
    M4_ASSERT (m_oContext.pItemDef_Interface);

    m4return_t res = M4_SUCCESS;
    m4uint8_t itype;
    // conversión cutrosa de tipos

    res = m_oContext.pItemDef_Interface->CppType (itype);    //mi tipo

	if (res != M4_SUCCESS)
	{
		return res;
	}

    switch (itype) {    // mi tipo
    case M4CL_CPP_TYPE_NUMBER:
        res=s_oVC.ToDouble (ai_var);
        break;

    case M4CL_CPP_TYPE_STRING:
        res=s_oVC.ToCharString (ai_var);
        break;

    case M4CL_CPP_TYPE_DATE:
        // soy una fecha 
        res=s_oVC.ToDate(ai_var);
        break;

    case M4CL_CPP_TYPE_NULL:
        // no hago nada
        res = M4_SUCCESS;
        break;

    default:
        // ### error, mi tipo es incorrecto  !!!
        res = M4_ERROR;
    }        

    return res;
}

m4return_t ClValue_State::_Notify (void)
{
    M4_ASSERT (_GetpItem_Interface());

    _GetpItem_Interface()->_NotifyChange ();

	/* Bug 0124398
	Solo hay que marcar el nodo y canal como modificado si el registro
	no tiene marcas de nuevo o borrado.
	Hay que verificar que el item sea de registro
	*/
	m4uint8_t			iScope = M4CL_ITEM_SCOPE_NODE ;
	m4bool_t			bIsNew = M4_FALSE ;
	m4bool_t			bIsDeleted = M4_FALSE ;
	_ClRegister_Base	*poRegister = NULL ;

	iScope = m_pItem_Interface->ItemDef.Scope() ;

	if( iScope == M4CL_ITEM_SCOPE_REGISTER )
	{
		poRegister = GetpRegister() ;

		if( poRegister != NULL )
		{
			poRegister->IsNew( bIsNew ) ;
			poRegister->IsDeleted( bIsDeleted ) ;
		}
	}

	if( bIsNew == M4_FALSE && bIsDeleted == M4_FALSE )
	{
		_GetpItem_Interface()->_NotifyOperation (M4DM_OPERATION_UPDATE);

		if (m_oContext.pItemDef_Interface->AffectsDB())
		{
			_GetpItem_Interface()->_NotifyOperation (M4DM_OPERATION_UPDATE_BY_DDBB);
		}
	}

    return M4_SUCCESS;
}

m4return_t ClValue_State::_SetVariant (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus)
{
    void     * paddress, * pvalueadd;
    m4uint32_t isize, iretsize;
    m4return_t res;
    m4bool_t   hasslices;

    res = _GetToVoid (ai_var, isize, paddress);

    if (res == M4_SUCCESS)
    {
        if (paddress)
		{
            res = SetNotUpdated (paddress, isize, 0, &iretsize, ai_bChangeStatus, ai_var.IsUser);
		}
        else
        {
            res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

            if (res == M4_SUCCESS)
            {
                res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

                if (hasslices)
				{
                    GetpKernelRW ()->SetNull(m_oContext, Index, M4_TRUE, &pvalueadd);
				}
                else
				{
                    m_oContext.pMemory_Interface->SetNull(m_oContext, ai_bChangeStatus);
				}
            }
        }
    }

    return res;
}

// estamos dentro del mismo tramo
m4return_t ClValue_State::_SetNoNotify (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus)
{
	m4return_t res;
    m4VariantType var;
    m4uint8_t itype;

    switch (ai_var.Type)
    {
    case M4CL_CPP_TYPE_NULL:
        return _SetVariant (var, ai_bChangeStatus);
        break;
	/* Bug 0089297
	Hay que considerar el tipo M4CL_CPP_TYPE_STRING_SYS para la genconn
	*/
    case M4CL_CPP_TYPE_STRING_VAR:
    case M4CL_CPP_TYPE_STRING_SYS:

        var.IsUser = ai_var.IsUser;
        var.Type = ai_var.Type;
        var.Data.PointerChar = ai_var.Data.PointerChar;

        m_oContext.pItemDef_Interface->CppType (itype);
        if (itype != M4CL_CPP_TYPE_STRING)
        {
            if (m_oContext.pItemDef_Interface->M4Type () != M4CL_M4_TYPE_VARIANT)
            {
				m4uint32_t iItemHandle  = m_oContext.pItemDef_Interface->GetHandle();
				ClCompiledMCR * pmcr   = m_oContext.pItemDef_Interface->GetpCMCR ();

                DUMP_CHLOG_WARNINGF(M4_CH_DM_SET_VALUE_CONVERSION, M4ClCppType(var.Type) << M4ClCppType(itype) << M4ObjNodeItemI(pmcr, iItemHandle));
            }
            res = _Convert (var);
        }
        return _SetVariant (var, ai_bChangeStatus);
        break;

    default:

        var = ai_var;
        m_oContext.pItemDef_Interface->CppType (itype);
        if (itype != var.Type)
        {
            if (m_oContext.pItemDef_Interface->M4Type () != M4CL_M4_TYPE_VARIANT)
            {
				m4uint32_t iItemHandle  = m_oContext.pItemDef_Interface->GetHandle();
				ClCompiledMCR * pmcr   = m_oContext.pItemDef_Interface->GetpCMCR ();

                DUMP_CHLOG_WARNINGF(M4_CH_DM_SET_VALUE_CONVERSION, M4ClCppType(var.Type) << M4ClCppType(itype) << M4ObjNodeItemI(pmcr, iItemHandle));
            }
            res = _Convert (var);
        }
        return _SetVariant (var, ai_bChangeStatus);
    }
}

m4return_t ClValue_State::_AddSlices(m4uint32_t & ao_index, m4date_t ai_startdate,
                                     m4date_t ai_enddate)
{
	m4bool_t hasslices;
	m4return_t res;
    void * paddress, * pslice, * pPointer;
    m4uint32_t isize, iretsize;

	priority_t		pNewPriority;
    m4VariantType   newValue;
    m4VariantType	vDefaultValue;
    m4VariantType	vTempValue;


	res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);
    if (res == M4_SUCCESS)
    {
		// Bug 0077473
		_GetDefaultWithDates( ai_startdate, ai_enddate, vDefaultValue ) ;

        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &paddress, hasslices);

	    if (hasslices)
        {
            m4date_t        dFirstStart,
                            dLastEnd,
                            dFirstSliceEnd,
                            dLastSliceStart,
                            dAuxSliceStart,
                            dAuxSliceEnd,
                            dUnitDate;
            m4date_t        pDates[4];

            m4uint32_t      icount,
                            index,
                            index2,
                            i, j;
            m4VariantType   var1,
                            var2;

            m4pchar_t auxpointer;
	        m_oContext.GetUnitDate (dUnitDate);

        // CASO 1: Ambas fechas fuera de rango => Crear dos tramos.

            GetpKernelRW ()->GetFirstStartDate (m_oContext, dFirstStart, &paddress);

        // CASO 1.1: Añado un solo tramo, por la coincidencia de fechas

        //  [tramonuevo][tramoviejo]
            if ((ai_enddate + dUnitDate) == dFirstStart)
            {
                GetpKernelRW ()->Insert (m_oContext, 0, 1, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);
                ao_index = 0;

                return M4_SUCCESS;
            }

            if ((ai_enddate + dUnitDate) < dFirstStart)
            {
                GetpKernelRW ()->Insert (m_oContext, 0, 2, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);

                _GetDefaultWithDates( ai_enddate + dUnitDate, dFirstStart - dUnitDate, vTempValue ) ;
                _GetToVoid(vTempValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 1, pPointer, isize, 0, iretsize, &paddress);
                ao_index = 0;

                return M4_SUCCESS;
            }

            GetpKernelRW ()->GetLastEndDate (m_oContext, dLastEnd, &paddress);

        //  [tramoviejo][tramonuevo]
            if (ai_startdate == (dLastEnd + dUnitDate))
            {
                GetpKernelRW ()->Count (m_oContext, icount, &paddress);
                GetpKernelRW ()->Insert (m_oContext, icount, 1, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, icount, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, icount, ai_enddate, &paddress);
                ao_index = icount;

                return M4_SUCCESS;
            }

            if (ai_startdate > (dLastEnd + dUnitDate))
            {
                GetpKernelRW ()->Count (m_oContext, icount, &paddress);
                GetpKernelRW ()->Insert (m_oContext, icount, 2, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount + 1, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, (icount + 1), ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, (icount + 1), ai_enddate, &paddress);

                _GetDefaultWithDates( dLastEnd + dUnitDate, ai_startdate - dUnitDate, vTempValue ) ;
                _GetToVoid(vTempValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount, pPointer, isize, 0, iretsize, &paddress);

                // El antiguo ultimo tramo se ha quedado sin fecha de fin
                GetpKernelRW ()->SetEndDate(m_oContext, (icount - 1), dLastEnd, &paddress);
                ao_index = icount + 1;

                return M4_SUCCESS;
            }

        // CASO 2: Solapamos con un tramo: el ultimo o el primero => Añadir 1 tramo.

            GetpKernelRW ()->GetEndDate (m_oContext, 0, dFirstSliceEnd, &paddress);

        // CASO 2.1: No añado tramo porque me coincide un fecha con el tramo viejo:

        //  [nuevo tramo[tramoviejo]
            if ((ai_startdate < dFirstStart)&&(ai_enddate == dFirstSliceEnd))
            {
                GetpKernelRW ()->SetNull(m_oContext, 0, M4_TRUE, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetPriority(m_oContext, 0, 3, &paddress);
                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);
                ao_index = 0;

                return M4_SUCCESS;
            }

            if ((ai_startdate < dFirstStart)&&(ai_enddate < dFirstSliceEnd))
            {
                // comprimo el primer tramo
                pDates[0] = dFirstStart;
                pDates[1] = dFirstSliceEnd;
                pDates[2] = ai_enddate + dUnitDate;
                pDates[3] = dFirstSliceEnd;

                GetpKernelRW ()->GetAddress(m_oContext, 0, &pslice, &paddress);
                _GetToVariant(pslice, var1);
                _GetWithDates(pDates, var1, var2, M4_TRUE);

                _GetToVoid(var2, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, 0, (ai_enddate + dUnitDate), &paddress);

                // Añado el nuevo
                GetpKernelRW ()->Insert (m_oContext, 0, 1, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);
                ao_index = 0;

                return M4_SUCCESS;
            }

            GetpKernelRW ()->Count (m_oContext, icount, &paddress);
            GetpKernelRW ()->GetStartDate (m_oContext, (icount - 1), dLastSliceStart, &paddress);

        //  [tramoviejo]nuevo tramo]
            if ((ai_enddate > dLastEnd)&&(ai_startdate == dLastSliceStart))
            {
                GetpKernelRW ()->SetNull(m_oContext,  (icount - 1), M4_TRUE, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount - 1, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetPriority(m_oContext, (icount - 1), 3, &paddress);
                GetpKernelRW ()->SetStartDate(m_oContext, (icount - 1), ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, (icount - 1), ai_enddate, &paddress);
                ao_index = icount - 1;

                return M4_SUCCESS;
            }

            if ((ai_enddate > dLastEnd)&&(ai_startdate > dLastSliceStart))
            {
                // comprimo el ultimo tramo
                pDates[0] = dLastSliceStart;
                pDates[1] = dLastEnd;
                pDates[2] = dLastSliceStart;
                pDates[3] = ai_startdate - dUnitDate;

                GetpKernelRW ()->GetAddress(m_oContext, (icount - 1), &pslice, &paddress);
                _GetToVariant(pslice, var1);
                _GetWithDates(pDates, var1, var2, M4_TRUE);

                _GetToVoid(var2, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount - 1, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetEndDate(m_oContext, (icount - 1), (ai_startdate - dUnitDate), &paddress);

                // Añado el nuevo
                GetpKernelRW ()->Insert (m_oContext, icount, 1, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, icount, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, icount, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, icount, ai_enddate, &paddress);
                ao_index = icount;

                return M4_SUCCESS;
            }

        // CASO 3: Solapamos con varios tramos: ya no se añaden tramos.

            if ((ai_startdate < dFirstStart)&&(ai_enddate < dLastEnd))
            {
                GetpKernelRW ()->FindByDate (m_oContext, ai_enddate, index, &paddress);

                // Borro todos los tramos que hay demas menos uno que uso como nuevo.
                j = 0;
                for (i = 0; i < (index - 1); i ++)
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);
                // Ahora, similar a caso 2, solo que sin añadir.

                // Necesito las fechas del tramo que queda en medio.
                GetpKernelRW ()->GetStartDate (m_oContext, 1, dAuxSliceStart, &paddress);
                GetpKernelRW ()->GetEndDate (m_oContext, 1, dAuxSliceEnd, &paddress);

				/* Bug 0078289
				Si la fecha fin del tramo 1 coincide con la de fin que se pide
				no hay que hacer prorrateo, sino borrar el tramo
				*/
				if( dAuxSliceEnd == ai_enddate )
				{
					j = 0;
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);
				}
				else
				{
					// comprimo el primer tramo
					pDates[0] = dAuxSliceStart;
					pDates[1] = dAuxSliceEnd;
					pDates[2] = ai_enddate + dUnitDate;
					pDates[3] = dAuxSliceEnd;

					GetpKernelRW ()->GetAddress(m_oContext, 1, &pslice, &paddress);
					_GetToVariant(pslice, var1);
					_GetWithDates(pDates, var1, var2, M4_TRUE);

					_GetToVoid(var2, isize, pPointer);
					GetpKernelRW ()->SetValue(m_oContext, 1, pPointer, isize, 0, iretsize, &paddress);

					GetpKernelRW ()->SetStartDate(m_oContext, 1, (ai_enddate + dUnitDate), &paddress);
				}

                // No añado, pero pongo fechas y valor
                GetpKernelRW ()->SetNull(m_oContext, 0, M4_TRUE, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetPriority(m_oContext, 0, 3, &paddress);
                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                ao_index = 0;

                // No hace falta hacer setenddate,
                // se ha hecho antes en setstart date del antiguo primer tramo

                return M4_SUCCESS;
            }

            if ((ai_enddate > dLastEnd)&&(ai_startdate > dFirstStart))
            {
                GetpKernelRW ()->FindByDate (m_oContext, ai_startdate, index, &paddress);

                // Borro todos los tramos que hay demas menos uno que uso como nuevo.
                j = index + 1;
                for (i = (index + 1); i < (icount - 1); i ++)
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);
                // Ahora, similar a caso 2, solo que sin añadir.

                // Necesito las fechas del tramo que queda en medio.
                GetpKernelRW ()->GetStartDate (m_oContext, index, dAuxSliceStart, &paddress);
                GetpKernelRW ()->GetEndDate (m_oContext, index, dAuxSliceEnd, &paddress);

				/* Bug 0078289
				Si la fecha inicio del tramo penúltimo coincide con la de inicio que se pide
				no hay que hacer prorrateo, sino borrar el tramo
				*/
				if( dAuxSliceStart == ai_startdate )
				{
					j = index + 1;
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);
					index-- ;
				}
				else
				{
					// comprimo el ultimo tramo
					pDates[0] = dAuxSliceStart;
					pDates[1] = dAuxSliceEnd;
					pDates[2] = dAuxSliceStart;
					pDates[3] = ai_startdate - dUnitDate;

					GetpKernelRW ()->GetAddress(m_oContext, index, &pslice, &paddress);
					_GetToVariant(pslice, var1);
					_GetWithDates(pDates, var1, var2, M4_TRUE);

					_GetToVoid(var2, isize, pPointer);
					GetpKernelRW ()->SetValue(m_oContext, index, pPointer, isize, 0, iretsize, &paddress);

					GetpKernelRW ()->SetEndDate(m_oContext, index, (ai_startdate - dUnitDate), &paddress);
				}

                GetpKernelRW ()->SetNull(m_oContext, (index + 1), M4_TRUE, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, index + 1, pPointer, isize, 0, iretsize, &paddress);

				/* Bug 0076620
				Ese índice que se ha escapado, debe ser index + 1 y no 0
				*/
                GetpKernelRW ()->SetPriority(m_oContext, index + 1, 3, &paddress);
                ao_index = index + 1;
                // No hace falta hacer setstartdate,
                // se ha hecho antes en setend date del antiguo ultimo tramo
                GetpKernelRW ()->SetEndDate(m_oContext, (index + 1), ai_enddate, &paddress);

                return M4_SUCCESS;
            }

        // CASO 4: Tramo nuevo esta en medio.

            if ((ai_startdate >= dFirstStart)&&(ai_enddate <= dLastEnd))
            {
				/* Bug 0076620
				Hay que poner la prioridad y el valor del tramo  existente cuando corresponda (caso 4.a)
				De momento se pone a lo que había.
				*/
				pNewPriority = 3 ;
				newValue = vDefaultValue ;

                GetpKernelRW ()->FindByDate (m_oContext, ai_startdate, index, &paddress);
                GetpKernelRW ()->FindByDate (m_oContext, ai_enddate, index2, &paddress);

                // tengo que insertar 2 tramos: [tramo[tramo nuevo]viejo]
                if (index == index2)
                {
                    // Cojo fecha inicio y fin del tramo viejo, para asignarla a los nuevos
                    GetpKernelRW ()->GetStartDate(m_oContext, index, dAuxSliceStart, &paddress);
                    GetpKernelRW ()->GetEndDate(m_oContext, index, dAuxSliceEnd, &paddress);

					// Optimización. Si las fechas coinciden ya está
					if( ai_startdate == dAuxSliceStart && ai_enddate == dAuxSliceEnd )
					{
						ao_index = index;
						return M4_SUCCESS;
					}

					/* Bug 0076620
					En este caso hay que dejar la prioridad y el valor que había
					*/
					GetpKernelRW ()->GetPriority(m_oContext, index, pNewPriority, &paddress);
					pDates[0] = dAuxSliceStart;
					pDates[1] = dAuxSliceEnd;
					pDates[2] = ai_startdate;
					pDates[3] = ai_enddate;

					GetpKernelRW ()->GetAddress(m_oContext, index, &pslice, &paddress);
					_GetToVariant(pslice, var1);
					_GetWithDates(pDates, var1, newValue, M4_TRUE);
					/* Fin bug 0076620 */


                    // Cojo el valor para idem
                    GetpKernelRW ()->GetAddress(m_oContext, index, &pslice, &paddress);
                    _GetToVariant(pslice, var1);
                    if (var1.Type == M4CL_CPP_TYPE_STRING_VAR)
                    {
                        auxpointer = var1.Data.PointerChar;
                        var1.SetString(auxpointer, var1.IsUser);
                    }

                    GetpKernelRW ()->Insert (m_oContext, index + 1, 2, &paddress);

                    //Ojo!!! inconsistencia temporal de fechas!!!
                    GetpKernelRW ()->SetEndDate(m_oContext, index, dAuxSliceEnd, &paddress);
                    GetpKernelRW ()->SetStartDate(m_oContext, index + 2, dAuxSliceStart, &paddress);

                    _GetToVoid(var1, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index + 2, pPointer, isize, 0, iretsize, &paddress);

					/* Bug 0076620
					En este caso hay que copiar también la prioridad
					*/
                    GetpKernelRW ()->SetPriority(m_oContext, index + 2, pNewPriority, &paddress);

                    var1.SetNull();
                }

                // tengo que insertar 1 tramo: [tramo viejo1[tramo nuevo]tramo viejo2]
                if ((index + 1) == index2)
                {
                    //Cojo la fecha de finde del viejo1
                    GetpKernelRW ()->GetEndDate(m_oContext, index, dAuxSliceEnd, &paddress);
                    //Cojo la fecha de inicio de del viejo2
                    GetpKernelRW ()->GetStartDate(m_oContext, index + 1, dAuxSliceStart, &paddress);

                    GetpKernelRW ()->Insert (m_oContext, index + 1, 1, &paddress);

                    //Ojo!!! inconsistencia temporal de fechas!!!
                    GetpKernelRW ()->SetEndDate(m_oContext, index, dAuxSliceEnd, &paddress);
                    GetpKernelRW ()->SetStartDate(m_oContext, index + 2, dAuxSliceEnd, &paddress);
                }

                //Ahora, puedo seguir.
                //Borro lo que estan en medio demas, menos uno que sera el nuevo.
                j = index + 1;
                for (i = (index + 2); i < index2; i ++)
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);

                // Ahora se pueden dar tres situaciones:

                GetpKernelRW ()->GetStartDate (m_oContext, index, dAuxSliceStart, &paddress);
                GetpKernelRW ()->GetEndDate (m_oContext, index + 2, dAuxSliceEnd, &paddress);

                //situacion 1: La mas compleja porque no hay coincidencia de fechas
                if ((ai_startdate != dAuxSliceStart)&&(ai_enddate != dAuxSliceEnd))
                {
                    // Necesito las fechas del priemro de los 2 tramos que quedan en medio.
                    GetpKernelRW ()->GetStartDate (m_oContext, index, dAuxSliceStart, &paddress);
                    GetpKernelRW ()->GetEndDate (m_oContext, index, dAuxSliceEnd, &paddress);

                    // comprimo el tramo
                    pDates[0] = dAuxSliceStart;
                    pDates[1] = dAuxSliceEnd;
                    pDates[2] = dAuxSliceStart;
                    pDates[3] = ai_startdate - dUnitDate;

                    GetpKernelRW ()->GetAddress(m_oContext, index, &pslice, &paddress);
                    _GetToVariant(pslice, var1);
                    _GetWithDates(pDates, var1, var2, M4_TRUE);

                    _GetToVoid(var2, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetEndDate(m_oContext, index, (ai_startdate - dUnitDate), &paddress);

                    // Necesito las fechas del segundo de los 2 tramos que quedan en medio.
                    GetpKernelRW ()->GetStartDate (m_oContext, (index + 2), dAuxSliceStart, &paddress);
                    GetpKernelRW ()->GetEndDate (m_oContext, (index + 2), dAuxSliceEnd, &paddress);

                    // comprimo el tramo
                    pDates[0] = dAuxSliceStart;
                    pDates[1] = dAuxSliceEnd;
                    pDates[2] = ai_enddate + dUnitDate;
                    pDates[3] = dAuxSliceEnd;

                    GetpKernelRW ()->GetAddress(m_oContext, index + 2, &pslice, &paddress);
                    _GetToVariant(pslice, var1);
                    _GetWithDates(pDates, var1, var2, M4_TRUE);

                    _GetToVoid(var2, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index + 2, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetStartDate(m_oContext, (index + 2), (ai_enddate + dUnitDate), &paddress);

                    GetpKernelRW ()->SetNull(m_oContext, (index + 1), M4_TRUE, &paddress);

                    _GetToVoid(newValue, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index + 1, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetPriority(m_oContext, (index + 1), pNewPriority, &paddress);
                    ao_index = index + 1;
                    // No hace falta hacer setstartdate ni setenddate, ambos estan ya hechos

                    return M4_SUCCESS;
                }
                //situacion 2: Una fecha ( la de inicio o fin) con la de uno de los tramos
                if ((ai_startdate == dAuxSliceStart)&&(ai_enddate != dAuxSliceEnd))
                {
                    // sobra un tramo
                    GetpKernelRW ()->Delete(m_oContext, index, &paddress);

                    GetpKernelRW ()->GetStartDate (m_oContext, index + 1, dAuxSliceStart, &paddress);
                    GetpKernelRW ()->GetEndDate (m_oContext, index + 1, dAuxSliceEnd, &paddress);

                    // comprimo el tramo index + 1
                    pDates[0] = dAuxSliceStart;
                    pDates[1] = dAuxSliceEnd;
                    pDates[2] = ai_enddate + dUnitDate;
                    pDates[3] = dAuxSliceEnd;

                    GetpKernelRW ()->GetAddress(m_oContext, index + 1, &pslice, &paddress);
                    _GetToVariant(pslice, var1);
                    _GetWithDates(pDates, var1, var2, M4_TRUE);

                    _GetToVoid(var2, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index + 1, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetStartDate(m_oContext, index + 1, (ai_enddate + dUnitDate), &paddress);

                    GetpKernelRW ()->SetStartDate(m_oContext, index, ai_startdate, &paddress);
                    GetpKernelRW ()->SetNull(m_oContext, index, M4_TRUE, &paddress);

                    _GetToVoid(newValue, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetPriority(m_oContext, index, pNewPriority, &paddress);
                    ao_index = index;

                    return M4_SUCCESS;
                }

                if ((ai_startdate != dAuxSliceStart)&&(ai_enddate == dAuxSliceEnd))
                {
                    // sobra un tramo, borro este para mantener la fecha de inicio del tramo nuevo
                    m4uint32_t aux_index = index + 2;
                    GetpKernelRW ()->Delete(m_oContext, aux_index, &paddress);

                    GetpKernelRW ()->GetStartDate (m_oContext, index, dAuxSliceStart, &paddress);
                    GetpKernelRW ()->GetEndDate (m_oContext, index, dAuxSliceEnd, &paddress);

                    // comprimo el tramo index + 1
                    pDates[0] = dAuxSliceStart;
                    pDates[1] = dAuxSliceEnd;
                    pDates[2] = dAuxSliceStart;
                    pDates[3] = ai_startdate - dUnitDate;

                    GetpKernelRW ()->GetAddress(m_oContext, index, &pslice, &paddress);
                    _GetToVariant(pslice, var1);
                    _GetWithDates(pDates, var1, var2, M4_TRUE);

                    _GetToVoid(var2, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetEndDate(m_oContext, index, (ai_startdate - dUnitDate), &paddress);

                    GetpKernelRW ()->SetEndDate(m_oContext, index + 1, ai_enddate, &paddress);
                    GetpKernelRW ()->SetNull(m_oContext, (index + 1), M4_TRUE, &paddress);

                    _GetToVoid(newValue, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index + 1, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetPriority(m_oContext, (index + 1), pNewPriority, &paddress);
                    ao_index = index + 1;

                    return M4_SUCCESS;
                }

                //situacion 3: Ambas fechas coinciden con la dels antiguos tramos
                if ((ai_startdate == dAuxSliceStart)&&(ai_enddate == dAuxSliceEnd))
                {
                    // sobran dos tramos
                    GetpKernelRW ()->Delete(m_oContext, index, &paddress);
                    GetpKernelRW ()->Delete(m_oContext, index, &paddress);

                    GetpKernelRW ()->SetStartDate(m_oContext, index, ai_startdate, &paddress);
                    GetpKernelRW ()->SetEndDate(m_oContext, index, ai_enddate, &paddress);

                    GetpKernelRW ()->SetNull(m_oContext, index, M4_TRUE, &paddress);

                    _GetToVoid(newValue, isize, pPointer);
                    GetpKernelRW ()->SetValue(m_oContext, index, pPointer, isize, 0, iretsize, &paddress);

                    GetpKernelRW ()->SetPriority(m_oContext, index, pNewPriority, &paddress);
                    ao_index = index;

                    return M4_SUCCESS;
                }
            }

        // CASO 5: todos los tramos viejos esta en medio!!!

            if ((ai_startdate <= dFirstStart)&&(ai_enddate >= dLastEnd))
            {
                //Borro todos los tramos menos 1
                j = 0;
                for (i = 0; i < (icount - 1); i ++)
                    GetpKernelRW ()->Delete (m_oContext, j, &paddress);

                GetpKernelRW ()->SetNull(m_oContext, 0, M4_TRUE, &paddress);

                _GetToVoid(vDefaultValue, isize, pPointer);
                GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

                GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
                GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);
                GetpKernelRW ()->SetPriority(m_oContext, 0, 3, &paddress);
                ao_index = 0;

                return M4_SUCCESS;
            }
            return M4_ERROR;
        }
        else
        {
            GetpKernelRW ()->Insert(m_oContext, 0, 1, &paddress);
            GetpKernelRW ()->SetStartDate(m_oContext, 0, ai_startdate, &paddress);
            GetpKernelRW ()->SetEndDate(m_oContext, 0, ai_enddate, &paddress);

            _GetToVoid(vDefaultValue, isize, pPointer);
            GetpKernelRW ()->SetValue(m_oContext, 0, pPointer, isize, 0, iretsize, &paddress);

	        ao_index = 0;

            return M4_SUCCESS;
        }
    }

    return M4_ERROR;
}

//-----------------------------------------------------------------------------
// Compactaciones
//-----------------------------------------------------------------------------

m4return_t ClValue_State::_Compact_BaseValue (_ClValue_Read_Slices * ai_pClVal)
{
    m4uint32_t            n = Count ();
	m4return_t            res = M4_SUCCESS;
	m4bool_t              hasslices;
    void                * pvalueadd;

    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);

    if (res == M4_SUCCESS)
    {
        res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);

        if (!hasslices)
		{
            return M4_SUCCESS; //Sin tramos
		}
    }
    else
	{
        return M4_ERROR;
	}

    if (n > 1UL) {    // por supuesto con más de un tramo

        M4_ASSERT (m_oContext.pItemDef_Interface);

        m4uint32_t ms = m_oContext.pItemDef_Interface->MaxSize ();

        m4double_t *pv1, *pv2;
        ClVariantConversor oVariantMediator;
        m4bool_t bequal;
        m4uint32_t i1, i2;

        for (i1 = 0; i1 < n; ++i1)
		{
            GetpKernelRW ()->GetAddress (m_oContext, i1, (void **)&pv1, &pvalueadd);

            for (i2 = i1 + 1; i2 < n; )
			{
                // SameRatios
                {
                    GetpKernelRW ()->GetAddress (m_oContext, i2, (void **)&pv2, &pvalueadd);

                    _SameValues (ai_pClVal, pv1, pv2, bequal);
	                if (bequal)
					{
						priority_t ai_p1, ai_p2;
						GetpKernelRW ()->GetPriority(m_oContext, i1, ai_p1, &pvalueadd);
                    	GetpKernelRW ()->GetPriority(m_oContext, i2, ai_p2, &pvalueadd);

						if (ai_p1 != ai_p2)
						{
							bequal = M4_FALSE;
						}
					}

                }

                if (bequal)
				{
					res = GetpKernelRW ()->Delete (m_oContext, i2, &pvalueadd);
                    n--;    // n se decrementa y de esta forma el bucle 'for' no es infinito

					if (i2 == i1) break;
                    //i2 no se incrementa porque el efecto del borrar el slice actual
                    // es el de pasar al siguiente.
                }
                else
				{
                    // diferentes
                    break;  // se salta el for
                }
            }   // for

            if (res != M4_SUCCESS)
			{
                break;
            }
        }   // for
    }

    return res;
}

m4return_t ClValue_State::_Compact_Ratios (_ClValue_Read_Slices * ai_pClVal)
{

    m4return_t	res = M4_SUCCESS;
	m4bool_t	hasslices;
    void		*pvalueadd;


    res = m_oContext.pMemory_Interface->Validation(m_oContext, 0);
    
	if (res != M4_SUCCESS)
    {
        return M4_ERROR;
    }

    res = m_oContext.pMemory_Interface->GetAddress_andhasslices(m_oContext, &pvalueadd, hasslices);
    
	if (!hasslices)
	{
        return M4_SUCCESS; //Sin tramos
	}

    m4uint32_t	n = Count ();

	if( n <= 1 )
	{
		return( M4_SUCCESS ) ;
	}


    M4_ASSERT (m_oContext.pItemDef_Interface);

    m4uint32_t ms = m_oContext.pItemDef_Interface->MaxSize ();

    m4date_t dt1, dt2;
    m4date_t st1, et2;
    m4double_t v1, v2;
    m4double_t *pv1, *pv2;
    ClVariantConversor oVariantMediator;
    m4bool_t bequal;
    m4uint32_t i1, i2;
	m4double_t dDeltaError = 0.0;

	m4bool_t		bRound = M4_FALSE ;
	m4int32_t		iPrecision = 0 ;
	m4uint8_t		slcbhvr;
	m4date_t		dToCheck, dtExtra;
	ClItem			oItemAuxiliarWalker(0);
	m4uint16_t		iAuxItem1, iAuxItem2;
	m4bool_t		bHayItemsAuxiliares;
	m4VariantType	vTemp;


	bRound = _GetRoundToPrecision( iPrecision ) ;

	//si es UNIT, debemos ver si tiene items auxiliares para hacer corrección
	//o si es valor final que no genera tramos (unidad de incidencia)
	bHayItemsAuxiliares = M4_FALSE;
	slcbhvr = m_oContext.pItemDef_Interface->SliceBhrv ();

	iAuxItem1 = m4uint16_t( -1 ) ;

	// Se busca el item auxiliar si es unidad, incidencia o valor final
	if( slcbhvr == M4CL_ITEM_SLICE_BHRV_UNIT || slcbhvr == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE )
	{
		// Primero se busca el de prorateo
		iAuxItem1 = m_oContext.pItemDef_Interface->AuxiliarItemProratIndex() ;

		if( iAuxItem1 == m4uint16_t( -1 ) )
		{
			// Si no tiene de prorateo se busca el normal para unidades o incidencias
			if( slcbhvr == M4CL_ITEM_SLICE_BHRV_UNIT ||	( slcbhvr == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE && m_oContext.pItemDef_Interface->GenerateSlices() == 0 ) )
			{
				iAuxItem1 = m_oContext.pItemDef_Interface->AuxiliarItemIndex() ;
			}
		}
	}

	if( iAuxItem1 != m4uint16_t( -1 ) )
	{
		ClNode			*poNodeAuxiliarWalker;
		_ClRegister_Base *poRegister;
		
		//nos creamos un item que vaya al auxiliar->Ojo el index que tenemos es por registro
		poRegister = m_oContext.GetpRegister();
		
		if (poRegister)
		{
			//si no hay register->no hay access, y no podemos leer los items auxiliares
			poNodeAuxiliarWalker = poRegister->GetpAccess()->Node.Get(m_oContext.pNode_Data->GetpNodeDef()->GetHandle() );
			oItemAuxiliarWalker  = poNodeAuxiliarWalker->RecordSet.Current.Item;

			//leemos el item auxiliar de este
			iAuxItem2 = oItemAuxiliarWalker[ClItemIndex(iAuxItem1)].ItemDef.AuxiliarItemIndex();
			if (iAuxItem2!=(m4uint16_t)-1) bHayItemsAuxiliares = M4_TRUE; //el item tiene item auxiliar
		}
	}

    for (i1 = 0; i1 < n; ++i1)
	{
        GetpKernelRW ()->GetAddress (m_oContext, i1, (void **)&pv1, &pvalueadd);

        if (pv1)
		{
            v1 = *pv1;
		}
        else
		{
            v1 = 0.0;
		}

        res = GetpKernelRW ()->GetStartDate (m_oContext, i1, st1, &pvalueadd);
        res = GetpKernelRW ()->GetEndDatePlusOne (m_oContext, i1, dt1, &pvalueadd);

		dtExtra = 0;
		if(bHayItemsAuxiliares)
		{ //tratamiento en caso de haber items auxiliares

			if ((oItemAuxiliarWalker[ClItemIndex(iAuxItem1)].Value[st1].Get(vTemp) == M4_SUCCESS) && (vTemp.Type == M4CL_CPP_TYPE_DATE) )
			{
				dToCheck = vTemp.Data.DoubleData;

				if ((oItemAuxiliarWalker[ClItemIndex(iAuxItem2)].Value[st1].Get(vTemp) == M4_SUCCESS) && (vTemp.Type == M4CL_CPP_TYPE_DATE) )
				{
					if((st1<=dToCheck) && (dToCheck<dt1))
					{
						//el dominio temporal contiene la discontinidad
						dtExtra = vTemp.Data.DoubleData - dToCheck;
					}
				}
			}
		}
		dt1 = dt1 - st1 + dtExtra;

        if (dt1 == 0.0)
		{
            res = M4_ERROR;
            break;
        }

        for (i2 = i1 + 1; i2 < n; )
		{
            // SameRatios
            GetpKernelRW ()->GetAddress (m_oContext, i2, (void **)&pv2, &pvalueadd);

            if (pv2)
			{
                v2 = *pv2;
			}
            else
			{
                v2 = 0.0;
			}

            res = GetpKernelRW ()->GetEndDatePlusOne (m_oContext, i2, et2, &pvalueadd);
            m4date_t sd2;
            res = GetpKernelRW ()->GetStartDate (m_oContext, i2, sd2, &pvalueadd);

			dtExtra = 0;
			if(bHayItemsAuxiliares)
			{ //tratamiento en caso de haber items auxiliares

				if ((oItemAuxiliarWalker[ClItemIndex(iAuxItem1)].Value[sd2].Get(vTemp) == M4_SUCCESS) && (vTemp.Type == M4CL_CPP_TYPE_DATE) )
				{
					dToCheck = vTemp.Data.DoubleData;
					if ((oItemAuxiliarWalker[ClItemIndex(iAuxItem2)].Value[sd2].Get(vTemp) == M4_SUCCESS) && (vTemp.Type == M4CL_CPP_TYPE_DATE) )
					{
						if((sd2<=dToCheck) && (dToCheck<et2))
						{
							//el dominio temporal contiene la discontinidad
							dtExtra = vTemp.Data.DoubleData - dToCheck;
						}
					}
				}
			}
			dt2 = et2 - sd2 + dtExtra;

			if(dt2 == 0.0)
			{
				if(fabs(v2) <= M4_LN4_MATH_PRECISION)
				{
					//Caso en el que hay item auxiliar y a la porción de tramo se le resta un número de días igual
					// al número de días del tramo. En este caso v2 debe venir con valor 0. Compactamos este
					// tramo con el de su izquierda (con el anterior temporalmente), porque es el que tiene sentido
					// funcional (la discontinuidad en el tramo está casi siempre a final de mes).
					bequal = M4_TRUE;
				}
				else
				{ // error chungo
					res = M4_ERROR;
					break;  // se salta el for
				}
			}
			else
			{
				if( bRound == M4_TRUE )
				{
					//el 1.0001 es porque hay que dar algo de holgura,
					// para evitar el error de C++ con dos números en teoría iguales.
					dDeltaError = 1.0001 * pow( 10.0, -m4double_t(iPrecision) ) / 2.0;
				}
				else
				{
					/* Bug 0110967
					Se obsoletiza la vieja formula y se usa la de redondeo con precisión 9
					*/
					dDeltaError = 1.0001 * M4_LN4_MATH_PRECISION / 2.0;
				}

				if( ( (v2-dDeltaError)*dt1 - (v1+dDeltaError)*dt2 <= 0.0 ) && ( (v1-dDeltaError)*dt2 - (v2+dDeltaError)*dt1 <= 0.0 ) )
				{
					bequal = M4_TRUE;
				}
				else
				{
					bequal = M4_FALSE;
				}
			}

			/* Bug 0078303
			Si uno de ellos es nulo y el otro no, no son realmente iguales
			*/
	        if( bequal == M4_TRUE )
			{
				if( ( pv1 == NULL && pv2 != NULL ) || ( pv2 == NULL && pv1 != NULL ) )
				{
					bequal = M4_FALSE ;
				}
			}

	        if (bequal)
			{
				priority_t ai_p1, ai_p2;
				GetpKernelRW ()->GetPriority(m_oContext, i1, ai_p1, &pvalueadd);
                GetpKernelRW ()->GetPriority(m_oContext, i2, ai_p2, &pvalueadd);

				if (ai_p1 != ai_p2)
				{
					bequal = M4_FALSE;
				}
			}

            if (bequal) {

				/* Bug 0093201
				El valor resultante es la suma de los dos tramos
				Así no hace falta hacer proporciones ni tener en cuenta el
				ajuste
				*/
				if( pv1 != NULL )
				{
					*pv1 = *pv1 + v2;
				}

                res = GetpKernelRW ()->Delete (m_oContext, i2, &pvalueadd);
                n--;    // n se decrementa y de esta forma el bucle 'for' no es infinito

				if (i2 == i1) break;
                //i2 no se incrementa porque el efecto del borrar el slice actual
                // es el de pasar al siguiente.
            }
            else
			{
                // diferentes
                break;  // se salta el for
            }
        }   // for

        if (res != M4_SUCCESS)
		{
            break;
        }
    }   // for

    return res;
}

m4return_t ClValue_State::_SameValues (_ClValue_Read_Slices * ai_pClVal, void *ai_p1, void *ai_p2, m4bool_t &ao_bresult)
{
    m4return_t res = M4_SUCCESS;

    // falla con los blobs, porque p1 y p2 no serían las verdaderas direcciones

    M4_ASSERT (m_oContext.pItemDef_Interface);

    if (ai_p1)
	{
        if (ai_p2)
		{
		    switch (m_oContext.pItemDef_Interface->CppType ())
			{
			    case M4CL_CPP_TYPE_NUMBER:
			    case M4CL_CPP_TYPE_DATE:
				    ao_bresult = ( fabs( *((m4double_t *)ai_p1) - *((m4double_t *)ai_p2 )  ) <=M4_LN4_MATH_PRECISION ? M4_TRUE : M4_FALSE );
				    break;

	            case M4CL_CPP_TYPE_STRING:
				    if (strcmp ( (m4pchar_t)ai_p1, (m4pchar_t)ai_p2))
					{ // cuidado con unicode
					    ao_bresult = M4_FALSE;
				    }
	                else
					{
		                ao_bresult = M4_TRUE;
			        }
				    break;

	            default:
		            res = M4_ERROR; // tipo no válido
		    }            
        }
        else
		{
            ao_bresult = M4_FALSE;
        }
    }
    else
	{
        if (ai_p2)
		{
            ao_bresult = M4_FALSE;
        }
        else
		{
            ao_bresult = M4_TRUE;
        }
    }

    return res;
}

//****************************
// GET SIZE
///***************************

m4return_t ClValue_State::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClValue_State);
	ao_size-=sizeof(m_oContext);
	
	return m_oContext.GetSize(ao_size);
}

