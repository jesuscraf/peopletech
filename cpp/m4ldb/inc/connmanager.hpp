#ifndef _CONNMANAGER_HPP_
#define _CONNMANAGER_HPP_

#include "m4stl.hpp"
#include "connectioninfo.hpp"
#include "m4notifier.hpp"

//Para el cursor.
#include "conex.hpp"

class ClAccess;
class ClLConn ;
class ClServerEvent;
class ClDBConnection;
class ILdbContext;

// Contenedor para el puntero a la conexion. 
class ClDBConn_cont{
public:
	ClDBConn_cont():m_pDBConn (0),m_eTransState(M4DBW_CLOSE),m_bDoDisconnect(M4_FALSE){}
	m4return_t CreateConnection(void);
	m4return_t DestroyConnection(void);
	m4void_t MarkToDisconnect(void);
	m4return_t CreateConnection(m4int32_t ai_idRConn, stConexionProperties *ai_pstConProp, ClLConn *ai_pLConn);

	eState_t m_eTransState;
	ClDBConnection *m_pDBConn;

private:
	m4bool_t m_bDoDisconnect;
};

//Integracion 2x.
//Clave para identificar contendor de 2x
class ClKeyDynamicConnection {
	
	m4uint64_t	m_iIdSession;
	string		m_sIdUser;
	m4int32_t	m_idRConn;	

public:
	ClKeyDynamicConnection (void)
		: m_sIdUser("")
	{
		m_iIdSession = 0;
		m_idRConn = M4LDB_NULL_INDEX;
	}

	ClKeyDynamicConnection( m4uint64_t ai_iIdSession, const string & ai_sIdUser, m4int32_t ai_idRConn ) 
		: m_sIdUser (ai_sIdUser)
	{
		m_iIdSession = ai_iIdSession;
		m_idRConn = ai_idRConn;
	}

	//Constructor copia.
	ClKeyDynamicConnection( const ClKeyDynamicConnection & ai_oCopyClKeyDynamicConnection)
	{		
		m_iIdSession = ai_oCopyClKeyDynamicConnection.m_iIdSession;
		m_sIdUser = ai_oCopyClKeyDynamicConnection.m_sIdUser;
		m_idRConn = ai_oCopyClKeyDynamicConnection.m_idRConn;
	}

	//Operador de asignacion.
	ClKeyDynamicConnection & operator = ( const ClKeyDynamicConnection & ai_oCopyClKeyDynamicConnection )
	{
		m_iIdSession = ai_oCopyClKeyDynamicConnection.m_iIdSession;
		m_sIdUser = ai_oCopyClKeyDynamicConnection.m_sIdUser;
		m_idRConn = ai_oCopyClKeyDynamicConnection.m_idRConn;

		return *this;
	}

	m4bool_t CompareAnyKey( const ClKeyDynamicConnection & ai_oPatternClKeyDynamicConnection) const
	{
		if ((m_iIdSession == ai_oPatternClKeyDynamicConnection.m_iIdSession)
			|| (m_sIdUser == ai_oPatternClKeyDynamicConnection.m_sIdUser)
			|| (m_idRConn == ai_oPatternClKeyDynamicConnection.m_idRConn) )
		{
			return M4_TRUE;
		}
		return M4_FALSE;
	}

	m4bool_t CompareIdSessionKey( const ClKeyDynamicConnection & ai_oPatternClKeyDynamicConnection) const
	{
		if (m_iIdSession == ai_oPatternClKeyDynamicConnection.m_iIdSession)	{
			return M4_TRUE;
		}
		return M4_FALSE;
	}

	m4bool_t CompareIdUserKey( const ClKeyDynamicConnection & ai_oPatternClKeyDynamicConnection) const
	{
		if (m_sIdUser == ai_oPatternClKeyDynamicConnection.m_sIdUser)	{
			return M4_TRUE;
		}
		return M4_FALSE;
	}

