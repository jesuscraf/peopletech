//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                finder.hpp   
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
//    This module defines the item finder used by the compiler.
//
//
//==============================================================================

#ifndef _M4_FINDER_
#define _M4_FINDER_

#include "vmtypes.hpp"
#include "register.hpp"
#include "cpdefine.hpp"

//Lo siento
#include "arecset.hpp"


#define M4CL_FINDER_CHANNEL_TI_CHANNEL						1
#define M4CL_FINDER_CHANNEL_DESIGN_CHANNEL					2

class ClTrad;
class ClCompiledMCR;
//Estructura para guardar información del último Item buscado en FindItem.
struct StItemInfo
{
	m4pcchar_t	m_pszItem_TI;
	m4pcchar_t	m_pszItem_T3;
	m4pcchar_t	m_pszItem_Node_T3;
	m4bool_t	m_bIsInherited;	// Indica si es del sistema.
};

struct StItemInfoJIT
{	
	
	m4pcchar_t		m_pszItemId;
	m4uint8_t		m_pdType;
	ClHandle		m_hItemHandle;
	ClCompiledMCR	*m_poCMCR;

};
typedef struct SystemItems_tag
{
	ClAccess *poAccessSystemItems ;
	ClAccessRecordSet *poRSSystemItems ;

	SystemItems_tag() : poAccessSystemItems(0),poRSSystemItems(0){}
	~SystemItems_tag ();
	void SetRecordSet (m4uint32_t ai_handle);

} SystemItems_t;

class ClFinder
{
	
	public:
		ClFinder(void){};
		ClFinder(ClTrad *ai_poTrad)		
		{
			m_poTrad=ai_poTrad;
			m_poAccess = NULL;
			m_poRecordSet = NULL;
			m_oRegIndex = EOF;
		}
		
		virtual m4bool_t    IsItem              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem) {return M4_FALSE;}
		virtual m4bool_t    IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem){ao_iLevel = 0 ; return M4_FALSE;}
		virtual m4bool_t    IsRule              (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4char_t *ai_Rule){return M4_FALSE;}
		virtual m4bool_t    IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem){ao_iLevel = 0 ;return M4_FALSE;}
		virtual m4bool_t    IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule){return M4_FALSE;}
		virtual m4return_t	SetValue			(m4pchar_t ai_pszItemId, m4pchar_t ai_pszValue, m4bool_t ai_bChangeStatus) {return M4_ERROR;}
		virtual m4return_t  SetValue			(m4pchar_t ai_pszItemId, m4double_t ai_dNumber, m4bool_t ai_bChangeStatus) {return M4_ERROR;}
		virtual m4return_t	SetAccess			(ClAccess * ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel) {return M4_ERROR;}
		virtual m4bool_t	Is_Nihil			(void) { return M4_TRUE; }
		virtual m4bool_t	Is_TICH				(void) { return M4_FALSE;}
		virtual m4bool_t	Is_JIT				(void) { return M4_FALSE;}
		virtual void		Reset				(void) {}
		virtual	m4bool_t	IsAliasOrMyT3		(m4char_t *ai_Channel) {return M4_FALSE;}
		ClAccess *			GetAccess			(void){	return m_poAccess;}

		ClAccessRecordSet			*m_poRecordSet;
		ClRegisterIndex		m_oRegIndex;

	protected:
		
		ClAccess			*m_poAccess;
		ClTrad				*m_poTrad;
};


class ClFinder_TICH	: public ClFinder

{
	friend class IntLN4;

	public:
		
		ClFinder_TICH(ClTrad *ai_poTrad):ClFinder(ai_poTrad)
		{
			m_oAccessFind = NULL;
			m_poAccessItems = NULL;
			m_pIdT3=0;
			m_pThisIdT3=0;
			m_iItemIdT3=M4DM_EOF_INDEX;
			m_iItemOld_TI = M4DM_EOF_INDEX;
			m_iItemID_TI = M4DM_EOF_INDEX;
			m_bTIAliasFound = M4_TRUE;
			m_pm2MetaChannel=0;

		};

