//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             aregiste2.cpp
// Project:          CVM-DM
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

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "aregister.hpp"
#include "recorset.hpp"

#include "clbsearch.hpp"
#include "px_block.hpp"
#include "compst_common.hpp"

#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "not_cdat.hpp"

#include "regi_smp.hpp"
#include "mmarray.hpp"

#include "clrgstrs.h"

#include "blockref.hpp"
#include "chlog.hpp"


//----------------------------------------------------------------------------
// Compare
//  función maestra de comparación
//----------------------------------------------------------------------------

// el estado de los registros se conserva

m4int8_t _ClRegister_Base::Compare (const _ClRegister_Base &ai_reg) const
{
    m4int8_t icomp = 0;  //signed!

    //pido al recordset info del indice activo:
    M4_ASSERT (m_pClNode_Data);
    ClBSearchIndex *psi = m_pClNode_Data->GetpSearchIndex ();
    M4_ASSERT (psi);
    m4uint32_t i,n;
    n = psi->Count ();

    m4VariantType val1, val2;
    ClBSItemIndex *psii;

    ClItem itemaux1 = Item;
    ClItem itemaux2 = ai_reg.Item;

    for (i = 0; i < n; i++) {
        psii = psi->Get (i);

        itemaux1 [psii->GetItemHandle ()].Value.Get(val1);
        itemaux2 [psii->GetItemHandle ()].Value.Get(val2);

        icomp = psii->VarCompare (val1, val2);
        if (icomp) {
            break;
        }
    }

    return icomp;
}

/**************************************************************************************/
// Nueva funcion para la comparacion de inidices en lugar de registros. Espero 
// acordarme de pasarla al register!!!!!!!!
/**************************************************************************************/

m4int8_t _compare_index_with_order(_ClRegister_Base &ai_reg, m4uint32_t index1, m4uint32_t index2)
{
    m4int8_t icomp = 0;  //signed!

    //pido al recordset info del indice activo:
    M4_ASSERT (ai_reg.GetpNode_Data());

    ClBSearchIndex *psi = ai_reg.GetpNode_Data()->GetpSearchIndex();

    m4uint32_t i,n;
    n = psi->Count ();

    m4VariantType val1, val2;
    ClBSItemIndex *psii;

    m4uint32_t index = ai_reg.Index;

    for (i = 0; i < n; i++) {
        psii = psi->Get (i);

        ai_reg.MoveTo(index1);

        if (ai_reg.Item [psii->GetItemHandle ()].Value.HasSlices())
            ai_reg.Item.Value.GetpSliceFooter().Get(val1);
        else
            ai_reg.Item [psii->GetItemHandle ()].Value.Get(val1);

        ai_reg.MoveTo(index2);

        if (ai_reg.Item [psii->GetItemHandle ()].Value.HasSlices())
            ai_reg.Item.Value.GetpSliceFooter().Get(val2);
        else
            ai_reg.Item [psii->GetItemHandle ()].Value.Get(val2);

        icomp = psii->VarCompare (val1, val2);
        if (icomp) {
            if (psii->GetOrder () < 0) {
                icomp = -icomp;
            }
            break;
        }
    }

    ai_reg.MoveTo(index); // No se si es necesario, por ahora lo dejo asi.

    return icomp;
}

//-----------------------------------------------------------------------------
// hacemos aquí un compare especial para la función sort, que tiene
// en cuenta el orden ascendente o descendente de la columna.
// además es un poco más rápida porque no crea item temporales, por contra
// modifica el estado del Item a la salida.
//-----------------------------------------------------------------------------

m4int8_t _ClRegister_Base::_compare_with_order (_ClRegister_Base &ai_reg)
{
    m4int8_t icomp = 0;  //signed!

    //pido al recordset info del indice activo:
    M4_ASSERT (m_pClNode_Data);
    ClBSearchIndex *psi = m_pClNode_Data->GetpSearchIndex ();
    M4_ASSERT (psi);

    m4uint32_t i,n;
    n = psi->Count ();

    m4VariantType val1, val2;
    ClBSItemIndex *psii;

    for (i = 0; i < n; i++) {
        psii = psi->Get (i);

        Item [psii->GetItemHandle ()].Value.Get(val1);
        ai_reg.Item [psii->GetItemHandle ()].Value.Get(val2);

        icomp = psii->VarCompare (val1, val2);
        if (icomp) {
            if (psii->GetOrder () < 0) {
                icomp = -icomp;
            }
            break;
        }
    }

    return icomp;
}

