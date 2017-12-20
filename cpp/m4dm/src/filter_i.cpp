#include "dmres.hpp"


#include "filter_i.hpp"

#include "regi_smp.hpp"
#include "clrgstrs.h"
#include "reg_filt.hpp"
#include "reg_plf.hpp"
#include "reg_load.hpp"

#include "clfuvar.hpp"
#include "aregister.hpp"
#include "channel.hpp"
#include "px_block.hpp"
#include "mmindex.hpp"

#include "node_knl.hpp"
#include "m4mdrt.hpp"


static ClRegisterPartialLoad        s_ClRegisterPartialLoad;
static ClRegisterFilter_Simple      s_ClRegisterFilter_Simple;
static ClRegisterFilter_AutoLoad    s_ClRegisterFilter_AutoLoad;
    //^(si las ponemos como miembros, hay que hacer includes extra en el .hpp)

ClFilterInterface::ClFilterInterface ()
{
    m_pClFilter = &s_ClRegisterFilter_Simple;   //inicialmente

    m_pClRegister = NULL;
	m_poChangeListener = NULL;
}

void ClFilterInterface::Attach (_ClRegister_Base *ai_pregister)
{
    m_pClRegister = ai_pregister;

    M4_ASSERT (m_pClRegister);
}


ClFilterInterface::~ClFilterInterface ()
{
    DeleteAll ();
}


m4return_t ClFilterInterface::AddNew (m4char_t * ai_pForm, FilterType ai_Static, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs, m4pchar_t ai_pName)
{
    _ClRegisterFilter *pAuxFilter = 0;

    if (m_pClRegister->GetpChannel ())
    {
        if (ai_Static == StaticFilter)
		{
            pAuxFilter = new ClRegisterStaFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, ai_pName);
		}
        else
		{
            pAuxFilter = new ClRegisterDynFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, ai_pName);
		}

        M4_ASSERT (pAuxFilter);

        m4return_t res = pAuxFilter->InstallFormula(ai_pForm, m_pClRegister, ai_iNumArgs, ai_pNameArgs, ai_pValueArgs);

        if (res != M4_SUCCESS)
        {
            delete pAuxFilter;
            return M4_ERROR;
        }

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


m4return_t ClFilterInterface::AddNew (m4int32_t ai_pHandleForm, FilterType ai_Static, m4pchar_t ai_pName)
{
    _ClRegisterFilter *pAuxFilter = 0;

    if (m_pClRegister->GetpChannel ())
    {
        if (ai_Static == StaticFilter)
		{
            pAuxFilter = new ClRegisterStaFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_Data ()->GetpExtInfo ()->Index, ai_pHandleForm, ai_pName);
		}
        else
		{
            pAuxFilter = new ClRegisterDynFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_Data ()->GetpExtInfo ()->Index, ai_pHandleForm, ai_pName);
		}

        M4_ASSERT (pAuxFilter);

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


m4return_t ClFilterInterface::AddNew (m4uint8_t ai_OpType, FilterType ai_Static, m4pchar_t ai_pName)
{
    _ClRegisterFilter *pAuxFilter = 0;

    if (ai_Static == StaticFilter)
	{
        pAuxFilter = new ClRegisterStaFilter (ai_OpType, ai_pName);
	}
    else
	{
        pAuxFilter = new ClRegisterDynFilter (ai_OpType, ai_pName);
	}

    M4_ASSERT (pAuxFilter);

    return Add (pAuxFilter);
}


m4return_t ClFilterInterface::AddFastFilter (m4pchar_t ai_pItemName, m4VariantType ai_Value, FilterType ai_Static, m4pchar_t ai_pName)
{
    _ClRegisterFilter *pAuxFilter = 0;

    if (m_pClRegister->GetpChannel ())
    {
        if (!m_pClRegister->Item.MoveTo(ai_pItemName))
		{
            return M4_ERROR;
		}

        ClItemIndex index = m_pClRegister->Item.GetIndex();

        if (ai_Static == StaticFilter)
		{
            pAuxFilter = new ClRegisterStaFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, index, ai_Value, ai_pName);
		}
        else
		{
            pAuxFilter = new ClRegisterDynFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, index, ai_Value, ai_pName);
		}

        M4_ASSERT (pAuxFilter);

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


