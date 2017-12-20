//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             MIT.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/17/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//		Nucleo de la gestion multithread y multiinstancia.
//
//
//==============================================================================

/*


*/

#ifndef __MIT__H__
#define __MIT__H__

#include "m4mienv_dll.hpp"

/*
Siempre accesibles los defines y tipos Meta 4
----------------------------------------------------------------------------------------------
*/

#include "m4define.hpp"
#include "m4types.hpp"

/*

Switchs de compilacion
----------------------------------------------------------------------------------------------

  Estos switchs SOLO aparecen en los *.c*, no en los *.h*. Esto es asi para que los 
componentes que hagn uso de este no tengan porque definir estas macros para que los *.h sean
consistentes.
	Definir o no estos switchs segun se desee. NO definirlos aqui: es mucho mejor hacerlo en :

	Project - Settings - C/C++ - Category:General - Preprocessor definitions

	El motivo es que asi el codigo es el mismo para las versiones Debug o Release, al no
tener que tocar este *.H para nada.

#define MIT_BUILDING_MIT

  Solo esta definido para el proyecto MIT. Gestiona la macro switch para imporat o exportar
las clases, segun desde donde se incluye el *.H

#define MIT_TRACE
  
	Indica que se quieren mensajes trace. El sistema trace siempre se inicializa, otra cosa
es que se haga uso de esta macro para llamar a los metodos ClTrace o no.
	Actualmente, siempre se utiliza el trace, los errores y warnings SIEMPRE estan 
hard-coded, pero los mensajes debug solo lo estan si se define MIT_TRACE 
(excepto algunos muy genericos que no repercuten en la velocidad).

#define MIT_MEM_PROF 

	Si esta definido, se activa el profiler de memoria. Lo mas normal es que se defina o 
no paralelamente con MIT_TRACE.
	Esta macro es la unica que rompe la regla anterior: si aparece en el memprof.h, pero no
hay problemas de consistencia, porque solo aparece para definir las macros de memoria de una
u otra forma. Asi pues, podemos definir o no esta macro con completa libertad en cualquier
componente.


Hay un *.cpp, MITdefs.cpp, que es como un *.h, pero de uso COMPLETAMENTE interno a esta 
componente. Por eso es un *.cpp, y SOLO debe ser incluido desde los *.cpps que lo
necesiten. Todos los componentes necesitan de un cpp similar.  

*/


/*

Portabilidad UNIX
----------------------------------------------------------------------------------------------

	UNIX tiene problemas si hay clases u objetos globales que se llamen igual en distintos 
	componentes.
	Una buena soluciones que por componente se asigne un prefijo unico para usar con nombres.
	Este componente usa el prefijo:
		-MIT. Multi intanceable thread environment.
*/

#ifndef _UNIX
#define MIT_PUBLIC public
#define MIT_PRIVATE private
#define MIT_PROTECTED protected
#else
//El compilador peta erroneamente si un enum privado o protected se usa como parametro de 
//metodos publicos.
#define MIT_PUBLIC public
#define MIT_PRIVATE public
#define MIT_PROTECTED public
#endif

/*
Macros y tipos de uso general
----------------------------------------------------------------------------------------------
*/

#define M4_MIN(a,b) ( (a)<(b)?(a):(b) )
#define M4_MAX(a,b) ( (a)>(b)?(a):(b) )
#define M4_IN_RANGE(c,a,b) ( (c)>=(a) && (c)<=(b) ) 

#define M4_END_STR 0		//Caracter fin de cadena ASCIIZ

#define MIT_NULL_ID -1L			//IDentificador nulo, preguntar usando ==, no <=.
typedef m4uint32_t MIT_Id_t ;	//Tipo para cualquier identificador numerico

/*
Siempre accesible el trace y el memprofiler
----------------------------------------------------------------------------------------------
*/

#include "memprof.h"
#include "trace.h"

/*
Gestion MULTITHREAD
----------------------------------------------------------------------------------------------
*/

//En otro fichero por comodidad

#include "mitthr.h"

