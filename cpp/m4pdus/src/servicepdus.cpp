//## begin module%3468A60E0336.cm preserve=no
//## end module%3468A60E0336.cm

//## begin module%3468A60E0336.cp preserve=no
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
//## end module%3468A60E0336.cp

//## Module: servicepdus%3468A60E0336; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\servicepdus.cpp

//## begin module%3468A60E0336.additionalIncludes preserve=no
//## end module%3468A60E0336.additionalIncludes

//## begin module%3468A60E0336.includes preserve=yes
#include <m4trace.hpp>
//## end module%3468A60E0336.includes

// servicepdus
#include <servicepdus.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%3468A60E0336.declarations preserve=no
//## end module%3468A60E0336.declarations

//## begin module%3468A60E0336.additionalDeclarations preserve=yes

m4uint32_t M4PDUAllocaMem(int ai_iSize,char *&ao_pcBuffer)
{
	ao_pcBuffer=new char[ai_iSize];
	if(NULL==ao_pcBuffer)
	{
		m4Trace(cerr << "Error allocando buffer. Size:"<<ai_iSize<<endl);
		return (m4uint32_t)-1;

	}
	else
		return ai_iSize;
}
//## end module%3468A60E0336.additionalDeclarations


// Class ClReplyPDU 




ClReplyPDU::ClReplyPDU (m4int_t ai_iVersion)
  //## begin ClReplyPDU::ClReplyPDU%875780549.hasinit preserve=no
      : m_ulServiceId(0), m_ulOperationId(0)
  //## end ClReplyPDU::ClReplyPDU%875780549.hasinit
  //## begin ClReplyPDU::ClReplyPDU%875780549.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion)
  //## end ClReplyPDU::ClReplyPDU%875780549.initialization
{
  //## begin ClReplyPDU::ClReplyPDU%875780549.body preserve=yes
	m_uiId = M4_ID_PDU_REPLY;
	m_uiType = M4_TYPE_PDU_SERVICE;
	m_ulReturnCode = 4;
  //## end ClReplyPDU::ClReplyPDU%875780549.body
}


ClReplyPDU::~ClReplyPDU ()
{
  //## begin ClReplyPDU::~ClReplyPDU%878377096.body preserve=yes
  //## end ClReplyPDU::~ClReplyPDU%878377096.body
}



//## Other Operations (implementation)
m4uint32_t ClReplyPDU::GetHeadSize ()
{
  //## begin ClReplyPDU::GetHeadSize%876211524.body preserve=yes
//	ULong	 m_ulOperationId 
//	ULong	 m_ulRequestId 
//	ULong	 m_ulReturnCode 
//	ULong	 m_ulServiceId 
//	ULong	 ulNBlocks 

	return ( 40 + ClPDU::GetHeadSize() ); 
  //## end ClReplyPDU::GetHeadSize%876211524.body
}

void ClReplyPDU::Dump (m4pchar_t  &ai_strBuffer)
{
  //## begin ClReplyPDU::Dump%878139980.body preserve=yes
   
	if ( m_szDump != NULL )
		delete [] m_szDump;

  if ( m_szDump != NULL ) 
	  delete m_szDump;

  m_szDump = new m4char_t[100000];  
  strcpy(m_szDump,"");	
  strcat(m_szDump," Dump de ClReplyPDU. \n\n Atributos. ");
   
   ClPDU::Dump(m_szDump);

   m4char_t	    aData[32];
   
   strcat(m_szDump,"\n m_ulOperationId. ");
   sprintf( aData, "%lu", m_ulOperationId );
   strcat(m_szDump,aData);
   
   strcat(m_szDump, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(m_szDump, aData);

   strcat(m_szDump, "\n m_ulReturnCode. ");
   sprintf( aData, "%lu", m_ulReturnCode );
   strcat(m_szDump, aData);
   
   strcat(m_szDump, "\n m_ulServiceId. ");
   sprintf( aData, "%lu", m_ulServiceId );
   strcat(m_szDump, aData);
   
   ClPDUWithData::Dump(m_szDump);
	
   strcat(m_szDump, "\n\n");

   ai_strBuffer = m_szDump;

  //## end ClReplyPDU::Dump%878139980.body
}

m4return_t ClReplyPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClReplyPDU::AddBlock%878468345.body preserve=yes

	m4uint32_t ulSize=0, ulBlockSize = 0, ulCont=0; 
	m4bool_t	bEncrypted;

	m_oSerializer.SetpBuffer(ai_pBlock);
    switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:

		if ( ClPDU::AddBlock( ai_pBlock ) == M4_ERROR ) 
			return M4_ERROR;

		m_ulOperationId=m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulRequestID  =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulReturnCode =m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulServiceId  =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulNBlocks    =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
		
		delete [] ai_pBlock;  
		
		m_uiState = M4_PDUSTATE_HEAD2;
		return M4_SUCCESS;		
		break;

	case M4_PDUSTATE_HEAD2:
	
		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32 ();
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

	  
  //## end ClReplyPDU::AddBlock%878468345.body
}

