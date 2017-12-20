//## begin module.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module.cm

//## begin module.cp preserve=no
//## end module.cp

//## Module: ChannelProcess; Package body
//## Subsystem: CPChannelProcess
//## Source file: D:\channel\Chlib\version\src\channelprocess.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// ChannelProcess
#include "channelprocess.hpp"
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#include "channel.hpp"
#include "chan_knl.hpp"
#include "rm.hpp"
#include "node_knl.hpp"
#include "bookmark.hpp"

#include "px_block.hpp"
#include "blockref.hpp"

#include "emd.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"

//## end module.additionalDeclarations


// Class ClChannelProcess 



ClChannelProcess::ClChannelProcess()
  //## begin ClChannelProcess::ClChannelProcess%.hasinit preserve=no
  //## end ClChannelProcess::ClChannelProcess%.hasinit
  //## begin ClChannelProcess::ClChannelProcess%.initialization preserve=yes
  : m_pChannelProcessNext(0)
  //## end ClChannelProcess::ClChannelProcess%.initialization
{
  //## begin ClChannelProcess::ClChannelProcess%.body preserve=yes
  //## end ClChannelProcess::ClChannelProcess%.body
}


ClChannelProcess::~ClChannelProcess()
{
  //## begin ClChannelProcess::~ClChannelProcess%.body preserve=yes
  //## end ClChannelProcess::~ClChannelProcess%.body
}



//## Other Operations (implementation)
m4return_t ClChannelProcess::Pre_Channel_Process (ClChannel *  ai_pClChannel)
{
  //## begin ClChannelProcess::Pre_Channel_Process%913915125.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_Channel_Process%913915125.body
}

m4return_t ClChannelProcess::Post_Channel_Process (ClChannel *  ai_pClChannel)
{
  //## begin ClChannelProcess::Post_Channel_Process%913915126.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_Channel_Process%913915126.body
}

m4return_t ClChannelProcess::Pre_ChannelData_Process (ClChannel_Data *  ai_pClChannel_Data)
{
  //## begin ClChannelProcess::Pre_ChannelData_Process%913915127.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_ChannelData_Process%913915127.body
}

m4return_t ClChannelProcess::Post_ChannelData_Process (ClChannel_Data *  ai_pClChannelData)
{
  //## begin ClChannelProcess::Post_ChannelData_Process%913915128.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_ChannelData_Process%913915128.body
}

m4return_t ClChannelProcess::Pre_NodeData_Process (ClNode_Data *  ai_pNodeData)
{
  //## begin ClChannelProcess::Pre_NodeData_Process%913915129.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_NodeData_Process%913915129.body
}

m4return_t ClChannelProcess::Post_NodeData_Process (ClNode_Data *  ai_pClNode_Data)
{
  //## begin ClChannelProcess::Post_NodeData_Process%913915130.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_NodeData_Process%913915130.body
}

m4return_t ClChannelProcess::Pre_VRBStaticPart_Process (ClVectorRecordBase *  ai_pClVectorRecordBase)
{
  //## begin ClChannelProcess::Pre_VRBStaticPart_Process%913915131.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_VRBStaticPart_Process%913915131.body
}

m4return_t ClChannelProcess::Post_VRBStaticPart_Process (ClVectorRecordBase *  ai_VectorRecordBase)
{
  //## begin ClChannelProcess::Post_VRBStaticPart_Process%913915132.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_VRBStaticPart_Process%913915132.body
}

m4return_t ClChannelProcess::Pre_BlockChannelProxy_Process (ClBlock_Channel_Proxy  *  ai_pBlockChannelProxy)
{
  //## begin ClChannelProcess::Pre_BlockChannelProxy_Process%913915133.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_BlockChannelProxy_Process%913915133.body
}

m4return_t ClChannelProcess::Post_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy)
{
  //## begin ClChannelProcess::Post_BlockChannelProxy_Process%913915134.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_BlockChannelProxy_Process%913915134.body
}

m4return_t ClChannelProcess::Pre_BlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel)
{
  //## begin ClChannelProcess::Pre_BlockChannel_Process%913915135.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_BlockChannel_Process%913915135.body
}

m4return_t ClChannelProcess::Post_BlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel)
{
  //## begin ClChannelProcess::Post_BlockChannel_Process%913915136.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_BlockChannel_Process%913915136.body
}

