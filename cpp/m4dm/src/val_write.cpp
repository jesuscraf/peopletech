//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_write.hpp
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
//	Write Interface for Value
//
//==============================================================================

#include "val_write.hpp"
#include "virt_val.hpp"
#include "medvarin.hpp"

#include "item.hpp" //#include "i_item.hpp"
#include "itemdef.hpp" //#include "i_itdef.hpp"
#include "chan_knl.hpp" //#include "i_datinf.hpp"

#include "node_knl.hpp"
#include "nodedef.hpp"

#include "v_contxt.hpp"
#include "m4mdrt.hpp"
#include "aregister.hpp"


//-----------------------------------------------------------------------------
// Set
//      operación de escritura
//-----------------------------------------------------------------------------

m4return_t ClValue_Write::Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus)
{
    m4return_t res=M4_SUCCESS;
	priority_t real_prior;
	done = M4_FALSE;

	if (ai_prior > 3) ai_prior = 3;

	real_prior = ai_pClVal->GetPriority ();
	if (ai_prior <= real_prior)
    {
		res = Set (ai_pClVal, ai_var, ai_bChangeStatus);
	    if (res == M4_ERROR)
		    return res;
		res = SetPriority (ai_pClVal, ai_prior);
	    if (res != M4_SUCCESS)
		    return res;
		done = M4_TRUE;
	}

    return res;
}

m4return_t ClValue_Write::Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, m4bool_t ai_bChangeStatus)
{
    m4return_t res;

	//Bug:0067379     Begin  ----------------------------------------
	//GP Cuando se intenta asignar un valor en un item con tipo de dato nulo
	m4uint8_t itype;
	ai_pClVal->m_oContext.pItemDef_Interface->CppType(itype);
	if(itype == M4CL_CPP_TYPE_NULL) {
        m4uint32_t ItemHandle  = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
        ClCompiledMCR * pmcr   = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

		DUMP_CHLOG_ERRORF(M4_CH_DM_SET_ITEM_NULL, M4ObjNodeItemI(pmcr, ItemHandle));

		return M4_ERROR;
	}
	//Bug:0067379     End    ----------------------------------------

	res = ai_pClVal->_SetNoNotify (ai_var, ai_bChangeStatus);
    if (res != M4_ERROR)
        ai_pClVal->_Notify ();

    return res;
}

m4return_t ClValue_Write::Set (_ClValue_Read_Slices * ai_pClVal, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t *ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser)
{
    m4return_t res = ai_pClVal->SetNotUpdated (ai_paddress, ai_size, ai_offset, ao_retSize, ai_bChangeStatus, ai_bIsUser);

    if (res != M4_ERROR)
        ai_pClVal->_Notify ();

    return res;
}


/////////////////////////////////////////////////////////////////////////
//      operación de escritura

m4return_t ClValue_Write::SetDescr(_ClValue_Read_Slices * ai_pClVal, m4pcchar_t ai_pcDescr)
{
	m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    if (ai_pClVal->m_oContext.pItemDef_Interface->M4Type () == M4CL_M4_TYPE_BLOB)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
        if (res == M4_SUCCESS)
        {
            res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
            if (hasslices)
                return ai_pClVal->GetpKernelRW ()->SetDescription(ai_pClVal->m_oContext, ai_pClVal->Index, ai_pcDescr);
            else
                return ai_pClVal->m_oContext.pMemory_Interface->SetDescription (ai_pClVal->m_oContext, ai_pcDescr);
        }
    }

    return M4_ERROR;
}

m4return_t ClValue_Write::SetNULL (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus)
{
	m4VariantType vNull;

    vNull.SetNull();
    return Set(ai_pClVal, vNull, ai_bChangeStatus);
}

m4return_t ClValue_Write::SetDefault (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus)
{
	m4VariantType Def_Val;
	m4return_t res;

	res = ai_pClVal->m_oContext.pItemDef_Interface->GetItemDefaultValue(Def_Val);
	if (res != M4_SUCCESS)
		return res;

    return Set(ai_pClVal, Def_Val, ai_bChangeStatus);
}

//Nueva Funcion: copio el contenido de un item a otro.

