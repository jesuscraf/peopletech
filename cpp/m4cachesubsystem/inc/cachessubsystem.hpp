//## begin module%38D9EC8C006F.cm preserve=no
//## end module%38D9EC8C006F.cm

//## begin module%38D9EC8C006F.cp preserve=no
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
//## end module%38D9EC8C006F.cp

//## Module: cachessubsystem%38D9EC8C006F; Package specification
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Source file: D:\Work\v600\M4CacheSubsystem\inc\cachessubsystem.hpp

#ifndef cachessubsystem_h
#define cachessubsystem_h 1

//## begin module%38D9EC8C006F.additionalIncludes preserve=no
//## end module%38D9EC8C006F.additionalIncludes

//## begin module%38D9EC8C006F.includes preserve=yes

#define ACTION_CLEAN_ALL_CACHES		"ACTION_CLEAN_ALL_CACHES"
#define LAST_CLEAN_ALL_CACHES		"LAST_CLEAN_ALL_CACHES"

#include <m4cachesubsystem_dll.hpp>
//## end module%38D9EC8C006F.includes

// Statistics
#include <statistics.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>

class ClCacheTest;
class ClCachesStats;
class ClCachesSubsystem;

//## begin module%38D9EC8C006F.declarations preserve=no
//## end module%38D9EC8C006F.declarations

//## begin module%38D9EC8C006F.additionalDeclarations preserve=yes
//## end module%38D9EC8C006F.additionalDeclarations


//## begin ClCachesSubsystem%355FDE2A03AF.preface preserve=yes
//## end ClCachesSubsystem%355FDE2A03AF.preface

//## Class: ClCachesSubsystem%355FDE2A03AF
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35606E0E00AA;ClCacheTest { -> F}
//## Uses: <unnamed>%38D9F08600CC;ClCachesVisitor { -> F}

class M4_DECL_M4CACHESUBSYSTEM ClCachesSubsystem : public ClSSImplementation  //## Inherits: <unnamed>%38D9EECF0310
{
  //## begin ClCachesSubsystem%355FDE2A03AF.initialDeclarations preserve=yes
  //## end ClCachesSubsystem%355FDE2A03AF.initialDeclarations

  public:
    //## begin ClCachesSubsystem::eCachesCommands%40B1BACA02E3.preface preserve=yes
    //## end ClCachesSubsystem::eCachesCommands%40B1BACA02E3.preface

    //## Class: eCachesCommands%40B1BACA02E3
    //## Category: M4CacheSubsystem%36AC6ED003DF
    //## Subsystem: M4CacheSubsystem%36AC6F60015B
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { CLEAN_ALL_CACHES = ClSSImplementation::TOTAL_COMMANDS,  TOTAL_COMMANDS} eCachesCommands;

    //## begin ClCachesSubsystem::eCachesCommands%40B1BACA02E3.postscript preserve=yes
    //## end ClCachesSubsystem::eCachesCommands%40B1BACA02E3.postscript

    //## Constructors (specified)
      //## Operation: ClCachesSubsystem%895476048
      ClCachesSubsystem (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClCachesSubsystem%895476049
      ~ClCachesSubsystem ();


    //## Other Operations (specified)
      //## Operation: GetCommandsId%895476025
      //	Devuleve los Ids y los literales de los comandsos que puede ejecutar el
      //	subsistema
      m4return_t GetCommandsId (M4DataStorage *ao_poDataStorage);

      //## Operation: ShutDown%895476021
      //	Hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: Synchronize%895476022
      //	Synchroniza los datos del subsistem, como por ejemplo la actualización de
      //	las estadísticas
      m4return_t Synchronize ();

      //## Operation: Initialize%895476023
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: ExecuteCommand%895476024
      //	Ejecuta un comando en concreto
      m4return_t ExecuteCommand (m4char_t ai_cCommand, M4DataStorage *ai_poParams, M4DataStorage *ai_poResult);

      //## Operation: GetCache%895476054
      ClCacheTest * GetCache (m4pchar_t ai_szName);

      //## Operation: ExecuteCommand%910862274
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%910862275
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%910862276
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: GetCommandParams%910862283
      m4return_t GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage);

      //## Operation: Start%958548531
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      virtual m4return_t Start ();

