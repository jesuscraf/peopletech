//## begin module%342013CF0308.cm preserve=no
//## end module%342013CF0308.cm

//## begin module%342013CF0308.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Executive
//	 File:				Excutive.hpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				970930
//	 Language:			C++
//	 Operating System:	WINDOWS NT, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%342013CF0308.cp

//## Module: Executive%342013CF0308; Package specification
//## Subsystem: M4Executive::inc%379462FF024C
//## Source file: F:\integration\m4executive\inc\executive.hpp

#ifndef executive_h
#define executive_h 1

//## begin module%342013CF0308.additionalIncludes preserve=no
//## end module%342013CF0308.additionalIncludes

//## begin module%342013CF0308.includes preserve=yes
#include <m4executive_dll.hpp>
#ifdef _KCC
using std::iostream;
#endif
//## end module%342013CF0308.includes

// ImsgContainer
#include <imsgcontainer.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// Statistics
#include <statistics.hpp>
// ExVariables
#include <exvariables.hpp>
// DataStorages
#include <datastorages.hpp>
// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// BlockSynchronization
#include <blocksynchronization.hpp>
// USInterface
#include <usinterface.hpp>

class ClConfiguration;
class ClNavigatorFirewall;
class ClBooleanCondition;
class ClChannel;

//## begin module%342013CF0308.declarations preserve=no
//## end module%342013CF0308.declarations

//## begin module%342013CF0308.additionalDeclarations preserve=yes
//## end module%342013CF0308.additionalDeclarations


//## begin ClEngineArgs%343273B3022B.preface preserve=yes
//## end ClEngineArgs%343273B3022B.preface

//## Class: ClEngineArgs%343273B3022B
//	Parámetros de entrada a la operation determinantes para que ésta pueda ser
//	ejecutada. Forman parte del request enviado por el cliente.
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EXECUTIVE ClEngineArgs 
{
  //## begin ClEngineArgs%343273B3022B.initialDeclarations preserve=yes
  //## end ClEngineArgs%343273B3022B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEngineArgs%879963494; C++
      ClEngineArgs (M4DataStorage *ai_pInData, M4DataStorage *ai_pOutData);


    //## Other Operations (specified)
      //## Operation: GetInputData%884795082; C++
      m4return_t GetInputData (M4DataStorage *&ao_pInData) const;

      //## Operation: GetOutputData%884854922; C++
      m4return_t GetOutputData (M4DataStorage *&ao_pOutData);

    // Additional Public Declarations
      //## begin ClEngineArgs%343273B3022B.public preserve=yes
      //## end ClEngineArgs%343273B3022B.public

  protected:
    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%37946F5D017C
      //## Role: ClEngineArgs::m_pInputData%37946F5D03D5
      //## begin ClEngineArgs::m_pInputData%37946F5D03D5.role preserve=no  protected: M4DataStorage {1 -> 1RHAN}
      M4DataStorage *m_pInputData;
      //## end ClEngineArgs::m_pInputData%37946F5D03D5.role

      //## Association: M4Executive::<unnamed>%37946FAB00F2
      //## Role: ClEngineArgs::m_pOutputData%37946FAB03C3
      //## begin ClEngineArgs::m_pOutputData%37946FAB03C3.role preserve=no  protected: M4DataStorage {1 -> 1RHAN}
      M4DataStorage *m_pOutputData;
      //## end ClEngineArgs::m_pOutputData%37946FAB03C3.role

    // Additional Protected Declarations
      //## begin ClEngineArgs%343273B3022B.protected preserve=yes
      //## end ClEngineArgs%343273B3022B.protected

  private:
    // Additional Private Declarations
      //## begin ClEngineArgs%343273B3022B.private preserve=yes
      //## end ClEngineArgs%343273B3022B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEngineArgs%343273B3022B.implementation preserve=yes
      //## end ClEngineArgs%343273B3022B.implementation

};

//## begin ClEngineArgs%343273B3022B.postscript preserve=yes
//## end ClEngineArgs%343273B3022B.postscript

//## begin ClExecSerializableObject%3471524B032C.preface preserve=yes
//## end ClExecSerializableObject%3471524B032C.preface