	//La PK del mapa es la sesion, el usuario y el indice de la conexion física.! En realidad podría ser solo la sesion 
	//y la conexion fisica!. (Dada una session, en la tabla podemos tener N conexiones fisicas dinamicas!).
	//Nos hacen falta operadores de comparacion para el mapa.
	friend m4bool_t operator < (const ClKeyDynamicConnection & ai_oLeft, const ClKeyDynamicConnection & ai_oRight)
	{
		if (ai_oLeft.m_iIdSession < ai_oRight.m_iIdSession){
			return M4_TRUE;
		}
		if (ai_oLeft.m_iIdSession != ai_oRight.m_iIdSession){
			return M4_FALSE;
		}
		if (ai_oLeft.m_sIdUser < ai_oRight.m_sIdUser){
			return M4_TRUE;
		}
		if (ai_oLeft.m_sIdUser != ai_oRight.m_sIdUser){
			return M4_FALSE;
		}
		if (ai_oLeft.m_idRConn < ai_oRight.m_idRConn){
			return M4_TRUE;
		}
		if (ai_oLeft.m_idRConn != ai_oRight.m_idRConn){
			return M4_FALSE;
		}
		return (M4_FALSE);
	}


	//Buscamos por la pk.
	friend m4bool_t operator == (const ClKeyDynamicConnection & ai_oLeft, const ClKeyDynamicConnection & ai_oRight)
	{
		if (ai_oLeft.m_iIdSession != ai_oRight.m_iIdSession){
			return M4_FALSE;
		}
		if (ai_oLeft.m_sIdUser != ai_oRight.m_sIdUser){
			return M4_FALSE;
		}
		if (ai_oLeft.m_idRConn != ai_oRight.m_idRConn){
			return M4_FALSE;
		}
		return (M4_TRUE);
	}

	string GetIdUser () const {
		return m_sIdUser;
	}

	m4int32_t GetIdRConn( void ) const {
		return m_idRConn;
	}

};

class ClValueDynamicConnection {
	
	ClDBConn_cont * m_pDBConn;
	ClLConn *	m_pLConn;
	string		m_sConnStr;
	string		m_sConnStrFormated;
	m4bool_t	m_bReadyToFree;

public:
	ClValueDynamicConnection (ClDBConn_cont * ai_pDBConn, ClLConn * ai_pLConn)
		: m_sConnStr(""), m_sConnStrFormated("")
	{
		m_pDBConn = ai_pDBConn;
		m_pLConn = ai_pLConn;
		m_bReadyToFree = M4_FALSE;
	}

	~ClValueDynamicConnection (void)
	{
		m_pDBConn = NULL;
		m_pLConn = NULL;
	}

	ClValueDynamicConnection( ClDBConn_cont * ai_pDBConn, ClLConn * ai_pLConn, const string & ai_sConnStr, const string & ai_sConnStrFormated ) 
		: m_sConnStr(ai_sConnStr), m_sConnStrFormated(ai_sConnStrFormated)
	{
		m_pDBConn = ai_pDBConn;
		m_pLConn = ai_pLConn;
		m_bReadyToFree = M4_FALSE;
	}

	void SetDBConn( ClDBConn_cont * ai_pDBConn) {
		m_pDBConn = ai_pDBConn;
	}
	ClDBConn_cont * GetDBConn( void ) {
		return m_pDBConn;
	}

	void SetLogicalConnection( ClLConn * ai_pLConn) {
		m_pLConn = ai_pLConn;
	}
	ClLConn * GetLogicalConnection( void ) {
		return m_pLConn;
	}

	void SetConnStr( const string & ai_sConnStr) {
		m_sConnStr = ai_sConnStr;
	}
	string GetConnStr( void ) const {
		return m_sConnStr;
	}

	void SetConnStrFormated( const string & ai_sConnStrFormated) {
		m_sConnStrFormated = ai_sConnStrFormated;
	}
	string GetConnStrFormated( void ) const {
		return m_sConnStrFormated;
	}

	void SetReadyToFree( m4bool_t ai_bReadyToFree) {
		m_bReadyToFree = ai_bReadyToFree;
	}
	m4bool_t GetReadyToFree( void ) const {
		return m_bReadyToFree;
	}

};

//----------------------------------------------------------------------------------------------
//Mapa de conexiones dinámicas
typedef map<ClKeyDynamicConnection, ClValueDynamicConnection *, less<ClKeyDynamicConnection> >	MpClKeyDynamicConnection;
typedef MpClKeyDynamicConnection *												PMpClKeyDynamicConnection;
typedef MpClKeyDynamicConnection::iterator										itMpClKeyDynamicConnection;

//Mapa de cadenas, para el parseo de la cadena de conexion
typedef map<string, string, less<string> >		MpParseStringConnection;
typedef MpParseStringConnection *				PMpParseStringConnection;
typedef MpParseStringConnection::iterator		itMpParseStringConnection;

