//## begin module%407D0C7C0119.cm preserve=no
//## end module%407D0C7C0119.cm

//## begin module%407D0C7C0119.cp preserve=no
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
//## end module%407D0C7C0119.cp

//## Module: m4transctx%407D0C7C0119; Package specification
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Source file: D:\Work\v600\m4mdtransfer\inc\m4transctx.hpp

#ifndef m4transctx_h
#define m4transctx_h 1

//## begin module%407D0C7C0119.additionalIncludes preserve=no
//## end module%407D0C7C0119.additionalIncludes

//## begin module%407D0C7C0119.includes preserve=yes
//## end module%407D0C7C0119.includes

#include "m4stl.hpp"
#include <m4mdtodbc.hpp>
#include <xmlstreamgenerator.hpp>
#include <m4types.hpp>

#include <xercesc/util/XercesDefs.hpp>


class IMDTService;
class ClMDTTransformerContext;


XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
XERCES_CPP_NAMESPACE_END
//## end module%407D0C7C0119.additionalDeclarations


//## begin ClMDTStringMap%407D671303CF.preface preserve=yes
//## end ClMDTStringMap%407D671303CF.preface

//## Class: ClMDTStringMap%407D671303CF; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,string,less<string> > ClMDTStringMap;

//## begin ClMDTStringMap%407D671303CF.postscript preserve=yes
//## end ClMDTStringMap%407D671303CF.postscript

//## begin ClMDTDOMNodeVector%408406D802AD.preface preserve=yes
//## end ClMDTDOMNodeVector%408406D802AD.preface

//## Class: ClMDTDOMNodeVector%408406D802AD; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< DOMNode* > ClMDTDOMNodeVector;

//## begin ClMDTDOMNodeVector%408406D802AD.postscript preserve=yes
//## end ClMDTDOMNodeVector%408406D802AD.postscript

//## begin ClMDTAliasMap%408557E202D3.preface preserve=yes
//## end ClMDTAliasMap%408557E202D3.preface

//## Class: ClMDTAliasMap%408557E202D3; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,string,less<string> > ClMDTAliasMap;

//## begin ClMDTAliasMap%408557E202D3.postscript preserve=yes
//## end ClMDTAliasMap%408557E202D3.postscript

//## begin ClMDTStringSet%408F6EC801CD.preface preserve=yes
//## end ClMDTStringSet%408F6EC801CD.preface

//## Class: ClMDTStringSet%408F6EC801CD; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef set< string > ClMDTStringSet;

//## begin ClMDTStringSet%408F6EC801CD.postscript preserve=yes
//## end ClMDTStringSet%408F6EC801CD.postscript

//## begin ClMDTStringIntMap%409F623B0034.preface preserve=yes
//## end ClMDTStringIntMap%409F623B0034.preface

//## Class: ClMDTStringIntMap%409F623B0034; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,m4int32_t,less<string> > ClMDTStringIntMap;

//## begin ClMDTStringIntMap%409F623B0034.postscript preserve=yes
//## end ClMDTStringIntMap%409F623B0034.postscript

//## begin ClMDTTemplateFunction%40A0F86E019C.preface preserve=yes
//## end ClMDTTemplateFunction%40A0F86E019C.preface

//## Class: ClMDTTemplateFunction%40A0F86E019C
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClMDTTemplateFunction 
{
  //## begin ClMDTTemplateFunction%40A0F86E019C.initialDeclarations preserve=yes
  //## end ClMDTTemplateFunction%40A0F86E019C.initialDeclarations

  public:
    //## Constructors (generated)
      ClMDTTemplateFunction();


    //## Other Operations (specified)
      //## Operation: AddArg%1084288695
      m4void_t AddArg (string& ai_sName, string& ai_sValue);

      //## Operation: GetArg%1084353856
      m4return_t GetArg (string& ai_sName, string& ao_sValue);

    // Data Members for Class Attributes

      //## Attribute: m_bIsGenerate%40A0F9900340
      //## begin ClMDTTemplateFunction::m_bIsGenerate%40A0F9900340.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsGenerate;
      //## end ClMDTTemplateFunction::m_bIsGenerate%40A0F9900340.attr

      //## Attribute: m_sName%40A10086023B
      //## begin ClMDTTemplateFunction::m_sName%40A10086023B.attr preserve=no  public: string {UA} 
      string m_sName;
      //## end ClMDTTemplateFunction::m_sName%40A10086023B.attr

      string m_sOutputName;

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40A1D32A014D
      //## Role: ClMDTTemplateFunction::m_oArgs%40A1D32A0248
      //## begin ClMDTTemplateFunction::m_oArgs%40A1D32A0248.role preserve=no  public: ClMDTStringMap { -> VHAN}
      ClMDTStringMap m_oArgs;
      //## end ClMDTTemplateFunction::m_oArgs%40A1D32A0248.role

    // Additional Public Declarations
      //## begin ClMDTTemplateFunction%40A0F86E019C.public preserve=yes
      //## end ClMDTTemplateFunction%40A0F86E019C.public

  protected:
    // Additional Protected Declarations
      //## begin ClMDTTemplateFunction%40A0F86E019C.protected preserve=yes
      //## end ClMDTTemplateFunction%40A0F86E019C.protected

  private:
    // Additional Private Declarations
      //## begin ClMDTTemplateFunction%40A0F86E019C.private preserve=yes
      //## end ClMDTTemplateFunction%40A0F86E019C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTTemplateFunction%40A0F86E019C.implementation preserve=yes
      //## end ClMDTTemplateFunction%40A0F86E019C.implementation

};

//## begin ClMDTTemplateFunction%40A0F86E019C.postscript preserve=yes
//## end ClMDTTemplateFunction%40A0F86E019C.postscript

//## begin ClMDTTemplateVector%40A0FAC70022.preface preserve=yes
//## end ClMDTTemplateVector%40A0FAC70022.preface

//## Class: ClMDTTemplateVector%40A0FAC70022; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40A0FB040031;ClMDTTemplateFunction { -> }

