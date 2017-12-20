//## begin module%355AD78D0078.cm preserve=no
//## end module%355AD78D0078.cm

//## begin module%355AD78D0078.cp preserve=no
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
//## end module%355AD78D0078.cp

//## Module: CacheSubsystem%355AD78D0078; Package specification
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Source file: f:\v400\M4CacheSubsystem\inc\cachesubsystem.hpp

#ifndef cachesubsystem_h
#define cachesubsystem_h 1

//## begin module%355AD78D0078.additionalIncludes preserve=no
//## end module%355AD78D0078.additionalIncludes

//## begin module%355AD78D0078.includes preserve=yes
#include <m4cachesubsystem_dll.hpp>
//## end module%355AD78D0078.includes

// ClCache
#include <clcache.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// Statistics
#include <statistics.hpp>
// propertiestable
#include <propertiestable.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%355AD78D0078.declarations preserve=no
//## end module%355AD78D0078.declarations

//## begin module%355AD78D0078.additionalDeclarations preserve=yes


class ClCacheSubSystem;

class ClSSStatsContainer;

class ClCacheStatsVisitor;

class ClCacheRecycleVisitor;

class ClCacheTest;

class ClCacheTree;

class ClCachePolicy;



//## end module%355AD78D0078.additionalDeclarations


//## begin ClCacheSubSystem%34FD993D00F4.preface preserve=yes
//## end ClCacheSubSystem%34FD993D00F4.preface

//## Class: ClCacheSubSystem%34FD993D00F4
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34FD9F68004B; { -> }
//## Uses: <unnamed>%3523E03B0285;ClOblConfiguration { -> }
//## Uses: <unnamed>%3523E06F023A;ClTables { -> }
//## Uses: <unnamed>%3650679700E3;ClAdminTagList { -> }

class M4_DECL_M4CACHESUBSYSTEM ClCacheSubSystem : public ClSSImplementation  //## Inherits: <unnamed>%38D9EECC03DE
{
  //## begin ClCacheSubSystem%34FD993D00F4.initialDeclarations preserve=yes
  //## end ClCacheSubSystem%34FD993D00F4.initialDeclarations

  public:
    //## begin ClCacheSubSystem::eCacheCommandsID%357F934200E3.preface preserve=yes
    //## end ClCacheSubSystem::eCacheCommandsID%357F934200E3.preface

    //## Class: eCacheCommandsID%357F934200E3
    //## Category: M4CacheSubsystem%36AC6ED003DF
    //## Subsystem: M4CacheSubsystem::inc%3798785C020E
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { CLEAN_CACHE = ClSSImplementation :: TOTAL_COMMANDS,REFRESH_CACHE,DUMP_REPORT,PERSIST_CACHE,CHANGE_REFRESH_RATIO,CHANGE_MAX_SIZE_CACHE,CHANGE_MAX_ELEM_IN_CACHE,CHANGE_CACHE_STATUS,CHANGE_DEFAULT_MAX_PERIOD,CHANGE_DEFAULT_MAX_UNUSED_PERIOD,TOTAL_COMMANDS} eCacheCommandsID;

    //## begin ClCacheSubSystem::eCacheCommandsID%357F934200E3.postscript preserve=yes
    //## end ClCacheSubSystem::eCacheCommandsID%357F934200E3.postscript

    //## begin ClCacheSubSystem::eCacheParamsId%3650649C0082.preface preserve=yes
    //## end ClCacheSubSystem::eCacheParamsId%3650649C0082.preface

    //## Class: eCacheParamsId%3650649C0082
    //## Category: M4CacheSubsystem%36AC6ED003DF
    //## Subsystem: M4CacheSubsystem::inc%3798785C020E
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { CACHE_REFRESH_RATIO,MAX_SIZE_CACHE,MAX_ELEM_IN_CACHE,CACHE_STATUS,DEFAULT_MAX_PERIOD,DEFAULT_MAX_UNUSED_PERIOD}  eCacheParamsId;

    //## begin ClCacheSubSystem::eCacheParamsId%3650649C0082.postscript preserve=yes
    //## end ClCacheSubSystem::eCacheParamsId%3650649C0082.postscript

