

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                finder.cpp   
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
//    Item Finder used with TI_CHANNEL and DESIGN_CHANNEL
//
//
//==============================================================================

#include <stdlib.h>
#include <search.h>

#include "finder.hpp"

#include "executor.hpp"
#include "vmres.hpp"
#include "cpdefine.hpp"
#include "trad.h"
#include "vmachin2.h"

#include "bookmark.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "m4dmcommon.hpp"
// Para crear el canaldinamico
#include "m4mdfac.hpp"
#include "m4facdef.hpp"
#include "m4buildversion.hpp"



/* Bug 0090300
Función que quita los # iniciales
*/
extern m4pcchar_t	M4SkipHash( m4pcchar_t ai_pccString ) ;



// Declare StSortRegister structure
typedef struct StSortRegister_tag {
	m4double_t  m_iIndex;			// Define Index por el que se Ordena
	m4uint32_t  m_iPosition;		// Define Position asociada al Index
} StSortRegister_t;

//Sort function to StSortRegister structure
int SortRegisterCompare( const void *ai_pstElem1, const void *ai_pstElem2 ) {
	return (int)( ((StSortRegister_t*)ai_pstElem1)->m_iIndex - ((StSortRegister_t*)ai_pstElem2)->m_iIndex );
}


SystemItems_tag::~SystemItems_tag (){
	if ( poAccessSystemItems ){
		poAccessSystemItems->Destroy();
	}
}

void SystemItems_tag::SetRecordSet (m4uint32_t ai_handle){
	M4_ASSERT (ai_handle);
	poRSSystemItems = &(poAccessSystemItems->Node[(ClHandle)ai_handle].RecordSet );
}



ClFinder_TICH::~ClFinder_TICH(void){
	Reset();
}

void ClFinder_TICH::Reset(){
	if (m_oAccessFind){
		m_oAccessFind->Destroy();
		m_oAccessFind = 0;
	}

	if (m_oSystemItems.poAccessSystemItems){
		m_oSystemItems.poAccessSystemItems->Destroy();
		m_oSystemItems.poAccessSystemItems = 0;
	}
	m_oSystemItems.poRSSystemItems = 0;

	m_poAccessItems = 0;  // No le hacemos Destroy porque viene de Nivel 2 y no es nuestro

	m_poAccess = 0;
	m_pIdT3=0;
	m_pThisIdT3=0;
	m_iItemIdT3=M4DM_EOF_INDEX;
	
	// Borramos el metacanal de herencia.
	if (m_pm2MetaChannel){
		m_pm2MetaChannel->Destroy();
		m_pm2MetaChannel=0;
	}
}

//--------------------------
//IsItem
//--------------------------
m4bool_t    ClFinder_TICH::IsItem	(m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem)
{
	ClRegister oRegItem (m_oAccessFind);
	m4double_t *pdType;
	//pasamos a mayusculas
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);

	//***********************************************************
	//OJO. Por ahora, devuelve TRUE si es Item y no es método.
    if ( FindItem (ai_Channel, ai_Node, ai_Item, &oRegItem ) == M4_SUCCESS){
        ao_RealItem = (m4pcchar_t) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();
        pdType = ( ( m4double_t * ) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE].Value.Get() );

        if (pdType == 0){
            g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Item type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
            m_poTrad->SetError();
            return M4_FALSE;
        }
		if (_CheckNodalItem(&oRegItem)!=M4_TRUE){
			m_poTrad->SetError();
			return M4_FALSE;
		}

        if ( *pdType != M4CL_ITEM_TYPE_METHOD) {
			// Se va a tomar como variable. Lo sacamos para que no se traduzca.
			return M4_TRUE;
        }
		m_poTrad->DeleteItemSynonym(ai_Item,M4_TRUE);
    }
	return M4_FALSE;
}

//--------------------------
//IsMethodItem
//--------------------------

m4bool_t    ClFinder_TICH::IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem)
{
	ClRegister oRegItem (m_oAccessFind);
    m4VariantType vVarArg;
    m4int32_t   i;
    m4double_t *pdType, *pdArg;

	//pasamos a mayusculas
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);
		
    if ( FindMethod (ai_Channel, ai_Node, ai_Item, &oRegItem ) != M4_SUCCESS){
	    ao_iLevel=0;
        return M4_FALSE;
    }
    ao_pcRealItem = (m4pcchar_t) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();

    pdType = ( ( m4double_t * ) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE].Value.Get() );
    if (pdType == 0){
		g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Item type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
        m_poTrad->SetError();
        return M4_FALSE;
    }

    if ( *pdType== M4CL_ITEM_TYPE_METHOD ||	 *pdType== M4CL_ITEM_TYPE_CONCEPT )	{
		m4uint32_t iHandle ;
		ClAccess *pAccess=oRegItem.GetpAccess();
		M4_ASSERT (pAccess);
        ClRecordSet oRecordSetArgs (pAccess);
		ClCompiledMCR * poCMCR;

		//Obtenemos el CMCR
		// El Metacanal, lo vamos a obtener del registro, porque puede ser de otro canal.
		poCMCR = oRegItem. GetpChannel()->GetpCMCR();
		iHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEM_ARGS, M4_TRUE ) ;

		if( iHandle == NULL ){
			g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_ITEM_ARGS<<M4Obj(poCMCR)<<EndLog;
            m_poTrad->SetError();
            ao_iLevel=0;
			return( M4_FALSE ) ;
		}

		if (_CheckNodalItem(&oRegItem)!=M4_TRUE){
			m_poTrad->SetError();
            ao_iLevel=0;
			return M4_FALSE;
		}

        oRecordSetArgs.SetCurrentBlock(oRegItem.GetChildBlock( ClHandle(iHandle)));
		// Colocamos el Access en el nodo de argumentos.
		pAccess->Node [ ClHandle(iHandle) ];
		m4uint8_t iNumArgs = (m4int8_t)oRecordSetArgs.Register.Count() ;
		//Crear vector temporal para ordenar a partir de los Registros
		StSortRegister_t* pstSortReg = new StSortRegister_t[iNumArgs];
		if (pstSortReg == NULL) {
			m_poTrad->SetError();
			return M4_FALSE;
		}

        m4double_t *pdTemp;
		m4bool_t	bDeleted=M4_FALSE;
		m4uint8_t	iNumDeleted=0;

		for (i = 0; i<iNumArgs; ++i) {
			//Comprobar si esta borrado
			if ( oRecordSetArgs.Register[i].IsDeleted(bDeleted) != M4_SUCCESS ) {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, ai_Node<<ai_Channel<<"IsMethodItem" );
				m_poTrad->SetError();
				delete []pstSortReg;
				return M4_FALSE;
			}
			if (bDeleted)
				++iNumDeleted;	//Contamos los argumentos borrados
			else {
				pdTemp = ( m4pdouble_t ) ( oRecordSetArgs.Register[i].Item[M4_ITEM_SCHTIS_ITEM_POSITION].Value.Get() );
				if (pdTemp == 0){
					g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Argument position"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
					m_poTrad->SetError();
					delete []pstSortReg;
					return M4_FALSE;
				}
				pstSortReg[i-iNumDeleted].m_iIndex = *pdTemp;
				pstSortReg[i-iNumDeleted].m_iPosition = i;
			}
		}

		ao_iRealArgs = iNumArgs-iNumDeleted;
		if (ao_iRealArgs>0) {
			//Ordenar vector
			qsort( (void *)pstSortReg, (size_t)ao_iRealArgs, sizeof(StSortRegister_t), SortRegisterCompare );
		}
		
        //cadena que describe los args
        for (i = 0; i< ao_iRealArgs; ++i) {
            pdArg=( (m4double_t *) oRecordSetArgs. Register[pstSortReg[i].m_iPosition].Item[M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT_TYPE].Value.Get() ); 
            if (pdArg == 0){
				g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Argument Type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
                m_poTrad->SetError();
				delete []pstSortReg;
                return M4_FALSE;
            }

            if ( *pdArg == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ) {
                ao_acArgDes[i]='r';
            }
            else{
                ao_acArgDes[i]='R';
            }
		}

		delete []pstSortReg;


        //si el número de args es variable lo multiplicamos por -1
        oRegItem.Item[M4_ITEM_SCHTIS_VARIABLE_ARGS].Value.Get(vVarArg);

		if( vVarArg.Type==M4CL_CPP_TYPE_NULL ){
			g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Variable Args"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
            m_poTrad->SetError();
            ao_iLevel=0;
            return( M4_FALSE ) ;
		}

		if( vVarArg.Type != M4CL_CPP_TYPE_NUMBER ){
			g_oChLog<<BeginError(M4_CH_VM_ITEM_TYPE_MISMATCH_COMP)<<M4_ITEM_SCHTIS_VARIABLE_ARGS<<M4_NODE_SCHTIS_ITEMS<<poCMCR->GetChannelId()<<EndLog;
            m_poTrad->SetError();
            ao_iLevel=0;
            return( M4_FALSE ) ;
		}

        if ( int(vVarArg.Data.DoubleData)){
            ao_iRealArgs=(ao_iRealArgs?-ao_iRealArgs:-(M4CL_MAX_PARAM+1)) ;
        }

		//NIvel->Si lleva nombre de canal->2 sino ->1
		if (strcmp(ai_Channel,"")==0 || ai_Channel==0){
			ao_iLevel=1;
		}
		else{
			ao_iLevel=2;
		}
		//	Miramos si es Inherit, para poder ver si es del sistema ( para las dependencias de ejecucion ).
		//	En caso de que no exista el nodo de Items, es porque es un metodo de nivel 2, que pertence al canal EXTENDED_ITEMS,
		//	y no puede ser del sistema (Dejamos m_bIsInherited a FALSE)
		m4uint32_t NItems=poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEMS, M4_TRUE );
		if (NItems)	{
			m4double_t * pIs_Inherited = (m4double_t*)oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_IS_INHERIT_ITEM].Value.Get();
			if (pIs_Inherited)
				m_oItemActual.m_bIsInherited = (*pIs_Inherited)?M4_TRUE:M4_FALSE;
		}


	}
	else{
        ao_iLevel=0;
		// Por si es una funcion. Lo sacamos para que no se traduzca.
		// Podria haber un item propiedad que se llamara asi.
		m_poTrad->DeleteItemSynonym(ai_Item,M4_TRUE);
		return M4_FALSE;
	}

	return M4_TRUE;

}

//--------------------------
//IsRule
//--------------------------

m4bool_t    ClFinder_TICH::IsRule              (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4char_t *ai_Rule)
{
	ClRegister oRegItem (m_oAccessFind);
    m4pcchar_t pcTemp1;
	//pasamos a mayusculas
	ai_Rule=strupr(ai_Rule);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	

	if ( FindMethod (ai_Channel, ai_Node, ai_Item, &oRegItem ) != M4_SUCCESS)
		return M4_FALSE;

	m4uint32_t numRows ;

	ClRegisterIndex i(0);
	ClCompiledMCR * poCMCR;

	// El Metacanal, lo vamos a obtener del registro, porque puede ser de otro canal.
	poCMCR = oRegItem. GetpChannel()->GetpCMCR();
	
	if (_CheckNodalItem(&oRegItem)!=M4_TRUE){
		m_poTrad->SetError();
		return M4_FALSE;
	}
	ClAccess *pAccess = oRegItem.GetpAccess();
	M4_ASSERT (pAccess);

	ClRecordSet oRecordSetRules (pAccess);

    oRecordSetRules.SetCurrentBlock(oRegItem.GetChildBlock(ClHandle(m_iNodeRules))) ;
	// Colocamos el access en el nodo de rules.
	pAccess->Node [ ClHandle(m_iNodeRules) ];
    numRows = oRecordSetRules.Register.Count() ;
	for ( i = 0; i<numRows; ++i){
        pcTemp1 = (m4pcchar_t) oRecordSetRules.Register[i].Item[M4_ITEM_SCHTIS_RULES_ID_RULE].Value.Get();
        if (pcTemp1== 0){
			g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"ID_RULE"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
            m_poTrad->SetError();
            return M4_FALSE;
        }

		if ( stricmp( pcTemp1, ai_Rule ) == 0 )	{
			//	Miramos si es Inherit, para poder ver si es del sistema ( para las dependencias de ejecucion ).
			//	En caso de que no exista el nodo de Items, es porque es un metodo de nivel 2, que pertence al canal EXTENDED_ITEMS,
			//	y no puede ser del sistema (Dejamos m_bIsInherited a FALSE)
			m4uint32_t NItems=poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEMS, M4_TRUE );
			if (NItems)	{
				m4double_t * pIs_Inherited = (m4double_t*)oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_IS_INHERIT_ITEM].Value.Get();
				if (pIs_Inherited)
					m_oItemActual.m_bIsInherited = (*pIs_Inherited)?M4_TRUE:M4_FALSE;
			}
			return M4_TRUE;
		}
	}
			
	return M4_FALSE;
}


