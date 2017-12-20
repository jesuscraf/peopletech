//## begin module%41F4D7450080.cm preserve=no
//## end module%41F4D7450080.cm

//## begin module%41F4D7450080.cp preserve=no
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
//## end module%41F4D7450080.cp

//## Module: serializedchanneldata%41F4D7450080; Package body
//## Subsystem: m4dm::cache-auto::src%41E2B42502DC
//## Source file: Z:\m4dm\src\serializedchanneldata.cpp

//## begin module%41F4D7450080.additionalIncludes preserve=no
//## end module%41F4D7450080.additionalIncludes

//## begin module%41F4D7450080.includes preserve=yes
#include "cldatstoiod.hpp"      // ClDataStorageIODriver
#include "chandata_ser.hpp"     // ClChannelDataSerializer
#include "m4filemanager.hpp"    // ClFileManager
#include "chan_knl.hpp"         // ClChannel_Data
//## end module%41F4D7450080.includes

// serializedchanneldata
#include <serializedchanneldata.hpp>
// clcofac
#include <clcofac.hpp>
// diskpers
#include <diskpers.hpp>
// clprsstb
#include <clprsstb.hpp>
#include <m4cryptc.hpp>
//## begin module%41F4D7450080.declarations preserve=no
//## end module%41F4D7450080.declarations

//## begin module%41F4D7450080.additionalDeclarations preserve=yes
//## end module%41F4D7450080.additionalDeclarations



// Class ClSerializedChannelData 



ClSerializedChannelData::ClSerializedChannelData()
  //## begin ClSerializedChannelData::ClSerializedChannelData%.hasinit preserve=no
      : m_pCO(NULL), m_poDataStorage(NULL)
  //## end ClSerializedChannelData::ClSerializedChannelData%.hasinit
  //## begin ClSerializedChannelData::ClSerializedChannelData%.initialization preserve=yes
    , ClPersistiableObject(NULL)
  //## end ClSerializedChannelData::ClSerializedChannelData%.initialization
{
  //## begin ClSerializedChannelData::ClSerializedChannelData%.body preserve=yes
  //## end ClSerializedChannelData::ClSerializedChannelData%.body
}


ClSerializedChannelData::~ClSerializedChannelData()
{
  //## begin ClSerializedChannelData::~ClSerializedChannelData%.body preserve=yes
    // frees the dataStorage.
    if (NULL != m_poDataStorage)
    {
        M4DataStorage::DeleteDataStorage(m_poDataStorage);
        m_poDataStorage = NULL;
    }

  //## end ClSerializedChannelData::~ClSerializedChannelData%.body
}



//## Other Operations (implementation)
m4date_t  ClSerializedChannelData::GetVersion () const
{
  //## begin ClSerializedChannelData::GetVersion%1106565729.body preserve=yes
    return ClMinusInfiniteDate();
  //## end ClSerializedChannelData::GetVersion%1106565729.body
}

void  ClSerializedChannelData::SetCO (ClCacheableObject* ai_pCO)
{
  //## begin ClSerializedChannelData::SetCO%1106565730.body preserve=yes
    m_pCO = ai_pCO;
  //## end ClSerializedChannelData::SetCO%1106565730.body
}

ClCacheableObject* ClSerializedChannelData::GetCO () const
{
  //## begin ClSerializedChannelData::GetCO%1106565731.body preserve=yes
    return m_pCO;
  //## end ClSerializedChannelData::GetCO%1106565731.body
}

m4return_t ClSerializedChannelData::GetChannelData (ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData)
{
  //## begin ClSerializedChannelData::GetChannelData%1106565732.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // initialize the channel_data.
    ao_rpoChannelData = NULL;
    if (NULL == m_poDataStorage)
    {
        return M4_ERROR;
    }
    ao_rpoChannelData = new ClChannel_Data();
    if (NULL == ao_rpoChannelData)
    {
        return M4_ERROR;
    }

	/* Bug 0106102
    resets serialization. m4pdus requirement
	*/
    m_poDataStorage->m_bFinishSerialization = M4_TRUE;
    m_poDataStorage->InitializeIteration();

    // create the IODriver.
	ClDataStoreIODriver dsIOD(GEN_IO_MODE_READ, m_poDataStorage);

    ClChannelDataSerializer channelDataSerializer;
    retCode = channelDataSerializer.DeSerialize(dsIOD, ai_poFactory, ai_poCMCR, ai_poLaneArgument, ao_rpoChannelData);

    return retCode;  
  //## end ClSerializedChannelData::GetChannelData%1106565732.body
}

