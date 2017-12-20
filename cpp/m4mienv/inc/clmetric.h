//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clmetric.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/24/97
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
#ifndef __CLMETRIC__H__
#define __CLMETRIC__H__


/*
	Toda nuestra aplicacion trabaja con una unidad de medida llamada DvcUnit. Lo realmente
importante de esta medida no es su valor en si, sino :
	-Que sea una medida independiente del dispositivo, para que el sistema de coordenadas
		que usemos sea valido en cualquier contexto.
	-Que no sea de tipo float.
	-Que tenga signo, para poder usar offsets positivos y negativos.
	-Que sea una unidad más pequeña que cualquiera que nos den, para no perder precision
		en las conversiones.
  Tambien es importante en cuellos de botella, a la hora de hacer conversiones de 
medidas de DvcUnits a lo que use el device concreto, no trabajar con floats, porque esta 
aritmetica siempre es infinitamente más lenta que la aritmetica entera.

1 inch -  25.4 milim
1 inch -  72 ptos
1 twip - 1/20 point

1 milim - 720 Dvc units

*/

typedef m4int32_t DvcUnit_t ;  //Unidad de coordenada: 1 DvcUnit son 0.01 milimetros.

typedef m4double_t Inch_t ;
typedef m4double_t Point_t ;
typedef m4double_t DPoint_t ;
typedef m4double_t Milim_t ;
typedef m4double_t Twip_t ;
typedef m4double_t PLU_t ;

typedef m4int32_t RoundInch_t ;
typedef m4int32_t RoundPoint_t ;
typedef m4int32_t RoundDPoint_t ;
typedef m4int32_t RoundMilim_t ;
typedef m4int32_t RoundTwip_t ;
typedef m4int32_t RoundPLU_t ;

typedef m4double_t CPI_t ; 

class M4_DECL_M4MIENV ClMetric  
{
MIT_PUBLIC:
	ClMetric() {}
	virtual ~ClMetric() {}
	virtual size_t GetSizeof() { return sizeof(ClMetric) ; }

	//-------Milim

	static m4int32_t RoundValue( m4double_t ai_dValue ) ;

	static DvcUnit_t MilimToDvc(  Milim_t   ai_Milim) 
		{ return (DvcUnit_t) (ai_Milim*720) ;}

	static Milim_t   DvcToMilim(  DvcUnit_t ai_Dvc) 
		{return ai_Dvc/720.0 ;}

	static RoundMilim_t   DvcToRoundMilim(  DvcUnit_t ai_Dvc) ;
	

	//-------Inch

	static DvcUnit_t InchToDvc(   Inch_t   ai_Inch) 
		{return (DvcUnit_t ) (ai_Inch*254*72) ;}

	static Inch_t   DvcToInch(  DvcUnit_t ai_Dvc) 
		{return ai_Dvc/(72.0*254.0) ;}

	static RoundInch_t   DvcToRoundInch(  DvcUnit_t ai_Dvc) ;
	

	//------Point

	static DvcUnit_t PointToDvc(  Point_t  ai_Point) 
		{return (DvcUnit_t )(254*ai_Point) ;}

	static Point_t  DvcToPoint( DvcUnit_t ai_Dvc) 
		{return ai_Dvc/254.0 ;}

	static RoundPoint_t  DvcToRoundPoint( DvcUnit_t ai_Dvc) ;
	

	//------DPoint

	static DvcUnit_t DPointToDvc( DPoint_t ai_Point) 
		{return (DvcUnit_t )(25.4*ai_Point) ;}

	static DPoint_t DvcToDPoint(DvcUnit_t ai_Dvc) 
		{return ai_Dvc/25.4 ;}

	static RoundDPoint_t DvcToRoundDPoint(DvcUnit_t ai_Dvc) ;
	

	//------PLU (plotter unit, para GL/PCL) 

	static DvcUnit_t PLU_ToDvc( PLU_t ai_PLU) 
		{return (DvcUnit_t)(18.0*ai_PLU) ;}

	static PLU_t DvcToPLU(DvcUnit_t ai_Dvc) 
		{return ai_Dvc/18.0 ;}

	static RoundPLU_t DvcToRoundPLU(DvcUnit_t ai_Dvc) ;


	//--------CPI

	static DvcUnit_t CPIToDvcWidthChar( CPI_t ai_CPI )
		{ return (DvcUnit_t)  ((254*72)/ai_CPI) ; } 

	static CPI_t DvcToCPIWidthChar( DvcUnit_t ai_Dvc )
		{ return (254*72.0)/ai_Dvc; } 


	//--------TWIPS (1 Twip = (1/20) Point)

	static DvcUnit_t TwipToDvc( Twip_t ai_Twip )
		{ return (DvcUnit_t )((ai_Twip*254)/20) ; }

	static Twip_t DvcToTwip( DvcUnit_t ai_Dvc )
		{ return (ai_Dvc*20.0)/254.0 ; }

	static RoundTwip_t DvcToRoundTwip( DvcUnit_t ai_Dvc ) ;

	static Point_t TwipToPoint(Twip_t ai_Twip) { return ai_Twip/20 ; } 
	static Twip_t  PointToTwip(Point_t ai_Point) { return ai_Point*20 ; } 
};

#endif



