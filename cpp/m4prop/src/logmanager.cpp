// LogManager.cpp: implementation of the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4prop.h"
#include "logmanager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LOG_BUFFER_SIZE 64000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogManager::CLogManager()
{
	m_pBuffer = NULL;
	m_pBufferActual = NULL;
	m_pBufferFinal = NULL;

}

CLogManager::~CLogManager()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
	}

}

void CLogManager::Init()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
	}
	m_pBuffer = (char*)malloc(LOG_BUFFER_SIZE*sizeof(char));
	m_pBufferActual = m_pBuffer;
	m_pBufferFinal = m_pBuffer + LOG_BUFFER_SIZE;
}

long CLogManager::Write(char * pChar)
{
	// Devolvemos la cantidad de caracteres escritos en el Buffer
	long nWrited = 0;
	if (m_pBuffer)
	{
		if (pChar)
		{
			nWrited = strlen(pChar);
			if (nWrited > 0)
			{
				if (TestBufferSize(nWrited+2))
				{
					memcpy(m_pBufferActual, pChar, nWrited);
					m_pBufferActual+=nWrited;
					*m_pBufferActual = 0x0D;
					*(m_pBufferActual+1) = 0x0A;
					m_pBufferActual+=2;
				}
				else
					nWrited = 0;
			}
		}
	}

	return nWrited;
}

BOOL CLogManager::TestBufferSize(long nSize)
{
	if ((m_pBufferFinal-m_pBufferActual)>nSize)
	{
		return true;
	}
	else
	{
		// Realocamos
		long nActualSize = m_pBufferFinal-m_pBuffer;
		char * pTemp = (char*)malloc((nActualSize+LOG_BUFFER_SIZE)*sizeof(char));
		if (pTemp)
		{
			memcpy(pTemp, m_pBuffer, nActualSize);
			m_pBufferActual = pTemp + (m_pBufferActual-m_pBuffer);
			m_pBufferFinal = pTemp + nActualSize + LOG_BUFFER_SIZE;;
			free(m_pBuffer);
			m_pBuffer = pTemp;
			return true;
		}
	}
	return false;
}

char * CLogManager::GetBuffer()
{
	// Antes de devolverlo le añadimos un /0
	*m_pBufferActual = 0;
	return m_pBuffer;
}