m4return_t ClFilterInterface::AddFastFilter (ClItemIndex ai_idxItem, m4VariantType ai_Value, m4uint32_t ai_iOperation/*=M4CL_CASE_EQUAL_FUNCTION*/ , FilterType ai_Static /*= StaticFilter*/, m4pchar_t ai_pName /*= 0*/)
{
	
    _ClRegisterFilter *pAuxFilter = 0;

    if (m_pClRegister->GetpChannel ())
    {
        if (!m_pClRegister->Item.MoveTo(ai_idxItem))
		{
            return M4_ERROR;
		}

        ClItemIndex index = m_pClRegister->Item.GetIndex();

        if (ai_Static == StaticFilter)
		{
            pAuxFilter = new ClRegisterStaFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, index, ai_Value, ai_iOperation, ai_pName);
		}
        else
		{
            pAuxFilter = new ClRegisterDynFilter (m_pClRegister->GetpChannel (), m_pClRegister->GetpNode_ExtInfo ()->Index, index, ai_Value, ai_iOperation, ai_pName);
		}

        M4_ASSERT (pAuxFilter);

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


//borra el último filtro que se metio, que es el primero de la lista:

m4return_t ClFilterInterface::DeleteLast (void)
{
    M4_ASSERT (m_pClFilter);

    _ClRegisterFilter * AuxFilter;
    AuxFilter = m_pClFilter->SubFilter();
	if (AuxFilter)
    { // Si es nulo estamos en el filtro simple
		delete m_pClFilter;

        AuxFilter->AttachFather(0); //ahora lo que era el padre es nulo
        AuxFilter->AttachInterface(this);

        m_pClFilter = AuxFilter;
        
        ClRegisterIndex auxIndex;
		m_pClFilter->UpdateIndex(auxIndex, m_pClRegister->GetMMIndex (), m_pClRegister->GetCurrentBlockRef (), m_pClRegister);
		m_pClRegister->MoveTo(auxIndex);

		if( m_poChangeListener != NULL )
		{
			m_poChangeListener->SetChange() ;
		}

    	return M4_SUCCESS;
	}
    else
        return M4_ERROR;
}

m4return_t ClFilterInterface::DeleteAll (void)
{
    _ClRegisterFilter * AuxFilter;

	if( m_pClFilter->IsTerminator() == M4_FALSE )
	{
		if( m_poChangeListener != NULL )
		{
			m_poChangeListener->SetChange() ;
		}
	}

    AuxFilter = m_pClFilter->Destroy();
    m_pClFilter = AuxFilter;

    return M4_SUCCESS;
}

m4return_t ClFilterInterface::Init (m4bool_t ai_ispartialload, m4uint8_t ai_autoloadmode)
{
    if (m_pClFilter->IsTerminator ())
    {
        if (ai_ispartialload)
            m_pClFilter = &s_ClRegisterPartialLoad;
        else {
            m_pClFilter = _GetAutoLoadTerminator (ai_autoloadmode);
        }
    }

    return M4_SUCCESS;
}

m4return_t ClFilterInterface::SetAutoLoadMode (m4uint8_t ai_mode)
{
    m4return_t res;

    if (ai_mode == M4CL_AUTOLOAD_OFF || 
        ai_mode == M4CL_AUTOLOAD_BLOCK || 
        ai_mode == M4CL_AUTOLOAD_PRG) 
    {

        //localizo el filtro anterior al terminador
        M4_ASSERT (m_pClFilter);
        _ClRegisterFilter * AuxFilter1 = m_pClFilter;
        _ClRegisterFilter * AuxFilter2 = 0;

        for (;;) {
            if (AuxFilter1->IsTerminator ()) {
                break;
            }
            AuxFilter2 = AuxFilter1;
            AuxFilter1 = AuxFilter1->SubFilter();
	    }

        //ahora AuxFilter1 es un terminador y AuxFilter2 es el filtro anterior al terminador (puede ser 0)
        M4_ASSERT (AuxFilter1);
        M4_ASSERT (AuxFilter1->IsTerminator ());
        #ifdef _DEBUG
        if (AuxFilter2) //puede ser cero
            M4_ASSERT (AuxFilter2->SubFilter () == AuxFilter1);
        #endif

        //una comprobación más: si el terminador es de carga parcial, no hacemos nada
        //el tema de autoload no afecta
        if (AuxFilter1->Type () != _ClRegisterFilter::PartialLoad) {
            //sólo si no es de carga parcial:
            _ClRegisterFilter *pf = _GetAutoLoadTerminator (ai_mode);

            if (AuxFilter2)
                AuxFilter2->AttachSubFilter (pf);
            else
                m_pClFilter = pf;
        }

        res = M4_SUCCESS;
    }
    else {
        //error
        if (m_pClRegister)
        {
			m4uint32_t iNodeHandle = m_pClRegister->GetpNode_Data()->GetpNodeDef()->GetHandle();
			ClCompiledMCR * pmcr  = m_pClRegister->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
			DUMP_CHLOG_ERRORF(M4_CH_DM_INVALIDAUTOLOADMODE_NODE, ai_mode << M4ObjNodeN(pmcr, iNodeHandle));
        }
        else
		{
            DUMP_CHLOG_ERRORF(M4_CH_DM_INVALIDAUTOLOADMODE, ai_mode);
		}
        res = M4_ERROR;
    }
            
    return res;
}

_ClRegisterFilter *ClFilterInterface::_GetAutoLoadTerminator (m4uint8_t ai_mode)
{
    if (ai_mode == M4CL_AUTOLOAD_OFF) {
        return &s_ClRegisterFilter_Simple;
    }
    else {
        return &s_ClRegisterFilter_AutoLoad;
    }
}

//función muy tonta para comprobaciones esquizofrénicas:

m4bool_t ClFilterInterface::IsInitialized (void)
{
    return (m_pClFilter)? M4_TRUE : M4_FALSE;
}

m4return_t ClFilterInterface::Add (_ClRegisterFilter * ai_pAuxFilter)
{
	// Bug 0305188 Control de número de filtros
	if( Count() >= 100 )
	{
		m4uint32_t		iNodeHandle = m_pClRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;
		ClCompiledMCR	*poCMCR = m_pClRegister->GetpNode_Data()->GetpNodeDef()->GetpCMCR() ;
		DUMP_CHLOG_ERRORF( M4_CH_DM_TOO_MANY_FILTERS, 100 << M4ObjNodeN( poCMCR, iNodeHandle ) ) ;
        return M4_ERROR;
	}

    //Me veo obligado a hacer esto para poder mantener la recurrencia
    ai_pAuxFilter->AttachSubFilter(m_pClFilter);
    m4return_t res = ai_pAuxFilter->UpdateThisFilter (m_pClRegister->GetCurrentBlockRef (), m_pClRegister);

    if (res == M4_SUCCESS)
    {
        m_pClFilter->AttachFather(ai_pAuxFilter);
        m_pClFilter = ai_pAuxFilter;
        m_pClFilter->AttachInterface(this);

        m_pClRegister->MoveToEOF(); //por ahora

		if( m_poChangeListener != NULL )
		{
			m_poChangeListener->SetChange() ;
		}

	    return M4_SUCCESS;
    }
    else
    {
        delete ai_pAuxFilter;
        return M4_ERROR;
    }
}

m4return_t ClFilterInterface::CopyFilter (const ClFilterInterface &ai_fi, m4bool_t ai_bTotal)
{
    m_pClFilter = ai_fi->Copy(this, ai_bTotal); //operador -> sobrecargado
    m_pClFilter->AttachRegister(m_pClRegister);

	if( m_poChangeListener != NULL )
	{
		m_poChangeListener->SetChange() ;
	}

    return M4_SUCCESS;
}

m4return_t ClFilterInterface::UpdateOneIndex(void)
{
    ClMMIndex aux_mmindex;
    aux_mmindex = m_pClRegister->GetMMIndex();
    return m_pClFilter->UpdateOneIndex(aux_mmindex, m_pClRegister->GetCurrentBlockRef (), m_pClRegister);
}

m4return_t ClFilterInterface::UpdateFilter(void)
{
	if( m_pClFilter->IsTerminator() == M4_FALSE )
	{
		if( m_poChangeListener != NULL )
		{
			m_poChangeListener->SetChange() ;
		}
	}
    return m_pClFilter->UpdateFilter(m_pClRegister->GetCurrentBlockRef (), m_pClRegister);
}

m4int16_t ClFilterInterface::Count (void)
{
    _ClRegisterFilter *pAuxFilter = m_pClFilter;
    m4int16_t count = 0;

    while (pAuxFilter->SubFilter())
    {
        pAuxFilter = pAuxFilter->SubFilter();
        count ++;
    }

    return count;
}

_ClRegisterFilter & ClFilterInterface::operator [] (m4int16_t ai_iFilter)
{
    _ClRegisterFilter *pAuxFilter = m_pClFilter;

    m4int32_t count = Count ();
    if (count)
    {
        count --; // La numeracion del primer filtro es 0 y asi suces....
        while (pAuxFilter->SubFilter())
        {
            if (count == ai_iFilter)
                break;
            pAuxFilter = pAuxFilter->SubFilter();
            count --;
        }
    }

    return *pAuxFilter;
}

_ClRegisterFilter & ClFilterInterface::operator [] (m4pchar_t ai_pName)
{
    _ClRegisterFilter *pAuxFilter = m_pClFilter;
    m4VariantType v;
    m4return_t res;

    m4int32_t count = Count ();
    if (count)
    {
        while (pAuxFilter->SubFilter())
        {
            res = pAuxFilter->GetName(v);
            if (res == M4_SUCCESS)
            {
                if (!(strcmp(v.Data.PointerChar, ai_pName)))
                    break;
            }
            pAuxFilter = pAuxFilter->SubFilter();
        }
    }

    return *pAuxFilter;
}

ClBSearchIndex * ClFilterInterface::GetSearchIndexAndSorted (m4bool_t & ao_bIsSorted)
{
    ao_bIsSorted = M4_FALSE;
    m4int16_t count = Count ();
    m4int16_t i;
    ClBSearchIndex * auxpsi = 0;
    for (i = (count - 1); i >= 0 ; i --)
    {
        auxpsi = (*this)[i]->GetSearchIndex();
        if (auxpsi)
        {
            ao_bIsSorted = (*this)[i]->IsSorted();
            break;
        }
    }

    return auxpsi;
}

void ClFilterInterface::AttachFilterCollection(_ClRegisterFilter * ai_pClFilter)
{
    if (ai_pClFilter)
        m_pClFilter = ai_pClFilter;
}

void ClFilterInterface::DestroyChannel_Before (ClChannel * ai_poCh)
{
    m4int16_t count = Count ();
    m4int16_t i;

    for (i = (count - 1); i >= 0 ; i --)
    {
        if ((*this)[i]->GetpChannel() == ai_poCh)
        {
            DeleteAll();
            return;
        }
    }
}    


m4return_t 
ClFilterInterface::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFilterInterface);

    if (m_pClFilter->GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}


