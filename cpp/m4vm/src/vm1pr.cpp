//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                vMachin1.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    funciones de nómina
////
//==============================================================================
#include "vm1pr.hpp"

#include "executor.hpp"
#include "vmcycle.hpp"
#include "vmdefine.hpp"
#include "vmtypes.hpp"
#include "vmres.hpp" 

#include "m4var.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "cldefine.hpp"
#include "cldbgutil.hpp"
#include "nodedef.hpp"

//------------------------------------------------------------
//FUNCIONES DE INICIALIZACION- publicas
//------------------------------------------------------------

m4return_t ClVMPayRoll::InitApplyValue (ClAccessRecordSet *ai_poRecordSet, m4char_t *ai_pszDMD ,m4char_t *ai_pszGroup ){

    m4return_t ret;
    m4VariantType vGroup, vDMD;

    vGroup.Data.PointerChar=ai_pszGroup;
    vDMD.Data.PointerChar  =ai_pszDMD;

    vGroup.Type=vDMD.Type=M4CL_CPP_TYPE_STRING_VAR;
    
    ret=_PrepareDMDComponents();
    M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "InitApplyValue" );
    
    
    //borramos todos los registros que tuviese
    //forzamos que la próxima vez que se acceda se dispare la autoload
    m_DmdComponentsInfo.RS->Destroy();
    
     //rellenamos el item M4CL_VM_PR_ID_DMD_ITEM
    ret=m_DmdComponentsInfo.RS->Register.Item [M4CL_VM_PR_ID_DMD_ITEM].Value.Set(vDMD);
    M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4CL_VM_PR_ID_DMD_ITEM<<M4ObjNodeN(m_DmdComponentsInfo.RS->GetpAccess()->GetpCMCR() , m_DmdComponentsInfo.RS->GetpNodeDef()->GetHandle() )<<"InitApplyValue" << m_DmdComponentsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    if (ret==M4_WARNING){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_DmdComponentsInfo.RS->GetpAccess()->GetpCMCR(),m_DmdComponentsInfo.RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_DmdComponentsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret=M4_SUCCESS;
    }


    //rellenamos el item M4CL_VM_PR_GROUP_ITEM
    ret=m_DmdComponentsInfo.RS->Register.Item [M4CL_VM_PR_GROUP_ITEM].Value.Set(vGroup);
    M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4CL_VM_PR_GROUP_ITEM<<M4ObjNodeN(m_DmdComponentsInfo.RS->GetpAccess()->GetpCMCR() , m_DmdComponentsInfo.RS->GetpNodeDef()->GetHandle() )<<"InitApplyValue" << m_DmdComponentsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    if (ret==M4_WARNING){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_DmdComponentsInfo.RS->GetpAccess()->GetpCMCR(),m_DmdComponentsInfo.RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_DmdComponentsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ret=M4_SUCCESS;
    }

    return ret;
}


m4return_t ClVMPayRoll::SetPeriods     (m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun, m4bool_t ai_bSliceMode){

    m4VariantType avParam[6],vTemp;
    m4int32_t iRegFind;
    m4return_t ret;

    //preparamos periods
    ret=_PreparePeriods ();
    M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "SetPeriods" );
    


    avParam[0].Type=avParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avParam[2].Type=avParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avParam[1].Type=avParam[4].Type=M4CL_CPP_TYPE_DATE;
    
    avParam[0].Data.DoubleData=m_PeriodsInfo.idxStart;
    avParam[1].Data.DoubleData=ai_dStart;
    avParam[3].Data.DoubleData=m_PeriodsInfo.idxEnd;
    avParam[4].Data.DoubleData=ai_dEnd;
    avParam[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;
    avParam[5].Data.DoubleData=M4CL_EQUAL_FUNCTION;

    //---desconectamos la autoload

    //ahora sólo deshabilitamos la autoload en el Register:
    m_PeriodsInfo.RS->Register.Filter.SetAutoLoadMode (M4CL_AUTOLOAD_OFF);

    ret=m_PeriodsInfo.RS->Register.FindSec(avParam, 6, &iRegFind);
    if (ret!=M4_SUCCESS){
        DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, M4ObjNodeN(m_PeriodsInfo.RS->GetpAccess()->GetpCMCR(), m_PeriodsInfo.RS->GetpNodeDef()->GetHandle() )<<"SetPeriods" << m_PeriodsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        return M4_ERROR;
    }

    //---conectamos la autoload

    //seguimos habilitando la autoload en todo el canal (access)
    m_PeriodsInfo.RS->GetpAccess ()->SetAutoLoadMode (M4CL_AUTOLOAD_BLOCK);

    if (iRegFind==-1){
        vTemp.Type=M4CL_CPP_TYPE_DATE;
        //añadimos y demas
        vTemp.Data.DoubleData=ai_dStart;
        m_PeriodsInfo.RS->Current.Add();
        m_PeriodsInfo.RS->Current.Item[m_PeriodsInfo.idxStart].Value.Set(vTemp);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_PeriodsInfo.RS->GetpAccess()->GetpCMCR() , m_PeriodsInfo.hStart )<<"SetPeriods" << m_PeriodsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        vTemp.Data.DoubleData=ai_dEnd;
        m_PeriodsInfo.RS->Current.Item[m_PeriodsInfo.idxEnd].Value.Set(vTemp);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_PeriodsInfo.RS->GetpAccess()->GetpCMCR() , m_PeriodsInfo.hEnd )<<"SetPeriods" << m_PeriodsInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

    }
    else{
        //nos situamos en ese
        m_PeriodsInfo.RS->Current[iRegFind];
    }

    //ponemos las fechas en el access y el slice mode...
    
    m_dStart=ai_dStart;
    m_dEnd=ai_dEnd;
    m_dRun=ai_dRun;
    m_bSlice=ai_bSliceMode;

    //Guardamso el slicemode y la fecha de ejecución en el nodo intermedio (ya esta iniciado)
    vTemp.Type=M4CL_CPP_TYPE_DATE;
    vTemp.Data.DoubleData=ai_dRun;
    m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxRunDate].Value.Set(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"SetPeriods" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    vTemp.Type=M4CL_CPP_TYPE_NUMBER;
    vTemp.Data.DoubleData=ai_bSliceMode;
    m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxSliceMode].Value.Set(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"SetPeriods" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    

    ///de la VM1
    return M4_SUCCESS;
}


