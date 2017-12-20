//## begin module%39DA029602CB.cm preserve=no
//## end module%39DA029602CB.cm

//## begin module%39DA029602CB.cp preserve=no
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
//## end module%39DA029602CB.cp

//## Module: xmlrequest%39DA029602CB; Package specification
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Source file: F:\integration\m4comsrv\inc\xmlrequest.hpp

#ifndef xmlrequest_h
#define xmlrequest_h 1

//## begin module%39DA029602CB.additionalIncludes preserve=no
//## end module%39DA029602CB.additionalIncludes

//## begin module%39DA029602CB.includes preserve=yes
//## end module%39DA029602CB.includes

// clbasedictionary
#include <clbasedictionary.hpp>
// clbaseobject
#include <clbaseobject.hpp>
//## begin module%39DA029602CB.declarations preserve=no
//## end module%39DA029602CB.declarations

//## begin module%39DA029602CB.additionalDeclarations preserve=yes
//## end module%39DA029602CB.additionalDeclarations


//## begin ClXMLRequest%39D9C37B0126.preface preserve=yes
//## end ClXMLRequest%39D9C37B0126.preface

//## Class: ClXMLRequest%39D9C37B0126
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClXMLRequest : public ClBaseObject  //## Inherits: <unnamed>%39DA02C40096
{
  //## begin ClXMLRequest%39D9C37B0126.initialDeclarations preserve=yes
  //## end ClXMLRequest%39D9C37B0126.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClXMLRequest%970572583
      ClXMLRequest (m4pchar_t ai_pBuffer);

    //## Destructor (specified)
      //## Operation: ~ClXMLRequest%970572584
      ~ClXMLRequest ();


    //## Other Operations (specified)
      //## Operation: CanDestroy%970572585
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

      //## Operation: CanRecycle%970572586
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%970572587
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: Recycle%970572588; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

    // Additional Public Declarations
      //## begin ClXMLRequest%39D9C37B0126.public preserve=yes
      //## end ClXMLRequest%39D9C37B0126.public

  protected:
    // Additional Protected Declarations
      //## begin ClXMLRequest%39D9C37B0126.protected preserve=yes
      //## end ClXMLRequest%39D9C37B0126.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pBuffer%39DA074303DA
      //## begin ClXMLRequest::m_pBuffer%39DA074303DA.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pBuffer;
      //## end ClXMLRequest::m_pBuffer%39DA074303DA.attr

    // Additional Private Declarations
      //## begin ClXMLRequest%39D9C37B0126.private preserve=yes
      //## end ClXMLRequest%39D9C37B0126.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClXMLRequest%39D9C37B0126.implementation preserve=yes
      //## end ClXMLRequest%39D9C37B0126.implementation

};

//## begin ClXMLRequest%39D9C37B0126.postscript preserve=yes
//## end ClXMLRequest%39D9C37B0126.postscript

//## begin ClXMLRequestDic%39D9C3B90144.preface preserve=yes
//## end ClXMLRequestDic%39D9C3B90144.preface

//## Class: ClXMLRequestDic%39D9C3B90144
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39D9C4B303E2;ClXMLRequest { -> }
//## Uses: <unnamed>%39DA03FD01EB;ClBaseDictionary { -> }

typedef ClBaseDictionary ClXMLRequestDic;

//## begin ClXMLRequestDic%39D9C3B90144.postscript preserve=yes
//## end ClXMLRequestDic%39D9C3B90144.postscript

// Class ClXMLRequest 

//## begin module%39DA029602CB.epilog preserve=yes
//## end module%39DA029602CB.epilog


#endif
