//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                ClLdb.cpp   
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


#include <stdio.h>
#include "conn_provider.hpp"
#include "clldb.hpp"
#include "m4var.hpp"
#include "access.hpp"
#include "ldbcontext.hpp"
#include "file_manager.hpp"
#include "ldbglobals.hpp"
#include "ldb_log.hpp"
#include "blocksynchronization.hpp"
#include "connmanager.hpp"
#include "m4buildversion.hpp"
#include "cachedir.hpp"
#include "m4logsys.hpp"
#include "m4unicode.hpp"


extern int	M4IsUnicode( void ) ;


#ifdef _M4LDB_DEBUG
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_pSysLConn->m_lSysDebugLevel <= GetSysDebugLevel ()) g_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DATE
	#define M4LDB_PRINT_DEBUG_DATE	if (m_pSysLConn->m_lSysDebugLevel <= GetSysDebugLevel ()) g_pInspector->PrintFileDebugDate

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_pSysLConn->m_lSysDebugLevel <= GetSysDebugLevel ()) g_pInspector->PrintFileDebugDetail
#endif


m4cint8_t *g_Literals_DDBBs[] = { "Undefined", "SQL Server","Informix","Oracle","DB2","Sybase" };
m4cint8_t *g_Literals_IsolationLevel[] = { "Undefined", "READ_UNCOMMITED","READ_COMMITED","REPEATABLE_READ","SERIALIZABLE"};
m4cint8_t *g_Literals_DebugLevel[] = { "(0) No trace", "(1) Data trace","(2) Data and MetaData trace","(3) Trace all"};
m4cint8_t *g_Literals_DebugDetailLevel[] = { "(0) No detail trace", "(1) Execution detail trace","(2) All detail trace","(3) Execution and Data detail trace", "(4) Execution and Data and Sentences detail trace"};

// -----------------------------------------------------------------------------------------
m4pcchar_t g_aPersist[] = {"", "Delete", "Insert", "Update", "", "Duplicate"};
m4pcchar_t g_aM4Type[] = {"Null", "String", "Variable String", "Long", "Date", "Timestamp", "Number", "Variant", "Currency", "Number Variant", "Blob", "Binary String", "Hour", "Unicode", "Unicode Long", "Unicode Variant"};

// Parametros globales
m4pcchar_t M4LDB_GL_PARAMS_USER_PWD								= "USER_KEY";
m4pcchar_t M4LDB_GL_PARAMS_USE_SECURITY_VIEW					= "USE_SECURITY_VIEW";
m4pcchar_t M4LDB_GL_PARAMS_EXECUTION_TIMEOUT					= "EXECUTION_TIMEOUT";
m4pcchar_t M4LDB_GL_PARAMS_ACCESS_LEVEL							= "ACCESS_LEVEL";
m4pcchar_t M4LDB_GL_PARAMS_AUDIT_TIME							= "AUDIT_TIME";
m4pcchar_t M4LDB_GL_PARAMS_AUDIT_ROWS							= "AUDIT_ROWS";

//Parámetros de aplicación
m4pcchar_t M4LDB_APP_PARAMS_APPLY_ORGANIZATION_FILTERS			= "APPLY_ORGANIZATION_FILTERS" ;
m4pcchar_t M4LDB_APP_PARAMS_TRANSLATION_AUDITORY				= "TRANSLATION_AUDITORY" ;


m4uint8_t g_iEncodingMode = M4LDB_ENCODING_ANSI;



ClLdb::ClLdb (void)
{
	m_oStmtCache.Init (this);
	m_pSysLConn = 0;
    m_pObjReg = 0;
	m_pContext = 0;
	m_poCacheDirectory = 0;
    m_pFactory = 0;
	m_numConn = 2;
	m_numStmtCache = 1024;
	m_numObject = 1024;
	m_numObjectTrans = 100;
	m_bMultFiles = M4_FALSE;
	m_lDefaultLanguage = M4LDB_DEFAULT_LANGUAGE;
	m_iDynamicLanguages = 0;
	m_bClearAll = M4_FALSE;
	m_bClearTables = M4_FALSE;
	m_bClearCache = M4_FALSE;
	m_bClearTrans = M4_FALSE;
	m_lReferences = 0;
	m_ulNumConnInUse = 0;
#ifdef _M4LDB_DEBUG
	m_pDebugDirectory = 0;
#endif
	m_contLConn28 = -2;
	m_nOperation = 0;
	m_lCompatibility = M4LDB_CLTY_UNDEFINED;
	m_lBufferSize = M4LDB_SIZE_LONG;
	m_bWarningsAreErrors = M4_FALSE;
	m_bUseSecurityView = M4_FALSE;
	m_iExecutionTimeOut = 0;
	m_iSQLAccessLevel = 0;
	m_iMultiOrgAccessLevel = 0;
	m_iAuditTime = 0;
	m_iAuditRows = 0;
	m_bRefreshInvalidConnections = M4_FALSE;	
	m_bApplyOrganizationFilters = M4_TRUE;
	m_bTranslationAuditory = M4_FALSE;
	m_iMinOrgLevel = 1 ;
}


ClLdb::~ClLdb (void)
{
	End ();
}


