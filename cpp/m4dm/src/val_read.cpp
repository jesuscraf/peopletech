//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_read.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             27/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//
//
//==============================================================================

#include "val_read.hpp"
#include "valh_rs.hpp"
#include "virt_val.hpp"
#include "itemdef.hpp"
#include "item.hpp"
#include "node_knl.hpp"
#include "nodedef.hpp"
#include "m4mdrt.hpp"
#include "chan_knl.hpp"
#include "chlog.hpp"
#include "v_contxt.hpp"

void *ClValue_Read::Get (_ClValue_Read_Slices * ai_pClVal, m4return_t * ao_pdone)
{
    if (ao_pdone)
        (* ao_pdone) = M4_SUCCESS;

    m4return_t res;
	m4bool_t hasslices;
    void * paddress, * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_READ_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
            res = ai_pClVal->GetpKernelR ()->GetAddress (ai_pClVal->m_oContext, ai_pClVal->Index, &paddress, &pvalueadd);
        else
            paddress = pvalueadd;

        return paddress;
    }
    else
		return 0;
}

m4return_t ClValue_Read::Get (_ClValue_Read_Slices * ai_pClVal, m4VariantType &ao_var, m4return_t * ao_pdone)
{
    m4return_t res;
	m4bool_t hasslices;
    void * paddress, * pvalueadd;

    if( ao_pdone != NULL )
	{
        *ao_pdone = M4_SUCCESS;
	}

	ao_var.SetNull() ;


    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_READ_OPERATION);

    if( res != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
	}

    res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);

    if( res != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
	}

    if (hasslices)
	{
        res = ai_pClVal->GetpKernelR ()->GetAddress (ai_pClVal->m_oContext, ai_pClVal->Index, &paddress, &pvalueadd);

		if( res != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}
    else
	{
        paddress = pvalueadd;
	}

    if (paddress)
    {
        ai_pClVal->m_oContext.pMemory_Interface->GetIsUser(ai_pClVal->m_oContext, ao_var.IsUser);

        m4uint8_t itype = ai_pClVal->m_oContext.pItemDef_Interface->CppType ();
        switch (itype) {
            case M4CL_CPP_TYPE_STRING:
                ao_var.Type = M4CL_CPP_TYPE_STRING_VAR;
				ao_var.Data.PointerChar = (m4pchar_t)paddress;
                break;

            case M4CL_CPP_TYPE_NUMBER:
            case M4CL_CPP_TYPE_DATE:
                ao_var.Type = itype;
                ao_var.Data.DoubleData = *((m4double_t *)paddress);
                break;

            default:
                // tipo no válido
                res = M4_ERROR;
        }
    }

    return res;
}

m4return_t ClValue_Read::Get (_ClValue_Read_Slices * ai_pClVal, m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone)
{
    if (ao_pdone)
        (* ao_pdone) = M4_SUCCESS;

    void * address;

	if (ai_pClVal->m_oContext.pItemDef_Interface->CppType () != M4CL_CPP_TYPE_STRING) 
	{
		if ( ao_retSize != NULL ) *ao_retSize = ai_size;
		return M4_ERROR;
	}

	address = Get (ai_pClVal);

	return ai_pClVal->_GetSubString (ao_buffer, address, ai_size, ai_offset, ao_retSize);
}

