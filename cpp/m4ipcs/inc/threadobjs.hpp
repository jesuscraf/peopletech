//## begin module%3AE01F550061.cm preserve=no
//## end module%3AE01F550061.cm

//## begin module%3AE01F550061.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-2017 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4ipcs
//	 File:              threadobjs.hpp
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

#ifndef threadobjs_hpp
#define threadobjs_hpp 1

#include <m4ipcs_dll.hpp>
#include <m4thread.hpp>
#include <m4stl.hpp>

// --------------------------------------------
// -- Class ClThreadObjects
// --------------------------------------------

class M4_DECL_M4IPCS ClThreadObjects
{
public:

	// -- Object keys.
	typedef enum { ROWS_QUOTA_CONTROLLER } eObjectKey;

private:

	// -- Map of object keys to object pointer.
	typedef map< eObjectKey, void*, less<eObjectKey> >		ObjectsMap_t;
	typedef ObjectsMap_t::iterator							ObjectsMapIt_t;

	// -- Map of thread ids to container of objects.
	typedef map< m4iden_t, ObjectsMap_t*, less<m4iden_t> >	ThreadObjectsMap_t;
	typedef ThreadObjectsMap_t::iterator					ThreadObjectsMapIt_t;

public:

	// -- Retrieves a thread object.
	static void* Get(eObjectKey ai_eObjKey);

	// -- Stores a thread object.
	static m4return_t Put(eObjectKey ai_eObjType, void* ai_pObj);

	// -- Removes objects map belonging to this thread.
	static m4return_t RemoveThread();

private:

	// -- Internal synch.
	static ClMutex s_oMutex;

	// -- All threads objects.
	static ThreadObjectsMap_t s_mObjsStore;
};

#endif
