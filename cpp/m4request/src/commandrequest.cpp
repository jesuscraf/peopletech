//## begin module%35643B2E0219.cm preserve=no
//## end module%35643B2E0219.cm

//## begin module%35643B2E0219.cp preserve=no
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
//## end module%35643B2E0219.cp

//## Module: CommandRequest%35643B2E0219; Subprogram body
//## Subsystem: M4Request::src%379439BF01B7
//## Source file: d:\eduardoma\M4Request\version\src\commandrequest.cpp

//## begin module%35643B2E0219.additionalIncludes preserve=no
//## end module%35643B2E0219.additionalIncludes

//## begin module%35643B2E0219.includes preserve=yes
//## end module%35643B2E0219.includes

// CommandRequest
#include <commandrequest.hpp>
// m4types
#include <m4types.hpp>
// BaseRequest
#include <baserequest.hpp>
// logsys
#include <logsys.hpp>
//## begin module%35643B2E0219.declarations preserve=no
//## end module%35643B2E0219.declarations

//## begin module%35643B2E0219.additionalDeclarations preserve=yes
//## end module%35643B2E0219.additionalDeclarations


// Class ClBinaryBlock 




ClBinaryBlock::ClBinaryBlock ()
  //## begin ClBinaryBlock::ClBinaryBlock%898195370.hasinit preserve=no
      : m_pContent(NULL), m_uiSize(0)
  //## end ClBinaryBlock::ClBinaryBlock%898195370.hasinit
  //## begin ClBinaryBlock::ClBinaryBlock%898195370.initialization preserve=yes
  //## end ClBinaryBlock::ClBinaryBlock%898195370.initialization
{
  //## begin ClBinaryBlock::ClBinaryBlock%898195370.body preserve=yes
  //## end ClBinaryBlock::ClBinaryBlock%898195370.body
}

ClBinaryBlock::ClBinaryBlock (m4pchar_t  ai_pBlock, m4uint32_t ai_uiLength)
  //## begin ClBinaryBlock::ClBinaryBlock%898195371.hasinit preserve=no
      : m_pContent(NULL), m_uiSize(0)
  //## end ClBinaryBlock::ClBinaryBlock%898195371.hasinit
  //## begin ClBinaryBlock::ClBinaryBlock%898195371.initialization preserve=yes
  //## end ClBinaryBlock::ClBinaryBlock%898195371.initialization
{
  //## begin ClBinaryBlock::ClBinaryBlock%898195371.body preserve=yes
	SetContent(ai_pBlock, ai_uiLength);
  //## end ClBinaryBlock::ClBinaryBlock%898195371.body
}


ClBinaryBlock::~ClBinaryBlock ()
{
  //## begin ClBinaryBlock::~ClBinaryBlock%898195372.body preserve=yes
	Clear();
  //## end ClBinaryBlock::~ClBinaryBlock%898195372.body
}



//## Other Operations (implementation)
m4return_t ClBinaryBlock::SetContent (m4pchar_t  ai_pBlock, m4uint32_t ai_uiLength)
{
  //## begin ClBinaryBlock::SetContent%898195375.body preserve=yes
	if (NULL != ai_pBlock)
	{
		m_uiSize = ai_uiLength;
		m_pContent = new m4char_t[m_uiSize];
		memcpy(m_pContent , ai_pBlock, m_uiSize);
	}

	return M4_SUCCESS;
  //## end ClBinaryBlock::SetContent%898195375.body
}

m4return_t ClBinaryBlock::Clear ()
{
  //## begin ClBinaryBlock::Clear%898195373.body preserve=yes
	if (NULL != m_pContent)
		delete []m_pContent;

	m_pContent = NULL;
	m_uiSize = 0;

	return M4_SUCCESS;
  //## end ClBinaryBlock::Clear%898195373.body
}

// Additional Declarations
  //## begin ClBinaryBlock%35895DEB0079.declarations preserve=yes
  //## end ClBinaryBlock%35895DEB0079.declarations

// Class ClCommandRequest 






ClCommandRequest::ClCommandRequest (m4handle_t ai_uiReqId, ClLogSystem *ai_poLog, ClStatistic *ai_poStatistic)
  //## begin ClCommandRequest::ClCommandRequest%894446851.hasinit preserve=no
      : m_uiCommandId(0), m_uiNumberOfArgs(0), m_rInit(M4_ERROR), m_pArglist(NULL)
  //## end ClCommandRequest::ClCommandRequest%894446851.hasinit
  //## begin ClCommandRequest::ClCommandRequest%894446851.initialization preserve=yes
	, ClControlRequest(M4_COMMAND_REQ_TYPE, ai_uiReqId, ai_poStatistic), m_poReqLog(ai_poLog)
  //## end ClCommandRequest::ClCommandRequest%894446851.initialization
{
  //## begin ClCommandRequest::ClCommandRequest%894446851.body preserve=yes
  //## end ClCommandRequest::ClCommandRequest%894446851.body
}


ClCommandRequest::~ClCommandRequest ()
{
  //## begin ClCommandRequest::~ClCommandRequest%894446852.body preserve=yes
	for (m4uint32_t index = 0; index < m_uiNumberOfArgs; index++)
	{
		delete m_pArglist[index];
		m_pArglist[index] = NULL;
	}

	free ( m_pArglist );
	m_pArglist = NULL;
  //## end ClCommandRequest::~ClCommandRequest%894446852.body
}


// Additional Declarations
  //## begin ClCommandRequest%35507CEF031C.declarations preserve=yes
  //## end ClCommandRequest%35507CEF031C.declarations

//## begin module%35643B2E0219.epilog preserve=yes
//## end module%35643B2E0219.epilog