m4date_t ClSerializedChannelData::GetStartDate ()
{
  //## begin ClSerializedChannelData::GetStartDate%1106565733.body preserve=yes
    return ClMinusInfiniteDate();
  //## end ClSerializedChannelData::GetStartDate%1106565733.body
}

m4date_t ClSerializedChannelData::GetEndDate ()
{
  //## begin ClSerializedChannelData::GetEndDate%1106565734.body preserve=yes
    return ClPlusInfiniteDate();
  //## end ClSerializedChannelData::GetEndDate%1106565734.body
}

m4date_t ClSerializedChannelData::GetCorStartDate ()
{
  //## begin ClSerializedChannelData::GetCorStartDate%1106565735.body preserve=yes
    return ClMinusInfiniteDate();
  //## end ClSerializedChannelData::GetCorStartDate%1106565735.body
}

m4date_t ClSerializedChannelData::GetCorEndDate ()
{
  //## begin ClSerializedChannelData::GetCorEndDate%1106565736.body preserve=yes
    return ClPlusInfiniteDate();
  //## end ClSerializedChannelData::GetCorEndDate%1106565736.body
}

m4uint32_t ClSerializedChannelData::GetSize ()
{
  //## begin ClSerializedChannelData::GetSize%1106565737.body preserve=yes
    m4uint32_t iObjectSize = 0;
    
    if (NULL != m_poDataStorage)
    {
        iObjectSize = m_poDataStorage->Size();
    }

    return iObjectSize;
  //## end ClSerializedChannelData::GetSize%1106565737.body
}

m4return_t ClSerializedChannelData::PutChannelData (ClChannel_Data* ai_poChannelData)
{
  //## begin ClSerializedChannelData::PutChannelData%1106565741.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // allocates the datastorage.
    if (NULL != m_poDataStorage)
    {
        M4DataStorage::DeleteDataStorage(m_poDataStorage);
    }
    m_poDataStorage = M4DataStorage::GetNewDataStorage();
    if (NULL == m_poDataStorage)
    {
        return M4_ERROR;
    }

	// Creates the IODriver.
	ClDataStoreIODriver dsIOD(GEN_IO_MODE_WRITE, m_poDataStorage);

    ClChannelDataSerializer channelDataSerializer;
    retCode = channelDataSerializer.Serialize(dsIOD, ai_poChannelData, M4_FALSE, M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_MAPPED);

    return retCode;
  //## end ClSerializedChannelData::PutChannelData%1106565741.body
}


#define	M4CH_DUMMY_D		"\xA\x9\x7\xA\x1\x9\xA\x2\xC\xB\x3\x3\xD\xF\x2\x1\x1\x1\x8\x4\x7\x4\x5\xD\xB\x2\xD\x8\x7\x3\x3\x5\x9\x1\xE\x4\xB\xA\xF\xC\xA\xD\x4\x5\xD\x8\x2\xB\x7\xA\xA\x7\xF\x1\xF\xD\x7\xF\x6\x1\xD\xE\x2\x0\x9\x1\x4\xA\x7\xC\x1\x5\x4\xE\xB\x2\x7\xA\xA\x5\xB\x9\x7\xA\x5\x5\x4\x6"


