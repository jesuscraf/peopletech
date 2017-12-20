//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                cllconn.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#include "cllconn.hpp"

#include "dm_types.hpp"
#include "clldb.hpp"
#include "clload.hpp"
#include "cldinarr.hpp"
#include "clpersis.hpp"
#include "ldbglobals.hpp"
#include "syncro.hpp"
#include "blocksynchronization.hpp"
#include "apisql.hpp"
#include "clsentcc.hpp"
#include "ldb_log.hpp"
#include "ldbcontext.hpp"
#include "cllgadap.hpp"
#include "m4lgadapdef.hpp"
#include "px_block.hpp"
#include "nodedef.hpp"
#include "compst_common.hpp"
#include "m4mdfac.hpp"
#include "compstit_common.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "bookmark.hpp"
#include "connmanager.hpp"
#include "m4event.hpp"
#include "m4thread.hpp"
#include "m4clock.hpp"
#include "licensemanagerinterface.hpp"


//Declaramos las variables empleadas para calculo de tiempos.
//El constructor tambien inicializa el tiempo. Por lo tanto podemos inicializar,..., y paramos el tiempo.
//Mostraremos la información y por último podemos reanudar el crono.
//Bug 0073832.
#define	M4LDB_DUMPTIMES_DECLARE			m4millisec_t _startingTime, _endingTime;
#define	M4LDB_DUMPTIMES_START			_startingTime = ClPrecisionTicker::GetTimeTick();
#define	M4LDB_DUMPTIMES_STOP			_endingTime = ClPrecisionTicker::GetTimeTick();

#ifdef _WINDOWS
	//Si estamos en windows.
	#define	M4LDB_DUMPTIMES_PRINT_TIMES		" Time = %d (ms) Thread ID = %d Tick = %I64d.", long( _endingTime - _startingTime ), M4Thread::GetCurrentIdThread(), ClPrecisionTicker::GetTimeTick()
#else
	//En plataformas No Windows (lo trataremos como caso por defecto):
	#define	M4LDB_DUMPTIMES_PRINT_TIMES		" Time = %d (ms) Thread ID = %d Tick = %lld.", long( _endingTime - _startingTime ), M4Thread::GetCurrentIdThread(), ClPrecisionTicker::GetTimeTick()
#endif
//Fin Tiempos en ldbinsp
//---------------------------------------------

class ClStringCleaner{
public:
	ClStringCleaner(string *ai_pst){
		m_pst = ai_pst;
	}
	~ClStringCleaner(void){
		if (m_pst){
			// Aprovecho aqui para comprobar el capacity
			// Tiene un problema y es el caso en que se produzca reentrada para una sentencia que tenga mas de 4K. ( Es bastante poco probable).
			if (m_pst->capacity()>4096){
				m_pst->erase();
				m_pst->reserve(4080);	// Reserva para 16*(4080/16 + 1) = 4096.
								// Si le paso 4096 me reserva para 4112
			}

			(*m_pst)="";
		}
	}
private:
	string *m_pst ;

};

//	#include "m4win2ux.hpp"

// Prefijos de las cadenas identificativas

// S -> SysLoad.
// L -> Load.
// E -> ExtendedLoad.
// D -> Delete.
// U -> Update.
// I -> Insert.
// F -> Filter.
// C -> Call.
// R -> Real.

#include "clldb.hpp"

#ifdef _M4LDB_DEBUG
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DATE
	#define M4LDB_PRINT_DEBUG_DATE	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebugDate

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebugDetail
	
	#undef M4LDB_PRINT_DEBUG_DETAIL_DATE
	#define M4LDB_PRINT_DEBUG_DETAIL_DATE	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebugDetailDate

	#undef M4LDB_BEGIN_DEBUG
	#define M4LDB_BEGIN_DEBUG	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) {

	#undef M4LDB_END_DEBUG
	#define M4LDB_END_DEBUG		}
#endif

#undef M4LDB_INFORMER
#define M4LDB_INFORMER(X)		m_pInformer = (m4pcchar_t) X;

m4cint8_t *g_aDDBBs[] = { "SQL Server","Informix","Oracle","DB2","Sybase" };

m4cint8_t g_aM4ADM[] = "M4ADM";

#define MAX_NUMBER_OF_PARAMETERS	32


// Para la reducción de índices de idiomas
m4uint8_t   g_aiReducedLanguagesArray[ M4CL_LANGUAGE_NUMBER ] =
{
	0, 0, 0, 1, 2, 3, 4, 5, 6
} ;




#ifdef _M4LDB_DEBUG
ClLConn::ClLConn (ClLdb *ai_pLdb, m4int32_t ai_idLConn, ClConnectionManager *ai_pConnManager, ClLDBInspector *ai_pInspector)
:	m_pConnManager(ai_pConnManager)
{
	m_pInspector = ai_pInspector;

#else
ClLConn::ClLConn (ClLdb *ai_pLdb, m4int32_t ai_idLConn, ClConnectionManager *ai_pConnManager)
:	m_pConnManager(ai_pConnManager)
{
#endif
	m_pInformer = NULL;
	m_pLdb = ai_pLdb;
	m_idLConn = ai_idLConn;
	m_paRS = NULL;
	m_pAcsDict = NULL;
   	m_plsPostValidation = NULL;
	m_pmpTransCache = NULL;
	m_pvcTransAccRl = NULL;
	m_numTrans = 0;
	ClActualDate (m_dDTLstUpdTrn);
	m_pLogonAdaptor = NULL;
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_0;
	m_poStatistics = NULL;
	m_idExecution = 0;
	m_indLastSentUsed = 0;
	m_pLogMCR = NULL;
	m_pLogAccess = NULL;
	m_hLogNode = 0;
	m_stBuffer.reserve(4080);	// Reserva para 16*(4080/16 + 1) = 4096.
								// Si le paso 4096 me reserva para 4112
	m_bIgnoreChannel = M4_FALSE;
	m_bPreVal = M4_FALSE;

	//Pedrojam. Bug 0064098
	//Solucionar la seguridad en Delete Cascade
	m_iStatus = M4LDB_CONEX_STATUS_NONE;

	//Auditoria select.
	m_pSelectAuditory = new ClSelectAuditory (this);
	m_poAuditTranslationNode = NULL;
	m_lReenterCounter = 0;
	m_iIdSession = 0;

	//Detalle de trazas fichero.
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;	
	m_pIdRoleTrace = NULL;

#ifdef _M4LDB_DEBUG
	m_lInsInfo = 0;
	m_lUpdInfo = 0;
	m_lDelInfo = 0;
#endif

	m_bLicenseSummaryDumped = M4_FALSE;
}


m4return_t ClLConn::Init (void)
{
	if (!m_paRS) {
		m_paRS = new PClAccessRecordSet[M4LDB_SYS_NODES_TOTAL];
	}
	
	return (M4_SUCCESS);
}


ClLConn::~ClLConn (void)
{
	End ();

	if (m_paRS) {
		delete[] m_paRS;
	}
	m_paRS = NULL;

	if (m_pLdb) {
		m_pLdb->m_pFactory->MDFac_RemoveDictAccess (m_pAcsDict,m_oNameTrans.GetAccess());
	}
	m_oNameTrans.End();
	m_pLdb = NULL;

	m_pAcsDict = NULL;

#ifdef _M4LDB_DEBUG
	if (m_pInspector) {
		m_pInspector->Detach ();

		if (!m_pInspector->GetReferences ()) {
			delete m_pInspector;
		}
	}
#endif

	//Select auditoria.
	if (m_pSelectAuditory)
	{
		delete m_pSelectAuditory;
	}
}

m4return_t ClLConn::End (void){
	return _End();
}

m4return_t ClLConn::_End (void)
{
	ReleaseConnection ();

	if (m_plsPostValidation) {
		while (!m_plsPostValidation->empty ()) {

			StPostValidation *pPostValidation = (StPostValidation *) m_plsPostValidation->back ();

			m_plsPostValidation->pop_back ();

			delete pPostValidation;
		}
		delete m_plsPostValidation;
	}
	m_plsPostValidation = NULL;


	if (m_pmpTransCache) {
		m_pmpTransCache->erase (m_pmpTransCache->begin (), m_pmpTransCache->end());

		delete m_pmpTransCache;

		m_pmpTransCache = NULL;
	}


	if (m_pvcTransAccRl) {
		while (!m_pvcTransAccRl->empty()){
			StAccReload *pARl = m_pvcTransAccRl->back();

			m_pvcTransAccRl->pop_back();
			pARl->m_pAccess->m_pDestroy_Notifier->Dettach(this);
			delete pARl;

		}
		delete m_pvcTransAccRl;
	}
	m_pvcTransAccRl = NULL;

	m_pConnManager->DisconnectAll(this);

	m_numTrans = 0;

	return (M4_SUCCESS);
}

m4uint32_t ClLConn::GetSystemDebugDetailLevel (void)
{
	if (m_pLdb) {
		return m_pLdb->GetSystemDebugDetailLevel();			
	}
	else {
		return M4LDB_DETAIL_LEVEL_SHOW_NONE;
	}
}

m4bool_t ClLConn::GetLicenseSummaryEncrypted( string & ao_sLicenseSummary )
{
	//Esta funcion obtiene el resumen pero no lo escribe en ningun sitio.
	ao_sLicenseSummary.assign("");

	ClAccess * pAccess = m_pAcsDict;
	ClChannel *  pChannel = NULL;
	ClChannelManager * pCM = NULL;
	if ((pAccess != NULL)
		&& ((pChannel = pAccess->GetpChannel()) != NULL)
		&& ((pCM = pChannel->GetpChannelManager()) != NULL))
	{
		ILicenseManager * pLicenseManager = pCM->GetpLicenseManager();
		if ((pLicenseManager != NULL) 
			&& (pLicenseManager->IsInitialized() == M4_TRUE)) 
		{
			ao_sLicenseSummary.append(pLicenseManager->GetLicenseSummaryEncrypted());
			return M4_TRUE;
		}
	}
	return M4_FALSE;
}

void ClLConn::_DumpLicenseSummaryEncrypted( ClCompiledMCR *ai_poCMCR )
{
	//En la primera Load del sistema siempre volcaremos la informacion de la licencia.
	if( m_bLicenseSummaryDumped == M4_TRUE )
	{
		return;
	}

	if( ai_poCMCR != NULL )
	{
		// Si es el canal de licencia todavía no se ha leido el número
		m4id_t pccChannel = ai_poCMCR->GetChannelId() ;

		if( pccChannel != NULL && ( strcmpi( pccChannel, "SDC_MT_LICENSE" ) == 0 || strcmpi( pccChannel, "M4SYS_LICENSE" ) == 0 ) )
		{
			return;
		}
	}

	//Si no se ha volcado, este es el momento.
	string sLicenseSummary("");
	if (GetLicenseSummaryEncrypted(sLicenseSummary) == M4_TRUE) {		
		if (sLicenseSummary.size() > 0) {
			m4int32_t iDebugLevel = m_lSysDebugLevel;
			m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_0;
			M4LDB_PRINT_DEBUG (this, "\nWatermark. %s\n", sLicenseSummary.c_str());
			m_lSysDebugLevel = iDebugLevel;
		}
		m_bLicenseSummaryDumped = M4_TRUE;
	}
}


m4bool_t	ClLConn::IsForeignLoaded( void )
{
	ClMutBlock oMutextBlock (m_pLdb->GetContext()->m_poMutexForeigns);
	return M4LDB_BOOL( m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_COMPLETE_LOAD].Value.GetWithOutSec() );
}


void	ClLConn::SetForeignLoaded( void )
{
	m4VariantType vAux( 1.0 );

	ClMutBlock oMutextBlock (m_pLdb->GetContext()->m_poMutexForeigns);
	m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_COMPLETE_LOAD].Value.SetWithOutSec(vAux, M4_FALSE);
}


m4bool_t	ClLConn::IsRSMLoaded( void )
{
	ClMutBlock oMutextBlock (m_pLdb->GetContext()->m_poMutexRSMs);
	return M4LDB_BOOL( m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_COMPLETE_LOAD].Value.GetWithOutSec() );
}


void	ClLConn::SetRSMLoaded( void )
{
	m4VariantType vAux( 1.0 );

	ClMutBlock oMutextBlock (m_pLdb->GetContext()->m_poMutexRSMs);
	m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_COMPLETE_LOAD].Value.SetWithOutSec(vAux, M4_FALSE);
}


m4return_t ClLConn::FreeConnections (void)
{
	return m_pConnManager->DisconnectAll(this);
}


m4return_t ClLConn::ReleaseConnection (void)
{
	m_pLdb->GetVectorPartialLoad ().DiscardResults (m_idLConn, m_bIgnoreChannel);

	m_idExecution = 0;

	return (M4_SUCCESS);
}


void ClLConn::AuditTime( m4uint32_t iTotalTime )
{
	m4uint32_t	iAuditTime = m_pLdb->GetAuditTime() ;

	if( iAuditTime > 0 && iTotalTime > iAuditTime )
	{
		DUMP_CHLOG_ERRORF( M4_CH_AUDIT_TIME_EXCEEDED, ( m_pInformer != NULL ? m_pInformer : "" ) << iTotalTime << iAuditTime << DumpLogAndLdb(this) ) ;
	}
}


