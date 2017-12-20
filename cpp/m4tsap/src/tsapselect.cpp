//## begin module%377768480324.cm preserve=no
//## end module%377768480324.cm

//## begin module%377768480324.cp preserve=no
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
//## end module%377768480324.cp

//## Module: tsapselect%377768480324; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\compon\m4tsap\version\src\tsapselect.cpp

//## begin module%377768480324.additionalIncludes preserve=no
//## end module%377768480324.additionalIncludes

//## begin module%377768480324.includes preserve=yes
#include "tsap.hpp"
#include "selectimpl.hpp"
#include "tsapimplfactory.hpp"
#include "syncro.hpp"
#include "m4trace.hpp"
#include <string.h>





//## end module%377768480324.includes

// m4types
#include <m4types.hpp>


//## begin module%377768480324.declarations preserve=no
//## end module%377768480324.declarations

//## begin module%377768480324.additionalDeclarations preserve=yes
//## end module%377768480324.additionalDeclarations


// Class ClTSAPSelect 



ClTSAPSelect::ClTSAPSelect (m4bool_t ai_bUseMutex)
  //## begin ClTSAPSelect::ClTSAPSelect%289367655.hasinit preserve=no
  //## end ClTSAPSelect::ClTSAPSelect%289367655.hasinit
  //## begin ClTSAPSelect::ClTSAPSelect%289367655.initialization preserve=yes
  //## end ClTSAPSelect::ClTSAPSelect%289367655.initialization
{
  //## begin ClTSAPSelect::ClTSAPSelect%289367655.body preserve=yes
	m_poTSAPSelectImpl = ClTSAP::Factory()->CreateTSAPSelectImpl();

	m_poMutex = NULL;
	if (ai_bUseMutex)
	{
		if ((m_poMutex = new ClMutex()) == NULL)
		{
			m4Trace(fprintf(stderr, "Error in ClTSAPSelect constructor: new ClMutex returns NULL\n"));
		}
		else
		{
			if (m_poMutex->Init() != M4_SUCCESS)
			{
				delete m_poMutex;
				m4Trace(fprintf(stderr, "Error in ClTSAPSelect constructor: m_poMutex->Init() return != M4_SUCCESS\n"));
			}
		}
	}

  //## end ClTSAPSelect::ClTSAPSelect%289367655.body
}


ClTSAPSelect::~ClTSAPSelect ()
{
  //## begin ClTSAPSelect::~ClTSAPSelect%1272558795.body preserve=yes
	if (m_poTSAPSelectImpl != NULL)
		delete m_poTSAPSelectImpl;

	if (m_poMutex != NULL)
		delete m_poMutex;
  //## end ClTSAPSelect::~ClTSAPSelect%1272558795.body
}



//## Other Operations (implementation)
void ClTSAPSelect::Add (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions)
{
  //## begin ClTSAPSelect::Add%-864637872.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return;

	if (ai_poTSAP == NULL)
	{
		m4Trace (fprintf(stderr, "TSAP pasada como argumento en Add es nula\n"));
		return;
	}
	
	if (m_poMutex != NULL)
		m_poMutex->Lock();

	m_poTSAPSelectImpl->Add(ai_poTSAP, ai_bRead, ai_bWrite, ai_bExceptions);
	
	if (m_poMutex!= NULL)
		m_poMutex->Unlock();
  //## end ClTSAPSelect::Add%-864637872.body
}

m4return_t ClTSAPSelect::Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions)
{
  //## begin ClTSAPSelect::Remove%-2076839024.body preserve=yes
	m4return_t	ret;

	if (m_poTSAPSelectImpl == NULL)
		return M4_ERROR;

	if (ai_poTSAP == NULL)
	{
		m4Trace (fprintf(stderr, "TSAP pasada como argumento en Remove es nula\n"));
		return M4_FALSE;
	}
	if (m_poMutex!= NULL)
		m_poMutex->Lock();
	
	ret = m_poTSAPSelectImpl->Remove(ai_poTSAP, ai_bRead, ai_bWrite,ai_bExceptions);
	
	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return ret;
  //## end ClTSAPSelect::Remove%-2076839024.body
}

