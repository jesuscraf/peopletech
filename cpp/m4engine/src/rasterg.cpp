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

#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"

#include "sysinfo.h"
#include "clfile.h"
#include "gif.h"
#include "jpeg.h"

#include "rasterg.hmg"
#include "rasterg.h"

#include <math.h>

//==============================================================================ClENG_RasterG

ClENG_RasterG::ClENG_RasterG()
{
}

void ClENG_RasterG::End()
{
	m_oMap.End() ;

	ClENG_Field::End() ;
}

void ClENG_RasterG::Init( 
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t * ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel, 
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize, 
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		m4bool_t ai_bSwGraphics,
		m4bool_t ai_bTempWebImage) 
{

	ClENG_RasterG::End();

	m_bSwGraphics = ai_bSwGraphics;
	m_bTempWebImage = ai_bTempWebImage;

	ClENG_Field::Init( ai_poFieldEnv, ai_poLocation, ai_Top, ai_Left, ai_Width, ai_Height, ai_pcExpr, 
		ai_pcIndexExpr, ai_uiIndexLevel, ai_pcHyperlink, ai_bSwStatic, 
		ai_bSwDuplicated, ai_bSwOptimize, ai_pcFrame, ai_FrameFillColour,
		M4_FALSE, M4_FALSE, NULL);
}


DvcUnit_t ClENG_RasterG::PostLoadValue() 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ;
	m4uint32_t RasterX, RasterY ;

	// Descarga de map

	m_oMap.UnloadMap() ;

	if ( m_oValue.GetType()!=ClMIT_ChnValue::STRING ) {
		//Error local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_70 ] ) 
			<< m_oExpr.GetExpressionStr() << SEND_MSG ;
		goto exit ;
	} ;

	// Si NO hay fichero grafico, exit

	if (ClMIT_Str::StrLenTrim( m_oValue.GetString() ) == 0) goto exit;

	////Concatenamos con el path del Registry si la propiedad esta activada
	if(m_bSwGraphics){		
		ClMIT_Str StrAux, StrAux2 ;

		ClMIT_SysInfo::ExtractFromPath(m_oValue.GetString(), &StrAux, &StrAux2); 
		m_poInsp->m_poSysInfo->GetValue( 
			Dvc->IsColourOn() ? ENG_RK_GRAPHICS_PATH : ENG_RK_GRAPHICS_PATH_BW, 
			&StrAux ) ;
		ClMIT_SysInfo::AppendToPath( &StrAux, StrAux2.InChar(0));
		m_oValue.InitString(StrAux);
	}

	//Lee mapa si procede
		
	if ( Dvc->IsRasterPreloaded() ) {

		if ( ! m_oMap.LoadFromFile(m_oValue.GetString()) ) {
			goto exit ;
		} ;

		//JPEG no habilitado para RasterPreloadDevices
		if(ClENG_RasterMap::GetFileFormat(m_oValue.GetString())==ClENG_RasterMap::JPG) {
			//Error local
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163015, 
									"JPG image files are only accepted in HTML, PDF or Viewer formats.", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
			goto exit ;
		};

		// Si hubo problemas, no usa m_oMap

		if ( m_oMap.IsMapLoaded()  ) {

			//Escala si procede

			if ( ! Dvc->IsHardScale() ) {
				
				Dvc->GetScaleData( GetWidth(), GetHeight(),		//Area a imprimir.
					m_oMap.GetRasterX(), m_oMap.GetRasterY(),	//Size bitmap
					&RasterX, &RasterY ) ;

				m_oMap.Scale(RasterX,RasterY) ;
			} ;
		} ;
	} ;

exit:
	return 0 ;
}

m4bool_t ClENG_RasterG::FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval )
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poFieldEnv->m_poDvc ;
	m4uint16_t IndexTargetID;

	if ( m_oValue.GetType()!=ClMIT_ChnValue::STRING ) {
		//Error local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_70 ] ) 
			<< m_oExpr.GetExpressionStr() << SEND_MSG ;
		goto exit ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() &&
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL)
	{
		ClMIT_Str strAux ;
		m_oValue.GetTraceValue(&strAux) ;
		*m_poInsp->m_poTrace << ClMIT_Msg(ENG_TGI_FIELD, 
			"Printing IMAGE Field %0:s at (y=%1:s mm,x=%2:s mm), value %3:s from Expression %4:s", 
			ClMIT_Msg::MIXED_OUT) 
			<< m_oDesc.InChar(0)
			<< ClMetric::DvcToMilim(ai_Y)
			<< ClMetric::DvcToMilim(ai_X)
			<< strAux.InChar(0)
			<< m_oExpr.GetExpressionStr()
			<< SEND_MSG ;
	}
	#endif


	if(m_iIndexLevel!=ClENG_Field::NO_INDEX_ENTRY) {
		Dvc->NewIndexEntry(ai_iNumPage, m_iIndexLevel, m_oIndexStr, &IndexTargetID);
	} else {
		IndexTargetID = ClENG_OutDevice::DVC_NO_INDEX_TARGET ;
	} ;

	// Volcar
		
	if ( ! Dvc->IsRasterPreloaded() ) {
	
		if (ClMIT_Str::StrLenTrim( m_oValue.GetString() ) != 0) {	//Solo si hay fichero
			Dvc->PrintRasterFile( ai_iNumPage, ai_X, ai_Y, GetWidth(), GetHeight(), 
				m_oValue.GetString(), m_oHyperlinkStr, IndexTargetID, m_bTempWebImage ) ;
		} ;
		
	} else {

		// Solo si no hubo problemas en la carga del fichero, imprime

		if ( m_oMap.IsMapLoaded()  ) {
			Dvc->PrintRasterMap( ai_iNumPage, ai_X, ai_Y, GetWidth(), GetHeight(), &m_oMap, m_oHyperlinkStr, IndexTargetID ) ;
		} ;

	} ;

exit:

	if ( !IsStatic() ) {
		m_oMap.UnloadMap() ;
	} ;
	
	return M4_TRUE ;
}



//=================================================================================ClENG_RasterMap

m4char_t const * ClENG_RasterMap::m_pcExt[ClENG_RasterMap::COUNT_FILE_FORMATS] = 
{
	".BMP",
	".GIF",
	".JPG"
} ;


ClENG_RasterMap::ClENG_RasterMap() 
{
	m_poInsp=GET_INSP();
	m_puiByte=NULL ;
}

void ClENG_RasterMap::End() 
{
	m_oPalette.End() ;
	M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;
}

void ClENG_RasterMap::UnloadMap() 
{
	m_oPalette.End() ;
	M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;
} 

void ClENG_RasterMap::Init() 
{
	ClENG_RasterMap::End() ;
}

ClENG_RasterMap::FILE_FORMATS ClENG_RasterMap::GetFileFormat(const m4char_t *ai_pcFileName)
{
	FILE_FORMATS FileFormat;
	size_t L1, L2;
	m4int16_t B;

	for (B=0, FileFormat=UNKNOWN, L1 = ClMIT_Str::StrLenTrim(ai_pcFileName) ; 
		B<COUNT_FILE_FORMATS; ++B) 
	{
		L2=ClMIT_Str::StrLenTrim(m_pcExt[B]);
		if (L1>L2) {
			if (stricmp(&ai_pcFileName[L1-L2],m_pcExt[B])==0) {
				FileFormat=(FILE_FORMATS)B;
				break;
			};
		};
	};

	return FileFormat;
}