m4return_t ClLConn::_SysLoad (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_indSelect)
{
	m4return_t result;
	m4int32_t indSelect = 0;
	m4pcchar_t pAPISQL = NULL;

	if (ai_indSelect != M4LDB_NULL_INDEX) {
		indSelect = ai_indSelect;
	}
	
	ClCompiledMCR *pMCR = m_pLogMCR;
	m4int32_t hNode = m_hLogNode;

	m_poStatistics = ai_poStatistics;

	m4int32_t hTIStm = pMCR->GetNodeReadSentenceHandle (hNode);

	if (!hTIStm) {
		// No hay APISQL para este nodo. Retornamos exito.
		return M4_SUCCESS;
	}

	m4int32_t idSent = ((pMCR->GetChannelType () - 400) * 1000000 * (indSelect + 1)) + hNode;

	if (!indSelect) {
		pAPISQL = M4LDB_PCSTR (pMCR->GetSentenceApiSql (hTIStm));
	}
	else {
		pAPISQL = M4LDB_PCSTR (pMCR->GetSentenceApiSql2 (hTIStm));
	}

	if (*pAPISQL==0){
		// No hay APISQL para este nodo. Retornamos exito.
		return M4_SUCCESS;
	}

	/* Bug 0226308
	Se permite tener sentencias por gestor en los m2
	*/
	m4bool_t bIgnore = M4_FALSE ;

	if( *pAPISQL == '[' )
	{
		eDDBBType_t eType = M4DBW_ORACLE_DDBB ;

		if( m_pConnManager->GetDDBBType( 0, this, eType ) != M4_SUCCESS )
		{
			return M4_ERROR;
		}

		if(    ( pAPISQL[ 1 ] == '1' && eType == M4DBW_SQL_SERVER_DDBB )
			|| ( pAPISQL[ 1 ] == '3' && eType == M4DBW_ORACLE_DDBB ) )
		{
			pAPISQL += 3 ;
		}
		else
		{
			bIgnore = M4_TRUE ;
		}
	}

	if( bIgnore == M4_FALSE )
	{
		#ifdef _M4LDB_DEBUG
			m4id_t idT3 = pMCR->GetChannelId ();
			m4id_t idNode = pMCR->GetNodeId (hNode);
			M4LDB_PRINT_DEBUG (this, "\n--.--");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.--");
		#endif

		M4LDB_INFORMER (pAPISQL);

		ClGlobVars *pGlobVars = NULL;

		itMMpSentCacheSystem itLastSystemSent;
		ClLoad *pStm = (ClLoad *) m_pLdb->m_oStmtCache.FindSentSystem (idSent, itLastSystemSent);

		//Pedro: Tiempos en ldbinsp
		M4LDB_DUMPTIMES_DECLARE;
		M4LDB_DUMPTIMES_START;
		//Fin Pedro Tiempos.

		if (!pStm) {
			// No está, la creamos.
			pGlobVars = new ClGlobVars();
			pGlobVars->Set (this, 0, M4CL_LANGUAGE_DEFAULT, NULL, NULL, pMCR, hNode, ai_pRecordSet);

			pStm = new ClLoad (pGlobVars);
			// A partir de ahora el pGlobVars se borra desde el pStm.

			m_pLdb->m_oStmtCache.InsertSentSystem (idSent, itLastSystemSent, (ClStmt *) pStm);
			// A partir de ahora el pStm se borra desde el mapa.

			#ifdef _M4LDB_DEBUG
				if (*pAPISQL == 'S') {
					M4LDB_PRINT_DEBUG_DATE (this, "\n\nPrepare SysLoad. Meta4Object = %s. Node = %s.", idT3, idNode);
					M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\n\nPrepare SysLoad. Meta4Object = %s. Node = %s.", idT3, idNode);
				}
				else {
					M4LDB_PRINT_DEBUG_DATE (this, "\n\nPrepare SysPersist. Meta4Object = %s. Node = %s.", idT3, idNode);
					M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\n\nPrepare SysPersist. Meta4Object = %s. Node = %s.", idT3, idNode);
				}
			#endif

			ClMetaSentence *pMtSent = new ClMetaSentence;

			pMtSent->Create (pGlobVars, M4LDB_SELECT, M4_TRUE, M4_TRUE, M4_FALSE, M4_FALSE, M4_TRUE);

			ClStringCleaner oSC(&m_stBuffer);

			m_stBuffer = pAPISQL;

			ClAPISQL oAPISQL ((*pAPISQL == 'S') ? M4LDB_APISQL_REAL_SELECT : M4LDB_APISQL_REAL_STMT);

			result = oAPISQL.AnalizeSQL (m_stBuffer, pMtSent, M4_FALSE);
			
			if (result == M4_ERROR) {
				delete pMtSent;
				m_pLdb->m_oStmtCache.DeleteSentSystem (itLastSystemSent);
				return (M4_ERROR);
			}

			result = pStm->Open (0, pMtSent);

			// A partir de ahora el pMtSent se borra desde el pStm.

			if (result == M4_ERROR) {
				m_pLdb->m_oStmtCache.DeleteSentSystem (itLastSystemSent);
				return (M4_ERROR);
			}

			result = pStm->Prepare (m_stBuffer);

			if (result == M4_ERROR) {
				m_pLdb->m_oStmtCache.DeleteSentSystem (itLastSystemSent);
				return (M4_ERROR);
			}

			// Bindamos los parámetros.
			result = pStm->BindParam ();

			if (result == M4_ERROR) {
				m_pLdb->m_oStmtCache.DeleteSentSystem (itLastSystemSent);
				return (M4_ERROR);
			}

			if (*pAPISQL == 'S') {
				// Bindamos las columnas.
				result = pStm->BindCol ();

				if (result == M4_ERROR) {
					m_pLdb->m_oStmtCache.DeleteSentSystem (itLastSystemSent);
					return (M4_ERROR);
				}
			}

			// Cuidado, limpiamos los objetos del MetaSentence porque ya no se utilizarán mas.
			pStm->ClearMetaSentence ();
		}
		else {
			pGlobVars = pStm->GetpGlobVars ();
			pGlobVars->Set (this, 0, M4CL_LANGUAGE_DEFAULT, NULL, NULL, pMCR, hNode, ai_pRecordSet);

			pStm->SetCursor (this);
			pStm->InitStt ();
		}

		#ifdef _M4LDB_DEBUG
			if (*pAPISQL == 'S') {
				M4LDB_PRINT_DEBUG (this, "\n\nExecute SysLoad. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\nExecute SysLoad. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
			}
			else {
				M4LDB_PRINT_DEBUG (this, "\n\nExecute SysPersist. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\nExecute SysPersist. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
			}
		#endif

		//Introducimos los parametros y ejecutamos la sentencia.
		result = pStm->Execute (NULL, 0);

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.FreeSentSystem (itLastSystemSent);
			return (M4_ERROR);
		}

		if (*pAPISQL == 'S')
		{
			m4bool_t bHasLongItem = pStm->HasLongItem ();
				
			if (bHasLongItem == M4_TRUE) {
				ai_pRecordSet->SetCheckPointMode (M4_FALSE);
			}
			
			//Obtenemos los datos.
			result = pStm->GetData ();
			
			if (bHasLongItem == M4_TRUE) {
				ai_pRecordSet->SetCheckPointMode (M4_TRUE);
			}
			
			if (result == M4_ERROR) {
				m_pLdb->m_oStmtCache.FreeSentSystem (itLastSystemSent);
				return (M4_ERROR);
			}
			
			#ifdef _M4LDB_DEBUG
				if (m_pLdb->GetShowData ()) {
					m4int32_t lSysDebugLevel = m_lSysDebugLevel;
					m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
					M4LDB_PRINT_DEBUG (this, ai_pRecordSet);
					m_lSysDebugLevel = lSysDebugLevel;

				}
			#endif
		}
		
		m_pLdb->m_oStmtCache.FreeSentSystem (itLastSystemSent);

		//Pedro: Tiempos en ldbinsp
		#ifdef _M4LDB_DEBUG
			M4LDB_DUMPTIMES_STOP;

			AuditTime( long( _endingTime - _startingTime ) / 1000 );

			M4LDB_PRINT_DEBUG (this, "\n\tCalculating Time. Execute Load and Fetch. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\tCalculating Time. Execute Load and Fetch. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
			M4LDB_PRINT_DEBUG (this, M4LDB_DUMPTIMES_PRINT_TIMES);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, M4LDB_DUMPTIMES_PRINT_TIMES);
			M4LDB_DUMPTIMES_START;
		#endif
		//Fin Tiempos en ldbinsp
	}

	pAPISQL = M4LDB_PCSTR (pMCR->GetSentenceApiSql2 (hTIStm));

	if ((ai_indSelect == M4LDB_NULL_INDEX) && 
		(*pAPISQL)) {
		return (SysLoad (ai_pRecordSet, m_poStatistics, indSelect + 1));
	}

	return (M4_SUCCESS);	
}


m4return_t ClLConn::_Load (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData)
{
	m4return_t result;
	ClHandle hItem(ai_hItem);
	m4pcchar_t pAPISQLStm = M4LDB_EMPTY;
	m4bool_t bSelect = M4_FALSE;
	m4bool_t bFirst = M4_TRUE;
	m4bool_t bHaveConnFilter = M4_FALSE;
	m4bool_t bExistsSysParam= M4_FALSE;
	m4VariantType *pParam = ai_pParam;
	m4int32_t numExtParam = ai_numParam;
	m4int32_t hConn = 0;
	ClMetaSentence *pMtSent = NULL;
	itMMpSentCache itLastSent;
	m4uint32_t iCountBefore, iCountAfter;
	m4int32_t hTIStm = 0;
	m4int32_t iNumDataToRead = 0;


	if( ao_iNumData != NULL )
	{
		/* Bug 0070557.
		Nos guardamos el número de registros que realmente queremos leer.
		*/
		iNumDataToRead = *ao_iNumData ;

		/* Bug 0072419
		Hay que resetear la variable de salida por si nos salimos sin
		haber cargado nada.
		*/
		*ao_iNumData = 0 ;
	}

	ClAccess *pAccess = m_pLogAccess;
	ClCompiledMCR *pMCR = m_pLogMCR;
	m4int32_t hNode = m_hLogNode;
	ClChannel *pChannel = pAccess->GetpChannel();

	m_pLogonAdaptor = pChannel->GetpChannelManager()->GetLogonAdaptor();

	// Se vuelca la licencia (una sóla vez)
	_DumpLicenseSummaryEncrypted(pMCR);

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n--.--");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.--");
	#endif
	
	m_poStatistics = ai_poStatistics;

	m4id_t pIdRole = M4LDB_ID (pChannel->GetIDRole());
	m4id_t pIdRSM = M4LDB_ID (pChannel->GetIDRSM());
	m4id_t idT3 = M4LDB_ID (pMCR->GetChannelId());
	m4id_t idNode = M4LDB_ID (pMCR->GetNodeId(hNode));

	//Detalle de trazas fichero.
	if( pMCR != NULL )
	{
		m_pIdT3Trace = pMCR->GetChannelId();

		if( idNode != NULL )
		{
			m_pIdNodeTrace = idNode;
		}

		if( m_pLogonAdaptor != NULL && m_pLogonAdaptor->GetSessionChannelPointer() != NULL )
		{
			if( m_pLogonAdaptor->GetUsername() )
			{
				m_pIdUserTrace = m_pLogonAdaptor->GetUsername();
			}
			if( m_pLogonAdaptor->GetRole() )
			{
				m_pIdRoleTrace = m_pLogonAdaptor->GetRole();
			}				
		}
	}

	m4id_t idLaneNode = M4LDB_ID (pMCR->GetLaneSonNodeId ());

	m4double_t dOrgType;
	pChannel->OrganizationType.Get(dOrgType);
	m4char_t cOrgType = (m4char_t)dOrgType;
	
	// Situamos el register con el indice del current
	ai_pRecordSet->Register.MoveTo (ai_pRecordSet->Current.Index);

	m4double_t dLang = M4CL_LANGUAGE_DEFAULT;
	pChannel->Data_Language.Get(dLang);
	m4uint32_t lEnvLang = (m4uint32_t) dLang;

	if (lEnvLang == -1)
	{
		lEnvLang = M4CL_LANGUAGE_DEFAULT;
	}
	
	// Atención, el primer underscore significa que no se le aplican conectores.
	ClStringCleaner oSC(&m_stBuffer);
	m_stBuffer = "_";
	m_stBuffer.append(idT3);
	m_stBuffer.append("_");
	m_stBuffer.append(pIdRSM);
	
	if ((*idLaneNode) && 
		(!strcmpi (idLaneNode, idNode))) {
		
		m_stBuffer.append("_");
		m_stBuffer.append(pMCR->GetLaneFatherNodeId());
		m_stBuffer.append("_");
		m_stBuffer.append(pMCR->GetLaneFatherChannelId());
	}
	m_stBuffer.append(" ");
	
	m4char_t idAutoFilter = _GetIdAutoFilter( ai_pRecordSet, pMCR, hNode );

	// Añadimos la sentencia APISQL que nos viene del ExecuteSQL.
	if (hItem) {
		// Tenemos una sentencia APISQL que nos viene del ExecuteSQL.
		pAPISQLStm = M4LDB_PCSTR (ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec ());
		
		if (!*pAPISQLStm) {
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, "<null>");
			return (M4_ERROR);
		}
		
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in ExecuteSQL = %s.", pAPISQLStm);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in ExecuteSQL = %s.", pAPISQLStm);
		#endif
		
		if ((*pAPISQLStm != 's') && 
			(*pAPISQLStm != 'S') &&
			(*pAPISQLStm != 'f') &&
			(*pAPISQLStm != 'F')) {
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQLStm);
			return (M4_ERROR);
		}
		
		if ((*pAPISQLStm == 's') || 
			(*pAPISQLStm == 'S')) {
			bSelect = M4_TRUE;
		}
		
		m_stBuffer.append (pAPISQLStm);
		
		bFirst = M4_FALSE;
	}
	
	if (!bSelect) {
		m4int32_t indSysSent;
		m4int32_t numSysSent = pMCR->GetNodeNumberOfSysSentences (hNode);
		
		for (indSysSent=0; indSysSent<numSysSent; indSysSent++) {
			// Añadimos la sentencia APISQL definida en el Item SysSentence.
			hItem = pMCR->GetNodeSysSentenceHandleByPosition (hNode, (m4int16_t) indSysSent);
			
			if (hItem) {
				// Tenemos una sentencia APISQL que nos viene del Item SysSentence.
				m4VariantType vtAPISQL;
				result = ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec (vtAPISQL);
				
				if (result == M4_SUCCESS && vtAPISQL.Type == M4CL_CPP_TYPE_STRING_VAR && vtAPISQL.Data.PointerChar != NULL && *(pAPISQLStm = M4LDB_PCSTR (vtAPISQL.Data.PointerChar)) != '\0')
				{
					if (_CheckUserSQL (vtAPISQL.IsUser, pAPISQLStm) != M4_SUCCESS)
					{
						return (M4_ERROR);
					}

					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in SysSentence[%ld] = %s.", indSysSent, pAPISQLStm);
						M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in SysSentence[%ld] = %s.", indSysSent, pAPISQLStm);
					#endif
					
					if ((*pAPISQLStm != 's') && 
						(*pAPISQLStm != 'S') &&
						(*pAPISQLStm != 'f') &&
						(*pAPISQLStm != 'F')) {
						DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQLStm);
						return (M4_ERROR);
					}
					
					if (!bFirst) {
						m_stBuffer.append (";");
					}
					
					m_stBuffer.append (pAPISQLStm);
					
					if ((*pAPISQLStm == 's') || 
						(*pAPISQLStm == 'S')) {
						bSelect = M4_TRUE;
					}

					bFirst = M4_FALSE;

					ClHandle hItemAux = pMCR->GetItemAuxItem (hItem);
					
					if ((hItemAux) && 
						(!ai_pParam)) {
						bExistsSysParam = M4_TRUE;
										
						#ifdef _M4LDB_DEBUG
							m4VariantType vtData;

							ai_pRecordSet->Register.Item[hItemAux].Value.GetWithOutSec (vtData);

							/* Bug 0132263
							Hay que controlar que el sysparam sea una cadena válida
							*/
							m4pchar_t pBeginToken = "";

							if( vtData.Type == M4CL_CPP_TYPE_STRING_VAR && vtData.Data.PointerChar != NULL )
							{
								pBeginToken = vtData.Data.PointerChar ;
							}
								
							M4LDB_PRINT_DEBUG (this, "\n\tSysParam = %s.", M4LDB_PSTR(pBeginToken));
							M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n\tSysParam = %s.", M4LDB_PSTR(pBeginToken));
						#endif
					}
					else {
						#ifdef _M4LDB_DEBUG
							M4LDB_PRINT_DEBUG (this, "\n\tNo SysParam defined");
							M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n\tNo SysParam defined");
						#endif
					}
				}
			}
		}
	}
		
	if (!bSelect) {
		// Añadimos el filtro APISQL del conector..
		if (ai_pRecordSet->GetpNode_ExtInfo ()->Parent.GetpNotFree ()) {
			hConn = ai_pRecordSet->GetpNode_ExtInfo ()->Parent.GetpNotFree ()->GetHandle ();                
			
			if (hConn) {
				m4int32_t hConnStm = pMCR->GetNodeConnectorSentenceHandle (hConn);
				
				if (hConnStm) {
					pAPISQLStm = M4LDB_PCSTR (pMCR->GetSentenceApiSql (hConnStm));
					
					if (!*pAPISQLStm) {
						DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, "<null>");
						return (M4_ERROR);
					}
					
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in Connector = %s.", pAPISQLStm);
						M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in Connector = %s.", pAPISQLStm);
					#endif
					
					if ((*pAPISQLStm != 'f') &&
						(*pAPISQLStm != 'F')) {
						DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQLStm);
						return (M4_ERROR);
					}
					
					if (!bFirst) {
						m_stBuffer.append (";");
					}
					
					m_stBuffer.append ("C"); // Esto es para indicar que es el filtro del conector.
					
					m_stBuffer.append (pAPISQLStm);

					//Bug 0089792.
					//Escenario:
					//Tenemos EN con tabla base &T1. La sentencia de conector tendrá la misma tabla base.
					//Si ahora heredamos la EN, tendremos una tabla base &T2 que será descendiente de T1, el problema
					//es que el conector continuará teniendo tabla base &T1 y hay un problema por tablas base diferentes.
					//Lo que vamos a hacer es sustituir la tabla base del conector &T1 por la tabla base que habrá en
					//la EN heredada &T2.

					//Sólo tenemos en cuenta este razonamiento si la Ti es heredada!!.
					if (pMCR->GetNodeTiInheritLevel( hNode ) > 1) {
						//Buscamos el ampersand y el primer blanco despues del ampersand.
						m4pcchar_t pReadObjectId = pMCR->GetNodeReadObjectId( hNode );						
						m4int32_t iPosBasis = m_stBuffer.find( "&" );
						m4int32_t iPosBlank = M4_STR_NPOS;
						if ((iPosBasis != M4_STR_NPOS) && (pReadObjectId != NULL)) {
							iPosBlank = m_stBuffer.find( " ", iPosBasis );
							//Sustituimos el id de la tabla base por el que viene en la sentencia.
							if (iPosBlank != M4_STR_NPOS) {
								m_stBuffer.replace( iPosBasis + 1, (iPosBlank - iPosBasis - 1), pReadObjectId );
							}
						}
					}

					bFirst = M4_FALSE;
				}
			}
		}
	}

	// La cadena de la clave no necesita contener la select del APISQL porque utilizamos el handle de la sentencia de la TI para ello.
	string stKey = m_stBuffer;

	if (!bSelect) {
		hTIStm = pMCR->GetNodeReadSentenceHandle (hNode);
		
		// Añadimos la sentencia APISQL definida en la sentencia de la TI.
		if (hTIStm) {
			pAPISQLStm = M4LDB_PCSTR (pMCR->GetSentenceApiSql (hTIStm));
			
			if (!*pAPISQLStm) {
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, "<null>");
				return (M4_ERROR);
			}
			
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in TI = %s.", pAPISQLStm);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in TI = %s.", pAPISQLStm);
			#endif
			
			switch (*pAPISQLStm)
			{
				case 's':
				case 'S':
					bSelect =M4_TRUE;
					// no hay break a proposito.
				case 'f':
				case 'F':
					break;
				default:
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQLStm);
				return (M4_ERROR);
					break;
			}
			
			if (!bFirst) {
				m_stBuffer.append (";");
			}
			
			m_stBuffer.append (pAPISQLStm);

			bFirst = M4_FALSE;
			
			// Si anteriormente no hemos añadido ninguna select completa significa que podemos utilizar los conectores en el filtro.
			bHaveConnFilter = M4_TRUE;
		}
	}
	
	// Vamos a conseguir el filtro de seguridad de la TI.
	pAPISQLStm = M4LDB_PCSTR (pMCR->GetNodeSecFilter (hNode));
	
	if (*pAPISQLStm) {
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in FilterSecTI = %s.", pAPISQLStm);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in FilterSecTI = %s.", pAPISQLStm);
		#endif
		
		if ((*pAPISQLStm != 'f') &&
			(*pAPISQLStm != 'F')) {
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQLStm);
			return (M4_ERROR);
		}
		
		if (!bFirst) {
			m_stBuffer.append (";");
		}
		
		m_stBuffer.append (pAPISQLStm);

		stKey.append (pAPISQLStm);

		bFirst = M4_FALSE;
	}

	if (ai_hItem==0){
		// Si no nos ha venido por ExecuteSQL.
		ClHandle 	hOrderBy (pMCR->GetNodeRAMOrderByItem(hNode));
		m4pcchar_t	pOrderBy = 0;
		if (hOrderBy){
			pOrderBy = M4LDB_PCSTR (ai_pRecordSet->Current.Item[hOrderBy].Value.GetWithOutSec ());
			if (*pOrderBy){
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (this, "\n SysSentence [Order By] = %s.", pOrderBy);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n SysSentence [Order By] = %s.", pOrderBy);
				#endif
				if (*pOrderBy!='o' && *pOrderBy!='O'){
					// solo puede venir un Order By.
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pOrderBy);
					return (M4_ERROR);
				}
				if (bFirst) {
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pOrderBy);
					return (M4_ERROR);
				}
				m_stBuffer.append(";");

				m_stBuffer.append(pOrderBy);

				stKey.append(pOrderBy);
			}
		}
	}

	m4pcchar_t pAuxAPISQL = m_stBuffer.c_str ();

	M4LDB_INFORMER (pAuxAPISQL);
	

	// Se asigna el modo de tramitatción a la caché
	m4bool_t bUseTramitation = M4_FALSE;
	m4pchar_t pcTramitation = NULL;

	pChannel->Tramitation.Get( pcTramitation );

	if( pcTramitation != NULL )
	{
		bUseTramitation = M4_TRUE ;
	}
	else
	{
		bUseTramitation = M4_FALSE ;
	}

	m4date_t	dtSessionVersion = M4LDB_MINUS_INF_DATE ;

	if( m_pLogonAdaptor != NULL )
	{
		ClChannel*	poSessionChannel = m_pLogonAdaptor->GetSessionChannelPointer() ;

		if( poSessionChannel != NULL )
		{
			ClCompiledMCR*	poSessionMCR = poSessionChannel->GetpCMCR() ;

			if( poSessionMCR != NULL )
			{
				dtSessionVersion = poSessionMCR->GetChannelVersion() ;
			}
		}
	}

	m4bool_t bHasRange = M4_FALSE;
	
	if( ai_pRecordSet->GetCurrentBlock()->GetRangeSize() > 0 )
	{
		bHasRange = M4_TRUE;
	}

	m4bool_t bSecurityView = M4_FALSE ;
	m4int32_t hBDLChecks = pMCR->GetNodeBDLChecksItem( hNode ) ;

	if( hBDLChecks != 0 )
	{
		m4int32_t lMask = M4LDB_NUMBER( ai_pRecordSet->Item[(ClHandle) hBDLChecks].Value.Get() ) ;
		bSecurityView = ( M4LDB_NO_USE_SECURITY_VIEW & lMask ) == 0 && ( m_pLdb->UseSecurityView() == M4_TRUE || ( M4LDB_USE_SECURITY_VIEW & lMask ) != 0 ) ;
	}

	StSentKey oSentKey( hNode, pMCR->GetChannelVersion(), dtSessionVersion, lEnvLang, idAutoFilter, pMCR->GetChannelCsType(), 'L', pIdRole, stKey.c_str (), cOrgType, 0, hTIStm, M4_TRUE, bUseTramitation, bHasRange, bSecurityView );

	if (ai_idRConn > 0)
	{
		oSentKey.m_idRConn = ai_idRConn;
	}

	ClGlobVars *pGlobVars = NULL;

	ClLoad *pStm = (ClLoad *) m_pLdb->m_oStmtCache.FindSent (oSentKey, itLastSent, this);
	
	//---------------------------------------------------------------------------------
	//Siempre buscamos hints.
	VcSysHint vVcSysHint;
	if (_GetSysHints(vVcSysHint, ai_pRecordSet, pMCR, hNode) != M4_SUCCESS) {
		if (pStm != NULL) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);				
		}
		return (M4_ERROR);
	}

	//Si está cacheada pero los hints han cambiado entonces tenemos que preparar...
	if ((pStm != NULL) && (pStm->GetMetaSentence() != NULL)) {				
		//Si está en la cache pero han cambiado los hints tenemos que quitarla y borrarla de la cache.			
		if (pStm->GetMetaSentence()->IsSameSysHints(vVcSysHint) != M4_TRUE) {
			//Si han cambiado tengo que resetear el statement para que se rehaga.
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);	
			pStm = NULL;
		}
	}
	//---------------------------------------------------------------------------------

	//Pedro: Tiempos en ldbinsp
	M4LDB_DUMPTIMES_DECLARE;
	M4LDB_DUMPTIMES_START;
	//Fin Pedro Tiempos.

	if (!pStm) {
		// No está, la creamos.
		
		if (!bSelect) {
			DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_WRONG_APISQL, "<null>");
			return (M4_WARNING);
		}

		pGlobVars = new ClGlobVars();
		pGlobVars->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode, ai_pRecordSet);

		pStm = new ClLoad (pGlobVars, M4_FALSE);
		
		m_pLdb->m_oStmtCache.InsertSent (oSentKey, itLastSent, (ClStmt *) pStm, this);

		m4bool_t bIsPartialLoad = (ao_iNumData) ? M4_TRUE : M4_FALSE;

		pMtSent = new ClMetaSentence;
		
		pMtSent->Create (pGlobVars, M4LDB_SELECT, M4_TRUE, M4_FALSE, bHasRange, bIsPartialLoad, M4_TRUE);

		if (vVcSysHint.size() != 0) {
			if (pMtSent->AddListOfSysHint( vVcSysHint ) != M4_SUCCESS) {
				delete pMtSent;
				m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
				return (M4_ERROR);
			}
		}
		
		ClAPISQL oAPISQL (M4LDB_APISQL_SELECT);

		result = oAPISQL.AnalizeSQL (m_stBuffer, pMtSent, M4_TRUE, bHaveConnFilter, idAutoFilter);

