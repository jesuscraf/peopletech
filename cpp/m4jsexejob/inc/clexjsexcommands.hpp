//## begin module%34FFEAFF005A.cm preserve=no
//## end module%34FFEAFF005A.cm

//## begin module%34FFEAFF005A.cp preserve=no
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
//## end module%34FFEAFF005A.cp

//## Module: ExJSExCommands%34FFEAFF005A; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: e:\m4server\m4jsexejob\inc\clexjsexcommands.hpp

#ifndef exjsexcommands_h
#define exjsexcommands_h 1

//## begin module%34FFEAFF005A.additionalIncludes preserve=no
//## end module%34FFEAFF005A.additionalIncludes

//## begin module%34FFEAFF005A.includes preserve=yes
#include <m4string.hpp>
class ClNode;
//## end module%34FFEAFF005A.includes

// syncro
#include <syncro.hpp>
#include <rowsquota.hpp>

class ClStatus;
class ClChannelManager;
class ClAccess;
class ClChannel;
class ClNode;
class ClStatsJSEXECUTE;
class ClJSParametersHandlers;
class ClSecurity;
class ClJSChannelHandler;

//## begin module%34FFEAFF005A.declarations preserve=no
//## end module%34FFEAFF005A.declarations

//## begin module%34FFEAFF005A.additionalDeclarations preserve=yes
//## end module%34FFEAFF005A.additionalDeclarations


//## begin ClJSExeCommand%34FE920503A3.preface preserve=yes
//## end ClJSExeCommand%34FE920503A3.preface

//## Class: ClJSExeCommand%34FE920503A3
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35893E320299;ClStatsJSEXECUTE { -> F}
//## Uses: <unnamed>%35A0827901E8;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%36EE687201C9; { -> F}
//## Uses: <unnamed>%3805925300AB;M4ClTimeStamp { -> F}
//## Uses: <unnamed>%39181D570013; { -> F}

class ClJSExeCommand 
{
  //## begin ClJSExeCommand%34FE920503A3.initialDeclarations preserve=yes
  //## end ClJSExeCommand%34FE920503A3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSExeCommand%889099940
      ClJSExeCommand (ClSecurity *ai_poSecurity, ClChannel *ai_poChannelJGDef, ClChannel *ai_poChannelExec, ClAccess *ai_poAccessJGDef, ClAccess *ai_poAccessExec, ClJSChannelHandler *ai_poChannelHandler);

    //## Destructor (specified)
      //## Operation: ~ClJSExeCommand%889099941
      ~ClJSExeCommand ();


    //## Other Operations (specified)
      //## Operation: Execute%889099942
      m4return_t Execute (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExe, ClStatsJSEXECUTE *aio_poStats);

      //## Operation: SetDumpInfo%921585523
      m4void_t SetDumpInfo (ExecDumpInfo *ai_poDumpInfo);

      //## Operation: Cancel%936777416
      //	Cuando la herramienta de administraci�n envie un comando de cancelaci�n al
      //	subsistema de ejecutores , estos lo enviar�n al launcher , el launcher se lo
      //	enviar� al ejecutor y el ejecutor llamar� a esta funci�n . Esta funci�n
      //	comprueba el estado interno del propio ejecutor ( es decir si est�
      //	ejecutando el c�digo JIT o no) enviando una petici�n de cancelaci�n a la MVC
      //	seg�n este valor
      m4return_t Cancel ();

      //## Operation: SetTimeOut%940319367
      //	Esta funci�n permite modificar el tiempo de TIME OUT  asiganado a  un
      //	ejecutor . Es decir el tiempo m�ximo que puede tardar en ejecutar una tarea
      m4return_t SetTimeOut (m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Reset%940319368
      m4return_t Reset (m4bool_t &ao_bIsCancel, m4bool_t &ao_bIsTimeOut);

      //## Operation: SetParameterHandler%941717920
      void SetParameterHandler (ClJSParametersHandlers *ai_poParmeterHandler);

      //## Operation: SetIdentification%942839182
      m4return_t SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName);

      //## Operation: GetUsedMemory%951331610
      m4uint32_t GetUsedMemory ();

