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

#ifndef __GIF__H__
#define __GIF__H__


#include "colour.h"
#include "rasterg.h"


//======================================================================================
//======================================================================================
//=====================                                  ===============================
//=====================   Clases auxiliares de proceso   ==============================
//=====================                                  ===============================
//======================================================================================
//======================================================================================
//======================================================================================

/*
Para simplificar el proceso de lectura, se montan capas logicas de proceso de la misma 
forma que si montasemos torres de protocolos.


PROCESO DE DECODIFICACION
-------------------------

Capa 3: LZW decoder

	ClENG_GIF_LZW_Decoder Haciendo uso del ClENG_GIF_CodeStreamer (ve todo como un 
stream de codes), suministra hacia fuera el metodo PopValue, de tal forma que las capas
clientes ven los datos raster data como una sucesión de valores ya desencriptados.

Capa 2: code stream

  ClENG_GIF_CodeStreamer Haciendo uso del ClENG_GIF_ByteStreamer (ve todo como un stream 
			de bytes), suministra hacia fuera el metodo PopCode, de tal forma que las capas
			cliente ven los datos raster data como un stream de codes.


Capa 1: data stream

  ClENG_GIF_ByteStreamer Usando ClENG_GIF_Block, suministra hacia fuera el metodo PopByte,
			de tal forma que las capas clientes ven todo los datos de los bloques del
			raster data como un unico stream lineal de bytes.
   

Capa 0: lectura de fichero

  ClENG_GIF_Block y ClENG_GIF_Block_XXXX. Leen bloques GIF del fichero


*/


typedef m4uint8_t ENG_GIF_BYTE ;
typedef m4uint16_t ENG_GIF_CODE ;

//======================================================================>ClENG_GIF_Block

//Contiene un bloque GIF (max 256 chars)

class ClENG_GIF_Block 
{
MIT_PUBLIC:

	enum {  MAX_SIZE=256 } ;
	ENG_GIF_BYTE m_Data[MAX_SIZE] ;

MIT_PRIVATE:

	ENG_GIF_BYTE m_Size ;
	m4bool_t m_Initialized ;

MIT_PUBLIC:

	ClENG_GIF_Block();

	m4bool_t IsTerminator() { return m_Size==0 ; }
	size_t GetSize() { return m_Size ; }

	//Lee el bloque desde la posicion del fichero. Si ai_bSwReadBlockSize, el tamaño se
	//	lee del fichero (debe ser el primer byte a leer). Si es FALSE, dicho tamaño no
	//	se lee de fichero, sino que se toma de ai_TestSize
	//Si ai_TestSize>=0, da error si el bloque no es exactamente del tamaño indicado.
	//Da false si hay errores, bien de lectura, o de tamaños de bloque no legales.
	m4bool_t Init(ClMIT_File* ai_poFile, m4int16_t ai_TestSize=-1, m4bool_t ai_bSwReadBlockSize=M4_TRUE) ;

	m4bool_t IsInitialized() { return m_Initialized ; }
} ;

//=============================================================>ClENG_GIF_SignatureBlock

class ClENG_GIF_SignatureBlock : private ClENG_GIF_Block
{
MIT_PUBLIC:
	m4char_t m_Version[4] ;

	m4bool_t Init(ClMIT_File* ai_poFile) ;
} ;

//============================================================>ClENG_GIF_ScreenDescriptorBlock

class ClENG_GIF_ScreenDescriptorBlock : private ClENG_GIF_Block
{
MIT_PUBLIC:
	m4uint16_t m_RasterWidth, m_RasterHeight ;
	m4bool_t m_SwGlobalColorMap ;
	m4uint16_t m_ColorResolution ;  //BitsPixel paleta original (valores 1, 4 u 8)
	m4uint16_t m_SizeOfGlobalColorTable ;  //Valor absoluto (2, 16 o 256)
	m4uint8_t m_BackgroundColorIndex ;
	m4uint8_t m_PixelAspectRatio ;

