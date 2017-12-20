//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clmetric.cpp
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

#include "mit.h"
#include "clmetric.h"
#include <math.h>

m4int32_t ClMetric::RoundValue( m4double_t ai_dValue )
{
	m4double_t rest;

	modf(ai_dValue,&rest);

	if (fabs(ai_dValue-rest)>0.5)
	{
		if (ai_dValue>0)
			return (m4int32_t) ceil(ai_dValue) ;
		else
			return (m4int32_t) floor(ai_dValue) ;
	}
	else
	{
		if (ai_dValue>0)
			return (m4int32_t) floor(ai_dValue) ;
		else
			return (m4int32_t) ceil(ai_dValue) ;
	}
}



RoundMilim_t   ClMetric::DvcToRoundMilim(  DvcUnit_t ai_Dvc) 
{
	return ClMetric::RoundValue(ai_Dvc/720.0) ;
}


RoundInch_t   ClMetric::DvcToRoundInch(  DvcUnit_t ai_Dvc) 
{
	return ClMetric::RoundValue(ai_Dvc/(254.0*72.0)) ;
}


RoundPoint_t  ClMetric::DvcToRoundPoint( DvcUnit_t ai_Dvc) 
{
	return ClMetric::RoundValue(ai_Dvc/254.0) ;
}


RoundDPoint_t ClMetric::DvcToRoundDPoint(DvcUnit_t ai_Dvc) 
{
	return ClMetric::RoundValue(ai_Dvc/25.4) ;
}

RoundTwip_t ClMetric::DvcToRoundTwip(DvcUnit_t ai_Dvc)
{
	return ClMetric::RoundValue(ai_Dvc*20.0/254.0) ;
}

RoundPLU_t ClMetric::DvcToRoundPLU(DvcUnit_t ai_Dvc) 
{
	return ClMetric::RoundValue(ai_Dvc/18.0) ;
}