//Declaramos un vector de punteros de keys para tener una lista de keys encontradas que cumplan cierta condicion.
typedef vector<ClValueDynamicConnection *>	VcClValueDynamicConnection;
typedef VcClValueDynamicConnection::iterator	itVcClValueDynamicConnection;
//----------------------------------------------------------------------------------------------

class ClConnectionManager{
public:

	ClConnectionManager( void );
	m4return_t			Init(ClVMBaseEnv *,ClAccess *,ClLConn *,ILdbContext *);
	m4return_t			End(void);

// Gestion de conexiones fisicas.
	ClDBConnection *	InitConnection(m4int32_t ai_indConn,ClLConn *);
	ClDBConnection *	InitTransaction(m4int32_t ai_indConn,ClLConn *);
	m4return_t			EndConnection(m4int32_t ai_indConn,ClLConn *);
	m4return_t			DisconnectAll(ClLConn *);
	m4return_t			MarkAllToDisconnect(ClLConn *);
	m4return_t			RefreshInvalidConnections(m4int32_t ai_indDBConn,ClLConn *ai_pLConn);
	// Informacion sobre las conexiones fisicas.
	m4return_t			GetConnectionInfo(m4int32_t ai_idRConn,ClLConn *,stConexionProperties &stConProp);
	m4return_t			GetDDBBType (m4int32_t ai_idRConn,ClLConn *ai_pLConn,eDDBBType_t &ao_eDDBB);
////////

// Gestion de conexiones logicas.
	m4return_t			InsertLogicalConnection(m4int32_t ai_indLConn);
	m4return_t			DisplayError (m4return_t ai_retcode, ClDBConnection * ai_Conn,ClLConn *ai_pLConn);
////////

// Transacciones.
	// Actua sobre todas las transacciones.
	m4return_t			Transact(eTransType_t ,ClLConn *);
	// Actua sobre la conexion que nos digan.
	// Esta hecha para que Joseco pueda cerrar la conexion especial de auditoria.
	m4return_t			Transact(eTransType_t ,m4int32_t,ClLConn *);
	m4return_t			TransactOneDBConn(ClDBConn_cont &,ClLConn *,eTransType_t );
	m4return_t			TransactBeginTrans(ClDBConn_cont &ai_DBConn,ClLConn *ai_pLConn);
	m4return_t			TransactEndTrans(ClDBConn_cont &ai_DBConn,ClLConn *ai_pLConn,eTransType_t );

////////

// Tipos varios:
	typedef map<m4int32_t,ClDBConn_cont,less<m4int32_t> >			MapDBConnection;
	typedef map<m4int32_t,MapDBConnection,less<m4int32_t> >			MapLogConn2DBConn;
	typedef MapDBConnection::iterator								itMapDBConnection;
	typedef MapLogConn2DBConn::iterator								itMapLogConn2DBConn;

	typedef map<m4int32_t,stConexionProperties,less<m4int32_t> >	MapConnProp;
	typedef MapConnProp::iterator									itMapConnProp;

	// Identificador de la conexion fisica principal.
	static	const m4int32_t				m_sSystemDBConn;
	//Para validar seguridad en conexion diferente.
	static	const m4int32_t				m_sValidateSecurity;
	ClNotifier							DBRypture_Notifier;

// Tratamiento del mapa generico.	
	m4return_t				InitGenericMap(ClAccess *ai_pDictAccess,ClLConn *ai_pLConn);
	m4return_t				InsertOneEntryInGenericMap(m4int32_t ai_indRConn,stConexionProperties &ai_stConProp);
////////

// Busqueda de conexiones.	
	ClDBConnection *		FindDBConnection(m4int32_t ai_indLogConn,m4int32_t ai_indDBConn);
////////

// Funciones especiales utilizadas durante la inicializacion
	m4return_t				UpdateSystemConnection(m4int32_t ai_indLConn);
	m4return_t				SetDBConnection(ClDBConnection *,m4int32_t ai_indLConn,m4int32_t ai_indRConn);
////////

// Transacciones.
	m4return_t				Transact (MapDBConnection &ai_MapDBConn,eTransType_t ai_eTransType,ClLConn *ai_pLConn,m4bool_t ai_bDoAll);
////////

// Gestion de conexiones.
	m4return_t				DisconnectAll(MapDBConnection &ai_MapDBConn,ClLConn *);
	m4return_t				ValidateConnectionProperties(m4int32_t ,stConexionProperties &,ClLConn *);
////////
	
