#include "propied.hpp"
#include "arraprop.hpp"
#include "def_bloq.hpp"                   
#include "arrhand.hpp"
#include "cabecera.h"   
#include "lobj.hpp"   
#include "arrayobj.hpp"                              
#include "m4stringobl.hpp"

#include "m4lgobj.hpp"
#include <malloc.h>
                         
// variable global del array de ficheros

M4ArrayLOBL Aobj;

#ifdef _WINDOWS
HINSTANCE g_hInst = NULL ;   
#else
extern HINSTANCE g_hInst ;   
#endif




// añade un fichero al array y devuelve el handle correspondiente


int M4AOblInicializa(char *szNombFich, char *szExtFich, char *szTitulo, char *szBase, UINT *uHand)
 {   
    
   //MessageBox ( NULL , "Se ha inicializado un elemento","M4AOblInicializa",MB_OK); 
   return Aobj.M4AOblInicializa(szNombFich,szExtFich,szTitulo,szBase,&(*uHand));
 }; 
 
int M4AOblAddTexto(UINT uHand,char *szTexto)
 {
   M4ClStringOld szBuffer;
   
   StrCpy(szBuffer,szTexto);
   //if ( strcpy(szBuffer,szTexto) < 0 ) return IDS_RTE_NO_MEMORIA;
   return Aobj.M4AOblAddTexto(uHand,szBuffer);
   
   
 }; 
int M4AOblGeneraObl(UINT uHand,char *szTexto)
 {
    M4ClStringOld szBuffer;
    int retdev;
                         
    StrCpy(szBuffer,szTexto);                     
    
    retdev = Aobj.M4AOblGeneraObl(uHand,szBuffer);
    

    _heapmin();
    
    return retdev;


 
 };

 
 
int M4AOblFinaliza(UINT uHand)
 { 
    int retdev;
   
    retdev =  Aobj.M4AOblFinaliza(uHand); 
    // Cuidado con el porque no se si funcionará muy bien .
    _heapmin();
    // En cuanto de gp se tiene que quitar.
    
    return retdev;
 }; 
 

int M4AOblGetVersion(UINT uHand,char *szVer,UINT uTamMaxszVer,UINT *uTamszVer)
 {                            
   char *pszVer;
   int retdev;
   UINT uLong;
   
   retdev = Aobj.M4AOblGetVersion(uHand,&pszVer);
   if (retdev != EXITO ) return retdev;
  
   uLong = (unsigned int) strlen(pszVer);          

   *uTamszVer= ( uLong < uTamMaxszVer) ? uLong : uTamMaxszVer;
   strncpy(szVer,pszVer,*uTamszVer);
   szVer[*uTamszVer]='\0';

   if (uLong>uTamMaxszVer) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 };  

 
int M4AOblSetVersion(UINT uHand,char *szVer )
 {
   return Aobj.M4AOblSetVersion( uHand , szVer );
 } 
 
 
 
 
int M4AOblGetClass(UINT uHand,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass)
 {  
    char *pszClass;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetClass(uHand,&pszClass);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszClass);          
    *uTamszClass= ( uLong < uTamMaxszClass) ? uLong : uTamMaxszClass;
    strncpy(szClass,pszClass,*uTamszClass);
	szClass[*uTamszClass]='\0';

    if (uLong>uTamMaxszClass) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 
 };

int M4AOblGetExt(UINT uHand,char *szExt,UINT uTamMaxszExt,UINT *uTamszExt)
{  
    char *pszExt;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetExt(uHand,&pszExt);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszExt);
    *uTamszExt= ( uLong < uTamMaxszExt) ? uLong : uTamMaxszExt;
    strncpy(szExt,pszExt,*uTamszExt);
	szExt[*uTamszExt]='\0';
    
    if (uLong>uTamMaxszExt) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 
 };
