//## begin module%37E8E4F4023D.cm preserve=no
//## end module%37E8E4F4023D.cm

//## begin module%37E8E4F4023D.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%37E8E4F4023D.cp

//## Module: Executive_M4XML_SAX%37E8E4F4023D; Package specification
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Source file: C:\m4server-xerces2.5.0\m4exeforchannel\inc\srvm4xmlsax.hpp

#ifndef executive_m4xml_sax_h
#define executive_m4xml_sax_h 1

//## begin module%37E8E4F4023D.additionalIncludes preserve=no
//## end module%37E8E4F4023D.additionalIncludes

//## begin module%37E8E4F4023D.includes preserve=yes
//## end module%37E8E4F4023D.includes

#include "m4stl.hpp"
#include <m4xmlutil.hpp>
#include <serverm4xml.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/HandlerBase.hpp>

class M4XMLSAXHandlers;



//## begin module%37E8E4F4023D.declarations preserve=no
//## end module%37E8E4F4023D.declarations

//## begin module%37E8E4F4023D.additionalDeclarations preserve=yes
//## end module%37E8E4F4023D.additionalDeclarations


//## begin ClM4XMLErrorHandler%380F41150285.preface preserve=yes
//## end ClM4XMLErrorHandler%380F41150285.preface

//## Class: ClM4XMLErrorHandler%380F41150285
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EXEFORCHANNEL ClM4XMLErrorHandler : public ErrorHandler  //## Inherits: <unnamed>%380F416F039D
{
  //## begin ClM4XMLErrorHandler%380F41150285.initialDeclarations preserve=yes
  //## end ClM4XMLErrorHandler%380F41150285.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLErrorHandler%941014276
      ClM4XMLErrorHandler ();


    //## Other Operations (specified)
      //## Operation: warning%940504735
      void warning (const SAXParseException& exception);

      //## Operation: error%940504736
      void error (const SAXParseException& exception);

      //## Operation: fatalError%940504737
      void fatalError (const SAXParseException& exception);

      //## Operation: resetErrors%940504738
      void resetErrors ();

      //## Operation: DumpMessage%940504739
      void DumpMessage (m4pcchar_t ai_pcMessage, const SAXParseException& exception);

      //## Operation: IsOk%940504740
      m4bool_t IsOk () const;

    // Additional Public Declarations
      //## begin ClM4XMLErrorHandler%380F41150285.public preserve=yes
      //## end ClM4XMLErrorHandler%380F41150285.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4XMLErrorHandler%380F41150285.protected preserve=yes
      //## end ClM4XMLErrorHandler%380F41150285.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsOk%380F4BE1010D
      //## begin ClM4XMLErrorHandler::m_bIsOk%380F4BE1010D.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bIsOk;
      //## end ClM4XMLErrorHandler::m_bIsOk%380F4BE1010D.attr

    // Additional Private Declarations
      //## begin ClM4XMLErrorHandler%380F41150285.private preserve=yes
      //## end ClM4XMLErrorHandler%380F41150285.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLErrorHandler%380F41150285.implementation preserve=yes
      //## end ClM4XMLErrorHandler%380F41150285.implementation

};

//## begin ClM4XMLErrorHandler%380F41150285.postscript preserve=yes
//## end ClM4XMLErrorHandler%380F41150285.postscript


//## begin FilterArg%37F9C78B0082.preface preserve=yes
//## end FilterArg%37F9C78B0082.preface

