

  //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:               VMachin1.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition: 
//
//    Payroll functions node definitions
////
//==============================================================================
#include "prnodef.hpp"

#include "executor.hpp"
#include "vmres.hpp"

#include "chlog.hpp"
#include "m4mdrt.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "node.hpp"
#include "register.hpp"
#include "item.hpp"
#include "handles.hpp"
#include "nodedef.hpp"

m4return_t NodeInfoPR::Prepare(ClAccessRecordSet  *ai_poRS){

    if (ai_poRS==0) return M4_ERROR;

    //RS
    RS=ai_poRS;
    
    m4uint32_t hTemp;
	m4return_t ret=M4_SUCCESS;

    //start date
    hTemp=ai_poRS->GetpNodeDef()->GetStartDateItem();
    if (hTemp==0){
        DUMP_CHLOG_ERRORF(M4_CH_VM_NO_START_DT_ITEM, M4ObjNodeN(ai_poRS->GetpAccess()->GetpCMCR(), ai_poRS->GetpNodeDef()->GetHandle() ) <<"ApplyValue (1)" << ai_poRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
    idxStart=ai_poRS->GetpNodeDef()->GetItemIndex (hTemp);

    hStart=hTemp;

    //end date
    hTemp=ai_poRS->GetpNodeDef()->GetEndDateItem();
    if (hTemp==0){
        DUMP_CHLOG_ERRORF(M4_CH_VM_NO_END_DT_ITEM, M4ObjNodeN(ai_poRS->GetpAccess()->GetpCMCR(), ai_poRS->GetpNodeDef()->GetHandle() ) <<"ApplyValue (1)" << ai_poRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
    idxEnd=ai_poRS->GetpNodeDef()->GetItemIndex (hTemp);

    hEnd=hTemp;

    idxChangeReason = M4DM_EOF_INDEX; //Cada hija debe iniciarlo.
    return ret;
}



m4return_t DmdComponentsInfo::Prepare(ClAccessRecordSet  *ai_poRS){

    RS=ai_poRS;
    idxDmdComponent =ai_poRS->GetpNodeDef()->GetDmdComponentItemIndex();
    m4uint32_t hItem;
    hItem=ai_poRS->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_SLICE_BEHAVIOUR_ITEM);
    if (hItem==0)
		idxSliceBehaviour = M4DM_EOF_INDEX;
	else 
		idxSliceBehaviour=ai_poRS->GetpNodeDef()->GetItemIndex (hItem);
	
	m4return_t ret=M4_SUCCESS;

    if (idxDmdComponent == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poRS->GetpAccess()->GetpCMCR(), ai_poRS->GetpNodeDef()->GetHandle() ) <<"ID_DMD_COMPONENT" << ai_poRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
    if (idxSliceBehaviour == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poRS->GetpAccess()->GetpCMCR(), ai_poRS->GetpNodeDef()->GetHandle() ) <<M4CL_VM_PR_SLICE_BEHAVIOUR_ITEM << ai_poRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
        
    return ret;

}



m4return_t ValuesInfoPR::Prepare  (ClAccessRecordSet  *ai_poValues){
    m4return_t ret=NodeInfoPR::Prepare (ai_poValues);

    idxDmdComponent =ai_poValues->GetpNodeDef()->GetDmdComponentItemIndex();
    idxChangeReason =ai_poValues->GetpNodeDef()->GetChangeReasonItemIndex();
    
    if (idxDmdComponent == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poValues->GetpAccess()->GetpCMCR(), ai_poValues->GetpNodeDef()->GetHandle() ) <<"ID_DMD_COMPONENT" << ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );
        return M4_ERROR;
    }

    return ret;

}

m4return_t OrgInfoPR::Prepare  (ClAccessRecordSet  *ai_poOrg){

    m4return_t ret=NodeInfoPR::Prepare (ai_poOrg);

    m4uint32_t hItem;

    hItem        =ai_poOrg->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_FATHER_UNIT_ITEM);
    if (hItem==0) 
		idxFatherValue = M4DM_EOF_INDEX;
	else
		idxFatherValue = ai_poOrg->GetpNodeDef()->GetItemIndex (hItem);
	
    hItem        =ai_poOrg->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_FATHER_REGISTER_ITEM);
	if (hItem==0) 
		idxFatherRegister = M4DM_EOF_INDEX;
	else
		idxFatherRegister=ai_poOrg->GetpNodeDef()->GetItemIndex (hItem);
    
    hItem        =ai_poOrg->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_CHILD_UNIT_ITEM);
    if (hItem==0) 
		idxSonValue = M4DM_EOF_INDEX;
	else
		idxSonValue      =ai_poOrg->GetpNodeDef()->GetItemIndex (hItem);


    idxChangeReason  =ai_poOrg->GetpNodeDef()->GetChangeReasonItemIndex();

    if (idxFatherValue == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poOrg->GetpAccess()->GetpCMCR(), ai_poOrg->GetpNodeDef()->GetHandle() ) <<M4CL_VM_PR_FATHER_UNIT_ITEM << ai_poOrg->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
    if (idxFatherRegister == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poOrg->GetpAccess()->GetpCMCR(), ai_poOrg->GetpNodeDef()->GetHandle() )<<M4CL_VM_PR_FATHER_REGISTER_ITEM << ai_poOrg->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }
    if (idxSonValue == M4DM_EOF_INDEX) {
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poOrg->GetpAccess()->GetpCMCR(), ai_poOrg->GetpNodeDef()->GetHandle() )<<M4CL_VM_PR_CHILD_UNIT_ITEM << ai_poOrg->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret= M4_ERROR;
    }

	/* Bug 0066759
	Si el organigrama no está cargado lo cargamos a mano para que no se haga por autoload
	*/
	if( ai_poOrg != NULL && ai_poOrg->GetCurrentBlock() != NULL && ai_poOrg->GetCurrentBlock()->IsLoaded() == M4_FALSE )
	{
		ai_poOrg->Load_Block() ;
	}

    return ret;

}


