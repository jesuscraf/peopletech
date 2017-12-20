//## begin module%3972CDBF01B3.cm preserve=no
//## end module%3972CDBF01B3.cm

//## begin module%3972CDBF01B3.cp preserve=no
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
//## end module%3972CDBF01B3.cp

//## Module: dspxmlreq%3972CDBF01B3; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: C:\WorkingFolder\m4dispatcher\inc\dspxmlreq.hpp

#ifndef dspxmlreq_h
#define dspxmlreq_h 1

//## begin module%3972CDBF01B3.additionalIncludes preserve=no
//## end module%3972CDBF01B3.additionalIncludes

//## begin module%3972CDBF01B3.includes preserve=yes
//## end module%3972CDBF01B3.includes

// dspreq
#include <dspreq.hpp>
//## begin module%3972CDBF01B3.declarations preserve=no
//## end module%3972CDBF01B3.declarations

//## begin module%3972CDBF01B3.additionalDeclarations preserve=yes
//## end module%3972CDBF01B3.additionalDeclarations


//## begin ClDspXMLRequest%396DB0370193.preface preserve=yes
//## end ClDspXMLRequest%396DB0370193.preface

//## Class: ClDspXMLRequest%396DB0370193
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDspXMLRequest : public ClDspRequest  //## Inherits: <unnamed>%396DB059000C
{
  //## begin ClDspXMLRequest%396DB0370193.initialDeclarations preserve=yes
  //## end ClDspXMLRequest%396DB0370193.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDspXMLRequest%963825077
      ClDspXMLRequest (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID);

    //## Destructor (specified)
      //## Operation: ~ClDspXMLRequest%963825078
      ~ClDspXMLRequest ();


    //## Other Operations (specified)
      //## Operation: BuildReqReply%963825079
      m4return_t BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen);

    // Additional Public Declarations
      //## begin ClDspXMLRequest%396DB0370193.public preserve=yes
      //## end ClDspXMLRequest%396DB0370193.public

  protected:
    // Additional Protected Declarations
      //## begin ClDspXMLRequest%396DB0370193.protected preserve=yes
      //## end ClDspXMLRequest%396DB0370193.protected

  private:
    // Additional Private Declarations
      //## begin ClDspXMLRequest%396DB0370193.private preserve=yes
      //## end ClDspXMLRequest%396DB0370193.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspXMLRequest%396DB0370193.implementation preserve=yes
      //## end ClDspXMLRequest%396DB0370193.implementation

};

//## begin ClDspXMLRequest%396DB0370193.postscript preserve=yes
//## end ClDspXMLRequest%396DB0370193.postscript

// Class ClDspXMLRequest 

//## begin module%3972CDBF01B3.epilog preserve=yes
//## end module%3972CDBF01B3.epilog


#endif