//## Class: FilterArg%37F9C78B0082
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class FilterArg 
{
  //## begin FilterArg%37F9C78B0082.initialDeclarations preserve=yes
  //## end FilterArg%37F9C78B0082.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: FilterArg%945198101
      FilterArg ();

      //## Operation: FilterArg%939117430
      FilterArg (const string &ai_sMode, const string &ai_sName, const string &ai_sValue);

    // Data Members for Class Attributes

      //## Attribute: sMode%37F9C8140101
      //## begin FilterArg::sMode%37F9C8140101.attr preserve=no  public: string {UA} 
      string sMode;
      //## end FilterArg::sMode%37F9C8140101.attr

      //## Attribute: sName%37F9C7A80391
      //## begin FilterArg::sName%37F9C7A80391.attr preserve=no  public: string {UA} 
      string sName;
      //## end FilterArg::sName%37F9C7A80391.attr

      //## Attribute: sValue%37F9D0CA0248
      //## begin FilterArg::sValue%37F9D0CA0248.attr preserve=no  public: string {UA} 
      string sValue;
      //## end FilterArg::sValue%37F9D0CA0248.attr

    // Additional Public Declarations
      //## begin FilterArg%37F9C78B0082.public preserve=yes
      //## end FilterArg%37F9C78B0082.public

  protected:
    // Additional Protected Declarations
      //## begin FilterArg%37F9C78B0082.protected preserve=yes
      //## end FilterArg%37F9C78B0082.protected

  private:
    // Additional Private Declarations
      //## begin FilterArg%37F9C78B0082.private preserve=yes
      //## end FilterArg%37F9C78B0082.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin FilterArg%37F9C78B0082.implementation preserve=yes
      //## end FilterArg%37F9C78B0082.implementation

};

//## begin FilterArg%37F9C78B0082.postscript preserve=yes
//## end FilterArg%37F9C78B0082.postscript

//## begin FilterArgVector%37F9C9740085.preface preserve=yes
//## end FilterArgVector%37F9C9740085.preface

//## Class: FilterArgVector%37F9C9740085; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38569ABB020F;FilterArg { -> }

typedef vector< FilterArg > FilterArgVector;

//## begin FilterArgVector%37F9C9740085.postscript preserve=yes
//## end FilterArgVector%37F9C9740085.postscript

//## begin FilterArgIt%37F9C9EE0207.preface preserve=yes
//## end FilterArgIt%37F9C9EE0207.preface

//## Class: FilterArgIt%37F9C9EE0207
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F9CA1F035C;FilterArgVector { -> }

typedef FilterArgVector::iterator FilterArgIt;

//## begin FilterArgIt%37F9C9EE0207.postscript preserve=yes
//## end FilterArgIt%37F9C9EE0207.postscript

//## begin ElementHandler%37E9243F03E1.preface preserve=yes
//## end ElementHandler%37E9243F03E1.preface

//## Class: ElementHandler%37E9243F03E1; Instantiated Class Utility
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3826D97400AF;M4XMLSAXHandlers { -> F}

typedef void (M4XMLSAXHandlers::* ElementHandler)(const XMLCh* const name, AttributeList* attributes);

//## begin ElementHandler%37E9243F03E1.postscript preserve=yes
//## end ElementHandler%37E9243F03E1.postscript

//## begin ElementHandlerMap%37E925FE0195.preface preserve=yes
//## end ElementHandlerMap%37E925FE0195.preface

//## Class: ElementHandlerMap%37E925FE0195; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3826D76F02C8;ElementHandler { -> }

typedef map< string,ElementHandler,less<string> > ElementHandlerMap;

//## begin ElementHandlerMap%37E925FE0195.postscript preserve=yes
//## end ElementHandlerMap%37E925FE0195.postscript

//## begin ElementHandlerIt%37E9264E03AC.preface preserve=yes
//## end ElementHandlerIt%37E9264E03AC.preface

//## Class: ElementHandlerIt%37E9264E03AC; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E9265F020C;ElementHandlerMap { -> }

typedef ElementHandlerMap::iterator ElementHandlerIt;

//## begin ElementHandlerIt%37E9264E03AC.postscript preserve=yes
//## end ElementHandlerIt%37E9264E03AC.postscript

//## begin ArgVector%396A1FE90227.preface preserve=yes
//## end ArgVector%396A1FE90227.preface

//## Class: ArgVector%396A1FE90227; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39EC652A028C;ClM4XMLVariant { -> }

typedef vector< ClM4XMLVariant > ArgVector;

//## begin ArgVector%396A1FE90227.postscript preserve=yes
//## end ArgVector%396A1FE90227.postscript

