//## begin module%3906B7910117.cm preserve=no
//## end module%3906B7910117.cm

//## begin module%3906B7910117.cp preserve=no
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
//## end module%3906B7910117.cp

//## Module: m4xmlbase%3906B7910117; Package specification
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Source file: C:\m4server\m4xmlutil\inc\m4xmlbase.hpp

#ifndef m4xmlbase_h
#define m4xmlbase_h 1

//## begin module%3906B7910117.additionalIncludes preserve=no
//## end module%3906B7910117.additionalIncludes

//## begin module%3906B7910117.includes preserve=yes
#include <m4string.hpp>
#include <m4types.hpp>
#include "m4stl.hpp"
#include <m4xmlutil_dll.hpp>
#include <xercesc/sax/AttributeList.hpp>
//## end module%3906B7910117.includes

// SAXHandlerBase
#include <xercesc/sax/HandlerBase.hpp>

class M4SAXHandlerBase;

//## begin module%3906B7910117.declarations preserve=no
//## end module%3906B7910117.declarations

//## begin module%3906B7910117.additionalDeclarations preserve=yes
XERCES_CPP_NAMESPACE_USE
//## end module%3906B7910117.additionalDeclarations


//## begin M4XMLChBase%3905BB55016E.preface preserve=yes
//## end M4XMLChBase%3905BB55016E.preface

//## Class: M4XMLChBase%3905BB55016E
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n


// Función de ordenación para optimizar la memoria de la lista de funciones
typedef	XMLCh*	XMLCharPointer_t ;

struct XMLCharLess : public binary_function<XMLCharPointer_t, XMLCharPointer_t, bool>
{
    bool operator()(const XMLCharPointer_t &x, const XMLCharPointer_t &y) const
	{
		return( XMLString::compareString( x, y ) < 0 ) ;
	}
};


//## begin ElementProcStart%390561040045.preface preserve=yes
//## end ElementProcStart%390561040045.preface

//## Class: ElementProcStart%390561040045
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%390565F60154;M4SAXHandlerBase { -> F}

typedef void (M4SAXHandlerBase::* ElementProcStart)(const XMLCh* const name, AttributeList* attributes);

//## begin ElementProcStart%390561040045.postscript preserve=yes
//## end ElementProcStart%390561040045.postscript

//## begin ElementProcStartMap%39056134029D.preface preserve=yes
//## end ElementProcStartMap%39056134029D.preface

//## Class: ElementProcStartMap%39056134029D; Instantiated Class
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%390561C4013C;ElementProcStart { -> }

typedef map< XMLCharPointer_t,ElementProcStart,XMLCharLess > ElementProcStartMap;

//## begin ElementProcStartMap%39056134029D.postscript preserve=yes
//## end ElementProcStartMap%39056134029D.postscript

//## begin ElementProcIter%3906B732012E.preface preserve=yes
//## end ElementProcIter%3906B732012E.preface

//## Class: ElementProcIter%3906B732012E
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3906B7460029;ElementProcStartMap { -> }

typedef ElementProcStartMap::iterator ElementProcIter;

//## begin ElementProcIter%3906B732012E.postscript preserve=yes
//## end ElementProcIter%3906B732012E.postscript

//## begin ElementProcEnd%3945173501E5.preface preserve=yes
//## end ElementProcEnd%3945173501E5.preface

//## Class: ElementProcEnd%3945173501E5
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3945175A00F8;M4SAXHandlerBase { -> F}

typedef void (M4SAXHandlerBase::* ElementProcEnd)(const XMLCh* const name);

//## begin ElementProcEnd%3945173501E5.postscript preserve=yes
//## end ElementProcEnd%3945173501E5.postscript

//## begin ElementProcEndMap%394517B00138.preface preserve=yes
//## end ElementProcEndMap%394517B00138.preface

//## Class: ElementProcEndMap%394517B00138; Instantiated Class
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%394518FB03E7;ElementProcEnd { -> }

typedef map< XMLCharPointer_t,ElementProcEnd,XMLCharLess > ElementProcEndMap;

//## begin ElementProcEndMap%394517B00138.postscript preserve=yes
//## end ElementProcEndMap%394517B00138.postscript

//## begin M4SAXHandlerBase%3905593E00DA.preface preserve=yes
//## end M4SAXHandlerBase%3905593E00DA.preface

//## Class: M4SAXHandlerBase%3905593E00DA
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3905BB95036F;M4XMLChBase { -> F}

class M4_DECL_M4XMLUTIL M4SAXHandlerBase : public HandlerBase  //## Inherits: <unnamed>%3905594E012D
{
  //## begin M4SAXHandlerBase%3905593E00DA.initialDeclarations preserve=yes
  //## end M4SAXHandlerBase%3905593E00DA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SAXHandlerBase%956652051
      M4SAXHandlerBase ();

    //## Destructor (specified)
      //## Operation: ~M4SAXHandlerBase%956652052
      ~M4SAXHandlerBase ();


    //## Other Operations (specified)
      //## Operation: GetError%956652053
      m4return_t GetError () const;