m4bool_t ClENG_RasterMap::LoadFromFile(const m4char_t *ai_pcFileName) 
{
	FILE_FORMATS FileFormat ;
	m4bool_t SwOk=M4_TRUE ;

	FileFormat=GetFileFormat(ai_pcFileName);

	if (FileFormat==UNKNOWN) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_10 ] ) 
			<< ai_pcFileName << SEND_MSG ;
		//Warning, local
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
		
	
	switch (FileFormat)
	{
	case BMP:
		SwOk=LoadFromBMP(ai_pcFileName);
		break;
	case GIF:
		SwOk=LoadFromGIF(ai_pcFileName);
		break;
	case JPG:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_10 ] ) 
			<< ai_pcFileName << SEND_MSG ;
		//Warning, local
		SwOk=M4_FALSE ;
		break;
	case UNKNOWN:
		break;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClENG_RasterMap::Scale( m4uint32_t ai_uiNewRasterX, m4uint32_t ai_uiNewRasterY  ) 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( !IsMapLoaded() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ai_uiNewRasterY == m_uiRasterY && ai_uiNewRasterX == m_uiRasterX ) goto exit ;

	switch (m_BitsPixel) {
	case MONOCHROME:
		ScaleMonochrome(ai_uiNewRasterX,ai_uiNewRasterY) ;
		break;
	default:
		SwOk=M4_FALSE ;
		goto exit ;
		break ;
	} ;

exit:
	return SwOk ;
}



// UTILIZO EL ALGORITMO DE BRESENHAM

void ClENG_RasterMap::ScaleMonochrome( m4uint32_t ai_uiNewRasterX, m4uint32_t ai_uiNewRasterY ) 
{
	m4uint8_t *Buff, BuffBit, BuffByte; 
	m4uint8_t *Map,  MapBit, MapByte; size_t MapRowSkip  ;
	m4uint32_t Y, X ; 
	m4float_t DeltaY ;
	m4int32_t Delta_x , Discr, DiscrK1, DiscrK2, DiscrReset ;
	m4int32_t Delta_fx ;
	m4bool_t  SwfxIsMap ; 
	m4bool_t  swResetMapByte ;

	struct StNewMap {
		m4uint8_t *m_puiByte ;
		m4uint32_t m_uiRasterX, m_uiRasterY ;	
		m4uint32_t m_uiBytesRow ;			
		m4uint32_t m_uiLastXIndex ;			
	} NewMap ;

	//----------------->Creo nuevo mapa destino

	NewMap.m_uiRasterY = ai_uiNewRasterY;
	NewMap.m_uiRasterX = ai_uiNewRasterX;
	NewMap.m_uiBytesRow=(NewMap.m_uiRasterX/8)+ (NewMap.m_uiRasterX % 8 ? 1 : 0 ) ;		
	NewMap.m_uiLastXIndex = NewMap.m_uiBytesRow -1 ;	//Calcular antes de alinear
	// Alinea a LONG
	if ( NewMap.m_uiBytesRow % sizeof(m4int32_t) ) {
		NewMap.m_uiBytesRow += sizeof(m4int32_t) -  NewMap.m_uiBytesRow % sizeof(m4int32_t) ;
	} ;
	M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		NewMap.m_puiByte, m4uint8_t ,NewMap.m_uiRasterY*NewMap.m_uiBytesRow ) ;

	//<-----------------

	// Bresenham no aplicable en eje Y, siempre debe recorrerse por filas.

	DeltaY=(m4float_t)m_uiRasterY/NewMap.m_uiRasterY ;

	// Incializaciones para Bresenham

	if (NewMap.m_uiRasterX>m_uiRasterX) {
		SwfxIsMap=M4_TRUE ;	
		Delta_x=NewMap.m_uiRasterX ;
		Delta_fx=m_uiRasterX ;
	} else {
		SwfxIsMap=M4_FALSE ;	
		Delta_x=m_uiRasterX ;
		Delta_fx=NewMap.m_uiRasterX ;
	} ;
	DiscrK1=Delta_fx*2 ;
	DiscrK2=2*(Delta_fx-Delta_x) ;
	DiscrReset=DiscrK1-Delta_x ;
	
	//Otras inicializaciones

	//BUG 0088565: tratamiento flag swResetMapByte, BuffByte=0x00 e 
	//			if (BuffByte) {	*Buff=BuffByte ; };

	MapRowSkip  = m_uiBytesRow ;

	if (!SwfxIsMap) {

		for (Y=0; Y<NewMap.m_uiRasterY; ++Y) {
	
			// Calcula linea ORIGEN map a procesar
	
			Map= & m_puiByte[((m4uint16_t)(DeltaY*Y))*MapRowSkip] ;

			//A 0 ultimo byte de la linea DESTINO, por si sobran pixels
			Buff= & NewMap.m_puiByte[ Y * NewMap.m_uiBytesRow ] ;
			Buff[ NewMap.m_uiLastXIndex ]= 0x00 ;
			BuffBit=MapBit=0x80 ;
			BuffByte=0x00; MapByte=*Map ;
			Discr=DiscrReset ;
		
			swResetMapByte=M4_FALSE ;

			for (X=m_uiRasterX; X; --X) {

				if (swResetMapByte) {
					swResetMapByte=M4_FALSE ;
					MapByte=*Map ;
				} ;

				BuffByte = ( BuffByte & (~BuffBit) ) | ( (MapByte & MapBit) ? BuffBit : 0x00  ) ;

				if (Discr<0) {
					Discr += DiscrK1 ;
				} else {
					Discr += DiscrK2 ;
					if (BuffBit!=0x01) {
						BuffBit= BuffBit >> 1 ;
					} else {
						BuffBit=0x80 ;
						*Buff=BuffByte ; ++Buff ; BuffByte=0x00 ;
					} ;
				} ;
				if (MapBit!=0x01) {
					MapBit= MapBit >> 1 ;
				} else {
					MapBit=0x80 ;
					++Map ; swResetMapByte=M4_TRUE ;
				} ;

			} ;

			//BuffByte sera != 0 solo si queda pendiente almacenar algo en el ultimo byte
			//  de la ultima linea, puesto que cuando se almacena un byte via *Buff, tambien
			//	se pone BuffByte a 0x00.
			if (BuffByte) {
				*Buff=BuffByte ;	
			} ;

		} ;

	} else {

		for (Y=0; Y<NewMap.m_uiRasterY; ++Y) {
	
			// Calcula linea map a procesar
	
			Map= & m_puiByte[((m4uint16_t)(DeltaY*Y))*MapRowSkip] ;

			//A 0 ultimo byte del buffer, por si sobran pixels

			Buff= & NewMap.m_puiByte[ Y * NewMap.m_uiBytesRow ] ;
			Buff[ NewMap.m_uiLastXIndex ]= 0x00 ;
			BuffBit=MapBit=0x80 ;
			BuffByte=0x00; MapByte=*Map ;
			Discr=DiscrReset ;

			swResetMapByte=M4_FALSE ;
		
			for (X=NewMap.m_uiRasterX; X; --X) {

				if (swResetMapByte) {
					swResetMapByte=M4_FALSE ;
					MapByte=*Map ;
				} ;

				BuffByte = ( BuffByte & (~BuffBit) ) | ( (MapByte & MapBit) ? BuffBit : 0x00  ) ;

				if (Discr<0) {
					Discr += DiscrK1 ;
				} else {
					Discr += DiscrK2 ;
					if (MapBit!=0x01) {
						MapBit= MapBit >> 1 ;
					} else {
						MapBit=0x80 ;
						++Map ; swResetMapByte=M4_TRUE ;
					} ;
				} ;
				if (BuffBit!=0x01) {
					BuffBit= BuffBit >> 1 ;
				} else {
					BuffBit=0x80 ;
					*Buff=BuffByte ; ++Buff ; BuffByte=0x00 ;
				} ;

			} ;

			//BuffByte sera != 0 solo si queda pendiente almacenar algo en el ultimo byte
			//  de la ultima linea, puesto que cuando se almacena un byte via *Buff, tambien
			//	se pone BuffByte a 0x00.
			if (BuffByte) {
				*Buff=BuffByte ;	
			} ;

		} ;

	} ;

	//----------------->Sustituyo mapa viejo por nuevo, borrando el viejo

	M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MEM_GRP_RASTERG, 
		m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;

	m_puiByte		= NewMap.m_puiByte  ;
	m_uiRasterX		= NewMap.m_uiRasterX;
	m_uiRasterY		= NewMap.m_uiRasterY;	
	m_uiBytesRow	= NewMap.m_uiBytesRow ;			
	m_uiLastXIndex	= NewMap.m_uiLastXIndex ;

	//<-----------------
} 


//------------->Carga BMP