m4return_t ClChannelProcess::Pre_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase, m4uint8_t *  ai_pRecordAddress, regindex_t ai_uiIndex)
{
  //## begin ClChannelProcess::Pre_VRBRecord_Process%914011920.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_VRBRecord_Process%914011920.body
}

m4return_t ClChannelProcess::Post_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase)
{
  //## begin ClChannelProcess::Post_VRBRecord_Process%914011921.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_VRBRecord_Process%914011921.body
}

m4bool_t ClChannelProcess::ErrorConditionReached ()
{
  //## begin ClChannelProcess::ErrorConditionReached%914151409.body preserve=yes
	return M4_FALSE;
  //## end ClChannelProcess::ErrorConditionReached%914151409.body
}

m4return_t ClChannelProcess::Pre_Record_Process (ClItemRecordManager *  ai_pClItemRecordManager)
{
  //## begin ClChannelProcess::Pre_Record_Process%914151411.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Pre_Record_Process%914151411.body
}

m4return_t ClChannelProcess::Post_Record_Process (ClItemRecordManager *ai_pClItemRecordManager)
{
  //## begin ClChannelProcess::Post_Record_Process%914151412.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Post_Record_Process%914151412.body
}

m4return_t ClChannelProcess::Reset ()
{
  //## begin ClChannelProcess::Reset%915793349.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::Reset%915793349.body
}

// Additional Declarations
  //## begin ClChannelProcess.declarations preserve=yes
  //## end ClChannelProcess.declarations



// Class ClChannelProcess_NullCheck 











ClChannelProcess_NullCheck::ClChannelProcess_NullCheck()
  //## begin ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.hasinit preserve=no
  //## end ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.hasinit
  //## begin ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.initialization preserve=yes
  //## end ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.initialization
{
  //## begin ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.body preserve=yes
  //## end ClChannelProcess_NullCheck::ClChannelProcess_NullCheck%.body

	m_pDataBuffer = NULL ;
	m_oNodeHandle = 0 ;
	m_oItemHandle = 0 ;
	m_uiLastRecIndex = -1 ;
	m_bIsOldValue = M4_FALSE ;
	m_bErrorConditionReached = M4_FALSE ;
	m_poChannel = NULL ;
	m_poAccessBookMark = NULL ;
	m_pMetaChannel = NULL ;
	m_pBlockChannelProxy = NULL ;
}


ClChannelProcess_NullCheck::~ClChannelProcess_NullCheck()
{
  //## begin ClChannelProcess_NullCheck::~ClChannelProcess_NullCheck%.body preserve=yes
  //## end ClChannelProcess_NullCheck::~ClChannelProcess_NullCheck%.body
}



//## Other Operations (implementation)
m4return_t ClChannelProcess_NullCheck::Pre_NodeData_Process (ClNode_Data *  ai_pNodeData)
{
  //## begin ClChannelProcess_NullCheck::Pre_NodeData_Process%913915117.body preserve=yes

	m_pBlockChannelProxy = NULL;

	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Pre_NodeData_Process%913915117.body
}

m4return_t ClChannelProcess_NullCheck::Post_NodeData_Process (ClNode_Data *  ai_pNodeData)
{
  //## begin ClChannelProcess_NullCheck::Post_NodeData_Process%913915118.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Post_NodeData_Process%913915118.body
}

m4return_t ClChannelProcess_NullCheck::Pre_VRBStaticPart_Process (ClVectorRecordBase *  ai_pVectorRecordBase)
{
  //## begin ClChannelProcess_NullCheck::Pre_VRBStaticPart_Process%913915119.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Pre_VRBStaticPart_Process%913915119.body
}

m4return_t ClChannelProcess_NullCheck::Post_VRBStaticPart_Process (ClVectorRecordBase *  ai_pClVectorRecordBase)
{
  //## begin ClChannelProcess_NullCheck::Post_VRBStaticPart_Process%913915120.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Post_VRBStaticPart_Process%913915120.body
}

m4return_t ClChannelProcess_NullCheck::Pre_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy)
{
  //## begin ClChannelProcess_NullCheck::Pre_BlockChannelProxy_Process%913915121.body preserve=yes

	m_pBlockChannelProxy = ai_pBlockChannelProxy;

	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Pre_BlockChannelProxy_Process%913915121.body
}

m4return_t ClChannelProcess_NullCheck::Post_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy)
{
  //## begin ClChannelProcess_NullCheck::Post_BlockChannelProxy_Process%913915122.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Post_BlockChannelProxy_Process%913915122.body
}

