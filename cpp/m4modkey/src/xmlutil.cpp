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

#include "xmlutil.hpp"
#include "m4string.hpp"
#include "m4unicode.hpp"

//--------------------------------------------------
// MANEJADOR DE ERRORES
//--------------------------------------------------
ModKeyErrorHandler::ModKeyErrorHandler()
{
	m_bHasErrors = M4_FALSE;
}

ModKeyErrorHandler::~ModKeyErrorHandler()
{}

m4bool_t ModKeyErrorHandler::HasErrors()
{
	return m_bHasErrors;
}

void ModKeyErrorHandler::error(const SAXParseException& e)
{
	m_bHasErrors = M4_TRUE;

	// UNICODE XML
	m4pchar_t	pcFile = M4XMLToCpp( (M4XMLCh*) e.getSystemId() ) ;
    printf("%s at file %s, line %d, col %d\n", "Error", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
}

void ModKeyErrorHandler::fatalError(const SAXParseException& e)
{
	m_bHasErrors = M4_TRUE;

	// UNICODE XML
	m4pchar_t	pcFile = M4XMLToCpp( (M4XMLCh*) e.getSystemId() ) ;
    printf("%s at file %s, line %d, col %d\n", "FatalError", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
}

void ModKeyErrorHandler::warning(const SAXParseException& e)
{
	m_bHasErrors = M4_TRUE;

	// UNICODE XML
	m4pchar_t	pcFile = M4XMLToCpp( (M4XMLCh*) e.getSystemId() ) ;
    printf("%s at file %s, line %d, col %d\n", "Warning", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
}

void ModKeyErrorHandler::resetErrors()
{
    m_bHasErrors = M4_FALSE;
}
//--------------------------------------------------
//	FIN MANEJADOR DE ERRORES
//--------------------------------------------------

//--------------------------------------------------
//Utilidades para lectura de DOM

DOMNode* ModKeyDOMUtils::ModKeyFindChildNodeByName(const DOMNode* ai_node, m4pcchar_t ai_name)
{
    DOMNode *n = NULL;
	// UNICODE XML
    m4char_t * s2 = NULL;
    DOMNodeList *dnlist = ai_node->getChildNodes();
    m4uint32_t listLength = dnlist->getLength();
	m4int32_t iCompare = 0;

    for (m4uint32_t i=0; i<listLength; i++)
    {
        n = dnlist->item(i);
		// UNICODE XML
        s2 = M4XMLToCpp( (M4XMLCh*) n->getNodeName() );
		iCompare = strcmpi(ai_name, s2);
		delete [] s2;

        if (iCompare == 0)
        {
            return n;
        }
    }

    return NULL;
}

m4char_t* ModKeyDOMUtils::ModKeyGetAttVal(const DOMNode* ai_node, m4pcchar_t ai_attname)
{
	//Ojo, retorna un puntero a un buffer que el invocante debera liberar!.

    DOMNamedNodeMap *nnm = ai_node->getAttributes();

    XMLCh* s1 = (XMLCh*) M4CppToXML( ai_attname );
    DOMNode *n = nnm->getNamedItem(s1);
	delete s1;

    if (n == NULL)
    {
        return NULL;
    }

	// UNICODE XML
	return( M4XMLToCpp( (M4XMLCh*) n->getNodeValue() ) );
}

