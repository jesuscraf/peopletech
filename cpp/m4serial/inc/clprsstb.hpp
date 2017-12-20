//## begin module%3911AA270240.cm preserve=no
//## end module%3911AA270240.cm

//## begin module%3911AA270240.cp preserve=no
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
//## end module%3911AA270240.cp

//## Module: clprsstb%3911AA270240; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\clprsstb.hpp

#ifndef clprsstb_h
#define clprsstb_h 1

//## begin module%3911AA270240.additionalIncludes preserve=no
//## end module%3911AA270240.additionalIncludes

//## begin module%3911AA270240.includes preserve=yes


#include "m4serial_dll.hpp"

#include "m4types.hpp"
#include "clsrlzbl.hpp"   // ClSerializableObject
#include "cldntfbl.hpp"   // ClIdentifiableObject


// ---- Class ClPersistId -------------------------------------------

//## end module%3911AA270240.includes

//## begin module%3911AA270240.declarations preserve=no
//## end module%3911AA270240.declarations

//## begin module%3911AA270240.additionalDeclarations preserve=yes
//## end module%3911AA270240.additionalDeclarations


//## begin ClPersistId%3911AA27024A.preface preserve=yes
//## end ClPersistId%3911AA27024A.preface

//## Class: ClPersistId%3911AA27024A
//	---- Class ClPersistId -------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClPersistId : public ClSerializableObject  //## Inherits: <unnamed>%3911AA270309
{
  //## begin ClPersistId%3911AA27024A.initialDeclarations preserve=yes
  //## end ClPersistId%3911AA27024A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPersistId%2141709434; C++
      //	---- Class ClPersistId -------------------------------------------
      ClPersistId (void );

      //## Operation: ClPersistId%-910944005; C++
      //	-- {AddDecl: 495} region.unprotectedFunction [835..1022]
      ClPersistId (const ClPersistId& ai_roPersistId);

      //## Operation: ClPersistId%1614162924; C++
      //	Destructor
      //	-- {AddDecl: 496} region.unprotectedFunction [1023..1162]
      ClPersistId (const m4pchar_t ai_string);

    //## Destructor (specified)
      //## Operation: ~ClPersistId%1142821737; C++
      //	Equality Operations (generated)
      //	-- {AddDecl: 497} region.unprotectedFunction [1163..1223]
      virtual ~ClPersistId (void );


    //## Other Operations (specified)
      //## Operation: operator ==%1194998261; C++
      //	-- {AddDecl: 498} region.unprotectedFunction [1224..1390]
      m4bool_t operator == (const ClPersistId& ai_roPersistId) const;

      //## Operation: GetPersistId%-1246418680; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 500} region.unprotectedFunction [1558..1634]
      m4pchar_t GetPersistId (void ) const;

      //## Operation: operator !=%784434613; C++
      //	Getters
      //	-- {AddDecl: 499} region.unprotectedFunction [1391..1557]
      m4bool_t operator != (const ClPersistId& ai_roPersistId) const;

      //## Operation: Serialize%731478525; C++
      //	-- {AddDecl: 501} region.unprotectedFunction [1635..1763]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%701944922; C++
      //	-- {AddDecl: 502} region.unprotectedFunction [1764..2176]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

    // Additional Public Declarations
      //## begin ClPersistId%3911AA27024A.public preserve=yes
      //## end ClPersistId%3911AA27024A.public

  protected:
    // Additional Protected Declarations
      //## begin ClPersistId%3911AA27024A.protected preserve=yes
      //## end ClPersistId%3911AA27024A.protected

  private:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA2702ED
      //## Role: ClPersistId::m_persistId%3911AA2702EF
      //## begin ClPersistId::m_persistId%3911AA2702EF.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_persistId;
      //## end ClPersistId::m_persistId%3911AA2702EF.role

    // Additional Private Declarations
      //## begin ClPersistId%3911AA27024A.private preserve=yes
      //## end ClPersistId%3911AA27024A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPersistId%3911AA27024A.implementation preserve=yes
      //## end ClPersistId%3911AA27024A.implementation

};

//## begin ClPersistId%3911AA27024A.postscript preserve=yes

