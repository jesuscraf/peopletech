//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorCache.cpp
//	 Project:           M4ExeForChannel
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================

#include "clexecutorcache.hpp"
#include <blocksynchronization.hpp>


//#include "clfileiod.hpp"	// ClExecutorStateCOFactory



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//================================
//	ClExecutorStateCacheTree
//================================

ClExecutorStateCacheTree::ClExecutorStateCacheTree(m4uint32_t ai_ulDefaultPolicy, m4uint32_t ai_ulMaxSizeCache, m4uint32_t ai_ulMaxNOfObjs, m4uint32_t ai_ulRefreshRatio)
	: ClCacheTree(0,0,ai_ulMaxSizeCache,ai_ulMaxNOfObjs,ai_ulRefreshRatio)
{
}



m4return_t ClExecutorStateCacheTree::NumOfObjectsControl ()
{
	ClCacheableObject * pObject = 0;

	// Objetos en RAM
	m4uint32_t ulContRAM;
	ulContRAM = m_ulNumOfObjInCache -  m_ulNumOfObjInDisk;

	//Se comprueba el número de objetos en caché y si se sobrepasa el límite, 
	//se intenta persistir los LRU (no referenciados) que estén en RAM, pero sin borrarlos.
	CacheableObject2LRUMap::iterator IterLRUMap;

	// En "m_ulMaxNOfObjects" sólo debemos llevar la cuenta de los "Objetos en RAM"
	// m_ulNumOfObjInCache

	if( ulContRAM >= m_ulMaxNOfObjects )
	{
		// Recorrido secuencial: No Refenciadas y en RAM => los mandamos a DISCO
		for(IterLRUMap=LRUDateMap.begin();
			IterLRUMap!=LRUDateMap.end();
			IterLRUMap++)
		{
			pObject=(*IterLRUMap).second;

			// Sólo tratamos los NO Referenciados, y en RAM
			if( !pObject->IsReferenced() ) {

				// Si esta en RAM => persistimos a disco y borramos de RAM
				if(pObject->Getm_bRam()==M4_TRUE) {
					m4bool_t bDisk = pObject->Getm_bDisk();

					if (bDisk == M4_FALSE) {
						// Uno mas al disco (y es nuevo)
						++m_ulNumOfObjInDisk;
					}

					// Persistir objeto a DISCO de todas las formas
					pObject->SaveValueToDisk();
					bDisk = pObject->Getm_bDisk();

					if (bDisk == M4_TRUE) {
						// Borrar objeto de RAM
						if((pObject->DeleteValueFromRam())==M4_ERROR) {
							return M4_ERROR;
						}

						m4uint32_t ulSizeObject=pObject->GetSize();
						m_ulSizeCache -= ulSizeObject;

						// Decrementamos el contador de objetos en RAM
						--ulContRAM;
					} else {
						// Algo debe haber pasado
						--m_ulNumOfObjInDisk;
						++ulContRAM;
					}
				} // En RAM
			} // No Referenciado

			//Cuando alcanzamos el valor m_ulMaxNOfObjects-m_ulDelRatioObjs ya no hace falta seguir enviando a disco.
			if((ulContRAM <= (m_ulMaxNOfObjects - m_ulDelRatioObjs))|| !LRUDateMap.size())
				break;
		}
	}

	return M4_SUCCESS;
}

m4return_t ClExecutorStateCacheTree::PutObject (ClCacheableObject *ai_pObject)
{
	if (ai_pObject->Getm_bDisk())
	{
		ClMutBlock oBlock(m_oMutex);

		// deletes previous version of the file. The cache system normally doesn't overwrites the file on disk
		// fix bug 0063507 and 0146136
		if (ai_pObject->DeleteValueFromDisk() != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		--m_ulNumOfObjInDisk;
	}

	return ClCacheTree::PutObject(ai_pObject);
}
