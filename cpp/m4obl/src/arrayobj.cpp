#include "arrayobj.hpp"

         
 M4ArrayLOBL::M4ArrayLOBL()
   {
     uNumObl = 0;
     ppContLOBL = NULL;
   
   };  
                 
                 
  M4ArrayLOBL::~M4ArrayLOBL()              
  {      
    UINT i=0;  

    for (i=0 ;i< uNumObl ; i++ ) 
      {
         if ( ppContLOBL[i] != NULL )
           {
             delete ppContLOBL[i];
             //MessageBox ( NULL, "HE BORRADO OTRO LBOL" , "M4ArrayLOBL", MB_OK);
           }
             
      }   
    
    delete []ppContLOBL;  
 };                                                 
  
  
  M4RETCODE M4ArrayLOBL::M4AOblIniciaAnonimo(UINT *uHandOut)
   {
     M4ClassLOBL **ppAux;
     UINT i;           
     BOOL bAllocado ;
 
     // jmms modificacion 18 / 07 / 95
     // para que busque un obj anterior  
      bAllocado = FALSE ; 
      for ( i = 0 ;  i< uNumObl ; i++)
         if ( ppContLOBL[i] == NULL )
            {
              *uHandOut = i;      
              ppContLOBL[ i ] = new M4ClassLOBL ;
              if ( !ppContLOBL[ i ]  ) return IDS_RTE_NO_MEMORIA;
              bAllocado = TRUE;
              break;
            }
      if ( ! bAllocado )
        {    
              // fin modificaicion  
              /*********** lo que estaba antes **********/ 
      
                 
              ppAux = new M4ClassLOBL* [uNumObl + 1 ];
               
              if (!ppAux) return IDS_RTE_NO_MEMORIA;
              for (i=0;i< uNumObl;i++)
              {
                 ppAux[i] = ppContLOBL[i];
                 ppContLOBL[i] = NULL;
              };
                 
              // una vez que ya lo tengo todo asignado lo puedo deletear
              delete []ppContLOBL;  
              ppContLOBL = NULL;
              
              ppContLOBL = ppAux;
              ppAux = NULL; 

              //Modificado Enrique 
              // Se le asigna el espacio con OblCpy en M4AOblCpy
              //    
                ppContLOBL[uNumObl] = NULL ;
                
//              ppContLOBL[uNumObl] = new M4ClassLOBL;
               
//              if (!ppContLOBL[uNumObl]) return IDS_RTE_NO_MEMORIA;
                
              uNumObl++; // acabo de alocarme donde va ir el nuevo bloque;
       
              *uHandOut = uNumObl - 1; // coincide con el numero de un handle nuevo es el numero de elemento menos uno
  }      
    
    return EXITO;
       
   };  
   
  M4RETCODE M4ArrayLOBL::M4AOblAcceso(UINT uHand)
   {
//    HTASK hTask,hTaskObl;
    if (uHand >= uNumObl ) return IDS_RTE_HAND_ERRONEO;
    if (!ppContLOBL[uHand]) return IDS_RTE_FICHERO_NULL;  
//   hTask = GetCurrentTask();
//   retdev =M4AOblGetTask(uHand,&hTaskObl);
//   if (retdev != EXITO)  return retdev;
//    if ( hTask != hTaskObl)  return IDS_NO_PERMISO;
    return EXITO;
  }; 
   
   
  
  M4RETCODE M4ArrayLOBL::M4AOblInicializa(char *szNombFich,char *szExtFich,char *szTitulo,char *szBase,UINT *uHand) 
   {
     M4ClassLOBL **ppAux;
     M4RETCODE retdev;
     UINT i;
     BOOL bAllocado;
     
     // jmms modificacion 18 / 07 / 95
     // para que busque un obj anterior  
      bAllocado = FALSE ; 
      for ( i = 0 ;  i< uNumObl ; i++)
         if ( ppContLOBL[i] == NULL )
            {
              *uHand = i;      
              ppContLOBL[ i ] = new M4ClassLOBL ;
              if ( !ppContLOBL[ i ]  ) return IDS_RTE_NO_MEMORIA;
              bAllocado = TRUE;
              break;
            }
      if ( ! bAllocado )
        {    
      // fin modificaicion  
      /*********** lo que estaba antes **********/ 
      
     
          ppAux = new M4ClassLOBL* [uNumObl + 1 ];
          
          if (!ppAux) return IDS_RTE_NO_MEMORIA;
          
          for (i=0;i< uNumObl;i++)
            {
              ppAux[i] = ppContLOBL[i];
              ppContLOBL[i] =NULL;
            };
            
          // una vez que ya lo tengo todo asignado lo puedo deletear
          delete [] ppContLOBL;  
          ppContLOBL = NULL; 
         
          ppContLOBL = ppAux;
          ppAux = NULL;
          

          ppContLOBL[uNumObl] = new M4ClassLOBL;
          
          if (!ppContLOBL[uNumObl]) return IDS_RTE_NO_MEMORIA;
           
          uNumObl++; // acabo de alocarme donde va ir el nuevo bloque;
          
          *uHand = uNumObl -1; // coincide con el numero de un handle nuevo es el numero de elemento menos uno
          
        }
    retdev = ppContLOBL[*uHand]->SetObl("",szNombFich,szExtFich,szTitulo,szBase,*uHand);
    if (retdev!=EXITO)
        {
          M4AOblFinaliza(*uHand);
          // Modificado Enrique  
          //uNumObl--;
        };
    return retdev;
 };                               
      
 M4RETCODE M4ArrayLOBL::M4AOblAddTexto(UINT uHand,char *szTexto)
   {
     M4RETCODE retdev ;
     
     retdev = M4AOblAcceso(uHand);  
     if (retdev != EXITO) return retdev;
     return ppContLOBL[uHand]->OblSetFich(szTexto);
     
   };  
   
 
 M4RETCODE M4ArrayLOBL::M4AOblGeneraObl(UINT uHand,char *szTexto)
   {
     M4RETCODE retdev ;
     
     retdev = M4AOblAcceso(uHand);  
     if (retdev != EXITO ) return EXITO;
     
     retdev =  ppContLOBL[uHand]->GeneraObl(szTexto);

     if (retdev != EXITO) return retdev;
     return EXITO;
   };  
   
      
     
 M4RETCODE M4ArrayLOBL::M4AOblFinaliza(UINT uHand)
   {
     M4RETCODE retdev ;
     
     retdev = M4AOblAcceso(uHand);  
     if (retdev != EXITO) return retdev;
     
     
#ifdef undef     
	//Este codigo no es multiplataforma y no se incluye en la compilacion
     #ifdef _DEBUG     
     
          char szbuffer[244];                                    
          
          wsprintf ( szbuffer ," Finaliza el obj -> %d", uHand );
          MessageBox ( NULL, szbuffer , "finaliza", MB_OK);
          
     #endif
#endif
          
     delete ppContLOBL[uHand];
     ppContLOBL[uHand] = NULL; 
     //_heapmin ();
     return EXITO;
   }   
   
