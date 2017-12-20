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
//          zdelta code
//	=====================================================================


#include "commprotocols.hpp"
#include "basiclog.hpp"
#include "zdlib.h"
#include "m4compress.hpp"
#include "clienttag.hpp"
#include "m4props.hpp"
#include "m4clock.hpp"
#include "m4regimpl.hpp"
#include "m4srvres.hpp"
#include "m4srvtrace.h"

#ifdef _UNIX
#   include <sys/types.h>
#   include <netinet/in.h>
#endif

#define M4_SRV_TRACE_COMP_ID "COMM"


/*
 * class M4ZDeltaManager
 */
ClMutex M4ZDeltaManager::m_InMutex(M4_TRUE);
ClMutex M4ZDeltaManager::m_OutMutex(M4_TRUE);
M4ZDeltaManager* M4ZDeltaManager::m_poInstance = NULL;

// singleton factory.
M4ZDeltaManager* M4ZDeltaManager::GetInstance()
{
    if (NULL != m_poInstance)
    {
        return m_poInstance;
    }

    m_InMutex.Lock();
    if (NULL == m_poInstance)
    {
        m_poInstance = new M4ZDeltaManager();
    }
    m_InMutex.Unlock();
    return m_poInstance;
}

M4ZDeltaManager::M4ZDeltaManager()
{
    m_isTraceEnable = M4_FALSE;
    m_InMapSize = m_OutMapSize = 0;
}



void M4ZDeltaManager::erase(m4uint64_t ai_session)
{
    m4uint32_t uiInitTickCount = ClPrecisionTicker::GetTimeTick();
    erase_in(ai_session);
    erase_out(ai_session);
    m4uint32_t uiEndTickCount = ClPrecisionTicker::GetTimeTick();
    
    trace(uiInitTickCount, ai_session, 0, "ERASE", 0, 0, 0, uiEndTickCount);
}


// operations over the output map.
M4ZDeltaElementMapIt_t M4ZDeltaManager::find_out(m4uint64_t ai_session)
{
    return _find(m_oReferenceOutMap, ai_session);
}

M4ZDeltaElementMapIt_t M4ZDeltaManager::end_out()
{
    return _end(m_oReferenceOutMap);
}

size_t M4ZDeltaManager::erase_out(m4uint64_t ai_session)
{
    m4uint32_t iCurrentSize = 0;
    M4ZDeltaElementMapIt_t it = find_out(ai_session);
	if (end_out() != it) 
    {
        iCurrentSize = (*it).second.size;
    }    

    ClBlockSync oBlock(m_OutMutex);
    m_OutMapSize -= iCurrentSize;
    return _erase(m_oReferenceOutMap, ai_session);
}

void M4ZDeltaManager::insert_out(m4uint64_t ai_session, M4ZDeltaElement& ai_oElm)
{
    ClMutBlock oBlock(&m_OutMutex);
    m_OutMapSize += ai_oElm.size;
    _insert(m_oReferenceOutMap, ai_session, ai_oElm);
}


// operations over the input map.
M4ZDeltaElementMapIt_t M4ZDeltaManager::find_in(m4uint64_t ai_session)
{
    return _find(m_oReferenceInMap, ai_session);
}

M4ZDeltaElementMapIt_t M4ZDeltaManager::end_in()
{
    return _end(m_oReferenceInMap);
}

size_t M4ZDeltaManager::erase_in(m4uint64_t ai_session)
{
    m4uint32_t iCurrentSize = 0;
    M4ZDeltaElementMapIt_t it = find_in(ai_session);
	if (end_in() != it) 
    {
        iCurrentSize = (*it).second.size;
    }    

    ClMutBlock oBlock(&m_InMutex);
    m_InMapSize -= iCurrentSize;
    return _erase(m_oReferenceInMap, ai_session);
}

void M4ZDeltaManager::insert_in(m4uint64_t ai_session, M4ZDeltaElement& ai_oElm)
{
    ClMutBlock oBlock(&m_InMutex);
    m_InMapSize += ai_oElm.size;
    _insert(m_oReferenceInMap, ai_session, ai_oElm);
}

    
// private operations.
M4ZDeltaElementMapIt_t M4ZDeltaManager::_find(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session)
{
    return ai_map.find(ai_session);
}

