//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             loadercn.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:18-Aug-97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano Perez (29/06/98) - LoadLongExpression
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "eng.h"
#include "engdf.inl"
#include "xapichn.h"
#include "m4time.hpp"
#include "cltimer.h"
#include "clargs.h"

#include "designdd.h"

//#include "ddlf_chn.hmg"
#include "loadercn.hmg"
#include "ddlf_chn.h"


//================================= ClENG_StorageDepChn ======================

ClENG_StorageDepChn::ClENG_StorageDepChn()
{
	m_poInsp=GET_INSP();
}


m4bool_t ClENG_StorageDepChn::Init(ClMIT_Chn *ai_poChn) 
{
	m4bool_t SwOk=M4_TRUE;	

	m_poChnDef = ai_poChn;

	//Nodos de las Tablas de dependencias de Item
	m_poNodePageFieldDep        = m_poChnDef->GetNode("SRP_PAGE_FIELD_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodePageFieldDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSectionDep          = m_poChnDef->GetNode("SRP_SECTION_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeSectionDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeContainerDep        = m_poChnDef->GetNode("SRP_CONTAINER_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeContainerDep) { 	SwOk = M4_FALSE ; goto exit ; }     
	m_poNodeContainerFieldDep   = m_poChnDef->GetNode("SRP_CONT_FIELD_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeContainerFieldDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeReportDep   = m_poChnDef->GetNode("SRP_REPORT_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeReportDep) { 	SwOk = M4_FALSE ; goto exit ; }

	//Nodos de las Tablas de dependencias de Nodo
	m_poNodePageFieldNodeDep        = m_poChnDef->GetNode("SRP_PAGE_FIELD_NODE_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodePageFieldNodeDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSectionNodeDep          = m_poChnDef->GetNode("SRP_SECTION_NODE_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeSectionNodeDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeContainerNodeDep        = m_poChnDef->GetNode("SRP_CONTAINER_NODE_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeContainerNodeDep) { 	SwOk = M4_FALSE ; goto exit ; }     
	m_poNodeContainerFieldNodeDep   = m_poChnDef->GetNode("SRP_CONT_FIELD_NODE_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeContainerFieldNodeDep) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeReportNodeDep   = m_poChnDef->GetNode("SRP_REPORT_NODE_DEP", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeReportNodeDep) { 	SwOk = M4_FALSE ; goto exit ; }

	//Nodos del REPORT_CHANNEL_PT
	m_poNodeSec					= m_poChnDef->GetNode("SRP_REPORT_SECTS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSec) { 			SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCnt				= m_poChnDef->GetNode("SRP_SECT_CONTAIN_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCnt) { 			SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCntFld			= m_poChnDef->GetNode("SRP_SECT_CONT_FLDS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCntFld) { 		SwOk = M4_FALSE ; goto exit ; }
	m_poNodePageCntFld			= m_poChnDef->GetNode("SRP_PAGE_FIELDS"		,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodePageCntFld) { 		SwOk = M4_FALSE ; goto exit ; }

exit:
	return SwOk;
}


m4bool_t ClENG_StorageDepChn::StoreItemDependency(const m4char_t *ai_pcIdTI, const m4char_t *ai_pcIdItem)
{
	ClMIT_ChnItem Item, ItemAux, ItemCnt, ItemFld;
	m4bool_t SwOk = M4_TRUE;
	m4int32_t Record;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_StorageDepChn::StoreItemDependency()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	switch (m_DepType) {
		case PAGE_FLD:  //Posicionamiento en la pagina
						ItemAux.Init(m_poNodePageCntFld, "ID_PAGE");
						if(!ItemAux.FindDouble(m_uIdPage)) {
							SwOk=M4_FALSE;
							goto exit;
						};
						//Posicionamiento en el field
						ItemAux.Init(m_poNodePageCntFld, "ID_FIELD");
						if(!ItemAux.FindDouble(m_uIdField)) {
							SwOk=M4_FALSE;
							goto exit;
						};
						//Buscamos el ID_ITEM y nos posicionamos
						Item.Init( m_poNodePageFieldDep,"ID_ITEM");
						Record = m_poNodePageFieldDep->GetNumRecord();
						if(Item.FindString(ai_pcIdItem)) {
							SwOk=M4_FALSE;
							goto exit;
						};

						//Insercion de datos
						m_poNodePageFieldDep->MoveTo(Record);
						m_poNodePageFieldDep->Insert();
					
						//ID_ITEM
						Item.m_oValue.InitString(ai_pcIdItem);
						Item.Save();

						//ID_TI
						Item.Init(m_poNodePageFieldDep,"ID_TI");
						Item.m_oValue.InitString(ai_pcIdTI);
						Item.Save();
					
						//NUM_USES
						Item.Init(m_poNodePageFieldDep,"NUM_USES");
						Item.m_oValue.InitDouble(1,20,0);
						Item.Save();
					    break;
					   
		case SECTION: //Posicionamiento en la seccion
				      ItemAux.Init(m_poNodeSec, "ID_SECTION");
				      if(!ItemAux.FindDouble(m_uIdSection)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };
					  //Buscamos el ID_ITEM y nos posicionamos
			          Item.Init(m_poNodeSectionDep,"ID_ITEM");
					  Record = m_poNodeSectionDep->GetNumRecord();
					  if(Item.FindString(ai_pcIdItem)) {
						  SwOk=M4_FALSE;
					  	  goto exit;
					  };

					  //Insercion de datos
					  m_poNodeSectionDep->MoveTo(Record);
					  m_poNodeSectionDep->Insert();
					
					  //ID_ITEM
					  Item.m_oValue.InitString(ai_pcIdItem);
					  Item.Save();

					  //ID_TI
					  Item.Init(m_poNodeSectionDep,"ID_TI");
					  Item.m_oValue.InitString(ai_pcIdTI);
					  Item.Save();
					
					  //NUM_USES
					  Item.Init(m_poNodeSectionDep,"NUM_USES");
					  Item.m_oValue.InitDouble(1,20,0);
					  Item.Save();
					  break;

		case CONTAIN: //Posicionamiento en la seccion
				      ItemAux.Init(m_poNodeSec, "ID_SECTION");
				      if(!ItemAux.FindDouble(m_uIdSection)) {
					  	  SwOk=M4_FALSE;
						  goto exit;
					  };
					  //Posicionamiento en el container
					  ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					  if(!ItemAux.FindDouble(m_uIdContainer)) {
					      SwOk=M4_FALSE;
					      goto exit;
					  };
			          //Buscamos el ID_ITEM y nos posicionamos
			          Item.Init(m_poNodeContainerDep,"ID_ITEM");
					  Record = m_poNodeContainerDep->GetNumRecord();
					  if(Item.FindString(ai_pcIdItem)) {
						  SwOk=M4_FALSE;
					  	  goto exit;
					  };

					  //Insercion de datos
					  m_poNodeContainerDep->MoveTo(Record);
					  m_poNodeContainerDep->Insert();
					
					  //ID_ITEM
					  Item.m_oValue.InitString(ai_pcIdItem);
					  Item.Save();

					  //ID_TI
					  Item.Init(m_poNodeContainerDep,"ID_TI");
					  Item.m_oValue.InitString(ai_pcIdTI);
					  Item.Save();
					
					  //NUM_USES
					  Item.Init(m_poNodeContainerDep,"NUM_USES");
					  Item.m_oValue.InitDouble(1,20,0);
					  Item.Save();
					  break;
		
		case FIELD:	//Posicionamiento en la seccion
				    ItemAux.Init(m_poNodeSec, "ID_SECTION");
				    if(!ItemAux.FindDouble(m_uIdSection)) {
						SwOk=M4_FALSE;
						goto exit;
					};
					//Posicionamiento en el container
					ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					if(!ItemAux.FindDouble(m_uIdContainer)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};
					//Posicionamiento en el field
					ItemAux.Init(m_poNodeSecCntFld, "ID_FIELD");
					if(!ItemAux.FindDouble(m_uIdField)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};
			        //Buscamos el ID_ITEM y nos posicionamos
			        Item.Init(m_poNodeContainerFieldDep,"ID_ITEM");
					Record = m_poNodeContainerFieldDep->GetNumRecord();
					if(Item.FindString(ai_pcIdItem)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};

					//Insercion de datos
					m_poNodeContainerFieldDep->MoveTo(Record);
					m_poNodeContainerFieldDep->Insert();
					
					//ID_ITEM
					Item.m_oValue.InitString(ai_pcIdItem);
					Item.Save();

					//ID_TI
					Item.Init(m_poNodeContainerFieldDep,"ID_TI");
					Item.m_oValue.InitString(ai_pcIdTI);
					Item.Save();
					
					//NUM_USES
					Item.Init(m_poNodeContainerFieldDep,"NUM_USES");
					Item.m_oValue.InitDouble(1,20,0);
					Item.Save();
					break;

		case REPORT: //Buscamos el ID_ITEM y nos posicionamos
			         Item.Init(m_poNodeReportDep,"ID_ITEM");
					 Record = m_poNodeReportDep->GetNumRecord();
					 if(Item.FindString(ai_pcIdItem)) {
					     SwOk=M4_FALSE;
					     goto exit;
					 };

					 //Insercion de datos
					 m_poNodeReportDep->MoveTo(Record);
					 m_poNodeReportDep->Insert();
					
					 //ID_ITEM
					 Item.m_oValue.InitString(ai_pcIdItem);
					 Item.Save();

					 //ID_TI
					 Item.Init(m_poNodeReportDep,"ID_TI");
					 Item.m_oValue.InitString(ai_pcIdTI);
					 Item.Save();
					
					 //NUM_USES
					 Item.Init(m_poNodeReportDep,"NUM_USES");
					 Item.m_oValue.InitDouble(1,20,0);
					 Item.Save();
					 break;
	};

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_StorageDepChn::StoreItemDependency()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk;
}

m4bool_t ClENG_StorageDepChn::StoreNodeDependency(const m4char_t *ai_pcIdT3, const m4char_t *ai_pcIdNode)
{
	ClMIT_ChnItem Item, ItemAux, ItemCnt, ItemFld;
	m4bool_t SwOk = M4_TRUE;
	m4int32_t Record;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_StorageDepChn::StoreNodeDependency()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	switch (m_DepType) {
		case PAGE_FLD:  //Posicionamiento en la pagina
						ItemAux.Init(m_poNodePageCntFld, "ID_PAGE");
						if(!ItemAux.FindDouble(m_uIdPage)) {
							SwOk=M4_FALSE;
							goto exit;
						};
						//Posicionamiento en el field
						ItemAux.Init(m_poNodePageCntFld, "ID_FIELD");
						if(!ItemAux.FindDouble(m_uIdField)) {
							SwOk=M4_FALSE;
							goto exit;
						};
						//Busco ID_NODE y si existe salimos
						Item.Init(m_poNodePageFieldNodeDep,"ID_NODE");
						Record = m_poNodePageFieldNodeDep->GetNumRecord();  
						if(Item.FindString(ai_pcIdNode)) {
					 		break;
						};
			
						//Insercion de datos
						m_poNodePageFieldNodeDep->MoveTo(Record);
			   			m_poNodePageFieldNodeDep->Insert();

						//ID_NODE
						Item.m_oValue.InitString(ai_pcIdNode);
						Item.Save();
 					 
						//ID_TI
						Item.Init(m_poNodePageFieldNodeDep,"ID_T3");
						Item.m_oValue.InitString(ai_pcIdT3);
						Item.Save();
					
						//NUM_USES
						Item.Init(m_poNodePageFieldNodeDep,"NUM_USES");
						Item.m_oValue.InitDouble(1,20,0);
						Item.Save();
					    break;
					   
		case SECTION: //Posicionamiento en la seccion
					  ItemAux.Init(m_poNodeSec, "ID_SECTION");
					  if(!ItemAux.FindDouble(m_uIdSection)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };

					  //Busco ID_NODE y si existe salimos
					  Item.Init( m_poNodeSectionNodeDep,"ID_NODE");
					  Record = m_poNodeSectionNodeDep->GetNumRecord();
					  if(Item.FindString(ai_pcIdNode)) {
					 	break;
					  };
			
					  //Insercion de datos
					  m_poNodeSectionNodeDep->MoveTo(Record);
			   		  m_poNodeSectionNodeDep->Insert();

					  //ID_NODE
					  Item.m_oValue.InitString(ai_pcIdNode);
					  Item.Save();
 					 
					  //ID_TI
					  Item.Init(m_poNodeSectionNodeDep,"ID_T3");
					  Item.m_oValue.InitString(ai_pcIdT3);
					  Item.Save();
					
					  //NUM_USES
					  Item.Init(m_poNodeSectionNodeDep,"NUM_USES");
					  Item.m_oValue.InitDouble(1,20,0);
					  Item.Save();
					  break;

		case CONTAIN: //Posicionamiento en la seccion
					  ItemAux.Init(m_poNodeSec, "ID_SECTION");
					  if(!ItemAux.FindDouble(m_uIdSection)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };
					  //Posicionamiento en el container
					  ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					  if(!ItemAux.FindDouble(m_uIdContainer)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };
					  //Busco ID_NODE y si existe salimos
					  Item.Init( m_poNodeContainerNodeDep,"ID_NODE");
					  Record = m_poNodeContainerNodeDep->GetNumRecord();
					  if(Item.FindString(ai_pcIdNode)) {
					 	break;
					  };
			
					  //Insercion de datos
					  m_poNodeContainerNodeDep->MoveTo(Record);
			   		  m_poNodeContainerNodeDep->Insert();

					  //ID_NODE
					  Item.m_oValue.InitString(ai_pcIdNode);
					  Item.Save();
 					 
					  //ID_TI
					  Item.Init(m_poNodeContainerNodeDep,"ID_T3");
					  Item.m_oValue.InitString(ai_pcIdT3);
					  Item.Save();
					
					  //NUM_USES
					  Item.Init(m_poNodeContainerNodeDep,"NUM_USES");
					  Item.m_oValue.InitDouble(1,20,0);
					  Item.Save();
					  break;

		case FIELD:	//Posicionamiento en la seccion
				    ItemAux.Init(m_poNodeSec, "ID_SECTION");
				    if(!ItemAux.FindDouble(m_uIdSection)) {
						SwOk=M4_FALSE;
						goto exit;
					};
					//Posicionamiento en el container
					ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					if(!ItemAux.FindDouble(m_uIdContainer)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};
					//Posicionamiento en el field
					ItemAux.Init(m_poNodeSecCntFld, "ID_FIELD");
					if(!ItemAux.FindDouble(m_uIdField)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};
					//Busco ID_NODE y si existe salimos
					Item.Init( m_poNodeContainerFieldNodeDep,"ID_NODE");
				 	Record = m_poNodeContainerFieldNodeDep->GetNumRecord();	
					if(Item.FindString(ai_pcIdNode)) {
					 	break;
					};
			
					//Insercion de datos
					m_poNodeContainerFieldNodeDep->MoveTo(Record);
			   		m_poNodeContainerFieldNodeDep->Insert();

					//ID_NODE
					Item.m_oValue.InitString(ai_pcIdNode);
					Item.Save();
 					 
					//ID_TI
					Item.Init(m_poNodeContainerFieldNodeDep,"ID_T3");
					Item.m_oValue.InitString(ai_pcIdT3);
					Item.Save();
					
					//NUM_USES
					Item.Init(m_poNodeContainerFieldNodeDep,"NUM_USES");
					Item.m_oValue.InitDouble(1,20,0);
					Item.Save();
					break;

		case REPORT: //Busco ID_NODE y si existe salimos
					 Item.Init( m_poNodeReportNodeDep,"ID_NODE");
					 Record = m_poNodeReportNodeDep->GetNumRecord();
					 if(Item.FindString(ai_pcIdNode)) {
					 	break;
					 };
			
					 //Insercion de datos
					 m_poNodeReportNodeDep->MoveTo(Record);
			   		 m_poNodeReportNodeDep->Insert();

					 //ID_NODE
					 Item.m_oValue.InitString(ai_pcIdNode);
					 Item.Save();
 					 
					 //ID_TI
					 Item.Init(m_poNodeReportNodeDep,"ID_T3");
					 Item.m_oValue.InitString(ai_pcIdT3);
					 Item.Save();
					
					 //NUM_USES
					 Item.Init(m_poNodeReportNodeDep,"NUM_USES");
					 Item.m_oValue.InitDouble(1,20,0);
					 Item.Save();
					 break;
	};


exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_StorageDepChn::StoreNodeDependency()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk;
}

m4bool_t ClENG_StorageDepChn::DeleteAllDependencies()
{
	ClMIT_ChnItem Item, ItemAux, ItemCnt, ItemFld;
	m4bool_t SwOk = M4_TRUE;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_StorageDepChn::DeleteAllDependencies()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	//Borra todos los registros del RecordSet Updated
	switch (m_DepType) {

	    case PAGE_FLD:	ItemAux.Init(m_poNodePageCntFld, "ID_PAGE");
						if(!ItemAux.FindDouble(m_uIdPage)) {
						SwOk=M4_FALSE;
							goto exit;
						};
						//Posicionamiento en el field
						ItemAux.Init(m_poNodePageCntFld, "ID_FIELD");
						if(!ItemAux.FindDouble(m_uIdField)) {
							SwOk=M4_FALSE;
							goto exit;
						};

						SwOk = M4_BOOL( SwOk && m_poNodePageFieldDep->DeleteAll() );
						SwOk = M4_BOOL( SwOk && m_poNodePageFieldNodeDep->DeleteAll() );						
						break;

		case SECTION: ItemAux.Init(m_poNodeSec, "ID_SECTION");
					  if(!ItemAux.FindDouble(m_uIdSection)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };

			          SwOk = M4_BOOL( SwOk && m_poNodeSectionDep->DeleteAll() );
					  SwOk = M4_BOOL( SwOk && m_poNodeSectionNodeDep->DeleteAll() );						
			          break;

		case CONTAIN: ItemAux.Init(m_poNodeSec, "ID_SECTION");
					  if(!ItemAux.FindDouble(m_uIdSection)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };
					  //Posicionamiento en el container
					  ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					  if(!ItemAux.FindDouble(m_uIdContainer)) {
						  SwOk=M4_FALSE;
						  goto exit;
					  };

			          SwOk = M4_BOOL( SwOk && m_poNodeContainerDep->DeleteAll() );
					  SwOk = M4_BOOL( SwOk && m_poNodeContainerNodeDep->DeleteAll() );
			          break;
					   
		case FIELD:	//Posicionamiento en la seccion
				    ItemAux.Init(m_poNodeSec, "ID_SECTION");
				    if(!ItemAux.FindDouble(m_uIdSection)) {
						SwOk=M4_FALSE;
						goto exit;
					};
					//Posicionamiento en el container
					ItemAux.Init(m_poNodeSecCnt, "ID_CONTAINER");
					if(!ItemAux.FindDouble(m_uIdContainer)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};
					//Posicionamiento en el field
					ItemAux.Init(m_poNodeSecCntFld, "ID_FIELD");
					if(!ItemAux.FindDouble(m_uIdField)) {
					    SwOk=M4_FALSE;
					    goto exit;
					};

					SwOk = M4_BOOL( SwOk && m_poNodeContainerFieldDep->DeleteAll() );
				    SwOk = M4_BOOL( SwOk && m_poNodeContainerFieldNodeDep->DeleteAll() );
					break;

		case REPORT: SwOk = M4_BOOL( SwOk && m_poNodeReportNodeDep->DeleteAll() );
					 SwOk = M4_BOOL( SwOk && m_poNodeReportDep->DeleteAll() );	
			         break;
	};	

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_StorageDepChn::DeleteAllDependencies()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk;
}

//================================= ClENG_DDLF_Chn ======================

struct ClENG_DDLF_Chn::StAuxFormat const ClENG_DDLF_Chn::m_oAuxFormat[ClENG_Format::MAX_NUM_PROPS] = 
{
	{"ID_COUNTRY",				"sFCountry"},
	{"LENGTH",					"iFLength"},
	{"ID_ALIGNMENT_TYPE",		"iFAlig"},
	{"FILL_STRING",				"sFCharsFill"},
	{"IS_BEGIN_OFFSET",			"iFBeginStr"},
	{"STRING_OFFSET_NUM",		"iFNoffsetstr"},
	{"ID_CASE_TYPE",			"iFCase"},
	{"SUBST_STRING",			"sFSubst"},
	{"IS_BEFORE_SUBST",			"iFSubst"},
	{"REQUIRED_DEC_NUM",		"iFCompDec"},
	{"REQUIRED_INT_NUM",		"iFCompInt"},
	{"PRESRV_RIGHT_ZEROS",		"iFLeadZero"},
	{"SHOW_LEFT_ZERO",			"iFLeftZero"},
	{"DECIMAL_STRING",			"sFDecimal"},
	{"THOUSAND_STRING",			"sFThousand"},
	{"IS_TRUNCATED",			"iFTrunc"},
	{"IS_CURRENCY",				"iFCur"},
	{"CURRENCY_STRING",			"sFCurrency"},
	{"NEGATIVE_MASK",			"sFMaskNeg"},
	{"POSITIVE_CURR_MASK",		"sFMaskCurPos"},
	{"NEGATIVE_CURR_MASK",		"sFMaskCurNeg"},
	{"DAY_STRING",				"sFDay"},
	{"WEEK_DAY_STRING",			"sFWeekDay"},
	{"MONTH_STRING",			"sFMonth"},
	{"YEAR_STRING",				"sFYear"},
	{"DATE_MASK",				"sFMaskDate"},
	{"MINUTE_STRING",			"sFMinute"},
	{"HOUR_STRING",				"sFHour"},
	{"TIME12_MASK",				"sFMaskTime12"},
	{"TIME24_MASK",				"sFMaskTime24"},
	{"TIMESTAMP_MASK",			"sFMaskTimeStamp"},
	{"NUMBERS_IN_TEXT",			"iFStyle"},
	{"INTEGER_KIND",			"iFIntKind"},
	{"DECIMAL_KIND",			"iFDecKind"},
	{"IS_ACENTLESS",			"iFAcentLess"},
	{"IS_BLANK_DATE",			"iFBlank"},
	{"PLUS_INF_DATE",			"sFPlusInfDate"},
	{"MINUS_INF_DATE",			"sFMinusInfDate"},
	{"SECOND_STRING",			"sFSecond"},
	{"SWORDSTOSUBSTITUTE",		"sFWordsToSubstitute"},
	{"IFWORDSTOSUBSTITUTEMODE",	"iFWordToKeepSubstitute"},
	{"SPREFIXES",				"sFPrefixes"},
	{"IFPREFIXESMODE",			"iFPrefixeMode"},
	{"SSEPARATORS",				"sFSeparators"},
	{"IFSEPARATORSMODE",		"iFSeparatorMode"},
} ;


ClENG_DDLF_Chn::ClENG_DDLF_Chn()
{
	m_poInsp=GET_INSP();
}

void ClENG_DDLF_Chn::End()
{
	if ( m_oChnDef.IsLinked() ) {
		if ( m_LinkDesign == ENG_LINK_UNLOAD_DESIGN || m_LinkDesign == ENG_LINK_LOAD_UNLOAD_DESIGN ) 
		{
			m_oChnDef.Unload() ;
		} ;
	} ;
	m_oChnDef.End() ;
}

m4bool_t ClENG_DDLF_Chn::Init( 
	m4char_t *ai_pcReportId, m4int16_t ai_iExecutionType,
	ENG_LINK_DESIGN_MODE ai_LinkDesign, m4int16_t ai_iMetadataLang,
	m4char_t *ai_pcChannelID, ClAccess *ai_poAccess, 
	ClMIT_Chn::AUTOLOAD_MODE ai_AutoLoadMode,
	m4bool_t ai_bPack, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClMIT_Timer TimerUpDesign ;
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str Str1, Str2 ;	
	ClMIT_ChnItem Item ;
	m4int16_t B ;
	ClMIT_ChnNode::Record_t iRecord;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace 
			<< ClMIT_Msg( GROUP_ID,
			"ClENG_DDLF_Chn::Init() ReportId %0:s",ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcReportId << SEND_MSG   ;
	};
	#endif

	ClENG_DDLF_Chn::End() ;

	m_LinkDesign = ai_LinkDesign ;

	m_oChannelID.StrCpy(ai_pcChannelID);
	
	//Enganche al canal de definicion

	TimerUpDesign.StartReset() ;

	if ( m_LinkDesign == ENG_BUILD_DESIGN) {
		SwOk = m_oChnDef.InitOwn( "REPORT_CHANNEL_PT", ai_iMetadataLang, ClMIT_Trace::PROPAGATE_ERROR ) ;
		if (!SwOk) goto exit ;
	} else {
		SwOk = m_oChnDef.InitCommonAccess( ai_poAccess, ClMIT_Trace::PROPAGATE_ERROR ) ;	
		if (!SwOk) goto exit ;
	} ;
	m_oChnDef.SetAutoLoad( ai_AutoLoadMode ) ;

	TimerUpDesign.Stop() ;

	m_poNodeRpr					= m_oChnDef.GetNode("SRP_REPORTS_PT"		,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeRpr) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeExe					= m_oChnDef.GetNode("SRP_EXECUTION_TPS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeExe) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeRprProp				= m_oChnDef.GetNode("SRP_REPORT_PROPS"		,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeRprProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeFont				= m_oChnDef.GetNode("SRP_REPORT_FONTS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeFont) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeCT					= m_oChnDef.GetNode("SRP_REPORT_CT"			,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeCT) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeCTProp				= m_oChnDef.GetNode("SRP_REPORT_CT_PROPS"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeCTProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeLongExpression		= m_oChnDef.GetNode("SRP_LONG_EXPRESS"		,ClMIT_Trace::PROPAGATE_ERROR);
	if (!m_poNodeLongExpression) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeFormat				= m_oChnDef.GetNode("SRP_REPORT_FORMATS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ; 
	if (!m_poNodeFormat) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSec					= m_oChnDef.GetNode("SRP_REPORT_SECTS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSec) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCnt				= m_oChnDef.GetNode("SRP_SECT_CONTAIN_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCnt) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCntProp			= m_oChnDef.GetNode("SRP_SEC_CONT_PROPS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCntProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCntFld			= m_oChnDef.GetNode("SRP_SECT_CONT_FLDS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCntFld) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecCntFldProp		= m_oChnDef.GetNode("SRP_SC_CNT_FLD_PRP_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecCntFldProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecProp				= m_oChnDef.GetNode("SRP_RPT_SECT_PROPS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeSecSon				= m_oChnDef.GetNode("SRP_FAMILY_SECTS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeSecSon) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodePage				= m_oChnDef.GetNode("SRP_PAGES"				,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodePage) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodePageProp			= m_oChnDef.GetNode("SRP_PAGE_PROPS"		,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodePageProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodePageCntFld			= m_oChnDef.GetNode("SRP_PAGE_FIELDS"		,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodePageCntFld) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodePageCntFldProp		= m_oChnDef.GetNode("SRP_PAGE_FLD_PROPS"	,ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodePageCntFldProp) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeMasterFont			= m_oChnDef.GetNode("SRP_FONTS"				,ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeMasterFont) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeMasterFormat		= m_oChnDef.GetNode("SRP_FORMATS"			,ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeMasterFormat) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeColour				= m_oChnDef.GetNode("SRP_REPORT_COLORS_PT"	,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poNodeColour) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poNodeMasterColour		= m_oChnDef.GetNode("SRP_COLORS"			,ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poNodeMasterColour) { 	SwOk = M4_FALSE ; goto exit ; }
	m_poReportString = m_oChnDef.GetNode("SRP_REPORT_STR", ClMIT_Trace::PROPAGATE_ERROR) ;  
	if (!m_poReportString) { SwOk = M4_FALSE ; goto exit ; };


	// Posicionamiento en el diseño deseado.

	TimerUpDesign.Start() ;

	//Temporal David

	Str1.Accommodate(100);
	Str2.Accommodate(100);

	sprintf(Str1, "FROM &SRP_REPORTS  A WHERE (A.ID_REPORT = '%s')", ai_pcReportId );
	sprintf(Str2, "FROM &SRP_EXECUTION_TPS  A WHERE (A.ID_EXECUTION_TYPE = %i)", ai_iExecutionType);		

	Str1.OverStringValid() ;
	Str2.OverStringValid() ;

	if ( m_LinkDesign == ENG_BUILD_DESIGN ) {

	
		Item.Init( m_poNodeRpr,"FLTR_SQL") ; 	
		Item.m_oValue.InitString(Str1);
		Item.Save() ;

		Item.Init( m_poNodeExe,"FLTR_SQL") ; 	
		Item.m_oValue.InitString(Str2);
		Item.Save() ;

		if ( ! m_oChnDef.Load() ) {
			SwOk=M4_FALSE ;
			goto exit ;
		};

	} else if ( m_LinkDesign == ENG_LINK_LOAD_DESIGN || m_LinkDesign == ENG_LINK_LOAD_UNLOAD_DESIGN)  
	{
		Item.Init( m_poNodeRpr,"FLTR_SQL") ; 	
		Item.m_oValue.InitString(Str1);
		Item.Save() ;

		Item.Init( m_poNodeExe,"FLTR_SQL") ; 	
		Item.m_oValue.InitString(Str2);
		Item.Save() ;

		if ( ! m_oChnDef.Load() ) {
			SwOk=M4_FALSE ;
			goto exit ;
		};
	} ;

	// Pack de diseño
	if ( ai_bPack ) {
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Performing ChnDef.CheckPoint()") << SEND_MSG ; 
		};
		#endif
		m_oChnDef.CheckPoint() ;
	} ;

	TimerUpDesign.Stop() ;

	// Fin temporizacion

	m_oTimerUpDesignValue = TimerUpDesign.GetTimer() ;

	// Posicionamiento en report

	if ( m_LinkDesign == ENG_LINK_DESIGN || m_LinkDesign == ENG_LINK_UNLOAD_DESIGN ) 
	{
		iRecord = m_poNodeRpr->GetNumRecord() ;

		if( iRecord != ClMIT_ChnNode::RECORD_EOF )
		{
			m_poNodeRpr->MoveTo( iRecord ) ;
		}
		else if( m_poNodeRpr->GetCount() > 0 )
		{
			// Si está en eof y hay registros se posiciona en el id report
            ClMIT_ChnItem ItemReport;
			
			ItemReport.Init( m_poNodeRpr, "ID_REPORT") ;

            if( ItemReport.FindIString(ai_pcReportId, M4_TRUE, M4_TRUE) == M4_FALSE )
			{
				goto exit ;
			}
		}
	}
	else
	{
		m_poNodeRpr->GoFirst() ;
	}

	m_poNodeExe->GoFirst() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		ClMIT_ChnItem Item1, Item2 ;
		Item1.InitLoad(m_poNodeRpr,"ID_REPORT") ;
		Item2.InitLoad(m_poNodeExe,"ID_EXECUTION_TYPE") ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "The report selected is %0:s execution type %1:s") 
			<< Item1.m_oValue.GetString() << (unsigned long)Item2.m_oValue.GetDouble() << SEND_MSG ;
	};
	#endif

	if (m_poNodeRpr->IsEOF()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADERCN_MSG_10 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace 
			<< ClMIT_Msg( GROUP_ID,
			"ClENG_DDLF_Chn::Init() ReportId %0:s SwOk %1:s",ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcReportId << SwOk << SEND_MSG   ;
	};
	#endif

	return SwOk ;
} 
	

void ClENG_DDLF_Chn::LoadBaseData( ClENG_DesignData *ao_poDD ) 
{
	m4int16_t B ;

	ClMIT_ChnItem ItemKey, ItemValue, Item, ItemC_R, ItemC_G, ItemC_B, ItemC_Id, ItemC_Name   ;

	StENG_DD_GlobalReport Global ;
	StENG_DD_Colour       Colour ;
	StENG_DD_PageData     Page ;
	StENG_DD_FieldData		Field ;
	

	//*************Carga de GlobalData

	Global.Reset() ;

	if ( ( Global.m_bReportNameLoaded = Item.InitLoad( m_poNodeRpr,"N_REPORT") ) ) { 
		Global.m_oReportName.StrCpy( Item.m_oValue.GetString() ) ;
	} ;

	if ( ( Global.m_bModeMetricLoaded = Item.InitLoad( m_poNodeRpr,"ID_UNIT_TYPE") ) ) { 
		Global.m_ModeMetric=(ENG_MODE_METRIC)(m4int16_t)Item.m_oValue.GetDouble() ;
	} ;

	if ( ( Global.m_bIdRootSectionLoaded = Item.InitLoad( m_poNodeRpr,"ID_INITIAL_SECTION") ) ) { 
		Global.m_iIdRootSection=Item.m_oValue.GetDouble() ;
	} ;

	if ( ( Global.m_bDataChnLoaded = Item.InitLoad( m_poNodeExe, "ID_DATA_T3") ) ) {
		//Para ejecuciones de Reports en canales de datos heredados, se
		// sobreescribe el nombre del canal sobre el que está diseñado
		if(m_oChannelID.StrLen()){
			Global.m_oDataChn.StrCpy(m_oChannelID);
		} else {
			Global.m_oDataChn.StrCpy(Item.m_oValue.GetString());
		};
	} ;

	if ( ( Global.m_bDataChnReprinterLoaded = Item.InitLoad( m_poNodeExe, "ID_DATA_T3_REPRINT") ) ) {
		Global.m_oDataChnReprinter.StrCpy( Item.m_oValue.GetString() ) ;
	} ;

	ItemKey.Init(  m_poNodeRprProp, "ID_PROPERTY") ; 
	ItemValue.Init( m_poNodeRprProp,"VALUE") ; 

	for ( m_poNodeRprProp->GoFirst(); !m_poNodeRprProp->IsEOF(); m_poNodeRprProp->Next() ) {

		if( !ItemKey.Load() || !ItemValue.Load() ) continue ;

		switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
		{
		case ROOT_SYNCHRONIZE:
			Global.m_bSwRootSyncLoaded = M4_TRUE ;
			Global.m_bSwRootSync = ItemValue.m_oValue.GetAnonymousBoolean() ;
			break ;

		case BREAK_MODE:
			Global.m_bBreakModeLoaded = M4_TRUE ;
			Global.m_BreakMode=(ClENG_Report::BREAK_MODE)(m4int16_t)ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case BREAK_SECTION:
			Global.m_bIdBreakSectionLoaded = M4_TRUE ;
			Global.m_iIdBreakSection = ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case TOTALIZE_COUNT:
			Global.m_bTotalizeListLoaded=M4_TRUE ;
			Global.m_oTotalizeList.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;
			break ;

		case CT_COUNT:
			Global.m_bCTListLoaded=M4_TRUE ;
			Global.m_oCTList.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;
			break ;

		case EXECUTION_PARS:			
			Global.m_bExecutionParsLoaded=M4_TRUE ;
			Global.m_oExecutionPars.StrCpy(ItemValue.m_oValue.GetAnonymousString())  ;
			break ;

		case IGNORE_TXT_WARNING:
			Global.m_bIgnoreTxtWarningLoaded=M4_TRUE ;
			Global.m_bIgnoreTxtWarning = ItemValue.m_oValue.GetAnonymousBoolean() ;
			break;

		case FILE_PATH:			
			// es una expresión a resolver en LoadDvc()
			Global.m_stDefaultFileParams.m_bIsFile = M4_FALSE ;
			Global.m_stDefaultFileParams.m_oPath.StrCpy(ItemValue.m_oValue.GetAnonymousString()) ;
			Global.m_stDefaultFileParams.m_bPathLoaded =   
				Global.m_stDefaultFileParams.m_bIsFileLoaded = M4_TRUE ;
			break ;

		case FILE_NAME:			
			// Expresión
			Global.m_stDefaultFileParams.m_bIsFile = M4_TRUE ;
			Global.m_stDefaultFileParams.m_oPath.StrCpy(ItemValue.m_oValue.GetAnonymousString()) ;
			Global.m_stDefaultFileParams.m_bPathLoaded =   
				Global.m_stDefaultFileParams.m_bIsFileLoaded = M4_TRUE ;
			
			break ;

		case FILE_NUMSECCHARS:			
			Global.m_stDefaultFileParams.m_iNumOfSecChars = ItemValue.m_oValue.GetAnonymousDouble() ;
			Global.m_stDefaultFileParams.m_bNumOfSecCharsLoaded = M4_TRUE ;
			break ;

		case FILE_NUMPAGECHARS:			
			Global.m_stDefaultFileParams.m_iNumOfPageChars = ItemValue.m_oValue.GetAnonymousDouble() ;
			Global.m_stDefaultFileParams.m_bNumOfPageCharsLoaded = M4_TRUE ;
			break ;

		case FILE_SEPARATOR:						
			Global.m_stDefaultFileParams.m_cSeparator = 
				*ItemValue.m_oValue.GetAnonymousString() ;
			Global.m_stDefaultFileParams.m_bSeparatorLoaded = M4_TRUE ;
			break ;

		case FILE_EXTENSION:			
			Global.m_stDefaultFileParams.m_oExtension.StrCpy(ItemValue.m_oValue.GetAnonymousString())  ;
			Global.m_stDefaultFileParams.m_bExtensionLoaded = M4_TRUE ;
			break ;

		case FILE_OFFSET:			
			Global.m_stDefaultFileParams.m_iOffSet = ItemValue.m_oValue.GetAnonymousDouble() ;
			Global.m_stDefaultFileParams.m_bOffSetLoaded = M4_TRUE ;
			break ;

		case USE_DEL_BREAK:
			Global.m_stDefaultFileParams.m_bUseDelBreak = ItemValue.m_oValue.GetAnonymousBoolean() ;
			Global.m_stDefaultFileParams.m_bUseDelBreakLoaded = M4_TRUE ;
			break ;

		case DEFAULT_DEL_BREAK:
			Global.m_stDefaultFileParams.m_bDefaultDelBreak = ItemValue.m_oValue.GetAnonymousBoolean() ;
			Global.m_stDefaultFileParams.m_bDefaultDelBreakLoaded = M4_TRUE ;
			break ;

		case CONSECUTIVE_BREAKS:
			Global.m_stDefaultFileParams.m_bConsecutiveBreaks = ItemValue.m_oValue.GetAnonymousBoolean() ;
			Global.m_stDefaultFileParams.m_bConsecutiveBreaksLoaded = M4_TRUE ;
			break ;
		} ;
	} ;

	LoadDefault( &Global.m_oReportDefaults, m_poNodeRpr, m_poNodeRprProp ) ;

	ao_poDD->SetGlobalData(&Global) ;

	//*************Carga de paleta


	ao_poDD->DeleteFGPalette() ;
	
	for ( m_poNodeColour->GoFirst(); ! m_poNodeColour->IsEOF(); m_poNodeColour->Next() ) {
		LoadColour(ao_poDD);
	} ;

	//*************Carga lista de Fonts

	ao_poDD->DeleteFontList() ;

	for ( m_poNodeFont->GoFirst(); ! m_poNodeFont->IsEOF(); m_poNodeFont->Next() ) {
		LoadFont(ao_poDD) ;
	} ;

	//*************Carga lista de Formatos

	ao_poDD->DeleteFormatList() ;

	for ( m_poNodeFormat->GoFirst(); ! m_poNodeFormat->IsEOF(); m_poNodeFormat->Next() ) {
		LoadFormat(ao_poDD) ;
	} ;

	//*************Carga PageData

	ao_poDD->DeletePageData() ;

	for ( B=0, m_poNodePage->GoFirst(); ! m_poNodePage->IsEOF(); m_poNodePage->Next(), ++B ) {

		Page.Reset() ;

		Page.m_bPageLoaded = M4_TRUE ;
		Page.m_iPage	   = B ;

		if ( (Page.m_bOffsetXLoaded = Item.InitLoad( m_poNodePage,"HORIZONTAL_OFFSET") ) ) {
			Page.m_dOffsetX = Item.m_oValue.GetDouble() ;	
		} ;

		if ( (Page.m_bOffsetYLoaded = Item.InitLoad( m_poNodePage,"VERTICAL_OFFSET") ) ) {
			Page.m_dOffsetY = Item.m_oValue.GetDouble() ;	
		} ;

		if ( (Page.m_bPageSizeLoaded = Item.InitLoad( m_poNodePage,"ID_PAPER_TYPE") ) ) {
			Page.m_PageSize = (ClENG_PagePrinter::PAPER_SIZE)(m4int16_t)Item.m_oValue.GetDouble() ;	
		} ;

		if ( (Page.m_bOrientationLoaded = Item.InitLoad( m_poNodePage,"ID_ORIENTATION_TP") ) ) {
			Page.m_Orientation = (ClENG_PagePrinter::ORIENTATION)(m4int16_t)Item.m_oValue.GetDouble() ;	
		} ;

		if ( (Page.m_bSizeXLoaded = Item.InitLoad( m_poNodePage,"WIDTH") ) ) {
			Page.m_dSizeX = Item.m_oValue.GetDouble() ;	
		} ;

		if ( (Page.m_bSizeYLoaded = Item.InitLoad( m_poNodePage,"HEIGHT_PAGE") ) ) {
			Page.m_dSizeY = Item.m_oValue.GetDouble() ;	
		} ;

		LoadDefault( & Page.m_stDefaultData, m_poNodePage, m_poNodePageProp ) ;

		//Datos page container

		Page.m_bCntLoaded = M4_TRUE ;
		Page.m_oCnt.Reset() ;
		Page.m_oCnt.m_bPageLoaded=M4_TRUE ;
		Page.m_oCnt.m_iPage = B ;
		LoadDefault( & Page.m_oCnt.m_stDefaultData, m_poNodePage, m_poNodePageProp ) ;

		//Fields de page container

		Page.m_oCnt.m_oFieldList.clear() ;
		for ( m_poNodePageCntFld->GoFirst(); ! m_poNodePageCntFld->IsEOF(); m_poNodePageCntFld->Next() ) {
			Field.Reset() ;
			LoadField( ao_poDD, &Field, m_poNodePageCntFld, m_poNodePageCntFldProp) ;
			Page.m_oCnt.m_oFieldList.push_back(Field) ;
		} ;

		ao_poDD->AddPage(&Page);
	} ;

	//*************Carga LE

	ao_poDD->DeleteLEList() ;

	ItemKey.Init( m_poNodeLongExpression, "ID_EXPRESSION" ) ; 
	ItemValue.Init( m_poNodeLongExpression, "EXPRESSION" ) ;

	for ( m_poNodeLongExpression->GoFirst(); ! m_poNodeLongExpression->IsEOF(); m_poNodeLongExpression->Next() ) {
		ao_poDD->AddLE(  
			ItemKey.Load()   ? ItemKey.m_oValue.GetString()		: "",
			ItemValue.Load() ? ItemValue.m_oValue.GetString()	: "") ;
	} ;

	//*************Carga Literales traducidos

	ao_poDD->DeleteTrLitList() ;

	ItemKey.Init( m_poReportString, "ID_STRING" ) ; 
	ItemValue.Init( m_poReportString, "ZVALUE" ) ;

	for ( m_poReportString->GoFirst(); ! m_poReportString->IsEOF(); m_poReportString->Next() ) {
		ao_poDD->AddTrLit(  
			ItemKey.Load()   ? ItemKey.m_oValue.GetString()		: "",
			ItemValue.Load() ? ItemValue.m_oValue.GetString()	: "") ;
	} ;

	//*************Reseteo CT List

	ao_poDD->DeleteCTList() ;

}

m4bool_t ClENG_DDLF_Chn::LoadFont(  ClENG_DesignData *ao_poDD, m4uint32_t ai_IdFont ) 
{
	ClMIT_ChnItem Item ;
	m4bool_t SwOk=M4_TRUE ;

	Item.Init(m_poNodeFont,"ID_FONT") ;
	if ( ! Item.FindDouble(ai_IdFont) ) {
		//Debe crear regsitro, o no tendra datos de Font
		m_poNodeFont->Insert() ;
		Item.m_oValue.InitDouble( ai_IdFont)  ;	//Mete PK
		Item.Save() ;
	} ;

	if ( Item.FindDouble(ai_IdFont) ) {
		LoadFont(ao_poDD) ;
	} else {
		SwOk=M4_FALSE ;
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Font %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_IdFont << SEND_MSG ;
	} ;
	return SwOk ;
}

m4bool_t ClENG_DDLF_Chn::LoadFormat( ClENG_DesignData *ao_poDD, m4uint32_t ai_IdFormat ) 
{
	ClMIT_ChnItem Item ;
	m4bool_t SwOk=M4_TRUE ;

	Item.Init(m_poNodeFormat,"ID_FORMAT") ;
	if ( Item.FindDouble(ai_IdFormat) ) {
		LoadFormat(ao_poDD) ;
	} else {
		SwOk=M4_FALSE ;
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Format %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_IdFormat << SEND_MSG ;
	} ;
	return SwOk ;
}


m4bool_t ClENG_DDLF_Chn::LoadColour(  ClENG_DesignData *ao_poDD, m4uint32_t ai_IdColour ) 
{
	ClMIT_ChnItem Item ;
	m4bool_t SwOk=M4_TRUE ;

	Item.Init(m_poNodeColour,"ID_COLOR") ;
	if ( ! Item.FindDouble(ai_IdColour) ) {
		//Debe crear regsitro, o no tendra datos de Font
		m_poNodeColour->Insert() ;
		Item.m_oValue.InitDouble(ai_IdColour)  ;	//Mete PK
		Item.Save() ;
	} ;

	if ( Item.FindDouble(ai_IdColour) ) {
		LoadColour(ao_poDD) ;
	} else {
		SwOk=M4_FALSE ;
		
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Colour %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_IdColour << SEND_MSG ;
	} ;
	return SwOk ;
}


void ClENG_DDLF_Chn::LoadFont(ClENG_DesignData *ao_poDD) 
{
	StENG_DD_FontData Font ;
	ClMIT_ChnItem Item ;

	Font.Reset() ;

	m_poNodeMasterFont->GoFirst() ;
	if ( m_poNodeMasterFont->IsEOF() ) {
		//Puede necesitar reload por insercion de regsitro en NodeFont
		m_poNodeMasterFont->ReLoad() ;
		m_poNodeMasterFont->GoFirst() ;
	} ;

	if ( (Font.m_bIdFontLoaded = Item.InitLoad( m_poNodeFont,"ID_FONT") ) ) {
		Font.m_uiIdFont= Item.m_oValue.GetDouble() ;	
	} ;

	if ( (Font.m_bDesignHeightLoaded = Item.InitLoad( m_poNodeMasterFont,"HEIGHT") ) ) {
		Font.m_DesignHeight= Item.m_oValue.GetDouble() ;	
	} ;

	if ( (Font.m_bFixedSpaceLoaded = Item.InitBooleanLoad( m_poNodeMasterFont,"IS_FIXED") ) ) {
		Font.m_bFixedSpace= Item.m_oValue.GetBoolean() ;	
	} ;

	if ( ( Font.m_bUnderlineLoaded = Item.InitBooleanLoad( m_poNodeMasterFont,"IS_UNDERLINED") ) ) {
		Font.m_bUnderline= Item.m_oValue.GetBoolean() ;	
	} ;

	if ( ( Font.m_bBoldLoaded = Item.InitBooleanLoad( m_poNodeMasterFont,"IS_BOLD") ) ) {
		Font.m_bBold= Item.m_oValue.GetBoolean() ;	
	} ;
	
	if ( ( Font.m_bItalicLoaded = Item.InitBooleanLoad( m_poNodeMasterFont,"IS_ITALIC") ) ) {
		Font.m_bItalic= Item.m_oValue.GetBoolean() ;	
	} ;

	if ( (Font.m_bRFNFileLoaded = Item.InitLoad( m_poNodeMasterFont,"RFN_NAME") ) ) {
		Font.m_oRFNFile.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;
		
	if ( (Font.m_bTraceNameLoaded = Item.InitLoad( m_poNodeMasterFont,"N_FONT") ) ) {
		Font.m_oTraceName.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	ao_poDD->AddFont(&Font);
}

void ClENG_DDLF_Chn::LoadFormat(ClENG_DesignData *ao_poDD) 
{
	StENG_DD_FormatData Format ;
	ClMIT_ChnItem Item ;
	StENG_DD_FormatData::StENG_DD_Popertie  FormatProp ;
	m4int16_t B ;

	Format.Reset() ;

	m_poNodeMasterFormat->GoFirst() ;
	if ( m_poNodeMasterFormat->IsEOF() ) {
		//Puede necesitar reload por insercion de registro en NodeFormat
		m_poNodeMasterFormat->ReLoad() ;
		m_poNodeMasterFormat->GoFirst() ;
	} ;

	if ( (Format.m_bIdFormatLoaded = Item.InitLoad( m_poNodeFormat,"ID_FORMAT") ) ) {
		Format.m_uiIdFormat = Item.m_oValue.GetDouble() ;	
	} ;

	if ( (Format.m_bFormatNameLoaded = Item.InitLoad( m_poNodeMasterFormat,"N_FORMAT") ) ) {
		Format.m_oFormatName.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (Format.m_bIdCurrencyLoaded = Item.InitLoad( m_poNodeFormat,"ID_CURRENCY") ) ) {
		Format.m_oIdCurrency.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	for ( B=0; B<ClENG_Format::MAX_NUM_PROPS; ++B ) {
		if ( Item.InitLoad(m_poNodeMasterFormat, m_oAuxFormat[B].m_pcItem) ) {
			FormatProp.m_oName.StrCpy(  m_oAuxFormat[B].m_pcProp ) ; 
			FormatProp.m_oValue.StrCpy( Item.m_oValue.GetString() ) ; 
			Format.m_oProperties.push_back( FormatProp ) ;
		} ;
	} ;

	ao_poDD->AddFormat(&Format);
}


void ClENG_DDLF_Chn::LoadColour(ClENG_DesignData *ao_poDD)
{
	StENG_DD_Colour Colour ;
	ClMIT_ChnItem Item ;

	Colour.Reset() ;

	m_poNodeMasterColour->GoFirst() ;
	if ( m_poNodeMasterColour->IsEOF() ) {
		//Puede necesitar reload por insercion de regsitro en NodeFont
		m_poNodeMasterColour->ReLoad() ;
		m_poNodeMasterColour->GoFirst() ;
	} ;

	if ( ( Colour.m_bIdColourLoaded = Item.InitLoad(m_poNodeMasterColour, "ID_COLOR") ) ) { 
		Colour.m_uiIdColour = Item.m_oValue.GetDouble() ;
	} ;
	if ( ( Colour.m_bNameLoaded = Item.InitLoad(m_poNodeMasterColour, "N_COLOR") ) ) { 
		Colour.m_oName.StrCpy( Item.m_oValue.GetString() ) ;
	} ;
	if ( ( Colour.m_b_R_Loaded = Item.InitLoad(m_poNodeMasterColour, "RED") ) ) { 
		Colour.m_R = Item.m_oValue.GetDouble() ;
	} ;
	if ( ( Colour.m_b_G_Loaded = Item.InitLoad(m_poNodeMasterColour, "GREEN") ) ) { 
		Colour.m_G = Item.m_oValue.GetDouble() ;
	} ;
	if ( ( Colour.m_b_B_Loaded = Item.InitLoad(m_poNodeMasterColour, "BLUE") ) ) { 
		Colour.m_B = Item.m_oValue.GetDouble() ;
	} ;

	ao_poDD->AddColour(&Colour);
}

void ClENG_DDLF_Chn::LoadAllCT(ClENG_DesignData *ao_poDD) 
{
	ClMIT_ChnItem Item ;

	ao_poDD->DeleteCTList() ;

	for ( m_poNodeCT->GoFirst(); !m_poNodeCT->IsEOF(); m_poNodeCT->Next() ) {
		if ( Item.InitLoad( m_poNodeCT,"ID_CROSSTAB") ) {
			LoadCT( ao_poDD,  Item.m_oValue.GetString() ) ;
		} ;
	} ;
}

m4bool_t ClENG_DDLF_Chn::LoadCT( ClENG_DesignData *ao_poDD, m4char_t *ai_pcCTId ) 
{
	ClMIT_ChnItem ItemKey, ItemValue, Item ;
	StENG_DD_CTData CTData ;
	m4bool_t SwOk=M4_TRUE ;
	StENG_DD_SectionData *SecData;
	StENG_DD_CTCell Cell;
	StENG_DD_SectionData::CntList_t::iterator CntIt ;
	StENG_DD_SectionData::SonList_t::iterator SonIt ;

	ENG_CT_CELL_TYPE CellType ;
	m4int16_t LevelCount ;

	//****Localizo el CT en el diseño

	Item.Init( m_poNodeCT,"ID_CROSSTAB") ;
	SwOk = M4_BOOL( SwOk && Item.FindIString(ai_pcCTId) ) ;
	if (!SwOk) goto exit ;

	//****Carga de datos básicos

	if ( (CTData.m_bCTIdLoaded= Item.InitLoad( m_poNodeCT,"ID_CROSSTAB") ) ) {
		CTData.m_oCTId.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (CTData.m_bNameLoaded = Item.InitLoad( m_poNodeCT,"N_CROSSTAB") ) ) {
		CTData.m_oName.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (CTData.m_bTotListLoaded= Item.InitLoad( m_poNodeCT,"TOT_LIST") ) ) {
		CTData.m_oTotList.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;
	
	if ( (CTData.m_bTreeListLoaded= Item.InitLoad( m_poNodeCT,"TREE_LIST") ) ) {
		CTData.m_oTreeList.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (CTData.m_bFillTreeLoaded= Item.InitLoad( m_poNodeCT,"SOURCE_KEY_LIST") ) ) {
		CTData.m_oFillTree.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (CTData.m_bIdParamSecLoaded= Item.InitLoad( m_poNodeCT,"INIT_PARAM_SECTION") ) ) {
		CTData.m_uiIdParamSec = (m4uint32_t)Item.m_oValue.GetDouble() ; 
	} ;

	if ( (CTData.m_bFirstCrossRowLoaded = Item.InitLoad( m_poNodeCT,"CROSS_ROW_FIRST") ) ) {
		CTData.m_iFirstCrossRow = (m4uint32_t)Item.m_oValue.GetDouble() ; 
	} ;

	//Obtiene informacion de si el registro ha sido modificado
	CTData.m_bIsModified = m_poNodeCT->IsRecordModified();

	//****Carga de propiedades

	ItemKey.Init(  m_poNodeCTProp, "ID_PROPERTY") ; 
	ItemValue.Init( m_poNodeCTProp,"VALUE") ; 

	for ( m_poNodeCTProp->GoFirst(); !m_poNodeCTProp->IsEOF(); m_poNodeCTProp->Next() ) {

		if( !ItemKey.Load() || !ItemValue.Load() ) continue ;

		switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
		{
		case CT_PROP_MERGE_PAGE:
			CTData.m_bMergePageLoaded = M4_TRUE ;
			CTData.m_MergePage=(ENG_CT_MERGE_PAGE)(m4int16_t)
				ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;
		case CT_PROP_XCOLUMN_LEFT:
			CTData.m_bXColumnLeftLoaded=M4_TRUE ;
			CTData.m_dXColumnLeft=ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;
		case CT_PROP_XCOLUMN_RIGHT:
			CTData.m_bXColumnRightLoaded=M4_TRUE ;
			CTData.m_dXColumnRight=ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;
		case CT_PROP_TOG_LEVEL:
			CTData.m_bTogLevelLoaded=M4_TRUE ;
			CTData.m_iTogLevel=ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;
		case CT_PROP_WIZARD_DATA:
			CTData.m_bWizardDataLoaded=M4_TRUE ;
			CTData.m_oWizardData.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;
			break ;
		//case CT_DERIVED_CT_LIST:
			//ao_pstData->m_oDerivedList.InitString( ItemValue.m_oValue.GetAnonymousString()) ;
		//	break ;
		} ;
	} ;

	//****Carga de plantilla

	//El dato IdParamSec es necesario

	if (!CTData.m_bIdParamSecLoaded) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//Busco sección raiz

	SecData = ao_poDD->FindSection(ENG_EXIST_REQUIRED,CTData.m_uiIdParamSec) ; 
	if (!SecData) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Bucle mientras haya secciones hijas anidadas; meter celdas

	for ( LevelCount=0; SecData; ++LevelCount ) {

		//Lista m_oParamSec

		CTData.m_oParamSec.push_back( SecData->m_iIdSection ) ;

		//Traducción Cnt --> CellType

		for ( CntIt=SecData->m_oCnt.begin(); CntIt != SecData->m_oCnt.end() ; ++CntIt ) { 
			switch ( (*CntIt).first ) {
			case ENG_CNT_BEFORE:
				CellType = ENG_CT_PRE_T  ;
				break ;
			case ENG_CNT_AFTER:
				CellType = ENG_CT_POST_T ;
				break ;
			case ENG_CNT_PRERECORD:
				CellType = ENG_CT_PRE_S ;
				break ;
			case ENG_CNT_POSTRECORD:
				CellType = ENG_CT_POST_S ;
				break ;
			case ENG_CNT_RECORD:
				CellType = ENG_CT_SUB ;
				break ;
			default:
				continue ;
				break ;
			} ;

			//Carga de Cell

			Cell.Reset() ;

			Cell.m_uiIdParamSec		= SecData->m_iIdSection  ;	Cell.m_bIdParamSecLoaded=M4_TRUE ;
			Cell.m_Type				= CellType ;				Cell.m_bTypeLoaded=M4_TRUE ;
			Cell.m_iLevel			= LevelCount ;				Cell.m_bLevelLoaded=M4_TRUE ;
			Cell.m_dHeight			= (*CntIt).second.m_dHeight ;
			Cell.m_bHeightLoaded	= (*CntIt).second.m_bHeightLoaded ;
			Cell.m_dWidth			= (*CntIt).second.m_dWidth ;
			Cell.m_bWidthLoaded		= (*CntIt).second.m_bWidthLoaded ;
			Cell.m_oFieldList		= (*CntIt).second.m_oFieldList ;
			Cell.m_bIsModified		= (*CntIt).second.m_bIsModified ;
			Cell.m_bRegionColourLoaded	= (*CntIt).second.m_bRegionColourLoaded  ;
			Cell.m_uiRegionColour		= (*CntIt).second.m_uiRegionColour  ;
			Cell.m_bRegionBorderLoaded = (*CntIt).second.m_bRegionBorderLoaded ;
			Cell.m_oRegionBorder.StrCpy( (*CntIt).second.m_oRegionBorder ) ;
			
			if((*CntIt).second.m_stDefaultData.m_SwAlignXLoaded) {
				Cell.m_bAlignParamCellXLoaded = (*CntIt).second.m_stDefaultData.m_SwAlignXLoaded;
				Cell.m_AlignParamCellX = (ENG_CT_CELL_X_ALIGN)(*CntIt).second.m_stDefaultData.m_AlignX;
			};

			if((*CntIt).second.m_stDefaultData.m_SwAlignYLoaded) {
				Cell.m_bAlignParamCellYLoaded = (*CntIt).second.m_stDefaultData.m_SwAlignYLoaded;
				Cell.m_AlignParamCellY = (ENG_CT_CELL_Y_ALIGN)(*CntIt).second.m_stDefaultData.m_AlignY;
			};

			//Se añade Cell

			CTData.m_oCellList.push_back(Cell) ;
		} ;

		//Hay que dejar en SecData siguiente Seccion ó NULL si no hay.

		SonIt = SecData->m_oSonList.begin() ;
		if ( SonIt==SecData->m_oSonList.end() ) {
			SecData=NULL ;
		} else {
			if ( ! (*SonIt).m_bIdSonSectionLoaded )  {
				SecData=NULL ;
			} else {
				SecData = ao_poDD->FindSection( ENG_TRY_FIND, (*SonIt).m_uiIdSonSection ) ; 
			} ;
		} ;
		

	} ;
	
	//****Añado CT
	
	ao_poDD->AddCT(&CTData) ;

exit:

	return SwOk ;
}

void ClENG_DDLF_Chn::LoadDefault( StENG_DD_Defaults *ao_poDef, 
	ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode ) 
{
	ClMIT_ChnItem Item ;
	ClMIT_ChnItem ItemKey ;
	ClMIT_ChnItem ItemValue ;

	ao_poDef->Reset() ;

	ItemKey.Init(  ai_poPropNode,"ID_PROPERTY") ; 
	ItemValue.Init( ai_poPropNode,"VALUE") ; 

	
	if (  (ao_poDef->m_SwIdFormatLoaded = Item.InitLoad(ai_poFieldNode,"ID_FORMAT") ) ) {
		ao_poDef->m_uiIdFormat=Item.m_oValue.GetDouble() ;
	} ;
	
	for (  ai_poPropNode->GoFirst(); ! ai_poPropNode->IsEOF();  ai_poPropNode->Next() ) {

		if( !ItemKey.Load() || !ItemValue.Load() ) continue ;

		switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
		{
		case DEFAULT_ID_FONT:
			ao_poDef->m_SwIdFontLoaded=M4_TRUE ;
			ao_poDef->m_uiIdFont=ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case DEFAULT_ID_ALIGNX:
			ao_poDef->m_SwAlignXLoaded=M4_TRUE ;
			ao_poDef->m_AlignX=(ClENG_FieldStr::ALIGN)(m4int16_t)ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case DEFAULT_ID_ALIGNY:
			ao_poDef->m_SwAlignYLoaded=M4_TRUE ;
			ao_poDef->m_AlignY=(ClENG_FieldStr::ALIGN)(m4int16_t)ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case DEFAULT_CLIP:
			ao_poDef->m_SwClipLoaded=M4_TRUE ;
			ao_poDef->m_bSwClip=ItemValue.m_oValue.GetAnonymousBoolean() ;
			break ;

		case DEFAULT_ID_COLOUR:
			ao_poDef->m_SwIdColourLoaded=M4_TRUE ;
			ao_poDef->m_uiIdColour=(m4uint32_t)ItemValue.m_oValue.GetAnonymousDouble() ;
			break ;

		case DEFAULT_GRAPH_PATH:
			ao_poDef->m_SwGraphicsLoaded=M4_TRUE ;
			ao_poDef->m_bSwGraphics=ItemValue.m_oValue.GetAnonymousBoolean() ;
			break ;

		case DEFAULT_DISPLAY_NULL:
			ao_poDef->m_SwDefDisplayNULLLoaded=M4_TRUE ;
			ao_poDef->m_oDefDisplayNULL.StrCpy(ItemValue.m_oValue.GetAnonymousString()) ;
			break ;
		case DEFAULT_FORMAT_NULL:
			ao_poDef->m_SwFormatNULLLoaded=M4_TRUE ;
			ao_poDef->m_bSwFormatNULL=ItemValue.m_oValue.GetAnonymousBoolean() ;
			break ;
		} ;
	} ;
}


void ClENG_DDLF_Chn::LoadAllSections( ClENG_DesignData *ao_poDD ) 
{
	ClMIT_ChnItem ItemKey, ItemValue, Item;
	StENG_DD_SectionData     Section ;
	StENG_DD_SonSectionData  SonSection ;
	StENG_DD_CntData		 Cnt ;
	ENG_TYPE_CONTAINER       CntType ;
	StENG_DD_FieldData		Field ;

	ao_poDD->DeleteSectionList() ;

	for ( m_poNodeSec->GoFirst(); ! m_poNodeSec->IsEOF(); m_poNodeSec->Next() ) {

		Section.Reset() ;

		//------------>Carga valores

		if( ( Section.m_bIdSectionLoaded =  Item.InitLoad(m_poNodeSec,"ID_SECTION") ) ) {
			Section.m_iIdSection = Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bSectionNameLoaded =  Item.InitLoad(m_poNodeSec,"N_SECTION") ) ) {
			Section.m_oSectionName.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bNoPrintEmptyLoaded = Item.InitLoad(m_poNodeSec,"NULL_IF_EMPTY") )) {
			Section.m_NoPrintEmpty = (ClENG_VR::NULL_IF_EMPTY)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bBeforePBreakLoaded = Item.InitLoad(m_poNodeSec,"ID_BEFORE_BRK_MOD") ) ) {
			Section.m_BeforePBreak = (ClENG_Section::BREAK_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bAfterPBreakLoaded =  Item.InitLoad(m_poNodeSec,"ID_AFTER_BRK_MOD") ) ) {
			Section.m_AfterPBreak = (ClENG_Section::BREAK_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bHeaderModeLoaded = Item.InitLoad(m_poNodeSec,"ID_HEAD_MODE_TYPE") ) ) {
			Section.m_HeaderMode = (ClENG_Section::HEADER_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bFooterModeLoaded = Item.InitLoad(m_poNodeSec,"ID_FOOT_MODE_TYPE") ) ) {
			Section.m_FooterMode = (ClENG_Section::FOOTER_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bGoTLoaded = Item.InitLoad(m_poNodeSec,"GO_THROUGH") ) ) {
			Section.m_GoT = (ClENG_VR::GO_THROUGH_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bRangeModeLoaded = Item.InitLoad(m_poNodeSec,"RANGE") ) ) {
			Section.m_RangeMode = (ClENG_VR::RANGE_MODE)(m4int16_t)Item.m_oValue.GetDouble() ;  
		} ;

		if( ( Section.m_bGroupKeyLoaded =  Item.InitLoad(m_poNodeSec,"GROUP") ) ) {
			Section.m_oGroupKey.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bNextRecNodeLoaded =  Item.InitLoad(m_poNodeSec,"ID_NODE") ) ) {
			Section.m_RecordsetNodeName.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bSwBreakVRLoaded = Item.InitBooleanLoad(m_poNodeSec,"IS_BREAK_RECORD") ) ) {
			Section.m_bSwBreakVR = Item.m_oValue.GetBoolean() ;  
		} ;

		if( ( Section.m_bCancelSectionLoaded = Item.InitLoad(m_poNodeSec,"CANCEL_SECTION") ) ) {
			Section.m_oCancelSection.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bResetBeforeTotalLoaded = Item.InitLoad(m_poNodeSec,"RESET_BEF_TOTALS") ) ) {
			Section.m_oResetBeforeTotal.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bPrepareRecordsetLoaded = Item.InitLoad(m_poNodeSec,"PREPARE_RECORDSET") ) ) {
			Section.m_oPrepareRecordset.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bResetHeaderTotalsLoaded = Item.InitLoad(m_poNodeSec,"RESET_HEA_TOTALS") ) ) {
			Section.m_oResetHeaderTotals.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bCancelVRLoaded = Item.InitLoad(m_poNodeSec,"CANCEL_RECORD") ) ) {
			Section.m_oCancelVR.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		if( ( Section.m_bSetPostVRTotalLoaded = Item.InitLoad(m_poNodeSec,"SET_TOTALS") ) ) {
			Section.m_oSetPostVRTotal.StrCpy( Item.m_oValue.GetString() ) ;  
		} ;

		//Comprueba si el registro actual ha sido midificado
		Section.m_bIsModified = m_poNodeSec->IsRecordModified();

		LoadDefault( & Section.m_stDefaultData, m_poNodeSec, m_poNodeSecProp ) ;

		//------------>Carga properties

		ItemKey.Init(  m_poNodeSecProp, "ID_PROPERTY") ; 
		ItemValue.Init( m_poNodeSecProp,"VALUE") ; 

		for ( m_poNodeSecProp->GoFirst(); !m_poNodeSecProp->IsEOF(); m_poNodeSecProp->Next() ) {

			if( !ItemKey.Load() || !ItemValue.Load() ) continue ;
	
			switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
			{

			case SEC_TOGETHER:
				Section.m_bTogetherLoaded=M4_TRUE ;
				Section.m_oTogether.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
				break ;

			case SEC_SLC_GUIDE_ITEM:
				Section.m_bGuideItemNameLoaded = M4_TRUE ;
				Section.m_GuideItemName.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
				break ;

			case SEC_NULL_ON_SONS:
				Section.m_bNullOnSonsLoaded = M4_TRUE ;
				Section.m_bNullOnSons = ItemValue.m_oValue.GetAnonymousBoolean() ;	
				break ;

			case SEC_ONE_ONE:
				Section.m_bOneOneStrLoaded = M4_TRUE ;
				Section.m_oOneOneStr.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
				break ;

			} ;

		} ;

		//------------>Carga SonSections

		Section.m_oSonList.clear() ;

		for ( m_poNodeSecSon->GoFirst(); ! m_poNodeSecSon->IsEOF(); m_poNodeSecSon->Next() ) {

			SonSection.Reset() ;

			if ( (SonSection.m_bIdSonSectionLoaded = Item.InitLoad(m_poNodeSecSon,"ID_CHILD_SECTION") ) ) {
				SonSection.m_uiIdSonSection = Item.m_oValue.GetDouble() ;
			} ;

			if ( (SonSection.m_bExeOrderLoaded = Item.InitLoad(m_poNodeSecSon,"EXE_ORDER") ) ) {
				SonSection.m_iExeOrder = Item.m_oValue.GetDouble() ;
			} ;

			if ( (SonSection.m_bIsSyncLoaded = Item.InitBooleanLoad(m_poNodeSecSon,"IS_SYNCHRONIZED") ) ) {
				SonSection.m_bIsSync = Item.m_oValue.GetBoolean() ;
			}  ;

			Section.m_oSonList.push_back(SonSection);
		} ;


		//------------>Carga containers

		Section.m_oCnt.clear() ;

		for ( m_poNodeSecCnt->GoFirst(); ! m_poNodeSecCnt->IsEOF(); m_poNodeSecCnt->Next() ) {

			Cnt.Reset() ;

			if ( Item.InitLoad(m_poNodeSecCnt,"ID_CONTAINER") ) {
				CntType = (ENG_TYPE_CONTAINER)(m4int16_t)Item.m_oValue.GetDouble() ;
			} else {
				continue ;	//Ignoramos el container
			} ;

			// Carga los datos

			if ( ( Cnt.m_bPageLoaded = Item.InitLoad(m_poNodeSecCnt,"ID_PAGE") ) ) {
				Cnt.m_iPage = Item.m_oValue.GetDouble()-1 ;
			} ;

			if ( ( Cnt.m_bTopLoaded = Item.InitLoad(m_poNodeSecCnt,"TOP") ) ) {
				Cnt.m_dTop = Item.m_oValue.GetDouble() ;
			} ;

			if ( ( Cnt.m_bLeftLoaded = Item.InitLoad(m_poNodeSecCnt,"LEFT") ) ) {
				Cnt.m_dLeft = Item.m_oValue.GetDouble() ;
			} ;

			if ( ( Cnt.m_bHeightLoaded = Item.InitLoad(m_poNodeSecCnt,"HEIGHT") ) ) {
				Cnt.m_dHeight = Item.m_oValue.GetDouble() ;
			} ;

			if ( ( Cnt.m_bWidthLoaded = Item.InitLoad(m_poNodeSecCnt,"WIDTH") ) ) {
				Cnt.m_dWidth = Item.m_oValue.GetDouble() ;
			} ;

			if ( ( Cnt.m_bIsFixedLoaded = Item.InitBooleanLoad(m_poNodeSecCnt,"IS_FIXED") ) ) {
				Cnt.m_IsFixed = Item.m_oValue.GetBoolean() ;
			} ;

			if ( ( Cnt.m_bSwTogetherLoaded = Item.InitBooleanLoad(m_poNodeSecCnt,"KEEP_CONTAIN_TOGH") ) ) {
				Cnt.m_bSwTogether = Item.m_oValue.GetBoolean() ;
			} ;

			if ( ( Cnt.m_bCancelCntLoaded = Item.InitLoad(m_poNodeSecCnt,"CANCEL_CONTAINER") ) ) {
				Cnt.m_oCancelCnt.StrCpy( Item.m_oValue.GetString() )  ;
			} ;

			//Obtiene informacion de si el registro ha sido modificado
			Cnt.m_bIsModified = m_poNodeSecCnt->IsRecordModified();

			LoadDefault( & Cnt.m_stDefaultData, m_poNodeSecCnt, m_poNodeSecCntProp ) ;

			ItemKey.Init( m_poNodeSecCntProp,"ID_PROPERTY") ; 
			ItemValue.Init( m_poNodeSecCntProp,"VALUE" ) ;

			for ( m_poNodeSecCntProp->GoFirst(); !m_poNodeSecCntProp->IsEOF(); m_poNodeSecCntProp->Next() ) {

				if( !ItemKey.Load() || !ItemValue.Load() ) continue ;

				switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
				{
				case CNT_LOWER_LIMIT:
					Cnt.m_bLowerLimitLoaded = M4_TRUE ;
					Cnt.m_dLowerLimit = ItemValue.m_oValue.GetAnonymousDouble() ;	
					break ;
				case CNT_CT_MERGE:
					Cnt.m_bCTMergeLoaded = M4_TRUE ;
					Cnt.m_oCTMerge.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
					break ;
				case CNT_REGION_BORDER:
					Cnt.m_bRegionBorderLoaded  = M4_TRUE ;
					Cnt.m_oRegionBorder.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
					break ;
				case CNT_REGION_COLOUR:
					Cnt.m_bRegionColourLoaded = M4_TRUE ;
					Cnt.m_uiRegionColour = ItemValue.m_oValue.GetAnonymousDouble() ;	
					break ;
				} ;
			} ;

			//Carga fields del container

			Cnt.m_oFieldList.clear() ;
			for ( m_poNodeSecCntFld->GoFirst(); ! m_poNodeSecCntFld->IsEOF(); m_poNodeSecCntFld->Next() ) {
				Field.Reset() ;
				LoadField( ao_poDD, &Field, m_poNodeSecCntFld, m_poNodeSecCntFldProp) ;
				Cnt.m_oFieldList.push_back(Field) ;
			} ;
			
			Section.m_oCnt.insert( StENG_DD_SectionData::CntList_t::value_type(CntType,Cnt));

		} ;


		//------------>Uf! Por fin, añade sección

		ao_poDD->AddSection(&Section);
	} ;
}


void ClENG_DDLF_Chn::LoadField( 
		ClENG_DesignData *ao_poDD,
		StENG_DD_FieldData *ao_pstData, 
		ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode) 
{
	ClMIT_ChnItem Item, ItemKey, ItemValue ;

	//Carga de datos

	if ( ( ao_pstData->m_bIdFieldLoaded =  Item.InitLoad( ai_poFieldNode,"ID_FIELD") ) ) {
		ao_pstData->m_uiIdField= Item.m_oValue.GetDouble() ;	
	} ;

	if ( ( ao_pstData->m_bFieldNameLoaded = Item.InitLoad( ai_poFieldNode,"N_FIELD") ) ) {
		ao_pstData->m_oFieldName.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( ( ao_pstData->m_bFldTypeLoaded = Item.InitLoad( ai_poFieldNode,"ID_FIELD_TYPE") ) ) {
		ao_pstData->m_FldType = (ENG_FIELD_TYPE)(m4int16_t)Item.m_oValue.GetDouble() ;
	} ;

	if ( ( ao_pstData->m_bTopLoaded = Item.InitLoad( ai_poFieldNode,"TOP") ) ) {
		ao_pstData->m_dTop = Item.m_oValue.GetDouble() ;
	} ;

	if ( ( ao_pstData->m_bLeftLoaded = Item.InitLoad( ai_poFieldNode,"LEFT") ) ) {
		ao_pstData->m_dLeft = Item.m_oValue.GetDouble() ;
	} ;

	if ( ( ao_pstData->m_bWidthLoaded=Item.InitLoad( ai_poFieldNode,"WIDTH") ) ) {
		ao_pstData->m_dWidth = Item.m_oValue.GetDouble() ;
	} ;

	if ( ( ao_pstData->m_bHeightLoaded = Item.InitLoad( ai_poFieldNode,"HEIGHT") ) ) {
		ao_pstData->m_dHeight = Item.m_oValue.GetDouble() ;
	} ;

	if ( ( ao_pstData->m_bExprLoaded = Item.InitLoad( ai_poFieldNode,"VALUE") ) ) {
		ao_pstData->m_oExpr.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	if ( (ao_pstData->m_bLiteralValueLoaded = Item.InitLoad( ai_poFieldNode,"LITERAL_VALUE") ) ) {
		ao_pstData->m_oLiteralValue.StrCpy( Item.m_oValue.GetString() ) ;	
	} ;

	//Obtiene informacion de si el registro ha sido modificado
	ao_pstData->m_bIsModified = ai_poFieldNode->IsRecordModified();

	LoadDefault( & ao_pstData->m_stDefaultData, ai_poFieldNode, ai_poPropNode ) ;
		
	ItemKey.Init(  ai_poPropNode, "ID_PROPERTY") ; 
	ItemValue.Init( ai_poPropNode,"VALUE") ; 

	for ( ai_poPropNode->GoFirst(); !ai_poPropNode->IsEOF(); ai_poPropNode->Next() ) {

		if( !ItemKey.Load() || !ItemValue.Load() ) continue ;

		switch( (m4int16_t)ItemKey.m_oValue.GetDouble() ) 
		{
		case FLD_SAVE_NODE:
			ao_pstData->m_bSaveNodeLoaded= M4_TRUE ;
			ao_pstData->m_oSaveNode.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
			break ;

		case FLD_SAVE_ITEM:
			ao_pstData->m_bSaveItemNameLoaded= M4_TRUE ;
			ao_pstData->m_oSaveItemName.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
			break ;

		case FLD_DESIGN_OPTIMIZE:
			ao_pstData->m_bSwOptimizeLoaded= M4_TRUE ;
			ao_pstData->m_bSwOptimize = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_DESIGN_STATIC:
			ao_pstData->m_bSwStaticLoaded= M4_TRUE ;
			ao_pstData->m_bSwStatic = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_MULTIL_INTERL:
			ao_pstData->m_bInterlineLoaded= M4_TRUE ;
			ao_pstData->m_dInterline = ItemValue.m_oValue.GetAnonymousDouble() ;	
			break ;

		case FLD_MULTIL_VAR_HEIGHT:
			ao_pstData->m_bIsVarHeightLoaded= M4_TRUE ;
			ao_pstData->m_bIsVarHeight = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_MULTIL_JUSTIFIED:
			ao_pstData->m_bIsJustifiedLoaded= M4_TRUE ;
			ao_pstData->m_bIsJustified = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_BARCODE_TYPE:
			ao_pstData->m_bBarCodeTypeLoaded= M4_TRUE ;
			ao_pstData->m_BarCodeType = (ENG_BAR_CODE_TYPE)(m4int16_t)ItemValue.m_oValue.GetAnonymousDouble() ;
			break;

		case FLD_MAX_BOXES:
			ao_pstData->m_bMaxBoxesLoaded = M4_TRUE ;
			ao_pstData->m_iMaxBoxes = ItemValue.m_oValue.GetAnonymousDouble() ;	
			break ;

		case FLD_FILL_MODE:
			ao_pstData->m_bFillModeLoaded = M4_TRUE ;
			ao_pstData->m_FillMode = (ClENG_FieldStr::FILL_MODE)(m4int16_t)ItemValue.m_oValue.GetAnonymousDouble() ;	
			break ;

		case FLD_FILL_CHAR:
			ao_pstData->m_bFillCharLoaded = M4_TRUE ;
			ao_pstData->m_cFillChar = *ItemValue.m_oValue.GetAnonymousString() ;	
			break ;

		case FLD_DUPLICATED:
			ao_pstData->m_bSwDuplicatedLoaded = M4_TRUE ;
			ao_pstData->m_bSwDuplicated = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_ID_CT_FIELD:
			ao_pstData->m_bIdCtFieldLoaded = M4_TRUE ;
			ao_pstData->m_oIdCtField.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
			break ;

		case FLD_ID_PI:
			ao_pstData->m_bIdPiLoaded = M4_TRUE ;
			ao_pstData->m_oIdPi.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
			break ;

		case FLD_REGION_BORDER:
			ao_pstData->m_bRegionBorderLoaded  = M4_TRUE ;
			ao_pstData->m_oRegionBorder.StrCpy( ItemValue.m_oValue.GetAnonymousString() ) ;	
			break ;

		case FLD_REGION_COLOUR:
			ao_pstData-> m_bRegionColourLoaded = M4_TRUE ;
			ao_pstData->m_uiRegionColour = ItemValue.m_oValue.GetAnonymousDouble() ;	
			break ;

		case FLD_INDEX_LEVEL:
			ao_pstData->m_bIndexLevelLoaded = M4_TRUE;
			ao_pstData->m_iIndexLevel = ItemValue.m_oValue.GetAnonymousDouble();	
			break;

		case FLD_INDEX_EXPR:
			ao_pstData->m_bIndexExprLoaded = M4_TRUE;
			ao_pstData->m_oIndexExpr.StrCpy(ItemValue.m_oValue.GetAnonymousString());
			break;

		case FLD_HYPERLINK:
			ao_pstData->m_bHyperlinkLoaded = M4_TRUE;
			ao_pstData->m_oHyperlink.StrCpy(ItemValue.m_oValue.GetAnonymousString());
			break;

		case FLD_RASTER_TEMP_WEB_IMAGE:
			ao_pstData->m_bTempWebImageLoaded = M4_TRUE ;
			ao_pstData->m_bTempWebImage = ItemValue.m_oValue.GetAnonymousBoolean() ;	
			break ;

		case FLD_LINE_ADJUST_TO_CONTAINER:
			ao_pstData->m_bAdjustLineToContainerLoaded = M4_TRUE;
			ao_pstData->m_bAdjustLineToContainer = ItemValue.m_oValue.GetAnonymousBoolean() ;
			break;
		} ;
	} ;

	//Gestión posibles item BLOB

	if ( ao_pstData->m_FldType == ENG_FLD_RASTERG ) {

		ao_pstData->m_bEmbebedRasterLoaded=M4_TRUE ;
		ao_pstData->m_bEmbebedRaster=M4_FALSE ;

		if ( ( ao_pstData->m_bColourRasterLoaded = Item.InitLoad( ai_poFieldNode,"BMP_COLOR") ) ) {
			ao_pstData->m_bEmbebedRaster=M4_TRUE ;
			ao_pstData->m_oColourRaster.StrCpy(Item.m_oValue.GetString()) ;
			ao_poDD->DeclareInterest(ao_pstData->m_oColourRaster) ;
		} ;

		if ( ( ao_pstData->m_bBNRasterLoaded = Item.InitLoad( ai_poFieldNode,"BMP_BN") ) ) {
			ao_pstData->m_bEmbebedRaster=M4_TRUE ;
			ao_pstData->m_oBNRaster.StrCpy(Item.m_oValue.GetString()) ;
			ao_poDD->DeclareInterest(ao_pstData->m_oBNRaster) ;
		} ;
			
	} ;

	//Trampeo de X1,Y1,X2,Y2 para FLD_LINE

	if ( ao_pstData->m_bFldTypeLoaded && ao_pstData->m_FldType==ENG_FLD_LINE ) {

		//Top,Left, etc, son X1,etc, y son obligatorios. Por ahora, NUNCA se mandan en caracteres

		ao_pstData->m_dX1			= ao_pstData->m_dLeft ; 
		ao_pstData->m_bX1Loaded		= ao_pstData->m_bLeftLoaded ; 
		ao_pstData->m_dY1			= ao_pstData->m_dTop ; 
		ao_pstData->m_bY1Loaded		= ao_pstData->m_bTopLoaded ; 
		ao_pstData->m_dY2			= ao_pstData->m_dWidth ; 
		ao_pstData->m_bY2Loaded		= ao_pstData->m_bWidthLoaded ; 
		ao_pstData->m_dX2			= ao_pstData->m_dHeight ; 
		ao_pstData->m_bX2Loaded		= ao_pstData->m_bHeightLoaded ; 

		//Calcula Left, Top, Width y Height en funcion de la linea

		ao_pstData->m_dLeft= M4_MIN(ao_pstData->m_dX1,ao_pstData->m_dX2) ;  
		ao_pstData->m_bLeftLoaded = M4_TRUE ;
		ao_pstData->m_dTop= M4_MIN(ao_pstData->m_dY1,ao_pstData->m_dY2) ;  
		ao_pstData->m_bTopLoaded = M4_TRUE ;
		ao_pstData->m_dWidth= abs((int)(ao_pstData->m_dX2 - ao_pstData->m_dX1)) ;  
		ao_pstData->m_bWidthLoaded = M4_TRUE ;
		ao_pstData->m_dHeight= abs((int)(ao_pstData->m_dY2 - ao_pstData->m_dY1)) ;  
		ao_pstData->m_bHeightLoaded = M4_TRUE ;
	} ;

}

void ClENG_DDLF_Chn::LoadAll(ClENG_DesignData *ao_poDD) 
{
	LoadBaseData(		ao_poDD ) ;
	LoadAllSections(	ao_poDD ) ;
	LoadAllCT(			ao_poDD ) ;
}


//================================= EXTRACCION DE LITERALES ======================

void ClENG_DDLF_Chn::ExtractString(const m4char_t* ai_pcString, ClMIT_Str *ai_poModifiedStr, const m4char_t* ai_pcIDReport)
{
	m4pcchar_t InitPos;
	m4pcchar_t EndPos;
	ClMIT_Str AuxString, AuxStr2, ID_String;

	InitPos=strchr(ai_pcString, '"');
	if(InitPos){
		ai_poModifiedStr->ToASCIZ(ai_pcString, InitPos-ai_pcString);
	};

	while(InitPos) {
		EndPos=strchr(InitPos+1,'"');
		AuxString.ToASCIZ(InitPos+1, (EndPos-InitPos)-1);
		StoreExtractedString((m4char_t*)AuxString, &ID_String, ai_pcIDReport);
		ai_poModifiedStr->StrCat("%GetString.STR_");
		ai_poModifiedStr->StrCat((m4char_t*)ID_String);
		ai_poModifiedStr->StrCat(" /*");
		ai_poModifiedStr->StrCat(AuxString.InChar(0));
		ai_poModifiedStr->StrCat("*/ ");
		InitPos=strchr(EndPos+1, '"');
		if(InitPos){
			AuxStr2.ToASCIZ(EndPos+1, InitPos-EndPos);
			ai_poModifiedStr->StrCat(AuxStr2);
		} else {
			ai_poModifiedStr->StrCat(EndPos+1);
		};
	};
};

void ClENG_DDLF_Chn::StoreExtractedString(const m4char_t *ai_pcString, ClMIT_Str* ao_poID, const m4char_t* ai_pcIDReport)
{
	m4uint32_t Record, Key;
	ClMIT_ChnItem Item;
	m4char_t buffer[8];
	
	Record = m_poReportString->GetRealCount();
	Key = Record;
	m_poReportString->MoveTo(Record);

	Item.Init(m_poReportString,"ID_STRING");

	sprintf(buffer, "%i", Key);
	while(Item.FindString(buffer)) {
		Key+=1;
		sprintf(buffer, "%i", Key);
	};
 
	//Insercion de datos
	m_poReportString->Insert();
	
	//Save del ID_STRING
	Item.m_oValue.InitString(buffer);
	Item.Save();

	//Cadena de retorno con el ID
	ao_poID->StrCpy(buffer);
	

	//Save del N_STRING
	Item.Init(m_poReportString,"N_STRING");
	Item.m_oValue.InitString(ai_pcString);
	Item.Save();			   
	
	//Save del ZVALUE
	Item.Init(m_poReportString,"VALUE");
	Item.m_oValue.InitString(ai_pcString);
	Item.Save();			   

	//Save del ID_REPORT
	Item.Init(m_poReportString,"ID_REPORT");
	Item.m_oValue.InitString(ai_pcIDReport);
	Item.Save();			   
}


m4bool_t ClENG_DDLF_Chn::ExtractLiterals()
{
	m4int16_t PushState=m_poInsp->m_poTrace->GetPushState();
	m4bool_t SwOk=M4_TRUE;
	
    #ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DDLF_Chn::ExtractLiterals", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && ExtractReportProps() );
	SwOk = M4_BOOL( SwOk && ExtractLongExpressions() );
	SwOk = M4_BOOL( SwOk && ExtractContainerProps() );
	SwOk = M4_BOOL( SwOk && ExtractPageCntFldStrings() );
	SwOk = M4_BOOL( SwOk && ExtractCntFldStrings() );

    #ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DDLF_Chn::ExtractLiterals", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState);

	return SwOk;
};

m4bool_t ClENG_DDLF_Chn::ExtractCntFldStrings()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, IDReport;
	m4bool_t SwOk=M4_TRUE;

	//Se posiciona en el primero
	for(m_poNodeSec->GoFirst(); !m_poNodeSec->IsEOF(); m_poNodeSec->Next()) {
		for(m_poNodeSecCnt->GoFirst(); !m_poNodeSecCnt->IsEOF(); m_poNodeSecCnt->Next()) {
			for(m_poNodeSecCntFld->GoFirst(); !m_poNodeSecCntFld->IsEOF(); m_poNodeSecCntFld->Next()) {
				if (Item.InitLoad(m_poNodeSecCntFld,"VALUE") ) {
					if (IDReport.InitLoad(m_poNodeSecCntFld,"ID_REPORT")) {
						ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
						Item.m_oValue.InitString(ModifiedStr);
						Item.Save();
					} else {
						SwOk=M4_FALSE;
						goto exit;
					};
				} else{
					SwOk=M4_FALSE;
					goto exit;
				};
			};
		};
	};

exit:

	return SwOk;
}

m4bool_t ClENG_DDLF_Chn::ExtractReportProps()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, Item2, IDReport;
	m4bool_t SwOk = M4_TRUE;

	for(m_poNodeRprProp->GoFirst(); !m_poNodeRprProp->IsEOF(); m_poNodeRprProp->Next()) {
		if(Item.InitLoad(m_poNodeRprProp, "ID_PROPERTY")){
			if(Item.m_oValue.GetDouble()==FILE_NAME || Item.m_oValue.GetDouble()==FILE_PATH){
				if(Item2.InitLoad(m_poNodeRprProp, "VALUE")){
					if (IDReport.InitLoad(m_poNodeRprProp,"ID_REPORT")) {
						ExtractString(Item2.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
						Item2.m_oValue.InitString(ModifiedStr);
						Item2.Save();
					} else {
						SwOk=M4_FALSE;
						goto exit;
					};
				} else {
					SwOk=M4_FALSE;
					goto exit;
				};
			} ;
		} else {
			SwOk=M4_FALSE;
			goto exit;
		};
	};

exit:

	return SwOk;
}


m4bool_t ClENG_DDLF_Chn::ExtractContainerProps()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, IDReport;
	m4bool_t SwOk = M4_TRUE;

	//Se posiciona en el primero
	for(m_poNodeSec->GoFirst(); !m_poNodeSec->IsEOF(); m_poNodeSec->Next()) {
		for(m_poNodeSecCnt->GoFirst(); !m_poNodeSecCnt->IsEOF(); m_poNodeSecCnt->Next()) {
			if (Item.InitLoad(m_poNodeSecCnt,"CANCEL_CONTAINER") ) {
				if (IDReport.InitLoad(m_poNodeSecCnt,"ID_REPORT")) {
					ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
					Item.m_oValue.InitString(ModifiedStr);
					Item.Save();
				} else {
					SwOk=M4_FALSE;
					goto exit;
				};
			};
		};
	};	

exit:
	 return SwOk;
}

m4bool_t ClENG_DDLF_Chn::ExtractSectionProps()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, IDReport;
	m4bool_t SwOk = M4_TRUE;

	//Se posiciona en el primero
	for(m_poNodeSec->GoFirst(); !m_poNodeSec->IsEOF(); m_poNodeSec->Next()) {
		if( Item.InitLoad(m_poNodeSec,"CANCEL_SECTION") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};

		if( Item.InitLoad(m_poNodeSec,"RESET_BEF_TOTALS") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};

		if( Item.InitLoad(m_poNodeSec,"PREPARE_RECORDSET") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};

		if( Item.InitLoad(m_poNodeSec,"RESET_HEA_TOTALS") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		} ;

		if( Item.InitLoad(m_poNodeSec,"CANCEL_RECORD") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};

		if( Item.InitLoad(m_poNodeSec,"SET_TOTALS") ) {
			if (IDReport.InitLoad(m_poNodeSec,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};
	};	

exit: 
	return SwOk;
}


m4bool_t ClENG_DDLF_Chn::ExtractLongExpressions()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, IDReport;
	m4bool_t SwOk = M4_TRUE;

	for(m_poNodeLongExpression->GoFirst(); !m_poNodeLongExpression->IsEOF(); m_poNodeLongExpression->Next()) {
		if(Item.InitLoad(m_poNodeLongExpression, "EXPRESSION")){
			if (IDReport.InitLoad(m_poNodeLongExpression,"ID_REPORT")) {
				ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
				Item.m_oValue.InitString(ModifiedStr);
				Item.Save();
			} else {
				SwOk=M4_FALSE;
				goto exit;
			};
		};
	};

exit: 
	return SwOk;
}

m4bool_t ClENG_DDLF_Chn::ExtractPageCntFldStrings()
{
	ClMIT_Str ModifiedStr;
	ClMIT_ChnItem Item, IDReport;
	m4bool_t SwOk = M4_TRUE;

	for(m_poNodePage->GoFirst(); !m_poNodePage->IsEOF(); m_poNodePage->Next()) {
		for(m_poNodePageCntFld->GoFirst(); !m_poNodePageCntFld->IsEOF(); m_poNodePageCntFld->Next()) {
			if(Item.InitLoad(m_poNodePageCntFld, "VALUE")){
				if (IDReport.InitLoad(m_poNodePageCntFld,"ID_REPORT")) {
					ExtractString(Item.m_oValue.GetString(), &ModifiedStr, IDReport.m_oValue.GetString());
					Item.m_oValue.InitString(ModifiedStr);
					Item.Save();
				} else {
					SwOk=M4_FALSE;
					goto exit;
				};
			};
		};
	};

exit: 
	return SwOk;
}

//======== FIN DE EXTRACCION DE LITERALES =================================

void ClENG_DDLF_Chn::SynchronizeProperties( ClMIT_ChnNode *ai_poPropNode, PropertieList_t *ai_poList, m4bool_t SwCT ) 
{
	PropertieList_t::iterator It ;
	ClMIT_ChnItem ItemKey, ItemValue ;

	ItemKey.Init(   ai_poPropNode, "ID_PROPERTY") ; 
	ItemValue.Init( ai_poPropNode, "VALUE") ; 

	//Borrar de canal las que no existan en DD

	for ( ai_poPropNode->GoFirst(); !ai_poPropNode->IsEOF();  ) {

		if( !ItemKey.Load() ) continue ;
		
		It = ai_poList->find( ItemKey.m_oValue.GetDouble() ) ;

		if ( It==ai_poList->end() && (SwCT && (CT_ROW_PROPS>(ItemKey.m_oValue.GetDouble()) || (ItemKey.m_oValue.GetDouble())>CT_LIMIT)) ) {
			ai_poPropNode->Delete() ;	//ya nos deja en el Next
		} else {
			if (SwCT && (CT_ROW_PROPS>(ItemKey.m_oValue.GetDouble()) || (ItemKey.m_oValue.GetDouble())>CT_LIMIT) ) {
				ai_poPropNode->Delete() ;
			} else {
				ai_poPropNode->Next() ;
			};
		} ;
	} ;

	for ( It=ai_poList->begin(); It!=ai_poList->end(); ++It ) {

		ItemKey.FindDouble( (*It).first ) ;

		if ( ai_poPropNode->IsEOF() ) {
			//Si no exite inserta y nos deja ahi. Inserta clave
			//Si existe, ya estamos posicionados en el registro correcto a modificar.
			ai_poPropNode->Insert() ;	
			ItemKey.m_oValue.InitDouble( (*It).first ) ;
			ItemKey.Save() ;
		}  ;
		// Aqui ya exite, con clave actualizada. Grabar contenido.

		ItemValue.m_oValue.InitString( (*It).second ) ;
		ItemValue.Save() ;
	} ;

}

m4bool_t ClENG_DDLF_Chn::SynchronizeCT( m4char_t *ai_pcCT, ClENG_DesignData *ao_poDD ) 
{
	m4bool_t SwOk=M4_TRUE;	
	StENG_DD_CTData *CT=NULL ;
	ClMIT_ChnItem Item ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_DDLF_Chn::SynchronizeCT(%0:s)", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCT << SEND_MSG ;
	};
	#endif

	//------>Datos generales CT

	//Busca CT Data

	CT=ao_poDD->FindCT(ENG_EXIST_REQUIRED,ai_pcCT) ;
	if (!CT) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Pos en canal

	Item.Init( m_poNodeCT,"ID_CROSSTAB") ;
	SwOk = M4_BOOL( SwOk && Item.FindIString(ai_pcCT) ) ;
	if (!SwOk) goto exit ;

	//****Grabar cambios CT Data

	SaveCTData(CT) ;

	//****Sincronizo CT Cells

	if ( ! SynchronizeCTCell( CT )  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//****Sincronizo rama de Secciones del CT 
	
	if ( ! CT->m_bFirstCrossRowLoaded ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! SynchronizeSectionCnt( CT->m_iFirstCrossRow,ao_poDD) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
exit:

	if (!SwOk) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Synchronize CT %0:s failed", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcCT << SEND_MSG ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_DDLF_Chn::SynchronizeCT(%0:s)", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcCT << SEND_MSG ;
	};
	#endif

	return SwOk;
}


m4bool_t ClENG_DDLF_Chn::SynchronizeCTCell( StENG_DD_CTData *ai_poCT )
{
	m4bool_t SwOk=M4_TRUE, SwNext ;
	StENG_DD_CTData::CellList_t::iterator ItCell ;
	ENG_IdSectionList_t::iterator ItSecParam ;
	m4int16_t Level ;
	ClMIT_ChnItem ItemSecKey, ItemCntType ;
	StENG_DD_CTCell *Cell ;
	ENG_CT_CELL_TYPE CellType ;
	ENG_TYPE_CONTAINER CntType ;

	ItemSecKey.Init( m_poNodeSec,"ID_SECTION") ;
	ItemCntType.Init( m_poNodeSecCnt,"ID_CONTAINER") ;

	//******Borra cells en canal

	//Recorro por SecParam

	for ( Level=0, ItSecParam=ai_poCT->m_oParamSec.begin() ; 
			ItSecParam != ai_poCT->m_oParamSec.end() ; ++ItSecParam, ++Level ) 
	{
		//Busco SecParam en canal

		if ( ! ItemSecKey.FindDouble( *(ItSecParam) ) ) {
			SwOk=M4_FALSE ;
			continue ;	//Para que intente otras Cell
		} ;

		//Recorro sus Cnt

		for ( m_poNodeSecCnt->GoFirst(), SwNext=M4_TRUE; ! m_poNodeSecCnt->IsEOF(); 
			//Qué $@!!# de compilador en Sun te fuerza a poner este casting ! 
			SwNext ? ((ClMIT_ChnNode *) m_poNodeSecCnt)->Next() : (SwNext=M4_TRUE)  ) 
			//( SwNext ? ( m_poNodeSecCnt->Next() ) : ( SwNext=M4_TRUE ) )  ) 
		{

			//Y los que no tengan CellData asociado, los borro

			if ( ! ItemCntType.Load() ) {
				continue ;	//Para que intente otras Cell
			} ;
			switch ( (ENG_TYPE_CONTAINER)(m4int16_t)ItemCntType.m_oValue.GetDouble() ) {
			case ENG_CNT_BEFORE:
				CellType =  ENG_CT_PRE_T ;
				break ;
			case ENG_CNT_AFTER:
				CellType = ENG_CT_POST_T;
				break ;
			case ENG_CNT_PRERECORD:
				CellType = ENG_CT_PRE_S ;
				break ;
			case ENG_CNT_POSTRECORD:
				CellType = ENG_CT_POST_S ;
				break ;
			case ENG_CNT_RECORD:
				CellType = ENG_CT_SUB ;
				break ;
			default:
				continue ;	//Para que intente otras Cell
				break ;
			} ;

			Cell = ai_poCT->FindCell(ENG_TRY_FIND, Level, CellType) ;
			if ( !Cell ) {
				m_poNodeSecCnt->Delete() ;
				SwNext = M4_FALSE ;	//Delete ya hace Next
			} ;
		} ;

	} ;

	//******Add o Modifica. Recorre por SecParam y por Cell. 

	for ( Level=0, ItSecParam=ai_poCT->m_oParamSec.begin() ; 
		ItSecParam != ai_poCT->m_oParamSec.end() ; ++ItSecParam, ++Level ) 
	{
		//Busco SecParam en canal

		if ( ! ItemSecKey.FindDouble( *(ItSecParam) ) ) {
			SwOk=M4_FALSE ;
			continue ;	//Para que intente otras Cell
		} ;

		//Recorro CellData en diseño, para la SecParam que estamos recorriendo

		for ( ItCell=ai_poCT->m_oCellList.begin() ; ItCell!=ai_poCT->m_oCellList.end(); ++ItCell ) {
			Cell=&(*ItCell) ;

			if ( !(Cell->m_bIdParamSecLoaded && Cell->m_uiIdParamSec == *(ItSecParam) 
				&& Cell->m_bTypeLoaded ) ) continue ;

			//La busco en el modelo de datos

			switch ( Cell->m_Type ) {
			case ENG_CT_PRE_T:
				CntType =  ENG_CNT_BEFORE ;
				break ;
			case ENG_CT_POST_T:
				CntType = ENG_CNT_AFTER ;
				break ;
			case ENG_CT_PRE_S:
				CntType = ENG_CNT_PRERECORD ;
				break ;
			case ENG_CT_POST_S:
				CntType = ENG_CNT_POSTRECORD ;
				break ;
			case ENG_CT_SUB:
				CntType = ENG_CNT_RECORD ;
				break ;
			default:
				continue ;	//Para que intente otras Cell
				break ;
			} ;

			if ( ! ItemCntType.FindDouble( CntType ) ) {

				//****Cnt NO encontrado: crear nueva Cell (es decir nuevo CNT)

				m_poNodeSecCnt->Insert() ;
				
				ItemCntType.m_oValue.InitDouble(CntType)  ;	//Mete PK
				ItemCntType.Save() ;

			} else {
				//****Cnt encontrado: modificar Cell existente
				;
			} ;

			//***Reflejar cambios: comun en regsitro nuevo o modificado

			//Grabar cambios CellData 

			SaveCellData(Cell) ;

			//Grabar Fields de CellData

			if ( ! SynchronizeField( &Cell->m_oFieldList, m_poNodeSecCntFld, m_poNodeSecCntFldProp ) ) {
				SwOk=M4_FALSE ;
				continue ;	//Para que intente otras Cell
			} ;

		} ;

	} ;

	return SwOk ;
}


m4bool_t ClENG_DDLF_Chn::SynchronizeSectionCnt( m4uint32_t ai_uiIdSection, ClENG_DesignData *ai_poDD ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_ChnItem ItemSecKey, ItemCntType ;
	StENG_DD_SectionData::SonList_t::iterator SonIt ;
	StENG_DD_SectionData::CntList_t::iterator ItCnt ;
	StENG_DD_SectionData *SecData ;
	StENG_DD_CntData *CntData;

	ItemSecKey.Init( m_poNodeSec,"ID_SECTION") ;
	ItemCntType.Init( m_poNodeSecCnt,"ID_CONTAINER") ;

	//***Posiciono seccion

	//Busca SecData en DD

	SecData = ai_poDD->FindSection(ENG_EXIST_REQUIRED, ai_uiIdSection ) ;
	if ( ! SecData ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Busca Sec en canal

	if ( ! ItemSecKey.FindDouble( ai_uiIdSection ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//***Recorro Cnts, y sincronizo Cnt (solo MODIF) y field (sync completa)

	
	for ( ItCnt=SecData->m_oCnt.begin() ; ItCnt!=SecData->m_oCnt.end(); ++ItCnt ) {
		CntData = &(*ItCnt).second ;

		//Ahora localizo y posiciono en canal el container

		if ( ! ItemCntType.FindDouble( (*ItCnt).first ) ) {
			SwOk=M4_FALSE ;
			continue ;	//Para que intente otras Cell
		} ;

		//Filtro, solo si se fusiona

		//CntData->GetCTId(&StrAux) ;
		//if ( ClMIT_Str::StrICmpTrim(StrAux,ai_pcCT)!=0 ) continue ;

		//****Grabar cambios CntData

		SaveCntData(CntData) ;

		//****Grabar Fields de CellData
	
		if ( ! SynchronizeField( &CntData->m_oFieldList, m_poNodeSecCntFld, m_poNodeSecCntFldProp ) ) { 
			SwOk=M4_FALSE ;
			continue ;	//Para que intente otras Cell
		} ;

	} ;

	//***Ahora, a procesar hijas

	for ( SonIt = SecData->m_oSonList.begin(); SonIt != SecData->m_oSonList.end(); ++SonIt ) {
		if ( ! (*SonIt).m_bIdSonSectionLoaded )  {
			SwOk=M4_FALSE ;
			continue ;	//A intentar otros
		} ;
		if ( ! SynchronizeSectionCnt( (*SonIt).m_uiIdSonSection, ai_poDD ) ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClENG_DDLF_Chn::SynchronizeField( ENG_DD_FieldList_t *ai_poFieldList, 
		ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode ) 
{
	m4bool_t SwOk=M4_TRUE, SwNext ;
	ENG_DD_FieldList_t::iterator ItField ;
	ClMIT_ChnItem FKeyItem ;

	FKeyItem.Init( ai_poFieldNode,"ID_FIELD") ;

	//***Sincronizacion borrados: aquellos que estan en Nodo, pero no en la lista

	for ( ai_poFieldNode->GoFirst(), SwNext=M4_TRUE; ! ai_poFieldNode->IsEOF(); 
        //Qué $@!!# de compilador en Sun te fuerza a poner este casting ! 
        SwNext ? ((ClMIT_ChnNode *) ai_poFieldNode)->Next() : (SwNext=M4_TRUE)  )
		//SwNext ? ai_poFieldNode->Next() : SwNext=M4_TRUE  ) 
	{
		FKeyItem.Load() ;

		for (ItField=ai_poFieldList->begin();ItField!=ai_poFieldList->end();++ItField) {
			if ( ! (*ItField).m_bIdFieldLoaded ) continue ;
			if ( (*ItField).m_uiIdField == FKeyItem.m_oValue.GetDouble() ) {
				break ;
			} ;
		} ;

		if ( ItField == ai_poFieldList->end() ) {
			//No encontrado, borrar
			ai_poFieldNode->Delete() ;
			SwNext=M4_FALSE ;	//Delete ya hace Next
		} ;
	} ;

	
	//***Sincronizacion modificados y nuevos. Rocorro DD y veo canal

	for (ItField=ai_poFieldList->begin();ItField!=ai_poFieldList->end();++ItField) {

		if ( ! (*ItField).m_bIdFieldLoaded ) continue ;

		//Localizo registro asociado

		if ( ! FKeyItem.FindDouble( (*ItField).m_uiIdField ) ) {

			//***No encontrado, Field nuevo

			ai_poFieldNode->Insert() ;
			FKeyItem.m_oValue.InitDouble( (*ItField).m_uiIdField ) ;
			FKeyItem.Save() ;

		} else {

			//***Encontrado, modificar Field
			;

		} ;

		// Comun a modificar o crear

		SaveField( ai_poFieldNode, ai_poPropNode, &(*ItField) ) ;
	} ;

exit:
	return SwOk ;
}



void ClENG_DDLF_Chn::AddDefaultProperties( ClMIT_ChnNode *ai_poMasterNode, 
	PropertieList_t *ai_poList, StENG_DD_Defaults *ai_poData ) 
{
	ClMIT_Str StrAux ;
	ClMIT_ChnItem Item ;

	if ( ai_poData->m_SwIdFontLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_uiIdFont,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_FONT,StrAux) ) ;
	} ;

	if ( ai_poData->m_SwAlignXLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_AlignX,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_ALIGNX,StrAux) ) ;
	} ;
	
	if ( ai_poData->m_SwAlignYLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_AlignY,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_ALIGNY,StrAux) ) ;
	} ;

	if ( ai_poData->m_SwClipLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwClip, &StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_CLIP,StrAux) ) ;
	} ;

	if ( ai_poData->m_SwIdColourLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_uiIdColour,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_COLOUR,StrAux) ) ;
	} ;
	
	if ( ai_poData->m_SwGraphicsLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwGraphics,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_GRAPH_PATH,StrAux) ) ;
	} ;

	if ( ai_poData->m_SwDefDisplayNULLLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oDefDisplayNULL,&StrAux) ;
		if(StrAux.StrLen()) {
			ai_poList->insert( PropertieList_t::value_type(DEFAULT_DISPLAY_NULL,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_SwFormatNULLLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwFormatNULL,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_FORMAT_NULL,StrAux) ) ;
	} ;

	if ( ai_poData->m_SwIdFormatLoaded  ) {
		Item.Init( ai_poMasterNode,"ID_FORMAT") ;
		Item.m_oValue.InitDouble(ai_poData->m_uiIdFormat) ;
		Item.Save() ;
	} ;
}