m4return_t ClLdb::Init (ClVMBaseEnv *ai_pObjReg, ClCMCRFactory *ai_pFactory, ILdbContext *ai_pContext)
{
	ClMutBlock oMutexBlock (ai_pContext->m_poMutexSystem);

	m4return_t result = M4_SUCCESS;

	m_pObjReg = ai_pObjReg;
	m_pFactory = ai_pFactory;
	m_pContext = ai_pContext;

	m_vcPartialLoad.Init(ai_pContext);

	if (m_pSysLConn) {
		// La conexión del sistema ya existe.
		// ¿Qué rayos hacemos aquí?
		return (M4_SUCCESS);
	}

	if (_InitCommon()!=M4_SUCCESS){
		return M4_ERROR;
	}

	m_vRefreshInvalidConnections.reserve(5);

	m_vcLogicConn.reserve(m_numConn+2); // Vamos a meter tambien la del sistema.

	// Vamos primero con la del sistema.
	if (_CreateLogicalConnection(-1,M4_FALSE)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Vamos con la del sistema. La cogemos directamente. Si hicieramos el GetConnect nos bloqueamos en el mismo semaforo que ya hemos bloqueado.
	// El status va a depender de si estamos en SM o en SERVER.
	m_vcLogicConn.back().m_bStatus = m_pContext->IsFreeSystemConnection();
	m_ulNumConnInUse++;
	
	m_pSysLConn = m_vcLogicConn.back().m_pLConn;

	if (_InitSystemConnection()!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Enh 163151.
	result = _CheckRepositoryVersion() ;

	if( result != M4_SUCCESS )
	{
		return M4_ERROR ;
	}

	// Inicializamos todas las conexiones.
	for (m4int32_t i=0;i<m_numConn;++i){
		if (_CreateLogicalConnection(i,M4_TRUE)!=M4_SUCCESS){
			return M4_ERROR;
		}
		m_vcLogicConn.back().m_pLConn->CreateSysAccess ();

	}

	// Después de esto borramos la cache de sentencias de sistema, porque estas no se volverán a usar.
	m_oStmtCache.ClearSystem ();

	// Inicializamos el contexto con el numero de conexiones normales y el numerode las del Kit.
	if (m_pContext->Init(m_numConn)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}


m4return_t ClLdb::SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory)
{
	if( m_poCacheDirectory != NULL )
	{
		m_poCacheDirectory->Detach( this ) ;
	}

	m_poCacheDirectory = ai_poCacheDirectory;

	if( m_poCacheDirectory != NULL )
	{
		m_poCacheDirectory->Attach( this ) ;
	}
	return M4_SUCCESS;
}


m4return_t ClLdb::_InitCommon(void)
{
	// Se calcula el modo de encoding
	if( M4IsUnicode() == 1 )
	{
		if( m_pObjReg->GetUseFieldUnicodeTypes() == 1 )
		{
			// Si es unicode y en el registro pone usar tipo de los campos se pone entorno mixto
			g_iEncodingMode = M4LDB_ENCODING_MIXED;
		}
		else
		{
			// Sino es, todos los campos son unicode
			g_iEncodingMode = M4LDB_ENCODING_UNICODE;
		}
	}
	else
	{
		// Si no es build unicode se está en ANSI
		g_iEncodingMode = M4LDB_ENCODING_ANSI;
	}

	m_bMultFiles = m_pObjReg->GetMultipleFilesTrace (); 
	m_numStmtCache = m_pObjReg->GetMaxNumStmtCache ();
	m_numObject = m_pObjReg->GetMaxNumObject ();
	m_numObjectTrans = m_pObjReg->GetMaxNumObjectTrans();
	m_oLDBDiskCache = m_pObjReg->GetLDBDiskCache();
	// Tranquilo, el igual está sobrecargado.
	m_oLDBDiskCache = m_pObjReg->GetCacheDirectory();
	m_iSizeLdbInspector = m_pObjReg->GetMaxLDBInspSize();
	m_lBufferSize = m_pObjReg->GetBufferSize();
	m_oStmtCache.SetMaxNumSent (m_numStmtCache);
#ifdef _M4LDB_DEBUG
	m_pDebugDirectory = m_pObjReg->GetDebugDirectory();
#endif

	// Lo pedimos al Context. Si es SM nos vendra una conexion y si no nos vendran las que haya
	m_numConn = m_pContext->GetMaxNumConn (m_pObjReg);
	return M4_SUCCESS;
}


m4return_t ClLdb::_CreateLogicalConnection(m4int32_t ai_indConn, m4bool_t ai_bIncludeDBConns)
{
#ifdef _M4LDB_DEBUG

	// LDBInspector.
	m4char_t	acDirectory[M4_LDB_MAX_PATH + 1];
	m4char_t	acFile1[M4_LDB_MAX_PATH + 1];
	m4char_t	acFile2[M4_LDB_MAX_PATH + 1];

	ClLDBInspector *pInsp = g_pInspector;

	if( pInsp != NULL && m_bMultFiles == M4_FALSE )
	{
		pInsp->Attach();
	}
	else
	{
		if( m_bMultFiles == M4_TRUE )
		{
			sprintf( acDirectory, "%s%sm4ldb", m_pDebugDirectory, M4_BACK_SLASH );

			M4MakeDirectory( acDirectory, NULL ) ;
		}
		else
		{
			strcpy( acDirectory, m_pDebugDirectory );
		}

		sprintf( acFile1, "%s%s%s%d_1.txt", acDirectory, M4_BACK_SLASH, _M4LDB_DEBUG_BASE_NAME, ai_indConn + 1 );
		sprintf( acFile2, "%s%s%s%d_2.txt", acDirectory, M4_BACK_SLASH, _M4LDB_DEBUG_BASE_NAME, ai_indConn + 1 );

		pInsp = new ClLDBInspector( acFile1, acFile2, m_iSizeLdbInspector );

		pInsp->Init( m_pContext );
		pInsp->Attach();
	}

	if( ai_indConn == -1 )
	{
		g_pInspector = pInsp;
	}

	PClLConn pLConn = new ClLConn( this, ai_indConn, &m_oConnManager, pInsp );
#else
	PClLConn pLConn = new ClLConn( this, ai_indConn, &m_oConnManager );
#endif

	struct StLogicConn stLConn;

	stLConn.m_pLConn = pLConn;
	m_vcLogicConn.push_back( stLConn );

	if( pLConn->Init() != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	// Por defecto, la conexión del diccionario
	// Nos incluimos en el Gestor de conexiones.
	// Con esto se nos crea la estructura necesaria para poder abrir otras conexiones.
	if( ai_bIncludeDBConns == M4_TRUE )
	{
		if( m_oConnManager.InsertLogicalConnection( ai_indConn ) != M4_SUCCESS )
		{
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::_InitSystemConnection(void)
{
	m4bool_t	bLoadedFromCache = M4_FALSE;

	if (m_pSysLConn == 0){
		return M4_ERROR;
	}
	if (m_pSysLConn->CreateSysAccess ()!=M4_SUCCESS){
		return M4_ERROR;
	}
	ClAccess *pAcsDict = m_pSysLConn->GetDictAccess ();

	// Lo hace el ConnectionManager.
	if (m_oConnManager.Init(m_pObjReg,pAcsDict,m_pSysLConn,m_pContext)!=M4_SUCCESS){
		return M4_ERROR;
	}

	m4VariantType oAutoload;
	oAutoload.Type = M4CL_CPP_TYPE_NUMBER;
	oAutoload.Data.DoubleData = M4_FALSE;
	
	pAcsDict->GetpChannel ()->WantsAutoLoad.Set (oAutoload);

	m_lCompatibility = M4LDB_NUMBER (pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_LOGIC_OBJECTS].RecordSet.Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_COMPATIBILITY].Value.GetWithOutSec ());

	// Cargamos el nodo de la DDBBS.
	if (m_oLDBDiskCache & M4CL_CACHE_READ){
		bLoadedFromCache = LoadDictionaryFromCache();
	}
	
	if (bLoadedFromCache==M4_FALSE){
		if (SysLoadDictio()!=M4_SUCCESS){
			return (M4_ERROR);
		}
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::SysLoadMetaDataDDBB (m4int32_t ai_indDDBB)
{
  	m4return_t result;
	ClAccess *pAcsDict = m_pSysLConn->GetDictAccess ();
   
	m4int32_t indDDBB = m_pSysLConn->FindIndexById (M4LDB_SYS_DICT_DDBBS, ai_indDDBB);
	pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet.Current[indDDBB];
	
	m4bool_t bCompleteLoad = M4LDB_BOOL (pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet.Current.Item[M4LDB_ITEM_SYS_DICT_DDBBS_COMPLETE_LOAD].Value.GetWithOutSec ());

	if (bCompleteLoad) {
		return (M4_SUCCESS);
	}
	
	// Cargamos los errores.
	result = m_pSysLConn->SysLoad (&pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ERRORS].RecordSet, NULL,M4LDB_NULL_INDEX);
	
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}
	
	m4int32_t numError = pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ERRORS].RecordSet.Count ();
			
	if (numError) {
		pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ERRORS].RecordSet.Arrange ();
	}


	// Cargamos las funciones.
	result = m_pSysLConn->SysLoad (&pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FUNCTIONS].RecordSet, NULL,M4LDB_NULL_INDEX);
	
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}
	
	m4int32_t numFunction = pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FUNCTIONS].RecordSet.Count ();
			
	if (numFunction) {
		pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FUNCTIONS].RecordSet.Arrange ();
	}

	m4VariantType vAux( 1.0 );
	pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet.Current.Item[M4LDB_ITEM_SYS_DICT_DDBBS_COMPLETE_LOAD].Value.SetWithOutSec(vAux, M4_FALSE);

	return (M4_SUCCESS);
}
	
	
m4return_t ClLdb::End (void)
{
	// Por si ya hubieramos hecho el End()
	if( m_pContext != NULL )
	{
		ClMutBlock oMutexBlock (m_pContext->m_poMutexSystem);

		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( this ) ;
		}

		m_oStmtCache.Clear ();
		m_oStmtCache.ClearSystem ();

		if (m_pSysLConn) {

			if (m_oLDBDiskCache & M4CL_CACHE_WRITE){
				if (m_pSysLConn->GetDictAccess()){
					m_pSysLConn->GetDictAccess()->GetpChannel()->PersistToFile((m4pchar_t)m_oLDBDiskCache, M4_FALSE);
				}
			}

		}
		m_pSysLConn = 0;

		m_vRefreshInvalidConnections.clear();

		for (itVcLogicConn itConn = m_vcLogicConn.begin (); itConn!=m_vcLogicConn.end (); itConn++) {

			(*itConn).m_pLConn->End ();

			delete (*itConn).m_pLConn;
		}
		m_vcLogicConn.clear();

		m_mpObject.erase (m_mpObject.begin (), m_mpObject.end ());
		m_mpObjectTrans.erase (m_mpObjectTrans.begin (), m_mpObjectTrans.end ());

		m_oConnManager.End();
		m_pObjReg = 0;
		m_pFactory = 0;
		m_pContext = 0;
	
		g_pInspector=0; // Se supone que se ha borrado ya.
	}

	return (M4_SUCCESS);
}	


