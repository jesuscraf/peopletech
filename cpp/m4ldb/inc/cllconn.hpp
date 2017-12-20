//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cllconn.hpp   
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

#ifndef _CLLCONN_HPP_
#define _CLLCONN_HPP_


#define M4CMPROP_CHANNEL_N_ROWS_DB						"NUM_ROWS_DB"
#define M4CMPROP_CHANNEL_N_ROWS_DB_LIMIT				"NUM_ROWS_DB_LIMIT"
#define M4CMPROP_CHANNEL_DISABLE_MAX_RECORDS_MESSAGE	"DISABLE_MAX_RECORDS_MESSAGE"


#include "m4stl.hpp"
#include "defmain.hpp"
#include "cltblmem.hpp"
#include "clsentcc.hpp"
#include "clnametrans.hpp"
#include "lconn_utils.hpp"
#include "lconn_interface.hpp"
#include "index.hpp"
#include "m4notifier.hpp"

class ClLdb;
class ClNode;
class ClAccessRecordSet;
class ClCompiledMCR;
class ClVMStatistics;
class ClStmt;
class ClLoad;
class ClMetaSentence;
class ClPersist;
class ClLogonAdaptor;
class ClDBConnection;
class ClDinArr;
class ClRegister;
class ClOperation_RecordSet;
class m4VariantType;
struct stConexionProperties;
class ClConnectionManager;
class ClChannelManager;


//--------------------------------------------------------------------
//Pedro :Bug 0064098 -> DC; 0074680 -> UC
//Estados de la conexión
const m4uint8_t M4LDB_CONEX_STATUS_NONE							= 0x00;
const m4uint8_t M4LDB_CONEX_STATUS_DELETE_ON_CASCADE			= 0x01;
const m4uint8_t M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE			= 0x02;
//Fin Pedro
//--------------------------------------------------------------------

//Mascaras de volcado en el inspector. Categorias.
//La entrada en el registro asociada es M4_INIT_SYSTEM_DEBUG_DETAIL_LEVEL = SystemDebugDetailLevel
//Variables que me indican el detalle con el que mostramos el ldbinsp.

//Veamos los valores del registro para el detalle:
	//	0	=	Nada. Es decir, no se aplica el detalle. Es análogo a no tener la entrada en el registro.
	//	1	=	Indicamos un detalle mínimo. En principio ejecucion!!.
	//	2	=	Es el OR de todos los detalles
	//	3	=	Grupo de detalle i
	//	4	=	Grupo de detalle i + 1
	//IMPORTANTE:
	//Tendremos todos los grupos de detalle posibles. Hay que tener en cuenta que en el registro no introduciremos
	//máscaras, se introducirá un valor decimal 0, 1, 2... que tendremos que traducirlo a la máscara correspondiente.

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_ALWAYS				= 0x0FFFFFFF;
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_NONE				= 0x00000000;		//     0

//Reservamos un rango de [1..15] inclusive para que no coincida con valores que devuelve GetSystemDebugDetailLevelMask.
//Si en el registro se introduce un valor [1..15], se traducirá por una mascara.
//Si en el registro se introduce un valor [16..[ tendremos mascaras mas detalladas.

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION		= 0x00000010;		//     16

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_PREPARE				= 0x00000020;		//     32
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_BIND				= 0x00000040;		//     64

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_EXECUTION			= 0x00000080;		//    128
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_DATA				= 0x00000100;		//    256

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES		= 0x00000200;		//    512

//----------------------------------------------------------
//Tenemos un OR de todas las mascaras que no son de fichero!!!
//Es importante que no se incluyan las de fichero!!
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_ALL_MASKS_MODE_WITHOUT_FILE		= M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION
																			| M4LDB_DETAIL_LEVEL_SHOW_PREPARE
																			| M4LDB_DETAIL_LEVEL_SHOW_BIND
																			| M4LDB_DETAIL_LEVEL_SHOW_EXECUTION
																			| M4LDB_DETAIL_LEVEL_SHOW_DATA
																			| M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES;
//----------------------------------------------------------

//Leera de un fichero de configuracion. Permitiremos mostrar informacion relacionada con un M4O
//con un usuario o con un role.
//Volvemos a dejar un colchón entre trazas.
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_FILE_CONFIGURABLE	= 0x00010000;		//  65536

