//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             RCTABLES.H
// Project:			 M4FMT32.DLL	
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the macros for the error codes
//
//
//==============================================================================

#ifndef _RCTABLES_H
#define _RCTABLES_H

#include "m4types.hpp"

//------------------------------------------------------------->Originalmente en constant.h

#define HANDLE_ISO       1
#define HANDLE_WINDOWS   2

#define TIPO_SQL 0
#define TIPO_M4  1


#define TIPOALFA         0    
#define TIPOFECHA        1
#define TIPONUMERO       2 
#define M4ISOPOSDAY      8   // YYYY-MM-DD HH:MM:SS   <- ISO FORMAT

#define TIPO_FLOAT       3
#define TIPO_TIME        4
#define TIPO_BINARY      5
#define TIPO_BIT         6
#define TIPO_ERROR       7

// CODIGOS ASCCII
/*
   241 'ñ'
   209 'Ñ'
   225 'á'
   233 'é'
   237 'í'
   243 'ó'
   250 'ú'
   193 'Á'
   201 'É'
   205 'Í'
   211 'Ó'
   218 'Ú'
*/

#define ALIG_DER         0
#define ALIG_CEN         1
#define ALIG_IZQ         2

//

#define FLAG_IBEGINSTR           1
#define FLAG_IACENTLESS          2
#define FLAG_ISUBST              4
#define FLAG_ITRUNC              8
#define FLAG_ILEADZERO          16
#define FLAG_ISTYLE             32
#define FLAG_ICUR               64
#define FLAG_ILEFTZERO         128
#define FLAG_ICAPAR            256

//

#define ANUMEROS_DOS     2   // yy    = año con 2 cifras
#define ANUMEROS_CUATRO  4   // yyyy  = año con 4 cifras
#define ANUMEROS_ALETRAS 5   // yyyyy = año en letras

#define MNUMEROS_UNO     1   // m     = mes con 1 cifra
#define MNUMEROS_DOS     2   // mm    = mes con 2 cifras
#define MLETRAS_TRES     3   // mmm   = mes con 3 letras
#define MLETRAS_TODAS    4   // mmmm  = mes con todas las letras

#define WNUMEROS_UNO     1   // w     = semana con 1 cifra
#define WLETRAS_TRES     2   // ww   = semana con 3 letras
#define WLETRAS_TODAS    3   // www  = semana con todas las letras

#define DNUMEROS_UNO     1   // d     = dia con 1 cifra
#define DNUMEROS_DOS     2   // dd    = dia con 2 cifras
#define DNUMEROS_ALETRAS 3   // ddd   = dia con 3 letras

#define HNUMEROS_UNO     1   // h     = horas con 1 cifra
#define HNUMEROS_DOS     2   // hh    = horas con 2 cifras

#define NNUMEROS_UNO     1   // n     = minutos con 1 cifra
#define NNUMEROS_DOS     2   // nn    = minutos con 2 cifras

#define SNUMEROS_UNO     1   // s     = segundos con 1 cifra
#define SNUMEROS_DOS     2   // ss    = segundos con 2 cifras

//------------------------------------------------------------->Originalmente en error.h

#define EXITO                      1

#define ERROR_NOMEMORY            -1000
#define IDS_NOMEMORY               1000

#define ERROR_INVALIDHANDLE       -1001
#define IDS_INVALIDHANDLE          1001

#define ERROR_NOPERMISSION        -1002
#define IDS_NOPERMISSION           1002

#define ERROR_TIPOSQL             -2000   // Tipo SQL erroneo
#define IDS_TIPOSQL                2000

#define ERROR_FORMATONOCREADO     -2001   // El formato no ha sido creado
#define IDS_FORMATONOCREADO        2001

#define ERROR_IMPOSIBLETRANSF     -2003   // No se puede convertir la cadena de entrada al formato de salida
#define IDS_IMPOSIBLETRANSF        2003

