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
//## Source file: e:\source\m4communication\src\commprotocols.cpp

//## begin module%390FEF300275.additionalIncludes preserve=no
//## end module%390FEF300275.additionalIncludes

//## begin module%390FEF300275.includes preserve=yes
#include <m4trace.hpp>
//## end module%390FEF300275.includes

// m4types
#include <m4types.hpp>
// commprotocols
#include <pduprotocols.hpp>
// Protocol
#include <protocol.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%390FEF300275.declarations preserve=no
//## end module%390FEF300275.declarations

//## begin module%390FEF300275.additionalDeclarations preserve=yes
#define M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE 262144
m4pcchar_t M4_COMMS_LOG_LABEL="#*#*#*#*LOG_INFO*#*#*#*#";
const m4int_t  M4_COMMS_LOG_LABEL_SIZE=24;
//## end module%390FEF300275.additionalDeclarations



// Class ClPDUProtocol 


ClPDUProtocol::ClPDUProtocol ()
  //## begin ClPDUProtocol::ClPDUProtocol%957345693.hasinit preserve=no
      : m_poReadPDU(NULL), m_iProcessedBytes(0), m_iNextBlockSize(0), m_pcNextBlockBuffer(NULL), m_poWritePDU(NULL), m_pcWriteBuffer(NULL), m_iWriteBufferSize(0)
  //## end ClPDUProtocol::ClPDUProtocol%957345693.hasinit
  //## begin ClPDUProtocol::ClPDUProtocol%957345693.initialization preserve=yes
  //## end ClPDUProtocol::ClPDUProtocol%957345693.initialization
{
  //## begin ClPDUProtocol::ClPDUProtocol%957345693.body preserve=yes
	m_pcWriteBuffer=new m4char_t[M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE];
	m_iWriteBufferSize=M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE;
  //## end ClPDUProtocol::ClPDUProtocol%957345693.body
}


ClPDUProtocol::~ClPDUProtocol ()
{
  //## begin ClPDUProtocol::~ClPDUProtocol%957345694.body preserve=yes
	if(m_pcWriteBuffer)
		delete m_pcWriteBuffer;
  //## end ClPDUProtocol::~ClPDUProtocol%957345694.body
}