//## begin ClM4XMLMethodStatus%37EB34A9023C.preface preserve=yes
//## end ClM4XMLMethodStatus%37EB34A9023C.preface

//## Class: ClM4XMLMethodStatus%37EB34A9023C
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3826D6AF0308;FilterArgVector { -> }
//## Uses: <unnamed>%396A21460085;ArgVector { -> }

class ClM4XMLMethodStatus 
{
  //## begin ClM4XMLMethodStatus%37EB34A9023C.initialDeclarations preserve=yes
  //## end ClM4XMLMethodStatus%37EB34A9023C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLMethodStatus%938162612
      ClM4XMLMethodStatus ();

    //## Destructor (specified)
      //## Operation: ~ClM4XMLMethodStatus%938162613
      ~ClM4XMLMethodStatus ();


    //## Other Operations (specified)
      //## Operation: Reset%938162614
      m4return_t Reset (ClM4XMLContext *ai_poContext, const string &ai_sItem, m4bool_t ai_bUpdate = M4_TRUE, m4bool_t ai_bRealItem = M4_TRUE);

      //## Operation: Reset%938162617
      void Reset ();

      //## Operation: AddArg%938162618
      m4return_t AddArg (const string &ai_sName, const string *ai_psValue, const string &ai_sType);

      //## Operation: GetNArgs%938162621
      m4int16_t GetNArgs () const;

      //## Operation: Execute%938162623
      m4return_t Execute ();

      //## Operation: GetResult%938162624
      m4return_t GetResult () const;

      //## Operation: SetFilter%939030967
      m4return_t SetFilter (ClM4XMLContext *ai_poContext, const string &ai_sType, const string &ai_sItem, const string &ai_sMode, const string &ai_sValue);

      //## Operation: AddFilterArg%939030968
      m4return_t AddFilterArg (const string &ai_sMode, const string &ai_sName, const string &ai_sValue);

      //## Operation: ExecuteFilter%939030969
      m4return_t ExecuteFilter (ClM4XMLContext *ai_poContext);

      //## Operation: GetResultType%941188387
      m4int16_t GetResultType () const;

      //## Operation: GetResultIsNull%941188388
	  const m4bool_t GetResultIsNull() const;

      //## Operation: GetResultValue%941188388
      const string & GetResultValue () const;

      //## Operation: SetRegisterUniqueId%941188388
	  m4void_t SetRegisterUniqueId (m4uint32_t ai_iUniqueId);

      //## Operation: CheckSecurity%941188388
	  m4return_t CheckSecurity();

	  m4return_t GetRoleOrg( string& ao_strRole, string& ao_strOrg );

	  m4void_t SetEncryptResult(m4bool_t ai_bEncryptResult);

	  m4bool_t GetEncryptResult();
	  
	  // Additional Public Declarations
      //## begin ClM4XMLMethodStatus%37EB34A9023C.public preserve=yes
      //## end ClM4XMLMethodStatus%37EB34A9023C.public

  protected:

    //## Other Operations (specified)
      //## Operation: Init%938162625
      void Init ();

    // Additional Protected Declarations
      //## begin ClM4XMLMethodStatus%37EB34A9023C.protected preserve=yes
      //## end ClM4XMLMethodStatus%37EB34A9023C.protected

	  m4return_t	_RegisterUserAuditXMLMethodEvent() ;

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oItem%37EB37230356
      //## begin ClM4XMLMethodStatus::m_oItem%37EB37230356.attr preserve=no  private: ClM4XMLItem {UA} 
      ClM4XMLItem m_oItem;
      //## end ClM4XMLMethodStatus::m_oItem%37EB37230356.attr

      //## Attribute: m_poContext%37EB375902B3
      //## begin ClM4XMLMethodStatus::m_poContext%37EB375902B3.attr preserve=no  private: ClM4XMLContext * {UA}  
      ClM4XMLContext *m_poContext;
      //## end ClM4XMLMethodStatus::m_poContext%37EB375902B3.attr