m4int32_t ClLdb::FindIndexByIdObject (m4id_t ai_idLgcTbl)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexObjects);

	struct stIdObject stAux;
	stAux.m_pIdObject = ai_idLgcTbl;

	itMpObject it = m_mpObject.find (stAux);

	if (it == m_mpObject.end ())
	{
		return (M4LDB_NULL_INDEX);
	}

	return ((*it).second);
}


void ClLdb::InsertObject (m4id_t ai_idLgcTbl, m4uint32_t ai_indLgcTbl)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexObjects);

	struct stIdObject stAux;
	stAux.m_pIdObject = ai_idLgcTbl;
	
	m_mpObject.insert (MpObject::value_type (stAux, ai_indLgcTbl));

	if ((m4int32_t) m_mpObject.size () > (m4int32_t) GetNumObject ())
	{
		ClearTables ();
	}
}


m4int32_t ClLdb::FindIndexTransByIdObject (m4id_t ai_idLgcTbl)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexObjectTrans);

	struct stIdObject stAux;
	stAux.m_pIdObject = ai_idLgcTbl;

	itMpObject it = m_mpObjectTrans.find (stAux);

	if (it == m_mpObjectTrans.end ())
	{
		return (M4LDB_NULL_INDEX);
	}

	return ((*it).second);
}


void ClLdb::InsertTransObject (m4id_t ai_idLgcTbl, m4uint32_t ai_indLgcTbl)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexObjectTrans);

	struct stIdObject stAux;
	stAux.m_pIdObject = ai_idLgcTbl;
	
	m_mpObjectTrans.insert (MpObject::value_type (stAux, ai_indLgcTbl));

	if ((m4int32_t) m_mpObjectTrans.size () > (m4int32_t) GetNumObjectTrans ())
	{
		ClearTrans ();
	}
}



m4pcchar_t	ClLdb::GetStrSystemDebugDetailLevel(void)
{
	if (m_pObjReg) {
		return g_Literals_DebugDetailLevel[m_pObjReg->GetSystemDebugDetailLevel()];
	}
	return 	ERROR_LDB_NIHIL;
}


m4uint32_t ClLdb::GetSystemDebugDetailLevel (void)
{
	//En el registro tenemos un numero que hay que convertirlo a máscara.
	//Es decir, si tenemos un tres no queremos 0x011, queremos que el bit
	if (m_pObjReg) {
		return (GetSystemDebugDetailLevelMask(m_pObjReg->GetSystemDebugDetailLevel ()));
	}
	else {
		return M4LDB_DETAIL_LEVEL_SHOW_NONE;
	}
}


//Convertimos lo que leemos del registro a la mascara que entiende la BDL.
//Se hace para poder interpretar los valores que hay en el registro.
m4uint32_t ClLdb::GetSystemDebugDetailLevelMask (m4uint32_t ai_uiSystemDebugDetailLevel)
{
	m4uint32_t	iMask = M4LDB_DETAIL_LEVEL_SHOW_NONE;

	switch (ai_uiSystemDebugDetailLevel)
	{
		case 0 :	iMask = M4LDB_DETAIL_LEVEL_SHOW_NONE;
					break;

		case 1 :	iMask = M4LDB_DETAIL_LEVEL_SHOW_EXECUTION;
					break;

		case 2 :	iMask = M4LDB_DETAIL_LEVEL_SHOW_ALL_MASKS_MODE_WITHOUT_FILE;
					break;

		//Ejecucion y datos
		case 3 :	iMask = M4LDB_DETAIL_LEVEL_SHOW_EXEC_DATA;
					break;

		//Ejecucion, datos y sentencias.
		case 4 :	iMask = M4LDB_DETAIL_LEVEL_SHOW_EXEC_DATA_SENTENCES;
					break;

		//Si el valor es 5 o superior no lo traducimos, correspondera a la mascara que toque.
		default:	iMask = ai_uiSystemDebugDetailLevel;
					break;
	}

	//Limitamos el rango [5..16[. Si en el registro introducimos cualquiera de estos valores consideramos sin traza de detalle.
	//Seria como si no se hubiera puesto la entrada de registro o si su valor fuera 0.
	if ((ai_uiSystemDebugDetailLevel >= 5) && (ai_uiSystemDebugDetailLevel < M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION)) {
		iMask = M4LDB_DETAIL_LEVEL_SHOW_NONE;
	}
	//De momento dejamos inhabilitada la posibilidad de que hay detalle de fichero.
	if (ai_uiSystemDebugDetailLevel >= 5) {
		iMask = M4LDB_DETAIL_LEVEL_SHOW_NONE;
	}

	return iMask;
}


ClLConn * ClLdb::GetConnection(m4int32_t &ao_idLConn)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexConnect);

	m4int32_t indLConn = 0;
	m4int32_t numLConn = m_vcLogicConn.size ();

	if (ao_idLConn > M4LDB_NULL_INDEX) {
		for (indLConn=0; indLConn<numLConn; indLConn++) {
			if (m_vcLogicConn[indLConn].m_pLConn->GetIdLConn () == ao_idLConn) {
				if (m_vcLogicConn[indLConn].m_bStatus == M4_TRUE) {
					m_vcLogicConn[indLConn].m_bStatus = M4_FALSE;
					m_ulNumConnInUse+=m_pContext->IncrementUsedLogicConn();
					return (m_vcLogicConn[indLConn].m_pLConn);
				}
				break;
			}
		}
	}

	// Buscamos la primera conexión lógica libre.
	for (indLConn=0; indLConn<numLConn; indLConn++) {
		if (m_vcLogicConn[indLConn].m_bStatus == M4_TRUE) {
			m_vcLogicConn[indLConn].m_bStatus = M4_FALSE;
			m_ulNumConnInUse+=m_pContext->IncrementUsedLogicConn();
			ao_idLConn = m_vcLogicConn[indLConn].m_pLConn->GetIdLConn ();
			return (m_vcLogicConn[indLConn].m_pLConn);
		}
	}

	// No hay ninguna libre.
	ao_idLConn = M4LDB_NULL_INDEX;
	return NULL;
}


