//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             BufferASN.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CBufferASN class.
//
//
//==============================================================================

#include "asndef.h"
#include "cldates.hpp"
#include "chlog.hpp"
#include "m4presres.hpp"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufferASN::CBufferASN(m4int32_t ai_iReferences)
{
	m4uint32_t i;

	length = 0;
	asn = NULL;

	lengthRef = 0;
	ref = NULL;

	references = ai_iReferences ;

	if( references > 0 )
	{
		ppiLengthRefs = new m4uint32_t[ references ] ;
		ppcRefs = new m4puchar_t[ references ] ;
		ppcCodes = new m4pchar_t[ references ] ;

		memset( ppiLengthRefs, 0, references * sizeof( m4uint32_t ) );
		memset( ppcRefs, NULL, references * sizeof( m4puchar_t ) );
		memset( ppcCodes, NULL, references * sizeof( m4pchar_t ) );
	}
	else
	{
		ppiLengthRefs = NULL ;
		ppcRefs = NULL ;
		ppcCodes = NULL ;
	}

	version = M4CL_MINUS_INFINITE_IN_JULIAN;
	style = 0;
	numT3 = 0;

	for( i = 0; i < MAX_NUM; i++ )
	{
		listT3[i] = NULL;
		listVersionT3[i] = version;
	}
}


CBufferASN::~CBufferASN()
{
	m4uint32_t i;

	if( asn != NULL )
	{
		delete asn;
		asn = NULL;
	}
	length = 0;

	if( ref != NULL )
	{
		delete ref;
		ref = NULL;
	}
	lengthRef = 0;
	
	if( references > 0 )
	{
		for( i = 0; i < references; i++ )
		{
			if( ppcRefs[i] != NULL )
			{
				delete ppcRefs[i];
			}
		}
		delete ppcRefs;
		ppcRefs = NULL;
		delete ppiLengthRefs;
		ppiLengthRefs = NULL;
		delete ppcCodes;
		ppcCodes = NULL;
	}

	for( i = 0; i < numT3;i++ )
	{
		if( listT3[i] != NULL )
		{
			delete listT3[i];
			listT3[i] = NULL;
		}
	}
	numT3 = 0;
}


m4pchar_t CBufferASN::GetIdT3ByIndex (m4int_t index)
{
	if (index < numT3)
	{
		return listT3[index];
	}
	return "";
}


m4date_t CBufferASN::GetVersionT3ByIndex(m4int_t index)
{
	if (index < numT3)
	{
		return (listVersionT3[index]);
	}
	return M4CL_MINUS_INFINITE_IN_JULIAN;
}


m4int_t CBufferASN::Exist(m4pcchar_t idT3)
{
	for (m4int_t i=0; i < numT3; i++)
	{
		if (strcmp(listT3[i], idT3) == 0)
		{
			return i;
		}
	}
	return -1;
}


m4int_t CBufferASN::ExistVersionOfT3(m4pcchar_t idT3)
{
	for (m4int_t i=0; i < numT3; i++)
	{
		if (strcmp(listT3[i], idT3) == 0)
		{
			if (listVersionT3[i] == M4CL_MINUS_INFINITE_IN_JULIAN)
			{
				return i;
			}
			//esta el canal pero ya tiene valor
			return -1;
		}
	}
	return -1;
}


m4int32_t CBufferASN::GetNumInclude( void ) const
{	
	return m_oVcIdInclude.size();		
}


string CBufferASN::GetIdIncludeByPos( m4int32_t ai_iPos ) const
{	
	if (ai_iPos < 0 || ai_iPos >= m_oVcIdInclude.size())
	{
		return "";
	}
	return m_oVcIdInclude[ai_iPos];
}


m4date_t CBufferASN::GetVersionIncludeByPos( m4int32_t ai_iPos )
{	
	if (ai_iPos < 0 || ai_iPos >= m_oVcVersionInclude.size())
	{
		return M4CL_MINUS_INFINITE_IN_JULIAN;
	}
	return m_oVcVersionInclude[ai_iPos];
}


m4return_t CBufferASN::AddInfoIncludeInBufferASN( const string & ai_sIdInclude, m4date_t ai_dVersionInclude )
{
	m_oVcIdInclude.push_back(ai_sIdInclude);
	m_oVcVersionInclude.push_back(ai_dVersionInclude);
	return M4_SUCCESS;
}


m4return_t CBufferASN::CheckInfiniteRecursionPush( const string & ai_sIdInclude )
{
	if (ai_sIdInclude.size() <= 0)
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	for (itSetIdInclude it = m_oSetIdInclude.begin(); it != m_oSetIdInclude.end(); it++)
	{
		if ((*it).compare(ai_sIdInclude) == 0)
		{
			//El id estaba repetido!. Volcamos un error funcional indicando que hay recursion.
			string sShowError("");
			m4bool_t bFirstOcurrenceFound = M4_FALSE;
			for (itSetIdInclude itShow = m_oSetIdInclude.begin(); itShow != m_oSetIdInclude.end(); itShow++)
			{				
				if ((*itShow).compare(ai_sIdInclude) == 0)
				{
					bFirstOcurrenceFound = M4_TRUE;
				}
				if (bFirstOcurrenceFound == M4_TRUE)
				{
					sShowError.append((*itShow));
					sShowError.append(" - ");
				}
			}
			sShowError.append(ai_sIdInclude);
			DUMP_CHLOG_ERRORF( M4_CH_PRES_INFINITE_RECURSION, ai_sIdInclude.c_str() << sShowError.c_str() ) ;
			return M4_ERROR;
		}
	}

	//No ha habido error, lo metemos.
	m_oSetIdInclude.push_back( ai_sIdInclude );

	return M4_SUCCESS;
}


m4return_t CBufferASN::CheckInfiniteRecursionPop( void )
{
	if (m_oSetIdInclude.empty() == M4_FALSE)
	{
		m_oSetIdInclude.pop_back();
		return M4_SUCCESS;
	}
	return M4_ERROR;	
}


