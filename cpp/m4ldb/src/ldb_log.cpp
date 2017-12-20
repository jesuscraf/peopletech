#include "ldb_log.hpp"
#include "cllconn.hpp"
#include "m4mdrt.hpp"
#include "cldbgutil.hpp"

// Volcado del buffer al LDBInspector
void flushToLdb(ClLConn* aio_poLConn)
{
#ifdef _M4LDB_DEBUG
	if (g_oChLog.IsDumpActive()) {
		if (aio_poLConn) {
			// Solo en version SERVER y CLIENT, pero no en DIC
			switch (g_oChLog.m_iType) {
			case ERRORLOG:
				aio_poLConn->m_pInspector->PrintFileError (aio_poLConn, g_oChLog.m_iNumError, g_oChLog.m_pcBuffer);
				break;
			case WARNINGLOG:
				aio_poLConn->m_pInspector->PrintFileWarning (aio_poLConn, g_oChLog.m_iNumError, g_oChLog.m_pcBuffer);
				break;
			case DEBUGINFOLOG:
				aio_poLConn->m_pInspector->PrintFileDebug (aio_poLConn, g_oChLog.m_iNumError, g_oChLog.m_pcBuffer);
				break;
			}
		}
	}
#endif
}

ChLog& DumpLogAndLdbf(ChLog& aio_chlog, ClLConn* aio_poLConn)
{
	if (aio_chlog.IsDumpActive()) {
		if ((aio_poLConn) && 
			(aio_poLConn->m_pLogMCR)) {
			//Id y Nombre del Canal
			m4id_t iM4Obj = aio_poLConn->m_pLogMCR->iM4Obj ();
			m4id_t nM4Obj = aio_poLConn->m_pLogMCR->nM4Obj ();
			if (!*nM4Obj)
				nM4Obj = iM4Obj;
			//Id y Nombre del Nodo
			m4id_t iNode = aio_poLConn->m_pLogMCR->iNodeN (aio_poLConn->m_hLogNode);
			m4id_t nNode = aio_poLConn->m_pLogMCR->nNodeN (aio_poLConn->m_hLogNode);
			if (!*nNode)
				nNode = iNode;
			//Volcado de Info
			aio_chlog << LOG_CAT_AND (iNode,nNode) << LOG_CAT_AND (iM4Obj,nM4Obj);
		}
	}
	return aio_chlog;
}
// Volcado de canal, nodo y el Bookmark.
ChLog& DumpLogAndLdbAndPathf(ChLog& aio_chlog, ClLConn* aio_poLConn,m4bool_t ai_bDumpPath)
{
	DumpLogAndLdbf(aio_chlog,aio_poLConn);
	if (ai_bDumpPath){
		aio_chlog << DumpTreePathRegisterItem(aio_poLConn->m_pLogAccess,aio_poLConn->m_hLogNode);
	}
	return aio_chlog;
}

m4dumplog_t EndLogAndLdbf(ChLog& aio_chlog, ClLConn* aio_poLConn)
{
#ifdef _M4LDB_DEBUG
	if (aio_poLConn) {
		flushToLdb(aio_poLConn);
	}
#endif
	return EndLog(aio_chlog);
}
ChLogManip_ldbinsp_t		EndLogAndLdb		(ClLConn* aio_poLConn){
	return ChLogManip_ldbinsp_t(		EndLogAndLdbf , aio_poLConn );
}
ChLogManip_ldbinsp_dump_t	DumpLogAndLdb		(ClLConn* aio_poLConn){
	return ChLogManip_ldbinsp_dump_t( DumpLogAndLdbf, aio_poLConn );
}
// Funciones-Manipulador con 2 arg (directa) que llaman a la funcion manipuladora real con 2 args
ChLogManip_ldbinsp_dump_and_path_t	DumpLogAndLdbAndPath(ClLConn* aio_poLConn,m4bool_t ai_bShowPath){
	return ChLogManip_ldbinsp_dump_and_path_t( DumpLogAndLdbAndPathf, aio_poLConn, ai_bShowPath);
}
