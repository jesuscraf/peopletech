//
//==============================================================================
//
//             (c) Copyright META4
//
//
//   Module:             Clase que engloba las funciones de manejo de fechas
//                           
//   File:                    M4ClDate.hpp
//   Project:            Calculo Distribuido.
//   Author:                  Jose Miguel Arigita
//   Date:                    24/9/1996
//   Language:           C++
//   Operating System:   MS-DOS, UNIX, NT
//   Design Document:    None.
//
//
//   Definition:
//
//        Funciones generales, las colas del Listener 
//        seguimiento qgeneral de la aplicacion
//        
//
//   Changes:
//        Version        Date      Author         Comment
//------------------------------------------------------------------------------
//
//
//==============================================================================
//



//==============================================================================
//
//   Include files from the project.
//
//==============================================================================

#include "m4stl.hpp"
#include "m4string.hpp"
#include "syncro.hpp"


extern "C" {
#include <stdio.h>
#include <time.h>
}

#include <math.h>

#include "m4date.hpp" 

static ClMutex	s_Mutex(M4_TRUE);

#ifdef _UNIX

/*
 * JRM, 02/Dec/1998 
 * Cambio m4sleep, debido a que:
 *
 *  Los defines de funciones son peligrosos; es mejor crear una función que encapsule
 * las diferencias entre distintos s.o.
 * 
 *  En el s.o. Solaris, usar sleep en  entorno multithread provoca errores. Por ello, se 
 * aprovecha el timeout que proporciona la función select, que si es MT safe.
 */
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

void m4sleep(int sec)
{
	sleep(sec);
}

void m4msleep(int msec)
{
	usleep(msec * 1000);
}

void m4usleep(int usec)
{
	usleep(usec);
}

#else

#include <windows.h>

void m4sleep(int sec) 
{
	Sleep(sec * 1000);
}

void m4msleep(int msec) 
{
	Sleep(msec);
}

void m4usleep(int usec)
{
	struct timeval timeout;

	/* Bug 0276025
	Se hace el socket estático para evitar leaks
	*/
    static SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd_set dummy;
    FD_ZERO(&dummy);
    FD_SET(s, &dummy);

	timeout.tv_sec = 0;
	timeout.tv_usec = usec;
    select(0, NULL, NULL, &dummy, &timeout);
}
#endif


//==============================================================================
//
//   Defines from the project.
//
//==============================================================================

#define	M4SECONDS_DAY		86400.0

#define M4SECONDS_HOUR      3600
#define M4SECONDS_MINUTE    60

//////////////////////////   DEFINES

#define ZERO  48   /// es el '0' en cifras
    

//==============================================================================
//
//   Definición  de las funciones de Julianos
//
//==============================================================================


        /************************************************/
        /****************** M4ClDate_diamesano *******************/
        /************************************************/
        /* m4int32_terna calcula dia, mes a¤o a partir del n§ */
        /* de d¡as desde 31-12-0000                     */
        /************************************************/

m4dates_t M4ClDate::numdias (m4int32_t dia, m4int32_t mes, m4int32_t anno)
{

        m4dates_t result;
        /* tabla de d¡as acumulados por mes */
        static const m4int32_t dpmAccTab [13] =
        {
            0, 0, 31, 28+31,
            31+59, 30+31+59, 31+30+31+59,
            30+151, 31+30+151, 31+31+30+151,
            30+243, 31+30+243, 30+31+30+243
        };

        m4int32_t dias;
        m4int32_t bisiestos, anno1;

        if (mes < 1)    /* ojo con los meses negativos */
        {
            anno = anno - (((-mes) / 12) + 1);
            mes = 12 - ((-mes) % 12);
        }

        anno1 = mes <= 2 ? anno-1 : anno;

        bisiestos = anno1 / 4 ;
        bisiestos -= anno1/100 ;
        bisiestos += anno1/400 ;
        dias = ((m4int32_t)anno-1) * 365 + (m4int32_t)bisiestos + (m4int32_t)dpmAccTab [mes] + (m4int32_t)(dia);

        if ((((m4int32_t)anno * 10000) + ((m4int32_t)mes *100) + (m4int32_t)dia ) >= ((GREGORIO_ANO*10000L) + (GREGORIO_MES*100L) + GREGORIO_DIA )) dias -= GREGORIO_DIAS;
        result = dias;
        return(result);
  }

/*
 * JRM Esta función debería ser de tipo void
 * No lo cambio por no hacer cambios en el API, pero lo apunto como
 * cambio futuro. Debería ser de tipo void ya que siempre retorna lo mismo, M4_SUCCESS
 * La modificación que sí he hecho es quitar la propiedad static de la tabla
 * diamesTab, ya que en entorno multithread puede provocar errores en acceso y modificación
 * de los días de Febrero
 */