typedef unsigned short BMP_WORD  ;
typedef unsigned long  BMP_DWORD  ;
typedef m4int32_t      BMP_LONG;
typedef m4uint8_t	   BMP_BYTE ;

enum BMP_COMPRESSION { XXBMP_BI_RGB=0, XXBMP_BI_RLE8=1, XXBMP_BI_RLE4=2, XXBI_BITFIELDS=3 } ;

#define BMP_TYPE_ID 0x4D42	

struct BMP_FILEHEADER {
	BMP_WORD	bfType ;
	BMP_DWORD	bfSize ;
	BMP_WORD	bfReserved1 ;
	BMP_WORD	bfReserved2 ;
	BMP_DWORD	bfOffBits ;
} ;

struct BMP_INFOHEADER{
	BMP_DWORD      biSize;
	BMP_LONG       biWidth;
	BMP_LONG       biHeight;
	BMP_WORD       biPlanes;
	BMP_WORD       biBitCount;
	BMP_DWORD      biCompression;
	BMP_DWORD      biSizeImage;
	BMP_LONG       biXPelsPerMeter;
	BMP_LONG       biYPelsPerMeter;
	BMP_DWORD      biClrUsed;
	BMP_DWORD      biClrImportant;
} ;

struct BMP_RGBQUAD {
	BMP_BYTE		rgbBlue ;
	BMP_BYTE		rgbGreen ;
	BMP_BYTE		rgbRed ;
	BMP_BYTE		rgbReserved ;
} ;

// Tamaños de las estructuras
#define SIZE_BMP_FILEHEADER (2*sizeof(unsigned long) + 3*sizeof(unsigned short))
#define SIZE_BMP_INFOHEADER (4*sizeof(m4int32_t)+5*sizeof(unsigned long) + 2*sizeof(unsigned short))
#define SIZE_BMP_RGBQUAD (4*sizeof(m4uint8_t))


// Offsets para leer miembros uno a uno; necesario por posibles desplazamientos en las
//	estructuras, debidas a alineaciones.

#define FILEHEADER_BEGIN 0x00	
#define INFOHEADER_BEGIN sizeof(BMP_WORD)*3+sizeof(BMP_DWORD)*2	

static m4bool_t ENG_BMP_LoadHeader(
	ClMIT_File *ai_F,
	BMP_FILEHEADER *ao_FileHeader, BMP_INFOHEADER *ao_InfoHeader)
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_BinaryAdapter Adapter ;

	ai_F->SetPos(FILEHEADER_BEGIN) ;
	ai_F->R(&ao_FileHeader->bfType,sizeof(ao_FileHeader->bfType)) ;
	ai_F->R(&ao_FileHeader->bfSize,sizeof(ao_FileHeader->bfSize)) ;
	ai_F->R(&ao_FileHeader->bfReserved1,sizeof(ao_FileHeader->bfReserved1)) ;
	ai_F->R(&ao_FileHeader->bfReserved2,sizeof(ao_FileHeader->bfReserved2)) ;
	ai_F->R(&ao_FileHeader->bfOffBits,sizeof(ao_FileHeader->bfOffBits)) ;

	//Deteccion automatica conversion ENDIAN

	ao_FileHeader->bfType=Adapter.DecodeInt(ao_FileHeader->bfType);

	if ( ao_FileHeader->bfType != BMP_TYPE_ID ) {

		Adapter.SetFileStorage(  Adapter.GetFileStorage()==ClMIT_BinaryAdapter::LITLE_ENDIAN ?
			ClMIT_BinaryAdapter::BIG_ENDIAN : ClMIT_BinaryAdapter::LITLE_ENDIAN ) ;

		ao_FileHeader->bfType=Adapter.DecodeInt(ao_FileHeader->bfType);

		if (ao_FileHeader->bfType != BMP_TYPE_ID ) {
			*GET_INSP()->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_30 ] ) 
				<< ai_F->GetName() << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	//Conversion FileHeader

	ao_FileHeader->bfSize=Adapter.DecodeLong(ao_FileHeader->bfSize);
	ao_FileHeader->bfReserved1=Adapter.DecodeInt(ao_FileHeader->bfReserved1);
	ao_FileHeader->bfReserved2=Adapter.DecodeInt(ao_FileHeader->bfReserved2);
	ao_FileHeader->bfOffBits=Adapter.DecodeLong(ao_FileHeader->bfOffBits);

	//Lectura InfoHeader

	ai_F->SetPos(INFOHEADER_BEGIN) ;
	ai_F->R(&ao_InfoHeader->biSize,sizeof(ao_InfoHeader->biSize)) ;
	ai_F->R(&ao_InfoHeader->biWidth,sizeof(ao_InfoHeader->biWidth)) ;
	ai_F->R(&ao_InfoHeader->biHeight,sizeof(ao_InfoHeader->biHeight)) ;
	ai_F->R(&ao_InfoHeader->biPlanes,sizeof(ao_InfoHeader->biPlanes)) ;
	ai_F->R(&ao_InfoHeader->biBitCount,sizeof(ao_InfoHeader->biBitCount)) ;
	ai_F->R(&ao_InfoHeader->biCompression,sizeof(ao_InfoHeader->biCompression)) ;
	ai_F->R(&ao_InfoHeader->biSizeImage,sizeof(ao_InfoHeader->biSizeImage)) ;
	ai_F->R(&ao_InfoHeader->biXPelsPerMeter,sizeof(ao_InfoHeader->biXPelsPerMeter)) ;
	ai_F->R(&ao_InfoHeader->biYPelsPerMeter,sizeof(ao_InfoHeader->biYPelsPerMeter)) ;
	ai_F->R(&ao_InfoHeader->biClrUsed,sizeof(ao_InfoHeader->biClrUsed)) ;
	ai_F->R(&ao_InfoHeader->biClrImportant,sizeof(ao_InfoHeader->biClrImportant)) ;
	
	//Conversion InfoHeader

	ao_InfoHeader->biSize=Adapter.DecodeLong(ao_InfoHeader->biSize);	
	ao_InfoHeader->biWidth=Adapter.DecodeLong(ao_InfoHeader->biWidth);	
	ao_InfoHeader->biHeight=Adapter.DecodeLong(ao_InfoHeader->biHeight);	
	ao_InfoHeader->biPlanes=Adapter.DecodeInt(ao_InfoHeader->biPlanes);		
	ao_InfoHeader->biBitCount=Adapter.DecodeInt(ao_InfoHeader->biBitCount);	
	ao_InfoHeader->biCompression=Adapter.DecodeLong(ao_InfoHeader->biCompression);	
	ao_InfoHeader->biSizeImage=Adapter.DecodeLong(ao_InfoHeader->biSizeImage);	
	ao_InfoHeader->biXPelsPerMeter=Adapter.DecodeLong(ao_InfoHeader->biXPelsPerMeter);	
	ao_InfoHeader->biYPelsPerMeter=Adapter.DecodeLong(ao_InfoHeader->biYPelsPerMeter);	
	ao_InfoHeader->biClrUsed=Adapter.DecodeLong(ao_InfoHeader->biClrUsed);	
	ao_InfoHeader->biClrImportant=Adapter.DecodeLong(ao_InfoHeader->biClrImportant);	

exit:
	return SwOk ;
}