      //## Operation: RecoverCompoundJobParams%958997021
      //	Este m�todo se encargar� de recuperar y propagar los valores de entrada y
      //	salida de la tarea compuesta y grabarlos en base de datos. Esto se llevar� a
      //	cabo a trav�s del m�todo del mismo nombre de la clase ClJSParametersHandler.
      //
      //	Este m�todo tiene dos par�metros, por defecto igual a 1. El primer par�metro
      //	nos indica si debemos grabar los par�metros de entrada. El segundo nos
      //	indica qu� valor del par�metro de salida debemos grabar: null o el valor que
      //	tenga.
      m4return_t RecoverCompoundJobParams (m4uint32_t ai_bInParamValue = 1, m4uint32_t ai_bOutParamValue = 1);

    // Additional Public Declarations
      //## begin ClJSExeCommand%34FE920503A3.public preserve=yes
      //## end ClJSExeCommand%34FE920503A3.public

  protected:
    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%37D6259D00CC
      //## Role: ClJSExeCommand::m_poMutexStatus%37D6259D0285
      //## begin ClJSExeCommand::m_poMutexStatus%37D6259D0285.role preserve=no  protected: ClMutex { -> VHAN}
      ClMutex m_poMutexStatus;
      //## end ClJSExeCommand::m_poMutexStatus%37D6259D0285.role

      //## Association: M4JSExeJob::<unnamed>%380C7D5C0068
      //## Role: ClJSExeCommand::m_poStatusTimeOut%380C7D5D0146
      //## begin ClJSExeCommand::m_poStatusTimeOut%380C7D5D0146.role preserve=no  protected: ClStatus { -> RFHAN}
      ClStatus *m_poStatusTimeOut;
      //## end ClJSExeCommand::m_poStatusTimeOut%380C7D5D0146.role

      //## Association: M4JSExeJob::<unnamed>%380F4BAF01D6
      //## Role: ClJSExeCommand::m_poChannelHandler%380F4BB001D8
      //## begin ClJSExeCommand::m_poChannelHandler%380F4BB001D8.role preserve=no  protected: ClJSChannelHandler { -> RFHAN}
      ClJSChannelHandler *m_poChannelHandler;
      //## end ClJSExeCommand::m_poChannelHandler%380F4BB001D8.role

      //## Association: M4JSSubsystem::<unnamed>%36519E9E00AA
      //## Role: ClJSExeCommand::m_poSecurity%36519E9E03DF
      //## begin ClJSExeCommand::m_poSecurity%36519E9E03DF.role preserve=no  protected: ClSecurity {1 -> 1RFHAN}
      ClSecurity *m_poSecurity;
      //## end ClJSExeCommand::m_poSecurity%36519E9E03DF.role

	  ClRowsQuotaController m_RowsQuotaCtrl;

    // Additional Protected Declarations
      //## begin ClJSExeCommand%34FE920503A3.protected preserve=yes
      //## end ClJSExeCommand%34FE920503A3.protected

  private:

      m4return_t _ActivateStatistics( m4pchar_t ao_pcStatisticsFile ) const ;
	  m4return_t _BuildJITChannel();
	  m4return_t _SendInitExecutionMail(m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe);
	  m4return_t _SendEndExecutionMail(m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe);
	  m4return_t _GetEmailRecipients();
	  
    //## Other Operations (specified)
      //## Operation: RecordJobs%893687424
      //	Este m�todo registra en la base de datos un nuevo job execution.
      m4return_t RecordJobs (m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobDEF, ClStatsJSEXECUTE *ai_poStats);

      //## Operation: AddDateEnd%916848072
      //	Esta funci�n a�ade la fecha de fin de un box en caso que estemos ejecutando
      //	el �ltimo box de un job y la fecha de fin del job si el p�rametro en el que
      //	pasamos el nodo de las ejecuciones de los jobs  no es nulo . En otro caso
      //	siginifica que esta fecha ya ha sido previamente a�adido con lo que solo
      //	tenemos que a�adir la fecha de fin del box
      m4return_t AddDateEnd (ClStatsJSEXECUTE *ai_poStats, m4bool_t ai_bPersistChannelExec = M4_TRUE, m4bool_t ai_bAnyError = M4_TRUE, m4uint32_t ai_iNumRows = 0, m4uint32_t ai_iPeakRows = 0, m4uint32_t ai_iLimitRows = 0);

