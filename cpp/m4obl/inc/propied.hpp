#ifndef __PROPIEDADES__H_
#define __PROPIEDADES__H_

#include "cabecera.h"  


class M4ClassPropiedad
{
   protected:
     UINT uhid;                  // identificador de la propiedad
     void *pBlqPadre;            // puntero del padre de la propiedad , bloque que la contiene
     int iTipo;                  // tipo de la propiedad, T_PROP_INT,T_PROP_STR 
     M4ClStringOld szNomPropiedad;  // Cadena con el nombre de la propiedad
     M4ClStringOld szValPropiedad;  // Cadena con el valor de la propiedad
     
   
   public:
                       
     // CONSTRUCTOR Y DESTRUCTOR
                       
     M4ClassPropiedad(); 
     ~M4ClassPropiedad(); 
    
     // FUNCIONES DE ASIGNACION Y MODIFICACION
     M4RETCODE M4CPSetPadre(void *pPadre);
     M4RETCODE M4CPGetPadre(void **pPadre);
     M4RETCODE M4CPSetPropiedad(char *pszPropiedad,char *pszValor,UINT uhident,int iTipoPrp); 
     M4RETCODE M4CPSetVal(char *pszVal,int iTipoPrp);
   
     // FUNCIONES DE CONSULTA
     M4RETCODE M4CPGetPropiedad(char **ppszValPropiedad); // devuelve el valor de la propiedad
     M4RETCODE M4CPGetNomPropiedad(char **ppszNomValor);  // devuelve el nombre de la propiedad
     M4RETCODE M4CPGetTipoPropiedad(int *iTipoPrp);
     M4RETCODE M4CPGetHandle(UINT *puhident);  
     
     // FUNCION AMIGA PARA COMPARAR DOS PROPIEDADES
     friend int ComparaProp(const void *pprop1,const void *pprop2);
};


#endif //__PROPIEDADES__H_         



