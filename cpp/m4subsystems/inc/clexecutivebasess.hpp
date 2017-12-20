//## begin module%37D75EBD03C6.cm preserve=no
//## end module%37D75EBD03C6.cm

//## begin module%37D75EBD03C6.cp preserve=no
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
//## end module%37D75EBD03C6.cp

//## Module: clexecutivebasess%37D75EBD03C6; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\clexecutivebasess.hpp

#ifndef clexecutivebasess_h
#define clexecutivebasess_h 1

//## begin module%37D75EBD03C6.additionalIncludes preserve=no
//## end module%37D75EBD03C6.additionalIncludes

//## begin module%37D75EBD03C6.includes preserve=yes
//## end module%37D75EBD03C6.includes

// m4string
#include <m4string.hpp>
// LauncherInterface
#include <launcher.hpp>
// queuebasedss
#include <queuebasedss.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%37D75EBD03C6.declarations preserve=no
//## end module%37D75EBD03C6.declarations

//## begin module%37D75EBD03C6.additionalDeclarations preserve=yes
//## end module%37D75EBD03C6.additionalDeclarations


//## begin ClExecutiveBaseSS%37D75E250089.preface preserve=yes
//## end ClExecutiveBaseSS%37D75E250089.preface

//## Class: ClExecutiveBaseSS%37D75E250089
//	Clase especializada de subsistema pero genérica de subsistema de ejecutor.
//	Sobrecarga las funciones necesarias para tener un comportamiento de un
//	subsistema de ejecutor y en sí misma puede instanciarse ya como tal, pero
//	todo aquel que precise un tipo de subsistema de ejecutor particular, podrá
//	heredar de estra clase para especializarlo aún más.
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SUBSYSTEMS ClExecutiveBaseSS : public ClQueueBasedSS  //## Inherits: <unnamed>%38EAF7AF03C1
{
  //## begin ClExecutiveBaseSS%37D75E250089.initialDeclarations preserve=yes
  //## end ClExecutiveBaseSS%37D75E250089.initialDeclarations

  public:
    //## begin ClExecutiveBaseSS::eExecutiveBaseCommands%37FA37DF02B8.preface preserve=yes
    //## end ClExecutiveBaseSS::eExecutiveBaseCommands%37FA37DF02B8.preface

    //## Class: eExecutiveBaseCommands%37FA37DF02B8
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {GET_LOG_OF_LAST_EXECUTION = ClSSImplementation::TOTAL_COMMANDS, CAPTURE_EXCEPTIONS , DONT_CAPTURE_EXCEPTIONS , GET_USED_MEMORY, TOTAL_COMMANDS } eExecutiveBaseCommands;

    //## begin ClExecutiveBaseSS::eExecutiveBaseCommands%37FA37DF02B8.postscript preserve=yes
    //## end ClExecutiveBaseSS::eExecutiveBaseCommands%37FA37DF02B8.postscript

    //## Constructors (specified)
      //## Operation: ClExecutiveBaseSS%936860991
      //	constructor de la clase
      ClExecutiveBaseSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveBaseSS%936860992
      ~ClExecutiveBaseSS ();


    //## Other Operations (specified)
      //## Operation: GetLauncher%938596862
      //	retorna la variable miembro m_poLauncher que es el Launcher que lleva
      //	asociado el subsistema
      ClLauncherInterface * GetLauncher ();

      //## Operation: SetUseExceptions%940264617
      //	Habilita o deshabilita las excepciones para el thread asociado al subsistema
      void SetUseExceptions (m4bool_t ai_bException);

      //## Operation: GetUseExceptions%940264618
      //	Retorna si el thread asociado al subsistema tiene activada la captura d
      //	eexcepciones o no.
      m4bool_t GetUseExceptions ();

      //## Operation: ExecuteCommand%940321346
      //	Ejecución de comandos del subsistema de ejecutor
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%940321347
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%940321348
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetLogOfLastExecution%941014432
      //	Devuelve la los de la última ejecución del thread correspondiente al
      //	ejecutor asociado al subsistema
      m4return_t GetLogOfLastExecution (m4pchar_t &ao_pBuffer);

      //## Operation: GetElement%942682763
      virtual void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: Start%955100216
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      m4return_t Start ();

      //## Operation: Initialize%957518548
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

      //## Operation: IsReadyToClose%962263775
      //	Durante el proceso de ShutDown, este métodose llamará para todos los
      //	subsistemas con objeto de verificar el momento en el que sea posible pasar a
      //	la destrucción de todos los subsistemas.
      //	Aquel subsistema que precise de una deshabitación previa a la destucción,
      //	como puede ser la finalización de los threads asociados, hará esto en un
      //	paso previo (Stop) y a la llamada de este método, devolverá true o false
      //	dependiendo de si el thread a finalizado ya o no, es decr dependiendo de si
      //	el subsistema está listo o no para ser destruido.
      virtual m4bool_t IsReadyToClose ();

      //## Operation: Stop%962287219
      //	Cuando se inicaia el proceso de ShutDown se notifica a todos los subsistemas
      //	mediante este método. Como parámetro se pasa en segundos el timepo que queda
      //	hasta el shutdown definitivo y cada subsistema debe de ir haciendo en los
      //	pasos previos según corresponda las aaciones pertinentes para dejar todo
      //	consistente antes de ese momento o al menos en la última llamada (tiempo = 0)
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: Reply%967113584
      virtual m4return_t Reply (ClBaseRequest *ai_poRequest);

	  // Indica que el subsistema es un ejecutor
      virtual m4bool_t IsExecutor( void )
	  {
		  return( M4_TRUE ) ;
	  }

  public:
    // Additional Public Declarations
      //## begin ClExecutiveBaseSS%37D75E250089.public preserve=yes
      //## end ClExecutiveBaseSS%37D75E250089.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_szInstanceOrder%3912E5DD00BD
      //## begin ClExecutiveBaseSS::m_szInstanceOrder%3912E5DD00BD.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_szInstanceOrder;
      //## end ClExecutiveBaseSS::m_szInstanceOrder%3912E5DD00BD.attr

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37D762060221
      //## Role: ClExecutiveBaseSS::m_poLauncher%37D762070074
      //## begin ClExecutiveBaseSS::m_poLauncher%37D762070074.role preserve=no  protected: ClLauncherInterface { -> RHAN}
      ClLauncherInterface *m_poLauncher;
      //## end ClExecutiveBaseSS::m_poLauncher%37D762070074.role

      //## Association: M4Subsystems::<unnamed>%3912D9FC0240
      //## Role: ClExecutiveBaseSS::m_strStringType%3912D9FD0043
      //## begin ClExecutiveBaseSS::m_strStringType%3912D9FD0043.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strStringType;
      //## end ClExecutiveBaseSS::m_strStringType%3912D9FD0043.role

      //## Association: M4Subsystems::<unnamed>%3922C2A700CE
      //## Role: ClExecutiveBaseSS::m_poServiceSS%3922C2A800F7
      //## begin ClExecutiveBaseSS::m_poServiceSS%3922C2A800F7.role preserve=no  protected: ClServiceBaseSS { -> RHAN}
      ClServiceBaseSS *m_poServiceSS;
      //## end ClExecutiveBaseSS::m_poServiceSS%3922C2A800F7.role

    // Additional Protected Declarations
      //## begin ClExecutiveBaseSS%37D75E250089.protected preserve=yes
      //## end ClExecutiveBaseSS%37D75E250089.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bUseExceptions%380B4C2D022C
      //	Es una variable miembro boleana indicativa de si en el thread del ejecutor
      //	contenido en el subsistema se capturan las excepciones o no.
      //## begin ClExecutiveBaseSS::m_bUseExceptions%380B4C2D022C.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bUseExceptions;
      //## end ClExecutiveBaseSS::m_bUseExceptions%380B4C2D022C.attr

    // Additional Private Declarations
      //## begin ClExecutiveBaseSS%37D75E250089.private preserve=yes
      //## end ClExecutiveBaseSS%37D75E250089.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveBaseSS%37D75E250089.implementation preserve=yes
      //## end ClExecutiveBaseSS%37D75E250089.implementation

};

//## begin ClExecutiveBaseSS%37D75E250089.postscript preserve=yes
//## end ClExecutiveBaseSS%37D75E250089.postscript

// Class ClExecutiveBaseSS 

//## begin module%37D75EBD03C6.epilog preserve=yes
//## end module%37D75EBD03C6.epilog


#endif
