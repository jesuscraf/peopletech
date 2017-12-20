#ifndef _LDB_LOG_HPP_
#define _LDB_LOG_HPP_

#include "chlog.hpp"
#include "ldbres.hpp"
#include "ldbdebug.hpp"

M4_DECL_M4OBJLOG extern ChLog	g_oChLog;


m4dumplog_t EndLogAndLdbf (ChLog& aio_chlog, ClLConn* aio_poLConn);								// Manipulador de 1 args. (llamada in-directa)
ChLog&		DumpLogAndLdbf(ChLog& aio_chlog, ClLConn* aio_poLConn);								// Manipulador de 1 args. (llamada in-directa)
ChLog&		DumpLogAndLdbAndPathf(ChLog& aio_chlog, ClLConn* aio_poLConn,m4bool_t ai_bDumpPath);// Manipulador de 2 args. (llamada in-directa)

typedef		BasicLogManipRefEsp<ChLog,ClLConn*,m4dumplog_t>		ChLogManip_ldbinsp_t;
typedef		BasicLogManip1<	  ChLog,ClLConn*>					ChLogManip_ldbinsp_dump_t;

ChLogManip_ldbinsp_t			EndLogAndLdb		(ClLConn* aio_poLConn);
ChLogManip_ldbinsp_dump_t		DumpLogAndLdb		(ClLConn* aio_poLConn);

// Instanciacion de las Clase para manipulador con 2 arg. para el LDB Inspector.
typedef BasicLogManip2<	  ChLog,ClLConn*,m4bool_t>			ChLogManip_ldbinsp_dump_and_path_t;
ChLogManip_ldbinsp_dump_and_path_t	DumpLogAndLdbAndPath(ClLConn* aio_poLConn,m4bool_t ai_bShowPath);


// Macros para volcar al LDBINSPECTOR y M4LOG simultaneamente
#define DUMP_LDBINSP_ERROR(LLL,NNN)		do { DumpBasicLog( g_oChLog << BeginError  (NNN) << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } while(0)
#define DUMP_LDBINSP_WARNING(LLL,NNN)	do { if (LLL->GetpLdb ()->WarningsAreErrors ()) { DUMP_LDBINSP_ERROR (LLL, NNN); } else { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS) { DumpBasicLog( g_oChLog << BeginWarning(NNN) << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } } } while(0)


// Macros para volcar al LDBINSPECTOR y M4LOG simultaneamente, con argumentos
#define DUMP_LDBINSP_ERRORF(LLL,NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginError  (NNN) << XXX << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } while(0)
#define DUMP_LDBINSP_WARNINGF(LLL,NNN,XXX)		do { if (LLL->GetpLdb ()->WarningsAreErrors ()) { DUMP_LDBINSP_ERRORF(LLL,NNN,XXX); } else { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS){DumpBasicLog( g_oChLog << BeginWarning(NNN) << XXX << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } } } while(0)
// Macros para volcar al LDBINSPECTOR y M4LOG simultaneamente, con argumentos.Tambien vuelca el TreePath.

// Estas macros solo tiene sentido que se utilicen para las clases que tengan como miembro m_pMtSent.
// Si no lo tiene dara error de compilacion al expandir la macro END_LOG_AND_LDB_WITH_INFO_AND_PATH_DEBUG.
// Se utiliza dentro de la clase ClPersistBase
#define DUMP_LDBINSP_ERROR_AND_PATHF(LLL,NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginError  (NNN) << XXX << END_LOG_AND_LDB_WITH_INFO_AND_PATH_DEBUG(LLL) ); } while(0)
#define DUMP_LDBINSP_WARNING_AND_PATHF(LLL,NNN,XXX)		do { if (LLL->GetpLdb ()->WarningsAreErrors ()) { DUMP_LDBINSP_ERROR_AND_PATHF(LLL,NNN,XXX); } else { if (STARTCODE( NNN ,WARNINGLOG )==M4_SUCCESS){DumpBasicLog( g_oChLog << BeginWarning(NNN) << XXX << END_LOG_AND_LDB_WITH_INFO_AND_PATH_DEBUG(LLL) ); } } } while(0)


// Volcado del buffer al LDBInspector
void flushToLdb(ClLConn* aio_poLConn);


#ifdef _M4LDB_DEBUG

	#define DUMP_LDBINSP_DEBUG(LLL,NNN)		do { DumpBasicLog( g_oChLog << BeginDebug  (NNN) << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } while(0)
	#define DUMP_LDBINSP_DEBUGF(LLL,NNN,XXX)		do { DumpBasicLog( g_oChLog << BeginDebug  (NNN) << XXX << END_LOG_AND_LDB_WITH_INFO_DEBUG(LLL) ); } while(0)

#else

	#define DUMP_LDBINSP_DEBUGF(LLL,NNN,XXX)
	#define DUMP_LDBINSP_DEBUG(LLL,NNN)
#endif


#endif
