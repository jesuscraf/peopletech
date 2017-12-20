//## begin module%36E4F5D703AA.cm preserve=no
//## end module%36E4F5D703AA.cm

//## begin module%36E4F5D703AA.cp preserve=no
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
//## end module%36E4F5D703AA.cp

//## Module: OLTP_SubSystem%36E4F5D703AA; Package specification
//## Subsystem: M4ExeForChannel::OLTP::inc%392276370123
//## Source file: F:\integration\m4exeforchannel\inc\oltpsubsys.hpp

#ifndef oltp_subsystem_h
#define oltp_subsystem_h 1

//## begin module%36E4F5D703AA.additionalIncludes preserve=no
//## end module%36E4F5D703AA.additionalIncludes

//## begin module%36E4F5D703AA.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
#include <executiveforoess.hpp>
#include <clservicebasess.hpp>

class ClTables;
class ClNavigatorFirewall;


#define M4_OLTPSUBSYSTEM_NAME							"OLTP_SUBSYSTEM"
#define MAX_OLTP_INFO_ITEM								1024
#define OLTP_MAX_USERNAME								128




//## begin module%36E4F5D703AA.declarations preserve=no
//## end module%36E4F5D703AA.declarations

//## begin module%36E4F5D703AA.additionalDeclarations preserve=yes
//## end module%36E4F5D703AA.additionalDeclarations


//## begin ClOLTPSubsystem%36E4F4FE0312.preface preserve=yes
//## end ClOLTPSubsystem%36E4F4FE0312.preface

//## Class: ClOLTPSubsystem%36E4F4FE0312
//## Category: M4ExeForChannel::OLTP%392274A701C8
//## Subsystem: M4ExeForChannel::OLTP::inc%392276370123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36E52D60005A;ClOblConfiguration { -> F}
//## Uses: <unnamed>%391047630368;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%39104A430276;ClExecutiveForChannelSS { -> F}
//## Uses: <unnamed>%391BC97F03A6;ClTables { -> F}

class M4_DECL_M4EXEFORCHANNEL ClOLTPSubsystem : public ClServiceBaseSS  //## Inherits: <unnamed>%36E4F51100A3
{
  //## begin ClOLTPSubsystem%36E4F4FE0312.initialDeclarations preserve=yes
  //## end ClOLTPSubsystem%36E4F4FE0312.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClOLTPSubsystem%920975044
      ClOLTPSubsystem (ClConfiguration *  ai_poConfiguration, ClTables *  ai_poTables, ClNavigatorFirewall *  ai_pNavigator);

    //## Destructor (specified)
      //## Operation: ~ClOLTPSubsystem%920975045
      ~ClOLTPSubsystem ();


    //## Other Operations (specified)
      //## Operation: Initialize%920975040
      m4return_t Initialize ();

      //## Operation: GenerateSubsystem%957346232
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    // Additional Public Declarations
      //## begin ClOLTPSubsystem%36E4F4FE0312.public preserve=yes
      //## end ClOLTPSubsystem%36E4F4FE0312.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: M4OLTPSubsystemName%3912D6AA020C
      //## begin ClOLTPSubsystem::M4OLTPSubsystemName%3912D6AA020C.attr preserve=no  protected: static m4pchar_t {VAC} "OLTP_SERVICE"
      static const m4pchar_t  M4OLTPSubsystemName;
      //## end ClOLTPSubsystem::M4OLTPSubsystemName%3912D6AA020C.attr

    // Additional Protected Declarations
      //## begin ClOLTPSubsystem%36E4F4FE0312.protected preserve=yes
      //## end ClOLTPSubsystem%36E4F4FE0312.protected

  private:
    // Additional Private Declarations
      //## begin ClOLTPSubsystem%36E4F4FE0312.private preserve=yes
      //## end ClOLTPSubsystem%36E4F4FE0312.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClOLTPSubsystem%36E4F4FE0312.implementation preserve=yes
      //## end ClOLTPSubsystem%36E4F4FE0312.implementation

  public:
      virtual m4return_t ShutDown();
};

//## begin ClOLTPSubsystem%36E4F4FE0312.postscript preserve=yes
//## end ClOLTPSubsystem%36E4F4FE0312.postscript

//## begin ClExecutiveForChannelSS%391049CD02DA.preface preserve=yes
//## end ClExecutiveForChannelSS%391049CD02DA.preface

//## Class: ClExecutiveForChannelSS%391049CD02DA
//## Category: M4ExeForChannel::OLTP%392274A701C8
//## Subsystem: M4ExeForChannel::OLTP::inc%392276370123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%391049DC0047;ClExecutiveForChannel { -> F}
//## Uses: <unnamed>%39104F950184;ClLauncherForOLTP { -> F}

class ClExecutiveForChannelSS : public ClExecutiveForOESS  //## Inherits: <unnamed>%391049D90115
{
  //## begin ClExecutiveForChannelSS%391049CD02DA.initialDeclarations preserve=yes
  //## end ClExecutiveForChannelSS%391049CD02DA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForChannelSS%957346233
      //	constructor de la clase
      ClExecutiveForChannelSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForChannelSS%957346234
      ~ClExecutiveForChannelSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%957346235
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

    // Additional Public Declarations
      //## begin ClExecutiveForChannelSS%391049CD02DA.public preserve=yes
      //## end ClExecutiveForChannelSS%391049CD02DA.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: s_szOLTPInstanceOrder%3912E76A023A
      //## begin ClExecutiveForChannelSS::s_szOLTPInstanceOrder%3912E76A023A.attr preserve=no  protected: static m4size_t {VA} 0
      static m4size_t s_szOLTPInstanceOrder;
      //## end ClExecutiveForChannelSS::s_szOLTPInstanceOrder%3912E76A023A.attr

    // Additional Protected Declarations
      //## begin ClExecutiveForChannelSS%391049CD02DA.protected preserve=yes
      //## end ClExecutiveForChannelSS%391049CD02DA.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutiveForChannelSS%391049CD02DA.private preserve=yes
      //## end ClExecutiveForChannelSS%391049CD02DA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForChannelSS%391049CD02DA.implementation preserve=yes
      //## end ClExecutiveForChannelSS%391049CD02DA.implementation

};

//## begin ClExecutiveForChannelSS%391049CD02DA.postscript preserve=yes
//## end ClExecutiveForChannelSS%391049CD02DA.postscript

// Class ClOLTPSubsystem 

// Class ClExecutiveForChannelSS 

//## begin module%36E4F5D703AA.epilog preserve=yes
//## end module%36E4F5D703AA.epilog


#endif
