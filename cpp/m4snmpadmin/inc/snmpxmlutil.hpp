

#ifndef SNMPXMLUTIL_HPP
#define SNMPXMLUTIL_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/util/XercesDefs.hpp>

#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4snmpadmin_dll.hpp"

// Additional declarations.
XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class SAXParseException;
class DOMNode;
class DOMNodeList;
XERCES_CPP_NAMESPACE_END

// ---------------------------------------------------------------------------
//  class SnmpErrorHandler
//  Simple error handler deriviative to install on parser
// ---------------------------------------------------------------------------
class M4_DECL_M4SNMPADMIN SnmpErrorHandler : public ErrorHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SnmpErrorHandler();
    ~SnmpErrorHandler();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    m4bool_t hasErrors() const
    {
        return bHasErrors;
    }


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
    SnmpErrorHandler(const SnmpErrorHandler&);
    void operator=(const SnmpErrorHandler&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  bHasErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    m4bool_t    bHasErrors;
};



// ---------------------------------------------------------------------------
//  class SnmpDOMUtils
//  Utilities to parse DOM document.
// ---------------------------------------------------------------------------
class M4_DECL_M4SNMPADMIN SnmpDOMUtils
{
public:
    static const DOMNode* findNodeByAtt(const DOMNodeList *ai_list, const XMLCh *ai_attname, m4pcchar_t ai_attval);
    static const DOMNode* findChildNodeByName(const DOMNode *ai_node, m4pcchar_t ai_name);
    static char* getAttVal(const DOMNode *ai_node, const XMLCh *ai_attname);

};


#endif // SNMPXMLUTIL_HPP