m4return_t M4ClDate::diamesano (m4dates_t dias , 
				m4int32_t * dia , m4int32_t * mes , m4int32_t * ano )
{
        /* tabla de d¡as de cada mes. Febrero se modifica. */
        /*static */m4int32_t diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        m4dates_t diascomp;
        m4int32_t diadelano;
        m4int32_t mes1  = 0;
        m4int32_t *mptr = diasmesTab;
        m4dates_t diasg;             /* con la correcci¢n de gregorio en 1752 */
        *ano  = ((m4int32_t)(((dias > GREGORIO_NUM_DIAS ? dias + GREGORIO_DIAS : dias)/365.2425)+.002))+1;
        diascomp = numdias( 1 , 1 , *ano );
        if (diascomp > dias ) (*ano)--;
            diasg = ((*ano == GREGORIO_ANO) && (dias > GREGORIO_NUM_DIAS)) ? dias + GREGORIO_DIAS : dias;

        diadelano = (m4int32_t) (diasg - (numdias( 1 , 1 , *ano )));
        if ((*ano % 4  ) == 0)
            diasmesTab[1] = 29;     /* Los m£ltiplos de 4 son bisiestos */
        else
            diasmesTab[1] = 28;
        if ((*ano % 100) == 0)
            diasmesTab[1] = 28;     /* L¢s multiplos de 100 no */
        if ((*ano % 400) == 0)
            diasmesTab[1] = 29;     /* L¢s multiplos de 400 si */
        while (diadelano >= *mptr)
        {
            ++mes1;
            diadelano  -= *mptr++ ;
        }
        *mes = mes1 + 1;
        *dia = diadelano + 1;
        return M4_SUCCESS;
  }



		/************************************************/
        /****************** M4ClDate_diamesano *******************/
        /************************************************/
        /* valida una fecha                             */
        /*                                              */
        /************************************************/

/// los meses van de 1-enero al 12-Diciembre
/*
 * JRM Cambio de optimización: antes de hacer cálculos, pregunto por condiciones de 
 * retorno inmediato
 */
