//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presfaccli.hpp   
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
//    This module defines the class of the client factory of presentations
//
//
//==============================================================================


#include "presfac.hpp"
#include "m4stl.hpp"

#ifndef __PRESFACCLI_HPP__
#define __PRESFACCLI_HPP__

class ClCache ;
class ClPresentationCache ;
class ClPresentation;
class ClVMStatisticsWrapper;
class ClVMBaseEnv;
class ClMDAdaptor;


class M4_DECL_M4PRESENTATION ClPresentationFactoryClient : public ClPresentationFactory
{

protected:

	ClMDAdaptor				*m_poMDAdaptor;

	virtual m4return_t _InternalBuildPresentationFromId ( const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, ClPresentation* &ao_rpPresentation );	

public:

	ClPresentationFactoryClient (void);

	~ClPresentationFactoryClient (void);
	
	m4return_t Init (ClVMBaseEnv * ai_poRegistry, ClCsTrace * ai_poCsTrace, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager, ClMDAdaptor * ai_poMDAdaptor, ClCacheDirectory * ai_poCacheDir, ClCache * ai_poPresCache);
	m4return_t End ();
};



#endif

