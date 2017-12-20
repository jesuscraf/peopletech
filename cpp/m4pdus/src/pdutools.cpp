//## begin module%3468A69F01B8.cm preserve=no
//## end module%3468A69F01B8.cm

//## begin module%3468A69F01B8.cp preserve=no
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
//## end module%3468A69F01B8.cp

//## Module: pdutools%3468A69F01B8; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\pdutools.cpp

//## begin module%3468A69F01B8.additionalIncludes preserve=no
//## end module%3468A69F01B8.additionalIncludes

//## begin module%3468A69F01B8.includes preserve=yes
//#include <stdio.h>
#include <protocol.hpp>
// #include <winsock2.h>
//## end module%3468A69F01B8.includes
// pdutools
#include <pdutools.hpp>
// DataStorages
#include <datastorages.hpp>
//## begin module%3468A69F01B8.declarations preserve=no
//## end module%3468A69F01B8.declarations

//## begin module%3468A69F01B8.additionalDeclarations preserve=yes
//## end module%3468A69F01B8.additionalDeclarations


// Class ClPairLongIntpChar 



ClPairLongIntpChar::ClPairLongIntpChar (m4int32_t ai_lSize, m4pchar_t ai_pChar)
  //## begin ClPairLongIntpChar::ClPairLongIntpChar%881748108.hasinit preserve=no
      : m_lSize(0), m_pChar(NULL)
  //## end ClPairLongIntpChar::ClPairLongIntpChar%881748108.hasinit
  //## begin ClPairLongIntpChar::ClPairLongIntpChar%881748108.initialization preserve=yes
  //## end ClPairLongIntpChar::ClPairLongIntpChar%881748108.initialization
{
  //## begin ClPairLongIntpChar::ClPairLongIntpChar%881748108.body preserve=yes
   m_pChar = ai_pChar;
  m_lSize = ai_lSize;
  //## end ClPairLongIntpChar::ClPairLongIntpChar%881748108.body
}


ClPairLongIntpChar::~ClPairLongIntpChar ()
{
  //## begin ClPairLongIntpChar::~ClPairLongIntpChar%881748109.body preserve=yes
	if (m_pChar!=NULL) {
		delete []m_pChar;
	};
  //## end ClPairLongIntpChar::~ClPairLongIntpChar%881748109.body
}


// Additional Declarations
  //## begin ClPairLongIntpChar%348E7C950238.declarations preserve=yes
  //## end ClPairLongIntpChar%348E7C950238.declarations

// Class ClBlockFormats 




ClBlockFormats::ClBlockFormats ()
  //## begin ClBlockFormats::ClBlockFormats%878806466.hasinit preserve=no
  //## end ClBlockFormats::ClBlockFormats%878806466.hasinit
  //## begin ClBlockFormats::ClBlockFormats%878806466.initialization preserve=yes
  //## end ClBlockFormats::ClBlockFormats%878806466.initialization
{
  //## begin ClBlockFormats::ClBlockFormats%878806466.body preserve=yes
	m_oIterator = m_oFormatQueue.begin();
  //## end ClBlockFormats::ClBlockFormats%878806466.body
}


ClBlockFormats::~ClBlockFormats ()
{
  //## begin ClBlockFormats::~ClBlockFormats%878806467.body preserve=yes

	ClBlockFormat	*pBlockFormat;
	m4return_t	ret;

	if ( ! m_oFormatQueue.empty() )
	{
		InitializeIteration();
		for ( ret = (m4return_t)m_oFormatQueue.begin( pBlockFormat )
				; ret != M4_ERROR
				; ret = m_oFormatQueue.get_next( m_oIterator, pBlockFormat ) )
		{
			m_oIterator ++;
			if ( pBlockFormat )
			{
				delete pBlockFormat;
				pBlockFormat = NULL;
			}
		}
		m_oFormatQueue.clear();
	}
  //## end ClBlockFormats::~ClBlockFormats%878806467.body
}