m4bool_t M4ClDate::is_ok( m4int32_t year, m4int32_t month, m4int32_t day)
{
	if (month < 1 || month > 12 || day < 1)
	{
		return M4_FALSE;
	}

	m4int32_t diasmesTab [13] =
	{
		0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	
	/// añadimos los bisiestos
	if ( (month == 2) &&   // si es febrero
		( (year % 400 == 0) || 
		( (year % 4 == 0) && ( year %  100 != 0) )
		) // si es bisiesto
		)  // los múltiplos de 100 no lo son
		
	{
		diasmesTab[2]++;
	}

	if (day > diasmesTab[month])
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}


//==============================================================================
//
//   Definición clase M4ClDate,  que es la que accede a la BD
//
//==============================================================================
/*
 * JRM Se sustituye la llamada a gmtime por M4ClDate::GmTime, que es MT safe
 */
m4return_t M4ClDate::now()
{
   struct tm when;
   time_t now;
   
   time( &now );
   M4ClDate::GmTime(now, when);

   return set( when.tm_year + 1900, when.tm_mon +1, when.tm_mday);
}

                                              
  //////////////  conversores de tipo
  
  //////////////  operaciones especificas  &}  puntero al objeto
m4return_t M4ClDate::set (const m4pchar_t ai_pcDate)
{

	m4int32_t year=0, month=0, day=0, hour=0, minute=0, sec=0;  

	if ( strlen(ai_pcDate) < 8)
	{
		/// la fecha pasada es erronea
		return M4_ERROR;
	}

		year = 	((ai_pcDate[0] - '0') * 1000) +
				((ai_pcDate[1] - '0') * 100)  +
				((ai_pcDate[2] - '0') * 10)   +
				(ai_pcDate[3] - '0'          );
		month = ((ai_pcDate[5] -'0') * 10) +
				(ai_pcDate[6] - '0'	    );
		day = 	((ai_pcDate[8] - '0') * 10) +
				(ai_pcDate[9] - '0'      );

	return M4ClDate::set (  year, month, day);

}

/*
 * JRM
 * Cambio para aprovechar M4ClDate::set (const m4pchar_t ai_pcDate) y no 
 * repetir código, que es siempre proclive a errores
 */
m4return_t M4ClDate::set (const M4ClString & ai_pcDate)
{
	return set((const m4pchar_t) ai_pcDate.c_str());

/*
	m4int32_t year=0, month=0, day=0, hour=0, minute=0, sec=0;  

	if ( ai_pcDate.size() < 8)
	{
		/// la fecha pasada es erronea
		return M4_ERROR;
	}

		year = 	((ai_pcDate[0] - '0') * 1000) +
				((ai_pcDate[1] - '0') * 100)  +
				((ai_pcDate[2] - '0') * 10)   +
				(ai_pcDate[3] - '0'          );
		month = ((ai_pcDate[5] -'0') * 10) +
				(ai_pcDate[6] - '0'	    );
		day = 	((ai_pcDate[8] - '0') * 10) +
				(ai_pcDate[9] - '0'      );

	return M4ClDate::set (  year, month, day);
*/
}

/*
 * JRM
 * Debiera ser de tipo void, ya que siempre devuelve M4_SUCCESS.
 * Cambio para utilizar sprintf, que además de ser más legible
 * evita un  problema (de tipo altamentes improbable), que había en el código anterior: 
 * si el año es mayor que 9999,  el código anterior devolvía un resultado erróneo
 */
m4return_t M4ClDate::get (M4ClString &ai_pcDate)
{
	m4int32_t iyear, imon, iday;
	char	buffer[250];

	diamesano( m_numdays, &iday, &imon, &iyear);
	sprintf(buffer, "%04d-%02d-%02d", iyear, imon, iday);
	ai_pcDate = buffer;

	return M4_SUCCESS;
/*
	m4char_t pcBuffer[2];
	
	diamesano( m_numdays, &iday, &imon, &iyear);

	pcBuffer[1] = '\0';

	pcBuffer[0] = (char) (iyear / 1000 + ZERO);
	ai_pcDate = pcBuffer;
	iyear %= 1000;
	pcBuffer[0] = (char) (iyear / 100 + ZERO);
	ai_pcDate += pcBuffer;
	iyear %= 100;
	pcBuffer[0] = (char) (iyear / 10 + ZERO);
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (iyear % 10 + ZERO);
	ai_pcDate += pcBuffer;

	pcBuffer[0] = '-';
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (imon / 10 + ZERO);
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (imon % 10 + ZERO);
	ai_pcDate += pcBuffer;

	pcBuffer[0] = '-';
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (iday / 10 + ZERO);
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (iday % 10 + ZERO);
	ai_pcDate += pcBuffer;


	return M4_SUCCESS;
*/
}

m4return_t M4ClDate::set ( const m4int32_t &year, const m4int32_t &month, const m4int32_t &day)
{
	//// OJO el año tiene que ser entero

	if ( !is_ok ( year, month, day))
	{
		return M4_ERROR;
	}

	m_numdays = numdias( day, month, year);

	 return M4_SUCCESS;
}

m4return_t M4ClDate::get ( m4int32_t &a_year, m4int32_t &a_month, m4int32_t &a_day)
{
	return diamesano( m_numdays, &a_day, &a_month, &a_year);
}

/*
 * JRM 
 * En el sistema inglés, el primer día de la semana es el domingo, no el Lunes
 * No cambio nada, ya que el que use esta función espera que el Lunes sea el 1, ...etc.
 */
m4return_t M4ClDate::get_dayofWeek( m4int32_t &ao_wday) // 1-Lunes, 2-Martes, 3-Miercoles, .., 7-Domingo
{
        ao_wday = (( ((int)m_numdays) % 7) + 4);
        ao_wday = ao_wday > 7 ? ao_wday - 7 : ao_wday;
        return M4_SUCCESS;
}


m4dates_t M4ClDate::operator + (const m4dates_t &ai_hora1)
{
	/*
	 * JRM 
	 * No entiendo la necesidad de hacer un código tan complejo, que en realidad
	 * equivale a la modificación que hago
	 */
	return m_numdays + ai_hora1;
/*
	m4dates_t  Temp_TS;

	Temp_TS = this->m_numdays + ai_hora1;

	return Temp_TS;
*/
}

m4dates_t M4ClDate::operator - (const m4dates_t &ai_hora1)
{
	return m_numdays - ai_hora1;

/*
	m4dates_t  Temp_TS;

	Temp_TS = this->m_numdays - ai_hora1;

	return Temp_TS;
*/
}


void M4ClDate::LocalTime(time_t ai_time, struct tm &ao_tm)
{
#ifdef _WINDOWS   
	struct tm* aux_tm;
	aux_tm = localtime(&ai_time);
	ao_tm = *aux_tm;
#else
    localtime_r(&ai_time, &ao_tm);
#endif
}

void M4ClDate::GmTime(time_t ai_time, struct tm &ao_tm)
{
#ifdef _WINDOWS   
	struct tm* aux_tm;
	aux_tm = gmtime(&ai_time);
	ao_tm = *aux_tm;
#else
    gmtime_r(&ai_time, &ao_tm);
#endif
}


void M4ClDate::AscTime (struct tm& ai_time, m4pchar_t ao_buffer)
{
/* The string result produced by asctime contains exactly 26 characters 
 * and has the form Wed Jan 02 02:03:55 1980\n\0.
 */
#ifdef _WINDOWS   
	const char* asc = NULL;
	asc = asctime(&ai_time);
	strcpy(ao_buffer, asc);
#else
    asctime_r(&ai_time, ao_buffer);
#endif

}


/// suma meses a una fecha corrigiendo fechas inexistentes
/* 
 * JRM 
 * diamesano devuelve siempre M4_SUCCESS. Por ello, esta función siempre devuelve
 * M4_SUCCESS. Luego debería ser de tipo void, al igual que diamesano debería ser de tipo void.
 */
m4return_t M4ClDate::add_months( const m4int32_t &ai_num_months)
{
	m4int32_t dia, mes, ano, maxdia;
	m4int32_t mesfin;
	/* tabla de d¡as de cada mes. Febrero se modifica. */
	/*static*/ m4int32_t diasmesTab [12] =
	{
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	
	if ( diamesano (m_numdays , &dia, &mes, &ano) != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	mesfin = mes + ai_num_months;
	
	if (mesfin > 0)
	{
		ano = (m4int32_t) (ano + (mesfin - 1) / 12);
		mesfin = mesfin - ((mesfin - 1) / 12) * 12;
	}
	else
	{
		ano = (m4int32_t) (ano - (((-mesfin) / 12) + 1));
		mesfin = 12 - ((-mesfin) % 12);
	}
	mes = (m4int32_t) mesfin;
	if (mes != 2)
	{
		if (dia > diasmesTab[mes-1])
			dia = diasmesTab[mes-1];
	}
	else
	{
		if ((ano % 4  ) == 0)
			maxdia = 29;            /* Los m£ltiplos de 4 son bisiestos */
		else
			maxdia = 28;
		if ((ano % 100) == 0)
			maxdia = 28;            /* L¢s multiplos de 100 no */
		if ((ano % 400) == 0)
			maxdia = 29;            /* L¢s multiplos de 400 si */
		if (dia > maxdia)
			dia = maxdia;
	}
	m_numdays = numdias (dia, mes, ano);

	return  M4_SUCCESS;
}
/*
 * JRM Debiera ser void 
 */
m4return_t M4ClDate::add_years( const m4int32_t &ai_num_years)
{
        m4int32_t dia, mes, ano;
        m4int32_t annofin, maxdia;

        diamesano (m_numdays , &dia, &mes, &ano);
        annofin = ano + ai_num_years;

        if (dia == 29 && mes == 2)
        {
            if ((annofin % 4  ) == 0)
                maxdia = 29;            /* Los m£ltiplos de 4 son bisiestos */
            else
                maxdia = 28;
            if ((annofin % 100) == 0)
                maxdia = 28;            /* L¢s multiplos de 100 no */
            if ((annofin % 400) == 0)
                maxdia = 29;            /* L¢s multiplos de 400 si */
            if (dia > maxdia)
                dia = maxdia;
        }

        m_numdays = numdias(dia, mes, ano);
        return  M4_SUCCESS;
}

     
//==============================================================================
//
//   Definición clase M4ClHour2Second,  que es la que accede a la BD
//
//==============================================================================
/*
 * JRM
 * Debiera ser void
 */
m4return_t M4ClHour2Second::horaminseg ( m4seconds_t ai_numsec, 
			  m4int32_t *sec, m4int32_t *min, m4int32_t *hour)
{
	m4int32_t numsec = (m4int32_t ) ai_numsec;

	*sec = numsec % M4SECONDS_MINUTE;

	
	*hour = numsec / M4SECONDS_HOUR;
	numsec -= (*hour) * M4SECONDS_HOUR;
	
	*min = numsec / M4SECONDS_MINUTE;
	

	return M4_SUCCESS;
}

m4seconds_t M4ClHour2Second::numseconds( m4int32_t sec, m4int32_t min, m4int32_t hour)
{
	return ( hour * M4SECONDS_HOUR) + ( min * M4SECONDS_MINUTE ) + sec;
}

m4return_t M4ClHour2Second::is_ok( m4int32_t hour, m4int32_t min, m4int32_t sec)
{
   if ( hour < 0 || hour > 23)
   {
	   return M4_ERROR;
   }

   if ( min < 0 || min > 59)
   {
	   return M4_ERROR;
   }

   if ( sec < 0 || hour > 59)
   {
	   return M4_ERROR;
   }

   return M4_SUCCESS;
}

m4return_t M4ClHour2Second::now()
{
   struct tm when;
   time_t now;
   
   time(&now);
   M4ClDate::GmTime(now, when);

   return set(when.tm_hour, when.tm_min, when.tm_sec);
}


m4return_t M4ClHour2Second::set( const M4ClHour2Second &ai_Date)
{
	
	m_numseconds = ai_Date.m_numseconds;     /// numero de  dias desde 1970
    return M4_SUCCESS;
}


                                             
  //////////////  conversores de tipo
  
  //////////////  operaciones especificas  &}  puntero al objeto

m4return_t M4ClHour2Second::set (const m4pchar_t ai_pcDate) //24:12:12
{

	m4int32_t hour=0, min=0, sec=0;

	if ( strlen(ai_pcDate) < 8)
	{
		/// la fecha pasada es erronea
		return M4_ERROR;
	}
	
		hour = ((ai_pcDate[0] -'0') * 10) +
				(ai_pcDate[1] - '0'	    );
		min = ((ai_pcDate[3] -'0') * 10) +
				(ai_pcDate[4] - '0'	    );
		sec = 	((ai_pcDate[6] - '0') * 10) +
				(ai_pcDate[7] - '0'      );
	
	return M4ClHour2Second::set (  hour, min, sec);
	
}

/*
 * JRM
 * Uso la función anterior, en vez de repetir código
 */
m4return_t M4ClHour2Second::set (const M4ClString & ai_pcDate) //24:12:12
{
	return set((const m4pchar_t)ai_pcDate.c_str());
/*
	m4int32_t hour=0, min=0, sec=0;

	if ( ai_pcDate.size() < 8)
	{
		/// la fecha pasada es erronea
		return M4_ERROR;
	}
	
		hour = ((ai_pcDate[0] -'0') * 10) +
				(ai_pcDate[1] - '0'	    );
		min = ((ai_pcDate[3] -'0') * 10) +
				(ai_pcDate[4] - '0'	    );
		sec = 	((ai_pcDate[6] - '0') * 10) +
				(ai_pcDate[7] - '0'      );
	
	return M4ClHour2Second::set (  hour, min, sec);
*/	
}

m4return_t M4ClHour2Second::get (M4ClString &ai_pcDate)
{
	m4int32_t	isec, imin, ihour;
	char		buffer[250];
	
	horaminseg( m_numseconds, &isec, &imin, &ihour);

	sprintf(buffer, "%02d:%02d:%02d", ihour, imin, isec);
	ai_pcDate = buffer;

	return M4_SUCCESS;

/*
	m4char_t pcBuffer[2];
	
	horaminseg( m_numseconds, &isec, &imin, &ihour);
	pcBuffer[1] = '\0';
	
	pcBuffer[0] = (char) (ihour / 10 + ZERO);
	ai_pcDate = pcBuffer;

	pcBuffer[0] = (char) (ihour % 10 + ZERO);
	ai_pcDate += pcBuffer;

	pcBuffer[0] = ':';
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (imin / 10 + ZERO);
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (imin % 10 + ZERO);
	ai_pcDate += pcBuffer;

	pcBuffer[0] = ':';
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (isec / 10 + ZERO);
	ai_pcDate += pcBuffer;
	pcBuffer[0] = (char) (isec % 10 + ZERO);
	ai_pcDate += pcBuffer;

	return M4_SUCCESS;
*/
}

m4return_t M4ClHour2Second::set ( const m4int32_t ai_hour, const m4int32_t ai_min, const m4int32_t ai_sec)
{
	if (is_ok(ai_hour, ai_min, ai_sec) != M4_SUCCESS)
		return M4_ERROR;

	m_numseconds = numseconds(ai_sec, ai_min, ai_hour);

	return M4_SUCCESS;
}

m4return_t M4ClHour2Second::get ( m4int32_t &ao_ihour, m4int32_t &ao_imin, m4int32_t &ao_isec)
{
	return horaminseg(m_numseconds, &ao_isec, &ao_imin, &ao_ihour);
}

m4return_t M4ClHour2Second::set (const m4seconds_t ai_time)
{
	if (ai_time < 0 || ai_time >= 86400)
	{
		//// comprueba que el dato sea correcto
		return M4_ERROR;
	}
	m_numseconds = ai_time;

	return M4_SUCCESS;
}

m4return_t M4ClHour2Second::get (m4seconds_t &ao_time)
{
	ao_time = m_numseconds;

	return M4_SUCCESS;
}


m4seconds_t M4ClHour2Second::operator + (const m4seconds_t &ai_hora1)
{
	return m_numseconds + ai_hora1;
/*
	m4seconds_t  Temp_TS;

	Temp_TS = this->m_numseconds + ai_hora1;

	return Temp_TS;
*/
}

m4seconds_t M4ClHour2Second::operator - (const m4seconds_t &ai_hora1)
{
	return m_numseconds - ai_hora1;
/*
	m4seconds_t  Temp_TS;

	Temp_TS = this->m_numseconds - ai_hora1;

	return Temp_TS;
*/
}

m4return_t M4ClHour2Second::add_seconds( const m4int32_t &ai_num_seconds)
{
	m4return_t retcode = M4_SUCCESS;
	m_numseconds += ai_num_seconds;

	if ( m_numseconds > 86400)
	{
		retcode = M4_WARNING;
	}

	m_numseconds %= 86400;

	return retcode;
}

m4return_t M4ClHour2Second::add_minutes(const m4int32_t &ai_num_minutes)
{
	m_numseconds += ( ai_num_minutes * M4SECONDS_MINUTE);
	m4return_t retcode;

	if ( m_numseconds > 86400)
	{
		retcode = M4_WARNING;
	}

	m_numseconds %= 86400;

	return retcode;

}

m4return_t M4ClHour2Second::add_hours( const m4int32_t &ai_num_hours)
{
	m_numseconds += ( ai_num_hours * M4SECONDS_HOUR);
	m4return_t retcode;

	if ( m_numseconds > 86400)
	{
		retcode = M4_WARNING;
	}

	m_numseconds %= 86400;

	return retcode;
}

//==============================================================================
//
//   Definición clase M4ClTimeStamp,  que es la que accede a la BD
//
//==============================================================================

// obtiene de un double los dias y los segundos
m4return_t M4ClTimeStamp::timellap_daysec( const m4time_t &ao_timellap, m4dates_t &ai_days, m4seconds_t & ai_seconds)
{

	m4time_t days, seconds;

	if (sizeof(m4time_t) < sizeof(double))
		return M4_ERROR;

	seconds = modf( ao_timellap, &days );
	ai_days = (m4dates_t ) days;
	ai_seconds = (m4seconds_t) (seconds * M4SECONDS_DAY + 0.5);

	return M4_SUCCESS;
}

// obtiene de unos dias y segundo un double
m4return_t M4ClTimeStamp::daysec_timellap(const m4dates_t &ai_days, const m4seconds_t &ai_seconds, m4time_t & ao_timellap)
{
	m4time_t secs;

	ao_timellap = (m4time_t) ai_days;
	secs = (m4time_t) ai_seconds;
	
	ao_timellap += (m4time_t) (secs / M4SECONDS_DAY);
	
	if ( ao_timellap < 0)
	{
		ao_timellap = 0;
	}

	return M4_SUCCESS;
}


m4return_t M4ClTimeStamp::set( const M4ClTimeStamp &ai_TS)
{
	m_numdays = ai_TS.m_numdays;
	m_numseconds = ai_TS.m_numseconds;
	return M4_SUCCESS;
}

                                              

  //////////////  operaciones especificas  &}  puntero al objeto
m4return_t M4ClTimeStamp::set (const m4pchar_t ai_pcTS)
{
    if ( M4ClDate::set(ai_pcTS) == M4_SUCCESS )
	{
		return M4ClHour2Second::set(ai_pcTS+11);
	}

	return M4_ERROR;
}


m4return_t M4ClTimeStamp::set (const M4ClString & ai_pcTS)
{
	return set((const m4pchar_t)ai_pcTS.c_str());

/*
	M4ClString TempST;

	TempST = ai_pcTS;

     TempST.erase(TempST.begin(), TempST.begin() +11);
    if ( M4ClDate::set(ai_pcTS) == M4_SUCCESS )
	{
		return M4ClHour2Second::set(TempST);
	}

	return M4_ERROR;
*/
}

m4return_t M4ClTimeStamp::get (M4ClString &ao_pcTS)
{
	m4return_t retcode = M4_SUCCESS;
	M4ClString sDate, sHour;

    if ( M4ClDate::get(sDate) == M4_SUCCESS )
	{
		retcode = M4ClHour2Second::get(sHour);
	}

	if ( retcode == M4_SUCCESS)
	{
		ao_pcTS = sDate;
		ao_pcTS	+= " "; 
		ao_pcTS += sHour;
		return M4_SUCCESS;
	}
	else
	{
	    return M4_ERROR;
	}
}

m4return_t M4ClTimeStamp::set ( const m4int32_t &ai_iyear, const m4int32_t &ai_imon, const m4int32_t &ai_iday, const m4int32_t &ai_ihour, const m4int32_t &ai_imin, const m4int32_t &ai_isec)
{
    if ( M4ClDate::set(ai_iyear, ai_imon, ai_iday) == M4_SUCCESS)
	{
		return M4ClHour2Second::set(ai_ihour, ai_imin, ai_isec);
	}
	return M4_ERROR;
}

m4return_t M4ClTimeStamp::get (  m4int32_t &ai_iyear, m4int32_t &ai_imon, m4int32_t &ai_iday, m4int32_t &ai_ihour, m4int32_t &ai_imin, m4int32_t &ai_isec)
{
    if ( M4ClDate::get(ai_iyear, ai_imon, ai_iday) == M4_SUCCESS)
	{
	   return M4ClHour2Second::get(ai_ihour, ai_imin, ai_isec);
	}

    return M4_ERROR;
}

m4return_t M4ClTimeStamp::set (const m4time_t &ai_TS)
{
    m4seconds_t seconds;
	m4dates_t   days;
        m4int32_t  num_days, num_seconds;

	timellap_daysec( ai_TS, days, seconds);

        /// comprobamos si los segundos tiene mas de un dia
        if (seconds >= 86400) {
            num_days = seconds / 86400;
            num_seconds = seconds % 86400;
            num_days += days;
	}else{
            num_days = days;
            num_seconds = seconds;
	}
                                          
	                                       
	if ( M4ClDate::set(num_days) == M4_SUCCESS)
	{
        return M4ClHour2Second::set( num_seconds);
	}
	
	return M4_ERROR;	

}

m4return_t M4ClTimeStamp::get (m4time_t &ao_TS)
{
    m4seconds_t seconds=0;
	m4dates_t   days=0;

	if ( M4ClDate::get( days) == M4_SUCCESS)
	{
        M4ClHour2Second::get( seconds);
	}
	
	daysec_timellap( days, seconds, ao_TS);
	return M4_SUCCESS;	

}
		
		/////////////////  comparacion

m4bool_t M4ClTimeStamp::operator < (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds < ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays < ai_hora1.m_numdays) ;
}

m4bool_t M4ClTimeStamp::operator > (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds > ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays > ai_hora1.m_numdays) ;
}

m4bool_t M4ClTimeStamp::operator == (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds == ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays == ai_hora1.m_numdays) ;
}

