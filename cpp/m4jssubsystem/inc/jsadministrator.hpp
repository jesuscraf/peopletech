//## begin module%37D537A90028.cm preserve=no
//## end module%37D537A90028.cm

//## begin module%37D537A90028.cp preserve=no
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
//## end module%37D537A90028.cp

//## Module: jsAdministrator%37D537A90028; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\inc\jsadministrator.hpp

#ifndef jsadministrator_h
#define jsadministrator_h 1

//## begin module%37D537A90028.additionalIncludes preserve=no
//## end module%37D537A90028.additionalIncludes

//## begin module%37D537A90028.includes preserve=yes
//## end module%37D537A90028.includes

// syncro
#include <syncro.hpp>
// m4thread
#include <m4thread.hpp>
// m4date
#include <m4date.hpp>
// jsmapjobexe
#include <jsmapjobexe.hpp>
// jsRequest
#include <jsrequest.hpp>

class ClJSSubsystem;


//## begin module%37D537A90028.declarations preserve=no
//## end module%37D537A90028.declarations

//## begin module%37D537A90028.additionalDeclarations preserve=yes
//## end module%37D537A90028.additionalDeclarations


//## begin ClJSAdministrator%37D537650106.preface preserve=yes
//## end ClJSAdministrator%37D537650106.preface

//## Class: ClJSAdministrator%37D537650106
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F9E4B900E6;M4ClTimeStamp { -> }

class ClJSAdministrator : public M4Thread  //## Inherits: <unnamed>%37D537920183
{
  //## begin ClJSAdministrator%37D537650106.initialDeclarations preserve=yes
  //## end ClJSAdministrator%37D537650106.initialDeclarations

  public:
    //## begin ClJSAdministrator::eMemoryStatus%381475950133.preface preserve=yes
    //## end ClJSAdministrator::eMemoryStatus%381475950133.preface

    //## Class: eMemoryStatus%381475950133
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {JSMEMORY_GREEN=0, JSMEMORY_YELLOW, JSMEMORY_RED} eMemoryStatus;

    //## begin ClJSAdministrator::eMemoryStatus%381475950133.postscript preserve=yes
    //## end ClJSAdministrator::eMemoryStatus%381475950133.postscript

    //## begin ClJSAdministrator::eIntervalType%381855C00087.preface preserve=yes
    //## end ClJSAdministrator::eIntervalType%381855C00087.preface

    //## Class: eIntervalType%381855C00087
    //	Nos indica todos los tipos de intervalos que tenemos en el administrador
    //	para luego poder establecerlo y recuperarlo con dos funciones
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { JS_INTERVAL_STRESS_RECOVER = 0,  JS_INTERVAL_RECOVER, JS_INTERVAL_CANCELLATION, JS_INTERVAL_EXPIRATION, JS_INTERVAL_RESOURCE_EXPIRATION} eIntervalType;

    //## begin ClJSAdministrator::eIntervalType%381855C00087.postscript preserve=yes
    //## end ClJSAdministrator::eIntervalType%381855C00087.postscript

    //## Constructors (specified)
      //## Operation: ClJSAdministrator%937559777; C++
      ClJSAdministrator (ClJSSubsystem *ai_pJSSubsystem);

    //## Destructor (specified)
      //## Operation: ~ClJSAdministrator%936966060
      ~ClJSAdministrator ();


    //## Other Operations (specified)
      //## Operation: SetCancelQueue%936690194; C++
      m4return_t SetCancelQueue (ClJSQueue *ai_poQueue);

      //## Operation: SetExecutorQueue%937399895
      m4return_t SetExecutorQueue (ClJSQueue *ai_poQueue);

      //## Operation: SetInputQueue%936690195; C++
      m4return_t SetInputQueue (ClJSQueue *ai_poQueue);

      //## Operation: GetCancelQueue%936690196
      ClJSQueue * GetCancelQueue () const;

      //## Operation: GetInputQueue%936690197; C++
      ClJSQueue * GetInputQueue () const;

      //## Operation: Run%936690198
      void Run ();

      //## Operation: GetInterval%938709944
      m4return_t GetInterval (m4uint32_t ai_uiIntervalType, m4uint32_t &ao_uiInterval);

      //## Operation: SetInterval%938709949
      m4return_t SetInterval (m4uint32_t ai_uiIntervalType, m4uint32_t ai_uiInterval);

