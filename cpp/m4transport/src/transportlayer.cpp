//## begin module%38D88150038D.cm preserve=no
//## end module%38D88150038D.cm

//## begin module%38D88150038D.cp preserve=no
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
//## end module%38D88150038D.cp

//## Module: TransportLayer%38D88150038D; Package body
//## Subsystem: m4transport::M4CTL::src%38D1FCC40071
//## Source file: D:\Work\v600\m4transport\src\transportlayer.cpp

//## begin module%38D88150038D.additionalIncludes preserve=no
//## end module%38D88150038D.additionalIncludes

//## begin module%38D88150038D.includes preserve=yes
//## end module%38D88150038D.includes

// CommComun
#include <commcomun.hpp>
// TransportLayer
#include <transportlayer.hpp>
// m4thread
#include <m4thread.hpp>
// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
// BlockSynchronization
#include <blocksynchronization.hpp>
//## begin module%38D88150038D.declarations preserve=no
//## end module%38D88150038D.declarations

//## begin module%38D88150038D.additionalDeclarations preserve=yes
#include <m4log.hpp>
#include <tsap.hpp>
#include <m4trace.hpp>
#include <m4crono.hpp>
#include <m4date.hpp>
#include <m4srvres.hpp>
#include <errno.h>
#include <pduserializer.hpp>
#include "m4unicode.hpp"


#ifdef _UNIX
#include <unistd.h>
#else
#include <io.h>
#endif

//## end module%38D88150038D.additionalDeclarations


// Class ClTransportLayer 


//## begin ClTransportLayer::m_bDumpData%3E7720400057.attr preserve=no  protected: static m4bool_t {UA} M4_FALSE
m4bool_t ClTransportLayer::m_bDumpData = M4_FALSE;
//## end ClTransportLayer::m_bDumpData%3E7720400057.attr

//## begin ClTransportLayer::m_pcBaseDumpDir%3E7721B40355.attr preserve=no  protected: static m4pchar_t {UA} NULL
m4pchar_t ClTransportLayer::m_pcBaseDumpDir = NULL;
//## end ClTransportLayer::m_pcBaseDumpDir%3E7721B40355.attr

//## begin ClTransportLayer::m_pcDumpPrefix%3E7721C603B5.attr preserve=no  protected: static m4pchar_t {UA} NULL
m4pchar_t ClTransportLayer::m_pcDumpPrefix = NULL;
//## end ClTransportLayer::m_pcDumpPrefix%3E7721C603B5.attr

//## begin ClTransportLayer::m_uiMaxDumpSize%3E7722980294.attr preserve=no  protected: static m4uint32_t {UA} 0
m4uint32_t ClTransportLayer::m_uiMaxDumpSize = 0;
//## end ClTransportLayer::m_uiMaxDumpSize%3E7722980294.attr




//## begin ClTransportLayer::m_bDoReset%3EE5A6590072.attr preserve=no  protected: static m4bool_t {UA} M4_FALSE
m4bool_t ClTransportLayer::m_bDoReset = M4_FALSE;
//## end ClTransportLayer::m_bDoReset%3EE5A6590072.attr

//## begin ClTransportLayer::m_iMaxFiles%3EE5CA170173.attr preserve=no  protected: static m4int32_t {UA} 0
m4int32_t ClTransportLayer::m_iMaxFiles = 0;
//## end ClTransportLayer::m_iMaxFiles%3EE5CA170173.attr

//## begin ClTransportLayer::m_poMutex%3E7735C40328.role preserve=no  protected: static ClMutex { -> RHAN}
ClMutex *ClTransportLayer::m_poMutex = NULL;
//## end ClTransportLayer::m_poMutex%3E7735C40328.role



ClTransportLayer::ClTransportLayer ()
  //## begin ClTransportLayer::ClTransportLayer%954835557.hasinit preserve=no
      : m_iError(M4_TRANSPORT_OK), m_uiCurrentDumpSize(0), m_uiDumpNumber(0), m_pFile(NULL)
  //## end ClTransportLayer::ClTransportLayer%954835557.hasinit
  //## begin ClTransportLayer::ClTransportLayer%954835557.initialization preserve=yes
  //## end ClTransportLayer::ClTransportLayer%954835557.initialization
{
  //## begin ClTransportLayer::ClTransportLayer%954835557.body preserve=yes
	if (SocketInit() != 0)
	{
		m4Trace(fprintf(stderr, "Error en SocketInit()\n"));
		m_iError = M4_TRANSPORT_SOCKET_INIT;
	}

	// inits mutex
	m_poMutex = new ClMutex(M4_TRUE);
  //## end ClTransportLayer::ClTransportLayer%954835557.body
}