m4bool_t ClENG_RasterMap::LoadFromBMP(const m4char_t *ai_pcFileName) 
{
	BMP_FILEHEADER FileHeader ;
	BMP_INFOHEADER InfoHeader ;
	m4bool_t SwTopDown ;
	m4bool_t SwOk=M4_TRUE ;

	m4int16_t PaletteCount, PaletteIndex ;
	BMP_RGBQUAD  RGBEntry ;
	
	ClMIT_File F ;


	UnloadMap() ; // Borra, por si acaso hay algo grabado


	if ( !F.Open( ai_pcFileName, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	

	if ( ! ENG_BMP_LoadHeader(&F, &FileHeader, &InfoHeader) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_uiRasterX = InfoHeader.biWidth ;

	if (InfoHeader.biHeight<0) {
		SwTopDown = M4_TRUE ; ;
		m_uiRasterY = -InfoHeader.biHeight ;
	} else {
		SwTopDown = M4_FALSE ; ;
		m_uiRasterY = InfoHeader.biHeight ;
	} ;

	if ( InfoHeader.biPlanes !=1 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_40 ] ) 
			<< ai_pcFileName << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( InfoHeader.biCompression != XXBMP_BI_RGB ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_50 ] ) 
			<< ai_pcFileName << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	// Lee mapa en formato nativo BMP

	switch (InfoHeader.biBitCount) {
	case 1:

		//------------------------------------------------------Monocromo

		m_BitsPixel=MONOCHROME ;
		m_uiBytesRow=(m_uiRasterX/8)+ (m_uiRasterX % 8 ? 1 : 0 ) ;		
		m_uiLastXIndex=m_uiBytesRow-1 ;
		// Alinea a LONG
		if ( m_uiBytesRow % sizeof(BMP_LONG) ) {
			m_uiBytesRow += sizeof(BMP_LONG) -  m_uiBytesRow % sizeof(BMP_LONG) ;
		} ;

		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;
		F.SetPos( FileHeader.bfOffBits ) ;
		F.R( m_puiByte, m_uiRasterY*m_uiBytesRow ) ;
	
		if ( ! SwTopDown ) {

			BMP_LONG *Pos, *Pos2, Swap;
			m4int32_t ContaX, ContaY, X, Y, SkipX2 ; 

			// Conversion top-left a bottom-left y 0-1, 1-0, recorre como long, + rapido.

			ContaY=m_uiRasterY/2;
			ContaX=m_uiBytesRow/sizeof(BMP_LONG) ;
			SkipX2=ContaX*2 ;
			Pos=(BMP_LONG *)m_puiByte ;
			Pos2=(BMP_LONG *)(&m_puiByte[(m_uiRasterY-1)*m_uiBytesRow]) ;
			for (Y=ContaY; Y; --Y, Pos2 -= SkipX2 ) {
				for (X=ContaX; X; --X, ++Pos, ++Pos2) {
					Swap = ~(*Pos);
					*Pos = ~(*Pos2) ;
					*Pos2= Swap ;
				} ;
			} ;
			if (m_uiRasterY%2) {
				Pos=(BMP_LONG *)(&m_puiByte[(m_uiRasterY/2)*m_uiBytesRow] );
				for (; ContaX; --ContaX, ++Pos ) {
					*Pos = ~(*Pos) ;
				} ;
			} ;

		} else {

			m4uint32_t To ; 
			BMP_LONG *Pos;

			// Conversion 0-1, 1-0, recorre como long, + rapido.
			To=(m_uiRasterY*m_uiBytesRow)/sizeof(BMP_LONG) ;
			Pos=(BMP_LONG *)m_puiByte ;
			while (To) {
				*Pos=~(*Pos) ;
				++Pos ;
				--To ;
			} ;
		} ;


		break ;

	case 8:

		//------------------------------------------------------256 colores

		m_BitsPixel=INDEXED_256 ;
		m_uiBytesRow=m_uiRasterX ;		
		m_uiLastXIndex=m_uiBytesRow-1 ;
		// Alinea a LONG
		if ( m_uiBytesRow % sizeof(BMP_LONG) ) {
			m_uiBytesRow += sizeof(BMP_LONG) -  m_uiBytesRow % sizeof(BMP_LONG) ;
		} ;

		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;
		F.SetPos( FileHeader.bfOffBits ) ;
		F.R( m_puiByte, m_uiRasterY*m_uiBytesRow ) ;

		if ( ! SwTopDown ) {

			BMP_LONG *Pos, *Pos2, Swap;
			m4uint32_t ContaX, ContaY, X, Y, SkipX2 ; 

			// Conversion top-left a bottom-left y (0-1, 1-0) ??, recorre como long, + rapido.

			ContaY=m_uiRasterY/2;
			ContaX=m_uiBytesRow/sizeof(BMP_LONG) ;
			SkipX2=ContaX*2 ;
			Pos=(BMP_LONG *)m_puiByte ;
			Pos2=(BMP_LONG *)(&m_puiByte[(m_uiRasterY-1)*m_uiBytesRow]) ;
			for (Y=ContaY; Y; --Y, Pos2 -= SkipX2 ) {
				for (X=ContaX; X; --X, ++Pos, ++Pos2) {
					/*Swap = ~(*Pos);	*/	Swap = *Pos ;
					/* *Pos = ~(*Pos2) ; */ *Pos = *Pos2 ;
					*Pos2= Swap ;
				} ;
			} ;
			/*
			if (m_uiRasterY%2) {
				Pos=(BMP_LONG *)(&m_puiByte[(m_uiRasterY/2)*m_uiBytesRow] );
				for (; ContaX; --ContaX, ++Pos ) {
					*Pos = ~(*Pos) ; 
				} ;
			} ;
			*/

		} else {

			/*
			m4uint32_t To ; 
			BMP_LONG *Pos;

			// Conversion (0-1, 1-0)?, recorre como long, + rapido.
			To=(m_uiRasterY*m_uiBytesRow)/sizeof(BMP_LONG) ;
			Pos=(BMP_LONG *)m_puiByte ;
			while (To) {
				*Pos=~(*Pos) ;
				++Pos ;
				--To ;
			} ;
			*/
			;
		} ;

		//Lectura de paleta: es un modo indexado

		PaletteCount =  InfoHeader.biClrUsed == 0 ? 256 : InfoHeader.biClrUsed ;
		if ( ! m_oPalette.Init(PaletteCount,ClMIT_Trace::PROPAGATE_ERROR) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		};
		
		F.SetPos( INFOHEADER_BEGIN + InfoHeader.biSize ) ;
		for ( PaletteIndex=0 ; PaletteIndex<PaletteCount; ++PaletteIndex) {

			F.R(&RGBEntry.rgbBlue,sizeof(RGBEntry.rgbBlue)) ;
			F.R(&RGBEntry.rgbGreen,sizeof(RGBEntry.rgbGreen)) ;
			F.R(&RGBEntry.rgbRed,sizeof(RGBEntry.rgbRed)) ;
			F.R(&RGBEntry.rgbReserved,sizeof(RGBEntry.rgbReserved)) ;
			
			//No hay que hacer Decode, los miembros RGB son bytes

			m_oPalette.SetColour( PaletteIndex, PaletteIndex, "",
				RGBEntry.rgbRed, RGBEntry.rgbGreen, RGBEntry.rgbBlue );
		} ;
		
		break ;

	
	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ RASTERG_MSG_60 ] )
			<< (m4uint32_t)(InfoHeader.biBitCount) << ai_pcFileName << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
		break ;
	} ;

exit:

	// Transforma mapa en formato BMP nativo, al manejado por CLRasterG
	
	F.Close () ;

	if (!SwOk) UnloadMap() ;

	return IsMapLoaded() ;
}



