//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           handles.cpp
// File:             handles.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             05/07/97
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

#include "index.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes
/*
template <class F>
class _VALUE_PUBLIC_
{
public:
    _VALUE_PUBLIC_ ()   { m_i = 0; }
    _VALUE_PUBLIC_ (const int ai_int) { m_i = ai_int; }

    const _VALUE_PUBLIC_ &operator = (const int ai_int)    { m_i = ai_int; return *this; }
    const _VALUE_PUBLIC_ &operator = (_VALUE_PUBLIC_ & ai_m)  { m_i = ai_m.m_i; return *this; }

    void inc (void)
    {
        m_i++;
    }

    operator const int () { return m_i; }

private:
    m4uint32_t m_i;
};

template <class F>
class _VALUE_PRIVATE_
{
private:
    _VALUE_PRIVATE_ ()
    {
        m_i = 0;
    }

    void inc (void)
    {
        m_i++;
    }

    m4uint32_t m_i;

friend F;
};

class clindex
{
    clindex () {}
};

class clhandle
{
    clhandle () {}
};

class clxxx
{
public:
    clxxx () {}

    f1 (_VALUE_PUBLIC_<clxxx> ai_pp);
    f2 (clhandle ai_pp);

    _VALUE_PRIVATE_<clxxx> index;
};

class clyyy
{
public:
    clyyy () {}

    f1 (_VALUE_PUBLIC_<clyyy> ai_pp);
    f2 (clhandle ai_pp);

    _VALUE_PRIVATE_<clyyy> index;
};

void fff1 (void)
{
    clxxx x;
    clyyy y;

    _VALUE_PUBLIC_<clxxx> i1;
    _VALUE_PUBLIC_<clyyy> i2;

    i1.inc ();
    i1++;
    if (i1 == 0) {
        --i1;
    }

    x.index.inc ();
    y.index.inc ();

    x.f1 (i1);
    y.f1 (i2);

    x.f1 (i2);
    y.f2 (i1);
}
*/
/*
ostream &operator<< (ostream &ai_os, const _ClIndex_Base_ &ai_ii)
{
    ai_ii << ai_os;

    return ai_os;
} 
*/
//## end module.epilog