m4return_t ClSerializedChannelData::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClSerializedChannelData::Serialize%1106652454.body preserve=yes
    m4return_t retCode = M4_ERROR;
    
    // resets serialization. m4pdus requirement
    m_poDataStorage->m_bFinishSerialization = M4_TRUE;
    m_poDataStorage->InitializeIteration();
    
    // Serializes the N.blocks
    m4uint32_t uiNumBlocks = m_poDataStorage->Blocks();
    retCode = ai_roIOD.Write(uiNumBlocks);
    if (M4_SUCCESS != retCode)
    {
        return retCode;
    }
    
    // Serializes the {blocks}*
    ClDataUnitInterface *poDataUnit = NULL;
    m4uint32_t size = 0;
    m4int_t iEncryptedSize = 0;
    m4pchar_t pBuffer = NULL;
    m4pchar_t pcEncryptedBuffer = NULL;
    m4int16_t iType = 0;
    while (M4_SUCCESS == m_poDataStorage->GetNext(poDataUnit)) 
    {
        // block type.
        iType = poDataUnit->GetType();
        retCode = ai_roIOD.Write( iType );
        if (M4_SUCCESS != retCode)
        {
            return retCode;
        }
        
        // block.
        if (M4_FILE_UNIT == iType) 
        {
            // Buffer con fichero asociado
            pBuffer = ((ClFileUnit *)poDataUnit)->GetFileName();
            if (pBuffer) 
            {
                size = strlen(pBuffer) + 1;
                ClFileManager* oFileManager = ClFileManager::Instance();
                ClFileName oFile(pBuffer);
                oFileManager->SetInterestInFile(oFile);
            } 
            else 
            {
                size = 0;
            }
        } 
        else 
        {
            // M4_DATA_UNIT
            size = poDataUnit->GetSize();
            pBuffer = poDataUnit->GetBuffer();
        }

		/* Bug 0102060
		Los datos de los auto que van a fichero deben ir encriptados
		*/
		retCode = EncryptTwoWayRaw( pBuffer, size, M4CH_DUMMY_D + 23, strlen( M4CH_DUMMY_D + 23 ) * sizeof( m4char_t ), pcEncryptedBuffer, iEncryptedSize ) ;

		if( retCode != M4_SUCCESS )
		{
            DUMP_CHLOG_ERROR( M4_CH_DM_NO_ENCRYPT ) ;
			return( M4_ERROR ) ;
		}
        
        retCode = ai_roIOD.Write(pcEncryptedBuffer, iEncryptedSize);
		delete [] pcEncryptedBuffer ;

        if (M4_SUCCESS != retCode)
        {
            return retCode;
        }
    }
    
    return M4_SUCCESS;
  //## end ClSerializedChannelData::Serialize%1106652454.body
}

m4return_t ClSerializedChannelData::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClSerializedChannelData::DeSerialize%1106652455.body preserve=yes
    m4return_t retCode = M4_ERROR;
    
    // allocates the datastorage.
    if (NULL != m_poDataStorage)
    {
        M4DataStorage::DeleteDataStorage(m_poDataStorage);
    }
    m_poDataStorage = M4DataStorage::GetNewDataStorage();
    if (NULL == m_poDataStorage)
    {
        return M4_ERROR;
    }
    
    // reads the N.blocks
    m4uint32_t uiNumBlocks = 0;
    retCode = ai_roIOD.Read(uiNumBlocks);
    if (M4_SUCCESS != retCode)
    {
        return retCode;
    }
    
    // reads the blocks
    m4pchar_t poBuffer = NULL;
    m4pchar_t pcDecryptedBuffer = NULL;
    m4uint32_t uiSizeDS = 0;
    m4int_t iDecryptedSize = 0;
    m4int16_t iType = 0;
    for (m4uint32_t i=0; i<uiNumBlocks; i++) 
    {
        // block type.
        retCode = ai_roIOD.Read(iType);
        if (M4_SUCCESS != retCode)
        {
            return retCode;
        }
        
        // block size
        retCode = ai_roIOD.ReadSize(uiSizeDS);
        if (M4_SUCCESS != retCode)
        {
            return retCode;
        }
        
        // block: filename or data
        poBuffer = new m4char_t[uiSizeDS];
        if (!poBuffer) 
        {
            return M4_ERROR;
        }
        retCode = ai_roIOD.ReadBuffer(poBuffer);
        if (M4_SUCCESS != retCode)
        {
            delete[] poBuffer;
            return retCode;
        }
        
		/* Bug 0102060
		Los datos de los auto que vienen de fichero están encriptados
		*/
		retCode = DecryptTwoWayRaw( poBuffer, uiSizeDS, M4CH_DUMMY_D + 23, strlen( M4CH_DUMMY_D + 23 ) * sizeof( m4char_t ), pcDecryptedBuffer, iDecryptedSize ) ;
        delete [] poBuffer ;

		if( retCode != M4_SUCCESS )
		{
            DUMP_CHLOG_ERROR( M4_CH_DM_NO_DECRYPT ) ;
			return( M4_ERROR ) ;
		}

        if (M4_FILE_UNIT == iType) 
        {
            // adds a new fileunit. Internally adds a new reference in the M4FileManager.
            retCode = m_poDataStorage->AddFileUnit(pcDecryptedBuffer);

            // sustract my reference in the M4FileManager.
            ClFileManager* oFileManager = ClFileManager::Instance();
            ClFileName oFile(pcDecryptedBuffer);
            oFileManager->UnSetInterestInFile(oFile);
            if (retCode == M4_SUCCESS) 
            {
                // free the filename.
                delete[] pcDecryptedBuffer;
            }
            
        } 
        else 
        {
            // adds the buffer to the datastorage.
            retCode = m_poDataStorage->Add(pcDecryptedBuffer, iDecryptedSize);
        }
        if (M4_SUCCESS != retCode)
        {
            delete[] pcDecryptedBuffer;
            return retCode;
        }
    }
       
    return M4_SUCCESS;
  //## end ClSerializedChannelData::DeSerialize%1106652455.body
}

