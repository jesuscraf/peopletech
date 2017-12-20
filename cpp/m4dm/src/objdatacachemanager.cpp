//## begin module%41E2B44D0359.cm preserve=no
//## end module%41E2B44D0359.cm

//## begin module%41E2B44D0359.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
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
//## end module%41E2B44D0359.cp

//## Module: objdatacachemanager%41E2B44D0359; Package body
//## Subsystem: m4dm::cache-auto::src%41E2B42502DC
//## Source file: Z:\m4dm\src\objdatacachemanager.cpp

//## begin module%41E2B44D0359.additionalIncludes preserve=no
//## end module%41E2B44D0359.additionalIncludes

//## begin module%41E2B44D0359.includes preserve=yes
#include "blocksynchronization.hpp"
#include "file_manager.hpp"
//## end module%41E2B44D0359.includes

// m4types
#include <m4types.hpp>
// objdatacachemanager
#include <objdatacachemanager.hpp>
// ClCache
#include <clcache.hpp>
//## begin module%41E2B44D0359.declarations preserve=no
//## end module%41E2B44D0359.declarations

//## begin module%41E2B44D0359.additionalDeclarations preserve=yes
//#define _M4_DUMP_OBJDATA_CACHE_ACTIONS
#ifdef _M4_DUMP_OBJDATA_CACHE_ACTIONS
#   include "m4thread.hpp"
    fstream fosCacheObjects("c:\\temp\\cacheobjects.txt", ios::out);
#   define DUMP_OBJDATA_CACHE_ACTIONS(action, object, address) fosCacheObjects << M4Thread::GetCurrentIdThread() << " - " << action << "(" << object << ", " << address << ")" << endl;
#else
#   define DUMP_OBJDATA_CACHE_ACTIONS(action, object, address) ;
#endif
//## end module%41E2B44D0359.additionalDeclarations


// Class ClObjDataCacheManager 

//## begin ClObjDataCacheManager::m_poObjDataCache%41E3C49D0263.role preserve=no  protected: static ClObjDataCache { -> 1RHAN}
ClObjDataCache *ClObjDataCacheManager::m_poObjDataCache = NULL;
//## end ClObjDataCacheManager::m_poObjDataCache%41E3C49D0263.role


//## Other Operations (implementation)
ClObjDataCache* ClObjDataCacheManager::GetObjDataCache (ClVMBaseEnv* ai_poEnvironment)
{
  //## begin ClObjDataCacheManager::GetObjDataCache%1105443262.body preserve=yes
    static ClMutex oLocalMutex(M4_TRUE);

    if (NULL == m_poObjDataCache)
    {
        ClMutBlock oBlock(&oLocalMutex);

        if (NULL == m_poObjDataCache)
        {
            // initialize the object data cache.
            ClObjDataCacheManager::Init(ai_poEnvironment);
        }
    }

    return m_poObjDataCache;
  //## end ClObjDataCacheManager::GetObjDataCache%1105443262.body
}