m4bool_t M4ClTimeStamp::operator <= (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds <= ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays <= ai_hora1.m_numdays) ;
}

m4bool_t M4ClTimeStamp::operator >= (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds >= ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays >= ai_hora1.m_numdays) ;
}

m4bool_t M4ClTimeStamp::operator != (const M4ClTimeStamp &ai_hora1)
{
	if ( m_numdays == ai_hora1.m_numdays )
	{
		return M4_BOOL(m_numseconds != ai_hora1.m_numseconds) ;
	}

	return M4_BOOL(m_numdays != ai_hora1.m_numdays) ;
}


m4time_t M4ClTimeStamp::operator + (const m4time_t &ai_hora1)
{
	m4time_t  Temp_TS;
	
	daysec_timellap(m_numdays, m_numseconds, Temp_TS);

	Temp_TS += ai_hora1;

	return Temp_TS; 
}

m4time_t M4ClTimeStamp::operator - (const m4time_t &ai_hora1)
{
	m4time_t  Temp_TS;
	
	daysec_timellap(m_numdays, m_numseconds, Temp_TS);

	Temp_TS -= ai_hora1;

	return Temp_TS; 
}

const m4time_t M4ClTimeStamp::operator += (const m4time_t &ai_hora1)
{
	m4time_t  Temp_TS;
	
	daysec_timellap(m_numdays, m_numseconds, Temp_TS);

	Temp_TS += ai_hora1;
	set(Temp_TS);

	return Temp_TS; 
}

