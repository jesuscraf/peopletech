//## begin module%359C8A9401EA.cm preserve=no
//## end module%359C8A9401EA.cm

//## begin module%359C8A9401EA.cp preserve=no
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
//## end module%359C8A9401EA.cp

//## Module: Proxy_SubSystem%359C8A9401EA; Package specification
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Source file: F:\integration\m4exeforchannel\inc\proxysubsys.hpp

#ifndef proxy_subsystem_h
#define proxy_subsystem_h 1

//## begin module%359C8A9401EA.additionalIncludes preserve=no
//## end module%359C8A9401EA.additionalIncludes

//## begin module%359C8A9401EA.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
#include <executiveforoess.hpp>
#include <clservicebasess.hpp>

class ClTables;
class ClConfiguration;
class ClNavigatorFirewall;


#define M4_PROXYSUBSYSTEM_NAME				"PROXY_SUBSYSTEM"



//## begin module%359C8A9401EA.declarations preserve=no
//## end module%359C8A9401EA.declarations

//## begin module%359C8A9401EA.additionalDeclarations preserve=yes
//## end module%359C8A9401EA.additionalDeclarations


//## begin ClProxySubSystem%359C8B9E011A.preface preserve=yes
//## end ClProxySubSystem%359C8B9E011A.preface

//## Class: ClProxySubSystem%359C8B9E011A
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38270FCF00C3;ClConfiguration { -> F}
//## Uses: <unnamed>%391BCC6B022F;ClLog { -> F}
//## Uses: <unnamed>%38270FA30200;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%391BCA410201;ClOblConfiguration { -> F}
//## Uses: <unnamed>%359C8D80036C;ClTables { -> F}

class M4_DECL_M4EXEFORCHANNEL ClProxySubSystem : public ClServiceBaseSS  //## Inherits: <unnamed>%359C8BB201EB
{
  //## begin ClProxySubSystem%359C8B9E011A.initialDeclarations preserve=yes
  //## end ClProxySubSystem%359C8B9E011A.initialDeclarations

  public:
    //## begin ClProxySubSystem::eProxySSCommandsId%35BCAFBD02AB.preface preserve=yes
    //## end ClProxySubSystem::eProxySSCommandsId%35BCAFBD02AB.preface

    //## Class: eProxySSCommandsId%35BCAFBD02AB
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {PROXY_SS_LIST_SPACES=ClServiceBaseSS::TOTAL_COMMANDS, PROXY_SS_TOTAL_COMMANDS} eProxySSCommandsId;

    //## begin ClProxySubSystem::eProxySSCommandsId%35BCAFBD02AB.postscript preserve=yes
    //## end ClProxySubSystem::eProxySSCommandsId%35BCAFBD02AB.postscript

    //## Constructors (specified)
      //## Operation: ClProxySubSystem%899454531
      ClProxySubSystem (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClProxySubSystem%899454532
      ~ClProxySubSystem ();


    //## Other Operations (specified)
      //## Operation: GenerateSubsystem%958122096
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: Initialize%958122097
      //	A virtual method that only calls to the derived class from ClSSStats
      //	Container Initialize method
      virtual m4return_t Initialize ();

  public:
    // Additional Public Declarations
      //## begin ClProxySubSystem%359C8B9E011A.public preserve=yes
      //## end ClProxySubSystem%359C8B9E011A.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: M4ProxySubsystemName%391BC89A00E0
      //## begin ClProxySubSystem::M4ProxySubsystemName%391BC89A00E0.attr preserve=no  protected: static m4pchar_t {UAC} "PROXY_SERVICE"
      static const m4pchar_t  M4ProxySubsystemName;
      //## end ClProxySubSystem::M4ProxySubsystemName%391BC89A00E0.attr

    // Additional Protected Declarations
      //## begin ClProxySubSystem%359C8B9E011A.protected preserve=yes
      //## end ClProxySubSystem%359C8B9E011A.protected

  private:
    // Additional Private Declarations
      //## begin ClProxySubSystem%359C8B9E011A.private preserve=yes
      //## end ClProxySubSystem%359C8B9E011A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProxySubSystem%359C8B9E011A.implementation preserve=yes
      //## end ClProxySubSystem%359C8B9E011A.implementation

};

//## begin ClProxySubSystem%359C8B9E011A.postscript preserve=yes
//## end ClProxySubSystem%359C8B9E011A.postscript

//## begin ClExecutiveForProxySS%391BD1D80223.preface preserve=yes
//## end ClExecutiveForProxySS%391BD1D80223.preface

//## Class: ClExecutiveForProxySS%391BD1D80223
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%391BD251026D;ClExecutiveForProxy { -> F}
//## Uses: <unnamed>%391BD3770099;ClLauncherForOLTP { -> F}

class ClExecutiveForProxySS : public ClExecutiveForOESS  //## Inherits: <unnamed>%391BD1ED0147
{
  //## begin ClExecutiveForProxySS%391BD1D80223.initialDeclarations preserve=yes
  //## end ClExecutiveForProxySS%391BD1D80223.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForProxySS%958122098
      ClExecutiveForProxySS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForProxySS%958122099
      ~ClExecutiveForProxySS ();


    //## Other Operations (specified)
      //## Operation: Initialize%958122100
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

    // Additional Public Declarations
      //## begin ClExecutiveForProxySS%391BD1D80223.public preserve=yes
      //## end ClExecutiveForProxySS%391BD1D80223.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: s_szProxyInstanceOrder%391BD23002C9
      //## begin ClExecutiveForProxySS::s_szProxyInstanceOrder%391BD23002C9.attr preserve=no  protected: static m4size_t {UA} 0
      static m4size_t s_szProxyInstanceOrder;
      //## end ClExecutiveForProxySS::s_szProxyInstanceOrder%391BD23002C9.attr

    // Additional Protected Declarations
      //## begin ClExecutiveForProxySS%391BD1D80223.protected preserve=yes
      //## end ClExecutiveForProxySS%391BD1D80223.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutiveForProxySS%391BD1D80223.private preserve=yes
      //## end ClExecutiveForProxySS%391BD1D80223.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForProxySS%391BD1D80223.implementation preserve=yes
      //## end ClExecutiveForProxySS%391BD1D80223.implementation

};

//## begin ClExecutiveForProxySS%391BD1D80223.postscript preserve=yes
//## end ClExecutiveForProxySS%391BD1D80223.postscript

// Class ClProxySubSystem 

// Class ClExecutiveForProxySS 

//## begin module%359C8A9401EA.epilog preserve=yes
//## end module%359C8A9401EA.epilog


#endif
