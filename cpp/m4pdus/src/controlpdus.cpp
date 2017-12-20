//## begin module%3468A65001C9.cm preserve=no
//## end module%3468A65001C9.cm

//## begin module%3468A65001C9.cp preserve=no
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
//## end module%3468A65001C9.cp

//## Module: controlpdus%3468A65001C9; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\controlpdus.cpp

//## begin module%3468A65001C9.additionalIncludes preserve=no
//## end module%3468A65001C9.additionalIncludes

//## begin module%3468A65001C9.includes preserve=yes
#include <m4trace.hpp>
//## end module%3468A65001C9.includes

// controlpdus
#include <controlpdus.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%3468A65001C9.declarations preserve=no
//## end module%3468A65001C9.declarations

//## begin module%3468A65001C9.additionalDeclarations preserve=yes
//## end module%3468A65001C9.additionalDeclarations


// Class ClControlPDU 

ClControlPDU::ClControlPDU (m4int_t ai_iVersion)
  //## begin ClControlPDU::ClControlPDU%878806460.hasinit preserve=no
  //## end ClControlPDU::ClControlPDU%878806460.hasinit
  //## begin ClControlPDU::ClControlPDU%878806460.initialization preserve=yes
  :ClPDU(ai_iVersion)
  //## end ClControlPDU::ClControlPDU%878806460.initialization
{
  //## begin ClControlPDU::ClControlPDU%878806460.body preserve=yes
  //## end ClControlPDU::ClControlPDU%878806460.body
}


ClControlPDU::~ClControlPDU ()
{
  //## begin ClControlPDU::~ClControlPDU%878806461.body preserve=yes
  //## end ClControlPDU::~ClControlPDU%878806461.body
}



//## Other Operations (implementation)
m4return_t ClControlPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClControlPDU::GetBinaryHead%878662677.body preserve=yes
	return M4_ERROR;
  //## end ClControlPDU::GetBinaryHead%878662677.body
}

m4uint32_t ClControlPDU::GetHeadSize ()
{
  //## begin ClControlPDU::GetHeadSize%878806448.body preserve=yes
	return 0;
  //## end ClControlPDU::GetHeadSize%878806448.body
}

void ClControlPDU::Dump (m4pchar_t  &ao_pTexto)
{
  //## begin ClControlPDU::Dump%879157701.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

	ClPDU::Dump(ao_pTexto);
  //## end ClControlPDU::Dump%879157701.body
}

m4return_t ClControlPDU::GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize)
{
  //## begin ClControlPDU::GetNextBlock%879502155.body preserve=yes
    if (m_uiState == M4_PDUSTATE_HEAD1) 
	{
		GetBinaryHead(ao_pBlock,ao_ulBlockSize);
		m_uiState=M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
	}

	ao_pBlock = NULL;
	ao_ulBlockSize = 0;
	return M4_ERROR;
	
  //## end ClControlPDU::GetNextBlock%879502155.body
}

m4uint32_t ClControlPDU::GetPDUSize ()
{
  //## begin ClControlPDU::GetPDUSize%970646986.body preserve=yes
	m_ulSize = GetHeadSize ();
	if (LogInfo()) 
		m_ulSize += m_oLogInformation.m_lSize;
	return m_ulSize;
  //## end ClControlPDU::GetPDUSize%970646986.body
}

// Additional Declarations
  //## begin ClControlPDU%345F0D430012.declarations preserve=yes
  //## end ClControlPDU%345F0D430012.declarations

// Class ClErrorPDU 


ClErrorPDU::ClErrorPDU (m4int_t ai_iVersion)
  //## begin ClErrorPDU::ClErrorPDU%875780548.hasinit preserve=no
      : m_ulErrorCode(0)
  //## end ClErrorPDU::ClErrorPDU%875780548.hasinit
  //## begin ClErrorPDU::ClErrorPDU%875780548.initialization preserve=yes
 , ClControlPDU(ai_iVersion)
  //## end ClErrorPDU::ClErrorPDU%875780548.initialization
{
  //## begin ClErrorPDU::ClErrorPDU%875780548.body preserve=yes
	m_uiId = M4_ID_PDU_ERROR;
  //## end ClErrorPDU::ClErrorPDU%875780548.body
}


ClErrorPDU::~ClErrorPDU ()
{
  //## begin ClErrorPDU::~ClErrorPDU%878377094.body preserve=yes
  //## end ClErrorPDU::~ClErrorPDU%878377094.body
}



//## Other Operations (implementation)
m4uint32_t ClErrorPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClErrorPDU::GetNextSizeBlock%875780567.body preserve=yes
	m4int32_t tam=0;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = ClPDU::GetHeadSize () - M4_PDU_IDPDU_SIZE; 
	    tam+=  M4_PDU_ALIGN_SIZE*2;
		ao_pBlock = new m4char_t[tam];
    	break;
	default:
		tam = 0;
	}
	return tam;
  //## end ClErrorPDU::GetNextSizeBlock%875780567.body
}

m4return_t ClErrorPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClErrorPDU::AddBlock%875780568.body preserve=yes
	m_oSerializer .SetpBuffer (ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulErrorCode =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
        
		m_uiState = M4_PDUSTATE_HEADFULL;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEADFULL:
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}	
  //## end ClErrorPDU::AddBlock%875780568.body
}

m4uint32_t ClErrorPDU::GetHeadSize ()
{
  //## begin ClErrorPDU::GetHeadSize%876211534.body preserve=yes
	m4uint32_t tam = ClPDU::GetHeadSize()+ M4_PDU_ALIGN_SIZE*2; // ReqId & ErrorCode
	return (tam) ;
  //## end ClErrorPDU::GetHeadSize%876211534.body
}

void ClErrorPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClErrorPDU::Dump%878139978.body preserve=yes
 	if ( m_szDump != NULL )
		delete [] m_szDump;

   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer," Dump de ClErrorPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);

   m4char_t	aData[32];

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n m_ulErrorCode. ");
   sprintf( aData, "%lu", m_ulErrorCode );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n\n" );


  //## end ClErrorPDU::Dump%878139978.body
}

m4uint32_t ClErrorPDU::GetErrorCode ()
{
  //## begin ClErrorPDU::GetErrorCode%878662672.body preserve=yes
	return m_ulErrorCode;
  //## end ClErrorPDU::GetErrorCode%878662672.body
}

void ClErrorPDU::SetErrorCode (m4uint32_t ai_ulErrorCode)
{
  //## begin ClErrorPDU::SetErrorCode%878662673.body preserve=yes
	m_ulErrorCode = ai_ulErrorCode;
  //## end ClErrorPDU::SetErrorCode%878662673.body
}

m4return_t ClErrorPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClErrorPDU::GetBinaryHead%878662674.body preserve=yes
	m4uint32_t lnCont=0;
  
	//GetPDUSize();  Se hace en el GetBinaryHead
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32(  m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(  m_ulErrorCode );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
  //## end ClErrorPDU::GetBinaryHead%878662674.body
}

// Additional Declarations
  //## begin ClErrorPDU%342FBA02026D.declarations preserve=yes
  //## end ClErrorPDU%342FBA02026D.declarations

// Class ClAckPDU 



ClAckPDU::ClAckPDU (m4int_t ai_iVersion)
  //## begin ClAckPDU::ClAckPDU%875780547.hasinit preserve=no
      : m_ulRequestId(0)
  //## end ClAckPDU::ClAckPDU%875780547.hasinit
  //## begin ClAckPDU::ClAckPDU%875780547.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion)
  //## end ClAckPDU::ClAckPDU%875780547.initialization
{
  //## begin ClAckPDU::ClAckPDU%875780547.body preserve=yes
	m_uiWhy = 0;
	m_uiId = M4_ID_PDU_ACK;
	m_uiType=M4_TYPE_PDU_CONTROL;
  //## end ClAckPDU::ClAckPDU%875780547.body
}


ClAckPDU::~ClAckPDU ()
{
  //## begin ClAckPDU::~ClAckPDU%878377093.body preserve=yes
  //## end ClAckPDU::~ClAckPDU%878377093.body
}



//## Other Operations (implementation)
m4uint32_t ClAckPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClAckPDU::GetNextSizeBlock%875780565.body preserve=yes

 	m4uint32_t	ulSize = 0, tam = 0, modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEAD2:
		m_ulNBlocks = GetNBlocks();
		tam = m_ulNBlocks * 4;
		if (tam) 
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;

	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		return 0;
	}

	return 0;

  //## end ClAckPDU::GetNextSizeBlock%875780565.body
}

m4return_t ClAckPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClAckPDU::AddBlock%875780566.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;
	m4uint32_t ulServiceHeadSize = 0,ulReturnCoden = 0, ulSize=0, ulBlockSize = 0, ulCont=0; 
	m4bool_t	bEncrypted;
	m_oSerializer.SetpBuffer(ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(pBlockTemp)== M4_ERROR)
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
	   
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiWhy=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulNBlocks=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		delete [] ai_pBlock;  
		
		m_uiState = M4_PDUSTATE_HEAD2;
		return M4_SUCCESS;		
		break;

	case M4_PDUSTATE_HEAD2:
	
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
		m_uiState = M4_PDUSTATE_HEADFULL;
		m_poBlockFormats -> InitializeIteration();
		return M4_SUCCESS;
		break;

	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}

	
  //## end ClAckPDU::AddBlock%875780566.body
}

m4uint32_t ClAckPDU::GetHeadSize ()
{
  //## begin ClAckPDU::GetHeadSize%876211533.body preserve=yes
    m4uint32_t tam =ClPDU::GetHeadSize()+3 * M4_PDU_ALIGN_SIZE;  //  ReqId & Why & NBlocks
	return (tam) ;	
  //## end ClAckPDU::GetHeadSize%876211533.body
}

void ClAckPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClAckPDU::Dump%878139977.body preserve=yes

    if ( m_szDump != NULL )
		delete [] m_szDump;
	
    if ( m_szDump != NULL ) 
		delete m_szDump;
	
    m_szDump = new m4char_t[100000];  
    strcpy(m_szDump,"");	
    strcat(m_szDump," Dump de ClConnectPDU. \n\n Atributos. ");
	
	ClPDU::Dump(m_szDump);
	
	m4char_t	    aData[32];
	
	strcat(m_szDump, "\n m_ulRequestID. ");
	sprintf( aData, "%lu", m_ulRequestID );
	strcat(m_szDump, aData);
	
	strcat(m_szDump, "\n m_uiWhy. ");
	sprintf( aData, "%u", m_uiWhy );
	strcat(m_szDump, aData);
	
	ClPDUWithData::Dump(m_szDump);
	
	strcat(m_szDump, "\n\n");
	
	ai_strBuffer = m_szDump;


  //## end ClAckPDU::Dump%878139977.body
}

