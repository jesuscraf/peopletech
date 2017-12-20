//## begin module%374E960F03F6.cm preserve=no
//## end module%374E960F03F6.cm

//## begin module%374E960F03F6.cp preserve=no
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
//## end module%374E960F03F6.cp

//## Module: generic_event%374E960F03F6; Package specification
//## Subsystem: M4FileManager::inc%374E95FD03B5
//	d:\fuentesservidor\m4filemanager\version\inc
//## Source file: d:\evol\m4filemanager\version\inc\generic_event.hpp

#ifndef _GENERIC_EVENT_HPP_
#define _GENERIC_EVENT_HPP_ 1

//## begin module%374E960F03F6.additionalIncludes preserve=no
//## end module%374E960F03F6.additionalIncludes

//## begin module%374E960F03F6.includes preserve=yes
//## end module%374E960F03F6.includes

// m4types
#include "m4types.hpp"
#include "m4filemanager_dll.hpp"
//## begin module%374E960F03F6.declarations preserve=no
//## end module%374E960F03F6.declarations


//## Class: ClEventType%374E95FF03C2
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4uint32_t ClEventType;

//## begin ClEventType%374E95FF03C2.postscript preserve=yes
const ClEventType EVENT_TYPE_FILE   = 0;
const ClEventType EVENT_TYPE_TIMER  = 1;
const ClEventType EVENT_TYPE_RM     = 2;
const ClEventType EVENT_TYPE_OTHER  = 3;
//## end ClEventType%374E95FF03C2.postscript

//## begin ClEventDescription%374E960F03F8.preface preserve=yes
//## end ClEventDescription%374E960F03F8.preface

//## Class: ClEventDescription%374E960F03F8
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClEventDescription 
{
  //## begin ClEventDescription%374E960F03F8.initialDeclarations preserve=yes
  //## end ClEventDescription%374E960F03F8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventDescription%-2107733181; C++
      ClEventDescription (ClEventType ai_oType, void* ai_pDetails);

    //## Destructor (specified)
      //## Operation: ~ClEventDescription%1169785350; C++
      //	-- {AddDecl: 178} region.unprotectedFunction [833..885]
      virtual ~ClEventDescription ();


    //## Other Operations (specified)
      //## Operation: GetType%-274530691; C++
      //	Inheritance can be done
      //	-- {AddDecl: 179} region.unprotectedFunction [886..956]
      ClEventType GetType ();

      //## Operation: SetType%1411540414; C++
      //	-- {AddDecl: 180} region.unprotectedFunction [957..1050]
      void SetType (ClEventType ai_oNewType);

      //## Operation: GetDetails%404087045; C++
      //	-- {AddDecl: 181} region.unprotectedFunction [1051..1127]
      void* GetDetails () const;

      //## Operation: SetDetails%2057788636; C++
      //	-- {AddDecl: 182} region.unprotectedFunction [1128..1227]
      void SetDetails (void* ai_pNewDetails);

    // Additional Public Declarations
      //## begin ClEventDescription%374E960F03F8.public preserve=yes
      //## end ClEventDescription%374E960F03F8.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventDescription%374E960F03F8.protected preserve=yes
      //## end ClEventDescription%374E960F03F8.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pDetails%374E96100042
      //## begin ClEventDescription::m_pDetails%374E96100042.attr preserve=no  private: void* {VA} 
      void* m_pDetails;
      //## end ClEventDescription::m_pDetails%374E96100042.attr

    // Data Members for Associations

      //## Association: M4FileManager::<unnamed>%374E9610003F
      //## Role: ClEventDescription::m_oType%374E96100041
      //## begin ClEventDescription::m_oType%374E96100041.role preserve=no  private: ClEventType {1 -> 1VHAPN}
      ClEventType m_oType;
      //## end ClEventDescription::m_oType%374E96100041.role

    // Additional Private Declarations
      //## begin ClEventDescription%374E960F03F8.private preserve=yes
      //## end ClEventDescription%374E960F03F8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventDescription%374E960F03F8.implementation preserve=yes
      //## end ClEventDescription%374E960F03F8.implementation

};

