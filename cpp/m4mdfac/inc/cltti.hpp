
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltti.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                20-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary ti
//
//
//==============================================================================


#include "clpstack.hpp"
#include "spoolstr.h"
#include "facide.hpp"



#ifndef __CLTTI_HPP__
#define __CLTTI_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempSentence ;
class	ClTempNode ;
class	ClTempTrigger ;
class	ClTempTiInherit ;
class	ClTempItemInherit ;
class	ClTempConcept ;
class	ClTempTotal ;
class	ClTempTotalTag ;
class	ClTempTag ;
class	ClTempItem ;
class	ClTempItemFormula ;
class	ClTempTiReference ;
class	ClMdStatistics ;
class	M4HsParser ;


//=================================================================================
//
// ClTempTi
//
// Clase que implementa la definición de las Tis
//
//=================================================================================

class	ClTempTi : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Fijos
    m4uint32_t          m_iTiId ;
    m4uint32_t          m_iReadObjectId ;
    m4uint32_t          m_iWriteObjectId ;
    m4uint32_t          m_iReferencesStart ;

    m4uint8_t           m_iTemporaryId ;
    m4uint8_t           m_iComplete ;
    m4uint8_t           m_iCorrected ;
    m4uint8_t           m_iOwnSentence ;

// Variables
    ClReferenceStack	m_oInherits ;
    ClReferenceStack	m_oItems ;

	ClTempTiReference	*m_poTiReferences ;

// Auxiliares
    ClReferenceStack	m_oTriggers ;
    ClReferenceStack	m_oItemInherits ;
    ClReferenceStack	m_oConcepts ;
    ClReferenceStack	m_oTotals ;
    ClReferenceStack	m_oTotalTags ;
    ClReferenceStack	m_oTags ;
    ClReferenceStack	m_oItemFormulas ;

    ClTempSentence      *m_poReadSentence ;
    ClTempSentence      *m_poWriteSentence ;
    ClTempSentence      *m_poSentence ;

    m4uint8_t           m_iGenerateSql ;
    m4int8_t			m_iSentenceNumber ;
    m4uint8_t           m_iInheritanceType ;

    ClCompIdentifier    *m_poComparisonItem ;
    ClCompIdentifier    m_oComparisonItem ;

// Client server
    m4uint8_t			m_iIsToCompile ;
	m4uint16_t			m_iItemsToCompile ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}


// Funciones de inicialización ================================================
	
        ClTempTi( void ) ;
        ~ClTempTi( void ) ;

	m4return_t	Delete( void ) ;


// Funciones de lectura de la parte fija ======================================

    m4uint32_t	GetTiIdHandle( void ) const
    {
		return( m_iTiId ) ;
    }

    m4pcchar_t  GetTiId( void ) const
    {
		return( m_poUpperPool->GetString( m_iTiId ) ) ;
    }

	m4pcchar_t	GetReadObjectId( void ) const
	{
		return( m_poUpperPool->GetString( m_iReadObjectId ) ) ;
	}

	m4pcchar_t	GetWriteObjectId( void ) const
	{
		return( m_poUpperPool->GetString( m_iWriteObjectId ) ) ;
	}

    m4uint8_t  GetGenerateSql() const
    {
        return( m_iGenerateSql ) ;
    }

    m4int8_t  GetSentenceNumber() const
    {
        return( m_iSentenceNumber ) ;
    }

    m4uint8_t  GetInheritanceType() const
    {
        return( m_iInheritanceType ) ;
    }