M4RETCODE M4ArrayLOBL::M4AOblGetTask(UINT uHand,t_PID *hTaskObl)
  {
   *hTaskObl = ppContLOBL[uHand]->OblGetTask();
   return EXITO;
  }   
    
M4RETCODE M4ArrayLOBL::M4AOblGetVersion(UINT uHand,char **pszVer)
 {               
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
     
   return ppContLOBL[uHand]->OblGetVersion(&(*pszVer));

 };  
 
 
M4RETCODE M4ArrayLOBL::M4AOblSetVersion( UINT uHand,char *pszVer)
 {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
     
   return ppContLOBL[uHand] -> OblSetVersion( pszVer );
 }




 
M4RETCODE M4ArrayLOBL::M4AOblGetExt(UINT uHand,char **pszExt)
  {               
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
     
   return ppContLOBL[uHand]->OblGetExtFich(&(*pszExt));

  };  
M4RETCODE M4ArrayLOBL::M4AOblGetNombFich(UINT uHand,char **pszNombFich)
 {               
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
     
   return ppContLOBL[uHand]->OblGetNombFich(&(*pszNombFich));

 };
   
M4RETCODE M4ArrayLOBL::M4AOblGetTit(UINT uHand,char **pszTitulo)
 {               
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
    
   return ppContLOBL[uHand]->OblGetTitulo(&(*pszTitulo));

 };  
M4RETCODE M4ArrayLOBL::M4AOblGetBase(UINT uHand,char **pszBase) 
 {               
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetBase(&(*pszBase));

 };  
                        

M4RETCODE M4ArrayLOBL::M4AOblGetClass(UINT uHand,char **pszClass)
 {                                                  
   M4RETCODE retdev ;
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetClass(&(*pszClass));
 };   
   


M4RETCODE M4ArrayLOBL::M4AOblGetHinst(UINT uHand,char **pszHinst)                       
 {                                                    
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetHinst(&(*pszHinst));
 }; 
 

M4RETCODE M4ArrayLOBL::M4AOblSetHinst(UINT uHand,char *pszHinst)                       
 {                                                    
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblSetHinst(pszHinst);
 }; 
 
 
 
 
 
   
M4RETCODE M4ArrayLOBL::M4AOblGetValProp(UINT uHand,char *pPathInst,char *pNomb,char **pszVal)
 {                                                      
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetValProp(pPathInst,pNomb,&(*pszVal));
 } 
 
                       
M4RETCODE M4ArrayLOBL::M4AOblGetPropHandNomb(UINT uHand,UINT hBlq,char *szNombProp,char **szVal)
 {
                                                       
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev; 
   
   
   return ppContLOBL[uHand]->OblGetPropHandNomb(hBlq,szNombProp,&(*szVal));
 
 }    
 
/***********************************************************************************
************************************************************************************
  Implementacion          -> 30 /06 / 95
  Ult Version             ->
  M4AObjGetTipoHandNomb(UINT uHand,UINT hBlq,char *szNombProp,int *iTipo)
  Con el handle de bloque y el nombre de la propiedad consigue el tipo de la propiedad
  que se esta referenciando 
************************************************************************************
************************************************************************************/


