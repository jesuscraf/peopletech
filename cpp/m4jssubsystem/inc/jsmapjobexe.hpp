//## begin module%37FA29890209.cm preserve=no
//## end module%37FA29890209.cm

//## begin module%37FA29890209.cp preserve=no
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
//## end module%37FA29890209.cp

//## Module: jsmapjobexe%37FA29890209; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\version\inc\jsmapjobexe.hpp

#ifndef jsmapjobexe_h
#define jsmapjobexe_h 1

//## begin module%37FA29890209.additionalIncludes preserve=no
//## end module%37FA29890209.additionalIncludes

//## begin module%37FA29890209.includes preserve=yes
#include "m4stl.hpp"
#include "jsjob.hpp"
//## end module%37FA29890209.includes


class ClExecutive;

//## begin module%37FA29890209.declarations preserve=no
//## end module%37FA29890209.declarations

//## begin module%37FA29890209.additionalDeclarations preserve=yes
//## end module%37FA29890209.additionalDeclarations


//## begin jsmapJobExecutor_t%37F9DC8901F1.preface preserve=yes
//## end jsmapJobExecutor_t%37F9DC8901F1.preface

//## Class: jsmapJobExecutor_t%37F9DC8901F1; Instantiated Class
//	Mapa que contiene una lista de ejecutores y asociado el trabajo que esta
//	ejecutando, si es NULL, es que no esta ejecutando ninguno.
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F9DF7D003F;ClJSJob { -> F}
//## Uses: <unnamed>%37FDA2C001ED;ClExecutive { -> F}

typedef map< ClExecutive *,ClJSJob *,less < ClExecutive * const > > jsmapJobExecutor_t;

//## begin jsmapJobExecutor_t%37F9DC8901F1.postscript preserve=yes
//## end jsmapJobExecutor_t%37F9DC8901F1.postscript

//## begin structJSJobInfo%37FC716100EA.preface preserve=yes
//## end structJSJobInfo%37FC716100EA.preface

//## Class: structJSJobInfo%37FC716100EA
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct structJSJobInfo 
{
  //## begin structJSJobInfo%37FC716100EA.initialDeclarations preserve=yes
  //## end structJSJobInfo%37FC716100EA.initialDeclarations

    //## begin structJSJobInfo::eJobStatus%3805B4C3007F.preface preserve=yes
    //## end structJSJobInfo::eJobStatus%3805B4C3007F.preface

    //## Class: eJobStatus%3805B4C3007F
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {JOB_WAITING=0, JOB_SEND, JOB_EXECUTING, JOB_END, JOB_CANCELED} eJobStatus;

    //## begin structJSJobInfo::eJobStatus%3805B4C3007F.postscript preserve=yes
    //## end structJSJobInfo::eJobStatus%3805B4C3007F.postscript

    //## Constructors (specified)
      //## Operation: structJSJobInfo%939898164
      structJSJobInfo ();

    // Data Members for Class Attributes

      //## Attribute: m_uiPriority%37FC71A00145
      //## begin structJSJobInfo::m_uiPriority%37FC71A00145.attr preserve=no  public: m4uint32_t {UA} 0
      m4uint32_t m_uiPriority;
      //## end structJSJobInfo::m_uiPriority%37FC71A00145.attr

      //## Attribute: m_uiActiveTask%37FDCB2C003E
      //## begin structJSJobInfo::m_uiActiveTask%37FDCB2C003E.attr preserve=no  public: m4uint32_t {UA} 0
      m4uint32_t m_uiActiveTask;
      //## end structJSJobInfo::m_uiActiveTask%37FDCB2C003E.attr

      //## Attribute: m_uiStatus%37FDCB4703CC
      //## begin structJSJobInfo::m_uiStatus%37FDCB4703CC.attr preserve=no  public: structJSJobInfo::eJobStatus {UA} JOB_WAITING
      structJSJobInfo::eJobStatus m_uiStatus;
      //## end structJSJobInfo::m_uiStatus%37FDCB4703CC.attr

  public:
    // Additional Public Declarations
      //## begin structJSJobInfo%37FC716100EA.public preserve=yes
      //## end structJSJobInfo%37FC716100EA.public

  protected:
    // Additional Protected Declarations
      //## begin structJSJobInfo%37FC716100EA.protected preserve=yes
      //## end structJSJobInfo%37FC716100EA.protected

  private:
    // Additional Private Declarations
      //## begin structJSJobInfo%37FC716100EA.private preserve=yes
      //## end structJSJobInfo%37FC716100EA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin structJSJobInfo%37FC716100EA.implementation preserve=yes
      //## end structJSJobInfo%37FC716100EA.implementation

};

