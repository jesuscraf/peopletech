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


#ifndef	_M4MODKEY_XMLANALYZERFACTORY_HPP_
#define _M4MODKEY_XMLANALYZERFACTORY_HPP_

#include "globdec.hpp"

enum eObjectType_t {
	OBJECT_TYPE_UNDEFINED	= 0,
	OBJECT_TYPE_MODULE,
	OBJECT_TYPE_LICENSE
};

class IXmlAnalyzer;

//----------------------------------------------
//Clase ClXmlAnalyzerFactory
//----------------------------------------------
class M4_DECL_M4MODKEY ClXmlAnalyzerFactory {

public:
	static IXmlAnalyzer * CreateInstanceXmlAnalyzer( eObjectType_t ai_eObjectType );
	static void DestroyInstanceXmlAnalyzer( IXmlAnalyzer * & aio_pIXmlAnalyzer );
};
//----------------------------------------------
//FIN Clase ClXmlAnalyzerFactory
//----------------------------------------------

#endif


