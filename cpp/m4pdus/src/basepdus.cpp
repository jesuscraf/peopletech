//## begin module%342BBAB6024A.cm preserve=no
//## end module%342BBAB6024A.cm

//## begin module%342BBAB6024A.cp preserve=no
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
//## end module%342BBAB6024A.cp

//## Module: basepdu%342BBAB6024A; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\basepdus.cpp

//## begin module%342BBAB6024A.additionalIncludes preserve=no
//## end module%342BBAB6024A.additionalIncludes

//## begin module%342BBAB6024A.includes preserve=yes
#include <m4trace.hpp>
//## end module%342BBAB6024A.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// pduserializer
#include <pduserializer.hpp>
// pdutools
#include <pdutools.hpp>
// DataStorages
#include <datastorages.hpp>
// basepdu
#include <basepdus.hpp>


//## begin module%342BBAB6024A.declarations preserve=no
//## end module%342BBAB6024A.declarations

//## begin module%342BBAB6024A.additionalDeclarations preserve=yes
m4pcchar_t M4_PDU_LOG_LABEL="#*#*#*#*LOG_INFO*#*#*#*#";
const m4int_t  M4_PDU_LOG_LABEL_SIZE=24;

m4pcchar_t M4_PDU_CREDENTIAL_LABEL="#*#*#*#*CREDENTIAL*#*#*#*#";
const m4int_t  M4_PDU_CREDENTIAL_LABEL_SIZE=26;

m4pcchar_t M4_PDU_CERTIFICATE_LABEL="#*#*#*#*CERTIFICATE*#*#*#*#";
const m4int_t  M4_PDU_CERTIFICATE_LABEL_SIZE=27;

m4pcchar_t M4_PDU_SESSCONTEXT_LABEL="#*#*#*#*SESSCONTEXT*#*#*#*#";
const m4int_t  M4_PDU_SESSCONTEXT_LABEL_SIZE=27;
//## end module%342BBAB6024A.additionalDeclarations


// Class ClPDUWithData 







ClPDUWithData::ClPDUWithData (m4int_t ai_iVersion)
  //## begin ClPDUWithData::ClPDUWithData%880389200.hasinit preserve=no
      : m_ulNBlocks(0), m_bDataStorageOwner(M4_TRUE), m_bIsExtDataInDataStorage(M4_FALSE)
  //## end ClPDUWithData::ClPDUWithData%880389200.hasinit
  //## begin ClPDUWithData::ClPDUWithData%880389200.initialization preserve=yes
  ,ClPDU(ai_iVersion)
  //## end ClPDUWithData::ClPDUWithData%880389200.initialization
{
  //## begin ClPDUWithData::ClPDUWithData%880389200.body preserve=yes
	m_poDataStorage = M4DataStorage::GetNewDataStorage();
	m_poBlockFormats = new ClBlockFormats() ;

	m_szCertificate = NULL;
	m_szSessContext = NULL ;
	m_iCertificateLen = 0 ;
	m_iSessContextLen = 0 ;

  //## end ClPDUWithData::ClPDUWithData%880389200.body
}


ClPDUWithData::~ClPDUWithData ()
{
  //## begin ClPDUWithData::~ClPDUWithData%880389201.body preserve=yes
	if ( m_bDataStorageOwner == M4_TRUE )
	{
		if ( m_poDataStorage != NULL )
		{
			M4DataStorage::DeleteDataStorage( m_poDataStorage );
			m_poDataStorage = NULL;
		}
	}
	else
		m_poDataStorage = NULL;

	if ( m_poBlockFormats != NULL )
		delete m_poBlockFormats;

  //## end ClPDUWithData::~ClPDUWithData%880389201.body
}



//## Other Operations (implementation)
void ClPDUWithData::SetNBlocks (m4uint32_t ai_ulNBlocks)
{
  //## begin ClPDUWithData::SetNBlocks%880389202.body preserve=yes
	m_ulNBlocks = ai_ulNBlocks;
  //## end ClPDUWithData::SetNBlocks%880389202.body
}

m4uint32_t ClPDUWithData::GetNBlocks ()
{
  //## begin ClPDUWithData::GetNBlocks%880389203.body preserve=yes
	return m_ulNBlocks;
  //## end ClPDUWithData::GetNBlocks%880389203.body
}

m4return_t ClPDUWithData::WAddDataBlock (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted)
{
  //## begin ClPDUWithData::WAddDataBlock%880389204.body preserve=yes
	// incremento la variable que controla el nº de bloques.
	m_ulNBlocks ++;
	
	m_poBlockFormats -> AddFormat( ai_ulSize, ai_bEncrypted );
	return ( m_poDataStorage -> Add( ai_poDataBlock, ai_ulSize, ai_bEncrypted ) );
  //## end ClPDUWithData::WAddDataBlock%880389204.body
}

m4return_t ClPDUWithData::AddFormatBlocks (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize)
{
  //## begin ClPDUWithData::AddFormatBlocks%880451466.body preserve=yes
	m4uint32_t	ulCont ,ulBlockSize;

	for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
	{
		ulBlockSize = m_oSerializer.GetUInt32 ();
		m_poBlockFormats -> AddFormat( ulBlockSize );
	}

	if ( ai_poDataBlock != NULL )
		delete []ai_poDataBlock;	

	return M4_SUCCESS;
  //## end ClPDUWithData::AddFormatBlocks%880451466.body
}

m4return_t ClPDUWithData::GetDataBlock1 (m4uint32_t ai_ulBlock, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4bool_t ao_bEncrypted, m4uint8_t ai_usMode)
{
  //## begin ClPDUWithData::GetDataBlock1%880389205.body preserve=yes
	M4_ASSERT(1==2);
	if ( ai_usMode != M4_PDUBLOCKMODE_REFERENCE )
		return M4_ERROR;
	return (m_poDataStorage -> Get( ao_pBlock, ao_ulBlockSize
				, ao_bEncrypted, ai_ulBlock ) );
  //## end ClPDUWithData::GetDataBlock1%880389205.body
}

m4return_t ClPDUWithData::GetDataBlock1 (m4uint32_t ai_ulBlock, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4uint8_t ai_usMode)
{
  //## begin ClPDUWithData::GetDataBlock1%880389206.body preserve=yes
	M4_ASSERT(1==2);
	m4bool_t bEncrypted;
	if ( ai_usMode != M4_PDUBLOCKMODE_REFERENCE )
		return M4_ERROR;
	return (m_poDataStorage -> Get( ao_pBlock, ao_ulBlockSize
			, bEncrypted, ai_ulBlock ) );
  //## end ClPDUWithData::GetDataBlock1%880389206.body
}