M4RETCODE  M4ArrayLOBL::M4AOblGetTipoHandNomb(UINT uHand,UINT hBlq,char *szNombProp,int *iTipo)
 {
    M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev; 
   
   
   return ppContLOBL[uHand]->OblGetTipoHandNomb(hBlq,szNombProp,&(*iTipo));
 }
 
 
/***********************************************************************************
************************************************************************************
  Implementacion          -> 30 /06 / 95
  Ult Version             ->
  M4AOblGetTipoPropHandle(UINT uHand,UINT uProp,int * iTipo)
  Extracto ->  con el handle de propiedad consigo el tipo de la fucnion
               que se esta referenciando 
************************************************************************************
************************************************************************************/

M4RETCODE  M4ArrayLOBL::M4AOblGetTipoPropHandle(UINT uHand,UINT uProp,int * iTipo)
 {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev; 
   
   
   return ppContLOBL[uHand]->OblGetTipoPropHandle(uProp,&(*iTipo));
 
 
 
 }
 


M4RETCODE M4ArrayLOBL::M4AOblGetValProp(UINT uHand,char *pPathInst,char **pszVal)
 {                                                     
  M4RETCODE retdev ;
     
  retdev = M4AOblAcceso(uHand);  
  if (retdev != EXITO) return retdev;
   
  return ppContLOBL[uHand]->OblGetValProp(pPathInst,&(*pszVal));
 };    
         

M4RETCODE M4ArrayLOBL::M4AOblGetHandProp(UINT uHand,char *pPathInst,char *pNomb,UINT *uHandout)
 {                                                   
  M4RETCODE retdev ;
     
  retdev = M4AOblAcceso(uHand);  
  if (retdev != EXITO) return retdev;
   
  return ppContLOBL[uHand]->OblGetHandProp(pPathInst,pNomb,&(*uHandout));
 }; 
 
M4RETCODE M4ArrayLOBL::M4AOblGetHandBlq(UINT uHand, char *pPathInst,UINT *uHandout)
 {
  M4RETCODE retdev ;
     
  retdev = M4AOblAcceso(uHand);  
  if (retdev != EXITO) return retdev;
  return ppContLOBL[uHand]->OblGetHandBlq(pPathInst,&(*uHandout));
 }
 
 
 
 

M4RETCODE M4ArrayLOBL::M4AOblGetHandProp(UINT uHand,char *pPathInst,UINT *uHandout)
 {                                                    
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetHandProp(pPathInst,&(*uHandout));
 };                                                                     
 
M4RETCODE M4ArrayLOBL::M4AOblGetClassBlqHandle (UINT uHand,UINT uHandBlq,char **pszClass) 
 {                                            
   
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetClassBlqHandle(uHandBlq,&(*pszClass));
 };
 
M4RETCODE M4ArrayLOBL::M4AOblGetHinstBlqHandle (UINT uHand,UINT uHandBlq,char **pszHinst)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetHinstBlqHandle(uHandBlq,&(*pszHinst));
 };


M4RETCODE M4ArrayLOBL::M4AOblSetHinstBlqHandle (UINT uHand,UINT uHandBlq,char *pszHinst)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblSetHinstBlqHandle( uHandBlq, pszHinst );
   
 };
 
 
                                                                        
 
 
M4RETCODE M4ArrayLOBL::M4AOblGetValPropHandle (UINT uHand,UINT uHandPrp,char **pszVal)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetValPropHandle(uHandPrp,&(*pszVal));
 };
     
M4RETCODE M4ArrayLOBL::M4AOblGetNomPropHandle (UINT uHand,UINT uHandPrp,char **pszNomb)
 {                                                    
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetNomPropHandle(uHandPrp,&(*pszNomb));
 };             
 
M4RETCODE M4ArrayLOBL::M4AOblSetPropPath(UINT uHand,char *pszPathInst,char *pszNomb,char *pszValor,int iTipo)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblSetPropPath(pszPathInst,pszNomb,pszValor,iTipo);
 
 };       
 
M4RETCODE M4ArrayLOBL::M4AOblSetPropNomb(UINT uHand,UINT uHandBlq,char *szNomb,char *szValor,int iTipo)
 {
    M4RETCODE retdev ;
    retdev = M4AOblAcceso(uHand);
    if (retdev != EXITO ) return retdev;        
      
    return ppContLOBL[uHand] -> OblSetPropNomb(uHandBlq,szNomb,szValor,iTipo);
 }     
 
M4RETCODE M4ArrayLOBL::M4AOblSetPropHand(UINT uHand,UINT uHandPrp,char *pszValor,int iTipo)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblSetPropHand(uHandPrp,pszValor,iTipo);
 
 };           

