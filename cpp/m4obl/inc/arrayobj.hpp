#ifndef __ARRAY_OBJETOS__H__
#define __ARRAY_OBJETOS__H__

           
#include "cabecera.h"
#include "def_bloq.hpp"
#include "analizad.hpp" 
#include "arrhand.hpp" 
#include "lobj.hpp"
#include "busque.hpp"      
#include "arraymat.hpp"                          

class M4ArrayLOBL
   {                 
    protected: 
       UINT uNumObl;    // me indica el numero de LObj que hay en el array
       
       ArrayMatch AMatch;
       M4ClassLOBL ** ppContLOBL; // es el que contiene a todos los ficheros
       
    public:   
      M4ArrayLOBL();
      ~M4ArrayLOBL(); 
      
      M4RETCODE M4AOblAcceso(UINT uHand); // esta funcion devuelve exito en caso de que se pueda acceder al objeto del array
       
      M4RETCODE M4AOblIniciaAnonimo(UINT *uHandOut); 
      M4RETCODE M4AOblInicializa(char *szNombFich,char *szExtFich,char *szTitulo,char *szBase,UINT *uHand);
      
      M4RETCODE M4AOblAddTexto(UINT uHand,char *szTexto); 
      M4RETCODE M4AOblGeneraObl(UINT uHand,char *szTexto);
      
      
      M4RETCODE M4AOblFinaliza(UINT uHand);
      
      M4RETCODE M4AOblGetTask(UINT uHand,t_PID *hTaskObl); 
      
      M4RETCODE M4AOblGetVersion( UINT uHand,char **pszVer);
      M4RETCODE M4AOblSetVersion( UINT uHand,char *pszVer);
      
      
      M4RETCODE M4AOblGetClass(UINT uHand,char **pszClass);
      M4RETCODE M4AOblGetHinst(UINT uHand,char **pszHinst);
      M4RETCODE M4AOblSetHinst(UINT uHand,char *pszHinst);
      
      
      M4RETCODE M4AOblGetExt(UINT uHand,char **pszExt);
      M4RETCODE M4AOblGetNombFich(UINT uHand,char **pszNombFich);
      M4RETCODE M4AOblGetTit(UINT uHand,char **pszTitulo);
      M4RETCODE M4AOblGetBase(UINT uHand,char **pszBase);
      
      M4RETCODE M4AOblGetValProp(UINT uHand,char *pPathInst,char *pNomb,char **pszVal);   
      M4RETCODE M4AOblGetValProp(UINT uHand,char *pPathInst,char **pszVal);    
      M4RETCODE M4AOblGetPropHandNomb(UINT uHand,UINT hBlq,char *szNombProp,char **szVal); // devuelve el valor de una propiedad dando el handle del bloque y el nombre de  la propiedad
      
      M4RETCODE M4AOblGetTipoHandNomb(UINT uHand,UINT hBlq,char *szNombProp,int *iTipo);
      M4RETCODE M4AOblGetTipoPropHandle(UINT uHand,UINT uProp,int * iTipo);
      
      M4RETCODE M4AOblGetHandProp(UINT uHand,char *pPathInst,char *pNomb,UINT *uHandout);
      M4RETCODE M4AOblGetHandProp(UINT uHand,char *pPathInst,UINT *uHandout);  
      
      M4RETCODE M4AOblGetHandBlq(UINT uHand, char *pPathInst,UINT *uHandout);
       
      M4RETCODE M4AOblGetClassBlqHandle (UINT uHand,UINT uHandBlq,char **pszClass); 
      M4RETCODE M4AOblGetHinstBlqHandle (UINT uHand,UINT uHandBlq,char **pszHinst); 
      M4RETCODE M4AOblSetHinstBlqHandle (UINT uHand,UINT uHandBlq,char *pszHinst);
      M4RETCODE M4AOblGetValPropHandle (UINT uHand,UINT uHandPrp,char **pszVal);    
      M4RETCODE M4AOblGetNomPropHandle (UINT uHand,UINT uHandPrp,char **pszNomb);    
                     
      M4RETCODE M4AOblSetPropPath(UINT uHand,char *pszPathInst,char *pszNomb,char *pszValor,int iTipo);           
      M4RETCODE M4AOblSetPropNomb(UINT uHand,UINT uHandBlq,char *szNomb,char *szValor,int iTipo);
      M4RETCODE M4AOblSetPropHand(UINT uHand, UINT uHandPrp,char *pszVal,int iTipo);    
      M4RETCODE M4AOblAddPropPath(UINT uHand, char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandPrp);
      M4RETCODE M4AOblAddPropHand(UINT uHand, UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandPrp);
      M4RETCODE M4AOblDelPropPath(UINT uHand, char *pPathInst,char *pNomb);
      M4RETCODE M4AOblDelPropNomb(UINT uHand, UINT uHanBlq,char *szNomb);
      M4RETCODE M4AOblDelPropHand(UINT uHand, UINT uHandPrp );
      
       
      M4RETCODE M4AOblAddBlqPath(UINT uHand,char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut);  
      M4RETCODE M4AOblAddBlqHand(UINT uHand,UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut);  
      M4RETCODE M4AOblDelBlqPath(UINT uHand, char *pPathInst); 
      M4RETCODE M4AOblDelBlqHand(UINT uHand, UINT uHandBlq);
      
      M4RETCODE M4AOblDelRama(UINT uHand , char *szPath);
      // FUNCIONES DE BUSQUEDAS Y CONSEGUIR PATHS
        M4RETCODE M4AOblFirstHndProp(UINT uHand,char *szMatchPath,char *szMatchProp,UINT *uHandPrp,UINT *uHandMatch);
        M4RETCODE M4AOblNextHndProp(UINT uHand,UINT uHandMatch,UINT *uHandPrp);
        M4RETCODE M4AOblFirstHndBlq(UINT uHand,char *szMatchPath,UINT *uHandMatch,UINT *uHandBlq);
        M4RETCODE M4AOblNextHndBlq(UINT uHand,UINT uHandMatch,UINT *uHandBlq);
        M4RETCODE M4AOblFirstProp(UINT uHand,char *szMatchPath,char *szMatchProp,char *szPathOut,char **pNomb,char **pVal,UINT *uHandPrp,UINT *uHandMatch);
        M4RETCODE M4AOblNextProp(UINT uHand,UINT uHandMatch,char *szPathOut,char **pNomb,char **pVal,UINT *uHandPrp);
        M4RETCODE M4AOblFirstBlq(UINT uHand,char *szMatchPath,char *szPathOut,UINT *uHandBlq,UINT *uHandMatch); 
        M4RETCODE M4AOblNextBlq(UINT uHand,UINT uHandMatch,char *szPathOut,UINT *uHandBlq);  
           
      
      // FUNCION DE DEBUG AÑADIDA JMMS 13 /07 / 95
      M4RETCODE M4AOblGetPosicionError (UINT uHand, DWORD * lPos, DWORD  * lLinea,DWORD  * lOffSet);
                
      // FUNCIONES DE UTILIDAD 
      
      
      
      M4RETCODE M4AOblCpy(UINT uHandSource,UINT  *uHandDest);
      M4RETCODE M4AOblCpyBlq ( UINT ai_uHandSource ,UINT  ai_uHandBlqSrc , UINT ai_uHandDest ,UINT ai_uHandBlqDest );
      M4RETCODE M4AOblCmp(UINT uHand1,UINT uHand2,BOOL *bResult);
      
      M4RETCODE M4AOblGetPath(UINT uHand,UINT uHandBlq, char *pPath); 
     
      
      M4RETCODE M4AOblWriteFicheroFirst(UINT uHand,char *pszBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout); 
      M4RETCODE M4AOblWriteFicheroNext(UINT uHand,char *pszBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout);
      
      
      M4RETCODE M4AOblWriteBloque(UINT uHand,UINT uHandBlq,char *pszBufferout,UINT uTamMaxBufferOut);
      
      // FUNCIONES DE BUSQUEDAS DE LOS FICHEROS
      
      M4RETCODE M4AOblFindFirstTask(UINT *uHandObl,UINT *uHandMatch);
      M4RETCODE M4AOblFindNextTask(UINT uHandMatch,UINT *uHandObl);
      
      M4RETCODE M4AOblFindFirstExt(char *szMatchExt,UINT *uHandObl,UINT *uHandMatch);
      M4RETCODE M4AOblFindNextExt(UINT uHandMatch,UINT *uHandObl);
      
      M4RETCODE M4AOblFindFirstNombFich(char *szMatchNomb,UINT *uHandObl,UINT *uHandMatch);
      M4RETCODE M4AOblFindNextNombFich(UINT uHandMatch,UINT *uHandObl);    
      
      
      //FUNCIONES PARA TONI
      
      M4RETCODE M4AOblFindHandBlq ( UINT ai_uHand , UINT ai_uHandBlq , char * ai_pszNombreClase  , int & aio_iPosicion , UINT & ao_uHandBlq );
      
      // NUEVO CANARIO
      M4RETCODE M4AOblKillSearch ( UINT ai_uHand , UINT ai_uHandMatch , int ai_iTipoSearch );
      
      // FIN  NUEVO CANARIO
      
    };   


#endif //__ARRAY_OBJETOS__H__




