//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clstatem.cpp
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


#include "clldb.hpp"
#include "cursor.hpp"
#include "clstatem.hpp"
#include "sttlib.hpp"
//#include "chvmenv.hpp"
#include "cldinarr.hpp"
#include "clsentcc.hpp"
#include "cllgadap.hpp"
#include "ldb_log.hpp"
#include "access.hpp"
#include "conex.hpp"
#include "m4ldb_notif.hpp"
#include "m4event.hpp"
#include "m4thread.hpp"				//La sesion se obtiene del thread.

//Bug: 0072298. El server se queda bloqueado al perder conectividad.
//Para tener acceso a M4PublishEvent.
#include "m4eventhandlers.hpp"

#include "clstatem.inl"



ClStmt::~ClStmt (void)
{
	Destroy ();
}


m4bool_t	ClStmt::GetCancelStatus( void )
{

	m4bool_t	bCanceled = M4_FALSE ;


	if( m_pGlobVars != NULL )
	{
		ClAccessRecordSet	*poRecordSet = m_pGlobVars->GetpAccessRecordSet() ;

		if( poRecordSet != NULL )
		{
			ClAccess	*poAccess = poRecordSet->GetpAccess() ;

			if( poAccess != NULL )
			{
				IExecutor	*poExecutor = poAccess->GetpExecutor() ;

				if( poExecutor != NULL )
				{
					bCanceled = poExecutor->GetCancelStatus() ;
				}
			}
		}
	}

	return( bCanceled ) ;
}


m4return_t	ClStmt::Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, m4bool_t ai_bMtSentOwner, ClDinArr *ai_pParam)
{
	m4return_t result;

	if (IsDDBBDisabled ()) {
		//Bug 0090288, 0092128
		//Enviamos una señal de retry. El subsistema intentará hacer un refresco de la conexion.		
		//ClEvent oEvent(this,M4_NOTIF_RETRY_CONNECTION);
		if ( (GetpGlobVars()) && 
			(GetpGlobVars()->GetpLConn()) && 
			(GetpGlobVars()->GetpLConn()->GetConnManager()) ) {

			ClConnectionManager * pConnManager = GetpGlobVars()->GetpLConn()->GetConnManager();
			m4int32_t iIdRConn = GetpGlobVars()->GetIdRConn();
			m4int32_t iIdLConn = GetpGlobVars()->GetpLConn()->GetIdLConn();

			ClEventIntInt oEvent (pConnManager, M4_NOTIF_RETRY_CONNECTION, iIdRConn, iIdLConn );		
			m_pGlobVars->GetpLConn ()->GetConnManager()->DBRypture_Notifier.Signal(oEvent);
		}
		// Si seguimos desconectados, solatmaos warning  y nos vamos.
		if (IsDDBBDisabled ()) {
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_DDBB_DISABLED);
			return (M4_ERROR);
		}
	}


	m_pMtSent = ai_pMtSent;

	m_eStmType = ai_eType;

	m_bMtSentOwner = ai_bMtSentOwner;

	if (ai_pParam) {
		m_pParam = ai_pParam;
		m_pVcClParam = ai_pParam->GetVcClParam();
	}
	else {
		m_pParam = new ClDinArr (m_pGlobVars);
		m_pVcClParam = m_pParam->GetVcClParam();
	}

	m_pParam->Attach ();

	InitStt ();

	m_pCursor = new ClCursor ();

	m_idRConn = ai_idRConn;

	if (m_idRConn == M4LDB_NULL_INDEX) {
		m_idRConn = 0;
	}

	result = m_pCursor->Open ();

	InsertCursor (m_pGlobVars->GetpLConn ()->GetIdLConn (), m_pCursor);

	return (DisplayError(result));
}


m4return_t ClStmt::Destroy (void)
{
	if (m_pParam) {
		m_pParam->Detach ();

		if (!m_pParam->GetReferences ()) {
			delete m_pParam;
		}
	}
	m_pParam = NULL;
	m_pVcClParam = NULL;

	for (itMpPClCursor it = m_mpCursor.begin (); it != m_mpCursor.end (); it++) {
		if (IsPartialLoad ()) {
			m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ().ErasePartialLoad ((*it).second);
		}

		delete (*it).second;
	}
	m_mpCursor.erase (m_mpCursor.begin (), m_mpCursor.end ());
	m_pCursor = NULL;

	if ((m_pMtSent) && 
		(m_bMtSentOwner)) {
		delete m_pMtSent;
	}
	m_pMtSent = 0;

	if (m_pGlobVars) {
		m_pGlobVars->Detach ();

		if (!m_pGlobVars->GetReferences ()) {
			delete m_pGlobVars;
		}
	}
	m_pGlobVars = NULL;

	return (M4_SUCCESS);
}


