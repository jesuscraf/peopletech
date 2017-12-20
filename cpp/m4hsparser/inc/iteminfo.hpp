//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                iteminfo.hpp
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
//    Definición de un item en una sentencia select
//
//
//==============================================================================

#if !defined(AFX_ITEMINFO_HPP__9F7E9D11_FC5B_11D4_BEBF_00C04FA913F4__INCLUDED_)
#define AFX_ITEMINFO_HPP__9F7E9D11_FC5B_11D4_BEBF_00C04FA913F4__INCLUDED_


#include "elementinfo.hpp"


class ItemInfo : public ElementInfo 
{
public:
	ItemInfo (m4pcchar_t pccItemName);
	virtual ~ItemInfo();

	m4void_t SetSessionChannel (m4bool_t ai_bIsSessionChannel );

protected:
	
	virtual m4void_t MyDump (ostream & ai_oStream);

private:

	// Id del item
	m4pchar_t m_pcItemName;
	m4bool_t m_bIsSessionChannel;

};

#endif // !defined(AFX_ITEMINFO_HPP__9F7E9D11_FC5B_11D4_BEBF_00C04FA913F4__INCLUDED_)