//			if (result == M4_ERROR) {
		if (result != M4_SUCCESS) {
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (result);
		}

		result = _X_AnalizeSecurity (&oAPISQL, pMtSent, pGlobVars, ai_pRecordSet, NULL, M4LDB_SELECT, NULL);
		
		if (result != M4_SUCCESS) {
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (result);
		} 
		
/*
		// NO BORRAR...
		// Esto es para la auditoria de procesos.
		result = AnalizeAuditory (pMtSent);

		if (result == M4_ERROR) {
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
*/

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG_DATE (this, "\n\nPrepare Load. Meta4Object = %s. Node = %s.", idT3, idNode);
			M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\n\nPrepare Load. Meta4Object = %s. Node = %s.", idT3, idNode);
		#endif
		
		result = pStm->Open (ai_idRConn, pMtSent);
		
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
		
		result = pStm->Prepare (m_stBuffer);
        
//			if (result == M4_ERROR) {
		if (result != M4_SUCCESS) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (result);
		}
		

		// Establecer la fecha de cierre
		result = SetClosingDate (pMtSent, ai_pRecordSet);
		
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
		
		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
		
		// Bindamos las columnas.
		result = pStm->BindCol ();
		
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
		
		// Cuidado, limpiamos los objetos del MetaSentence porque ya no se utilizarán mas.
		//<<< Select auditoria. Comentamos la línea, en el destructor ya se hará la limpieza.
		//Al limpiar se pone a nulo el vector de from, pero ese vector se necesita para
		//obtener las tablas involucradas en la select.
		//pStm->ClearMetaSentence ();
		//>>> fin select auditoria.

	}
	else {
		pGlobVars = pStm->GetpGlobVars ();
		pGlobVars->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode, ai_pRecordSet);

		pStm->SetCursor (this);
		pMtSent = pStm->GetMetaSentence ();
		pStm->InitStt ();
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\nExecute Load. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\nExecute Load. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
	#endif
	
	if (bExistsSysParam) {
		m4int32_t indSysSent;
		m4int32_t numSysSent = pMCR->GetNodeNumberOfSysSentences (hNode);
		
		for (indSysSent = 0; indSysSent<numSysSent; indSysSent++) {				
			// Añadimos la sentencia APISQL definida en el Item SysSentence.
			hItem = pMCR->GetNodeSysSentenceHandleByPosition (hNode, (m4int16_t) indSysSent);
			
			if (hItem) {
				// Tenemos una sentencia APISQL que nos viene del Item SysSentence.
				pAPISQLStm = M4LDB_PCSTR (ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec ());
				
				if (*pAPISQLStm) {
					ClHandle hItemAux = pMCR->GetItemAuxItem (hItem);
					
					if ((hItemAux) && 
						(!ai_pParam)) {
						if (!pParam){
							pParam = new m4VariantType[MAX_NUMBER_OF_PARAMETERS];
						}
						GetSysParam (ai_pRecordSet, pMCR->GetItemIndex(hItemAux), pParam, numExtParam);
					}
				}
			}
		}
	}

	m4uint32_t	iTimeOut = m_pLdb->GetExecutionTimeOut() ;

	if( iTimeOut > 0 )
	{
		m4uint32_t	iBlockTimeOut = ai_pRecordSet->GetCurrentBlock()->GetTimeOut() ;

		if( iBlockTimeOut > 0 && iBlockTimeOut < iTimeOut )
		{
			iTimeOut = iBlockTimeOut ;
		}
	}

	//Introducimos los parametros y ejecutamos la sentencia.
	result = pStm->Execute( pParam, numExtParam, NULL, ai_iFirstData, iTimeOut ) ;

	if( result == M4_SUCCESS && bHasRange == M4_TRUE )
	{
		ai_pRecordSet->GetCurrentBlock()->SetRangeStartNext() ;
	}

	if ((pParam) && 
		(!ai_pParam)) {
		delete [] pParam;
		pParam = NULL;
	}
			
/*
// NO BORRAR...
// Esto es para la auditoria de procesos.
	RealizeAuditory (pMtSent);
*/

	if (result == M4_ERROR) {
		m_pLdb->m_oStmtCache.FreeSent (itLastSent);
		return (M4_ERROR);
	}
	
	m4bool_t bHasLongItem = pStm->HasLongItem ();
	
	if (bHasLongItem == M4_TRUE) {
		iCountBefore = ai_pRecordSet->Count();
//			ai_pRecordSet->SetCheckPointMode (M4_FALSE);
	}
	
	m4int32_t lNumData = pMCR->GetNodeNumRowsDB( hNode ) ;

	if( lNumData > 0 )
	{
		// Enhancement 007978
		// Sólo se coge la propiedad si ya el nodo tiene NumRowsDB
		m4VariantType vValue;
		if( pChannel->Property.GetPropValue( M4CMPROP_CHANNEL_N_ROWS_DB, vValue ) == M4_SUCCESS )
		{
			if( vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData > 0 )
			{
				lNumData = (m4int32_t) vValue.Data.DoubleData ;
			}
		}
	}
	
	if( ao_iNumData || lNumData == 0 )
	{

		/* Bug 0070557.
		Al final se actualiza el estado de finalización de carga parcial.
		*/
		if( ao_iNumData )
		{
			pStm->GetpGlobVars()->GetpAccessRecordSet()->Register.SetEndOfRegisters( M4_FALSE ) ;

			// Hay que volver a poner el ao_iNumData porque el GetData espera el numero de registros a leer
			*ao_iNumData = iNumDataToRead ;
		}

		//Obtenemos los datos.
		result = pStm->GetData (ao_iNumData, ai_iFirstData, M4LDB_NULL_INDEX);

		if( ao_iNumData )
		{
			if( *ao_iNumData < iNumDataToRead )
			{
				pStm->GetpGlobVars()->GetpAccessRecordSet()->Register.SetEndOfRegisters( M4_TRUE ) ;
			}	
			else
			{
				pStm->GetpGlobVars()->GetpAccessRecordSet()->Register.SetEndOfRegisters( M4_FALSE ) ;
			}
		}
	}
	else {
		//Obtenemos los datos.
		result = pStm->GetData (&lNumData, M4LDB_NULL_INDEX);
	}
	
	if (bHasLongItem == M4_TRUE) {
		// para todos los registros insertados con la load,
		// hago checkpoint
		iCountAfter = ai_pRecordSet->Count();
		while (iCountAfter != iCountBefore) {
			ai_pRecordSet->CheckPoint(-- iCountAfter);
		}
		//luego actualizo el estado del VRB
		m4uint8_t iop;
		ai_pRecordSet->GetCurrentBlock()->RefreshFlags(iop);
	}
	

	if (result == M4_ERROR) {
		pStm->Close ();
		m_pLdb->m_oStmtCache.FreeSent (itLastSent);
		return (M4_ERROR);
	}
	
	#ifdef _M4LDB_DEBUG
		if ((!ao_iNumData) && 
			(m_pLdb->GetShowData ())) {
			m4int32_t lSysDebugLevel = m_lSysDebugLevel;
			m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
			M4LDB_PRINT_DEBUG (this, ai_pRecordSet);
			m_lSysDebugLevel = lSysDebugLevel;
		}
	#endif

	//Bug 0074647.
	//Problemas auditando en SQLServer SCH_RATES, SDC_LOGIC_OBJECTS
	//Auditoria Select.
	//Si la ejecución ha ido bien lanzamos la auditoría.
	//Importante: Es necesario no haber hecho pStm->ClearMetaSentence ();
	//porque en auditoria se necesita saber las tablas del from.
	if (result != M4_ERROR) {
		//Pasamos pStm porque a partir de la load consigo el cursor y de este
		//obtengo la sentencia real.
		result = AnalizeAuditorySelect (pStm, ai_pRecordSet);
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.FreeSent (itLastSent);
			return M4_ERROR;
		}
	}
	//Fin Auditoria Select.

	m_pLdb->m_oStmtCache.FreeSent (itLastSent);

	//Pedro: Tiempos en ldbinsp
	#ifdef _M4LDB_DEBUG
		M4LDB_DUMPTIMES_STOP;

		AuditTime( long( _endingTime - _startingTime ) / 1000 );

		M4LDB_PRINT_DEBUG (this, "\n\tCalculating Time. Execute Load and Fetch. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\tCalculating Time. Execute Load and Fetch. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG (this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_DUMPTIMES_START;
	#endif
	//Fin Tiempos en ldbinsp

	return (M4_SUCCESS);
}