//## Other Operations (implementation)
void ClBlockFormats::AddFormat (m4uint32_t ai_ulSizef)
{
  //## begin ClBlockFormats::AddFormat%878723092.body preserve=yes
	ClBlockFormat	*pFormat;

 	pFormat = new ClBlockFormat( ai_ulSizef );
	m_oFormatQueue.push_back( pFormat );

  //## end ClBlockFormats::AddFormat%878723092.body
}

m4bool_t ClBlockFormats::IsEmpty ()
{
  //## begin ClBlockFormats::IsEmpty%878806474.body preserve=yes
	if ( m_oFormatQueue.empty() )
	{
		return M4_TRUE;
	}
	
	return M4_FALSE;
  //## end ClBlockFormats::IsEmpty%878806474.body
}

void ClBlockFormats::Clear ()
{
  //## begin ClBlockFormats::Clear%878848732.body preserve=yes
	if(IsEmpty())
		return;

	ClFormatQueue::iterator	iter;

	ClBlockFormat	*pBlockFormat;

	m4return_t	ret;

	iter = m_oFormatQueue.begin( pBlockFormat );
	ret = m_oFormatQueue.get( iter++, pBlockFormat );


	while( ret != M4_ERROR)
	{
		if ( pBlockFormat != NULL )
		{
			delete pBlockFormat;
		}

		ret = m_oFormatQueue.get( iter++, pBlockFormat );
	}
	m_oFormatQueue.clear();
	m_oIterator = m_oFormatQueue.begin();
  //## end ClBlockFormats::Clear%878848732.body
}

m4return_t ClBlockFormats::GetNext (m4uint32_t &ao_ulSizef)
{
  //## begin ClBlockFormats::GetNext%878893911.body preserve=yes
	ClBlockFormat *pFormat = NULL;
	m4uint32_t	ulSize = 0;
	m4bool_t	bEncrypted = M4_FALSE;

	if ( m_oFormatQueue.get_next( m_oIterator, pFormat ) == M4_ERROR )
	{
		return M4_ERROR;
	}
	ao_ulSizef = pFormat -> m_ulSize;
	bEncrypted = pFormat -> m_bEncrypted;

	if ( bEncrypted )
	{
		ao_ulSizef = ( ao_ulSizef | 0x80000000 ) ;
	}

	m_oIterator ++;
	return M4_SUCCESS;
  //## end ClBlockFormats::GetNext%878893911.body
}

m4return_t ClBlockFormats::GetNext (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted)
{
  //## begin ClBlockFormats::GetNext%878893912.body preserve=yes
	ClBlockFormat *pFormat = NULL;


	if ( m_oFormatQueue.get_next( m_oIterator, pFormat ) == M4_ERROR )
	{
		ao_ulSize = 0;
		ao_bEncrypted = M4_FALSE;
		return M4_ERROR;
	}

	ao_ulSize = pFormat -> m_ulSize;
	ao_bEncrypted = pFormat -> m_bEncrypted;

	m_oIterator ++;

	return M4_SUCCESS;
  //## end ClBlockFormats::GetNext%878893912.body
}

void ClBlockFormats::InitializeIteration ()
{
  //## begin ClBlockFormats::InitializeIteration%878893913.body preserve=yes
	m_oIterator = m_oFormatQueue.begin();
  //## end ClBlockFormats::InitializeIteration%878893913.body
}

m4return_t ClBlockFormats::GetTop (m4uint32_t &ao_ulSizef)
{
  //## begin ClBlockFormats::GetTop%878893916.body preserve=yes
	ClBlockFormat *pFormat = NULL;
	m4uint32_t	ulSize = 0;
	m4bool_t	bEncrypted = M4_FALSE;

	if ( m_oFormatQueue.begin( pFormat ) == M4_ERROR )
	{
		ao_ulSizef = 0;
		return M4_ERROR;
	}

	ao_ulSizef = pFormat -> m_ulSize;
	bEncrypted = pFormat -> m_bEncrypted;
	

	if ( bEncrypted )
	{
		ao_ulSizef = ( ao_ulSizef | 0x80000000 ) ;
	}

	return M4_SUCCESS;
  //## end ClBlockFormats::GetTop%878893916.body
}