// Get promediado según el tipo de slice
// si no tiene tramos, devolvemos el valor actual:
m4return_t ClValue_Read::Get (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone)
{
    if (ao_pdone)
        (* ao_pdone) = M4_SUCCESS;

    if (ai_date1 > ai_date2)
        //Hay que agragar un mensaje de error.
        return M4_ERROR;

    m4uint32_t      index1,
                    index2,
                    icount,
                    i;
    m4uint8_t       cpptype,
                    slcbhvr;

    m4bool_t        bBefore = M4_FALSE,
                    bAfter = M4_FALSE;

    m4date_t        dAuxStart,
                    dAuxEnd,
                    dFirstStart,
                    dLastEnd,
                    dUnitDate,
                    pDates[4];

    m4VariantType   vFirst,
                    vLast,
                    vDefaultValue,
                    vAux;

    m4return_t              res;
	m4bool_t                hasslices;
    void                  * pvalueadd,
                          * paddress;


	ao_var.IsUser = M4_TRUE;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_READ_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
        {
            ai_pClVal->GetpKernelR ()->Count (ai_pClVal->m_oContext, icount, &pvalueadd);

			/* Bug 0088076
			Si hay un elemento que tiene 0 tramos se devuelve el valor por defecto.
			Esto no debería pasar nunca.
			*/
			if( icount == 0 )
			{
				ai_pClVal->_GetDefaultWithDates( ai_date1, ai_date2, vAux ) ;
				ao_var.Set( vAux, M4_TRUE ) ;

				return M4_SUCCESS;
			}

            ai_pClVal->GetpKernelR ()->GetFirstStartDate (ai_pClVal->m_oContext, dFirstStart, &pvalueadd);
            ai_pClVal->GetpKernelR ()->GetLastEndDate (ai_pClVal->m_oContext, dLastEnd, &pvalueadd);

            ai_pClVal->GetpKernelR ()->FindByDate (ai_pClVal->m_oContext, ai_date1, index1, &pvalueadd);
            ai_pClVal->GetpKernelR ()->FindByDate (ai_pClVal->m_oContext, ai_date2, index2, &pvalueadd);
        }
        else
        {
            ai_pClVal->m_oContext.GetStartDate(pDates[0]);
            dFirstStart = pDates[0];
            ai_pClVal->m_oContext.GetEndDate(pDates[1]);
            dLastEnd = pDates[1];
            pDates[2] = ai_date1; 
            pDates[3] = ai_date2;

            ai_pClVal->_GetToVariant(pvalueadd, vAux);

            if ((ai_date2 < dFirstStart)||(ai_date1 > dLastEnd))
            {
				ao_var = vAux;
            }
            else
            {
                ai_pClVal->_GetWithDates(pDates, vAux, ao_var, M4_FALSE);
            }

	        ai_pClVal->m_oContext.pMemory_Interface->GetIsUser(ai_pClVal->m_oContext, ao_var.IsUser);
            return M4_SUCCESS;
        }
    }
    else
    {
        ao_var.SetNull();
        return M4_ERROR;
    }

    dAuxStart = ai_date1;
    dAuxEnd = ai_date2;

// CASO 1: Ambas fechas fuera de rango, por un lado OR por otro.

    if ((ai_date2 < dFirstStart)||(ai_date1 > dLastEnd))
    {
		// Bug 0077473
		ai_pClVal->_GetDefaultWithDates( ai_date1, ai_date2, vAux ) ;
		ao_var.Set( vAux, M4_TRUE ) ;

        return M4_SUCCESS;
    }

// CASO 2: la primera fuera.

    if (ai_date1 < dFirstStart)
    {
        bBefore = M4_TRUE;
        index1 = 0;
        dAuxStart = dFirstStart;
    }

    if (ai_date2 > dLastEnd)
    {
        bAfter = M4_TRUE;
        index2 = icount - 1;
        dAuxEnd = dLastEnd;
    }

	// Bug 0077473
	if( ( bBefore ) || ( bAfter ) )
	{
	    ai_pClVal->m_oContext.pItemDef_Interface->GetItemDefaultValue( vDefaultValue ) ;
		ai_pClVal->m_oContext.GetUnitDate( dUnitDate ) ;
	}


//**************************************
// AHORA EL BUCLE