typedef vector< ClMDTTemplateFunction* > ClMDTTemplateVector;

//## begin ClMDTTemplateVector%40A0FAC70022.postscript preserve=yes
//## end ClMDTTemplateVector%40A0FAC70022.postscript

//## begin ClMDTStringVector%40A0FFCD01DE.preface preserve=yes
//## end ClMDTStringVector%40A0FFCD01DE.preface

//## Class: ClMDTStringVector%40A0FFCD01DE; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< string > ClMDTStringVector;

//## begin ClMDTStringVector%40A0FFCD01DE.postscript preserve=yes
//## end ClMDTStringVector%40A0FFCD01DE.postscript

//## begin ClMDTIntVector%408CC3BB00A4.preface preserve=yes
//## end ClMDTIntVector%408CC3BB00A4.preface

//## Class: ClMDTIntVector%408CC3BB00A4; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< m4int32_t > ClMDTIntVector;

//## begin ClMDTIntVector%408CC3BB00A4.postscript preserve=yes
//## end ClMDTIntVector%408CC3BB00A4.postscript




//=================================================================================
//
// ClDoubleXMLStreamGenerator
//
// Clase que permite volcar el xml de datos a dos salidas complemetarias
// Para poder guardar los campos que no son camparables en otro fichero
//=================================================================================

class ClDoubleXMLStreamGenerator
{
public:

		ClDoubleXMLStreamGenerator( M4XMLStreamGenerator *ai_poFirstStream, M4XMLStreamGenerator *ai_poSecondStream ) ;
		virtual ~ClDoubleXMLStreamGenerator( void );

	M4XMLStreamGenerator	*GetFirstStream( void )
	{
		return( m_poFirstStream ) ;
	}

	M4XMLStreamGenerator	*GetSecondStream( void )
	{
		return( m_poSecondStream ) ;
	}

	m4return_t MakeXMLDeclaration( m4uint8_t ai_iMode ) ;
	m4return_t MakeElement( m4pcchar_t ai_pccElement, m4uint8_t ai_iMode ) ;
	m4return_t AddElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, m4uint8_t ai_iMode ) ;
	m4return_t AddElementValue( m4pcchar_t ai_pccElementValue, m4uint8_t ai_iMode ) ;
	m4return_t AddElementValue( m4pcchar_t ai_pccElementValue, size_t ai_iLength, m4uint8_t ai_iMode ) ;
	m4return_t AddEscapedElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, size_t ai_iLength, m4uint8_t ai_iMode ) ;
	m4return_t CloseElement( m4pcchar_t ai_pccElement, m4uint8_t ai_iMode ) ;
	void CloseSingleClosedElement( m4uint8_t ai_iMode ) ;
	void EndElement( m4uint8_t ai_iMode ) ;

protected:

	M4XMLStreamGenerator	*m_poFirstStream ;
	M4XMLStreamGenerator	*m_poSecondStream ;
};





//## begin ClMDTTableLoader%4085256000B0.preface preserve=yes
//## end ClMDTTableLoader%4085256000B0.preface

//## Class: ClMDTTableLoader%4085256000B0
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%408F6EFA03B1;ClMDTStringSet { -> }

class ClMDTTableLoader 
{
  //## begin ClMDTTableLoader%4085256000B0.initialDeclarations preserve=yes
  //## end ClMDTTableLoader%4085256000B0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTTableLoader%1082553358
      ClMDTTableLoader (IMDTService* ai_poService, ClDoubleXMLStreamGenerator* &ai_poOutput);

    //## Destructor (specified)
      //## Operation: ~ClMDTTableLoader%1086770005
      virtual ~ClMDTTableLoader ();


    //## Other Operations (specified)
      //## Operation: AddField%1082385413
      m4void_t SetLogicObject (DOMNode* ai_poNode);

      m4void_t AddField (DOMNode* ai_poNode);

      //## Operation: AddNewField%1084353850
      m4return_t AddNewField (string& ai_sName);

      //## Operation: Load%1082385414
      m4return_t Load ();

      //## Operation: SetContext%1082721861
      m4void_t SetContext (ClMDTTransformerContext* ai_poContext);

      ClMDTTransformerContext*	GetContext (void)
	  {
		  return( m_poContext ) ;
	  }

      //## Operation: GetNRecords%1082721864
      m4int32_t GetNRecords () const;

      //## Operation: GetParentNRecords%1082721865
      m4int32_t GetParentNRecords ();

      //## Operation: GetParentRecord%1082967065
      m4int32_t GetParentRecord (m4int32_t ai_iReg);

      //## Operation: GetField%1082967066
      m4return_t GetField (const string& ai_sName, m4int32_t ai_iReg, ClMDTValue& ao_oRes);

      //## Operation: Dump%1083080791
      m4return_t Dump (void);

      //## Operation: GetValue%1084353854
      m4return_t GetValue (m4int32_t ai_iReg, string& ai_sName, ClMDTValue& ao_vRes);

      //## Operation: SetValue%1084353855
      m4return_t SetValue (m4int32_t ai_iReg, string& ai_sName, ClMDTValue& ai_vValue);

      //## Operation: Sort%1089021408
      m4return_t Sort ();

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408F96D202E6
      //## Role: ClMDTTableLoader::m_oFieldMap%408F96D30170
      //## begin ClMDTTableLoader::m_oFieldMap%408F96D30170.role preserve=no  public: ClMDTStringIntMap { -> VHAN}
      ClMDTStringIntMap m_oFieldMap;
      //## end ClMDTTableLoader::m_oFieldMap%408F96D30170.role

	  DOMNode* m_poLogicObject;

    // Additional Public Declarations
      //## begin ClMDTTableLoader%4085256000B0.public preserve=yes
      //## end ClMDTTableLoader%4085256000B0.public

  protected:

    //## Other Operations (specified)
      //## Operation: GenerateLoadSelect%1082385417
      m4return_t GenerateLoadSelect ();

      //## Operation: LoadBlock%1082553357
      m4return_t LoadBlock (m4int32_t ai_iParentReg);