//-----------------------------------------------------------------------------------------------
//Compuestos
const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_EXEC_IDENTIF		= M4LDB_DETAIL_LEVEL_SHOW_EXECUTION 
																	| M4LDB_DETAIL_LEVEL_SHOW_IDENTIFICATION;	
																					//    144

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_EXEC_DATA			= M4LDB_DETAIL_LEVEL_SHOW_EXECUTION 
																	| M4LDB_DETAIL_LEVEL_SHOW_DATA;
																					//	  384

const	m4uint32_t	M4LDB_DETAIL_LEVEL_SHOW_EXEC_DATA_SENTENCES	= M4LDB_DETAIL_LEVEL_SHOW_EXECUTION 
																	| M4LDB_DETAIL_LEVEL_SHOW_DATA
																	| M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES;	
																					//	  896
//-----------------------------------------------------------------------------------------------

//Auditoria select. Se va a utilizar en la conexion.
class ClSelectAuditory;
class ClAPISQL;
class ClGlobVars;  


class ClSysHint;
typedef vector<ClSysHint * >		VcSysHint;
typedef VcSysHint *					PVcSysHint;
typedef VcSysHint::iterator			itVcSysHint;
typedef VcSysHint::const_iterator	itConstVcSysHint;

class ClDynamicHint;
typedef vector<ClDynamicHint * >		VcDynamicHint;
typedef VcDynamicHint *					PVcDynamicHint;
typedef VcDynamicHint::iterator			itVcDynamicHint;
typedef VcDynamicHint::const_iterator	itConstVcDynamicHint;

typedef map<string, string, less<string> >		MpParseSysHint;
typedef MpParseSysHint *						PMpParseSysHint;
typedef MpParseSysHint::iterator				itMpParseSysHint;

class ClLConn : public ClLConn_Interface, public ClNotifiable
{
public:

	virtual ~ClLConn(void);
	virtual m4return_t Init (void);
	virtual m4return_t End (void);
	// Carga.
	virtual m4return_t Load		(ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem = 0, m4VariantType *ai_pParam = 0, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX, m4int32_t   *ao_iNumData = 0,m4int32_t   ai_iFirstData = 0);
	virtual m4return_t SysLoad	(ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_indSelect = M4LDB_NULL_INDEX, m4int32_t ai_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2);
	
	// Persistencia.
	m4return_t InsertBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution)
	{
		return (Persist (ai_pRecordSet, M4LDB_INSERT, ai_poStatistics,ao_bStopExecution));
	}

	m4return_t UpdateBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution )
	{
		return (Persist (ai_pRecordSet, M4LDB_UPDATE, ai_poStatistics,ao_bStopExecution));
	}

	m4return_t DeleteBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution )
	{
		return (Persist (ai_pRecordSet, M4LDB_DELETE, ai_poStatistics,ao_bStopExecution));
	}

	// Gestión de transacciones.
	virtual m4return_t BeginTransaction (ClAccessRecordSet *ai_pRecordSet);
	virtual m4return_t EndTransaction (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_iAction, m4bool_t &ao_bFree); 

	// Ejecuciones directas.
  	virtual m4return_t ExecuteSQL (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4bool_t ai_bReal, m4VariantType *ai_pParam = 0, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX,m4int32_t ai_lMask = 0);

	m4uint32_t GetSystemDebugDetailLevel (void);

	m4bool_t GetLicenseSummaryEncrypted( string & ao_sLicenseSummary );
	void _DumpLicenseSummaryEncrypted( ClCompiledMCR *ai_poCMCR );

	//Bug:63638
	m4int32_t GetNumTrans(void)
	{
		return m_numTrans;
	}

	// reenter
	m4bool_t IsReenter(void)
	{
		return m_lReenterCounter > 1;
	}

	//Detalle de trazas fichero.
	m4id_t GetIdT3Trace (void) const
	{
		return m_pIdT3Trace;
	}

	m4id_t GetIdNodeTrace (void) const
	{
		return m_pIdNodeTrace;
	}

	m4id_t GetIdUserTrace (void) const
	{
		return m_pIdUserTrace;
	}

	m4id_t GetIdRoleTrace (void) const
	{
		return m_pIdRoleTrace;
	}

	m4pcchar_t GetInformer (void) const
	{
		return m_pInformer;
	}

	m4uint64_t GetIdSession ( void )
	{
		return m_iIdSession;
	}
	void SetIdSession ( m4uint64_t ai_iIdSession )
	{
		m_iIdSession = ai_iIdSession;
	}

	m4bool_t	IsForeignLoaded( void );
	void		SetForeignLoaded( void );

	m4bool_t	IsRSMLoaded( void );
	void		SetRSMLoaded( void );

