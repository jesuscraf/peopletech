
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facfill.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las funciones de fichero de la factoría
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"
#include "m4facdef.hpp"

#include "m4buildversion.hpp"
#include "m4objglb.hpp"
#include "cltiform.hpp"
#include "cltarg.hpp"
#include "cltt3inh.hpp"
#include "cltitem.hpp"
#include "cltti.hpp"
#include "cltnode.hpp"
#include "cltndinh.hpp"
#include "cltindex.hpp"
#include "cltsent.hpp"
#include "cltchann.hpp"
#include "cltnitem.hpp"
#include "cltnicon.hpp"
#include "cltconn.hpp"
#include "cltindit.hpp"
#include "clttiinh.hpp"
#include "clfileiod.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"
#include "m4objreg.hpp"
#include "m4unicode.hpp"

extern int	M4IsUnicode( void ) ;



//=============================================================================
// En este fichero no se hace control de los leaks de memoria en casos erróneos
// No merece la pena ya que todo este código es de debug
//=============================================================================




#ifdef	_DEBUG


// Mascaras de lectura de fichero

#define M4MDFAC_NUMBER_MASK( id )					( id ": %lf\n" )
#define M4MDFAC_STRING_MASK( id )					( id ": %s\n" )
#define M4MDFAC_LONG_MASK( id )						( id ": " )
#define M4MDFAC_EMPTY_MASK( id )					( id ": \n" )


#define	M4MDFAC_READ_FILE_BUFFER_SIZE				4000




// Funciones auxiliares

