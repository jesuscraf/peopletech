#include "dmres.hpp"
//#include "vmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             item.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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
#include "dm_def.hpp"
#include "m4define.hpp"
#include "item.hpp"
#include "itemconn.hpp"
#include "access.hpp"
#include "node_knl.hpp"
#include "medvarin.hpp"     // para la conversión de la fecha
#include "cltypes.hpp"
#include "cldefine.hpp"
#include "chan_knl.hpp"
#include "iexecutor.hpp"
#include "laneinfo.hpp"
#include "m4mdrt.hpp"


//## end module.includes preserve=yes

///////////////////////////////////////////////////////////////////////////////
//
// ClItem
//

//-----------------------------------------------------------------------------
// Contructor / Destructor
//-----------------------------------------------------------------------------

ClItem::ClItem (const ClItem &ai_item) : ClNoCallableItem (ai_item)
{
// Value
    Value._AttachX (ai_item.Value, this);
    OldValue._AttachX (ai_item.OldValue, this);

    m_pClAccess = ai_item.m_pClAccess;
}

void ClItem::_Copy (const ClItem &ai_item)
{
    ClNoCallableItem::_Copy (ai_item);

// Value
    Value._AttachX (ai_item.Value, this);
    OldValue._AttachX (ai_item.OldValue, this);

    m_pClAccess = ai_item.m_pClAccess;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// operator=
//----------------------------------------------------------------------------

ClItem &ClItem::operator= (const ClItem &ai_item)
{
    _Copy (ai_item);
    return *this;
}

//----------------------------------------------------------------------------
// operator++
//----------------------------------------------------------------------------

ClItem &ClItem::operator++ (void)
{
    Next ();
    return *this;
}

//-----------------------------------------------------------------------------
// operator --
//-----------------------------------------------------------------------------

ClItem &ClItem::operator-- (void)
{
    Prev ();
    return *this;
}

//-----------------------------------------------------------------------------
// Begin
//-----------------------------------------------------------------------------

ClItem &ClItem::Begin (void)
{
    ClItemIndex i (0);
    MoveTo (i);

    return *this;
}

//-----------------------------------------------------------------------------
// End
//-----------------------------------------------------------------------------

ClItem &ClItem::End (void)
{
    M4_ASSERT (m_pClRTItemIterator);

    ClItemIndex i = m_pClRTItemIterator->Count ();

    MoveTo (i);

    return *this;
}

//-----------------------------------------------------------------------------
// Call
//-----------------------------------------------------------------------------

m4return_t ClItem::Call (IExecutor *ai_pexecutor,m4int32_t ai_iArgs, m4bool_t ai_bFromWrapper/*=M4_FALSE*/, m4bool_t ai_bNoInsiteRec/*=M4_FALSE*/)
{

    m4return_t res;


    if( Index == M4DM_EOF_INDEX )
	{
		// el item no está posicionado correstamente
        ClHandle NodeHandle = m_pClNode_Data->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = m_pClNode_Data->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
        DUMP_CHLOG_ERRORF (M4_CH_DM_EXECUTINGITEM, M4ObjNodeN(pmcr, NodeHandle));
		return( M4_ERROR ) ;
	}

    // el item no es 'ejecutable', no pasa nada malo, pero devolvemos error
    if( ItemDef.Type() != M4CL_ITEM_TYPE_METHOD && ItemDef.Type() != M4CL_ITEM_TYPE_CONCEPT )
	{
        ClHandle NodeHandle = m_pClNode_Data->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = m_pClNode_Data->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
        DUMP_CHLOG_ERRORF (M4_CH_DM_BADITEMTYPE, M4ObjNodeN(pmcr, NodeHandle));
		return( M4_ERROR ) ;
	}


    if (0 == m_pClAccess)
    { //puede ser cero
        ClCompiledMCR * pmcr = ItemDef.GetpCMCR ();
        ClHandle ItemHandle = ItemDef.GetHandle ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_ITEMCALLNOACCESS, M4ObjNodeItemI(pmcr, ItemHandle));
        return M4_ERROR;
    }

    //si no viene, cojo el ejecutor del access:
    if (ai_pexecutor == 0)
	{
        ai_pexecutor = m_pClAccess->GetpExecutor();
	}

    M4_ASSERT (ai_pexecutor);
    M4_ASSERT (m_pClNode_Data);

    ClAccessRecordSet *prs = &(m_pClAccess->Node.Get (m_pClNode_Data->GetpExtInfo ()->Index)->RecordSet);
    M4_ASSERT (prs);

    m4uint16_t noa = ItemDef.NumberOfArguments ();
    M4_ASSERT (noa < 256);  //por el casting que viene a continuación:
    
	res = ai_pexecutor->Call( this, ai_iArgs, ai_bFromWrapper, ai_bNoInsiteRec ) ;

    return res;
}


//Se usa. Edu, no la quites por dios, que hace falta.
ClNode * ClItem::GetpNodeAccess (void)
{
    if (m_pClAccess)
    	return m_pClAccess->Node.Get (m_pClNode_Data->GetpExtInfo ()->Index);
    else
        return 0;
}


ClAccess* ClItem::GetpAccess (void) const
{
	return m_pClAccess;
}

//-----------------------------------------------------------------------------
// _NotifyChange
//-----------------------------------------------------------------------------

void ClItem::_NotifyChange (void)
{
    ClConnectItemInfo *pcii = GetpConnectItemInfo ();

    M4_ASSERT (pcii);

    m4uint16_t i, j, k;
    m4uint16_t nic = pcii->Count ();

    for (i = 0; i < nic; i++) {
        M4_ASSERT ((*pcii)[i]);

        (*pcii)[i]->SetChildValue (this);
    }

    ClNode * pnode = GetpNodeAccess();
    if (pnode)
    {
        if (pnode->GetpLane_Info())
        {
            for (i = 0; i < pnode->GetpLane_Info()->m_iDocs; i++)
            {
                ClLane_Item_Info * pLaneItInf = pnode->GetpLane_Info()->GetpLane_Item_Info_by_Doc(i);
                if (pLaneItInf->m_pConnectItemInfo)
                {
                    for (j = 0; j < pLaneItInf->m_iCount; j ++)
                    {
                        if (pLaneItInf->m_pHandles[j] == ItemDef.GetHandle())
                        {
                            nic = pLaneItInf->m_pConnectItemInfo[j].Count ();
                            for (k = 0; k < nic; k++)
                            {
                                pLaneItInf->m_pConnectItemInfo[j][k]->SetChildValue (this);
                            }
                        }
                    }
                }
            }
        }
    }
}

 
m4return_t 
ClItem::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClItem);
		ao_size-=sizeof(ClNoCallableItem);

	if (ClNoCallableItem::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

m4return_t	
ClItem::StackPush (const m4VariantType &ai_var){
	return m_pClAccess->GetpExecutor()->Stack.Push(ai_var);
}

m4return_t	
ClItem::StackPop  (m4VariantType &ao_var){
	return m_pClAccess->GetpExecutor()->Stack.Pop(ao_var);
}

void		
ClItem::StackReset(void){
	m_pClAccess->GetpExecutor()->Stack.Reset();
}

