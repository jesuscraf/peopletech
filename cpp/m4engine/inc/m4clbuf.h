//////////////////////////////////==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4clbuf.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/17/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestiona buffers variables, optimizados en tiempo y tamaño.
//
//
//==============================================================================

#ifndef __M4CLBUFFER__H__
#define __M4CLBUFFER__H__

#include "clfile.h"

/*
  Esta clase provee un buffer redimensionable en run-time, creciendo 
este a multiplos de szBlock bytes.
  Importante: el maximo tamaño de bloque alocado viene marcado por el sistema. size_t 
indica cual este limite. 
	Lo que no nos produce ningún problema es tener tantos bloques como haga falta, siempre
teniendo en cuenta que el limite de InterBlock es size_t.
*/

class M4ClBuffer  
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	m4uint16_t m_uiNBlock;	//Número bloques alocados
	m4int8_t * m_piFirstBlock, * m_piLastBlock  ; 	//Buffer, primer y ultimo bloque
	size_t  m_szBlock;      //Tamaño de cada bloque
	size_t m_szFill ;       //Numero bytes ocupados en el ultimo bloque 

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	M4ClBuffer();
	virtual ~M4ClBuffer() { M4ClBuffer::End(); }
	virtual size_t GetSizeof() { return sizeof(M4ClBuffer) ; }

	/*
	  Init puede llamarse las veces que se quiera. Libera la memoria del buffer.
	    No se crea ningún bloque hasta que no se escriba algo.
	  InterBlock debe ser >0, o se produce un error INMEDIATO.
      ¡¡¡Llamar a Init antes de usar otras funciones!!!
	*/
	
	void Init(size_t ai_szInterBlock);

	//Init válido si no es el primer init. Sirve para borrar los buffers, dejando el mismo
	//  valor de szBlock.
	void Init() { Init(m_szBlock); }

	//Init válido si no es el primer init. Como Init(), pero además no borra los Preserve
	//   bytes iniciales del buffer. Si excede el tamaño actual, se ajusta a todo el buffer.
	void InitPreserve(m4uint32_t ai_uiPreserve);

	//VIRTUAL EXTENSIBLE
	//Libera toda la memoria del buffer y resetea los valores de toda la clase.
	virtual void End();


