//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldevice.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:17/07/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __CLDEVICE__HPP__
#define __CLDEVICE__HPP__

#include "m4stl.hpp"
#include "m4clbuf.h"
#include "m4clflag.h"
#include "font.h"
#include "colour.h"

class ClENG_RasterMap ;
class ClENG_Region ;
class ClENG_PhysicalFile ;
class ClENG_Chn ;


//========================================================================ClENG_OutDevice

/*

  De esta clase derivarán todos los posibles devices de salida. Hay que tener en cuenta
que el resto de la aplicación solo debe conocer y usar los metodos que da esta clase,
que es la genérica.
  Basicamente, lo que hace es:
	-Mantiene tantos juegos buffers en memoria como páginas de Layout tengamos. Los buffers
		son independientes por pagina.
	-Por una parte, hay unas acciones comunes a todos devices, pero cuya implementación es
		dependiente del device. Estas acciones las llevan a cabo las clases derivadas, 
		rellenando los buffers con los valores adecuados. Ejemplo de esto es un salto de
		pagina, o la selección de una fuente.
	-Las clases Field se encargan de obtener y formatear datos provenientes de distintas
		fuentes, y de enviarlos a los buffers haciendo uso de estos metodos tipo 'accion'.
		Los llamaremos metodos tipo 'Action'.
	-Otras clases se encargan de dirigir el orden y secuencia en que se van llenando los
		buffers y se van volcando estos al dispositivo. Llamaremos a estos metodos tipo
		'Flow'.
  NOTAS:

	-El sistema de coordenadas empieza en la esquina superior izquierda, coordenada 0,0.
	-Siempre se usa como medida DvcUnits, que son medidas independientes  del device.
	-Los identificadores de página van del 0 a n.


  
	Diagrama de flujo de metodos Flow:
   --------------------------------------------------------------------------------------
	  
	//---->INICIALIZACION DEL OBJETO DEVICE (1 vez)	

	//SOLO se debe operar a nivel de Buffer (no abrir ficheros ni nada parecido).
	//Se cargan via Buffers u otro metodo todos los datos que necesita un PreReset().
		
	DeviceXX::Init(NumPages)
	for (Pag=0;Pag<NumPages;++Pag) DeviceXX::InitPage(Pag)		//1 reseteo por pagina
	DeviceXX:PostPagesInit() ;

	.....ejecucion de metodos Action para CARGAR datos Optimized Preload, buffer m_oOpPrBuff

	DeviceXX::FixOptimizedPreload() ;	
			  
	.....ejecucion de metodos Action para CARGAR datos Preload, buffer m_oPrBuff 
				
	DeviceXX::FixPreload() ;	
				  
					
	//---->USO DEL OBJETO DEVICE (n veces, 1 por break de fichero o similar)	

	while ( MAS BREAKS ) {	//DEBE HABER AL MENOS UN BREAK!!!!

		//----->PREPARANDO BREAK ACTUAL

		//Haciendo uso de los buffers llenados en la fase de inicializacion, prepara el break.
		//Esto puede significar abrir ficheros, dejar datos fijos en el buffer de impresion, 
		//	grabar alguna cabecera en fichero, etc ...
		//Fijarse qu el PreReset puede ejecutarse n veces, por lo que no hay que destruir los
		//	buffers de inicializacion, si estos son necesarios en esta fase.
		
		DeviceXX::PreReset() ;	
	
		//----->Bucle de impresion de paginas, tantas como se quieran 

		while (Quedan paginas en actual break) {
		
			DeviceXX::RefresLayout() ;	//Preparar bloque de paginas Layout a imprimir

			.........Metodos Action rellenan Paginas Layout, buffer m_oLayoutBuff

			DeviceXX::FlushLayout() ;	//Fin de bloque paginas

		} ;
  
	
		//----->CERRANDO BREAK ACTUAL

		//Tareas tipicas son meter al codigo de fin de impresion, cerrar el fichero, etc.

		DeviceXX::PostReset() ;	
	  
		
	} ;	  
						
						  
	//---->DESTRUCCION DEL OBJETO DEVICE (1 vez)	

	DeviceXX:End() ;	
	
							
							  
	NOTAS:
	Respecto metodos Action, segun en que fase se disparan, escriben a uno de  estos buffers:
		-m_oOpPrBuff. Fase inicializacion, datos Optimized Preload.
		-m_oPrBuff. Fase de inicializacion, datos Preload.
		-m_oLayoutBuff. Fase normal de impresion, datos de pagina.
	Por ello, los metodos action jamas deben acceder directamente a ningun buffer concreto,
		si no obtener el puntero al tipo de buffer actual via m_poActualBuff, y usar este
		buffer en concreto.
			  
	PARA VER DETALLES ESPECIFICOS DE LOS METODOS, VER LA DESCRIPCION DE ESTOS EN LA CLASE.
	
*/

//========================================================================ClENG_Page

class ClENG_Page {

	friend class ClENG_OutDevice ;

MIT_PRIVATE:	
	DvcUnit_t   m_OffsetX, m_OffsetY, m_SizeX, m_SizeY;	
	DvcUnit_t   m_BaseLineY, m_BaseLineX ;
	//Dice cuantos datos del buffer se preservan entre FlushLayout.
	m4uint32_t		m_uiPreserve ;	
	ClFONT_M_Font	* m_poLastFont;
	m4uint32_t		m_uiLastIdColour;

MIT_PUBLIC:
	//Solo para usar a la hora de volcar, por derivadas de ClENG_OutDevice.
	M4ClBuffer  m_oOpPrBuff, m_oPrBuff, m_oLayoutBuff ; 
	M4ClBuffer *m_poActualBuff ; //Apunta a alguno de los tres anteriores

	// ----------------------------- Inicialización ------------------------------	