//## Class: ClExecSerializableObject%3471524B032C
//	This class enables objects to be de/serialized.
//	This should be called by ClExecutive to process back and forward serialized
//	data in a previous step to execute the engine. The executive class sould
//	also initialize each relevant object using the Serialize() and Deserialize()
//	member funcitons and initialize the class passing as argument a kind of
//	container like a stream.
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34715FF50209; { -> }

class ClExecSerializableObject 
{
  //## begin ClExecSerializableObject%3471524B032C.initialDeclarations preserve=yes
  //## end ClExecSerializableObject%3471524B032C.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Serialize%879931901
      virtual m4return_t Serialize () = 0;

      //## Operation: Deserialize%879931902
      virtual m4return_t Deserialize () = 0;

      //## Operation: GetIOStream%880110467
      virtual iostream* GetIOStream () = 0;

    // Additional Public Declarations
      //## begin ClExecSerializableObject%3471524B032C.public preserve=yes
      //## end ClExecSerializableObject%3471524B032C.public

  protected:
    //## Constructors (specified)
      //## Operation: ClExecSerializableObject%879931900
      ClExecSerializableObject (iostream* p);

    // Additional Protected Declarations
      //## begin ClExecSerializableObject%3471524B032C.protected preserve=yes
	  iostream *m_pIOStream;
      //## end ClExecSerializableObject%3471524B032C.protected
  private:
    // Additional Private Declarations
      //## begin ClExecSerializableObject%3471524B032C.private preserve=yes
      //## end ClExecSerializableObject%3471524B032C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecSerializableObject%3471524B032C.implementation preserve=yes
      //## end ClExecSerializableObject%3471524B032C.implementation

};

//## begin ClExecSerializableObject%3471524B032C.postscript preserve=yes
//## end ClExecSerializableObject%3471524B032C.postscript

//## Class: ClEngineContext%34310EA800E9
//	This object must be created by the LocalMonitor and attached to the Cl
//	Request object in order for the Executive to have access to it, build up the
//	VM arguments and let the ClVirtualMachine object update it on every new
//	operation sequence.
//	No public interface or internal structure has been design so far. This is
//	because no analysis has been made about the requirements of the classes that
//	will have to use this context. For the sake of flexibility some options may
//	be considered being the OBL architecture an appropriatte starting point.
//	Remarks
//	Se refiere a un conjunto de datos que contienen el estado de la operation
//	tras una ejecución de alguna de sus partes. Sólo tiene sentido, por lo
//	tanto, para operations con estado, esto es, operaciones que se subdividan en
//	suboperaciones más pequeñas cada una de las cuales forma un request.
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3562A98701A4;m4handle_t { -> }

typedef m4handle_t ClEngineContext;

//## Class: ClEngineExecEnv%34311FD90175
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3561C33C009C;ClChannel { -> F}
//## Uses: <unnamed>%373998AE007F;ClUSSessionInterface { -> }

typedef ClUSSessionInterface ClEngineExecEnv;

//## begin ClImage%3430E9EE031B.preface preserve=yes
//## end ClImage%3430E9EE031B.preface

