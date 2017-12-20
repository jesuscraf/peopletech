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


#ifndef _ClVMPRNODEDEF_H
#define _ClVMPRNODEDEF_H


#include "cldefine.hpp"
#include "prdefine.hpp"
#include "prnodef.hpp"
#include "index.hpp"
#include "handles.hpp"

class ClAccessRecordSet;
class ClCompiledMCR;
class ClVMachin1;
class ClAccess;
class ClAccessRecordSet;


//structuras auxiliares


class  NodeInfoPR{

public:
    ClAccessRecordSet     *RS;

    ClItemIndex     idxStart;
    ClHandle        hStart;

    ClItemIndex     idxEnd;
    ClHandle        hEnd;
    ClItemIndex     idxChangeReason;

    virtual m4return_t Prepare  (ClAccessRecordSet  *ai_poRS);
    NodeInfoPR(){
		RS=0;
	}
	
	virtual void Clean(void){
		RS=0;
		hStart=0;
		hEnd=0;
		idxEnd = M4DM_EOF_INDEX;
		idxStart = M4DM_EOF_INDEX;
		idxChangeReason = M4DM_EOF_INDEX;
	
	}

};



class  DmdComponentsInfo{
public:
    ClAccessRecordSet     *RS;
    ClItemIndex     idxDmdComponent;
    ClItemIndex     idxSliceBehaviour;
   
    m4return_t Prepare(ClAccessRecordSet  *ai_poRS);
	
	virtual void Clean(void){
		RS=0;
		idxDmdComponent = M4DM_EOF_INDEX;
		idxSliceBehaviour = M4DM_EOF_INDEX;
	}
};

class OrgInfoPR:public NodeInfoPR{
    
    public:
    ClItemIndex     idxFatherValue;
    ClItemIndex     idxSonValue;
    ClItemIndex     idxFatherRegister;

    m4return_t Prepare  (ClAccessRecordSet  *ai_poHost);
	void Clean(void){
		idxFatherValue = M4DM_EOF_INDEX;
		idxSonValue = M4DM_EOF_INDEX;
		idxFatherRegister = M4DM_EOF_INDEX;
	}
};

class ValuesInfoPR:public NodeInfoPR{
    public:
    //ClItemIndex     idxValue;
    ClItemIndex     idxDmdComponent;
    
    m4return_t Prepare  (ClAccessRecordSet  *ai_poValues);
	void Clean(void){
		idxDmdComponent = M4DM_EOF_INDEX;
	}

};

class EmployeeInfoPR:public NodeInfoPR{
    public:
    ClItemIndex     idxDmdComponent;
    ClItemIndex     idxStartFilter;
    ClItemIndex     idxEndFilter;
    ClItemIndex     idxRunDate;
    ClItemIndex     idxSliceMode; 

	ClItemIndex     idxSrcTable; //para poder copiar en el intermedio la tabla de valores de la que viene el registro
    
    m4return_t      Prepare(ClAccessRecordSet *ai_poEmployeeRS);

	void Clean(void){
		idxDmdComponent = M4DM_EOF_INDEX;
		idxStartFilter = M4DM_EOF_INDEX;
		idxEndFilter = M4DM_EOF_INDEX;
		idxRunDate = M4DM_EOF_INDEX;
		idxSliceMode = M4DM_EOF_INDEX;
		idxSrcTable = M4DM_EOF_INDEX;
	}

};


class HistoricInfoPR:public NodeInfoPR{
    public:
    m4return_t      Prepare(ClAccessRecordSet *ai_poHistoricRS);
};




#endif