m4return_t ClVMPayRoll::SetDates       (m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun){

    m4return_t ret;
    m4VariantType vTemp;
    ClItemIndex idxOld=m_EmployeeInfo.RS->Register.Item.GetIndex();

    vTemp.Type=M4CL_CPP_TYPE_DATE;


     //rellenamos el item start
    vTemp.Data.DoubleData=ai_dStart;

    ret=m_EmployeeInfo.RS->Register.Item [m_EmployeeInfo.idxStartFilter].Value.Set(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"SetDates" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

    //rellenamos el item end
    vTemp.Data.DoubleData=ai_dEnd;
    ret=m_EmployeeInfo.RS->Register.Item [m_EmployeeInfo.idxEndFilter].Value.Set(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"SetDates" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

 

    //establecemso las fechas
    if (ai_dRun!=0.0){
        m_dRun=ai_dRun;
        vTemp.Type=M4CL_CPP_TYPE_DATE;
        vTemp.Data.DoubleData=ai_dRun;
        m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxRunDate].Value.Set(vTemp);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"SetDates" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    }

    m_dStart= ai_dStart;
    m_dEnd  = ai_dEnd;

    m_EmployeeInfo.RS->Register.Item[idxOld];
    return ret;
}


//--------------------------------------------------------
//APPLY VALUE---- >Punto de entrada principal
//--------------------------------------------------------

m4return_t ClVMPayRoll::ApplyTable (ClAccessRecordSet *ai_poValues, 
                                    ClAccessRecordSet  *ai_poHistorical, 
                                    ClAccessRecordSet  *ai_poOrganigram,
                                    ClItemIndex *ai_PkHistoric,
                                    ClItemIndex *ai_PkValues,
                                    m4uint32_t  iNumPkMapping,
									m4bool_t ai_bApplyAlways){


    //OJO--> Los recordset llegan con los bloques activados

    m4return_t ret=M4_SUCCESS;
    m4VariantType oTemp;

    
    //leemos la unidad y la metemos en el miembro
    //ClChannel *pChannel=ai_poValues->GetpNode_Channel()->GetpChannel();
    ClChannel *pChannel=ai_poValues->GetpAccess ()->GetpChannel();
    
    pChannel->UnitDate.Get(oTemp);
    m_dTimeUnit=oTemp.Data.DoubleData;

    //leemos las fechas
    ret=_DatesToMembers();
	if (ret!=M4_SUCCESS) return ret; //el emnsaje ya sale en la función DatesToMembers
    //M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "ApplyValue. Error reading dates to apply" << ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );

	ret=_PrepareDMDComponents();
    M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyTable" << ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );


    //preparamos las PK's
    ret=_PreparePKMapping (ai_PkHistoric, ai_PkValues, iNumPkMapping);
    //M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_PK_MAPPING, ERRORLOG, "ApplyValue. Error preparing Pk mapping"<<ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );

	//preparamos
	ret=m_ValuesInfo.Prepare(ai_poValues);
	M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyValue" << ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );

	//el m_EmployeeInfo ya esta preapre porque desde fuera se llama al setEmployee que lo prepara
	
	//--------------------ASIGNAMOS EL APPLIER
	if (ai_bApplyAlways){
		m_poApplier=&m_ApplierAlways;
	}
	else{
		m_poApplier=&m_ApplierNormal;
	}
	m_poApplier->SetContext (&m_EmployeeInfo, &m_ValuesInfo, &m_PeriodsInfo, &m_DmdComponentsInfo, m_dTimeUnit);

    if (ai_poOrganigram==0){ //modo de tabla    


            ret=_PrepareHistorical (ai_poHistorical);
            M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyTable" << ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );

            //Mandamos aplicar
            ret=_ApplyTableMode();
            

     
    } 
    else{ //modo de organigrama

		ret=_PrepareHistorical (ai_poHistorical);
		M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyTable" <<ai_poValues->GetpAccess()->GetpExecutor()->DumpCallStack() );
		
		ret=m_OrgInfo.Prepare (ai_poOrganigram);
		M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyTable" <<ai_poOrganigram->GetpAccess()->GetpExecutor()->DumpCallStack() );
		
		//Mandamos aplicar
		ret=_ApplyOrgMode();
		
    }
    
    
	
    return ret;
}


//----------------------------------------------------------------
//APPLIES Funciones principales de aplicación
//----------------------------------------------------------------
m4return_t ClVMPayRoll::_ApplyTableMode (void){
	
    m4return_t ret;
    //tenemos en cuenta si estamos en modo slice o no
    if (m_bSlice ){
        //tenemos en cuenta si viene con target o no
        if (m_HistoricInfo.RS==0) {
            ret= _ApplyOnlyTableSlices();
			//			M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error applying values table only with slices."<< DumpCallStack())	;
        }
        else{
            ret= _ApplyTableTargetSlices();
            if (ret!=M4_SUCCESS){
				return M4_ERROR;
			}
			//            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error applying with historic & slices."<<m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack())	;
            
			ret= _FillPkBlockHistoricItem ();
			//            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error filling intermediate block items with historic's pks items ."<<m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack())	;
        }
    }
    else{
        //tenemos en cuenta si viene con target o no
        if (m_HistoricInfo.RS==0) {
            ret=_ApplyOnlyTableNoSlices();
			//            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error applying values table only ."<<DumpCallStack())	;
        }
        else{
            ret=_ApplyTableTargetNoSlices();
			//            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error applying with historic ."<<m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack())	;
        }
    }
    return ret;
}


m4return_t ClVMPayRoll::_ApplyOrgMode (void){
	
    m4return_t ret;
    //tenemos en cuenta si estamos en modo slice o no
    if (m_bSlice ){
        ret=_ApplyOrgSlices();
		if (ret!=M4_SUCCESS){
			return M4_ERROR;
		}
		//        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyOrgMode. Error applying with slices."<<DumpCallStack())	;
		
		ret= _FillPkBlockHistoricItem ();
		//        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableMode. Error filling intermediate block items with historic's pks items ."<<DumpCallStack())	;
    }
    else{
        ret=_ApplyOrgNoSlices();
		//        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyOrgMode. Error applying without slices."<<DumpCallStack())	;
    }

    return ret;
}


//-------------------------------------------------------------
// A P P L Y    T O  E M P L O Y E E  ONLY TABLE +SLICES ------
//-------------------------------------------------------------

m4return_t  ClVMPayRoll::_ApplyOnlyTableSlices (void){

    //Sol nos dan la tabla de valores. Aplicamos todos
    m4return_t ret=M4_SUCCESS;
    
    m4VariantType vTemp, vTemp2;
    m4VariantType avFindParam[7];
    m4int32_t iRegFind=0;


    m4uint32_t iNumValues=m_ValuesInfo.RS->Register.Count();
    
    if (iNumValues==0) return M4_SUCCESS;

    avFindParam[0].Type=avFindParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_ValuesInfo.idxStart;
    avFindParam[3].Data.DoubleData=m_ValuesInfo.idxEnd;
    
    avFindParam[2].Type=avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[2].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[5].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    
    avFindParam[1].Type=avFindParam[4].Type=M4CL_CPP_TYPE_DATE;
    avFindParam[1].Data.DoubleData=m_dEnd;
    avFindParam[4].Data.DoubleData=m_dStart;

    avFindParam[6].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[6].Data.DoubleData=iRegFind;

    while (iRegFind!=-1){


        ret=m_ValuesInfo.RS->Register.FindSec(avFindParam, 7 , &iRegFind);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"ApplyOnlyTableSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        if (iRegFind!=-1){

            ret=m_ValuesInfo.RS->Current[iRegFind].Item[m_ValuesInfo.idxStart].Value.Get(vTemp);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.StartDate#_ApplyOnlyTableSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            
            ret=m_ValuesInfo.RS->Current.Item [m_ValuesInfo.idxEnd].Value.Get(vTemp2);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.EndDate#_ApplyOnlyTableSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

            //añadimso 
            ret=_AddRegisterToEmployeeWithSlices (iRegFind, max (m_dStart,vTemp.Data.DoubleData), min (m_dEnd, vTemp2.Data.DoubleData) , &m_ValuesInfo , iRegFind);
            //M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyOnlyTableSlices. Error applying to one slice.")	;
			if (ret==M4_ERROR){ //el mensaje ya sale antes
				return M4_ERROR;
			}
        }

        avFindParam[6].Data.DoubleData=iRegFind+1;
    }
    return ret;
}
//-------------------------------------------------------------
// A P P L Y    T O  E M P L O Y E E  ONLY TABLE + NO SLICES --
//-------------------------------------------------------------
///OK!!
m4return_t  ClVMPayRoll::_ApplyOnlyTableNoSlices (void){

    //Solo nos dan la tabla de valores. Aplicamos todos
    m4return_t ret=M4_SUCCESS;
    m4VariantType avFindParam[7];
    m4int32_t iRegFind=0;


    m4uint32_t iNumValues=m_ValuesInfo.RS->Register.Count();
    
    if (iNumValues==0) return M4_SUCCESS;

    avFindParam[0].Type=avFindParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_ValuesInfo.idxStart;
    avFindParam[3].Data.DoubleData=m_ValuesInfo.idxEnd;
    
    avFindParam[2].Type=avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[2].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[5].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    
    avFindParam[1].Type=avFindParam[4].Type=M4CL_CPP_TYPE_DATE;
    avFindParam[1].Data.DoubleData=m_dRun;
    avFindParam[4].Data.DoubleData=m_dRun;

    avFindParam[6].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[6].Data.DoubleData=iRegFind;

    while (iRegFind!=-1){


        ret=m_ValuesInfo.RS->Register.FindSec(avFindParam, 7 , &iRegFind);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyOnlyTableNoSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        
        if (iRegFind!=-1){ //si ha encontrado algún registro en el empleado
            ret=_AddRegisterToEmployeeNoSlices (iRegFind, m_dRun, m_dRun);
			//            M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyOnlyTableNoSlices. Error applying to employee.")	;
			if (ret==M4_ERROR){ //el mensaje ya sale antes
				return M4_ERROR;
			}

           /* if (ret==M4_SUCCESS) { //hemos conseguido aplicar un registro-> no tenemos que seguir intentando porque es sin tramos
                iRegFind=-1;
            }*/ 
            //si el resultado fuera WARNING-> No hemos podidi aplicar porque no esta en aplicables->seguimos intentandolo
            
        }

        avFindParam[6].Data.DoubleData=iRegFind+1;
    }
    return M4_SUCCESS;

}

//-------------------------------------------------------------
// A P P L Y    T O  E M P L O Y E E  VALUES+HISTORIC+SLICES --
//-------------------------------------------------------------