//## begin ClEventDescription%374E960F03F8.postscript preserve=yes


/*************************************************************************

NAME: ClEventHandler
	
*************************************************************************/
//## end ClEventDescription%374E960F03F8.postscript

//## begin ClEventHandler%374E96100043.preface preserve=yes
//## end ClEventHandler%374E96100043.preface

//## Class: ClEventHandler%374E96100043; Abstract
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%374E96100044;m4return_t { -> }
//## Uses: <unnamed>%374E96100045;ClEventDescription { -> }

class M4_DECL_M4FILEMANAGER ClEventHandler 
{
  //## begin ClEventHandler%374E96100043.initialDeclarations preserve=yes
  //## end ClEventHandler%374E96100043.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: EventHandler%317377500; C++
      virtual m4return_t EventHandler (ClEventDescription& ai_oEventDescription) = 0;

    // Additional Public Declarations
      //## begin ClEventHandler%374E96100043.public preserve=yes
      //## end ClEventHandler%374E96100043.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventHandler%374E96100043.protected preserve=yes
      //## end ClEventHandler%374E96100043.protected

  private:
    // Additional Private Declarations
      //## begin ClEventHandler%374E96100043.private preserve=yes
      //## end ClEventHandler%374E96100043.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventHandler%374E96100043.implementation preserve=yes
      //## end ClEventHandler%374E96100043.implementation

};

//## begin ClEventHandler%374E96100043.postscript preserve=yes


/*************************************************************************

NAME: ClEventsClientId
	
*************************************************************************/
//## end ClEventHandler%374E96100043.postscript

//## begin ClEventsClientId%374E96100048.preface preserve=yes
//## end ClEventsClientId%374E96100048.preface

//## Class: ClEventsClientId%374E96100048
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClEventsClientId : public ClEventHandler  //## Inherits: <unnamed>%374E96100049
{
  //## begin ClEventsClientId%374E96100048.initialDeclarations preserve=yes
  //## end ClEventsClientId%374E96100048.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClEventsClientId%374E96100048.public preserve=yes
      //## end ClEventsClientId%374E96100048.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventsClientId%374E96100048.protected preserve=yes
      //## end ClEventsClientId%374E96100048.protected

  private:
    // Additional Private Declarations
      //## begin ClEventsClientId%374E96100048.private preserve=yes
      //## end ClEventsClientId%374E96100048.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventsClientId%374E96100048.implementation preserve=yes
      //## end ClEventsClientId%374E96100048.implementation

};

//## begin ClEventsClientId%374E96100048.postscript preserve=yes


/*************************************************************************

NAME: ClEventControllableObject
	
*************************************************************************/
//## end ClEventsClientId%374E96100048.postscript

//## begin ClEventControllableObject%374E9610004A.preface preserve=yes
//## end ClEventControllableObject%374E9610004A.preface

//## Class: ClEventControllableObject%374E9610004A
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClEventControllableObject : public ClEventsClientId  //## Inherits: <unnamed>%374E9610004B
{
  //## begin ClEventControllableObject%374E9610004A.initialDeclarations preserve=yes
  //## end ClEventControllableObject%374E9610004A.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClEventControllableObject%374E9610004A.public preserve=yes
      //## end ClEventControllableObject%374E9610004A.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventControllableObject%374E9610004A.protected preserve=yes
      //## end ClEventControllableObject%374E9610004A.protected

  private:
    // Additional Private Declarations
      //## begin ClEventControllableObject%374E9610004A.private preserve=yes
      //## end ClEventControllableObject%374E9610004A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventControllableObject%374E9610004A.implementation preserve=yes
      //## end ClEventControllableObject%374E9610004A.implementation

};

