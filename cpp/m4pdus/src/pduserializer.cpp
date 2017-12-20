//## begin module%3977FF180194.cm preserve=no
//## end module%3977FF180194.cm

//## begin module%3977FF180194.cp preserve=no
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
//## end module%3977FF180194.cp

//## Module: pduserializer%3977FF180194; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\pduserializer.cpp

//## begin module%3977FF180194.additionalIncludes preserve=no
//## end module%3977FF180194.additionalIncludes

//## begin module%3977FF180194.includes preserve=yes
#ifdef _WINDOWS
#include <windows.h>  // Para las funciones que convierten los enteros a formato de red
#elif defined(_UNIX)
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif
//## end module%3977FF180194.includes

// m4types
#include <m4types.hpp>
// pduserializer
#include <pduserializer.hpp>
// pdutools
#include <pdutools.hpp>
//## begin module%3977FF180194.declarations preserve=no
//## end module%3977FF180194.declarations

//## begin module%3977FF180194.additionalDeclarations preserve=yes
//## end module%3977FF180194.additionalDeclarations


// Class ClAdvNetHost 



//## Other Operations (implementation)
m4pduuint32_t ClAdvNetHost::m4ntohul (m4pduuint32_t ai_ulNet)
{
  //## begin ClAdvNetHost::m4ntohul%877341252.body preserve=yes
	return ntohl( ai_ulNet );
  //## end ClAdvNetHost::m4ntohul%877341252.body
}

m4pduuint16_t ClAdvNetHost::m4ntohui (m4pduuint16_t ai_uiNet)
{
  //## begin ClAdvNetHost::m4ntohui%877341253.body preserve=yes
	return ntohs( ai_uiNet );
  //## end ClAdvNetHost::m4ntohui%877341253.body
}

m4pduuint16_t ClAdvNetHost::m4htonui (m4pduuint16_t ai_uiHost)
{
  //## begin ClAdvNetHost::m4htonui%877341254.body preserve=yes
	return htons( ai_uiHost );
  //## end ClAdvNetHost::m4htonui%877341254.body
}

m4pduuint32_t ClAdvNetHost::m4htonul (m4pduuint32_t ai_ulHost)
{
  //## begin ClAdvNetHost::m4htonul%877341255.body preserve=yes
	return htonl( ai_ulHost );
  //## end ClAdvNetHost::m4htonul%877341255.body
}

m4pduuint64_t ClAdvNetHost::m4ntohulw (m4pduuint64_t ai_ulwHost)
{
  //## begin ClAdvNetHost::m4ntohulw%880122148.body preserve=yes
  m4uint64_t ao_ulwHost;
  m4uint32_t low,hi;

  //  Descomposicion. 
  low = (m4uint32_t )   ai_ulwHost;
  hi  = (m4uint32_t )  (ai_ulwHost >> 32 );
  
  //  Transformacion.
  low =  m4ntohul ( low );
  hi  =  m4ntohul ( hi  );
  
  //  Composicion.
  ao_ulwHost  = (m4uint64_t ) hi ;
  ao_ulwHost  = ao_ulwHost << 32;
  ao_ulwHost += low;

  return ao_ulwHost;
  //## end ClAdvNetHost::m4ntohulw%880122148.body
}

m4pduuint64_t ClAdvNetHost::m4htonulw (m4pduuint64_t ai_ulwHost)
{
  //## begin ClAdvNetHost::m4htonulw%880122149.body preserve=yes

  m4uint64_t ao_ulwHost;
  m4uint32_t low,hi;

  //  Descomposicion. 
  low = (m4uint32_t )   ai_ulwHost;
  hi  = (m4uint32_t )  (ai_ulwHost >> 32 );
  
  //  Transformacion.
  low =  m4htonul ( low );
  hi  =  m4htonul ( hi  );
  
  //  Composicion.
  ao_ulwHost  = (m4uint64_t ) hi ;
  ao_ulwHost  = ao_ulwHost << 32;
  ao_ulwHost += low;

  return ao_ulwHost;

  //## end ClAdvNetHost::m4htonulw%880122149.body
}

// Additional Declarations
  //## begin ClAdvNetHost%344B138F003C.declarations preserve=yes
  //## end ClAdvNetHost%344B138F003C.declarations

// Class ClPDUAdvSerializer 