    //## Constructors (specified)
      //## Operation: ClCacheSubSystem%927543267
      ClCacheSubSystem (ClConfiguration *  ai_poConfiguration = NULL, ClTables *  ai_poTables = NULL, ClNavigatorFirewall *ai_poNavigator = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCacheSubSystem%889088398
      ~ClCacheSubSystem ();


    //## Other Operations (specified)
      //## Operation: Synchronize%889088400
      m4return_t  Synchronize ();

      //## Operation: Initialize%890218027
      m4return_t Initialize ();

      //## Operation: ShutDown%890218028
      m4return_t ShutDown ();

      //## Operation: Accept%892555075
      m4return_t Accept (ClCacheStatsVisitor &   ai_oVisitor);

      //## Operation: GetCache%895146046
      ClCacheTest *  GetCache ();

      //## Operation: CreateCache%895146047
      void CreateCache (m4pchar_t ai_pcID_Cache = "SIN NOMBRE", m4uint32_t ai_ulCacheStyle = 0, m4uint32_t ai_ulMaxSizeCache = 10000000, m4uint32_t ai_ulMaxNumOfObjs = 1000, m4uint32_t ai_ulRefreshRatio = 50, m4double_t ai_dDefaultMaxPeriod = 0, m4double_t ai_dDefaultMaxUnusedPeriod = 0);

      //## Operation: GetCacheTree%895680861
      ClCacheTree *  GetCacheTree ();

      //## Operation: GetCacheName%895680862
      m4pchar_t GetCacheName ();

      //## Operation: Accept%897034126
      m4return_t Accept (ClCacheRecycleVisitor &  ai_poVisitor);

      //## Operation: GetCommandsId%911234943
      m4return_t GetCommandsId (ClAdminTagList &  aio_oCommandList);

      //## Operation: GetCommandParams%911234944
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &  aio_oParamList);