      //## Operation: CheckIsMultiline%1084447570
      m4bool_t CheckIsMultiline (string& ai_sValue);

      //## Operation: GetRegisterPKs%1084956753
      m4return_t GetRegisterPKs (ClMDTStringSet& ao_oPKSet);

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40E93491004D
      //## Role: ClMDTTableLoader::m_oPKPos%40E9349101D5
      //## begin ClMDTTableLoader::m_oPKPos%40E9349101D5.role preserve=no  protected: ClMDTIntVector { -> VHAN}
      ClMDTIntVector m_oPKPos;
      //## end ClMDTTableLoader::m_oPKPos%40E9349101D5.role

    // Additional Protected Declarations
      //## begin ClMDTTableLoader%4085256000B0.protected preserve=yes
      //## end ClMDTTableLoader%4085256000B0.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sSelect%40855707039E
      //## begin ClMDTTableLoader::m_sSelect%40855707039E.attr preserve=no  private: string {UA} 
      string m_sSelect;
      //## end ClMDTTableLoader::m_sSelect%40855707039E.attr

      //## Attribute: m_poService%4086988E011E
      //## begin ClMDTTableLoader::m_poService%4086988E011E.attr preserve=no  private: IMDTService* {UA} ai_poService
      IMDTService* m_poService;
      //## end ClMDTTableLoader::m_poService%4086988E011E.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%4085259D036F
      //## Role: ClMDTTableLoader::m_poContext%4085259E012D
      //## begin ClMDTTableLoader::m_poContext%4085259E012D.role preserve=no  private: ClMDTTransformerContext { -> 1RFHGAN}
      ClMDTTransformerContext *m_poContext;
      //## end ClMDTTableLoader::m_poContext%4085259E012D.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%4085273000C0
      //## Role: ClMDTTableLoader::m_oFieldsVector%408527300267
      //## begin ClMDTTableLoader::m_oFieldsVector%408527300267.role preserve=no  private: ClMDTDOMNodeVector { -> VHAN}
      ClMDTDOMNodeVector m_oFieldsVector;
      //## end ClMDTTableLoader::m_oFieldsVector%408527300267.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%4085537700CF
      //## Role: ClMDTTableLoader::m_oParameters%408553770267
      //## begin ClMDTTableLoader::m_oParameters%408553770267.role preserve=no  private: M4Row { -> VHAN}
      M4Row m_oParameters;
      //## end ClMDTTableLoader::m_oParameters%408553770267.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408695AD0266
      //## Role: ClMDTTableLoader::m_oResultSet%408695AE0053
      //## begin ClMDTTableLoader::m_oResultSet%408695AE0053.role preserve=no  private: M4ResultSet { -> VHAN}
      M4ResultSet m_oResultSet;
      //## end ClMDTTableLoader::m_oResultSet%408695AE0053.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408CC3F900D3
      //## Role: ClMDTTableLoader::m_oRegisterRef%408CC3FA0122
      //## begin ClMDTTableLoader::m_oRegisterRef%408CC3FA0122.role preserve=no  private: ClMDTIntVector { -> VHAN}
      ClMDTIntVector m_oRegisterRef;
      //## end ClMDTTableLoader::m_oRegisterRef%408CC3FA0122.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E8CBF00D3
      //## Role: ClMDTTableLoader::m_poOutput%408E8CBF01DD
      //## begin ClMDTTableLoader::m_poOutput%408E8CBF01DD.role preserve=no  private: ClDoubleXMLStreamGenerator { -> RHAN}
      ClDoubleXMLStreamGenerator* &m_poOutput;
      //## end ClMDTTableLoader::m_poOutput%408E8CBF01DD.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40A0FFF90077
      //## Role: ClMDTTableLoader::m_oPKRefVector%40A0FFF9029B
      //## begin ClMDTTableLoader::m_oPKRefVector%40A0FFF9029B.role preserve=no  private: ClMDTStringVector { -> VHAN}
      ClMDTStringVector m_oPKRefVector;
      //## end ClMDTTableLoader::m_oPKRefVector%40A0FFF9029B.role

    // Additional Private Declarations
      //## begin ClMDTTableLoader%4085256000B0.private preserve=yes
      //## end ClMDTTableLoader%4085256000B0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTTableLoader%4085256000B0.implementation preserve=yes
      //## end ClMDTTableLoader%4085256000B0.implementation

};

//## begin ClMDTTableLoader%4085256000B0.postscript preserve=yes
//## end ClMDTTableLoader%4085256000B0.postscript

//## begin ClMDTProcTemplateImpl%40E2781C021C.preface preserve=yes
//## end ClMDTProcTemplateImpl%40E2781C021C.preface

//## Class: ClMDTProcTemplateImpl%40E2781C021C
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40E936BC00EA;ClMDTTableLoader { -> }

class ClMDTProcTemplateImpl 
{
  //## begin ClMDTProcTemplateImpl%40E2781C021C.initialDeclarations preserve=yes
  //## end ClMDTProcTemplateImpl%40E2781C021C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTProcTemplateImpl%1088582842
      ClMDTProcTemplateImpl (ClMDTTableLoader& ai_oLoader, IMDTService& ai_oService);


    //## Other Operations (specified)
      //## Operation: Presentation2OBL%1088582841
      m4return_t Presentation2OBL (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate);

      //## Operation: BLOB2File%1089021409
      m4return_t BLOB2File (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate);

      m4return_t BLOB2External (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate);

	  m4return_t PresentationPackage2External (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate);

	  m4return_t Field2File (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate);

    // Additional Public Declarations
      //## begin ClMDTProcTemplateImpl%40E2781C021C.public preserve=yes
      //## end ClMDTProcTemplateImpl%40E2781C021C.public

