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


#ifndef	_M4MODKEY_LICENSEMANAGER_HPP_
#define _M4MODKEY_LICENSEMANAGER_HPP_

#include "globdec.hpp"

#include "clm4objservice.hpp"
#include "licensemanagerinterface.hpp"

#include "m4ipcs.hpp"
#include "blocksynchronization.hpp"

class ILicenseManager;
class ClLicense;
class ClLicenseThread;
class ClLicenseChannelLoader;

//----------------------------------------------
//Clase ClLicenseManagerBase
//----------------------------------------------
class ClLicenseManagerBase : public ILicenseManager {

protected:
	m4bool_t m_bMultiThread;
	ClM4ObjService * m_pM4ObjService;
	ClChannelManager * m_pClChannelManager;

	ClLicenseChannelLoader * m_pClLicenseChannelLoader;

	ClLicenseThread * m_pClLicenseThread;	

	m4bool_t m_bInitialized;

	m4char_t * m_pLicenseEncrypted;
	m4int32_t m_iLicenseEncryptedSize;

	ClMutex m_oSyncroState;

public:
	ClLicenseManagerBase( void );
	~ClLicenseManagerBase( );

	m4return_t Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier ) = 0;
	m4return_t Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier ) = 0;	
	m4return_t End( void );

	m4bool_t IsInitialized( void );

	eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void );
	m4return_t SetStateValidateExecutionNp ( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp );

	m4bool_t	ValidateLicense( void );
	m4pchar_t	GetpLicenseEncrypted( void );
	string		GetLicenseSummaryEncrypted( void );
	m4pcchar_t	GetPropertyValueById( m4pcchar_t ai_pPropertyId );
};

//----------------------------------------------
//Clase ClLicenseManagerServer
//----------------------------------------------
class ClLicenseManagerServer : public ClLicenseManagerBase {

public:
	ClLicenseManagerServer( void );
	~ClLicenseManagerServer( );

	m4return_t Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier );
	m4return_t Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier );	
};

//----------------------------------------------
//FIN Clase ClLicenseManagerServer
//----------------------------------------------

//----------------------------------------------
//Clase ClLicenseManagerClient
//----------------------------------------------
class ClLicenseManagerClient : public ClLicenseManagerBase {

protected:
	eStateValidateExecutionNp_t m_eStateValidateExecutionNp;

public:
	ClLicenseManagerClient( void );
	~ClLicenseManagerClient( );

	m4return_t Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier );
	m4return_t Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier );	

	eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void );
	m4return_t SetStateValidateExecutionNp ( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp );
};

//----------------------------------------------
//FIN Clase ClLicenseManagerClient
//----------------------------------------------

#endif


