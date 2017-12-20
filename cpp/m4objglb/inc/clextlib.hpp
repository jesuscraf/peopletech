//## begin module%3912A6B800AD.cm preserve=no
//## end module%3912A6B800AD.cm

//## begin module%3912A6B800AD.cp preserve=no
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
//## end module%3912A6B800AD.cp

//## Module: clextlib%3912A6B800AD; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clextlib.hpp

#ifndef __CLEXTLIB__HPP__
#define __CLEXTLIB__HPP__ 1

//## begin module%3912A6B800AD.additionalIncludes preserve=no
//## end module%3912A6B800AD.additionalIncludes

//## begin module%3912A6B800AD.includes preserve=yes
//## end module%3912A6B800AD.includes

// clextobj
#include <clextobj.hpp>
// m4types
#include <m4types.hpp>
// m4loadll
#include <m4loadll.hpp>
//## begin module%3912A6B800AD.declarations preserve=no
//## end module%3912A6B800AD.declarations

//## begin module%3912A6B800AD.additionalDeclarations preserve=yes
//## end module%3912A6B800AD.additionalDeclarations


//## begin M4ClExternLibrary%3912A6B800AE.preface preserve=yes
//## end M4ClExternLibrary%3912A6B800AE.preface

//## Class: M4ClExternLibrary%3912A6B800AE
//	----Para las Dll
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB M4ClExternLibrary : public M4ClExternObject  //## Inherits: <unnamed>%3912A6B90110
{
  //## begin M4ClExternLibrary%3912A6B800AE.initialDeclarations preserve=yes
  //## end M4ClExternLibrary%3912A6B800AE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClExternLibrary%-997596868; C++
      M4ClExternLibrary (void );

    //## Destructor (specified)
      //## Operation: ~M4ClExternLibrary%-1277333319; C++
      //	-- {AddDecl: 229} region.unprotectedFunction [1321..1566]
      ~M4ClExternLibrary (void );


    //## Other Operations (specified)
      //## Operation: Init%-1655963059; C++
      //	-- {AddDecl: 228} region.unprotectedFunction [959..1320]
      m4return_t Init (m4pcchar_t ai_pccDllName);

      //## Operation: Execute%-59165089; C++
      //	-- {AddDecl: 230} region.unprotectedFunction [1567..2491]
      m4return_t Execute (m4pcchar_t ai_pccMethodName, m4VariantType* ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType& ai_pvReturn, m4pvoid_t ai_pvContext);

    // Additional Public Declarations
      //## begin M4ClExternLibrary%3912A6B800AE.public preserve=yes
      //## end M4ClExternLibrary%3912A6B800AE.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClExternLibrary%3912A6B800AE.protected preserve=yes
      //## end M4ClExternLibrary%3912A6B800AE.protected

  private:
    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6B800DE
      //## Role: M4ClExternLibrary::m_bInit%3912A6B800E0
      //	Dll Iniciada S/N
      //## begin M4ClExternLibrary::m_bInit%3912A6B800E0.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bInit;
      //## end M4ClExternLibrary::m_bInit%3912A6B800E0.role

      //## Association: m4objglb::<unnamed>%3912A6B80337
      //## Role: M4ClExternLibrary::m_oDll%3912A6B80339
      //	Objeto para manejar la Dll
      //## begin M4ClExternLibrary::m_oDll%3912A6B80339.role preserve=no  private: M4ClLoaDll {1 -> 1VHAPN}
      M4ClLoaDll m_oDll;
      //## end M4ClExternLibrary::m_oDll%3912A6B80339.role

      //## Association: m4objglb::<unnamed>%3912A6B8033B
      //## Role: M4ClExternLibrary::m_pszObjectName%3912A6B8033D
      //## begin M4ClExternLibrary::m_pszObjectName%3912A6B8033D.role preserve=no  private: m4char_t {1 -> 0..1RHAPN}
      m4char_t *m_pszObjectName;
      //## end M4ClExternLibrary::m_pszObjectName%3912A6B8033D.role

    // Additional Private Declarations
      //## begin M4ClExternLibrary%3912A6B800AE.private preserve=yes
      //## end M4ClExternLibrary%3912A6B800AE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClExternLibrary%3912A6B800AE.implementation preserve=yes
      //## end M4ClExternLibrary%3912A6B800AE.implementation

};

//## begin M4ClExternLibrary%3912A6B800AE.postscript preserve=yes
//## end M4ClExternLibrary%3912A6B800AE.postscript

// Class M4ClExternLibrary 

//## begin module%3912A6B800AD.epilog preserve=yes
//## end module%3912A6B800AD.epilog


#endif