//## Other Operations (implementation)
m4return_t ClPDUProtocol::AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize)
{
  //## begin ClPDUProtocol::AddChunk%957345695.body preserve=yes
	ao_uiUssed=0;
	if(NULL==m_poReadPDU)
	{
		int iHeadSize=m_Protocol.GetHeadSize ();
		if(iHeadSize>ai_uiChunkSize)
		{
			ao_uiNextBLockSize=iHeadSize;
			return M4_PDU_COMM_PROTOCOL_RETURN_CONTINUE_READING;
		}
		else
		{
			ai_uiChunkSize -=iHeadSize;
			m_poReadPDU=m_Protocol.GetPDU (ai_pcChunk) ;
			ai_pcChunk+=iHeadSize;
			ao_uiUssed+=iHeadSize;
			if ( NULL ==m_poReadPDU )
			{
				m4Trace(cerr <<"Error obtaining PDU from Protocol. Deberia abortar la conexion"<<endl);
				return M4_PDU_COMM_PROTOCOL_RETURN_ERROR ;
			}
		}
	}
	
	m4return_t iRet;
	while(ai_uiChunkSize>=0)
	{
		if(0==m_iNextBlockSize  )
		{
			m_iNextBlockSize  = m_poReadPDU -> GetNextSizeBlock(m_pcNextBlockBuffer);
		}

		if( 0==m_iNextBlockSize)
		{
			m_uiReadedBytes+=ao_uiUssed;
			if(ai_uiChunkSize!=0)
			{
				m4Trace(cerr << "Error in PDU format:"<< ai_uiChunkSize<< " unexpected bytes"<<endl);
				if(ai_uiChunkSize=atoi(ai_pcChunk))
				{
					m4Trace(cerr<< "Old format log"<<endl);
					m_poReadPDU->SetLogInfo (ai_pcChunk,ai_uiChunkSize);
					return M4_PDU_COMM_PROTOCOL_RETURN_COMPLETED;
				}
				delete m_poReadPDU;
				m_poReadPDU=NULL;
				return M4_PDU_COMM_PROTOCOL_RETURN_ERROR ;
			}
			m4TraceLevel(2,cout<<"PDU Completa!!!" <<endl);
			ExtractLog();
			return M4_PDU_COMM_PROTOCOL_RETURN_COMPLETED;
		}
		if(ai_uiChunkSize>=m_iNextBlockSize)
		{
			memcpy(m_pcNextBlockBuffer,ai_pcChunk,m_iNextBlockSize);
			iRet=m_poReadPDU->AddBlock(m_pcNextBlockBuffer);
			m4TraceLevel(3,cerr <<"Addblock:"<<m_iNextBlockSize<<endl);
			if(M4_ERROR==iRet)
			{
				m4Trace(cerr << "Error in PDU format"<<endl);
				delete m_poReadPDU;
				m_poReadPDU=NULL;
				return M4_PDU_COMM_PROTOCOL_RETURN_ERROR ;
			}
			else
			{
				ai_pcChunk+=m_iNextBlockSize;
				ai_uiChunkSize-=m_iNextBlockSize;
				ao_uiUssed+=m_iNextBlockSize;
				m_iNextBlockSize=0;
			}
		} 
		else
		{
			ao_uiNextBLockSize=m_iNextBlockSize;
			m_uiReadedBytes+=ao_uiUssed;
			return M4_PDU_COMM_PROTOCOL_RETURN_CONTINUE_READING;
		}
	}// While
	
	return M4_PDU_COMM_PROTOCOL_RETURN_CONTINUE_READING;
  //## end ClPDUProtocol::AddChunk%957345695.body
}


m4return_t ClPDUProtocol::GetReadedPDU (ClPDU *&ao_pPDU)
{
  //## begin ClPDUProtocol::GetReadedPDU%957345698.body preserve=yes
	ao_pPDU=m_poReadPDU;
	return M4_SUCCESS;
  //## end ClPDUProtocol::GetReadedPDU%957345698.body
}

m4return_t ClPDUProtocol::SetWritePDU (ClPDU *ai_pPDU)
{
  //## begin ClPDUProtocol::SetWritePDU%957364994.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	if(m_poWritePDU)
		iRet=M4_WARNING;
	m_poWritePDU=ai_pPDU;
	AddLog();
	return iRet;
  //## end ClPDUProtocol::SetWritePDU%957364994.body
}

m4return_t ClPDUProtocol::Initialize ()
{
  //## begin ClPDUProtocol::Initialize%957517150.body preserve=yes

	m_iProcessedBytes=0;
	m_pLostBlock = NULL;
	m_iSizeLostBlock = 0;

	return M4_SUCCESS;
  //## end ClPDUProtocol::Initialize%957517150.body
}

m4return_t ClPDUProtocol::ReleaseContents ()
{
  //## begin ClPDUProtocol::ReleaseContents%957944567.body preserve=yes
	if(NULL==m_poReadPDU)
	{
		m4Trace(cerr<< "La PDU ya era nula!!!!!"<<endl);
		return M4_ERROR;
	}
	else 
	{
	//	m_iProcessedBytes=0;   Solo se usa en Write y el release es para Read
		m_poReadPDU=NULL;
	
		return M4_SUCCESS;
	}
  //## end ClPDUProtocol::ReleaseContents%957944567.body
}