      //## Operation: GetExecutorQueue%937399896
      ClJSQueue * GetExecutorQueue () const;

      //## Operation: GetNextStopDate%941120812
      m4return_t GetNextStopDate (M4ClTimeStamp &ao_oTime);

      //## Operation: GetNextStartDate%941120813
      m4return_t GetNextStartDate (M4ClTimeStamp &ao_oTime);

  public:
    // Additional Public Declarations
      //## begin ClJSAdministrator%37D537650106.public preserve=yes
      //## end ClJSAdministrator%37D537650106.public

  protected:
    //## Constructors (specified)
      //## Operation: ClJSAdministrator%936690199
      ClJSAdministrator ();


    //## Other Operations (specified)
      //## Operation: ProcessClockTick%937559774; C++
      //	Mira si hay que realizar alguna operación planificada y devuelve el tiempo
      //	que falta para realizar la siguiente operación.
      m4uint32_t ProcessClockTick ();

      //## Operation: BeginJob%939122283
      m4return_t BeginJob ();

      //## Operation: EndJob%939122284
      m4return_t EndJob ();

      //## Operation: ExecutorCreated%939122285
      m4return_t ExecutorCreated ();

      //## Operation: ExecutorDeleted%939912563
      m4return_t ExecutorDeleted ();

      //## Operation: TerminateJS%939213538
      m4return_t TerminateJS ();

      //## Operation: EndJS%939213539; C++
      m4return_t EndJS (M4ClTimeStamp *ai_poTime = NULL);

      //## Operation: BeginJS%939213540; C++
      m4return_t BeginJS (M4ClTimeStamp *ai_poTime = NULL);

      //## Operation: NewJob%939213541
      m4return_t NewJob ();

      //## Operation: EndNewJobs%939213542
      m4return_t EndNewJobs ();

      //## Operation: ProcessCommands%940853232
      m4void_t ProcessCommands ();

      //## Operation: ChangeMemoryStatus%940853236
      m4return_t ChangeMemoryStatus (eMemoryStatus ai_eMemStatus);

      //## Operation: InitializedRecover%941043667
      m4void_t InitializedRecover ();

      //## Operation: SetNextStopDate%941120814
      m4return_t SetNextStopDate (M4ClTimeStamp *ai_poTime);

      //## Operation: SetNextStartDate%941120815
      m4return_t SetNextStartDate (M4ClTimeStamp *ai_poTime);

    // Data Members for Class Attributes

      //## Attribute: m_oTimeNextCancel%37DCF13602DF
      //	Indica cuando es la siguiente hora para ir a buscar cancelaciones.
      //## begin ClJSAdministrator::m_oTimeNextCancel%37DCF13602DF.attr preserve=no  protected: M4ClTimeStamp {UA} 
      M4ClTimeStamp m_oTimeNextCancel;
      //## end ClJSAdministrator::m_oTimeNextCancel%37DCF13602DF.attr

      //## Attribute: m_uiIntervalCancel%37F1FFF003CD
      //	Intervalo de cada cuanto tiempo en segundos, hay que ir a buscar
      //	cancelaciones
      //## begin ClJSAdministrator::m_uiIntervalCancel%37F1FFF003CD.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIntervalCancel;
      //## end ClJSAdministrator::m_uiIntervalCancel%37F1FFF003CD.attr

      //## Attribute: m_oTimeNextExpired%37DE33560294
      //	Siguiente fecha para ver si hay jobs expirados
      //## begin ClJSAdministrator::m_oTimeNextExpired%37DE33560294.attr preserve=no  protected: M4ClTimeStamp {UA} 
      M4ClTimeStamp m_oTimeNextExpired;
      //## end ClJSAdministrator::m_oTimeNextExpired%37DE33560294.attr

      //## Attribute: m_uiIntervalExpired%37F2000F016F
      //	Intervalo de expiración en segundos.
      //## begin ClJSAdministrator::m_uiIntervalExpired%37F2000F016F.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIntervalExpired;
      //## end ClJSAdministrator::m_uiIntervalExpired%37F2000F016F.attr

