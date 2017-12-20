#include "arrhand.hpp"
  
   
  M4AHandles::M4AHandles()
   {
     m_uNumHand=0;                
     
     libres=0;    
     
     for ( int j = 0 ; j < NUMERO_SEGMENTOS ; j ++ ) {
          
           m_pContHand[j] = NULL;
      }     
     
   }; 
  
  M4AHandles::~M4AHandles()   
   {
     int i;
     int j;
     
     for ( j = 0 ; j < NUMERO_SEGMENTOS; j++  ) {
     
      if ( m_pContHand[j] != NULL  ) {
       
          for (i=0 ; i< NUM_ADD_HANDLES ; i++)   { 
          
                    m_pContHand[j][i].ptr=NULL;
                    
                    m_pContHand[j][i].tipo=0;
           }                  
           
          delete m_pContHand[j];     
      }    
    };
    
  };          
   
   

 M4RETCODE M4AHandles::HandleLibre(UINT *ulibre)
   {
     /* deuelve 
        -1                   -> no hay memoria
        devuelve el handle   -> */    
     
//     UINT i;
//     HAND *HANDaux;  
    // UINT uTam;
    
     
    
      
//     if ( m_uNumHand > NUM_ADD_HANDLES ) 
     
     if ( !DentroDelLimite( m_uNumHand ) )
     
          return IDS_RTE_NUMMAXHANDLES;
          
    
    if ( m_uNumHand > 31000 ) {
       
     for ( int iCont = m_uNumHand - 1 ;  iCont >  0  ;  iCont--  )  {
     

       if ( GetPtr( iCont ) == NULL ) {
        
            *ulibre = iCont;
            
            return EXITO;
       }
      
     }
    } 
/*    
    if (libres==0)
       {                         
         HANDaux = new  HAND [numhand + NUM_ADD_HANDLES] ;
         
         if (HANDaux==NULL) return IDS_RTE_HAND_NO_MEMORIA; // no ha habido memoria suficiente
         
         libres=NUM_ADD_HANDLES;
         
         // igualo  los que tenia antes  realoco
         for (i=0;i < numhand ;i++) 
           {
             
             HANDaux[i].tipo=ContHand[i].tipo;
             HANDaux[i].ptr=ContHand[i].ptr;
             
           };      
                           
         // inicializo los que acabo de añadir  
         for (i= numhand; i < numhand + libres ;i++) 
           {
             HANDaux[i].tipo=0;
             HANDaux[i].ptr=NULL;
           }; 
           delete []ContHand;
           ContHand=HANDaux;
           HANDaux=NULL; 
        };
      *ulibre=numhand + 1;
      
      */
      
     if ( !DentroDelLimite( m_uNumHand + 1 ) ) {
     
          return IDS_RTE_NUMMAXHANDLES;
     }     
      
     //m_uNumHand ++;
     
     *ulibre = m_uNumHand + 1;
      
     return EXITO;
   };
                  
       
   M4RETCODE M4AHandles::AddHandle(int iTipo,void *puntero,UINT *uhand)
     {
       UINT ulibre;          
       int retdev;
       
       HAND *pHandle;
       // esta funcion consigue un hueco y me devuelve el handle  que le corresponde
        
       retdev=HandleLibre(&ulibre); // primer handle libre que es uno mas donde lo voy a colocar
       if (retdev!=EXITO) return retdev;
       
       pHandle = GetHand ( ulibre ) ;
       
       //ContHand[ulibre].tipo = iTipo;      // asigno valores
       //ContHand[ulibre].ptr = puntero;
       
       pHandle -> tipo = iTipo;      // asigno valores
       pHandle -> ptr = puntero;
       
       if ( ulibre >= m_uNumHand ) {
          
          m_uNumHand++;                            // hay uno mas
          libres--;                             // hay uno memos libre
          
       }   
       *uhand=ulibre;                        // devuelvo el handle donde se ha puesto
       return EXITO;
     };   
     
         
  M4RETCODE M4AHandles::TipoHandle(UINT uhId,int *piTipo)
   {                              
      int iTipo;
     // sabemos que el idice del array es el handle de lo que esta apuntando menos uno
     if (uhId > m_uNumHand || uhId < 0) return IDS_RTE_AHAND_HANDLE_OVERFLOW;
     
     iTipo = GetTipo ( uhId ); 
      
     //if (ContHand[uhId].tipo != T_BLOQUE && ContHand[uhId].tipo != T_PROPIEDAD)
     if ( iTipo != T_BLOQUE && iTipo != T_PROPIEDAD ) {
      
         return IDS_PROP_CORRUPTA;
         
     }    
     
     *piTipo = iTipo;
     
     return EXITO;
   };         
  
  
   M4RETCODE M4AHandles::GetPtrProp(UINT uhId,void **ptrProp)
   { 
    int iTipo;     // variable indica si es propiedad o bloque
    int retdev;
    retdev=TipoHandle(uhId,&iTipo);
    if ( retdev!= EXITO ) return retdev;
    
    if (HandleNulo(uhId)) return IDS_RTE_AHAND_HANDLE_BORRADO;
    
    if (iTipo != T_PROPIEDAD ) return IDS_RTE_OBL_TIPO_OBJ;
    //*ptrProp = ContHand[uhId].ptr;
    *ptrProp = GetPtr ( uhId );
    return EXITO;
   };
   
   M4RETCODE M4AHandles::GetPtrBlq(UINT uhId,void **ptrBlq)
   {
    int iTipo;     // variable indica si es propiedad o bloque
    int retdev;
    retdev=TipoHandle(uhId,&iTipo);
    if ( retdev!= EXITO ) return retdev;
    
    if (HandleNulo(uhId)) return IDS_RTE_AHAND_HANDLE_BORRADO; 
    
    if (iTipo != T_BLOQUE) return IDS_RTE_OBL_TIPO_OBJ;
    
    //*ptrBlq = ContHand[uhId].ptr;
    
    *ptrBlq = GetPtr ( uhId );
    
    return EXITO;
   };                     
   
   
   M4RETCODE M4AHandles::DelPtrProp(UINT uhId)
    {
      int iTipo;     // variable indica si es propiedad o bloque
      int retdev;
      HAND * pHand;
      
      retdev=TipoHandle(uhId,&iTipo);
      if ( retdev!= EXITO ) return retdev;
    
      if (HandleNulo(uhId)) return IDS_RTE_AHAND_HANDLE_BORRADO;
    
      if (iTipo != T_PROPIEDAD ) return IDS_RTE_OBL_TIPO_OBJ;
      
      //ContHand[uhId].ptr = NULL;
      
      pHand = GetHand ( uhId );
      
      pHand -> ptr = NULL;
      
      pHand -> tipo = 0;
      
      return EXITO;
    };
   
   M4RETCODE M4AHandles::DelPtrBlq(UINT uhId)
    {
      int iTipo;     // variable indica si es propiedad o bloque
      int retdev;
      HAND *pHand;
      
      retdev=TipoHandle(uhId,&iTipo);
      if ( retdev!= EXITO ) return retdev;
    
      if (HandleNulo(uhId)) return IDS_RTE_AHAND_HANDLE_BORRADO; 
    
      if (iTipo != T_BLOQUE) return IDS_RTE_OBL_TIPO_OBJ;
      //ContHand[uhId].ptr = NULL;
      
      pHand = GetHand ( uhId );
      
      pHand -> ptr = NULL;
      
      pHand -> tipo = 0;
      
      return EXITO;
    };
       
  BOOL M4AHandles::RepararHand(UINT uhId,void *ptr,int iTipo)
    {
     
        HAND * pHand;
      
      // en caso de que se pase mal el handle se devuelve false
      //if (uhId>numhand || uhId<0) return FALSE;
      if (uhId > m_uNumHand || uhId < 0) {

          return FALSE;
          
      } 
    
           
      pHand = GetHand ( uhId );

      // en caso de que este a null este handle se repara y se devuelve TRUE
      if (HandleNulo(uhId))
        { 
         
        // ContHand[uhId].ptr= ptr;
        // ContHand[uhId].tipo = iTipo;
      
           pHand -> ptr = ptr;
      
           pHand -> tipo = uhId;

         return TRUE;
        };
      // COMPROBAMOS QUE SI NO ES NULO LOS DATOS SON CONSISTETES CON LOS QUE SE LE HA PASADO
      
      
      //if (ContHand[uhId].ptr == ptr && ContHand[uhId].tipo == iTipo ) return TRUE;
      
      if ( pHand -> ptr == ptr && pHand -> tipo == iTipo ) return TRUE;
      return FALSE;
    };
  
  
       
       
   BOOL M4AHandles::HandleNulo(UINT uhId)
   {
     if (uhId>m_uNumHand || uhId<0) return TRUE;
     //return (ContHand[uhId].ptr==NULL);
     return ( GetPtr ( uhId ) == NULL);
   };           
   
  UINT M4AHandles::NumeroHandles() 
   {
    return m_uNumHand;
   }
   
   
   
   
   
  void * M4AHandles::GetPtr ( UINT ai_uHand ) 
     {
     
         UINT uSegmento;
         UINT uOffSet;
//         UINT i , j;
         
         
         uSegmento = ai_uHand / NUM_ADD_HANDLES;
         
         uOffSet = ai_uHand %  NUM_ADD_HANDLES;    
         
         if ( uSegmento >= NUMERO_SEGMENTOS ) {
         
            return NULL;
         }
         
         if ( m_pContHand [ uSegmento ] == NULL ) {
         
             m_pContHand [ uSegmento ] = new HAND [ NUM_ADD_HANDLES ] ;
             
             for ( int iCont =0 ; iCont < NUM_ADD_HANDLES ; iCont ++ ) {
              
                 m_pContHand [ uSegmento ] [ iCont ].ptr = NULL ;
                 
                 m_pContHand [ uSegmento ] [ iCont ].tipo = 0 ;
             
             }
         
         }   
         
         return m_pContHand [ uSegmento ][ uOffSet].ptr;
     }