m4return_t ClTSAPSelect::Remove (ClTSAP* ai_poTSAP)
{
  //## begin ClTSAPSelect::Remove%-2076839024.body preserve=yes
	m4return_t	ret;

	if (m_poTSAPSelectImpl == NULL)
		return M4_ERROR;

	if (ai_poTSAP == NULL)
	{
		m4Trace (fprintf(stderr, "TSAP pasada como argumento en Remove es nula\n"));
		return M4_FALSE;
	}
	if (m_poMutex!= NULL)
		m_poMutex->Lock();
	
	ret = m_poTSAPSelectImpl->Remove(ai_poTSAP);
	
	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return ret;
  //## end ClTSAPSelect::Remove%-2076839024.body
}

int ClTSAPSelect::Select (unsigned long ai_Segundos, unsigned long ai_Microsegundos, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions)
{
  //## begin ClTSAPSelect::Select%-2055469014.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return -1;

  return m_poTSAPSelectImpl->Select(m_poMutex, ai_Segundos, ai_Microsegundos,
		ai_bRead, ai_bWrite, ai_bExceptions);
  //## end ClTSAPSelect::Select%-2055469014.body
}

ClTSAP* ClTSAPSelect::NextRead ()
{
  //## begin ClTSAPSelect::NextRead%1656925455.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return NULL;

	ClTSAP	*poTSAP;

	if (m_poMutex!= NULL)
		m_poMutex->Lock();

	poTSAP = m_poTSAPSelectImpl->NextRead();

	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return poTSAP;
  //## end ClTSAPSelect::NextRead%1656925455.body
}

ClTSAP* ClTSAPSelect::NextWrite ()
{
  //## begin ClTSAPSelect::NextWrite%312378509.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return NULL;

	ClTSAP	*poTSAP;

	if (m_poMutex!= NULL)
		m_poMutex->Lock();

	poTSAP = m_poTSAPSelectImpl->NextWrite();

	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return poTSAP;
  //## end ClTSAPSelect::NextWrite%312378509.body
}

ClTSAP* ClTSAPSelect::NextExceptions ()
{
  //## begin ClTSAPSelect::NextExceptions%-318900140.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return NULL;

	ClTSAP	*poTSAP;

	if (m_poMutex!= NULL)
		m_poMutex->Lock();

	poTSAP = m_poTSAPSelectImpl->NextExceptions();

	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return poTSAP;
  //## end ClTSAPSelect::NextExceptions%-318900140.body
}

m4bool_t ClTSAPSelect::IsInRead (ClTSAP* ai_poTSAP)
{
  //## begin ClTSAPSelect::IsInRead%2121007519.body preserve=yes

	if (m_poTSAPSelectImpl == NULL)
		return M4_FALSE;

	m4bool_t	bRet = IsInMask(ai_poTSAP, IS_IN_READ);

	return bRet;
  //## end ClTSAPSelect::IsInRead%2121007519.body
}

m4bool_t ClTSAPSelect::IsInWrite (ClTSAP* ai_poTSAP)
{
  //## begin ClTSAPSelect::IsInWrite%987484626.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return M4_FALSE;

	m4bool_t	bRet = IsInMask(ai_poTSAP, IS_IN_WRITE);

	return bRet;
  //## end ClTSAPSelect::IsInWrite%987484626.body
}

m4bool_t ClTSAPSelect::IsInExceptions (ClTSAP* ai_poTSAP)
{
  //## begin ClTSAPSelect::IsInExceptions%1391337547.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return M4_FALSE;

	m4bool_t	bRet = IsInMask(ai_poTSAP, IS_IN_EXCEPTIONS);

	return bRet;
  //## end ClTSAPSelect::IsInExceptions%1391337547.body
}

m4bool_t ClTSAPSelect::IsInMask (ClTSAP* ai_poTSAP, m4int8_t ai_iTipoMascara)
{
  //## begin ClTSAPSelect::IsInMask%1431804889.body preserve=yes
	if (m_poTSAPSelectImpl == NULL)
		return M4_FALSE;

	if (m_poMutex!= NULL)
		m_poMutex->Lock();

	m4bool_t	bRet = m_poTSAPSelectImpl->IsInMask(ai_poTSAP, ai_iTipoMascara);

	if (m_poMutex!= NULL)
		m_poMutex->Unlock();

	return bRet;
  //## end ClTSAPSelect::IsInMask%1431804889.body
}

// Additional Declarations
  //## begin ClTSAPSelect%377768440404.declarations preserve=yes
  //## end ClTSAPSelect%377768440404.declarations

//## begin module%377768480324.epilog preserve=yes
//## end module%377768480324.epilog
