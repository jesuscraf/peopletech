//## begin module%39105E6D0278.cm preserve=no
//## end module%39105E6D0278.cm

//## begin module%39105E6D0278.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-2017 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4ipcs
//	 File:              threadobjs.cpp
//	 Project:           m4ipcs
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================

#include <threadobjs.hpp>


// --------------------------------------------
// -- Class ClThreadObjects
// --------------------------------------------


// -- Globals.
ClMutex	ClThreadObjects::s_oMutex(M4_TRUE);
ClThreadObjects::ThreadObjectsMap_t	ClThreadObjects::s_mObjsStore;


// -- Retrieves a thread object.
void* ClThreadObjects::Get(eObjectKey ai_eObjKey)
{
	m4iden_t				lTheadId = M4Thread::GetCurrentIdThread();
	ThreadObjectsMapIt_t	iteThreads;
	ObjectsMap_t*			pObjsMap;
	ObjectsMapIt_t			iteObjs;
	void*					pRetObj = NULL;


	s_oMutex.Lock();

	iteThreads = s_mObjsStore.find(lTheadId);

	if (iteThreads == s_mObjsStore.end())
	{
		s_oMutex.Unlock();
		return NULL;
	}
	else
	{
		pObjsMap = (*iteThreads).second;
	}

	s_oMutex.Unlock();


	iteObjs = pObjsMap->find(ai_eObjKey);

	if (iteObjs != pObjsMap->end())
	{
		pRetObj = (*iteObjs).second;
	}


	return pRetObj;
}


// -- Stores a thread object.
m4return_t ClThreadObjects::Put(eObjectKey ai_eObjKey, void* ai_pObj)
{
	m4iden_t				lTheadId = M4Thread::GetCurrentIdThread();
	ThreadObjectsMapIt_t	iteThreads;
	ObjectsMap_t*			pObjsMap;


	s_oMutex.Lock();

	iteThreads = s_mObjsStore.find(lTheadId);

	if (iteThreads == s_mObjsStore.end())
	{
		pObjsMap = new ObjectsMap_t();
		s_mObjsStore[lTheadId] = pObjsMap;
	}
	else
	{
		pObjsMap = (*iteThreads).second;
	}

	s_oMutex.Unlock();


	if (ai_pObj == NULL)
	{
		pObjsMap->erase(ai_eObjKey);
	}
	else
	{
		(*pObjsMap)[ai_eObjKey] = ai_pObj;
	}


	return M4_SUCCESS;
}


// -- Removes objects map belonging to this thread.
m4return_t ClThreadObjects::RemoveThread()
{
	m4iden_t				lTheadId = M4Thread::GetCurrentIdThread();
	ThreadObjectsMapIt_t	iteThreads;


	s_oMutex.Lock();

	iteThreads = s_mObjsStore.find(lTheadId);

	if (iteThreads != s_mObjsStore.end())
	{
		delete (*iteThreads).second;
		s_mObjsStore.erase(lTheadId);
	}

	s_oMutex.Unlock();


	return M4_SUCCESS;
}