int M4AOblGetNombFich(UINT uHand,char *szNombFich,UINT uTamMaxszNombFich,UINT *uTamszNombFich)
{  
    char *pszNombFich;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetNombFich(uHand,&pszNombFich);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszNombFich);
    *uTamszNombFich= ( uLong < uTamMaxszNombFich) ? uLong : uTamMaxszNombFich;
    strncpy(szNombFich,pszNombFich,*uTamszNombFich);
    szNombFich[*uTamszNombFich]='\0';

   if (uLong>uTamMaxszNombFich) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 
 };
int M4AOblGetTit(UINT uHand,char *szTitulo,UINT uTamMaxszTitulo,UINT *uTamszTitulo)
{  
    char *pszTitulo;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetTit(uHand,&pszTitulo);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszTitulo);
    *uTamszTitulo= ( uLong < uTamMaxszTitulo) ? uLong : uTamMaxszTitulo;
    strncpy(szTitulo,pszTitulo,*uTamszTitulo);
	szTitulo[*uTamszTitulo]='\0';
    
    if (uLong>uTamMaxszTitulo) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 
 };
int M4AOblGetBase(UINT uHand,char *szBase,UINT uTamMaxszBase,UINT *uTamszBase)
{  
    char *pszBase;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetBase(uHand,&pszBase);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszBase);
    *uTamszBase= ( uLong < uTamMaxszBase) ? uLong : uTamMaxszBase;
    strncpy(szBase,pszBase,*uTamszBase);
	szBase[*uTamszBase]='\0';
    
    if (uLong>uTamMaxszBase) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 
 };

 
int M4AOblGetHinst(UINT uHand,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst)
 {
    char *pszHinst;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetHinst(uHand,&pszHinst);
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszHinst);
    *uTamszHinst= ( uLong < uTamMaxszHinst) ? uLong : uTamMaxszHinst;
    strncpy(szHinst,pszHinst,*uTamszHinst);  
	szHinst[*uTamszHinst]='\0';
    
    if (uLong>uTamMaxszHinst) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 }; 
 
int M4AOblSetHinst(UINT uHand,char *szHinst )
 {
    return Aobj.M4AOblSetHinst( uHand , szHinst);
 
 }  
 
