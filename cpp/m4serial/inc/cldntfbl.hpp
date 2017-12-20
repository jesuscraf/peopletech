//## begin module%3911AA240278.cm preserve=no
//## end module%3911AA240278.cm

//## begin module%3911AA240278.cp preserve=no
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
//## end module%3911AA240278.cp

//## Module: cldntfbl%3911AA240278; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\cldntfbl.hpp

#ifndef cldntfbl_h
#define cldntfbl_h 1

//## begin module%3911AA240278.additionalIncludes preserve=no
//## end module%3911AA240278.additionalIncludes

//## begin module%3911AA240278.includes preserve=yes

#include "m4types.hpp"
#include "m4stl.hpp"
#include "clsrlzbl.hpp"    // Todo ID debe ser serializable, sino no puedo enviarlo por la red

//## end module%3911AA240278.includes

//## begin module%3911AA240278.declarations preserve=no
//## end module%3911AA240278.declarations

//## begin module%3911AA240278.additionalDeclarations preserve=yes

class ClEnvironment {};    // ??? Solo para que compile

//## end module%3911AA240278.additionalDeclarations


//## begin m4ObjectType_t%3911AA240387.preface preserve=yes
//## end m4ObjectType_t%3911AA240387.preface

//## Class: m4ObjectType_t%3911AA240387
//	  ATENCION
//	   Actualmente, en el Directory se están utilizando los siguientes
//	identificadores de objetos:
//	   CHANNEL
//	      ObjectType   =ChannelObjectType
//	      EnvironmentId=NULL
//	      TypedID      =nombre del Channel, ej "TI_CHANNEL"
//	      UnikeKey     =NULL
//
//	   ACCESS
//	      ObjectType   =AccessObjectType
//	      EnvironmentId=NULL
//	      TypedID      =nombre del Channel, ej "TI_CHANNEL"
//	      UnikeKey     =indice del Access dentro del Channel, ej "3"
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum eObjectType {UndefinedObjectType, RequestObjectType, ResponseObjectType, EnvironmentObjectType, ProxyspaceObjectType, MetaChannelObjectType, ChannelObjectType, ChannelDataObjectType, ObjDataObjectType, AccessObjectType, NodeObjectType, BlockObjectType, OLTPExecutionContextType, ClientServerResultObjectType, CacheObjectType, MetaSecurityObjectType, PresentationObjectType, CacheDirectoryObjectType, ExecutorStateObjectType, XMLFileObjectType, CompiledXMLObjectType, ServiceDLLObjectType, ActiveServiceConnectionObjectType, InactiveServiceConnectionObjectType, MapObjectType, LastObjectType} m4ObjectType_t;

//## begin m4ObjectType_t%3911AA240387.postscript preserve=yes
//## end m4ObjectType_t%3911AA240387.postscript

//## begin ClObjectType%3911AA240282.preface preserve=yes
//## end ClObjectType%3911AA240282.preface

