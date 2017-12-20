//## begin module%3912C58400DA.cm preserve=no
//## end module%3912C58400DA.cm

//## begin module%3912C58400DA.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%3912C58400DA.cp

//## Module: m4notifier%3912C58400DA; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\factor\m4sysall\inc\m4notifier.hpp

#ifndef _CLNOTIFIER_HPP_
#define _CLNOTIFIER_HPP_ 1

//## begin module%3912C58400DA.additionalIncludes preserve=no
//## end module%3912C58400DA.additionalIncludes

//## begin module%3912C58400DA.includes preserve=yes

#ifdef _WINDOWS
// to avoid the next warning:
// ..\..\public\stl-toolkit\win32\ospace/osstd/rbtree.h(204) : warning C4284: return type for 'os_tree_const_iterator<class ClNotifiable *,int>::operator ->' is 'class ClNotifiable *const * ' (ie; not a UDT or reference to a UDT.  Will produce errors if applied using infix notation)
//        ..\..\public\stl-toolkit\win32\ospace/osstd/set.h(145) : see reference to class template instantiation 'os_tree_const_iterator<class ClNotifiable *,int>' being compiled
//        ..\..\m4sysall\inc\m4notifier.hpp(166) : see reference to class template instantiation 'set<class ClNotifiable *,class less<class ClNotifiable *>,class allocator<class ClNotifiable *> >' being compiled

#pragma warning( disable : 4284 ) 
#endif

#include "m4sysall_dll.hpp"
#include "m4types.hpp"
#include "m4notif_id.hpp"
#include "m4stl.hpp"

//## end module%3912C58400DA.includes


class M4_DECL_M4SYSALL ClEvent;

//## begin module%3912C58400DA.declarations preserve=no
//## end module%3912C58400DA.declarations

//## begin module%3912C58400DA.additionalDeclarations preserve=yes
//## end module%3912C58400DA.additionalDeclarations


//## begin ClNotifiable%3912C5840133.preface preserve=yes
//## end ClNotifiable%3912C5840133.preface

//## Class: ClNotifiable%3912C5840133
//	-- {AddDecl: 65} class [494..725]
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClNotifiable 
{
  //## begin ClNotifiable%3912C5840133.initialDeclarations preserve=yes
  //## end ClNotifiable%3912C5840133.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Notify%-2033177191; C++
      virtual m4bool_t Notify (ClEvent& )
      {
        //## begin ClNotifiable::Notify%-2033177191.body preserve=yes
return M4_FALSE;
        //## end ClNotifiable::Notify%-2033177191.body
      }

      //## Operation: OnKill%165872287; C++
      virtual m4bool_t OnKill (ClEvent& )
      {
        //## begin ClNotifiable::OnKill%165872287.body preserve=yes
return M4_FALSE;
        //## end ClNotifiable::OnKill%165872287.body
      }

    // Additional Public Declarations
      //## begin ClNotifiable%3912C5840133.public preserve=yes
      //## end ClNotifiable%3912C5840133.public

  protected:
    //## Constructors (specified)
      //## Operation: ClNotifiable%-2031691827; C++
      ClNotifiable (void )
        //## begin ClNotifiable::ClNotifiable%-2031691827.hasinit preserve=no
        //## end ClNotifiable::ClNotifiable%-2031691827.hasinit
        //## begin ClNotifiable::ClNotifiable%-2031691827.initialization preserve=yes
        //## end ClNotifiable::ClNotifiable%-2031691827.initialization
      {
        //## begin ClNotifiable::ClNotifiable%-2031691827.body preserve=yes
        //## end ClNotifiable::ClNotifiable%-2031691827.body
      }

    //## Destructor (specified)
      //## Operation: ~ClNotifiable%2056085386; C++
      virtual ~ClNotifiable (void )
      {
        //## begin ClNotifiable::~ClNotifiable%2056085386.body preserve=yes
        //## end ClNotifiable::~ClNotifiable%2056085386.body
      }

    // Additional Protected Declarations
      //## begin ClNotifiable%3912C5840133.protected preserve=yes
      //## end ClNotifiable%3912C5840133.protected

  private:
    // Additional Private Declarations
      //## begin ClNotifiable%3912C5840133.private preserve=yes
      //## end ClNotifiable%3912C5840133.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNotifiable%3912C5840133.implementation preserve=yes
      //## end ClNotifiable%3912C5840133.implementation

};

