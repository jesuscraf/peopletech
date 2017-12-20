//## begin module%390FEF300275.cm preserve=no
//## end module%390FEF300275.cm

//## begin module%390FEF300275.cp preserve=no
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
//## end module%390FEF300275.cp

//## Module: commprotocols%390FEF300275; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commprotocols.cpp

//## begin module%390FEF300275.additionalIncludes preserve=no
//## end module%390FEF300275.additionalIncludes

//## begin module%390FEF300275.includes preserve=yes
#include <m4trace.hpp>
#include <m4thread.hpp>
// No funciona el bodyReference de Rose
#include <m4clock.hpp>
//## end module%390FEF300275.includes

// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// stdio
#include <stdio.h>
// commprotocols
#include <commprotocols.hpp>
// basepdu
#include <basepdus.hpp>
// Protocol
#include <protocol.hpp>
//## begin module%390FEF300275.declarations preserve=no
//## end module%390FEF300275.declarations

//## begin module%390FEF300275.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
#include "m4unicode.hpp"


// like calloc but using c++ operator.
static void* cnew(size_t ai_num, size_t ai_size)
{
    size_t nbytes = ai_num*ai_size;
    char* buffer = new char[nbytes];
    if (NULL != buffer)
    {
        memset(buffer, 0, nbytes);
    }
    return (void*)buffer;
}


// like realloc but using c++ operator.
static void* renew(void *ai_ptr, size_t ai_currentSize, size_t ai_addedSize)
{
    char* buffer = new char[ai_currentSize+ai_addedSize];
    if (NULL == buffer)
    {
        if (NULL != ai_ptr)
        {
            delete ai_ptr;
        }
        return NULL;
    }

    if (NULL != ai_ptr)
    {
        memcpy(buffer, ai_ptr, ai_currentSize);
        delete ai_ptr;
    }

    return (void*)buffer;
}


//## end module%390FEF300275.additionalDeclarations


// Class ClCommProtocol 














ClCommProtocol::ClCommProtocol ()
  //## begin ClCommProtocol::ClCommProtocol%957345699.hasinit preserve=no
      : m_uiSentBytes(0), m_uiReadedBytes(0), m_ui64StartWrittingTime(0
  ), m_ui64FinishWrittingTime(0), m_ui64StartReadingTime(0), m_ui64FinishReadingTime(0), m_uiNSentRequests(0), m_uiNReceivedRequests(0), m_pDumpFile(NULL), m_ui64StartProccessingWrittingTime(0), m_ui64FinishProccessingReadingTime(0)
  //## end ClCommProtocol::ClCommProtocol%957345699.hasinit
  //## begin ClCommProtocol::ClCommProtocol%957345699.initialization preserve=yes
  ,m_pDumpReadFile(NULL),m_pDumpWriteFile(NULL)
  //## end ClCommProtocol::ClCommProtocol%957345699.initialization
{
  //## begin ClCommProtocol::ClCommProtocol%957345699.body preserve=yes
	m4pchar_t pcEnvVar=getenv("M4_DUMP_PDU");
	if(pcEnvVar && !strcmp(pcEnvVar,"1") )
		m_bDump=M4_TRUE;
	else
		m_bDump=M4_FALSE;
  //## end ClCommProtocol::ClCommProtocol%957345699.body
}


ClCommProtocol::~ClCommProtocol ()
{
  //## begin ClCommProtocol::~ClCommProtocol%957345700.body preserve=yes
  //## end ClCommProtocol::~ClCommProtocol%957345700.body
}



//## Other Operations (implementation)
m4uint64_t ClCommProtocol::GetStartWrittingTime ()
{
  //## begin ClCommProtocol::GetStartWrittingTime%963233189.body preserve=yes
	return m_ui64StartWrittingTime;
  //## end ClCommProtocol::GetStartWrittingTime%963233189.body
}

m4uint64_t ClCommProtocol::GetFinishWrittingTime ()
{
  //## begin ClCommProtocol::GetFinishWrittingTime%963233190.body preserve=yes
	return m_ui64FinishWrittingTime;
  //## end ClCommProtocol::GetFinishWrittingTime%963233190.body
}

m4uint64_t ClCommProtocol::GetStartReadingTime ()
{
  //## begin ClCommProtocol::GetStartReadingTime%963233191.body preserve=yes
	return m_ui64StartReadingTime;
  //## end ClCommProtocol::GetStartReadingTime%963233191.body
}

m4uint64_t ClCommProtocol::GetFinishReadingTime ()
{
  //## begin ClCommProtocol::GetFinishReadingTime%963233192.body preserve=yes
	return m_ui64FinishReadingTime;
  //## end ClCommProtocol::GetFinishReadingTime%963233192.body
}

m4uint_t ClCommProtocol::GetNSentRequests ()
{
  //## begin ClCommProtocol::GetNSentRequests%964423551.body preserve=yes
	return m_uiNSentRequests;
  //## end ClCommProtocol::GetNSentRequests%964423551.body
}

m4uint_t ClCommProtocol::GetNReceivedRequests ()
{
  //## begin ClCommProtocol::GetNReceivedRequests%964423552.body preserve=yes
	return m_uiNReceivedRequests;
  //## end ClCommProtocol::GetNReceivedRequests%964423552.body
}

