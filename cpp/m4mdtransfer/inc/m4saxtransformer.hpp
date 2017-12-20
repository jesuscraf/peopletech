//## begin module%407ABBB9001D.cm preserve=no
//## end module%407ABBB9001D.cm

//## begin module%407ABBB9001D.cp preserve=no
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
//## end module%407ABBB9001D.cp

//## Module: m4saxtransformer%407ABBB9001D; Package specification
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Source file: D:\Work\v600\m4mdtransfer\inc\m4saxtransformer.hpp

#ifndef m4saxtransformer_h
#define m4saxtransformer_h 1

//## begin module%407ABBB9001D.additionalIncludes preserve=no
//## end module%407ABBB9001D.additionalIncludes

//## begin module%407ABBB9001D.includes preserve=yes
//## end module%407ABBB9001D.includes

#include "m4stl.hpp"
#include <m4transctx.hpp>
#include <m4imdtparameter.hpp>
#include <m4imdtdictionary.hpp>
#include <xmlstreamgenerator.hpp>
#include <m4xmlbase.hpp>

//## begin module%407ABBB9001D.declarations preserve=no
//## end module%407ABBB9001D.declarations

//## begin module%407ABBB9001D.additionalDeclarations preserve=yes
class IMDTService;
//## end module%407ABBB9001D.additionalDeclarations


//## begin ClMDTDataSAXTransformer%40740F5B0334.preface preserve=yes
//## end ClMDTDataSAXTransformer%40740F5B0334.preface

//## Class: ClMDTDataSAXTransformer%40740F5B0334
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40741FF90315; { -> }
//## Uses: <unnamed>%407D60730357;IMDTReadParameterManager { -> }
//## Uses: <unnamed>%408FD4DF00C3;IMDTDictionary { -> }

class ClMDTDataSAXTransformer : public M4SAXHandlerBase  //## Inherits: <unnamed>%407BE8870290
{
  //## begin ClMDTDataSAXTransformer%40740F5B0334.initialDeclarations preserve=yes
  //## end ClMDTDataSAXTransformer%40740F5B0334.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTDataSAXTransformer%1081852904
      ClMDTDataSAXTransformer (IMDTService* ai_poService, string& ai_sConnectionId);

    //## Destructor (specified)
      //## Operation: ~ClMDTDataSAXTransformer%1081852911
      virtual ~ClMDTDataSAXTransformer ();


    //## Other Operations (specified)
      //## Operation: endDocument%1081852902
      m4void_t endDocument ();

      //## Operation: startDocument%1081852903
      m4void_t startDocument ();

      //## Operation: Reset%1081852905
      m4return_t Reset (m4pcchar_t ai_pccConnectionId, m4pcchar_t ai_pccObjId, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds);

    // Additional Public Declarations
      //## begin ClMDTDataSAXTransformer%40740F5B0334.public preserve=yes
      //## end ClMDTDataSAXTransformer%40740F5B0334.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetValue%1086694050
      m4return_t GetValue (const string& ai_sName, string& aio_sValue);

      //## Operation: Release%1084529907
      m4return_t Release ();

      //## Operation: WriteInitMessage%1081852906
      m4return_t WriteInitMessage ();

      //## Operation: InitParameters%1081852921
      m4return_t InitParameters ();