m4return_t ClAckPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClAckPDU::GetBinaryHead%878662671.body preserve=yes
	
	m4uint32_t	ulPDUSize = 0, ulPos = 0,  ln = 0, ulNBlocks = 0;
	m4uint16_t	uin = 0;
	m4uint8_t	usCont = 0;

// Cabecera
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) 
				== M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}
//	m_oSerializer.AddULong( m_ulRequestID );
	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	//m_oSerializer.AddUInt( m_uiWhy );
	m_oSerializer.AddUInt16( m_uiWhy );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

// Formatos
	ulNBlocks = (m_poBlockFormats -> GetFormatsSize())/4;
//	m_oSerializer.AddULong( ulNBlocks );
	m_oSerializer.AddUInt32( ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( ulNBlocks > 0 )
	{    	
		SerializeFormatsBlock( );
	};

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;

  //## end ClAckPDU::GetBinaryHead%878662671.body
}

m4uint16_t ClAckPDU::GetWhy ()
{
  //## begin ClAckPDU::GetWhy%892545848.body preserve=yes
	return m_uiWhy;
  //## end ClAckPDU::GetWhy%892545848.body
}

void ClAckPDU::SetWhy (m4uint16_t ai_uiWhy)
{
  //## begin ClAckPDU::SetWhy%892545849.body preserve=yes
	m_uiWhy = ai_uiWhy; 
  //## end ClAckPDU::SetWhy%892545849.body
}

// Additional Declarations
  //## begin ClAckPDU%342FB9EF0201.declarations preserve=yes
  //## end ClAckPDU%342FB9EF0201.declarations

// Class ClDisconnectPDU 



ClDisconnectPDU::ClDisconnectPDU (m4int_t ai_iVersion)
  //## begin ClDisconnectPDU::ClDisconnectPDU%875780545.hasinit preserve=no
      : m_hUserId(0)
  //## end ClDisconnectPDU::ClDisconnectPDU%875780545.hasinit
  //## begin ClDisconnectPDU::ClDisconnectPDU%875780545.initialization preserve=yes
  ,ClControlPDU(ai_iVersion)
  //## end ClDisconnectPDU::ClDisconnectPDU%875780545.initialization
{
  //## begin ClDisconnectPDU::ClDisconnectPDU%875780545.body preserve=yes
	m_uiId = M4_ID_PDU_DISCONNECT;
	m_uiWhy = M4_ERROR;
  //## end ClDisconnectPDU::ClDisconnectPDU%875780545.body
}


ClDisconnectPDU::~ClDisconnectPDU ()
{
  //## begin ClDisconnectPDU::~ClDisconnectPDU%878377091.body preserve=yes
  //## end ClDisconnectPDU::~ClDisconnectPDU%878377091.body
}



//## Other Operations (implementation)
m4uint32_t ClDisconnectPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClDisconnectPDU::GetNextSizeBlock%875780563.body preserve=yes
	m4int32_t tam=0;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE; 
		ao_pBlock = new m4char_t[tam];
		break;
	default:
		tam = 0;
	}
	return tam;

  //## end ClDisconnectPDU::GetNextSizeBlock%875780563.body
}

m4return_t ClDisconnectPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClDisconnectPDU::AddBlock%875780564.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;
	m_oSerializer .SetpBuffer (ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(pBlockTemp)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
		
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_hUserId=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiWhy=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_uiState = M4_PDUSTATE_HEADFULL;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEADFULL:
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}
	
  //## end ClDisconnectPDU::AddBlock%875780564.body
}

m4uint32_t ClDisconnectPDU::GetHeadSize ()
{
  //## begin ClDisconnectPDU::GetHeadSize%876211531.body preserve=yes
    m4uint32_t tam=ClPDU::GetHeadSize()+ 3 * M4_PDU_ALIGN_SIZE;   //reqid & userid & why
	return (tam) ;
  //## end ClDisconnectPDU::GetHeadSize%876211531.body
}

void ClDisconnectPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClDisconnectPDU::Dump%878139975.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer,"");	
   strcat(ai_strBuffer," Dump de ClDisConnectPDU. \n\n Atributos. ");

   ClPDU::Dump( ai_strBuffer );

   m4char_t	aData[32];

   strcat(ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(ai_strBuffer, aData);

   strcat(ai_strBuffer, "\n m_hUserId. ");
   sprintf( aData, "%lu", m_hUserId );
   strcat(ai_strBuffer, aData);

   strcat(ai_strBuffer, "\n m_uiWhy. ");
   sprintf( aData, "%u", m_uiWhy );
   strcat(ai_strBuffer, aData);
   
   strcat(ai_strBuffer, "\n\n");
  //## end ClDisconnectPDU::Dump%878139975.body
}

m4uint16_t ClDisconnectPDU::GetWhy ()
{
  //## begin ClDisconnectPDU::GetWhy%878662661.body preserve=yes
	return m_uiWhy;
  //## end ClDisconnectPDU::GetWhy%878662661.body
}

void ClDisconnectPDU::SetWhy (m4uint16_t ai_uiWhy)
{
  //## begin ClDisconnectPDU::SetWhy%878662662.body preserve=yes
	m_uiWhy = ai_uiWhy;
  //## end ClDisconnectPDU::SetWhy%878662662.body
}

void ClDisconnectPDU::SetUserId (m4handle_t ai_hUserId)
{
  //## begin ClDisconnectPDU::SetUserId%889708088.body preserve=yes
     m_hUserId = ai_hUserId;
  //## end ClDisconnectPDU::SetUserId%889708088.body
}

m4handle_t ClDisconnectPDU::GetUserId ()
{
  //## begin ClDisconnectPDU::GetUserId%889708089.body preserve=yes
	return m_hUserId;
  //## end ClDisconnectPDU::GetUserId%889708089.body
}

m4return_t ClDisconnectPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClDisconnectPDU::GetBinaryHead%878662663.body preserve=yes
 
	m4uint32_t	ulPos = 0;

	ao_ulBufferSize = 0;
	// GetPDUSize();	Se hace en el GetBinaryHead
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) == M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}

	m_oSerializer.AddUInt32(m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(m_hUserId );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16(m_uiWhy );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	return M4_SUCCESS;


  //## end ClDisconnectPDU::GetBinaryHead%878662663.body
}

// Additional Declarations
  //## begin ClDisconnectPDU%342FB9770169.declarations preserve=yes
  //## end ClDisconnectPDU%342FB9770169.declarations

// Class ClAuthenticationPDU 





ClAuthenticationPDU::ClAuthenticationPDU (m4int_t ai_iVersion)
  //## begin ClAuthenticationPDU::ClAuthenticationPDU%875780541.hasinit preserve=no
      : m_uiFlags(0), m_ulDefaultRole(0)
  //## end ClAuthenticationPDU::ClAuthenticationPDU%875780541.hasinit
  //## begin ClAuthenticationPDU::ClAuthenticationPDU%875780541.initialization preserve=yes
,  ClPDUWithData(ai_iVersion)
  //## end ClAuthenticationPDU::ClAuthenticationPDU%875780541.initialization
{
  //## begin ClAuthenticationPDU::ClAuthenticationPDU%875780541.body preserve=yes
	m_uiId = M4_ID_PDU_AUTHENTICATION;
  //## end ClAuthenticationPDU::ClAuthenticationPDU%875780541.body
}


ClAuthenticationPDU::~ClAuthenticationPDU ()
{
  //## begin ClAuthenticationPDU::~ClAuthenticationPDU%878377087.body preserve=yes
  //## end ClAuthenticationPDU::~ClAuthenticationPDU%878377087.body
}



//## Other Operations (implementation)
m4uint32_t ClAuthenticationPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClAuthenticationPDU::GetNextSizeBlock%875780555.body preserve=yes
  
	m4int32_t tam=0;
	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize()-M4_PDU_IDPDU_SIZE;	
		ao_pBlock = new m4char_t[tam];
		break;
	
	case M4_PDUSTATE_HEAD2:	
		tam = m_oUserId.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;

	case M4_PDUSTATE_HEAD3:	
    	tam = m_oPassword.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;

	case M4_PDUSTATE_HEAD4:
		tam = m_ulNBlocks * 4;
		if (tam)
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		tam = 0;
	}
	return tam;

  //## end ClAuthenticationPDU::GetNextSizeBlock%875780555.body
}

m4return_t ClAuthenticationPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClAuthenticationPDU::AddBlock%875780556.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	m4uint32_t  ulBlockSize = 0, ulCont=0; 

	m_oSerializer .SetpBuffer (ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClAuthenticationPDU

		m_ulRequestID = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		m_uiFlags = m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_oUserId.m_iSize =m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_oPassword.m_iSize =m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulDefaultRole = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulNBlocks= m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	    delete []ai_pBlock;
				
		if (m_oUserId.m_iSize  !=0)		
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
			if (m_oPassword.m_iSize!=0)     
				m_uiState = M4_PDUSTATE_HEAD3; 
			else		
				m_uiState = M4_PDUSTATE_HEAD4;

		return M4_SUCCESS;		
		break;

	case M4_PDUSTATE_HEAD2:
		m_oUserId.m_pChar = pBlockTemp;
	    if (m_oPassword.m_iSize!=0)     
			m_uiState = M4_PDUSTATE_HEAD3; 
		else	
			m_uiState = M4_PDUSTATE_HEAD4;
		return M4_SUCCESS;
		break;
    
	case M4_PDUSTATE_HEAD3:
		m_oPassword.m_pChar = pBlockTemp;
		m_uiState = M4_PDUSTATE_HEAD4;
		return M4_SUCCESS;
		break;
	
	case M4_PDUSTATE_HEAD4:
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
	
		m_poBlockFormats -> InitializeIteration();

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;
	
	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}
	
  //## end ClAuthenticationPDU::AddBlock%875780556.body
}

m4uint32_t ClAuthenticationPDU::GetHeadSize ()
{
  //## begin ClAuthenticationPDU::GetHeadSize%876211527.body preserve=yes
	m4uint32_t tam=ClPDU::GetHeadSize() +
		M4_PDU_ALIGN_SIZE * 6 + // ReqId & Flag & User.Size & Passwd.Size & DefRole & NBlocks
		m_oUserId.m_iSize +
		m_oPassword.m_iSize;
	return (tam);
  //## end ClAuthenticationPDU::GetHeadSize%876211527.body
}

void ClAuthenticationPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClAuthenticationPDU::Dump%878139971.body preserve=yes
  
	if ( m_szDump != NULL )
		delete [] m_szDump;
	
	ai_strBuffer = new m4char_t[100000];  
	strcpy(ai_strBuffer,"");	
	strcat(ai_strBuffer," Dump de ClAuthenticationPDU. \n\n Atributos. ");
	
	ClPDU::Dump(ai_strBuffer);
	
	m4char_t	aData[32];
	m4pchar_t  pcad = NULL, paux=NULL; 
	m4uint16_t tam;
	
	strcat (ai_strBuffer, "\n m_ulRequestID. ");
	sprintf( aData, "%u", m_ulRequestID );
	strcat (ai_strBuffer, aData );
	
	strcat (ai_strBuffer, "\n m_uiFlags. ");
	sprintf( aData, "%u", m_uiFlags );
	strcat (ai_strBuffer, aData );
	
	strcat (ai_strBuffer, "\n m_ulDefaultRole. ");
	if (m_ulDefaultRole == 0) strcat(ai_strBuffer," M4_FALSE "); 
	else strcat(ai_strBuffer," M4_TRUE ");
	
	tam  = m_oUserId.m_iSize;
	pcad = m_oUserId.m_pChar;
	
	if (tam!=0) 
	{
		paux = new m4char_t[tam+1];
		paux[tam]=0;
		memcpy(paux,pcad,tam);
		
		strcat (ai_strBuffer, "\n\n m_oUserId. ");
		
		strcat (ai_strBuffer, "\n Tamaño. ");
		sprintf( aData, "%u", tam  );
		strcat (ai_strBuffer, aData);
		
		strcat (ai_strBuffer, "\n Cadena. ");
		strcat (ai_strBuffer, paux);
		
		delete []paux;
	}
	
	tam  = m_oPassword.m_iSize;
	pcad = m_oPassword.m_pChar;
	
	if (tam!=0) 
	{
		paux = new m4char_t[tam+1];
		paux[tam]=0;
		memcpy(paux,pcad,tam);
		
		strcat (ai_strBuffer, "\n\n m_oPassword. ");
		
		strcat (ai_strBuffer, "\n Tamaño. ");
		sprintf( aData, "%u", tam  );
		strcat (ai_strBuffer, aData);
		
		strcat (ai_strBuffer, "\n Cadena. ");
		strcat (ai_strBuffer, paux);
		
		delete []paux;
	}
	
	strcat (ai_strBuffer, "\n\n");
	
	ClPDUWithData::Dump(ai_strBuffer);
	
	strcat (ai_strBuffer, "\n\n");

  //## end ClAuthenticationPDU::Dump%878139971.body
}

m4uint16_t ClAuthenticationPDU::GetFlags ()
{
  //## begin ClAuthenticationPDU::GetFlags%878662639.body preserve=yes
/////////// TEMPORAL
	return m_uiFlags;
  //## end ClAuthenticationPDU::GetFlags%878662639.body
}

void ClAuthenticationPDU::SetFlags (m4uint16_t ai_uiFlags)
{
  //## begin ClAuthenticationPDU::SetFlags%878662640.body preserve=yes
	m_uiFlags = ai_uiFlags;
  //## end ClAuthenticationPDU::SetFlags%878662640.body
}

m4bool_t ClAuthenticationPDU::GetDefaultRole ()
{
  //## begin ClAuthenticationPDU::GetDefaultRole%932487953.body preserve=yes
	if (m_ulDefaultRole == 0) 
		return M4_FALSE;
	else
		return M4_TRUE;
  //## end ClAuthenticationPDU::GetDefaultRole%932487953.body
}

m4uint16_t ClAuthenticationPDU::GetUserId (m4pchar_t &ao_pUserId)
{
  //## begin ClAuthenticationPDU::GetUserId%878662641.body preserve=yes

	ao_pUserId = m_oUserId.m_pChar;
	return m_oUserId.m_iSize;
  
  //## end ClAuthenticationPDU::GetUserId%878662641.body
}

void ClAuthenticationPDU::SetUserId (m4pchar_t ai_pUserId, m4uint16_t ai_uiIdSize)
{
  //## begin ClAuthenticationPDU::SetUserId%878662642.body preserve=yes


	m_oUserId.m_iSize = ai_uiIdSize;
	
	if (m_oUserId.m_iSize%4) 
		m_oUserId.m_iSize += (4-(m_oUserId.m_iSize%4));
	
	m_oUserId.m_pChar = new m4char_t[m_oUserId.m_iSize];

	// bg 0094386
	memcpy(m_oUserId.m_pChar,ai_pUserId,ai_uiIdSize);


  //## end ClAuthenticationPDU::SetUserId%878662642.body
}

m4uint16_t ClAuthenticationPDU::GetPassword (m4pchar_t &ao_pPassword)
{
  //## begin ClAuthenticationPDU::GetPassword%878662643.body preserve=yes

	ao_pPassword = m_oPassword.m_pChar;
	return m_oPassword.m_iSize;
  
  //## end ClAuthenticationPDU::GetPassword%878662643.body
}

void ClAuthenticationPDU::SetPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength)
{
  //## begin ClAuthenticationPDU::SetPassword%878662644.body preserve=yes
   
	m_oPassword.m_iSize = ai_uiPasswordLength;	
	
	if (m_oPassword.m_iSize%4) 
		m_oPassword.m_iSize += (4-(m_oPassword.m_iSize%4));

	m_oPassword.m_pChar = new m4char_t[m_oPassword.m_iSize];

	// bg 0094386
	memcpy(m_oPassword.m_pChar,ai_pPassword,ai_uiPasswordLength);


  //## end ClAuthenticationPDU::SetPassword%878662644.body
}

m4return_t ClAuthenticationPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClAuthenticationPDU::GetBinaryHead%878662645.body preserve=yes
	
	m4uint32_t lnCont=0;
	m4uint32_t	ulNBlocks = 0;
	
	ao_ulBufferSize = GetHeadSize();
	ao_pBuffer = NULL;
	
	//	GetPDUSize(); Se hace en el GetBinaryHead
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32(m_ulRequestID);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16(m_uiFlags );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16(m_oUserId.m_iSize);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16(m_oPassword.m_iSize);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32(m_ulDefaultRole);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
	
	m_oSerializer.AddUInt32(m_ulNBlocks);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if (m_oUserId.m_iSize!=0)
		m_oSerializer.AddpBufferC( m_oUserId.m_pChar, m_oUserId.m_iSize );

	if (m_oPassword.m_iSize!=0) 
		m_oSerializer.AddpBufferC( m_oPassword.m_pChar,m_oPassword.m_iSize );

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	return M4_SUCCESS;
	
  //## end ClAuthenticationPDU::GetBinaryHead%878662645.body
}

void ClAuthenticationPDU::SetDefaultRole (m4bool_t ai_DefaultRole)
{
  //## begin ClAuthenticationPDU::SetDefaultRole%932487956.body preserve=yes
	m_ulDefaultRole = (m4uint32_t) ai_DefaultRole;
  //## end ClAuthenticationPDU::SetDefaultRole%932487956.body
}

// Class ClConnectPDU 















ClConnectPDU::ClConnectPDU (m4int_t ai_iVersion)
  //## begin ClConnectPDU::ClConnectPDU%875780539.hasinit preserve=no
      : m_ulMagicNumber(0), m_ulFlags(0), m_ulSiteId(0), m_ulClientSign(0), m_uiHostId(0), m_ulRequestId(0)
  //## end ClConnectPDU::ClConnectPDU%875780539.hasinit
  //## begin ClConnectPDU::ClConnectPDU%875780539.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion)
  //## end ClConnectPDU::ClConnectPDU%875780539.initialization
{
  //## begin ClConnectPDU::ClConnectPDU%875780539.body preserve=yes
	m_uiId = M4_ID_PDU_CONNECT;
	m_uiType=M4_TYPE_PDU_CONTROL;
	InitializeIteration();
  //## end ClConnectPDU::ClConnectPDU%875780539.body
}


ClConnectPDU::~ClConnectPDU ()
{
  //## begin ClConnectPDU::~ClConnectPDU%878377085.body preserve=yes

 	ClIntDeque::iterator	iter;

	if ( ! m_oVersions.empty() )
	{
		for ( iter = m_oVersions.begin(); iter != m_oVersions.end(); iter = m_oVersions.begin() )
		m_oVersions.erase( iter );
	};

  //## end ClConnectPDU::~ClConnectPDU%878377085.body
}



//## Other Operations (implementation)
m4uint32_t ClConnectPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClConnectPDU::GetNextSizeBlock%875780551.body preserve=yes

	m4uint32_t	ulSize = 0, tam = 0, modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE; 
		ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEAD2:
		tam = m_ulNBlocks * 4;
		if (tam) 
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;

	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		return 0;
	}

	return 0;
  //## end ClConnectPDU::GetNextSizeBlock%875780551.body
}

m4return_t ClConnectPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClConnectPDU::AddBlock%875780552.body preserve=yes

	m4pchar_t pBlockTemp= ai_pBlock;
	m4uint32_t ulServiceHeadSize = 0,ulReturnCoden = 0, ulSize=0, ulBlockSize = 0, ulCont=0; 
	m4bool_t	bEncrypted;

	m_oSerializer .SetpBuffer (ai_pBlock);
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClconnectPDU	

		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiHostId =m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulNBlocks    =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		delete [] ai_pBlock;  
		
		m_uiState = M4_PDUSTATE_HEAD2;
		return M4_SUCCESS;		
		break;

	case M4_PDUSTATE_HEAD2:
	
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
		m_uiState = M4_PDUSTATE_HEADFULL;
		m_poBlockFormats -> InitializeIteration();
		return M4_SUCCESS;
		break;

	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}

  //## end ClConnectPDU::AddBlock%875780552.body
}

m4uint32_t ClConnectPDU::GetHeadSize ()
{
  //## begin ClConnectPDU::GetHeadSize%876211525.body preserve=yes
    m4uint32_t tam=ClPDU::GetHeadSize()+ M4_PDU_ALIGN_SIZE * 3;	//ReqID & HostId & NBlocks
	return (tam) ;
  //## end ClConnectPDU::GetHeadSize%876211525.body
}

void ClConnectPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClConnectPDU::Dump%876384556.body preserve=yes

	if ( m_szDump != NULL )
		delete [] m_szDump;
	
    if ( m_szDump != NULL ) delete m_szDump;
	
    m_szDump = new m4char_t[100000];  
    strcpy(m_szDump,"");	
    strcat(m_szDump," Dump de ClConnectPDU. \n\n Atributos. ");
	
	ClPDU::Dump(m_szDump);
	
	m4char_t	    aData[32];
	
	strcat(m_szDump, "\n m_uiHostId. ");
	sprintf( aData, "%lu", m_uiHostId );
	strcat(m_szDump, aData);
	
	strcat(m_szDump, "\n m_ulRequestID. ");
	sprintf( aData, "%lu", m_ulRequestID );
	strcat(m_szDump, aData);
	
	ClPDUWithData::Dump(m_szDump);
	
	strcat(m_szDump, "\n\n");
	
	ai_strBuffer = m_szDump;

  //## end ClConnectPDU::Dump%876384556.body
}

m4return_t ClConnectPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClConnectPDU::GetBinaryHead%878662625.body preserve=yes

	m4uint32_t	ulPos = 0;
	m4uint16_t	uin = 0;
	m4uint8_t	usCont = 0;

	ao_ulBufferSize = 0;
	//GetPDUSize();	 Se hace en el GetBinaryHead
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) == M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}
	
	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_uiHostId );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	return M4_SUCCESS;

  //## end ClConnectPDU::GetBinaryHead%878662625.body
}

void ClConnectPDU::InitializeIteration ()
{
  //## begin ClConnectPDU::InitializeIteration%880122145.body preserve=yes
   m_oIterator = m_oVersions.begin();
  //## end ClConnectPDU::InitializeIteration%880122145.body
}

// Additional Declarations
  //## begin ClConnectPDU%342FA0480209.declarations preserve=yes
  //## end ClConnectPDU%342FA0480209.declarations

// Class ClSetRolePDU 



ClSetRolePDU::ClSetRolePDU (m4int_t ai_iVersion)
  //## begin ClSetRolePDU::ClSetRolePDU%889805593.hasinit preserve=no
      : m_bDefaultRole(M4_FALSE)
  //## end ClSetRolePDU::ClSetRolePDU%889805593.hasinit
  //## begin ClSetRolePDU::ClSetRolePDU%889805593.initialization preserve=yes
  ,ClControlPDU(ai_iVersion)
  //## end ClSetRolePDU::ClSetRolePDU%889805593.initialization
{
  //## begin ClSetRolePDU::ClSetRolePDU%889805593.body preserve=yes
  m_uiId = M4_ID_PDU_SETROLE;
  //## end ClSetRolePDU::ClSetRolePDU%889805593.body
}


ClSetRolePDU::~ClSetRolePDU ()
{
  //## begin ClSetRolePDU::~ClSetRolePDU%889805594.body preserve=yes
  //## end ClSetRolePDU::~ClSetRolePDU%889805594.body
}



//## Other Operations (implementation)
m4uint32_t ClSetRolePDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClSetRolePDU::GetNextSizeBlock%889805595.body preserve=yes
 	m4int32_t tam=0;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;	
		ao_pBlock = new m4char_t[tam];
    	break;
	case M4_PDUSTATE_HEAD2:	
		tam = m_oRole.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	default:
		tam = 0;
	}
	return tam;
	
  //## end ClSetRolePDU::GetNextSizeBlock%889805595.body
}

m4return_t ClSetRolePDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClSetRolePDU::AddBlock%889805596.body preserve=yes
  
	m_oSerializer .SetpBuffer(ai_pBlock);
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

	// Ejecuto el AddDataBlock propio de ClDisconnectPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
 
		m_bDefaultRole=m_oSerializer.GetUInt32()!=0?M4_TRUE:M4_FALSE;
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_oRole=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);
 
		
        delete  []ai_pBlock;

		if (m_oRole.m_iSize  !=0)		
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
			m_uiState = M4_PDUSTATE_HEADFULL;

		return M4_SUCCESS;		
		break;
	
	case M4_PDUSTATE_HEAD2:
		
		m_oRole.m_pChar = ai_pBlock;
		
		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;	
	
	case M4_PDUSTATE_HEADFULL:
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}
	
  //## end ClSetRolePDU::AddBlock%889805596.body
}

m4uint32_t ClSetRolePDU::GetHeadSize ()
{
  //## begin ClSetRolePDU::GetHeadSize%889805598.body preserve=yes
    m4uint32_t tam= ClPDU::GetHeadSize()+ 3* M4_PDU_ALIGN_SIZE+ m_oRole.m_iSize; //ReqID & RoleSize & DefaultRole
	return (tam) ;
  //## end ClSetRolePDU::GetHeadSize%889805598.body
}

void ClSetRolePDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClSetRolePDU::Dump%889805601.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

   m_szDump = new m4char_t[1000];  
   strcpy(m_szDump," Dump de ClSetRolePDU. \n\n Atributos. ");
   
   ClPDU::Dump(m_szDump);

   m4char_t	aData[32];
   m4pchar_t  pcad = NULL, paux=NULL; 
   m4uint16_t tam;

   strcat(m_szDump, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(m_szDump, aData);

   strcat(m_szDump, "\n m_bDefaultRole. ");
   sprintf( aData, "%u", m_bDefaultRole );
   strcat(m_szDump, aData);

   tam  = m_oRole.m_iSize;
   pcad = m_oRole.m_pChar;

   if (tam!=0) 
   {
     paux = new m4char_t[tam+1];
     paux[tam]=0;
     memcpy(paux,pcad,tam);

	 strcat (m_szDump, "\n\n m_oRole. ");

     strcat (m_szDump, "\n Tamaño. ");
     sprintf( aData, "%u", tam  );
     strcat (m_szDump, aData);
	 
     strcat (m_szDump, "\n Cadena. ");
     strcat (m_szDump, paux);

	 delete []paux;
   };

   strcat (m_szDump, "\n\n" );
   ai_strBuffer = m_szDump;
  //## end ClSetRolePDU::Dump%889805601.body
}

m4return_t ClSetRolePDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClSetRolePDU::GetBinaryHead%889805604.body preserve=yes
  	m4uint32_t lnCont=0;
	
	ao_ulBufferSize = GetHeadSize();

	//GetPDUSize(); Se hace en el getBinaryHead
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_bDefaultRole );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16( m_oRole.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	if (m_oRole.m_iSize!=0)
		m_oSerializer.AddpBufferC( m_oRole.m_pChar, m_oRole.m_iSize );

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
  //## end ClSetRolePDU::GetBinaryHead%889805604.body
}

m4uint16_t ClSetRolePDU::GetRole (m4pchar_t &ao_pRoleName)
{
  //## begin ClSetRolePDU::GetRole%896888578.body preserve=yes
 	ao_pRoleName = m_oRole.m_pChar;
	return m_oRole.m_iSize;
  //## end ClSetRolePDU::GetRole%896888578.body
}

void ClSetRolePDU::SetRole (m4pchar_t ai_pRoleName, m4uint16_t ai_uiRoleLength)
{
  //## begin ClSetRolePDU::SetRole%896888579.body preserve=yes
	m_oRole.m_pChar = new m4char_t[ai_uiRoleLength];
	memcpy(m_oRole.m_pChar,ai_pRoleName,ai_uiRoleLength);
	m_oRole.m_iSize = ai_uiRoleLength;
  //## end ClSetRolePDU::SetRole%896888579.body
}

void ClSetRolePDU::SetDefaultRole (m4bool_t ai_bDefaultRole)
{
  //## begin ClSetRolePDU::SetDefaultRole%897314596.body preserve=yes
	m_bDefaultRole = ai_bDefaultRole;
  //## end ClSetRolePDU::SetDefaultRole%897314596.body
}

m4bool_t ClSetRolePDU::IsDefaultRole ()
{
  //## begin ClSetRolePDU::IsDefaultRole%897314597.body preserve=yes
  	if (m_bDefaultRole == M4_TRUE ) 
		return M4_TRUE;
	else
		return M4_FALSE;
  //## end ClSetRolePDU::IsDefaultRole%897314597.body
}

// Additional Declarations
  //## begin ClSetRolePDU%350959CF0278.declarations preserve=yes
  //## end ClSetRolePDU%350959CF0278.declarations

// Class ClChangePasswordPDU 



ClChangePasswordPDU::ClChangePasswordPDU (m4int_t ai_iVersion)
  //## begin ClChangePasswordPDU::ClChangePasswordPDU%898707101.hasinit preserve=no
  //## end ClChangePasswordPDU::ClChangePasswordPDU%898707101.hasinit
  //## begin ClChangePasswordPDU::ClChangePasswordPDU%898707101.initialization preserve=yes
  :ClControlPDU(ai_iVersion)
  //## end ClChangePasswordPDU::ClChangePasswordPDU%898707101.initialization
{
  //## begin ClChangePasswordPDU::ClChangePasswordPDU%898707101.body preserve=yes
 	m_uiId = M4_ID_PDU_CHANGEPASSWORD;
  //## end ClChangePasswordPDU::ClChangePasswordPDU%898707101.body
}


ClChangePasswordPDU::~ClChangePasswordPDU ()
{
  //## begin ClChangePasswordPDU::~ClChangePasswordPDU%898707102.body preserve=yes
  //## end ClChangePasswordPDU::~ClChangePasswordPDU%898707102.body
}



//## Other Operations (implementation)
m4uint32_t ClChangePasswordPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClChangePasswordPDU::GetNextSizeBlock%898707103.body preserve=yes
  	m4int32_t tam=0;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize()-M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD2:	
		tam = m_oCurrentPassword.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD3:	
    	tam = m_oNewPassword.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	default:
		tam = 0;
	}
	return tam;
  //## end ClChangePasswordPDU::GetNextSizeBlock%898707103.body
}

m4return_t ClChangePasswordPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClChangePasswordPDU::AddBlock%898707104.body preserve=yes

	m_oSerializer.SetpBuffer(ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClChangePasswordPDU
         
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_oCurrentPassword.m_iSize=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_oNewPassword.m_iSize=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	    delete []ai_pBlock;
		if (m_oCurrentPassword.m_iSize  !=0)		
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
			if (m_oNewPassword.m_iSize!=0)				
				m_uiState = M4_PDUSTATE_HEAD3; 
			else
				m_uiState = M4_PDUSTATE_HEADFULL;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		
		m_oCurrentPassword.m_pChar = ai_pBlock;
		
	    if (m_oNewPassword.m_iSize!=0)     
			m_uiState = M4_PDUSTATE_HEAD3; 
		else	
			m_uiState = M4_PDUSTATE_HEADFULL;

		return M4_SUCCESS;
		break;
    
	case M4_PDUSTATE_HEAD3:
		
		m_oNewPassword.m_pChar = ai_pBlock;
		
		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;
	
	case M4_PDUSTATE_HEADFULL:

	    return M4_SUCCESS;		
		break;
	default:
		return M4_ERROR;
	}
	
  //## end ClChangePasswordPDU::AddBlock%898707104.body
}