m4return_t ClStmt::Close (void)
{
	m4return_t result;

	if (m_pCursor) {

		if (IsPartialLoad ()) {
			m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ().ClosePartialLoad (m_pCursor);
		}

		result = m_pCursor->Close ();

		if (result != M4_SUCCESS) {
			return (DisplayError(result));
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClStmt::Prepare (m4pchar_t ai_pStmSQL)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tPrepare: Stmt = %s", ai_pStmSQL);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n\tPrepare: Stmt = %s", ai_pStmSQL);
	#endif

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	m4bool_t bIsPartialLoad = IsPartialLoad ();

	result = m_pCursor->Prepare (ai_pStmSQL, m_pGlobVars->GetDDBBType(), bIsPartialLoad);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m4pcchar_t pSQL = 0;
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}

    return (DisplayError(result));
}


m4return_t ClStmt::BindParam (m4int32_t ai_lPar, m4int32_t ai_lParam, m4bool_t ai_bUseOldValue, m4bool_t ai_bBlobAsBinary)
{
    m4return_t result;

	m4int8_t m4type = m_pParam ->GetInfoM4Type (ai_lParam) ;

	/* Bug 0165266
	Si es un parámetro de auditoría de tipo fichero, se debe leer como binario, ya que no se copia a fichero
	*/
	if( ai_bBlobAsBinary == M4_TRUE && m4type == M4CL_M4_TYPE_BLOB )
	{
		m4type = M4CL_M4_TYPE_BINARY_STRING ;
	}

	m4pchar_t *ppValue = ((!ai_bUseOldValue) || (!m_pParam->GetHasOldValue (ai_lParam))) ? m_pParam->GetDataPointerPointer (ai_lParam) : m_pParam->GetOldDataPointerPointer (ai_lParam);

	m4pint32_t pLength = ((!ai_bUseOldValue) || (!m_pParam->GetHasOldValue (ai_lParam))) ? m_pParam->GetLengthPointer (ai_lParam) : m_pParam->GetOldLengthPointer (ai_lParam);

  	#ifdef _M4LDB_DEBUG
  		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tPar[%d] -> Data[%ld].", ai_lPar, ai_lParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n\tPar[%d] -> Data[%ld].", ai_lPar, ai_lParam);
	#endif

	if (m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG || m4type == M4CL_M4_TYPE_BINARY_STRING ) {
		// Bindamos el puntero del puntero.
		ppValue = m_pParam->GetDataPointerPointer (ai_lParam);
	}

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->BindParam (ai_lPar, NULL, ppValue, m_pParam->GetInfoPrec (ai_lParam), m4type, m_pParam->GetInfoScale (ai_lParam), pLength);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m4pcchar_t pSQL = 0;
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}

	return (DisplayError(result));
}

m4return_t ClStmt::BindParam (m4int32_t ai_lPar, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ai_pLength)
{
    m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tPar[%d] -> Type[%s]. Size[%ld].", ai_lPar, g_aM4Type[ai_lM4Type], ai_lPrec);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n\tPar[%d] -> Type[%s]. Size[%ld].", ai_lPar, g_aM4Type[ai_lM4Type], ai_lPrec);
	#endif

	result = m_pCursor->BindParam (ai_lPar, ai_pValue, ai_ppValue, ai_lPrec, ai_lM4Type, ai_lScale, ai_pLength);

    return (DisplayError(result));
}

m4return_t ClStmt::BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld)
{
    m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tCol[%d] -> Type[%s]. Size[%ld].", ai_lCol, g_aM4Type[ai_lM4Type], *aio_plMaxLen);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n\tCol[%d] -> Type[%s]. Size[%ld].", ai_lCol, g_aM4Type[ai_lM4Type], *aio_plMaxLen);
	#endif

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->BindResult (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, ao_lLength, ai_lMaxLenFld);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m4pcchar_t pSQL = 0;
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}


    return (DisplayError(result));
}


