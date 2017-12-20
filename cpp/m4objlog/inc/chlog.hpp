//## begin module%3912858801C6.cm preserve=no
//## end module%3912858801C6.cm

//## begin module%3912858801C6.cp preserve=no
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
//## end module%3912858801C6.cp

//## Module: chlog%3912858801C6; Package specification
//## Subsystem: M4ObjLog::inc%3912860B01D9
//## Source file: d:\new\m4objlog\inc\chlog.hpp

#ifndef chlog_h
#define chlog_h 1

//## begin module%3912858801C6.additionalIncludes preserve=no
//## end module%3912858801C6.additionalIncludes

//## begin module%3912858801C6.includes preserve=yes


// Las macros para poder usar todo el mecanismo de forma comoda están al final
// Sólo se deberían usar dichas macros.

#include "m4objlog_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module%3912858801C6.includes

// basiclog
#include <basiclog.hpp>
// basiclog_manip
#include <basiclog_manip.hpp>
//## begin module%3912858801C6.declarations preserve=no
//## end module%3912858801C6.declarations

//## begin module%3912858801C6.additionalDeclarations preserve=yes



// Declaration
class m4VariantTypeBase;
class ClCompiledMCR;
class ClLConn;


// Macros para referirnos a Errores Extendidos	(Migrarlo al clres.hpp o similar)
#define M4LOGSYS_EXT_ERROR_TREE_PATH	"||#*E101*#"	//DumpTreePath:			cldbgutil.cpp
#define M4LOGSYS_EXT_ERROR_CALL_STACK	"||#*E102*#"	//ClVVM::DumpCallStack: vvm.cpp
//#define BASICLOG_EXTRA_SOURCE_FILE		103 //	BasicLogLineInfoDebug:	basiclog.cpp
#define M4LOGSYS_EXT_ERROR_CODE_LINE	"||#*E104*#"	//ClTrad::DumpError: trad.cpp


#if defined(_AIX) && defined(_XLC_R)
    class ChLog;
    ChLog& DateLogf(ChLog& aio_chlog, m4date_t ai_iDate);
    ChLog& NewErrorf(ChLog& aio_chlog, m4uint32_t ai_uCode);
    ChLog& NewExtraf(ChLog& aio_chlog, m4uint32_t ai_uExtra);
    ChLog& LogCat(ChLog& aio_chlog);
    ChLog& LogCatAnd(ChLog& aio_chlog);
    ChLog& BeginLogCat(ChLog& aio_chlog);
    ChLog& EndLogCat(ChLog& aio_chlog);
#endif
//## end module%3912858801C6.additionalDeclarations


//## begin ChLog%3912858801E5.preface preserve=yes
//## end ChLog%3912858801E5.preface

