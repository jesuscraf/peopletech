//## begin module%407D081C0341.cm preserve=no
//## end module%407D081C0341.cm

//## begin module%407D081C0341.cp preserve=no
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
//## end module%407D081C0341.cp

//## Module: snmpadmininstance%407D081C0341; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmpadmininstance.hpp

#ifndef snmpadmininstance_h
#define snmpadmininstance_h 1

//## begin module%407D081C0341.additionalIncludes preserve=no
//## end module%407D081C0341.additionalIncludes

#include <snmpinterfaces.hpp>

//## begin module%407D081C0341.additionalDeclarations preserve=yes
//## end module%407D081C0341.additionalDeclarations


//## begin M4SnmpAdminInstance%406BF093035B.preface preserve=yes
//## end M4SnmpAdminInstance%406BF093035B.preface

//## Class: M4SnmpAdminInstance%406BF093035B
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SNMPADMIN M4SnmpAdminInstance 
{
  //## begin M4SnmpAdminInstance%406BF093035B.initialDeclarations preserve=yes
  //## end M4SnmpAdminInstance%406BF093035B.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Create%1080808689
      static m4return_t Create (ClConfiguration* ai_config);

      //## Operation: Get%1081265059
      static M4ISnmpAdministrator* Get ();

      //## Operation: Destroy%1081862818
      static void Destroy ();

      //## Operation: Create%1084791067
      static m4return_t Create (const DOMNode *ai_config);

    // Additional Public Declarations
      //## begin M4SnmpAdminInstance%406BF093035B.public preserve=yes
      //## end M4SnmpAdminInstance%406BF093035B.public

  protected:
    // Additional Protected Declarations
      //## begin M4SnmpAdminInstance%406BF093035B.protected preserve=yes
      //## end M4SnmpAdminInstance%406BF093035B.protected

  private:
    //## Constructors (generated)
      M4SnmpAdminInstance();


    //## Other Operations (specified)
      //## Operation: M4SnmpAdminFactory%1080808688
      void M4SnmpAdminFactory ();

    // Data Members for Associations

      //## Association: M4SnmpAdmin::<unnamed>%40729FDE012E
      //## Role: M4SnmpAdminInstance::<m_pM4ISnmpAdministratorFactory>%40729FDE039C
      //## begin M4SnmpAdminInstance::<m_pM4ISnmpAdministratorFactory>%40729FDE039C.role preserve=no  private: static M4ISnmpAdministratorFactory { -> RHAN}
      static M4ISnmpAdministratorFactory *m_pM4ISnmpAdministratorFactory;
      //## end M4SnmpAdminInstance::<m_pM4ISnmpAdministratorFactory>%40729FDE039C.role

      //## Association: M4SnmpAdmin::<unnamed>%4072CB7D01B8
      //## Role: M4SnmpAdminInstance::<m_pM4ISnmpAdministrator>%4072CB7E014C
      //## begin M4SnmpAdminInstance::<m_pM4ISnmpAdministrator>%4072CB7E014C.role preserve=no  private: static M4ISnmpAdministrator { -> RHAN}
      static M4ISnmpAdministrator *m_pM4ISnmpAdministrator;
      //## end M4SnmpAdminInstance::<m_pM4ISnmpAdministrator>%4072CB7E014C.role

    // Additional Private Declarations
      //## begin M4SnmpAdminInstance%406BF093035B.private preserve=yes
      //## end M4SnmpAdminInstance%406BF093035B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpAdminInstance%406BF093035B.implementation preserve=yes
      //## end M4SnmpAdminInstance%406BF093035B.implementation

};

//## begin M4SnmpAdminInstance%406BF093035B.postscript preserve=yes
//## end M4SnmpAdminInstance%406BF093035B.postscript

// Class M4SnmpAdminInstance 

//## begin module%407D081C0341.epilog preserve=yes
//## end module%407D081C0341.epilog


#endif