//## Class: ClObjectType%3911AA240282
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClObjectType : public ClSerializableObject  //## Inherits: <unnamed>%3911AA240392
{
  //## begin ClObjectType%3911AA240282.initialDeclarations preserve=yes
  //## end ClObjectType%3911AA240282.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClObjectType%-1955862339; C++
      //	---- Class ClObjectType ------------------------------------------
      ClObjectType (void );

      //## Operation: ClObjectType%-1754057246; C++
      //	-- {AddDecl: 186} region.unprotectedFunction [827..933]
      ClObjectType (const ClObjectType& ai_roObjectType);

      //## Operation: ClObjectType%356133023; C++
      //	Equality Operations
      //	-- {AddDecl: 187} region.unprotectedFunction [934..1021]
      ClObjectType (enum eObjectType ai_eObjectType);


    //## Other Operations (specified)
      //## Operation: operator ==%1425800807; C++
      //	-- {AddDecl: 188} region.unprotectedFunction [1022..1171]
      m4bool_t operator == (const ClObjectType& ai_roObjectType) const;

      //## Operation: SetType%-1398515932; C++
      //	Getters
      //	-- {AddDecl: 190} region.unprotectedFunction [1322..1408]
      void SetType (enum eObjectType ai_eObjectType);

      //## Operation: operator !=%-893442710; C++
      //	Setters
      //	-- {AddDecl: 189} region.unprotectedFunction [1172..1321]
      m4bool_t operator != (const ClObjectType& ai_roObjectType) const;

      //## Operation: GetType%1042897194; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 191} region.unprotectedFunction [1409..1478]
      enum eObjectType GetType (void ) const;

      //## Operation: Serialize%1051096846; C++
      //	-- {AddDecl: 192} region.unprotectedFunction [1479..1593]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%-1996810521; C++
      //	-- {AddDecl: 193} region.unprotectedFunction [1594..1804]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DumpInfo%-23255759; C++
      //	-- {AddDecl: 194} region.unprotectedFunction [1952..3151]
      m4return_t DumpInfo (ostream& ai_oStream);

    // Additional Public Declarations
      //## begin ClObjectType%3911AA240282.public preserve=yes
      //## end ClObjectType%3911AA240282.public

  protected:
    // Additional Protected Declarations
      //## begin ClObjectType%3911AA240282.protected preserve=yes
      //## end ClObjectType%3911AA240282.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA240388
      //## Role: ClObjectType::m_type%3911AA24038A
      //## begin ClObjectType::m_type%3911AA24038A.role preserve=no  private: m4ObjectType_t {1 -> 1VHAPN}
      m4ObjectType_t m_type;
      //## end ClObjectType::m_type%3911AA24038A.role

    // Additional Private Declarations
      //## begin ClObjectType%3911AA240282.private preserve=yes
      //## end ClObjectType%3911AA240282.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjectType%3911AA240282.implementation preserve=yes
      //## end ClObjectType%3911AA240282.implementation

};

//## begin ClObjectType%3911AA240282.postscript preserve=yes

M4_DECL_M4SERIAL ostream & operator << (ostream &ao_Output, ClObjectType &ai_oClObjectType);

// ---- Class ClEnvironmentId ---------------------------------------

//## end ClObjectType%3911AA240282.postscript

//## begin ClEnvironmentId%3911AA240393.preface preserve=yes
//## end ClEnvironmentId%3911AA240393.preface

//## Class: ClEnvironmentId%3911AA240393
//	---- Class ClEnvironmentId ---------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClEnvironmentId : public ClSerializableObject  //## Inherits: <unnamed>%3911AA250061
{
  //## begin ClEnvironmentId%3911AA240393.initialDeclarations preserve=yes
  //## end ClEnvironmentId%3911AA240393.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEnvironmentId%-1648070787; C++
      //	---- Class ClEnvironmentId ---------------------------------------
      ClEnvironmentId (void );

      //## Operation: ClEnvironmentId%1914995882; C++
      //	-- {AddDecl: 197} region.unprotectedFunction [3527..3710]
      ClEnvironmentId (const ClEnvironmentId& ai_roEnvironmentId);

      //## Operation: ClEnvironmentId%-1503812114; C++
      //	-- {AddDecl: 198} region.unprotectedFunction [3711..3911]
      ClEnvironmentId (const ClEnvironment& ai_roEnvironment);

      //## Operation: ClEnvironmentId%946509623; C++
      //	Destructor
      //	-- {AddDecl: 199} region.unprotectedFunction [3912..4040]
      ClEnvironmentId (m4pcchar_t ai_string);

    //## Destructor (specified)
      //## Operation: ~ClEnvironmentId%-357777085; C++
      //	Equality Operations
      //	-- {AddDecl: 200} region.unprotectedFunction [4041..4098]
      virtual ~ClEnvironmentId (void );


    //## Other Operations (specified)
      //## Operation: operator ==%1014829916; C++
      //	-- {AddDecl: 201} region.unprotectedFunction [4099..4267]
      m4bool_t operator == (const ClEnvironmentId& ai_roEnvironmentId) const;

      //## Operation: operator !=%810395447; C++
      //	Getters
      //	-- {AddDecl: 202} region.unprotectedFunction [4268..4436]
      m4bool_t operator != (const ClEnvironmentId& ai_roEnvironmentId) const;

      //## Operation: Serialize%1325898263; C++
      //	-- {AddDecl: 204} region.unprotectedFunction [4500..4618]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%-1445095051; C++
      //	-- {AddDecl: 205} region.unprotectedFunction [4619..5014]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

      //## Operation: GetId%1562027569; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 203} region.unprotectedFunction [4437..4499]
      m4pchar_t GetId (void ) const;

    // Additional Public Declarations
      //## begin ClEnvironmentId%3911AA240393.public preserve=yes
      //## end ClEnvironmentId%3911AA240393.public

  protected:
    // Additional Protected Declarations
      //## begin ClEnvironmentId%3911AA240393.protected preserve=yes
      //## end ClEnvironmentId%3911AA240393.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA25005E
      //## Role: ClEnvironmentId::m_id%3911AA250060
      //## begin ClEnvironmentId::m_id%3911AA250060.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_id;
      //## end ClEnvironmentId::m_id%3911AA250060.role

    // Additional Private Declarations
      //## begin ClEnvironmentId%3911AA240393.private preserve=yes
      //## end ClEnvironmentId%3911AA240393.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEnvironmentId%3911AA240393.implementation preserve=yes
      //## end ClEnvironmentId%3911AA240393.implementation

};

