
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrt.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz del run time de metadatos
//
//
//==============================================================================


#include <stdio.h>

#include "m4mdrt_dll.hpp"
#include "m4mdrtde.hpp"

#include "m4types.hpp"
#include "cldefine.hpp"
#include "cldates.hpp"
#include "m4serial.hpp"
#include "chlog.hpp"
#include "m4var.hpp"
#include "datedco.hpp"
#include "cachebase.hpp"
#include "diskpers.hpp"
#include "m4cachetypes.hpp"
#include "vmmcrdef.hpp"

#ifndef __M4MDRT_HPP__
#define __M4MDRT_HPP__

//=============================================================================
// Globales
//=============================================================================

const m4uint16_t g_aiFlagToFunc[]={
	M4CL_VM_FT_READCOMMENT						,//M4CL_VM_EMB_INS_READCOMMENT 
	M4CL_VM_FT_INCONDITIONALJUMP				,//M4CL_VM_EMB_INS_INCOND_JUMP
	M4CL_VM_FT_CONDITIONALJUMP					,//M4CL_VM_EMB_INS_COND_JUMP
	M4CL_VM_FT_ASSIGNVAR						,//M4CL_VM_EMB_INS_ASSIGN
	M4CL_VM_FT_READVAR							,//M4CL_VM_EMB_INS_READ
	M4CL_VM_FT_PURGESTACK						,//M4CL_VM_EMB_INS_PURGE
	M4CL_VM_FT_RETURN							,//M4CL_VM_EMB_INS_RETURN
	M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM ,//M4CL_VM_EMB_INS_ASSIGN_VAR_ARGS
	M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM	//M4CL_VM_EMB_INS_READ_VAR_ARGS

};



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	m4VariantType ;
class	ClCompiledMCR ;
class	ClCMCRWrapper ;
class	ClCSCRWrapper ;
class	ClCacheableObject ;
class	ClLongString ;
class	ClDecodeFormula ;
class	ClDecodeInstruction ;
class	ClMappedMemoryManager;



//=================================================================================
// Funciones de empaquetado
//=================================================================================

