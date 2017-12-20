//## begin module%344E313502C1.cm preserve=no
//## end module%344E313502C1.cm

//## begin module%344E313502C1.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%344E313502C1.cp

//## Module: DataStorages%344E313502C1; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\datastorages.cpp

//## begin module%344E313502C1.additionalIncludes preserve=no
//## end module%344E313502C1.additionalIncludes

//## begin module%344E313502C1.includes preserve=yes
//## end module%344E313502C1.includes

// m4types
#include <m4types.hpp>
#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
// DataStorages
#include <datastorages.hpp>
#include "m4unicode.hpp"

class ClBlockFormat;

//## begin module%344E313502C1.declarations preserve=no
//## end module%344E313502C1.declarations

//## begin module%344E313502C1.additionalDeclarations preserve=yes

#define M4_PDU_FRAGMENTATION_SIZE  1024*8
#define M4_DATASTORAGE_DEFAULT_DATAUNIT_NUMBER 128

static m4pchar_t s_DefaultTemp = NULL;


#define M4_PDU_FILEUNIT_ID_STRING "@@Attached File."

#define M4_PDU_FILEUNIT_SIZE_STRING "@@Size."

#define M4_PDU_CHECKUNIT_ID_STRING "@@Check Unit."

#include <m4trace.hpp>

// No funciona el bodyreference de Rose
#include <file_manager.hpp>
//## end module%344E313502C1.additionalDeclarations


// Class ClDataUnit 

ClDataUnit::ClDataUnit ()
  //## begin ClDataUnit::ClDataUnit%885978338.hasinit preserve=no
  //## end ClDataUnit::ClDataUnit%885978338.hasinit
  //## begin ClDataUnit::ClDataUnit%885978338.initialization preserve=yes
  //## end ClDataUnit::ClDataUnit%885978338.initialization
{
  //## begin ClDataUnit::ClDataUnit%885978338.body preserve=yes
  //## end ClDataUnit::ClDataUnit%885978338.body
}

ClDataUnit::ClDataUnit (m4pchar_t ai_pBuffer, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypt, m4uint32_t ai_ulOffset)
  //## begin ClDataUnit::ClDataUnit%877685667.hasinit preserve=no
  //## end ClDataUnit::ClDataUnit%877685667.hasinit
  //## begin ClDataUnit::ClDataUnit%877685667.initialization preserve=yes
  //## end ClDataUnit::ClDataUnit%877685667.initialization
{
  //## begin ClDataUnit::ClDataUnit%877685667.body preserve=yes
	m_pBuffer = ai_pBuffer;
	m_ulSize = ai_ulSize;
	m_usEncrypt = ai_usEncrypt;
//	m_ulOffset = ai_ulOffset;
  //## end ClDataUnit::ClDataUnit%877685667.body
}


ClDataUnit::~ClDataUnit ()
{
  //## begin ClDataUnit::~ClDataUnit%878295962.body preserve=yes
// Se borra el contenido en el DataUnitInterface
  //## end ClDataUnit::~ClDataUnit%878295962.body
}



//## Other Operations (implementation)
m4uint32_t ClDataUnit::GetSize ()
{
  //## begin ClDataUnit::GetSize%877685665.body preserve=yes
	return m_ulSize;
  //## end ClDataUnit::GetSize%877685665.body
}

m4pchar_t ClDataUnit::GetBuffer ()
{
  //## begin ClDataUnit::GetBuffer%877685666.body preserve=yes
	return m_pBuffer;
  //## end ClDataUnit::GetBuffer%877685666.body
}

m4bool_t ClDataUnit::GetCryptState ()
{
  //## begin ClDataUnit::GetCryptState%878377079.body preserve=yes
	return m_usEncrypt;
  //## end ClDataUnit::GetCryptState%878377079.body
}

void ClDataUnit::Dump (m4pchar_t  &ao_strBuffer)
{
  //## begin ClDataUnit::Dump%878377101.body preserve=yes

	m4char_t auxcad[32];

  	strcat (ao_strBuffer,"\n + DataUnit.");
	strcat (ao_strBuffer,"\n     - Size. ");
	sprintf(auxcad,"%lu",GetSize());
	strcat (ao_strBuffer,auxcad);
//	strcat (ao_strBuffer,"\n     - Data. ");
//	strcat (ao_strBuffer,GetBuffer());
	strcat (ao_strBuffer,"\n     - Encrypted. ");
	switch ( m_usEncrypt )
	{
	case M4_ENCRYPTED:
		strcat (ao_strBuffer,"yes");
		break;
	case M4_DECRYPTED:
		strcat (ao_strBuffer,"no");
		break;
	}

  //## end ClDataUnit::Dump%878377101.body
}

m4return_t ClDataUnit::SetSize (m4uint32_t ai_uiSize)
{
  //## begin ClDataUnit::SetSize%885978335.body preserve=yes
	return SetBufferSize(ai_uiSize);
  //## end ClDataUnit::SetSize%885978335.body
}

m4int16_t ClDataUnit::GetType ()
{
  //## begin ClDataUnit::GetType%903536123.body preserve=yes
	return M4_DATA_UNIT;
  //## end ClDataUnit::GetType%903536123.body
}

m4return_t ClDataUnit::InitializeSerialState ()
{
  //## begin ClDataUnit::InitializeSerialState%940592258.body preserve=yes
	return M4_SUCCESS;
  //## end ClDataUnit::InitializeSerialState%940592258.body
}

// Class ClBaseStorage 







ClBaseStorage::ClBaseStorage ()
  //## begin ClBaseStorage::ClBaseStorage%877685670.hasinit preserve=no
      : m_ulOffsetOut(0), m_ulRelativeSize(0), m_pTempPath(NULL), m_DefragSize(M4_PDU_FRAGMENTATION_SIZE), m_poDataUnitQueue(NULL)
  //## end ClBaseStorage::ClBaseStorage%877685670.hasinit
  //## begin ClBaseStorage::ClBaseStorage%877685670.initialization preserve=yes
  //## end ClBaseStorage::ClBaseStorage%877685670.initialization
{
  //## begin ClBaseStorage::ClBaseStorage%877685670.body preserve=yes
    m_bHasFiles = M4_FALSE;
  //## end ClBaseStorage::ClBaseStorage%877685670.body
}


ClBaseStorage::~ClBaseStorage ()
{
  //## begin ClBaseStorage::~ClBaseStorage%878295961.body preserve=yes

	if (m_pTempPath) {
		free(m_pTempPath);
		m_pTempPath = NULL;
	}
	
	if ( m_bOwner == M4_TRUE ) {
		delete m_poDataUnitQueue;
		m_poDataUnitQueue= NULL;
	 } else 
		m_poDataUnitQueue= NULL;

  //## end ClBaseStorage::~ClBaseStorage%878295961.body
}



//## Other Operations (implementation)
m4return_t ClBaseStorage::Add (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypted)
{
  //## begin ClBaseStorage::Add%877539739.body preserve=yes
	size_t					iLen; 
	ClDataUnitInterface*	pUnit;

	iLen = strlen( M4_PDU_CHECKUNIT_ID_STRING );
	if( ai_ulSize >= iLen && !memcmp( ai_pBlock, M4_PDU_CHECKUNIT_ID_STRING, iLen ))
	{
		pUnit = new ClCheckUnit();
	}
	else
	{
		pUnit = new ClDataUnit( ai_pBlock, ai_ulSize, ai_usEncrypted );
	}

	m_poDataUnitQueue->push_back( pUnit );
	return M4_SUCCESS;
  //## end ClBaseStorage::Add%877539739.body
}

m4return_t ClBaseStorage::Add (ClDataUnitInterface *ai_pDataUnit)
{
  //## begin ClBaseStorage::Add%885288225.body preserve=yes

//	m_ulRelativeSize += ai_pDataUnit -> GetSize();     // Este GetSize si se acaba de crear la DataUnit será 0

    if (NULL == ai_pDataUnit)
    {
        return M4_ERROR;
    }

    if (M4_FILE_UNIT == ai_pDataUnit->GetType())
    {
        m_bHasFiles = M4_TRUE;
    }

	m_poDataUnitQueue->push_back( ai_pDataUnit );
	return M4_SUCCESS;
  //## end ClBaseStorage::Add%885288225.body
}

m4return_t ClBaseStorage::Get (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted, m4uint32_t ai_ulBlockOrder)
{
  //## begin ClBaseStorage::Get%877602376.body preserve=yes

// bloque por el que vamos.
	m4uint32_t ulBlock = 0;
// puntero a la data unit de la que extraeremos la información.
	ClDataUnitInterface	*pDataUnit = NULL;

	ClDataUnitQueue::iterator iter;


	iter = ai_ulBlockOrder;
	if ( m_poDataUnitQueue->get( iter, pDataUnit ) != M4_ERROR )
	{
		if ((pDataUnit -> GetType ()) == M4_DATA_UNIT || (pDataUnit -> GetType ()) == M4_CHECK_UNIT ) 
			ao_pBlock = pDataUnit -> GetBuffer();
		else 
			ao_pBlock = ((ClFileUnit *)pDataUnit) -> GetFileName();
		
		ao_ulSize = pDataUnit -> GetSize();
		ao_bEncrypted = pDataUnit -> GetCryptState();
		return M4_SUCCESS;
		
	}

// si hemos salido del bucle, es porque no hemos encontrado
//lo que buscabamos.
	ao_pBlock = NULL;
	ao_ulSize = 0;
	ao_bEncrypted = M4_FALSE;
	return M4_ERROR;
  //## end ClBaseStorage::Get%877602376.body
}

m4return_t ClBaseStorage::Get (ClDataUnitInterface *ai_pDataUnit)
{
  //## begin ClBaseStorage::Get%903715593.body preserve=yes

	m_poDataUnitQueue->end ( ai_pDataUnit );
	
	return M4_ERROR;
  //## end ClBaseStorage::Get%903715593.body
}