m4return_t ClLdb::FreeConnection (m4int32_t ao_idLConn, m4bool_t ai_bIgnoreChannel)
{
	m4return_t ret = M4_SUCCESS;
	ClMutBlock oMutexBlock (m_pContext->m_poMutexConnect);

	m4int32_t indLConn = 0;
	m4int32_t numLConn = m_vcLogicConn.size ();
	
	for (indLConn=0; indLConn<numLConn; indLConn++) {
		if (m_vcLogicConn[indLConn].m_pLConn->GetIdLConn () == ao_idLConn) {
			if (m_vcLogicConn[indLConn].m_bStatus == M4_FALSE) {
				m4bool_t bFree;
				ClLConn *pLConn = m_vcLogicConn[indLConn].m_pLConn;
				pLConn->SetIgnoreChannel (ai_bIgnoreChannel);
				pLConn->EndTransaction (NULL, M4_FALSE, bFree);
				ret = pLConn->ReleaseConnection ();
				pLConn->SetIgnoreChannel (M4_FALSE);
				m_vcLogicConn[indLConn].m_bStatus = M4_TRUE;
				m_ulNumConnInUse-=m_pContext->DecrementUsedLogicConn();
				//Cada vez que liberamos una conexion logica vamos a comprobar si hay que liberar alguna conexion
				//fisica dinamica!. Se destruiran SOLO las que estan ready para eliminar.
				m_oConnManager.DestroyDynamicConnection( M4_FALSE );
				//Despues de destuir es cuando podemos resetear la sesion.
				pLConn->SetIdSession( 0 );
			}
			break;
		}
	}
	return ret;
}


m4return_t ClLdb::_Clear (void)
{
	if( m_bClearAll == M4_TRUE )
	{
		_ClearAll();
	}
	else
	{
		if( m_bClearTables == M4_TRUE )
		{
			_ClearTables();
		}

		if( m_bClearCache == M4_TRUE )
		{
			_ClearCache( M4_FALSE );
		}

		if( m_bClearTrans == M4_TRUE )
		{
			_ClearTrans();
		}
	}

	//Bug 0090288, 0092128
	//Vamos a refrescar conexiones invalidas cuando no haya nadie dentro!.
	if( m_bRefreshInvalidConnections == M4_TRUE )
	{
		_RefreshInvalidConnections();
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::_ClearAll (void)
{
	if( m_oStmtCache.GetStmtUsed() > 0 )
	{
		return M4_SUCCESS;
	}

	m_bClearAll = M4_FALSE;

    m4return_t result = _ClearTrans ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = _ClearTables ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

#ifdef _M4LDB_DEBUG
	m4int32_t lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG_DATE(m_pSysLConn, "\n\n*************** DELETING DICTIONARY META4OBJECT.\t\t\t\t\t\t\t\t\t***************");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	ClAccess *pAcsDict = m_pSysLConn->GetDictAccess ();
	pAcsDict->GetpChannel()->ReleaseAll();

#ifdef _M4LDB_DEBUG
	lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG_DATE(m_pSysLConn, "\n\n*************** DELETING NOT USED CONNECTIONS.\t\t\t\t\t\t\t\t\t\t***************");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	m4int32_t numLConn = m_vcLogicConn.size ();

	for (m4int32_t indLConn=0; indLConn<numLConn; indLConn++) {
		m_vcLogicConn[indLConn].m_pLConn->FreeConnections ();
	}

	//Tambien tratamos las conexiones físicas dinámicas!.
	m_oConnManager.DisconnectAllDynamicConnections();

	//Bug 0077759
	//En el conexion manager tb está. Pero es necesario cargar el nodo por si me borran las caches!.
	//Lo que haremos es destruirlo y cargarlo de nuevo para que no haya doble carga.
	//Nodo de Conexiones	
	if (m_pSysLConn->SysLoad (&pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_CONNECTIONS].RecordSet, NULL,M4LDB_NULL_INDEX)==M4_ERROR){
		return (M4_ERROR);
	}

	// Volvemos a activar los indices.
	m_pSysLConn->SetCurrentDictIndexes ();

	result = SysLoadDictio ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClLdb::_ClearTables (void)
{
	if( m_oStmtCache.GetStmtUsed() > 0 )
	{
		return M4_SUCCESS;
	}

	m_bClearTables = M4_FALSE;

	m4return_t result = _ClearCache (M4_TRUE);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

#ifdef _M4LDB_DEBUG
	m4int32_t lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG(m_pSysLConn, "\n\n*************** DELETING LOGIC OBJECTS.");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif
	
	m4int32_t numObj = m_mpObject.size();
	m_mpObject.erase( m_mpObject.begin(), m_mpObject.end() );

	m_pSysLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Destroy();

#ifdef _M4LDB_DEBUG
	lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG_DATE(m_pSysLConn, " NUMBER OF DELETED OBJECTS = %ld.\t\t\t\t***************", numObj);
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	return (M4_SUCCESS);
}


m4return_t ClLdb::_ClearCache (m4bool_t ai_bAll)
{
	m_bClearCache = M4_FALSE;

#ifdef _M4LDB_DEBUG
	m4int32_t lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG(m_pSysLConn, "\n\n*************** DELETING SENTENCES.");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	// No es necesario usar el mutex de la cache porque sabemos que no puede haber nadie dentro (por el contador de referencias).
	m4int32_t numStmt = m_oStmtCache.EraseSentsNoUsed(ai_bAll);

#ifdef _M4LDB_DEBUG
	lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG_DATE(m_pSysLConn, " NUMBER OF DELETED SENTENCES = %ld.\t\t\t\t***************", numStmt);
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	return (M4_SUCCESS);
}


m4return_t ClLdb::_ClearTrans (void)
{
	m_bClearTrans = M4_FALSE;

#ifdef _M4LDB_DEBUG
	m4int32_t lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG(m_pSysLConn, "\n\n*************** DELETING TRANSLATION META4OBJECT.");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif
	
	m4int32_t numObj = m_mpObjectTrans.size ();
	m_mpObjectTrans.erase( m_mpObjectTrans.begin(), m_mpObjectTrans.end() );
	m_pSysLConn->m_oNameTrans.GetAccess()->GetpChannel()->ReleaseAll();

#ifdef _M4LDB_DEBUG
	lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1;
	M4LDB_PRINT_DEBUG_DATE(m_pSysLConn, " NUMBER OF DELETED RECORDS = %ld.\t***************", numObj);
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel;
#endif

	return (M4_SUCCESS);
}


// Funciones para la Versión 2.8....
ClLConn *ClLdb::GetConnection28 (m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexConnect);

	if (_CreateLogicalConnection(m_contLConn28,M4_TRUE)!=M4_SUCCESS){
		return NULL;
	}
	
	ClLConn *pLConn = m_vcLogicConn.back().m_pLConn;
	
	pLConn->CreateSysAccess ();

	// Se lo pido al conecction manager
	stConexionProperties stConSysProp;		

	m_oConnManager.GetConnectionInfo(0,m_pSysLConn,stConSysProp);

	stConSysProp.m_stConnStr = ai_pConnStr;

	stConSysProp.m_stConnStr.append (";UID=");

	stConSysProp.m_stConnStr.append (ai_pUser);

	stConSysProp.m_stConnStr.append (";PWD=");
	
	stConSysProp.m_stConnStr.append (ai_pPassword);

	if ((m_oConnManager.ValidateConnectionProperties(m_contLConn28,stConSysProp,pLConn)==M4_SUCCESS) && 
		(m_oConnManager.InsertOneEntryInGenericMap(m_contLConn28,stConSysProp)!=M4_SUCCESS)) {
		m_contLConn28--;
		m_vcLogicConn.back().m_bStatus = M4_FALSE;
		m_ulNumConnInUse+=m_pContext->IncrementUsedLogicConn();
		return (pLConn);
	}

	DUMP_LDBINSP_ERRORF (pLConn,M4_CH_LDB_INVALID_DB_CONNECTION,m_contLConn28);
	return NULL;
}


m4return_t ClLdb::FreeConnection28 (ClLConn *&ao_pLConn)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexConnect);

	m4bool_t bFree;
	if (ao_pLConn) {
//		ao_pLConn->SetIgnoreChannel (ai_bIgnoreChannel);
		ao_pLConn->EndTransaction (NULL, M4_FALSE, bFree);
		ao_pLConn->ReleaseConnection ();
		ao_pLConn->SetIgnoreChannel (M4_FALSE);
		m_ulNumConnInUse-=m_pContext->DecrementUsedLogicConn();
		m_oStmtCache.DefuseSent (ao_pLConn);
		m_oStmtCache.DefuseSentSystem (ao_pLConn);

		for (itVcLogicConn it = m_vcLogicConn.begin (); it!=m_vcLogicConn.end (); it++) {
			if ((*it).m_pLConn == ao_pLConn) {
				m_ulNumConnInUse+=m_pContext->IncrementUsedLogicConn();
				m_vcLogicConn.erase (it);
				break;
			}
		}

		delete ao_pLConn;
	}

	ao_pLConn = NULL;

	return (M4_SUCCESS);
}