      //## Operation: procObject%1081852910
      m4void_t procObject (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndObject%1081852909
      m4void_t procEndObject (const XMLCh * const name);

      //## Operation: procCallTable%1081852923
      m4void_t procCallTable (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndCallTable%1081852924
      m4void_t procEndCallTable (const XMLCh * const name);

      //## Operation: procWithPK%1081852928
      m4void_t procWithPK (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procField%1083066177
      m4void_t procField (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procReadTable%1083150386
      m4void_t procReadTable (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndReadTable%1083150387
      m4void_t procEndReadTable (const XMLCh * const name);

      //## Operation: procIncludeObject%1083166611
      m4void_t procIncludeObject (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndIncludeObject%1083166612
      m4void_t procEndIncludeObject (const XMLCh * const name);

      //## Operation: procWithParam%1083166613
      m4void_t procWithParam (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procCallObject%1083244863
      m4void_t procCallObject (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndCallObject%1083244864
      m4void_t procEndCallObject (const XMLCh * const name);

      //## Operation: procCallTemplate%1084288691
      m4void_t procCallTemplate (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procWithArg%1084288692
      m4void_t procWithArg (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procGenerateFunctions%1084288693
      m4void_t procGenerateFunctions (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procUpdateFunctions%1084288694
      m4void_t procUpdateFunctions (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procCalculatedFields%1084347795
      m4void_t procCalculatedFields (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procEndCalculatedFields%1084347796
      m4void_t procEndCalculatedFields (const XMLCh* const name);

      //## Operation: procOptions%1086775446
      m4void_t procOptions (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procParam%1086778455
      m4void_t procParam (const XMLCh* const name, AttributeList& attributes);

    // Additional Protected Declarations
      //## begin ClMDTDataSAXTransformer%40740F5B0334.protected preserve=yes
      //## end ClMDTDataSAXTransformer%40740F5B0334.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iPKs%407C23530203
      //## begin ClMDTDataSAXTransformer::m_iPKs%407C23530203.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iPKs;
      //## end ClMDTDataSAXTransformer::m_iPKs%407C23530203.attr

      //## Attribute: m_iChilds%407C235E0399
      //## begin ClMDTDataSAXTransformer::m_iChilds%407C235E0399.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iChilds;
      //## end ClMDTDataSAXTransformer::m_iChilds%407C235E0399.attr

      //## Attribute: m_bInit%407CED4401D4
      //## begin ClMDTDataSAXTransformer::m_bInit%407CED4401D4.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bInit;
      //## end ClMDTDataSAXTransformer::m_bInit%407CED4401D4.attr

      //## Attribute: m_sObjId%407CF4250109
      //## begin ClMDTDataSAXTransformer::m_sObjId%407CF4250109.attr preserve=no  private: string {UA} 
      string m_sObjId;
      //## end ClMDTDataSAXTransformer::m_sObjId%407CF4250109.attr

      //## Attribute: m_poService%407D5FCA00E5
      //## begin ClMDTDataSAXTransformer::m_poService%407D5FCA00E5.attr preserve=no  private: IMDTService* {UA} ai_poService
      IMDTService* m_poService;
      //## end ClMDTDataSAXTransformer::m_poService%407D5FCA00E5.attr

      //## Attribute: m_sConnectionId%4086979900DF
      //## begin ClMDTDataSAXTransformer::m_sConnectionId%4086979900DF.attr preserve=no  private: string {UA} 
      string m_sConnectionId;
      //## end ClMDTDataSAXTransformer::m_sConnectionId%4086979900DF.attr

      //## Attribute: m_iLevel%4090E6450046
      //## begin ClMDTDataSAXTransformer::m_iLevel%4090E6450046.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iLevel;
      //## end ClMDTDataSAXTransformer::m_iLevel%4090E6450046.attr

      //## Attribute: m_sOutputName%40A49F190385
      //## begin ClMDTDataSAXTransformer::m_sOutputName%40A49F190385.attr preserve=no  private: string {UA} 
      string m_sOutputName;
      //## end ClMDTDataSAXTransformer::m_sOutputName%40A49F190385.attr

      string m_sExclusionOutputName;

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%407D52120340
      //## Role: ClMDTDataSAXTransformer::m_oCManager%407D52130063
      //## begin ClMDTDataSAXTransformer::m_oCManager%407D52130063.role preserve=no  private: ClMDTContextManager { -> VHAN}
      ClMDTContextManager m_oCManager;
      //## end ClMDTDataSAXTransformer::m_oCManager%407D52130063.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E81CD03E0
      //## Role: ClMDTDataSAXTransformer::m_poOutput%408E81CE0345
      //## begin ClMDTDataSAXTransformer::m_poOutput%408E81CE0345.role preserve=no  private: ClDoubleXMLStreamGenerator { -> RHAN}
      ClDoubleXMLStreamGenerator *m_poOutput;
      //## end ClMDTDataSAXTransformer::m_poOutput%408E81CE0345.role

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%408E8257020B
      //## Role: ClMDTDataSAXTransformer::m_poOutStream%408E82580009
      //## begin ClMDTDataSAXTransformer::m_poOutStream%408E82580009.role preserve=no  private: ofstream { -> RHAN}
      ofstream *m_poOutStream;
      //## end ClMDTDataSAXTransformer::m_poOutStream%408E82580009.role

      ofstream *m_poExclusionOutStream;

    // Additional Private Declarations
      //## begin ClMDTDataSAXTransformer%40740F5B0334.private preserve=yes
      //## end ClMDTDataSAXTransformer%40740F5B0334.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTDataSAXTransformer%40740F5B0334.implementation preserve=yes
      //## end ClMDTDataSAXTransformer%40740F5B0334.implementation

};

//## begin ClMDTDataSAXTransformer%40740F5B0334.postscript preserve=yes
//## end ClMDTDataSAXTransformer%40740F5B0334.postscript

// Class ClMDTDataSAXTransformer 

//## begin module%407ABBB9001D.epilog preserve=yes
//## end module%407ABBB9001D.epilog


#endif