m4return_t ClObjDataCacheManager::Init (ClVMBaseEnv* ai_poEnvironment)
{
  //## begin ClObjDataCacheManager::Init%1105443263.body preserve=yes
    if (NULL == ai_poEnvironment)
    {
        return M4_ERROR;
    }

    m4pcchar_t pcDir = ai_poEnvironment->GetCacheDirectory();
    m4uint32_t iMemory = ai_poEnvironment->GetObjDataCacheMaxMemory();
    m4uint32_t iObjects = ai_poEnvironment->GetObjDataCacheMaxNumObjects();

    if (NULL == pcDir) pcDir = ".";
    if (iMemory <= 0) iMemory = 30000000;
    if (iObjects <= 0) iObjects = 1024;

    // bugid 102467.
    // elimino el directorio de cache de DELTA. La solución no es muy buena, ya que aquí
    // pongo el nombre del subdirectorio de DELTA, y en la caché también está, pero no
    // tengo forma de saberlo.
    m4string_t sAutoDirName = m4string_t(pcDir) + M4_BACK_SLASH + m4string_t("DELTA");
    ClFileDirOperations::RemoveDirectory(sAutoDirName.c_str());

	// Creamos la nueva base de cache
	ClObjDataCacheTree* poCacheTree = new ClObjDataCacheTree(iMemory, iObjects);
	ClCacheTest* poCache = new ClCacheTest();
	poCache->SetCacheStyle(poCacheTree);

    // creo la cache.
    m_poObjDataCache = new ClObjDataCache(
        poCache,        // the cache
        poCacheTree,
		pcDir,          // CacheDirectory 
		iMemory,        // m_iMaxMemory,
		iObjects        // m_iMaxNumObjects
        );

    if (m_poObjDataCache == NULL)
    {
		return( M4_ERROR  ) ;
    }

    return M4_SUCCESS;
  //## end ClObjDataCacheManager::Init%1105443263.body
}

m4return_t ClObjDataCacheManager::Destroy ()
{
  //## begin ClObjDataCacheManager::Destroy%1110965086.body preserve=yes
    if (NULL != m_poObjDataCache)
    {
        delete m_poObjDataCache;
        m_poObjDataCache = NULL;
    }

    return M4_SUCCESS;
  //## end ClObjDataCacheManager::Destroy%1110965086.body
}

// Additional Declarations
  //## begin ClObjDataCacheManager%41E2B3F003B7.declarations preserve=yes
  //## end ClObjDataCacheManager%41E2B3F003B7.declarations

// Class ClObjDataCache 





ClObjDataCache::ClObjDataCache (ClCache* ai_poCache, ClObjDataCacheTree* ai_poCacheTree, m4pcchar_t ai_pccPath, m4uint32_t ai_uiMaxMemory, m4uint32_t ai_uiNumObjects)
  //## begin ClObjDataCache::ClObjDataCache%1105443264.hasinit preserve=no
      : m_poCacheTree(NULL)
  //## end ClObjDataCache::ClObjDataCache%1105443264.hasinit
  //## begin ClObjDataCache::ClObjDataCache%1105443264.initialization preserve=yes
  , ClObjDataCacheBase_t(ai_poCache, ai_pccPath, "DeltaCache" /*ai_pId*/, M4CL_CACHE_READ_WRITE /*ai_iMode*/, ai_uiMaxMemory, ai_uiNumObjects, 1 /*ai_iRefreshRatio*/, 1.0 /*ai_dMaxPeriod*/, 1.0 /*ai_dMaxUnusedPeriod*/)
  , m_oMutex(M4_TRUE)
  //## end ClObjDataCache::ClObjDataCache%1105443264.initialization
{
  //## begin ClObjDataCache::ClObjDataCache%1105443264.body preserve=yes
    static ClSerializedChannelDataFactory* poCOFactory = new ClSerializedChannelDataFactory();
    SetPersistor(poCOFactory);
    SetCOFactory(poCOFactory);

    // initialize the cache.
    m_poCacheTree = ai_poCacheTree;
    Init();
    RemoveAll();
  //## end ClObjDataCache::ClObjDataCache%1105443264.body
}


ClObjDataCache::~ClObjDataCache()
{
  //## begin ClObjDataCache::~ClObjDataCache%.body preserve=yes
    RemoveAll();
    PersistCache();
    if (NULL != m_poCache)
    {
        delete m_poCache;
        m_poCache = NULL;
    }
  //## end ClObjDataCache::~ClObjDataCache%.body
}



//## Other Operations (implementation)
m4uint16_t ClObjDataCache::GetType ()
{
  //## begin ClObjDataCache::GetType%1105443265.body preserve=yes
    return M4_CACHE_OBJDATA_CACHE_TYPE;
  //## end ClObjDataCache::GetType%1105443265.body
}