      //## Attribute: m_oTimeNextExpiredRes%37DE335E02DC
      //	Siguiente fecha para ver las expiraciones de recursos.
      //## begin ClJSAdministrator::m_oTimeNextExpiredRes%37DE335E02DC.attr preserve=no  protected: M4ClTimeStamp {UA} 
      M4ClTimeStamp m_oTimeNextExpiredRes;
      //## end ClJSAdministrator::m_oTimeNextExpiredRes%37DE335E02DC.attr

      //## Attribute: m_uiIntervalExpiredRes%37F200150222
      //	Intervalo de expiraciones de recursos en segundos.
      //## begin ClJSAdministrator::m_uiIntervalExpiredRes%37F200150222.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIntervalExpiredRes;
      //## end ClJSAdministrator::m_uiIntervalExpiredRes%37F200150222.attr

      //## Attribute: m_oTimeNextRecover%37DE33670054
      //	Siguiente fecha para ir a buscar trabajos.
      //## begin ClJSAdministrator::m_oTimeNextRecover%37DE33670054.attr preserve=no  protected: M4ClTimeStamp {UA} 
      M4ClTimeStamp m_oTimeNextRecover;
      //## end ClJSAdministrator::m_oTimeNextRecover%37DE33670054.attr

      //## Attribute: m_uiIntervalRecover%37F2001D0256
      //	Intervalo de recuperacion de trabajos.
      //## begin ClJSAdministrator::m_uiIntervalRecover%37F2001D0256.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIntervalRecover;
      //## end ClJSAdministrator::m_uiIntervalRecover%37F2001D0256.attr

      //## Attribute: m_uiIntervalRecoverStress%37F21EA902BE
      //	Intervalo de recuperacion de trabajos para recuperación rápida. Porque
      //	sabemos que quedan trabajos por ejecutar.
      //## begin ClJSAdministrator::m_uiIntervalRecoverStress%37F21EA902BE.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIntervalRecoverStress;
      //## end ClJSAdministrator::m_uiIntervalRecoverStress%37F21EA902BE.attr

      //## Attribute: m_eMemoryStatus%37E0D39E01E8
      //	VERDE
      //	AMARILLO
      //	ROJO
      //## begin ClJSAdministrator::m_eMemoryStatus%37E0D39E01E8.attr preserve=no  protected: eMemoryStatus {UA} 
      eMemoryStatus m_eMemoryStatus;
      //## end ClJSAdministrator::m_eMemoryStatus%37E0D39E01E8.attr

      //## Attribute: m_bInmediateRecovery%37E0D4230239
      //## begin ClJSAdministrator::m_bInmediateRecovery%37E0D4230239.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bInmediateRecovery;
      //## end ClJSAdministrator::m_bInmediateRecovery%37E0D4230239.attr

      //## Attribute: m_uiShutdownRequested%37E0D4480070
      //## begin ClJSAdministrator::m_uiShutdownRequested%37E0D4480070.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiShutdownRequested;
      //## end ClJSAdministrator::m_uiShutdownRequested%37E0D4480070.attr

      //## Attribute: m_bRecoveringJobs%37E0D45F00A5
      //## begin ClJSAdministrator::m_bRecoveringJobs%37E0D45F00A5.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bRecoveringJobs;
      //## end ClJSAdministrator::m_bRecoveringJobs%37E0D45F00A5.attr

      //## Attribute: m_poTimeBeginJS%380D73E202F9
      //## begin ClJSAdministrator::m_poTimeBeginJS%380D73E202F9.attr preserve=no  protected: M4ClTimeStamp * {UA} 
      M4ClTimeStamp *m_poTimeBeginJS;
      //## end ClJSAdministrator::m_poTimeBeginJS%380D73E202F9.attr

      //## Attribute: m_poTimeEndJS%380D751200C6
      //## begin ClJSAdministrator::m_poTimeEndJS%380D751200C6.attr preserve=no  protected: M4ClTimeStamp * {UA} 
      M4ClTimeStamp *m_poTimeEndJS;
      //## end ClJSAdministrator::m_poTimeEndJS%380D751200C6.attr

      //## Attribute: m_iPendingExecutors%381447B70297
      //	Nº de ejecutores que queremos tener operativos. De forma que vayamos
      //	aumentando o disminuyendo el nº de ejecutores.
      //## begin ClJSAdministrator::m_iPendingExecutors%381447B70297.attr preserve=no  protected: m4int32_t {UA} 
      m4int32_t m_iPendingExecutors;
      //## end ClJSAdministrator::m_iPendingExecutors%381447B70297.attr