m4return_t ClReplyPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClReplyPDU::GetBinaryHead%878662676.body preserve=yes
	m4uint32_t	ulPos = 0,
			ulNBlocks = 0;

	m4pchar_t	pBuffer = NULL;

	ao_ulBufferSize =  GetHeadSize()+ m_poBlockFormats -> GetFormatsSize();

	ao_pBuffer = NULL;
	//	GetPDUSize(); Se llama desde GetBinaryHead
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) 	== M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}

	m_oSerializer.AddUInt32( m_ulOperationId );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulReturnCode );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulServiceId );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);
	
	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;
  //## end ClReplyPDU::GetBinaryHead%878662676.body
}

m4uint32_t ClReplyPDU::GetReturnCode ()
{
  //## begin ClReplyPDU::GetReturnCode%878723090.body preserve=yes
	return m_ulReturnCode;
  //## end ClReplyPDU::GetReturnCode%878723090.body
}

void ClReplyPDU::SetReturnCode (m4uint32_t ai_ulReturnCode)
{
  //## begin ClReplyPDU::SetReturnCode%878723091.body preserve=yes
	m_ulReturnCode = ai_ulReturnCode;
  //## end ClReplyPDU::SetReturnCode%878723091.body
}

m4uint32_t ClReplyPDU::GetServiceId ()
{
  //## begin ClReplyPDU::GetServiceId%881934553.body preserve=yes
  
   return m_ulServiceId;
	
  //## end ClReplyPDU::GetServiceId%881934553.body
}

void ClReplyPDU::SetServiceId (m4uint32_t ai_ulServiceId)
{
  //## begin ClReplyPDU::SetServiceId%881934554.body preserve=yes
  
	 m_ulServiceId = ai_ulServiceId;
  //## end ClReplyPDU::SetServiceId%881934554.body
}

m4uint32_t ClReplyPDU::GetOperationId ()
{
  //## begin ClReplyPDU::GetOperationId%881934555.body preserve=yes
 return m_ulOperationId;
  //## end ClReplyPDU::GetOperationId%881934555.body
}

void ClReplyPDU::SetOperationId (m4uint32_t ai_ulOperationId)
{
  //## begin ClReplyPDU::SetOperationId%881934556.body preserve=yes
  
   m_ulOperationId = ai_ulOperationId;
	
  //## end ClReplyPDU::SetOperationId%881934556.body
}

m4uint32_t ClReplyPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClReplyPDU::GetNextSizeBlock%881934560.body preserve=yes
 	
	m4uint32_t	ulSize = 0, tam = 0, modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize() - M4_PDU_IDPDU_SIZE;
		//ao_pBlock = new m4char_t[tam];
		//return tam;
		return M4PDUAllocaMem(tam,ao_pBlock);
		break;
	case M4_PDUSTATE_HEAD2:
		m_ulNBlocks = GetNBlocks();
		tam = m_ulNBlocks * 4;
	//	ao_pBlock = new m4char_t[tam];
	//	return tam;

		if (tam) 
			return M4PDUAllocaMem(tam,ao_pBlock);
		else 
			return (m4uint32_t)-1;
		break;

	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		m4Trace(cerr << "Invalid state in pdu serialization"<<endl);
		return (m4uint32_t)-1;
	}

	return 0;
	
  //## end ClReplyPDU::GetNextSizeBlock%881934560.body
}

// Class ClRequestPDU 