// Additional Declarations
  //## begin ClSerializedChannelData%41F4D5220268.declarations preserve=yes
  //## end ClSerializedChannelData%41F4D5220268.declarations

// Class ClSerializedChannelDataContainer 

ClSerializedChannelDataContainer::ClSerializedChannelDataContainer (ClCachePolicy* ai_poPolicy, m4uint32_t ai_iSizeObject, ClCacheDatedKey* ai_poClCacheKey)
  //## begin ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.hasinit preserve=no
  //## end ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.hasinit
  //## begin ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.initialization preserve=yes
  : ClSerializedChannelDataContainerBase_t(ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
  //## end ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.initialization
{
  //## begin ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.body preserve=yes
  //## end ClSerializedChannelDataContainer::ClSerializedChannelDataContainer%1106565740.body
}



//## Other Operations (implementation)
ClObjectId* ClSerializedChannelDataContainer::CreateObjectIdFromKey ()
{
  //## begin ClSerializedChannelDataContainer::CreateObjectIdFromKey%1106565739.body preserve=yes
    m4pchar_t pcId = NULL;

    pcId = m_poClCacheKey->Getm_pszKey();
    ClTypedId oTypeID(pcId);
    ClUniqueKey oUniqueKey("");
    ClEnvironmentId oEnvId( GetEnvId() );
    ClObjectId * poObjID = new ClObjectId (ObjDataObjectType, &oEnvId, &oTypeID, &oUniqueKey);

    return poObjID;
  //## end ClSerializedChannelDataContainer::CreateObjectIdFromKey%1106565739.body
}

m4uint8_t ClSerializedChannelDataContainer::GetType ()
{
  //## begin ClSerializedChannelDataContainer::GetType%1106737889.body preserve=yes
    return M4_CO_FACTORY_OBJDATA_CONTAINER;
  //## end ClSerializedChannelDataContainer::GetType%1106737889.body
}

// Additional Declarations
  //## begin ClSerializedChannelDataContainer%41F4D5EB019B.declarations preserve=yes
  //## end ClSerializedChannelDataContainer%41F4D5EB019B.declarations

// Class ClSerializedChannelDataFactory 

ClSerializedChannelDataFactory::ClSerializedChannelDataFactory()
  //## begin ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.hasinit preserve=no
  //## end ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.hasinit
  //## begin ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.initialization preserve=yes
  //## end ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.initialization
{
  //## begin ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.body preserve=yes
  //## end ClSerializedChannelDataFactory::ClSerializedChannelDataFactory%.body
}


ClSerializedChannelDataFactory::~ClSerializedChannelDataFactory()
{
  //## begin ClSerializedChannelDataFactory::~ClSerializedChannelDataFactory%.body preserve=yes
  //## end ClSerializedChannelDataFactory::~ClSerializedChannelDataFactory%.body
}



//## Other Operations (implementation)
m4return_t ClSerializedChannelDataFactory::CreateObject (m4uint8_t ai_iType, ClCacheableObject*& aio_pObject)
{
  //## begin ClSerializedChannelDataFactory::CreateObject%1106737890.body preserve=yes
    aio_pObject = new ClSerializedChannelDataContainer();
    ((ClSerializedChannelDataContainer*)aio_pObject)->SetPersistor(this);
    return M4_SUCCESS;
  //## end ClSerializedChannelDataFactory::CreateObject%1106737890.body
}

// Additional Declarations
  //## begin ClSerializedChannelDataFactory%41F4FD8B016A.declarations preserve=yes
  //## end ClSerializedChannelDataFactory%41F4FD8B016A.declarations

//## begin module%41F4D7450080.epilog preserve=yes
//## end module%41F4D7450080.epilog
