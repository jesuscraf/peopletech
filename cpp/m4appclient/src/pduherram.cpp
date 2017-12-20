//## begin module%345F71E6008F.cm preserve=no
//## end module%345F71E6008F.cm

//## begin module%345F71E6008F.cp preserve=no
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
//## end module%345F71E6008F.cp

//## Module: PDUHerram%345F71E6008F; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\pduherram.cpp

//## begin module%345F71E6008F.additionalIncludes preserve=no
//## end module%345F71E6008F.additionalIncludes

//## begin module%345F71E6008F.includes preserve=yes
//## end module%345F71E6008F.includes

// pduherram
#include <pduherram.hpp>
// tsap
#include <tsap.hpp>
// Protocol
#include <protocol.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%345F71E6008F.declarations preserve=no
//## end module%345F71E6008F.declarations

//## begin module%345F71E6008F.additionalDeclarations preserve=yes
//## end module%345F71E6008F.additionalDeclarations


// Class ClGetPDU 


ClGetPDU::ClGetPDU (ClProtocol *ai_pProtocol)
  //## begin ClGetPDU::ClGetPDU%878671640.hasinit preserve=no
  //## end ClGetPDU::ClGetPDU%878671640.hasinit
  //## begin ClGetPDU::ClGetPDU%878671640.initialization preserve=yes
  //## end ClGetPDU::ClGetPDU%878671640.initialization
{
  //## begin ClGetPDU::ClGetPDU%878671640.body preserve=yes

	m_poProtocol = ai_pProtocol;

  //## end ClGetPDU::ClGetPDU%878671640.body
}


ClGetPDU::~ClGetPDU ()
{
  //## begin ClGetPDU::~ClGetPDU%878671641.body preserve=yes

	m_poProtocol = NULL;

  //## end ClGetPDU::~ClGetPDU%878671641.body
}



//## Other Operations (implementation)
m4return_t ClGetPDU::GetPDU (ClPDU *&ao_poPDU)
{
  //## begin ClGetPDU::GetPDU%878671642.body preserve=yes
//	m4uint32_t iReqStatus ;
	m4pchar_t pszBuffer;
	m4pchar_t pszBufferHead;
	m4int32_t iReaded;
	m4int32_t lTotalReaded;
	m4int32_t lNextToRead;
	m4return_t iRet;

	lNextToRead =  m_poProtocol -> GetHeadSize ();

	pszBufferHead = new m4char_t [ lNextToRead + 2 ];

	iReaded=0;
	m4int32_t iChunkReaded=0;
	while(iChunkReaded<lNextToRead)
	{
		iReaded = Read ( pszBufferHead + iChunkReaded,  lNextToRead );
		
		if ( iReaded <=0 ) 
		{
			if (!iReaded)
				{m4TraceLevel(2,cerr<<"He leido 0 byte!!!"<<endl);}
			SETCODE(M4_ERR_TSAP_ERROR,ERRORLOG,"Error leyendo de TSAP.");
			ao_poPDU=NULL;
			delete [] pszBufferHead;
			return M4_ERROR;
		}
		else 
		{
			iChunkReaded+=iReaded;
			if(iChunkReaded != lNextToRead) 
				{m4TraceLevel(2,cout <<"Leidos "<<iReaded<<".Hasta ahora:"<<iChunkReaded<<endl);}
		}
	}
	lTotalReaded = iChunkReaded;

	// Consigo la pdu
	ao_poPDU = m_poProtocol -> GetPDU (pszBufferHead) ;

	if ( ao_poPDU == NULL )
	{
		SETCODE(M4_ERR_PDU_ERROR,ERRORLOG,"Error obtaining PDU from Protocol.");
		return M4_ERROR ;
	}

	delete [] pszBufferHead;

	while ( 1)
	{
		lNextToRead = ao_poPDU -> GetNextSizeBlock(pszBuffer);
		
		if ( lNextToRead == 0 )
		{
			break;
		}

		m4int32_t iChunkReaded =0;
		while(iChunkReaded!=lNextToRead)
		{
			iReaded = Read ( pszBuffer + iChunkReaded, lNextToRead );
			
			if (iReaded <=0 ) // BROKEN CONNECTION
			{
				if(!iReaded)
					cout<<"Ha llegado un 0"<<endl;
				SETCODE(M4_ERR_TSAP_ERROR,ERRORLOG,"Error leyendo de TSAP.");
				delete ao_poPDU ;
				ao_poPDU=NULL;
				return M4_ERROR;
			}
			else
			{
				iChunkReaded+=iReaded;
				if(iChunkReaded != lNextToRead) 
					{m4Trace(cout <<"Leidos "<<iReaded<<".Hasta ahora:"<<iChunkReaded<<endl);}
			}
		}
		ao_poPDU -> AddBlock ( pszBuffer );
	}

	if (  ao_poPDU  -> LogInfo ( ) )
	{
		m4uint32_t iLogSize = 0 ;
		m4pchar_t pLogBuffer ;
		m4uint32_t iReaded=0;
		m4int32_t iChunkReaded=0;
		
		iRet=ao_poPDU -> GetLogInfo ( pLogBuffer , iLogSize ) ;
		
		pLogBuffer = new m4char_t [ iLogSize ] ;
		

		iReaded= Read ( pLogBuffer , iLogSize) ;
		
		m4DebugExecute(if (iReaded!=iLogSize) cerr<<"ERROR in comunication"<<endl);

		iRet=ao_poPDU -> SetLogInfo ( pLogBuffer , iLogSize ) ;

		delete [] pLogBuffer;
	}

	return M4_SUCCESS;

  //## end ClGetPDU::GetPDU%878671642.body
}

