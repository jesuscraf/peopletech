//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             acno_dir.inl
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

#include "acno_dir.hpp"

#include "access.hpp"
//#include "channel.hpp"
#include "node_knl.hpp"
#include "nod_data.hpp"
//#include "chan_knl.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// ClNodeAccessDirectory
//

m4return_t ClNodeAccessDirectory::Init (ClAccess *ai_paccess, m4uint16_t nnodes)
{
    m4uint16_t j;

    //nnodes = pcd->NumberOfNodes ();

    // apuntamos el buffer al final del access:
    ClNode *pbuffer = (ClNode *)(ai_paccess + 1);
    
    // llamamos a todos los constructores del array, pasandoles la dirección que tienen 
    // asignada:
    for (j = 0; j < nnodes; j++) {        
        new (pbuffer + j) ClNode (ai_paccess);
    }

    // ahora ya estamos preparamos para llamar al attach de la clase base:
    ClNodeDirectory<ClNode>::Attach ((ClNode *)pbuffer);

    return M4_SUCCESS;
}

void ClNodeAccessDirectory::Attach (ClChannelDef *pcd, ClNodeDirectory_Data *pnd)
{
    M4_ASSERT (pcd);
    M4_ASSERT (pnd);

    ClNodeDirectory<ClNode>::Attach (pcd);
    
    ClNodeIndex i = 0;
    m4uint16_t nnodes = pnd->Count ();
    
    M4_ASSERT (Count () == pnd->Count ());
        //sino estamos en lios

    for (; i < nnodes; i++) {
        //operator [] (i).Init (ai_paccess, pnd->Get (i));
        operator [] (i).Attach (pnd->Get (i));
    }
}

void ClNodeAccessDirectory::Destroy (void)
{
    // llamamos a manubrio a los destructores del array de nodos:
    m4uint16_t nnodes = Count ();
    ClNodeIndex i = 0;
    
    for (; i < nnodes; i++) {
        delete &(operator [] (i));
    }

    //_Destroy ();
}

m4return_t ClNodeAccessDirectory::SetAutoLoadMode (m4uint8_t ai_iautoload)
{
    m4return_t res = M4_SUCCESS;

    m4uint16_t nnodes = Count ();
    ClNodeIndex i = 0;

    for (; i < nnodes; i++) {
        res = operator [](i).SetAutoLoadMode_fromAccess (ai_iautoload);
    }

    return res;
}
m4return_t 
ClNodeAccessDirectory::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNodeAccessDirectory);
		ao_size-=sizeof(ClNodeDirectory<ClNode>);

	if (ClNodeDirectory<ClNode>::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}