m4return_t ClBaseStorage::Remove (m4uint32_t ai_ulDataUnit)
{
  //## begin ClBaseStorage::Remove%877602378.body preserve=yes
	return m_poDataUnitQueue->Delete( ai_ulDataUnit );
  //## end ClBaseStorage::Remove%877602378.body
}

void ClBaseStorage::Dump (m4pchar_t  &ao_strBuffer)
{
  //## begin ClBaseStorage::Dump%878295968.body preserve=yes
  //## end ClBaseStorage::Dump%878295968.body
}

void ClBaseStorage::Clear ()
{
  //## begin ClBaseStorage::Clear%878893910.body preserve=yes
	 m_poDataUnitQueue->clear();
     m_bHasFiles = M4_FALSE;
  //## end ClBaseStorage::Clear%878893910.body
}

m4return_t ClBaseStorage::AddFileUnit (m4pchar_t ai_FileName, m4bool_t ai_bEncrypted)
{
  //## begin ClBaseStorage::AddFileUnit%903536124.body preserve=yes

	ClFileUnit	*pFileUnit;
	pFileUnit = new ClFileUnit( ai_FileName, ai_bEncrypted );
	if (NULL == pFileUnit)
    {
        return M4_ERROR;
    }

    m_poDataUnitQueue->push_back( pFileUnit );
    m_bHasFiles = M4_TRUE;

	return M4_SUCCESS;
	
  //## end ClBaseStorage::AddFileUnit%903536124.body
}

void ClBaseStorage::SetTmpPath (m4pchar_t ai_pTempPath)
{
  //## begin ClBaseStorage::SetTempPath%903536125.body preserve=yes
  
	size_t size = 0;

	size = strlen(ai_pTempPath) +1 ;
	m_pTempPath = (m4pchar_t) malloc (size);
	memcpy(m_pTempPath,ai_pTempPath,size);
	
  //## end ClBaseStorage::SetTempPath%903536125.body
}

m4pchar_t ClBaseStorage::GetTmpPath ()
{
  //## begin ClBaseStorage::GetTempPath%903536126.body preserve=yes
 
	return m_pTempPath;
	
  //## end ClBaseStorage::GetTempPath%903536126.body
}

void ClBaseStorage::SetDefragSize (m4uint32_t ai_DefragSize)
{
  //## begin ClBaseStorage::SetDefragSize%903536134.body preserve=yes
 
	m_DefragSize = ai_DefragSize;
	
  //## end ClBaseStorage::SetDefragSize%903536134.body
}

m4uint32_t ClBaseStorage::GetDefragSize ()
{
  //## begin ClBaseStorage::GetDefragSize%903536135.body preserve=yes
  
	return m_DefragSize;
		 
  //## end ClBaseStorage::GetDefragSize%903536135.body
}

m4return_t ClBaseStorage::CheckDataStorage (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypted)
{
  //## begin ClBaseStorage::CheckDataStorage%903715592.body preserve=yes

	ClDataUnitInterface *pDataUnitInt;
	ClFileUnit *pFileUnit;

	if ( !m_poDataUnitQueue->empty() ) 
	{
		m_poDataUnitQueue->end(pDataUnitInt);
		if ( (pDataUnitInt -> GetType ()) == M4_FILE_UNIT ) 
		{
			pFileUnit=(ClFileUnit *) pDataUnitInt ;
			if (! pFileUnit-> GetSerialState ()) 
			{
				pFileUnit-> AttachBuffer ( ai_pBlock, ai_ulSize);
				return M4_SUCCESS;
			}
		}
	}

	if ( CheckForFileUnit( ai_pBlock, ai_ulSize )) 
	{
		pFileUnit=new ClFileUnit;
		pFileUnit -> SetFilePath(m_pTempPath);
		pFileUnit -> SetDefragSize(m_DefragSize);
		pFileUnit -> SetFileDescriptorString( ai_pBlock, ai_ulSize );

		free(ai_pBlock);
		Add ( pFileUnit );
	} 
	else 
		Add( ai_pBlock, ai_ulSize, ai_usEncrypted );
 
	return M4_SUCCESS;

  //## end ClBaseStorage::CheckDataStorage%903715592.body
}

m4bool_t ClBaseStorage::CheckForFileUnit (m4pchar_t ai_pBlock, m4uint32_t ai_blockSize)
{
  //## begin ClBaseStorage::CheckForFileUnit%903961923.body preserve=yes
 
	m4char_t keycad[] = M4_PDU_FILEUNIT_ID_STRING;

	int		iret = 0; 
	size_t	isize = 0;

	isize=sizeof(keycad)-1;		// Le quitamos el /0 
	isize = strlen(keycad);
	if (ai_blockSize<isize) 
		return M4_FALSE;

	iret = memcmp(keycad,ai_pBlock,isize);

	if (iret)
		return M4_FALSE;
	else 
		return M4_TRUE;


	
  //## end ClBaseStorage::CheckForFileUnit%903961923.body
}

m4bool_t ClBaseStorage::isDataUnitInterfaceQueueOwner ()
{
  //## begin ClBaseStorage::isDataUnitInterfaceQueueOwner%945078896.body preserve=yes
 
	return m_bOwner;

  //## end ClBaseStorage::isDataUnitInterfaceQueueOwner%945078896.body
}

void ClBaseStorage::SetDataUnitInterfaceQueueOwner (m4bool_t ai_bOwner)
{
  //## begin ClBaseStorage::SetDataUnitInterfaceQueueOwner%945078897.body preserve=yes
  
	m_bOwner = ai_bOwner;
	
  //## end ClBaseStorage::SetDataUnitInterfaceQueueOwner%945078897.body
}

ClDataUnitQueue * ClBaseStorage::GetDataUnitInterfaceQueue ()
{
  //## begin ClBaseStorage::GetDataUnitInterfaceQueue%945078898.body preserve=yes
  
	return m_poDataUnitQueue;

  //## end ClBaseStorage::GetDataUnitInterfaceQueue%945078898.body
}

// Class M4DataStorage 






M4DataStorage::M4DataStorage ()
  //## begin M4DataStorage::M4DataStorage%877685668.hasinit preserve=no
      : m_uiSize(0), m_bFinishSerialization(M4_FALSE), m_bObsoleteBlockFormat(M4_FALSE)
  //## end M4DataStorage::M4DataStorage%877685668.hasinit
  //## begin M4DataStorage::M4DataStorage%877685668.initialization preserve=yes
  //## end M4DataStorage::M4DataStorage%877685668.initialization
{
  //## begin M4DataStorage::M4DataStorage%877685668.body preserve=yes
	
	SetDataUnitInterfaceQueueOwner(M4_TRUE);
	
	m_poDataUnitQueue = new ClDataUnitQueue();
    if (NULL == m_poDataUnitQueue)
    {
        return;
    }
	m_oIter = m_poDataUnitQueue->begin();
	if (s_DefaultTemp) 
		SetTmpPath(s_DefaultTemp);
  //## end M4DataStorage::M4DataStorage%877685668.body
}

M4DataStorage::M4DataStorage (ClDataUnitQueue *ai_pDataUnitInterfaceQueue)
  //## begin M4DataStorage::M4DataStorage%945078894.hasinit preserve=no
      : m_uiSize(0), m_bFinishSerialization(M4_FALSE), m_bObsoleteBlockFormat(M4_FALSE)
  //## end M4DataStorage::M4DataStorage%945078894.hasinit
  //## begin M4DataStorage::M4DataStorage%945078894.initialization preserve=yes
  //## end M4DataStorage::M4DataStorage%945078894.initialization
{
  //## begin M4DataStorage::M4DataStorage%945078894.body preserve=yes
 
	SetDataUnitInterfaceQueueOwner(M4_FALSE);
	
	m_poDataUnitQueue = ai_pDataUnitInterfaceQueue;
	m_oIter = m_poDataUnitQueue->begin();
	if (s_DefaultTemp) 
		SetTmpPath(s_DefaultTemp);
	
  //## end M4DataStorage::M4DataStorage%945078894.body
}


M4DataStorage::~M4DataStorage ()
{
  //## begin M4DataStorage::~M4DataStorage%878295959.body preserve=yes
  //## end M4DataStorage::~M4DataStorage%878295959.body
}



//## Other Operations (implementation)
m4return_t M4DataStorage::Get (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted, m4uint32_t ai_ulBlock)
{
  //## begin M4DataStorage::Get%877602377.body preserve=yes

	return ( ClBaseStorage::Get( ao_pBlock, ao_ulSize
					, ao_bEncrypted, ai_ulBlock ) );

  //## end M4DataStorage::Get%877602377.body
}

m4return_t M4DataStorage::Add (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted)
{
  //## begin M4DataStorage::Add%878295969.body preserve=yes
	return ( ClBaseStorage::Add( ai_pBlock, ai_ulSize, ai_bEncrypted ) );
  //## end M4DataStorage::Add%878295969.body
}

m4return_t M4DataStorage::Add (ClDataUnitInterface *ai_pDataUnit)
{
  //## begin M4DataStorage::Add%885288224.body preserve=yes
	return ( ClBaseStorage::Add( ai_pDataUnit ) );
  //## end M4DataStorage::Add%885288224.body
}

m4return_t M4DataStorage::Remove (m4uint32_t ai_ulDataUnit)
{
  //## begin M4DataStorage::Remove%878295970.body preserve=yes
	m_bObsoleteBlockFormat=M4_TRUE;
	return ( ClBaseStorage::Remove( ai_ulDataUnit ) );
  //## end M4DataStorage::Remove%878295970.body
}