int M4AHandles::GetTipo ( UINT ai_uHand ) 
     {
     
         UINT uSegmento;
         UINT uOffSet;
//         UINT i ;
         
         
         uSegmento = ai_uHand / NUM_ADD_HANDLES;
         
         uOffSet = ai_uHand %  NUM_ADD_HANDLES;    
         
         if ( uSegmento >= NUMERO_SEGMENTOS ) {
         
            return NULL;
         }
         
         if ( m_pContHand [ uSegmento ] == NULL ) {
         
             m_pContHand [ uSegmento ] = new HAND [ NUM_ADD_HANDLES ] ;
             
             for ( int iCont =0 ; iCont < NUM_ADD_HANDLES ; iCont ++ ) {
              
                 m_pContHand [ uSegmento ] [ iCont ].ptr = NULL ;
                 
                 m_pContHand [ uSegmento ] [ iCont ].tipo = 0 ;
             
             }
         
         }   
         
         return m_pContHand [ uSegmento ][ uOffSet].tipo;
     }   
     
HAND * M4AHandles::GetHand ( UINT ai_uHand ) 
     {
     
         UINT uSegmento;
         UINT uOffSet;
//         UINT i , j;
         
         
         uSegmento = ai_uHand / NUM_ADD_HANDLES;
         
         uOffSet = ai_uHand %  NUM_ADD_HANDLES;    
         
         if ( uSegmento >= NUMERO_SEGMENTOS ) {
         
            return NULL;
         }
         
         if ( m_pContHand [ uSegmento ] == NULL ) {
         
             m_pContHand [ uSegmento ] = new HAND [ NUM_ADD_HANDLES ] ;
             
             for ( int iCont =0 ; iCont < NUM_ADD_HANDLES ; iCont ++ ) {
              
                 m_pContHand [ uSegmento ] [ iCont ].ptr = NULL ;
                 
                 m_pContHand [ uSegmento ] [ iCont ].tipo = 0 ;
             
             }
         
         }   
         
         return &m_pContHand [ uSegmento ][ uOffSet];
     }   
     
     
     
   
   int M4AHandles::DentroDelLimite ( UINT ai_uHand )
     {
       return ( ai_uHand < 31999 )   ;
     }