m4return_t ClCommProtocol::EnableDump (m4pchar_t ai_pcFileName)
{
  //## begin ClCommProtocol::EnableDump%967562173.body preserve=yes
	M4_ASSERT(0);
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	m_pDumpFile=M4Fopen(ai_pcFileName, M4UniWriteBinary, eEncoding);
	if(NULL==m_pDumpFile)
		return M4_ERROR;
	else
		return M4_SUCCESS;
  //## end ClCommProtocol::EnableDump%967562173.body
}

m4return_t ClCommProtocol::DisableDump ()
{
  //## begin ClCommProtocol::DisableDump%967562174.body preserve=yes
	M4_ASSERT(0);
	if(m_pDumpFile!=NULL)
	{
		fclose(m_pDumpFile);
		m_pDumpFile=NULL;
	}
	return M4_SUCCESS;
  //## end ClCommProtocol::DisableDump%967562174.body
}

m4return_t ClCommProtocol::Dump (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize)
{
  //## begin ClCommProtocol::Dump%967646980.body preserve=yes
	M4_ASSERT(0);
	if(m_pDumpFile && ai_iChunkSize)
    {
		fwrite(ai_pcChunk, 1, ai_iChunkSize, m_pDumpFile);
    }
	
	return M4_SUCCESS;
  //## end ClCommProtocol::Dump%967646980.body
}

m4return_t ClCommProtocol::SetVersion (m4int_t ai_iVersion)
{
  //## begin ClCommProtocol::SetVersion%973247123.body preserve=yes
	return M4_ERROR;
  //## end ClCommProtocol::SetVersion%973247123.body
}

m4int_t ClCommProtocol::GetVersion ()
{
  //## begin ClCommProtocol::GetVersion%973247124.body preserve=yes
	return 0;
  //## end ClCommProtocol::GetVersion%973247124.body
}

m4uint64_t ClCommProtocol::GetStartProccessingWrittingTime ()
{
  //## begin ClCommProtocol::GetStartProccessingWrittingTime%975501045.body preserve=yes
	return m_ui64StartProccessingWrittingTime;
  //## end ClCommProtocol::GetStartProccessingWrittingTime%975501045.body
}

m4uint64_t ClCommProtocol::GetFinishProccessingReadingTime ()
{
  //## begin ClCommProtocol::GetFinishProccessingReadingTime%975501046.body preserve=yes
	return m_ui64FinishProccessingReadingTime;
  //## end ClCommProtocol::GetFinishProccessingReadingTime%975501046.body
}

// Additional Declarations
  //## begin ClCommProtocol%390FEB93008A.declarations preserve=yes
	  m4return_t ClCommProtocol::EnableWriteDump (m4pchar_t ai_pcFileName)
	  {
		if(m_pDumpWriteFile==NULL)
		{
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniANSI ;
			m_pDumpWriteFile=M4Fopen(ai_pcFileName, M4UniWriteBinary, eEncoding);
			if(NULL==m_pDumpWriteFile)
			{
				m4Trace(cerr << "error abriendo dumpfile "<<ai_pcFileName<<endl<<flush);
				return M4_ERROR;
			}
			else
			{
				m4Trace(cerr << "abriendo dumpfile "<<ai_pcFileName<<endl<<flush);
				return M4_SUCCESS;
			}
		}
		else
			return M4_SUCCESS;

	  }

      m4return_t ClCommProtocol::DisableWriteDump ()
	  {
		if(m_pDumpWriteFile!=NULL)
		{
			fclose(m_pDumpWriteFile);
			m_pDumpWriteFile=NULL;
		}
		return M4_SUCCESS;
	  }
      m4return_t ClCommProtocol::DumpWrite (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize)
	  {
		if(m_pDumpWriteFile && ai_iChunkSize)
        {
			fwrite(ai_pcChunk, 1, ai_iChunkSize, m_pDumpWriteFile);
        }
		return M4_SUCCESS;
	  }
	  m4return_t ClCommProtocol::EnableReadDump (m4pchar_t ai_pcFileName)
	  {
		if(m_pDumpReadFile==NULL)
		{
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniANSI ;
			m_pDumpReadFile=M4Fopen(ai_pcFileName, M4UniWriteBinary, eEncoding);
			if(NULL==m_pDumpReadFile)
			{
				m4Trace(cerr << "error abriendo dumpfile "<<ai_pcFileName<<endl<<flush);
				return M4_ERROR;
			}
			else
			{
				m4Trace(cerr << "abierto dumpfile "<<ai_pcFileName<<endl<<flush);
				return M4_SUCCESS;
			}
		}
		else
			return M4_SUCCESS;
	  }
      m4return_t ClCommProtocol::DisableReadDump ()
	  {
		if(m_pDumpReadFile!=NULL)
		{
			fclose(m_pDumpReadFile);
			m_pDumpReadFile=NULL;
		}
		return M4_SUCCESS;
	  }
      m4return_t ClCommProtocol::DumpRead (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize)
	  {
		if(m_pDumpReadFile && ai_iChunkSize)
        {
			fwrite(ai_pcChunk, 1, ai_iChunkSize, m_pDumpReadFile);
        }
		return M4_SUCCESS;
	  }

  //## end ClCommProtocol%390FEB93008A.declarations

// Class ClCommPDUProtocol 













