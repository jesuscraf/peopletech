//## begin module%3A093C3A01C3.cm preserve=no
//## end module%3A093C3A01C3.cm

//## begin module%3A093C3A01C3.cp preserve=no
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
//## end module%3A093C3A01C3.cp

//## Module: dspreqcontainer%3A093C3A01C3; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: C:\WorkingFolder\m4dispatcher\inc\dspreqcontainer.hpp

#ifndef dspreqcontainer_h
#define dspreqcontainer_h 1

//## begin module%3A093C3A01C3.additionalIncludes preserve=no
//## end module%3A093C3A01C3.additionalIncludes

//## begin module%3A093C3A01C3.includes preserve=yes
//## end module%3A093C3A01C3.includes

// dspreq
#include <dspreq.hpp>
// CriticalSection
#include <criticalsection.hpp>
//## begin module%3A093C3A01C3.declarations preserve=no
//## end module%3A093C3A01C3.declarations

//## begin module%3A093C3A01C3.additionalDeclarations preserve=yes
//## end module%3A093C3A01C3.additionalDeclarations


//## begin DspReqMap%39744A8C01A6.preface preserve=yes
//## end DspReqMap%39744A8C01A6.preface

//## Class: DspReqMap%39744A8C01A6
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39744AE6032C;ClDspRequest { -> }

typedef map < m4uint64_t , ClDspRequest * , less <m4uint64_t> > DspReqMap;

//## begin DspReqMap%39744A8C01A6.postscript preserve=yes
//## end DspReqMap%39744A8C01A6.postscript

//## begin DspReqContainer%39744AEC017C.preface preserve=yes
//## end DspReqContainer%39744AEC017C.preface

//## Class: DspReqContainer%39744AEC017C
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DspReqContainer 
{
  //## begin DspReqContainer%39744AEC017C.initialDeclarations preserve=yes
  //## end DspReqContainer%39744AEC017C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DspReqContainer%963906299
      DspReqContainer ();


    //## Other Operations (specified)
      //## Operation: AddReq%963906297
      m4return_t AddReq (m4uint64_t ai_iReqID, ClDspRequest *ai_poRequest);

      //## Operation: RemoveReq%963906298
      m4return_t RemoveReq (m4uint64_t  ai_iReqID, ClDspRequest *&ao_poRequest);

    // Additional Public Declarations
      //## begin DspReqContainer%39744AEC017C.public preserve=yes
      //## end DspReqContainer%39744AEC017C.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%39744B660178
      //## Role: DspReqContainer::m_oReqMap%39744B6700A7
      //## begin DspReqContainer::m_oReqMap%39744B6700A7.role preserve=no  protected: DspReqMap { -> VHAN}
      DspReqMap m_oReqMap;
      //## end DspReqContainer::m_oReqMap%39744B6700A7.role

      //## Association: M4Dispatcher::<unnamed>%39744D220036
      //## Role: DspReqContainer::m_oSynchronizer%39744D2202E0
      //## begin DspReqContainer::m_oSynchronizer%39744D2202E0.role preserve=no  protected: ClCriticalSection { -> VHAN}
      ClCriticalSection m_oSynchronizer;
      //## end DspReqContainer::m_oSynchronizer%39744D2202E0.role

    // Additional Protected Declarations
      //## begin DspReqContainer%39744AEC017C.protected preserve=yes
      //## end DspReqContainer%39744AEC017C.protected

  private:
    // Additional Private Declarations
      //## begin DspReqContainer%39744AEC017C.private preserve=yes
      //## end DspReqContainer%39744AEC017C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DspReqContainer%39744AEC017C.implementation preserve=yes
      //## end DspReqContainer%39744AEC017C.implementation

};

//## begin DspReqContainer%39744AEC017C.postscript preserve=yes
//## end DspReqContainer%39744AEC017C.postscript

// Class DspReqContainer 

//## begin module%3A093C3A01C3.epilog preserve=yes
//## end module%3A093C3A01C3.epilog


#endif