m4return_t  ClVMPayRoll::_ApplyTableTargetSlices (void){

    //para todos los de la tabla de historicos buscamos en la de valore s y aplicamos
    m4return_t ret=M4_SUCCESS, FindRes=M4_SUCCESS ;
    m4uint32_t i,j;
    m4VariantType   avFindParam[M4CL_VM_PR_MAX_PK_MAP*3 +7];
    m4VariantType vTemp2, vTemp;
    m4int32_t iRegFind=0;
    

    m4uint32_t iRegHist= m_HistoricInfo.RS->Current.Count();

    if (iRegHist==0){
		//	DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_HistoricInfo.RS->GetpAccess()->GetpCMCR(),m_HistoricInfo.RS->GetpNodeDef()->GetHandle()) << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            return M4_SUCCESS;
    }
    
    //parametros de busqueda: fechas
    avFindParam[0].Type=avFindParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_ValuesInfo.idxStart;
    avFindParam[3].Data.DoubleData=m_ValuesInfo.idxEnd;
    
    avFindParam[2].Type=avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[2].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[5].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    //parametros de busqueda: MAPEOS DE PK's ******
    for (i=2; i <m_iNumMapping+2; ++i){
        avFindParam[(i*3)+0].Type=M4CL_CPP_TYPE_NUMBER;
        avFindParam[(i*3)+0].Data.DoubleData=m_PkMapping[i-2].idxRelationField;

        avFindParam[(i*3)+2].Type=M4CL_CPP_TYPE_NUMBER;
        avFindParam[(i*3)+2].Data.DoubleData=M4CL_EQUAL_FUNCTION;

        //leemos el valors de field en el histórico dentro del bucle que recorre cada reg. de historico
 
    }

    avFindParam[ (m_iNumMapping+2)*3 ].Type=M4CL_CPP_TYPE_NUMBER;
    for (j=0; j < iRegHist; ++j){

        m_HistoricInfo.RS->Current.MoveTo(j); //nos posicionamos sobre el que trabajaremos

        for (i=2; i <m_iNumMapping+2; ++i){
            ret=m_HistoricInfo.RS->Current.Item[m_PkMapping[i-2].idxField].Value.Get(vTemp);
            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, LOG_CAT_BLOCK("Pk_item["<<i-2<<"]")<<m_HistoricInfo.RS->GetpNodeDef()->Id()<< LogCatAnd << m_HistoricInfo.RS->GetpNodeDef()->Name()<< "" <<"ApplyValue (1)" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            avFindParam[(i*3)+1]=vTemp;
        }
        
        ret=m_HistoricInfo.RS->Current.Item[m_HistoricInfo.idxStart].Value.Get(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_ApplyOnlyTableSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        ret=m_HistoricInfo.RS->Current.Item [m_HistoricInfo.idxEnd].Value.Get(vTemp2);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.EndDate#_ApplyOnlyTableSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

		//si el registro del histórico no queda dentro de fechas->pasamos de el y aplicamos el siguiente
        if (vTemp2.Data.DoubleData <m_dStart || vTemp.Data.DoubleData > m_dEnd ) continue;
        
        
        avFindParam[1]=vTemp2;
        avFindParam[4]=vTemp;

        avFindParam[ (m_iNumMapping+2)*3 ].Data.DoubleData=0;
        iRegFind=0;
        while (iRegFind!=-1){
            FindRes=m_ValuesInfo.RS->Register.FindSec(avFindParam, m4uint16_t( (m_iNumMapping+2)*3 + 1 ), &iRegFind);
	        DEBUG_CHECK_RES (FindRes==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyTableTargetSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            
            if (iRegFind!=-1){ //si ha encontrado algún registro en el empleado
                ret=_AddRegisterToEmployeeWithSlices (iRegFind, max (m_dStart,vTemp.Data.DoubleData), min (m_dEnd, vTemp2.Data.DoubleData), &m_HistoricInfo , m_HistoricInfo.RS->Current.Index );
//                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyTableTargetSlices. Error applying to one slice.")	;
				if (ret==M4_ERROR){ //el mensaje ya sale antes
					return M4_ERROR;
				}

            }
            avFindParam[ (m_iNumMapping+2)*3 ].Data.DoubleData=iRegFind+1;
        }
        
    }
    
    return M4_SUCCESS;
}


//----------------------------------------------------------------
// A P P L Y    T O  E M P L O Y E E  VALUES+HISTORIC+NO SLICES --
//----------------------------------------------------------------

m4return_t  ClVMPayRoll::_ApplyTableTargetNoSlices (void){

    //para todos los de la tabla de historicos buscamos en la de valore s y aplicamos
    m4return_t ret=M4_SUCCESS, FindRes=M4_SUCCESS ;
    //m4uint32_t i,j;
	m4uint32_t i;
    m4VariantType   avFindParam[M4CL_VM_PR_MAX_PK_MAP*3 +7];
    m4VariantType vTemp2, vTemp;
    m4int32_t iRegFind=0;
    

    m4uint32_t iRegHist= m_HistoricInfo.RS->Current.Count();

    if (iRegHist==0){
//		DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_HistoricInfo.RS->GetpAccess()->GetpCMCR(),m_HistoricInfo.RS->GetpNodeDef()->GetHandle()) << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        return M4_SUCCESS;
    }
    
    //parametros de busqueda: fechas
    avFindParam[0].Type=avFindParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_ValuesInfo.idxStart;
    avFindParam[3].Data.DoubleData=m_ValuesInfo.idxEnd;
    
    avFindParam[2].Type=avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[2].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[5].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    //las fechas del registro de históricos deben incluir la de run_date 
    avFindParam[1].Data.DoubleData=m_dRun;
    avFindParam[4].Data.DoubleData=m_dRun;
    avFindParam[1].Type=avFindParam[4].Type=M4CL_CPP_TYPE_DATE;
    
    //parametros de busqueda: MAPEOS DE PK's ******
    for (i=2; i <m_iNumMapping+2; ++i){
        avFindParam[(i*3)+0].Type=M4CL_CPP_TYPE_NUMBER;
        avFindParam[(i*3)+0].Data.DoubleData=m_PkMapping[i-2].idxRelationField;

        avFindParam[(i*3)+2].Type=M4CL_CPP_TYPE_NUMBER;
        avFindParam[(i*3)+2].Data.DoubleData=M4CL_EQUAL_FUNCTION;

        //leemos el valors de field en el histórico dentro del bucle que recorre cada reg. de historico
 
    }
 
    avFindParam[ (m_iNumMapping+2)*3 ].Type=M4CL_CPP_TYPE_NUMBER; 
    //for (j=0; j < iRegHist; ++j){
	m4uint32_t h=0;
	
	do{
		if (h >= iRegHist){ //si no hay registros que aplicar salimos-para evitar ciclo
			//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_HistoricInfo.RS->GetpAccess()->GetpCMCR(),m_HistoricInfo.RS->GetpNodeDef()->GetHandle()) << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
			return M4_SUCCESS;
		}

        //nos movemos a ese registro
        //si las fechas de ese reg del histórico no incluyes a la de run->skip
        ret=m_HistoricInfo.RS->Current[h].Item[m_HistoricInfo.idxStart].Value.Get(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_ApplyTableTargetSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        ret=m_HistoricInfo.RS->Current.Item [m_HistoricInfo.idxEnd].Value.Get(vTemp2);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.EndDate#_ApplyTableTargetSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
		//if (vTemp.Data.DoubleData>m_dRun || vTemp2.Data.DoubleData<m_dRun) continue;
		++h;

	}while (vTemp.Data.DoubleData>m_dRun || vTemp2.Data.DoubleData<m_dRun);


	for (i=2; i <m_iNumMapping+2; ++i){
		ret=m_HistoricInfo.RS->Current.Item[m_PkMapping[i-2].idxField].Value.Get(vTemp);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, LOG_CAT_BLOCK("Pk_item["<<i-2<<"]")<<m_HistoricInfo.RS->GetpNodeDef()->Id()<<LogCatAnd<<m_HistoricInfo.RS->GetpNodeDef()->Name()<<""<<"ApplyValue (1)" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		avFindParam[(i*3)+1]=vTemp;
	}
	
	avFindParam[ (m_iNumMapping+2)*3 ].Data.DoubleData=0;
	iRegFind=0;
	while (iRegFind!=-1){
		FindRes=m_ValuesInfo.RS->Register.FindSec(avFindParam, m4uint16_t( (m_iNumMapping+2)*3 + 1 ), &iRegFind);
        DEBUG_CHECK_RES (FindRes==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyTableTargetNoSlices" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		
		if (iRegFind!=-1){ //si ha encontrado algún registr en el empleado
			ret=_AddRegisterToEmployeeNoSlices (iRegFind, m_dRun, m_dRun);
//			M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyTableTargetNoSlices. Error applying to employee.")	;
			if (ret==M4_ERROR){ //el mensaje ya sale antes
				return M4_ERROR;
			}

			/* if (ret==M4_SUCCESS) { //hemos conseguido aplicar un registro-> no tenemos que seguir intentando porque es sin tramos
			iRegFind=-1;
		}*/
			//si el resultado fuera WARNING-> No hemos podidi aplicar porque no esta en aplicables->seguimos intentandolo
			
			
		}
		avFindParam[ (m_iNumMapping+2)*3 ].Data.DoubleData=iRegFind+1;
	}
		// si ha llegado aquí es porque hemos aplicado algun reg. del histórico
//    }
    
	//llenamos los item de bloque cuyos nombres coincidan con las pk
	ret=_FillPkBlockHistoricItemNoSlices();
//	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableTargetNoSlices. Error filling intermediate block items with historic's pks items ."<<DumpCallStack())	;

    return ret;
}


//-----------------------------------------------------
// A P P L Y    T O  E M P L O Y E E  ORG+SLICES ------
//-----------------------------------------------------

m4return_t  ClVMPayRoll::_ApplyOrgSlices (void){

    //en lso miembros tenemos un recordset con el nodo de histórico
    //y el del organigrama
    //con los correspondientes bloques atachados.
    //Histórico tiene que venir ordenado por fecha_inicio
    
    m4return_t ret=M4_SUCCESS, FindRes=M4_SUCCESS ;
    m4uint32_t i,j;
    

    m4uint32_t      iNumSlices;
    m4date_t         dStart, dEnd;

    struct pairDates{
           m4date_t Start;
           m4date_t End;
		   ClRegisterIndex idxRegister;
    };

    m4VariantType   avFindParam[9];
    m4VariantType vTemp2, vTemp;
    m4VariantType  vFatherValue;
    m4int32_t iRegFind;

   
    //1.- en la pila de fechas metemos las del organigrama y las de HRPUnit y hacemos la partición mínima
    m_poDatesStack->Clear (m_dStart, m_dEnd);
    
    m4uint32_t iRegHist= m_HistoricInfo.RS->Current.Count();
    m4uint32_t iRegOrg = m_OrgInfo.RS->Current.Count();

    if (iRegHist==0) {
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_HistoricInfo.RS->GetpAccess()->GetpCMCR(),m_HistoricInfo.RS->GetpNodeDef()->GetHandle()) << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_SUCCESS;
    }
    if (iRegOrg==0) {
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_OrgInfo.RS->GetpAccess()->GetpCMCR(),m_OrgInfo.RS->GetpNodeDef()->GetHandle()) << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_SUCCESS;
    }


    m4date_t   dTempDate;
    
    pairDates *adHistUnitDates=new pairDates[iRegHist];

    i=0;
    //bGoOn=M4_TRUE;
    dTempDate=0.0;
    while (i<iRegOrg ){ //los des organigrama

        ret=m_OrgInfo.RS->Register[i].Item[m_OrgInfo.idxStart].Value.Get(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.StartDate#_ApplyOrgSlices" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        ret=m_OrgInfo.RS->Register.Item[m_OrgInfo.idxEnd].Value.Get(vTemp2);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.EndDate#_ApplyOrgSlices" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        if (vTemp2.Data.DoubleData < dTempDate) break;
        if (vTemp.Type!=M4CL_CPP_TYPE_NULL ){
            m_poDatesStack->AddElement(vTemp.Data.DoubleData);
        }
        
        if (vTemp2.Type!=M4CL_CPP_TYPE_NULL ){
            m_poDatesStack->AddElement(vTemp2.Data.DoubleData+m_dTimeUnit);
            dTempDate=vTemp.Data.DoubleData;
        }
        ++i;
    }

    m4uint32_t iDiscount=0;
    for (i=0; i < iRegHist; ++i){

        ret=m_HistoricInfo.RS->Register[i].Item[m_HistoricInfo.idxStart].Value.Get(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_ApplyOrgSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        ret=m_HistoricInfo.RS->Register.Item [m_HistoricInfo.idxEnd].Value.Get(vTemp2);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.EndDate#_ApplyOrgSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        //si el registro del historico queda fuera del rango no lo debemos meter y debemis en la estructura
        // y contar como uno menos
        if (vTemp2.Data.DoubleData <m_dStart || vTemp.Data.DoubleData > m_dEnd ){
            ++iDiscount;
        }
        else{
            adHistUnitDates[i-iDiscount].Start = vTemp.Data.DoubleData;
            adHistUnitDates[i-iDiscount].End   = vTemp2.Data.DoubleData;
			adHistUnitDates[i-iDiscount].idxRegister=i;
            
            if (vTemp.Type!=M4CL_CPP_TYPE_NULL  )
                m_poDatesStack->AddElement(vTemp.Data.DoubleData);
            
            if (vTemp2.Type!=M4CL_CPP_TYPE_NULL  )
                m_poDatesStack->AddElement(vTemp2.Data.DoubleData+m_dTimeUnit);
        }
    }
    
    iRegHist-=iDiscount;  //descontamos los que estaban fuera de rango. No debe haber ninguno si los flt son correctos

    m_poDatesStack->Reduce();
    m_poDatesStack->AddEndDate(m_dTimeUnit);
    iNumSlices=m_poDatesStack->GetLength(); //en realidad no es el numero de slices sino el de valores de la pila que podemos leer


 
    //2.- Para cada uno de los registros nos enganchamos para cada tramso que este entre
    //las fechas de ese reg.
    
    //--metemos los parámetros de búsqueda fijos
    avFindParam[0].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[6].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_OrgInfo.idxSonValue;
    avFindParam[3].Data.DoubleData=m_OrgInfo.idxStart;
    avFindParam[6].Data.DoubleData=m_OrgInfo.idxEnd;
    
    avFindParam[4].Type = avFindParam[7].Type=M4CL_CPP_TYPE_DATE;
    
    avFindParam[2].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[8].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;
    avFindParam[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[8].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;
    //--fin parámetros fijos

    for (i=0; i < iRegHist; ++i){

        //nos situamos en el primer tramo de la pila que esté
        //contenido en en adHRPUnitDates[i].Start y End 
        j=0;
        ret=m_poDatesStack->GetElement ( m4uint16_t( j ), dStart);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<<"_ApplyOrgSlices. Error getting date from stack.")	;

        while (dStart <adHistUnitDates[i].Start) {
            ++j;
            ret=m_poDatesStack->GetElement ( m4uint16_t( j ), dStart);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "_ApplyOrgSlices. Error getting date from stack.")	;
            
        }

		++j;
        ret=m_poDatesStack->GetElement ( m4uint16_t( j ), dEnd);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "_ApplyOrgSlices. Error getting date from stack.")	;

        dEnd-=m_dTimeUnit;
        //leemos los valores de Value, moviendonos al registro del Hco. que tiene las fechas a tratar
        ret=m_HistoricInfo.RS->Current[ adHistUnitDates[i].idxRegister ].Item [m_PkMapping[m_iNumMapping-1].idxField].Value.Get(vFatherValue); //solo consideramos una pk->la ultima de la lista, la primera desde ln4
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.father value#_ApplyOrgSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        avFindParam[1]=vFatherValue;

        //para todos los tramos de la pila que estén en nuestro rango
        //aplicamos el tratamiento
        while (dEnd <=adHistUnitDates[i].End && dEnd>=dStart){

            //buscamos en la tabla del organigrama
           
            avFindParam[4].Data.DoubleData=dStart;
            avFindParam[7].Data.DoubleData=dEnd;

             
            FindRes=m_OrgInfo.RS->Register.FindSec (avFindParam, 9, &iRegFind);
	        DEBUG_CHECK_RES (FindRes==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_OrgInfo.RS->GetpAccess()->GetpCMCR(), m_OrgInfo.RS->GetpNodeDef()->GetHandle() )<<"ApplyOrgSlices" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            
            if (iRegFind!=-1){ //si ha encontrado algún registro en el empleado
                ret=_ApplyToOneSliceOrg (iRegFind, dStart, dEnd);
				
				if (ret==M4_ERROR){
//					SETCODE (M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyOrgSlices. Error applying to one slice.")	;
					//el mensaje ya sale antes
					delete[] adHistUnitDates;       
					return M4_ERROR;
				}

            }
            
                 //Fin del tratamiento---------
            //para pasar al siguente tramo
            dStart=dEnd+m_dTimeUnit;
            ++j;
			if ( j < iNumSlices) { //ini cambio
                ret=m_poDatesStack->GetElement( m4uint16_t( j ), dEnd);
                dEnd-=m_dTimeUnit;

#ifdef _DEBUG
                if (ret==M4_ERROR) {
					g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<< __FILE__<<__LINE__<< "_ApplyOrgSlices. Error getting date from stack."<<EndLog;
                }
#endif
            }
            else{
                dEnd=adHistUnitDates[i].End+m_dTimeUnit;
            } //fin cambio
           /* ret=m_poDatesStack->GetElement ( m4uint16_t( j+1 ), dEnd);
            DEBUG_CHECK_RES (ret==M4_SUCCESS,  M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<<"_ApplyOrgSlices. Error getting date from stack.")	;
            
            dEnd-=m_dTimeUnit; */ //CAMBIO

        }

    }

 delete[]adHistUnitDates;       
 return ret;
}



//-------------------------------------------------------
//Apply org no slices
//-------------------------------------------------------

m4return_t  ClVMPayRoll::_ApplyOrgNoSlices (void){

    //en los miembros tenemos un recordset con el nodo de histórico
    //y el del organigrama
    //con los correspondientes bloques atachados.
    //Histórico tiene que venir ordenado por fecha_inicio
    
    m4return_t ret=M4_SUCCESS, FindRes=M4_SUCCESS ;
    m4uint32_t i;
    m4VariantType   avFindParam[9], vTemp, vTemp2;
    m4VariantType  vFatherValue;
    m4int32_t iRegFind;
	
    
    m4uint32_t iRegHist= m_HistoricInfo.RS->Current.Count();
    m4uint32_t iRegOrg = m_OrgInfo.RS->Current.Count();
    
	if (iRegHist==0) {
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_HistoricInfo.RS->GetpAccess()->GetpCMCR(),m_HistoricInfo.RS->GetpNodeDef()->GetHandle()) << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_SUCCESS;
    }
    if (iRegOrg==0) {
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_OrgInfo.RS->GetpAccess()->GetpCMCR(),m_OrgInfo.RS->GetpNodeDef()->GetHandle()) << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_SUCCESS;
    }

    //2.- Para cada uno de los registros nos enganchamos para cada tramso que este entre
    //las fechas de ese reg.
    
    //--metemos los parámetros de búsqueda fijos
    avFindParam[0].Type=avFindParam[2].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[0].Data.DoubleData=m_OrgInfo.idxSonValue;
    avFindParam[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;

    avFindParam[3].Type=avFindParam[6].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[3].Data.DoubleData=m_OrgInfo.idxStart;
    avFindParam[6].Data.DoubleData=m_OrgInfo.idxEnd;
    avFindParam[4].Type=avFindParam[7].Type=M4CL_CPP_TYPE_DATE;
    avFindParam[4].Data.DoubleData=m_dRun;
    avFindParam[7].Data.DoubleData=m_dRun;
    avFindParam[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[8].Type=M4CL_CPP_TYPE_NUMBER;
    avFindParam[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFindParam[8].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    
    //--fin parámetros fijos
    
    for (i=0; i < iRegHist; ++i){
        
        ret=m_HistoricInfo.RS->Current[i].Item[m_HistoricInfo.idxStart].Value.Get(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_ApplyTableTargetSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        ret=m_HistoricInfo.RS->Current.Item [m_HistoricInfo.idxEnd].Value.Get(vTemp2);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.EndDate#_ApplyTableTargetSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		//si las fechas del reg. de histórico no incluyen las de eje->siguiente
		if (vTemp.Data.DoubleData>m_dRun || vTemp2.Data.DoubleData<m_dRun) continue;

        //leemos los valores de Value
        ret=m_HistoricInfo.RS->Current.Item [m_PkMapping[m_iNumMapping-1].idxField].Value.Get(vFatherValue); //solo consideramos una pk->la ultima de la lista, la primera desde ln4
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.ParentValue#_ApplyOrgNoSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        avFindParam[1]=vFatherValue;
        
        FindRes=m_OrgInfo.RS->Register.FindSec (avFindParam, 9,&iRegFind);
        DEBUG_CHECK_RES (FindRes==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_OrgInfo.RS->GetpAccess()->GetpCMCR(), m_OrgInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyOrgNoSlices" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        if (iRegFind!=-1){ //si ha encontrado algún registro en el organigrama
            ret=_ApplyToOneEmployeeOrg (iRegFind);
//            M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_INVALID_CALL, ERRORLOG, "_ApplyOrgNoSlices. Error applying to one employee.")	;
			if (ret==M4_ERROR){ //el mensaje ya sale antes
				return M4_ERROR;
			}

            
            //llenamos los item de bloque cuyos nombres coincidan con las pk
            ret=_FillPkBlockHistoricItemNoSlices();
            if (ret!=M4_SUCCESS){
				return M4_ERROR;
			}
//            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_AV_APPLY_TABLE, ERRORLOG, "_ApplyTableTargetNoSlices. Error filling intermediate block items with historic's pks items ."<<DumpCallStack())	;

            if (ret==M4_SUCCESS) { //hemos conseguido aplicar un registro-> no tenemos que seguir intentando porque es sin tramos
                break; //para no seguir aplicando. Solo aplicamos uno (no debe haber más de uno en las fechas)
            }
            //si el resultado fuera WARNING-> No hemos podidi aplicar porque no esta en aplicables->seguimos intentandolo

        }
    }
    
    return ret;
}



//--------------------------------------------------------------------------------
//APPLY TO ONE SLICE ORG
//--------------------------------------------------------------------------------
m4return_t ClVMPayRoll::_ApplyToOneSliceOrg (ClRegisterIndex ai_iRegOrg, m4date_t ai_dStart, m4date_t ai_dEnd){


    m4return_t ret=M4_SUCCESS;
    m4VariantType avFind[10];
    m4int32_t iStartRegInValues;
    m4int32_t  iRegValueFind;
    m4VariantType vChange, vCRTemp;
    NodeInfoPR *poNodeInfo ;
    ClRegisterIndex idxRegister;

 
    
    m4VariantType   vNextReg, vValue;


    m4uint32_t iRegValues= m_ValuesInfo.RS->Register.Count();

    if (iRegValues==0 ){
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(),m_ValuesInfo.RS->GetpNodeDef()->GetHandle()) << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        return M4_SUCCESS;
    }



    //metemos los argumentos comunes a todas las búsquedas
    
    avFind[0].Type=M4CL_CPP_TYPE_NUMBER;  //********
    avFind[3].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[6].Type=M4CL_CPP_TYPE_NUMBER;
    
    avFind[0].Data.DoubleData=m_PkMapping[m_iNumMapping-1].idxRelationField;
    avFind[3].Data.DoubleData=m_ValuesInfo.idxStart;
    avFind[6].Data.DoubleData=m_ValuesInfo.idxEnd;
    
    avFind[4].Type=avFind[7].Type=M4CL_CPP_TYPE_DATE;
    avFind[4].Data.DoubleData=ai_dEnd;
    avFind[7].Data.DoubleData=ai_dStart;
    
    avFind[2].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[8].Type=M4CL_CPP_TYPE_NUMBER;

    avFind[9].Type=M4CL_CPP_TYPE_NUMBER;
    
    avFind[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;
    
    avFind[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFind[8].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    
    while (ai_iRegOrg!=-1){//mientras el registro iNumreg del nodo organigramas no sea EOF

        //ret=m_OrgInfo.RS->Current[ai_iRegOrg].Item[m_OrgInfo.idxFatherValue].Value.Get(vValue);
        ret=m_OrgInfo.RS->Current[ai_iRegOrg].Item[m_OrgInfo.idxSonValue].Value.Get(vValue);
        
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.SonValue#_ApplyToOneSliceOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );


        avFind[1]=vValue;
        

        
        iStartRegInValues=0;
        while (iStartRegInValues!= -1){
            avFind[9].Data.DoubleData=iStartRegInValues;

            ret=m_ValuesInfo.RS->Register.FindSec (avFind,10, &iRegValueFind);
	        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyToOneSliceOrg" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

            if (iRegValueFind!=-1){

                //si en el organigrama el change reason es null usamos el del historico
                if ( m_OrgInfo.idxChangeReason != M4DM_EOF_INDEX ) {
                    ret=m_OrgInfo.RS->Register[ai_iRegOrg].Item[m_OrgInfo.idxChangeReason].Value.Get(vChange);
                    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.ChangeReason#_ApplyToOneSliceOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                }
                else{
                    vChange.Type=M4CL_CPP_TYPE_NULL;
                }

                ret=m_OrgInfo.RS->Register.Item[m_OrgInfo.idxStart].Value.Get(vCRTemp);
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.StartDate#_ApplyToOneSliceOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

                //si no es nulo y la fecha de inicio no es menor que la que aplicamos
                if (vChange.Type !=M4CL_CPP_TYPE_NULL && vCRTemp.Data.DoubleData>=m_dStart) { //aplicamos el change reason del organigrama
                    poNodeInfo = &m_OrgInfo;
                    idxRegister= ai_iRegOrg;
                }
                else { //usamos el del historico si las fechas corresponden o el de valores
                    ret=m_HistoricInfo.RS->Current.Item[m_HistoricInfo.idxStart].Value.Get (vCRTemp);
                    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_ApplyToOneSliceOrg" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

                    if (vCRTemp.Data.DoubleData < m_dStart) { //usamos values
                        poNodeInfo = &m_ValuesInfo;
                    }
                    else {
                        poNodeInfo = &m_HistoricInfo;
                        idxRegister= m_HistoricInfo.RS->Current.Index;
                    }
                }
                
                ret=_AddRegisterToEmployeeWithSlices ( iRegValueFind,max (m_dStart,  ai_dStart), min(m_dEnd, ai_dEnd) , poNodeInfo , idxRegister);
                //M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_CANT_ADD_RECORD, ERRORLOG, "_ApplyToOneSliceOrg. Error adding register to employee node." << DumpCallStack() );
                //El mensaje ya sale en el _AddRegister...
				if (ret==M4_ERROR) {
					return M4_ERROR;
				}

				iStartRegInValues=iRegValueFind+1;
            }
            else iStartRegInValues=-1;

        }


//nos vamos al registro padre (Fatherregister) del registro ai_iReg del nodo organigram
//y seguimos con el
        ret=m_OrgInfo.RS->Current.Item[m_OrgInfo.idxFatherRegister].Value.Get(vNextReg); //debemos estar posicionados
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.fatherReg#_ApplyToOneSliceOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        if (vNextReg.Type==M4CL_CPP_TYPE_NULL){
            ai_iRegOrg=-1;
        }
        else{
            ai_iRegOrg= (m4uint32_t)vNextReg.Data.DoubleData;
        }
    }

	//si llega aqui, salimos con éxito
    //return ret;
	return M4_SUCCESS;
}

//--------------------------------------------------------------------------------
//APPLY TO ONE EMPLOYEE (no slices) ORG
//--------------------------------------------------------------------------------
m4return_t ClVMPayRoll::_ApplyToOneEmployeeOrg (ClRegisterIndex ai_iRegOrg){


    m4return_t ret=M4_SUCCESS;
    m4VariantType avFind[10];
    m4int32_t iStartRegInValues;
    m4int32_t  iRegValueFind;
    m4VariantType   vNextReg, vValue;

    m4uint32_t iRegValues= m_ValuesInfo.RS->Register.Count();

    if (iRegValues==0 ){
		//DUMP_CHLOG_WARNINGF(M4_CH_VM_AV_NO_RECORDS, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(),m_ValuesInfo.RS->GetpNodeDef()->GetHandle()) << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_SUCCESS;
    }

    //metemos los argumentos comunes a todas las búsquedas
    
    avFind[0].Type=M4CL_CPP_TYPE_NUMBER;  //********
    avFind[0].Data.DoubleData=m_PkMapping[m_iNumMapping-1].idxRelationField;
    avFind[2].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[9].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;
    
    avFind[3].Type=avFind[6].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[3].Data.DoubleData=m_ValuesInfo.idxStart;
    avFind[6].Data.DoubleData=m_ValuesInfo.idxEnd;
    avFind[4].Type=avFind[7].Type=M4CL_CPP_TYPE_DATE;
    avFind[4].Data.DoubleData=m_dRun;
    avFind[7].Data.DoubleData=m_dRun;
    avFind[5].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[8].Type=M4CL_CPP_TYPE_NUMBER;
    avFind[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;
    avFind[8].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

    
    while (ai_iRegOrg!=-1){//mientras el registro iNumreg del nodo organigramas no sea EOF

        ret=m_OrgInfo.RS->Current[ai_iRegOrg].Item[m_OrgInfo.idxSonValue].Value.Get(vValue);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.son value#_ApplyToEmployeeOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

        avFind[1]=vValue;

        iStartRegInValues=0;
        while (iStartRegInValues!= -1){
            avFind[9].Data.DoubleData=iStartRegInValues;

            ret=m_ValuesInfo.RS->Register.FindSec (avFind,10, &iRegValueFind);
	        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_IN_FIND, ERRORLOG, M4ObjNodeN(m_ValuesInfo.RS->GetpAccess()->GetpCMCR(), m_ValuesInfo.RS->GetpNodeDef()->GetHandle() )<<"_ApplyToEmployeeOrg" << m_ValuesInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

            if (iRegValueFind!=-1){
				
				//Sin tramos no hay change reason

                ret=_AddRegisterToEmployeeNoSlices ( iRegValueFind, m_dRun, m_dRun);
                //M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_CANT_ADD_RECORD, ERRORLOG, "_ApplyToEmployeeOrg. Error adding register to employee node." << DumpCallStack() );
                //El mensaje ya sale en el _AddRegister...
				if (ret==M4_ERROR) {
					return M4_ERROR;
				}

                iStartRegInValues=iRegValueFind+1;
            }
            else iStartRegInValues=-1;

        }

        //nos vamos al registro padre (Fatherregister) del registro ai_iReg del nodo organigram
        //y seguimos con el
        ret=m_OrgInfo.RS->Current.Item[m_OrgInfo.idxFatherRegister].Value.Get(vNextReg); //debemos estar posicionados
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.fatherReg#_ApplyToEmployeeOrg" << m_OrgInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        if (vNextReg.Type==M4CL_CPP_TYPE_NULL){
            ai_iRegOrg=-1;
        }
        else{
            ai_iRegOrg= (m4uint32_t)vNextReg.Data.DoubleData;
        }
    }

	//si llega aqui, salimos con éxito
    //return ret;
	return M4_SUCCESS;
}



//-----------------------------------------------------
// ADD REGISTER TO EMPLOYEE
//-----------------------------------------------------

m4return_t ClVMPayRoll::_AddRegisterToEmployeeWithSlices ( m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR){

	return m_poApplier->_AddRegisterToEmployeeWithSlices (iRegValueFind,ai_dStart,ai_dEnd,ai_poNode,ai_idxCR);
}


//versión sin tramos

m4return_t ClVMPayRoll::_AddRegisterToEmployeeNoSlices  ( m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd){

	return m_poApplier->_AddRegisterToEmployeeNoSlices  (iRegValueFind,ai_dStart,ai_dEnd);
}


//--------------------------------------------------------------------------------
//ORGANIGRAMA
//--------------------------------------------------------------------------------


//--------------------------------------------------------
// Create organigram
//genera el organigrama para el aplica valor de nivel 1
//Premisas: El recorset viene con el bloque asignado...
//--------------------------------------------------------
m4return_t ClVMPayRoll::_CreateOrganigram (ClAccessRecordSet *ai_poRecordSet)
{

    M4_ASSERT (m_poDatesStack);
    m4return_t ret;
	
	if (!IsEmployeeSetted(ai_poRecordSet) ){
		//no tenemos hecho un setdates o ya no es válido->le damos un error
		DUMP_CHLOG_ERROR(M4_CH_VM_AV_NO_DATES_SET);
		return M4_ERROR;
	}

	ret=_DatesToMembers();
	if (ret!=M4_SUCCESS) return ret; //el emnsaje ya sale en la función DatesToMembers

    if (ai_poRecordSet->Register.Count() <=0){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_RECORDS, M4ObjNodeN(ai_poRecordSet->GetpAccess()->GetpCMCR(),ai_poRecordSet->GetpNodeDef()->GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        return M4_SUCCESS; //si no hay registros --pasamos
    }
	//preparamos el organigrama
    ret=m_OrgInfo.Prepare (ai_poRecordSet);
    M4CHECK_DEBUGINFO (ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );

	if (m_bSlice)
	{
		return _CreateOrganigramSlices(ai_poRecordSet);
	}
	else
	{
		return 	_CreateOrganigramNoSlices(ai_poRecordSet);
	}
}

//-----------------------
//CreateOrgChart Slices
//-----------------------
m4return_t ClVMPayRoll::_CreateOrganigramSlices (ClAccessRecordSet *ai_poRecordSet)
{
    m4return_t      findRet,ret=M4_SUCCESS;

    m4uint16_t      j;
    m4uint32_t      iNumReg, i;
    m4date_t        dSliceStartDate;
    m4date_t        dStartDate, dEndDate;
    m4VariantType   oTemp;
    m4VariantType	vFatherValue , vStartDate,vEndDate, vTempStart, vTempEnd, vTemp, vChange;
    m4VariantType   pstArguments[9];
    ClRegister      *poRegister;
    m4int32_t       iRegister;

    M4_ASSERT (m_poDatesStack);
    

    //1.-----Primera pasada. Rellenar el array de fechas

    iNumReg=ai_poRecordSet->Register.Count();

    m_poDatesStack->Clear (m_dStart, m_dEnd);
    for (i=0; i<iNumReg; i++){

        ret=ai_poRecordSet->Register[i].Item[m_OrgInfo.idxStart].Value.Get(vTempStart);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.StartDate#CreateOrganigram" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        ret=ai_poRecordSet->Register[i].Item[m_OrgInfo.idxEnd].Value.Get(vTempEnd  );
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.EndDate#CreateOrganigram" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        //No comprobamos los tipos porque ya hemops mirado los tipos de los items

        if (vTempStart.Type!=M4CL_CPP_TYPE_NULL  )
            m_poDatesStack->AddElement(vTempStart.Data.DoubleData);
        if (vTempEnd.Type!=M4CL_CPP_TYPE_NULL  )
            m_poDatesStack->AddElement(vTempEnd.Data.DoubleData+m_dTimeUnit);
        
    }
    m_poDatesStack->Reduce();
    m_poDatesStack->AddEndDate(m_dTimeUnit);
    m4uint32_t iNumSlices=m_poDatesStack->GetLength();

	if (iNumSlices < 2){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_RECORDS_IN_DATES, M4ObjNodeN(ai_poRecordSet->GetpAccess()->GetpCMCR(),ai_poRecordSet->GetpNodeDef()->GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
            return M4_SUCCESS;
	}

    /*
    Para cada registro los duplicamos, cambiando fecha_ini y fecha_fin
    tantas veces como fechas intermedias tengamos en el array de fechas. así
    creamos una copia, con las fechas bien puestas, para cada par de fechas que tengamos para ese tramo
    */

    ClRegister oReg (ai_poRecordSet->Register);

    for (i=0; i< iNumReg; ++i){

		/* jcr
		Para el arreglo del bug 0068188, ahora la partición de los registros
		se hace con un for, saltándose los registros fuera de fechas que
		no hay que procesar. También se hace la partición de la primera fecha, que
		antes no se hacía.
		*/

        ai_poRecordSet->Register[i].Item[m_OrgInfo.idxStart].Value.Get(vTemp);
        dStartDate=vTemp.Data.DoubleData;
        
        ai_poRecordSet->Register[i].Item[m_OrgInfo.idxEnd].Value.Get(vTemp);
        dEndDate=vTemp.Data.DoubleData;

		// Descartamos los registros con fechas no comprendidas en los tramos
		if( dStartDate > m_dEnd || dEndDate < m_dStart )
		{
			continue ;
		}

		// Recorremos los registros de los tramos partiendo el registro

		for( j = 0 ; j < iNumSlices ; j++ )
		{
			ret = m_poDatesStack->GetElement( j, dSliceStartDate ) ;
			DEBUG_CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__ << __LINE__ << "CreateOrganigram. Error getting date from stack." ) ;

			// Si la fecha inicio del tramo es mayor que la fecha fin del registro hemos acabado
			if( dSliceStartDate > dEndDate )
			{
				break ;
			}

			// Si la fecha inicio del tramo es menor o igual que la fecha inicio del registro hay que continuar
			if( dSliceStartDate <= dStartDate )
			{
				continue ;
			}

			// Hay que partir el registro

            ret=ai_poRecordSet->Register[i].Insert();
            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_CANT_ADD_RECORD, ERRORLOG, M4ObjNodeN(ai_poRecordSet->GetpAccess()->GetpCMCR(),ai_poRecordSet->GetpNodeDef()->GetHandle() )<<"ApplyValue (1)"<<DumpTreePath(ai_poRecordSet->GetpAccess(), ai_poRecordSet->GetpNodeDef()->GetHandle()) << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
       
            //copiamos del registro  i+1 al i
            oReg.MoveTo (i+1);
            ai_poRecordSet->Register[i]<<oReg; //siempre retorna exito

			// Asignamos las fechas correctamente
            vTemp.Type=M4CL_CPP_TYPE_DATE;
            vTemp.Data.DoubleData=dSliceStartDate;
            ai_poRecordSet->Register[i+1].Item[m_OrgInfo.idxStart].Value.Set(vTemp);
            
            vTemp.Data.DoubleData=dSliceStartDate-m_dTimeUnit;
            ai_poRecordSet->Register[i].Item[m_OrgInfo.idxEnd].Value.Set(vTemp);

            vTemp.Type=M4CL_CPP_TYPE_NULL;
            ai_poRecordSet->Register.Item[m_OrgInfo.idxFatherRegister].Value.Set(vTemp); //no nos movemos porque estamos posicionados

            //si el change reason es != null en el añadido lo ponemos a null, así solo tendremos 
            //a !=null los "originales" (no los de los tramos creados). Al aplicar, pondremos el change reason del organigrama si no es nulo
            //y el del historico si el del org. es nulo.
            
             //leemos en i el idxChangeReason --No nos movemos porque ya estamos posicioneados
            if ( m_OrgInfo.idxChangeReason != M4DM_EOF_INDEX ) {
                ret=ai_poRecordSet->Register[i+1].Item[m_OrgInfo.idxChangeReason].Value.Get(vChange);
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "OrgChart.ChangeReason#CreateOrganigram" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
                
                if (vChange.Type !=M4CL_CPP_TYPE_NULL){ //lo ponemos a nulo
                    vChange.Type = M4CL_CPP_TYPE_NULL;
                    ai_poRecordSet->Register.Item[m_OrgInfo.idxChangeReason].Value.Set(vChange);
                }
            }
            
            i++;
			iNumReg++ ;
        }
    }


    pstArguments[0].Type=M4CL_CPP_TYPE_NUMBER;
    pstArguments[3].Type=M4CL_CPP_TYPE_NUMBER;
	pstArguments[6].Type=M4CL_CPP_TYPE_NUMBER;
    
    pstArguments[2].Type=M4CL_CPP_TYPE_NUMBER;
    pstArguments[5].Type=M4CL_CPP_TYPE_NUMBER;
	pstArguments[8].Type=M4CL_CPP_TYPE_NUMBER;
    
    pstArguments[0].Data.DoubleData=m_OrgInfo.idxSonValue;
    pstArguments[3].Data.DoubleData=m_OrgInfo.idxStart;
	pstArguments[6].Data.DoubleData=m_OrgInfo.idxEnd;
    
	/* Bug 0200264
	Se hace la comparación case sensitive
	*/
    pstArguments[2].Data.DoubleData=M4CL_CASE_EQUAL_FUNCTION;
    pstArguments[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION ;
	pstArguments[8].Data.DoubleData=M4CL_BIGGER_OR_EQUAL_FUNCTION;

	//(ciclos)Creamos un array de iNumReg posiciones
	m4int32_t *aiGraph=new m4int32_t[iNumReg];

    for (i=0; i<iNumReg; ++i)
	{
        poRegister= &(ai_poRecordSet->Register[i]);

        ret = poRegister->Item[m_OrgInfo.idxFatherValue].Value.Get(vFatherValue);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poRegister->GetpAccess()->GetpCMCR(),poRegister->Item.ItemDef.GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );	//En estos mensajes de error el register.item ya esta movido.
        
        ret = poRegister->Item[m_OrgInfo.idxStart].Value.Get(vStartDate);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poRegister->GetpAccess()->GetpCMCR(),poRegister->Item.ItemDef.GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );

		ret = poRegister->Item[m_OrgInfo.idxEnd].Value.Get(vEndDate);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poRegister->GetpAccess()->GetpCMCR(),poRegister->Item.ItemDef.GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );

        //rellenamos el array de busqueda-> 6 elementos

        pstArguments[1]=vFatherValue;
        pstArguments[4]=vStartDate;
		pstArguments[7]=vEndDate;

        //buscamos
        findRet=ai_poRecordSet->Register.FindSec (pstArguments, 9 , &iRegister);

        if (iRegister!=-1){

			vTemp.Type=M4CL_CPP_TYPE_NUMBER;
            vTemp.Data.DoubleData=iRegister;
            ret=ai_poRecordSet->Register[i].Item[m_OrgInfo.idxFatherRegister].Value.Set (vTemp);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG,M4ObjNodeItemI(ai_poRecordSet->GetpAccess()->GetpCMCR() , ai_poRecordSet->Register.Item.ItemDef.GetHandle() )<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        }

		//(ciclos)Metemos en la posición i del array el valor del padre (iRegister), aunque sea -1
		aiGraph[i]=iRegister;
    }

	//miramos si tiene o no ciclos(resolvemos)->Si error =>mensaje y ret=M4_ERROR
	ClVMCycleSimple oCicleSolver(aiGraph,iNumReg);
	ret=oCicleSolver.DetectCyclesOrdering();
	delete []aiGraph;

	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ORG_CHART_CICLE, M4ObjNodeN(ai_poRecordSet->GetpAccess()->GetpCMCR(),ai_poRecordSet->GetpNodeDef()->GetHandle()) << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
	}
	
	//retornamos lo que corresponda
    return ret;
}

//-----------------------
//CreateOrgChart NoSlices
//-----------------------
m4return_t ClVMPayRoll::_CreateOrganigramNoSlices (ClAccessRecordSet *ai_poRecordSet)
{
    m4return_t      findRet,ret=M4_SUCCESS;

    m4uint32_t      iNumReg,i;
    m4VariantType    vFatherValue , vStartDate, vTemp;
    m4VariantType   pstArguments[6];
    ClRegister      *poRegister;
    m4int32_t       iRegister;

    M4_ASSERT (m_poDatesStack);


    iNumReg=ai_poRecordSet->Register.Count();

    pstArguments[0].Type=M4CL_CPP_TYPE_NUMBER;
    pstArguments[3].Type=M4CL_CPP_TYPE_NUMBER;
    
    pstArguments[2].Type=M4CL_CPP_TYPE_NUMBER;
    pstArguments[5].Type=M4CL_CPP_TYPE_NUMBER;
    
    pstArguments[0].Data.DoubleData=m_OrgInfo.idxSonValue;
    pstArguments[3].Data.DoubleData=m_OrgInfo.idxStart;
    
	/* Bug 0200264
	Se hace la comparación case sensitive
	*/
    pstArguments[2].Data.DoubleData=M4CL_CASE_EQUAL_FUNCTION;
    pstArguments[5].Data.DoubleData=M4CL_SMALLER_OR_EQUAL_FUNCTION;

	pstArguments[4].Type=M4CL_CPP_TYPE_DATE;
	pstArguments[4].Data.DoubleData=m_dRun;

	//(ciclos)Creamos un array de iNumReg posiciones
	m4int32_t *aiGraph=new m4int32_t[iNumReg];

    for (i=0; i<iNumReg; ++i)
	{
        poRegister= &(ai_poRecordSet->Register[i]);
        
        ret = poRegister->Item[m_OrgInfo.idxFatherValue].Value.Get(vFatherValue);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poRegister->GetpAccess()->GetpCMCR(),poRegister->Item.ItemDef.GetHandle())<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        //rellenamos el array de busqueda-> 6 elementos

        pstArguments[1]=vFatherValue;
    
        //buscamos
        findRet=ai_poRecordSet->Register.FindSec(pstArguments, 6 , &iRegister);

        if (iRegister!=-1)
		{
            vTemp.Type=M4CL_CPP_TYPE_NUMBER;
            vTemp.Data.DoubleData=iRegister;
            ret=ai_poRecordSet->Register[i].Item[m_OrgInfo.idxFatherRegister].Value.Set (vTemp);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(ai_poRecordSet->GetpAccess()->GetpCMCR() , ai_poRecordSet->Register.Item.ItemDef.GetHandle() )<<"CreateOrgChart" << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
        }
		//(ciclos)Metemos en la posición i del array el valor del padre (iRegister), aunque sea -1
		aiGraph[i]=iRegister;
    }

	//miramos si tiene o no ciclos(resolvemos)->Si error =>mensaje y ret=M4_ERROR
	ClVMCycleSimple oCicleSolver(aiGraph,iNumReg);
	ret=oCicleSolver.DetectCyclesOrdering();
	delete []aiGraph;

	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ORG_CHART_CICLE, M4ObjNodeN(ai_poRecordSet->GetpAccess()->GetpCMCR(),ai_poRecordSet->GetpNodeDef()->GetHandle()) << ai_poRecordSet->GetpAccess()->GetpExecutor()->DumpCallStack() );
	}
	
	//retornamos lo que corresponda
    return ret;
}

