//## begin module%3991771201EF.cm preserve=no
//## end module%3991771201EF.cm

//## begin module%3991771201EF.cp preserve=no
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
//## end module%3991771201EF.cp

//## Module: CommBin%3991771201EF; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commbin.hpp

#ifndef commbin_h
#define commbin_h 1

//## begin module%3991771201EF.additionalIncludes preserve=no
//## end module%3991771201EF.additionalIncludes

//## begin module%3991771201EF.includes preserve=yes
//## end module%3991771201EF.includes

// m4types
#include <m4types.hpp>
//## begin module%3991771201EF.declarations preserve=no
//## end module%3991771201EF.declarations

//## begin module%3991771201EF.additionalDeclarations preserve=yes
#define M4_COMM_WRITTING_TASK			0
#define M4_COMM_READING_TASK			1
#define M4_COMM_OPENPORT_TASK			2
#define M4_COMM_CLOSEPORT_TASK			3
#define M4_COMM_OPENSOCKET_TASK			4
#define M4_COMM_CLOSESOCKET_TASK		5
#define M4_COMM_SHUTDOWN_TASK			6
#define M4_COMM_STOPREADING_TASK		7

//## end module%3991771201EF.additionalDeclarations


//## begin ClCommRecyclable%399174ED02E0.preface preserve=yes
//## end ClCommRecyclable%399174ED02E0.preface

//## Class: ClCommRecyclable%399174ED02E0
//## Category: M4Communication::M4COML::Recycler%399174C30311
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommRecyclable 
{
  //## begin ClCommRecyclable%399174ED02E0.initialDeclarations preserve=yes
  //## end ClCommRecyclable%399174ED02E0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommRecyclable%967625198; C++
      ClCommRecyclable (m4int_t ai_iRecycleObjID = -1);

    //## Destructor (specified)
      //## Operation: ~ClCommRecyclable%967625196; C++
      virtual ~ClCommRecyclable ();


    //## Other Operations (specified)
      //## Operation: Recycle%965834589; C++
      virtual m4return_t Recycle () = 0;

      //## Operation: GetRecycleObjID%967625197
      m4int_t GetRecycleObjID ();

    // Additional Public Declarations
      //## begin ClCommRecyclable%399174ED02E0.public preserve=yes
      //## end ClCommRecyclable%399174ED02E0.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommRecyclable%399174ED02E0.protected preserve=yes
      //## end ClCommRecyclable%399174ED02E0.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iRecyclableObjID%39ACD2DF0175
      //## begin ClCommRecyclable::m_iRecyclableObjID%39ACD2DF0175.attr preserve=no  private: m4int_t {UA} -1
      m4int_t m_iRecyclableObjID;
      //## end ClCommRecyclable::m_iRecyclableObjID%39ACD2DF0175.attr

    // Additional Private Declarations
      //## begin ClCommRecyclable%399174ED02E0.private preserve=yes
      //## end ClCommRecyclable%399174ED02E0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommRecyclable%399174ED02E0.implementation preserve=yes
      //## end ClCommRecyclable%399174ED02E0.implementation

};

//## begin ClCommRecyclable%399174ED02E0.postscript preserve=yes
//## end ClCommRecyclable%399174ED02E0.postscript

//## begin ClCommBin%399175B70308.preface preserve=yes
//## end ClCommBin%399175B70308.preface

//## Class: ClCommBin%399175B70308
//## Category: M4Communication::M4COML::Recycler%399174C30311
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%399176220031;ClCommRecyclable { -> }
//## Uses: <unnamed>%3991781F006F;m4return_t { -> }
//## Uses: <unnamed>%39917B4D028C;ClCommReadingTask { -> F}
//## Uses: <unnamed>%39917B5002AF;ClCommWrittingTask { -> F}

class ClCommBin 
{
  //## begin ClCommBin%399175B70308.initialDeclarations preserve=yes
  //## end ClCommBin%399175B70308.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Get%965834587; C++
      ClCommRecyclable * Get (m4int_t ai_iObjectType);

      //## Operation: Release%965834588; C++
      m4return_t Release (ClCommRecyclable *ai_pRecyclable);

    // Additional Public Declarations
      //## begin ClCommBin%399175B70308.public preserve=yes
      //## end ClCommBin%399175B70308.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommBin%399175B70308.protected preserve=yes
      //## end ClCommBin%399175B70308.protected

  private:
    // Additional Private Declarations
      //## begin ClCommBin%399175B70308.private preserve=yes
      //## end ClCommBin%399175B70308.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommBin%399175B70308.implementation preserve=yes
      //## end ClCommBin%399175B70308.implementation

};

//## begin ClCommBin%399175B70308.postscript preserve=yes
//## end ClCommBin%399175B70308.postscript

// Class ClCommRecyclable 

// Class ClCommBin 

//## begin module%3991771201EF.epilog preserve=yes
//## end module%3991771201EF.epilog


#endif