m4return_t ClLConn::_Persist (ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t ai_lMask)
{
	m4return_t result;
	m4bool_t bInvRS = M4_FALSE;
	ClHandle hItem (ai_hItem);
	m4bool_t bAPISQL = M4_FALSE;
	m4id_t pIdUser = g_aM4ADM;
	m4id_t pIdDebugUser = g_aM4ADM;
	ClMetaSentence *pMtSent = NULL;
	itMMpSentCache itLastSent;
	m4uint32_t numPost = m_plsPostValidation->size ();

	// Obtenemos el metacanal compilado.
	ClAccess *pAccess = m_pLogAccess;
	ClCompiledMCR *pMCR = m_pLogMCR;
	m4int32_t hNode = m_hLogNode;
	ClChannel *pChannel = pAccess->GetpChannel();

	m_pLogonAdaptor = pChannel->GetpChannelManager()->GetLogonAdaptor();
	
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n--.--");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.--");
	#endif

	m_poStatistics = ai_poStatistics;

	m4id_t idT3 = M4LDB_ID (pMCR->GetChannelId ());
	m4id_t idNode = M4LDB_ID (pMCR->GetNodeId (hNode));

	//Detalle de trazas fichero.
	if( pMCR != NULL )
	{
		m_pIdT3Trace = pMCR->GetChannelId();

		if( idNode != NULL )
		{
			m_pIdNodeTrace = idNode;
		}

		if( m_pLogonAdaptor != NULL && m_pLogonAdaptor->GetSessionChannelPointer() != NULL )
		{
			if( m_pLogonAdaptor->GetUsername() )
			{
				m_pIdUserTrace = m_pLogonAdaptor->GetUsername();
			}
			if( m_pLogonAdaptor->GetRole() )
			{
				m_pIdRoleTrace = m_pLogonAdaptor->GetRole();
			}				
		}
	}
	
	m4double_t dOrgType;
	pChannel->OrganizationType.Get(dOrgType);
	m4char_t cOrgType = (m4char_t)dOrgType;

	m4id_t pIdRole = M4LDB_ID (pChannel->GetIDRole());
	m4id_t pIdRSM = M4LDB_ID (pChannel->GetIDRSM());

	if( m_pLogonAdaptor->GetSessionChannelPointer() != NULL )
	{
		pIdUser = M4LDB_ID (m_pLogonAdaptor->GetSessionRecordSet()->Current.Item[M4_ITEM_USER].Value.GetWithOutSec ());
		pIdDebugUser = M4LDB_ID (m_pLogonAdaptor->GetSessionRecordSet()->Current.Item[M4_ITEM_DEBUG_USER].Value.GetWithOutSec ());

		if( pIdDebugUser == NULL || *pIdDebugUser == '\0' )
		{
			pIdDebugUser = pIdUser;
		}
	}

	// Situamos el register con el indice del current
	ai_pRecordSet->Register.MoveTo (ai_pRecordSet->Current.Index);

	m4double_t dLang = M4CL_LANGUAGE_DEFAULT;
	pChannel->Data_Language.Get(dLang);
	m4uint32_t lEnvLang = (m4uint32_t) dLang;

	if (lEnvLang == -1)
	{
		lEnvLang = M4CL_LANGUAGE_DEFAULT;
	}
	
	ClStringCleaner oSC(&m_stBuffer);

	m_stBuffer = idT3;
	m_stBuffer.append("_");
	m_stBuffer.append(pIdRSM);
	m_stBuffer.append(" ");

	if (hItem)
	{
		m4pcchar_t pAPISQLStm = M4LDB_PCSTR (ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec ());
		m_stBuffer.append (pAPISQLStm);

		bAPISQL = M4_TRUE;

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n APISQL Statement in ExecuteSQL = %s.", pAPISQLStm);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION | M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n APISQL Statement in ExecuteSQL = %s.", pAPISQLStm);
		#endif
	}


	M4LDB_INFORMER (m_stBuffer.c_str ());

	/* Bug 0123525
	Hay que poner si se optimiza o no como pk de la caché de sentancias
	*/
	m4bool_t	bHasValidation = M4_TRUE ;

	if( bAPISQL == M4_TRUE && ai_lMask != 0 )
	{
		bHasValidation = M4_FALSE ;
	}
			
	//---------------------------------------------------------------------
	//Si llega ai_idRConn, busco en cache por ese Id:
	//	Si no esta en cache -> Se guarda en cache por ese Id.
	//	Si sí está en cache -> Se coge sin más.
	//Si no llega ai_idRConn se busca con 0:
	//	Si no esta en cache -> Se guarda en cache por 0, y el analisis permitira saber que Id es el correcto.
	//	Si sí está en cache -> Se coge sin más. Al final terminará ejecutandose donde diga el pStm.

	// Se asigna el modo de tramitatción a la caché
	m4bool_t bUseTramitation = M4_FALSE;
	m4pchar_t pcTramitation = NULL;

	pChannel->Tramitation.Get( pcTramitation );

	if( pcTramitation != NULL )
	{
		bUseTramitation = M4_TRUE ;
	}
	else
	{
		bUseTramitation = M4_FALSE ;
	}

	m4date_t	dtSessionVersion = M4LDB_MINUS_INF_DATE ;

	if( m_pLogonAdaptor != NULL )
	{
		ClChannel*	poSessionChannel = m_pLogonAdaptor->GetSessionChannelPointer() ;

		if( poSessionChannel != NULL )
		{
			ClCompiledMCR*	poSessionMCR = poSessionChannel->GetpCMCR() ;

			if( poSessionMCR != NULL )
			{
				dtSessionVersion = poSessionMCR->GetChannelVersion() ;
			}
		}
	}

	StSentKey oSentKey( hNode, pMCR->GetChannelVersion(), dtSessionVersion, lEnvLang, 0, pMCR->GetChannelCsType(), *g_aPersist[ai_eStmType], pIdRole, m_stBuffer.c_str(), cOrgType, 0, 0, bHasValidation, bUseTramitation, M4_FALSE, M4_FALSE );

	if (ai_idRConn > 0)
	{
		oSentKey.m_idRConn = ai_idRConn;
	}
	//---------------------------------------------------------------------
	
	ClGlobVars *pGlobVars = NULL;

	ClPersist *pStm = (ClPersist *) m_pLdb->m_oStmtCache.FindSent (oSentKey, itLastSent, this);
	
	//Pedro: Tiempos en ldbinsp
	M4LDB_DUMPTIMES_DECLARE;
	M4LDB_DUMPTIMES_START;
	//Fin Pedro Tiempos.

	if (!pStm)
	{
		// No está, la creamos.
		pGlobVars = new ClGlobVars();
		pGlobVars->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode, ai_pRecordSet, pIdUser, pIdDebugUser);

		// Se calsula si el canal tiene herencia de datos o no
		m4bool_t bUseInheritance = M4_FALSE;
		if( cOrgType != M4CL_ORGANIZATION_TYPE_NONE )
		{
			bUseInheritance = M4_TRUE ;
		}
		else
		{
			bUseInheritance = M4_FALSE ;
		}

		pStm = new ClPersist (pGlobVars, M4_FALSE, bUseTramitation, bUseInheritance, M4_TRUE);

		m_pLdb->m_oStmtCache.InsertSent (oSentKey, itLastSent, (ClStmt *) pStm, this);

		pMtSent = new ClMetaSentence;

		if (bAPISQL)
		{
			pMtSent->Create (pGlobVars, ai_eStmType, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE, bHasValidation);
			
			ClAPISQL o_X_APISQL ((eApiSqlType_t) ai_eStmType);
			result = o_X_APISQL.AnalizeSQL (m_stBuffer, pMtSent, M4_TRUE);

			if (result == M4_ERROR)
			{
				delete pMtSent;
				m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
				return (M4_ERROR);
			}
		}
		else
		{
			pMtSent->Create (pGlobVars, ai_eStmType, M4_FALSE, M4_FALSE, M4_FALSE, M4_FALSE, bHasValidation);
		}


		// Obtenemos el identificador de la tabla de escritura.
		m4id_t idLgcTbl = pMtSent->GetIdObjectBasis ();

		if (!*idLgcTbl)
		{
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
		    DUMP_LDBINSP_WARNING (this, M4_CH_LDB_LOGIC_OBJECT_NOT_DEFINED);
			return (M4_WARNING);
		}

		// Esto nos situa el logic object.
		result = LoadDictionaryChannel (idLgcTbl, NULL, M4_TRUE);
		
		if (result == M4_ERROR) {
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

/*
		// NO BORRAR...
		// Esto es para la auditoria de procesos.
		result = AnalizeAuditory (pMtSent);

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
*/

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG_DATE (this, "\n\nPrepare %s. Meta4Object = %s. Node = %s.", g_aPersist[ai_eStmType], idT3, idNode);
			M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\n\nPrepare %s. Meta4Object = %s. Node = %s.", g_aPersist[ai_eStmType], idT3, idNode);
		#endif

		m4int32_t idRConn = pGlobVars->GetIdRConn ();

		if (idRConn == M4LDB_NULL_INDEX)
		{
			idRConn = M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[(ClRegisterIndex) 0].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
		}

		result = pStm->Open (ai_eStmType, idRConn, pMtSent);

		if (result == M4_ERROR)
		{
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		result = pStm->Prepare ();
        
		if (result == M4_ERROR)
		{
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR)
		{
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
	}
	else
	{
		pGlobVars = pStm->GetpGlobVars ();
		pGlobVars->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode, ai_pRecordSet, pIdUser, pIdDebugUser);
		pMtSent = pStm->GetMetaSentence ();
			
		pStm->InitStt ();

		// Obtenemos el identificador de la tabla de escritura.
		m4id_t idLgcTbl = pMtSent->GetIdObjectBasis ();

		result = LoadDictionaryChannel (idLgcTbl, NULL, M4_TRUE);

		if (result != M4_SUCCESS)
		{
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}
	}
		
	pGlobVars->SetPostValidate (M4_TRUE);

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\nExecute %s. Meta4Object = %s. Node = %s. RecordSet = %ld. NumReg = %ld.", g_aPersist[ai_eStmType], idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey, ai_pRecordSet->Count ());
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\nExecute %s. Meta4Object = %s. Node = %s. RecordSet = %ld. NumReg = %ld.", g_aPersist[ai_eStmType], idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey, ai_pRecordSet->Count ());
	#endif

	//Introducimos los parametros y ejecutamos la sentencia.
	if (bAPISQL)
	{
		result = pStm->Execute2 (ai_pParam, ai_numParam);
	}
	else
	{
		//Bug 0081643
		//Marcamos la persist como raiz de la recursión que se lanzará.
		pStm->SetIsRootInRecursion (M4_TRUE);
		result = pStm->Execute2 ();
		pStm->SetIsRootInRecursion (M4_FALSE);
	}


/*
// NO BORRAR...
// Esto es para la auditoria de procesos.
	RealizeAuditory (pMtSent);
*/

	if (_FindPersistedAccess(pAccess)==M4_FALSE)
	{
		m_pvcTransAccRl->push_back (new StAccReload (pAccess));
        pAccess->m_pDestroy_Notifier->Attach(this);
	}
	
	if (result == M4_ERROR)
	{
		m_pLdb->m_oStmtCache.FreeSent (itLastSent);
		return (M4_ERROR);
	}
	
	// Si no fuera igual no lo soltamos hasta el End Transaction.
	if (m_plsPostValidation->size () == numPost)
	{
		m_pLdb->m_oStmtCache.FreeSent (itLastSent);
	}

	//Pedro: Tiempos en ldbinsp
	#ifdef _M4LDB_DEBUG
		M4LDB_DUMPTIMES_STOP;
		M4LDB_PRINT_DEBUG (this, "\n\tCalculating Time. Execute %s. Meta4Object = %s. Node = %s. RecordSet = %ld. NumReg = %ld.", g_aPersist[ai_eStmType], idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey, ai_pRecordSet->Count ());
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\tCalculating Time. Execute %s. Meta4Object = %s. Node = %s. RecordSet = %ld. NumReg = %ld.", g_aPersist[ai_eStmType], idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey, ai_pRecordSet->Count ());
		M4LDB_PRINT_DEBUG (this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_DUMPTIMES_START;
	#endif
	//Fin Tiempos en ldbinsp

	return (result);
}


m4return_t ClLConn::_RealStmt (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn)
{
	m4return_t result;
	itMMpSentCache itLastSent;
	// Cuidado con esta función. Se supone que, por optimización no se graba. 
	// ¿Es eso correcto? Si es así, quitar de esta función lo que no sirva.

	m4bool_t bHasSysHint = M4_FALSE;

	ClAccess *pAccess = m_pLogAccess;
	ClCompiledMCR *pMCR = m_pLogMCR;
	m4int32_t hNode = m_hLogNode;

	ClChannel *pChannel = pAccess->GetpChannel();
	
	m_pLogonAdaptor = pChannel->GetpChannelManager()->GetLogonAdaptor();

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n--.--");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.--");
	#endif
	
	m_poStatistics = ai_poStatistics;

	m4id_t pIdRole = M4LDB_ID (pChannel->GetIDRole());
	m4id_t pIdRSM = M4LDB_ID (pChannel->GetIDRSM());
	m4id_t idT3 = M4LDB_ID (pMCR->GetChannelId());
	m4id_t idNode = M4LDB_ID (pMCR->GetNodeId(hNode));

	//Detalle de trazas fichero.
	if( pMCR != NULL )
	{
		m_pIdT3Trace = pMCR->GetChannelId();

		if( idNode != NULL )
		{
			m_pIdNodeTrace = idNode;
		}

		if( m_pLogonAdaptor != NULL && m_pLogonAdaptor->GetSessionChannelPointer() != NULL )
		{
			if( m_pLogonAdaptor->GetUsername() )
			{
				m_pIdUserTrace = m_pLogonAdaptor->GetUsername();
			}
			if( m_pLogonAdaptor->GetRole() )
			{
				m_pIdRoleTrace = m_pLogonAdaptor->GetRole();
			}				
		}
	}
	
	m4double_t dOrgType;
	pChannel->OrganizationType.Get(dOrgType);
	m4char_t cOrgType = (m4char_t)dOrgType;

	// Situamos el register con el indice del current.
	ai_pRecordSet->Register.MoveTo (ai_pRecordSet->Current.Index);

	m4double_t dLang = M4CL_LANGUAGE_DEFAULT;
	pChannel->Data_Language.Get(dLang);
	m4uint32_t lEnvLang = (m4uint32_t) dLang;

	if (lEnvLang == -1)
	{
		lEnvLang = M4CL_LANGUAGE_DEFAULT;
	}
	
	ClStringCleaner oSC(&m_stBuffer);

	m_stBuffer = idT3;
	m_stBuffer.append("_");
	m_stBuffer.append(pIdRSM);
	m_stBuffer.append(" ");


	ClHandle hItem (ai_hItem);

	m4pchar_t pAPISQL = M4LDB_PSTR (ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec ());
	
	m_stBuffer.append (pAPISQL); 

	M4LDB_INFORMER (m_stBuffer.c_str ());

	
	// Se asigna el modo de tramitatción a la caché
	m4bool_t bUseTramitation = M4_FALSE;
	m4pchar_t pcTramitation = NULL;

	pChannel->Tramitation.Get( pcTramitation );

	if( pcTramitation != NULL )
	{
		bUseTramitation = M4_TRUE ;
	}
	else
	{
		bUseTramitation = M4_FALSE ;
	}

	m4date_t	dtSessionVersion = M4LDB_MINUS_INF_DATE ;

	if( m_pLogonAdaptor != NULL )
	{
		ClChannel*	poSessionChannel = m_pLogonAdaptor->GetSessionChannelPointer() ;

		if( poSessionChannel != NULL )
		{
			ClCompiledMCR*	poSessionMCR = poSessionChannel->GetpCMCR() ;

			if( poSessionMCR != NULL )
			{
				dtSessionVersion = poSessionMCR->GetChannelVersion() ;
			}
		}
	}

	m4bool_t bHasRange = M4_FALSE;
	
	if( strnicmp( pAPISQL, "SELECT", 6 ) == 0  && ai_pRecordSet->GetCurrentBlock()->GetRangeSize() > 0 )
	{
		bHasRange = M4_TRUE;
	}

	m4bool_t bSecurityView = M4_FALSE ;

	if( strnicmp( pAPISQL, "SELECT", 6 ) == 0 )
	{
		m4int32_t hBDLChecks = pMCR->GetNodeBDLChecksItem( hNode ) ;

		if( hBDLChecks != 0 )
		{
			m4int32_t lMask = M4LDB_NUMBER( ai_pRecordSet->Item[(ClHandle) hBDLChecks].Value.Get() ) ;
			bSecurityView = ( M4LDB_NO_USE_SECURITY_VIEW & lMask ) == 0 && ( m_pLdb->UseSecurityView() == M4_TRUE || ( M4LDB_USE_SECURITY_VIEW & lMask ) != 0 ) ;
		}
	}

	StSentKey oSentKey( hNode, pMCR->GetChannelVersion(), dtSessionVersion, lEnvLang, 0, pMCR->GetChannelCsType(), 'R', pIdRole, m_stBuffer.c_str(), cOrgType, 0, 0, M4_TRUE, bUseTramitation, bHasRange, bSecurityView );

	if (ai_idRConn > 0)
	{
		oSentKey.m_idRConn = ai_idRConn;
	}

	ClLoad *pStm = (ClLoad *) m_pLdb->m_oStmtCache.FindSent (oSentKey, itLastSent, this);

	//Pedro: Tiempos en ldbinsp
	M4LDB_DUMPTIMES_DECLARE;
	M4LDB_DUMPTIMES_START;
	//Fin Pedro Tiempos.

	if (!pStm) {
		// No está, la creamos.
		ClGlobVars *pGlobVars = new ClGlobVars();
		pGlobVars->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode, ai_pRecordSet);

		pStm = new ClLoad (pGlobVars, M4_FALSE);

		m_pLdb->m_oStmtCache.InsertSent (oSentKey, itLastSent, (ClStmt *) pStm, this);

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG_DATE (this, "\n\nPrepare Real Stmt. Meta4Object = %s. Node = %s.", idT3, idNode);
			M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\n\nPrepare Real Stmt. Meta4Object = %s. Node = %s.", idT3, idNode);
		#endif

		ClMetaSentence *pMtSent = new ClMetaSentence;

		pMtSent->Create (pGlobVars, M4LDB_SELECT, M4_TRUE, M4_TRUE, bHasRange, M4_FALSE, M4_TRUE);

		eApiSqlType_t eApiSql ;

		/* Bug 0165070
		Hay que comparar la palabra completa y no sólo el caracter inicial
		*/
		if( strnicmp( pAPISQL, "SELECT", 6 ) == 0 )
		{
			eApiSql = M4LDB_APISQL_REAL_SELECT ;

			/* Bug 0065627
			Si se entra en la load se marca como cargado
			Esto vale para RealStmt
			*/
			ai_pRecordSet->GetCurrentBlock()->_SetIsLoaded (M4_TRUE);
		}
		else
		{
			eApiSql = M4LDB_APISQL_REAL_STMT ;
		}

		//Bug 0084278.
		ClAPISQL oAPISQL (eApiSql);

		result = oAPISQL.AnalizeSQL (m_stBuffer, pMtSent, M4_TRUE);

		//Bug 0076002.
		//Se gpa si es sentencia real y el item no es de registro.
		//Si el analisis Warning/Error (!= M4SUCCESS) tb deberiamos salir, pero retornando Warning o Error, dependiendo lo que retorne Analize,
		//es decir, retornariamos result.
		//Decidimos no tratar warnings!!
		if (result == M4_ERROR) {
			delete pMtSent;
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		result = pStm->Open (ai_idRConn, pMtSent);

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		result = pStm->Prepare (m_stBuffer);

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		// Bindamos los parámetros.
		result = pStm->BindParam ();

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		if (*pAPISQL == 'S') {
			// Bindamos las columnas.
			result = pStm->BindCol ();

			if (result == M4_ERROR) {
				m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
				return (M4_ERROR);
			}
		}
	}
	else {
 		pStm->GetpGlobVars ()->Set (this, ai_idRConn, lEnvLang, pIdRole, pIdRSM, pMCR, hNode);

		pStm->InitStt ();
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\nExecute Real Stmt. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\nExecute Real Stmt. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
	#endif

	//-------------------------------------------------------
	//Tenemos que diferenciar entre Ejecutar y auditar.
	//Ahora comprobamos si puede ejecutar, no comprobamos, ni ejecutamos auditoria.
	result = AnalizeRealSqlMode (pStm, ai_pRecordSet, M4_FALSE);
	if (result == M4_ERROR) {
		m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
		return (M4_ERROR);
	}
	//-------------------------------------------------------

	m4uint32_t	iTimeOut = m_pLdb->GetExecutionTimeOut() ;

	if( strnicmp( pAPISQL, "SELECT", 6 ) == 0 && iTimeOut > 0 )
	{
		m4uint32_t	iBlockTimeOut = ai_pRecordSet->GetCurrentBlock()->GetTimeOut() ;

		if( iBlockTimeOut > 0 && iBlockTimeOut < iTimeOut )
		{
			iTimeOut = iBlockTimeOut ;
		}
	}

	//Introducimos los parametros y ejecutamos la sentencia.
	result = pStm->Execute( ai_pParam, ai_numParam, NULL, 0, iTimeOut ) ;

	if( result == M4_SUCCESS && bHasRange == M4_TRUE )
	{
		ai_pRecordSet->GetCurrentBlock()->SetRangeStartNext() ;
	}

	if (result == M4_ERROR) {
		m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
		return (M4_ERROR);
	}

	if ((result == M4_SUCCESS) && 
		(*pAPISQL == 'S')) {
		m4bool_t bHasLongItem = pStm->HasLongItem ();
		
		if (bHasLongItem == M4_TRUE) {
			ai_pRecordSet->SetCheckPointMode (M4_FALSE);
		}
		
		//Obtenemos los datos.
		result = pStm->GetData ();
		
		if (bHasLongItem == M4_TRUE) {
			ai_pRecordSet->SetCheckPointMode (M4_TRUE);
		}

		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			if (m_pLdb->GetShowData ()) {
				m4int32_t lSysDebugLevel = m_lSysDebugLevel;
				m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
				M4LDB_PRINT_DEBUG (this, ai_pRecordSet);
				m_lSysDebugLevel = lSysDebugLevel;
			}
		#endif
	}

	//-------------------------------------------------------
	//Comprobamos si es posible la ejecución de la sentencia real: si hay que auditarla,...
	if (result != M4_ERROR) {

		//Aqui sí es posible que tengamos que auditarla.
		result = AnalizeRealSqlMode (pStm, ai_pRecordSet, M4_TRUE);
		if (result == M4_ERROR) {
			m_pLdb->m_oStmtCache.DeleteSent (itLastSent);
			return M4_ERROR;
		}
	}
	//-------------------------------------------------------

	m_pLdb->m_oStmtCache.DeleteSent (itLastSent);

	//Pedro: Tiempos en ldbinsp
	#ifdef _M4LDB_DEBUG
		M4LDB_DUMPTIMES_STOP;

		if( strnicmp( pAPISQL, "SELECT", 6 ) )
		{
			AuditTime( long( _endingTime - _startingTime ) / 1000 );
		}

		M4LDB_PRINT_DEBUG (this, "\n\tCalculating Time. Execute Real Stmt. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\tCalculating Time. Execute Real Stmt. Meta4Object = %s. Node = %s. RecordSet = %ld.", idT3, idNode, ai_pRecordSet->GetCurrentBlock ()->ID.BlockUniqueKey);
		M4LDB_PRINT_DEBUG (this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, M4LDB_DUMPTIMES_PRINT_TIMES);
		M4LDB_DUMPTIMES_START;
	#endif
	//Fin Tiempos en ldbinsp

	return (M4_SUCCESS);
}


m4return_t ClLConn::_BeginTransaction (ClAccessRecordSet *ai_pRecordSet)
{
	m4return_t result;

	if (m_numTrans<0) {
		m_numTrans = 0;
	}

	if (!m_numTrans) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n--.--");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.--");
		#endif

		m_bPreVal = M4_FALSE;

		m4int32_t hItem = m_pLogMCR->GetNodeBDLChecksItem (m_hLogNode);

		if (hItem) {
			m4int32_t lMask = M4LDB_NUMBER (ai_pRecordSet->Item[(ClHandle) hItem].Value.Get ());

			if (lMask & M4LDB_PREVALIDATE_FK) {
				m_bPreVal = M4_TRUE;
			}
		}

		if (!m_plsPostValidation) {
			m_plsPostValidation = new LsPStPostValidation;
		}
	

		if (!m_pvcTransAccRl) {
			m_pvcTransAccRl = new VcPClAccessRl;
			m_pvcTransAccRl->reserve(2);
		}
	
		// Esto sólo servirá mientras que no tengamos auditoria de procesos.
		m_idExecution = 0;
	
		ClActualDate (m_dDTLstUpdTrn);

		#ifdef _M4LDB_DEBUG
			M4LDB_BEGIN_DEBUG;
			m4char_t aDTLstUpdTrn[M4LDB_SIZE_TIMESTAMP + 1];
			ClDateToString (m_dDTLstUpdTrn, aDTLstUpdTrn, M4LDB_SIZE_TIMESTAMP + 1);
			M4LDB_PRINT_DEBUG (this, "\n\n#-#----------- BEGIN TRANSACTION -----------#-#");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\n#-#----------- BEGIN TRANSACTION -----------#-#");
			M4LDB_PRINT_DEBUG (this, "\n Transacion Date = %s.", aDTLstUpdTrn);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n Transacion Date = %s.", aDTLstUpdTrn);
			m_lInsInfo = 0;
			m_lUpdInfo = 0;
			m_lDelInfo = 0;
			M4LDB_END_DEBUG;
		#endif

		result = m_pConnManager->Transact(M4LDB_BEGIN_TRANS,this);
		if (result==M4_ERROR) {
			return result;
		}
	}

	m_numTrans++;

	return (M4_SUCCESS);
}