      //## Operation: ExecuteCommand%911234945
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &  ai_oParamList, ClAdminTagList &  ao_oResultList);

      //## Operation: Update%942334657
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: GetElement%953806192
      //	Función virtual (al igual que todas las de esta clase) que sobrecargada en
      //	otra clase que herede de esta, devolverá culaquier elemento contenido por el
      //	subsistema, en forma de un puntero a void.
      //	El elemento que ha de devolver, se indica con una cadena pasada como
      //	parámetro de entrada.
      //	Elementos pueden ser todas aquellas referencias que el subsistema contenga,
      //	que por agrupación formen el conjunto lógico denominado subsistema.
      //	Por ejemplo, el subsistema sesión tendrá un elemento que es la sesión, el
      //	subsistema servicio tendrá otro que sea la cola de servicio y otro que sea
      //	en propio servicio, etc...
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: Start%958033384
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      virtual m4return_t Start ();

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%3577DBE600E1
      //## Role: ClCacheSubSystem::m_poRecycleVisitor%3577DBE70363
      //## begin ClCacheSubSystem::m_poRecycleVisitor%3577DBE70363.role preserve=no  public: ClVisitor { -> RHAN}
      ClVisitor *m_poRecycleVisitor;
      //## end ClCacheSubSystem::m_poRecycleVisitor%3577DBE70363.role

  public:
    // Additional Public Declarations
      //## begin ClCacheSubSystem%34FD993D00F4.public preserve=yes
      //## end ClCacheSubSystem%34FD993D00F4.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheSubSystem%34FD993D00F4.protected preserve=yes
      //## end ClCacheSubSystem%34FD993D00F4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcCacheName%355B10B10339
      //## begin ClCacheSubSystem::m_pcCacheName%355B10B10339.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcCacheName;
      //## end ClCacheSubSystem::m_pcCacheName%355B10B10339.attr

      //## Attribute: m_eMemoryLevel%38312E110220
      //## begin ClCacheSubSystem::m_eMemoryLevel%38312E110220.attr preserve=no  private: EVENTTYPE {UA} 
      EVENTTYPE m_eMemoryLevel;
      //## end ClCacheSubSystem::m_eMemoryLevel%38312E110220.attr

      //## Attribute: m_eOldMemoryLevel%38312E7000A0
      //## begin ClCacheSubSystem::m_eOldMemoryLevel%38312E7000A0.attr preserve=no  private: EVENTTYPE {UA} 
      EVENTTYPE m_eOldMemoryLevel;
      //## end ClCacheSubSystem::m_eOldMemoryLevel%38312E7000A0.attr

      //## Attribute: m_ulOldMaxSizeCache%383130C001D8
      //## begin ClCacheSubSystem::m_ulOldMaxSizeCache%383130C001D8.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulOldMaxSizeCache;
      //## end ClCacheSubSystem::m_ulOldMaxSizeCache%383130C001D8.attr

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%353F91F702C7
      //## Role: ClCacheSubSystem::m_poClCacheTest%353F91F801F6
      //## begin ClCacheSubSystem::m_poClCacheTest%353F91F801F6.role preserve=no  private: ClCacheTest { -> RHAN}
      ClCacheTest *m_poClCacheTest;
      //## end ClCacheSubSystem::m_poClCacheTest%353F91F801F6.role

      //## Association: M4Cache::<unnamed>%353F921E020F
      //## Role: ClCacheSubSystem::m_poClCachePolicy%353F921F0170
      //## begin ClCacheSubSystem::m_poClCachePolicy%353F921F0170.role preserve=no  private: ClCachePolicy { -> RHAN}
      ClCachePolicy *m_poClCachePolicy;
      //## end ClCacheSubSystem::m_poClCachePolicy%353F921F0170.role

      //## Association: M4Cache::<unnamed>%3562FA190356
      //## Role: ClCacheSubSystem::m_poClCacheTree%3562FA1A034E
      //## begin ClCacheSubSystem::m_poClCacheTree%3562FA1A034E.role preserve=no  private: ClCacheTree { -> RFHAN}
      ClCacheTree *m_poClCacheTree;
      //## end ClCacheSubSystem::m_poClCacheTree%3562FA1A034E.role

    // Additional Private Declarations
      //## begin ClCacheSubSystem%34FD993D00F4.private preserve=yes
      //## end ClCacheSubSystem%34FD993D00F4.private

  private: //## implementation
    // Data Members for Class Attributes

      //## Attribute: m_ulOldMaxNumOfObjs%383130E30337
      //## begin ClCacheSubSystem::m_ulOldMaxNumOfObjs%383130E30337.attr preserve=no  implementation: m4uint32_t {UA} 
      m4uint32_t m_ulOldMaxNumOfObjs;
      //## end ClCacheSubSystem::m_ulOldMaxNumOfObjs%383130E30337.attr

    // Additional Implementation Declarations
      //## begin ClCacheSubSystem%34FD993D00F4.implementation preserve=yes
      //## end ClCacheSubSystem%34FD993D00F4.implementation

};

//## begin ClCacheSubSystem%34FD993D00F4.postscript preserve=yes
//## end ClCacheSubSystem%34FD993D00F4.postscript

//## begin ClCacheRecycleVisitor%3577DC33018C.preface preserve=yes
//## end ClCacheRecycleVisitor%3577DC33018C.preface

//## Class: ClCacheRecycleVisitor%3577DC33018C
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCacheRecycleVisitor : public ClVisitor  //## Inherits: <unnamed>%3577DC3903B1
{
  //## begin ClCacheRecycleVisitor%3577DC33018C.initialDeclarations preserve=yes
  //## end ClCacheRecycleVisitor%3577DC33018C.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitCache%897034125
      m4return_t VisitCache (ClCacheTree &  ai_oVisitCache);

    // Additional Public Declarations
      //## begin ClCacheRecycleVisitor%3577DC33018C.public preserve=yes
      //## end ClCacheRecycleVisitor%3577DC33018C.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheRecycleVisitor%3577DC33018C.protected preserve=yes
      //## end ClCacheRecycleVisitor%3577DC33018C.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheRecycleVisitor%3577DC33018C.private preserve=yes
      //## end ClCacheRecycleVisitor%3577DC33018C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheRecycleVisitor%3577DC33018C.implementation preserve=yes
      //## end ClCacheRecycleVisitor%3577DC33018C.implementation

};

