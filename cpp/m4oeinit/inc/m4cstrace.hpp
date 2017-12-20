
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4cstrace.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                17-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la definición abstracta de la traza CS
//
//
//==============================================================================


#include "m4types.hpp"



#ifndef __M4CSTRACE_HPP__
#define __M4CSTRACE_HPP__


//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	IExecutor ;
class	ClCMObjectDirectory ;
class	ClBuidParameters ;
class	ClCMCRWrapper ;
class	ClCSCRWrapper ;
class	ClVMBaseEnv ;
class	m4VariantType ;
class	ClCsTraceSize;
class	ClPresentationAttributes;



//=================================================================================
//
// ClCsTrace
//
// Clase que define el interfaz abstracto de trazas CS
//
//=================================================================================

class	ClCsTrace
{

public:
	/**
	 * Constantes que definen el nivel de detalle de las trazas.
	 * Valores superiores a CS_TRACE_TRANSACTION dan más información sobre los tamaños de serialización.
	 */
	enum { CS_TRACE_DISABLE = 0, CS_TRACE_TRANSACTION, CS_TRACE_CHANNEL, CS_TRACE_NODE, CS_TRACE_BLOCK, CS_TRACE_REGISTER };

			ClCsTrace( ) {} ;
	virtual ~ClCsTrace( ) {} ;

	virtual	m4return_t	Init( ClVMBaseEnv *ai_poEnvironment );
	virtual	m4return_t	End( void );
	virtual	m4return_t	Reset( void );

	virtual	m4return_t	StartupM4ObjectEngine( void );
	virtual	m4return_t	ShutdownM4ObjectEngine( void );
	virtual	m4return_t	Logon( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer );
	virtual	m4return_t	Logoff( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer );
	virtual	m4return_t	PreMethodExecutionTrace( IExecutor *ai_poExecutor, ClCMObjectDirectory* ai_poObjectDirectory, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache );
	virtual	m4return_t	PostMethodExecutionTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccService );
	virtual	m4return_t	PreMetaDataTrace( void );
	virtual	m4return_t	PostMetaDataTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache );
//	virtual	m4return_t	PreMapTrace( void );
//	virtual	m4return_t	PostMapTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache );
	virtual	m4return_t	PrePresentationTrace( void );
	virtual	m4return_t	PostPresentationTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bFromCache );
	virtual	m4return_t	PreSecurityTrace( void );
	virtual	m4return_t	PostSecurityTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache );
	virtual	m4return_t	PrintVBTrace(m4pcchar_t ai_pccService, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccFunction, m4pcchar_t ai_pccAction, m4pcchar_t ai_pccRole, m4pcchar_t ai_pccInitTime, m4pcchar_t ai_pccEndTime, m4pcchar_t ai_pccElapsed);

	virtual	m4bool_t	IsEnabled( void ) const;

	/** Activa las trazas.
	 *
	 * @param ai_iCsTraceLevel	Nivel de detalle de las trazas.
	 *					> 1 muestra información detallada sobre los tamaños de serialización.
	 * @param ai_iVbTraceLevel	Nivel de detalle de las trazas de las acciones de usuario.
	 */
	virtual	void		TraceLevel( const m4uint8_t ai_iCsTraceLevel, const m4uint8_t ai_iVbTraceLevel );

	virtual	void		EnableCachedOperations( void );
	virtual	void		DisableCachedOperations( void );

	virtual m4return_t	SetOutputFileName( m4pcchar_t ai_pccCsFile, m4pcchar_t ai_pccVbFile );
	virtual m4return_t	GetOutputFileName (m4pchar_t ao_pzFileName);

	virtual m4return_t	SetTagName (m4pchar_t ai_pzNewTabName);
	virtual m4return_t	ResetTagName (void);

	/** Devuelve el objeto que se encarga de tomar los tamaños de serializacion. */
	virtual ClCsTraceSize* const CreateSerializeCsTraceSize();

	/** Devuelve el objeto que se encarga de tomar los tamaños de deserializacion. */
	virtual ClCsTraceSize* const CreateDeSerializeCsTraceSize();

} ;


inline m4return_t ClCsTrace::Init( ClVMBaseEnv *ai_poEnvironment )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::End( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::Reset( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::StartupM4ObjectEngine( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::ShutdownM4ObjectEngine( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::Logon( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::Logoff( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PreMethodExecutionTrace( IExecutor *ai_poExecutor, ClCMObjectDirectory* ai_poObjectDirectory, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PostMethodExecutionTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccService )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PreMetaDataTrace( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PostMetaDataTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	return( M4_SUCCESS ) ;
}

/*
inline m4return_t ClCsTrace::PreMapTrace( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PostMapTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	return( M4_SUCCESS ) ;
}
*/

inline m4return_t ClCsTrace::PrePresentationTrace( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PostPresentationTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bFromCache )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PreSecurityTrace( void )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PostSecurityTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	return( M4_SUCCESS ) ;
}

inline m4return_t ClCsTrace::PrintVBTrace(m4pcchar_t ai_pccService, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccFunction, m4pcchar_t ai_pccAction, m4pcchar_t ai_pccRole, m4pcchar_t ai_pccInitTime, m4pcchar_t ai_pccEndTime, m4pcchar_t ai_pccElapsed)
{
	return( M4_SUCCESS ) ;
}

inline m4bool_t ClCsTrace::IsEnabled( void ) const
{
	return( M4_FALSE ) ;
}

/** Activa las trazas con un nivel de detalle determinado.
 *
 * @param ai_iCsTraceLevel	Nivel de detalle de las trazas.
 *							> 1 muestra información detallada sobre los tamaños de serialización.
 * @param ai_iVbTraceLevel	Nivel de detalle de las trazas de las acciones de usuario.
 */
inline void ClCsTrace::TraceLevel( const m4uint8_t ai_iCsTraceLevel, const m4uint8_t ai_iVbTraceLevel )
{
	return ;
}

inline void ClCsTrace::EnableCachedOperations( void )
{
	return ;
}

inline void ClCsTrace::DisableCachedOperations( void )
{
	return ;
}

inline m4return_t ClCsTrace::SetOutputFileName( m4pcchar_t ai_pccCsFile, m4pcchar_t ai_pccVbFile )
{
	return M4_SUCCESS;
}

inline m4return_t ClCsTrace::GetOutputFileName (m4pchar_t ao_pzFileName)
{
	return M4_SUCCESS;
}

inline m4return_t ClCsTrace::SetTagName (m4pchar_t ai_pzNewTabName)
{
	return M4_SUCCESS;
}

inline m4return_t ClCsTrace::ResetTagName (void)
{
	return M4_SUCCESS;
}

inline ClCsTraceSize* const ClCsTrace::CreateSerializeCsTraceSize()
{
	return 0;
}

inline ClCsTraceSize* const ClCsTrace::CreateDeSerializeCsTraceSize()
{
	return 0;
}

#endif