m4return_t ClPDUWithData::GetNextDataBlock (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClPDUWithData::GetNextDataBlock%880389207.body preserve=yes
	m4bool_t	bEncrypted;
	return m_poDataStorage -> GetNext( ao_pBlock, ao_ulBlockSize, bEncrypted );
  //## end ClPDUWithData::GetNextDataBlock%880389207.body
}

m4return_t ClPDUWithData::GetNextDataBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClPDUWithData::GetNextDataBlock%880389208.body preserve=yes
	m4bool_t	bEncrypted = M4_FALSE;
	return ( m_poDataStorage -> GetNext( ai_ulReadSize, ao_pBlock, ao_ulBlockSize, bEncrypted ) );
  //## end ClPDUWithData::GetNextDataBlock%880389208.body
}

m4uint32_t ClPDUWithData::GetDataSize ()
{
  //## begin ClPDUWithData::GetDataSize%880389209.body preserve=yes
    m4return_t retCode = M4_ERROR;
	
    //marcoscp
    if(m_poDataStorage->NewData() || !m_bIsExtDataInDataStorage)
	{
        retCode = ExtData2DataStorage();
	}   

    m4uint32_t  ulTotalSize = m_poDataStorage->Size()+m_poBlockFormats -> GetFormatsSize();
	return ulTotalSize;	
  //## end ClPDUWithData::GetDataSize%880389209.body
}

void ClPDUWithData::Clear ()
{
  //## begin ClPDUWithData::Clear%880389211.body preserve=yes
	m_poDataStorage -> Clear();
	m_poBlockFormats -> Clear();
  //## end ClPDUWithData::Clear%880389211.body
}

void ClPDUWithData::Dump (m4pchar_t &ao_pTexto)
{
  //## begin ClPDUWithData::Dump%880389212.body preserve=yes
	
   m4char_t	    aData[32];
   m4uint32_t   tam=0,usCont=0,nblock;
  // m4bool_t     isEnc;
  // m4pchar_t    Datos; 
   

 //  ao_pTexto +=" \n Atributos de ClPDUWithDataPDU. \n\n Atributos. ";
   
   strcat (ao_pTexto ,"\n m_ulNBlocks. ");
   sprintf( aData, "%lu", m_ulNBlocks );
   strcat (ao_pTexto , aData );
   m4pchar_t  paux = NULL;
   
   nblock = GetNBlocks();
   InitializeIteration();

   m_poDataStorage -> Dump(ao_pTexto);

	
	InitializeIteration();

  //## end ClPDUWithData::Dump%880389212.body
}

void ClPDUWithData::InitializeIteration ()
{
  //## begin ClPDUWithData::InitializeIteration%880389213.body preserve=yes
	m_poBlockFormats -> InitializeIteration();
	m_poDataStorage -> InitializeIteration();
  //## end ClPDUWithData::InitializeIteration%880389213.body
}

void ClPDUWithData::InitializeReadIteration ()
{
  //## begin ClPDUWithData::InitializeReadIteration%880389214.body preserve=yes
	m_poBlockFormats -> InitializeIteration();
	m_poDataStorage -> InitializeIteration();
  //## end ClPDUWithData::InitializeReadIteration%880389214.body
}

M4DataStorage * ClPDUWithData::WGetDataStorage (m4bool_t ai_bOwner)
{
  //## begin ClPDUWithData::WGetDataStorage%884343826.body preserve=yes
	m_bDataStorageOwner = ai_bOwner;
	return m_poDataStorage;
  //## end ClPDUWithData::WGetDataStorage%884343826.body
}

void ClPDUWithData::WSetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner)
{
  //## begin ClPDUWithData::WSetDataStorage%884343827.body preserve=yes
	if ( m_bDataStorageOwner == M4_TRUE )
	{
		if ( m_poDataStorage != NULL )
		{
			M4DataStorage::DeleteDataStorage( m_poDataStorage );
			m_poDataStorage = NULL;
		}
	}
	m_bDataStorageOwner = ai_bOwner;
	m_poDataStorage = ai_poDataStorage;

	m_poDataStorage->NewData();
	ActualizeBlockFormats();

  //## end ClPDUWithData::WSetDataStorage%884343827.body
}

m4return_t ClPDUWithData::WAddDataUnit (ClDataUnit *ai_pDataUnit)
{
  //## begin ClPDUWithData::WAddDataUnit%885978330.body preserve=yes
	return ( m_poDataStorage -> Add( ai_pDataUnit ) );
  //## end ClPDUWithData::WAddDataUnit%885978330.body
}

m4return_t ClPDUWithData::ActualizeBlockFormats ()
{
  //## begin ClPDUWithData::ActualizeBlockFormats%886605281.body preserve=yes

	m_poBlockFormats -> Clear();

	ClDataUnitInterface	*pDataUnit = NULL;
	m_poDataStorage -> Size();
	m_poDataStorage -> NewData ();
	m_poDataStorage -> InitializeIteration();

	m4return_t	test;

	m4uint32_t	uiTotalFileSize=0,
				uiDefragSize=0,
				uiAcumulado=0,
				uiChunkSize=0,
				uiResto=0;
	ClFileUnit *pFU=NULL;
	m4bool_t bHeader=M4_TRUE;

	while( M4_ERROR!=(test = m_poDataStorage -> GetNext( pDataUnit)))
	{
		if( pDataUnit->GetType()==M4_DATA_UNIT || pDataUnit->GetType()==M4_CHECK_UNIT )
			m_poBlockFormats -> AddFormat(	pDataUnit->GetSize(), pDataUnit->GetCryptState());
		else
		{
			if(bHeader)
			{
				pFU=(ClFileUnit *)pDataUnit;
				m_poBlockFormats -> AddFormat(	pFU->GetFileDescriptorStringSize(), pFU->GetCryptState());
				uiDefragSize=pFU->GetDefragSize();
				uiTotalFileSize=pFU->GetFileSize();
				uiAcumulado=0;
				if(uiTotalFileSize==M4PDUS_FILEUNIT_UNDEFINED_FILE)
					uiTotalFileSize=0;
				if(uiTotalFileSize<=uiDefragSize)
				{
					pFU->SetSerialState(M4_TRUE);
					if(0==uiTotalFileSize)
						bHeader=M4_TRUE;
					else
						bHeader=M4_FALSE;
				}
				else
				{
					pFU->SetSerialState(M4_FALSE);
					bHeader=M4_FALSE;
				}
			}
			else
			{
				uiResto=uiTotalFileSize-uiAcumulado;
				if(uiResto>uiDefragSize)
				{
					uiChunkSize=uiDefragSize;
				}
				else
				{
					uiChunkSize=uiResto;
				}
				uiAcumulado+=uiChunkSize;

				if(uiTotalFileSize-uiAcumulado<=uiDefragSize)
					pFU->SetSerialState(M4_TRUE);

				if(uiTotalFileSize==uiAcumulado)
				{
					bHeader=M4_TRUE;
					pFU->SetSerialState(M4_FALSE);
				}

				m_poBlockFormats -> AddFormat(	uiChunkSize, pFU->GetCryptState());
			}
		}
	}

	m_ulNBlocks = m_poBlockFormats -> GetFormatsSize()/4;		// Aunque tengamos 1 bloque para serializarlo se puede dividir en varios
	m_poDataStorage -> InitializeIteration();
	return M4_SUCCESS;
  //## end ClPDUWithData::ActualizeBlockFormats%886605281.body
}

