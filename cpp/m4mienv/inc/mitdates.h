//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                midates.hpp   
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
//    This module defines the prototypes for date manipulating functions
//
//
//==============================================================================


#ifndef __MITDATES_HPP__
#define __MITDATES_HPP__

#include "cldefine.hpp"
#include "fechas.h"

class ClMIT_Str ;

typedef double JulianType;

//Cualquier metodo que tenga problemas al convertir fechas, pondra la fecha a - infinito.

class M4_DECL_M4MIENV ClMIT_Dates
{
	MIT_PUBLIC:

			//Constructores
		ClMIT_Dates();

			//Destructores
		virtual ~ClMIT_Dates() { ClMIT_Dates::End(); }

		virtual void End();

		    //Estructura que guardara valores enteros de fecha desglosada

		struct stDateData
		{
			m4int32_t	iYear;
			m4int32_t	iMonth;
			m4int32_t	iDay;
			m4int32_t	iHour;
		 	m4int32_t	iMinute;
			m4int32_t	iSecond;
		} ;

		//Length de string fecha, (cadena obtenible con DateToDateStr)
		//Formato ISO : aaaa-mm-dd 

		static m4int16_t GetStrDateLength() { return 10 ; }

		//Length de string fecha, (cadena obtenible con DateToTimeStampStr)
		//Formato ISO : aaaa-mm-dd hh:mm:ss 

		static m4int16_t GetStrTimeStampLength() { return 19 ; }

		// Convierte una serie de números que representan una fecha a esa fecha numérica
		static m4date_t NumbersToDate(ClMIT_Dates::stDateData *ai_stData);


		//Convierte una fecha numérica a una serie de números que representan esa fecha
		//  Le debemos pasar como primer parametro una estructura de tipo stDateData que
		//  almacenara los valores numericos de la fecha desglosada
		static void DateToNumbers(ClMIT_Dates::stDateData *ao_stData, m4date_t ai_dDate);


		// Convierte una cadena en formato TS a una fecha numérica
		// Si el formato es incorrecto, se lanzara un error local y se devolvera 
		//	la fecha menos infinito
		static m4date_t StringToDate(const m4char_t *ai_pcStr) ;
		
		// Normaliza la cadena de fecha o TS.
		// Solo normaliza la parte de fecha, que debe ser la inicial de la cadena. 
		// Como formato de fecha de entrada se admite solo ISO, de esta forma:
		//		(aaaa/aa)?(mm/m)?(dd/d) ....
		//	? son separadores, cualquier caracter que no sea un digito. La parte de
		//		año admite numeros de 4 o 2 digitos. Si es de 2, una valor <30 se tomara
		//		como del 20xx, y uno mayor como 19xx
		//	Para mm y dd se admiten numeros de 1 o 2 digitos, si es necesario se insertara un 0.
		// Como formato de fecha de salida normalizada tendremos:
		//		aaaa?mm?dd ....
		//	Donde .... es el resto de la cadena.
		// El mes debe estar entre 1 y 12, y el dia entre 1 y 31 (no chequea dias segun el mes)
		// Si el formato de entrada es erroneo, se devolverá FALSE y se dejará la cadena
		//	tal cual, si  tocar.

		static  m4bool_t FormateaCadena(ClMIT_Str *ao_poStr);

