#include "vmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                findrJIT.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                07-01-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//		Item Finder used to compile "Just In Time"
//
//
//==============================================================================

#include "finder.hpp"
#include "trad.h"

#include "arecset.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "nodedef.hpp"
#include "clcodejit.hpp"

m4bool_t    ClFinder_JIT::IsItem              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem)
{
	
	
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);

		
	if (FindItem(ai_Channel,ai_Node,ai_Item,&m_oItemActual)==M4_SUCCESS){
		ao_RealItem=m_oItemActual.m_pszItemId;
		if (m_oItemActual.m_pdType == 0){
			DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_JIT, "Item Type"<<ai_Item<<ai_Node<<ai_Channel);
            m_poTrad->SetError();
            return M4_FALSE;
        }
        if ( m_oItemActual.m_pdType != M4CL_ITEM_TYPE_METHOD ) {
			return M4_TRUE;
        }
 
	}

	return M4_FALSE;
}


m4bool_t    ClFinder_JIT::IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem )
{
	m4uint8_t	cArgType;
	
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);

	if (FindItem(ai_Channel,ai_Node,ai_Item,&m_oItemActual)!=M4_SUCCESS){
		ao_iLevel=0;
		return M4_FALSE;
	}
	ao_pcRealItem=m_oItemActual.m_pszItemId;
	if (strcmp (ai_Channel,"") == 0 && m_oItemActual.m_pdType==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_JIT, "Item Type"<<ai_Item<<ai_Node<<ai_Channel);
        m_poTrad->SetError();
        return M4_FALSE;
	}
	

	ClCompiledMCR *poCMCR = m_oItemActual.m_poCMCR;
	
	if ( m_oItemActual.m_pdType== M4CL_ITEM_TYPE_METHOD ||	 m_oItemActual.m_pdType== M4CL_ITEM_TYPE_CONCEPT ){
		ao_iRealArgs=(m4uint8_t) poCMCR->GetItemNumberOfArguments(m_oItemActual.m_hItemHandle);

		for (m4int16_t i=0;i<ao_iRealArgs;i++){
			cArgType=poCMCR->GetItemArgumentTypeByPosition(m_oItemActual.m_hItemHandle,i);
			if (cArgType==0){
				DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_JIT,"Argument type"<<ai_Item<<ai_Node<<ai_Channel);
                m_poTrad->SetError();
                return M4_FALSE;
            }
			if (cArgType==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){
				ao_acArgDes[i]='r';
			}
			else{
				ao_acArgDes[i]='R';
			}
		}

		if (poCMCR->GetItemVariableArguments(m_oItemActual.m_hItemHandle)){
            ao_iRealArgs=(ao_iRealArgs?-ao_iRealArgs:-(M4CL_MAX_PARAM+1)) ;
		}

		if (strcmp(ai_Channel,"")==0 || ai_Channel==0){
			ao_iLevel=1;
		}
		else{
			ao_iLevel=2;
		}
	}
	else {
		ao_iLevel=0;
		return M4_FALSE;
	}
	return M4_TRUE;

}

	
m4bool_t    ClFinder_JIT::IsRule              (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4char_t *ai_Rule)
{



	ai_Rule = strupr( ai_Rule );
	ai_Item = strupr( ai_Item );
	ai_Node = strupr( ai_Node );
	ai_Channel = strupr( ai_Channel );	
	if ( FindItem (ai_Channel, ai_Node, ai_Item, &m_oItemActual ) != M4_SUCCESS)
		return M4_FALSE;
	ClCompiledMCR *poCMCR = m_oItemActual.m_poCMCR;
	m4int16_t  iHandle=poCMCR->GetItemFormOrderByFormId(m_oItemActual.m_hItemHandle ,ai_Rule);
	if (iHandle<0){
		return M4_FALSE ;
	}
	return M4_TRUE;

}
m4bool_t    ClFinder_JIT::IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem)
{
	return M4_FALSE;

/*  Ya no se puede acceder a las reglas del padre

	m4uint8_t	cArgType;
	
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);

	if (FindParentItem(ai_Channel,ai_Node,ai_Item,&m_oItemActual)!=M4_SUCCESS){
		ao_iLevel=0;
		return M4_FALSE;
	}
	ao_pcRealItem=m_oItemActual.m_pszItemId;
	if (m_oItemActual.m_pdType==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_JIT,"Item Type"<<ai_Item<<ai_Node<<ai_Channel);
        m_poTrad->SetError();
        return M4_FALSE;
	}
	

	ClCompiledMCR *poCMCR = m_oItemActual.m_poCMCR;;

	if ( m_oItemActual.m_pdType== M4CL_ITEM_TYPE_METHOD ||	 m_oItemActual.m_pdType== M4CL_ITEM_TYPE_CONCEPT ){
		ao_iRealArgs=(m4uint8_t)poCMCR->GetItemNumberOfArguments(m_oItemActual.m_hItemHandle);

		for (m4int16_t i=0;i<ao_iRealArgs;i++){
			cArgType=poCMCR->GetItemArgumentTypeByPosition(m_oItemActual.m_hItemHandle,i);
			if (cArgType==0){
				DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_JIT,"Argument type"<<ai_Item<<ai_Node<<ai_Channel);
                m_poTrad->SetError();
                return M4_FALSE;
            }
			if (cArgType==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){
				ao_acArgDes[i]='r';
			}
			else{
				ao_acArgDes[i]='R';
			}
		}

		if (poCMCR->GetItemVariableArguments(m_oItemActual.m_hItemHandle)){
            ao_iRealArgs=(ao_iRealArgs?-ao_iRealArgs:-(M4CL_MAX_PARAM+1)) ;
		}
		if (strcmp(ai_Channel,"")==0 || ai_Channel==0){
			ao_iLevel=1;
		}
		else{
			ao_iLevel=2;
		}
	}
	else {
		ao_iLevel=0;
		return M4_FALSE;
	}
	return M4_TRUE;
*/

}