m4uint32_t ClChangePasswordPDU::GetHeadSize ()
{
  //## begin ClChangePasswordPDU::GetHeadSize%898707106.body preserve=yes
	m4uint32_t tam = ClPDU::GetHeadSize()+ M4_PDU_ALIGN_SIZE * 3  // ReqID & NewPasswdSize & CurrentPasswdSize
		+ m_oCurrentPassword.m_iSize
		+ m_oNewPassword.m_iSize;
	return (tam);
  //## end ClChangePasswordPDU::GetHeadSize%898707106.body
}

void ClChangePasswordPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClChangePasswordPDU::Dump%898707109.body preserve=yes
 	if ( m_szDump != NULL )
		delete [] m_szDump;
   
   ai_strBuffer = new m4char_t[100000];  
   strcpy(ai_strBuffer,"");	
   strcat(ai_strBuffer," Dump de ClChangePasswordPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);
   
 //  ai_strBuffer +="\n\n Atributos de ClRequestPDU. \n";

   m4char_t	aData[32];
   m4pchar_t  pcad = NULL, paux=NULL; 
   m4uint16_t tam;

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );


   tam  = m_oCurrentPassword.m_iSize;
   pcad = m_oCurrentPassword.m_pChar;

   if (tam!=0) {
   
     paux = new m4char_t[tam+1];
     paux[tam]=0;
     memcpy(paux,pcad,tam);

	 strcat (ai_strBuffer, "\n\n m_oCurrentPassword. ");

     strcat (ai_strBuffer, "\n Tamaño. ");
     sprintf( aData, "%u", tam  );
     strcat (ai_strBuffer, aData);
	 
     strcat (ai_strBuffer, "\n Cadena. ");
     strcat (ai_strBuffer, paux);

	 delete []paux;
   };

   
   tam  = m_oNewPassword.m_iSize;
   pcad = m_oNewPassword.m_pChar;
   
   if (tam!=0) 
   {
	 paux = new m4char_t[tam+1];
     paux[tam]=0;
     memcpy(paux,pcad,tam);
	   
	 strcat (ai_strBuffer, "\n\n m_oNewPassword. ");

     strcat (ai_strBuffer, "\n Tamaño. ");
     sprintf( aData, "%u", tam  );
     strcat (ai_strBuffer, aData);
	 
     strcat (ai_strBuffer, "\n Cadena. ");
     strcat (ai_strBuffer, paux);

	 delete []paux;
   }
  
   strcat (ai_strBuffer, "\n\n");
  //## end ClChangePasswordPDU::Dump%898707109.body
}

m4uint16_t ClChangePasswordPDU::GetCurrentPassword (m4pchar_t &ao_pPass)
{
  //## begin ClChangePasswordPDU::GetCurrentPassword%898707110.body preserve=yes
 	ao_pPass = m_oCurrentPassword.m_pChar;
	return m_oCurrentPassword.m_iSize;
  //## end ClChangePasswordPDU::GetCurrentPassword%898707110.body
}

void ClChangePasswordPDU::SetCurrentPassword (m4pchar_t ai_pPass, m4uint16_t ai_uiPassSize)
{
  //## begin ClChangePasswordPDU::SetCurrentPassword%898707111.body preserve=yes
	m_oCurrentPassword.m_pChar = new m4char_t[ai_uiPassSize];
	memcpy(m_oCurrentPassword.m_pChar,ai_pPass,ai_uiPassSize);
	m_oCurrentPassword.m_iSize = ai_uiPassSize;
  //## end ClChangePasswordPDU::SetCurrentPassword%898707111.body
}

m4uint16_t ClChangePasswordPDU::GetNewPassword (m4pchar_t &ao_pPassword)
{
  //## begin ClChangePasswordPDU::GetNewPassword%898707112.body preserve=yes
	ao_pPassword = m_oNewPassword.m_pChar;
	return m_oNewPassword.m_iSize;
  //## end ClChangePasswordPDU::GetNewPassword%898707112.body
}

void ClChangePasswordPDU::SetNewPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength)
{
  //## begin ClChangePasswordPDU::SetNewPassword%898707113.body preserve=yes
	m_oNewPassword.m_pChar = new m4char_t[ai_uiPasswordLength];
	memcpy(m_oNewPassword.m_pChar,ai_pPassword,ai_uiPasswordLength);
	m_oNewPassword.m_iSize = ai_uiPasswordLength;
  //## end ClChangePasswordPDU::SetNewPassword%898707113.body
}

m4return_t ClChangePasswordPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClChangePasswordPDU::GetBinaryHead%898707114.body preserve=yes
	m4uint32_t lnCont=0;
	
	ao_ulBufferSize = GetHeadSize();
	ao_pBuffer = NULL;

    //	GetPDUSize();  Se hace en el GetBinaryHead
	ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_oCurrentPassword.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16( m_oNewPassword.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	if (m_oCurrentPassword.m_iSize!=0)
		m_oSerializer.AddpBufferC( m_oCurrentPassword.m_pChar, m_oCurrentPassword.m_iSize );

	if (m_oNewPassword.m_iSize!=0) 
		m_oSerializer.AddpBufferC( m_oNewPassword.m_pChar,m_oNewPassword.m_iSize );

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;
  //## end ClChangePasswordPDU::GetBinaryHead%898707114.body
}

// Additional Declarations
  //## begin ClChangePasswordPDU%35912C8D0201.declarations preserve=yes
  //## end ClChangePasswordPDU%35912C8D0201.declarations

// Class ClCancelPDU 

ClCancelPDU::ClCancelPDU (m4int_t ai_iVersion)
  //## begin ClCancelPDU::ClCancelPDU%898780370.hasinit preserve=no
  //## end ClCancelPDU::ClCancelPDU%898780370.hasinit
  //## begin ClCancelPDU::ClCancelPDU%898780370.initialization preserve=yes
  : ClControlPDU(ai_iVersion)
  //## end ClCancelPDU::ClCancelPDU%898780370.initialization
{
  //## begin ClCancelPDU::ClCancelPDU%898780370.body preserve=yes
 	m_uiId = M4_ID_PDU_CANCEL;
  //## end ClCancelPDU::ClCancelPDU%898780370.body
}


ClCancelPDU::~ClCancelPDU ()
{
  //## begin ClCancelPDU::~ClCancelPDU%898780371.body preserve=yes
  //## end ClCancelPDU::~ClCancelPDU%898780371.body
}



//## Other Operations (implementation)
m4uint32_t ClCancelPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClCancelPDU::GetNextSizeBlock%898780372.body preserve=yes
 
	m4int32_t tam=0;
	
	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
    	break;
	default:
		tam = 0;
	}
	return tam;
		
  //## end ClCancelPDU::GetNextSizeBlock%898780372.body
}

m4return_t ClCancelPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClCancelPDU::AddBlock%898780373.body preserve=yes

	m_oSerializer.SetpBuffer (ai_pBlock);
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClCancelPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEADFULL;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEADFULL:
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}	
	
  //## end ClCancelPDU::AddBlock%898780373.body
}

m4uint32_t ClCancelPDU::GetHeadSize ()
{
  //## begin ClCancelPDU::GetHeadSize%898780375.body preserve=yes
	m4uint32_t tam= ClPDU::GetHeadSize()+M4_PDU_ALIGN_SIZE;  // ReqID
	return (tam) ;
  //## end ClCancelPDU::GetHeadSize%898780375.body
}

void ClCancelPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClCancelPDU::Dump%898780378.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;
   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer,"");	
   strcat(ai_strBuffer," Dump de ClCancelPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);
   
   m4char_t	aData[32];

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );
   strcat (ai_strBuffer, "\n\n" );
  //## end ClCancelPDU::Dump%898780378.body
}

m4return_t ClCancelPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClCancelPDU::GetBinaryHead%898780381.body preserve=yes
 
	m4uint32_t lnCont=0;
  
	ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32(  m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
	
  //## end ClCancelPDU::GetBinaryHead%898780381.body
}

void ClCancelPDU::SetCancelRequestId (m4uint32_t ai_ulCancelRequestId)
{
  //## begin ClCancelPDU::SetCancelRequestId%964182213.body preserve=yes
	SetTimeMark((m4uint32_t) ai_ulCancelRequestId);
  //## end ClCancelPDU::SetCancelRequestId%964182213.body
}

m4uint32_t ClCancelPDU::GetCancelRequestId ()
{
  //## begin ClCancelPDU::GetCancelRequestId%964182214.body preserve=yes
	return (m4uint32_t)GetTimeMark();
  //## end ClCancelPDU::GetCancelRequestId%964182214.body
}

// Additional Declarations
  //## begin ClCancelPDU%35924B610246.declarations preserve=yes
  //## end ClCancelPDU%35924B610246.declarations

// Class ClNewCancelPDU 


ClNewCancelPDU::ClNewCancelPDU (m4int_t ai_iVersion)
  //## begin ClNewCancelPDU::ClNewCancelPDU%965046694.hasinit preserve=no
      : m_uiRequestID2Cancel(0)
  //## end ClNewCancelPDU::ClNewCancelPDU%965046694.hasinit
  //## begin ClNewCancelPDU::ClNewCancelPDU%965046694.initialization preserve=yes
   , ClPDUWithData(ai_iVersion)
  //## end ClNewCancelPDU::ClNewCancelPDU%965046694.initialization
{
  //## begin ClNewCancelPDU::ClNewCancelPDU%965046694.body preserve=yes
	m_uiId = M4_ID_PDU_NEWCANCEL;
  //## end ClNewCancelPDU::ClNewCancelPDU%965046694.body
}


ClNewCancelPDU::~ClNewCancelPDU ()
{
  //## begin ClNewCancelPDU::~ClNewCancelPDU%965046695.body preserve=yes
  //## end ClNewCancelPDU::~ClNewCancelPDU%965046695.body
}



//## Other Operations (implementation)
m4uint32_t ClNewCancelPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClNewCancelPDU::GetNextSizeBlock%965046696.body preserve=yes
	m4int32_t tam=0;
	
	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
    	break;
	case M4_PDUSTATE_HEAD2:
		tam = m_ulNBlocks * 4;
		if (tam) 
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		tam = 0;
	}
	return tam;

  //## end ClNewCancelPDU::GetNextSizeBlock%965046696.body
}

