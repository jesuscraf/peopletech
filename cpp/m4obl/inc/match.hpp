#ifndef __CLASE_DE_MATCHES__
#define __CLASE_DE_MATCHES__

#include "cabecera.h" 

class ClMatch 
  {    
  protected:
    M4ClStringOld stPath;
    M4ClStringOld stMatch;
    UINT uPosAct;
    UINT uHandMat;
    t_PID hTask;
    
  public:  
    ClMatch();
    ~ClMatch();
    M4RETCODE CMSetMatch(char *szPath,char *stMatch,UINT uHandle);
    char *CMGetPath();
    char *CMGetMatch();
    UINT CMGetPos();
    void CMSetPos(UINT uPos);
    void CMBorrar();
    BOOL CMVacio();
    
    //   funcion que obtiene la handle a la tarea que lo creo
    t_PID CMGetTask();
  }; 
  
#endif   //  __CLASE_DE_MATCHES__