// Este if es para ver si estoy dentro de un solo tramo o entre varios,
    if (index1 != index2)
    {
        ai_pClVal->GetpKernelRW ()->GetStartDate(ai_pClVal->m_oContext, index1, pDates[0], &pvalueadd);
        ai_pClVal->GetpKernelRW ()->GetEndDate(ai_pClVal->m_oContext, index1, pDates[1], &pvalueadd);

        pDates[2] = dAuxStart;
        pDates[3] = pDates[1]; // esto haciendo [/////[XXXX]

        ai_pClVal->GetpKernelRW ()->GetAddress (ai_pClVal->m_oContext, index1, &paddress, &pvalueadd);
        ai_pClVal->_GetToVariant(paddress, vAux);

        ai_pClVal->_GetWithDates(pDates, vAux, vFirst, M4_FALSE);

        ai_pClVal->GetpKernelRW ()->GetStartDate(ai_pClVal->m_oContext, index2, pDates[0], &pvalueadd);
        ai_pClVal->GetpKernelRW ()->GetEndDate(ai_pClVal->m_oContext, index2, pDates[1], &pvalueadd);

        pDates[2] = pDates[0];
        pDates[3] = dAuxEnd; // esto haciendo [XXXX]/////]

        ai_pClVal->GetpKernelRW ()->GetAddress (ai_pClVal->m_oContext, index2, &paddress, &pvalueadd);
        ai_pClVal->_GetToVariant(paddress, vAux);

        ai_pClVal->_GetWithDates(pDates, vAux, vLast, M4_FALSE);
    }
    else
    {
        ai_pClVal->GetpKernelRW ()->GetStartDate(ai_pClVal->m_oContext, index1, pDates[0], &pvalueadd);
        ai_pClVal->GetpKernelRW ()->GetEndDate(ai_pClVal->m_oContext, index1, pDates[1], &pvalueadd);

        pDates[2] = dAuxStart;
        pDates[3] = dAuxEnd; // esto haciendo [///[XXXX]////]

        ai_pClVal->GetpKernelRW ()->GetAddress (ai_pClVal->m_oContext, index1, &paddress, &pvalueadd);
        ai_pClVal->_GetToVariant(paddress, vAux);

        ai_pClVal->_GetWithDates(pDates, vAux, vFirst, M4_FALSE);
    }

	cpptype = ai_pClVal->m_oContext.pItemDef_Interface->CppType ();
	slcbhvr = ai_pClVal->m_oContext.pItemDef_Interface->SliceBhrv ();

    if ((slcbhvr == M4CL_ITEM_SLICE_BHRV_UNIT) && (index1 != index2))
    {
        // Si es Unidad no puedo hacer get entre varios tramos.
        ao_var.SetNull();
        return M4_ERROR;
    }

    if ((cpptype == M4CL_CPP_TYPE_NUMBER)&&
        (slcbhvr == M4CL_ITEM_SLICE_BHRV_FINAL_VALUE))
    {
            ao_var.Type = M4CL_CPP_TYPE_NUMBER;
            ao_var.Data.DoubleData = 0;

            if (bBefore)
            {
                if (vDefaultValue.Type != M4CL_CPP_TYPE_NULL)
				{
					// Bug 0077473
					ai_pClVal->_GetDefaultWithDates( ai_date1, dFirstStart - dUnitDate, vAux ) ;
                    ao_var.Data.DoubleData += vAux.Data.DoubleData;
				}
                else
				{
                    ao_var.SetNull();
				}

            }
            if (bAfter)
            {
                if (vDefaultValue.Type != M4CL_CPP_TYPE_NULL)
				{
					// Bug 0077473
					ai_pClVal->_GetDefaultWithDates( dLastEnd + dUnitDate, ai_date2, vAux ) ;
                    ao_var.Data.DoubleData += vAux.Data.DoubleData;
				}
                else
				{
                    ao_var.SetNull();
				}
            }

            if (index1 != index2)
            {
                if ((ao_var.Type !=  M4CL_CPP_TYPE_NULL)  ||
                    ((vFirst.Type == M4CL_CPP_TYPE_NULL)  &&
                    (vLast.Type  ==  M4CL_CPP_TYPE_NULL)))
                {
                    ao_var.Type = vFirst.Type | vLast.Type;
                    ao_var.Data.DoubleData += vFirst.Data.DoubleData;
                    ao_var.Data.DoubleData += vLast.Data.DoubleData;
                }
                else
                {
                    ao_var.SetNull();
                    return M4_ERROR;
                }
            }
            else
            {
                if ((ao_var.Type != M4CL_CPP_TYPE_NULL) ||
                    (vFirst.Type == M4CL_CPP_TYPE_NULL))
                {
                    ao_var.Type = vFirst.Type | vLast.Type;
                    ao_var.Data.DoubleData += vFirst.Data.DoubleData;
                }
                else
				{
                    ao_var.SetNull();
				}
            }

            for (i = (index1 + 1); i < index2; i ++)
            {
                ai_pClVal->GetpKernelRW ()->GetAddress (ai_pClVal->m_oContext, i, &paddress, &pvalueadd);
                ai_pClVal->_GetToVariant(paddress, vAux);

                if (vAux.Type == M4CL_CPP_TYPE_NUMBER)
				{
                    ao_var.Data.DoubleData += vAux.Data.DoubleData;
				}
            }
            return M4_SUCCESS;
    }
    else
    {
        if ((bBefore)||(bAfter))
        {
            if (((vDefaultValue.Type == M4CL_CPP_TYPE_NULL) &&
                (vFirst.Type == M4CL_CPP_TYPE_NULL))
                ||(vDefaultValue == vFirst))
                ao_var = vDefaultValue;
            else
            {
                ao_var.SetNull();
                return M4_ERROR;
            }
        }
        else
		{
            ao_var = vFirst;
		}

        if (index1 != index2)
        {
            if (((vFirst.Type != M4CL_CPP_TYPE_NULL) || (vLast.Type != M4CL_CPP_TYPE_NULL))
                && (vFirst != vLast))
            {
                ao_var.SetNull();
                return M4_ERROR;
            }
        }

        for (i = (index1 + 1); i < index2; i++)
        {
            ai_pClVal->GetpKernelRW ()->GetAddress (ai_pClVal->m_oContext, i, &paddress, &pvalueadd);
            ai_pClVal->_GetToVariant(paddress, vAux);

            if (((ao_var.Type != M4CL_CPP_TYPE_NULL) || (vAux.Type != M4CL_CPP_TYPE_NULL))
                && (vAux != ao_var))
            {
                ao_var.SetNull();
                return M4_ERROR;
            }
        }

        return M4_SUCCESS;
    }

    return M4_ERROR;
}