ClPDUAdvSerializer::ClPDUAdvSerializer ()
  //## begin ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.hasinit preserve=no
      : m_pBuffer(NULL), m_ulBufferSize(0), m_ulIterator(0), m_ulUsedBuffer(0)
  //## end ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.hasinit
  //## begin ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.initialization preserve=yes
  ,m_bBufferOwner(M4_FALSE)
  //## end ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.initialization
{
  //## begin ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.body preserve=yes
//	m_ulBufferSize=200;
//	m_pBuffer=(char *)calloc(1,m_ulBufferSize);
  //## end ClPDUAdvSerializer::ClPDUAdvSerializer%963471068.body
}


ClPDUAdvSerializer::~ClPDUAdvSerializer ()
{
  //## begin ClPDUAdvSerializer::~ClPDUAdvSerializer%963471069.body preserve=yes
	if((m_bBufferOwner) && (NULL!=m_pBuffer))
		free (m_pBuffer);
  //## end ClPDUAdvSerializer::~ClPDUAdvSerializer%963471069.body
}



//## Other Operations (implementation)
m4return_t ClPDUAdvSerializer::AddUShort (m4uint8_t ai_usNum)
{
  //## begin ClPDUAdvSerializer::AddUShort%963471070.body preserve=yes

	if(M4_ERROR==Check4Space(M4_PDU_ALIGN_SIZE))
		return M4_ERROR;
	m4pchar_t	pBufferTemp = m_pBuffer+ m_ulUsedBuffer;

	m_ulUsedBuffer+= M4_PDU_ALIGN_SIZE;

	memset(pBufferTemp,0,M4_PDU_ALIGN_SIZE);
	m4uint8_t	sn = (m4uint8_t )m_oNH.m4htonui(ai_usNum);
	*(( m4uint8_t *) &pBufferTemp[0]) = sn;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUShort%963471070.body
}

m4return_t ClPDUAdvSerializer::AddUInt (m4uint16_t ai_uiNum)
{
  //## begin ClPDUAdvSerializer::AddUInt%963471071.body preserve=yes
	if(M4_ERROR==Check4Space(M4_PDU_ALIGN_SIZE))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer + m_ulUsedBuffer;
	m_ulUsedBuffer+= M4_PDU_ALIGN_SIZE;

	memset(pBufferTemp,0,M4_PDU_ALIGN_SIZE);
	m4uint16_t in = m_oNH.m4htonui(ai_uiNum);
	*(( m4uint16_t *) &pBufferTemp[0]) = in;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUInt%963471071.body
}

m4return_t ClPDUAdvSerializer::AddULong (m4uint32_t ai_ulNum)
{
  //## begin ClPDUAdvSerializer::AddULong%963471072.body preserve=yes

	if(M4_ERROR==Check4Space(M4_PDU_ALIGN_SIZE))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer+ m_ulUsedBuffer;
	m_ulUsedBuffer += M4_PDU_ALIGN_SIZE;

	memset(pBufferTemp,0,M4_PDU_ALIGN_SIZE);
	m4uint32_t	 ln = m_oNH.m4htonul(ai_ulNum);
	*(( m4uint32_t *) &pBufferTemp[0]) = ln;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddULong%963471072.body
}

m4return_t ClPDUAdvSerializer::AddUWLong (m4uint64_t ai_uwlNum)
{
  //## begin ClPDUAdvSerializer::AddUWLong%963471073.body preserve=yes

	if(M4_ERROR==Check4Space(M4_PDU_ALIGN_SIZE))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer+ m_ulUsedBuffer;
	m_ulUsedBuffer += M4_PDU_ALIGN_SIZE;

	memset(pBufferTemp,0,M4_PDU_ALIGN_SIZE);
	m4uint64_t	wln = m_oNH.m4htonulw(ai_uwlNum);
	*(( m4uint64_t *) &pBufferTemp[0]) = wln;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUWLong%963471073.body
}

m4return_t ClPDUAdvSerializer::AddpBuffer (m4pchar_t ai_pBuffer)
{
  //## begin ClPDUAdvSerializer::AddpBuffer%963471074.body preserve=yes
	M4_ASSERT(1==2);
	return M4_ERROR;
  //## end ClPDUAdvSerializer::AddpBuffer%963471074.body
}