	ClENG_Page();
	virtual	~ClENG_Page() { ClENG_Page::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Page) ; }

	void Init( DvcUnit_t ai_SizeX, DvcUnit_t  ai_SizeY, 
		DvcUnit_t ai_OffsetX, DvcUnit_t  ai_OffsetY, size_t ai_szBlockBuffer ) ;

	//  Resetea valores y buffer.
	virtual void End() ;

} ;


//====================== Class ClENG_InstanceNode =============================

class ClENG_IndexNode;
//La clave es el m_uiTargetID del nodo en cuestion
typedef map<m4uint32_t,ClENG_IndexNode*, less<m4uint32_t> > IndexNodeList_t;

//Siempre hay un root 'dummy', para contener a los datos de verdad.

class ClENG_IndexNode {

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:
	
	//---------------------------------------Inicializacion--------------------------------
	
	ClENG_IndexNode();
	virtual ~ClENG_IndexNode() { ClENG_IndexNode::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_IndexNode) ; }

	void Init(m4int32_t ai_iIndexLevel, m4uint32_t ai_uiTargetID, 
		      const m4char_t* ai_pcIndexString, m4int16_t ai_iLayoutPages,
			  m4uint32_t ai_uiPrintedPages, m4int16_t ai_iNumPage, ClENG_IndexNode* ai_poFather);

	virtual void End() ;

MIT_PRIVATE:

	m4uint32_t m_iIndexLevel;   //Nivel de profundidad del nodo
	m4uint32_t m_uiTargetID;	//Identificador unico del ancla destino. Es unico por arbol y 
									//se va creando con numeros secuenciales para respetar 
									//el orden de llegada para recorrer con iteradores.
	ClMIT_Str  m_oIndexString;	//Texto asociado a la entrada del indice
	
	m4int16_t  m_iLayoutPages;		//Numero de paginas Layout
	m4uint32_t m_uiPrintedPages;	//Numero de paginas impresas hasta el momento
	m4int16_t  m_iNumPage;			//Numero de pagina del indice

	IndexNodeList_t m_oIndexNodeList; //Lista de nodos hijos ordenados en un map

	ClENG_IndexNode *m_poFather;     //Puntero al nodo padre (referencia directa)

	ClMIT_Str m_oAuxValue ;	//Cadena auxiliar puesta a "". Una implementacion de device
							//	puede usarla para almacenar un valor particular

MIT_PUBLIC:

	//Localiza a uno de los hijos en toda la rama de del arbol. Puede ser el mismo nodo.
	// Si no lo encuentra devuelve NULL
	ClENG_IndexNode* GetNode(m4uint32_t ai_iTargetID, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR);

	//Inserta un nuevo nodo hijo en el arbol, lo hace colgando del nodo this.
	// Se le pasa el puntero al nodo YA INICIALIZADO. El m_uiTargetID se usa como clave en la
	//	lista.
	void InsertSonNode(ClENG_IndexNode* ai_poSonNode);

	//-------------------------- Gets & Sets ------------------------------------------

	m4int32_t GetIndexLevel()		{ return m_iIndexLevel; }
	m4int16_t  GetNumSons()			{ return m_oIndexNodeList.size(); }
	ClENG_IndexNode* GetFather()	{ return m_poFather; }
	IndexNodeList_t* GetNodeList()	{ return &m_oIndexNodeList; }
	m4char_t* GetIndexString()		{ return m_oIndexString.InChar(0); }
	m4uint32_t GetTargetID()		{ return m_uiTargetID; }
	m4bool_t IsLeaveNode()			{ return m_oIndexNodeList.size()==0; }
	m4int16_t GetNumPage()			{ return m_iNumPage; }

	const m4char_t *GetAuxValue()	{ return m_oAuxValue.InChar(0) ; }
	void SetAuxValue(const m4char_t *ai_Str)	{ m_oAuxValue.StrCpy(ai_Str) ; }

	m4uint32_t GetRecursiveNumSons() ;

	m4bool_t IsBrother( ClENG_IndexNode *ai_poIndex ) 
		{ return ai_poIndex->m_poFather==m_poFather ; }

	//True para el root dummy y para sus hijos

	m4bool_t IsRoot() ;

	m4bool_t IsDummyRoot() { return m_poFather==NULL ; }

	//Da una lista de referencias del path a seguir hasta este nodo (es decir, sus padres).
	//El root dummy no entra en la cuenta y el propio nodo tampoco.
	//Como la clave es el id secuencial, el map esta en orden.

	void GetPathToThis( IndexNodeList_t *ao_poList ) ;

MIT_PRIVATE:

	ClENG_IndexNode* GetSonNodeAux(m4uint32_t ai_iTargetID);
	void GetRecursiveNumSons(m4uint32_t* ai_piSons) ;

};


//========================================================================StENG_DvcParams
//
//Esta clase simplemente recopila enums y utilidades que son comunes a algunos devices y a
// otros no porque no los usan.

struct StENG_DvcParams 
{
	//Hay otro duplex a nivel de file, no confundir.
	enum DUPLEX_MODE { NO_DUPLEX, EMULATED_DUPLEX, REAL_DUPLEX } ;

	enum TUMBLE_MODE { NO_TUMBLE, TUMBLE_FALSE, TUMBLE_TRUE }; 


} ;


//========================================================================ClENG_OutDevice

class ClENG_OutDevice {	

MIT_PROTECTED:
	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	// Del bit 0 al 10, flags publicos que se pueden usar en Init
	enum { 
		PRELOAD_ON_DEVICE		=ClMIT_Flag::BIT0,	//Ver metodos flow

		//Dice si el device soporta la gestion de color

		COLOUR_SUPPORTED		=ClMIT_Flag::BIT1,

		//Util solo si COLOUR_SUPPORTED, dice si se hace uso de la capacidad de color.
		//Internamente se pone a FALSE si ! COLOUR_SUPPORTED