      //## Attribute: m_bValidRecover%381730440398
      //## begin ClJSAdministrator::m_bValidRecover%381730440398.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bValidRecover;
      //## end ClJSAdministrator::m_bValidRecover%381730440398.attr

    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%37D7D55603BA
      //## Role: ClJSAdministrator::m_poCancelQ%37D7D55603BB
      //## begin ClJSAdministrator::m_poCancelQ%37D7D55603BB.role preserve=no  protected: ClJSQueue { -> RHAN}
      ClJSQueue *m_poCancelQ;
      //## end ClJSAdministrator::m_poCancelQ%37D7D55603BB.role

      //## Association: M4JSSubsystem::<unnamed>%37D7D55B0212
      //## Role: ClJSAdministrator::m_poInputQ%37D7D55B0213
      //## begin ClJSAdministrator::m_poInputQ%37D7D55B0213.role preserve=no  protected: ClJSQueue { -> RHAN}
      ClJSQueue *m_poInputQ;
      //## end ClJSAdministrator::m_poInputQ%37D7D55B0213.role

      //## Association: M4JSSubsystem::<unnamed>%37D8E6B200E9
      //## Role: ClJSAdministrator::m_poJSSubsystem%37D8E6B3007C
      //## begin ClJSAdministrator::m_poJSSubsystem%37D8E6B3007C.role preserve=no  protected: ClJSSubsystem { -> RFHAN}
      ClJSSubsystem *m_poJSSubsystem;
      //## end ClJSAdministrator::m_poJSSubsystem%37D8E6B3007C.role

      //## Association: M4JSSubsystem::<unnamed>%37DCEFAA02F4
      //## Role: ClJSAdministrator::m_poExecutorQ%37DCEFAB0093
      //## begin ClJSAdministrator::m_poExecutorQ%37DCEFAB0093.role preserve=no  protected: ClJSQueue { -> RHAN}
      ClJSQueue *m_poExecutorQ;
      //## end ClJSAdministrator::m_poExecutorQ%37DCEFAB0093.role

      //## Association: M4JSSubsystem::<unnamed>%37F3A46E0143
      //## Role: ClJSAdministrator::m_oMutex%37F3A46F0216
      //## begin ClJSAdministrator::m_oMutex%37F3A46F0216.role preserve=no  protected: ClMutex { -> VHAN}
      ClMutex m_oMutex;
      //## end ClJSAdministrator::m_oMutex%37F3A46F0216.role

      //## Association: M4JSSubsystem::<unnamed>%37FA1CB90004
      //## Role: ClJSAdministrator::m_oMapJobExe%37FA1CBA007E
      //## begin ClJSAdministrator::m_oMapJobExe%37FA1CBA007E.role preserve=no  protected: ClJSMapJobExe { -> VHAN}
      ClJSMapJobExe m_oMapJobExe;
      //## end ClJSAdministrator::m_oMapJobExe%37FA1CBA007E.role

      //## Association: M4JSSubsystem::<unnamed>%37FA2DA202E4
      //## Role: ClJSAdministrator::m_poActualRequest%37FA2DA30317
      //## begin ClJSAdministrator::m_poActualRequest%37FA2DA30317.role preserve=no  protected: ClJSRequest { -> RFHAN}
      ClJSRequest *m_poActualRequest;
      //## end ClJSAdministrator::m_poActualRequest%37FA2DA30317.role

    // Additional Protected Declarations
      //## begin ClJSAdministrator%37D537650106.protected preserve=yes
      //## end ClJSAdministrator%37D537650106.protected

  private:
    // Additional Private Declarations
      //## begin ClJSAdministrator%37D537650106.private preserve=yes
      //## end ClJSAdministrator%37D537650106.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSAdministrator%37D537650106.implementation preserve=yes
      //## end ClJSAdministrator%37D537650106.implementation

};

//## begin ClJSAdministrator%37D537650106.postscript preserve=yes
//## end ClJSAdministrator%37D537650106.postscript

// Class ClJSAdministrator 


