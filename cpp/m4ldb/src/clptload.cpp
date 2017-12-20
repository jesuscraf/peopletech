//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clptload.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================



#include "clptload.hpp"
#include "ldbcontext.hpp"
#include "m4event.hpp"
#include "blocksynchronization.hpp"



void StPartialLoad::Close ()
{
	m_pCursor->Close ();
	m_pChannel = 0;
	m_pBlock = 0;
	m_idLConn = -1;
	m_iReadRegisters = 0;
	m_bOpenCursor = M4_FALSE;
}


PStPartialLoad VcPartialLoad::FindPartialLoad (ClCursor *ai_pCursor, m4int32_t ai_idLConn)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexPartialLoad);

	for (itVcPartialLoad it=begin (); it!=end (); it++)
	{
		if ((*it)->m_pCursor == ai_pCursor)
		{
			return (*it);
		}
	}

	PStPartialLoad pPL = new StPartialLoad (ai_pCursor, ai_idLConn);
	push_back (pPL);
	return (pPL);
}


m4return_t VcPartialLoad::ClosePartialLoad (ClCursor *ai_pCursor)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexPartialLoad);

	ClChannel *pChannel = 0;
	for (itVcPartialLoad it=begin (); it!=end (); it++)
	{
		if ((*it)->m_pCursor == ai_pCursor)
		{
			pChannel = (*it)->m_pChannel;
			(*it)->Close ();
			break;
		}
	}

	// Si lo hemos encontrado, miramos a ver si nos podemos detachar.
	if (pChannel){
		_DettachIfPossible(pChannel);
	}

	return (M4_SUCCESS);
}


m4return_t VcPartialLoad::ErasePartialLoad (ClCursor *ai_pCursor)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexPartialLoad);

	for (itVcPartialLoad it=begin (); it!=end (); it++)
	{
		if ((*it)->m_pCursor == ai_pCursor)
		{
			delete (*it);
			erase (it);
			break;
		}
	}

	return (M4_SUCCESS);
}


m4return_t VcPartialLoad::DiscardResults (m4int32_t ai_idLConn, m4bool_t ai_bIgnoreChannel)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexPartialLoad);

	for (m4int32_t indPL = size ()-1; indPL>=0; indPL--)
	{
		itVcPartialLoad it=begin () + indPL; 

		if ((*it)->m_idLConn == ai_idLConn)
		{
			if (ai_bIgnoreChannel)
			{
				(*it)->SetChannel (NULL);
			}
			delete (*it);
			erase (it);
		}
	}
	
	return (M4_SUCCESS);
}


m4bool_t VcPartialLoad::Notify (ClEvent &ai_Event)
{
	// no compruebo el retorno. (En cualqueir caso me detacho.)
	_ClosePartialLoad (ai_Event);
	return M4_TRUE;
}


m4return_t VcPartialLoad::_ClosePartialLoad (ClEvent &ai_Event)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexPartialLoad);

	ClChannel *pChannel = (ClChannel *)ai_Event.GetContainer();

	// Recorro el vector, buscando los que apunten al canal que va a morir.
	for (itVcPartialLoad it=begin (); it!=end (); it++)
	{
		if ((*it)->m_pChannel == pChannel)
		{
			// OK. Me lo cargo.
			(*it)->Close ();
		}
	}
	// OJO !!
	// No nos detachamos. Hemos borrado todas las referencias que teniamos al canal.
	// Cuando retornamos, le decimos que nos detache el propio canal.
	//pChannel->m_pDestroy_Notifier.Dettach(this);
	return M4_SUCCESS;
}


/*
Recorrido de vector para ver si me puedo detachar del canal.
Si la única referencia que tengo, es el iterador que me pasan, me detacho.
*/
m4bool_t VcPartialLoad::_DettachIfPossible(ClChannel *ai_pChannel)
{
	for (itVcPartialLoad it=begin (); it!=end (); it++)
	{
		if ((*it)->m_pChannel == ai_pChannel)
		{
			// El canal es el mismo. No nos podemos detachar.
			return M4_FALSE;
		}
	}

	// Si hemos llegado aqui, nos podemos detachar sin problemas.
	ai_pChannel->m_pDestroy_Notifier->Dettach(this);
	return M4_TRUE;
}