void ClPDUWithData::SetTmpPath (m4pchar_t ai_pTempPath)
{
  //## begin ClPDUWithData::SetTempPath%904051590.body preserve=yes
	m_poDataStorage -> SetTmpPath(ai_pTempPath);
  //## end ClPDUWithData::SetTempPath%904051590.body
}

m4pchar_t ClPDUWithData::GetTmpPath ()
{
  //## begin ClPDUWithData::GetTempPath%904051591.body preserve=yes
	return m_poDataStorage -> GetTmpPath();
  //## end ClPDUWithData::GetTempPath%904051591.body
}

m4return_t ClPDUWithData::SerializeFormatsBlock ()
{
  //## begin ClPDUWithData::SerializeFormatsBlock%963407995.body preserve=yes
	m4uint32_t	ulFormatf = 0;
	m_poBlockFormats -> InitializeIteration();
    while ( m_poBlockFormats -> GetNext(ulFormatf) != M4_ERROR) 
	   m_oSerializer.AddUInt32( ulFormatf );

	return M4_SUCCESS;
  //## end ClPDUWithData::SerializeFormatsBlock%963407995.body
}

M4DataStorage * ClPDUWithData::GetDataStorage (m4bool_t ai_bOwner)
{
  //## begin ClPDUWithData::GetDataStorage%969437745.body preserve=yes
	return WGetDataStorage( ai_bOwner );
  //## end ClPDUWithData::GetDataStorage%969437745.body
}

void ClPDUWithData::SetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner)
{
  //## begin ClPDUWithData::SetDataStorage%969437746.body preserve=yes
	WSetDataStorage( ai_poDataStorage, ai_bOwner );
  //## end ClPDUWithData::SetDataStorage%969437746.body
}

m4uint32_t ClPDUWithData::GetPDUSize ()
{
  //## begin ClPDUWithData::GetPDUSize%970563313.body preserve=yes
	m_ulSize = GetHeadSize () + GetDataSize();
	return m_ulSize;

	// this comments fix bug 0075340
//	if(m_bIsExtDataInDataStorage)
//	{
//		m_ulSize = GetHeadSize () + GetDataSize();
//	}
//	else
//	{
//		m_ulSize = GetHeadSize () + GetDataSize();
//		if (LogInfo()) 
//			m_ulSize += M4_PDU_LOG_LABEL_SIZE + m_oLogInformation.m_lSize+ 4 /* Por el nuevo BlockFormat*/;
//		if(!m_strCredential.empty())
//			m_ulSize += M4_PDU_CREDENTIAL_LABEL_SIZE + m_strCredential.size() + 1 /* Le ponemos un \0 para facilitar luego */ +4/* Por el nuevo BlockFormat*/;
//		if(m_szCertificate)
//			m_ulSize += M4_PDU_CERTIFICATE_LABEL_SIZE + m_iCertificateLen + 1 /* Le ponemos un \0 para facilitar luego */ +4/* Por el nuevo BlockFormat*/;
//		if(m_szSessContext)
//			m_ulSize += M4_PDU_SESSCONTEXT_LABEL_SIZE + m_iSessContextLen + 1 /* Le ponemos un \0 para facilitar luego */ +4/* Por el nuevo BlockFormat*/;
//	}
  //## end ClPDUWithData::GetPDUSize%970563313.body
}

m4return_t ClPDUWithData::GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClPDUWithData::GetNextBlock%970646985.body preserve=yes

	m4return_t iRet;
	switch (m_uiState)
	{
	case M4_PDUSTATE_HEAD1:
		if(m_iVersion==M4_PDU_40_VERSION)
		{
			if(!m_bIsExtDataInDataStorage)
			{
				iRet = ExtData2DataStorage();
			}
		}
		else
		{
			if(m_poDataStorage->NewData())
			{
				ActualizeBlockFormats();
			}
		}

		if ( m_pBinaryHead != NULL )
		{
			delete []m_pBinaryHead;
		}
	
		if ( GetBinaryHead( ao_pBlock, ao_ulBlockSize ) == M4_ERROR )
		{
			ao_pBlock = NULL;
			ao_ulBlockSize = 0;
			return M4_ERROR;
		}

		InitializeIteration();
		m_uiState = M4_PDUSTATE_HEADFULL;		
		return M4_SUCCESS;
	break;
	case M4_PDUSTATE_HEADFULL:
		iRet=GetNextDataBlock( 0, ao_pBlock, ao_ulBlockSize );
		if(M4_ERROR==iRet)
		{
			DataStorage2ExtData();
			m_uiState = M4_PDUSTATE_HEAD1;
			if(!m_poDataStorage->m_bFinishSerialization)
			{
				m_poDataStorage->m_bFinishSerialization=M4_TRUE;
				//m_poDataStorage->InitializeSerialState();
			}
		}
		return iRet;
	break;
	default:
		m4Trace(cerr <<"Unkown state " << m_uiState<<endl);
		return M4_ERROR;
	break;
	}
  //## end ClPDUWithData::GetNextBlock%970646985.body
}

m4return_t ClPDUWithData::SetCredential (m4string_t &ai_strCredential)
{
  //## begin ClPDUWithData::SetCredential%972376844.body preserve=yes
	m_strCredential=ai_strCredential;
	return M4_SUCCESS;
  //## end ClPDUWithData::SetCredential%972376844.body
}

m4return_t ClPDUWithData::GetCredential (m4string_t &ao_strCredential)
{
  //## begin ClPDUWithData::GetCredential%972376845.body preserve=yes
	if(m_strCredential.empty())
		return M4_ERROR;
	else
	{
		ao_strCredential=m_strCredential;
		return M4_SUCCESS;
	}
  //## end ClPDUWithData::GetCredential%972376845.body
}