		COLOUR_ACTIVATED		=ClMIT_Flag::BIT2,

		// Si !RASTER_SUPPORTED, se pondra incondicionalmente ! RASTER_PRELOADED 

		RASTER_SUPPORTED		=ClMIT_Flag::BIT3,	//Graficos ... 
		RASTER_PRELOADED		=ClMIT_Flag::BIT4,	//
		HARD_DITHERING			=ClMIT_Flag::BIT5,	//
		HARD_SCALE				=ClMIT_Flag::BIT6	//	... ver despues
											
	} ;

MIT_PRIVATE:

	// Del bit 11 al 15, flags internos que NO se pueden usar en Init
	enum { 

		//Esta clase no hace uso efectivo del flag, pero algunas derivadas 
		//	pueden usarlo para invalidar salidas. Por ejemplo, si se producen errores, para
		//	no imprimir a fichero. El Init lo pone a TRUE. 

		VALID_OUTPUT			=ClMIT_Flag::BIT11

	} ;
	ClMIT_Flag m_oFlag ;

	m4int16_t  m_iNLayoutPages;
	m4uint32_t m_uiNPageDvc;	//Numero de m_iNPageDvc volcadas.
	m4uint32_t m_uiNPageBB ;	//m_uiNPageDvc en el ultimo PreReset (BreakBegin).
	m4uint32_t m_uiBreakCount ;	//Numero de breaks actual (1 tras abrir el primer break). 
	m4int16_t  m_iNPhysicalPages;

	// Bug 0102841. Me indica si tengo páginas replicadas, como en el caso de los CTs 
	// que se crean las páginas en ejecución si el CT no cabe en una sola página, o no.
	m4bool_t m_bReplicatePages;

MIT_PROTECTED:	

	ClENG_Page *m_poLayoutPages ;	//Array dinámico de ClENG_Pages

	//  Paletas de color. Estas paletas existen se soporte o no el color. Por defecto, el Init del
	//device las resetea.
	//	Otra cosa es que el device las use o no. En caso de que no se use el color, cualquier 
	//accion de color se ignora, aunque el device reciba la accion, por lo que la capa usuaria de 
	//los devices normalmente no sabe ni se preocupa de si hay o no color.
	//
	//	Para saber si el color esta activado o no, usar IsColourOn() 
	//
	//	Las distintas paletas son:
	//	-FG palette. ForeGround color. Colores para fonts.

	ClENG_Palette m_oFGPalette ;

	//Arbol de indices. El dispositivo mantiene un arbol de entradas en la Tabla de Contenidos
	//  de un report, que van metiendose dinamicamente segun se va recorriendo el informe.
	//El Nodo Raiz no contiene ninguna informacion, sino que tiene los hijos de primer nivel

	ClENG_IndexNode m_oIndexRootNode;
	ClENG_IndexNode* m_poLastAccessedNode;
	m4uint16_t m_uiIndexTotal;
	
	//Contiene un array con links a los indices root para el actual break. Puede no haber ninguno.
	//Este array se consigue almacenando la primera entrada TOC que llega despues de abrir 
	//	un nuevo break, y almacenando luego solo las entradas hermamas a esta que le lleguen.

	typedef vector<ClENG_IndexNode *> IndexVector_t;
	IndexVector_t m_oBreakIndexRoots ;

MIT_PUBLIC:

	enum { DVC_NO_INDEX_TARGET=0 };

	//------------------------------ Entrada en el arbol de indices --------------

	m4bool_t NewIndexEntry(m4int16_t ai_iNumPage, m4int32_t ai_iIndexLevel, const m4char_t* ai_pcIndexString, m4uint16_t* ai_iTargetID);

MIT_PUBLIC:

	// ----------------------------- Inicialización (metodos Flow)----------------	

	ClENG_OutDevice();
	virtual	~ClENG_OutDevice() { ClENG_OutDevice::End(); }

	virtual size_t GetSizeof() { return sizeof(ClENG_OutDevice) ; }

	//REGLAS PARA DERIVAR: 
	//
	//ClENG_DvcDerivated::End() {
	//	Destruccion objeto derivado
	//	ClENG_OutDevice::End() ;
	//} ;

	virtual void End() ;

	//QUE HACE
	// Da true si todo va bien. Entre otras cosas, crea los buffers (pero NO los inicializa).
	// ai_poFGPalette se copia a m_oFGPalette. Si es NULL, se inicializa m_oFGPalette con 0.
	//
	//REGLAS PARA DERIVAR: 
	//
	//ClENG_DvcDerivated::Init() {
	//	ClENG_DvcDerivated::End() ;
	//	ClENG_OutDevice::Init() ;
	//} ;

	m4bool_t Init( m4int16_t ai_iNumPages, 
		ClMIT_Flag::Value_t ai_Flags,	//Validos los flags publicos
		ClENG_Palette *ai_poFGPalette=NULL,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR);

	//QUE HACE
	//
	//  Este metodo DEBE ser implementado en las derivadas. Es el encargado de inicializar los
	//		objetos agregados necesarios por pagina, propios de la derivada, y de inicializar
	//		los buffers de ClENG_OutDevice. Como es una llamada de construccion, el prototipo
	//		varia segun la clase derivada.
	//	DEBE testear que el numero de pagina es valido (entre 0 y GetNumLayoutPages()-1)
	//	DEBE inicializar los buffers de m_poLayoutPages[Page].
	//	El ClENG_Page::Init() hace que m_poActualBuff apunte a m_oOpPrBuff (que es el que toca). 

	//m4bool_t InitPage(Page) 


	//VIRTUAL SUSTITUIBLE
	//
	//QUE HACE
	// Simplemente se llama tras hacer los Init de todas las paginas, por si el 
	//	dispositivo quiere inicializar datos dependientes de valores de pagina.