protected:

	static m4return_t	_CreateSysHint( ClLConn * ai_pClLConn, const string & ai_sSysHint, ClSysHint * & ao_pClSysHint );
	static m4return_t	_ParseSysHint( ClLConn * ai_pClLConn, const string & ai_sSysHint, MpParseSysHint & ao_oMpParseSysHint );		
	static m4bool_t		_IsReservedWordSysHint( m4pcchar_t ai_pTokenSysHint, m4int32_t & ao_iNumChrParsed );
	static m4return_t	_GetSysHintIdParsed( ClLConn * ai_pClLConn, m4pcchar_t ai_pCurrent, string & ao_sSysHintId, m4int32_t & ao_iNumChrParsed);
	static m4return_t	_ComprimeixBlancs( string & aio_sToCompress, m4int32_t ai_iPosBegin );
	static m4return_t	_IsInsideLiteral( ClLConn * ai_pClLConn, m4int32_t ai_iPosSourceString, const string & ai_sSourceString, m4bool_t & ao_bIsInsideLiteral, m4int32_t & aio_iPosBeginLiteral, m4int32_t & aio_iPosEndLiteral );

	m4uint32_t AddReenter (m4uint32_t ai_lReenterCounter)
	{
		m_lReenterCounter +=  ai_lReenterCounter;
		return m_lReenterCounter;
	}

	ClAccessRecordSet * GetpaRS (void) 
	{
		if (m_paRS)	{
			return (*m_paRS);
		}
		else {
			return NULL;
		}
	}

	//Select de auditoria.
	ClSelectAuditory * m_pSelectAuditory;

	// Nodo de auditoría de traducciones
	ClNode	*m_poAuditTranslationNode;

	m4uint64_t	m_iIdSession;

	//Quitamos el comentario pq se utiliza para preguntar el usuario en conexiones dinamicas.
	//Detalle de trazas fichero.
	m4id_t	m_pIdT3Trace;
	m4id_t	m_pIdNodeTrace;
	m4id_t	m_pIdUserTrace;
	m4id_t	m_pIdRoleTrace;

	// reenter
	m4uint32_t m_lReenterCounter;

	m4return_t _End (/*m4bool_t ai_bFreeSysConn*/);

	virtual m4bool_t Notify (ClEvent &);	// Notificador.
	ClConnectionManager *m_pConnManager;	// Gestor de conexiones.

	m4bool_t m_bLicenseSummaryDumped;

	// A partir de la versión 170 hacemos el objeto ClLConn reentrante.

	// Todo esto, a priori, se puede compartir perfectamente.
	m4pcchar_t m_pInformer;

	//-------------------------------------------------------------------------
	//Pedro: Estado para indicar si a la conexion se llega con un Delete Cascade.
	//Se utiliza para validar la seguridad. Solucionando el bug Bug 0064098
	m4uint8_t m_iStatus;
	//Fin Pedro
	//-------------------------------------------------------------------------
	
	ClLdb *m_pLdb;

	m4int32_t m_idLConn;

	LsPStPostValidation *m_plsPostValidation;

	MpTransCache *m_pmpTransCache;

	VcPClAccessRl *m_pvcTransAccRl;

	ClAccessRecordSet **m_paRS;

	ClAccess *m_pAcsDict;

	m4date_t m_dDTLstUpdTrn;

	m4int32_t m_numTrans;

	m4int32_t m_idExecution;

	m4int32_t m_lSysDebugLevel;

	ClLogonAdaptor *m_pLogonAdaptor;

	// Mucho cuidado con esto. Hay que validar que no cambia.
	ClVMStatistics *m_poStatistics;

	// Esto lo pasamos al Stmt y lo cambiamos por m_idLastLConnUsed
	m4int32_t m_indLastSentUsed;

	string m_stBuffer;

	m4bool_t m_bIgnoreChannel;

	m4bool_t m_bPreVal;

	ClNameTranslator	m_oNameTrans;
	
#ifdef _M4LDB_DEBUG
	m4int32_t m_lInsInfo;
	m4int32_t m_lUpdInfo;
	m4int32_t m_lDelInfo;