ClRequestPDU::ClRequestPDU (m4int_t ai_iVersion)
  //## begin ClRequestPDU::ClRequestPDU%875692543.hasinit preserve=no
      : m_ulOperationId(0)
  //## end ClRequestPDU::ClRequestPDU%875692543.hasinit
  //## begin ClRequestPDU::ClRequestPDU%875692543.initialization preserve=yes
  ,ClPDUWithData(ai_iVersion),m_ulFlags(0),m_uiCharge(0)
  //## end ClRequestPDU::ClRequestPDU%875692543.initialization
{
  //## begin ClRequestPDU::ClRequestPDU%875692543.body preserve=yes
	m_uiId = M4_ID_PDU_REQUEST;
	m_uiType = M4_TYPE_PDU_SERVICE;
  //## end ClRequestPDU::ClRequestPDU%875692543.body
}


ClRequestPDU::~ClRequestPDU ()
{
  //## begin ClRequestPDU::~ClRequestPDU%878377095.body preserve=yes
/*	char kk[256];
	sprintf(kk,"Del:%d\n",(int)this);
	cerr <<kk<<flush;
*/
  //## end ClRequestPDU::~ClRequestPDU%878377095.body
}



//## Other Operations (implementation)
m4uint32_t ClRequestPDU::GetHeadSize ()
{
  //## begin ClRequestPDU::GetHeadSize%876211523.body preserve=yes

	// ULong	m_ulServiceId.m_lSize 
	// ULong	m_ulOperationId 
	// ULong	m_ulRequestId 
	// ULong	m_ulFlags
	// UInt		m_uiCharge 
	// ULong	m_ulNBlocks 
	// Todos ocupan 8 bytes
	
	return ClPDU::GetHeadSize() + 6*M4_PDU_ALIGN_SIZE + m_ulServiceId.m_lSize;
  //## end ClRequestPDU::GetHeadSize%876211523.body
}

void ClRequestPDU::Dump (m4pchar_t &ai_strBuffer)
{
  //## begin ClRequestPDU::Dump%878139979.body preserve=yes
	
	m4pchar_t  paux = NULL, pcad = NULL;
	m4uint32_t ulNBlocks = 0, ulFormatSize = 0, lnCont=0, tam=0;
	m4uint8_t  lnCont2=0;

	if ( m_szDump != NULL )
		delete [] m_szDump;

	if ( m_szDump != NULL ) 
	   delete m_szDump;

   m_szDump = new m4char_t[100000];  
   strcpy(m_szDump,"");	
   strcat(m_szDump," Dump de ClRequestPDU. \n\n Atributos. ");


   ClPDU::Dump( m_szDump );
  

   m4char_t	aData[32];

   tam  = m_ulServiceId.m_lSize;
   pcad = m_ulServiceId.m_pChar;

   if (tam!=0) 
   {
     paux = new m4char_t[tam+1];
     paux[tam]=0;
     memcpy(paux,pcad,tam);

	 strcat (m_szDump, "\n\n m_ulServiceId. ");

     strcat (m_szDump, "\n Tamaño. ");
     sprintf( aData, "%u", tam  );
     strcat (m_szDump, aData);
	 
     strcat (m_szDump, "\n Cadena. ");
     strcat (m_szDump, paux);
	 strcat (m_szDump, "\n");	

	 delete []paux;
   }


   strcat(m_szDump, "\n m_ulOperationId. ");
   sprintf( aData, "%lu", m_ulOperationId );
   strcat(m_szDump, aData);

   strcat(m_szDump, "\n m_ulRequestID. ");
   sprintf( aData, "%lu", m_ulRequestID );
   strcat(m_szDump, aData);


   strcat(m_szDump, "\n m_ulFlags. ");
   sprintf( aData, "%lu", m_ulFlags );
   strcat(m_szDump, aData);

   strcat(m_szDump, "\n m_uiCharge. ");
   sprintf( aData, "%u", m_uiCharge );
   strcat(m_szDump, aData);
   
   ClPDUWithData::Dump( m_szDump );

   strcat(m_szDump, "\n\n");

   ai_strBuffer = m_szDump;

  //## end ClRequestPDU::Dump%878139979.body
}