//## begin ClCacheRecycleVisitor%3577DC33018C.postscript preserve=yes
//## end ClCacheRecycleVisitor%3577DC33018C.postscript

//## begin ClCacheStats%35055B4B00E7.preface preserve=yes
//## end ClCacheStats%35055B4B00E7.preface

//## Class: ClCacheStats%35055B4B00E7
//	Clase que permite almacenar las estadísticas de la cache y de donde las
//	recogerá el visitor
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHESUBSYSTEM ClCacheStats : public ClStatistic  //## Inherits: <unnamed>%35055B61000C
{
  //## begin ClCacheStats%35055B4B00E7.initialDeclarations preserve=yes
  //## end ClCacheStats%35055B4B00E7.initialDeclarations

  public:
    //## Class: eCacheStatsID%3562FA6E027C
    //## Category: M4CacheSubsystem%36AC6ED003DF
    //## Subsystem: M4CacheSubsystem::inc%3798785C020E
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { SIZE_CACHE= ClStatistic::TOTAL_NUM,NUM_OF_OBJS_IN_CACHE , OBJS_IN_QUEUE,OBJS_IN_LRU,NUM_OF_REF_TO_CACHE, HIT_RATIO,TOTAL_NUM } eCacheStatsID;

    //## Constructors (specified)
      //## Operation: ClCacheStats%889521624
      ClCacheStats ();

    //## Destructor (specified)
      //## Operation: ~ClCacheStats%889521625
      ~ClCacheStats ();


    //## Other Operations (specified)
      //## Operation: Reset%889629076
      //	Permite realizar un reset de las estadísticas
      m4return_t Reset ();

      //## Operation: SetNOfRef%889694507
      //	Actualiza la estadística de número de referencias a la caché
      m4return_t SetNOfRef (ClStatNumericalType ai_oNOfRef);

      //## Operation: SetNOfObjInCache%889694509
      //	Actualiza la estadística de número de objetos en la caché
      m4return_t SetNOfObjInCache (ClStatNumericalType ai_oNObjInCache);

      //## Operation: SetNOfObjInQueue%889694508
      //	Actualiza la estadística de número de objetos en la cola de borrado
      m4return_t SetNOfObjInQueue (ClStatNumericalType ai_oNObjInQueue);

      //## Operation: GetStat%889777601
      //	Permite obtener las estadísticas
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: GetNumberOfStats%889777602
      //	Devuelve el número de estadísticas que se pueden obtener de la caché
      m4uint32_t GetNumberOfStats ();

      //## Operation: ResetStat%889777603
      m4return_t ResetStat (ClStatHandle &  ai_hStatld);

      //## Operation: SetHitsRatio%889863966
      //	Actualiza la estadística de porcentaje de aciertos
      m4return_t SetHitsRatio (ClStatPercentType ai_oHitRatio);

      //## Operation: SetDefaultPolicy%889863967
      //	Actualiza la estadística de la política por defecto de la caché
      m4return_t SetDefaultPolicy (m4uint32_t ai_ulDefaultPolicy);

      //## Operation: SetSizeCache%895680863
      m4return_t SetSizeCache (ClStatPercentType ai_ulSizeCache);

      //## Operation: SetMaxNOfObjs%897498325
      m4return_t SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs);

      //## Operation: SetMaxSizeCache%897498326
      m4return_t SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache);

      //## Operation: SetRefreshRatio%897498327
      m4return_t SetRefreshRatio (m4uint32_t ai_ulRefreshRatio);

      //## Operation: SetNOfObjInLRU%911234946
      m4return_t SetNOfObjInLRU (m4uint32_t ai_ulNOfObjInLRU);

  public:
    // Additional Public Declarations
      //## begin ClCacheStats%35055B4B00E7.public preserve=yes
      //## end ClCacheStats%35055B4B00E7.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheStats%35055B4B00E7.protected preserve=yes
      //## end ClCacheStats%35055B4B00E7.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_onOfReferences%3507BAA70312
      //	Número de referencias a objetos de la caché
      //## begin ClCacheStats::m_onOfReferences%3507BAA70312.attr preserve=no  private: ClStatNumericalType {VA} 0
      ClStatNumericalType m_onOfReferences;
      //## end ClCacheStats::m_onOfReferences%3507BAA70312.attr

      //## Attribute: m_onObjInQueue%3507BB0E00F4
      //	Nuúmero de objetos en la cola de borrado
      //## begin ClCacheStats::m_onObjInQueue%3507BB0E00F4.attr preserve=no  private: ClStatNumericalType {UA} 0
      ClStatNumericalType m_onObjInQueue;
      //## end ClCacheStats::m_onObjInQueue%3507BB0E00F4.attr

      //## Attribute: m_onObjInCache%3507BB2B0023
      //	Número de objetos existentes en la caché
      //## begin ClCacheStats::m_onObjInCache%3507BB2B0023.attr preserve=no  private: ClStatNumericalType {UA} 0
      ClStatNumericalType m_onObjInCache;
      //## end ClCacheStats::m_onObjInCache%3507BB2B0023.attr

      //## Attribute: m_oHitRatio%350CEA2F02D9
      //	Porcentaje de aciertos de acceso a la caché
      //## begin ClCacheStats::m_oHitRatio%350CEA2F02D9.attr preserve=no  private: ClStatPercentType {UA} 0
      ClStatPercentType m_oHitRatio;
      //## end ClCacheStats::m_oHitRatio%350CEA2F02D9.attr

      //## Attribute: m_ulMinPerTime%350E395A024F
      //	Tiempo mínimo de persistencia en la caché de la política por defecto
      //## begin ClCacheStats::m_ulMinPerTime%350E395A024F.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulMinPerTime;
      //## end ClCacheStats::m_ulMinPerTime%350E395A024F.attr

      //## Attribute: m_oLastPersist%350E93240107
      //	Fecha de último volcado a disco de la caché
      //## begin ClCacheStats::m_oLastPersist%350E93240107.attr preserve=no  private: M4ClString  {UA} 
      M4ClString  m_oLastPersist;
      //## end ClCacheStats::m_oLastPersist%350E93240107.attr

      //## Attribute: m_oRecycleInterval%350E938001BE
      //	Intervalo de chequeo de la caché
      //## begin ClCacheStats::m_oRecycleInterval%350E938001BE.attr preserve=no  private: M4ClString  {UA} 
      M4ClString  m_oRecycleInterval;
      //## end ClCacheStats::m_oRecycleInterval%350E938001BE.attr

      //## Attribute: m_oSizeCache%354EE891003C
      //## begin ClCacheStats::m_oSizeCache%354EE891003C.attr preserve=no  private: ClStatPercentType {UA} 
      ClStatPercentType m_oSizeCache;
      //## end ClCacheStats::m_oSizeCache%354EE891003C.attr

      //## Attribute: m_ulMaxSizeCache%357EBDAD0210
      //## begin ClCacheStats::m_ulMaxSizeCache%357EBDAD0210.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulMaxSizeCache;
      //## end ClCacheStats::m_ulMaxSizeCache%357EBDAD0210.attr

      //## Attribute: m_ulMaxNOfObjs%357EBDC400B4
      //## begin ClCacheStats::m_ulMaxNOfObjs%357EBDC400B4.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulMaxNOfObjs;
      //## end ClCacheStats::m_ulMaxNOfObjs%357EBDC400B4.attr

      //## Attribute: m_ulRefreshRatio%357EBDD70379
      //## begin ClCacheStats::m_ulRefreshRatio%357EBDD70379.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulRefreshRatio;
      //## end ClCacheStats::m_ulRefreshRatio%357EBDD70379.attr

      //## Attribute: m_ulObjInLRU%365061F30227
      //## begin ClCacheStats::m_ulObjInLRU%365061F30227.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulObjInLRU;
      //## end ClCacheStats::m_ulObjInLRU%365061F30227.attr

      //## Attribute: m_ulDefaultPolicy%3742DC180030
      //## begin ClCacheStats::m_ulDefaultPolicy%3742DC180030.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_ulDefaultPolicy;
      //## end ClCacheStats::m_ulDefaultPolicy%3742DC180030.attr

    // Additional Private Declarations
      //## begin ClCacheStats%35055B4B00E7.private preserve=yes
      //## end ClCacheStats%35055B4B00E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheStats%35055B4B00E7.implementation preserve=yes
      //## end ClCacheStats%35055B4B00E7.implementation

};