ClCommPDUProtocol::ClCommPDUProtocol ()
  //## begin ClCommPDUProtocol::ClCommPDUProtocol%957345693.hasinit preserve=no
      : m_bLog(M4_FALSE), m_iVersion(M4_PDU_40_VERSION), m_poReadPDU(NULL), m_iProcessedBytes(0), m_iNextBlockSize(0), m_pcNextBlockBuffer(NULL), m_poWritePDU(NULL), m_pcWriteBuffer(NULL), m_iWriteBufferSize(0)
  //## end ClCommPDUProtocol::ClCommPDUProtocol%957345693.hasinit
  //## begin ClCommPDUProtocol::ClCommPDUProtocol%957345693.initialization preserve=yes
  //## end ClCommPDUProtocol::ClCommPDUProtocol%957345693.initialization
{
  //## begin ClCommPDUProtocol::ClCommPDUProtocol%957345693.body preserve=yes
	m_pcWriteBuffer=new m4char_t[M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE];
	m_iWriteBufferSize=M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE;
  //## end ClCommPDUProtocol::ClCommPDUProtocol%957345693.body
}


ClCommPDUProtocol::~ClCommPDUProtocol ()
{
  //## begin ClCommPDUProtocol::~ClCommPDUProtocol%957345694.body preserve=yes
	if(m_pcWriteBuffer)
		delete m_pcWriteBuffer;
  //## end ClCommPDUProtocol::~ClCommPDUProtocol%957345694.body
}



