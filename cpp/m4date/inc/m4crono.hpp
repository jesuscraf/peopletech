//## begin module%37F86299018A.cm preserve=no
//## end module%37F86299018A.cm

//## begin module%37F86299018A.cp preserve=no
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
//## end module%37F86299018A.cp

//## Module: m4crono%37F86299018A; Subprogram specification
//## Subsystem: M4Date::inc%3779DEF502DA
//	d:\compon\m4date\version\inc
//## Source file: d:\eduardoma\m4date\version\inc\m4crono.hpp

#ifndef m4crono_h
#define m4crono_h 1

//## begin module%37F86299018A.additionalIncludes preserve=no
//## end module%37F86299018A.additionalIncludes

//## begin module%37F86299018A.includes preserve=yes
//## end module%37F86299018A.includes

// m4clock
#include <m4clock.hpp>
//## begin module%37F86299018A.declarations preserve=no
//## end module%37F86299018A.declarations

//## begin module%37F86299018A.additionalDeclarations preserve=yes
#include <m4date_dll.hpp>
//## end module%37F86299018A.additionalDeclarations


//## begin ICrono%37F85E55028E.preface preserve=yes
//## end ICrono%37F85E55028E.preface

//## Class: ICrono%37F85E55028E
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F8636303BB;m4millisec_t { -> }

class M4_DECL_M4DATE ICrono 
{
  //## begin ICrono%37F85E55028E.initialDeclarations preserve=yes
  //## end ICrono%37F85E55028E.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Start%939027152; C++
      //	Starts the crono.
      virtual void Start () = 0;

      //## Operation: Resume%939027161
      virtual void Resume () = 0;

      //## Operation: Stop%939027154; C++
      //	Stops and initializes the crono. Returns the ellapsed time.
      virtual m4millisec_t Stop () = 0;

      //## Operation: GetEllapsedTime%939027155
      //	Returns the ellapsed time without pausing the crono.
      virtual m4millisec_t GetEllapsedTime () const = 0;

    // Additional Public Declarations
      //## begin ICrono%37F85E55028E.public preserve=yes
      //## end ICrono%37F85E55028E.public

  protected:
    // Additional Protected Declarations
      //## begin ICrono%37F85E55028E.protected preserve=yes
      //## end ICrono%37F85E55028E.protected

  private:
    // Additional Private Declarations
      //## begin ICrono%37F85E55028E.private preserve=yes
      //## end ICrono%37F85E55028E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ICrono%37F85E55028E.implementation preserve=yes
      //## end ICrono%37F85E55028E.implementation

};

//## begin ICrono%37F85E55028E.postscript preserve=yes
//## end ICrono%37F85E55028E.postscript

//## begin ClCrono%37F4699A0378.preface preserve=yes
//## end ClCrono%37F4699A0378.preface

//## Class: ClCrono%37F4699A0378
//	This class offers a basic crono funtionality.
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F488DE01B5;ClPrecisionTicker { -> }

class M4_DECL_M4DATE  ClCrono : public ICrono  //## Inherits: <unnamed>%37F8609C0214
{
  //## begin ClCrono%37F4699A0378.initialDeclarations preserve=yes
  //## end ClCrono%37F4699A0378.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCrono%938765143
      ClCrono ();


    //## Other Operations (specified)
      //## Operation: GetDifference%938765146
      m4millisec_t GetDifference () const;

      //## Operation: GetClockDelay%938779152; C++
      m4millisec_t GetClockDelay ();

      //## Operation: Start%939027160; C++
      //	Starts the crono.
      void Start ();

      //## Operation: Resume%939027162
      void Resume ();

      //## Operation: Stop%939027158; C++
      //	Stops and initializes the crono. Returns the ellapsed time.
      m4millisec_t Stop ();

      //## Operation: GetEllapsedTime%939027159
      //	Returns the ellapsed time without pausing the crono.
      m4millisec_t GetEllapsedTime () const;

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%37F47552025A
      //## Role: ClCrono::m_iClockDelay%37F47555018D
      //## begin ClCrono::m_iClockDelay%37F47555018D.role preserve=no  public: m4millisec_t { -> 1VHAN}
      m4millisec_t m_iClockDelay;
      //## end ClCrono::m_iClockDelay%37F47555018D.role

    // Additional Public Declarations
      //## begin ClCrono%37F4699A0378.public preserve=yes
      //## end ClCrono%37F4699A0378.public

  protected:
    // Data Members for Associations

      //## Association: M4Date::<unnamed>%37F46ACA0112
      //## Role: ClCrono::m_iStartMilliseconds%37F46ACB0101
      //## begin ClCrono::m_iStartMilliseconds%37F46ACB0101.role preserve=no  protected: m4millisec_t { -> 1VHAN}
      m4millisec_t m_iStartMilliseconds;
      //## end ClCrono::m_iStartMilliseconds%37F46ACB0101.role

      //## Association: M4Date::<unnamed>%37F86D60001F
      //## Role: ClCrono::m_iEllapsedMilliseconds%37F86D600213
      //## begin ClCrono::m_iEllapsedMilliseconds%37F86D600213.role preserve=no  protected: m4millisec_t { -> 1VHAN}
      m4millisec_t m_iEllapsedMilliseconds;
      //## end ClCrono::m_iEllapsedMilliseconds%37F86D600213.role

    // Additional Protected Declarations
      //## begin ClCrono%37F4699A0378.protected preserve=yes
      //## end ClCrono%37F4699A0378.protected

  private:
    // Additional Private Declarations
      //## begin ClCrono%37F4699A0378.private preserve=yes
      //## end ClCrono%37F4699A0378.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCrono%37F4699A0378.implementation preserve=yes
      //## end ClCrono%37F4699A0378.implementation

};

//## begin ClCrono%37F4699A0378.postscript preserve=yes
//## end ClCrono%37F4699A0378.postscript

// Class ICrono 

// Class ClCrono 


//## Other Operations (inline)
inline void ClCrono::Start ()
{
  //## begin ClCrono::Start%939027160.body preserve=yes
	m_iEllapsedMilliseconds = 0;
	m_iStartMilliseconds = ClPrecisionTicker::GetTimeTick();
  //## end ClCrono::Start%939027160.body
}

inline void ClCrono::Resume ()
{
  //## begin ClCrono::Resume%939027162.body preserve=yes
	if (0 == m_iStartMilliseconds)
	{
		m_iStartMilliseconds = ClPrecisionTicker::GetTimeTick();
	}
  //## end ClCrono::Resume%939027162.body
}

inline m4millisec_t ClCrono::Stop ()
{
  //## begin ClCrono::Stop%939027158.body preserve=yes
	if (0 != m_iStartMilliseconds)
	{
		m_iEllapsedMilliseconds += ClPrecisionTicker::GetTimeTick() - m_iStartMilliseconds;
		m_iStartMilliseconds = 0;
	}

	return m_iEllapsedMilliseconds;
  //## end ClCrono::Stop%939027158.body
}

inline m4millisec_t ClCrono::GetEllapsedTime () const
{
  //## begin ClCrono::GetEllapsedTime%939027159.body preserve=yes
	m4millisec_t		iTotalEllapsed = 0;

	if (0 != m_iStartMilliseconds)
	{
		iTotalEllapsed = ClPrecisionTicker::GetTimeTick() - m_iStartMilliseconds;
	}
	iTotalEllapsed += m_iEllapsedMilliseconds;

	return iTotalEllapsed;
  //## end ClCrono::GetEllapsedTime%939027159.body
}

//## begin module%37F86299018A.epilog preserve=yes
//## end module%37F86299018A.epilog


#endif
