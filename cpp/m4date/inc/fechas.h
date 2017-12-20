//## begin module.cp preserve=yes
/***************************************************************************/
/**** Manipulacion de Fechas Julianas                                   ****/
/****  Rev 1.0          24-Abril-1992                                   ****/
/****                                                                   ****/
/***************************************************************************/

//## end module.cp
#ifndef FECHAS_H
#define FECHAS_H

#include <m4date_dll.hpp>

#include "fechasbase.h"
//## begin module.epilog preserve=yes

#define	M4CL_TIME_GM_TIME	                            1
#define	M4CL_TIME_LOCAL_TIME	                        2


/*--- prototipos */

int M4_DECL_M4DATE nano (JulianType dias );
int M4_DECL_M4DATE nmes (JulianType dias );
int M4_DECL_M4DATE ndia (JulianType dias );
int M4_DECL_M4DATE ndiasem (JulianType dias );

int esAnioBisiesto(int iAnio);
JulianType M4_DECL_M4DATE nfecha30 ( int dia, int mes , int ano, int februaryas30 );
JulianType M4_DECL_M4DATE nfecha30 ( JulianType dias, int februaryas30 );
int M4_DECL_M4DATE nannos   (JulianType dias1 , JulianType dias2);
JulianType M4_DECL_M4DATE nmeses     (JulianType dias1 , JulianType dias2);
JulianType M4_DECL_M4DATE nsumaannos (JulianType dias, int annos);
JulianType M4_DECL_M4DATE nsumameses (JulianType dias, long meses);
JulianType M4_DECL_M4DATE ndiascomun   (JulianType fini1, JulianType ffin1, JulianType fini2, JulianType ffin2);
JulianType M4_DECL_M4DATE nhoy (short ai_Mode = M4CL_TIME_GM_TIME);
int M4_DECL_M4DATE ndiasdelmes   (int mes, int ano);
int M4_DECL_M4DATE ndiasdelmes   (JulianType dias);
int M4_DECL_M4DATE ndiasdelano   (int ano);

//## end module.epilog
#endif // FECHAS_H
 
