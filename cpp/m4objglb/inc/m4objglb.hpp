//## begin module%3912A6BD0170.cm preserve=no
//## end module%3912A6BD0170.cm

//## begin module%3912A6BD0170.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A6BD0170.cp

//## Module: m4objglb%3912A6BD0170; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\m4objglb.hpp

#ifndef __M4OBJGLB_HPP__
#define __M4OBJGLB_HPP__ 1

//## begin module%3912A6BD0170.additionalIncludes preserve=no
//## end module%3912A6BD0170.additionalIncludes

//## begin module%3912A6BD0170.includes preserve=yes

#include "m4objglb_dll.hpp"

#include "m4types.hpp"
#include "cldefine.hpp"
#include "clextman.hpp"
//## end module%3912A6BD0170.includes

//## begin module%3912A6BD0170.declarations preserve=no
//## end module%3912A6BD0170.declarations

//## begin module%3912A6BD0170.additionalDeclarations preserve=yes
//## end module%3912A6BD0170.additionalDeclarations


//## begin module%3912A6BD0170.epilog preserve=yes


// Para que quien tire de global no tenga que incluir y linkar con M4Variant
class m4VariantType;	//	#include "m4variant.hpp"

//=============================================================================
// Declaraciones adelantadas
//=============================================================================


//=================================================================================
// Array de datos
//=================================================================================

extern	m4uint8_t   M4_DECL_M4OBJGLB	g_aiScopeArray[ M4CL_NODE_CONNECTOR_MAX_TYPE ][ M4CL_ITEM_MAX_SCOPE ][ M4CL_ITEM_MAX_SCOPE ] ;


//=============================================================================
// Para comparar cadenas con tamaño
//=============================================================================
inline int	M4MdFacStrcmp( m4pcchar_t ai_pccString1, m4pcchar_t ai_pccString2 )
{
	m4uchar_t uiSize1 ;
	m4uchar_t uiSize2 ;

	if( *ai_pccString1 == '\0' )
	{
		uiSize1 = 0 ;
	}
	else
	{
		uiSize1 = *( ai_pccString1 - 1 ) ;
	}

	if( *ai_pccString2 == '\0' )
	{
		uiSize2 = 0 ;
	}
	else
	{
		uiSize2 = *( ai_pccString2 - 1 ) ;
	}

	if( uiSize1 != uiSize2 )
	{
		return( uiSize1 - uiSize2 ) ;
	}

	return( memcmp( ai_pccString1, ai_pccString2, uiSize1 ) ) ;
}



//=================================================================================
// Funciones de búsqueda
//=================================================================================

m4pvoid_t	M4_DECL_M4OBJGLB	M4VMBSearch
							(
								const void *key,
								const void *base,
								size_t num,
								size_t width,
								int (*compare)(const void *, const void *)
							) ;


//=================================================================================
// Funciones de impresion
//=================================================================================

m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnectedNodeType			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClCppType						( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClM4Type						( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemScope					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClCsType						( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnCsType					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClOrganizationType			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClCsExeType					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClCacheable					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemType					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClLanguageType				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClNodeCapacity				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClNodeTemporality				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClNodeCompleteness			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClNodeAutoFilter				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemSliceBhrv				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemSliceSplit				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClBool						( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClIdTotalize					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemTransactionMode			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemModifyData				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemInternalType			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClItemConvertFunction			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClArgumentType				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnectedItemPrecedence		( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnectedItemSpin			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnectedItemRelation		( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClConnectedItemContext		( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClSecurity					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClFormType					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClAutoloadMode				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClSyncMode					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClOpenMode					( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClInstanceScope				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClInstanceCsAccess			( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClNodeDBReload				( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClInstanceOrganizationType	( m4uint8_t ai_iValue ) ;
m4pcchar_t  M4_DECL_M4OBJGLB	M4ClFilterBehaviourType			( m4uint8_t ai_iValue ) ;



//=============================
// Funciones de ENCRIPTACION
//=============================

// Función de conversión
m4return_t	M4_DECL_M4OBJGLB M4ConvertOneWayMD5	( m4pcchar_t ai_pccInString, m4pchar_t ao_pcOutString, m4uint16_t ai_iOutLength ) ;
m4return_t  M4_DECL_M4OBJGLB M4ConvertPassword	( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccPassword, m4pchar_t ai_pcPasswEncripted, m4int_t ai_iMaxLenOut ) ;
m4return_t  M4_DECL_M4OBJGLB M4ComparePassword	( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccPassword, m4pcchar_t ai_pccStoredPassword, m4double_t ai_dReserved, m4bool_t ai_bIs2FA, m4bool_t &ao_rbEqual, m4pchar_t &ao_rpcNewPassword, m4pchar_t &ao_rpcVerificationCode ) ;

//=============================
// Funciones de REDONDEO
//=============================

// Ajuste a punto gordo, para evitar las perdidas de precision del modf
inline m4double_t M4_DECL_M4OBJGLB M4VMDoubleAdjust(m4double_t ai_dValue)
{
	return ( ai_dValue + ( ( ai_dValue >= 0.0 ) ? M4_LN4_MATH_PRECISION_MODF : -M4_LN4_MATH_PRECISION_MODF ) );
}

// Redondeo de 'dValue' a 'iScale' decimales, con iScale<=8
m4double_t M4_DECL_M4OBJGLB M4VMRound(m4double_t ai_dValue, int ai_iScale=8);


//=============================
// Funciones VARIAS
//=============================

//Convertir de M4Type a CppType
m4uint8_t M4_DECL_M4OBJGLB ConvertM4TypeToCppType( m4uint8_t ai_iItemM4Type );



// Function that returns the extension of a file name
// In case it doesn't exists, returns a null string (not null ptr)
m4pcchar_t M4_DECL_M4OBJGLB M4ClGetFileExtension(m4pcchar_t ai_psFileName);

// Function that compare file names
int M4_DECL_M4OBJGLB M4ClFileNameComp(m4pcchar_t ai_psFileName1, m4pcchar_t ai_psFileName2);


// Function that resets the locale environment struct of the MSVCRT.dll ( or MSVCRTD.dll )
// Its implementation is filled only dor _WINDOWS
void	M4_DECL_M4OBJGLB	M4ResetLocale( void ) ;




// Para el tipo de operación de un item

m4uint32_t	M4_DECL_M4OBJGLB M4GetDBOperation( m4pcchar_t ai_pccItem ) ;




//## end module%3912A6BD0170.epilog


#endif