m4return_t ClPDUWithData::SerializeCredential ()
{
  //## begin ClPDUWithData::SerializeCredential%972376848.body preserve=yes
	if(!m_strCredential.empty())
	{
		m4uint_t uiCredentialSize = (m4uint_t) m_strCredential.size();
		if(NULL==m_poDataStorage )
		{
			m_poDataStorage=M4DataStorage::GetNewDataStorage();
            if (NULL == m_poDataStorage)
            {
                return M4_ERROR;
            }
			SetDataStorage(m_poDataStorage);
		}
		ClDataUnit * poDU;
		m_poDataStorage->GetNewDataUnit(poDU);
		if(NULL==poDU)
		{
			m4Trace(cerr<<"Null DataUnit"<<endl);
			return M4_ERROR;
		}
		poDU->SetSize(uiCredentialSize+M4_PDU_CREDENTIAL_LABEL_SIZE+1);
		m4pchar_t pcPUBuffer=poDU->GetBuffer();
		if(NULL==pcPUBuffer)
		{
			m4Trace(cerr<<"Null Buffer"<<endl);
			return M4_ERROR;
		}
		strcpy(pcPUBuffer,M4_PDU_CREDENTIAL_LABEL);
		strcat(pcPUBuffer,m_strCredential.c_str());
		return M4_SUCCESS;
	}
	else
		return M4_WARNING;


  //## end ClPDUWithData::SerializeCredential%972376848.body
}

m4return_t ClPDUWithData::GetCertificate (m4pchar_t &ao_pCertificate , m4uint32_t & ao_iCertificateLen)
{
	if(! m_szCertificate)
		return M4_ERROR;
	else
	{
		ao_pCertificate=new m4char_t [m_iCertificateLen] ;
		memcpy ( ao_pCertificate, m_szCertificate , m_iCertificateLen ) ;
		ao_iCertificateLen = m_iCertificateLen ;
		return M4_SUCCESS;
	}
}

m4return_t ClPDUWithData::SetCertificate (m4pchar_t ai_pCertificate , m4uint32_t ai_iCertificateLen)
{
	m4uint32_t iTerminator = 0 ;
	if ( m_szCertificate )
	{
		delete [] m_szCertificate ;
	}

	if ( ai_pCertificate [ai_iCertificateLen -1 ] )
		iTerminator = 1 ;

	m_szCertificate = new m4char_t [ ai_iCertificateLen + iTerminator ] ;
	memcpy ( m_szCertificate  , ai_pCertificate , ai_iCertificateLen ) ;

	m_iCertificateLen = ai_iCertificateLen + iTerminator ;

	return M4_SUCCESS;
}


m4return_t ClPDUWithData::SetSessContext (m4pchar_t ai_pSessContex , m4uint32_t ai_iSessContextLen)
{
	if ( m_szSessContext )
	{
		delete [] m_szCertificate ;
	}
	m_szSessContext = new m4char_t [ ai_iSessContextLen ] ;
	memcpy ( m_szSessContext  , ai_pSessContex , ai_iSessContextLen ) ;

	m_iSessContextLen = ai_iSessContextLen ;

	return M4_SUCCESS;
}

m4return_t ClPDUWithData::GetSessContext (m4pchar_t & ao_pSessContex , m4uint32_t & ao_iSessContextLen)
{
	if(! m_szSessContext)
		return M4_ERROR;
	else
	{
		ao_pSessContex=new m4char_t [m_iSessContextLen] ;
		memcpy ( ao_pSessContex, m_szSessContext , m_iSessContextLen ) ;
		ao_iSessContextLen = m_iSessContextLen ;
		return M4_SUCCESS;
	}

}

m4return_t ClPDUWithData::SerializeCertificate ()
{
	if(m_szCertificate )
	{
		if(NULL==m_poDataStorage )
		{
			m_poDataStorage=M4DataStorage::GetNewDataStorage();
			SetDataStorage(m_poDataStorage);
		}
		ClDataUnit * poDU;
		m_poDataStorage->GetNewDataUnit(poDU);
		if(NULL==poDU)
		{
			m4Trace(cerr<<"Null DataUnit"<<endl);
			return M4_ERROR;
		}
		poDU->SetSize(m_iCertificateLen + M4_PDU_CERTIFICATE_LABEL_SIZE );
		m4pchar_t pcPUBuffer=poDU->GetBuffer();
		if(NULL==pcPUBuffer)
		{
			m4Trace(cerr<<"Null Buffer"<<endl);
			return M4_ERROR;
		}
		strcpy(pcPUBuffer,M4_PDU_CERTIFICATE_LABEL);
		pcPUBuffer += M4_PDU_CERTIFICATE_LABEL_SIZE ;
		memcpy(pcPUBuffer,m_szCertificate, m_iCertificateLen-1);
		pcPUBuffer [m_iCertificateLen-1] = 0 ;
		return M4_SUCCESS;
	}
	else
		return M4_WARNING;

}


m4return_t ClPDUWithData::SerializeSessContext ()
{
	if(m_szSessContext )
	{
		if(NULL==m_poDataStorage )
		{
			m_poDataStorage=M4DataStorage::GetNewDataStorage();
			SetDataStorage(m_poDataStorage);
		}
		ClDataUnit * poDU;
		m_poDataStorage->GetNewDataUnit(poDU);
		if(NULL==poDU)
		{
			m4Trace(cerr<<"Null DataUnit"<<endl);
			return M4_ERROR;
		}
		poDU->SetSize(m_iSessContextLen + M4_PDU_SESSCONTEXT_LABEL_SIZE + 1);
		m4pchar_t pcPUBuffer=poDU->GetBuffer();
		if(NULL==pcPUBuffer)
		{
			m4Trace(cerr<<"Null Buffer"<<endl);
			return M4_ERROR;
		}
		strcpy(pcPUBuffer,M4_PDU_SESSCONTEXT_LABEL);
		pcPUBuffer += M4_PDU_SESSCONTEXT_LABEL_SIZE ;
		memcpy(pcPUBuffer,m_szSessContext, m_iSessContextLen);
		pcPUBuffer [m_iSessContextLen] = 0 ;
		return M4_SUCCESS;
	}
	else
		return M4_WARNING;

}




