//## begin module%3819AFF80346.cm preserve=no
//## end module%3819AFF80346.cm

//## begin module%3819AFF80346.cp preserve=no
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
//## end module%3819AFF80346.cp

//## Module: chaninfo%3819AFF80346; Package body
//## Subsystem: M4ObjEngine::src%3819B13101F2
//## Source file: D:\ULTIMOSERVER\m4objengine\src\chaninfo.cpp

//## begin module%3819AFF80346.additionalIncludes preserve=no
//## end module%3819AFF80346.additionalIncludes

//## begin module%3819AFF80346.includes preserve=yes
#include "m4mdrt.hpp"
#include "channel.hpp"
//## end module%3819AFF80346.includes

// chaninfo
#include <chaninfo.hpp>
// m4types
#include <m4types.hpp>
#include "m4stl.hpp"
// cldefine
#include <cldefine.hpp>
// ObjectDirectory
#include <objdir.hpp>
#include "m4objglb.hpp"

class ClChannelDirectory;

//## begin module%3819AFF80346.declarations preserve=no
//## end module%3819AFF80346.declarations

//## begin module%3819AFF80346.additionalDeclarations preserve=yes
//## end module%3819AFF80346.additionalDeclarations


// Class ClChannelsInfo::ClChanInfo 

//## begin ClChannelsInfo::ClChanInfo::MAX_STR_LEN%381EB7320289.attr preserve=no  public: static m4uint16_t {UAC} M4CL_MAX_T3_ID + 11
const m4uint16_t  ClChannelsInfo::ClChanInfo::MAX_STR_LEN = M4CL_MAX_T3_ID + 11;
//## end ClChannelsInfo::ClChanInfo::MAX_STR_LEN%381EB7320289.attr



ClChannelsInfo::ClChanInfo::ClChanInfo (ClChannel& ai_oChannel)
  //## begin ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.hasinit preserve=no
      : m_iCsType(M4CL_CSTYPE_UNKNOWN), m_pszId(NULL)
  //## end ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.hasinit
  //## begin ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.initialization preserve=yes
  //## end ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.initialization
{
  //## begin ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.body preserve=yes
	ClCompiledMCR* pCompiledMCR = ai_oChannel.GetpCMCR();

	m_iCsType = pCompiledMCR->GetChannelCsType();

	const m4char_t* pszId = pCompiledMCR->GetChannelId();
	if (pszId != NULL)
	{
		m_pszId = new m4char_t [strlen(pszId) + 1];
		if (m_pszId != NULL)
			strcpy(m_pszId, pszId);
	}
  //## end ClChannelsInfo::ClChanInfo::ClChanInfo%941208377.body
}


ClChannelsInfo::ClChanInfo::~ClChanInfo ()
{
  //## begin ClChannelsInfo::ClChanInfo::~ClChanInfo%941208378.body preserve=yes
	delete [] m_pszId;
  //## end ClChannelsInfo::ClChanInfo::~ClChanInfo%941208378.body
}



//## Other Operations (implementation)
const m4char_t* const ClChannelsInfo::ClChanInfo::GetId () const
{
  //## begin ClChannelsInfo::ClChanInfo::GetId%941208379.body preserve=yes
	return m_pszId;
  //## end ClChannelsInfo::ClChanInfo::GetId%941208379.body
}

m4uint8_t ClChannelsInfo::ClChanInfo::GetCsType () const
{
  //## begin ClChannelsInfo::ClChanInfo::GetCsType%941208380.body preserve=yes
	return m_iCsType;
  //## end ClChannelsInfo::ClChanInfo::GetCsType%941208380.body
}

void ClChannelsInfo::ClChanInfo::ConvertToString (m4char_t* aio_psz) const
{
  //## begin ClChannelsInfo::ClChanInfo::ConvertToString%941533947.body preserve=yes
	// Precondicion: Me han tenido que dar un buffer para concatenar la cadena
	M4_ASSERT(aio_psz != NULL);

	strcat(aio_psz, m_pszId);
	strcat(aio_psz, " (");
	strcat(aio_psz, M4ClCsType(m_iCsType));
	strcat(aio_psz, ")");

  //## end ClChannelsInfo::ClChanInfo::ConvertToString%941533947.body
}

// Class ClChannelsInfo 



