
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facacc.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions of the class of the factory of
//    compiled metachannels related to the access
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"
#include "m4facdef.hpp"

#include "m4var.hpp"
#include "cltitem.hpp"
#include "cltitinh.hpp"
#include "cltti.hpp"
#include "cltchann.hpp"
#include "cltnode.hpp"
#include "cltndinh.hpp"
#include "cltsent.hpp"
#include "cltiform.hpp"
#include "cltarg.hpp"
#include "clttrig.hpp"
#include "cltconn.hpp"
#include "cltnicon.hpp"
#include "cltiplug.hpp"
#include "cltfilt.hpp"
#include "cltindit.hpp"
#include "cltt3inh.hpp"
#include "clttiinh.hpp"
#include "cltrsm.hpp"
#include "clttotal.hpp"
#include "clttotaltag.hpp"
#include "clttag.hpp"
#include "cltinst.hpp"
#include "cltbarg.hpp"
#include "cltbmeth.hpp"
#include "cltcpt.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "register.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "m4lang.hpp"
#include "cllgadap.hpp"
#include "m4objreg.hpp"

extern int	M4IsUnicode( void ) ;




// ============================================================================
// Importaciones
// ============================================================================

extern	m4int8_t   g_aiReducedNodeCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] ;
extern	m4int8_t   g_aiReducedItemCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] ;
extern	m4int8_t   g_aiReducedConnCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] ;



// ============================================================================
// Defines
// ============================================================================

#define	M4MDFAC_MAX_T3_INHERITANCE				200

#define	M4MDFAC_MAX_INT_8						127				// ( 2 exp 7 ) - 1
#define	M4MDFAC_MAX_INT_16						32767			// ( 2 exp 15 ) - 1
#define	M4MDFAC_MAX_INT_32						2147483647		// ( 2 exp 31 ) - 1
#define	M4MDFAC_MAX_REGISTERS					536870911		// ( 2 exp 29 ) - 1




// ============================================================================
// Arrays
// ============================================================================

// Para la reducción de índices de idiomas
// Si se mete un nuevo idioma o se cambia el defecto hay que cambiarlo

m4uint8_t   g_aiReducedLanguagesArray[ M4CL_LANGUAGE_NUMBER ] =
{
	0, 0, 0, 1, 2, 3, 4, 5, 6
} ;





// ============================================================================
// Funciones globales
// ============================================================================

// Lectura de valores conociendo el tempchannel

