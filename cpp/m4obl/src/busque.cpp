#include "busque.hpp"   
#include "cabecera.h"


  BOOL MatchCmp(char *szMatch,char *szCmp)
   {
     int	iCar;
     size_t iTamMatch;
	 size_t iTamCmp;
          
     
     if (!szMatch || !szCmp ) return FALSE; // SI UNA DE LAS DOS CADENAS ES VACIA DA ERROR
     iCar = 0;
     iTamMatch = strlen (szMatch);
     iTamCmp = strlen (szCmp);
     
     // MIENTRAS QUE HAYA CADENA QUE COMPARAR
     while (iCar < iTamMatch && iCar < iTamCmp ) 
      {
       switch ( szMatch[iCar] )
         {
         case '*' :
                    return TRUE;
                    break;
         case '?' :
                    iCar++;
                    break;
         
         default :
                    if ( szMatch[iCar] != szCmp[iCar])
                        return FALSE;
                    else 
                        iCar ++;
         };
      };   
      
     // si ha llegado aqui es porque uno a uno tasan todos los caracteres de las dos cadenas 
     if ( iTamMatch > iTamCmp )  return FALSE;  // si no hubiese devuelto true
     if ( iTamMatch < iTamCmp )  return szMatch[iCar] == '*' ;
     return TRUE;
     
   };
   
   
 BOOL MatchDir(char *szMatchPath,char *szCmpPath)
   {  
    /* En teoria tiene que pasarse dos cadenas allocadas y con dos caminos
       devuelve true si tasa y false si no tasa  o no son caminos */
    M4ClStringOld  szPath;
    M4ClStringOld szMatch;
    char *pMatch;
    char *pAnt;
    char *pPost ; // variables para ir consiguindo los distitos token

    // Funciona como una cadena cualquiera
    szPath.stralloc (256);
    szMatch.stralloc (256);

    // caso basico  
    if ( szCmpPath == NULL ) {
            #ifdef _DEBUG
               MessageBox ( GetFocus () , "El path pasado es null" , "MatchDir" , MB_OK | MB_ICONSTOP ); 
            #endif
       
       }
    
    
    if (strcmp(szCmpPath,"") == 0 ) {
       return MatchCmp(szMatchPath,szCmpPath);
    }                                  
    
    // en caso de nos ser vacio   
    strcpy(szMatch,szMatchPath);  
    strcpy(szPath,szCmpPath);
    // añido un punto para hacerlo todo con un while
    strcat(szPath,".");
    
    // CONSIGO EL PRIMER TOCKEN DE ABAS DOS CADENAS 
    // ESTOS TOCKEN CORRESPONDEN A LA CLASE DEL PRIMER NIVEL
    pMatch = strtok(szMatch,".");
    if (!pMatch)  {
    
      
          
          return FALSE;
    
    }
    // he conseguido un tocken y calculo el otro
    pAnt = szPath;
    // consigo un puentero a la posicion del primer punto
    pPost = strpbrk(pAnt,".");
    if (!pPost)  {
          return FALSE;
    }
    
    // he conseguido el puntero  luego lo cambio al caracter cero asi ahora con pAnt tengo una
    // cadena con el primer tocken
    *pPost = 0;
    
    // Entro siempre con una clase y un Match de clase en las variables pAnt y pPost respectivamente
    while  ( pAnt || pMatch)
      {
        // comparo la clase si no es correcta salgo del programa devolviendo false
        if (!MatchCmp(pMatch,pAnt)) {
          return FALSE;
        }              
        
        // por leche tiene que haber una instacia 
        // de no haberla devolveria error
        // con el mismo metodo de antes calculo el siguiente tocken de las dos cadenas con la misma tecnica
        // primero dejo la cadena como estaba antes
        *pPost = '.';  
        // compruebo si despues del punto hay cadena que seguir estudiando
        if (strlen(pPost) <= 1)  return FALSE;
         // debido a que no tiene Instancia
         // pongo pAnt en la siguente posiciòn despues del punto que es la que le corresponde
        pAnt = pPost +1;
        // calculo la posicion del siguiente punto
        pPost = strpbrk(pAnt,".");
        
        if (!pPost) return FALSE;
        // debido a que no tiene Instancia
        // donde habia un punto pongo cero asi aislo el token 
        *pPost = 0;
        
        // ahora consigo el siguiente Token del Match
        pMatch = strtok(NULL,".");
        if (!pMatch) return FALSE;
        
        // Aqui tengo pAnt la instancia a comparar del camino y en pMatch el match de instancia
        if (!MatchCmp(pMatch,pAnt))  {
          return FALSE;
        }
        // aqui la instancia y el match por ahora van coincidiendo
        // preparo los punteros para el siguiente clase
        *pPost = '.';
        // ahora veo si existe mas clases en el path
        if (strlen(pPost) > 1) 
           {  
              // existe path en la cadena pasada
              pAnt = pPost +1;
              pPost = strpbrk(pAnt,"."); // tiene que existir porque hemos concatenado un punto al principio del programa
              *pPost = 0; // la cadena a comparar siempre acaba en punto
           }   
        else
             pAnt = NULL; // no hay tocken , luego lo pongo a null
        // preparo el PathMatch
        // calculo el siguinete path match
        pMatch = strtok(NULL,".");
       };
              
     // devuelvo true si sale con los dos a null
      
     if (!pAnt && !pMatch )  return TRUE;
     // si alguno de los dos no es null devuelvo false
     return FALSE;   
        
   };