inline	void	M4_DECL_M4MDRT M4ClPackInt8( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4uint8_t ai_iNumber )
{
	*( ( m4uint8_t* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_iNumber ;
	ao_riOffset += M4_SIZE_OF_INT8 ;
}

inline	void	M4_DECL_M4MDRT M4ClPackInt16( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4uint16_t ai_iNumber )
{
	*( ( m4uint16_t* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_iNumber ;
	ao_riOffset += M4_SIZE_OF_INT16 ;
}

inline	void	M4_DECL_M4MDRT M4ClPackInt32( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4uint32_t ai_iNumber )
{
	*( ( m4uint32_t* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_iNumber ;
	ao_riOffset += M4_SIZE_OF_INT32 ;
}

inline	void	M4_DECL_M4MDRT M4ClPackDouble  ( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4double_t ai_dNumber )
{
	*( ( m4double_t* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_dNumber ;
	ao_riOffset += M4_SIZE_OF_DOUBLE ;
}

inline	void	M4_DECL_M4MDRT M4ClPackDate( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4date_t ai_dDate )
{
	*( ( m4date_t* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_dDate ;
	ao_riOffset += M4_SIZE_OF_DATE ;
}

inline	void	M4_DECL_M4MDRT M4ClPackVariant ( m4pchar_t ai_pcBuffer, m4uint32_t &ao_riOffset, m4VariantData ai_uData )
{
	*( ( m4VariantData* ) ( ai_pcBuffer + ao_riOffset ) ) = ai_uData ;
	ao_riOffset += M4_SIZE_OF_VARIANT_DATA ;

}

//=================================================================================
// Funciones de desempaquetado
//=================================================================================

inline	m4uint8_t	M4_DECL_M4MDRT M4ClUnPackInt8( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4uint8_t* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}

inline	m4uint16_t	M4_DECL_M4MDRT M4ClUnPackInt16( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4uint16_t* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}

inline	m4uint32_t	M4_DECL_M4MDRT M4ClUnPackInt32( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4uint32_t* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}

inline	m4date_t	M4_DECL_M4MDRT M4ClUnPackDate( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4date_t* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}

inline	m4double_t  M4_DECL_M4MDRT M4ClUnPackDouble( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4double_t* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}

inline	m4VariantData	M4_DECL_M4MDRT M4ClUnPackVariant ( m4pcchar_t ai_pcBuffer, m4uint32_t ai_riOffset )
{
	return( *( ( m4VariantData* ) ( ai_pcBuffer + ai_riOffset ) ) ) ;
}



//=================================================================================
// Funciones de log
//=================================================================================

// Instanciacion de las Clase para manipulador con 2 arg. para acceder a las funciones de Log del MCR.
typedef BasicLogManip2< ChLog, ClCompiledMCR*, m4uint32_t >		ChLogManip2_mcr_t ;


// Funciones-Manipuladores con 1 arg, que llaman al manipulador final con 2 args.
ChLogManip2_mcr_t	M4_DECL_M4MDRT	M4Obj			( ClCompiledMCR *poCMCR ) ;
ChLogManip2_mcr_t	M4_DECL_M4MDRT	M4ObjExtended	( ClCompiledMCR *poCMCR ) ;
ChLogManip2_mcr_t	M4_DECL_M4MDRT	M4ObjNodeN		( ClCompiledMCR *poCMCR, m4uint32_t hNode ) ; 
ChLogManip2_mcr_t	M4_DECL_M4MDRT	M4ObjNodeI		( ClCompiledMCR *poCMCR, m4uint32_t hItem ) ; 
ChLogManip2_mcr_t	M4_DECL_M4MDRT	M4ObjNodeItemI	( ClCompiledMCR *poCMCR, m4uint32_t hItem ) ; 
ChLogManip2_mcr_t	M4_DECL_M4MDRT	NodeN			( ClCompiledMCR *poCMCR, m4uint32_t hNode ) ;
ChLogManip2_mcr_t	M4_DECL_M4MDRT	NodeI			( ClCompiledMCR *poCMCR, m4uint32_t hItem ) ;
ChLogManip2_mcr_t	M4_DECL_M4MDRT	NodeItemI		( ClCompiledMCR *poCMCR, m4uint32_t hItem ) ;
ChLogManip2_mcr_t	M4_DECL_M4MDRT	ItemI			( ClCompiledMCR *poCMCR, m4uint32_t hItem ) ;



//=================================================================================
//
// ClLaneMCR
//
// Clase para la información del carril en el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClLaneMCR
{

public:

	m4uint32_t		m_iSonNodeHandle ;
	m4uint16_t		m_iNodeItems ;
	m4uint16_t		m_iBlockItems ;
	m4uint16_t		m_iRegisterItems ;
	m4char_t		m_acFatherChannelId	[ M4CL_MAX_T3_ID   + 1 ] ;
	m4char_t		m_acFatherNodeId	[ M4CL_MAX_NODE_ID + 1 ] ;
	m4char_t		m_acSonNodeId		[ M4CL_MAX_NODE_ID + 1 ] ;
	m4pchar_t		m_pcLaneInfo ;


		ClLaneMCR( void ) ;
		ClLaneMCR( const ClLaneMCR &ai_roLane, ClCompiledMCR *ai_poMCR ) ;

		~ClLaneMCR( void ) ;

	void	Delete( void ) ;

} ;



//=================================================================================
//
// ClMCRDependentsBase
//
// Clase base para la información de los dependientes en el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClMCRDependentsBase
{

public:

		ClMCRDependentsBase( void ) ;
		virtual ~ClMCRDependentsBase( void ) ;


	m4return_t	Calculate( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) ;

	m4uint32_t	GetDependent( m4uint16_t ai_iPosition ) const ;

	m4uint16_t	GetLength( void ) const
	{
		return( m_iLength ) ;
	}

protected:

	m4uint32_t		m_iItemHandle ;
	m4uint16_t		m_iSize ;
	m4uint16_t		m_iLength ;
	m4uint32_t		*m_piDependents ;

	m4return_t	_Increment( void ) ;
	m4return_t	_AddDependent( m4uint32_t ai_iDependent ) ;

	virtual m4uint16_t	_GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const = 0 ; 
	virtual m4uint32_t	_GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const = 0  ;

} ;

//=================================================================================
//
// ClMCRDependents
//
// Clase para la información de los dependientes correspondientes a los condicionantes
// en el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClMCRDependents : public ClMCRDependentsBase
{
	m4uint16_t	_GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const ;

	m4uint32_t	_GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const ;
} ;

//=================================================================================
//
// ClMCRAssignedDependents
//
// Clase para la información de los dependientes correspondientes a los asignados
// en el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClMCRAssignedDependents : public ClMCRDependentsBase
{
	m4uint16_t	_GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const ;

	m4uint32_t	_GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const ;
	
} ;

//=================================================================================
//
// ClMCRExecutedDependents
//
// Clase para la información de los dependientes correspondientes a los ejecutados
// en el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClMCRExecutedDependents : public ClMCRDependentsBase
{
	m4uint16_t	_GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const ;

	m4uint32_t	_GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const ;
} ;





//=================================================================================
//
// ClMdStatistics
//
// Clase que implementa la definición de las estadísticas
//
//=================================================================================

#define	M4MDRT_STT_M4OBJ								0
#define	M4MDRT_STT_CONNECTOR							1
#define	M4MDRT_STT_FILTER								2
#define	M4MDRT_STT_FORMULAS								3
#define	M4MDRT_STT_INDEX								4
#define	M4MDRT_STT_INSTANCE								5
#define	M4MDRT_STT_T3_ITEM_CON							6
#define	M4MDRT_STT_ITEM_METHOD							7
#define	M4MDRT_STT_ITEM_PROPERTY						8
#define	M4MDRT_STT_ITEM_FIELD							9
#define	M4MDRT_STT_ITEM_CONCEPT							10
#define	M4MDRT_STT_GROUP								11
#define	M4MDRT_STT_ITEM_CON								12
#define	M4MDRT_STT_RITEM_METHOD							13
#define	M4MDRT_STT_RITEM_PROPERTY						14
#define	M4MDRT_STT_RITEM_FIELD							15
#define	M4MDRT_STT_RITEM_CONCEPT						16
#define	M4MDRT_STT_NITEM								17
#define	M4MDRT_STT_NODE									18
#define	M4MDRT_STT_T3_CON								19
#define	M4MDRT_STT_SENTENCE								20
#define	M4MDRT_STT_TI									21
#define	M4MDRT_STT_BUSINESS_METHOD						22

#define	M4MDRT_STT_TYPE_NUMBER							23


class	ClMdStatistics
{

protected:

	m4uint32_t		m_aiTimes [ M4MDRT_STT_TYPE_NUMBER ] ;
	m4uint32_t		m_aiValues[ M4MDRT_STT_TYPE_NUMBER ] ;


public:


// Funciones de inicialización ================================================
	
        ClMdStatistics( void )
		{
			Reset() ;
		}

        ~ClMdStatistics( void ) {} ;

		void	Reset( void )
		{
			memset( m_aiTimes, 0, sizeof( m4uint32_t ) * M4MDRT_STT_TYPE_NUMBER ) ;
			memset( m_aiValues, 0, sizeof( m4uint32_t ) * M4MDRT_STT_TYPE_NUMBER ) ;
		}


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetTimes( m4uint32_t ai_iType ) const
	{
		if( ai_iType < M4MDRT_STT_TYPE_NUMBER )
		{
			return( m_aiTimes[ ai_iType ] ) ;
		}
		return( 0 ) ;
	}

	m4uint32_t	GetValue( m4uint32_t ai_iType ) const
	{
		if( ai_iType < M4MDRT_STT_TYPE_NUMBER )
		{
			return( m_aiValues[ ai_iType ] ) ;
		}
		return( 0 ) ;
	}


// Funciones de escritura de parte fija =======================================

	void	AddValue( m4uint32_t ai_iType, m4uint32_t ai_iValue )
	{
		if( ai_iType < M4MDRT_STT_TYPE_NUMBER )
		{
			m_aiTimes[ ai_iType ]++ ;
			m_aiValues[ ai_iType ] += ai_iValue ;
		}
	}

} ;




//=================================================================================
//
// ClCompiledMCR
//
// Clase con todo el metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT ClCompiledMCR
{

protected:

    m4pchar_t				m_pcBase ;
    m4pchar_t				m_pcSecurity ;

	ClCMCRWrapper			*m_poCMCRWrapper ;
	ClCSCRWrapper			*m_poCSCRWrapper ;

    m4uint8_t				m_iCacheCsType ;	// Para las cachés
	ClLaneMCR				m_oLane ;			// Para el carril
	ClMCRDependents			m_oDependents ;		// Para los dependientes condicionantes
	ClMCRAssignedDependents	m_oAssignedDependents ;		// Para los dependientes asignados
	ClMCRExecutedDependents	m_oExecutedDependents ;		// Para los dependientes ejecutados


	m4uint16_t	_SetWorkingArea( m4puint32_t ai_piWorkingArea, m4pcchar_t ai_pccString, m4uint16_t ai_iMaxLength, m4bool_t ai_iUpperCase = M4_FALSE ) const ;


// Funciones de decodificación ================================================

	ClDecodeInstruction*	_GetInstruction	( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, m4uint16_t &ai_riFunction ) const ;
	m4return_t				_ProcessAssign	( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;
	m4return_t				_ProcessIf		( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;
	m4return_t				_ProcessCall	( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;
	m4return_t				_ProcessEnd		( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;
	m4return_t				_GetCodeSource	( m4pcchar_t ai_pccCode, m4uint32_t ai_iItemHandle, ClLongString &ao_roString, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;


// Funciones de impresión =====================================================

    m4return_t  _PrintCode			( m4pcchar_t ai_pccCode, m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, ClLongString &ao_roString, FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;

    m4return_t  _PrintItem			( m4uint32_t ai_iItemHandle, ClLongString &ao_roString, FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintNode			( m4uint32_t ai_iNodeHandle, ClLongString &ao_roString, FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;

    m4return_t  _PrintPlug			( m4uint32_t ai_iPlugHandle,			FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintGroup			( m4uint32_t ai_iGroupHandle,			FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintItemDefinition( m4uint32_t ai_iItemHandle,			FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintItemConn		( m4uint32_t ai_iItemConnHandle,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintConnector		( m4uint32_t ai_iConnectorHandle,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintIndex			( m4uint32_t ai_iIndexHandle,			FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintFilter		( m4uint32_t ai_iFilterHandle,			FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintSentence		( m4uint32_t ai_iSentenceHandle,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintInstance		( m4uint32_t ai_iInstanceHandle,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintBusinessMethod( m4uint32_t ai_iBusinessMethodHandle,	FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;

    m4return_t  _PrintStatistics	( ClMdStatistics *ai_poStatistics,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
    m4return_t  _PrintM4Obj			( ClMdStatistics *ai_poStatistics,		FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;

    m4return_t  _PrintSecurity		(										FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;

public:

// Funciones de inicialización ================================================
	
		ClCompiledMCR( void ) ;
		ClCompiledMCR( ClCompiledMCR * ai_pMCR ) ;

		~ClCompiledMCR( void ) ;

	void Destroy( m4bool_t ai_bRemoveFromCache = M4_FALSE ) ;
	void RemoveFromCache( void ) ;


	m4return_t	SetCMCRWrapper( ClCMCRWrapper *ai_poCMCRWrapper ) ;
	m4return_t	SetCSCRWrapper( ClCSCRWrapper *ai_poCSCRWrapper ) ;


	m4pchar_t	GetBase( void ) const
    {
        return( m_pcBase ) ;
    }

	m4pchar_t	GetSecurity( void ) const
    {
        return( m_pcSecurity ) ;
    }

    m4pcchar_t  GetPointer( m4uint32_t ai_iHandle )
    {
        return( m_pcBase + ai_iHandle ) ;
    }

	ClCMCRWrapper*	GetCMCRWrapper( void )
	{
        return( m_poCMCRWrapper ) ;
	}

	ClCSCRWrapper*	GetCSCRWrapper( void )
	{
        return( m_poCSCRWrapper ) ;
	}

// Varias =====================================================================

	m4bool_t IsEqual( const ClCompiledMCR * ai_poCMCR ) const ;

    m4return_t  PrintM4Obj   ( m4uint8_t ai_iSystemDebug, m4pcchar_t ai_pccPath, ClMdStatistics *ai_poStatistics ) ;
    m4return_t  PrintSecurity( m4uint8_t ai_iSystemDebug, m4pcchar_t ai_pccPath ) ;

// Funciones del carril =======================================================

	m4pcchar_t	GetLaneFatherChannelId( void ) const ;
	m4pcchar_t	GetLaneFatherNodeId( void ) const ;
	m4pcchar_t	GetLaneSonNodeId( void ) const ;

    m4return_t  ConnectNodeToNode( m4pcchar_t ai_pccNode,     ClCompiledMCR *ai_poFatherCMCR, m4pcchar_t ai_pccFatherChannel, m4pcchar_t ai_pccFatherNode,     m4uint32_t &ao_riPlugHandle ) ;
    m4return_t  ConnectNodeToNode( m4uint32_t ai_iNodeHandle, ClCompiledMCR *ai_poFatherCMCR, m4pcchar_t ai_pccFatherChannel, m4uint32_t ai_iFatherNodeHandle, m4uint32_t &ao_riPlugHandle ) ;
    m4return_t  DisconnectLane( void ) ;
    
// Funciones del cache ========================================================

	m4uint8_t	GetCacheCsType( void ) const
	{
		// Si es desconocido, devolvemos el CsType real del metacanal
		if( m_iCacheCsType == M4CL_CSTYPE_UNKNOWN )
		{
			return( GetChannelCsType() ) ;
		}

		return( m_iCacheCsType ) ;
	}

	void	SetCacheCsType( m4uint8_t a_iCacheCsType )
	{
		m_iCacheCsType = a_iCacheCsType ;
	}

// Funciones de Log ===========================================================

    m4pcchar_t  iM4Obj( void ) const ;
    m4pcchar_t  nM4Obj( void ) const ;

    m4pcchar_t  iNodeN( m4uint32_t ai_iNodeHandle ) const ;
    m4pcchar_t  nNodeN( m4uint32_t ai_iNodeHandle ) const ;

    m4pcchar_t  iItemI( m4uint32_t ai_iItemHandle ) const ;
    m4pcchar_t  nItemI( m4uint32_t ai_iItemHandle ) const ;
    m4pcchar_t  iNodeI( m4uint32_t ai_iItemHandle ) const ;
    m4pcchar_t  nNodeI( m4uint32_t ai_iItemHandle ) const ;

// Funciones de acceso al canal ===============================================

	m4uint32_t  GetChannelSize							( void ) const ;
	m4uint16_t  GetChannelBuild							( void ) const ;
	m4uint8_t   GetChannelTimeUnit						( void ) const ;
	m4uint8_t   GetChannelCsType						( void ) const ;
	m4uint8_t   GetChannelLanguage						( void ) const ;

	m4date_t    GetChannelStartDate						( void ) const ;
	m4date_t    GetChannelEndDate						( void ) const ;
	m4date_t    GetChannelCorStartDate					( void ) const ;
	m4date_t    GetChannelCorEndDate					( void ) const ;
	m4date_t    GetChannelVersion						( void ) const ;
	m4date_t    GetChannelCreationDate					( void ) const ;
	m4date_t    GetChannelCacheMaxPeriod				( void ) const ;

	m4pcchar_t  GetChannelId							( void ) const ;
	m4pcchar_t  GetChannelName							( void ) const ;
	m4uint32_t  GetChannelPool							( void ) const ;
	m4uint32_t  GetChannelSysLoadItem					( void ) const ;
	m4uint32_t  GetChannelSysPersistItem				( void ) const ;
	m4uint32_t  GetChannelTiSystemHandle				( void ) const ;
	m4uint32_t  GetChannelSysLoadClientHandle			( void ) const ;
	m4uint32_t  GetChannelSysPersistClientHandle		( void ) const ;
	m4uint32_t  GetChannelSysLoadServerHandle			( void ) const ;
	m4uint32_t  GetChannelSysPersistServerHandle		( void ) const ;
	m4uint32_t  GetChannelExecuteReportHandle			( void ) const ;
	m4uint32_t  GetChannelDynFilterHandle				( void ) const ;
	m4uint32_t  GetChannelEnvironmentVars				( void ) const ;
	m4uint32_t  GetChannelOwnerFlag						( void ) const ;

	m4pcchar_t  GetChannelOleClassId					( void ) const ;
	m4pcchar_t  GetChannelServiceId						( void ) const ;
	m4pcchar_t  GetChannelServiceAltId					( void ) const ;

	m4uint8_t   GetChannelIsExternal					( void ) const ;
	m4uint8_t   GetChannelType							( void ) const ;
	m4uint8_t   GetChannelHasSecurity					( void ) const ;
	m4uint8_t   GetChannelIsCacheable					( void ) const ;
	m4uint8_t   GetChannelIsMDCacheable					( void ) const ;
	m4uint8_t   GetChannelCheckConcurrency				( void ) const ;
	m4uint8_t   GetChannelOrganizationType				( void ) const ;
	m4uint8_t   GetChannelInheritLevel					( void ) const ;
	m4uint8_t   GetChannelDataLanguage					( void ) const ;
	m4uint8_t   GetChannelMetaLanguage					( void ) const ;
	m4uint8_t   GetChannelCreationType					( void ) const ;
	m4uint8_t   GetChannelStyleUse						( void ) const ;
	m4uint8_t   GetChannelWSSSecurity					( void ) const ;

	m4uint16_t  GetChannelNumberOfNodes					( void ) const ;
	m4uint32_t  GetChannelNodeHandleById				( m4pcchar_t ai_pccNodeId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelNodeHandleByPosition			( m4uint16_t ai_iNodePosition ) const ;
	m4pcchar_t  GetChannelNodeIdByPosition				( m4uint16_t ai_iNodePosition ) const ;

	m4uint16_t  GetChannelNumberOfRoots					( void ) const ;
	m4uint32_t  GetChannelRootHandleById				( m4pcchar_t ai_pccRootId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelRootHandleByPosition			( m4uint16_t ai_iRootPosition ) const ;
	m4pcchar_t  GetChannelRootIdByPosition				( m4uint16_t ai_iRootPosition ) const ;

	m4uint16_t  GetChannelNumberOfSentences				( void ) const ;
	m4uint32_t  GetChannelSentenceHandleById			( m4pcchar_t ai_pccSentenceId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelSentenceHandleByPosition		( m4uint16_t ai_iSentencePosition ) const ;
	m4pcchar_t  GetChannelSentenceIdByPosition			( m4uint16_t ai_iSentencePosition ) const ;

	m4uint16_t  GetChannelNumberOfInstances				( void ) const ;
	m4uint32_t  GetChannelInstanceHandleById			( m4pcchar_t ai_pccInstanceId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelInstanceHandleByPosition		( m4uint16_t ai_iInstancePosition ) const ;
	m4pcchar_t  GetChannelInstanceIdByPosition			( m4uint16_t ai_iInstancePosition ) const ;

	m4uint16_t  GetChannelNumberOfItems					( void ) const ;
	m4uint32_t  GetChannelItemHandleById				( m4pcchar_t ai_pccItemId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelItemHandleByPosition			( m4uint16_t ai_iItemPosition ) const ;
	m4pcchar_t  GetChannelItemIdByPosition				( m4uint16_t ai_iItemPosition ) const ;

	m4uint16_t  GetChannelNumberOfTis					( void ) const ;
	m4uint32_t  GetChannelTiHandleById					( m4pcchar_t ai_pccTiId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelTiHandleByPosition			( m4uint16_t ai_iTiPosition ) const ;
	m4pcchar_t  GetChannelTiIdByPosition				( m4uint16_t ai_iTiPosition ) const ;

	m4uint16_t  GetChannelNumberOfParameters			( void ) const ;
	m4uint32_t  GetChannelParameterHandleByPosition		( m4uint16_t ai_iParameterPosition ) const ;

	m4uint16_t  GetChannelNumberOfRSMs					( void ) const ;
	m4pcchar_t  GetChannelRSMMaskById					( m4pcchar_t ai_pccRSMId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4pcchar_t  GetChannelRSMIdByPosition				( m4uint16_t ai_iRSMPosition ) const ;
	m4pcchar_t  GetChannelRSMMaskIdByPosition			( m4uint16_t ai_iRSMPosition ) const ;
      
	m4uint16_t  GetChannelNumberOfBusinessMethods       ( void ) const ;
	m4uint32_t  GetChannelBusinessMethodHandleById		( m4pcchar_t ai_pccBusinessMethodId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetChannelBusinessMethodHandleByPosition( m4uint16_t ai_iBusinessMethodPosition ) const ;
	m4pcchar_t  GetChannelBusinessMethodIdByPosition	( m4uint16_t ai_iBusinessMethodPosition ) const ;


// Funciones de acceso a nodos ================================================

// Cosas fijas

	m4uint32_t  GetNodeReadSentenceHandle               ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeWriteSentenceHandle              ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeReadSentenceId                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeWriteSentenceId                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeReadObjectId                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeWriteObjectId                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeTemporality                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeCompleteness                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodePeriodCorrected                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeOwnSentence                      ( m4uint32_t ai_iNodeHandle ) const ;

// Cosas variables

	m4pcchar_t  GetNodeId                               ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeM4ObjId                          ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeName                             ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeTiId                             ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeGroupObjects						( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeStartDateItem                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeEndDateItem                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeStartCorDateItem                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeEndCorDateItem                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeVirtualFlagItem                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeFilterStartDateItem              ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeFilterEndDateItem                ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeFilterStartCorDateItem           ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeFilterEndCorDateItem             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeCurrencyTypeItem                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeCurrencyExchDateItem             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeChangeReasonItem                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeDmdComponentItem                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeDmdValueItem                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeLoadSQLItem                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodePriorityItem                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeImputeDateItem                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodePayDateItem                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodePayTypeItem                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodePayFrequencyItem                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeImputePayTypeItem                ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeImputePayFrequencyItem           ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeClosingDateItem                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeSysParamsItem                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeBDLChecksItem                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeRAMOrderByItem                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeOrganizationIdItem               ( m4uint32_t ai_iNodeHandle ) const ;

	m4uint32_t  GetNodeLoadBlkItem						( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeDeleteBlkItem                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeUpdateBlkItem                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeInsertBlkItem                    ( m4uint32_t ai_iNodeHandle ) const ;

	m4uint32_t  GetNodeDefaultIndex                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeNumRows                          ( m4uint32_t ai_iNodeHandle ) const ;
	m4int32_t   GetNodeNumKeepRows                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeNumRowsDB						( m4uint32_t ai_iNodeHandle ) const ;

	m4uint32_t  GetNodeOleDispId                        ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeOleNodeInterId                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeMaxNumBlocks                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeMaxNumRecords                    ( m4uint32_t ai_iNodeHandle ) const ;

	m4uint16_t  GetNodeIndex                            ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeType                             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeOrder                            ( m4uint32_t ai_iNodeHandle ) const ;

	m4uint8_t   GetNodeCapacity                         ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsRoot                           ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsRealRoot						  ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeAutoLoad                         ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsOrdered                        ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsVisible                        ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeDynFilter                        ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIdAutoFilter                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsExternal                       ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsQBFFilter                      ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeAffectsDB						( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeDBReload							( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeTiInheritLevel					( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeIsOverWritten					( m4uint32_t ai_iNodeHandle ) const ;

	m4uint16_t  GetNodeNumberOfItems                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfNodeItems                ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfBlockItems               ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfRegisterItems            ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeItemHandleById                   ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccItemId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeItemHandleByPosition             ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iItemPosition ) const ;
	m4pcchar_t  GetNodeItemIdByPosition                 ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iItemPosition ) const ;

	m4uint16_t  GetNodeNumberOfConnectors               ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfRBConnectors             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfBBConnectors             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfNRConnectors             ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeRBFirstConnector                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeBBFirstConnector                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNRFirstConnector                 ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint16_t  GetNodeNumberOfAutoConnectors			( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeConnectorHandleByPosition        ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iConnectedPosition ) const ;

	m4uint16_t  GetNodeNumberOfInvConnectors            ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeInvConnectorHandleByPosition     ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iInvConnectedPosition ) const ;

	m4uint16_t  GetNodeNumberOfGroups                   ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeGroupHandleById                  ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccGroupId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeGroupHandleByPosition            ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iGroupPosition ) const ;
	m4pcchar_t  GetNodeGroupIdByPosition                ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iGroupPosition ) const ;

	m4uint16_t  GetNodeNumberOfPlugs                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodePlugHandleByIds                  ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFatherChannelId, m4pcchar_t ai_pccFatherNodeId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodePlugHandleByPosition             ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const ;
	m4pcchar_t  GetNodePlugFatherChanneIdByPosition     ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const ;
	m4pcchar_t  GetNodePlugFatherNodeIdByPosition       ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const ;

	m4uint16_t  GetNodeNumberOfIndexes                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeIndexHandleById                  ( m4uint32_t ai_iNodeHandle, m4uint32_t ai_iIndexId ) const ;
	m4uint32_t  GetNodeIndexHandleByPosition            ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iIndexPosition ) const ;
	m4uint32_t  GetNodeIndexIdByPosition                ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iIndexPosition ) const ;

	m4uint16_t  GetNodeNumberOfAlias                    ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeAliasNodeHandleById              ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAliasId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4pcchar_t  GetNodeAliasIdByPosition                ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iAliasPosition ) const ;
	m4uint32_t  GetNodeAliasNodeHandleByPosition        ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iAliasPosition ) const ;

	m4uint32_t  GetNodeAliasNodeHandleByAlias           ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAlias, m4uint8_t ai_iLevel, m4bool_t ai_iUpperCase = M4_FALSE ) const;

	m4uint16_t  GetNodeNumberOfT3Alias                  ( m4uint32_t ai_iNodeHandle ) const ;
	m4pcchar_t  GetNodeT3AliasInstanceById              ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3AliasId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeT3AliasInstanceHandleById        ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3AliasId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4pcchar_t  GetNodeT3AliasIdByPosition              ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const ;
	m4pcchar_t  GetNodeT3AliasInstanceByPosition        ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const ;
	m4uint32_t  GetNodeT3AliasInstanceHandleByPosition  ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const ;

	m4uint32_t  GetNodeT3AliasInstanceHandleByAlias     ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAlias, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4pcchar_t  GetNodeT3AliasInstanceIdByAlias         ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAlias, m4bool_t ai_iUpperCase = M4_FALSE ) const ;

	m4uint16_t  GetNodeNumberOfDmds                     ( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeDmdHandleByIds                   ( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccDmdId, m4pcchar_t ai_pccDmdComponentId, m4pcchar_t ai_pccDmdFieldId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeDmdHandleByPosition              ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const ;
	m4pcchar_t  GetNodeDmdDmdComponentIdByPosition      ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const ;
	m4pcchar_t  GetNodeDmdDmdFieldIdByPosition          ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const ;
	m4pcchar_t  GetNodeDmdDmdIdByPosition               ( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const ;

	m4uint16_t  GetNodeNumberOfQBFFilters				( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeQBFFilterHandleById				( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFilterId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeQBFFilterHandleByPosition		( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const ;
	m4pcchar_t  GetNodeQBFFilterIdByPosition			( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const ;

	m4uint16_t  GetNodeNumberOfFindFilters				( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeFindFilterHandleById				( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFilterId, m4bool_t ai_iUpperCase = M4_FALSE ) const ;
	m4uint32_t  GetNodeFindFilterHandleByPosition		( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const ;
	m4pcchar_t  GetNodeFindFilterIdByPosition			( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const ;

	m4uint16_t  GetNodeNumberOfSysSentences				( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeSysSentenceHandleByPosition		( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iSysSentencePosition ) const ;

	m4uint16_t  GetNodeNumberOfSysHints					( m4uint32_t ai_iNodeHandle ) const ;
	m4uint32_t  GetNodeSysHintHandleByPosition			( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iSysHintPosition ) const ;

// Funciones de acceso a tis ==================================================

	m4pcchar_t  GetTiId									( m4uint32_t ai_iTiHandle ) const ;
	m4uint16_t  GetTiNumberOfItems                      ( m4uint32_t ai_iTiHandle ) const ;
	m4uint32_t  GetTiItemHandleByPosition               ( m4uint32_t ai_iTiHandle, m4uint16_t ai_iItemPosition ) const ;

	m4uint32_t  GetTiReferenceNodeHandleByPosition      ( m4uint32_t ai_iTiHandle, m4uint16_t ai_iReferencePosition ) const ;
	m4uint32_t  GetTiReferenceNumberByPosition          ( m4uint32_t ai_iTiHandle, m4uint16_t ai_iReferencePosition ) const ;

// Funciones de acceso a items ================================================

// Cosas fijas

	void		GetItemDefaultValue                 ( m4uint32_t ai_iItemHandle, m4VariantType &ao_rvValue ) const ;

	m4pcchar_t	GetItemId                           ( m4uint32_t ai_iItemHandle ) const ;
	m4int32_t	GetItemSynonymId                    ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemTiId                         ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemDmd                          ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemDmdComponent                 ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemDmdCrosstab                  ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemDmdField                     ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemDBPrecision                  ( m4uint32_t ai_iItemHandle ) const ;
	m4int32_t	GetItemPrecision                    ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemMaxSize                      ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemReadObjectAlias              ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemWriteObjectAlias             ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemReadFieldId                  ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemWriteFieldId                 ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemReadObjectId                 ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemWriteObjectId                ( m4uint32_t ai_iItemHandle ) const ;

	m4uint16_t	GetItemOrder                        ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemFlag							( m4uint32_t ai_iItemHandle, m4uint16_t ai_iFlag ) const ;

	m4uint8_t	GetItemType	                        ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemTheoreticScope               ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemM4Type                       ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemCppType                      ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemScale                        ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemSliceBhrv                    ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemSliceSplit                   ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIdTotalize                   ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemSliceTotalize                ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemTransactionMode              ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemMethodLevel                  ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsVariableLength             ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemSentTotType                  ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemVariableArguments            ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsVisible                    ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemModifyData                   ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemDefaultValueFlag             ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemInternalType                 ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemConvertFunction				( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsInherited                  ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsPublic                     ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsParameter                  ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemCheckNotNull                 ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemClientNotNull                ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemNotNull					    ( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemAffectsDB					( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemGenerateSlices				( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsPK							( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsNodal						( m4uint32_t ai_iItemHandle ) const ;

	m4uint16_t	GetItemNumberOfArguments            ( m4uint32_t ai_iItemHandle ) const ;
	m4uint16_t	GetItemNumberOfReferedArguments     ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemArgumentIdByPosition         ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4pcchar_t	GetItemArgumentNameByPosition		( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4uint16_t	GetItemArgumentPrecisionByPosition  ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4uint8_t	GetItemArgumentTypeByPosition       ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4uint8_t	GetItemArgumentM4TypeByPosition     ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4int16_t	GetItemArgumentPositionById			( m4uint32_t ai_iItemHandle, m4pcchar_t ai_pccArgumentId ) const ;

// Cosas variables

	m4uint32_t	GetItemDefinitionHandle	            ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemNodeHandle                   ( m4uint32_t ai_iItemHandle ) const ;
	m4pcchar_t	GetItemName                         ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemInverseConnector             ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemSliceItem                    ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemAuxItem                      ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemAuxItemProrat                ( m4uint32_t ai_iItemHandle ) const ;

	m4uint16_t	GetItemIndex                        ( m4uint32_t ai_iItemHandle ) const ;

	m4uint8_t	GetItemIsMyItem						( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemScope                        ( m4uint32_t ai_iItemHandle ) const ;

	m4uint16_t	GetItemNumberOfConnectedItems       ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemConnectorHandleByPosition    ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iConnectorPosition ) const ;

	m4uint16_t	GetItemNumberOfConditionants        ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t	GetItemConditionantHandleByPosition ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iConditionantPosition ) const ;

	m4uint16_t	GetItemNumberOfDependents			( m4uint32_t ai_iItemHandle ) ;
	m4uint32_t	GetItemDependentHandleByPosition	( m4uint32_t ai_iItemHandle, m4uint16_t ai_iDependentPosition ) ;

	m4uint16_t	GetItemNumberOfAssigned             ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t  GetItemAssignedHandleByPosition     ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iAssignedPosition ) const ;

	m4uint16_t	GetItemNumberOfAssignedDependents			( m4uint32_t ai_iItemHandle ) ;
	m4uint32_t  GetItemAssignedDependentHandleByPosition	( m4uint32_t ai_iItemHandle, m4uint16_t ai_iAssignedPosition )  ;

	m4uint16_t	GetItemNumberOfExecuted             ( m4uint32_t ai_iItemHandle ) const ;
	m4uint32_t  GetItemExecutedHandleByPosition     ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iExecutedPosition ) const ;

	m4uint16_t	GetItemNumberOfExecutedDependents			( m4uint32_t ai_iItemHandle ) ;
	m4uint32_t  GetItemExecutedDependentHandleByPosition	( m4uint32_t ai_iItemHandle, m4uint16_t ai_iAssignedPosition )  ;

	m4uint16_t	GetItemNumberOfFormIds              ( m4uint32_t ai_iItemHandle ) const ;
	m4int16_t   GetItemFormOrderByFormId			( m4uint32_t ai_iItemHandle, m4pcchar_t ai_iFormId ) const ;
	m4pcchar_t  GetItemFormIdByPosition				( m4uint32_t ai_iItemHandle, m4uint16_t ai_iFormIdPosition ) const ;

	m4uint16_t	GetItemNumberOfSlices               ( m4uint32_t ai_iItemHandle ) const ;
	m4uint16_t	GetItemNumberOfFormulas             ( m4uint32_t ai_iItemHandle ) const ;

	m4date_t    GetItemSliceStartDate               ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iSlicePosition ) const ;
	m4date_t    GetItemSliceEndDate                 ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iSlicePosition ) const ;
	m4pcchar_t  GetItemCodeByDate                   ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4date_t ai_dSliceDate, m4uint16_t &ao_riSlicePosition ) const ;
	m4pcchar_t  GetItemCodeByPosition               ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4uint16_t ai_iSlicePosition ) const ;
	m4pcchar_t  GetItemCodeByRuleDates              ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4date_t ai_dStartDate, m4date_t ai_dEndDate ) const ;
	m4pcchar_t	GetItemNextCode						( m4uint32_t ai_iItemHandle, m4int16_t &ai_rioSequence ) const ;

// Funciones de acceso al código fuente =======================================

	m4uint32_t	GetCodeNumberOfVariales		( m4pcchar_t ai_pccCode ) const ;
	m4uint8_t	GetCodeMustAssign			( m4pcchar_t ai_pccCode ) const ;
	m4uint8_t	GetCodeAutoGenerated		( m4pcchar_t ai_pccCode ) const ;
	m4uint8_t	GetCodePriority				( m4pcchar_t ai_pccCode ) const ;
	m4uint8_t	GetCodeType					( m4pcchar_t ai_pccCode ) const ;
	m4uint8_t	GetCodeLevel				( m4pcchar_t ai_pccCode ) const ;
	m4uint32_t	GetCodeNumberOfInstructions	( m4pcchar_t ai_pccCode ) const ;
	m4uint32_t	GetCodeSize					( m4pcchar_t ai_pccCode ) const ;
	m4pcchar_t	GetCodeRuleTiId				( m4pcchar_t ai_pccCode ) const ;
	m4date_t	GetCodeStartDate			( m4pcchar_t ai_pccCode ) const ;
	m4date_t	GetCodeEndDate				( m4pcchar_t ai_pccCode ) const ;

	m4return_t	GetCodeSource				( m4pcchar_t ai_pccCode, m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, ClLongString &ao_roString, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const ;

	m4int32_t	GetNextCodeLine				( m4pcchar_t ai_pccCode, m4int32_t ai_iLine ) const ;
	m4int32_t	GetNextCodeLine				( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, m4pcchar_t ai_pccRuleId, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4int32_t ai_iLine ) const ;

// Funciones de acceso a conectores de node  ==================================

	m4uint32_t  GetNodeConnectorFatherNodeHandle    ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4uint32_t  GetNodeConnectorSonNodeHandle       ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4uint32_t  GetNodeConnectorSentenceHandle      ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4pcchar_t  GetNodeConnectorSentenceId          ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4uint8_t   GetNodeConnectorType                ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4uint8_t   GetNodeConnectorExpandDelete        ( m4uint32_t ai_iNodeConnectorHandle ) const ;
	m4uint8_t   GetNodeConnectorExpandUndo          ( m4uint32_t ai_iNodeConnectorHandle ) const ;

// Funciones de acceso a conectores de item  ==================================

	m4uint32_t  GetItemConnectorNodeConnectorHandle ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint32_t  GetItemConnectorFatherItemHandle    ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint32_t  GetItemConnectorSonItemHandle       ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint8_t   GetItemConnectorPrecedence          ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint8_t   GetItemConnectorSpin                ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint8_t   GetItemConnectorRelation            ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint8_t   GetItemConnectorContext             ( m4uint32_t ai_iItemConnectorHandle ) const ;
	m4uint8_t   GetItemConnectorCsType              ( m4uint32_t ai_iItemConnectorHandle ) const ;

// Funciones de acceso a grupos de nodos  =====================================

	m4uint16_t  GetGroupNumberOfItems           ( m4uint32_t ai_iGroupHandle ) const ;
	m4uint32_t  GetGroupItemHandleByPosition    ( m4uint32_t ai_iGroupHandle, m4uint16_t ai_iItemPosition ) const ;
    
// Funciones de acceso a plugs ================================================

	m4pcchar_t  GetPlugFatherChanneId        ( m4uint32_t ai_iPlugHandle ) const ;
	m4pcchar_t  GetPlugFatherChanneName      ( m4uint32_t ai_iPlugHandle ) const ;
	m4pcchar_t  GetPlugFatherNodeId          ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint8_t   GetPlugType                  ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint8_t   GetPlugIsFilter              ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint8_t   GetPlugRelationTypeId        ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint8_t   GetPlugIsMultiselection      ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint16_t  GetPlugNumberOfItems         ( m4uint32_t ai_iPlugHandle ) const ;
	m4uint32_t  GetPlugItemHandleByPosition  ( m4uint32_t ai_iPlugHandle, m4uint16_t ai_iItemPosition ) const ;
	m4uint32_t  GetPlugItemHandleByItemHandle( m4uint32_t ai_iPlugHandle, m4uint32_t ai_iItemHandle ) const ;

// Funciones de acceso a plug items ===========================================

	m4pcchar_t  GetPlugItemFatherItemId     ( m4uint32_t ai_iPlugItemHandle ) const ;
	m4uint32_t  GetPlugItemSonItemHandle    ( m4uint32_t ai_iPlugItemHandle ) const ;
	m4uint8_t   GetPlugItemRelation         ( m4uint32_t ai_iPlugItemHandle ) const ;

// Funciones de acceso a indices ==============================================

	m4uint32_t  GetIndexId                   ( m4uint32_t ai_iIndexHandle ) const ;
	m4uint16_t  GetIndexNumberOfItems        ( m4uint32_t ai_iIndexHandle ) const ;
	m4uint32_t  GetIndexItemHandleByPosition ( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const ;
	m4uint8_t   GetIndexFunctionByPosition   ( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const ;
	m4uint8_t   GetIndexWayByPosition        ( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const ;
    
// Funciones de acceso a indices ==============================================

	m4pcchar_t  GetFilterId					( m4uint32_t ai_iFilterHandle ) const ;
	m4pcchar_t  GetFilterT3Id				( m4uint32_t ai_iFilterHandle ) const ;
	m4pcchar_t  GetFilterNodeId				( m4uint32_t ai_iFilterHandle ) const ;
	m4pcchar_t  GetFilterSysSentenceItem	( m4uint32_t ai_iFilterHandle ) const ;
	m4pcchar_t  GetFilterSysParamItem		( m4uint32_t ai_iFilterHandle ) const ;
	m4uint8_t   GetFilterBehaviourType		( m4uint32_t ai_iFilterHandle ) const ;
	m4uint8_t   GetFilterIsOwnNode			( m4uint32_t ai_iFilterHandle ) const ;

// Funciones de acceso a una sentencia ========================================

	m4pcchar_t  GetSentenceId           ( m4uint32_t ai_iSentenceHandle ) const ;
	m4pcchar_t  GetSentenceGroupObjects ( m4uint32_t ai_iSentenceHandle ) const ;
	m4pcchar_t  GetSentenceApiSql		( m4uint32_t ai_iSentenceHandle ) const ;
	m4pcchar_t  GetSentenceApiSql2		( m4uint32_t ai_iSentenceHandle ) const ;
	m4pcchar_t  GetSentenceApiSql3		( m4uint32_t ai_iSentenceHandle ) const ;
	m4pcchar_t  GetSentenceApiSql4		( m4uint32_t ai_iSentenceHandle ) const ;
	m4uint32_t  GetSentenceMaxNumCol    ( m4uint32_t ai_iSentenceHandle ) const ;

// Funciones de acceso a una instancia ========================================

	m4pcchar_t  GetInstanceId              ( m4uint32_t ai_iInstanceHandle ) const ;
	m4pcchar_t  GetInstanceT3Id            ( m4uint32_t ai_iInstanceHandle ) const ;
	m4pcchar_t  GetInstanceRoleId          ( m4uint32_t ai_iInstanceHandle ) const ;
	m4pcchar_t  GetInstanceOrganization    ( m4uint32_t ai_iInstanceHandle ) const ;
	m4uint8_t   GetInstanceScope           ( m4uint32_t ai_iInstanceHandle ) const ;
	m4uint8_t   GetInstanceOpenMode        ( m4uint32_t ai_iInstanceHandle ) const ;
	m4uint8_t   GetInstanceCsAccess        ( m4uint32_t ai_iInstanceHandle ) const ;
	m4uint8_t   GetInstanceUseRole         ( m4uint32_t ai_iInstanceHandle ) const ;
	m4uint8_t   GetInstanceOrganizationType( m4uint32_t ai_iInstanceHandle ) const ;
	  
// Funciones de acceso a un método de negocio =================================

	m4pcchar_t  GetBusinessMethodId			( m4uint32_t ai_iBusinessMethodHandle ) const ;
	m4pcchar_t  GetBusinessMethodName		( m4uint32_t ai_iBusinessMethodHandle ) const ;
	m4pcchar_t  GetBusinessMethodDescription( m4uint32_t ai_iBusinessMethodHandle ) const ;
	m4uint32_t  GetBusinessMethodItemHandle	( m4uint32_t ai_iBusinessMethodHandle ) const ;

	m4uint16_t  GetBusinessMethodNumberOfArguments				( m4uint32_t ai_iBusinessMethodHandle ) const ;
	m4uint32_t  GetBusinessMethodArgumentNodeHandleByPosition	( m4uint32_t ai_iBusinessMethodHandle, m4uint16_t ai_iArgumentPosition ) const ;
	m4uint8_t   GetBusinessMethodArgumentTypeByPosition			( m4uint32_t ai_iBusinessMethodHandle, m4uint16_t ai_iArgumentPosition ) const ;

// Cliente servidor ===========================================================

	m4uint16_t  GetChannelVMCapacityReq	( void ) const ;
	m4uint8_t   GetChannelIsSeparable	( void ) const ;
	m4uint8_t   GetChannelCsExeType		( void ) const ;

	m4uint8_t   GetNodeCsType			( m4uint32_t ai_iNodeHandle ) const ;
	m4uint8_t   GetNodeSyncType			( m4uint32_t ai_iNodeHandle ) const ;

	m4uint8_t	GetItemCsType			( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemSyncType			( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemIsEvent			( m4uint32_t ai_iItemHandle ) const ;
	m4uint8_t	GetItemLevel			( m4uint32_t ai_iItemHandle ) const ;

// Seguridad ==================================================================

    m4uint32_t  GetSecuritySize				( void ) const ;
    m4uint16_t  GetSecurityBuild			( void ) const ;
    m4uint8_t   GetSecurityCsType			( void ) const ;
	m4date_t	GetSecurityT3Version		( void ) const ;
	m4date_t	GetSecurityOwnVersion		( void ) const ;
	m4pcchar_t  GetSecurityMaskId			( void ) const ;
	m4pcchar_t  GetSecurityChannelId		( void ) const ;
    m4uint16_t  GetSecurityNumberOfNodes	( void ) const ;

	m4uint16_t  GetChannelSecurity		    ( void ) const ;

	m4uint8_t   GetChannelSecRead			( void ) const ;
    m4uint8_t   GetChannelSecWrite			( void ) const ;
    m4uint8_t   GetChannelSecDelete			( void ) const ;
    m4uint8_t   GetChannelSecUpdate			( void ) const ;
    m4uint8_t   GetChannelSecExecute		( void ) const ;

	m4uint16_t  GetNodeSCRStart			    ( m4uint32_t ai_iNodeHandle ) const ;

    m4uint8_t   GetNodeSecRead				( m4uint32_t ai_iNodeHandle ) const ;
    m4uint8_t   GetNodeSecWrite				( m4uint32_t ai_iNodeHandle ) const ;
    m4uint8_t   GetNodeSecDelete			( m4uint32_t ai_iNodeHandle ) const ;
    m4uint8_t   GetNodeSecUpdate			( m4uint32_t ai_iNodeHandle ) const ;
    m4uint8_t   GetNodeSecExecute			( m4uint32_t ai_iNodeHandle ) const ;
    m4uint8_t	GetNodeSecDynFilter			( m4uint32_t ai_iNodeHandle ) const ;
    m4pcchar_t  GetNodeSecFilter			( m4uint32_t ai_iNodeHandle ) const ;

	m4uint16_t  GetItemSCRStart			    ( m4uint32_t ai_iItemHandle ) const ;

    m4uint8_t   GetItemSecRead				( m4uint32_t ai_iItemHandle ) const ;
    m4uint8_t   GetItemSecWrite				( m4uint32_t ai_iItemHandle ) const ;
    m4uint8_t   GetItemSecDelete			( m4uint32_t ai_iItemHandle ) const ;
    m4uint8_t   GetItemSecUpdate			( m4uint32_t ai_iItemHandle ) const ;
    m4uint8_t   GetItemSecExecute			( m4uint32_t ai_iItemHandle ) const ;
    m4uint8_t   GetItemSecContinue  		( m4uint32_t ai_iItemHandle ) const ;

	m4uint16_t  GetFilterSCRStart			( m4uint32_t ai_iFilterHandle ) const ;

    m4uint8_t   GetFilterIsApplicable		( m4uint32_t ai_iFilterHandle ) const ;

	m4uint16_t  GetPlugSCRStart			    ( m4uint32_t ai_iPlugHandle ) const ;

    m4uint8_t   GetPlugIsApplicable			( m4uint32_t ai_iPlugHandle ) const ;

} ;



//=================================================================================
//
// ClCMCRWrapper
//
// Clase para el wrapper de metadatos
//
//=================================================================================

class	M4_DECL_M4MDRT	ClCMCRWrapper : public ClPersistiableObject
{

protected:

    m4pchar_t				m_pcBase ;
	ClMappedMemoryManager	*m_poMMManager ;
	ClCacheableObject		*m_pCO ;

	void	_Reset( void ) ;

public:

		ClCMCRWrapper( void ) ;
		ClCMCRWrapper( ClCMCRWrapper *ai_pCMCRWRapper ) ;

		~ClCMCRWrapper( void ) ;

	void Destroy( m4bool_t ai_bRemoveFromCache = M4_FALSE ) ;

	void RemoveFromCache( void ) ;

	m4pchar_t	CreateBuffer( m4uint32_t ai_iSize ) ;

	m4pchar_t	GetBuffer( void )
	{
		return( m_pcBase ) ;
	}

	m4uint32_t	GetSize( void ) const ;

    m4date_t    GetStartDate( void ) const ;
    m4date_t    GetEndDate( void ) const ;
    m4date_t    GetCorStartDate( void ) const ;
    m4date_t    GetCorEndDate( void ) const ; 
    m4date_t    GetVersion( void ) const ;
    m4uint8_t	GetMetaLanguage( void ) const ;
    m4uint8_t	GetIsMDCacheable( void ) const ;

    m4return_t  Serialize( ClGenericIODriver &IOD, m4bool_t ai_bConvert ) ;

	m4return_t  Serialize( ClGenericIODriver &IOD )
	{
		return Serialize( IOD, M4_FALSE ) ;
	}

    m4return_t  DeSerialize( ClGenericIODriver &IOD ) ;

	void SetCO( ClCacheableObject *ai_pCO )
	{
		m_pCO = ai_pCO;
	}

	ClCacheableObject*	GetCO( void ) const
    {
        return( m_pCO ) ;
    }

} ;


//=================================================================================
//
// ClCMCRWrapper
//
// Clase para el wrapper de seguridad
//
//=================================================================================

class	M4_DECL_M4MDRT	ClCSCRWrapper : public ClPersistiableObject
{

protected:

    m4pchar_t			m_pcSecurity ;
	ClCacheableObject	*m_pCO ;

public:

		ClCSCRWrapper( void ) ;
		ClCSCRWrapper( ClCSCRWrapper *ai_pCSCRWRapper ) ;

		~ClCSCRWrapper( void ) ;

	void	Destroy( m4bool_t ai_bRemoveFromCache = M4_FALSE ) ;

	void	RemoveFromCache( void );

	void	SetBuffer( m4pchar_t ai_pcSecurity )
	{
		m_pcSecurity = ai_pcSecurity ;
	}

	m4pchar_t	GetBuffer( void )
	{
		return( m_pcSecurity ) ;
	}

	m4uint32_t	GetSize( void ) const ;

    m4date_t    GetStartDate( void ) const
    {
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ; 
    }

    m4date_t    GetEndDate( void ) const
    {
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
    }

    m4date_t    GetCorStartDate( void ) const
    {
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
    }

    m4date_t    GetCorEndDate( void ) const
    {
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
    }

    m4date_t    GetVersion( void ) const ;
	m4date_t    GetT3Version( void ) const ;

    m4return_t  Serialize( ClGenericIODriver &IOD ) ;
    m4return_t  DeSerialize( ClGenericIODriver &IOD ) ;

	void SetCO( ClCacheableObject *ai_pCO )
	{
		m_pCO = ai_pCO ;
	}

	ClCacheableObject*	GetCO( void ) const
    {
        return( m_pCO ) ;
    }

} ;




//=================================================================================
//
// ClCMCRContainer
//
// Clase container de objeto cacheable metacanal compilado
//
//=================================================================================

class	ClCMCRContainer : public ClDatedContainer <ClCMCRWrapper>					   
{
public:
	ClCMCRContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0) : ClDatedContainer <ClCMCRWrapper> (ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
	{
	};

	virtual m4uint8_t GetType(void)
	{
		return M4_CO_FACTORY_CMCR_CONTAINER;
	};

	virtual ClObjectId * CreateObjectIdFromKey(void)
	{
		m4pchar_t pcId;

		pcId = m_poClCacheKey->Getm_pszKey();
		ClTypedId oTypeID(pcId);
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( GetEnvId() );
		ClObjectId * poObjID = new ClObjectId (MetaChannelObjectType, &oEnvId, &oTypeID, &oUniqueKey);
	
		return poObjID;
	}

	virtual m4bool_t MatchCondition (void * ai_pCondition);

};



//=================================================================================
//
// ClCMCRCacheBase_t
//
//=================================================================================

typedef	ClBaseCache<ClCMCRWrapper, ClCMCRContainer>	ClCMCRCacheBase_t ;


//=================================================================================
//
// ClCMCRContainer
//
// Clase caché de objeto cacheable metacanal compilado
//
//=================================================================================

class	M4_DECL_M4MDRT	ClCMCRCache : public ClCMCRCacheBase_t
{
public:

	ClCMCRCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

	~ClCMCRCache ();

	virtual m4uint16_t GetType(void)
	{
		return M4_CACHE_CMCR_CACHE_TYPE;
	}

	m4return_t GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, m4uint16_t &ao_iVerCounter, ClCMCRWrapper* &ao_rpCMCR);

	m4return_t PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage, m4uint16_t ai_iVerCounter, ClCMCRWrapper* ai_pCMCR, m4date_t ai_dMaxPeriod = 0);

	virtual m4return_t RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount = NULL);

	virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

private:
	m4pchar_t ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage);

};


//=================================================================================
//
// ClCMCRCacheableObjectFactory
//
// Clase de la factoria de CMCRCacheableObject
//
//=================================================================================

class	M4_DECL_M4MDRT ClCMCRCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{
public:
	ClCMCRCacheableObjectFactory(void)
	{
	};

	~ClCMCRCacheableObjectFactory(void)
	{
	};

private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject)
	{
		aio_pObject = new ClCMCRContainer();
		((ClCMCRContainer*)aio_pObject)->SetPersistor(this);
		return M4_SUCCESS;
	}
};


//=================================================================================
//
// ClCSCRContainer
//
// Clase container de objeto cacheable seguridad compilada
//
//=================================================================================

class	ClCSCRContainer : public ClDatedContainer <ClCSCRWrapper>					   
{
private:
	//VersionT3
	m4date_t m_dT3Version;
public:

	ClCSCRContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0) : ClDatedContainer <ClCSCRWrapper> (ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
	{
		m_dT3Version = 0;
	};

	virtual m4return_t Serialize(ClGenericIODriver& IOD);

	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	virtual m4return_t SetValue ( ClCSCRWrapper * ai_pObject );

	virtual m4uint8_t GetType(void)
	{
		return M4_CO_FACTORY_CSCR_CONTAINER;
	}

	virtual ClObjectId * CreateObjectIdFromKey(void)
	{
		m4pchar_t pcId;

		pcId = m_poClCacheKey->Getm_pszKey();
		ClTypedId oTypeID(pcId);
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( GetEnvId() );
		ClObjectId * poObjID = new ClObjectId (MetaSecurityObjectType, &oEnvId, &oTypeID, &oUniqueKey);
	
		return poObjID;
	}

	virtual m4bool_t MatchCondition (void * ai_pCondition);
};



//=================================================================================
//
// ClCSCRCacheBase_t
//
//=================================================================================

typedef ClBaseCache<ClCSCRWrapper, ClCSCRContainer>	ClCSCRCacheBase_t ;


//=================================================================================
//
// ClCSCRCache
//
// Clase caché de objeto cacheable seguridad compilada
//
//=================================================================================

class	M4_DECL_M4MDRT	ClCSCRCache : public ClCSCRCacheBase_t
{
public:

	ClCSCRCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

	~ClCSCRCache ();

	virtual m4uint16_t GetType(void)
	{
		return M4_CACHE_CSCR_CACHE_TYPE;
	}

	m4return_t GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask, ClCSCRWrapper* &ao_rpCSCR);

	m4return_t PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask, ClCSCRWrapper* ai_pCMCR, m4date_t ai_dMaxPeriod = 0);

	virtual m4return_t RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount = NULL);
	
	virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

private:
	m4pchar_t ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask);
};


//=================================================================================
//
// ClCSCRCacheableObjectFactory
//
// Clase de la factoria de CSCRCacheableObject
//
//=================================================================================

class	M4_DECL_M4MDRT ClCSCRCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{
public:
	ClCSCRCacheableObjectFactory(void)
	{
	};

	~ClCSCRCacheableObjectFactory(void)
	{
	};

private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject)
	{
		aio_pObject = new ClCSCRContainer();
		((ClCSCRContainer*)aio_pObject)->SetPersistor(this);
		return M4_SUCCESS;
	}
};


//=================================================================================
//
// ClDecodeInstruction
//
// Clase decodificar instrucciones
//
//=================================================================================

// Ojo si hay cambios en los tipos de instruccion
// ver tambien mcdefine.hpp !!!
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_HEX_MASK		0xC0000000
#define M4CL_MCR_CODE_INSTRUCTION_OFFSET_HEX_MASK	0x3FFFFFFF



class	M4_DECL_M4MDRT ClDecodeInstruction
{
	const m4char_t *m_pcFirstOffsetIns;		// Puntero al Offset de la Primera Instruccion
	const m4char_t *m_pcOffsetInstruction;	// Puntero al Offset de la Instruccion
	const m4char_t *m_pcInstruction;		// Puntero a la Instruccion	

	// Cambiado el Nivel de Debug a 10... tambien en cltnform.cpp (:: Compile) y clcmcrpt.cpp (:: PrintCode)

public:

	// ############ CONSTRUCTOR #################
	
	ClDecodeInstruction ():m_pcOffsetInstruction(0), m_pcInstruction(0), m_pcFirstOffsetIns(0) {}

	ClDecodeInstruction (const m4char_t *ai_pcIniInstruction) {
		Init (ai_pcIniInstruction);
	}

	// Inicializacion
	void Init (const m4char_t *ai_pcIniInstruction){

		m_pcFirstOffsetIns	  = ai_pcIniInstruction;
		m_pcOffsetInstruction = m_pcFirstOffsetIns;
		m4int32_t iTempOffset = M4ClUnPackInt32 (m_pcOffsetInstruction, 0) ;

		//Calculamos el Offset de la instruccion
		if (M4CL_MCR_INS_EMB_IS_EMBEBBED (iTempOffset) ){
			m_pcInstruction	  = m_pcOffsetInstruction ; //si es embebida, la instrucción está en el propio offset
		}
		else{
			m_pcInstruction	  = m_pcOffsetInstruction + M4CL_MCR_INS_EMB_OFFSET(iTempOffset) ;
		}
	}


	// ############ CAMBIO DE ESTADO: Avance o Retroceso por la Formula #############
	// excepto algunos

	//Cambia el estado del objeto, moviendose al no. de Instruccion indicado
	void MoveTo (m4int32_t ai_iNumIns);

	
	// ############ OBTENCION DE DATOS DE LA INSTRUCCION #############


	m4uint16_t		GetExeFunc (void);
	m4int16_t		GetFlags(void);
	m4uint32_t		GetNumNodo(void);
	m4uint32_t		GetNumItem(void);
	m4int32_t		GetNumVar(void);
	m4uint16_t		GetNumRegla(void);
	m4uint8_t		GetNumAtrib(void);
	m4uint32_t		GetNameChannel(void);
	m4int32_t		GetVariantInt(void);
	m4double_t		GetVariantDouble(void);

	
	// ######## OPERADORES SOBRECARGADOS: Cambian el estado de la Instruccion #########

	void operator ++  (void);
	void operator --  (void);
	
	ClDecodeInstruction *operator [] (m4int32_t ai_iNumIns){
		MoveTo (ai_iNumIns);
		return this;
	}

	void operator +=  (m4int16_t ai_iNumIns){
		MoveTo (ai_iNumIns);
	}

	void operator -=  (m4int16_t ai_iNumIns){		
		MoveTo (-ai_iNumIns);
	}

	const ClDecodeInstruction *operator +  (m4int16_t ai_iNumIns){		
		MoveTo (ai_iNumIns);
		return this;
	}

	const ClDecodeInstruction *operator -  (m4int16_t ai_iNumIns){		
		MoveTo (-ai_iNumIns);
		return this;
	}

	const ClDecodeInstruction &operator =  (const ClDecodeInstruction &ai_rhs){		
		
		m_pcFirstOffsetIns		= ai_rhs.m_pcFirstOffsetIns;
		m_pcOffsetInstruction	= ai_rhs.m_pcOffsetInstruction;
		m_pcInstruction			= ai_rhs.m_pcInstruction;


		return (*this);
	}


	// ############ Serializacion/Deserializacion usando IODriver ############

	m4return_t Serialize(ClGenericIODriver& IOD);

	m4return_t DeSerialize(ClGenericIODriver& IOD){
	
		m4uint32_t iNumIns;

		m4return_t RetVal = IOD.Read(iNumIns);

		if (RetVal == M4_SUCCESS){
			MoveTo (iNumIns);
		}

		return RetVal;
	}	
};


//=================================================================================
//
// ClDecodeFormula
//
// Clase decodificar fórmulas
//
//=================================================================================

class	M4_DECL_M4MDRT ClDecodeFormula
{
	const m4char_t		*m_pcIniFormula;		// Inicio de la Formula

	/* Bug 0071482
	Inicio de la fórmula en el lado remoto, en caso de depuración JIT
	*/
	const m4char_t		*m_pcRemoteIniFormula;
	ClDecodeInstruction m_oInstruction;			// Instrucción
public:

	ClDecodeFormula(const m4char_t * ai_pcIniFormula){
		Init (ai_pcIniFormula);
		m_pcRemoteIniFormula=0;
	}

	ClDecodeFormula (void){
		m_pcIniFormula=0;
		m_pcRemoteIniFormula=0;
	}
	
	//inicialización
	void Init (const m4char_t * ai_pcIniFormula);

	const m4char_t *GetFormulaHandle (void) const {
		return m_pcIniFormula;
	}
	const m4char_t *GetRemoteFormulaHandle (void) const {
		return m_pcRemoteIniFormula;
	}

	void SetRemoteFormulaHandle (const m4char_t * ai_pcRemoteIniFormula) {
		m_pcRemoteIniFormula=ai_pcRemoteIniFormula;
	}


	m4uint16_t	GetNumVars	(void) const;
   	m4bool_t	GetMustAssign (void) const;
	m4bool_t	GetIsAutogenerated(void) const;
	m4int8_t	GetPriority (void) const;
	m4int8_t	GetType	(void) const;
	m4uint8_t	GetLevel (void) const;
	m4int32_t	GetNumberOfInstructions (void) const;
	m4int32_t	GetFormulaSize (void) const;
	m4int32_t	GetRuleTI (void) const;
	m4date_t	GetStartDate(void) const;
	m4date_t	GetEndDate(void) const;

	//operadores para manejar el 'chorro' de instrucciones
	//operadores
	ClDecodeInstruction *operator [] (m4int32_t ai_iNumIns){
			return ( m_oInstruction[ai_iNumIns] );
	}

	void operator ++  (void){
		++m_oInstruction;
	}

	void operator --  (void){
		--m_oInstruction;
	}

	void operator +=  (m4int16_t ai_iNumIns){
		m_oInstruction+=ai_iNumIns;
	}

	void operator -=  (m4int16_t ai_iNumIns){
		m_oInstruction-=ai_iNumIns;
	}

	const ClDecodeInstruction *operator +  (m4int16_t ai_iNumIns){	
		return m_oInstruction+ai_iNumIns;
	}
 
	const ClDecodeInstruction *operator -  (m4int16_t ai_iNumIns){	
		return m_oInstruction-ai_iNumIns;
	}

	m4bool_t IsNull (void) const{
		return M4_BOOL(!m_pcIniFormula);
	}

	m4bool_t IsNotNull (void) const{
		return ( m_pcIniFormula ? M4_TRUE : M4_FALSE );
	}


	//serialize-> by pipo
	m4return_t Serialize(ClGenericIODriver& IOD, const ClCompiledMCR* ai_poCMCR, m4uint32_t ai_hItem)
	{
		//serializamos el offset del inicio de la formula respecto al handle del item
		m4uint32_t PtrDiff = (m4pcchar_t)m_pcIniFormula -  ( (m4pcchar_t)ai_poCMCR->GetBase() + ai_hItem );

		m4return_t RetVal = IOD.Write(PtrDiff);
		if (RetVal == M4_SUCCESS){
			RetVal= m_oInstruction.Serialize(IOD);
		}

		return RetVal;
	}

	m4return_t DeSerialize(ClGenericIODriver& IOD, const ClCompiledMCR* ai_poCMCR, m4uint32_t ai_hItem)
	{
		m4uint32_t PtrDiff;

		m4return_t RetVal = IOD.Read(PtrDiff);
		
		

		if (RetVal == M4_SUCCESS){
			m_pcIniFormula = (m4pcchar_t)ai_poCMCR->GetBase() + PtrDiff + ai_hItem;
			m_oInstruction.Init			(m_pcIniFormula);
			
			//deserializamos la instrucción (se mueve)
			m_oInstruction.DeSerialize  (IOD);
		}

		return RetVal;
	}	

};


#endif