#define ERROR_FORMATOENTRADA      -2004   // La cadena de entrada no está formateada con el formato de entrada
#define IDS_FORMATOENTRADA         2004

#define ERROR_PROPIEDADNOE        -2005   // La propiedad especificada no existe
#define IDS_PROPIEDADNOE           2005

#define ERROR_IDIOMANOSOPORTADO   -2006   // El idioma pedido no esta soportado
#define IDS_IDIOMANOSOPORTADO      2006

#define ERROR_VALORNOCORRECTO     -2007   // El valor que se ha intentado asignar a la propiedad no es correcto
#define IDS_VALORNOCORRECTO        2007

#define ERROR_FORMATOSDIFERENTES  -2008   // Error interno: se ha intentado la copia entre formatos de diferentes idiomas
#define IDS_FORMATOSDIFERENTES     2008

#define ERROR_FORMATOFUENTENULO   -2009   // Se ha intentado copiar de un formato que no existe
#define IDS_FORMATOFUENTENULO      2009

#define ERROR_DESFORMONEDA        -2010   // La cadena que se supone es la moneda no coincide
#define IDS_DESFORMONEDA           2010

#define ERROR_DESFORANIO          -2011   // Año formateado de una forma no soportada
#define IDS_DESFORANIO             2011

#define ERROR_DESFORMES           -2012   // Mes formateado de una forma no soportada
#define IDS_DESFORMES              2012

#define ERROR_DESFORMES1          -2013   // La cadena del mes no es un nombre de mes valido
#define IDS_DESFORMES1             2013

#define ERROR_DESFORSEM           -2014   // Dia de la semana formateado de una forma no soportada
#define IDS_DESFORSEM              2014

#define ERROR_DESFORSEM1          -2015   // La cadena del dia-semana no es un nombre de un día de la semana valido
#define IDS_DESFORSEM1             2015

#define ERROR_DESFORDIA           -2016   // Dia formateado de una forma no soportada
#define IDS_DESFORDIA              2016

#define ERROR_DESFORHORA          -2017   // Hora formateada de una forma no soportada
#define IDS_DESFORHORA             2017

#define ERROR_DESFORMIN           -2018   // Minuto formateado de una forma no soportada
#define IDS_DESFORMIN              2018

#define ERROR_DESFORSEG           -2019   // Segundo formateado de una forma no soportada
#define IDS_DESFORSEG              2019

#define ERROR_DESFORTIME          -2020   // Error general al desformatear un TIME
#define IDS_DESFORTIME             2020

#define ERROR_NOENOMBRE           -2021   // No existe un handle con ese nombre
#define IDS_NOENOMBRE              2021

#define ERROR_ERRORNOEXISTE       -2022   // Error desconocido
#define IDS_ERRORNOEXISTE          2022

#define ERROR_MALANIO             -2023   // Valor del año incorrecto
#define IDS_MALANIO                2023

#define ERROR_MALMES              -2024   // Valor del mes incorrecto
#define IDS_MALMES                 2024

#define ERROR_MALDIASEMANA        -2025   // Valor del dia de la semana incorrecto
#define IDS_MALDIASEMANA           2025

#define ERROR_MALDIA              -2026   // Valor del dia incorrecto
#define IDS_MALDIA                 2026

#define ERROR_MALHORA             -2027   // Valor de la hora incorrecto
#define IDS_MALHORA                2027

#define ERROR_MALMINUTO           -2028   // Valor de los minutos incorrecto
#define IDS_MALMINUTO              2028

#define ERROR_MALSEGUNDO          -2029   // Valor de los segundos incorrecto
#define IDS_MALSEGUNDO             2029

#define ERROR_MALNUMERO           -2030   // Número incorrecto
#define IDS_MALNUMERO              2030

#define ERROR_MALTIPO             -2031   // Tipo erroneo
#define IDS_MALTIPO                2031

