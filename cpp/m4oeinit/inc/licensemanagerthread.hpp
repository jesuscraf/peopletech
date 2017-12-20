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


#ifndef	_M4MODKEY_LICENSEMANAGERTHREAD_HPP_
#define _M4MODKEY_LICENSEMANAGERTHREAD_HPP_

#include "globdec.hpp"

#include "licensemanager.hpp"

#include "m4thread.hpp"

#include "dm.hpp"
#include "channel.hpp"
#include "access.hpp"

#include "clm4objservice.hpp"

#include "m4ipcs.hpp"

class ClXmlAnalyzerFactory;
class ClEncryption;
class IXmlAnalyzer;
class ClLicense;
class ClModKeyProperties;

//----------------------------------------------------------------------------
// Definicion del canal que carga la licencia encriptada.
const m4char_t M4_LICENSE_CHANNEL_ID [] = "SDC_MT_LICENSE";
const m4char_t M4_LICENSE_NODE_ID [] = "SDC_MT_LICENSE";
const m4char_t M4_LICENSE_ITEM_ID [] = "LICENSE";
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Definicion del canal del systema.
const m4char_t M4_LICENSE_SYSTEM_T3S_ID [] = "M4SYS_LICENSE";
const m4char_t M4_LICENSE_SYSTEM_NODE_ID [] = "M4SYS_LICENSE";
const m4char_t M4_LICENSE_SYSTEM_TI_ID [] = "M4SYS_LICENSE";
const m4char_t M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID [] = "SYS_EXECUTE_REAL";
const m4char_t M4_LICENSE_SYSTEM_EXECUTE_REAL_RETURN_ITEM_ID [] = "SYS_EXECUTE_REAL_RETURN";
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//Frecuencia de lanzamiento de la select de Np (en seg). Dejamos 1 dia: 60*60*24
// bg 178790. 60*60*25
const m4int32_t	M4MODKEY_TIME_FREQUENCY_EXECUTION_NP			= 90000;
//Tiempo de espera para que se termine el thread de ejecución de Np.
const m4int32_t	M4MODKEY_TIME_WAIT_THREAD						= 5;
//Time out del mutex.
//Por si el thread de la select llega antes al unlock que el thread principal
//llegue al lock. Si ocurriera eso el principal se quedaría bloqueado para siempre.
//Esto son milisegundos!. Damos 5 seg de TO = 5 * 1000
const m4int32_t	M4MODKEY_TIME_OUT_WAIT_NP_THREAD				= 5000;
//----------------------------------------------------------------------------


//----------------------------------------------
//Clase ClLicenseChannelLoader
//----------------------------------------------
class ClLicenseChannelLoader {
	ClChannelManager * m_pClChannelManager;

	//Canal de mantenimiento de licencia.
	ClChannel	*m_pChannel;
	ClAccess	*m_pAccess;
	ClItemIndex   m_iLicenseIndex;

	m4char_t * m_pLicenseBuffer;
	m4int32_t m_iLicenseBufferSize;

	m4bool_t m_bInitialized;

protected:

	m4return_t _CreateLicenseChannel( void );
	m4return_t _CreateLicenseAccess( void );

public:
	ClLicenseChannelLoader( void );
	~ClLicenseChannelLoader( );

	m4return_t Init( ClChannelManager * ai_pClChannelManager );
	m4return_t End ( void );

	const m4char_t * GetpLicenseBuffer( void ) const;
	m4int32_t GetLicenseBufferSize( void ) const;
};
//----------------------------------------------
//FIN Clase ClLicenseChannelLoader
//----------------------------------------------

//Adelantamos pq se va a utilizar.
class ILicenseManager;

//----------------------------------------------
// Class ClExecuteNPNotifier
// This class is a Visitor enabling external listeners to be notified about the
// NP execution
//----------------------------------------------
typedef m4void_t (*pfNPEventHandler)(m4int32_t ai_iState);
class ClExecuteNPNotifier
{
protected:
	pfNPEventHandler m_pfNotifier;
public:
	ClExecuteNPNotifier(pfNPEventHandler ai_pfHandler);
	m4void_t Notify(m4int32_t ai_iState);
};

