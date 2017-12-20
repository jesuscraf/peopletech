//==============================================================================
//
// (c) Copyright 2002 Meta4 Spain S.A
// All rights reserved.
//
// Module:           
// File:            clseschnexport.cpp
// Project:         Mind3x
// Author:         
// Date:            
// Language:         C++
//
// Definition:
//   This module defines the code for import/export the session channel
//	 from/to a ClSessionContext object
//
//==============================================================================



#ifndef _CLSESCHNEXPORT_HPP_
#define _CLSESCHNEXPORT_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "sessioncontext.hpp"
#include "channel.hpp"

// interface

class M4_DECL_M4OEINIT IClSesChnExport 
{
public:
	virtual m4return_t Export(const ClChannel* ai_poSesChn, ClSessionContext &ao_sesCtx) = 0;
	virtual m4return_t Import(ClChannel* aio_poSesChn, ClSessionContext &ai_sesCtx) = 0;
};

class M4_DECL_M4OEINIT ClSesChnExport : public IClSesChnExport
{
public:
	virtual m4return_t Export(const ClChannel* ai_poSesChn, ClSessionContext &ao_sesCtx);
	virtual m4return_t Import(ClChannel* aio_poSesChn, ClSessionContext &ai_sesCtx);

private:
	virtual m4return_t SetItem(ClNode* ai_poNode, m4pcchar_t ai_pcItemId, m4pcchar_t ai_pcItemValue);
};


#endif //_CLSESCHNEXPORT_HPP_