	virtual m4bool_t PostPagesInit() { return M4_TRUE ; } 


	//VIRTUAL EXTENSIBLE.
	//	
	//QUE HACE
	// Actualiza m_poActualBuff para que apunte a m_oPrBuff. 
	// Tambien, como en el RefreshLayout, hace un ForceSelectFont y ForceSelectFG
	// Normalmente NO hace falta derivarlo.
	//
	//REGLAS PARA DERIVAR: 
	//
	//ClENG_DvcDerivated::FixOptimizedPreload() {
	//	....Hacer lo que sea; m_oOpPrBuff YA esta actualizado por metodos action.
	//	ClENG_OutDevice::FixOptimizedPreload() ;	//LLamar al final.
	//} ;

	virtual void FixOptimizedPreload() ;

	//VIRTUAL EXTENSIBLE

	//QUE HACE
	// Actualiza m_poActualBuff para que apunte a m_oLayoutBuff. 
	// Normalmente NO hace falta derivarlo.
	//
	//REGLAS PARA DERIVAR: 
	//
	//ClENG_DvcDerivated::FixPreload() {
	//	....Hacer lo que sea; m_oOpPrBuff y m_oPrBuff YA esta actualizado por metodos action.
	//	ClENG_OutDevice::FixPreload() ;	//LLamar al final.
	//} ;

	virtual void FixPreload();

	// ----------------------------- Varios metodos Flow---------------------------------	


	//VIRTUAL EXTENSIBLE

	//QUE HACE
	//	MUY IMPORTANTE. Prepara el entorno de un break. Normalmente, SE DERIVA.
	//	Devuelve false si no pudo preparar el break (por ejemplo, no pudo crear un fichero).
	//		En ese caso, la ejecucion se parara (se llamara al End, pasando de los demas 
	//		metodos como PostReset, etc). 
	//	La clase base siempre retorna true. Actualiza m_uiNPageBB y m_uiBreakCount.
	//	
	//REGLAS PARA DERIVAR: 
	//	
	//	Tiene disponibles los buffers m_oOpPrBuff y m_oPrBuff ya actualizados. Pueden usarse
	//		pero normalmente NO deben destruirse, pues se pueden producir N breaks.
	//	Aqui se realizan tareas tales como abrir ficheros, volcar macros a el, etc.
	//	Pueden volcarse datos a m_oLayoutBuff. Los que se vuelquen, se marcan como Preserve,
	//		y seran repetidos por cada FlushLayout. Como algunos devices meten aqui grandes
	//		cantidades de datos (graficos optimizados, por ejemplo), puede realizarse esto
	//		solo en el primer break (usar ifs con GetBreakCount(). La primera vez da 0, 
	//		pues lo actualiza ClENG_OutDevice::PreReset y este aun no se ejecuto).
	//
	//ClENG_DvcDerivated::PreReset() {
	//	....Hacer lo que sea
	//	ClENG_OutDevice::PreReset() ;	//Llamar al final.
	//} ;

	virtual m4bool_t PreReset() ;

	//VIRTUAL EXTENSIBLE

	//QUE HACE
	// MUY IMPORTANTE. Limpia o cierra lo que hizo PreReset. Finaliza el entorno break. 
	//	Normalmente, SE DERIVA.
	//	PostReset sin parametros es la que se sobrecarga en clases instanciables, y que
	//		usa PostReset con el parametro ai_bEmptyPreserveData ya predefinido, puesto
	//		que depende del device en cuestion.
	//	
	//REGLAS PARA DERIVAR: 
	//	
	//	Aqui se realizan tareas tipicas de cierre de ficheros, comandos fin de job, etc. 
	//		Estos comandos no deberian de enviarse via buffer, pues hay datos Preserve
	//		en el que serian tambien enviados.
	//	Los datos marcados como Preserve, se respetan o se eliminan en funcion de
	//		lo que le indiquemos en ai_bEmptyPreserveData. El funcionamiento debe ir en 
	//		conjuncion con el de PreReset: si PreReset carga datos Preserve siempre, 
	//		PostReset deberia de borrarlos siempre, y si PreReset los carga solo la 
	//		primera vez, PostReset no deberia destruirlos nunca (el End() lo hara).
	//
	//ClENG_DvcDerivated::PostReset() {
	//	....Hacer lo que sea
	//	ClENG_OutDevice::PostReset() ;	//Llamar al final.
	//} ;

MIT_PROTECTED:

	virtual void PostReset(m4bool_t ai_bEmptyPreserveData) ; 

MIT_PUBLIC:
	virtual void PostReset()=0 ;


	//VIRTUAL SUSTITUIBLE

	//Este método se llama tras ejecutar el ultimo Reset, antes del End
	//No hay un NotifyFirstReset porque dentro del PreReset se puede saber preguntando  por
	//	GetBreakCount()==0
	//Util para devices que hagan cosas fuera de los Breaks. 

	virtual void NotifyLastReset() { ; } 

	//VIRTUAL EXTENSIBLE.

	//QUE HACE
	//
	// Prepara la impresion de un bloque de paginas layout.
	// La base hace un ForceSelectFont en todas paginas con la ultima font usada por pagina,
	//	como precaucion de que una macro pueda cambiar la font y desincronizar lo que 
	//	GetLastFont() devuelve y la font que realmente tiene activa el dispositivo.
	// Lo mismo es valido para ForceSelectFG.
	// Normalmente, NO SE DERIVA.

	//REGLAS PARA DERIVAR: 
	//	
	//ClENG_DvcDerivated::RefreshLayout() {
	//	....Hacer lo que sea
	//	ClENG_OutDevice::RefreshLayout() ;	//Llamar al final.
	//} ;

	virtual m4bool_t RefreshLayout() ;

