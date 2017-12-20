//## begin module.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module.cm

//## begin module.cp preserve=no
//## end module.cp

//## Module: ChannelProcess; Package specification
//## Subsystem: CPChannelProcess
//## Source file: D:\channel\Chlib\version\inc\channelprocess.hpp

#ifndef ChnnlPrc_h
#define ChnnlPrc_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "handles.hpp"
#include "dm_types.hpp"

//#include "channel.hpp"
//#include "chan_knl.hpp"
//#include "node_knl.hpp"



//## end module.includes


class ClBlock_Channel_Proxy;
class ClAccessBookMark;
class ClCompiledMCR;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
class ClChannel;
class ClChannel_Data;
class ClNode_Data;
class ClVectorRecordBase;
class ClBlock_Channel_Proxy;
class ClBlock_Channel;
class ClItemRecordManager;
class ClAccess;
class ClExtendedMetaData;
class ClChannel_Kernel;
class ClNode_Kernel;
class ClHandle;
class ClCompiledMCR;
class ClAccessBookMark;
//class ClChannel_Kernel;
#define PROCCESS_MASK_CHECK_NULL      1
#define PROCCESS_MASK_2               2
#define PROCCESS_MASK_3               4
//## end module.additionalDeclarations


//## Class: ClChannelProcess
//## Category: ChannelProcess
//## Subsystem: CPChannelProcess
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClChannelProcess 
{
  //## begin ClChannelProcess.initialDeclarations preserve=yes
  //## end ClChannelProcess.initialDeclarations

  public:
    //## Constructors (generated)
      ClChannelProcess();

    //## Destructor (generated)
      virtual ~ClChannelProcess();


    //## Other Operations (specified)
      //## Operation: Pre_Channel_Process%913915125
      virtual m4return_t Pre_Channel_Process (ClChannel *  ai_pClChannel);

      //## Operation: Post_Channel_Process%913915126
      virtual m4return_t Post_Channel_Process (ClChannel *  ai_pClChannel);

      //## Operation: Pre_ChannelData_Process%913915127
      virtual m4return_t Pre_ChannelData_Process (ClChannel_Data *  ai_pClChannel_Data);

      //## Operation: Post_ChannelData_Process%913915128
      virtual m4return_t Post_ChannelData_Process (ClChannel_Data *  ai_pClChannelData);

      //## Operation: Pre_NodeData_Process%913915129
      virtual m4return_t Pre_NodeData_Process (ClNode_Data *  ai_pNodeData);

      //## Operation: Post_NodeData_Process%913915130
      virtual m4return_t Post_NodeData_Process (ClNode_Data *  ai_pClNode_Data);

      //## Operation: Pre_VRBStaticPart_Process%913915131
      virtual m4return_t Pre_VRBStaticPart_Process (ClVectorRecordBase *  ai_pClVectorRecordBase);

      //## Operation: Post_VRBStaticPart_Process%913915132
      virtual m4return_t Post_VRBStaticPart_Process (ClVectorRecordBase *  ai_VectorRecordBase);

      //## Operation: Pre_BlockChannelProxy_Process%913915133
      virtual m4return_t Pre_BlockChannelProxy_Process (ClBlock_Channel_Proxy  *  ai_pBlockChannelProxy);

      //## Operation: Post_BlockChannelProxy_Process%913915134
      virtual m4return_t Post_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy);

      //## Operation: Pre_BlockChannel_Process%913915135
      virtual m4return_t Pre_BlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel);

      //## Operation: Post_BlockChannel_Process%913915136
      virtual m4return_t Post_BlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel);

      //## Operation: Pre_VRBRecord_Process%914011920
      virtual m4return_t Pre_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase, m4uint8_t *  ai_pRecordAddress, regindex_t ai_uiIndex);

      //## Operation: Post_VRBRecord_Process%914011921
      virtual m4return_t Post_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase);

      //## Operation: ErrorConditionReached%914151409
      virtual m4bool_t ErrorConditionReached ();

      //## Operation: Pre_Record_Process%914151411
      virtual m4return_t Pre_Record_Process (ClItemRecordManager *  ai_pClItemRecordManager);

      //## Operation: Post_Record_Process%914151412
      virtual m4return_t Post_Record_Process (ClItemRecordManager *ai_pClItemRecordManager);

      //## Operation: Reset%915793349
      virtual m4return_t Reset ();

      //## Operation: CallNextProcess%916243010
      m4return_t CallNextProcess ();

    //## Get and Set Operations for Associations (generated)

      //## Association: ChannelProcess::<unnamed>%369CE98A03DA
      //## Role: ClChannelProcess::m_pChannelProcessNext
      const ClChannelProcess * get_m_pChannelProcessNext () const;
      void set_m_pChannelProcessNext (ClChannelProcess * value);

    // Additional Public Declarations
      //## begin ClChannelProcess.public preserve=yes
      //## end ClChannelProcess.public

  protected:
    // Additional Protected Declarations
      //## begin ClChannelProcess.protected preserve=yes
      //## end ClChannelProcess.protected

  private:
    // Additional Private Declarations
      //## begin ClChannelProcess.private preserve=yes
      //## end ClChannelProcess.private

  private:  //## implementation
    // Data Members for Associations

      //## Association: ChannelProcess::<unnamed>%369CE98A03DA
      //## begin ClChannelProcess::m_pChannelProcessNext.role preserve=no  public: ClChannelProcess { -> 1RHN}
      ClChannelProcess *m_pChannelProcessNext;
      //## end ClChannelProcess::m_pChannelProcessNext.role

      //## Association: ChannelProcess::<unnamed>%369CE98A03DA
      //## Role: ClChannelProcess::<the_ClChannelProcess>
      //## begin ClChannelProcess::<the_ClChannelProcess>.role preserve=no  public: ClChannelProcess {1 -> RHG}
      //## end ClChannelProcess::<the_ClChannelProcess>.role

    // Additional Implementation Declarations
      //## begin ClChannelProcess.implementation preserve=yes
      //## end ClChannelProcess.implementation

};