//## Other Operations (inline)
inline m4return_t ClJSAdministrator::SetCancelQueue (ClJSQueue *ai_poQueue)
{
  //## begin ClJSAdministrator::SetCancelQueue%936690194.body preserve=yes
	if (m_poCancelQ)
		delete m_poCancelQ;

	m_poCancelQ = ai_poQueue;
	return M4_SUCCESS;
  //## end ClJSAdministrator::SetCancelQueue%936690194.body
}

inline m4return_t ClJSAdministrator::SetExecutorQueue (ClJSQueue *ai_poQueue)
{
  //## begin ClJSAdministrator::SetExecutorQueue%937399895.body preserve=yes
	if (m_poExecutorQ)
		delete m_poExecutorQ;

	m_poExecutorQ = ai_poQueue;
 	return M4_SUCCESS;
  //## end ClJSAdministrator::SetExecutorQueue%937399895.body
}

inline m4return_t ClJSAdministrator::SetInputQueue (ClJSQueue *ai_poQueue)
{
  //## begin ClJSAdministrator::SetInputQueue%936690195.body preserve=yes
	if (m_poInputQ)
		delete m_poInputQ;

	m_poInputQ = ai_poQueue;
 	return M4_SUCCESS;
  //## end ClJSAdministrator::SetInputQueue%936690195.body
}

inline ClJSQueue * ClJSAdministrator::GetCancelQueue () const
{
  //## begin ClJSAdministrator::GetCancelQueue%936690196.body preserve=yes
	return m_poCancelQ;
  //## end ClJSAdministrator::GetCancelQueue%936690196.body
}

inline ClJSQueue * ClJSAdministrator::GetInputQueue () const
{
  //## begin ClJSAdministrator::GetInputQueue%936690197.body preserve=yes
	return m_poInputQ;
  //## end ClJSAdministrator::GetInputQueue%936690197.body
}

inline ClJSQueue * ClJSAdministrator::GetExecutorQueue () const
{
  //## begin ClJSAdministrator::GetExecutorQueue%937399896.body preserve=yes
	return m_poExecutorQ;
  //## end ClJSAdministrator::GetExecutorQueue%937399896.body
}

inline m4return_t ClJSAdministrator::GetNextStopDate (M4ClTimeStamp &ao_oTime)
{
  //## begin ClJSAdministrator::GetNextStopDate%941120812.body preserve=yes
	m4return_t ret = M4_ERROR;
	m_oMutex.Lock();
	if (m_poTimeEndJS != NULL) {
		ao_oTime = *m_poTimeEndJS;
		ret = M4_SUCCESS;
	}
	m_oMutex.Unlock();
	return ret;
  //## end ClJSAdministrator::GetNextStopDate%941120812.body
}

inline m4return_t ClJSAdministrator::GetNextStartDate (M4ClTimeStamp &ao_oTime)
{
  //## begin ClJSAdministrator::GetNextStartDate%941120813.body preserve=yes
	m4return_t ret = M4_ERROR;
	m_oMutex.Lock();
	if (m_poTimeBeginJS != NULL) {
		ao_oTime = *m_poTimeBeginJS;
		ret = M4_SUCCESS;
	}
	m_oMutex.Unlock();
	return ret;
  //## end ClJSAdministrator::GetNextStartDate%941120813.body
}

inline m4return_t ClJSAdministrator::SetNextStopDate (M4ClTimeStamp *ai_poTime)
{
  //## begin ClJSAdministrator::SetNextStopDate%941120814.body preserve=yes
	m_oMutex.Lock();
	if (m_poTimeEndJS != NULL)
		delete m_poTimeEndJS;
	m_poTimeEndJS = ai_poTime;
	m_oMutex.Unlock();
	return M4_SUCCESS;
  //## end ClJSAdministrator::SetNextStopDate%941120814.body
}

inline m4return_t ClJSAdministrator::SetNextStartDate (M4ClTimeStamp *ai_poTime)
{
  //## begin ClJSAdministrator::SetNextStartDate%941120815.body preserve=yes
	m_oMutex.Lock();
	if (m_poTimeBeginJS != NULL)
		delete m_poTimeBeginJS;
	m_poTimeBeginJS = ai_poTime;
	m_oMutex.Unlock();
	return M4_SUCCESS;
  //## end ClJSAdministrator::SetNextStartDate%941120815.body
}

//## begin module%37D537A90028.epilog preserve=yes
//## end module%37D537A90028.epilog


#endif