m4return_t ClRequestPDU::AddBlock (m4pchar_t  &ai_pBlock)
{
  //## begin ClRequestPDU::AddBlock%878468344.body preserve=yes

	m4uint32_t 	ulSize = 0,
			ulBlockSize = 0; 

	m4uint16_t	ulCont = 0;

	m4bool_t	bEncrypted;

	m_oSerializer.SetpBuffer(ai_pBlock);

	switch ( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:

		if ( ClPDU::AddBlock( ai_pBlock ) == M4_ERROR ){
			return M4_ERROR;
		}

// Ejecuto el AddDataBlock propio de ClRequestPDU

		m_ulServiceId.m_lSize =m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulOperationId =m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulRequestID =m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_ulFlags =m_oSerializer.GetUInt32();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		m_uiCharge =m_oSerializer.GetUInt16 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);
		
		m_ulNBlocks =m_oSerializer.GetUInt32 ();
		m_oSerializer.SkipBytes(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

		delete [ ] ai_pBlock;  
		
		if (m_ulServiceId.m_lSize  !=0)	
			m_uiState = M4_PDUSTATE_HEAD2; 
		else
			m_uiState = M4_PDUSTATE_HEAD3;
		return M4_SUCCESS;		
		break;

	case M4_PDUSTATE_HEAD2:
		
		m_ulServiceId.m_pChar = ai_pBlock;
		
		m_uiState = M4_PDUSTATE_HEAD3;
		return M4_SUCCESS;
		break;	
	
	case M4_PDUSTATE_HEAD3:

		for ( ulCont = 0; ulCont < m_ulNBlocks ; ulCont ++ )
		{
			ulBlockSize =m_oSerializer.GetUInt32 ();
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

  //## end ClRequestPDU::AddBlock%878468344.body
}

m4return_t ClRequestPDU::GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize)
{
  //## begin ClRequestPDU::GetBinaryHead%878662675.body preserve=yes
	m4uint32_t	ulPos = 0, ln = 0;
	m4uint32_t	ulNBlocks = 0;
	m4pchar_t	pBuffer = NULL;

	ao_ulBufferSize = NULL;
	// GetPDUSize();  Se hace desde GetBinaryHead
	if ( ClPDU::GetBinaryHead( ao_pBuffer, ulPos ) 	== M4_ERROR )
	{	
		ao_ulBufferSize = ulPos;
		return M4_ERROR;
	}

	m_oSerializer.AddUInt32( m_ulServiceId.m_lSize );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulOperationId );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulRequestID );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt32( m_ulFlags );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	m_oSerializer.AddUInt16( m_uiCharge );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT16_SIZE);

	m_oSerializer.AddUInt32( m_ulNBlocks );
	m_oSerializer.AddZero(M4_PDU_ALIGN_SIZE-M4_PDUSERIALIZER_UINT32_SIZE);

	if (m_ulServiceId.m_lSize!=0) 
		m_oSerializer.AddpBufferC( m_ulServiceId.m_pChar, m_ulServiceId.m_lSize );
	
	if ( m_ulNBlocks > 0 )
		SerializeFormatsBlock();

	ao_pBuffer = m_oSerializer.GetSerializedBuffer();
	ao_ulBufferSize = m_oSerializer.GetBufferSize();

	return M4_SUCCESS;

  //## end ClRequestPDU::GetBinaryHead%878662675.body
}

m4uint32_t ClRequestPDU::GetFlags ()
{
  //## begin ClRequestPDU::GetFlags%878723086.body preserve=yes
	return m_ulFlags;
  //## end ClRequestPDU::GetFlags%878723086.body
}

void ClRequestPDU::SetFlags (m4uint32_t ai_ulFlags)
{
  //## begin ClRequestPDU::SetFlags%878723087.body preserve=yes
	m_ulFlags = ai_ulFlags;
  //## end ClRequestPDU::SetFlags%878723087.body
}

m4uint16_t ClRequestPDU::GetCharge ()
{
  //## begin ClRequestPDU::GetCharge%878723088.body preserve=yes
	return m_uiCharge;
  //## end ClRequestPDU::GetCharge%878723088.body
}

void ClRequestPDU::SetCharge (m4uint16_t ai_uiCharge)
{
  //## begin ClRequestPDU::SetCharge%878723089.body preserve=yes
	m_uiCharge = ai_uiCharge;
  //## end ClRequestPDU::SetCharge%878723089.body
}

