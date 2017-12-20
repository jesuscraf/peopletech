//## begin module%3923E8690305.cp preserve=no
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
//## end module%3923E8690305.cp

//## Module: CriticalSection%3923E8690305; Subprogram specification
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//	d:\fuentesservidor\m4ipcs\version\inc\win32
//## Source file: D:\eduardoma\m4ipcs\inc\win32\criticalsection.hpp

#ifndef criticalsection_h
#define criticalsection_h 1

// syncro
#include <syncro.hpp>

//## Class: ClCriticalSection%3923E7D00228
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCriticalSection : public ClSynObject  //## Inherits: <unnamed>%3923E7D8037E
{
  public:
    //## Constructors (specified)
      //## Operation: ClCriticalSection%958654617; C++
      ClCriticalSection ()
        //## begin ClCriticalSection::ClCriticalSection%958654617.hasinit preserve=no
        //## end ClCriticalSection::ClCriticalSection%958654617.hasinit
        //## begin ClCriticalSection::ClCriticalSection%958654617.initialization preserve=yes
        //## end ClCriticalSection::ClCriticalSection%958654617.initialization
      {
        //## begin ClCriticalSection::ClCriticalSection%958654617.body preserve=yes
        //## end ClCriticalSection::ClCriticalSection%958654617.body
      }

      //## Operation: ClCriticalSection%958654618; C++
      ClCriticalSection (m4bool_t ao_bInit)
        //## begin ClCriticalSection::ClCriticalSection%958654618.hasinit preserve=no
        //## end ClCriticalSection::ClCriticalSection%958654618.hasinit
        //## begin ClCriticalSection::ClCriticalSection%958654618.initialization preserve=yes
        //## end ClCriticalSection::ClCriticalSection%958654618.initialization
      {
        //## begin ClCriticalSection::ClCriticalSection%958654618.body preserve=yes
		  if (M4_TRUE == ao_bInit)
			  	InitializeCriticalSection(&m_oCriticalSection);
        //## end ClCriticalSection::ClCriticalSection%958654618.body
      }

    //## Destructor (specified)
      //## Operation: ~ClCriticalSection%958654619; C++
      ~ClCriticalSection (void )
      {
        //## begin ClCriticalSection::~ClCriticalSection%958654619.body preserve=yes
			DeleteCriticalSection(&m_oCriticalSection);
        //## end ClCriticalSection::~ClCriticalSection%958654619.body
      }


    //## Other Operations (specified)
      //## Operation: Lock%958654620; C++
      m4return_t Lock (void )
      {
        //## begin ClCriticalSection::Lock%958654620.body preserve=yes
			EnterCriticalSection(&m_oCriticalSection);

			return M4_SUCCESS;
        //## end ClCriticalSection::Lock%958654620.body
      }

      //## Operation: Unlock%958654621; C++
      m4return_t Unlock (void )
      {
        //## begin ClCriticalSection::Unlock%958654621.body preserve=yes
			LeaveCriticalSection(&m_oCriticalSection);

			return M4_SUCCESS;
        //## end ClCriticalSection::Unlock%958654621.body
      }

      //## Operation: Handle%958654622; C++
      HANDLE Handle ()
      {
        //## begin ClCriticalSection::Handle%958654622.body preserve=yes
		  return (HANDLE)&m_oCriticalSection;
        //## end ClCriticalSection::Handle%958654622.body
      }

      //## Operation: Init%958654623; C++
      m4return_t Init (void )
      {
        //## begin ClCriticalSection::Init%958654623.body preserve=yes
			InitializeCriticalSection(&m_oCriticalSection);

			return M4_SUCCESS;
        //## end ClCriticalSection::Init%958654623.body
      }

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_oCriticalSection%3923E84B006D
      //## begin ClCriticalSection::m_oCriticalSection%3923E84B006D.attr preserve=no  private: CRITICAL_SECTION {UA} 
      CRITICAL_SECTION m_oCriticalSection;
      //## end ClCriticalSection::m_oCriticalSection%3923E84B006D.attr

  private: //## implementation
};

// Class ClCriticalSection 




#endif
