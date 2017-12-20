//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             rasterg.cpp
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

#include "eng.h"
#include "engdf.inl"

#include "clfile.h"

#include "gif.h"

#define GROUP_ID    ENG_TGI_RASTERG

#define LM_to_uint(LSB, MSB)		(((MSB)<<8)|(LSB))


//======================================================================>ClENG_GIF_Loader

ClENG_GIF_Loader::ClENG_GIF_Loader()
{
	m_poInsp=GET_INSP();
}

void ClENG_GIF_Loader::End()
{
	m_oFile.Close() ;
}

m4bool_t ClENG_GIF_Loader::Init(
		const m4char_t *ai_pcFileName, 
		m4uint32_t &ao_RasterX, m4uint32_t &ao_RasterY,
		ClENG_RasterMap::BITS_PER_PIXEL &ao_RasterBPP,
		m4uint32_t &ao_BytesRow, m4uint32_t &ao_LastXIndex,
		ClENG_Palette *ao_poPalette,
		m4uint8_t * &ao_puiByte, m4bool_t ai_bReadRaster
		)
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_GIF_BYTE c;
	ClENG_Palette globalPalette ;


	ClENG_GIF_Loader::End() ;

	//--------------->Open file

	if(! m_oFile.Open(ai_pcFileName, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE)){
		SwOk=M4_FALSE;
		goto exit;
	};

	//--------------->Read image header

	//GIF mark test

	if ( ! m_Signature.Init(&m_oFile) ) {
		SwOk=M4_FALSE;
		goto exit;
	} ;

	//--------------->Read screen descriptor

	if ( ! m_ScreenDescriptor.Init(&m_oFile) ) {
		SwOk=M4_FALSE;
		goto exit;
	} ;
	ao_RasterX= m_ScreenDescriptor.m_RasterWidth ;
	ao_RasterY= m_ScreenDescriptor.m_RasterHeight ;
		
	//--------------->Read Global Color Table 
	
	if ( m_ScreenDescriptor.m_SwGlobalColorMap ) {
		
		if ( ! ClENG_GIF_BlockTools::LoadPalette( &m_oFile, &globalPalette, 
			m_ScreenDescriptor.m_SizeOfGlobalColorTable) )
		{
			SwOk=M4_FALSE;
			goto exit ;
		};
	} ;
	
	//--------------->Main loop; read blocks 

	while (M4_TRUE) {
		
		m_oFile.R(&c,1);
	
		if (c == ';') {	 
			
			//************GIF terminator
			
			//Error porque no se ha procesado ninguna imagen. 
			// Si fuese asi se habria salido ya del bucle.
			
			SwOk=M4_FALSE;
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163005, 
				"Invalid GIF File Format", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
			goto exit;

		} else if (c == '!') {	 

			//************Extensions
	
			m_oFile.R(&c,1);

			switch (c) {
		
			case 0xf9:	//************Graphic Control Extension	
			case 0x01:	//************Plain Text Extension	
			case 0xff:	//************Application Extension	
			case 0xfe:	//************Comment Extension 
				
				if ( ! ClENG_GIF_BlockTools::SkipSubBlocks(&m_oFile) ) {
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				break;

			default:
				
				//************Not processed extension
				SwOk=M4_FALSE ;
				goto exit ;
			};

		} else if (c == 0x2c)  {

			//************Image Descriptor

			//------>Read image descriptor

			if ( ! m_ImageDescriptor.Init(&m_oFile) ) {
				SwOk=M4_FALSE;
				goto exit;
			} ;

			//------>Proccess Local Color Palete

			if (m_ImageDescriptor.m_SwLocalColorTable) {
				
				//Threre are Local Color Table; read it !
				if ( ! ClENG_GIF_BlockTools::LoadPalette( 
					&m_oFile, ao_poPalette,
					m_ImageDescriptor.m_LocalColorTableSize )  )
				{
					SwOk=M4_FALSE;
					goto exit ;
				};

			} else {
				//Threre are not Local Color Table, global table will be used
				if ( ! m_ScreenDescriptor.m_SwGlobalColorMap ) {
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				ao_poPalette->Init(&globalPalette) ;
			};

			//------>Proccess Table based Image Data
		
			if (ai_bReadRaster) {
				SwOk = ReadRasterData( ao_RasterX, ao_RasterY, ao_RasterBPP, 
					ao_BytesRow, ao_LastXIndex, ao_poPalette, ao_puiByte ) ;
			} ;

			break ;
		} ;
	};

exit:

	if( !SwOk ) {

		if (ai_bReadRaster) {
			M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MEM_GRP_RASTERG, 
				ao_puiByte, m4uint8_t , ao_RasterY*ao_BytesRow ) ;
		} ;

		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163005, 
			"Invalid GIF File Format", ClMIT_Msg::MIXED_OUT)
			<< SEND_MSG ;
	
	} ;

	m_oFile.Close();
	return SwOk;

}