m4bool_t    ClFinder_TICH::IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem)
{
	return M4_FALSE;
/* ya no se puede acceder a los métodos del padre
	ClRegister oRegItem (m_oAccessFind);
    m4VariantType vVarArg;
    m4int32_t   i;
    m4double_t *pdType, *pdArg;

	//pasamos a mayusculas
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);
		
	//Encontramoes el Item Padre.
	if ( FindParentItem (ai_Channel, ai_Node, ai_Item, &oRegItem ) != M4_SUCCESS){
		return M4_FALSE;
	}
    ao_pcRealItem = (m4pcchar_t) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();

    pdType = ( ( m4double_t * ) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE].Value.Get() );
    if (pdType == 0){
		g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Item type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
        m_poTrad->SetError();
        return M4_FALSE;
    }

    if ( *pdType== M4CL_ITEM_TYPE_METHOD ||	 *pdType == M4CL_ITEM_TYPE_CONCEPT ){
		m4uint32_t iHandle ;
		ClAccess *pAccess=oRegItem.GetpAccess();
		M4_ASSERT (pAccess);
        ClRecordSet oRecordSetArgs (pAccess);
		ClCompiledMCR * poCMCR;

		// El Metacanal, lo vamos a obtener del registro, porque puede ser de otro canal.
		poCMCR = oRegItem.GetpChannel()->GetpCMCR();
	
		iHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEM_ARGS, M4_TRUE ) ;

		if( iHandle == NULL ){
			g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_ITEM_ARGS<<M4Obj(poCMCR)<<EndLog;
            m_poTrad->SetError();
			return( M4_FALSE ) ;
		}
        
		if (_CheckNodalItem(&oRegItem)!=M4_TRUE){
			m_poTrad->SetError();
			return M4_FALSE;
		}

        oRecordSetArgs.SetCurrentBlock(oRegItem.GetChildBlock( ClHandle(iHandle)));
		//Colocamos el access en el node de argumentos
		pAccess->Node [ ClHandle(iHandle) ];
		m4uint8_t	iNumArgs = (m4int8_t)oRecordSetArgs.Register.Count() ;
		//Crear vector temporal para ordenar a partir de los Registros
		StSortRegister_t* pstSortReg = new StSortRegister_t[iNumArgs];
		if (pstSortReg == NULL) {
			//DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "IsFatherMethod" << __FILE__ << __LINE__ );
			m_poTrad->SetError();
			return M4_FALSE;
		}

		m4bool_t	bDeleted=M4_FALSE;
		m4uint8_t	iNumDeleted=0;

        for (i = 0; i<iNumArgs; ++i){
			//Comprobar si esta borrado
			if ( oRecordSetArgs.Register[i].IsDeleted(bDeleted) != M4_SUCCESS ) {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, ai_Node<<ai_Channel<<"IsFatherMethod" );
				m_poTrad->SetError();
				delete []pstSortReg;
				return M4_FALSE;
			}
			if (bDeleted){
				++iNumDeleted;	//Contamos los argumentos borrados
			}
			else {
				pdArg=( (m4double_t *) oRecordSetArgs.Register[i].Item[M4_ITEM_SCHTIS_ITEM_POSITION].Value.Get() ); 
				if (pdArg == 0){
					g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Argument type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
					m_poTrad->SetError();
					delete []pstSortReg;
					return M4_FALSE;
				}
				pstSortReg[i-iNumDeleted].m_iIndex = *pdArg;
				pstSortReg[i-iNumDeleted].m_iPosition = i;
			}
		}
		ao_iRealArgs = iNumArgs-iNumDeleted;
		if (ao_iRealArgs>0) {
			//Ordenar vector
			qsort( (void *)pstSortReg, (size_t)ao_iRealArgs, sizeof(StSortRegister_t), SortRegisterCompare );
		}

        //cadena que describe los args
        for (i = 0; i<iNumArgs; ++i){
            pdArg=( (m4double_t *) oRecordSetArgs.Register[i].Item[M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT_TYPE].Value.Get() ); 
            if (pdArg == 0){
				g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Argument type"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
                m_poTrad->SetError();
				delete []pstSortReg;
                return M4_FALSE;
            }
            if ( *pdArg == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ) {
                ao_acArgDes[i]='r';
            }
            else{
                ao_acArgDes[i]='R';
            }
		}

		delete []pstSortReg;

        //si el número de args es variable lo multiplicamos por -1
        oRegItem.Item[M4_ITEM_SCHTIS_VARIABLE_ARGS].Value.Get(vVarArg);
		if( vVarArg.Type==M4CL_CPP_TYPE_NULL){
			g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Variable Args"<<ai_Item<<ai_Node<<ai_Channel<<EndLog;
            m_poTrad->SetError();
            ao_iLevel=0;
            return( M4_FALSE ) ;
		}
		if( vVarArg.Type != M4CL_CPP_TYPE_NUMBER ){
			g_oChLog<<BeginError(M4_CH_VM_ITEM_TYPE_MISMATCH_COMP)<<M4_ITEM_SCHTIS_VARIABLE_ARGS<<M4_NODE_SCHTIS_ITEMS<<poCMCR->GetChannelId()<<EndLog;
            m_poTrad->SetError();
            ao_iLevel=0;
            return( M4_FALSE ) ;
		}

        if ( int(vVarArg.Data.DoubleData)){
            ao_iRealArgs=(ao_iRealArgs?-ao_iRealArgs:-(M4CL_MAX_PARAM+1)) ;
        }

		if (strcmp(ai_Channel,"")==0 || ai_Channel==0){
			ao_iLevel=1;
		}
		else{
			ao_iLevel=2;
		}
		//	Miramos si es Inherit, para poder ver si es del sistema ( para las dependencias de ejecucion ).
		//	En caso de que no exista el nodo de Items, es porque es un metodo de nivel 2, que pertence al canal EXTENDED_ITEMS,
		//	y no puede ser del sistema (Dejamos m_bIsInherited a FALSE)
		m4uint32_t NItems=poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEMS, M4_TRUE );
		if (NItems){
			m4double_t * pIs_Inherited = (m4double_t*)oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_IS_INHERIT_ITEM].Value.Get();
			if (pIs_Inherited)
				m_oItemActual.m_bIsInherited = (*pIs_Inherited)?M4_TRUE:M4_FALSE;
		}

	}
	else{
		ao_iLevel=0;	//?? por simetria con IsMethodItem
		return M4_FALSE;
	}

	return M4_TRUE;
*/
}

m4bool_t    ClFinder_TICH::IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule)
{

	return M4_FALSE;
	/*ya no se puede acceder a los métodos del padre

	ClRegister oRegItem (m_oAccessFind);

	//pasamos a mayusculas
	ai_Rule=strupr(ai_Rule);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	

	if ( FindParentItem (ai_Channel, ai_Node, ai_Item, &oRegItem ) != M4_SUCCESS){
		return M4_FALSE;
	}

	m4uint32_t  numRows ;

	ClRegisterIndex i(0);
	ClCompiledMCR * poCMCR;
	// El Metacanal, lo vamos a obtener del registro, porque puede ser de otro canal.
	poCMCR = oRegItem. GetpChannel()->GetpCMCR();
	
	if (_CheckNodalItem(&oRegItem)!=M4_TRUE){
		m_poTrad->SetError();
		return M4_FALSE;
	}
	ClAccess *pAccess=oRegItem.GetpAccess();
	M4_ASSERT (pAccess);

    ClRecordSet oRecordSetRules (pAccess);

    oRecordSetRules.SetCurrentBlock(oRegItem.GetChildBlock( ClHandle(m_iNodeRules))) ;
	//colocamos el acess en el nodo de rules
	pAccess->Node [ ClHandle(m_iNodeRules) ];
    numRows = oRecordSetRules.Register.Count() ;
	for ( i = 0; i<numRows; ++i)
	{
		if ( stricmp( (m4pcchar_t) oRecordSetRules.Register[i].Item[M4_ITEM_SCHTIS_RULES_ID_RULE].Value.Get(), 
				        ai_Rule ) == 0 ){
			//	Miramos si es Inherit, para poder ver si es del sistema ( para las dependencias de ejecucion ).
			//	En caso de que no exista el nodo de Items, es porque es un metodo de nivel 2, que pertence al canal EXTENDED_ITEMS,
			//	y no puede ser del sistema (Dejamos m_bIsInherited a FALSE)
			m4uint32_t NItems=poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEMS, M4_TRUE );
			if (NItems)	{
				m4double_t * pIs_Inherited = (m4double_t*)oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_IS_INHERIT_ITEM].Value.Get();
				if (pIs_Inherited)
					m_oItemActual.m_bIsInherited = (*pIs_Inherited)?M4_TRUE:M4_FALSE;
			}
			return M4_TRUE;
		}
	}
			
	return M4_FALSE;

*/
}



//Busqueda Binaria
m4return_t __BSearch( ClAccessRecordSet & ai_RecSet, m4pchar_t ai_pcVal, m4int32_t ai_Index)
{
	m4return_t ret;
	m4VariantType   vSearch ;

	vSearch.Type = M4CL_CPP_TYPE_STRING_VAR;

	ret = ai_RecSet.SetCurrentSearchIndexById(ai_Index); 

	if (ret == M4_ERROR){
		return M4_ERROR;
	}

	vSearch.Data.PointerChar = ai_pcVal;
	ai_RecSet.Register.Begin();
	ai_RecSet.Register.Find (&vSearch);

	return M4_SUCCESS;
}

//--------------------------
//FindMethod
//--------------------------

m4return_t  ClFinder_TICH::FindMethod ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcTI=0;
	m4pchar_t pcNodeTI=0;
	ClRegisterIndex iNode=M4DM_EOF_INDEX;


	m_oItemActual.m_pszItem_TI=0;
	m_oItemActual.m_pszItem_T3=0;
	m_oItemActual.m_pszItem_Node_T3=0;
	m_oItemActual.m_bIsInherited=M4_FALSE;


	if (m_poAccess == NULL){
		g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	if (m_oAccessFind == NULL){
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_oAccessFind->Follow(*m_poAccess);

	ClCompiledMCR * poCMCR = m_poAccess->GetpCMCR();

	if ((ai_pcChannel != NULL) && (ai_pcChannel[0] != '\0')){
		//Comprobamos la existencia del canal y TI
		ret = _FindChannel(ai_pcChannel, ai_pcNode, pcTI);
		
		if (ret == M4_ERROR){
			// En caso de que no lo encuentre en el nodo de T3, lo buscamos primero como metodo del sistema.
			// Si es un metodo del sistema buscamos el nodo.
			// Si no lo buscamos como item de nivel 2 en el canal nuevo.
			if (ai_pcChannel==0){
				// En este caso no ha encontrado ningun alias que coincida con el nombre.
				return M4_ERROR;
			}

			if ( _FindSystemMethod ( ai_pcItem, ao_pRegister ) == M4_SUCCESS ){
				// Buscamos el nodo. Si no existe devolvemos error.
				return _FindNodeLevel2 ( ai_pcChannel, ai_pcNode ) ;
			}
			else {
				// No es un metodo de sistema. Lo buscamos en el nuevo canal.
				return _FindItemLevel2 ( ai_pcChannel, ai_pcNode, ai_pcItem, ao_pRegister);
			}
		}

		if (pcTI){
			ret = _FindTI(pcTI, pcTI);
			m_bTIAliasFound = M4_TRUE;

			if (ret == M4_ERROR){
				return M4_ERROR;
			}
		}
	}
	else if ((ai_pcNode != NULL) && (ai_pcNode[0] != '\0'))	{	
		//Comprobamos la existencia del Nodo y TI
		ret = _FindNode(ai_pcNode, pcTI, pcNodeTI, iNode);
		
		if (ret == M4_ERROR){
			return M4_ERROR;
		}

		if (pcTI && pcNodeTI){
			ret = _FindExtendedTIOrInherited(pcTI, pcNodeTI, iNode);
	
			if (ret == M4_ERROR){
				return M4_ERROR;
			}
		}
	}

	//Buscamos en los items del sistema.
	ret = _FindSystemMethod ( ai_pcItem, ao_pRegister );

	if ( ret != M4_SUCCESS ){		
		ret = _FindItem(pcTI, ai_pcItem, ao_pRegister);
	}

	return ret;
}



//--------------------------
//FindParentItem
//--------------------------

m4return_t  ClFinder_TICH::FindParentItem      ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister)
{
	// De momento busca solo en los items del sistema
	return _FindSystemMethod ( ai_pcItem, ao_pRegister );

}

//--------------------------
//SetArguments
//--------------------------

m4return_t	ClFinder_TICH::SetArguments( void )
{
	ClRegister oRegItem (m_oAccessFind);
	m4uint8_t iNumArgs;

	ClRegisterIndex i(0);
    m4VariantType vType, vVarArg;

	m4return_t ret=M4_SUCCESS;
    m4char_t    *pszItemName;
    m4double_t  *pdType, *pdRef;
	

    pszItemName = ( (m4char_t *) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULES_ID_ITEM].Value.Get() );
    if (pszItemName==NULL || (strcmp(pszItemName,"")==0) ){
		g_oChLog<<BeginError(M4_CH_VM_NO_ITEM_NAME_COMPILING)<<EndLog;
		m_poTrad->SetError();
        return M4_ERROR;
   
    }

	ret=FindItem ("", "", pszItemName, &oRegItem);
    if (ret!=M4_SUCCESS){
		g_oChLog<<BeginError(M4_CH_VM_COMP_ITEM_NOT_FOUND_COMPILING)<<pszItemName<<EndLog;
		m_poTrad->SetError();
        return M4_ERROR;
   
    }
	
    
    //comprobamos si es de tipo LN4  (si no es ->salimoes)
    m_poRecordSet->Register[m_oRegIndex].Item[M4CL_VM_COMPILE_ITEM_LANGUAGE].Value.Get(vType);
    
    if (vType.Type == M4CL_CPP_TYPE_NULL ){ //si no tienen puesto el tipo de la regla
		g_oChLog<<BeginError(M4_CH_VM_WRONG_LANGUAGE)<< "NULL"<<pszItemName<<EndLog;
        return M4_ERROR; //retornamos error
    }

    if (vType.Data.DoubleData !=M4CL_LENGUAGE_LN4) {
        //un metodo que no LN4 no se puede compilar. No damos error, simplemente retornamos OK
        return M4_SUCCESS;
    }

	oRegItem.Item[M4_ITEM_SCHTIS_VARIABLE_ARGS].Value.Get(vVarArg);

	if( vVarArg.Type==M4CL_CPP_TYPE_NULL ){
		g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Variable Args"<<pszItemName<<""<<""<<EndLog;
        m_poTrad->SetError();
        return( M4_ERROR ) ;

	}
	if( vVarArg.Type != M4CL_CPP_TYPE_NUMBER ){
		g_oChLog<<BeginError(M4_CH_VM_ITEM_TYPE_MISMATCH_COMP)<<M4_ITEM_SCHTIS_VARIABLE_ARGS<<M4_NODE_SCHTIS_ITEMS<<m_poAccess->GetpCMCR()->GetChannelId()<<EndLog;
        m_poTrad->SetError();
        return( M4_ERROR ) ;

	}

    pdType = ( ( m4double_t * ) oRegItem.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE].Value.Get() );
    if (pdType == 0){
		g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Item type"<<pszItemName<<""<<""<<EndLog;
		m_poTrad->SetError();
        return M4_ERROR;
    }

	if ( *pdType == M4CL_ITEM_TYPE_METHOD || *pdType == M4CL_ITEM_TYPE_CONCEPT ){

		m4uint32_t iHandle ;
		ClAccess *pAccess=oRegItem.GetpAccess();
		M4_ASSERT (pAccess);
 
        ClRecordSet oRSArgs (pAccess);
		ClCompiledMCR * poCMCR;

		//Obtenemos el CMCR
		poCMCR = m_poAccess->GetpCMCR() ;
	
		iHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEM_ARGS, M4_TRUE ) ;

		if( iHandle == NULL ){
			g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_ITEM_ARGS<<M4Obj(poCMCR)<<EndLog;
			m_poTrad->SetError();
			return( M4_ERROR ) ;
		}

        oRSArgs.SetCurrentBlock(oRegItem.GetChildBlock ( ClHandle(iHandle) ));
		iNumArgs = (m4int8_t)oRSArgs.Register.Count() ;	//Contamos (incluidos los borrados)

		//Crear vector temporal para ordenar a partir de los Registros
		StSortRegister_t* pstSortReg = new StSortRegister_t[iNumArgs];
		if (pstSortReg == NULL) {
			//DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "SetArguments" << __FILE__ << __LINE__ );
			return M4_ERROR;
		}
		m4pchar_t * ppszArguments = new m4pchar_t [ iNumArgs ];//Lista Argumentos
		if (ppszArguments == NULL) {
			delete [] pstSortReg;
			//DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "SetArguments" << __FILE__ << __LINE__ );
			return M4_ERROR;
		}
		m4bool_t  * pbReference = new m4bool_t [ iNumArgs ]; //M4_TRUE si es por referencia
		if (pbReference == NULL) {
			delete [] pstSortReg;
			delete [] ppszArguments;
			//DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "SetArguments" << __FILE__ << __LINE__ );
			return M4_ERROR;
		}

        m4double_t *pdTemp;
		m4bool_t	bDeleted=M4_FALSE;
		m4uint8_t	iNumDeleted=0;

		for (i = 0; i<iNumArgs; ++i) {
		   //Comprobar si esta borrado
		   if ( oRSArgs.Register[i].IsDeleted(bDeleted) != M4_SUCCESS ) {
			   //DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, ai_Node<<ai_Channel<<"IsMethodItem" );
		       ret = M4_ERROR;
			   break; //salimos del for
		   }
		   if (bDeleted){
			   ++iNumDeleted;	//Contamos los argumentos borrados
		   }
		   else {
			   pdTemp = ( m4pdouble_t ) ( oRSArgs.Register[i].Item[M4_ITEM_SCHTIS_ITEM_POSITION].Value.Get() );
			   if (pdTemp == 0){
				   g_oChLog<<BeginError(M4_CH_VM_WRONG_ARGUMENT_COMP)<<"position"<< pszItemName <<EndLog;
				   m_poTrad->SetError();
				   //Liberamos memoria previamente reservada
				   ret = M4_ERROR;
				   break; //salimos del for
			   }
			   pstSortReg[i-iNumDeleted].m_iIndex = *pdTemp;
			   pstSortReg[i-iNumDeleted].m_iPosition = i;
		   }
			   
		}
		
		if (ret==M4_SUCCESS){
			m4uint8_t iNumRealArgs = iNumArgs-iNumDeleted;
			//Ordenar vector
			if (iNumRealArgs>0){
				qsort( (void *)pstSortReg, (size_t)iNumRealArgs, sizeof(StSortRegister_t), SortRegisterCompare );
			}
			
			//Obtener items de forma ordenada (sin contar los borrados)
			for (i = 0; i<iNumRealArgs; ++i) {
			
				ppszArguments [ i ] = ( m4pchar_t ) oRSArgs.Register[pstSortReg[i].m_iPosition].Item[M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT].Value.Get();
				if (ppszArguments [ i ] == 0){
					g_oChLog<<BeginError(M4_CH_VM_WRONG_ARGUMENT_COMP)<<"Id"<< pszItemName <<EndLog;
					m_poTrad->SetError();
					ret=M4_ERROR;
					break ; //salimos del for
				}

				pdRef=((m4double_t *) oRSArgs.Register[pstSortReg[i].m_iPosition].Item[M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT_TYPE].Value.Get() );
				if (pdRef  == 0){
		            g_oChLog<<BeginError(M4_CH_VM_WRONG_ARGUMENT_COMP)<<"type"<< pszItemName <<EndLog;
					m_poTrad->SetError();
					ret = M4_ERROR;
					break ; //salimos del for
				}
				pbReference [ i ] = ( ( *pdRef == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ) ? M4_TRUE : M4_FALSE );

			} //del for

			//establecer argumentos
			if (ret!=M4_ERROR){
				//ret = m_poTrad->SetArguments( ppszArguments, pbReference, iNumArgs, (vVarArg.Data.DoubleData == 0 ? M4_FALSE : M4_TRUE));
				ret = m_poTrad->SetArguments( ppszArguments, pbReference, iNumRealArgs, (vVarArg.Data.DoubleData == 0 ? M4_FALSE : M4_TRUE));
			}

		}

		//fredom
		delete [] pstSortReg;
		delete [] ppszArguments;
		delete [] pbReference;
		
		return ret;
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_BAD_CURRENT_ITEM_TYPE_COMPILING, pszItemName);
		m_poTrad->SetError();
		return M4_ERROR;
	}

	return M4_SUCCESS;
	

}

