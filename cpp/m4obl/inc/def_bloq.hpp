#ifndef __DEFINICION_BLOQUES__
#define __DEFINICION_BLOQUES__
 
 #include "cabecera.h"
 #include "arraprop.hpp" 
 #include "hstring.h"
 
 class M4ClassBloque;

 class M4ArrayBloques 
 {
   
  protected: 
   M4ClassBloque **ContBloques;
   UINT numbloq;    //nos indica el numero de  bloques que hay en el array
   BOOL  bordenbloque; // nos indica si el array está ordenado o no
   UINT uBlqLibres;
   void PermutarBloque(int izq,int der);
   void M4ABlQuicksort(int inf,int sup); 
   
   
  public:                 
   M4ArrayBloques();
   ~M4ArrayBloques();
   
   
   UINT NumBloques(); 
   M4ClassBloque *PtrBloquen(UINT i);
   BOOL M4ABlOrdenado();
   
   
   M4RETCODE M4ABAddNuevoBlq(char *szHins,char *szClass,UINT hident,M4ClassBloque *pAnterior,M4ClassBloque **pBloque); //devuelve el puntero 
   M4RETCODE M4ABAddNuevoBlq(M4ClassBloque *pAnterior,M4ClassBloque **pBloque);
        
   M4RETCODE M4ABlDelBlq(char *pszClass,char *pszHinst); //borra el bloque cuya instancia es pszHinst
   M4RETCODE M4ABlDelBlqn(char *pszClass,char *pszHinst); // borra el bloque y no comprueba si tiene propiedades o subloques 
   M4RETCODE M4ABlGetpBlqCI(char * szClase,char *szInstancia,M4ClassBloque **blq); // consigue el puntero del bloque que coincide 
                                                                                    // con Instancia clase
   M4RETCODE M4ABlGetpBlqn(UINT uPos,M4ClassBloque **pBlq);                                                                                 
   M4RETCODE M4ABlGetHand(char *szClase,char *szInstancia,UINT *hIdent);   // cons. el handle por el nombre 
   
   M4RETCODE M4ABlGetHandpBlq(M4ClassBloque *pBloque,UINT *hIdent); //cons el handle dando el puentero al bloque
   M4RETCODE M4ABlGetHinstpBlq(M4ClassBloque *pBloque,char **szHinst); // cons. la hinst dando el puntero al bloque
   M4RETCODE M4ABlGetClasspBlq(M4ClassBloque *pBloque,char **szClass);  // cons. la clase dando el putero al bloque
   M4RETCODE M4ABlOrdenar();
   int M4ABlBuscarHinst(char *hHinst); // devuelve el numero donde esta localizado si no lo ha encontrado      
   int M4ABlBuscar(char *szClase,char *szInstancia); // devuelve el indice donde se encuentra el bloque
 
 };


 
 class M4ClassBloque : public M4ArrayPropiedades,public M4ArrayBloques
 {
   protected:
     char HinstBl[MAX_TAM_IDENT];
     char ClassBl[MAX_TAM_IDENT];
     BOOL bdefinido; // me indica si el bloque ya tiene asignado la hinstancia,classe y handle
     UINT hId;
     M4ClassBloque *pPadre;
     
     
   public:
       M4ClassBloque();
       ~M4ClassBloque(); 
      
      BOOL M4CBVacio(); 
       
      // Esta funcion asigna la hinstancia,clase y devuelve el puntero de donde lo ha asignado
      
      M4RETCODE M4CBSetBloque(char *szClass,char *szHins,UINT hIdent,M4ClassBloque **pBloq);
      
      M4RETCODE M4CBSetPadre(M4ClassBloque *pFather);
      M4RETCODE M4CBGetPadre(M4ClassBloque **pFather);
      
      M4RETCODE M4CBGetHinst(char **Hinst);    // la instancia viene siendo el nombre
      M4RETCODE M4CBSetHinst(char *a_pszHinst);
      
      M4RETCODE M4CBGetClass(char **Class);    // la clase viene siendo el tipo
      M4RETCODE M4CBGetHandle(UINT *hIdent);
      M4RETCODE M4CBAddBlqArray(char *szHins,char *szClass,UINT hident,M4ClassBloque **pBloque); 
      BOOL M4CBDefinido() { return bdefinido; };
         
      M4RETCODE M4CBGetBlqPath(char *path,M4ClassBloque **pBlqOut);   
      
      
      
      friend int ComparaBloque(M4ClassBloque *ele1,M4ClassBloque *ele2);
       
#if !defined(_WINDOWS) && !defined(_UNIX)
	  friend M4RETCODE WriteRecursivo(int nivel,M4ClassBloque *ptbloque,M4ClHString & pszBufferOut) ;
#else
	  friend M4RETCODE WriteRecursivo(int nivel,M4ClassBloque *ptbloque,M4ClStringOld & pszBufferOut);      
#endif
      
      friend BOOL BlqCmp(M4ClassBloque *pBlq1,M4ClassBloque *pBlq2);
                      
 };
                     
#endif //  __DEFINICION_BLOQUES__