m4bool_t ClENG_GIF_Loader::ReadRasterData(

	m4uint32_t &ao_RasterX, m4uint32_t &ao_RasterY,
	ClENG_RasterMap::BITS_PER_PIXEL &ao_RasterBPP,
	m4uint32_t &ao_BytesRow, m4uint32_t &ao_LastXIndex,
	ClENG_Palette *ao_poPalette,
	m4uint8_t * &ao_puiByte ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_GIF_LZW_Decoder decoder ;

	//***** Para traza


	//*****Por si esta cargado, lo descarga

	M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MEM_GRP_RASTERG, 
		ao_puiByte, m4uint8_t , ao_RasterY*ao_BytesRow ) ;


	//*****Los prefijos R_ son para variables de LECTURA (datos LWZ a leer del fichero), y los
	//	W_ de escritura (mapa RasterG a escribir)

	//*****Inicializacion y test de valores R
	//*****Fill raster map values matching with header values

	ENG_GIF_BYTE R_c,R_v;				//Aux de codigo y valor leido
	ENG_GIF_BYTE *R_scanline=NULL;	//Aux, buffer con de tamaño=mayor linea posible de lectura
	m4bool_t R_swComplement=M4_FALSE ; 
	size_t R_wib ;	//Ancho en bytes que ocupa cada linea de lectura
	size_t R_lineCount ; //Numero de lineas a leer
	size_t R_ypos ; //Linea absoluta que se esta leyendo (en entrelazado hay que convertir)
	
	R_lineCount = ao_RasterY ;

	if(ao_poPalette->GetSize()==2) {
		
		ao_RasterBPP = ClENG_RasterMap::MONOCHROME ;
		ao_BytesRow  = (ao_RasterX/8)+ (ao_RasterX % 8 ? 1 : 0 ) ;		
		ao_LastXIndex = ao_BytesRow-1 ;

		R_wib = ao_BytesRow ; 

		//Ajusta R_swComplement (defecto FALSE)
		if ( ao_poPalette->GetColourByIndex(0)->IsBlack() ) {
			R_swComplement = M4_TRUE ;
		} ;

	} else if (ao_poPalette->GetSize()==256) {

		ao_RasterBPP = ClENG_RasterMap::INDEXED_256 ;
		ao_BytesRow  = ao_RasterX ;		
		ao_LastXIndex = ao_BytesRow-1 ;

		R_wib = ao_BytesRow ; //Paso directo, un pixel es un byte

	} else {
		
		//NO PERMITIDO !
		
		SwOk=M4_FALSE ;
		goto exit ;
	};

	M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		ao_puiByte, m4uint8_t ,ao_RasterY*ao_BytesRow ) ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
				 R_scanline, ENG_GIF_BYTE, R_wib);

	//*****Inicializar decompresor LWZ

	if ( ! decoder.Init(&m_oFile) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//*****Bucle principal

	m4int_t	W_ypos;	//Linea a escribir, puede NO coincidir con la de R en modo entrelazado
	m4int_t	R_xpos; //Pos x a leida, coincide con la que hay que escribir
	size_t W_LastIndex ; //Auxiliar, ultimo indice absoluto valido para escribir
	size_t W_FromIndex ; //Auxiliar, inicio para copiar la linea
	size_t W_BytesToCopy ;	//Auxiliar, bytes a copiar
	size_t W_wib ;	//Anchura en bytes del buffer de escritura (puede no coincidir con W_wib)
	size_t R_interlacedSize[4] ; //Size de cada block en modo entrelazado
	size_t R_interlacedBlock, R_interlacedIndex ; //Auxiliares de recorrido 

	//Auxiliares para inverso
	ENG_GIF_BYTE *puiMapAux, *puiScanLineAux ;
	size_t counterAux ;

	W_LastIndex = ao_RasterY*ao_BytesRow-1 ;
	W_wib = ao_BytesRow ;
	if (W_wib<R_wib) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//---->Calculo limites para interlaced

	//Cada 8 lineas, desde la linea 0
	R_interlacedSize[0]=((R_lineCount-1)/8) + 1  ;
	//Cada 8 lineas, desde la linea 4
	if (R_lineCount>=5) {
		R_interlacedSize[1]=((R_lineCount-5)/8) + 1 ; 
	} else {
		R_interlacedSize[1]=0 ;
	} ;
	//Cada 4 lineas, desde la linea 2
	if (R_lineCount>=3) {
		R_interlacedSize[2]=((R_lineCount-3)/4) + 1 ; 
	} else {
		R_interlacedSize[2]=0 ;
	} ;
	//Cada 2 lineas, desde la linea 1
	if (R_lineCount>=2) {
		R_interlacedSize[3]=((R_lineCount-2)/2) + 1 ; 
	} else {
		R_interlacedSize[3]=0 ;
	} ;

	for (
		R_ypos=0, R_interlacedBlock=0, R_interlacedIndex=0 ; 
		R_ypos < R_lineCount; 
		++R_ypos ) 
	{
	
		//---->Lee una scanline completa
		
		switch(ao_RasterBPP) {
		case ClENG_RasterMap::MONOCHROME:
			for (R_xpos=0; R_xpos < ao_RasterX; ++R_xpos ) {
				R_v = decoder.PopValue(&SwOk) ;
				if (!SwOk) goto exit ;
				if (R_v<0) {
					break ;
				} else if(R_v) {
					R_scanline[R_xpos>>3] |= 128 >> (R_xpos&7);
				} else {
					R_scanline[R_xpos>>3] &= 0xff7f >> (R_xpos&7);
				};
			} ;
			break;
		case ClENG_RasterMap::INDEXED_256:

			for (R_xpos=0; R_xpos < W_wib; ++R_xpos ) {
				R_v = decoder.PopValue(&SwOk) ;
				if (!SwOk) goto exit ;
				R_scanline[R_xpos]= R_v ;
			} ;

			break;
		
		default:
			SwOk=M4_FALSE ;
			R_v=-1 ;
		}
		if (R_v<0) break ;

		//---->calcular W_ypos en funcion de R_ypos y modo entrelazado

		if (m_ImageDescriptor.m_SwInterlaced) {

			switch(R_interlacedBlock) {
			case 0:
				W_ypos = R_interlacedIndex*8 ;
				break;
			case 1:
				W_ypos = R_interlacedIndex*8+4 ;
				break;
			case 2:
				W_ypos = R_interlacedIndex*4+2 ;
				break;
			case 3:
				W_ypos = R_interlacedIndex*2+1 ;
				break;
			} ;

			++R_interlacedIndex ;
			//Testea y efectua cambio de bloque entrelazado
			if (R_interlacedIndex>=R_interlacedSize[R_interlacedBlock]) {
				++R_interlacedBlock ;
				R_interlacedIndex =0 ;
			} ;


		} else {

			W_ypos = R_ypos ;

		} ;

		//---->Copia la scanline leida a la linea W que corresponda

		W_FromIndex = W_ypos*W_wib ;
		W_BytesToCopy = W_wib ;
		if ( W_FromIndex > W_LastIndex ) {
			continue ;
		} ;
		if ( W_FromIndex+W_BytesToCopy-1 > W_LastIndex ) {
			 W_BytesToCopy = W_LastIndex-W_FromIndex+1 ;
		} ;
		if(!R_swComplement) {
			memcpy(&ao_puiByte[W_FromIndex], R_scanline, W_BytesToCopy );
		} else {
			for ( 
				puiMapAux=&ao_puiByte[W_FromIndex], puiScanLineAux =R_scanline, 
				counterAux= W_BytesToCopy ;  
				counterAux ;
				--counterAux, ++puiMapAux, ++puiScanLineAux )
			{
				*puiMapAux = ~(*puiScanLineAux) ;
			} ;
		} ;
	}

exit:

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		 R_scanline, ENG_GIF_BYTE, R_wib);

	return SwOk;
}


