//## begin module%37DF53EC0192.cm preserve=no
//## end module%37DF53EC0192.cm

//## begin module%37DF53EC0192.cp preserve=no
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
//## end module%37DF53EC0192.cp

//## Module: Executive_M4XML%37DF53EC0192; Package specification
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Source file: D:\Work\v600\m4exeforchannel\inc\serverm4xml.hpp

#ifndef executive_m4xml_h
#define executive_m4xml_h 1

//## begin module%37DF53EC0192.additionalIncludes preserve=no
//## end module%37DF53EC0192.additionalIncludes

//## begin module%37DF53EC0192.includes preserve=yes
#include <xercesc/util/XercesDefs.hpp>
XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class DOM_Document;
class DOM_Node;
class DOM_Element;
XERCES_CPP_NAMESPACE_END

class ClNode;
class ClItem;
class ClExecutorStateManager;

#include "m4exeforchannel_dll.hpp"
//## end module%37DF53EC0192.includes

// DataStorages
#include <datastorages.hpp>
#include "m4stl.hpp"
// Util_M4XML
#include <m4xmlutil.hpp>

class ClM4ObjService;
class ClT3AccessProvider;
class ClVectorDelta;

//## begin module%37DF53EC0192.declarations preserve=no
//## end module%37DF53EC0192.declarations

//## begin module%37DF53EC0192.additionalDeclarations preserve=yes
#include <m4types.hpp>
#include <_base64.h>
#include <access.hpp>
#include <m4mdrt.hpp>

#define XMLICMP(X,Y,Z) M4XMLString::ICaseComp(X,Y,Z)

#define M4CL_M4XML_CONTEXT_UUID_KEY_SIZE				25 //24+1 for null terminator

//## end module%37DF53EC0192.additionalDeclarations


//## begin ClM4XMLCurrent%37DFE587026D.preface preserve=yes
//## end ClM4XMLCurrent%37DFE587026D.preface

//## Class: ClM4XMLCurrent%37DFE587026D
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4XMLCurrent 
{
  //## begin ClM4XMLCurrent%37DFE587026D.initialDeclarations preserve=yes
  //## end ClM4XMLCurrent%37DFE587026D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLCurrent%937383069
      ClM4XMLCurrent ();

    //## Destructor (specified)
      //## Operation: ~ClM4XMLCurrent%939117435
      ~ClM4XMLCurrent ();


    //## Other Operations (specified)
      //## Operation: Init%937383070
      void Init ();

      //## Operation: Reset%939117436
      m4return_t Reset ();

      //## Operation: Serialize%940247659
      m4return_t Serialize (ClGenericIODriver& ao_oIODriver);

      //## Operation: Deserialize%940247660
      m4return_t Deserialize (ClGenericIODriver& ai_oIODriver);

    // Data Members for Class Attributes

      //## Attribute: poAccess%37DFE5DC00D5
      //## begin ClM4XMLCurrent::poAccess%37DFE5DC00D5.attr preserve=no  public: ClAccess * {UA} NULL
      ClAccess *poAccess;
      //## end ClM4XMLCurrent::poAccess%37DFE5DC00D5.attr

      //## Attribute: poNode%37DFE5E800FA
      //## begin ClM4XMLCurrent::poNode%37DFE5E800FA.attr preserve=no  public: ClNode * {UA} NULL
      ClNode *poNode;
      //## end ClM4XMLCurrent::poNode%37DFE5E800FA.attr

      //## Attribute: poAccessGet%37FA0B3F02FB
      //## begin ClM4XMLCurrent::poAccessGet%37FA0B3F02FB.attr preserve=no  public: ClAccess * {UA} NULL
      ClAccess *poAccessGet;
      //## end ClM4XMLCurrent::poAccessGet%37FA0B3F02FB.attr

      //## Attribute: poNodeGet%37FA0B40032E
      //## begin ClM4XMLCurrent::poNodeGet%37FA0B40032E.attr preserve=no  public: ClNode * {UA} NULL
      ClNode *poNodeGet;
      //## end ClM4XMLCurrent::poNodeGet%37FA0B40032E.attr

      //## Attribute: bPreserve%38231559008A
      //## begin ClM4XMLCurrent::bPreserve%38231559008A.attr preserve=no  public: m4bool_t {UA} M4_TRUE
      m4bool_t bPreserve;
      //## end ClM4XMLCurrent::bPreserve%38231559008A.attr

      //## Attribute: bOwner%38354C97007F
      //## begin ClM4XMLCurrent::bOwner%38354C97007F.attr preserve=no  public: m4bool_t {UA} M4_TRUE
      m4bool_t bOwner;
      //## end ClM4XMLCurrent::bOwner%38354C97007F.attr

    // Additional Public Declarations
      //## begin ClM4XMLCurrent%37DFE587026D.public preserve=yes
      //## end ClM4XMLCurrent%37DFE587026D.public

  protected:

    //## Other Operations (specified)
      //## Operation: SerializeLL%943275970
      static m4return_t SerializeLL (ClGenericIODriver& ao_oIODriver, ClAccess* &ao_poAccess, ClNode* &ao_poNode, m4bool_t ai_bOwner);

      //## Operation: DeserializeLL%943275971
      static m4return_t DeserializeLL (ClGenericIODriver& ai_oIODriver, ClAccess* &ai_poAccess, ClNode* &ai_poNode, m4bool_t ai_bOwner);

    // Additional Protected Declarations
      //## begin ClM4XMLCurrent%37DFE587026D.protected preserve=yes
      //## end ClM4XMLCurrent%37DFE587026D.protected

  private:
    // Additional Private Declarations
      //## begin ClM4XMLCurrent%37DFE587026D.private preserve=yes
      //## end ClM4XMLCurrent%37DFE587026D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLCurrent%37DFE587026D.implementation preserve=yes
      //## end ClM4XMLCurrent%37DFE587026D.implementation

};

//## begin ClM4XMLCurrent%37DFE587026D.postscript preserve=yes
//## end ClM4XMLCurrent%37DFE587026D.postscript

//## begin TranslArgsMap%37FCBC1F0065.preface preserve=yes
//## end TranslArgsMap%37FCBC1F0065.preface

//## Class: TranslArgsMap%37FCBC1F0065; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< m4int16_t,string,less<m4int16_t> > TranslArgsMap;

//## begin TranslArgsMap%37FCBC1F0065.postscript preserve=yes
//## end TranslArgsMap%37FCBC1F0065.postscript

//## begin TranslArgsIt%37FCBC870367.preface preserve=yes
//## end TranslArgsIt%37FCBC870367.preface

//## Class: TranslArgsIt%37FCBC870367
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FCBC9901B5;TranslArgsMap { -> }

typedef TranslArgsMap::iterator TranslArgsIt;

//## begin TranslArgsIt%37FCBC870367.postscript preserve=yes
//## end TranslArgsIt%37FCBC870367.postscript

//## begin M4OAliasMap%37DF8DAA02D8.preface preserve=yes
//## end M4OAliasMap%37DF8DAA02D8.preface

//## Class: M4OAliasMap%37DF8DAA02D8; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,ClM4XMLCurrent *,less<string> > M4OAliasMap;

//## begin M4OAliasMap%37DF8DAA02D8.postscript preserve=yes
//## end M4OAliasMap%37DF8DAA02D8.postscript

//## begin M4OAliasMapIt%37DF8E8203B5.preface preserve=yes
//## end M4OAliasMapIt%37DF8E8203B5.preface

//## Class: M4OAliasMapIt%37DF8E8203B5
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37DF8E9B018A;M4OAliasMap { -> }

typedef M4OAliasMap::iterator M4OAliasMapIt;

//## begin M4OAliasMapIt%37DF8E8203B5.postscript preserve=yes
//## end M4OAliasMapIt%37DF8E8203B5.postscript

//## begin TranslArgs%37FCBB7D021A.preface preserve=yes
//## end TranslArgs%37FCBB7D021A.preface

//## Class: TranslArgs%37FCBB7D021A
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FCBD3C02F9;TranslArgsIt { -> }

class TranslArgs 
{
  //## begin TranslArgs%37FCBB7D021A.initialDeclarations preserve=yes
  //## end TranslArgs%37FCBB7D021A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TranslArgs%939289651
      TranslArgs (ClAccess *ai_poAccess, const string& ai_sNode, const string& ai_sItem);


    //## Other Operations (specified)
      //## Operation: Add%939289644
      void Add (const string& param, const string& value);

      //## Operation: Size%939289650
      m4int16_t Size () const;

      //## Operation: GetArg%939289648
      m4pcchar_t GetArg (m4int16_t ai_iIndex);

      //## Operation: GetNode%939289653
      const string & GetNode () const;

      //## Operation: GetItem%939289654
      const string & GetItem () const;

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%37FCBD5A00E0
      //## Role: TranslArgs::m_oArgsMap%37FCBD5A0339
      //## begin TranslArgs::m_oArgsMap%37FCBD5A0339.role preserve=no  public: TranslArgsMap {1 -> 1VHAN}
      TranslArgsMap m_oArgsMap;
      //## end TranslArgs::m_oArgsMap%37FCBD5A0339.role

    // Additional Public Declarations
      //## begin TranslArgs%37FCBB7D021A.public preserve=yes
      //## end TranslArgs%37FCBB7D021A.public

  protected:

    //## Other Operations (specified)
      //## Operation: Reset%939289652
      void Reset ();

    // Additional Protected Declarations
      //## begin TranslArgs%37FCBB7D021A.protected preserve=yes
      //## end TranslArgs%37FCBB7D021A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poAccess%37FCD04F02F1
      //## begin TranslArgs::m_poAccess%37FCD04F02F1.attr preserve=no  private: ClAccess * {UA} ai_poAccess
      ClAccess *m_poAccess;
      //## end TranslArgs::m_poAccess%37FCD04F02F1.attr

      //## Attribute: m_iHandle%37FCD05A0120
      //## begin TranslArgs::m_iHandle%37FCD05A0120.attr preserve=no  private: m4int32_t {UA} -1
      m4int32_t m_iHandle;
      //## end TranslArgs::m_iHandle%37FCD05A0120.attr

      //## Attribute: m_sNode%37FCDDC001D6
      //## begin TranslArgs::m_sNode%37FCDDC001D6.attr preserve=no  private: string {UA} ai_sNode
      string m_sNode;
      //## end TranslArgs::m_sNode%37FCDDC001D6.attr

      //## Attribute: m_sItem%37FCDDD303A0
      //## begin TranslArgs::m_sItem%37FCDDD303A0.attr preserve=no  private: string {UA} ai_sItem
      string m_sItem;
      //## end TranslArgs::m_sItem%37FCDDD303A0.attr

    // Additional Private Declarations
      //## begin TranslArgs%37FCBB7D021A.private preserve=yes
      //## end TranslArgs%37FCBB7D021A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TranslArgs%37FCBB7D021A.implementation preserve=yes
      //## end TranslArgs%37FCBB7D021A.implementation

};

//## begin TranslArgs%37FCBB7D021A.postscript preserve=yes
//## end TranslArgs%37FCBB7D021A.postscript

//## begin TranslItemPK%37FCF8A80127.preface preserve=yes
//## end TranslItemPK%37FCF8A80127.preface

//## Class: TranslItemPK%37FCF8A80127
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TranslItemPK 
{
  //## begin TranslItemPK%37FCF8A80127.initialDeclarations preserve=yes
  //## end TranslItemPK%37FCF8A80127.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TranslItemPK%945198098
      TranslItemPK ();

      //## Operation: TranslItemPK%939289655
      TranslItemPK (const string& ai_sM4OAlias, m4int32_t ai_iHandle);


    //## Other Operations (specified)
      //## Operation: operator <%939289656
      m4bool_t operator < (const TranslItemPK& o) const;

    // Additional Public Declarations
      //## begin TranslItemPK%37FCF8A80127.public preserve=yes
      //## end TranslItemPK%37FCF8A80127.public

  protected:
    // Additional Protected Declarations
      //## begin TranslItemPK%37FCF8A80127.protected preserve=yes
      //## end TranslItemPK%37FCF8A80127.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sM4OAlias%37FCF91F0286
      //## begin TranslItemPK::m_sM4OAlias%37FCF91F0286.attr preserve=no  private: string {UA} 
      string m_sM4OAlias;
      //## end TranslItemPK::m_sM4OAlias%37FCF91F0286.attr

      //## Attribute: m_iHandle%37FCF93203CE
      //## begin TranslItemPK::m_iHandle%37FCF93203CE.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iHandle;
      //## end TranslItemPK::m_iHandle%37FCF93203CE.attr

    // Additional Private Declarations
      //## begin TranslItemPK%37FCF8A80127.private preserve=yes
      //## end TranslItemPK%37FCF8A80127.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TranslItemPK%37FCF8A80127.implementation preserve=yes
      //## end TranslItemPK%37FCF8A80127.implementation

};

