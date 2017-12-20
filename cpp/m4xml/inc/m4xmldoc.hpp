//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4xml
// File:                m4xmldoc.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                18-02-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la declaración de las funciones del objeto documento
//
//
//==============================================================================


#ifndef __M4XMLDOC_HPP__
#define __M4XMLDOC_HPP__


#include "resource.h"
#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"


XERCES_CPP_NAMESPACE_USE


XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
class xercesc::DOMDocument;
XERCES_CPP_NAMESPACE_END


//=============================================================================
//
// M4XmlDocument
//
//=============================================================================

class ATL_NO_VTABLE M4XmlDocument : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<M4XmlDocument, &CLSID_M4XmlDocument>,
	public IDispatchImpl<IM4XmlDocument, &IID_IM4XmlDocument, &LIBID_M4XMLLib>
{
public:
		M4XmlDocument( void ) ;
	virtual ~M4XmlDocument( void ) ;

DECLARE_REGISTRY_RESOURCEID(IDR_M4XMLDOCUMENT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(M4XmlDocument)
	COM_INTERFACE_ENTRY(IM4XmlDocument)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4XmlDocument
public:
	STDMETHOD(GetNextElement)(/*[out]*/ SHORT *ao_pbExists, /*[out]*/ BSTR *ao_psName, /*[out]*/ SAFEARRAY **ao_pasAttributes, /*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);
	STDMETHOD(Open)(/*[in]*/ BSTR ai_sXMLFileName, /*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);

protected:
	
	XercesDOMParser			m_oParser ;		// Parser
	xercesc::DOMDocument	*m_poDocument ;	// Documento actual
	DOMNode					*m_poNode ;		// Nodo DOM actual

};

#endif //__M4XMLDOC_HPP__