m4int32_t ClGetPDU::Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer)
{
  //## begin ClGetPDU::Read%878671643.body preserve=yes
	return 0;
  //## end ClGetPDU::Read%878671643.body
}

m4return_t ClGetPDU::GetPDU (ClPDU *&ao_poPDU, m4pchar_t ai_pcBuffer, m4uint32_t ai_uiBufferSize)
{
  //## begin ClGetPDU::GetPDU%922271317.body preserve=yes

	m4TraceLevel(2,cout<<"Me ha llegado un buffer de "<<ai_uiBufferSize<<endl);
	m4pchar_t pszBuffer;
	m4pchar_t pszBufferHead;
	m4int32_t iReaded;
	m4int32_t lTotalReaded;
	m4int32_t lNextToRead;
	m4return_t iRet;

	m4pchar_t TheBuffer=ai_pcBuffer;
	m4uint32_t BufferSize=ai_uiBufferSize;

	lNextToRead =  m_poProtocol -> GetHeadSize ();

	m4TraceLevel(2,cout <<"\nNew PDU in Receiver\nRead-Chunk:"<<lNextToRead<<endl);

	pszBufferHead = new m4char_t [ lNextToRead + 2 ]; // ¿Para que ese 2?
	
	if(lNextToRead<=0)
	{
		delete pszBufferHead;
		m4Trace(cerr<<"Error creating PDU:HeadSize="<<lNextToRead<<endl);
		return M4_ERROR;
	}
// Ya sabemos que no es negativo
	if( ((m4uint32_t)lNextToRead) <= BufferSize )
	{
		memcpy(pszBufferHead,TheBuffer,lNextToRead);

		BufferSize-=lNextToRead;
		TheBuffer+=lNextToRead;
	}
	else
	{
		memcpy(pszBufferHead,TheBuffer,BufferSize);
// Se nos ha acabado el buffer, seguimos leyendo de TSAP
		m4int32_t iChunkReaded=BufferSize;
		BufferSize=0;
		TheBuffer=NULL;

		iReaded=0;

		while(iChunkReaded<lNextToRead)
		{
			iReaded = Read ( pszBufferHead+iChunkReaded,  lNextToRead - iChunkReaded);
			
			if ( iReaded <=0 ) 
			{
				if (!iReaded)
				{	m4Trace(cerr<<"He leido 0 byte!!!"<<endl);	}
				SETCODE(M4_ERR_TSAP_ERROR,ERRORLOG,"Error reading from TSAP.");
				ao_poPDU=NULL;
				delete [] pszBufferHead;
				return M4_ERROR;
			}
			else 
			{
				iChunkReaded+=iReaded;
				if(iChunkReaded != lNextToRead) 
				{m4TraceLevel(3,cout <<"Leidos "<<iReaded<<".Hasta ahora:"<<iChunkReaded<<" de un total de "<<lNextToRead<<endl);}
			}
		}
		lTotalReaded = iChunkReaded;
	}
// Creamos una PDU con la informacion de la cabecera
	ao_poPDU = m_poProtocol -> GetPDU (pszBufferHead) ;

	delete [] pszBufferHead;

	if ( ao_poPDU == NULL )
	{
		SETCODE(M4_ERR_PDU_ERROR,ERRORLOG,"Error obtaining PDU from Protocol.");
		return M4_ERROR ;
	}

	while ( 1)
	{
		pszBuffer=NULL;
		lNextToRead = ao_poPDU -> GetNextSizeBlock(pszBuffer);
		m4TraceLevel(2,cout <<"ReadChunk:"<<lNextToRead<<endl);
		
		if ( lNextToRead == 0 )
			break;
		else
			if(lNextToRead<0)
			{
				m4Trace(cerr<<"Error creating PDU:SizeBlock="<<lNextToRead<<endl);
				return M4_ERROR;
			}
		if(!pszBuffer)
		{
			m4Trace(cerr<<"NULL buffer creating pdu"<<endl);
			return M4_ERROR;
		}
// Ya sabemos que no es negativo
		if(((m4uint32_t)lNextToRead) <= BufferSize)
		{
			memcpy(pszBuffer,TheBuffer,lNextToRead);
			BufferSize-=lNextToRead;
			TheBuffer+=lNextToRead;
		}
		else
		{
			memcpy(pszBuffer,TheBuffer,BufferSize);
			m4int32_t iChunkReaded = BufferSize;
			BufferSize=0;
			TheBuffer=NULL;
			iReaded=0;
			while(iChunkReaded<lNextToRead)
			{
				iReaded = Read ( pszBuffer+iChunkReaded , lNextToRead - iChunkReaded);
				if (iReaded <=0 ) // BROKEN CONNECTION
				{
					if(!iReaded)
					{
						m4Trace(cout<<"Ha llegado un 0.Tamaño TOTAL de la PDU:"<<ao_poPDU->GetPDUSize()<<endl);
						return M4_ERROR;
					}
					else
					{
						SETCODE(M4_ERR_TSAP_ERROR,ERRORLOG,"Error reading from  TSAP.");
						delete ao_poPDU ;
						ao_poPDU=NULL;
						return M4_ERROR;
					}
				}
				else
				{
					iChunkReaded+=iReaded;
					if(iChunkReaded != lNextToRead) 
					{m4TraceLevel(3,cout <<"Leidos "<<iReaded<<".Hasta ahora:"<<iChunkReaded<<" de un total de "<<lNextToRead<<endl);}
				}
			}

		}

		iRet=ao_poPDU -> AddBlock ( pszBuffer );
		if ( iRet==M4_ERROR)
		{
			SETCODE(M4_ERR_PDU_ERROR,ERRORLOG,"Error adding block PDU.");
			return M4_ERROR ;
		}
	}

	if (  (ao_poPDU->GetVersion()<M4_PDU_40_VERSION) && ao_poPDU  -> LogInfo ( ) )
	{
		m4uint32_t iLogSize = 0 ;
		m4pchar_t pLogBuffer=NULL ;
		
		iRet=ao_poPDU -> GetLogInfo ( pLogBuffer , iLogSize ) ;
		if(NULL==pLogBuffer)
		{
			if(iRet==M4_ERROR)
			{m4Trace(cerr<<"Error obtaining loginfo from PDU"<<endl);}

			if(iLogSize<0)
			{
				m4Trace(cerr<<"Error:Logsize="<<iLogSize<<endl);
				return M4_ERROR;
			}

			m4TraceLevel(2,cout <<"LogSize:"<<iLogSize<<endl);
			pLogBuffer = new m4char_t [ iLogSize ] ;
			if(BufferSize>=((m4uint32_t)iLogSize))
			{
				memcpy(pLogBuffer,TheBuffer,iLogSize);
				if(BufferSize>iLogSize)
				{m4Trace(cerr<<"CODE ERRRORROROR!!!!"<<endl);}
			}
			else
			{
				m4uint32_t iReaded=0;
				m4uint32_t iChunkReaded=BufferSize;
				memcpy(pLogBuffer,TheBuffer,BufferSize);
				TheBuffer=NULL;
				BufferSize=0;
				while(iChunkReaded<iLogSize)
				{
					iReaded= Read ( pLogBuffer+iChunkReaded , iLogSize-iChunkReaded) ;
					if (iReaded <=0 ) // BROKEN CONNECTION
					{
						if(!iReaded)
						{m4Trace(cout<<"Ha llegado un 0.Retry."<<endl);}
						else
						{
							SETCODE(M4_ERR_TSAP_ERROR,ERRORLOG,"Error reading from TSAP.");
							delete ao_poPDU ;
							ao_poPDU=NULL;
							delete [] pLogBuffer;
							return M4_ERROR;
						}
					}
					else
					{
						iChunkReaded+=iReaded;
						if(iChunkReaded != iLogSize)
						{m4TraceLevel(3,cout <<"Leidos "<<iReaded<<".Hasta ahora:"<<iChunkReaded<<" de un total de "<<lNextToRead<<endl);}
					}
				}
			}
			iRet=ao_poPDU -> SetLogInfo ( pLogBuffer , iLogSize ) ;
			delete [] pLogBuffer;
		}
	}
	
	return M4_SUCCESS;
  //## end ClGetPDU::GetPDU%922271317.body
}

