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


#ifndef	_M4MODKEY_XMLANALYZERINTERFACE_HPP_
#define _M4MODKEY_XMLANALYZERINTERFACE_HPP_

#include "globdec.hpp"

class ClLicense;
class ClSecurizedModule;

//----------------------------------------------
//Clase IXmlAnalyzer
//----------------------------------------------
class M4_DECL_M4MODKEY IXmlAnalyzer {

public:
	virtual ~IXmlAnalyzer() {}

	virtual m4return_t AnalyzeXmlFile( const m4char_t * ai_pFileBuffer ) = 0;

	virtual m4return_t MergeXml( IXmlAnalyzer * ai_pIXmlAnalyzer, m4pchar_t & ao_pBufferMerged, m4int32_t & ao_iSizeMerged ) = 0;

	virtual const ClLicense * GetpClLicense ( void ) const = 0;
	virtual const ClLicense * GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted ) = 0;

	virtual m4int32_t GetNumSecurizedModules ( void ) const = 0;
	virtual const ClSecurizedModule * GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const = 0;

	virtual void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel ) = 0;
};
//----------------------------------------------
//FIN Clase IXmlAnalyzer
//----------------------------------------------

#endif