//-----------------------------------------------------------------
//PREPARES. Leemos la información necesaria sobre los diferentes RS
//-----------------------------------------------------------------
m4return_t ClVMPayRoll::_PreparePKMapping   (ClItemIndex *ai_PkHistoric, 
                                             ClItemIndex *ai_PkValues,
                                             m4uint32_t  ai_iNumPkMapping){


    m4uint32_t i;
   
    if (ai_iNumPkMapping> M4CL_VM_PR_MAX_PK_MAP) {
        DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_MANY_PKS, ai_iNumPkMapping<<M4CL_VM_PR_MAX_PK_MAP<<"ApplyValue (1)" );
        ai_iNumPkMapping=M4CL_VM_PR_MAX_PK_MAP;
    }
    
    for(i=0; i<ai_iNumPkMapping; ++i){
        m_PkMapping[i].idxField         = ai_PkHistoric[i];
        m_PkMapping[i].idxRelationField = ai_PkValues[i];
    }
    m_iNumMapping=ai_iNumPkMapping;
    return M4_SUCCESS;
}



m4return_t ClVMPayRoll::_PrepareHistorical(ClAccessRecordSet  *ai_poHistorical){
    //por si no viene target
    if (ai_poHistorical  ==0){
        m_HistoricInfo.RS=0;
        m_HistoricInfo.idxStart=-1;
        m_HistoricInfo.idxEnd=-1;
        return M4_SUCCESS;
    }
    return m_HistoricInfo.Prepare (ai_poHistorical);
}