void ClENG_DDLF_Chn::AddCellAlignProps( ClMIT_ChnNode *ai_poMasterNode, 
	PropertieList_t *ai_poList, StENG_DD_CTCell *ai_poData ) 
{
	ClMIT_Str StrAux ;
	ClMIT_ChnItem Item ;

	if ( ai_poData->m_bAlignParamCellXLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_AlignParamCellX,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_ALIGNX,StrAux) ) ;
	} ;
	
	if ( ai_poData->m_bAlignParamCellYLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_AlignParamCellY,&StrAux) ;
		ai_poList->insert( PropertieList_t::value_type(DEFAULT_ID_ALIGNY,StrAux) ) ;
	} ;
}


void ClENG_DDLF_Chn::SaveField(ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode, 
		StENG_DD_FieldData *ai_poData ) 
{
	ClMIT_ChnItem Item ;
	PropertieList_t PList ;
	ClMIT_Str StrAux ;

	//****Grabo registro

	if ( ai_poData->m_bIdFieldLoaded ) {
		Item.Init( ai_poFieldNode,"ID_FIELD") ;
		Item.m_oValue.InitDouble(ai_poData->m_uiIdField) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bFieldNameLoaded ) {
		Item.Init( ai_poFieldNode,"N_FIELD") ;
		Item.m_oValue.InitString(ai_poData->m_oFieldName) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bFldTypeLoaded ) {
		Item.Init( ai_poFieldNode,"ID_FIELD_TYPE") ;
		Item.m_oValue.InitDouble(ai_poData->m_FldType) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bTopLoaded ) {
		Item.Init( ai_poFieldNode,"TOP") ;
		Item.m_oValue.InitDouble(ai_poData->m_dTop) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bLeftLoaded ) {
		Item.Init( ai_poFieldNode,"LEFT") ;
		Item.m_oValue.InitDouble(ai_poData->m_dLeft) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bWidthLoaded ) {
		Item.Init( ai_poFieldNode,"WIDTH") ;
		Item.m_oValue.InitDouble(ai_poData->m_dWidth) ;
		Item.Save() ;
	} ;
	
	if ( ai_poData->m_bHeightLoaded ) {
		Item.Init( ai_poFieldNode,"HEIGHT") ;
		Item.m_oValue.InitDouble(ai_poData->m_dHeight) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bExprLoaded ) {
		Item.Init( ai_poFieldNode,"VALUE") ;
		Item.m_oValue.InitString(ai_poData->m_oExpr) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bLiteralValueLoaded ) {
		Item.Init( ai_poFieldNode,"LITERAL_VALUE") ;
		Item.m_oValue.InitString(ai_poData->m_oLiteralValue) ;
		Item.Save() ;
	} ;


	//****Lista de properties

	if ( ai_poData->m_bSaveNodeLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oSaveNode,&StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_SAVE_NODE,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bSaveItemNameLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oSaveItemName, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_SAVE_ITEM,StrAux) ) ;
		};
	} ;
	
	if ( ai_poData->m_bSwOptimizeLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwOptimize, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_DESIGN_OPTIMIZE,StrAux) ) ;
	} ;

	if ( ai_poData->m_bSwStaticLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwStatic, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_DESIGN_STATIC,StrAux) ) ;
	} ;

	if ( ai_poData->m_bInterlineLoaded ) {
		ClMIT_ChnValue::DoubleToAnonymous( ai_poData->m_dInterline, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_MULTIL_INTERL,StrAux) ) ;
	} ;

	if ( ai_poData->m_bIsVarHeightLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bIsVarHeight, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_MULTIL_VAR_HEIGHT,StrAux) ) ;
	} ;

	if ( ai_poData->m_bIsJustifiedLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bIsJustified, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_MULTIL_JUSTIFIED,StrAux) ) ;
	} ;

	if ( ai_poData->m_bBarCodeTypeLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_BarCodeType, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_BARCODE_TYPE,StrAux) ) ;
	} ;

	if ( ai_poData->m_bMaxBoxesLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_iMaxBoxes, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_MAX_BOXES,StrAux) ) ;
	} ;

	if ( ai_poData->m_bFillModeLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_FillMode, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_FILL_MODE,StrAux) ) ;
	} ;

	if ( ai_poData->m_bFillCharLoaded ) {
		StrAux.Accommodate(2) ; 
		*StrAux.InChar(0) = ai_poData->m_cFillChar ; 
		*StrAux.InChar(1) = M4_END_STR ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_FILL_CHAR,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bSwDuplicatedLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bSwDuplicated, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_DUPLICATED,StrAux) ) ;
	} ;

	if ( ai_poData->m_bIdCtFieldLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oIdCtField, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_ID_CT_FIELD,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bIdPiLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oIdPi, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_ID_PI,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bRegionBorderLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oRegionBorder, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_REGION_BORDER,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bRegionColourLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_uiRegionColour, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_REGION_COLOUR,StrAux) ) ;
	} ;

	if ( ai_poData->m_bIndexLevelLoaded ) {
		if(ai_poData->m_iIndexLevel!=0) {
			ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_iIndexLevel, &StrAux) ;
			PList.insert( PropertieList_t::value_type(FLD_INDEX_LEVEL,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bIndexExprLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oIndexExpr, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_INDEX_EXPR,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bHyperlinkLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oHyperlink, &StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(FLD_HYPERLINK,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bTempWebImageLoaded ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bTempWebImage, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_RASTER_TEMP_WEB_IMAGE,StrAux) ) ;
	} ;

	if ( ai_poData->m_bAdjustLineToContainer ) {
		ClMIT_ChnValue::BooleanToAnonymous( ai_poData->m_bAdjustLineToContainer, &StrAux) ;
		PList.insert( PropertieList_t::value_type(FLD_LINE_ADJUST_TO_CONTAINER,StrAux) ) ;
	} ;

	AddDefaultProperties( ai_poFieldNode, &PList, &ai_poData->m_stDefaultData ) ;

	SynchronizeProperties( ai_poPropNode, &PList ) ; 	

	/*??
		Items Blob ???
	*/

}

