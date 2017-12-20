//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             acno_dir.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             19/09/97
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

//## begin module.includes preserve=yes
#include "anoddir.hpp"
#include "node_x.hpp"
//## end module.includes preserve=yes


#ifndef __ACNO_DIR__HPP__
#define __ACNO_DIR__HPP__

class ClChannelDef;
class ClNodeDirectory_Data;

///////////////////////////////////////////////////////////////////////////////
//
// ClNodeAccessDirectory
//

class ClNodeAccessDirectory : public ClNodeDirectory<ClNode>
{
public:    
    m4return_t Init (ClAccess *ai_paccess, m4uint16_t nnodes);
    void Attach (ClChannelDef *pcd, ClNodeDirectory_Data *pnd);

    void Destroy (void);

    m4return_t SetAutoLoadMode (m4uint8_t ai_iautoload);
	m4return_t GetSize(ClSize &ao_size);

};

#endif  //__ACNO_DIR__HPP__