// Otras funciones de lectura ================================================

	ClTempNode*	GetNode( m4uint8_t ai_iLevel ) const ;
	m4uint32_t	GetReferences( m4uint8_t ai_iLevel ) const ;

    ClTempSentence*	GetReadSentence( void ) const
    {
        return( m_poReadSentence ) ;
    }

    ClTempSentence*	GetWriteSentence( void ) const
    {
        return( m_poWriteSentence ) ;
    }

    ClTempSentence*	GetSentence( void ) const
    {
        return( m_poSentence ) ;
    }

    m4uint16_t  GetNumberOfItems() const
    {
        return( m_oItems.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfInherits() const
    {
        return( m_oInherits.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfTriggers() const
    {
        return( m_oTriggers.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfItemInherits() const
    {
        return( m_oItemInherits.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfConcepts() const
    {
        return( m_oConcepts.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfTotals() const
    {
        return( m_oTotals.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfTotalTags() const
    {
        return( m_oTotalTags.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfTags() const
    {
        return( m_oTags.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfItemFormulas() const
    {
        return( m_oItemFormulas.GetLength() ) ;
    }


	ClTempTiInherit*	GetInheritByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempItem*			GetItemByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempItemInherit*	GetItemInheritByPosition( m4uint16_t ai_iPosition ) ;
    ClTempConcept*		GetConceptByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempTrigger*		GetTriggerByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempTotal*		GetTotalByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempTotalTag*		GetTotalTagByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempTag*			GetTagByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempItemFormula*	GetItemFormulaByPosition( m4uint16_t ai_iPosition ) ;

	ClTempItem*			GetItemById				( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert ) ;


// Funciones de escritura de parte fija =======================================

    m4return_t  SetTiId( m4pcchar_t ai_pccTiId )
	{
		return( M4MdFacSetPoolId( ai_pccTiId, m_iTiId, m_poUpperPool ) ) ;
	}

    void    SetReferences( ClTempTiReference *ai_poTiReferences )
    {
        m_poTiReferences = ai_poTiReferences ;
    }

    void    SetReadSentence( ClTempSentence *ai_poReadSentence )
    {
        m_poReadSentence = ai_poReadSentence ;
    }

    void    SetWriteSentence( ClTempSentence *ai_poWriteSentence )
    {
        m_poWriteSentence = ai_poWriteSentence ;
    }

    m4return_t  SetReadObjectId( m4pcchar_t ai_pccReadObjectId )
	{
		return( M4MdFacSetPoolId( ai_pccReadObjectId, m_iReadObjectId, m_poUpperPool ) ) ;
	}

    m4return_t  SetWriteObjectId( m4pcchar_t ai_pccWriteObjectId )
	{
		return( M4MdFacSetPoolId( ai_pccWriteObjectId, m_iWriteObjectId, m_poUpperPool ) ) ;
	}

    void    SetTemporaryId( m4uint8_t ai_iTemporaryId )
    {
        m_iTemporaryId = ai_iTemporaryId ;
    }

    void    SetComplete( m4uint8_t ai_iComplete )
    {
        m_iComplete = ai_iComplete ;
    }

    void    SetCorrected( m4uint8_t ai_iCorrected )
    {
        m_iCorrected = ai_iCorrected ;
    }

    void    SetGenerateSql( m4uint8_t ai_iGenerateSql )
    {
        m_iGenerateSql = ai_iGenerateSql ;
    }

    void    SetSentenceNumber( m4int8_t ai_iSentenceNumber )
    {
        m_iSentenceNumber = ai_iSentenceNumber ;
    }

    void    SetInheritanceType( m4uint8_t ai_iInheritanceType )
    {
        m_iInheritanceType = ai_iInheritanceType ;
    }


// Otras funciones de escritura ===============================================

	void	SetNode( ClTempNode* ai_poNode, m4uint8_t ai_iLevel ) ;


// Funciones de referencia ====================================================

    void	AddInherit( void )
	{
		m_oInherits.AddElement() ;
	}

	void	InitInherits( ClTempTiInherit **ai_ppoInherits )
	{
		m_oInherits.Init( ( m4pvoid_t* ) ai_ppoInherits ) ;
	}

    void	AddItem( void )
	{
		m_oItems.AddElement() ;
	}

	void	InitItems( ClTempItem **ai_ppoItems )
	{
		m_oItems.Init( ( m4pvoid_t* ) ai_ppoItems ) ;
	}

    void	AddItemInherit( void )
	{
		m_oItemInherits.AddElement() ;
	}

	void	InitItemInherits( ClTempItemInherit **ai_ppoItemInherits )
	{
		m_oItemInherits.Init( ( m4pvoid_t* ) ai_ppoItemInherits ) ;
	}

    void	AddConcept( void )
	{
		m_oConcepts.AddElement() ;
	}

	void	InitConcepts( ClTempConcept **ai_ppoConcepts )
	{
		m_oConcepts.Init( ( m4pvoid_t* ) ai_ppoConcepts ) ;
	}

    void	AddTrigger( void )
	{
		m_oTriggers.AddElement() ;
	}

	void	InitTriggers( ClTempTrigger **ai_ppoTriggers )
	{
		m_oTriggers.Init( ( m4pvoid_t* ) ai_ppoTriggers ) ;
	}

    void	AddTotal( void )
	{
		m_oTotals.AddElement() ;
	}

    void	AddTotalTag( void )
	{
		m_oTotalTags.AddElement() ;
	}

    void	AddTag( void )
	{
		m_oTags.AddElement() ;
	}

	void	InitTotals( ClTempTotal **ai_ppoTotals )
	{
		m_oTotals.Init( ( m4pvoid_t* ) ai_ppoTotals ) ;
	}

	void	InitTotalTags( ClTempTotalTag **ai_ppoTotalTags )
	{
		m_oTotalTags.Init( ( m4pvoid_t* ) ai_ppoTotalTags ) ;
	}

	void	InitTags( ClTempTag **ai_ppoTags )
	{
		m_oTags.Init( ( m4pvoid_t* ) ai_ppoTags ) ;
	}

    void	AddItemFormula( void )
	{
		m_oItemFormulas.AddElement() ;
	}

	void	InitItemFormulas( ClTempItemFormula **ai_ppoItemFormulas )
	{
		m_oItemFormulas.Init( ( m4pvoid_t* ) ai_ppoItemFormulas ) ;
	}


// Funciones de serialización =================================================

	m4uint32_t	GetHandle( void ) const
	{
		return( m_iHandle ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

	void	SetNoUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poNoUpperPool = ai_poPool ;
	}

    m4return_t	GetSize   ( m4uint32_t &ao_iSize, m4uint16_t ai_iInheritLevel, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile   ( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize, m4uint16_t ai_iInheritLevel ) ;


// Funciones de client server =================================================

	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetIsToCompile( void )
    {
        return( m_iIsToCompile ) ;
    }

	void	SetItemsToCompile( m4uint16_t ai_iItems )
	{
		m_iItemsToCompile = ai_iItems ;
	}


// Funciones varias ===========================================================

    m4return_t	CalculateDynamicSentence( void ) ;
    m4return_t	CalculateInheritedSentence( M4HsParser *ai_poHsParser ) ;

} ;


#endif

