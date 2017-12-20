//## begin module%39085DDB0211.cm preserve=no
//## end module%39085DDB0211.cm

//## begin module%39085DDB0211.cp preserve=no
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
//## end module%39085DDB0211.cp

//## Module: M4SAXAdmClient%39085DDB0211; Package specification
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Source file: E:\INTEGRATION\m4xmladminclient\inc\m4saxadmclient.hpp

#ifndef m4saxadmclient_h
#define m4saxadmclient_h 1

//## begin module%39085DDB0211.additionalIncludes preserve=no
//## end module%39085DDB0211.additionalIncludes

//## begin module%39085DDB0211.includes preserve=yes
//## end module%39085DDB0211.includes

#include "m4stl.hpp"
// m4xmlbase
#include <m4xmlbase.hpp>
//## begin module%39085DDB0211.declarations preserve=no
//## end module%39085DDB0211.declarations

//## begin module%39085DDB0211.additionalDeclarations preserve=yes
#include "m4xmladminclient_dll.hpp"
//## end module%39085DDB0211.additionalDeclarations


//## begin M4SAXAdmClient%39085A93030D.preface preserve=yes
//## end M4SAXAdmClient%39085A93030D.preface

//## Class: M4SAXAdmClient%39085A93030D
//## Category: M4XMLAdminClient%39084C2003A4
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3912D45A00FC;basic_ostringstream { -> }

class M4_DECL_M4XMLADMINCLIENT M4SAXAdmClient : public M4SAXHandlerBase  //## Inherits: <unnamed>%39085AEB02EC
{
  //## begin M4SAXAdmClient%39085A93030D.initialDeclarations preserve=yes
  //## end M4SAXAdmClient%39085A93030D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SAXAdmClient%956910565
      M4SAXAdmClient ();

    //## Destructor (specified)
      //## Operation: ~M4SAXAdmClient%956910566
      ~M4SAXAdmClient ();


    //## Other Operations (specified)
      //## Operation: ProcCommand%956910560
      void ProcCommand (const XMLCh* const ai_oName, AttributeList* ai_oAttributes);

      //## Operation: ProcConfiguration%956910561
      void ProcConfiguration (const XMLCh* const ai_oName, AttributeList* ai_oAttributes);

      //## Operation: ProcAttValue%956910564
      void ProcAttValue (const XMLCh *ai_szName, const XMLCh *ai_szValue);
	  
      //## Operation: GetOutStream%960193451
      m4return_t GetOutStream (char *aio_strBuffer, long ai_lBufferSize);

      //## Operation: GetOutStreamSize%961674443
      long GetOutStreamSize ();

      //## Operation: ProcParams%1003250411
      void ProcParams (const XMLCh* const ai_oName, AttributeList* ai_oAttributes);

    // Additional Public Declarations
      //## begin M4SAXAdmClient%39085A93030D.public preserve=yes
      //## end M4SAXAdmClient%39085A93030D.public

  protected:
    // Additional Protected Declarations
      //## begin M4SAXAdmClient%39085A93030D.protected preserve=yes
      //## end M4SAXAdmClient%39085A93030D.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oOutStream%39127DC8016D
      //## begin M4SAXAdmClient::m_oOutStream%39127DC8016D.attr preserve=no  private: strstream {UA} 
      strstream m_oOutStream;
      //## end M4SAXAdmClient::m_oOutStream%39127DC8016D.attr

    // Additional Private Declarations
      //## begin M4SAXAdmClient%39085A93030D.private preserve=yes
      //## end M4SAXAdmClient%39085A93030D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SAXAdmClient%39085A93030D.implementation preserve=yes
      //## end M4SAXAdmClient%39085A93030D.implementation

};

//## begin M4SAXAdmClient%39085A93030D.postscript preserve=yes
//## end M4SAXAdmClient%39085A93030D.postscript

// Class M4SAXAdmClient 

//## begin module%39085DDB0211.epilog preserve=yes
//## end module%39085DDB0211.epilog


#endif