//## begin TranslItemPK%37FCF8A80127.postscript preserve=yes
//## end TranslItemPK%37FCF8A80127.postscript

//## begin TranslItemDataStr%37FE14900149.preface preserve=yes
//## end TranslItemDataStr%37FE14900149.preface

//## Class: TranslItemDataStr%37FE14900149
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TranslItemDataStr 
{
  //## begin TranslItemDataStr%37FE14900149.initialDeclarations preserve=yes
  //## end TranslItemDataStr%37FE14900149.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TranslItemDataStr%945198097
      TranslItemDataStr ();

      //## Operation: TranslItemDataStr%939630172
      TranslItemDataStr (const string &ai_sName, const string &ai_sValue);


    //## Other Operations (specified)
      //## Operation: operator <%939630173
      m4bool_t operator < (const TranslItemDataStr& o) const;

    // Data Members for Class Attributes

      //## Attribute: m_sName%37FE14D301C8
      //## begin TranslItemDataStr::m_sName%37FE14D301C8.attr preserve=no  public: string {UA} 
      string m_sName;
      //## end TranslItemDataStr::m_sName%37FE14D301C8.attr

      //## Attribute: m_sValue%37FE14E30058
      //## begin TranslItemDataStr::m_sValue%37FE14E30058.attr preserve=no  public: string {UA} 
      string m_sValue;
      //## end TranslItemDataStr::m_sValue%37FE14E30058.attr

    // Additional Public Declarations
      //## begin TranslItemDataStr%37FE14900149.public preserve=yes
      //## end TranslItemDataStr%37FE14900149.public

  protected:
    // Additional Protected Declarations
      //## begin TranslItemDataStr%37FE14900149.protected preserve=yes
      //## end TranslItemDataStr%37FE14900149.protected

  private:
    // Additional Private Declarations
      //## begin TranslItemDataStr%37FE14900149.private preserve=yes
      //## end TranslItemDataStr%37FE14900149.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TranslItemDataStr%37FE14900149.implementation preserve=yes
      //## end TranslItemDataStr%37FE14900149.implementation

};

//## begin TranslItemDataStr%37FE14900149.postscript preserve=yes
//## end TranslItemDataStr%37FE14900149.postscript

//## begin TranslItemDataSet%37FDC3D103DB.preface preserve=yes
//## end TranslItemDataSet%37FDC3D103DB.preface

//## Class: TranslItemDataSet%37FDC3D103DB; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FE150B0128;TranslItemDataStr { -> }

typedef set< TranslItemDataStr,less<TranslItemDataStr> > TranslItemDataSet;

//## begin TranslItemDataSet%37FDC3D103DB.postscript preserve=yes
//## end TranslItemDataSet%37FDC3D103DB.postscript

//## begin TranslItemDataIt%37FDC3E90317.preface preserve=yes
//## end TranslItemDataIt%37FDC3E90317.preface

//## Class: TranslItemDataIt%37FDC3E90317
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FDC41D009B;TranslItemDataSet { -> }

typedef TranslItemDataSet::iterator TranslItemDataIt;

//## begin TranslItemDataIt%37FDC3E90317.postscript preserve=yes
//## end TranslItemDataIt%37FDC3E90317.postscript

//## begin TranslItemData%37FDC2710226.preface preserve=yes
//## end TranslItemData%37FDC2710226.preface

//## Class: TranslItemData%37FDC2710226
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TranslItemData 
{
  //## begin TranslItemData%37FDC2710226.initialDeclarations preserve=yes
  //## end TranslItemData%37FDC2710226.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TranslItemData%945198099
      TranslItemData ();

      //## Operation: TranslItemData%939372145
      TranslItemData (const string &ai_sType);


    //## Other Operations (specified)
      //## Operation: AddArg%939372146
      void AddArg (const string &ai_sName, const string &ai_sValue);

    // Data Members for Class Attributes

      //## Attribute: m_sType%37FDC47203AA
      //## begin TranslItemData::m_sType%37FDC47203AA.attr preserve=no  public: string {UA} 
      string m_sType;
      //## end TranslItemData::m_sType%37FDC47203AA.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%37FDC42A01B2
      //## Role: TranslItemData::m_oArgSet%37FDC42A032E
      //## begin TranslItemData::m_oArgSet%37FDC42A032E.role preserve=no  public: TranslItemDataSet {1 -> 1VHAN}
      TranslItemDataSet m_oArgSet;
      //## end TranslItemData::m_oArgSet%37FDC42A032E.role

    // Additional Public Declarations
      //## begin TranslItemData%37FDC2710226.public preserve=yes
      //## end TranslItemData%37FDC2710226.public

  protected:
    // Additional Protected Declarations
      //## begin TranslItemData%37FDC2710226.protected preserve=yes
      //## end TranslItemData%37FDC2710226.protected

  private:
    // Additional Private Declarations
      //## begin TranslItemData%37FDC2710226.private preserve=yes
      //## end TranslItemData%37FDC2710226.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TranslItemData%37FDC2710226.implementation preserve=yes
      //## end TranslItemData%37FDC2710226.implementation

};

//## begin TranslItemData%37FDC2710226.postscript preserve=yes
//## end TranslItemData%37FDC2710226.postscript

//## begin TranslItemsMap%37FCF9FC0360.preface preserve=yes
//## end TranslItemsMap%37FCF9FC0360.preface

//## Class: TranslItemsMap%37FCF9FC0360; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FDC66400E9;TranslItemData { -> }
//## Uses: <unnamed>%385698810057;TranslItemPK { -> }

typedef map< TranslItemPK,TranslItemData,less<TranslItemPK> > TranslItemsMap;

//## begin TranslItemsMap%37FCF9FC0360.postscript preserve=yes
//## end TranslItemsMap%37FCF9FC0360.postscript

//## begin TranslItemsIt%37FCFA370289.preface preserve=yes
//## end TranslItemsIt%37FCFA370289.preface

//## Class: TranslItemsIt%37FCFA370289
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FCFA580381;TranslItemsMap { -> }

typedef TranslItemsMap::iterator TranslItemsIt;

//## begin TranslItemsIt%37FCFA370289.postscript preserve=yes
//## end TranslItemsIt%37FCFA370289.postscript

//## begin ClM4XMLStream%37F4869C024F.preface preserve=yes
//## end ClM4XMLStream%37F4869C024F.preface

//## Class: ClM4XMLStream%37F4869C024F
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EXEFORCHANNEL ClM4XMLStream 
{
  //## begin ClM4XMLStream%37F4869C024F.initialDeclarations preserve=yes
  //## end ClM4XMLStream%37F4869C024F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLStream%938775329
      ClM4XMLStream ();

    //## Destructor (specified)
      //## Operation: ~ClM4XMLStream%939030963
      ~ClM4XMLStream ();


    //## Other Operations (specified)
      //## Operation: Clear%938775327
      void Clear ();

      //## Operation: operator <<%938775328
      ClM4XMLStream & operator << (m4pcchar_t ai_pcString);

      //## Operation: operator <<%938775331
      ClM4XMLStream & operator << (const XMLCh *ai_pcString);

      //## Operation: operator <<%938775332
      ClM4XMLStream & operator << (ClM4XMLStream& (*f)(ClM4XMLStream&));

      //## Operation: operator <<%938775333
      ClM4XMLStream & operator << (m4int32_t ai_iArg);

      //## Operation: SetBuffer%938775330
      void SetBuffer (M4DataStorage *ai_Data);

      //## Operation: GetpDataStorage%939889211
      M4DataStorage* GetpDataStorage () const;

      //## Operation: Transl%940247663
      ClM4XMLStream & Transl (m4pcchar_t ai_pcString);

	  //## Operation: LocalM4XMLEscapeChar%940247667
      const char* const LocalM4XMLEscapeChar (const char ai_ccChar) ;

      //## Operation: Write%940247664
      m4return_t Write (m4pcchar_t ai_pcString, m4int32_t ai_iSize = -1);

      //## Operation: IsBin%941014275
      m4bool_t IsBin (const m4uint8_t* ai_pcString, m4uint32_t ai_ulLen);

      //## Operation: Transl64%941014277
      m4return_t Transl64 (m4pcchar_t ai_pcString, m4uint32_t ai_iLen);

      //## Operation: Decode64%941014278
      m4return_t Decode64 (m4pcchar_t ai_pcString, string &ao_sOutput);

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%37F4921A00AB
      //## Role: ClM4XMLStream::m_poData%37F4921A02F0
      //## begin ClM4XMLStream::m_poData%37F4921A02F0.role preserve=no  public: M4DataStorage {1 -> 1RHAN}
      M4DataStorage *m_poData;
      //## end ClM4XMLStream::m_poData%37F4921A02F0.role

      //## Association: M4ExeForChannel::<unnamed>%37F49B4D0274
      //## Role: ClM4XMLStream::m_poDataUnit%37F49B4E0171
      //## begin ClM4XMLStream::m_poDataUnit%37F49B4E0171.role preserve=no  public: ClDataUnit {1 -> 1RHAN}
      ClDataUnit *m_poDataUnit;
      //## end ClM4XMLStream::m_poDataUnit%37F49B4E0171.role

    // Additional Public Declarations
      //## begin ClM4XMLStream%37F4869C024F.public preserve=yes
      //## end ClM4XMLStream%37F4869C024F.public

  protected:

    //## Other Operations (specified)
      //## Operation: Close%939030962
      void Close ();

    // Additional Protected Declarations
      //## begin ClM4XMLStream%37F4869C024F.protected preserve=yes
      //## end ClM4XMLStream%37F4869C024F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iLen%37F494230231
      //## begin ClM4XMLStream::m_iLen%37F494230231.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iLen;
      //## end ClM4XMLStream::m_iLen%37F494230231.attr

      //## Attribute: m_iLenCurrent%37F4944500F9
      //## begin ClM4XMLStream::m_iLenCurrent%37F4944500F9.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iLenCurrent;
      //## end ClM4XMLStream::m_iLenCurrent%37F4944500F9.attr

      //## Attribute: DEFAULT_BLOCK%37F49B8F037D
      //## begin ClM4XMLStream::DEFAULT_BLOCK%37F49B8F037D.attr preserve=no  private: m4int16_t {UAC} 4096
      const m4int16_t  DEFAULT_BLOCK;
      //## end ClM4XMLStream::DEFAULT_BLOCK%37F49B8F037D.attr

      //## Attribute: m_oBase64%3816C62702AA
      //## begin ClM4XMLStream::m_oBase64%3816C62702AA.attr preserve=no  private: static C_Base64 {UA} 
      static C_Base64 m_oBase64;
      //## end ClM4XMLStream::m_oBase64%3816C62702AA.attr

    // Additional Private Declarations
      //## begin ClM4XMLStream%37F4869C024F.private preserve=yes
      //## end ClM4XMLStream%37F4869C024F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLStream%37F4869C024F.implementation preserve=yes
      //## end ClM4XMLStream%37F4869C024F.implementation

};

//## begin ClM4XMLStream%37F4869C024F.postscript preserve=yes
inline ClM4XMLStream& endl(ClM4XMLStream& ai_oOut) { return ai_oOut << "\n"; }
//## end ClM4XMLStream%37F4869C024F.postscript

//## begin M4LevelType%37F1F9D7022E.preface preserve=yes
//## end M4LevelType%37F1F9D7022E.preface

//## Class: M4LevelType%37F1F9D7022E
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {LEVEL_ALIAS, LEVEL_NODE, LEVEL_REGISTER, LEVEL_ITEM} M4LevelType;

//## begin M4LevelType%37F1F9D7022E.postscript preserve=yes
//## end M4LevelType%37F1F9D7022E.postscript

//## begin StSetItem%37F219E60045.preface preserve=yes
//## end StSetItem%37F219E60045.preface

//## Class: StSetItem%37F219E60045; implementation
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3826D57601FA;M4LevelType { -> }

class StSetItem 
{
  //## begin StSetItem%37F219E60045.initialDeclarations preserve=yes
  //## end StSetItem%37F219E60045.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: StSetItem%963478679
      StSetItem ();


    //## Other Operations (specified)
      //## Operation: operator <%938598812
      m4bool_t operator < (const StSetItem &obj) const;

    // Data Members for Class Attributes

      //## Attribute: sValue%37F21A1A01F8
      //## begin StSetItem::sValue%37F21A1A01F8.attr preserve=no  public: string {UA} 
      string sValue;
      //## end StSetItem::sValue%37F21A1A01F8.attr