//------------------------------------------------------------->Originalmente en resources.h

#define IDS_NOMBRE_MONEDA                10

#define IDS_MESES                        11
   #define IDS_ENERO                     (IDS_MESES+0)
   #define IDS_FEBRERO                   (IDS_MESES+1)
   #define IDS_MARZO                     (IDS_MESES+2)
   #define IDS_ABRIL                     (IDS_MESES+3)
   #define IDS_MAYO                      (IDS_MESES+4)
   #define IDS_JUNIO                     (IDS_MESES+5)
   #define IDS_JULIO                     (IDS_MESES+6)
   #define IDS_AGOSTO                    (IDS_MESES+7)
   #define IDS_SEPTIEMBRE                (IDS_MESES+8)
   #define IDS_OCTUBRE                   (IDS_MESES+9)
   #define IDS_NOVIEMBRE                 (IDS_MESES+10)
   #define IDS_DICIEMBRE                 (IDS_MESES+11)  
   
#define IDS_DIASEMANA                    (IDS_DICIEMBRE+1)
   #define IDS_LUNES                     (IDS_DIASEMANA+0)
   #define IDS_MARTES                    (IDS_DIASEMANA+1)
   #define IDS_MIERCOLES                 (IDS_DIASEMANA+2)
   #define IDS_JUEVES                    (IDS_DIASEMANA+3)
   #define IDS_VIERNES                   (IDS_DIASEMANA+4)
   #define IDS_SABADO                    (IDS_DIASEMANA+5)
   #define IDS_DOMINGO                   (IDS_DIASEMANA+6)

#define IDS_MESESLIGHT                   (IDS_DOMINGO+1)
   #define IDS_ENEROLIGHT                (IDS_MESESLIGHT+0)
   #define IDS_FEBREROLIGHT              (IDS_MESESLIGHT+1)
   #define IDS_MARZOLIGHT                (IDS_MESESLIGHT+2)
   #define IDS_ABRILLIGHT                (IDS_MESESLIGHT+3)
   #define IDS_MAYOLIGHT                 (IDS_MESESLIGHT+4)
   #define IDS_JUNIOLIGHT                (IDS_MESESLIGHT+5)
   #define IDS_JULIOLIGHT                (IDS_MESESLIGHT+6)
   #define IDS_AGOSTOLIGHT               (IDS_MESESLIGHT+7)
   #define IDS_SEPTIEMBRELIGHT           (IDS_MESESLIGHT+8)
   #define IDS_OCTUBRELIGHT              (IDS_MESESLIGHT+9)
   #define IDS_NOVIEMBRELIGHT            (IDS_MESESLIGHT+10)
   #define IDS_DICIEMBRELIGHT            (IDS_MESESLIGHT+11)  
   
#define IDS_DIASEMANALIGHT               (IDS_DICIEMBRELIGHT+1)
   #define IDS_LUNESLIGHT                (IDS_DIASEMANALIGHT+0)
   #define IDS_MARTESLIGHT               (IDS_DIASEMANALIGHT+1)
   #define IDS_MIERCOLESLIGHT            (IDS_DIASEMANALIGHT+2)
   #define IDS_JUEVESLIGHT               (IDS_DIASEMANALIGHT+3)
   #define IDS_VIERNESLIGHT              (IDS_DIASEMANALIGHT+4)
   #define IDS_SABADOLIGHT               (IDS_DIASEMANALIGHT+5)
   #define IDS_DOMINGOLIGHT              (IDS_DIASEMANALIGHT+6)
  

//============ FORMATO ENGLISH ===========================

