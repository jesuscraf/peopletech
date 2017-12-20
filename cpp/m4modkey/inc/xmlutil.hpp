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


#ifndef	_M4MODKEY_XMLUTIL_HPP_
#define _M4MODKEY_XMLUTIL_HPP_


#include "m4types.hpp"

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class SAXParseException;
XERCES_CPP_NAMESPACE_END


//Simplemente derivamos la clase.
class ModKeyErrorHandler : public ErrorHandler
{
public:
    ModKeyErrorHandler();
    ~ModKeyErrorHandler();

	m4bool_t HasErrors();
    // -----------------------------------------------------------------------
    //  Implementation of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& e);
    void error(const SAXParseException& e);
    void fatalError(const SAXParseException& e);
    void resetErrors();

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ModKeyErrorHandler(const ModKeyErrorHandler&);
    void operator=(const ModKeyErrorHandler&);

	m4bool_t m_bHasErrors;
};

//Utilidades para el parseo del documento.
class ModKeyDOMUtils
{
public:
    static DOMNode* ModKeyFindChildNodeByName(const DOMNode* ai_node, m4pcchar_t ai_name);
    static m4char_t* ModKeyGetAttVal(const DOMNode* ai_node, m4pcchar_t ai_attname);
};

#endif