//======================================================================>ClENG_GIF_Block

ClENG_GIF_Block::ClENG_GIF_Block()
{
	m_Size=0 ;
	m_Initialized=M4_FALSE ;
}

m4bool_t ClENG_GIF_Block::Init(ClMIT_File* ai_poFile, m4int16_t ai_TestSize, m4bool_t ai_bSwReadBlockSize) 
{
	m4bool_t SwOk=M4_TRUE ;

	if (ai_bSwReadBlockSize) {
		ai_poFile->R(&m_Size,1) ;
	} else {
		m_Size=ai_TestSize ;
	} ;

	if ( ( m_Size<0 || m_Size>256 ) || (ai_TestSize>=0  && m_Size != ai_TestSize ) ) {
		//ERROR
		SwOk=M4_FALSE ;
	} else if (m_Size>0) {
		if ( ! ai_poFile->R(&m_Data,m_Size) ) {
			SwOk=M4_FALSE ;
		};
	} ;
	m_Initialized=SwOk ;

	return SwOk ;
}

//=============================================================>ClENG_GIF_SignatureBlock

m4bool_t ClENG_GIF_SignatureBlock::Init(ClMIT_File* ai_poFile) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4char_t m_GIFStr[4] ;

	if ( ! ClENG_GIF_Block::Init(ai_poFile, 6, M4_FALSE)  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//GIF mark test

	if (strncmp( (char *)m_Data,"GIF",3) != 0) {
		SwOk=M4_FALSE;
		*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163005, 
				"Invalid GIF File Format", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		goto exit;
	};

	//Version test
	strncpy(m_Version, (char *)&m_Data[3],3);
	m_Version[3] = '\0';
	if ((strcmp(m_Version, "87a") != 0) && (strcmp(m_Version, "89a") != 0)) {
		SwOk=M4_FALSE;
		*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163006, 
			"Wrong GIF file version number", ClMIT_Msg::MIXED_OUT)
			<< SEND_MSG ;
		goto exit;
	};