	m4bool_t Init(ClMIT_File* ai_poFile) ;
} ;

//=======================================================>ClENG_GIF_ImageDescriptorBlock

class ClENG_GIF_ImageDescriptorBlock : private ClENG_GIF_Block
{
MIT_PUBLIC:
	m4uint16_t m_PixelWidth, m_PixelHeight ;
	m4bool_t m_SwInterlaced, m_SwLocalColorTable ;
	m4uint16_t m_LocalColorTableSize ; //Valor absoluto (2, 16 o 256)

	m4bool_t Init(ClMIT_File* ai_poFile) ;
} ;

//============================================================>ClENG_GIF_BlockTools

class ClENG_GIF_BlockTools
{
MIT_PUBLIC:

	static m4bool_t LoadPalette( ClMIT_File* ai_poFile, ClENG_Palette *ao_poPalette, 
		m4int16_t ai_PaletteEntries) ;

	//Consume n subbloques hasta que encuentra un Terminator (bloque de size=0)
	static m4bool_t SkipSubBlocks(ClMIT_File* ai_poFile) ;
} ;


//================================================================>ClENG_GIF_ByteStreamer

//Hace que todo se vea como un flujo de bytes, en vez de un flujo de bloques

class ClENG_GIF_ByteStreamer
{
MIT_PRIVATE:

	ClMIT_File* m_poFile ;	//Link
	ClENG_GIF_Block m_CurrentBlock ;
	size_t  m_Readed ;

MIT_PUBLIC:
	
	ClENG_GIF_ByteStreamer();

	//El fichero ya debe estar posicionado en el primer bloque de datos, lee este.
	m4bool_t Init(ClMIT_File* ai_poFile) ;

	//Actualiza ao_SwOk SOLO en caso de error.
	ENG_GIF_BYTE PopByte(m4bool_t *ao_SwOk) ;
} ;

//================================================================>ClENG_GIF_CodeStreamer

class ClENG_GIF_CodeStreamer
{
MIT_PRIVATE:
	
	enum { MIN_BIT_SIZE=1, MAX_BIT_SIZE=16 } ;
	
	ClENG_GIF_ByteStreamer m_ByteStreamer ;
	m4uint8_t m_PackBitSize ;

	ENG_GIF_BYTE m_CurrentByte ;
	m4uint8_t m_BitsUnreaded ;


MIT_PUBLIC:

	ClENG_GIF_CodeStreamer() ;

	m4bool_t Init(ClMIT_File* ai_poFile, m4uint8_t ai_PackBitSize) ;

	m4bool_t ResetPackBitSize(m4uint8_t ai_PackBitSize) ;

	//Actualiza ao_SwOk SOLO en caso de error.
	ENG_GIF_CODE PopCode(m4bool_t *ao_SwOk) ;

	m4uint8_t GetPackBitSize() { return m_PackBitSize; }
} ;


//================================================================>ClENG_GIF_LZW_Decoder

class ClENG_GIF_LZW_Decoder
{
MIT_PRIVATE:
	ClMIT_File* m_poFile ;	//Link

	enum { MIN_BIT_SIZE=3, MAX_BIT_SIZE=12, MAX_CODE_SIZE_TABLE=(1<<MAX_BIT_SIZE) } ;

	//Bit size leido en el fichero al comienzo del raster data
	m4uint8_t m_BitSize ;
	ClENG_GIF_CodeStreamer m_CodeStreamer ;

	//Stack for storing pixels (OUTPUT)
	ENG_GIF_BYTE m_stack[MAX_CODE_SIZE_TABLE] ;
	//Suffix table
	ENG_GIF_BYTE m_suffix[MAX_CODE_SIZE_TABLE] ;
	//Prefix linked list
	ENG_GIF_CODE m_prefix[MAX_CODE_SIZE_TABLE] ;

	//stack pointer to m_stack
	ENG_GIF_BYTE *m_sp ;