m4return_t ClValue_Write::CopyItem (_ClValue_Read_Slices * ai_pClVal, _ClValue_Read_Slices * ai_pClValRead)
{
    m4return_t res;
    void * AddressSource, * AddressDest;
	m4bool_t bhs_source, bhs_dest;
	m4bool_t bIsUser = M4_FALSE;

    res = ai_pClValRead->m_oContext.pMemory_Interface->Validation(ai_pClValRead->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
	{
        res = ai_pClValRead->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClValRead->m_oContext, &AddressSource, bhs_source);
        ai_pClValRead->m_oContext.pMemory_Interface->GetIsUser(ai_pClValRead->m_oContext, bIsUser);
	}
    else
	{
        return M4_ERROR;
	}

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
	{
        ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &AddressDest, bhs_dest);
	}
    else
	{
        return M4_ERROR;
	}

    if (AddressSource != AddressDest)
    {
        if (bhs_dest)
		{
            ai_pClVal->GetpKernelRW()->DeleteAll (ai_pClVal->m_oContext);
		}

        if (ai_pClValRead->m_oContext.pItemDef_Interface->IsVariableLength () != ai_pClVal->m_oContext.pItemDef_Interface->IsVariableLength ())
        {
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
            ClDateStack     oDateStack;
            m4date_t        dstart, dend;
            void            * psaddress, * pdaddress, * pslice;
            m4uint32_t      n, i, auxIndex;
            m4bool_t        bhs;
            m4VariantType   vAux;
            priority_t      iprior;

            ai_pClValRead->m_oContext.GetStartDate(dstart);
            ai_pClValRead->m_oContext.GetEndDate(dend);
            ai_pClValRead->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClValRead->m_oContext, &psaddress, bhs);

			/* Bug 0071032
			Si el origen no tiene tramos se hace una asignación sencilla
			*/
			if( bhs == M4_FALSE )
			{
	            m4bool_t IsNull;
		        m4char_t Prior;

		        ai_pClValRead->m_oContext.pMemory_Interface->IsNull(ai_pClValRead->m_oContext, IsNull);

				if (!IsNull)
				{
					ai_pClValRead->_GetToVariant(psaddress, vAux);
					vAux.IsUser = bIsUser;
					Set(ai_pClVal, vAux, M4_TRUE);
				}

		        ai_pClValRead->m_oContext.pMemory_Interface->GetPriority(ai_pClValRead->m_oContext, Prior);
				ai_pClVal->m_oContext.pMemory_Interface->SetPriority(ai_pClVal->m_oContext, Prior);

	            return M4_SUCCESS;
			}

            ai_pClValRead->GetpKernelR ()->Count(ai_pClValRead->m_oContext, n, &psaddress);

            if (n)
            {
                oDateStack.Init(dstart, dend, (m4uint16_t)(n + 1));
                ai_pClValRead->FillDateStack(&oDateStack);

				/*
				Bug 0088076
				Hay que añadir la última fecha
				*/
				m4date_t	dUnitDate ;
		        ai_pClValRead->m_oContext.GetUnitDate( dUnitDate ) ;
				oDateStack.AddEndDate( dUnitDate ) ;

                ai_pClVal->GetpKernelRW()->DeleteAll(ai_pClVal->m_oContext);
                AllocSlices(ai_pClVal, &oDateStack);
            }
            else
			{
                n = 1;
			}

            ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pdaddress, bhs);

            auxIndex = ai_pClVal->Index;

            for (i = 0; i < n; i++)
            {
                ai_pClVal->Index = i;

                ai_pClValRead->GetpKernelR ()->GetAddress(ai_pClValRead->m_oContext, i, &pslice, &psaddress);
                ai_pClValRead->_GetToVariant(pslice, vAux);
                Set(ai_pClVal, vAux, M4_TRUE);

                ai_pClValRead->GetpKernelR ()->GetPriority(ai_pClValRead->m_oContext, i, iprior, &psaddress);
                ai_pClVal->GetpKernelRW()->SetPriority(ai_pClVal->m_oContext, i, iprior, &pdaddress);
            }

            ai_pClVal->Index = auxIndex;

            return M4_SUCCESS;
        }
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
        else
        {
            m4uint32_t ItemSize, RetSize;
            m4bool_t IsNull;
	        m4char_t Prior;

            ai_pClValRead->m_oContext.pMemory_Interface->GetSize(ai_pClValRead->m_oContext, ItemSize);
	        ai_pClValRead->m_oContext.pMemory_Interface->IsNull(ai_pClValRead->m_oContext, IsNull);
	        ai_pClValRead->m_oContext.pMemory_Interface->GetPriority(ai_pClValRead->m_oContext, Prior);

            if (!IsNull)
            {
                if (bhs_source)
                {
                    ai_pClVal->m_oContext.pMemory_Interface->SetSize(ai_pClVal->m_oContext, ItemSize);
                    ai_pClVal->m_oContext.pMemory_Interface->SetHasSlices(ai_pClVal->m_oContext, bhs_source);  //despues de SetSize
                }
    	        ai_pClVal->m_oContext.pMemory_Interface->SetValueByOffset (ai_pClVal->m_oContext, AddressSource, ItemSize, 0, &RetSize, M4_TRUE, bIsUser);
            }
            /*else
                res = ai_pClVal->m_oContext.pMemory_Interface->SetNull(ai_pClVal->m_oContext);
*/
	        ai_pClVal->m_oContext.pMemory_Interface->SetPriority(ai_pClVal->m_oContext, Prior);

            return M4_SUCCESS;
        }
    }

    return M4_SUCCESS;
}


