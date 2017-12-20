#ifndef __ARRAY_DE_MATCHES__
#define __ARRAY_DE_MATCHES__
#include "match.hpp"

class ArrayMatch
 {
   private:
     UINT uNumMat; // indica el numero de matchs que hay abiertos
     ClMatch **ContMatch;
   public:
     
     ArrayMatch();
     ~ArrayMatch();
     
     
     M4RETCODE AMAddNuevo(char *szPath,char *szMatch,UINT *uHandMat);
     M4RETCODE AMBorrar(UINT uHandMat);
     M4RETCODE AMGetPath(UINT uHandMat,char **pPath);
     M4RETCODE AMGetMatch(UINT uHandMat,char **pMatch);
     M4RETCODE AMGetPos(UINT uHandMat,UINT *uPos);
     M4RETCODE AMSetPos(UINT uHandMat,UINT uPos);
     M4RETCODE AMGetTask(UINT uHandMat,t_PID *hTaskMatch);
 };                         
 
#endif  // __ARRAY_DE_MATCHES__ 