ClTransportLayer::~ClTransportLayer ()
{
  //## begin ClTransportLayer::~ClTransportLayer%1047994134.body preserve=yes

	if (m_poMutex != NULL) {
		delete m_poMutex;
		m_poMutex = NULL;
	}

	// checks internal members
	if (m_pcBaseDumpDir != NULL) {
		delete [] m_pcBaseDumpDir;
		m_pcBaseDumpDir = NULL;
	}

	if (m_pcDumpPrefix != NULL) {
		delete [] m_pcDumpPrefix;
		m_pcDumpPrefix = NULL;
	}

	if (m_pFile != NULL) {
		fclose(m_pFile);
	}
  //## end ClTransportLayer::~ClTransportLayer%1047994134.body
}



//## Other Operations (implementation)
m4return_t ClTransportLayer::EnableDumpData (m4pchar_t ai_pcBasePath, m4uint32_t ai_uiMaxSize, m4pchar_t ai_pcBasePrefix, m4uint32_t ai_uiMaxFiles)
{
  //## begin ClTransportLayer::EnableDumpData%1047994131.body preserve=yes

	ClMutBlock oBlock (m_poMutex);

	// if already enabled just returns
	if (m_bDumpData) {
		return M4_SUCCESS;
	}

	// resets flag
	m_bDoReset = M4_TRUE;
	m_bDumpData = M4_TRUE;

	if (ai_pcBasePath != NULL)
	{
		// Bug 0099587. Check if directory exist and it we have write permissions.
		m4int32_t iErrorCode;
		m4int32_t M4_WRITE_ACCESS = 0;
#ifdef _UNIX
		M4_WRITE_ACCESS = W_OK;
		iErrorCode = access(ai_pcBasePath, M4_WRITE_ACCESS);
#else
		M4_WRITE_ACCESS = 2;
		iErrorCode = _access(ai_pcBasePath, M4_WRITE_ACCESS);
#endif
		
		if (iErrorCode != M4_SUCCESS)
		{
			// disable dump data.
			m_bDoReset = M4_FALSE;
			m_bDumpData = M4_FALSE;
			SETCODEF(M4_SRV_DUMP_DIRECTORY_ERROR, ERRORLOG, "#*s1*#%s#" , ai_pcBasePath);
			return M4_ERROR;
		}

		if (m_pcBaseDumpDir != NULL) {
			delete m_pcBaseDumpDir;
		}
		m_pcBaseDumpDir = new m4char_t[strlen(ai_pcBasePath)+1];
		strcpy(m_pcBaseDumpDir,ai_pcBasePath);
	}

	if (ai_pcBasePrefix != NULL) {
		if (m_pcDumpPrefix != NULL) {
			delete m_pcDumpPrefix;
		}
		m_pcDumpPrefix = new m4char_t[strlen(ai_pcBasePrefix)+1];
		strcpy(m_pcDumpPrefix,ai_pcBasePrefix);
	}

	if (ai_uiMaxSize) {
		m_uiMaxDumpSize = ai_uiMaxSize;
	}

	if (ai_uiMaxFiles >= 0) {
		m_iMaxFiles = ai_uiMaxFiles;
	}

	return M4_SUCCESS;
  //## end ClTransportLayer::EnableDumpData%1047994131.body
}

void ClTransportLayer::DisableDumpData ()
{
  //## begin ClTransportLayer::DisableDumpData%1047994132.body preserve=yes

	// flushes and closes the current file
	ClMutBlock oBlock (m_poMutex);

	// if already disabled just returns
	if (m_bDumpData == M4_FALSE) {
		return;
	}

	// resets flag
	m_bDumpData = M4_FALSE;
  //## end ClTransportLayer::DisableDumpData%1047994132.body
}

