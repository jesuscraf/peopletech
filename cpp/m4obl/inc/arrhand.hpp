#ifndef __ARRAY_HANDLES__H_
#define __ARRAY_HANDLES__H_ 
#include "cabecera.h" 
  
 
typedef struct tag_HAND
   {
     int tipo;     //T_PROPIEDAD , T_BLOQUE
     void *ptr;
   } HAND; 
 
 


class M4AHandles
  {
   protected: 
    UINT m_uNumHand; // nos indica el numero de celdas usadas 
    UINT libres;  // indica el numero de celdas no usadas todavia
    HAND * m_pContHand[ NUMERO_SEGMENTOS ];    
        
   public:
    M4AHandles();
    ~M4AHandles();
    M4RETCODE HandleLibre(UINT *ulibre);
    M4RETCODE AddHandle(int iTipo,void *puntero,UINT *uhand);
    M4RETCODE TipoHandle(UINT uid,int *piTipo); 
    M4RETCODE GetPtrProp(UINT uhId,void **ptrProp);
    M4RETCODE GetPtrBlq(UINT uhId,void **ptrBlq);
    M4RETCODE DelPtrProp(UINT uhId);
    M4RETCODE DelPtrBlq(UINT uhId);               
    // funcion para poder repara un handle borrado sin querer
    BOOL RepararHand(UINT uhId,void *ptr,int iTipo);
    
    BOOL HandleNulo(UINT uhId);
    UINT NumeroHandles();   
    
    
    int DentroDelLimite ( UINT ai_uHand );
    void * GetPtr( UINT ai_uHand );
    int GetTipo ( UINT ai_uHand );
    HAND * GetHand ( UINT ai_uHand );
    
    
    
  };  

 
 #endif // __ARRAY_HANDLES__H_