m4return_t ClNewCancelPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClNewCancelPDU::AddBlock%965046697.body preserve=yes
  	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t ulBlockSize,ulCont ,ulSize;
	m4bool_t bEncrypted;
	
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
		m_oSerializer .SetpBuffer (ai_pBlock);
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		m_uiRequestID2Cancel=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulNBlocks=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEAD2;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		m_oSerializer .SetpBuffer(ai_pBlock);
		
		for (  ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
		m_uiState = M4_PDUSTATE_HEADFULL;
		m_poBlockFormats -> InitializeIteration();
		return M4_SUCCESS;
		break;
	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}	
	
  //## end ClNewCancelPDU::AddBlock%965046697.body
}

m4uint32_t ClNewCancelPDU::GetHeadSize ()
{
  //## begin ClNewCancelPDU::GetHeadSize%965046699.body preserve=yes
	m4uint32_t tam= ClPDU::GetHeadSize()+3* M4_PDU_ALIGN_SIZE;  //ReqId & Req2Cancel & NBlocks
	return (tam) ;
  //## end ClNewCancelPDU::GetHeadSize%965046699.body
}

void ClNewCancelPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClNewCancelPDU::Dump%965046702.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;
   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer,"");	
   strcat(ai_strBuffer," Dump de ClNewCancelPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);
   
   m4char_t	aData[32];

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );
   strcat (ai_strBuffer, "\n\n" );

  //## end ClNewCancelPDU::Dump%965046702.body
}

m4return_t ClNewCancelPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClNewCancelPDU::GetBinaryHead%965046703.body preserve=yes
	m4uint32_t lnCont=0;
  
	ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32(  m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(  m_uiRequestID2Cancel);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( m_ulNBlocks  > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
  //## end ClNewCancelPDU::GetBinaryHead%965046703.body
}

void ClNewCancelPDU::SetCancelRequestId (m4uint32_t ai_ulCancelRequestId)
{
  //## begin ClNewCancelPDU::SetCancelRequestId%965046704.body preserve=yes
	m_uiRequestID2Cancel=ai_ulCancelRequestId;
  //## end ClNewCancelPDU::SetCancelRequestId%965046704.body
}

m4uint32_t ClNewCancelPDU::GetCancelRequestId ()
{
  //## begin ClNewCancelPDU::GetCancelRequestId%965046705.body preserve=yes
	return m_uiRequestID2Cancel;
  //## end ClNewCancelPDU::GetCancelRequestId%965046705.body
}

// Additional Declarations
  //## begin ClNewCancelPDU%39856E0002FC.declarations preserve=yes
  //## end ClNewCancelPDU%39856E0002FC.declarations

// Class ClNewErrorPDU 


ClNewErrorPDU::ClNewErrorPDU (m4int_t ai_iVersion)
  //## begin ClNewErrorPDU::ClNewErrorPDU%965046735.hasinit preserve=no
      : m_ulErrorCode(0)
  //## end ClNewErrorPDU::ClNewErrorPDU%965046735.hasinit
  //## begin ClNewErrorPDU::ClNewErrorPDU%965046735.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion)
  //## end ClNewErrorPDU::ClNewErrorPDU%965046735.initialization
{
  //## begin ClNewErrorPDU::ClNewErrorPDU%965046735.body preserve=yes
	m_uiId = M4_ID_PDU_NEWERROR;
  //## end ClNewErrorPDU::ClNewErrorPDU%965046735.body
}


ClNewErrorPDU::~ClNewErrorPDU ()
{
  //## begin ClNewErrorPDU::~ClNewErrorPDU%965046736.body preserve=yes
  //## end ClNewErrorPDU::~ClNewErrorPDU%965046736.body
}



//## Other Operations (implementation)
m4uint32_t ClNewErrorPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClNewErrorPDU::GetNextSizeBlock%965046737.body preserve=yes
	m4int32_t tam=0;
	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
    	break;
	case M4_PDUSTATE_HEAD2:
		tam = m_ulNBlocks * 4;
		if (tam)
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;

	default:
		tam = 0;
	}
	return tam;
  //## end ClNewErrorPDU::GetNextSizeBlock%965046737.body
}

m4return_t ClNewErrorPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClNewErrorPDU::AddBlock%965046738.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	m4uint32_t  ulBlockSize = 0, ulCont=0; 

	m_oSerializer .SetpBuffer (ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		m_ulErrorCode =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
        
		m_ulNBlocks= m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEAD2;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
	
		m_poBlockFormats -> InitializeIteration();

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;
	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}	
  //## end ClNewErrorPDU::AddBlock%965046738.body
}

m4uint32_t ClNewErrorPDU::GetHeadSize ()
{
  //## begin ClNewErrorPDU::GetHeadSize%965046740.body preserve=yes
	m4uint32_t tam = ClPDU::GetHeadSize()+M4_PDU_ALIGN_SIZE*3;  // RequestID & ErrorCode & NBlocks
	return (tam) ;
  //## end ClNewErrorPDU::GetHeadSize%965046740.body
}

void ClNewErrorPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClNewErrorPDU::Dump%965046743.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer," Dump de ClNewErrorPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);

   m4char_t	aData[32];

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n m_ulErrorCode. ");
   sprintf( aData, "%lu", m_ulErrorCode );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n\n" );

  //## end ClNewErrorPDU::Dump%965046743.body
}

m4uint32_t ClNewErrorPDU::GetErrorCode ()
{
  //## begin ClNewErrorPDU::GetErrorCode%965046744.body preserve=yes
	return m_ulErrorCode;
  //## end ClNewErrorPDU::GetErrorCode%965046744.body
}

void ClNewErrorPDU::SetErrorCode (m4uint32_t ai_ulErrorCode)
{
  //## begin ClNewErrorPDU::SetErrorCode%965046745.body preserve=yes
	m_ulErrorCode = ai_ulErrorCode;
  //## end ClNewErrorPDU::SetErrorCode%965046745.body
}

m4return_t ClNewErrorPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClNewErrorPDU::GetBinaryHead%965046746.body preserve=yes
	m4uint32_t lnCont=0;
  
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32(  m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(  m_ulErrorCode );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(m_ulNBlocks);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
  //## end ClNewErrorPDU::GetBinaryHead%965046746.body
}

// Additional Declarations
  //## begin ClNewErrorPDU%39856E9502ED.declarations preserve=yes
  //## end ClNewErrorPDU%39856E9502ED.declarations

// Class ClNewChangePasswordPDU 



ClNewChangePasswordPDU::ClNewChangePasswordPDU (m4int_t ai_iVersion)
  //## begin ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.hasinit preserve=no
  //## end ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.hasinit
  //## begin ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.initialization preserve=yes
  :ClPDUWithData(ai_iVersion)
  //## end ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.initialization
{
  //## begin ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.body preserve=yes
	m_uiId = M4_ID_PDU_NEWCHANGEPASSWORD;
  //## end ClNewChangePasswordPDU::ClNewChangePasswordPDU%965046706.body
}


ClNewChangePasswordPDU::~ClNewChangePasswordPDU ()
{
  //## begin ClNewChangePasswordPDU::~ClNewChangePasswordPDU%965046707.body preserve=yes
  //## end ClNewChangePasswordPDU::~ClNewChangePasswordPDU%965046707.body
}



//## Other Operations (implementation)
m4uint32_t ClNewChangePasswordPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClNewChangePasswordPDU::GetNextSizeBlock%965046708.body preserve=yes
	m4int32_t tam=0;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize() - M4_PDU_IDPDU_SIZE; // Ya hemos contado los 8 del ID_PDU
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD2:	
		tam = m_oCurrentPassword.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD3:	
    	tam = m_oNewPassword.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD4:
		tam = m_ulNBlocks * 4;
		if (tam) 
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		tam = 0;
	}
	return tam;
  //## end ClNewChangePasswordPDU::GetNextSizeBlock%965046708.body
}

m4return_t ClNewChangePasswordPDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClNewChangePasswordPDU::AddBlock%965046709.body preserve=yes
	m4uint32_t ulServiceHeadSize = 0,ulReturnCoden = 0, ulSize=0, ulBlockSize = 0, ulCont=0; 
	m4pchar_t pBlockTemp= ai_pBlock;
	m4bool_t	bEncrypted=M4_FALSE;

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU
		
		m_oSerializer .SetpBuffer(ai_pBlock);
	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClNewChangePasswordPDU
         
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_oCurrentPassword.m_iSize=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_oNewPassword.m_iSize=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulNBlocks    =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	    delete []ai_pBlock;
		if (m_oCurrentPassword.m_iSize  !=0)		
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
		    if (m_oNewPassword.m_iSize!=0)				
				m_uiState = M4_PDUSTATE_HEAD3; 
			else		
				m_uiState = M4_PDUSTATE_HEAD4;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		m_oCurrentPassword.m_pChar = ai_pBlock;
	    if (m_oNewPassword.m_iSize!=0)     
			m_uiState = M4_PDUSTATE_HEAD3; 
		else	
			m_uiState = M4_PDUSTATE_HEAD4;
		return M4_SUCCESS;
		break;
    
	case M4_PDUSTATE_HEAD3:
		m_oNewPassword.m_pChar = ai_pBlock;
		m_uiState = M4_PDUSTATE_HEAD4;
		return M4_SUCCESS;
		break;

	case M4_PDUSTATE_HEAD4:
		m_oSerializer .SetpBuffer(ai_pBlock);
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
		m_uiState = M4_PDUSTATE_HEADFULL;
		m_poBlockFormats -> InitializeIteration();
		return M4_SUCCESS;
		break;

	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
	    return M4_SUCCESS;		
		break;

	default:
		return M4_ERROR;
	}
  //## end ClNewChangePasswordPDU::AddBlock%965046709.body
}

m4uint32_t ClNewChangePasswordPDU::GetHeadSize ()
{
  //## begin ClNewChangePasswordPDU::GetHeadSize%965046711.body preserve=yes
	m4uint32_t tam = ClPDU::GetHeadSize()+ 
		M4_PDU_ALIGN_SIZE * 4 +  // RequestID & CurrentPassword.size & NewPassword.size & NBlocks
		m_oCurrentPassword.m_iSize+ 
		m_oNewPassword.m_iSize;
	return (tam);
  //## end ClNewChangePasswordPDU::GetHeadSize%965046711.body
}

void ClNewChangePasswordPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClNewChangePasswordPDU::Dump%965046714.body preserve=yes
 	if ( m_szDump != NULL )
		delete [] m_szDump;
   m4pchar_t auxbuffer;
   ai_strBuffer = new m4char_t[100000];  

   strcpy(ai_strBuffer," Dump de ClNewChangePasswordPDU. \n\n Atributos. ");
   auxbuffer=ai_strBuffer+strlen(ai_strBuffer);
   ClPDU::Dump(auxbuffer);

   sprintf(ai_strBuffer+strlen(ai_strBuffer), "\n m_ulRequestID. %lu", m_ulRequestID );

   if (m_oCurrentPassword.m_iSize!=0) 
	 sprintf(ai_strBuffer+strlen(ai_strBuffer), "\n\n m_oCurrentPassword. \n Tamaño. %u\n Cadena. %s",m_oCurrentPassword.m_iSize,m_oCurrentPassword.m_pChar);
   
  
   if (m_oNewPassword.m_iSize!=0) 
	sprintf(ai_strBuffer+strlen(ai_strBuffer), "\n\n m_oNewPassword. \n Tamaño. %u\n Cadena. %s",m_oNewPassword.m_iSize,m_oNewPassword.m_pChar);

   ClPDUWithData::Dump(m_szDump);
  
   strcat (ai_strBuffer, "\n\n");
  //## end ClNewChangePasswordPDU::Dump%965046714.body
}

m4uint16_t ClNewChangePasswordPDU::GetCurrentPassword (m4pchar_t &ao_pPass)
{
  //## begin ClNewChangePasswordPDU::GetCurrentPassword%965046715.body preserve=yes
 	ao_pPass = m_oCurrentPassword.m_pChar;
	return m_oCurrentPassword.m_iSize;
  //## end ClNewChangePasswordPDU::GetCurrentPassword%965046715.body
}

void ClNewChangePasswordPDU::SetCurrentPassword (m4pchar_t ai_pPass, m4uint16_t ai_uiPassSize)
{
  //## begin ClNewChangePasswordPDU::SetCurrentPassword%965046716.body preserve=yes
	int newSize=ai_uiPassSize;
	if(newSize%4)
		newSize+=4-newSize%4;
	m_oCurrentPassword.m_pChar = new m4char_t[newSize];
	memset(m_oCurrentPassword.m_pChar ,0,newSize);
	memcpy(m_oCurrentPassword.m_pChar,ai_pPass,ai_uiPassSize);
	m_oCurrentPassword.m_iSize = newSize;
  //## end ClNewChangePasswordPDU::SetCurrentPassword%965046716.body
}

m4uint16_t ClNewChangePasswordPDU::GetNewPassword (m4pchar_t &ao_pPassword)
{
  //## begin ClNewChangePasswordPDU::GetNewPassword%965046717.body preserve=yes
	ao_pPassword = m_oNewPassword.m_pChar;
	return m_oNewPassword.m_iSize;
  //## end ClNewChangePasswordPDU::GetNewPassword%965046717.body
}

void ClNewChangePasswordPDU::SetNewPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength)
{
  //## begin ClNewChangePasswordPDU::SetNewPassword%965046718.body preserve=yes
	int newSize=ai_uiPasswordLength;
	if(newSize%4)
		newSize+=4-newSize%4;
	m_oNewPassword.m_pChar = new m4char_t[newSize];
	memset(m_oNewPassword.m_pChar ,0,newSize);
	memcpy(m_oNewPassword.m_pChar,ai_pPassword,ai_uiPasswordLength);
	m_oNewPassword.m_iSize = newSize;
  //## end ClNewChangePasswordPDU::SetNewPassword%965046718.body
}

m4return_t ClNewChangePasswordPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClNewChangePasswordPDU::GetBinaryHead%965046719.body preserve=yes
	m4uint32_t lnCont=0;
	
	ao_ulBufferSize = GetHeadSize();
	ao_pBuffer = NULL;

    //	GetPDUSize();  Se hace en el GetBinaryHead
	ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_oCurrentPassword.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16( m_oNewPassword.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if (m_oCurrentPassword.m_iSize!=0)
		m_oSerializer.AddpBufferC( m_oCurrentPassword.m_pChar, m_oCurrentPassword.m_iSize );

	if (m_oNewPassword.m_iSize!=0) 
		m_oSerializer.AddpBufferC( m_oNewPassword.m_pChar,m_oNewPassword.m_iSize );

	if ( m_ulNBlocks  > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;
  //## end ClNewChangePasswordPDU::GetBinaryHead%965046719.body
}

// Additional Declarations
  //## begin ClNewChangePasswordPDU%39856ED60079.declarations preserve=yes
  //## end ClNewChangePasswordPDU%39856ED60079.declarations

// Class ClNewDisconnectPDU 



ClNewDisconnectPDU::ClNewDisconnectPDU (m4int_t ai_iVersion)
  //## begin ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.hasinit preserve=no
      : m_hUserId(0)
  //## end ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.hasinit
  //## begin ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion)
  //## end ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.initialization
{
  //## begin ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.body preserve=yes
	m_uiId = M4_ID_PDU_NEWDISCONNECT;
	m_uiWhy = M4_ERROR;
  //## end ClNewDisconnectPDU::ClNewDisconnectPDU%965046720.body
}


ClNewDisconnectPDU::~ClNewDisconnectPDU ()
{
  //## begin ClNewDisconnectPDU::~ClNewDisconnectPDU%965046721.body preserve=yes
  //## end ClNewDisconnectPDU::~ClNewDisconnectPDU%965046721.body
}



//## Other Operations (implementation)
m4uint32_t ClNewDisconnectPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClNewDisconnectPDU::GetNextSizeBlock%965046722.body preserve=yes
	m4int32_t tam=0;
	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE ;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD2:
		tam = m_ulNBlocks * 4;
		if (tam)
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		tam = 0;
	}
	return tam;
  //## end ClNewDisconnectPDU::GetNextSizeBlock%965046722.body
}

m4return_t ClNewDisconnectPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClNewDisconnectPDU::AddBlock%965046723.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	m4uint32_t  ulBlockSize = 0, ulCont=0; 

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
		m_oSerializer .SetpBuffer (ai_pBlock);
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClDisconnectPDU
		
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_hUserId=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		m_uiWhy=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulNBlocks=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEAD2;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		m_oSerializer .SetpBuffer (ai_pBlock);
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
	
		m_poBlockFormats -> InitializeIteration();

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;
	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}

  //## end ClNewDisconnectPDU::AddBlock%965046723.body
}

m4uint32_t ClNewDisconnectPDU::GetHeadSize ()
{
  //## begin ClNewDisconnectPDU::GetHeadSize%965046725.body preserve=yes
    m4uint32_t tam=ClPDU::GetHeadSize()+ 4 * M4_PDU_ALIGN_SIZE; // ReqId & UserId & Why & NBlocks
	return (tam) ;
  //## end ClNewDisconnectPDU::GetHeadSize%965046725.body
}

void ClNewDisconnectPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClNewDisconnectPDU::Dump%965046728.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer,"");	
   strcat(ai_strBuffer," Dump de ClNewDisConnectPDU. \n\n Atributos. ");

   ClPDU::Dump( ai_strBuffer );

   m4char_t	aData[32];

   strcat(ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(ai_strBuffer, aData);

   strcat(ai_strBuffer, "\n m_hUserId. ");
   sprintf( aData, "%lu", m_hUserId );
   strcat(ai_strBuffer, aData);

   strcat(ai_strBuffer, "\n m_uiWhy. ");
   sprintf( aData, "%u", m_uiWhy );
   strcat(ai_strBuffer, aData);

   strcat (ai_strBuffer, "\n\n");

   	ClPDUWithData::Dump(ai_strBuffer);

   strcat(ai_strBuffer, "\n\n");
  //## end ClNewDisconnectPDU::Dump%965046728.body
}

m4uint16_t ClNewDisconnectPDU::GetWhy ()
{
  //## begin ClNewDisconnectPDU::GetWhy%965046729.body preserve=yes
	return m_uiWhy;
  //## end ClNewDisconnectPDU::GetWhy%965046729.body
}

void ClNewDisconnectPDU::SetWhy (m4uint16_t ai_uiWhy)
{
  //## begin ClNewDisconnectPDU::SetWhy%965046730.body preserve=yes
	m_uiWhy = ai_uiWhy;
  //## end ClNewDisconnectPDU::SetWhy%965046730.body
}

void ClNewDisconnectPDU::SetUserId (m4handle_t ai_hUserId)
{
  //## begin ClNewDisconnectPDU::SetUserId%965046731.body preserve=yes
	     m_hUserId = ai_hUserId;
  //## end ClNewDisconnectPDU::SetUserId%965046731.body
}

m4handle_t ClNewDisconnectPDU::GetUserId ()
{
  //## begin ClNewDisconnectPDU::GetUserId%965046732.body preserve=yes
	return m_hUserId;
  //## end ClNewDisconnectPDU::GetUserId%965046732.body
}

m4return_t ClNewDisconnectPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClNewDisconnectPDU::GetBinaryHead%965046733.body preserve=yes

	m4uint32_t	ulPos = 0;

	ao_ulBufferSize = 0;
	// GetPDUSize();	Se hace en el GetBinaryHead
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) == M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}

	m_oSerializer.AddUInt32(m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32(m_hUserId );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16(m_uiWhy );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32(m_ulNBlocks);
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	return M4_SUCCESS;
  //## end ClNewDisconnectPDU::GetBinaryHead%965046733.body
}

// Additional Declarations
  //## begin ClNewDisconnectPDU%39856F730179.declarations preserve=yes
  //## end ClNewDisconnectPDU%39856F730179.declarations

// Class ClNewSetRolePDU 



ClNewSetRolePDU::ClNewSetRolePDU (m4int_t ai_iVersion)
  //## begin ClNewSetRolePDU::ClNewSetRolePDU%965046748.hasinit preserve=no
      : m_bDefaultRole(M4_FALSE)
  //## end ClNewSetRolePDU::ClNewSetRolePDU%965046748.hasinit
  //## begin ClNewSetRolePDU::ClNewSetRolePDU%965046748.initialization preserve=yes
  , ClPDUWithData(ai_iVersion)
  //## end ClNewSetRolePDU::ClNewSetRolePDU%965046748.initialization
{
  //## begin ClNewSetRolePDU::ClNewSetRolePDU%965046748.body preserve=yes
	  m_uiId = M4_ID_PDU_NEWSETROLE;
  //## end ClNewSetRolePDU::ClNewSetRolePDU%965046748.body
}


ClNewSetRolePDU::~ClNewSetRolePDU ()
{
  //## begin ClNewSetRolePDU::~ClNewSetRolePDU%965046749.body preserve=yes
  //## end ClNewSetRolePDU::~ClNewSetRolePDU%965046749.body
}