//## begin ClEnvironmentId%3911AA240393.postscript preserve=yes
//## end ClEnvironmentId%3911AA240393.postscript

//## begin ClTypedId%3911AA250062.preface preserve=yes
//## end ClTypedId%3911AA250062.preface

//## Class: ClTypedId%3911AA250062
//	-- {AddDecl: 34} class [3942..4760]
//	---- Class ClTypedId ---------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClTypedId : public ClSerializableObject  //## Inherits: <unnamed>%3911AA2500E6
{
  //## begin ClTypedId%3911AA250062.initialDeclarations preserve=yes
  //## end ClTypedId%3911AA250062.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTypedId%-930413434; C++
      //	-- {AddDecl: 206} region.unprotectedFunction [5015..5131]
      //	---- Class ClTypedId ---------------------------------------------
      ClTypedId (void );

      //## Operation: ClTypedId%1582595709; C++
      //	-- {AddDecl: 207} region.unprotectedFunction [5132..5279]
      ClTypedId (const ClTypedId& ai_roTypedId);

      //## Operation: ClTypedId%-701555389; C++
      //	Destructor
      //	-- {AddDecl: 208} region.unprotectedFunction [5280..5396]
      ClTypedId (m4pcchar_t ai_string);

    //## Destructor (specified)
      //## Operation: ~ClTypedId%507446288; C++
      //	Equality Operations
      //	-- {AddDecl: 209} region.unprotectedFunction [5397..5442]
      virtual ~ClTypedId (void );


    //## Other Operations (specified)
      //## Operation: operator ==%667498224; C++
      //	-- {AddDecl: 210} region.unprotectedFunction [5443..5587]
      m4bool_t operator == (const ClTypedId& ai_roTypedId) const;

      //## Operation: operator !=%370000431; C++
      //	Getters
      //	-- {AddDecl: 211} region.unprotectedFunction [5588..5732]
      m4bool_t operator != (const ClTypedId& ai_roTypedId) const;

      //## Operation: Serialize%-1555423904; C++
      //	-- {AddDecl: 213} region.unprotectedFunction [5790..5902]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%-1505914124; C++
      //	-- {AddDecl: 214} region.unprotectedFunction [5903..6292]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

      //## Operation: GetId%-90383977; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 212} region.unprotectedFunction [5733..5789]
      m4pchar_t GetId (void ) const;

    // Additional Public Declarations
      //## begin ClTypedId%3911AA250062.public preserve=yes
      //## end ClTypedId%3911AA250062.public

  protected:
    // Additional Protected Declarations
      //## begin ClTypedId%3911AA250062.protected preserve=yes
      //## end ClTypedId%3911AA250062.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA2500E3
      //## Role: ClTypedId::m_id%3911AA2500E5
      //## begin ClTypedId::m_id%3911AA2500E5.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_id;
      //## end ClTypedId::m_id%3911AA2500E5.role

    // Additional Private Declarations
      //## begin ClTypedId%3911AA250062.private preserve=yes
      //## end ClTypedId%3911AA250062.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTypedId%3911AA250062.implementation preserve=yes
      //## end ClTypedId%3911AA250062.implementation

};

//## begin ClTypedId%3911AA250062.postscript preserve=yes
//## end ClTypedId%3911AA250062.postscript

//## begin ClUniqueKey%3911AA2500E7.preface preserve=yes
//## end ClUniqueKey%3911AA2500E7.preface