m4return_t ClStmt::Execute (m4uint32_t ai_iTimeOut)
{
    m4return_t result;
	m4pcchar_t pSQL = NULL;

	if (IsDDBBDisabled ()) {
		//Bug 0090288, 0092128
		//Enviamos una señal de retry. El subsistema intentará hacer un refresco de la conexion.		
		//ClEvent oEvent(this,M4_NOTIF_RETRY_CONNECTION);
		if ( (GetpGlobVars()) && 
			(GetpGlobVars()->GetpLConn()) && 
			(GetpGlobVars()->GetpLConn()->GetConnManager()) ) {

			ClConnectionManager * pConnManager = GetpGlobVars()->GetpLConn()->GetConnManager();
			m4int32_t iIdRConn = GetpGlobVars()->GetIdRConn();
			m4int32_t iIdLConn = GetpGlobVars()->GetpLConn()->GetIdLConn();

			ClEventIntInt oEvent (pConnManager, M4_NOTIF_RETRY_CONNECTION, iIdRConn, iIdLConn );		
			m_pGlobVars->GetpLConn ()->GetConnManager()->DBRypture_Notifier.Signal(oEvent);
		}
		// Si seguimos desconectados, solatmaos warning  y nos vamos.
		if (IsDDBBDisabled ()) {
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_DDBB_DISABLED);
			return (M4_ERROR);
		}
	}

	#ifdef _M4LDB_DEBUG
		string sDynamic ("");		
		if (m_pGlobVars->GetpLConn()->GetConnManager()->IsDynamicConnectionByIdRConn(m_idRConn) == M4_TRUE) {
			sDynamic.append(" (Dynamic). ");
		}

		m4id_t	pccRole = m_pGlobVars->GetIdSecRole();

		if (pccRole == NULL)
		{
			pccRole = "";
		}

		m4id_t	pccRSM = m_pGlobVars->GetIdRSM();

		if (pccRSM == NULL)
		{
			pccRSM = "";
		}

		m4pcchar_t		pccOrg = "";
		m4VariantType	vOrg;
		ClChannel		*pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

		if (pChannel->Organization.Get(vOrg) == M4_SUCCESS && vOrg.Type == M4CL_CPP_TYPE_STRING_VAR && vOrg.Data.PointerChar != NULL)
		{
			pccOrg = vOrg.Data.PointerChar;
		}

		M4LDB_PRINT_DEBUG_DATE (m_pGlobVars->GetpLConn(), "\n\tDB Connection(%d).%s Role <%s> RSM <%s> Organization <%s>.", m_idRConn, sDynamic.c_str(), pccRole, pccRSM, pccOrg);
		M4LDB_PRINT_DEBUG_DETAIL_DATE (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn(), "\n\tDB Connection(%d).%s Role <%s> RSM <%s> Organization <%s>.", m_idRConn, sDynamic.c_str(), pccRole, pccRSM, pccOrg);
	#endif

	m_pGlobVars->SetInvStmt (M4_TRUE);
		
	result = SetCursor (m_pGlobVars->GetpLConn ());

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	ClDBConnection *pDBConn = 0;

	m4uint64_t iIdSession = 0;

	//Obtenemos la sesion del thread!.Hacemos el casting a (m4objid_t *). Es un entero de 64.
	m4uint64_t * pIdSession = (m4uint64_t *) M4Thread::GetProperty( M4_THREADPROP_SESSION_ID );	
	if (pIdSession != NULL) {
		iIdSession = *pIdSession;
	}

	if (pLConn != NULL) {
		pLConn->SetIdSession( iIdSession );
	}

	pDBConn = pLConn->GetConnManager()->InitTransaction(m_idRConn,pLConn);
	if (pDBConn==0) {
		return M4_ERROR;
	}

	m4bool_t bReallyInitialized = M4_FALSE;

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->InitCursor (pDBConn, bReallyInitialized);

	//Bug 0072356. Comprobamos el puntero.
	if ( (m_bSttInit) && (m_pGlobVars->GetpLConn ()->m_poStatistics) && (M4_TRUE == bReallyInitialized) ) {
		// Pillamos la PK en el prepare
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn(), "\n\tExecute");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), "\n\tExecute");

		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), m_pCursor);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), m_pCursor);
	#endif

	if (result == M4_ERROR) {
		return (DisplayError(result));
	}

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	m4return_t iCanceled = M4_SUCCESS;
	result = m_pCursor->Execute (ai_iTimeOut, this, iCanceled);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		if (!pSQL)
			m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_EXECUTE, m_eStmType, m_bIsInternal, pSQL);
	}

	m4return_t iReturn = DisplayError(result);

	if( ai_iTimeOut > 0 )
	{
		m_pCursor->SetBlockingMode( 0 ) ;
	}
	
	if( iCanceled == -1 )
	{
		DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn (), M4_CH_LDB_CANCELED, "Execute" ) ;
		return( M4_ERROR ) ;
	}
	else if( iCanceled == -2 )
	{
		DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn (), M4_CH_LDB_COMPLEX_EXECUTION, ai_iTimeOut ) ;
		return( M4_ERROR ) ;
	}

	if (result != M4_ERROR) {
		m_pGlobVars->SetInvStmt (M4_FALSE);
	}

	return (iReturn);
}


