//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                midates.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                19-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
// Propietary:			Manuel Lazcano Perez
// Modifications:
//
// Definition:
//
//    This module defines the  date manipulating functions
//
//
//==============================================================================

#include "mit.h"

#include "mitdefs.cpp"

#include "clstr.h"
#include "m4date.hpp"

#include "mitdates.h"

#define GROUP_ID MIT_TGI_CL_STR

//=========== Constructores y destructor ============================

ClMIT_Dates::ClMIT_Dates()
{ 
}


void ClMIT_Dates::End()
{ 
}



//====================== Metodos ====================================



m4bool_t ClMIT_Dates::IsInfinite(m4date_t ai_dFecha) 
{
	return  M4_BOOL( CmpDate(ai_dFecha,MinusInfiniteDate())==0 || CmpDate(ai_dFecha,PlusInfiniteDate())==0 );
}



m4date_t ClMIT_Dates::NumbersToDate(ClMIT_Dates::stDateData *ai_stData)
{
	m4date_t dDate;
	
	dDate = numdias( ai_stData->iDay, ai_stData->iMonth, ai_stData->iYear ) ;
	dDate += ai_stData->iHour   / M4_HOURS_DAY ;
	dDate += ai_stData->iMinute / M4_MINUTES_DAY ;
	dDate += ai_stData->iSecond / M4_SECONDS_DAY ;

	if ( dDate < 0 ) dDate = MinusInfiniteDate() ;

	return (dDate);
}


void ClMIT_Dates::DateToNumbers(ClMIT_Dates::stDateData *ao_stData, m4date_t ai_dDate)
{

	m4int32_t	iInteger ;
	m4double_t	dRemain ;

	iInteger = m4int32_t( ai_dDate ) ;

	dRemain = ai_dDate - iInteger ;
	ao_stData->iHour = m4int32_t( dRemain * M4_HOURS_DAY ) ;

	dRemain -= ao_stData->iHour / M4_HOURS_DAY ;
	ao_stData->iMinute = m4int32_t( dRemain * M4_MINUTES_DAY ) ;

	dRemain -= ao_stData->iMinute / M4_MINUTES_DAY ;
	ao_stData->iSecond = m4int32_t( dRemain * M4_SECONDS_DAY + 0.5 ) ;

	if ( ao_stData->iSecond == 60 ) {
		ao_stData->iSecond = 0 ;
		ao_stData->iMinute++ ;
		if ( ao_stData->iMinute == 60 ) {
			ao_stData->iMinute = 0 ;
			ao_stData->iHour++ ;
			if ( ao_stData->iHour == 24 ) {
				ao_stData->iHour = 0 ;
				iInteger++ ;
			}
		}
	}
	diamesano( JulianType( iInteger ), &(ao_stData->iDay), &(ao_stData->iMonth), &(ao_stData->iYear) ) ;
}


m4date_t ClMIT_Dates::StringToDate(const m4char_t *ai_pcStr)
{

	ClMIT_Dates::stDateData stData;
	m4date_t dDate;
	ClMIT_Str StrAux;

	StrAux.StrCpy(ai_pcStr);
	if ( ClMIT_Dates::FormateaCadena(&StrAux) ) {

		if( StrAux.StrLen() >= M4_DAY_END ) {

			stData.iYear  = atol( StrAux.InChar(M4_YEAR) ) ;
			stData.iMonth = atol( StrAux.InChar(M4_MONTH) ) ;
			stData.iDay   = atol( StrAux.InChar(M4_DAY) ) ;

			if( StrAux.StrLen() >= M4_HOUR_END ) {

				stData.iHour = atol( StrAux.InChar(M4_HOUR) ) ;
	
				if( StrAux.StrLen() >= M4_MINUTE_END ) {
					
					stData.iMinute = atol( StrAux.InChar(M4_MINUTE) ) ;
					
					if( StrAux.StrLen() >= M4_SECOND_END ) {

						stData.iSecond = atol( StrAux.InChar(M4_SECOND) ) ;
					
					} else {
						stData.iSecond = 0 ;
					}
				
				} else {

					stData.iMinute = 0 ;
					stData.iSecond = 0 ;
				}
			
			} else {
				stData.iHour = 0 ;
				stData.iMinute = 0 ;
				stData.iSecond = 0 ;
			}

	        dDate = NumbersToDate(&stData);

		} else  {
			//Por un por si acaso, si FormateaCadena es OK siempre habra los char necesarios
			
			dDate = MinusInfiniteDate();
		}

	} else {
		//Fallo el FormateaCadena

		//LANZAR ERROR LOCAL
		*GET_INSP()->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163035,
			"ClMIT_Dates::StringToDate, la cadena %0:s no es una fecha en formato ISO", 
			ClMIT_Msg::MIXED_OUT) 
			<< ai_pcStr << SEND_MSG ;
		dDate = MinusInfiniteDate();
	} ;

	return dDate;
}