m4bool_t    ClFinder_JIT::IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule)
{

	return M4_FALSE;
	/*  Ya no se puede acceder a las reglas del padre

	m4uint16_t	iNumRules;
	ai_Rule=strupr(ai_Rule);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	
	if ( FindParentItem (ai_Channel, ai_Node, ai_Item, &m_oItemActual ) != M4_SUCCESS)
		return M4_FALSE;
	ClCompiledMCR	*poCMCR = m_oItemActual.m_poCMCR;
	

	iNumRules=poCMCR->GetItemNumberOfFormulas(m_oItemActual.m_hItemHandle);

	for (m4uint16_t i=0;i<iNumRules;i++)
	{
////////////////////////////////////////////////////////////////////////////////////////////
//		Hay que mirar a ver si existe la regla buscada.
//		Me falta una funcion del CMCR que me de el ID de las reglas por la posición.
////////////////////////////////////////////////////////////////////////////////////////////
	}

	return M4_FALSE;
	*/
}


m4return_t  ClFinder_JIT::FindItem         ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem,StItemInfoJIT *ao_pStItem)
{
	m4return_t ret=M4_SUCCESS;

	m4bool_t	bNodeFinded=M4_FALSE;
	ClHandle	hNodoActual=0,hItem=0,hNodo=0,hChannel;
	
	m4pcchar_t		pccChannelName = NULL;

	ClCompiledMCR *poCMCR;
//PARA NIVEL 0

	ai_pcItem=strupr(ai_pcItem);
	ai_pcNode=strupr(ai_pcNode);
	ai_pcChannel=strupr(ai_pcChannel);

#ifdef _DEBUG
	if (m_poAccess == NULL) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ACCESS,"Compiler JIT");
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
#endif

	poCMCR = m_poAccess->GetpCMCR() ;
	hNodoActual=m_poRecordSet->GetpNodeDef()->GetHandle();

	//Obtenemos el CMCR
	if (stricmp(ai_pcChannel,"")!=0){
	
	
		// Busco primero si me han definido una instancia en Run Time con este nombre.
		
		pccChannelName = m_poAccess->GetpChannel()->Level2.GetChannelInstanceforJIT ( ai_pcChannel );
		
		// Si no han definido la instancia, miro a ver si esta en las instancias del canal (T3_ALIAS_RES)
		if (pccChannelName == NULL)	{
			 m4pcchar_t pccChannelInstance = poCMCR->GetNodeT3AliasInstanceIdByAlias(hNodoActual,ai_pcChannel);
			 ClHandle hInstance = poCMCR -> GetChannelInstanceHandleById (pccChannelInstance);
			 if (hInstance){
				 pccChannelName = poCMCR -> GetInstanceT3Id (hInstance);
			 }
			 else {

				DUMP_CHLOG_ERRORF(M4_CH_VM_INSTANCE_NOT_FOUND_COMP_JIT,pccChannelInstance);
				m_poTrad->SetError();
				return M4_ERROR;
			 }

			
			if (pccChannelName == NULL ){
				DUMP_CHLOG_ERRORF(M4_CH_VM_INSTANCE_NOT_FOUND_COMP_JIT,ai_pcChannel);
				m_poTrad->SetError();
				return M4_ERROR;
			}
		}

		// Tengo en pccChannelName el nombre verdadero del canal.
		// Obtengo el metacanal

		ret =  m_poAccess->GetpChannel()->GetpCodeJIT()->GetMetaChannel ( pccChannelName,poCMCR );

		if ( poCMCR == NULL || ret != M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_BUILD_CHANNEL_COMP_JIT,pccChannelName<<ai_pcChannel);
			m_poTrad->SetError();
			return M4_ERROR;
		}

		hNodo = poCMCR ->GetChannelNodeHandleById (ai_pcNode);
		if (hNodo == 0)	{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NODE_NOT_FOUND_COMP_JIT, poCMCR->iM4Obj()<<LogCat<<"!"<<LogCat<<ai_pcNode<< LogCatAnd << poCMCR->nM4Obj()<<LogCat<<"!"<<LogCat<<ai_pcNode);
			m_poTrad->SetError();
			return M4_ERROR;
		}

	}
	else{
		if (stricmp(ai_pcNode,"")!=0){

			hNodo=poCMCR->GetNodeAliasNodeHandleByAlias(hNodoActual,ai_pcNode, m_iRuleLevel);
			if (hNodo==0){
				DUMP_CHLOG_ERRORF(M4_CH_VM_NODE_NOT_FOUND_COMP_JIT,ai_pcNode);
				m_poTrad->SetError();
				return M4_ERROR;
			}
			bNodeFinded=M4_TRUE;
		
		}
		else{
			hNodo=hNodoActual;
			bNodeFinded=M4_TRUE;
		}
	}


	if (strcmp(ai_pcItem,"")!=0){

		switch (*ai_pcItem)	{
		case '#':
			break;
		case '@':

			break;
		default:


			hItem=poCMCR->GetNodeItemHandleById(hNodo,ai_pcItem);

			if (hItem==0){
				return (M4_ERROR); //No encontró el ITEM para la TI.
			}

			m_oItemActual.m_pdType = poCMCR->GetItemType(hItem);
			m_oItemActual.m_pszItemId = ai_pcItem;
			m_oItemActual.m_hItemHandle = hItem;
			m_oItemActual.m_poCMCR = poCMCR;

			return M4_SUCCESS;
		}
	}

	return M4_ERROR;
}