//## Class: ChLog%3912858801E5
//	Macros para referirnos a Errores Extendidos	(Migrarlo al clres.hpp o similar)
//## Category: M4ObjLog%3912858801E4
//## Subsystem: M4ObjLog::inc%3912860B01D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJLOG ChLog : public M4ClBasicLog  //## Inherits: <unnamed>%3912858802E0
{
  //## begin ChLog%3912858801E5.initialDeclarations preserve=yes
public:
	// Total memory used by object

	// Sobrecarga para tipos básicos - volcado

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4double_t ai_d)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_d; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4uint32_t ai_ui)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_ui; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4int32_t ai_i)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_i; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4int_t ai_i)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_i; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4pcchar_t ai_s)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_s; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4char_t ai_c)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_c; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4bool_t ai_b)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_b; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4float_t ai_f)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_f; return aio_oLog; };

	friend inline M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const string &ai_s)
	{ *(M4ClBasicLog*)&aio_oLog<<ai_s; return aio_oLog; };

	// Otros tipos

	friend M4_DECL_M4OBJLOG ChLog &operator << (ChLog& aio_oLog,const m4VariantTypeBase &ai_v);


	// Friends - manipulator-functions (indirect)

	friend inline M4_DECL_M4OBJLOG ChLog& DateLogf	(ChLog& aio_chlog, m4date_t ai_iDate) // Manipulador de 1 args. (llamda in-directa)
	{ DateBasicLogf(aio_chlog, ai_iDate); return aio_chlog; };

	friend inline M4_DECL_M4OBJLOG ChLog& NewErrorf	(ChLog& aio_chlog, m4uint32_t ai_uCode)	// Manipulador de 1 args. (llamda in-directa)
	{ NewErrorBasicLogf(aio_chlog, ai_uCode); return aio_chlog; };

	friend inline M4_DECL_M4OBJLOG ChLog& NewExtraf	(ChLog& aio_chlog, m4uint32_t ai_uExtra)	// Manipulador de 1 args. (llamda in-directa)
	{ NewExtraBasicLogf(aio_chlog, ai_uExtra); return aio_chlog; };


	// Friends - manipulator-functions (direct)

	friend inline M4_DECL_M4OBJLOG ChLog& LogCat(ChLog& aio_chlog) // Manipulador de 0 args. (llamada directa)
	{ BL_Concat(*(M4ClBasicLog*)&aio_chlog); return aio_chlog; };

	friend inline M4_DECL_M4OBJLOG ChLog& LogCatAnd(ChLog& aio_chlog) // Manipulador de 0 args. (llamada directa)
	{ BL_ConcatIdAndName(*(M4ClBasicLog*)&aio_chlog); return aio_chlog; };

	friend inline M4_DECL_M4OBJLOG ChLog& BeginLogCat(ChLog& aio_chlog) // Manipulador de 0 args. (llamada directa)
	{ BL_BeginConcat(*(M4ClBasicLog*)&aio_chlog); return aio_chlog; };

	friend inline M4_DECL_M4OBJLOG ChLog& EndLogCat(ChLog& aio_chlog) // Manipulador de 0 args. (llamada directa)
	{ BL_EndConcat(*(M4ClBasicLog*)&aio_chlog); return aio_chlog; };

	friend void flushToLdb (ClLConn* aio_poLConn);	// Dump buffer to LDB

	// Friends - manipulator-functions (indirect) - Dump CMCR Info
	friend ChLog&	M4Obj_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNotUsed);
	friend ChLog&	M4ObjExtended_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNotUsed);
	friend ChLog&	M4ObjNodeN_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNode);
	friend ChLog&	M4ObjNodeI_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem);
	friend ChLog&	M4ObjNodeItemI_f	(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem);
	friend ChLog&	NodeN_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNode );
	friend ChLog&	NodeI_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem );
	friend ChLog&	NodeItemI_f			(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem );
	friend ChLog&	ItemI_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem );

  //## end ChLog%3912858801E5.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetSize%-880623417; C++
      //	-- {AddDecl: 10} module.vulnerableDeclarations [1304..5168]
      m4return_t GetSize (m4uint32_t& ao_iSize);

    // Additional Public Declarations
      //## begin ChLog%3912858801E5.public preserve=yes
      //## end ChLog%3912858801E5.public

  protected:
    // Additional Protected Declarations
      //## begin ChLog%3912858801E5.protected preserve=yes
      //## end ChLog%3912858801E5.protected

  private:
    // Additional Private Declarations
      //## begin ChLog%3912858801E5.private preserve=yes
      //## end ChLog%3912858801E5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ChLog%3912858801E5.implementation preserve=yes
      //## end ChLog%3912858801E5.implementation

};

//## begin ChLog%3912858801E5.postscript preserve=yes
//## end ChLog%3912858801E5.postscript

// Class ChLog 

//## begin module%3912858801C6.epilog preserve=yes


// Sobrecarga para llamada a funciones-manipuladores de 0 args.
inline M4_DECL_M4OBJLOG ChLog& operator<< (ChLog& aio_oLog,ChLog& (*_f)( ChLog& ))
{ return (*_f) (aio_oLog); };

// Sobrecarga para llamada a funcion-manipulador de 0 args. EndLog (especial)
inline M4_DECL_M4OBJLOG m4dumplog_t operator<< (ChLog& aio_oLog,m4dumplog_t (*_f)( ChLog& ))
{ return (*_f) (aio_oLog); };

// Declaracion de manipuladores globales (funcionales)

inline M4_DECL_M4OBJLOG ChLog& BeginLog(ChLog& aio_chlog, m4int32_t ai_iNumError, m4int16_t ai_iTipoError) // Manipulador de 2 args. (llamada in-directa)
{ BeginBasicLog(aio_chlog,ai_iNumError,ai_iTipoError); return aio_chlog; };