m4return_t ClValue_Write::SetPriority (_ClValue_Read_Slices * ai_pClVal, priority_t ai_prior)
{
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
		{
            return ai_pClVal->GetpKernelRW ()->SetPriority (ai_pClVal->m_oContext, ai_pClVal->Index, ai_prior, &pvalueadd);
		}
        else
		{
            return ai_pClVal->m_oContext.pMemory_Interface->SetPriority(ai_pClVal->m_oContext, ai_prior);
		}
    }

    return M4_ERROR;
}

m4return_t ClValue_Write::SetStartDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
	m4date_t              sd_next;
    m4return_t            res;
	m4bool_t              hasslices;
    void                * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
        {
	        ai_pClVal->GetpKernelRW ()->GetEndDatePlusOne (ai_pClVal->m_oContext, ai_pClVal->Index, sd_next, &pvalueadd);

            if (ai_date >= sd_next)
            {
                m4uint32_t ItemHandle  = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
                ClCompiledMCR * pmcr   = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

                DUMP_CHLOG_WARNINGF(M4_CH_DM_DATEOUTOFRANGE_SETDATE, DateLog(ai_date)
                    << M4ObjNodeItemI(pmcr, ItemHandle));

                return M4_ERROR;
            }

	        //no se puede sobrepasar la fecha final del siguiente tramo, si existe
            ai_pClVal->GetpKernelRW ()->SetStartDate (ai_pClVal->m_oContext, ai_pClVal->Index, ai_date, &pvalueadd);
        }
        else
            return M4_SUCCESS;
    }

    if (res != M4_ERROR) {
        ai_pClVal->_Notify ();
    }

    return res;
}

m4return_t ClValue_Write::SetEndDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
	m4date_t              ed_next;
    m4uint32_t            n;
    m4return_t            res;
	m4bool_t              hasslices;
    void                * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
        {
            res = ai_pClVal->GetpKernelRW ()->Count (ai_pClVal->m_oContext, n, &pvalueadd);
	        if (res != M4_SUCCESS)
		        return res;

            if (ai_pClVal->Index + 1 < n)
            {
	            res = ai_pClVal->GetpKernelRW ()->GetEndDate (ai_pClVal->m_oContext, ai_pClVal->Index + 1, ed_next, &pvalueadd);
                if (ai_date >= ed_next)
                {
                    m4uint32_t ItemHandle  = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
                    ClCompiledMCR * pmcr   = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

                    DUMP_CHLOG_WARNINGF(M4_CH_DM_DATEOUTOFRANGE_SETDATE, DateLog(ai_date)
                        << M4ObjNodeItemI(pmcr, ItemHandle));

                    return M4_ERROR;
                } 
    	        //no se puede sobrepasar la fecha final del siguiente tramo, si existe
            }

	        res = ai_pClVal->GetpKernelRW ()->SetEndDate (ai_pClVal->m_oContext, ai_pClVal->Index, ai_date, &pvalueadd);
        }
        else
            return M4_SUCCESS;
    }
    else
        return M4_ERROR;

    if (res != M4_ERROR) {
        ai_pClVal->_Notify ();
    }

    return res;
}

// SplitSlice
// cogemos un tramo y lo dividimos en dos, cortando por la fecha que nos indican
// los nuevos valores se calculan prorateando el valor antiguo
//
// Si el item no tiene tramos --> error
// Si el ai_pClVal->Index del Value está en EOF --> error
// Si la fecha de corte no está comprendida entre las del tramo 'current' --> error
// Si la fecha de corte coincide con la de inicio o fin del tramo 'current' --> no hace nada y devuelve exito
//
// ai_pClVal->Index mantiene el valor que tuviera antes de la llamada.