#endif

	friend class ClLConnReenterControl;
	friend class ClDinArr;
	friend class ClPersistBase;
	friend class ClPersist;
	friend class ClGlobVars;
	friend class ClLDBInspector;
	friend class ClMetaItemSess;
	friend class ClAPISQL;
	friend class ClMetaSentence;
	friend class ClStmt;
	friend class ClLoad;
	friend class _AClSQLTranslator;
	friend class ClTraductor;
	friend class ClSQLTranslator_SQLServer;
	friend class ClSQLTranslator_Informix;
	friend class TableAnalyzer;
	friend class ClLdb;
	friend class ClNameTranslator;
	friend class ClConnectionManager;
	m4return_t _X_AnalizeSecurity (ClAPISQL *ai_pAPISQL, ClMetaSentence *&aio_pMtSent, ClGlobVars *ai_pGlobVars, ClAccessRecordSet *ai_pRecordSet, m4int32_t *ao_plSecMask, eStmType_t ai_eStmType, ClDinArr *ai_Param);
	m4return_t _X_FindFilterByIdObject (eStmType_t ai_eStmType, string& ao_stFilter, m4pcchar_t ai_pAlias);
	m4bool_t _X_CanAccessByField (m4id_t ai_idField, m4bool_t ai_bIsSelect);

	m4return_t _X_X_AnalizeSecurity (ClAPISQL *ai_pAPISQL, ClMetaSentence *&aio_pMtSent, ClGlobVars *ai_pGlobVars, ClAccessRecordSet *ai_pRecordSet = NULL, m4int32_t *ao_plSecMask = NULL, eStmType_t ai_eStmType = M4LDB_SELECT, ClDinArr *ai_Param = NULL);
	m4return_t _X_X_FindFilterByIdObject (VcPStSecurityFilter &ai_vcSecFil, eStmType_t ai_eStmType, string& ao_stFilter, m4pcchar_t ai_pAlias);
	m4return_t _X_ComposeJoin (string &ai_stJoin, m4pchar_t ai_pFilter, m4pchar_t ai_pJoin);
	m4return_t _X_ReplaceAlias (string &ai_stFilter, m4int32_t ai_lOffset, m4pcchar_t ai_pAlias);

	m4return_t SetClosingDate (ClMetaSentence *ai_pMtSent ,ClAccessRecordSet *ai_pRecordSet );

	m4return_t ReserveDictionary(void);
	m4return_t ReserveDictionaryTrans(void);
	m4return_t CreateField(m4pcchar_t ai_pccId, m4pcchar_t ai_pccRealName, m4uint16_t ai_iPosPk, m4pcchar_t ai_pccType, m4uint16_t ai_iInternalType, m4bool_t ai_bNotNull, m4bool_t ai_bControlRepetition);
	m4return_t CloneTranslationField(m4pcchar_t ai_pccName = NULL);
	m4return_t CreateTranslationMetaData(m4uint32_t ai_iTableNumber, m4uint32_t ai_iTranslatedNumber, m4uint32_t ai_iDynamicLangNumber, m4pcchar_t ai_pccTransRealObject, m4uint8_t ai_iMirrorType);
	m4return_t CreateTramitationView(ClRegisterIndex ai_iIndRealTbl, m4int32_t ai_iNumFields, m4pcchar_t ai_pccMirrorBase, m4pcchar_t ai_pccMirrorRealObject, m4pcchar_t ai_pccRealObject, m4bool_t ai_bIsTranslation);
	m4return_t CreateInheritView(ClRegisterIndex ai_iIndRealTbl, m4int32_t ai_iNumFields, m4pcchar_t ai_pccBase, m4pcchar_t ai_pccRealObject, m4bool_t ai_bIsTranslation);
	m4return_t LoadDictionaryChannel (m4id_t ai_idLgcTbl, m4int32_t *ai_indLgcTbl, m4bool_t ai_bDumpError);
	m4return_t LoadDictionaryChannelForeigns (void);
	m4return_t LoadDictionaryTrans (m4id_t ai_idLgcTbl, m4int32_t &ai_indLgcTbl);
	m4return_t LoadDictionaryChannelRSM (void);
	m4return_t Commit (void);
	m4return_t RollBack (void);
	m4return_t ValidateOnePostValidation (StPostValidation *ai_pPostValidation, m4bool_t ai_bPostValidateOperations, m4bool_t ai_bPostValidateFKs);
	m4return_t ValidatePostValidations (void);
	m4return_t ValidatePostOperations (void);

	m4int32_t FindIndexByIdField (m4id_t ai_idField);
	m4int32_t FindIndexByIdInternalField (m4int32_t ai_idIntFld);

	m4return_t InitAuditory (void);
	
	//Requerimiento 0084073.
	//Limitacion por parametrizacion en la ejecucion de RealSql.
	m4return_t AnalizeRealSqlMode (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet, m4bool_t ai_bCheckAuditory);
	m4return_t AnalizeAuditoryRealSql (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet);

	//Auditoria select.	
	m4return_t AnalizeAuditorySelect (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet);
	m4return_t AnalizeAuditorySelectToTable (ClAccessRecordSet *ai_pRecordSet, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength);	
	//Fin auditoria select.

	m4return_t AnalizeAuditoryObject (m4id_t ai_idRSM, m4int32_t ai_indLgcTbl, m4int32_t &ao_lAudMask);

	m4return_t GetAuditTranslationNode( ClChannelManager *ai_poChannelManager, ClNode* &ao_rpNode );
	m4return_t PersistAuditTranslation( void );
	m4return_t ResetAuditTranslation( void );

	m4id_t GetnLgcTbl (m4int32_t ai_indLgcTbl);
	m4id_t GetnField (ClStmt *ai_pStmt, m4int32_t ai_indParam);

	m4bool_t	_CheckOperation(ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType,m4int32_t ai_hItem);
	m4bool_t	_CheckOperationDelete(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg);
	m4bool_t	_CheckOperationInsert(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg);
	m4bool_t	_CheckOperationUpdate(ClOperation_RecordSet *ai_pBOper,ClRegister *ai_pRg);

	void AuditTime( m4uint32_t iTotalTime );
	m4return_t _SysLoad (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_indSelect);
  	m4return_t _Load (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData);
  	m4return_t _Persist (ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn, m4int32_t ai_lMask);
	m4return_t _RealStmt (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics*ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam, m4int32_t ai_numParam, m4int32_t ai_idRConn);
	m4return_t _BeginTransaction (ClAccessRecordSet *ai_pRecordSet);
	m4return_t _EndTransaction (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_iAction, m4bool_t &ao_bFree);

  	m4return_t Persist (ClAccessRecordSet *ai_pRecordSet, eStmType_t ai_eStmType, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution, m4int32_t ai_hItem = 0, m4VariantType *ai_pParam = NULL, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX, m4int32_t ai_lMask = 0);
	m4return_t RealStmt (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics*ai_poStatistics, m4int32_t ai_hItem, m4VariantType *ai_pParam = NULL, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX);

	m4return_t GetSysParam (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_ItemIndex, m4VariantType *&ao_pParam, m4int32_t &ao_numParam);

	m4bool_t		_FindPersistedAccess		(ClAccess	*ai_pAccess);

	m4return_t	_GetSysHints ( VcSysHint & ao_VcSysHint, ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode );

	m4char_t	_GetIdAutoFilter( ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode );
	m4bool_t	_IsInfiniteDates( ClAccessRecordSet * ai_pRecordSet, ClCompiledMCR * ai_pMCR, m4int32_t ai_hNode, m4uint8_t ai_iAutoFilterType );

	m4return_t	_CheckUserSQL( m4bool_t ai_bIsUser, m4pcchar_t ai_pccSQL );