inline M4_DECL_M4OBJLOG m4dumplog_t EndLog(ChLog& aio_chlog) // Manipulador de 0 args. (llamada directa)
{ return EndBasicLog(aio_chlog); };


// Usando Templates
// Instanciacion de las Clase para manipulador con 2 args.
typedef BasicLogManip2<ChLog,m4int32_t,m4int16_t>	ChLogManip2_int_t;
// Funciones-Manipulador con 1 arg (directas) que llaman a la funcion manipuladora real con 2 args
inline M4_DECL_M4OBJLOG ChLogManip2_int_t BeginError		( m4int32_t ai_iNumError ) { return  ChLogManip2_int_t( BeginLog, ai_iNumError, ERRORLOG   ); };
inline M4_DECL_M4OBJLOG ChLogManip2_int_t BeginWarning	( m4int32_t ai_iNumError ) { return  ChLogManip2_int_t( BeginLog, ai_iNumError, WARNINGLOG ); };
inline M4_DECL_M4OBJLOG ChLogManip2_int_t BeginDebug		( m4int32_t ai_iNumError ) { return  ChLogManip2_int_t( BeginLog, ai_iNumError, DEBUGINFOLOG ); };

// Instanciacion de las Clase para manipulador con 1 arg. = m4date_t
typedef BasicLogManip1<ChLog,m4date_t> ChLogManip1_date_t;
inline M4_DECL_M4OBJLOG ChLogManip1_date_t DateLog( m4date_t ai_iDate ) { return ChLogManip1_date_t( DateLogf, ai_iDate ); };

// Instanciacion de las Clase para manipulador con 1 arg. = m4uin32_t
typedef BasicLogManip1<ChLog,m4uint32_t> ChLogManip1_uint32_t;
inline M4_DECL_M4OBJLOG ChLogManip1_uint32_t CL_NewError( m4uint32_t ai_uCode ) { return ChLogManip1_uint32_t( NewErrorf, ai_uCode ); };
inline M4_DECL_M4OBJLOG ChLogManip1_uint32_t CL_NewExtra( m4uint32_t ai_uExtra ) { return ChLogManip1_uint32_t( NewExtraf, ai_uExtra ); };


// Global object for Log
extern ChLog M4_DECL_M4OBJLOG g_oChLog;

//////////////////////////////////////////////////////////////////
//
// Macros para poder usar todo el mecanismo de forma comoda
//
//////////////////////////////////////////////////////////////////

// Info de "*** File: namefile(line) ***" en DEBUG


#ifdef _DEBUG
	#define LOG_WITH_INFO_DEBUG	CL_NewExtra(BASICLOG_EXTRA_SOURCE_FILE) << BasicLogLineInfoDebug(__FILE__, __LINE__) << // *** File: d:\v150\chlib\version\src\vmachine.cpp(40) ***
#else
	#define LOG_WITH_INFO_DEBUG
#endif

#define END_LOG_WITH_INFO_DEBUG					LOG_WITH_INFO_DEBUG EndLog
#ifndef M4_VM_DIC_CLIENT
#define END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL)	DumpLogAndLdb(LLL) << LOG_WITH_INFO_DEBUG EndLogAndLdb(LLL)
// Esta macro solo tiene sentido que se utilice para las clases que tengan como miembro m_pMtSent.
// En la BDL se utiliza desde ClPeristBase.
#define END_LOG_AND_LDB_WITH_INFO_AND_PATH_DEBUG(LLL)	DumpLogAndLdbAndPath(LLL,m_pMtSent?M4_TRUE:M4_FALSE) << LOG_WITH_INFO_DEBUG EndLogAndLdb(LLL)
#else
#define END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL)	LOG_WITH_INFO_DEBUG EndLogAndLdb(LLL)
#endif

// Macros para volcar a la M4LOG
#define DUMP_CHLOG_ERROR(NNN)	do { DumpBasicLog( g_oChLog << BeginError  (NNN) << END_LOG_WITH_INFO_DEBUG ); } while(0)
#define DUMP_CHLOG_WARNING(NNN)	do { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS) { DumpBasicLog(  g_oChLog << BeginWarning(NNN) << END_LOG_WITH_INFO_DEBUG ); } } while(0)
#ifdef _DEBUG
#define DUMP_CHLOG_DEBUG(NNN)	do { DumpBasicLog( g_oChLog << BeginDebug  (NNN) << END_LOG_WITH_INFO_DEBUG ); } while(0)
//#define DUMP_CHLOG_DEBUG(NNN)	do { DumpBasicLog( g_oChLog << BeginError  (NNN) << END_LOG_WITH_INFO_DEBUG ); } while(0)
#else
#define DUMP_CHLOG_DEBUG(NNN)
#endif