//## Class: ClImage%3430E9EE031B
//	This class is bound to be used as parameter for ClVirtualMachine objects. It
//	must store all information concerning execution of operations. It can serve
//	either as the output parameter to return contextual information related to
//	the status of the whole Virtual Machine after execution. This is
//	particularly important when executing state-based operations as this context
//	must be stored in the LocalMonitor for retrieval when the next
//	operation-in-sequence is ready to be executed.
//	Remarks
//	Es un objeto compuesto por diferentes informaciones que permite a un engine
//	ejecutar una operación completamente. Representa, por tanto, la lógica de la
//	petición que se envía desde el cliente. Algunos de los datos que pueden
//	incluirse en este objeto son: el context (cuando la operation es con
//	estados, ya que sin estados el context es único), los arguments de la
//	operation pedida, un channels cache y un dispositivo de salida que, en
//	principio, llamaremos log
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EXECUTIVE ClImage 
{
  //## begin ClImage%3430E9EE031B.initialDeclarations preserve=yes
  //## end ClImage%3430E9EE031B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClImage%876139086; C++
      ClImage (ClEngineArgs *  ai_pArgs = NULL, ClEngineExecEnv *ai_pEngineEnv = NULL, ClStatus *ai_pStatus = NULL, ClStatistic *ai_pStats = NULL);


    //## Other Operations (specified)
      //## Operation: GetArgs%884623038; C++
      void GetArgs (ClEngineArgs *&ao_pArgs) const;

      //## Operation: SetArgs%943284077; C++
      void SetArgs (ClEngineArgs *ai_pArgs);

      //## Operation: GetStats%890222217; C++
      void GetStats (ClStatistic *&ao_pStats) const;

      //## Operation: SetStats%943284078; C++
      void SetStats (ClStatistic *ai_pStats);

      //## Operation: GetEnv%895580956; C++
      void GetEnv (ClEngineExecEnv *&ao_pEngineEnv) const;

      //## Operation: SetEnv%896353044; C++
      void SetEnv (ClEngineExecEnv *ai_pEngineEnv);

      //## Operation: GetStatus%901734574; C++
      void GetStatus (ClStatus *&ao_pStatus) const;

      //## Operation: SetStatus%901734575; C++
      void SetStatus (ClStatus *ai_pStatus);

      //## Operation: Reset%943350231
      void Reset ();

	  void GetReqUniqueId (m4string_t &ao_sReqUniqueId) const;

	  void SetReqUniqueId (m4string_t &ai_sReqUniqueId);

    // Additional Public Declarations
      //## begin ClImage%3430E9EE031B.public preserve=yes
      //## end ClImage%3430E9EE031B.public

  protected:
    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%37946B20010E
      //## Role: ClImage::m_pArgs%37946B210119
      //## begin ClImage::m_pArgs%37946B210119.role preserve=no  protected: ClEngineArgs {1 -> 1RHAN}
      ClEngineArgs *m_pArgs;
      //## end ClImage::m_pArgs%37946B210119.role

      //## Association: M4Executive::<unnamed>%3794704703A0
      //## Role: ClImage::m_pEnvironment%3794704801FC
      //## begin ClImage::m_pEnvironment%3794704801FC.role preserve=no  protected: ClEngineExecEnv {1 -> 1RHAN}
      ClEngineExecEnv *m_pEnvironment;
      //## end ClImage::m_pEnvironment%3794704801FC.role

      //## Association: M4Executive::<unnamed>%379470A103B3
      //## Role: ClImage::m_pStatus%379470A20314
      //## begin ClImage::m_pStatus%379470A20314.role preserve=no  protected: ClStatus {1 -> 1RHAN}
      ClStatus *m_pStatus;
      //## end ClImage::m_pStatus%379470A20314.role

      //## Association: M4Executive::<unnamed>%379754F50009
      //## Role: ClImage::m_pStats%379754F50187
      //## begin ClImage::m_pStats%379754F50187.role preserve=no  protected: ClStatistic { -> 1RHAN}
      ClStatistic *m_pStats;
      //## end ClImage::m_pStats%379754F50187.role

	  m4string_t m_sReqUniqueId;

    // Additional Protected Declarations
      //## begin ClImage%3430E9EE031B.protected preserve=yes
      //## end ClImage%3430E9EE031B.protected

  private:
    // Additional Private Declarations
      //## begin ClImage%3430E9EE031B.private preserve=yes
      //## end ClImage%3430E9EE031B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClImage%3430E9EE031B.implementation preserve=yes
      //## end ClImage%3430E9EE031B.implementation

};

//## begin ClImage%3430E9EE031B.postscript preserve=yes
//## end ClImage%3430E9EE031B.postscript

//## Class: ClEngine%3432761A0056
//	Representa al menos una máquina virtual y otros datos colaterales que puedan
//	ir unidos a la propia máquina virtual de por vida. Cada objeto executive
//	tiene una única máquina virtual durante el tiempo en el que se ejecuta una
//	request.
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3438F6380348;ClImage { -> }

