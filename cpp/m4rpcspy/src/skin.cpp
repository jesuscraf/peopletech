
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              rpcspy
// File:                skin.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                20-10-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the wrapper class for the skin
//
//
//==============================================================================


#include "skin.hpp"



//=================================================================================
// Símbolos importados
//=================================================================================

extern "C"	int	g_iSkinInitialized ;




//=================================================================================
// Constantes
//=================================================================================

// Identificador del skin
static	const LPCTSTR	s_pcSkin = "m4comctl.m4skin" ;

// Identificador de la propiedad de identificación del skin
static	const OLECHAR 	s_pcSkinPeoplenet[] = L"M4_PEOPLENET_INIT" ;

// Identificador de la propiedad de identificación del skin
static	const DISPID	s_dSkinPeoplenet = 0x68030004 ;

// Identificador del método de arranque del skin
static	const DISPID	s_dSkinStart= 0x60030014 ;

// Objeto skin
static ClCSkin	g_oSkin ;





//=================================================================================
//
// ClCSkin
//
//=================================================================================

//=================================================================================
// Constructor
// Inicializa la variable de estado
//=================================================================================

ClCSkin::ClCSkin( void )
{

	HRESULT hr = S_OK ;
	BOOL	bResult = TRUE ;
	BSTR	sPeoplenet = NULL ;

	static const BYTE	abParams[] = VTS_PBSTR ;

	
	hr = CoInitialize( NULL ) ;

	if( hr != S_OK && hr != S_FALSE && hr != RPC_E_CHANGED_MODE )
	{
		return ;
	}

	bResult = CreateDispatch( s_pcSkin ) ;

	if( bResult == FALSE )
	{
		return ;
	}


	sPeoplenet = SysAllocString( s_pcSkinPeoplenet ) ;

	if( sPeoplenet == NULL )
	{
		ReleaseDispatch() ;
		return ;
	}

	InvokeHelper( s_dSkinPeoplenet, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, abParams, &sPeoplenet ) ;
	SysFreeString( sPeoplenet ) ;

	InvokeHelper( s_dSkinStart, DISPATCH_METHOD, VT_BOOL, (void*)&bResult, NULL ) ;

	if( bResult == FALSE )
	{
		ReleaseDispatch() ;
		return ;
	}

	g_iSkinInitialized = true ;
}