m4return_t ClVMPayRoll::_PreparePeriods(void){

    
    if (&(m_poAccess->Node[M4CL_VM_PR_PERIODS_NODE])==0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4CL_VM_PR_PERIODS_NODE <<M4Obj(m_poAccess->GetpCMCR())<<"SetPeriods" << m_poAccess->GetpExecutor()->DumpCallStack() );
        return M4_ERROR;
    }
    else{
        return m_PeriodsInfo.Prepare( &(m_poAccess->Node->RecordSet));
    }
}
m4return_t ClVMPayRoll::_PrepareDMDComponents(void){

    if ( &(m_poAccess->Node[M4CL_VM_PR_DMD_COMPONENTS_NODE])==0 ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4CL_VM_PR_DMD_COMPONENTS_NODE <<M4Obj(m_poAccess->GetpCMCR())<<"ApplyValue (1)" << m_poAccess->GetpExecutor()->DumpCallStack() );
        return M4_ERROR;
    }
    return m_DmdComponentsInfo.Prepare( &(m_poAccess->Node->RecordSet) );
}


m4return_t ClVMPayRoll::_FillPkBlockHistoricItem (void){

    m4return_t ret=M4_SUCCESS;
    const m4char_t *pszName;
    m4uint32_t iNumSlices, i,j;
    m4VariantType vTemp, vStartDate, vEndDate;
	ClItemIndex		idxItem;

    for(i=0; i<m_iNumMapping; ++i){
        //1- Obtenemos el nombre del item del nodo de historicos
        pszName=m_HistoricInfo.RS->Register.Item[m_PkMapping[i].idxField].ItemDef.Id();         

        //2.- Nos intentamos mover en el nodo intermedio a ese item, que debe ser de bloque
        idxItem=m_EmployeeInfo.RS->Register[0].Item.TryMove(pszName,M4_TRUE);
        
        if (idxItem != M4DM_EOF_INDEX) { //si no es eof creamos slices y copiamos

			m_EmployeeInfo.RS->Register.Item.MoveTo(idxItem); //savemos que existe

            iNumSlices=m_HistoricInfo.RS->Register.Count();
            if (iNumSlices==0) continue; //si no hay nada en el historico no creamos slices
            
            m_EmployeeInfo.RS->Register.Item.Value.DeleteAllSlices();
            //m_EmployeeInfo.RS->Register.Item.Value.AllocSlices(iNumSlices);

            for (j=0; j<iNumSlices; ++j){ //para cada registro leemos el valoer y las fechas
                
               //leemos la fecha de inicio
                ret=m_HistoricInfo.RS->Register[j].Item[m_HistoricInfo.idxStart].Value.Get (vStartDate);
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.StartDate#_FillPkBlockHistoricItem" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                //ponemos la fecha inicio en el slice j
            //    ret=m_EmployeeInfo.RS->Register.Item.Value[ClSliceIndex(j)].SetStartDate (vTemp.Data.DoubleData);
             //   M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_SLICE_CREATION, ERRORLOG, "_FillPkBlockHistoricItem. Error setting start date in slice .")	;

                
                //leemos la fecha fin
                ret=m_HistoricInfo.RS->Register.Item[m_HistoricInfo.idxEnd].Value.Get (vEndDate); 
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.EndDate#_FillPkBlockHistoricItem" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                //ponemos la fecha fin en el slice j
               // ret=m_EmployeeInfo.RS->Register.Item.Value.SetEndDate (vTemp.Data.DoubleData);
                //M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_SLICE_CREATION, ERRORLOG, "_FillPkBlockHistoricItem. Error setting end date in slice .")	;
                

                //leemos el valor 
                ret=m_HistoricInfo.RS->Register.Item[m_PkMapping[i].idxField].Value.Get (vTemp); //no posicionamos el item porque ya lo hemos hecho
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.pk item#_FillPkBlockHistoricItem" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                //ponemos el valor
                //ret=m_EmployeeInfo.RS->Register.Item.Value[ClSliceIndex(j)].Set (vTemp);
                //M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_WRITING_SLICE, ERRORLOG, "_FillPkBlockHistoricItem. Error setting value in slice ." << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

				/* Bug 0076620
				Ya no se puede asignar a la vez que se crea el tramo
				Primero hay que crear el tramo y después asignar
				*/
				/* Bug 0101447
				No hay que posicionarse por posición, porque puede que el tramo ya exista.
				La creación del tramo ya lo deja posicionado
				*/
 				ret=m_EmployeeInfo.RS->Register.Item.Value.AddSlice (vStartDate.Data.DoubleData,vEndDate.Data.DoubleData);
				ret=m_EmployeeInfo.RS->Register.Item.Value.Set (vTemp);
				
				M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_WRITING_SLICE, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<<DateLog (vStartDate.Data.DoubleData)<<DateLog(vEndDate.Data.DoubleData)<< "ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                    ret=M4_SUCCESS;
                }


            }


        }
    }
    return M4_SUCCESS;

}