const m4time_t M4ClTimeStamp::operator -= (const m4time_t &ai_hora1)
{
	m4time_t  Temp_TS;
	
	daysec_timellap(m_numdays, m_numseconds, Temp_TS);

	Temp_TS -= ai_hora1;
	set(Temp_TS);

	return Temp_TS; 
}

m4return_t M4ClTimeStamp::add_seconds( const m4int32_t &ai_num_seconds)
{
	m_numseconds += ai_num_seconds;

	add_days ( m_numseconds / 86400);
	m_numseconds %= 86400;

	return M4_SUCCESS;
}

m4return_t M4ClTimeStamp::add_minutes( const m4int32_t &ai_num_minutes)
{
	m_numseconds += ai_num_minutes * M4SECONDS_MINUTE;

	add_days ( m_numseconds / 86400);
	m_numseconds %= 86400;

	return M4_SUCCESS;
}

m4return_t M4ClTimeStamp::add_hours( const m4int32_t &ai_num_hours)
{
	m_numseconds += ai_num_hours * M4SECONDS_HOUR;

	add_days ( m_numseconds / 86400);
	m_numseconds %= 86400;

	return M4_SUCCESS;
}


//----------------------------------------------------------
//*******************M4Clcrono*******************************
//-----------------------------------------------------------