m4uint32_t ClRequestPDU::GetServiceId (m4pchar_t &ao_pServiceName)
{
  //## begin ClRequestPDU::GetServiceId%880451467.body preserve=yes
	ao_pServiceName = m_ulServiceId.m_pChar;
	return m_ulServiceId.m_lSize;
  //## end ClRequestPDU::GetServiceId%880451467.body
}

void ClRequestPDU::SetServiceId (m4pchar_t ai_pServiceName, m4uint32_t ai_ulServiceNameLength)
{
  //## begin ClRequestPDU::SetServiceId%880451468.body preserve=yes
	
	// RAC 07.09.98
	// !!! CUIDADO CON ESTO.  !!!

	// Modifico internamente el tamaño de la cadena del indentificador de Servicio
	// para que no haya problemas con el alineamiento en maquinas SUN
	
	// Si m_ulServiceId.m_lSize no es multiplo de 4 ==> 
	// m_ulServiceId.m_lSize += (4-(m_ulServiceId.m_lSize%4))
	
	m_ulServiceId.m_lSize = ai_ulServiceNameLength;
	
	// Modificacion del tamaño.
	m4uint_t uiResto=m_ulServiceId.m_lSize%4;
	if (uiResto) 
		m_ulServiceId.m_lSize += (4-uiResto);
	
	m_ulServiceId.m_pChar = new m4char_t[m_ulServiceId.m_lSize];
	memcpy(m_ulServiceId.m_pChar,ai_pServiceName,ai_ulServiceNameLength);
	if(uiResto)
		memset(m_ulServiceId.m_pChar+ai_ulServiceNameLength,0,4-uiResto);
	
  //## end ClRequestPDU::SetServiceId%880451468.body
}

m4uint32_t ClRequestPDU::GetOperationId ()
{
  //## begin ClRequestPDU::GetOperationId%880451469.body preserve=yes
	return m_ulOperationId;
  //## end ClRequestPDU::GetOperationId%880451469.body
}

void ClRequestPDU::SetOperationId (m4uint32_t ai_ulOperationId)
{
  //## begin ClRequestPDU::SetOperationId%880451470.body preserve=yes
	m_ulOperationId = ai_ulOperationId;
  //## end ClRequestPDU::SetOperationId%880451470.body
}

m4uint32_t ClRequestPDU::GetNextSizeBlock (m4pchar_t &ao_pBlock)
{
  //## begin ClRequestPDU::GetNextSizeBlock%881934559.body preserve=yes
  	m4uint32_t	ulSize = 0, tam=0;
	m4uint32_t  modulo=0;
	m4bool_t	bEncrypted = M4_FALSE;
	switch( m_uiState )
	{
	case M4_PDUSTATE_HEAD1:	
		tam = GetHeadSize() - M4_PDU_IDPDU_SIZE;	
        //ao_pBlock = new m4char_t[tam];
		//return tam;
		return M4PDUAllocaMem(tam,ao_pBlock);
		break;
	case M4_PDUSTATE_HEAD2:	
		tam = m_ulServiceId.m_lSize;
		//ao_pBlock = new m4char_t[tam];
		//return tam;
		return M4PDUAllocaMem(tam,ao_pBlock);
		break;
	case M4_PDUSTATE_HEAD3:
		tam = m_ulNBlocks * 4;
		if (tam>1024000)
		{
			m4Trace(cerr<<"Invalid size in PDU:" <<tam<<endl);
			return (m4uint32_t)-1;
		}
		if (tam) 
			return M4PDUAllocaMem(tam,ao_pBlock);
		else
		{
			m4Trace(cerr<<"tam=0" <<endl);
			return (m4uint32_t)-1;
		}

/*		if ( (tam)  && (tam<1024))
			ao_pBlock = new m4char_t[tam];
		else
		{
			m4Trace(cerr<<"Invalid size in PDU:" <<tam<<endl);
			tam=-1;
		}
		
		return tam;
*/
		break;

	case M4_PDUSTATE_HEADFULL:
		return GetNextDataStorageChunkSize(ao_pBlock);
		break;
	default:
		m4Trace(cerr << "Invalid state in pdu serialization"<<endl);
		return (m4uint32_t)-1;
	}

	return 0;

	
  //## end ClRequestPDU::GetNextSizeBlock%881934559.body
}

//## begin module%3468A60E0336.epilog preserve=yes
//## end module%3468A60E0336.epilog


