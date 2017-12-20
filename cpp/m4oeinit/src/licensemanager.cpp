//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#include "licensemanager.hpp"
#include "licensemanagerthread.hpp"

#include "license.hpp"

#include "clfactm4objservice.hpp"
#include "conn_provider.hpp"

//----------------------------------------------
//Clase ClLicenseManagerBase
//----------------------------------------------
//Nota:
//Jeraquia de clases (En SM):
//- ClM4ObjService 
//		- ClM4ObjServiceClient
//			- ClM4ObjServiceClientSM
//		- ClM4ObjServiceServerSM

//Jeraquia de clases (En SERVER):
//- ClM4ObjService 
//		- ClM4ObjServiceServer
//
//En SM:
//		Idem que la LDB. Se creara un ClLicenseManagerClient en la parte cliente ClM4ObjServiceClientSM.
//		Se la pasara a la parte server ClM4ObjServiceServerSM.
//		Para pasarlo, tendremos que modificar la Init del ClM4ObjServiceServerSM para que acepte el argumento.
//		Tenemos un LicenseManagerCliente y otro LicenseManagerServer.
//		En la parte cliente NO se puede inicializar pq no hay sesion.
//		Cuando se inicializa la parte server SI podemos inicializarlo!!. En ese momento
//		tambien podremos inicializar LicenseManagerCliente!.
//		A partir del ChannelManager siempre podemos obtener su LicenseManager (cliente o server).
//
//En server:
//		Idem que la LDB. La creación se realiza al mismo tiempo que la LDB y la inicializacion 
//		se debe efectuar cuando ya tenemos inicializado todo, por ejemplo en la creacion de la BDL.
//		En DIC: La clase es ClM4ObjServiceClient, se creará un ClLicenseManagerClient pero NO
//		se puede inicilizar. En el logado inicializaremos el LicenseManagerCliente (esto es posible a
//		que en el canal sesion tenemos un item donde el server pone la licencia enciptada).
//		Importante: El estado Np se serializa como una propiedad, de esta forma, en el logon podemos
//		saber el estado Np. En sucesivas transacciones c/s se serializa cada vez el estado.
//		El LicenseManager del server es el unico que lanza las sentencias Np, se queda el estado
//		y en cada transaccion c/s se la pasa al cliente.
//
//Comprobaciones de permisos sobre items segurizados:
//		Modificamos el MCR para que se almacene el identificador de la licencia. Esto nos permitira
//		controlar el siguiente escenario:
//		- Cuando se genera un MCR se inserta el id licencia y se queda cacheado. Cuando se intente
//		levantar un m4o cacheado cuyo Id licencia no corresponda con la licencia actual significa
//		que el canal debe volverse a construir, desestimando el que hay en la cache.
//		Estos cambios se realizan en m4mdfac y m4mdrt.
//		También en la factoria, cuando se vaya a construir el metacanal (cltchann.cpp) comprobaremos
//		cada item de las estructuras de nodo del m4o .
//Comprobaciones de Np:
//		Podria dejarse que VB preguntara cuando quisiera, pe despues del logado 
//		y en el buildfromid.
//		Solución: La licencia y el estado se serializa. Cada vez que se realice una transaccion
//		c/s se invocará al Get del estado. Depende si estamos en Server o en DIC haremos 
//		cosas diferentes; Si se pregunta al server, el estado lo tiene el thread que está lanzando
//		la sentencia. En DIC el server habrá serializado el estado y se lo podemos preguntar
//		directamente al DIC.

//--------------------------------------------------------------------------
//Modificaciones en:
//dll:						m4chwrap, m4common, m4dm, m4exeforchannel, m4ldb, 
//							m4mdfac, m4mdrt, m4modkey, m4modkeywrap, m4oeinit,
//							m4serverbase, 
//Mensajes y compilacion:	logmsg, componentlist.ini, mvc.ini
//--------------------------------------------------------------------------

ClLicenseManagerBase::ClLicenseManagerBase( void )	
	:m_oSyncroState(M4_TRUE)
{
	m_pClLicenseChannelLoader = NULL;
	m_pClLicenseThread = NULL;
	m_bInitialized = M4_FALSE;

	m_pM4ObjService = NULL;
	m_pClChannelManager = NULL;
	m_bMultiThread = M4_FALSE;
	m_pLicenseEncrypted = NULL;
	m_iLicenseEncryptedSize = 0;
}

ClLicenseManagerBase::~ClLicenseManagerBase()
{
	End();	
}