m4return_t ClObjDataCache::RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount)
{
  //## begin ClObjDataCache::RemoveObjectById%1105443266.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // remove all elements. All elements with references are moved to the
    // deletion queue.
    retCode = m_poCache->RemoveAll(ai_pcChannelId, ai_piRemovedCount);

    // clear the deletion queue.
    retCode = m_poCacheTree->ClearDeletionQueue();

    // trace.
    m_poCacheTree->Trace(ClObjDataCacheTree::M4_EVENT_DELETE, NULL, ai_pcChannelId, NULL);

    return retCode;
  //## end ClObjDataCache::RemoveObjectById%1105443266.body
}

m4return_t ClObjDataCache::RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dateVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
  //## begin ClObjDataCache::RemoveObjectByIdAndVersion%1105443267.body preserve=yes
    return M4_ERROR;
  //## end ClObjDataCache::RemoveObjectByIdAndVersion%1105443267.body
}

m4return_t ClObjDataCache::GetObject (m4pchar_t ai_pcObjectId, ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData)
{
  //## begin ClObjDataCache::GetObject%1105443268.body preserve=yes
    m4return_t retCode = M4_ERROR;
    m4uint16_t iVerCounter = 0;
    ClSerializedChannelData* poSCD = NULL;
    m4pchar_t pcSource = "RAM";
    
    // static values.
    static m4date_t dMaxPeriod = 0.0;
    static m4date_t dMaxUnusedPeriod = 0.0;
    static m4date_t dStartDate = ClMinusInfiniteDate();
    static m4date_t dEndDate = ClPlusInfiniteDate();
    static m4date_t dCorDate = ClMinusInfiniteDate();   
    static ClCacheInterface::eGetMode eGetMode = ClCacheInterface::GET_SHARED;

    // check input arguments.
    if (NULL == ai_pcObjectId)
    {
        // trace.
        m_poCacheTree->Trace(ClObjDataCacheTree::M4_EVENT_GET, "Failed", ai_pcObjectId, poSCD);
        return M4_ERROR;
    }

    // get object from cache.
    retCode = _GetObject (ai_pcObjectId, dStartDate, dEndDate, dCorDate, iVerCounter, poSCD, eGetMode);
    DUMP_OBJDATA_CACHE_ACTIONS("Get", ai_pcObjectId, poSCD);

    // trace.
    m_poCacheTree->Trace(ClObjDataCacheTree::M4_EVENT_GET, NULL, ai_pcObjectId, poSCD);

    // get channel data from the cache object.
    if (NULL != poSCD)
    {
        retCode = poSCD->GetChannelData(ai_poFactory, ai_poCMCR, ai_poLaneArgument, ao_rpoChannelData);
    }
    DUMP_OBJDATA_CACHE_ACTIONS("GetChannelData", ai_pcObjectId, ao_rpoChannelData);


    return retCode;
  //## end ClObjDataCache::GetObject%1105443268.body
}

