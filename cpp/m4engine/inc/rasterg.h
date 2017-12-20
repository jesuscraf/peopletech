//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             rasterg.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:18/09/97
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
#ifndef __RASTERG__H__
#define __RASTERG__H__

#include "clstr.h"

#include "field.h"

//=================================================================================ClENG_RasterMap

struct GIF_CONVERSION_DATA;
struct GIF_SCREEN;
struct BMP_RGBQUAD;

class ClENG_RasterMap 
{

MIT_PUBLIC:

	enum FILE_FORMATS { BMP, GIF, JPG, COUNT_FILE_FORMATS, UNKNOWN /*Va detras, no cuenta como COUNT_FILE_FORMATS*/  } ;

	enum BITS_PER_PIXEL { MONOCHROME, INDEXED_256 } ;

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	// Extensiones, indice debe coincidir con FILE_FORMATS

	static const m4char_t * m_pcExt[COUNT_FILE_FORMATS] ;	

	BITS_PER_PIXEL m_BitsPixel ;		

	//	El formato de almacenamiento NO depende del lugar de donde se lee el mapa: si ese
	//formato varia, traducirlo. Es necesario un formato de almacenamiento no dependiente del 
	//formato del fichero origen para no tener que reprogramar los algoritmos de escala y
	//dithering varias veces, simplemente por formatos distintos. Este formato es, en la 
	//mayoria de las veces, optimo y directamente utilizable por los devices. 
	//
	//  Segun BitsPixel, el formato debe ser:
	//	-1 bit/pixel (MONOCHROME). Secuencia de m4uint8_t, donde cada bit es un pixel.
	//		El MSB (bit de mas peso) del primer byte es el primer punto de la fila.
	//		El mapa consiste en un conjunto de lineas consecutivas, alineadas a long (siempre
	//		se usa m_uiBytesRow para saltar de linea a linea). 
	//		Fijarse que, por linea, puede haber tanto bytes que sobran por la alineacion, como
	//		bits de sobra en el ultimo byte valido.
	//	-1 byte/pixel(INDEXED_256). Secuencia de m4uint8_t, donde cada byte es un pixel. 
	//		este es un modo INDEXADO de color, por lo que se carga la paleta. Puede ser que
	//		solo se cargue parcialmente. 
	//		El primer byte es el primer punto de la fila.
	//		El mapa consiste en un conjunto de lineas consecutivas, alineadas a long (siempre
	//		se usa m_uiBytesRow para saltar de linea a linea). 
	//		Fijarse que, por linea, puede haber solom bytes que sobran, pero no bits.

	//	-NOTA:Como puede verse, una linea siempre esta alineada a long, lo que implica que 
	//		pueden sobrar bytes (y en el caso de monocromo, bits en el ultimo byte). 
	//		Los algoritmos de dithering y escala ignoran o no garantizan el contenido de los
	//		bytes de sobra, lo que quiere decir que a la hora de imprimir, estos no deberian 
	//		usarse. En el caso de monocromo, si en el ultimo byte hay bits validos, estos se
	//		ponen a 0, para que podamos imprimir el byte sin aplicar mascaras.

	m4uint8_t *m_puiByte ;				//Mapa de bytes, ocupa RasterY*BytesRow. NULL si
										//	hubo algun problema de carga.
	m4uint32_t m_uiRasterX, m_uiRasterY ;	//Medidos en pixels

	//Bytes por columna, depende de BitsPixel y RasterX
	//Puede tener algunos bytes de sobra, por alineaciones. Usarlo solo para saltar a
	//	los bytes de la siguiente linea.
		
	m4uint32_t m_uiBytesRow ;			

	//Parecido a BytesRow, solo que da el ultimo indice de linea raster correspondiente al 
	//ultimo byte UTILIZADO. Es decir, si por alineacion a long sobran 3 bytes, estos no cuentan
	//aqui. Apunta a:
	//	-MONOCHROME. Ultimo byte con bits validos (algunos bits pueden no serlo).
	//	-INDEXED_256. Ultimo byte valido.

	m4uint32_t m_uiLastXIndex ;

	// Solo se carga en modos indexados de color.

	ClENG_Palette m_oPalette ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_RasterMap() ;
	virtual ~ClENG_RasterMap() { ClENG_RasterMap::End(); } 
	virtual void End() ;	//Descarga el mapa, si lo hay.
	virtual size_t GetSizeof() { return sizeof(ClENG_RasterMap) ; }