//-----------------------------------------------------------------------------
// Operadores de comparación (todos!), compara los dos registros, determina cual es el menor según el
// indice de ordenación actualmente activo en el recordset.
//-----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::operator < (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) < 0) ? M4_TRUE : M4_FALSE );
}

m4bool_t _ClRegister_Base::operator > (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) > 0) ? M4_TRUE : M4_FALSE );
}

m4bool_t _ClRegister_Base::operator <= (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) <= 0) ? M4_TRUE : M4_FALSE );
}

m4bool_t _ClRegister_Base::operator >= (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) >= 0) ? M4_TRUE : M4_FALSE );
}

m4bool_t _ClRegister_Base::operator == (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) == 0) ? M4_TRUE : M4_FALSE );
}

m4bool_t _ClRegister_Base::operator != (const _ClRegister_Base &ai_reg) const
{
    return ( (Compare (ai_reg) != 0) ? M4_TRUE : M4_FALSE );
}

//-----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::SetCurrentSearchIndex (ClBSearchIndex *ai_psi)
{
    m4return_t res;

    /*
    if (ai_psi) {
        M4_ASSERT (GetCurrentBlock ());        
        ClBlockSearchInfo *psi = GetCurrentBlock ()->GetpSearchInfo ();
        if (psi) {
            if (GetCurrentSearchIndex () != ai_psi) {
                psi->SetpSearchIndex (ai_psi);

                //si el indice es diferente al actual reordenamos:
                res = Arrange ();
            }
            else {
                if (! psi->IsSorted ()) {
                    // sólo si no está ordenado
                    res = Arrange ();
                }
                else {
                    res = M4_SUCCESS;
                }
            }
        }
        else {
            //psi puede ser cero cuando el bloque sobre el que estamos es nihil
            res = M4_ERROR;
        }
    }
    else {
        res = M4_ERROR;
    }
    */

    M4_ASSERT (m_pClNode_Data);
    
    if (ai_psi) {
        m_pClNode_Data->SetpSearchIndex (ai_psi);
        res = M4_SUCCESS;
    }
    else {
        //mal !!
        res = M4_ERROR;
    }

    return res;
}

m4return_t _ClRecordSet_Base::SetCurrentSearchIndexByPos (m4uint16_t ai_ipos)
{
    return SetCurrentSearchIndex (m_pClNode_Data->GetpExtInfo ()->GetSearchIndexByPos (ai_ipos));
}

m4return_t _ClRecordSet_Base::SetCurrentSearchIndexById  (m4uint32_t ai_id)
{
    return SetCurrentSearchIndex (m_pClNode_Data->GetpExtInfo ()->GetSearchIndexById (ai_id));
}

ClBSearchIndex *_ClRecordSet_Base::GetCurrentSearchIndex (void) const
{
    M4_ASSERT (m_pClNode_Data);
    return m_pClNode_Data->GetpSearchIndex ();
}

//para todos los hijos RB, 
//actualizo el array de bloques para que quede coherente:

void ClBlock_Channel_Proxy::_UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n)
{
    //los assert hístericos:
    M4_ASSERT (m_pClNode_Data);    
    M4_ASSERT (ai_n);   //si ai_n = 0, ¿qué xxxx hacemos aquí?
    M4_ASSERT (ai_pindex_qtc);

    m4uint16_t i, nchildren;
    ClNode_Data *pchildnode;

    nchildren = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();
    
    for (i = 0; i < nchildren; i++) {
        pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)i].GetAddress ();
        M4_ASSERT (pchildnode->GetpBlockStore ());

        pchildnode->GetpBlockStore ()->UpdateIndex (ID.BlockUniqueKey, ai_pindex_qtc, ai_n);
    }

    //llamamos a Carlos para que reordene el indice interno que lleva él:
    UpdateIndex (ai_pindex_qtc, ai_n);
}

m4return_t _ClRecordSet_Base::Arrange (void)
{
    M4_ASSERT (GetCurrentBlock ());
    return GetCurrentBlock ()->Arrange ();
}