void ClENG_DDLF_Chn::SaveCTData(StENG_DD_CTData *ai_poData) 
{
	ClMIT_ChnItem Item ;
	PropertieList_t PList ;
	ClMIT_Str StrAux ;

	//****Grabo registro

	if ( ai_poData->m_bCTIdLoaded ) {
		Item.Init( m_poNodeCT, "ID_CROSSTAB") ;
		Item.m_oValue.InitString(ai_poData->m_oCTId) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bNameLoaded ) {
		Item.Init( m_poNodeCT, "N_CROSSTAB") ;
		Item.m_oValue.InitString(ai_poData->m_oName) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bTotListLoaded ) {
		Item.Init( m_poNodeCT, "TOT_LIST") ;
		Item.m_oValue.InitString(ai_poData->m_oTotList) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bTreeListLoaded ) {
		Item.Init( m_poNodeCT, "TREE_LIST") ;
		Item.m_oValue.InitString(ai_poData->m_oTreeList) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bFillTreeLoaded ) {
		Item.Init( m_poNodeCT, "SOURCE_KEY_LIST") ;
		Item.m_oValue.InitString(ai_poData->m_oFillTree) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bIdParamSecLoaded ) {
		Item.Init( m_poNodeCT, "INIT_PARAM_SECTION") ;
		Item.m_oValue.InitDouble(ai_poData->m_uiIdParamSec) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bFirstCrossRowLoaded ) {
		Item.Init( m_poNodeCT, "CROSS_ROW_FIRST") ;
		Item.m_oValue.InitDouble(ai_poData->m_iFirstCrossRow) ;
		Item.Save() ;
	} ;

	//****Lista de properties

	if ( ai_poData->m_bMergePageLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_MergePage,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CT_PROP_MERGE_PAGE,StrAux) ) ;
	} ;

	if ( ai_poData->m_bXColumnLeftLoaded ) {
		ClMIT_ChnValue::DoubleToAnonymous( ai_poData->m_dXColumnLeft,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CT_PROP_XCOLUMN_LEFT,StrAux) ) ;
	} ;

	if ( ai_poData->m_bXColumnRightLoaded ) {
		ClMIT_ChnValue::DoubleToAnonymous( ai_poData->m_dXColumnRight,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CT_PROP_XCOLUMN_RIGHT,StrAux) ) ;
	} ;
	
	if ( ai_poData->m_bTogLevelLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_iTogLevel,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CT_PROP_TOG_LEVEL,StrAux) ) ;
	} ;

	if ( ai_poData->m_bWizardDataLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oWizardData,&StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(CT_PROP_WIZARD_DATA,StrAux) ) ;
		};
	} ;

	SynchronizeProperties( m_poNodeCTProp, &PList ) ; 	
}


