


#include <m4meminfo.hpp>
#include <m4types.hpp>
#include <m4date.hpp>


#include <m4win.hpp>
#include <stdio.h>
#include <process.h>
#include <pdh.h>
#include "locale.h"



#define	M4MEMINFO_BUFFER_SIZE			50
#define	M4MEMINFO_ITERATIONS			 2



m4return_t  M4GetSystemCPU( m4double_t &ao_dSystemCpu )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	j = 0 ;
	m4uint32_t	iHits = 0 ;
	m4uint32_t	iIterations = 0 ;
	m4double_t	dCpuSum = 0 ;
	long		lError = ERROR_SUCCESS ;
	DWORD		dwSize = 0 ;
	m4char_t	acProcessor	[     M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;
	m4char_t	acTime		[     M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;
	m4char_t	acCounter	[ 4 * M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;

	SYSTEM_INFO				stSystemInfo ;
	PDH_FMT_COUNTERVALUE	stFmtCounterValue ;

	static	m4char_t	s_cInit = -1 ;
	static	m4uint32_t	iProcessors = -1 ;
	static	HQUERY		s_hQuery = 0 ;
	static	HCOUNTER	*s_phCounter = NULL ;


	// Se inicializa la variable de salida
	ao_dSystemCpu = 0 ;

	// Si se intentó inicializar y no se pudo no se continua. Bug 0099545
	if( s_cInit == 0 )
	{
		return( M4_ERROR ) ;
	}


	// Se calculan los procesadores
	if( s_phCounter == NULL )
	{
		GetSystemInfo( &stSystemInfo ) ;

		iProcessors = stSystemInfo.dwNumberOfProcessors ;

		s_phCounter = new HCOUNTER[ iProcessors ] ;

		if( s_phCounter == NULL )
		{
			return( M4_ERROR ) ;
		}

		memset( s_phCounter, 0, iProcessors * sizeof( HCOUNTER ) ) ;
	}


	// Se inicializa si no está inicializado
	if( s_cInit == -1 )
	{
		// Se marca que se ha intentado inicializar
		s_cInit = 0 ;

		// Se leen los nombres de lo que se va a medir
		dwSize = M4MEMINFO_BUFFER_SIZE ;
		lError = PdhLookupPerfNameByIndex( NULL, 238, acProcessor, &dwSize ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		dwSize = M4MEMINFO_BUFFER_SIZE ;
		lError = PdhLookupPerfNameByIndex( NULL, 6, acTime, &dwSize ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		// Se inicializa la query
		lError = PdhOpenQuery( NULL, 0, &s_hQuery ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		for( i = 0 ; i < iProcessors ; i++ )
		{
			sprintf( acCounter, "\\%s(%d)\\%s", acProcessor, i , acTime ) ;

			lError = PdhAddCounter( s_hQuery, acCounter, 0, &s_phCounter[ i ] ) ;
			setlocale( LC_ALL, "C" ) ;

			if( lError != ERROR_SUCCESS )
			{
				for( j = 0 ; j < i ; j++ )
				{
					PdhRemoveCounter( s_phCounter[ j ] ) ;
				}

				delete [] s_phCounter ;
				s_phCounter = NULL ;

				PdhCloseQuery( s_hQuery ) ;
				return( M4_ERROR ) ;
			}
		}

		// Se marca que se ha inicializado correctamente
		s_cInit = 1 ;
	}


	// Se leen las cpus 2 veces (por el delay en PdhGetFormattedCounterValue) y se porratea
	iHits = 0 ;
	dCpuSum = 0 ;

	for( iIterations = 0 ; iIterations < M4MEMINFO_ITERATIONS ; iIterations++ )
	{
		for( i = 0 ; i < iProcessors ; i++ )
		{
			m4sleep( 1 ) ;

			lError = PdhCollectQueryData( s_hQuery ) ;
			setlocale( LC_ALL, "C" ) ;

			if( lError != ERROR_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			// Se lee el proceso
			lError = PdhGetFormattedCounterValue( s_phCounter[ i ], PDH_FMT_DOUBLE | PDH_FMT_NOSCALE, NULL, &stFmtCounterValue ) ;
			setlocale( LC_ALL, "C" ) ;

			if( lError != ERROR_SUCCESS )
			{
				continue ;
			}

			dCpuSum += stFmtCounterValue.doubleValue ;
			iHits++ ;
		}
	}
	
	if( iHits > 0 )
	{
		ao_dSystemCpu = dCpuSum / iHits ;
	}

	return( M4_SUCCESS ) ;
}



m4int32_t	M4GetProcessMem( m4uint64_t &ao_lPrivateBytes )
{

	m4uint32_t	iIndex = 0 ;
	m4uint32_t	iProcessId = 0 ;
	long		lError = ERROR_SUCCESS ;
	DWORD		dwSize = 0 ;
	m4char_t	acProcess		[     M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;
	m4char_t	acPrivateBytes	[     M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;
	m4char_t	acProcessID		[     M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;
	m4char_t	acCounter		[ 4 * M4MEMINFO_BUFFER_SIZE + 1 ] = "" ;

	PDH_FMT_COUNTERVALUE	stFmtCounterValue ;

	static	m4char_t	s_cInit = -1 ;
	static	m4uint32_t	s_iCurrentProcessId = GetCurrentProcessId() ;
	static	HQUERY		s_hQuery = 0 ;
	static	HCOUNTER	s_hProcessId = 0 ;
	static	HCOUNTER	s_hBytes = 0 ;


	// Se inicializa la variable de salida
	ao_lPrivateBytes = 0 ;

	// Si se intentó inicializar y no se pudo no se continua. Bug 0099545
	if( s_cInit == 0 )
	{
		return( M4_ERROR ) ;
	}


	/* Bug 0117318
	Se comprueba que el marcador siga siendo válido para este proceso
	Si es así se lee el valor y sino se vuelve a inicializar
	*/
	if( s_cInit == 1 )
	{
		lError = PdhCollectQueryData( s_hQuery ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError == ERROR_SUCCESS )
		{
			lError = PdhGetFormattedCounterValue( s_hProcessId, PDH_FMT_LONG | PDH_FMT_NOSCALE, NULL, &stFmtCounterValue ) ;
			setlocale( LC_ALL, "C" ) ;

			if( lError == ERROR_SUCCESS )
			{
				iProcessId = stFmtCounterValue.longValue ;

				if( iProcessId == s_iCurrentProcessId )
				{
					// Se leen los bytes
					lError = PdhGetFormattedCounterValue( s_hBytes, PDH_FMT_LARGE | PDH_FMT_NOSCALE, NULL, &stFmtCounterValue ) ;
					setlocale( LC_ALL, "C" ) ;

					if( lError == ERROR_SUCCESS )
					{
						ao_lPrivateBytes = stFmtCounterValue.largeValue ;
						return( M4_SUCCESS ) ;
					}
				}
			}
		}

		PdhRemoveCounter( s_hBytes ) ;
		PdhRemoveCounter( s_hProcessId ) ;
		PdhCloseQuery( s_hQuery ) ;

		s_hBytes = 0 ;
		s_hProcessId = 0 ;
		s_hQuery = 0 ;
	}


	// Se marca que se ha intentado inicializar
	s_cInit = 0 ;

	// Se leen los nombres de lo que se va a medir
	dwSize = M4MEMINFO_BUFFER_SIZE ;
	lError = PdhLookupPerfNameByIndex( NULL, 230, acProcess, &dwSize ) ;
    setlocale( LC_ALL, "C" ) ;

    if( lError != ERROR_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	dwSize = M4MEMINFO_BUFFER_SIZE ;
	lError = PdhLookupPerfNameByIndex( NULL, 186, acPrivateBytes, &dwSize ) ;
    setlocale( LC_ALL, "C" ) ;

    if( lError != ERROR_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	dwSize = M4MEMINFO_BUFFER_SIZE ;
	lError = PdhLookupPerfNameByIndex( NULL, 784, acProcessID, &dwSize ) ;
    setlocale( LC_ALL, "C" ) ;

    if( lError != ERROR_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se inicializa la query
	lError = PdhOpenQuery( NULL, 0, &s_hQuery ) ;
    setlocale( LC_ALL, "C" ) ;

    if( lError != ERROR_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iIndex = 0 ;

	do
	{
		// Contador del proceso
		sprintf( acCounter, "\\%s(%s#%d)\\%s", acProcess, "m4server", iIndex, acProcessID ) ;

		lError = PdhAddCounter( s_hQuery, acCounter, 0, &s_hProcessId ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			PdhCloseQuery( s_hQuery ) ;
			return( M4_ERROR ) ;
		}

		// Contador de los bytes
		sprintf( acCounter, "\\%s(%s#%d)\\%s", acProcess, "m4server", iIndex, acPrivateBytes ) ;

		lError = PdhAddCounter( s_hQuery, acCounter, 0, &s_hBytes ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			PdhRemoveCounter( s_hProcessId ) ;
			PdhCloseQuery( s_hQuery ) ;
			return( M4_ERROR ) ;
		}

		lError = PdhCollectQueryData( s_hQuery ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			PdhRemoveCounter( s_hBytes ) ;
			PdhRemoveCounter( s_hProcessId ) ;
			PdhCloseQuery( s_hQuery ) ;
			return( M4_ERROR ) ;
		}

		// Se lee el proceso
		lError = PdhGetFormattedCounterValue( s_hProcessId, PDH_FMT_LONG | PDH_FMT_NOSCALE, NULL, &stFmtCounterValue ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			PdhRemoveCounter( s_hBytes ) ;
			PdhRemoveCounter( s_hProcessId ) ;
			PdhCloseQuery( s_hQuery ) ;
			return( M4_ERROR ) ;
		}

		iProcessId = stFmtCounterValue.longValue ;

		if( iProcessId != s_iCurrentProcessId )
		{
			PdhRemoveCounter( s_hBytes ) ;
			PdhRemoveCounter( s_hProcessId ) ;
			iIndex++ ;
			continue ;
		}

		// Se leen los bytes
		lError = PdhGetFormattedCounterValue( s_hBytes, PDH_FMT_LARGE | PDH_FMT_NOSCALE, NULL, &stFmtCounterValue ) ;
		setlocale( LC_ALL, "C" ) ;

		if( lError != ERROR_SUCCESS )
		{
			PdhRemoveCounter( s_hBytes ) ;
			PdhRemoveCounter( s_hProcessId ) ;
			PdhCloseQuery( s_hQuery ) ;
			return( M4_ERROR ) ;
		}

		// Se marca que se ha inicializado correctamente
		s_cInit = 1 ;

		ao_lPrivateBytes = stFmtCounterValue.largeValue ;
		return( M4_SUCCESS ) ;

	}
	while( 1 ) ;


	return( M4_SUCCESS ) ;
}


m4int32_t  M4GetSystemMem (  m4uint64_t & ao_lAvailPhysMem , m4uint64_t & ao_lTotalPhysMem , m4uint64_t & ao_lAvailSwapMem , m4uint64_t & ao_lTotalSwapMem ) 
{		
	MEMORYSTATUSEX memstatus;
	BOOL bRet = false;
	DWORD dwLastError = 0;

	
	memstatus.dwLength = sizeof (memstatus);

	// bg 106708
	bRet = GlobalMemoryStatusEx ( &memstatus ) ;

	if( bRet != false )
	{
		ao_lTotalPhysMem =memstatus.ullTotalPhys ;
		ao_lAvailPhysMem =memstatus.ullAvailPhys ;
		ao_lTotalSwapMem =memstatus.ullTotalPageFile -ao_lTotalPhysMem;
		ao_lAvailSwapMem =memstatus.ullAvailPageFile -ao_lAvailPhysMem ;
	}
	else
	{
		dwLastError = GetLastError();
		return M4_ERROR;
	}


	return M4_SUCCESS ;
}