//## begin ClCacheStats%35055B4B00E7.postscript preserve=yes
//## end ClCacheStats%35055B4B00E7.postscript

//## begin ClCacheStatsVisitor%34FD9E4801D7.preface preserve=yes
//## end ClCacheStatsVisitor%34FD9E4801D7.preface

//## Class: ClCacheStatsVisitor%34FD9E4801D7
//	Clase derivada de clvisitor que permite acceder a la caché y recoger las
//	estadisticas
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3503C86402FB;ClCacheStats { -> }
//## Uses: <unnamed>%3504381A02C1;ClCacheStyle { -> F}

class M4_DECL_M4CACHESUBSYSTEM ClCacheStatsVisitor : public ClVisitor  //## Inherits: <unnamed>%34FD9E5C00F0
{
  //## begin ClCacheStatsVisitor%34FD9E4801D7.initialDeclarations preserve=yes
  //## end ClCacheStatsVisitor%34FD9E4801D7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheStatsVisitor%889629077
      ClCacheStatsVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitCache%889088404; C++
      m4return_t VisitCache (ClCacheTree &  ai_oVisitCache);

    // Additional Public Declarations
      //## begin ClCacheStatsVisitor%34FD9E4801D7.public preserve=yes
      //## end ClCacheStatsVisitor%34FD9E4801D7.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheStatsVisitor%34FD9E4801D7.protected preserve=yes
      //## end ClCacheStatsVisitor%34FD9E4801D7.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheStatsVisitor%34FD9E4801D7.private preserve=yes
      //## end ClCacheStatsVisitor%34FD9E4801D7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheStatsVisitor%34FD9E4801D7.implementation preserve=yes
      //## end ClCacheStatsVisitor%34FD9E4801D7.implementation

};

