//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             iteminfo.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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
//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "iteminfo.hpp"
#include "item_prx.hpp"
#include "itemhash.hpp"
#include "colitdef2.hpp"


//## end module.includes preserve=yes


////////////////////////////////////////////////////////////////////////////////
//
// ClRTItemIterator
//


ClRTItemIterator::ClRTItemIterator () : _ClSmartPointer ()
{
    m_iCount = 0;
    m_iScope = M4DM_SCOPE_NONE;
    m_iType = M4DM_TYPE_NONE;

    m_ppItemProxy = 0;
    m_pHash = 0;
}

ClRTItemIterator::~ClRTItemIterator ()
{
    _DestroyPool ();
}

ClRTItemIterator &ClRTItemIterator::operator = (const ClRTItemIterator &ai_ClRTItemIterator)
{
    //no hace nada
    return *this;
}

m4return_t ClRTItemIterator::_DestroyPool (void)
{
    if (m_ppItemProxy) {
        delete []m_ppItemProxy;
        m_ppItemProxy = 0;

        m_iCount = 0;
    }

    if (m_pHash) {
        delete m_pHash;
        m_pHash = 0;
    }

    return M4_SUCCESS;
}

m4return_t ClRTItemIterator::_AllocPool (m4uint16_t ai_n)
{
    _DestroyPool ();

    m_ppItemProxy = new _AClItemProxy *[ai_n];

    return M4_SUCCESS;
}

void ClRTItemIterator::Attach (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pcolitemdef)
{
    m_itemdefit.Attach (ai_pcmcr, ai_pcolitemdef);
}

m4bool_t ClRTItemIterator::_test (m4uint8_t ai_type, m4uint8_t ai_scope)
{
    // ¿pasa o no pasa?
    m4uint8_t bok = 0;

    // test de type:        
    switch (ai_type) {
    case M4CL_ITEM_TYPE_METHOD:
        bok = (m_iType & M4DM_TYPE_METHOD);
        break;
    case M4CL_ITEM_TYPE_PROPERTY:
        bok = (m_iType & M4DM_TYPE_PROPERTY);
        break;
    case M4CL_ITEM_TYPE_FIELD:
        bok = (m_iType & M4DM_TYPE_FIELD);
        break;
    case M4CL_ITEM_TYPE_CONCEPT:
        bok = (m_iType & M4DM_TYPE_CONCEPT);
        break;
    }

    if (bok) {

        bok = 0;

        // test de scope:
        switch (ai_scope) {
        case M4CL_ITEM_SCOPE_NODE:
            bok = (m_iScope & M4DM_SCOPE_NODE);
            break;
        case M4CL_ITEM_SCOPE_BLOCK:
            bok = (m_iScope & M4DM_SCOPE_BLOCK);
            break;
        case M4CL_ITEM_SCOPE_REGISTER:
            bok = (m_iScope & M4DM_SCOPE_REGISTER);
            break;
        }
    }

    return bok? M4_TRUE : M4_FALSE;
}

m4return_t ClRTItemIterator::SetModeMandatory (m4bool_t ai_bIsCacheable, m4uint8_t ai_newscope, m4uint8_t ai_newtype, m4uint16_t ai_count)
{
    m_iScope = ai_newscope;
    m_iType = ai_newtype;

//    _DestroyPool ();

    m4uint16_t i;
    //m4uint16_t n;
    

    /// esto es provisional:
    if (ai_count == 0xffff) {
        ai_count = _CalcCount (ai_newscope, ai_newtype);
    }

    _AllocPool (ai_count);    
    i = 0;
    //n = ai_onodedef.NumberOfItems ();
    
    m_itemdefit.Begin ();

    #ifdef _DEBUG
        m4uint32_t lasthandle = 0;
    #endif

    while ( m_iCount < ai_count && /*i < n*/ m_itemdefit.Index != M4DM_EOF_INDEX) {
        //m_auxitemdef = ai_onodedef.ColItemDef [(ClItemIndex)i];

        // ¿pasa o no pasa?
        if ( _test (m_itemdefit.ItemDef.Type (), m_itemdefit.ItemDef.Scope ()) ) {
            // pasó la prueba!!!
            m_ppItemProxy [m_iCount] = m_itemdefit.GetpItemProxy ();            

            //en las búsquedas por handle tenemos como hipotesis que están
            //ordenados de menor a mayor:
            M4_ASSERT (lasthandle < m_ppItemProxy [m_iCount]->GetHandle ());
            #ifdef _DEBUG
                lasthandle = m_ppItemProxy [m_iCount]->GetHandle ();
            #endif

            m_iCount++;
        }

        i++;
        m_itemdefit.Next ();
    }    

	if( ai_bIsCacheable == M4_TRUE )
	{
		if (0 == m_pHash) {
			m_pHash = new ClItemHash (m_itemdefit, m_ppItemProxy, m_iCount);
		}
	}

    return M4_SUCCESS;
}

m4uint16_t ClRTItemIterator::_CalcCount (m4uint8_t ai_newscope, m4uint8_t ai_newtype)
{
    m4uint16_t i;
    //m4uint16_t n;

    m4uint16_t iCount = 0;
    
    //n = ai_onodedef.NumberOfItems ();
	i=0; //pipo

    m_itemdefit.Begin ();

    while (/*i < n*/m_itemdefit.Index != M4DM_EOF_INDEX) {
        //m_auxitemdef = ai_onodedef.ColItemDef [(ClItemIndex)i];

        // ¿pasa o no pasa?
        if ( _test (m_itemdefit.ItemDef.Type (), m_itemdefit.ItemDef.Scope ()) ) {
            // pasó la prueba!!!
            iCount++;
        }

        i++;
        m_itemdefit.Next ();
    }

    return iCount;
}

