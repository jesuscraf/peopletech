//## begin module%342BCA270031.cm preserve=no
//## end module%342BCA270031.cm

//## begin module%342BCA270031.cp preserve=no
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
//## end module%342BCA270031.cp

//## Module: Connects%342BCA270031; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\connects.hpp

#ifndef connects_h
#define connects_h 1

//## begin module%342BCA270031.additionalIncludes preserve=no
//## end module%342BCA270031.additionalIncludes

#include <connbase.hpp>
#include <statistics.hpp>
#include <connects40.hpp>

class ClConnectionVisitor;
class ClUSSessionInterface;



//## begin module%342BCA270031.declarations preserve=no
//## end module%342BCA270031.declarations

//## begin module%342BCA270031.additionalDeclarations preserve=yes
//## end module%342BCA270031.additionalDeclarations


//## begin ClConnection%342240440006.preface preserve=yes
//## end ClConnection%342240440006.preface

//## Class: ClConnection%342240440006
//	This is the connection object that contain information about the phisical
//	connection.
//	It has a refernece of its user owner, the name of its launcher host, a
//	reference to the receiver that will listen it and both cryptogrphy and
//	protocol objects.
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3455B560037A;ClConnectionVisitor { -> F}
//## Uses: <unnamed>%3782141002A2;ClRequest { -> F}

class ClConnection : public ClConnBase  //## Inherits: <unnamed>%34929E8F010A
{
  //## begin ClConnection%342240440006.initialDeclarations preserve=yes
  //## end ClConnection%342240440006.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConnection%882023676
      ClConnection (m4uint32_t ai_lIdConnection, ClTSAP *ai_pTSAP);

	  ClConnection (ClConnection40 *ai_poConn40);

    //## Destructor (specified)
      //## Operation: ~ClConnection%902904499
      ~ClConnection ();


    //## Other Operations (specified)
      //## Operation: Accept%878029381
      void Accept (ClConnectionVisitor &ai_oVisitor);

      //## Operation: GetMainSession%893147831
      ClUSSessionInterface * GetMainSession ();

      //## Operation: SetMainSesion%893147832
      void SetMainSesion (ClUSSessionInterface *ai_poSession);

      //## Operation: Recycle%941721547; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721548
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721549
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721550
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

      //## Operation: SetStatus%967450718
      m4return_t SetStatus (m4uint32_t ai_iValue);

      //## Operation: GetSessId%1010489723
      m4objid_t GetSessId ();

      //## Operation: GetUserId%1010489724
      m4objid_t GetUserId ();

    // Additional Public Declarations
      //## begin ClConnection%342240440006.public preserve=yes
      //## end ClConnection%342240440006.public

  protected:
    //## Constructors (specified)
      //## Operation: ClConnection%876138363
      ClConnection (m4objid_t ai_lIdConnection);

    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%353F08C9024C
      //## Role: ClConnection::m_pMainSession%353F08CA01A0
      //## begin ClConnection::m_pMainSession%353F08CA01A0.role preserve=no  protected: ClUSSessionInterface {1 -> 1RFHAN}
      ClUSSessionInterface *m_pMainSession;
      //## end ClConnection::m_pMainSession%353F08CA01A0.role

    // Additional Protected Declarations
      //## begin ClConnection%342240440006.protected preserve=yes
      //## end ClConnection%342240440006.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iNet%3438C729030D
      //## begin ClConnection::m_iNet%3438C729030D.attr preserve=no  private: m4int8_t {UA} 
      m4int8_t m_iNet;
      //## end ClConnection::m_iNet%3438C729030D.attr

      //## Attribute: m_iSessId%3C3ADF9C02D8
      //## begin ClConnection::m_iSessId%3C3ADF9C02D8.attr preserve=no  private: m4objid_t {UA} 0
      m4objid_t m_iSessId;
      //## end ClConnection::m_iSessId%3C3ADF9C02D8.attr

      //## Attribute: m_iUserId%3C3ADFB802BA
      //## begin ClConnection::m_iUserId%3C3ADFB802BA.attr preserve=no  private: m4objid_t {UA} 0
      m4objid_t m_iUserId;
      //## end ClConnection::m_iUserId%3C3ADFB802BA.attr

    // Additional Private Declarations
      //## begin ClConnection%342240440006.private preserve=yes
      //## end ClConnection%342240440006.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnection%342240440006.implementation preserve=yes
      //## end ClConnection%342240440006.implementation

};

//## begin ClConnection%342240440006.postscript preserve=yes
//## end ClConnection%342240440006.postscript

//## begin ClConnectionVisitor%344F5427004D.preface preserve=yes
//## end ClConnectionVisitor%344F5427004D.preface

//## Class: ClConnectionVisitor%344F5427004D
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConnectionVisitor : public ClVisitor  //## Inherits: <unnamed>%344F5436015D
{
  //## begin ClConnectionVisitor%344F5427004D.initialDeclarations preserve=yes
  //## end ClConnectionVisitor%344F5427004D.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitConnection%878029380
      void VisitConnection (ClConnection &ai_oConnection);

    // Additional Public Declarations
      //## begin ClConnectionVisitor%344F5427004D.public preserve=yes
      //## end ClConnectionVisitor%344F5427004D.public

  protected:
    // Additional Protected Declarations
      //## begin ClConnectionVisitor%344F5427004D.protected preserve=yes
      //## end ClConnectionVisitor%344F5427004D.protected

  private:
    // Additional Private Declarations
      //## begin ClConnectionVisitor%344F5427004D.private preserve=yes
      //## end ClConnectionVisitor%344F5427004D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnectionVisitor%344F5427004D.implementation preserve=yes
      //## end ClConnectionVisitor%344F5427004D.implementation

};

//## begin ClConnectionVisitor%344F5427004D.postscript preserve=yes
//## end ClConnectionVisitor%344F5427004D.postscript

// Class ClConnection 

// Class ClConnectionVisitor 

//## begin module%342BCA270031.epilog preserve=yes
//## end module%342BCA270031.epilog


#endif