//---------------------------------------------------------
//*****************Constructores M4Clcrono********************
//------------------------------------------------------

M4ClCrono::M4ClCrono(m4bool_t ai_bSelectUn,m4bool_t ai_bSelectModMed)
{
	m_extra = 0;
	m_start=0;
	m_lExtraTimeP=0;
	m_stExtraTimeR.tv_sec=0;
	m_stExtraTimeR.tv_usec=0;
	m_bSelectUn=ai_bSelectUn;
	m_bSelectModMed=ai_bSelectModMed;
	//JRM, quitar warning de acceso a memoria ni inicializada
#ifdef _UNIX
	gettimeofday(&m_stStartTimeR,NULL);
#endif
}

M4ClCrono::M4ClCrono(M4ClCrono &ai_oCrono)
{
	m_start=ai_oCrono.m_start;
	m_extra=ai_oCrono.m_extra;
	m_bSelectUn=ai_oCrono.m_bSelectUn;
	m_bSelectModMed=ai_oCrono.m_bSelectModMed;
}


//---------------------------------------------------------
//********************Start******************************
//-------------------------------------------------------
 
m4double_t 	M4ClCrono::Start()
{
#ifdef _UNIX
     if( m_bSelectModMed==false)
     {
	   gettimeofday(&m_stStartTimeR,NULL);
		
	   if (m_bSelectUn==false)
	   {
		   return ((m_stStartTimeR.tv_sec*1000)+(double)(m_stStartTimeR.tv_usec/1000));
	   }

	   else
	   {

		return ((m_stStartTimeR.tv_sec*1000000)+m_stStartTimeR.tv_usec);
       }
	 }
     else{
       
          m_lStartTimeP=clock();

		  if (m_bSelectUn==false)
		  {
			  return ((double)m_lStartTimeP/1000);
		  }
		  else
		  {
			  return (m_lStartTimeP);
		  }
       }
#else   /// WINDOWS
	 if (m_bSelectModMed==false)
	 {
	 
		m_start = (double) GetTickCount();	

		if (m_bSelectUn==false)
		 {
			 return m_start;
		 }

		 else
		 {

			 return (m_start*1000);
		 }
	 }
		 
	
	else
	{

		 m_start=(double) clock();

		 if (m_bSelectUn==false)
		 {
			 return ((double)(m_start/1000));
		 }
		 else
		 {
			 return m_start;
		 }
	}

	
#endif
}