m4return_t ClVMPayRoll::_FillPkBlockHistoricItemNoSlices (void){

    m4return_t ret=M4_SUCCESS;
    const m4char_t *pszName;
    m4uint32_t i;
    m4VariantType vTemp;
	ClItemIndex		idxItem;

    for(i=0; i<m_iNumMapping; ++i){
        //1- Obtenemos el nombre del item del nodo de historicos
        pszName=m_HistoricInfo.RS->Current.Item[m_PkMapping[i].idxField].ItemDef.Id();         

        //2.- Nos intentamos mover en el nodo intermedio a ese item, que debe ser de bloque
        idxItem=m_EmployeeInfo.RS->Register[0].Item.TryMove(pszName,M4_TRUE);
        
        if (idxItem != M4DM_EOF_INDEX) { //si no es eof copiamos
            m_EmployeeInfo.RS->Register.Item.MoveTo(idxItem); //existe

            //leemos el valor 
            ret=m_HistoricInfo.RS->Current.Item[m_PkMapping[i].idxField].Value.Get (vTemp); //no posicionamos el item porque ya lo hemos hecho
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Historic.pk_item ["<<LogCat<<i<<LogCat<<"]"<<"_FillPkBlockHistoricItemNoSlices" << m_HistoricInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            //ponemos el valor
            ret=m_EmployeeInfo.RS->Register.Item.Value.Set (vTemp);
            M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG,M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(), m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<< "ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            if (ret==M4_WARNING){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                ret=M4_SUCCESS;
            }
            
        }
    }
    return M4_SUCCESS;

}