	//Integracion 2x.
	m4return_t			DisconnectAllDynamicConnections( void );	
	m4return_t			DestroyDynamicConnection( m4bool_t ai_bAllDynamicConnections );
	m4return_t			InsertIntoMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection, ClValueDynamicConnection * ai_pValueDynamicConnection);
	void				EraseFromMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection );
	m4return_t			EraseAnyKeyFromMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection );

	ClValueDynamicConnection *		FindIntoMapDynamicConnectionByKey( const ClKeyDynamicConnection & ai_oKeyDynamicConnection );

	void				SetReadyToFreeDynamicConnectionByIdSession( m4uint64_t ai_iIdSession, m4bool_t ai_bReadyToFree );	
	
	static m4bool_t		IsDynamicConnection( eConnectionType_t ai_eConnectionType );
	m4bool_t			IsDynamicConnectionByIdRConn( m4int32_t ai_idRConn );
	m4return_t			GetDynamicConnectionStringByConnectionType( eConnectionType_t ai_eConnectionType, ClLConn *ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp );
	
protected:
	m4return_t			_DisconnectAllDynamicConnections( void );
	void				_FindKeyDynamicConnectionByIdSession( m4uint64_t ai_iIdSession, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection );
	void				_FindKeyDynamicConnectionByIdUser( const string & ai_sUser, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection );
	void				_FindKeyDynamicConnectionByLogicalConnection( const ClLConn * ai_pLConn, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection );

	m4return_t			_GetDynamicConnectionString( ClLConn *ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp );
	m4return_t			_GetDynamicConnectionString2x( ClLConn * ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp );	
	m4return_t			_ExecuteStatementInConnectionDB( ClLConn * ai_pLConn, ClDBConnection * ai_pDBConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, const stConexionProperties * ai_pStConProp, string & ao_sUserDB, string & ao_sPasswordDB );

	void				_DisplayErrorCursorDynamic( ClLConn * ai_pLConn, m4int32_t ai_iErrorType, ClCursor * ai_pClCursor, const string & ai_sWhereIsError, const string & ai_sDescription );	

public:
	static m4return_t	FillDynamicConnectionString( string & aio_stConnStr, const string * ai_psUser, const string * ai_psPassword );	
	static m4return_t	ParseConnectionString( const string & ai_stConnStr, MpParseStringConnection & ao_mMpParseStringConnection );
	static m4return_t	FormatConnectionString( MpParseStringConnection & ai_mMpParseStringConnection, string & ao_stFormated );

protected:

	// Información sobre las conexiones. Solo se guarda aqui, y cada conexion tiene referencia a estas.
	MapConnProp						m_mGenericProperies;
	// Instancia estática. Únicamente sirve para optimizar la entrada de elementos en el mapa.
	ClDBConn_cont					m_oDBConn_Nihil;
	MapDBConnection					m_mGenericMapDBConn;

	ClConnectionInfo				m_oConnInfo;

	//Integracion 2x.
	MpClKeyDynamicConnection *		m_pMpClKeyDynamicConnection;
	
private:
	ClDBConn_cont	&		_InitConnection(m4int32_t ai_indConn,ClLConn *);
	ClDBConn_cont	&		_FindDBConnection(m4int32_t ai_indLogConn,m4int32_t ai_indDBConn);
	m4return_t				_GetConnectionInfo(m4int32_t ai_idRConn,ClLConn *,stConexionProperties *&stConProp);
	m4bool_t				FindDBConns(m4int32_t ai_indLConn,MapDBConnection	*&);

	MapLogConn2DBConn				m_mLogConn2DBConn;
	
	static	ClDBConnection *		m_sDBConnNihil;
};
/* La estructura del mapa sería :
	LConn	-1					0			1			2			....
DBConn		
0		DBConn_0_-1		DBConn_0_0	DBConn_0_1	DBConn_0_2		....
1		DBConn_1_-1		DBConn_1_0	DBConn_1_1	DBConn_1_2		....
3		DBConn_3_-1		DBConn_3_0	DBConn_3_1	DBConn_3_2		....
7		DBConn_7_-1		DBConn_7_0	DBConn_7_1	DBConn_7_2		....
.
.
.

Es decir, para la conexion logica -1 tenemos un mapa de conexiones fisicas, indexado por el indice de la conexion fisica.

*/

//Vamos a implementar una clase que se encargue de gestionar conexiones físicas de forma dinámica!.
//Se trata de un pool de conexiones físicas que se podrán creara de forma dinamica.

#endif	//_CONNMANAGER_HPP_