      //## Attribute: iLevel%37F21A29024A
      //## begin StSetItem::iLevel%37F21A29024A.attr preserve=no  public: M4LevelType {UA} LEVEL_NODE
      M4LevelType iLevel;
      //## end StSetItem::iLevel%37F21A29024A.attr

      //## Attribute: iNextCurrent%396C840902F5
      //## begin StSetItem::iNextCurrent%396C840902F5.attr preserve=no  public: m4int32_t {UA} -1
      m4int32_t iNextCurrent;
      //## end StSetItem::iNextCurrent%396C840902F5.attr

      //## Attribute: bRecursive%3FB8B0E10014
      //## begin StSetItem::bRecursive%3FB8B0E10014.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t bRecursive;
      //## end StSetItem::bRecursive%3FB8B0E10014.attr

      //## Attribute: bEnableDeltas%3FB8B0E10014
      //## begin StSetItem::bEnableDeltas%3FB8B0E10014.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t bEnableDeltas;
      //## end StSetItem::bEnableDeltas%3FB8B0E10014.attr

	  // Additional Public Declarations
      //## begin StSetItem%37F219E60045.public preserve=yes
      //## end StSetItem%37F219E60045.public

  protected:
    // Additional Protected Declarations
      //## begin StSetItem%37F219E60045.protected preserve=yes
      //## end StSetItem%37F219E60045.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: iCounter%396D8A5103C5
      //## begin StSetItem::iCounter%396D8A5103C5.attr preserve=no  private: static m4uint32_t {UA} 0
      static m4uint32_t iCounter;
      //## end StSetItem::iCounter%396D8A5103C5.attr

      //## Attribute: iLocalCounter%396D8ADC019E
      //## begin StSetItem::iLocalCounter%396D8ADC019E.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t iLocalCounter;
      //## end StSetItem::iLocalCounter%396D8ADC019E.attr

    // Additional Private Declarations
      //## begin StSetItem%37F219E60045.private preserve=yes
      //## end StSetItem%37F219E60045.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin StSetItem%37F219E60045.implementation preserve=yes
      //## end StSetItem%37F219E60045.implementation

};

//## begin StSetItem%37F219E60045.postscript preserve=yes
//## end StSetItem%37F219E60045.postscript

//## begin M4ItemSet%37E246EE01C9.preface preserve=yes
//## end M4ItemSet%37E246EE01C9.preface

//## Class: M4ItemSet%37E246EE01C9; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F21C07000C;StSetItem { -> }

typedef set< StSetItem,less<StSetItem> > M4ItemSet;

//## begin M4ItemSet%37E246EE01C9.postscript preserve=yes
//## end M4ItemSet%37E246EE01C9.postscript

//## begin M4ItemSetIt%37E2479D00A8.preface preserve=yes
//## end M4ItemSetIt%37E2479D00A8.preface

//## Class: M4ItemSetIt%37E2479D00A8; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E247B901D5;M4ItemSet { -> }

typedef M4ItemSet::iterator M4ItemSetIt;

//## begin M4ItemSetIt%37E2479D00A8.postscript preserve=yes
//## end M4ItemSetIt%37E2479D00A8.postscript

//## begin M4XMLString%38450A1A007A.preface preserve=yes
//## end M4XMLString%38450A1A007A.preface

//## Class: M4XMLString%38450A1A007A
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38450A550093;string { -> }

class M4XMLString 
{
  //## begin M4XMLString%38450A1A007A.initialDeclarations preserve=yes
  //## end M4XMLString%38450A1A007A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ICaseComp%944049067
      static m4bool_t ICaseComp (const string& ai_sToken, m4pcchar_t ai_sString, m4int16_t ai_iMin = -1);

    // Additional Public Declarations
      //## begin M4XMLString%38450A1A007A.public preserve=yes
      //## end M4XMLString%38450A1A007A.public

  protected:
    // Additional Protected Declarations
      //## begin M4XMLString%38450A1A007A.protected preserve=yes
      //## end M4XMLString%38450A1A007A.protected

  private:
    // Additional Private Declarations
      //## begin M4XMLString%38450A1A007A.private preserve=yes
      //## end M4XMLString%38450A1A007A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4XMLString%38450A1A007A.implementation preserve=yes
      //## end M4XMLString%38450A1A007A.implementation

};

//## begin M4XMLString%38450A1A007A.postscript preserve=yes
//## end M4XMLString%38450A1A007A.postscript

//## begin ClM4AliasParam%384D1DD0018C.preface preserve=yes
//## end ClM4AliasParam%384D1DD0018C.preface

//## Class: ClM4AliasParam%384D1DD0018C
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4AliasParam 
{
  //## begin ClM4AliasParam%384D1DD0018C.initialDeclarations preserve=yes
  //## end ClM4AliasParam%384D1DD0018C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4AliasParam%944575104
      ClM4AliasParam (const string& ai_sName, const string& ai_sValue, m4int16_t ai_iType);

      //## Operation: ClM4AliasParam%944739021
      ClM4AliasParam ();


    //## Other Operations (specified)
      //## Operation: operator <%944575105
      m4bool_t operator < (const ClM4AliasParam& v) const;

    // Data Members for Class Attributes

      //## Attribute: sName%384D1DE300CB
      //## begin ClM4AliasParam::sName%384D1DE300CB.attr preserve=no  public: string {UA} 
      string sName;
      //## end ClM4AliasParam::sName%384D1DE300CB.attr

      //## Attribute: sValue%384D1DF5035C
      //## begin ClM4AliasParam::sValue%384D1DF5035C.attr preserve=no  public: string {UA} 
      string sValue;
      //## end ClM4AliasParam::sValue%384D1DF5035C.attr

      //## Attribute: iCPPType%384D1E0502DD
      //## begin ClM4AliasParam::iCPPType%384D1E0502DD.attr preserve=no  public: m4int16_t {UA} -1
      m4int16_t iCPPType;
      //## end ClM4AliasParam::iCPPType%384D1E0502DD.attr

    // Additional Public Declarations
      //## begin ClM4AliasParam%384D1DD0018C.public preserve=yes
      //## end ClM4AliasParam%384D1DD0018C.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4AliasParam%384D1DD0018C.protected preserve=yes
      //## end ClM4AliasParam%384D1DD0018C.protected

  private:
    // Additional Private Declarations
      //## begin ClM4AliasParam%384D1DD0018C.private preserve=yes
      //## end ClM4AliasParam%384D1DD0018C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4AliasParam%384D1DD0018C.implementation preserve=yes
      //## end ClM4AliasParam%384D1DD0018C.implementation

};

//## Class: ClM4AliasRef%384D1DD0018C
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4AliasRef 
{
  //## begin ClM4AliasRef%384D1DD0018C.initialDeclarations preserve=yes
  //## end ClM4AliasRef%384D1DD0018C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4AliasRef%944575104
      ClM4AliasRef (const string& ai_sM4OAliasOrigin, const string& ai_sM4OAlias, const string& ai_sL2Alias);

    //## Other Operations (specified)
      //## Operation: operator <%944575105
      m4bool_t operator < (const ClM4AliasRef& v) const;

    // Data Members for Class Attributes

      //## Attribute: sName%384D1DE300CB
      //## begin ClM4AliasRef::sM4OAlias%384D1DE300CB.attr preserve=no  public: string {UA} 
      string sM4OAlias;
      //## end ClM4AliasRef::sM4OAlias%384D1DE300CB.attr

      //## Attribute: sValue%384D1DF5035C
      //## begin ClM4AliasRef::sL2Alias%384D1DF5035C.attr preserve=no  public: string {UA} 
      string sL2Alias;
      //## end ClM4AliasRef::sL2Alias%384D1DF5035C.attr

	  //## Attribute: sValue%384D1DF5035C
	  //## begin ClM4AliasRef::sL2AliasOrigin%384D1DF5035C.attr preserve=no  public: string {UA} 
	  string sM4OAliasOrigin;
	  //## end ClM4AliasRef::sL2AliasOrigin%384D1DF5035C.attr

	  // Additional Public Declarations
      //## begin ClM4AliasRef%384D1DD0018C.public preserve=yes
      //## end ClM4AliasRef%384D1DD0018C.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4AliasRef%384D1DD0018C.protected preserve=yes
      //## end ClM4AliasRef%384D1DD0018C.protected

  private:
    // Additional Private Declarations
      //## begin ClM4AliasRef%384D1DD0018C.private preserve=yes
      //## end ClM4AliasRef%384D1DD0018C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4AliasRef%384D1DD0018C.implementation preserve=yes
      //## end ClM4AliasRef%384D1DD0018C.implementation

};

//## begin ClM4AliasParam%384D1DD0018C.postscript preserve=yes
//## end ClM4AliasParam%384D1DD0018C.postscript

//## begin ClM4AliasParamSet%384D174E0263.preface preserve=yes
//## end ClM4AliasParamSet%384D174E0263.preface

//## Class: ClM4AliasParamSet%384D174E0263; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%384D1E500000;ClM4AliasParam { -> }

typedef set< ClM4AliasParam,less<ClM4AliasParam> > ClM4AliasParamSet;

//## begin ClM4AliasParamSet%384D174E0263.postscript preserve=yes
//## end ClM4AliasParamSet%384D174E0263.postscript

//## begin ClM4AliasParamIt%384D1C8001E3.preface preserve=yes
//## end ClM4AliasParamIt%384D1C8001E3.preface

//## Class: ClM4AliasParamIt%384D1C8001E3
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%384D1CB202B7;ClM4AliasParamSet { -> }

typedef ClM4AliasParamSet::iterator ClM4AliasParamIt;

//## begin ClM4AliasReferenceSet%384D174E0263.preface preserve=yes
//## end ClM4AliasReferenceSet%384D174E0263.preface

//## Class: ClM4AliasReferenceSet%384D174E0263; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%384D1E500000;ClM4AliasRef { -> }

typedef set< ClM4AliasRef,less<ClM4AliasRef> > ClM4AliasReferenceSet;

//## begin ClM4AliasReferenceSet%384D174E0263.postscript preserve=yes
//## end ClM4AliasReferenceSet%384D174E0263.postscript

//## begin ClM4AliasRefIt%384D1C8001E3.preface preserve=yes
//## end ClM4AliasRefIt%384D1C8001E3.preface

//## Class: ClM4AliasRefIt%384D1C8001E3
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%384D1CB202B7;ClM4AliasReferenceSet { -> }

typedef ClM4AliasReferenceSet::iterator ClM4AliasRefIt;

//## begin ClM4AliasParamIt%384D1C8001E3.postscript preserve=yes
//## end ClM4AliasParamIt%384D1C8001E3.postscript

//## begin ClM4XMLAliasDef%384D154D02DD.preface preserve=yes
//## end ClM4XMLAliasDef%384D154D02DD.preface

//## Class: ClM4XMLAliasDef%384D154D02DD
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%384D1CDD0396;ClM4AliasParamIt { -> }

class ClM4XMLAliasDef 
{
  //## begin ClM4XMLAliasDef%384D154D02DD.initialDeclarations preserve=yes
  //## end ClM4XMLAliasDef%384D154D02DD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLAliasDef%944575103
      ClM4XMLAliasDef ();

    //## Other Operations (specified)
      //## Operation: Reset%944575101
      void Reset ();

	  //## Operation: ResetReferences%944575101
	  void ResetM4OReferences();

      //## Operation: Generate%944575102
      ClChannel* Generate (ClChannelManager* ai_poCM);

      //## Operation: AddParam%944575106
      m4return_t AddParam (ClM4AliasParam ai_oAliasParam);

      //## Operation: AddRef%944575106
      m4return_t AddRef (ClM4AliasRef ai_oAliasRef);

      //## Operation: FindChannel%944575107
      ClChannel* FindChannel (ClChannelManager* ai_poCM, const string& ai_sId, m4int16_t ai_iCSMode, ClChannel* ai_poBaseChannel = NULL);

      //## Operation: GetReferences%944575107
	  ClM4AliasReferenceSet& GetReferences();

    // Data Members for Class Attributes

      //## Attribute: sName%384D211702A1
      //## begin ClM4XMLAliasDef::sName%384D211702A1.attr preserve=no  public: string {UA} 
      string sName;
      //## end ClM4XMLAliasDef::sName%384D211702A1.attr

      //## Attribute: sM4OId%384D157A0061
      //## begin ClM4XMLAliasDef::sM4OId%384D157A0061.attr preserve=no  public: string {UA} 
      string sM4OId;
      //## end ClM4XMLAliasDef::sM4OId%384D157A0061.attr

      //## Attribute: sMode%384D15AF0306
      //## begin ClM4XMLAliasDef::sMode%384D15AF0306.attr preserve=no  public: string {UA} 
      string sMode;
      //## end ClM4XMLAliasDef::sMode%384D15AF0306.attr