//--------------------------
//GetLN4Code
//--------------------------

m4return_t ClFinder_TICH::GetLN4Code(m4pchar_t &ao_pszFormula)
{

	m4return_t ret = M4_SUCCESS;

    m4VariantType vType;

	m4uint32_t iItemHandle;

	ClCompiledMCR * poCMCR;

	
	if ((m_poAccess == NULL) || ( !m_poRecordSet->IsValid() ) ){
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	//Obtenemos el CMCR
    poCMCR = m_poAccess->GetpCMCR() ;


	//comprobamos si es de tipo LN4  (si no es ->salimos)
    m_poRecordSet->Register[m_oRegIndex].Item[M4CL_VM_COMPILE_ITEM_LANGUAGE].Value.Get(vType);

    if (vType.Type == M4CL_CPP_TYPE_NULL ){ //si no tienen puesto el tipo de la regla
		g_oChLog<<BeginError(M4_CH_VM_WRONG_LANGUAGE)<< "NULL"<<"JIT code"<<EndLog;
        return M4_ERROR; //retornamos error
    }

    if (vType.Data.DoubleData != M4CL_LENGUAGE_LN4) return M4_WARNING; // Porque no hay codigo LN4.
    
	///// Comprobamos que el TEMP_CODE sea de tipo campo. ( si no es que no está actualizado el usuario y se haría como antes ).
	///// También compruebo que exista el TEMP_CODE.
	m4bool_t tempcode = M4_FALSE;

	iItemHandle = poCMCR->GetNodeItemHandleById ( m_iNodeRules , M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE, M4_TRUE );

	if ( strcmp ( poCMCR->GetItemWriteFieldId( iItemHandle ), "" ) != 0 ){
		// En caso de que el Temp_Code tenga WF, nos han copiado el codigo alli.
		ao_pszFormula = (m4pchar_t) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE].Value.Get(); 
		tempcode = M4_TRUE ;
	}
	else  {
		g_oChLog<<BeginDebug(M4_CH_VM_WRONG_ITEM_WRITE_FIELD)<<M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE<<poCMCR->GetNodeId(m_iNodeRules)<<poCMCR->GetChannelId()<<EndLog;
	}

	if (!M4_BOOL(tempcode)){
		ao_pszFormula = (m4pchar_t) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE].Value.Get(); 
	}

	return ret;
}

//--------------------------
//GetCurrentRuleInfo
//--------------------------

m4return_t ClFinder_TICH::GetCurrentRuleInfo( m4char_t *ai_pszChannel, m4uint32_t ai_iSizeChannel, m4char_t *ai_pszTi,m4uint32_t ai_iSizeTi, m4char_t *ai_pszItem, m4uint32_t ai_iSizeItem, m4char_t *ai_pszRule,m4uint32_t ai_iSizeRule)
{

	m4return_t ret = M4_SUCCESS;
	m4char_t *pszTemp;

	
	if ((m_poAccess == NULL) || ( !m_poRecordSet->IsValid() ) ){
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	//leemos canal, ti, item, rule
	
	//rule: M4_NODE_SCHTIS_RULES.M4_ITEM_SCHTIS_RULES_ID_RULE
	pszTemp = ( (m4char_t *) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULES_ID_RULE].Value.Get() );
	if (pszTemp){
		strncpy(ai_pszRule,pszTemp,ai_iSizeRule);
		ai_pszRule[ai_iSizeRule]=0;
	}

	//item M4_NODE_SCHTIS_RULES.M4_ITEM_SCHTIS_RULES_ID_ITEM
	pszTemp = ( (m4char_t *) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULES_ID_ITEM].Value.Get() );
	if (pszTemp){
		strncpy(ai_pszItem,pszTemp,ai_iSizeItem);
		ai_pszItem[ai_iSizeItem]=0;
	}
	
	
	//ti M4_NODE_SCHTIS_RULES.M4_ITEM_SCHTIS_RULES_ID_TI
	
	pszTemp = ( (m4char_t *) m_poRecordSet->Register[m_oRegIndex].Item[M4_ITEM_SCHTIS_RULES_ID_TI].Value.Get() );
	if (pszTemp){
		strncpy(ai_pszTi,pszTemp,ai_iSizeTi);
		ai_pszTi[ai_iSizeTi]=0;
	}



	//t3  :M4_NODE_SCHTIS_T3S.M4_ITEM_SCHTIS_T3S_ID_T3
	ClNode *poNodeT3s = &(m_poAccess->Node[ (ClHandle)m_iNodeT3s ]) ;
	pszTemp = ( (m4char_t *) poNodeT3s->RecordSet.Current.Item[M4_ITEM_SCHTIS_T3S_ID_T3].Value.Get() );
	if (pszTemp){
		strncpy(ai_pszChannel,pszTemp,ai_iSizeChannel);
		ai_pszChannel[ai_iSizeChannel]=0;
	}

	return M4_SUCCESS;
}

//--------------------------
//Set value en el design channel (texto)
//--------------------------

m4return_t	ClFinder_TICH::SetValue(m4pchar_t ai_pszItemId, m4pchar_t ai_pszValue, m4bool_t ai_bChangeStatus)
{
	m4return_t ret=M4_ERROR;
    m4VariantType vValue;

	if (m_poRecordSet->IsValid()){

		vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vValue.Data.PointerChar = ai_pszValue ;

		if( ai_bChangeStatus == M4_TRUE )
		{
			ret=m_poRecordSet->Register[m_oRegIndex].Item[ai_pszItemId].Value.Set(vValue);
		}
		else
		{
			ret=m_poRecordSet->Register[m_oRegIndex].Item[ai_pszItemId].Value.SetNoChangeStatus(vValue);
		}

		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SET_VALUE_COMPILING, ai_pszValue<<ai_pszItemId);
		}
	}
	return ret;
}

//--------------------------
//SetValue en el design channel
//--------------------------

m4return_t	ClFinder_TICH::SetValue(m4pchar_t ai_pszItemId, m4double_t ai_pszValue, m4bool_t ai_bChangeStatus)
{

	m4return_t ret=M4_ERROR;
    m4VariantType vValue;
	ClCompiledMCR * poCMCR = m_poAccess->GetpCMCR();

	if ( poCMCR->GetNodeItemHandleById(poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES, M4_TRUE),ai_pszItemId, M4_TRUE)){
		if (m_poRecordSet->IsValid()){

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = ai_pszValue ;

			if( ai_bChangeStatus == M4_TRUE )
			{
				ret=m_poRecordSet->Register[m_oRegIndex].Item[ai_pszItemId].Value.Set(vValue);
			}
			else
			{
				ret=m_poRecordSet->Register[m_oRegIndex].Item[ai_pszItemId].Value.SetNoChangeStatus(vValue);
			}
			if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SET_VALUE_COMPILING, ai_pszValue<<ai_pszItemId);
			}
		}
	}

	return ret;
}

//--------------------------
//SetAccess
//--------------------------

m4return_t ClFinder_TICH::SetAccess (ClAccess *ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel)
{
	m4return_t ret;

	//Comprobamos si ha cambiado el canal.
	if (m_poAccess){
		if (m_poAccess->GetpChannel() == ai_poAccess->GetpChannel()){
			// No hace falta acutalizar nada mas.
			m_poAccess = ai_poAccess;
			return M4_SUCCESS;
		}
	}

	m_poAccess = ai_poAccess;

	if (m_oAccessFind){
		m_oAccessFind->Destroy();
	}

	m_oAccessFind = ai_poAccess->GetpChannel()->CreateAccess ();

	// Otro Access más ( y van 4)
	// Para tenerlo quieto en los metodos del sistema
	if (m_oSystemItems.poAccessSystemItems){
		m_oSystemItems.poAccessSystemItems->Destroy();
	}

	m_oSystemItems.poAccessSystemItems = ai_poAccess->GetpChannel()->CreateAccess () ;
	m_oSystemItems.poRSSystemItems = 0;




	if (strcmpi(m_poAccess->GetpCMCR()->GetChannelId(), M4_CHANNEL_SCHT3S_TI_CHANNEL) != 0){
		m_iChanType = M4CL_FINDER_CHANNEL_DESIGN_CHANNEL;
	}
	else{
		m_iChanType = M4CL_FINDER_CHANNEL_TI_CHANNEL;
	}

	ret = SetHandles();

	if (ret == M4_ERROR){
		return M4_ERROR;
	}
	// Me quedo con el ID_NODE para el que esta definido y el ID_T3.
	if (m_iItemIdT3!=M4DM_EOF_INDEX){
		ClNode * pNItems = m_poAccess->Node.Get((ClHandle)m_iNodeItems);
		if (pNItems){
			m_pIdT3=(m4pchar_t)pNItems->RecordSet.Current.Item[(ClItemIndex)m_iItemIdT3].Value.Get();
		}
	}
	m_pThisIdT3=(m4pcchar_t)m_poAccess->Node[(ClHandle)m_iNodeT3s].RecordSet.Current.Item[M4_ITEM_SCHTIS_T3S_ID_T3].Value.Get();


	// Se llamara bajo demanda.
/*	if (CreateDynamicChannel()!=M4_SUCCESS){
		return M4_ERROR;
	}
*/	return ret;
}

//--------------------------
//SetHandles
//--------------------------

m4return_t ClFinder_TICH::SetHandles (void)
{
	ClCompiledMCR * poCMCR = m_poAccess->GetpCMCR() ;


	m_iNodeT3s = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_T3S, M4_TRUE ) ;

	if( m_iNodeT3s == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_T3S<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_iNodeT3s_Inherit =  poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_T3S_INHERITED, M4_TRUE ) ;
/*	if( m_iNodeT3s_Inherit  == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_T3S_INHERITED<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
*/	
	m_iNodeNodes = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_NODES, M4_TRUE ) ;

	if( m_iNodeNodes == 0 )	{
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_NODES<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_iNodeNodes_Inherit = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_NODES_INHERIT, M4_TRUE ) ;

/*	if( m_iNodeNodes_Inherit == 0 )	{
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_NODES_INHERIT<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
*/
	m_iNodeTis = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_TIS, M4_TRUE ) ;

	if( m_iNodeTis == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_TIS<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_iNodeTis_Inherit =  poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_TIS_INHERITED, M4_TRUE ) ;
/*	if( m_iNodeTis_Inherit == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_TIS_INHERITED<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
*/
	m_iNodeAliasRes = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ALIAS_RES, M4_TRUE );
/*	if( m_iNodeAliasRes == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_ALIAS_RES<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
*/
	m4uint32_t htmp  = poCMCR->GetNodeItemHandleById( m_iNodeTis ,M4_ITEM_SCHTIS_TIS_ID_TI_OLD ,M4_TRUE);
	if (htmp){
		m_iItemOld_TI = poCMCR->GetItemIndex(htmp);
	}
	htmp  = poCMCR->GetNodeItemHandleById( m_iNodeTis ,M4_ITEM_SCHTIS_TIS_ID_TI,M4_TRUE);
	if (htmp){
		m_iItemID_TI = poCMCR->GetItemIndex(htmp);
	}

	m_iNodeItems = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_ITEMS, M4_TRUE ) ;

	if( m_iNodeItems == 0 )	{
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_ITEMS<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_iNodeRules = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_RULES, M4_TRUE ) ;

	if( m_iNodeRules == NULL ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_RULES<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_FALSE ) ;
	}

	m_iNodeTiAlias = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_TI_ALIAS, M4_TRUE ) ;

	if( m_iNodeTiAlias == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_TI_ALIAS<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
	
	m_iNodeTiAliasRes = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_TI_ALIAS_RES, M4_TRUE ) ;

	if( m_iNodeTiAliasRes == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_TI_ALIAS_RES<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}
	
	m_iNodet3Alias = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_T3_ALIAS, M4_TRUE ) ;

	if( m_iNodet3Alias == 0 ){
		g_oChLog<<BeginError(M4_CH_VM_NO_SUCH_NODE_COMP)<<M4_NODE_SCHTIS_T3_ALIAS<<M4Obj(poCMCR)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	htmp = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_ID_T3,M4_TRUE);
	if (htmp){
		m_iItemIdT3= poCMCR->GetItemIndex(htmp);
	}

	return M4_SUCCESS;
}