m4return_t ClLConn::_EndTransaction (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_iAction, m4bool_t &ao_bFree)
{
	m4return_t result = M4_SUCCESS;

	ao_bFree = M4_FALSE;

	if (ai_iAction != 2 && ai_iAction != 3) {
		// Las postvalidaciones no decrementan el contador.
		m_numTrans--;
	}

	if (m_numTrans<0) {
		ao_bFree = M4_TRUE;
		RollBack ();
		m_numTrans = 0;
		if (m_pmpTransCache) {
			m_pmpTransCache->erase (m_pmpTransCache->begin (), m_pmpTransCache->end());
		}
		return (M4_SUCCESS);
	}

	switch (ai_iAction) {
		case 0:
			ao_bFree = M4_TRUE;
			m_numTrans = 0;
			result = M4_ERROR;
			RollBack ();
			break;

		case 1:
			if (!m_numTrans) {
				ao_bFree = M4_TRUE;

				m_numTrans++;
				result = PersistAuditTranslation();
				m_numTrans--;

				if (result == M4_ERROR) {
					RollBack ();
				}
				else {
					// Bug 0281817 En postvalidaciones se necesita saber si se está dentro de una transacción o no
					m_numTrans++;
					result = ValidatePostValidations();
					m_numTrans--;

					if (result == M4_ERROR) {
						RollBack ();
					}
					else {
						result = Commit ();
					}
				}
			}
			else {
				return (M4_SUCCESS);
			}
			break;

		case 2:
			result = ValidatePostValidations ();

			if (result == M4_ERROR) {
				RollBack ();
			}
			break;

		case 3:
			result = ValidatePostOperations ();

			if (result == M4_ERROR) {
				RollBack ();
			}
			break;
	}

	if (m_pmpTransCache) {
		m_pmpTransCache->erase (m_pmpTransCache->begin (), m_pmpTransCache->end());
	}

	return (result);
}


m4return_t ClLConn::Commit (void)
{
	m4return_t result = M4_SUCCESS;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\n#-#----------- COMMIT -----------#-#");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\n#-#----------- COMMIT -----------#-#");
	#endif
	// Hacemos commit en la base de datos.

	ResetAuditTranslation();

	if (m_pConnManager->Transact(M4LDB_COMMIT,this)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if ((m_pvcTransAccRl) && 
		(!m_pvcTransAccRl->empty ())) {

		VersionMark oLastUpdate;

		oLastUpdate.Date = 0.0;

		// Hacemos commit persisted en cada canal.
		while (!m_pvcTransAccRl->empty ()) {

			StAccReload *pARl=m_pvcTransAccRl->back ();

			pARl->m_pAccess->GetpChannel()->CommitPersisted(oLastUpdate);
			
			// Lo saco del vector antes de llamar a la Reload, porque en caso de que hubiera una persist conectada a la load, nos metemos en un 
			// bucle infinito. De esta forma, cuando vuelva a llegar aqui en la persistencia, el vector estara vacio o tendra de nuevo el mismo Access.
			// Si tiene el mismo Access es un error funcional, porque en la persistencia nos obligan a hacer una Reload, que lanza otra persistencia, ....
			// Lo normal deberia ser que despues de hacer recargado, si hay persistencia, o no persista relmente nada o no invalide el bloque provocando la Reload.
			// Y con esto y un bizcocho...
			m_pvcTransAccRl->pop_back();
			if (pARl->m_bReload)
			{
#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\nBeginning Reload.\n");
#endif
				result = pARl->m_pAccess->GetpChannel()->ReLoad();

				if (result == M4_ERROR)
				return M4_ERROR;

#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\nReload Successfull.\n");
#endif
			}
            pARl->m_pAccess->m_pDestroy_Notifier->Dettach(this);
			delete pARl;
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n\n Statistics.\n");
			M4LDB_PRINT_DEBUG (this, "\n Number of INSERT = %ld.", m_lInsInfo);
			M4LDB_PRINT_DEBUG (this, "\n Number of UPDATE = %ld.", m_lUpdInfo);
			M4LDB_PRINT_DEBUG (this, "\n Number of DELETE = %ld.", m_lDelInfo);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\n Statistics.\n");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n Number of INSERT = %ld.", m_lInsInfo);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n Number of UPDATE = %ld.", m_lUpdInfo);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n Number of DELETE = %ld.", m_lDelInfo);
		#endif
	}

	return (result);
}


m4return_t ClLConn::RollBack (void)
{
	m4return_t result = M4_SUCCESS;

	#ifdef _M4LDB_DEBUG
	if (m_numTrans == 0){
		M4LDB_PRINT_DEBUG (this, "\n\n#-#----------- ROLLBACK -----------#-#");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, this, "\n\n#-#----------- ROLLBACK -----------#-#");
	}
	#endif

	ResetAuditTranslation();

	m4return_t result2 = m_pConnManager->Transact(M4LDB_ROLLBACK,this);

	if (result2 == M4_ERROR) {
		result = M4_ERROR;
	}


	if ((m_pvcTransAccRl) && 
		(!m_pvcTransAccRl->empty ())) {

		m4bool_t bExistsPost = M4_FALSE;

		while (!m_plsPostValidation->empty ()) {

			bExistsPost = M4_TRUE;

			StPostValidation *pPostValidation = (StPostValidation *) m_plsPostValidation->back ();

			m_plsPostValidation->pop_back ();

			delete pPostValidation;
		}

		if (bExistsPost) {
			m_pLdb->m_oStmtCache.FreeSents (this);
		}

		if (!m_bIgnoreChannel) {
				while (!m_pvcTransAccRl->empty ()) {

				StAccReload *pARl = m_pvcTransAccRl->back ();

				pARl->m_pAccess->GetpChannel ()->RollbackPersisted ();

				m_pvcTransAccRl->pop_back ();
                pARl->m_pAccess->m_pDestroy_Notifier->Dettach(this);

				delete pARl;
			}
		}
	}

	return (result);
}


m4return_t ClLConn::ValidateOnePostValidation (StPostValidation *ai_pPostValidation, m4bool_t ai_bPostValidateOperations, m4bool_t ai_bPostValidateFKs)
{

	m4return_t result;
	ClAccessBookMark oAcBookMark;


	ClPersist *pStmt = ai_pPostValidation->m_pStm;

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;

	m_pLogAccess = ai_pPostValidation->m_pAccess;
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pPostValidation->m_hNode;

	// Esto es para que, por lo menos, sepamos el nombre del nodo en el que ha ocurrido el problema.
	m4id_t idNode = M4LDB_ID (m_pLogMCR->GetNodeId (m_hLogNode));

	M4LDB_INFORMER (idNode);

	m_pLogonAdaptor = ai_pPostValidation->m_pAccess->GetpChannel()->GetpChannelManager()->GetLogonAdaptor();

	//Detalle de trazas fichero.
	ClCompiledMCR *pMCR = m_pLogMCR;

	if( pMCR != NULL )
	{
		m_pIdT3Trace = pMCR->GetChannelId();

		if( idNode != NULL )
		{
			m_pIdNodeTrace = idNode;
		}

		if( m_pLogonAdaptor != NULL && m_pLogonAdaptor->GetSessionChannelPointer() != NULL )
		{
			if( m_pLogonAdaptor->GetUsername() )
			{
				m_pIdUserTrace = m_pLogonAdaptor->GetUsername();
			}
			if( m_pLogonAdaptor->GetRole() )
			{
				m_pIdRoleTrace = m_pLogonAdaptor->GetRole();
			}				
		}
	}

	ClGlobVars *pGlobVars = pStmt->GetpGlobVars ();
	// Atención, la globvars debería de estar inicializada con los valores anteriores.

	pGlobVars->SetNumberOfAttempts (M4LDB_NULL_INDEX);

	pGlobVars->SetPostValidate (M4_FALSE);
	pGlobVars->SetPostValidateOperations (ai_bPostValidateOperations);
	pGlobVars->SetPostValidateFKs (ai_bPostValidateFKs);

	m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_pPostValidation->m_indLgcTbl];

	m4bool_t bInvRS = M4_FALSE;

	m4bool_t bRetry = M4_FALSE;

	// Guardamos un Bookmark del Access.
	oAcBookMark.GetFrom (*m_pLogAccess);

	// Colocamos el Access en el Bloque en concreto.
	ClNode * pnode = &ai_pPostValidation->m_pAccess->Node[(ClHandle) ai_pPostValidation->m_hNode];

	ClBlockReference oBR;
	ClBlock_Channel_Proxy *pBlock =0;


	// Obtenemos el bloque
	_AClComponentStore_ForwardIt *poIter = pnode->GetpNode_Data()->GetpBlockStore()->CreateIterator();

	poIter->Begin (oBR);
	while (poIter->IsNotEOF()) {
		if (oBR->ID.BlockUniqueKey == ai_pPostValidation->m_indBlock) {
			pBlock = oBR.GetpProxy ();
			break;
		}
		poIter->Next(oBR);
	}
	pnode->GetpNode_Data()->GetpBlockStore()->DestroyIterator(poIter);

	// Colocamos el Access. Si no hay bloque no pasa nada, simplemente no sacaremos infromacion sobre el path.
	if (pBlock){
		// Bug 0305523 Sólo se posiciona si el bloque sigue cargado
		if (pBlock->IsLoaded() == M4_TRUE){
			pnode->RecordSet.Follow(pBlock);
			pnode->RecordSet.Register.MoveTo(ai_pPostValidation->m_indReg);
		}
	}

	do {
		result = pStmt->GetpParams ()->Restore (&(ai_pPostValidation->m_oParam));

		if (result == M4_ERROR)
		{
			m_pLogAccess = pOldAccess;
			m_pLogMCR = pOldMCR;
			m_hLogNode = hOldNode;
			return (M4_ERROR);
		}

		result = pStmt->Execute (NULL, 0, ai_pPostValidation->m_indOperation, &bInvRS);

		bRetry = M4_FALSE;

		if (pGlobVars->IsRetry ()) {
			pGlobVars->SetIsRetry (M4_FALSE);
			if (pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX) {
				if (pGlobVars->GetNumberOfAttempts ()) {
					bRetry = M4_TRUE;
				}
				pGlobVars->SetNumberOfAttempts (pGlobVars->GetNumberOfAttempts ()-1);
			}
		}

	} while (bRetry);

	// Dejamos el Access como estaba.
	oAcBookMark.ApplyTo(*m_pLogAccess);

	if (bInvRS) {
		// Buscamos el access dentro de nuestra lista.
		for (itPVcPClAccessRl it = m_pvcTransAccRl->begin (); it!=m_pvcTransAccRl->end(); it++) {
			if ((*it)->m_pAccess == ai_pPostValidation->m_pAccess) {

				if (pBlock) {
					(*it)->SetReLoad();
					pBlock->B_SetReLoad (M4_TRUE);
				}
			}
		}
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;

	return (result);
}

m4return_t ClLConn::ValidatePostValidations (void)
{
	m4return_t result;

	if (m_pvcTransAccRl != NULL && !m_pvcTransAccRl->empty ())
	{
		size_t	iSize = m_plsPostValidation->size();

		if( iSize > 0 )
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n--.-- Post validations %d", iSize);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.-- Post validations %d", iSize);
			#endif

			// Estos son los cursores de post-validación.

			while (!m_plsPostValidation->empty ()) {

				StPostValidation *pPostValidation = (StPostValidation *) m_plsPostValidation->front ();
				m_plsPostValidation->pop_front ();

				result = ValidateOnePostValidation( pPostValidation, pPostValidation->m_bOperationValidated == M4_FALSE, M4_TRUE );
//				result = ValidateOnePostValidation( pPostValidation, M4_TRUE, M4_TRUE );

				delete pPostValidation;

				if (result == M4_ERROR) {
					m_pLdb->m_oStmtCache.FreeSents (this);
					return (M4_ERROR);
				}
			}

			m_pLdb->m_oStmtCache.FreeSents (this);
		}
	}

	return (M4_SUCCESS);
}

m4return_t ClLConn::ValidatePostOperations (void)
{
	m4return_t result = M4_ERROR;
	StPostValidation *pPostValidation = NULL;
	itLsPStPostValidation it;

	if (m_pvcTransAccRl != NULL && !m_pvcTransAccRl->empty ())
	{
		// Estos son los cursores de post-validación.

		size_t	iLastSize = 0;
		size_t	iSize = m_plsPostValidation->size();

		while( iSize > iLastSize )
		{
			StPostValidation**	ppoPostValidations = new StPostValidation*[ iSize ] ;

			it = m_plsPostValidation->begin();

			size_t	iLength = 0;
			while( it != m_plsPostValidation->end() )
			{
				pPostValidation = *it;

				if( pPostValidation->m_bOperationValidated == M4_FALSE )
				{
					ppoPostValidations[ iLength++ ] = pPostValidation;
				}
				it++;
			}

			if( iLength > 0 )
			{
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (this, "\n--.-- Interal Operations %d", iLength);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, this, "\n--.-- Interal Operations %d", iLength);
				#endif

				for( size_t i = 0 ; i < iLength ; i++ )
				{
					StPostValidation *pPostValidation = ppoPostValidations[ i ];
					result = ValidateOnePostValidation( pPostValidation, M4_TRUE, M4_FALSE );
					pPostValidation->m_bOperationValidated = M4_TRUE;

					if( result == M4_ERROR )
					{
						return (M4_ERROR);
					}
				}
			}

			delete [] ppoPostValidations;

			iLastSize = iSize;
			iSize = m_plsPostValidation->size();
		}
	}

	return (M4_SUCCESS);
}

m4return_t ClLConn::ExecuteSQL (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4bool_t ai_bReal, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t ai_lMask)
{
	m4return_t result;
	m4bool_t bStopExecution = M4_TRUE;

	m4VariantType vtAPISQL;
	result = ai_pRecordSet->Current.Item[(ClHandle) ai_hItem].Value.GetWithOutSec (vtAPISQL);
	
	if (result == M4_SUCCESS && vtAPISQL.Type == M4CL_CPP_TYPE_STRING_VAR && vtAPISQL.Data.PointerChar != NULL && *(vtAPISQL.Data.PointerChar) != '\0')
	{
		m4pcchar_t pAPISQL = M4LDB_PCSTR (vtAPISQL.Data.PointerChar);

		ClAccess		*pOldAccess = m_pLogAccess;
		ClCompiledMCR	*pOldMCR = m_pLogMCR;
		m4int32_t		hOldNode = m_hLogNode;

		m_pLogAccess = ai_pRecordSet->GetpAccess();
		m_pLogMCR = m_pLogAccess->GetpCMCR();
		m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();

		result = _CheckUserSQL (vtAPISQL.IsUser, pAPISQL);

		m_pLogAccess = pOldAccess;
		m_pLogMCR = pOldMCR;
		m_hLogNode = hOldNode;

		if (result == M4_SUCCESS)
		{
			if (!strncmp (pAPISQL, "INTERNAL", 8))
			{
				result = m_pLdb->_ExecuteInternalCommand (pAPISQL+9);
			}
			else
			{
				if (ai_bReal)
				{
					result = RealStmt (ai_pRecordSet, ai_poStatistics, ai_hItem, ai_pParam, ai_numParam, ai_idRConn);
				}
				else
				{
					switch (*pAPISQL)
					{
						case 'f':
						case 'F':
						case 's':
						case 'S':

							/* Bug
							Si hay carga parcial no se puede cargar con ExecuteSQL
							*/
							if( ai_pRecordSet->GetpNodeDef()->NumRows() > 0 )
							{
								DUMP_LDBINSP_ERROR( this, M4_CH_LDB_EXECUTESQL_IN_PARTIAL ) ;
								return( M4_ERROR ) ;
							}

							result = Load (ai_pRecordSet, ai_poStatistics, ai_hItem, ai_pParam, ai_numParam, ai_idRConn );
							break;

						case 'd':
						case 'D':
							result = Persist (ai_pRecordSet, M4LDB_DELETE, ai_poStatistics, bStopExecution,ai_hItem, ai_pParam, ai_numParam, ai_idRConn, ai_lMask);
							break;

						case 'u':
						case 'U':
							result = Persist (ai_pRecordSet, M4LDB_UPDATE, ai_poStatistics, bStopExecution,ai_hItem, ai_pParam, ai_numParam, ai_idRConn, ai_lMask);
							break;

						case 'i':
						case 'I':
							result = Persist (ai_pRecordSet, M4LDB_INSERT, ai_poStatistics, bStopExecution,ai_hItem, ai_pParam, ai_numParam, ai_idRConn, ai_lMask);
							break;

						default:
							DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, pAPISQL);
							return (M4_ERROR);
					}
				}
			}
		}

		return (result);
	}


	DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_APISQL, "<null>");
	return (M4_ERROR);
}