      //## Attribute: sPreserve%384D15B0004B
      //## begin ClM4XMLAliasDef::sPreserve%384D15B0004B.attr preserve=no  public: string {UA} 
      string sPreserve;
      //## end ClM4XMLAliasDef::sPreserve%384D15B0004B.attr

      //## Attribute: sRemove%384D15B0011D
      //## begin ClM4XMLAliasDef::sRemove%384D15B0011D.attr preserve=no  public: string {UA} 
      string sRemove;
      //## end ClM4XMLAliasDef::sRemove%384D15B0011D.attr

      //## Attribute: sFind%384D15B001C7
      //## begin ClM4XMLAliasDef::sFind%384D15B001C7.attr preserve=no  public: string {UA} 
      string sFind;
      //## end ClM4XMLAliasDef::sFind%384D15B001C7.attr

      //## Attribute: sMDInfoLevel%3A37484601D0
      //## begin ClM4XMLAliasDef::sMDInfoLevel%3A37484601D0.attr preserve=no  public: string {UA} 
      string sMDInfoLevel;
      //## end ClM4XMLAliasDef::sMDInfoLevel%3A37484601D0.attr

      //## Attribute: sMDInfoLevel%3A37484601D0
      //## begin ClM4XMLAliasDef::sMDInfoLevel%3A37484601D0.attr preserve=no  public: string {UA} 
      string sUseFunctionalGroup;
      //## end ClM4XMLAliasDef::sMDInfoLevel%3A37484601D0.attr

	  // Additional Public Declarations
      //## begin ClM4XMLAliasDef%384D154D02DD.public preserve=yes
      //## end ClM4XMLAliasDef%384D154D02DD.public

  protected:

    //## Other Operations (specified)
      //## Operation: AddDefParam%944739019
      m4return_t AddDefParam (ClM4AliasParam& ai_oAliasParam);

      //## Operation: FindDef%944739020
      m4return_t FindDef (ClM4AliasParam& ao_iKey, ClM4AliasParam& ao_iResult, m4bool_t& ao_bFound);

      //## Operation: ApplyParams%944742373
      m4return_t ApplyParams (ClChannel* ai_poChannel);

      //## Operation: ApplyParam%944742374
      m4return_t ApplyParam (ClChannel* ai_poChannel, const ClM4AliasParam& ai_Param, const ClM4AliasParam* ai_ParamRef = NULL);

    // Additional Protected Declarations
      //## begin ClM4XMLAliasDef%384D154D02DD.protected preserve=yes
      //## end ClM4XMLAliasDef%384D154D02DD.protected

  private:
    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%384D1AC9037D
      //## Role: ClM4XMLAliasDef::oParams%384D1ACA0139
      //## begin ClM4XMLAliasDef::oParams%384D1ACA0139.role preserve=no  private: ClM4AliasParamSet {1 -> 1VHAN}
      ClM4AliasParamSet oParams;
      //## end ClM4XMLAliasDef::oParams%384D1ACA0139.role

      //## Association: M4ExeForChannel::XML::<unnamed>%384D1F05001E
      //## Role: ClM4XMLAliasDef::oDefParams%384D1F050259
      //## begin ClM4XMLAliasDef::oDefParams%384D1F050259.role preserve=no  private: ClM4AliasParamSet {1 -> 1VHAN}
      ClM4AliasParamSet oDefParams;
      //## end ClM4XMLAliasDef::oDefParams%384D1F050259.role

      //## Association: M4ExeForChannel::XML::<unnamed>%384D1AC9037D
      //## Role: ClM4XMLAliasDef::oReferences%384D1ACA0139
      //## begin ClM4XMLAliasDef::oReferences%384D1ACA0139.role preserve=no  private: ClM4AliasReferenceSet {1 -> 1VHAN}
      ClM4AliasReferenceSet oReferences;
      //## end ClM4XMLAliasDef::oReferences%384D1ACA0139.role

	  // Additional Private Declarations
      //## begin ClM4XMLAliasDef%384D154D02DD.private preserve=yes
      //## end ClM4XMLAliasDef%384D154D02DD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLAliasDef%384D154D02DD.implementation preserve=yes
      //## end ClM4XMLAliasDef%384D154D02DD.implementation

};

//## begin ClM4XMLAliasDef%384D154D02DD.postscript preserve=yes
//## end ClM4XMLAliasDef%384D154D02DD.postscript

//## begin DumpedItem%3850D89D0233.preface preserve=yes
//## end DumpedItem%3850D89D0233.preface

//## Class: DumpedItem%3850D89D0233
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DumpedItem 
{
  //## begin DumpedItem%3850D89D0233.initialDeclarations preserve=yes
  //## end DumpedItem%3850D89D0233.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DumpedItem%945198100
      DumpedItem ();

      //## Operation: DumpedItem%944822915
      DumpedItem (const string& ai_sItem);


    //## Other Operations (specified)
      //## Operation: operator <%944822916
      m4bool_t operator < (const DumpedItem& ai_oItem) const;

      //## Operation: GetValue%1071498574
      const string & GetValue () const;

    // Additional Public Declarations
      //## begin DumpedItem%3850D89D0233.public preserve=yes
      //## end DumpedItem%3850D89D0233.public

  protected:
    // Additional Protected Declarations
      //## begin DumpedItem%3850D89D0233.protected preserve=yes
      //## end DumpedItem%3850D89D0233.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sItem%3850DD480105
      //## begin DumpedItem::m_sItem%3850DD480105.attr preserve=no  private: string {UA} 
      string m_sItem;
      //## end DumpedItem::m_sItem%3850DD480105.attr

    // Additional Private Declarations
      //## begin DumpedItem%3850D89D0233.private preserve=yes
      //## end DumpedItem%3850D89D0233.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DumpedItem%3850D89D0233.implementation preserve=yes
      //## end DumpedItem%3850D89D0233.implementation

};

//## begin DumpedItem%3850D89D0233.postscript preserve=yes
//## end DumpedItem%3850D89D0233.postscript

//## begin DumpedItemSet%3850D86D0374.preface preserve=yes
//## end DumpedItemSet%3850D86D0374.preface

//## Class: DumpedItemSet%3850D86D0374; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3850D9250116;DumpedItem { -> }

typedef set< DumpedItem,less<DumpedItem> > DumpedItemSet;

//## begin DumpedItemSet%3850D86D0374.postscript preserve=yes
//## end DumpedItemSet%3850D86D0374.postscript

//## begin DumpedItemIt%3850D9E801C0.preface preserve=yes
//## end DumpedItemIt%3850D9E801C0.preface

//## Class: DumpedItemIt%3850D9E801C0
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3850DA1203AC;DumpedItemSet { -> }

typedef DumpedItemSet::iterator DumpedItemIt;

//## begin DumpedItemIt%3850D9E801C0.postscript preserve=yes
//## end DumpedItemIt%3850D9E801C0.postscript

//## begin ClM4XMLDumpedItems%3850D74B0331.preface preserve=yes
//## end ClM4XMLDumpedItems%3850D74B0331.preface

//## Class: ClM4XMLDumpedItems%3850D74B0331
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3850D81B009B;string { -> }
//## Uses: <unnamed>%3850DA3701F6;DumpedItemIt { -> }

class ClM4XMLDumpedItems 
{
  //## begin ClM4XMLDumpedItems%3850D74B0331.initialDeclarations preserve=yes
  //## end ClM4XMLDumpedItems%3850D74B0331.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Reset%944822917
      void Reset ();

      //## Operation: AddItem%944822918
      void AddItem (const string& ai_sItem);

      //## Operation: CheckItem%944822919
      m4bool_t CheckItem (const string& ai_sItem);

      //## Operation: GetSet%1071498573
      DumpedItemSet& GetSet ();

    // Additional Public Declarations
      //## begin ClM4XMLDumpedItems%3850D74B0331.public preserve=yes
      //## end ClM4XMLDumpedItems%3850D74B0331.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4XMLDumpedItems%3850D74B0331.protected preserve=yes
      //## end ClM4XMLDumpedItems%3850D74B0331.protected

  private:
    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%3850D96E028D
      //## Role: ClM4XMLDumpedItems::m_oItemSet%3850D96F0018
      //## begin ClM4XMLDumpedItems::m_oItemSet%3850D96F0018.role preserve=no  private: DumpedItemSet {1 -> 1VHAN}
      DumpedItemSet m_oItemSet;
      //## end ClM4XMLDumpedItems::m_oItemSet%3850D96F0018.role

    // Additional Private Declarations
      //## begin ClM4XMLDumpedItems%3850D74B0331.private preserve=yes
      //## end ClM4XMLDumpedItems%3850D74B0331.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLDumpedItems%3850D74B0331.implementation preserve=yes
      //## end ClM4XMLDumpedItems%3850D74B0331.implementation

};

//## begin ClM4XMLDumpedItems%3850D74B0331.postscript preserve=yes
//## end ClM4XMLDumpedItems%3850D74B0331.postscript

//## begin ClM4XMLContext%37DF520B0399.preface preserve=yes
//## end ClM4XMLContext%37DF520B0399.preface

//## Class: ClM4XMLContext%37DF520B0399
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37DFA0980272;string { -> }
//## Uses: <unnamed>%37DFE8640267;ClM4XMLCurrent { -> }
//## Uses: <unnamed>%380592AC0137;ClFileUnit { -> F}
//## Uses: <unnamed>%3826D2350120;TranslArgs { -> }
//## Uses: <unnamed>%382989B40211;TranslItemsMap { -> }
//## Uses: <unnamed>%38F73F2C0015;ClM4ObjService { -> F}
//## Uses: <unnamed>%39E2D5740094;ClCMInitParameters { -> }

// this class cleans any object and access associated
class ClM4XMLObjectCleaner
{
public:
	ClM4XMLObjectCleaner(ClChannel * &ai_poChannel, ClAccess * &ai_poAccess);
	virtual ~ClM4XMLObjectCleaner();

private:
	ClAccess  * &m_poAccess;
	ClChannel * &m_poChannel;
};

inline ClM4XMLObjectCleaner::ClM4XMLObjectCleaner(ClChannel * &ai_poChannel, ClAccess * &ai_poAccess)
: m_poChannel(ai_poChannel), m_poAccess(ai_poAccess)
{
}

inline ClM4XMLObjectCleaner::~ClM4XMLObjectCleaner()
{
	if (m_poAccess != NULL)
	{
		m_poAccess->Destroy();
	}
	if (m_poChannel != NULL)
	{
		m_poChannel->Destroy();
	}
}

// The M4XML context handler
class ClM4XMLContext 
{
  //## begin ClM4XMLContext%37DF520B0399.initialDeclarations preserve=yes
  //## end ClM4XMLContext%37DF520B0399.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLContext%937383048
      ClM4XMLContext (ClCMInitParameters &aio_oCMInitParameters);

    //## Destructor (specified)
      //## Operation: ~ClM4XMLContext%937383049
      ~ClM4XMLContext ();


    //## Other Operations (specified)
      //## Operation: Reset%942060103
      m4return_t Reset ();

	  //## Operation: ResetM4OReferences%937383050
	  m4return_t ResetM4OReferences();

      //## Operation: ProcAlias%937383050
      m4return_t ProcAlias (const string& ai_sAlias, const string& ai_sChannelId, const string& ai_sMode, const string& ai_sPreserve, const string& ai_sRemove, const string& ai_sFind, const string& ai_sMDInfoLevel, const string& ai_sUseFunctionalGroup);

      //## Operation: SetObjectService%937383051
      void SetObjectService (ClM4ObjService *ai_poChannelManager);

      //## Operation: GetCurrentByAlias%937383071
      ClM4XMLCurrent * GetCurrentByAlias (const string& ai_sAlias);

      //## Operation: AddTranslType%939289642
      m4return_t AddTranslType (const string &ai_sType, const string &ai_sMethod, const string &ai_sResultItems);

      //## Operation: AddTranslItem%939289643
      m4return_t AddTranslItem (const string &ai_sType, const string &ai_sItem);

      //## Operation: AddTranslItemArg%939372143
      m4return_t AddTranslItemArg (const string &ai_sName, const string &ai_sValue);

      //## Operation: ResetTranslItem%939372144
      void ResetTranslItem ();

      //## Operation: Translate%939372148
      m4return_t Translate (const string& ai_sM4OAlias, m4int32_t ai_iHandle, const m4VariantType& ai_vValue, m4VariantType& ao_vValueOut, m4bool_t& ao_bFound);

      //## Operation: GetInFileName%939889206
      string GetInFileName (m4int32_t ai_iIndex);