//-----------------------------------------------------------------------------
// Es una pena seguir a estas alturas de la vida reinventando la rueda.
// Pues sí, este es el algoritmo 'quicksort'
// adaptado sin permiso de Leendert Ammeraal.
//
// Juro por mis niños que 
//  1- intenté utilizar el 'sort' de las STL
//  2- intenté utilizar la función C qsort
// y fracasé, la razón es que de las dos maneras necesito una función
// a la que se le pasan dos elementos y nos devuelve el resultado de 
// compararlos. Para nosotros los elementos los punteros al buffer de cada
// registro, que se guarda en el indice de cada recordset. Desgraciadamente con
// un puntero a un buffer no podemos hacer la comparación, (depende del indice de 
// ordenación activo en el recordset).
// Una posibilidad sería usar un static pointer al indice activo, pero esto
// compromete el multitherding.
//-----------------------------------------------------------------------------

// Copyright(c) 1996 Leendert Ammeraal. All rights reserved.
// This program text occurs in Chapter 3 of
//
//    Ammeraal, L. (1996) Algorithms and Data Structures in C++,
//       Chichester: John Wiley.

//(sort: en catalán suerte)

//tema del índex_qtc: en todos los accessos se hace una indirección a través de él

inline void _qsort_swap (m4uint32_t ai_p, m4uint32_t ai_q, m4uint32_t *ai_pindex_qtc)
{
    m4uint32_t w;

    w = ai_pindex_qtc [ai_p];
    ai_pindex_qtc [ai_p] = ai_pindex_qtc [ai_q];
    ai_pindex_qtc [ai_q] = w;
}

inline void _qsort_swap (m4uint32_t ai_p, m4uint32_t ai_q, m4uint32_t ai_r, m4uint32_t *ai_pindex_qtc)
{
    m4uint32_t w;

    w = ai_pindex_qtc [ai_p];
    ai_pindex_qtc [ai_p] = ai_pindex_qtc [ai_q];
    ai_pindex_qtc [ai_q] = ai_pindex_qtc [ai_r];
    ai_pindex_qtc [ai_r] = w;
}

static m4return_t _qsort (m4uint32_t first, m4uint32_t n, m4uint32_t *ai_pindex_qtc, _ClRegister_Base &reg1, _ClRegister_Base &reg2)
{  
    m4int8_t ci, cj;
    m4uint32_t nleft, nright;
    m4uint32_t i, j, h, last;
   
    while (n >= 3) {  
        last = first + n - 1;
        h = first + ((n-1)>>1);

        // Grandiosa ordenación óptima de tres elementos:
//        reg1.MoveTo (ai_pindex_qtc [first]);
//        reg2.MoveTo (ai_pindex_qtc [h]);
//        if (reg1._compare_with_order (reg2) > 0) {
        if (_compare_index_with_order(reg1, ai_pindex_qtc [first], ai_pindex_qtc [h]) > 0) {
//            reg2.MoveTo (ai_pindex_qtc [last]);
//            if (reg1._compare_with_order (reg2) > 0) {
            if (_compare_index_with_order(reg1, ai_pindex_qtc [first], ai_pindex_qtc [last]) > 0) {
//                reg1.MoveTo (ai_pindex_qtc [h]);
//                if (reg1._compare_with_order (reg2) > 0) {
                if (_compare_index_with_order(reg1, ai_pindex_qtc [h], ai_pindex_qtc [last]) > 0) {
                    _qsort_swap(first, last, ai_pindex_qtc);    //321
                }
                else {
                    _qsort_swap(first, h, last, ai_pindex_qtc); //312
                }
            }  
            else {
                _qsort_swap(first, h, ai_pindex_qtc);           //213
            }
        } 
        else {
//            reg2.MoveTo (ai_pindex_qtc [last]);
//            if (reg1._compare_with_order (reg2) > 0) {
            if (_compare_index_with_order(reg1, ai_pindex_qtc [first], ai_pindex_qtc [last]) > 0) {
                _qsort_swap(first, last, h, ai_pindex_qtc);     //231
            }
            else {
//                reg1.MoveTo (ai_pindex_qtc [h]);
//                if (reg1._compare_with_order (reg2) > 0) {
                if (_compare_index_with_order(reg1, ai_pindex_qtc [h], ai_pindex_qtc [last]) > 0) {
                    _qsort_swap(h, last, ai_pindex_qtc);        //132
                }
                // else do nothing                              //123
            }
        }
        
        if (n == 3)
            return M4_SUCCESS;
      
        i = first + 1; 
        j = last - 1;
        // Pivot a[h] is median of three elements
      
        do {
//            reg2.MoveTo (ai_pindex_qtc [h]);

            for (;;) {
//                reg1.MoveTo (ai_pindex_qtc [i]);
//                if ((ci = reg1._compare_with_order (reg2)) >= 0)
                if ((ci = _compare_index_with_order(reg1, ai_pindex_qtc [i], ai_pindex_qtc [h])) >= 0)
                    break;
                ++i;
            }
         
            for (;;) {
                reg1.MoveTo (ai_pindex_qtc [j]);
//                if ((cj = reg1._compare_with_order (reg2)) <= 0)
                if ((cj = _compare_index_with_order(reg1, ai_pindex_qtc [j], ai_pindex_qtc [h])) <= 0)
                    break;
                --j;
            }
         
            if (i < j) {  
                _qsort_swap(i, j, ai_pindex_qtc);
                if (h == i) 
                    h = j; 
                else if (h == j) 
                    h = i;
            }  
            else {  
                if (i == j)  {
                    i ++;
                    j --;
                } 
                else {  
                    if (ci == 0) 
                        i ++;
                    if (cj == 0) 
                        j --;
                }
                break;
            }
        } while ((++i) <= (--j));

        nleft = j + 1 - first;
        nright = last + 1 - i;
      
        if (nleft < nright) {  
            if (nleft > 1) 
                _qsort (first, nleft, ai_pindex_qtc, reg1, reg2);
            first = i; 
            n = nright;
        }
        else {  
            if (nright > 1)
                _qsort (i, nright, ai_pindex_qtc, reg1, reg2);
            n = nleft;
        }
    }    

    if (n == 2) {
//        reg1.MoveTo (ai_pindex_qtc [first]);
//        reg2.MoveTo (ai_pindex_qtc [first + 1]);
//        if (reg1._compare_with_order (reg2) > 0) {
        if (_compare_index_with_order(reg1, ai_pindex_qtc [first], ai_pindex_qtc [first + 1]) > 0) {
            _qsort_swap(first, first + 1, ai_pindex_qtc);
        }
    }

    return M4_SUCCESS;
}

