//## begin module%41E2B43A000D.cm preserve=no
//## end module%41E2B43A000D.cm

//## begin module%41E2B43A000D.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%41E2B43A000D.cp

//## Module: objdatacachemanager%41E2B43A000D; Package specification
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Source file: Z:\m4dm\inc\objdatacachemanager.hpp

#ifndef objdatacachemanager_h
#define objdatacachemanager_h 1

//## begin module%41E2B43A000D.additionalIncludes preserve=no
//## end module%41E2B43A000D.additionalIncludes

//## begin module%41E2B43A000D.includes preserve=yes
#include "m4dm_dll.hpp"
#include "datacache.hpp"
#include "m4regimpl.hpp"
#include "rollingfile.hpp"
//## end module%41E2B43A000D.includes

// m4types
#include <m4types.hpp>
// serializedchanneldata
#include <serializedchanneldata.hpp>
// ClCache
#include <clcache.hpp>
//## begin module%41E2B43A000D.declarations preserve=no
//## end module%41E2B43A000D.declarations

//## begin module%41E2B43A000D.additionalDeclarations preserve=yes
//## end module%41E2B43A000D.additionalDeclarations


//## begin ClObjDataCacheBase_t%41E3D9470110.preface preserve=yes
//## end ClObjDataCacheBase_t%41E3D9470110.preface

//## Class: ClObjDataCacheBase_t%41E3D9470110
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%41F4DA5D027F;ClSerializedChannelData { -> }

typedef ClBaseCache<ClSerializedChannelData, ClSerializedChannelDataContainer> ClObjDataCacheBase_t;
//## begin ClObjDataCacheBase_t%41E3D9470110.postscript preserve=yes
//## end ClObjDataCacheBase_t%41E3D9470110.postscript

//## begin ClObjDataCacheTree%4237FE720240.preface preserve=yes
//## end ClObjDataCacheTree%4237FE720240.preface

//## Class: ClObjDataCacheTree%4237FE720240
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClObjDataCacheTree : public ClCacheTree  //## Inherits: <unnamed>%4237FE78004C
{
  //## begin ClObjDataCacheTree%4237FE720240.initialDeclarations preserve=yes
public:
    typedef enum {M4_EVENT_GET=0, M4_EVENT_IGET, M4_EVENT_PUT, M4_EVENT_DELETE, M4_EVENT_DUMP_BEFORE, M4_EVENT_DUMP_AFTER} M4CacheEvent_t;
  //## end ClObjDataCacheTree%4237FE720240.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClObjDataCacheTree%1110965078
      ClObjDataCacheTree (m4uint32_t ai_iMemory, m4uint32_t ai_iObjects);

      ~ClObjDataCacheTree (void);

    //## Other Operations (specified)
      //## Operation: ClearDeletionQueue%1110965079
      m4return_t ClearDeletionQueue ();

      //## Operation: Trace%1110965081
      m4return_t Trace (M4CacheEvent_t ai_event, void* ai_poInfo, m4pcchar_t ai_pcObjectId, ClSerializedChannelData* ai_poObject);

      //## Operation: NumOfObjectsControl%1110965083
      virtual m4return_t NumOfObjectsControl ();

      //## Operation: SizeControl%1110965082
      virtual m4return_t SizeControl ();

      //## Operation: Refresh%1110965084
      m4return_t Refresh (ClSelfPersistiableObject *   ai_poSelfPersistor);

      //## Operation: GetObject%1112259308; C++
      //	Permite obtener un puntero a objeto de la cache. Si el objeto no existe en
      //	la cache o se obtiene un puntero a NULL devuelve M4_ERROR.
      //
      //	Si el puntero a objeto buscado existe se incrementa el atributo de Cl
      //	CacheableObject m_iRefCounter para indicar que existe un usuario más del
      //	objeto.
      //
      //	Argumentos:
      //
      //	ClCacheKey * ai_pKey : Puntero a la clave asociada al objeto.
      //	ClCacheableObject * ao_pObject: Argumento de salida para el objeto buscado.
      //
      //	m4uint32_t ai_iTimeOut: Tiempo límite de búsqueda en la cache.
      m4return_t GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode = GET_SHARED, m4int32_t  ai_iTimeOut = -1);

      //## Operation: UpdateObject%1112259309
      m4return_t UpdateObject (ClSerializedChannelData* ai_poSCD, ClChannel_Data* ai_poChannelData);

    // Additional Public Declarations
      //## begin ClObjDataCacheTree%4237FE720240.public preserve=yes
      //## end ClObjDataCacheTree%4237FE720240.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_poRollingFile%4238012603C7
      //## begin ClObjDataCacheTree::m_poRollingFile%4238012603C7.attr preserve=no  protected: ClRollingFile* {UA} NULL
      ClRollingFile* m_poRollingFile;
      //## end ClObjDataCacheTree::m_poRollingFile%4238012603C7.attr

    // Additional Protected Declarations
      //## begin ClObjDataCacheTree%4237FE720240.protected preserve=yes
      //## end ClObjDataCacheTree%4237FE720240.protected

  private:
    // Additional Private Declarations
      //## begin ClObjDataCacheTree%4237FE720240.private preserve=yes
      //## end ClObjDataCacheTree%4237FE720240.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjDataCacheTree%4237FE720240.implementation preserve=yes
      //## end ClObjDataCacheTree%4237FE720240.implementation

};

//## begin ClObjDataCacheTree%4237FE720240.postscript preserve=yes
//## end ClObjDataCacheTree%4237FE720240.postscript

//## begin ClObjDataCache%41E3C0FE0370.preface preserve=yes
//## end ClObjDataCache%41E3C0FE0370.preface