m4return_t ClPDUAdvSerializer::AddpBufferC (m4pchar_t ai_pBuffer, m4uint32_t ai_ulBufferSize)
{
  //## begin ClPDUAdvSerializer::AddpBufferC%963471075.body preserve=yes
	if(M4_ERROR==Check4Space(ai_ulBufferSize))
		return M4_ERROR;
	
	m4pchar_t pBufferTemp = m_pBuffer+ m_ulUsedBuffer;
	m_ulUsedBuffer += ai_ulBufferSize;

	memcpy( pBufferTemp, ai_pBuffer, ai_ulBufferSize );

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddpBufferC%963471075.body
}

m4return_t ClPDUAdvSerializer::AddBlockFormat (m4uint32_t ai_ulFormat)
{
  //## begin ClPDUAdvSerializer::AddBlockFormat%963471076.body preserve=yes
	if(M4_ERROR==Check4Space(4))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer+ m_ulUsedBuffer;
	m_ulUsedBuffer += 4;

	m4uint32_t ln = m_oNH.m4htonul(ai_ulFormat);
	*(( m4uint32_t *) &pBufferTemp[0]) = ln;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddBlockFormat%963471076.body
}

m4return_t ClPDUAdvSerializer::AddPairIntpChar (ClPairIntpChar *ai_pData)
{
  //## begin ClPDUAdvSerializer::AddPairIntpChar%963471077.body preserve=yes
	M4_ASSERT(1==2);
	return M4_ERROR;
  //## end ClPDUAdvSerializer::AddPairIntpChar%963471077.body
}

m4pchar_t ClPDUAdvSerializer::GetSerializedBuffer ()
{
  //## begin ClPDUAdvSerializer::GetSerializedBuffer%963471078.body preserve=yes
	return m_pBuffer;
  //## end ClPDUAdvSerializer::GetSerializedBuffer%963471078.body
}

m4return_t ClPDUAdvSerializer::SetpBuffer (m4pchar_t ai_pBuffer)
{
  //## begin ClPDUAdvSerializer::SetpBuffer%963471079.body preserve=yes
	if(m_bBufferOwner && (NULL!=m_pBuffer))
	{
		free (m_pBuffer);
		m_bBufferOwner=M4_FALSE;
	}

  m_pBuffer = ai_pBuffer;
  return Initialize();
  //## end ClPDUAdvSerializer::SetpBuffer%963471079.body
}

m4return_t ClPDUAdvSerializer::Initialize ()
{
  //## begin ClPDUAdvSerializer::Initialize%963471080.body preserve=yes
	m_ulUsedBuffer=0;
	m_ulIterator = 0;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::Initialize%963471080.body
}

m4return_t ClPDUAdvSerializer::Initialize (m4pchar_t ai_pBuffer, m4uint32_t ai_ulBufferSize)
{
  //## begin ClPDUAdvSerializer::Initialize%963471081.body preserve=yes
	Initialize();
	m_pBuffer = ai_pBuffer;
	m_ulBufferSize = ai_ulBufferSize;
    m_ulIterator = 0;

	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::Initialize%963471081.body
}

m4uint32_t ClPDUAdvSerializer::GetBufferSize ()
{
  //## begin ClPDUAdvSerializer::GetBufferSize%963471082.body preserve=yes
 	return m_ulUsedBuffer;
  //## end ClPDUAdvSerializer::GetBufferSize%963471082.body
}

m4uint8_t ClPDUAdvSerializer::_GetUShort ()
{
  //## begin ClPDUAdvSerializer::_GetUShort%963471083.body preserve=yes
	m4pchar_t pBufferTemp = m_pBuffer+ m_ulIterator;
    m4uint8_t sn = (m4uint8_t )m_oNH.m4ntohui( *((m4uint8_t *) (pBufferTemp)));
	m_ulIterator+=M4_PDU_ALIGN_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::_GetUShort%963471083.body
}

m4uint16_t ClPDUAdvSerializer::_GetUInt ()
{
  //## begin ClPDUAdvSerializer::_GetUInt%963471084.body preserve=yes
 	m4pchar_t pBufferTemp = m_pBuffer+ m_ulIterator;
    m4uint16_t	 sn = (m4uint16_t) m_oNH.m4ntohui( *((m4uint16_t *) (pBufferTemp)));
	m_ulIterator+=M4_PDU_ALIGN_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::_GetUInt%963471084.body
}