	void Init();

	//-------------------------Carga y descarga-----------------------------------------

	static FILE_FORMATS GetFileFormat(const m4char_t *ai_pcFileName);

	//Lee info sin cargar garfico, solo leyendo cabeceras.
	static m4bool_t GetFileInfo(
		const m4char_t *ai_pcFileName, FILE_FORMATS *ao_FileFormat, 
		m4uint32_t* ai_uiPixelWidth, m4uint32_t* ai_uiPixelHeight, m4int16_t *ao_bpp );

	//Deja m_Map.Byte a NULL si tiene problemas. Devuelve true si tiene exito.
	//Si ya habia un mapa cargado, lo descarga previamente.
	//Segun la extension, llama al LoadFrom adecuado.
	m4bool_t LoadFromFile(const m4char_t *ai_pcFileName) ;	

	//Deja m_Map.Byte a NULL si tiene problemas. Devuelve true si tiene exito.
	//Si ya habia un mapa cargado, lo descarga previamente.
	m4bool_t LoadFromBMP(const m4char_t *ai_pcFileName);	
	m4bool_t LoadFromGIF(const m4char_t *ai_pcFileName);	

	m4bool_t SaveAsBMP(m4char_t *ai_pcFileName);
	m4bool_t SaveAsGIF(m4char_t *ai_pcFileName);


MIT_PUBLIC:

	//Descarga el mapa. Si no hay ninguno cargado, no hace nada.

	void UnloadMap() ;	

	m4bool_t IsMapLoaded() { return m_puiByte ? M4_TRUE : M4_FALSE ; }

	//-------------------------Manipulacion del mapa--------------------------------------

	//Convierte todo el mapa, sustituyendolo por otro con diferente numero de colores.
	//Da false si no puede hacer la conversion, ya sea porque no hay mapa cargado, o porque
	//	no es valida la conversion.
	//Durante la conversion, es preciso tener dos mapas completos en memoria, uno el que ya
	//	esta cargado, y otro el nuevo. A la salida solo existe el nuevo, que sustituye 
	//	al viejo. Por supuesto, los miembros m_BitsPixel, etc, tambien se actualizan.
	//Conversiones permitidas son:
	//	-De INDEXED_256 a MONOCHROME. Se descarga ademas la paleta, al no hacer falta.

	//m4bool_t Dither(BITS_PER_PIXEL ai_Bits) ;


	//Convierte todo el mapa, sustituyendolo por otro escalado con diferente tamaño. 
	//Da false si no puede escalar, ya sea porque no hay mapa cargado, o porque
	//	no esté implementada la escala para esete formato.
	//Si detecta que el tamaño coincide con el actual, no hace nada y devuelve true.
	//Durante la conversion, es preciso tener dos mapas completos en memoria, uno el que ya
	//	esta cargado, y otro el nuevo. A la salida solo existe el nuevo, que sustituye 
	//	al viejo. Por supuesto, los miembros m_BitsPixel, etc, tambien se actualizan.
	//Se puede escalar:
	//	-MONOCHROME 

	m4bool_t Scale( m4uint32_t ai_uiNewRasterX, m4uint32_t ai_uiNewRasterY  ) ;

	//Convierte todo el mapa a formato INDEXED_256. Si ya esta en ese formato, no hace nada
	// si no lo esta, lo transforma, modificando todas las propiedades del objeto que
	// hacen referencia al formato de la imagen
	m4bool_t RemapTo256Colors();

	

MIT_PRIVATE:

	void ScaleMonochrome( m4uint32_t ai_uiNewRasterX, m4uint32_t ai_uiNewRasterY ) ;

	//----- Funciones auxiliares especificas para la conversion a GIF
	void Compress(m4int_t ai_InitBits, GIF_CONVERSION_DATA* ai_stConvData, ClMIT_File *ai_poFile);
	void Output(m4int_t ai_iCode, GIF_CONVERSION_DATA* ai_stConvData, ClMIT_File* ai_poFile);
	void Hash(register m4int_t hsize, GIF_CONVERSION_DATA* ai_stConvData);
	m4int_t GIFNextPixel(GIF_CONVERSION_DATA* ai_stConvData);