	//VIRTUAL EXTENSIBLE.

	//QUE HACE
	//	Volcar datos de pagina al dispositivo. 
	//	La base vacia los buffers m_oLayoutBuff, a excepcion de la parte Preserve. Tambien
	//		actualiza m_uiNPageDvc.
	//  MUY IMPORTANTE, normalmente, SE DERIVA.

	//REGLAS PARA DERIVAR: 
	//	
	// Vuelca el buffer m_oLayoutBuff (datos Preserve y los introducidos por metodos action)
	//	al dispositivo.
	//
	//ClENG_DvcDerivated::FlushLayout() {
	//	RunAllEOP() ;	//Llamar primero. Puede que actualize m_oLayoutBuff.
	//	....Hacer lo que sea (volcar datos).
	//	ClENG_OutDevice::FlushLayout() ;	//Llamar al final. Vacia m_oLayoutBuff.
	//} ;

	virtual m4bool_t FlushLayout();

	// Dispara todos los EOP

	void RunAllEOP() ;
 
MIT_PROTECTED:

	//VIRTUAL EXTENSIBLE, End Of Page. Ligada a FlushLayout.

	//QUE HACE
	//	Meter posibles codigos de fin de pagina en el buffer m_oLayoutPage, o tareas 
	//		similares.
	//	La base vacia actualiza las lineas base.

	//REGLAS PARA DERIVAR: 
	//	
	//ClENG_DvcDerivated::EOP() {
	//	....Hacer lo que sea 
	//	ClENG_OutDevice::EOP() ;	//Llamar al final. Actualiza lineas base.
	//} ;

	virtual void EOP(m4int16_t ai_iPage) 
		{ SetBaseLineY(ai_iPage,0); SetBaseLineX(ai_iPage,0);  } 

MIT_PUBLIC:

	// La fase preload puede implementarse sobre el device o externamente. 
	// Implementarla sobre el device significa usar las reglas de la fase preload, y disparar
	//	los metodos action en esta fase. Los bytes generados por estos metodos se volcaran
	//	secuencialmente en cada FlushLayout. Por lo tanto, cualquier objeto que genero las
	//	llamadas a los metodos action, puede destruirse, al no hacer falta. Otra 
	//	consideracion a tomar en cuenta es que estos datos van ANTES que otros en el buffer
	//	(e incluso primero los optimizados), lo que significa que, por ejemplo en impresora,
	//	un grafico preload sera sobreescrito por otros datos no preload, y que un grafico
	//	preload optimizado sera sobreescrito por otro preload no optimizado.
	// Implementarla fuera significa NO llamar a los metodos action en la fase preload, 
	//	si no encargarnos nosotros de hacerlo cuando queramos, siempre entre las llamadas 
	//	RefreshLayout y FlushLayout, y normalmente antes de disparar metodos action de 
	//	objetos normales.
	//
	// Respecto al engine, todo lo comentado antes significa que un :
	//		-Un dispositivo IsPreloadOnDevice() puede tener datos que tratar en PreReset,
	//			en los buffers m_oOpPrBuff y m_oOpBuff
	//		-Un dispositivo !IsPreloadOnDevice() no tiene datos que tratar en PreReset,
	//			en los buffers m_oOpPrBuff y m_oOpBuff. Los datos preload se generan y
	//			envian en cada llamada.

	// Este metodo es simplemente un switch cara al exterior, indicando la forma de 
	//	funcionamiento a otras capas usuarias de devices; esta clase, de hecho, no lo usa
	//	para nada.

	m4bool_t IsPreloadOnDevice() { return m_oFlag.True(PRELOAD_ON_DEVICE) ; }
	
	//------------------------------ Metodos Get------------------------------------------

MIT_PUBLIC:

	m4int16_t GetNumLayoutPages() { return m_iNLayoutPages ; }

	m4int16_t GetNumPhysicalPages() {return m_iNPhysicalPages; }

	m4uint32_t GetPagesDvcPrinted() { return m_uiNPageDvc ; }

	m4uint32_t GetBreakPagesDvcPrinted() { return m_uiNPageDvc-m_uiNPageBB ; }

	// Ojo, ai_iPage (pagina de Layout) va de 0 a n. El numero de pagina de dispositivo 
	//   correspondiente va de 1 a n.
	m4uint32_t GetNumPageDvc(m4int16_t ai_iPage) { return m_uiNPageDvc+ai_iPage+1 ; }

	m4uint32_t GetNumBreakPageDvc(m4int16_t ai_iPage) { return GetBreakPagesDvcPrinted()+ai_iPage+1 ; }

	//Va de 1 a n. Dentro del PreReset derivado, da -1 respecto del actual, ya que 
	//	precisamente el ClENG_OutDevice::PreReset() es el que actualiza este valor, y en ese
	//	punto aun no se ha llamdo. Por ejemplo, en la primera llamada PreReset, da 0
	
	m4uint32_t GetBreakCount() { return m_uiBreakCount ; }

	m4bool_t GetReplicatePages() { return m_bReplicatePages ; }
	void SetReplicatePages(m4bool_t ai_bReplicatePages) { m_bReplicatePages = ai_bReplicatePages; }

	// VIRTUAL SUSTITUIBLE.
	//
	// -Para devices que no vuelquen a fichero siempre devuelve NULL.
	// -Si un dev ice vuelca a fichero, devuelve el flush file asociado o NULL si se llama 
	//	desde un sitio incorrecto.
	//		-Para modos ATOMIC_DUPLEX, esta función solo devolverá algo no NULL si se llama
	//			desde dentro de un bucle flush.
	//		-Para el resto de modos, puede ser llamado desde dentro de un flush o desde dentro
	//			de un break.
	// -Puede ser que el fichero se cierre a posteriori.Esto es testeable con GetState(). 
	// -NOTA: el ai_iNPage debe ser un numero de pagina layout valido.