// ---- Class ClPersistiableObject ----------------------------------

class ClPersistor;
//## end ClPersistId%3911AA27024A.postscript

//## begin ClPersistiableObject%3911AA27030A.preface preserve=yes
//## end ClPersistiableObject%3911AA27030A.preface

//## Class: ClPersistiableObject%3911AA27030A
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClPersistiableObject : public ClSerializableObject, //## Inherits: <unnamed>%3911AA27039F
    	public ClIdentifiableObject  //## Inherits: <unnamed>%3911AA2703A0
{
  //## begin ClPersistiableObject%3911AA27030A.initialDeclarations preserve=yes
public:

   /*private:
      // Data Members for Associations
      ClPersistId *m_poPersistId;*/


friend class ClPersistor;  // Para que le pueda poner el PersistId
  //## end ClPersistiableObject%3911AA27030A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPersistiableObject%-1172167449; C++
      //	-- {AddDecl: 503} region.unprotectedFunction [2332..2618]
      ClPersistiableObject (const ClPersistiableObject& ai_roPersistiableObject);

      //## Operation: ClPersistiableObject%1316295238; C++
      //	-- {AddDecl: 504} region.unprotectedFunction [2619..2800]
      ClPersistiableObject (const ClIdentifiableObject& ai_roIdentifiableObject);

      //## Operation: ClPersistiableObject%1869755369; C++
      //	ClPersistiableObject(ClObjectId  *ai_poObjectId,
      //	ClPersistId *ai_poPersistId=0);
      //	Destructor
      //	-- {AddDecl: 505} region.unprotectedFunction [2801..2913]
      ClPersistiableObject (ClObjectId* ai_poObjectId);

    //## Destructor (specified)
      //## Operation: ~ClPersistiableObject%-704253074; C++
      //	// Getter: return a string
      //	m4pchar_t GetPersistId(void) const;
      //
      //	// Getter: return an object, so you can do any cuestion to it
      //	ClPersistId *GetObjPersistId(void) const;
      //
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 506} region.unprotectedFunction [3184..3263]
      ~ClPersistiableObject (void );


    //## Other Operations (specified)
      //## Operation: Serialize%-1317523330; C++
      //	-- {AddDecl: 507} region.unprotectedFunction [3518..4111]
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%-1437971505; C++
      //	-- {AddDecl: 53} module.vulnerableDeclarations [2869..3028]
      //	-- {AddDecl: 508} region.unprotectedFunction [4112..5382]
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

    // Additional Public Declarations
      //## begin ClPersistiableObject%3911AA27030A.public preserve=yes
      //## end ClPersistiableObject%3911AA27030A.public

  protected:
    // Additional Protected Declarations
      //## begin ClPersistiableObject%3911AA27030A.protected preserve=yes
      //## end ClPersistiableObject%3911AA27030A.protected

  private:
    // Additional Private Declarations
      //## begin ClPersistiableObject%3911AA27030A.private preserve=yes
      //## end ClPersistiableObject%3911AA27030A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPersistiableObject%3911AA27030A.implementation preserve=yes
      //## end ClPersistiableObject%3911AA27030A.implementation

};

//## begin ClPersistiableObject%3911AA27030A.postscript preserve=yes
//## end ClPersistiableObject%3911AA27030A.postscript

//## begin ClSelfPersistiableObject%3911AA2703A1.preface preserve=yes
//## end ClSelfPersistiableObject%3911AA2703A1.preface