void M4DataStorage::Dump (m4pchar_t  &ao_strBuffer)
{
  //## begin M4DataStorage::Dump%878295965.body preserve=yes
	ClDataUnitInterface *pDataInterface;
		
	strcat(ao_strBuffer,"\n Dump del DataStorage .\n");
	while ( GetNext(pDataInterface)==M4_SUCCESS ) 
		pDataInterface -> Dump ( ao_strBuffer );
  
  //## end M4DataStorage::Dump%878295965.body
}

m4uint32_t M4DataStorage::Blocks ()
{
  //## begin M4DataStorage::Blocks%878806451.body preserve=yes
	return m_poDataUnitQueue->size();
  //## end M4DataStorage::Blocks%878806451.body
}

m4uint32_t M4DataStorage::Size ()
{
  //## begin M4DataStorage::Size%878806452.body preserve=yes
	ClDataUnitInterface	*pDataUnit;
	m4uint32_t	ulSize = 0;
	m4int32_t ulLocalSize =0;
	ClDataUnitQueue::iterator	oIterator;
	
	if(!NewData() && m_uiSize)
		return m_uiSize;

	if(!m_poDataUnitQueue->empty())
	{
		oIterator=m_poDataUnitQueue->begin();
		while(oIterator<=m_poDataUnitQueue->end())
		{
			 m_poDataUnitQueue->get(oIterator, pDataUnit);
			 if(pDataUnit)
			 {
				if ( (pDataUnit -> GetType()) == M4_FILE_UNIT ) 
				{
					ulLocalSize = ((ClFileUnit *)pDataUnit) -> GetTotalSize();
					if (ulLocalSize > 0 ) 
						ulSize += ulLocalSize;	
					ulSize+=((ClFileUnit *)pDataUnit) ->GetFileDescriptorStringSize();
				} 
				else 
					ulSize += pDataUnit -> GetSize();
			 }
			 else
			 {m4TraceLevel(2,cerr <<"Hay un hueco"<<endl);}
			 oIterator++;
		}
	}
	m_uiSize=ulSize;
	return ulSize;
  //## end M4DataStorage::Size%878806452.body
}

m4return_t M4DataStorage::GetNext (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted)
{
  //## begin M4DataStorage::GetNext%879336057.body preserve=yes
	ClDataUnitInterface	*pDataUnit=NULL;
	m4int8_t    modulo=0;
	
	if (m_poDataUnitQueue->empty() || m_oIter > m_poDataUnitQueue->end() )
	{
		ao_pBlock = NULL;
		ao_ulSize = 0;
		ao_bEncrypted = M4_FALSE;
		return M4_ERROR;
	}

	while(pDataUnit==NULL)			// Para salvar posibles huecos en el DataUnitQueue
	{
		m_poDataUnitQueue->get( m_oIter, pDataUnit );
		if(NULL==pDataUnit)
			m_oIter++;
	}

	M4_ASSERT(NULL!=pDataUnit);
	ao_pBlock = pDataUnit -> GetBuffer();
	ao_ulSize = pDataUnit -> GetSize();	
	
	ao_bEncrypted = pDataUnit -> GetCryptState();

	if (pDataUnit -> GetSerialState() )			// El serialState de un FileUnit no se pone a True hasta que no se ha enviado todo
	{
		m_oIter ++;
		if ((pDataUnit -> GetType ()) == M4_FILE_UNIT ) 
			pDataUnit -> SetSerialState(M4_FALSE);
	}
	
	return M4_SUCCESS;

  //## end M4DataStorage::GetNext%879336057.body
}

m4return_t M4DataStorage::GetNext (m4uint32_t ai_ulDataOffset, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4bool_t &ao_bEncrypted)
{
  //## begin M4DataStorage::GetNext%879425090.body preserve=yes
	ClDataUnitInterface	*pDataUnit = NULL;

	if ( m_poDataUnitQueue->empty() || m_oIter > m_poDataUnitQueue->end() )
	{
		ao_pBlock = NULL;
		ao_ulBlockSize = 0;
		return M4_ERROR;
	}

	while(NULL==pDataUnit)
	{
		m_poDataUnitQueue->get( m_oIter, pDataUnit );	
		if(NULL==pDataUnit )		// Para salvar los huecos
			m_oIter++;
	}

	ao_pBlock = pDataUnit -> GetBuffer();
	ao_ulBlockSize = pDataUnit -> GetChunkSize();
	ao_bEncrypted = pDataUnit -> GetCryptState();

	if (pDataUnit -> GetSerialState() ) 
	{
		m_oIter ++;
		if ((pDataUnit -> GetType ()) == M4_FILE_UNIT ) 
			pDataUnit -> SetSerialState(M4_FALSE);
	}
	
	return M4_SUCCESS;

  //## end M4DataStorage::GetNext%879425090.body
}

m4return_t M4DataStorage::GetNext (ClDataUnitInterface * &ao_pDataUnit)
{
  //## begin M4DataStorage::GetNext%885978336.body preserve=yes

	if (m_poDataUnitQueue->empty() ||  m_oIter > m_poDataUnitQueue->end() )
	{
		ao_pDataUnit = NULL;
		return M4_ERROR;
	}
	ao_pDataUnit=NULL;
	while(ao_pDataUnit==NULL)
	{
		m_poDataUnitQueue->get( m_oIter, ao_pDataUnit );		// Para salvar los huecos
		if(ao_pDataUnit==NULL)
			m_oIter ++;
	}

	if (ao_pDataUnit -> GetSerialState() || m_bFinishSerialization ) 
	{
		m_oIter ++;
//		if ((ao_pDataUnit -> GetType ()) == M4_FILE_UNIT ) 
//			ao_pDataUnit -> SetSerialState(M4_FALSE);
	}
	else
	{
		if ((ao_pDataUnit -> GetType ()) == M4_FILE_UNIT ) 
		{
			if ( ((ClFileUnit*)ao_pDataUnit)->GetFileSize()==0)
				m_oIter ++;
		}
	}

	return M4_SUCCESS;
  //## end M4DataStorage::GetNext%885978336.body
}

m4return_t M4DataStorage::GetNewDataUnit (ClDataUnit * &ao_pDataUnit)
{
  //## begin M4DataStorage::GetNewDataUnit%885978334.body preserve=yes
	m_bObsoleteBlockFormat=M4_TRUE;
	ClDataUnit* pDataUnit = NULL;

	pDataUnit = new ClDataUnit;
    if (NULL == pDataUnit)
    {
        ao_pDataUnit = NULL;
        return M4_ERROR;
    }
	ao_pDataUnit = pDataUnit;

	return Add( pDataUnit );

  //## end M4DataStorage::GetNewDataUnit%885978334.body
}

void M4DataStorage::InitializeIteration ()
{
  //## begin M4DataStorage::InitializeIteration%879336058.body preserve=yes
	m_oIter = m_poDataUnitQueue->begin();
  //## end M4DataStorage::InitializeIteration%879336058.body
}

m4return_t M4DataStorage::InitializeSerialState ()
{
  //## begin M4DataStorage::InitializeSerialState%940592255.body preserve=yes
  
	ClDataUnitInterface * pDataUnit;
	m4return_t iRet;

	InitializeIteration ();

	while (GetNext (pDataUnit) != M4_ERROR) 
			iRet = pDataUnit -> InitializeSerialState();

	InitializeIteration ();
	
	return iRet;
	
  //## end M4DataStorage::InitializeSerialState%940592255.body
}

void M4DataStorage::Clear ()
{
  //## begin M4DataStorage::Clear%886066130.body preserve=yes
	ClBaseStorage::Clear();
  //## end M4DataStorage::Clear%886066130.body
}

M4DataStorage * M4DataStorage::GetNewDataStorage ()
{
  //## begin M4DataStorage::GetNewDataStorage%886410895.body preserve=yes
	M4DataStorage *pDataStorage;
	pDataStorage = new	M4DataStorage();
	return pDataStorage;

  //## end M4DataStorage::GetNewDataStorage%886410895.body
}

M4DataStorage * M4DataStorage::GetNewDataStorage (ClDataUnitQueue *ai_pDataUnitInterfaceQueue)
{
  //## begin M4DataStorage::GetNewDataStorage%945078895.body preserve=yes
 
	M4DataStorage *pDataStorage;
	pDataStorage = new	M4DataStorage(ai_pDataUnitInterfaceQueue);
	return pDataStorage;
	
  //## end M4DataStorage::GetNewDataStorage%945078895.body
}

m4return_t M4DataStorage::DeleteDataStorage (M4DataStorage * &aio_pDataStorage)
{
  //## begin M4DataStorage::DeleteDataStorage%886410896.body preserve=yes
	if ( aio_pDataStorage == NULL )
		return M4_ERROR;
	delete	aio_pDataStorage;
	aio_pDataStorage = NULL;
	return M4_SUCCESS;
  //## end M4DataStorage::DeleteDataStorage%886410896.body
}

m4return_t M4DataStorage::GetNewFileUnit (ClFileUnit * &ao_pFileUnit)
{
  //## begin M4DataStorage::GetNewFileUnit%903449685.body preserve=yes
	m_bObsoleteBlockFormat=M4_TRUE;
	ao_pFileUnit = new ClFileUnit;
	return Add( ao_pFileUnit );
  //## end M4DataStorage::GetNewFileUnit%903449685.body
}

m4return_t M4DataStorage::AddFileUnit (m4pchar_t ai_FileName, m4bool_t ai_bEncrypted)
{
  //## begin M4DataStorage::AddFileUnit%903536127.body preserve=yes
	return ( ClBaseStorage::AddFileUnit( ai_FileName, ai_bEncrypted ) );
  //## end M4DataStorage::AddFileUnit%903536127.body
}

m4return_t M4DataStorage::GetCurrent (ClDataUnitInterface *&ao_pDataUnit)
{
  //## begin M4DataStorage::GetCurrent%932487947.body preserve=yes
	if ( m_oIter > m_poDataUnitQueue->end() )
	{
		ao_pDataUnit = NULL;
		return M4_ERROR;
	}

	m_poDataUnitQueue->get( m_oIter, ao_pDataUnit );
	
	return M4_SUCCESS;
  //## end M4DataStorage::GetCurrent%932487947.body
}

