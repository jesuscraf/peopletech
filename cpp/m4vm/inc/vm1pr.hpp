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
//    Payroll functions
////
//==============================================================================


#ifndef _ClVMPAYROLL_H
#define _ClVMPAYROLL_H


#include "vmprapplier.hpp"

#include "prnodef.hpp"
#include "prdefine.hpp"

#include "cldefine.hpp"
#include "m4var.hpp"
#include "access.hpp"
#include "channel.hpp"

class ClAccessRecordSet;
class ClCompiledMCR;
class ClVMachin1;
class ClDateStack;



//structuras auxiliares

struct  stPkMapping{

    ClItemIndex     idxField;
    ClItemIndex     idxRelationField;

};



class ClVMPayRoll{
public:

    m4return_t _CreateOrganigram (ClAccessRecordSet *ai_poRecordSet);

    ClVMPayRoll (void){
        	m_poDatesStack   =0;
            m_poAccess      =0;
            m_dStart        =0;
            m_dEnd          =0;
            m_dRun          =0;
            m_bSlice        =M4_FALSE;
            m_dTimeUnit     =0;
			m_poApplier		=0;
    }

    //EDU:
    m4uint32_t GetSize (void) const;

    void SetAccess (ClAccess *ai_poAccess){
        m_poAccess=ai_poAccess;
        if (!m_dTimeUnit){ //establecemos la unit
            m4VariantType oTemp;
            ClChannel *pChannel=ai_poAccess->GetpChannel();
            
            pChannel->UnitDate.Get(oTemp);
            m_dTimeUnit=oTemp.Data.DoubleData;

        }
    }

    
    void SetDatesStack (ClDateStack *ai_poDatesStack){
        m_poDatesStack=ai_poDatesStack;
    }

    void SetWorkingDates (m4date_t ai_StartDate,m4date_t ai_EndDate,m4date_t ai_RunDate){
        if (!m_dStart){
            m_dStart=ai_StartDate;
            m_dEnd=ai_EndDate;
            m_dRun=ai_RunDate;
        }
    }


    m4return_t SetEmployee (ClAccessRecordSet *ai_poEmployeeRS){
        return m_EmployeeInfo.Prepare(ai_poEmployeeRS);
    }


    m4return_t ApplyTable (ClAccessRecordSet *ai_poValues, ClAccessRecordSet  *ai_poHistorical, 
                       ClAccessRecordSet  *ai_poOrganigram, ClItemIndex *ai_PkHistoric, ClItemIndex *ai_PkValues,
                       m4uint32_t  iNumPkMapping,m4bool_t ai_bApplyAlways);

    m4return_t InitApplyValue (ClAccessRecordSet *ai_poRecordSet, m4char_t *ai_pszDMD ,m4char_t *ai_pszGroup );
    m4return_t SetPeriods     (m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun, m4bool_t ai_bSliceMode);
    m4return_t SetDates       (m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun);

private:

    m4return_t _ApplyTableMode (void);
    m4return_t _ApplyOrgMode (void);
    
    m4return_t _ApplyOnlyTableSlices(void);
    m4return_t _ApplyTableTargetSlices(void); 
    m4return_t _ApplyOnlyTableNoSlices(void);
    m4return_t _ApplyTableTargetNoSlices(void);
    m4return_t _ApplyOrgSlices(void);
    m4return_t _ApplyOrgNoSlices(void);

	m4return_t _CreateOrganigramSlices   (ClAccessRecordSet *ai_poRecordSet);
    m4return_t _CreateOrganigramNoSlices (ClAccessRecordSet *ai_poRecordSet);


    m4return_t _FillPkBlockHistoricItem (void);
    m4return_t _FillPkBlockHistoricItemNoSlices (void);

    m4return_t _ApplyToOneSliceOrg              (ClRegisterIndex ai_iRegOrg, m4date_t ai_dStart, m4date_t ai_dEnd);
    m4return_t _ApplyToOneEmployeeOrg           (ClRegisterIndex ai_iRegOrg);
   
	//----Las siguientes han pasado al applier***
	//m4return_t _CopyFromValToEmpSlice (m4uint8_t ai_iSliceBehaviour, m4date_t ai_dStartToApply, m4date_t ai_dEndToApply, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR);
    //m4return_t _CopyFromValToEmp      (m4uint8_t ai_iSliceBehaviour,m4date_t ai_dStartToApply, m4date_t ai_dEndToApply);
    //m4bool_t   DmdMustApply (m4VariantType &ai_DmdComponent, m4uint8_t &ao_iSliceBehaviour);
    
	m4return_t _AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR );
    m4return_t _AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd );

    
    //prepares
    m4return_t _PrepareHistorical  (ClAccessRecordSet  *ai_poHistorical);
    m4return_t _PreparePKMapping   (ClItemIndex *ai_aiPkHost, ClItemIndex *ai_aiPkHistoric,m4uint32_t  iNumPkMapping);
    m4return_t _PreparePeriods(void);
    m4return_t _PrepareDMDComponents(void);

    m4return_t _DatesToMembers (void);
    m4bool_t	IsEmployeeSetted(ClAccessRecordSet *ai_poRecordSet);

    //m4return_t _ApplyToEmpoyeeWithSlices (m4date_t ai_dStart, m4date_t ai_dEnd);



    //Recordset varios

    //información de la VM
    ClDateStack			*m_poDatesStack;
    ClAccess            *m_poAccess;
    m4date_t            m_dStart;
    m4date_t            m_dEnd;
    m4date_t            m_dRun;
    m4date_t            m_dTimeUnit;
    m4bool_t            m_bSlice;

    HistoricInfoPR    m_HistoricInfo;
    OrgInfoPR         m_OrgInfo;
    ValuesInfoPR      m_ValuesInfo;
    NodeInfoPR        m_PeriodsInfo;
    DmdComponentsInfo m_DmdComponentsInfo;
    EmployeeInfoPR    m_EmployeeInfo;

    stPkMapping         m_PkMapping[M4CL_VM_PR_MAX_PK_MAP];
    m4uint32_t          m_iNumMapping;

	ClVMPayRollApplier				*m_poApplier;
	ClVMPayRollApplierNormal		m_ApplierNormal;
	ClVMPayRollApplierApplyAlways	m_ApplierAlways;

};






#endif