m4return_t EmployeeInfoPR::Prepare (ClAccessRecordSet *ai_poEmployeeRS){
        
        m4return_t ret=NodeInfoPR::Prepare ( ai_poEmployeeRS);

        idxDmdComponent =ai_poEmployeeRS->GetpNodeDef()->GetDmdComponentItemIndex();
        idxChangeReason =ai_poEmployeeRS->GetpNodeDef()->GetChangeReasonItemIndex();
        idxStartFilter  =ai_poEmployeeRS->GetpNodeDef()->GetFilterStartDateItemIndex();
        idxEndFilter    =ai_poEmployeeRS->GetpNodeDef()->GetFilterEndDateItemIndex();

        m4uint32_t hItem;

        hItem       =ai_poEmployeeRS->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_SLICE_MODE_ITEM);
        if (hItem==0) 
			idxSliceMode = M4DM_EOF_INDEX;
		else
			idxSliceMode    =ai_poEmployeeRS->GetpNodeDef()->GetItemIndex (hItem);
        
        hItem       =ai_poEmployeeRS->GetpNodeDef()->GetNodeItemHandle (M4CL_VM_PR_RUN_DATE_ITEM);
		if (hItem==0) 
			idxRunDate = M4DM_EOF_INDEX;
		else
			idxRunDate      =ai_poEmployeeRS->GetpNodeDef()->GetItemIndex (hItem);

		hItem		=ai_poEmployeeRS->GetpNodeDef()->GetNodeItemHandle ("SOURCE_NODE");
        if (hItem==0) 
			idxSrcTable = M4DM_EOF_INDEX;
		else
			idxSrcTable     =ai_poEmployeeRS->GetpNodeDef()->GetItemIndex (hItem);
        
        if (idxDmdComponent == M4DM_EOF_INDEX) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poEmployeeRS->GetpAccess()->GetpCMCR(), ai_poEmployeeRS->GetpNodeDef()->GetHandle() ) << "ID_DMD_COMPONENT" << ai_poEmployeeRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ret=M4_ERROR;
        }
        if (idxStartFilter == M4DM_EOF_INDEX) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poEmployeeRS->GetpAccess()->GetpCMCR(), ai_poEmployeeRS->GetpNodeDef()->GetHandle() ) << "START_DT_FILTER" << ai_poEmployeeRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ret= M4_ERROR;
        }
        if (idxEndFilter == M4DM_EOF_INDEX) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poEmployeeRS->GetpAccess()->GetpCMCR(), ai_poEmployeeRS->GetpNodeDef()->GetHandle() ) << "END_DT_FILTER" << ai_poEmployeeRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ret= M4_ERROR;
        }
        if (idxRunDate == M4DM_EOF_INDEX) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poEmployeeRS->GetpAccess()->GetpCMCR(), ai_poEmployeeRS->GetpNodeDef()->GetHandle() )<< M4CL_VM_PR_RUN_DATE_ITEM << ai_poEmployeeRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ret= M4_ERROR;
        }
        if (idxSliceMode == M4DM_EOF_INDEX) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PREP_NODE_ITEM, M4ObjNodeN(ai_poEmployeeRS->GetpAccess()->GetpCMCR(), ai_poEmployeeRS->GetpNodeDef()->GetHandle() )<< M4CL_VM_PR_SLICE_MODE_ITEM << ai_poEmployeeRS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ret= M4_ERROR;
        }

        return ret;

}

m4return_t HistoricInfoPR::Prepare (ClAccessRecordSet *ai_poHistoricRS){
       
        m4return_t ret=NodeInfoPR::Prepare ( ai_poHistoricRS);
        idxChangeReason =ai_poHistoricRS->GetpNodeDef()->GetChangeReasonItemIndex();
        
        
        return ret;

}