//---------------------------------------------------
//              Date to members
//---------------------------------------------------
//Para poner en los miembros las fechas y el modo 
m4return_t ClVMPayRoll::_DatesToMembers (void){

    m4return_t ret;
    m4VariantType vTemp;
	
	if (!m_EmployeeInfo.RS){
		//no tenemos las fechas puestas->Error
		DUMP_CHLOG_ERROR(M4_CH_VM_AV_NO_DATES_SET);
		return M4_ERROR;
	}
    //leemos la fecha inicio y fin del nodo intermedio
    ret=m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxStartFilter].Value.Get (vTemp); 
    if (ret!=M4_SUCCESS){
		//usamos itemdef porque debe estar posicionado..
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_ERROR;
	}
    m_dStart=vTemp.Data.DoubleData;

    ret=m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxEndFilter].Value.Get (vTemp); 
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_ERROR;
	}

    m_dEnd=vTemp.Data.DoubleData;

    //Leemos la fecha de ejecución del nodo intermedio
    ret=m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxRunDate].Value.Get (vTemp); 
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_ERROR;
	}

    m_dRun=vTemp.Data.DoubleData;

    //Leemnos el slice mode del nodo intermedio
    ret=m_EmployeeInfo.RS->Register.Item[m_EmployeeInfo.idxSliceMode].Value.Get (vTemp); 
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(m_EmployeeInfo.RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo.RS->Register.Item.ItemDef.GetHandle())<<"ApplyValue (1)" << m_EmployeeInfo.RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		return M4_ERROR;
	}

    if (vTemp.Type==M4CL_CPP_TYPE_NULL) m_bSlice=M4_FALSE;
	else m_bSlice=(vTemp.Data.DoubleData==0 ? M4_FALSE : M4_TRUE);

    return M4_SUCCESS;
}


