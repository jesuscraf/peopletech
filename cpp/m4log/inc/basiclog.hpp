//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              M4Log
// File:                basiclog.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                10-May-1999
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Class implementation that is a wrapper for parsing parameter
//			in the error messages.
//
//
//==============================================================================
#ifndef _BASICLOG_HPP_
#define _BASICLOG_HPP_

/*

	The public macros and manipulators are listed below. Any other thing should
  not be used because implementation can change.

	BASIC MACROS (implementation may change):
	-----------------------------------------
	BL_BEGIN(severity,error)
	BL_END
	BL_OBJ


	MANIPULATORS FOR BL_OBJ:
	------------------------
	BL_Concat	
	BL_ConcatIdAndName
	BL_BeginConcat
	BL_EndConcat
	BL_Date( m4date_t ai_iDate )
	BL_NewError( m4uint32_t ai_uCode )
	BL_NewExtra( m4uint32_t ai_uExtra )


	OVERLOADED << FOR BL_OBJ:
	-------------------------
	m4double_t
	m4uint32_t
	m4int32_t
	m4int_t
	m4pchar_t
	m4char_t
	m4bool_t
	m4float_t
	string


	HELPER MACROS (these macros only use basic macros and/or BL_OBJ manipulators):
	------------------------------------------------------------------------------
	BL_ERROR(NNN)
	BL_WARNING(NNN)

	BL_ERRORF(NNN,XXX)
	BL_WARNINGF(NNN,XXX)

	BL_ID_NAME(XX,YY)
	BL_CONCAT2(XX,YY)	
	BL_CONCATN(XX)

	BL_CHECK_ERROR( condition, result, NNN )
	BL_CHECK_WARNING( condition, result, NNN )

	BL_CHECK_ERRORF( condition, result, NNN, XXX )
	BL_CHECK_WARNINGF( condition, result, NNN, XXX )
*/

#include "m4log.hpp"

#include "m4types.hpp"
#include "m4stl.hpp"

#include "basiclog_manip.hpp"

// Extra information
#define BASICLOG_EXTRA_SOURCE_FILE		103 //	BasicLogLineInfoDebug:	basiclog.cpp


// Declarations
class ClMutex;

// Enum for dump-check when compiling
typedef enum e_m4dumplog {DUMPLOG_OK, DUMPLOG_ERROR}		m4dumplog_t;

// Dump-check: en Debug se chequea el tipo, pero en release no (nos ahorramos la llamada a la funcion en las trazas de log)
#ifdef _DEBUG
	M4_DECL_M4LOG m4return_t DumpBasicLog(m4dumplog_t ai_eDumpState);
#else
	#define DumpBasicLog
#endif // _DEBUG

class M4_DECL_M4LOG M4ClBasicLog;
M4_DECL_M4LOG M4ClBasicLog& DateBasicLogf	(M4ClBasicLog& aio_oLog, m4date_t ai_iDate);	// Manipulador de 1 args. (llamda in-directa)
M4_DECL_M4LOG M4ClBasicLog& NewErrorBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uCode);	// Manipulador de 1 args. (llamda in-directa)
M4_DECL_M4LOG M4ClBasicLog& NewExtraBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uExtra);	// Manipulador de 1 args. (llamda in-directa)


class M4_DECL_M4LOG M4ClBasicLog {
public:

	// Constructor and Destructor
	M4ClBasicLog (m4bool_t ai_bEnableLocks=M4_TRUE);
	virtual ~M4ClBasicLog();

	// Get & Set functions
	void SetTypeError (m4int16_t ai_iTypeError) { m_iType = ai_iTypeError; };
	void SetNumError  (m4int32_t ai_iNumError) { m_iNumError = ai_iNumError; };	
	m4int16_t GetTypeError() const { return m_iType; };
	m4int32_t GetNumError() const { return m_iNumError; };
	m4bool_t	IsDumpActive() const { return m_bDumpActive; };
	m4bool_t	SetDumpState() {					//Establece el estado del volcado, preguntando a la M4Log
		return ( m_bDumpActive = ( (STARTCODE(m_iNumError, m_iType)==M4_SUCCESS) ? M4_TRUE : M4_FALSE ) );
	};

	// Function Lock-Unlock for thread-safe
	void		Lock();
	void		Unlock();
	m4bool_t	IsLocked() const { return m_bLocked; };

	void flush () const {						// Dump buffer to M4LOG
		if (IsDumpActive())
			SETCODE( m_iNumError, m_iType, m_pcBuffer);
	};
	
