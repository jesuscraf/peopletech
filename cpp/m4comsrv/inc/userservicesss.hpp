//## begin module%38ECB24B03AE.cm preserve=no
//## end module%38ECB24B03AE.cm

//## begin module%38ECB24B03AE.cp preserve=no
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
//## end module%38ECB24B03AE.cp

//## Module: userservicesss%38ECB24B03AE; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\userservicesss.hpp

#ifndef userservicesss_h
#define userservicesss_h 1

//## begin module%38ECB24B03AE.additionalIncludes preserve=no
//## end module%38ECB24B03AE.additionalIncludes

#include <clservicebasess.hpp>
#include <clexecutivebasess.hpp>
#include <sessionidgenerator.hpp>

class ClUSServiceAPI;

//## begin module%38ECB24B03AE.declarations preserve=no
//## end module%38ECB24B03AE.declarations

//## begin module%38ECB24B03AE.additionalDeclarations preserve=yes
//## end module%38ECB24B03AE.additionalDeclarations


//## begin ClUSExecutiveSS%38ECB36B00CD.preface preserve=yes
//## end ClUSExecutiveSS%38ECB36B00CD.preface

//## Class: ClUSExecutiveSS%38ECB36B00CD
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38ECBB94009F;ClExeFactory { -> F}

class ClUSExecutiveSS : public ClExecutiveBaseSS  //## Inherits: <unnamed>%38ECB38001F0
{
  //## begin ClUSExecutiveSS%38ECB36B00CD.initialDeclarations preserve=yes
  //## end ClUSExecutiveSS%38ECB36B00CD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSExecutiveSS%955035286
      ClUSExecutiveSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClUSExecutiveSS%955035287
      ~ClUSExecutiveSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%955035288
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

      //## Operation: Start%955035292
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      m4return_t Start ();

    // Additional Public Declarations
      //## begin ClUSExecutiveSS%38ECB36B00CD.public preserve=yes
      //## end ClUSExecutiveSS%38ECB36B00CD.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSExecutiveSS%38ECB36B00CD.protected preserve=yes
      static m4int32_t m_iNumOfInstances;
      //## end ClUSExecutiveSS%38ECB36B00CD.protected

  private:
    // Additional Private Declarations
      //## begin ClUSExecutiveSS%38ECB36B00CD.private preserve=yes
      //## end ClUSExecutiveSS%38ECB36B00CD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSExecutiveSS%38ECB36B00CD.implementation preserve=yes
      //## end ClUSExecutiveSS%38ECB36B00CD.implementation

};

//## begin ClUSExecutiveSS%38ECB36B00CD.postscript preserve=yes
//## end ClUSExecutiveSS%38ECB36B00CD.postscript

//## begin ClUSServiceSS%3560106502E7.preface preserve=yes
//## end ClUSServiceSS%3560106502E7.preface

//## Class: ClUSServiceSS%3560106502E7
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3755475400F2;ClUSServiceAPI { -> F}
//## Uses: <unnamed>%3778E3DB0294;ClBoomerangRequest { -> F}
//## Uses: <unnamed>%3778E7830195;ClCommandRequest { -> F}
//## Uses: <unnamed>%38ECB3DE0123;ClUSExecutiveSS { -> }

class ClUSServiceSS : public ClServiceBaseSS  //## Inherits: <unnamed>%375544050234
{
  //## begin ClUSServiceSS%3560106502E7.initialDeclarations preserve=yes
  //## end ClUSServiceSS%3560106502E7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSServiceSS%895476052
      ClUSServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClUSServiceSS%895476053
      ~ClUSServiceSS ();


    //## Other Operations (specified)
      //## Operation: Start%927628813
      m4return_t Start ();

      //## Operation: EmptyQueue%930666545
      m4return_t EmptyQueue ();

      //## Operation: Initialize%947603787
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: SetService%954922423
      void SetService (ClUSServiceAPI *ai_poServiceAPI);

      //## Operation: GenerateSubsystem%955035285
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    // Additional Public Declarations
      //## begin ClUSServiceSS%3560106502E7.public preserve=yes
      //## end ClUSServiceSS%3560106502E7.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%39FE7BDD037A
      //## Role: ClUSServiceSS::m_poSessionIDGenerator%39FE7BDE029F
      //## begin ClUSServiceSS::m_poSessionIDGenerator%39FE7BDE029F.role preserve=no  protected: ISessionIDGenerator { -> RHAN}
      ISessionIDGenerator *m_poSessionIDGenerator;
      //## end ClUSServiceSS::m_poSessionIDGenerator%39FE7BDE029F.role