class M4_DECL_M4EXECUTIVE ClEngine 
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClEngine%915791251
      virtual ~ClEngine ();


    //## Other Operations (specified)
      //## Operation: Process%876139087
      virtual bool Process (ClImage &a_oImage) = 0;

      //## Operation: Init%893680778; C++
      virtual m4return_t Init () = 0;

      //## Operation: End%893680779
      virtual m4return_t End () = 0;

  protected:
    //## Constructors (specified)
      //## Operation: ClEngine%877616177
      ClEngine (ClStatistic *ai_poStats = NULL);

    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%392AAE520123
      //## Role: ClEngine::m_poStats%392AAE52039A
      //## begin ClEngine::m_poStats%392AAE52039A.role preserve=no  protected: ClStatistic { -> 1RHAN}
      ClStatistic *m_poStats;
      //## end ClEngine::m_poStats%392AAE52039A.role

  private:
  private: //## implementation
};

//## Class: ClExecutive%3430E38A006C
//	Es el objeto que ejecuta la operation en función de los parámetros
//	especificados en la request. Como veremos mas adelante, existen diferentes
//	aproximaciones para abordar la implementación de una jerarquía de executives
//	que cumplan los requisitos de escalabilidad, flexibilidad y rendimiento a
//	los que se ha aludido anteriormente. Por tanto no se abundará mas en su
//	composición aunque se pueden diferenciar dos elementos deferenciados: el
//	conjunto de acciones, atributos y condiciones que determinan QUÉ se espera
//	de dicha operación (image) y la plataforma sobre la que se despliega todo el
//	"software" incluido en el objeto image (engine).
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3430EA290379;ClImage { -> }
//## Uses: <unnamed>%3471528A0296;ClExecSerializableObject { -> }
//## Uses: <unnamed>%3562AB46033A;ClCacheTest { -> }
//## Uses: <unnamed>%3562B2A603B7;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%3694E8E7033A;ClConfiguration { -> F}
//## Uses: <unnamed>%36F7B28D00E4;ClAdminTag { -> }
//## Uses: <unnamed>%392AAD770287;ClMutBlock { -> }

class M4_DECL_M4EXECUTIVE ClExecutive 
{
  public:
    //## begin ClExecutive::eExecutiveCommands%36F7B2230087.preface preserve=yes
    //## end ClExecutive::eExecutiveCommands%36F7B2230087.preface

    //## Class: eExecutiveCommands%36F7B2230087; Instantiated Class
    //## Category: M4Executive%35F65F3D007A
    //## Subsystem: M4Executive::inc%379462FF024C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {QUIT = 0, PAUSE, RESUME, USED_MEMORY, TOTAL_COMMANDS} eExecutiveCommands;

    //## begin ClExecutive::eExecutiveCommands%36F7B2230087.postscript preserve=yes
    //## end ClExecutive::eExecutiveCommands%36F7B2230087.postscript

    //## begin ClExecutive::eOwnState%3B0A748F021D.preface preserve=yes
    //## end ClExecutive::eOwnState%3B0A748F021D.preface

    //## Class: eOwnState%3B0A748F021D
    //## Category: M4Executive%35F65F3D007A
    //## Subsystem: M4Executive%35F65F020237
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { STATE_ACTIVE , STATE_HALTED} eOwnState;

    //## begin ClExecutive::eOwnState%3B0A748F021D.postscript preserve=yes
    //## end ClExecutive::eOwnState%3B0A748F021D.postscript

    //## Destructor (specified)
      //## Operation: ~ClExecutive%875605501
      virtual ~ClExecutive ();