//## Other Operations (implementation)
m4uint32_t ClNewSetRolePDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClNewSetRolePDU::GetNextSizeBlock%965046750.body preserve=yes
 	m4int32_t tam=0;
	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;	 
		ao_pBlock = new m4char_t[tam];
    	break;
	case M4_PDUSTATE_HEAD2:	
		tam = m_oRole.m_iSize;
		ao_pBlock = new m4char_t[tam];
		break;
	case M4_PDUSTATE_HEAD3:
		tam = m_ulNBlocks * 4;
		if (tam)
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		tam = 0;
	}
	return tam;

  //## end ClNewSetRolePDU::GetNextSizeBlock%965046750.body
}

m4return_t ClNewSetRolePDU::AddBlock (m4pchar_t &ai_pBlock)
{
  //## begin ClNewSetRolePDU::AddBlock%965046751.body preserve=yes
	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	m4uint32_t  ulBlockSize = 0, ulCont=0; 

	m_oSerializer .SetpBuffer(ai_pBlock);
	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)	
		   return M4_ERROR;
    
	// Ejecuto el AddDataBlock propio de ClDisconnectPDU
 
		m_ulRequestID=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_bDefaultRole=m_oSerializer.GetUInt32()!=0?M4_TRUE:M4_FALSE;
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_oRole.m_iSize=m_oSerializer.GetUInt16();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

		m_ulNBlocks=m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		
        delete  []ai_pBlock;

		if (m_oRole.m_iSize  !=0)		
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
			m_uiState = M4_PDUSTATE_HEAD3; 

		return M4_SUCCESS;		
		break;
	
	case M4_PDUSTATE_HEAD2:
		
		m_oRole.m_pChar = pBlockTemp;
		
		m_uiState = M4_PDUSTATE_HEAD3;
		return M4_SUCCESS;
		break;	
	case M4_PDUSTATE_HEAD3:
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
	
		m_poBlockFormats -> InitializeIteration();

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;

	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;

	default:
		return M4_ERROR;
	}

  //## end ClNewSetRolePDU::AddBlock%965046751.body
}

m4uint32_t ClNewSetRolePDU::GetHeadSize ()
{
  //## begin ClNewSetRolePDU::GetHeadSize%965046753.body preserve=yes
    m4uint32_t tam= ClPDU::GetHeadSize()+ 4* M4_PDU_ALIGN_SIZE+  // ReaID & DefaultRole &  RoleSize & NBlocks
			m_oRole.m_iSize;
	return (tam) ;
  //## end ClNewSetRolePDU::GetHeadSize%965046753.body
}

void ClNewSetRolePDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClNewSetRolePDU::Dump%965046756.body preserve=yes
	if ( m_szDump != NULL )
		delete [] m_szDump;

   m_szDump = new m4char_t[1000];  
   strcpy(m_szDump," Dump de ClNewSetRolePDU. \n\n Atributos. ");
   
   ClPDU::Dump(m_szDump);

   m4char_t	aData[32];
   m4pchar_t  pcad = NULL, paux=NULL; 
   m4uint16_t tam;

   strcat(m_szDump, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(m_szDump, aData);

   strcat(m_szDump, "\n m_bDefaultRole. ");
   sprintf( aData, "%u", m_bDefaultRole );
   strcat(m_szDump, aData);

   tam  = m_oRole.m_iSize;
   pcad = m_oRole.m_pChar;

   if (tam!=0) 
   {
     paux = new m4char_t[tam+1];
     paux[tam]=0;
     memcpy(paux,pcad,tam);

	 strcat (m_szDump, "\n\n m_oRole. ");

     strcat (m_szDump, "\n Tamaño. ");
     sprintf( aData, "%u", tam  );
     strcat (m_szDump, aData);
	 
     strcat (m_szDump, "\n Cadena. ");
     strcat (m_szDump, paux);

	 delete []paux;
   }

	strcat (ai_strBuffer, "\n\n");
	
	ClPDUWithData::Dump(ai_strBuffer);
   strcat (m_szDump, "\n\n" );
   ai_strBuffer = m_szDump;
  //## end ClNewSetRolePDU::Dump%965046756.body
}

m4return_t ClNewSetRolePDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClNewSetRolePDU::GetBinaryHead%965046757.body preserve=yes
  	m4uint32_t lnCont=0;
	
	ao_ulBufferSize = GetHeadSize();

	//GetPDUSize(); Se hace en el getBinaryHead
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_bDefaultRole );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt16( m_oRole.m_iSize );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero (M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if (m_oRole.m_iSize!=0)
		m_oSerializer.AddpBufferC( m_oRole.m_pChar, m_oRole.m_iSize );

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
  //## end ClNewSetRolePDU::GetBinaryHead%965046757.body
}

m4uint16_t ClNewSetRolePDU::GetRole (m4pchar_t &ao_pRoleName)
{
  //## begin ClNewSetRolePDU::GetRole%965046758.body preserve=yes
	ao_pRoleName = m_oRole.m_pChar;
	return m_oRole.m_iSize;
  //## end ClNewSetRolePDU::GetRole%965046758.body
}

void ClNewSetRolePDU::SetRole (m4pchar_t ai_pRoleName, m4uint16_t ai_uiRoleLength)
{
  //## begin ClNewSetRolePDU::SetRole%965046759.body preserve=yes
	int newSize=(int)ai_uiRoleLength;
	if(newSize%4)
		newSize+=4-newSize%4;
	m_oRole.m_pChar = new m4char_t[newSize];
	memset(m_oRole.m_pChar,0,newSize);
	memcpy(m_oRole.m_pChar,ai_pRoleName,ai_uiRoleLength);
	m_oRole.m_iSize = newSize;
  //## end ClNewSetRolePDU::SetRole%965046759.body
}

void ClNewSetRolePDU::SetDefaultRole (m4bool_t ai_bDefaultRole)
{
  //## begin ClNewSetRolePDU::SetDefaultRole%965046760.body preserve=yes
	m_bDefaultRole = ai_bDefaultRole;
  //## end ClNewSetRolePDU::SetDefaultRole%965046760.body
}

m4bool_t ClNewSetRolePDU::IsDefaultRole ()
{
  //## begin ClNewSetRolePDU::IsDefaultRole%965046761.body preserve=yes
  	if (m_bDefaultRole == M4_TRUE ) 
		return M4_TRUE;
	else
		return M4_FALSE;
  //## end ClNewSetRolePDU::IsDefaultRole%965046761.body
}

// Additional Declarations
  //## begin ClNewSetRolePDU%39856FB90260.declarations preserve=yes
  //## end ClNewSetRolePDU%39856FB90260.declarations

//## begin module%3468A65001C9.epilog preserve=yes
//## end module%3468A65001C9.epilog



// Class ClPingPDU 


ClPingPDU::ClPingPDU (m4int_t ai_iVersion)
      : m_uiElapsedTime(0),ClPDUWithData(ai_iVersion)
{
	m_uiId = M4_ID_PDU_PING;
}


ClPingPDU::~ClPingPDU ()
{

}



m4uint32_t ClPingPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
	m4int32_t tam=0;
	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize () - M4_PDU_IDPDU_SIZE;
		ao_pBlock = new m4char_t[tam];
    	break;
	case M4_PDUSTATE_HEAD2:
		tam = m_ulNBlocks * 4;
		if (tam)
			ao_pBlock = new m4char_t[tam];
		return tam;
		break;
	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;

	default:
		tam = 0;
	}
	return tam;
}

m4return_t ClPingPDU::AddBlock (m4pchar_t &ai_pBlock)
{
	m4pchar_t pBlockTemp= ai_pBlock;

	m4uint32_t	ulSize = 0 , modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;

	m4uint32_t  ulBlockSize = 0, ulCont=0; 

	m_oSerializer .SetpBuffer (ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:
	
    // Ejecuto el AddDataBlock propio de ClPDU

	   if (ClPDU::AddBlock(ai_pBlock)== M4_ERROR)
		   return M4_ERROR;

    // Ejecuto el AddDataBlock propio de ClPingPDU
 
		m_ulRequestID = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		m_uiElapsedTime = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
        
		m_ulNBlocks = m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiState = M4_PDUSTATE_HEAD2;
        delete  []ai_pBlock;

		return M4_SUCCESS;		
		break;
	case M4_PDUSTATE_HEAD2:
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32();
			m_poBlockFormats -> AddFormat( ulBlockSize );
		}
		delete [] ai_pBlock;
	
		m_poBlockFormats -> InitializeIteration();

		m_uiState = M4_PDUSTATE_HEADFULL;
		return M4_SUCCESS;
		break;
	case M4_PDUSTATE_HEADFULL:
		m_poBlockFormats -> GetNext( ulSize, bEncrypted );
		m_poDataStorage -> CheckDataStorage( ai_pBlock, ulSize, bEncrypted );
		return M4_SUCCESS;
		break;
	default:
		return M4_ERROR;
	}	
}

m4uint32_t ClPingPDU::GetHeadSize ()
{
	m4uint32_t tam = ClPDU::GetHeadSize()+M4_PDU_ALIGN_SIZE*3;  // RequestID & ErrorCode & NBlocks
	return (tam) ;
}

void ClPingPDU::Dump (m4pchar_t &ai_strBuffer)
{
	if ( m_szDump != NULL )
		delete [] m_szDump;

   ai_strBuffer = new m4char_t[1000];  
   strcpy(ai_strBuffer," Dump de ClNewErrorPDU. \n\n Atributos. ");
   
   ClPDU::Dump(ai_strBuffer);

   m4char_t	aData[32];

   strcat (ai_strBuffer, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n m_uiElapsedTime. ");
   sprintf( aData, "%lu", m_uiElapsedTime );
   strcat (ai_strBuffer, aData );

   strcat (ai_strBuffer, "\n\n" );
}

m4uint32_t ClPingPDU::GetElapsedTime ()
{
	return m_uiElapsedTime;
}

void ClPingPDU::SetElapsedTime (m4uint32_t ai_uiElapsedTime)
{
	m_uiElapsedTime = ai_uiElapsedTime;
}

m4return_t ClPingPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
	m4uint32_t lnCont=0;
  
    ClPDU::GetBinaryHead(ao_pBuffer,lnCont);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero( M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE );

	m_oSerializer.AddUInt32( m_uiElapsedTime );
	m_oSerializer.AddZero( M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE );

	m_oSerializer.AddUInt32(m_ulNBlocks);
	m_oSerializer.AddZero( M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE );

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();
	
	return M4_SUCCESS;
}