//## Other Operations (implementation)
m4return_t ClCommPDUProtocol::AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize)
{
  //## begin ClCommPDUProtocol::AddChunk%957345695.body preserve=yes
	ao_uiUssed=0;

    m4uint32_t iObjectId = 0;
    m4uint32_t iCompressionType = 0;
    m4uint_t uiUsedBytes = 0;
    m4bool_t isZDelta = M4_FALSE;
	m4bool_t bFinish = M4_FALSE;
	m4bool_t bUpdtDoDelta = M4_TRUE;

	// Bg 139141
	// Ya comenzó la deserialización de la PDU por lo que no volvemos a 
	// comprobar si el chunk es zDelta.
	if(m_poReadPDU != NULL)
	{
		bUpdtDoDelta = M4_FALSE;
	}

	m4return_t iRes = m_oZDelta.AddChunk(ai_pcChunk,ai_uiChunkSize,bUpdtDoDelta,bFinish);


	if (m_oZDelta.DoDelta()) {
		// bugid: 0108865.
        // Cuando el trozo que llega no es suficiente para decodificar la cabecera, no nos
        // quedamos nada, y devolvemos todo a la capa anterior.
		if (bFinish == M4_FALSE) {
			ao_uiUssed = 0;
            ao_uiNextBLockSize = m_oZDelta.GetNextBlockSize();
			return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
		}
			
		m4uint32_t iLen = 0;
		iRes = m_oZDelta.DoDeCompress(ai_pcChunk, iLen);

		/* Bug 0111554
		Si la descompresión da error ya no se continua y se da el mismo error
		que si no se consigue la pdu
		*/
		if( iRes != M4_SUCCESS )
		{
			m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
			M4_SRV_ERROR_N(0,"Error decompresing PDU.");
			return M4_COMM_PROTOCOL_RETURN_ERROR ;
		}

		ao_uiUssed = m_oZDelta.GetNextBlockSize();
		ai_uiChunkSize = iLen;

		// si he descomprimido, recupero identificadores.
        iObjectId = m_oZDelta.GetSessionId();
        iCompressionType = m_oZDelta.GetCompressionType();
        isZDelta = M4_TRUE;
	}


	if(NULL==m_poReadPDU)
	{
if(m_bDump)
{
	m4char_t filename[256];
	sprintf(filename,"%lu-%lu.rpdu", this, m_uiNReceivedRequests);
	EnableReadDump(filename);
}
		m4TraceLevel(2,cout << "br "<<flush);
		m_ui64StartReadingTime=ClPrecisionTicker::GetTimeTick();
		int iHeadSize=m_Protocol.GetHeadSize ();
		if(iHeadSize>ai_uiChunkSize)
		{
			ao_uiNextBLockSize=iHeadSize;
			return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
		}
		else
		{
			ai_uiChunkSize -=iHeadSize;
			m_poReadPDU=m_Protocol.GetPDU (ai_pcChunk) ;
			if ( NULL ==m_poReadPDU )
			{
				m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
				M4_SRV_ERROR_N(0,"Error obtaining PDU from Protocol.");
				return M4_COMM_PROTOCOL_RETURN_ERROR ;
			}

			// Bg 115991. Borramos los patrones zDelta ya que el GC los mantiene para poder descomprimir
			// peticiones posteriores a un time-out. Sólo con protocolo http. 
			if( m_poReadPDU->GetIdPDU() == M4_ID_PDU_NEWDISCONNECT )
			{
				if( isZDelta == M4_TRUE )
				{
					M4ZDeltaManager* poDeltaManager = M4ZDeltaManager::GetInstance();
					if (NULL != poDeltaManager)
					{
						// Bg 118467. Aun no borramos los patrones, los necesitamos para comprimir la respuesta
						// al disconnect. Lo marcamos en el patron de entrada y borramos despues de comprimir.
						M4ZDeltaElementMapIt_t oIt = poDeltaManager->find_in( iObjectId ) ;

						if ( oIt != poDeltaManager->end_in() )
						{
							M4ZDeltaElement oElm;
							oElm.buffer = (*oIt).second.buffer ;
							oElm.size = (*oIt).second.size ;
							oElm.identifier = (*oIt).second.identifier ;
							oElm.bJustDisconnectedSession = M4_TRUE ;
							oElm.bJustCanceledRequest = M4_FALSE;
							
							poDeltaManager->erase_in( iObjectId ) ;

							poDeltaManager->insert_in( iObjectId, oElm ) ;
						}					
					}
				}
			}
			

			ai_pcChunk+=iHeadSize;
			uiUsedBytes+=iHeadSize;

            // set pdu identifiers.
            m_poReadPDU->SetObjectId(iObjectId);
            m_poReadPDU->SetCompressionType(iCompressionType);
		}
	}

	m4return_t iRet;
	while(ai_uiChunkSize>=0)
	{
		if(0==m_iNextBlockSize  )
		{
			m_iNextBlockSize  = m_poReadPDU -> GetNextSizeBlock(m_pcNextBlockBuffer);
			if(m_iNextBlockSize==(m4uint32_t)-1)
			{
				ao_uiNextBLockSize=0;
				M4_SRV_ERROR_N(0,"Internal error in a pdu");
                if (!isZDelta) ao_uiUssed = uiUsedBytes;
				return M4_COMM_PROTOCOL_RETURN_ERROR ; 
			}
		}
		
		if( (ai_uiChunkSize>0) && !m_iNextBlockSize )
		{
			if (m_poReadPDU->GetVersion()<M4_PDU_40_VERSION)
			{
				m4pchar_t tempbuffer;
				m4uint32_t logsizeinpdu;
				iRet=m_poReadPDU->GetLogInfo(tempbuffer,logsizeinpdu);
				if(logsizeinpdu && (logsizeinpdu!=ai_uiChunkSize))
				{
					M4_SRV_DEBUG("Sobran:%0",ai_uiChunkSize);
					M4_SRV_DEBUG("Dump:%0",ai_pcChunk);
                    if (!isZDelta) ao_uiUssed = uiUsedBytes;
					return M4_COMM_PROTOCOL_RETURN_ERROR ;
				}
				else
				{
					m4TraceLevel(2,cout<<"." );
					//ExtractLog();
					m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
					m_uiNReceivedRequests++;
					m_uiReadedBytes+=uiUsedBytes;
					m4TraceLevel(2,cout << "fr "<<flush);
					m_oZDelta.Reset(m_poReadPDU);
                    if (!isZDelta) ao_uiUssed = uiUsedBytes;
					return M4_COMM_PROTOCOL_RETURN_COMPLETED;
				}
			}
			else
			{
				m4TraceLevel(2,cout<<"." );
				//ExtractLog();
				m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
				m_uiNReceivedRequests++;
				m_uiReadedBytes+=uiUsedBytes;
				m4TraceLevel(2,cout << "fr "<<flush);
				m_oZDelta.Reset(m_poReadPDU);
                if (!isZDelta) ao_uiUssed = uiUsedBytes;
				return M4_COMM_PROTOCOL_RETURN_COMPLETED;
			}
		}

		if( (0==m_iNextBlockSize) ||  (m_bLog))
		{
			
			m4pchar_t tempbuffer=NULL;
			m4uint32_t logsizeinpdu;
			iRet=m_poReadPDU->GetLogInfo(tempbuffer,logsizeinpdu);
			if( (M4_SUCCESS==iRet) && (logsizeinpdu>0) && (m_poReadPDU->GetVersion()!=M4_PDU_40_VERSION))
			{
				if(NULL==tempbuffer)
				{
					m_bLog=M4_TRUE;
					if(ai_uiChunkSize<logsizeinpdu)
					{
						m_iNextBlockSize=logsizeinpdu;
						ao_uiNextBLockSize=m_iNextBlockSize;
						m_uiReadedBytes+=uiUsedBytes;
                        if (!isZDelta) ao_uiUssed = uiUsedBytes;
						return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
					}
					else
					{	
						m4TraceLevel(2,cerr<< "\nOld format log:"<< logsizeinpdu<<endl);
						SetVersion(M4_PDU_32_VERSION);
						m_poReadPDU->SetLogInfo (ai_pcChunk,ai_uiChunkSize);
						m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
						m_bLog=M4_FALSE;
						m_iNextBlockSize=0;
						m_uiNReceivedRequests++;
						m_uiReadedBytes+=uiUsedBytes;
						m_oZDelta.Reset(m_poReadPDU);
                        if (!isZDelta) ao_uiUssed = uiUsedBytes;
						return M4_COMM_PROTOCOL_RETURN_COMPLETED;
					}
				}
			}
			m4TraceLevel(2,cout<<"." );
			//ExtractLog();
			m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
			m_uiNReceivedRequests++;
			m_uiReadedBytes+=uiUsedBytes;
			m4TraceLevel(2,cout << "fr "<<flush);
			m_oZDelta.Reset(m_poReadPDU);
            if (!isZDelta) ao_uiUssed = uiUsedBytes;
			return M4_COMM_PROTOCOL_RETURN_COMPLETED;
		}
		if(ai_uiChunkSize>=m_iNextBlockSize)
		{
			memcpy(m_pcNextBlockBuffer,ai_pcChunk,m_iNextBlockSize);
			iRet=m_poReadPDU->AddBlock(m_pcNextBlockBuffer);
			m4TraceLevel(3,cout <<"Addblock:"<<m_iNextBlockSize<<endl);
			if(M4_ERROR==iRet)
			{
				M4_SRV_ERROR_N(0,"Error in PDU format");
				delete m_poReadPDU;
				m_poReadPDU=NULL;
				m_ui64FinishReadingTime=ClPrecisionTicker::GetTimeTick();
                if (!isZDelta) ao_uiUssed = uiUsedBytes;
				return M4_COMM_PROTOCOL_RETURN_ERROR ;
			}
			else
			{
				ai_pcChunk+=m_iNextBlockSize;
				ai_uiChunkSize-=m_iNextBlockSize;
				uiUsedBytes+=m_iNextBlockSize;
				m_iNextBlockSize=0;
			}
		} 
		else
		{
			if(m_iNextBlockSize==(m4uint32_t)(-1))
			{
				m4Trace(cerr << "Error in pduprotocol"<<endl);
                if (!isZDelta) ao_uiUssed = uiUsedBytes;
				return M4_COMM_PROTOCOL_RETURN_ERROR ;
			}
			ao_uiNextBLockSize=m_iNextBlockSize;
			m_uiReadedBytes+=uiUsedBytes;
            if (!isZDelta) ao_uiUssed = uiUsedBytes;
			return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
		}
	}// While
	
    if (!isZDelta) ao_uiUssed = uiUsedBytes;
	return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
  //## end ClCommPDUProtocol::AddChunk%957345695.body
}