M4RETCODE M4ArrayLOBL::M4AOblAddPropPath(UINT uHand,char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandPrp)
 {     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblAddPropPath(pPathInst,strupr(pszNomb),pszVal,iTipoPrp,&(*uHandPrp));
 
 };          
 
 M4RETCODE M4ArrayLOBL::M4AOblAddPropHand(UINT uHand,UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandPrp)
 {     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblAddPropHand(uHandBlq,strupr(pszNomb),pszVal,iTipoPrp,&(*uHandPrp));
 
 };
 
 M4RETCODE M4ArrayLOBL::M4AOblDelPropPath(UINT uHand, char *pPathInst,char *pszNomb)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
    return ppContLOBL[uHand]->OblDelPropPath(pPathInst,pszNomb);
  };    
  
 M4RETCODE M4ArrayLOBL::M4AOblDelPropNomb(UINT uHand, UINT uHandBlq,char *szNomb)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblDelPropHandNomb(uHandBlq,szNomb);
  }                   
 
 M4RETCODE M4ArrayLOBL::M4AOblDelPropHand(UINT uHand,UINT uHandPrp)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblDelPropHand(uHandPrp);
  };  
 
 
 M4RETCODE M4ArrayLOBL::M4AOblAddBlqPath(UINT uHand,char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblAddBlqPath(strupr(pPathInst),strupr(pszClass),strupr(pszHinst),&(*uHandBlqOut));
  };


 M4RETCODE M4ArrayLOBL::M4AOblAddBlqHand(UINT uHand,UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblAddBlqHand(uHandBlq,strupr(pszClass),strupr(pszHinst),&(*uHandBlqOut));
  };                         
                        
M4RETCODE M4ArrayLOBL::M4AOblDelBlqPath(UINT uHand, char *pPathInst)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblDelBlqPath(pPathInst);
  };
 
                        
 M4RETCODE M4ArrayLOBL::M4AOblDelBlqHand(UINT uHand,UINT uHandBlq)
   {
    M4RETCODE retdev ;
     
    retdev = M4AOblAcceso(uHand);  
    if (retdev != EXITO) return retdev;
   
    return ppContLOBL[uHand]->OblDelBlqHand(uHandBlq);
   }; 
   
 M4RETCODE M4ArrayLOBL::M4AOblDelRama(UINT uHand , char *szPath)
  {
    M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblDelRama(szPath);
  };
   
M4RETCODE M4ArrayLOBL::M4AOblFirstHndProp(UINT uHand,char *szMatchPath,char *szMatchProp,UINT *uHandPrp,UINT *uHandMatch)
  {
   M4RETCODE retdev;  
   char *szPathOut ;
   char  *pszMatchProp ;
   char *pNomb,*pVal;
     
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;

   
   szPathOut = new char [MAX_TAM_PATH];                            
   pszMatchProp = new char [MAX_TAM_PATH];                                                                      

                                             
   strcpy ( pszMatchProp , szMatchProp);
   retdev = ppContLOBL[uHand] -> OblFindFirstMatchProp(szMatchPath,pszMatchProp,szPathOut,&pNomb,&pVal,&(*uHandPrp),&(*uHandMatch));

   delete pszMatchProp;
   delete szPathOut;
   return retdev;
   }
 
M4RETCODE M4ArrayLOBL::M4AOblNextHndProp(UINT uHand,UINT uHandMatch,UINT *uHandPrp)
  {
   M4RETCODE retdev;  
   char *szPathOut;  
   char *pNomb,*pVal;

   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;

   
   szPathOut = new char [MAX_TAM_PATH];

   retdev = ppContLOBL[uHand]->OblFindNextMatchProp(uHandMatch,szPathOut,&pNomb,&pVal,&(*uHandPrp));
  
   delete szPathOut;
   return retdev;
  };
   
M4RETCODE M4ArrayLOBL::M4AOblFirstHndBlq(UINT uHand,char *szMatchPath,UINT *uHandBlq,UINT *uHandMatch)
 { 
   M4RETCODE retdev;  
   char *szPathOut;  

     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   szPathOut = new char [MAX_TAM_PATH];
   retdev = ppContLOBL[uHand]->OblFindFirstMatchBlq(szMatchPath,szPathOut,&(*uHandBlq),&(*uHandMatch));
   delete []szPathOut;
   return retdev;
 };
 
 M4RETCODE M4ArrayLOBL::M4AOblNextHndBlq(UINT uHand,UINT uHandMatch,UINT *uHandBlq)
  {
    
   M4RETCODE retdev;  
   char *szPathOut;  
     
  
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   szPathOut = new char [MAX_TAM_PATH];
   retdev = ppContLOBL[uHand]->OblFindNextMatchBlq(uHandMatch,szPathOut,&(*uHandBlq));
   delete []szPathOut;
     
   return retdev;
 } 
  
  
M4RETCODE M4ArrayLOBL::M4AOblFirstProp(UINT uHand,char *szMatchPath,char *szMatchProp,char *szPathOut,char **pNomb,char **pVal,UINT *uHandPrp,UINT *uHandMatch)
 {
     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return  ppContLOBL[uHand]->OblFindFirstMatchProp(szMatchPath,szMatchProp,szPathOut,&(*pNomb),&(*pVal),&(*uHandPrp),&(*uHandMatch));
 } 


