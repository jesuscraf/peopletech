//## begin module.cp preserve=yes
/***************************************************************************/
/**** Manipulacion de Fechas Julianas                                   ****/
/****  Rev 1.0          24-Abril-1992                                   ****/
/****                                                                   ****/
/***************************************************************************/

#ifndef FECHASBASE_H
#define FECHASBASE_H

#include <m4date_dll.hpp>


#define GREGORIO_ANO         1752L           /* a¤o del ajuste gregoriano */
#define GREGORIO_MES            9L           /* mes del ajuste gregoriano */
#define GREGORIO_DIA           20L           /* dia del ajuste gregoriano */
#define GREGORIO_DIAS          11L           /* dias a restar de la fecha */
#define GREGORIO_NUM_DIAS  639792L           /* dias hasta el 20-09-92    */


/*--- Definicion del tipo que contiene cuenta de dias */
typedef double JulianType;

JulianType M4_DECL_M4DATE numdias (int dia, int mes, int anno);
void M4_DECL_M4DATE diamesano (JulianType dias , int * dia , int * mes , int * ano );

#endif
 