m4return_t ClBlockFormats::GetTop (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted)
{
  //## begin ClBlockFormats::GetTop%878893914.body preserve=yes
	ClBlockFormat *pFormat = NULL;

	if ( m_oFormatQueue.begin( pFormat ) == M4_ERROR )
	{
		ao_ulSize = 0;
		ao_bEncrypted = M4_FALSE;
		return M4_ERROR;
	}

	ao_ulSize = pFormat -> m_ulSize;
	ao_bEncrypted = pFormat -> m_bEncrypted;
	
	return M4_SUCCESS;
  //## end ClBlockFormats::GetTop%878893914.body
}

m4return_t ClBlockFormats::QuitTop ()
{
  //## begin ClBlockFormats::QuitTop%878893915.body preserve=yes
/////////// TEMPORAL
	return M4_ERROR;
  //## end ClBlockFormats::QuitTop%878893915.body
}

void ClBlockFormats::AddFormat (m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted)
{
  //## begin ClBlockFormats::AddFormat%878893919.body preserve=yes
	ClBlockFormat	*pFormat;

	pFormat = new ClBlockFormat( ai_ulSize, ai_bEncrypted );
	m_oFormatQueue.push_back( pFormat );

  //## end ClBlockFormats::AddFormat%878893919.body
}

m4return_t ClBlockFormats::GetNextNInc (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted)
{
  //## begin ClBlockFormats::GetNextNInc%879243180.body preserve=yes
	ClBlockFormat *pFormat = NULL;

	if ( m_oFormatQueue.get( m_oIterator, pFormat ) == M4_ERROR )
	{
		ao_ulSize=0;
		return M4_ERROR;
	}

	ao_ulSize = pFormat -> m_ulSize;
	ao_bEncrypted = pFormat -> m_bEncrypted;

	return M4_SUCCESS;

  //## end ClBlockFormats::GetNextNInc%879243180.body
}

m4return_t ClBlockFormats::GetNextNInc (m4uint32_t &ao_ulSizef)
{
  //## begin ClBlockFormats::GetNextNInc%879243181.body preserve=yes
	ClBlockFormat *pFormat = NULL;
	m4uint32_t	ulSize = 0;
	m4bool_t	bEncrypted = M4_FALSE;

	if ( m_oFormatQueue.get( m_oIterator, pFormat ) == M4_ERROR )
	{
		return M4_ERROR;
	}

	ao_ulSizef = pFormat -> m_ulSize;
	bEncrypted = pFormat -> m_bEncrypted;

	if ( bEncrypted )
	{
		ao_ulSizef = ( ao_ulSizef | 0x80000000 ) ;
	}

	return M4_SUCCESS;
  //## end ClBlockFormats::GetNextNInc%879243181.body
}

m4uint32_t ClBlockFormats::GetFormatsSize ()
{
  //## begin ClBlockFormats::GetFormatsSize%879502156.body preserve=yes
	m4uint32_t	ulBlock = m_oFormatQueue.size();
	return ( ulBlock * 4 );
  //## end ClBlockFormats::GetFormatsSize%879502156.body
}

// Additional Declarations
  //## begin ClBlockFormats%345CC9020018.declarations preserve=yes
  //## end ClBlockFormats%345CC9020018.declarations

// Class ClBlockFormat 



