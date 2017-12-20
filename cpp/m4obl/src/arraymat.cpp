 #include "arraymat.hpp"
 
 
 ArrayMatch::ArrayMatch()
  {
   uNumMat = 0;
   ContMatch=NULL;                           
  }           
  
 ArrayMatch::~ArrayMatch()
  {  
    UINT uCont=0;
    for (uCont = 0 ;uCont < uNumMat; uCont ++ )
     {
       delete ContMatch[uCont];
     };
    uNumMat = 0;
    delete []ContMatch; 
  }                 
  
 M4RETCODE ArrayMatch::AMAddNuevo(char *szPath,char *szMatch,UINT *uHandMat)
   { 
     int  iLibre; // esta variable me idica que celda esta libre
     ClMatch **ppCMAux;
     UINT uCont;
     if (uNumMat == 0)
       {
         ContMatch = new ClMatch *[1];
         ContMatch[0] = new ClMatch;
         iLibre = 0;
         uNumMat++;
       }
     else
       { 
         uCont = 0;
         iLibre = -1;
         while ( uCont < uNumMat && iLibre==-1) {
	          if ( ContMatch [uCont] -> CMVacio() ) {
	          
	          	 iLibre = uCont;
	          }	 
	          else {
	             
	             uCont++;
	          }   
	     }     
          
         if (iLibre == -1) // si no hay ninguno libre hago sitio a lo que hay
            {
              
              ppCMAux = new ClMatch * [uNumMat+ 1 ];
              for (uCont=0;uCont < uNumMat; uCont++)
                 ppCMAux[uCont] = ContMatch[uCont];
              
              ppCMAux[uNumMat]= new ClMatch;
              
              if (ContMatch != NULL ) delete ContMatch;
              
              ContMatch = ppCMAux;
              
              iLibre = uNumMat;
              
              uNumMat ++ ;
              
            }
       }  
       
    // este punto ya tengo allocada la memoria y tengo en iLibre el sitio donde le coresponde
    // ahora le asigno los valores
    *uHandMat = (UINT)iLibre + 1 ;
    return ContMatch[iLibre]->CMSetMatch(szPath,szMatch,*uHandMat) ;
   };
   
    


 M4RETCODE ArrayMatch::AMBorrar(UINT uHandMat)
   {
      
     	 char szBuffer[255];
     	 
      sprintf ( szBuffer , "Hmatch -> %u y Mhnd -> %u " , uHandMat , uNumMat ); 
      
      #ifdef DEBUG
	      OutputDebugString ( szBuffer );
      #endif   

      
      
     if ( uHandMat < 1  || uHandMat > uNumMat )  {
     	
     	return IDS_RTE_HAND_PATH;
     }	 
     
     ContMatch[ uHandMat - 1 ] -> CMBorrar();   
     
     return EXITO; 
   }        
   
   
   
   
  M4RETCODE ArrayMatch::AMGetPath(UINT uHandMat,char **pPath)         
  {
   
   if ( uHandMat < 1  || uHandMat > uNumMat ) return IDS_RTE_HAND_PATH;
   if ( ContMatch[uHandMat - 1]->CMVacio()) return IDS_RTE_HAND_MATCH_VACIO;
   *pPath = ContMatch[uHandMat-1]->CMGetPath();
   return EXITO;
   
  }        
  
  M4RETCODE ArrayMatch::AMGetMatch(UINT uHandMat,char **pMatch)         
  {
   if ( uHandMat < 1  || uHandMat > uNumMat ) return IDS_RTE_HAND_PATH;
   if ( ContMatch[uHandMat - 1]->CMVacio()) return IDS_RTE_HAND_MATCH_VACIO;
   *pMatch = ContMatch[uHandMat - 1]->CMGetMatch();
   return EXITO;
  } ;
  
  M4RETCODE ArrayMatch::AMGetPos(UINT uHandMat,UINT *uPos)         
  {
   if ( uHandMat < 1  || uHandMat > uNumMat ) return IDS_RTE_HAND_PATH;
   if ( ContMatch[uHandMat - 1]->CMVacio()) return IDS_RTE_HAND_MATCH_VACIO;
   *uPos = ContMatch[uHandMat - 1]->CMGetPos();
   return EXITO;
  } ;
  
  M4RETCODE  ArrayMatch::AMSetPos(UINT uHandMat , UINT uPos)
    {
      if ( uHandMat < 1  || uHandMat > uNumMat ) return IDS_RTE_HAND_PATH;
      if ( ContMatch[uHandMat - 1]->CMVacio()) return IDS_RTE_HAND_MATCH_VACIO;
      ContMatch[uHandMat - 1]->CMSetPos(uPos);
      return EXITO;
    } 
    
  M4RETCODE ArrayMatch::AMGetTask(UINT uHandMat,t_PID *hTaskMatch)
    {                                                                     
    
      if ( uHandMat < 1  || uHandMat > uNumMat ) return IDS_RTE_HAND_PATH;
      
      if ( ContMatch[uHandMat - 1]->CMVacio()) return IDS_RTE_HAND_MATCH_VACIO;
      
      *hTaskMatch=ContMatch[uHandMat - 1]->CMGetTask();
      
      return EXITO;
    
    }