      //## Operation: RecoverFileAfterPersist%941096539
      //	Este  m�todo se encarga de recorrerse los ficheros externos una vez que se
      //	han grabado en base de datos para eliminar el path al fichero en aquellos
      //	que sean s�lo de copia y borrarlos de su almacenamiento f�sico.
      m4return_t RecoverFileAfterPersist ();

      //## Operation: AddLogMessage%917867871
      //	Este m�todo concatena mensajes de error que el ejecutor de jobs vuelca
      //	directamente al log de las tablas del job
      m4return_t AddLogMessage (m4pchar_t ai_pBuffer, m4uint32_t ai_idJobDef);

      //## Operation: AddParams%927125794
      //	Esta funci�n a�ade los parametros de entrada a la pila del canal de
      //	ejecuci�n y los par�metros de salida com entrada al nodo del canal de
      //	ejecuciones donde luego posteriormente recuperamos el valor de salida
      m4return_t AddParams (ClNode *ai_poNodeExeJIT, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF);

      //## Operation: RecoverParams%927125795
      //	Esta funci�n permite recuperar par�metros de salida cuyo valor ha quedado
      //	almacenado en el canal de ejecuciones
      m4return_t RecoverParams ();

      //## Operation: RecoverExternalResource%927648836
      //	Esta funci�n se encarge de recorrer el nodo donde han quedado los recursos
      //	genereados por la ejecuci�n del c�digo de la tarea.Recoge el path (que puede
      //	ser un path completo o un plantilla) i busca todos los ficheros que
      //	cioncidan cona la plantilla o el nombre en ese path. Obtienen detalles del
      //	fichero tal como su tama�o y seg�n la acci�n requerida :
      //
      //	  1.Si es una acci�n TEMPORARY -> borra el fichero del disco
      //
      //	 2. Si es una acci�n REFERENCE -> almacena el path al fichero
      //
      //	 3. Si es una acci�n de copia -> almacena el fichero en base de datos
      //
      //	 4.Si es una acci�n de REF_AND_COPY -> almacena el fichero en base de datos
      //	y almacena el path la fichero
      m4return_t RecoverExternalResource (m4uint32_t ai_uiIdJobGDef, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdBoxExe, m4uint32_t ai_uiIdJobExec);

      //## Operation: InitializeGroupExecution%894391143
      //	Esta funci�n busca el job group excution a partir del identificador del job
      //	group definition y de la fecha de ejecuci�n prevista.Este job group excution
      //	estar�a pendiente de ejecuci�n en caso de ser la ejecuci�n correspondiente
      //	al primer job del job group o en ejecuci�n en otro caso.
      //
      //	  Si est� pendiente de ejecuci�n tendr�amos  que cambiar su estatus a
      //	ejcutandose  y grabar en la base de datos la fecha en comenz� a
      //	ejecutarse.El m�todo devolver�a , a trav�s de un par�metro por variable  lel
      //	identificador del job group execution.
      //
      //	  Si est� ejecutandose lo �nico que hacemos es obtener e identificador del
      //	job group execution  que necesitamos luego para crear el job excution que
      //	vayamos a lanzar.
      m4return_t InitializeGroupExecution (m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, ClStatsJSEXECUTE *ai_poStats);

      //## Operation: CreatePrimaryKeyExec%927794195
      //	Permite rellenar los campos :
      //
      //	          -ID_JOB_GROUP_DEF
      //	          -ID_JOB_DEF
      //	          -ID_JOB_GROUP_EXEC
      //	          -ID_JOB_EXEC
      //
      //	que constituyen parte de la clave primaria de las tablas de par�metros de
      //	salida y de los recursos externos
      m4return_t CreatePrimaryKeyExec (ClNode *ai_poNode, m4uint32_t ai_uiIdJobGDef, m4uint32_t ai_uiIdBoxExe, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdJobExec = 0);

      //## Operation: ReLoadChannel%929100413
      //	Esta funci�n es la encargada de descargar un canal y volverlo a cargar
      //	situ�ndonos en la posici�n dentro de los nodos en la que nos encontrabamos
      //	antes de descargarlo
      m4return_t ReLoadChannel (m4uint32_t ai_uiIdJobGroupDef, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdBoxExe);

