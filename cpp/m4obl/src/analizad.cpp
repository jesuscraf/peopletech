#include "analizad.hpp"
                       
                  

M4ClassAnalizador::M4ClassAnalizador()
  {                                                 
     bhaytexto=FALSE;
    // buffer=NULL;
     palabra=NULL;
     UltimaLinea=NULL;
     size=0;
     sizepalabra=0;
    // car =NULL;      
     
  };
  
M4ClassAnalizador::~M4ClassAnalizador()  
  {  
     
      
     if ( palabra != NULL ) delete []palabra;
     if ( UltimaLinea != NULL ) delete []UltimaLinea;
     if ( delimitadores != NULL )delete []delimitadores;
     size = 0;                       
     sizepalabra=0;
    // car = NULL;
     bhaytexto=FALSE;
  };
  
BOOL M4ClassAnalizador::HayTexto()     
{ 
 return bhaytexto;
};             

M4RETCODE M4ClassAnalizador::SetFichero(const char *texto)
  {
     // lo que hago es allocar la memoria para acceder al texto y que nadie lo pueda tocar
	  palabra = new( char[MAX_TAM_PROP] );
      delimitadores= new(char[MAX_TAM_DELIMITADORES]);
      if ( (delimitadores==NULL) || (palabra==NULL)/* || (buffer==NULL)*/ ) return IDS_RTE_NO_MEMORIA;
      strcpy(delimitadores,"\"\n=\r \t");
      //strcpy(buffer,texto);
#if !defined(_WINDOWS) && !defined(_UNIX)
      OBLstrcpy(szBuffer,texto);
#else
	  szBuffer.strcpy(texto);
#endif
      size=(DWORD)strlen(texto);
      car=0;
      strcpy(palabra,"\0");
      sizepalabra=0;
      UltimaLinea=NULL;
      bhaytexto=TRUE;
      return EXITO;                                                        
      
   }; 
                                        
M4RETCODE M4ClassAnalizador::Hastalacomilla(char **pal)
 { 
 
    // esta devuelve de donde esta hasta la comilla
   sizepalabra=0;      
   while ( ( szBuffer[car] != '\"' ) && ( sizepalabra < MAX_TAM_PROP - 1))
    {     
      if ( szBuffer[car]=='\n' ) return IDS_RTE_NO_COMILLAS;
      palabra[sizepalabra] = szBuffer[ car ];
      car++;
      sizepalabra++;
    };   
    if (sizepalabra==(MAX_TAM_PROP -1)) return 2900;
    palabra[sizepalabra]=0;
    *pal=palabra;
    return EXITO;
 
 };                                         


M4RETCODE M4ClassAnalizador::NextPalabra(char **pal)
 {  
    int retdev;
                           
    /* si devuelve 
        0   EXITO es un error y si devuelve 
        -1  FINFICH es que ha llegado a fin de fichero
        -2  Si ha leido una palabra muy larga mas de ocheta*/
    
    retdev=Saltablancos();         //salta los blancos desde donde esta
    if (retdev==FINFICH) return FINFICH;
    sizepalabra=0;
    palabra[sizepalabra]=szBuffer[car];                                                            
    car++;
    sizepalabra++;
    // el if es por si entro y en el buffer tengo la siguiente situacion
    // (delimitador)Un_texto
    if(strchr(delimitadores,palabra[sizepalabra-1])==NULL)  
       while ((strchr(delimitadores,szBuffer[car])==NULL) && car<size)                                                        
         { 
          palabra[sizepalabra]= szBuffer[car];
          sizepalabra++;
          car++; 
          if (sizepalabra==MAX_TAM_PROP)
             {
              palabra[MAX_TAM_PROP-1]='\0';
              return -2;
             };
         };
   // si sale por aqui la ultima palabra no buena
    palabra[sizepalabra]='\0';
    *pal= strupr(palabra);  
    return EXITO;                                         
 };      
 
     
/*******************************************************************************
********************************************************************************      
M4RETCODE NextCarNoNull(char &car);

   Implementacion -> 13 / 07 / 95
   
    parametros : car (O)  Devuelvo el caracter
                 
    Entrada  : 
    
    Extracto  : Salta los blancos y devuelve el caracter que se encuentra y avanzo
                uno al puntero de caracter
********************************************************************************      
********************************************************************************/      
              
 
M4RETCODE M4ClassAnalizador::NextCarNoNull(char * caracter)
 {        
  M4RETCODE retdev;
  
  retdev = Saltablancos();
  if ( retdev != EXITO ) return retdev;
  
  *caracter = szBuffer[car ];
  car++;
  
  return EXITO;
 }
 
     