m4return_t ClBlock_Channel_Proxy::Arrange (void)
{
    m4return_t res;

    //cogemos la del nodo siempre:
    if (m_pClNode_Data == 0)
    {
        DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
        return M4_SUCCESS;
    }

    if (m_pClNode_Data->GetpSearchIndex ()) {
        //enesima implementación:
        //me monto un índice temporal, ordeno sobre él, se lo paso a Carlos y de una tacada
        //se actualiza su indice. Luego se lo paso a todos los reg que estuvieran trabajando sobre
        //este bloque y calculan con él a donde tienen que moverse para que no se note el cambio
                //me explico: si un reg estaba en la posición 17, por efecto de la ordenación
                //ahora el registro pasa a estar en la 1360, en el indice veremos que ahora en la 
                //posición 17 aparece el valor '1360'. Al reg le pasamos el indice y mediante
                //la operacion this->Index = indice [this->Index], hacemos que siga apuntando
                //al mismo registro al que apuntaba antes de reordenar.

        //como de cualquier forma va a ser provisional, no me complico la vida y 
        //el indice me le creo de enteros 32bits a cañon

        m4uint32_t n = Count ();

        if (n) {
            // preparamos el indice:            
            m4uint32_t *pindex_qtc = new m4uint32_t [n];
            register m4uint32_t val = n - 1;
            register m4uint32_t *pi = pindex_qtc + val;
            do {
                *(pi--) = val;
            } while (val--);

            ClRegister reg0 (0);    //sin autoload
            reg0.SetCurrentBlock (this);
            ClRegister reg1 = reg0;

            res = _qsort (0, n, pindex_qtc, reg0, reg1);

            //vale, pues ya está ordenado, ahora actualizamos los índices internos
            //que dependen del indice de registro:
            _UpdateIndex (pindex_qtc, n);

            // llamamos a todos los reg que estaban sobre el bloque para que se actualicen:
			ClBlockReference br;
			br.Set (this);
            m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->Arrange_After (m_pClNode_Data, br, pindex_qtc);

            delete []pindex_qtc;
        }
        else {
            // no hay registros, no es mi problema
            res = M4_SUCCESS;
        }

        //marco el bloque como 'ordenado':
        //ClBlockSearchInfo *psi = GetpSearchInfo ();
        //M4_ASSERT (psi);
        ClBlockSearchInfo::SetSearchIndexId (m_pClNode_Data->GetpSearchIndex ()->Id ());   //<- le asigno el índice del nodo
    }
    else {
        res = M4_ERROR; // no hay indice definido
    }

    return res;
}