m4uint32_t ClPDUAdvSerializer::_GetULong ()
{
  //## begin ClPDUAdvSerializer::_GetULong%963471085.body preserve=yes
	m4pchar_t pBufferTemp = m_pBuffer+ m_ulIterator;
    m4uint32_t	sn = m_oNH.m4ntohul( *((m4uint32_t *) (pBufferTemp)));
	m_ulIterator+=M4_PDU_ALIGN_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::_GetULong%963471085.body
}

m4uint64_t ClPDUAdvSerializer::_GetUWLong ()
{
  //## begin ClPDUAdvSerializer::_GetUWLong%963471086.body preserve=yes
	m4pchar_t pBufferTemp = m_pBuffer+ m_ulIterator;
    m4uint64_t	sn = m_oNH.m4ntohulw( *((m4uint64_t *) (pBufferTemp)));
	m_ulIterator+=M4_PDU_ALIGN_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::_GetUWLong%963471086.body
}

m4pchar_t ClPDUAdvSerializer::GetpBuffers (m4uint32_t ai_ulBufferSize)
{
  //## begin ClPDUAdvSerializer::GetpBuffers%963471087.body preserve=yes
	M4_ASSERT(1==2);
	return NULL;
  //## end ClPDUAdvSerializer::GetpBuffers%963471087.body
}

m4uint32_t ClPDUAdvSerializer::GetBlockFormat ()
{
  //## begin ClPDUAdvSerializer::GetBlockFormat%963471088.body preserve=yes
	m4pchar_t pBufferTemp = m_pBuffer+ m_ulIterator;
    m4uint32_t	 sn = m_oNH.m4ntohul( *((m4uint32_t *) (pBufferTemp)));
	m_ulIterator+=4;
	return sn;
  //## end ClPDUAdvSerializer::GetBlockFormat%963471088.body
}

