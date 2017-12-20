#include "propied.hpp"




M4ClassPropiedad::M4ClassPropiedad()
  {
    
    uhid=0;
    pBlqPadre=NULL;
    
  };


M4ClassPropiedad::~M4ClassPropiedad()
  {
    uhid=0;  
    pBlqPadre = NULL;
    
   };                                      
   
M4RETCODE M4ClassPropiedad::M4CPSetPadre(void *pPadre)
 {
   pBlqPadre = pPadre; 
   return EXITO;
 }; 
 
M4RETCODE M4ClassPropiedad::M4CPGetPadre(void **pPadre)
 {
  *pPadre = pBlqPadre;
  return EXITO;
 };                                          
 



M4RETCODE M4ClassPropiedad::M4CPSetPropiedad(char *pszPropiedad,char *pszValor,UINT uhident,int iTipoPrp)
  { 
    /* Asigna los valores a una propiedad y los modifica
       En caso de modificar libera la memoria anteriormente allocada.
       Los valores de vueltos
          IDS_RTE_PROP_HAND   -> error handle mal definido
          IDS_RTE_PROP_CAD_NULL   -> cadena no validad
          IDS_RTE_STR_SIN_CARECTERES -> se mete un nombre sin letras
          EXITO -> se ha introducido las cadenas correctamente */
  
    if ( uhident < 0 ) return IDS_RTE_PROP_HAND;                                                
    if (pszPropiedad==NULL || pszValor==NULL) return IDS_RTE_PROP_CAD_NULL; 
    if ( strlen(pszPropiedad) == 0 ) return IDS_RET_STR_SIN_CARACTERES;
    // por si acaso ya estaba asignado este objeto se libera las cadenas 
    // que pudieran estar asignadas anteriormente 
    uhid=uhident;
    StrCpy(szNomPropiedad,strupr(pszPropiedad));
    StrCpy(szValPropiedad,pszValor);
    iTipo = iTipoPrp;               
    // EN CASO DE QUE SEA DE TIPO ENTERO TENGO QUE COMPROBAR QUE LO QUE ESTOY ASIGNANDO ES UN NUMERO
    if (iTipo==T_PRP_INT && strlen((const char*)szValPropiedad) != strspn(szValPropiedad,"1234567890,.-")) return IDS_NUM_INCORECTO;

    return EXITO;
   }; 
   
 
 M4RETCODE M4ClassPropiedad::M4CPSetVal(char *pszValor,int iTipoProp)
   {
      // Asigna el valor al a una propiedad, se usara para modificar
      if (pszValor==NULL) return IDS_RTE_PROP_CAD_NULL; 
      if (iTipoProp != T_PRP_INT && iTipoProp != T_PRP_STR && iTipoProp != T_PRP_FEC ) return IDS_RTE_PROP_TIPO;
      StrCpy ( szValPropiedad , pszValor ); // POR DEFINICION LAS PROPIEDADES SON EM MAYUSCULAS
      iTipo = iTipoProp;
      return EXITO;
   };
   
 
           

 M4RETCODE M4ClassPropiedad::M4CPGetNomPropiedad(char **ppszNomPropiedad) 
   {                   
      /* devuelvo la cadena de la propiedad
         el codigo de vuelta es:
               -1 quiere decir que la proiedad no ha sido asignada
            EXITO  */
      
      if ((const char *)szNomPropiedad==NULL) return -1;
      // no hay error
      *ppszNomPropiedad = szNomPropiedad;
      return EXITO; 
   };
 
 M4RETCODE M4ClassPropiedad::M4CPGetTipoPropiedad(int *iTipoPrp)
    {
      *iTipoPrp = iTipo;
      return EXITO;
    };
      
 
 M4RETCODE M4ClassPropiedad::M4CPGetPropiedad(char **ppszValPropiedad) 
   {                   
      /* devuelvo la cadena de la propiedad
         el codigo de vuelta es:
               -1 quiere decir que la proiedad no ha sido asignada
            EXITO  */
      
      if ((const char *)szValPropiedad==NULL) return -1;
      // no hay error
      *ppszValPropiedad = szValPropiedad;
      return EXITO; 
   }; 
 
 
 M4RETCODE M4ClassPropiedad::M4CPGetHandle(UINT *puhident)
   {
       /* devuelvo la cadena de la propiedad
         el codigo de vuelta es:
               -1 quiere decir que la proiedad no ha sido asignada
            EXITO  */        
     
     if (uhid==0) return -1;
     *puhident = uhid;     // EL HANDLE DEVUELTO SIEMPRE ES UNO MENO DE LO QUE TENGO GUARDADO
                             // ESTO ES PARA EVITAR QUE HAYA HANDLES CON VALORES NULOS 
     return EXITO;
   };                             
     

int ComparaProp(const void *pprop1,const void *pprop2)
   {
     char *cad1;
     char *cad2;
     
     ((M4ClassPropiedad *)pprop1)->M4CPGetNomPropiedad(&cad1);        
     ((M4ClassPropiedad *)pprop2)->M4CPGetNomPropiedad(&cad2);   
     return strcmp(cad1,cad2);   
   };  
               