      //## Operation: endDocument%956652054
      void endDocument ();

      //## Operation: endElement%956652055
      void endElement (const XMLCh * const name);

      //## Operation: startDocument%956652056
      void startDocument ();

      //## Operation: startElement%956652057
      void startElement (const XMLCh* const name, AttributeList& attributes);

      //## Operation: characters%1082986242
      void characters (const XMLCh* const chars, const unsigned int length);

      //## Operation: warning%956652058
      void warning (const SAXParseException& exception);

      //## Operation: error%956652059
      void error (const SAXParseException& exception);

      //## Operation: fatalError%956652060
      void fatalError (const SAXParseException& exception);

      //## Operation: resetErrors%956652061
      void resetErrors ();

      //## Operation: IsOk%956652062
      m4bool_t IsOk () const;

      //## Operation: SetError%956652063
      void SetError (m4return_t ai_iError = M4_ERROR);

      //## Operation: RegisterElementProc%956741691
      void RegisterElementProc (m4pchar_t ai_string, ElementProcStart ai_pElementProc);

      //## Operation: RegisterElementEndProc%960828891
      void RegisterElementEndProc (m4pchar_t ai_string, ElementProcEnd ai_pElementProc);

    // Additional Public Declarations
      //## begin M4SAXHandlerBase%3905593E00DA.public preserve=yes
      //## end M4SAXHandlerBase%3905593E00DA.public

  protected:
    // Data Members for Associations

      //## Association: M4XMLUtil::<unnamed>%39451576019C
      //## Role: M4SAXHandlerBase::m_oElementEndProcMap%394515760373
      //## begin M4SAXHandlerBase::m_oElementEndProcMap%394515760373.role preserve=no  protected: ElementProcEndMap { -> VHAN}
      ElementProcEndMap m_oElementEndProcMap;
      //## end M4SAXHandlerBase::m_oElementEndProcMap%394515760373.role

    // Additional Protected Declarations
      //## begin M4SAXHandlerBase%3905593E00DA.protected preserve=yes
      //## end M4SAXHandlerBase%3905593E00DA.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsOk%39055FBD01A4
      //## begin M4SAXHandlerBase::m_bIsOk%39055FBD01A4.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bIsOk;
      //## end M4SAXHandlerBase::m_bIsOk%39055FBD01A4.attr

      //## Attribute: m_iError%39055FD603BC
      //## begin M4SAXHandlerBase::m_iError%39055FD603BC.attr preserve=no  private: m4return_t {UA} 
      m4return_t m_iError;
      //## end M4SAXHandlerBase::m_iError%39055FD603BC.attr

    // Data Members for Associations

      //## Association: M4XMLUtil::<unnamed>%3906BBAA03C8
      //## Role: M4SAXHandlerBase::m_oElementProcMap%3906BBAC00A0
      //## begin M4SAXHandlerBase::m_oElementProcMap%3906BBAC00A0.role preserve=no  private: ElementProcStartMap { -> VHAN}
      ElementProcStartMap m_oElementProcMap;
      //## end M4SAXHandlerBase::m_oElementProcMap%3906BBAC00A0.role

    // Additional Private Declarations
      //## begin M4SAXHandlerBase%3905593E00DA.private preserve=yes
      //## end M4SAXHandlerBase%3905593E00DA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SAXHandlerBase%3905593E00DA.implementation preserve=yes
      //## end M4SAXHandlerBase%3905593E00DA.implementation

};

//## begin M4SAXHandlerBase%3905593E00DA.postscript preserve=yes
//## end M4SAXHandlerBase%3905593E00DA.postscript

//## begin ElementProcEndIter%39451A4500DE.preface preserve=yes
//## end ElementProcEndIter%39451A4500DE.preface

//## Class: ElementProcEndIter%39451A4500DE
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39451A6701E1;ElementProcEndMap { -> }

typedef ElementProcEndMap::iterator ElementProcEndIter;

//## begin ElementProcEndIter%39451A4500DE.postscript preserve=yes
//## end ElementProcEndIter%39451A4500DE.postscript

// Class M4XMLChBase 

// Class M4SAXHandlerBase 


//## Other Operations (inline)
inline m4return_t M4SAXHandlerBase::GetError () const
{
  //## begin M4SAXHandlerBase::GetError%956652053.body preserve=yes
	return m_iError ;
  //## end M4SAXHandlerBase::GetError%956652053.body
}

inline m4bool_t M4SAXHandlerBase::IsOk () const
{
  //## begin M4SAXHandlerBase::IsOk%956652062.body preserve=yes
	return 	m_bIsOk ;
  //## end M4SAXHandlerBase::IsOk%956652062.body
}

inline void M4SAXHandlerBase::SetError (m4return_t ai_iError)
{
  //## begin M4SAXHandlerBase::SetError%956652063.body preserve=yes
	m_bIsOk = M4_FALSE ;
	m_iError = ai_iError ;
  //## end M4SAXHandlerBase::SetError%956652063.body
}

//## begin module%3906B7910117.epilog preserve=yes
//## end module%3906B7910117.epilog


#endif
