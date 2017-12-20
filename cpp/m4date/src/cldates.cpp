//## begin module.includes preserve=yes
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cldates.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                19-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the  date manipulating functions
//
//
//==============================================================================



#include "cldates.hpp"
#include "m4date.hpp"
#include <math.h>

#include "syncro.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes
static ClMutex s_oMxTime(M4_TRUE);


void    ClNumbersToDate( int ai_iYear, int ai_iMonth, int ai_iDay, m4int32_t ai_iHour, m4int32_t ai_iMinute, m4int32_t ai_iSecond, m4date_t &ao_rdDate )
{
	ao_rdDate = numdias( ai_iDay, ai_iMonth, ai_iYear ) ;

	ao_rdDate += ai_iHour   / M4_HOURS_DAY ;
	ao_rdDate += ai_iMinute / M4_MINUTES_DAY ;
	ao_rdDate += ai_iSecond / M4_SECONDS_DAY ;

	if ( ao_rdDate < 0 )
	{
		ao_rdDate = 0 ;
	}
}


void    ClDateToNumbers( m4date_t ai_dDate, int &ao_riYear, int &ao_riMonth, int &ao_riDay, m4int32_t &ao_riHour, m4int32_t &ao_riMinute, m4int32_t &ao_riSecond )
{

	m4int32_t	iInteger ;
	m4double_t	dRemain ;


	iInteger = m4int32_t( ai_dDate ) ;

	dRemain = ai_dDate - iInteger ;
	ao_riHour = m4int32_t( dRemain * M4_HOURS_DAY ) ;

	dRemain -= ao_riHour / M4_HOURS_DAY ;
	ao_riMinute = m4int32_t( dRemain * M4_MINUTES_DAY ) ;

	dRemain -= ao_riMinute / M4_MINUTES_DAY ;
	ao_riSecond = m4int32_t( dRemain * M4_SECONDS_DAY + 0.5 ) ;

	if( ao_riSecond == 60 )
	{
		ao_riSecond = 0 ;
		ao_riMinute++ ;

		if( ao_riMinute == 60 )
		{
			ao_riMinute = 0 ;
			ao_riHour++ ;

			if( ao_riHour == 24 )
			{
				ao_riHour = 0 ;
				iInteger++ ;
			}
		}
	}

	diamesano( JulianType( iInteger ), &ao_riDay, &ao_riMonth, &ao_riYear ) ;
}


void    ClStringToDate( m4pcchar_t ai_pszString, m4date_t &ao_rdDate )
{
	if( ai_pszString )
	{
		ClStringToDate( ai_pszString, strlen( ai_pszString ), ao_rdDate ) ;
	}
	else
	{
		ao_rdDate = 0.0 ;
	}
}


void    ClStringToDate( m4pcchar_t ai_pszString, size_t ai_szLength, m4date_t &ao_rdDate )
{

	int			iYear ;
	int			iMonth ;
	int			iDay ;
	m4int32_t	iHour ;
	m4int32_t	iMinute ;
	m4int32_t	iSecond ;


	if( ( ai_pszString ) && ( ai_szLength >= M4_DAY_END ) )
	{
		iYear  = atoi( ai_pszString + M4_YEAR ) ;
		iMonth = atoi( ai_pszString + M4_MONTH ) ;
		iDay   = atoi( ai_pszString + M4_DAY ) ;

		if( ai_szLength >= M4_HOUR_END )
		{
			iHour = atoi( ai_pszString + M4_HOUR ) ;

			if( ai_szLength >= M4_MINUTE_END )
			{
				iMinute = atoi( ai_pszString + M4_MINUTE ) ;

				if( ai_szLength >= M4_SECOND_END )
				{
					iSecond = atoi( ai_pszString + M4_SECOND ) ;
				}
				else
				{
					iSecond = 0 ;
				}
			}
			else
			{
				iMinute = 0 ;
				iSecond = 0 ;
			}
		}
		else
		{
			iHour = 0 ;
			iMinute = 0 ;
			iSecond = 0 ;
		}

        ClNumbersToDate( iYear, iMonth, iDay, iHour, iMinute, iSecond, ao_rdDate ) ;
	}
	else
	{
		ao_rdDate = 0.0 ;
	}
}


m4date_t    ClStringToDate( m4pcchar_t ai_pszString )
{

	m4date_t	dResult ;

	ClStringToDate( ai_pszString, dResult ) ;

	return( dResult ) ;
}