m4return_t ClLicenseManagerBase::End( void )
{
	if (m_pClLicenseChannelLoader != NULL) {
		delete m_pClLicenseChannelLoader;
		m_pClLicenseChannelLoader = NULL;
	}

	if (m_pClLicenseThread != NULL) {
		delete m_pClLicenseThread;	
		m_pClLicenseThread = NULL;
	}

	if (m_pLicenseEncrypted != NULL) {
		delete m_pLicenseEncrypted;
		m_pLicenseEncrypted = NULL;
	}

	//Importante:
	//El objservice SOLO lo tengo que eliminar si somos nosotros el que lo hemos creado!.
	if (m_bMultiThread == M4_TRUE) {
		//Esto acabará eliminando el channel manager.
		if (m_pM4ObjService != NULL) {
			//Hacemos el end, pq en la creacion en el server se hizo un new.
			//Somos amigos, por eso podemos invocar al end y al destructor.
			m_pM4ObjService->End();			
			delete m_pM4ObjService;	
			m_pM4ObjService = NULL;
			//Por si acaso!.
			m_pClChannelManager = NULL;
		}
	}
	else {
		m_pM4ObjService = NULL;
		m_pClChannelManager = NULL;
	}

	m_bInitialized = M4_FALSE;

	return M4_SUCCESS;
}

m4bool_t ClLicenseManagerBase::IsInitialized( void )
{
	return m_bInitialized;
}

eStateValidateExecutionNp_t ClLicenseManagerBase::GetStateValidateExecutionNp ( void )
{
	if (IsInitialized() == M4_FALSE) {
		return STATE_VALIDATE_EXECUTION_NP_GREEN;
	}

	//Le preguntamos el estado al thread que ejecuta los Np.
	if (m_pClLicenseThread != NULL) {
		return m_pClLicenseThread->GetStateValidateExecutionNp();
	}

	return STATE_VALIDATE_EXECUTION_NP_RED;
}

m4return_t ClLicenseManagerBase::SetStateValidateExecutionNp ( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp )
{
	//Si no es DIC esto no puede hacerse!.
	return M4_ERROR;
}

m4bool_t ClLicenseManagerBase::ValidateLicense( void )
{
	if (m_pClLicenseThread != NULL) {
		return m_pClLicenseThread->ValidateLicense();
	}
	return M4_FALSE;
}

m4pchar_t ClLicenseManagerBase::GetpLicenseEncrypted( void )
{
	return m_pLicenseEncrypted;
}

string ClLicenseManagerBase::GetLicenseSummaryEncrypted( void )
{	
	string sLicenseSummaryEncrypted("");
	if (m_pClLicenseThread != NULL) {
		return m_pClLicenseThread->GetLicenseSummaryEncrypted();
	}

	//Se estará retornando una copia.
	return sLicenseSummaryEncrypted;
}

m4pcchar_t ClLicenseManagerBase::GetPropertyValueById( m4pcchar_t ai_pPropertyId )
{
	if (m_pClLicenseThread != NULL) {
		return m_pClLicenseThread->GetPropertyValueById( ai_pPropertyId );
	}

	return NULL;
}


//----------------------------------------------
//FIN Clase ClLicenseManagerBase
//----------------------------------------------

//----------------------------------------------
//Clase ClLicenseManagerServer
//----------------------------------------------
ClLicenseManagerServer::ClLicenseManagerServer( void )
{
}

ClLicenseManagerServer::~ClLicenseManagerServer( )
{
}