      //## Attribute: m_iUsedArgs%37EB37700342
      //## begin ClM4XMLMethodStatus::m_iUsedArgs%37EB37700342.attr preserve=no  private: m4int16_t {UA}  
      m4int16_t m_iUsedArgs;
      //## end ClM4XMLMethodStatus::m_iUsedArgs%37EB37700342.attr

      //## Attribute: m_iNArgs%37EB378D013B
      //## begin ClM4XMLMethodStatus::m_iNArgs%37EB378D013B.attr preserve=no  private: m4int16_t {UA}  
      m4int16_t m_iNArgs;
      //## end ClM4XMLMethodStatus::m_iNArgs%37EB378D013B.attr

      //## Attribute: m_iResult%37EB85D402C7
      //## begin ClM4XMLMethodStatus::m_iResult%37EB85D402C7.attr preserve=no  private: m4return_t {UA}  
      m4return_t m_iResult;
      //## end ClM4XMLMethodStatus::m_iResult%37EB85D402C7.attr

      //## Attribute: m_oFilterArgVector%37F9CC1F0041
      //## begin ClM4XMLMethodStatus::m_oFilterArgVector%37F9CC1F0041.attr preserve=no  private: FilterArgVector {UA} 
      FilterArgVector m_oFilterArgVector;
      //## end ClM4XMLMethodStatus::m_oFilterArgVector%37F9CC1F0041.attr

      //## Attribute: m_oArgVector%396A215300F1
      //## begin ClM4XMLMethodStatus::m_oArgVector%396A215300F1.attr preserve=no  private: ArgVector {UA} 
      ArgVector m_oArgVector;
      //## end ClM4XMLMethodStatus::m_oArgVector%396A215300F1.attr

      //## Attribute: m_bIsVariableArgs%396C326A0083
      //## begin ClM4XMLMethodStatus::m_bIsVariableArgs%396C326A0083.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bIsVariableArgs;
      //## end ClM4XMLMethodStatus::m_bIsVariableArgs%396C326A0083.attr

      //## Attribute: m_iNFixedArgs%396C327F025A
      //## begin ClM4XMLMethodStatus::m_iNFixedArgs%396C327F025A.attr preserve=no  private: m4int16_t {UA} 
      m4int16_t m_iNFixedArgs;
      //## end ClM4XMLMethodStatus::m_iNFixedArgs%396C327F025A.attr

      //## Attribute: m_iNVariableArgs%396C32A00121
      //## begin ClM4XMLMethodStatus::m_iNVariableArgs%396C32A00121.attr preserve=no  private: m4int16_t {UA} 
      m4int16_t m_iNVariableArgs;
      //## end ClM4XMLMethodStatus::m_iNVariableArgs%396C32A00121.attr

      //## Attribute: m_iRegisterUniqueId%396C32A00121
      //## begin ClM4XMLMethodStatus::m_iRegisterUniqueId%396C32A00121.attr preserve=no  private: m4int16_t {UA} 
      m4uint32_t m_iRegisterUniqueId;
      //## end ClM4XMLMethodStatus::m_iRegisterUniqueId%396C32A00121.attr

      //## Attribute: m_bEncrypted%396C32A00121
      //## begin ClM4XMLMethodStatus::m_bEncrypted%396C32A00121.attr preserve=no  private: m4int16_t {UA} 
      m4bool_t m_bEncrypted;
      //## end ClM4XMLMethodStatus::m_bEncrypted%396C32A00121.attr

      //## Attribute: m_sEncryptionFuncGroups%396C32A00121
      //## begin ClM4XMLMethodStatus::m_sEncryptionFuncGroups%396C32A00121.attr preserve=no  private: m4int16_t {UA} 
      string m_sEncryptionFuncGroups;
      //## end ClM4XMLMethodStatus::m_sEncryptionFuncGroups%396C32A00121.attr

	  // Additional Private Declarations
      //## begin ClM4XMLMethodStatus%37EB34A9023C.private preserve=yes
      //## end ClM4XMLMethodStatus%37EB34A9023C.private

	  // Encrypts method result
	  m4bool_t m_bEncryptResult;

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLMethodStatus%37EB34A9023C.implementation preserve=yes
      //## end ClM4XMLMethodStatus%37EB34A9023C.implementation

};

