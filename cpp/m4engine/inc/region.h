//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             region.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/18/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//
//==============================================================================

#ifndef __REGION__H__
#define __REGION__H__

struct StENG_TaskEnv ;
#include "cldevice.h"

//=================================================================================ClENG_Line

class ClENG_Line {

	//El objeto linea puede no representarse en algunos devices. 
	//-En RVW, PCL, PSC y PDF son completamente funcionales.
	//-En HTML solo pueden representarse lineas horizontales y verticales (pendiente 0 ó 1).
	//-Ni en TXT ni en ESCP se representan.

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	ClENG_OutDevice * m_poDvc ; //Simple link

	//Coordena comienzo. Es relativa a las coordenadas que se le pasan al resolver. 
	//En el caso de frames, las coordenadas pueden ser negativas, para repartir espacio de borde 
	//  fuera de la región.
	// Considerando grosores, las coordenadas que usamos son simpre fijas, es decir, apuntan al 
	//	eje central de la linea: hacer la linea más ancha es repartir grosor a ambos lados, no 
	//	alterar las coordenadas.

	DvcUnit_t m_Top, m_Left, m_Top2, m_Left2 ;	


	// Anchura de la linea en Dvc.

	DvcUnit_t m_Width ;

	// Estilo

	ClENG_OutDevice::STYLE m_Style ;

	//IdColour linea.

	m4uint32_t m_uiIdColour ;	


	//Si no es visible, los Show se ignoran.

	m4bool_t m_bSwVisible ;

	// Determina si la línea debe ajustar su altura al contenedor
	m4bool_t m_bAdjustLineToContainer ;

MIT_PUBLIC:

	ClENG_Line();
	virtual ~ClENG_Line() { ClENG_Line::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_Line) ; }

	//-------------------------Inicializaciones-----------------------------------------

	virtual void End() ;

	void Init( 
		ClENG_OutDevice * ai_poDvc,
		DvcUnit_t ai_Top, DvcUnit_t  ai_Left, DvcUnit_t ai_Top2, DvcUnit_t ai_Left2,
		m4bool_t ai_bAdjustLineToContainer,
		DvcUnit_t ai_Width, ClENG_OutDevice::STYLE ai_Style, m4uint32_t ai_uiIdColour, 
		m4bool_t ai_bSwVisible
	);
	
	//Constructor copia

	void Init( ClENG_Line *ai_poSource ) ;

	//-------------------------Gets / Sets ----------------------------------------------
	
	DvcUnit_t GetTop() { return m_Top ; }
	DvcUnit_t GetLeft() { return m_Left ; }
	DvcUnit_t GetTop2() { return m_Top2 ; }
	DvcUnit_t GetLeft2() { return m_Left2 ; }
	DvcUnit_t GetWidth() { return m_Width ; }
	ClENG_OutDevice::STYLE GetStyle() { return m_Style ; } 
	m4uint32_t GetIdColour() { return m_uiIdColour ; }
	m4bool_t IsVisible() { return m_bSwVisible ; }
	m4bool_t GetAdjustLineToContainer() { return m_bAdjustLineToContainer ; }

	void SetTop( DvcUnit_t ai_Top ) { m_Top=ai_Top ; }
	void SetLeft( DvcUnit_t ai_Left ) { m_Left=ai_Left ; }
	void SetTop2( DvcUnit_t ai_Top2 ) { m_Top2=ai_Top2 ; }
	void SetLeft2( DvcUnit_t ai_Left2 ) { m_Left2=ai_Left2 ; }
	void SetWidth( DvcUnit_t ai_Width ) { m_Width=ai_Width ; }
	void SetStyle(ClENG_OutDevice::STYLE ai_Style) { m_Style=ai_Style ; } 
	void SetIdColour(m4uint32_t ai_uiIdColour) { m_uiIdColour=ai_uiIdColour ; }
	void SetVisible(m4bool_t ai_bSw) { m_bSwVisible=ai_bSw ; }
	void SetAdjustLineToContainer(m4bool_t ai_bAdjustValue) { m_bAdjustLineToContainer = ai_bAdjustValue ; }
	

	// Esta es una forma cómoda de resetear los datos de la linea, pasando una cadena. 
	// Si la cadena está vacía, resetea la linea a invisible.
	// Si no, el formato es el siguiente:
	//		WWWWSSVCCCCC
	//	-WWWW(4). En centesimas de point, anchura,
	//	-SS(2). Style, codigo nuemrico del enum.
	//	-V(1). 0 ó 1, visibilidad.
	//	-CCCCC(5). Id colour.
	// Si el formato es incorrecto (se testean validez de enums, etc), genera error local y 
	//	devuelve FALSE.

	m4bool_t SetLine( m4char_t *ai_pcLineProps ) ;
	static m4int16_t GetSizeStrLine() { return 12 ; }	//Tamaño de cadena aplicable a SetLine
	
	//-------------------------Shows--------------------------------------------------------

	//Si no es visible, esta llamada no hace nada.

	void Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) ;

	// XBase e YBase contienen los mismos valores que el Show atómico -sin offsets por zonas ya 
	//	impresas-, ai_HeightPrinted es la franja de linea YA impresa anteriormente y a ignorar 
	//	ahora, y  ai_HeightToPrint es el máximo a imprimir.
	// ai_HeightPrinted es además la parte de container ya impresa en otra hoja, por lo que NO
	//	se suma al eje Y al imprimir en otra hoja.

	void Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) ;

} ;

