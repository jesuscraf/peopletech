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

#ifndef __JPEG__H__
#define __JPEG__H__


class ClENG_RasterMap ;
class M4ClBuffer ;

//=================================================================================ClENG_DCT

class ClENG_DCT 
{
MIT_PUBLIC:

	enum PUBLIC_CONSTS { N=8 } ;	//Tamaño bloque DCT 8*8

	enum INDEX_DATA { LUMIN=0, CHRO=1, INDEX_COUNT } ;	//Tamaño bloque DCT 8*8

	static const m4double_t m_AAN_ScaleFactor[N] ;

	m4int16_t m_Quality ;

MIT_PRIVATE:

	//m_Qu es el Quantum, m_Dv el divisor.
	//El indice 0 corresponde a la luminancia y el 1 a la crominancia.
	
	m4int32_t	m_Qu[INDEX_COUNT][N*N] ;
	m4double_t	m_Dv[INDEX_COUNT][N*N] ;


MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_DCT() ;
	virtual ~ClENG_DCT() { ClENG_DCT::End(); } 
	virtual void End() ;
	virtual size_t GetSizeof() { return sizeof(ClENG_DCT) ; }

	//Quality: 0 peor, 100 mejor. 80 es una buena relación calidad/compresión

	void Init(m4int32_t ai_Quality);

	//----------------------------------Gets---------------------------------------------

	m4int32_t GetQu(INDEX_DATA ai_Data, m4int16_t ai_Index ) 
		{ return m_Qu[ai_Data][ai_Index] ; } 
	m4double_t GetDv(INDEX_DATA ai_Data, m4int16_t ai_Index ) 
		{ return m_Dv[ai_Data][ai_Index] ; } 

	m4int16_t GetQuality() { return m_Quality ; }

	//----------------------------------Calculos-----------------------------------------

	// Calculo DCT por el metodo AAN

    void ForwardDCT(m4float_t ai_In[N][N], m4double_t ao_Out[N][N] ) ;

    // Cuantifica los datos y los redondea al entero mas próximo

    void QuantizeBlock(m4double_t ai_In[N][N], m4int32_t ai_Code, m4int32_t ao_Out[N*N] ) ;

} ;


//===========================================================================ClENG_BlkEncoder

class ClENG_BlkEncoder
{

MIT_PUBLIC:

	enum CONSTS { DC_LUMIN=0, AC_LUMIN, DC_CHRO, AC_CHRO, AC_DC_ARR_SIZE } ;

	static const m4uint8_t m_Bits[	AC_DC_ARR_SIZE][17] ;
	static const m4uint8_t m_Val[	AC_DC_ARR_SIZE][162] ;	//Sobran valores para DC, da igual.

MIT_PRIVATE:

	m4int32_t m_DC_Matrix[2][12][2] ;
	m4int32_t m_AC_Matrix[2][255][2] ;

	m4int32_t m_BufferPutBuffer, m_BufferPutBits ; 

	M4ClBuffer *m_poOut ;	//Auxiliar, link

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_BlkEncoder() ;
	virtual ~ClENG_BlkEncoder() { ClENG_BlkEncoder::End(); } 
	virtual void End() ;	
	virtual size_t GetSizeof() { return sizeof(ClENG_BlkEncoder) ; }

	void Init();

	//----------------------------------Operaciones-----------------------------------------

	//Para comenzar a codificar. Depues, secuencias de n Encode, y acabar con un FlushBuffer

	void BeginEncode(M4ClBuffer *ao_OutStream) ;


	//Codificación Run-Length y Huffmann
	
	void Encode(m4int32_t ai_ZigZag[ClENG_DCT::N*ClENG_DCT::N], 
		m4int32_t ai_Prec, m4int32_t ai_DC_Code, m4int32_t ai_AC_Code) ;


	void FlushBuffer() ;

MIT_PRIVATE:

	void BufferIt(m4int32_t ai_Code, m4int32_t ai_Size) ;

} ;

//===========================================================================ClENG_JpgEncoder

class ClENG_JpgEncoder
{
MIT_PUBLIC:


	//-----------------------------Info general de parseo-----------------------------------

	enum CONSTS{ NUMBER_OF_COMPONENTS=3, PRECISION=ClENG_DCT::N } ;