// Estas RegKeys existen en todos los componentes.
// Cada componente tiene, ademas, unos rangos de RK privados para sus propios RK.

enum MIT_REG_KEYS { MIT_RK_LOG_INI_FILE=0x00, MIT_RK_TRACE_FILE=0x010, MIT_RK_SYSTEM_DEBUG_ENABLE=0x020, MIT_RK_WORKDIR=0x30 } ;

// Todos los grupos de traza y memoria que pueden intervenir en tiempo de creacion y 
//	destruccion de inspectors son redireccionados a los componentes en cuestion. Por ello,
//	siempre aparecen en todos los inspectors y estan reservados. 
// El resto de indices desde (y contando con) MIT_TGI_FIRST_SHARED son reutilizados por 
//	cada componente.

enum MIT_SHARED_TRACE_GROUPS {
	MIT_TGI_CL_STR,
	MIT_TGI_TRACE,
	MIT_TGI_MEMPROF,
	MIT_TGI_MIT,
	MIT_TGI_FILE,
	MIT_TGI_CL_ARGS,
	
	MIT_TGI_TEMPORAL,

	MIT_TGI_NOT_SHARED_FIRST
} ;

//Los indices son el anterior enum
//Puden rpetirse Ids para reagrupar grupos de traza cara afuera
const m4char_t m_g_pcTraceGroupsId[][9]=
{
	"STR",
	"TRACE",
	"MEMPROF",
	"MIT",
	"FILE",
	"ARGS",
	"TEMPORAL",
	"SHAREDFS"
} ;

enum MIT_SHARED_MEM_GROUPS {
	MIT_MGI_CL_STR,

	MIT_MGI_OTHERS,
	MIT_MGI_TEST,		//Sirve para redirigir subgrupos de news en debug para pillar bugs
	MIT_MGI_NOT_SHARED_FIRST,
	MIT_MGI_SHARED_COUNT=MIT_MGI_NOT_SHARED_FIRST
} ;

extern M4_DECL_M4MIENV const m4char_t * g_MIT_pcSharedMG[MIT_MGI_SHARED_COUNT] ;

//OBJETO GLOBAL, ESTATICO, UNICO POR PROCESO 

extern M4_DECL_M4MIENV  ClMIT_PM g_oMIT_P ; 

// Para este Comp, solo hay un tipo posible de PlugIn.
// Se aloca al construir el g_oMIT_P, para asegurarnos de que en tiempo de construccionde DLL
//	esta disponible.

extern M4_DECL_M4MIENV StMIT_CompPlugIn *g_MIT_pstPlugIn ;