//## Class: ClSelfPersistiableObject%3911AA2703A1; Abstract
//	-- {AddDecl: 54} class [3031..3743]
//	---- Class ClSelfPersistiableObject
//	-------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClSelfPersistiableObject : public ClPersistiableObject  //## Inherits: <unnamed>%3911AA280069
{
  //## begin ClSelfPersistiableObject%3911AA2703A1.initialDeclarations preserve=yes
  //## end ClSelfPersistiableObject%3911AA2703A1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSelfPersistiableObject%2010852052; C++
      //	-- {AddDecl: 509} region.unprotectedFunction [5383..5540]
      //	SelfPersistiable
      ClSelfPersistiableObject (void );

      //## Operation: ClSelfPersistiableObject%1838470577; C++
      //	-- {AddDecl: 510} region.unprotectedFunction [5541..5733]
      ClSelfPersistiableObject (const ClPersistiableObject& ai_roPersistiableObject);

      //## Operation: ClSelfPersistiableObject%1299958730; C++
      //	-- {AddDecl: 511} region.unprotectedFunction [5734..5893]
      ClSelfPersistiableObject (const ClIdentifiableObject& ai_roIdentifiableObject);

      //## Operation: ClSelfPersistiableObject%2110413596; C++
      //	Destructor
      //	-- {AddDecl: 512} region.unprotectedFunction [5894..6014]
      ClSelfPersistiableObject (ClObjectId* ai_poObjectId);

    //## Destructor (specified)
      //## Operation: ~ClSelfPersistiableObject%1975116357; C++
      //	Redefinición de los métodos de serialización
      //	-- {AddDecl: 513} region.unprotectedFunction [6015..6077]
      ~ClSelfPersistiableObject (void );


    //## Other Operations (specified)
      //## Operation: Persist%-1049810124; C++
      virtual m4return_t Persist (void ) = 0;

      //## Operation: DePersist%-1952387385; C++
      virtual m4return_t DePersist (void ) = 0;

      //## Operation: Delete%621724022; C++
      virtual m4return_t Delete (void ) = 0;

    // Additional Public Declarations
      //## begin ClSelfPersistiableObject%3911AA2703A1.public preserve=yes
      //## end ClSelfPersistiableObject%3911AA2703A1.public

  protected:
    // Additional Protected Declarations
      //## begin ClSelfPersistiableObject%3911AA2703A1.protected preserve=yes
      //## end ClSelfPersistiableObject%3911AA2703A1.protected

  private:
    // Additional Private Declarations
      //## begin ClSelfPersistiableObject%3911AA2703A1.private preserve=yes
      //## end ClSelfPersistiableObject%3911AA2703A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSelfPersistiableObject%3911AA2703A1.implementation preserve=yes
      //## end ClSelfPersistiableObject%3911AA2703A1.implementation

};

//## begin ClSelfPersistiableObject%3911AA2703A1.postscript preserve=yes
//## end ClSelfPersistiableObject%3911AA2703A1.postscript

//## begin ClPersistor%3911AA28006A.preface preserve=yes
//## end ClPersistor%3911AA28006A.preface

//## Class: ClPersistor%3911AA28006A; Abstract
//	-- {AddDecl: 56} class [3744..4161]
//	---- Class ClPersistor -------------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClPersistor 
{
  //## begin ClPersistor%3911AA28006A.initialDeclarations preserve=yes
  //## end ClPersistor%3911AA28006A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Persist%-1874789; C++
      virtual m4return_t Persist (ClPersistiableObject& ai_persistiableObj) = 0;

      //## Operation: DePersist%-1608297540; C++
      virtual m4return_t DePersist (ClPersistiableObject& ai_persistiableObj) = 0;

      //## Operation: Delete%-481422387; C++
      virtual m4return_t Delete (ClPersistiableObject& ai_persistiableObj) = 0;

    // Additional Public Declarations
      //## begin ClPersistor%3911AA28006A.public preserve=yes
      //## end ClPersistor%3911AA28006A.public

  protected:
    // Additional Protected Declarations
      //## begin ClPersistor%3911AA28006A.protected preserve=yes
      //## end ClPersistor%3911AA28006A.protected

  private:
    // Additional Private Declarations
      //## begin ClPersistor%3911AA28006A.private preserve=yes
      //## end ClPersistor%3911AA28006A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPersistor%3911AA28006A.implementation preserve=yes
      //## end ClPersistor%3911AA28006A.implementation

};

//## begin ClPersistor%3911AA28006A.postscript preserve=yes
//## end ClPersistor%3911AA28006A.postscript

// Class ClPersistId 

// Class ClPersistiableObject 

// Class ClSelfPersistiableObject 

// Class ClPersistor 

//## begin module%3911AA270240.epilog preserve=yes
//## end module%3911AA270240.epilog


#endif