//## Class: ClUniqueKey%3911AA2500E7
//	-- {AddDecl: 36} class [4761..5586]
//	---- Class ClUniqueKey -------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClUniqueKey : public ClSerializableObject  //## Inherits: <unnamed>%3911AA250168
{
  //## begin ClUniqueKey%3911AA2500E7.initialDeclarations preserve=yes
  //## end ClUniqueKey%3911AA2500E7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUniqueKey%780596879; C++
      //	-- {AddDecl: 215} region.unprotectedFunction [6293..6414]
      //	---- Class ClUniqueKey -------------------------------------------
      ClUniqueKey (void );

      //## Operation: ClUniqueKey%1449227943; C++
      //	-- {AddDecl: 216} region.unprotectedFunction [6415..6578]
      ClUniqueKey (const ClUniqueKey& ai_roUniqueKey);

      //## Operation: ClUniqueKey%1888177138; C++
      //	Destructor
      //	-- {AddDecl: 217} region.unprotectedFunction [6579..6701]
      ClUniqueKey (m4pcchar_t ai_string);

    //## Destructor (specified)
      //## Operation: ~ClUniqueKey%-777551699; C++
      //	Equality Operations
      //	-- {AddDecl: 218} region.unprotectedFunction [6702..6759]
      virtual ~ClUniqueKey (void );


    //## Other Operations (specified)
      //## Operation: operator ==%402743725; C++
      //	-- {AddDecl: 219} region.unprotectedFunction [6760..6914]
      m4bool_t operator == (const ClUniqueKey& ai_roUniqueKey) const;

      //## Operation: operator !=%1915945393; C++
      //	Getters
      //	-- {AddDecl: 220} region.unprotectedFunction [6915..7069]
      m4bool_t operator != (const ClUniqueKey& ai_roUniqueKey) const;

      //## Operation: Serialize%-675907019; C++
      //	-- {AddDecl: 222} region.unprotectedFunction [7135..7251]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%1437480156; C++
      //	-- {AddDecl: 223} region.unprotectedFunction [7252..7646]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

      //## Operation: GetKey%10258674; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 221} region.unprotectedFunction [7070..7134]
      m4pchar_t GetKey (void ) const;

    // Additional Public Declarations
      //## begin ClUniqueKey%3911AA2500E7.public preserve=yes
      //## end ClUniqueKey%3911AA2500E7.public

  protected:
    // Additional Protected Declarations
      //## begin ClUniqueKey%3911AA2500E7.protected preserve=yes
      //## end ClUniqueKey%3911AA2500E7.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA250165
      //## Role: ClUniqueKey::m_key%3911AA250167
      //## begin ClUniqueKey::m_key%3911AA250167.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_key;
      //## end ClUniqueKey::m_key%3911AA250167.role

    // Additional Private Declarations
      //## begin ClUniqueKey%3911AA2500E7.private preserve=yes
      //## end ClUniqueKey%3911AA2500E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUniqueKey%3911AA2500E7.implementation preserve=yes
      //## end ClUniqueKey%3911AA2500E7.implementation

};

//## begin ClUniqueKey%3911AA2500E7.postscript preserve=yes
//## end ClUniqueKey%3911AA2500E7.postscript

//## begin ClObjectId%3911AA250169.preface preserve=yes
//## end ClObjectId%3911AA250169.preface