	// Sobrecarga para tipos básicos - volcado
	friend M4_DECL_M4LOG M4ClBasicLog  &operator << (M4ClBasicLog& aio_oLog,const m4double_t ai_d);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4uint32_t ai_ui);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4int32_t ai_i);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4int_t ai_i);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog, const m4int64_t ai_i);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4pcchar_t ai_s);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4char_t ai_c);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4bool_t ai_b);
	friend M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4float_t ai_f);
	friend inline M4_DECL_M4LOG M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const string &ai_s) { return aio_oLog << ai_s.c_str(); };

	// Friends - manipulator-functions (indirect)
	friend M4_DECL_M4LOG M4ClBasicLog& DateBasicLogf	(M4ClBasicLog& aio_oLog, m4date_t ai_iDate);	// Manipulador de 1 args. (llamda in-directa)
	friend M4_DECL_M4LOG M4ClBasicLog& NewErrorBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uCode);	// Manipulador de 1 args. (llamda in-directa)
	friend M4_DECL_M4LOG M4ClBasicLog& NewExtraBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uExtra);	// Manipulador de 1 args. (llamda in-directa)


	// Friends - manipulator-functions (direct)

	friend inline M4_DECL_M4LOG M4ClBasicLog& BL_Concat(M4ClBasicLog& aio_oLog) {	// Manipulador de 0 args. (llamada directa)
		aio_oLog.m_bConcat = M4_TRUE;
		return aio_oLog;
	};

	friend inline M4_DECL_M4LOG M4ClBasicLog& BL_ConcatIdAndName(M4ClBasicLog& aio_oLog) { // Manipulador de 0 args. (llamada directa)
		aio_oLog.m_bConcat = M4_TRUE;
		aio_oLog.m_bConcatAnd = M4_TRUE;
		return aio_oLog;
	};

	friend inline M4_DECL_M4LOG M4ClBasicLog& BL_BeginConcat(M4ClBasicLog& aio_oLog) { // Manipulador de 0 args. (llamada directa)
		aio_oLog.m_bConcatOn = M4_TRUE;
		aio_oLog.m_bIsFirst = M4_TRUE;
		return aio_oLog;
	};

	friend inline M4_DECL_M4LOG M4ClBasicLog& BL_EndConcat(M4ClBasicLog& aio_oLog) { // Manipulador de 0 args. (llamada directa)
		aio_oLog.m_bConcatOn = M4_FALSE;
		return aio_oLog;
	};

protected:

	m4uint32_t	m_iSize;			// Buffer Size
	size_t		m_iPos;			// Current position in buffer
	m4bool_t	m_bLocked;		// Locked or Unlocked buffer
	m4pchar_t	m_pcBuffer;		// Output buffer
	m4char_t	m_pcCadFormato[64];	// Buffer to build format string

	m4bool_t	m_bConcat;		// Concat only the Next argument
	m4bool_t	m_bConcatOn;		// Concat from BeginLogCat to EndLogCat
	m4bool_t	m_bIsFirst;		// Is the first argument or no
	m4bool_t	m_bDumpActive;	// Log dump state (activated or not activated, for the current error)
	m4bool_t	m_bConcatAnd;		// Concat using &&

	// Info for M4Log
	m4int16_t	m_iType;		// Log type: Error, Warning, Debug, Trace
	m4int32_t	m_iNumError;	// Error number

	// Thread-safe
	ClMutex*	m_poMutex;	// Lock mutex

	// Prepare Output to buffer
	void PrepareOutput(m4pcchar_t ai_pcSubCadFormato, m4pchar_t &ao_pcBuffer, m4uint32_t iSize);
	void ReallocToConcat(m4uint32_t ai_iSize);	// Buffer reallocation of the error message.

	// Initialize and Reset functions
	void Init();
	void Reset();
};



// Sobrecarga para llamada a funciones-manipuladores de 0 args.
inline M4_DECL_M4LOG M4ClBasicLog& operator<< (M4ClBasicLog& aio_oLog,M4ClBasicLog& (*_f)( M4ClBasicLog& )) 
{ return (*_f) (aio_oLog); };

// Sobrecarga para llamada a funcion-manipulador de 0 args. EndLog (especial)
inline M4_DECL_M4LOG m4dumplog_t operator<< (M4ClBasicLog& aio_oLog,m4dumplog_t (*_f)( M4ClBasicLog& )) 
{ return (*_f) (aio_oLog); };



// Declaracion de manipuladores globales (funcionales)

M4_DECL_M4LOG M4ClBasicLog& BeginBasicLog(M4ClBasicLog& aio_oLog, m4int32_t ai_iNumError, m4int16_t ai_iTipoError);// Manipulador de 2 args. (llamada in-directa)
M4_DECL_M4LOG m4dumplog_t EndBasicLog(M4ClBasicLog& aio_oLog);													// Manipulador de 0 args. (llamada directa)