m4return_t M4DataStorage::SetDefaultPath (m4pchar_t ai_DefaultPath)
{
  //## begin M4DataStorage::SetDefaultPath%962624238.body preserve=yes
	if (s_DefaultTemp != NULL)
		return M4_SUCCESS;

	size_t size = 0;

	size = strlen(ai_DefaultPath) +1 ;
	s_DefaultTemp = (m4pchar_t) malloc (size);
	memcpy(s_DefaultTemp,ai_DefaultPath,size);

	return M4_SUCCESS;
  //## end M4DataStorage::SetDefaultPath%962624238.body
}

m4return_t M4DataStorage::CloneFiles( m4pcchar_t ai_pccPrerix )
{

	m4return_t			iResult = M4_ERROR ;
	size_t				iSize = 0 ;
	int					iLength = -1 ;
	m4pcchar_t			pccInputFile = NULL ;
	m4pchar_t			pcANSIFile = NULL ;
	m4char_t			acBuffer[ 4096 ] ;
	ClDataUnitInterface	*poDataUnit = NULL ;
	ClFileUnit			*pFileUnit = NULL ;
	FILE				*pfInput = NULL ;
	FILE				*pfOutPut = NULL ;
	ClFileManager		*poFileManager = NULL ;
	ClFileName			oOutputFile ;


	if( HasFileUnits() != M4_TRUE )
	{
		return M4_SUCCESS ;
	}

	poFileManager = ClFileManager::Instance() ;
	ClDataUnitQueue::iterator oIter = m_poDataUnitQueue->begin() ;

	while( oIter <= m_poDataUnitQueue->end() )
	{
		m_poDataUnitQueue->get( oIter, poDataUnit ) ;
		oIter++ ;

		if( poDataUnit == NULL || poDataUnit->GetType() != M4_FILE_UNIT )
		{
			continue ;
		}

		pFileUnit = (ClFileUnit *) poDataUnit ;
		pccInputFile = pFileUnit->GetFileName() ;

		if( pccInputFile == NULL )
		{
			continue ;
		}

		iResult = poFileManager->CreateTempFile( oOutputFile, ai_pccPrerix, pFileUnit->GetFileExtension() ) ;

		if( iResult != M4_SUCCESS )
		{
			continue ;
		}

		iLength = -1 ;
		pcANSIFile = M4CppToANSI( pccInputFile, iLength ) ;
		pfInput = fopen( pcANSIFile, "rb" ) ;
		delete( pcANSIFile ) ;

		if( pfInput == NULL )
		{
			continue ;
		}

		iLength = -1 ;
		pcANSIFile = M4CppToANSI( oOutputFile, iLength ) ;
		pfOutPut = fopen( pcANSIFile, "wb" ) ;
		delete( pcANSIFile ) ;

		if( pfOutPut == NULL )
		{
			fclose( pfInput ) ;
			continue ;
		}

		iSize = fread( acBuffer, sizeof( m4char_t ), 4096, pfInput ) ;

		while( iSize > 0 )
		{
			fwrite( acBuffer, sizeof( m4char_t ), iSize, pfOutPut ) ;
			iSize = fread( acBuffer, sizeof( m4char_t ), 4096, pfInput ) ;
		}

		fclose( pfOutPut ) ;
		fclose( pfInput ) ;

		pFileUnit->SetFile( (m4pchar_t)(m4pcchar_t)oOutputFile ) ;
		poFileManager->UnSetInterestInFile( oOutputFile ) ;
	}

	return M4_SUCCESS ;
}

m4bool_t M4DataStorage::NewData ()
{
  //## begin M4DataStorage::NewData%962696603.body preserve=yes
	if(m_bObsoleteBlockFormat)
	{
		m_bObsoleteBlockFormat=M4_FALSE;
		return M4_TRUE;
	}
	else
	{
		return M4_FALSE;
	}
  //## end M4DataStorage::NewData%962696603.body
}

M4DataStorage * M4DataStorage::GetNewDataStorage (M4DataStorage *ai_pDataStorage, m4bool_t ai_bCopyIt)
{
  //## begin M4DataStorage::GetNewDataStorage%963819982.body preserve=yes
	M4DataStorage * theDS=NULL;
	if(M4_TRUE==ai_bCopyIt)
	{
		cerr << "Not available!!!!"<<endl;
		return theDS;
	}
	if(NULL==ai_pDataStorage)
	{
		m4Trace(cerr <<"NULL datastorage"<<endl);
		return theDS;
	}
	theDS= new	M4DataStorage(ai_pDataStorage->GetDataUnitInterfaceQueue());
	return theDS;
  //## end M4DataStorage::GetNewDataStorage%963819982.body
}


m4return_t M4DataStorage::GetNewCheckUnit (ClCheckUnit * &ao_pCheckUnit)
{
	ClCheckUnit* pCheckUnit = NULL;

	pCheckUnit = new ClCheckUnit;
    if (NULL == pCheckUnit)
    {
        ao_pCheckUnit = NULL;
        return M4_ERROR;
    }
	ao_pCheckUnit = pCheckUnit;

	return Add( pCheckUnit );
}

// Class ClFormatQueue 





ClFormatQueue::ClFormatQueue ()
  //## begin ClFormatQueue::ClFormatQueue%887968732.hasinit preserve=no
      : m_ArraypData(NULL), m_ulTotalSize(0), m_ulFilled(0)
  //## end ClFormatQueue::ClFormatQueue%887968732.hasinit
  //## begin ClFormatQueue::ClFormatQueue%887968732.initialization preserve=yes
  //## end ClFormatQueue::ClFormatQueue%887968732.initialization
{
  //## begin ClFormatQueue::ClFormatQueue%887968732.body preserve=yes
	if ( m_ArraypData = (ClBlockFormat * *) calloc( M4_DATASTORAGE_DEFAULT_DATAUNIT_NUMBER, sizeof( m4pchar_t ) ) )
	{
		m_ulTotalSize = M4_DATASTORAGE_DEFAULT_DATAUNIT_NUMBER;
	}
	else
	{
		m_ulTotalSize = 0;
	}
  //## end ClFormatQueue::ClFormatQueue%887968732.body
}


ClFormatQueue::~ClFormatQueue ()
{
  //## begin ClFormatQueue::~ClFormatQueue%887968733.body preserve=yes
  
	free( m_ArraypData );
	
  //## end ClFormatQueue::~ClFormatQueue%887968733.body
}



//## Other Operations (implementation)
m4return_t ClFormatQueue::push_back (ClBlockFormat *ai_data)
{
  //## begin ClFormatQueue::push_back%887968726.body preserve=yes
	
	m_ArraypData[ m_ulFilled ] = ai_data;
	m_ulFilled ++;

	if ( m_ulFilled >= m_ulTotalSize )
	{
		m_ArraypData = (ClBlockFormat * * ) realloc( m_ArraypData, ( m_ulTotalSize + 32 ) * sizeof( m4pchar_t ) );

		if ( m_ArraypData == NULL )
		{
			return M4_ERROR;
		}
		m_ulTotalSize += 32;
	}
	return M4_SUCCESS;
	
  //## end ClFormatQueue::push_back%887968726.body
}

m4return_t ClFormatQueue::erase (ClFormatQueue::iterator ai_ulPos)
{
  //## begin ClFormatQueue::erase%887968730.body preserve=yes
  
	if ( ai_ulPos >= m_ulTotalSize )
	{
		m4Trace(cerr <<"m_ulTotalSize="<< m_ulTotalSize<<".Referenciando a "<<ai_ulPos <<endl);
		return M4_ERROR;
	}
	
	if(ai_ulPos >=m_ulFilled)
	{
		m4Trace(cerr <<"Solo hay "<< m_ulFilled<<".Referenciando a "<<ai_ulPos <<endl);
		return M4_ERROR;
	}
	else
	{
		m_ArraypData[ ai_ulPos ] = NULL;
		m_ulFilled --;
		return M4_SUCCESS;
	}	
  //## end ClFormatQueue::erase%887968730.body
}

ClFormatQueue::iterator ClFormatQueue::begin ()
{
  //## begin ClFormatQueue::begin%888225518.body preserve=yes
 
	return 0;
	
  //## end ClFormatQueue::begin%888225518.body
}

ClFormatQueue::iterator ClFormatQueue::begin (ClBlockFormat * &ao_pData)
{
  //## begin ClFormatQueue::begin%887968728.body preserve=yes
  

	if ( m_ulTotalSize == 0 )
	{
		ao_pData = NULL;
		return 0;
	}


	ao_pData = m_ArraypData[ 0 ];
	return 0;
	
  //## end ClFormatQueue::begin%887968728.body
}

ClFormatQueue::iterator ClFormatQueue::end ()
{
  //## begin ClFormatQueue::end%888225519.body preserve=yes
  
	return m_ulFilled;
	
  //## end ClFormatQueue::end%888225519.body
}

ClFormatQueue::iterator ClFormatQueue::end (ClBlockFormat * &ao_pData)
{
  //## begin ClFormatQueue::end%887968729.body preserve=yes
  
	ao_pData = m_ArraypData[ m_ulFilled - 1 ];
	return m_ulFilled;
	
  //## end ClFormatQueue::end%887968729.body
}

m4return_t ClFormatQueue::resize ()
{
  //## begin ClFormatQueue::resize%887968731.body preserve=yes
  
	return M4_ERROR;
	
  //## end ClFormatQueue::resize%887968731.body
}