//## Class: ClObjectId%3911AA250169
//	-- {AddDecl: 38} class [5587..7560]
//	---- Class ClObjectId --------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClObjectId : public ClSerializableObject  //## Inherits: <unnamed>%3911AA25022D
{
  //## begin ClObjectId%3911AA250169.initialDeclarations preserve=yes
  //## end ClObjectId%3911AA250169.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClObjectId%-1970467006; C++
      //	-- {AddDecl: 224} region.unprotectedFunction [7647..7905]
      //	---- Class ClObjectId --------------------------------------------
      ClObjectId (void );

      //## Operation: ClObjectId%1581107277; C++
      //	-- {AddDecl: 225} region.unprotectedFunction [7906..8545]
      ClObjectId (const ClObjectId& ai_roObjectId);

      //## Operation: ClObjectId%-543994594; C++
      //	-- {AddDecl: 226} region.unprotectedFunction [8546..9363]
      ClObjectId (enum eObjectType ai_eObjectType, m4pcchar_t ai_pcEnvironmentId, m4pcchar_t ai_pcTypedId, m4pcchar_t ai_pcUniqueKey);

      //## Operation: ClObjectId%-996875636; C++
      //	??? ClObjectId(eObjectType            ai_eObjectType,
      //	???            m4pcchar_t        ai_string);
      //	Destructor
      //	-- {AddDecl: 227} region.unprotectedFunction [9364..10337]
      ClObjectId (enum eObjectType ai_eObjectType, const ClEnvironmentId* ai_poEnvironmentId, const ClTypedId* ai_poTypedId, const ClUniqueKey* ai_poUniqueKey);

    //## Destructor (specified)
      //## Operation: ~ClObjectId%446757455; C++
      //	Equality Operations
      //	-- {AddDecl: 229} region.unprotectedFunction [11347..11462]
      ~ClObjectId (void );


    //## Other Operations (specified)
      //## Operation: operator ==%-1789238716; C++
      //	-- {AddDecl: 230} region.unprotectedFunction [11463..11753]
      m4bool_t operator == (const ClObjectId& ai_roObjectId) const;

      //## Operation: operator !=%296438034; C++
      //	Getters: return a string
      //	Devuelve una concatenación de todos los "subidentificadores",
      //	-- {AddDecl: 231} region.unprotectedFunction [11754..12044]
      m4bool_t operator != (const ClObjectId& ai_roObjectId) const;

      //## Operation: GetType%-2044762307; C++
      //	-- {AddDecl: 233} region.unprotectedFunction [12103..12183]
      enum eObjectType GetType (void ) const;

      //## Operation: Serialize%-1353828062; C++
      //	-- {AddDecl: 237} region.unprotectedFunction [12404..13020]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%1664292358; C++
      //	-- {AddDecl: 238} region.unprotectedFunction [13021..14409]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DumpInfo%201084953; C++
      //	-- {AddDecl: 239} region.unprotectedFunction [14410..15067]
      m4return_t DumpInfo (ostream& ai_oStream);

      //## Operation: GetId%505784152; C++
      //	-- {AddDecl: 232} region.unprotectedFunction [12045..12102]
      m4pchar_t GetId (void ) const;

      //## Operation: GetEnvironmentId%1775905917; C++
      //	-- {AddDecl: 234} region.unprotectedFunction [12184..12263]
      m4pchar_t GetEnvironmentId (void ) const;

      //## Operation: GetTypedId%-1128067222; C++
      //	-- {AddDecl: 235} region.unprotectedFunction [12264..12331]
      m4pchar_t GetTypedId (void ) const;

      //## Operation: GetUniqueKey%1429022585; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 236} region.unprotectedFunction [12332..12403]
      m4pchar_t GetUniqueKey (void ) const;

    // Additional Public Declarations
      //## begin ClObjectId%3911AA250169.public preserve=yes
      //## end ClObjectId%3911AA250169.public

  protected:
    // Additional Protected Declarations
      //## begin ClObjectId%3911AA250169.protected preserve=yes
      //## end ClObjectId%3911AA250169.protected

  private:

    //## Other Operations (specified)
      //## Operation: _oidcat%1074933201; C++
      //	Data Members for Class Attributes
      //	-- {AddDecl: 228} region.unprotectedFunction [10338..11054]
      m4pchar_t _oidcat (void );

    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA25022A
      //## Role: ClObjectId::m_oObjectType%3911AA25022C
      //## begin ClObjectId::m_oObjectType%3911AA25022C.role preserve=no  private: ClObjectType {1 -> 1VHAPN}
      ClObjectType m_oObjectType;
      //## end ClObjectId::m_oObjectType%3911AA25022C.role

      //## Association: M4Serial::<unnamed>%3911AA250216
      //## Role: ClObjectId::m_id%3911AA250218
      //	Es una concatenación de los 3 siguientes
      //## begin ClObjectId::m_id%3911AA250218.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_id;
      //## end ClObjectId::m_id%3911AA250218.role

      //## Association: M4Serial::<unnamed>%3911AA250221
      //## Role: ClObjectId::m_environmentId%3911AA250223
      //## begin ClObjectId::m_environmentId%3911AA250223.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_environmentId;
      //## end ClObjectId::m_environmentId%3911AA250223.role

      //## Association: M4Serial::<unnamed>%3911AA250224
      //## Role: ClObjectId::m_typedId%3911AA250226
      //## begin ClObjectId::m_typedId%3911AA250226.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_typedId;
      //## end ClObjectId::m_typedId%3911AA250226.role

      //## Association: M4Serial::<unnamed>%3911AA250227
      //## Role: ClObjectId::m_uniqueKey%3911AA250229
      //	Data Members for Associations
      //## begin ClObjectId::m_uniqueKey%3911AA250229.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_uniqueKey;
      //## end ClObjectId::m_uniqueKey%3911AA250229.role

    // Additional Private Declarations
      //## begin ClObjectId%3911AA250169.private preserve=yes
      //## end ClObjectId%3911AA250169.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjectId%3911AA250169.implementation preserve=yes
      //## end ClObjectId%3911AA250169.implementation

};