m4return_t ClFiltSort::CreateArray(const ClBlockReference &ai_br, _ClRegisterFilter * ai_pClSubFilter, ClMMArray * ai_pmmarray, ClAccessProvider *ai_pap)
{
    m4return_t res;

    //cogemos la del nodo siempre:
//    M4_ASSERT (m_pClRegister->GetpNode_Channel());


    m4uint32_t n;
    res = ai_pClSubFilter->Count (n, ai_br, ai_pap);

    if (n) {

        ai_pmmarray->Init (n, ai_br, ai_pap, ai_pClSubFilter);

        ClBSearchIndex * psi = GetSearchIndex ();

        M4_ASSERT (psi);
/*
********************YA NO DEBERIA SER NECESARIO*************************
        if (m_pClRegister == 0)
            m_pClRegister = (_ClRegister_Base * ) ai_pap;
*/
        ClBSearchIndex *auxSearchIndex = m_pClRegister->GetpNode_Data ()->GetpSearchIndex ();
        res = m_pClRegister->GetpNode_Data ()->SetpSearchIndex (psi);

        // preparamos el indice:            
        m4uint32_t *pindex_qtc = new m4uint32_t [n];
        register m4uint32_t val = n - 1;
        register m4uint32_t *pi = pindex_qtc + val;
        do {
            *(pi--) = val;
        } while (val--);

        ClAccess *paccess = ai_pap->GetpAccess ();
        M4_ASSERT (paccess);
        ClRecordSet rs (paccess);

        rs.SetCurrentBlock (ai_br);
//        rs.Register.Filter.CopyFilter(m_pClRegister->Filter);
        m4int16_t m = rs.Register.Filter.Count();

        rs.Register.Filter.AttachFilterCollection(ai_pClSubFilter);

        ClRegister reg1 = rs.Register;

//        res = _qsort (0, n, pindex_qtc, *m_pClRegister, reg1); // CAMBIAR INTERFACE!!!!!
        res = _qsort (0, n, pindex_qtc, rs.Register, reg1); // CAMBIAR INTERFACE!!!!!

        ai_pmmarray->Set (pindex_qtc, n, ai_br, ai_pap, ai_pClSubFilter);

        delete []pindex_qtc;

        res = m_pClRegister->GetpNode_Data ()->SetpSearchIndex (auxSearchIndex);

        _ClRegisterFilter *pAuxFilter = ai_pClSubFilter;

        while (pAuxFilter->SubFilter())
            pAuxFilter = pAuxFilter->SubFilter();

        rs.Register.Filter.AttachFilterCollection(pAuxFilter);

    }
    else {
        // no hay registros, no es mi problema
        res = M4_SUCCESS;
    }

    SetSorted();
    ai_pmmarray->ResetNewElements();

    return M4_SUCCESS;
}

m4return_t ClFilterInterface::AddSort (m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders, m4pchar_t ai_pName)
{
    if (ai_iNumberItems)
    {
        M4_ASSERT(ai_pItemsNames);
        M4_ASSERT(ai_pOrders);

        _ClRegisterFilter *pAuxFilter = 0;
        pAuxFilter = new ClRegisterStaFilter (m_pClRegister, ai_iNumberItems, ai_pItemsNames, ai_pOrders, ai_pName);

        if (!(*pAuxFilter)->GetSearchIndex())
        {
            delete pAuxFilter;
            return M4_ERROR;
        }
        else
        {
            M4_ASSERT (pAuxFilter);
            return Add (pAuxFilter);
        }
    }

	return M4_ERROR;
}