m4return_t ClRTItemIterator::Get (ClItemIndex ai_itemindex, _AClItemProxy * &ao_prtitem)
{
    m4return_t res;

    if (ai_itemindex < m_iCount) {
        ao_prtitem = m_ppItemProxy [ai_itemindex];
        res = M4_SUCCESS;
    }
    else {
        ao_prtitem = 0;
        res = M4_ERROR;
    }

    return res;
}

//static
int ClRTItemIterator::s_compare (const void *ai_elem1, const void *ai_elem2)
{
    return ( (*((const _AClItemProxy **)ai_elem1))->GetHandle () - (*((const _AClItemProxy **)ai_elem2))->GetHandle ());
}

m4return_t ClRTItemIterator::Get (ClHandle ai_itemhandle, _AClItemProxy * &ao_prtitem, ClItemIndex &ao_itemindex)
{
    ao_prtitem = 0;

    /*
    //cutre: búsqueda secuencial
    for (m4uint32_t i = 0; i < m_iCount; i++) {
        if (m_ppItemProxy [i]->GetHandle () == ai_itemhandle) {
            ao_prtitem = m_ppItemProxy [i];

            ao_itemindex = i;
            return M4_SUCCESS;
        }
    }
    */

    /*
    //vamos a hacerlo así, que va a ser más rápido:
    _ClItemProxy_Nihil truqui_truqui (ai_itemhandle);
    _ClItemProxy_Nihil *ptruqui_truqui = &truqui_truqui;
    
    _AClItemProxy **pres = (_AClItemProxy **)bsearch (&ptruqui_truqui, m_ppItemProxy, m_iCount, sizeof (_AClItemProxy *), s_compare);
    
    if (pres) {
        ao_prtitem = *pres;

        M4_ASSERT (ao_prtitem->GetHandle () == ai_itemhandle);

        ao_itemindex = (pres - m_ppItemProxy);
        M4_ASSERT (m_ppItemProxy [ao_itemindex]->GetHandle () == ai_itemhandle);

        return M4_SUCCESS;
    }
    */

    //busqueda binaria ad-hoc:
        m4uint32_t num = m_iCount;
        m4uint32_t lo = 0;
        m4uint32_t hi = num - 1;
        m4uint32_t mid;
        m4uint32_t half;
        int result;

        while (lo <= hi)
                if (half = (num >> 1))
                {
                        mid = lo + (num & 1 ? half : (half - 1));
                        if (!(result = (ai_itemhandle - m_ppItemProxy [mid]->GetHandle ()) )) {
                                ao_prtitem = m_ppItemProxy [mid];
                                ao_itemindex = mid;
                                break;
                        }
                        else if (result < 0)
                        {
                                hi = mid - 1;
                                num = num & 1 ? half : half-1;
                        }
                        else    {
                                lo = mid + 1;
                                num = half;
                        }
                }
                else if (num) {
                        if ( ai_itemhandle - m_ppItemProxy [lo]->GetHandle () ){
                            //no encontrado!
                            break;
                        }
                        else {
                            ao_prtitem = m_ppItemProxy [lo];
                            ao_itemindex = lo;
                            break;
                        }
                }
                else
                        break;


    return M4_ERROR;
}

m4return_t ClRTItemIterator::Get (m4pcchar_t ai_itemid, _AClItemProxy * &ao_prtitem, ClItemIndex &ao_itemindex, m4bool_t ai_bIsUpper)
{
    ao_prtitem = 0;

// +jcr
//	m4char_t	cLength ;	// las longitudes son de 0 a 30
//	m4pcchar_t	pccItemId ;

//	cLength = m4char_t( strlen( ai_itemid ) ) ;

    if (0 == m_pHash) {
        m_pHash = new ClItemHash (m_itemdefit, m_ppItemProxy, m_iCount);
    }

    m4uint32_t proxyindex = m_pHash->Hfind(m_itemdefit, m_ppItemProxy, ai_itemid, ai_bIsUpper);

    if (proxyindex != M4_HASH_EMPTY)
    {
        ao_prtitem = m_ppItemProxy [proxyindex];
        ao_itemindex = proxyindex;
        return M4_SUCCESS;
    }

/*
// -jcr

    // esto es bastante cutre, hay que hacer una búsqueda binaria:
    // ojo con  strcmpi que no es estandar
    for (m4uint32_t i = 0; i < m_iCount; i++) {
        // aquí es donde utilizamos el m_itemdefit

        //para poder llamar a una función no const de ItemDef, utilizo
        //GetpItemDef:
        m_itemdefit.ItemDef.SetHandle (m_ppItemProxy [i]->GetHandle ());

// +jcr
// el truco del almendruco. Tengo guardada la longitud delante de la cadena

		pccItemId = m_itemdefit.ItemDef.Id() ;

		if( *( pccItemId - 1 ) == cLength )
		{
		    if (strcmpi (pccItemId, ai_itemid) == 0) {
				ao_prtitem = m_ppItemProxy [i];

				ao_itemindex = i;
				return M4_SUCCESS;
			}
        }
    }
// -jcr
*/
    return M4_ERROR;
}
m4return_t 
ClRTItemIterator::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClRTItemIterator);

    ao_size-=sizeof(_ClSmartPointer);
	if (_ClSmartPointer::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
    ao_size-=sizeof(m_itemdefit);
	if (m_itemdefit.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_ppItemProxy){
		// Som punteros. El objeto ItemProxy se cuenta en ColItDefKernel
	    ao_size+=sizeof(*m_ppItemProxy)*m_iCount;
	}

	if (m_pHash){
		if (m_pHash->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;

}



