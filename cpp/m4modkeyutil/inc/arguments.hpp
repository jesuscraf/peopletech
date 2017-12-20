//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkeyutil
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


#ifndef	_M4MODKEYUTIL_ARGUMENTS_HPP_
#define _M4MODKEYUTIL_ARGUMENTS_HPP_

#include "m4stl.hpp"

#include "globdec.hpp"
#include "switches.hpp"

#include "xmlanalyzerfactory.hpp"
#include "encrypt.hpp"

#include "m4date.hpp"

enum eOperation_t {
	OPERATION_TYPE_UNDEFINED	= 0,
	OPERATION_TYPE_ENCRYPTION,
	OPERATION_TYPE_DECRYPTION
};

enum ePermissionMask_t {
	PERMISSION_OPERATION_UNDEFINED		= 0,
	PERMISSION_OPERATION_ENCRYPT		= 1,
	PERMISSION_OPERATION_DECRYPT		= 2,
	PERMISSION_OPERATION_SUPERUSER		= 3		//Todos los bits a 1.
};


typedef vector<string>							VcOptionArgumentsString;
typedef VcOptionArgumentsString *				PVcOptionArgumentsString;
typedef VcOptionArgumentsString::iterator		itVcOptionArgumentsString;

class ClEncryption;

//----------------------------------------------
//Clase ClOptionArguments
//----------------------------------------------
class ClOptionArguments {	

private:
	static m4return_t _InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey );

protected:

	ePermissionMask_t m_ePermissionMask;

	m4int32_t m_iArgc;
	m4char_t ** m_pArgv;

	//Tenemos un vector con los strings que nos han metido como parametro.
	VcOptionArgumentsString * m_pVcOptionArgumentsString;

	m4bool_t m_bReWrite;

	eSwitchType_t m_eSwitchType;
	eObjectType_t m_eObjectType;		
	eOperation_t m_eOperationType;

	m4char_t * m_pTextBuffer;
	m4char_t * m_pEncryptedBuffer;
	m4char_t * m_pLicenseInfoBuffer;

	m4int32_t m_iTextSize;
	m4int32_t m_iEncryptedSize;
	m4int32_t m_iLicenseInfoSize;	

	m4char_t * m_pInputFileName;
	m4char_t * m_pInputFileNameLicenseInfo;
	m4char_t * m_pOutputFileName;
	m4char_t * m_pVerboseFileName;

	IXmlAnalyzer * m_pXmlAnalyzerInput;
	IXmlAnalyzer * m_pXmlAnalyzerLicenseInfo;	

	ClEncryption m_oClEncryption;

protected:
	void _DumpUsageCompleteVersion( void );
	void _DumpUsageInfoVersion( void );

	m4return_t _ParseOptionArgumentsString( const string & ai_sOptionArguments, VcOptionArgumentsString * ai_pVcOptionArgumentsString );

	void _FillOptionArguments( VcOptionArgumentsString * ai_pVcOptionArgumentsString );
	void _FillOptionArguments( m4int32_t ai_iArgc, m4char_t **ai_ppArgv );
	void _Reset( void );

	m4bool_t _IsInitilized( void );
		
	m4bool_t _PermissionInOperation( eOperation_t ai_eOperation );
	m4bool_t _IsSwitchAllowed( const m4char_t * ai_pSwitch );
	m4bool_t _IsSuperUser( void );

	void _DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );

	m4return_t _StartParse( void );

	m4return_t _Execute( void );	
	m4return_t _ExecuteObjectsLicense( IXmlAnalyzer * ai_pIXmlAnalyzer );	
	m4return_t _ExecuteGenerateLicense( IXmlAnalyzer * ai_pIXmlAnalyzer );
	m4return_t _ExecuteInfo( IXmlAnalyzer * ai_pIXmlAnalyzer );
	m4return_t _ExecuteInfoSummary( void );

	m4return_t _AnalyzeXmlFile( IXmlAnalyzer * ai_pIXmlAnalyzer, const m4char_t * ai_pFileBuffer );

public:
	ClOptionArguments( m4int32_t argc, char **argv );
	ClOptionArguments( const string & ai_sOptionArguments );

	~ClOptionArguments();

	void DumpUsage( void );		
	void DumpInfo( m4char_t * ai_pFileName );

	m4return_t StartParse( void );
};
//----------------------------------------------
//FIN Clase ClOptionArguments
//----------------------------------------------

#endif