	//----- Funciones auxiliares para JPEG

MIT_PUBLIC:
	//Lee todos los pixels válidos del gráfico, y los devuelve en el formato YCbCr
	//Los pixels leidos son los válidos. Es decir, se lee un bloque en cada array de 
	//	[m_uiRasterY][m_uiRasterX] pixels. Los arrays son de 2 dimensiones.
	//Cada array ha debido de ser creado con sitio suficiente para almacenar el gráfico, 
	//	aunque puede tener más espacio. Por eso se pasan los valores ai_Offset de comienzo
	//	dentro de cada array, y ai_SkipY con los bytes a saltar al cambiar de linea.
	//Funciona tanto para color como BW. Solo para BW, se usa ai_ForegroundColor para pixels
	//	con bit 1, y ai_BackgroundColor para 0.
	//Si no hay mapa cargado, retorna false y no hace nada.

	m4bool_t GrabYCbCrPixels(m4float_t *ao_Y, m4float_t *ao_Cb, m4float_t *ao_Cr,
		m4uint32_t ai_uiOffset, m4uint32_t ai_uiSkip,
		ClENG_Colour *ai_ForegroundColor, ClENG_Colour *ai_BackgroundColor ) ;

	//---------------------------------Gets--------------------------------------------------

MIT_PUBLIC:

	BITS_PER_PIXEL GetBitsPixel() { return m_BitsPixel ; }		
	m4uint32_t GetRasterX() { return m_uiRasterX ; } 
	m4uint32_t GetRasterY() { return m_uiRasterY ; } 
	m4uint32_t GetBytesRow() { return m_uiBytesRow ; }			
	m4uint32_t GetLastXIndex() { return m_uiLastXIndex ; }

	//Se debe usar SOLO para leerlo, no para manipularlo.
	m4uint8_t *GetMap() { return m_puiByte ; }

	//Se debe usar SOLO para leerlo, no para manipularlo.
	ClENG_Palette *GetPalette() { return &m_oPalette ; }

	static const m4char_t *  GetExtension(FILE_FORMATS ai_Format) 
		{ return m_pcExt[ai_Format] ;}

} ;

//=================================================================================ClENG_RasterG
class ClENG_RasterG : public ClENG_Field  
{

MIT_PRIVATE:

	//Switch para path en Registry
	m4bool_t m_bSwGraphics;

	//Switch para imagenes en temporal del web server
	m4bool_t m_bTempWebImage;

	ClENG_RasterMap m_oMap;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_RasterG() ;
	virtual ~ClENG_RasterG() { ClENG_RasterG::End(); } 
	virtual void End() ;
	virtual size_t GetSizeof() { return sizeof(ClENG_RasterG) ; }

	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t * ai_pcExpr,
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel, 
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize, 
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		m4bool_t ai_bSwGraphics,
		m4bool_t ai_bTempWebImage
	);

MIT_PUBLIC:


	//-------------------------Gets-----------------------------------------------------
	m4bool_t IsVarHeight() {return M4_FALSE; }
	DvcUnit_t GetVarHeight() {return 0; }
	m4bool_t IsRemaining() {return M4_FALSE; }

	//-------------------------Funciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:


	// -Se descarga el mapa en RAM, si lo había.
	// -Se chequea que el valor sea una cadena.
	// -m_oValue es el path del grafico a imprimir.
	// -Si el path es una cadena vacia, NO se hace nada; se interpreta que no hay grafico.	
	// -Solo si el Dvc->IsRasterPreloaded(), se lee el fichero , aplicando Dithering
	//	si ! Dvc->IsHardDithering(). Tras esto, si ! Dvc->IsHardScale(), se escala.

	virtual DvcUnit_t PostLoadValue() ;

	//-Se chequea que el valor sea una cadena.
	//-Si ! Dvc->IsRasterPreloaded(), se cede la responsabilidad al device, que se encarga de 
	//	todo. No hay mapa en memoria.
	//-Si Dvc->IsRasterPreloaded(), pasa al Dvc el usa el mapa en RAM, en el que ya se 
	//	aplico el dithering y escala si procedia. 
	//-Si NO es estatic, tras imprimir el grafico, se borra el mapa (si es que hay). Esto
	//	optimiza la memoria. Si es static, aqui no se borra nunca, lo hara End(), u otro
	//	PostLoadValue.

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;
};


#endif
