
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4trcfil.hpp   
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
//    Este módulo contiene la definición de traza a fichero
//
//
//==============================================================================


#include "m4oeinit_dll.hpp"
#include "m4cstrace.hpp"

#include "m4clock.hpp"

// RollingFile.
#include "rollingfile.hpp"

#include <stdio.h>
#include <time.h>


#ifndef __M4TRCFIL_HPP__
#define __M4TRCFIL_HPP__




//=============================================================================
// Definiciones
//=============================================================================

// De momento ""
#define	M4_CS_TRACE_MAX_TAG_SIZE			0


typedef struct tm DateTime_t ;



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClVMBaseEnv ;
class	ClCsTraceSize;

//=================================================================================
//
// ClCsFileTrace
//
// Clase que define las trazas CS a fichero
//
//=================================================================================

class	ClCsFileTrace : public ClCsTrace
{

public:

	ClCsFileTrace(const m4int8_t ai_iCsLevel, const m4int8_t ai_iVbLevel) ;
	~ClCsFileTrace( ) ;

	virtual m4return_t	Init( ClVMBaseEnv *ai_poEnvironment ) ;
	virtual m4return_t	End( void ) ;
	
	virtual m4return_t	Reset( void ) ;
	virtual m4return_t	StartupM4ObjectEngine( void ) ;
	virtual m4return_t	ShutdownM4ObjectEngine( void ) ;
	virtual m4return_t	Logon( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer ) ;
	virtual m4return_t	Logoff( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer ) ;
	virtual m4return_t	PreMethodExecutionTrace( IExecutor *ai_poExecutor, ClCMObjectDirectory* ai_poObjectDirectory, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache ) ;
	virtual m4return_t	PostMethodExecutionTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccService ) ;
	virtual m4return_t	PreMetaDataTrace( void ) ;
	virtual m4return_t	PostMetaDataTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache ) ;
//	virtual	m4return_t	PreMapTrace( void );
//	virtual	m4return_t	PostMapTrace( m4double_t ai_fOutCommsTime, m4double_t ai_fInCommsTime, m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache );
	virtual m4return_t	PrePresentationTrace( void ) ;
	virtual m4return_t	PostPresentationTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bFromCache ) ;
	virtual m4return_t	PreSecurityTrace( void ) ;
	virtual m4return_t	PostSecurityTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache ) ;
	virtual m4return_t	PrintVBTrace(m4pcchar_t ai_pccService, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccFunction, m4pcchar_t ai_pccAction, m4pcchar_t ai_pccRole, m4pcchar_t ai_pccInitTime, m4pcchar_t ai_pccEndTime, m4pcchar_t ai_pccElapsed) ;
	
	virtual m4bool_t	IsEnabled( void ) const;

	/**
	 * Activa las trazas.
	 *
	 * @param ai_iCsTraceLevel	Nivel de detalle de las trazas.
	 *							> 1 muestra información detallada sobre los tamaños de serialización.
	 * @param ai_iVbTraceLevel	Nivel de detalle de las trazas de las acciones de usuario.
	 */
	virtual void		TraceLevel( const m4int8_t ai_iCsTraceLevel, const m4uint8_t ai_iVbTraceLevel ) ;

	virtual void		EnableCachedOperations( void ) ;
	virtual void		DisableCachedOperations( void ) ;

	virtual m4return_t	SetOutputFileName (m4pcchar_t ai_pccCsFile, m4pcchar_t ai_pccVbFile) ;
	virtual m4return_t	GetOutputFileName (m4pchar_t ao_pzFileName);
	virtual m4return_t	SetTagName (m4pchar_t ai_pzNewTabName);
	virtual m4return_t	ResetTagName (void);

	/** Devuelve el objeto que se encarga de tomar los tamaños de serializacion. */
	virtual ClCsTraceSize* const CreateSerializeCsTraceSize();

	/** Devuelve el objeto que se encarga de tomar los tamaños de deserializacion. */
	virtual ClCsTraceSize* const CreateDeSerializeCsTraceSize();


