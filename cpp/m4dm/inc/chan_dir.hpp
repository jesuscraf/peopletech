//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             chan_dir.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             17/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __CHAN_DIR__HPP__
#define __CHAN_DIR__HPP__
//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module.includes preserve=yes

//## begin module.additionalDeclarations preserve=yes
class ClChannel;
class ClCMCRFactory;
class ClChannelManager;
class ClSize;
//## end module.additionalDeclarations

class M4_DECL_M4DM ClChannelDirectory
{
public:
    ClChannelDirectory ();
   ~ClChannelDirectory ();

    ClChannel *operator [] (m4uint32_t ai_index);
    //ClChannel *operator -> (void);
    //ClChannel &operator * (void);

    m4uint32_t Count (void);

	m4return_t Attach (ClChannel *ai_oChannel, m4bool_t ai_bFirst = M4_FALSE);
	m4return_t Detach (ClChannel *ai_oChannel);

    m4return_t Clear (void);

	m4return_t GetSize(ClSize &ao_size);

	m4uint32_t GetNumberOfChannels();

//## begin ClChannelDirectory.initialDeclarations preserve=yes
private:
    void _NotifyGroup(ClChannel * const ai_pChannel, m4bool_t const ai_bDelete = M4_TRUE);
	typedef vector<ClChannel *> LISTCHANNEL;
//## end ClChannelDirectory.initialDeclarations
	LISTCHANNEL m_ListChannel;
};

#endif  //__CHAN_DIR__HPP__ 