public:

#ifdef _M4LDB_DEBUG
	ClLDBInspector *m_pInspector;

	ClLConn (ClLdb *ai_pLdb, m4int32_t ai_idLConn, ClConnectionManager *ai_pConnManager,ClLDBInspector *ai_pInspector);
#else
	ClLConn (ClLdb *ai_pLdb, m4int32_t ai_idLConn,ClConnectionManager *ai_pConnManager);
#endif


	m4return_t CreateSysAccess (void);
	m4return_t FreeConnections (void);
	m4return_t ReleaseConnection (void);
	m4return_t SetCurrentDictIndexes(void);

	m4return_t DisplayError (m4return_t ai_retcode, ClDBConnection *ai_Conn );

	m4int32_t FindIndexById (m4int32_t ai_idNode, m4id_t ai_idValue);
	m4int32_t FindIndexById (m4int32_t ai_idNode, m4double_t ai_dValue);
	m4int32_t FindIndexByIds (m4int32_t ai_idNode, m4id_t ai_idValue, m4double_t ai_dValue);
	m4int32_t FindIndexByIds (m4int32_t ai_idNode, m4double_t ai_dValue1, m4double_t ai_dValue2);

	m4bool_t FindStmtInLsPostVal (ClStmt *ai_pStmt);
	m4pcchar_t FindOrgLevelName (m4int32_t ai_iLevel);
	m4int32_t FindOrgLevel (m4pcchar_t ai_pccOrganization);
	m4pcchar_t FindParentOrg (m4pcchar_t ai_pccOrganization, m4int32_t ai_iLevel, m4bool_t ai_bDumpError);
	m4bool_t IsDynamicLanguage(m4uint32_t ai_iLanguage);


	// No usadas pero necesarias.
  	friend m4bool_t operator < (const ClLConn &lhc, const ClLConn &rhc);
  	friend m4bool_t operator ==(const ClLConn &lhc, const ClLConn &rhc);

	// Esto sólo lo utilizará la Log para mostrar mensajes.
	ClCompiledMCR *m_pLogMCR;
	ClAccess	  *m_pLogAccess;

	m4int32_t m_hLogNode;

	
	// Funciones inline.
	ClAccess *GetDictAccess (void)
	{
		return (m_pAcsDict);
	}
	m4int32_t GetSystemDebugEnable(void)
	{
		return (m_lSysDebugLevel);
	}
	ClConnectionManager *GetConnManager (void)
	{
		return (m_pConnManager);
	}
	ClLdb * GetpLdb(void)
	{
		return (m_pLdb);
	}
	m4int32_t GetIdLConn (void)
	{
		return (m_idLConn);
	}

	m4int32_t GetLastSentUsed (void) 
	{
		return (m_indLastSentUsed);
	}

	void SetLastSentUsed (m4int32_t ai_indLastSentUsed)
	{
		m_indLastSentUsed = ai_indLastSentUsed;
	}

	m4bool_t IgnoreChannel (void)
	{
		return (m_bIgnoreChannel);
	}

	void SetIgnoreChannel (m4bool_t ai_bIgnoreChannel)
	{
		m_bIgnoreChannel = ai_bIgnoreChannel;
	}
};