m4return_t	M4ReadNumber( ClTempChannel *ai_poChannel, m4double_t &ao_rdValue, m4double_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadNumber( ClTempChannel *ai_poChannel, m4double_t &ao_rdValue, m4double_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister, m4double_t ai_dMinimum, m4double_t ai_dMaximum )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;

// sólo si hemos leido valor

		if( ao_rdValue < ai_dMinimum )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MIM_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MIM_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << ai_dMinimum << ai_dDefault << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}

		if( ao_rdValue > ai_dMaximum )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MAX_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << ai_dMaximum << ai_dDefault << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadString( ClTempChannel *ai_poChannel, m4pcchar_t &ao_rpccValue, m4pcchar_t ai_pccDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rpccValue = ai_pccDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rpccValue = vValue.Data.PointerChar ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadDate( ClTempChannel *ai_poChannel, m4date_t &ao_rdValue, m4date_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4char_t		acDefault[ M4_SECOND_END + 1 ] ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_DATE )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;

// sólo si hemos leido valor

		if( ao_rdValue < M4CL_MINUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MIM_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				ClDateToString( ai_dDefault, acDefault, M4_SECOND_END + 1 ) ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MIM_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_MINUS_INFINITE_DATE << acDefault << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}

		if( ao_rdValue > M4CL_PLUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MAX_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				ClDateToString( ai_dDefault, acDefault, M4_SECOND_END + 1 ) ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_PLUS_INFINITE_DATE << acDefault << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadVersion( ClTempChannel *ao_poChannel, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
	m4date_t		dVersion ;
	m4date_t		dCreationDate ;
    m4char_t		acVersion     [ M4_SECOND_END + 1 ] ;
    m4char_t		acCreationDate[ M4_SECOND_END + 1 ] ;
	ClCompiledMCR	*poCMCR ;


	
	iResult = M4ReadDate( ao_poChannel, dVersion, M4CL_MINUS_INFINITE_IN_JULIAN, ai_iIndex, ai_pccItem, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	/* Mejora 0077892
	Hay que controlar que la fecha no sea posterior
	*/
	dCreationDate = ao_poChannel->GetCreationDate() ;

	if( dVersion > dCreationDate )
	{
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

		ClDateToString( dVersion,		acVersion,		M4_SECOND_END + 1 ) ;
		ClDateToString( dCreationDate,	acCreationDate,	M4_SECOND_END + 1 ) ;

		DUMP_CHLOG_WARNINGF( M4_MDFAC_FUTURE_DATE, acVersion << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << acCreationDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
	}


	ao_poChannel->SetVersion( dVersion ) ;

	return( M4_SUCCESS ) ;
}





//=============================================================================
// Lectura del access =========================================================
//=============================================================================

m4return_t	ClFactoryImp :: _ReadOneT3FromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister )
{

    m4return_t	iResult ;

	m4uint8_t	iCacheable ;
	m4pcchar_t	pccChannelId ;
	m4pcchar_t	pccService ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4date_t	dValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccChannelId, "", M4MDFAC_T3S_ID_T3, M4MDFAC_ITEM_ID_T3, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poChannel->SetChannelId( pccChannelId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_T3S_N_T3, M4MDFAC_ITEM_N_T3, ai_poRegister, M4MDFAC_NODE_SYS_T3S_TRANS, M4MDFAC_T3S_TRANS_N_T3, pccChannelId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poChannel->SetName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3S_OLE_CLASS_ID, M4MDFAC_ITEM_OLE_CLASS_ID, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poChannel->SetOleClassId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CACHEABLE_FALSE, M4MDFAC_T3S_IS_CACHEABLE, M4MDFAC_ITEM_IS_CACHEABLE, ai_poRegister, M4CL_CACHEABLE_FALSE, M4CL_CACHEABLE_TRUE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iCacheable = M4CL_CACHEABLE_FALSE ;
	}
	else
	{
		iCacheable = M4CL_CACHEABLE_TRUE ;
	}

    ao_poChannel->SetIsCacheable( iCacheable ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CACHEABLE_TRUE, M4MDFAC_T3S_IS_MD_CACHEABLE, M4MDFAC_ITEM_IS_MD_CACHEABLE, ai_poRegister, M4CL_CACHEABLE_FALSE, M4CL_CACHEABLE_VOLATILE ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( fValue == 1.0 )
	{
		iValue = M4CL_CACHEABLE_TRUE ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_CACHEABLE_VOLATILE ;
	}
	else
	{
		if( iCacheable == 1 )
		{
			// Si es cacheable de datos lo debe ser de metadatos
			iValue = M4CL_CACHEABLE_TRUE ;
		}
		else
		{
			iValue = M4CL_CACHEABLE_FALSE ;
		}
	}

    ao_poChannel->SetIsMDCacheable( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_CACHE_MAX_PERIOD, M4MDFAC_ITEM_CACHE_MAX_PERIOD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetCacheMaxPeriod( m4date_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSEXETYPE_OLTP, M4MDFAC_T3S_CS_EXE_TYPE, M4MDFAC_ITEM_CS_EXE_TYPE, ai_poRegister, M4CL_CSEXETYPE_OLTP, M4CL_CSEXETYPE_AUTO ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( fValue == 0.0 )
	{
		iValue = M4CL_CSEXETYPE_OLTP ;
		pccService = M4CL_EXECUTION_SERVICE ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_CSEXETYPE_PROXY ;
		pccService = M4CL_PROXY_SERVICE ;

		/* Bug 0106504
		Si es proxy se deja como proxy pero se quita de cacheable
		*/
	    ao_poChannel->SetIsCacheable( 0 ) ;
	}
	else
	{
		/* Bug 0105344 y 0105394
		Si es cacheable de datos no puede ser delta
		*/
		if( iCacheable == 1 )
		{
			iValue = M4CL_CSEXETYPE_OLTP ;
		}
		else
		{
			iValue = M4CL_CSEXETYPE_AUTO ;
		}
		pccService = M4CL_EXECUTION_SERVICE ;
	}

    ao_poChannel->SetCsExeType( iValue ) ;

	// El servicio siempre debe coincidir con el tipo
    iResult = ao_poChannel->SetServiceId( pccService ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = ao_poChannel->SetServiceAltId( pccService ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CHANNEL_TYPE_NORMAL, M4MDFAC_T3S_ID_STREAM_TYPE, M4MDFAC_ITEM_ID_STREAM_TYPE, ai_poRegister, M4CL_CHANNEL_TYPE_NORMAL, M4CL_CHANNEL_TYPE_APPLY_VALUE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 1, M4MDFAC_T3S_IS_SEPARABLE, M4MDFAC_ITEM_IS_SEPARABLE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else
	{
		iValue = 0 ;
	}

    ao_poChannel->SetIsSeparable( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_IS_EXTERNAL, M4MDFAC_ITEM_IS_EXTERNAL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poChannel->SetIsExternal( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_VM_CAPACITY_REQ, M4MDFAC_ITEM_VM_CAPACITY_REQ, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetVMCapacityReq( m4uint16_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_HAS_SECURITY, M4MDFAC_ITEM_HAS_SECURITY, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poChannel->SetHasSecurity( iValue ) ;



// Por defecto es que no, como antes, un 0
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_CHECK_CONCURRENCY, M4MDFAC_ITEM_CHECK_CONCURRENCY, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poChannel->SetCheckConcurrency( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ORGANIZATION_TYPE_MONO, M4MDFAC_T3S_ID_ORGANIZATION_TYPE, M4MDFAC_ITEM_ORGANIZATION_TYPE, ai_poRegister, M4CL_ORGANIZATION_TYPE_NONE, M4CL_ORGANIZATION_TYPE_MULTI ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_ORGANIZATION_TYPE_MULTI ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_ORGANIZATION_TYPE_MONO ;
	}
	else
	{
		iValue = M4CL_ORGANIZATION_TYPE_NONE ;
	}

    ao_poChannel->SetOrganizationType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_LANGUAGE_TYPE_LOGON, M4MDFAC_T3S_DATA_LANGUAGE, M4MDFAC_ITEM_DATA_LANGUAGE, ai_poRegister, M4CL_LANGUAGE_TYPE_LOGON, M4CL_LANGUAGE_TYPE_DEVELOPMENT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_LANGUAGE_TYPE_LOGON ;
	}
	else
	{
		iValue = M4CL_LANGUAGE_TYPE_DEVELOPMENT ;
	}

    ao_poChannel->SetDataLanguage( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_LANGUAGE_TYPE_LOGON, M4MDFAC_T3S_META_LANGUAGE, M4MDFAC_ITEM_META_LANGUAGE, ai_poRegister, M4CL_LANGUAGE_TYPE_LOGON, M4CL_LANGUAGE_TYPE_DEVELOPMENT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_LANGUAGE_TYPE_LOGON ;
	}
	else
	{
		iValue = M4CL_LANGUAGE_TYPE_DEVELOPMENT ;
	}

    ao_poChannel->SetMetaLanguage( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CHANNEL_CREATION_TYPE_NONE, M4MDFAC_T3S_CREATION_TYPE, M4MDFAC_ITEM_CREATION_TYPE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetCreationType( m4uint32_t( fValue ) ) ;



	iResult = M4ReadDate( ao_poChannel, dValue, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_T3S_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetUserStartDate( dValue ) ;



	iResult = M4ReadDate( ao_poChannel, dValue, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_T3S_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetUserEndDate( dValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_ENVIRONMENT_VARS, M4MDFAC_ITEM_ENVIRONMENT_VARS, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetEnvironmentVars( m4uint32_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3S_OWNER_FLAG, M4MDFAC_ITEM_OWNER_FLAG, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poChannel->SetOwnerFlag( m4uint32_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_T3S_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneT3InheritFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempT3Inherit *ao_poT3Inherit )
{

    m4return_t	iResult ;

	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_INHERIT_ID_T3_BASE, M4MDFAC_ITEM_ID_T3_BASE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poT3Inherit->SetBaseT3Id( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3_INHERIT_ID_LEVEL, M4MDFAC_ITEM_MAX_NUM_COL, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poT3Inherit->SetLevel( m4uint8_t( fValue ) ) ;


	
	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_T3_INHERIT_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneSentenceFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempSentence *ao_poSentence )
{

    m4return_t	iResult ;

	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_SENTENCES_ID_SENTENCE, M4MDFAC_ITEM_ID_SENTENCE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poSentence->SetSentenceId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_SENTENCES_ID_GROUP_OBJECTS, M4MDFAC_ITEM_ID_GROUP_OBJECTS, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poSentence->SetGroupObjects( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_SENTENCES_APISQL, M4MDFAC_ITEM_APISQL, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poSentence->SetApiSql1( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_SENTENCES_MAX_NUM_COL, M4MDFAC_ITEM_MAX_NUM_COL, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poSentence->SetMaxNumCol( m4uint32_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_SENTENCES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneRSMFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempRSM *ao_poRSM )
{

    m4return_t	iResult ;

	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RSMS_ID_RSM, M4MDFAC_ITEM_ID_RSM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poRSM->SetRSMId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RSMS_ID_MASK, M4MDFAC_ITEM_ID_MASK, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poRSM->SetMaskId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_RSMS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTiFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTi *ao_poTi, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t      iResult ;
    ClTempSentence  *poSentence ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;

    

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TIS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poTi->SetTiId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_TI_INHERIT_TYPE_ALL, M4MDFAC_TIS_ID_INHERIT_TYPE, M4MDFAC_ITEM_ID_INHERIT_TYPE, ai_poRegister, M4CL_TI_INHERIT_TYPE_NONE, M4CL_TI_INHERIT_TYPE_ALL ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTi->SetInheritanceType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_TIS_GENERATE_SQL, M4MDFAC_ITEM_GENERATE_SQL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

	ao_poTi->SetGenerateSql( iValue ) ;

	
	
	if( iValue == 0 )
	{
		iResult = M4ReadString( ao_poChannel, pccValue, NULL, M4MDFAC_TIS_ID_READ_SENTENCE, M4MDFAC_ITEM_ID_READ_SENTENCE, ai_poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( pccValue != NULL )
		{
			poSentence = ao_poChannel->GetSentenceById( pccValue, M4_TRUE ) ;
			CHECK_CHLOG_ERRORF( poSentence == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI_SENTENCE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << ao_poTi->GetTiId() ) ;
		}
		else
		{
			poSentence = NULL ;
		}
	}
	else
	{
        poSentence = new ClTempSentence ;
		CHECK_CHLOG_ERRORF( poSentence == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempSentence ) ) << __LINE__ << __FILE__ ) ;

		poSentence->SetUpperPool( ao_poUpperPool ) ;
		poSentence->SetNoUpperPool( ao_poNoUpperPool ) ;
	}

    ao_poTi->SetReadSentence( poSentence ) ;
    ao_poTi->SetWriteSentence( NULL ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TIS_ID_READ_OBJECT, M4MDFAC_ITEM_ID_READ_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poTi->SetReadObjectId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TIS_ID_WRITE_OBJECT, M4MDFAC_ITEM_ID_WRITE_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poTi->SetWriteObjectId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_TEMPORALITY_EVERGREEN, M4MDFAC_TIS_ID_TEMPORARY, M4MDFAC_ITEM_ID_TEMPORARY, ai_poRegister, M4CL_NODE_TEMPORALITY_EVERGREEN, M4CL_NODE_TEMPORALITY_N_P_WITH_OVERLAPED ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTi->SetTemporaryId( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_COMPLETE_NOT_COMPLETE, M4MDFAC_TIS_COMPLETE, M4MDFAC_ITEM_COMPLETE, ai_poRegister, M4CL_NODE_COMPLETE_NOT_COMPLETE, M4CL_NODE_COMPLETE_COMPLETE_NOT_NULL ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = M4CL_NODE_COMPLETE_NOT_COMPLETE ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_NODE_COMPLETE_COMPLETE_NULL ;
	}
	else
	{
		iValue = M4CL_NODE_COMPLETE_COMPLETE_NOT_NULL ;
	}

    ao_poTi->SetComplete( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_TIS_CORRECTED, M4MDFAC_ITEM_CORRECTED, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poTi->SetCorrected( iValue ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TIS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTiInheritFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTiInherit *ao_poTiInherit )
{

	m4return_t	iResult ;
	ClTempTi	*poBaseTi ;

	m4double_t	fValue ;
	m4pcchar_t	pccValue ;


	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TIS_INHERIT_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poTiInherit->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TIS_INHERIT_ID_TI_BASE, M4MDFAC_ITEM_ID_TI_BASE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	poBaseTi = ao_poChannel->GetTiById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Tis Inheritance" ) ;

	ao_poTiInherit->SetBaseTi( poBaseTi ) ;

	// Se comprueba que ninguna Ti herede de TI_SYTEM
	CHECK_CHLOG_ERRORF( strcmp( poBaseTi->GetTiId(), M4MDFAC_TI_TI_SYSTEM ) == 0, M4_ERROR, M4_MDFAC_SYSTEM_INHERIT, M4MDFAC_TI_TI_SYSTEM << ao_poTiInherit->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_TIS_INHERIT_ID_LEVEL, M4MDFAC_ITEM_ID_LEVEL, ai_poRegister, -1, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTiInherit->SetLevel( m4uint8_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TIS_INHERIT_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneItemFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItem *ao_poItem )
{

    m4return_t	iResult ;
	m4uint8_t	iInternalType = M4CL_INTERNAL_TYPE_NULL ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;

    

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poItem->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poItem->SetItemId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



// maximo
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_ID_SYNONYM, M4MDFAC_ITEM_SYNONYM_ID, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	ao_poItem->SetSynonym( m4int32_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_TYPE_METHOD, M4MDFAC_ITEMS_ID_ITEM_TYPE, M4MDFAC_ITEM_ID_ITEM_TYPE, ai_poRegister, M4CL_ITEM_TYPE_METHOD, M4CL_ITEM_TYPE_CONCEPT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 3.0 )
	{
		iValue = M4CL_ITEM_TYPE_FIELD ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_ITEM_TYPE_METHOD ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_TYPE_PROPERTY ;
	}
	else
	{
		iValue = M4CL_ITEM_TYPE_CONCEPT ;
	}

    ao_poItem->SetType( iValue ) ;

    
	
	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_SCOPE_REGISTER, M4MDFAC_ITEMS_ID_SCOPE_TYPE, M4MDFAC_ITEM_ID_SCOPE_TYPE, ai_poRegister, M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_REGISTER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 3.0 )
	{
		iValue = M4CL_ITEM_SCOPE_REGISTER ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_SCOPE_BLOCK ;
	}
	else
	{
		iValue = M4CL_ITEM_SCOPE_NODE ;
	}

    ao_poItem->SetScope( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_M4_TYPE_NUMBER, M4MDFAC_ITEMS_ID_M4_TYPE, M4MDFAC_ITEM_ID_M4_TYPE, ai_poRegister, M4CL_M4_TYPE_NULL, M4CL_M4_TYPE_TIME_SPAN ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


	iValue = m4uint8_t( fValue ) ;

	// Los tipos unicode se reducen
	if( iValue == M4CL_M4_TYPE_UNI_VAR_STRING )
	{
		iValue = M4CL_M4_TYPE_VAR_STRING ;
	}
	else if( iValue == M4CL_M4_TYPE_UNI_LONG )
	{
		iValue = M4CL_M4_TYPE_LONG ;
	}
	else if( iValue == M4CL_M4_TYPE_UNI_VARIANT )
	{
		iValue = M4CL_M4_TYPE_VARIANT ;
	}
	else if( iValue == M4CL_M4_TYPE_UNI_FIXED_STRING )
	{
		iValue = M4CL_M4_TYPE_VAR_STRING ;
	}
	else if( iValue == M4CL_M4_TYPE_TIME_SPAN )
	{
		// Si es time span se reduce con un tipo interno
		iValue = M4CL_M4_TYPE_NUMBER ;
		iInternalType = M4CL_INTERNAL_TYPE_TIME_SPAN ;
	}

	// Dejamos de soportar cadenas fijas
	if( iValue == M4CL_M4_TYPE_FIXED_STRING )
	{
		iValue = M4CL_M4_TYPE_VAR_STRING ;
	}

    ao_poItem->SetM4Type( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_PREC, M4MDFAC_ITEM_PREC, ai_poRegister, -1, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetDBPrecision( m4uint32_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_SCALE, M4MDFAC_ITEM_SCALE, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetScale( m4uint8_t( fValue ) ) ;

    iResult = ao_poItem->SetCppType( ao_poChannel ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



// este tiene que ser NULL !!!!!!!!!
	iResult = M4ReadString( ao_poChannel, pccValue, NULL, M4MDFAC_ITEMS_DEFAULT_VALUE, M4MDFAC_ITEM_DEFAULT_VALUE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetDefaultValue( pccValue, ao_poChannel ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_DMD, M4MDFAC_ITEM_ID_DMD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetDmd( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_DMD_COMPONENT, M4MDFAC_ITEM_ID_DMD_COMPONENT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetDmdComponent( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_DMD_CROSSTAB, M4MDFAC_ITEM_ID_DMD_CROSSTAB, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetDmdCrosstab( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_DMD_FIELD, M4MDFAC_ITEM_ID_DMD_FIELD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetDmdField( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_SLICE_BHRV_BASE_VALUE, M4MDFAC_ITEMS_ID_SLICE_BHVR_TYPE, M4MDFAC_ITEM_ID_SLICE_BHVR_TYPE, ai_poRegister, M4CL_ITEM_SLICE_BHRV_BASE_VALUE, M4CL_ITEM_SLICE_BHRV_PERIOD_FINAL_VALUE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_ITEM_SLICE_BHRV_BASE_VALUE ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_SLICE_BHRV_FINAL_VALUE ;
	}
	else if( fValue == 3.0 )
	{
		iValue = M4CL_ITEM_SLICE_BHRV_UNIT ;
	}
	else if( fValue == 4.0 )
	{
		iValue = M4CL_ITEM_SLICE_BHRV_INCIDENCE ;
	}
	else
	{
		// Si es PERIOD_FINAL_VALUE es como si fuera FINAL_VALUE
		iValue = M4CL_ITEM_SLICE_BHRV_FINAL_VALUE ;
	}

    ao_poItem->SetSliceBhrv( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_SLICE_SPLIT_LINEAR, M4MDFAC_ITEMS_ID_SLICE_SPLIT_TP, M4MDFAC_ITEM_ID_SLICE_SPLIT_TP, ai_poRegister, M4CL_ITEM_SLICE_SPLIT_LINEAR, M4CL_ITEM_SLICE_SPLIT_NO_SLICES ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_ITEM_SLICE_SPLIT_LINEAR ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_SLICE_SPLIT_NON_LINEAR ;
	}
	else
	{
		iValue = M4CL_ITEM_SLICE_SPLIT_NO_SLICES ;
	}

    ao_poItem->SetSliceSplit( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_TOTALIZE_FIRST, M4MDFAC_ITEMS_ID_SLICE_TOT_TYPE, M4MDFAC_ITEM_ID_SLICE_TOT_TYPE, ai_poRegister, M4CL_ITEM_TOTALIZE_MIN_TYPE, M4CL_ITEM_TOTALIZE_MAX_TYPE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetIdSliceTotType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_TOTALIZE_SUM, M4MDFAC_ITEMS_ID_TOTALIZE_TYPE, M4MDFAC_ITEM_ID_TOTALIZE_TYPE, ai_poRegister, M4CL_ITEM_TOTALIZE_MIN_TYPE, M4CL_ITEM_TOTALIZE_MAX_TYPE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetIdTotalize( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED, M4MDFAC_ITEMS_ID_TRANSACT_TYPE, M4MDFAC_ITEM_ID_TRANSACT_TYPE, ai_poRegister, M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 4.0 )
	{
		iValue = M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION ;
	}
	else
	{
		iValue = M4CL_ITEM_TRAN_MODE_TRANSACTION_SUPPORTED ;
	}

    ao_poItem->SetTransactionMode( iValue ) ;



// Por defecto 2. Mínimo 1 y máximo 32 (5bits)
	iResult = M4ReadNumber( ao_poChannel, fValue, 2, M4MDFAC_ITEMS_ID_EXE_LEVEL, M4MDFAC_ITEM_ID_EXE_LEVEL, ai_poRegister, 1, 32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetMethodLevel( m4uint8_t( fValue ) ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_EXE_GROUP, M4MDFAC_ITEM_EXE_GROUP, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetGroup( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_SLICE_ITEM, M4MDFAC_ITEM_ID_SLICE_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetSliceItem( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_ITEM_AUX, M4MDFAC_ITEM_ID_ITEM_AUX, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetAuxItem( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_ITEM_AUX_PRORAT, M4MDFAC_ITEM_ID_ITEM_AUX_PRORAT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetAuxItemProrat( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ALIAS_READ_OBJECT, M4MDFAC_ITEM_ALIAS_READ_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetReadObjectAlias( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ALIAS_WRITE_OBJECT, M4MDFAC_ITEM_ALIAS_WRITE_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetWriteObjectAlias( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_READ_FIELD, M4MDFAC_ITEM_ID_READ_FIELD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetReadFieldId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_AFFECTS_DB, M4MDFAC_ITEM_AFFECTS_DB, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}


	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_WRITE_FIELD, M4MDFAC_ITEM_ID_WRITE_FIELD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetWriteFieldId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// la marca de que afecta a base de datos se leyó má arriba

	if( iValue == 1 || ( pccValue != NULL && *pccValue != '\0' ) )
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_AFFECTS_DB ) ;
	}
	else
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_AFFECTS_DB ) ;
	}



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_READ_OBJECT, M4MDFAC_ITEM_ID_READ_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetReadObjectId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ID_WRITE_OBJECT, M4MDFAC_ITEM_ID_WRITE_OBJECT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetWriteObjectId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    
	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_TOTALIZE_NONE, M4MDFAC_ITEMS_ID_SENT_TOT_TYPE, M4MDFAC_ITEM_ID_SENT_TOT_TYPE, ai_poRegister, M4CL_ITEM_TOTALIZE_NONE, M4CL_ITEM_TOTALIZE_MIN ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetSentTotType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_ITEM_ORDER, M4MDFAC_ITEM_ORDER, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetOrder( m4uint16_t( fValue ) ) ;
    

    
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_VARIABLE_ARGUMENTS, M4MDFAC_ITEM_VARIABLE_ARGUMENTS, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_IS_VISIBLE, M4MDFAC_ITEM_IS_VISIBLE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VISIBLE ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_VISIBLE ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_MODIFY_NONE, M4MDFAC_ITEMS_MODIFY_OTHER_ITEMS, M4MDFAC_ITEM_MODIFY_OTHER_ITEMS, ai_poRegister, M4CL_ITEM_MODIFY_NONE, M4CL_ITEM_MODIFY_LANE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetModifyData( m4uint8_t( fValue ) ) ;


	if( iInternalType == M4CL_INTERNAL_TYPE_NULL )
	{
		iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_INTERNAL_TYPE_NULL, M4MDFAC_ITEMS_ID_INTERNAL_TYPE, M4MDFAC_ITEM_INTERNAL_TYPE, ai_poRegister, M4CL_INTERNAL_TYPE_NULL, M4CL_INTERNAL_TYPE_MAX ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		iInternalType = m4uint8_t( fValue ) ;
	}

    ao_poItem->SetInternalType( iInternalType ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_IS_PUBLIC, M4MDFAC_ITEM_IS_PUBLIC, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_PUBLIC ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_PUBLIC ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_IS_PARAMETER, M4MDFAC_ITEM_IS_PARAMETER, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_PARAMETER ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_PARAMETER ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_CHECK_NOT_NULL, M4MDFAC_ITEM_CHECK_NOT_NULL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_CLIENT_NOT_NULL, M4MDFAC_ITEM_CLIENT_NOT_NULL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_CLIENT_NOT_NULL ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_CLIENT_NOT_NULL ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_NOT_NULL, M4MDFAC_ITEM_NOT_NULL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_NOT_NULL ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_NOT_NULL ) ;
	}



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ITEM_ID_T3, M4MDFAC_ITEM_ITEM_ID_T3, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetT3Id( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	if( pccValue != NULL && *pccValue != '\0' )
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_NODAL_ITEM ) ;
	}
	else
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_NODAL_ITEM ) ;
	}



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEMS_ITEM_ID_NODE, M4MDFAC_ITEM_ITEM_ID_NODE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItem->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_IS_EVENT_ITEM, M4MDFAC_ITEM_IS_EVENT_ITEM, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else if( fValue == 1.0 )
	{
		iValue = 1 ;
	}

	ao_poItem->SetIsEvent( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CONV_FUNCT_NONE, M4MDFAC_ITEMS_ID_CONV_FUNCTION, M4MDFAC_ITEM_ID_CONV_FUNCTION, ai_poRegister, M4CL_CONV_FUNCT_NONE, M4CL_CONV_FUNCT_FIRST_UPPER_CASE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_CONV_FUNCT_NONE ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_CONV_FUNCT_UPPER_CASE ;
	}
	else if( fValue == 3.0 )
	{
		iValue = M4CL_CONV_FUNCT_LOWER_CASE ;
	}
	else
	{
		iValue = M4CL_CONV_FUNCT_FIRST_UPPER_CASE ;
	}

    ao_poItem->SetConvertFunction( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEMS_IS_PK, M4MDFAC_ITEM_IS_PK, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_PK ) ;
	}
	else
	{
		ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_PK ) ;
	}



// Lo ponemos a M4CL_CSTYPE_UNKNOWN para luego ponerlo a lo que venga del nodo
//	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_BOTH,    M4MDFAC_ITEMS_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, ai_poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;
	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_UNKNOWN, M4MDFAC_ITEMS_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, ai_poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iValue = m4uint8_t( fValue ) ;

	// por si no es un tipo válido
	if( iValue != M4CL_CSTYPE_UNKNOWN )
	{
		if( g_aiReducedItemCsTypesArray[ iValue - 1 ] == -1 )
		{
			DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_VALUE, iValue << M4MDFAC_ITEM_ID_CSTYPE << M4ClCsType( M4CL_CSTYPE_BOTH ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
			iValue = M4CL_CSTYPE_BOTH ;
		}
	}

    ao_poItem->SetCsType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4_SYNC_NULL, M4MDFAC_ITEMS_ID_SYNC_TYPE, M4MDFAC_ITEM_ID_SYNC_TYPE, ai_poRegister, M4_SYNC_NONE, M4_SYNC_REGISTER_BRANCH ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poItem->SetSyncType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_ITEMS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    
    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneItemInheritFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItemInherit *ao_poItemInherit )
{

    m4return_t	iResult ;
	m4pcchar_t	pccTiId ;
	m4pcchar_t	pccItemId ;
	m4pcchar_t	pccValue ;

	
	
	iResult = M4ReadString( ao_poChannel, pccTiId, "", M4MDFAC_ITEM_INHERIT_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poItemInherit->SetTiId( pccTiId, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	
	iResult = M4ReadString( ao_poChannel, pccItemId, "", M4MDFAC_ITEM_INHERIT_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poItemInherit->SetItemId( pccItemId, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_ITEM_INHERIT_N_ITEM, M4MDFAC_ITEM_N_ITEM, ai_poRegister, M4MDFAC_NODE_SYS_ITEMS_INHERIT_TRANS, M4MDFAC_ITEM_INHERIT_TRANS_N_ITEM, pccTiId, pccItemId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poItemInherit->SetItemName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_CONCEPTS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

		

    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneConceptFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempConcept *ao_poConcept )
{

    m4return_t	iResult ;
	m4pcchar_t	pccTiId ;
	m4pcchar_t	pccItemId ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;

	
	
	iResult = M4ReadString( ao_poChannel, pccTiId, "", M4MDFAC_CONCEPTS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poConcept->SetTiId( pccTiId, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	
	iResult = M4ReadString( ao_poChannel, pccItemId, "", M4MDFAC_CONCEPTS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poConcept->SetItemId( pccItemId, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	// No se lee del de por defecto si no está el del idioma, para dejar el que venga del item
	iResult = _ReadName( ao_poChannel, pccValue, M4_FALSE, M4MDFAC_CONCEPTS_N_ITEM, M4MDFAC_ITEM_N_ITEM, ai_poRegister, M4MDFAC_NODE_SYS_CONCEPTS_TRANS, M4MDFAC_CONCEPTS_TRANS_N_ITEM, pccTiId, pccItemId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poConcept->SetItemName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_CONCEPTS_HAS_BACKPAY, M4MDFAC_ITEM_HAS_BACKPAY, ai_poRegister, 0, 1 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poConcept->SetHasBackPay( iValue ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_CONCEPTS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

		

    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneArgumentFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempArgument *ao_poArgument )
{

    m4return_t	iResult ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;

    

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_ARGS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poArgument->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_ARGS_ID_ITEM, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poArgument->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_ARGS_ID_ARGUMENT, M4MDFAC_ITEM_ID_ARGUMENT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poArgument->SetArgumentId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_ARGS_N_ARGUMENT, M4MDFAC_ITEM_N_ARGUMENT, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poArgument->SetName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_ARGUMENT_TYPE_INPUT, M4MDFAC_ITEM_ARGS_ID_ARGUMENT_TYPE, M4MDFAC_ITEM_ID_ARGUMENT_TYPE, ai_poRegister, M4CL_ITEM_ARGUMENT_TYPE_INPUT, M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_ITEM_ARGUMENT_TYPE_INPUT ;
	}
	else
	{
		iValue = M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ;
	}

    ao_poArgument->SetType( iValue ) ;



	/* Bug 0132500
	Se actualiza el valor máximo de los tipos
	*/
	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_M4_TYPE_NUMBER, M4MDFAC_ITEM_ARGS_ID_M4_TYPE, M4MDFAC_ITEM_ID_M4_TYPE, ai_poRegister, M4CL_M4_TYPE_NULL, M4CL_M4_TYPE_TIME_SPAN ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poArgument->SetM4Type( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEM_ARGS_PREC, M4MDFAC_ITEM_PREC, ai_poRegister, -1, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poArgument->SetPrecision( m4uint16_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_ITEM_ARGS_POSITION, M4MDFAC_ITEM_POSITION, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poArgument->SetOrder( m4uint32_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_ITEM_ARGS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneRuleFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItemFormula *ao_poFormula )
{

    m4return_t		iResult ;
    m4uint8_t		iType ;
    m4uint8_t		iIsCorrectRule ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;
	m4date_t		dOldDate ;
	m4date_t		dUserDate ;
    m4char_t		acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t		acEndDate  [ M4_SECOND_END + 1 ] ;

	m4uint32_t		iHandle ;
	ClCompiledMCR	*poCMCR ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;
    m4VariantType	vValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RULES_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poFormula->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RULES_ID_ITEM, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poFormula->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RULES_ID_RULE, M4MDFAC_ITEM_ID_RULE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poFormula->SetFormulaId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 1, M4MDFAC_RULES_IS_CORRECT_RULE, M4MDFAC_ITEM_IS_CORRECT_RULE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else
	{
		iValue = 0 ;
	}

	iIsCorrectRule = iValue ;

	/* Bug 0099169
	El control hay que hacerlo después de controlar los items nodales
	CHECK_CHLOG_ERRORF( iIsCorrectRule != 1, M4_ERROR, M4_MDFAC_INCORRECT_RULE, pccValue << ao_poFormula->GetItemId() << ao_poFormula->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
	*/
	ao_poFormula->SetIsCorrect( iIsCorrectRule ) ;


	
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_IS_METARULE, M4MDFAC_ITEM_IS_METARULE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

	ao_poFormula->SetIsMetarule( iValue ) ;



	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_RULES_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_RULES_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Chequeos de validez de fechas
	if( dEndDate < dStartDate )
	{
		ClDateToString( dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_RULE, ao_poFormula->GetFormulaId() << acStartDate << acEndDate << ao_poFormula->GetItemId() << ao_poFormula->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

    ao_poFormula->SetStartDate( dStartDate ) ;
    ao_poFormula->SetEndDate( dEndDate ) ;


	// Asignar las fechas de aplicacion al canal
	dOldDate = ao_poChannel->GetStartDate() ;
	dUserDate = ao_poChannel->GetUserStartDate() ;

	if( dStartDate > dOldDate && dStartDate <= dUserDate )
	{
		ao_poChannel->SetStartDate( dStartDate ) ;
	}

	dOldDate = ao_poChannel->GetEndDate() ;
	dUserDate = ao_poChannel->GetUserEndDate() ;

	if( dEndDate < dOldDate && dEndDate >= dUserDate )
	{
		ao_poChannel->SetEndDate( dEndDate ) ;
	}



	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_RULES_DT_START_CORR, M4MDFAC_ITEM_DT_START_CORR, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_RULES_DT_END_CORR, M4MDFAC_ITEM_DT_END_CORR, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Asignar las fechas de corrección al canal
	dOldDate = ao_poChannel->GetCorStartDate() ;

	if( dStartDate > dOldDate )
	{
		ao_poChannel->SetCorStartDate( dStartDate ) ;
	}

	dOldDate = ao_poChannel->GetCorEndDate() ;

	if( dEndDate < dOldDate )
	{
		ao_poChannel->SetCorEndDate( dEndDate ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NO_LENGUAJE, M4MDFAC_RULES_ID_CODE_TYPE, M4MDFAC_ITEM_ID_CODE_TYPE, ai_poRegister, M4CL_NO_LENGUAJE, M4CL_INTERN_OBJECT_DLL ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iType = m4uint8_t( fValue ) ;
    ao_poFormula->SetType( iType ) ;



	if( iIsCorrectRule == 1 )
	{
		iResult = ai_poRegister->Item[ M4MDFAC_RULES_POLISH_CODE ].Value.Get( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, M4MDFAC_ITEM_POLISH_CODE << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		if( vValue.Type == M4CL_CPP_TYPE_NULL )
		{
			CHECK_CHLOG_ERRORF( iType == M4CL_LENGUAGE_LN4, M4_ERROR, M4_MDFAC_NULL_POLISH, ao_poFormula->GetFormulaId() << ao_poFormula->GetItemId() << ao_poFormula->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;

			vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
			vValue.Data.PointerChar = "" ;
		}
		else if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_poFormula->SetPolish( vValue.Data.PointerChar, m_poFunctionTable ) ;
	}



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_RULE_MIN_PRIORITY, M4MDFAC_RULES_ID_PRIORITY, M4MDFAC_ITEM_ID_PRIORITY, ai_poRegister, M4CL_RULE_MAX_PRIORITY, M4CL_RULE_MIN_PRIORITY ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else if( fValue == 2.0 )
	{
		iValue = 2 ;
	}
	else
	{
		iValue = 3 ;
	}

    ao_poFormula->SetPriority( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_IS_EVENT_RULE, M4MDFAC_ITEM_IS_EVENT_RULE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

	ao_poFormula->SetIsEvent( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_IS_RETROACTIVITY, M4MDFAC_ITEM_IS_RETROACTIVITY, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poFormula->SetIsRetroactivity( iValue ) ;


	
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_BATCH_TYPE, M4MDFAC_ITEM_BATCH_TYPE, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poFormula->SetBatchType( m4uint8_t( fValue ) ) ;


	
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_IS_BATCH_METARULE, M4MDFAC_ITEM_IS_BATCH_METARULE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poFormula->SetIsBatchMetarule( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_RULES_RULE_ORDER, M4MDFAC_ITEM_RULE_ORDER, ai_poRegister, -M4MDFAC_MAX_INT_32, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poFormula->SetDBOrder( m4int32_t( fValue ) ) ;


	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RULES_ID_RESOURCE, M4MDFAC_ITEM_ID_RESOURCE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poFormula->SetObject( pccValue ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_RULES_ID_METHOD, M4MDFAC_ITEM_ID_METHOD, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poFormula->SetMethod( pccValue, m_poFunctionTable ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_RULES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTagFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTag *ao_poTag )
{

    m4return_t	iResult ;
	m4char_t	cOperator ;
	m4date_t	dStartDate ;
	m4date_t	dEndDate ;
	m4date_t	dOldDate ;
	m4date_t	dUserDate ;
    m4char_t	acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t	acEndDate  [ M4_SECOND_END + 1 ] ;

	m4pcchar_t	pccValue ;

	

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ID_TAG, M4MDFAC_ID_TAG, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetTagId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ID_RULE, M4MDFAC_ITEM_ID_RULE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetRuleId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ID_DIMENSION_ITEM, M4MDFAC_ID_DIMENSION_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetDimItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_ZVALUE, M4MDFAC_ITEM_ZVALUE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTag->SetZValue( pccValue, M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TAGS_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TAGS_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Chequeos de validez de fechas
	if( dEndDate < dStartDate )
	{
		ClDateToString( dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_TAG, ao_poTag->GetDimItemId() << ao_poTag->GetRuleId() << ao_poTag->GetItemId() << ao_poTag->GetTiId() << acStartDate << acEndDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

    ao_poTag->SetStartDate( dStartDate ) ;
    ao_poTag->SetEndDate( dEndDate ) ;


	// Asignar las fechas de aplicacion al canal
	dOldDate = ao_poChannel->GetStartDate() ;
	dUserDate = ao_poChannel->GetUserStartDate() ;

	if( dStartDate > dOldDate && dStartDate <= dUserDate )
	{
		ao_poChannel->SetStartDate( dStartDate ) ;
	}

	dOldDate = ao_poChannel->GetEndDate() ;
	dUserDate = ao_poChannel->GetUserEndDate() ;

	if( dEndDate < dOldDate && dEndDate >= dUserDate )
	{
		ao_poChannel->SetEndDate( dEndDate ) ;
	}


	
	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TAGS_DT_START_CORR, M4MDFAC_ITEM_DT_START_CORR, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TAGS_DT_END_CORR, M4MDFAC_ITEM_DT_END_CORR, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Asignar las fechas de corrección al canal
	dOldDate = ao_poChannel->GetCorStartDate() ;

	if( dStartDate > dOldDate )
	{
		ao_poChannel->SetCorStartDate( dStartDate ) ;
	}

	dOldDate = ao_poChannel->GetCorEndDate() ;

	if( dEndDate < dOldDate )
	{
		ao_poChannel->SetCorEndDate( dEndDate ) ;
	}



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TAGS_OPERATOR, M4MDFAC_ITEM_OPERATOR, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( strcmp( pccValue, "=" ) == 0 )
	{
		cOperator = '3' ;
	}
	else if( strcmp( pccValue, "<>" ) == 0 )
	{
		cOperator = '4' ;
	}
	else if( strcmp( pccValue, ">" ) == 0 )
	{
		cOperator = '5' ;
	}
	else if( strcmp( pccValue, ">=" ) == 0 )
	{
		cOperator = '6' ;
	}
	else if( strcmp( pccValue, "<" ) == 0 )
	{
		cOperator = '7' ;
	}
	else if( strcmp( pccValue, "<=" ) == 0 )
	{
		cOperator = '8' ;
	}
	else
	{
		cOperator = '3' ;
	}

	ao_poTag->SetOperator( cOperator ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TAGS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTriggerFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTrigger *ao_poTrigger )
{

    m4return_t	iResult ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;

    

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TRIGGERS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTrigger->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TRIGGERS_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    iResult = ao_poTrigger->SetFatherItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TRIGGERS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    iResult = ao_poTrigger->SetSonItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_AFTER, M4MDFAC_TRIGGERS_ID_PRECEDENCE_TYPE, M4MDFAC_ITEM_ID_PRECEDENCE_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_BEFORE, M4CL_ITEM_CONNECTOR_AFTER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_CONNECTOR_AFTER ;
	}
	else
	{
		iValue = M4CL_ITEM_CONNECTOR_BEFORE ;
	}

    ao_poTrigger->SetPrecedence( iValue ) ;

    

	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_DOWNWARDS, M4MDFAC_TRIGGERS_ID_SPIN_TYPE, M4MDFAC_ITEM_ID_SPIN_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_UPWARDS, M4CL_ITEM_CONNECTOR_DOWNWARDS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_CONNECTOR_DOWNWARDS ;
	}
	else
	{
		iValue = M4CL_ITEM_CONNECTOR_UPWARDS ;
	}

    ao_poTrigger->SetSpin( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_EQUALITY, M4MDFAC_TRIGGERS_ID_RELSHIP_TYPE, M4MDFAC_ITEM_ID_RELSHIP_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_EQUALITY, M4CL_ITEM_CONNECTOR_DISTINCT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTrigger->SetRelation( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_VM_TRIGGER_ALL, M4MDFAC_TRIGGERS_ID_CONTEXT_TYPE, M4MDFAC_ITEM_ID_CONTEXT_TYPE, ai_poRegister, M4CL_VM_TRIGGER_ALL, M4CL_VM_TRIGGER_CURRENT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTrigger->SetContext( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_BOTH, M4MDFAC_TRIGGERS_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, ai_poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iValue = m4uint8_t( fValue ) ;

	// por si no es un tipo válido
	if( g_aiReducedConnCsTypesArray[ iValue - 1 ] == -1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_VALUE, iValue << M4MDFAC_ITEM_ID_CSTYPE << M4ClCsType( M4CL_CSTYPE_BOTH ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		iValue = M4CL_CSTYPE_BOTH ;
	}

	ao_poTrigger->SetCsType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_TRIGGERS_CALL_ORDER, M4MDFAC_ITEM_CALL_ORDER, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poTrigger->SetCallOrder( m4uint8_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TRIGGERS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneTotalFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTotal *ao_poTotal )
{

    m4return_t	iResult ;

    m4uint8_t	iValue ;
    m4int8_t	iSumType ;
	m4double_t	fValue ;
	m4date_t	dStartDate ;
	m4date_t	dEndDate ;
	m4date_t	dTotalStart ;
	m4date_t	dTotalEnd ;
	m4pcchar_t	pccValue ;
    m4char_t	acDate     [ M4_SECOND_END + 1 ] ;
    m4char_t	acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t	acEndDate  [ M4_SECOND_END + 1 ] ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTALS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotal->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTALS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotal->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTALS_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotal->SetItemUsedId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadDate( ao_poChannel, dTotalStart, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TOTALS_TOTAL_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dTotalEnd, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TOTALS_TOTAL_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TOTALS_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TOTALS_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Chequeos de validez de fechas

	if( dStartDate < dTotalStart || dStartDate > dTotalEnd )
	{
		ClDateToString( dStartDate,  acDate,      M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_TOTAL_START_DATE, acDate << ao_poTotal->GetItemUsedId() << acStartDate << acEndDate << ao_poTotal->GetItemId() << ao_poTotal->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dEndDate < dTotalStart || dEndDate > dTotalEnd )
	{
		ClDateToString( dEndDate,    acDate,      M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_TOTAL_END_DATE, acDate << ao_poTotal->GetItemUsedId() << acStartDate << acEndDate << ao_poTotal->GetItemId() << ao_poTotal->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dTotalEnd < dTotalStart )
	{
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_TOTAL, ao_poTotal->GetItemUsedId() << ao_poTotal->GetItemId() << acStartDate << acEndDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dEndDate < dStartDate )
	{
		ClDateToString( dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_TOTAL_REF, ao_poTotal->GetItemUsedId() << ao_poTotal->GetItemId() << acStartDate << acEndDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

    ao_poTotal->SetStartDate( dStartDate ) ;
    ao_poTotal->SetEndDate( dEndDate ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_TOTALS_HAS_BACKPAY, M4MDFAC_ITEM_HAS_BACKPAY, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poTotal->SetHasBackPay( iValue ) ;

	
	
	iResult = M4ReadNumber( ao_poChannel, fValue, 1, M4MDFAC_TOTALS_IS_SUM, M4MDFAC_ITEM_IS_SUM, ai_poRegister, -1, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Si es 1 es suma, -1 es nada y 0 es resta
	if( fValue == 1.0 )
	{
		iSumType = 1 ;
	}
	else if( fValue == -1.0 )
	{
		iSumType = 0 ;
	}
	else
	{
		iSumType = -1 ;
	}

    ao_poTotal->SetSumType( iSumType ) ;

	
	
	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TOTALS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TOTALS_TOTAL_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTotalTagFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTotalTag *ao_poTotalTag )
{

    m4return_t	iResult ;
	m4char_t	cOperator ;
	m4date_t	dStartDate ;
	m4date_t	dEndDate ;
	m4date_t	dTotalStart ;
	m4date_t	dTotalEnd ;
	m4date_t	dOldDate ;
	m4date_t	dUserDate ;
    m4char_t	acDate     [ M4_SECOND_END + 1 ] ;
    m4char_t	acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t	acEndDate  [ M4_SECOND_END + 1 ] ;

	m4pcchar_t	pccValue ;

	

	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ID_TAG, M4MDFAC_ID_TAG, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetTagId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetTiId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetItemUsedId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ID_DIMENSION_ITEM, M4MDFAC_ID_DIMENSION_ITEM, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetDimItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_ZVALUE, M4MDFAC_ITEM_ZVALUE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poTotalTag->SetZValue( pccValue, M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadDate( ao_poChannel, dTotalStart, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TOTAL_TAGS_DT_START_USED, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dTotalEnd, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TOTAL_TAGS_DT_END_USED, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	
	
	iResult = M4ReadDate( ao_poChannel, dStartDate, M4CL_MINUS_INFINITE_IN_JULIAN, M4MDFAC_TOTAL_TAGS_DT_START, M4MDFAC_ITEM_DT_START, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = M4ReadDate( ao_poChannel, dEndDate, M4CL_PLUS_INFINITE_IN_JULIAN, M4MDFAC_TOTAL_TAGS_DT_END, M4MDFAC_ITEM_DT_END, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	// Chequeos de validez de fechas

	if( dStartDate < dTotalStart || dStartDate > dTotalEnd )
	{
		ClDateToString( dStartDate,  acDate,      M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_TOTAL_TAG_START_DATE, acDate << ao_poTotalTag->GetDimItemId() << ao_poTotalTag->GetItemUsedId() << acStartDate << acEndDate << ao_poTotalTag->GetItemId() << ao_poTotalTag->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dEndDate < dTotalStart || dEndDate > dTotalEnd )
	{
		ClDateToString( dEndDate,    acDate,      M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_TOTAL_TAG_END_DATE, acDate << ao_poTotalTag->GetDimItemId() << ao_poTotalTag->GetItemUsedId() << acStartDate << acEndDate << ao_poTotalTag->GetItemId() << ao_poTotalTag->GetTiId() << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dTotalEnd < dTotalStart )
	{
		ClDateToString( dTotalStart, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dTotalEnd,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_TOTAL_REF, ao_poTotalTag->GetItemUsedId() << ao_poTotalTag->GetItemId() << acStartDate << acEndDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	if( dEndDate < dStartDate )
	{
		ClDateToString( dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_TOTAL_TAG, ao_poTotalTag->GetDimItemId() << ao_poTotalTag->GetItemUsedId() << ao_poTotalTag->GetItemId() << ao_poTotalTag->GetTiId() << acStartDate << acEndDate << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

    ao_poTotalTag->SetStartDate( dStartDate ) ;
    ao_poTotalTag->SetEndDate( dEndDate ) ;


	// Asignar las fechas de aplicacion al canal
	dOldDate = ao_poChannel->GetStartDate() ;
	dUserDate = ao_poChannel->GetUserStartDate() ;

	if( dStartDate > dOldDate && dStartDate <= dUserDate )
	{
		ao_poChannel->SetStartDate( dStartDate ) ;
	}

	dOldDate = ao_poChannel->GetEndDate() ;
	dUserDate = ao_poChannel->GetUserEndDate() ;

	if( dEndDate < dOldDate && dEndDate >= dUserDate )
	{
		ao_poChannel->SetEndDate( dEndDate ) ;
	}



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_TOTAL_TAGS_OPERATOR, M4MDFAC_ITEM_OPERATOR, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( strcmp( pccValue, "=" ) == 0 )
	{
		cOperator = '3' ;
	}
	else if( strcmp( pccValue, "<>" ) == 0 )
	{
		cOperator = '4' ;
	}
	else if( strcmp( pccValue, ">" ) == 0 )
	{
		cOperator = '5' ;
	}
	else if( strcmp( pccValue, ">=" ) == 0 )
	{
		cOperator = '6' ;
	}
	else if( strcmp( pccValue, "<" ) == 0 )
	{
		cOperator = '7' ;
	}
	else if( strcmp( pccValue, "<=" ) == 0 )
	{
		cOperator = '8' ;
	}
	else
	{
		cOperator = '3' ;
	}

	ao_poTotalTag->SetOperator( cOperator ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_TOTAL_TAGS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneNodeFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNode *ao_poNode )
{

    m4return_t		iResult ;
    m4int32_t		iNumRows ;
	m4uint16_t		iLevel ;
	m4uint32_t		iHandle ;
	m4uint32_t		iMaxNumRecords ;
	m4uint32_t		iNumRowsDB ;
	m4double_t		dMaxNumKeepRows ;
	m4pcchar_t		pccNodeId;
	ClTempTi		*poTi ;
	ClTempT3Inherit	*poT3Inherit ;
	ClCompiledMCR	*poCMCR ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;

    

	iResult = M4ReadString( ao_poChannel, pccNodeId, "", M4MDFAC_NODES_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poNode->SetNodeId( pccNodeId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODES_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poTi = ao_poChannel->GetTiById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poTi == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Nodes" ) ;

    ao_poNode->SetTi( poTi ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_ID_LEVEL, M4MDFAC_ITEM_ID_LEVEL, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iLevel = m4uint16_t( fValue ) ;

	poT3Inherit = ao_poChannel->GetT3InheritByLevel( iLevel ) ;
	CHECK_CHLOG_ERRORF( poT3Inherit == NULL, M4_ERROR, M4_MDFAC_BAD_INHERIT_LEVEL, iLevel << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Nodes" ) ;

    ao_poNode->SetLevel( poT3Inherit->GetIndex() ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODES_ID_GROUP_OBJECTS, M4MDFAC_ITEM_ID_GROUP_OBJECTS, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poNode->SetGroupObjects( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_NODES_N_NODE, M4MDFAC_ITEM_N_NODE, ai_poRegister, M4MDFAC_NODE_SYS_NODES_TRANS, M4MDFAC_NODES_TRANS_N_NODE, pccNodeId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poNode->SetName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODES_OLE_NODE_INTER_ID, M4MDFAC_ITEM_OLE_NODE_INTER_ID, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poNode->SetOleNodeInterId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_OLE_DISP_ID, M4MDFAC_ITEM_OLE_DISP_ID, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetOleDispId( m4uint32_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_NODES_TYPE, M4MDFAC_ITEM_NODES_TYPE, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetType( m4uint16_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_POS_NODO, M4MDFAC_ITEM_POS_NODO, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetOrder( m4uint16_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK, M4MDFAC_NODES_ID_CAPACITY_TYPE, M4MDFAC_ITEM_ID_CAPACITY_TYPE, ai_poRegister, M4CL_NODE_CAPACITY_TYPE_MONOBLOCK, M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK ;
	}
	else
	{
		iValue = M4CL_NODE_CAPACITY_TYPE_MONOBLOCK ;
	}

    ao_poNode->SetCapacity( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_IS_ROOT, M4MDFAC_ITEM_IS_ROOT, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poNode->SetIsRoot( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_AUTOLOAD_OFF, M4MDFAC_NODES_AUTOLOAD, M4MDFAC_ITEM_AUTOLOAD, ai_poRegister, M4CL_AUTOLOAD_OFF, M4CL_AUTOLOAD_PRG ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_AUTOLOAD_BLOCK ;
	}
	else if( fValue == 3.0 )
	{
		iValue = M4CL_AUTOLOAD_NODESAYS ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_AUTOLOAD_PRG ;
	}
	else
	{
		iValue = M4CL_AUTOLOAD_OFF ;
	}

    ao_poNode->SetAutoload( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_IS_ORDERED, M4MDFAC_ITEM_IS_ORDERED, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poNode->SetIsOrdered( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_MAX_NUM_RECORDS, M4MDFAC_ITEM_MAX_NUM_RECORDS, ai_poRegister, 0, M4MDFAC_MAX_REGISTERS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iMaxNumRecords = m4uint32_t( fValue ) ;
    ao_poNode->SetMaxNumRecords( iMaxNumRecords ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_NUM_ROWS, M4MDFAC_ITEM_NUM_ROWS, ai_poRegister, 0, M4MDFAC_MAX_REGISTERS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iNumRows = m4int32_t( fValue ) ;

	if( iNumRows < 0 )
	{
		iNumRows = 0 ;
	}

// Limitamos a iMaxNumRecords
	if( iMaxNumRecords > 0 && m4uint32_t( iNumRows ) > iMaxNumRecords )
	{
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;
		DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_LIMIT, iNumRows << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << iMaxNumRecords << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;

		iNumRows = iMaxNumRecords ;
	}

    ao_poNode->SetNumRows( iNumRows ) ;



	if( iNumRows <= 0 )
	{
		dMaxNumKeepRows = 0.0 ;
	}
	else
	{
		dMaxNumKeepRows = iNumRows - 1 ;
	}

	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_NUM_KEEP_ROWS, M4MDFAC_ITEM_NUM_KEEP_ROWS, ai_poRegister, -1, dMaxNumKeepRows ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetNumKeepRows( m4int32_t( fValue ) ) ;




	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_NUM_ROWS_DB, M4MDFAC_ITEM_NUM_ROWS_DB, ai_poRegister, 0, M4MDFAC_MAX_REGISTERS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iNumRowsDB = m4uint32_t( fValue ) ;

// Limitamos a iMaxNumRecords
	if( iMaxNumRecords > 0 && iNumRowsDB > iMaxNumRecords )
	{
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;
		DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_LIMIT, iNumRowsDB << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << iMaxNumRecords << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;

		iNumRowsDB = iMaxNumRecords ;
	}

    ao_poNode->SetNumRowsDB( iNumRowsDB ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_IS_VISIBLE, M4MDFAC_ITEM_IS_VISIBLE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else
	{
		iValue = 0 ;
	}

    ao_poNode->SetIsVisible( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_DYN_FILTER, M4MDFAC_ITEM_DYN_FILTER, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poNode->SetDynFilter( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_AUTO_FILTER_NONE, M4MDFAC_NODES_ID_AUTO_FILTER, M4MDFAC_ITEM_ID_AUTO_FILTER, ai_poRegister, M4CL_NODE_AUTO_FILTER_NONE, M4CL_NODE_AUTO_FILTER_APP_CORR ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = M4CL_NODE_AUTO_FILTER_NONE ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_NODE_AUTO_FILTER_APPLICATION ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_NODE_AUTO_FILTER_CORRECTION ;
	}
	else
	{
		iValue = M4CL_NODE_AUTO_FILTER_APP_CORR ;
	}

    ao_poNode->SetIdAutoFilter( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_AFFECTS_DB, M4MDFAC_ITEM_AFFECTS_DB, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

// Si tiene tabla de escritura siempre se pone a 1

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poNode->SetAffectsDB( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_DB_RELOAD_LOAD_PRG, M4MDFAC_NODES_DB_RELOAD, M4MDFAC_ITEM_DB_RELOAD, ai_poRegister, M4CL_NODE_DB_RELOAD_NONE, M4CL_NODE_DB_RELOAD_LOAD_PRG ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = M4CL_NODE_DB_RELOAD_NONE ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_NODE_DB_RELOAD_LOAD_BLK ;
	}
	else
	{
		iValue = M4CL_NODE_DB_RELOAD_LOAD_PRG ;
	}

    ao_poNode->SetDBReload( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_BOTH, M4MDFAC_NODES_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, ai_poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iValue = m4uint8_t( fValue ) ;

	// por si no es un tipo válido
	if( g_aiReducedNodeCsTypesArray[ iValue - 1 ] == -1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_VALUE, iValue << M4MDFAC_ITEM_ID_CSTYPE << M4ClCsType( M4CL_CSTYPE_BOTH ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		iValue = M4CL_CSTYPE_BOTH ;
	}

    ao_poNode->SetCsType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4_SYNC_NULL, M4MDFAC_NODES_ID_SYNC_TYPE, M4MDFAC_ITEM_ID_SYNC_TYPE, ai_poRegister, M4_SYNC_NONE, M4_SYNC_REGISTER_BRANCH ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetSyncType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_IS_EXTERNAL, M4MDFAC_ITEM_IS_EXTERNAL, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poNode->SetIsExternal( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODES_MAX_NUM_BLOCKS, M4MDFAC_ITEM_MAX_NUM_BLOCKS, ai_poRegister, 0, M4MDFAC_MAX_REGISTERS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNode->SetMaxNumBlocks( m4uint32_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_NODES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneNodeInheritFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNodeInherit *ao_poNodeInherit )
{

    m4return_t      iResult ;
	m4uint16_t		iLevel ;
	ClTempTi		*poTi ;
	ClTempT3Inherit	*poT3Inherit ;

	m4double_t		fValue ;
	m4pcchar_t		pccValue ;

	
	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_INHERIT_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poNodeInherit->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_INHERIT_ID_TI, M4MDFAC_ITEM_ID_TI, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poTi = ao_poChannel->GetTiById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poTi == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Node Inherits" ) ;

    ao_poNodeInherit->SetTi( poTi ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_NODE_INHERIT_ID_LEVEL, M4MDFAC_ITEM_ID_LEVEL, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iLevel = m4uint16_t( fValue ) ;

	poT3Inherit = ao_poChannel->GetT3InheritByLevel( iLevel ) ;
	CHECK_CHLOG_ERRORF( poT3Inherit == NULL, M4_ERROR, M4_MDFAC_BAD_INHERIT_LEVEL, iLevel << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Node Inherits" ) ;

    ao_poNodeInherit->SetLevel( poT3Inherit->GetIndex() ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_NODE_INHERIT_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneConnectorFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempConnector *ao_poConnector )
{

    m4return_t      iResult ;
	m4uint8_t		iType ;
	m4pcchar_t		pccFatherNodeId ;
	m4pcchar_t		pccSonNodeId ;
    ClTempNode      *poSonNode ;
    ClTempSentence  *poSentence ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;



	iResult = M4ReadString( ao_poChannel, pccFatherNodeId, "", M4MDFAC_CONNECTORS_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    ao_poConnector->SetFatherNodeId( pccFatherNodeId, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccSonNodeId, "", M4MDFAC_CONNECTORS_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poSonNode = ao_poChannel->GetNodeById( pccSonNodeId, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccSonNodeId << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Connectors" ) ;

    ao_poConnector->SetSonNode( poSonNode ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_CONNECTOR_TYPE_FREE, M4MDFAC_CONNECTORS_ID_CONNECTION_TYPE, M4MDFAC_ITEM_ID_CONNECTION_TYPE, ai_poRegister, M4CL_NODE_CONNECTOR_TYPE_RB, M4CL_NODE_CONNECTOR_TYPE_AUTO ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iType = m4uint8_t( fValue ) ;

    if( strcmpi( pccSonNodeId, pccFatherNodeId ) == 0 )
    {
        if( iType == M4CL_NODE_CONNECTOR_TYPE_FREE )
        {
            iType = M4CL_NODE_CONNECTOR_TYPE_AUTO ;
        }
        else if( iType != M4CL_NODE_CONNECTOR_TYPE_AUTO )
        {
			DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_AUTO_CONNECTOR_TYPE, LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( poSonNode->iM4Obj(), poSonNode->nM4Obj() ) << M4ClConnectedNodeType( iType ) ) ;
		    return( M4_ERROR ) ;
        }
    }
	else
	{
		if( iType == M4CL_NODE_CONNECTOR_TYPE_AUTO )
        {
			DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_NO_AUTO_CONNECTOR_TYPE, pccFatherNodeId << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( poSonNode->iM4Obj(), poSonNode->nM4Obj() ) ) ;
		    return( M4_ERROR ) ;
        }
	}

    ao_poConnector->SetType( iType ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_CONNECTORS_EXPAND_DELETE, M4MDFAC_ITEM_EXPAND_DELETE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poConnector->SetExpandDelete( iValue ) ;


	
	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_CONNECTORS_EXPAND_UNDO, M4MDFAC_ITEM_EXPAND_UNDO, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poConnector->SetExpandUndo( iValue ) ;
	

    
	iResult = M4ReadString( ao_poChannel, pccValue, NULL, M4MDFAC_CONNECTORS_ID_SENTENCE, M4MDFAC_ITEM_ID_SENTENCE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( pccValue != NULL )
	{
		poSentence = ao_poChannel->GetSentenceById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poSentence == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_CONNECTOR_SENTENCE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << pccFatherNodeId << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) ) ;
	}
	else
	{
		poSentence = NULL ;
	}

    ao_poConnector->SetSentence( poSentence ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_CONNECTORS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneItemConnFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNitemConn *ao_poNitemConn )
{

    m4return_t	iResult ;
    ClTempNitem	*poSonNitem ;
    ClTempNode	*poSonNode ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ITEM_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poNitemConn->SetFatherNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ITEM_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poNitemConn->SetFatherItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ITEM_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poSonNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Item Connectors" ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ITEM_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poSonNitem = poSonNode->GetNitemById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Item Connectors" ) ;

    ao_poNitemConn->SetSonNitem( poSonNitem ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_AFTER, M4MDFAC_CONNECTOR_ITEM_ID_PRECEDENCE_TYPE, M4MDFAC_ITEM_ID_PRECEDENCE_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_BEFORE, M4CL_ITEM_CONNECTOR_AFTER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_CONNECTOR_AFTER ;
	}
	else
	{
		iValue = M4CL_ITEM_CONNECTOR_BEFORE ;
	}

    ao_poNitemConn->SetPrecedence( iValue ) ;

    

	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_DOWNWARDS, M4MDFAC_CONNECTOR_ITEM_ID_SPIN_TYPE, M4MDFAC_ITEM_ID_SPIN_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_UPWARDS, M4CL_ITEM_CONNECTOR_DOWNWARDS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 2.0 )
	{
		iValue = M4CL_ITEM_CONNECTOR_DOWNWARDS ;
	}
	else
	{
		iValue = M4CL_ITEM_CONNECTOR_UPWARDS ;
	}

    ao_poNitemConn->SetSpin( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_EQUALITY, M4MDFAC_CONNECTOR_ITEM_ID_RELSHIP_TYPE, M4MDFAC_ITEM_ID_RELSHIP_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_EQUALITY, M4CL_ITEM_CONNECTOR_DISTINCT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNitemConn->SetRelation( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_VM_TRIGGER_ALL, M4MDFAC_CONNECTOR_ITEM_ID_CONTEXT_TYPE, M4MDFAC_ITEM_ID_CONTEXT_TYPE, ai_poRegister, M4CL_VM_TRIGGER_ALL, M4CL_VM_TRIGGER_CURRENT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNitemConn->SetContext( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_BOTH, M4MDFAC_CONNECTOR_ITEM_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, ai_poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iValue = m4uint8_t( fValue ) ;

	// por si no es un tipo válido
	if( g_aiReducedConnCsTypesArray[ iValue - 1 ] == -1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_VALUE, iValue << M4MDFAC_ITEM_ID_CSTYPE << M4ClCsType( M4CL_CSTYPE_BOTH ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
		iValue = M4CL_CSTYPE_BOTH ;
	}

    ao_poNitemConn->SetCsType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_CONNECTOR_ITEM_CALL_ORDER, M4MDFAC_ITEM_CALL_ORDER, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poNitemConn->SetCallOrder( m4uint8_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_CONNECTOR_ITEM_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneArgumentMappingFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, StArgumentMappig_t *ao_pstMapping, ClTempNitem *ai_poSonNitem, ClTempNode *ai_poSonNode, ClTempItem *ai_poFatherItem, ClTempNode *ai_poFatherNode, ClTempArgument *ai_poArgument )
{

    m4return_t		iResult ;
    m4uint8_t		iOrder ;
    m4uint8_t		iType ;
    ClTempItem		*poSonItem ;
    ClTempArgument	*poArgument ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;



    iOrder = ai_poArgument->GetRealOrder() ;
    ao_pstMapping->m_iFatherRealOrder = iOrder ;
    
    iType = ai_poArgument->GetType() ;
    ao_pstMapping->m_iFatherArgumentType = iType ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_ARG, M4MDFAC_ITEM_ID_ARG, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poSonItem = ai_poSonNitem->GetItem() ;
	CHECK_CHLOG_DEBUGF( poSonItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    poArgument = poSonItem->GetArgumentById( pccValue ) ;
	CHECK_CHLOG_ERRORF( poArgument == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_ARG, pccValue << LOG_CAT_AND( poSonItem->iItem(), poSonItem->nItem() ) << LOG_CAT_AND( ai_poSonNode->iNode(), ai_poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << LOG_CAT_AND( ai_poFatherItem->iItem(), ai_poFatherItem->nItem() ) << LOG_CAT_AND( ai_poFatherNode->iNode(), ai_poFatherNode->nNode() ) ) ;


    iOrder = poArgument->GetRealOrder() ;
    ao_pstMapping->m_iSonRealOrder = iOrder ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ARGUMENT_MAPPING_INPUT, M4MDFAC_CONNECTOR_ARGS_ID_ARG_VAL_USED_TYPE, M4MDFAC_ITEM_ID_ARG_VAL_USED_TYPE, ai_poRegister, M4CL_ARGUMENT_MAPPING_INPUT, M4CL_ARGUMENT_MAPPING_REFERENCE ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_ARGUMENT_MAPPING_INPUT ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_ARGUMENT_MAPPING_OUTPUT ;
	}
	else
	{
		iValue = M4CL_ARGUMENT_MAPPING_REFERENCE ;
	}

    ao_pstMapping->m_iMappingType = iValue ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_CONNECTOR_ARGS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneIndexFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempIndex *ao_poIndex )
{

    m4return_t	iResult ;
    m4uint32_t	iIndex ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_INDEXES_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poIndex->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_INDEXES_ID_INDEX, M4MDFAC_ITEM_ID_INDEX, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iIndex = m4uint32_t( fValue ) ;
	CHECK_CHLOG_ERRORF( iIndex == 0, M4_ERROR, M4_MDFAC_ZERO_INDEX, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;

    ao_poIndex->SetIndexId( iIndex ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_INDEXES_IS_DEFAULT_INDEX, M4MDFAC_ITEM_IS_DEFAULT_INDEX, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poIndex->SetIsDefault( iValue ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_INDEXES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneIndexItemFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempIndexItem *ao_poIndexItem )
{

    m4return_t	iResult ;
    m4uint32_t	iIndex ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_INDEX_ITEMS_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_poIndexItem->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_INDEX_ITEMS_ID_INDEX, M4MDFAC_ITEM_ID_INDEX, ai_poRegister, 0, M4MDFAC_MAX_INT_32 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iIndex = m4uint32_t( fValue ) ;
	CHECK_CHLOG_ERRORF( iIndex == 0, M4_ERROR, M4_MDFAC_ZERO_INDEX, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;

	ao_poIndexItem->SetIndexId( iIndex ) ;


			
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_INDEX_ITEMS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iResult = ao_poIndexItem->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_INDEX_ITEMS_SEARCH_ORDER, M4MDFAC_ITEM_SEARCH_ORDER, ai_poRegister, 0, M4MDFAC_MAX_INT_8 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poIndexItem->SetOrder( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_EQUAL_FUNCTION, M4MDFAC_INDEX_ITEMS_ID_FUNCTION, M4MDFAC_ITEM_ID_FUNCTION, ai_poRegister, M4CL_EQUAL_FUNCTION, M4CL_SMALLER_OR_EQUAL_FUNCTION_OR_NULL ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poIndexItem->SetFunction( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_INDEX_WAY_ASCENDING, M4MDFAC_INDEX_ITEMS_WAY, M4MDFAC_ITEM_WAY, ai_poRegister, M4CL_INDEX_WAY_ASCENDING, M4CL_INDEX_WAY_DESCENDING ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_INDEX_WAY_ASCENDING ;
	}
	else
	{
		iValue = M4CL_INDEX_WAY_DESCENDING ;
	}

    ao_poIndexItem->SetWay( iValue ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_INDEX_ITEMS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOnePlugFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempPlug *ao_poPlug )
{

    m4return_t	iResult ;
	m4pcchar_t	pccUsedT3Id ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECTORS_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poPlug->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		

		
	iResult = M4ReadString( ao_poChannel, pccUsedT3Id, "", M4MDFAC_T3_CONNECTORS_ID_T3_USED, M4MDFAC_ITEM_ID_T3_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poPlug->SetUsedT3Id( pccUsedT3Id ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_T3_CONNECTORS_N_T3, M4MDFAC_ITEM_N_T3, ai_poRegister, M4MDFAC_NODE_SYS_T3_CONNECTORS_TRANS, M4MDFAC_T3_CONNECTORS_TRANS_N_T3, pccUsedT3Id ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poPlug->SetUsedT3Name( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECTORS_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poPlug->SetUsedNodeId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_NODE_CONNECTOR_TYPE_RB, M4MDFAC_T3_CONNECTORS_ID_CONNECTION_TYPE, M4MDFAC_ITEM_ID_CONNECTION_TYPE, ai_poRegister, M4CL_NODE_CONNECTOR_TYPE_RB, M4CL_NODE_CONNECTOR_TYPE_RB ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poPlug->SetType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3_CONNECTORS_IS_FILTER, M4MDFAC_ITEM_IS_FILTER, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poPlug->SetIsFilter( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3_CONNECTORS_ID_RELATION_TYPE, M4MDFAC_ITEM_ID_RELATION_TYPE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poPlug->SetRelationTypeId( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3_CONNECTORS_IS_MULTISELECTION, M4MDFAC_ITEM_IS_MULTISELECTION, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else
	{
		iValue = 1 ;
	}

    ao_poPlug->SetIsMultiselection( iValue ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_T3_CONNECTORS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOnePlugItemFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempPlugItem *ao_poPlugItem )
{

    m4return_t	iResult ;

	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECT_ITEM_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poPlugItem->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	


	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECT_ITEM_ID_T3_USED, M4MDFAC_ITEM_ID_T3_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poPlugItem->SetUsedT3Id( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECT_ITEM_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poPlugItem->SetUsedNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECT_ITEM_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poPlugItem->SetUsedItemId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_CONNECT_ITEM_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poPlugItem->SetItemId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ITEM_CONNECTOR_EQUALITY, M4MDFAC_T3_CONNECT_ITEM_ID_RELSHIP_TYPE, M4MDFAC_ITEM_ID_RELSHIP_TYPE, ai_poRegister, M4CL_ITEM_CONNECTOR_EQUALITY, M4CL_ITEM_CONNECTOR_DISTINCT ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poPlugItem->SetRelation( m4uint8_t( fValue ) ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_T3_CONNECT_ITEM_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneAliasFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempAlias *ao_poAlias )
{

    m4return_t	iResult ;
    ClTempNode	*poUsedNode ;

	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ALIAS_RES_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poAlias->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ALIAS_RES_ALIAS, M4MDFAC_ITEM_ALIAS, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poAlias->SetAliasId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ALIAS_RES_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    poUsedNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poUsedNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Alias Resolution" ) ;

    ao_poAlias->SetUsedNode( poUsedNode ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_ALIAS_RES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    
    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneT3AliasFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempT3Alias *ao_poT3Alias )
{

    m4return_t      iResult ;
	m4uint16_t		iLevel ;
	ClTempT3Inherit	*poT3Inherit ;

    m4uint8_t		iValue ;
	m4double_t		fValue ;
	m4pcchar_t		pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poT3Alias->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_ALIAS, M4MDFAC_ITEM_ALIAS, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poT3Alias->SetT3AliasId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_INSTANCE, M4MDFAC_ITEM_INSTANCE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poT3Alias->SetInstanceId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_ID_T3_USED, M4MDFAC_ITEM_ID_T3_USED, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poT3Alias->SetT3Id( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_ID_ROLE, M4MDFAC_ITEM_ID_ROLE, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poT3Alias->SetRoleId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_T3_ALIAS_RES_ID_ORGANIZATION, M4MDFAC_ITEM_ID_ORGANIZATION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poT3Alias->SetOrgId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED, M4MDFAC_T3_ALIAS_RES_ID_SHARED_POLICY, M4MDFAC_ITEM_ID_SHARED_POLICY, ai_poRegister, M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED, M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 3.0 )
	{
		iValue = M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED ;
	}
	else
	{
		iValue = M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED ;
	}

    ao_poT3Alias->SetScope( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_T3ALIAS_OPEN_MODE_NO_LOAD, M4MDFAC_T3_ALIAS_RES_ID_OPEN_MODE_TP, M4MDFAC_ITEM_ID_OPEN_MODE_TP, ai_poRegister, M4CL_T3ALIAS_OPEN_MODE_NO_LOAD, M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_NODESAYS ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poT3Alias->SetOpenMode( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_CSTYPE_L2_SITE, M4MDFAC_T3_ALIAS_RES_CS_ACCESS, M4MDFAC_ITEM_CS_ACCESS, ai_poRegister, M4CL_CSTYPE_L2_MIN_NUMBER, M4CL_CSTYPE_L2_MAX_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poT3Alias->SetCsAccess( m4uint8_t( fValue ) ) ;


	
	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_ORGANIZATION_L2_TYPE_FATHER, M4MDFAC_T3_ALIAS_RES_ID_ORGANIZATION_TYPE, M4MDFAC_ITEM_ORGANIZATION_TYPE, ai_poRegister, M4CL_ORGANIZATION_L2_TYPE_NONE, M4CL_ORGANIZATION_L2_TYPE_ANY ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    ao_poT3Alias->SetOrgType( m4uint8_t( fValue ) ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_T3_ALIAS_RES_ID_LEVEL, M4MDFAC_ITEM_ID_LEVEL, ai_poRegister, 0, M4MDFAC_MAX_INT_16 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	iLevel = m4uint16_t( fValue ) ;

	poT3Inherit = ao_poChannel->GetT3InheritByLevel( iLevel ) ;
	CHECK_CHLOG_ERRORF( poT3Inherit == NULL, M4_ERROR, M4_MDFAC_BAD_INHERIT_LEVEL, iLevel << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Node Inherits" ) ;

    ao_poT3Alias->SetLevel( poT3Inherit->GetIndex() ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_T3_ALIAS_RES_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    
    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneFilterFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempFilter *ao_poFilter )
{

    m4return_t	iResult ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poFilter->SetNodeId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_ID_FILTER, M4MDFAC_ITEM_ID_FILTER, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poFilter->SetFilterId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_ID_T3_FILTER, M4MDFAC_ITEM_ID_T3_FILTER, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( *pccValue == '\0' )
	{
		pccValue = ao_poChannel->GetChannelId() ;
	}

    iResult = ao_poFilter->SetUsedT3Id( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_ID_NODE_FILTER, M4MDFAC_ITEM_ID_NODE_FILTER, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poFilter->SetUsedNodeId( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_FILTER_TYPE_QBF, M4MDFAC_FILTERS_FILTER_TYPE, M4MDFAC_ITEM_FILTER_TYPE, ai_poRegister, M4CL_FILTER_TYPE_QBF, M4CL_FILTER_TYPE_FIND ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = M4CL_FILTER_TYPE_QBF ;
	}
	else if( fValue == 2.0 )
	{
		iValue = M4CL_FILTER_TYPE_MAPI ;
	}
	else
	{
		iValue = M4CL_FILTER_TYPE_FIND ;
	}

    ao_poFilter->SetType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, M4CL_FILTER_BEHAVIOUR_QBF, M4MDFAC_FILTERS_ID_BEHAVIOUR_TYPE, M4MDFAC_ITEM_ID_BEHAVIOUR_TYPE, ai_poRegister, M4CL_FILTER_BEHAVIOUR_QBF, M4CL_FILTER_BEHAVIOUR_ALL ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = M4CL_FILTER_BEHAVIOUR_QBF ;
	}
	else if( fValue == 1.0 )
	{
		iValue = M4CL_FILTER_BEHAVIOUR_MULTI_ALL ;
	}
	else
	{
		iValue = M4CL_FILTER_BEHAVIOUR_ALL ;
	}

    ao_poFilter->SetBehaviourType( iValue ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 1, M4MDFAC_FILTERS_IS_OWN_NODE, M4MDFAC_ITEM_IS_OWN_NODE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else
	{
		iValue = 0 ;
	}

    ao_poFilter->SetIsOwnNode( iValue ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_SYS_SENTENCE_ITEM, M4MDFAC_ITEM_SYS_SENTENCE_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poFilter->SetSysSentenceItem( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_FILTERS_SYS_PARAM_ITEM, M4MDFAC_ITEM_SYS_PARAM_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poFilter->SetSysParamItem( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_FILTERS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneBusinessMethodFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempBusinessMethod *ao_poBusinessMethod )
{

    m4return_t	iResult ;
	m4pcchar_t	pccMethodId ;

	m4pcchar_t	pccValue ;
	ClTempNitem	*poNitem ;
	ClTempNode	*poNode ;



	iResult = M4ReadString( ao_poChannel, pccMethodId, "", M4MDFAC_BUSINESS_METHODS_ID_METHOD, M4MDFAC_ITEM_ID_METHOD, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poBusinessMethod->SetMethodId( pccMethodId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_BUSINESS_METHODS_NM_METHOD, M4MDFAC_ITEM_XML_METHOD_NAME, ai_poRegister, M4MDFAC_NODE_SYS_BUSINESS_METHODS_TRANS, M4MDFAC_BUSINESS_METHODS_TRANS_NM_METHOD, pccMethodId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poBusinessMethod->SetName( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = _ReadName( ao_poChannel, pccValue, M4_TRUE, M4MDFAC_BUSINESS_METHODS_DESC_METHOD, M4MDFAC_ITEM_XML_DESCRIPTION, ai_poRegister, M4MDFAC_NODE_SYS_BUSINESS_METHODS_TRANS, M4MDFAC_BUSINESS_METHODS_TRANS_DESC_METHOD, pccMethodId ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iResult = ao_poBusinessMethod->SetDescription( pccValue ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_BUSINESS_METHODS_ID_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	poNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Business Method" ) ;


	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_BUSINESS_METHODS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	poNitem = poNode->GetNitemById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Business Method" ) ;

    ao_poBusinessMethod->SetNitem( poNitem ) ;



	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_BUSINESS_METHODS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneBusinessArgFromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempBusinessArg *ao_poBusinessArg )
{

    m4return_t	iResult ;

    m4uint8_t	iValue ;
	m4double_t	fValue ;
	m4pcchar_t	pccValue ;

	ClTempNode	*poNode ;



	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_BUSINESS_ARGS_ID_METHOD, M4MDFAC_ITEM_ID_METHOD, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = ao_poBusinessArg->SetMethodId( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
		
	iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_BUSINESS_ARGS_ID_ARGUMENT_NODE, M4MDFAC_ITEM_ID_NODE, ai_poRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	poNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Business Argument" ) ;

    ao_poBusinessArg->SetNode( poNode ) ;



	iResult = M4ReadNumber( ao_poChannel, fValue, 2, M4MDFAC_BUSINESS_ARGS_ID_ARGUMENT_TYPE, M4MDFAC_ITEM_ID_ARGUMENT_TYPE, ai_poRegister, 0, 2 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	if( fValue == 0.0 )
	{
		iValue = 0 ;
	}
	else if( fValue == 1.0 )
	{
		iValue = 1 ;
	}
	else
	{
		iValue = 2 ;
	}

    ao_poBusinessArg->SetType( iValue ) ;

	
	
	iResult = M4ReadVersion( ao_poChannel, M4MDFAC_BUSINESS_ARGS_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    return( M4_SUCCESS ) ;
}



// Lectura plana de access ====================================================

m4return_t  ClFactoryImp :: _ReadT3FromAccess( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister )
{

    m4return_t	iResult ;


// T3 =========================================================================

    iResult = _ReadOneT3FromAccess( ao_poChannel, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// T3 inherits ================================================================

    iResult = _ReadT3InheritsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Sentences ==================================================================

    iResult = _ReadSentencesFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// RSMs =======================================================================

    iResult = _ReadRSMsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Tis ========================================================================

    iResult = _ReadTisFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Creamos las referencias de TIs =============================================

	iResult = ao_poChannel->CreateTiReferences() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Ti Inherits ================================================================

    iResult = _ReadTiInheritsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Items ======================================================================

    iResult = _ReadItemsFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// ItemInherits ===================================================================

    iResult = _ReadItemInheritsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Concepts ===================================================================

    iResult = _ReadConceptsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Arguments ==================================================================

    iResult = _ReadArgumentsFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Rules ======================================================================

    iResult = _ReadRulesFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Tags =======================================================================

    iResult = _ReadTagsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Triggers ===================================================================

    iResult = _ReadTriggersFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Totals =====================================================================

    iResult = _ReadTotalsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Total Tags =================================================================

    iResult = _ReadTotalTagsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Nodes ======================================================================

    iResult = _ReadNodesFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	
// Node Inherits ==============================================================

    iResult = _ReadNodeInheritsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Node Reads =================================================================

    iResult = _ReadNodeReadsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Conectores =================================================================

    iResult = _ReadConnectorsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Conectores de item =========================================================

    iResult = _ReadItemConnsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// ============================================================================
// Añadimos la herencia a los items ===========================================
// ============================================================================

	iResult = ao_poChannel->FillItemsWithInheritance() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Item connector mappings ====================================================

    iResult = _ReadArgumentMappingsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Indices ====================================================================

    iResult = _ReadIndexesFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Items de indice ============================================================

    iResult = _ReadIndexItemsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Carriles ===================================================================

    iResult = _ReadPlugsFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Items de carril ============================================================

    iResult = _ReadItemPlugsFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Alias ======================================================================

    iResult = _ReadAliasFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// T3 Alias ===================================================================

    iResult = _ReadT3AliasFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Filtros ====================================================================

    iResult = _ReadFiltersFromAccess( ao_poChannel, &m_oAccessUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Comportamiento cliente servidor ============================================

    iResult = _ReadItemCsDescFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	
// Objecto de negocio =========================================================

    iResult = _ReadBusinessServiceFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Métodos de negocio =========================================================

    iResult = _ReadBusinessMethodsFromAccess( ao_poChannel, &m_oAccessUpperPool, &m_oAccessNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Argumentos de métodos de negocio =========================================================

    iResult = _ReadBusinessArgsFromAccess( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// ============================================================================

    return( M4_SUCCESS ) ;
}





m4return_t  ClFactoryImp :: _ReadT3InheritsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
	m4uint8_t			iLevel ;
    m4uint16_t          i, j ;
    m4uint16_t          iLength ;
    m4uint16_t          iSize ;
    m4uint16_t          iInherits ;
	m4pcchar_t			pccT3Id ;
	m4pcchar_t			pccStoredT3Id ;
	ClTempT3Inherit		*poT3Inherit ;
	ClTempT3Inherit		*poStoredT3Inherit ;
	ClTempT3Inherit		*poT3InheritsArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_T3S_INHERIT, "M4Obj Inheritance", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	// El máximo de niveles de herencia son 200
	CHECK_CHLOG_ERRORF( iLength > M4MDFAC_MAX_T3_INHERITANCE, M4_ERROR, M4_MDFAC_TOO_T3_INHERITANCES, LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << iLength << M4MDFAC_MAX_T3_INHERITANCE ) ;


	iSize = iLength + 1 ;

	if( iSize > 0 )
	{
		poT3InheritsArray = new ClTempT3Inherit[ iSize ] ;
		CHECK_CHLOG_ERRORF( poT3InheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempT3Inherit ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetT3InheritsArray( poT3InheritsArray ) ;
		ao_poChannel->SetT3InheritsDelta( iSize ) ;


// Ponemos el propio M4Obj a nivel 0 ==========================================

		poT3Inherit = &( poT3InheritsArray[ 0 ] ) ;
		poT3Inherit->SetUpperPool( ao_poPool ) ;


		iResult = poT3Inherit->SetBaseT3Id( ao_poChannel->GetChannelId() ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poT3Inherit->SetLevel( 0 ) ;


		// La ponemos en el array global
       	iResult = ao_poChannel->AddT3Inherit( poT3Inherit ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poT3Inherit = &( poT3InheritsArray[ i + 1 ] ) ;

			poT3Inherit->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneT3InheritFromAccess( ao_poChannel, poRegister, poT3Inherit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			iLevel = poT3Inherit->GetLevel() ;

			// Los de nivel 0 no las insertamos
			if( iLevel != 0 )
			{
				// Comprobamos que no esté ya el T3, porque sino hay ciclos
				pccT3Id = poT3Inherit->GetBaseT3Id() ;

				iInherits = ao_poChannel->GetNumberOfT3Inherits() ;

				for( j = 0 ; j < iInherits ; j++ )
				{
					poStoredT3Inherit = ao_poChannel->GetT3InheritByPosition( j ) ;
					CHECK_CHLOG_DEBUGF( poStoredT3Inherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					pccStoredT3Id = poStoredT3Inherit->GetBaseT3Id() ;
					CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccStoredT3Id, pccT3Id ) == 0, M4_ERROR, M4_MDFAC_T3_INHERIT_CYCLE, pccT3Id << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
				}


				// La ponemos en el array global
       			iResult = ao_poChannel->AddT3Inherit( poT3Inherit ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		ao_poChannel->OrderT3Inherits() ;


		// Establecemos los índices
		for( i = 0 ; i < iSize ; i++ )
		{
			poT3Inherit = ao_poChannel->GetT3InheritByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poT3Inherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poT3Inherit->SetIndex( m4uint8_t( i ) ) ;
		}
	}


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadSentencesFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempSentence      *poSentence ;
    ClTempSentence      *poSentencesArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_SENTENCES, "Sentences", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poSentencesArray = new ClTempSentence[ iLength ] ;
		CHECK_CHLOG_ERRORF( poSentencesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempSentence ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetSentencesArray( poSentencesArray ) ;
		ao_poChannel->SetSentencesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

//		pccOldSentence = NULL ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poSentence = &( poSentencesArray[ i ] ) ;

			poSentence->SetUpperPool( ao_poUpperPool ) ;
			poSentence->SetNoUpperPool( ao_poNoUpperPool ) ;


			iResult = _ReadOneSentenceFromAccess( ao_poChannel, poRegister, poSentence ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

/*
			Para la chapuza de los campos long, que puede venir la sentencia repetida
			Pasamos. Si está repetida, pues mala suerte, tampoco pasa nada

			m4uint8_t			iInsert ;
			m4pcchar_t			pccSentence ;
			m4pcchar_t			pccOldSentence ;

			iInsert = 1 ;

			pccSentence = poSentence->GetSentenceId() ;

			if( pccSentence != NULL && pccOldSentence != NULL )
			{
				if( M4MdFacStrcmp( pccSentence, pccOldSentence ) == 0 )
				{
					iInsert = 0 ;
				}
			}

			if( iInsert == 1 )
			{
				// La ponemos en el array global
       			iResult = ao_poChannel->AddSentence( poSentence ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
*/

			// La ponemos en el array global
       		iResult = ao_poChannel->AddSentence( poSentence ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderSentences() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadRSMsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
	ClTempRSM			*poRSM ;
	ClTempRSM			*poRSMsArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_RSMS, "RSMs", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poRSMsArray = new ClTempRSM[ iLength ] ;
		CHECK_CHLOG_ERRORF( poRSMsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempRSM ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetRSMsArray( poRSMsArray ) ;
		ao_poChannel->SetRSMsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poRSM = &( poRSMsArray[ i ] ) ;

			poRSM->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneRSMFromAccess( ao_poChannel, poRegister, poRSM ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddRSM( poRSM ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderRSMs() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadTisFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempTi			*poTi ;
	ClTempTi			*poTisArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_TIS, "Node Strunctures", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poTisArray = new ClTempTi[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTisArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTi ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTisArray( poTisArray ) ;
		ao_poChannel->SetTisDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTi = &( poTisArray[ i ] ) ;

			poTi->SetUpperPool( ao_poUpperPool ) ;
			poTi->SetNoUpperPool( ao_poNoUpperPool ) ;

			iResult = _ReadOneTiFromAccess( ao_poChannel, poRegister, poTi, ao_poUpperPool, ao_poNoUpperPool ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTi( poTi ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTis() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadTiInheritsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
	m4uint8_t			iLevel ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTis ;
    m4uint16_t			iSize ;
    ClTempTi			*poTi ;
    ClTempTiInherit		*poTiInherit ;
	ClTempTiInherit		*poTiInheritsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_TIS_INHERIT, "Tis Inheritance", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	iTis = ao_poChannel->GetNumberOfTis() ;
	iSize = iTis + iLength ;

	if( iSize > 0 )
	{
		poTiInheritsArray = new ClTempTiInherit[ iSize ] ;
		CHECK_CHLOG_ERRORF( poTiInheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTiInherit ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTiInheritsArray( poTiInheritsArray ) ;
		ao_poChannel->SetTiInheritsDelta( iSize ) ;


// Ponemos las propias Tis a nivel 0 ==========================================

		for( i = 0 ; i < iTis ; i++ )
		{
			poTiInherit = &( poTiInheritsArray[ i ] ) ;
			poTi = ao_poChannel->GetTiByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			iResult = poTiInherit->SetTiId( poTi->GetTiId(), M4_FALSE ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poTiInherit->SetBaseTi( poTi ) ;
			poTiInherit->SetLevel( 0 ) ;


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTiInherit( poTiInherit ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTiInherit = &( poTiInheritsArray[ i + iTis ] ) ;


			iResult = _ReadOneTiInheritFromAccess( ao_poChannel, poRegister, poTiInherit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			iLevel = poTiInherit->GetLevel() ;

			// Las de nivel 0 no las insertamos
			if( iLevel != 0 )
			{
				// La ponemos en el array global
       			iResult = ao_poChannel->AddTiInherit( poTiInherit ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		ao_poChannel->OrderTiInherits() ;


// Asignamos los ti inherits a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithInherits() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadItemsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempItem			*poItem ;
    ClTempItem			*poItemsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ITEMS, "Items", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poItemsArray = new ClTempItem[ iLength ] ;
		CHECK_CHLOG_ERRORF( poItemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItem ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetItemsArray( poItemsArray ) ;
		ao_poChannel->SetItemsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poItem = &( poItemsArray[ i ] ) ;

			// Inicializamos el item
			poItem->SetUpperPool( ao_poUpperPool ) ;
			poItem->SetNoUpperPool( ao_poNoUpperPool ) ;


			iResult = _ReadOneItemFromAccess( ao_poChannel, poRegister, poItem ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddItem( poItem ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderItems() ;


// Asignamos los items a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithItems() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadItemInheritsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempItemInherit	*poItemInherit ;
	ClTempItemInherit	*poItemInheritsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ITEMS_INHERIT, "Items Inheritance", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poItemInheritsArray = new ClTempItemInherit[ iLength ] ;
		CHECK_CHLOG_ERRORF( poItemInheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItemInherit ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetItemInheritsArray( poItemInheritsArray ) ;
		ao_poChannel->SetItemInheritsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poItemInherit = &( poItemInheritsArray[ i ] ) ;


			iResult = _ReadOneItemInheritFromAccess( ao_poChannel, poRegister, poItemInherit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddItemInherit( poItemInherit ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderItemInherits() ;


// Asignamos la sobrescritura de items a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithItemInherits() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadConceptsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempConcept		*poConcept ;
	ClTempConcept		*poConceptsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_CONCEPTS, "Concepts", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poConceptsArray = new ClTempConcept[ iLength ] ;
		CHECK_CHLOG_ERRORF( poConceptsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempConcept ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetConceptsArray( poConceptsArray ) ;
		ao_poChannel->SetConceptsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poConcept = &( poConceptsArray[ i ] ) ;


			iResult = _ReadOneConceptFromAccess( ao_poChannel, poRegister, poConcept ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddConcept( poConcept ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderConcepts() ;


// Asignamos los conceptos a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithConcepts() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadArgumentsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempArgument      *poArgument ;
    ClTempArgument      *poArgumentsArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ITEM_ARGS, "Item Arguments", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poArgumentsArray = new ClTempArgument[ iLength ] ;
		CHECK_CHLOG_ERRORF( poArgumentsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempArgument ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetArgumentsArray( poArgumentsArray ) ;
		ao_poChannel->SetArgumentsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poArgument = &( poArgumentsArray[ i ] ) ;

			// Inicializamos el argumento
			poArgument->SetUpperPool( ao_poUpperPool ) ;
			poArgument->SetNoUpperPool( ao_poNoUpperPool ) ;


			iResult = _ReadOneArgumentFromAccess( ao_poChannel, poRegister, poArgument ) ;

			if( iResult != M4_SUCCESS )
			{
				return( iResult ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddArgument( poArgument ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderArguments() ;


// Asignamos los argumentos a los items =======================================

		iResult = ao_poChannel->FillItemsWithArguments() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadRulesFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t			iResult ;
	m4uint8_t			iType ;
	m4uint8_t			iInsert ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
	m4pcchar_t			pccString ;
    ClTempItemFormula	*poItemFormula ;
    ClTempItemFormula	*poItemFormulasArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_RULES, "Rules", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poItemFormulasArray = new ClTempItemFormula[ iLength ] ;
		CHECK_CHLOG_ERRORF( poItemFormulasArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItemFormula ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetItemFormulasArray( poItemFormulasArray ) ;
		ao_poChannel->SetItemFormulasDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poItemFormula = &( poItemFormulasArray[ i ] ) ;


			iResult = _ReadOneRuleFromAccess( ao_poChannel, poRegister, poItemFormula ) ;

			if( iResult != M4_SUCCESS )
			{
				return( iResult ) ;
			}

			// Controlamos su validez
			iInsert = 1 ;

			if( poItemFormula->GetIsRetroactivity() == 0 )
			{
				if( poItemFormula->GetBatchType() == 2 )
				{
					// Si no es de retoractividad y es total
					iInsert =  0;
				}
				else if( poItemFormula->GetIsMetarule() == 1 && poItemFormula->GetIsBatchMetarule() == 1 )
				{
					// Si no es de retoractividad y es metaregla y generada por el sistema
					iInsert =  0;
				}
			}
			else
			{
				iType = poItemFormula->GetType() ;

				switch( iType )
				{
					case M4CL_LENGUAGE_LN4 :

						pccString = poItemFormula->GetPolish() ;

						if( pccString == NULL || *pccString == '\0' )
						{
							iInsert =  0 ;
						}
						break ;

					case M4CL_LENGUAGE_CPP :

						pccString = poItemFormula->GetMethod() ;

						if( pccString == NULL || *pccString == '\0' )
						{
							iInsert =  0 ;
						}
						break ;
    
					default :
						break ;
				}
			}

			if( iInsert == 1 )
			{
				// Lo ponemos en el array global
       			iResult = ao_poChannel->AddItemFormula( poItemFormula ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		ao_poChannel->OrderItemFormulas() ;


// Asignamos las formulas a las TIs ===========================================

		iResult = ao_poChannel->FillTisWithItemFormulas() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadTagsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempTag			*poTag ;
	ClTempTag			*poTagsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_TAGS, "Tags", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poTagsArray = new ClTempTag[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTagsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTag ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTagsArray( poTagsArray ) ;
		ao_poChannel->SetTagsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTag = &( poTagsArray[ i ] ) ;


			iResult = _ReadOneTagFromAccess( ao_poChannel, poRegister, poTag ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTag( poTag ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTags() ;


// Asignamos los totales a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithTags() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadTriggersFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempTrigger       *poTrigger ;
    ClTempTrigger       *poTriggersArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ITEM_TRIGGERS, "Node Structure Connector Items", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poTriggersArray = new ClTempTrigger[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTriggersArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTrigger ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTriggersArray( poTriggersArray ) ;
		ao_poChannel->SetTriggersDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTrigger = &( poTriggersArray[ i ] ) ;


			iResult = _ReadOneTriggerFromAccess( ao_poChannel, poRegister, poTrigger ) ;

			if( iResult != M4_SUCCESS )
			{
				return( iResult ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddTrigger( poTrigger ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTriggers() ;


// Asignamos los triggers a las TIs ===========================================

		iResult = ao_poChannel->FillTisWithTriggers() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadTotalsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempTotal			*poTotal ;
	ClTempTotal			*poTotalsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_TOTALS, "Totals", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poTotalsArray = new ClTempTotal[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTotalsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTotal ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTotalsArray( poTotalsArray ) ;
		ao_poChannel->SetTotalsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTotal = &( poTotalsArray[ i ] ) ;


			iResult = _ReadOneTotalFromAccess( ao_poChannel, poRegister, poTotal ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTotal( poTotal ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTotals() ;


// Asignamos los totales a las TIs ============================================

		iResult = ao_poChannel->FillTisWithTotals() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadTotalTagsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempTotalTag		*poTotalTag ;
	ClTempTotalTag		*poTotalTagsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_TOTAL_TAGS, "Total Tags", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poTotalTagsArray = new ClTempTotalTag[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTotalTagsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTotal ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTotalTagsArray( poTotalTagsArray ) ;
		ao_poChannel->SetTotalTagsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poTotalTag = &( poTotalTagsArray[ i ] ) ;


			iResult = _ReadOneTotalTagFromAccess( ao_poChannel, poRegister, poTotalTag ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTotalTag( poTotalTag ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTotalTags() ;


// Asignamos las condiciones de total a las TIs ===============================

		iResult = ao_poChannel->FillTisWithTotalTags() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadNodesFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
	m4pcchar_t			pccNodeId ;
	m4pcchar_t			pccLastNodeId ;
    ClTempNode          *poNode ;
    ClTempNode          *poNodesArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_NODES, "Nodes", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poNodesArray = new ClTempNode[ iLength ] ;
		CHECK_CHLOG_ERRORF( poNodesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNode ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNodesArray( poNodesArray ) ;
		ao_poChannel->SetNodesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poNode = &( poNodesArray[ i ] ) ;

			poNode->SetUpperPool( ao_poUpperPool ) ;
			poNode->SetNoUpperPool( ao_poNoUpperPool ) ;

			poNode->SetChannel( ao_poChannel ) ;


			iResult = _ReadOneNodeFromAccess( ao_poChannel, poRegister, poNode ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddNode( poNode ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderNodes() ;


		// Controlamos que no haya nodos repetidos

		pccLastNodeId = NULL ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poNode = ao_poChannel->GetNodeByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccNodeId = poNode->GetNodeId() ;

			if( pccLastNodeId != NULL && pccNodeId != NULL )
			{
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccLastNodeId, pccNodeId ) == 0, M4_ERROR, M4_MDFAC_REPEATED_NODE, LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
			}

			pccLastNodeId = pccNodeId ;
		}
	}


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadNodeInheritsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iNodes ;
    m4uint16_t			iSize ;
    ClTempNode			*poNode ;
    ClTempNodeInherit	*poNodeInherit ;
	ClTempNodeInherit	*poNodeInheritsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_NODES_INHERIT, "Nodes Inheritance", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	iNodes = ao_poChannel->GetNumberOfNodes() ;
	iSize = iNodes + iLength ;

	if( iSize > 0 )
	{
		poNodeInheritsArray = new ClTempNodeInherit[ iSize ] ;
		CHECK_CHLOG_ERRORF( poNodeInheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNodeInherit ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNodeInheritsArray( poNodeInheritsArray ) ;
		ao_poChannel->SetNodeInheritsDelta( iSize ) ;


// Ponemos los propios nodos al nivel del nodo ================================

		for( i = 0 ; i < iNodes ; i++ )
		{
			poNodeInherit = &( poNodeInheritsArray[ i ] ) ;
			poNode = ao_poChannel->GetNodeByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			iResult = poNodeInherit->SetNodeId( poNode->GetNodeId(), M4_FALSE ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNodeInherit->SetTi( poNode->GetTi() ) ;
			poNodeInherit->SetLevel( poNode->GetLevel() ) ;


			// La ponemos en el array global
       		iResult = ao_poChannel->AddNodeInherit( poNodeInherit ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poNodeInherit = &( poNodeInheritsArray[ i + iNodes ] ) ;


			iResult = _ReadOneNodeInheritFromAccess( ao_poChannel, poRegister, poNodeInherit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddNodeInherit( poNodeInherit ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderNodeInherits() ;


// Asignamos los node inherits a los nodos ====================================

		iResult = ao_poChannel->FillNodesWithInherits() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}


// Añadimos los items a los nodos =============================================

		iResult = ao_poChannel->FillNodesWithItems() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadNodeReadsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempNitem			*poNitem ;
    ClTempNode			*poNode ;
    ClTempNode			*poRevNode ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;

	m4pcchar_t			pccValue ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_NODES_READ, "Node Reads", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;


		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_READS_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}


		poNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Node Reads" ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_READS_ID_BACKPAY_ITEM, M4MDFAC_ITEM_ID_BACKPAY_ITEM, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( pccValue != NULL && *pccValue != '\0' )
		{
			poNitem = poNode->GetNitemById( pccValue, M4_TRUE ) ;

			if( poNitem == NULL )
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Back Pay Item" ) ;
			}
		}
		else
		{
			poNitem = NULL ;
		}

		poNode->SetBackPayNitem( poNitem ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_READS_ID_NODE_DATES, M4MDFAC_ITEM_ID_NODE_DATES, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( pccValue != NULL && *pccValue != '\0' )
		{
			poRevNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;

			if( poRevNode == NULL )
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Revision Node" ) ;
			}
		}
		else
		{
			poRevNode = NULL ;
		}

		poNode->SetRevNode( poRevNode ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_NODE_READS_ID_REV_ITEM, M4MDFAC_ITEM_ID_REV_ITEM, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( pccValue != NULL && *pccValue != '\0' )
		{
			if( poRevNode != NULL )
			{
				poNitem = poRevNode->GetNitemById( pccValue, M4_TRUE ) ;

				if( poNitem == NULL )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poRevNode->iNode(), poRevNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Revision Item" ) ;
				}
			}
			else
			{
				poNitem = NULL ;
			}
		}
		else
		{
			poNitem = NULL ;
		}

		poNode->SetRevNitem( poNitem ) ;



		iResult = M4ReadVersion( ao_poChannel, M4MDFAC_NODE_READS_VERSION, M4MDFAC_ITEM_VERSION, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadConnectorsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t			iResult ;
    m4uint8_t           iType ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iNodes ;
    m4uint16_t			iSize ;
	m4pcchar_t			pccFatherNodeId ;
    ClTempNode			*poNode ;
    ClTempConnector		*poConnector ;
	ClTempConnector		*poConnectorsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_CONNECTORS, "Connectors", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	iNodes = ao_poChannel->GetNumberOfNodes() ;
	iSize = iNodes + iLength ;

	if( iSize > 0 )
	{
		poConnectorsArray = new ClTempConnector[ iSize ] ;
		CHECK_CHLOG_ERRORF( poConnectorsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempConnector ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetConnectorsArray( poConnectorsArray ) ;
		ao_poChannel->SetConnectorsDelta( iSize ) ;


// Ponemos los autoconectores =================================================

		for( i = 0 ; i < iNodes ; i++ )
		{
			poConnector = &( poConnectorsArray[ i ] ) ;
			poConnector->SetUpperPool( ao_poPool ) ;

			poNode = ao_poChannel->GetNodeByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poConnector->SetFatherNode( poNode ) ;
			poConnector->SetSonNode( poNode ) ;
			poConnector->SetType( M4CL_NODE_CONNECTOR_TYPE_AUTO ) ;
			poConnector->SetExpandDelete( 0 ) ;
			poConnector->SetExpandUndo( 0 ) ;
			poConnector->SetSentence( NULL ) ;

			pccFatherNodeId = poNode->GetNodeId() ;
			poConnector->SetFatherNodeId( pccFatherNodeId, M4_FALSE ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// La ponemos en el array global
       		iResult = ao_poChannel->AddConnector( poConnector ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poConnector = &( poConnectorsArray[ i + iNodes ] ) ;

			poConnector->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneConnectorFromAccess( ao_poChannel, poRegister, poConnector ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			iType = poConnector->GetType() ;

			// Los autoconectados no les ponemos
			if( iType != M4CL_NODE_CONNECTOR_TYPE_AUTO )
			{
				// Lo ponemos en el array global de directos e inversos
       			iResult = ao_poChannel->AddConnector( poConnector ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

       			iResult = ao_poChannel->AddInvConnector( poConnector ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		ao_poChannel->OrderConnectors() ;
		ao_poChannel->OrderInvConnectors() ;


// Asignamos los conectores a los nodos =============================================

		iResult = ao_poChannel->FillNodesWithConnectors() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadItemConnsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTriggers ;
    m4uint16_t			iSize ;
    ClTempNitemConn		*poNitemConn ;
    ClTempNitemConn		*poNitemConnArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_CONNECTOR_ITEM, "Connector Items", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	// Como mucho tennemos los conectores de los triggers
	iTriggers = ao_poChannel->GetNumberOfTriggers() ;
	iSize = iLength + iTriggers ;

	if( iSize > 0 )
	{
		poNitemConnArray = new ClTempNitemConn[ iSize ] ;
		CHECK_CHLOG_ERRORF( poNitemConnArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemConn ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNitemConnsArray( poNitemConnArray ) ;
		ao_poChannel->SetNitemConnsDelta( iLength ) ;

		// Ponemos los que hemos creado
		ao_poChannel->SetNitemConnsArraySize( iSize ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poNitemConn = &( poNitemConnArray[ i ] ) ;

			poNitemConn->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneItemConnFromAccess( ao_poChannel, poRegister, poNitemConn ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddNitemConn( poNitemConn ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderNitemConns() ;


// Añadimos los conectores a los nitems =======================================

		iResult = ao_poChannel->FillItemsWithConnectors() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



// ESTA FALTA OPTIMIZAR. Pasamos porque no se utiliza

m4return_t	ClFactoryImp :: _ReadArgumentMappingsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint8_t			iSonOrder ;
    m4uint8_t			iStoredSonOrder ;
    m4uint16_t			i, j ;
    m4uint16_t			iLength ;
    m4uint16_t			iMappings ;
	m4pcchar_t			pccArgumentId ;
    ClTempNode			*poFatherNode ;
    ClTempNode			*poSonNode ;
    ClTempNitem			*poFatherNitem ;
    ClTempNitem			*poSonNitem ;
    ClTempRitem			*poFatherRitem ;
    ClTempItem			*poFatherItem ;
    ClTempItem			*poSonItem ;
    ClTempNitemConn		*poNitemConn ;
    ClTempArgument		*poArgument ;
    StArgumentMappig_t	*pstMapping ;
    StArgumentMappig_t	*pstStoredMapping ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;

	m4pcchar_t			pccValue ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_CONNECTOR_ARGS, "Connector Item Arguments", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    

// No contamos los mapping y los añadimos directamente ========================

    iLength = m4uint16_t( poDRecordSet->Count() ) ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;


		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poFatherNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poFatherNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Connector Item Arguments" ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_ITEM_USED, M4MDFAC_ITEM_ID_ITEM_USED, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poFatherNitem = poFatherNode->GetNitemById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poFatherNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Connector Item Arguments" ) ;

		poFatherRitem = poFatherNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poFatherRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        poFatherItem = poFatherNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poFatherItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poSonNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poSonNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Connector Item Arguments" ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poSonNitem = poSonNode->GetNitemById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Connector Item Arguments" ) ;

		poSonItem = poSonNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poSonItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


        poNitemConn = poFatherRitem->GetConnectorBySonNitem( poSonNitem ) ;
		CHECK_CHLOG_ERRORF( poNitemConn == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_CON, LOG_CAT_AND( poFatherNitem->iItem(), poFatherNitem->nItem() ) << LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_CONNECTOR_ARGS_ID_ARG_USED, M4MDFAC_ITEM_ID_ARG_USED, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poArgument = poFatherItem->GetArgumentById( pccValue ) ;
		CHECK_CHLOG_ERRORF( poArgument == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_ARG, pccValue << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << LOG_CAT_AND( poFatherNitem->iItem(), poFatherNitem->nItem() ) << LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) ) ;



        pstMapping = new StArgumentMappig_t ;
		CHECK_CHLOG_ERRORF( pstMapping == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( StArgumentMappig_t ) ) << __LINE__ << __FILE__ ) ;

        iResult = _ReadOneArgumentMappingFromAccess( ao_poChannel, poRegister, pstMapping, poSonNitem, poSonNode, poFatherItem, poFatherNode, poArgument ) ;

        if( iResult != M4_SUCCESS )
        {
			delete( pstMapping ) ;
            return( iResult ) ;
        }


		// Controlamos que no haya mapeos repetidos

		iSonOrder = pstMapping->m_iSonRealOrder ;
		iMappings = poNitemConn->GetNumberOfArgMappings() ;

		for( j = 0 ; j < iMappings ; j++ )
		{
			pstStoredMapping = poNitemConn->GetArgMappingByPosition ( j ) ;
			CHECK_CHLOG_DEBUGF( pstStoredMapping == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iStoredSonOrder = pstStoredMapping->m_iSonRealOrder ;

			if( iStoredSonOrder == iSonOrder )
			{
		        poArgument = poSonItem->GetArgumentByPosition( iSonOrder ) ;

				if( poArgument != NULL )
				{
					pccArgumentId = poArgument->GetArgumentId() ;
				}
				else
				{
					pccArgumentId = "" ;
				}

				DUMP_CHLOG_ERRORF( M4_MDFAC_REPEATED_ITEM_CONNECTOR_MAPPING, pccArgumentId << LOG_CAT_AND( poFatherItem->iItem(), poFatherItem->nItem() ) << LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) << LOG_CAT_AND( poSonItem->iItem(), poSonItem->nItem() ) << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
				delete( pstMapping ) ;
				return( M4_ERROR ) ;
			}
		}


        iResult = poNitemConn->AddArgMapping( pstMapping ) ;

	    if( iResult != M4_SUCCESS )
	    {
			delete( pstMapping ) ;
	    }

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    }


// Ordenamos los mappings =====================================================

    iResult = ao_poChannel->OrderItemConnMappings() ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadIndexesFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempIndex			*poIndex ;
	ClTempIndex			*poIndexesArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_INDEXES, "Indexes", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poIndexesArray = new ClTempIndex[ iLength ] ;
		CHECK_CHLOG_ERRORF( poIndexesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempIndex ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetIndexesArray( poIndexesArray ) ;
		ao_poChannel->SetIndexesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poIndex = &( poIndexesArray[ i ] ) ;

			poIndex->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneIndexFromAccess( ao_poChannel, poRegister, poIndex ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddIndex( poIndex ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderIndexes() ;


// Asignamos los indices al nodo ================================================

		iResult = ao_poChannel->FillNodesWithIndexes() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadIndexItemsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempIndexItem		*poIndexItem ;
	ClTempIndexItem		*poIndexItemsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_INDEX_ITEMS, "Index Items", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poIndexItemsArray = new ClTempIndexItem[ iLength ] ;
		CHECK_CHLOG_ERRORF( poIndexItemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempIndexItem ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetIndexItemsArray( poIndexItemsArray ) ;
		ao_poChannel->SetIndexItemsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poIndexItem = &( poIndexItemsArray[ i ] ) ;


			iResult = _ReadOneIndexItemFromAccess( ao_poChannel, poRegister, poIndexItem ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddIndexItem( poIndexItem ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderIndexItems() ;


// Asignamos los indices con los items ==========================================

		iResult = ao_poChannel->FillIndexesWithItems() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadPlugsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool, ClStaticPoolStr *ao_poNoUpperPool )
{


    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempPlug			*poPlug ;
    ClTempPlug			*poPlugsArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_T3_CONNECTORS, "Lists", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poPlugsArray = new ClTempPlug[ iLength ] ;
		CHECK_CHLOG_ERRORF( poPlugsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempPlug ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetPlugsArray( poPlugsArray ) ;
		ao_poChannel->SetPlugsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poPlug = &( poPlugsArray[ i ] ) ;

			poPlug->SetUpperPool( ao_poPool ) ;
			poPlug->SetNoUpperPool( ao_poNoUpperPool ) ;


			iResult = _ReadOnePlugFromAccess( ao_poChannel, poRegister, poPlug ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddPlug( poPlug ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderPlugs() ;


// Añadimos los carriles a los nodos ==========================================

		iResult = ao_poChannel->FillNodesWithPlugs() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadItemPlugsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempPlugItem		*poPlugItem ;
    ClTempPlugItem		*poPlugItemsArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_T3_CONNECT_ITEM, "List Items", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poPlugItemsArray = new ClTempPlugItem[ iLength ] ;
		CHECK_CHLOG_ERRORF( poPlugItemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempPlugItem ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetPlugItemsArray( poPlugItemsArray ) ;
		ao_poChannel->SetPlugItemsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poPlugItem = &( poPlugItemsArray[ i ] ) ;

			poPlugItem->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOnePlugItemFromAccess( ao_poChannel, poRegister, poPlugItem ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddPlugItem( poPlugItem ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderPlugItems() ;


// Añadimos los items a los carriles ==========================================

		iResult = ao_poChannel->FillPlugsWithItems() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadAliasFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempAlias			*poAlias ;
    ClTempAlias			*poAliasArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ALIAS_RES, "Alias", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poAliasArray = new ClTempAlias[ iLength ] ;
		CHECK_CHLOG_ERRORF( poAliasArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempAlias ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetAliasArray( poAliasArray ) ;
		ao_poChannel->SetAliasDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poAlias = &( poAliasArray[ i ] ) ;

			poAlias->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneAliasFromAccess( ao_poChannel, poRegister, poAlias ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddAlias( poAlias ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderAlias() ;


// Añadimos los alias a los nodos =============================================

		iResult = ao_poChannel->FillNodesWithAlias() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _ReadT3AliasFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempT3Alias		*poT3Alias ;
    ClTempT3Alias		*poT3AliasArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_T3_ALIAS_RES, "M4Obj Alias", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poT3AliasArray = new ClTempT3Alias[ iLength ] ;
		CHECK_CHLOG_ERRORF( poT3AliasArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempT3Alias ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetT3AliasArray( poT3AliasArray ) ;
		ao_poChannel->SetT3AliasDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poT3Alias = &( poT3AliasArray[ i ] ) ;

			poT3Alias->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneT3AliasFromAccess( ao_poChannel, poRegister, poT3Alias ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddT3Alias( poT3Alias ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderT3Alias() ;


// Añadimos los alias de t3 a los nodos =======================================

		iResult = ao_poChannel->FillNodesWithT3Alias() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


// Añadimos las instancias al m4obj ===========================================

    iResult = ao_poChannel->FillChannelWithInstances() ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadFiltersFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool )
{

    m4return_t          iResult ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempFilter		*poFilter ;
    ClTempFilter		*poFiltersArray ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_FILTERS, "Filters", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poFiltersArray = new ClTempFilter[ iLength ] ;
		CHECK_CHLOG_ERRORF( poFiltersArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempFilter ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetFiltersArray( poFiltersArray ) ;
		ao_poChannel->SetFiltersDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poFilter = &( poFiltersArray[ i ] ) ;

			poFilter->SetUpperPool( ao_poPool ) ;


			iResult = _ReadOneFilterFromAccess( ao_poChannel, poRegister, poFilter ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddFilter( poFilter ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderFilters() ;


// Añadimos los filtros a los nodos =============================================

		iResult = ao_poChannel->FillNodesWithFilters() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadItemCsDescFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t          iResult ;
	m4uint8_t			iIsMyRitem ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    ClTempNode          *poNode ;
    ClTempNitem         *poNitem ;
    ClRegister          *poRegister ;
    ClAccessRecordSet	*poDRecordSet ;

    m4uint8_t			iValue ;
	m4double_t			fValue ;
    m4pcchar_t          pccValue ;


	iResult = _GetRecorset( M4MDFAC_NODE_SYS_ITEM_CSDESC, "Item C/S Description", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

// Asignamos el tipo directamente

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;

		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_CSDESC_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poNode = ao_poChannel->GetNodeById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccValue << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Item Cs Descriptions" ) ;



		iResult = M4ReadString( ao_poChannel, pccValue, "", M4MDFAC_ITEM_CSDESC_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        poNitem = poNode->GetNitemById( pccValue, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccValue << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) << "Item Cs Descriptions" ) ;


		iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

		if( iIsMyRitem == 0 )
		{
			iResult = poNitem->UnreferNitem( ao_poChannel->GetSystemTi() ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


		iResult = M4ReadNumber( ao_poChannel, fValue, poNitem->GetCsType(), M4MDFAC_ITEM_CSDESC_ID_CSTYPE, M4MDFAC_ITEM_ID_CSTYPE, poRegister, M4CL_CSTYPE_MIN_NUMBER, M4CL_CSTYPE_MAX_NUMBER ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		iValue = m4uint8_t( fValue ) ;

		// por si no es un tipo válido
		if( g_aiReducedItemCsTypesArray[ iValue - 1 ] == -1 )
		{
			DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_VALUE, iValue << M4MDFAC_ITEM_ID_CSTYPE << M4ClCsType( M4CL_CSTYPE_BOTH ) << LOG_CAT_AND( ao_poChannel->iM4Obj(), ao_poChannel->nM4Obj() ) ) ;
			iValue = M4CL_CSTYPE_BOTH ;
		}

		poNitem->SetCsType( iValue ) ;



		iResult = M4ReadNumber( ao_poChannel, fValue, poNitem->GetSyncType(), M4MDFAC_ITEM_CSDESC_ID_SYNC_TYPE, M4MDFAC_ITEM_ID_SYNC_TYPE, poRegister, M4_SYNC_NONE, M4_SYNC_REGISTER_BRANCH ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		poNitem->SetSyncType( m4uint8_t( fValue ) ) ;



		iResult = M4ReadVersion( ao_poChannel, M4MDFAC_ITEM_CSDESC_VERSION, M4MDFAC_ITEM_VERSION, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadBusinessServiceFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			iLength ;
    m4uint8_t			iValue ;
	m4double_t			fValue ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_BUSINESS_SERVICES, "Business Service", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poRegister = &( poDRecordSet->Register[ 0 ] ) ;


		iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_BUSINESS_SERVICES_ID_STYLEUSE, M4MDFAC_ITEM_ID_STYLEUSE, poRegister, 0, 2 ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( fValue == 0.0 )
		{
			iValue = 0 ;
		}
		else if( fValue == 1.0 )
		{
			iValue = 1 ;
		}
		else
		{
			iValue = 2 ;
		}

		ao_poChannel->SetStyleUse( iValue ) ;


		iResult = M4ReadNumber( ao_poChannel, fValue, 0, M4MDFAC_BUSINESS_SERVICES_ID_WSS_SECURITY, M4MDFAC_ITEM_ID_WSS_SECURITY, poRegister, 0, 1 ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( fValue == 0.0 )
		{
			iValue = 0 ;
		}
		else
		{
			iValue = 1 ;
		}

		ao_poChannel->SetWSSSecurity( iValue ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadBusinessMethodsFromAccess( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool, ClStaticPoolStr *ao_poNoUpperPool )
{

    m4return_t				iResult ;
    m4uint16_t				i ;
    m4uint16_t				iLength ;
    ClTempBusinessMethod	*poBusinessMethod ;
    ClTempBusinessMethod	*poBusinessMethodsArray ;
    ClRegister				*poRegister ;
    ClAccessRecordSet		*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_BUSINESS_METHODS, "Business Methods", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poBusinessMethodsArray = new ClTempBusinessMethod[ iLength ] ;
		CHECK_CHLOG_ERRORF( poBusinessMethodsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempBusinessMethod ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetBusinessMethodsArray( poBusinessMethodsArray ) ;
		ao_poChannel->SetBusinessMethodsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poBusinessMethod = &( poBusinessMethodsArray[ i ] ) ;

			poBusinessMethod->SetUpperPool( ao_poPool ) ;
			poBusinessMethod->SetNoUpperPool( ao_poNoUpperPool ) ;


			iResult = _ReadOneBusinessMethodFromAccess( ao_poChannel, poRegister, poBusinessMethod ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddBusinessMethod( poBusinessMethod ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderBusinessMethods() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadBusinessArgsFromAccess( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempBusinessArg	*poBusinessArg ;
    ClTempBusinessArg	*poBusinessArgsArray ;
    ClRegister			*poRegister ;
    ClAccessRecordSet	*poDRecordSet ;



	iResult = _GetRecorset( M4MDFAC_NODE_SYS_BUSINESS_ARGS, "Business Arguments", poDRecordSet ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poDRecordSet->Count() ) ;

	if( iLength > 0 )
	{
		poBusinessArgsArray = new ClTempBusinessArg[ iLength ] ;
		CHECK_CHLOG_ERRORF( poBusinessArgsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempBusinessArg ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetBusinessArgsArray( poBusinessArgsArray ) ;
		ao_poChannel->SetBusinessArgsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;
			poBusinessArg = &( poBusinessArgsArray[ i ] ) ;

			iResult = _ReadOneBusinessArgFromAccess( ao_poChannel, poRegister, poBusinessArg ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddBusinessArg( poBusinessArg ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderBusinessArgs() ;


// Añadimos los argumentos a los métodos de negocio =============================

		iResult = ao_poChannel->FillBusinessMethodsWithArgs() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _GetRecorset( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccMessage, ClAccessRecordSet* &ao_rpoRecordset )
{

	m4uint32_t	iHandle ;
    ClNode		*poDNode ;


	ao_rpoRecordset = NULL;

	iHandle = m_poCMCR->GetChannelNodeHandleById( ai_pccNode, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, ai_pccNode << LOG_CAT_AND( m_poCMCR->iM4Obj(), m_poCMCR->nM4Obj() ) << ai_pccMessage ) ;

	poDNode = &( m_poAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, ai_pccNode << LOG_CAT_AND( m_poCMCR->iM4Obj(), m_poCMCR->nM4Obj() ) << ai_pccMessage ) ;

	ao_rpoRecordset = &( poDNode->RecordSet ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _ReadName( ClTempChannel *ai_poChannel, m4pcchar_t &ao_rpccValue, m4bool_t ai_bUseDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister, m4pcchar_t ai_pccTransNode, ClItemIndex ai_iTransIndex, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond )
{

    m4return_t			iResult ;
	m4uint8_t			iLanguage ;
	m4uint8_t			iDefaultLanguage ;
	ClItemIndex			iIndex ;
	m4VariantType		avValues[ 2 ] ;
    ClAccessRecordSet   *poDRecordSet ;


	ao_rpccValue = NULL ;

	iLanguage = ai_poChannel->GetLanguage() ;

	// Primero se busca en las traducciones dinámicas
	iResult = _GetRecorset( ai_pccTransNode, ai_pccTransNode, poDRecordSet ) ;

	if( iResult == M4_SUCCESS && poDRecordSet != NULL && poDRecordSet->Register.Begin() == M4_TRUE )
	{
		avValues[ 0 ].SetStringRef( (m4pchar_t) ai_pccFirst ) ;
		avValues[ 1 ].SetStringRef( (m4pchar_t) ai_pccSecond ) ;

		poDRecordSet->Register.Find( avValues ) ;

		if( poDRecordSet->Register.Index != M4DM_EOF_INDEX )
		{
			iResult = M4ReadString( ai_poChannel, ao_rpccValue, "", ai_iTransIndex, ai_pccItem, &poDRecordSet->Register ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	// Después en los estáticos
	if( iLanguage < M4CL_LANGUAGE_NUMBER && ( ao_rpccValue == NULL || *ao_rpccValue == '\0' ) )
	{
		iIndex = ClItemIndex( ai_iIndex + g_aiReducedLanguagesArray[ iLanguage ] ) ;
		iResult = M4ReadString( ai_poChannel, ao_rpccValue, "", iIndex, ai_pccItem, ai_poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	// Finalmente en el de por defecto
	if( ai_bUseDefault == M4_TRUE && ( ao_rpccValue == NULL || *ao_rpccValue == '\0' ) )
	{
		iDefaultLanguage = ai_poChannel->GetDefaultLanguage() ;

		if( iDefaultLanguage != iLanguage && iDefaultLanguage < M4CL_LANGUAGE_NUMBER )
		{
			iIndex = ClItemIndex( ai_iIndex + g_aiReducedLanguagesArray[ iDefaultLanguage ] ) ;
			iResult = M4ReadString( ai_poChannel, ao_rpccValue, "", iIndex, ai_pccItem, ai_poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Genera un metacanal compilado a partir de un acceso
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_BuildFromAccess( ClAccess *ai_poAccess, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, m4uint8_t ai_iRegister, m4bool_t ai_bAddSystem, ClCompiledMCR* &ao_rpCMCR )
{

    m4return_t          iResult ;
	m4uint8_t			iType ;
	m4uint16_t          iLength ;
	m4language_t		lLanguage ;
	m4pcchar_t			pccThisT3Id ;
	m4double_t			dValue ;
    m4VariantType		vValue ;
	ClTempTi			*poTi ;
	ClTempNode			*poNode ;
    ClRegister          *poRegister ;
    ClNode              *poDNode ;
    ClAccessRecordSet   *poDRecordSet ;
    ClTempChannel       oChannel ;


	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildFromAccess" ) ;
	CHECK_CHLOG_DEBUGF( ai_poAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Inicializamos ==============================================================

    oChannel.SetFactory( this ) ;
	oChannel.SetUpperPool( &m_oAccessUpperPool ) ;
	oChannel.SetNoUpperPool( &m_oAccessNoUpperPool ) ;

    

    poDNode = &( ai_poAccess->Node[ M4MDFAC_NODE_SYS_T3S ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


// Conseguimos el id del canal para errores y otros ===========================

	iResult = poDRecordSet->Item[ M4MDFAC_ITEM_ID_T3 ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		pccThisT3Id = "" ;
	}
	else
	{
		pccThisT3Id = vValue.Data.PointerChar ;
	}



// Si hay que meter los items del sistema =====================================

	if( ai_bAddSystem == M4_TRUE )
	{
		if( m_iIsSystemBuilt == 0 )
		{
			iResult = _CreateSystemAccess() ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, pccThisT3Id << M4ClCsType( ai_iCsType ) ) ;
				return( M4_ERROR ) ;
			}
		}

		m_oAccessUpperPool.Clone( &m_oSystemUpperPool ) ;
		m_oAccessNoUpperPool.Clone( &m_oSystemNoUpperPool ) ;
		
		poTi = m_poSystemTempM4Obj->GetTiByPosition( 0 ) ;
		poNode = m_poSystemTempM4Obj->GetNodeByPosition( 0 ) ;

		oChannel.SetSystemTi( poTi ) ;
		oChannel.SetSystemNode( poNode ) ;
	}

	// Esto tiene que ir aquí porque el _CreateSystemAccess modifica m_poAccess
    m_poAccess = ai_poAccess ;
    m_poCMCR = ai_poAccess->GetpCMCR() ;


// Si no está el registro que queremos nos vamos ==============================

    iLength = m4uint16_t( poDRecordSet->Count() ) ;

    if( iLength <= ai_iRegister )
    {
        DUMP_CHLOG_ERRORF( M4_MDFAC_NO_SUCH_M4OBJ, pccThisT3Id ) ;
		return( M4_ERROR ) ;
    }


	// Aqui nos posicionamos en el current para que ponga bien los bloques hijos
	// Despues ya trabajamos con el register
    poDRecordSet->Current.MoveTo( ai_iRegister ) ;
    poRegister = &( poDRecordSet->Register[ ai_iRegister ] ) ;


	// Leemos el tipo de lenguage de metadatos del canal
	iResult = M4ReadNumber( &oChannel, dValue, M4CL_LANGUAGE_TYPE_LOGON, M4MDFAC_T3S_META_LANGUAGE, M4MDFAC_ITEM_META_LANGUAGE, poRegister, M4CL_LANGUAGE_TYPE_LOGON, M4CL_LANGUAGE_TYPE_DEVELOPMENT ) ;

    if( iResult != M4_SUCCESS )
    {
		// Si algo va mal pongo el de toda la vida (logon)
		iType = M4CL_LANGUAGE_TYPE_LOGON ;
    }
	else
	{
		if( dValue == 1.0 )
		{
			iType = M4CL_LANGUAGE_TYPE_LOGON ;
		}
		else
		{
			iType = M4CL_LANGUAGE_TYPE_DEVELOPMENT ;
		}
	}


	if( iType == M4CL_LANGUAGE_TYPE_LOGON )
	{
		lLanguage = m_poLogonAdaptor->GetSessionLanguage( pccThisT3Id, ai_iLanguage ) ;
	}
	else
	{
		lLanguage = m_poLogonAdaptor->GetSessionDevLanguage( pccThisT3Id, ai_iLanguage ) ;
	}
	oChannel.SetLanguage( m4uint8_t( lLanguage ) ) ;

	lLanguage = M4ClLangInfo::Instance()->GetDefaultLanguageId() ;
	oChannel.SetDefaultLanguage( m4uint8_t( lLanguage ) ) ;

    iResult = _ReadT3FromAccess( &oChannel, poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
        DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, pccThisT3Id << M4ClCsType( ai_iCsType ) ) ;
        return( iResult ) ;
    }


    iResult = _CreateFromTemp( &oChannel, ai_iCsType, &m_oAccessUpperPool, &m_oAccessNoUpperPool, ao_rpCMCR ) ;

    if( iResult != M4_SUCCESS )
    {
        DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, pccThisT3Id << M4ClCsType( ai_iCsType ) ) ;
        return( iResult ) ;
    }


    return( M4_SUCCESS ) ;
}