M4RETCODE M4ArrayLOBL::M4AOblNextProp(UINT uHand,UINT uHandMatch,char *szPathOut,char **pNomb,char **pVal,UINT *uHandPrp)
 {
     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return  ppContLOBL[uHand]->OblFindNextMatchProp(uHandMatch,szPathOut,&(*pNomb),&(*pVal),&(*uHandPrp));
 } 
 
M4RETCODE M4ArrayLOBL::M4AOblFirstBlq(UINT uHand,char *szMatchPath,char *szPathOut,UINT *uHandBlq,UINT *uHandMatch)
 {
     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblFindFirstMatchBlq(szMatchPath,szPathOut,&(*uHandBlq),&(*uHandMatch));
 }
  
M4RETCODE M4ArrayLOBL::M4AOblNextBlq(UINT uHand,UINT uHandMatch,char *szPathOut,UINT *uHandBlq)
 {    
     
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblFindNextMatchBlq(uHandMatch,szPathOut,&(*uHandBlq)); 
 };  
 
                                         

/*******************************************************************************
********************************************************************************      
M4AOblGetPosicionError(UINT uHand,DWORD * lPos, DWORD  * lLinea,DWORD  * lOffSet);

   Implementacion -> 13 / 07 / 95
   
    parametros :  uHand    ->  asigna handle del bloque erroneo
                  lPos     ->  posicion actual
                  lLinea   ->  linea actual cuando se ha cometido un error
                  lOffSet  ->  desplazmiento actual en la linea
                  
    Extracto  :  Esta funcion es de depuracion y solo se hace cuando se ha 
                 cometido un error al crearse el obj
   
********************************************************************************      
********************************************************************************/   

M4RETCODE M4ArrayLOBL::M4AOblGetPosicionError(UINT uHand,DWORD * lPos, DWORD  * lLinea,DWORD  * lOffSet)
 { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
 
   return ppContLOBL[uHand]->OblGetPoscionError( & (*lPos), & (*lLinea) , & (*lOffSet) );
 }



// FIN DE FUNCIONES DE BUSQUEDAS          

// FUNCIONES DE UTILIDAD

M4RETCODE M4ArrayLOBL::M4AOblCpy(UINT uHandSource,UINT  *uHandDest)
  {                     
    UINT uHandOut;
    M4ClassLOBL *pAux;
    
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHandSource);  
   if (retdev != EXITO) return retdev;
   
    
   retdev =M4AOblIniciaAnonimo(&uHandOut);
   if (retdev != EXITO) return retdev;
    
   retdev = OblCpy(ppContLOBL[uHandSource],&pAux,uHandOut);
   if (retdev != EXITO) return retdev;
   ppContLOBL[uHandOut]= pAux;
   *uHandDest = uHandOut;
                     
   return EXITO;                 
 }  
 

M4RETCODE M4ArrayLOBL::M4AOblCpyBlq ( UINT ai_uHandSource ,UINT  ai_uHandBlqSource , UINT ai_uHandDest ,UINT ai_uHandBlqDest )
 {
              
               M4ClassBloque *pBlqSource;
               M4ClassBloque *pBlqDest;
               M4ClassBloque *pBlqAux;
               int retdev;
     
     // Comprobamos que el fuente es correcto
     retdev = M4AOblAcceso( ai_uHandSource ) ;
     if ( retdev != EXITO ) return retdev ;
     
      
     // Comprobamos que el destino es correcto
     retdev = M4AOblAcceso( ai_uHandDest ) ;
     if ( retdev != EXITO ) return retdev ;
     
     
     // Buscamos el puntero bloque fuente.
     retdev = ppContLOBL [ ai_uHandSource ] -> GetPtrBlq ( ai_uHandBlqSource , & pBlqSource )  ;
     if ( retdev != EXITO) return retdev;


     // Buscamos el puntero bloque que contendrá al destino
     retdev = ppContLOBL [ ai_uHandDest ] -> GetPtrBlq ( ai_uHandBlqDest, & pBlqDest )  ;
     if ( retdev != EXITO) return retdev;

     
     // En el destino es donde guardo el bloque
     retdev = pBlqDest -> M4ABAddNuevoBlq( pBlqDest , & pBlqAux );
     if ( retdev != EXITO ) {

  
     return IDS_RTE_ERROR_COPIANDO;
          
     }
     //retdev = ppContLOBL [ ai_uHandeDest ]  -> OblCpyBlq ( /* Bloque Fuente */  , /* Bloque Destino */  );
     retdev = ppContLOBL [ ai_uHandDest ]  -> OblCpyBlq ( pBlqSource , pBlqAux  ); 
     
     if ( retdev != EXITO ) {
          
          ppContLOBL [ ai_uHandDest ]  -> ReparaObjeto( ppContLOBL [ ai_uHandDest ] -> pBloque );
          delete pBlqAux;     
          
          return IDS_RTE_ERROR_COPIANDO;
     }    
     
     
     return EXITO;
 }

 
 
  
  