// Class ClGetPDUFile 


ClGetPDUFile::ClGetPDUFile (FILE *ai_pFile, ClProtocol *ai_pProtocol)
  //## begin ClGetPDUFile::ClGetPDUFile%878671644.hasinit preserve=no
  //## end ClGetPDUFile::ClGetPDUFile%878671644.hasinit
  //## begin ClGetPDUFile::ClGetPDUFile%878671644.initialization preserve=yes
  :  ClGetPDU ( ai_pProtocol )
  //## end ClGetPDUFile::ClGetPDUFile%878671644.initialization
{
  //## begin ClGetPDUFile::ClGetPDUFile%878671644.body preserve=yes
	m_pFile = ai_pFile;
  //## end ClGetPDUFile::ClGetPDUFile%878671644.body
}


ClGetPDUFile::~ClGetPDUFile ()
{
  //## begin ClGetPDUFile::~ClGetPDUFile%878671645.body preserve=yes
  //## end ClGetPDUFile::~ClGetPDUFile%878671645.body
}



//## Other Operations (implementation)
m4int32_t ClGetPDUFile::Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer)
{
  //## begin ClGetPDUFile::Read%878671646.body preserve=yes
	return fread ( ai_pBuffer, 1  , ai_iSizeBuffer, m_pFile );
  //## end ClGetPDUFile::Read%878671646.body
}