m4return_t ClLConn::DisplayError (m4return_t ai_retcode, ClDBConnection * ai_Conn)
{
	return m_pConnManager->DisplayError(ai_retcode,ai_Conn,this);
}


m4return_t ClLConn::SetClosingDate (ClMetaSentence *ai_pMtSent ,ClAccessRecordSet *ai_pRecordSet)
{
	m4int32_t hItem = m_pLogMCR->GetNodeClosingDateItem (m_hLogNode);

	if (!hItem)	{
		return (M4_SUCCESS);
	}
	
	m4id_t idObject = m_pLogMCR->GetNodeWriteObjectId (m_hLogNode);
	
	if (!*idObject)	{
		// Si no existe un write object, cogemos la tabla base
		idObject = ai_pMtSent->GetIdObjectBasis ();
	}
	
	m4return_t result = LoadDictionaryChannel (idObject, NULL, M4_TRUE);

	if (result != M4_SUCCESS)
	{
		return (M4_ERROR);
	}
	
	m4date_t dClosingDate = 0;
		
	m4id_t idClsDate = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_CLS_DATE].Value.GetWithOutSec ());

	if ((*idClsDate) && 
		(m_paRS[M4LDB_SYS_DICT_CLS_DATE]->Register.Count ())) {
		dClosingDate = M4LDB_DOUBLE (m_paRS[M4LDB_SYS_DICT_CLS_DATE]->Register[0].Item[M4LDB_ITEM_SYS_DICT_CLS_DATE_DT_CLOSING].Value.GetWithOutSec ());
	}
	
	m4VariantType vTemp;
	vTemp.Type=M4CL_CPP_TYPE_DATE;
	vTemp.Data.DoubleData = (dClosingDate) ? dClosingDate : M4LDB_MINUS_INF_DATE;
	ai_pRecordSet->Current.Item[(ClItemIndex)hItem].Value.Set(vTemp);
	
	return (M4_SUCCESS);
}

m4return_t ClLConn::GetSysParam (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_ItemIndex, m4VariantType *&ao_pParam, m4int32_t &ao_numParam)
{
	m4char_t aArgs[M4LDB_SIZE_LONG];
	m4int32_t numParam = 0;
	m4int32_t lLength=0;
	m4int32_t lMax = MAX_NUMBER_OF_PARAMETERS;
	
	m4VariantType vtData;
	m4pchar_t pLongBuf = 0,pBuf =0,pBeginToken=0,pEndToken=0;

	ai_pRecordSet->Register.Item[(ClItemIndex)ai_ItemIndex].Value.GetWithOutSec (vtData);

	if ((vtData.Type != M4CL_CPP_TYPE_STRING) && 
		(vtData.Type != M4CL_CPP_TYPE_STRING_VAR) && 
		(vtData.Type != M4CL_CPP_TYPE_STRING_POOL)) {
		return (M4_SUCCESS);
	}

	// Se supone que el variant sera de tipo string
	pBeginToken = vtData.Data.PointerChar;

/*		#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\tSysParam = %s.", pBeginToken );
	#endif*/

	//Bug 0071471
	//Dada una syssentence, tenemos un sysparam. Mientras parseamos el sysparam,
	//permitimos que los argumentos sean nulos. Si el último parametro es nulo
	//debe analizarse como tal (ver bug 0068053).
	//Lo que ocurre es que si el sysparam no tiene datos, se tiene que descartar.
	//El motivo es que si no la descartamos tendremos el siguiente resultado:
	//Suponiendo que sysparam de la primera syssentence es nulo y de la segunda es p1|p2
	//tendremos null|p1|p2.
	//Si el sentence espera dos argumentos se va a construir la sentencia con null|p1.
	//Pero esto es incorrecto, se está tomando datos del primer param y en realidad
	//son dos parámetros del segundo param.
	//Solución: Si un sysparam no tiene ningún parámetro, lo descartamos en la construccion
	//global. Si la cadena que se nos pasa ya está vacía, es que el sysparam que se nos pasa
	//está vacío.
	if ((pBeginToken) && (*pBeginToken==0))
	{
		return M4_SUCCESS;
	}

	while (pBeginToken){
		
		if (*pBeginToken==0){
			//<<< Bug 0068053.
			//Comentado el return. Damos información y continuamos.
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n\tLast parameter in SysParam is Null.");
			#endif			
			//return M4_SUCCESS;
			//>>>>
		}
		
		if (ao_numParam >= lMax-1) {
			lMax = ao_numParam + MAX_NUMBER_OF_PARAMETERS;
			m4VariantType *pParamAux = new m4VariantType [lMax];
			for (m4int32_t ind = 0; ind < ao_numParam; ind++) {
				pParamAux[ind].SetString (ao_pParam[ind].Data.PointerChar);
			}
			delete [] ao_pParam;
			ao_pParam = pParamAux;
		}

		pEndToken = strchr(pBeginToken,'|');

		if (pEndToken){
			lLength = pEndToken-pBeginToken;
		}else{
			lLength = strlen(pBeginToken);
		}
		
		if (lLength>=M4LDB_SIZE_LONG){
			pLongBuf = new m4char_t [lLength+1];
			pBuf = pLongBuf ;
		}
		else{
			pBuf = aArgs;
		}

		memcpy(pBuf,pBeginToken,lLength);
		pBuf[lLength]=0;

		ao_pParam[ao_numParam].SetString (pBuf);

		if (pEndToken){
			pBeginToken = pEndToken+1;
		}
		else{
			pBeginToken =0;
		}
	
		++ao_numParam;

		if (pLongBuf){
			delete [] pLongBuf;
			pLongBuf=0;
		}
	}		

	return (M4_SUCCESS);
}

m4bool_t ClLConn::_CheckOperation(ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType,m4int32_t ai_hItem)
{
	if (ai_hItem != 0) {
		return M4_TRUE;
	}
	m4bool_t bOperation = M4_FALSE;
	m4bool_t bModified = M4_FALSE;

	ClAccess *pAccess = ai_pRecordSet->GetpAccess ();

	// Miramos si el bloque esté modificado.
	// Si el bloque no está modificado no miramos los registros.
	if (ai_pRecordSet->IsValid())
	{
		if (ai_pRecordSet->Operation.IsModifiedByDDBB())
		{
			// Insertamos el Access. Habrá que hacer CommitPersisted.
			bModified = M4_TRUE;

			if (_FindPersistedAccess(pAccess)==M4_FALSE)
			{
				m_pvcTransAccRl->push_back( new StAccReload(pAccess) );
                pAccess->m_pDestroy_Notifier->Attach(this);
			}
		}
	}

	if (bModified == M4_FALSE){
		return M4_FALSE;
	}
	
	ClOperation_RecordSet *pB_Operation = &ai_pRecordSet->Operation;

	switch (ai_eStmType) {
		
	case M4LDB_INSERT:
		return _CheckOperationInsert(pB_Operation,&ai_pRecordSet->Register);
		break;
	case M4LDB_UPDATE:
		return _CheckOperationUpdate(pB_Operation,&ai_pRecordSet->Register);
		break;
	case M4LDB_DELETE:
		return _CheckOperationDelete(pB_Operation,&ai_pRecordSet->Register);
		break;
	default:
		return  M4_FALSE;
		break;
	}
}

m4bool_t ClLConn::_CheckOperationInsert(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg)
{
	if (ai_pBOper->IsNewByDDBB())
	{
		// Puede ser New And Deleted. 
		// Me lo recorro en cualquier caso, ( pueden haber insertado registros y al hacer Destory no se refresca la marca ).
		// Me lo recorro por si de verdad hubiera un registro New.
		// Los NewAndDeleted los marco SOLO en el Delete.
		ClOperation_Register *pR_Operation= &ai_pRg->Operation;
		// Empiezo por el final porque los añadidos suelen estar ahi.
		for (ai_pRg->End(); ai_pRg->Index != M4DM_EOF_INDEX; ai_pRg->Prev())
		{
			// Si hay uno nuevo y no borrado.
			// Si hubiera un NewandDeleted no me dice que es new a nivel de registro.
			if (pR_Operation->IsNewByDDBB())
			{
				return M4_TRUE;
			}
		}
	}
	return M4_FALSE;
}

m4bool_t ClLConn::_CheckOperationUpdate(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg)
{
	if (ai_pBOper->IsUpdatedByDDBB())
	{
		// Me lo tengo que recorrer porque si el nodo tiene solo items de bloque, y han insertado un registro, tengo el vrb de bloque
		// marcado como modificado, pero el vrb de registro esta insertado con lo que no encuentro registro para updatear.
		
		// Puede ser Updated And Deleted.
		ClOperation_Register *pR_Operation= &ai_pRg->Operation;
		for( ai_pRg->Begin(); ai_pRg->Index != M4DM_EOF_INDEX; ai_pRg->Next() )
		{
			// Si hay uno nuevo y no borrado.
			if (pR_Operation->IsUpdatedByDDBB() && !pR_Operation->IsDeletedByDDBB() && !pR_Operation->IsNewByDDBB())
			{
				return M4_TRUE;
			}
		}
	}
	return M4_FALSE;
}

m4bool_t ClLConn::_CheckOperationDelete(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg)
{
	m4bool_t bOperation = M4_FALSE;

	if (ai_pBOper->IsDeletedByDDBB())
	{
		// Si no hay marcas de Update y New en el bloque, adelante.
		// Puede haber NewAndDeleted. Me lo recorro para marcarlos.
		ClOperation_Register *pR_Operation= &ai_pRg->Operation;
		for (ai_pRg->Begin(); ai_pRg->Index != M4DM_EOF_INDEX; ai_pRg->Next())
		{
			if (pR_Operation->IsDeletedByDDBB())
			{
				if (pR_Operation->IsNewAndDeleted())
				{
					// Es NewAndDeleted. Lo marco. No quiere decir que tenga que hacer ninguna operacion.
					ai_pRg->SetFlagPersisted (M4_TRUE);
				}
				else
				{
					// Aunque haya encontrado uno, tengo que seguir para ver si hay NewAndDeleted
					bOperation = M4_TRUE;
				}
			}
		}
	}
	return bOperation;
}
	
m4bool_t ClLConn::_FindPersistedAccess(ClAccess	*ai_pAccess)
{
	for (itPVcPClAccessRl it = m_pvcTransAccRl->begin (); it!=m_pvcTransAccRl->end(); it++) {
		if ((*it)->m_pAccess == ai_pAccess) {
			return M4_TRUE;
		}
	}
	return M4_FALSE;
}

m4return_t ClLConn::_GetSysHints ( VcSysHint & ao_VcSysHint, ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode )
{
	if ((ai_pMCR == NULL) || (ai_pRecordSet == NULL) ) {
		return M4_ERROR;
	}
	//Se obtienen los hints.
	m4int32_t iNumSysHint = ai_pMCR->GetNodeNumberOfSysHints (ai_hNode);
	
	for (m4int16_t i=0; i<iNumSysHint; i++) {
		ClHandle hItem = ai_pMCR->GetNodeSysHintHandleByPosition (ai_hNode, i);		
		if (hItem) {
			string sSysHint("");
			m4pcchar_t pAPISQLStm = M4LDB_PCSTR (ai_pRecordSet->Current.Item[hItem].Value.GetWithOutSec ());
			sSysHint.append(pAPISQLStm);
			
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n\t SysHint[%ld] = %s .", i, (sSysHint.size() > 0) ? sSysHint.c_str() : "<null>");
			#endif

			if (sSysHint.size() > 0) {
				//Habemus hint.
				//Se analiza extrayendo la información.
				ClSysHint * pClSysHint = NULL;
				if (_CreateSysHint( this, sSysHint, pClSysHint ) == M4_ERROR) {
					return M4_ERROR;
				}

				//Se ha creado un hint, a la butxaca!.
				ao_VcSysHint.push_back(pClSysHint);
			}			
		}
	}

	return M4_SUCCESS;
}