m4return_t ClLdb::RefreshObjectMapFromDictionary(void)
{
	struct stIdObject stNew;
	
	m4int32_t numreg = m_pSysLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Count();

	for (m4int32_t i = 0;i<numreg ;++i){
		m_pSysLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(i);
		stNew.m_pIdObject = M4LDB_ID (m_pSysLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
		m_mpObject.insert (MpObject::value_type (stNew, (m4uint32_t) i));
	}
	return M4_SUCCESS;
}


m4bool_t ClLdb::LoadDictionaryFromCache(void)
{
	ClAccess *pAcsDict = 0;

	if (ClFileName(m_oLDBDiskCache).IsFile()){
		pAcsDict=m_pSysLConn->GetDictAccess ();
		if (pAcsDict ->GetpChannel()->LoadFromFile(m_oLDBDiskCache)==M4_SUCCESS){
			// Actualizamos los indices del diccionario. ( El Load From File se los carga ).
			m_pSysLConn->SetCurrentDictIndexes();
			// Hay que actualizar el objeto m_mpObject, con los objetos que tenemos en el diccionario.
			if (RefreshObjectMapFromDictionary()!=M4_SUCCESS){
				pAcsDict ->UnLoad();
				return M4_FALSE;
			}
			return M4_TRUE;
		}
	}
	return M4_FALSE;
}


// 05-10-99 RAC.
// ------------------------------------------------ Interfaz de acceso a datos CurrentInfo.  

m4uint32_t	ClLdb::GetNumConnInUse()
{
	return m_ulNumConnInUse;
}

m4uint32_t	ClLdb::GetNumLogicObjectInUse()
{
	return m_mpObject.size();
}

m4uint32_t	ClLdb::GetNumStatmntCacheInUse()
{
	return m_oStmtCache.GetSentCacheSize();
}

m4pcchar_t	ClLdb::GetIsolationLevel()
{ 
	return g_Literals_IsolationLevel[(eIsolationLevel_t) m_pObjReg->GetIsolationLevel ()];
}

m4pcchar_t	ClLdb::GetDataBaseType()
{
	return g_Literals_DDBBs[(eDDBBType_t) m_pObjReg->GetDataBase()];
}

m4pcchar_t	ClLdb::GetDebugLevel()
{
	return g_Literals_DebugLevel[m_pObjReg-> GetSystemDebugEnable()];
}



// ------------------------------------------------ Metodos 

m4return_t	ClLdb::SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel)
{
	m_pObjReg -> SetSystemDebugEnable(ai_uiSysDebugLevel);
	return M4_SUCCESS;
}
	

//Detalle de trazas en ldbinsp.
m4return_t	ClLdb::SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel)
{
	if (m_pObjReg)
	{
		m_pObjReg->SetSystemDebugDetailLevel(ai_uiSystemDebugDetailLevel);
	}
	return M4_SUCCESS;
}
// ---------------------------------------------------------------------------------------


void ClLdb::Attach (void)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexSystem);

	if( m_lReferences == 0 )
	{
		// No hay nadie dentro de ninguna conexión.
		_Clear();
	}

	m_lReferences++;
}


void ClLdb::Detach (void)
{
	ClMutBlock oMutexBlock (m_pContext->m_poMutexSystem);

	m_lReferences--;

	if( m_lReferences == 0 )
	{
		// No hay nadie dentro de ninguna conexión.
		_Clear();
	}
}


m4return_t ClLdb::RefreshOrgTree(void)
{

	ClAccess			*pAcsDict = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;


	ClMutBlock oMutexBlock( m_pContext->m_poMutexLoadDict ) ;

	pAcsDict = m_pSysLConn->GetDictAccess ();
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_TREE].RecordSet ;

#ifdef _M4LDB_DEBUG
	m4int32_t lSysDebugLevel = m_pSysLConn->m_lSysDebugLevel ;
	m_pSysLConn->m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_1 ;
	M4LDB_PRINT_DEBUG(m_pSysLConn, "\n\n*************** DELETING ORGANIZATION TREE.\t**************");
	m_pSysLConn->m_lSysDebugLevel = lSysDebugLevel ;
