 #include "match.hpp"
  
 ClMatch::ClMatch() 
  {
    uHandMat = 0;
    hTask = GetCurrentTask();
    
  };
 
 ClMatch::~ClMatch() 
  {
    uHandMat = 0;
  };          
  
 M4RETCODE ClMatch::CMSetMatch(char *szPath,char *szMatch,UINT uHandle)
  {
    if (!szPath || !szMatch) return IDS_RTE_PARAM_NULL;
    StrCpy(stPath,szPath);
    StrCpy(stMatch,szMatch);
    uHandMat = uHandle; 
    uPosAct = 0 ;
    return EXITO;  
  };
 
   char *ClMatch::CMGetPath()
   {
     return (char *)stPath;
   };             
  
   char *ClMatch::CMGetMatch()
   {
     return (char *)stMatch;
   };
   
   UINT ClMatch::CMGetPos()
   {
     return uPosAct;
   };   
   
   void ClMatch::CMSetPos(UINT uPos)
    {
     uPosAct = uPos;
    } 
    
   // CAMBIO CANARIO
   void ClMatch::CMBorrar()
    {
     uHandMat = 0;
    };              
    // FIN CAMBIO CANARIO
 
  BOOL ClMatch::CMVacio()
    {
     return uHandMat==0;
    }; 
 
  t_PID ClMatch::CMGetTask() 
    {
     return hTask;
    }