		~ClFinder_TICH(void);

		m4bool_t    IsItem              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem);
		m4bool_t    IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsRule              (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4char_t *ai_Rule);
		m4bool_t    IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule);
		m4return_t	SetArguments( void );		
		m4return_t	GetLN4Code(m4pchar_t &pszFormula);
		m4return_t	SetValue(m4pchar_t ai_pszItemId, m4pchar_t ai_pszValue, m4bool_t ai_bChangeStatus);
		m4return_t  SetValue (m4pchar_t ai_pszItemId, m4double_t ai_dNumber, m4bool_t ai_bChangeStatus);
		void		GetLastItemID_T3_USED (m4pcchar_t &ai_pszId_T3_Used){ai_pszId_T3_Used=m_oItemActual.m_pszItem_T3;}
		void		GetLastItemID_TI_USED (m4pcchar_t &ai_pszId_TI_Used){ai_pszId_TI_Used=m_oItemActual.m_pszItem_TI;}
		void		GetLastItemID_NODE_T3_USED (m4pcchar_t &ai_pszId_Node_T3_Used){ai_pszId_Node_T3_Used=m_oItemActual.m_pszItem_Node_T3;}
		m4bool_t 	GetLastItemIs_Inherit (void){return m_oItemActual.m_bIsInherited;}
		m4return_t	SetAccess			(ClAccess * ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel);
		m4bool_t	Is_Nihil			(void) { return M4_FALSE; }
		m4bool_t	Is_TICH				(void) { return M4_TRUE;}
		m4bool_t	Is_JIT				(void) { return M4_FALSE;}
		void		Reset				(void);
		m4bool_t	IsAliasOrMyT3		(m4char_t *ai_Channel);
		
		//que regla estamos compilando
		m4return_t  GetCurrentRuleInfo( m4char_t *ai_pszChannel, m4uint32_t ai_iSizeChannel, m4char_t *ai_pszTi,m4uint32_t ai_iSizeTi, m4char_t *ai_pszItem, m4uint32_t ai_iSizeItem, m4char_t *ai_pszRule,m4uint32_t ai_iSizeRule);

	private:
		m4return_t  FindMethod            ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister);
		m4return_t  FindItem            ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister, ClAccess *ai_paccess = NULL,m4bool_t ai_bLookInAlias = M4_TRUE);
		m4return_t  FindParentItem      ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister);

		m4return_t  _FindItem ( m4pchar_t ai_pcTi, m4pchar_t ai_pcItem, ClRegister * ao_pRegister, ClAccess *ai_paccess = NULL);
//RRRR
		m4return_t  _FindItemLevel2	( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister);
		m4return_t  _FindNodeLevel2 ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode );
		m4return_t  _FindSystemMethod ( m4pchar_t ai_pcItem, ClRegister * ao_pRegister );