//## begin ClM4XMLMethodStatus%37EB34A9023C.postscript preserve=yes
//## end ClM4XMLMethodStatus%37EB34A9023C.postscript

//## begin ArgElementMap%40EE4BBF02E8.preface preserve=yes
//## end ArgElementMap%40EE4BBF02E8.preface

//## Class: ArgElementMap%40EE4BBF02E8; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,string,less<string> > ArgElementMap;

//## begin ArgElementMap%40EE4BBF02E8.postscript preserve=yes
//## end ArgElementMap%40EE4BBF02E8.postscript

//## begin M4XMLSAXHandlers%37E8E47C0295.preface preserve=yes
//## end M4XMLSAXHandlers%37E8E47C0295.preface

//## Class: M4XMLSAXHandlers%37E8E47C0295
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E901520376;ClM4XMLContext { -> }
//## Uses: <unnamed>%37EB398A010B;ClM4XMLMethodStatus { -> }
//## Uses: <unnamed>%37EF6CB901D6;ClM4XMLItemFilterList { -> }
//## Uses: <unnamed>%37F49E0602D0;ClM4XMLStream { -> }
//## Uses: <unnamed>%3804D2700040;ClFileUnit { -> F}
//## Uses: <unnamed>%3826D7E002F3;ElementHandlerMap { -> }

class M4_DECL_M4EXEFORCHANNEL M4XMLSAXHandlers : public HandlerBase  //## Inherits: <unnamed>%37E8E98B00F2
{
  //## begin M4XMLSAXHandlers%37E8E47C0295.initialDeclarations preserve=yes
  //## end M4XMLSAXHandlers%37E8E47C0295.initialDeclarations

  public:
	
	  typedef enum {PARSING_PHASE_PRE, PARSING_PHASE_1, PARSING_PHASE_2} ParsingPhase;

    //## Constructors (specified)
      //## Operation: M4XMLSAXHandlers%938011536
      M4XMLSAXHandlers (ClCMInitParameters &aio_oClCMInitParameters, ClM4XMLStream &ai_oStream, ClM4XMLInfo* ai_poExecInfo = NULL);

    //## Destructor (specified)
      //## Operation: ~M4XMLSAXHandlers%938011537
      ~M4XMLSAXHandlers ();


    //## Other Operations (specified)
      //## Operation: IsOk%938011538
      m4bool_t IsOk () const;

      //## Operation: GetError%941109329
      m4return_t GetError () const;

      //## Operation: DumpAttrib%938073888
      void DumpAttrib (AttributeList *attributes);

      //## Operation: checkUserConsistency%938073889
	  string GetUserId();

      //## Operation: endDocument%938011540
      void endDocument ();

      //## Operation: endElement%938011541
      void endElement (const XMLCh * const name);

      //## Operation: SetError%938073890
      void SetError (m4return_t ai_iError = M4_ERROR);

      //## Operation: startDocument%938011542
      void startDocument ();	

