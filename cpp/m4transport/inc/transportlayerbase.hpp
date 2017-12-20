//## begin module%38D200DB02C5.cm preserve=no
//## end module%38D200DB02C5.cm

//## begin module%38D200DB02C5.cp preserve=no
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
//## end module%38D200DB02C5.cp

//## Module: TransportLayerBase%38D200DB02C5; Package specification
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Source file: F:\integration\m4transport\inc\transportlayerbase.hpp

#ifndef transportlayerbase_h
#define transportlayerbase_h 1

//## begin module%38D200DB02C5.additionalIncludes preserve=no
//## end module%38D200DB02C5.additionalIncludes

//## begin module%38D200DB02C5.includes preserve=yes
const int	TRANSPORT_ACTION_OPEN_PORT				=	1;
const int	TRANSPORT_ACTION_BEGIN_READ				=	2;
const int	TRANSPORT_ACTION_CLOSE_PORT				=	3;
const int	TRANSPORT_ACTION_STOP_READING			=	4;
const int	TRANSPORT_ACTION_WRITE					=	5;
const int	TRANSPORT_ACTION_STOP_WRITING			=	6;
const int	TRANSPORT_ACTION_CONNECT				=	7;
const int	TRANSPORT_ACTION_CLOSE_CONNECTION		=	8;
const int	TRANSPORT_ACTION_SSL_CONNECT			=	9;
const int	TRANSPORT_ACTION_OPEN_SSL_PORT			=	10;
const int	TRANSPORT_ACTION_STOP					=	11;
const int	TRANSPORT_ACTION_GET_TRANSFERED_BYTES	=	12;
//## end module%38D200DB02C5.includes

// CommComun
#include <commcomun.hpp>
// m4types
#include <m4types.hpp>
// stl
#include "m4stl.hpp"
// tsap
#include <tsap.hpp>
// stdio
#include <stdio.h>
// FastQ
#include <fastfifoq.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>

class ClTransportLayerContainer;

//## begin module%38D200DB02C5.declarations preserve=no
//## end module%38D200DB02C5.declarations

//## begin module%38D200DB02C5.additionalDeclarations preserve=yes
#define MAX_TRANSPORT_IDENTIFIERS_NUMBER  500
#define MIN_TRANSPORT_IDENTIFIERS_NUMBER  5
//## end module%38D200DB02C5.additionalDeclarations


//## begin TSAPType%38D78E9702E4.preface preserve=yes
//## end TSAPType%38D78E9702E4.preface

//## Class: TSAPType%38D78E9702E4
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum{CONTROL_PIPE,LISTEN_SOCKET,CONNECTION_SOCKET} TSAPType;

//## begin TSAPType%38D78E9702E4.postscript preserve=yes
//## end TSAPType%38D78E9702E4.postscript

//## begin ClTransportAction%38D226E40141.preface preserve=yes
//## end ClTransportAction%38D226E40141.preface

//## Class: ClTransportAction%38D226E40141
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38EB0FB8032B;ClTransportLayerContainer { -> F}

class ClTransportAction : public IGarbageable  //## Inherits: <unnamed>%3951F9370096
{
  //## begin ClTransportAction%38D226E40141.initialDeclarations preserve=yes
  //## end ClTransportAction%38D226E40141.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportAction%954835574
      ClTransportAction (int ai_iType, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: ClTransportAction%961667113
      ClTransportAction (int ai_iType);

    //## Destructor (specified)
      //## Operation: ~ClTransportAction%962001296
      virtual ~ClTransportAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954166906; C++
      virtual m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer) = 0;

      //## Operation: Recycle%961597594; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

      //## Operation: CanRecycle%961597595
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%961667110
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: CanDestroy%961667111
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClTransportAction%38D226E40141.public preserve=yes
      //## end ClTransportAction%38D226E40141.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38D75CCC0082
      //## Role: ClTransportAction::m_pTSAP%38D75CCC03C1
      //## begin ClTransportAction::m_pTSAP%38D75CCC03C1.role preserve=no  protected: ClTSAP { -> RHAN}
      ClTSAP *m_pTSAP;
      //## end ClTransportAction::m_pTSAP%38D75CCC03C1.role