//=================================================================================ClENG_Region

class ClENG_Region
{
	/*
	Tener en cuenta a la hora de imprimir que un Field se imprime de forma atomica, pero un 
	container puede imprimirse en sucesivas veces.

	También hay dos formas de imprimir el frame de una region. Si el objeto testea que la 
	apariencia es igual en los 4 lados (mismo grosor, estilo y color y los 4 visibles), disparará 
	en el device un único evento Frame. Si los 4 lados nos son iguales, diaparará 4 eventos line.

	El grosor del frame se reparte a ambos lados (interior y exterior) del area de la region. 
	Se imprimirá la parte de relleno que reste del area de la region menos el area interior del 
	borde que se haya impreso.
	*/

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	ClENG_OutDevice * m_poDvc ; //Simple link

	// Coordenadas comienzo, como el resto de coordenadas, son coordenadas relativas. 
	// Coinciden con el area del Field o del Container, a la hora de resolver se les pasa una X e
	//  Y base.
	// Los miembros FillTop, etc, son los homólogos a m_Top, etc, solo que calculados restandole
	//	el espacio consumido por los bordes. Un cambio de anchura de lineas significa un reajuste 
	//	de estos valores y de las posiciones de las lineas del borde.
	// Si las lineas no dejan espacio para relleno (raro, pero posible), los parametros Fill se
	//	ponen a 0, y nunca se mandaran eventos a la capa Dvc para que pinte.

	DvcUnit_t m_Top, m_Left, m_Width, m_Height ;	//Area 
	DvcUnit_t m_FillTop, m_FillLeft, m_FillWidth, m_FillHeight ;

	// Lados. Se almacenan por separado al poder cambiar cada linea el aspecto individualmente.

MIT_PUBLIC:
	enum LINE_ID { TOP_LINE, BOTTOM_LINE, LEFT_LINE, RIGHT_LINE, FRAME } ;
	enum { LINE_COUNT=4 } ;
MIT_PRIVATE:

	ClENG_Line m_Line[ LINE_COUNT ] ;


	// Id colour relleno.
	// Hay un Id especial que indica color transparente, o lo que es lo mismo, que no hay color de
	// relleno: el efecto es que solo se dibuja el Frame.

	m4uint32_t m_uiFillColour ;
	
MIT_PUBLIC:

