//## begin module%3588F16A0083.cm preserve=no
//## end module%3588F16A0083.cm

//## begin module%3588F16A0083.cp preserve=no
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
//## end module%3588F16A0083.cp

//## Module: StatsJSEXECUTE%3588F16A0083; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: e:\m4server\m4jsexejob\inc\clstatsjsexecute.hpp

#ifndef statsjsexecute_h
#define statsjsexecute_h 1

//## begin module%3588F16A0083.additionalIncludes preserve=no
//## end module%3588F16A0083.additionalIncludes

//## begin module%3588F16A0083.includes preserve=yes
#include <m4jsexejob_dll.hpp>
//## end module%3588F16A0083.includes

// LauncherInterface
#include <launcher.hpp>
// syncro
#include <syncro.hpp>
//## begin module%3588F16A0083.declarations preserve=no
//## end module%3588F16A0083.declarations

//## begin module%3588F16A0083.additionalDeclarations preserve=yes
//## end module%3588F16A0083.additionalDeclarations


//## begin ClStatsJSEXECUTE%3588EEFC010B.preface preserve=yes
//## end ClStatsJSEXECUTE%3588EEFC010B.preface

//## Class: ClStatsJSEXECUTE%3588EEFC010B
//	Esta clase recoge las estadísticas asociadas a los ejecutores de jobs . Las
//	estadísticas que recorgerá son;
//
//	 -número de tareas  finalizados con éxito en el ejecutor
//	-número de tareas  finalizados con error en el ejecutor
//	-número de  trabajos interrumpidos ( cancelados / abortados)
//	-fecha de inicio de vida del ejecutor
//	-fecha de fin de vida del ejecutor
//	-fecha de inicio de la última ejecución
//	-fecha de fin de la última ejecución
//	-Identificador de la tarea en ejecución
//	-Identificador del trabajo en ejecución
//	-Identificador del usuario  bajo el que se está ejecutando la tarea
//
//	Además necesiteramos almacenar los siguientes datos que tendremos que
//	proporcionar al servicio de ejecutores de tareas para que estos puedan
//	calcular las estadísticas
//	-Tiempo total dedicado ese ejecutor a realizar tareas
//	-Retraso total de las tareas ejecutadas en ese ejecutor
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClStatsJSEXECUTE : public ClLauncherStats  //## Inherits: <unnamed>%359CA2E20235
{
  //## begin ClStatsJSEXECUTE%3588EEFC010B.initialDeclarations preserve=yes
  //## end ClStatsJSEXECUTE%3588EEFC010B.initialDeclarations

  public:
    //## begin ClStatsJSEXECUTE::eJSExecuteStats%358E73380180.preface preserve=yes
    //## end ClStatsJSEXECUTE::eJSExecuteStats%358E73380180.preface

    //## Class: eJSExecuteStats%358E73380180
    //## Category: M4JSExeJob%37F8BA0B0245
    //## Subsystem: M4JSExeJob::inc%37F8BA610325
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {NUM_JOBS_TOTAL =ClLauncherStats::TOTAL_NUM ,NUM_JOBS_ERROR , NUM_JOBS_SUCCESS ,  NUM_JOB_GROUPS_COMPLETE,NUM_BOX_INTERRUPTED ,M4_USER_JOB,M4_JS_ID_EXECUTION,M4_LAST_TIME_JOB,M4_TOTAL_DELAY_IN_JOBS,M4_TOTAL_TIME_IN_EXECUTION,M4_DATE_START_EXECUTION, M4_DATE_END_EXECUTION,TOTAL_NUM} eJSExecuteStats;

    //## begin ClStatsJSEXECUTE::eJSExecuteStats%358E73380180.postscript preserve=yes
    //## end ClStatsJSEXECUTE::eJSExecuteStats%358E73380180.postscript

    //## Constructors (specified)
      //## Operation: ClStatsJSEXECUTE%898160025
      ClStatsJSEXECUTE ();

    //## Destructor (specified)
      //## Operation: ~ClStatsJSEXECUTE%898160026
      //	Esta es la clase que recoge estadísticas del ejecutor de job scheduler  que
      //	lanza tareas.
      //
      //	  Las principales estadísticas que se recogen con este ejecutor son :
      //
      //	  -número de jobs lanzados
      //	  -número de job ejecutados con éxito
      //	  -número de jobs fracasados
      //	  -máximo tiempo tardado por un job.
      ~ClStatsJSEXECUTE ();


    //## Other Operations (specified)
      //## Operation: GetNumberJobsSuccess%898160028
      m4uint32_t GetNumberJobsSuccess ();

      //## Operation: GetNumberJobsError%898160029
      m4uint32_t GetNumberJobsError ();

      //## Operation: SetNumberJobsSuccess%898160036
      m4return_t SetNumberJobsSuccess (m4uint32_t ai_numjobsSuccess);

      //## Operation: SetNumberJobsError%898160037
      m4return_t SetNumberJobsError (m4uint32_t ai_numjobsError);

      //## Operation: SetNumberJobGroupsComplete%898160039
      m4return_t SetNumberJobGroupsComplete (m4uint32_t ai_numjobgroupsComplete);

      //## Operation: Reset%898160043
      //	Este método nos permite resetear todas las estadísticas del ejecutor de jobs.
      m4return_t Reset ();

      //## Operation: GetStat%898513641
      m4return_t  GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%898513642
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: GetIdJob%936603572
      //	Permite recuperar el identificador de la tarea que está actualmente
      //	ejecutándose . Si el ejecutor no está ejecutando ninguna tarea el valor será
      //	0
      m4uint32_t GetIdJob ();

      //## Operation: SetIdJob%936603573
      //	Permite modificar  el idenficador de la tarea que estamos ejecutamod en un
      //	instante
      m4return_t SetIdJob (m4uint32_t ai_uiIdJob);

      //## Operation: GetIdBox%936603574
      //	Permite recuperar el valor del trabajo que está ejecutandose en ese momento
      //	en el ejecutor. Si el ejecutor está inactivo ( no ejecuta ningun trabajo el
      //	valor será 0
      m4uint32_t GetIdBox ();

      //## Operation: SetIdBox%936603575
      //	Permite modificar el valor del identificador del trabajo cuando vamos a
      //	empezar una nueva ejecución de un tarea
      m4return_t SetIdBox (m4uint32_t ai_uiIdBox);

      //## Operation: SetUser%936603576
      //	Permite modificar el usuario bajo el cual se va a ejecutar la tarea actual
      m4return_t SetUser (m4pchar_t ai_pcUser);

      //## Operation: GetUser%936603577
      //	Permite recuperar el usuario bajo el que se está ejecutanodo la tarea actual
      //	.Si no hay tarea ejecutándose en el ejecutor entonces el valor del usuario
      //	es una cadena vacía
      m4return_t GetUser (M4ClString &a_sUser);

      //## Operation: GetJobsInterrupted%936603578
      m4uint32_t GetJobsInterrupted ();

      //## Operation: SetJobsInterrupted%936603579
      m4return_t SetJobsInterrupted (m4uint32_t ai_uiJobsInterrupted);

      //## Operation: GetTimeInExecution%936603580
      //	Permite recuperar el valor que indica cuánto tiempo del total de tiempo de
      //	vida del ejecutor se ha dedicado a ejecución de tareas.
      m4double_t GetTimeInExecution ();

      //## Operation: GetTimeDelay%936603582
      //	Permite recuperar el tiempo total que el ejecutor lleva de retraso en la
      //	ejecución respecto a la fecha prevista para la tarea actualmente en el
      //	ejecutor
      m4double_t GetTimeDelay ();

      //## Operation: SetTimeDelay%936603583
      //	Permite aumentar el tiempo que el ejecutor lleva de retraso en la ejecución
      //	de las tareas
      m4return_t SetTimeDelay (m4double_t ai_dDateScheduled);

      //## Operation: DesactiveExecution%936621011
      //	Esta función actualiza los valores del usuario , identificador de la tarea y
      //	del trabajo a no disponibles una vez que el ejecutor ha finalizado la
      //	ejecución de una tarea y como consecuencia pasa a inactivo
      m4return_t DesactiveExecution ();

      //## Operation: SetLastTimeJob%936621012
      //	Esta función permite actualizar el tiempo tardado por la última ejecución
      m4return_t SetLastTimeJob ();

      //## Operation: GetLastTimeJob%936621013
      //	Esta función permite recuperar el tiempo tardado por la última tarea
      //	completada por el ejecutor .Cuando hay una ejecución activa en el ejecutor
      //	este valor no está disponible por lo que será 0
      m4uint32_t GetLastTimeJob ();

      //## Operation: GetNumberJobGroupsComplete%898160031
      m4uint32_t GetNumberJobGroupsComplete ();

      //## Operation: AddJobsError%936621014
      //	Añade a la estadística del número de tareas finalizadas con error la
      //	cantidad que indiquemos en el parámetro
      m4return_t AddJobsError (m4uint32_t ai_uiNumJobsError);

      //## Operation: AddJobsSuccess%936621015
      //	Añade al número de tareas finalizadas con error la cantidad que indique el
      //	parámetro
      m4return_t AddJobsSuccess (m4uint32_t ai_uiNumJobsSuccess);

      //## Operation: AddJobsGroupCompleted%936621016
      //	Añade al númro de grupos de tareas completados la cantidad que le indiquemos
      //	en el parámetro
      m4return_t AddJobsGroupCompleted (m4uint32_t ai_uiJobGroupsCompleted);

      //## Operation: DesactiveLastExecution%936691798
      //	Esta función desactiva los valores correspondientes a las estadísticas de la
      //	última ejecución de una tarea . Al iniciar una una nueva tarea debemos
      //	resetear los valores de la ejecución anterior tal como la fecha en la que se
      //	finalizó y cuánto duró. La razón es para que no se confundan estos valores
      //	como datos de la ejecución actualmente en curso
      m4return_t DesactiveLastExecution ();

      //## Operation: ActiveIdentifiersJob%936691799
      //	Esta función activará las estadísticas que informan sobre el identificador
      //	del trabajo y de la tarea .La razón de modificarlas  de una sola vez es para
      //	que el administrador no pueda consultar un valor antes de que no se hayan
      //	modificado los dos de modo que pueda ver el valor de la trabajo pero aún no
      //	esté disponible el de tarea
      m4return_t ActiveIdentifiersJob (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExec);

      //## Operation: AddBoxInterrupted%936691800
      //	Esta función añade a la estadística de trabajos interrumpidos ( cancelados /
      //	abortados ) la cantidad que indiquemos en el parámetro
      m4return_t AddBoxInterrupted (m4uint32_t ai_uiBoxInterrupted);

      //## Operation: SetTimeInExecution%936691801
      m4return_t SetTimeInExecution ();

      //## Operation: GetIdBoxExec%938599737
      m4uint32_t GetIdBoxExec ();

      //## Operation: SetIdBoxExec%938599738
      m4return_t SetIdBoxExec (m4uint32_t ai_uiIdBoxExec);

      //## Operation: SetDateEndExecution%942339040
      m4return_t SetDateEndExecution (m4double_t ai_dDateEndExecution);

      //## Operation: SetDateStartExecution%942339041
      m4return_t SetDateStartExecution (m4double_t ai_dDateStartExecution);

  public:
    // Additional Public Declarations
      //## begin ClStatsJSEXECUTE%3588EEFC010B.public preserve=yes
      //## end ClStatsJSEXECUTE%3588EEFC010B.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_numjobsSuccess%3588EF57038C
      //	Número de jobs completados con éxito
      //## begin ClStatsJSEXECUTE::m_numjobsSuccess%3588EF57038C.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_numjobsSuccess;
      //## end ClStatsJSEXECUTE::m_numjobsSuccess%3588EF57038C.attr

      //## Attribute: m_numjobsError%3588EFBB02AA
      //	Número de jobs finalizados con error
      //## begin ClStatsJSEXECUTE::m_numjobsError%3588EFBB02AA.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_numjobsError;
      //## end ClStatsJSEXECUTE::m_numjobsError%3588EFBB02AA.attr

      //## Attribute: m_numjobgroupsComplete%3588F00E00DC
      //	Número de job groups finalizados con éxito
      //## begin ClStatsJSEXECUTE::m_numjobgroupsComplete%3588F00E00DC.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_numjobgroupsComplete;
      //## end ClStatsJSEXECUTE::m_numjobgroupsComplete%3588F00E00DC.attr

      //## Attribute: m_uiIdBox%37D36C490383
      //## begin ClStatsJSEXECUTE::m_uiIdBox%37D36C490383.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIdBox;
      //## end ClStatsJSEXECUTE::m_uiIdBox%37D36C490383.attr

      //## Attribute: m_uiIdJob%37D36C5D01AB
      //## begin ClStatsJSEXECUTE::m_uiIdJob%37D36C5D01AB.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIdJob;
      //## end ClStatsJSEXECUTE::m_uiIdJob%37D36C5D01AB.attr

      //## Attribute: m_pcUserJob%37D36C6900C2
      //## begin ClStatsJSEXECUTE::m_pcUserJob%37D36C6900C2.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcUserJob;
      //## end ClStatsJSEXECUTE::m_pcUserJob%37D36C6900C2.attr

      //## Attribute: m_uiJobsInterrupted%37D36C8802BB
      //## begin ClStatsJSEXECUTE::m_uiJobsInterrupted%37D36C8802BB.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiJobsInterrupted;
      //## end ClStatsJSEXECUTE::m_uiJobsInterrupted%37D36C8802BB.attr

      //## Attribute: m_dTimeInExecution%37D36C9A0307
      //## begin ClStatsJSEXECUTE::m_dTimeInExecution%37D36C9A0307.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_dTimeInExecution;
      //## end ClStatsJSEXECUTE::m_dTimeInExecution%37D36C9A0307.attr

      //## Attribute: m_dTimeDelay%37D36EDC0330
      //## begin ClStatsJSEXECUTE::m_dTimeDelay%37D36EDC0330.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_dTimeDelay;
      //## end ClStatsJSEXECUTE::m_dTimeDelay%37D36EDC0330.attr

      //## Attribute: m_uiLastTimeJob%37D3CCA60190
      //## begin ClStatsJSEXECUTE::m_uiLastTimeJob%37D3CCA60190.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiLastTimeJob;
      //## end ClStatsJSEXECUTE::m_uiLastTimeJob%37D3CCA60190.attr

      //## Attribute: m_uiDelayCurrentJob%37D502300324
      //	En este atributo almacenamos el tiempo de retraso que llevamos sobre la
      //	fecha prevista de ejecución en el job  que  actualmente se está ejecutando
      //## begin ClStatsJSEXECUTE::m_uiDelayCurrentJob%37D502300324.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiDelayCurrentJob;
      //## end ClStatsJSEXECUTE::m_uiDelayCurrentJob%37D502300324.attr

      //## Attribute: m_uiIdBoxExec%37F1E44D02BA
      //## begin ClStatsJSEXECUTE::m_uiIdBoxExec%37F1E44D02BA.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIdBoxExec;
      //## end ClStatsJSEXECUTE::m_uiIdBoxExec%37F1E44D02BA.attr

      //## Attribute: m_dDateEndExecution%382AF51C0137
      //## begin ClStatsJSEXECUTE::m_dDateEndExecution%382AF51C0137.attr preserve=no  protected: m4double_t {UA} 
      m4double_t m_dDateEndExecution;
      //## end ClStatsJSEXECUTE::m_dDateEndExecution%382AF51C0137.attr

      //## Attribute: m_dDateStartExecution%382AF524039B
      //## begin ClStatsJSEXECUTE::m_dDateStartExecution%382AF524039B.attr preserve=no  protected: m4double_t {UA} 
      m4double_t m_dDateStartExecution;
      //## end ClStatsJSEXECUTE::m_dDateStartExecution%382AF524039B.attr

    // Additional Protected Declarations
      //## begin ClStatsJSEXECUTE%3588EEFC010B.protected preserve=yes
      //## end ClStatsJSEXECUTE%3588EEFC010B.protected

  private:

    //## Other Operations (specified)
      //## Operation: DateToHourSecondMinutes%936691802
      m4return_t DateToHourSecondMinutes (m4uint32_t ai_uiTime, m4uint32_t &ao_uiHour, m4uint32_t &ao_uiMinutes, m4uint32_t &ao_uiSeconds);

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%37D3A18F010C
      //## Role: ClStatsJSEXECUTE::m_poMutex%37D3A18F0275
      //## begin ClStatsJSEXECUTE::m_poMutex%37D3A18F0275.role preserve=no  private: ClMutex { -> VHAN}
      ClMutex m_poMutex;
      //## end ClStatsJSEXECUTE::m_poMutex%37D3A18F0275.role

    // Additional Private Declarations
      //## begin ClStatsJSEXECUTE%3588EEFC010B.private preserve=yes
      //## end ClStatsJSEXECUTE%3588EEFC010B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStatsJSEXECUTE%3588EEFC010B.implementation preserve=yes
      //## end ClStatsJSEXECUTE%3588EEFC010B.implementation

};

//## begin ClStatsJSEXECUTE%3588EEFC010B.postscript preserve=yes
//## end ClStatsJSEXECUTE%3588EEFC010B.postscript

// Class ClStatsJSEXECUTE 

//## begin module%3588F16A0083.epilog preserve=yes
//## end module%3588F16A0083.epilog


#endif
