
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

#include "snmpxmlutil.hpp"
#include "m4string.hpp"
#include "snmpres.hpp"
#include "m4unicode.hpp"


// ---------------------------------------------------------------------------
//  SnmpErrorHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
SnmpErrorHandler::SnmpErrorHandler()
{
    bHasErrors = M4_FALSE;
}

SnmpErrorHandler::~SnmpErrorHandler()
{}

void SnmpErrorHandler::error(const SAXParseException& e)
{
    bHasErrors = M4_TRUE;
	// UNICODE XML
    m4pchar_t pcFile = M4XMLToCpp( (M4XMLCh*)e.getSystemId() );

    m4char_t msg[256];
    sprintf(msg, "%s at file %s, line %d, col %d", "Error", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
    DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSER_ERROR, msg);
}

void SnmpErrorHandler::fatalError(const SAXParseException& e)
{
    bHasErrors = M4_TRUE;
	// UNICODE XML
    m4pchar_t pcFile = M4XMLToCpp( (M4XMLCh*)e.getSystemId() );

    m4char_t msg[256];
    sprintf(msg, "%s at file %s, line %d, col %d", "FatalError", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
    DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSER_ERROR, msg);
}

void SnmpErrorHandler::warning(const SAXParseException& e)
{
	// UNICODE XML
    m4pchar_t pcFile = M4XMLToCpp( (M4XMLCh*)e.getSystemId() );

    m4char_t msg[256];
    sprintf(msg, "%s at file %s, line %d, col %d", "Warning", pcFile, e.getLineNumber(), e.getColumnNumber());
	delete [] pcFile ;
    DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSER_ERROR, msg);
}

void SnmpErrorHandler::resetErrors()
{
    bHasErrors = M4_FALSE;
}


// ---------------------------------------------------------------------------
//  class SnmpDOMUtils
//  Utilities to parse DOM document.
// ---------------------------------------------------------------------------
const DOMNode* SnmpDOMUtils::findNodeByAtt(const DOMNodeList *ai_list, const XMLCh *ai_attname, m4pcchar_t ai_attval)
{
    DOMNode *n1 = NULL, *n2 = NULL;
    DOMNamedNodeMap *nnm = NULL;
    short nodeType = 0;
	// UNICODE XML
    char* s2 = NULL;
	m4int32_t iCompare = 0;

    for (m4uint32_t i=0; i<ai_list->getLength(); i++)
    {
        n1 = ai_list->item(i);
        nodeType = n1->getNodeType();
        if (DOMNode::ELEMENT_NODE == nodeType)
        {
            nnm = n1->getAttributes();
            n2 = nnm->getNamedItem(ai_attname); 

			// bg 292577. Avoid server crashing.
			if( n2 == NULL )
			{
				// Traces the node attributes in order to debug its origin.
				unsigned int iLen = nnm->getLength();
				DOMNode *oNode = NULL;
				string sNodeInfo = "";
				char* pcTmp;

				for( m4uint32_t iNode = 0; iNode < iLen; iNode++ )
				{
					oNode = nnm->item( iNode );

					pcTmp = M4XMLToCpp( (M4XMLCh*)oNode->getNodeName() );
					sNodeInfo += pcTmp;
					delete pcTmp;

					sNodeInfo += "=";

					pcTmp = M4XMLToCpp( (M4XMLCh*)oNode->getNodeValue() );
					sNodeInfo += pcTmp; 
					delete pcTmp;

					if( iNode < iLen - 1 )
					{
						sNodeInfo += ", ";
					}
				}

				pcTmp = M4XMLToCpp( (M4XMLCh*)ai_attname );
				DUMP_CHLOG_ERRORF( M4_SNMP_ATTRIBULTE_NOT_FOUND, pcTmp << i << sNodeInfo );
				delete pcTmp;
			}
			else
			{
				// UNICODE XML
				s2 = M4XMLToCpp( (M4XMLCh*)n2->getNodeValue() );

				iCompare = stricmp(ai_attval, s2);
				delete s2;

				if (iCompare == 0)
				{
					return n1;
				}
			}
        }
    }

    return NULL;
}


const DOMNode* SnmpDOMUtils::findChildNodeByName(const DOMNode *ai_node, m4pcchar_t ai_name)
{
    DOMNode *n = NULL;
	// UNICODE XML
    char* s2 = NULL;
    DOMNodeList *dnlist = ai_node->getChildNodes();
    m4uint32_t listLength = dnlist->getLength();
	m4int32_t iCompare = 0;

    for (m4uint32_t i=0; i<listLength; i++)
    {
        n = dnlist->item(i);
		// UNICODE XML
        s2 = M4XMLToCpp( (M4XMLCh*)n->getNodeName() );

		iCompare = stricmp(ai_name, s2);
        delete s2;

        if (iCompare == 0)
        {
            return n;
        }
    }

    return NULL;
}

char* SnmpDOMUtils::getAttVal(const DOMNode *ai_node, const XMLCh *ai_attname)
{
    DOMNamedNodeMap *nnm = ai_node->getAttributes();
    DOMNode *n = nnm->getNamedItem(ai_attname);
    if (n == 0)
    {
        return NULL;
    }

	// UNICODE XML
    return M4XMLToCpp( (M4XMLCh*)n->getNodeValue() );
}