	ClENG_Region();
	virtual ~ClENG_Region() { ClENG_Region::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_Region) ; }


	//-------------------------Inicializaciones-----------------------------------------

	virtual void End() ;

	void Init( 
		ClENG_OutDevice * ai_poDvc,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		m4char_t *ai_pcBordeProps="",
		// Bug 0151166: en aix se asigna un cero cuando intentas asignar un número
		// negativo a una variable unsigned.
		m4uint32_t ai_uiFillColour=(m4int32_t)ClENG_Colour::ID_TRANSPARENT ) ;
		
	//Constructor copia

	void Init( ClENG_Region *ai_poSource ) ;

	//-------------------------Modificar frame--------------------------------------------

	//Varian lineas frame. Variar ancho altera coordenadas lineas individuales.

	void SetWidth(    LINE_ID ai_Line, DvcUnit_t ai_Width) ;
	void SetStyle(    LINE_ID ai_Line, ClENG_OutDevice::STYLE ai_Style) ;
	void SetIdColour( LINE_ID ai_Line, m4uint32_t ai_uiIdColour) ;
	void SetVisible(  LINE_ID ai_Line, m4bool_t ai_bSw) ;

	void SetFillColour(  m4uint32_t ai_uiFillColour) { m_uiFillColour=ai_uiFillColour ; }
	
	//Variar datos de la region en si. de la region altera coordenadas lineas individuales.

	void SetTop(DvcUnit_t ai_Top) ;
	void SetLeft(DvcUnit_t ai_Left) ;
	void SetWidth(DvcUnit_t ai_Width) ;
	void SetHeight(DvcUnit_t ai_Height) ;

	// Esta es una forma cómoda de restear todo el border, pasando una cadena. Si la cadena
	//	está vacía, resetea el border a invisible.
	// Si no, el formato es el siguiente, cuatro zonas de 12 caracteres en este orden.
	//		UP-DOWN-LEFT_RIGHT
	// Cada zona esta codificada:
	//		WWWWSSVCCCCC
	//	-WWWW(4). En centesimas de point, anchura,
	//	-SS(2). Style, codigo nuemrico del enum.
	//	-V(1). 0 ó 1, visibilidad.
	//	-CCCCC(5). Id colour.
	// Si el formato es incorrecto (se testean validez de enums, etc), genera error local y 
	//	devuelve FALSE.

	m4bool_t SetFrame( m4char_t *ai_pcBordeProps ) ;
	
	//-------------------------Gets-------------------------------------------------------

	// TRUE si puede dispararse un unico evento Frame, es decir, los 4 lados son visibles y 
	//	tienenen la misma apariencia.

	m4bool_t IsHomogeneousFrame() ;

	DvcUnit_t GetTop() { return m_Top; }
	DvcUnit_t GetLeft() { return m_Left; }
	DvcUnit_t GetWidth() { return m_Width; }
	DvcUnit_t GetHeight() { return m_Height; }
	m4uint32_t GetFillColour() { return m_uiFillColour; }

	//-------------------------Shows------------------------------------------------------

	// Llaman directamente a sus homologos en la capa Device, simplemente para permitir a 
	//	devices no muy precisos ajustar por aproximación las zonas de relleno y Frame.
	// Lo más normal es que el device no haga nada y redirija directamente la llamada a 
	//  los ShowOnDvc de esta clase.
	
	void Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) ;
	void Show( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) ;

	
	void ShowOnDvc( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) ;

	// Igual que la anterior, solo que puede limitarse la altura la zona imprimible. 
	// XBase e YBase contienen los mismos valores que el Show atómico -sin offsets por zonas ya 
	//	impresas-, ai_HeightPrinted es la franja de linea YA impresa anteriormente y a ignorar 
	//	ahora, y  ai_HeightToPrint es el máximo a imprimir.
	// ai_HeightPrinted es además la parte de container ya impresa en otra hoja, por lo que NO
	//	se suma al eje Y al imprimir en otra hoja.
	// Detecta llamadas que van a imprimir todo de golpe, y redirige la llamada a la otra 
	//	version de la funcion.

	void ShowOnDvc( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) ;

MIT_PRIVATE:
	
	void LineIndex(LINE_ID ai_Line, m4int16_t *ao_piBeg,m4int16_t *ao_piEnd) ; 

	// Ajusta todo segun el nuevo Width de las lineas de borde.

	void AdjustFrameAndFillZone() ;
} ;

#endif