      //## Operation: ProcInFileRefs%939889207
      m4return_t ProcInFileRefs (M4DataStorage* ai_poDataStorage);

      //## Operation: AddOutFileRef%939889210
      m4int32_t AddOutFileRef (M4DataStorage *ao_poDataStorage, m4pcchar_t ai_pcFileName, string& ao_sFileExtension, m4uint32_t& ao_lFileSize);

      //## Operation: GetFileInfo%939889210
	  m4return_t GetFileInfo(m4pcchar_t ai_pcFileName, string& ao_sExtension, m4uint32_t& ao_lFileLen);

      //## Operation: Serialize%940236198
      m4return_t Serialize (ClGenericIODriver& ao_oIODriver);

      //## Operation: Deserialize%940236199
      m4return_t Deserialize (ClGenericIODriver& ai_oIODriver);

      //## Operation: PopStack%940341733
      m4return_t PopStack (ClAccess* ai_poAccess, m4int16_t ai_iNParam, m4VariantType** ao_voParam, m4bool_t ai_bReset = M4_TRUE);

      //## Operation: SetStateManager%940341736
      void SetStateManager (ClExecutorStateManager* ai_poStateManager);

      //## Operation: SetSessionId%940341737
      void SetSessionId (m4int32_t ai_iSessionId);

      //## Operation: GetObjectService%940581439
      ClM4ObjService * GetObjectService () const;

      //## Operation: ProcAliasCommands%941822487
      m4return_t ProcAliasCommands ();

      //## Operation: GenAlias%944575099
      m4return_t GenAlias (ClM4XMLStream &ao_oOut);

      //## Operation: SetUseFunctionalGroup%944575099
	  m4void_t ClM4XMLContext::SetUseFunctionalGroup ( ClAccess *ai_poAccess );

      //## Operation: GetUseFunctionalGroup%944575099
	  static m4bool_t ClM4XMLContext::GetUseFunctionalGroup ( ClAccess *ai_poAccess );

      //## Operation: AddL2ReferencesToAlias%944575099
	  m4return_t AddL2ReferencesToAlias();

      //## Operation: RetrieveL2ReferencesFromAlias%944575099
	  m4return_t RetrieveL2ReferencesFromAlias ();

      //## Operation: CloneAccess%944575099
	  ClAccess* CloneAccess (ClAccess *ai_poOrigin);

      //## Operation: RestoreAccess%944575099
	  m4return_t RestoreAccess (ClAccess *ai_poOrigin, ClAccess *ai_poDestine);

      //## Operation: ProcM4OParam%944575100
      m4return_t ProcM4OParam (const string &ai_sName, const string &ai_sValue, const string &ai_sCppType);

      //## Operation: ProcM4ORef%944575100
      m4return_t ProcM4ORef (const string &ai_sM4OAlias, const string &ai_sL2Alias);

      //## Operation: AddDumpedItem%944822920
      void AddDumpedItem (const string &ai_sItem);

      //## Operation: CheckDumpedItem%944822921
      m4bool_t CheckDumpedItem (const string &ai_sItem);

      //## Operation: SetBaseSessionChannel%945440687
      void SetBaseSessionChannel (ClChannel* ai_poSessionChannel);

      //## Operation: InitSession%945440688
      m4return_t InitSession ();

      //## Operation: SetSession%945440689
      m4return_t SetSession (m4bool_t ai_bClose = M4_FALSE);

	  // sets the sub session id
	  void SetSubSessionId(string ai_sSubSessionId);

      //## Operation: ReleaseLogicalConnection%971105347
      m4return_t ReleaseLogicalConnection ();

      //## Operation: GetSessionId%971105348
      m4uint32_t GetSessionId () const;

	  // retrieves the sub session id
	  const string & GetSubSessionId() const;

      //## Operation: GetSessionChannel%971105349
      ClChannel* GetBaseSessionChannel () const;

      //## Operation: GetStateManager%971105350
      ClExecutorStateManager* GetStateManager () const;

      //## Operation: Init%971105356
      void Init ();

      //## Operation: Update%978518712
      void Update (ClCMInitParameters &aio_oCMInitParameters);

      //## Operation: AddDumpedM4O%1071498570
      void AddDumpedM4O (const string &ai_sM4O);

      //## Operation: CheckDumpedM4O%1071498571
      m4bool_t CheckDumpedM4O (const string &ai_sM4O);

      //## Operation: DumpM4OInfo%1071498572
      void DumpM4OInfo (ClM4XMLStream& ao_oOut);

      //## Operation: GetUniqueId%4039402937
      string& GetUniqueId ();

      //## Operation: GetUniqueId%4039402937
      void InitUniqueId ();

      //## Operation: Encrypt%3043937852
	  m4return_t Encrypt(string &aio_sValue, m4bool_t ai_bIsNull, string& ai_sFuncGroups);

      //## Operation: DeCrypt%0425002933
	  m4return_t DeCrypt(string &aio_sValue, m4bool_t &ao_bIsNull, string& ai_sFuncGroups);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: PersistLevel%3AE051E800A1
      const m4uint16_t& GetPersistLevel () const;
      void SetPersistLevel (const m4uint16_t& value);

      //## Attribute: LastAccess%3AE0522E0124
      const m4uint32_t& GetLastAccess () const;
      void SetLastAccess (const m4uint32_t& value);

      //## Operation: ExecLock%1071498572
	  void ExecLock();

      //## Operation: ExecUnlock%1071498572
	  void ExecUnlock();

      //## Operation: CheckActiveAuthentication%1071498572
	  m4return_t CheckActiveAuthentication();
	  void SetEnableMethods(m4bool_t ai_bEnable);
	  const m4bool_t GetEnableMethods() const;

      //## Operation: SetAuthPassword%1071498572
	  void SetAuthPassword(string &ai_sAuthPassword);

      //## Operation: GetAuthPassword%1071498572
	  const string & GetAuthPassword() const;

	  void SetEnableNulls(m4bool_t ai_bEnableNulls);
	  const m4bool_t GetEnableNulls() const;

	  void SetEnableAuth(m4bool_t ai_bEnableAuth);
	  const m4bool_t GetEnableAuth() const;

	  void SetDataMode(string & ai_sDataMode);
	  const string & GetDataMode() const;