#define IDS_MONTHS                    (IDS_DOMINGOLIGHT+1)
   #define IDS_M1                     (IDS_MONTHS+0)
   #define IDS_M2			          (IDS_MONTHS+1)
   #define IDS_M3                     (IDS_MONTHS+2)
   #define IDS_M4                     (IDS_MONTHS+3)
   #define IDS_M5                     (IDS_MONTHS+4)
   #define IDS_M6				      (IDS_MONTHS+5)
   #define IDS_M7                     (IDS_MONTHS+6)
   #define IDS_M8					  (IDS_MONTHS+7)
   #define IDS_M9					  (IDS_MONTHS+8)
   #define IDS_M10                    (IDS_MONTHS+9)
   #define IDS_M11                    (IDS_MONTHS+10)
   #define IDS_M12                    (IDS_MONTHS+11)  
   
#define IDS_DAYS                      (IDS_M12+1)
   #define IDS_D1                     (IDS_DAYS+0)
   #define IDS_D2                     (IDS_DAYS+1)
   #define IDS_D3                     (IDS_DAYS+2)
   #define IDS_D4                     (IDS_DAYS+3)
   #define IDS_D5                     (IDS_DAYS+4)
   #define IDS_D6                     (IDS_DAYS+5)
   #define IDS_D7                     (IDS_DAYS+6) 


//============== FORMATO DEUTSCH ========================= 

#define IDS_MONATE                    (IDS_D7+1)
   #define IDS_JANUAR				  (IDS_MONATE+0)
   #define IDS_FEBRUAR				  (IDS_MONATE+1)
   #define IDS_MAERZ				  (IDS_MONATE+2)
   #define IDS_APRIL				  (IDS_MONATE+3)
   #define IDS_MAI					  (IDS_MONATE+4)
   #define IDS_JUNI					  (IDS_MONATE+5)
   #define IDS_JULI					  (IDS_MONATE+6)
   #define IDS_AUGUST				  (IDS_MONATE+7)
   #define IDS_SEPTEMBER			  (IDS_MONATE+8)
   #define IDS_OKTOBER				  (IDS_MONATE+9)
   #define IDS_NOVEMBER				  (IDS_MONATE+10)
   #define IDS_DEZEMBER				  (IDS_MONATE+11)

#define IDS_TAGE					  (IDS_DEZEMBER+1)
   #define IDS_MONTAG				  (IDS_TAGE+0)
   #define IDS_DIENSTAG				  (IDS_TAGE+1)
   #define IDS_MITTWOCH				  (IDS_TAGE+2)
   #define IDS_DONNERSTAG			  (IDS_TAGE+3)
   #define IDS_FREITAG				  (IDS_TAGE+4)
   #define IDS_SAMSTAG				  (IDS_TAGE+5)
   #define IDS_SONNTAG				  (IDS_TAGE+6)


//=============== FORMATO FRENCH =============================

#define IDS_FRM						  (IDS_SONNTAG+1)
	#define IDS_JAN					  (IDS_FRM+0)
	#define IDS_FEV					  (IDS_FRM+1)
	#define IDS_MAR					  (IDS_FRM+2)
	#define IDS_AVR					  (IDS_FRM+3)
	#define IDS_FMAI				      (IDS_FRM+4)	
	#define IDS_JUI					  (IDS_FRM+5)
	#define IDS_JUIL					  (IDS_FRM+6)
	#define IDS_AOU				      (IDS_FRM+7)
	#define IDS_SEP					  (IDS_FRM+8)
	#define IDS_OCT					  (IDS_FRM+9)
	#define IDS_NOV					  (IDS_FRM+10)
	#define IDS_DEC					  (IDS_FRM+11)

#define IDS_JOURS					  (IDS_DEC+1)
	#define IDS_LUNDI				  (IDS_JOURS+0)
	#define IDS_MARDI				  (IDS_JOURS+1)
	#define IDS_MERCREDI			  (IDS_JOURS+2)
	#define IDS_JEUDI				  (IDS_JOURS+3)
	#define IDS_VENDREDI			  (IDS_JOURS+4)
	#define IDS_SAMEDI				  (IDS_JOURS+5)
	#define IDS_DIMANCHE			  (IDS_JOURS+6)