m4return_t ClFormatQueue::get (ClFormatQueue::iterator ao_Iterator, ClBlockFormat * &ao_pData)
{
  //## begin ClFormatQueue::get%888225514.body preserve=yes
  
	if ( (0==m_ulFilled) || ( ao_Iterator >= m_ulFilled ) )
		return M4_ERROR;

	ao_pData = m_ArraypData[ ao_Iterator ];
	return M4_SUCCESS;
	
  //## end ClFormatQueue::get%888225514.body
}

m4return_t ClFormatQueue::get_next (ClFormatQueue::iterator ao_Iterator, ClBlockFormat * &ao_pData)
{
  //## begin ClFormatQueue::get_next%888225515.body preserve=yes
  
	if ( ao_Iterator >= m_ulFilled )
	{
		return M4_ERROR;
	}
	ao_pData = m_ArraypData[ ao_Iterator ];
	return M4_SUCCESS;
	
  //## end ClFormatQueue::get_next%888225515.body
}

m4uint32_t ClFormatQueue::size ()
{
  //## begin ClFormatQueue::size%888225516.body preserve=yes
  
	return m_ulFilled;
	
  //## end ClFormatQueue::size%888225516.body
}

m4bool_t ClFormatQueue::empty ()
{
  //## begin ClFormatQueue::empty%888225517.body preserve=yes
  
	if ( m_ulFilled == 0 )
	{
		return M4_TRUE;
	}

	return M4_FALSE;
	
  //## end ClFormatQueue::empty%888225517.body
}

void ClFormatQueue::clear ()
{
  //## begin ClFormatQueue::clear%888685577.body preserve=yes
  
	m_ulFilled = 0;
	
  //## end ClFormatQueue::clear%888685577.body
}

// Additional Declarations
  //## begin ClFormatQueue%34ED4BB701B7.declarations preserve=yes
  //## end ClFormatQueue%34ED4BB701B7.declarations

// Class ClDataUnitQueue 







ClDataUnitQueue::ClDataUnitQueue ()
  //## begin ClDataUnitQueue::ClDataUnitQueue%888826864.hasinit preserve=no
      : m_ArraypData(NULL), m_ulTotalSize(0), m_ulFilled(0), m_uiFirstDataUnit(0), m_uiLastDataUnit(0)
  //## end ClDataUnitQueue::ClDataUnitQueue%888826864.hasinit
  //## begin ClDataUnitQueue::ClDataUnitQueue%888826864.initialization preserve=yes
  //## end ClDataUnitQueue::ClDataUnitQueue%888826864.initialization
{
  //## begin ClDataUnitQueue::ClDataUnitQueue%888826864.body preserve=yes
  	if ( m_ArraypData = (ClDataUnitInterface * *) calloc( M4_DATASTORAGE_DEFAULT_DATAUNIT_NUMBER, sizeof( m4pchar_t ) ) )
		m_ulTotalSize = M4_DATASTORAGE_DEFAULT_DATAUNIT_NUMBER;
	else
		m_ulTotalSize = 0;

  //## end ClDataUnitQueue::ClDataUnitQueue%888826864.body
}


ClDataUnitQueue::~ClDataUnitQueue ()
{
  //## begin ClDataUnitQueue::~ClDataUnitQueue%888826865.body preserve=yes
	clear();
	free( m_ArraypData );
  //## end ClDataUnitQueue::~ClDataUnitQueue%888826865.body
}



//## Other Operations (implementation)
m4return_t ClDataUnitQueue::push_back (ClDataUnitInterface *ai_data)
{
  //## begin ClDataUnitQueue::push_back%888826866.body preserve=yes
    if (NULL == m_ArraypData)
    {
        return M4_ERROR;
    }
	m_ArraypData[ m_ulFilled ] = ai_data;
	m_uiLastDataUnit=m_ulFilled ;
	m_ulFilled ++;

	if ( m_ulFilled >= m_ulTotalSize )
	{
		m_ArraypData = (ClDataUnitInterface * * ) realloc( m_ArraypData, ( m_ulTotalSize + 32 ) * sizeof( m4pchar_t ) );

		if ( m_ArraypData == NULL )
		{
			return M4_ERROR;
		}
		m_ulTotalSize += 32;
	}
	return M4_SUCCESS;
	
  //## end ClDataUnitQueue::push_back%888826866.body
}

ClDataUnitQueue::iterator ClDataUnitQueue::begin ()
{
  //## begin ClDataUnitQueue::begin%888826868.body preserve=yes
		return m_uiFirstDataUnit;
  //## end ClDataUnitQueue::begin%888826868.body
}

ClDataUnitQueue::iterator ClDataUnitQueue::begin (ClDataUnitInterface * &ao_pData)
{
  //## begin ClDataUnitQueue::begin%888826869.body preserve=yes
 	if ( m_ulFilled )
		ao_pData = m_ArraypData[ m_uiFirstDataUnit ];
	else
		ao_pData = NULL;

	return m_uiFirstDataUnit;
  //## end ClDataUnitQueue::begin%888826869.body
}

ClDataUnitQueue::iterator ClDataUnitQueue::end ()
{
  //## begin ClDataUnitQueue::end%888826870.body preserve=yes
	return m_uiLastDataUnit;
  //## end ClDataUnitQueue::end%888826870.body
}

ClDataUnitQueue::iterator ClDataUnitQueue::end (ClDataUnitInterface * &ao_pData)
{
  //## begin ClDataUnitQueue::end%888826871.body preserve=yes
	if (m_ulFilled) 
		ao_pData = m_ArraypData[ m_uiLastDataUnit];
	else
		ao_pData =NULL;
	return m_uiLastDataUnit;
  //## end ClDataUnitQueue::end%888826871.body
}

m4return_t ClDataUnitQueue::get (ClDataUnitQueue::iterator ao_Iterator, ClDataUnitInterface * &ao_pData)
{
  //## begin ClDataUnitQueue::get%888826873.body preserve=yes
	
	if( !m_ulFilled || (m_ulTotalSize<=ao_Iterator) || ( ao_Iterator <m_uiFirstDataUnit) || ( ao_Iterator > m_uiLastDataUnit ))
	{
		ao_pData=NULL;
		return M4_ERROR;
	}
	ao_pData = m_ArraypData[ ao_Iterator ];
	if(NULL==ao_pData )		//Existe un hueco
		return M4_ERROR; 
	else
		return M4_SUCCESS;
	
  //## end ClDataUnitQueue::get%888826873.body
}

m4uint32_t ClDataUnitQueue::size ()
{
  //## begin ClDataUnitQueue::size%888826875.body preserve=yes
	return m_ulFilled;
  //## end ClDataUnitQueue::size%888826875.body
}

m4bool_t ClDataUnitQueue::empty ()
{
  //## begin ClDataUnitQueue::empty%888826876.body preserve=yes
	if ( m_ulFilled == 0 )
		return M4_TRUE;
	else
		return M4_FALSE;
  //## end ClDataUnitQueue::empty%888826876.body
}

void ClDataUnitQueue::clear ()
{
  //## begin ClDataUnitQueue::clear%888826877.body preserve=yes
	if(m_ulFilled )
	{
		ClDataUnitQueue::iterator	iter;
		ClDataUnitInterface *pDataUnit = NULL;
		m4return_t iRet=M4_SUCCESS;

		iter = begin( pDataUnit );
		
		while (NULL!=pDataUnit )
		{
			Delete( iter );
			iter= begin( pDataUnit );
		}
	
		M4_ASSERT(m_ulFilled==0);

 		m_ulFilled = 0;
		m_uiFirstDataUnit=0;
		m_uiLastDataUnit =0;
	}
  //## end ClDataUnitQueue::clear%888826877.body
}

m4return_t ClDataUnitQueue::Delete (ClDataUnitQueue::iterator ai_ulPos)
{
  //## begin ClDataUnitQueue::Delete%963315749.body preserve=yes
	if ( ai_ulPos >= m_ulTotalSize )
	{
		m4Trace(cerr <<"m_ulTotalSize="<< m_ulTotalSize<<".Referenciando a "<<ai_ulPos <<endl);
		return M4_ERROR;
	}
	if(ai_ulPos > 	m_uiLastDataUnit )
	{
		m4Trace(cerr <<"Solo hay "<< m_uiLastDataUnit <<".Referenciando a "<<ai_ulPos <<endl);
		return M4_ERROR;
	}

	if(ai_ulPos < 	m_uiFirstDataUnit )
	{
		m4Trace(cerr <<"La primera es"<< m_uiLastDataUnit <<".Referenciando a "<<ai_ulPos <<endl);
		return M4_ERROR;
	}

	m_ulFilled --;
	delete m_ArraypData[ ai_ulPos ];

	m_ArraypData[ ai_ulPos ] = NULL;
	if(ai_ulPos == m_uiFirstDataUnit)
	{
		while(m_ulFilled && (m_ArraypData[m_uiFirstDataUnit]==NULL ) )
		{
			m_uiFirstDataUnit++;
		}
	}
	
	if(ai_ulPos == m_uiLastDataUnit)
	{
		while(m_ulFilled && (m_ArraypData[m_uiLastDataUnit]==NULL ) )
		{
			m_uiLastDataUnit--;
		}
	}
		
	return M4_SUCCESS;

  //## end ClDataUnitQueue::Delete%963315749.body
}

// Class ClDataUnitInterface 





ClDataUnitInterface::ClDataUnitInterface ()
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369740.hasinit preserve=no
      : m_ulSize(0), m_pBuffer(NULL), m_usEncrypt(M4_FALSE), m_usSerialState(M4_TRUE)
  //## end ClDataUnitInterface::ClDataUnitInterface%903369740.hasinit
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369740.initialization preserve=yes
  //## end ClDataUnitInterface::ClDataUnitInterface%903369740.initialization
{
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369740.body preserve=yes
  //## end ClDataUnitInterface::ClDataUnitInterface%903369740.body
}

