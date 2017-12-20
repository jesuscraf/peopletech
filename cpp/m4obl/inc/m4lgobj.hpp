#ifndef _M4LGOBJ_HPP
#define	_M4LGOBJ_HPP

#define T_OBJ_PRP_INT 1
#define T_OBJ_PRP_STR 2
#define T_OBJ_PRP_FEC 3

#ifndef OBL_SUCCESS
    #define OBL_SUCCESS 1
#endif

#ifdef M4RETCODE
#undef M4RETCODE
#endif

#ifndef M4RETCODE 
    #define M4RETCODE int
#endif


#ifdef _UNIX


// contiene el objeto array de obj y tiene todos lo includes necesarios
#include "arrayobj.hpp"

 
// FUNCIONES DEL API

// FUNCIONES DE INICIALIZACION Y CONSTRUCCION
extern "C" M4RETCODE FAR PASCAL _export M4AOblInicializa(char *szNombFich,char *szExtFich,char *szTitulo,char *szBase,UINT *uHand); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblAddTexto(UINT uHand,char *szTexto); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblGeneraObl(UINT uHand,char *szTexto);
//
extern "C" M4RETCODE FAR PASCAL _export M4AOblFinaliza(UINT uHand);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetVersion(UINT uHand,char *szVer,UINT uTamMaxszVer,UINT *uTamszVer);
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetVersion(UINT uHand,char *szVer );


extern "C" M4RETCODE FAR PASCAL _export M4AOblGetExt(UINT uHand,char *szExt,UINT uTamMaxszExt,UINT *uTamszExt);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetNombFich(UINT uHand,char *szNombFich,UINT uTamMaxszNombFich,UINT *uTamszNombFich);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetTit(UINT uHand,char *szTitulo,UINT uTamMaxszTitulo,UINT *uTamszTitulo);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetBase(UINT uHand,char *szBase,UINT uTamMaxszBase,UINT *uTamszBase);

extern "C" M4RETCODE FAR PASCAL _export M4AOblGetClass(UINT uHand,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetHinst(UINT uHand,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst);
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetHinst(UINT uHand,char *szHinst );

extern "C" M4RETCODE FAR PASCAL _export M4AOblGetValPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,char *pszVal,UINT uTamMaxszVal,UINT *uTamszVal);   
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetValPropPath(UINT uHand,char *pPathInst,char *szVal,UINT uTamMaxszval,UINT *uTamszVal);    
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetPropHandNomb(UINT uHand,UINT hBlq,char *szNombProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal); 

extern "C" M4RETCODE FAR PASCAL _export M4AOblGetHandPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,UINT *uHandout);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetHandPropPath(UINT uHand,char *pPathInst,UINT *uHandout);  


extern "C" M4RETCODE FAR PASCAL _export M4AOblGetHandBlq(UINT uHand,char *pPathInst,UINT *uHandout);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetClassBlqHandle(UINT uHand,UINT uHandBlq,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass); 