void ClTransportLayer::DumpData (m4uint64_t ai_pId, void *ai_pBuffer, m4uint32_t ai_uiLen)
{
  //## begin ClTransportLayer::DumpData%1047994133.body preserve=yes
	// check status
	if (!m_bDumpData || ((m4int32_t)ai_uiLen) <= 0) {
		return;
	}

	// blocks
	ClMutBlock oBlock(m_poMutex);

	// reset status if requested
	if (m_bDoReset) {
		m_bDoReset = M4_FALSE;
		m_uiDumpNumber = 0;
		m_sBaseName = "";
		if (m_pFile != NULL) {
			fclose(m_pFile);
			m_pFile = NULL;
		}
		if (m_iMaxFiles == 0) {
			m_vFiles.clear();
		}
	}

	// checks the current file
	if (m_pFile == NULL) {

		// checks for files
		if (m_iMaxFiles && (m4int32_t)m_vFiles.size() == m_iMaxFiles) {
			// deletes the first object
			vector<string>::iterator it = m_vFiles.begin();
			unlink((*it).c_str());
			m_vFiles.erase(it);
		}

		// generates the name if not already generated
		string sName;
		m4char_t tmpNumber[32] = "";
		
		if (m_sBaseName.size() == 0) {
			sName = m_pcBaseDumpDir;
			sName.append("/");
			sName.append(m_pcDumpPrefix);

			// gets the date
			M4ClTimeStamp oDate;
			oDate.now();
			m4int32_t year,month,day,hour,min,sec;
			oDate.get(year,month,day,hour,min,sec);

			sprintf(tmpNumber,"%04d%02d%02d-%02d%02d%02d",year,month,day,hour,min,sec);
			sName.append("-");
			sName.append(tmpNumber);

			sName.append("-");

			// saves for new files
			m_sBaseName = sName;
		}
		else {
			sName = m_sBaseName;
		}

		sprintf(tmpNumber,"%d",m_uiDumpNumber++);
		sName.append(tmpNumber);
		sName.append(".bin");

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		m_pFile = M4Fopen(sName.c_str(), M4UniWriteBinary, eEncoding);
		if (m_pFile == NULL) {
			SETCODEF (M4_SRV_DUMP_ERROR_OPEN,ERRORLOG,"#*s1*#%s#%s#" , (m4pchar_t)sName.c_str(), strerror(errno));
			return;
		}

		// includes the file in the vector of created files
		if (m_iMaxFiles) {
			m_vFiles.push_back(sName);
		}
	}

	// writes the header
	time_t iTime = 0;
	time(&iTime);
	ClAdvNetHost oConv;
	m4uint32_t tmpInt = oConv.m4htonul(iTime);
	if (fwrite(&tmpInt,sizeof(tmpInt),1,m_pFile) != 1) {
		SETCODEF (M4_SRV_DUMP_WRITE_ERROR,ERRORLOG,"#*s1*#%s#" , strerror(errno));
		return;
	}
	m_uiCurrentDumpSize += sizeof(tmpInt);

	m4uint64_t tmpLong = oConv.m4htonulw(ai_pId);
	if (fwrite(&tmpLong,sizeof(tmpLong),1,m_pFile) != 1) {
		SETCODEF (M4_SRV_DUMP_WRITE_ERROR,ERRORLOG,"#*s1*#%s#" , strerror(errno));
		return;
	}
	m_uiCurrentDumpSize += sizeof(tmpLong);

	tmpInt = oConv.m4htonul(ai_uiLen);
	if (fwrite(&tmpInt,sizeof(tmpInt),1,m_pFile) != 1) {
		SETCODEF (M4_SRV_DUMP_WRITE_ERROR,ERRORLOG,"#*s1*#%s#" , strerror(errno));
		return;
	}
	m_uiCurrentDumpSize += sizeof(tmpInt);

	if (fwrite(ai_pBuffer,ai_uiLen,1,m_pFile) != 1) {
		SETCODEF (M4_SRV_DUMP_WRITE_ERROR,ERRORLOG,"#*s1*#%s#" , strerror(errno));
		return;
	}
	m_uiCurrentDumpSize += ai_uiLen;

	fflush(m_pFile);

	if (m_uiCurrentDumpSize >= m_uiMaxDumpSize) {
		m_uiCurrentDumpSize = 0;
		fclose(m_pFile);
		m_pFile = NULL;
	}

  //## end ClTransportLayer::DumpData%1047994133.body
}

m4bool_t ClTransportLayer::GetDumpStatus (m4pcchar_t &ao_pcBasePath, m4uint32_t &ao_uiMaxSize, m4pcchar_t &ao_pcBasePrefix, m4int32_t &ao_iMaxFiles)
{
  //## begin ClTransportLayer::GetDumpStatus%1049711019.body preserve=yes

	// return the values
	ao_pcBasePath = m_pcBaseDumpDir;
	ao_uiMaxSize = m_uiMaxDumpSize;
	ao_pcBasePrefix = m_pcDumpPrefix;
	ao_iMaxFiles = m_iMaxFiles;
	return m_bDumpData;

  //## end ClTransportLayer::GetDumpStatus%1049711019.body
}

// Additional Declarations
  //## begin ClTransportLayer%38D735EA02F6.declarations preserve=yes
  //## end ClTransportLayer%38D735EA02F6.declarations

//## begin module%38D88150038D.epilog preserve=yes
//## end module%38D88150038D.epilog