m4return_t ClCommPDUProtocol::IsCompleted ()
{
  //## begin ClCommPDUProtocol::IsCompleted%957345696.body preserve=yes
	 char *thebuffer;
	if((NULL!=m_poReadPDU) && (m_poReadPDU->GetNextSizeBlock(thebuffer)==0))
	{
		return M4_SUCCESS;
	}
	else
	{
		return M4_ERROR;
	}

  //## end ClCommPDUProtocol::IsCompleted%957345696.body
}

m4return_t ClCommPDUProtocol::GetReadedPDU (ClPDU *&ao_pPDU)
{
  //## begin ClCommPDUProtocol::GetReadedPDU%957345698.body preserve=yes
//	if(M4_SUCCESS==IsCompleted())
//	{
		ao_pPDU=m_poReadPDU;
		return M4_SUCCESS;
//	}
//	ao_pPDU=NULL;
//	return M4_ERROR;
  //## end ClCommPDUProtocol::GetReadedPDU%957345698.body
}

m4return_t ClCommPDUProtocol::SetWritePDU (ClPDU *ai_pPDU)
{
  //## begin ClCommPDUProtocol::SetWritePDU%957364994.body preserve=yes

if(m_bDump)
{
    static int clientside = 0;
    int nrequest = 0;
	m4char_t filename[256];

    // En el server el número de request ya se ha incrementado en 1.
    // le resto 1 para que la pdu de escritura se corresponda con la de lectura
    if (0 == clientside)
    {
        if (0 == m_uiNReceivedRequests)
        {
            clientside = 1;
        }
    }

    if (1 == clientside)
        nrequest = m_uiNReceivedRequests;
    else
        nrequest = m_uiNReceivedRequests -1;


	sprintf(filename,"%lu-%lu.wpdu", this, nrequest);
	EnableWriteDump(filename);
}

	m_ui64StartProccessingWrittingTime=ClPrecisionTicker::GetTimeTick();
	m_poWritePDU=ai_pPDU;
	return M4_SUCCESS;
  //## end ClCommPDUProtocol::SetWritePDU%957364994.body
}

m4return_t ClCommPDUProtocol::Initialize ()
{
  //## begin ClCommPDUProtocol::Initialize%957517150.body preserve=yes

	m_iProcessedBytes=0;
	m_pLostBlock = NULL;
	m_iSizeLostBlock = 0;
	m_bLog=M4_FALSE;

	// reset delta status
	m_oZDelta.Reset(m_poReadPDU);

	return M4_SUCCESS;

  //## end ClCommPDUProtocol::Initialize%957517150.body
}

m4return_t ClCommPDUProtocol::ReleaseContents ()
{
  //## begin ClCommPDUProtocol::ReleaseContents%957944567.body preserve=yes
	if(NULL==m_poReadPDU)
	{
		M4_SRV_DEBUG_N("La PDU ya era nula!!!!!");
		return M4_ERROR;
	}
	else 
	{
		m_poReadPDU=NULL;
		return M4_SUCCESS;
	}
  //## end ClCommPDUProtocol::ReleaseContents%957944567.body
}