void ClMIT_Dates::DateToTimeStampStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate)
{
	m4return_t	iResult = M4_SUCCESS;
	ClMIT_Dates::stDateData stData;
	
	ao_poStr->Accommodate(M4_SECOND_END+1);
	DateToNumbers(&stData, ai_dDate);
	
	sprintf(ao_poStr->InChar(0), "%04d-%02d-%02d %02d:%02d:%02d", stData.iYear, stData.iMonth, stData.iDay,
		                                                          stData.iHour, stData.iMinute, stData.iSecond);
		
	ao_poStr->OverStringValid();
}

void ClMIT_Dates::DateToDateStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate)
{
	ClMIT_Str Aux ;

	DateToTimeStampStr(&Aux,ai_dDate) ;
	GetStringDate(ao_poStr,Aux) ;
}

void ClMIT_Dates::DateToTimeStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate)
{
	ClMIT_Str Aux ;

	DateToTimeStampStr(&Aux,ai_dDate) ;
	GetStringTime(ao_poStr,Aux) ;
}


void ClMIT_Dates::DateToFileString(ClMIT_Str *ao_poStr, m4date_t ai_dDate)
{
	m4return_t	iResult = M4_SUCCESS;
	ClMIT_Dates::stDateData stData;
	
	ao_poStr->Accommodate(M4_SECOND_END+1);
	DateToNumbers(&stData, ai_dDate);
	
	sprintf(ao_poStr->InChar(0), "%04d%02d%02d_%02d%02d%02d", stData.iYear, stData.iMonth, stData.iDay,		                                                      stData.iHour, stData.iMinute, stData.iSecond ) ;
		
	ao_poStr->OverStringValid() ;
}

m4date_t ClMIT_Dates::LongsToDate(m4int32_t ai_iDay, m4int32_t ai_iSeconds)
{
	m4date_t dDate;

	dDate = ai_iDay + ai_iSeconds / M4_SECONDS_DAY ;

    return dDate;
}


void ClMIT_Dates::DateToLongs(ClMIT_Dates::stDateData *ao_stData, m4date_t ai_dDate)
{
    ao_stData->iDay = m4uint32_t(ai_dDate);
    ao_stData->iSecond = m4uint32_t((ai_dDate - ao_stData->iDay) * M4_SECONDS_DAY + 0.5);
}


m4date_t ClMIT_Dates::ActualTimeStamp(ClMIT_Str *ao_poStr, ClMIT_Dates::stDateData *ao_stData, m4uint8_t ai_Mode )
{
	m4date_t    dDate;
    time_t      lTime ;
    struct tm   stDate ;
	ClMIT_Dates::stDateData stDateAux ;
    
	if (!ao_stData) ao_stData=&stDateAux;

    time( &lTime ) ;

    switch( ai_Mode )
    {
		case M4CL_TIME_GM_TIME :

            M4ClDate::GmTime(lTime , stDate) ;
            break ;

        case M4CL_TIME_LOCAL_TIME:

            M4ClDate::LocalTime(lTime , stDate) ;
            break ;

        default :

            M4ClDate::GmTime(lTime , stDate) ;
            break ;
    }

	ao_stData->iYear = stDate.tm_year+1900;
	ao_stData->iMonth = stDate.tm_mon + 1;
	ao_stData->iDay = stDate.tm_mday;
	ao_stData->iHour = stDate.tm_hour;
	ao_stData->iMinute = stDate.tm_min;
	ao_stData->iSecond = stDate.tm_sec;
    dDate = NumbersToDate(ao_stData) ;
	DateToLongs(ao_stData, dDate);
	if (ao_poStr) DateToTimeStampStr(ao_poStr, dDate);

	return dDate;
}