m4return_t ClObjDataCache::PutObject (m4pchar_t ai_pcObjectId, ClChannel_Data* ai_poChannelData)
{
  //## begin ClObjDataCache::PutObject%1105443269.body preserve=yes
    m4return_t retCode = M4_ERROR;
    m4uint16_t iVerCounter = 0;
    ClSerializedChannelData* poSCD = NULL;

    // static values.
    static m4bool_t bSaveToDisk = M4_FALSE; 
    static m4date_t dMaxPeriod = 0.0;
    static m4date_t dMaxUnusedPeriod = 0.0;
    static m4date_t dStartDate = ClMinusInfiniteDate();
    static m4date_t dEndDate = ClPlusInfiniteDate();
    static m4date_t dCorDate = ClMinusInfiniteDate();   
    static ClCacheInterface::eGetMode eGetMode = ClCacheInterface::GET_SHARED;

    // check input arguments.
    if ((NULL == ai_pcObjectId) || (NULL == ai_poChannelData))
    {
        // trace.
        m_poCacheTree->Trace(ClObjDataCacheTree::M4_EVENT_PUT, "Failed", ai_pcObjectId, poSCD);
        return M4_ERROR;
    }

    // look for ai_poChannelData in the cache.
    retCode = _GetObject (ai_pcObjectId, dStartDate, dEndDate, dCorDate, iVerCounter, poSCD, eGetMode);

    // bugid: 0102599.
    // if was found, but it's NULL, the object is blocked, so two threads are accessing the same object.
    // Return error.
    if ((M4_SUCCESS == retCode) && (NULL == poSCD))
    {
        return M4_ERROR;
    }
    
    // if object was found, update it, otherwise insert the new object.
    if (NULL != poSCD)
    {
        retCode = m_poCacheTree->UpdateObject(poSCD, ai_poChannelData);
        DUMP_OBJDATA_CACHE_ACTIONS("UpdateObject", ai_pcObjectId, ai_poChannelData);
    }
    else
    {
        // the new serialized the channel data.
        poSCD = new ClSerializedChannelData();
        if (NULL == poSCD)
        {
            return M4_ERROR;
        }
        retCode = poSCD->PutChannelData(ai_poChannelData);
        DUMP_OBJDATA_CACHE_ACTIONS("PutChannelData", ai_pcObjectId, ai_poChannelData);
        if (M4_SUCCESS != retCode)
        {
            return retCode;
        }

        // insert in cache.
        retCode = _PutObject(ai_pcObjectId, iVerCounter, poSCD, dMaxPeriod, dMaxUnusedPeriod, bSaveToDisk);
    }

    // trace.
    DUMP_OBJDATA_CACHE_ACTIONS("Put", ai_pcObjectId, poSCD);
    m_poCacheTree->Trace(ClObjDataCacheTree::M4_EVENT_PUT, NULL, ai_pcObjectId, poSCD);

    // free all references and delete the object.
    if (NULL != poSCD)
    {
        DUMP_OBJDATA_CACHE_ACTIONS("FreeAllReferences", ai_pcObjectId, poSCD);
        FreeAllReferences(poSCD);
    }
        
    return retCode;
  //## end ClObjDataCache::PutObject%1105443269.body
}

m4return_t ClObjDataCache::FreeAllReferences (ClSerializedChannelData* ai_poSCD)
{
  //## begin ClObjDataCache::FreeAllReferences%1110965085.body preserve=yes
    if (NULL != ai_poSCD)
    {
        ClCacheableObject* poCacheableObject = ai_poSCD->GetCO();
        m4uint32_t iRef = poCacheableObject->GetNumOfRef();
        for (m4uint32_t i=0; i<iRef; i++)
        {
            DUMP_OBJDATA_CACHE_ACTIONS("Free", "-", ai_poSCD);
            FreeObject(ai_poSCD);
        }
    }

    return M4_SUCCESS;
  //## end ClObjDataCache::FreeAllReferences%1110965085.body
}

// Additional Declarations
  //## begin ClObjDataCache%41E3C0FE0370.declarations preserve=yes
  //## end ClObjDataCache%41E3C0FE0370.declarations

// Class ClObjDataCacheTree 



ClObjDataCacheTree::ClObjDataCacheTree (m4uint32_t ai_iMemory, m4uint32_t ai_iObjects)
  //## begin ClObjDataCacheTree::ClObjDataCacheTree%1110965078.hasinit preserve=no
      : m_poRollingFile(NULL)
  //## end ClObjDataCacheTree::ClObjDataCacheTree%1110965078.hasinit
  //## begin ClObjDataCacheTree::ClObjDataCacheTree%1110965078.initialization preserve=yes
    , ClCacheTree(0, 0, ai_iMemory, ai_iObjects, 50)
  //## end ClObjDataCacheTree::ClObjDataCacheTree%1110965078.initialization
{
  //## begin ClObjDataCacheTree::ClObjDataCacheTree%1110965078.body preserve=yes
  //## end ClObjDataCacheTree::ClObjDataCacheTree%1110965078.body
}


