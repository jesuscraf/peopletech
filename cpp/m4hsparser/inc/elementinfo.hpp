//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                elementinfo.hpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//		Definición de un elemento de una select.
//		En determinados puntos de un objeto select, los elemntos 
//		pueden formar un árbol binario entre si.
//
//
//==============================================================================


#if !defined(AFX_ELEMENTINFO_HPP__F8D805C1_FCDC_11D4_BEC0_00C04FA913F4__INCLUDED_)
#define AFX_ELEMENTINFO_HPP__F8D805C1_FCDC_11D4_BEC0_00C04FA913F4__INCLUDED_


#include "m4types.hpp"
#include "m4stl.hpp"



typedef enum {	ELEMENTINFO, 
				COLUMNINFO, 
				FUNCTIONINFO, 
				ITEMINFO,
				JOININFO,
				SELECTINFO,
				TABLEINFO, 
				TEXTINFO		} M4HSPARSER_ELEMENT_T;



class ElementInfo  
{

public:

	ElementInfo();
	virtual ~ElementInfo();

	m4void_t AddLeft (ElementInfo * ai_poElementInfo);
	m4void_t AddRight (ElementInfo * ai_poElementInfo);

	ElementInfo * GetLeft ();
	ElementInfo * GetRight ();	

	m4void_t PushLeft (ElementInfo * ai_poElementInfo);
	m4void_t PushRight (ElementInfo * ai_poElementInfo);

	virtual m4void_t Dump (ostream & ai_oStream);

	virtual m4void_t ChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

	M4HSPARSER_ELEMENT_T GetType ();

protected:

	virtual m4void_t MyDump (ostream & ai_oStream) = 0;
	
	virtual m4void_t MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

	M4HSPARSER_ELEMENT_T m_eType;

	ElementInfo * m_poElementInfoRight;
	ElementInfo * m_poElementInfoLeft;

};

#endif // !defined(AFX_ELEMENTINFO_HPP__F8D805C1_FCDC_11D4_BEC0_00C04FA913F4__INCLUDED_)
