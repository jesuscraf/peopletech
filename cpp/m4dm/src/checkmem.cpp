//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             checkmem.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Checking & measure memory utilities 
//
//
//==============================================================================




#include "access.hpp"
#include "channel.hpp"
#include "node.hpp"
//#include "ch_node.hpp"
#include "px_block.hpp"
#include "ch_block.hpp"
#include "aregister.hpp"
#include "itemsimp.hpp"
#include "node_knl.hpp"

#include "checkmem.hpp"
#ifdef _DEBUG 


	#define new DEBUG_CLIENTBLOCK
#endif



M4_DECL_M4DM StMemInfo g_StMemInfo;

StMemInfo::StMemInfo ()
{
    iAccess = 0;
    iChannel = 0;
    iNode_Channel = 0;
    iNode_Access = 0;
    iBlock_Channel = 0;
    iBlock_Proxy = 0;
    iRegister = 0;
    iItem = 0;

    iAccessSize = sizeof (ClAccess);
    iChannelSize = sizeof (ClChannel);
    iNode_ChannelSize = sizeof (ClNode_Data);
    iNode_AccessSize = sizeof (ClNode);
    iBlock_ChannelSize = sizeof (ClBlock_Channel);
    iBlock_ProxySize = sizeof (ClBlock_Channel_Proxy);
    iRegisterSize = sizeof (_ClRegister_Base);
    iItemSize = sizeof (ClNoCallableItem);
}
