//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clsentcc.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines ...
//
//
//==============================================================================


#include "clsentcc.hpp"
#include "clload.hpp"
#include "clpersis.hpp"
#include "clmtsent.hpp"
#include "clldb.hpp"
#include "ldbcontext.hpp"
#include "blocksynchronization.hpp"
#include "m4condition.hpp"


ClSentCache::ClSentCache (void)
{
	m_nMaxNumSent = 1024;
	m_indStmt = 0;
	m_pLdb = NULL;
	m_nStmtUsed = 0;
}


ClSentCache::~ClSentCache (void)
{
	Clear ();		
	ClearSystem ();
}


m4int32_t ClSentCache::EraseSentsNoUsed (m4bool_t ai_bAll)
{
	MpSentIter mpSentIter;
	itMMpSentCache it;
	itMpSentIter itt;

	for (it = m_mmpSentCache.begin (); it!=m_mmpSentCache.end(); it++) {
		if ((*it).second->m_bIsFree) {
			// No está siendo usada.
			mpSentIter.insert (MpSentIter::value_type ((*it).second->m_indStmt, it));
		}
	}

	// Borramos todas o la la mitad de las que no esten siendo utilizadas.
	m4int32_t nSent = (ai_bAll) ? mpSentIter.size () : (mpSentIter.size () > 2) ? (m4int32_t) (mpSentIter.size () / 2) : 0;

	for (m4int32_t indSent=0; indSent<nSent; indSent++) {
		
		itt = mpSentIter.begin ();
		it = (*itt).second;
		StSentStmt *pSentStmt = (*it).second;

		pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());
		delete pSentStmt->m_pStmt;
		delete pSentStmt;

		m_mmpSentCache.erase (it);
		mpSentIter.erase (itt);
	}

	return (nSent);
}


m4return_t ClSentCache::InsertSent (struct StSentKey &ai_oSentKey, itMMpSentCache &ao_itSent, ClStmt *ai_pStmt, ClLConn *aio_pLConn)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	struct StSentStmt *poSentStmt = NULL;

	if (m_mmpSentCache.size () >= m_nMaxNumSent)
	{
		m_pLdb->ClearCache ();
	}

	poSentStmt = new struct StSentStmt (ai_pStmt);

	// Insertamos el objeto sentencia y su clave en la cache.
	ao_itSent = m_mmpSentCache.insert (MMpSentCache::value_type (ai_oSentKey, poSentStmt));

	// Hacemos un intercambio.
	ai_oSentKey.m_bOwn = M4_FALSE;

	// Ahora el propietario de la memoria es el objeto que está en el mapa.
	StSentKey *pSentKey = (StSentKey *) &((*ao_itSent).first);

	pSentKey->m_bOwn = M4_TRUE;
	m_indStmt++;
	m_nStmtUsed++;

	aio_pLConn->SetLastSentUsed (m_indStmt);

	((*ao_itSent).second)->m_indStmt = m_indStmt;

	return (M4_SUCCESS);
}


m4return_t ClSentCache::DeleteSent (itMMpSentCache &ao_itSent)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	struct StSentStmt *pSentStmt = (*ao_itSent).second;

	pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());

	delete pSentStmt->m_pStmt;
	delete pSentStmt;

	m_mmpSentCache.erase (ao_itSent);

	m_nStmtUsed--;

	ao_itSent = m_mmpSentCache.end ();

	return (M4_SUCCESS);
}


ClStmt *ClSentCache::FindSent (struct StSentKey &ai_oSentKey, itMMpSentCache &ao_itSent, ClLConn *aio_pLConn)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	m4bool_t bFirst = M4_TRUE;
	itMMpSentCache itFreeSent;

	// Buscamos el apuntador del objeto sentencia por su clave.
	ao_itSent = m_mmpSentCache.find (ai_oSentKey);

	if (ao_itSent == m_mmpSentCache.end ()) {
		return (NULL);
	}

	m4bool_t bExit = M4_FALSE;

	do {
		struct StSentStmt* pSentStmt = (*ao_itSent).second;

		if (pSentStmt->m_pStmt->GetpGlobVars()->GetpLConn () == aio_pLConn) {
			// No está libre pero lo tenemos cogido nosotros.
			bExit = M4_TRUE;
		}
		else {
			if (pSentStmt->m_bIsFree) {			
				if (pSentStmt->m_indStmt == aio_pLConn->GetLastSentUsed ()) {
					// Hemos encontrado el anteriormente usado y está libre.
					m_nStmtUsed++;
					bExit = M4_TRUE;
				}
				else {
					if (bFirst) {
						// Cogemos el primero libre.
						itFreeSent = ao_itSent;
						bFirst = M4_FALSE;
					}
				}
			}
		}

		if (!bExit) {
			ao_itSent++;

			if ( !((*ao_itSent).first == ai_oSentKey))
			{
				if (!bFirst) {
					// Tenemos por lo menos uno libre.
					ao_itSent = itFreeSent;
					m_nStmtUsed++;
				}
				else {
					ao_itSent = m_mmpSentCache.end ();
				}
				bExit = M4_TRUE;
			}
		}
	} while (!bExit);

	if (ao_itSent != m_mmpSentCache.end ()) {
		m_indStmt++;

		aio_pLConn->SetLastSentUsed (m_indStmt);

		struct StSentStmt* pSentStmt = pSentStmt = (*ao_itSent).second;
		pSentStmt->m_bIsFree = M4_FALSE;
		pSentStmt->m_indStmt = m_indStmt;

		return (pSentStmt->m_pStmt);
	}

	return (NULL);
}


