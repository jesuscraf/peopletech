//## begin module%377768400156.cm preserve=no
//## end module%377768400156.cm

//## begin module%377768400156.cp preserve=no
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
//## end module%377768400156.cp

//## Module: selectimpl%377768400156; Package specification
//## Subsystem: M4Tsap::inc%37776840008D
//	d:\compon\m4tsap\version\inc
//## Source file: d:\compon\m4tsap\version\inc\selectimpl.hpp

#ifndef selectimpl_h
#define selectimpl_h 1

//## begin module%377768400156.additionalIncludes preserve=no
//## end module%377768400156.additionalIncludes

//## begin module%377768400156.includes preserve=yes





#include <m4win.hpp>
#ifndef _UNIX
#include <m4winsock.hpp>
#endif


#include <stdlib.h>



// m4types
#include <m4types.hpp>
//## end module%377768400156.includes

// tsap
#include <tsap.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377768400156.declarations preserve=no
//## end module%377768400156.declarations

//## begin module%377768400156.additionalDeclarations preserve=yes
// tsap
//#include <inc/cltsap.hpp>
// inet
//#include <inc/inet.hpp>
//#include <syncro.hpp>





class	ClTSAP;
class	ClMutex;













//## end module%377768400156.additionalDeclarations


//## begin ClTSAPSelectImpl%3777684001C5.preface preserve=yes
//## end ClTSAPSelectImpl%3777684001C5.preface

//## Class: ClTSAPSelectImpl%3777684001C5; Abstract
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37776843020E;ClTSAP { -> }
//## Uses: <unnamed>%37776843023B;m4bool_t { -> }
//## Uses: <unnamed>%377768430243;m4return_t { -> }
//## Uses: <unnamed>%377768430255;ClMutex { -> }
//## Uses: <unnamed>%377768430273;m4int8_t { -> }

class ClTSAPSelectImpl 
{
  //## begin ClTSAPSelectImpl%3777684001C5.initialDeclarations preserve=yes
  //## end ClTSAPSelectImpl%3777684001C5.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClTSAPSelectImpl%711291878; C++
      virtual ~ClTSAPSelectImpl ();


    //## Other Operations (specified)
      //## Operation: Add%-729969712; C++
      virtual void Add (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions) = 0;

      //## Operation: Remove%-202677654; C++
      virtual m4return_t Remove (ClTSAP* ai_poTSAP) = 0;

      virtual m4return_t Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions) = 0;

      //## Operation: Select%-1553871961; C++
      virtual int Select (ClMutex* ao_pMutex, unsigned long ai_Segundos = 0L, unsigned long ai_Microsegundos = 0L, m4bool_t ai_bRead = M4_TRUE, m4bool_t ai_bWrite = M4_TRUE, m4bool_t ai_bExceptions = M4_TRUE) = 0;

      //## Operation: NextRead%1852420795; C++
      virtual ClTSAP* NextRead () = 0;

      //## Operation: NextWrite%-1561453844; C++
      virtual ClTSAP* NextWrite () = 0;

      //## Operation: NextExceptions%-765018656; C++
      virtual ClTSAP* NextExceptions () = 0;

      //## Operation: IsInMask%-174998406; C++
      virtual m4bool_t IsInMask (ClTSAP* ai_poTSAP, m4int8_t ai_iTipoMascara) = 0;

    // Additional Public Declarations
      //## begin ClTSAPSelectImpl%3777684001C5.public preserve=yes
      //## end ClTSAPSelectImpl%3777684001C5.public

  protected:
    // Additional Protected Declarations
      //## begin ClTSAPSelectImpl%3777684001C5.protected preserve=yes
      //## end ClTSAPSelectImpl%3777684001C5.protected

  private:
    // Additional Private Declarations
      //## begin ClTSAPSelectImpl%3777684001C5.private preserve=yes
      //## end ClTSAPSelectImpl%3777684001C5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTSAPSelectImpl%3777684001C5.implementation preserve=yes
      //## end ClTSAPSelectImpl%3777684001C5.implementation

};

//## begin ClTSAPSelectImpl%3777684001C5.postscript preserve=yes
//## end ClTSAPSelectImpl%3777684001C5.postscript

// Class ClTSAPSelectImpl 

inline ClTSAPSelectImpl::~ClTSAPSelectImpl ()
{
  //## begin ClTSAPSelectImpl::~ClTSAPSelectImpl%711291878.body preserve=yes
  //## end ClTSAPSelectImpl::~ClTSAPSelectImpl%711291878.body
}


//## begin module%377768400156.epilog preserve=yes
//## end module%377768400156.epilog


#endif