M4ZDeltaElementMapIt_t M4ZDeltaManager::_end(M4ZDeltaElementMap_t& ai_map)
{
    return ai_map.end();
}

size_t M4ZDeltaManager::_erase(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session)
{
    return ai_map.erase(ai_session);
}

void M4ZDeltaManager::_insert(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session, M4ZDeltaElement& ai_oElm)
{
    ai_map.insert(M4ZDeltaElementMap_t::value_type(ai_session, ai_oElm));
}

void M4ZDeltaManager::trace(m4uint32_t ai_uiInitTickCount, 
        m4uint64_t ai_iSessionId, m4uint32_t ai_iIdentifier, m4pcchar_t ai_pcOperationId, 
        m4uint32_t ai_uiSourceSize, m4uint32_t ai_uiReferenceSize, m4uint32_t ai_uiTargetSize, 
        m4uint32_t ai_uiEndTickCount)
{
    // check if trace is enable.
    ClVMDebugEnv* poVmDebugEnv = ClVMDebugEnv::getInstance();
    if (NULL == poVmDebugEnv)
    {
        return;
    }
    if (poVmDebugEnv->getDebugLevel() >= 2)
    {
        if (M4_TRUE != m_isTraceEnable)
        {
            m4pcchar_t pcDebugDir = poVmDebugEnv->getDebugDir();
            setTrace(M4_TRUE, pcDebugDir);
        }
    }
    else
    {
        setTrace(M4_FALSE, "");
    }


    if (M4_TRUE != m_isTraceEnable)
    {
        return;
    }

    m4char_t buffer[256];
	m4uint32_t iSessionId, iPreKey ;
	
	ClPDUCommZDelta::ExtractKey( ai_iSessionId, iSessionId, iPreKey ) ;
	
    sprintf(buffer, "%lu\t%lu\t%lu\t%lu\t%s\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu",
        ai_uiInitTickCount, 
        iPreKey, iSessionId, ai_iIdentifier, ai_pcOperationId, 
        ai_uiSourceSize, ai_uiReferenceSize, ai_uiTargetSize, 
        ai_uiEndTickCount,
        m_InMapSize, m_OutMapSize);

    m_traceFile.PrintLn(buffer);
}

void M4ZDeltaManager::setTrace(m4bool_t ai_enable, m4string_t ai_traceDirName)
{
    m4return_t retCode = M4_ERROR;

    // disable traces.
    if (M4_FALSE == ai_enable)
    {
        // close current file.
        m_isTraceEnable = M4_FALSE;
        return;
    }
       
    // enable traces.
    if (M4_TRUE == m_isTraceEnable)
    {
        return;
    }

    // creates the file.
#ifdef _UNIX
#   define M4_FILE_SEPARATOR	"/"
#else
#   define M4_FILE_SEPARATOR	"\\"
#endif
    m4string_t fileName = "zdeltatrace.txt";
    m4string_t fullFileName = ai_traceDirName + M4_FILE_SEPARATOR + fileName;
    retCode = m_traceFile.SetFile(fullFileName);
    if (M4_SUCCESS != retCode)
    {
        return;
    }

    // sets header.
    m4string_t sHeader = m4string_t("InitTickCount") 
		+ '\t' + m4string_t("PreKey")
        + '\t' + m4string_t("SessionId")
        + '\t' + m4string_t("Identifier")
        + '\t' + m4string_t("OperationId")
        + '\t' + m4string_t("SourceSize")
        + '\t' + m4string_t("ReferenceSize")
        + '\t' + m4string_t("TargetSize")
        + '\t' + m4string_t("EndTickCount")
        + '\t' + m4string_t("InputMapSize")
        + '\t' + m4string_t("OutputMapSize");
    m_traceFile.SetHeader(sHeader);
    
    // enable traces.
    m_isTraceEnable = ai_enable;
}


/*
 * class ClPDUWithDataForZDelta
 */
M4DataStorage* ClPDUWithDataForZDelta::GetDataStorageWrapper() 
{
    return m_poDataStorage;
}

