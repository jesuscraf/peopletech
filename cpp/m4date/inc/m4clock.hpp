//## begin module%37F46B05033E.cm preserve=no
//## end module%37F46B05033E.cm

//## begin module%37F46B05033E.cp preserve=no
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
//## end module%37F46B05033E.cp

//## Module: m4clock%37F46B05033E; Subprogram specification
//## Subsystem: M4Date::inc%3779DEF502DA
//	d:\compon\m4date\version\inc
//## Source file: d:\eduardoma\m4date\version\inc\m4clock.hpp

#ifndef m4clock_h
#define m4clock_h 1

//## begin module%37F46B05033E.additionalIncludes preserve=no
//## end module%37F46B05033E.additionalIncludes

//## begin module%37F46B05033E.includes preserve=yes
#include <m4date_dll.hpp>
#include "m4stl.hpp"
//## end module%37F46B05033E.includes

// m4types
#include <m4types.hpp>
//## begin module%37F46B05033E.declarations preserve=no
//## end module%37F46B05033E.declarations

//## begin module%37F46B05033E.additionalDeclarations preserve=yes
//## end module%37F46B05033E.additionalDeclarations


//## Class: m4millisec_t%37F469D003E4
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F4707702A2;m4uint64_t { -> }

typedef m4int64_t m4millisec_t;

//## begin ClPrecisionTicker%37F4878A03A1.preface preserve=yes
//## end ClPrecisionTicker%37F4878A03A1.preface

//## Class: ClPrecisionTicker%37F4878A03A1; Class Utility
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DATE ClPrecisionTicker 
{
  //## begin ClPrecisionTicker%37F4878A03A1.initialDeclarations preserve=yes
  //## end ClPrecisionTicker%37F4878A03A1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPrecisionTicker%938765156
      ClPrecisionTicker ();


    //## Other Operations (specified)
      //## Operation: GetTimeTick%938765152
      static m4millisec_t GetTimeTick ();

      //## Operation: QueryClockDelay%938765155; C++
      static m4millisec_t QueryClockDelay ();

      //## Operation: GetAbsDifference%938765153; C++
      static m4millisec_t GetAbsDifference (m4millisec_t ai_iFirstTime, m4millisec_t ai_iLastTime);

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%37F478F502DA
      //## Role: ClPrecisionTicker::s_bHighPerformance%37F478F503C1
      //## begin ClPrecisionTicker::s_bHighPerformance%37F478F503C1.role preserve=yes  public: static m4bool_t { -> 1VHAN}
#ifdef _WINDOWS
      static m4bool_t s_bHighPerformance;
#endif
      //## end ClPrecisionTicker::s_bHighPerformance%37F478F503C1.role

      //## Association: M4Date::<unnamed>%37F487EC0397
      //## Role: ClPrecisionTicker::s_iPrecision%37F487ED01CD
      //## begin ClPrecisionTicker::s_iPrecision%37F487ED01CD.role preserve=yes  public: m4millisec_t { -> 1VHAN}
#ifdef _WINDOWS
      static m4millisec_t s_iPrecision;
#endif
      //## end ClPrecisionTicker::s_iPrecision%37F487ED01CD.role

    // Additional Public Declarations
      //## begin ClPrecisionTicker%37F4878A03A1.public preserve=yes
      //## end ClPrecisionTicker%37F4878A03A1.public

  protected:
    // Additional Protected Declarations
      //## begin ClPrecisionTicker%37F4878A03A1.protected preserve=yes
      //## end ClPrecisionTicker%37F4878A03A1.protected

  private:
    // Data Members for Associations

      //## Association: M4Date::<unnamed>%37F48B140277
      //## Role: ClPrecisionTicker::s_isInit%37F48B1500CB
      //## begin ClPrecisionTicker::s_isInit%37F48B1500CB.role preserve=no  private: m4bool_t { -> 1VHAN}
      static m4bool_t s_isInit;
      //## end ClPrecisionTicker::s_isInit%37F48B1500CB.role

    // Additional Private Declarations
      //## begin ClPrecisionTicker%37F4878A03A1.private preserve=yes
      //## end ClPrecisionTicker%37F4878A03A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPrecisionTicker%37F4878A03A1.implementation preserve=yes
      //## end ClPrecisionTicker%37F4878A03A1.implementation

};

//## begin ClPrecisionTicker%37F4878A03A1.postscript preserve=yes
//## end ClPrecisionTicker%37F4878A03A1.postscript

// Class Utility ClPrecisionTicker 


//## Other Operations (inline)
inline m4millisec_t ClPrecisionTicker::GetAbsDifference (m4millisec_t ai_iFirstTime, m4millisec_t ai_iLastTime)
{
  //## begin ClPrecisionTicker::GetAbsDifference%938765153.body preserve=yes
	m4millisec_t		iDifference;
  
	/* Bug 0184652
	Se devuelve la diferencia sin sumar posibles errores
	*/
	iDifference = ai_iLastTime - ai_iFirstTime;

	return iDifference;
  //## end ClPrecisionTicker::GetAbsDifference%938765153.body
}

//## begin module%37F46B05033E.epilog preserve=yes
//## end module%37F46B05033E.epilog


#endif