ClObjDataCacheTree::~ClObjDataCacheTree (void)
{
	if( m_poRollingFile != NULL )
	{
		delete( m_poRollingFile ) ;
		m_poRollingFile = NULL ;
	}
}


//## Other Operations (implementation)
m4return_t ClObjDataCacheTree::ClearDeletionQueue ()
{
  //## begin ClObjDataCacheTree::ClearDeletionQueue%1110965079.body preserve=yes
    ClMutBlock oBlock(&m_oMutex);

    ObjectDeletionQueue.clear();
    m_ulNumOfObjInDelQueue = ObjectDeletionQueue.size();

    return M4_SUCCESS;
  //## end ClObjDataCacheTree::ClearDeletionQueue%1110965079.body
}


m4return_t ClObjDataCacheTree::Trace (M4CacheEvent_t ai_event, void* ai_poInfo, m4pcchar_t ai_pcObjectId, ClSerializedChannelData* ai_poObject)
{
  //## begin ClObjDataCacheTree::Trace%1110965081.body preserve=yes
    m4char_t msg[1024];
    m4pcchar_t pcObjectId = "Null-Object-Id";
    m4pchar_t pcEventName = NULL;
    m4int32_t iSize = -1;
    static m4bool_t bWasInit = M4_FALSE;
    m4pcchar_t pcInfo = NULL;

    static m4pcchar_t pcEventNames[] = {
        "GET", "get", "PUT", "DELETE", "DUMP_BEFORE", "DUMP_AFTER"
    };


	/* Bug 0108638
	Se consulta el nivel de traza dinámicamente
	*/
    if( ClVMDebugEnv::getInstance()->getDebugLevel() < 2 )
    {
		return( M4_SUCCESS );
	}

    if( m_poRollingFile == NULL )
    {
        m_poRollingFile = new ClRollingFile();

		if( m_poRollingFile == NULL )
		{
			return( M4_ERROR );
		}

        m4string_t sFileName = ClVMDebugEnv::getInstance()->getDebugDir();
        sFileName += M4_BACK_SLASH;
        sFileName += "deltacache.log";

        m_poRollingFile->EnableTickCount();
        m_poRollingFile->EnableThreadId();
        m_poRollingFile->SetFile( sFileName );
	}


    if (M4_FALSE == bWasInit)
    {
        sprintf(msg, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", 
             "ThreadId", "TickCount", "Event", "ObjectId", "Info", "ObjectSize", "ObjectsInCache", "ObjectsInRAM", "ObjectsInDisk", "CacheSize");
        m_poRollingFile->SetHeader(msg);
        bWasInit = M4_TRUE;
    }

    // set additional info.
    pcInfo = (m4pcchar_t)ai_poInfo;
    if (NULL == pcInfo)
    {
        pcInfo = "-";
    }
    
    // set message values.
    if (NULL != ai_pcObjectId)
    {
        pcObjectId = ai_pcObjectId;
    }
    if (NULL != ai_poObject)
    {
        iSize = ai_poObject->GetSize();
    }

    // number of objects.
    m4uint32_t uiInCache = m_ulNumOfObjInCache;
    m4uint32_t uiInDisk = m_ulNumOfObjInDisk;

    // print.
    sprintf(msg, "%s\t%s\t%s\t%d\t%u\t%u\t%u\t%u", 
        pcEventNames[ai_event], pcObjectId, pcInfo, iSize, uiInCache, uiInCache-uiInDisk, uiInDisk, m_ulSizeCache);
    m_poRollingFile->PrintLn(msg);
    
    return M4_SUCCESS;
  //## end ClObjDataCacheTree::Trace%1110965081.body
}

