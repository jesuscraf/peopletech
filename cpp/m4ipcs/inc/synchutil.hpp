//## begin module%37F20B140063.cm preserve=no
//## end module%37F20B140063.cm

//## begin module%37F20B140063.cp preserve=no
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
//## end module%37F20B140063.cp

//## Module: synchutil%37F20B140063; Subprogram specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: d:\eduardoma\m4ipcs\version\inc\synchutil.hpp

#ifndef synchutil_h
#define synchutil_h 1

//## begin module%37F20B140063.additionalIncludes preserve=no
//## end module%37F20B140063.additionalIncludes

//## begin module%37F20B140063.includes preserve=yes
//## end module%37F20B140063.includes

// syncronization
#include <m4syncronization.hpp>
//## begin module%37F20B140063.declarations preserve=no
//## end module%37F20B140063.declarations

//## begin module%37F20B140063.additionalDeclarations preserve=yes
//## end module%37F20B140063.additionalDeclarations


//## begin ClBlockWritersReaders%37F208A50003.preface preserve=yes
//## end ClBlockWritersReaders%37F208A50003.preface

//## Class: ClBlockWritersReaders%37F208A50003
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClBlockWritersReaders
{
  //## begin ClBlockWritersReaders%37F208A50003.initialDeclarations preserve=yes
  //## end ClBlockWritersReaders%37F208A50003.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBlockWritersReaders%938598215; C++
      ClBlockWritersReaders (ClWriterReaders *ai_poLock);

    //## Destructor (specified)
      //## Operation: ~ClBlockWritersReaders%938598216
      ~ClBlockWritersReaders ();

    // Additional Public Declarations
      //## begin ClBlockWritersReaders%37F208A50003.public preserve=yes
      //## end ClBlockWritersReaders%37F208A50003.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%37F208FA018C
      //## Role: ClBlockWritersReaders::m_poLocker%37F208FA0377
      //## begin ClBlockWritersReaders::m_poLocker%37F208FA0377.role preserve=no  protected: ClWriterReaders { -> 1RHAN}
      ClWriterReaders *m_poLocker;
      //## end ClBlockWritersReaders::m_poLocker%37F208FA0377.role

    // Additional Protected Declarations
      //## begin ClBlockWritersReaders%37F208A50003.protected preserve=yes
      //## end ClBlockWritersReaders%37F208A50003.protected

  private:
    // Additional Private Declarations
      //## begin ClBlockWritersReaders%37F208A50003.private preserve=yes
      //## end ClBlockWritersReaders%37F208A50003.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBlockWritersReaders%37F208A50003.implementation preserve=yes
      //## end ClBlockWritersReaders%37F208A50003.implementation

};

//## begin ClBlockWritersReaders%37F208A50003.postscript preserve=yes
//## end ClBlockWritersReaders%37F208A50003.postscript

//## begin ClBlockReaders%37F20A4C0355.preface preserve=yes
//## end ClBlockReaders%37F20A4C0355.preface

//## Class: ClBlockReaders%37F20A4C0355
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClBlockReaders : public ClBlockWritersReaders  //## Inherits: <unnamed>%37F20A5801B7
{
  //## begin ClBlockReaders%37F20A4C0355.initialDeclarations preserve=yes
  //## end ClBlockReaders%37F20A4C0355.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBlockReaders%938598219; C++
      ClBlockReaders (ClWriterReaders *ai_poLock);

    //## Destructor (specified)
      //## Operation: ~ClBlockReaders%938598220
      ~ClBlockReaders ();

    // Additional Public Declarations
      //## begin ClBlockReaders%37F20A4C0355.public preserve=yes
      //## end ClBlockReaders%37F20A4C0355.public

  protected:
    // Additional Protected Declarations
      //## begin ClBlockReaders%37F20A4C0355.protected preserve=yes
      //## end ClBlockReaders%37F20A4C0355.protected

  private:
    // Additional Private Declarations
      //## begin ClBlockReaders%37F20A4C0355.private preserve=yes
      //## end ClBlockReaders%37F20A4C0355.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBlockReaders%37F20A4C0355.implementation preserve=yes
      //## end ClBlockReaders%37F20A4C0355.implementation

};

//## begin ClBlockReaders%37F20A4C0355.postscript preserve=yes
//## end ClBlockReaders%37F20A4C0355.postscript

//## begin ClBlockWriters%37F20A34026A.preface preserve=yes
//## end ClBlockWriters%37F20A34026A.preface

//## Class: ClBlockWriters%37F20A34026A
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClBlockWriters : public ClBlockWritersReaders  //## Inherits: <unnamed>%37F20A3E025A
{
  //## begin ClBlockWriters%37F20A34026A.initialDeclarations preserve=yes
  //## end ClBlockWriters%37F20A34026A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBlockWriters%938598217; C++
      ClBlockWriters (ClWriterReaders *ai_poLock);

    //## Destructor (specified)
      //## Operation: ~ClBlockWriters%938598218
      ~ClBlockWriters ();

    // Additional Public Declarations
      //## begin ClBlockWriters%37F20A34026A.public preserve=yes
      //## end ClBlockWriters%37F20A34026A.public

  protected:
    // Additional Protected Declarations
      //## begin ClBlockWriters%37F20A34026A.protected preserve=yes
      //## end ClBlockWriters%37F20A34026A.protected

  private:
    // Additional Private Declarations
      //## begin ClBlockWriters%37F20A34026A.private preserve=yes
      //## end ClBlockWriters%37F20A34026A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBlockWriters%37F20A34026A.implementation preserve=yes
      //## end ClBlockWriters%37F20A34026A.implementation

};

//## begin ClBlockWriters%37F20A34026A.postscript preserve=yes
//## end ClBlockWriters%37F20A34026A.postscript

// Class ClBlockWritersReaders 

// Class ClBlockReaders 

// Class ClBlockWriters 

//## begin module%37F20B140063.epilog preserve=yes
//## end module%37F20B140063.epilog


#endif