/*
Esta función comprueba si la regla es heredada o no.
Para ello comprueba varios campos del design channel.
*/
m4bool_t	ClFinder_TICH::_IsInheritedRule( void )
{

	m4return_t		iResult ;
	m4VariantType	vValue ;


	ClRegister	&oRegisterRules = m_poAccess->Node[ (ClHandle)m_iNodeRules ].RecordSet.Register ;

	/*
	Caso sencillo
	*/
	// Si existe el item RESOLVE_ALIAS se le hace caso
	oRegisterRules.Item[ M4_ITEM_SCHTIS_RULES_RESOLVE_ALIAS ] ;

	if( oRegisterRules.Item.GetIndex() != M4DM_EOF_INDEX )
	{
		iResult = oRegisterRules.Item.Value.Get( vValue ) ;

		if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1 )
		{
			return( M4_TRUE ) ;
		}
	}

	/*
	Caso avanzado
	m4pcchar_t		pccRuleTiId ;
	m4pcchar_t		pccTiNivelId ;

	// Si existe el item RUNING_INHERITANCE y es un 1 es que estamos heredando
	oRegisterRules.Item[ "RUNING_INHERITANCE" ] ;

	if( oRegisterRules.Item.GetIndex() != M4DM_EOF_INDEX )
	{
		iResult = oRegisterRules.Item.Value.Get( vValue ) ;

		if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1 )
		{
			return( M4_TRUE ) ;
		}
	}

	// Si ID_RULE_TI y ID_TI_NIVEL son distintas es heredada
	oRegisterRules.Item[ "ID_RULE_TI" ] ;

	if( oRegisterRules.Item.GetIndex() != M4DM_EOF_INDEX )
	{
		iResult = oRegisterRules.Item.Value.Get( vValue ) ;

		if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
		{
			pccRuleTiId = vValue.Data.PointerChar ;

			oRegisterRules.Item[ "ID_TI_NIVEL" ] ;

			if( oRegisterRules.Item.GetIndex() != M4DM_EOF_INDEX )
			{
				iResult = oRegisterRules.Item.Value.Get( vValue ) ;

				if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
				{
					pccTiNivelId = vValue.Data.PointerChar ;

					if( strcmpi( pccRuleTiId, pccTiNivelId ) != 0 )
					{
						return( M4_TRUE ) ;
					}
				}
			}
		}
	}

	// Si el nodo es heredado y no sobreescrito también es regla heredada
	ClRegister	&oRegisterNodes = m_poAccess->Node[ (ClHandle)m_iNodeNodes ].RecordSet.Register ;

	oRegisterNodes.Item[ "IS_INHERITED_NODE" ] ;

	if( oRegisterNodes.Item.GetIndex() != M4DM_EOF_INDEX )
	{
		iResult = oRegisterNodes.Item.Value.Get( vValue ) ;

		if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1 )
		{
			oRegisterNodes.Item[ "IS_OVERWRITTEN_NODE" ] ;

			if( oRegisterNodes.Item.GetIndex() != M4DM_EOF_INDEX )
			{
				iResult = oRegisterNodes.Item.Value.Get( vValue ) ;

				if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 0 )
				{
					return( M4_TRUE ) ;
				}
			}
		}
	}
	*/

	return( M4_FALSE ) ;
}


ClCompiledMCR	*ClFinder_TICH::GetDynamicMCR(void){
	if (m_pm2MetaChannel==0){
		if (CreateDynamicMetaChannel()!=M4_SUCCESS){
			return 0;
		}
	}
	return m_pm2MetaChannel;
}

