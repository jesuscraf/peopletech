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


#ifndef	_M4MODKEY_LICENSEMANAGERINTERFACE_HPP_
#define _M4MODKEY_LICENSEMANAGERINTERFACE_HPP_

#include "m4oeinit_dll.hpp"


class ClM4ObjService;
class ClLicense;
class ClExecuteNPNotifier;

//Tipos de resultados que tenemos cuando estamos validando Nps.
enum eStateValidateExecutionNp_t {
	STATE_VALIDATE_EXECUTION_NP_GREEN		= 0,
	STATE_VALIDATE_EXECUTION_NP_YELLOW,
	STATE_VALIDATE_EXECUTION_NP_RED
};

//----------------------------------------------
//Clase ILicenseManager
//----------------------------------------------
class M4_DECL_M4OEINIT ILicenseManager {

public:
	virtual ~ILicenseManager() {}

	virtual m4return_t Init( m4bool_t ai_bMultiThread, ClM4ObjService * ai_pM4ObjService, ClExecuteNPNotifier *ai_pNotifier ) = 0;	
	virtual m4return_t Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier ) = 0;	
	virtual m4return_t End( void ) = 0;

	virtual m4bool_t IsInitialized( void ) = 0;

	virtual eStateValidateExecutionNp_t GetStateValidateExecutionNp ( void ) = 0;
	virtual m4return_t SetStateValidateExecutionNp ( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp ) = 0;

	virtual m4bool_t	ValidateLicense( void ) = 0;
	virtual	m4pchar_t	GetpLicenseEncrypted( void ) = 0;
	virtual string		GetLicenseSummaryEncrypted( void ) = 0;
	virtual m4pcchar_t	GetPropertyValueById( m4pcchar_t ai_pPropertyId ) = 0;
};

//----------------------------------------------
//FIN Clase ILicenseManager
//----------------------------------------------

#endif