		// Convierte una fecha numérica en una cadena en formato TS
		// Si la cadena es más pequeña del formato, se trunca
		static void DateToTimeStampStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate);
		static void DateToDateStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate);
		static void DateToTimeStr(ClMIT_Str *ao_poStr, m4date_t ai_dDate);

		//Como la anterior, pero la cadena es valida como parte de un nombre de fichero
		static void DateToFileString(ClMIT_Str *ao_poStr, m4date_t ai_dDate);

	    //Convierte dos longs con los dias y segundos a una fecha numérica
		static m4date_t LongsToDate(m4int32_t ai_iDay, m4int32_t ai_iSeconds);
		//Obtiene los dias y segundos de una fecha de tipo date. Le debemos pasar un
		// puntero a una stDateData para que escriba ahi los valores de vuelta
		static void DateToLongs(ClMIT_Dates::stDateData *ao_stData, m4date_t ai_dDate);

		//Devuelve la fecha actual en m4date_t. Ademas le pasamos como parametros un
		// puntero a un ClMIT_Str, donde nos dejara una cadena formateada con el timestamp y tambien
		// le pasamos un puntero a una stDateData, donde nos dejara los valores desglosados 
		// de la fecha.
		//Si ao_poStr o ao_stData es NULL, no actualiza el parametro en cuestion.
		static m4date_t ActualTimeStamp( ClMIT_Str* ao_poStr=NULL, 
			ClMIT_Dates::stDateData *ao_stData=NULL, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME) ;

		//Exactamente iguales a ActualTimeStamp, solo varia la cadena ao_poStr.
		static m4date_t ActualDate( ClMIT_Str* ao_poStr=NULL, 
			ClMIT_Dates::stDateData *ao_stData=NULL, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME) ;
		static m4date_t ActualTime( ClMIT_Str* ao_poStr=NULL, 
			ClMIT_Dates::stDateData *ao_stData=NULL, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME) ;

		// Devuelve fecha redondeada
		static m4date_t DateRoundToDays(m4date_t ai_dDate);
		static m4date_t DateRoundToHours(m4date_t ai_dDate);
		static m4date_t DateRoundToMinutes(m4date_t ai_dDate);


		// Devuelve fecha -,+INF
		static m4date_t MinusInfiniteDate ( );
		static m4date_t PlusInfiniteDate ( );

		//Le pasamos una cadena con un TimeStamp y devuelve una cadena que contiene
		// solamente la fecha. Le debemos pasar un puntero a un ClMIT_Str de salida.
		static void GetStringDate(ClMIT_Str *ao_poStr, const m4char_t *ai_pcStr);
		//Le pasamos una cadena con un TimeStamp y devuelve una cadena que contiene
		// solamente la hora. Le debemos pasar un puntero a un ClMIT_Str de salida.
		static void GetStringTime(ClMIT_Str *ao_poStr, const m4char_t *ai_pcStr);

		//Compara fechas (si lo que se pasa es un TS, da igual, solo usa la fecha). Devuelve:
		//		0 -> Fecha1 = Fecha2
		//		1 -> Fecha1 > Fecha2
		//	   -1 -> Fecha1 < Fecha2	
		static m4int32_t CmpDate( m4date_t ai_dFecha1,m4date_t ai_dFecha2 );

		//Compara TimeStamp. Devuelve:
		//		0 -> Fecha1 = Fecha2
		//		1 -> Fecha1 > Fecha2
		//	   -1 -> Fecha1 < Fecha2	
		static m4int32_t CmpTimeStamp( m4date_t ai_dFecha1,m4date_t ai_dFecha2 );

		//Da true si la fecha ó TS es + o - infinito.

		static m4bool_t IsInfinite(m4date_t ai_dFecha) ;

MIT_PRIVATE:

		static JulianType numdias (m4int32_t dia, m4int32_t mes, m4int32_t anno);
		static m4int32_t nano (JulianType dias );
		static m4int32_t nmes (JulianType dias );
		static m4int32_t ndia (JulianType dias );
		static m4int32_t ndiasem (JulianType dias );
		static void diamesano (JulianType dias , m4int32_t* dia , m4int32_t* mes , m4int32_t* ano );
		static JulianType nfecha30 ( m4int32_t dia, m4int32_t mes , m4int32_t ano );
		static JulianType nfecha30 ( JulianType dias );
		static m4int32_t nannos   (JulianType dias1 , JulianType dias2);
		static JulianType nmeses     (JulianType dias1 , JulianType dias2);
		static JulianType nsumaannos (JulianType dias, m4int32_t annos);
		static JulianType nsumameses (JulianType dias, m4int32_t meses);
		static JulianType ndiascomun   (JulianType fini1, JulianType ffin1, JulianType fini2, JulianType ffin2);
		static JulianType nhoy (void);
		static m4int32_t ndiasdelmes   (m4int32_t mes, m4int32_t ano);
		static m4int32_t ndiasdelmes   (JulianType dias);
		static m4int32_t ndiasdelano   (m4int32_t ano);

};



#endif