void    ClDateToString( m4date_t ai_dDate, m4pchar_t ao_pszString, size_t ai_szLength )
{

	m4return_t	iResult = M4_SUCCESS ;
	int			iYear ;
	int			iMonth ;
	int			iDay ;
	m4int32_t	iHour ;
	m4int32_t	iMinute ;
	m4int32_t	iSecond ;
	char		azTemp[ M4_SECOND_END + 1 ] ;


	if( ao_pszString )
	{
		if (ai_dDate<0)  //Antes de Xto no habia Mind
		{
			ai_dDate = 0;
		}

		ClDateToNumbers( ai_dDate, iYear, iMonth, iDay, iHour, iMinute,iSecond ) ;
		
		if (iYear<0)  //puede dar un número negativo por overflow. Tomamos el abs
		{
			iYear = -iYear;
		}

		if ( iYear >= M4CL_PLUS_INFINITE_YEAR )	//para evitar años de 5 cifras que provocan memory overrun. Con esto tenemos el problema del año 9999 ;-)
												//queda como topo la fecha máxima
		{
			iYear = M4CL_PLUS_INFINITE_YEAR ;
			iMonth = 1 ;
			iDay = 1;
			iHour = 0 ;
			iMinute = 0 ;
			iSecond = 0;
		}

		sprintf( azTemp, "%04d-%02d-%02d %02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMinute, iSecond ) ;

		if( ai_szLength > M4_SECOND_END + 1 )
		{
			ai_szLength = M4_SECOND_END + 1 ;
		}

		memcpy( ao_pszString, azTemp, sizeof( m4char_t ) * ( ai_szLength - 1 ) ) ;
		ao_pszString[ ai_szLength - 1 ] = '\0' ;
	}
}


void    ClLongsToDate( m4uint32_t ai_iDays, m4uint32_t ai_iSeconds, m4date_t &ao_rdDate )
{
	ao_rdDate = ai_iDays + ai_iSeconds / M4_SECONDS_DAY ;
}


void    ClDateToLongs( m4date_t ai_dDate, m4uint32_t &ao_riDays, m4uint32_t &ao_riSeconds )
{
    ao_riDays = m4uint32_t( ai_dDate ) ;
    ao_riSeconds = m4uint32_t( ( ai_dDate - ao_riDays ) * M4_SECONDS_DAY + 0.5 ) ;
}


void    ClActualDate( m4date_t &ao_rdDate, m4uint8_t ai_Mode )
{
    time_t      lTime ;
    struct tm   stDate ;

	s_oMxTime.Lock ();

    time( &lTime ) ;

    switch( ai_Mode )
    {
        case M4CL_TIME_LOCAL_TIME:

			M4ClDate::LocalTime( lTime,stDate) ;
            break ;

        default :

            M4ClDate::GmTime( lTime,stDate ) ;
            break ;
    }

	s_oMxTime.Unlock ();

    ClNumbersToDate( stDate.tm_year + 1900, stDate.tm_mon + 1, stDate.tm_mday, stDate.tm_hour, stDate.tm_min, stDate.tm_sec, ao_rdDate ) ;
}


m4date_t    ClActualDate( m4uint8_t ai_Mode )
{

    m4date_t    dDate ;


    ClActualDate( dDate, ai_Mode ) ;

    return( dDate ) ;
}




void    ClActualDate( m4uint32_t &ao_riDays, m4uint32_t &ao_riSeconds, m4uint8_t ai_Mode )
{

    m4date_t    dDate ;

    ClActualDate( dDate, ai_Mode ) ;
    ClDateToLongs( dDate, ao_riDays, ao_riSeconds ) ;
}


void    ClActualDate( m4pchar_t ao_pszString, size_t ai_szLength, m4uint8_t ai_Mode )
{

    m4date_t    dDate ;

    ClActualDate( dDate, ai_Mode ) ;
    ClDateToString( dDate, ao_pszString, ai_szLength ) ;
}


m4date_t    ClDateRoundToDays( m4date_t ai_dDate )
{

	m4int32_t	iInteger ;


	if( ai_dDate >= 0 )
	{
		iInteger = m4int32_t( ai_dDate + 0.5 ) ;
	}
	else
	{
		iInteger = m4int32_t( ai_dDate - 0.5 ) ;
	}

    return( iInteger ) ;
}


m4date_t    ClDateRoundToHours( m4date_t ai_dDate )
{

	m4int32_t	iInteger ;


	if( ai_dDate >= 0 )
	{
		iInteger = m4int32_t( ai_dDate * M4_HOURS_DAY + 0.5 ) ;
	}
	else
	{
		iInteger = m4int32_t( ai_dDate * M4_HOURS_DAY - 0.5 ) ;
	}

    return( iInteger / M4_HOURS_DAY ) ;
}


m4date_t    ClDateRoundToMinutes( m4date_t ai_dDate )
{

	m4int32_t	iInteger ;


	if( ai_dDate >= 0 )
	{
		iInteger = m4int32_t( ai_dDate * M4_MINUTES_DAY + 0.5 ) ;
	}
	else
	{
		iInteger = m4int32_t( ai_dDate * M4_MINUTES_DAY - 0.5 ) ;
	}

    return( iInteger / M4_MINUTES_DAY ) ;
}


m4date_t    ClDateRoundToSeconds( m4date_t ai_dDate )
{

	m4int32_t	iDays ;
	m4int32_t	iInteger ;


	// Hay que controlar el overflow
	iDays = m4int32_t( ai_dDate ) ;
	ai_dDate -= iDays ;

	if( ai_dDate >= 0 )
	{
		iInteger = m4int32_t( ai_dDate * M4_SECONDS_DAY + 0.5 ) ;
	}
	else
	{
		iInteger = m4int32_t( ai_dDate * M4_SECONDS_DAY - 0.5 ) ;
	}

    return( iInteger / M4_SECONDS_DAY + iDays ) ;
}


m4date_t ClMinusInfiniteDate( void )
{
	return ClStringToDate( M4CL_MINUS_INFINITE_DATE ) ;
}


m4date_t ClPlusInfiniteDate (void)
{
	return ClStringToDate( M4CL_PLUS_INFINITE_DATE ) ;
}


m4date_t ClTime_tToDate( time_t ai_lTime, m4uint8_t ai_Mode )
{
    struct tm   stDate ;
	 m4date_t rdDate;

 	 s_oMxTime.Lock ();

    switch( ai_Mode )
    {
        case M4CL_TIME_LOCAL_TIME:

			M4ClDate::LocalTime( ai_lTime,stDate) ;
            break ;

        default :

            M4ClDate::GmTime( ai_lTime,stDate ) ;
            break ;
    }

	 s_oMxTime.Unlock ();

    ClNumbersToDate( stDate.tm_year + 1900, stDate.tm_mon + 1, stDate.tm_mday, stDate.tm_hour, stDate.tm_min, stDate.tm_sec, rdDate ) ;
    return rdDate;
}

 
//## end module.epilog