m4return_t ClCommPDUProtocol::GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize)
{
  //## begin ClCommPDUProtocol::GetNextChunk%959266651.body preserve=yes
M4_ASSERT(NULL!=m_poWritePDU);

	m4pchar_t pcBuffer = NULL;
	m4uint32_t uiLen = 0;
	if (m_iProcessedBytes == 0 && m_oZDelta.DoCompress(m_poWritePDU,pcBuffer,uiLen) == M4_SUCCESS) {

        if ((NULL==pcBuffer) || (0==uiLen))
        {
            return M4_ERROR;
        }
        if (uiLen > m_iWriteBufferSize)
        {
            m4pchar_t tmpBuffer = new m4char_t[uiLen];
            if (tmpBuffer != NULL)
            {
                delete[] m_pcWriteBuffer;
                m_pcWriteBuffer = tmpBuffer;
                m_iWriteBufferSize = uiLen;
            }
            else
            {
                return M4_ERROR;
            }
        }

        memcpy(m_pcWriteBuffer, pcBuffer, uiLen);
        delete[] pcBuffer;
		ao_pcChunk = m_pcWriteBuffer;
		ao_uiChunkSize = uiLen;
		m_iProcessedBytes = -1;
		return M4_SUCCESS;
	}

	if (m_iProcessedBytes == -1) //En el ciclo anterior leímos una pdu completa
	{
		m_uiNSentRequests++;
		m_ui64FinishWrittingTime=ClPrecisionTicker::GetTimeTick();
		m_iProcessedBytes = 0;
		return M4_ERROR;
	}

	unsigned int	uiChunkSize,	uiTotalChunkSize = 0;
	unsigned long	AuxLong;
	char * pBuffer = m_pcWriteBuffer;
	m4pchar_t pcPDUBuffer;
	m4return_t	ret;

	if(m_iProcessedBytes==0)
		m_ui64StartWrittingTime=ClPrecisionTicker::GetTimeTick();
	while (1)
	{
		if (m_pLostBlock != NULL)
		{
			pcPDUBuffer = m_pLostBlock;
			AuxLong = m_iSizeLostBlock;
			m_pLostBlock = NULL;
			m_iSizeLostBlock = 0;
			ret = M4_SUCCESS;
		}
		else
			ret = m_poWritePDU->GetNextBlock(m_iProcessedBytes, pcPDUBuffer, AuxLong);

		if (ret == M4_SUCCESS)
		{
			uiChunkSize = AuxLong;
			if (uiChunkSize > M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE)
			{
				//Si tenemos algo almacenado, enviarlo, y enviar el bloque gordo en la siguiente llamada
				if (uiTotalChunkSize > 0)
				{
					ao_pcChunk = m_pcWriteBuffer;
					ao_uiChunkSize = uiTotalChunkSize;
					m_pLostBlock = pcPDUBuffer;
					m_iSizeLostBlock = AuxLong;
					return M4_SUCCESS;
				}
				else 
				{
					//Enviemos el bloque gordo
					ao_pcChunk = pcPDUBuffer;
					ao_uiChunkSize = AuxLong;
					m_iProcessedBytes += AuxLong;
					return M4_SUCCESS;
				}
			}
			if (uiTotalChunkSize + uiChunkSize > M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE)
			{
				ao_pcChunk = m_pcWriteBuffer;
				ao_uiChunkSize = uiTotalChunkSize;
				m_pLostBlock = pcPDUBuffer;
				m_iSizeLostBlock = AuxLong;
				return M4_SUCCESS;
			}

			memcpy(&m_pcWriteBuffer[uiTotalChunkSize], pcPDUBuffer, uiChunkSize);
			uiTotalChunkSize += uiChunkSize;
			m_iProcessedBytes += uiChunkSize;
		}
		else
		{
			//Hemos terminado con la PDU;
			m_uiSentBytes+=m_iProcessedBytes;
			m_iProcessedBytes = -1;
			ao_pcChunk = m_pcWriteBuffer;
			ao_uiChunkSize = uiTotalChunkSize;
			if (ao_uiChunkSize)
				return M4_SUCCESS; //Hay que enviar lo almacenado
			else
			{
				m_uiNSentRequests++;
				m_ui64FinishWrittingTime=ClPrecisionTicker::GetTimeTick();
				return M4_ERROR; // Significa que hemos terminado con la PDU, y hemos enmviado todo
			}
		}
	}
	
  //## end ClCommPDUProtocol::GetNextChunk%959266651.body
}

m4return_t ClCommPDUProtocol::ExtractLog ()
{
  //## begin ClCommPDUProtocol::ExtractLog%962101127.body preserve=yes
/*	
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogInfoSize;

	m4uint32_t uiDataUnit=0;
	
	M4DataStorage *poDS =m_poReadPDU->GetDataStorage ();
	if(NULL!=poDS )
	{
		uiDataUnit=poDS->Blocks();
		if(uiDataUnit)
		{
			uiDataUnit--;	// Si tenemos N la ultima sera la N-1
			m4bool_t bAux;
			poDS->Get (pcLogBuffer, uiLogInfoSize, bAux, uiDataUnit);
			if(pcLogBuffer && (uiLogInfoSize>M4_COMMS_LOG_LABEL_SIZE) && (!strncmp(pcLogBuffer,M4_COMMS_LOG_LABEL,M4_COMMS_LOG_LABEL_SIZE)))
			{
				if(NULL!=strstr(pcLogBuffer+M4_COMMS_LOG_LABEL_SIZE,M4_COMMS_LOG_LABEL))
				{
					cerr <<"Ya esta"<<endl;
					return M4_ERROR;
				}
				m_poReadPDU->EnableLogInfo();
				m_poReadPDU->SetLogInfo (pcLogBuffer+M4_COMMS_LOG_LABEL_SIZE,uiLogInfoSize-M4_COMMS_LOG_LABEL_SIZE);
				poDS->Remove(uiDataUnit);
			}
		}
	}

*/
	return M4_SUCCESS;
  //## end ClCommPDUProtocol::ExtractLog%962101127.body
}

m4return_t ClCommPDUProtocol::AddLog ()
{
  //## begin ClCommPDUProtocol::AddLog%962101128.body preserve=yes
	/*
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogSize;
	m4return_t iRet=m_poWritePDU->GetLogInfo(pcLogBuffer,uiLogSize);
	
	if((M4_ERROR!=iRet) && (uiLogSize>0))
	{
		if(NULL!=strstr(pcLogBuffer,M4_COMMS_LOG_LABEL))
		{	
			cerr <<"Ya esta"<<endl;
			return M4_ERROR;
		}

		M4DataStorage *poDS =m_poWritePDU->GetDataStorage ();
		if(NULL==poDS )
		{
			poDS=M4DataStorage::GetNewDataStorage();
			m_poWritePDU->SetDataStorage(poDS);
		}
		ClDataUnit * poDU;
		poDS ->GetNewDataUnit(poDU);
		poDU->SetSize(uiLogSize+M4_COMMS_LOG_LABEL_SIZE);
		m4pchar_t pcPUBuffer=poDU->GetBuffer();
		memcpy(pcPUBuffer,M4_COMMS_LOG_LABEL,M4_COMMS_LOG_LABEL_SIZE);
		memcpy(pcPUBuffer+M4_COMMS_LOG_LABEL_SIZE,pcLogBuffer,uiLogSize);
		m_poWritePDU->SetLogInfo(NULL,0);
		m_poWritePDU->DisableLogInfo();
	}
	*/
	return M4_SUCCESS;
  //## end ClCommPDUProtocol::AddLog%962101128.body
}