// bg 153381
ClMutex		ClPDUCommZDelta::m_oMutex( M4_TRUE ) ;
m4int32_t	ClPDUCommZDelta::s_iInstCounter = 0 ;

/*
 * class ClPDUCommZDelta
 */
// ctor.
ClPDUCommZDelta::ClPDUCommZDelta() 
{
	m_iSessionId = 0;
    m_iCompressionType = 0;
	m_iGapBetweenIdentifiers = -1;
	Reset(NULL);

	ClBlockSync oBlock(m_oMutex);
	s_iInstCounter++ ;
	m_iInstNum = s_iInstCounter ;
}

// dtor.
ClPDUCommZDelta::~ClPDUCommZDelta() 
{
}

void ClPDUCommZDelta::Reset(ClPDU* ai_oPDU) 
{
	/* Bug 0113962
	Se guarda el tamaño de la pdu antes de descomprimir el zdelta
	*/
	if( ai_oPDU != NULL && m_iSize > ZDELTA_HEADER_OFFSET )
	{
		ai_oPDU->SetDeltaSize( m_iSize ) ;
	}

    // bugid: 0108865.
    // el tamaño inicial es por defecto el tamaño de la cabecera. Si ponemos 0, tenemos
    // problemas cuando llega un trozo menor ZDELTA_HEADER_OFFSET, pues no sabemos calcular
    // el tamaño del siguiente trozo.
	m_iSize = ZDELTA_HEADER_OFFSET;
	m_iIdentifier = 0;
    m_iSessionId = 0;
    m_iCompressionType = 0;
	m_bDoDelta = M4_TRUE;
}

m4return_t ClPDUCommZDelta::ComposeOutput(m4uint32_t ai_iSesssion, m4pchar_t aio_pcInput, m4uint32_t ai_iInputSize, m4uint32_t ai_iIdentifier, m4pchar_t& ao_pcCompressed, m4uint32_t& ao_iSize) 
{
	m4uint32_t iBufferLen = ai_iInputSize + ZDELTA_HEADER_OFFSET;
	m4pchar_t pcBuffer = new m4char_t[iBufferLen];

    m4uint32_t currentPos = 0;

    // 2 bytes para el tipo.
	*(m4int16_t*)(&pcBuffer[currentPos]) = htons(M4_PDU_ZDELTA_ID);
    currentPos+=2;

    // 6 bytes de relleno para compatibilidad.
    *(m4int16_t*)(&pcBuffer[currentPos]) = 0;
    currentPos+=2;
    *(m4int32_t*)(&pcBuffer[currentPos]) = 0;
    currentPos+=4;

	// 4 bytes para la longitud.
    *(m4int32_t*)(&pcBuffer[currentPos]) = htonl(iBufferLen);
    currentPos+=4;

	// 4 bytes para el sessionid.
	*(m4int32_t*)(&pcBuffer[currentPos]) = htonl(ai_iSesssion);
    currentPos+=4;

	// 4 bytes para el identificador.
	*(m4int32_t*)(&pcBuffer[currentPos]) = htonl(ai_iIdentifier);
    currentPos+=4;

	memcpy(pcBuffer + ZDELTA_HEADER_OFFSET,aio_pcInput,ai_iInputSize);

	// prepares output
	ao_pcCompressed = pcBuffer;
	ao_iSize = iBufferLen;

	return M4_SUCCESS;
}


m4bool_t ClPDUCommZDelta::CheckForFiles(ClPDU* ai_oPDU) 
{	
	// scans for FileUnits
	m4bool_t bThereIsAFile = M4_FALSE;

	M4DataStorage *oDS = ((ClPDUWithDataForZDelta*)ai_oPDU)->GetDataStorageWrapper();
	if (oDS == NULL) 
    {
		return bThereIsAFile;
	}

	ClDataUnitQueue *poQueue = oDS->GetDataUnitInterfaceQueue();
	if (poQueue == NULL) 
    {
		return bThereIsAFile;
	}

	// gets the iterator
	ClDataUnitInterface *poDU = NULL;
	ClDataUnitQueue::iterator oIt = poQueue->begin();
	while (oIt != poQueue->end()) 
    {
		if (poQueue->get(oIt,poDU) != M4_SUCCESS) 
        {
			return bThereIsAFile;
			break;
		}

		if (poDU->GetType() == M4_FILE_UNIT) 
        {
			bThereIsAFile = M4_TRUE;
			break;
		}

		++oIt;
	}

	return bThereIsAFile;
}