m4return_t ClPDUWithData::SerializeLog ()
{
  //## begin ClPDUWithData::SerializeLog%972376849.body preserve=yes
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogSize;
	m4return_t iRet=GetLogInfo(pcLogBuffer,uiLogSize);
	
	if((M4_ERROR!=iRet) && (uiLogSize>0))
	{
		if(NULL==m_poDataStorage )
		{
			m_poDataStorage=M4DataStorage::GetNewDataStorage();
			SetDataStorage(m_poDataStorage);
		}
		ClDataUnit * poDU;
		m_poDataStorage->GetNewDataUnit(poDU);
		if(NULL==poDU)
		{
			m4Trace(cerr<<"Null DataUnit"<<endl);
			return M4_ERROR;
		}
		poDU->SetSize(uiLogSize+M4_PDU_LOG_LABEL_SIZE);
		m4pchar_t pcPUBuffer=poDU->GetBuffer();
		if(NULL==pcPUBuffer)
		{
			m4Trace(cerr<<"Null Buffer"<<endl);
			return M4_ERROR;
		}
		memcpy(pcPUBuffer,M4_PDU_LOG_LABEL,M4_PDU_LOG_LABEL_SIZE);
		memcpy(pcPUBuffer+M4_PDU_LOG_LABEL_SIZE,pcLogBuffer,uiLogSize);
		SetLogInfo(NULL,0);
		DisableLogInfo();
		return M4_SUCCESS;
	}
	else
		return M4_WARNING;
	
  //## end ClPDUWithData::SerializeLog%972376849.body
}

m4return_t ClPDUWithData::ExtractCredential ()
{
  //## begin ClPDUWithData::ExtractCredential%972395359.body preserve=yes
	m4pchar_t pcCredential;
	m4uint32_t uiCredentialSize;

	m4uint32_t uiDataUnit=0;
	
	if(NULL!=m_poDataStorage )
	{
		uiDataUnit=m_poDataStorage->Blocks();
		if(uiDataUnit)
		{
			uiDataUnit--;	// Si tenemos N la ultima sera la N-1
			m4bool_t bAux;
			m_poDataStorage->Get (pcCredential, uiCredentialSize, bAux, uiDataUnit);
			if(pcCredential && (uiCredentialSize>M4_PDU_CREDENTIAL_LABEL_SIZE) && (!strncmp(pcCredential,M4_PDU_CREDENTIAL_LABEL,M4_PDU_CREDENTIAL_LABEL_SIZE)))
			{
				m_strCredential=pcCredential+M4_PDU_CREDENTIAL_LABEL_SIZE;
				m_poDataStorage->Remove(uiDataUnit);
			}
		}
	}

	return M4_SUCCESS;
  //## end ClPDUWithData::ExtractCredential%972395359.body
}

m4return_t ClPDUWithData::ExtractCertificate ()
{
	m4pchar_t pcCertificate;
	m4uint32_t uiCertificateSize;

	m4uint32_t uiDataUnit=0;
	
	if(NULL!=m_poDataStorage )
	{
		uiDataUnit=m_poDataStorage->Blocks();
		if(uiDataUnit)
		{

			uiDataUnit--;	// Si tenemos N la ultima sera la N-1
			m4bool_t bAux;
			m_poDataStorage->Get (pcCertificate, uiCertificateSize, bAux, uiDataUnit);
			if(pcCertificate && (uiCertificateSize>M4_PDU_CERTIFICATE_LABEL_SIZE) && (!strncmp(pcCertificate,M4_PDU_CERTIFICATE_LABEL,M4_PDU_CERTIFICATE_LABEL_SIZE)))
			{
				m_iCertificateLen = uiCertificateSize - M4_PDU_CERTIFICATE_LABEL_SIZE ;
				m_szCertificate= new m4char_t [ m_iCertificateLen + 1 ] ;
				memcpy ( m_szCertificate , pcCertificate+M4_PDU_CERTIFICATE_LABEL_SIZE , m_iCertificateLen ) ;
				m_poDataStorage->Remove(uiDataUnit);
			}
		}
	}

	return M4_SUCCESS;
}

m4return_t ClPDUWithData::ExtractSessContext ()
{
	m4pchar_t pcSessContext;
	m4uint32_t uiSessContextSize;

	m4uint32_t uiDataUnit=0;
	
	if(NULL!=m_poDataStorage )
	{
		uiDataUnit=m_poDataStorage->Blocks();
		if(uiDataUnit)
		{
			uiDataUnit--;	// Si tenemos N la ultima sera la N-1
			m4bool_t bAux;
			m_poDataStorage->Get (pcSessContext, uiSessContextSize, bAux, uiDataUnit);
			if(pcSessContext && (uiSessContextSize>M4_PDU_SESSCONTEXT_LABEL_SIZE) && (!strncmp(pcSessContext,M4_PDU_SESSCONTEXT_LABEL,M4_PDU_SESSCONTEXT_LABEL_SIZE)))
			{
				m_iSessContextLen = uiSessContextSize - M4_PDU_SESSCONTEXT_LABEL_SIZE ;
				m_szSessContext=new m4char_t [ m_iSessContextLen  +1 ] ;
				memcpy ( m_szSessContext , pcSessContext+M4_PDU_SESSCONTEXT_LABEL_SIZE , m_iSessContextLen ) ;
				m_poDataStorage->Remove(uiDataUnit);
			}
		}
	}

	return M4_SUCCESS;
}



m4return_t ClPDUWithData::ExtractLog ()
{
  //## begin ClPDUWithData::ExtractLog%972395360.body preserve=yes
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogInfoSize;

	m4uint32_t uiDataUnit=0;
	
	if(NULL!=m_poDataStorage )
	{
		uiDataUnit=m_poDataStorage->Blocks();
		if(uiDataUnit)
		{
			uiDataUnit--;	// Si tenemos N la ultima sera la N-1
			m4bool_t bAux;
			m_poDataStorage->Get (pcLogBuffer, uiLogInfoSize, bAux, uiDataUnit);
			if(pcLogBuffer && (uiLogInfoSize>M4_PDU_LOG_LABEL_SIZE) && (!strncmp(pcLogBuffer,M4_PDU_LOG_LABEL,M4_PDU_LOG_LABEL_SIZE)))
			{
				EnableLogInfo();
				SetLogInfo (pcLogBuffer+M4_PDU_LOG_LABEL_SIZE,uiLogInfoSize-M4_PDU_LOG_LABEL_SIZE);
				m_poDataStorage->Remove(uiDataUnit);
			}
		}
	}

	return M4_SUCCESS;
  //## end ClPDUWithData::ExtractLog%972395360.body
}

m4uint32_t ClPDUWithData::GetNextDataStorageChunkSize (m4pchar_t &ao_pBlock)
{
  //## begin ClPDUWithData::GetNextDataStorageChunkSize%972395361.body preserve=yes
	m4uint32_t	ulSize = 0;
	m4bool_t	bEncrypted = M4_FALSE;
	if(m_poDataStorage->m_bFinishSerialization)
	{
		m_poDataStorage->m_bFinishSerialization=M4_FALSE;
		m_poDataStorage->InitializeSerialState();
	}
	if ( m_poBlockFormats -> GetNextNInc( ulSize, bEncrypted ) == M4_ERROR )
	{
		DataStorage2ExtData();
		m_poDataStorage->m_bFinishSerialization=M4_TRUE;
		m_uiState = M4_PDUSTATE_HEAD1;
		return 0;
	}

	ao_pBlock = (m4pchar_t) malloc ( ulSize );
	return ulSize;
  //## end ClPDUWithData::GetNextDataStorageChunkSize%972395361.body
}