//## begin structJSJobInfo%37FC716100EA.postscript preserve=yes
//## end structJSJobInfo%37FC716100EA.postscript

//## begin jsmapJob_t%37FB68B4030A.preface preserve=yes
//## end jsmapJob_t%37FB68B4030A.preface

//## Class: jsmapJob_t%37FB68B4030A; Instantiated Class
//	Lista de trabajos que tenemos actualmente en el JS, junto con información
//	adicioneal.
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FB68E2032E;ClJSJob { -> F}
//## Uses: <unnamed>%37FC71C90360;structJSJobInfo { -> }

typedef map< ClJSJob *,structJSJobInfo *,less < ClJSJob* const > > jsmapJob_t;

//## begin jsmapJob_t%37FB68B4030A.postscript preserve=yes
//## end jsmapJob_t%37FB68B4030A.postscript

//## begin ClJSMapJobExe%37FA1AA601AF.preface preserve=yes
//## end ClJSMapJobExe%37FA1AA601AF.preface

//## Class: ClJSMapJobExe%37FA1AA601AF
//	Clase que encapsula toda la operativa en la relacion ejecutor-job.
//	Contiene un miembro que es un mapa de ejecutores-jobs y nos da métodos para
//	manejar dicho mapa, asi como funciones especiales
//
//	Tambien contiene una lista de todos los jobs que tenemos.
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3806ECA202E8;ClExeForJobSchecEXECUTE { -> }
//## Uses: <unnamed>%3806F0140155;ClJSJob { -> }

class ClJSMapJobExe 
{
  //## begin ClJSMapJobExe%37FA1AA601AF.initialDeclarations preserve=yes
  //## end ClJSMapJobExe%37FA1AA601AF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSMapJobExe%939122286; C++
      ClJSMapJobExe ();


    //## Other Operations (specified)
      //## Operation: UpdateExecutor%939122280; C++
      //	Asocia al ejecutor que se pasa como parámetro el trabajo que se le da.
      //	Si el trabajo es NULL, significa que ha finalizado una ejecución y por lo
      //	tanto disminuye el nº de trabajos ejecutándose.
      m4return_t UpdateExecutor (ClExecutive *ai_pExecutive, ClJSJob *ai_pJSJob);

      //## Operation: AddExecutor%939122281; C++
      m4return_t AddExecutor (ClExecutive *ai_pExecutive);

      //## Operation: DeleteExecutor%939122282; C++
      //	Borra el ejecutor que se le pasa como parámetro.
      //	Se mira si estaba ejecutando algún trabajo, porque entonces habría que
      //	marcarlo como cancelado y hay que disminuir el nº de trabajos ejecutándose.
      m4return_t DeleteExecutor (ClExecutive *ai_pExecutive);

      //## Operation: GetFreeExecutors%939122287
      m4uint32_t GetFreeExecutors ();

      //## Operation: GetUsedExecutors%939213537
      m4uint32_t GetUsedExecutors ();

      //## Operation: GetNumExecutors%941108272
      m4uint32_t GetNumExecutors ();

      //## Operation: AddJob%939805990; C++
      m4return_t AddJob (ClJSJob *ai_pJob);

      //## Operation: DeleteJob%939805991; C++
      m4return_t DeleteJob (ClJSJob *ai_pJob);

      //## Operation: GetPendingJob%939805992; C++
      //	Devuelve un job que esta esperando o que su ejecutor ha dejado de funcionar.
      //	Solamente devuelve un valor cuando existen ejecutores disponibles para
      //	ejecución.
      m4return_t GetPendingJob (ClJSJob *&ao_pJob);

      //## Operation: GetCanceledJob%939912564
      m4return_t GetCanceledJob (ClJSJob *&ao_pJob);

      //## Operation: GetWaitingJob%939912566
      m4return_t GetWaitingJob (ClJSJob *&ao_pJob);

      //## Operation: InterruptJob%939912567
      m4return_t InterruptJob (const ClJSJob::eInterruptRequestLevel &ai_uiInterruptType, ClJSJob *ai_pJob);