void ClENG_DDLF_Chn::SaveCntData(StENG_DD_CntData *ai_poData) 
{
	ClMIT_ChnItem Item ;
	PropertieList_t PList ;
	ClMIT_Str StrAux ;

	//****Grabo registro

	if ( ai_poData->m_bPageLoaded ) {
		Item.Init( m_poNodeSecCnt, "ID_PAGE") ;
		Item.m_oValue.InitDouble(ai_poData->m_iPage) ;
		Item.Save() ;
	} ;
	
	if ( ai_poData->m_bTopLoaded ) {
		Item.Init( m_poNodeSecCnt, "TOP") ;
		Item.m_oValue.InitDouble(ai_poData->m_dTop) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bLeftLoaded ) {
		Item.Init( m_poNodeSecCnt, "LEFT") ;
		Item.m_oValue.InitDouble(ai_poData->m_dLeft) ;
		Item.Save() ;
	} ;
	
	if ( ai_poData->m_bHeightLoaded ) {
		Item.Init( m_poNodeSecCnt, "HEIGHT") ;
		Item.m_oValue.InitDouble(ai_poData->m_dHeight) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bWidthLoaded ) {
		Item.Init( m_poNodeSecCnt, "WIDTH") ;
		Item.m_oValue.InitDouble(ai_poData->m_dWidth) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bIsFixedLoaded ) {
		Item.InitBoolean( m_poNodeSecCnt, "IS_FIXED") ;
		Item.m_oValue.InitBoolean(ai_poData->m_IsFixed) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bSwTogetherLoaded ) {
		Item.InitBoolean( m_poNodeSecCnt, "KEEP_CONTAIN_TOGH") ;
		Item.m_oValue.InitBoolean(ai_poData->m_bSwTogether) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bCancelCntLoaded ) {
		Item.Init( m_poNodeSecCnt, "CANCEL_CONTAINER") ;
		Item.m_oValue.InitString(ai_poData->m_oCancelCnt) ;
		Item.Save() ;
	} ;

	//****Lista de properties

	if ( ai_poData->m_bLowerLimitLoaded ) {
		ClMIT_ChnValue::DoubleToAnonymous( ai_poData->m_dLowerLimit,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CNT_LOWER_LIMIT,StrAux) ) ;
	} ;

	if ( ai_poData->m_bCTMergeLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oCTMerge,&StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(CNT_CT_MERGE,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bRegionBorderLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oRegionBorder,&StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(CNT_REGION_BORDER,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bRegionColourLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_uiRegionColour,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CNT_REGION_COLOUR,StrAux) ) ;
	} ;
	
	AddDefaultProperties( m_poNodeSecCnt, &PList, &ai_poData->m_stDefaultData ) ;

	SynchronizeProperties( m_poNodeSecCntProp, &PList ) ; 	
}