m4return_t ClPDUProtocol::GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize)
{
  //## begin ClPDUProtocol::GetNextChunk%959266651.body preserve=yes

	if(NULL==m_poWritePDU)
	{
		m4Trace(cerr<< "NULL PDU in Protocol");
		return M4_ERROR;
	}
	if (m_iProcessedBytes == -1) //En el ciclo anterior leímos una pdu completa
	{
		m_iProcessedBytes = 0;
		return M4_ERROR;
	}


	unsigned int	uiChunkSize,
					uiTotalChunkSize = 0;
	unsigned long	AuxLong;
	char * pBuffer = m_pcWriteBuffer;
	m4pchar_t pcPDUBuffer;
	m4return_t	ret;

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

				//Si tenemos algo almacenado, enviarlo, y enviar el bloque goedo en la siguiente llamada
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
		
			m_iProcessedBytes = -1;
			ao_pcChunk = m_pcWriteBuffer;
			ao_uiChunkSize = uiTotalChunkSize;
			if (ao_uiChunkSize)
			{
				return M4_SUCCESS; //Hay que enviar lo alamcenado
			}
			else
			{
				return M4_ERROR; // Significa que hemos terminado con la PDU, y hemos enmviado todo
			}
		}
	}
	

  //## end ClPDUProtocol::GetNextChunk%959266651.body
}

m4return_t ClPDUProtocol::ExtractLog ()
{
  //## begin ClPDUProtocol::ExtractLog%962101127.body preserve=yes
	
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogInfoSize;

	m4uint32_t uiDataUnit=0;
	M4DataStorage *poDS =m_poReadPDU->GetDataStorage ();
	if(NULL!=poDS )
	{
		ClDataUnitInterface *poDUI;
		ClDataUnit * poDU;
		poDS->InitializeIteration();
		while(M4_SUCCESS==	poDS ->GetNext(poDUI))
		{
			if(poDUI ->GetType()==M4_DATA_UNIT)
			{
				poDU=(ClDataUnit *)poDUI;
				uiLogInfoSize=poDU->GetSize();
				if(uiLogInfoSize>=M4_COMMS_LOG_LABEL_SIZE)
				{
					pcLogBuffer=poDU->GetBuffer();
					if(!strncmp(pcLogBuffer,M4_COMMS_LOG_LABEL,M4_COMMS_LOG_LABEL_SIZE))
					{
						m_poReadPDU->SetLogInfo (pcLogBuffer+M4_COMMS_LOG_LABEL_SIZE,uiLogInfoSize-M4_COMMS_LOG_LABEL_SIZE);
						poDS->Remove(uiDataUnit);
						break;
					}
				}
			}
			uiDataUnit++;
		}
	}

	return M4_SUCCESS;
  //## end ClPDUProtocol::ExtractLog%962101127.body
}

m4return_t ClPDUProtocol::AddLog ()
{
  //## begin ClPDUProtocol::AddLog%962101128.body preserve=yes
	
	m4pchar_t pcLogBuffer;
	m4uint32_t uiLogSize;
	m4return_t iRet=m_poWritePDU->GetLogInfo(pcLogBuffer,uiLogSize);
	
	if((M4_ERROR!=iRet) && (uiLogSize>0))
	{
		m4pchar_t loginfo=new m4char_t[uiLogSize+M4_COMMS_LOG_LABEL_SIZE];
		memcpy(loginfo,M4_COMMS_LOG_LABEL,M4_COMMS_LOG_LABEL_SIZE);
		memcpy(loginfo+M4_COMMS_LOG_LABEL_SIZE,pcLogBuffer,uiLogSize);
		
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
		delete loginfo;
		m_poWritePDU->SetLogInfo(NULL,0);
		m_poWritePDU->DisableLogInfo();
	}
	return M4_SUCCESS;
  //## end ClPDUProtocol::AddLog%962101128.body
}

// Additional Declarations
  //## begin ClPDUProtocol%390FEC49000A.declarations preserve=yes
  //## end ClPDUProtocol%390FEC49000A.declarations



//## begin module%390FEF300275.epilog preserve=yes
//## end module%390FEF300275.epilog