    //## Other Operations (specified)
      //## Operation: StartUp%875605502
      virtual m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL) = 0;

      //## Operation: Execute%875605503
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      virtual m4return_t Execute (ClImage &aio_pImage) = 0;

      //## Operation: ShutDown%875605504
      virtual m4return_t ShutDown () = 0;

      //## Operation: Reset%880392786; C++
      virtual m4return_t Reset () = 0;

      //## Operation: GetNDumpLevels%921058931
      virtual m4int16_t GetNDumpLevels () const;

      //## Operation: DumpLevel%921058932; C++
      virtual m4return_t DumpLevel (const m4int16_t ai_iLevel);

      //## Operation: ExecuteCommand%922189095
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: ReleaseResources%927196072; C++
      virtual m4return_t ReleaseResources ();

      //## Operation: SetSession%932660209
      virtual void SetSession (ClUSSessionInterface *ai_poSession);

      //## Operation: GetSession%932660210
      virtual ClUSSessionInterface * GetSession () const;

      //## Operation: GetStatus%982580499
      m4int16_t GetStatus ();

      //## Operation: AddTraceMessage%986914179
      m4return_t AddTraceMessage (m4string_t &ai_strMessage) const;

      //## Operation: SetMsgContainer%986914180
      void SetMsgContainer (ClMessageContainer *ai_poMsgContainer);

      //## Operation: GetOwnState%990541046
      ClExecutive::eOwnState GetOwnState ();

      //## Operation: SetOwnState%990541047
      m4return_t SetOwnState (ClExecutive::eOwnState ai_eState);

  public:
  protected:
    //## Constructors (specified)
      //## Operation: ClExecutive%875605500; C++
      ClExecutive (ClEngine *ai_pEngine, ClStatistic *ai_poStats = NULL);

    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%343276290008
      //## Role: ClExecutive::m_pEngine%3432762901DC
      //## begin ClExecutive::m_pEngine%3432762901DC.role preserve=no  protected: ClEngine {1 -> 1RHAN}
      ClEngine *m_pEngine;
      //## end ClExecutive::m_pEngine%3432762901DC.role

      //## Association: M4Executive::<unnamed>%3797456C019B
      //## Role: ClExecutive::m_bInitialized%3797456C0354
      //## begin ClExecutive::m_bInitialized%3797456C0354.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bInitialized;
      //## end ClExecutive::m_bInitialized%3797456C0354.role

      //## Association: M4Executive::<unnamed>%379745AC002B
      //## Role: ClExecutive::m_bMutInit%379745AC0175
      //## begin ClExecutive::m_bMutInit%379745AC0175.role preserve=no  protected: static m4bool_t { -> 1VHAN}
      static m4bool_t m_bMutInit;
      //## end ClExecutive::m_bMutInit%379745AC0175.role

      //## Association: M4Executive::<unnamed>%379746F000E5
      //## Role: ClExecutive::m_poSession%379746F0030C
      //## begin ClExecutive::m_poSession%379746F0030C.role preserve=no  protected: ClUSSessionInterface { -> 1RHAN}
      ClUSSessionInterface *m_poSession;
      //## end ClExecutive::m_poSession%379746F0030C.role

      //## Association: M4Executive::<unnamed>%3797553D00A3
      //## Role: ClExecutive::m_poStatis%3797553D02F2
      //## begin ClExecutive::m_poStatis%3797553D02F2.role preserve=no  protected: ClStatistic { -> 1RFHAN}
      ClStatistic *m_poStatis;
      //## end ClExecutive::m_poStatis%3797553D02F2.role

      //## Association: M4Executive::<unnamed>%381569EC0090
      //## Role: ClExecutive::m_oMutAccess%381569EC0361
      //	Este mutex nos permite bloquear el acceso al "ClStatus".
      //	Tambien es el mutex que se utiliza para inicializar el "ClBooleanCondition".
      //## begin ClExecutive::m_oMutAccess%381569EC0361.role preserve=no  protected: ClCriticalSection { -> 1VHAN}
      ClCriticalSection m_oMutAccess;
      //## end ClExecutive::m_oMutAccess%381569EC0361.role

      //## Association: M4Executive::<unnamed>%38156F21034A
      //## Role: ClExecutive::m_pStatus%38156F22030F
      //	Estado de la ejecucion.
      //## begin ClExecutive::m_pStatus%38156F22030F.role preserve=no  protected: ClStatus { -> 1RFHAN}
      ClStatus *m_pStatus;
      //## end ClExecutive::m_pStatus%38156F22030F.role

      //## Association: M4Executive::<unnamed>%3815700A0327
      //## Role: ClExecutive::m_pCondIntrVal%3815700B0224
      //	Nos va a permitir implementar el "PAUSE" y el "RESUME".
      //	Se lo asignamos al ClStatus antes de empezar la ejecución. Y se lo quitamos
      //	despues de la misma.
      //	Es unico por ejecutor.
      //## begin ClExecutive::m_pCondIntrVal%3815700B0224.role preserve=no  protected: ClBooleanCondition { -> 1RFHAN}
      ClBooleanCondition *m_pCondIntrVal;
      //## end ClExecutive::m_pCondIntrVal%3815700B0224.role

      //## Association: M4Executive::<unnamed>%392AAD66035F
      //## Role: ClExecutive::m_oMut%392AAD660360
      //## begin ClExecutive::m_oMut%392AAD660360.role preserve=no  protected: static ClCriticalSection { -> 1VHAN}
      static ClCriticalSection m_oMut;
      //## end ClExecutive::m_oMut%392AAD660360.role

      //## Association: M4Executive::<unnamed>%3AD32433028E
      //## Role: ClExecutive::m_poMsgContainer%3AD324340127
      //## begin ClExecutive::m_poMsgContainer%3AD324340127.role preserve=no  protected: ClMessageContainer { -> RHAN}
      ClMessageContainer *m_poMsgContainer;
      //## end ClExecutive::m_poMsgContainer%3AD324340127.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_eOwnState%3B0A75090056
      //## begin ClExecutive::m_eOwnState%3B0A75090056.attr preserve=no  private: eOwnState {UA} STATE_ACTIVE
      eOwnState m_eOwnState;
      //## end ClExecutive::m_eOwnState%3B0A75090056.attr

  private: //## implementation
};