M4RETCODE M4ArrayLOBL::M4AOblCmp(UINT uHand1,UINT uHand2,BOOL *bResult)
  {
    
   // COMPRUEBO SI LOS PARAMETROS PASADOS SON CORRECTOS
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand1);  
   if (retdev != EXITO) return retdev;

   retdev = M4AOblAcceso(uHand2);  
   if (retdev != EXITO) return retdev;
   
    
   *bResult = OblCmp(ppContLOBL[uHand1],ppContLOBL[uHand2]);
    
   return EXITO;
  
  };  

   
 M4RETCODE M4ArrayLOBL::M4AOblGetPath(UINT uHand,UINT uHandBlq, char *szPath)
  { 
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   return ppContLOBL[uHand]->OblGetPath(uHandBlq,szPath);
  };
 

M4RETCODE M4ArrayLOBL::M4AOblWriteFicheroFirst(UINT uHand,char *pszBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout)
  {
   M4RETCODE retdev ;
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   return ppContLOBL[uHand]->WriteFicheroFirst(pszBufferout,uTamMaxszBufferout,uTamszBufferout);
   
  } 
M4RETCODE M4ArrayLOBL::M4AOblWriteFicheroNext(UINT uHand,char *pszBufferout,UINT uTamMaxszBufferout,UINT *uTamszBufferout)
  {
   M4RETCODE retdev ;

   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   return ppContLOBL[uHand]->WriteFicheroNext(pszBufferout,uTamMaxszBufferout,&(*uTamszBufferout));
  }     
    
M4RETCODE M4ArrayLOBL::M4AOblWriteBloque(UINT uHand,UINT uHandBlq,char * pszBufferout,UINT uTamMaxBufferOut)
  {
   M4RETCODE retdev ;           
#if !defined(_WINDOWS) && !defined(_UNIX)
   M4ClHString szhBuffer;
#else
   M4ClStringOld szhBuffer;
#endif

   size_t lTamBuff;  
   UINT uTamSalida;
   
     
   retdev = M4AOblAcceso(uHand);  
   if (retdev != EXITO) return retdev;
   
   // Escribo el bloque en una cadena huge
   retdev = ppContLOBL[uHand]->WriteBloque(uHandBlq,szhBuffer);
   
   // Si ha habido error devuelvo error
   if (retdev != EXITO ) return retdev;

   // calculo la longitud de  la cadena
   lTamBuff = strlen (szhBuffer);


   // si el tamaño de la cadena es mayor entonces truncamos y devolvemos un error
   if ( lTamBuff > ( long )(uTamMaxBufferOut - 1) ) {
       
       // Copiamos lo que cabe en el buffer
       strncpy(pszBufferout,(char *)szhBuffer, uTamMaxBufferOut - 1 );
       
       // Cero terminamos la cadena
       pszBufferout[uTamMaxBufferOut - 1] = 0;
       
       retdev = IDS_RES_TRUNCADO;
   }
   else {
       
       uTamSalida = (UINT)lTamBuff ; 
       
       // Copiamos lo que me dice la longitud de la cadena
       strncpy(pszBufferout,(char huge *)szhBuffer,uTamSalida );
          
       // Cero terminamos la cadena de salida
       pszBufferout[lTamBuff] = 0;           
       
       retdev = EXITO;
       
   }    
   
   return retdev;

  } ;                    
  
  
// FUNCIONES DE BUSQUEDAS DE LOS FICHEROS
      
  M4RETCODE M4ArrayLOBL::M4AOblFindFirstTask(UINT *uHandObl,UINT *uHandMatch)
   {   
     t_PID hTask,hTaskObl; 
     UINT uCont;
         
     
     // lo que estoy buscando son los ficheros que he metido desde mi tarea
     AMatch.AMAddNuevo(STR_TASK,"",&(*uHandMatch));
     uCont = 0;
     while ( uCont < uNumObl)
       {                                   
        if (ppContLOBL[uCont])
         { // SI ES DISTINTO DE NULO

    	   hTask = GetCurrentTask() ;
/*
           #if defined ( _WINDOWS )   
                    
                    hTask = GetCurrentProcess();

             #elif defined ( _WINDOWS )
                     
                    hTask = GetCurrentTask();

             #endif
*/
           hTaskObl = ppContLOBL[uCont]->OblGetTask();
           if (hTask == hTaskObl)
            {
              *uHandObl= uCont;
              AMatch.AMSetPos(*uHandMatch,uCont);
              return EXITO;
            }     
         } 
         uCont ++;
       } 
       AMatch.AMBorrar(*uHandMatch);
       return IDS_OBJ_NOT_FOUND;
   }
   
   
