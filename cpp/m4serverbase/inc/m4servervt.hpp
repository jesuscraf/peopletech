//## begin module%36DBDB5C0071.cm preserve=no
//## end module%36DBDB5C0071.cm

//## begin module%36DBDB5C0071.cp preserve=no
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
//## end module%36DBDB5C0071.cp

//## Module: m4servervt%36DBDB5C0071; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: e:\mybuild\m4serverbase\inc\m4servervt.hpp

#ifndef m4servervt_h
#define m4servervt_h 1

//## begin module%36DBDB5C0071.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
//## end module%36DBDB5C0071.additionalIncludes

//## begin module%36DBDB5C0071.includes preserve=yes
#include "m4stl.hpp"
//## end module%36DBDB5C0071.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%36DBDB5C0071.declarations preserve=no
//## end module%36DBDB5C0071.declarations

//## begin module%36DBDB5C0071.additionalDeclarations preserve=yes
#define	M4_SVT_TYPE_NULL				0
#define	M4_SVT_TYPE_DOUBLE				1
#define	M4_SVT_TYPE_NUMBER				M4_SVT_TYPE_DOUBLE
#define	M4_SVT_TYPE_STRING	            2
#define	M4_SVT_TYPE_POINTER				3
#define	M4_SVT_TYPE_INT					4
#define	M4_SVT_TYPE_M4RETURN			5
#define	M4_SVT_TYPE_INT64				6
#define M4_SVT_TYPE_STRING_VAR			0x22

union m4ServerVTData{
	m4double_t	DoubleData;
	m4int_t		IntData;
	m4pchar_t	PointerChar;
	m4int64_t	Int64Data;
};
//## end module%36DBDB5C0071.additionalDeclarations


//## begin m4ServerVT%36DBD83B03C3.preface preserve=yes
//## end m4ServerVT%36DBD83B03C3.preface

//## Class: m4ServerVT%36DBD83B03C3
//	-- {AddDecl: 5} class [987..2618]
//## Category: M4ServerBase::ClTagList_package%358A4049013C
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3905689E0279;stdXXiostream { -> }
//## Uses: <unnamed>%3A34A6830365;m4string_t { -> }

class M4_DECL_M4SERVERBASE m4ServerVT 
{
  //## begin m4ServerVT%36DBD83B03C3.initialDeclarations preserve=yes
  //## end m4ServerVT%36DBD83B03C3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: m4ServerVT%2057418851; C++
      //	Copy constructor
      //	-- {AddDecl: 7} region.unprotectedFunction [2619..2744]
      //	Default constructor
      m4ServerVT (void );

      //## Operation: m4ServerVT%-1660970746; C++
      //	Basic types constructors
      //	-- {AddDecl: 8} region.unprotectedFunction [2745..2876]
      //	Copy constructor
      m4ServerVT (const m4ServerVT& ai_var);

      //## Operation: m4ServerVT%361154961; C++
      //	-- {AddDecl: 9} region.unprotectedFunction [2877..3063]
      //	Basic types constructor
      m4ServerVT (const m4double_t& ai_double);

      //## Operation: m4ServerVT%-1347296588; C++
      //	Destructor
      //	-- {AddDecl: 10} region.unprotectedFunction [3064..3488]
      m4ServerVT (const m4pchar_t& ai_char, m4bool_t ai_bCopy = M4_FALSE);

      //## Operation: m4ServerVT%956649482; C++
      //	-- {AddDecl: 9} region.unprotectedFunction [2877..3063]
      //	Basic types constructor
      m4ServerVT (m4int_t ai_int);

      //## Operation: m4ServerVT%956825070; C++
      //	Destructor
      //	-- {AddDecl: 10} region.unprotectedFunction [3064..3488]
      m4ServerVT (const m4pcchar_t& ai_char, m4bool_t ai_bCopy = M4_FALSE);

      //## Operation: m4ServerVT%972896440; C++
      m4ServerVT (m4int64_t ai_i64Value);

      //## Operation: m4ServerVT%972896445; C++
      //	-- {AddDecl: 9} region.unprotectedFunction [2877..3063]
      //	Basic types constructor
      m4ServerVT (m4return_t ai_m4ret);