      //## Operation: RecoverLog%935686407
      //	Este funci�n se encarga de recopilar de la pila de la log todos los mensajes
      //	de error , warning o debug que se haya almacenado y volcarla a base de datos
      //	concatenandola con lo que hubiera previamente en  el campo LOG de la tabla
      //	de jobs.
      m4return_t RecoverLog (m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF);

      //## Operation: SetInternalStatus%936777417
      //	Esta funci�n es la encargada de moficar el estado interno de la ejecuci�n
      m4return_t SetInternalStatus (m4uint32_t ai_uiInternalStatus);

      //## Operation: GetInternalStatus%938539043
      //	Esta funci�n permite recuperar el valor de la variable que indica cu�l es el
      //	estado interno de la ejecuci�n
      m4uint32_t GetInternalStatus ();

      //## Operation: LoadAllMeta4Objects%941010640
      //	Este m�todo es el encargado de cargar los canales que vamos utilizar en la
      //	ejecuci�n de la tarea
      m4return_t LoadAllMeta4Objects (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExe);

      //## Operation: ReleaseAllMeta4Objects%941010641
      //	Este m�todo es el encargado de vaciar el contenido de todos los canales que
      //	hemos cargado para ejecutar la tarea
      m4return_t ReleaseAllMeta4Objects ();

      //## Operation: ExecuteJob%900945535
      //	Ejecuta el job que tengamos previsto en ese momento y registra en la BDL
      //	toda la informaci�n que tengamos respecto a la ejecuci�n .Por ejemplo :
      //	fecha de finalizaci�n , resultado de la ejecuci�n, etc..
      m4return_t ExecuteJob (ClStatsJSEXECUTE *ai_poStats, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF);

      //## Operation: ChangeSecurity%941096542
      m4return_t ChangeSecurity ();

	  m4uint32_t rowsQuotaProlog();

	  void rowsQuotaEpilog(m4uint32_t& ao_iNumRows, m4uint32_t& ao_iPeakRows);

    // Data Members for Class Attributes

      //## Attribute: m_poDumpInfo%36EE5DA00130
      //## begin ClJSExeCommand::m_poDumpInfo%36EE5DA00130.attr preserve=no  private: ExecDumpInfo * {UA} 
      ExecDumpInfo *m_poDumpInfo;
      //## end ClJSExeCommand::m_poDumpInfo%36EE5DA00130.attr

      //## Attribute: m_poInternalStatus%37D614E90218
      //	Este atributo informa del estado interno del ejecutor , es decir en que
      //	punto de la ejecuci�n nos encontramos , si estamos levantando canales ,
      //	buscando el trabajo , registrando la tarea, ejecutando el JIT
      //## begin ClJSExeCommand::m_poInternalStatus%37D614E90218.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_poInternalStatus;
      //## end ClJSExeCommand::m_poInternalStatus%37D614E90218.attr

      //## Attribute: m_uiTimeOut%380C211C036D
      //	Esta variable recoge el tiempo que debemos permitir al ejecutor realizar una
      //	tarea antes de interrumpirla .El tiempo m�ximo que un ejecutor puede tardar
      //	en ejecutar una tarea
      //## begin ClJSExeCommand::m_uiTimeOut%380C211C036D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_uiTimeOut;
      //## end ClJSExeCommand::m_uiTimeOut%380C211C036D.attr

      //## Attribute: m_stServerName%38329EAC001B
      //## begin ClJSExeCommand::m_stServerName%38329EAC001B.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_stServerName;
      //## end ClJSExeCommand::m_stServerName%38329EAC001B.attr

      //## Attribute: m_stServiceName%38329EDF019B
      //## begin ClJSExeCommand::m_stServiceName%38329EDF019B.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_stServiceName;
      //## end ClJSExeCommand::m_stServiceName%38329EDF019B.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%37E73EEA020D
      //## Role: ClJSExeCommand::m_poParametersHandler%37E73EEA03A7
      //## begin ClJSExeCommand::m_poParametersHandler%37E73EEA03A7.role preserve=no  private: ClJSParametersHandlers { -> RFHAN}
      ClJSParametersHandlers *m_poParametersHandler;
      //## end ClJSExeCommand::m_poParametersHandler%37E73EEA03A7.role