M4RETCODE M4ArrayLOBL::M4AOblFindNextTask(UINT uHandMatch,UINT *uHandObl)
   {         
    char *pszTipoBusqueda;
    M4RETCODE retdev;  
    UINT uAnt;
    t_PID hTaskObl,hTask,hTaskMatch;    
    UINT uCont;     

       hTask = GetCurrentTask() ;   
/*    
    // compruebo primero que el match pertenece a quien lo ha llamado
       #if defined ( _WINDOWS )  
               
               hTask = GetCurrentProcess();

        #elif defined ( _WINDOWS )
                
               hTask = GetCurrentTask();

        #endif
*/

    retdev = AMatch.AMGetTask(uHandMatch,&hTaskMatch);
    if (retdev != EXITO ) return retdev;
    if (hTaskMatch != hTask) return IDS_RTE_HAND_MATCH; 
    
    
    
    retdev = AMatch.AMGetPath(uHandMatch,&pszTipoBusqueda);
    if (retdev != EXITO ) return retdev;
    // compruebo el tipo de busqueda haber si el findfirst fue a tareas
    if (strcmp ( pszTipoBusqueda, STR_TASK) != 0) return IDS_TIPO_BUSQ_EQUIV;
    
    retdev = AMatch.AMGetPos(uHandMatch,&uAnt);
    if (retdev != EXITO ) return retdev;
    // empiezo a buscar desde el siguiente de donde me quede
    uAnt ++;  
    uCont = uAnt;
    while ( uCont < uNumObl)
       {
        if (ppContLOBL[uCont])
         { // SI ES DISTINTO DE NULO
           hTaskObl = ppContLOBL[uCont]->OblGetTask();
           if (hTask == hTaskObl)
            {
             AMatch.AMSetPos(uHandMatch,uCont);
             *uHandObl = uCont;
             return EXITO;
            }
         }        
         uCont ++;
       } 
       AMatch.AMBorrar(uHandMatch);
       return IDS_OBJ_NOT_FOUND;
   }
   
   
  M4RETCODE M4ArrayLOBL::M4AOblFindFirstExt(char *szMatchExt,UINT *uHandObl,UINT *uHandMatch)
  {  
    M4RETCODE retdev;
    char *szExt;
    UINT uCont;           
    t_PID hTask,hTaskObl;

		hTask = GetCurrentTask() ;    
/*       #if defined ( _WINDOWS )  
               
               hTask = GetCurrentProcess();

        #elif defined ( _WINDOWS )
                
               hTask = GetCurrentTask();

        #endif
*/

    retdev = AMatch.AMAddNuevo(STR_EXTENSION,szMatchExt,&(*uHandMatch));
    if (retdev != EXITO ) return retdev;
    
    uCont = 0 ;
    while (uCont < uNumObl)
     {
       if (ppContLOBL[uCont]) // pillo datos de los obl definidos
        { 
         
         ppContLOBL[uCont]->OblGetExtFich(&szExt); 
         hTaskObl = ppContLOBL[uCont]->OblGetTask();
         if ( MatchCmp(szMatchExt,szExt) && hTask == hTaskObl) 
           {
             *uHandObl = uCont;
             AMatch.AMSetPos(*uHandMatch,uCont);
             return EXITO;
           }
        }   
        uCont++; 
     }
     AMatch.AMBorrar(*uHandMatch);
     return IDS_OBJ_NOT_FOUND;
   } 
   
   
   
  M4RETCODE M4ArrayLOBL::M4AOblFindNextExt(UINT uHandMatch,UINT *uHandObl)
  {
    M4RETCODE retdev;
    char *szExt,*szMatchExt,*pszTipoBusqueda;
    UINT uCont;
    UINT uAnt;    
                               
    t_PID hTask,hTaskMatch,hTaskObl;
    // compruebo primero que el match pertenece a quien lo ha llamado
    
    	hTask = GetCurrentTask() ;
/*
       #if defined ( _WINDOWS )  
               
               hTask = GetCurrentProcess();

        #elif defined ( _WINDOWS )
                
               hTask = GetCurrentTask();

        #endif
*/
    retdev = AMatch.AMGetTask(uHandMatch,&hTaskMatch);
    if (retdev != EXITO ) return retdev;
    if (hTaskMatch != hTask) return IDS_RTE_HAND_MATCH; 
    
    // tipo de busqueda                                  
    retdev = AMatch.AMGetPath(uHandMatch,&pszTipoBusqueda);
    if (retdev != EXITO ) return retdev;
    if (strcmp ( pszTipoBusqueda, STR_EXTENSION) != 0) return IDS_TIPO_BUSQ_EQUIV;                                  
                                      
    // consigo el match
    retdev = AMatch.AMGetMatch(uHandMatch,&szMatchExt);
    if (retdev != EXITO ) return retdev;
    
    // posicion en la que nos quedamos
    retdev = AMatch.AMGetPos(uHandMatch,&uAnt);
    if (retdev != EXITO ) return retdev;
                       
                       
    // adelanto uno para mirar el siguiente
    uAnt++;
    uCont = uAnt ; 
    
    while (uCont < uNumObl)
     {
       if (ppContLOBL[uCont]) // pillo datos de los obl definidos
        { 
         ppContLOBL[uCont]->OblGetExtFich(&szExt);
         // consigo el task del objeto fichero y compruebo que pertenece a mi aplicacion
         hTaskObl = ppContLOBL[uCont]->OblGetTask();
         if ( MatchCmp(szMatchExt,szExt) && hTask == hTaskObl) 
           {
             *uHandObl = uCont;
             AMatch.AMSetPos(uHandMatch,uCont);
             return EXITO;
           }
        }   
        uCont++; 
     }
     AMatch.AMBorrar(uHandMatch);
     return IDS_OBJ_NOT_FOUND;
   };
           
  M4RETCODE M4ArrayLOBL::M4AOblFindFirstNombFich(char *szMatchNomb,UINT *uHandObl,UINT *uHandMatch)
  {  
    M4RETCODE retdev;
    char *szNomb;
    UINT uCont;
    t_PID hTask,hTaskObl; 

		hTask = GetCurrentTask() ;    
/*
       #if defined ( _WINDOWS )  
               
               hTask = GetCurrentProcess();

        #elif defined ( _WINDOWS )
                
               hTask = GetCurrentTask();

        #endif
*/
    retdev = AMatch.AMAddNuevo(STR_NOMBRE_FICHERO,szMatchNomb,&(*uHandMatch));
    if (retdev != EXITO ) return retdev;
    
    uCont = 0 ;
    while (uCont < uNumObl)
     {
       if (ppContLOBL[uCont]) // pillo datos de los obl definidos
        { 
         ppContLOBL[uCont]->OblGetNombFich(&szNomb);
         hTaskObl = ppContLOBL[uCont]->OblGetTask();
         if ( MatchCmp(szMatchNomb,szNomb) && hTask == hTaskObl) 
           {
             *uHandObl = uCont;
             AMatch.AMSetPos(*uHandMatch,uCont);
             return EXITO;
           }
        }   
        uCont++; 
     }
     AMatch.AMBorrar(*uHandMatch);
     return IDS_OBJ_NOT_FOUND;
  } 
   
   
  M4RETCODE M4ArrayLOBL::M4AOblFindNextNombFich(UINT uHandMatch,UINT *uHandObl)
   { 
    M4RETCODE retdev;
    char *szNomb,*szMatchNomb,*pszTipoBusqueda;
    UINT uCont;
    UINT uAnt;                               
    t_PID hTask,hTaskMatch,hTaskObl; 
    
    // compruebo primero que el match pertenece a quien lo ha llamado
       
       hTask = GetCurrentTask() ;
/*
       #if defined ( _WINDOWS )  
               
               hTask = GetCurrentProcess();

        #elif defined ( _WINDOWS )
                
               hTask = GetCurrentTask();

        #endif
*/
    retdev = AMatch.AMGetTask(uHandMatch,&hTaskMatch);
    if (retdev != EXITO ) return retdev;
    if (hTaskMatch != hTask) return IDS_RTE_HAND_MATCH; 
      
    // tipo de busqueda                                  
    retdev = AMatch.AMGetPath(uHandMatch,&pszTipoBusqueda);
    if (retdev != EXITO ) return retdev;
    if (strcmp ( pszTipoBusqueda,STR_NOMBRE_FICHERO) != 0) return IDS_TIPO_BUSQ_EQUIV;                                  
    // consigo el match
    retdev = AMatch.AMGetMatch(uHandMatch,&szMatchNomb);
    if (retdev != EXITO ) return retdev;
    
    // posicion en la que nos quedamos
    retdev = AMatch.AMGetPos(uHandMatch,&uAnt);
    if (retdev != EXITO ) return retdev;
                       
                       
    // adelanto uno para mirar el siguiente
    uAnt++;
    uCont = uAnt ; 
    
    while (uCont < uNumObl)
     {
       if (ppContLOBL[uCont]) // pillo datos de los obl definidos
        { 
         ppContLOBL[uCont]->OblGetNombFich(&szNomb);
         hTaskObl = ppContLOBL[uCont]->OblGetTask();
         if ( MatchCmp(szMatchNomb,szNomb) && hTask == hTaskObl ) 
           {
             *uHandObl = uCont;
             AMatch.AMSetPos(uHandMatch,uCont);
             return EXITO;
           }
        }   
        uCont++; 
     }
     AMatch.AMBorrar(uHandMatch);
     return IDS_OBJ_NOT_FOUND;
   };
 
 
 M4RETCODE M4ArrayLOBL::M4AOblFindHandBlq ( UINT ai_uHand, UINT ai_uHandBlq , char * ai_pszNombreClase  , int & aio_iPosicion , UINT & ao_uHandBlq )
    {
          M4RETCODE retdev ;

          retdev = M4AOblAcceso( ai_uHand);  
          if (retdev != EXITO) return retdev;

          return  ppContLOBL[ai_uHand] -> OblFindHandBlq  ( ai_uHandBlq ,  ai_pszNombreClase  ,  aio_iPosicion , ao_uHandBlq );
      
    }
    



// NUEVO CANARIO    

 
 // Si el ai_TipoSearch = 0 	Bloque
 //       ai_TipoSearch = 1		Propiedad
 
 M4RETCODE M4ArrayLOBL::M4AOblKillSearch (UINT ai_uHand , UINT ai_uHandMatch  , int ai_iTipoSearch )
    {
		
				M4RETCODE retdev;  
			
			retdev = M4AOblAcceso( ai_uHand );  
	
			if (retdev != EXITO) {
			
				return retdev;
			}	
	
	        
			return  ppContLOBL [ ai_uHand ] -> OblKillSearch ( ai_uHandMatch , ai_iTipoSearch );
       }           
       
    
// FIN NUEVO CANARIO    