m4return_t ClCommPDUProtocol::SetVersion (m4int_t ai_iVersion)
{
  //## begin ClCommPDUProtocol::SetVersion%967707113.body preserve=yes
	m_iVersion=ai_iVersion;
	m_Protocol.SetVersion(ai_iVersion);
	return M4_SUCCESS;
  //## end ClCommPDUProtocol::SetVersion%967707113.body
}

m4int_t ClCommPDUProtocol::GetVersion ()
{
  //## begin ClCommPDUProtocol::GetVersion%967707114.body preserve=yes
	return m_iVersion;
  //## end ClCommPDUProtocol::GetVersion%967707114.body
}

// Additional Declarations
  //## begin ClCommPDUProtocol%390FEC49000A.declarations preserve=yes
  //## end ClCommPDUProtocol%390FEC49000A.declarations

// Class ClCommASCIIProtocol 





ClCommASCIIProtocol::ClCommASCIIProtocol ()
  //## begin ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.hasinit preserve=no
      : m_pcReadBuffer(NULL), m_iBufferSize(0), m_pcWriteBuffer(NULL), m_iSentBytes(0)
  //## end ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.hasinit
  //## begin ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.initialization preserve=yes
  //## end ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.initialization
{
  //## begin ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.body preserve=yes
  //## end ClCommASCIIProtocol::ClCommASCIIProtocol%957345701.body
}


ClCommASCIIProtocol::~ClCommASCIIProtocol ()
{
  //## begin ClCommASCIIProtocol::~ClCommASCIIProtocol%957345702.body preserve=yes
  //## end ClCommASCIIProtocol::~ClCommASCIIProtocol%957345702.body
}

//## Other Operations (implementation)
m4return_t ClCommASCIIProtocol::AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize)
{
  //## begin ClCommASCIIProtocol::AddChunk%957345703.body preserve=yes
	ao_uiUssed=0;
	ao_uiNextBLockSize=0;
	if(NULL==m_pcReadBuffer)
	{
		m_ui64StartReadingTime=ClPrecisionTicker::GetTimeTick();
		// problemas porque despues hacemos delete.
        //m_pcReadBuffer=(char *)calloc(1,ai_uiChunkSize+1) ;
        m_pcReadBuffer=(char *)cnew(1, ai_uiChunkSize+1);
		        
        memcpy(m_pcReadBuffer,ai_pcChunk,ai_uiChunkSize);
		m_iBufferSize=ai_uiChunkSize;
	}
	else
	{
		// problemas porque despues hacemos delete.
		//m_pcReadBuffer=(char *)realloc((void *)m_pcReadBuffer,m_iBufferSize+ai_uiChunkSize);
		m_pcReadBuffer=(char *)renew(m_pcReadBuffer, m_iBufferSize, ai_uiChunkSize);
		        
        memcpy(m_pcReadBuffer+m_iBufferSize,ai_pcChunk,ai_uiChunkSize);
		m_iBufferSize+=ai_uiChunkSize;
	}
	ao_uiUssed=ai_uiChunkSize;
	m_uiReadedBytes+=ai_uiChunkSize;

// Bastaria con esta comprobacion, pero haciendo esta otra vemos si hay algun error de protocolo
/*	
	if(ai_pcChunk[ai_uiChunkSize-1]==0)
	{
		m_uiNReceivedRequests++;
		m_dFinishReadingTime=m_oReadCrono.Stop();
		return M4_COMM_PROTOCOL_RETURN_COMPLETED;
	}
*/
	m4pchar_t pcCeroPosition = (m4pchar_t)memchr(ai_pcChunk, 0, ai_uiChunkSize);
	if (pcCeroPosition == NULL)
	{
		return M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
	}
	
	m_uiNReceivedRequests++;
	m_ui64FinishReadingTime = ClPrecisionTicker::GetTimeTick();
	return M4_COMM_PROTOCOL_RETURN_COMPLETED;
  //## end ClCommASCIIProtocol::AddChunk%957345703.body
}

m4return_t ClCommASCIIProtocol::IsCompleted ()
{
  //## begin ClCommASCIIProtocol::IsCompleted%957345704.body preserve=yes
	return M4_ERROR;
  //## end ClCommASCIIProtocol::IsCompleted%957345704.body
}

m4return_t ClCommASCIIProtocol::GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize)
{
  //## begin ClCommASCIIProtocol::GetNextChunk%957345705.body preserve=yes
	if(0==m_iSentBytes)
	{
		ao_pcChunk=m_pcWriteBuffer;
		if(m_pcWriteBuffer)
			m_iSentBytes=ao_uiChunkSize=m_iBufferSize;
		else
			m_iSentBytes=ao_uiChunkSize=0;
		m_uiSentBytes+=ao_uiChunkSize;
		m_uiNSentRequests++;
		return M4_SUCCESS;
	}
	else 
	{
		m_ui64StartWrittingTime=ClPrecisionTicker::GetTimeTick();
		m_uiNSentRequests++;
		ao_pcChunk=NULL;
		ao_uiChunkSize=0;
		return M4_ERROR;
	}

  //## end ClCommASCIIProtocol::GetNextChunk%957345705.body
}