int M4AOblGetValPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal)
 {  
    char *pszVal; 
    int retdev;
    UINT uLong;          
    
 
  //  if (uTamszVal<MAX_TAM_PROP) return IDS_RTE_TAM_STR_MENUDO;
    retdev = Aobj.M4AOblGetValProp(uHand,pPathInst,pNomb,&pszVal);   
    if (retdev != EXITO ) return retdev;
  
    uLong = (unsigned int) strlen(pszVal);
    *uTamszVal= ( uLong < uTamMaxszVal) ? uLong : uTamMaxszVal;
    strncpy(szVal,pszVal,*uTamszVal);
	szVal[*uTamszVal]='\0';
  
     
    if (uLong>uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 };   

int M4AOblGetValPropPath(UINT uHand,char *pPathInst,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal)
  {
    char *pszVal;
    int retdev;
    UINT uLong;
    
    retdev = Aobj.M4AOblGetValProp(uHand,pPathInst,&pszVal);
    if (retdev != EXITO ) return retdev;
   
    uLong = (unsigned int) strlen(pszVal);
    *uTamszVal= ( uLong < uTamMaxszVal) ? uLong : uTamMaxszVal;
    strncpy(szVal,pszVal,*uTamszVal);
	szVal[*uTamszVal]='\0';
   
     
    if (uLong>uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  };    

int M4AOblGetPropHandNomb(UINT uHand,UINT hBlq,char *szNombProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal)
  {
    char *pszVal;
    int retdev;
    UINT uLong;
    
    retdev = Aobj.M4AOblGetPropHandNomb(uHand,hBlq,szNombProp,&pszVal);
    if (retdev != EXITO ) return retdev;
   
    uLong = (unsigned int) strlen(pszVal);
    *uTamszVal= ( uLong < uTamMaxszVal) ? uLong : uTamMaxszVal;
    strncpy(szVal,pszVal,*uTamszVal);
	szVal[*uTamszVal]='\0';
   
     
    if (uLong>uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  
  }
  
      
int M4AOblGetHandPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,UINT *uHandout)
 {            
    UINT uHandle;
    int retdev;
 
    retdev = Aobj.M4AOblGetHandProp(uHand,pPathInst,pNomb,&uHandle);
    if (retdev != EXITO ) return retdev;
    *uHandout=uHandle;  
    return EXITO;
  
  };
int M4AOblGetHandPropPath(UINT uHand,char *pPathInst,UINT *uHandout)
  {
    UINT uHandle;
    int retdev;
 
    retdev = Aobj.M4AOblGetHandProp(uHand,pPathInst,&uHandle);
    if (retdev != EXITO ) return retdev;
    *uHandout=uHandle;  
    return EXITO;
  
  
  };
  
int M4AOblGetHandBlq(UINT uHand,char *pPathInst,UINT *uHandout)
  {
    UINT uHandle;
    int retdev;
   
    
    retdev = Aobj.M4AOblGetHandBlq(uHand,pPathInst,&uHandle);
             
    if (retdev != EXITO ) return retdev;
    *uHandout=uHandle;  
    return EXITO;
  
  };

int M4AOblGetClassBlqHandle (UINT uHand,UINT uHandBlq,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass)
 {
    char *pszClass;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetClassBlqHandle(uHand,uHandBlq,&pszClass);
    if (retdev != EXITO ) return retdev;
    
    uLong = (unsigned int) strlen(pszClass);
    *uTamszClass= ( uLong < uTamMaxszClass) ? uLong : uTamMaxszClass;
    strncpy(szClass,pszClass,*uTamszClass);
	szClass[*uTamszClass]='\0';
    
     
    if (uLong>uTamMaxszClass) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
 }; 
 

int M4AOblGetHinstBlqHandle (UINT uHand,UINT uHandBlq,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst)
  {        
    char *pszHinst;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetHinstBlqHandle(uHand,uHandBlq,&pszHinst);
    if (retdev != EXITO ) return retdev;
    
    uLong = (unsigned int) strlen(pszHinst);
    *uTamszHinst= ( uLong < uTamMaxszHinst) ? uLong : uTamMaxszHinst;
    strncpy(szHinst,pszHinst,*uTamszHinst);
	szHinst[*uTamszHinst]='\0';
    
     
    if (uLong>uTamMaxszHinst) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  
  
  };            
  
int M4AOblSetHinstBlqHandle(UINT uHand,UINT uHandBlq,char *a_pszHinst)
 {
     return Aobj.M4AOblSetHinstBlqHandle(uHand, uHandBlq, a_pszHinst ); 
 } 
  
  
int M4AOblGetValPropHandle (UINT uHand,UINT uHandProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal)
  {       
    char *pszVal;
    int retdev;
    UINT uLong;           
    
    
    retdev = Aobj.M4AOblGetValPropHandle(uHand,uHandProp,&pszVal);
    if (retdev != EXITO ) return retdev;
    
    uLong = (unsigned int) strlen(pszVal);
    *uTamszVal= ( uLong < uTamMaxszVal) ? uLong : uTamMaxszVal;
    strncpy(szVal,pszVal,*uTamszVal);
	szVal[*uTamszVal]='\0';
   
     
   if (uLong>uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  
  };

    
int M4AOblGetNomPropHandle (UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb)
  {      
    char *pszNomb;
    int retdev;
    UINT uLong;
 
    retdev = Aobj.M4AOblGetNomPropHandle(uHand,uHandProp,&pszNomb);
    if (retdev != EXITO ) return retdev;
    
    uLong = (unsigned int) strlen(pszNomb);
    *uTamszNomb= ( uLong < uTamMaxszNomb) ? uLong : uTamMaxszNomb;
    strncpy(szNomb,pszNomb,*uTamszNomb);
	szNomb[*uTamszNomb]='\0';
     
    if (uLong>uTamMaxszNomb) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  
  };


/***********************************************************************************
************************************************************************************
  Implementacion          -> 30 /06 / 95
  Ult Version             ->
  M4AOblGetTipoHandNomb(UINT uHand,UINT hBlq,char *szNombProp,int *iTipo)
  Llama al arrya de obj y dado el handle de la propidedad ya le devuelve el tipo
************************************************************************************
************************************************************************************/
  
int M4AOblGetTipoPropHandle(UINT uHand,UINT uHandProp,int *iTipo)
{
       return  Aobj.M4AOblGetTipoPropHandle(uHand,uHandProp,&(*iTipo));
}
/***********************************************************************************
************************************************************************************
  Implementacion          -> 30 /06 / 95
  Ult Version             ->
  M4AOblGetTipoHandNomb(UINT uHand,UINT hBlq,char *szNombProp,int *iTipo)
  Con el handle de bloque y el nombre de la propiedad consigue el tipo de la propiedad
  llamando a la funcion miembro del array de obj
************************************************************************************
************************************************************************************/


int M4AOblGetTipoPropHandNomb(UINT uHand,UINT uBlq,char *szNombProp,int *iTipo)
{
    return  Aobj.M4AOblGetTipoHandNomb(uHand,uBlq,szNombProp,&(*iTipo));
} 

  
  
int M4AOblGetPropHandle (UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal)
  {
   char *pszNomb,*pszVal;
   int retdev;
   UINT uLongNomb,uLongVal; 
    
    
   retdev = Aobj.M4AOblGetNomPropHandle(uHand,uHandProp,&pszNomb);
   if (retdev != EXITO ) return retdev;
    
   uLongNomb = (unsigned int) strlen(pszNomb);
   *uTamszNomb= ( uLongNomb < uTamMaxszNomb) ? uLongNomb : uTamMaxszNomb;
   strncpy(szNomb,pszNomb,*uTamszNomb);
   szNomb[*uTamszNomb]='\0';
  
   
   retdev = Aobj.M4AOblGetValPropHandle(uHand,uHandProp,&pszVal);
    if (retdev != EXITO ) return retdev;
    
   uLongVal = (unsigned int) strlen(pszVal);
   *uTamszVal= ( uLongVal < uTamMaxszVal) ? uLongVal : uTamMaxszVal;
   strncpy(szVal,pszVal,*uTamszVal);
   szVal[*uTamszVal]='\0';
   
   if (uLongNomb>uTamMaxszNomb || uLongVal > uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;
  }




int M4AOblSetPropPath(UINT uHand,char *pszPathInst,char *pszNomb,char *pszValor,int iTipo)
 {
   return Aobj.M4AOblSetPropPath(uHand,pszPathInst,pszNomb,pszValor,iTipo); 
 };

int M4AOblSetPropNomb(UINT uHand,UINT uHandBlq,char *szNomb,char *szValor,int iTipo)
 {
   return Aobj.M4AOblSetPropNomb(uHand,uHandBlq,szNomb,szValor,iTipo);
 }  
 
int M4AOblSetPropHand(UINT uHand,UINT uHandProp,char *pszValor,int iTipo)
 {
    return Aobj.M4AOblSetPropHand(uHand,uHandProp,pszValor,iTipo);
 };  

int M4AOblAddPropPath(UINT uHand, char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp)
 {
    return Aobj.M4AOblAddPropPath(uHand,pPathInst,pszNomb,pszVal,iTipoPrp,&(*uHandProp));
 }; 

int M4AOblAddPropHand(UINT uHand, UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp)
 {
    return Aobj.M4AOblAddPropHand(uHand,uHandBlq,pszNomb,pszVal,iTipoPrp,&(*uHandProp));
 };                                   
 
int M4AOblDelPropPath(UINT uHand,char *pPathInst,char *pszNomb)
  {
     return Aobj.M4AOblDelPropPath(uHand,pPathInst,pszNomb);
  }; 

int M4AOblDelPropNomb(UINT uHand, UINT uHandBlq,char *szNomb)
  {
     return Aobj.M4AOblDelPropNomb(uHand,uHandBlq,szNomb);
  }
  

int M4AOblDelPropHand(UINT uHand,UINT uHandBlq)
  {
     return Aobj.M4AOblDelPropHand(uHand,uHandBlq);
  }; 

                               
int M4AOblAddBlqPath(UINT uHand,char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut)
 {
   return Aobj.M4AOblAddBlqPath(uHand,pPathInst,pszClass,pszHinst,&(*uHandBlqOut));
 };
 
int M4AOblAddBlqHand(UINT uHand,UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut)
 {
   return Aobj.M4AOblAddBlqHand(uHand,uHandBlq,pszClass,pszHinst,&(*uHandBlqOut));
 };    
                                                                               
int M4AOblDelBlqPath(UINT uHand, char *pPathInst)
 {
  return Aobj.M4AOblDelBlqPath(uHand,pPathInst);
 }           

int M4AOblDelBlqHand(UINT uHand,UINT uHandBlq)
 {
  return Aobj.M4AOblDelBlqHand(uHand,uHandBlq);
 }   
int M4AOblDelRama(UINT uHand , char *szPath)
 {
  return Aobj.M4AOblDelRama(uHand,szPath);
 }          
    

// FUNCIONES QUE SE DEDICAN A BUSQUEDAS REITERATIVAS
int M4AOblFirstHndProp(UINT uHand,char *szMatchPath,char *szMatchProp,UINT *uHandPrp,UINT *uHandMatch)
 { 
   int retdev ;

   retdev = Aobj.M4AOblFirstHndProp(uHand,szMatchPath,szMatchProp,&(*uHandPrp),&(*uHandMatch));
   return retdev;
 }
 
int M4AOblNextHndProp(UINT uHand,UINT uHandMatch,UINT *uHandPrp)
 {
   return Aobj.M4AOblNextHndProp(uHand,uHandMatch,&(*uHandPrp));
 }                               
 
int M4AOblFirstHndBlq(UINT uHand,char *szMatchPath,UINT *uHandBlq,UINT *uHandMatch)
 {
   return Aobj.M4AOblFirstHndBlq(uHand,szMatchPath,&(*uHandBlq),&(*uHandMatch));
 } 
 
int M4AOblNextHndBlq(UINT uHand,UINT uHandMatch,UINT *uHandBlq)
 {
   return Aobj.M4AOblNextHndBlq(uHand,uHandMatch,&(*uHandBlq));
 }                                                                             


int M4AOblFirstProp(UINT uHand,char *szMatchPath,char *szMatchPrp,
														char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,
                                                        char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,
                                                        UINT *uHandPrp,UINT *uHandMatch)
 {
   char *pVal,*pNomb;     
   char *szPath;                             
   UINT uLongVal,uLongNomb,uLongPath;
   int retdev ;
   
   szPath = new char[MAX_TAM_PATH];
   retdev = Aobj.M4AOblFirstProp(uHand,szMatchPath,szMatchPrp,szPath,&pNomb,&pVal,&(*uHandPrp),&(*uHandMatch));
   if (retdev != EXITO ) 
       {  
          delete []szPath;
          return retdev;
       };
   
   // compruebo si algunas de las cadenas se tinen que truncar
   uLongPath = (unsigned int) strlen(szPath);
   *uTamszPathOut= ( uLongPath < uTamMaxszPathOut) ? uLongPath : uTamMaxszPathOut;
   strncpy(szPathOut,szPath,*uTamszPathOut);
	szPathOut[*uTamszPathOut]='\0';

   delete []szPath;
   
   uLongNomb = (unsigned int) strlen(pNomb);
   *uTamszNomb= ( uLongNomb < uTamMaxszNomb) ? uLongNomb : uTamMaxszNomb;
   strncpy(szNomb,pNomb,*uTamszNomb);
   szNomb[*uTamszNomb]='\0';
   
   uLongVal = (unsigned int) strlen(pVal);
   *uTamszVal= ( uLongVal < uTamMaxszVal) ? uLongVal : uTamMaxszVal;
   strncpy(szVal,pVal,*uTamszVal);
   szVal[*uTamszVal]='\0';
   
   if (uLongPath>uTamMaxszPathOut || uLongNomb > uTamMaxszNomb || uLongVal > uTamMaxszVal) 
     return IDS_RES_TRUNCADO;
   else
     return EXITO;                                   
 }                                                       
                                                        
int M4AOblNextProp(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,UINT *uHandPrp)
 {
  char *pVal,*pNomb;     
  char *szPath;                             
  UINT  uLongVal,uLongNomb,uLongPath;
  int retdev ;
  
  
  szPath = new char[MAX_TAM_PATH];
  retdev = Aobj.M4AOblNextProp(uHand,uHandMatch,szPath,&pNomb,&pVal,&(*uHandPrp));
  if (retdev != EXITO )
      {  
        delete []szPath;
        return retdev;
       
      };
     // compruebo si algunas de las cadenas se tinen que truncar
  uLongPath = (unsigned int) strlen(szPath);
  *uTamszPathOut= ( uLongPath < uTamMaxszPathOut) ? uLongPath : uTamMaxszPathOut;
  strncpy(szPathOut,szPath,*uTamszPathOut);
  szPathOut[*uTamszPathOut]='\0';

  delete []szPath;
  
  uLongNomb = (unsigned int) strlen(pNomb);
  *uTamszNomb= ( uLongNomb < uTamMaxszNomb) ? uLongNomb : uTamMaxszNomb;
  strncpy(szNomb,pNomb,*uTamszNomb);
  szNomb[*uTamszNomb]='\0';
   
  uLongVal = (unsigned int) strlen(pVal);
  *uTamszVal= ( uLongVal < uTamMaxszVal) ? uLongVal : uTamMaxszVal;
  strncpy(szVal,pVal,*uTamszVal);
  szVal[*uTamszVal]='\0';
   
  if (uLongPath>uTamMaxszPathOut || uLongNomb > uTamMaxszNomb || uLongVal > uTamMaxszVal) 
    return IDS_RES_TRUNCADO;
  else
    return EXITO;        
  
  
 }                                                        

int M4AOblFirstBlq(UINT uHand,char *szMatchPath,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq,UINT *uHandMatch)
 {
  char *szPath;    
  UINT uLongPath;
  int retdev;
  
  szPath = new char[MAX_TAM_PATH]; 
  retdev=Aobj.M4AOblFirstBlq(uHand,szMatchPath,szPath,&(*uHandBlq),&(*uHandMatch));
  if (retdev != EXITO )
      {  
         delete []szPath;
         return retdev;
      };

  uLongPath = (unsigned int) strlen(szPath);
  *uTamszPathOut= ( uLongPath < uTamMaxszPathOut) ? uLongPath : uTamMaxszPathOut;
  strncpy(szPathOut,szPath,*uTamszPathOut);
  szPathOut[*uTamszPathOut]='\0';

  delete []szPath;
  
  
  if (uLongPath > uTamMaxszPathOut )
    return IDS_RES_TRUNCADO;
  else
    return EXITO;        
 }  

int M4AOblNextBlq(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq)
 {
  char *szPath;    
  UINT uLongPath;
  int retdev;
  
  szPath = new char[MAX_TAM_PATH]; 
  retdev=Aobj.M4AOblNextBlq(uHand,uHandMatch,szPath,&(*uHandBlq));
  if (retdev != EXITO )
      {  
         delete  []szPath;
         return retdev;
      };

  uLongPath = (unsigned int) strlen(szPath);
  *uTamszPathOut= ( uLongPath < uTamMaxszPathOut) ? uLongPath : uTamMaxszPathOut;
  strncpy(szPathOut,szPath,*uTamszPathOut);
  szPathOut[*uTamszPathOut]='\0';

  delete []szPath;
  
  
  if (uLongPath > uTamMaxszPathOut )
    return IDS_RES_TRUNCADO;
  else
    return EXITO;        
 }                                                   


                                               
// FIN DE FUNCIONES DE BUSQUEDAS
                                               
int M4AOblGetPath(UINT uHand,UINT uHandBlq, char *pPath)
 {
   return Aobj.M4AOblGetPath(uHand,uHandBlq,pPath);
 };    


int M4AOblWriteFicheroFirst(UINT uHand,char *szBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout)
 {
   return Aobj.M4AOblWriteFicheroFirst(uHand,szBufferout,uTamMaxszBufferout,&(*uTamszBufferout));
 }

int M4AOblWriteFicheroNext(UINT uHand,char *szBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout)
{
   return Aobj.M4AOblWriteFicheroNext(uHand,szBufferout,uTamMaxszBufferout,&(*uTamszBufferout));
 }

int M4AOblWriteBloque(UINT uHand,UINT uHandBlq,char *szBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout)
  {       
    char *szBuffer;
    UINT uLong ;
    int retdev;
    
    szBuffer = new char[MAX_TAM_BUFFER]; 
    
    retdev = Aobj.M4AOblWriteBloque(uHand,uHandBlq,szBuffer,uTamMaxszBufferout);
    if ( retdev != EXITO ) 
        {
            delete []szBuffer ;
            return retdev ; 
        }
    
    uLong = (unsigned int) strlen(szBuffer);
    *uTamszBufferout = ( uLong < (uTamMaxszBufferout - 1) ) ? uLong : (uTamMaxszBufferout - 1);
    strncpy(szBufferout,szBuffer,*uTamszBufferout); 
	szBufferout[*uTamszBufferout]='\0';    
    // para que sea una cadena cero terminada: JRM
    //szBufferout[uTamMaxszBufferout - 1] = 0 ;
    delete []szBuffer;    
    
    return retdev ;   
    
  
  };   
 
     
int M4AOblError(int iCodErr,char *szError,UINT uTamszError)
  {  
    int iRet;
    
   iRet = LoadString_obl(iCodErr,szError,uTamszError);
   if (!iRet) return -1;
   return EXITO;
  }             
  
int M4AOblGetPosicionError(UINT uHand,unsigned long *lPos, unsigned long  * lLinea, unsigned long * lOffSet)
  {
  
   return Aobj.M4AOblGetPosicionError(uHand, &( *lPos ), &( *lLinea ) , &( *lOffSet ) );
  }  

// FUNCIONES DE UTILIDAD

int M4AOblCpy(UINT uHandSource,UINT  *uHandDest)
  {
    return Aobj.M4AOblCpy(uHandSource,&(*uHandDest));
    
  } 
  
  
int M4AOblCpyBlq ( UINT ai_uHandSource , UINT ai_uHandBlqSrc , UINT ai_uHandDest , UINT ai_uHandBlqDest)
  {
  
    return Aobj.M4AOblCpyBlq ( ai_uHandSource , ai_uHandBlqSrc , ai_uHandDest , ai_uHandBlqDest );
  }


int M4AOblCmp(UINT uHand1,UINT uHand2,BOOL *bResult)
  {
     return Aobj.M4AOblCmp(uHand1,uHand2,&(*bResult));
  };


// FUNCIONES DE BUSQUEDAS DE LOS FICHEROS
      
int M4AOblFindFirstTask(UINT *uHandObl,UINT *uHandMatch)
  {
     return Aobj.M4AOblFindFirstTask(&(*uHandObl),&(*uHandMatch));
  }              

int M4AOblFindNextTask(UINT uHandMatch,UINT *uHandObl)
  {
     return Aobj.M4AOblFindNextTask(uHandMatch,&(*uHandObl));
  }
                                                             
                                                             
int M4AOblFindFirstExt(char *szMatchExt,UINT *uHandObl,UINT *uHandMatch)
  {
     return Aobj.M4AOblFindFirstExt(szMatchExt,&(*uHandObl),&(*uHandMatch));
  };
  

int M4AOblFindNextExt(UINT uHandMatch,UINT *uHandObl)
  {
     return Aobj.M4AOblFindNextExt(uHandMatch,&(*uHandObl));
  };
      
int M4AOblFindFirstNombFich(char *szMatchNomb,UINT *uHandObl,UINT *uHandMatch)
  {
     return Aobj.M4AOblFindFirstNombFich(szMatchNomb,&(*uHandObl),&(*uHandMatch));
  }

int M4AOblFindNextNombFich(UINT uHandMatch,UINT *uHandObl)
  {
     return Aobj.M4AOblFindNextNombFich(uHandMatch,&(*uHandObl));
  }
  
int M4AOblFindHandBlq (UINT ai_uHand, UINT ai_uHandBlq , char * ai_pszNombreClase  , int & aio_iPosicion , UINT & ao_uHandBlq )
  {
  
     return Aobj.M4AOblFindHandBlq( ai_uHand , ai_uHandBlq , ai_pszNombreClase  ,  aio_iPosicion ,  ao_uHandBlq );
  }     
      

// Funcion añadida por Canario
// NUEVO CANARIO


// Si el ai_TipoSearch = 0 	Bloque
//       ai_TipoSearch = 1		Propiedad

int M4AOblKillSearch ( UINT ai_uHand , UINT ai_uHandMatch , int ai_iTipoSearch )
  {
  
     return Aobj.M4AOblKillSearch ( ai_uHand , ai_uHandMatch  , ai_iTipoSearch );
  }     



// FIN NUEVO CANARIO


// funciones necesarias para la dll
                         
#ifdef _WINDOWS

// Para volcar los recursos a fichero

#ifdef _M4_MAKE_RESORCE_FILE

int  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    int     i ;
    int     iCount = 0 ;
    int     iFlag ;
    char    acResource[ 500 ] ;
    FILE*   fFile ;
    
    
    fFile = fopen( "resource.rc", "w" ) ;
    
    for( i = 0 ; i < 100000 ; i++ ) {
    
        if( LoadString( hInstance, i, acResource, 500 ) != 0 )
        {            
            iCount++ ;
            
            iFlag = 0 ;
           
            fprintf( fFile, "%d,%d,%s\n", iFlag, i, acResource ) ;           
        }        
    } 

    return( 0 ) ;
}

#elif defined _WINDOWS


BOOL CALLBACK DllMain(HANDLE hInst,DWORD ul_reason_being_called,LPVOID lpReserved) 
{
    switch (ul_reason_being_called) 
     {
     case DLL_THREAD_ATTACH:
     case DLL_PROCESS_ATTACH: // case of libentry call in win 3.x
          g_hInst = (HINSTANCE)hInst; 
     break;
     case DLL_PROCESS_DETACH: // case of wep call in win 3.x
               break;
     case DLL_THREAD_DETACH:
               break;
          default:
               break;
     } /* switch */

    return TRUE;                                                                
                                                                                
    UNREFERENCED_PARAMETER(lpReserved);                                         
} /* LibMain */                                                                 

#else

int CALLBACK LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeap, LPSTR lpszCmdLine )
{

    g_hInst = hinst;
    return 1;
}         
           
int CALLBACK WEP(int nExitType)
{   
    return 1;
}   

#endif

#endif