// Macros para volcar a la M4LOG con argumentos
#define DUMP_CHLOG_ERRORF(NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginError  (NNN) << XXX << END_LOG_WITH_INFO_DEBUG ); } while(0)
#define DUMP_CHLOG_WARNINGF(NNN,XXX)	do { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS) { DumpBasicLog( g_oChLog << BeginWarning(NNN) << XXX << END_LOG_WITH_INFO_DEBUG ); } } while(0)
#ifdef _DEBUG
#define DUMP_CHLOG_DEBUGF(NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginDebug  (NNN) << XXX << END_LOG_WITH_INFO_DEBUG ); } while(0)
//#define DUMP_CHLOG_DEBUGF(NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginError  (NNN) << XXX << END_LOG_WITH_INFO_DEBUG ); } while(0)
#else
#define DUMP_CHLOG_DEBUGF(NNN,XXX)
#endif

// jcr
// Macros para volcar a la M4LOG con chequeo
#define CHECK_CHLOG_ERROR( condition, result, NNN )		do { if( condition ) { DUMP_CHLOG_ERROR  ( NNN ) ; return( result ) ; } } while( 0 )
#define CHECK_CHLOG_WARNING( condition, result, NNN )	do { if( condition ) { DUMP_CHLOG_WARNING( NNN ) ; return( result ) ; } } while( 0 )
#ifdef _DEBUG
#define CHECK_CHLOG_DEBUG( condition, result, NNN )		do { if( condition ) { DUMP_CHLOG_DEBUG  ( NNN ) ; return( result ) ; } } while( 0 )
#else
#define CHECK_CHLOG_DEBUG( condition, result, NNN )
#endif

// jcr
// Macros para volcar a la M4LOG con argumento y con chequeo
#define CHECK_CHLOG_ERRORF( condition, result, NNN, XXX )	do { if( condition ) { DUMP_CHLOG_ERRORF  ( NNN, XXX ) ; return( result ) ; } } while( 0 )
#define CHECK_CHLOG_WARNINGF( condition, result, NNN, XXX )	do { if( condition ) { DUMP_CHLOG_WARNINGF( NNN, XXX ) ; return( result ) ; } } while( 0 )
#ifdef _DEBUG
#define CHECK_CHLOG_DEBUGF( condition, result, NNN, XXX )	do { if( condition ) { DUMP_CHLOG_DEBUGF  ( NNN, XXX ) ; return( result ) ; } } while( 0 )
#else
#define CHECK_CHLOG_DEBUGF( condition, result, NNN, XXX )
#endif


// Macros para usar ciertos manipuladores como funciones
#define LOG_CAT(XX,YY)		XX << LogCat << YY
#define LOG_CAT_AND(XX,YY)	XX << LogCatAnd << YY
#define LOG_CAT_BLOCK(XX)	BeginLogCat << XX << EndLogCat


/*
	Ejemplos de uso:

	g_oChLog << BeginError(iNumError) << n1 << d1 << n2 << DateLog(d2) << v1 << EndLog;					// Esto si compila y es correcto
	DumpBasicLog( g_oChLog << BeginError(iNumError) << n1 << d1 << n2 << DateLog(d2) << v1 << EndLog );	// Esto si compila y es correcto
	DumpBasicLog( g_oChLog << BeginError(iNumError) << n1 << d1 << n2 << DateLog(d2) << v1  );				// Esto no compila en Debug (porque es erroneo)
	DUMP_CHLOG_ERROR(iNumError, n1 << d1 << n2 << DateLog(d2) << v1 );									// Esto si compila y es correcto
*/

// Comprobar si un Cod. Error ha sido volcado a la M4Log
m4uint8_t	M4_DECL_M4OBJLOG	M4ChLogIsErrorIn( m4int32_t ai_iError ) ;


//## end module%3912858801C6.epilog


#endif