//----------------------------------------------------------
//**********************Pause*******************************
//----------------------------------------------------------

m4double_t	M4ClCrono::Pause()
{
#ifdef _UNIX

    m4double_t result=0;
    struct timeval stTimeNow ;
    struct timeval temp ;

    if (m_bSelectModMed==false)
      {
	gettimeofday(&stTimeNow,NULL);
	
	if(m_stStartTimeR.tv_usec>stTimeNow.tv_usec)
	  {
	    stTimeNow.tv_usec+=1000000;
	    stTimeNow.tv_sec--;
	  }
	temp.tv_usec=stTimeNow.tv_usec-m_stStartTimeR.tv_usec+m_stExtraTimeR.tv_usec;
	temp.tv_sec=stTimeNow.tv_sec-m_stStartTimeR.tv_sec+m_stExtraTimeR.tv_sec;	
	    
	if( m_bSelectUn==true)

	    return ((temp.tv_sec*1000000)+temp.tv_usec);
	else
	    return((temp.tv_sec*1000)+((double)(temp.tv_usec)/1000));
		   
      }
    else{
      
           result=clock()- m_lStartTimeP+m_lExtraTimeP;
           if( m_bSelectUn==true)
	     
	      return (double)(result);
	   
	   else
	     
	        return ((double)(result)/1000);
	     
        }
	 

#else
	m4double_t result=0;

	if(m_bSelectModMed==false)
	{

		result = (double) GetTickCount() - m_start + m_extra;
	
		if(m_bSelectUn==false)

			return result;
		else

			return (result*1000);
	}

	else
	{

		result=(double) clock()-m_start+m_extra;

		if(m_bSelectUn==TRUE)
		
			return result;

		else

			return (result/1000);
	}
#endif
}


m4double_t	M4ClCrono::Stop()
{
#ifdef _UNIX
     m4double_t result;
     m4int32_t temp;
     struct timeval stTimeNow;
     struct timeval auxtemp;
     struct timeval auxresult;
    

     if (m_bSelectModMed==false)
       {
	 gettimeofday(&stTimeNow,NULL);
	 
	 if(m_stStartTimeR.tv_usec>stTimeNow.tv_usec)
	   {
	     stTimeNow.tv_usec+=1000000;

	     stTimeNow.tv_sec--;
	   }
	 auxtemp.tv_sec=stTimeNow.tv_sec-m_stStartTimeR.tv_sec;
	 auxtemp.tv_usec=stTimeNow.tv_usec-m_stStartTimeR.tv_usec;
	  
	 auxresult.tv_sec=m_stExtraTimeR.tv_sec+auxtemp.tv_sec;
	 auxresult.tv_usec=m_stExtraTimeR.tv_usec+auxtemp.tv_usec;

	 m_stExtraTimeR.tv_sec+=auxtemp.tv_sec;
     m_stExtraTimeR.tv_usec+=auxtemp.tv_usec;

	 gettimeofday(&m_stStartTimeR,NULL);

	 if( m_bSelectUn==true)

	   return ((auxresult.tv_sec*1000000)+auxresult.tv_usec);
	else
	    return ((auxresult.tv_sec*1000)+((double)(auxresult.tv_usec)/1000));
       }
      else
       {
	 temp=clock()-m_lStartTimeP;
	 
	 result=(double)(m_lExtraTimeP+temp);
	 
	 m_lExtraTimeP+=temp;
	 
	 m_lStartTimeP=clock();


	 if( m_bSelectUn==true)
	    
	     return result;
	else
	     return ((double)(result)/1000);
	     
       }

#else

	m4double_t tseguido, result;

	if(m_bSelectModMed==false)
	{

		tseguido = (double) GetTickCount() - m_start;

		result = m_extra + tseguido;

		m_extra += tseguido;
	
		m_start = (double) GetTickCount();

		if(m_bSelectUn==false)

				return result;	
		else

			   return (result*1000);
	}

	else
	{

		tseguido=(double) clock() - m_start;

		result = m_extra + tseguido;

		m_extra+= tseguido;

		m_start=(double) clock();

		if(m_bSelectUn==TRUE)

			return result;

		else

			return (result/1000);
	}
#endif
}