//## begin ClCacheStatsVisitor%34FD9E4801D7.postscript preserve=yes
//## end ClCacheStatsVisitor%34FD9E4801D7.postscript

//## begin ClCacheRecycleVisit%3577D60B0258.preface preserve=yes
//## end ClCacheRecycleVisit%3577D60B0258.preface

//## Class: ClCacheRecycleVisit%3577D60B0258
//## Category: M4CacheSubsystem%36AC6ED003DF
//## Subsystem: M4CacheSubsystem::inc%3798785C020E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCacheRecycleVisit : public ClVisitor  //## Inherits: <unnamed>%3577D60F0380
{
  //## begin ClCacheRecycleVisit%3577D60B0258.initialDeclarations preserve=yes
  //## end ClCacheRecycleVisit%3577D60B0258.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheRecycleVisit%897034124
      ClCacheRecycleVisit ();

    // Additional Public Declarations
      //## begin ClCacheRecycleVisit%3577D60B0258.public preserve=yes
      //## end ClCacheRecycleVisit%3577D60B0258.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheRecycleVisit%3577D60B0258.protected preserve=yes
      //## end ClCacheRecycleVisit%3577D60B0258.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheRecycleVisit%3577D60B0258.private preserve=yes
      //## end ClCacheRecycleVisit%3577D60B0258.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheRecycleVisit%3577D60B0258.implementation preserve=yes
      //## end ClCacheRecycleVisit%3577D60B0258.implementation

};

//## begin ClCacheRecycleVisit%3577D60B0258.postscript preserve=yes
//## end ClCacheRecycleVisit%3577D60B0258.postscript

// Class ClCacheSubSystem 

// Class ClCacheRecycleVisitor 

// Class ClCacheStats 

// Class ClCacheStatsVisitor 

// Class ClCacheRecycleVisit 

//## begin module%355AD78D0078.epilog preserve=yes
//## end module%355AD78D0078.epilog


#endif