#endif

	poRecordSet->Destroy() ;

	if( SysLoadOrgTree( pAcsDict ) != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::SysLoadOrgTree( ClAccess *ai_poAcsDict )
{

	ClAccessRecordSet	*poRecordSet = NULL ;


	//Cargamos el nodo del árbol de organización
	poRecordSet = &ai_poAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_TREE].RecordSet ;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL,M4LDB_NULL_INDEX) == M4_ERROR )
	{
		return M4_ERROR;
	}

	//Problemas de multithread. Este nodo tiene índices y es necesario ordenarlo.
	poRecordSet->Arrange();

	// Se recorre el nodo calculando duplicados
	// Se recorre de alante atrás y de atrás adelante
	m4int16_t		i = 0 ;
	m4int16_t		j = 0 ;
	m4int16_t		iLength = 0 ;
	m4int32_t		iLevel = -1 ;
	m4int32_t		iLastLevel = -1 ;
	m4pcchar_t		pccOrganization = NULL ;
	m4pcchar_t		pccLastOrganization = NULL ;
	m4VariantType	vAux ;

	vAux.Type = M4CL_CPP_TYPE_NUMBER ;
	vAux.Data.DoubleData = 1 ;

	iLength = (m4int16_t) poRecordSet->Count() ;

	for( j = 0 ; j < 2 ; j++ )
	{
		iLastLevel = -1 ;
		pccLastOrganization = NULL ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( j == 0 )
			{
				poRecordSet->Current[ i ] ;
			}
			else
			{
				poRecordSet->Current[ iLength - i - 1 ] ;
			}

			pccOrganization = M4LDB_PCSTR (poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_ORGANIZATION].Value.GetWithOutSec ());
			iLevel = M4LDB_NUMBER (poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_LEVEL].Value.GetWithOutSec ());

			if( pccLastOrganization != NULL && pccOrganization != NULL )
			{
				if( strcmpi( pccOrganization, pccLastOrganization ) == 0 && iLevel == iLastLevel )
				{
					poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_TREE_IS_DUPLICATED].Value.SetWithOutSec(vAux, M4_FALSE);
				}
			}

			pccLastOrganization = pccOrganization ;
			iLastLevel = iLevel ;
		}
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::SysLoadDictio(void)
{

    m4uint32_t			i = 0 ;
    m4uint32_t			iLength = 0 ;
    m4int32_t			iOrgLevel = 0 ;
	ClAccess			*pAcsDict = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;


	pAcsDict = m_pSysLConn->GetDictAccess ();

	// Nodo de DDBB.
	if( m_pSysLConn->SysLoad(&pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet, NULL,M4LDB_NULL_INDEX ) == M4_ERROR )
	{
		return (M4_ERROR);
	}

	// Reordenamos el nodo de bases de datos.
	pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet.Arrange();


	// Nodos de funciones y errores de la conexion por defecto.
	if( SysLoadMetaDataDDBB((m4int32_t)  m_pObjReg->GetDataBase() ) == M4_ERROR )
	{
		return (M4_ERROR);
	}

	//Lo que se va a hacer es que en el borrado de las caches del diccionario cargaremos el nodo de conexiones,
	//la razon es que sólo se están cargando una vez cuando se inicia la conexion.


	// Nodo de Extended Types.
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_EXTENDED_TYPES].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX ) == M4_ERROR )
	{
		return (M4_ERROR);
	}

	// Reordenamos el nodo de extended_types.
	poRecordSet->Arrange();


	// Cargamos los errores y las funciones de las distintas DDBBs.
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_CONNECTIONS].RecordSet;

	iLength = poRecordSet->Count();

	for( i = 0; i < iLength; i++ )
	{
		poRecordSet->Current[i];

		m4pcchar_t pConnStr = M4LDB_PCSTR( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_CONN_STR].Value.GetWithOutSec() );

		if (*pConnStr)
		{
			eDDBBType_t eDDBB = (eDDBBType_t) M4LDB_NUMBER( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_DDBB].Value.GetWithOutSec() );

			if( SysLoadMetaDataDDBB( m4int32_t(eDDBB) ) == M4_ERROR )
			{
				return M4_ERROR;
			}
		}
	}

	// Se cargan los lenguajes
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DEFAULT_LANGUAGE].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX) == M4_ERROR )
	{
		return M4_ERROR;
	}

	// Se leen los lenguajes
	if( poRecordSet->Register.Begin() == M4_TRUE )
	{
		m_lDefaultLanguage = (m4uint32_t) M4LDB_NUMBER( poRecordSet->Register.Item[M4LDB_ITEM_SYS_DICT_DEFAULT_LANGUAGE_ID_LANGUAGE].Value.GetWithOutSec() );
	}

	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DYNAMIC_LANGUAGES].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL,M4LDB_NULL_INDEX) == M4_ERROR )
	{
		return M4_ERROR;
	}

	m_iDynamicLanguages = poRecordSet->Count();
	poRecordSet->Arrange();

	if( m_iDynamicLanguages > 0 )
	{
		for( i = 0; i < m_iDynamicLanguages; i++ )
		{
			poRecordSet->Register[ i ] ;

			m4int32_t	iLanguage = M4LDB_NUMBER( poRecordSet->Register.Item[ M4LDB_ITEM_SYS_DICT_DYNAMIC_LANGUAGES_ID_LANGUAGE ].Value.GetWithOutSec() ) ;
			LogsysAddLanguage( iLanguage, 0, NULL, NULL, NULL ) ;
		}
	}


	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_GLOBAL_PARAMS].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX ) == M4_ERROR )
	{
		return M4_ERROR;
	}

	//Problemas de multithread. Este nodo tiene índices y es necesario ordenarlo.
	poRecordSet->Arrange();


	//Cargamos el nodo de parámetros de aplicación LOGÓN-SCH_SESSION
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_APPLICATION_PARAMS].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX ) == M4_ERROR )
	{
		return M4_ERROR;
	}

	//Problemas de multithread. Este nodo tiene índices y es necesario ordenarlo.
	poRecordSet->Arrange();

	/* Bug 0276677
	Se releen los parámetros después de un flush
	*/
	if (_UpdateParamInfo(pAcsDict)!=M4_SUCCESS){
		return (M4_ERROR);
	}


	//Cargamos el nodo del árbol de organización
	if( SysLoadOrgTree( pAcsDict ) != M4_SUCCESS )
	{
		return M4_ERROR;
	}


	//Cargamos el nodo de niveles de organización
	poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_LEVELS].RecordSet;

	if( m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX) == M4_ERROR )
	{
		return M4_ERROR;
	}

	//Problemas de multithread. Este nodo tiene índices y es necesario ordenarlo.
	poRecordSet->Arrange();

	// Se calcula el nivel mínimo
	m_iMinOrgLevel = 1 ;

	iLength = poRecordSet->Count();

	if( iLength > 0 )
	{
		for( i = 0; i < iLength; i++ )
		{
			poRecordSet->Current[i];

			iOrgLevel = M4LDB_NUMBER( poRecordSet->Current.Item[ M4LDB_ITEM_SYS_DICT_ORG_LEVELS_ID_LEVEL ].Value.GetWithOutSec() ) ;

			/* Bug 0259854
			Se ignora el nivel tecnológico para el cálculo del mínimo
			*/
			if( iOrgLevel < m_iMinOrgLevel && iOrgLevel != M4LDB_ORG_LEVEL_TECHNOLOGY )
			{
				m_iMinOrgLevel = iOrgLevel ;
			}
		}

		if( m_iDynamicLanguages > 0 )
		{
			//Cargamos el nodo de traducción de niveles de organización
			poRecordSet = &pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_LEVELS_TRANS].RecordSet;

			/* Bug 0198984
			No se chequea el error por si la tabla de idiomas no está creada
			*/
			m_pSysLConn->SysLoad(poRecordSet, NULL, M4LDB_NULL_INDEX);

			//Problemas de multithread. Este nodo tiene índices y es necesario ordenarlo.
			poRecordSet->Arrange();
		}
	}

	return M4_SUCCESS;
}