      //## Association: m4transport::M4CTL::<unnamed>%38F44C0C026E
      //## Role: ClTransportAction::m_pIOEventHandler%38F44C0D0233
      //## begin ClTransportAction::m_pIOEventHandler%38F44C0D0233.role preserve=no  protected: ClIOEventHandler { -> RHAN}
      ClIOEventHandler *m_pIOEventHandler;
      //## end ClTransportAction::m_pIOEventHandler%38F44C0D0233.role

    // Additional Protected Declarations
      //## begin ClTransportAction%38D226E40141.protected preserve=yes
      //## end ClTransportAction%38D226E40141.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportAction%38D226E40141.private preserve=yes
      //## end ClTransportAction%38D226E40141.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportAction%38D226E40141.implementation preserve=yes
      //## end ClTransportAction%38D226E40141.implementation

};

//## begin ClTransportAction%38D226E40141.postscript preserve=yes
//## end ClTransportAction%38D226E40141.postscript

//## begin ClListTransportAction%38D20F2B0328.preface preserve=yes
//## end ClListTransportAction%38D20F2B0328.preface

//## Class: ClListTransportAction%38D20F2B0328; Instantiated Class
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38D226EC020B;ClTransportAction { -> }

typedef FastQueue< ClTransportAction * > ClListTransportAction;

//## begin ClListTransportAction%38D20F2B0328.postscript preserve=yes
//## end ClListTransportAction%38D20F2B0328.postscript

//## begin ClTransportLayerIdentifier%38D1E2410372.preface preserve=yes
//## end ClTransportLayerIdentifier%38D1E2410372.preface