void ClENG_DDLF_Chn::SaveCellData(StENG_DD_CTCell *ai_poData) 
{
	ClMIT_ChnItem Item ;
	PropertieList_t PList ;
	ClMIT_Str StrAux ;

	//****Grabo registro

	//Grabamos valor obligatorio del ID_PAGE
	Item.Init( m_poNodeSecCnt, "ID_PAGE") ;
	Item.m_oValue.InitDouble(1) ;
	Item.Save() ;
	
	//Grabamos valor obligatorio del TOP
	Item.Init( m_poNodeSecCnt, "TOP") ;
	Item.m_oValue.InitDouble(0) ;
	Item.Save() ;
	
	//Grabamos valor obligatorio del LEFT
	Item.Init( m_poNodeSecCnt, "LEFT") ;
	Item.m_oValue.InitDouble(0) ;
	Item.Save() ;
	
	if ( ai_poData->m_bHeightLoaded ) {
		Item.Init( m_poNodeSecCnt, "HEIGHT") ;
		Item.m_oValue.InitDouble(ai_poData->m_dHeight) ;
		Item.Save() ;
	} ;

	if ( ai_poData->m_bWidthLoaded ) {
		Item.Init( m_poNodeSecCnt, "WIDTH") ;
		Item.m_oValue.InitDouble(ai_poData->m_dWidth) ;
		Item.Save() ;
	} ;

	//Grabamos valor obligatorio del IS_FIXED
	Item.InitBoolean( m_poNodeSecCnt, "IS_FIXED") ;
	Item.m_oValue.InitBoolean(0) ;
	Item.Save() ;
	
	//****Lista de properties

	if ( ai_poData->m_bRegionBorderLoaded ) {
		ClMIT_ChnValue::StringToAnonymous( ai_poData->m_oRegionBorder,&StrAux) ;
		if(StrAux.StrLen()) {
			PList.insert( PropertieList_t::value_type(CNT_REGION_BORDER,StrAux) ) ;
		};
	} ;

	if ( ai_poData->m_bRegionColourLoaded ) {
		ClMIT_ChnValue::NumberToAnonymous( ai_poData->m_uiRegionColour,&StrAux) ;
		PList.insert( PropertieList_t::value_type(CNT_REGION_COLOUR,StrAux) ) ;
	} ;
	
	AddCellAlignProps( m_poNodeSecCnt, &PList, ai_poData ) ;

	SynchronizeProperties( m_poNodeSecCntProp, &PList ) ; 	
}