m4date_t ClMIT_Dates::ActualDate(ClMIT_Str *ao_poStr, ClMIT_Dates::stDateData *ao_stData, m4uint8_t ai_Mode )
{
	if (ao_poStr)  {

		ClMIT_Str Aux ;
		m4date_t Date;
		
		Date=ActualTimeStamp(&Aux,ao_stData,ai_Mode) ;
		GetStringDate(ao_poStr,Aux) ;
		return  Date ;

	} else {

		return ActualTimeStamp(ao_poStr,ao_stData,ai_Mode) ;
	} ;
}

m4date_t ClMIT_Dates::ActualTime(ClMIT_Str *ao_poStr, ClMIT_Dates::stDateData *ao_stData, m4uint8_t ai_Mode )
{
	if (ao_poStr)  {

		ClMIT_Str Aux ;
		m4date_t Date;
		
		Date=ActualTimeStamp(&Aux,ao_stData,ai_Mode) ;
		GetStringTime(ao_poStr,Aux) ;
		return  Date ;

	} else {

		return ActualTimeStamp(ao_poStr,ao_stData,ai_Mode) ;
	} ;
}



m4date_t ClMIT_Dates::DateRoundToDays(m4date_t ai_dDate)
{
    return(m4int32_t(ai_dDate * 1)) ;
}

m4date_t ClMIT_Dates::DateRoundToHours(m4date_t ai_dDate)
{
    return(m4int32_t(ai_dDate * M4_HOURS_DAY) / M4_HOURS_DAY) ;
}

m4date_t ClMIT_Dates::DateRoundToMinutes(m4date_t ai_dDate)
{
    return(m4int32_t(ai_dDate * M4_MINUTES_DAY) / M4_MINUTES_DAY) ;
}


m4date_t ClMIT_Dates::MinusInfiniteDate( void )
{
	m4date_t dDate;

	dDate = StringToDate(M4CL_MINUS_INFINITE_DATE);
	return dDate;
}


m4date_t ClMIT_Dates::PlusInfiniteDate (void)
{
	m4date_t dDate;

	dDate = StringToDate(M4CL_PLUS_INFINITE_DATE);
	return dDate;
}


void ClMIT_Dates::GetStringDate(ClMIT_Str *ao_poStr, const m4char_t *ai_pcStr)
{
	ao_poStr->StrCpy(ai_pcStr);
	
	// Bug 0312520. Insertar solo en el caso de que la longitud sea mayor de GetStrDateLength (10).
	if (strlen(ai_pcStr) >= GetStrDateLength())
	{
		ao_poStr->StrInsertChar(GetStrDateLength(), 1, M4_END_STR);
	}
}


void ClMIT_Dates::GetStringTime(ClMIT_Str *ao_poStr, const m4char_t *ai_pcStr)
{
	ClMIT_Str StrAux;
	StrAux.StrCpy(ai_pcStr);

	ao_poStr->StrCpy(StrAux.InChar(11));
}




//======== Convierte una Cadena a formato TS ==========================

#define M4_DATE_DIGITS "0123456789"

