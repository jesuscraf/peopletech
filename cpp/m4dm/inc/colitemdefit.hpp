//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itemdfit.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             29/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines ...
//
//
//==============================================================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "itemdef.hpp"
#include "itemdfit.hpp"
//## end module.includes preserve=yes

#ifndef __COLITEMDEFIT_HPP__
#define __COLITEMDEFIT_HPP__

//## begin module.additionalDeclarations preserve=yes
class ClNodeDef;
class ClColItemDef_Kernel;
class ClNode_Kernel;
//## end module.additionalDeclarations

class M4_DECL_M4DM ClItemDefIt_Random : public ClItemProxyIt_Random
{
public:
    ClItemDefIt_Random () {}

    ClItemDefIt_Random (const ClNode_Kernel *ai_pnk)
    {
        Attach (ai_pnk);
    }

    ClItemDefIt_Random (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pClColItemDef_Kernel) 
    {
        Attach (ai_pcmcr, ai_pClColItemDef_Kernel);
    }

    ClItemDefIt_Random (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel &ai_ClColItemDef_Kernel)
    {
        Attach (ai_pcmcr, ai_ClColItemDef_Kernel);
    }

    void Attach (const ClNode_Kernel *ai_pnk);

    void Attach (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pClColItemDef_Kernel)
    {
        ClItemProxyIt_Random::Attach (ai_pClColItemDef_Kernel);
        ItemDef.Attach (ai_pcmcr);
    }

    void Attach (ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel &ai_ClColItemDef_Kernel)
    {
        ClItemProxyIt_Random::Attach (ai_ClColItemDef_Kernel);
        ItemDef.Attach (ai_pcmcr);
    }

   ~ClItemDefIt_Random ()
    {
    }

    //ClItemIndex Index;  // de momento
    ClItemDef ItemDef;

    ClItemDef &operator [] (ClItemIndex ai_itemindex);
    ClItemDef &operator [] (ClHandle ai_handle);
    ClItemDef &operator [] (m4pcchar_t ai_pcId);

    const ClItemDef *operator ->() const
    {
        return &ItemDef;
    }

    ClItemDef &Begin (void);

    m4bool_t   Next (void);

	m4return_t GetSize(ClSize &ao_size);


/*
    //añadido:
    ClConnectItemInfo *GetpConnectItemInfo (void);
    {
        return (_AClItemProxy *)m_ipit.operator -> ();
    }
*/

private:
    //ClItemDef &_k (void);
};

#endif // __COLITEMDEFIT_HPP__