void ClENG_DDLF_Chn::SynchronizeFontsUsedByReport(ClENG_DesignData *ao_poDD) 
{
	ClMIT_ChnItem Item ;
	ClENG_DesignData::FontList_t::iterator It ;

	Item.Init(m_poNodeFont,"ID_FONT") ; 

	//***Borra Fonts no usadas

	for ( m_poNodeFont->GoFirst(); ! m_poNodeFont->IsEOF(); ) {

		Item.Load() ;

		if ( ! ao_poDD->FindFont(Item.m_oValue.GetDouble(),It) ) {
			m_poNodeFont->Delete() ;	//ya nos deja en el Next
		} else {
			m_poNodeFont->Next() ;
		} ;
	} ;

	//***Añade Fonts usadas que no existan ya

	for (It=ao_poDD->m_oFontList.begin();It!=ao_poDD->m_oFontList.end();++It) {
		if ( ! (*It).m_bIdFontLoaded ) continue ;
		if ( ! Item.FindDouble((*It).m_uiIdFont)  ) {
			m_poNodeFont->Insert() ;
			Item.m_oValue.InitDouble((*It).m_uiIdFont)  ;	//Mete PK
			Item.Save() ;
		} ;
	} ;
}

void  ClENG_DDLF_Chn::SynchronizeFormatsUsedByReport(ClENG_DesignData *ao_poDD) 
{
	ClMIT_ChnItem Item ;
	ClENG_DesignData::FormatList_t::iterator It ;

	Item.Init(m_poNodeFormat,"ID_FORMAT") ; 

	//***Borra Formats no usados

	for ( m_poNodeFormat->GoFirst(); ! m_poNodeFormat->IsEOF(); ) {

		Item.Load() ;

		if ( ! ao_poDD->FindFormat(Item.m_oValue.GetDouble(),It) ) {
			m_poNodeFormat->Delete() ;	//ya nos deja en el Next
		} else {
			m_poNodeFormat->Next() ;
		} ;
	} ;

	//***Añade Formats usados que no existan ya

	for (It=ao_poDD->m_oFormatList.begin();It!=ao_poDD->m_oFormatList.end();++It) {
		if ( ! (*It).m_bIdFormatLoaded ) continue ;
		if ( ! Item.FindDouble((*It).m_uiIdFormat)  ) {
			m_poNodeFormat->Insert() ;
			Item.m_oValue.InitDouble((*It).m_uiIdFormat)  ;	//Mete PK
			Item.Save() ;
		} ;
	} ;

}