m4return_t ClPDUWithData::ExtData2DataStorage ()
{
  //## begin ClPDUWithData::ExtData2DataStorage%972460315.body preserve=yes
	m4return_t iRet=SerializeCredential();
	if(iRet==M4_ERROR)
	{
		m4Trace(cerr<<"Error serializing Credential"<<endl);
		return M4_ERROR;
	}

	iRet=SerializeCertificate();
	if(iRet==M4_ERROR)
	{
		m4Trace(cerr<<"Error serializing Certificate"<<endl);
		return M4_ERROR;
	}

	iRet=SerializeSessContext ();
	if(iRet==M4_ERROR)
	{
		m4Trace(cerr<<"Error serializing Credential"<<endl);
		return M4_ERROR;
	}

	iRet=SerializeLog();
	if(iRet==M4_ERROR)
	{
		m4Trace(cerr<<"Error serializing Log"<<endl);
		return M4_ERROR;
	}

	/* Bug 0106736
	Si llega aquí ya se ha ejecutado bien la función
	Además siempre se hace la actualización para que no se haga fuera
	*/
	m_poDataStorage->Size();
	ActualizeBlockFormats();
	m_bIsExtDataInDataStorage=M4_TRUE;

	return M4_SUCCESS;
  //## end ClPDUWithData::ExtData2DataStorage%972460315.body
}

m4return_t ClPDUWithData::DataStorage2ExtData ()
{
  //## begin ClPDUWithData::DataStorage2ExtData%972460316.body preserve=yes
//	if(m_iVersion<M4_PDU_40_VERSION)
//		return M4_SUCCESS;
	m4return_t iRet=ExtractLog();
	if(iRet!=M4_SUCCESS)
	{
		m4Trace(cerr<<"Error extracting Log"<<endl);
		return M4_ERROR;
	}
	iRet=ExtractSessContext();
	if(iRet!=M4_SUCCESS)
	{
		m4Trace(cerr<<"Error extracting Session Context"<<endl);
		return M4_ERROR;
	}

	iRet=ExtractCertificate();
	if(iRet!=M4_SUCCESS)
	{
		m4Trace(cerr<<"Error extracting Certificate"<<endl);
		return M4_ERROR;
	}

	iRet=ExtractCredential();
	if(iRet!=M4_SUCCESS)
	{
		m4Trace(cerr<<"Error extracting Credential"<<endl);
		return M4_ERROR;
	}

	m_poDataStorage->Size();
	ActualizeBlockFormats();
	m_bIsExtDataInDataStorage=M4_FALSE;
	return M4_SUCCESS;
  //## end ClPDUWithData::DataStorage2ExtData%972460316.body
}

// Class ClPDU 







//## begin ClPDU::m_oFile%34F6D750017F.attr preserve=no  protected: static fstream {VA} 
fstream ClPDU::m_oFile;

ClPDU::UserInfoCallBackType ClPDU::m_pfUserInfoCallBack = NULL;

//## end ClPDU::m_oFile%34F6D750017F.attr



void ClPDU::RegisterUserInfoCallBack(UserInfoCallBackType ai_pfCallBack)
{
	m_pfUserInfoCallBack = ai_pfCallBack;
}

void ClPDU::UnregisterUserInfoCallBack()
{
	m_pfUserInfoCallBack = NULL;
}




ClPDU::ClPDU (m4int_t ai_iVersion)
  //## begin ClPDU::ClPDU%875692541.hasinit preserve=no
      : m_uiId(0), m_ulSize(0), m_ulDeltaSize(0), m_ulTimeMark(0), m_uiState(M4_PDUSTATE_HEAD1), m_uiHeadSize(0), m_pBinaryHead(NULL), m_szDump(NULL), m_iVersion(M4_PDU_40_VERSION), m_ulRequestID(0), m_uiType(M4_TYPE_PDU_CONTROL)
  //## end ClPDU::ClPDU%875692541.hasinit
  //## begin ClPDU::ClPDU%875692541.initialization preserve=yes
  //## end ClPDU::ClPDU%875692541.initialization
{
  //## begin ClPDU::ClPDU%875692541.body preserve=yes
	 m_uiLogInfo = 0;     // Por defecto no hay informacion de debug
	 m_iHasOverwriteMask = 0;
	 m_iOverwriteMask = 0;
	 m_iVersion=ai_iVersion;
     m_CompressionType = 0;
     m_ObjectId = 0;
	 m_uiUserInfo = 0;
  //## end ClPDU::ClPDU%875692541.body
}


ClPDU::~ClPDU ()
{
  //## begin ClPDU::~ClPDU%878377084.body preserve=yes
	m4uint32_t	ulHeadSize;
	if ( m_pBinaryHead != NULL )
	{
		ulHeadSize = GetHeadSize();
//		free ( m_pBinaryHead );
		delete [] m_pBinaryHead;
		m_pBinaryHead = NULL;
	};

	if ( m_szDump != NULL ) 
		delete m_szDump;

  //## end ClPDU::~ClPDU%878377084.body
}



//## Other Operations (implementation)
m4uint32_t ClPDU::GetHeadSize ()
{
  //## begin ClPDU::GetHeadSize%875278540.body preserve=yes
	//		UInt	m_uiId(2) + 0(6)
	//		ULong	m_ulSize(4) + m_iVersion(4)
	//		UWLong	m_ulTimeMark(8)
	//		UInt	m_uiLogInfo(2) + m_iHasOverwriteMask(1) + m_iOverwriteMask(1) + 0(4)
	//		ULong	m_oLogInformation.m_lSize(4) +  UInt StatInfo(4)
	// Todos ocupan 8 bytes
	return 5*M4_PDU_ALIGN_SIZE;
  //## end ClPDU::GetHeadSize%875278540.body
}

m4uint16_t ClPDU::GetIdPDU ()
{
  //## begin ClPDU::GetIdPDU%876384553.body preserve=yes
	return m_uiId;
  //## end ClPDU::GetIdPDU%876384553.body
}

m4uint16_t ClPDU::GetTypePDU ()
{
  //## begin ClPDU::GetTypePDU%882288136.body preserve=yes
	return m_uiType;
  //## end ClPDU::GetTypePDU%882288136.body
}