	virtual ClENG_PhysicalFile *HasFlushFile(m4int16_t ai_iNPage) { return NULL ; }

	// Virtual pura. Definida en la derivada ClENG_DvcFile. 
	// Obtiene información sobre donde esta la salida a efectos de navegacion. 
	// Detalles en ClENG_ReportOuputRepository.

	virtual void GetNavigationPathMask(ClMIT_Str *ao_Str)=0 ;

	DvcUnit_t GetSizeX(m4int16_t ai_iPage) { return m_poLayoutPages[ai_iPage].m_SizeX ; } 
	DvcUnit_t GetSizeY(m4int16_t ai_iPage) { return m_poLayoutPages[ai_iPage].m_SizeY ; } 
	DvcUnit_t GetOffsetX(m4int16_t ai_iPage) { return m_poLayoutPages[ai_iPage].m_OffsetX ; } 
	DvcUnit_t GetOffsetY(m4int16_t ai_iPage) { return m_poLayoutPages[ai_iPage].m_OffsetY ; } 
	ClFONT_M_Font   *GetLastFont(m4int16_t ai_iPage) {return m_poLayoutPages[ai_iPage].m_poLastFont ; }  
	m4uint32_t GetLastColour(m4int16_t ai_iPage) {return m_poLayoutPages[ai_iPage].m_uiLastIdColour ; }  

	m4bool_t IsValidOutput() { return m_oFlag.TrueAND(VALID_OUTPUT) ; }
	void SetValidOutput(m4bool_t ai_bSw) { m_oFlag.Switch(VALID_OUTPUT,ai_bSw) ; }


	m4bool_t IsColourOn() { return m_oFlag.TrueAND(COLOUR_SUPPORTED|COLOUR_ACTIVATED) ; }
	m4bool_t IsColourSupported() { return m_oFlag.TrueAND(COLOUR_SUPPORTED) ; }

	m4bool_t IsRasterSupported() { return m_oFlag.TrueAND(RASTER_SUPPORTED) ; }
	m4bool_t IsRasterPreloaded() { return m_oFlag.TrueAND(RASTER_PRELOADED) ; }
	m4bool_t IsHardDithering() { return m_oFlag.TrueAND(HARD_DITHERING) ; }
	m4bool_t IsHardScale() { return m_oFlag.TrueAND(HARD_SCALE) ; }

	//   Actualmente, lo unico que esta clase y las derivadas hacen con las BaseLines es 
	//ponerlas a 0 al principio, y cada EOP. Pueden ser utiles para otras clases.
	//	Fijarse que IncrBaseLine puede operar con offsets positivos y negativos. 
	//  Estos valores son relativos al offset de pagina (osea, al inicializarlos se 
	//ponen a 0, no al offset de pagina).
	//	Estos metodos, por rapidez, no testean limites de arrays, por lo que ai_iPage debe
	//	ser correcto.

	DvcUnit_t GetBaseLineY(m4int16_t ai_iPage) 
		{ return m_poLayoutPages[ai_iPage].m_BaseLineY ; }
	void SetBaseLineY( m4int16_t ai_iPage, DvcUnit_t ai_BaseLineY ) 
		{ m_poLayoutPages[ai_iPage].m_BaseLineY=ai_BaseLineY ; }
	void IncrBaseLineY( m4int16_t ai_iPage, DvcUnit_t ai_OffsetY ) 
		{ m_poLayoutPages[ai_iPage].m_BaseLineY+=ai_OffsetY ; }

	DvcUnit_t GetBaseLineX(m4int16_t ai_iPage) 
		{ return m_poLayoutPages[ai_iPage].m_BaseLineX ; }
	void SetBaseLineX( m4int16_t ai_iPage, DvcUnit_t ai_BaseLineX ) 
		{ m_poLayoutPages[ai_iPage].m_BaseLineX=ai_BaseLineX ; }
	void IncrBaseLineX( m4int16_t ai_iPage, DvcUnit_t ai_OffsetX ) 
		{ m_poLayoutPages[ai_iPage].m_BaseLineX+=ai_OffsetX ; }

	//Virtuales para manipular el flag run-time de break
	virtual m4bool_t IsDelBreak()=0 ; 
	virtual void SetDelBreak(m4bool_t ai_bDelBreak)=0 ;
	virtual m4uint32_t GetPublishedBreakCount() { return  0; }


	//------------------------------ Metodos Action---------------------------------------
	// Actuan sobre el buffer de memoria. Los usan mas que nadie las clases Field, y la
	//   mayoria son implementados por las derivadas.

	//----------Imprimir cadenas de caracteres

	//  Optimiza la selección de fuentes: si la ultima fuente seleccionada en la página es 
	//la misma, no hace nada. Si es distinta, llama a SelectFontDevice. Cualquier 
	//operacion PrintStr posterior utilizara la fuente seleccionada.

	void SelectFont( m4int16_t ai_iPage,  ClFONT_M_Font *ai_poFont ) ;

	//Como SelectFont, pero sin optimizacion:siempre se selecciona la font

	void ForceSelectFont( m4int16_t ai_iPage,  ClFONT_M_Font *ai_poFont ) ;


	//Exactamente igual a SelectFont y ForceSelectFont, pero para el color.
	//Llaman siempre (tenga o no activado el color) a SelectColourDevice(). Si hace falta,
	//	dentro de esta funcion se puede preguntar por IsColoyrOn para hacer lo que sea.

	void SelectColour( m4int16_t ai_iPage,  m4uint32_t ai_uiIdColour ) ; 
	void ForceSelectColour( m4int16_t ai_iPage,  m4uint32_t ai_uiIdColour ) ;