ClDataUnitInterface::ClDataUnitInterface (m4pchar_t ai_pBuffer, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypt, m4uint32_t ai_ulOffset)
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369741.hasinit preserve=no
      : m_ulSize(0), m_pBuffer(NULL), m_usEncrypt(M4_FALSE), m_usSerialState(M4_TRUE)
  //## end ClDataUnitInterface::ClDataUnitInterface%903369741.hasinit
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369741.initialization preserve=yes
  //## end ClDataUnitInterface::ClDataUnitInterface%903369741.initialization
{
  //## begin ClDataUnitInterface::ClDataUnitInterface%903369741.body preserve=yes
  //## end ClDataUnitInterface::ClDataUnitInterface%903369741.body
}


ClDataUnitInterface::~ClDataUnitInterface ()
{
  //## begin ClDataUnitInterface::~ClDataUnitInterface%903369742.body preserve=yes
Clear();
  //## end ClDataUnitInterface::~ClDataUnitInterface%903369742.body
}



//## Other Operations (implementation)
m4uint32_t ClDataUnitInterface::GetSize ()
{
  //## begin ClDataUnitInterface::GetSize%903369743.body preserve=yes
	return 0;
  //## end ClDataUnitInterface::GetSize%903369743.body
}

m4pchar_t ClDataUnitInterface::GetBuffer ()
{
  //## begin ClDataUnitInterface::GetBuffer%903369744.body preserve=yes
	return NULL;
  //## end ClDataUnitInterface::GetBuffer%903369744.body
}

m4bool_t ClDataUnitInterface::GetCryptState ()
{
  //## begin ClDataUnitInterface::GetCryptState%903369745.body preserve=yes
	return M4_FALSE;
  //## end ClDataUnitInterface::GetCryptState%903369745.body
}

void ClDataUnitInterface::Clear ()
{
  //## begin ClDataUnitInterface::Clear%903369746.body preserve=yes
	if (m_pBuffer != NULL ) {
		free ( m_pBuffer );
		m_pBuffer = NULL;
		m_ulSize=0;
		m_usEncrypt=M4_FALSE;
	}
  //## end ClDataUnitInterface::Clear%903369746.body
}

void ClDataUnitInterface::Dump (m4pchar_t  &ao_strBuffer)
{
  //## begin ClDataUnitInterface::Dump%903369747.body preserve=yes
  //## end ClDataUnitInterface::Dump%903369747.body
}

m4return_t ClDataUnitInterface::SetSize (m4uint32_t ai_uiSize)
{
  //## begin ClDataUnitInterface::SetSize%903369748.body preserve=yes
	return M4_ERROR;
  //## end ClDataUnitInterface::SetSize%903369748.body
}

m4int16_t ClDataUnitInterface::GetType ()
{
  //## begin ClDataUnitInterface::GetType%903536128.body preserve=yes
	return M4_DATA_UNIT;
  //## end ClDataUnitInterface::GetType%903536128.body
}

m4bool_t ClDataUnitInterface::GetSerialState ()
{
  //## begin ClDataUnitInterface::GetSerialState%903603130.body preserve=yes
	return m_usSerialState;
  //## end ClDataUnitInterface::GetSerialState%903603130.body
}

void ClDataUnitInterface::SetSerialState (m4bool_t ai_SerialState)
{
  //## begin ClDataUnitInterface::SetSerialState%903603131.body preserve=yes
	m_usSerialState = ai_SerialState;
  //## end ClDataUnitInterface::SetSerialState%903603131.body
}

void ClDataUnitInterface::SetCryptState (m4bool_t ai_bCrypt)
{
  //## begin ClDataUnitInterface::SetCryptState%932487948.body preserve=yes
	m_usEncrypt = ai_bCrypt;
  //## end ClDataUnitInterface::SetCryptState%932487948.body
}

m4return_t ClDataUnitInterface::InitializeSerialState ()
{
  //## begin ClDataUnitInterface::InitializeSerialState%940592256.body preserve=yes
	SetSerialState(M4_FALSE);
	return M4_SUCCESS;
  //## end ClDataUnitInterface::InitializeSerialState%940592256.body
}

m4return_t ClDataUnitInterface::SetBufferSize (m4uint_t ai_uiBufferSize)
{
  //## begin ClDataUnitInterface::SetBufferSize%970483019.body preserve=yes
	if(ai_uiBufferSize!=m_ulSize)
	{
		if(m_pBuffer )
			m_pBuffer = (m4pchar_t)realloc(m_pBuffer, ai_uiBufferSize);
		else
			m_pBuffer = (m4pchar_t)calloc( ai_uiBufferSize,1);
	}
	if (NULL == m_pBuffer)
	{
		m_ulSize = 0;
		return M4_ERROR;
	}
	else
	{
		m_ulSize = ai_uiBufferSize;
		return M4_SUCCESS;
	}
  //## end ClDataUnitInterface::SetBufferSize%970483019.body
}

m4uint32_t ClDataUnitInterface::GetChunkSize ()
{
  //## begin ClDataUnitInterface::GetChunkSize%970483022.body preserve=yes
	return m_ulSize;
  //## end ClDataUnitInterface::GetChunkSize%970483022.body
}

// Additional Declarations
  //## begin ClDataUnitInterface%35D85D2C012E.declarations preserve=yes
  //## end ClDataUnitInterface%35D85D2C012E.declarations

// Class ClFileUnit 












ClFileUnit::ClFileUnit ()
  //## begin ClFileUnit::ClFileUnit%903369749.hasinit preserve=no
      : m_pFileName(NULL), m_pFileExtension(NULL), m_pFileDescriptor(NULL), m_uiFileIterator(0), m_uiDefragSize(M4_PDU_FRAGMENTATION_SIZE), m_uiTotalSize(M4PDUS_FILEUNIT_UNDEFINED_SIZE), m_pTempPath(NULL), m_pSourceFileName(NULL), m_bCompressed(M4_FALSE), m_bGetFileNameString(M4_TRUE), m_bGetFileNameStringSize(M4_TRUE)
  //## end ClFileUnit::ClFileUnit%903369749.hasinit
  //## begin ClFileUnit::ClFileUnit%903369749.initialization preserve=yes
  //## end ClFileUnit::ClFileUnit%903369749.initialization
{
  //## begin ClFileUnit::ClFileUnit%903369749.body preserve=yes
	SetSerialState(M4_FALSE);
  //## end ClFileUnit::ClFileUnit%903369749.body
}

ClFileUnit::ClFileUnit (m4pchar_t ai_pFileName, m4bool_t ai_usEncrypt, m4uint32_t ai_ulOffset)
  //## begin ClFileUnit::ClFileUnit%903369750.hasinit preserve=no
      : m_pFileName(NULL), m_pFileExtension(NULL), m_pFileDescriptor(NULL), m_uiFileIterator(0), m_uiDefragSize(M4_PDU_FRAGMENTATION_SIZE), m_uiTotalSize(M4PDUS_FILEUNIT_UNDEFINED_SIZE), m_pTempPath(NULL), m_pSourceFileName(NULL), m_bCompressed(M4_FALSE), m_bGetFileNameString(M4_TRUE), m_bGetFileNameStringSize(M4_TRUE)
  //## end ClFileUnit::ClFileUnit%903369750.hasinit
  //## begin ClFileUnit::ClFileUnit%903369750.initialization preserve=yes
  //## end ClFileUnit::ClFileUnit%903369750.initialization
{
  //## begin ClFileUnit::ClFileUnit%903369750.body preserve=yes
    SetFile (ai_pFileName);
	SetSerialState(M4_FALSE);
	m_usEncrypt = ai_usEncrypt;
  //## end ClFileUnit::ClFileUnit%903369750.body
}


ClFileUnit::~ClFileUnit ()
{
  //## begin ClFileUnit::~ClFileUnit%903369751.body preserve=yes
  	if(NULL!=m_pFileDescriptor)
	{
		fclose(m_pFileDescriptor);
		m_pFileDescriptor=NULL;
	}

	if (m_pFileName != NULL ) 
	{
		ClFileManager* myFileManager = ClFileManager::Instance();
		myFileManager -> UnSetInterestInFile(m_pFileName);

		free ( m_pFileName );
	}

	if (m_pFileExtension != NULL ) 
		free ( m_pFileExtension );

	if (m_pSourceFileName) 
		free(m_pSourceFileName);


  //## end ClFileUnit::~ClFileUnit%903369751.body
}



//## Other Operations (implementation)
m4uint32_t ClFileUnit::GetSize ()
{
  //## begin ClFileUnit::GetSize%903369752.body preserve=yes
	if(m_uiTotalSize==M4PDUS_FILEUNIT_UNDEFINED_SIZE)
		m_uiTotalSize=GetFileSize();
	return m_uiTotalSize+GetFileDescriptorStringSize();
  //## end ClFileUnit::GetSize%903369752.body
}

m4pchar_t ClFileUnit::GetBuffer ()
{
  //## begin ClFileUnit::GetBuffer%903369753.body preserve=yes
	int iLength = -1 ;
	m4pchar_t pcANSIFile = NULL ;

	if(m_bGetFileNameString)
	{
		GetFileDescriptorString();
		SetSerialState(M4_FALSE);
		m_bGetFileNameString=M4_FALSE;
		m_uiTotalSize=GetFileSize();
		return m_pBuffer;
	}

	m4size_t sReaded=0;
	if (!GetSerialState()) 
	{
		fpos_t pos;
		if (m_pFileDescriptor == NULL ) 
		{
			/* Bug 0098932
			Hay que abrir el fichero si no está abierto
			*/
			iLength = -1 ;
			pcANSIFile = M4CppToANSI( m_pFileName, iLength ) ;
	 		m_pFileDescriptor = fopen( pcANSIFile, "rb" ) ; 
			delete( pcANSIFile ) ;

			if( m_pFileDescriptor == NULL )
			{
				return( NULL ) ;
			}
		}

		if(m_ulSize!=m_uiDefragSize)
			SetBufferSize(m_uiDefragSize);

		sReaded=fread(m_pBuffer,m_uiDefragSize,1,m_pFileDescriptor)*m_uiDefragSize;
		m_ulSize = m_uiTotalSize - m_uiFileIterator;
		if (m_ulSize > m_uiDefragSize ) 
			m_ulSize = m_uiDefragSize; 
		fgetpos ( m_pFileDescriptor , &pos);
		m_uiFileIterator = (m4uint32_t) pos;

		if(m_uiFileIterator>=m_uiTotalSize)
		{
			SetSerialState(M4_TRUE);
			fclose(m_pFileDescriptor);
			m_pFileDescriptor = NULL;
			m_uiFileIterator = 0;
		}
		else
			SetSerialState(M4_FALSE);
	}
	
	return m_pBuffer;

  //## end ClFileUnit::GetBuffer%903369753.body
}