  protected:
    // Additional Protected Declarations
      //## begin ClMDTProcTemplateImpl%40E2781C021C.protected preserve=yes
      //## end ClMDTProcTemplateImpl%40E2781C021C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oLoader%40E2A5A30161
      //## begin ClMDTProcTemplateImpl::m_oLoader%40E2A5A30161.attr preserve=no  private: ClMDTTableLoader& {UA} ai_oLoader
      ClMDTTableLoader& m_oLoader;
      //## end ClMDTProcTemplateImpl::m_oLoader%40E2A5A30161.attr

      //## Attribute: m_oService%40E2A61100F4
      //## begin ClMDTProcTemplateImpl::m_oService%40E2A61100F4.attr preserve=no  private: IMDTService& {UA} ai_oService
      IMDTService& m_oService;
      //## end ClMDTProcTemplateImpl::m_oService%40E2A61100F4.attr

    // Additional Private Declarations
      //## begin ClMDTProcTemplateImpl%40E2781C021C.private preserve=yes
      //## end ClMDTProcTemplateImpl%40E2781C021C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTProcTemplateImpl%40E2781C021C.implementation preserve=yes
      //## end ClMDTProcTemplateImpl%40E2781C021C.implementation

};

//## begin ClMDTProcTemplateImpl%40E2781C021C.postscript preserve=yes
//## end ClMDTProcTemplateImpl%40E2781C021C.postscript

//## begin ClMDTProcTemplate%40E2733A0307.preface preserve=yes
//## end ClMDTProcTemplate%40E2733A0307.preface

//## Class: ClMDTProcTemplate%40E2733A0307
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40E2998902A9;ClMDTProcTemplateImpl { -> }

typedef m4return_t (ClMDTProcTemplateImpl::* ClMDTProcTemplate)(m4int32_t ai_iReg,ClMDTTemplateFunction* ai_poTemplate);

//## begin ClMDTProcTemplate%40E2733A0307.postscript preserve=yes
//## end ClMDTProcTemplate%40E2733A0307.postscript

//## begin ClMDTStringTemplateMap%40E29A22025B.preface preserve=yes
//## end ClMDTStringTemplateMap%40E29A22025B.preface

//## Class: ClMDTStringTemplateMap%40E29A22025B; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40E937860109;ClMDTProcTemplate { -> }

typedef map< string,ClMDTProcTemplate,less<string> > ClMDTStringTemplateMap;

//## begin ClMDTStringTemplateMap%40E29A22025B.postscript preserve=yes
//## end ClMDTStringTemplateMap%40E29A22025B.postscript

//## begin ClMDTTransformerContext%407D0C0E0119.preface preserve=yes
//## end ClMDTTransformerContext%407D0C0E0119.preface

//## Class: ClMDTTransformerContext%407D0C0E0119
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%407D0EE60399;string { -> }

class ClMDTTransformerContext 
{
  //## begin ClMDTTransformerContext%407D0C0E0119.initialDeclarations preserve=yes
  //## end ClMDTTransformerContext%407D0C0E0119.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTTransformerContext%1081852930
      ClMDTTransformerContext (string& ai_sType, ClMDTTransformerContext* ai_poParent, IMDTService* ai_poService, string& ai_sConnectionId, ClDoubleXMLStreamGenerator* &ai_poOutput);

    //## Destructor (specified)
      //## Operation: ~ClMDTTransformerContext%1081852916
      virtual ~ClMDTTransformerContext ();


    //## Other Operations (specified)
      //## Operation: AddParameter%1081852922
      m4void_t AddParameter (string ai_sName, string ai_sValue);

      //## Operation: AddOption%1086775445
      m4void_t AddOption (string ai_sName, string ai_sValue);

      //## Operation: FlushTable%1081852925
      m4return_t FlushTable ();

      //## Operation: GetType%1081852926
      string GetType () const;

      //## Operation: ProcValue%1081852929
      m4return_t ProcValue (string& ai_sValue, m4int32_t ai_iParentReg, ClMDTValue& ao_sRes);

      //## Operation: AddChild%1081852931
      m4void_t AddChild (ClMDTTransformerContext* ai_poChild);

      //## Operation: AddPK%1081852935
      m4void_t AddPK (string ai_sName, string ai_sValue);

      //## Operation: GetPK%1082721863
      m4return_t GetPK (string& ai_sName, string& ao_sValue) const;

      m4void_t AddPKOperation (string ai_sName, string ai_sValue);
      m4return_t GetPKOperation (string& ai_sName, string& ao_sValue) const;

      //## Operation: GetParent%1082385406
      ClMDTTransformerContext* GetParent () const;

      //## Operation: GetConnectionId%1082721862
      string GetConnectionId () const;

      //## Operation: GetNRecords%1082721866
      m4int32_t GetNRecords () const;

      //## Operation: AddFieldProperty%1083066178
      m4void_t AddFieldProperty (string& ai_sFieldName, m4pcchar_t ai_pccName, string& ai_sValue);

      //## Operation: CheckFieldProp%1083066179
      m4bool_t CheckFieldProp (string& ai_sFieldName, string& ai_sProp, string& aio_sValue);

      //## Operation: DumpParameters%1083145763
      m4return_t DumpParameters ();

      //## Operation: IsRoot%1083166615
      m4bool_t IsRoot () const;

      //## Operation: DumpChilds%1083244865
      m4return_t DumpChilds (m4int32_t ai_iChilds);

      //## Operation: AddTemplate%1084288696
      m4return_t AddTemplate (string& ai_sName, const string& ai_sOutputName);

      //## Operation: AddTemplateArg%1084288697
      m4return_t AddTemplateArg (string& ai_sName, string& ai_sValue);

      //## Operation: AddCalculatedField%1084347794
      m4void_t AddCalculatedField (string& ai_sName);

      //## Operation: GenerateUpdateFunction%1084347797
      string GenerateUpdateFunction ();

      //## Operation: ProcGenerateFunctions%1084353848
      m4return_t ProcGenerateFunctions ();

      //## Operation: PopulateCalculatedFields%1084353849
      m4return_t PopulateCalculatedFields ();

      //## Operation: GetCalculatedFieldsSet%1084353851
      ClMDTStringSet& GetCalculatedFieldsSet ();

