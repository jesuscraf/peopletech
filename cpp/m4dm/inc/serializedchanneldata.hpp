//## begin module%41F4D72800AF.cm preserve=no
//## end module%41F4D72800AF.cm

//## begin module%41F4D72800AF.cp preserve=no
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
//## end module%41F4D72800AF.cp

//## Module: serializedchanneldata%41F4D72800AF; Package specification
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Source file: Z:\m4dm\inc\serializedchanneldata.hpp

#ifndef serializedchanneldata_h
#define serializedchanneldata_h 1

//## begin module%41F4D72800AF.additionalIncludes preserve=no
//## end module%41F4D72800AF.additionalIncludes

//## begin module%41F4D72800AF.includes preserve=yes
#include "datedco.hpp"      // ClDatedContainer
class ClChannel_Data;
class M4DataStorage;
class ClCompiledMCR;

#include <diskpers.hpp>
//## end module%41F4D72800AF.includes

// clcofac
#include <clcofac.hpp>
// clprsstb
#include <clprsstb.hpp>
//## begin module%41F4D72800AF.declarations preserve=no
//## end module%41F4D72800AF.declarations

//## begin module%41F4D72800AF.additionalDeclarations preserve=yes
//## end module%41F4D72800AF.additionalDeclarations


//## begin ClSerializedChannelData%41F4D5220268.preface preserve=yes
//## end ClSerializedChannelData%41F4D5220268.preface

//## Class: ClSerializedChannelData%41F4D5220268
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSerializedChannelData : public ClPersistiableObject  //## Inherits: <unnamed>%41F4ECAD031C
{
  //## begin ClSerializedChannelData%41F4D5220268.initialDeclarations preserve=yes
  //## end ClSerializedChannelData%41F4D5220268.initialDeclarations

  public:
    //## Constructors (generated)
      ClSerializedChannelData();

    //## Destructor (generated)
      virtual ~ClSerializedChannelData();


    //## Other Operations (specified)
      //## Operation: GetVersion%1106565729
      m4date_t  GetVersion () const;

      //## Operation: SetCO%1106565730
      void  SetCO (ClCacheableObject* ai_pCO);

      //## Operation: GetCO%1106565731
      ClCacheableObject* GetCO () const;

      //## Operation: GetChannelData%1106565732
      m4return_t GetChannelData (ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData);

      //## Operation: GetStartDate%1106565733
      m4date_t GetStartDate ();

      //## Operation: GetEndDate%1106565734
      m4date_t GetEndDate ();

      //## Operation: GetCorStartDate%1106565735
      m4date_t GetCorStartDate ();

      //## Operation: GetCorEndDate%1106565736
      m4date_t GetCorEndDate ();

      //## Operation: GetSize%1106565737
      m4uint32_t GetSize ();

      //## Operation: PutChannelData%1106565741
      m4return_t PutChannelData (ClChannel_Data* ai_poChannelData);

      //## Operation: Serialize%1106652454; C++
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD);

      //## Operation: DeSerialize%1106652455; C++
      //	virtual m4uint16_t GetSerializeVersion(void)=0;
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD);

    // Additional Public Declarations
      //## begin ClSerializedChannelData%41F4D5220268.public preserve=yes
      //## end ClSerializedChannelData%41F4D5220268.public

  protected:
    // Additional Protected Declarations
      //## begin ClSerializedChannelData%41F4D5220268.protected preserve=yes
      //## end ClSerializedChannelData%41F4D5220268.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pCO%41F4EDDB0387
      //## begin ClSerializedChannelData::m_pCO%41F4EDDB0387.attr preserve=no  private: ClCacheableObject* {UA} NULL
      ClCacheableObject* m_pCO;
      //## end ClSerializedChannelData::m_pCO%41F4EDDB0387.attr

      //## Attribute: m_poDataStorage%41F7C2C3015B
      //## begin ClSerializedChannelData::m_poDataStorage%41F7C2C3015B.attr preserve=no  private: M4DataStorage* {UA} NULL
      M4DataStorage* m_poDataStorage;
      //## end ClSerializedChannelData::m_poDataStorage%41F7C2C3015B.attr

    // Additional Private Declarations
      //## begin ClSerializedChannelData%41F4D5220268.private preserve=yes
      //## end ClSerializedChannelData%41F4D5220268.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSerializedChannelData%41F4D5220268.implementation preserve=yes
      //## end ClSerializedChannelData%41F4D5220268.implementation

};

//## begin ClSerializedChannelData%41F4D5220268.postscript preserve=yes
//## end ClSerializedChannelData%41F4D5220268.postscript

//## begin ClSerializedChannelDataContainerBase_t%41F4D592010F.preface preserve=yes
//## end ClSerializedChannelDataContainerBase_t%41F4D592010F.preface