//------------------------------------------------------------
//******************Reset*************************************
//------------------------------------------------------------


m4double_t	M4ClCrono::Reset()
{
#ifdef _UNIX
    m4double_t result=0;
    struct timeval temp;
    struct timeval stTimeNow;
   

    if (m_bSelectModMed==false)
       {
	 gettimeofday(&stTimeNow,NULL);
	 
	 if(m_stStartTimeR.tv_usec>stTimeNow.tv_usec)
	   {
	     stTimeNow.tv_usec+=1000000;

	     stTimeNow.tv_sec--;
	   }
	 if (m_stStartTimeR.tv_usec==0 &&m_stStartTimeR.tv_sec==0)
	   {
	      temp.tv_sec=0;
	      temp.tv_usec=0;
	   }
	 else{

	      temp.tv_sec=m_stExtraTimeR.tv_sec+stTimeNow.tv_sec-m_stStartTimeR.tv_sec;
	      temp.tv_usec=m_stExtraTimeR.tv_usec+stTimeNow.tv_usec-m_stStartTimeR.tv_usec;
	 }
       
	 gettimeofday(&m_stStartTimeR,NULL);
	 
	 m_stExtraTimeR.tv_sec=0;

	 m_stExtraTimeR.tv_usec=0;

	 if( m_bSelectUn==true)

	   return (temp.tv_sec*1000000+temp.tv_usec);
	    
	 else

	   return(temp.tv_sec*1000+((double)(temp.tv_usec)/1000));
	   
       }

     else
       {

	 if(m_lStartTimeP==0)
	    {
		 result=0;
	    }
	 else{
	 	
	 	 result=m_lExtraTimeP+clock()-m_lStartTimeP;
			
	 }
	 
	 m_lStartTimeP=clock();

	 m_lExtraTimeP=0;

	 if( m_bSelectUn==true)
	    
	   return result;
          
	 else
	     return (result/1000);
	 
       
       }

#else

	m4double_t result = 0;

	if(m_bSelectModMed==false)
	{

		if (m_extra==0)

			result=0;
		
		else
		
			result = m_extra + (double) GetTickCount() - m_start;

		m_start = (double) GetTickCount();

		m_extra = 0;
	
		if(m_bSelectUn==false)
		
			return result;	

		else

			return (result*1000);
	}
	else
	{

		if (m_extra==0)

			result=0;

		else

			result=m_extra+(double) clock() - m_start;

		m_start=(double) clock();

		m_extra=0;

		if(m_bSelectUn==TRUE)
		
			return result;

		else

			return (result/1000);
	}
#endif
}

//----------------------------------------------------------

//****************M4ClCronoStatistic*************************

//------------------------------------------------------------


//-------------------------------------------------------
//**************Constructor M4Clcronostatistic***********
//-------------------------------------------------------

M4ClCronoStatistic::M4ClCronoStatistic(m4bool_t ai_bSelectUn,m4bool_t ai_bSelectModMed): M4ClCrono(ai_bSelectUn,ai_bSelectModMed)

{
	m_ipulsaciones = 0;
	m_dttotal = 0;	
}


//----------------------------------------------------------
//****************Destructor ClCrnoStatistic****************
//-----------------------------------------------------------

M4ClCronoStatistic::~M4ClCronoStatistic()
{
}

//-------------------------------------------------------
//**********************Start****************************
//-------------------------------------------------------

void  	M4ClCronoStatistic::Start()
{
	M4ClCrono::Reset();
	m_lastStart.now();
}

//-----------------------------------------------------------
//**********************Pause********************************
//------------------------------------------------------------

m4double_t	M4ClCronoStatistic::Pause()
{
	return  M4ClCrono::Pause();
}

//-----------------------------------------------------------
//***********************Stop**********************************
//-------------------------------------------------------------

m4double_t	M4ClCronoStatistic::Stop()
{
	m4double_t	dret = 0;
	dret = M4ClCrono::Stop();
	
	m_ipulsaciones++;
	m_dttotal += dret;

	return dret;

}

//-------------------------------------------------------------
//*******************GetAverage*********************************
//--------------------------------------------------------------

m4double_t	M4ClCronoStatistic::GetAverage()
{
	return ( m_dttotal / m_ipulsaciones );
}

//-----------------------------------------------------------
//********************GetTotal*********************************
//------------------------------------------------------------

m4double_t	M4ClCronoStatistic::GetTotal()
{
	return m_dttotal;
}

//------------------------------------------------------------
//********************GetLastStartTime**************************
//-----------------------------------------------------------

m4double_t	M4ClCronoStatistic::GetLastStartTime( M4ClString &lst )
{
	if (Pause()==0)

		 return M4_ERROR;

	m_lastStart.get(lst);

	return M4_SUCCESS;
}