//RRRR
		m4return_t  _FindChannel ( m4pchar_t &aio_pcChannel, m4pchar_t ai_pcNode, m4pchar_t &ao_pcTI, m4bool_t ai_bLookInAlias = M4_TRUE);
		m4return_t  _FindNode ( m4pchar_t ai_pcNode, m4pchar_t &ao_rpcTI, m4pchar_t &ao_rpcNodeTI, ClRegisterIndex &ao_riNode);
		// Busca, teniendo en cuanta que la TI puede ser padre de alguna de las que hay en el canal.
		m4return_t  _FindExtendedTIOrInherited (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI, ClRegisterIndex ai_iNode);
		// Busqueda de la Ti en el canal.
		m4return_t  _FindExtendedTI (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI, ClRegisterIndex ai_iNode);
		m4return_t  _FindTI (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI);
		
		m4bool_t	_CheckNodalItem(ClRegister * ao_pRegister);

		// Canal dinamico para compilaciones de herencia.
		m4bool_t		_IsInheritedRule( void ) ;
		ClCompiledMCR	*GetDynamicMCR(void);
		m4return_t		CreateDynamicMetaChannel(void);
		m4return_t		FillDynamicChannel(ClAccess *ai_pAccessm2);
		m4return_t		GetT3InheritanceLevelByIdT3(m4pcchar_t pT3S,m4VariantType &vNodeLevel);
		m4return_t		CleanTIs(ClAccess *ai_pm2Access);

		m4return_t  SetItemSystemRecordSet (void);
		m4return_t	SetHandles (void);

		ClAccess *	GetpAccessItemChan	(void);

		StItemInfo		m_oItemActual;

		ClAccess *		m_oAccessFind;
		SystemItems_t	m_oSystemItems;

		ClCompiledMCR *	m_pm2MetaChannel;

		m4uint8_t		m_iChanType;

		m4uint32_t		m_iNodeT3s,
						m_iNodeNodes, 
						m_iNodeTis, 
						m_iNodeItems, 
						m_iNodeRules, 
						m_iNodeTiAlias, 
						m_iNodeTiAliasRes, 
						m_iNodet3Alias,
						m_iItemIdT3,
						m_iItemOld_TI,
						m_iItemID_TI,
						m_iNodeT3s_Inherit,
						m_iNodeNodes_Inherit,
						m_iNodeTis_Inherit,
						m_iNodeAliasRes;


		// IdT3 en el que esta definido el Item.
		m4pchar_t		m_pIdT3;
		// IdT3 que estamos compilando.
		m4pcchar_t		m_pThisIdT3;

		// Item,Ti y Nodo actuales. Solo tiene valor en el caso de herencia.
		m4pcchar_t		m_pCurrentIdItem;
		m4pcchar_t		m_pCurrentIdTiLevel;
		m4pcchar_t		m_pCurrentIdNode;
// Para la nueva forma de busqueda.
		ClAccess *		m_poAccessItems;
		m4bool_t		m_bTIAliasFound;	// Indica si hemos encontrado Alias de TI. 
											// Se utiliza en el caso de que hay q eu cambiar de OLD_ID_TI a ID_TI
};


class ClFinder_JIT	:public ClFinder

{
	public:
		ClFinder_JIT(ClTrad *ai_poTrad):ClFinder(ai_poTrad)	{
			m_hCurrentItem=0;
			m_iRuleLevel=0;
		}
		m4bool_t    IsItem              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem);
		m4bool_t    IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsRule              (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4char_t *ai_Rule);
		m4bool_t    IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule);
		m4return_t	SetValue(m4pchar_t ai_pszItemId, m4pchar_t ai_pszValue, m4bool_t ai_bChangeStatus);
		m4return_t  SetValue(m4pchar_t ai_pszItemId, m4double_t ai_dNumber, m4bool_t ai_bChangeStatus);
		m4return_t	SetAccess			(ClAccess * ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel){
			m_poAccess = ai_poAccess;
			m_hCurrentItem = ai_hItem ;
			m_iRuleLevel = ai_iRuleLevel ;
			return M4_SUCCESS;
		};
		m4bool_t	Is_Nihil			(void) { return M4_FALSE; }
		m4bool_t	Is_TICH				(void) { return M4_FALSE;}
		m4bool_t	Is_JIT				(void) { return M4_TRUE;}
		m4bool_t	IsAliasOrMyT3		(m4char_t *ai_Channel);


	private:
		m4return_t  FindItem            ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, StItemInfoJIT * ao_pStItem);
		m4return_t  FindParentItem      ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, StItemInfoJIT * ao_pStItem);

		StItemInfoJIT	m_oItemActual;
		m4uint32_t		m_hCurrentItem ;
		m4uint8_t		m_iRuleLevel ;
};

#endif