// the Compression function
m4return_t ClPDUCommZDelta::DoCompress(ClPDU* ai_oPDU, m4pchar_t& ao_pcBuffer, m4uint32_t& ao_uiLen) 
{
	m4uint64_t	llKey = 0 ;
	ao_pcBuffer = NULL;
	ao_uiLen = 0;

    // checks for compression type.
    m_iCompressionType = ai_oPDU->GetCompressionType();
    if (M4_COMPRESSION_TYPE_ZDELTA != m_iCompressionType)
    {
        return M4_ERROR;
    }

	// checks the credential
    m_iSessionId = ai_oPDU->GetObjectId();
    if (0 == m_iSessionId)
    {
        return M4_ERROR;
    }

	// PDUs with FileDataUnits are not supported.
    if (CheckForFiles(ai_oPDU)) 
    {
		return M4_ERROR;
	}

	// checks the reference for zdelta
    M4ZDeltaManager* poZD = M4ZDeltaManager::GetInstance();
    if (NULL == poZD)
    {
        return M4_ERROR;
    }

    // starts the zdelta proccess.
    // trace variables.
    m4pcchar_t pcOperationId = "COMPRESS";
    m4uint32_t uiInitTickCount = ClPrecisionTicker::GetTimeTick();
    m4uint32_t uiEndTickCount = 0;
    m4uint32_t uiSourceSize = 0;
    m4uint32_t uiReferenceSize = 0;
    m4uint32_t uiTargetSize = 0;

	// ok, lets to generate the output buffer. 
	m4pchar_t pcBuffer = NULL;
	m4uint32_t iOffSet = 0;
	m4uint32_t iOutputIdentifier = 0;
	m4uint32_t iInputIdentifier = 0;

	m4bool_t bErasePatterns = M4_FALSE;

	// moves the pdu to the buffer
	m4bool_t bFirstBlock = M4_TRUE;
	for (;;) 
    {
		m4uint32_t uiLen = 0;
		m4pchar_t bOut = NULL;
		m4return_t iRet = ai_oPDU->GetNextBlock(0, bOut, uiLen);
		if (iRet != M4_SUCCESS) 
        {
			break;
		}

        // creates the new buffer.
        if (NULL == pcBuffer)
        {
            m4uint32_t uiPDULen = ai_oPDU->GetSize();
            pcBuffer = new m4char_t[uiPDULen];
            iOffSet = 0;
            if (NULL == pcBuffer)
            {
				M4_SRV_ERROR(M4_SRV_STARTUPOBL_ERROR, "%s", "Error compresing ZDelta PDU. Not enough memory.");
                return M4_ERROR;
            }
        }

		// updates the output buffer
		memcpy(pcBuffer+iOffSet,bOut,uiLen);
		iOffSet += uiLen;
	}

	// First operation is compress so this is client side.
	if ( m_iGapBetweenIdentifiers == -1 )
	{
		m_iGapBetweenIdentifiers = 0;
	}

	llKey = MakeKey( m_iSessionId, m_iInstNum, (m_iGapBetweenIdentifiers == 0) ) ;

    // retrieves zdelta reference.
	M4ZDeltaElementMapIt_t oOutputIt = poZD->find_out( llKey );
	Bytef *bDelta = NULL;
	uLongf iDSize = 0;

	
	M4ZDeltaElementMapIt_t oInputIt = poZD->find_in ( llKey );


	// bg 109664. If input and output identifiers don't match we have to restart zdelta patterns.
	if (oOutputIt != poZD->end_out())
    {
		iOutputIdentifier = (*oOutputIt).second.identifier;

		if ( oInputIt != poZD->end_in() )
		{
			iInputIdentifier = (*oInputIt).second.identifier;
		}
		
		// An error has happened so we restart zdelta output identifier and pattern.
		if ( iInputIdentifier != (iOutputIdentifier + m_iGapBetweenIdentifiers) || 
			 ((*oOutputIt).second).bJustCanceledRequest == M4_TRUE ||
			 ai_oPDU->GetIdPDU() == M4_ID_PDU_NEWCANCEL )
		{
			delete [] ((*oOutputIt).second).buffer;
			poZD->erase_out( llKey );
			oOutputIt = poZD->end_out();
			iOutputIdentifier = 0;
		}
	}
    
	if (oOutputIt != poZD->end_out()) 
    {
        // trace
        uiReferenceSize = (*oOutputIt).second.size;
		iOutputIdentifier = (*oOutputIt).second.identifier;
		
		// ok, we have a reference. Lets compress
		int res = zd_compress1 ((const Bytef *)(*oOutputIt).second.buffer, (*oOutputIt).second.size,
			(const Bytef *)pcBuffer, iOffSet, &bDelta, &iDSize);

		if (res != ZD_OK) 
        {
			M4_SRV_ERROR(M4_SRV_STARTUPOBL_ERROR, "%s", "Error compresing ZDelta PDU.");
			return M4_ERROR;
		}

		/* Bug 0113962
		Se guarda el tamaño de la pdu después de comprimir el zdelta + la cabecera
		*/
		if( ai_oPDU != NULL )
		{
			ai_oPDU->SetDeltaSize( iDSize + ZDELTA_HEADER_OFFSET ) ;
		}

		// removes the old reference
		delete [] ((*oOutputIt).second).buffer;
		poZD->erase_out( llKey );
	}

	// bg 118467. Comprobamos si llego una PDU de desconexion. Solo si server.
	if( m_iGapBetweenIdentifiers == 1 )
	{
		if( oInputIt != poZD->end_in() )
		{
			bErasePatterns = (*oInputIt).second.bJustDisconnectedSession;
		}
	}

	if( bErasePatterns == M4_FALSE )
	{
		// inserts the new reference
		M4ZDeltaElement oElm;
		oElm.buffer = pcBuffer;
		oElm.size = iOffSet;
		oElm.identifier = iOutputIdentifier + 1;
		oElm.bJustDisconnectedSession = M4_FALSE ;

		// Bg 125975. Si enviamos una pdu de cancelación, utilizamos el patrón de salida
		// para avisar que en la siguiente compresión deben resetearse los patrones.
		// De este modo forzamos el reseteo en el servidor, quien nos ha enviado una
		// pdu de ACK y una de ERROR y se ha quedado la segunda, que no ha llegado al
		// cliente, como patrón de compresión.
		if(ai_oPDU->GetIdPDU() == M4_ID_PDU_NEWCANCEL)
		{
			oElm.bJustCanceledRequest = M4_TRUE;
		}
		else
		{
			oElm.bJustCanceledRequest = M4_FALSE;
		}

		poZD->insert_out( llKey, oElm);
	}

	// generates the output buffer
	if (bDelta == NULL) 
    {
        ComposeOutput(m_iSessionId, pcBuffer, iOffSet, iOutputIdentifier, ao_pcBuffer, ao_uiLen);
	}
	else 
    {
        ComposeOutput(m_iSessionId, m4pchar_t(bDelta), iDSize, iOutputIdentifier, ao_pcBuffer, ao_uiLen);
		// bDelta was allocated with malloc in zd_compress1 function.
        free(bDelta);

	}

    // trace.
    uiEndTickCount = ClPrecisionTicker::GetTimeTick();
    uiSourceSize = ai_oPDU->GetSize();
    uiTargetSize = ao_uiLen;
    poZD->trace(uiInitTickCount, llKey, iOutputIdentifier, pcOperationId, uiSourceSize, uiReferenceSize, uiTargetSize, uiEndTickCount);

	// bg 118467. Si llego una PDU de desconexión borramos los patrones zDelta de la sesion.
	if( bErasePatterns == M4_TRUE )
	{
		poZD->erase( llKey ) ;
	}

	return M4_SUCCESS;
}