      //## Operation: ExecuteTemplates%1084353852
      m4return_t ExecuteTemplates ();

      //## Operation: GetNPKs%1085073893
      m4uint32_t GetNPKs () const;

      //## Operation: GetParamPKIterator%1085073894
      ClMDTStringMap::const_iterator GetParamPKIterator () const;

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: IsReadTable%408F8FE201AE
      const m4bool_t& GetIsReadTable () const;
      void SetIsReadTable (const m4bool_t& value);

      //## Attribute: GenerateFunction%40A0F4F703B5
      const m4bool_t& GetGenerateFunction () const;
      void SetGenerateFunction (const m4bool_t& value);

      //## Attribute: CalculatedFields%40A1D64102A0
      const m4bool_t& GetCalculatedFields () const;
      void SetCalculatedFields (const m4bool_t& value);

      //## Attribute: HasData%40A49CD801A5
      const m4bool_t& GetHasData () const;
      void SetHasData (const m4bool_t& value);

      const m4bool_t& GetHasExcludedData () const;
      void SetHasExcludedData (const m4bool_t& value);

      //## Attribute: DBName%40C71FE00328
      const string& GetDBName () const;
      void SetDBName (const string& value);

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40E29A7800C5
      //## Role: ClMDTTransformerContext::m_oTemplateImplMap%40E29A78023D
      //## begin ClMDTTransformerContext::m_oTemplateImplMap%40E29A78023D.role preserve=no  public: ClMDTStringTemplateMap { -> VHAN}
      ClMDTStringTemplateMap m_oTemplateImplMap;
      //## end ClMDTTransformerContext::m_oTemplateImplMap%40E29A78023D.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40E29B520048
      //## Role: ClMDTTransformerContext::m_oTemplateImpl%40E29B52023D
      //## begin ClMDTTransformerContext::m_oTemplateImpl%40E29B52023D.role preserve=no  public: ClMDTProcTemplateImpl { -> VHAN}
      ClMDTProcTemplateImpl m_oTemplateImpl;
      //## end ClMDTTransformerContext::m_oTemplateImpl%40E29B52023D.role

    // Additional Public Declarations
      //## begin ClMDTTransformerContext%407D0C0E0119.public preserve=yes
      //## end ClMDTTransformerContext%407D0C0E0119.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetParameter%1082385407
      m4return_t GetParameter (m4pcchar_t ai_pccParam, ClMDTValue& ao_sRes);

      //## Operation: GetOption%1086775447
      m4return_t GetOption (m4pcchar_t ai_pccParam, string& ao_sRes);

      //## Operation: GetField%1082967064
      m4return_t GetField (string ai_sName, m4int32_t ai_iReg, ClMDTValue& ao_vRes);

      //## Operation: GenerateTableFields%1082385409
      m4return_t GenerateTableFields ();

    // Data Members for Class Attributes

      //## begin ClMDTTransformerContext::IsReadTable%408F8FE201AE.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bIsReadTable;
      //## end ClMDTTransformerContext::IsReadTable%408F8FE201AE.attr

      //## begin ClMDTTransformerContext::GenerateFunction%40A0F4F703B5.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bGenerateFunction;
      //## end ClMDTTransformerContext::GenerateFunction%40A0F4F703B5.attr

      //## begin ClMDTTransformerContext::CalculatedFields%40A1D64102A0.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bCalculatedFields;
      //## end ClMDTTransformerContext::CalculatedFields%40A1D64102A0.attr

      //## begin ClMDTTransformerContext::HasData%40A49CD801A5.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bHasData;
      //## end ClMDTTransformerContext::HasData%40A49CD801A5.attr

      m4bool_t m_bHasExcludedData;

      //## begin ClMDTTransformerContext::DBName%40C71FE00328.attr preserve=no  public: string {U} 
      string m_sDBName;
      //## end ClMDTTransformerContext::DBName%40C71FE00328.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40A0FB1303BB
      //## Role: ClMDTTransformerContext::m_oTemplateVector%40A0FB140206
      //## begin ClMDTTransformerContext::m_oTemplateVector%40A0FB140206.role preserve=no  protected: ClMDTTemplateVector { -> VHAN}
      ClMDTTemplateVector m_oTemplateVector;
      //## end ClMDTTransformerContext::m_oTemplateVector%40A0FB140206.role

    // Additional Protected Declarations
      //## begin ClMDTTransformerContext%407D0C0E0119.protected preserve=yes
      //## end ClMDTTransformerContext%407D0C0E0119.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sType%407D0DC5029F
      //## begin ClMDTTransformerContext::m_sType%407D0DC5029F.attr preserve=no  private: string {UA} ai_sType
      string m_sType;
      //## end ClMDTTransformerContext::m_sType%407D0DC5029F.attr

      //## Attribute: m_poParent%407D420C02D3
      //## begin ClMDTTransformerContext::m_poParent%407D420C02D3.attr preserve=no  private: ClMDTTransformerContext* {UA} ai_poParent
      ClMDTTransformerContext* m_poParent;
      //## end ClMDTTransformerContext::m_poParent%407D420C02D3.attr

      //## Attribute: m_poChild%407D3E980025
      //## begin ClMDTTransformerContext::m_poChild%407D3E980025.attr preserve=no  private: ClMDTTransformerContext* {UA} NULL
      ClMDTTransformerContext* m_poChild;
      //## end ClMDTTransformerContext::m_poChild%407D3E980025.attr

      //## Attribute: m_poService%407D633F0271
      //## begin ClMDTTransformerContext::m_poService%407D633F0271.attr preserve=no  private: IMDTService* {UA} ai_poService
      IMDTService* m_poService;
      //## end ClMDTTransformerContext::m_poService%407D633F0271.attr

      //## Attribute: m_sConnectionId%408905DB02D6
      //## begin ClMDTTransformerContext::m_sConnectionId%408905DB02D6.attr preserve=no  private: string {UA} ai_sConnectionId
      string m_sConnectionId;
      //## end ClMDTTransformerContext::m_sConnectionId%408905DB02D6.attr

