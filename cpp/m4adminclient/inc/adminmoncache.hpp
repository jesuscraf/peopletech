//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              adminmoncache.hpp
//	 Project:           M4AdminClient
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines admin monitor cache classes.
//
//	==============================================================================
//## end module.cp

//## Module: MonitorCache; Package specification
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\inc\adminmoncache.hpp

#ifndef monitorcache_h
#define monitorcache_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
// m4types
#include <m4types.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClAdminInfoTagCache
//	Esta clase es el elemento básico para almacenar un
//	elemento básico de información.
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminInfoTagCache 
{
  //## begin ClAdminInfoTagCache.initialDeclarations preserve=yes
  //## end ClAdminInfoTagCache.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminInfoTagCache%917339823
      ClAdminInfoTagCache ();


    //## Other Operations (specified)
      //## Operation: Set%917339807
      //	Salva el valor en la cache.
      //	Return:
      //	   M4_TRUE: Ok
      //	   M4_FALSE: error
      m4bool_t Set (const M4ClString &ai_Info, int ai_InfoId = -1);

      //## Operation: Get%917339808
      //	Devuelve el valor almacenado en la cache.
      //	return:
      //	   M4_TRUE: el dato de la cache es válido.
      //	   M4_FALSE: el dato no es válido
      m4bool_t Get (M4ClString &ao_Info);

      //## Operation: Reset%917339815
      //	Pone el atributo m_ IsValid a false.
      void Reset ();

      //## Operation: GetId%917426673
      int GetId ();

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36AE0AC501FE
      //## Role: ClAdminInfoTagCache::<m_pClAdminInfoCache>
      //## begin ClAdminInfoTagCache::<m_pClAdminInfoCache>.role preserve=no  public: ClAdminInfoCache {n -> RHGA}
      //## end ClAdminInfoTagCache::<m_pClAdminInfoCache>.role

      //## Association: M4AdminClient::<unnamed>%36AE0B940314
      //## Role: ClAdminInfoTagCache::<m_pClAdminInfoCache>
      //## begin ClAdminInfoTagCache::<m_pClAdminInfoCache>.role preserve=no  public: ClAdminInfoCache {n -> RHGA}
      //## end ClAdminInfoTagCache::<m_pClAdminInfoCache>.role

      //## Association: M4AdminClient::<unnamed>%36AE0B9A01AA
      //## Role: ClAdminInfoTagCache::<m_pClAdminInfoCache>
      //## begin ClAdminInfoTagCache::<m_pClAdminInfoCache>.role preserve=no  public: ClAdminInfoCache {n -> RHGA}
      //## end ClAdminInfoTagCache::<m_pClAdminInfoCache>.role

    // Additional Public Declarations
      //## begin ClAdminInfoTagCache.public preserve=yes
      //## end ClAdminInfoTagCache.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminInfoTagCache.protected preserve=yes
      //## end ClAdminInfoTagCache.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_Info
      //	Almacena la informacion del atributo o subsistema que se
      //	trae del servidor.
      //## begin ClAdminInfoTagCache::m_Info.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_Info;
      //## end ClAdminInfoTagCache::m_Info.attr

      //## Attribute: m_IsValid
      //	Indica si el dato en la cache es valido o no. Se pone a
      //	M4_FALSE cada vez que se lee la cache, y se pone a
      //	M4_TRUE cuando se escribe en la cache.
      //## begin ClAdminInfoTagCache::m_IsValid.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_IsValid;
      //## end ClAdminInfoTagCache::m_IsValid.attr

      //## Attribute: m_InfoId
      //	Este es el valor del identificador del atributo
      //	almacenado en este item.
      //## begin ClAdminInfoTagCache::m_InfoId.attr preserve=no  private: int {UA} -1
      int m_InfoId;
      //## end ClAdminInfoTagCache::m_InfoId.attr

    // Additional Private Declarations
      //## begin ClAdminInfoTagCache.private preserve=yes
      //## end ClAdminInfoTagCache.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminInfoTagCache.implementation preserve=yes
      //## end ClAdminInfoTagCache.implementation

};

//## begin ClAdminInfoTagCache.postscript preserve=yes
//## end ClAdminInfoTagCache.postscript



//## Class: ClAdminInfoCache
//	Almacena la informacion de un subsistema para
//	posteriores sentencias del monitor de administracion.
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; m4bool_t { -> }
//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; deque { -> }

class ClAdminInfoCache 
{
  //## begin ClAdminInfoCache.initialDeclarations preserve=yes
  //## end ClAdminInfoCache.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: SetIds%917339816
      m4return_t SetIds (const M4ClString &ai_Path, const M4ClString &ai_Ids);

      //## Operation: SetAttributes%917339817
      m4return_t SetAttributes (const M4ClString &ai_Path, const M4ClString &ai_Attributes);

      //## Operation: SetStatistics%917339818
      m4return_t SetStatistics (const M4ClString &ai_Path, const M4ClString &ai_Statistics);

      //## Operation: SetCommands%917339819
      m4return_t SetCommands (const M4ClString &ai_Path, const M4ClString &ai_Commands);

      //## Operation: SetAttributeValues%917339820
      m4return_t SetAttributeValues (const M4ClString &ai_Path, int ai_nAttributes = 0, const M4ClString *ai_Attributes = NULL, const int *ai_AttributesId = NULL);

      //## Operation: SetCommandParams%917339821
      m4return_t SetCommandParams (const M4ClString &ai_Path, int ai_nCommandParams = 0, const M4ClString *ai_CommandParams = NULL, const int *ai_ParamsId = NULL);

