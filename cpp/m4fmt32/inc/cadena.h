//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CADENA.H
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Reviewed: Manuel Lazacano Perez - 08/07/98
//
// Definition:
//
//    This module defines a class for an optimized treatment of strings
//
//
//==============================================================================

#ifndef __CADENAS_H
#define __CADENAS_H

#include "m4types.hpp"
#include "m4define.hpp"
#include "m4string.hpp"

#include "mit.h"

/////////////////////////////////////////////////////////////////////////////////

#define INICIO 0
#define ACTUAL 1
#define FINAL  2

#define ESTADO_CREADA 1

/////////////////////////////////////////////////////////////////////////////////
//  CLASE ClCadena
//
// Gestiona el tratamiento interno de las Cadenas de caracteres en la DLL.
// Debe hacerse uso de esta clase en las DLLs que implementen los Plug-Ins de
//  los idiomas.
//
//////////////////////////////////////////////////////////////////////////////////

class ClCadena
{  

MIT_PUBLIC:          
	
	ClCadena();
	~ClCadena();
   
	//Copia un objeto cadena en el objeto host
	m4int16_t CopiaDe(ClCadena* a_pCad);
   
	//Hay dos versiones sobrecargadas de la siguiente funcion. Esta funcion asigna una cadena
	// a nuestro objeto. La rimera version aloca la memoria necesaria para contenerla, mientras
	// que a la segunda le pasamos un long para indicar el espacio de memoria a alocar
	m4int16_t Asigna(m4char_t*);
	m4int16_t Asigna(m4char_t*, m4uint32_t);   

	//Aqui tenemos tres versiones sobrecargadas de una funcion que crea un objeto cadena de tres
	// formas diferentes. La primera crea una cadena a partir de una cadena de solo lectura,
	// alocando la memoria suficiente, mientras que a la seguna le asignamos el numero de bytes a
	// alocar. En la tercera version, unicamente alocamos espacio en memoria para, posteriormente
	// escribir la cadena.
	m4int16_t Crea(const m4char_t*);
	m4int16_t Crea(const m4char_t*, m4uint32_t);   
	m4int16_t Crea(m4uint32_t);
   
	//Esta funcion situa al puntero m_iPos en el byte de la cadena indicado por el primer para- 
	// metro, a partir del lugar indicado por el segundo, que puede ser INICIO, ACTUAL o FINAL.
	void Posiciona(m4int16_t, m4char_t);

	//LeeCadena vuelca un numero de caracteres (2º parametro) de la cadena objeto a una cadena
	// externa identificada por el primer parametro.
	m4int16_t LeeCadena(m4char_t*, m4uint32_t);

	//Estas son tres versiones sobrecargadas de una funcion que debe escribir sobre la cadena 
	// host desde una cadena extrena, que puede ser un string standard (1ª version) o un objeto
	// cadena (2ª version). En ambos casos debemos pasarle el numero de caracteres a escribir.
	// La escritura se realiza desde m_iPos, con lo cual primero habri que hacer un Posiciona. 
	// En la tercera version, se escribe la cadena completa, alocando la memoria suficiente.
	m4int16_t EscribeCadena(const m4char_t*, m4uint32_t);
	m4int16_t EscribeCadena(ClCadena* a_pCad, m4uint32_t l);
	m4int16_t EscribeCadena(const m4char_t*);

	//Estas tres funciones son las versiones sobrecargadas para un funcion que debe leer un char
	// de la cadena objeto. En el primer caso, se lee el caracter apuntado por m_iPos, y avanza 
	// un byte en el array. En la segunda no se avanza despues de leer y en la tercera el puntero
	// se mueve hacia atras.
	m4char_t LeeChar(void);
	m4char_t LeeCharNA(void);
	m4char_t LeeCharPaTras(void);

	//Escribe un caracter en la cadena objeto.
	void EscribeChar(char);              
             
	//Estas dos funciones comparan un objeto cadena con la cadena host. Hacen uso de la funcion
	// standard strcmp, por lo que devuelven un 0 si sin iguales y distinto de cero si no lo son
	// (segun sea una cadena ascii-mente mayor o menor que la otra). La unica diferencia entre 
	// ambas, es que en la segunda, el puntero m_iPos de la cadena que pasamos como parametro,
	// regresa a la posicion en la que estaba antes de realizar la llamada.
	m4int16_t ComparaCon(ClCadena* a_pCad);
	m4int16_t ComparaConNA(ClCadena* a_pCad);
   
	//Esta funcion se utiliza para mapear una cadena mediante una tabla definida externamente
	// y pasada como cadena de solo lectura. Se utiliza en el proyecto para cosas como quitar
	// o poner acentos, cambiar mauysculas por minusculas, etc...
	m4int16_t Mapeo(const m4char_t* a_pTabla);
   
	//Funcion que devuelve la longitud de la cadena host.
	m4int16_t LonReal(void);
	
	//Comprueba si la cadena es nula
	m4bool_t EsNula(void){ return M4_BOOL(m_pDirIni==NULL); };
	
	//Devuelve la direccion de inicio de la cadena objeto
	m4char_t* DirIni(void){ return m_pDirIni; };
	
	//Comprueba si hemos llegado al final de la cadena
	m4bool_t IsEOF(void){ return M4_BOOL(m_iPos>=m_iLonBuffer); };
      
MIT_PRIVATE:        
   
	m4char_t   m_Estado;
	m4uint32_t m_iPos;         
	m4uint32_t m_iLonBuffer;   

	m4char_t*  m_pDir;
	m4char_t*  m_pDirIni;   

	//Aloca los punteros a NULL e inicializa el buffer y la posicion a cero
	void Init(void);

	//Vacia el contenido de la cadena, pero sin desalocarla
	void End(void);
   
friend class FMT_ClFormato;

};

#endif

