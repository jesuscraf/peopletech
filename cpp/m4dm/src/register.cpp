//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             register.cpp
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

#include "register.hpp"
#include "access.hpp"
#include "node_knl.hpp"
#include "bnihil.hpp"    
#include "blockref.hpp"
#include "laneinfo.hpp"
#include "pl_data.hpp"
#include "m4mdrt.hpp"


///////////////////////////////////////////////////////////////////////////////
//
// ClRegister_Current
//


//----------------------------------------------------------------------------
// _SetNewIndex
// actualiza los bloques activos en el access
//      => utiliza el ChildNodeRB del canal
//----------------------------------------------------------------------------

void ClRegister_Current::_Refresh (void)
{
    ClNode *pnode = GetpNode ();
    m4uint16_t nchildren = pnode->GetpExtInfo ()->ChildNodeRB.Count ();
    m4uint16_t i;

    for (i = 0; i < nchildren; i++) {
        // para cada hijo RB, se actualiza el contexto:
        // llamamos al nodo del access al que pertenece este registro
		ClBlockReference br;
		GetChildBlock (i, br);
        pnode->_UpdateAccessNodeData (pnode->GetpExtInfo ()->ChildNodeRB, (ClChildIndex)i,
            br);
    }

    if (GetpNode ())
    {
        if (GetpNode ()->GetpLane_Info())
        {
            nchildren = GetpNode ()->GetpLane_Info()->ChildNodeRB.Count ();
            for (i = 0; i < nchildren; i++) {
                // para cada hijo RB, se actualiza el contexto:
                // llamamos al nodo del access al que pertenece este registro
		        ClBlockReference br;
		        GetChildBlock (i + GetpNode_Data()->GetpExtInfo()->ChildNodeRB.Count (), br);
                pnode->_UpdateAccessNode (pnode->GetpLane_Info()->ChildNodeRB, (ClChildIndex)i, br);
            }
        }
    }

	SetChange() ;
}