//## begin ClEventControllableObject%374E9610004A.postscript preserve=yes


/*************************************************************************

NAME: ClEventBroker
	
*************************************************************************/
//## end ClEventControllableObject%374E9610004A.postscript

//## begin ClEventBroker%374E9610004C.preface preserve=yes
//## end ClEventBroker%374E9610004C.preface

//## Class: ClEventBroker%374E9610004C
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%374E96100058;m4return_t { -> }
//## Uses: <unnamed>%374E96100059;ClEventType { -> }
//## Uses: <unnamed>%374E9610005A;ClEventsClientId { -> }
//## Uses: <unnamed>%374E96100062;ClEventDescription { -> }

class M4_DECL_M4FILEMANAGER ClEventBroker 
{
  //## begin ClEventBroker%374E9610004C.initialDeclarations preserve=yes
  //## end ClEventBroker%374E9610004C.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClEventBroker%109429335; C++
      //	-- {AddDecl: 185} region.unprotectedFunction [1541..1577]
      virtual ~ClEventBroker ();


    //## Other Operations (specified)
      //## Operation: Instance%625172185; C++
      //	Inheritance can be done
      //	-- {AddDecl: 186} region.unprotectedFunction [1578..2045]
      static ClEventBroker* Instance ();

      //## Operation: SetInterestInEventType%9126463; C++
      //	Singleton
      //	Set/unset interest by type of event
      //	-- {AddDecl: 187} region.unprotectedFunction [2046..2188]
      m4return_t SetInterestInEventType (ClEventType ai_oEventType, ClEventsClientId* ai_oClientId);

      //## Operation: UnSetInterestInEventType%999267762; C++
      //	-- {AddDecl: 188} region.unprotectedFunction [2189..2333]
      m4return_t UnSetInterestInEventType (ClEventType ai_oEventType, ClEventsClientId* ai_oClientId);

      //## Operation: SetInterestInEvent%-2032400336; C++
      //	Set/unset interest by specific event
      //	-- {AddDecl: 189} region.unprotectedFunction [2334..2480]
      m4return_t SetInterestInEvent (ClEventDescription ai_oEventType, ClEventsClientId* ai_oClientId);

      //## Operation: UnSetInterestInEvent%-1530773485; C++
      //	-- {AddDecl: 190} region.unprotectedFunction [2481..2629]
      m4return_t UnSetInterestInEvent (ClEventDescription ai_oEventType, ClEventsClientId* ai_oClientId);

    // Additional Public Declarations
      //## begin ClEventBroker%374E9610004C.public preserve=yes
      //## end ClEventBroker%374E9610004C.public

  protected:
    //## Constructors (specified)
      //## Operation: ClEventBroker%-247254367; C++
      //	-- {AddDecl: 191} region.unprotectedFunction [2630..2671]
      ClEventBroker ();

    // Additional Protected Declarations
      //## begin ClEventBroker%374E9610004C.protected preserve=yes
      //## end ClEventBroker%374E9610004C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poInstance%374E96100069
      //## begin ClEventBroker::m_poInstance%374E96100069.attr preserve=no  private: static ClEventBroker* {VA} NULL
      static ClEventBroker* m_poInstance;
      //## end ClEventBroker::m_poInstance%374E96100069.attr

    // Additional Private Declarations
      //## begin ClEventBroker%374E9610004C.private preserve=yes
      //## end ClEventBroker%374E9610004C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventBroker%374E9610004C.implementation preserve=yes
      //## end ClEventBroker%374E9610004C.implementation

};

//## begin ClEventBroker%374E9610004C.postscript preserve=yes
//## end ClEventBroker%374E9610004C.postscript

// Class ClEventDescription 

// Class ClEventHandler 

// Class ClEventsClientId 

// Class ClEventControllableObject 

// Class ClEventBroker 

//## begin module%374E960F03F6.epilog preserve=yes
//## end module%374E960F03F6.epilog


#endif