ClBlockFormat::ClBlockFormat (m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted)
  //## begin ClBlockFormat::ClBlockFormat%878806464.hasinit preserve=no
      : m_bEncrypted(M4_FALSE)
  //## end ClBlockFormat::ClBlockFormat%878806464.hasinit
  //## begin ClBlockFormat::ClBlockFormat%878806464.initialization preserve=yes
  //## end ClBlockFormat::ClBlockFormat%878806464.initialization
{
  //## begin ClBlockFormat::ClBlockFormat%878806464.body preserve=yes
	m_ulSize = ai_ulSize;
	m_bEncrypted = ai_bEncrypted;
  //## end ClBlockFormat::ClBlockFormat%878806464.body
}

ClBlockFormat::ClBlockFormat (m4uint32_t ai_ulSizef)
  //## begin ClBlockFormat::ClBlockFormat%878893920.hasinit preserve=no
      : m_bEncrypted(M4_FALSE)
  //## end ClBlockFormat::ClBlockFormat%878893920.hasinit
  //## begin ClBlockFormat::ClBlockFormat%878893920.initialization preserve=yes
  //## end ClBlockFormat::ClBlockFormat%878893920.initialization
{
  //## begin ClBlockFormat::ClBlockFormat%878893920.body preserve=yes
	if ( ai_ulSizef & 0x80000000 )
	{
		m_bEncrypted = M4_TRUE;
	}

	m_ulSize = ( ai_ulSizef & 0x7fffffff );
  //## end ClBlockFormat::ClBlockFormat%878893920.body
}


ClBlockFormat::~ClBlockFormat ()
{
  //## begin ClBlockFormat::~ClBlockFormat%878806465.body preserve=yes
  //## end ClBlockFormat::~ClBlockFormat%878806465.body
}


// Additional Declarations
  //## begin ClBlockFormat%345CC84F01B5.declarations preserve=yes
  //## end ClBlockFormat%345CC84F01B5.declarations

// Class ClPairIntpChar 



ClPairIntpChar::ClPairIntpChar (m4int16_t ai_iSize, m4pchar_t ai_pChar)
  //## begin ClPairIntpChar::ClPairIntpChar%875697578.hasinit preserve=no
      : m_iSize(0), m_pChar(NULL)
  //## end ClPairIntpChar::ClPairIntpChar%875697578.hasinit
  //## begin ClPairIntpChar::ClPairIntpChar%875697578.initialization preserve=yes
  //## end ClPairIntpChar::ClPairIntpChar%875697578.initialization
{
  //## begin ClPairIntpChar::ClPairIntpChar%875697578.body preserve=yes
  m_pChar = ai_pChar;
  m_iSize = ai_iSize;
  //## end ClPairIntpChar::ClPairIntpChar%875697578.body
}


ClPairIntpChar::~ClPairIntpChar ()
{
  //## begin ClPairIntpChar::~ClPairIntpChar%880712904.body preserve=yes

 	if (m_pChar!=NULL) {
		delete []m_pChar;
	};
 
  //## end ClPairIntpChar::~ClPairIntpChar%880712904.body
}


// Additional Declarations
  //## begin ClPairIntpChar%34323AC7000C.declarations preserve=yes
  //## end ClPairIntpChar%34323AC7000C.declarations

//## begin module%3468A69F01B8.epilog preserve=yes
//## end module%3468A69F01B8.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClData_kk::AddUShort%890648923.body preserve=no
 
	return M4_ERROR;
	
//## end ClData_kk::AddUShort%890648923.body

//## begin ClData_kk::AddUInt%890648924.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddUInt%890648924.body

//## begin ClData_kk::AddULong%890648925.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddULong%890648925.body

//## begin ClData_kk::AddUWLong%890648926.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddUWLong%890648926.body

//## begin ClData_kk::AddpBuffer%890648927.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddpBuffer%890648927.body

//## begin ClData_kk::AddpBufferC%890648928.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddpBufferC%890648928.body

//## begin ClData_kk::AddBlockFormat%890648929.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddBlockFormat%890648929.body

//## begin ClData_kk::AddPairIntpChar%890648930.body preserve=no
  
	return M4_ERROR;
	
//## end ClData_kk::AddPairIntpChar%890648930.body

#endif