m4bool_t ClENG_RasterMap::GetFileInfo(
	const m4char_t *ai_pcFileName, FILE_FORMATS *ao_FileFormat, 
	m4uint32_t* ai_uiPixelWidth, m4uint32_t* ai_uiPixelHeight, m4int16_t *ao_bpp )
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_File F ;

	*ao_FileFormat = GetFileFormat(ai_pcFileName);
	if ( *ao_FileFormat==BMP ) {
		BMP_FILEHEADER fileHeader ;
		BMP_INFOHEADER infoHeader ;
		
		if ( !F.Open( ai_pcFileName, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( ! ENG_BMP_LoadHeader(&F, &fileHeader, &infoHeader) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		*ao_bpp = infoHeader.biBitCount ;
		*ai_uiPixelWidth = infoHeader.biWidth ;
		*ai_uiPixelHeight = infoHeader.biHeight<0 ?  -infoHeader.biHeight : infoHeader.biHeight ;

	} else if ( *ao_FileFormat==JPG ) {

		if ( !F.Open( ai_pcFileName, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( ! ClENG_JpgEncoder::LocateJPG_HeaderData(
			&F, ai_uiPixelWidth, ai_uiPixelHeight, ao_bpp ) )
		{
			SwOk=M4_FALSE ;
			goto exit ;
		}


	} else if ( *ao_FileFormat==GIF ) {

		ClENG_GIF_Loader GIF_Loader ;
		ClENG_RasterMap::BITS_PER_PIXEL rasterBPP ;
		m4uint32_t bytesRow, lastXIndex ;
		ClENG_Palette palette ;
		m4uint8_t * puiByte ;
	
		if ( ! GIF_Loader.Init( ai_pcFileName, 
			*ai_uiPixelWidth, *ai_uiPixelHeight, rasterBPP,
			bytesRow, lastXIndex, &palette, 
			puiByte, M4_FALSE ) ) 
		{
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		*ao_bpp = log10((double)palette.GetSize())/log10((double)2) ;
		
	} else {
		*GET_INSP()->m_poTrace << ClMIT_Msg(
			ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163037,
			"Unknow graphic file format, file %0:s", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcFileName << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	

exit:
	return SwOk ;
}


//<------------ Fin carga BMP


//<------------ Salvar mapa como BMP

m4bool_t ClENG_RasterMap::SaveAsBMP(m4char_t *ai_pcFileName)
{
	BMP_FILEHEADER FileHeader;
	BMP_INFOHEADER InfoHeader;
	m4int_t BitsPerPixel, n, width, cnt, y;
	ClMIT_File File;
	m4int32_t wib;
	m4int16_t PaletteCount, PaletteIndex;
	m4bool_t SwOk=M4_TRUE;

	//Abre fichero destino
	if(!File.Open(ai_pcFileName, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN, M4_FALSE)){
		SwOk=M4_FALSE;
		goto exit;
	};

	switch (m_BitsPixel){
		case MONOCHROME: 
			BitsPerPixel=1;
			break;
		case INDEXED_256:	
			BitsPerPixel=8;
			break;
    };

	n= 1<<BitsPerPixel;

	if(n>16) {
		InfoHeader.biBitCount= 8;
	} else if(n>2) {
		InfoHeader.biBitCount= 4;
	} else {
		InfoHeader.biBitCount= 1;
	};

	InfoHeader.biSize= sizeof(InfoHeader);
	InfoHeader.biPlanes= 1;
	InfoHeader.biCompression= 0;
	InfoHeader.biWidth=m_uiRasterX;
	InfoHeader.biHeight=m_uiRasterY;

	switch (BitsPerPixel){
		case 1: width= (InfoHeader.biWidth+31) >> 3; 
				wib = width & ~3;
				break;
		case 4: width= (InfoHeader.biWidth+7)  >> 1; 
			    wib = width & ~3;
			    break;
		case 8: width=  InfoHeader.biWidth+3; 
			    wib = width & ~3;
			    break;
		case 16: width= (InfoHeader.biWidth*2)+3; 
				 wib = width & ~3;
				 break;
		case 24: width= (InfoHeader.biWidth*3)+3; 
				 wib = width & ~3;
			     break;
		case 32: wib = InfoHeader.biWidth*4;
	};
	
	InfoHeader.biSizeImage= InfoHeader.biHeight * wib;
	InfoHeader.biClrUsed= n;
	InfoHeader.biClrImportant= n;
	InfoHeader.biXPelsPerMeter= 0;
	InfoHeader.biYPelsPerMeter= 0;

	FileHeader.bfType= BMP_TYPE_ID;	/* BM */
	FileHeader.bfReserved1= 0;
	FileHeader.bfReserved2= 0;
	FileHeader.bfOffBits = SIZE_BMP_FILEHEADER + InfoHeader.biSize + n*SIZE_BMP_RGBQUAD;
	FileHeader.bfSize= FileHeader.bfOffBits + InfoHeader.biSizeImage;

	File.W(&FileHeader.bfType,sizeof(FileHeader.bfType)) ;
	File.W(&FileHeader.bfSize,sizeof(FileHeader.bfSize)) ;
	File.W(&FileHeader.bfReserved1,sizeof(FileHeader.bfReserved1)) ;
	File.W(&FileHeader.bfReserved2,sizeof(FileHeader.bfReserved2)) ;
	File.W(&FileHeader.bfOffBits,sizeof(FileHeader.bfOffBits)) ;

	File.W(&InfoHeader.biSize,sizeof(InfoHeader.biSize)) ;
	File.W(&InfoHeader.biWidth,sizeof(InfoHeader.biWidth)) ;
	File.W(&InfoHeader.biHeight,sizeof(InfoHeader.biHeight)) ;
	File.W(&InfoHeader.biPlanes,sizeof(InfoHeader.biPlanes)) ;
	File.W(&InfoHeader.biBitCount,sizeof(InfoHeader.biBitCount)) ;
	File.W(&InfoHeader.biCompression,sizeof(InfoHeader.biCompression)) ;
	File.W(&InfoHeader.biSizeImage,sizeof(InfoHeader.biSizeImage)) ;
	File.W(&InfoHeader.biXPelsPerMeter,sizeof(InfoHeader.biXPelsPerMeter)) ;
	File.W(&InfoHeader.biYPelsPerMeter,sizeof(InfoHeader.biYPelsPerMeter)) ;
	File.W(&InfoHeader.biClrUsed,sizeof(InfoHeader.biClrUsed)) ;
	File.W(&InfoHeader.biClrImportant,sizeof(InfoHeader.biClrImportant)) ;

	PaletteCount =  InfoHeader.biClrUsed == 0 ? 256 : InfoHeader.biClrUsed;

	for ( PaletteIndex=0 ; PaletteIndex<PaletteCount; ++PaletteIndex) {
		File.WChar(m_oPalette.GetColourByIndex(PaletteIndex)->GetB());
		File.WChar(m_oPalette.GetColourByIndex(PaletteIndex)->GetG());
		File.WChar(m_oPalette.GetColourByIndex(PaletteIndex)->GetR());
		File.WChar(0);
	} ;

	cnt=(m4int_t)(m_uiRasterY*width);
	for(y=0; y<(m4int_t)m_uiRasterY; y++){
		cnt-=width;
		File.W(m_puiByte+cnt, width);	
	}
	File.Close();

exit:

	return SwOk;
}
//<------------ Fin salvar mapa como BMP


//<------------ Cargar y Salvar mapa como GIF

m4bool_t ClENG_RasterMap::LoadFromGIF(const m4char_t *ai_pcFileName) 
{
	ClENG_GIF_Loader GIF_Loader ;

	return GIF_Loader.Init(
		ai_pcFileName, m_uiRasterX, m_uiRasterY, m_BitsPixel,
		m_uiBytesRow, m_uiLastXIndex, &m_oPalette,
		m_puiByte, M4_TRUE ) ;
}


#define MAXCODE(iNBits)		(((m4int_t) 1 << (iNBits)) - 1)

#define INTERLACED_GIF		0x40
#define NOT_INTERLACED_GIF	0x00
#define GIF_TERMINATOR		';'
#define GIF_IMAGE_SEPARATOR	','
#define GIF_BLOCK_START		'!'
#define GIF_TRANSPARENT_ID	0xf9
#define GIF_COMMENT_ID		0xfe
#define GIF_BITS			12
#define GIF_HSIZE			5003

struct GIF_CONVERSION_DATA {
	m4int_t m_iMaxCode, m_iMaxMaxCode, m_iNBits, m_iMaxBits, m_iInitBits, m_iCurrentBits;
	m4int_t m_iWidth, m_iHeight, m_iCurrentX, m_iCurrentY, m_iLastY, m_iLastPixel;
	m4int_t m_iInterlace, m_iPass, m_iClearFlag, m_iClearCode, m_iEOFCode;
	m4int32_t m_iOutCount, m_iInCount, m_iCountDown, m_iSzLine;
	m4int_t m_iFreeEntry, m_iAcumCnt, m_iHSize;
	BMP_WORD m_wCodeTab[GIF_HSIZE];
	m4int_t m_piHTable[GIF_HSIZE];
	m4char_t m_pcAccum[256]; 
	BMP_BYTE m_ColUsed[256];
	BMP_DWORD m_dwCurAcc;
	BMP_BYTE *m_Scanline;
	m4uint8_t *m_puiMonoMap ;
};

BMP_DWORD masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
					  0x001F, 0x003F, 0x007F, 0x00FF,
					  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
				      0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };


m4bool_t ClENG_RasterMap::SaveAsGIF(m4char_t *ai_pcFileName)
{
	m4int_t Resolution, ColorMapSize, InitCodeSize, BitsPerPixel;
	BMP_BYTE m_puiOutMap[256*4]; //Mapa de colores de salida
	m4int_t RWidth, RHeight, LeftOfs, TopOfs, B, i; ;
	m4int_t x, y, cnt;
	m4int_t Transparent=-1;
	m4bool_t SwOk=M4_TRUE;
	m4int_t Background=0;
	ClMIT_File File;

	GIF_CONVERSION_DATA stConvData;

	stConvData.m_iInterlace=0;
	stConvData.m_iPass=0;
	stConvData.m_iOutCount=0;
	stConvData.m_iInCount=1;
	stConvData.m_iClearFlag=0;
	stConvData.m_iFreeEntry=0;
	stConvData.m_iAcumCnt=0;
	stConvData.m_dwCurAcc=0;
	stConvData.m_iCurrentBits=0;
	stConvData.m_iLastY=1;
	stConvData.m_iLastPixel=0;
	stConvData.m_iMaxBits=GIF_BITS;
	stConvData.m_iHSize=GIF_HSIZE;
	stConvData.m_iMaxMaxCode=(m4int_t)1 << GIF_BITS;
	
	if(m_BitsPixel==MONOCHROME){
		RemapTo256Colors();
	};

	//Forma la paleta de salida comprimida
	if(m_BitsPixel==INDEXED_256) {
		stConvData.m_iSzLine=m_uiRasterX+3; 
		BitsPerPixel=8;
		stConvData.m_iSzLine = stConvData.m_iSzLine & ~3;

		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			stConvData.m_Scanline, m4uint8_t ,(m4int_t)stConvData.m_iSzLine ) ;
	
		for(x= 0; x<256; x++){
			stConvData.m_ColUsed[x]= 0;
		};
		cnt=(m4int_t)((m_uiRasterY-1)*(stConvData.m_iSzLine));
		for(y=0; y<(m4int_t)m_uiRasterY; y++){
			memcpy(stConvData.m_Scanline, m_puiByte+cnt,(m4int_t)stConvData.m_iSzLine);
			cnt-=stConvData.m_iSzLine;
			for(x= 0; x<(m4int_t)m_uiRasterX; x++) {
				stConvData.m_ColUsed[stConvData.m_Scanline[x]]= 1;
			};
		};

		for(x= y= 0; x<256; x++){
			if(stConvData.m_ColUsed[x]!=0){
				stConvData.m_ColUsed[x]= y/3;
				m_puiOutMap[y++]= m_oPalette.GetColourByIndex(x)->GetR();
				m_puiOutMap[y++]= m_oPalette.GetColourByIndex(x)->GetG();
				m_puiOutMap[y++]= m_oPalette.GetColourByIndex(x)->GetB();
			};
		};
	};
	//Fin de paleta salida

	//Abre fichero destino
	if(!File.Open(ai_pcFileName, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN, M4_FALSE)){
		SwOk=M4_FALSE;
		goto exit;
	};

	ColorMapSize = 1 << BitsPerPixel;
	RWidth = stConvData.m_iWidth = m_uiRasterX;
	RHeight = stConvData.m_iHeight = m_uiRasterY;
	LeftOfs = TopOfs = 0;
	Resolution=BitsPerPixel;

	//Calculo del numero de bits esperados
	stConvData.m_iCountDown = (m4int32_t)stConvData.m_iWidth * (m4int32_t)stConvData.m_iHeight;

	//Indica en que pasada estamos (si es Interlaced)
	stConvData.m_iPass=0;

	//Tamaño del codigo inicial
	if(BitsPerPixel <= 1){
		InitCodeSize = 2;
	} else {
		InitCodeSize = BitsPerPixel;
	};

	//Inicializa las posiciones actuales de X e Y
	stConvData.m_iCurrentX = stConvData.m_iCurrentY = 0;

	//Escribe la cabecera
	File.WStr(Transparent < 0 ? "GIF87a" : "GIF89a");

	//Altura y anchura
	File.WChar(RWidth & 0xff);
	File.WChar((RWidth/256) & 0xff);
	File.WChar(RHeight & 0xff);
	File.WChar((RHeight/256) & 0xff);

	//Indica que existe un mapa de colores global
	// OR en la resolucion y en los Bits por Pixel
	B = 0x80;	
	B |= (Resolution - 1) << 5;
	B |= (BitsPerPixel - 1);
	File.WChar(B);

	//Escribe el color de fondo
	File.WChar(Background);

	//Byte de 0's 
	File.WChar(0);

	//Escribe el mapa global de colores
	for( i=0, x=0; i<ColorMapSize; i++ ) {
		File.WChar(m_puiOutMap[x]);	// Red
		File.WChar(m_puiOutMap[x+1]);	// Green
		File.WChar(m_puiOutMap[x+2]);	// Blue
		x+=3;
	}
	
	//Escribe la extension para el indice del color transparente (si es necesario)
	if ( Transparent >= 0 ) {
		File.WChar(GIF_BLOCK_START);
		File.WChar(GIF_TRANSPARENT_ID);
		File.WChar(4);
		File.WChar(1);
		File.WChar(0);
		File.WChar(0);
		File.WChar(Transparent);
		File.WChar(0);
	};

	//Separador de imagen
	File.WChar(GIF_IMAGE_SEPARATOR);

	//Cabecera de la imagen
	File.WChar(LeftOfs & 0xff);
	File.WChar((LeftOfs/256) & 0xff);
	File.WChar(TopOfs & 0xff);
	File.WChar((TopOfs/256) & 0xff);
	File.WChar(stConvData.m_iWidth & 0xff);
	File.WChar((stConvData.m_iWidth/256) & 0xff);
	File.WChar(stConvData.m_iHeight & 0xff);
	File.WChar((stConvData.m_iHeight/256) & 0xff);
	
	//Escribe el switch que indica si el GIF es Interlaced
	if(stConvData.m_iInterlace){
		File.WChar(INTERLACED_GIF);
	} else {
		File.WChar(NOT_INTERLACED_GIF);
	};

	//Escribe el tamaño del codigo inicial
	File.WChar(InitCodeSize);

	//Compresion de datos
	Compress(InitCodeSize+1, &stConvData, &File);

	//Escribe un paquete de longitud cero (al final de la serie de bytes)
	File.WChar(0);

	//Fin del GIF
	File.WChar(GIF_TERMINATOR);

exit:

	//Close the file
	File.Close();

	return SwOk;
}


void ClENG_RasterMap::Compress(m4int_t ai_iInitBits, GIF_CONVERSION_DATA* ai_stConvData, ClMIT_File *ai_poFile){
    register m4int32_t iFCode;
    register m4int_t i;
    register m4int_t c;
    register m4int_t ent;
    register m4int_t disp;
    register m4int_t hsize_reg;
    register m4int_t hshift;

    ai_stConvData->m_iInitBits = ai_iInitBits;
    ai_stConvData->m_iOutCount = 0;
    ai_stConvData->m_iClearFlag = 0;
    ai_stConvData->m_iInCount = 1;
    ai_stConvData->m_iMaxCode = MAXCODE(ai_stConvData->m_iNBits=ai_stConvData->m_iInitBits);
    ai_stConvData->m_iClearCode = (1 << (ai_iInitBits - 1));
    ai_stConvData->m_iEOFCode = ai_stConvData->m_iClearCode + 1;
    ai_stConvData->m_iFreeEntry = ai_stConvData->m_iClearCode + 2;
    ai_stConvData->m_iAcumCnt = 0;

    ent = GIFNextPixel(ai_stConvData);

    hshift = 0;
    for(iFCode=(m4int32_t)ai_stConvData->m_iHSize; iFCode<65536L; iFCode*=2L){
		++hshift;
	};
    hshift = 8 - hshift;		/* set hash code range bound */

    hsize_reg = ai_stConvData->m_iHSize;
    Hash((m4int_t)hsize_reg, ai_stConvData);		/* clear hash table */

    Output((m4int_t)ai_stConvData->m_iClearCode, ai_stConvData, ai_poFile);

    while ((c=GIFNextPixel(ai_stConvData))!=EOF) {

		++ai_stConvData->m_iInCount;
		iFCode = (m4int32_t) (((m4int32_t) c << ai_stConvData->m_iMaxBits) + ent);
		i = (((m4int_t)c << hshift) ^ ent);	/* xor hashing */

		if (ai_stConvData->m_piHTable[i]==iFCode) {
			ent = ai_stConvData->m_wCodeTab[i];
			continue;
		} else if ((m4int32_t)ai_stConvData->m_piHTable[i] < 0) {	   /* empty slot */
			goto nomatch;
		};
	
		disp = hsize_reg - i;		/* secondary hash (after G. Knott) */
		
		if ( i == 0 ) {
			disp = 1;
		};

probe:
		if ( (i -= disp) < 0 ) {
			i += hsize_reg;
		};
		if (ai_stConvData->m_piHTable[i]==iFCode) {
			ent = ai_stConvData->m_wCodeTab[i];
			continue;
		};
		if ((m4int32_t)ai_stConvData->m_piHTable[i] > 0) {
			goto probe;
		};
	
nomatch:
		Output ((m4int_t)ent, ai_stConvData, ai_poFile);
		++ai_stConvData->m_iOutCount;
		ent = c;
		if (ai_stConvData->m_iFreeEntry < ai_stConvData->m_iMaxMaxCode) {	
			ai_stConvData->m_wCodeTab[i]=ai_stConvData->m_iFreeEntry++; /* code -> hashtable */
			ai_stConvData->m_piHTable[i]=iFCode;
		} else {
			Hash((m4int_t) ai_stConvData->m_iHSize, ai_stConvData);
			ai_stConvData->m_iFreeEntry = ai_stConvData->m_iClearCode + 2;
			ai_stConvData->m_iClearFlag = 1;
			Output((m4int_t)ai_stConvData->m_iClearCode, ai_stConvData, ai_poFile);
		};
    };
    
	// Put out the final code.
    Output((m4int_t)ent, ai_stConvData, ai_poFile);
    ++ai_stConvData->m_iOutCount;
    Output((m4int_t)ai_stConvData->m_iEOFCode, ai_stConvData, ai_poFile);
}

void ClENG_RasterMap::Output(m4int_t ai_iCode, GIF_CONVERSION_DATA* ai_stConvData, ClMIT_File* ai_poFile)
{
    ai_stConvData->m_dwCurAcc &= masks[ai_stConvData->m_iCurrentBits];

    if(ai_stConvData->m_iCurrentBits > 0) {
		ai_stConvData->m_dwCurAcc |= ((m4int32_t)ai_iCode << ai_stConvData->m_iCurrentBits);
    } else {
		ai_stConvData->m_dwCurAcc = ai_iCode;
	};

    ai_stConvData->m_iCurrentBits += ai_stConvData->m_iNBits;

    while(ai_stConvData->m_iCurrentBits >= 8) {
		ai_stConvData->m_pcAccum[ai_stConvData->m_iAcumCnt++]=(unsigned int)(ai_stConvData->m_dwCurAcc & 0xff);
		if(ai_stConvData->m_iAcumCnt >= 254) {
			if(ai_stConvData->m_iAcumCnt > 0) {
				ai_poFile->WChar(ai_stConvData->m_iAcumCnt);
				ai_poFile->W(ai_stConvData->m_pcAccum, ai_stConvData->m_iAcumCnt);
				ai_stConvData->m_iAcumCnt = 0;
			};
		};
		ai_stConvData->m_dwCurAcc >>= 8;
		ai_stConvData->m_iCurrentBits -= 8;
    };

    
	if (ai_stConvData->m_iFreeEntry>ai_stConvData->m_iMaxCode || ai_stConvData->m_iClearFlag) {

	    if(ai_stConvData->m_iClearFlag) {
			ai_stConvData->m_iMaxCode=MAXCODE(ai_stConvData->m_iNBits = ai_stConvData->m_iInitBits);
			ai_stConvData->m_iClearFlag=0;
	    } else {
			++ai_stConvData->m_iNBits;
			if(ai_stConvData->m_iNBits == ai_stConvData->m_iMaxBits) {
			    ai_stConvData->m_iMaxCode=ai_stConvData->m_iMaxMaxCode;
			} else {
				ai_stConvData->m_iMaxCode=MAXCODE(ai_stConvData->m_iNBits);
			};
	    }
	}

	if(ai_iCode==ai_stConvData->m_iEOFCode) {
	
		//At EOF, write the rest of the buffer.
		while(ai_stConvData->m_iCurrentBits > 0) {
			ai_stConvData->m_pcAccum[ai_stConvData->m_iAcumCnt++]=(unsigned int)(ai_stConvData->m_dwCurAcc & 0xff);
			if(ai_stConvData->m_iAcumCnt >= 254) {
				if(ai_stConvData->m_iAcumCnt > 0) {
					ai_poFile->WChar(ai_stConvData->m_iAcumCnt);
					ai_poFile->W(ai_stConvData->m_pcAccum, ai_stConvData->m_iAcumCnt);
					ai_stConvData->m_iAcumCnt = 0;
				};
			};
			ai_stConvData->m_dwCurAcc >>= 8;
			ai_stConvData->m_iCurrentBits -= 8;
		};

		if(ai_stConvData->m_iAcumCnt > 0 ) {
			ai_poFile->WChar(ai_stConvData->m_iAcumCnt);
			ai_poFile->W(ai_stConvData->m_pcAccum, ai_stConvData->m_iAcumCnt);
			ai_stConvData->m_iAcumCnt = 0;
		}
		ai_poFile->Flush();
    };
}

void ClENG_RasterMap::Hash(register m4int_t m_iHSize, GIF_CONVERSION_DATA* ai_stConvData)
{
	register m4int_t *htab_p = ai_stConvData->m_piHTable+ai_stConvData->m_iHSize;
	register m4int32_t i;
	register m4int32_t m1 = -1;

	i = ai_stConvData->m_iHSize - 16;
	do {				/* might use Sys V memset(3) here */
		*(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for ( i += 16; i > 0; --i )
		*--htab_p = m1;
}

m4int_t ClENG_RasterMap::GIFNextPixel(GIF_CONVERSION_DATA* ai_stConvData)
{
	m4int_t r;

	if(ai_stConvData->m_iCountDown==0) {
		return EOF;
	};
	--ai_stConvData->m_iCountDown;

	if(ai_stConvData->m_iCurrentY!=ai_stConvData->m_iLastY){
		memcpy(ai_stConvData->m_Scanline, m_puiByte+ai_stConvData->m_iLastPixel, (m4int_t)ai_stConvData->m_iSzLine);
		ai_stConvData->m_iLastPixel+=(m4int_t)ai_stConvData->m_iSzLine;
		ai_stConvData->m_iLastY= ai_stConvData->m_iCurrentY;
	};

	r = ai_stConvData->m_ColUsed[ai_stConvData->m_Scanline[ai_stConvData->m_iCurrentX]];
	
	//Bump the current X position
	++ai_stConvData->m_iCurrentX;

	// If we are at the end of a scan line, set m_iCurrentX back to the beginning
	// If we are interlaced, bump the m_iCurrentY to the appropriate spot,
	//  otherwise, just increment it.
	if(ai_stConvData->m_iCurrentX == ai_stConvData->m_iWidth) {
		ai_stConvData->m_iCurrentX = 0;
		if(!ai_stConvData->m_iInterlace) {
			++ai_stConvData->m_iCurrentY;
		} else {
		     switch(ai_stConvData->m_iPass) {
				case 0:
					ai_stConvData->m_iCurrentY += 8;
					if(ai_stConvData->m_iCurrentY >= ai_stConvData->m_iHeight) {
						++ai_stConvData->m_iPass;
						ai_stConvData->m_iCurrentY = 4;
					}; 
					break;

				case 1:
					ai_stConvData->m_iCurrentY += 8;
					if(ai_stConvData->m_iCurrentY >= ai_stConvData->m_iHeight) {
						++ai_stConvData->m_iPass;
						ai_stConvData->m_iCurrentY = 2;
					};
					break;
		       
				case 2:
					ai_stConvData->m_iCurrentY += 4;
					if(ai_stConvData->m_iCurrentY >= ai_stConvData->m_iHeight) {
						++ai_stConvData->m_iPass;
						ai_stConvData->m_iCurrentY=1;
					}; 
					break;
		       
				case 3:
					ai_stConvData->m_iCurrentY += 2;
					break;
			};
		};
	};

	return r;
}
//<------------ Fin salvar mapa como GIF


m4bool_t ClENG_RasterMap::RemapTo256Colors()
{
	BMP_BYTE *SBeginLine, *SProcessByte, *TBeginLine, *TProcessByte;
	m4int16_t x, y, PaletteIndex, CountLinePixels;
	m4int32_t SSkipLine, TSkipLine, iSzLine;
	m4bool_t SwOk=M4_TRUE;
	BMP_BYTE* m_puiNewMap;
	BMP_BYTE Mask;		

	if ( !IsMapLoaded() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if (m_BitsPixel==INDEXED_256) {
		goto exit ;
	} else if (m_BitsPixel==MONOCHROME){

		//----------------->Creo nuevo mapa destino

		if (!m_oPalette.Init(256,ClMIT_Trace::PROPAGATE_ERROR)){
			SwOk=M4_FALSE;
			goto exit ;
		};	
		for (PaletteIndex=0 ; PaletteIndex<256; ++PaletteIndex) {
			m_oPalette.SetColour( PaletteIndex, PaletteIndex, "", PaletteIndex, PaletteIndex, PaletteIndex );
		} ;
		
		iSzLine   = (m_uiRasterX+31) >> 3; 
		iSzLine	  = iSzLine & ~3;
		TSkipLine = (m_uiRasterX+3) & ~3;
			
		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			m_puiNewMap, m4uint8_t, TSkipLine*m_uiRasterY) ;
		
		SBeginLine   = m_puiByte;
		SSkipLine    = iSzLine ;
		SProcessByte = SBeginLine;

		TBeginLine   = m_puiNewMap;
		TProcessByte = TBeginLine;

		for(y=m_uiRasterY; y; y--, SProcessByte=SBeginLine+=SSkipLine, 
			TProcessByte=TBeginLine+=TSkipLine ) 
		{
			CountLinePixels=m_uiRasterX ;
			for(x=m_uiLastXIndex+1; x; --x, ++SProcessByte) {
				for(Mask=0x80; Mask && CountLinePixels ; 
				Mask=Mask>>1, ++TProcessByte, --CountLinePixels) 
				{
					*TProcessByte = (*SProcessByte & Mask) ? 0x00 : 0xFF ;
				};
			};
		};

		//----------------->Sustituyo mapa viejo por nuevo, borrando el viejo

		M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MEM_GRP_RASTERG, 
			m_puiByte, m4uint8_t ,m_uiRasterY*m_uiBytesRow ) ;

		iSzLine = m_uiRasterX+3; 
		iSzLine = iSzLine & ~3;

		m_BitsPixel	   = INDEXED_256;
		m_puiByte	   = m_puiNewMap;
		m_uiBytesRow   = iSzLine;			
		m_uiLastXIndex = m_uiBytesRow-1;

		//<-----------------
	};	

exit:
	return SwOk ;
}


m4bool_t ClENG_RasterMap::GrabYCbCrPixels(m4float_t *ao_Y, m4float_t *ao_Cb, m4float_t *ao_Cr,
		m4uint32_t ai_uiOffset, m4uint32_t ai_uiSkip,
		ClENG_Colour *ai_ForegroundColor, ClENG_Colour *ai_BackgroundColor) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4float_t *BegCy, *BegCb, *BegCr;
	m4uint32_t YCount, XCount ;
	m4uint8_t *SByte, *SBegByte ;
	ClENG_Colour *Color ;
	m4float_t r,g,b ;
	m4uint32_t CountLinePixels ;
	m4uint8_t  Mask ;

	if ( ! IsMapLoaded() ) {
		SwOk=M4_FALSE;
		goto exit ;
	} ;

	switch( GetBitsPixel() ) {

	case INDEXED_256:
		
		for( YCount=m_uiRasterY, 
			ao_Y=BegCy=ao_Y+ai_uiOffset, 
			ao_Cb=BegCb=ao_Cb+ai_uiOffset, 
			ao_Cr=BegCr=ao_Cr+ai_uiOffset,
			SByte=SBegByte=m_puiByte ;
			YCount; 
			--YCount, 
			ao_Y=BegCy+=ai_uiSkip, 
			ao_Cb=BegCb+=ai_uiSkip, 
			ao_Cr=BegCr+=ai_uiSkip,
			SByte=SBegByte+=m_uiBytesRow )
			//SByte = m_puiByte+(m_uiRasterY-YCount)*m_uiBytesRow+m_uiRasterY-YCount )
		{
			for (XCount=m_uiRasterX ; 
				XCount; 
				--XCount, ++ao_Y, ++ao_Cb, ++ao_Cr, ++SByte )
			{
				Color = m_oPalette.GetColourByIndex(*SByte) ;
				r = Color->GetR() ;	g = Color->GetG() ; b = Color->GetB() ;
				*ao_Y = (m4float_t)((0.299 * (m4float_t)r + 0.587 * (m4float_t)g + 0.114 * 
					(m4float_t)b)) ;
				*ao_Cb= 128 + (m4float_t)((-0.16874 * (m4float_t)r - 0.33126 * (float)g + 
					0.5 * (m4float_t)b));
				*ao_Cr=128 + (m4float_t)((0.5 * (m4float_t)r - 0.41869 * 
					(m4float_t)g - 0.08131 * (m4float_t)b)) ;
			} ;
		} ;


		break;

	case MONOCHROME:

		
		for( YCount=m_uiRasterY, 
			ao_Y=BegCy=ao_Y+ai_uiOffset, 
			ao_Cb=BegCb=ao_Cb+ai_uiOffset, 
			ao_Cr=BegCr=ao_Cr+ai_uiOffset,
			SByte=SBegByte=m_puiByte ;
			YCount; 
			--YCount, 
			ao_Y=BegCy+=ai_uiSkip, 
			ao_Cb=BegCb+=ai_uiSkip, 
			ao_Cr=BegCr+=ai_uiSkip,
			SByte=SBegByte+=m_uiBytesRow )
		{
			CountLinePixels=m_uiRasterX ;	//Recorrer pixels=recorrer bytes+recorrer bits
			for (XCount=m_uiLastXIndex+1 ; 
				XCount; 
				--XCount, ++SByte )
			{
				for(Mask=0x80; 
					Mask && CountLinePixels ; 
					Mask=Mask>>1, ++ao_Y, ++ao_Cb, ++ao_Cr, --CountLinePixels) 
				{
					Color = (*SByte & Mask) ? ai_ForegroundColor : ai_BackgroundColor ;
					r = Color->GetR() ;	g = Color->GetG() ; b = Color->GetB() ;
					*ao_Y = (m4float_t)((0.299 * (m4float_t)r + 0.587 * (m4float_t)g + 0.114 * 
						(m4float_t)b)) ;
					*ao_Cb= 128 + (m4float_t)((-0.16874 * (m4float_t)r - 0.33126 * (float)g + 
						0.5 * (m4float_t)b));
					*ao_Cr=128 + (m4float_t)((0.5 * (m4float_t)r - 0.41869 * 
						(m4float_t)g - 0.08131 * (m4float_t)b)) ;
				} ;
			} ;
		} ;
		
		
		break;

	default:
		SwOk=M4_FALSE ;
		break;
		
	} ;

exit:
	return SwOk ;
}