m4uint32_t ClPDU::GetSize ()
{
  //## begin ClPDU::GetSize%875556372.body preserve=yes
	return m_ulSize;
  //## end ClPDU::GetSize%875556372.body
}

m4uint32_t ClPDU::GetDeltaSize( void ) const
{
	return( m_ulDeltaSize ) ;
}

void ClPDU::SetDeltaSize( m4uint32_t ai_iDeltaSize )
{
	m_ulDeltaSize = ai_iDeltaSize ;
}

m4uint64_t ClPDU::GetTimeMark ()
{
  //## begin ClPDU::GetTimeMark%875556373.body preserve=yes
	return m_ulTimeMark;
  //## end ClPDU::GetTimeMark%875556373.body
}

void ClPDU::SetTimeMark (m4uint64_t ai_ulTimeMark)
{
  //## begin ClPDU::SetTimeMark%878662601.body preserve=yes
	m_ulTimeMark = ai_ulTimeMark;
  //## end ClPDU::SetTimeMark%878662601.body
}

void ClPDU::Dump (m4pchar_t  &ao_pTexto)
{
  //## begin ClPDU::Dump%876384568.body preserve=yes
   m4char_t	aData[64];

 //  ao_pTexto +="\n Atributos de ClPDU.";

	strcat (ao_pTexto ,"\n m_uiId. ");
   sprintf( aData, "%u", m_uiId );
   strcat (ao_pTexto ,aData);

   strcat (ao_pTexto ,"\n m_ulSize. ");
   sprintf( aData, "%lu", m_ulSize );
   strcat (ao_pTexto ,aData);

   strcat (ao_pTexto ,"\n m_ulTimeMark. ");
   sprintf( aData, "%lu", m_ulTimeMark );
   strcat (ao_pTexto ,aData);

   strcat (ao_pTexto ,"\n m_uiLogInfo. ");
   sprintf( aData, "%d", m_uiLogInfo );
   strcat (ao_pTexto ,aData);

   strcat (ao_pTexto ,"\n m_iHasOverwriteMask. ");
   sprintf( aData, "%d", m_iHasOverwriteMask );
   strcat (ao_pTexto ,aData);
   
   strcat (ao_pTexto ,"\n m_iOverwriteMask. ");
   sprintf( aData, "%d", m_iOverwriteMask );
   strcat (ao_pTexto ,aData);
   
   strcat (ao_pTexto ,"\n m_oLogInformation.size ");
   sprintf( aData, "%lu", m_oLogInformation.m_lSize);
   strcat (ao_pTexto ,aData);

   strcat (ao_pTexto ,"\n m_uiUserInfo ");
   sprintf( aData, "%lu", m_uiUserInfo);
   strcat (ao_pTexto ,aData);

   //## end ClPDU::Dump%876384568.body
}

m4uint32_t ClPDU::GetPDUSize ()
{
  //## begin ClPDU::GetPDUSize%932487951.body preserve=yes
	return 0;
  //## end ClPDU::GetPDUSize%932487951.body
}

void ClPDU::Clear ()
{
  //## begin ClPDU::Clear%878662608.body preserve=yes
/////////// TEMPORAL
	m_uiState = M4_PDUSTATE_HEAD1;
  //## end ClPDU::Clear%878662608.body
}

m4return_t ClPDU::AddBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClPDU::AddBlock%879761053.body preserve=yes
	
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:

    // Ejecuto el AddDataBlock propio de ClPDU

		m_ulSize =m_oSerializer.GetUInt32 ();
		m_iVersion=m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulTimeMark =m_oSerializer.GetUInt64();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT64_SIZE);

		m_uiLogInfo =m_oSerializer.GetUInt16 ();
		m_iHasOverwriteMask =m_oSerializer.GetUInt8 ();
		m_iOverwriteMask =m_oSerializer.GetUInt8 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE-M4_PDUSERIALIZER_UINT8_SIZE-M4_PDUSERIALIZER_UINT8_SIZE);

		m_oLogInformation.m_lSize =m_oSerializer.GetUInt32();
		m_uiUserInfo = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;		
		break;
	
	case M4_PDUSTATE_HEADFULL:
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}
  //## end ClPDU::AddBlock%879761053.body
}

m4return_t ClPDU::AddDataBlock (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted)
{
  //## begin ClPDU::AddDataBlock%885373442.body preserve=yes
	M4_ASSERT(1==2);
	return M4_ERROR;
  //## end ClPDU::AddDataBlock%885373442.body
}

m4return_t ClPDU::GetHeadBlock (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClPDU::GetHeadBlock%878662610.body preserve=yes
	M4_ASSERT(1==2);
	return M4_ERROR;
  //## end ClPDU::GetHeadBlock%878662610.body
}

m4return_t ClPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClPDU::GetBinaryHead%878662611.body preserve=yes

	m4uint32_t	ulCont = 0, ln = 0;
	m4uint64_t	lwn = 0;
	m4uint16_t	in = 0;
	GetPDUSize();		//Para que el m_ulSize  sea correcto
	m_oSerializer.Initialize();

	m_oSerializer.AddUInt16(m_uiId );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32(m_ulSize );
	m_oSerializer.AddUInt32(m_iVersion );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt64(m_ulTimeMark );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT64_SIZE);

	m_oSerializer.AddUInt16(m_uiLogInfo );
	m_oSerializer.AddUInt8(m_iHasOverwriteMask );
	m_oSerializer.AddUInt8(m_iOverwriteMask );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE-M4_PDUSERIALIZER_UINT8_SIZE-M4_PDUSERIALIZER_UINT8_SIZE);

	m_oSerializer.AddUInt32(m_oLogInformation.m_lSize );
	m_oSerializer.AddUInt32( GetUserInfoFromCallBack() );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;
  //## end ClPDU::GetBinaryHead%878662611.body
}

m4return_t ClPDU::GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClPDU::GetNextBlock%879425087.body preserve=yes

	m_uiState = M4_PDUSTATE_READSOCKET;
	ao_pBlock = NULL;
	ao_ulBlockSize = 0;
	return M4_ERROR;
  //## end ClPDU::GetNextBlock%879425087.body
}

m4uint32_t ClPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClPDU::GetNextSizeBlock%882359184.body preserve=yes
	return 0;
  //## end ClPDU::GetNextSizeBlock%882359184.body
}

M4DataStorage * ClPDU::GetDataStorage (m4bool_t ai_bOwner)
{
  //## begin ClPDU::GetDataStorage%884343824.body preserve=yes
	return NULL;
  //## end ClPDU::GetDataStorage%884343824.body
}