m4return_t ClLicenseManagerServer::Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier )
{
	if (IsInitialized() == M4_TRUE) {
		return M4_ERROR;
	}

	//--------------------------------------------------------------------------
	//Importante:
	//En MonoThread utilizo el ClM4ObjService que me pasan, NO se crea uno nuevo!.
	//A partir de ClM4ObjService puedo obtener un ChannelManager.

	//SOLO tiene sentido que me pasen un ClM4ObjService si estamos en SM.
	//En server nos crearemos nuestro propio ClM4ObjService para no interferir.

	m_bMultiThread = ai_bMultiThread;
	
	if (m_bMultiThread == M4_TRUE) {
		//Hace falta crear un channel manager (gestor de canales), lo creamos a partir del objservice
		//que nos habran pasado.
		m_pM4ObjService = ai_pM4ObjService;
		if (m_pM4ObjService != NULL) {
			m_pClChannelManager = m_pM4ObjService->GetpChannelManager();
		}
	}
	else {
		//Obligatoriamente nos han tenido que pasar un ClM4ObjService!.		
		if (ai_pM4ObjService != NULL) {
			//Me es indivisible el ClM4ObjService, lo dejamos NULL.
			//Al ser nulo tampoco eliminaré el channel manager que nos han pasado!.

			//Lo que realmente me importa es el ChannelManager			
			m_pM4ObjService = NULL;
			m_pClChannelManager = ai_pM4ObjService->GetpChannelManager();
		}
	}
	//--------------------------------------------------------------------------

	//Creamos la clase que carga la licencia de BD. Inicializamos.
	m_pClLicenseChannelLoader = new ClLicenseChannelLoader();	
	if (m_pClLicenseChannelLoader->Init( m_pClChannelManager ) == M4_ERROR) {
		End();
		return M4_ERROR;
	}

	/* Bug 0115934
	Hay que liberar la conexión debase de datos para no gastar recursos
	*/
	if( m_pM4ObjService != NULL )
	{
		ClLConn_Provider	*poPovider = m_pM4ObjService->GetpConnProvider() ;

		if( poPovider != NULL )
		{
			poPovider->SetConnection( NULL ) ;
		}
	}

	//Ya tenemos la licencia, ahora iniciamos el thread!.

	//Si no está inicializado...
	m_pClLicenseThread = new ClLicenseThread(m_bMultiThread, this, m_pClChannelManager);

	//----------------------------------------------------------------------------
	m_iLicenseEncryptedSize = m_pClLicenseChannelLoader->GetLicenseBufferSize();
	m_pLicenseEncrypted = new m4char_t [m_iLicenseEncryptedSize + 1];
	if (m_pLicenseEncrypted == NULL) {
		End();
		return M4_ERROR;
	}
	memcpy(m_pLicenseEncrypted, m_pClLicenseChannelLoader->GetpLicenseBuffer(), m_iLicenseEncryptedSize);
	m_pLicenseEncrypted [m_iLicenseEncryptedSize] = NULL;
	
	//Importante!!.
	//Ya tenemos el contenido de la licencia. Ahora vamos a iniciar el thread!.
	if (m_pClLicenseThread->Init(m_pLicenseEncrypted, m_iLicenseEncryptedSize, ai_pNotifier) == M4_ERROR) {
		End();
		return M4_ERROR;
	}
	//----------------------------------------------------------------------------

	m_bInitialized = M4_TRUE;

	return M4_SUCCESS;
}


m4return_t ClLicenseManagerServer::Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier )
{
	//Permitimos esta llamada SOLO para DIC.
	return M4_ERROR;
}

//----------------------------------------------
//FIN Clase ClLicenseManagerServer
//----------------------------------------------

//----------------------------------------------
//Clase ClLicenseManagerClient
//----------------------------------------------
ClLicenseManagerClient::ClLicenseManagerClient( void )
{
	m_eStateValidateExecutionNp = STATE_VALIDATE_EXECUTION_NP_RED;
}

ClLicenseManagerClient::~ClLicenseManagerClient(  )
{
}

m4return_t ClLicenseManagerClient::Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier )
{
	//Solo en Server se puede inicializar así.
	return M4_ERROR;
}

m4return_t ClLicenseManagerClient::Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier )
{
	//En DIC no queremos lanzar selects, unicamente queremos la licencia.
	//Si no está inicializado...

	m_pClLicenseThread = new ClLicenseThread(M4_FALSE, this, NULL);

	//----------------------------------------------------------------------------
	//Importante!!.
	//Ya tenemos el contenido de la licencia. Ahora vamos a iniciar el thread!.
	if (m_pClLicenseThread->Init(ai_pLicenseEncrypted, ai_iSizeLicenseEncrypted, ai_pNotifier) == M4_ERROR) {
		End();
		return M4_ERROR;
	}
	//----------------------------------------------------------------------------

	m_iLicenseEncryptedSize = ai_iSizeLicenseEncrypted;
	m_pLicenseEncrypted = new m4char_t [m_iLicenseEncryptedSize + 1];
	if (m_pLicenseEncrypted == NULL) {
		End();
		return M4_ERROR;
	}
	memcpy(m_pLicenseEncrypted, ai_pLicenseEncrypted, m_iLicenseEncryptedSize);
	m_pLicenseEncrypted [m_iLicenseEncryptedSize] = NULL;

	m_bInitialized = M4_TRUE;

	return M4_SUCCESS;
}

eStateValidateExecutionNp_t ClLicenseManagerClient::GetStateValidateExecutionNp ( void )
{
	ClMutBlock oCriticalSection (m_oSyncroState);

	//En Client esta guardado pq se serializa.
	return m_eStateValidateExecutionNp;
}

m4return_t ClLicenseManagerClient::SetStateValidateExecutionNp ( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp )
{
	ClMutBlock oCriticalSection (m_oSyncroState);

	//En Client esta guardado pq se serializa.
	m_eStateValidateExecutionNp = ai_eStateValidateExecutionNp;
	return M4_SUCCESS;
}
//----------------------------------------------
//FIN Clase ClLicenseManagerDIC
//----------------------------------------------