m4return_t ClLdb::_UpdateParamInfo(ClAccess *pAcsDict)
{
	// Buscamos los parametros.
	m4int32_t	iParamIndex = M4LDB_NULL_INDEX;
	m4int32_t	iLength = -1;
	m4int32_t	iExecutionTimeOut = 0;
	m4int32_t	iAuditTime = 0;
	m4int32_t	iAuditRows = 0;
	m4pcchar_t	pCrypt_User_key = NULL;
	m4pcchar_t	pcUseSecurityView = NULL;
	m4pcchar_t	pcExecutionTimeOut = NULL;
	m4pcchar_t	pcAccessLevel = NULL;
	m4pcchar_t	pcAuditTime = NULL;
	m4pcchar_t	pcAuditRows = NULL;
	m4pcchar_t	pccAppValue = NULL;
	
	// Parametro 1 : USER_KEY
	//-----------------------------------------------------------------------------------
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_USER_PWD);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pCrypt_User_key = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		// Todo correcto. Actualizamos la Key para encriptación de datos.
		if (_SetUserKey(pCrypt_User_key)!=M4_SUCCESS){
			DUMP_LDBINSP_ERROR(m_pSysLConn,M4_CH_LDB_WRONG_DATA_KEY);
			return M4_ERROR;
		}
	}

	// Parametro 2 : M4LDB_GL_PARAMS_USE_SECURITY_VIEW
	//-----------------------------------------------------------------------------------
	m_bUseSecurityView = M4_FALSE;
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_USE_SECURITY_VIEW);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pcUseSecurityView = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		if (strcmp(pcUseSecurityView, "1") == 0){
			m_bUseSecurityView = M4_TRUE;
		}
	}

	// Parametro 3 : M4LDB_GL_PARAMS_EXECUTION_TIMEOUT
	//-----------------------------------------------------------------------------------
	m_iExecutionTimeOut = 0;
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_EXECUTION_TIMEOUT);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pcExecutionTimeOut = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		if( pcExecutionTimeOut != NULL )
		{
			iExecutionTimeOut = atoi( pcExecutionTimeOut );

			if( iExecutionTimeOut < 0 || iExecutionTimeOut > 86400 )
			{
				DUMP_CHLOG_ERRORF( M4_CH_LDB_WRONG_PARAM_VALUE, "EXECUTION TIME OUT" << "0" << pcExecutionTimeOut );
			}
			else
			{
				m_iExecutionTimeOut = iExecutionTimeOut;
			}

#ifdef _WINDOWS
			if( m_iExecutionTimeOut > 0 )
			{
				// Se incrementa el tiempo de refresco para el polling activo
				timeBeginPeriod( 1 ) ;
			}
#endif
		}
	}

	// Parametro 4 : M4LDB_GL_PARAMS_ACCESS_LEVEL
	//-----------------------------------------------------------------------------------
	m_iMultiOrgAccessLevel = 0;
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_ACCESS_LEVEL);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pcAccessLevel = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		if( pcAccessLevel != NULL )
		{
			iLength = strlen( pcAccessLevel );

			if( iLength > 0 )
			{
				if( pcAccessLevel[0] >= '0' && pcAccessLevel[0] <= '2' )
				{
					m_iSQLAccessLevel = pcAccessLevel[0] - '0';
				}
				else
				{
					DUMP_CHLOG_ERRORF( M4_CH_LDB_WRONG_PARAM_VALUE, "SQL CONTROL" << "0" << pcAccessLevel[0] );
				}

				if( iLength > 1 )
				{
					if( pcAccessLevel[1] >= '0' && pcAccessLevel[1] <= '2' )
					{
						m_iMultiOrgAccessLevel = pcAccessLevel[1] - '0';
					}
					else
					{
						DUMP_CHLOG_ERRORF( M4_CH_LDB_WRONG_PARAM_VALUE, "ORGANIZATION CONTROL" << "0" << pcAccessLevel[1] );
					}
				}
			}
		}
	}

	// Parametro 5 : M4LDB_GL_PARAMS_AUDIT_TIME
	//-----------------------------------------------------------------------------------
	m_iAuditTime = 0;
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_AUDIT_TIME);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pcAuditTime = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		if( pcAuditTime != NULL )
		{
			iAuditTime = atoi( pcAuditTime );

			if( iAuditTime < 0 )
			{
				DUMP_CHLOG_ERRORF( M4_CH_LDB_WRONG_PARAM_VALUE, "AUDIT TIME" << "0" << pcAuditTime );
			}
			else
			{
				m_iAuditTime = iAuditTime;
			}
		}
	}

	// Parametro 6 : M4LDB_GL_PARAMS_AUDIT_ROWS
	//-----------------------------------------------------------------------------------
	m_iAuditRows = 0;
	iParamIndex = m_pSysLConn->FindIndexById(M4LDB_SYS_DICT_GLOBAL_PARAMS, M4LDB_GL_PARAMS_AUDIT_ROWS);
	if (iParamIndex != M4LDB_NULL_INDEX)
	{
		// Encontrado. Leemos el valor
		pcAuditRows = (m4pcchar_t)m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.Get();

		if( pcAuditRows != NULL )
		{
			iAuditRows = atoi( pcAuditRows );

			if( iAuditRows < 0 )
			{
				DUMP_CHLOG_ERRORF( M4_CH_LDB_WRONG_PARAM_VALUE, "AUDIT ROWS" << "0" << pcAuditRows );
			}
			else
			{
				m_iAuditRows = iAuditRows;
			}
		}
	}

	//-----------------------------------------------------------------------------------
	
	//Parámetros de aplicación
	//Enh 0116664:
	//-----------------------------------------------------------------------------------
	//Buscamos el parámetro APPLY_ORGANIZATION_FILTERS
	m_bApplyOrganizationFilters = M4_TRUE;
	iParamIndex = m_pSysLConn->FindIndexById( M4LDB_SYS_DICT_APPLICATION_PARAMS, M4LDB_APP_PARAMS_APPLY_ORGANIZATION_FILTERS );

	if( iParamIndex != M4LDB_NULL_INDEX )
	{
		pccAppValue = (m4pcchar_t) m_pSysLConn->m_paRS[M4LDB_SYS_DICT_APPLICATION_PARAMS]->Register[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_APPLICATION_PARAMS_APP_VALUE].Value.GetWithOutSec();
		
		if( pccAppValue != NULL && strcmpi( pccAppValue, "0" ) == 0 )
		{
			m_bApplyOrganizationFilters = M4_FALSE;
		}				
	}

	m_bTranslationAuditory = M4_FALSE;
	iParamIndex = m_pSysLConn->FindIndexById( M4LDB_SYS_DICT_APPLICATION_PARAMS, M4LDB_APP_PARAMS_TRANSLATION_AUDITORY );

	if( iParamIndex != M4LDB_NULL_INDEX )
	{
		pccAppValue = (m4pcchar_t) m_pSysLConn->m_paRS[M4LDB_SYS_DICT_APPLICATION_PARAMS]->Register[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_APPLICATION_PARAMS_APP_VALUE].Value.GetWithOutSec();
		
		if( pccAppValue != NULL && strcmpi( pccAppValue, "1" ) == 0 )
		{
			m_bTranslationAuditory = M4_TRUE;
		}				
	}


	return M4_SUCCESS;
}


m4return_t ClLdb::_SetUserKey(m4pcchar_t ai_pUserKey)
{
	if( ai_pUserKey == NULL )
	{
		return M4_SUCCESS;
	}

	m4pcchar_t pUserLicence = m_pObjReg->GetUserLicense();

	if( pUserLicence == NULL )
	{
		pUserLicence = "";
	}

	// Nos viene LicenciaEncriptada+UserKeyencriptada.
	m4int32_t LenLicence = strlen(pUserLicence);
	m4int32_t LenKey = strlen(ai_pUserKey);

	if( LenLicence > LenKey )
	{
		return M4_ERROR;
	}

	if( strncmp( pUserLicence, ai_pUserKey, LenLicence ) )
	{
		// La licencia es distinta de la que hay grabada en base de datos. Error
		return M4_ERROR;
	}

	return( M4SetUserKey( ai_pUserKey+LenLicence) ) ;
}


m4int16_t ClLdb::GetNumConn()
{
	return m_pContext->GetNumConn();
}


m4return_t ClLdb::RefreshInvalidConnections( m4int32_t ai_indDBConn,m4int32_t ai_indLConn )
{
	//Esta función la invoca el subsistema para indicar que se recuperen conexiones.
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pSysLConn, " Critical connections will be recovered.\n" );
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, m_pSysLConn, " Critical connections will be recovered.\n" );
	#endif

	//Nos apuntamos que tenemos que refescar conexiones y lo metemos en el mapa.
	ClRefreshInvalidConnections oRefreshInvalidConnections( ai_indDBConn, ai_indLConn );
	m_vRefreshInvalidConnections.push_back( oRefreshInvalidConnections );
	m_bRefreshInvalidConnections = M4_TRUE;

	return M4_SUCCESS;
}