m4return_t ClValue_Write::SplitSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
    m4return_t            res;
	m4bool_t              hasslices;
    void                * pvalueadd;
    m4date_t ed;
    m4date_t sd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (!hasslices)
            return M4_ERROR;
    }
    else
        return M4_ERROR;

    ai_pClVal->GetpKernelRW ()->GetStartDate (ai_pClVal->m_oContext, ai_pClVal->Index, sd, &pvalueadd);
    ai_pClVal->GetpKernelRW ()->GetEndDate (ai_pClVal->m_oContext, ai_pClVal->Index, ed, &pvalueadd);

    if (ai_date > sd && ai_date < ed) {
        m4VariantType val1, val2;
		priority_t prior;

		m4date_t unitdate;
		ai_pClVal->m_oContext.GetUnitDate (unitdate);

        // inserto un nuevo slice
        ai_pClVal->GetpKernelRW ()->Insert (ai_pClVal->m_oContext,
            ai_pClVal->Index + 1, 1, &pvalueadd);
		// Se actualiza a cero!!!!
		ai_pClVal->GetpKernelRW ()->SetStartDate (ai_pClVal->m_oContext,
            ai_pClVal->Index + 1, ed + unitdate, &pvalueadd);

        // calculo los nuevos valores prorateados:

        ai_pClVal->_Get_OneSlice (sd, ai_date, val1, ai_pClVal->Index);
        ai_pClVal->_Get_OneSlice (ai_date + unitdate, ed, val2, ai_pClVal->Index);

		prior = ai_pClVal->GetPriority();

        // asigno los nuevos valores:

		m4char_t *buff1, *buff2;
		m4uint32_t realsize;
		res = ai_pClVal->GetpKernelRW ()->Size (ai_pClVal->m_oContext,
            ai_pClVal->Index, realsize, &pvalueadd);
		m4uint8_t cpp = ai_pClVal->m_oContext.pItemDef_Interface->CppType ();

		if (cpp == M4CL_CPP_TYPE_STRING) {
			if (val1.Type != M4CL_CPP_TYPE_NULL) {
				buff1 = new m4char_t[realsize];
				memcpy ( buff1, val1.Data.PointerChar, realsize);
				val1.Data.PointerChar = buff1;
			}

			if (val2.Type != M4CL_CPP_TYPE_NULL) {
				buff2 = new m4char_t[realsize];
				memcpy ( buff2, val2.Data.PointerChar, realsize);
				val2.Data.PointerChar = buff2;
			}
		}

		m4bool_t done;

        Set (ai_pClVal, val1, prior, done, M4_TRUE);
		ai_pClVal->Index ++;
        Set (ai_pClVal, val2, prior, done, M4_TRUE);

        // cojo la nueva direccion, puedo haber cambiado despues de las asignaciones.
        // asigno la fecha de inicio del nuevo tramo
        ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        ai_pClVal->GetpKernelRW ()->SetStartDate (ai_pClVal->m_oContext, ai_pClVal->Index,
            ai_date + unitdate, &pvalueadd);

        ai_pClVal->Index --;

		if (cpp == M4CL_CPP_TYPE_STRING) {
			delete (val1.Data.PointerChar);
			delete (val2.Data.PointerChar);
		}

        res = M4_SUCCESS;
    }
    else {
        if (ai_date == sd || ai_date == ed) {
            // el tio se ha liado
            // no ha lugar al split porque la fecha de corte coincide con la de inicio o de fin del tramo
            res = M4_SUCCESS;
        }
        else {
            // nos está tangando, la fecha no está comprendida en el tramo actual
            res = M4_ERROR;
        }
    }

    if (res != M4_ERROR) {
        ai_pClVal->_Notify ();
    }

    return res;
}

// AddSlice en condiciones, que habre hueco, cierra tramos, etc:
// TC: Totalmente Cañera