m4return_t ClStmt::DisplayError (m4return_t ai_retcode, m4bool_t ai_bWarningLikeDebug)
{
	m4pchar_t pcError = NULL;
	m4int32_t result = ai_retcode;
	m4int32_t iErrorCode;
	m4bool_t bClose = M4_FALSE;
	m4bool_t bDestroy = M4_FALSE;
	m4bool_t bDisconnect = M4_FALSE;
	m4bool_t bDisconnectAll = M4_FALSE;

	if (ai_retcode == M4_SUCCESS) {
		return ((m4return_t) ai_retcode);
	}

	if (ai_retcode == M4LDB_SQL_NO_DATA_FOUND) {
		// El bWarningLikeDebug sólo se utiliza en el GetData.
		if (!ai_bWarningLikeDebug) {
			bClose = M4_TRUE;
		}
		result = M4LDB_SQL_NO_DATA_FOUND;
	}

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	ClConnectionManager * pConnManager = pLConn->GetConnManager();

	while (result != M4LDB_SQL_NO_DATA_FOUND) {
		
		if (IsDDBBDisabled ()) {
			// Si el error lo hemos capturado aqui significa que se estaba realizando una operación,
			// por lo que es mejor si borramos el cursor físico.
			m_pCursor->Destroy();
			DUMP_LDBINSP_ERROR (pLConn, M4_CH_LDB_DDBB_DISABLED);
			return (M4_ERROR);
		}

		result = m_pCursor->GetErrorString (ai_retcode, iErrorCode, &pcError);

		if (result != M4LDB_SQL_NO_DATA_FOUND) {
			if (result == M4_ERROR) {
				DUMP_LDBINSP_ERROR (pLConn, M4_CH_LDB_DBWRAP_DRIVER_ERROR);
				return (M4_ERROR);
			}
			else {
				//Bug 0075304
				//Este flag lo utlizamos para indicar que se trata de un mensaje de error definido por el usuario
				//en la taBLA M4RDC_ERRORS.
				m4bool_t bChangeOnErrorMsg = M4_FALSE;

				ClRegisterIndex indError(0);
				m4int32_t iAction = M4LDB_ACT_UNDEFINED;

				m4int32_t lLDBErrorCode = M4_CH_LDB_DDBB_ERROR;
				m4int32_t nRetry = 0;


				if (ai_retcode == M4_WARNING) {
					lLDBErrorCode = M4_CH_LDB_DDBB_WARNING;
				}

				m4double_t dError = (m4double_t) iErrorCode;

				m4VariantType *pParams = NULL;
				m4int32_t nPar = 0;
				m4int32_t CurrentPar = 0;

				// Bug 0281818 Si es un warning y el código nativo es 0 no se hace la acción
				if( ai_retcode != M4_WARNING || iErrorCode != 0 )
				{
					eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
					
					if (pConnManager->GetDDBBType(m_idRConn,pLConn,eDDBB)==M4_SUCCESS){
					
						// Nos situamos en el bloque de los errores para esta DDBB.
						m4int32_t indDDBB = pLConn->FindIndexById (M4LDB_SYS_DICT_DDBBS, eDDBB);
						pLConn->m_paRS[M4LDB_SYS_DICT_DDBBS]->Current[indDDBB];

						indError = pLConn->FindIndexById (M4LDB_SYS_DICT_ERRORS, dError);

						if (indError != M4LDB_NULL_INDEX) {

							m4int32_t lError1 = M4LDB_NUMBER (pLConn->m_paRS[M4LDB_SYS_DICT_ERRORS]->Current[indError].Item[M4LDB_ITEM_SYS_DICT_ERRORS_ID_ERROR].Value.GetWithOutSec ());
							m4int32_t lError2 = M4LDB_NUMBER (pLConn->m_paRS[M4LDB_SYS_DICT_ERRORS]->Current[indError].Item[M4LDB_ITEM_SYS_DICT_ERRORS_ID_ERROR2].Value.GetWithOutSec ());

							// Tenemos que hacer esto porque error2 puede ser un cero.
							if (((iErrorCode == lError1) || 
								(iErrorCode == lError2) || 
								((iErrorCode > lError1) && 
								(iErrorCode < lError2)))) {
								iAction = M4LDB_NUMBER (pLConn->m_paRS[M4LDB_SYS_DICT_ERRORS]->Current[indError].Item[M4LDB_ITEM_SYS_DICT_ERRORS_ID_ACTION].Value.GetWithOutSec ());
								// Leo los argumentos que me vengan ( vienen separados por | )
								pParams = new m4VariantType[32];
								pLConn->GetSysParam (pLConn->m_paRS[M4LDB_SYS_DICT_ERRORS], M4LDB_ITEM_SYS_DICT_ERRORS_ARGUMENT1, pParams, nPar);
							}
						}
					}
				}

				if (iAction == M4LDB_ACT_GPF) {
					// Inducimos a que se produzca un GPF para poder depurar bajo que condiciones se ha producido el error.
					memcpy (0, "GPF", 10);
				}


				if (iAction & M4LDB_ACT_DESTROY_CURSOR) {
					bDestroy = M4_TRUE;
				}

				//Bug: 0072298. El server se queda bloqueado al perder conectividad.
				//Un shutdown permite reintentos. Por lo tanto no decimos que se desconecte todavía.
				if ((iAction & M4LDB_ACT_DESTROY_CONNECTION) ) {
					bDisconnect = M4_TRUE;
				}

				// eh 202503
				if ((iAction & M4LDB_ACT_DESTROY_ALL_CONNECTIONS) ) {
					bDisconnectAll = M4_TRUE;
				}

				
				if (iAction & M4LDB_ACT_CHANGE_ERROR){
					//Bug 0075304	
					bChangeOnErrorMsg = M4_TRUE;
					lLDBErrorCode = 0;
					if (pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING ||
						pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING_VAR) {
						lLDBErrorCode = atol (pParams[CurrentPar].Data.PointerChar);
					}
					CurrentPar++;
					// Una cadena vacía significa que no queremos que salga ningún error.
				}


				if (iAction & M4LDB_ACT_RETRY){
					nRetry = 0;
					if (pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING ||
						pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING_VAR) {
						nRetry = atol (pParams[CurrentPar].Data.PointerChar);
						if (nRetry==0){
							nRetry = 1000000;
						}
					}
					CurrentPar++;
				}


				if (iAction & M4LDB_ACT_CHANGE_ERROR_IF_ORDINAL){
					//Bug 0075304
					bChangeOnErrorMsg = M4_TRUE;
					// Si no tenemos ordinales, pasamos.
					if (HasOrdinal()){
						lLDBErrorCode = 0;// Lo reseteo, porque estamos con ordinales y tengo accion para ellos

						if (pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING ||
							pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING_VAR) {
							lLDBErrorCode  = atol (pParams[CurrentPar].Data.PointerChar);
						}
					}
					CurrentPar++;
					// Una cadena vacía significa que no queremos que salga ningún error.
				}


				if (iAction & M4LDB_ACT_RETRY_IF_ORDINAL){
					// Si no tenemos ordinales, pasamos.
					if (HasOrdinal()){
						nRetry = 0;// Lo reseteo, porque estamos con ordinales y tengo accion para ellos
						if (pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING ||
							pParams[CurrentPar].Type==M4CL_CPP_TYPE_STRING_VAR) {
							nRetry = atol (pParams[CurrentPar].Data.PointerChar);
							if (nRetry==0){
								nRetry = 1000000;
							}
						}
					}
					CurrentPar++;
				}

				if (pParams) {
					delete [] pParams;
				}
				pParams = NULL;

				//Bug 0071661.
				//Si estamos en una sentencia Select NOWAIT, no debemos hacer reintentos.
				if ( (m_pMtSent) && (m_pMtSent->GetStmType () == M4LDB_SELECT) && (m_pMtSent->GetNoWait() == M4_TRUE) ) {
					nRetry = 0;
				}

				if (nRetry) {
					m_pGlobVars->SetIsRetry (M4_TRUE);
					if (m_pGlobVars->GetNumberOfAttempts () == M4LDB_NULL_INDEX) {
						m_pGlobVars->SetNumberOfAttempts (nRetry);
					}
				}


				if (m_pGlobVars->GetNumberOfAttempts () > 0) {
					// Mientras estemos reintentando sólo sacamos un debuginfo y no hacemos nada mas.
					DUMP_LDBINSP_DEBUGF (pLConn, lLDBErrorCode, iErrorCode << pcError);
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (pLConn, "\n\tRetry.");
					#endif
					continue;
				}


				if (lLDBErrorCode) {					
					if (ai_retcode == M4_WARNING) {
						if (ai_bWarningLikeDebug) {
							if (pLConn->m_pLdb->GetSysDebugLevel () == M4LDB_DEBUG_LEVEL_3) {
								DUMP_LDBINSP_DEBUGF (pLConn, lLDBErrorCode, iErrorCode << pcError);
							}
						}
						else {
							DUMP_LDBINSP_WARNINGF (pLConn, lLDBErrorCode, iErrorCode << pcError);
						}
					}
					else {
						string stSQL(M4LDB_EMPTY);
						if (!pLConn->m_pLdb->GetShowData ()) {
							m4pcchar_t pSQL = 0;
							m_pCursor->GetStmt (pSQL);
							stSQL = pSQL;
						}
						else {
							m_pCursor->GetStmt (stSQL);
						}

						//Bug 0075304
						//Ejemplo de parametros que le pasamos:
						//0: 54			(Código de mensaje de error)
						//1: ORA-0054	(Error que da Oracle)
						//2: Select...	(La sentencia que ocasiona el error)
						//3: MixSti		(Es el identificador del registro)
						//4: MixIdNm	(Id-Nombre de la tabla)	-- Para que el mapeo sea adecuado meteremos blanco si no hay Id-Nm.
						//5: Nodo		(Este lo mete la log)
						//6: M4O		(Este lo mete la log)
						//Si no hay nombre de tabla concatenamos "", Dupla Id-Nm.
						//El motivo es que el formateo del mensaje en el logmsg.ini es fijo y se
						//mapea a un determinado número de argumentos. Se estaban utilizando %3, %4, %5, %6.
						
						string stnPKRegister(M4LDB_EMPTY);
						string stiPKRegister(M4LDB_EMPTY);
						ComposePK (stnPKRegister, stiPKRegister);
						m4id_t nLgcTbl = pLConn->GetnLgcTbl (GetIndLgcTbl ());
						m4id_t iLgcTbl = GetIdLgcTbl ();

						if (bChangeOnErrorMsg == M4_FALSE) {
							//Por problemas con el mensaje de error duplicamos los casos
							if(!strlen(iLgcTbl) && !strlen(nLgcTbl)) {
								//En este caso las dos cadenas son vacias
								DUMP_LDBINSP_ERRORF (pLConn, lLDBErrorCode, 
									iErrorCode << 
									pcError << 
									stSQL.c_str () << 
									LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
									"" << 
									M4_CH_LDB_ERROR
									);
							}
							else {
								DUMP_LDBINSP_ERRORF (pLConn, lLDBErrorCode, 
									iErrorCode << 
									pcError << 
									stSQL.c_str () << 
									LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
									LOG_CAT_AND (iLgcTbl, nLgcTbl) << 
									M4_CH_LDB_ERROR
									);
							}
						}
						else {	//Es un mensaje de error definido por el usuario. Tiene un patron concreto.
							m4id_t	nLgcTblAux = nLgcTbl;
							m4id_t	iLgcTblAux = iLgcTbl;
							//Si la cadena es vacia.
							if(!strlen(iLgcTblAux) && !strlen(nLgcTblAux)) {
								nLgcTblAux = "";
								iLgcTblAux = "";
							}

							DUMP_LDBINSP_ERRORF (pLConn, lLDBErrorCode, 
								iErrorCode << 
								pcError << 
								stSQL.c_str () << 
								LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
								LOG_CAT_AND (iLgcTblAux, nLgcTblAux)
								);

						}	//if (bChangeOnErrorMsg == M4_FALSE)

						bClose = M4_TRUE;
					}
				}


				if (iAction & M4LDB_ACT_DESTROY_STMT) {
					m_pGlobVars->SetInvStmt (M4_TRUE);
				}

				//Bug 0090288, 0092128
				//Si lo que ha pasado es que la conexion ha muerto o deseamos reintentar conectarnos
				//inhabilitamos la base de datos y en el open o el execute ya se enviara el evento de Retry!.
				//... y si no queremos un shutdown.
				if ((iAction & M4LDB_ACT_NOTIF_RETRY_CONNECTION) && ((iAction & M4LDB_ACT_SHUTDOWN) == 0) )	{
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (pLConn, "\n\tNotifying retry connection.");
					#endif

					//Desconectaremos.
					bDisconnect = M4_TRUE;

					//Inhabilitamos la BD.
					DisableDDBB ();
					DUMP_LDBINSP_WARNING (pLConn, M4_CH_LDB_DDBB_DISABLED);
				}

				/* Bug 269052
				Acción de borrado del diccionario
				*/
				if( iAction & M4LDB_ACT_FLUSH_DICTIONARY && ( iAction & M4LDB_ACT_SHUTDOWN ) == 0 )
				{
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG( pLConn, "\n\tFlush dictionary." ) ;
					#endif

					pLConn->m_pLdb->ClearAll() ;
				}

				/* Bug 0140347
				Si la conexión no es crítica no se hace shutdown
				*/
				m4return_t				iResult = M4_ERROR ;
				m4bool_t				bIsCritical = M4_TRUE ;
				stConexionProperties	stProperties ;

				iResult = pConnManager->GetConnectionInfo( m_idRConn, pLConn, stProperties ) ;

				if( iResult == M4_SUCCESS )
				{
					bIsCritical = stProperties.m_bIsCritical ;
				}

				//Bug: 0072298. El server se queda bloqueado al perder conectividad.
				//Si llegamos aqui es pq se han acabado los reintentos, avisamos al server.
				if( iAction & M4LDB_ACT_SHUTDOWN && bIsCritical == M4_TRUE )
				{
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (pLConn, "\n\tNotifying server shutdown.");
					#endif

					//Desconectaremos.
					bDisconnect = M4_TRUE;

					//Inhabilitamos la BD.
					DisableDDBB ();
					DUMP_LDBINSP_WARNING (pLConn, M4_CH_LDB_DDBB_DISABLED);

					// 655522 = "DBMS connection lost"
					//Publicacion en la log
					M4PublishEvent("65552","");

					// Notificamos al servidor que haga shutdown.
					//LDB_RYPTURE_CONNECTION =1
					
//					ClEvent oEvent(this,M4_NOTIF_DB_RYPTURE_CONNECTION);
//					pConnManager->DBRypture_Notifier.Signal(oEvent);

					m4int32_t iIdRConn = GetpGlobVars()->GetIdRConn();
					m4int32_t iIdLConn = pLConn->GetIdLConn();

					ClEventIntInt oEventII (pConnManager, M4_NOTIF_DB_RYPTURE_CONNECTION, iIdRConn, iIdLConn );
					pConnManager->DBRypture_Notifier.Signal(oEventII);
				}	//if (iAction & M4LDB_ACT_SHUTDOWN)

				delete [] pcError ;
			}
		}
	} 

	if (bClose) {
		Close ();
	}

	if (bDestroy) {
		m_pCursor->Destroy ();
	}

	if (bDisconnect) {
		pConnManager->EndConnection (m_idRConn, pLConn);
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (pLConn, "\n*************** CONNECTION %d DISCONNECTED ***************", pLConn->GetIdLConn());
		#endif
	}

	if (bDisconnectAll) {
		pConnManager->MarkAllToDisconnect (pLConn);
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (pLConn, "\n*************** ALL CONNECTIONS DISCONNECTED ***************");
		#endif
	}

	return (ai_retcode);
}