m4return_t ClLConn::SysLoad( ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_indSelect, m4int32_t ai_lSysDebugLevel )
{
	m4return_t result = M4_ERROR;

	/* Bug 0065627
	Si se entra en la load se marca como cargado
	Esto vale para SysLoad
	*/
	ai_pRecordSet->GetCurrentBlock()->_SetIsLoaded( M4_TRUE );

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	m_pLogAccess = ai_pRecordSet->GetpAccess();
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	m_lSysDebugLevel = ai_lSysDebugLevel;
	m_pInformer = NULL;

	m4bool_t bCopy = M4_FALSE;
	string stCopy;
	
	if( *(m_stBuffer.c_str()) != 0 )
	{
		// Hay que hacer copia.
		bCopy = M4_TRUE;
		stCopy = m_stBuffer;
	}

	result = _SysLoad( ai_pRecordSet, ai_poStatistics, ai_indSelect );

	if( bCopy == M4_TRUE )
	{
		m_stBuffer = stCopy;	//Restauramos la copia.
	}

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4return_t ClLConn::Load( ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData )
{
	//Incrementamos el contador de referencias.
	ClLConnReenterControl oControl(this);
	
	m4return_t result = M4_ERROR;

	/* Bug 0065627
	Si se entra en la load se marca como cargado
	Esto vale para Load, ExecuteSQL, ExecuteDirectSQL y algún ExecuteRealSQL
	*/
	ai_pRecordSet->GetCurrentBlock()->_SetIsLoaded( M4_TRUE );

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	m_pLogAccess = ai_pRecordSet->GetpAccess();
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	m_pInformer = NULL;

	if( m_idLConn < M4LDB_NULL_INDEX )
	{
		// Para la Versión 2.8....
		ai_idRConn = m_idLConn;
	}

	m_pLogonAdaptor = NULL;

	m4bool_t bCopy = M4_FALSE;
	string stCopy;
	
	if( *(m_stBuffer.c_str()) != 0 )
	{
		// Hay que hacer copia.
		bCopy = M4_TRUE;
		stCopy = m_stBuffer;
	}

	result = _Load( ai_pRecordSet, ai_poStatistics, ai_hItem, ai_pParam, ai_numParam, ai_idRConn, ao_iNumData, ai_iFirstData );

	if( bCopy == M4_TRUE )
	{
		m_stBuffer = stCopy;	//Restauramos la copia.
	}

	//Lo reseteamos al salir.
	m_pLogonAdaptor = NULL;
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;
	m_pIdRoleTrace = NULL;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4return_t ClLConn::Persist( ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t ai_lMask )
{
	//Incrementamos el contador de referencias.
	ClLConnReenterControl oControl(this);

	m4return_t result = M4_ERROR;

	if( m_numTrans < 1 )
	{
		DUMP_LDBINSP_ERROR( this, M4_CH_LDB_TRANSACTION_IS_NEEDED );
		return( M4_ERROR );
	}

	// Paramos la ejecución sólo en el caso de que tengamos activada la marca de prevalidaciones.
	// Con esto evitamos que se haga Rollback.
	if( m_bPreVal )
	{
		ao_bStopExecution = M4_FALSE;
	}
	else
	{
		ao_bStopExecution = M4_TRUE;
	}

	m4bool_t bOperation = _CheckOperation( ai_pRecordSet, ai_eStmType, ai_hItem );

	if (!bOperation)
	{
		#ifdef _M4LDB_DEBUG
			ClAccess		*pOldAccess = m_pLogAccess;
			m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
			m_pLogAccess = ai_pRecordSet->GetpAccess();
			m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
			M4LDB_PRINT_DEBUG( this, "\n\n%s. Meta4Object = %s. Node = %s. RecordSet = %ld. NumReg = %ld.", g_aPersist[ai_eStmType], ai_pRecordSet->GetpAccess()->GetpCMCR()->GetChannelId(), ai_pRecordSet->GetpAccess()->GetpCMCR ()->GetNodeId(ai_pRecordSet->GetpNodeDef()->GetHandle()), ai_pRecordSet->GetCurrentBlock()->ID.BlockUniqueKey, ai_pRecordSet->Count() );
			m_pLogAccess = pOldAccess;
			m_lSysDebugLevel = lOldSysDebugLevel;
		#endif
		return( M4_SUCCESS );
	}

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	m_pLogAccess = ai_pRecordSet->GetpAccess();
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	m_pInformer = NULL;

	if( m_idLConn < M4LDB_NULL_INDEX )
	{
		// Para la Versión 2.8....
		ai_idRConn = m_idLConn;
	}

	m_pLogonAdaptor = NULL;

	m4bool_t bCopy = M4_FALSE;
	string stCopy;
	
	if( *(m_stBuffer.c_str()) != 0 )
	{
		// Hay que hacer copia.
		bCopy = M4_TRUE;
		stCopy = m_stBuffer;
	}

	result = _Persist( ai_pRecordSet, ai_eStmType, ai_poStatistics, ai_hItem, ai_pParam, ai_numParam, ai_idRConn, ai_lMask );

	if( bCopy == M4_TRUE )
	{
		m_stBuffer = stCopy;	//Restauramos la copia.
	}

	//Lo reseteamos al salir.
	m_pLogonAdaptor = NULL;
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;
	m_pIdRoleTrace = NULL;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4return_t ClLConn::RealStmt( ClAccessRecordSet *ai_pRecordSet, ClVMStatistics*ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn )
{
	//Incrementamos el contador de referencias.
	ClLConnReenterControl oControl(this);

	m4return_t result = M4_ERROR;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		 m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	m_pLogAccess = ai_pRecordSet->GetpAccess();
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	m_pInformer = NULL;

	if( m_idLConn < M4LDB_NULL_INDEX )
	{
		// Para la Versión 2.8....
		ai_idRConn = m_idLConn;
	}

	m_pLogonAdaptor = NULL;

	m4bool_t bCopy = M4_FALSE;
	string stCopy;
	
	if( *(m_stBuffer.c_str()) != 0 )
	{
		// Hay que hacer copia.
		bCopy = M4_TRUE;
		stCopy = m_stBuffer;
	}

	result = _RealStmt( ai_pRecordSet, ai_poStatistics, ai_hItem, ai_pParam, ai_numParam, ai_idRConn );

	if( bCopy == M4_TRUE )
	{
		m_stBuffer = stCopy;	//Restauramos la copia.
	}

	//Lo reseteamos al salir.
	m_pLogonAdaptor = NULL;
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;
	m_pIdRoleTrace = NULL;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4return_t ClLConn::BeginTransaction( ClAccessRecordSet *ai_pRecordSet )
{
	//Realmente no es necesario el control de referencias en una begintransaction. El motivo es que
	//la auditoria se ejecuta cuando el contador de transacciones llega a cero, es decir, despues de un
	//inicio de transaccion nunca se ejecutara auditoria pq el contador es > 0.	
	//Incrementamos el contador de referencias.
	ClLConnReenterControl oControl(this);

	m4return_t result = M4_ERROR;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	m_pLogAccess = ai_pRecordSet->GetpAccess();
	m_pLogMCR = m_pLogAccess->GetpCMCR();
	m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	m_pInformer = NULL;

	m_pLogonAdaptor = NULL;

	result = _BeginTransaction( ai_pRecordSet );

	//Lo reseteamos al salir.
	m_pLogonAdaptor = NULL;
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;
	m_pIdRoleTrace = NULL;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4return_t ClLConn::EndTransaction( ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_iAction, m4bool_t &ao_bFree )
{
	//Incrementamos el contador de referencias.
	ClLConnReenterControl oControl(this);

	m4return_t result = M4_ERROR;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Attach();
	}

	ClAccess		*pOldAccess = m_pLogAccess;
	ClCompiledMCR	*pOldMCR = m_pLogMCR;
	m4int32_t		hOldNode = m_hLogNode;
	m4int32_t		lOldSysDebugLevel = m_lSysDebugLevel;
	m4pcchar_t		pOldInformer = m_pInformer;

	if( ai_pRecordSet != NULL )
	{
		m_pLogAccess = ai_pRecordSet->GetpAccess();
		m_pLogMCR = m_pLogAccess->GetpCMCR();
		m_hLogNode = ai_pRecordSet->GetpNodeDef()->GetHandle();
	}
	else
	{
		m_pLogAccess = NULL;
		m_pLogMCR = NULL;
		m_hLogNode = 0;
	}
	m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	m_pInformer = NULL;

	m_pLogonAdaptor = NULL;

	result = _EndTransaction( ai_pRecordSet, ai_iAction, ao_bFree );

	//Lo reseteamos al salir.
	m_pLogonAdaptor = NULL;
	m_pIdT3Trace = NULL;
	m_pIdNodeTrace = NULL;
	m_pIdUserTrace = NULL;
	m_pIdRoleTrace = NULL;

	if( m_pLdb->GetContext()->MustAttachConnection( m_idLConn ) )
	{
		m_pLdb->Detach();
	}

	m_pLogAccess = pOldAccess;
	m_pLogMCR = pOldMCR;
	m_hLogNode = hOldNode;
	m_lSysDebugLevel = lOldSysDebugLevel;
	m_pInformer = pOldInformer;

	return( result );
}


m4bool_t ClLConn::FindStmtInLsPostVal (ClStmt *ai_pStmt)
{
	ClStmt *poStmt;
	ClGlobVars *poGlobVars;

	if (m_plsPostValidation)
	{
		poGlobVars = ai_pStmt->GetpGlobVars();

		for (itLsPStPostValidation it=m_plsPostValidation->begin (); it!=m_plsPostValidation->end (); it++)
		{
			/*
			Bug 0136578
			No sólo basta comprobar que la sentencia está en la lista a postvalidar.
			También hay que ver que no haya otra sentencia que comparta globvars, que es lo que se borra
			al hacer un FreeSent.
			Se asume que esta función sólo se usa para comprobar si se puede o no borrar una sentencia
			en el FreeSent.
			*/
			poStmt = (*it)->m_pStm;

			if (poStmt == ai_pStmt || poStmt->GetpGlobVars() == poGlobVars)
			{
				return (M4_TRUE);
			}
		}
	}

	return (M4_FALSE);
}


/*
Obtiene el nombre de un nivel de organización.
*/
m4pcchar_t ClLConn::FindOrgLevelName( m4int32_t ai_iLevel )
{

	m4pcchar_t		pccResult = NULL ;
	m4int32_t		iIndex = 0 ;
	m4uint8_t		iLanguage = M4CL_LANGUAGE_DEFAULT ;
	m4uint8_t		iDefaultLanguage = M4CL_LANGUAGE_DEFAULT ;
	m4double_t		dValue = M4CL_LANGUAGE_DEFAULT ;
	ClItemIndex		iName ;


	m_pLogAccess->GetpChannel()->Data_Language.Get( dValue ) ;
	iLanguage = (m4uint8_t) dValue ;

	// Primero se busca como dinámico
	iIndex = FindIndexByIds( M4LDB_SYS_DICT_ORG_LEVELS_TRANS, ai_iLevel, iLanguage ) ;

	if( iIndex != M4LDB_NULL_INDEX )
	{
		pccResult = M4LDB_ID( m_paRS[ M4LDB_SYS_DICT_ORG_LEVELS_TRANS ]->Register.Item[ M4LDB_ITEM_SYS_DICT_ORG_LEVELS_TRANS_DESC ].Value.GetWithOutSec() ) ;

		if( pccResult != NULL && *pccResult != '\0' )
		{
			return( pccResult ) ;
		}
	}

	// Luego como estático en el lenguaje que se pide
	iIndex = FindIndexById( M4LDB_SYS_DICT_ORG_LEVELS, ai_iLevel ) ;

	if( iIndex == M4LDB_NULL_INDEX )
	{
		return( "Undefined level" ) ;
	}

	m_paRS[ M4LDB_SYS_DICT_ORG_LEVELS ]->Register[ iIndex ] ;

	if( iLanguage < M4CL_LANGUAGE_NUMBER )
	{
		iName = ClItemIndex( M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC + g_aiReducedLanguagesArray[ iLanguage ] ) ;
		pccResult = M4LDB_ID( m_paRS[ M4LDB_SYS_DICT_ORG_LEVELS ]->Register.Item[ iName ].Value.GetWithOutSec() ) ;

		if( pccResult != NULL && *pccResult != '\0' )
		{
			return( pccResult ) ;
		}
	}

	// Luego como estático en el lenguaje por defecto
	iDefaultLanguage = m_pLdb->GetDefaultLanguage() ;

	if( iDefaultLanguage != iLanguage && iDefaultLanguage < M4CL_LANGUAGE_NUMBER )
	{
		iName = ClItemIndex( M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC + g_aiReducedLanguagesArray[ iDefaultLanguage ] ) ;
		pccResult = M4LDB_ID( m_paRS[ M4LDB_SYS_DICT_ORG_LEVELS ]->Register.Item[ iName ].Value.GetWithOutSec() ) ;

		if( pccResult != NULL && *pccResult != '\0' )
		{
			return( pccResult ) ;
		}
	}

	return( "Undefined name" ) ;
}


/*
Obtiene el nivel de una organización.
Si no existe se recarga el árbol de sociedades y si no existe se devuelve el mínimo
*/
m4int32_t ClLConn::FindOrgLevel( m4pcchar_t ai_pccOrganization )
{

    m4return_t		iResult = M4_ERROR ;
    m4int32_t		iRegister = -1 ;
	m4int32_t		iLevel = 0 ;
    m4VariantType	avArguments[ 6 ] ;


	/* Bug 0155230
	Si la organización es nula no se intenta encontrar el nivel
	*/
	if( ai_pccOrganization == NULL )
	{
		return( m_pLdb->GetMinOrgLevel() ) ;
	}

	avArguments[ 0 ].SetDouble( M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_ORGANIZATION ) ;
	avArguments[ 1 ].SetStringRef( (const m4pchar_t) ai_pccOrganization ) ;
	avArguments[ 2 ].SetDouble( M4CL_EQUAL_FUNCTION ) ;

	avArguments[ 3 ].SetDouble( M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_PAR_ORGANIZATION ) ;
	avArguments[ 4 ].SetStringRef( (const m4pchar_t) ai_pccOrganization ) ;
	avArguments[ 5 ].SetDouble( M4CL_EQUAL_FUNCTION ) ;


	/* Bug 0198694
	Se utiliza el mismo mutex de carga del diccionario para evitar problemas de multithread
	*/
	ClMutBlock oMutexBlock( m_pLdb->GetContext()->m_poMutexOrgTree ) ;

	iResult = m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Register.FindSec( avArguments, 6, &iRegister ) ;

	if( iResult != M4_SUCCESS )
	{
		return( m_pLdb->GetMinOrgLevel() ) ;
	}
	
	if( iRegister < 0 )
	{
		// Si no se encuentra la sociedad se recarga el árbol de sociedades y se vuelve a buscar
		iResult = m_pLdb->RefreshOrgTree() ;

		if( iResult != M4_SUCCESS )
		{
			return( m_pLdb->GetMinOrgLevel() ) ;
		}

		iResult = m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Register.FindSec( avArguments, 6, &iRegister ) ;

		if( iResult != M4_SUCCESS || iRegister < 0 )
		{
			return( m_pLdb->GetMinOrgLevel() ) ;
		}
	}

	iLevel = M4LDB_NUMBER( m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Register[ iRegister ].Item[ M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_LEVEL ].Value.GetWithOutSec() ) ;

	return( iLevel ) ;
}


/*
Obtiene la organización padre de otra para un nivel en concreto.
Hace la gestión de errores si no se encuentra o está duplicada.
*/
m4pcchar_t ClLConn::FindParentOrg( m4pcchar_t ai_pccOrganization, m4int32_t ai_iLevel, m4bool_t ai_bDumpError )
{

	m4pcchar_t		pccResult = NULL ;
    m4return_t		iResult = M4_ERROR ;
	m4uint8_t		iIsDuplicated = 0 ;
	m4int32_t		iIndex = M4LDB_NULL_INDEX ;
	m4int32_t		iLevel = 1 ;
    m4int32_t		iRegister = -1 ;
	m4pcchar_t		pccLevel1 = NULL ;
	m4pcchar_t		pccLevel2 = NULL ;
    m4VariantType	avArguments[ 6 ] ;


	/* Bug 0155230
	Si la organización es nula no se intenta encontrar la padre
	*/
	if( ai_pccOrganization == NULL )
	{
		return( NULL ) ;
	}

	/* Bug 0198694
	Se utiliza el mismo mutex de carga del diccionario para evitar problemas de multithread
	*/
	ClMutBlock oMutexBlock( m_pLdb->GetContext()->m_poMutexOrgTree ) ;

	iIndex = FindIndexByIds( M4LDB_SYS_DICT_ORG_TREE, ai_pccOrganization, ai_iLevel ) ;

	if( iIndex == M4LDB_NULL_INDEX )
	{
		// Si no se encuentra la sociedad se recarga el árbol de sociedades y se vuelve a buscar
		avArguments[ 0 ].SetDouble( M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_ORGANIZATION ) ;
		avArguments[ 1 ].SetStringRef( (const m4pchar_t) ai_pccOrganization ) ;
		avArguments[ 2 ].SetDouble( M4CL_EQUAL_FUNCTION ) ;

		avArguments[ 3 ].SetDouble( M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_PAR_ORGANIZATION ) ;
		avArguments[ 4 ].SetStringRef( (const m4pchar_t) ai_pccOrganization ) ;
		avArguments[ 5 ].SetDouble( M4CL_EQUAL_FUNCTION ) ;

		iResult = m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Register.FindSec( avArguments, 6, &iRegister ) ;

		/* Bug 0198695
		Se verifica que realmente la sociedad no está
		*/
		if( iResult == M4_SUCCESS && iRegister < 0 )
		{
			iResult = m_pLdb->RefreshOrgTree() ;

			if( iResult == M4_SUCCESS )
			{
				iIndex = FindIndexByIds( M4LDB_SYS_DICT_ORG_TREE, ai_pccOrganization, ai_iLevel ) ;
			}
		}
	}

	if( iIndex == M4LDB_NULL_INDEX )
	{
		if( ai_bDumpError == M4_TRUE )
		{
			pccLevel1 = FindOrgLevelName( ai_iLevel ) ;
			pccLevel2 = FindOrgLevelName( iLevel ) ;

			/* Bug 0153425
			Si el nivel es inferior se vuelca un mensaje de error diferente
			*/
			if( iLevel > ai_iLevel )
			{
				DUMP_LDBINSP_ERRORF( this, M4_CH_LDB_NOT_SUCH_ORG_IN_LOWER_LEVEL, LOG_CAT_AND( ai_iLevel, pccLevel1 ) << ai_pccOrganization << LOG_CAT_AND( iLevel, pccLevel2 ) ) ;
			}
			else
			{
				DUMP_LDBINSP_ERRORF( this, M4_CH_LDB_NOT_SUCH_ORG_IN_UPPER_LEVEL, LOG_CAT_AND( ai_iLevel, pccLevel1 ) << ai_pccOrganization << LOG_CAT_AND( iLevel, pccLevel2 ) ) ;
			}
		}
		return( NULL ) ;
	}


	// Chequear que no haya duplicados
	m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Current[ iIndex ] ;

	iIsDuplicated = M4LDB_NUMBER( m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Current.Item[ M4LDB_ITEM_SYS_DICT_ORG_TREE_IS_DUPLICATED ].Value.GetWithOutSec() ) ;

	if( iIsDuplicated == 1 )
	{
		if( ai_bDumpError == M4_TRUE )
		{
			pccLevel1 = FindOrgLevelName( ai_iLevel ) ;
			DUMP_LDBINSP_ERRORF( this, M4_CH_LDB_REPEATED_ORG_IN_LEVEL, ai_pccOrganization << LOG_CAT_AND( ai_iLevel, pccLevel1 ) ) ;
		}
		return( NULL ) ;
	}

	pccResult = M4LDB_ID( m_paRS[ M4LDB_SYS_DICT_ORG_TREE ]->Current.Item[ M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_PAR_ORGANIZATION ].Value.GetWithOutSec() ) ;

	return( pccResult ) ;
}


/*
Obtiene Si un lenguaje es dinámico o no.
*/
m4bool_t ClLConn::IsDynamicLanguage( m4uint32_t ai_iLanguage )
{
	
	m4int32_t	iIndex = M4LDB_NULL_INDEX ;


	if( m_pLdb->DynamicLanguages() > 0 )
	{
		iIndex = FindIndexById( M4LDB_SYS_DICT_DYNAMIC_LANGUAGES, ai_iLanguage ) ;

		if( iIndex != M4LDB_NULL_INDEX )
		{
			return M4_TRUE ;
		}
	}

	return M4_FALSE ;
}



m4bool_t ClLConn::Notify (ClEvent &ai_Event){
    
	if (ai_Event.GetId()==M4_NOTIF_ACCESS_DESTROY){
		if (m_numTrans) {
			ClAccess *pAccess = (ClAccess *)ai_Event.GetContainer();
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_DESTROYED_ACCESS, M4Obj(pAccess ->GetpCMCR()));
			m4bool_t bFree = M4_FALSE;
			EndTransaction (NULL, 0, bFree);
		}
	}
	return M4_TRUE;
}

m4return_t ClLConn::_CreateSysHint( ClLConn * ai_pClLConn, const string & ai_sSysHint, ClSysHint * & ao_pClSysHint )
{
	m4return_t iRet = M4_SUCCESS;
	//Se parsea la cadena y sacamos un hint.
	if (ao_pClSysHint != NULL) {
		return M4_ERROR;
	}

	MpParseSysHint oMpParseSysHint;
	//Extraemos los atributos.	
	iRet = _ParseSysHint( ai_pClLConn, ai_sSysHint, oMpParseSysHint );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}

	//Ya tenemos parseado el syshint y rellenado el mapa.
	itMpParseSysHint itDb = oMpParseSysHint.find(M4LDB_SYSHINT_LITERAL_DB);
	itMpParseSysHint itId = oMpParseSysHint.find(M4LDB_SYSHINT_LITERAL_ID);
	itMpParseSysHint itType = oMpParseSysHint.find(M4LDB_SYSHINT_LITERAL_TYPE);
	itMpParseSysHint itText = oMpParseSysHint.find(M4LDB_SYSHINT_LITERAL_TEXT);
	if ((itDb == oMpParseSysHint.end()) 
		|| (itId == oMpParseSysHint.end())
		|| (itType == oMpParseSysHint.end())
		|| (itText == oMpParseSysHint.end()) )
	{		
		if (itDb == oMpParseSysHint.end()) {
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_DB );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_DB);
		}
		if (itId == oMpParseSysHint.end()) {
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_ID );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_ID);
		}
		if (itType == oMpParseSysHint.end()) {
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_TYPE );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_TYPE);
		}
		if (itText == oMpParseSysHint.end()) {
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_TEXT );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES, M4LDB_SYSHINT_LITERAL_TEXT);
		}

		return M4_ERROR;
	}

	ClSysHintPk * pClSysHintPk = new ClSysHintPk((*itDb).second, (*itId).second);
	if ((*itId).second.size() == 0) {
		ao_pClSysHint = new ClSysHint( pClSysHintPk, M4_TRUE, (*itType).second, (*itText).second );
	}
	else {
		ao_pClSysHint = new ClSysHint( pClSysHintPk, M4_FALSE, (*itType).second, (*itText).second );
	}
	
	return iRet;
}