      //## Operation: SetStatisticValues%917339822
      m4return_t SetStatisticValues (const M4ClString &ai_Path, int ai_nStatistics = 0, const M4ClString *ai_Statistics = NULL, const int *ai_StatisticsId = NULL);

      //## Operation: GetIds%917426675
      m4bool_t GetIds (const M4ClString &ai_Path, M4ClString &ao_Ids);

      //## Operation: GetAttributes%917426666
      m4bool_t GetAttributes (const M4ClString &ai_Path, M4ClString &ao_Attributes);

      //## Operation: GetStatistics%917426667
      m4bool_t GetStatistics (const M4ClString &ai_Path, M4ClString &ao_Statistics);

      //## Operation: GetCommands%917426668
      m4bool_t GetCommands (const M4ClString &ai_Path, M4ClString &ao_Commands);

      //## Operation: GetAttributeValue%917426669
      m4bool_t GetAttributeValue (const M4ClString &ai_Path, int ai_AttributeId, M4ClString &ao_Attribute);

      //## Operation: GetStatisticValue%917426670
      m4bool_t GetStatisticValue (const M4ClString &ai_Path, int ai_StatisticId, M4ClString &ao_Statistic);

      //## Operation: GetCommandParams%917426671
      m4bool_t GetCommandParams (const M4ClString &ai_Path, int ai_CommandId, M4ClString &ao_Params);

      //## Operation: Reset%917426674
      //	Este método borra todos los datos de la cache.
      void Reset ();

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36AF460A03BB
      //## Role: ClAdminInfoCache::<m_pClAdminMonClient>
      //## begin ClAdminInfoCache::<m_pClAdminMonClient>.role preserve=no  public: ClAdminMonClient { -> RHGA}
      //## end ClAdminInfoCache::<m_pClAdminMonClient>.role

    // Additional Public Declarations
      //## begin ClAdminInfoCache.public preserve=yes
      //## end ClAdminInfoCache.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminInfoCache.protected preserve=yes
      //## end ClAdminInfoCache.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_Path
      //## begin ClAdminInfoCache::m_Path.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_Path;
      //## end ClAdminInfoCache::m_Path.attr

      //## Attribute: m_Ids
      //## begin ClAdminInfoCache::m_Ids.attr preserve=no  private: ClAdminInfoTagCache {UA} 
      ClAdminInfoTagCache m_Ids;
      //## end ClAdminInfoCache::m_Ids.attr

      //## Attribute: m_Attributes
      //## begin ClAdminInfoCache::m_Attributes.attr preserve=no  private: ClAdminInfoTagCache {UA} 
      ClAdminInfoTagCache m_Attributes;
      //## end ClAdminInfoCache::m_Attributes.attr

      //## Attribute: m_Statistics
      //## begin ClAdminInfoCache::m_Statistics.attr preserve=no  private: ClAdminInfoTagCache {UA} 
      ClAdminInfoTagCache m_Statistics;
      //## end ClAdminInfoCache::m_Statistics.attr

      //## Attribute: m_Commands
      //## begin ClAdminInfoCache::m_Commands.attr preserve=no  private: ClAdminInfoTagCache {UA} 
      ClAdminInfoTagCache m_Commands;
      //## end ClAdminInfoCache::m_Commands.attr

      //## Attribute: m_nAttributes
      //## begin ClAdminInfoCache::m_nAttributes.attr preserve=no  private: int {UA} 0
      int m_nAttributes;
      //## end ClAdminInfoCache::m_nAttributes.attr

      //## Attribute: m_nStatistics
      //## begin ClAdminInfoCache::m_nStatistics.attr preserve=no  private: int {UA} 0
      int m_nStatistics;
      //## end ClAdminInfoCache::m_nStatistics.attr

      //## Attribute: m_nCommandParams
      //## begin ClAdminInfoCache::m_nCommandParams.attr preserve=no  private: int {UA} 0
      int m_nCommandParams;
      //## end ClAdminInfoCache::m_nCommandParams.attr

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36AE0AC501FE
      //## Role: ClAdminInfoCache::m_AttributeValues
      //## begin ClAdminInfoCache::m_AttributeValues.role preserve=no  private: ClAdminInfoTagCache { -> nUHAN}
      deque<ClAdminInfoTagCache> m_AttributeValues;
      //## end ClAdminInfoCache::m_AttributeValues.role

      //## Association: M4AdminClient::<unnamed>%36AE0B940314
      //## Role: ClAdminInfoCache::m_StatisticValues
      //## begin ClAdminInfoCache::m_StatisticValues.role preserve=no  private: ClAdminInfoTagCache { -> nUHAN}
      deque<ClAdminInfoTagCache> m_StatisticValues;
      //## end ClAdminInfoCache::m_StatisticValues.role

      //## Association: M4AdminClient::<unnamed>%36AE0B9A01AA
      //## Role: ClAdminInfoCache::m_CommandParams
      //## begin ClAdminInfoCache::m_CommandParams.role preserve=no  private: ClAdminInfoTagCache { -> nUHAN}
      deque<ClAdminInfoTagCache> m_CommandParams;
      //## end ClAdminInfoCache::m_CommandParams.role

    // Additional Private Declarations
      //## begin ClAdminInfoCache.private preserve=yes
      //## end ClAdminInfoCache.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminInfoCache.implementation preserve=yes
      //## end ClAdminInfoCache.implementation

};

//## begin ClAdminInfoCache.postscript preserve=yes
//## end ClAdminInfoCache.postscript



// Class ClAdminInfoTagCache 





// Class ClAdminInfoCache 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