// Usando Templates
// Instanciacion de las Clase para manipulador con 2 args.
typedef BasicLogManip2<M4ClBasicLog,m4int32_t,m4int16_t>		BasicLogManip2_int_t;
// Funciones-Manipulador con 1 arg (directas) que llaman a la funcion manipuladora real con 2 args
inline M4_DECL_M4LOG BasicLogManip2_int_t BeginBasicError		( m4int32_t ai_iNumError ) { return  BasicLogManip2_int_t( BeginBasicLog, ai_iNumError, ERRORLOG   ); };
inline M4_DECL_M4LOG BasicLogManip2_int_t BeginBasicWarning	( m4int32_t ai_iNumError ) { return  BasicLogManip2_int_t( BeginBasicLog, ai_iNumError, WARNINGLOG ); };
inline M4_DECL_M4LOG BasicLogManip2_int_t BeginBasicDebug		( m4int32_t ai_iNumError ) { return  BasicLogManip2_int_t( BeginBasicLog, ai_iNumError, DEBUGINFOLOG ); };
inline M4_DECL_M4LOG BasicLogManip2_int_t BeginBasicTrace		( m4int32_t ai_iNumError ) { return  BasicLogManip2_int_t( BeginBasicLog, ai_iNumError, TRACEINFOLOG ); };

// Instanciacion de las Clase para manipulador con 1 arg. = m4date_t
typedef BasicLogManip1<M4ClBasicLog,m4date_t> BasicLogManip1_date_t;
inline M4_DECL_M4LOG BasicLogManip1_date_t BL_Date( m4date_t ai_iDate ) { return BasicLogManip1_date_t( DateBasicLogf, ai_iDate ); };

// Instanciacion de las Clase para manipulador con 1 arg. = m4uin32_t
typedef BasicLogManip1<M4ClBasicLog,m4uint32_t> BasicLogManip1_uint32_t;
inline M4_DECL_M4LOG BasicLogManip1_uint32_t BL_NewError( m4uint32_t ai_uCode ) { return BasicLogManip1_uint32_t( NewErrorBasicLogf, ai_uCode ); };
inline M4_DECL_M4LOG BasicLogManip1_uint32_t BL_NewExtra( m4uint32_t ai_uExtra ) { return BasicLogManip1_uint32_t( NewExtraBasicLogf, ai_uExtra ); };


// Funcion que vuelca Info de "*** File: namefile(line) ***" en DEBUG
// Volcamos INFO como Error EXTENDIDO: E103
M4_DECL_M4LOG string BasicLogLineInfoDebug(const char *ai_pszFileName, int ai_iLine);

#define BL_END_LOG	BL_NewExtra(BASICLOG_EXTRA_SOURCE_FILE) << BasicLogLineInfoDebug(__FILE__, __LINE__) << EndBasicLog // *** File: d:\v150\chlib\version\src\vmachine.cpp(40) ***

//////////////////////////////////////////////////////////////////
//
// Macros para poder usar todo el mecanismo de forma comoda
//
//////////////////////////////////////////////////////////////////

// Basic Macros


#define BL_BEGIN(severity,error)		{ M4ClBasicLog _oBasicLog(M4_FALSE); BeginBasicLog(_oBasicLog, error, severity)
#define BL_END								_oBasicLog << BL_END_LOG; }
#define BL_OBJ								_oBasicLog


// Macros which depend on basic macros
// -----------------------------------

// Macros para volcar a la M4LOG sin argumentos

#define BL_ERROR(NNN)	do { BL_BEGIN(ERRORLOG, NNN); BL_END; } while(0)
#define BL_WARNING(NNN)	do { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS) { BL_BEGIN(WARNINGLOG, NNN); BL_END; } } while(0)

// Macros para volcar a la M4LOG con argumentos

#define BL_ERRORF(NNN,XXX)	do { BL_BEGIN(ERRORLOG, NNN); BL_OBJ << XXX; BL_END;  } while(0)
#define BL_WARNINGF(NNN,XXX)	do { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS) { BL_BEGIN(WARNINGLOG, NNN); BL_OBJ << XXX; BL_END;  } } while(0)

// Macros para usar ciertos manipuladores como funciones
#define BL_ID_NAME(XX,YY)	XX << BL_ConcatIdAndName << YY
#define BL_CONCAT2(XX,YY)	XX << BL_Concat << YY
#define BL_CONCATN(XX)		BL_BeginConcat << XX << BL_EndConcat


// Funciones más avanzadas

// Macros para volcar a la M4LOG con chequeo
#define BL_CHECK_ERROR( condition, result, NNN )	do { if( condition ) { BL_ERROR  ( NNN ) ; return( result ) ; } } while( 0 )
#define BL_CHECK_WARNING( condition, result, NNN )	do { if( condition ) { BL_WARNING( NNN ) ; return( result ) ; } } while( 0 )

// Macros para volcar a la M4LOG con argumentos y con chequeo
#define BL_CHECK_ERRORF( condition, result, NNN, XXX )	do { if( condition ) { BL_ERRORF  ( NNN, XXX ) ; return( result ) ; } } while( 0 )
#define BL_CHECK_WARNINGF( condition, result, NNN, XXX )	do { if( condition ) { BL_WARNINGF( NNN, XXX ) ; return( result ) ; } } while( 0 )


#endif // _BASICLOG_HPP_

