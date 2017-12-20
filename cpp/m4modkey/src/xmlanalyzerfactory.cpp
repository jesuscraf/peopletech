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

#include "xmlanalyzerfactory.hpp"
#include "xmlanalyzer.hpp"

//La factoria se encargara de crear los objetos analizadores.
//Por el momento solamamente tenemos una implementacion que cumple el interfaz.
//Se trata del analizador xml.
IXmlAnalyzer * ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( eObjectType_t ai_eObjectType )
{
	IXmlAnalyzer * pIXmlAnalyzer = NULL;

	switch (ai_eObjectType)
	{
	case OBJECT_TYPE_MODULE:
		pIXmlAnalyzer = new ClXmlAnalyzerModule();

		break;

	case OBJECT_TYPE_LICENSE:
		pIXmlAnalyzer = new ClXmlAnalyzerLicense();

		break;

	default:
		break;
	}

	return pIXmlAnalyzer;
}

void ClXmlAnalyzerFactory::DestroyInstanceXmlAnalyzer( IXmlAnalyzer * & aio_pIXmlAnalyzer )
{
	if (aio_pIXmlAnalyzer != NULL) {
		delete aio_pIXmlAnalyzer;
	}
	aio_pIXmlAnalyzer = NULL;
}