ClChannelsInfo::ClChannelsInfo (ClChannelDirectory& ai_oChannelDirectory)
  //## begin ClChannelsInfo::ClChannelsInfo%941208381.hasinit preserve=no
      : m_pszFormatedString(NULL)
  //## end ClChannelsInfo::ClChannelsInfo%941208381.hasinit
  //## begin ClChannelsInfo::ClChannelsInfo%941208381.initialization preserve=yes
  //## end ClChannelsInfo::ClChannelsInfo%941208381.initialization
{
  //## begin ClChannelsInfo::ClChannelsInfo%941208381.body preserve=yes
	ClChannel*	pChannel = NULL;
	ClChanInfo*	pChanInfo = NULL;
	m4uint32_t	iCount = ai_oChannelDirectory.Count();

    // No queremos que reserve una pagina (4K),
    // con que tenga esapcio para los canales que vamos a meter tenemos suficiente
    m_vectorOfChanInfo.reserve(iCount);

	for (m4uint32_t i = 0; i < iCount; i++)
	{
		pChannel = ai_oChannelDirectory[i];
		if (pChannel != NULL)
		{
			pChanInfo = new ClChanInfo(*pChannel);
			m_vectorOfChanInfo.push_back(pChanInfo);
		}
	}
  //## end ClChannelsInfo::ClChannelsInfo%941208381.body
}

ClChannelsInfo::ClChannelsInfo (ClCMObjectDirectory& ai_oObjectDirectory)
  //## begin ClChannelsInfo::ClChannelsInfo%943284669.hasinit preserve=no
      : m_pszFormatedString(NULL)
  //## end ClChannelsInfo::ClChannelsInfo%943284669.hasinit
  //## begin ClChannelsInfo::ClChannelsInfo%943284669.initialization preserve=yes
  //## end ClChannelsInfo::ClChannelsInfo%943284669.initialization
{
  //## begin ClChannelsInfo::ClChannelsInfo%943284669.body preserve=yes
	ClChannel*	pChannel = NULL;
	ClChanInfo*	pChanInfo = NULL;
	m4uint32_t	iCount = ai_oObjectDirectory.GetCountByType(ChannelObjectType);

    // No queremos que reserve una pagina (4K),
    // con que tenga esapcio para los canales que vamos a meter tenemos suficiente
    m_vectorOfChanInfo.reserve(iCount);

    m4int16_t iIndex;

    iIndex=ai_oObjectDirectory.GetFirstObjectIndex(ChannelObjectType);

	for (m4uint32_t i = 0; i < iCount; i++)
	{
		pChannel = (ClChannel*)ai_oObjectDirectory.GetObjectAddress(iIndex);
		if (pChannel != NULL)
		{
			pChanInfo = new ClChanInfo(*pChannel);
			m_vectorOfChanInfo.push_back(pChanInfo);

         iIndex=ai_oObjectDirectory.GetNextObjectIndex();
		}
	}
  //## end ClChannelsInfo::ClChannelsInfo%943284669.body
}


ClChannelsInfo::~ClChannelsInfo ()
{
  //## begin ClChannelsInfo::~ClChannelsInfo%941208382.body preserve=yes
	delete [] m_pszFormatedString;

	vectorOfClChanInfo::iterator it;
	for (it = m_vectorOfChanInfo.begin(); it != m_vectorOfChanInfo.end(); it++)
		delete (*it);

	m_vectorOfChanInfo.clear();
  //## end ClChannelsInfo::~ClChannelsInfo%941208382.body
}



//## Other Operations (implementation)
m4uint32_t ClChannelsInfo::GetCount () const
{
  //## begin ClChannelsInfo::GetCount%941208383.body preserve=yes
	return m_vectorOfChanInfo.size();
  //## end ClChannelsInfo::GetCount%941208383.body
}

const ClChannelsInfo::ClChanInfo* const ClChannelsInfo::GetChanInfo (const m4uint32_t ai_iIndex) const
{
  //## begin ClChannelsInfo::GetChanInfo%941208384.body preserve=yes
	// Precondicion: No puedo pedir un elemento que no tengo
	M4_ASSERT(ai_iIndex < GetCount());

	return m_vectorOfChanInfo[ai_iIndex];
  //## end ClChannelsInfo::GetChanInfo%941208384.body
}

const m4char_t* const ClChannelsInfo::ConvertToString ()
{
  //## begin ClChannelsInfo::ConvertToString%941208385.body preserve=yes
	if (m_pszFormatedString == NULL)
	{
		// Calculo el tamaño maximo de la cadena (es aproximado, siempre va a sobrar)
		m_pszFormatedString = new m4char_t [ClChannelsInfo::ClChanInfo::MAX_STR_LEN * GetCount() + 1];

		if (m_pszFormatedString != NULL)
		{
         m_pszFormatedString[0] = '\0';

         m4bool_t bIsFirstTime=M4_TRUE;
			ClChannelsInfo::vectorOfClChanInfo::iterator it;
			for (it = m_vectorOfChanInfo.begin(); it != m_vectorOfChanInfo.end(); it++)
         {
            if (bIsFirstTime==M4_TRUE)
               bIsFirstTime=M4_FALSE;
            else
               strcat(m_pszFormatedString,", ");
				(*it)->ConvertToString(m_pszFormatedString);
         }
		}
	}

	return m_pszFormatedString;
  //## end ClChannelsInfo::ConvertToString%941208385.body
}

//## begin module%3819AFF80346.epilog preserve=yes
//## end module%3819AFF80346.epilog