void ClENG_DDLF_Chn::SynchronizeColoursUsedByReport(ClENG_DesignData *ao_poDD) 
{
	ClMIT_ChnItem Item ;
	ClENG_DesignData::ColourList_t::iterator It ;

	Item.Init(m_poNodeColour,"ID_COLOR") ; 

	//***Borra Colores no usados

	for ( m_poNodeColour->GoFirst(); ! m_poNodeColour->IsEOF(); ) {

		Item.Load() ;

		if ( ! ao_poDD->FindColour(Item.m_oValue.GetDouble(),It) ) {
			m_poNodeColour->Delete() ;	//ya nos deja en el Next
		} else {
			m_poNodeColour->Next() ;
		} ;
	} ;

	//***Añade Colores usados que no existan ya

	for (It=ao_poDD->m_oFGPalette.begin();It!=ao_poDD->m_oFGPalette.end();++It) {
		if ( ! (*It).m_bIdColourLoaded ) continue ;
		if ( ! Item.FindDouble((*It).m_uiIdColour)  ) {
			m_poNodeColour->Insert() ;
			Item.m_oValue.InitDouble((*It).m_uiIdColour)  ;	//Mete PK
			Item.Save() ;
		} ;
	} ;
}


m4bool_t ClENG_DDLF_Chn::SetCTProps(m4char_t *ai_pcCTId, m4char_t* ai_pcCTProps) 
{
	ClMIT_Str RowStr, ColStr, TotStr;
	ClMIT_ChnItem ItemAux, Item;
	m4bool_t SwOk=M4_TRUE;
	m4int16_t B, Record;
	ClMIT_Args Args;
	PropertieList_t PList ;
	ClMIT_Str StrAux ;

	
	ItemAux.Init(m_poNodeCT,"ID_CROSSTAB") ; 
	if(!ItemAux.FindString(ai_pcCTId)) {
		SwOk=M4_FALSE;
		goto exit;
	};

	//***Borra properties 
	for ( m_poNodeCTProp->GoFirst(); ! m_poNodeCTProp->IsEOF(); ) {
		m_poNodeCTProp->Delete() ;	//borra todos
	} ;

	Args.Init(ai_pcCTProps, ClMIT_Args::NO_SUBARG, ";");
	if(Args.GetArgCount()!=3) {
		SwOk=M4_FALSE;
		goto exit;
	} else {
		Args.GetString(0,0,&RowStr) ;
		Args.GetString(1,0,&ColStr) ;
		Args.GetString(2,0,&TotStr) ;
	};


	ClMIT_ChnValue::StringToAnonymous(RowStr,&StrAux) ;
	PList.insert( PropertieList_t::value_type(CT_ROW_PROPS,StrAux) ) ;

	ClMIT_ChnValue::StringToAnonymous(ColStr,&StrAux) ;
	PList.insert( PropertieList_t::value_type(CT_COL_PROPS,StrAux) ) ;

	ClMIT_ChnValue::StringToAnonymous(TotStr,&StrAux) ;
	PList.insert( PropertieList_t::value_type(CT_TOT_PROPS,StrAux) ) ;

	SynchronizeProperties( m_poNodeCTProp, &PList, M4_TRUE ) ;
	
exit:
	return SwOk;
}