//## Class: ClSerializedChannelDataContainerBase_t%41F4D592010F
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%41F4D812005F;ClSerializedChannelData { -> }

typedef ClDatedContainer <ClSerializedChannelData> ClSerializedChannelDataContainerBase_t;
//## begin ClSerializedChannelDataContainerBase_t%41F4D592010F.postscript preserve=yes
//## end ClSerializedChannelDataContainerBase_t%41F4D592010F.postscript

//## begin ClSerializedChannelDataContainer%41F4D5EB019B.preface preserve=yes
//## end ClSerializedChannelDataContainer%41F4D5EB019B.preface

//## Class: ClSerializedChannelDataContainer%41F4D5EB019B
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSerializedChannelDataContainer : public ClSerializedChannelDataContainerBase_t  //## Inherits: <unnamed>%41F4D5F6016D
{
  //## begin ClSerializedChannelDataContainer%41F4D5EB019B.initialDeclarations preserve=yes
  //## end ClSerializedChannelDataContainer%41F4D5EB019B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSerializedChannelDataContainer%1106565740
      ClSerializedChannelDataContainer (ClCachePolicy* ai_poPolicy = NULL, m4uint32_t ai_iSizeObject = 0, ClCacheDatedKey* ai_poClCacheKey = NULL);


    //## Other Operations (specified)
      //## Operation: CreateObjectIdFromKey%1106565739
      ClObjectId* CreateObjectIdFromKey ();

      //## Operation: GetType%1106737889
      virtual m4uint8_t GetType ();

    // Additional Public Declarations
      //## begin ClSerializedChannelDataContainer%41F4D5EB019B.public preserve=yes
      //## end ClSerializedChannelDataContainer%41F4D5EB019B.public

  protected:
    // Additional Protected Declarations
      //## begin ClSerializedChannelDataContainer%41F4D5EB019B.protected preserve=yes
      //## end ClSerializedChannelDataContainer%41F4D5EB019B.protected

  private:
    // Additional Private Declarations
      //## begin ClSerializedChannelDataContainer%41F4D5EB019B.private preserve=yes
      //## end ClSerializedChannelDataContainer%41F4D5EB019B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSerializedChannelDataContainer%41F4D5EB019B.implementation preserve=yes
      //## end ClSerializedChannelDataContainer%41F4D5EB019B.implementation

};

//## begin ClSerializedChannelDataContainer%41F4D5EB019B.postscript preserve=yes
//## end ClSerializedChannelDataContainer%41F4D5EB019B.postscript

//## begin ClSerializedChannelDataFactory%41F4FD8B016A.preface preserve=yes
//## end ClSerializedChannelDataFactory%41F4FD8B016A.preface

//## Class: ClSerializedChannelDataFactory%41F4FD8B016A
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSerializedChannelDataFactory : public ClDiskPersistor, //## Inherits: <unnamed>%41F8F8C300C5
    	public ClBaseCacheableObjectFactory  //## Inherits: <unnamed>%41F8FA0C0249
{
  //## begin ClSerializedChannelDataFactory%41F4FD8B016A.initialDeclarations preserve=yes
  //## end ClSerializedChannelDataFactory%41F4FD8B016A.initialDeclarations

  public:
    //## Constructors (generated)
      ClSerializedChannelDataFactory();

    //## Destructor (generated)
      virtual ~ClSerializedChannelDataFactory();


    //## Other Operations (specified)
      //## Operation: CreateObject%1106737890
      virtual m4return_t CreateObject (m4uint8_t ai_iType, ClCacheableObject*& aio_pObject);

    // Additional Public Declarations
      //## begin ClSerializedChannelDataFactory%41F4FD8B016A.public preserve=yes
      //## end ClSerializedChannelDataFactory%41F4FD8B016A.public

  protected:
    // Additional Protected Declarations
      //## begin ClSerializedChannelDataFactory%41F4FD8B016A.protected preserve=yes
      //## end ClSerializedChannelDataFactory%41F4FD8B016A.protected

  private:
    // Additional Private Declarations
      //## begin ClSerializedChannelDataFactory%41F4FD8B016A.private preserve=yes
      //## end ClSerializedChannelDataFactory%41F4FD8B016A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSerializedChannelDataFactory%41F4FD8B016A.implementation preserve=yes
      //## end ClSerializedChannelDataFactory%41F4FD8B016A.implementation

};

//## begin ClSerializedChannelDataFactory%41F4FD8B016A.postscript preserve=yes
//## end ClSerializedChannelDataFactory%41F4FD8B016A.postscript

// Class ClSerializedChannelData 

// Class ClSerializedChannelDataContainer 

// Class ClSerializedChannelDataFactory 

//## begin module%41F4D72800AF.epilog preserve=yes
//## end module%41F4D72800AF.epilog


#endif
