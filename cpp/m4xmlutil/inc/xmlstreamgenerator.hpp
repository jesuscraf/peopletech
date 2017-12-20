//## begin module%3958CC170157.cm preserve=no
//## end module%3958CC170157.cm

//## begin module%3958CC170157.cp preserve=no
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
//## end module%3958CC170157.cp

//## Module: xmlstreamgenerator%3958CC170157; Package specification
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Source file: D:\Work\v600\m4xmlutil\inc\xmlstreamgenerator.hpp

#ifndef xmlstreamgenerator_h
#define xmlstreamgenerator_h 1

//## begin module%3958CC170157.additionalIncludes preserve=no
//## end module%3958CC170157.additionalIncludes

//## begin module%3958CC170157.includes preserve=yes
#include <m4types.hpp>
#include "m4stl.hpp"
#include "m4xmlutil_dll.hpp"
//## end module%3958CC170157.includes

// m4string
#include <m4string.hpp>
//## begin module%3958CC170157.declarations preserve=no
//## end module%3958CC170157.declarations

//## begin module%3958CC170157.additionalDeclarations preserve=yes
//## end module%3958CC170157.additionalDeclarations


//## begin M4XMLStreamGenerator%3958CAC8008D.preface preserve=yes
//## end M4XMLStreamGenerator%3958CAC8008D.preface

//## Class: M4XMLStreamGenerator%3958CAC8008D
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3958CAC8008E;m4string_t { -> }
//## Uses: <unnamed>%3975675503B1;ostrstream { -> }

class M4_DECL_M4XMLUTIL M4XMLStreamGenerator 
{
  //## begin M4XMLStreamGenerator%3958CAC8008D.initialDeclarations preserve=yes
  //## end M4XMLStreamGenerator%3958CAC8008D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4XMLStreamGenerator%962121079
      M4XMLStreamGenerator (m4string_t ai_strQuote = "\"");

