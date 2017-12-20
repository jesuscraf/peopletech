//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             aregister.hpp
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

#ifndef __AREGWRITE__HPP__
#define __AREGWRITE__HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "clres.hpp"

//#include "aregister.hpp"
//#include "node_knl.hpp"

class _ClRegister_Base;
class ClSize;

class M4_DECL_M4DM _ClRegister_Insert
{
public:

    virtual m4return_t   Add (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;
    virtual m4return_t   Insert (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;
	virtual m4return_t	 GetSize(ClSize &ao_size);

};

class M4_DECL_M4DM ClRegister_Insert : public _ClRegister_Insert
{
public:

    m4return_t   Add (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   Insert (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
	m4return_t	 GetSize(ClSize &ao_size);
};

class M4_DECL_M4DM ClRegister_InsertNihil : public _ClRegister_Insert
{
public:

    m4return_t   Add (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   Insert (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
	m4return_t	 GetSize(ClSize &ao_size);
};

class M4_DECL_M4DM _ClRegister_Delete
{
public:

    virtual m4return_t   Delete (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;
    virtual m4return_t   Destroy (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;

    virtual m4return_t   DeleteAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;
    virtual m4return_t   DestroyAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0) = 0;
	virtual m4return_t	 GetSize(ClSize &ao_size);
};

class M4_DECL_M4DM ClRegister_Delete : public _ClRegister_Delete
{
public:

    m4return_t   Delete (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   Destroy (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);

    m4return_t   DeleteAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   DestroyAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
	m4return_t	 GetSize(ClSize &ao_size);

private:
    m4return_t _DeleteCascade (_ClRegister_Base * ai_pClReg);
};

class M4_DECL_M4DM ClRegister_DeleteNihil : public _ClRegister_Delete
{
public:

    m4return_t   Delete (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   Destroy (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   DeleteAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
    m4return_t   DestroyAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone = 0);
	m4return_t	 GetSize(ClSize &ao_size);
};

#endif // __AREGWRITE__HPP__