/*******************************************************************************
********************************************************************************      
M4RETCODE NextCarNoNull(char &car);

   Implementacion -> 13 / 07 / 95
   
    parametros : car (O)  Devuelvo el caracter
                 
    Entrada  :  
    Retorno  :  Devuelve EXITO
    
    Extracto  : Devuelve el caracter que esta viendo en este momento pero no
                salta al siguiente
********************************************************************************      
********************************************************************************/      
 
M4RETCODE M4ClassAnalizador::CurrentCar(char * caracter )        
 {
   *caracter = szBuffer [ car ];
   return EXITO;
 }
 
 
 
 
M4RETCODE M4ClassAnalizador::AppendTexto(char *texto)
 { 
   
   /* si devuelve 
       -1 entonces se ha pasado un puntero a nulo*/
#if !defined(_WINDOWS) && !defined(_UNIX)   
   OBLstrcat(szBuffer,texto);
   size = OBLstrlen(szBuffer);
#else
   szBuffer.strcat(texto);
   size = (DWORD)szBuffer.strlen();
#endif

   return EXITO ;
 };
       
      
          
        
M4RETCODE M4ClassAnalizador::Saltablancos()
  {
    while ( car <size  &&  szBuffer[car]<=' ' && szBuffer[car]!='\n' ) car++;
    if (car >=  size) return FINFICH;
    return EXITO;
  };       
        
// esta funcion para poderla usar se tiene que pasar el puntero del caracter apuntando a un
// blanco o bien a un /n entonces salta todo lo que encuetre por delate        
        
M4RETCODE M4ClassAnalizador::SaltaLineasBlancas()
  {
    DWORD iUltimoSalto; 
    BOOL bContinuar;
    
    
   // retdev =Saltablancos();                 
   // if (retdev!=EXITO) return retdev;
   iUltimoSalto = car;
   bContinuar = TRUE;    
   // Esto es para distiguir el caso inicial que se seguro de que el anterior no es salto carro
   // y ademas es invisible el primer caracter
   if ( car == 0  &&  szBuffer[car] <= ' ' )
          car++;  
   else bContinuar = ( car == 0 ) ?  FALSE : TRUE;
   // compruebo antes que se puede continuar en la busqueda de caracteres
   if ( bContinuar )       
        while (szBuffer[car-1] == '\n'  &&  car < size  && bContinuar)
         {
           iUltimoSalto = car;
           Saltablancos();
           if (szBuffer[car] == '\n' )  
               car++;
           else  
               bContinuar = FALSE;
     
         };
      
    car = iUltimoSalto; 
    return EXITO;
   };




     
/*******************************************************************************
********************************************************************************      
M4RETCODE GetPosicionActual(DWORD *lPosicion,DWORD *lLinea,DWORD *lOffSet)

   Implementacion -> 13 / 07 / 95
   
    parametros : lPosicion (O)  Posicion Actual
                 lLinea    (O)  Linea Actual
                 lOffSet   (O)  Posicion en la linea
                 
    Entrada  : 
    
    Extracto  : Esta funcion calcula la posicion actual donde estamos, 
                El OffSet es donde aparece el primer salto carro
                Y tenemos el numero de '/n' que hay hasta esa poscion
********************************************************************************      
********************************************************************************/      

M4RETCODE M4ClassAnalizador::GetPosicionActual(DWORD *lPosicion,DWORD *lLinea,DWORD *lOffSet)
  {

    DWORD      lBarrido;   
    DWORD      lContOffSet;
    DWORD      lContLinea;
    
    *lPosicion = car;
    lBarrido = car;                                           
    lContOffSet = 0;
    
    // calculo el offset
    while ( szBuffer [lBarrido] != '\n'  && lBarrido > 0 )
        {         
          lBarrido --;
          lContOffSet ++;
        };  
    
    // ya llevo una linea
    lContLinea = 1;
    while ( lBarrido != 0 )
      { 
        lContLinea = ( szBuffer[lBarrido] == '\n' ) ? lContLinea + 1 : lContLinea;
        lBarrido-- ;
       };
         
    *lOffSet = lContOffSet;
    
    *lLinea = lContLinea;  
    
    return EXITO;

  }


