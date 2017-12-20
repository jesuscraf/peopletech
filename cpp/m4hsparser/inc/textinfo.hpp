//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                textinfo.hpp
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
//    Definición de un elemento literal en una sentencia select
//
//
//==============================================================================


#if !defined(AFX_TEXTINFO_HPP__0A019673_FCF7_11D4_BEC0_00C04FA913F4__INCLUDED_)
#define AFX_TEXTINFO_HPP__0A019673_FCF7_11D4_BEC0_00C04FA913F4__INCLUDED_


#include "elementinfo.hpp"


class TextInfo : public ElementInfo
{

public:

	TextInfo (m4pcchar_t ai_pccText);
	virtual ~TextInfo();

protected:

	virtual m4void_t MyDump (ostream & ai_oStream);

private:

	m4pchar_t m_pcText;

};

#endif // !defined(AFX_TEXTINFO_HPP__0A019673_FCF7_11D4_BEC0_00C04FA913F4__INCLUDED_)