m4return_t ClStmt::SetCursor (ClLConn *ai_pLConn)
{
	m4return_t result = M4_SUCCESS;

//	m_pGlobVars->Set (ai_pLConn);

	m_pCursor = FindCursor (m_pGlobVars->GetpLConn ()->GetIdLConn ());
	
	if (!m_pCursor) {

		ClCursor *pBaseCur = GetFirstCursor ();
		
		m_pCursor = new ClCursor ();

		result = m_pCursor->Clone (pBaseCur);

		if (result == M4_ERROR) {
			delete m_pCursor;
			return (M4_ERROR);
		}

		InsertCursor (m_pGlobVars->GetpLConn ()->GetIdLConn (), m_pCursor);
	}

	return (result);
}


m4return_t ClStmt::Defuse (ClLConn *ai_pLConn)
{
	m4return_t result = M4_SUCCESS;

	m4int32_t idLConn = ai_pLConn->GetIdLConn ();

	ClCursor *pCursor = FindCursor (idLConn);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	EraseCursor (idLConn);

	if (!m_mpCursor.size ()) {
		// Esto es para mantener siempre, al menos, uno en la pila.
		ClCursor *pRefCur = new ClCursor ();

		result = pRefCur->Clone (pCursor);

		if (result == M4_ERROR) {
			delete pCursor;
			delete pRefCur;
			return (M4_ERROR);
		}

		InsertCursor (-1, pRefCur);
	}

	if (IsPartialLoad ()) {
		m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ().ErasePartialLoad (pCursor);
	}

	delete pCursor;

	return (result);
}
m4return_t
ClStmt::_ReadChannelPropertyOrSessionItem(m4int32_t ai_hItem, m4VariantType &ao_vValue)
{
	// Primero lo intentamos leer del canal. ( Necesitamos el ID_ITEM, lo cogemos del canal sesion ).
	ao_vValue.SetNull();
	if (ai_hItem==0){
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClStmt::_ReadChannelPropertyOrSessionItem" << "Null Handle in Param ");
		return (M4_ERROR);
	}

	ClChannel *pChannel = 0;
	m4pcchar_t	PropertyName=0;

	ClAccessRecordSet *pARSSession = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet();
	
	if (pARSSession==0){
		// No podemos darle valor. Retornamos exito.
		return M4_SUCCESS;
		
	}
	
	PropertyName = pARSSession->GetpAccess()->GetpCMCR()->GetItemId(ai_hItem);	
	if (PropertyName==0){
		// No podemos darle valor. Retornamos exito.
		return M4_SUCCESS;
	}
	
	pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();
//	pChannel = pARSSession->GetpAccess()->GetpChannel();

	if (pChannel->Property.GetPropValue((m4pchar_t)PropertyName,ao_vValue)==M4_SUCCESS){
		// OK. Lo hemos leido del canal.
		return M4_SUCCESS;
	}
	
	pARSSession->Current.Item[(ClHandle) ai_hItem].Value.Get (ao_vValue);
	return M4_SUCCESS;
}


