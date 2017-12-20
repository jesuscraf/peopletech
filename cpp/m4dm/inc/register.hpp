//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             register.hpp
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

#ifndef __REGISTER__HPP__
#define __REGISTER__HPP__


#include "m4dm_dll.hpp"
#include "aregister.hpp"
#include "m4stl.hpp"
#include "changelistener.hpp"


class ClAccess;
class ClNotifier;


///////////////////////////////////////////////////////////////////////////////
//
// ClAccessRegister
//

class M4_DECL_M4DM ClAccessRegister : public _ClRegister_Base
{
public:
    ClAccessRegister (ClAccess *ai_paccess) : _ClRegister_Base (ai_paccess) {}
    ClAccessRegister (const ClAccessRegister &ai_reg) : _ClRegister_Base (ai_reg) {}
   ~ClAccessRegister () {}

    ClAccessRegister &            operator[] (ClRegisterIndex ai_regindex)              //random
    {    
        MoveTo (ai_regindex);
        return *this;
    }

    ClAccessRegister &            operator++ () // pre
    {    
        Next ();
        return *this;
    }

    ClAccessRegister &            operator-- () // pre
    {    
        Prev ();
        return *this;
    }

	_ClRegister_Base& BaseIndex(ClRegisterIndex ai_regindex)
	{
		return ((_ClRegister_Base&) operator[](ai_regindex));
	}

	_ClRegister_Base* Clone(m4bool_t ai_bTotal = M4_FALSE);

	m4return_t GetSize(ClSize &ao_size);
private:
};

///////////////////////////////////////////////////////////////////////////////
//
// ClRegister
//

class M4_DECL_M4DM ClRegister : public _ClRegister_Free
{
public:
    ClRegister (ClAccess *ai_paccess, m4bool_t ai_bnotifierenabled = M4_TRUE) : _ClRegister_Free (ai_paccess, ai_bnotifierenabled) {}
    ClRegister (const _ClRegister_Free &ai_reg) : _ClRegister_Free (ai_reg) {}
    ClRegister (const _ClRegister_Base &ai_reg) : _ClRegister_Free (ai_reg) {}
   ~ClRegister () {}

    ClRegister &            operator[] (ClRegisterIndex ai_regindex) ;

    ClRegister &            operator++ () // pre
    {    
        Next ();
        return *this;
    }

    ClRegister &            operator-- () // pre
    {    
        Prev ();
        return *this;
    }

	_ClRegister_Base& BaseIndex(ClRegisterIndex ai_regindex)
	{
		return ((_ClRegister_Base&) operator[](ai_regindex));
	}

	_ClRegister_Base* Clone(m4bool_t ai_bTotal = M4_FALSE);

	m4return_t GetSize(ClSize &ao_size);
private:
};

///////////////////////////////////////////////////////////////////////////////
//
// ClRegister_Current
//

class M4_DECL_M4DM ClRegister_Current : public _ClRegister_Base, public ClChangeListener
{
public:
    ClRegister_Current (ClAccess *ai_paccess) : _ClRegister_Base (ai_paccess) {}
   ~ClRegister_Current () {}

    ClRegister_Current &operator[] (ClRegisterIndex ai_regindex)              //random
    {    
        MoveTo (ai_regindex);
        return *this;
    }

    ClRegister_Current &operator++ ()  // pre
    {    
        Next ();
        return *this;
    }

    ClRegister_Current &operator-- ()  // pre
    {    
        Prev ();
        return *this;
    }

	_ClRegister_Base& BaseIndex(ClRegisterIndex ai_regindex)
	{
		return ((ClRegister_Current&) operator[](ai_regindex));
	}	

	_ClRegister_Base* Clone(m4bool_t ai_bTotal = M4_FALSE);

    //----------------------------
    //Iterator interface:
    m4bool_t     Begin (void);
    m4bool_t     End (void);
    m4bool_t     Next (void);
    m4bool_t     Prev (void);    
    m4bool_t     MoveTo (ClRegisterIndex ai_newindex);
    m4bool_t     MoveToMM (ClMMIndex ai_MMIndex);
    void         MoveToEOF (void);
    m4bool_t     MoveToMandatory (ClRegisterIndex ai_newindex);

    m4return_t   Add (m4return_t * ao_pDone = 0);
    m4return_t   Insert (m4return_t * ao_pDone = 0);
    //----------------------------

    void _Refresh (void);
    void _RefreshEOF (void);

	m4return_t GetSize(ClSize &ao_size);
private:
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClRegister &ai_register);
M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClRegister_Current &ai_register);

#endif  //__REGISTER__HPP__ 