m4void_t ClPDUCommZDelta::GetHeader(m4pchar_t ai_pcBuffer, m4uint16_t& ao_iType, m4uint32_t& ao_iSize, m4uint32_t& ao_iIdentifier, m4uint32_t& ao_iSession) 
{
    // init output values.
    ao_iType = 0;
	ao_iSize = 0;
	ao_iIdentifier = 0;
	ao_iSession = 0;

    int currentPos = 0;
	// 2 bytes para el tipo.
    ao_iType = ntohs(*((m4uint16_t*) &ai_pcBuffer[currentPos]));
    currentPos += 2;
	ao_iSize = 0;
	ao_iSession = 0;

	if (ao_iType != M4_PDU_ZDELTA_ID) 
    {
		return;
	}
    m_iCompressionType = M4_COMPRESSION_TYPE_ZDELTA;

    // 6 bytes de relleno para compatibilidad con antiguas PDUs.
    currentPos += 6;

	// 4 bytes para el tamaño pdu.
    ao_iSize    = ntohl(*((m4uint32_t*) &ai_pcBuffer[currentPos]));
    currentPos += 4;

	// 4 bytes para el sessionid.
    ao_iSession = ntohl(*((m4uint32_t*) &ai_pcBuffer[currentPos]));
    currentPos += 4;
    
	// 4 bytes para el identificador.
    ao_iIdentifier = ntohl(*((m4uint32_t*) &ai_pcBuffer[currentPos]));
    currentPos += 4;
    
    //m4Trace(cout << "ClPDUCommZDelta::GetHeader(): id:" << ao_iType << ", size: " << ao_iSize << ", sid: " << ao_iSession << endl);
}