      //## Attribute: m_bIsFlushed%408E30360373
      //## begin ClMDTTransformerContext::m_bIsFlushed%408E30360373.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsFlushed;
      //## end ClMDTTransformerContext::m_bIsFlushed%408E30360373.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%407D679302B7
      //## Role: ClMDTTransformerContext::m_oParamMap%407D67940056
      //## begin ClMDTTransformerContext::m_oParamMap%407D67940056.role preserve=no  private: ClMDTStringMap { -> VHAN}
      ClMDTStringMap m_oParamMap;
      //## end ClMDTTransformerContext::m_oParamMap%407D67940056.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%407D68DA01A0
      //## Role: ClMDTTransformerContext::m_oPKMap%407D68DA0307
      //## begin ClMDTTransformerContext::m_oPKMap%407D68DA0307.role preserve=no  private: ClMDTStringMap { -> VHAN}
      ClMDTStringMap m_oPKMap;
      //## end ClMDTTransformerContext::m_oPKMap%407D68DA0307.role

      ClMDTStringMap m_oPKOperationMap;

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%4085259D036F
      //## Role: ClMDTTransformerContext::m_oLoader%4085259E012E
      //## begin ClMDTTransformerContext::m_oLoader%4085259E012E.role preserve=no  private: ClMDTTableLoader {1 -> VHAN}
      ClMDTTableLoader m_oLoader;
      //## end ClMDTTransformerContext::m_oLoader%4085259E012E.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E6241023A
      //## Role: ClMDTTransformerContext::m_oFieldPropMap%408E62420009
      //## begin ClMDTTransformerContext::m_oFieldPropMap%408E62420009.role preserve=no  private: ClMDTStringMap { -> VHAN}
      ClMDTStringMap m_oFieldPropMap;
      //## end ClMDTTransformerContext::m_oFieldPropMap%408E62420009.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E8A940373
      //## Role: ClMDTTransformerContext::m_poOutput%408E8A960009
      //## begin ClMDTTransformerContext::m_poOutput%408E8A960009.role preserve=no  private: ClDoubleXMLStreamGenerator { -> RHAN}
      ClDoubleXMLStreamGenerator *m_poOutput;
      //## end ClMDTTransformerContext::m_poOutput%408E8A960009.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40A1D4B9009E
      //## Role: ClMDTTransformerContext::m_oCalculatedFieldsSet%40A1D4BA02C2
      //## begin ClMDTTransformerContext::m_oCalculatedFieldsSet%40A1D4BA02C2.role preserve=no  private: ClMDTStringSet { -> VHAN}
      ClMDTStringSet m_oCalculatedFieldsSet;
      //## end ClMDTTransformerContext::m_oCalculatedFieldsSet%40A1D4BA02C2.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40C6E539021D
      //## Role: ClMDTTransformerContext::m_oOptionsMap%40C6E53903D2
      //## begin ClMDTTransformerContext::m_oOptionsMap%40C6E53903D2.role preserve=no  private: ClMDTStringMap { -> VHAN}
      ClMDTStringMap m_oOptionsMap;
      //## end ClMDTTransformerContext::m_oOptionsMap%40C6E53903D2.role

    // Additional Private Declarations
      //## begin ClMDTTransformerContext%407D0C0E0119.private preserve=yes
      //## end ClMDTTransformerContext%407D0C0E0119.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTTransformerContext%407D0C0E0119.implementation preserve=yes
      //## end ClMDTTransformerContext%407D0C0E0119.implementation

};

//## begin ClMDTTransformerContext%407D0C0E0119.postscript preserve=yes
//## end ClMDTTransformerContext%407D0C0E0119.postscript

//## begin ClMDTContextManager%407D5089020F.preface preserve=yes
//## end ClMDTContextManager%407D5089020F.preface

//## Class: ClMDTContextManager%407D5089020F
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClMDTContextManager 
{
  //## begin ClMDTContextManager%407D5089020F.initialDeclarations preserve=yes
  //## end ClMDTContextManager%407D5089020F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTContextManager%1081852936
      ClMDTContextManager (IMDTService* ai_poService, string& ai_sConnectionId, ClDoubleXMLStreamGenerator* &ai_poOutput);


    //## Other Operations (specified)
      //## Operation: AddContext%1081852932
      m4return_t AddContext (string& ai_sType);

      //## Operation: LeaveContext%1081852933
      m4return_t LeaveContext ();

      //## Operation: GetCurrent%1081852934
      ClMDTTransformerContext& GetCurrent () const;

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: HasData%40A4BDF30278
      const m4bool_t& GetHasData () const;
      void SetHasData (const m4bool_t& value);

      const m4bool_t& GetHasExcludedData () const;
      void SetHasExcludedData (const m4bool_t& value);

    // Additional Public Declarations
      //## begin ClMDTContextManager%407D5089020F.public preserve=yes
      //## end ClMDTContextManager%407D5089020F.public

  protected:
    // Data Members for Class Attributes

      //## begin ClMDTContextManager::HasData%40A4BDF30278.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bHasData;
      //## end ClMDTContextManager::HasData%40A4BDF30278.attr

      m4bool_t m_bHasExcludedData;

    // Additional Protected Declarations
      //## begin ClMDTContextManager%407D5089020F.protected preserve=yes
      //## end ClMDTContextManager%407D5089020F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poService%407D621A005C
      //## begin ClMDTContextManager::m_poService%407D621A005C.attr preserve=no  private: IMDTService* {UA} ai_poService
      IMDTService* m_poService;
      //## end ClMDTContextManager::m_poService%407D621A005C.attr

      //## Attribute: m_sConnectionId%408906FF03DF
      //## begin ClMDTContextManager::m_sConnectionId%408906FF03DF.attr preserve=no  private: string {UA} ai_sConnectionId
      string m_sConnectionId;
      //## end ClMDTContextManager::m_sConnectionId%408906FF03DF.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%407D50CB0191
      //## Role: ClMDTContextManager::m_poContextList%407D50CB0338
      //## begin ClMDTContextManager::m_poContextList%407D50CB0338.role preserve=no  private: ClMDTTransformerContext { -> RHAN}
      ClMDTTransformerContext *m_poContextList;
      //## end ClMDTContextManager::m_poContextList%407D50CB0338.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%407D5CA702D4
      //## Role: ClMDTContextManager::m_poCurrent%407D5CA703BF
      //## begin ClMDTContextManager::m_poCurrent%407D5CA703BF.role preserve=no  private: ClMDTTransformerContext { -> RHAN}
      ClMDTTransformerContext *m_poCurrent;
      //## end ClMDTContextManager::m_poCurrent%407D5CA703BF.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E89EA0056
      //## Role: ClMDTContextManager::m_poOutput%408E89F10057
      //## begin ClMDTContextManager::m_poOutput%408E89F10057.role preserve=no  private: ClDoubleXMLStreamGenerator { -> RHAN}
      ClDoubleXMLStreamGenerator* &m_poOutput;
      //## end ClMDTContextManager::m_poOutput%408E89F10057.role

    // Additional Private Declarations
      //## begin ClMDTContextManager%407D5089020F.private preserve=yes
      //## end ClMDTContextManager%407D5089020F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTContextManager%407D5089020F.implementation preserve=yes
      //## end ClMDTContextManager%407D5089020F.implementation

};