	  // Access functions
	  m4bool_t		CanRead( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	  m4bool_t		CanWrite( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	  m4bool_t		CanExecute( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	  m4bool_t		MustAuthenticate( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	  m4bool_t		IsEncrypted( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, string &ao_sFuncGroups) ;
	  m4uint32_t	GetSecurityMask( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;

	  // enables security
	  void				SetHaveFunctionalGroup(m4bool_t ai_bHaveFunctionalGroup);
	  const m4bool_t	GetHaveFunctionalGroup();

	  // enables blob files support
	  void				SetEnableBlobFiles(m4bool_t ai_bEnableBlobFiles);
	  const m4bool_t	GetEnableBlobFiles();

	  void				SetReqNumber(m4int_t ai_iReqNum);
	  const m4int_t		GetReqNumber();

	  // Additional Public Declarations
      //## begin ClM4XMLContext%37DF520B0399.public preserve=yes
      //## end ClM4XMLContext%37DF520B0399.public

  protected:

    //## Other Operations (specified)
      //## Operation: Indent%937826954
      static void Indent (ostream &ao_oStream, m4int16_t ai_iIndent);

      //## Operation: ExecuteTransl%939289645
      m4return_t ExecuteTransl (TranslArgs &ai_oArgs, m4bool_t ai_bDoReset = M4_TRUE);

      //## Operation: GetTranslAccess%939289647
      ClAccess * GetTranslAccess ();

      //## Operation: ResetM4OAlias%940247661
      void ResetM4OAlias ();

      //## Operation: ResetFileRefs%972490548
      void ResetFileRefs ();

      //## Operation: GetLastAuthenticationTime%972490548
	  m4uint32_t GetLastAuthenticationTime() const;
	  void SetLastAuthenticationTime(m4uint32_t ai_iLastAuthenticationTime);

      //## Operation: GetMaxAuthenticationTime%972490548
	  m4uint32_t GetMaxAuthenticationTime();

	  // retrieves a application parameter value
	  string LoadParamValue(m4pcchar_t ai_pcApplication, m4pcchar_t ai_pcSection, m4pcchar_t ai_pcKey);
	  
	  // Data Members for Class Attributes

      //## begin ClM4XMLContext::PersistLevel%3AE051E800A1.attr preserve=no  public: m4uint16_t {U} 0
      m4uint16_t m_iPersistLevel;
      //## end ClM4XMLContext::PersistLevel%3AE051E800A1.attr

      //## begin ClM4XMLContext::LastAccess%3AE0522E0124.attr preserve=no  public: m4uint32_t {U} 0
      m4uint32_t m_uiLastAccess;
      //## end ClM4XMLContext::LastAccess%3AE0522E0124.attr

    // Additional Protected Declarations
      //## begin ClM4XMLContext%37DF520B0399.protected preserve=yes
      //## end ClM4XMLContext%37DF520B0399.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poObjectService%37DF53700338
      //## begin ClM4XMLContext::m_poObjectService%37DF53700338.attr preserve=no  private: ClM4ObjService * {UA} NULL
      ClM4ObjService *m_poObjectService;
      //## end ClM4XMLContext::m_poObjectService%37DF53700338.attr

      //## Attribute: m_oAliasMap%37DF91910339
      //## begin ClM4XMLContext::m_oAliasMap%37DF91910339.attr preserve=no  private: M4OAliasMap {UA} 
      M4OAliasMap m_oAliasMap;
      //## end ClM4XMLContext::m_oAliasMap%37DF91910339.attr

      //## Attribute: m_poTranslAccess%37FCD4B00216
      //## begin ClM4XMLContext::m_poTranslAccess%37FCD4B00216.attr preserve=no  private: ClAccess * {UA} NULL
      ClAccess *m_poTranslAccess;
      //## end ClM4XMLContext::m_poTranslAccess%37FCD4B00216.attr

      //## Attribute: m_oTranslItems%37FCFB040049
      //## begin ClM4XMLContext::m_oTranslItems%37FCFB040049.attr preserve=no  private: TranslItemsMap {UA} 
      TranslItemsMap m_oTranslItems;
      //## end ClM4XMLContext::m_oTranslItems%37FCFB040049.attr

      //## Attribute: m_poTranslItemData%37FDC4CA0361
      //## begin ClM4XMLContext::m_poTranslItemData%37FDC4CA0361.attr preserve=no  private: TranslItemData * {UA} NULL
      TranslItemData *m_poTranslItemData;
      //## end ClM4XMLContext::m_poTranslItemData%37FDC4CA0361.attr

      //## Attribute: m_iNInFileRefs%380591D1038A
      //## begin ClM4XMLContext::m_iNInFileRefs%380591D1038A.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iNInFileRefs;
      //## end ClM4XMLContext::m_iNInFileRefs%380591D1038A.attr

      //## Attribute: m_poInFileRefs%380591D2006B
      //## begin ClM4XMLContext::m_poInFileRefs%380591D2006B.attr preserve=no  private: ClFileUnit** {UA} NULL
      ClFileUnit** m_poInFileRefs;
      //## end ClM4XMLContext::m_poInFileRefs%380591D2006B.attr

      //## Attribute: m_iNOutFileRefs%3805A794009C
      //## begin ClM4XMLContext::m_iNOutFileRefs%3805A794009C.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iNOutFileRefs;
      //## end ClM4XMLContext::m_iNOutFileRefs%3805A794009C.attr

      //## Attribute: m_poStateManager%380CAE9F003D
      //## begin ClM4XMLContext::m_poStateManager%380CAE9F003D.attr preserve=no  private: ClExecutorStateManager* {UA} NULL
      ClExecutorStateManager* m_poStateManager;
      //## end ClM4XMLContext::m_poStateManager%380CAE9F003D.attr

      //## Attribute: m_iSessionId%380CAE9F012D
      //## begin ClM4XMLContext::m_iSessionId%380CAE9F012D.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iSessionId;
      //## end ClM4XMLContext::m_iSessionId%380CAE9F012D.attr

	  // The sub session id
	  string m_sSubSessionId;

	  // Permission provider
	  ClT3AccessProvider* m_poT3AccessProvider;

	  // The time of last authentication
	  m4uint32_t m_uLastAuthenticationTime;

	  // The max time the user reuse the authentication
	  m4uint32_t m_uMaxAuthenticationTime;

	  // The authorization password
	  string m_sAuthPassword;

	  // The enable nulls status
	  m4bool_t m_bEnableNulls;

	  // The enable auth status
	  m4bool_t m_bEnableAuth;

	  // The enable methods status
	  m4bool_t m_bEnableMethods;

	  // The data serialization mode
	  string m_sDataMode;

	  // Enables security using functional groups
	  m4bool_t m_bHaveFunctionalGroup;

	  // Enables blob files support. Otherwite only basic info will be returned
	  m4bool_t m_bEnableBlobFiles;

      //## Attribute: m_poBaseSessionChannel%385A46BB0076
      //## begin ClM4XMLContext::m_poBaseSessionChannel%385A46BB0076.attr preserve=no  private: ClChannel* {UA} NULL
      ClChannel* m_poBaseSessionChannel;
      //## end ClM4XMLContext::m_poBaseSessionChannel%385A46BB0076.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%384D16380046
      //## Role: ClM4XMLContext::m_oAliasDef%384D16380213
      //## begin ClM4XMLContext::m_oAliasDef%384D16380213.role preserve=no  private: ClM4XMLAliasDef {1 -> 1VHAN}
      ClM4XMLAliasDef m_oAliasDef;
      //## end ClM4XMLContext::m_oAliasDef%384D16380213.role

      //## Association: M4ExeForChannel::XML::<unnamed>%3850DAB403C3
      //## Role: ClM4XMLContext::m_oDumpedItems%3850DAB5017F
      //## begin ClM4XMLContext::m_oDumpedItems%3850DAB5017F.role preserve=no  private: ClM4XMLDumpedItems { -> 1VHAN}
      ClM4XMLDumpedItems m_oDumpedItems;
      //## end ClM4XMLContext::m_oDumpedItems%3850DAB5017F.role

      //## Association: M4ExeForChannel::XML::<unnamed>%3FDDC4BE02C5
      //## Role: ClM4XMLContext::m_oDumpedM4O%3FDDC4BF0231
      //## begin ClM4XMLContext::m_oDumpedM4O%3FDDC4BF0231.role preserve=no  private: ClM4XMLDumpedItems { -> 1VHAN}
      ClM4XMLDumpedItems m_oDumpedM4O;
      //## end ClM4XMLContext::m_oDumpedM4O%3FDDC4BF0231.role

	  // The internal exec mutext
	  ClMutex* m_poExecMutex;

	  // The request number
	  m4int_t m_iReqNumber;

	  // The context unique id
	  string m_sUUID;

    // Additional Private Declarations
      //## begin ClM4XMLContext%37DF520B0399.private preserve=yes
      //## end ClM4XMLContext%37DF520B0399.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLContext%37DF520B0399.implementation preserve=yes
      //## end ClM4XMLContext%37DF520B0399.implementation

};

//## begin ClM4XMLContext%37DF520B0399.postscript preserve=yes
//## end ClM4XMLContext%37DF520B0399.postscript

//## begin ClM4XMLItem%37DF59240175.preface preserve=yes
//## end ClM4XMLItem%37DF59240175.preface

//## Class: ClM4XMLItem%37DF59240175
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37DFE8A30272;ClM4XMLContext { -> }

class ClM4XMLItem 
{
  //## begin ClM4XMLItem%37DF59240175.initialDeclarations preserve=yes
  //## end ClM4XMLItem%37DF59240175.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLItem%937383057
      ClM4XMLItem (ClM4XMLContext &ai_oContext, const string& ai_sItem);

      //## Operation: ClM4XMLItem%937555629
      ClM4XMLItem (const string& ai_sItem, m4bool_t ai_bForceData = M4_FALSE);

      //## Operation: ClM4XMLItem%938162619
      ClM4XMLItem ();

    //## Destructor (specified)
      //## Operation: ~ClM4XMLItem%937383058
      ~ClM4XMLItem ();


    //## Other Operations (specified)
      //## Operation: Match%937383059
      m4bool_t Match (ClM4XMLItem &ai_oItem);

      //## Operation: Execute%937383065
      m4return_t Execute (ClM4XMLContext &ai_oContext, m4int16_t ai_iNArgs, m4uint32_t ai_uiRegisterUniqueId);

      //## Operation: SetValue%937383066
      m4return_t SetValue (ClM4XMLContext &ai_oContext, const string& ai_sValue, m4int32_t ai_iLevel = 0);

      //## Operation: SetFileValueByRef%939889209
      m4return_t SetFileValueByRef (ClM4XMLContext &ai_oContext, const string& ai_sFileName, const string& ai_sFileDescr);

      //## Operation: SetNull%939630177
      m4return_t SetNull (ClM4XMLContext &ai_oContext);

      //## Operation: Reset%937383068
      void Reset ();

      //## Operation: Reset%938162620
      m4return_t Reset (ClM4XMLContext &ai_oContext, const string& ai_sItem, m4bool_t ai_bUpdate = M4_FALSE, m4bool_t ai_bRealItem = M4_TRUE);

      //## Operation: Move%937470765
      m4return_t Move (ClM4XMLContext &ai_oContext, m4bool_t ai_bUpdate = M4_FALSE, m4bool_t ai_bRealItem = M4_TRUE);

      //## Operation: GetResult%937470761
      const string & GetResult () const;

      //## Operation: SetResult%937470761
      void SetResult (const string& ai_sResult);

	  const m4bool_t GetResultIsNull() const;

      //## Operation: GetResultType%941188386
      m4int16_t GetResultType () const;

      //## Operation: GetNArgs%937470763
      m4int16_t GetNArgs () const;

      //## Operation: IsVariableArgs%963252651
      m4bool_t IsVariableArgs () const;

      //## Operation: GetAccess%937470764
      ClAccess * GetAccess () const;

      //## Operation: GetItem%937826947
      m4pchar_t GetItem () const;

      //## Operation: GetM4OItem%939630178
      m4pchar_t GetM4OItem () const;

      //## Operation: GetArgIndex%937470762
      m4int16_t GetArgIndex (m4pcchar_t ai_pcArgName);

      //## Operation: GetM4OAlias%937555630
      m4pchar_t GetM4OAlias () const;

      //## Operation: GetNode%937555631
      m4pchar_t GetNode () const;

      //## Operation: GetRegister%937555632
      m4pchar_t GetRegister () const;

      //## Operation: GetRegisterMin%937922617
      m4int32_t GetRegisterMin () const;

      //## Operation: GetRegisterMax%937922618
      m4int32_t GetRegisterMax () const;

      //## Operation: SetNode%938708977
      void SetNode (const string &ai_sNode);

      //## Operation: SetRegister%938708978
      void SetRegister (const string &ai_sRegister);

	  //## Operation: SetRegisterById%93870897a
      void SetRegisterById (const string &ai_sRegister);

      //## Operation: SetItem%938708979
      void SetItem (const string &ai_sItem);

      //## Operation: SetFilterType%939117431
      void SetFilterType (const string &ai_sType);

      //## Operation: RemoveFilter%939117432
      m4return_t RemoveFilter (ClM4XMLContext &ai_oContext);

      //## Operation: ExecuteFilter%939117433
      m4return_t ExecuteFilter (ClM4XMLContext &ai_oContext, m4int16_t ai_iNArgs, m4pcchar_t *ai_Mode, m4pcchar_t *ai_Name, m4pcchar_t *ai_Value);

      //## Operation: SetFilterFormula%939200067
      void SetFilterFormula (const string &ai_sFormula);

      //## Operation: GetFilterType%939200068
      const string& GetFilterType () const;

      //## Operation: GetHandle%939289646
      m4int32_t GetHandle () const;

      //## Operation: GetValue%939630176
      m4return_t GetValue (ClM4XMLContext &ai_oContext, string& ao_sValue);

      //## Operation: IsRegRelative%940581440
      m4bool_t IsRegRelative () const;

      //## Operation: ConvArg%942245952
      m4return_t ConvArg (m4int16_t ai_iIndex, m4bool_t ai_bIsVariableArgs, const string &ai_sValue, const string &ai_sType, m4VariantType& aio_vResult);

      //## Operation: ParseLimit%963491328
      m4int32_t ParseLimit (string &ai_sValue, ClAccessRecordSet *ai_poRS);

      // Additional Public Declarations
	  void SetIsById (bool ai_bIsRegId); 
	  bool GetIsById ();

      //## begin ClM4XMLItem%37DF59240175.public preserve=yes
      //## end ClM4XMLItem%37DF59240175.public

  protected:

    //## Other Operations (specified)
      //## Operation: Parse%937383067
      m4return_t Parse (const string &ai_sString, m4bool_t ai_bForceData = M4_FALSE);

      //## Operation: Init%937555633
      void Init ();

      //## Operation: Parse%938708980
      m4return_t Parse ();

    // Additional Protected Declarations
      //## begin ClM4XMLItem%37DF59240175.protected preserve=yes
      //## end ClM4XMLItem%37DF59240175.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcM4OAlias%37DF59FA024F
      //## begin ClM4XMLItem::m_pcM4OAlias%37DF59FA024F.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcM4OAlias;
      //## end ClM4XMLItem::m_pcM4OAlias%37DF59FA024F.attr

      //## Attribute: m_pcNode%37DF5A0E0230
      //## begin ClM4XMLItem::m_pcNode%37DF5A0E0230.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcNode;
      //## end ClM4XMLItem::m_pcNode%37DF5A0E0230.attr

      //## Attribute: m_iRegisterMin%37DF5A1803CF
      //## begin ClM4XMLItem::m_iRegisterMin%37DF5A1803CF.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iRegisterMin;
      //## end ClM4XMLItem::m_iRegisterMin%37DF5A1803CF.attr

      //## Attribute: m_iRegisterMax%37DF5AE4035A
      //## begin ClM4XMLItem::m_iRegisterMax%37DF5AE4035A.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iRegisterMax;
      //## end ClM4XMLItem::m_iRegisterMax%37DF5AE4035A.attr

      //## Attribute: m_pcItem%37DF5A270344
      //## begin ClM4XMLItem::m_pcItem%37DF5A270344.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcItem;
      //## end ClM4XMLItem::m_pcItem%37DF5A270344.attr

      //## Attribute: m_pcM4OItem%3802046E0217
      //## begin ClM4XMLItem::m_pcM4OItem%3802046E0217.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcM4OItem;
      //## end ClM4XMLItem::m_pcM4OItem%3802046E0217.attr

      //## Attribute: m_pcValue%37DF5A330392
      //## begin ClM4XMLItem::m_pcValue%37DF5A330392.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcValue;
      //## end ClM4XMLItem::m_pcValue%37DF5A330392.attr

      //## Attribute: m_sResult%37E0AB1301D1
      //## begin ClM4XMLItem::m_sResult%37E0AB1301D1.attr preserve=no  private: string {UA} 
      string m_sResult;
      //## end ClM4XMLItem::m_sResult%37E0AB1301D1.attr

      //## Attribute: m_poAccess%37E1495101BE
      //## begin ClM4XMLItem::m_poAccess%37E1495101BE.attr preserve=no  private: ClAccess * {UA} 
      ClAccess *m_poAccess;
      //## end ClM4XMLItem::m_poAccess%37E1495101BE.attr

      //## Attribute: m_iHandle%37E1496002CE
      //## begin ClM4XMLItem::m_iHandle%37E1496002CE.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iHandle;
      //## end ClM4XMLItem::m_iHandle%37E1496002CE.attr

      //## Attribute: m_pcRegister%37EFBF820111
      //## begin ClM4XMLItem::m_pcRegister%37EFBF820111.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcRegister;
      //## end ClM4XMLItem::m_pcRegister%37EFBF820111.attr

      //## Attribute: m_sNode%37F3AEAF0033
      //## begin ClM4XMLItem::m_sNode%37F3AEAF0033.attr preserve=no  private: string {UA} 
      string m_sNode;
      //## end ClM4XMLItem::m_sNode%37F3AEAF0033.attr

      //## Attribute: m_sRegister%37F3AEBC00BE
      //## begin ClM4XMLItem::m_sRegister%37F3AEBC00BE.attr preserve=no  private: string {UA} 
      string m_sRegister;
      //## end ClM4XMLItem::m_sRegister%37F3AEBC00BE.attr

      //## Attribute: m_sItem%37F3AEBC0212
      //## begin ClM4XMLItem::m_sItem%37F3AEBC0212.attr preserve=no  private: string {UA} 
      string m_sItem;
      //## end ClM4XMLItem::m_sItem%37F3AEBC0212.attr

      //## Attribute: m_poNode%37F3B5800000
      //## begin ClM4XMLItem::m_poNode%37F3B5800000.attr preserve=no  private: ClNode * {UA} 
      ClNode *m_poNode;
      //## end ClM4XMLItem::m_poNode%37F3B5800000.attr

      //## Attribute: m_sFilterType%37F9D5EC02CA
      //## begin ClM4XMLItem::m_sFilterType%37F9D5EC02CA.attr preserve=no  private: string {UA} 
      string m_sFilterType;
      //## end ClM4XMLItem::m_sFilterType%37F9D5EC02CA.attr

      //## Attribute: m_sFilterFormula%37FB0E520345
      //## begin ClM4XMLItem::m_sFilterFormula%37FB0E520345.attr preserve=no  private: string {UA} 
      string m_sFilterFormula;
      //## end ClM4XMLItem::m_sFilterFormula%37FB0E520345.attr

      //## Attribute: m_bRegRelative%38103FB20291
      //## begin ClM4XMLItem::m_bRegRelative%38103FB20291.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bRegRelative;
      //## end ClM4XMLItem::m_bRegRelative%38103FB20291.attr

      //## Attribute: m_bIsParsed%381469540137
      //## begin ClM4XMLItem::m_bIsParsed%381469540137.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bIsParsed;
      //## end ClM4XMLItem::m_bIsParsed%381469540137.attr

      //## Attribute: m_iResultType%381964DC013D
      //## begin ClM4XMLItem::m_iResultType%381964DC013D.attr preserve=no  private: m4int16_t {UA} -1
      m4int16_t m_iResultType;
      //## end ClM4XMLItem::m_iResultType%381964DC013D.attr

      //## Attribute: m_sRegisterMin%396DB5D3006D
      //## begin ClM4XMLItem::m_sRegisterMin%396DB5D3006D.attr preserve=no  private: string {UA} 
      string m_sRegisterMin;
      //## end ClM4XMLItem::m_sRegisterMin%396DB5D3006D.attr

      //## Attribute: m_sRegisterMax%396DB5E103DF
      //## begin ClM4XMLItem::m_sRegisterMax%396DB5E103DF.attr preserve=no  private: string {UA} 
      string m_sRegisterMax;
      //## end ClM4XMLItem::m_sRegisterMax%396DB5E103DF.attr

	  //## Attribute: m_sRegisterMax%396DB5E103ED
      //## begin ClM4XMLItem::m_sRegisterMax%396DB5E103ED.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bIsRegId;
      //## end ClM4XMLItem::m_sRegisterMax%396DB5E103ED.attr

	  //## Attribute: m_bResultIsNull%396DB5E103ED
      //## begin ClM4XMLItem::m_bResultIsNull%396DB5E103ED.attr preserve=no  private: m4bool_t {UA} 
	  m4bool_t m_bResultIsNull;
      //## end ClM4XMLItem::m_bResultIsNull%396DB5E103ED.attr

    // Additional Private Declarations
      //## begin ClM4XMLItem%37DF59240175.private preserve=yes
      //## end ClM4XMLItem%37DF59240175.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLItem%37DF59240175.implementation preserve=yes
      //## end ClM4XMLItem%37DF59240175.implementation

};

//## begin ClM4XMLItem%37DF59240175.postscript preserve=yes
//## end ClM4XMLItem%37DF59240175.postscript

//## begin ClM4XMLItemFilterList%37E2282403B0.preface preserve=yes
//## end ClM4XMLItemFilterList%37E2282403B0.preface

//## Class: ClM4XMLItemFilterList%37E2282403B0
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E2284202F5;ClM4XMLContext { -> }
//## Uses: <unnamed>%37F4E49E013C;ClM4XMLStream { -> }
//## Uses: <unnamed>%3826D3B00084;M4ItemSetIt { -> }
//## Uses: <unnamed>%3826D57801F2;M4LevelType { -> }

class ClM4XMLItemFilterList 
{
  //## begin ClM4XMLItemFilterList%37E2282403B0.initialDeclarations preserve=yes
  //## end ClM4XMLItemFilterList%37E2282403B0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLItemFilterList%937555625
      ClM4XMLItemFilterList ();

    //## Destructor (specified)
      //## Operation: ~ClM4XMLItemFilterList%937555626
      ~ClM4XMLItemFilterList ();


    //## Other Operations (specified)
      //## Operation: Execute%937555628
      m4return_t Execute (ClM4XMLStream &ao_oOut);

      //## Operation: DumpTranslation%939372147
      void DumpTranslation (const string &ai_sM4OAlias, m4int32_t ai_iHandle, const m4VariantType& ai_vValue, ClM4XMLStream &ao_oOut);

      //## Operation: DumpParentPosition%941119357
      m4return_t DumpParentPosition (ClNode* ai_poNode, ClM4XMLStream &ao_oOut, m4bool_t ai_bFirst = M4_FALSE);

      //## Operation: Reset%938437889
      m4return_t Reset (ClM4XMLContext *ai_poContext);

      //## Operation: AddNode%938437890
      m4return_t AddNode (const string &ai_sNode, const string &ai_sRecursive, const string &ai_bSyncCurrent, const string &ai_sEnableDeltas);

      //## Operation: AddRegister%938437891
      m4return_t AddRegister (const string &ai_sRegister, m4bool_t ai_bConvRange = M4_FALSE);

      //## Operation: AddItem%937555634
      m4return_t AddItem (const string &ai_sItem);

    // Additional Public Declarations
      //## begin ClM4XMLItemFilterList%37E2282403B0.public preserve=yes
      //## end ClM4XMLItemFilterList%37E2282403B0.public

  protected:

    //## Other Operations (specified)

      //## Operation: LoadNodeList%937826948
	  m4void_t LoadNodeList (m4pcchar_t ai_pcNodeId, set<string> &ao_oNodeList, ClAccess *ai_poAccess);

	  //## Operation: DumpVectorDeltas%937826948
	  m4void_t DumpVectorDeltas (ClVectorDelta* ai_oVDelta, ClM4XMLStream &ao_oOut);
      
	  //## Operation: DumpNode%937826948
      m4return_t DumpNode (M4ItemSetIt &ai_iIt, const StSetItem *ai_poSetItem, m4bool_t ai_bDumpData, ClM4XMLStream &ao_oOut);

      //## Operation: DumpRegister%937826949
      m4return_t DumpRegister (const string &ai_sM4OAlias, M4ItemSetIt &ai_iIt, ClM4XMLItem *ai_poItem, ClRegister &ai_oRegister, ClVectorDelta* ai_oVDelta, ClM4XMLStream &ao_oOut);

      //## Operation: DumpRBNodes%937826953
	  m4return_t DumpChildRBNodes (const string &ai_sM4OAlias, M4ItemSetIt &ai_iIt, ClNodeDef *ai_poNodeDef, m4bool_t ai_bDumpData, ClM4XMLStream &ao_oOut);

      //## Operation: GetRecordStatus%937826949
      m4return_t GetRecordStatus (ClRegister &ai_oRegister, m4uint8_t &ao_uiStatus);

      //## Operation: DumpItem%937826950
      m4return_t DumpItem (const string &ai_sM4OAlias, ClItem &ai_iItem, ClM4XMLStream &ao_oOut);

      //## Operation: DumpItemDef%940504733
      m4return_t DumpItemDef (ClItem &ai_iItem, ClM4XMLStream &ao_oOut);

      //## Operation: Update%938437892
      void Update ();

      //## Operation: UpdateVector%938598811
      m4return_t UpdateVector ();

      //## Operation: Reset%938708972
      void Reset ();

    // Additional Protected Declarations
      //## begin ClM4XMLItemFilterList%37E2282403B0.protected preserve=yes
      //## end ClM4XMLItemFilterList%37E2282403B0.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poContext%37E22F2300DA
      //## begin ClM4XMLItemFilterList::m_poContext%37E22F2300DA.attr preserve=no  private: ClM4XMLContext * {UA} NULL
      ClM4XMLContext *m_poContext;
      //## end ClM4XMLItemFilterList::m_poContext%37E22F2300DA.attr

      //## Attribute: m_sNode%37E22F3D00F6
      //## begin ClM4XMLItemFilterList::m_sNode%37E22F3D00F6.attr preserve=no  private: string {UA} 
      string m_sNode;
      //## end ClM4XMLItemFilterList::m_sNode%37E22F3D00F6.attr

      //## Attribute: m_sRegister%37E22F480047
      //## begin ClM4XMLItemFilterList::m_sRegister%37E22F480047.attr preserve=no  private: string {UA} 
      string m_sRegister;
      //## end ClM4XMLItemFilterList::m_sRegister%37E22F480047.attr

      //## Attribute: m_sM4OAlias%37E230510098
      //## begin ClM4XMLItemFilterList::m_sM4OAlias%37E230510098.attr preserve=no  private: string {UA} 
      string m_sM4OAlias;
      //## end ClM4XMLItemFilterList::m_sM4OAlias%37E230510098.attr

      //## Attribute: m_oItemSet%37E247E7000E
      //## begin ClM4XMLItemFilterList::m_oItemSet%37E247E7000E.attr preserve=no  private: M4ItemSet {UA} 
      M4ItemSet m_oItemSet;
      //## end ClM4XMLItemFilterList::m_oItemSet%37E247E7000E.attr

      //## Attribute: m_sItem%37EFBCAE0229
      //## begin ClM4XMLItemFilterList::m_sItem%37EFBCAE0229.attr preserve=no  private: string {UA} 
      string m_sItem;
      //## end ClM4XMLItemFilterList::m_sItem%37EFBCAE0229.attr

      //## Attribute: m_bConvRange%396C568500E8
      //## begin ClM4XMLItemFilterList::m_bConvRange%396C568500E8.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bConvRange;
      //## end ClM4XMLItemFilterList::m_bConvRange%396C568500E8.attr

      //## Attribute: m_bRecursive%3FB8AB5601FE
      //## begin ClM4XMLItemFilterList::m_bRecursive%3FB8AB5601FE.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bRecursive;
      //## end ClM4XMLItemFilterList::m_bRecursive%3FB8AB5601FE.attr

	  // Indicates if we have to sync the current register with the client
	  m4bool_t m_bSyncCurrent;

      //## Attribute: m_bEnableDeltas%3FB8AB5601FE
      //## begin ClM4XMLItemFilterList::m_bEnableDeltas%3FB8AB5601FE.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bEnableDeltas;
      //## end ClM4XMLItemFilterList::m_bEnableDeltas%3FB8AB5601FE.attr

	  // Additional Private Declarations
      //## begin ClM4XMLItemFilterList%37E2282403B0.private preserve=yes
      //## end ClM4XMLItemFilterList%37E2282403B0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLItemFilterList%37E2282403B0.implementation preserve=yes
      //## end ClM4XMLItemFilterList%37E2282403B0.implementation

};

//## begin ClM4XMLItemFilterList%37E2282403B0.postscript preserve=yes
//## end ClM4XMLItemFilterList%37E2282403B0.postscript

// Class ClM4XMLCurrent 

// Class TranslArgs 


//## Other Operations (inline)
inline m4int16_t TranslArgs::Size () const
{
  //## begin TranslArgs::Size%939289650.body preserve=yes
	return m_oArgsMap.size();
  //## end TranslArgs::Size%939289650.body
}

inline const string & TranslArgs::GetNode () const
{
  //## begin TranslArgs::GetNode%939289653.body preserve=yes
	return m_sNode;
  //## end TranslArgs::GetNode%939289653.body
}

inline const string & TranslArgs::GetItem () const
{
  //## begin TranslArgs::GetItem%939289654.body preserve=yes
	return m_sItem;
  //## end TranslArgs::GetItem%939289654.body
}

// Class TranslItemPK 

// Class TranslItemDataStr 

// Class TranslItemData 

// Class ClM4XMLStream 


//## Other Operations (inline)
inline ClM4XMLStream & ClM4XMLStream::operator << (ClM4XMLStream& (*f)(ClM4XMLStream&))
{
  //## begin ClM4XMLStream::operator <<%938775332.body preserve=yes
	return (*f)(*this);
  //## end ClM4XMLStream::operator <<%938775332.body
}

inline void ClM4XMLStream::SetBuffer (M4DataStorage *ai_Data)
{
  //## begin ClM4XMLStream::SetBuffer%938775330.body preserve=yes
	m_poData = ai_Data;
  //## end ClM4XMLStream::SetBuffer%938775330.body
}

inline M4DataStorage* ClM4XMLStream::GetpDataStorage () const
{
  //## begin ClM4XMLStream::GetpDataStorage%939889211.body preserve=yes
	return m_poData;
  //## end ClM4XMLStream::GetpDataStorage%939889211.body
}

// Class StSetItem 

// Class M4XMLString 

// Class ClM4AliasParam 

// Class ClM4XMLAliasDef 

// Class DumpedItem 


//## Other Operations (inline)
inline const string & DumpedItem::GetValue () const
{
  //## begin DumpedItem::GetValue%1071498574.body preserve=yes
	return m_sItem;
  //## end DumpedItem::GetValue%1071498574.body
}

// Class ClM4XMLDumpedItems 

// Class ClM4XMLContext 


//## Other Operations (inline)
inline void ClM4XMLContext::SetObjectService (ClM4ObjService *ai_poChannelManager)
{
  //## begin ClM4XMLContext::SetObjectService%937383051.body preserve=yes
	m_poObjectService = ai_poChannelManager;
  //## end ClM4XMLContext::SetObjectService%937383051.body
}

inline void ClM4XMLContext::SetStateManager (ClExecutorStateManager* ai_poStateManager)
{
  //## begin ClM4XMLContext::SetStateManager%940341736.body preserve=yes
	m_poStateManager = ai_poStateManager;
  //## end ClM4XMLContext::SetStateManager%940341736.body
}

inline void ClM4XMLContext::SetSessionId (m4int32_t ai_iSessionId)
{
  //## begin ClM4XMLContext::SetSessionId%940341737.body preserve=yes
	m_iSessionId = ai_iSessionId;
  //## end ClM4XMLContext::SetSessionId%940341737.body
}

inline void ClM4XMLContext::SetSubSessionId (string ai_sSubSessionId)
{
	m_sSubSessionId = ai_sSubSessionId;
}

inline ClM4ObjService * ClM4XMLContext::GetObjectService () const
{
  //## begin ClM4XMLContext::GetObjectService%940581439.body preserve=yes
	return m_poObjectService;
  //## end ClM4XMLContext::GetObjectService%940581439.body
}

inline void ClM4XMLContext::SetBaseSessionChannel (ClChannel* ai_poSessionChannel)
{
  //## begin ClM4XMLContext::SetBaseSessionChannel%945440687.body preserve=yes
	m_poBaseSessionChannel = ai_poSessionChannel;
  //## end ClM4XMLContext::SetBaseSessionChannel%945440687.body
}

inline m4uint32_t ClM4XMLContext::GetSessionId () const
{
  //## begin ClM4XMLContext::GetSessionId%971105348.body preserve=yes
	return m_iSessionId;
  //## end ClM4XMLContext::GetSessionId%971105348.body
}

inline const string & ClM4XMLContext::GetSubSessionId () const
{
	return m_sSubSessionId;
}

inline ClChannel* ClM4XMLContext::GetBaseSessionChannel () const
{
  //## begin ClM4XMLContext::GetSessionChannel%971105349.body preserve=yes
	return m_poBaseSessionChannel;
  //## end ClM4XMLContext::GetSessionChannel%971105349.body
}

inline ClExecutorStateManager* ClM4XMLContext::GetStateManager () const
{
  //## begin ClM4XMLContext::GetStateManager%971105350.body preserve=yes
	return m_poStateManager;
  //## end ClM4XMLContext::GetStateManager%971105350.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4uint16_t& ClM4XMLContext::GetPersistLevel () const
{
  //## begin ClM4XMLContext::GetPersistLevel%3AE051E800A1.get preserve=no
  return m_iPersistLevel;
  //## end ClM4XMLContext::GetPersistLevel%3AE051E800A1.get
}

inline void ClM4XMLContext::SetPersistLevel (const m4uint16_t& value)
{
  //## begin ClM4XMLContext::SetPersistLevel%3AE051E800A1.set preserve=no
  m_iPersistLevel = value;
  //## end ClM4XMLContext::SetPersistLevel%3AE051E800A1.set
}

inline const m4uint32_t& ClM4XMLContext::GetLastAccess () const
{
  //## begin ClM4XMLContext::GetLastAccess%3AE0522E0124.get preserve=no
  return m_uiLastAccess;
  //## end ClM4XMLContext::GetLastAccess%3AE0522E0124.get
}

inline void ClM4XMLContext::SetLastAccess (const m4uint32_t& value)
{
  //## begin ClM4XMLContext::SetLastAccess%3AE0522E0124.set preserve=no
  m_uiLastAccess = value;
  //## end ClM4XMLContext::SetLastAccess%3AE0522E0124.set
}

inline m4uint32_t ClM4XMLContext::GetLastAuthenticationTime () const
{
  //## begin ClM4XMLContext::SetLastAccess%3AE0522E0124.set preserve=no
  return m_uLastAuthenticationTime;
  //## end ClM4XMLContext::SetLastAccess%3AE0522E0124.set
}

inline void ClM4XMLContext::SetLastAuthenticationTime(m4uint32_t ai_iLastAuthenticationTime)
{
  m_uLastAuthenticationTime = ai_iLastAuthenticationTime;
}

inline void ClM4XMLContext::SetAuthPassword (string &ai_sAuthPassword)
{
  //## begin ClM4XMLContext::SetAuthPassword%3AE0522E0124.set preserve=no
  m_sAuthPassword = ai_sAuthPassword;
  //## end ClM4XMLContext::SetAuthPassword%3AE0522E0124.set
}

inline const string & ClM4XMLContext::GetAuthPassword () const
{
  //## begin ClM4XMLContext::GetAuthPassword%3AE0522E0124.set preserve=no
  return m_sAuthPassword;
  //## end ClM4XMLContext::GetAuthPassword%3AE0522E0124.set
}

inline void ClM4XMLContext::SetEnableNulls (m4bool_t ai_bEnableNulls)
{
  m_bEnableNulls = ai_bEnableNulls;
}

inline const m4bool_t ClM4XMLContext::GetEnableNulls () const
{
  return m_bEnableNulls;
}

inline void ClM4XMLContext::SetEnableMethods (m4bool_t ai_bEnable)
{
  m_bEnableMethods = ai_bEnable;
}

inline const m4bool_t ClM4XMLContext::GetEnableMethods () const
{
  return m_bEnableMethods;
}

inline void ClM4XMLContext::SetEnableAuth (m4bool_t ai_bEnableAuth)
{
  m_bEnableAuth = ai_bEnableAuth;
}

inline const m4bool_t ClM4XMLContext::GetEnableAuth () const
{
  return m_bEnableAuth;
}

inline void ClM4XMLContext::SetDataMode (string &ai_sDataMode)
{
  m_sDataMode = ai_sDataMode;
}

inline const string& ClM4XMLContext::GetDataMode () const
{
  return m_sDataMode;
}

inline void ClM4XMLContext::SetHaveFunctionalGroup(m4bool_t ai_bHaveFunctionalGroup)
{
	m_bHaveFunctionalGroup = ai_bHaveFunctionalGroup;
}

inline const m4bool_t ClM4XMLContext::GetHaveFunctionalGroup()
{
	return m_bHaveFunctionalGroup;
}

inline void ClM4XMLContext::SetEnableBlobFiles(m4bool_t ai_bEnableBlobFiles)
{
	m_bEnableBlobFiles = ai_bEnableBlobFiles;
}

inline const m4bool_t ClM4XMLContext::GetEnableBlobFiles()
{
	return m_bEnableBlobFiles;
}

inline void ClM4XMLContext::SetReqNumber(m4int_t ai_iReqNum)
{
	m_iReqNumber = ai_iReqNum;
}

inline const m4int_t ClM4XMLContext::GetReqNumber()
{
	return m_iReqNumber;
}

// Class ClM4XMLItem 


//## Other Operations (inline)
inline const string & ClM4XMLItem::GetResult () const
{
  //## begin ClM4XMLItem::GetResult%937470761.body preserve=yes
	return m_sResult;
  //## end ClM4XMLItem::GetResult%937470761.body
}

//## Other Operations (inline)
inline void ClM4XMLItem::SetResult ( const string& ai_sResult )
{
  //## begin ClM4XMLItem::GetResult%937470761.body preserve=yes
	m_sResult = ai_sResult;
  //## end ClM4XMLItem::GetResult%937470761.body
}

inline m4int16_t ClM4XMLItem::GetResultType () const
{
  //## begin ClM4XMLItem::GetResultType%941188386.body preserve=yes
	return m_iResultType;
  //## end ClM4XMLItem::GetResultType%941188386.body
}

inline const m4bool_t ClM4XMLItem::GetResultIsNull () const
{
  //## begin ClM4XMLItem::GetResultIsNull%941188386.body preserve=yes
	return m_bResultIsNull;
  //## end ClM4XMLItem::GetResultIsNull%941188386.body
}

inline m4int16_t ClM4XMLItem::GetNArgs () const
{
  //## begin ClM4XMLItem::GetNArgs%937470763.body preserve=yes
	if (m_pcM4OItem != NULL)
		return 0;

	if (m_iHandle <= 0 || m_poAccess == NULL)
		return -1;

	return m_poAccess->GetpCMCR()->GetItemNumberOfArguments(m_iHandle);

  //## end ClM4XMLItem::GetNArgs%937470763.body
}

inline m4bool_t ClM4XMLItem::IsVariableArgs () const
{
  //## begin ClM4XMLItem::IsVariableArgs%963252651.body preserve=yes
	if (m_pcM4OItem != NULL)
		return M4_FALSE;

	if (m_iHandle <= 0 || m_poAccess == NULL)
		return M4_FALSE;

	return m_poAccess->GetpCMCR()->GetItemVariableArguments(m_iHandle) ? M4_TRUE : M4_FALSE;

  //## end ClM4XMLItem::IsVariableArgs%963252651.body
}

inline ClAccess * ClM4XMLItem::GetAccess () const
{
  //## begin ClM4XMLItem::GetAccess%937470764.body preserve=yes
	return m_poAccess;
  //## end ClM4XMLItem::GetAccess%937470764.body
}

inline m4pchar_t ClM4XMLItem::GetItem () const
{
  //## begin ClM4XMLItem::GetItem%937826947.body preserve=yes
	return m_pcItem;
  //## end ClM4XMLItem::GetItem%937826947.body
}

inline m4pchar_t ClM4XMLItem::GetM4OItem () const
{
  //## begin ClM4XMLItem::GetM4OItem%939630178.body preserve=yes
	return m_pcM4OItem;
  //## end ClM4XMLItem::GetM4OItem%939630178.body
}

inline m4pchar_t ClM4XMLItem::GetM4OAlias () const
{
  //## begin ClM4XMLItem::GetM4OAlias%937555630.body preserve=yes
	return m_pcM4OAlias;
  //## end ClM4XMLItem::GetM4OAlias%937555630.body
}

inline m4pchar_t ClM4XMLItem::GetNode () const
{
  //## begin ClM4XMLItem::GetNode%937555631.body preserve=yes
	return m_pcNode;
  //## end ClM4XMLItem::GetNode%937555631.body
}

inline m4pchar_t ClM4XMLItem::GetRegister () const
{
  //## begin ClM4XMLItem::GetRegister%937555632.body preserve=yes
	return m_pcRegister;
  //## end ClM4XMLItem::GetRegister%937555632.body
}

inline m4int32_t ClM4XMLItem::GetRegisterMin () const
{
  //## begin ClM4XMLItem::GetRegisterMin%937922617.body preserve=yes
	return m_iRegisterMin;
  //## end ClM4XMLItem::GetRegisterMin%937922617.body
}

inline m4int32_t ClM4XMLItem::GetRegisterMax () const
{
  //## begin ClM4XMLItem::GetRegisterMax%937922618.body preserve=yes
	return m_iRegisterMax;
  //## end ClM4XMLItem::GetRegisterMax%937922618.body
}

inline void ClM4XMLItem::SetFilterType (const string &ai_sType)
{
  //## begin ClM4XMLItem::SetFilterType%939117431.body preserve=yes
	m_sFilterType = ai_sType;
  //## end ClM4XMLItem::SetFilterType%939117431.body
}

inline void ClM4XMLItem::SetFilterFormula (const string &ai_sFormula)
{
  //## begin ClM4XMLItem::SetFilterFormula%939200067.body preserve=yes
	m_sFilterFormula = ai_sFormula;
  //## end ClM4XMLItem::SetFilterFormula%939200067.body
}

inline const string& ClM4XMLItem::GetFilterType () const
{
  //## begin ClM4XMLItem::GetFilterType%939200068.body preserve=yes
	return m_sFilterType;
  //## end ClM4XMLItem::GetFilterType%939200068.body
}

inline m4int32_t ClM4XMLItem::GetHandle () const
{
  //## begin ClM4XMLItem::GetHandle%939289646.body preserve=yes
	return m_iHandle;
  //## end ClM4XMLItem::GetHandle%939289646.body
}

inline m4bool_t ClM4XMLItem::IsRegRelative () const
{
  //## begin ClM4XMLItem::IsRegRelative%940581440.body preserve=yes
	return m_bRegRelative;
  //## end ClM4XMLItem::IsRegRelative%940581440.body
}

// Class ClM4XMLItemFilterList 

//## begin module%37DF53EC0192.epilog preserve=yes
//## end module%37DF53EC0192.epilog


#endif