m4return_t ClSentCache::FreeSents (ClLConn *ai_pLConn)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	for (itMMpSentCache it = m_mmpSentCache.begin (); it!=m_mmpSentCache.end(); it++) {

		ClStmt *pStmt = (*it).second->m_pStmt;
		struct StSentStmt *pSentStmt = (*it).second;

		if ((pStmt->GetpGlobVars()->GetpLConn () == ai_pLConn) && 
			(!pSentStmt->m_bIsFree)) {
			if (pStmt->GetpGlobVars ()->IsInvStmt ()) {

				pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());

				delete pStmt;

				delete pSentStmt;

				// Borramos la entrada en el mapa.
				if (it == m_mmpSentCache.begin ()) {
					m_mmpSentCache.erase (it);
					it = m_mmpSentCache.begin ();
				}
				else {
					// Me quedo referencia al anterior, y despues de borrarlo lo asigno. Al incrementar apuntara al siguiente.
					itMMpSentCache itBef = it;
					itBef--;
					m_mmpSentCache.erase (it);
					it = itBef;
				}
			}
			else {
				pSentStmt->m_pStmt->GetpGlobVars()->Clear ();
				pSentStmt->m_bIsFree = M4_TRUE;
			}

			m_nStmtUsed--;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClSentCache::FreeSent (itMMpSentCache &ao_itSent)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	struct StSentStmt *pSentStmt = (*ao_itSent).second;

	if (!pSentStmt->m_pStmt->GetpGlobVars()->GetpLConn ()->FindStmtInLsPostVal (pSentStmt->m_pStmt)) {
		// Podemos dejarla/borrarla puesto que no está en la cache de postvalidaciones.
		if (pSentStmt->m_pStmt->GetpGlobVars ()->IsInvStmt ()) {

			pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());
			delete pSentStmt->m_pStmt;
			delete pSentStmt;

			m_mmpSentCache.erase (ao_itSent);
		}
		else {
			pSentStmt->m_pStmt->GetpGlobVars()->Clear ();
			pSentStmt->m_bIsFree = M4_TRUE;
		}

		m_nStmtUsed--;
	}

	ao_itSent = m_mmpSentCache.end ();

	return (M4_SUCCESS);
}


m4int32_t ClSentCache::Clear (void)
{
	m4int32_t numStmt = m_mmpSentCache.size ();

	// Cerramos y borramos todos los objetos sentencia.
	for (itMMpSentCache it = m_mmpSentCache.begin (); it!=m_mmpSentCache.end(); it++) {

		struct StSentStmt *pSentStmt = (*it).second;

		pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());
		delete pSentStmt->m_pStmt;
		delete pSentStmt;
	}

	// Borramos la cache completa.
	m_mmpSentCache.erase (m_mmpSentCache.begin (), m_mmpSentCache.end());

	m_nStmtUsed = 0;

	return (numStmt);
}


m4return_t ClSentCache::DefuseSent (ClLConn *ai_pLConn)
{
	m4return_t result;

	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSentences);

	for (itMMpSentCache it = m_mmpSentCache.begin (); it!=m_mmpSentCache.end(); it++) {

		ClStmt *pStmt = (*it).second->m_pStmt;

		if (pStmt->m_eStmType == M4LDB_SELECT) {
			result = ((ClLoad *) pStmt)->Defuse (ai_pLConn);
		}
		else {
			result = ((ClPersist *) pStmt)->Defuse (ai_pLConn);
		}

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClSentCache::InsertSentSystem (m4int32_t ai_idSent, itMMpSentCacheSystem &ao_itSent, ClStmt *ai_pStmt)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSysSentences);

	struct StSentStmt *poSentStmt = new struct StSentStmt (ai_pStmt);

	// Insertamos el objeto sentencia y su clave en la cache.
	ao_itSent = m_mmpSentCacheSystem.insert (MMpSentCacheSystem::value_type (ai_idSent, poSentStmt));

	return (M4_SUCCESS);
}