m4return_t ClLdb::_RefreshInvalidConnections( void )
{
	m4return_t result = M4_SUCCESS;

	//Esta funcion se invoca desde el Attach/Dettach por lo tanto esta protegida com semaforos. Se ejecutara cuando no
	//haya nadie en la bdl.
	//Recorremos el vector de conexiones a refrescar y las refrescamos.
	//Nos apuntamos que tenemos que refescar conexiones y lo metemos en el mapa.
	for (itVcClRefreshInvalidConnections it = m_vRefreshInvalidConnections.begin(); it != m_vRefreshInvalidConnections.end(); ++it) {
		result = _RefreshInvalidConnections( (*it).m_indDBConn, (*it).m_indLConn );
		if (result != M4_SUCCESS) {			
			break;
		}
	}
	
	//Limpiamos el vector y reseteamos el flag.
	m_vRefreshInvalidConnections.clear();
	m_bRefreshInvalidConnections = M4_FALSE;

	return result;
}


m4return_t ClLdb::_RefreshInvalidConnections( m4int32_t ai_indDBConn, m4int32_t ai_indLConn )
{
	// Me recorro el vector de conexiones logicasbuscando la que me piden.
	itVcLogicConn it = m_vcLogicConn.begin();
	itVcLogicConn itend = m_vcLogicConn.end();

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// OJO CON LO QUE HAYA QUE PROTEGER CON SEMAFOROS

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pSysLConn, " Attempting to recover critical connections.\n" );
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, m_pSysLConn, " Attempting to recover critical connections.\n" );
	#endif

	for (;it!=itend;++it){
		ClLConn *pConn = (*it).m_pLConn;
		if (pConn->GetIdLConn()==ai_indLConn) {
			pConn->m_pLogMCR = NULL;
			pConn->m_pLogAccess = NULL;
			pConn->m_hLogNode = NULL;
			if (m_oConnManager.RefreshInvalidConnections(ai_indDBConn, pConn)!=M4_SUCCESS){
				// ¿Que pasa con el "Esto sucedio en .. de ..".
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pSysLConn," Error trying to recover physical connection %d with DDBB in the logical connection %d.\n",ai_indDBConn, pConn->GetIdLConn());
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, m_pSysLConn," Error trying to recover physical connection %d with DDBB in the logical connection %d.\n",ai_indDBConn, pConn->GetIdLConn());
			#endif
				return M4_ERROR;
			}
		}
	}
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pSysLConn, " Success recovering critical connections.\n" );
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, m_pSysLConn, " Success recovering critical connections.\n" );
	#endif
	return M4_SUCCESS;
}


void ClLdb::Attach_DBRyptureConnection(ClNotifiable *ai_pNotifiable)
{
	m_oConnManager.DBRypture_Notifier.Attach(ai_pNotifiable);
}


m4return_t ClLdb::_ExecuteInternalCommand (m4pcchar_t ai_pCommand)
{
	if (!strcmp (ai_pCommand, "CLEARALL")) {
		return (ClearAll ());
	}
	if (!strcmp (ai_pCommand, "CLEARCACHE")) {
		return (ClearCache ());
	}
	if (!strcmp (ai_pCommand, "WARNINGSAREERRORS")) {
		if (m_bWarningsAreErrors == M4_TRUE) {
			m_bWarningsAreErrors = M4_FALSE;
		}
		else {
			m_bWarningsAreErrors = M4_TRUE;
		}
		return (M4_SUCCESS);
	}
	return (M4_WARNING);
}


//Este miembro lo invocarán desde fuera para indicar que un usuario ha finalizado!.
//La conexion fisica que se había creado hay que marcarla como lista para eliminar.
void ClLdb::UserSessionClosedByIdSession( m4uint64_t ai_iIdSession )
{
	//Es necesario mutex para bloquear a todo el mundo para actualizar el valor del flag de logoff.
	ClMutBlock oMutexBlock (m_pContext->m_poMutexConnect);

	//Buscaremos en el mapa todas las conexiones que puede tener esa sesion y las tendremos que marcar para borrar.
	m_oConnManager.SetReadyToFreeDynamicConnectionByIdSession(ai_iIdSession, M4_TRUE );
}


m4return_t	ClLdb::_CheckRepositoryVersion()
{
	m4return_t	iRet = M4_ERROR ;
	m4int32_t	iParamIndex ;
	m4pcchar_t	pccParamValue ;
	m4pcchar_t	pccMindVersion ;
	m4pchar_t	pcVersionsList ;
	m4pchar_t	pcSingleVersion ;


	// Recuperamos la versión tecnológica.
	pccMindVersion = GetMindVersion() ;


	// Recuperamos la versión del repositorio.
	iParamIndex = m_pSysLConn->FindIndexById( M4LDB_SYS_DICT_GLOBAL_PARAMS, GetContext()->GetVersionCheckingKey() ) ;

	if( iParamIndex == M4LDB_NULL_INDEX )
	{
		DUMP_CHLOG_ERROR( M4_CH_LDB_MISMATCHED_VERSIONS ) ;
		return M4_ERROR ;
	}

	pccParamValue = (m4pcchar_t) m_pSysLConn->m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->Current[iParamIndex].Item[M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE].Value.GetWithOutSec() ;
	
	if( pccParamValue == NULL )
	{
		DUMP_CHLOG_ERROR( M4_CH_LDB_MISMATCHED_VERSIONS ) ;
		return M4_ERROR ;
	}

	pcVersionsList = new m4char_t[ strlen( pccParamValue ) + 1 ] ;
	strcpy( pcVersionsList, pccParamValue ) ;

	pcSingleVersion = strtok( pcVersionsList, ";" ) ;

	while( pcSingleVersion != NULL )
	{
		if( strcmpi( pcSingleVersion, pccMindVersion ) == 0 )
		{
			iRet = M4_SUCCESS ;

			// Guardamos la lista de versiones admitidas para las conexiones entrantes.
			SetMindDBVersion( pccParamValue ) ;

			break ;
		}

		pcSingleVersion = strtok( NULL, ";" ) ;
	}

	if( iRet == M4_ERROR )
	{
		DUMP_CHLOG_ERRORF( M4_CH_LDB_MISMATCHED_VERSIONS_VERBOSE, pccMindVersion << pccParamValue ) ;
	}
	
	delete pcVersionsList ;

	return iRet ;
}


// Api de caché
m4return_t ClLdb::RemoveAll (m4date_t ai_dDate)
{
	if( m_bClearAll == M4_FALSE && m_bClearTables == M4_FALSE )
	{
		m_bClearTables = M4_TRUE;
	}
	return M4_SUCCESS;
}


m4return_t ClLdb::RemoveObjectByIdAndVersion (m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	if( m_bClearAll == M4_FALSE )
	{
		if( strcmpi( ai_pcId, "SDC_LU_LANGUAGES" ) == 0 )
		{
			// Si se han tocado los lenguajes se borra todo
			m_bClearAll = M4_TRUE ;
		}
		else
		{
			if( m_bClearTables == M4_FALSE )
			{
				if( FindIndexByIdObject( ai_pcId ) != M4LDB_NULL_INDEX )
				{
					m_bClearTables = M4_TRUE;
				}
			}
		}
	}
	return M4_SUCCESS;
}