m4pchar_t ClFileUnit::GetFileName ()
{
  //## begin ClFileUnit::GetFileName%903536129.body preserve=yes
	return m_pFileName;
  //## end ClFileUnit::GetFileName%903536129.body
}

m4pchar_t ClFileUnit::GetOnlyFileName ()
{
  //## begin ClFileUnit::GetOnlyFileName%904235866.body preserve=yes

	if(NULL==m_pFileName)
		return NULL;

	m4pchar_t FileName;
	
#ifdef _UNIX 
	m4int32_t Sep = '/';
#else 
	m4int32_t Sep = '\\';
#endif

	FileName = strrchr((char *)m_pFileName,(int)Sep);
	if (!FileName) 
		FileName = m_pFileName; 
	else 
		FileName++;  
	return FileName;    
	
  //## end ClFileUnit::GetOnlyFileName%904235866.body
}

m4pchar_t ClFileUnit::GetFilePath ()
{
  //## begin ClFileUnit::GetFilePath%904051589.body preserve=yes
	return m_pTempPath;
  //## end ClFileUnit::GetFilePath%904051589.body
}

m4pchar_t ClFileUnit::GetFileExtension ()
{
  //## begin ClFileUnit::GetFileExtension%903536130.body preserve=yes
	return m_pFileExtension;
  //## end ClFileUnit::GetFileExtension%903536130.body
}

m4pchar_t ClFileUnit::GetSourceFileName ()
{
  //## begin ClFileUnit::GetSourceFileName%904575294.body preserve=yes
	return m_pSourceFileName;
  //## end ClFileUnit::GetSourceFileName%904575294.body
}

FILE * ClFileUnit::GetFileDescriptor ()
{
  //## begin ClFileUnit::GetFileDescriptor%903536131.body preserve=yes
	return m_pFileDescriptor;
  //## end ClFileUnit::GetFileDescriptor%903536131.body
}

m4bool_t ClFileUnit::GetCryptState ()
{
  //## begin ClFileUnit::GetCryptState%903369754.body preserve=yes
	return m_usEncrypt;
  //## end ClFileUnit::GetCryptState%903369754.body
}

void ClFileUnit::Dump (m4pchar_t  &ao_strBuffer)
{
  //## begin ClFileUnit::Dump%903369756.body preserve=yes
 
	m4pchar_t auxcad = (m4pchar_t)malloc(32);

  	strcat (ao_strBuffer,"\n + FileUnit.");
	strcat (ao_strBuffer,"\n     - Size. ");
	sprintf(auxcad,"%i",GetTotalSize());
	strcat (ao_strBuffer,auxcad);
	strcat (ao_strBuffer,"\n     - Name. ");
	strcat (ao_strBuffer,GetFileName());
	strcat (ao_strBuffer,"\n     - SourceName. ");
	if (GetSourceFileName()) strcat (ao_strBuffer,GetSourceFileName());
	else strcat (ao_strBuffer,"No Source Name.");
	strcat (ao_strBuffer,"\n     - Encrypted. ");
	switch ( m_usEncrypt )
	{
	case M4_ENCRYPTED:
		strcat (ao_strBuffer,"yes");
		break;
	case M4_DECRYPTED:
		strcat (ao_strBuffer,"no");
		break;
	}

	free (auxcad);
	
  //## end ClFileUnit::Dump%903369756.body
}

m4return_t ClFileUnit::SetFile (m4pchar_t ai_pFileName)
{
  //## begin ClFileUnit::SetFile%903536132.body preserve=yes
  
	size_t		size;
	m4pchar_t	auxcad = NULL;

	ClFileManager* myFileManager = ClFileManager::Instance();
	
	if (m_pFileName != NULL ) 
	{
		myFileManager -> UnSetInterestInFile(m_pFileName);
		free ( m_pFileName );
		m_pFileName = NULL;
	}
	
	if (m_pFileExtension != NULL ) 
	{
		free ( m_pFileExtension );
		m_pFileExtension = NULL;
	}
	
	size  = strlen ( ai_pFileName ) + 1;
	
	m_pFileName = (m4pchar_t) malloc ( size );
	memcpy(m_pFileName, ai_pFileName, size );
	
	auxcad = strrchr (m_pFileName, '.');
	if (auxcad) 
	{
		auxcad++;
		size = strlen ( auxcad ) + 1 ;
		m_pFileExtension = (m4pchar_t) malloc ( size );
		memcpy(m_pFileExtension, auxcad, size );
		
	}
	
	myFileManager -> SetInterestInFile(m_pFileName);
	
	return M4_SUCCESS;
  //## end ClFileUnit::SetFile%903536132.body
}

m4return_t ClFileUnit::SetFilePath (m4pchar_t ai_pFileName)
{
  //## begin ClFileUnit::SetFilePath%904051592.body preserve=yes
  
	m4return_t ret;

	if (m_pTempPath) 
		ret =  M4_WARNING;
	else 
		ret = M4_SUCCESS;

	m_pTempPath = ai_pFileName;
	
	return ret;
	
  //## end ClFileUnit::SetFilePath%904051592.body
}

m4int16_t ClFileUnit::GetType ()
{
  //## begin ClFileUnit::GetType%903536133.body preserve=yes
	return M4_FILE_UNIT;
  //## end ClFileUnit::GetType%903536133.body
}

m4return_t ClFileUnit::AttachBuffer (m4pchar_t ai_pBuffer, m4uint32_t ai_size)
{
  //## begin ClFileUnit::AttachBuffer%903603132.body preserve=yes
 
	size_t			ulwrited = 0;
	int				iLength = -1 ;
	m4pchar_t		pcANSIFile = NULL ;
	size_t			iElementSize = (m4uint32_t)ai_size;
	size_t			iElementCount = 1;
	ClFileManager*	myFileManager = ClFileManager::Instance();
	
	if (m_pFileDescriptor==NULL)
	{
		// Bug 0308827. Hay que pasar el path a ANSI
		iLength = -1 ;
		pcANSIFile = M4CppToANSI( m_pFileName, iLength ) ;
		m_pFileDescriptor = fopen( pcANSIFile, "wb" ) ;
		delete( pcANSIFile ) ;
	}
	
	if (m_pFileDescriptor!=NULL) 
	{ 
		// Si podemos escribir ....
		 
		ulwrited = fwrite(ai_pBuffer,iElementSize,iElementCount,m_pFileDescriptor);
		//if (ulwrited!=ai_pBuffer) -> Error, nos hemos quedado sin espacio en disco. 
		free(ai_pBuffer);
		fflush(m_pFileDescriptor);
	}	
	
	m_uiFileIterator += ai_size;
	if(m_uiFileIterator>=m_uiTotalSize)
	{
		SetSerialState(M4_TRUE);
		if (m_pFileDescriptor!=NULL) 
			fclose(m_pFileDescriptor);

		m_pFileDescriptor = NULL;
		m_uiFileIterator = 0;
	}
	else
		SetSerialState(M4_FALSE);

	return M4_SUCCESS;
	
  //## end ClFileUnit::AttachBuffer%903603132.body
}

void ClFileUnit::SetDefragSize (m4uint32_t ai_uiDefragSize)
{
  //## begin ClFileUnit::SetDefragSize%903603133.body preserve=yes
	if(m_uiDefragSize != ai_uiDefragSize)
		m_uiDefragSize = ai_uiDefragSize;
  //## end ClFileUnit::SetDefragSize%903603133.body
}

m4uint32_t ClFileUnit::GetFileDescriptorString ()
{
  //## begin ClFileUnit::GetFileDescriptorString%903715590.body preserve=yes
  
	m4uint32_t size;
	m4char_t auxcad[32];
	sprintf( auxcad,"%d", m_uiTotalSize );

	size = (m4uint32_t) (strlen(M4_PDU_FILEUNIT_ID_STRING) + strlen(GetOnlyFileName()) +  strlen(M4_PDU_FILEUNIT_SIZE_STRING) + strlen(auxcad) + 1);

	if (size!= m_ulSize)
		SetBufferSize(size);

	sprintf( m_pBuffer,"%s%s%s%s",M4_PDU_FILEUNIT_ID_STRING,GetOnlyFileName(),M4_PDU_FILEUNIT_SIZE_STRING,auxcad );

	return  size ;
  //## end ClFileUnit::GetFileDescriptorString%903715590.body
}