m4return_t ClSentCache::DeleteSentSystem (itMMpSentCacheSystem &ao_itSent)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSysSentences);

	struct StSentStmt *pSentStmt = (*ao_itSent).second;

	pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());
	delete pSentStmt->m_pStmt;
	delete pSentStmt;

	m_mmpSentCacheSystem.erase (ao_itSent);
	ao_itSent = m_mmpSentCacheSystem.end ();

	return (M4_SUCCESS);
}


ClStmt *ClSentCache::FindSentSystem (m4int32_t ai_idSent, itMMpSentCacheSystem &ao_itSent)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSysSentences);

	m4bool_t bFirst = M4_TRUE;
	itMMpSentCacheSystem itFreeSent;

	// Buscamos el apuntador del objeto sentencia por su clave.
	itMMpSentCacheSystem itSent = m_mmpSentCacheSystem.find (ai_idSent);

	if (itSent == m_mmpSentCacheSystem.end ()) {
		return (NULL);
	}

	do {
		struct StSentStmt* pSentStmt = (*itSent).second;

		if (pSentStmt->m_bIsFree) {
			if (ao_itSent == itSent) {
				// Hemos encontrado el anteriormente usado y está libre.
				pSentStmt->m_bIsFree = M4_FALSE;

				return (pSentStmt->m_pStmt);
			}
			else {
				if (bFirst) {
					// Cogemos el primero libre.
					itFreeSent = itSent;
					bFirst = M4_FALSE;
				}
			}
		}

		itSent++;

		if (itSent == m_mmpSentCacheSystem.end ()) {
			return (NULL);
		}

		if ((*itSent).first != ai_idSent) {

			if (!bFirst) {
				// Tenemos por lo menos uno libre.
				ao_itSent = itFreeSent;

				pSentStmt = (*ao_itSent).second;
				pSentStmt->m_bIsFree = M4_FALSE;

				return (pSentStmt->m_pStmt);
			}
			else {
				return (NULL);
			}
		}

	} while (1);

	return (NULL);
}


m4return_t ClSentCache::FreeSentSystem (itMMpSentCacheSystem &ao_itSent)
{
	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSysSentences);

	struct StSentStmt *pSentStmt = (*ao_itSent).second;

	if (pSentStmt->m_pStmt->GetpGlobVars ()->IsInvStmt ()) {

		pSentStmt->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());

		delete pSentStmt->m_pStmt;
		delete pSentStmt;

		m_mmpSentCacheSystem.erase (ao_itSent);
	}
	else {
		pSentStmt->m_bIsFree = M4_TRUE;
	}

	ao_itSent = m_mmpSentCacheSystem.end ();

	return (M4_SUCCESS);
}


m4int32_t ClSentCache::ClearSystem (void)
{
	m4int32_t numStmt = m_mmpSentCacheSystem.size ();

	// Cerramos y borramos todos los objetos sentencia.
	for (itMMpSentCacheSystem it = m_mmpSentCacheSystem.begin (); it!=m_mmpSentCacheSystem.end(); it++)
	{
		(*it).second->m_pStmt->GetpGlobVars()->Set (m_pLdb->GetSysLConn ());

		delete ((*it).second->m_pStmt);
		delete (*it).second;
	}

	// Borramos la cache completa.
	m_mmpSentCacheSystem.erase (m_mmpSentCacheSystem.begin (), m_mmpSentCacheSystem.end());

	return (numStmt);
}


m4return_t ClSentCache::DefuseSentSystem (ClLConn *ai_pLConn)
{
	m4return_t result;

	ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexSysSentences);

	for (itMMpSentCacheSystem itS = m_mmpSentCacheSystem.begin (); itS!=m_mmpSentCacheSystem.end(); itS++) {

		ClStmt *pStmt = (*itS).second->m_pStmt;

		if (pStmt->m_eStmType == M4LDB_SELECT) {
			result = ((ClLoad *) pStmt)->Defuse (ai_pLConn);
		}
		else {
			result = ((ClPersist *) pStmt)->Defuse (ai_pLConn);
		}

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