m4return_t ClLConn::_ParseSysHint( ClLConn * ai_pClLConn, const string & ai_sSysHint, MpParseSysHint & ao_oMpParseSysHint )
{
	//Retorna un mapa con los atributos de un hint.
	m4int32_t iPosStart = 0;
	m4int32_t iPosEnd = 0;
	m4int32_t iPosSemicolon = 0;
	m4int32_t iNumChr = ai_sSysHint.size() - 1;

	//Inicialmente siempre entramos una vez. Tenemos que iterar mientras hayan puntos y coma pero 
	//no haya finalizado la cadena.	
	while ((iPosSemicolon != M4_STR_NPOS) && (iPosSemicolon < iNumChr) ) {
		//Buscamos el punto y coma desde el comienzo de subcadena
		iPosSemicolon = ai_sSysHint.find(";", iPosStart);
		//Si no hay mas punto y coma significa que el resto de la cadena es el token
		if (iPosSemicolon == M4_STR_NPOS) {
			iPosEnd = ai_sSysHint.size();
		}
		else {
			iPosEnd = iPosSemicolon - 1;
		}

		//Ya tenemos limitado el token con [iPosStart,iPosEnd] de la forma Clave=Valor.
		//Buscamos el igual. Debe existir y no nos podemos pasar del token!.
		//Si la cadena empieza por punto y coma o hay varios seguidos en algun sitio daremos un error. Esto es debido
		//a que se habremos pasado al siguiente token sin tener finalizado el token actual.
		m4int32_t iPosEqual = ai_sSysHint.find("=", iPosStart);
		if ((iPosEqual == M4_STR_NPOS) || (iPosEqual > iPosEnd)) {			
			ao_oMpParseSysHint.clear();
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_MISSING_CHARACTER_OR_KEYWORD, "=" );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_MISSING_CHARACTER_OR_KEYWORD, "=");
			return M4_ERROR;
		}
		else {
			//Metemos los valores en el mapa.
			string sKey = ai_sSysHint.substr(iPosStart, iPosEqual - iPosStart);
			string sValue = ai_sSysHint.substr(iPosEqual + 1, iPosEnd - iPosEqual);

			ao_oMpParseSysHint.insert( MpParseSysHint::value_type(sKey, sValue) );
		}

		//Ahora incrementamos el inicio al siguiente token
		iPosStart = iPosSemicolon + 1;
	}

	return M4_SUCCESS;
}

m4bool_t ClLConn::_IsReservedWordSysHint( m4pcchar_t ai_pTokenSysHint, m4int32_t & ao_iNumChrParsed )
{
	ao_iNumChrParsed = 0;

	if (ai_pTokenSysHint == NULL) {
		return M4_FALSE;
	}

	//Nos saltamos los blancos.
	while (*ai_pTokenSysHint == ' ')	{
		ai_pTokenSysHint++;
		ao_iNumChrParsed++;
	}

	//Debemos verificar que efectivamente es un Hint.
	if (strncmp(ai_pTokenSysHint, M4LDB_SYSHINT_APISQL_WORD, strlen(M4LDB_SYSHINT_APISQL_WORD)) == 0)	{
		ao_iNumChrParsed += strlen(M4LDB_SYSHINT_APISQL_WORD);
		return M4_TRUE;
	}

	return M4_FALSE;
}

m4return_t ClLConn::_GetSysHintIdParsed( ClLConn * ai_pClLConn, m4pcchar_t ai_pCurrent, string & ao_sSysHintId, m4int32_t & ao_iNumChrParsed )
{
	ao_iNumChrParsed = 0;
	ao_sSysHintId.assign(M4LDB_EMPTY);

	if (ai_pCurrent == NULL) {		
		return M4_ERROR;
	}

	//Si estamos aqui es pq ya sabemos que es un hint.
	//Ademas, el current lo hemos dejado justo en el caracter despues de la palabra que identifica al Hint!!.
	//Pasamos de los espacios, buscamos el comienzo del parentesis.	
	while ((*ai_pCurrent != NULL) && (*ai_pCurrent == ' ')) {
		ai_pCurrent++;
		ao_iNumChrParsed++;
	}

	//Tiene que ser un parentesis abierto
	if ((*ai_pCurrent == NULL) || (*ai_pCurrent != '(')) {
		//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_SINTAX_ERROR, "(" );
		DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_SINTAX_ERROR, "(");
		return M4_ERROR;
	}
	ai_pCurrent++;
	ao_iNumChrParsed++;

	m4pcchar_t pBeginSysHintId = ai_pCurrent;
	while ((ai_pCurrent != NULL) && ((*ai_pCurrent) != ')'))
	{
		ao_iNumChrParsed++;
		ai_pCurrent++;
		//No hemos encontrado el cierre y ya hemos terminado, es un error sintactico.
		if (ai_pCurrent == NULL) {
			//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_SINTAX_ERROR, ")" );
			DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_SINTAX_ERROR, ")");
			return M4_ERROR;
		}
	}

	//Si estamos aqui es pq se ha encontrado el parentesis.	
	ao_sSysHintId.append(pBeginSysHintId, ai_pCurrent - pBeginSysHintId);

	//Obligtoriamente tienen que haber especificado un id.
	if (ao_sSysHintId.size() == 0) {
		//DUMP_CHLOG_ERROR ( M4_CH_LDB_SYSHINT_IDHINT_MISSING );
		DUMP_LDBINSP_ERROR (ai_pClLConn, M4_CH_LDB_SYSHINT_IDHINT_MISSING);
		return M4_ERROR;
	}

	//Nos saltamos el parentesis cerrado.
	ai_pCurrent++;
	ao_iNumChrParsed++;

	//Buscamos el parentesis de cierre y lo que hay en medio es el id del hint
	return M4_SUCCESS;
}

m4return_t ClLConn::_ComprimeixBlancs( string & aio_sToCompress, m4int32_t ai_iPosBegin )
{
	//Comprime los blancos a partir de cierta posicion de inicio.
	m4int32_t iSize = aio_sToCompress.size();
	if (ai_iPosBegin >= iSize) {
		return M4_ERROR;
	}

	m4int32_t iPos = ai_iPosBegin;
	for (iPos = ai_iPosBegin; (iPos < iSize), (aio_sToCompress[iPos] == ' '); iPos++);

	//Cuando se sale del bucle tenemos cuantos espacios seguidos hay. Dejamos solo uno.
	aio_sToCompress.replace( ai_iPosBegin, iPos - ai_iPosBegin, " ");
	return M4_SUCCESS;
}

m4return_t ClLConn::_IsInsideLiteral( ClLConn * ai_pClLConn, m4int32_t ai_iPosSourceString, const string & ai_sSourceString, m4bool_t & ao_bIsInsideLiteral, m4int32_t & aio_iPosBeginLiteral, m4int32_t & aio_iPosEndLiteral )
{
	//Devuelve si la posicion ai_iPosSourceString es parte de un literal de la cadena de entrada.
	//Hay dos argumentos de entrada/salida que nos dicen donde empezar a buscar el comienzo del literal 
	//y donde puede acabar.
	//Como salida se dejarán el comienzo y el fin del literal que comprende a ai_iPosSourceString.

	//Importante: Se asume que la posicion de inicio a buscar un literal NO está dentro de un literal!.
	//Si se pudieran anidar se hubiera hecho recursivamente o utilizando las funciones de busqueda por el final.
	ao_bIsInsideLiteral = M4_FALSE;

	//Tiene que haber suficientes espacios para tener las dos comillas y otros caracteres.
	if ((aio_iPosEndLiteral - aio_iPosBeginLiteral) < 2) {
		return M4_SUCCESS;
	}
	//Si los indices están fuera de rango, no hay nada que analizar.
	if ((ai_iPosSourceString < aio_iPosBeginLiteral)
		|| (ai_iPosSourceString > aio_iPosEndLiteral)) 
	{
		return M4_SUCCESS;
	}

	m4int32_t iPosBeginSearch = aio_iPosBeginLiteral;
	m4int32_t iPosEndSearch = aio_iPosEndLiteral;
	
	m4int32_t iPosBegin = ai_sSourceString.find_first_of("'", iPosBeginSearch);
	m4int32_t iPosEnd = iPosBegin + 1;

	while (M4_TRUE)
	{
		if (iPosBegin != -1) {
			//Se ha encontrado un comienzo de literal.
			//-------------------------------------------------------------
			while (M4_TRUE)
			{
				iPosEnd = ai_sSourceString.find_first_of("'", iPosEnd);
				if ((iPosEnd == -1) || (iPosEnd > iPosEndSearch)) {
					//Mal rollo, no hay matching o nos hemos pasado del fin estipulado!.
					//DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_SINTAX_ERROR, " ' " );
					DUMP_LDBINSP_ERRORF (ai_pClLConn, M4_CH_LDB_SYSHINT_SINTAX_ERROR, " ' ");
					return M4_ERROR;
				}

				//Vamos a comprobar que no es una comilla escapada!.
				m4int32_t iPosScaped = ai_sSourceString.find_first_of("'", iPosEnd + 1);			
				if (iPosScaped == (iPosEnd + 1)) {
					//Está escapada.
					iPosEnd = ai_sSourceString.find_first_of("'", iPosScaped + 1);
				}
				else {
					//No es escapada. Salimos del bucle interno.
					break;
				}
			}
			//-------------------------------------------------------------

			//Ya tenemos comienzo y fin.
			if ((iPosBegin < ai_iPosSourceString) && (iPosEnd > ai_iPosSourceString)) {
				//La posicion que nos han pasado se encuentra dentro de un literal.
				ao_bIsInsideLiteral = M4_TRUE;
				aio_iPosBeginLiteral = iPosBegin;
				aio_iPosEndLiteral = iPosEnd;
				return M4_SUCCESS;
			}
			if (iPosBegin > ai_iPosSourceString) {
				//Nos hemos pasado de la posicion, nos vamos.
				return M4_SUCCESS;
			}

			//Si estamos aqui es pq se ha encontrado un literal antes de la posicion que estoy comprobando.
			//Iteramos de nuevo.
			iPosBeginSearch = iPosEnd + 1;
			iPosBegin = ai_sSourceString.find_first_of("'", iPosBeginSearch);
			iPosEnd = iPosBegin + 1;
		}
		else {			
			break;
		}
	}

	//Si estamos aqui es pq la posicion no se encuentra entre literales, vamos a devolver falso.
	return M4_SUCCESS;
}

m4char_t ClLConn::_GetIdAutoFilter( ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode )
{
	m4char_t cIdAutoFilter = M4_FALSE;

	if (ai_pRecordSet == NULL) {
		return cIdAutoFilter;
	}

	if (ai_pMCR->GetChannelIsCacheable () == M4_TRUE) {
		return M4_FALSE;
	}
			
	cIdAutoFilter = ai_pMCR->GetNodeIdAutoFilter( ai_hNode );
	if ( cIdAutoFilter != 0) {
		//Si hay filtro vamos a comprobar valores!.
		//Puede haber filtros de aplicacion/correcion pero si son infinito NO deben aplicarse.
		if (cIdAutoFilter & M4CL_NODE_AUTO_FILTER_APPLICATION) {				
			if (_IsInfiniteDates(ai_pRecordSet, ai_pMCR, ai_hNode, M4CL_NODE_AUTO_FILTER_APPLICATION) == M4_TRUE) {
				cIdAutoFilter = cIdAutoFilter & (~M4CL_NODE_AUTO_FILTER_APPLICATION);
			}
		}
		if (cIdAutoFilter & M4CL_NODE_AUTO_FILTER_CORRECTION) {				
			if (_IsInfiniteDates(ai_pRecordSet, ai_pMCR, ai_hNode, M4CL_NODE_AUTO_FILTER_CORRECTION) == M4_TRUE) {
				cIdAutoFilter = cIdAutoFilter & (~M4CL_NODE_AUTO_FILTER_CORRECTION);
			}
		}
	}

	return cIdAutoFilter;	
}

m4bool_t ClLConn::_IsInfiniteDates( ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode, m4uint8_t ai_iAutoFilterType )
{
	m4int32_t hItem = 0;
	m4VariantType vDate;
	m4date_t dStartDate = 0;
	m4date_t dEndDate = 0;

	if (ai_pRecordSet == NULL) {
		return M4_FALSE;
	}

	if (ai_iAutoFilterType == M4CL_NODE_AUTO_FILTER_APPLICATION) {			
		//------------------------------------------------------
		vDate.SetNull();
		hItem = ai_pMCR->GetNodeFilterStartDateItem (ai_hNode);
		if (hItem) {
			ai_pRecordSet->Item[(ClHandle) hItem].Value.Get(vDate);
		}
		if (vDate.GetType () == M4CL_CPP_TYPE_NULL) {
			ai_pRecordSet->GetpAccess()->GetpChannel()->Data_StartDate.Get (vDate);
		}				
		dStartDate = vDate.Data.DoubleData;
		//------------------------------------------------------

		//------------------------------------------------------
		vDate.SetNull();
		hItem = ai_pMCR->GetNodeFilterEndDateItem (ai_hNode);
		if (hItem) {
			ai_pRecordSet->Item[(ClHandle) hItem].Value.Get(vDate);
		}
		if (vDate.GetType () == M4CL_CPP_TYPE_NULL) {
			ai_pRecordSet->GetpAccess()->GetpChannel()->Data_EndDate.Get (vDate);
		}				
		dEndDate = vDate.Data.DoubleData;
		//------------------------------------------------------

		if ((dStartDate == M4LDB_MINUS_INF_DATE) && (dEndDate == M4LDB_PLUS_INF_DATE)) {
			return M4_TRUE;
		}
	}

	if (ai_iAutoFilterType == M4CL_NODE_AUTO_FILTER_CORRECTION) {
		//------------------------------------------------------
		vDate.SetNull();
		hItem = ai_pMCR->GetNodeFilterStartCorDateItem (ai_hNode);
		if (hItem) {
			ai_pRecordSet->Item[(ClHandle) hItem].Value.Get(vDate);
		}
		if (vDate.GetType () == M4CL_CPP_TYPE_NULL) {
			ai_pRecordSet->GetpAccess()->GetpChannel()->Data_CorrectionDate.Get (vDate);
		}				
		dStartDate = vDate.Data.DoubleData;
		//------------------------------------------------------

		//------------------------------------------------------
		vDate.SetNull();
		hItem = ai_pMCR->GetNodeFilterEndCorDateItem (ai_hNode);
		if (hItem) {
			ai_pRecordSet->Item[(ClHandle) hItem].Value.Get(vDate);
		}
		if (vDate.GetType () == M4CL_CPP_TYPE_NULL) {
			ai_pRecordSet->GetpAccess()->GetpChannel()->Data_CorrectionDate.Get (vDate);
		}				
		dEndDate = vDate.Data.DoubleData;
		//------------------------------------------------------

		if ((dStartDate == M4LDB_MINUS_INF_DATE) && (dEndDate == M4LDB_PLUS_INF_DATE)) {
			return M4_TRUE;
		}
	}

	return M4_FALSE;
}


m4return_t	ClLConn::_CheckUserSQL( m4bool_t ai_bIsUser, m4pcchar_t ai_pccSQL )
{
	if (ai_bIsUser == M4_TRUE)
	{
		if (m_pLdb->GetSQLAccessLevel() == 1)
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG( this, "\nUnauthorized statement." ) ;
				M4LDB_PRINT_DEBUG_DETAIL( M4LDB_DETAIL_LEVEL_SHOW_PREPARE, this, "\nUnauthorized statement." ) ;
			#endif

			DUMP_CHLOG_WARNINGF( M4_CH_ACCESS_UNAUTHORIZED_SENTENCE, ai_pccSQL << DumpLogAndLdb(this) ) ;
		}
		else if (m_pLdb->GetSQLAccessLevel() == 2)
		{
			DUMP_CHLOG_ERRORF( M4_CH_ACCESS_UNAUTHORIZED_SENTENCE, ai_pccSQL << DumpLogAndLdb(this) ) ;
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_UNAUTHORIZED_SENTENCE, ai_pccSQL ) ;
			return (M4_ERROR);
		}
	}
	return (M4_SUCCESS);
}


//Control de reentrada en DBL.
ClLConnReenterControl::ClLConnReenterControl(ClLConn * ai_pLConn)
{
	if (ai_pLConn) {
		m_pLConn = ai_pLConn;
		//Cada vez que se invoca al constructor incrementamos en 1.
		m_pLConn->AddReenter (1);
	}
	else {
		m_pLConn = NULL;
	}
}

ClLConnReenterControl::~ClLConnReenterControl()
{
	if (m_pLConn) {
	
		//Cuando llegamos a 0 es cuando hay que invocar a la auditoría.
		if (m_pLConn->IsReenter() == M4_FALSE) {
			//Invocamos a la ejecucion de auditoria.
			//Auditoria select.
			//Como este punto es un punto de salida de la BDL, necesitamos desencadenar la
			//ejecucion (persistencia) de la auditoria.
			//La ejecuión tendrá en cuenta si es reentrada o si efectivamente.
			//hay auditoria que hay que persistir.
			m4return_t iResultAud;
			iResultAud = m_pLConn->m_pSelectAuditory->ExecuteAudit();
			if (iResultAud == M4_ERROR)
			{
				DUMP_CHLOG_ERRORF ( M4_CH_LDB_AUDIT_EXEC_ERROR, "Reenter control" );
				//return (M4_ERROR);
			}
			//Fin auditoria select.
		}	//if (lReenterCounter == 0) {

		//Decrementamos en 1.
		m_pLConn->AddReenter (- 1);

	} //if (m_pLConn) {

	m_pLConn = NULL;
}