m4return_t  ClFinder_JIT::FindParentItem   ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem,StItemInfoJIT *ao_pStItem)
{
	return M4_ERROR;
}


m4return_t	ClFinder_JIT::SetValue(m4pchar_t ai_pszItemId, m4pchar_t ai_pszValue, m4bool_t ai_bChangeStatus)
{
	return M4_SUCCESS;
}

m4return_t ClFinder_JIT::SetValue(m4pchar_t ai_pszItemId, m4double_t ai_dNumber, m4bool_t ai_bChangeStatus)
{
	return M4_SUCCESS;
}



//--------------------------------------
//Retorna si el argumetno es un alias o no 
//--------------------------------------
m4bool_t	ClFinder_JIT::IsAliasOrMyT3				(m4char_t *ai_Channel){
	m4return_t ret=M4_SUCCESS;

	m4bool_t	bNodeFinded=M4_FALSE;
	ClHandle	hNodoActual=0;
	m4pcchar_t  pccChannelName,pccChannelInstance;
	

	ClCompiledMCR *poCMCR;


	ai_Channel=strupr(ai_Channel);

#ifdef _DEBUG
	if (m_poAccess == NULL) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ACCESS,"Compiler JIT");
        m_poTrad->SetError();
		return( M4_FALSE ) ;
	}
#endif

	poCMCR = m_poAccess->GetpCMCR() ;
	hNodoActual=m_poRecordSet->GetpNodeDef()->GetHandle();

	//Obtenemos el CMCR
	if (stricmp(ai_Channel,"")==0) {
		return M4_FALSE;
	}

	
	
	// Busco primero si me han definido una instancia en Run Time con este nombre.
	
	pccChannelName = m_poAccess->GetpChannel()->Level2.GetChannelInstanceforJIT ( ai_Channel );
	
	// Si no han definido la instancia, miro a ver si esta en las instancias del canal (T3_ALIAS_RES)
	if (pccChannelName == 0){
		pccChannelInstance = poCMCR->GetNodeT3AliasInstanceIdByAlias(hNodoActual,ai_Channel);
		m4uint32_t hInstance = poCMCR -> GetChannelInstanceHandleById (pccChannelInstance);
		if (!hInstance){
			//miramos si es nuestro mismo canal
			if (strcmp(ai_Channel,poCMCR->GetChannelId()) !=0){
				DUMP_CHLOG_ERRORF(M4_CH_VM_INSTANCE_NOT_FOUND_COMP_JIT,ai_Channel);
				m_poTrad->SetError();
				return M4_FALSE;
			}
		}
	}
	return M4_TRUE;
}