m4return_t ClValue_Write::AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate, m4date_t ai_enddate)
{
    m4return_t		res = M4_SUCCESS;
	m4bool_t		bIsDeleted = M4_FALSE ;
    m4uint32_t		ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle() ;
    ClCompiledMCR	*pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR() ;


    if (ai_startdate > ai_enddate)
    {
        DUMP_CHLOG_WARNINGF(M4_CH_DM_ADDSLICES_DATESINVALID, DateLog(ai_startdate) <<
            DateLog(ai_enddate) << M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }

	/* Bug 0080945
	Sólo para items de registro
	*/
	if( pmcr->GetItemScope( ItemHandle ) == M4CL_ITEM_SCOPE_REGISTER )
	{
		/* Bug 0080778
		No se pueden crear tramos en registros borrados
		*/
		res = ai_pClVal->GetpRegister()->IsDeleted( bIsDeleted ) ;

		if( res != M4_SUCCESS || bIsDeleted == M4_TRUE )
		{
			DUMP_CHLOG_ERRORF( M4_CH_DM_REGISTERISDELETED, ai_pClVal->GetpRegister()->Index << M4ObjNodeItemI( pmcr, ItemHandle ) ) ;
			return M4_ERROR;
		}
	}


	//Bug:0067379     Begin  ----------------------------------------
	//GP Cuando se intenta asignar un valor en un item con tipo de dato nulo
	m4uint8_t itype;
	ai_pClVal->m_oContext.pItemDef_Interface->CppType(itype);
	if(itype == M4CL_CPP_TYPE_NULL) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_SET_ITEM_NULL, M4ObjNodeItemI(pmcr, ItemHandle));
		return M4_ERROR;
	}
	//Bug:0067379     End    ----------------------------------------


    m4uint32_t auxIndex;
    res = ai_pClVal->_AddSlices(auxIndex, ai_startdate, ai_enddate);

    if (res == M4_SUCCESS)
		ai_pClVal->Index = auxIndex;
	else
		ai_pClVal->MoveToEOF();

    if (res != M4_ERROR) {
        ai_pClVal->_Notify ();
    }

    return res;
}

//      operación de escritura
m4return_t ClValue_Write::DeleteSlice (_ClValue_Read_Slices * ai_pClVal)
{
    m4return_t res;
    m4uint32_t i = ai_pClVal->Index;
	m4bool_t hasslices;
    void * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
        	res = ai_pClVal->GetpKernelRW ()->Delete (ai_pClVal->m_oContext, i, &pvalueadd);
        else
            return M4_SUCCESS;

        ai_pClVal->Index = i;
        ai_pClVal->_Notify ();
    }

    return res;
}

//      operación de escritura
m4return_t ClValue_Write::DeleteAllSlices (_ClValue_Read_Slices * ai_pClVal)
{
    m4return_t res;
	m4bool_t hasslices;
    void * pvalueadd;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
        	res = ai_pClVal->GetpKernelRW ()->DeleteAll (ai_pClVal->m_oContext, &pvalueadd);
        else
            return M4_SUCCESS;

        ai_pClVal->Index = M4DM_EOF_INDEX;
        ai_pClVal->_Notify ();
    }

    return res;
}

m4return_t ClValue_Write::Compact (_ClValue_Read_Slices * ai_pClVal)
{
    m4return_t res;

    M4_ASSERT (ai_pClVal->m_oContext.pItemDef_Interface);

    switch (ai_pClVal->m_oContext.pItemDef_Interface->SliceBhrv ()) {
        case M4CL_ITEM_SLICE_BHRV_BASE_VALUE:
            // se comprueba directamente que sean iguales
            res = ai_pClVal->_Compact_BaseValue (ai_pClVal);
            break;

        case M4CL_ITEM_SLICE_BHRV_FINAL_VALUE:
        case M4CL_ITEM_SLICE_BHRV_UNIT:
            if (ai_pClVal->m_oContext.pItemDef_Interface->CppType () == M4CL_CPP_TYPE_NUMBER)
			{
                res = ai_pClVal->_Compact_Ratios (ai_pClVal);
            }
            else {
                res = ai_pClVal->_Compact_BaseValue (ai_pClVal);
				// para cadenas, nos comportamos como value_base
            }
            break;

        default:
            res = M4_ERROR;
    }

    ai_pClVal->Index = 0;

    if (res != M4_ERROR) {
        ai_pClVal->_Notify ();
    }

    return res;
}

//----------------------------------------------------------------------------
// función a extinguir:
// De momento suponemos que los Add vienen ordenados por fecha
//      operación de escritura
//----------------------------------------------------------------------------

m4return_t ClValue_Write::AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate)
{
    m4return_t res;

    m4date_t enddate;
    ai_pClVal->m_oContext.GetEndDate (enddate);

    res = AddSlice (ai_pClVal, ai_startdate, enddate);

    if (res == M4_SUCCESS) {
        ai_pClVal->_Notify ();
    }

    return res;
}