typedef ClLConn* PClLConn;

// for cllcon reenter control
class ClLConnReenterControl {
private:
	ClLConn * m_pLConn;
public:

	ClLConnReenterControl(ClLConn * ai_pLConn);
	~ClLConnReenterControl();
};

//-------------------------------
//Auditoria de select.
class ClItem;
class ClChannelManager;
class ClAcess;
class ClChannel;

#define M4_LDB_AUDIT_ADD_ITEM      "SDC_AU_ADD"
#define M4_LDB_AUDIT_EXECUTE_ITEM  "SDC_AU_EXECUTE"
#define M4_LDB_AUDIT_CHANNEL_ID	   "SDC_LDB_AUDIT" 
#define M4_LDB_AUDIT_NODE_ID	   "SDC_LDB_AUDIT" 
#define M4_LDB_AUDIT_TABLE_INDEX	3

class ClSelectAuditory
{
protected: 

	typedef list<m4VariantType *> AuditList_t;
	typedef AuditList_t::iterator AuditListIt_t;
	
	//Es un set de strings.
	typedef set<string, less <string> > AuditTableSet_t;
	typedef AuditTableSet_t::iterator AuditTableSetIt_t;

	ClLConn		*m_pLConn;
	ClChannelManager *m_poChannelManager;
	ClChannel	*m_poChannel;
	ClNode		*m_poNode;
	ClItemIndex   m_iAddIndex;
	ClItemIndex   m_iExecuteIndex;
	m4bool_t     m_bHaveToExecute;

	AuditList_t  m_oList;
	AuditTableSet_t m_oTableSet;

	m4return_t	CreateAuditChannel(  );
	m4return_t	CreateAuditAccess(  );

	m4return_t	DestroyAuditChannel(  );
	m4return_t	DestroyAuditAccess(  );

	m4return_t	_ExecuteAudit(  );

	void		_Reset();

public:

	ClSelectAuditory( ClLConn * ai_pLConn ) ; 

	virtual ~ClSelectAuditory( void ) ;

	m4return_t	ExecuteAudit(  );

	m4return_t	AddAudit( ClChannelManager *ai_poChannelManager, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength );
};
//-------------------------------

#endif