//## begin ClNotifiable%3912C5840133.postscript preserve=yes
//## end ClNotifiable%3912C5840133.postscript

//## begin ClNotifier%3912C584013C.preface preserve=yes
//## end ClNotifier%3912C584013C.preface

//## Class: ClNotifier%3912C584013C
//	-- {AddDecl: 72} class [726..1162]
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39180CC9014A;ClEvent { -> F}

class M4_DECL_M4SYSALL ClNotifier 
{
  //## begin ClNotifier%3912C584013C.initialDeclarations preserve=yes
  //## end ClNotifier%3912C584013C.initialDeclarations

  protected:
    //## begin ClNotifier::NotifiableSet%3912C58402B1.preface preserve=yes
    //## end ClNotifier::NotifiableSet%3912C58402B1.preface

    //## Class: NotifiableSet%3912C58402B1; protected
    //## Category: m4sysall%3912C5830151
    //## Subsystem: m4sysall%3912C5830147
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef set<ClNotifiable *,less <ClNotifiable *> > NotifiableSet;

    //## begin ClNotifier::NotifiableSet%3912C58402B1.postscript preserve=yes
	typedef NotifiableSet::iterator itNotifiableSet;
    //## end ClNotifier::NotifiableSet%3912C58402B1.postscript

  public:
    //## Constructors (specified)
      //## Operation: ClNotifier%-1048724705; C++
      ClNotifier (void )
        //## begin ClNotifier::ClNotifier%-1048724705.hasinit preserve=no
        //## end ClNotifier::ClNotifier%-1048724705.hasinit
        //## begin ClNotifier::ClNotifier%-1048724705.initialization preserve=yes
        //## end ClNotifier::ClNotifier%-1048724705.initialization
      {
        //## begin ClNotifier::ClNotifier%-1048724705.body preserve=yes
        //## end ClNotifier::ClNotifier%-1048724705.body
      }

    //## Destructor (specified)
      //## Operation: ~ClNotifier%-37477669; C++
      virtual ~ClNotifier (void )
      {
        //## begin ClNotifier::~ClNotifier%-37477669.body preserve=yes
        //## end ClNotifier::~ClNotifier%-37477669.body
      }


    //## Other Operations (specified)
      //## Operation: OnKill%341468323; C++
      //	-- {AddDecl: 159} region.unprotectedFunction [968..1209]
      virtual void OnKill (ClEvent& ai_pEvent);

      //## Operation: Attach%-226474902; C++
      virtual m4return_t Attach (ClNotifiable* ai_pNewClient);

      //## Operation: Dettach%-1208465467; C++
      //	-- {AddDecl: 157} region.unprotectedFunction [272..511]
      virtual m4return_t Dettach (ClNotifiable* ai_pOldClient);

      //## Operation: Signal%-489603831; C++
      //	-- {AddDecl: 158} region.unprotectedFunction [512..967]
      virtual void Signal (ClEvent& ai_pEvent);

    // Additional Public Declarations
      //## begin ClNotifier%3912C584013C.public preserve=yes
      //## end ClNotifier%3912C584013C.public

  protected:
    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C58402B2
      //## Role: ClNotifier::m_setNotifiable%3912C58402B4
      //## begin ClNotifier::m_setNotifiable%3912C58402B4.role preserve=no  protected: ClNotifier::NotifiableSet {1 -> 1VHAPN}
      NotifiableSet m_setNotifiable;
      //## end ClNotifier::m_setNotifiable%3912C58402B4.role

    // Additional Protected Declarations
      //## begin ClNotifier%3912C584013C.protected preserve=yes
      //## end ClNotifier%3912C584013C.protected

  private:
    // Additional Private Declarations
      //## begin ClNotifier%3912C584013C.private preserve=yes
      //## end ClNotifier%3912C584013C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNotifier%3912C584013C.implementation preserve=yes
      //## end ClNotifier%3912C584013C.implementation

};

//## begin ClNotifier%3912C584013C.postscript preserve=yes
//## end ClNotifier%3912C584013C.postscript

//## begin module%3912C58400DA.epilog preserve=yes
//## end module%3912C58400DA.epilog


#endif
