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
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "handles.hpp"
#include "wproxy.hpp"

#ifndef __ITEMDFIT_HPP__
#define __ITEMDFIT_HPP__

class ClNodeDef;
class ClColItemDef_Kernel;
class _AClItemProxy;

class M4_DECL_M4DM ClItemProxyIt : public _withitemproxy
{
public:
    ClItemProxyIt ();
    ClItemProxyIt (ClColItemDef_Kernel *ai_pClColItemDef_Kernel);
    ClItemProxyIt (ClColItemDef_Kernel &ai_ClColItemDef_Kernel);

    virtual ~ClItemProxyIt ();

    void Attach (ClColItemDef_Kernel *ai_pClColItemDef_Kernel);
    void Attach (ClColItemDef_Kernel &ai_ClColItemDef_Kernel);

    _AClItemProxy *operator [] (ClItemIndex ai_itemindex);
    _AClItemProxy *operator [] (ClHandle ai_handle);
    _AClItemProxy *operator [] (m4pcchar_t ai_pcId);

    _AClItemProxy *operator ->() const
    {
        return GetpItemProxy ();
    }
	m4return_t GetSize(ClSize &ao_size);

protected:
    ClColItemDef_Kernel *m_pClColItemDef_Kernel;    // mi papa
};

class M4_DECL_M4DM ClItemProxyIt_Random : public ClItemProxyIt
{
public:
    ClItemProxyIt_Random () : ClItemProxyIt () {}

    ClItemProxyIt_Random (ClColItemDef_Kernel *ai_pClColItemDef_Kernel) : ClItemProxyIt (ai_pClColItemDef_Kernel) {}
    ClItemProxyIt_Random (ClColItemDef_Kernel &ai_ClColItemDef_Kernel) : ClItemProxyIt (ai_ClColItemDef_Kernel) {}

   ~ClItemProxyIt_Random ();

    ClItemIndex Index;  // de momento

    m4uint16_t Count (void) const;

    _AClItemProxy *Begin (void);
    _AClItemProxy *End (void);    
    m4bool_t   Next (void);
    m4bool_t   Prev (void);

/*
    _AClItemProxy *operator [] (ClItemIndex ai_itemindex)
    {
        return ClItemProxyIt::operator [] (ai_itemindex);
    }
    
    _AClItemProxy *operator [] (ClHandle ai_handle)
    {
        return ClItemProxyIt::operator [] (ai_handle);
    }
    
    _AClItemProxy *operator [] (m4pcchar_t ai_pcId)
    {
        return ClItemProxyIt::operator [] (ai_pcId);
    }
*/
	m4return_t GetSize(ClSize &ao_size);

    const _AClItemProxy *operator ->() const
    {
        return ClItemProxyIt::operator -> ();
    }

private:
};

#endif // __ITEMDFIT_HPP__