m4return_t ClPDUCommZDelta::AddChunk(m4pchar_t ai_pcChunk, m4uint32_t ai_iChunkSize, m4bool_t ai_bUpdtDoDelta, m4bool_t& ao_bFinish) 
{
    if (ai_iChunkSize < ZDELTA_HEADER_OFFSET) 
    {
		return M4_SUCCESS;
	}

	// checks for the first chunck
	if (m_iSize == ZDELTA_HEADER_OFFSET)
    {
		m4uint16_t iType = 0;
		m4uint32_t iSize = 0, iSession = 0, iIdentifier = 0;
		GetHeader(ai_pcChunk, iType, iSize, iIdentifier, iSession);

		if (iType != M4_PDU_ZDELTA_ID) 
        {
			m_bDoDelta = M4_FALSE;
			return M4_SUCCESS;
		}
		else
		{
			// bg 139141
			if(ai_bUpdtDoDelta == M4_TRUE)
			{
				// bg 109664
				m_bDoDelta = M4_TRUE;
			}
		}

		// initialize members
		m_iSize = iSize;
		m_iIdentifier = iIdentifier;
		m_iSessionId = iSession;
	}

	if (ai_iChunkSize >= m_iSize)         
    {
		ao_bFinish = M4_TRUE;
	}

	return M4_SUCCESS;
}