void ClPDU::SetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner)
{
  //## begin ClPDU::SetDataStorage%884343825.body preserve=yes
  //## end ClPDU::SetDataStorage%884343825.body
}

void ClPDU::Inicialize (ClPDU *ai_pPDU)
{
  //## begin ClPDU::Inicialize%884850778.body preserve=yes
    m_ObjectId = ai_pPDU->GetObjectId();
    m_CompressionType = ai_pPDU->GetCompressionType();
  //## end ClPDU::Inicialize%884850778.body
}

m4return_t ClPDU::AddDataUnit (ClDataUnit *ai_pDataUnit)
{
  //## begin ClPDU::AddDataUnit%885978329.body preserve=yes
	return M4_ERROR;
  //## end ClPDU::AddDataUnit%885978329.body
}

m4return_t ClPDU::GetNewDataBuffer (m4pchar_t &ao_pBuffer, m4uint32_t ao_ulSize)
{
  //## begin ClPDU::GetNewDataBuffer%887043276.body preserve=yes
	return M4_ERROR;
  //## end ClPDU::GetNewDataBuffer%887043276.body
}

void ClPDU::EnableLogInfo ()
{
  //## begin ClPDU::EnableLogInfo%896370093.body preserve=yes
	m_uiLogInfo = 1;
  //## end ClPDU::EnableLogInfo%896370093.body
}

void ClPDU::DisableLogInfo ()
{
  //## begin ClPDU::DisableLogInfo%896370094.body preserve=yes
	m_uiLogInfo = 0;
  //## end ClPDU::DisableLogInfo%896370094.body
}

m4return_t ClPDU::GetLogInfo (m4pchar_t &ao_pLogInfo, m4uint32_t &ai_lLogInfoSize)
{
  //## begin ClPDU::GetLogInfo%896370095.body preserve=yes

	if (LogInfo() == M4_TRUE) {
	  ao_pLogInfo = m_oLogInformation.m_pChar;
	  ai_lLogInfoSize = m_oLogInformation.m_lSize ;
	  return M4_SUCCESS;
	};
	ao_pLogInfo=NULL;
	ai_lLogInfoSize=0;

	return M4_ERROR;

  //## end ClPDU::GetLogInfo%896370095.body
}

m4return_t ClPDU::SetLogInfo (m4pchar_t ai_pLogInfo, m4uint32_t ai_lLogInfoSize)
{
  //## begin ClPDU::SetLogInfo%896370096.body preserve=yes

	if (LogInfo() == M4_TRUE) {
		if( (m_oLogInformation.m_lSize) && (NULL!=m_oLogInformation.m_pChar) )	{
			delete m_oLogInformation.m_pChar ;
			m_oLogInformation.m_pChar=NULL;
		}
		if(ai_lLogInfoSize>0){
		  m_oLogInformation.m_pChar = new m4char_t[ai_lLogInfoSize];
		  memcpy(m_oLogInformation.m_pChar,ai_pLogInfo,ai_lLogInfoSize);
		  m_oLogInformation.m_lSize = ai_lLogInfoSize;
		}
	  return M4_SUCCESS;
	}

	return M4_ERROR;

  //## end ClPDU::SetLogInfo%896370096.body
}

m4bool_t ClPDU::LogInfo ()
{
  //## begin ClPDU::LogInfo%896370097.body preserve=yes

	if (m_uiLogInfo == 1)
	{
		return M4_TRUE;
	}
	else
	{
		return M4_FALSE;
	}

  //## end ClPDU::LogInfo%896370097.body
}

m4uint8_t	ClPDU::GetHasOverwriteMask( void ) const
{
	return( m_iHasOverwriteMask ) ;
}

void	ClPDU::SetHasOverwriteMask( m4uint8_t ai_iHasOverwriteMask )
{
	m_iHasOverwriteMask = ai_iHasOverwriteMask ;
}

m4uint8_t	ClPDU::GetOverwriteMask( void ) const
{
	return( m_iOverwriteMask ) ;
}

void	ClPDU::SetOverwriteMask( m4uint8_t ai_iOverwriteMask )
{
	m_iOverwriteMask = ai_iOverwriteMask ;
}

m4int_t ClPDU::GetVersion ()
{
  //## begin ClPDU::GetVersion%964010743.body preserve=yes
	return m_iVersion;
  //## end ClPDU::GetVersion%964010743.body
}

m4uint32_t ClPDU::GetRequestId ()
{
  //## begin ClPDU::GetRequestId%964010744.body preserve=yes
	return m_ulRequestID;
  //## end ClPDU::GetRequestId%964010744.body
}

m4uint32_t ClPDU::GetUserInfo ()
{
	return m_uiUserInfo;
}

m4void_t ClPDU::SetuserInfo(m4uint32_t ai_uiUserInfo)
{
	m_uiUserInfo = ai_uiUserInfo;
}

void ClPDU::SetRequestId (m4uint32_t ai_ulRequestId)
{
  //## begin ClPDU::SetRequestId%964010745.body preserve=yes
	m_ulRequestID=ai_ulRequestId;
  //## end ClPDU::SetRequestId%964010745.body
}

m4return_t ClPDU::SetCredential (m4string_t &ai_strCredential)
{
  //## begin ClPDU::SetCredential%972376846.body preserve=yes
	return M4_ERROR;
  //## end ClPDU::SetCredential%972376846.body
}

m4return_t ClPDU::GetCredential (m4string_t &ao_strCredential)
{
  //## begin ClPDU::GetCredential%972376847.body preserve=yes
	return M4_ERROR;
  //## end ClPDU::GetCredential%972376847.body
}

m4return_t ClPDU::GetCertificate (m4pchar_t &ao_pCertificate , m4uint32_t & ao_iCertificateLen)
{
	return M4_ERROR ;
}
m4return_t ClPDU::SetCertificate (m4pchar_t ai_pCertificate , m4uint32_t ai_iCertificateLen)
{
	return M4_ERROR ;
}

m4return_t ClPDU::SetSessContext (m4pchar_t ao_pSessContex , m4uint32_t ao_iSessContextLen)
{
	return M4_ERROR ;
}
m4return_t ClPDU::GetSessContext (m4pchar_t & ai_pSessContex , m4uint32_t & ai_iSessContextLen)
{
	return M4_ERROR ;
}

m4uint32_t ClPDU::GetUserInfoFromCallBack()
{
	m_uiUserInfo = 0;

	if (m_pfUserInfoCallBack != NULL)
	{
		m_uiUserInfo = (*m_pfUserInfoCallBack)();
	}

	return m_uiUserInfo;
}


//## begin module%342BBAB6024A.epilog preserve=yes
//## end module%342BBAB6024A.epilog
