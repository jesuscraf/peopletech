//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presfacsrv.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:  
//
//    This module defines the class of the server factory of presentations
//
//
//==============================================================================


#include "presfac.hpp"

#ifndef __PRESFACSRV_HPP__
#define __PRESFACSRV_HPP__

class ClCache ;
class ClChannelManager ;
class ClPresentationCache ;
class CInternalPresentationFactory ;
class ClPresentation;
class ClVMStatisticsWrapper;
class ClVMBaseEnv;

class M4_DECL_M4PRESENTATION ClPresentationFactoryServer : public ClPresentationFactory
{

protected:

	CInternalPresentationFactory	*m_poInternalPresFactory;
	virtual m4return_t  _InternalBuildPresentationFromId ( const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, ClPresentation* &ao_rpPresentation );

public:

	ClPresentationFactoryServer(void);

	~ClPresentationFactoryServer(void);
	
	m4return_t Init (ClVMBaseEnv * ai_poRegistry, ClCsTrace * ai_poCsTrace, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager, ClCacheDirectory * ai_poCacheDir, ClCache * ai_poPresCache);
	m4return_t End ();
};


#endif