//## begin ClChannelProcess.postscript preserve=yes
//## end ClChannelProcess.postscript



//## Class: ClChannelProcess_NullCheck
//## Category: ChannelProcess
//## Subsystem: CPChannelProcess
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClChannelProcess_NullCheck : public ClChannelProcess  //## Inherits: <unnamed>
{
  //## begin ClChannelProcess_NullCheck.initialDeclarations preserve=yes
  //## end ClChannelProcess_NullCheck.initialDeclarations

  public:
    //## Constructors (generated)
      ClChannelProcess_NullCheck();

    //## Destructor (generated)
      ~ClChannelProcess_NullCheck();


    //## Other Operations (specified)
      //## Operation: Pre_NodeData_Process%913915117
      m4return_t Pre_NodeData_Process (ClNode_Data *  ai_pNodeData);

      //## Operation: Post_NodeData_Process%913915118
      m4return_t Post_NodeData_Process (ClNode_Data *  ai_pNodeData);

      //## Operation: Pre_VRBStaticPart_Process%913915119
      m4return_t Pre_VRBStaticPart_Process (ClVectorRecordBase *  ai_pVectorRecordBase);

      //## Operation: Post_VRBStaticPart_Process%913915120
      m4return_t Post_VRBStaticPart_Process (ClVectorRecordBase *  ai_pClVectorRecordBase);

      //## Operation: Pre_BlockChannelProxy_Process%913915121
      m4return_t Pre_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy);

      //## Operation: Post_BlockChannelProxy_Process%913915122
      m4return_t Post_BlockChannelProxy_Process (ClBlock_Channel_Proxy *  ai_pBlockChannelProxy);

      //## Operation: PreBlockChannel_Process%913915123
      m4return_t PreBlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel);

      //## Operation: PostBlockChannel_Process%913915124
      m4return_t PostBlockChannel_Process (ClBlock_Channel *  ai_pBlockChannel);

      //## Operation: Pre_VRBRecord_Process%914011922
      m4return_t Pre_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase, m4uint8_t *  ai_pRecordAddress, regindex_t ai_uiIndex);

      //## Operation: Post_VRBRecord_Process%914011923
      m4return_t Post_VRBRecord_Process (ClVectorRecordBase *  ai_pClVectorRecordBase);

      //## Operation: ErrorConditionReached%914151410
      m4bool_t ErrorConditionReached ();

      //## Operation: Pre_Record_Process%914151413
      m4return_t Pre_Record_Process (ClItemRecordManager *  ai_pClItemRecordManager);

      //## Operation: Post_Record_Process%914151414
      m4return_t Post_Record_Process (ClItemRecordManager *  ai_pClItemRecordmanager);

      //## Operation: Init%914428960
      m4return_t Init (ClChannel *  ai_poChannel, ClAccessBookMark *  ai_poAccessBookMark);

      //## Operation: Reset%915793350
      virtual m4return_t Reset ();

      //## Operation: GetNodeHandleWithError%916243006
      ClHandle GetNodeHandleWithError ();

      //## Operation: PosInNodeWithError%916243007
      m4return_t PosInNodeWithError (ClBlock_Channel_Proxy *  ai_poBlockChannelProxy, m4uint32_t ai_iIndex, ClAccess *  ao_poAccess);

      //## Operation: GetItemHandle%916243008
      ClHandle GetItemHandle ();

      //## Operation: GetNodeHandle%916243009
      ClHandle GetNodeHandle ();

      ClChannel* GetChannel( void ) const ;
      ClCompiledMCR* GetMetaChannel( void ) const ;
      ClAccessBookMark* GetAccessBookMark( void ) const ;

    // Additional Public Declarations
      //## begin ClChannelProcess_NullCheck.public preserve=yes
      //## end ClChannelProcess_NullCheck.public

  protected:
    // Additional Protected Declarations
      //## begin ClChannelProcess_NullCheck.protected preserve=yes
      //## end ClChannelProcess_NullCheck.protected

  private:
    // Additional Private Declarations
      //## begin ClChannelProcess_NullCheck.private preserve=yes
      //## end ClChannelProcess_NullCheck.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## Attribute: m_pDataBuffer
      //## begin ClChannelProcess_NullCheck::m_pDataBuffer.attr preserve=no  private: m4uint8_t *  {U} 
      m4uint8_t *  m_pDataBuffer;
      //## end ClChannelProcess_NullCheck::m_pDataBuffer.attr

      //## Attribute: m_oNodeHandle
      //## begin ClChannelProcess_NullCheck::m_oNodeHandle.attr preserve=no  private: ClHandle {U} 
      ClHandle m_oNodeHandle;
      //## end ClChannelProcess_NullCheck::m_oNodeHandle.attr

      //## Attribute: m_oItemHandle
      //## begin ClChannelProcess_NullCheck::m_oItemHandle.attr preserve=no  private: ClHandle {U} 
      ClHandle m_oItemHandle;
      //## end ClChannelProcess_NullCheck::m_oItemHandle.attr

      //## Attribute: m_uiLastRecIndex
      //## begin ClChannelProcess_NullCheck::m_uiLastRecIndex.attr preserve=no  private: regindex_t {U} 
      regindex_t m_uiLastRecIndex;
      //## end ClChannelProcess_NullCheck::m_uiLastRecIndex.attr

      //## Attribute: m_bIsOldValue
      //## begin ClChannelProcess_NullCheck::m_bIsOldValue.attr preserve=no  private: m4bool_t {U} 
      m4bool_t m_bIsOldValue;
      //## end ClChannelProcess_NullCheck::m_bIsOldValue.attr

      //## Attribute: m_bErrorConditionReached
      //## begin ClChannelProcess_NullCheck::m_bErrorConditionReached.attr preserve=no  private: m4bool_t {U} 
      m4bool_t m_bErrorConditionReached;
      //## end ClChannelProcess_NullCheck::m_bErrorConditionReached.attr

      //## Attribute: m_poChannel
      //## begin ClChannelProcess_NullCheck::m_poChannel.attr preserve=no  private: ClChannel *  {U} 
      ClChannel *  m_poChannel;
      //## end ClChannelProcess_NullCheck::m_poChannel.attr

    // Data Members for Associations

      //## Association: ChannelProcess::<unnamed>%369DD84401CF
      //## Role: ClChannelProcess_NullCheck::m_poAccessBookMark
      //## begin ClChannelProcess_NullCheck::m_poAccessBookMark.role preserve=no  private: ClAccessBookMark { -> RFHN}
      ClAccessBookMark *m_poAccessBookMark;
      //## end ClChannelProcess_NullCheck::m_poAccessBookMark.role

      //## Association: ChannelProcess::<unnamed>%369DD8B300A2
      //## Role: ClChannelProcess_NullCheck::m_pMetaChannel
      //## begin ClChannelProcess_NullCheck::m_pMetaChannel.role preserve=no  public: ClCompiledMCR { -> RFHN}
      ClCompiledMCR *m_pMetaChannel;
      //## end ClChannelProcess_NullCheck::m_pMetaChannel.role

      //## Association: ChannelProcess::<unnamed>%369DDA60027F
      //## Role: ClChannelProcess_NullCheck::m_pBlockChannelProxy
      //## begin ClChannelProcess_NullCheck::m_pBlockChannelProxy.role preserve=no  private: ClBlock_Channel_Proxy { -> RFHN}
      ClBlock_Channel_Proxy *m_pBlockChannelProxy;
      //## end ClChannelProcess_NullCheck::m_pBlockChannelProxy.role

    // Additional Implementation Declarations
      //## begin ClChannelProcess_NullCheck.implementation preserve=yes
      //## end ClChannelProcess_NullCheck.implementation

};

//## begin ClChannelProcess_NullCheck.postscript preserve=yes
//## end ClChannelProcess_NullCheck.postscript



// Class ClChannelProcess 


//## Other Operations (inline)
inline m4return_t ClChannelProcess::CallNextProcess ()
{
  //## begin ClChannelProcess::CallNextProcess%916243010.body preserve=yes
	return M4_SUCCESS;
  //## end ClChannelProcess::CallNextProcess%916243010.body
}

//## Get and Set Operations for Associations (inline)

inline const ClChannelProcess * ClChannelProcess::get_m_pChannelProcessNext () const
{
  //## begin ClChannelProcess::get_m_pChannelProcessNext%.get preserve=no
  return m_pChannelProcessNext;
  //## end ClChannelProcess::get_m_pChannelProcessNext%.get
}

inline void ClChannelProcess::set_m_pChannelProcessNext (ClChannelProcess * value)
{
  //## begin ClChannelProcess::set_m_pChannelProcessNext%.set preserve=no
  m_pChannelProcessNext = value;
  //## end ClChannelProcess::set_m_pChannelProcessNext%.set
}





// Class ClChannelProcess_NullCheck 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
