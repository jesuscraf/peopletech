//## begin module%38E06664024A.cm preserve=no
//## end module%38E06664024A.cm

//## begin module%38E06664024A.cp preserve=no
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
//## end module%38E06664024A.cp

//## Module: suplementaryservices%38E06664024A; Package specification
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Source file: F:\integration\m4echoservice\inc\suplementaryservices.hpp

#ifndef suplementaryservices_h
#define suplementaryservices_h 1

//## begin module%38E06664024A.additionalIncludes preserve=no
//## end module%38E06664024A.additionalIncludes

//## begin module%38E06664024A.includes preserve=yes
#include <m4echoservice_dll.hpp>
//## end module%38E06664024A.includes

// Statistics
#include <statistics.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>

class ClSuplementaryServicesVisitor;


//## begin module%38E06664024A.declarations preserve=no
//## end module%38E06664024A.declarations

//## begin module%38E06664024A.additionalDeclarations preserve=yes
//## end module%38E06664024A.additionalDeclarations


//## Class: ClSuplementaryServicesSS%34901F1B033B
//## Category: M4echoservice%38DF86CA013C
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36A474AB0105;ClSuplementaryServicesVisitor { -> F}

class M4_DECL_M4ECHOSERVICE ClSuplementaryServicesSS : public ClSSImplementation  //## Inherits: <unnamed>%38E06623005C
{
  public:
    //## begin ClSuplementaryServicesSS::eServiceListCommands%34F5BCB602FC.preface preserve=yes
    //## end ClSuplementaryServicesSS::eServiceListCommands%34F5BCB602FC.preface

    //## Class: eServiceListCommands%34F5BCB602FC
    //## Category: M4echoservice%38DF86CA013C
    //## Subsystem: M4echoservice::inc%38DF873D03CC
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {  TOTAL_COMMANDS = ClSSImplementation::TOTAL_COMMANDS } eServiceListCommands;

    //## begin ClSuplementaryServicesSS::eServiceListCommands%34F5BCB602FC.postscript preserve=yes
    //## end ClSuplementaryServicesSS::eServiceListCommands%34F5BCB602FC.postscript

    //## Constructors (specified)
      //## Operation: ClSuplementaryServicesSS%882177081
      ClSuplementaryServicesSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSuplementaryServicesSS%882177082
      ~ClSuplementaryServicesSS ();


    //## Other Operations (specified)
      //## Operation: Accept%884777559
      void Accept (ClSuplementaryServicesVisitor &ai_oVisitor);

      //## Operation: Synchronize%884942697
      m4return_t Synchronize ();

      //## Operation: Initialize%885806474
      m4return_t Initialize ();

      //## Operation: ShutDown%885806519
      m4return_t ShutDown ();

      //## Operation: ExecuteCommand%901709443
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709444
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709445
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GenerateSubsystem%954230996
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

  public:
  protected:
  private:
  private: //## implementation
};

//## Class: ClSuplementaryServicesVisitor%34BCE5CF0229
//## Category: M4echoservice%38DF86CA013C
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34C4714D0109;ClSuplementaryServicesSS { -> F}

class ClSuplementaryServicesVisitor : public ClVisitor  //## Inherits: <unnamed>%34BCE5DD03E2
{
  public:

    //## Other Operations (specified)
      //## Operation: VisitServiceList%885288584
      void VisitServiceList (ClSuplementaryServicesSS &ai_oServiceList);

  protected:
  private:
  private: //## implementation
};

// Class ClSuplementaryServicesSS 

// Class ClSuplementaryServicesVisitor 

//## begin module%38E06664024A.epilog preserve=yes
//## end module%38E06664024A.epilog


#endif