//## begin ClMDTContextManager%407D5089020F.postscript preserve=yes
//## end ClMDTContextManager%407D5089020F.postscript

// Class ClMDTTemplateFunction 

// Class ClMDTTableLoader 


//## Other Operations (inline)
inline m4void_t ClMDTTableLoader::SetContext (ClMDTTransformerContext* ai_poContext)
{
  //## begin ClMDTTableLoader::SetContext%1082721861.body preserve=yes
	m_poContext = ai_poContext;
  //## end ClMDTTableLoader::SetContext%1082721861.body
}

inline m4int32_t ClMDTTableLoader::GetNRecords () const
{
  //## begin ClMDTTableLoader::GetNRecords%1082721864.body preserve=yes
	return m_oResultSet.GetRowNumber();
  //## end ClMDTTableLoader::GetNRecords%1082721864.body
}

// Class ClMDTProcTemplateImpl 

// Class ClMDTTransformerContext 


//## Other Operations (inline)
inline string ClMDTTransformerContext::GetType () const
{
  //## begin ClMDTTransformerContext::GetType%1081852926.body preserve=yes
	return m_sType;
  //## end ClMDTTransformerContext::GetType%1081852926.body
}

inline m4void_t ClMDTTransformerContext::AddChild (ClMDTTransformerContext* ai_poChild)
{
  //## begin ClMDTTransformerContext::AddChild%1081852931.body preserve=yes
	m_poChild = ai_poChild;
  //## end ClMDTTransformerContext::AddChild%1081852931.body
}

inline m4void_t ClMDTTransformerContext::AddPK (string ai_sName, string ai_sValue)
{
  //## begin ClMDTTransformerContext::AddPK%1081852935.body preserve=yes
	m_oPKMap.insert(ClMDTStringMap::value_type(ai_sName,ai_sValue));
  //## end ClMDTTransformerContext::AddPK%1081852935.body
}

inline m4return_t ClMDTTransformerContext::GetPK (string& ai_sName, string& ao_sValue) const
{
  //## begin ClMDTTransformerContext::GetPK%1082721863.body preserve=yes
	ClMDTStringMap::const_iterator oIt = m_oPKMap.find(ai_sName);
		
	if (oIt == m_oPKMap.end()) {
		ao_sValue = "";
		return M4_WARNING;
	}
	ao_sValue = (*oIt).second;
	return M4_SUCCESS;
  //## end ClMDTTransformerContext::GetPK%1082721863.body
}

inline m4void_t ClMDTTransformerContext::AddPKOperation (string ai_sName, string ai_sValue)
{
	m_oPKOperationMap.insert( ClMDTStringMap::value_type( ai_sName, ai_sValue ) );
}

inline m4return_t ClMDTTransformerContext::GetPKOperation (string& ai_sName, string& ao_sValue) const
{
	ao_sValue = "=";

	if( m_oPKOperationMap.size() == 0 )
	{
		return( M4_SUCCESS );
	}

	ClMDTStringMap::const_iterator oIt = m_oPKOperationMap.find( ai_sName );

	if( oIt == m_oPKOperationMap.end() )
	{
		return( M4_SUCCESS );
	}

	ao_sValue = (*oIt).second;

	return( M4_SUCCESS );
}

inline ClMDTTransformerContext* ClMDTTransformerContext::GetParent () const
{
  //## begin ClMDTTransformerContext::GetParent%1082385406.body preserve=yes
	return m_poParent;
  //## end ClMDTTransformerContext::GetParent%1082385406.body
}

inline string ClMDTTransformerContext::GetConnectionId () const
{
  //## begin ClMDTTransformerContext::GetConnectionId%1082721862.body preserve=yes
	if (m_sDBName.size()) {
		return m_sDBName;
	}

	return m_sConnectionId;
  //## end ClMDTTransformerContext::GetConnectionId%1082721862.body
}

inline m4int32_t ClMDTTransformerContext::GetNRecords () const
{
  //## begin ClMDTTransformerContext::GetNRecords%1082721866.body preserve=yes
	return m_oLoader.GetNRecords();
  //## end ClMDTTransformerContext::GetNRecords%1082721866.body
}

inline m4void_t ClMDTTransformerContext::AddFieldProperty (string& ai_sFieldName, m4pcchar_t ai_pccName, string& ai_sValue)
{
  //## begin ClMDTTransformerContext::AddFieldProperty%1083066178.body preserve=yes
	if (ai_sValue.empty()) {
		return;
	}

	m_oFieldPropMap.insert(ClMDTStringMap::value_type(ai_sFieldName+":"+ai_pccName,ai_sValue));
  //## end ClMDTTransformerContext::AddFieldProperty%1083066178.body
}