      //## Operation: M4XMLStreamGenerator%1083076556
      M4XMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote = "\"");

    //## Destructor (specified)
      //## Operation: ~M4XMLStreamGenerator%962121080
      virtual ~M4XMLStreamGenerator ();


    //## Other Operations (specified)
      //## Operation: MakeXMLDeclaration%962121081
	  // UNICODE XML
      virtual	m4return_t MakeXMLDeclaration ();

      //## Operation: InsertComment%962121082
      virtual	m4return_t InsertComment (m4string_t ai_strComment);
      virtual	m4return_t InsertComment (m4pcchar_t ai_pccComment);

      //## Operation: MakeElement%962121083
      virtual	m4return_t MakeElement (m4string_t ai_strElement);
      virtual	m4return_t MakeElement (m4pcchar_t ai_pccElement);

      //## Operation: AddElementAttribute%962272941
      virtual	m4return_t AddElementAttribute (m4string_t ai_strAttribute, m4string_t ai_strAttValue = "");
      virtual	m4return_t AddElementAttribute (m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue);

      //## Operation: EndElement%962272944
      virtual	void EndElement ();

      //## Operation: CloseElement%962121084
      virtual	m4return_t CloseElement (m4string_t ai_strElement);
      virtual	m4return_t CloseElement (m4pcchar_t ai_pccElement);

      //## Operation: CloseSingleClosedElement%962272943
      virtual	void CloseSingleClosedElement ();

      //## Operation: GetXMLStream%962121086
      virtual	m4return_t GetXMLStream (m4pchar_t &aio_strBuffer, long ai_lBufferSize);

      //## Operation: GetXMLStreamSize%962121087
      virtual	long GetXMLStreamSize ();

      //## Operation: AddEscapedElementAttribute%982171973
      virtual	m4return_t AddEscapedElementAttribute (m4string_t ai_strAttribute, m4pchar_t ai_strAttValue = "", m4uint32_t ai_iStrLen = 0);
      virtual	m4return_t AddEscapedElementAttribute (m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue);
      virtual	m4return_t AddEscapedElementAttribute (m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, size_t ai_iLength);

      //## Operation: EscapeXMLStream%982230715
      static m4return_t EscapeXMLStream (m4string_t &ai_strStream);

      //## Operation: EscapeXMLStream%1085739520
      static m4return_t EscapeXMLStream (m4string_t &ai_strInput, m4string_t &ao_strOutput);

      virtual	m4return_t AddScapedString (m4pcchar_t ai_pccString);
      virtual	m4return_t AddScapedString (m4pcchar_t ai_pccString, size_t ai_iLength);

      static m4return_t AppendScapedString (m4pcchar_t ai_pccString, string &ao_strOutput);
      static m4return_t AppendScapedString (m4pcchar_t ai_pccString, size_t ai_iLength, string &ao_strOutput);

      //## Operation: AddElementValue%1082986243
      virtual	m4return_t AddElementValue (m4string_t ai_strElementValue = "");
      virtual	m4return_t AddElementValue (m4pcchar_t ai_pccElementValue);
      virtual	m4return_t AddElementValue (m4pcchar_t ai_pccElementValue, size_t ai_iLength);

      //## Operation: AddEscapedElementValue%1083076555
      virtual	m4return_t AddEscapedElementValue (m4pchar_t ai_strElementValue = "", m4uint32_t ai_iStrLen = 0);
      virtual	m4return_t AddEscapedElementValue (m4pcchar_t ai_strElementValue);
      virtual	m4return_t AddEscapedElementValue (m4pcchar_t ai_pccElementValue, size_t ai_iLength);

    // Additional Public Declarations
      //## begin M4XMLStreamGenerator%3958CAC8008D.public preserve=yes
      //## end M4XMLStreamGenerator%3958CAC8008D.public


	  ostream*	GetStream( void )
	  {
		  return( &m_oStream ) ;
	  }

  protected:

      virtual	m4return_t _PutString( m4pcchar_t ai_pccString );
      virtual	m4return_t _PutString( m4pcchar_t ai_pccString, size_t ai_iLength );
      virtual	m4pcchar_t _GetHeader( void ) const;
      virtual	m4return_t _AddScapedString( m4pcchar_t ai_pccString, size_t ai_iLength );


    // Additional Protected Declarations
      //## begin M4XMLStreamGenerator%3958CAC8008D.protected preserve=yes
      //## end M4XMLStreamGenerator%3958CAC8008D.protected

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_strBuffer%3958CAC800B0
      //## begin M4XMLStreamGenerator::m_strBuffer%3958CAC800B0.attr preserve=no  private: ostrstream {UA} 
      ostrstream m_strBuffer;
      //## end M4XMLStreamGenerator::m_strBuffer%3958CAC800B0.attr

      //## Attribute: m_strQuote%395B1E12026B
      //## begin M4XMLStreamGenerator::m_strQuote%395B1E12026B.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strQuote;
      //## end M4XMLStreamGenerator::m_strQuote%395B1E12026B.attr

      //## Attribute: m_oStream%408E73720305
      //## begin M4XMLStreamGenerator::m_oStream%408E73720305.attr preserve=no  private: ostream& {UA} 
      ostream& m_oStream;
      //## end M4XMLStreamGenerator::m_oStream%408E73720305.attr

    // Additional Private Declarations
      //## begin M4XMLStreamGenerator%3958CAC8008D.private preserve=yes
      //## end M4XMLStreamGenerator%3958CAC8008D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4XMLStreamGenerator%3958CAC8008D.implementation preserve=yes
      //## end M4XMLStreamGenerator%3958CAC8008D.implementation

};

//## begin M4XMLStreamGenerator%3958CAC8008D.postscript preserve=yes
//## end M4XMLStreamGenerator%3958CAC8008D.postscript

// Class M4XMLStreamGenerator 


// ANSI Stream Generator

class M4_DECL_M4XMLUTIL M4ANSIXMLStreamGenerator : public M4XMLStreamGenerator
{
  public:

      M4ANSIXMLStreamGenerator (m4string_t ai_strQuote = "\"");
      M4ANSIXMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote = "\"");
      virtual ~M4ANSIXMLStreamGenerator (void);

      virtual	m4return_t AddScapedString (m4pcchar_t ai_pccString, size_t ai_iLength);

  protected:

      virtual	m4return_t _PutString( m4pcchar_t ai_pccString );
      virtual	m4return_t _PutString( m4pcchar_t ai_pccString, size_t ai_iLength );
      virtual	m4pcchar_t _GetHeader( void ) const;
};

// UTF8 Stream Generator

class M4_DECL_M4XMLUTIL M4Utf8XMLStreamGenerator : public M4XMLStreamGenerator
{
  public:

      M4Utf8XMLStreamGenerator (m4string_t ai_strQuote = "\"");
      M4Utf8XMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote = "\"");
      virtual ~M4Utf8XMLStreamGenerator (void);

      virtual	m4return_t AddScapedString (m4pcchar_t ai_pccString, size_t ai_iLength);

  protected:

      virtual	m4return_t _PutString( m4pcchar_t ai_pccString );
      virtual	m4return_t _PutString( m4pcchar_t ai_pccString, size_t ai_iLength );
      virtual	m4pcchar_t _GetHeader( void ) const;
};

//## begin module%3958CC170157.epilog preserve=yes
//## end module%3958CC170157.epilog


#endif