// Additional Declarations
  //## begin ClGetPDUFile%345F707F0255.declarations preserve=yes
  //## end ClGetPDUFile%345F707F0255.declarations

// Class ClGetPDUTSAP 


ClGetPDUTSAP::ClGetPDUTSAP (ClTSAP *ai_pTSAP, ClProtocol *ai_pProtocol)
  //## begin ClGetPDUTSAP::ClGetPDUTSAP%878671647.hasinit preserve=no
  //## end ClGetPDUTSAP::ClGetPDUTSAP%878671647.hasinit
  //## begin ClGetPDUTSAP::ClGetPDUTSAP%878671647.initialization preserve=yes
   :  ClGetPDU ( ai_pProtocol )
  //## end ClGetPDUTSAP::ClGetPDUTSAP%878671647.initialization
{
  //## begin ClGetPDUTSAP::ClGetPDUTSAP%878671647.body preserve=yes
	m_pTSAP   = ai_pTSAP;
  //## end ClGetPDUTSAP::ClGetPDUTSAP%878671647.body
}


ClGetPDUTSAP::~ClGetPDUTSAP ()
{
  //## begin ClGetPDUTSAP::~ClGetPDUTSAP%878671648.body preserve=yes
	m_pTSAP = NULL;
  //## end ClGetPDUTSAP::~ClGetPDUTSAP%878671648.body
}



//## Other Operations (implementation)
m4int32_t ClGetPDUTSAP::Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer)
{
  //## begin ClGetPDUTSAP::Read%878671649.body preserve=yes
	return m_pTSAP -> Read ( ai_pBuffer, ai_iSizeBuffer);
  //## end ClGetPDUTSAP::Read%878671649.body
}

// Class ClGetPDUBuffer 




