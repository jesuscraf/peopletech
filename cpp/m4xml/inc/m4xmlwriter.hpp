//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4xml
// File:                m4xmlwriter.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                26-01-2007
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la declaración de las funciones del objeto writer
//
//
//==============================================================================


#ifndef __M4XMLWRITER_HPP__
#define __M4XMLWRITER_HPP__


#include <stdio.h>
#include "resource.h"


//=============================================================================
//
// M4XmlWriter
//
//=============================================================================

class ATL_NO_VTABLE M4XmlWriter : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<M4XmlWriter, &CLSID_M4XmlWriter>,
	public IDispatchImpl<IM4XmlWriter, &IID_IM4XmlWriter, &LIBID_M4XMLLib>
{
public:
		M4XmlWriter( void ) ;
	virtual ~M4XmlWriter( void ) ;

DECLARE_REGISTRY_RESOURCEID(IDR_M4XMLWRITER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(M4XmlWriter)
	COM_INTERFACE_ENTRY(IM4XmlWriter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4XmlWriter
public:
	STDMETHOD(EndElement)(/*[in]*/ BSTR ai_sName, /*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);
	STDMETHOD(StartElement)(/*[in]*/ BSTR ai_sName, /*[in]*/ SAFEARRAY **ai_pasAttributes, /*[in]*/ SHORT ai_bClose, /*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);
	STDMETHOD(Close)(/*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);
	STDMETHOD(Open)(/*[in]*/ BSTR ai_sXMLFileName, /*[out]*/ BSTR *ao_psErrorMessage, /*[out, retval]*/ LONG *ao_plResult);

protected:

	int	_Close( void ) ;
	
	FILE*	m_pfFile ;
	BSTR	m_sFileName ;

};

#endif //__M4XMLWRITER_HPP__

