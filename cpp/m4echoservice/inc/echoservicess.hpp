//## begin module%38DB44E0002F.cm preserve=no
//## end module%38DB44E0002F.cm

//## begin module%38DB44E0002F.cp preserve=no
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
//## end module%38DB44E0002F.cp

//## Module: echoservicess%38DB44E0002F; Package specification
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Source file: F:\integration\m4echoservice\inc\echoservicess.hpp

#ifndef echoservicess_h
#define echoservicess_h 1

//## begin module%38DB44E0002F.additionalIncludes preserve=no
//## end module%38DB44E0002F.additionalIncludes

//## begin module%38DB44E0002F.includes preserve=yes
#include <m4echoservice_dll.hpp>
//## end module%38DB44E0002F.includes

// clexecutivebasess
#include <clexecutivebasess.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%38DB44E0002F.declarations preserve=no
//## end module%38DB44E0002F.declarations

//## begin module%38DB44E0002F.additionalDeclarations preserve=yes
//## end module%38DB44E0002F.additionalDeclarations


//## begin ClECHOServiceSS%38DB43230166.preface preserve=yes
//## end ClECHOServiceSS%38DB43230166.preface

//## Class: ClECHOServiceSS%38DB43230166
//## Category: M4echoservice%38DF86CA013C
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4ECHOSERVICE ClECHOServiceSS : public ClServiceBaseSS  //## Inherits: <unnamed>%38DB445D0226
{
  //## begin ClECHOServiceSS%38DB43230166.initialDeclarations preserve=yes
  //## end ClECHOServiceSS%38DB43230166.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClECHOServiceSS%953897286
      ClECHOServiceSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClECHOServiceSS%953897287
      ~ClECHOServiceSS ();


    //## Other Operations (specified)
      //## Operation: GenerateSubsystem%953897293
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

      //## Operation: Initialize%954230997
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

    // Additional Public Declarations
      //## begin ClECHOServiceSS%38DB43230166.public preserve=yes
      //## end ClECHOServiceSS%38DB43230166.public

  protected:
    // Additional Protected Declarations
      //## begin ClECHOServiceSS%38DB43230166.protected preserve=yes
      //## end ClECHOServiceSS%38DB43230166.protected

  private:
    // Additional Private Declarations
      //## begin ClECHOServiceSS%38DB43230166.private preserve=yes
      //## end ClECHOServiceSS%38DB43230166.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClECHOServiceSS%38DB43230166.implementation preserve=yes
      //## end ClECHOServiceSS%38DB43230166.implementation

};

//## begin ClECHOServiceSS%38DB43230166.postscript preserve=yes
//## end ClECHOServiceSS%38DB43230166.postscript

//## begin ClECHOExecutorSS%38DB4346004E.preface preserve=yes
//## end ClECHOExecutorSS%38DB4346004E.preface

//## Class: ClECHOExecutorSS%38DB4346004E
//## Category: M4echoservice%38DF86CA013C
//## Subsystem: M4echoservice::inc%38DF873D03CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38DF8B1102C7;ClExeForEcho { -> F}

class ClECHOExecutorSS : public ClExecutiveBaseSS  //## Inherits: <unnamed>%38DB446000EA
{
  //## begin ClECHOExecutorSS%38DB4346004E.initialDeclarations preserve=yes
  //## end ClECHOExecutorSS%38DB4346004E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClECHOExecutorSS%953897288
      ClECHOExecutorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClECHOExecutorSS%953897289
      ~ClECHOExecutorSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%953897290
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

      //## Operation: Start%953897291
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      virtual m4return_t Start ();

    // Additional Public Declarations
      //## begin ClECHOExecutorSS%38DB4346004E.public preserve=yes
      //## end ClECHOExecutorSS%38DB4346004E.public

  protected:
    // Additional Protected Declarations
      //## begin ClECHOExecutorSS%38DB4346004E.protected preserve=yes
        // bugid: 0100528
        static m4int32_t m_iNumOfInstances;
      //## end ClECHOExecutorSS%38DB4346004E.protected

  private:
    // Additional Private Declarations
      //## begin ClECHOExecutorSS%38DB4346004E.private preserve=yes
      //## end ClECHOExecutorSS%38DB4346004E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClECHOExecutorSS%38DB4346004E.implementation preserve=yes
      //## end ClECHOExecutorSS%38DB4346004E.implementation

};

//## begin ClECHOExecutorSS%38DB4346004E.postscript preserve=yes
//## end ClECHOExecutorSS%38DB4346004E.postscript

// Class ClECHOServiceSS 

// Class ClECHOExecutorSS 

//## begin module%38DB44E0002F.epilog preserve=yes
//## end module%38DB44E0002F.epilog


#endif
