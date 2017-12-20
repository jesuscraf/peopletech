//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              cladministrator.hpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the administrator interface in the
//		server side.
//
//	==============================================================================
//## end module.cp

//## Module: ClAdministrator; Package specification
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\inc\cladministrator.hpp

#ifndef cladministrator_h
#define cladministrator_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4adminsrv_dll.hpp>
#include <usinterface.hpp>
//## end module.includes

// m4types
#include <m4types.hpp>
// adminPDUExecutor
#include <adminpduexec.hpp>
// SystemInterface
#include <systeminteface.hpp>
// LogonInterface
#include <logoninterface.hpp>
// NavigatorF
#include <navigatorf.hpp>
// admonpdus
//#include <admonpdus.hpp>
// tsap
#include <tsap.hpp>
// m4thread
#include <m4thread.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#define M4_DEFAULT_ADMINISTRATOR_DEFAULT_ROLE "M4ADM"
#define M4_DEFAULT_ADMINISTRATOR_ROLE "M4SYSADM"
//## end module.additionalDeclarations


//## Class: ClCSAdministrator
//	It manages all administration requests received from the
//	network.
//	This class is instanciated when  a conexion request is
//	received in the administrator port.
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClReplyParamPDU { -> }
//## Uses: <unnamed>; m4int64_t { -> }
//## Uses: <unnamed>; ClPDU { -> }
//## Uses: <unnamed>; ClLogonInterface { -> }
//## Uses: <unnamed>; ClAdminPDUExecutor { -> }

class M4_DECL_M4ADMINSRV ClCSAdministrator : public M4Thread  //## Inherits: <unnamed> 
, public IEventConsumer, public IEventProducer
{
  //## begin ClCSAdministrator.initialDeclarations preserve=yes
  //## end ClCSAdministrator.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCSAdministrator%878114676
      ClCSAdministrator (ClTSAP *ai_poTSAP, ClNavigatorFirewall *ai_poNavigator, ClSystemInterface *ai_poSystem);

    //## Destructor (specified)
      //## Operation: ~ClCSAdministrator%878114677
      ~ClCSAdministrator ();


    //## Other Operations (specified)
      //## Operation: Run%878114678
      //	This method is called the administrator is created. It's
      //	waiting for input PDU's, process them, sends the answer
      //	PDU, and waits for the next PDU.
      void Run ();

      //## Operation: ReadPDU%885830286
      //	Read the input administrator PDU and return the PDU and
      //	the PDU type.
      //
      //	Output parameters:
      //	   - ao_poInputPDU. Pointer to the input PDU. The memory
      //	for this PDU is allocated here. The memory must be
      //	deleted in the function this method is called from.
      //	   - ao_iPDUId. Here is returned the PDU Id. Valid
      //	values are in basepdus.hpp header file.
      //
      //	Return:
      //	   M4_SUCCESS on success
      //	   M4_ERROR otherwise
      m4return_t ReadPDU (ClPDU* &ao_poInputPDU, m4int16_t &ao_iPDUId);

      //## Operation: SendPDU%885830287
      //	Send the output PDU to the network.
      //
      //	Return:
      //	    M4_SUCCESS on success
      //	    M4_ERROR otherwise
      m4return_t SendPDU (ClPDU* ai_poPDU);

    // Data Members for Class Attributes

      //## Attribute: m_User
      //	Save the logon administrator user.
      //## begin ClCSAdministrator::m_User.attr preserve=no  public: M4ClString {UA} 
      M4ClString m_User;
      //## end ClCSAdministrator::m_User.attr

      //## Attribute: m_Password
      //	Save the logon administrator password.
      //## begin ClCSAdministrator::m_Password.attr preserve=no  public: M4ClString {UA} 
      M4ClString m_Password;
      //## end ClCSAdministrator::m_Password.attr

      //## Attribute: m_Role
      //	Save the logon administrator role.
      //## begin ClCSAdministrator::m_Role.attr preserve=no  public: M4ClString {UA} 
      M4ClString m_Role;
      //## end ClCSAdministrator::m_Role.attr

      //## Attribute: m_poLogonInterface
      //	Save the logonInterface object created in the
      //	administrator authentication.
      //## begin ClCSAdministrator::m_poLogonInterface.attr preserve=no  public: ClLogonInterface * {UA} NULL
      ClLogonInterface *m_poLogonInterface;
      //## end ClCSAdministrator::m_poLogonInterface.attr

      //## Attribute: m_poUSSessionInterface
      //	Save the sessionInterface object created during the
      //	administrator authentication.
      //## begin ClCSAdministrator::m_poUSSessionInterface.attr preserve=no  public: ClUSSessionInterface * {UA} NULL
      ClUSSessionInterface *m_poUSSessionInterface;
      //## end ClCSAdministrator::m_poUSSessionInterface.attr

    // Data Members for Associations

      //## Association: M4AdminSrv::<unnamed>%369C79CD00BB
      //## Role: ClCSAdministrator::m_poNavigator
      //## begin ClCSAdministrator::m_poNavigator.role preserve=no  public: ClNavigatorFirewall { -> RHAN}
      ClNavigatorFirewall *m_poNavigator;
      //## end ClCSAdministrator::m_poNavigator.role

      //## Association: M4AdminSrv::<unnamed>%369CD03C0375
      //## Role: ClCSAdministrator::m_poSystem
      //## begin ClCSAdministrator::m_poSystem.role preserve=no  public: ClSystemInterface { -> RHAN}
      ClSystemInterface *m_poSystem;
      //## end ClCSAdministrator::m_poSystem.role

      //## Association: M4ComSrv::Administrator::<unnamed>%34CCB23301FB
      //## Role: ClCSAdministrator::m_poTSAP
      //## begin ClCSAdministrator::m_poTSAP.role preserve=no  public: ClTSAP { -> 1RHAN}
      ClTSAP *m_poTSAP;
      //## end ClCSAdministrator::m_poTSAP.role

      //## Association: M4ComSrv::Administrator::<unnamed>%35800AEE029F
      //## Role: ClCSAdministrator::m_oPduExecutorCreator
      //## begin ClCSAdministrator::m_oPduExecutorCreator.role preserve=no  public: ClAdminPDUExecutorCreator { -> 1UHAN}
      ClAdminPDUExecutorCreator m_oPduExecutorCreator;
      //## end ClCSAdministrator::m_oPduExecutorCreator.role

      //## Association: M4ComSrv::LocalMonitor::<unnamed>%348FCF0E004F
      //## Role: ClCSAdministrator::<m_pClAdminSS>
      //## begin ClCSAdministrator::<m_pClAdminSS>.role preserve=no  public: ClAdminSS { -> RHGA}
      //## end ClCSAdministrator::<m_pClAdminSS>.role

    // Additional Public Declarations
      //## begin ClCSAdministrator.public preserve=yes
	  m4return_t GetConsumerID(m4handle_t &ao_hConsumerID ){return M4_ERROR;}
	  m4return_t GetProducerID(m4handle_t &ao_hProducerID ){return M4_ERROR;}
      //## end ClCSAdministrator.public

  protected:
    // Additional Protected Declarations
      //## begin ClCSAdministrator.protected preserve=yes
      //## end ClCSAdministrator.protected

  private:
    // Additional Private Declarations
      //## begin ClCSAdministrator.private preserve=yes
      //## end ClCSAdministrator.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClCSAdministrator.implementation preserve=yes
      //## end ClCSAdministrator.implementation

};

//## begin ClCSAdministrator.postscript preserve=yes
//## end ClCSAdministrator.postscript



// Class ClCSAdministrator 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