protected:

	void			_PutSeparator( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace );
	void			_PutEndOfLine( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace );
	void			_PutVbTraceLabels( m4bool_t ai_bDumpVbTrace ) ;
	void			_PutCsTraceLabels( m4bool_t ai_bDumpCsTrace );
	void			_GetCurrTime( DateTime_t& ao_oDateTime ) ;
	void			_PutTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace ) ;
	void			_PutDateTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace );
	void			_PutTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, DateTime_t& ai_oTime, m4bool_t ai_bPutSeparator = M4_TRUE ) ;
	void			_PutEventCount( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace ) ;
	void			_PutTag( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace ) ;
	void			_PutService( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccService ) ;
	void			_PutRole( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccRole ) ;
	void			_PutElapsedTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4double_t ai_dElapsedTimeInSecs ) ;
	void			_PutFromCache( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4bool_t ai_bFromCache ) ;
	void			_PutDataSize( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4uint32_t ai_iSizeInBytes ) ;
	void			_GetPreTime( DateTime_t& ao_oDateTime ) ;
	void			_StorePreTime( void ) ;
	m4millisec_t	_GetEllapsedMiliseconds( void ) ;
	void			_PutCommonData( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccService, m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize );
	void			_SetRole( m4pcchar_t ai_pccRole ) ;
	void			_SetMethodDescription( m4pcchar_t ai_pccMethodDescription ) ;

	m4return_t		_ReleaseResources( void ) ;

	m4bool_t		_CheckTraceOptions(m4bool_t & aio_bDumpCsTrace, m4bool_t & aio_bDumpVbTrace, m4bool_t & aio_bCacheOptions);
	m4return_t		_OpenCsTraceFile(m4pcchar_t ai_pccCsTraceFile);
	m4return_t		_OpenVbTraceFile(m4pcchar_t ai_pccVbTraceFile);

// Miembros ===================================================================

	m4bool_t				m_bIsInit ;

	ClRollingFile			m_oCsRollingFile ;
	ClRollingFile			m_oVbRollingFile ;
	m4int8_t				m_iCsTraceLevel ;
	m4int8_t				m_iVbTraceLevel ;
	m4bool_t				m_bFromCache ;
	m4bool_t				m_bCacheOperationsEnabled ;
	m4pchar_t				m_pcCsOutputFile ;
	m4pchar_t				m_pcVbOutputFile ;
	m4pchar_t				m_pcMethodDescription ;
	m4pchar_t				m_pcRole ;
	m4char_t				m_acTag[ M4_CS_TRACE_MAX_TAG_SIZE + 1 ] ;
	m4millisec_t			m_oBeginningOfEvent ;
	DateTime_t				m_oBeginningOfEventAsDate ;

	/** Acumula los tamaños de la serialización. */
	ClCsTraceSize*			m_pSerializeCsTraceSize ;

	/** Acumula los tamaños de la deserialización. */
	ClCsTraceSize*			m_pDeSerializeCsTraceSize ;
} ;


inline m4bool_t ClCsFileTrace::IsEnabled( void ) const
{
	m4bool_t bIsEnabled = M4_FALSE;
	if (m_iCsTraceLevel != CS_TRACE_DISABLE)
	{
		bIsEnabled = M4_TRUE;
	}
	else
	{
		if (m_iVbTraceLevel != 0)
		{
			bIsEnabled = M4_TRUE;
		}
	}

	return bIsEnabled;
}

inline void ClCsFileTrace :: TraceLevel( const m4int8_t ai_iCsTraceLevel, const m4uint8_t ai_iVbTraceLevel )
{
	m_iCsTraceLevel = ai_iCsTraceLevel ;
	m_iVbTraceLevel = ai_iVbTraceLevel ;
}

inline void ClCsFileTrace :: EnableCachedOperations( void )
{
	m_bCacheOperationsEnabled = M4_TRUE ;
}

inline void ClCsFileTrace :: DisableCachedOperations( void )
{
	m_bCacheOperationsEnabled = M4_FALSE ;
}

#endif


