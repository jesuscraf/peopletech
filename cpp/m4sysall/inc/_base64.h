//## begin module%3912C5830148.cm preserve=no
//## end module%3912C5830148.cm

//## begin module%3912C5830148.cp preserve=no
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
//## end module%3912C5830148.cp

//## Module: _base64%3912C5830148; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\new\m4sysall\inc\_base64.h

#ifndef AFX__BASE64_H__951C8215_FA2E_11D1_95FD_00C04FB7147A__INCLUDED_
#define AFX__BASE64_H__951C8215_FA2E_11D1_95FD_00C04FB7147A__INCLUDED_ 1

//## begin module%3912C5830148.additionalIncludes preserve=no
//## end module%3912C5830148.additionalIncludes

//## begin module%3912C5830148.includes preserve=yes

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "m4sysall_dll.hpp"
#include "m4define.hpp"
//## end module%3912C5830148.includes

//## begin module%3912C5830148.declarations preserve=no
//## end module%3912C5830148.declarations

//## begin module%3912C5830148.additionalDeclarations preserve=yes
//## end module%3912C5830148.additionalDeclarations


//## begin byte%3912C5830183.preface preserve=yes
//## end byte%3912C5830183.preface

//## Class: byte%3912C5830183
//	Byte type
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef unsigned char byte;

//## begin byte%3912C5830183.postscript preserve=yes
//## end byte%3912C5830183.postscript

//## begin C_Base64%3912C5830152.preface preserve=yes
//## end C_Base64%3912C5830152.preface

//## Class: C_Base64%3912C5830152
//	Byte type
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL C_Base64 
{
  //## begin C_Base64%3912C5830152.initialDeclarations preserve=yes
  //## end C_Base64%3912C5830152.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: C_Base64%-1127064404; C++
      //	Byte type
      //	////////////////////////////////////////////////////////////////////
      //	Construction/Destruction
      //	////////////////////////////////////////////////////////////////////
      C_Base64 ();

    //## Destructor (specified)
      //## Operation: ~C_Base64%-1393573843; C++
      //	-- {AddDecl: 111} region.unprotectedFunction [1677..1709]
      virtual ~C_Base64 ();


    //## Other Operations (specified)
      //## Operation: Decode%-1539693195; C++
      //	-- {AddDecl: 113} region.unprotectedFunction [3545..4945]
      short Decode (char* input, char*& output, long* length);

      //## Operation: Code%-1270952636; C++
      //	-- {AddDecl: 112} region.unprotectedFunction [1710..3544]
      short Code (char* input, long length, char*& output);

      //## Operation: FreeBuffer%708457139; C++
      //	release buffer allocated in code/decode
      //	-- {AddDecl: 114} region.unprotectedFunction [4946..5072]
      void FreeBuffer (char* input);

    // Additional Public Declarations
      //## begin C_Base64%3912C5830152.public preserve=yes
      //## end C_Base64%3912C5830152.public

  protected:
    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C5830184
      //## Role: C_Base64::decodeTable%3912C5830186
      //	Decode table
      //## begin C_Base64::decodeTable%3912C5830186.role preserve=no  protected: byte {1 -> 256VHAPN}
      byte decodeTable[256];
      //## end C_Base64::decodeTable%3912C5830186.role

      //## Association: m4sysall::<unnamed>%3912C5830187
      //## Role: C_Base64::encodeTable%3912C5830189
      //	Encode table
      //## begin C_Base64::encodeTable%3912C5830189.role preserve=no  protected: byte {1 -> 256VHAPN}
      byte encodeTable[256];
      //## end C_Base64::encodeTable%3912C5830189.role

    // Additional Protected Declarations
      //## begin C_Base64%3912C5830152.protected preserve=yes
      //## end C_Base64%3912C5830152.protected

  private:
    // Additional Private Declarations
      //## begin C_Base64%3912C5830152.private preserve=yes
      //## end C_Base64%3912C5830152.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin C_Base64%3912C5830152.implementation preserve=yes
      //## end C_Base64%3912C5830152.implementation

};

//## begin C_Base64%3912C5830152.postscript preserve=yes
//## end C_Base64%3912C5830152.postscript

// Class C_Base64 

//## begin module%3912C5830148.epilog preserve=yes
//## end module%3912C5830148.epilog


#endif