exit:
	return SwOk ;
}

//============================================================>ClENG_GIF_ScreenDescriptorBlock

m4bool_t ClENG_GIF_ScreenDescriptorBlock::Init(ClMIT_File* ai_poFile) 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( ! ClENG_GIF_Block::Init(ai_poFile, 7, M4_FALSE)  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_RasterWidth	= LM_to_uint(m_Data[0],m_Data[1]);
	m_RasterHeight	= LM_to_uint(m_Data[2],m_Data[3]);

	m_SwGlobalColorMap = m_Data[4]&0x80 ;
	m_ColorResolution = (((m_Data[4]&0x70)>>4)+1);
	m_SizeOfGlobalColorTable = 2<<( (m_Data[4]&0x07) ) ;

	m_BackgroundColorIndex	= m_Data[5] ; 
	m_PixelAspectRatio		= m_Data[6] ;

exit:
	return SwOk ;
}

//=======================================================>ClENG_GIF_ImageDescriptorBlock

m4bool_t ClENG_GIF_ImageDescriptorBlock::Init(ClMIT_File* ai_poFile) 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( ! ClENG_GIF_Block::Init(ai_poFile, 9, M4_FALSE)  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_PixelWidth = LM_to_uint(m_Data[4], m_Data[5]) ;
	m_PixelHeight = LM_to_uint(m_Data[6], m_Data[7]) ;
	m_SwInterlaced = m_Data[8]&0x40 ;
	m_SwLocalColorTable = m_Data[8]&0x80 ;
	m_LocalColorTableSize =  2 << (m_Data[8]&0x07)  ;

exit:
	return SwOk ;
}