      //## Association: M4JSExeJob::<unnamed>%380F48CD03B6
      //## Role: ClJSExeCommand::m_poChannelGJDef%380F48CE0168
      //## begin ClJSExeCommand::m_poChannelGJDef%380F48CE0168.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poChannelGJDef;
      //## end ClJSExeCommand::m_poChannelGJDef%380F48CE0168.role

      //## Association: M4JSExeJob::<unnamed>%380F48F10155
      //## Role: ClJSExeCommand::m_poChannelExec%380F48F10277
      //## begin ClJSExeCommand::m_poChannelExec%380F48F10277.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poChannelExec;
      //## end ClJSExeCommand::m_poChannelExec%380F48F10277.role

	  //## Association: M4JSExeJob::<unnamed>%380F49120044
      //## Role: ClJSExeCommand::m_poChannelJIT%380F491202E3
      //## begin ClJSExeCommand::m_poChannelJIT%380F491202E3.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poChannelJIT;
      //## end ClJSExeCommand::m_poChannelJIT%380F491202E3.role

      //## Association: M4JSExeJob::<unnamed>%380F497C01AF
      //## Role: ClJSExeCommand::m_poAccessGJDef%380F497C0371
      //## begin ClJSExeCommand::m_poAccessGJDef%380F497C0371.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poAccessGJDef;
      //## end ClJSExeCommand::m_poAccessGJDef%380F497C0371.role

      //## Association: M4JSExeJob::<unnamed>%380F499902BF
      //## Role: ClJSExeCommand::m_poAccessExec%380F499A00AD
      //## begin ClJSExeCommand::m_poAccessExec%380F499A00AD.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poAccessExec;
      //## end ClJSExeCommand::m_poAccessExec%380F499A00AD.role

      //## Association: M4JSExeJob::<unnamed>%380F49C2028C
      //## Role: ClJSExeCommand::m_poAccessJIT%380F49C30070
      //## begin ClJSExeCommand::m_poAccessJIT%380F49C30070.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poAccessJIT;
      //## end ClJSExeCommand::m_poAccessJIT%380F49C30070.role

      //## Association: M4JSExeJob::<unnamed>%3816B46C0250
      //## Role: ClJSExeCommand::m_poNodeJGDef%3816B46D0052
      //## begin ClJSExeCommand::m_poNodeJGDef%3816B46D0052.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeJGDef;
      //## end ClJSExeCommand::m_poNodeJGDef%3816B46D0052.role

      //## Association: M4JSExeJob::<unnamed>%3816B4960250
      //## Role: ClJSExeCommand::m_poNodeJDef%3816B49700C1
      //## begin ClJSExeCommand::m_poNodeJDef%3816B49700C1.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeJDef;
      //## end ClJSExeCommand::m_poNodeJDef%3816B49700C1.role

      //## Association: M4JSExeJob::<unnamed>%3816B4B60148
      //## Role: ClJSExeCommand::m_poNodeGExec%3816B4B602D8
      //## begin ClJSExeCommand::m_poNodeGExec%3816B4B602D8.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeGExec;
      //## end ClJSExeCommand::m_poNodeGExec%3816B4B602D8.role

      //## Association: M4JSExeJob::<unnamed>%3816B4DC0305
      //## Role: ClJSExeCommand::m_poNodeJExec%3816B4DD0126
      //## begin ClJSExeCommand::m_poNodeJExec%3816B4DD0126.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeJExec;
      //## end ClJSExeCommand::m_poNodeJExec%3816B4DD0126.role

    // Additional Private Declarations
      //## begin ClJSExeCommand%34FE920503A3.private preserve=yes
	  string	m_sEmailRecipients;
      //## end ClJSExeCommand%34FE920503A3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSExeCommand%34FE920503A3.implementation preserve=yes
      //## end ClJSExeCommand%34FE920503A3.implementation

};

//## begin ClJSExeCommand%34FE920503A3.postscript preserve=yes
//## end ClJSExeCommand%34FE920503A3.postscript

// Class ClJSExeCommand 

//## begin module%34FFEAFF005A.epilog preserve=yes
//## end module%34FFEAFF005A.epilog


#endif
