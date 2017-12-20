//## begin module%3911AA2401F7.cm preserve=no
//## end module%3911AA2401F7.cm

//## begin module%3911AA2401F7.cp preserve=no
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
//## end module%3911AA2401F7.cp

//## Module: cltadof%3911AA2401F7; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\cltadof.hpp

#ifndef cltadof_h
#define cltadof_h 1

//## begin module%3911AA2401F7.additionalIncludes preserve=no
//## end module%3911AA2401F7.additionalIncludes

//## begin module%3911AA2401F7.includes preserve=yes

//#define _HASH_

#include "m4types.hpp"
#include "m4stl.hpp"

#ifdef _HASH_
#include <hashmap>
#endif

#ifndef _HASH_
#include "m4stl.hpp"
#endif
//## end module%3911AA2401F7.includes

//## begin module%3911AA2401F7.declarations preserve=no
//## end module%3911AA2401F7.declarations

//## begin module%3911AA2401F7.additionalDeclarations preserve=yes


const m4uint32_t INDEX_BY_ADDRESS = 0;
const m4uint32_t INDEX_BY_OFFSET  = 1;
const m4uint32_t TAO_OFFSET_NULL  = 2000000000; // Por peticion de los JAVA boys...


const m4uint32_t MAX              = 50000;

//## end module%3911AA2401F7.additionalDeclarations


//## begin ClTableAddressOffset%3911AA2401F6.preface preserve=yes
//## end ClTableAddressOffset%3911AA2401F6.preface

//## Class: ClTableAddressOffset%3911AA2401F6
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTableAddressOffset 
{
  //## begin ClTableAddressOffset%3911AA2401F6.initialDeclarations preserve=yes
  //## end ClTableAddressOffset%3911AA2401F6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTableAddressOffset%672414148; C++
      //	-- {AddDecl: 524} region.unprotectedFunction [749..878]
      ClTableAddressOffset (m4uint32_t ai_iMode);

    //## Destructor (specified)
      //## Operation: ~ClTableAddressOffset%-310045900; C++
      //	-- {AddDecl: 525} region.unprotectedFunction [879..929]
      ~ClTableAddressOffset ();


    //## Other Operations (specified)
      //## Operation: GetOffset%-1068731964; C++
      //	-- {AddDecl: 526} region.unprotectedFunction [930..1332]
      m4uint32_t GetOffset (void* ai_pPtr);

      //## Operation: GetAddress%154562017; C++
      //	Returns true if exist previously
      //	-- {AddDecl: 527} region.unprotectedFunction [1333..1761]
      void* GetAddress (m4uint32_t ai_iOffset);

      //## Operation: Add%-1442246846; C++
      //	Returns true if exist previously
      //	-- {AddDecl: 528} region.unprotectedFunction [1762..2031]
      m4bool_t Add (void* ai_pPtr, const m4uint32_t ai_iOffset);

      //## Operation: Add%1162784538; C++
      //	Resets the table to nil
      //	-- {AddDecl: 529} region.unprotectedFunction [2032..2321]
      m4bool_t Add (const m4uint32_t ai_iOffset, const void* ai_pPtr);

      //## Operation: Reset%834609009; C++
      //	-- {AddDecl: 530} region.unprotectedFunction [2322..2866]
      void Reset (void );

    // Additional Public Declarations
      //## begin ClTableAddressOffset%3911AA2401F6.public preserve=yes
      //## end ClTableAddressOffset%3911AA2401F6.public

  protected:
    // Additional Protected Declarations
      //## begin ClTableAddressOffset%3911AA2401F6.protected preserve=yes
      //## end ClTableAddressOffset%3911AA2401F6.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA2800FC
      //## Role: ClTableAddressOffset::NOT_FOUND%3911AA2800FE
      //## begin ClTableAddressOffset::NOT_FOUND%3911AA2800FE.role preserve=no  private: static m4int32_t {1 -> 1VHAPNC}
      static const m4int32_t  NOT_FOUND;
      //## end ClTableAddressOffset::NOT_FOUND%3911AA2800FE.role

      //## Association: M4Serial::<unnamed>%3911B75802F1
      //## Role: ClTableAddressOffset::m_iMode%3911B759011C
      //## begin ClTableAddressOffset::m_iMode%3911B759011C.role preserve=no  private: m4uint32_t { -> VHAN}
      m4uint32_t m_iMode;
      //## end ClTableAddressOffset::m_iMode%3911B759011C.role

    // Additional Private Declarations
      //## begin ClTableAddressOffset%3911AA2401F6.private preserve=yes
#ifdef _HASH_
      hash_map<m4uint32_t, void*, hash<m4uint32_t>, equal_to<m4uint32_t> > m_oTableOffset;
      hash_map<void*, m4uint32_t, hash<void*>, equal_to<void*> > m_oTableAddress;
#endif

#ifndef _HASH_
      map<m4uint32_t, void*, less<m4uint32_t> > m_oTableOffset;
      map<void*, m4uint32_t, less<void*> > m_oTableAddress;
#endif	
      //## end ClTableAddressOffset%3911AA2401F6.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTableAddressOffset%3911AA2401F6.implementation preserve=yes
      //## end ClTableAddressOffset%3911AA2401F6.implementation

};

//## begin ClTableAddressOffset%3911AA2401F6.postscript preserve=yes
//## end ClTableAddressOffset%3911AA2401F6.postscript

// Class ClTableAddressOffset 

//## begin module%3911AA2401F7.epilog preserve=yes
//## end module%3911AA2401F7.epilog


#endif
