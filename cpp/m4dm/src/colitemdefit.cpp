#include "colitemdefit.hpp"

    #include "nodedef.hpp"
    #include "node_knl.hpp"
#include "item_prx.hpp"


//haciendo la vida más fácil a la peña:

void ClItemDefIt_Random::Attach (const ClNode_Kernel *ai_pnk)
{
    M4_ASSERT (ai_pnk->IsValid ());

    Attach (ai_pnk->GetpCMCR (), (ClColItemDef_Kernel *)&ai_pnk->ColItemDef_Kernel);
}

ClItemDef &ClItemDefIt_Random::operator [] (ClItemIndex ai_itemindex)
{
    ClItemProxyIt_Random::operator [] (ai_itemindex);
    ItemDef.SetHandle (GetpItemProxy ()->GetHandle ());
    Index = ItemDef.Index ();

    return ItemDef;
}

ClItemDef &ClItemDefIt_Random::operator [] (ClHandle ai_handle)
{
    ClItemProxyIt_Random::operator [] (ai_handle);
    ItemDef.SetHandle (GetpItemProxy ()->GetHandle ());
    Index = ItemDef.Index ();
    
    return ItemDef;
}

ClItemDef &ClItemDefIt_Random::operator [] (m4pcchar_t ai_pcId)
{
    ClItemProxyIt_Random::operator [] (ai_pcId);
    ItemDef.SetHandle (GetpItemProxy ()->GetHandle ());
    Index = ItemDef.Index ();
    
    return ItemDef;
}

ClItemDef &ClItemDefIt_Random::Begin (void)
{
    ClItemProxyIt_Random::Begin ();
    ItemDef.SetHandle (GetpItemProxy ()->GetHandle ());
    
    return ItemDef;
}

m4bool_t ClItemDefIt_Random::Next (void)
{
    m4bool_t bisnoteof = ClItemProxyIt_Random::Next ();
    ItemDef.SetHandle (GetpItemProxy ()->GetHandle ());
    
    return bisnoteof;
}
m4return_t 
ClItemDefIt_Random::GetSize(ClSize &ao_size)
{
	ao_size +=sizeof(ClItemDefIt_Random);

	ao_size -=sizeof(ClItemProxyIt_Random);
	if (ClItemProxyIt_Random::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	ao_size -=sizeof(ItemDef);
	if (ItemDef.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;

}