//----------------------------------------------------------------------------
// función a extinguir:
//----------------------------------------------------------------------------

m4return_t ClValue_Write::AllocSlices (_ClValue_Read_Slices * ai_pClVal, m4uint32_t ai_islices)
{
    m4return_t            res;
	m4bool_t              hasslices;
    void                * pvalueadd;
    m4uint32_t            n = 0;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
        if (hasslices)
            res = ai_pClVal->GetpKernelRW ()->Count (ai_pClVal->m_oContext, n, &pvalueadd);

        if (ai_islices > n)
        {
            res = ai_pClVal->GetpKernelRW ()->Insert (ai_pClVal->m_oContext, n, ai_islices - n, &pvalueadd);
            if (res != M4_SUCCESS)
                return M4_ERROR;
            ai_pClVal->_Notify ();
        }
    }

    return res;
}

//----------------------------------------------------------------------------
// AllocSlices optimizado
//----------------------------------------------------------------------------
m4return_t ClValue_Write::AllocSlices (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack)
{
    m4return_t res;

    res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
    if (res == M4_SUCCESS)
    {
        ai_pClVal->GetpKernelRW ()->AllocSlices (ai_pClVal->m_oContext, ai_stack);
        ai_pClVal->_Notify ();
    }

    return res;
}

m4return_t ClValue_Write::CreateTempFile(_ClValue_Read_Slices * ai_pClVal, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
{
    if (ai_pClVal->m_oContext.pItemDef_Interface->M4Type () == M4CL_M4_TYPE_BLOB)
    {
	    m4bool_t hasslices;
        void * pvalueadd;
        m4return_t res;

        res = ai_pClVal->m_oContext.pMemory_Interface->Validation(ai_pClVal->m_oContext, M4_CH_DM_ITEM_WRITE_OPERATION);
        if (res == M4_SUCCESS)
        {
            res = ai_pClVal->m_oContext.pMemory_Interface->GetAddress_andhasslices(ai_pClVal->m_oContext, &pvalueadd, hasslices);
//            if (hasslices)
//        	    return ai_pClVal->GetpKernelRW ()->CreateTempFile(ai_pClVal->m_oContext, ai_pClVal->Index, ai_pDirectory, ai_pExtension);
//            else
                return ai_pClVal->m_oContext.pMemory_Interface->CreateTempFile(ai_pClVal->m_oContext, ai_pDirectory, ai_pExtension);
        }
    }

    return M4_ERROR;
}

/*****************************************************/
// cLASE NULA

m4return_t ClValue_Write_Nihil::Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, m4bool_t ai_bChangeStatus)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE_SETVALUE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_SETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetDescr(_ClValue_Read_Slices * ai_pClVal, m4pcchar_t ai_pcDescr)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE_SETVALUE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_SETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::Set (_ClValue_Read_Slices * ai_pClVal, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE_SETVALUE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE_SETVALUE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetNULL (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetDefault (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetStartDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetEndDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SplitSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate, m4date_t ai_enddate)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::DeleteSlice (_ClValue_Read_Slices * ai_pClVal)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::DeleteAllSlices (_ClValue_Read_Slices * ai_pClVal)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::AllocSlices (_ClValue_Read_Slices * ai_pClVal, m4uint32_t ai_islices)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::AllocSlices (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::Compact (_ClValue_Read_Slices * ai_pClVal)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

// a extinguir:
m4return_t ClValue_Write_Nihil::AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::SetPriority (_ClValue_Read_Slices * ai_pClVal, priority_t ai_prior)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr = ai_pClVal->m_oContext.pItemDef_Interface->GetpCMCR ();

	if (ItemHandle != 0)
	{
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_ITEM_WRITE, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_SUCCESS;
	}
	else
	{
		m4uint32_t NodeHandle = ai_pClVal->m_oContext.pNode_Data->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_WARNING;
	}
}

m4return_t ClValue_Write_Nihil::CopyItem (_ClValue_Read_Slices * ai_pClVal, _ClValue_Read_Slices * ai_pClValRead)
{
    m4uint32_t ItemHandle = ai_pClVal->m_oContext.pItemDef_Interface->GetHandle();

    // en este caso no vuelco mensaje, no es una funcio que se vea desde fuera

	if (ItemHandle != 0)
        return M4_SUCCESS;

    else
        return M4_WARNING;
}