MIT_PUBLIC:

	//-------------------------Gets-----------------------------------------------------

	// El total alocado puede sobrepasar sin problema el limite de size_t.
	m4uint32_t GetTotal() const  ;  //Da número de bytes totales introducidos en el buffer.
	m4uint16_t GetNBlock() const { return m_uiNBlock; }
	int IsEmpty() const { return m_uiNBlock==0 ; }
	size_t GetSizeBlock() const { return m_szBlock ; }

	//-------------------------Actualizacion---------------------------------------------
	/*
	    Añade NBytes al final buffer, copiando los datos desde Data. 
		NBytes puede ser mayor que InterBlock.
        Si no hay sitio en el buffer, se alocan más bloques. La función es muy rápida 
	  porque se usa una lista de bloques, con lo cual el realocar no implica en ningún 
	  momento copiar.
	    Cada uno de estos bloques tiene m_szBlock bytes, más lo necesario para mantener 
	  el puntero al siguiente bloque.
	*/

	void Append( const void *ai_Data, size_t ai_szNBytes );

	//Graba cadenas SIN copiar el nulo final
	void AppendStr(  const m4char_t * ai_cStr );

	//Graba cadenas copiando el nulo final
	void AppendStrNULL(  const m4char_t * ai_cStr );

	//Concatena el contenido del buffer ai_poBuff
	void CatM4ClBuffer(M4ClBuffer *ai_poBuff) 
		{	ai_poBuff->Flush(ToM4ClBuffManip,this) ; }

	//	Copia al buffer el contenido de un fichero. Se copia desde el byte indicado en
	//ai_From, y los n bytes indicados en ai_Count. Si este valor es READ_UNTIL_EOF, se copia 
	//hasta EOF. Asi pues, los defectos de From=0 y READ_UNTIL_EOF copian todo el fichero.
	//	De cualquier forma, el numero de bytes escritos siempre esta supeditado a los datos en 
	//el fichero. Es decir, un From fuera de rango no escribe nada, un Count de 0 tampoco,
	//y un Pos correcto con un Count que sobrepasa el tamaño a escribir, solo escribe tantos
	//bytes como se lean desde fichero.

	enum { READ_UNTIL_EOF=-1 } ;
	void AppendFile( ClMIT_File *ai_poF, m4uint32_t ai_From=0, m4uint32_t ai_Count=READ_UNTIL_EOF ) ;

	//-------------------------Flush-----------------------------------------------------

	typedef void (FlushFunc_t)(m4int8_t *ai_Data, size_t ai_szNBytes, void *ai_pvParam) ;

	/*
	  Flush simplemente recorre una parte del buffer, bloque a bloque, llamando por cada
	bloque a la funcion Process. Esta recibe informacion indicando desde que direccion de
	memoria procesar y cuantos bytes deben procesarse. También se le pasa el valor Param, 
	que puede ser cualquier cosa (incluyendo un puntero a una estructura con un número de
	parametros indefinido).
	  Dentro de Process podemos hacer lo que queramos; volcarlos a fichero, a impresora, 
	etc. Esta clase suministra algunos metodos process, pero externamente pueden 
	implementarse todos los que se quieran.
	  Estas tres versiones de Flush simplemente recorren un rango distinto del buffer:
		-La primera lo recorre por completo, 
		-La segunda versión de flush recorre desde From (0 el primero), y cuantos bytes
			se recorren desde ahi. 
			Si el valor NBytes es 0, no se testea ni se hace absolutamente nada.
			Si el valor From excede los limites, se produce un error immediato (si NBytes
				no es 0).
			Si el valor NBytes desde la posicion From sobrepasa el tamaño del buffer, 
			no pasa nada, se recorren solo los bytes que existen. 
		-La tercera version recorre desde el byte From hasta el final.
	*/

	void Flush( FlushFunc_t *Process, void *ai_pvParam );
	void Flush( FlushFunc_t *Process, void *ai_pvParam, 
		m4uint32_t ai_uiFrom, m4uint32_t ai_uiNBytes) ;
	void Flush( FlushFunc_t *Process, void *ai_pvParam, m4uint32_t ai_uiFrom ) 
		{ Flush(Process, ai_pvParam, ai_uiFrom, GetTotal()-ai_uiFrom); }


	//-------------------------Metodos Process utilizables con Flush---------------------

	//El parametro auxiliar debe ser un ClMIT_File. El fichero ya está abierto, y debe tener 
	//  permiso de W. Se va escribiendo la informacion via ClMIT_File::Append().

	static void WFileManip(m4int8_t *ai_Data, size_t ai_szNBytes, void *ai_pvParam) ;


	struct StCopy {
		void * Data ;
		size_t Count ;
	} ;

	//El parametro auxiliar debe ser de tipo StCopy. Data apunta a la zona de memoria 
	//	que se va a copiar, y Count dice cuantos bytes se copiaran, sobreescribiendo lo
	//  que tiene  el buffer. CUIDADO!!! Los valores de Param son utilizados dentro del
	//  metodo process, lo cual los inutiliza.
	//Ejemplos:
	//
	//  StCopy Param = { "Hola, que tal", 4 } ;
	//	Flush(MemToBuff, &Param ) ; Recorre todo el buffer, si al menos hay 4 caracteres al
	//		principio, los sobreescribe poniendo 'Hola'. Si solo hubiese 2, escribiria 'Ho'.
	//	Flush(MemToBuff,&Param,0,4) ; Como antes, pero mas optimo porque no recorre todo el
	//		buffer.
	//
	//	long Long ;
	//  StCopy Param = { &Long, sizeof(long) } ;
	//	Flush(MemToBuff,&Param,5,sizeof(long) ) ; Copia un long a partir del offeset 5, 
	//		suponiendo siempre que hay al menos sizeof(long) bytes para ser sobreescritos.

	static void MemToBuffManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) ;

	//BuffToMem es igual que el anterior, solo que en vez de escribir de memoria al buffer, se
	//	lee del buffer a memoria.

	static void BuffToMemManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) ;


	// Hace un Append en el ao_pvParam, que debe ser un parametro a M4ClBuffer (por supuesto, 
	//	el objeto target no puede ser el objeto source).

	static void ToM4ClBuffManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) ;

	//-----------------------------------------------------------------------------------


};


#endif