//----------------------------------------------
//FIN Clase ClExecuteNPNotifier
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNp
//----------------------------------------------
class ClExecuteNp {
	ILicenseManager * m_pLicenseManager;
	ClExecuteNPNotifier * m_pNotifier;
	ClChannelManager * m_pClChannelManagerSystem;
	ClChannel	*m_pChannelSystem;
	ClAccess	*m_pAccessSystem;

	const ClLicense * m_pClLicense;

protected:
	eStateValidateExecutionNp_t m_eStateValidateExecutionNp;
	ClMutex m_oSyncroState;

protected:

	m4return_t _CreateMetaChannelAndSystemChannel( void );
	m4return_t _CreateMetaChannelStruct( ClAccess * ai_pAccess );

	//Despues de validar deberé comprobar el estado.
	m4return_t _ValidateExecutionNp( void );

	m4return_t _ExecuteSelectNp( const string & ai_sSelect, m4int32_t & ao_iCount);
	m4void_t _SetNPState(eStateValidateExecutionNp_t ai_iState);

public:
	ClExecuteNp( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier );
	~ClExecuteNp( );

	static m4int32_t StringToNumber( const string & ai_sNumber );

	//--------------------------------------------------------
	virtual m4return_t StartExecute ( m4pchar_t ai_pId ) = 0;
	virtual m4bool_t IsRunningExecute ( void ) = 0;
	virtual m4return_t FinishExecute ( void ) = 0;

	virtual eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void ) = 0;
	//--------------------------------------------------------

	m4bool_t ValidateLicense( void );
};

//----------------------------------------------
//FIN Clase ClExecuteNp
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNpMonoThread
//----------------------------------------------
class ClExecuteNpMonoThread: public ClExecuteNp {

protected:
	ClM4Clock m_oClock;

	m4return_t _ExecuteSelectNp( const string & ai_sSelect, m4int32_t & ao_iCount);

public:
	ClExecuteNpMonoThread( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier );
	~ClExecuteNpMonoThread( );

	//--------------------------------------------------------
	m4return_t StartExecute ( m4pchar_t ai_pId );
	m4bool_t IsRunningExecute( void );
	m4return_t FinishExecute ( void );

	//Lo metemos, pero no es necesario en monothread.
	void Run (void );

	eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void );
	//--------------------------------------------------------
};
//----------------------------------------------
//FIN Clase ClExecuteNpMonoThread
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNpMultiThread
//----------------------------------------------
class ClExecuteNpMultiThread : public ClExecuteNp, public M4Thread {

public:
	ClExecuteNpMultiThread( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier );
	~ClExecuteNpMultiThread( );

	//--------------------------------------------------------
	m4return_t StartExecute ( m4pchar_t ai_pId );
	m4bool_t IsRunningExecute( void );
	m4return_t FinishExecute ( void );

	//Este metodo es necesario sobreescribirlo por el thread!. Será invocado en el start.
	void Run (void );

	eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void );
	//--------------------------------------------------------
};
//----------------------------------------------
//FIN Clase ClExecuteNpMultiThread
//----------------------------------------------

//----------------------------------------------
//Clase ClLicenseThread
//----------------------------------------------
//Esta clase se emplea en el server para poder parsear una licencia.
//Si estamos en multithread instanciara un thread que será el que definitivamente lanzará la select Np.
//Si es monothread la ejecución de la sentencia la lanza el único thread.
class ClLicenseThread {

protected:
	m4bool_t m_bMultiThread;
	ILicenseManager * m_pLicenseManager;
	ClChannelManager * m_pClChannelManager;

	ClEncryption * m_pClEncryption;
	IXmlAnalyzer * m_pIXmlAnalyzer;
	const ClLicense * m_pClLicense;

	ClExecuteNp * m_pClExecuteNp;

	ClModKeyProperties * m_pClProperties;

	void _Init( void );
	void _End( void );
	void _Reset( void );	

public:
	ClLicenseThread( m4bool_t ai_bMultiThread, ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManager );
	~ClLicenseThread( );

	m4return_t Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier );

	//Preguntamos por el estado de Np.
	eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void );

	m4bool_t	ValidateLicense( void );
	string		GetLicenseSummaryEncrypted( void );
	m4pcchar_t	GetPropertyValueById( m4pcchar_t ai_pPropertyId );
};

//----------------------------------------------
//FIN Clase ClLicenseThread
//----------------------------------------------

#endif