m4return_t ClFilterInterface::AddSort (m4uint32_t ai_id, m4pchar_t ai_pName)
{
    ClBSearchIndex * pBSearchIndex = m_pClRegister->GetpNode_ExtInfo ()->GetSearchIndexById (ai_id);

    if (pBSearchIndex)
    {
        _ClRegisterFilter *pAuxFilter = 0;
        pAuxFilter = new ClRegisterStaFilter (m_pClRegister, pBSearchIndex, ai_pName);

        M4_ASSERT (pAuxFilter);

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


m4return_t ClFilterInterface::AddSort (m4uint16_t ai_ipos, m4pchar_t ai_pName)
{
    ClBSearchIndex * pBSearchIndex = m_pClRegister->GetpNode_ExtInfo ()->GetSearchIndexByPos (ai_ipos);
    if (pBSearchIndex)
    {
        _ClRegisterFilter *pAuxFilter = 0;
        pAuxFilter = new ClRegisterStaFilter (m_pClRegister, pBSearchIndex, ai_pName);

        M4_ASSERT (pAuxFilter);

        return Add (pAuxFilter);
    }

	return M4_ERROR;
}


//----------------------------------------------------------------------------
// Find
//      Empieza a buscar a partir de la posición del registro
//      Lleva el registro a la posición donde encontró el registro
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::Find (m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions)
{   
	if (Index == M4DM_EOF_INDEX) {
		return M4_FALSE;   //no estoy en eof 
	}
	
	M4_ASSERT (GetCurrentBlock ());
	//ClBlockSearchInfo *psinfo = GetCurrentBlock ()->GetpSearchInfo ();
	
	//psinfo puede ser cero cuando el bloque sobre el que estamos es nihil        
	//pero entonces tengo que estar en eof ...
	//M4_ASSERT (psinfo);
	
	//ClBSearchIndex *psi = psinfo->GetpSearchIndex ();
	ClBSearchIndex *psi = m_pClNode_Data->GetpSearchIndex ();
	m4return_t res;
	
	//if (psi != m_pClNode_Data->GetpSearchIndex ())
	if (psi) {        
		if (GetCurrentBlock ()->GetSearchIndexId () != psi->Id ()) {
			res = GetCurrentBlock ()->Arrange ();            
			if (res != M4_ERROR) {
				//después de hacer un arrange automático, me voy al primero:
				//por favor documentar esto !!!
				Begin ();		        
			}
		} else {
			res = M4_SUCCESS;
		}
	} else {
		//error, no hay indice asignado
		res = M4_ERROR;
	}
	
	if (M4_ERROR == res) {
		MoveToEOF ();
		return M4_FALSE;
	}
	
	//ahora va a estar ordenado:
	M4_ASSERT (GetCurrentBlock ()->GetSearchIndexId () == psi->Id ());
	//M4_ASSERT (psi == m_pClNode_Data->GetpSearchIndex ());
	
	//m4uint32_t iRegister;
	
	
	m4int32_t Lim;
		
	//A buscar...
	
	//Lim1 = 0;     
	m4int32_t Lim1 = Index;
	
	m4int32_t Lim2 = Count();
	
	M4_ASSERT (Lim2);   //si no estoy en eof => tengo que tener registros
	M4_ASSERT (Lim1 <= Lim2);
	
	Lim2--;
	
	/*CompareIndex devuelve:
	-1 significa que el registro en el que buscamos está por debajo del valor buscado
	+1 significa que el registro en el que buscamos está por encima del valor buscado
	0 significa que el registro en el que buscamos es el buscado
	*/
	
	//Buscamos en el primer registro
	m4int32_t cond = psi->CompareIndex (*this, ai_pstArguments, ai_pfunctions);
	MoveTo (Lim1);
	switch (cond)
	{
        case +1:
		//iRegister = -1;
		MoveToEOF ();
		return M4_FALSE;
        case 0:
		//iRegister = Lim1;
		//MoveTo (Lim1);
		return M4_TRUE;
	}
        
	//Buscamos en el ultimo registro
	MoveTo (Lim2);
	cond = psi->CompareIndex (*this, ai_pstArguments, ai_pfunctions);
	switch (cond)
	{
        case -1:
		//ao_iRegister = -1;
		MoveToEOF ();
		return M4_FALSE;
        case 0:
		//ao_iRegister = Lim2;
		//MoveTo (Lim2);
		return M4_TRUE;
	}
	
	//Hacemos la busqueda binaria hasta que se encuentre o no podamos seguir.
	while (1)
	{
		Lim = (Lim1 + Lim2) / 2;
		MoveTo (Lim);
		cond = psi->CompareIndex (*this, ai_pstArguments, ai_pfunctions);
		
		switch (cond)
		{
		case +1:
			Lim2 = Lim;
			break;
		case -1:
			Lim1 = Lim;
			break;          
		default:
			//iRegister = Lim;
			//MoveTo (Lim);
			return M4_TRUE;
		}
		
		if (abs((int)(Lim1-Lim2)) <=1 )
		{
			//iRegister = -1;
			MoveToEOF ();
			return(M4_FALSE);
		}
	}    
}

/**********************************************************************************/
// Nueva Find desde el FilterInterface: solo funciona si hay algun filtro de
// ordenacion aplicado. Sino devuelve error.

m4bool_t ClFilterInterface::Find (m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions, m4puint32_t ai_piNextIndex)
{   
    if (m_pClRegister->Index == M4DM_EOF_INDEX) {
		return M4_FALSE;   //no estoy en eof 
    }

    m4bool_t bIsSorted;
 
    // metodo de ClFilterInterface
    ClBSearchIndex *psi = GetSearchIndexAndSorted(bIsSorted);

    if ((ai_piNextIndex == 0)&&(!bIsSorted))
        UpdateFilter(); //Hay que reordenar si no estamos en el caso de Dani.

    if (!psi)
        return M4_FALSE;

    m4int32_t Lim1;
    m4int32_t Lim2;
    m4int32_t Lim;
    m4int32_t cond;

    Lim1 = m_pClRegister->Index;
    
    if (ai_piNextIndex)
        Lim2 = m_pClRegister->Count() - 1;
    else
        Lim2 = m_pClRegister->Count();

    if (Lim2 == 0)
    {
        if (ai_piNextIndex)
        {
    	    cond = psi->CompareIndex (*m_pClRegister, ai_pstArguments, ai_pfunctions);
            if (cond == 0)
            {
                * ai_piNextIndex = Lim2;
                return M4_TRUE;
            }
            else
            {
                * ai_piNextIndex = M4DM_EOF_INDEX;
                return M4_FALSE;
            }
        }
        else
        {
            return M4_FALSE;
        }
    }

//    M4_ASSERT (Lim1 <= Lim2);

	Lim2--;
    
	/*CompareIndex devuelve:
		-1 significa que el registro en el que buscamos está por debajo del valor buscado
		+1 significa que el registro en el que buscamos está por encima del valor buscado
		 0 significa que el registro en el que buscamos es el buscado
	*/

	//Buscamos en el primer registro
	cond = psi->CompareIndex (*m_pClRegister, ai_pstArguments, ai_pfunctions);
    m_pClRegister->MoveTo (Lim1);
    switch (cond)
    {
        case +1:
            //iRegister = -1;
            m_pClRegister->MoveToEOF ();
            if (ai_piNextIndex)
                * ai_piNextIndex = 0xffffffff;
            return M4_FALSE;
        case 0:
            //iRegister = Lim1;
            //MoveTo (Lim1);
            if (ai_piNextIndex)
                * ai_piNextIndex = Lim1;
            return M4_TRUE;
    }
        
	//Buscamos en el ultimo registro
    m_pClRegister->MoveTo (Lim2);
    cond = psi->CompareIndex (*m_pClRegister, ai_pstArguments, ai_pfunctions);
    switch (cond)
    {
        case -1:
            //ao_iRegister = -1;
            m_pClRegister->MoveToEOF ();
            if (ai_piNextIndex)
                * ai_piNextIndex = Lim2;
            return M4_FALSE;
        case 0:
            //ao_iRegister = Lim2;
            //MoveTo (Lim2);
            if (ai_piNextIndex)
                * ai_piNextIndex = Lim2 - 1;
            return M4_TRUE;
    }
      
	//Hacemos la busqueda binaria hasta que se encuentre o no podamos seguir.
    while (1)
    {
        Lim = (Lim1 + Lim2) / 2;
        m_pClRegister->MoveTo (Lim);
		cond = psi->CompareIndex (*m_pClRegister, ai_pstArguments, ai_pfunctions);
            
        switch (cond)
        {
            case +1:
                Lim2 = Lim;
            break;
            case -1:
                 Lim1 = Lim;
            break;          
            default:
                //iRegister = Lim;
                //MoveTo (Lim);
                if (ai_piNextIndex)
                    * ai_piNextIndex = Lim;
                return M4_TRUE;
        }
            
        if (abs((int)(Lim1-Lim2)) <=1 )
        {
			//iRegister = -1;
            if (ai_piNextIndex)
                * ai_piNextIndex = Lim1;
            m_pClRegister->MoveToEOF ();
            return(M4_FALSE);
        }
    }    
}