extern "C" M4RETCODE FAR PASCAL _export M4AOblGetHinstBlqHandle(UINT uHand,UINT uHandBlq,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetHinstBlqHandle(UINT uHand,UINT uHandBlq,char *szHinst );
 
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetValPropHandle(UINT uHand,UINT uHandProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal);    
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetNomPropHandle(UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb);    
                                        
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetTipoPropHandle(UINT uHand,UINT uHandProp,int *iVal);  
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetTipoPropHandNomb(UINT uHand,UINT uBlq,char *szNombProp,int *iVal); 
                                        

extern "C" M4RETCODE FAR PASCAL _export M4AOblGetPropHandle (UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal);
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetPropPath(UINT uHand,char *pszPathInst,char *pszNomb,char *pszValor,int iTipo); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetPropNomb(UINT uHand,UINT uHandBlq,char *szNomb,char *szValor,int iTipo);
extern "C" M4RETCODE FAR PASCAL _export M4AOblSetPropHand(UINT uHand,UINT uHandProp,char *pszValor,int iTipo); 

extern "C" M4RETCODE FAR PASCAL _export M4AOblAddPropPath(UINT uHand, char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp);
extern "C" M4RETCODE FAR PASCAL _export M4AOblAddPropHand(UINT uHand,UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp);
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelPropPath(UINT uHand,char *pPathInst,char *pszNomb);
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelPropHand(UINT uHand,UINT uHandBlq);
extern "C" M4RETCODE FAR PASCAL _export M4AOblAddBlqPath(UINT uHand,char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblAddBlqHand(UINT uHand,UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut);
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelBlqPath(UINT uHand, char *pPathInst); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelPropPath(UINT uHand, char *pPathInst,char *szNomb);
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelBlqHand(UINT uHand,UINT uHandBlq);
extern "C" M4RETCODE FAR PASCAL _export M4AOblDelRama(UINT uHand , char *szPath);

// funciones de busquedas
extern "C" M4RETCODE FAR PASCAL _export M4AOblFirstHndProp(UINT uHand,char *szMatchPath,char *szMatchProp,UINT *uHandPrp,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export M4AOblNextHndProp(UINT uHand,UINT uHandMatch,UINT *uHandPrp);
extern "C" M4RETCODE FAR PASCAL _export M4AOblFirstHndBlq(UINT uHand,char *szMatchPath,UINT *uHandBlq,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export M4AOblNextHndBlq(UINT uHand,UINT uHandMatch,UINT *uHandBlq);                                                                             

extern "C" M4RETCODE FAR PASCAL _export M4AOblFirstProp(UINT uHand,char *szMatchPath,char *szMatchProp,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,
                                                        UINT *uHandPrp,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export M4AOblNextProp(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamszMaxNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,UINT *uHandPrp);
extern "C" M4RETCODE FAR PASCAL _export M4AOblFirstBlq(UINT uHand,char *szMatchPath,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq,UINT *uHandMatch); 
extern "C" M4RETCODE FAR PASCAL _export M4AOblNextBlq(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq);  
   


extern "C" M4RETCODE FAR PASCAL _export M4AOblGetPath(UINT uHand,UINT uHandBlq, char *pPath);

// FUNCIONES DE ESCRITURA DE BLOQUES
extern "C" M4RETCODE FAR PASCAL _export M4AOblWriteFicheroCompleto(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout); 

extern "C" M4RETCODE FAR PASCAL _export M4AOblWriteFicheroFirst(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout);
extern "C" M4RETCODE FAR PASCAL _export M4AOblWriteFicheroNext(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout);
extern "C" M4RETCODE FAR PASCAL _export M4AOblWriteBloque(UINT uHand,UINT uHandBlq,char *szBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout);

// FUNCION DE GESTION DE ERROR
extern "C" M4RETCODE FAR PASCAL _export M4AOblError(int iCodErr,char *szError,UINT uTamszError);
extern "C" M4RETCODE FAR PASCAL _export M4AOblGetPosicionError(UINT uHand,unsigned long *lPos, unsigned long  * lLinea, unsigned long * lOffSet);

// FUNCIONES DE UTILIDADES
extern "C" M4RETCODE FAR PASCAL _export  M4AOblCpy(UINT uHandSource,UINT  *uHandDest);

extern "C" M4RETCODE FAR PASCAL _export  M4AOblCpyBlq ( UINT uHandSource , UINT uHandBlqSrc , UINT uHandDest , UINT uHandBlqDest);

extern "C" M4RETCODE FAR PASCAL _export  M4AOblCmp(UINT uHand1,UINT uHand2,BOOL *bResult);


// FUNCIONES DE BUSQUEDAS DE LOS FICHEROS
      
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindFirstTask(UINT *uHandObl,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindNextTask(UINT uHandMatch,UINT *uHandObl);
      
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindFirstExt(char *szMatchExt,UINT *uHandObl,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindNextExt(UINT uHandMatch,UINT *uHandObl);
      
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindFirstNombFich(char *szMatchNomb,UINT *uHandObl,UINT *uHandMatch);
extern "C" M4RETCODE FAR PASCAL _export  M4AOblFindNextNombFich(UINT uHandMatch,UINT *uHandObl);


// Funcion especifica de toni.
// la primera vez que se llame se pasa un cero y las siguientes el handle que corresponda.
extern "C" M4RETCODE FAR PASCAL _export M4AOblFindHandBlq (UINT ai_uHand, UINT ai_uHandBlq , char * ai_pszNombreClase  , int & aio_iPosicion , UINT & ao_uHandBlq );   


// NUEVO CANARIO
extern "C" M4RETCODE FAR PASCAL _export M4AOblKillSearch (UINT ai_uHand , UINT ai_uHandMathc , int ai_iTipoSearch );
// FIN NUEVO CANARIO   
   
/// DEFINES DE ERROR DE LA OBJ
#ifdef __NOFINIDOENM4LGOBJ
          /// Si se quiere usar en 
          #define IDS_PRUEBA                      1
          #define IDS_NO_PERMISO                  2000
          #define IDS_RTE_NO_MEMORIA              2100
          #define IDS_RTE_HAND_MATCH              2110
          #define IDS_RTE_HAND_NO_MEMORIA         2140
          #define IDS_RTE_AHAND_HANDLE_BORRADO    2150
          #define IDS_RTE_NUMMAXHANDLES           2151
          #define IDS_STRING2152                  2152
          #define IDS_RTE_BLQ_ERROR_ESCRITURA     2160
          #define IDS_RTE_OBJ_NULL                2161
          #define IDS_RTE_LOBJ_PARAM_NULL         2162
          #define IDS_RTE_ABLQ_NO_VACIO           2170
          #define IDS_RTE_ABLQ_NO_FOUND           2175
          #define IDS_RTE_ABLQ_INDEX_OVERFLOW     2180
          #define IDS_RTE_VERSION                 2181
          #define IDS_RTE_NO_FIN_LINEA            2183
          #define IDS_RTE_NOBEGIN                 2184
          #define IDS_RTE_ESPERAIGUAL             2188
          #define IDS_RTE_NO_ENCONTRADO_REM       2189
          #define IDS_RTE_CASTING                 2299
          #define IDS_RTE_PROP_CAD_NULL           2300
          #define IDS_RET_STR_SIN_CARACTERES      2305
          #define IDS_RTE_PROP_CADENA_LARGA       2310
          #define IDS_RTE_PROP_HAND               2315
          #define IDS_RTE_APROP_POS               2320
          #define IDS_RTE_APROP_STR_NULL          2330
          #define IDS_RTE_APROP_VACIO             2331
          #define IDS_OBJ_NOT_FOUND               2340
          #define IDS_FALTA_CLASE                 2341
          #define IDS_FALTA_INST                  2342
          #define IDS_FIN_OBL_ERRONEO             2343
          #define IDS_TIPO_BUSQ_EQUIV             2345
          #define IDS_RTE_PROP_TIPO               2350
          #define IDS_RTE_BLQ_CONSTRUIDO          2500
          #define IDS_RTE_LOBL_NO_DEFINIDO        3100
          #define IDS_RTE_LOBL_NO_BLOQUE          3110
          #define IDS_OBL_BORRAR_RAIZ             3115
          #define IDS_RTE_AHAND_HANDLE_OVERFLOW   3120
          #define IDS_RTE_OBL_TIPO_OBJ            3121
          #define IDS_RTE_BLQ_NO_PATH             3140
          #define IDS_RTE_BLQ_PATH_NO_FOUND       3141
          #define IDS_RTE_PATH                    3143
          #define IDS_RTE_NO_COMILLAS             3190
          #define IDS_NUM_INCORECTO               3191
          #define IDS_RTE_OBL_HANDLE_INCORECTO    3200
          #define IDS_RTE_PARAM_NULL              3201
          #define IDS_RTE_TAM_STR_GRANDE          3210
          #define IDS_RTE_TAM_STR_MENUDO          3211
          #define IDS_RTE_HAND_ERRONEO            3220
          #define IDS_RTE_FICHERO_NULL            3230
          #define IDS_RES_TRUNCADO                3300
          #define IDS_RTE_WRITE_PRIMERO           3301
          #define IDS_RTE_HAND_PATH               3400
          #define IDS_RTE_REPARAR                 3450
          #define IDS_PROP_CORRUPTA               10000
          #define IDS_RTE_HAND_MATCH_VACIO        10001
#endif




// FIN DE LOS DEFINES      


#else

#ifdef _WINDOWS
#include <windows.h>
#include "m4obl_dll.hpp"
#endif

   

// FUNCIONES DEL API

// FUNCIONES DE INICIALIZACION Y CONSTRUCCION
int M4_DECL_M4OBL M4AOblInicializa(char *szNombFich, char *szExtFich, char *szTitulo, char *szBase, UINT *uHand);
int M4_DECL_M4OBL M4AOblAddTexto(UINT uHand,char *szTexto); 
int M4_DECL_M4OBL M4AOblGeneraObl(UINT uHand,char *szTexto);
//
int M4_DECL_M4OBL M4AOblFinaliza(UINT uHand);
int M4_DECL_M4OBL M4AOblGetVersion(UINT uHand,char *szVer,UINT uTamMaxszVer,UINT *uTamszVer);

int M4_DECL_M4OBL M4AOblGetExt(UINT uHand,char *szExt,UINT uTamMaxszExt,UINT *uTamszExt);
int M4_DECL_M4OBL M4AOblGetNombFich(UINT uHand,char *szNombFich,UINT uTamMaxszNombFich,UINT *uTamszNombFich);
int M4_DECL_M4OBL M4AOblGetTit(UINT uHand,char *szTitulo,UINT uTamMaxszTitulo,UINT *uTamszTitulo);
int M4_DECL_M4OBL M4AOblGetBase(UINT uHand,char *szBase,UINT uTamMaxszBase,UINT *uTamszBase);

int M4_DECL_M4OBL M4AOblGetClass(UINT uHand,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass);
int M4_DECL_M4OBL M4AOblGetHinst(UINT uHand,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst);

int M4_DECL_M4OBL M4AOblGetValPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,char *pszVal,UINT uTamMaxszVal,UINT *uTamszVal);   
int M4_DECL_M4OBL M4AOblGetValPropPath(UINT uHand,char *pPathInst,char *szVal,UINT uTamMaxszval,UINT *uTamszVal);    
int M4_DECL_M4OBL M4AOblGetPropHandNomb(UINT uHand,UINT hBlq,char *szNombProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal); 

int M4_DECL_M4OBL M4AOblGetHandPropPathNomb(UINT uHand,char *pPathInst,char *pNomb,UINT *uHandout);
int M4_DECL_M4OBL M4AOblGetHandPropPath(UINT uHand,char *pPathInst,UINT *uHandout);  


int M4_DECL_M4OBL M4AOblGetHandBlq(UINT uHand,char *pPathInst,UINT *uHandout);
int M4_DECL_M4OBL M4AOblGetClassBlqHandle(UINT uHand,UINT uHandBlq,char *szClass,UINT uTamMaxszClass,UINT *uTamszClass); 
int M4_DECL_M4OBL M4AOblGetHinstBlqHandle(UINT uHand,UINT uHandBlq,char *szHinst,UINT uTamMaxszHinst,UINT *uTamszHinst); 

int M4_DECL_M4OBL M4AOblGetValPropHandle(UINT uHand,UINT uHandProp,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal);    
int M4_DECL_M4OBL M4AOblGetNomPropHandle(UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb);    
                                        
int M4_DECL_M4OBL M4AOblGetTipoPropHandle(UINT uHand,UINT uHandProp,int *iVal);  
int M4_DECL_M4OBL M4AOblGetTipoPropHandNomb(UINT uHand,UINT uBlq,char *szNombProp,int *iVal); 
                                        

int M4_DECL_M4OBL M4AOblGetPropHandle (UINT uHand,UINT uHandProp,char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal);
int M4_DECL_M4OBL M4AOblSetPropPath(UINT uHand,char *pszPathInst,char *pszNomb,char *pszValor,int iTipo); 
int M4_DECL_M4OBL M4AOblSetPropNomb(UINT uHand,UINT uHandBlq,char *szNomb,char *szValor,int iTipo);
int M4_DECL_M4OBL M4AOblSetPropHand(UINT uHand,UINT uHandProp,char *pszValor,int iTipo); 

int M4_DECL_M4OBL M4AOblAddPropPath(UINT uHand, char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp);
int M4_DECL_M4OBL M4AOblAddPropHand(UINT uHand,UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp);
int M4_DECL_M4OBL M4AOblDelPropPath(UINT uHand,char *pPathInst,char *pszNomb);
int M4_DECL_M4OBL M4AOblDelPropHand(UINT uHand,UINT uHandBlq);
int M4_DECL_M4OBL M4AOblAddBlqPath(UINT uHand,char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut); 
int M4_DECL_M4OBL M4AOblAddBlqHand(UINT uHand,UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut);
int M4_DECL_M4OBL M4AOblDelBlqPath(UINT uHand, char *pPathInst); 
int M4_DECL_M4OBL M4AOblDelPropPath(UINT uHand, char *pPathInst,char *szNomb);
int M4_DECL_M4OBL M4AOblDelBlqHand(UINT uHand,UINT uHandBlq);
int M4_DECL_M4OBL M4AOblDelRama(UINT uHand , char *szPath);

// funciones de busquedas
int M4_DECL_M4OBL M4AOblFirstHndProp(UINT uHand,char *szMatchPath,char *szMatchProp,UINT *uHandPrp,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblNextHndProp(UINT uHand,UINT uHandMatch,UINT *uHandPrp);
int M4_DECL_M4OBL M4AOblFirstHndBlq(UINT uHand,char *szMatchPath,UINT *uHandBlq,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblNextHndBlq(UINT uHand,UINT uHandMatch,UINT *uHandBlq);                                                                             

int M4_DECL_M4OBL M4AOblFirstProp(UINT uHand,char *szMatchPath,char *szMatchProp,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamMaxszNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,
                                                        UINT *uHandPrp,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblNextProp(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,
                                                        char *szNomb,UINT uTamszMaxNomb,UINT *uTamszNomb,char *szVal,UINT uTamMaxszVal,UINT *uTamszVal,UINT *uHandPrp);
int M4_DECL_M4OBL M4AOblFirstBlq(UINT uHand,char *szMatchPath,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq,UINT *uHandMatch); 
int M4_DECL_M4OBL M4AOblNextBlq(UINT uHand,UINT uHandMatch,char *szPathOut,UINT uTamMaxszPathOut,UINT *uTamszPathOut,UINT *uHandBlq);  
   


int M4_DECL_M4OBL M4AOblGetPath(UINT uHand,UINT uHandBlq, char *pPath);

// FUNCIONES DE ESCRITURA DE BLOQUES
int M4_DECL_M4OBL M4AOblWriteFicheroCompleto(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout); 

int M4_DECL_M4OBL M4AOblWriteFicheroFirst(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout);
int M4_DECL_M4OBL M4AOblWriteFicheroNext(UINT uHand,char *szBufferout,UINT uTamMAxBufferout,UINT *uTamBufferout);
int M4_DECL_M4OBL M4AOblWriteBloque(UINT uHand,UINT uHandBlq,char *szBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout);

// FUNCION DE GESTION DE ERROR
int M4_DECL_M4OBL M4AOblError(int iCodErr,char *szError,UINT uTamszError);
int M4_DECL_M4OBL M4AOblGetPosicionError(UINT uHand,unsigned long *lPos, unsigned long  * lLinea, unsigned long * lOffSet);
                                        
// FUNCIONES DE UTILIDADES
int M4_DECL_M4OBL M4AOblCpy(UINT uHandSource,UINT  *uHandDest);
int M4_DECL_M4OBL M4AOblCmp(UINT uHand1,UINT uHand2,BOOL *bResult);


// FUNCIONES DE BUSQUEDAS DE LOS FICHEROS
      
int M4_DECL_M4OBL M4AOblFindFirstTask(UINT *uHandObl,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblFindNextTask(UINT uHandMatch,UINT *uHandObl);
      
int M4_DECL_M4OBL M4AOblFindFirstExt(char *szMatchExt,UINT *uHandObl,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblFindNextExt(UINT uHandMatch,UINT *uHandObl);
      
int M4_DECL_M4OBL M4AOblFindFirstNombFich(char *szMatchNomb,UINT *uHandObl,UINT *uHandMatch);
int M4_DECL_M4OBL M4AOblFindNextNombFich(UINT uHandMatch,UINT *uHandObl);




   
   
/// DEFINES DE ERROR DE LA OBJ

#define IDS_NO_PERMISO                  2000
#define IDS_RTE_NO_MEMORIA              2100
#define IDS_RTE_HAND_MATCH              2110
#define IDS_RTE_HAND_NO_MEMORIA         2140
#define IDS_RTE_AHAND_HANDLE_BORRADO    2150
#define IDS_RTE_NUMMAXHANDLES           2151
#define IDS_STRING2152                  2152
#define IDS_RTE_BLQ_ERROR_ESCRITURA     2160
#define IDS_RTE_OBJ_NULL                2161
#define IDS_RTE_LOBJ_PARAM_NULL         2162
#define IDS_RTE_ABLQ_NO_VACIO           2170
#define IDS_RTE_ABLQ_NO_FOUND           2175
#define IDS_RTE_ABLQ_INDEX_OVERFLOW     2180
#define IDS_RTE_VERSION                 2181
#define IDS_RTE_NO_FIN_LINEA            2183
#define IDS_RTE_NOBEGIN                 2184
#define IDS_RTE_ESPERAIGUAL             2188
#define IDS_RTE_NO_ENCONTRADO_REM       2189
#define IDS_RTE_CASTING                 2299
#define IDS_RTE_PROP_CAD_NULL           2300
#define IDS_RET_STR_SIN_CARACTERES      2305
#define IDS_RTE_PROP_CADENA_LARGA       2310
#define IDS_RTE_PROP_HAND               2315
#define IDS_RTE_APROP_POS               2320
#define IDS_RTE_APROP_STR_NULL          2330
#define IDS_RTE_APROP_VACIO             2331
#define IDS_OBJ_NOT_FOUND               2340
#define IDS_FALTA_CLASE                 2341
#define IDS_FALTA_INST                  2342
#define IDS_FIN_OBL_ERRONEO             2343
#define IDS_TIPO_BUSQ_EQUIV             2345
#define IDS_RTE_PROP_TIPO               2350
#define IDS_RTE_BLQ_CONSTRUIDO          2500
#define IDS_RTE_LOBL_NO_DEFINIDO        3100
#define IDS_RTE_LOBL_NO_BLOQUE          3110
#define IDS_OBL_BORRAR_RAIZ             3115
#define IDS_RTE_AHAND_HANDLE_OVERFLOW   3120
#define IDS_RTE_OBL_TIPO_OBJ            3121
#define IDS_RTE_BLQ_NO_PATH             3140
#define IDS_RTE_BLQ_PATH_NO_FOUND       3141
#define IDS_RTE_PATH                    3143
#define IDS_RTE_NO_COMILLAS             3190
#define IDS_NUM_INCORECTO               3191
#define IDS_RTE_OBL_HANDLE_INCORECTO    3200
#define IDS_RTE_PARAM_NULL              3201
#define IDS_RTE_TAM_STR_GRANDE          3210
#define IDS_RTE_TAM_STR_MENUDO          3211
#define IDS_RTE_HAND_ERRONEO            3220
#define IDS_RTE_FICHERO_NULL            3230
#define IDS_RES_TRUNCADO                3300
#define IDS_RTE_WRITE_PRIMERO           3301
#define IDS_RTE_HAND_PATH               3400
#define IDS_RTE_REPARAR                 3450
#define IDS_PROP_CORRUPTA               10000
#define IDS_RTE_HAND_MATCH_VACIO        10001

// FIN DE LOS DEFINES      


#endif
      
#endif