    // Additional Protected Declarations
      //## begin ClUSServiceSS%3560106502E7.protected preserve=yes
      //## end ClUSServiceSS%3560106502E7.protected

  private:
    // Additional Private Declarations
      //## begin ClUSServiceSS%3560106502E7.private preserve=yes
      //## end ClUSServiceSS%3560106502E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSServiceSS%3560106502E7.implementation preserve=yes
      //## end ClUSServiceSS%3560106502E7.implementation

};

//## begin ClUSServiceSS%3560106502E7.postscript preserve=yes
//## end ClUSServiceSS%3560106502E7.postscript

//## begin ClSessChExecutiveSS%38ECB38301F4.preface preserve=yes
//## end ClSessChExecutiveSS%38ECB38301F4.preface

//## Class: ClSessChExecutiveSS%38ECB38301F4
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSessChExecutiveSS : public ClExecutiveBaseSS  //## Inherits: <unnamed>%38ECB39A01A7
{
  //## begin ClSessChExecutiveSS%38ECB38301F4.initialDeclarations preserve=yes
  //## end ClSessChExecutiveSS%38ECB38301F4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSessChExecutiveSS%955035289
      ClSessChExecutiveSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClSessChExecutiveSS%955035290
      ~ClSessChExecutiveSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%955035291
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      m4return_t Initialize ();

      //## Operation: Start%955035293
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      m4return_t Start ();

    // Additional Public Declarations
      //## begin ClSessChExecutiveSS%38ECB38301F4.public preserve=yes
      //## end ClSessChExecutiveSS%38ECB38301F4.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessChExecutiveSS%38ECB38301F4.protected preserve=yes
        // bugid: 0100528
        static m4int32_t m_iNumOfInstances;
      //## end ClSessChExecutiveSS%38ECB38301F4.protected

  private:
    // Additional Private Declarations
      //## begin ClSessChExecutiveSS%38ECB38301F4.private preserve=yes
      //## end ClSessChExecutiveSS%38ECB38301F4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessChExecutiveSS%38ECB38301F4.implementation preserve=yes
      //## end ClSessChExecutiveSS%38ECB38301F4.implementation

};

//## begin ClSessChExecutiveSS%38ECB38301F4.postscript preserve=yes
//## end ClSessChExecutiveSS%38ECB38301F4.postscript

//## begin ClSessChServiceSS%356010B60153.preface preserve=yes
//## end ClSessChServiceSS%356010B60153.preface

//## Class: ClSessChServiceSS%356010B60153
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38ECB3E0031B;ClSessChExecutiveSS { -> }

class ClSessChServiceSS : public ClServiceBaseSS  //## Inherits: <unnamed>%375544030001
{
  //## begin ClSessChServiceSS%356010B60153.initialDeclarations preserve=yes
  //## end ClSessChServiceSS%356010B60153.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSessChServiceSS%895476050
      ClSessChServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSessChServiceSS%895476051
      ~ClSessChServiceSS ();


    //## Other Operations (specified)
      //## Operation: GenerateSubsystem%955035284
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    // Additional Public Declarations
      //## begin ClSessChServiceSS%356010B60153.public preserve=yes
      //## end ClSessChServiceSS%356010B60153.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessChServiceSS%356010B60153.protected preserve=yes
      //## end ClSessChServiceSS%356010B60153.protected

  private:
    // Additional Private Declarations
      //## begin ClSessChServiceSS%356010B60153.private preserve=yes
      //## end ClSessChServiceSS%356010B60153.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessChServiceSS%356010B60153.implementation preserve=yes
      //## end ClSessChServiceSS%356010B60153.implementation

};

//## begin ClSessChServiceSS%356010B60153.postscript preserve=yes
//## end ClSessChServiceSS%356010B60153.postscript

// Class ClUSExecutiveSS 

// Class ClUSServiceSS 

// Class ClSessChExecutiveSS 

// Class ClSessChServiceSS 

//## begin module%38ECB24B03AE.epilog preserve=yes
//## end module%38ECB24B03AE.epilog


#endif