/*
NOTAS sobre OOP
----------------------------------------------------------------------------------------------
*/
//---------------------------------Miembros virtuales-------------------------------------
/*
  Vamos a dar unas reglas para documentar el *.h, de forma que quede claro cuando y como 
sobrecargar funciones virtuales.
  Cuando se declare una función virtual, indicar claramente su tipo, que puede ser:

  VIRTUAL PURA. No definida en la clase base, obligatorio crearla en la derivada. Una
     de estas funciones ya hace que una clase sea virtual.
  VIRTUAL SUSTITUIBLE. Definida en la clase base, las sobrecargardas en las derivadas
     no llaman a la de la clase base. Osea, la virtual derivada sustituye a la de la base.
  VIRTUAL EXTENSIBLE. Como la anterior, solo que la virtual derivada llama a la virtual de
     la base. Osea, la virtual derivada extiende la funcionalidad de la de la base.
  VIRTUAL VACIA. Codigo en la base definido, pero vacio. Simplemente existe para no tener 
	que definirla obligatoriamente en las derivadas. Si sera luego SUSTITUIBLE o EXTENSIBLE,
	depende del uso que se le de en las derivadas.
  
//---------------------------------Constructor Init() y destructor End()------------------

  Los destructores y constructores normales tienen unas pegas importantes:
	-El constructor solo se invoca al crear el objeto. Puede ser que al crearlo aun no
		sepamos algún parametro necesario para inicializar el objeto.
	-Si tenemos varios objetos globales, y un constructor hace referencia a otro objeto, 
		no tenemos garantía de que este esté creado; dicho de otra forma, con objetos 
		globales no hay una forma estandar de secuenciar el orden de construccion.
	-Tanto el constructor como el destructor solo se invocan una vez, al crear o destruir
		el objeto.
  Vamos a ver como subsanar esto, con un ejemplo:
	
class CLASE : public BASE {

 private:
	Miembros tipo variable
	char *Str ;	//Miembros creados dinamicamente por la clase
	OBJA Array[] ;
  
MIT_PUBLIC:
	CLASE();
	virtual ~CLASE() { CLASE::End(); }

	void Init(Lista parametros inicializacion) ;
	virtual void End() ;
} ;

void CLASE::CLASE() {

	//---------Poner flag de no inicializado a miembros dinámicos

	Str=NULL;	
	Array=NULL;
}

void CLASE::Init(Lista parametros inicializacion) {

	//---------Limpia el objeto, sea o no la primera llamada a Init

	CLASE::End() ;

	//---------Propagar constructores Init(); de la jerarquia. 

	BASE::Init(SubLista parametros inicializacion) ;

	//---------Inicializar miembros propios del objeto usando los parametros de Init

}

void CLASE::End() {

	//---------Borrar miembros dinámicos, si se crearon, y poner flag de no inicializado

	M4_SAFE_DELETE(Str) ; 
	M4_SAFE_DELETE_ARRAY(Array);

	//---------Limpiar miembros propios del objeto

	//---------Propagar destructores End(); de la jerarquia

	BASE::End();	
}

  Observar que los destructores son virtuales, e invocan a End(), que también es virtual. 
Esto asegura que Init() y End() funcionan con polimorfismo, además de poder ejecutar 
End() las veces que queramos, independientemente de si llamamos antes a Init o no.
  También es muy importante poner dentro de las clases el especificador de ambito a End(),
porque si no se llamará al End() de la derivada (al ser virtual), en vez de a la propia. 
  Sin embargo, con Init tenemos un problema. Lo ideal sería declararlo virtual para que
funcionase con polimorfismo, pero no podemos hacer esto, porque si se declara virtual, 
el numero y tipo de argumentos ya debe ir prefijado de antemano, cosa que en cada 
derivada puede variar. el problema en este contexto, es que el polimorfismo no funciona
con miembros no virtuales. Pero si nos fijamos en unos detalles:
	-Llamar a Init implica conocer el tipo real, ya que esto a su vez implica saber los 
		parametros necesarios para inicializarlo. 
	-El polimorfismo solo puede implementarse a través de punteros.
  Por lo tanto, nuestro problema solo se produce con punteros a objetos, en el momento
que estos son alocados, momento en el que se conoce el tipo real (necesario en la 
llamada a new). Lo que hay que hacer es, siempre que se aloque un objeto, llamar a Init
con un molde indicando el tipo de objeto a invocar. Para ello se suministra la macro
siguiente.
  Ejemplo de uso de esta macro:

	BASE *Obj ;
	BASE *Array[4] ;

	Obj      = new CLASE ;
	Array[2] = new CLASE;

	M4_VIRTUAL_OBJECT(Obj,CLASE)->Init(Init particular de CLASE);
	M4_VIRTUAL_OBJECT(&(Array[2]),CLASE)->Init(Init particular de CLASE);

  Puede darse el caso de que haga falta alguna funcion Init auxiliar, por ejemplo para
cargar elementos de un array que pertenece al objeto. A estos metodos se les llamarán
de tipo VIRTUAL INIT, y presentan la misma problemática y reglas que el metodo Init.

  NOTAS: Cuando en BASE haya un objeto cuya construcción se relega a un Init derivado,
	el End() del derivado (no el de la base), debe ser quien lo destruya, o cuando el
	Init derivado llame al Init base, este disparara el End() y destruira el objeto.
*/

#define M4_VIRTUAL_OBJECT(Ptr,Type) ((Type *)(Ptr))

#endif