inline m4bool_t ClMDTTransformerContext::IsRoot () const
{
  //## begin ClMDTTransformerContext::IsRoot%1083166615.body preserve=yes
	if (GetParent() == NULL) {
		return M4_TRUE;
	}

	return GetParent()->m_oPKMap.empty() ? M4_TRUE : M4_FALSE;
  //## end ClMDTTransformerContext::IsRoot%1083166615.body
}

inline ClMDTStringSet& ClMDTTransformerContext::GetCalculatedFieldsSet ()
{
  //## begin ClMDTTransformerContext::GetCalculatedFieldsSet%1084353851.body preserve=yes
	return m_oCalculatedFieldsSet;
  //## end ClMDTTransformerContext::GetCalculatedFieldsSet%1084353851.body
}

inline m4uint32_t ClMDTTransformerContext::GetNPKs () const
{
  //## begin ClMDTTransformerContext::GetNPKs%1085073893.body preserve=yes
	return m_oPKMap.size();
  //## end ClMDTTransformerContext::GetNPKs%1085073893.body
}

inline ClMDTStringMap::const_iterator ClMDTTransformerContext::GetParamPKIterator () const
{
  //## begin ClMDTTransformerContext::GetParamPKIterator%1085073894.body preserve=yes
	return m_oPKMap.begin();
  //## end ClMDTTransformerContext::GetParamPKIterator%1085073894.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4bool_t& ClMDTTransformerContext::GetIsReadTable () const
{
  //## begin ClMDTTransformerContext::GetIsReadTable%408F8FE201AE.get preserve=no
  return m_bIsReadTable;
  //## end ClMDTTransformerContext::GetIsReadTable%408F8FE201AE.get
}

inline void ClMDTTransformerContext::SetIsReadTable (const m4bool_t& value)
{
  //## begin ClMDTTransformerContext::SetIsReadTable%408F8FE201AE.set preserve=no
  m_bIsReadTable = value;
  //## end ClMDTTransformerContext::SetIsReadTable%408F8FE201AE.set
}

inline const m4bool_t& ClMDTTransformerContext::GetGenerateFunction () const
{
  //## begin ClMDTTransformerContext::GetGenerateFunction%40A0F4F703B5.get preserve=no
  return m_bGenerateFunction;
  //## end ClMDTTransformerContext::GetGenerateFunction%40A0F4F703B5.get
}

inline void ClMDTTransformerContext::SetGenerateFunction (const m4bool_t& value)
{
  //## begin ClMDTTransformerContext::SetGenerateFunction%40A0F4F703B5.set preserve=no
  m_bGenerateFunction = value;
  //## end ClMDTTransformerContext::SetGenerateFunction%40A0F4F703B5.set
}

inline const m4bool_t& ClMDTTransformerContext::GetCalculatedFields () const
{
  //## begin ClMDTTransformerContext::GetCalculatedFields%40A1D64102A0.get preserve=no
  return m_bCalculatedFields;
  //## end ClMDTTransformerContext::GetCalculatedFields%40A1D64102A0.get
}

inline void ClMDTTransformerContext::SetCalculatedFields (const m4bool_t& value)
{
  //## begin ClMDTTransformerContext::SetCalculatedFields%40A1D64102A0.set preserve=no
  m_bCalculatedFields = value;
  //## end ClMDTTransformerContext::SetCalculatedFields%40A1D64102A0.set
}

inline const m4bool_t& ClMDTTransformerContext::GetHasData () const
{
  //## begin ClMDTTransformerContext::GetHasData%40A49CD801A5.get preserve=no
  return m_bHasData;
  //## end ClMDTTransformerContext::GetHasData%40A49CD801A5.get
}

inline void ClMDTTransformerContext::SetHasData (const m4bool_t& value)
{
  //## begin ClMDTTransformerContext::SetHasData%40A49CD801A5.set preserve=no
  m_bHasData = value;
  //## end ClMDTTransformerContext::SetHasData%40A49CD801A5.set
}

inline const m4bool_t& ClMDTTransformerContext::GetHasExcludedData () const
{
  return m_bHasExcludedData;
}

inline void ClMDTTransformerContext::SetHasExcludedData (const m4bool_t& value)
{
  m_bHasExcludedData = value;
}

inline const string& ClMDTTransformerContext::GetDBName () const
{
  //## begin ClMDTTransformerContext::GetDBName%40C71FE00328.get preserve=no
  return m_sDBName;
  //## end ClMDTTransformerContext::GetDBName%40C71FE00328.get
}

inline void ClMDTTransformerContext::SetDBName (const string& value)
{
  //## begin ClMDTTransformerContext::SetDBName%40C71FE00328.set preserve=no
  m_sDBName = value;
  //## end ClMDTTransformerContext::SetDBName%40C71FE00328.set
}

// Class ClMDTContextManager 


//## Other Operations (inline)
inline ClMDTTransformerContext& ClMDTContextManager::GetCurrent () const
{
  //## begin ClMDTContextManager::GetCurrent%1081852934.body preserve=yes
	return *m_poCurrent;
  //## end ClMDTContextManager::GetCurrent%1081852934.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4bool_t& ClMDTContextManager::GetHasData () const
{
  //## begin ClMDTContextManager::GetHasData%40A4BDF30278.get preserve=no
  return m_bHasData;
  //## end ClMDTContextManager::GetHasData%40A4BDF30278.get
}

inline void ClMDTContextManager::SetHasData (const m4bool_t& value)
{
  //## begin ClMDTContextManager::SetHasData%40A4BDF30278.set preserve=no
  m_bHasData = value;
  //## end ClMDTContextManager::SetHasData%40A4BDF30278.set
}

inline const m4bool_t& ClMDTContextManager::GetHasExcludedData () const
{
  return m_bHasExcludedData;
}

inline void ClMDTContextManager::SetHasExcludedData (const m4bool_t& value)
{
  m_bHasExcludedData = value;
}

//## begin module%407D0C7C0119.epilog preserve=yes
//## end module%407D0C7C0119.epilog


#endif