m4bool_t ClMIT_Dates::FormateaCadena(ClMIT_Str *ao_poStr)
{
	m4bool_t SwOk=M4_TRUE ;
	m4char_t * inPos ;
	size_t len ;

	ClMIT_Str digits, targetStr ;

	targetStr.StrCpy("") ;
	
	//Año 
	inPos = ao_poStr->InChar(0) ;
	len = strspn(inPos,M4_DATE_DIGITS) ;
	switch ( len ) {
	case 4:
		targetStr.ToASCIZ(inPos,len) ;  //Copia el año tal cual
		break ;
	case 2:
		digits.ToASCIZ(inPos,len) ;  //Mete 20aa o 19aa segun aa<30 o no
		targetStr.ToASCIZ( atoi(digits.InChar(0)) < 30 ? "20" : "19", 2 ) ;
		targetStr.StrCat(digits) ;
		break ;
	default:
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//Copia el separador de año-mes
	inPos+=len ;
	targetStr.StrCatChar( *inPos ) ;
	++inPos ;

	//Meses
	len = strspn(inPos,M4_DATE_DIGITS) ;
	switch ( len ) {
	case 2:
		digits.ToASCIZ(inPos,len) ;
		targetStr.StrCat(digits.InChar(0)) ;  //Copia el mes tal cual
		break ;
	case 1:
		digits.ToASCIZ(inPos,len) ;  //Inserta un 0
		targetStr.StrCatChar('0') ;
		targetStr.StrCat(digits) ;
		break ;
	default:
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	if ( ! M4_IN_RANGE( atol(digits.InChar(0)), 1, 12 ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Copia el separador de mes-dia
	inPos+=len ;
	targetStr.StrCatChar( *inPos ) ;
	++inPos ;

	//Dias
	len = strspn(inPos,M4_DATE_DIGITS) ;
	switch ( len ) {
	case 2:
		digits.ToASCIZ(inPos,len) ;
		targetStr.StrCat(digits.InChar(0)) ;  //Copia el dia tal cual
		break ;
	case 1:
		digits.ToASCIZ(inPos,len) ;  //Inserta un 0
		targetStr.StrCatChar('0') ;
		targetStr.StrCat(digits) ;
		break ;
	default:
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	if ( ! M4_IN_RANGE( atol(digits.InChar(0)), 1, 31 ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Copia el resto de la cadena

	inPos+=len ;
	targetStr.StrCat( inPos ) ;

exit:
	if (SwOk) {
		ao_poStr->StrCpy(targetStr.InChar(0)) ;
	} ;
	return SwOk ;
}

//====================== metodos privados ==================================
//=========== para calculo y manipulacion de fechas=========================



#define GREGORIO_ANO         1752L           /* año del ajuste gregoriano */
#define GREGORIO_MES            9L           /* mes del ajuste gregoriano */
#define GREGORIO_DIA           20L           /* dia del ajuste gregoriano */
#define GREGORIO_DIAS          11L           /* dias a restar de la fecha */
#define GREGORIO_NUM_DIAS  639792L           /* dias hasta el 20-09-92    */


        /************************************************/
        /****************** diasem  *********************/
        /************************************************/
        /* calcula el d¡a de la semana a partir de una  */
        /* fecha en formato d¡as desde 31-12-0. Devuelve*/
        /* 1-Lunes, 2-Martes, 3-Miercoles, .., 7-Domingo*/
        /************************************************/

m4int32_t ClMIT_Dates::ndiasem (JulianType dias )
{
        m4int32_t diasem;
        diasem = (m4int32_t)(((m4int32_t)dias % 7) + 4);
        diasem = diasem > 7 ? diasem - 7 : diasem;
        return(diasem);
}

        /************************************************/
        /********************* hoy **********************/
        /************************************************/
        /* Devuelve el n£mero de d¡as desde el          */
        /* 31-12-0000 hasta la fecha del sistema        */
        /************************************************/

// !!CambioI                                      
// Aquí se usuaba la librería de control de fechas de Borland
// La he cambiado por la de Microsoft (Añadiendo el header <time.h>
// Ojo: El mes pertenece a [0,11] ¡¡ Microsoft !!        
JulianType ClMIT_Dates::nhoy (void)
{
	struct tm pfecha ;
    time_t ltime;
	JulianType R ;
                        
    // Devuelve segundos desde 1/1/1970 hasta la fecha del sistema
    time( &ltime );                    

    // Mete ese número en una estructura legible (que representa el día del sistema)
	M4ClDate::GmTime( ltime,pfecha);                  
	R=numdias (pfecha.tm_mday, pfecha.tm_mon + 1, 1900 + pfecha.tm_year);
    
    return R ;  
}           
//!!CambioF

        /************************************************/
        /****************** diascomun *******************/
        /************************************************/
        /* Devuelve el n£mero de d¡as comunes a dos     */
        /* intervalos de fechas                         */
        /* si cualquiera de las fechas finales es cero  */
        /* se hace igual a la fecha final del otro      */
        /* intervalo.                                   */
        /************************************************/

JulianType ClMIT_Dates::ndiascomun   (JulianType fini1, JulianType ffin1, JulianType fini2, JulianType ffin2)
{
        JulianType result, auxini, auxfin;
        if (ffin1 == 0)
            ffin1 = ffin2;
        if (ffin2 == 0)
            ffin2 = ffin1;

        if (fini1 > fini2)      // toma la mayor de las iniciales
            auxini = fini1;
        else
            auxini = fini2;
        if (ffin1 < ffin2)      // toma la menor de las finales
            auxfin = ffin1;
        else
            auxfin = ffin2;

        result = auxfin - auxini + 1;
        if (result < 0)
            result = 0;
        return(result);
}

        /************************************************/
        /****************** sumaannos *******************/
        /************************************************/
        /* suma a¤os a una fecha                        */
        /* corrige fechas inexistentes                  */
        /************************************************/

JulianType ClMIT_Dates::nsumaannos   (JulianType dias,  m4int32_t annos)
{
        m4int32_t dia, mes, ano;
        JulianType result;
        m4int32_t annofin, maxdia;

        diamesano (dias , &dia, &mes, &ano);
        annofin = ano + annos;

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
        result = numdias (dia, mes, annofin);
        return(result);
}

        /************************************************/
        /****************** sumameses *******************/
        /************************************************/
        /* suma meses a una fecha                       */
        /* corrige fechas inexistentes                  */
        /************************************************/



JulianType ClMIT_Dates::nsumameses   (JulianType dias,  m4int32_t meses)
{
        m4int32_t dia, mes, ano, maxdia;
        JulianType result;
        m4int32_t mesfin;
        /* tabla de d¡as de cada mes. Febrero se modifica. */
        static const m4int32_t diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        diamesano (dias , &dia, &mes, &ano);

        mesfin = mes + meses;

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
        result = numdias (dia, mes, ano);
        return(result);
}

        /************************************************/
        /****************** DIASDELMES ******************/
        /************************************************/
        /* calcula los d¡as del mes de                  */
        /* un mes y un a¤o                              */
        /************************************************/

m4int32_t ClMIT_Dates::ndiasdelmes   (m4int32_t mes, m4int32_t ano)
{
        m4int32_t result;
        /* tabla de d¡as de cada mes. Febrero se modifica. */
        static const m4int32_t diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        if (mes < 1)  mes = 1;
        if (mes > 12) mes = 12;

        if (mes != 2)
        {
            result = diasmesTab[mes-1];
        }
        else
        {
            if ((ano % 4  ) == 0)
                result = 29;            /* Los m£ltiplos de 4 son bisiestos */
            else
                result = 28;
            if ((ano % 100) == 0)
                result = 28;            /* L¢s multiplos de 100 no */
            if ((ano % 400) == 0)
                result = 29;            /* L¢s multiplos de 400 si */
        }
        return(result);
}

        /************************************************/
        /****************** DIASDELMES ******************/
        /************************************************/
        /* calcula los d¡as del mes de                  */
        /* una fecha                                    */
        /************************************************/

m4int32_t ClMIT_Dates::ndiasdelmes   (JulianType dias)
{
        m4int32_t ano;
        m4int32_t mes;
        m4int32_t dia;

        diamesano (dias , &dia, &mes, &ano);

        return(ndiasdelmes(mes, ano));
}

        /************************************************/
        /****************** DIASDELANO ******************/
        /************************************************/
        /* calcula los d¡as del a¤o del a¤o             */
        /* correspondiente                              */
        /************************************************/

m4int32_t ClMIT_Dates::ndiasdelano   (m4int32_t ano)
{
        m4int32_t result;

        if ((ano % 4  ) == 0)
            result = 366;            /* Los m£ltiplos de 4 son bisiestos */
        else
            result = 365;
        if ((ano % 100) == 0)
            result = 365;            /* L¢s multiplos de 100 no */
        if ((ano % 400) == 0)
            result = 366;            /* L¢s multiplos de 400 si */

        return(result);
}

        /************************************************/
        /****************** annos   *********************/
        /************************************************/
        /* calcula el n£mero de a¤os completos entre    */
        /* fecha inicial y fecha final                  */
        /************************************************/

m4int32_t ClMIT_Dates::nannos   (JulianType dias1 , JulianType dias2)
{
        m4int32_t dia1,mes1,ano1;
        m4int32_t dia2,mes2,ano2;
        m4int32_t restaano = 0;
        diamesano (dias1 , &dia1 , &mes1 , &ano1);
        diamesano (dias2 , &dia2 , &mes2 , &ano2);
        if ((mes1 * 100 + dia1) > (mes2 * 100 + dia2))
            restaano = 1;
        return(ano2 - ano1 - restaano);
}

        /************************************************/
        /****************** meses   *********************/
        /************************************************/
        /* calcula el n£mero de meses completos entre   */
        /* fecha inicial y fecha final                  */
        /************************************************/

JulianType ClMIT_Dates::nmeses   (JulianType dias1 , JulianType dias2)
{
        m4int32_t dia1,mes1,ano1;
        m4int32_t dia2,mes2,ano2;
        m4int32_t restames = 0;
        diamesano (dias1 , &dia1 , &mes1 , &ano1);
        diamesano (dias2 , &dia2 , &mes2 , &ano2);
        if (dia1 > dia2)
            restames = 1;
        return(((JulianType)(ano2 - ano1)) * 12 + mes2 - mes1 - restames);
}

        /************************************************/
        /****************** fecha30 *********************/
        /************************************************/
        /* calcula la distancia entre dia mes y a¤o y   */
        /* el 31 - 12 - 0000 suponiendo que todos los   */
        /* meses son de treinta d¡as                    */
        /************************************************/

JulianType ClMIT_Dates::nfecha30 ( m4int32_t dia, m4int32_t mes , m4int32_t ano )
{
        JulianType r1;
        r1 = ((JulianType) ano) * 360 + mes * 30 + ((dia <= 30) ? dia : 30);
        return(r1);
}

        /************************************************/
        /****************** fecha30 *********************/
        /************************************************/
        /* pasa n£mero de d¡as desde 31-12-0000 a n£mero*/
        /* de d¡as desde 31-12-0000 suponiendo que todos*/
        /* los meses son de treinta d¡as                */
        /************************************************/

JulianType ClMIT_Dates::nfecha30 ( JulianType dias )
{
        JulianType r1;
        m4int32_t dia,mes,ano;
        if (dias)       // si los d¡as son cero, la fecha es cero
        {
            diamesano (dias , &dia , &mes , &ano);
            r1 = ((JulianType) ano) * 360 + mes * 30 + ((dia <= 30) ? dia : 30);
        }
        else
        {
            r1 = 0;
        }
        return(r1);
}

        /************************************************/
        /******************   ano   *********************/
        /************************************************/
        /* calcula el a¤o a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/

m4int32_t ClMIT_Dates::nano (JulianType dias )
{
        m4int32_t pp1,pp2;
        m4int32_t ano;
        diamesano (dias , &pp1 , &pp2 , &ano);
        return(ano);
}

        /************************************************/
        /******************   mes   *********************/
        /************************************************/
        /* calcula el mes a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/

m4int32_t ClMIT_Dates::nmes (JulianType dias )
{
        m4int32_t pp1,pp2;
        m4int32_t mes;
        diamesano (dias , &pp1 , &mes , &pp2);
        return(mes);
}


        /************************************************/
        /******************   dia   *********************/
        /************************************************/
        /* calcula el dia a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/


m4int32_t ClMIT_Dates::ndia (JulianType dias )
{
        m4int32_t pp1,pp2;
        m4int32_t dia;
        diamesano (dias , &dia , &pp1 , &pp2);
        return(dia);
}

        /************************************************/
        /****************** diamesano *******************/
        /************************************************/
        /* interna calcula dia, mes a¤o a partir del n§ */
        /* de d¡as desde 31-12-0000                     */
        /************************************************/

void ClMIT_Dates::diamesano (JulianType dias , m4int32_t * dia , m4int32_t * mes , m4int32_t *ano)
{
        /* tabla de d¡as de cada mes. Febrero se modifica. */
		//NO puede ser estatica, o no sera multithread. No puede ser const, se modifica.
        m4int32_t diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        JulianType diascomp;
        m4int32_t diadelano;
        m4int32_t mes1  = 0;
        m4int32_t *mptr = diasmesTab;
        JulianType diasg;             /* con la correcci¢n de gregorio en 1752 */
        *ano  = (m4int32_t(((dias > GREGORIO_NUM_DIAS ? dias + GREGORIO_DIAS : dias)/365.2425)+.002))+1;
        diascomp = numdias( 1 , 1 , *ano );
        if (diascomp > dias ) (*ano)--;
            diasg = ((*ano == GREGORIO_ANO) && (dias > GREGORIO_NUM_DIAS)) ? dias + GREGORIO_DIAS : dias;

        diadelano = m4int32_t (diasg - (numdias( 1 , 1 , *ano )));
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
        return;
  }



        /************************************************/
        /****************** numdias *********************/
        /************************************************/
        /*         Calcula n§ de d¡as desde 31-12-0000  */
        /* a partir de dia, mes, a¤o. Tiene en cuenta   */
        /* la correcci¢n gregoriana de 1752 (Faltan 9   */
        /* d¡as en Septiembre)                          */
        /************************************************/

JulianType ClMIT_Dates::numdias (m4int32_t dia, m4int32_t mes, m4int32_t anno)
{

        JulianType result;
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





//=================== Funciones externas ===============================


m4int32_t ClMIT_Dates::CmpDate( m4date_t ai_dFecha1,m4date_t ai_dFecha2 )
{
	ClMIT_Dates::stDateData TSDataA, TSDataB  ;
	m4int32_t R ;

	DateToNumbers(&TSDataA, ai_dFecha1) ;
	DateToNumbers(&TSDataB, ai_dFecha2) ;
	
	if ( TSDataA.iYear > TSDataB.iYear ) {
		R = 1 ;
	} else if ( TSDataA.iYear < TSDataB.iYear ) {
		R = -1 ;
	} else {
		if ( TSDataA.iMonth > TSDataB.iMonth ) {
			R = 1 ;
		} else if ( TSDataA.iMonth < TSDataB.iMonth ) {
			R = -1 ;
		} else {
			if ( TSDataA.iDay > TSDataB.iDay ) {
				R = 1 ;
			} else if ( TSDataA.iDay < TSDataB.iDay ) {
				R = -1 ;
			} else {
				R = 0 ;
			} ;
		} ;
	} ;
	return R ;
}


m4int32_t ClMIT_Dates::CmpTimeStamp( m4date_t ai_dFecha1,m4date_t ai_dFecha2 ) 
{
	ClMIT_Dates::stDateData TSDataA, TSDataB  ;
	m4int32_t R ;

	DateToNumbers(&TSDataA, ai_dFecha1) ;
	DateToNumbers(&TSDataB, ai_dFecha2) ;
	
	if ( TSDataA.iYear > TSDataB.iYear ) {
		R = 1 ;
	} else if ( TSDataA.iYear < TSDataB.iYear ) {
		R = -1 ;
	} else {
		if ( TSDataA.iMonth > TSDataB.iMonth ) {
			R = 1 ;
		} else if ( TSDataA.iMonth < TSDataB.iMonth ) {
			R = -1 ;
		} else {
			if ( TSDataA.iDay > TSDataB.iDay ) {
				R = 1 ;
			} else if ( TSDataA.iDay < TSDataB.iDay ) {
				R = -1 ;
			} else {
				if ( TSDataA.iHour > TSDataB.iHour ) {
					R = 1 ;
				} else if ( TSDataA.iHour < TSDataB.iHour ) {
					R = -1 ;
				} else {
					if ( TSDataA.iMinute > TSDataB.iMinute ) {
						R = 1 ;
					} else if ( TSDataA.iMinute < TSDataB.iMinute ) {
						R = -1 ;
					} else {
						if ( TSDataA.iSecond > TSDataB.iSecond ) {
							R = 1 ;
						} else if ( TSDataA.iSecond < TSDataB.iSecond ) {
							R = -1 ;
						} else {
							R = 0 ;
						} ;
					} ;
				} ;
			} ;
		} ;
	} ;
	return R ;
}