	//VIRTUAL PURA.
	//	-Acordarse de sumar a ai_X GetOffsetX(ai_iPage) y a ai_Y GetOffsetY(ai_iPage)
	//  -Otro tema a tener en cuenta es el siguiente: las coordenadas que le llegan al 
	//	device son siempre el top-left de la cadena. Sin embargo, en muchos dispositivos
	//	estas coordenadas se interpretan como bottom-left (o linea base). En estos casos,
	//	tambien deberia sumarse a Y la GetDvHeight de la LastFont correspondiente. 
	//	-Lo anteriormente comentado es valido tambien para graficos, pero sin embargo casi
	//	todos devices si interpretan las coordenadas como top-left.
	
	struct StPIParams {
		//Argumento de entrada, PI que se esta imprimiendo.
		ClMIT_Str m_oIdPi;
		//Argumento de salida, posición en el buffer donde va a comenzar ai_pcStr una vez 
		//	que se hayan escrito posibles caracteres de escape,posicionamiento....
		m4uint32_t m_uiOff ;	
		//Argumento de salida. Caracteres que realmente se escriben. Si hay algun problema,
		//	este valor es 0, y no deberia crearse el PI.
		size_t m_szNChars ;	
	} ;

	// Si ai_pstPI es NULL, no se esta impimiendo ningun PI.
	// ai_bRawText dice que la cadena conteien los codigos a sacar directamente (util para
	//	codigos de barra). La cadena no debe sufrir ninguna transformacion de caracteres 
	//	ANSI a ISO ni nada parecido.