void ClRegister_Current::_RefreshEOF (void)
{
	ClBlockReference br;
	br.Set (&g_oNihilBlock);
    ClNode *pnode = GetpNode ();

    m4uint16_t nchildren = pnode->GetpExtInfo ()->ChildNodeRB.Count ();
    m4uint16_t i;

    for (i = 0; i < nchildren; i++) {
        // para cada hijo RB, se actualiza el contexto:
        // llamamos al nodo del access al que pertenece este registro
		ClBlockReference br;
		GetChildBlock (i, br);
        pnode->_UpdateAccessNodeData (pnode->GetpExtInfo ()->ChildNodeRB, (ClChildIndex)i, br);
    }

    if (GetpNode ())
    {
        if (GetpNode ()->GetpLane_Info())
        {
            nchildren = GetpNode ()->GetpLane_Info()->ChildNodeRB.Count ();
            for (i = 0; i < nchildren; i++) {
                // para cada hijo RB, se actualiza el contexto:
                // llamamos al nodo del access al que pertenece este registro
		        ClBlockReference br;
		        GetChildBlock (i + GetpNode_Data()->GetpExtInfo()->ChildNodeRB.Count (), br);
                pnode->_UpdateAccessNode (pnode->GetpLane_Info()->ChildNodeRB, (ClChildIndex)i, br);
            }
        }
    }

	SetChange() ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

m4bool_t ClRegister_Current::Begin (void)
{
    ClRegisterIndex oldindex = Index;
    
    m4bool_t isnoteof = _ClRegister_Base::Begin ();

    if (Index != oldindex) {
        if (isnoteof) {
            _Refresh ();
        }
        else {
            _RefreshEOF ();
        }
    }

    return isnoteof;
}

m4bool_t ClRegister_Current::End (void)
{
    ClRegisterIndex oldindex = Index;
    
    m4bool_t isnoteof = _ClRegister_Base::End ();

    if (Index != oldindex) {
        if (isnoteof) {
            _Refresh ();
        }
        else {
            _RefreshEOF ();
        }
    }

    return isnoteof;
}

m4bool_t ClRegister_Current::Next (void)
{
    ClRegisterIndex oldindex = Index;
    
    m4bool_t isnoteof = _ClRegister_Base::Next ();

    if (Index != oldindex) {
        if (isnoteof) {
            _Refresh ();
        }
        else {
            _RefreshEOF ();
        }
    }

    return isnoteof;
}

m4bool_t ClRegister_Current::Prev (void)
{
    ClRegisterIndex oldindex = Index;
    
    m4bool_t isnoteof = _ClRegister_Base::Prev ();

    if (Index != oldindex) {
        if (isnoteof) {
            _Refresh ();
        }
        else {
            _RefreshEOF ();
        }
    }

    return isnoteof;
}

m4bool_t ClRegister_Current::MoveTo (ClRegisterIndex ai_newindex)
{
    ClRegisterIndex oldindex = Index;
    
    m4bool_t isnoteof = _ClRegister_Base::MoveTo (ai_newindex);

    if (Index != oldindex) {
        if (isnoteof) {
            _Refresh ();
        }
        else {
            _RefreshEOF ();
        }
    }

    return isnoteof;
}

m4bool_t ClRegister_Current::MoveToMM (ClMMIndex ai_MMIndex)
{
    m4bool_t isnoteof = _ClRegister_Base::MoveToMM (ai_MMIndex);

    if (isnoteof) {
        _Refresh ();
    }
    else {
        _RefreshEOF ();
    }

    return isnoteof;
}

void ClRegister_Current::MoveToEOF (void)
{
    ClRegisterIndex oldindex = Index;

    _ClRegister_Base::MoveToEOF ();

    if (Index != oldindex) {
	    _RefreshEOF ();
	}
}

m4return_t ClRegister_Current::Add (m4return_t * ao_pDone)
{
    m4return_t res, ao_done = M4_SUCCESS;

    res = _ClRegister_Base::Add (&ao_done);

    //además hacemos un refresh, para que se enteren los hijos del cambio:
    if ((res == M4_SUCCESS)&&(ao_done == M4_SUCCESS))
        _Refresh ();

    return res;
}

m4return_t ClRegister_Current::Insert (m4return_t * ao_pDone)
{
    m4return_t res, ao_done = M4_SUCCESS;

    res = _ClRegister_Base::Insert (&ao_done);

    //además hacemos un refresh, para que se enteren los hijos del cambio:
    if ((res == M4_SUCCESS)&&(ao_done == M4_SUCCESS))
        _Refresh ();

    return res;
}

//----------------------------------------------------------------------------
// MoveToMandatory
//      igual que el moveto, pero hace refresh incondicionalmente, aunque el
//      índice aparentemente no se halla movido. Se usa en varias funciones
//      en areg_v2.cpp
//----------------------------------------------------------------------------

m4bool_t ClRegister_Current::MoveToMandatory (ClRegisterIndex ai_newindex)
{
    m4bool_t isnoteof = _ClRegister_Base::MoveTo (ai_newindex);

    if (isnoteof) {
        _Refresh ();
    }
    else {
        _RefreshEOF ();
    }

    return isnoteof;
}


//////////////////////////////////////////////////////////////////////////////

//
// Function:  ClRegister_Current::Clone
//
// Arguments:  void
//
// Returns: _ClRegister_Base* - The cloned register
//
// Description:
//    Creates a new register and assigns the state
//    of the current register to that register using
//    the equality operator.  Returns the base class
//    so it can be used by the wrapper
//
// Author: Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 14 December 1998
//
_ClRegister_Base*
ClRegister_Current::Clone(m4bool_t ai_bTotal)
{
	_ClRegister_Base * poRegister;

	if (ai_bTotal)
	{
		// Create the new register WORK!!!!!!!!!
		poRegister = new ClRegister (GetpAccess ());
	}

	else
	{
		// Create the new register CURRENT!!!!!!
		poRegister = new ClRegister_Current (GetpAccess ());
	}

	// If the creation of the register succeeded
	if (poRegister)
	{
		// Assign the state of the current register
		// to the new register
        if (!ai_bTotal)
		    *poRegister = *this;
        else
            poRegister->_Copy (*this, M4_TRUE);
	}

	// Return the pointer to the new register or NULL
	return poRegister;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClRegister
//

ClRegister&	ClRegister::operator[]( ClRegisterIndex ai_regindex )
{
	/* Bug 0098510
	Si estamos en carga parcial y dentro de la ventana actual nos podemos
	mover, pero si va a saltar de ventana se da un error y se mueve a EOF
	*/
	m4uint32_t			iNumRows = 0 ;
	m4int32_t			iFirstVisible = -1 ;
	m4int32_t			iLastVisible = -1 ;
	ClNodeDef			*poNodeDef = NULL ;
	ClPartialLoadData	*poPartial = NULL ;


	/* Bug 0099198
	Si se mueve a EOF hay que dejar que se mueva, que es correcto
	*/
	if( ai_regindex != M4DM_EOF_INDEX )
	{
		poNodeDef = GetpNodeDef() ;
		iNumRows = poNodeDef->NumRows() ;

		if( iNumRows > 0 )
		{
			poPartial = GetCurrentBlock()->GetpPL_Data() ;
			iFirstVisible = poPartial->GetFirstVisible() ;
			iLastVisible = poPartial->GetLastVisible() ;

			if( ai_regindex < iFirstVisible || ai_regindex >= iLastVisible )
			{
				DUMP_CHLOG_ERRORF( M4_CH_DM_NO_BRACKETS_IN_PL, ai_regindex << iFirstVisible << iLastVisible - 1 << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
				MoveToEOF() ;
				return( *this ) ;
			}
		}
	}

    MoveTo (ai_regindex);
    return *this;
}



//
// Function:  ClRegister::Clone
//
// Arguments:  void
//
// Returns: _ClRegister_Base* - The cloned register
//
// Description:
//    Creates a new register and assigns the state
//    of the current register to that register using
//    the equality operator.  Returns the base class
//    so it can be used by the wrapper
//
// Author: Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 14 December 1998
//
_ClRegister_Base*
ClRegister::Clone(m4bool_t ai_bTotal)
{
	// Create the new register
	ClRegister* poRegister = new ClRegister (GetpAccess ());

	// If the creation of the register succeeded
	if (poRegister)
	{
		// Assign the state of the current register
		// to the new register
        if (!ai_bTotal)
		    *poRegister = *this;
        else
            poRegister->_Copy (*this, M4_TRUE);
	}

	// Return the pointer to the new register or NULL
	return poRegister;
}

_ClRegister_Base*
ClAccessRegister::Clone(m4bool_t ai_bTotal)
{
	// Create the new register
	ClRegister* poRegister = new ClRegister (GetpAccess ());

	// If the creation of the register succeeded
	if (poRegister)
	{
		// Assign the state of the current register
		// to the new register
        if (!ai_bTotal)
		    *poRegister = *this;
        else
            poRegister->_Copy (*this, M4_TRUE);
	}

	// Return the pointer to the new register or NULL
	return poRegister;
}
/*******************************************************************/
// Estas no harian falta, pero por si se les añaden miembros.
m4return_t 
ClRegister_Current::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClRegister_Current);
	    ao_size-=sizeof(_ClRegister_Base);

	if (_ClRegister_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t 
ClRegister::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClRegister);
	    ao_size-=sizeof(_ClRegister_Free);

	if (_ClRegister_Free::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t 
ClAccessRegister::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClAccessRegister);
	    ao_size-=sizeof(_ClRegister_Base);

	if (_ClRegister_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

/*******************************************************************/