//## begin ClObjectId%3911AA250169.postscript preserve=yes

M4_DECL_M4SERIAL ostream & operator << (ostream &ao_Output, ClObjectId &ai_oClObjectId);

// ---- Class ClIdentifiableObject ----------------------------------

//## end ClObjectId%3911AA250169.postscript

//## begin ClIdentifiableObject%3911AA25022E.preface preserve=yes
//## end ClIdentifiableObject%3911AA25022E.preface

//## Class: ClIdentifiableObject%3911AA25022E
//	---- Class ClIdentifiableObject ----------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClIdentifiableObject 
{
  //## begin ClIdentifiableObject%3911AA25022E.initialDeclarations preserve=yes
  //## end ClIdentifiableObject%3911AA25022E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClIdentifiableObject%-695804480; C++
      //	---- Class ClIdentifiableObject ----------------------------------
      //
      //	ClIdentifiableObject::ClIdentifiableObject():
      //	m_poObjectId  (0)
      //	{
      //	}
      ClIdentifiableObject (const ClIdentifiableObject& ai_roIdentifiableObject);

      //## Operation: ClIdentifiableObject%1313486248; C++
      //	Destructor
      //	-- {AddDecl: 242} region.unprotectedFunction [15630..15739]
      ClIdentifiableObject (ClObjectId* ai_poObjectId);

    //## Destructor (specified)
      //## Operation: ~ClIdentifiableObject%-21800869; C++
      //	Getters: return a string
      //	-- {AddDecl: 243} region.unprotectedFunction [15740..15833]
      ~ClIdentifiableObject (void );


    //## Other Operations (specified)
      //## Operation: GetId%-2008249606; C++
      //	Getters: return an object itself, so you can do any question to it
      //	-- {AddDecl: 244} region.unprotectedFunction [15834..15967]
      m4pchar_t GetId (void ) const;

      //## Operation: GetObjId%-1583760229; C++
      //	-- {AddDecl: 245} region.unprotectedFunction [15968..16052]
      ClObjectId* GetObjId (void ) const;

      //## Operation: SetObjId%949787626; C++
      //	-- {AddDecl: 246} region.unprotectedFunction [16053..16201]
      void SetObjId (ClObjectId* ai_poObjectId);

    // Additional Public Declarations
      //## begin ClIdentifiableObject%3911AA25022E.public preserve=yes
      //## end ClIdentifiableObject%3911AA25022E.public

  protected:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA250274
      //## Role: ClIdentifiableObject::m_poObjectId%3911AA250276
      //## begin ClIdentifiableObject::m_poObjectId%3911AA250276.role preserve=no  protected: ClObjectId {1 -> 0..1RHAPN}
      ClObjectId *m_poObjectId;
      //## end ClIdentifiableObject::m_poObjectId%3911AA250276.role

    // Additional Protected Declarations
      //## begin ClIdentifiableObject%3911AA25022E.protected preserve=yes
      //## end ClIdentifiableObject%3911AA25022E.protected

  private:
    // Additional Private Declarations
      //## begin ClIdentifiableObject%3911AA25022E.private preserve=yes
      //## end ClIdentifiableObject%3911AA25022E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClIdentifiableObject%3911AA25022E.implementation preserve=yes
      //## end ClIdentifiableObject%3911AA25022E.implementation

};

//## begin ClIdentifiableObject%3911AA25022E.postscript preserve=yes
//## end ClIdentifiableObject%3911AA25022E.postscript

// Class ClObjectType 

// Class ClEnvironmentId 

// Class ClTypedId 

// Class ClUniqueKey 

// Class ClObjectId 

// Class ClIdentifiableObject 

//## begin module%3911AA240278.epilog preserve=yes
//## end module%3911AA240278.epilog


#endif
