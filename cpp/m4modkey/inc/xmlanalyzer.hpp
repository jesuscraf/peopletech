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


#ifndef	_M4MODKEY_XMLANALYZER_HPP_
#define _M4MODKEY_XMLANALYZER_HPP_

#include "xmlutil.hpp"
#include "xmlanalyzerinterface.hpp"
#include "license.hpp"

class ClXmlTagContainer;
class ClEncryption;




//----------------------------------------------
//Clase ClXmlSystemInitializer
//----------------------------------------------
class ClXmlSystemInitializer {
public:
	ClXmlSystemInitializer( void );
	~ClXmlSystemInitializer();
};

//----------------------------------------------
//FIN Clase ClXmlSystemInitializer
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlAnalyzer
//----------------------------------------------
class ClXmlAnalyzer : public IXmlAnalyzer {
protected:

	ClXmlTagContainer * m_ClXmlTagContainerRoot;

protected:
	virtual m4return_t _MergeXml( IXmlAnalyzer* ai_pIXmlAnalyzer, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged ) = 0;
	
	virtual m4return_t _ReadXmlFile( DOMElement* ai_pRoot ) = 0;

	virtual m4return_t _RegisterAttributes( ClXmlTagContainer* ai_pXmlTagContainerToAdd, DOMNode* ai_pNode, const m4int16_t ai_iLabelIndex ) = 0;

public:
	ClXmlAnalyzer( void );	
	virtual ~ClXmlAnalyzer();

	//Primero analizaremos el buffer para extraer la informacion.
	virtual m4return_t AnalyzeXmlFile( const m4char_t* ai_pFileBuffer );

	virtual m4return_t MergeXml( IXmlAnalyzer* ai_pIXmlAnalyzer, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged );

	//----------------------------------------------------	
	//Podemos extraer la licencia o el objeto a securizar.
	const ClLicense * GetpClLicense ( void ) const = 0;
	const ClLicense * GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted ) = 0;

	m4int32_t GetNumSecurizedModules ( void ) const = 0;
	const ClSecurizedModule * GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const = 0;
	//----------------------------------------------------

	virtual void DumpInfo( FILE* ai_pFile, m4int32_t ai_iLevel ) = 0;
};
//----------------------------------------------
//FIN Clase ClXmlAnalyzer
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlAnalyzerModule
//----------------------------------------------
class ClXmlAnalyzerModule : public ClXmlAnalyzer {
protected:

	PVcClSecurizedModule m_pVcSecurizedModule;	

protected:
	m4return_t _MergeXml( IXmlAnalyzer* ai_pIXmlAnalyzer, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged );

	m4return_t _ReadXmlFile( DOMElement* ai_pRoot );

	m4return_t _RegisterAttributes( ClXmlTagContainer* ai_pXmlTagContainerToAdd, DOMNode* ai_pNode, const m4int16_t ai_iLabelIndex );

public:
	ClXmlAnalyzerModule( void );	
	~ClXmlAnalyzerModule();

	//----------------------------------------------------
	//Podemos extraer la licencia o el objeto a securizar.
	const ClLicense * GetpClLicense ( void ) const;	
	const ClLicense * GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted );

	m4int32_t GetNumSecurizedModules ( void ) const;
	const ClSecurizedModule * GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const;
	//----------------------------------------------------

	void DumpInfo( FILE* ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClXmlAnalyzerModule
//----------------------------------------------


//----------------------------------------------
//Clase ClXmlAnalyzerLicense
//----------------------------------------------
class ClXmlAnalyzerLicense : public ClXmlAnalyzer {

private:
	static m4return_t _InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey );

protected:
	ClLicense * m_pClLicense;

protected:
	m4return_t _MergeXml( IXmlAnalyzer* ai_pXmlAnalyzerModule, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged );

	m4return_t _ReadXmlFile( DOMElement* ai_pRoot );

	m4return_t _RegisterAttributes( ClXmlTagContainer* ai_pXmlTagContainerToAdd, DOMNode* ai_pNode, const m4int16_t ai_iLabelIndex );

public:
	ClXmlAnalyzerLicense( void );	
	~ClXmlAnalyzerLicense();

	//----------------------------------------------------
	//Podemos extraer la licencia o el objeto a securizar.
	const ClLicense * GetpClLicense ( void ) const;
	const ClLicense * GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted );

	m4int32_t GetNumSecurizedModules ( void ) const;
	const ClSecurizedModule * GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const;
	//----------------------------------------------------

	void DumpInfo( FILE* ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClXmlAnalyzerLicense
//----------------------------------------------

#endif