	virtual void PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, 
		m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=DVC_NO_INDEX_TARGET, StPIParams *ai_pstPI=NULL)=0 ; 	


    //---------- Imprimir Pi (Impresiones Pendientes)
	
	//VIRTUAL PURA
	//se le indica a la capa Device que se debe añadir otro item de pi, pasándole como argumento
	// el identificador de Pi, la pagina, su posicion y el número de bytes.
	//DEBE de testearse que el buffer usado es m_oLayoutPage, ya que una PI NO puede meterse
	//	de ninguna forma como dato PreLoad. Tambien puede optarse (y asi se ha hecho) a 
	//	relegar este control a otra capa. En concreto, ahora lo controla la capa Field.

	virtual void PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		 size_t ai_szNChar, 
		 m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID=DVC_NO_INDEX_TARGET) = 0;
	

	//VIRTUAL PURA
	// habrá que ir a buscar el elemento identificado por ai_PiHandle e imprimirlo en el lugar
	//adecuado rellenando teniendo cuidado de no sobreescribir. 
	//Finalmente se elimina el elemento.

	virtual void ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) = 0;


	//VIRTUAL SUSTITUIBLE
	//  Puede sustituirse por optimizaciones. Esta version usa PrintNChar.
	//  NO suma los offset de pagina, porque llama a PrintNChar y normalmente es este metodo
	//		el que los suma.
	virtual void PrintStr( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
						   m4pchar_t ai_pcStr, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink,
						   m4uint16_t ai_iTargetID=DVC_NO_INDEX_TARGET ); 	

	//----------Imprimir lineas, frames y regiones

	//Estilo de la linea,  solo debe permitirse un conjunto predeterminado de valores, 
	// representables con más o menos fidelidad en todos los dispositivos. 
	//Parametrice lo que se parametrice para HTML, el device reolverá siempre solido.

	// No alterar valores enum , se serializan en RVW

	enum STYLE { SOLID=0, DASH, DOT, DASH_DOT, DASH_DOT_DOT } ;

	//VIRTUAL SUSTITUIBLE
	//	-Acordarse de sumar a ai_X1 y ai_X2 GetOffsetX(ai_iPage) y a ai_Y1 y ai_Y2, 
	//		GetOffsetY(ai_iPage)
	//  -Las coordenadas de las lineas, independientemente del grosor, se refieren al eje central
	//		de la linea.
	//	-El color, como en las fonts, es el ultimo usado.
	//	-Asegurarse que, a nivel de salida de device, uan linea siempre sale, aunque la LineWidth
	//		sea menor que la anchura de la menor linea que el device es capaz de imprimir.

	virtual void PrintLine( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
		DvcUnit_t ai_LineWidth, STYLE ai_Style
	) { ;}

	//VIRTUAL SUSTITUIBLE
	//Mismas consideraciones que PrintLine
	//La sobrecarga es opcional, la función por defecto resuelve llamando a PrintLine

	virtual void PrintFrame( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineWidth, STYLE ai_Style
	) ;

	//VIRTUAL SUSTITUIBLE
	//Mismas consideraciones que PrintLine. Rellena un area del color actualmente activo.

	virtual void FillZone( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height
	) {; }


	//VIRTUAL SUSTITUIBLE
	//Estas dos funciones imprimen regiones, y son llamadas por el objeto Region en el Show.
	//Los metodos defecto de esta capa vuelven a llamar al objeto Region para que muestre 
	//	la region, disparandose en ultima instancia, según proceda, 0, 1 metodo 
	//	PrintFrame ó varios PrintLine, y 0 ó 1 FillZone.
	//Si algún device NO puede representar grosores de linea con fidelidad, debería sobrecargar
	//	estos metodos, para hacer ajustes, ya que si imprime grosores inferiores y hay color de
	//	relleno en el area, acabaría viendose una orla interior vacía en el Frame. Estos metodos
	//	podrían crearese un objeto Region auxiliar, copiar el de entrada, actualizar con los 
	//	grosores que de verdad va a representar el Dvc, y llamar a los ShowOnDvc de la Region.

	void ShowRegion( 
		ClENG_Region *ai_poRegion,
		m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) ;
	void ShowRegion( 
		ClENG_Region *ai_poRegion,
		m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) ;


	//----------Imprimir gráficos

	//	Para imprimir en un device, hay que escalar por fuerza, porque intervienen varios
	//factores. En principio, solo sabemos que queremos imprimir un bitmap en un area 
	//ponderada en DvcUnits, requisito imprescindible para hacer diseños independientes de los
	//dispositivos de salida.
	//	-Primero, hay que saber a que equivalen las DvcUnits del area a imprimir, en 
	//		raster-lines de dispositivo. Osea, unas DvcUnits X en impresora pueden ser
	//		20 lineas de raster-lines de impresora (o lineas de dosts), y en pantalla 15
	//		raster-lines de pantalla (o lineas de pixels.)
	//	-Segundo, hay que escalar las raster-lines de bitmap a las raster-lines de dispositivo,
	//		e ir mandando cada linea al dispositivo, para que las muestre.
	//
	//	Otro punto a considerar es que los bitmaps son voraces consumidores de memoria, por
	//lo que hay que tener cuidado con los colores usados y la resolucion a la que se almacena.
	//Además, segun cual sea el dispositivo de salida, es absurdo usar resoluciones altas de 
	//bitmap, si luego los dispositivos destino no son capaces de representar esas resoluciones.
	//
	//Hay unos flags en los devices que influyen en la gestion de los graficos:
	//
	//	-RASTER_SUPPORTED. Si esta a false, el Dvc no soporta graficos raster, aunque todo el 
	//		proceso sigue su curso normal. Si esta a false,  RASTER_PRELOADED tambien lo
	//		estara, para evitar cargas innecesarias de ficheros.
	//		Este flag es util para puentear cargas innecesarias de items BLOB.
	//	-RASTER_PRELOADED. Indica que un cliente de la capa device nos va a pasar los graficos
	//		via PrintRasterMap, pasandonos un objeto ClENG_RasterMap que ya ha sido cargado, y en 
	//		el que -si estan activados los flags pertinentes- ya se ha hecho la escala y 
	//		dithering soft pertinente. 
	//		Si el switch esta a false, los graficos se pasaran via PrintRasterFile, en cuyo caso
	//		la capa cliente solo nos da el nombre de fichero, y la capa device se ocupa de todo.
	//	-HARD_DITHERING. Dice si el device soporta dithering por hardware. Es decir, las capas
	//		clientes del device NO deberian hacer dithering por soft, el device se las apañara.
	//	-HARD_SCALE. Lo mismo que antes, pero referente a la escala.
	//

	// VIRTUAL VACIA

	// Esta funcion SOLO es llamada para saber a cuanto escalar un grafico, ya que
	//el device comunica al cliente que, en este device en concreto, una area de 
	//SizeX*SizeY DvcUnits equivalen a RasterXDvc*RasterYDvc. Se pasan tambien los 
	//RasterX/Y del raster por si influyen en el calculo (por ejemplo, en PCL, puede jugarse
	//con los dpi de resolucion y grafico para optimizar ampliaciones).
	//	Acordarse de sumar a ai_X GetOffsetX(ai_iPage) y a ai_Y GetOffsetY(ai_iPage)
	//	Si un device implementa HARD_SCALE, esta funcion no tiene porque usarse.

	virtual void GetScaleData( 
		DvcUnit_t ai_SizeXDvc,			DvcUnit_t ai_SizeYDvc,			//Area a imprimir.
		m4uint32_t ai_iRasterXMap,	m4uint32_t ai_iRasterYMap,					//Size bitmap
		m4uint32_t *ao_piRasterXDvc, m4uint32_t *ao_piRasterYDvc ) 
		{ ; } 

	// VIRTUAL VACIA

	//	Solo si un device es RASTER_PRELOADED, esta funcion debe usarse.
	//  Se siguen pasando ai_SizeX y ai_SizeY porque el RasterMap pueda o NO estar ya escalado.
	//		Ademas, aunque lo estuviese, puede seguir siendo necesarios estos datos para 
	//		calcular algun parametro adicional, como datos referentes a resoluciones en 
	//		el device.
	//  Acordarse de hacer el ai_X += GetOffsetX(ai_iPage) y el ai_Y += GetOffsetY(ai_iPage) ;

	virtual void PrintRasterMap(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,				//Coordenadas de comienzo.
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,			//Area a imprimir.
		ClENG_RasterMap * ai_poMap, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=DVC_NO_INDEX_TARGET
	) {;}
	

	// VIRTUAL VACIA

	//	Solo si un device es NOT RASTER_PRELOADED, esta funcion debe usarse.
	//  Acordarse de hacer el ai_X += GetOffsetX(ai_iPage) y el ai_Y += GetOffsetY(ai_iPage) ;

	virtual void PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,				//Coordenadas de comienzo.
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,			//Area a imprimir.
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=DVC_NO_INDEX_TARGET,
		m4bool_t ai_bIsTempWebPath=M4_FALSE
	) {;}

	//----------Varios

	// VIRTUAL SUSTITUIBLE. Sobrecargar en Dvcs para los que se quiera ajustar la altura
	//	de los container.
	virtual DvcUnit_t AdjustHeight( m4int16_t ai_iPage, DvcUnit_t ai_Height ) 
		{ return ai_Height ; }
	virtual DvcUnit_t AdjustWidth( m4int16_t ai_iPage, DvcUnit_t ai_Width ) 
		{ return ai_Width ; }

	// VIRTUAL SUSTITUIBLE. Sobrecargar en Dvcs que soporten publicacion
	virtual m4bool_t PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) 
		{ return M4_TRUE;}


MIT_PROTECTED:

	// VIRTUAL PURA

	virtual void SelectFontDevice( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont )=0 ;

	// VIRTUAL SUSTITUIBLE
	// Solo sera llamada si IsColourOn().
	// Pone el color foreground del device.

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour ) {;} 

	//------------------------------------------------------------------------------------
} ;

#endif