void ClStmt::DisplayData (m4int32_t ai_indParam)
{
	M4LDB_BEGIN_DEBUG;
	if (!m_pParam->IsNull (ai_indParam)) {

		m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING + 1];

		m4int8_t m4Type = m_pParam->GetInfoM4Type (ai_indParam);

		m4pcchar_t pValue = m_pParam->GetDataPointer (ai_indParam);

		m4int32_t lLength = -1;

		/* Bug 0146180
		Los campos long y binarios se imprimen por trozos y teniendo en cuenta que pueden no ser
		null terminados.
		Se repasan y completan los demás tipos.
		*/
		switch (m4Type) {

			case M4CL_M4_TYPE_FIXED_STRING:
			case M4CL_M4_TYPE_UNI_VAR_STRING:
			case M4CL_M4_TYPE_BLOB:
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\"%s\"", pValue);
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\"%s\"", pValue);
				break;

			case M4CL_M4_TYPE_LONG:
			case M4CL_M4_TYPE_UNI_LONG:
			case M4CL_M4_TYPE_BINARY_STRING:

				lLength = m_pParam->GetLength (ai_indParam);

				if (lLength == M4LDB_SQL_NTS)
				{
					lLength = strlen( pValue );
				}

				if (lLength > M4LDB_SIZE_DEFAULT_STRING)
				{
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\"%.32s...%.32s\" Size = %ld ; ", pValue, &pValue[lLength-32], lLength);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\"%.32s...%.32s\" Size = %ld ; ", pValue, &pValue[lLength-32], lLength);
				}
				else
				{
					memcpy(aValue, pValue, lLength);
					aValue[ lLength ] = '\0';
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
				}
				break;

			case M4CL_M4_TYPE_DATE:
				ClDateToString (M4LDB_DOUBLE (pValue), aValue, M4LDB_SIZE_DATE + 1);
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
				break;

			case M4CL_M4_TYPE_TIMESTAMP:
				ClDateToString (M4LDB_DOUBLE (pValue), aValue, M4LDB_SIZE_TIMESTAMP + 1);
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\"%s\"", aValue);
				break;

			case M4CL_M4_TYPE_NUMBER:
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "%.20G", M4LDB_DOUBLE (pValue));							
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "%.20G", M4LDB_DOUBLE (pValue));
				break;

			default:
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "<null>");
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "<null>");
				break;
		}
	}
	else {
		M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "<null>");
		M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "<null>");
	}
	M4LDB_END_DEBUG;
}

ClParam * ClStmt::GetClParamByType( eFldType_t ai_eFldType )
{
	if (m_pVcClParam != NULL) {
		for (itVcClParam it = m_pVcClParam->begin(); it != m_pVcClParam->end(); it++)	{
			ClParam * pClParam = (*it);
			//if ((pClParam != NULL) && (pClParam->m_eFldType == ai_eFldType) && (pClParam->m_pData != NULL)) {		
			if ((pClParam != NULL) && (pClParam->m_eFldType == ai_eFldType)) {
				return pClParam;
			}
		}
	}
	return NULL;
}