m4return_t ClFileUnit::SetFileDescriptorString (m4pchar_t ao_pbuffer, m4int32_t ao_size)
{
  //## begin ClFileUnit::SetFileDescriptorString%903961924.body preserve=yes
 
	m4pchar_t  FileName = NULL , auxcad = NULL, OnlyFileName = NULL;
	m4pchar_t no_filename_message = "no filename";
	m4pchar_t FilePath = NULL, local_extension = NULL, auxcad2;
	m4bool_t  exists = M4_TRUE;

#ifdef _UNIX 
	m4pchar_t Sep = "/";
#else 
	m4pchar_t Sep = "\\";
#endif
 
	ClFileManager* myFileManager = ClFileManager::Instance();
	ClFileName myManaFile;
	
	size_t		isize = strlen (M4_PDU_FILEUNIT_ID_STRING);
	size_t		ipos, auxint, size = 0;

	ao_pbuffer+= isize;
	auxcad = strstr ( ao_pbuffer , M4_PDU_FILEUNIT_SIZE_STRING );
	
	ipos = auxcad - ao_pbuffer;
	
	FilePath = GetFilePath();

	if (m_pSourceFileName) 
	{
		free(m_pSourceFileName);
		m_pSourceFileName = NULL;
	}

	m_pSourceFileName = (m4pchar_t ) malloc (ipos + 1 );
	memcpy(m_pSourceFileName,ao_pbuffer,ipos);
	m_pSourceFileName[ipos] = 0;	
	
	ao_pbuffer+= ipos;
	ao_pbuffer+= strlen(M4_PDU_FILEUNIT_SIZE_STRING);
	SetTotalSize (atoi(ao_pbuffer));

	if (m_uiTotalSize==M4PDUS_FILEUNIT_UNDEFINED_FILE) 
	{
		if (m_pFileDescriptor!=NULL) 
			fclose(m_pFileDescriptor);

		auxint = strlen( no_filename_message ) +1;
		m_pFileName = (m4pchar_t) malloc( auxint );
		memcpy (m_pFileName,no_filename_message,auxint);
		m_pFileDescriptor = NULL;
		m_uiFileIterator = 0;
		SetSerialState(M4_TRUE);
		return M4_SUCCESS;	
	}
	
	SetFile(m_pSourceFileName);
	
	// busco la extension.

	auxcad2 = strrchr (m_pSourceFileName, '.');
	if (auxcad2) 
	{
		auxcad2++;
		size = strlen ( auxcad2 ) + 1 ;
		local_extension = (m4pchar_t) malloc ( size );
		memcpy(local_extension, auxcad2, size );
	}
	
	myFileManager -> CreateTempFile(myManaFile, "~pdu", local_extension, NULL);

	if (m_pFileName != NULL ) 
	{
		// nos desinteresamos
		
		ClFileManager* myFileManager = ClFileManager::Instance();
		myFileManager -> UnSetInterestInFile(m_pFileName);

		free ( m_pFileName );
		m_pFileName = NULL;
	}

	m_pFileName = (m4pchar_t ) malloc (myManaFile.GetLength() + 1);
	myManaFile.GetFileName(m_pFileName);

	if (local_extension) 
		free (local_extension);
	if (OnlyFileName) 
		free (OnlyFileName);	
/*
	ao_pbuffer+= ipos;
	ao_pbuffer+= strlen(M4_PDU_FILEUNIT_SIZE_STRING);
	SetTotalSize (atoi(ao_pbuffer));
*/
	SetSerialState((1>m_uiTotalSize)?M4_TRUE:M4_FALSE);
	if (GetSerialState()) 
	{
		if (m_pFileDescriptor!=NULL) 
			fclose(m_pFileDescriptor);

		m_pFileDescriptor = NULL;
		m_uiFileIterator = 0;
	}
	
	return M4_SUCCESS;
	
  //## end ClFileUnit::SetFileDescriptorString%903961924.body
}

m4int32_t ClFileUnit::GetTotalSize ()
{
  //## begin ClFileUnit::GetTotalSize%903715591.body preserve=yes
	if(m_uiTotalSize==M4PDUS_FILEUNIT_UNDEFINED_SIZE)
		m_uiTotalSize=GetFileSize();
	return m_uiTotalSize;
  //## end ClFileUnit::GetTotalSize%903715591.body
}

m4return_t ClFileUnit::SetTotalSize (m4uint32_t ai_ulsize)
{
  //## begin ClFileUnit::SetTotalSize%903961925.body preserve=yes
	m_uiTotalSize = ai_ulsize;
	return M4_SUCCESS;
  //## end ClFileUnit::SetTotalSize%903961925.body
}

void ClFileUnit::GetRandomName (m4pchar_t &ao_RandomName)
{
  //## begin ClFileUnit::GetRandomName%904320068.body preserve=yes
  
	m4int16_t RandomSize = 7,i=0;

	if (ao_RandomName) 
	{
		free(ao_RandomName);
		ao_RandomName = NULL;
	}

	ao_RandomName = (m4pchar_t) malloc(RandomSize + 1);
	while (i<RandomSize) 
	{
		ao_RandomName[i] = 97 + (rand()%25);
		i++;
	}
	
	ao_RandomName[i] = 0;
  //## end ClFileUnit::GetRandomName%904320068.body
}

m4bool_t ClFileUnit::IsCompressed ()
{
  //## begin ClFileUnit::IsCompressed%932487949.body preserve=yes
	return m_bCompressed;
  //## end ClFileUnit::IsCompressed%932487949.body
}

void ClFileUnit::SetCompressed ()
{
  //## begin ClFileUnit::SetCompressed%932487950.body preserve=yes
	m_bCompressed=M4_TRUE;
  //## end ClFileUnit::SetCompressed%932487950.body
}

m4return_t ClFileUnit::InitializeSerialState ()
{
  //## begin ClFileUnit::InitializeSerialState%940592257.body preserve=yes

    m_pFileDescriptor = NULL;
	m_uiFileIterator = 0;
	m_uiTotalSize = 0;
	SetSerialState(M4_FALSE);
	m_uiTotalSize = M4PDUS_FILEUNIT_UNDEFINED_SIZE;	
		
	return M4_ERROR;
	
  //## end ClFileUnit::InitializeSerialState%940592257.body
}

m4uint_t ClFileUnit::GetFileDescriptorStringSize ()
{
  //## begin ClFileUnit::GetFileDescriptorStringSize%970483020.body preserve=yes
	m4char_t auxcad[32];
	if(M4PDUS_FILEUNIT_UNDEFINED_SIZE==m_uiTotalSize)
		m_uiTotalSize=GetFileSize();
	sprintf( auxcad,"%d", m_uiTotalSize );
	m4pchar_t pcOnlyFileName=GetOnlyFileName();
	if(NULL==pcOnlyFileName)
		return (m4uint_t) (strlen(M4_PDU_FILEUNIT_ID_STRING) + strlen(M4_PDU_FILEUNIT_SIZE_STRING) + strlen(auxcad) + 1);
	else
		return (m4uint_t) (strlen(M4_PDU_FILEUNIT_ID_STRING) + strlen(pcOnlyFileName) +  strlen(M4_PDU_FILEUNIT_SIZE_STRING) + strlen(auxcad) + 1);
  //## end ClFileUnit::GetFileDescriptorStringSize%970483020.body
}

m4uint_t ClFileUnit::GetFileSize ()
{
  //## begin ClFileUnit::GetFileSize%970483021.body preserve=yes
	m4uint_t size;
	fpos_t pos;
	int iLength = -1 ;
	m4pchar_t pcANSIFile = NULL ;

	if (m_pFileDescriptor==NULL) 
	{
		iLength = -1 ;
		pcANSIFile = M4CppToANSI( m_pFileName, iLength ) ;
		m_pFileDescriptor = fopen( pcANSIFile, "rb" ) ;
		delete( pcANSIFile ) ;

		if (m_pFileDescriptor == NULL ) 
		{
			SetSerialState(M4_TRUE);
			return M4PDUS_FILEUNIT_UNDEFINED_FILE; 
		}
	}
	fseek ( m_pFileDescriptor, 0 , SEEK_END);
	fgetpos ( m_pFileDescriptor , &pos) ;

	/* Bug 0098932
	Hay que cerrar el fichero y abrirlo después
	fseek ( m_pFileDescriptor, 0 , SEEK_SET);
	*/
	fclose( m_pFileDescriptor ) ;
	m_pFileDescriptor = NULL ;

	size=(m4int32_t)pos;
	if(0==size)
		SetSerialState(M4_TRUE);		// No tenemos nada que serializar
	else
		SetSerialState(M4_FALSE);

	return size;
  //## end ClFileUnit::GetFileSize%970483021.body
}

m4uint32_t ClFileUnit::GetChunkSize ()
{
  //## begin ClFileUnit::GetChunkSize%970483023.body preserve=yes
	/*if(m_bGetFileNameStringSize)
	{
		m_bGetFileNameStringSize=M4_FALSE;
		return GetFileDescriptorStringSize();
	}
	else*/
		return m_ulSize;

  //## end ClFileUnit::GetChunkSize%970483023.body
}

m4uint32_t ClFileUnit::GetDefragSize ()
{
  //## begin ClFileUnit::GetDefragSize%970563312.body preserve=yes
	return m_uiDefragSize;
  //## end ClFileUnit::GetDefragSize%970563312.body
}

// Additional Declarations
  //## begin ClFileUnit%35D8607101BD.declarations preserve=yes
  //## end ClFileUnit%35D8607101BD.declarations


ClCheckUnit::ClCheckUnit()
{
	m4uint16_t	iLen;

	iLen = (m4uint16_t) strlen( M4_PDU_CHECKUNIT_ID_STRING );

	SetBufferSize( iLen );
	memcpy( m_pBuffer, M4_PDU_CHECKUNIT_ID_STRING, iLen );
}

ClCheckUnit::~ClCheckUnit()
{
}

m4int16_t ClCheckUnit::GetType ()
{
	return M4_CHECK_UNIT;
}

m4uint32_t ClCheckUnit::GetSize()
{
	return m_ulSize;
}

m4pchar_t ClCheckUnit::GetBuffer()
{
	return m_pBuffer;
}

m4return_t ClCheckUnit::SetSize (m4uint32_t ai_uiSize)
{
 	return SetBufferSize( ai_uiSize );
}

//## begin module%344E313502C1.epilog preserve=yes
//## end module%344E313502C1.epilog