m4pchar_t ClValue_Read::GetDescr(_ClValue_Read_Slices * ai_pClVal)
{
    m4pchar_t address = 0;
    m4return_t res;

    if (ai_pClVal->m_oContext.pItemDef_Interface->M4Type () == M4CL_M4_TYPE_BLOB)
    {
        res = ai_pClVal->GetpKernelR ()->GetDescription (ai_pClVal->m_oContext, ai_pClVal->Index, (void **) &address);
	    if (res != M4_SUCCESS)
		    return 0;
    }

    return address;
}

m4return_t ClValue_Read::ChangedMask(_ClValue_Read_Slices * ai_pClVal, m4uint32_t & ao_oChangedMask)
{
    if (ai_pClVal->m_oContext.pItemDef_Interface->M4Type () == M4CL_M4_TYPE_BLOB)
        return ai_pClVal->GetpKernelRW ()->ChangedMask(ai_pClVal->m_oContext, ai_pClVal->Index, ao_oChangedMask);
    else
        return M4_ERROR;
}

m4return_t ClValue_Read::FillDateStack (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack)
{
    return ai_pClVal->FillDateStack (ai_stack);
}

/************************************************************************************/
// Clase Nula

m4return_t ClValue_Read_Nihil::Get (_ClValue_Read_Slices * ai_pClVal, m4VariantType &ao_var, m4return_t * ao_pdone)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

    ao_var.SetNull();

	if (ItemHandle != 0)
	{
        if (ao_pdone)
            (* ao_pdone) = M4_ERROR;
        else
        {
            DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_READ, M4ObjNodeItemI(pmcr,
                ItemHandle));
        }
        return M4_SUCCESS;
	}
	else
	{
        if (ao_pdone)
            (* ao_pdone) = M4_SUCCESS;

		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_GETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Read_Nihil::Get (_ClValue_Read_Slices * ai_pClVal, m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

    * ao_retSize = 0;

	if (ItemHandle != 0)
	{
        if (ao_pdone)
            (* ao_pdone) = M4_ERROR;
        else
        {
            DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_READ, M4ObjNodeItemI(pmcr,
                ItemHandle));
        }
        return M4_SUCCESS;
	}
	else
	{
        if (ao_pdone)
            (* ao_pdone) = M4_SUCCESS;

		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_GETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Read_Nihil::Get (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

    ao_var.SetNull();

	if (ItemHandle != 0)
	{
        if (ao_pdone)
            (* ao_pdone) = M4_ERROR;
        else
        {
            DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_READ, M4ObjNodeItemI(pmcr,
                ItemHandle));
        }
        return M4_SUCCESS;
	}
	else
	{
        if (ao_pdone)
            (* ao_pdone) = M4_SUCCESS;

		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_GETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}