//## Class: ClObjDataCache%41E3C0FE0370
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClObjDataCache : public ClObjDataCacheBase_t  //## Inherits: <unnamed>%41E3D9B30111
{
  //## begin ClObjDataCache%41E3C0FE0370.initialDeclarations preserve=yes
  //## end ClObjDataCache%41E3C0FE0370.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClObjDataCache%1105443264
      ClObjDataCache (ClCache* ai_poCache, ClObjDataCacheTree* ai_poCacheTree, m4pcchar_t ai_pccPath, m4uint32_t ai_uiMaxMemory, m4uint32_t ai_uiNumObjects);

    //## Destructor (generated)
      virtual ~ClObjDataCache();


    //## Other Operations (specified)
      //## Operation: GetType%1105443265
      virtual m4uint16_t GetType ();

      //## Operation: RemoveObjectById%1105443266
      virtual m4return_t RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount = NULL);

      //## Operation: RemoveObjectByIdAndVersion%1105443267
      virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dateVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

      //## Operation: GetObject%1105443268
      m4return_t GetObject (m4pchar_t ai_pcObjectId, ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData);

      //## Operation: PutObject%1105443269
      m4return_t PutObject (m4pchar_t ai_pcObjectId, ClChannel_Data* ai_poChannelData);

      //## Operation: FreeAllReferences%1110965085
      m4return_t FreeAllReferences (ClSerializedChannelData* ai_poSCD);

    // Additional Public Declarations
      //## begin ClObjDataCache%41E3C0FE0370.public preserve=yes
      //## end ClObjDataCache%41E3C0FE0370.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oMutex%42380289002D
      //## begin ClObjDataCache::m_oMutex%42380289002D.attr preserve=no  protected: ClMutex {UA} 
      ClMutex m_oMutex;
      //## end ClObjDataCache::m_oMutex%42380289002D.attr

    // Data Members for Associations

      //## Association: m4dm::cache-auto::<unnamed>%4237FE920359
      //## Role: ClObjDataCache::m_poCacheTree%4237FE930222
      //## begin ClObjDataCache::m_poCacheTree%4237FE930222.role preserve=no  protected: ClObjDataCacheTree { -> RHAN}
      ClObjDataCacheTree *m_poCacheTree;
      //## end ClObjDataCache::m_poCacheTree%4237FE930222.role

    // Additional Protected Declarations
      //## begin ClObjDataCache%41E3C0FE0370.protected preserve=yes
      //## end ClObjDataCache%41E3C0FE0370.protected

  private:
    // Additional Private Declarations
      //## begin ClObjDataCache%41E3C0FE0370.private preserve=yes
      //## end ClObjDataCache%41E3C0FE0370.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjDataCache%41E3C0FE0370.implementation preserve=yes
      //## end ClObjDataCache%41E3C0FE0370.implementation

};

//## begin ClObjDataCache%41E3C0FE0370.postscript preserve=yes
//## end ClObjDataCache%41E3C0FE0370.postscript

//## begin ClObjDataCacheManager%41E2B3F003B7.preface preserve=yes
//## end ClObjDataCacheManager%41E2B3F003B7.preface

//## Class: ClObjDataCacheManager%41E2B3F003B7
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%41E3BE6E0192;m4return_t { -> }
//## Uses: <unnamed>%41FA1F1300F8; { -> }

class M4_DECL_M4DM ClObjDataCacheManager 
{
  //## begin ClObjDataCacheManager%41E2B3F003B7.initialDeclarations preserve=yes
  //## end ClObjDataCacheManager%41E2B3F003B7.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetObjDataCache%1105443262
      static ClObjDataCache* GetObjDataCache (ClVMBaseEnv* ai_poEnvironment);

      //## Operation: Init%1105443263
      static m4return_t Init (ClVMBaseEnv* ai_poEnvironment);

      //## Operation: Destroy%1110965086
      static m4return_t Destroy ();

    // Additional Public Declarations
      //## begin ClObjDataCacheManager%41E2B3F003B7.public preserve=yes
      //## end ClObjDataCacheManager%41E2B3F003B7.public

  protected:
    // Data Members for Associations

      //## Association: m4dm::<unnamed>%41E3C49C035C
      //## Role: ClObjDataCacheManager::m_poObjDataCache%41E3C49D0263
      //## begin ClObjDataCacheManager::m_poObjDataCache%41E3C49D0263.role preserve=no  protected: static ClObjDataCache { -> 1RHAN}
      static ClObjDataCache *m_poObjDataCache;
      //## end ClObjDataCacheManager::m_poObjDataCache%41E3C49D0263.role

    // Additional Protected Declarations
      //## begin ClObjDataCacheManager%41E2B3F003B7.protected preserve=yes
      //## end ClObjDataCacheManager%41E2B3F003B7.protected

  private:
    // Additional Private Declarations
      //## begin ClObjDataCacheManager%41E2B3F003B7.private preserve=yes
      //## end ClObjDataCacheManager%41E2B3F003B7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjDataCacheManager%41E2B3F003B7.implementation preserve=yes
      //## end ClObjDataCacheManager%41E2B3F003B7.implementation

};

//## begin ClObjDataCacheManager%41E2B3F003B7.postscript preserve=yes
//## end ClObjDataCacheManager%41E2B3F003B7.postscript

// Class ClObjDataCacheTree 

// Class ClObjDataCache 

// Class ClObjDataCacheManager 

//## begin module%41E2B43A000D.epilog preserve=yes
//## end module%41E2B43A000D.epilog


#endif