//--------------------------------------------------->Conversion de los *.rc

#ifndef _INCLUDE_BUILDING_RESOURCES_
extern 
#endif
const struct StResources {
	m4uint16_t m_iResId ;
	const m4char_t *m_pcResStr ;
} g_stResources[] 
#ifdef _INCLUDE_BUILDING_RESOURCES_

//IMPORTANTE:La tabla debe estar ordenada de ID menor a ID mayor:se hace una busqueda dicotomica

= {


	//------------>

    { IDS_NOMBRE_MONEDA ,     "Pts" },
    { IDS_ENERO,              "Enero" },
    { IDS_FEBRERO,            "Febrero" },
    { IDS_MARZO,              "Marzo" },
    { IDS_ABRIL,              "Abril" },
    { IDS_MAYO,               "Mayo" },
    { IDS_JUNIO,              "Junio" },
    { IDS_JULIO,              "Julio" },
    { IDS_AGOSTO,             "Agosto" },
    { IDS_SEPTIEMBRE,         "Septiembre" },
    { IDS_OCTUBRE,            "Octubre" },
    { IDS_NOVIEMBRE,          "Noviembre" },
    { IDS_DICIEMBRE,          "Diciembre" },
    { IDS_LUNES,              "Lunes" },
    { IDS_MARTES,             "Martes" },
    { IDS_MIERCOLES,          "Mi\351rcoles" },
    { IDS_JUEVES,             "Jueves" },
    { IDS_VIERNES,            "Viernes" },
    { IDS_SABADO,             "S\341bado" },
    { IDS_DOMINGO,            "Domingo" },

	//------------>

    { IDS_M1,                 "January" },
    { IDS_M2,                 "February" },
    { IDS_M3,                 "March" },
    { IDS_M4,                 "April" },
    { IDS_M5,                 "May" },
    { IDS_M6,                 "June" },
    { IDS_M7,                 "July" },
    { IDS_M8,                 "August" },
    { IDS_M9,                 "September" },
    { IDS_M10,                "October" },
    { IDS_M11,                "November" },
    { IDS_M12,                "December" },
    { IDS_D1,                 "Monday" },
    { IDS_D2,                 "Tuesday" },
    { IDS_D3,                 "Wednesday" },
    { IDS_D4,                 "Thursday" },
    { IDS_D5,                 "Friday" },
    { IDS_D6,                 "Saturday" },
    { IDS_D7,                 "Sunday" },

	//------------>

    { IDS_JANUAR,		    "Januar" },
    { IDS_FEBRUAR,		    "Februar" },
    { IDS_MAERZ,           "März" },
    { IDS_APRIL,	       "April" },
    { IDS_MAI,	       "Mai" },
    { IDS_JUNI,           "Juni" },
    { IDS_JULI,	       "Juli" },
    { IDS_AUGUST,		    "August" },
    { IDS_SEPTEMBER,         "September" },
    { IDS_OKTOBER,		    "Oktober" },
    { IDS_NOVEMBER,            "November" },
    { IDS_DEZEMBER,	    "Dezember" },
    { IDS_MONTAG,		    "Montag" },
    { IDS_DIENSTAG,	    "Dienstag" },
    { IDS_MITTWOCH,	    "Mittwoch" },
    { IDS_DONNERSTAG,          "Donnerstag" },
    { IDS_FREITAG,		    "Freitag" },
    { IDS_SAMSTAG,		    "Samtag" },
    { IDS_SONNTAG,             "Sonntag" },

	//------------>
	 
	{ IDS_JAN,				"Janvier" },
	{ IDS_FEV,				"Février" },
	{ IDS_MAR,				"Mars" },
	{ IDS_AVR,				"Avril" },
	{ IDS_FMAI,				"Mai" },
	{ IDS_JUI,				"Juin" },
	{ IDS_JUIL,				"Juillet" },
	{ IDS_AOU,				"Août" },
	{ IDS_SEP,				"Septembre" },
	{ IDS_OCT,				"Octobre" },
	{ IDS_NOV,				"Novembre" },
	{ IDS_DEC,				"Décembre" },
	{ IDS_LUNDI,			"Lundi" },
	{ IDS_MARDI,			"Mardi" },
	{ IDS_MERCREDI,			"Mercredi" },
	{ IDS_JEUDI,			"Jeudi" },
	{ IDS_VENDREDI,			"Vendredi" },
	{ IDS_SAMEDI,			"Samedi" },
	{ IDS_DIMANCHE,			"Dimanche" },

	//------------>
	
    { IDS_NOMEMORY,  "No hay memoria suficiente" } ,
    { IDS_INVALIDHANDLE, "Handle de formato no válido" } ,
    { IDS_NOPERMISSION , "No se tiene permiso sobre el elemento" },

	//------------>

    { IDS_TIPOSQL,            "El Tipo SQL es err\363neo o no tiene equivalente en M4" },
    { IDS_FORMATONOCREADO,    "El formato no ha sido creado" },    
    { IDS_IMPOSIBLETRANSF,    "No se puede convertir la cadena de entrada al formato de salida" },
    { IDS_FORMATOENTRADA,     "La cadena de entrada no est\341 formateada con el formato de entrada" },
    { IDS_PROPIEDADNOE,       "La propiedad especificada no existe" },
    { IDS_IDIOMANOSOPORTADO,  "El idioma pedido no esta soportado" },
    { IDS_VALORNOCORRECTO,    "El valor que se ha intentado asignar a la propiedad no es correcto" },
    { IDS_FORMATOSDIFERENTES, "Error interno: se ha intentado la copia entre formatos de diferentes idiomas" },
    { IDS_FORMATOFUENTENULO,  "Se ha intentado copiar de un formato que no existe" },
    { IDS_DESFORMONEDA,       "La cadena que se supone es la moneda no coincide" },
    { IDS_DESFORANIO,         "Se ha intentado formatear el a\361o de una forma no soportada" },
    { IDS_DESFORMES,          "Se ha intentado formatear el mes de una forma no soportada" },
    { IDS_DESFORMES1,         "La cadena del mes no es un nombre de mes valido" },
    { IDS_DESFORSEM,          "Se ha intentado formatear el d\355a de la semana de una forma no soportada" },
    { IDS_DESFORSEM1,         "La cadena del d\355a de la semana no es un nombre de un d\355a de la semana valido" },
    { IDS_DESFORDIA,          "Se ha intentado formatear el d\355a de una forma no soportada" },
    { IDS_DESFORHORA,         "Se ha intentado formatear la hora de una forma no soportada" },
    { IDS_DESFORMIN,          "Se han intentado formatear los minutos de una forma no soportada" },
    { IDS_DESFORSEG,          "Se han intentado formatear los segundos de una forma no soportada" },
    { IDS_DESFORTIME,         "Error general al desformatear un tipo \221Time\222" },
    { IDS_NOENOMBRE,          "No existe un handle con ese nombre" },
    { IDS_ERRORNOEXISTE,      "Error desconocido" },

	//------------>

    { IDS_MALANIO,            "Año no válido" },
    { IDS_MALMES,             "Mes no válido" },
    { IDS_MALDIASEMANA,       "Día de la semana no válido" },
    { IDS_MALDIA,             "Día no válido" },
    { IDS_MALHORA,            "Hora no válido" },
    { IDS_MALMINUTO,          "Minutos no válidos" },
    { IDS_MALSEGUNDO,         "Segundos no válidos" },
    { IDS_MALNUMERO,          "Número no válido" },
    { IDS_MALTIPO,            "Tipo erroneo" },

	//------------>

} 
#endif
;

m4int16_t g_CargaCadena(m4uint32_t id, m4char_t* buf, m4int16_t lbuf) ;

#endif     