ClGetPDUBuffer::ClGetPDUBuffer (m4pchar_t ai_pszBuffer, m4int32_t ai_lSizeBuffer, ClProtocol *ai_pProtocol)
  //## begin ClGetPDUBuffer::ClGetPDUBuffer%878730735.hasinit preserve=no
  //## end ClGetPDUBuffer::ClGetPDUBuffer%878730735.hasinit
  //## begin ClGetPDUBuffer::ClGetPDUBuffer%878730735.initialization preserve=yes
    :  ClGetPDU ( ai_pProtocol )
  //## end ClGetPDUBuffer::ClGetPDUBuffer%878730735.initialization
{
  //## begin ClGetPDUBuffer::ClGetPDUBuffer%878730735.body preserve=yes
	m_pszBuffer = ai_pszBuffer;
	m_iCurrentPos = 0;
	m_iBufferSize= ai_lSizeBuffer;

  //## end ClGetPDUBuffer::ClGetPDUBuffer%878730735.body
}


ClGetPDUBuffer::~ClGetPDUBuffer ()
{
  //## begin ClGetPDUBuffer::~ClGetPDUBuffer%878730736.body preserve=yes

	m_pszBuffer = NULL ;
	m_iCurrentPos = 0 ;
	m_iBufferSize = 0 ;

  //## end ClGetPDUBuffer::~ClGetPDUBuffer%878730736.body
}



//## Other Operations (implementation)
m4int32_t ClGetPDUBuffer::Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer)
{
  //## begin ClGetPDUBuffer::Read%878730737.body preserve=yes
	if  ( m_iCurrentPos + ai_iSizeBuffer > m_iBufferSize )
		return -1;
	
	memcpy ( ai_pBuffer , &m_pszBuffer [ m_iCurrentPos ] , ai_iSizeBuffer );
	m_iCurrentPos += ai_iSizeBuffer;
	return ai_iSizeBuffer;
  //## end ClGetPDUBuffer::Read%878730737.body
}

// Additional Declarations
  //## begin ClGetPDUBuffer%346048FA02EF.declarations preserve=yes
  //## end ClGetPDUBuffer%346048FA02EF.declarations

// Class ClGetPDUTSAPBuffer 





ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer (ClTSAP *ai_pTSAP, ClProtocol *ai_pProtocol, m4pchar_t ai_pszBuffer, m4int32_t ai_lSizeBuffer)
  //## begin ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.hasinit preserve=no
      : m_pszBuffer(ai_pszBuffer), m_iBufferSize(ai_lSizeBuffer), m_iCurrentPos(0), m_pTSAP(ai_pTSAP)
  //## end ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.hasinit
  //## begin ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.initialization preserve=yes
  ,  ClGetPDU ( ai_pProtocol )
  //## end ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.initialization
{
  //## begin ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.body preserve=yes
  //## end ClGetPDUTSAPBuffer::ClGetPDUTSAPBuffer%922441067.body
}


ClGetPDUTSAPBuffer::~ClGetPDUTSAPBuffer ()
{
  //## begin ClGetPDUTSAPBuffer::~ClGetPDUTSAPBuffer%922441068.body preserve=yes
	m_pTSAP = NULL;
	m_pszBuffer = NULL ;
	m_iCurrentPos = 0 ;
	m_iBufferSize = 0 ;
  //## end ClGetPDUTSAPBuffer::~ClGetPDUTSAPBuffer%922441068.body
}



//## Other Operations (implementation)
m4int32_t ClGetPDUTSAPBuffer::Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer)
{
  //## begin ClGetPDUTSAPBuffer::Read%922441069.body preserve=yes
	m4uint32_t iReaded=0;

// Parte de buffer
	if  ( m_iCurrentPos + ai_iSizeBuffer > m_iBufferSize )
		return -1;
	
	memcpy ( ai_pBuffer , &m_pszBuffer [ m_iCurrentPos ] , ai_iSizeBuffer );
	m_iCurrentPos += ai_iSizeBuffer;
	iReaded=ai_iSizeBuffer;
// Parte de buffer

// Parte TSAP
	iReaded=m_pTSAP -> Read ( ai_pBuffer, ai_iSizeBuffer, M4_TRUE );
// Parte TSAP
	return iReaded;
  //## end ClGetPDUTSAPBuffer::Read%922441069.body
}

// Additional Declarations
  //## begin ClGetPDUTSAPBuffer%36FB54170036.declarations preserve=yes
  //## end ClGetPDUTSAPBuffer%36FB54170036.declarations

//## begin module%345F71E6008F.epilog preserve=yes
//## end module%345F71E6008F.epilog