	static const m4int32_t m_CompID[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_QtableNumber[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_DCtableNumber[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_ACtableNumber[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_HsampFactor[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_VsampFactor[NUMBER_OF_COMPONENTS] ;
	static const m4int32_t m_MaxHsampFactor, m_MaxVsampFactor ;

	static const m4int32_t m_JpegNaturalOrder[ClENG_DCT::N*ClENG_DCT::N] ;

MIT_PRIVATE:
	
	ClENG_DCT			m_DCT ;
	ClENG_BlkEncoder	m_BlkEncoder ;

	ClENG_Colour m_Foreground, m_Background ;	//Defectos para conversion BW


	//----------------Info auxiliar del raster en proceso-----------------------------------

	m4int32_t m_ImageHeight, m_ImageWidth ;
	m4int32_t m_BlockWidth[NUMBER_OF_COMPONENTS] ;
	m4int32_t m_BlockHeight[NUMBER_OF_COMPONENTS] ;
	m4int32_t m_compWidth[NUMBER_OF_COMPONENTS] ;
	m4int32_t m_compHeight[NUMBER_OF_COMPONENTS] ;
	m4bool_t  m_lastColumnIsDummy[NUMBER_OF_COMPONENTS] ; 
	m4bool_t  m_lastRowIsDummy[NUMBER_OF_COMPONENTS] ; 

	//Array dinámico por cada componente, compuesto por [m_compHeight[c]][m_compWidth[c]]

	m4float_t *m_Component[NUMBER_OF_COMPONENTS] ;
	
	M4ClBuffer *m_poOut ;	//Auxiliar, link

MIT_PUBLIC:

	//-------------------------Inicializaciones---------------------------------------------

	ClENG_JpgEncoder() ;
	virtual ~ClENG_JpgEncoder() { ClENG_JpgEncoder::End(); } 
	virtual void End() ;	
	virtual size_t GetSizeof() { return sizeof(ClENG_JpgEncoder) ; }

	void Init(m4int32_t ai_Quality);

	//----------------------------------Operaciones-----------------------------------------

	//Detecta si la calidad actual es la misma que la pasada. Si es asi, no hace nada.

	void ChangeQuality(m4int32_t ai_Quality);


	//El grafico debe estar cargado o retorna false.
	//Codifica con la ultima calidad que se haya puesto	en este objeto.
	//Almacena el Jpeg codificado en un buffer. NO resetea este, lo usa directamente, por lo
	//	que ya debe estar inicializado.
	//Da false si tiene algun problema.

	m4bool_t Encode(ClENG_RasterMap *ai_Image, M4ClBuffer *ai_poBuff ) ;

	//Exactamente igual a la anterior, solo que almacena en fichero.

	m4bool_t Encode(ClENG_RasterMap *ai_Image, m4char_t *ai_pcFile ) ;

	//Exactamente igual a la anterior, solo que almacena en fichero, y lee la imagen de fichero.

	m4bool_t Encode(m4char_t *ai_pcSourceImageFile, m4char_t *ai_pcFile ) ;


	//Exactamente igual a la anterior, solo que almacena en un M4ClBuff. No restea este, 
	//	simplemente lo utiliza.

	m4bool_t Encode(m4char_t *ai_pcSourceImageFile, M4ClBuffer *ai_poBuff ) ;

	//Sobre un file JPG ya abierto, localiza y da los valores de altura y anchura 
	//Deja la posicion del fichero donde estaba.
	//bpp=bits per pixel, o bien 8 (256 colores) o bien 24 (millones de colores)

	static m4bool_t LocateJPG_HeaderData(
		ClMIT_File* ai_poFile, m4uint32_t* ai_uiWidth, m4uint32_t* ai_uiHeight,
		m4int16_t *ao_bpp ) ;


MIT_PRIVATE:

	//Borrar m_Componen (si no está a NULL), y pone a NULL

	void DeleteComponent() ;

	void WMarker(m4uint8_t * ai_Data) ;
	void WArray(m4uint8_t * ai_Data) ;
	
	void WHeaders() ;
	void WData() ;
	void WEOI() ;

	m4float_t GetValue(m4int16_t ai_Comp, m4int16_t ai_Y, m4int16_t ai_X )
		{  return m_Component[ai_Comp][ai_Y*m_compWidth[ai_Comp]+ai_X] ; }

	void SetValue(m4int16_t ai_Comp, m4int16_t ai_Y, m4int16_t ai_X, m4float_t ai_Val )
		{  m_Component[ai_Comp][ai_Y*m_compWidth[ai_Comp]+ai_X]=ai_Val ; }
} ;

#endif