// Class ClEngineArgs 

// Class ClExecSerializableObject 

inline ClExecSerializableObject::ClExecSerializableObject (iostream* p)
  //## begin ClExecSerializableObject::ClExecSerializableObject%879931900.hasinit preserve=no
  //## end ClExecSerializableObject::ClExecSerializableObject%879931900.hasinit
  //## begin ClExecSerializableObject::ClExecSerializableObject%879931900.initialization preserve=yes
  : m_pIOStream(p)
  //## end ClExecSerializableObject::ClExecSerializableObject%879931900.initialization
{
  //## begin ClExecSerializableObject::ClExecSerializableObject%879931900.body preserve=yes
  //## end ClExecSerializableObject::ClExecSerializableObject%879931900.body
}


// Class ClImage 

// Class ClEngine 

inline ClEngine::ClEngine (ClStatistic *ai_poStats)
  //## begin ClEngine::ClEngine%877616177.hasinit preserve=no
  //## end ClEngine::ClEngine%877616177.hasinit
  //## begin ClEngine::ClEngine%877616177.initialization preserve=yes
  //## end ClEngine::ClEngine%877616177.initialization
{
  //## begin ClEngine::ClEngine%877616177.body preserve=yes
  //## end ClEngine::ClEngine%877616177.body
}


// Class ClExecutive 


//## Other Operations (inline)
inline m4int16_t ClExecutive::GetNDumpLevels () const
{
  //## begin ClExecutive::GetNDumpLevels%921058931.body preserve=yes
	return 0;
  //## end ClExecutive::GetNDumpLevels%921058931.body
}

inline m4return_t ClExecutive::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutive::ExecuteCommand%922189095.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutive::ExecuteCommand%922189095.body
}

inline void ClExecutive::SetSession (ClUSSessionInterface *ai_poSession)
{
  //## begin ClExecutive::SetSession%932660209.body preserve=yes
	ClMutBlock	oBlock(m_oMut);
	m_poSession = ai_poSession;
  //## end ClExecutive::SetSession%932660209.body
}

inline ClUSSessionInterface * ClExecutive::GetSession () const
{
  //## begin ClExecutive::GetSession%932660210.body preserve=yes
	ClMutBlock	oBlock(m_oMut);
	return m_poSession;
  //## end ClExecutive::GetSession%932660210.body
}

//## begin module%342013CF0308.epilog preserve=yes
//## end module%342013CF0308.epilog


#endif
