#ifndef __CONTENEDOR_DE_ARBOLES__H__
#define __CONTENEDOR_DE_ARBOLES__H__                                  


#include "cabecera.h"         
#include "def_bloq.hpp"
#include "analizad.hpp" 
#include "arrhand.hpp"                                 
#include "arraymat.hpp" 

#if !defined(_WINDOWS) && !defined(_UNIX)
#include "hstring.h"  
#endif

class M4ClassLOBL
   {                 
    protected: 
         t_PID hTask; // IDENTIFICARA A LA TAREA EN ESE MOMENTO
         M4ClStringOld szVersion;                 // guardaremos la version de la clase
         M4ClStringOld szNombFich;
         M4ClStringOld szExtFich;
         M4ClStringOld szTitulo;
         M4ClStringOld szBase;

#if !defined(_WINDOWS) && !defined(_UNIX)
         M4ClHString szOut;
#else
		 M4ClStringOld szOut;
#endif

         DWORD uPosOut;
         
         BOOL bdefbloq;                     //no se ha metido toda las cadenas                                          
                                            
         M4AHandles *pHands;                // esta es la clase que contiene a los handles a los bloques y propiedades
         
         M4ClassAnalizador *pAnaliz;
         BOOL banalizador;
         
         UINT hId;                              // Esto lo que lleva  es el HANDLE  del LOBL
         BOOL bdefinido;                        // nos va a decir si esto esta definido
         
         ArrayMatch *pAMProp;     // array de matchs              
         ArrayMatch *pAMBlq;
         
         // posciones de error añadidas el 13 /07 / 94
         
         DWORD m_lPosicionError;
         DWORD m_lLineaError;
         DWORD m_lOffSetError;
         
    
     public:   
         M4ClassBloque *pBloque;
         M4ClassLOBL();
         ~M4ClassLOBL();
         
         M4RETCODE GetPtrBlq ( UINT ai_uHandBlq , M4ClassBloque ** ai_pBloque );
         
         
         
         M4RETCODE SetObl(char *pszVer,char *NombFich,char *ExtFich,char *Titulo,char *Base,UINT hObl);                     
         
         BOOL definidoObl();
         // FUNCIONES DE CONSULTAS A PARTIR DE LOS DATOS
         M4RETCODE OblGetVersion(char **pszVer);  // este devuelve la version que tiene el Obl
         M4RETCODE OblSetVersion(char *szVer);
         M4RETCODE OblGetNombFich(char **szNombFich);
         M4RETCODE OblGetExtFich(char **szNombFich);
         M4RETCODE OblGetTitulo(char **szTitulo);
         M4RETCODE OblGetBase(char **szBase);
         M4RETCODE OblGetClass(char **szCls);     // conseguimos la clase de este archivo
         M4RETCODE OblGetHinst(char **szHis);     // conseguimos la histancia de este objeto
         M4RETCODE OblSetHinst(char *szHis);
         t_PID OblGetTask();     
         M4RETCODE OblSetTask(t_PID hTaskFich);
         
         M4RETCODE OblSetFich( char *pszfichero);  // le pasamos el fichero que nos van a dar de betrive
         
         // FUNCIONES DE CONSULTAS A PARTIR DE LOS HANDLES
         
         M4RETCODE OblGetClassBlqHandle (UINT handle,char **pszClass); // consigue la clase a partir del handle  si no devuelve error
         M4RETCODE OblGetHinstBlqHandle (UINT handle,char **pszHinst); // cons. la Hinst del handle si es bloque si no devuelve error
         M4RETCODE OblSetHinstBlqHandle (UINT handle,char *a_pszHinst);
         
         M4RETCODE OblGetValPropHandle (UINT handle,char **pszVal);    // idem
         M4RETCODE OblGetNomPropHandle (UINT handle,char **pszVal);    // idem
         
         M4RETCODE OblGetPropHandNomb(UINT hBlq,char *szNombProp,char **szVal);
         
         
         // FUNCIONES DE CONSULTAS DEL TIPO
         M4RETCODE OblGetTipoHandNomb(UINT hBlq,char * szNombProp,int * iTipo);
         M4RETCODE OblGetTipoPropHandle(UINT uProp,int *iTipo);
         
         // funcion auxiliar para obtener el bloque a partir del path
         
         M4RETCODE OblGetBlqPath(char *pPath,M4ClassBloque **pBlqOut); // me permite conseguir el puntero del bloque buscado
         
         // funcines de consulta a partir del camino        
                                                 
         M4RETCODE OblGetValProp( char *pPathInst,char *pNomb,char **pszVal);   // paso el path y el nombre de la propiedad
         M4RETCODE OblGetValProp( char *pPathInst,char **pszVal);    // paso el path i separado por punto el nombre de la propiedad

         M4RETCODE OblGetHandProp( char *pPathInst,char *pNomb,UINT *uHand);
         M4RETCODE OblGetHandProp( char *pPathInst,UINT *uHand);
         
         M4RETCODE OblGetHandBlq( char *pPathInst,UINT *uHand);
         
         // funciones para la modificacion en memoria       
         M4RETCODE OblSetPropPath(char *pPathInst,char *pszNomb,char *pszVal,int iTipo);
         M4RETCODE OblSetPropNomb(UINT uHandBlq,char *szNomb,char *szValor,int iTipo);
         M4RETCODE OblSetPropHand( UINT uHandProp,char *pszVal,int iTipo);
         
         M4RETCODE OblAddPropPath(char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp); 
         M4RETCODE OblAddPropHand(UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp); 
         
         M4RETCODE OblDelPropPath(char *pPathIns ,char *pszNomb);
         //M4RETCODE OblDelPropNomb(UINT uHandBlq,char *szNomb);
         M4RETCODE OblDelPropHand(UINT uHandProp);        
         
         // funciones dedicadas a la generacion del arbol
         
         M4RETCODE OblAddBlqPath(char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut); 
         M4RETCODE OblAddBlqHand(UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut); 
         
         M4RETCODE OblDelBlqPath(char *pPathInst);
         M4RETCODE OblDelBlqHand(UINT uHandBlq);    
         M4RETCODE OblDelPropHandNomb(UINT uHandBlq,char *pNomb);
         
         // funciones que se encargan de hacer de busquedas usando los handles
         M4RETCODE OblGetPath(UINT uHandItm,char *szPathout);
         
         // funcines de busquedas en todo el arbol
         M4RETCODE OblBuscProp(BOOL bprimeravez,char *szPathInst,char *szPrpMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandPrp,UINT *uHandMatch);
         M4RETCODE OblBuscBlq(BOOL bprimeravez,char *szPathInst,char *szPathOut,UINT *uHandBlq,UINT *uHandMatch);
         
         M4RETCODE OblFindFirstMatchProp(char *szPathInst,char *szPrpMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandProp,UINT *uHandMatch);
         M4RETCODE OblFindFirstMatchBlq(char *szPathInst,char *szPathOut,UINT *uHanBlq,UINT *uHandMatch);
         M4RETCODE OblFindNextMatchProp(UINT uHandMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandProp);
         M4RETCODE OblFindNextMatchBlq(UINT uHandMatch,char *szPathOut,UINT *uHandBlq);
         
         
        
         // FUNCIONES DE ANALISIS SINTANTICO Y COMPILADOR
         
         M4RETCODE GeneraObl(char *psztexto);
         M4RETCODE GeneraOblBloque(M4ClassBloque *pBloque); //le pasamos el puntero del objet y nos genera un bloque 
         M4RETCODE ConsegirValorPropiedad ( char * szBuffer ,int iBufferLen, int  * iTipo );
      
               
         char *NextPal();          // accede al analizador y pide un nueva palabra
         M4RETCODE SaltoLinea();   // accede al anal. y salta todo lo que encuentra hasta que llega a un STR_FIN_LINEA
         
         M4RETCODE WriteFicheroCompleto(); // este imprime un fichero completo en el buffer miembro szOut;
         
         M4RETCODE WriteFicheroFirst(char *szBuffer,UINT TamMaxszBuffer,UINT *uTamszBuffer); // estas funciones me permiten ir recogiendo toda la informacion de los bloques
         M4RETCODE WriteFicheroNext(char *szBuffer,UINT TamMaxszBuffer,UINT *uTamszBuffer);  // me permite pillar el primero hasta el ultimo;

//#ifndef _WINDOWS
#if !defined(_WINDOWS) && !defined(_UNIX)
        M4RETCODE WriteBloque(UINT uHandBlq,M4ClHString & pszBufferout); // escribe en el buffer el bloque con el handle indicado
#else 
		M4RETCODE WriteBloque(UINT uHandBlq,M4ClStringOld & pszBufferout); 
#endif
         // funcion  añadida el 03 / 08 / 95
         void WriteHandles(M4ClStringOld & strBuffer);
         
         // FUNCION QUE REPARA EL ARRAY DE HANDLES SI SE PRODUCE UN ERROR
         M4RETCODE ReparaObjeto(M4ClassBloque *pBlq); // Revisa todos los handles de una rama a ver si estan correctos
         M4RETCODE DelHandRama(M4ClassBloque *pBlq);  // Borrar todos los enlaces handles de una rama
         M4RETCODE OblDelRama(char *szPath);


         // FUNCIONES DE UTILIDADES
        
         M4RETCODE OblCpyBlq(M4ClassBloque *pBlqSource,M4ClassBloque *pBlqCopia);
         
         // FUNCION DE DEPURACION
         // funcion  añadida el 13 / 07 / 95 JMMS
         M4RETCODE OblGetPoscionError(DWORD * lPos, DWORD  * lLinea,DWORD  * lOffSet);
         
         friend  M4RETCODE OblCpy(M4ClassLOBL *pOblSource,M4ClassLOBL **pOblDestino,UINT hId);
         friend  BOOL OblCmp(M4ClassLOBL *pObl1,M4ClassLOBL *pObl2);
        
         
         // PARA TONI
         M4RETCODE OblFindHandBlq  ( UINT ai_uHandBlq , char *  ai_pszNombreClase  ,  int & aio_iPosicion ,   UINT &  ao_uHandBlq );
         M4RETCODE OblKillSearch ( UINT ai_uHandMatch , int ai_iTipoSearch );
        
        
        
        
        
        
 } ;                           
         
                                           
#endif  // FIN DEL LOBJ.HPP                                           