m4return_t ClCommASCIIProtocol::GetFullChunk (m4pchar_t &ao_pcXML, m4uint_t &ao_uiXMLSize)
{
  //## begin ClCommASCIIProtocol::GetFullChunk%957345706.body preserve=yes
	return M4_ERROR;
  //## end ClCommASCIIProtocol::GetFullChunk%957345706.body
}

m4return_t ClCommASCIIProtocol::Initialize ()
{
  //## begin ClCommASCIIProtocol::Initialize%957517151.body preserve=yes
	m_iSentBytes=0;
	return M4_SUCCESS;
  //## end ClCommASCIIProtocol::Initialize%957517151.body
}

m4pchar_t ClCommASCIIProtocol::GetReadedBuffer ()
{
  //## begin ClCommASCIIProtocol::GetReadedBuffer%957856081.body preserve=yes
	return m_pcReadBuffer;
  //## end ClCommASCIIProtocol::GetReadedBuffer%957856081.body
}

m4return_t ClCommASCIIProtocol::ReleaseContents ()
{
  //## begin ClCommASCIIProtocol::ReleaseContents%957944568.body preserve=yes
	if(NULL==m_pcReadBuffer)
	{
		M4_SRV_DEBUG_N("El buffer ya era nulo!!!!");
		return M4_ERROR;
	}
	else 
	{
		m_pcReadBuffer=NULL;
		return M4_SUCCESS;
	}
  //## end ClCommASCIIProtocol::ReleaseContents%957944568.body
}

m4return_t ClCommASCIIProtocol::SetWriteBuffer (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClCommASCIIProtocol::SetWriteBuffer%958730350.body preserve=yes
if(m_bDump)
{
	m4char_t filename[256];
	sprintf(filename,"%lu-%lu.rpdu",this,m_uiNReceivedRequests);
	EnableWriteDump(filename);
}
	m_ui64StartWrittingTime=ClPrecisionTicker::GetTimeTick();
	m_pcWriteBuffer=ai_pcBuffer;
	m_iBufferSize=ai_iBufferSize;
	return M4_SUCCESS;
  //## end ClCommASCIIProtocol::SetWriteBuffer%958730350.body
}

// Additional Declarations
  //## begin ClCommASCIIProtocol%390FEDC2037D.declarations preserve=yes
  //## end ClCommASCIIProtocol%390FEDC2037D.declarations

// Class ClCommPDUHTTPProtocol 




ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol ()
  //## begin ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.hasinit preserve=no
      : m_pcWriteHeaders(NULL), m_pcReadedHeaders(NULL), m_bHeaderSent(M4_TRUE)
  //## end ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.hasinit
  //## begin ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.initialization preserve=yes
  //## end ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.initialization
{
  //## begin ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.body preserve=yes
  //## end ClCommPDUHTTPProtocol::ClCommPDUHTTPProtocol%972554298.body
}



//## Other Operations (implementation)
m4return_t ClCommPDUHTTPProtocol::SetWriteHeaders (m4pchar_t ai_pcWriteHeaders)
{
  //## begin ClCommPDUHTTPProtocol::SetWriteHeaders%972554295.body preserve=yes
	return M4_ERROR;
  //## end ClCommPDUHTTPProtocol::SetWriteHeaders%972554295.body
}

m4pchar_t ClCommPDUHTTPProtocol::GetReadedHeaders ()
{
  //## begin ClCommPDUHTTPProtocol::GetReadedHeaders%972554296.body preserve=yes
	return NULL;
  //## end ClCommPDUHTTPProtocol::GetReadedHeaders%972554296.body
}

m4return_t ClCommPDUHTTPProtocol::AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize)
{
  //## begin ClCommPDUHTTPProtocol::AddChunk%972554302.body preserve=yes
	return M4_ERROR;
  //## end ClCommPDUHTTPProtocol::AddChunk%972554302.body
}

m4return_t ClCommPDUHTTPProtocol::GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize)
{
  //## begin ClCommPDUHTTPProtocol::GetNextChunk%972554303.body preserve=yes
	m4pchar_t pcWriteBuffer=NULL;
	if(m_bHeaderSent)
	{
		if(NULL==m_pcWriteHeaders)
		{
			m4Trace(cerr << "NULL write HTTP Header"<<endl);
			return M4_ERROR;
		}
		m4int_t iHeaderSize=strlen(m_pcWriteHeaders);
		m_iProcessedBytes+=iHeaderSize;
		ao_uiChunkSize=iHeaderSize;
		ao_pcChunk=m_pcWriteBuffer;
		strcpy(m_pcWriteBuffer,m_pcWriteHeaders);
		m_bHeaderSent=M4_FALSE;
		pcWriteBuffer=m_pcWriteBuffer;
	}
	m4return_t iRet=ClCommPDUProtocol::GetNextChunk(ao_pcChunk,ao_uiChunkSize);
	return M4_ERROR;
  //## end ClCommPDUHTTPProtocol::GetNextChunk%972554303.body
}

// Additional Declarations
  //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.declarations preserve=yes
  //## end ClCommPDUHTTPProtocol%39F7FE6902A1.declarations

//## begin module%390FEF300275.epilog preserve=yes
//## end module%390FEF300275.epilog