      //## Operation: startElement%938011543
      void startElement (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procContext%938011545
      void procContext (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procContextPreProcess%938011545
      void procContextPreProcess (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procM4OAlias%938073889
      void procM4OAlias (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procCommandSet%938708970
      void procCommandSet (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procMethodPreProcess%938096977
      void procMethodPreProcess (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procMethod%938096977
      void procMethod (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procArg%938096978
      void procArg (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procData%938708973
      void procData (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procDataDef%938437884
      void procDataDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procNode%938708974
      void procNode (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procNodeDef%938437886
      void procNodeDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procRegister%938708975
      void procRegister (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procRegisterDef%938437887
      void procRegisterDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procItem%938708976
      void procItem (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procItemDef%938437888
      void procItemDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procFilter%939030964
      void procFilter (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procFilterArg%939030965
      void procFilterArg (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procTranslType%939289638
      void procTranslType (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procTranslItem%939289641
      void procTranslItem (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procTranslItemArg%939372141
      void procTranslItemArg (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procMove%940850223
      void procMove (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procMoveDef%940850224
      void procMoveDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procLog%941014279
      void procLog (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procM4OParam%944575097
      void procM4OParam (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procGenCommands%1089357392
      void procGenCommands (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procCommand%1089357393
      void procCommand (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procCommArg%1089357394
      void procCommArg (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procM4ORef%944575097
      void procM4ORef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndSimple%938096976
      void procEndSimple (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndCommandSet%938708971
      void procEndCommandSet (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndMethod%938162622
      void procEndMethod (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndDataDef%938437885
      void procEndDataDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndFilter%939030966
      void procEndFilter (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndTranslItem%939372142
      void procEndTranslItem (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndContextPreProcess%940341738
      void procEndContextPreProcess (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndContext%940341738
      void procEndContext (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndNodeDef%940864584
      void procEndNodeDef (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndM4OAlias%944575098
      void procEndM4OAlias (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndGenCommands%1089357395
      void procEndGenCommands (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndCommand%1089357396
      void procEndCommand (const XMLCh* const name, AttributeList *attributes);

      //## Operation: DumpLog%940504734
      m4return_t DumpLog (const string& ai_sId, const string& ai_sDateMode, const string& ai_sFullMode);

      //## Operation: LoadState%971105393
      m4return_t LoadState ();

      //## Operation: PersistState%971105394
      m4return_t PersistState ();

      //## Operation: SetPreParseMode%971105394
	  m4void_t SetParsingPhase(ParsingPhase ai_ePhase);

	  //## Operation: SetPreParseMode%971105394
	  m4string_t GetRepositoryKey();

	  // Additional Public Declarations
      //## begin M4XMLSAXHandlers%37E8E47C0295.public preserve=yes
#if _M4_DEBUG > 10
	  // For parser debugging
	  m4bool_t bEndContextStart;	// Before Process Context
	  m4bool_t bEndContextEnd;		// After  Process Context
#endif
      //## end M4XMLSAXHandlers%37E8E47C0295.public
  protected:

    //## Other Operations (specified)
      //## Operation: Reset%942060102
      m4return_t Reset ();

      //## Operation: InitHandlers%942060102
	  m4void_t InitHandlers();

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%38551EB002A8
      //## Role: M4XMLSAXHandlers::m_poExecInfo%38551EB10083
      //## begin M4XMLSAXHandlers::m_poExecInfo%38551EB10083.role preserve=no  private: ClM4XMLInfo {1 -> 1RHN}
      ClM4XMLInfo *m_poExecInfo;
      //## end M4XMLSAXHandlers::m_poExecInfo%38551EB10083.role

    // Additional Protected Declarations
      //## begin M4XMLSAXHandlers%37E8E47C0295.protected preserve=yes
      //## end M4XMLSAXHandlers%37E8E47C0295.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsOk%37E8F1B30113
      //## begin M4XMLSAXHandlers::m_bIsOk%37E8F1B30113.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsOk;
      //## end M4XMLSAXHandlers::m_bIsOk%37E8F1B30113.attr

      //## Attribute: m_oOut%37E8F8EC008F
      //## begin M4XMLSAXHandlers::m_oOut%37E8F8EC008F.attr preserve=no  private: ClM4XMLStream & {UA} ai_oStream
      ClM4XMLStream &m_oOut;
      //## end M4XMLSAXHandlers::m_oOut%37E8F8EC008F.attr

      //## Attribute: m_poContext%37E9031A03A4
      //## begin M4XMLSAXHandlers::m_poContext%37E9031A03A4.attr preserve=no  private: ClM4XMLContext * {UA} NULL
      ClM4XMLContext *m_poContext;
      //## end M4XMLSAXHandlers::m_poContext%37E9031A03A4.attr

      //## Attribute: m_oElementStart%37E926C8020D
      //## begin M4XMLSAXHandlers::m_oElementStart%37E926C8020D.attr preserve=no  private: ElementHandlerMap {UA} 
      ElementHandlerMap m_oElementStart;
      //## end M4XMLSAXHandlers::m_oElementStart%37E926C8020D.attr

      //## Attribute: m_oElementEnd%37EA51DD0159
      //## begin M4XMLSAXHandlers::m_oElementEnd%37EA51DD0159.attr preserve=no  private: ElementHandlerMap {UA} 
      ElementHandlerMap m_oElementEnd;
      //## end M4XMLSAXHandlers::m_oElementEnd%37EA51DD0159.attr

      //## Attribute: m_oMethod%37EB4025033D
      //## begin M4XMLSAXHandlers::m_oMethod%37EB4025033D.attr preserve=no  private: ClM4XMLMethodStatus {UA} 
      ClM4XMLMethodStatus m_oMethod;
      //## end M4XMLSAXHandlers::m_oMethod%37EB4025033D.attr

      //## Attribute: m_iError%37EB40B001DF
      //## begin M4XMLSAXHandlers::m_iError%37EB40B001DF.attr preserve=no  private: m4return_t {UA} M4_SUCCESS
      m4return_t m_iError;
      //## end M4XMLSAXHandlers::m_iError%37EB40B001DF.attr

      //## Attribute: m_oItemList%37EF6CE40231
      //## begin M4XMLSAXHandlers::m_oItemList%37EF6CE40231.attr preserve=no  private: ClM4XMLItemFilterList {UA} 
      ClM4XMLItemFilterList m_oItemList;
      //## end M4XMLSAXHandlers::m_oItemList%37EF6CE40231.attr

      //## Attribute: m_oItem%37F3AB940170
      //## begin M4XMLSAXHandlers::m_oItem%37F3AB940170.attr preserve=no  private: ClM4XMLItem {UA} 
      ClM4XMLItem m_oItem;
      //## end M4XMLSAXHandlers::m_oItem%37F3AB940170.attr

      //## Attribute: m_oCMInitParameters%39E35AC3024F
      //## begin M4XMLSAXHandlers::m_oCMInitParameters%39E35AC3024F.attr preserve=no  private: ClCMInitParameters & {UA} aio_oClCMInitParameters
      ClCMInitParameters &m_oCMInitParameters;
      //## end M4XMLSAXHandlers::m_oCMInitParameters%39E35AC3024F.attr

      //## Attribute: m_sGenCommand%40EE4DC60077
      //## begin M4XMLSAXHandlers::m_sGenCommand%40EE4DC60077.attr preserve=no  private: string {UA} 
      string m_sGenCommand;
      //## end M4XMLSAXHandlers::m_sGenCommand%40EE4DC60077.attr

      //## Attribute: m_bHaveFunctionalGroup%40EE4DC60077
      //## begin M4XMLSAXHandlers::m_bHaveFunctionalGroup%40EE4DC60077.attr preserve=no  private: string {UA} 
	  m4bool_t m_bHaveFunctionalGroup;
      //## end M4XMLSAXHandlers::m_bHaveFunctionalGroup%40EE4DC60077.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%40EE4CDD03A3
      //## Role: M4XMLSAXHandlers::m_oGenCommArgs%40EE4CDE0113
      //## begin M4XMLSAXHandlers::m_oGenCommArgs%40EE4CDE0113.role preserve=no  private: ArgElementMap { -> VHAN}
      ArgElementMap m_oGenCommArgs;
      //## end M4XMLSAXHandlers::m_oGenCommArgs%40EE4CDE0113.role

      //## Attribute: m_bNormalMode%37E8F1B30113
      //## begin M4XMLSAXHandlers::m_eParsingPhase%37E8F1B30113.attr preserve=no  private: enum
	  ParsingPhase m_eParsingPhase;
      //## end M4XMLSAXHandlers::m_bNormalMode%37E8F1B30113.attr

    // Additional Private Declarations
      //## begin M4XMLSAXHandlers%37E8E47C0295.private preserve=yes
      //## end M4XMLSAXHandlers%37E8E47C0295.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4XMLSAXHandlers%37E8E47C0295.implementation preserve=yes
      //## end M4XMLSAXHandlers%37E8E47C0295.implementation

};

//## begin M4XMLSAXHandlers%37E8E47C0295.postscript preserve=yes
//## end M4XMLSAXHandlers%37E8E47C0295.postscript

// Class ClM4XMLErrorHandler 


//## Other Operations (inline)
inline m4bool_t ClM4XMLErrorHandler::IsOk () const
{
  //## begin ClM4XMLErrorHandler::IsOk%940504740.body preserve=yes
	return m_bIsOk;
  //## end ClM4XMLErrorHandler::IsOk%940504740.body
}

// Class M4XMLCh 

// Class FilterArg 

// Class ClM4XMLMethodStatus 


//## Other Operations (inline)
inline m4int16_t ClM4XMLMethodStatus::GetNArgs () const
{
  //## begin ClM4XMLMethodStatus::GetNArgs%938162621.body preserve=yes
	return m_iNArgs;
  //## end ClM4XMLMethodStatus::GetNArgs%938162621.body
}

inline m4return_t ClM4XMLMethodStatus::GetResult () const
{
  //## begin ClM4XMLMethodStatus::GetResult%938162624.body preserve=yes
	return m_iResult;
  //## end ClM4XMLMethodStatus::GetResult%938162624.body
}

inline m4int16_t ClM4XMLMethodStatus::GetResultType () const
{
  //## begin ClM4XMLMethodStatus::GetResultType%941188387.body preserve=yes
	return m_oItem.GetResultType();
  //## end ClM4XMLMethodStatus::GetResultType%941188387.body
}

inline const string & ClM4XMLMethodStatus::GetResultValue () const
{
  //## begin ClM4XMLMethodStatus::GetResultValue%941188388.body preserve=yes
	return m_oItem.GetResult();
  //## end ClM4XMLMethodStatus::GetResultValue%941188388.body
}

inline m4void_t ClM4XMLMethodStatus::SetRegisterUniqueId (m4uint32_t ai_iUniqueId)
{
  //## begin ClM4XMLMethodStatus::GetResultValue%941188388.body preserve=yes
	m_iRegisterUniqueId = ai_iUniqueId;
  //## end ClM4XMLMethodStatus::GetResultValue%941188388.body
}

inline m4void_t ClM4XMLMethodStatus::SetEncryptResult(m4bool_t ai_bEncryptResult)
{
	m_bEncryptResult = ai_bEncryptResult;
}

inline m4bool_t ClM4XMLMethodStatus::GetEncryptResult()
{
	return m_bEncryptResult;
}

inline const m4bool_t ClM4XMLMethodStatus::GetResultIsNull () const
{
  //## begin ClM4XMLMethodStatus::GetResultIsNull%941188388.body preserve=yes
	return m_oItem.GetResultIsNull();
  //## end ClM4XMLMethodStatus::GetResultIsNull%941188388.body
}

// Class M4XMLSAXHandlers 


//## Other Operations (inline)
inline m4bool_t M4XMLSAXHandlers::IsOk () const
{
  //## begin M4XMLSAXHandlers::IsOk%938011538.body preserve=yes
	return m_bIsOk;
  //## end M4XMLSAXHandlers::IsOk%938011538.body
}

inline m4return_t M4XMLSAXHandlers::GetError () const
{
  //## begin M4XMLSAXHandlers::GetError%941109329.body preserve=yes
	return m_iError;
  //## end M4XMLSAXHandlers::GetError%941109329.body
}

inline void M4XMLSAXHandlers::SetError (m4return_t ai_iError)
{
  //## begin M4XMLSAXHandlers::SetError%938073890.body preserve=yes
	m_bIsOk = M4_FALSE;
	m_iError = ai_iError;
  //## end M4XMLSAXHandlers::SetError%938073890.body
}

//## Operation: SetParsingPhase%971105394
inline m4void_t M4XMLSAXHandlers::SetParsingPhase(ParsingPhase ai_ePhase)
{
	m_eParsingPhase = ai_ePhase;
	InitHandlers();
}

//## begin module%37E8E4F4023D.epilog preserve=yes
//## end module%37E8E4F4023D.epilog


#endif
