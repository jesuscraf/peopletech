
#include <oblutil.hpp>
#include <string.h>

ClOblUtil::ClOblUtil ( m4pchar_t ai_szOblPath )
{
	m_oblFile =NULL ;

	if ( ai_szOblPath )
	{
		m_szOblPath = new m4char_t [ strlen ( ai_szOblPath ) +1 ];

		strcpy ( m_szOblPath , ai_szOblPath  );

		if ( m_oblFile = fopen ( m_szOblPath , "r+") )
			fgetpos ( m_oblFile , &m_pos ) ;
	}

	
	m_err = (m_oblFile== NULL)?M4_TRUE:M4_FALSE ;

}


ClOblUtil::~ClOblUtil ( )
{
	fclose ( m_oblFile ) ;
}

m4bool_t ClOblUtil::Error( )
{
	return m_err ;
}

m4return_t ClOblUtil::PersistOblProp ( m4pchar_t ai_szProperty , m4pchar_t ai_szValue )
{
	m4char_t szLine [1024];
	m4char_t szValue [1024];
	m4pchar_t pValue ;
	fpos_t inicio = 0 ;

	fsetpos ( m_oblFile , &inicio) ;

	if ( M4_ERROR ==GetProperty (  ai_szProperty , szValue , 1024 ) )
	{
		return M4_ERROR ;
	}

	if ( strstr ( szValue , ai_szValue ) )
	{
		return M4_SUCCESS ;
	}

	fsetpos( m_oblFile , &m_pos ) ;

	if (! fgets (szLine , 1024 , m_oblFile ) )
	{
		return M4_ERROR ;
	}

	pValue = strstr ( szLine , "=" ) ;

	sprintf ( pValue , "= %s\n" , ai_szValue ) ;

	fsetpos( m_oblFile , &m_pos ) ;

	fputs ( szLine , m_oblFile ) ;


	return M4_SUCCESS ;
}

m4return_t ClOblUtil::GetProperty (  m4pchar_t ai_szProperty , m4pchar_t ao_szValue , m4uint32_t ai_szBuffLen )
{
	m4char_t szLine [1024] ;
	m4pchar_t pProp = NULL ;
	m4bool_t Encontrado = M4_FALSE;
	m4bool_t error = M4_FALSE;
	


	while ( ( ! Encontrado) && ( ! feof ( m_oblFile)) )
	{
		fgetpos ( m_oblFile , &m_pos ) ;

		fgets ( szLine , 1024 , m_oblFile  ) ;

		pProp = strstr ( szLine , ai_szProperty ) ;

		if ( pProp )
		{
			Encontrado = M4_TRUE ;
			if ( pProp = strstr ( szLine , "=" ) )
			{
				if ( ai_szBuffLen < strlen (pProp) )
				{
					error = M4_TRUE ;
				}
				else
					sprintf ( ao_szValue , pProp+1 ) ;
			}
			else
				error = M4_TRUE ;
		}
	}

	if ( error )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
}

m4return_t ClOblUtil::GetProperty ( m4pchar_t ai_szBolckName , m4pchar_t ai_szProperty , m4pchar_t ao_szValue , m4uint32_t ai_szBuffLen ) 
{
	m4char_t szLine [1024] ;
	m4pchar_t pProp = NULL ;
	m4bool_t Encontrado = M4_FALSE;
	m4bool_t error = M4_FALSE;
	


	while ( ( ! Encontrado) && ( ! feof ( m_oblFile)) )
	{
		fgetpos ( m_oblFile , &m_pos ) ;

		fgets ( szLine , 1024 , m_oblFile  ) ;

		pProp = strstr ( szLine , ai_szBolckName ) ;

		if ( pProp )
		{
			Encontrado = M4_TRUE ;
		}
	}

	Encontrado = M4_FALSE ;

	while ( ( ! Encontrado) && ( ! feof ( m_oblFile)) )
	{
		fgetpos ( m_oblFile , &m_pos ) ;

		fgets ( szLine , 1024 , m_oblFile  ) ;

		pProp = strstr ( szLine , ai_szProperty ) ;

		if ( pProp )
		{
			Encontrado = M4_TRUE ;
			if ( pProp = strstr ( szLine , "=" ) )
			{
				if ( ai_szBuffLen < strlen (pProp) )
				{
					error = M4_TRUE ;
				}
				else
					sprintf ( ao_szValue , pProp+1 ) ;
			}
			else
				error = M4_TRUE ;
		}
	}

	if ( error )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
}