m4return_t ClObjDataCacheTree::NumOfObjectsControl ()
{
  //## begin ClObjDataCacheTree::NumOfObjectsControl%1110965083.body preserve=yes
    // check the number of objects in RAM.
    m4uint32_t ulNumOfObjInRAM = m_ulNumOfObjInCache - m_ulNumOfObjInDisk;   
    m4uint32_t ulMinObjInRAM = m_ulMaxNOfObjects - m_ulDelRatioObjs;
    if(ulNumOfObjInRAM < m_ulMaxNOfObjects)
    {
        return M4_SUCCESS;
    }

    // trace.
    Trace(ClObjDataCacheTree::M4_EVENT_DUMP_BEFORE, "NumObjects", "-", NULL);

    // Any object in the LRU map is moved to disk.
    ClCacheableObject* poObject = NULL;
    CacheableObject2LRUMap::iterator itLRUMap;
    for(itLRUMap=LRUDateMap.begin(); itLRUMap!=LRUDateMap.end(); itLRUMap++)
    {
        poObject=(*itLRUMap).second;
        
        // Move to disk only is it's not referenced and in RAM
        if((poObject->IsReferenced()==M4_FALSE) && (poObject->Getm_bRam()==M4_TRUE))
        {
            if (poObject->Getm_bDisk() == M4_FALSE)
            {
                m_ulNumOfObjInDisk++;
            }
                
            // Send to disk
            poObject->SaveValueToDisk();            
            if (poObject->Getm_bDisk() == M4_TRUE) 
            {
                // delete from RAM
                if((poObject->DeleteValueFromRam())==M4_ERROR) 
                {
                    return M4_ERROR;
                }
                
                // update cache size.
                m_ulSizeCache -= poObject->GetSize();
                
                // an object less in RAM
                ulNumOfObjInRAM--;
            } 
            else 
            {
                // error saving to disk
                m_ulNumOfObjInDisk--;
            }
        }
        
        // Checks the minimun objects in RAM (20% of max size)
        if((ulNumOfObjInRAM < ulMinObjInRAM) || (0 == LRUDateMap.size()))
        {
            break;
        }
    }
    
    // trace.
    Trace(ClObjDataCacheTree::M4_EVENT_DUMP_AFTER, "NumObjects", "-", NULL);

    return M4_SUCCESS;
  //## end ClObjDataCacheTree::NumOfObjectsControl%1110965083.body
}

m4return_t ClObjDataCacheTree::SizeControl ()
{
  //## begin ClObjDataCacheTree::SizeControl%1110965082.body preserve=yes
    // check the cache size in memory.
    if(m_ulSizeCache < m_ulMaxSizeCache)
    {
        return M4_SUCCESS;
    }

    // trace.
    Trace(ClObjDataCacheTree::M4_EVENT_DUMP_BEFORE, "Memory", "-", NULL);

    // Any object in the LRU map is moved to disk.
    ClCacheableObject* poObject = NULL;
    CacheableObject2LRUMap::iterator itLRUMap;
    for(itLRUMap=LRUDateMap.begin(); itLRUMap!=LRUDateMap.end(); itLRUMap++)
    {
        poObject=(*itLRUMap).second;
        
        // Move to disk only is it's not referenced and in RAM
        if((poObject->IsReferenced()==M4_FALSE) && (poObject->Getm_bRam()==M4_TRUE))
        {
            if (poObject->Getm_bDisk() == M4_FALSE)
            {
                m_ulNumOfObjInDisk++;
            }
                
            // Send to disk
            poObject->SaveValueToDisk();            
            if (poObject->Getm_bDisk() == M4_TRUE) 
            {
                // delete from RAM
                if((poObject->DeleteValueFromRam())==M4_ERROR) 
                {
                    return M4_ERROR;
                }
                
                // update cache size.
                m_ulSizeCache -= poObject->GetSize();
            } 
            else 
            {
                // error saving to disk
                m_ulNumOfObjInDisk--;
            }
        }
        
        // Checks the size in RAM (20% of max size)
        if((m_ulSizeCache < m_ulDelRatioMem) || (0 == LRUDateMap.size()))
        {
            break;
        }
    }
    
    // trace.
    Trace(ClObjDataCacheTree::M4_EVENT_DUMP_AFTER, "Memory", "-", NULL);

    return M4_SUCCESS;
  //## end ClObjDataCacheTree::SizeControl%1110965082.body
}