m4return_t ClFinder_TICH::CreateDynamicMetaChannel(void){
	// Levantamos el canal.
	
	ClChannel	*pchannelm2=0;
	ClCMCRFactory	*poFactory = NULL ;

	// Comprobamos que esten los nodos que vamos a necesitar.
	if (m_iNodeT3s_Inherit==0||m_iNodeNodes_Inherit==0||m_iNodeTis_Inherit==0||m_iNodeAliasRes==0){
		return M4_ERROR;
	}

	if (m_oAccessFind->GetpChannel()->GetpChannelManager()->CreateChannel(pchannelm2)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (pchannelm2->BuildAsMetachannel()!=M4_SUCCESS){
		pchannelm2->Destroy();
		return M4_ERROR;
	}
	ClAccess *pAccessm2=0;
	pAccessm2=pchannelm2->CreateAccess();
	if (pAccessm2==0){
		pchannelm2->Destroy();
		return M4_ERROR;
	}

	if (FillDynamicChannel(pAccessm2)!=M4_SUCCESS){
		pAccessm2->Destroy();
		pchannelm2->Destroy();
		return M4_ERROR;
	}

	// Creamos un nuevo metacanal y lo levantamos desde el m2.

	poFactory = m_oAccessFind->GetpChannel()->GetpChannelManager()->GetpFactory() ;

	if( poFactory == NULL )
	{
		pAccessm2->Destroy();
		pchannelm2->Destroy();
		return M4_ERROR;
	}

    m_pm2MetaChannel = new ClCompiledMCR;

	if( m_pm2MetaChannel == NULL )
	{
		pAccessm2->Destroy();
		pchannelm2->Destroy();
		return M4_ERROR;
	}


	if (poFactory->MDFac_BuildFromAccess( pAccessm2, M4CL_CSTYPE_BOTH, M4_LANG_DEFAULT_ID, 0, M4_FALSE, m_pm2MetaChannel )!=M4_SUCCESS){
		pAccessm2->Destroy();
		pchannelm2->Destroy();
		m_pm2MetaChannel->Destroy();
		m_pm2MetaChannel=0;
		return M4_ERROR;
	}


	pAccessm2->Destroy();
	pchannelm2->Destroy();
	return M4_SUCCESS;
}

m4return_t ClFinder_TICH::FillDynamicChannel(ClAccess *ai_pAccessm2){
	if (ai_pAccessm2==0){
		return M4_ERROR;
	}
	// Vamos rellenando los distintos nodos.

//////////////////////////////
// T3S.
	m4pcchar_t pT3S=0;
	// Nos posicionamos en el DESIGN_CHANNEL. Nodo de T3S.
	ClRegister_Current &RegCurrentT3=m_oAccessFind->Node[(ClHandle)m_iNodeT3s].RecordSet.Current;
	pT3S = (m4pcchar_t)RegCurrentT3.Item[M4_ITEM_SCHTIS_T3S_ID_T3].Value.Get();
	if (pT3S==0){
		return M4_ERROR;
	}
	// Ponemos el valor en el m2.
	ClRegister_Current &m2RegCurrentT3 = ai_pAccessm2->Node[M4MDFAC_NODE_SYS_T3S].RecordSet.Current;
	m2RegCurrentT3.Add();
	m2RegCurrentT3.Item[M4MDFAC_T3S_ID_T3].Value.Set(pT3S,strlen(pT3S)+1);

//////////////////////////////
// T3S_INHERIT.
	// Nos posicionamos.Nodo de T3S_INHERIT.
	ClRegister_Current &RegCurrentT3S_Inherit=m_oAccessFind->Node[(ClHandle)m_iNodeT3s_Inherit].RecordSet.Current;
	ClRegister_Current &m2RegCurrentT3S_Inherit=ai_pAccessm2->Node[M4MDFAC_NODE_SYS_T3S_INHERIT].RecordSet.Current;

	
	// Vamos copiando los registros, excepto el de nivel 0.
	m4VariantType	vT3Level;
	m4pcchar_t pT3_Base=0;
	
	for (RegCurrentT3S_Inherit.Begin(); RegCurrentT3S_Inherit.Index != M4DM_EOF_INDEX; RegCurrentT3S_Inherit.Next()){
		// Leemos el nivel 
		RegCurrentT3S_Inherit.Item[M4_ITEM_SCHTIS_T3S_ID_LEVEL].Value.Get(vT3Level);
		if (vT3Level.Type==M4CL_CPP_TYPE_NUMBER&&vT3Level.Data.DoubleData!=0){
			// Si es distinto de 0, copiamos el registro al m2.
			m2RegCurrentT3S_Inherit.Add();
			m2RegCurrentT3S_Inherit.Item[M4MDFAC_T3_INHERIT_ID_LEVEL].Value.Set(vT3Level);
			pT3_Base = (m4pcchar_t)RegCurrentT3S_Inherit.Item[M4_ITEM_SCHTIS_T3S_ID_T3_BASE].Value.Get();
			if (pT3_Base==0){
				return M4_ERROR;
			}
			m2RegCurrentT3S_Inherit.Item[M4MDFAC_T3_INHERIT_ID_T3_BASE].Value.Set(pT3_Base,strlen(pT3_Base)+1);
		}
	}

//////////////////////////////
// NODES, NODES_INHERITED, TIS, TIS_INHERITED, ALIAS_RES.
	ClRegister_Current &RegCurrentNodes=			m_oAccessFind->Node[(ClHandle)m_iNodeNodes].RecordSet.Current;
	ClRegister_Current &RegCurrentNodes_Inherited=	m_oAccessFind->Node[(ClHandle)m_iNodeNodes_Inherit].RecordSet.Current;
	ClRegister_Current &RegCurrentTis=				m_oAccessFind->Node[(ClHandle)m_iNodeTis].RecordSet.Current;
	ClRegister_Current &RegCurrentTis_Inherited=	m_oAccessFind->Node[(ClHandle)m_iNodeTis_Inherit].RecordSet.Current;
	ClRegister_Current &RegCurrentAliasRes=			m_oAccessFind->Node[(ClHandle)m_iNodeAliasRes].RecordSet.Current;

	ClRegister_Current &m2RegCurrentNodes=			ai_pAccessm2->Node[M4MDFAC_NODE_SYS_NODES].RecordSet.Current;
	ClRegister_Current &m2RegCurrentNodes_Inherited=ai_pAccessm2->Node[M4MDFAC_NODE_SYS_NODES_INHERIT].RecordSet.Current;
	ClRegister_Current &m2RegCurrentTis=			ai_pAccessm2->Node[M4MDFAC_NODE_SYS_TIS].RecordSet.Current;
	ClRegister_Current &m2RegCurrentTis_Inherited=	ai_pAccessm2->Node[M4MDFAC_NODE_SYS_TIS_INHERIT].RecordSet.Current;
	ClRegister_Current &m2RegCurrentAliasRes=		ai_pAccessm2->Node[M4MDFAC_NODE_SYS_ALIAS_RES].RecordSet.Current;

	// Segun nos vayamos moviendo por los nodos actualizamos todo lo que cuelga de ellos.
	for (RegCurrentNodes.Begin(); RegCurrentNodes.Index != M4DM_EOF_INDEX; RegCurrentNodes.Next()){

		// Para cada nodo actualizamos : 
		// SYS_NODES:			ID_NODE		ID_TI		ID_LEVEL-->Lo obtenemos del nodo de T3S_INHERIT.
		// SYS_NODES_INHERITED: ID_NODE		ID_TI		ID_LEVEL-->Lo obtenemos del nodo de T3S_INHERIT con el ID_NODE_T3 de cada registro.
		// SYS_TIS:				ID_TI		
		// SYS_TIS_INHERITED:	ID_TI		ID_TI_BASE	ID_LEVEL
		// SYS_ALIAS_RES:		ID_NODE		ID_ALIAS	ID_NODE_USED

		// Y lo leemos de :
		// NODES:				ID_NODE		ID_TI		ID_NODE_T3-->ID_LEVEL
		// NODES_INHERITED:		ID_NODE		ID_TI		ID_NODE_T3-->ID_LEVEL
		// TIS					ID_TI								-->Realmente lo copiamos todo del nodo hijo.
		// TIS_INHERITED		ID_TI		ID_TI_BASE	ID_LEVEL
		// ALIAS_RES			ID_NODE		ID_ALIAS	ID_NODE_USED

		//////////////////////////////
		// SYS_NODES.
		m4pcchar_t	pNodesIdNode=0;
		m4pcchar_t	pNodesIdTi=0;
		m4pcchar_t	pNodesIdNodeT3=0;	// T3 donde se definio el nodo. Sirve para obtener el nivel.
		m4VariantType	vNodeLevel;

		// Obtenemos los valores.
		pNodesIdNode=(m4pcchar_t)RegCurrentNodes.Item[M4_ITEM_SCHTIS_NODES_ID_NODE].Value.Get();
		pNodesIdTi=(m4pcchar_t)RegCurrentNodes.Item[M4_ITEM_SCHTIS_NODES_ID_TI].Value.Get();
		pNodesIdNodeT3=(m4pcchar_t)RegCurrentNodes.Item[M4_ITEM_SCHTIS_NODES_ID_T3].Value.Get();
		if (pNodesIdNode==0||pNodesIdTi==0||pNodesIdNodeT3==0){
			return M4_ERROR;
		}
		if (GetT3InheritanceLevelByIdT3(pNodesIdNodeT3,vNodeLevel)!=M4_SUCCESS){
			return M4_ERROR;
		}
		// Los volcamos en el m2.
		m2RegCurrentNodes.Add();
		m2RegCurrentNodes.Item[M4MDFAC_NODES_ID_NODE].Value.Set(pNodesIdNode,strlen(pNodesIdNode)+1);
		m2RegCurrentNodes.Item[M4MDFAC_NODES_ID_TI].Value.Set(pNodesIdTi,strlen(pNodesIdTi)+1);
		m2RegCurrentNodes.Item[M4MDFAC_NODES_ID_LEVEL].Value.Set(vNodeLevel);

/*		//////////////////////////////
		// SYS_NODES_INHERIT.
		m4pcchar_t	pNodesInheritedIdNode=0;
		m4pcchar_t	pNodesInheritedIdTi=0;
		m4pcchar_t	pNodesInheritedIdT3=0;	// T3 donde se definio el nodo. Sirve para excluir
		m4pcchar_t	pNodesInheritedIdNodeT3=0;	// T3 donde se definio la sobreescritura de nodo. Sirve para obtener el nivel
		m4VariantType	vNodeInheritedLevel;
		
		// Lo hacemos para todas las sobreescrituras de cada nodo.
		for (RegCurrentNodes_Inherited.Begin();RegCurrentNodes_Inherited.Index.IsNotEOF();RegCurrentNodes_Inherited.Next()){

			pNodesInheritedIdT3=(m4pcchar_t)RegCurrentNodes_Inherited.Item[M4_ITEM_SCHTIS_NODES_INHERIT_ID_T3].Value.Get();
			pNodesInheritedIdNodeT3=(m4pcchar_t)RegCurrentNodes_Inherited.Item[M4_ITEM_SCHTIS_NODES_INHERIT_ID_NODE_T3].Value.Get();

			if (pNodesInheritedIdT3==0||pNodesInheritedIdNodeT3==0){
				return M4_ERROR;
			}
			
			if( strcmpi( pNodesInheritedIdT3, pNodesInheritedIdNodeT3 ) != 0 ){
				pNodesInheritedIdNode=(m4pcchar_t)RegCurrentNodes_Inherited.Item[M4_ITEM_SCHTIS_NODES_INHERIT_ID_NODE].Value.Get();
				pNodesInheritedIdTi=(m4pcchar_t)RegCurrentNodes_Inherited.Item[M4_ITEM_SCHTIS_NODES_INHERIT_ID_TI].Value.Get();
				if (pNodesInheritedIdNode==0||pNodesInheritedIdTi==0){
					return M4_ERROR;
				}

				if (GetT3InheritanceLevelByIdT3(pNodesInheritedIdNodeT3,vNodeInheritedLevel)!=M4_SUCCESS){
					return M4_ERROR;
				}

				m2RegCurrentNodes_Inherited.Add();
				m2RegCurrentNodes_Inherited.Item[M4MDFAC_NODE_INHERIT_ID_NODE].Value.Set(pNodesInheritedIdNode,strlen(pNodesInheritedIdNode)+1);
				m2RegCurrentNodes_Inherited.Item[M4MDFAC_NODE_INHERIT_ID_TI].Value.Set(pNodesInheritedIdTi,strlen(pNodesInheritedIdTi)+1);
				m2RegCurrentNodes_Inherited.Item[M4MDFAC_NODE_INHERIT_ID_LEVEL].Value.Set(vNodeInheritedLevel);
			}
		}
*/		
		//////////////////////////////
		// SYS_TIS.
		// Facilito. Solo hay un registro
		RegCurrentTis.Begin();
			//////////////////////////////
			// SYS_TIS_INHERITED
			m4pcchar_t	pTisInheritedIdTi=0;
			m4pcchar_t	pTisInheritedIdTiBase=0;
			m4VariantType	vTiInheritedLevel;
			m4VariantType	vTiInheritType;

			// Recorremos el bloque copiando todas excepto las de nivel 0. Luego habra que pasar limpiando las que esten repetidas.
			for (RegCurrentTis_Inherited.Begin(); RegCurrentTis_Inherited.Index != M4DM_EOF_INDEX; RegCurrentTis_Inherited.Next()){
				RegCurrentTis_Inherited.Item[M4_ITEM_SCHTIS_TIS_INHERIT_ID_LEVEL].Value.Get(vTiInheritedLevel);
				pTisInheritedIdTiBase=(m4pcchar_t)RegCurrentTis_Inherited.Item[M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI_BASE].Value.Get();
				if (pTisInheritedIdTiBase==0){
					return M4_ERROR;
				}

				// Copiamos al de TIs
				m2RegCurrentTis.Add();
				m2RegCurrentTis.Item[M4MDFAC_TIS_ID_TI].Value.Set(pTisInheritedIdTiBase,strlen(pTisInheritedIdTiBase)+1);
				// Le ponemos que no derive los items del sistema para que vaya más rápido
				vTiInheritType.Type = M4CL_CPP_TYPE_NUMBER ;
				vTiInheritType.Data.DoubleData = M4CL_TI_INHERIT_TYPE_NONE ;
				m2RegCurrentTis.Item[M4MDFAC_TIS_ID_INHERIT_TYPE].Value.Set(vTiInheritType);

				if (vTiInheritedLevel.Type==M4CL_CPP_TYPE_NUMBER&&vTiInheritedLevel.Data.DoubleData!=0){
					// Si el nivel es != 0, lo volcamos. Sino no hay que volcarlo, porque jesus los tiene metidos por defecto.
					pTisInheritedIdTi=(m4pcchar_t)RegCurrentTis_Inherited.Item[M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI].Value.Get();
					if (pTisInheritedIdTi==0){
						return M4_ERROR;
					}
					// Copiamos al de TIS_INHERITED	
					m2RegCurrentTis_Inherited.Add();
					m2RegCurrentTis_Inherited.Item[M4MDFAC_TIS_INHERIT_ID_TI].Value.Set(pTisInheritedIdTi,strlen(pTisInheritedIdTi)+1);
					m2RegCurrentTis_Inherited.Item[M4MDFAC_TIS_INHERIT_ID_TI_BASE].Value.Set(pTisInheritedIdTiBase,strlen(pTisInheritedIdTiBase)+1);
					m2RegCurrentTis_Inherited.Item[M4MDFAC_TIS_INHERIT_ID_LEVEL].Value.Set(vTiInheritedLevel);
				}
			}

		//////////////////////////////
		// SYS_ALIAS_RES
		//m4pcchar_t	pAliasResIdNode=0;
		m4pcchar_t	pAliasResIdAlias=0;
		m4pcchar_t	pAliasResIdNodeUsed=0;
		// Recorremos el bloque copiando todas las resoluciones de Alias.
		for (RegCurrentAliasRes.Begin(); RegCurrentAliasRes.Index != M4DM_EOF_INDEX; RegCurrentAliasRes.Next()){
			//Esta conectado.-->pNodesIdNode
			//pAliasResIdNode=(m4pcchar_t)RegCurrentAliasRes.Item[M4_ITEM_SCHTIS_ALIAS_RES_ID_NODE].Value.Get();
			pAliasResIdAlias=(m4pcchar_t)RegCurrentAliasRes.Item[M4_ITEM_SCHTIS_ALIAS_RES_ID_ALIAS].Value.Get();
			pAliasResIdNodeUsed=(m4pcchar_t)RegCurrentAliasRes.Item[M4_ITEM_SCHTIS_ALIAS_RES_ID_NODE_USED].Value.Get();
			if (pAliasResIdAlias==0||pAliasResIdNodeUsed==0){
				return M4_ERROR;
			}

			m2RegCurrentAliasRes.Add();
			m2RegCurrentAliasRes.Item[M4MDFAC_ALIAS_RES_ID_NODE].Value.Set(pNodesIdNode,strlen(pNodesIdNode)+1);
			m2RegCurrentAliasRes.Item[M4MDFAC_ALIAS_RES_ALIAS].Value.Set(pAliasResIdAlias,strlen(pAliasResIdAlias)+1);
			m2RegCurrentAliasRes.Item[M4MDFAC_ALIAS_RES_ID_NODE_USED].Value.Set(pAliasResIdNodeUsed,strlen(pAliasResIdNodeUsed)+1);

		}
	}

//////////////////////////////
// ITEMS.
	// Ponemos el item que estamos compilando.
	// m_poAccess esta posicionado en el item que estamos compilando.
	// Para todos los demas hmeos utilizado m_poAccessFind, con el que nos podemos mover.
	ClRegister_Current &RegCurrentItems=m_poAccess->Node[(ClHandle)m_iNodeItems].RecordSet.Current;
	ClRegister_Current &m2RegCurrentItems=			ai_pAccessm2->Node[M4MDFAC_NODE_SYS_ITEMS].RecordSet.Current;
	
	m_pCurrentIdItem=(m4pcchar_t)RegCurrentItems.Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();
	m_pCurrentIdTiLevel=(m4pcchar_t)RegCurrentItems.Item[M4_ITEM_SCHTIS_ITEMS_ID_TI].Value.Get();
	if (m_pCurrentIdItem==0||m_pCurrentIdTiLevel==0){
		return M4_ERROR;
	}

	m2RegCurrentItems.Add();
	m2RegCurrentItems.Item[M4MDFAC_ITEMS_ID_TI].Value.Set(m_pCurrentIdTiLevel,strlen(m_pCurrentIdTiLevel)+1);
	m2RegCurrentItems.Item[M4MDFAC_ITEMS_ID_ITEM].Value.Set(m_pCurrentIdItem,strlen(m_pCurrentIdItem)+1);

	// Actualizamos el nodo actual.
	m_pCurrentIdNode=(m4pcchar_t)m_poAccess->Node[(ClHandle)m_iNodeNodes].RecordSet.Current.Item[M4_ITEM_SCHTIS_NODES_ID_NODE].Value.Get();

// Limpiamos las TIs que estuvieran repetidas.
	if (CleanTIs(ai_pAccessm2)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

m4return_t ClFinder_TICH::GetT3InheritanceLevelByIdT3(m4pcchar_t ai_pT3S,m4VariantType &ao_vLevel){
	// Buscamos en el nodo T3_INHERITANCE, a ver si encontramos el nivel.
	ClRegister_Current &RegCurrentT3S_Inherit=m_oAccessFind->Node[(ClHandle)m_iNodeT3s_Inherit].RecordSet.Current;
	m4pcchar_t	pT3Base=0;
	for (RegCurrentT3S_Inherit.Begin(); RegCurrentT3S_Inherit.Index != M4DM_EOF_INDEX; RegCurrentT3S_Inherit.Next()){
		pT3Base=(m4pcchar_t)RegCurrentT3S_Inherit.Item[M4_ITEM_SCHTIS_T3S_ID_T3_BASE].Value.Get();
		if (strcmpi(pT3Base,ai_pT3S)==0){
			// Encontrado.
			RegCurrentT3S_Inherit.Item[M4_ITEM_SCHTIS_T3S_ID_LEVEL].Value.Get(ao_vLevel);
			return M4_SUCCESS;
		}
	}
	// No estaba el T3
	ao_vLevel.SetNull();
	return M4_ERROR;
}

m4return_t ClFinder_TICH::CleanTIs(ClAccess *ai_pAccessm2){
	// La Pk de cada registro es ID_TI.
	m4pcchar_t pTisLastId_Ti="";
	m4pcchar_t pTisCurrentId_Ti=0;
	m4int32_t i=0;
	
	ClAccessRecordSet &RSTis=ai_pAccessm2->Node[M4MDFAC_NODE_SYS_TIS].RecordSet;
	ClAccessRecordSet &RSTis_Inherited=ai_pAccessm2->Node[M4MDFAC_NODE_SYS_TIS_INHERIT].RecordSet;
	
	ClRegister_Current &m2RegCurrentTis=RSTis.Current;
	ClRegister_Current &m2RegCurrentTis_Inherited=RSTis_Inherited.Current;
//////////////////////////////
// TIS.
	// Ordenamos por ID_TI.
	if (RSTis.SetCurrentSearchIndexById(1)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (RSTis.Arrange()!=M4_SUCCESS){
		return M4_ERROR;
	}
	m2RegCurrentTis.Begin();
	for (i=0; m2RegCurrentTis.Index != M4DM_EOF_INDEX; m2RegCurrentTis.MoveTo(i)){
		pTisCurrentId_Ti=(m4pcchar_t)m2RegCurrentTis.Item[M4MDFAC_TIS_ID_TI].Value.Get();
		if (strcmpi(pTisCurrentId_Ti,pTisLastId_Ti)==0){
			//Son iguales. Lo borro.
			m2RegCurrentTis.Destroy();
		}
		else{
			pTisLastId_Ti=pTisCurrentId_Ti;
			++i;
		}
	}
//////////////////////////////
// TIS_INHERIT
	m4pcchar_t pTisInheritLastId_Ti="";
	m4pcchar_t pTisInheritCurrentId_Ti=0;
	m4pcchar_t pTisInheritLastId_Ti_Base="";
	m4pcchar_t pTisInheritCurrentId_Ti_Base=0;

	if (RSTis_Inherited.SetCurrentSearchIndexById(1)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (RSTis_Inherited.Arrange()!=M4_SUCCESS){
		return M4_ERROR;
	}
	m2RegCurrentTis_Inherited.Begin();
	for (i=0; m2RegCurrentTis_Inherited.Index != M4DM_EOF_INDEX; m2RegCurrentTis_Inherited.MoveTo(i)){
		pTisInheritCurrentId_Ti=(m4pcchar_t)m2RegCurrentTis_Inherited.Item[M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI].Value.Get();
		pTisInheritCurrentId_Ti_Base=(m4pcchar_t)m2RegCurrentTis_Inherited.Item[M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI_BASE].Value.Get();
		if (strcmpi(pTisInheritCurrentId_Ti,pTisInheritLastId_Ti)==0 && strcmpi(pTisInheritCurrentId_Ti_Base,pTisInheritLastId_Ti_Base)==0 ){
			//Son iguales. Lo borro.
			m2RegCurrentTis_Inherited.Destroy();
		}
		else{
			pTisInheritLastId_Ti=pTisInheritCurrentId_Ti;
			pTisInheritLastId_Ti_Base=pTisInheritCurrentId_Ti_Base;
			++i;
		}
	}


	return M4_SUCCESS;
}

//--------------------------
//SetItemSystemRecordSet
//--------------------------

m4return_t ClFinder_TICH::SetItemSystemRecordSet (void)
{
	m4return_t ret;

	if (m_iChanType == M4CL_FINDER_CHANNEL_TI_CHANNEL){
		ret = FindItem ( "", M4_TIS_SCH_TI_SYSTEM, "", NULL, m_oSystemItems.poAccessSystemItems );

		if (ret != M4_SUCCESS){
			g_oChLog<<BeginError(M4_CH_VM_TI_NOT_FOUND_COMPILING)<<M4_TIS_SCH_TI_SYSTEM<<EndLog;
			m_poTrad->SetError();
		}
	}
	else{		
		ret = FindItem ( M4_CHANNEL_SCHT3S_T3_SYSTEM, M4_NODE_SCHT3SYS_NODE_SYSTEM, NULL, NULL, m_oSystemItems.poAccessSystemItems,M4_FALSE);

		if (ret != M4_SUCCESS) {
			g_oChLog<<BeginError(M4_CH_VM_NODE_NOT_FOUND_COMPILING)<<M4_NODE_SCHT3SYS_NODE_SYSTEM<<M4_CHANNEL_SCHT3S_T3_SYSTEM<<EndLog;
			m_poTrad->SetError();
		}
	}

	if ( ret == M4_SUCCESS ) m_oSystemItems.SetRecordSet(m_iNodeItems);

	return ret;
}


//--------------------------
//Find item, el global, por IDS, que llama a lso demás según channel, node, item
//--------------------------

m4return_t  ClFinder_TICH::FindItem ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister, ClAccess * ai_paccess,m4bool_t ai_bLookInAlias)
{
	m4return_t      ret ;
	m4pchar_t pcTI = 0; //TI a buscar en la tabla de TIS.
	m4pchar_t pcNodeTI=0;
	ClCompiledMCR * poCMCR;
	ClRegisterIndex iNode=M4DM_EOF_INDEX;


	if (ai_bLookInAlias){ // No estamos buscando TI_SYSTEM
	
		m_oItemActual.m_pszItem_TI=0;
		m_oItemActual.m_pszItem_T3=0;
		m_oItemActual.m_pszItem_Node_T3=0;
		m_oItemActual.m_bIsInherited=M4_FALSE;
	}


	if (m_poAccess == NULL){
	
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	if (m_oAccessFind == NULL){
	
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

	m_oAccessFind->Follow(*m_poAccess);

	//Obtenemos el CMCR
    poCMCR = m_poAccess->GetpCMCR() ;

	if ((ai_pcChannel != NULL) && (ai_pcChannel[0] != '\0')){
		//Canal!Nodo.Item
		ret = _FindChannel(ai_pcChannel, ai_pcNode, pcTI,ai_bLookInAlias);
		
		if (ret == M4_ERROR){
			if (ai_pcChannel == 0){
				// En este caso no ha encontrado ningun alias que coincida con el nombre.
				return M4_ERROR;
			}
			// En caso de que no lo encuentre en el nodo de T3, lo buscamos directamente en el canal nuevo.
			// Si este canal no existe, devolvemos error, porque no se encuentra el nombre del canal.
			// Si existe devolvemos el registro que nos venga.

			return _FindItemLevel2 ( ai_pcChannel, ai_pcNode, ai_pcItem, ao_pRegister);
		}



		if (pcTI){
			ret = _FindTI(pcTI, pcTI);
			m_bTIAliasFound = M4_TRUE;

			if (ret == M4_ERROR)
				return M4_ERROR;
		}
	}
	else if ((ai_pcNode != NULL) && (ai_pcNode[0] != '\0'))	{	
		//Node ( = Alias), Item
		ret = _FindNode(ai_pcNode, pcTI, pcNodeTI, iNode);
		
		if (ret == M4_ERROR){
			return M4_ERROR;
		}

		if (pcTI && pcNodeTI)
		{
			ret = _FindExtendedTIOrInherited(pcTI, pcNodeTI, iNode);
	
			if (ret == M4_ERROR){
				return M4_ERROR;
			}
		}
	}

	return _FindItem (pcTI, ai_pcItem, ao_pRegister, ai_paccess);
}

//--------------------------
//FindItem
//--------------------------

m4return_t  ClFinder_TICH::_FindItem (m4pchar_t ai_pcTi, m4pchar_t ai_pcItem, ClRegister * ao_pRegister, ClAccess *ai_paccess)
{

	m4return_t      ret = M4_SUCCESS;

	ClRegisterIndex i(0);

	m4uint32_t numRows;

	m4bool_t bItemFound = M4_FALSE;

	ClCompiledMCR * poCMCR;

	ClNode * poNodeItems;
	ClRegister	* poTempReg;
    m4uint8_t       iType ;
    m4uint32_t      iItemHandle ;
    m4double_t      dNumber ;
    m4pcchar_t      pccString ;
    ClItemIndex     iActualIndex ;
    ClItemIndex     iItemIdIndex ;
    ClItemIndex     iSynonymIdIndex ;
    ClItemIndex     iSynonymNameIndex ;
    m4VariantType   vValue ;
    

	//Obtenemos el CMCR
    poCMCR = m_poAccess->GetpCMCR() ;

	if( m_poTrad->GetSourceMode() == M4_OLD_ITEM_TO_ID_ITEM )
	{
		// En este modo hay que compilar con el id antiguo
	    iItemHandle = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_ID_ITEM_OLD, M4_TRUE ) ;
	}
	else
	{
	    iItemHandle = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_ID_ITEM, M4_TRUE ) ;
	}

	if( iItemHandle == 0 ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_ITEM_SCHTIS_ITEMS_ID_ITEM <<poCMCR->GetNodeId(m_iNodeItems)<<poCMCR->GetChannelId() );
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

    iItemIdIndex = poCMCR->GetItemIndex( iItemHandle ) ;


    iItemHandle = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_ID_SYNONYM, M4_TRUE ) ;

	if( iItemHandle != 0 ){
        iSynonymIdIndex = poCMCR->GetItemIndex( iItemHandle ) ;
    }
    else {
        iSynonymIdIndex = iItemIdIndex ;
    }


    iItemHandle = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_N_SYNONYM, M4_TRUE ) ;

	if( iItemHandle != 0 )
	{
        iSynonymNameIndex = poCMCR->GetItemIndex( iItemHandle ) ;
    }
    else{
        iSynonymNameIndex = iItemIdIndex ;

    }



	poNodeItems = &(m_oAccessFind->Node[ (ClHandle)m_iNodeItems ]) ; 

	if ( ai_paccess ){
		ClOldAccessBookMark oBM;
		if ( oBM.GetFrom ( *m_oAccessFind ) == M4_SUCCESS )	{
			ret = oBM.ApplyTo ( *ai_paccess );
		}
	}

	if ((ai_pcItem == NULL) || (ai_pcItem[0] == '\0')){
		//Devolvemos success sin buscar el item.
		return ret;
	}


	bItemFound = M4_FALSE;

    numRows = poNodeItems->RecordSet.Register.Count() ;
    if( *ai_pcItem == '#' ) {
	    if (m_poTrad->GetSourceMode()==M4_RESTORE_SOURCE){
			pccString=ai_pcItem+sizeof(m4char_t);
			iActualIndex=iItemIdIndex;
			iType=M4CL_CPP_TYPE_STRING_VAR;
		}
		else {
			/* Bug 0089065
			Si lo que viene detrás del hash no es un número es un error
			*/
			m4pchar_t	pcEndPointer = NULL ;
			dNumber = strtod( ai_pcItem + sizeof( m4char_t ), &pcEndPointer );

			if( *pcEndPointer != '\0' )
			{
				return M4_ERROR;
			}

			iActualIndex = iSynonymIdIndex ;
			iType = M4CL_CPP_TYPE_NUMBER ;
		}
				
	}
    else if( *ai_pcItem == '@' ) {
        if (m_poTrad->GetSourceMode()==M4_RESTORE_SOURCE){
			pccString=ai_pcItem+sizeof(m4char_t);
			iActualIndex=iItemIdIndex;
			iType=M4CL_CPP_TYPE_STRING_VAR;
		}
		else{
			pccString = ai_pcItem + sizeof( m4char_t ) ;
			iActualIndex = iSynonymNameIndex ;
			iType = M4CL_CPP_TYPE_STRING_VAR ;
		}

    }
    else {
         pccString = ai_pcItem ;
         iActualIndex = iItemIdIndex ;
         iType = M4CL_CPP_TYPE_STRING_VAR ;
    }

	m4bool_t bDeleted=M4_FALSE;

	poTempReg = &(poNodeItems->RecordSet.Register);

	/* Bug 0086408 Se saca fuera el posicionamiento del item */
	poTempReg->Item.MoveTo( iActualIndex ) ;

	for ( i = 0; i<numRows; ++i){
		 poTempReg->MoveTo( i ) ;

		 if ( poTempReg->IsDeleted(bDeleted) != M4_SUCCESS ) {
            m_poTrad->SetError();
            return M4_ERROR;
		 }
		 if (bDeleted){
			 continue ;	//Si el registro esta borrado, seguimos buscando por si se hubiese creado otro con el mismo nombre
		 }

         ret =  poTempReg->Item.Value.Get( vValue );
         if (ret!= M4_SUCCESS){
			//USGE
            //SETCODE (M4_CH_VM_NULL_IS_INVALID, ERRORLOG, "Item ID_ITEM in ITEMS == NULL");
            m_poTrad->SetError();
            return M4_ERROR;
         }

         if( vValue.Type == M4CL_CPP_TYPE_NULL ){
			continue ;
         }

         if( iType == M4CL_CPP_TYPE_NUMBER ){
			if( vValue.Data.DoubleData == dNumber ){
			   bItemFound = M4_TRUE;
			   if (ao_pRegister != NULL) {
				    *ao_pRegister = *poTempReg;
					// Nos posicionamos para que el access quede posicionado
					poNodeItems->RecordSet.Current.MoveTo (poTempReg->Index);
			   }
			   break; //Encontrado el ITEM de la TI.
			}
         }
         else if( iType == M4CL_CPP_TYPE_STRING_VAR){
		    if( stricmp( vValue.Data.PointerChar , pccString ) == 0 )
		    {
			    bItemFound = M4_TRUE;
			    if (ao_pRegister != NULL){
				    *ao_pRegister = *poTempReg;
					// Nos posicionamos para que el access quede posicionado.
					poNodeItems->RecordSet.Current.MoveTo (poTempReg->Index);
				}
			    break; //Encontrado el ITEM de la TI.
		    }
         }
	}


	/* Bug 0069443
	Hay que verificar que el item pertenezca a la TI que nos dicen
	y no a una hija
	*/
	m4int32_t iItemLevel;
	m4int32_t iTiLevel;
	m4pchar_t pcItemTI;
	ClNode *poNodeTisInherit;

	if (bItemFound && ai_pcTi!=NULL) {

		pcItemTI = (m4pchar_t) poNodeItems->RecordSet.Current.Item[ "ID_TI" ].Value.Get() ;

		if (pcItemTI == NULL) {
			return M4_ERROR;
		}

		if ( strcmpi(pcItemTI, ai_pcTi) != 0) {
		
			poNodeTisInherit = &(m_oAccessFind->Node[ (ClHandle)m_iNodeTis_Inherit ]) ;

			// Se buscan las TIs en el árbol de herencia y su nivel
			ret = __BSearch(poNodeTisInherit->RecordSet, pcItemTI, 1);

			if (ret != M4_SUCCESS) {
				return M4_ERROR;
			}

			if ( poNodeTisInherit->RecordSet.Register.Index == M4DM_EOF_INDEX ) {
				return M4_ERROR;
			}

			ret = poNodeTisInherit->RecordSet.Register.Item[ "ID_LEVEL" ].Value.Get( vValue ) ;

			if (ret != M4_SUCCESS) {
				return M4_ERROR;
			}

			if (vValue.Type==M4CL_CPP_TYPE_NULL) {
				iItemLevel=0;
			}
			else if (vValue.Type==M4CL_CPP_TYPE_NUMBER ) {
				iItemLevel= (m4int32_t) vValue.Data.DoubleData;
			}
			else
			{
				return M4_ERROR;
			}

			ret = __BSearch(poNodeTisInherit->RecordSet, ai_pcTi, 1);

			if (ret != M4_SUCCESS) {
				return M4_ERROR;
			}

			if ( poNodeTisInherit->RecordSet.Register.Index == M4DM_EOF_INDEX ) {
				return M4_ERROR;
			}

			ret = poNodeTisInherit->RecordSet.Register.Item[ "ID_LEVEL" ].Value.Get( vValue ) ;

			if (ret != M4_SUCCESS) {
				return M4_ERROR;
			}

			if (vValue.Type==M4CL_CPP_TYPE_NULL) {
				iTiLevel=0;
			}
			else if (vValue.Type==M4CL_CPP_TYPE_NUMBER ) {
				iTiLevel= (m4int32_t) vValue.Data.DoubleData;
			}
			else
			{
				return M4_ERROR;
			}

			if (iTiLevel>iItemLevel) {
				bItemFound = M4_FALSE;
			}
		}
	}

	if (bItemFound){
		ret = M4_SUCCESS;	// Encontró el ITEM para la TI.
		ret = m_poTrad->AddItemSynonym(ao_pRegister,ai_pcItem);
	}
	else{
		ret = M4_ERROR; //No encontró el ITEM para la TI.      
    }


	return ret;

}

//--------------------------
//FindChannel
//--------------------------

m4return_t  ClFinder_TICH::_FindChannel ( m4pchar_t &aio_pcChannel, m4pchar_t ai_pcNode, m4pchar_t &ao_pcTI,m4bool_t ai_bLookInAlias)
{
	ClNode	*poNodeT3Alias, 
			*poNodeT3s,
			*poNodeNodes; 

	m4return_t ret;
	//ClItemIndex iSearchItem;

	ClCompiledMCR *	poCMCR = m_poAccess->GetpCMCR() ;

	m4pcchar_t pcTemp;

	ClRegisterIndex	iRegIndex;

	// Primero buscamos aio_pcChannel como alias de canal en T3_ALIAS
	// Si hay errores fallar o continuar??

	// Si hay errores, devolver error, excepto en el caso de que sea el T3_SYSTEM.
	
	poNodeT3Alias = &(m_oAccessFind->Node[ (ClHandle)m_iNodet3Alias ]) ; 

	//M4_NODE_SCHTIS_T3_ALIAS  indice == M4_ITEM_SCHTIS_T3_ALIAS_ID_ALIAS
	ret = __BSearch(poNodeT3Alias->RecordSet, aio_pcChannel, 1);

	if (ret == M4_ERROR) 
		return M4_ERROR;

	// Actualmente hay un solo canal que se busca de forma diferente, el T3_SYSTEM.
	// Este canal no tiene porque esta en T3Alias, porque las busquedas de los items del
	// sistema las hacemos nosotros de forma cableada.

	// ATENCION : VERSION 140.
	// En caso de que no sea el T3_SYSTEM, tiene que existir un Alias OBLIGATORIAMENTE
	// ( antes no era necesario. Bastaba con que existiera el canal ).
	if (ai_bLookInAlias){
		if ( poNodeT3Alias->RecordSet.Register.Index != M4DM_EOF_INDEX ) {		
			pcTemp = (m4pchar_t) poNodeT3Alias->RecordSet.Register.Item[M4_ITEM_SCHTIS_T3_ALIAS_ID_T3_USED].Value.Get();
			if (pcTemp== 0)	{
				DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_ALIAS, "T3Alias.Id_T3_Used"<< LOG_CAT_BLOCK(aio_pcChannel<<"!"<<ai_pcNode) );
				m_poTrad->SetError();
				return M4_ERROR;
			}
				
			aio_pcChannel = (m4pchar_t)pcTemp; //Nos quedamos con el nombre del canal.						
			m_oItemActual.m_pszItem_T3=aio_pcChannel;
//			m_oItemActual.m_pszItem_Node_T3=0;

		}
		else {
				// Si no hay Alias damos error de compilación. Ponemos el puntero de canal a nulo para que no se siga bscando.
				/* Bug 0090300 Se quitan los # iniciales */
				g_oChLog<<BeginError(M4_CH_VM_CHANNEL_ALIAS_NOT_FOUND_COMPILING)<<M4SkipHash( aio_pcChannel )<<EndLog;
				aio_pcChannel = 0;
				m_poTrad->SetError();
				return M4_ERROR;
		}
	}

	// Hemos encontrado el alias. Buscamos a ver si estuviera en el nodo de T3s
	// Si no estuviera devolvemos error (pero el verdadero nombre) para buscarlo en el canal auxiliar.
	// Si estuviera devolvemos exito iy seguimos buscando en el DESIGN_CHANNEL
	
	poNodeT3s       = &(m_oAccessFind->Node[ (ClHandle)m_iNodeT3s ]) ;

	//M4_NODE_SCHTIS_T3S  indice == M4_ITEM_SCHTIS_T3S_ID_T3
	ret = __BSearch(poNodeT3s->RecordSet, aio_pcChannel, 1);

	if (ret == M4_ERROR) {
		return M4_ERROR;
	}

	iRegIndex = poNodeT3s->RecordSet.Register.Index;

	if ( iRegIndex != M4DM_EOF_INDEX ) {
		poNodeT3s->RecordSet.Current[iRegIndex];

		poNodeNodes = &(m_oAccessFind->Node[ (ClHandle)m_iNodeNodes ]) ;

		ret = __BSearch(poNodeNodes->RecordSet, ai_pcNode, 1);

		if (ret == M4_ERROR) {
			return M4_ERROR;
		}

		iRegIndex = poNodeNodes->RecordSet.Register.Index;

		if ( iRegIndex != M4DM_EOF_INDEX ) {
			poNodeNodes->RecordSet.Current[iRegIndex];

			ao_pcTI = (m4pchar_t) poNodeNodes->RecordSet.Register.Item[M4_ITEM_SCHTIS_NODES_ID_TI].Value.Get();
			if (ao_pcTI== 0){
				 g_oChLog<<BeginError(M4_CH_VM_NULL_ITEM_VAL_COMP)<<"Nodes.Id_TI"<<""<<ai_pcNode<<aio_pcChannel<<EndLog;
				 m_poTrad->SetError();
				 return M4_ERROR;
			}
			if (ai_bLookInAlias){ // No es el T3_SYSTEM
				m_oItemActual.m_pszItem_TI=ao_pcTI;
				m_oItemActual.m_pszItem_Node_T3=(m4pchar_t) poNodeNodes->RecordSet.Register.Item[M4_ITEM_SCHTIS_NODES_ID_T3].Value.Get();
			// Herencia.
			// Tenemos que llenar el item ID_NODE_T3_USED en el nodo de dependencias de canal.
			// Esto solo es necesario en el caso de que por nivel 2 hagamos referencia a nuestro propio canal.
			// Para obtener el leemos el item ID_T3 de TISNODES.

			}
		}
		else{
			/* Bug 0090300 Se quitan los # iniciales */
			g_oChLog<<BeginError(M4_CH_VM_NODE_NOT_FOUND_COMPILING)<<M4SkipHash( ai_pcNode )<<M4SkipHash( aio_pcChannel )<<EndLog;
			return (M4_ERROR); //No encontró el Node para el Channel.
		}
	}
	else{
//		No sacamos error. Podemos encontrarlo en el canal auxiliar.
//		g_oChLog<<BeginError(M4_CH_VM_CHANNEL_NOT_FOUND_COMPILING)<<aio_pcChannel<<EndLog;

		return (M4_ERROR); //No encontró el Channel.
	}
	//Aquí ya hemos comprobado la existencia del Channel y TI.
	return M4_SUCCESS;
}

//--------------------------
//FindNode
//--------------------------

m4return_t  ClFinder_TICH::_FindNode ( m4pchar_t ai_pcNode, m4pchar_t &ao_rpcTI, m4pchar_t &ao_rpcNodeTI, ClRegisterIndex &ao_riNode)
{
	m4return_t		ret;
	m4uint32_t		numRows;
	m4bool_t		bFound;
	m4pchar_t		pcNode;
	ClRegisterIndex	i(0);
	ClNode			*poNodeNodes;
	ClNode			*poNodeOverwriteNodes;
	ClNode			*poNodeTIAlias;
	ClNode			*poNodeTIAliasRes;


	ao_riNode = M4DM_EOF_INDEX ;
	ao_rpcTI = ai_pcNode ;
	ao_rpcNodeTI = ai_pcNode ;


	//Buscamos el Node en Alias.
	poNodeTIAlias = &(m_oAccessFind->Node[ (ClHandle)m_iNodeTiAlias ]) ; 

	ret = __BSearch(poNodeTIAlias->RecordSet, ai_pcNode, 1);

	if (ret == M4_ERROR) {
		return M4_ERROR;
	}

	ClCompiledMCR * poCMCR = m_poAccess->GetpCMCR() ;

	if ( poNodeTIAlias->RecordSet.Register.Index != M4DM_EOF_INDEX ) {
		ao_rpcTI = (m4pchar_t) poNodeTIAlias->RecordSet.Register.Item[M4_ITEM_SCHTIS_TI_ALIAS_ID_TI_USED].Value.Get();
        if (ao_rpcTI== 0){
			DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_ITEM_VAL_COMP_ALIAS, "TIAlias.Id_TI_Used"<< LOG_CAT_BLOCK("Node: "<<ai_pcNode<<" TI: "<<ao_rpcTI) );
            m_poTrad->SetError();
            return M4_ERROR;
        }
		// Tenemos Alias para la TI.
		// Guardamos en m_pszItem_TI el verdadero nombre de la TI.
		// En caso de que esté a nulo, quiere decir que se ha utilizado una TI sin indicar el Alias.
		m_oItemActual.m_pszItem_TI=ao_rpcTI;

		/* Bug 0070944
		Si hay resolución de alias consideramos el nodo al que apunta
		*/
		poNodeTIAliasRes = &( m_oAccessFind->Node[ (ClHandle)m_iNodeTiAliasRes ] ) ; 

		ret = __BSearch( poNodeTIAliasRes->RecordSet, ai_pcNode, 1 ) ;

		if( ret == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		if( poNodeTIAliasRes->RecordSet.Register.Index != M4DM_EOF_INDEX )
		{
			pcNode = (m4pchar_t) poNodeTIAliasRes->RecordSet.Register.Item[ M4_ITEM_SCHTIS_TI_ALIAS_RES_ID_NODE_USED ].Value.Get() ;

			if( pcNode != NULL )
			{
				poNodeNodes = &( m_oAccessFind->Node[ (ClHandle)m_iNodeNodes ] ) ;

				ret = __BSearch( poNodeNodes->RecordSet, pcNode, 1 ) ;

				if( ret == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				ao_riNode = poNodeNodes->RecordSet.Register.Index ;
			}
		}
	}
	else{
		//No encontró la TI para el alias, nos quedamos con el nodo como TI.
		if (m_poTrad->GetSourceMode()==M4_OLD_TI_TO_ID_TI){
			// En este caso, si no habia alias, marco para que busque el ID_TI en OLD_TI en lugar de en ID_TI.
			// El bool se resetea despues de las llamadas a la funcion _FindTI.
			m_bTIAliasFound = M4_FALSE;
		}
		ao_rpcTI = ai_pcNode;
	}


	/* Bug 0069443
	Hay que dar una TI que pertenezca a un nodo
	Para ello la búsqueda se hace en la sobreescritura de nodos
	para encontrar un nodo que utilice una Ti hija de esa
	*/

	/* Bug 0070188
	Por defecto la TI es la original, para que si no se encuentra todo siga como antes
	*/
	ao_rpcNodeTI = ao_rpcTI ;

	//Recorremos todos los nodos.
	poNodeNodes = &(m_oAccessFind->Node[ (ClHandle)m_iNodeNodes ]) ;
	poNodeOverwriteNodes = &(m_oAccessFind->Node[ (ClHandle)m_iNodeNodes_Inherit ]) ;


	/* Bug 0070944
	Si hay resolución de alias consideramos el nodo al que apunta
	*/
	bFound = M4_FALSE ;

	if( ao_riNode != M4DM_EOF_INDEX )
	{
		poNodeNodes->RecordSet.Current[ ao_riNode ] ;

		// Se busca la TI en la sobreescritura
		ret = __BSearch( poNodeOverwriteNodes->RecordSet, ao_rpcTI, 1 ) ;

		if( ret == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		if( poNodeOverwriteNodes->RecordSet.Register.Index != M4DM_EOF_INDEX )
		{
			// Si se encuentra se coge como TI la hija, que es la del nodo
			ao_rpcNodeTI = (m4pchar_t) poNodeNodes->RecordSet.Current.Item[ "ID_TI" ].Value.Get() ;

			if( ao_rpcNodeTI == NULL )
			{
				return( M4_ERROR ) ;
			}
			bFound = M4_TRUE ;
		}
	}

	if( bFound == M4_FALSE )
	{
		numRows = poNodeNodes->RecordSet.Register.Count() ;
		for ( i = 0; i<numRows; ++i ){

			poNodeNodes->RecordSet.Current[i];

			// Se busca la TI en la sobreescritura
			ret = __BSearch(poNodeOverwriteNodes->RecordSet, ao_rpcTI, 1);

			if (ret != M4_SUCCESS) {
				return M4_ERROR;
			}

			if ( poNodeOverwriteNodes->RecordSet.Register.Index != M4DM_EOF_INDEX ) {

				// Si se encuentra se coge como TI la hija, que es la del nodo
				ao_rpcNodeTI = (m4pchar_t) poNodeNodes->RecordSet.Current.Item[ "ID_TI" ].Value.Get() ;

				if (ao_rpcNodeTI == NULL) {
					return M4_ERROR;
				}
				break;
			}
		}
	}

	return M4_SUCCESS;
}

//--------------------------
//Find Ti en todos los nodos del canal actual (design channel)
//--------------------------
m4return_t  ClFinder_TICH::_FindExtendedTIOrInherited (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI, ClRegisterIndex ai_iNode)
{
	// !!! OJO. Qué pasa con el TI_ALIAS. Es posible que tenga que utilizar m_bTIAliasFound.
	if (m_iChanType == M4CL_FINDER_CHANNEL_DESIGN_CHANNEL){

		// Mirar si tengo que ponerlo.
		if( _IsInheritedRule() == M4_TRUE )
		{
			ClCompiledMCR *pDynamicMCR = GetDynamicMCR();

			if (pDynamicMCR){

				m4uint32_t hNodeRes=0;
				m4pcchar_t pIdNodeRes=0; // Nodo al que resuelve.
				m4pcchar_t pIdTiRes=0;	// Ti a la que resuleve.


				m4uint32_t hNode=pDynamicMCR->GetChannelNodeHandleById(m_pCurrentIdNode);

				if( hNode == 0 )
				{
					return( M4_ERROR ) ;
				}
				
				m4uint32_t hItem=pDynamicMCR->GetNodeItemHandleById(hNode,m_pCurrentIdItem);

				if( hItem == 0 )
				{
					return( M4_ERROR ) ;
				}
				
				/* Bug 0069036
				Ojo, que aquí se pone el nivel de herencia más bajo
				y no de la regla, por que no es fácil obtenerlo
				*/
				/* Bug 0074629
				No se puede poner 1 porque no se sabe si es herencia de canal o de TI.
				Habrá que dejarlo en 0 si no hay nivel de herencia y 1 si hay herencia
				*/
				m4uint8_t iInherits = 0 ;

				if( pDynamicMCR->GetChannelInheritLevel() > 1 )
				{
					iInherits = 1 ;
				}

				hNodeRes=pDynamicMCR->GetNodeAliasNodeHandleByAlias(hNode,ai_rpcNodeTI,iInherits);
				
				if( hNodeRes == 0 )
				{
					return( M4_ERROR ) ;
				}

				// Tenemos el handle de nodo. 
				pIdTiRes=pDynamicMCR->GetNodeTiId(hNodeRes);
				ai_rpcNodeTI=(m4pchar_t)pIdTiRes;

				// Si la sustitucion la hecemos por nodo:
//				pIdNodeRes=pDynamicMCR->GetNodeId(hNodeRes);
				// Si la hacemos por TI
				/* Bug 0079665 */
				pIdNodeRes=pIdTiRes;
				
				
				m_poTrad->AddTISynonym(ai_rpcTI,pIdNodeRes);

				// OJO, que seguimos buscando la TI. Ahora se llama de verdad a _FindExtendedTI, para posicionarnos y buscar el item en concreto.
			}
		}

	}

	// Tenemos que posicionarnos en la TI que corresponda.
	/* Bug 0070188
	Hay que subir hacia arriba la TI cambiada para que se compruebe bien el item
	*/
	return _FindExtendedTI(ai_rpcTI, ai_rpcNodeTI, ai_iNode);

}

//Si es designchannel busca en todos las tis de todos los nodos del canal.
m4return_t  ClFinder_TICH::_FindExtendedTI (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI, ClRegisterIndex ai_iNode)
{
	if (m_iChanType == M4CL_FINDER_CHANNEL_DESIGN_CHANNEL){

		ClNode	*poNodeNodes;
		ClRegisterIndex i(0);
		m4uint32_t numRows;

		//Recorremos todos los nodos.
		poNodeNodes = &(m_oAccessFind->Node[ (ClHandle)m_iNodeNodes ]) ;

		/* Bug 0070944
		Si hay resolución de alias consideramos el nodo al que apunta
		*/
		if( ai_iNode != M4DM_EOF_INDEX )
		{
			poNodeNodes->RecordSet.Current[ai_iNode];

			if ( _FindTI(ai_rpcTI, ai_rpcNodeTI) == M4_SUCCESS){
				m_bTIAliasFound = M4_TRUE;
				return M4_SUCCESS;
			}
		}

        numRows = poNodeNodes->RecordSet.Register.Count() ;
		for ( i = 0; i<numRows; ++i ){
			poNodeNodes->RecordSet.Current[i];

			/* Bug 0070188
			Hay que subir hacia arriba la TI cambiada para que se compruebe bien el item
			*/
			if ( _FindTI(ai_rpcTI, ai_rpcNodeTI) == M4_SUCCESS){
				m_bTIAliasFound = M4_TRUE;
				return M4_SUCCESS;
			}
		}
	}
	else{
		/* Bug 0070188
		Hay que subir hacia arriba la TI cambiada para que se compruebe bien el item
		*/
		if ( _FindTI(ai_rpcTI, ai_rpcNodeTI) == M4_SUCCESS ){
			m_bTIAliasFound = M4_TRUE;
			return M4_SUCCESS;
		}
	}

	/* Bug 0090300 Se quitan los # iniciales */
	g_oChLog<<BeginError(M4_CH_VM_TI_NOT_FOUND_COMPILING)<<M4SkipHash( ai_rpcNodeTI )<<EndLog;

	m_bTIAliasFound = M4_TRUE;
	return M4_ERROR; //No encontró la TI

}

//--------------------------
//Find ti
//--------------------------

m4return_t  ClFinder_TICH::_FindTI (m4pchar_t &ai_rpcTI, m4pchar_t &ai_rpcNodeTI)
{
	m4return_t ret;

	ClNode	*poNodeTIS;
	//ClItemIndex iSearchItem;
	ClRegisterIndex	iRegIndex;


	//Buscamos en el current de TIS.
    poNodeTIS = &(m_oAccessFind->Node[ (ClHandle)m_iNodeTis ]) ;

	if (m_poTrad->GetSourceMode()==M4_OLD_TI_TO_ID_TI){
		// En este modo, si no hemos encontrado el Alias , tenemos que buscar en OLD_TI en lugar de 
		// en ID_TI.
		if (m_bTIAliasFound == M4_FALSE){
			// Buscamos en OLD_TI.
			// En el DESIGN_CHANNEL solo hay un registro en el nodo de TIS.
			m4pchar_t pOldTI = (m4pchar_t)poNodeTIS->RecordSet.Register[0].Item[(ClItemIndex)m_iItemOld_TI].Value.Get();
			if (pOldTI){
				if (stricmp(ai_rpcNodeTI,pOldTI)==0){

					/* Bug 0070188
					Hay que subir hacia arriba la TI cambiada para que se compruebe bien el item
					*/
					ai_rpcTI = (m4pchar_t)poNodeTIS->RecordSet.Register[0].Item[(ClItemIndex)m_iItemID_TI].Value.Get();
					m_poTrad->AddTISynonym(pOldTI,ai_rpcTI);
					m_bTIAliasFound = M4_TRUE;
					poNodeTIS->RecordSet.Current.MoveTo(0);
					return M4_SUCCESS;
				}
			}
		}
	}
	ret = __BSearch(poNodeTIS->RecordSet, ai_rpcNodeTI, 1);

	if (ret == M4_ERROR) {
		return M4_ERROR;
	}

	iRegIndex = poNodeTIS->RecordSet.Register.Index;

	if ( iRegIndex != M4DM_EOF_INDEX ) {
		poNodeTIS->RecordSet.Current[iRegIndex];
		return M4_SUCCESS;
	}
	else{
	   return M4_ERROR; //No encontró la TI
	}
}

ClAccess *	ClFinder_TICH::GetpAccessItemChan(void)
{
	
	if ( !m_poAccessItems )	{
		if ( m_poAccess->GetpChannel()->Level2.GetL2Access ( M4_CHANNEL_ITEMS_L2_INSTANCE, &m_poAccessItems , M4_FALSE) != M4_SUCCESS )
			m_poAccessItems = NULL;
	}


	return m_poAccessItems;

}

//--------------------------
//FindItem level 2
//--------------------------

m4return_t  ClFinder_TICH::_FindItemLevel2	( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClRegister * ao_pRegister)
{
	m4return_t ret = M4_ERROR;
	ClRegister_Current * pCurrentNodes=0 ;
	ClRegister *pRegisterItems=0;
	m4VariantType vResult;

	ClAccess * pAccess = GetpAccessItemChan ();
	if ( !pAccess || m_iChanType != M4CL_FINDER_CHANNEL_DESIGN_CHANNEL ){
		return M4_ERROR;
	}

// Ejecutamos el metodo FindNode.
	// Metemos los argumentos.
	m4VariantType vIsRestoreSource(m_poTrad->GetSourceMode()==M4_RESTORE_SOURCE?M4_TRUE:M4_FALSE);
	m4VariantType vThisChannel((m4pchar_t)m_pThisIdT3);
	m4VariantType vItemL2(ai_pcItem);
	m4VariantType vNodeL2(ai_pcNode);
	m4VariantType vChannelL2(ai_pcChannel);

	// Canal (L2)
	pAccess->GetpExecutor()->Stack.Push(vChannelL2);
	// Nodo (L2)
	pAccess->GetpExecutor()->Stack.Push(vNodeL2);
	// Item (L2)
	pAccess->GetpExecutor()->Stack.Push(vItemL2);
	// Canal actual.
	pAccess->GetpExecutor()->Stack.Push(vThisChannel);
	// RestoreSource
	pAccess->GetpExecutor()->Stack.Push(vIsRestoreSource);

	pCurrentNodes = &pAccess->Node[M4NODE_LEVEL2_ITEMS_NODES_L2].RecordSet.Current;

	//Ejecutamos.
	ret = pCurrentNodes->Item[M4ITEM_NODES_L2_FINDITEM].Call();
	if (ret!=M4_SUCCESS){
		g_oChLog<<BeginError(M4_CH_VM_COMP_IE_INTERNAL_ERROR)<<M4ITEM_NODES_L2_FINDITEM<<ai_pcNode<<ai_pcChannel<<EndLog;
		return M4_ERROR;
	}

	// Sacamos el resultado
	ret = pAccess->GetpExecutor()->Stack.Pop(vResult);

	if (ret!=M4_SUCCESS||vResult.Type!=M4CL_CPP_TYPE_NUMBER){
		g_oChLog<<BeginError(M4_CH_VM_COMP_IE_INTERNAL_ERROR)<<M4ITEM_NODES_L2_FINDITEM<<ai_pcNode<<ai_pcChannel<<EndLog;
		return M4_ERROR;
	}

// Comprobamos el resultado.
	if (vResult.Data.DoubleData==M4_TRUE){
		pRegisterItems = &pAccess->Node[M4NODE_LEVEL2_ITEMS_ITEMS_L2].RecordSet.Register;
		*ao_pRegister = *pRegisterItems ;
		m_oItemActual.m_pszItem_TI = (m4pchar_t)pRegisterItems->Item[M4ITEM_ITEMS_L2_ID_TI].Value.Get();
		m_oItemActual.m_pszItem_T3 = (m4pchar_t)pCurrentNodes->Item[M4ITEM_NODES_L2_ID_T3].Value.Get();
		m_oItemActual.m_pszItem_Node_T3 = (m4pchar_t)pCurrentNodes->Item[M4ITEM_NODES_L2_ID_NODE_T3].Value.Get();
		// encontrado el item para la TI.
		m_poTrad->AddItemSynonym(ao_pRegister,ai_pcItem);
		ret = M4_SUCCESS;
	}
	else {

		/* Bug 0090300 Se quitan los # iniciales */
		g_oChLog<<BeginError(M4_CH_VM_ITEM_CH_NOT_FOUND_COMPILING)<<M4SkipHash( ai_pcItem )<<M4SkipHash( ai_pcNode )<<ai_pcChannel<<EndLog;
		ret = M4_ERROR;
	}
	return ret;
}

//--------------------------
//FindSystemMethod
//--------------------------

m4return_t  ClFinder_TICH::_FindSystemMethod	( m4pchar_t ai_pcItem, ClRegister * ao_pRegister)
{
	
	//Esta funcion puede ser llamada desde fuera, no descoloca el access.
	m4return_t ret = M4_SUCCESS;
	m4uint32_t hSearchItem;
	ClItemIndex iSearchItem;
	ClCompiledMCR * poCMCR = m_poAccess->GetpCMCR();

    hSearchItem = poCMCR->GetNodeItemHandleById( m_iNodeItems, M4_ITEM_SCHTIS_ITEMS_ID_ITEM, M4_TRUE ) ;

	if( hSearchItem == 0 ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_ITEM_SCHTIS_ITEMS_ID_ITEM <<poCMCR->GetNodeId(m_iNodeItems)<<poCMCR->GetChannelId() );
        m_poTrad->SetError();
		return( M4_ERROR ) ;
	}

    iSearchItem = poCMCR->GetItemIndex( hSearchItem ) ;
	if (m_oSystemItems.poRSSystemItems == NULL ){
		ClOldAccessBookMark oBM;
		// Guardamos la posicion del Access de busqueda.

		if (oBM.GetFrom ( *m_oAccessFind )!= M4_SUCCESS){
			return M4_ERROR;
		}

		if ( SetItemSystemRecordSet() != M4_SUCCESS ){
			return M4_ERROR;
		}

		if (oBM.ApplyTo ( *m_oAccessFind )!= M4_SUCCESS){
			return M4_ERROR;
		}
	}

 	__SSearch(m_oSystemItems.poRSSystemItems->Register, ai_pcItem, iSearchItem);
	if ( m_oSystemItems.poRSSystemItems->Register.Index != M4DM_EOF_INDEX ) {		
		//Encontrado entre los items del sistem. Buen rollo!
		*ao_pRegister = m_oSystemItems.poRSSystemItems->Register;	
		// Tenemos que colocar m_oAccessFind igual que el access de los items del sistema.
		m_oSystemItems.poRSSystemItems->Current.MoveTo ( m_oSystemItems.poRSSystemItems->Register.Index );
		m_oAccessFind->Follow ( *(m_oSystemItems.poAccessSystemItems ) );
		
		ret = M4_SUCCESS;
	}
	else{
		ret = M4_ERROR;
	}

	return ret;
}

//--------------------------
//FindNode level 2
//--------------------------

m4return_t  ClFinder_TICH::_FindNodeLevel2 ( m4pchar_t ai_pcChannel, m4pchar_t ai_pcNode )
{

	m4return_t ret = M4_ERROR;
	ClRegister_Current * pCurrentNodes=0;
	m4VariantType	vResult;

	ClAccess * pAccess = GetpAccessItemChan ();
	if ( !pAccess || m_iChanType != M4CL_FINDER_CHANNEL_DESIGN_CHANNEL ){
		return M4_ERROR;
	}

// Ejecutamos el metodo FindNode.
	// Metemos los argumentos.
	m4VariantType vThisChannel((m4pchar_t)m_pThisIdT3);
	m4VariantType vNodeL2(ai_pcNode);
	m4VariantType vChannelL2(ai_pcChannel);

	// Canal (L2)
	pAccess->GetpExecutor()->Stack.Push(vChannelL2);
	// Nodo (L2)
	pAccess->GetpExecutor()->Stack.Push(vNodeL2);
	// Canal actual.
	pAccess->GetpExecutor()->Stack.Push(vThisChannel);

	pCurrentNodes = &pAccess->Node[M4NODE_LEVEL2_ITEMS_NODES_L2].RecordSet.Current;

	//Ejecutamos.
	ret = pCurrentNodes->Item[M4ITEM_NODES_L2_FINDNODE].Call();
	if (ret!=M4_SUCCESS){
		g_oChLog<<BeginError(M4_CH_VM_COMP_IE_INTERNAL_ERROR)<<M4ITEM_NODES_L2_FINDNODE<<ai_pcNode<<ai_pcChannel<<EndLog;
		return M4_ERROR;
	}

	// Sacamos el resultado
	ret = pAccess->GetpExecutor()->Stack.Pop(vResult);

	if (ret!=M4_SUCCESS||vResult.Type!=M4CL_CPP_TYPE_NUMBER){
		g_oChLog<<BeginError(M4_CH_VM_COMP_IE_INTERNAL_ERROR)<<M4ITEM_NODES_L2_FINDNODE<<ai_pcNode<<ai_pcChannel<<EndLog;
		return M4_ERROR;
	}

// Comprobamos el resultado.
	if (vResult.Data.DoubleData==M4_TRUE){
		return M4_SUCCESS;
	}

	/* Bug 0090300 Se quitan los # iniciales */
	g_oChLog<<BeginError(M4_CH_VM_NODE_NOT_FOUND_COMPILING)<<M4SkipHash( ai_pcNode )<<M4SkipHash( ai_pcChannel )<<EndLog;
	return M4_ERROR;

}

//--------------------------
//CheckNodalItem
//--------------------------
m4bool_t	
ClFinder_TICH::_CheckNodalItem(ClRegister * ao_pRegister)
{
	// Obtenemos el ID_NODE, ID_T3 del item en que estemos.
	if (ao_pRegister==0){
		return M4_FALSE;
	}
	m4pchar_t pIdT3=(m4pchar_t)ao_pRegister->GetpAccess()->GetpCMCR()->GetChannelId();

	// Si el registro pertenece al canal EXTENDED_ITEMS no me hace falta comprobar nada,
	// porque va en el filtro de este canal.
	if (strcmp(pIdT3,M4OBJECT_LEVEL2_ITEMS)==0){
		return M4_TRUE;
	}
	// Si no, cojo el ID_T3 del item que estoy buscando.

	if (m_iItemIdT3!=M4DM_EOF_INDEX){
		pIdT3 = (m4pchar_t)ao_pRegister->Item[(ClItemIndex)m_iItemIdT3].Value.Get();
	}
	if (pIdT3){
		// Comprobamos que el item que estamos compilando tenga el mismo ID_T3.
		if (m_pIdT3){
			if (strcmp(pIdT3,m_pIdT3)!=0){
				// Tenemos IdT3 distinto en el item llamado que en el llamador.
				m4pchar_t idItem = (m4pchar_t)ao_pRegister->Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();
				g_oChLog<<BeginError(M4_CH_VM_COMP_NODAL_ITEM_CALL_FROM_NODAL_ITEM)<<idItem<<pIdT3<<m_pIdT3<<EndLog;
				return M4_FALSE;
			}
		}
		else{
			// Llamda desde un item normal a uno de nodo. No se permite.
			m4pchar_t idItem = (m4pchar_t)ao_pRegister->Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get();
			g_oChLog<<BeginError(M4_CH_VM_COMP_NODAL_ITEM_CALL_FROM_NORMAL_ITEM)<<idItem<<pIdT3<<EndLog;
			return M4_FALSE;
		}
	}
	return M4_TRUE;
}


//--------------------------------------
//Retorna si el argumetno es un alias o no 
//--------------------------------------
m4bool_t	ClFinder_TICH::IsAliasOrMyT3			(m4char_t *ai_Channel){
	
	ClNode	*poNodeT3Alias,*poNodeT3s;
	
	m4return_t ret;
	if (m_poAccess == NULL){
		g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_FALSE ) ;
	}

	if (m_oAccessFind == NULL){
        g_oChLog<<BeginError(M4_CH_VM_INVALID_ACCESS_COMPILING)<<EndLog;
        m_poTrad->SetError();
		return( M4_FALSE ) ;
	}

	m_oAccessFind->Follow(*m_poAccess);


	if ( (ai_Channel == 0 ) || (ai_Channel[0] == '\0') ){
		return M4_FALSE;
	}
	

	ai_Channel=strupr(ai_Channel);
	// Primero buscamos ai_Channel como alias de canal en T3_ALIAS
	// Si hay errores fallar o continuar??
	
	// Si hay errores, devolver error, excepto en el caso de que sea el T3_SYSTEM.
	
	poNodeT3Alias = &(m_oAccessFind->Node[ (ClHandle)m_iNodet3Alias ]) ; 
	
	//M4_NODE_SCHTIS_T3_ALIAS  indice == M4_ITEM_SCHTIS_T3_ALIAS_ID_ALIAS
	ret = __BSearch(poNodeT3Alias->RecordSet, ai_Channel, 1);
	
	if (ret == M4_ERROR) {
		return M4_FALSE;
	}
	
	if ( poNodeT3Alias->RecordSet.Register.Index != M4DM_EOF_INDEX ) {
		return M4_TRUE;
	}

	poNodeT3s       = &(m_oAccessFind->Node[(ClHandle)m_iNodeT3s]) ;
	
	m4char_t *pszTemp = (m4char_t*)poNodeT3s->RecordSet.Current.Item[M4_ITEM_SCHTIS_T3S_ID_T3].Value.Get();
	
	if (strcmp( pszTemp,ai_Channel) == 0){
		return M4_TRUE;
	}

	/* Bug 0090300 Se quitan los # iniciales */
	g_oChLog<<BeginError(M4_CH_VM_CHANNEL_ALIAS_NOT_FOUND_COMPILING)<<M4SkipHash( ai_Channel )<<EndLog;
	m_poTrad->SetError();
	return M4_FALSE;
	
	
}