    //## Destructor (specified)
      //## Operation: ~m4ServerVT%-497375751; C++
      //	-- {AddDecl: 11} region.unprotectedFunction [3489..3553]
      ~m4ServerVT (void );


    //## Other Operations (specified)
      //## Operation: FreeData%428367957; C++
      //	Aritmethical operators
      //	-- {AddDecl: 12} region.unprotectedFunction [3554..3824]
      void FreeData (void );

      //## Operation: operator =%-449823595; C++
      //	-- {AddDecl: 15} region.unprotectedFunction [4426..4550]
      m4ServerVT& operator = (const m4ServerVT& ai_var);

      //## Operation: GetType%575041740; C++
      //	-- {AddDecl: 13} region.unprotectedFunction [3825..3916]
      m4char_t GetType (void );

      //## Operation: SetDouble%1397363735; C++
      void SetDouble (m4double_t ai_double);

      //## Operation: SetString%-1556363982; C++
      //	-- {AddDecl: 23} region.unprotectedFunction [8075..8496]
      void SetString (const m4pchar_t ai_char, m4bool_t ai_bFree = M4_TRUE);

      //## Operation: SetNull%-1597179984; C++
      //	Type conversion
      //	Return a double everywhere its expected
      //	-- {AddDecl: 28} region.unprotectedFunction [10578..10697]
      void SetNull (void );

      //## Operation: operator ==%920379248; C++
      m4bool_t operator == (m4ServerVT ai_right);

      //## Operation: operator !=%920379249; C++
      m4bool_t operator != (m4ServerVT ai_right);

      //## Operation: TraceSVT%920908742; C++
      void TraceSVT (m4string_t &ao_oTraceStr);

      //## Operation: SetPointer%956592443; C++
      void SetPointer (void *ai_Pointer);

      //## Operation: SetInt%956649483; C++
      void SetInt (m4int_t ai_int);

      //## Operation: SetInt64%972896439; C++
      void SetInt64 (m4int64_t ai_i64Value);

      //## Operation: SetM4return%972896446; C++
      void SetM4return (m4return_t ai_ret);

    // Data Members for Class Attributes

      //## Attribute: Data%36DBD83C00D5
      //## begin m4ServerVT::Data%36DBD83C00D5.attr preserve=no  public: union m4ServerVTData {VA} 
      union m4ServerVTData Data;
      //## end m4ServerVT::Data%36DBD83C00D5.attr

    // Additional Public Declarations
      //## begin m4ServerVT%36DBD83B03C3.public preserve=yes
      //## end m4ServerVT%36DBD83B03C3.public

  protected:
    // Additional Protected Declarations
      //## begin m4ServerVT%36DBD83B03C3.protected preserve=yes
      //## end m4ServerVT%36DBD83B03C3.protected

  private:

    //## Other Operations (specified)
      //## Operation: Set%-685867871; C++
      //	Assigns a VARIANT
      //	-- {AddDecl: 14} region.unprotectedFunction [3917..4425]
      void Set (const m4ServerVT& ai_var);

    // Data Members for Associations

      //## Association: M4ServerBase::ClTagList_package::<unnamed>%36DBD83C0111
      //## Role: m4ServerVT::Type%36DBD83C011C
      //## begin m4ServerVT::Type%36DBD83C011C.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t Type;
      //## end m4ServerVT::Type%36DBD83C011C.role

      //## Association: M4ServerBase::ClTagList_package::<unnamed>%36DBD83C0130
      //## Role: m4ServerVT::m_bOwner%36DBD83C0132
      //## begin m4ServerVT::m_bOwner%36DBD83C0132.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bOwner;
      //## end m4ServerVT::m_bOwner%36DBD83C0132.role

    // Additional Private Declarations
      //## begin m4ServerVT%36DBD83B03C3.private preserve=yes
      //## end m4ServerVT%36DBD83B03C3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin m4ServerVT%36DBD83B03C3.implementation preserve=yes
      //## end m4ServerVT%36DBD83B03C3.implementation

};

//## begin m4ServerVT%36DBD83B03C3.postscript preserve=yes
//## end m4ServerVT%36DBD83B03C3.postscript

// Class m4ServerVT 

//## begin module%36DBDB5C0071.epilog preserve=yes
//## end module%36DBDB5C0071.epilog


#endif