m4return_t ClPDUAdvSerializer::Check4Space (m4uint32_t ai_uiNeededSpace)
{
  //## begin ClPDUAdvSerializer::Check4Space%963492921.body preserve=yes
	m4uint32_t uifreespace=m_ulBufferSize - m_ulUsedBuffer;
	if(uifreespace<ai_uiNeededSpace)
	{
		if((m_ulBufferSize ==0) && (NULL!=m_pBuffer) && (!m_bBufferOwner) )
		{
			m_bBufferOwner=M4_TRUE;
			m_pBuffer=NULL;
		}

		m_ulBufferSize +=ai_uiNeededSpace>=100?ai_uiNeededSpace+50:100; 

		if(m_pBuffer )
		{
			m_pBuffer = (m4pchar_t)realloc( m_pBuffer, m_ulBufferSize);
		}
		else
		{
			m_pBuffer = (m4pchar_t)calloc( m_ulBufferSize,1);
		}
		m_bBufferOwner=M4_TRUE;
		M4_ASSERT(NULL!=m_pBuffer);
		if(NULL==m_pBuffer)
			return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::Check4Space%963492921.body
}

m4uint8_t ClPDUAdvSerializer::GetUInt8 ()
{
  //## begin ClPDUAdvSerializer::GetUInt8%964092281.body preserve=yes
  	m4uint8_t	sn = *((m4uint8_t *) (m_pBuffer+ m_ulIterator));
	m_ulIterator+=M4_PDUSERIALIZER_UINT8_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::GetUInt8%964092281.body
}

m4uint16_t ClPDUAdvSerializer::GetUInt16 ()
{
  //## begin ClPDUAdvSerializer::GetUInt16%964092282.body preserve=yes
	m4uint16_t	  sn = (m4uint16_t) m_oNH.m4ntohui( *((m4uint16_t *) (m_pBuffer+ m_ulIterator)));
	m_ulIterator+=M4_PDUSERIALIZER_UINT16_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::GetUInt16%964092282.body
}

m4uint32_t ClPDUAdvSerializer::GetUInt32 ()
{
  //## begin ClPDUAdvSerializer::GetUInt32%964092283.body preserve=yes
	m4uint32_t	sn = m_oNH.m4ntohul( *((m4uint32_t *) (m_pBuffer+ m_ulIterator)));
	m_ulIterator+=M4_PDUSERIALIZER_UINT32_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::GetUInt32%964092283.body
}

m4uint64_t ClPDUAdvSerializer::GetUInt64 ()
{
  //## begin ClPDUAdvSerializer::GetUInt64%964092284.body preserve=yes
	m4uint64_t	sn = m_oNH.m4ntohulw( *((m4uint64_t *) (m_pBuffer+ m_ulIterator)));
	m_ulIterator+=M4_PDUSERIALIZER_UINT64_SIZE;
	return sn;
  //## end ClPDUAdvSerializer::GetUInt64%964092284.body
}

m4return_t ClPDUAdvSerializer::SkipBytes (m4int_t ai_iNBytes)
{
  //## begin ClPDUAdvSerializer::SkipBytes%964092285.body preserve=yes
	m_ulIterator+=ai_iNBytes;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::SkipBytes%964092285.body
}

m4return_t ClPDUAdvSerializer::AddUInt8 (m4uint8_t ai_usNum)
{
  //## begin ClPDUAdvSerializer::AddUInt8%964101575.body preserve=yes
	if(M4_ERROR==Check4Space(M4_PDUSERIALIZER_UINT8_SIZE))
		return M4_ERROR;
	m4pchar_t pBufferTemp = m_pBuffer + m_ulUsedBuffer;
	m_ulUsedBuffer+= M4_PDUSERIALIZER_UINT8_SIZE;
	m4uint8_t	sn = ai_usNum;
	*(( m4uint8_t *) &pBufferTemp[0]) = sn;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUInt8%964101575.body
}

m4return_t ClPDUAdvSerializer::AddUInt16 (m4uint16_t ai_uiNum)
{
  //## begin ClPDUAdvSerializer::AddUInt16%964101576.body preserve=yes
	if(M4_ERROR==Check4Space(M4_PDUSERIALIZER_UINT16_SIZE))
		return M4_ERROR;
	m4pchar_t	pBufferTemp = m_pBuffer+m_ulUsedBuffer;
	m_ulUsedBuffer+= M4_PDUSERIALIZER_UINT16_SIZE;
	m4uint16_t	 in = m_oNH.m4htonui(ai_uiNum);
	*(( m4uint16_t *) &pBufferTemp[0]) = in;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUInt16%964101576.body
}

m4return_t ClPDUAdvSerializer::AddUInt32 (m4uint32_t ai_ulNum)
{
  //## begin ClPDUAdvSerializer::AddUInt32%964101577.body preserve=yes
	if(M4_ERROR==Check4Space(M4_PDUSERIALIZER_UINT32_SIZE))
		return M4_ERROR;
	m4pchar_t	pBufferTemp =  m_pBuffer+ m_ulUsedBuffer;
	m_ulUsedBuffer += M4_PDUSERIALIZER_UINT32_SIZE;
	m4uint32_t	ln =m_oNH.m4htonul(ai_ulNum);
	*(( m4uint32_t *) &pBufferTemp[0]) = ln;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUInt32%964101577.body
}

m4return_t ClPDUAdvSerializer::AddUInt64 (m4uint64_t ai_uwlNum)
{
  //## begin ClPDUAdvSerializer::AddUInt64%964101578.body preserve=yes
	if(M4_ERROR==Check4Space(M4_PDUSERIALIZER_UINT64_SIZE))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer + m_ulUsedBuffer;
	m_ulUsedBuffer += M4_PDUSERIALIZER_UINT64_SIZE;

	m4uint64_t wln = m_oNH.m4htonulw(ai_uwlNum);
	*(( m4uint64_t *) &pBufferTemp[0]) = wln;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddUInt64%964101578.body
}

m4return_t ClPDUAdvSerializer::AddZero (m4int_t ai_iBytes)
{
  //## begin ClPDUAdvSerializer::AddZero%964423559.body preserve=yes
	if(M4_ERROR==Check4Space(ai_iBytes))
		return M4_ERROR;

	m4pchar_t pBufferTemp = m_pBuffer + m_ulUsedBuffer;
	memset(pBufferTemp ,0,ai_iBytes);
	m_ulUsedBuffer += ai_iBytes;
	return M4_SUCCESS;
  //## end ClPDUAdvSerializer::AddZero%964423559.body
}

// Additional Declarations
  //## begin ClPDUAdvSerializer%396D652C004D.declarations preserve=yes
  //## end ClPDUAdvSerializer%396D652C004D.declarations

//## begin module%3977FF180194.epilog preserve=yes
//## end module%3977FF180194.epilog