//---------------------------------------------------
//              IsEmployeeSetted
//---------------------------------------------------
//Comprobamso qye el RS del employee y el del RS que nos pasan (el del org_chart) sean del mismo accees
//Si es así, es que se ha hecho un setEmployee, y por tanto han tenido que llamar al SetDates
//antes de llamar al CreateOrgChart.
//Además el setDates y el createOrgChart se han tenido que hacer dentro de la misma transaccion C/S si es que están
//en oltp, porque si lo hacen en distintas transacciones, se destruye el access y por tanto el RS del ORG
//será de un access distinto del que es el RS del employee
m4bool_t ClVMPayRoll::IsEmployeeSetted(ClAccessRecordSet *ai_poRecordSet){
	
	if (ai_poRecordSet==m_EmployeeInfo.RS){
		return M4_TRUE;
	}
	ClAccess* poAccess=ai_poRecordSet->GetpAccess();
	ClAccessRecordSet* poRS;
	
	m4uint32_t	iNodeCount=poAccess->Node.Count();
	m4uint32_t	i;
	m4bool_t	bRes=M4_FALSE;

	for (i=0; i<iNodeCount; ++i){
		poRS=&(poAccess->Node.Get(ClNodeIndex(i))->RecordSet);
		if (poRS==m_EmployeeInfo.RS){
			bRes=M4_TRUE;
			break;
		}
	}
	return bRes;
}