  public:
    // Additional Public Declarations
      //## begin ClCachesSubsystem%355FDE2A03AF.public preserve=yes
      //## end ClCachesSubsystem%355FDE2A03AF.public

  protected:

    //## Other Operations (specified)
      //## Operation: InitializeAttributes%1085388500
      m4return_t InitializeAttributes ();

    // Additional Protected Declarations
      //## begin ClCachesSubsystem%355FDE2A03AF.protected preserve=yes
      //## end ClCachesSubsystem%355FDE2A03AF.protected

  private:
    // Additional Private Declarations
      //## begin ClCachesSubsystem%355FDE2A03AF.private preserve=yes
	  m4return_t _CleanAllCaches () ;
      //## end ClCachesSubsystem%355FDE2A03AF.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCachesSubsystem%355FDE2A03AF.implementation preserve=yes
      //## end ClCachesSubsystem%355FDE2A03AF.implementation

};

//## begin ClCachesSubsystem%355FDE2A03AF.postscript preserve=yes
//## end ClCachesSubsystem%355FDE2A03AF.postscript

//## begin ClCachesStats%355FDE470357.preface preserve=yes
//## end ClCachesStats%355FDE470357.preface

//## Class: ClCachesStats%355FDE470357
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%355FE8680034;ClCachesSubsystem { -> F}

class ClCachesStats : public ClStatistic  //## Inherits: <unnamed>%355FDEF4039C
{
  //## begin ClCachesStats%355FDE470357.initialDeclarations preserve=yes
  //## end ClCachesStats%355FDE470357.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Reset%895476045; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%895476046; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%895476047
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

    // Additional Public Declarations
      //## begin ClCachesStats%355FDE470357.public preserve=yes
      //## end ClCachesStats%355FDE470357.public

  protected:
    // Additional Protected Declarations
      //## begin ClCachesStats%355FDE470357.protected preserve=yes
      //## end ClCachesStats%355FDE470357.protected

  private:
    // Additional Private Declarations
      //## begin ClCachesStats%355FDE470357.private preserve=yes
      //## end ClCachesStats%355FDE470357.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCachesStats%355FDE470357.implementation preserve=yes
      //## end ClCachesStats%355FDE470357.implementation

};

//## begin ClCachesStats%355FDE470357.postscript preserve=yes
//## end ClCachesStats%355FDE470357.postscript

//## begin ClCachesVisitor%355FDE7F00AF.preface preserve=yes
//## end ClCachesVisitor%355FDE7F00AF.preface

//## Class: ClCachesVisitor%355FDE7F00AF
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%355FDE9C00EC;ClCachesStats { -> F}
//## Uses: <unnamed>%355FDEB0007D;ClCachesSubsystem { -> F}

class ClCachesVisitor : public ClVisitor  //## Inherits: <unnamed>%355FDEE00203
{
  //## begin ClCachesVisitor%355FDE7F00AF.initialDeclarations preserve=yes
  //## end ClCachesVisitor%355FDE7F00AF.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitCaches%895476020
      void VisitCaches (ClCachesSubsystem &ai_oCaches);

    // Additional Public Declarations
      //## begin ClCachesVisitor%355FDE7F00AF.public preserve=yes
      //## end ClCachesVisitor%355FDE7F00AF.public

  protected:
    // Additional Protected Declarations
      //## begin ClCachesVisitor%355FDE7F00AF.protected preserve=yes
      //## end ClCachesVisitor%355FDE7F00AF.protected

  private:
    // Additional Private Declarations
      //## begin ClCachesVisitor%355FDE7F00AF.private preserve=yes
      //## end ClCachesVisitor%355FDE7F00AF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCachesVisitor%355FDE7F00AF.implementation preserve=yes
      //## end ClCachesVisitor%355FDE7F00AF.implementation

};

//## begin ClCachesVisitor%355FDE7F00AF.postscript preserve=yes
//## end ClCachesVisitor%355FDE7F00AF.postscript

// Class ClCachesSubsystem 

// Class ClCachesStats 

// Class ClCachesVisitor 

//## begin module%38D9EC8C006F.epilog preserve=yes
//## end module%38D9EC8C006F.epilog


#endif