m4return_t	ClFactoryImp :: _ReadNumber( m4pcchar_t ai_pccPattern )
{

    int iScan ;


    iScan = fscanf( m_pfFile, ai_pccPattern, &m_dBuffer ) ;
	CHECK_CHLOG_ERRORF( iScan != 1, M4_ERROR, M4_MDFAC_BAD_READ_NUMBER, ai_pccPattern << ftell( m_pfFile ) ) ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _ReadString( m4pcchar_t ai_pccPattern )
{

    int iScan ;


    iScan = fscanf( m_pfFile, ai_pccPattern, m_pcBuffer ) ;
	CHECK_CHLOG_ERRORF( iScan != 1, M4_ERROR, M4_MDFAC_BAD_READ_STRING, ai_pccPattern << ftell( m_pfFile ) ) ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _ReadLong( m4pcchar_t ai_pccPattern )
{

    int			iScan ;
	int			iLength ;
	m4pcchar_t	pccString ;


	*m_pcBuffer = '\0' ;

    iScan = fscanf( m_pfFile, ai_pccPattern ) ;
	CHECK_CHLOG_ERRORF( iScan != 0, M4_ERROR, M4_MDFAC_BAD_READ_STRING, ai_pccPattern << ftell( m_pfFile ) ) ;

	pccString = fgets( m_pcBuffer, M4MDFAC_READ_FILE_BUFFER_SIZE - 1, m_pfFile ) ;
	CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDFAC_BAD_READ_STRING, ai_pccPattern << ftell( m_pfFile ) ) ;

// El retorno de carro
    iScan = fscanf( m_pfFile, "\n" ) ;

	iLength = strlen( m_pcBuffer ) ;
	CHECK_CHLOG_ERRORF( iLength >= ( M4MDFAC_READ_FILE_BUFFER_SIZE - 1 ), M4_ERROR, M4_MDFAC_BAD_READ_STRING, ai_pccPattern << ftell( m_pfFile ) ) ;

	m_pcBuffer[ iLength - 1 ] = '\0' ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _ReadEmpty( m4pcchar_t ai_pccPattern )
{

    int iScan ;


    iScan = fscanf( m_pfFile, ai_pccPattern ) ;
	CHECK_CHLOG_ERRORF( iScan != 0, M4_ERROR, M4_MDFAC_BAD_READ_STRING, ai_pccPattern << ftell( m_pfFile ) ) ;

    return( M4_SUCCESS ) ;
}






//=============================================================================
// Lectura de fichero =========================================================
//=============================================================================

m4return_t  ClFactoryImp :: _ReadOneT3FromFile( ClTempChannel* ao_poChannel )
{

    m4return_t	iResult ;

    

	iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_T3 ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_T3 << __LINE__ << __FILE__ ) ;

    iResult = ao_poChannel->SetChannelId( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_STREAM_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_STREAM_TYPE << __LINE__ << __FILE__ ) ;

    ao_poChannel->SetType( m4uint8_t( m_dBuffer ) ) ;



	// para que sea separable los .m2
    iResult = ao_poChannel->SetServiceId( M4CL_EXECUTION_SERVICE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneSentenceFromFile( ClTempSentence* ao_poSentence )
{

    m4return_t	iResult ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_SENTENCE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_SENTENCE << __LINE__ << __FILE__ ) ;

    iResult = ao_poSentence->SetSentenceId( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadLong( M4MDFAC_LONG_MASK( M4MDFAC_ITEM_APISQL_1 ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_APISQL_1 << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) == 0 )
	{
		*m_pcBuffer = '\0' ;
	}

	iResult = ao_poSentence->SetApiSql1( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadLong( M4MDFAC_LONG_MASK( M4MDFAC_ITEM_APISQL_2 ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_APISQL_2 << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) == 0 )
	{
		*m_pcBuffer = '\0' ;
	}

	iResult = ao_poSentence->SetApiSql2( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadLong( M4MDFAC_LONG_MASK( M4MDFAC_ITEM_APISQL_3 ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_APISQL_3 << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) == 0 )
	{
		*m_pcBuffer = '\0' ;
	}

	iResult = ao_poSentence->SetApiSql3( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadLong( M4MDFAC_LONG_MASK( M4MDFAC_ITEM_APISQL_4 ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_APISQL_4 << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) == 0 )
	{
		*m_pcBuffer = '\0' ;
	}

	iResult = ao_poSentence->SetApiSql4( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    

    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneTiFromFile( ClTempChannel* ai_poChannel, ClTempTi* ao_poTi )
{

    m4return_t      iResult ;
	ClTempSentence	*poSentence ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI << __LINE__ << __FILE__ ) ;

    iResult = ao_poTi->SetTiId( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_READ_SENTENCE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_READ_SENTENCE << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) != 0 )
	{
		poSentence = ai_poChannel->GetSentenceById( m_pcBuffer, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( poSentence == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI_SENTENCE, m_pcBuffer << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << ao_poTi->GetTiId() ) ;
	}
	else
	{
		poSentence = NULL ;
	}

    ao_poTi->SetReadSentence( poSentence ) ;
    ao_poTi->SetWriteSentence( NULL ) ;


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneTiInheritFromFile( ClTempChannel *ai_poChannel, ClTempTiInherit *ai_poTiInherit )
{

    m4return_t	iResult ;
	ClTempTi	*poBaseTi ;


    
	iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI << __LINE__ << __FILE__ ) ;

	iResult = ai_poTiInherit->SetTiId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI_BASE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI_BASE << __LINE__ << __FILE__ ) ;

	poBaseTi = ai_poChannel->GetTiById( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI, m_pcBuffer << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Tis Inheritance" ) ;

	ai_poTiInherit->SetBaseTi( poBaseTi ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_LEVEL ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_LEVEL << __LINE__ << __FILE__ ) ;

    ai_poTiInherit->SetLevel( m4uint8_t( m_dBuffer ) ) ;


    return( M4_SUCCESS ) ;
}

	
m4return_t  ClFactoryImp :: _ReadOneItemFromFile( ClTempChannel *ai_poChannel, ClTempItem *ao_poItem )
{

    m4return_t	iResult ;
    m4uint8_t	iValue ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI << __LINE__ << __FILE__ ) ;

	iResult = ao_poItem->SetTiId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_ITEM ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM << __LINE__ << __FILE__ ) ;

    iResult = ao_poItem->SetItemId( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_ITEM_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM_TYPE << __LINE__ << __FILE__ ) ;

    ao_poItem->SetType( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_SCOPE_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_SCOPE_TYPE << __LINE__ << __FILE__ ) ;

    ao_poItem->SetScope( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_M4_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_M4_TYPE << __LINE__ << __FILE__ ) ;

	iValue = m4uint8_t( m_dBuffer ) ;

	// Dejamos de soportar cadenas fijas
	if( iValue == M4CL_M4_TYPE_FIXED_STRING )
	{
		iValue = M4CL_M4_TYPE_VAR_STRING ;
	}

    ao_poItem->SetM4Type( iValue ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_PREC ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_PREC << __LINE__ << __FILE__ ) ;

    ao_poItem->SetDBPrecision( m4uint32_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_SCALE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_SCALE << __LINE__ << __FILE__ ) ;

    ao_poItem->SetScale( m4uint8_t( m_dBuffer ) ) ;

    iResult = ao_poItem->SetCppType( ai_poChannel ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadLong( M4MDFAC_LONG_MASK( M4MDFAC_ITEM_DEFAULT_VALUE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_DEFAULT_VALUE << __LINE__ << __FILE__ ) ;

    if( strcmpi( m_pcBuffer, "NULL" ) == 0 )
    {
        iResult = ao_poItem->SetDefaultValue( NULL, ai_poChannel ) ;
    }
    else
    {
        iResult = ao_poItem->SetDefaultValue( m_pcBuffer, ai_poChannel ) ;
    }
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetDmd( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetDmdComponent( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetDmdField( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    ao_poItem->SetSliceBhrv( 0 ) ;
    ao_poItem->SetSliceSplit( 0 ) ;
    ao_poItem->SetIdSliceTotType( 0 ) ;
    ao_poItem->SetIdTotalize( 0 ) ;
    ao_poItem->SetTransactionMode( 0 ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_EXE_LEVEL ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_EXE_LEVEL << __LINE__ << __FILE__ ) ;

    ao_poItem->SetMethodLevel( m4uint8_t( m_dBuffer ) ) ;



    iResult = ao_poItem->SetGroup( "", M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetSliceItem( "", M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	iResult = ao_poItem->SetReadObjectAlias( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetWriteObjectAlias( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetReadFieldId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetWriteFieldId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetReadObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = ao_poItem->SetWriteObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    
    

    ao_poItem->SetSentTotType( 0 ) ;
    ao_poItem->SetOrder( 0 ) ;
	ao_poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ;
	ao_poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_VISIBLE ) ;
	ao_poItem->SetModifyData( 0 ) ;



    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneRuleFromFile( ClTempChannel *ai_poChannel, ClTempItemFormula* ao_poFormula )
{

    m4return_t	iResult ;
    m4uint32_t	iHandle ;
    m4pchar_t	pcString ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI << __LINE__ << __FILE__ ) ;

	iResult = ao_poFormula->SetTiId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_ITEM ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM << __LINE__ << __FILE__ ) ;

    iResult = ao_poFormula->SetItemId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    
	
	iResult = ao_poFormula->SetFormulaId( "", M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



	ao_poFormula->SetStartDate( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	ao_poFormula->SetEndDate( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	ao_poFormula->SetType( M4CL_LENGUAGE_CPP ) ;



    iResult = m_oTempPool.AddString( "", iHandle ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    pcString = m_oTempPool.GetString( iHandle ) ;
	ao_poFormula->SetPolish( pcString, m_poFunctionTable ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_METHOD ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_METHOD << __LINE__ << __FILE__ ) ;

	iResult = m_oTempPool.AddString( m_pcBuffer, iHandle ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    pcString = m_oTempPool.GetString( iHandle ) ;
    ao_poFormula->SetMethod( pcString, m_poFunctionTable ) ;


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadOneNodeFromFile( ClTempChannel *ai_poChannel, ClTempNode* ao_poNode )
{

    m4return_t  iResult ;
	ClTempTi	*poTi ;


    
    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE << __LINE__ << __FILE__ ) ;

    iResult = ao_poNode->SetNodeId( m_pcBuffer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_TI ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_TI << __LINE__ << __FILE__ ) ;

    poTi = ai_poChannel->GetTiById( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poTi == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_TI, m_pcBuffer << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Nodes" ) ;

    ao_poNode->SetTi( poTi ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_IS_ROOT ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_IS_ROOT << __LINE__ << __FILE__ ) ;

    ao_poNode->SetIsRoot( m4uint8_t( m_dBuffer ) ) ;


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadOneConnectorFromFile( ClTempChannel *ai_poChannel, ClTempConnector *ao_poConnector )
{

    m4return_t		iResult ;
    ClTempNode		*poSonNode ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE_USED ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE_USED << __LINE__ << __FILE__ ) ;

    ao_poConnector->SetFatherNodeId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE << __LINE__ << __FILE__ ) ;

    poSonNode = ai_poChannel->GetNodeById( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, m_pcBuffer << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Connectors" ) ;

    ao_poConnector->SetSonNode( poSonNode ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_CONNECTION_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_CONNECTION_TYPE << __LINE__ << __FILE__ ) ;

    ao_poConnector->SetType( m4uint8_t( m_dBuffer ) ) ;



    ao_poConnector->SetExpandDelete( 0 ) ;
    ao_poConnector->SetExpandUndo( 0 ) ;
    ao_poConnector->SetSentence( NULL ) ;


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneNitemConnFromFile( ClTempChannel *ai_poChannel, ClTempNitemConn *ao_poNitemConn )
{

    m4return_t      iResult ;
    ClTempNitem     *poSonNitem ;
    ClTempNode      *poSonNode ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE_USED ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE_USED << __LINE__ << __FILE__ ) ;

	iResult = ao_poNitemConn->SetFatherNodeId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE << __LINE__ << __FILE__ ) ;

    poSonNode = ai_poChannel->GetNodeById( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, m_pcBuffer << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Item Connectors" ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_ITEM_USED ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM_USED << __LINE__ << __FILE__ ) ;

	iResult = ao_poNitemConn->SetFatherItemId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_ITEM ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM << __LINE__ << __FILE__ ) ;

    poSonNitem = poSonNode->GetNitemById( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_ERRORF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, m_pcBuffer << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Item Connectors" ) ;

    ao_poNitemConn->SetSonNitem( poSonNitem ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_PRECEDENCE_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_PRECEDENCE_TYPE << __LINE__ << __FILE__ ) ;

    ao_poNitemConn->SetPrecedence( m4uint8_t( m_dBuffer ) ) ;


    
    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_SPIN_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_SPIN_TYPE << __LINE__ << __FILE__ ) ;
    
    ao_poNitemConn->SetSpin( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_RELSHIP_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_RELSHIP_TYPE << __LINE__ << __FILE__ ) ;
    
    ao_poNitemConn->SetRelation( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_CONTEXT_TYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_CONTEXT_TYPE << __LINE__ << __FILE__ ) ;
    
    ao_poNitemConn->SetContext( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_CALL_ORDER ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_CALL_ORDER << __LINE__ << __FILE__ ) ;
    
    ao_poNitemConn->SetCallOrder( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_CSTYPE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_CSTYPE << __LINE__ << __FILE__ ) ;
    
    ao_poNitemConn->SetCsType( m4uint8_t( m_dBuffer ) ) ;


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneIndexFromFile( ClTempIndex *ao_poIndex )
{

    m4return_t	iResult ;


    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE << __LINE__ << __FILE__ ) ;

	iResult = ao_poIndex->SetNodeId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    
    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_INDEX ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_INDEX << __LINE__ << __FILE__ ) ;

    ao_poIndex->SetIndexId( m4uint32_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_IS_DEFAULT_INDEX ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_IS_DEFAULT_INDEX << __LINE__ << __FILE__ ) ;

    ao_poIndex->SetIsDefault( m4uint8_t( m_dBuffer ) ) ;


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadOneIndexItemFromFile( ClTempIndexItem *ao_poIndexItem )
{


    m4return_t	iResult ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_NODE ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_NODE << __LINE__ << __FILE__ ) ;

	iResult = ao_poIndexItem->SetNodeId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    
    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_INDEX ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_INDEX << __LINE__ << __FILE__ ) ;

	ao_poIndexItem->SetIndexId( m4uint32_t( m_dBuffer ) ) ;



    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_ITEM_ID_ITEM ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_ITEM << __LINE__ << __FILE__ ) ;

	iResult = ao_poIndexItem->SetItemId( m_pcBuffer, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_SEARCH_ORDER ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_SEARCH_ORDER << __LINE__ << __FILE__ ) ;

    ao_poIndexItem->SetOrder( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_ID_FUNCTION ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_ID_FUNCTION << __LINE__ << __FILE__ ) ;

    ao_poIndexItem->SetFunction( m4uint8_t( m_dBuffer ) ) ;



    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( M4MDFAC_ITEM_WAY ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_ITEM_WAY << __LINE__ << __FILE__ ) ;

    ao_poIndexItem->SetWay( m4uint8_t( m_dBuffer ) ) ;


    return( M4_SUCCESS ) ;
}





m4return_t  ClFactoryImp :: _ReadT3FromFile( ClTempChannel* ao_poChannel, m4pcchar_t ai_pccFile )
{

    m4return_t	iResult ;
    m4uint16_t	iBuild ;
    m4uint16_t	iRealBuild ;


    iResult = _ReadString( M4MDFAC_STRING_MASK( M4MDFAC_BUILD ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, M4MDFAC_BUILD << __LINE__ << __FILE__ ) ;

    iBuild = ConvertM2ToNumericVersion( m_pcBuffer ) ;
	iRealBuild = GetM2VersionNumber() ;
	CHECK_CHLOG_ERRORF( iBuild != iRealBuild, M4_ERROR, M4_MDFAC_BAD_FILE_VERSION, ai_pccFile << m_pcBuffer << GetM2VersionString() ) ;



// T3 =========================================================================

	iResult = _ReadOneT3FromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// T3 inherits ================================================================

// No leemos nada de fichero. Simplemente ponemos los del propio canal

    iResult = _ReadT3InheritsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Sentences ==================================================================

	iResult = _ReadSentencesFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Tis ========================================================================

	iResult = _ReadTisFromFile( ao_poChannel ) ;

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


// Ti inherits ================================================================

	iResult = _ReadTiInheritsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Items ======================================================================

	iResult = _ReadItemsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	
// Rules ======================================================================

	iResult = _ReadRulesFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Nodos ======================================================================

	iResult = _ReadNodesFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Node Inherits ==============================================================

// No leemos nada de fichero. Simplemente ponemos los del propio nodo

    iResult = _ReadNodeInheritsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Conectores =================================================================

	iResult = _ReadConnectorsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Conectores de item =========================================================

	iResult = _ReadItemConnectorsFromFile( ao_poChannel ) ;

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


// Indices ====================================================================

	iResult = _ReadIndexesFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Items de indice ============================================================

	iResult = _ReadIndexItemsFromFile( ao_poChannel ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    return( M4_SUCCESS ) ;
}



// No leemos nada de fichero. Simplemente ponemos los del propio canal

m4return_t  ClFactoryImp :: _ReadT3InheritsFromFile( ClTempChannel* ao_poChannel )
{

    m4return_t          iResult ;
	ClTempT3Inherit		*poT3Inherit ;
	ClTempT3Inherit		*poT3InheritsArray ;


	poT3InheritsArray = new ClTempT3Inherit[ 1 ] ;
	CHECK_CHLOG_ERRORF( poT3InheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempT3Inherit ) * 1 ) << __LINE__ << __FILE__ ) ;

	ao_poChannel->SetT3InheritsArray( poT3InheritsArray ) ;
	ao_poChannel->SetT3InheritsDelta( 1 ) ;


// Ponemos el propio M4Obj a nivel 0 ==========================================

	poT3Inherit = &( poT3InheritsArray[ 0 ] ) ;
	poT3Inherit->SetUpperPool( &m_oFilePool ) ;

	iResult = poT3Inherit->SetBaseT3Id( ao_poChannel->GetChannelId() ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poT3Inherit->SetLevel( 0 ) ;
	poT3Inherit->SetIndex( 0 ) ;


	// La ponemos en el array global
    iResult = ao_poChannel->AddT3Inherit( poT3Inherit ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    return( M4_SUCCESS ) ;
}




m4return_t  ClFactoryImp :: _ReadSentencesFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempSentence	*poSentence ;
	ClTempSentence	*poSentencesArray ;


    
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "SENTENCE_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "SENTENCE_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poSentencesArray = new ClTempSentence[ iLength ] ;
		CHECK_CHLOG_ERRORF( poSentencesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempSentence ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetSentencesArray( poSentencesArray ) ;
		ao_poChannel->SetSentencesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poSentence = &( poSentencesArray[ i ] ) ;
			poSentence->SetUpperPool( &m_oFilePool ) ;
			poSentence->SetNoUpperPool( &m_oFilePool ) ;

        
			iResult = _ReadOneSentenceFromFile( poSentence ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddSentence( poSentence ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderSentences() ;
	}


	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadTisFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempTi		*poTi ;
	ClTempTi		*poTisArray ;


    
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "TI_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "TI_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poTisArray = new ClTempTi[ iLength ] ;
		CHECK_CHLOG_ERRORF( poTisArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTi ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetTisArray( poTisArray ) ;
		ao_poChannel->SetTisDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poTi = &( poTisArray[ i ] ) ;
			poTi->SetUpperPool  ( &m_oFilePool ) ;
			poTi->SetNoUpperPool( &m_oFilePool ) ;

        
			iResult = _ReadOneTiFromFile( ao_poChannel, poTi ) ;

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



m4return_t  ClFactoryImp :: _ReadTiInheritsFromFile( ClTempChannel *ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTis ;
    m4uint16_t			iSize ;
    ClTempTi			*poTi ;
    ClTempTiInherit		*poTiInherit ;
	ClTempTiInherit		*poTiInheritsArray ;

	
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "INHERIT_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "INHERIT_NUMBER" << __LINE__ << __FILE__ ) ;

// Alojamos la memoria necesaria ==============================================

	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( m_dBuffer ) ;

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
			poTiInherit = &( poTiInheritsArray[ i + iTis ] ) ;


			iResult = _ReadOneTiInheritFromFile( ao_poChannel, poTiInherit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddTiInherit( poTiInherit ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderTiInherits() ;


// Asignamos los ti inherits a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithInherits() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadItemsFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempItem		*poItem ;
	ClTempItem		*poItemsArray ;

    
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "ITEM_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "ITEM_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poItemsArray = new ClTempItem[ iLength ] ;
		CHECK_CHLOG_ERRORF( poItemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItem ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetItemsArray( poItemsArray ) ;
		ao_poChannel->SetItemsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poItem = &( poItemsArray[ i ] ) ;

			// Inicializamos el item
			poItem->SetUpperPool( &m_oFilePool ) ;
			poItem->SetNoUpperPool( &m_oFilePool ) ;


			iResult = _ReadOneItemFromFile( ao_poChannel, poItem ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddItem( poItem ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderItems() ;


// Asignamos los ítems a las TIs ==============================================

		iResult = ao_poChannel->FillTisWithItems() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadRulesFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempItemFormula	*poItemFormula ;
    ClTempItemFormula	*poItemFormulasArray ;

    
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "RULE_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "RULE_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poItemFormulasArray = new ClTempItemFormula[ iLength ] ;
		CHECK_CHLOG_ERRORF( poItemFormulasArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItemFormula ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetItemFormulasArray( poItemFormulasArray ) ;
		ao_poChannel->SetItemFormulasDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poItemFormula = &( poItemFormulasArray[ i ] ) ;


			iResult = _ReadOneRuleFromFile( ao_poChannel, poItemFormula ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// Lo ponemos en el array global
       		iResult = ao_poChannel->AddItemFormula( poItemFormula ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ao_poChannel->OrderItemFormulas() ;


// Asignamos las formulas a las TIs ===========================================

		iResult = ao_poChannel->FillTisWithItemFormulas() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadNodesFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempNode		*poNode ;
	ClTempNode		*poNodesArray ;


    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "NODE_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "NODE_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poNodesArray = new ClTempNode[ iLength ] ;
		CHECK_CHLOG_ERRORF( poNodesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNode ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNodesArray( poNodesArray ) ;
		ao_poChannel->SetNodesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poNode = &( poNodesArray[ i ] ) ;

			// Inicializamos el nodo
			poNode->SetUpperPool( &m_oFilePool ) ;
			poNode->SetNoUpperPool( &m_oFilePool ) ;

			poNode->SetChannel( ao_poChannel ) ;


			iResult = _ReadOneNodeFromFile( ao_poChannel, poNode ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ao_poChannel->AddNode( poNode ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


		ao_poChannel->OrderNodes() ;
	}


	return( M4_SUCCESS ) ;
}



// No leemos nada de fichero. Simplemente ponemos los del propio nodo

m4return_t  ClFactoryImp :: _ReadNodeInheritsFromFile( ClTempChannel* ao_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iNodes ;
    m4uint16_t			iSize ;
    ClTempNode			*poNode ;
    ClTempNodeInherit	*poNodeInherit ;
	ClTempNodeInherit	*poNodeInheritsArray ;


	iNodes = ao_poChannel->GetNumberOfNodes() ;
	iSize = iNodes ;

	if( iSize > 0 )
	{
		poNodeInheritsArray = new ClTempNodeInherit[ iSize ] ;
		CHECK_CHLOG_ERRORF( poNodeInheritsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNodeInherit ) * iSize ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNodeInheritsArray( poNodeInheritsArray ) ;
		ao_poChannel->SetNodeInheritsDelta( iSize ) ;


// Ponemos los propios nodos a nivel del nodo =================================

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

		ao_poChannel->OrderNodeInherits() ;


// Asignamos los node inherits a los nodos ====================================

		iResult = ao_poChannel->FillNodesWithInherits() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Añadimos los items a los nodos =============================================

		iResult = ao_poChannel->FillNodesWithItems() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadConnectorsFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint8_t       iType ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iNodes ;
    m4uint16_t		iSize ;
	m4pcchar_t		pccFatherNode ;
    ClTempNode		*poNode ;
    ClTempConnector	*poConnector ;
	ClTempConnector	*poConnectorsArray ;


    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "CONNECTOR_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "CONNECTOR_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

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
			poConnector->SetUpperPool( &m_oFilePool ) ;

			poNode = ao_poChannel->GetNodeByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poConnector->SetFatherNode( poNode ) ;
			poConnector->SetSonNode( poNode ) ;
			poConnector->SetType( M4CL_NODE_CONNECTOR_TYPE_AUTO ) ;
			poConnector->SetExpandDelete( 0 ) ;
			poConnector->SetExpandUndo( 0 ) ;
			poConnector->SetSentence( NULL ) ;

			pccFatherNode = poNode->GetNodeId() ;
			poConnector->SetFatherNodeId( pccFatherNode, M4_FALSE ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// La ponemos en el array global
       		iResult = ao_poChannel->AddConnector( poConnector ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poConnector = &( poConnectorsArray[ i + iNodes ] ) ;

			poConnector->SetUpperPool( &m_oFilePool ) ;


			iResult = _ReadOneConnectorFromFile( ao_poChannel, poConnector ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			iType = poConnector->GetType() ;

			// Los autoconectados no les ponemos
			if( iType != M4CL_NODE_CONNECTOR_TYPE_AUTO )
			{
				// La ponemos en el array global
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




m4return_t  ClFactoryImp :: _ReadItemConnectorsFromFile( ClTempChannel* ao_poChannel )
{
	
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempNitemConn	*poNitemConn ;
    ClTempNitemConn	*poNitemConnArray ;


    iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "ITEM_CONN_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "ITEM_CONN_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poNitemConnArray = new ClTempNitemConn[ iLength ] ;
		CHECK_CHLOG_ERRORF( poNitemConnArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemConn ) * iLength ) << __LINE__ << __FILE__ ) ;

		ao_poChannel->SetNitemConnsArray( poNitemConnArray ) ;
		ao_poChannel->SetNitemConnsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poNitemConn = &( poNitemConnArray[ i ] ) ;

			poNitemConn->SetUpperPool( &m_oFilePool ) ;


			iResult = _ReadOneNitemConnFromFile( ao_poChannel, poNitemConn ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
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



m4return_t  ClFactoryImp :: _ReadIndexesFromFile( ClTempChannel *ai_poChannel )
{
    
    m4return_t		iResult ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    ClTempIndex		*poIndex ;
	ClTempIndex		*poIndexesArray ;

	
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "INDEX_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "INDEX_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poIndexesArray = new ClTempIndex[ iLength ] ;
		CHECK_CHLOG_ERRORF( poIndexesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempIndex ) * iLength ) << __LINE__ << __FILE__ ) ;

		ai_poChannel->SetIndexesArray( poIndexesArray ) ;
		ai_poChannel->SetIndexesDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poIndex = &( poIndexesArray[ i ] ) ;

			poIndex->SetUpperPool( &m_oFilePool ) ;


			iResult = _ReadOneIndexFromFile( poIndex ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ai_poChannel->AddIndex( poIndex ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ai_poChannel->OrderIndexes() ;


// Asignamos los indices al nodo ================================================

		iResult = ai_poChannel->FillNodesWithIndexes() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadIndexItemsFromFile( ClTempChannel *ai_poChannel )
{

    m4return_t			iResult ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    ClTempIndexItem		*poIndexItem ;
	ClTempIndexItem		*poIndexItemsArray ;

	
	iResult = _ReadNumber( M4MDFAC_NUMBER_MASK( "INDEX_ITEM_NUMBER" ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_SUCH_TOKEN, "INDEX_ITEM_NUMBER" << __LINE__ << __FILE__ ) ;


// Alojamos la memoria necesaria ==============================================

    iLength = m4uint16_t( m_dBuffer ) ;

	if( iLength > 0 )
	{
		poIndexItemsArray = new ClTempIndexItem[ iLength ] ;
		CHECK_CHLOG_ERRORF( poIndexItemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempIndexItem ) * iLength ) << __LINE__ << __FILE__ ) ;

		ai_poChannel->SetIndexItemsArray( poIndexItemsArray ) ;
		ai_poChannel->SetIndexItemsDelta( iLength ) ;


// Rellenamos el array y lo ordenamos =========================================

		for( i = 0 ; i < iLength ; i++ )
		{
			poIndexItem = &( poIndexItemsArray[ i ] ) ;


			iResult = _ReadOneIndexItemFromFile( poIndexItem ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}


			// La ponemos en el array global
       		iResult = ai_poChannel->AddIndexItem( poIndexItem ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ai_poChannel->OrderIndexItems() ;


// Asignamos los indices con los items ==========================================

		iResult = ai_poChannel->FillIndexesWithItems() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


    return( M4_SUCCESS ) ;
}



#endif







//=================================================================================
// Genera un metacanal compilado a partir de un fichero
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_BuildFromFile( m4pcchar_t ai_pccFileName, m4uint8_t ai_iCsType, m4uint8_t ai_iUseMcrs, m4pchar_t ai_pccPath, ClCompiledMCR* &ao_rpCMCR )
{

    m4return_t          iResult ;
    m4uint8_t			iDumpError ;
    m4uint8_t			iRead ;
    m4pcchar_t          pccPath ;
    m4char_t            acFile[ M4CH_MAX_FILE + 1 ] ;
	ClCMCRWrapper		*poCMCRWrapper ;
	ClFileIODriver		oFileDriver ;


	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildFromFile" ) ;
	CHECK_CHLOG_DEBUGF( ai_pccFileName == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// vamos a ver si tenemos unos .mcr guais

	iRead = 0 ;
	iDumpError = 1 ;

	if( ai_pccPath != NULL && *ai_pccPath != '\0' )
	{
		pccPath = ai_pccPath ;
	}
	else
	{
		pccPath = m_poEnvironment->GetWorkingDirectory() ;
	}

	if( ai_iUseMcrs == 1 )
	{
		if( pccPath != NULL && *pccPath != '\0' )
		{
			sprintf( acFile, "%s%s%s&%d.mcr", pccPath, M4_BACK_SLASH, ai_pccFileName, ai_iCsType ) ;
		}
		else
		{
			sprintf( acFile, "%s&%d.mcr", ai_pccFileName, ai_iCsType ) ;
		}

		iResult = oFileDriver.Init( GEN_IO_MODE_READ, acFile, M4_FALSE ) ;

		if( iResult == M4_SUCCESS )
		{
			iDumpError = 0 ;

			poCMCRWrapper = new ClCMCRWrapper ;
			CHECK_CHLOG_ERRORF( poCMCRWrapper == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCMCRWrapper ) ) << __LINE__ << __FILE__ ) ;

			iResult = poCMCRWrapper->DeSerialize( oFileDriver ) ;

			oFileDriver.End( M4_TRUE ) ;

			if( iResult == M4_SUCCESS )
			{
				ao_rpCMCR = new ClCompiledMCR ;

				if( ao_rpCMCR == NULL )
				{
					delete( poCMCRWrapper ) ;
				}

				CHECK_CHLOG_ERRORF( ao_rpCMCR == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCompiledMCR ) ) << __LINE__ << __FILE__ ) ;

				ao_rpCMCR->SetCacheCsType(ai_iCsType);
				ao_rpCMCR->SetCMCRWrapper( poCMCRWrapper ) ;

				iRead = 1 ;
			}
			else
			{
				delete( poCMCRWrapper ) ;
			}
		}
	}

	if( iRead == 1 )
	{
		return( M4_SUCCESS ) ;
	}



#ifdef	_DEBUG
// en debug no se han leido los .mcr ==========================================


    m4date_t            dStartDate ;
    m4date_t            dEndDate ;
    m4date_t            dCorDate ;
    ClTempChannel       oChannel ;


	oChannel.SetUpperPool( &m_oFilePool ) ;
	oChannel.SetNoUpperPool( &m_oFilePool ) ;

// inicialización de fechas

    dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
    dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
    dCorDate = ClActualDate() ;


    m_oFilePool.Reset() ;
    m_oTempPool.Reset() ;


    if( m_pcBuffer == NULL )
    {
        m_pcBuffer = new m4char_t[ M4MDFAC_READ_FILE_BUFFER_SIZE ] ;
		CHECK_CHLOG_ERRORF( m_pcBuffer == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * M4MDFAC_READ_FILE_BUFFER_SIZE ) << __LINE__ << __FILE__ ) ;
    }

    oChannel.SetFactory( this ) ;


	if( pccPath != NULL && *pccPath != '\0' )
    {
        sprintf( acFile, "%s%s%s.m2", pccPath, M4_BACK_SLASH, ai_pccFileName ) ;
    }
    else
    {
        sprintf( acFile, "%s.m2", ai_pccFileName ) ;
    }

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

    m_pfFile = M4Fopen( acFile, M4UniRead, eEncoding ) ;
	CHECK_CHLOG_ERRORF( m_pfFile == NULL, M4_ERROR, M4_MDFAC_NO_OPEN_FILE, acFile << "r" ) ;


    iResult = _ReadT3FromFile( &oChannel, acFile ) ;


    fclose( m_pfFile ) ;
    m_pfFile = NULL ;

    if( iResult != M4_SUCCESS )
    {
        return( iResult ) ;
    }


    ao_rpCMCR = new ClCompiledMCR ;
	CHECK_CHLOG_ERRORF( ao_rpCMCR == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCompiledMCR ) ) << __LINE__ << __FILE__ ) ;

	ao_rpCMCR->SetCacheCsType(ai_iCsType);


    iResult = _CreateFromTemp( &oChannel, ai_iCsType, &m_oFilePool, NULL, ao_rpCMCR ) ;

	if( iResult != M4_SUCCESS )
    {
		delete( ao_rpCMCR ) ;
		return( M4_ERROR ) ;
    }


// si estamos en debug y no hay .mcr lo generamos (buenas personas) ===========

	if( pccPath != NULL && *pccPath != '\0' )
    {
        sprintf( acFile, "%s%s%s&%d.mcr", pccPath, M4_BACK_SLASH, ai_pccFileName, ai_iCsType ) ;
    }
    else
    {
        sprintf( acFile, "%s&%d.mcr", ai_pccFileName, ai_iCsType ) ;
    }

	iResult = oFileDriver.Init( GEN_IO_MODE_WRITE, acFile ) ;

	if( iResult == M4_SUCCESS )
	{
		poCMCRWrapper = ao_rpCMCR->GetCMCRWrapper() ;

		iResult = poCMCRWrapper->Serialize( oFileDriver ) ;

		oFileDriver.End( M4_TRUE ) ;
	}


    return( M4_SUCCESS ) ;



#else

// en release no se han leido los .mcr ========================================

	if( iDumpError == 1 )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_OPEN_FILE, acFile << "rb" ) ;
	}

	return( M4_ERROR ) ;

#endif

}


