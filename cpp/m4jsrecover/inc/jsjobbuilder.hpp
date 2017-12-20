//## begin module%37E60334016F.cm preserve=no
//## end module%37E60334016F.cm

//## begin module%37E60334016F.cp preserve=no
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
//## end module%37E60334016F.cp

//## Module: jsjobbuilder%37E60334016F; Package specification
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Source file: C:\Trabajo\Server\m4jsrecover\version\inc\jsjobbuilder.hpp

#ifndef jsjobbuilder_h
#define jsjobbuilder_h 1

//## begin module%37E60334016F.additionalIncludes preserve=no
//## end module%37E60334016F.additionalIncludes

//## begin module%37E60334016F.includes preserve=yes
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module%37E60334016F.includes

// jsjob
#include <jsjob.hpp>
// jscompoundjob
#include <jscompoundjob.hpp>
// jssimplejob
#include <jssimplejob.hpp>
//## begin module%37E60334016F.declarations preserve=no
//## end module%37E60334016F.declarations

//## begin module%37E60334016F.additionalDeclarations preserve=yes
//## end module%37E60334016F.additionalDeclarations


//## begin ProductList%3815DB550338.preface preserve=yes
//## end ProductList%3815DB550338.preface

//## Class: ProductList%3815DB550338; Instantiated Class
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3815DB9000A7;ClJSJob { -> }

typedef list< ClJSJob * > ProductList;

//## begin ProductList%3815DB550338.postscript preserve=yes
//## end ProductList%3815DB550338.postscript

//## begin ClJSJobBuilder%3767ACF5032E.preface preserve=yes
//## end ClJSJobBuilder%3767ACF5032E.preface

//## Class: ClJSJobBuilder%3767ACF5032E
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: builds%37E600AA00C0;protected: ClJSJob {1..1 -> 1..1}
//## Uses: <unnamed>%380AF99000E3;ClJSCompoundJob { -> }
//## Uses: <unnamed>%380AFAA3002A;ClJSSimpleJob { -> }

class ClJSJobBuilder 
{
  //## begin ClJSJobBuilder%3767ACF5032E.initialDeclarations preserve=yes
  //## end ClJSJobBuilder%3767ACF5032E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSJobBuilder%941016332
      ClJSJobBuilder ();

    //## Destructor (specified)
      //## Operation: ~ClJSJobBuilder%941120254
      ~ClJSJobBuilder ();


    //## Other Operations (specified)
      //## Operation: GetpProduct%939977800
      ClJSJob * GetpProduct ();

      //## Operation: GetRemainingJobs%941016330
      m4return_t GetRemainingJobs (m4uint_t &ao_uiNumJobs);

      //## Operation: GetNumberOfProducts%941016331
      m4return_t GetNumberOfProducts (m4uint_t &ao_uiNumJobs);

      //## Operation: ReadScheduledJobs%941096723
      virtual m4return_t ReadScheduledJobs (m4uint_t &ai_uiMaxNumJobs) = 0;

      //## Operation: ReadCancelledJobs%941096724
      virtual m4return_t ReadCancelledJobs () = 0;

      //## Operation: CleanProductList%941120253
      m4return_t CleanProductList ();

    // Additional Public Declarations
      //## begin ClJSJobBuilder%3767ACF5032E.public preserve=yes
      //## end ClJSJobBuilder%3767ACF5032E.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiRemainingJobs%3816C96802F3
      //## begin ClJSJobBuilder::m_uiRemainingJobs%3816C96802F3.attr preserve=no  protected: m4uint_t {UA} 
      m4uint_t m_uiRemainingJobs;
      //## end ClJSJobBuilder::m_uiRemainingJobs%3816C96802F3.attr

    // Data Members for Associations

      //## Association: M4JSRecover::<unnamed>%3815DB9803DE
      //## Role: ClJSJobBuilder::m_lProducts%3815DB9902C7
      //## begin ClJSJobBuilder::m_lProducts%3815DB9902C7.role preserve=no  protected: ProductList { -> VHAN}
      ProductList m_lProducts;
      //## end ClJSJobBuilder::m_lProducts%3815DB9902C7.role

    // Additional Protected Declarations
      //## begin ClJSJobBuilder%3767ACF5032E.protected preserve=yes
      //## end ClJSJobBuilder%3767ACF5032E.protected

  private:
    // Additional Private Declarations
      //## begin ClJSJobBuilder%3767ACF5032E.private preserve=yes
      //## end ClJSJobBuilder%3767ACF5032E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSJobBuilder%3767ACF5032E.implementation preserve=yes
      //## end ClJSJobBuilder%3767ACF5032E.implementation

};

//## begin ClJSJobBuilder%3767ACF5032E.postscript preserve=yes
//## end ClJSJobBuilder%3767ACF5032E.postscript

// Class ClJSJobBuilder 

//## begin module%37E60334016F.epilog preserve=yes
//## end module%37E60334016F.epilog


#endif