	m4int16_t m_currSize;                   /* The current code size */
	ENG_GIF_CODE m_clear;                   /* Value for a clear code */
	ENG_GIF_CODE m_ending;                  /* Value for a ending code */
	m4int16_t  m_newCodes;                  /* First available code */
	m4int16_t  m_topSlot;                   /* Highest code for current size */
	m4int16_t  m_slot;                      /* Last read code */
	ENG_GIF_CODE m_fc, m_oc ; /* FirstCode, OldCode */

	m4uint32_t m_BadCodeCount ;

	m4int16_t m_nAvailBytes, m_nBitsLeft ;
	ENG_GIF_BYTE m_b1 ; //Current byte
	ENG_GIF_BYTE m_byteBuff[257] ; //Current block
	ENG_GIF_BYTE *m_pBytes ; //Nxt byte in block

MIT_PUBLIC:
	
	ClENG_GIF_LZW_Decoder() ;
	virtual ~ClENG_GIF_LZW_Decoder() { ClENG_GIF_LZW_Decoder::End() ; }
	virtual void End();
	virtual size_t GetSizeof() { return sizeof(ClENG_GIF_LZW_Decoder) ; }

	//El fichero debe estar al comienzo del raster data (es decir, en el byte que 
	//	contiene el code size)
	
	m4bool_t Init(ClMIT_File* ai_poFile) ;

	//Actualiza ao_SwOk SOLO en caso de error.
	//Va decodifando el flujo LWZ y dando valores. Si ya no hay mas datos o hay errores, 
	//	devuelve siempre el valor 0.

	ENG_GIF_BYTE PopValue(m4bool_t *ao_SwOk) ;
} ;


//======================================================================================
//======================================================================================
//=====================                                  ===============================
//=====================   Clases de interface            ==============================
//=====================                                  ===============================
//======================================================================================
//======================================================================================
//======================================================================================

//====================================================================>ClENG_GIF_Loader
//
//Clase auxiliar de ClENG_RasterMap para cargar un GIF
//

class ClENG_GIF_Loader
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	
	ClMIT_File m_oFile ;
	ClENG_GIF_SignatureBlock m_Signature ;
	ClENG_GIF_ScreenDescriptorBlock m_ScreenDescriptor ;
	ClENG_GIF_ImageDescriptorBlock m_ImageDescriptor ;


MIT_PUBLIC:

	ClENG_GIF_Loader() ;
	virtual ~ClENG_GIF_Loader() { ClENG_GIF_Loader::End(); } 
	virtual void End() ;	
	virtual size_t GetSizeof() { return sizeof(ClENG_GIF_Loader) ; }

	//Si ai_bReadRaster es false, no lee la información del gráfico. Solo se obtiene de 
	//	informacion de cabecera estos datos:
	//	-ao_RasterX
	//	-ao_RasterY
	//	-ao_poPalette (de ahi pueden obtenerse los bpp)
	//Sirve para obtener el resto de los datos, sin la penalización de leer el grafico en si.

	m4bool_t Init(
		const m4char_t *ai_pcFileName, 
		m4uint32_t &ao_RasterX, m4uint32_t &ao_RasterY,
		ClENG_RasterMap::BITS_PER_PIXEL &ao_RasterBPP,
		m4uint32_t &ao_BytesRow, m4uint32_t &ao_LastXIndex,
		ClENG_Palette *ao_poPalette,
		m4uint8_t * &ao_puiByte, m4bool_t ai_bReadRaster
	) ;

MIT_PRIVATE:

	m4bool_t ReadRasterData(

		m4uint32_t &ao_RasterX, m4uint32_t &ao_RasterY,
		ClENG_RasterMap::BITS_PER_PIXEL &ao_RasterBPP,
		m4uint32_t &ao_BytesRow, m4uint32_t &ao_LastXIndex,
		ClENG_Palette *ao_poPalette,
		m4uint8_t * &ao_puiByte ) ;
} ;


#endif

