//## begin module.includes preserve=yes
/***************************************************************************/
/**** Manipulacion de Fechas Julianas                                   ****/
/****  Rev 1.0          24-Abril-1992                                   ****/
/****                                                                   ****/
/***************************************************************************/

#include <time.h>
#include "fechas.h"
#include "m4date.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes


        /************************************************/
        /****************** diasem  *********************/
        /************************************************/
        /* calcula el d¡a de la semana a partir de una  */
        /* fecha en formato d¡as desde 31-12-0. Devuelve*/
        /* 1-Lunes, 2-Martes, 3-Miercoles, .., 7-Domingo*/
        /************************************************/

int ndiasem (JulianType dias )
{
        int diasem;
        diasem = (int)(((long)dias % 7) + 4);
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
JulianType nhoy (short ai_Mode)
{
        struct tm fecha ;
        time_t ltime;

    // Devuelve segundos desde 1/1/1970 hasta la fecha del sistema
    time( &ltime );
    // Mete ese número en una estructura legible (que representa el día del sistema)
    switch( ai_Mode )
    {
        case M4CL_TIME_LOCAL_TIME:

			M4ClDate::LocalTime( ltime,fecha ) ;
            break ;

        default :

			M4ClDate::GmTime( ltime,fecha ) ;
            break ;
    }

    return(numdias (fecha.tm_mday, fecha.tm_mon + 1, 1900 + fecha.tm_year));
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

JulianType ndiascomun   (JulianType fini1, JulianType ffin1, JulianType fini2, JulianType ffin2)
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

JulianType nsumaannos   (JulianType dias,  int annos)
{
        int dia, mes, ano;
        JulianType result;
        int annofin, maxdia;

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
		/* Realiza correccion sobre la fecha            */
        /************************************************/

JulianType nsumameses   (JulianType dias,  long meses)
{
        int dia, mes, ano, maxdia;
        JulianType result;
        long mesfin;
        /* tabla de d¡as de cada mes. Febrero se modifica. */
        static int diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        diamesano (dias , &dia, &mes, &ano);

        mesfin = mes + meses;

		// Antonio ADN 3/12/2001 FCC
        if (mesfin < 1)
        {
            ano = (int) (ano - (((-mesfin) / 12) + 1));
            mesfin = 12 - ((-mesfin) % 12);
        }
		else
		{
			ano = (int) (ano + ((mesfin - 1) / 12));
			mesfin = ((mesfin - 1) % 12) + 1;
		}

        mes = (int) mesfin;
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
		/* Realiza correccion sobre la fecha            */
        /************************************************/

int ndiasdelmes   (int mes, int ano)
{
        int result;
        /* tabla de d¡as de cada mes. Febrero se modifica. */
        static int diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

		// Antonio ADN 3/12/2001 FCC
        if (mes < 1)
        {
            ano = ano - (((-mes) / 12) + 1);
            mes = 12 - ((-mes) % 12);
        }
		else
		{
			ano = ano + ((mes - 1) / 12);
			mes = ((mes - 1) % 12) + 1;
		}

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

int ndiasdelmes   (JulianType dias)
{
        int ano;
        int mes;
        int dia;

        diamesano (dias , &dia, &mes, &ano);

        return(ndiasdelmes(mes, ano));
}

        /************************************************/
        /****************** DIASDELANO ******************/
        /************************************************/
        /* calcula los d¡as del a¤o del a¤o             */
        /* correspondiente                              */
        /************************************************/

int ndiasdelano   (int ano)
{
        int result;

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

int nannos   (JulianType dias1 , JulianType dias2)
{
        int dia1,mes1,ano1;
        int dia2,mes2,ano2;
        int restaano = 0;
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

JulianType nmeses   (JulianType dias1 , JulianType dias2)
{
        int dia1,mes1,ano1;
        int dia2,mes2,ano2;
        int restames = 0;
        diamesano (dias1 , &dia1 , &mes1 , &ano1);
        diamesano (dias2 , &dia2 , &mes2 , &ano2);
        if (dia1 > dia2)
            restames = 1;
        return(((JulianType)(ano2 - ano1)) * 12 + mes2 - mes1 - restames);
}


/****************************************************************************
*	Name:			esAnioBisiesto	
*	Description:	Esta función se utiliza en nfecha30, para ver si el año 
*					es bisiesto
*  Parameters:
*  	Input:			int iAnio: el año en cuestión.
*
* 	Output:
*
*  Return:			int : flag de salida que indicará si el año dado
*					es bisiesto (1) o no (0).
*
*  Additional Information:
*

****************************************************************************/
int esAnioBisiesto(int iAnio)
{
	int iBisiesto = 0;

	// Verificar si el año es bisiesto.
	if ((iAnio % 4) == 0)
		// Los mutiplos de 4 son bisiestos
		iBisiesto = 1;
	else
		iBisiesto = 0;
	
	if ((iAnio % 100) == 0)
        // Los multiplos de 100 no
		iBisiesto = 0;

	if ((iAnio % 400) == 0)
		// Los multiplos de 400 si
		iBisiesto = 1;

	return iBisiesto;
}

        /************************************************/
        /****************** fecha30 *********************/
        /************************************************/
        /* calcula la distancia entre dia mes y a¤o y   */
        /* el 31 - 12 - 0000 suponiendo que todos los   */
        /* meses son de treinta d¡as                    */
        /************************************************/

JulianType nfecha30 ( int dia, int mes, int ano, int februaryas30 )
{
        JulianType r1 = 0;

		// Se debe tener en cuenta que en el caso del mes de febrero
		// es una excepción y su último dia es el 28 o el 29 en el caso
		// de ser un año bisiesto.
		// Sólo si se pide que febrero se considere como de 30 días

		if (februaryas30 == 1 && mes == 2)	//  mes de febrero.
		{
			int iBisiesto = esAnioBisiesto(ano);

			// Si el año es bisiesto y estamos en el 29 de febrero, 
			// ultimo dia de mes, dia = 30.
			if (iBisiesto == 1 && dia >= 29)
				dia = 30;
			
			// Si el año no es bisiesto, y estamos en el dia 28 de 
			// febrero, también último dia de mes, dia = 30.
			if (iBisiesto == 0 && dia >= 28)
				dia = 30;
		}
		
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

JulianType nfecha30 ( JulianType dias, int februaryas30 )
{
        JulianType r1 = 0;

        if (dias)       // si los d¡as son cero, la fecha es cero
        {
	        int dia,mes,ano;
            diamesano (dias , &dia , &mes , &ano);

			r1 = nfecha30( dia, mes, ano, februaryas30 );
        }

        return(r1);
}

        /************************************************/
        /******************   ano   *********************/
        /************************************************/
        /* calcula el a¤o a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/

int nano (JulianType dias )
{
        int pp1,pp2;
        int ano;
        diamesano (dias , &pp1 , &pp2 , &ano);
        return(ano);
}

        /************************************************/
        /******************   mes   *********************/
        /************************************************/
        /* calcula el mes a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/

int nmes (JulianType dias )
{
        int pp1,pp2;
        int mes;
        diamesano (dias , &pp1 , &mes , &pp2);
        return(mes);
}


        /************************************************/
        /******************   dia   *********************/
        /************************************************/
        /* calcula el dia a partir de una fecha en      */
        /* formato d¡as desde 31-12-0000                */
        /************************************************/

int ndia (JulianType dias )
{
        int pp1,pp2;
        int dia;
        diamesano (dias , &dia , &pp1 , &pp2);
        return(dia);
}

        /************************************************/
        /****************** diamesano *******************/
        /************************************************/
        /* interna calcula dia, mes a¤o a partir del n§ */
        /* de d¡as desde 31-12-0000                     */
        /************************************************/

void diamesano (JulianType dias , int * dia , int * mes , int * ano )
{
        /* tabla de d¡as de cada mes. Febrero se modifica. */
		// removes static reference
		// FIX bug 69006
        // static int diasmesTab [12] =
        int diasmesTab [12] =
        {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        JulianType diascomp;
        int diadelano;
        int mes1  = 0;
        int *mptr = diasmesTab;
        JulianType diasg;             /* con la correcci¢n de gregorio en 1752 */
        *ano  = ((int)(((dias > GREGORIO_NUM_DIAS ? dias + GREGORIO_DIAS : dias)/365.2425)+.002))+1;
        diascomp = numdias( 1 , 1 , *ano );
        if (diascomp > dias ) (*ano)--;
            diasg = ((*ano == GREGORIO_ANO) && (dias > GREGORIO_NUM_DIAS)) ? dias + GREGORIO_DIAS : dias;

        diadelano = (int) (diasg - (numdias( 1 , 1 , *ano )));
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
		/* Realiza correcciones sobre la fecha          */
        /************************************************/

JulianType numdias (int dia, int mes, int anno)
{

        JulianType result;
        /* tabla de d¡as acumulados por mes */
        static const int dpmAccTab [13] =
        {
            0, 0, 31, 28+31,
            31+59, 30+31+59, 31+30+31+59,
            30+151, 31+30+151, 31+31+30+151,
            30+243, 31+30+243, 30+31+30+243
        };

        long dias;
        int bisiestos, anno1;

		// Antonio ADN 3/12/2001 FCC
        if (mes < 1)
        {
            anno = anno - (((-mes) / 12) + 1);
            mes = 12 - ((-mes) % 12);
        }
		else
		{
			anno = anno + ((mes - 1) / 12);
			mes = ((mes - 1) % 12) + 1;
		}

        anno1 = mes <= 2 ? anno-1 : anno;

        bisiestos = anno1 / 4 ;
        bisiestos -= anno1/100 ;
        bisiestos += anno1/400 ;
        dias = ((long)anno-1) * 365 + (long)bisiestos + (long)dpmAccTab [mes] + (long)(dia);

        if ((((long)anno * 10000) + ((long)mes *100) + (long)dia ) >= ((GREGORIO_ANO*10000L) + (GREGORIO_MES*100L) + GREGORIO_DIA )) dias -= GREGORIO_DIAS;
        result = dias;
        return(result);
  }
 
//## end module.epilog