      //## Operation: GetNextTask%940322788
      //	Devuelve la siguiente tarea ejecutable del trabajo que se le pasa por
      //	parámetro.
      //	Actualiza el nº de trabajos ejecutándose.
      ClJSJob * GetNextTask (ClJSJob *ai_pJob);

      //## Operation: InterruptAllJobs%940521658
      m4return_t InterruptAllJobs (const ClJSJob::eInterruptRequestLevel &ai_uiInterruptType);

      //## Operation: ReqDelExecutor%940853233
      //	Preguntamos si se puede borrar algún ejecutor, y si es asi actualiza las
      //	variables internas para que se tenga en cuenta el borrado.
      m4return_t ReqDelExecutor ();

      //## Operation: DeleteWaitingJobs%941043668
      m4return_t DeleteWaitingJobs ();

    // Additional Public Declarations
      //## begin ClJSMapJobExe%37FA1AA601AF.public preserve=yes
      //## end ClJSMapJobExe%37FA1AA601AF.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetJob%939912565
      //	Obtiene un job con el estado que le pases como primer parámetro..
      //	Además, se le cambia el estado de ese job a SEND y aumentamos el nº de jobs
      //	ejecutándose.
      m4return_t GetJob (structJSJobInfo::eJobStatus ai_uiJobStatus, ClJSJob *&ao_pJob);

    // Data Members for Class Attributes

      //## Attribute: m_uiNumJobsExecuting%3806138D0227
      //	Nos indica el nº de trabajos que se estan ejecutando o que se van a mandar.
      //	Aumentamos su valor, cuando se devuelve un nuevo job que esta waiting o
      //	canceled o bien cuando se soliciata una nueva tarea a un job determinado.
      //	Por el contrario se disminuye cuando un trabajo ha sido cancelado (Excepcion
      //	en su ejecutor) o cuando se finaliza una tarea.
      //## begin ClJSMapJobExe::m_uiNumJobsExecuting%3806138D0227.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiNumJobsExecuting;
      //## end ClJSMapJobExe::m_uiNumJobsExecuting%3806138D0227.attr

      //## Attribute: m_uiNumExecutors%3814691C0005
      //	Se utiliza, porque hay ocasiones en que el nº que hay en el mapa y este no
      //	coincide, porque se desea borrar alguno, y entre el tiempo que pasa desde
      //	que se realiza la petición hasta que se lleva a cabo ocurre esta anomalia.
      //## begin ClJSMapJobExe::m_uiNumExecutors%3814691C0005.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiNumExecutors;
      //## end ClJSMapJobExe::m_uiNumExecutors%3814691C0005.attr

    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%37F9DDB7008C
      //## Role: ClJSMapJobExe::m_oMapJobExe%37F9DDB8007A
      //## begin ClJSMapJobExe::m_oMapJobExe%37F9DDB8007A.role preserve=no  protected: jsmapJobExecutor_t { -> VHAN}
      jsmapJobExecutor_t m_oMapJobExe;
      //## end ClJSMapJobExe::m_oMapJobExe%37F9DDB8007A.role

      //## Association: M4JSSubsystem::<unnamed>%37FC5F2A0261
      //## Role: ClJSMapJobExe::m_oMapJobs%37FC5F2B01A5
      //## begin ClJSMapJobExe::m_oMapJobs%37FC5F2B01A5.role preserve=no  protected: jsmapJob_t { -> VHAN}
      jsmapJob_t m_oMapJobs;
      //## end ClJSMapJobExe::m_oMapJobs%37FC5F2B01A5.role

    // Additional Protected Declarations
      //## begin ClJSMapJobExe%37FA1AA601AF.protected preserve=yes
      //## end ClJSMapJobExe%37FA1AA601AF.protected

  private:
    // Additional Private Declarations
      //## begin ClJSMapJobExe%37FA1AA601AF.private preserve=yes
      //## end ClJSMapJobExe%37FA1AA601AF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSMapJobExe%37FA1AA601AF.implementation preserve=yes
      //## end ClJSMapJobExe%37FA1AA601AF.implementation

};

//## begin ClJSMapJobExe%37FA1AA601AF.postscript preserve=yes
//## end ClJSMapJobExe%37FA1AA601AF.postscript

// Class structJSJobInfo 

// Class ClJSMapJobExe 

//## begin module%37FA29890209.epilog preserve=yes
//## end module%37FA29890209.epilog


#endif
