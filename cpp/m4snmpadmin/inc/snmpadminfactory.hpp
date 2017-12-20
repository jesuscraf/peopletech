//## begin module%406BF15D0329.cm preserve=no
//## end module%406BF15D0329.cm

//## begin module%406BF15D0329.cp preserve=no
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
//## end module%406BF15D0329.cp

//## Module: snmpadminfactory%406BF15D0329; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmpadminfactory.hpp

#ifndef snmpadminfactory_h
#define snmpadminfactory_h 1

//## begin module%406BF15D0329.additionalIncludes preserve=no
//## end module%406BF15D0329.additionalIncludes

//## begin module%406BF15D0329.includes preserve=yes
//## end module%406BF15D0329.includes

// snmpadmin
//#include <snmpadmin.hpp>
// snmpinterfaces
#include <snmpinterfaces.hpp>
//## begin module%406BF15D0329.declarations preserve=no
//## end module%406BF15D0329.declarations

//## begin module%406BF15D0329.additionalDeclarations preserve=yes
//## end module%406BF15D0329.additionalDeclarations


//## begin M4SnmpAdministratorFactory%40729F95004D.preface preserve=yes
//## end M4SnmpAdministratorFactory%40729F95004D.preface

//## Class: M4SnmpAdministratorFactory%40729F95004D
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4072A47202A6;M4SnmpStandAloneAdministrator { -> }

class M4SnmpAdministratorFactory : public M4ISnmpAdministratorFactory  //## Inherits: <unnamed>%40729FB00236
{
  //## begin M4SnmpAdministratorFactory%40729F95004D.initialDeclarations preserve=yes
  //## end M4SnmpAdministratorFactory%40729F95004D.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: getStandAloneAdmin%1081250851
      M4ISnmpAdministrator* getStandAloneAdmin ();

      //## Operation: getMasterAdmin%1081250852
      M4ISnmpAdministrator* getMasterAdmin ();

      //## Operation: getSlaveAdmin%1081250853
      M4ISnmpAdministrator* getSlaveAdmin ();

    // Additional Public Declarations
      //## begin M4SnmpAdministratorFactory%40729F95004D.public preserve=yes
      //## end M4SnmpAdministratorFactory%40729F95004D.public

  protected:
    // Additional Protected Declarations
      //## begin M4SnmpAdministratorFactory%40729F95004D.protected preserve=yes
      //## end M4SnmpAdministratorFactory%40729F95004D.protected

  private:
    // Additional Private Declarations
      //## begin M4SnmpAdministratorFactory%40729F95004D.private preserve=yes
      //## end M4SnmpAdministratorFactory%40729F95004D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpAdministratorFactory%40729F95004D.implementation preserve=yes
      //## end M4SnmpAdministratorFactory%40729F95004D.implementation

};

//## begin M4SnmpAdministratorFactory%40729F95004D.postscript preserve=yes
//## end M4SnmpAdministratorFactory%40729F95004D.postscript

// Class M4SnmpAdministratorFactory 

//## begin module%406BF15D0329.epilog preserve=yes
//## end module%406BF15D0329.epilog


#endif