m4return_t ClPDUCommZDelta::DoDeCompress(m4pchar_t& aio_pcBuffer, m4uint32_t& ao_uiLen) 
{
    m4pchar_t pcInputBuffer = aio_pcBuffer;
	m4uint64_t	llKey = 0 ;

    if (NULL == pcInputBuffer)
    {
        return M4_ERROR;
    }

	aio_pcBuffer += ZDELTA_HEADER_OFFSET;
	ao_uiLen = m_iSize - ZDELTA_HEADER_OFFSET;

    // starts the zdelta proccess.
    // trace variables.
    m4pcchar_t pcOperationId = "DECOMPRESS";
    m4uint32_t uiIdentifier = 0;
    m4uint32_t uiInitTickCount = ClPrecisionTicker::GetTimeTick();
    m4uint32_t uiEndTickCount = 0;
    m4uint32_t uiSourceSize = ao_uiLen;
    m4uint32_t uiReferenceSize = 0;
    m4uint32_t uiTargetSize = 0;
	m4uint32_t uiOutputIdentifier = 0;

	// gets the old reference
    M4ZDeltaManager* poZD = M4ZDeltaManager::GetInstance();
    if (NULL == poZD)
    {
        return M4_ERROR;
    }

	// First operation is decompress so this is server side.
	if ( m_iGapBetweenIdentifiers == -1 )
	{
		m_iGapBetweenIdentifiers = 1;
	}

	llKey = MakeKey( m_iSessionId, m_iInstNum, (m_iGapBetweenIdentifiers == 0) ) ;

	M4ZDeltaElementMapIt_t oIt = poZD->find_in( llKey );

	Bytef *bOriginal = NULL;
	uLongf iOriginalSize = 0;
	
	// bg 109664. An error has happened so we restart zdelta input identifier and pattern.
	if ( m_iIdentifier == 0 )
	{
		if ( oIt != poZD->end_in() )
		{			
			free ((*oIt).second.buffer);
			poZD->erase_in( llKey );
			oIt = poZD->end_in();
		}
	}
	
	/* Bug 0111554
	Si lo que viene no se comprimió con el patrón que tenemos no se puede descomprimir
	*/
	if (oIt != poZD->end_in())
    {
		uiIdentifier = (*oIt).second.identifier;
	}
	else
	{
		uiIdentifier = 0;
	}

	if( uiIdentifier != m_iIdentifier )
	{
		M4_SRV_ERROR( M4_SRV_DECOMPRESS_PDU_BAD_IDENTIFIER, "Error descomprimiendo datos.\n \nNo se han podido descomprimir los datos correspondientes al paquete %0:s de la sesión %2:s porque no se corresponde con el paquete actual %1:s.\nDebe consultar con el administrador.", m_iIdentifier << uiIdentifier << m_iSessionId );
        return M4_ERROR;
	}

	if (oIt != poZD->end_in())
    {
        // traces.
        uiReferenceSize = (*oIt).second.size;

		// generates the original data
        m4pchar_t pcReference = (*oIt).second.buffer;
		int res = zd_uncompress1((const Bytef*)pcReference, (*oIt).second.size,
					&bOriginal, &iOriginalSize,(const Bytef *)aio_pcBuffer, ao_uiLen);
		if (res != ZD_OK) 
        {
			return M4_ERROR;
		}

		// replaces the output
		aio_pcBuffer = (m4pchar_t) bOriginal;
		ao_uiLen = iOriginalSize;

		// deletes the reference
		free(pcReference);
		poZD->erase_in( llKey );
	}
	else 
    {
		// keep the buffer as it is
        // Bytef structs are also allocated in zd_uncompress1 function with malloc,
        // so we use this method and not de c++ new operator.
		iOriginalSize = ao_uiLen;
		bOriginal = (Bytef*)malloc(sizeof(Bytef)*iOriginalSize);
		memcpy(bOriginal, aio_pcBuffer, ao_uiLen);
	}

	// inserts the new reference
	M4ZDeltaElement oElm;
	oElm.buffer = (m4pchar_t)bOriginal;
	oElm.size = iOriginalSize;
	oElm.identifier = uiIdentifier + 1;
	oElm.bJustDisconnectedSession = M4_FALSE ;
	oElm.bJustCanceledRequest = M4_FALSE;
	poZD->insert_in( llKey, oElm);

    // trace.
    uiEndTickCount = ClPrecisionTicker::GetTimeTick();
    uiTargetSize = oElm.size;
    poZD->trace(uiInitTickCount, llKey, uiIdentifier, pcOperationId, uiSourceSize, uiReferenceSize, uiTargetSize, uiEndTickCount);

	return M4_SUCCESS;
}

m4uint64_t ClPDUCommZDelta::MakeKey( m4uint32_t ai_iSesssion, m4uint32_t ai_iPreKey, m4bool_t ai_bClientSide )
{
	m4uint64_t	llRet = 0 ;
	m4int32_t*	pInt = (m4int32_t*)&llRet ;

	*pInt = ai_iSesssion ;

	if( ai_bClientSide == M4_TRUE )
	{
		*(++pInt) = (m4int32_t)ai_iPreKey ;
	}


	return llRet ;
}

m4return_t ClPDUCommZDelta::ExtractKey(m4uint64_t ai_iKey, m4uint32_t& ao_iSesssion, m4uint32_t& ao_iPreKey )
{
	ao_iSesssion = *((m4int32_t*)&ai_iKey) ;
	ao_iPreKey = *(((m4int32_t*)&ai_iKey) + 1) ;


	return M4_SUCCESS ;
}