m4bool_t ClENG_DDLF_Chn::GetCTProps(m4char_t *ai_pcCTId, m4char_t* ai_pcCTProps) 
{
	ClMIT_Str RowStr, ColStr, TotStr, Str;
	ClMIT_ChnItem ItemAux, Item;
	m4bool_t SwOk=M4_TRUE;
	m4double_t IdProperty;

	ItemAux.Init(m_poNodeCT,"ID_CROSSTAB") ; 
	if(!ItemAux.FindString(ai_pcCTId)) {
		SwOk=M4_FALSE;
		goto exit;
	};

	//***Lee propiedades
	for ( m_poNodeCTProp->GoFirst(); ! m_poNodeCTProp->IsEOF(); m_poNodeCTProp->Next() ) {
		Item.InitLoad(m_poNodeCTProp,"ID_PROPERTY");
		IdProperty = Item.m_oValue.GetDouble();
		
		if(IdProperty>=CT_ROW_PROPS && IdProperty<CT_COL_PROPS) {
			Item.InitLoad(m_poNodeCTProp,"VALUE");
			if(RowStr.StrLen()>0) RowStr.StrCat(",");
			RowStr.StrCat(Item.m_oValue.GetString());
		} else if(IdProperty>=CT_COL_PROPS && IdProperty<CT_TOT_PROPS) {
			Item.InitLoad(m_poNodeCTProp,"VALUE");
			if(ColStr.StrLen()>0) ColStr.StrCat(",");
			ColStr.StrCat(Item.m_oValue.GetString());
		} else if(IdProperty>=CT_TOT_PROPS && IdProperty<CT_LIMIT) {
			Item.InitLoad(m_poNodeCTProp,"VALUE");
			if(TotStr.StrLen()>0) TotStr.StrCat(",");
			TotStr.StrCat(Item.m_oValue.GetString());
		};
	};

	if(!RowStr.StrLen() && !ColStr.StrLen() && !TotStr.StrLen()) {
		Str.StrCat("");
	} else {
		Str.StrCat(RowStr);
		Str.StrCat(";");
		Str.StrCat(ColStr);
		Str.StrCat(";");
		Str.StrCat(TotStr);
	};

	strcpy(ai_pcCTProps, Str);

exit:
	return SwOk;
}