//## Class: ClTransportLayerIdentifier%38D1E2410372
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportLayerIdentifier 
{
  //## begin ClTransportLayerIdentifier%38D1E2410372.initialDeclarations preserve=yes
  //## end ClTransportLayerIdentifier%38D1E2410372.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportLayerIdentifier%954835571
      ClTransportLayerIdentifier (ClTSAP *ai_pTSAP, TSAPType ai_TypeTSAP, ClIOEventHandler *ai_pIOEventHandler = NULL);


    //## Other Operations (specified)
      //## Operation: SetTSAP%954835576
      void SetTSAP (ClTSAP *ai_pTSAP);

      //## Operation: GetTSAP%954835577
      ClTSAP * GetTSAP ();

      //## Operation: SetTSAPType%954835587
      void SetTSAPType (TSAPType ai_TSAPType);

      //## Operation: GetTSAPType%954835588
      TSAPType GetTSAPType ();

      //## Operation: SetIOEventHandlerWrite%955436197
      void SetIOEventHandlerWrite (ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: GetIOEventHandlerWrite%955436198
      ClIOEventHandler * GetIOEventHandlerWrite ();

      //## Operation: SetIOEventHandlerRead%955360063
      void SetIOEventHandlerRead (ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: GetIOEventHandlerRead%954835558
      ClIOEventHandler * GetIOEventHandlerRead ();

      //## Operation: GetId%1014805135
      m4uint64_t GetId ();

    // Data Members for Class Attributes

      //## Attribute: m_ulReaded%391BD1540194
      //## begin ClTransportLayerIdentifier::m_ulReaded%391BD1540194.attr preserve=no  public: unsigned long {UA} 0L
      unsigned long m_ulReaded;
      //## end ClTransportLayerIdentifier::m_ulReaded%391BD1540194.attr

      //## Attribute: m_ulWrited%391BD1A002C0
      //## begin ClTransportLayerIdentifier::m_ulWrited%391BD1A002C0.attr preserve=no  public: unsigned long {UA} 0L
      unsigned long m_ulWrited;
      //## end ClTransportLayerIdentifier::m_ulWrited%391BD1A002C0.attr

    // Additional Public Declarations
      //## begin ClTransportLayerIdentifier%38D1E2410372.public preserve=yes
      //## end ClTransportLayerIdentifier%38D1E2410372.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38E9A10C01EF
      //## Role: ClTransportLayerIdentifier::m_pIOEventHandlerRead%38E9A10D00ED
      //## begin ClTransportLayerIdentifier::m_pIOEventHandlerRead%38E9A10D00ED.role preserve=no  protected: ClIOEventHandler { -> RHAN}
      ClIOEventHandler *m_pIOEventHandlerRead;
      //## end ClTransportLayerIdentifier::m_pIOEventHandlerRead%38E9A10D00ED.role

      //## Association: m4transport::M4CTL::<unnamed>%38EA001303A9
      //## Role: ClTransportLayerIdentifier::m_pTSAP%38EA00140197
      //## begin ClTransportLayerIdentifier::m_pTSAP%38EA00140197.role preserve=no  protected: ClTSAP { -> RHAN}
      ClTSAP *m_pTSAP;
      //## end ClTransportLayerIdentifier::m_pTSAP%38EA00140197.role

      //## Association: m4transport::M4CTL::<unnamed>%38F1B9810240
      //## Role: ClTransportLayerIdentifier::m_TSAPType%38F1B9820011
      //## begin ClTransportLayerIdentifier::m_TSAPType%38F1B9820011.role preserve=no  protected: TSAPType { -> VHAN}
      TSAPType m_TSAPType;
      //## end ClTransportLayerIdentifier::m_TSAPType%38F1B9820011.role

      //## Association: m4transport::M4CTL::<unnamed>%38F2DD300187
      //## Role: ClTransportLayerIdentifier::m_pIOEventHandlerWrite%38F2DD3102C9
      //## begin ClTransportLayerIdentifier::m_pIOEventHandlerWrite%38F2DD3102C9.role preserve=no  protected: ClIOEventHandler { -> RHAN}
      ClIOEventHandler *m_pIOEventHandlerWrite;
      //## end ClTransportLayerIdentifier::m_pIOEventHandlerWrite%38F2DD3102C9.role

    // Additional Protected Declarations
      //## begin ClTransportLayerIdentifier%38D1E2410372.protected preserve=yes
      //## end ClTransportLayerIdentifier%38D1E2410372.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lId%3C7CB22F0025
      //## begin ClTransportLayerIdentifier::m_lId%3C7CB22F0025.attr preserve=no  private: m4uint64_t {UA} 0
      m4uint64_t m_lId;
      //## end ClTransportLayerIdentifier::m_lId%3C7CB22F0025.attr

    // Additional Private Declarations
      //## begin ClTransportLayerIdentifier%38D1E2410372.private preserve=yes
      //## end ClTransportLayerIdentifier%38D1E2410372.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportLayerIdentifier%38D1E2410372.implementation preserve=yes
      //## end ClTransportLayerIdentifier%38D1E2410372.implementation

};

//## begin ClTransportLayerIdentifier%38D1E2410372.postscript preserve=yes
//## end ClTransportLayerIdentifier%38D1E2410372.postscript

//## begin ClMapTransportLayerIdentifier%38D1EB8F0129.preface preserve=yes
//## end ClMapTransportLayerIdentifier%38D1EB8F0129.preface

//## Class: ClMapTransportLayerIdentifier%38D1EB8F0129; Instantiated Class
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38D2105401A8;ClTransportLayerIdentifier { -> }

typedef map< m4uint32_t,ClTransportLayerIdentifier *,less <m4uint32_t> > ClMapTransportLayerIdentifier;

//## begin ClMapTransportLayerIdentifier%38D1EB8F0129.postscript preserve=yes
//## end ClMapTransportLayerIdentifier%38D1EB8F0129.postscript

//## begin ClTransportActionOpenPort%38DF327502CF.preface preserve=yes
//## end ClTransportActionOpenPort%38DF327502CF.preface

//## Class: ClTransportActionOpenPort%38DF327502CF
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38EB0F750325;ClServerTSAPSelectable { -> }

class ClTransportActionOpenPort : public ClTransportAction  //## Inherits: <unnamed>%38DF32910366
{
  //## begin ClTransportActionOpenPort%38DF327502CF.initialDeclarations preserve=yes
  //## end ClTransportActionOpenPort%38DF327502CF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionOpenPort%961667112
      ClTransportActionOpenPort ();

      //## Operation: ClTransportActionOpenPort%961742125
      ClTransportActionOpenPort (int ai_iType);

    //## Destructor (specified)
      //## Operation: ~ClTransportActionOpenPort%962001297
      ~ClTransportActionOpenPort ();


    //## Other Operations (specified)
      //## Operation: Init%954835573
      m4return_t Init (int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: Execute%954835575
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

    // Additional Public Declarations
      //## begin ClTransportActionOpenPort%38DF327502CF.public preserve=yes
      //## end ClTransportActionOpenPort%38DF327502CF.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38DF347F0009
      //## Role: ClTransportActionOpenPort::m_iPort%38DF3480001F
      //## begin ClTransportActionOpenPort::m_iPort%38DF3480001F.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iPort;
      //## end ClTransportActionOpenPort::m_iPort%38DF3480001F.role

    // Additional Protected Declarations
      //## begin ClTransportActionOpenPort%38DF327502CF.protected preserve=yes
      //## end ClTransportActionOpenPort%38DF327502CF.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionOpenPort%38DF327502CF.private preserve=yes
      //## end ClTransportActionOpenPort%38DF327502CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionOpenPort%38DF327502CF.implementation preserve=yes
      //## end ClTransportActionOpenPort%38DF327502CF.implementation

};

//## begin ClTransportActionOpenPort%38DF327502CF.postscript preserve=yes
//## end ClTransportActionOpenPort%38DF327502CF.postscript

//## begin ClTransportActionBeginRead%38DF353B0009.preface preserve=yes
//## end ClTransportActionBeginRead%38DF353B0009.preface

//## Class: ClTransportActionBeginRead%38DF353B0009
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionBeginRead : public ClTransportAction  //## Inherits: <unnamed>%38DF35570226
{
  //## begin ClTransportActionBeginRead%38DF353B0009.initialDeclarations preserve=yes
  //## end ClTransportActionBeginRead%38DF353B0009.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionBeginRead%961667115
      ClTransportActionBeginRead ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionBeginRead%962001298
      ~ClTransportActionBeginRead ();


    //## Other Operations (specified)
      //## Operation: Execute%955035324; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961667116
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionBeginRead%38DF353B0009.public preserve=yes
      //## end ClTransportActionBeginRead%38DF353B0009.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionBeginRead%38DF353B0009.protected preserve=yes
      //## end ClTransportActionBeginRead%38DF353B0009.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionBeginRead%38DF353B0009.private preserve=yes
      //## end ClTransportActionBeginRead%38DF353B0009.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionBeginRead%38DF353B0009.implementation preserve=yes
      //## end ClTransportActionBeginRead%38DF353B0009.implementation

};

//## begin ClTransportActionBeginRead%38DF353B0009.postscript preserve=yes
//## end ClTransportActionBeginRead%38DF353B0009.postscript

//## begin ClTransportActionClosePort%38DF357300B4.preface preserve=yes
//## end ClTransportActionClosePort%38DF357300B4.preface

//## Class: ClTransportActionClosePort%38DF357300B4
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionClosePort : public ClTransportAction  //## Inherits: <unnamed>%38DF3581014A
{
  //## begin ClTransportActionClosePort%38DF357300B4.initialDeclarations preserve=yes
  //## end ClTransportActionClosePort%38DF357300B4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionClosePort%961742128
      ClTransportActionClosePort ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionClosePort%962001299
      ~ClTransportActionClosePort ();


    //## Other Operations (specified)
      //## Operation: Execute%955035320
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742129
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionClosePort%38DF357300B4.public preserve=yes
      //## end ClTransportActionClosePort%38DF357300B4.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionClosePort%38DF357300B4.protected preserve=yes
      //## end ClTransportActionClosePort%38DF357300B4.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionClosePort%38DF357300B4.private preserve=yes
      //## end ClTransportActionClosePort%38DF357300B4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionClosePort%38DF357300B4.implementation preserve=yes
      //## end ClTransportActionClosePort%38DF357300B4.implementation

};

//## begin ClTransportActionClosePort%38DF357300B4.postscript preserve=yes
//## end ClTransportActionClosePort%38DF357300B4.postscript

//## begin ClTransportActionStopReading%38DF35C703D6.preface preserve=yes
//## end ClTransportActionStopReading%38DF35C703D6.preface

//## Class: ClTransportActionStopReading%38DF35C703D6
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionStopReading : public ClTransportAction  //## Inherits: <unnamed>%38DF35D603A6
{
  //## begin ClTransportActionStopReading%38DF35C703D6.initialDeclarations preserve=yes
  //## end ClTransportActionStopReading%38DF35C703D6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionStopReading%961667119
      ClTransportActionStopReading ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionStopReading%962001300
      ~ClTransportActionStopReading ();


    //## Other Operations (specified)
      //## Operation: Execute%955466400
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961667120
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionStopReading%38DF35C703D6.public preserve=yes
      //## end ClTransportActionStopReading%38DF35C703D6.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionStopReading%38DF35C703D6.protected preserve=yes
      //## end ClTransportActionStopReading%38DF35C703D6.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionStopReading%38DF35C703D6.private preserve=yes
      //## end ClTransportActionStopReading%38DF35C703D6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionStopReading%38DF35C703D6.implementation preserve=yes
      //## end ClTransportActionStopReading%38DF35C703D6.implementation

};

//## begin ClTransportActionStopReading%38DF35C703D6.postscript preserve=yes
//## end ClTransportActionStopReading%38DF35C703D6.postscript

//## begin ClTransportActionWrite%38DF365000BC.preface preserve=yes
//## end ClTransportActionWrite%38DF365000BC.preface

//## Class: ClTransportActionWrite%38DF365000BC
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionWrite : public ClTransportAction  //## Inherits: <unnamed>%38DF366A0204
{
  //## begin ClTransportActionWrite%38DF365000BC.initialDeclarations preserve=yes
  //## end ClTransportActionWrite%38DF365000BC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionWrite%961667117
      ClTransportActionWrite ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionWrite%962001301
      ~ClTransportActionWrite ();


    //## Other Operations (specified)
      //## Operation: Execute%955360065; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961667118
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38DF3740022A
      //## Role: ClTransportActionWrite::m_pBuffer%38DF37410204
      //## begin ClTransportActionWrite::m_pBuffer%38DF37410204.role preserve=no  public: m4char_t { -> RHAN}
      m4char_t *m_pBuffer;
      //## end ClTransportActionWrite::m_pBuffer%38DF37410204.role

    // Additional Public Declarations
      //## begin ClTransportActionWrite%38DF365000BC.public preserve=yes
      //## end ClTransportActionWrite%38DF365000BC.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38DF36D303B3
      //## Role: ClTransportActionWrite::m_Size%38DF36D500EF
      //## begin ClTransportActionWrite::m_Size%38DF36D500EF.role preserve=no  protected: size_t { -> VHAN}
      size_t m_Size;
      //## end ClTransportActionWrite::m_Size%38DF36D500EF.role

    // Additional Protected Declarations
      //## begin ClTransportActionWrite%38DF365000BC.protected preserve=yes
      //## end ClTransportActionWrite%38DF365000BC.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionWrite%38DF365000BC.private preserve=yes
      //## end ClTransportActionWrite%38DF365000BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionWrite%38DF365000BC.implementation preserve=yes
      //## end ClTransportActionWrite%38DF365000BC.implementation

};

//## begin ClTransportActionWrite%38DF365000BC.postscript preserve=yes
//## end ClTransportActionWrite%38DF365000BC.postscript

//## begin ClTransportActionStopWriting%38DF37860361.preface preserve=yes
//## end ClTransportActionStopWriting%38DF37860361.preface

//## Class: ClTransportActionStopWriting%38DF37860361
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionStopWriting : public ClTransportAction  //## Inherits: <unnamed>%38DF379E0170
{
  //## begin ClTransportActionStopWriting%38DF37860361.initialDeclarations preserve=yes
  //## end ClTransportActionStopWriting%38DF37860361.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionStopWriting%961667121
      ClTransportActionStopWriting ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionStopWriting%962001302
      ~ClTransportActionStopWriting ();


    //## Other Operations (specified)
      //## Operation: Execute%955466402
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961667122
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionStopWriting%38DF37860361.public preserve=yes
      //## end ClTransportActionStopWriting%38DF37860361.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionStopWriting%38DF37860361.protected preserve=yes
      //## end ClTransportActionStopWriting%38DF37860361.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionStopWriting%38DF37860361.private preserve=yes
      //## end ClTransportActionStopWriting%38DF37860361.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionStopWriting%38DF37860361.implementation preserve=yes
      //## end ClTransportActionStopWriting%38DF37860361.implementation

};

//## begin ClTransportActionStopWriting%38DF37860361.postscript preserve=yes
//## end ClTransportActionStopWriting%38DF37860361.postscript

//## begin ClTransportActionConnect%38DF37BC012D.preface preserve=yes
//## end ClTransportActionConnect%38DF37BC012D.preface

//## Class: ClTransportActionConnect%38DF37BC012D
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionConnect : public ClTransportAction  //## Inherits: <unnamed>%38DF37C60345
{
  //## begin ClTransportActionConnect%38DF37BC012D.initialDeclarations preserve=yes
  //## end ClTransportActionConnect%38DF37BC012D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionConnect%961742122
      ClTransportActionConnect ();

      //## Operation: ClTransportActionConnect%961742124
      ClTransportActionConnect (int ai_iType);

    //## Destructor (specified)
      //## Operation: ~ClTransportActionConnect%962001303
      ~ClTransportActionConnect ();


    //## Other Operations (specified)
      //## Operation: Execute%955536868; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742123
      m4return_t Init (char *ai_pHost, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionConnect%38DF37BC012D.public preserve=yes
      //## end ClTransportActionConnect%38DF37BC012D.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38DF37DE0231
      //## Role: ClTransportActionConnect::m_pHost%38DF37DF02A0
      //## begin ClTransportActionConnect::m_pHost%38DF37DF02A0.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pHost;
      //## end ClTransportActionConnect::m_pHost%38DF37DF02A0.role

      //## Association: m4transport::M4CTL::<unnamed>%38DF38C303AD
      //## Role: ClTransportActionConnect::m_bSSL%38DF38C402E6
      //## begin ClTransportActionConnect::m_bSSL%38DF38C402E6.role preserve=no  protected: m4bool_t { -> UHAN}
      m4bool_t m_bSSL;
      //## end ClTransportActionConnect::m_bSSL%38DF38C402E6.role

      //## Association: m4transport::M4CTL::<unnamed>%38F49D8E03BC
      //## Role: ClTransportActionConnect::m_iPort%38F49D8F0165
      //## begin ClTransportActionConnect::m_iPort%38F49D8F0165.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iPort;
      //## end ClTransportActionConnect::m_iPort%38F49D8F0165.role

    // Additional Protected Declarations
      //## begin ClTransportActionConnect%38DF37BC012D.protected preserve=yes
      //## end ClTransportActionConnect%38DF37BC012D.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionConnect%38DF37BC012D.private preserve=yes
      //## end ClTransportActionConnect%38DF37BC012D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionConnect%38DF37BC012D.implementation preserve=yes
      //## end ClTransportActionConnect%38DF37BC012D.implementation

};

//## begin ClTransportActionConnect%38DF37BC012D.postscript preserve=yes
//## end ClTransportActionConnect%38DF37BC012D.postscript

//## begin ClTransportActionCloseConnection%38DF390C0235.preface preserve=yes
//## end ClTransportActionCloseConnection%38DF390C0235.preface

//## Class: ClTransportActionCloseConnection%38DF390C0235
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionCloseConnection : public ClTransportAction  //## Inherits: <unnamed>%38DF391F00E8
{
  //## begin ClTransportActionCloseConnection%38DF390C0235.initialDeclarations preserve=yes
  //## end ClTransportActionCloseConnection%38DF390C0235.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionCloseConnection%961742130
      ClTransportActionCloseConnection ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionCloseConnection%962001304
      ~ClTransportActionCloseConnection ();


    //## Other Operations (specified)
      //## Operation: Execute%955536870; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742131
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionCloseConnection%38DF390C0235.public preserve=yes
      //## end ClTransportActionCloseConnection%38DF390C0235.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionCloseConnection%38DF390C0235.protected preserve=yes
      //## end ClTransportActionCloseConnection%38DF390C0235.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionCloseConnection%38DF390C0235.private preserve=yes
      //## end ClTransportActionCloseConnection%38DF390C0235.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionCloseConnection%38DF390C0235.implementation preserve=yes
      //## end ClTransportActionCloseConnection%38DF390C0235.implementation

};

//## begin ClTransportActionCloseConnection%38DF390C0235.postscript preserve=yes
//## end ClTransportActionCloseConnection%38DF390C0235.postscript

//## begin ClTransportActionSSLConnect%39042600001F.preface preserve=yes
//## end ClTransportActionSSLConnect%39042600001F.preface

//## Class: ClTransportActionSSLConnect%39042600001F
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionSSLConnect : public ClTransportActionConnect  //## Inherits: <unnamed>%3904275403AD
{
  //## begin ClTransportActionSSLConnect%39042600001F.initialDeclarations preserve=yes
  //## end ClTransportActionSSLConnect%39042600001F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionSSLConnect%961742120
      ClTransportActionSSLConnect ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionSSLConnect%962001305
      ~ClTransportActionSSLConnect ();


    //## Other Operations (specified)
      //## Operation: Execute%956570161; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742121
      m4return_t Init (int ai_iMode, char *ai_pCipher, char *ai_pHost, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionSSLConnect%39042600001F.public preserve=yes
      //## end ClTransportActionSSLConnect%39042600001F.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%390427630200
      //## Role: ClTransportActionSSLConnect::m_iMode%3904276401CF
      //## begin ClTransportActionSSLConnect::m_iMode%3904276401CF.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iMode;
      //## end ClTransportActionSSLConnect::m_iMode%3904276401CF.role

      //## Association: m4transport::M4CTL::<unnamed>%390427920348
      //## Role: ClTransportActionSSLConnect::m_pCipher%3904279403CE
      //## begin ClTransportActionSSLConnect::m_pCipher%3904279403CE.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pCipher;
      //## end ClTransportActionSSLConnect::m_pCipher%3904279403CE.role

    // Additional Protected Declarations
      //## begin ClTransportActionSSLConnect%39042600001F.protected preserve=yes
      //## end ClTransportActionSSLConnect%39042600001F.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionSSLConnect%39042600001F.private preserve=yes
      //## end ClTransportActionSSLConnect%39042600001F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionSSLConnect%39042600001F.implementation preserve=yes
      //## end ClTransportActionSSLConnect%39042600001F.implementation

};

//## begin ClTransportActionSSLConnect%39042600001F.postscript preserve=yes
//## end ClTransportActionSSLConnect%39042600001F.postscript

//## begin ClTransportActionOpenSSLPort%390454D10023.preface preserve=yes
//## end ClTransportActionOpenSSLPort%390454D10023.preface

//## Class: ClTransportActionOpenSSLPort%390454D10023
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionOpenSSLPort : public ClTransportActionOpenPort  //## Inherits: <unnamed>%390455310306
{
  //## begin ClTransportActionOpenSSLPort%390454D10023.initialDeclarations preserve=yes
  //## end ClTransportActionOpenSSLPort%390454D10023.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionOpenSSLPort%961742126
      ClTransportActionOpenSSLPort ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionOpenSSLPort%962001306
      ~ClTransportActionOpenSSLPort ();


    //## Other Operations (specified)
      //## Operation: Execute%956570163
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742127
      m4return_t Init (int ai_iMode, char *ai_pCipher, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionOpenSSLPort%390454D10023.public preserve=yes
      //## end ClTransportActionOpenSSLPort%390454D10023.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%3904557900A6
      //## Role: ClTransportActionOpenSSLPort::m_iMode%390455790287
      //## begin ClTransportActionOpenSSLPort::m_iMode%390455790287.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iMode;
      //## end ClTransportActionOpenSSLPort::m_iMode%390455790287.role

      //## Association: m4transport::M4CTL::<unnamed>%390455C5000F
      //## Role: ClTransportActionOpenSSLPort::m_pCipher%390455C50273
      //## begin ClTransportActionOpenSSLPort::m_pCipher%390455C50273.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pCipher;
      //## end ClTransportActionOpenSSLPort::m_pCipher%390455C50273.role

    // Additional Protected Declarations
      //## begin ClTransportActionOpenSSLPort%390454D10023.protected preserve=yes
      //## end ClTransportActionOpenSSLPort%390454D10023.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionOpenSSLPort%390454D10023.private preserve=yes
      //## end ClTransportActionOpenSSLPort%390454D10023.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionOpenSSLPort%390454D10023.implementation preserve=yes
      //## end ClTransportActionOpenSSLPort%390454D10023.implementation

};

//## begin ClTransportActionOpenSSLPort%390454D10023.postscript preserve=yes
//## end ClTransportActionOpenSSLPort%390454D10023.postscript

//## begin ClTransportActionStop%39094FB900D2.preface preserve=yes
//## end ClTransportActionStop%39094FB900D2.preface

//## Class: ClTransportActionStop%39094FB900D2
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionStop : public ClTransportAction  //## Inherits: <unnamed>%39094FC8019C
{
  //## begin ClTransportActionStop%39094FB900D2.initialDeclarations preserve=yes
  //## end ClTransportActionStop%39094FB900D2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionStop%961742132
      ClTransportActionStop ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionStop%962001307
      ~ClTransportActionStop ();


    //## Other Operations (specified)
      //## Operation: Execute%956912717; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%956912716
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionStop%39094FB900D2.public preserve=yes
      //## end ClTransportActionStop%39094FB900D2.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionStop%39094FB900D2.protected preserve=yes
      //## end ClTransportActionStop%39094FB900D2.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionStop%39094FB900D2.private preserve=yes
      //## end ClTransportActionStop%39094FB900D2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionStop%39094FB900D2.implementation preserve=yes
      //## end ClTransportActionStop%39094FB900D2.implementation

};

//## begin ClTransportActionStop%39094FB900D2.postscript preserve=yes
//## end ClTransportActionStop%39094FB900D2.postscript

//## begin ClTransportActionGetTransferedBytes%391BD333017F.preface preserve=yes
//## end ClTransportActionGetTransferedBytes%391BD333017F.preface

//## Class: ClTransportActionGetTransferedBytes%391BD333017F
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportActionGetTransferedBytes : public ClTransportAction  //## Inherits: <unnamed>%391BD884009F
{
  //## begin ClTransportActionGetTransferedBytes%391BD333017F.initialDeclarations preserve=yes
  //## end ClTransportActionGetTransferedBytes%391BD333017F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportActionGetTransferedBytes%961742133
      ClTransportActionGetTransferedBytes ();

    //## Destructor (specified)
      //## Operation: ~ClTransportActionGetTransferedBytes%962001308
      ~ClTransportActionGetTransferedBytes ();


    //## Other Operations (specified)
      //## Operation: Execute%958124561; C++
      m4return_t Execute (ClTransportLayerContainer *ai_pTransportLayerContainer);

      //## Operation: Init%961742134
      m4return_t Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportActionGetTransferedBytes%391BD333017F.public preserve=yes
      //## end ClTransportActionGetTransferedBytes%391BD333017F.public

  protected:
    // Additional Protected Declarations
      //## begin ClTransportActionGetTransferedBytes%391BD333017F.protected preserve=yes
      //## end ClTransportActionGetTransferedBytes%391BD333017F.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportActionGetTransferedBytes%391BD333017F.private preserve=yes
      //## end ClTransportActionGetTransferedBytes%391BD333017F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportActionGetTransferedBytes%391BD333017F.implementation preserve=yes
      //## end ClTransportActionGetTransferedBytes%391BD333017F.implementation

};

//## begin ClTransportActionGetTransferedBytes%391BD333017F.postscript preserve=yes
//## end ClTransportActionGetTransferedBytes%391BD333017F.postscript

// Class ClTransportAction 

inline ClTransportAction::~ClTransportAction ()
{
  //## begin ClTransportAction::~ClTransportAction%962001296.body preserve=yes
  //## end ClTransportAction::~ClTransportAction%962001296.body
}


// Class ClTransportLayerIdentifier 

// Class ClTransportActionOpenPort 

// Class ClTransportActionBeginRead 

// Class ClTransportActionClosePort 

// Class ClTransportActionStopReading 

// Class ClTransportActionWrite 

// Class ClTransportActionStopWriting 

// Class ClTransportActionConnect 

// Class ClTransportActionCloseConnection 

// Class ClTransportActionSSLConnect 

// Class ClTransportActionOpenSSLPort 

// Class ClTransportActionStop 

// Class ClTransportActionGetTransferedBytes 

//## begin module%38D200DB02C5.epilog preserve=yes
//## end module%38D200DB02C5.epilog


#endif
