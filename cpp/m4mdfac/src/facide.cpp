
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                facide.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para los identificadores
//
//
//==============================================================================


#include "facide.hpp"
#include "m4objglb.hpp"


// Funciones de comparacion ===================================================

int	M4ClCompareObjectsByNumber( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 )
{
	return( ( *( ClNumericId** )ai_pcvObject1 )->GetIdentifier() - ( *( ClNumericId** )ai_pcvObject2 )->GetIdentifier() ) ;
}


int	M4ClCompareObjectsById( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 )
{
	return( M4MdFacStrcmp( ( *( ClIdentifier** )ai_pcvObject1 )->GetIdentifier(), ( *( ClIdentifier** )ai_pcvObject2 )->GetIdentifier() ) ) ;
}


int	M4ClCompareObjectsByTwoIds( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTwoIdentifier** )ai_pcvObject1 )->GetParentIdentifier(), ( *( ClTwoIdentifier** )ai_pcvObject2 )->GetParentIdentifier() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( ( *( ClTwoIdentifier** )ai_pcvObject1 )->GetIdentifier(), ( *( ClTwoIdentifier** )ai_pcvObject2 )->GetIdentifier() ) ) ;
}