m4return_t ClChannelProcess_NullCheck::PreBlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel)
{
  //## begin ClChannelProcess_NullCheck::PreBlockChannel_Process%913915123.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::PreBlockChannel_Process%913915123.body
}

m4return_t ClChannelProcess_NullCheck::PostBlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel)
{
  //## begin ClChannelProcess_NullCheck::PostBlockChannel_Process%913915124.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::PostBlockChannel_Process%913915124.body
}

m4return_t ClChannelProcess_NullCheck::Pre_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase, m4uint8_t *  ai_pRecordAddress, regindex_t ai_uiIndex)
{
  //## begin ClChannelProcess_NullCheck::Pre_VRBRecord_Process%914011922.body preserve=yes

	m_pDataBuffer = ai_pRecordAddress;
	
	m_uiLastRecIndex = ai_uiIndex;

	m_bIsOldValue = M4_FALSE;

	// Tengo que saber el numero de items y el handle del item;

	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Pre_VRBRecord_Process%914011922.body
}

m4return_t ClChannelProcess_NullCheck::Post_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase)
{
  //## begin ClChannelProcess_NullCheck::Post_VRBRecord_Process%914011923.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Post_VRBRecord_Process%914011923.body
}

m4bool_t ClChannelProcess_NullCheck::ErrorConditionReached ()
{
  //## begin ClChannelProcess_NullCheck::ErrorConditionReached%914151410.body preserve=yes
	return m_bErrorConditionReached;
  //## end ClChannelProcess_NullCheck::ErrorConditionReached%914151410.body
}

m4return_t ClChannelProcess_NullCheck::Pre_Record_Process (ClItemRecordManager*  ai_pClItemRecordManager)
{
  //## begin ClChannelProcess_NullCheck::Pre_Record_Process%914151413.body preserve=yes
    if (m_bIsOldValue == M4_FALSE)
	{
        // Antes de intentar hacer nada con el registro tengo que ver si es valido
		/* Bug 0103145
		No se debe aplicar el chequeo a los borrados
        if (ai_pClItemRecordManager->IsValid(m_pDataBuffer) == M4_TRUE)
		*/
		RegisterStatus_t iStatus = ai_pClItemRecordManager->GetStatus( m_pDataBuffer ) ;

		if	(
				iStatus != M4_RS_INVALID
			&&	iStatus != M4_RS_DELETED
			&&	iStatus != M4_RS_MODIFIED_AND_DELETED
			&&	iStatus != M4_RS_NEW_AND_DELETED
			)
        {
            m4uint16_t NItems;
            
            m4uint32_t ItemHandle;
            
            const EMDStaticData* pSMData = ai_pClItemRecordManager->GetpEMDStaticData();
            
            NItems = pSMData->NItems();
            
            //		m_pMetaChannel = m_poChannel-> GetpCMCR();
            
            for( m4uint16_t i=0 ; i < NItems ; i++)
            {
                ItemHandle = pSMData->ItemHandle(i);
                
                if (m_pMetaChannel->GetItemClientNotNull(ItemHandle) == M4_TRUE) 
                {
                    if( ai_pClItemRecordManager->GetItemNull(m_pDataBuffer,i) == M4_TRUE)
                    {
                        ClAccess * poAccess = NULL;
                        
                        m_poChannel->CreateAccess(poAccess);
                        
                        if (poAccess == NULL) return M4_ERROR;
                        
                        if (m_pBlockChannelProxy==NULL)
                        {
                            // estoy en un dato a nivel de nodo, por lo tanto no tengo el 
                            // contexto del proxy como ayuda, ¿ como lo hago ?
                            poAccess->Node [(ClHandle)pSMData->NodeHandle()].RecordSet.Register.Item.MoveTo((ClHandle)ItemHandle);
                            poAccess->Node [(ClHandle)pSMData->NodeHandle()].RecordSet.Current.Item.MoveTo((ClHandle)ItemHandle);
                        }
                        else
                        {
                            PosInNodeWithError (m_pBlockChannelProxy, m_uiLastRecIndex, poAccess);
                            
                            poAccess->Node [(ClHandle)pSMData->NodeHandle()].RecordSet.Register.Item.MoveTo((ClHandle)ItemHandle);
                        }
                        
                        m_poAccessBookMark->GetFrom(*poAccess);
                        
                        m_oNodeHandle=(ClHandle)pSMData->NodeHandle();
                        
                        m_oItemHandle=(ClHandle)ItemHandle;
                        
                        m_bErrorConditionReached = M4_TRUE;
                        
                        poAccess->Destroy();
                        
                        return M4_ERROR;
                    }
                }
            }
        }

	    m_bIsOldValue = M4_TRUE;
	}

	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Pre_Record_Process%914151413.body
}

