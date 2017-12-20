

#include <srvping.hpp>



ClServers::ClServers	() 
	{
		m_oServersMap.clear () ;

		m_iIterator = m_oServersMap.begin () ;
	}

ClServers::~ClServers	() 
	{
		m4pchar_t szServer ;

		for ( m_iIterator = m_oServersMap.begin() ; m_iIterator != m_oServersMap.end () ; m_iIterator ++ )
		{
			szServer = (*m_iIterator).first ;

			delete szServer ;
		}

		m_oServersMap.clear () ;
	}




m4uint32_t ClServers::nServers ( ) 
{
	m4uint32_t iServers ;

	iServers = m_oServersMap.size() ;

	return iServers ;
}

m4return_t ClServers::GetNextServer ( m4pchar_t & ao_szServerHost , m4uint32_t & ao_iPort ) 
{
	
	m_iIterator ++ ;

	if ( M4_ERROR == CheckIterator () )
	{
		return M4_ERROR ;
	}

	ao_szServerHost = (*m_iIterator).first ;

	ao_iPort = (*m_iIterator).second ;

	return M4_SUCCESS ;
}

m4return_t ClServers::GetFirstServer ( m4pchar_t & ao_szServerHost , m4uint32_t & ao_iPort ) 
{
	m_iIterator = m_oServersMap.begin () ;


	if ( M4_ERROR == CheckIterator () )
	{
		return M4_ERROR ;
	}

	ao_szServerHost = (*m_iIterator).first ;

	ao_iPort = (*m_iIterator).second ;

	return M4_SUCCESS ;
}


m4return_t ClServers::AddServer ( const m4pchar_t ai_szServerHost , const m4uint32_t ai_iPort ) 
{
	m4pchar_t szServer = new m4char_t [ strlen ( ai_szServerHost) +1  ] ;


	strcpy ( szServer , ai_szServerHost ) ;

	m_oServersMap [szServer] = ai_iPort ;

	m_iIterator = m_oServersMap.end () ;

	return M4_SUCCESS ;
}


m4return_t ClServers::CheckIterator() 
{

	return ( m_iIterator == m_oServersMap.end ( ) ? M4_ERROR : M4_SUCCESS ) ;
}