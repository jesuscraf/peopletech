//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itemhash.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             1/10/98
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

#ifndef __ITEMHASH__HPP__
#define __ITEMHASH__HPP__

#include <stdio.h>
#include <string.h>

#include "m4var.hpp"

class ClItemDefIt_Random;
class _AClItemProxy;

#define M4_HASH_INITVAL     0 //por ahora lo ponemos a cero
#define M4_HASH_EMPTY       0xffffffff

/*
By Bob Jenkins, 1996.  bob_jenkins@compuserve.com.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

See http://ourworld.compuserve.com/homepages/bob_jenkins/evahash.htm
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

#define hashsize(n) ((m4uint32_t)1<<(n))
#define hashmask(n) (hashsize(n)-1)


struct ChainTable
{
    ChainTable() : Index (M4_HASH_EMPTY) {}
    /*
    {
        Index = M4_HASH_EMPTY;
        next = 0;
    }

   ~ChainTable()
    {
        if (next)
            delete next;
    }
    */
	m4return_t GetSize(ClSize &ao_size);

    m4uint32_t Index;
    //ChainTable * next;
};

class ClItemHash
{
public:

	ClItemHash(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4uint32_t ai_iCount);

    m4uint32_t Hfind(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4pcchar_t name, m4bool_t ai_bIsUpper = M4_FALSE);

	~ClItemHash()
	{
		delete [] CTable;
	}
	m4return_t GetSize(ClSize &ao_size);


private:

    m4uint32_t hash(m4uint8_t *k, m4uint32_t length);
    m4uint32_t hash_AlreadyUpper(m4uint8_t *k, m4uint32_t length);

    m4pcchar_t GiveString(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4uint32_t ai_index);

    m4uint32_t m_iCount;
    //m4uint8_t truncbits;
    m4uint32_t m_hashmask;

    //ClItemDefIt_Random * m_itemdefit;
    //_AClItemProxy **m_ppItemProxy;

    ChainTable * CTable;
};

#endif //__ITEMHASH__HPP__