m4return_t ClChannelProcess_NullCheck::Post_Record_Process (ClItemRecordManager *  ai_pClItemRecordmanager)
{
  //## begin ClChannelProcess_NullCheck::Post_Record_Process%914151414.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Post_Record_Process%914151414.body
}

m4return_t ClChannelProcess_NullCheck::Init (ClChannel *  ai_poChannel, ClAccessBookMark *  ai_poAccessBookMark)
{
  //## begin ClChannelProcess_NullCheck::Init%914428960.body preserve=yes

	m_poChannel = ai_poChannel;

	m_pMetaChannel = ai_poChannel->GetpCMCR();

	m_poAccessBookMark = ai_poAccessBookMark;

	m_bErrorConditionReached = M4_FALSE;

	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Init%914428960.body
}

m4return_t ClChannelProcess_NullCheck::Reset ()
{
  //## begin ClChannelProcess_NullCheck::Reset%915793350.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess_NullCheck::Reset%915793350.body
}

ClHandle ClChannelProcess_NullCheck::GetNodeHandleWithError ()
{
  //## begin ClChannelProcess_NullCheck::GetNodeHandleWithError%916243006.body preserve=yes
	return m_oNodeHandle;
  //## end ClChannelProcess_NullCheck::GetNodeHandleWithError%916243006.body
}

m4return_t ClChannelProcess_NullCheck::PosInNodeWithError (ClBlock_Channel_Proxy *  ai_poBlockChannelProxy, m4uint32_t ai_iIndex, ClAccess *  ao_poAccess)
{
  //## begin ClChannelProcess_NullCheck::PosInNodeWithError%916243007.body preserve=yes
	if (ai_poBlockChannelProxy->ExistParentBlock() == M4_TRUE)
    {
        ClBlockReference oBlockReference;
        ai_poBlockChannelProxy->GetParentBlock(oBlockReference);

		ClBlock_Channel_Proxy* pParentProxy = oBlockReference.GetpProxy();

		if (ai_poBlockChannelProxy->GetpNode_Data()->GetpChannel_Data() == 
			pParentProxy->GetpNode_Data()->GetpChannel_Data())
            PosInNodeWithError(pParentProxy, ai_poBlockChannelProxy->ID.RegisterId, ao_poAccess);
    }

    ao_poAccess->Node[ai_poBlockChannelProxy->GetpNode_Data()->GetpNodeDef()->GetHandle()].RecordSet.Current.MoveTo(ai_iIndex);
    ao_poAccess->Node[ai_poBlockChannelProxy->GetpNode_Data()->GetpNodeDef()->GetHandle()].RecordSet.Register.MoveTo(ai_iIndex);

	return M4_SUCCESS;

  //## end ClChannelProcess_NullCheck::PosInNodeWithError%916243007.body
}

ClHandle ClChannelProcess_NullCheck::GetItemHandle ()
{
  //## begin ClChannelProcess_NullCheck::GetItemHandle%916243008.body preserve=yes
	return m_oItemHandle;
  //## end ClChannelProcess_NullCheck::GetItemHandle%916243008.body
}

ClHandle ClChannelProcess_NullCheck::GetNodeHandle ()
{
  //## begin ClChannelProcess_NullCheck::GetNodeHandle%916243009.body preserve=yes
	return m_oNodeHandle;
  //## end ClChannelProcess_NullCheck::GetNodeHandle%916243009.body
}

ClChannel* ClChannelProcess_NullCheck::GetChannel( void ) const
{
	return( m_poChannel ) ;
}

ClCompiledMCR* ClChannelProcess_NullCheck::GetMetaChannel( void ) const
{
	return( m_pMetaChannel ) ;
}

ClAccessBookMark* ClChannelProcess_NullCheck::GetAccessBookMark( void ) const
{
	return( m_poAccessBookMark ) ;
}



// Additional Declarations
  //## begin ClChannelProcess_NullCheck.declarations preserve=yes
  //## end ClChannelProcess_NullCheck.declarations



//## begin module.epilog preserve=yes


//## end module.epilog