m4return_t ClObjDataCacheTree::Refresh (ClSelfPersistiableObject *   ai_poSelfPersistor)
{
  //## begin ClObjDataCacheTree::Refresh%1110965084.body preserve=yes
    // disable refresh operation.
    m4return_t retCode = M4_SUCCESS;
    return retCode;
  //## end ClObjDataCacheTree::Refresh%1110965084.body
}

m4return_t ClObjDataCacheTree::GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode, m4int32_t  ai_iTimeOut)
{
  //## begin ClObjDataCacheTree::GetObject%1112259308.body preserve=yes
    m4return_t retCode = M4_ERROR;
    m4pcchar_t pcSource = "RAM";
    
    // calls parent class.
    retCode = ClCacheTree::GetObject(ai_pKey, ao_pObject, ai_eGetMode, ai_iTimeOut);

    // if object was in this, remove it.
    if (M4_SUCCESS == retCode) 
    {
        if ((NULL != ao_pObject) && (M4_TRUE == ao_pObject->Getm_bDisk()))
        {
            ao_pObject->DeleteValueFromDisk();
            m_ulNumOfObjInDisk--;
            pcSource = "DISK";
        }        
    }
    else
    {
        pcSource = "Failed";
    }

    // trace.
    Trace(ClObjDataCacheTree::M4_EVENT_IGET, (void*)pcSource, "-", NULL);

    return retCode;
  //## end ClObjDataCacheTree::GetObject%1112259308.body
}

m4return_t ClObjDataCacheTree::UpdateObject (ClSerializedChannelData* ai_poSCD, ClChannel_Data* ai_poChannelData)
{
  //## begin ClObjDataCacheTree::UpdateObject%1112259309.body preserve=yes
    m4return_t retCode = M4_ERROR;
    m4uint32_t uiCurrentSize = 0;
    m4uint32_t uiNewSize = 0;
    m4int32_t uiIncrSize = 0;
    ClCacheableObject* poCO = NULL;
    M4ClTimeStamp* poLRUDate = NULL;
    
    // get the current object size.
    uiCurrentSize = ai_poSCD->GetSize();
    
    // update channel data.
    retCode = ai_poSCD->PutChannelData(ai_poChannelData);

    // update cache size and info in cacheable object.
    if (M4_ERROR != retCode)
    {
        uiNewSize = ai_poSCD->GetSize();
        poCO = ai_poSCD->GetCO();
        if (NULL != poCO)
        {
            // update size.
            poCO->SetSize(uiNewSize);
            
            // update lru date.
            poLRUDate = poCO->GetLRUDate();
            poLRUDate->now();
            poCO->SetLRUDate(poLRUDate);
        }
    }
    m_ulSizeCache = m_ulSizeCache - uiCurrentSize + uiNewSize;
    
	// CacheSize control
    if(m_ulMaxSizeCache)
    {
        // bugid 0103418. Block before SizeControl.
	    ClMutBlock oBlock(m_oMutex);
        SizeControl();
	}

    return retCode;

  //## end ClObjDataCacheTree::UpdateObject%1112259309.body
}

// Additional Declarations
  //## begin ClObjDataCacheTree%4237FE720240.declarations preserve=yes
  //## end ClObjDataCacheTree%4237FE720240.declarations

//## begin module%41E2B44D0359.epilog preserve=yes
//## end module%41E2B44D0359.epilog