//============================================================>ClENG_GIF_BlockTools

m4bool_t ClENG_GIF_BlockTools::LoadPalette( 
	ClMIT_File* ai_poFile, ClENG_Palette *ao_poPalette, 
	m4int16_t ai_PaletteEntries) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t i ;
	ClENG_GIF_Block rgbBlock ;

	if ( ! ao_poPalette->Init(ai_PaletteEntries, ClMIT_Trace::PROPAGATE_ERROR) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	for (i=0; i <ai_PaletteEntries ; ++i) {
		if ( ! rgbBlock.Init(ai_poFile,3,M4_FALSE) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		ao_poPalette->SetColour(i,i,"", rgbBlock.m_Data[0], rgbBlock.m_Data[1], 
			rgbBlock.m_Data[2]);
	};

exit:
	return SwOk ;
}

m4bool_t ClENG_GIF_BlockTools::SkipSubBlocks(ClMIT_File* ai_poFile) 
{
	m4bool_t SwOk=M4_TRUE;
	ClENG_GIF_Block block ;

	while (M4_TRUE) {
		if ( ! block.Init( ai_poFile) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( block.IsTerminator() ) break ;
	} ;

exit:
	return SwOk ;
}

//================================================================>ClENG_GIF_ByteStreamer

ClENG_GIF_ByteStreamer::ClENG_GIF_ByteStreamer()
{
	m_poFile=NULL ;
	m_Readed=0 ;
}

m4bool_t ClENG_GIF_ByteStreamer::Init(ClMIT_File* ai_poFile) 
{
	m_poFile = ai_poFile ;	
	m_Readed = 0 ;
	return m_CurrentBlock.Init(ai_poFile) ;
}

ENG_GIF_BYTE ClENG_GIF_ByteStreamer::PopByte(m4bool_t *ao_SwOk) 
{
	ENG_GIF_BYTE  v ;

	if ( m_CurrentBlock.GetSize() > m_Readed ) {
		v = m_CurrentBlock.m_Data[m_Readed] ;
		++m_Readed ;
	} else {
		if ( ! m_CurrentBlock.Init(m_poFile) || m_CurrentBlock.GetSize()==0  ) {
			*ao_SwOk=M4_FALSE ;
		} else {
			v = m_CurrentBlock.m_Data[0] ;
			m_Readed=1 ;
		} ;
	} ;
	return v ;
}


//================================================================>ClENG_GIF_CodeStreamer

ClENG_GIF_CodeStreamer::ClENG_GIF_CodeStreamer()
{
	m_PackBitSize = MIN_BIT_SIZE ;

	m_CurrentByte = 0 ;
	m_BitsUnreaded = 0 ;
}

m4bool_t ClENG_GIF_CodeStreamer::Init(ClMIT_File* ai_poFile, m4uint8_t ai_PackBitSize) 
{
	m_PackBitSize = ai_PackBitSize  ;
	if ( ! M4_IN_RANGE(m_PackBitSize, MIN_BIT_SIZE, MAX_BIT_SIZE) ) {
		return M4_FALSE ;
	} ;
	m_CurrentByte=0 ;
	m_BitsUnreaded=0 ;

	return m_ByteStreamer.Init(ai_poFile) ;
}

m4bool_t ClENG_GIF_CodeStreamer::ResetPackBitSize(m4uint8_t ai_PackBitSize) 
{
	m_PackBitSize = ai_PackBitSize  ;
	if ( ! M4_IN_RANGE(m_PackBitSize, MIN_BIT_SIZE, MAX_BIT_SIZE) ) {
		return M4_FALSE ;
	} ;
	//m_CurrentByte y m_BitsUnreaded siguen siendo válidos

	return M4_TRUE ;
}

ENG_GIF_CODE ClENG_GIF_CodeStreamer::PopCode(m4bool_t *ao_SwOk) 
{
	ENG_GIF_CODE W_code;
	m4uint16_t W_unpacked, R_bits ;
	ENG_GIF_BYTE R_valueAux ; 
	static ENG_GIF_BYTE maskAux[9] =
	{
		0x00, 
		0x01, 0x03, 0x07, 0x0F,
		0x1F, 0x3F, 0x7F, 0xFF
	} ;

	for (W_unpacked=0, W_code = 0x00; W_unpacked<m_PackBitSize;  ) {

		//Me aseguro de que haya algun bit por leer
		if (m_BitsUnreaded==0) {
			m_CurrentByte = m_ByteStreamer.PopByte(ao_SwOk) ;
			if (! *ao_SwOk) return 0 ;
			m_BitsUnreaded=8 ;
		} ;
	
		//Leo los bits que correspondan del actual byte
		//Dejo los no leidos en la parte LSB

		R_bits = M4_MIN(m_BitsUnreaded, (m_PackBitSize-W_unpacked) ) ;
		R_valueAux = m_CurrentByte & maskAux[R_bits] ;
		m_BitsUnreaded -= R_bits  ;
		m_CurrentByte = m_CurrentByte >> R_bits ;

		//Dejos los bits leidos en R_valueAux donde corresponda en el code

		if (W_unpacked==0) {
			W_code = R_valueAux  ;
		} else {
			W_code |=  ((ENG_GIF_CODE)R_valueAux) << W_unpacked ; 
		} ;

		//Actualizo W_unpacked

		W_unpacked+=R_bits ;
	} ;
	return W_code ;
}

//================================================================>ClENG_GIF_LZW_Decoder

ClENG_GIF_LZW_Decoder::ClENG_GIF_LZW_Decoder() 
{
}

void ClENG_GIF_LZW_Decoder::End()
{
	m_sp = NULL ; //Flag 'fin de decodificacion'
}

m4bool_t ClENG_GIF_LZW_Decoder::Init(ClMIT_File* ai_poFile) 
{
	ENG_GIF_BYTE byteAux ;

	ClENG_GIF_LZW_Decoder::End() ;

	m_poFile = ai_poFile ;

	//****Lee y valida m_BitSize

	if ( ! ai_poFile->R(&byteAux,1) ) return M4_FALSE ;
	m_BitSize = byteAux ;
	if ( ! M4_IN_RANGE(m_BitSize, 2, 8) ) return M4_FALSE ;

	//****Inicializa valores de proceso

	m_currSize = m_BitSize + 1;

	m_topSlot = 1 << m_currSize ;
	m_clear = 1 << m_BitSize ;
	m_ending = m_clear + 1;
	m_slot = m_newCodes = m_ending + 1;
	m_nAvailBytes = m_nBitsLeft = 0 ;

	m_sp = m_stack ;

	/* Initialize in case they forgot to put in a clear code.
	* (This shouldn't happen, but we'll try and decode it anyway...)
    */
	m_oc = m_fc = 0;

	m_BadCodeCount = 0 ;

	//****Inicializa el code streamer

	
	if ( m_CodeStreamer.Init(ai_poFile, byteAux ) ) {
		m_CodeStreamer.ResetPackBitSize(m_currSize) ;
		return M4_TRUE ;
	} else {
		return  M4_FALSE ;
	} ;
	
	return M4_TRUE ;
}


ENG_GIF_BYTE ClENG_GIF_LZW_Decoder::PopValue(m4bool_t *ao_SwOk) 
{
	ENG_GIF_CODE code, c ;

	//****Si es fin de decodificacion, siempre da 0

	if (m_sp==NULL) return 0 ;

	//****Si alguna llamda anterior en la decodificacion genero valores de salida,
	//	estos se consumen antes de seguir decodificando.

	if ( m_sp > m_stack ) {
		return *--m_sp ;
	} ;


   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character" for
    * each code onto the stack.  When the list reaches a single "character"
    * we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
    * included for the clear code, and the whole thing ends when we get
    * an ending code.
    */
	
	while ( M4_TRUE ) {

		c = m_CodeStreamer.PopCode(ao_SwOk) ;

		//If we had a error, return without completing the decode
		if (c < 0 || ! (*ao_SwOk) ) {
			//Marca fin de datos
			m_sp=NULL ;
			return 0 ;
        } ;

		if (c == m_ending) {

			//***************************ENDING CODE ********************************
			
			m_sp=NULL ; //Marca fin de datos
			return 0 ;

		} else if (c == m_clear) {

			//***************************CLEAR CODE ********************************
			
			//If the code is a clear code, reinitialize all necessary items.
			
			m_currSize = m_BitSize + 1;
			m_CodeStreamer.ResetPackBitSize(m_currSize) ;

			m_slot = m_newCodes;
			m_topSlot = 1 << m_currSize;

			/* Continue reading codes until we get a non-clear code
			* (Another unlikely, but possible case...)
			*/
			while ((c = m_CodeStreamer.PopCode(ao_SwOk) ) == m_clear) {
				;
			} ;

			/* If we get an ending code immediately after a clear code
			* (Yet another unlikely case), then break out of the loop.
			*/
			if (c == m_ending) {
				m_sp=NULL ; //Marca fin de datos
				return 0 ;
			} ;

			/* Finally, if the code is beyond the range of already set codes,
			* (This one had better NOT happen...  I have no idea what will
			* result from this, but I doubt it will look good...) then set it
			* to color zero.
			*/
         
			if (c >= m_slot) {
				c = 0;
			} ;

			//Reset old code and first code to the readed code

			m_oc = m_fc = c;

			//And let us not forget to put the char into the buffer... 

			return c ;

  
		} else {

			//***************************CODE CODE ...********************************

			/* In this case, it's not a clear code or an ending code, so
			* it must be a code code...  So we can now decode the code into
			* a stack of character codes. (Clear as mud, right?)
			*/
			code = c;

			/* Here we go again with one of those off chances...  If, on the
			* off chance, the code we got is beyond the range of those already
			* set up (Another thing which had better NOT happen...) we trick
			* the decoder into thinking it actually got the last code read.
			* (Hmmn... I'm not sure why this works...  But it does...)
			*/
			if (code >= m_slot) {
            	if (code > m_slot) {
					++m_BadCodeCount;
				} ;
				code = m_oc;
				*m_sp++ = m_fc;
			} ;

			/* Here we scan back along the linked list of prefixes, pushing
			* helpless characters (ie. suffixes) onto the stack as we do so.
			*/
			while (code >= m_newCodes) {
				*m_sp++ = m_suffix[code];
				code = m_prefix[code];
            } ;

			/* Push the last character on the stack, and set up the new
			* prefix and suffix, and if the required slot number is greater
			* than that allowed by the current bit size, increase the bit
			* size.  (NOTE - If we are all full, we *don't* save the new
			* suffix and prefix...  I'm not certain if this is correct...
			* it might be more proper to overwrite the last code...
			*/
			*m_sp++ = code;
			
			if (m_slot < m_topSlot) {
				m_suffix[m_slot] = m_fc = code;
				m_prefix[m_slot++] = m_oc;
				m_oc = c;
            } ;
         
			if (m_slot >= m_topSlot) {
				if (m_currSize < MAX_BIT_SIZE) {
					m_topSlot <<= 1;
					++m_currSize;
					m_CodeStreamer.ResetPackBitSize(m_currSize) ;
				} ;
			} ;

			/* Now that we've pushed the decoded string (in reverse order)
			* onto the stack, lets pop it off and put it into our decode
			* buffer...  
			* Puede que mas de un caracter este disponible; si es asi, se retorna el
			*	primero, --sp, y el if ( m_sp > m_stack ) ... del principio 
			*	consumirá los que queden.
			*/
			if (m_sp > m_stack) {
				return *--m_sp ;
			} ;

		} //end of cc/not cc proccess path
	} //end of while proccess
} ;


