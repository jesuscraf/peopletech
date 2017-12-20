//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#include "licensemanagerthread.hpp"
#include "m4stl.hpp"

#include "license.hpp"
#include "xmlanalyzerfactory.hpp"
#include "xmlanalyzerinterface.hpp"
#include "encrypt.hpp"
#include "licenseproperties.hpp"

//Para el crono.
#include "m4clock.hpp"

//Para la funcion de sleep.
#include "m4date.hpp"

#include "m4facdef.hpp"
#include "m4var.hpp"

#include "clfactm4objservice.hpp"

#include "m4ipcs.hpp"
#include "blocksynchronization.hpp"

//----------------------------------------------
//Clase ClLicenseChannelLoader
//----------------------------------------------
ClLicenseChannelLoader::ClLicenseChannelLoader( void )
{
	m_pClChannelManager = NULL;

	//Canal de mantenimiento de licencia.
	m_pChannel = NULL;
	m_pAccess = NULL;
	m_iLicenseIndex = M4DM_EOF_INDEX;

	//Contenido del registro de licencia.
	m_pLicenseBuffer = NULL;
	m_iLicenseBufferSize = 0;
	m_bInitialized = M4_FALSE;
}

ClLicenseChannelLoader::~ClLicenseChannelLoader( )
{
	m_pClChannelManager = NULL;
	End();
}

m4return_t ClLicenseChannelLoader::_CreateLicenseChannel( void )
{	
	m4return_t	iResult = M4_SUCCESS ;

	if (m_pClChannelManager == NULL) {
		return M4_ERROR;
	}

	iResult = m_pClChannelManager->CreateChannel( m_pChannel );
	if ((iResult == M4_ERROR) || (m_pChannel == NULL))	{
		return M4_ERROR;
	}

	//En la inicializacion todavia no estoy logado, pero para poder hacer el BuildFromId
	//necesito tener un role.
	m_pChannel->Role_ID.Set (M4CL_SEC_SUPER_USER);

	iResult = m_pChannel->BuildFromIdNoSec( M4_LICENSE_CHANNEL_ID ) ;
	if (iResult == M4_ERROR) {
		return M4_ERROR;
	}
	
	return iResult;
}

m4return_t	ClLicenseChannelLoader::_CreateLicenseAccess( void )
{
	m4return_t	iResult = M4_SUCCESS ;
	ClNode	* pNode = NULL ;

	if (m_pClChannelManager == NULL) {
		return M4_ERROR;
	}

	//No puede crearse un access si el canal no ha sido creado previamente.
	if (m_pChannel == NULL) {
		return M4_ERROR;
	}

	//Estamos ligados al CM correcto, ahora creamos el access
	iResult = m_pChannel->CreateAccess( m_pAccess ) ;
	if ((iResult == M4_ERROR) || (m_pAccess == NULL)) {
		return M4_ERROR;
	}
	
	//Nos situamos en el nodo.
	pNode = &( m_pAccess->Node[ M4_LICENSE_NODE_ID ] ) ;
	if (pNode == NULL) {
		return M4_ERROR;
	}

	//Cargamos el canal.
	iResult = m_pChannel->Load();
	if (iResult == M4_ERROR) {
		return M4_ERROR;
	}

	//Tiene que haber exactamente un registro de licencia.
	m4int32_t iCount = pNode->RecordSet.Count();
	if (iCount != 1) {
		return M4_ERROR;
	}

	//Nos situamos en el primer y unico registro.
	iResult = pNode->RecordSet.Register.Begin();
	if (iResult == M4_ERROR) {
		return M4_ERROR;
	}

	// Del recorset del nodo, obtenemos el indice del item que nos da la licencia.
	m_iLicenseIndex = pNode->RecordSet.Register.Item[M4_LICENSE_ITEM_ID].GetIndex();
	if (m_iLicenseIndex == M4DM_EOF_INDEX) {
		return M4_ERROR;
	}

	//Ya podemos ver el valor del item que se ha traido el canal.
	ClItem *pItem = &pNode->RecordSet.Register.Item[m_iLicenseIndex];
	if (pItem == NULL) {
		return M4_ERROR;
	}

	m4pchar_t pLicense = (m4pchar_t) (pItem->Value.Get());
	
	//Tiene que tener valor.
	if (pLicense == NULL){
		return M4_ERROR;
	}

	m4int32_t iSizeLicense = strlen(pLicense);
	if (iSizeLicense == 0){
		return M4_ERROR;
	}
	
	m_pLicenseBuffer = pLicense;
	m_iLicenseBufferSize = iSizeLicense;

	return( M4_SUCCESS ) ;
}

m4return_t ClLicenseChannelLoader::Init( ClChannelManager * ai_pClChannelManager )
{
	m_pClChannelManager = ai_pClChannelManager;

	//Creamos canal.
	if (_CreateLicenseChannel() == M4_ERROR) {
		End();
		return M4_ERROR;
	}

	//Creamos access que cargara el canal y obtendremos el item de la licencia.
	if (_CreateLicenseAccess() == M4_ERROR) {
		End();
		return M4_ERROR;
	}

	m_bInitialized = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClLicenseChannelLoader::End( void )
{
	if (m_pAccess != NULL) {
		if (m_pAccess->Destroy() == M4_ERROR) {
			return M4_ERROR;
		}
		m_pAccess = NULL;
	}

	//Destruimos el canal.
	if (m_pChannel != NULL) {
		if (m_pChannel->Destroy() == M4_ERROR) {
			return M4_ERROR;
		}
		m_pChannel = NULL;
	}

	m_iLicenseIndex = M4DM_EOF_INDEX;

	m_pLicenseBuffer = NULL;
	m_iLicenseBufferSize = 0;

	m_bInitialized = M4_FALSE;

	return M4_SUCCESS;
}

const m4char_t * ClLicenseChannelLoader::GetpLicenseBuffer( void ) const
{
	return m_pLicenseBuffer;
}

m4int32_t ClLicenseChannelLoader::GetLicenseBufferSize( void ) const
{
	return m_iLicenseBufferSize;
}

//----------------------------------------------
//FIN Clase ClLicenseChannelLoader
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNPNotifier
//----------------------------------------------

ClExecuteNPNotifier::ClExecuteNPNotifier(pfNPEventHandler ai_pfHandler)
{
	m_pfNotifier = ai_pfHandler;
}

m4void_t ClExecuteNPNotifier::Notify(m4int32_t ai_iState)
{
	if (m_pfNotifier != NULL)
	{
		(*m_pfNotifier)(ai_iState);
	}
}


//----------------------------------------------
//FIN Clase ClExecuteNPNotifier
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNp
//----------------------------------------------
ClExecuteNp::ClExecuteNp( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier )
	:m_oSyncroState(M4_TRUE)
{
	m_pLicenseManager = ai_pLicenseManager;
	m_pClChannelManagerSystem = ai_pClChannelManagerSystem;
	m_pChannelSystem = NULL;
	m_pAccessSystem = NULL;
	m_pClLicense = ai_pClLicense;
	m_eStateValidateExecutionNp = STATE_VALIDATE_EXECUTION_NP_RED;
	m_pNotifier = ai_pNotifier;
}

ClExecuteNp::~ClExecuteNp( )
{
	m_pLicenseManager = NULL;	
	m_pClChannelManagerSystem = NULL;
	m_pClLicense = NULL;

	if (m_pAccessSystem != NULL) {
		m_pAccessSystem->Destroy();
	}
	m_pAccessSystem = NULL;

	//Destruimos el canal.
	if (m_pChannelSystem != NULL) {
		m_pChannelSystem->Destroy();
	}
	m_pChannelSystem = NULL;

	if (m_pNotifier != NULL)
	{
		delete m_pNotifier;
		m_pNotifier = NULL;
	}
}

m4return_t ClExecuteNp::_CreateMetaChannelAndSystemChannel( void )
{
	//Creamos el metacanal que nos permitira tener nuestro propio canal que se encarga de lanzar sentencias.
	m4return_t	iResult = M4_SUCCESS ;

	//-----------------------------------------------------------------------------------
	//Creación del MetaCanal.
	//-----------------------------------------------------------------------------------
	if (m_pClChannelManagerSystem == NULL) {
		return M4_ERROR;
	}

	ClChannel *pMetaChannel = NULL;
	iResult = m_pClChannelManagerSystem->CreateChannel( pMetaChannel );
	if ((pMetaChannel == NULL) || (iResult == M4_ERROR))	{
		return M4_ERROR;
	}

	//Necesitamos un role.
	pMetaChannel->Role_ID.Set (M4CL_SEC_SUPER_USER);

	//Se crea igual al canal LOAD del sistema.
	iResult = pMetaChannel->BuildAsMetachannel() ;
	if (iResult == M4_ERROR) {
		pMetaChannel->Destroy();
		return M4_ERROR;
	}	

	//---------------- Ya tenemos el metacanal --------------------

	//-----------------------------------------------------------------------------------
	//Creación del MetaAccess.
	//-----------------------------------------------------------------------------------
	//Creamos el access.
	ClAccess * pAccess = NULL;

	iResult = pMetaChannel->CreateAccess( pAccess ) ;
	if ((iResult == M4_ERROR) || (pAccess == NULL)) {
		pMetaChannel->Destroy();
		return M4_ERROR;
	}
	
	//--- Vamos a crear la estructura del metacanal ---
	if (_CreateMetaChannelStruct(pAccess) == M4_ERROR) {
		pAccess->Destroy();
		pMetaChannel->Destroy();
		return M4_ERROR;
	}

	//Importante!:
	//Necesitamos crear el canal del sistema, es el canal sobre el que vamos a lanzar la sentencia!.	
	iResult = m_pClChannelManagerSystem->CreateChannel( m_pChannelSystem );
	if ((m_pChannelSystem == NULL) || (iResult == M4_ERROR))	{
		pAccess->Destroy();
		pMetaChannel->Destroy();
		return M4_ERROR;
	}

	//Necesitamos un role.
	m_pChannelSystem->Role_ID.Set (M4CL_SEC_SUPER_USER);

	//Creamos un canal del sistema. Se crea a partir del metacanal que acabamos de construir.

	//Se creara el mt del canal que hemos definido.
	iResult = m_pChannelSystem->BuildFromAccess(pAccess);
	if (iResult == M4_ERROR) {
		//No se ha podido crear el canal del sistema.
		pAccess->Destroy();
		pMetaChannel->Destroy();
		return M4_ERROR;
	}

	//Creamos el access del sistema para poder trabajar.
	iResult = m_pChannelSystem->CreateAccess( m_pAccessSystem ) ;
	if ((iResult == M4_ERROR) || (m_pAccessSystem == NULL)) {
		pAccess->Destroy();
		pMetaChannel->Destroy();
		m_pChannelSystem->Destroy();
		m_pChannelSystem = NULL;
		return M4_ERROR;
	}
	//-----------------------------------------------------------------------------------

	//--------------------------------------
	//Podemos eliminar el metacanal.
	if (pAccess != NULL) {
		pAccess->Destroy();
	}

	//Destruimos el canal.
	if (pMetaChannel != NULL) {
		pMetaChannel->Destroy();
	}
	//--------------------------------------

	return iResult;
}

m4return_t ClExecuteNp::_CreateMetaChannelStruct( ClAccess * ai_pAccess )
{
	//Creamos la estructura de nodos, ti,... del canal del sistema.

	if (ai_pAccess == NULL) {
		return M4_ERROR;
	}

	ClAccessRecordSet * pDRecordSet = NULL;
	m4VariantType vValueString;
	vValueString.Type = M4CL_CPP_TYPE_STRING_VAR;
	m4VariantType vValueNumber;
	vValueNumber.Type = M4CL_CPP_TYPE_NUMBER;

	//----------------------------------------------------------------------------
	//T3S.	    
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_T3S].RecordSet);
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_T3S_ID;
	pDRecordSet->Current.Item[ M4MDFAC_T3S_ID_T3 ].Value.Set( vValueString ) ;

	//Indicamos que NO es societario.
	vValueNumber.Data.DoubleData = M4CL_ORGANIZATION_TYPE_NONE;
	pDRecordSet->Current.Item[ M4MDFAC_T3S_ID_ORGANIZATION_TYPE ].Value.Set( vValueNumber ) ;

	//owner flag 1.
	vValueNumber.Data.DoubleData = 1;
	pDRecordSet->Current.Item[ M4MDFAC_T3S_OWNER_FLAG ].Value.Set( vValueNumber ) ;

	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//NODO
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_NODES].RecordSet);
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_NODE_ID;
	pDRecordSet->Current.Item[ M4MDFAC_NODES_ID_NODE ].Value.Set( vValueString ) ;
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	pDRecordSet->Current.Item[ M4MDFAC_NODES_ID_TI ].Value.Set( vValueString ) ;
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//TI
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_TIS].RecordSet);
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	pDRecordSet->Current.Item[ M4MDFAC_TIS_ID_TI ].Value.Set( vValueString ) ;
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//ITEM.
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_ITEMS].RecordSet);
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_TI ].Value.Set( vValueString ) ;
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_ITEM ].Value.Set( vValueString ) ;
	//Es de bloque.
	vValueNumber.Data.DoubleData = M4CL_ITEM_SCOPE_BLOCK;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_SCOPE_TYPE ].Value.Set( vValueNumber ) ;
	//Es long.
	vValueNumber.Data.DoubleData = M4CL_M4_TYPE_LONG;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_M4_TYPE ].Value.Set( vValueNumber ) ;
	//Es un concepto.
	vValueNumber.Data.DoubleData = M4CL_ITEM_TYPE_CONCEPT;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_ITEM_TYPE ].Value.Set( vValueNumber ) ;
	//Hay argumentos variables.
	vValueNumber.Data.DoubleData = M4_TRUE;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_VARIABLE_ARGUMENTS ].Value.Set( vValueNumber ) ;
	//----------------------------------------------------------------------------
	
	//--------------------------------------------------------------------
	//RULES.
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_RULES].RecordSet);	
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	pDRecordSet->Current.Item[ M4MDFAC_RULES_ID_TI ].Value.Set( vValueString ) ;
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID;
	pDRecordSet->Current.Item[ M4MDFAC_RULES_ID_ITEM ].Value.Set( vValueString ) ;
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID;
	pDRecordSet->Current.Item[ M4MDFAC_RULES_ID_RULE ].Value.Set( vValueString ) ;

	//Mapeamos la regla a un ExecuteRealSQL
	vValueNumber.Data.DoubleData = M4CL_LENGUAGE_CPP;	
	pDRecordSet->Current.Item[ M4MDFAC_RULES_ID_CODE_TYPE ].Value.Set( vValueNumber ) ;	
	//** vValueString.Data.PointerChar = "ExecuteRealSQL";
	vValueString.Data.PointerChar = "ExecuteSQL";
	pDRecordSet->Current.Item[ M4MDFAC_RULES_ID_METHOD ].Value.Set( vValueString ) ;
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	//ARGUMENTOS. El ExecuteReal tiene un argumento fijo que es la conexion.
	//Pero ExecuteSQL no tiene la conexión como argumento, por lo tano comentamos.
	//** pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_ITEM_ARGS].RecordSet);	
	//** if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
	//** 	return M4_ERROR;
	//** }

	//Pk
	//** vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	//** pDRecordSet->Current.Item[ M4MDFAC_ITEM_ARGS_ID_TI ].Value.Set( vValueString ) ;		
	//** vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID;
	//** pDRecordSet->Current.Item[ M4MDFAC_ITEM_ARGS_ID_ITEM ].Value.Set( vValueString ) ;

	//Indicamos el tipo de argumento.
	//** vValueNumber.Data.DoubleData = M4CL_M4_TYPE_NUMBER;
	//** pDRecordSet->Current.Item[ M4MDFAC_ITEM_ARGS_ID_M4_TYPE ].Value.Set( vValueNumber ) ;
	//** vValueNumber.Data.DoubleData = 18;
	//** pDRecordSet->Current.Item[ M4MDFAC_ITEM_ARGS_PREC ].Value.Set( vValueNumber ) ;
	//Posicion.
	//** vValueNumber.Data.DoubleData = M4CL_M4_TYPE_NUMBER;
	//** pDRecordSet->Current.Item[ M4MDFAC_ITEM_ARGS_POSITION ].Value.Set( vValueNumber ) ;
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//Creamos el ITEM: Recoger valor de la select.
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	//ITEM RETORNO.
	pDRecordSet = &(ai_pAccess->Node[M4MDFAC_NODE_SYS_ITEMS].RecordSet);
	if ((pDRecordSet == NULL) || (pDRecordSet->Current.Add() == M4_ERROR)) {
		return M4_ERROR;
	}

	//Pk
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_TI_ID;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_TI ].Value.Set( vValueString ) ;
	vValueString.Data.PointerChar = (m4pchar_t) M4_LICENSE_SYSTEM_EXECUTE_REAL_RETURN_ITEM_ID;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_ITEM ].Value.Set( vValueString ) ;
	//Es de registro.
	vValueNumber.Data.DoubleData = M4CL_ITEM_SCOPE_REGISTER;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_SCOPE_TYPE ].Value.Set( vValueNumber ) ;
	//Es entero.
	vValueNumber.Data.DoubleData = M4CL_M4_TYPE_NUMBER;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_M4_TYPE ].Value.Set( vValueNumber ) ;
	//Es una propiedad.
	vValueNumber.Data.DoubleData = M4CL_ITEM_TYPE_PROPERTY;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_ID_ITEM_TYPE ].Value.Set( vValueNumber ) ;
	//Precision.
	vValueNumber.Data.DoubleData = 10;
	pDRecordSet->Current.Item[ M4MDFAC_ITEMS_PREC ].Value.Set( vValueNumber ) ;
	//----------------------------------------------------------------------------

	return M4_SUCCESS;
}

m4void_t ClExecuteNp::_SetNPState(eStateValidateExecutionNp_t ai_iState)
{
	m_eStateValidateExecutionNp = ai_iState;
	if (m_pNotifier != NULL)
	{
		m_pNotifier->Notify(ai_iState);
	}
}

m4return_t ClExecuteNp::_ValidateExecutionNp( void )
{
	ClMutBlock oCriticalSection (m_oSyncroState);

	//Temporalmente.
	eStateValidateExecutionNp_t eMaxState = STATE_VALIDATE_EXECUTION_NP_GREEN;

	//Este objeto debería ser capaz de lanzar la select.
	if (m_pClLicense != NULL) {
		//Ahora obtenemos la informacion de NP, lanzamos y validamos.
		const ClNp * pClNp = m_pClLicense->GetNp();
		if (pClNp != NULL) {
			m4int32_t iNumNpItem = pClNp->GetNumNpItem();
			for (m4int32_t j=0; j<iNumNpItem; j++) {
				const ClNpItem * pClNpItem = pClNp->GetNpItemByIndex(j);
				if (pClNpItem != NULL) {
					const string & sSelect = pClNpItem->GetSelect();
					m4int32_t iEffectiveAuthorized = ClExecuteNp::StringToNumber(pClNpItem->GetEffectiveAuthorized());
					m4int32_t iLimit = ClExecuteNp::StringToNumber(pClNpItem->GetLimit());
					m4int32_t iCount = 0;
					//Ejecuta la sentencia.
					if (_ExecuteSelectNp(sSelect, iCount) == M4_ERROR) {
						//Directamente establecemos a alerta!.
						_SetNPState(STATE_VALIDATE_EXECUTION_NP_RED);
						return M4_ERROR;
					}																	

					// Se actualiza el count
					m4char_t acTemp[ 128 ] ;
					sprintf( acTemp, "%.0d", iCount ) ;
					((ClNpItem *)pClNpItem)->SetValue( acTemp ) ;

					if (iCount ==  M4DM_EOF_INDEX) {
						//Directamente establecemos a alerta!.
						_SetNPState(STATE_VALIDATE_EXECUTION_NP_RED);
						return M4_ERROR;
					}

					// El 0 se trata como infinito
					if( iEffectiveAuthorized == 0 )
					{
						continue;
					}

					if (iCount <= iEffectiveAuthorized ) {
						//Estamos por debajo por lo tanto en esta licencia todo ha ido bien.								
						//El estado maximo no lo cambio.
						continue;
					}

					if ((iEffectiveAuthorized < iCount) && (iCount <= iLimit)) {
						//Estamos entre limites, dariamos un aviso, pero puede ser peor.
						if (eMaxState < STATE_VALIDATE_EXECUTION_NP_YELLOW) {
							eMaxState = STATE_VALIDATE_EXECUTION_NP_YELLOW;
						}
						continue;
					}

					if (iCount > iLimit) {
						//Estamos fuera del limite. Daremos cada vez avisos. Salimos pq no puede ser peor.
						_SetNPState(STATE_VALIDATE_EXECUTION_NP_RED);

						return M4_SUCCESS;
					}
				}
			}
		}
	}

	_SetNPState(eMaxState);

	return M4_SUCCESS;
}

m4return_t ClExecuteNp::_ExecuteSelectNp( const string & ai_sSelect, m4int32_t & ao_iCount)
{
	m4return_t iResult = M4_ERROR;
	ao_iCount = M4DM_EOF_INDEX;

	if ((m_pChannelSystem == NULL) || (m_pAccessSystem == NULL)) {
		return M4_ERROR;
	}

	//Si tenemos access, eliminamos el bloque,  ejecutamos el item (concepto) que hace la carga.
	//La select que nos pasan sera del tipo: SELECT zzz = #COUNT(CAMPO) FROM TABLA_LOGICA
	//Tenemos que convertirla a: SELECT @ITEM = #COUNT(CAMPO) FROM TABLA_LOGICA
	//La sentencia tiene que empezar con "SELECT"

	m4int32_t iPos = M4DM_EOF_INDEX;
	string sSelect(ai_sSelect);
	string sItem("");		
	iPos = sSelect.find("#COUNT");
	if (iPos == M4DM_EOF_INDEX) {
		return M4_ERROR;
	}
	sItem.append("SELECT @"); sItem.append(M4_LICENSE_SYSTEM_EXECUTE_REAL_RETURN_ITEM_ID); sItem.append(" = ");
	
	//Sustituimos toda la subcadena, desde la posicion 0 hasta donde hemos 
	//encontrado el count!.
	sSelect.replace(0, iPos, sItem);

	//Lanzamos la sentencia.
	ClNode * pNode = &( m_pAccessSystem->Node[ M4_LICENSE_SYSTEM_NODE_ID ] ) ;
	if (pNode == NULL) {
		return M4_ERROR;
	}
	//Antes que nada, limpiamos el bloque!.
	if (pNode->RecordSet.Destroy() == M4_ERROR) {
		return M4_ERROR;		
	}

	//Selecciono el item que lanzara la sentencia.
	ClItem *pExecuteReal = &(pNode->RecordSet.Item[M4_LICENSE_SYSTEM_EXECUTE_REAL_ITEM_ID]);	
	if (pExecuteReal == NULL) {
		return M4_ERROR;
	}

	//Ejecutamos el concepto!.
	m4VariantType vValueString;
	vValueString.Type = M4CL_CPP_TYPE_STRING_VAR;
	vValueString.Data.PointerChar = (m4pchar_t) sSelect.c_str();

	//Metemos como argumento la conexion fisica donde hay que ejecutar.
	//En ExecuteSQL no es necesario ningun argumento!.
	//** m4VariantType vValueNumber;
	//** vValueNumber.Type = M4CL_CPP_TYPE_NUMBER;
	//** vValueNumber.Data.DoubleData = 0;
	//** pExecuteReal->StackPush(vValueNumber);

	//Es un concepto, por lo tanto primero hacemos un set y despues invocamos su ejecucion.
	pExecuteReal->Value.Set( vValueString );

	//Ejecutamos. El primer argumento es el ejecutor y el segundo es el número de argumentos (son opcionales).
	//Lo indicamos pq realmente sabemos que vamos a meter sólo un argumento.
	//** iResult = pExecuteReal->Call(NULL, 1);
	iResult = pExecuteReal->Call(NULL);
	if (iResult == M4_ERROR) {		
		return M4_ERROR;
	}

	//Miramos la pila.
	m4VariantType vValue;
	iResult = pExecuteReal->StackPop(vValue);
	if ((iResult == M4_ERROR) || (vValue.Type != M4CL_CPP_TYPE_NUMBER) || (vValue.Data.DoubleData != M4_SUCCESS)) {
		return M4_ERROR;
	}

	//--------------------------------------------------------------
	//Obtencion del resultado.
	//Sólo me vale que haya un registro.
	m4int32_t iCount = pNode->RecordSet.Count();
	if (iCount != 1) {
		return M4_ERROR;
	}
	//Nos situamos en el primer y unico registro.
	iResult = pNode->RecordSet.Register.Begin();
	if (iResult == M4_ERROR) {
		return M4_ERROR;
	}

	//Selecciono el item donde vendra el resultado.
	ClItem *pExecuteRealReturn = &(pNode->RecordSet.Register.Item[M4_LICENSE_SYSTEM_EXECUTE_REAL_RETURN_ITEM_ID]);
	if (pExecuteRealReturn == NULL) {
		return M4_ERROR;
	}
	//--------------------------------------------------------------

	//Despues de la ejecucion se ha tenido que rellenar el item.
	m4VariantType vValueCount;
	pExecuteRealReturn->Value.Get(vValueCount);
	if (vValueCount.Type != M4CL_CPP_TYPE_NUMBER) {
		return M4_ERROR;
	}

	ao_iCount = (m4int32_t) vValueCount.Data.DoubleData;

	return M4_SUCCESS;
}

m4int32_t ClExecuteNp::StringToNumber( const string & ai_sNumber )
{
	//Convertimos una cadena en un número entero.	
	if (ai_sNumber.size() == 0) {
		return 0;
	}
	return atoi(ai_sNumber.c_str());
}

m4bool_t ClExecuteNp::ValidateLicense( void )
{
	if (m_pClLicense != NULL) {
		return m_pClLicense->ValidateLicenseExpiration();
	}
	return M4_FALSE;
}


//----------------------------------------------
//FIN Clase ClExecuteNp
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNpMonoThread
//----------------------------------------------
ClExecuteNpMonoThread::ClExecuteNpMonoThread( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier )
	:ClExecuteNp( ai_pLicenseManager, ai_pClChannelManagerSystem, ai_pClLicense, ai_pNotifier )
	, m_oClock()
{
	//Importante!
	//Se inicializa el timer que controlará la periodicidad de lanzamiento de sentencia.!.
}

ClExecuteNpMonoThread::~ClExecuteNpMonoThread( )
{
}

m4return_t ClExecuteNpMonoThread::StartExecute (  m4pchar_t ai_pId )
{
	//Importante!.
	//El metodo de StartExecute en un monothread TIENE que lanzar directamente la ejecución!.
	//En monothread el Start-Run se hace en un solo paso!.

	Run();
	return M4_SUCCESS;
}

m4bool_t ClExecuteNpMonoThread::IsRunningExecute ( void )
{
	//En monothread podemos retornar directamente falso pq asumimos que ya no estamos ejecutando.
	return M4_FALSE;
}

m4return_t ClExecuteNpMonoThread::FinishExecute ( void )
{
	//En monothread no hace falta.
	return M4_SUCCESS;
}

void ClExecuteNpMonoThread::Run (void )
{
	//----------------------------------------------------------------------------
	//Creamos el metacanal y el canal del sistema que sera el que utilizaremos para lanzar las select.
	if (_CreateMetaChannelAndSystemChannel() == M4_ERROR) {
		return;
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//Lanzamos la sentencia de validacion ya!.
	_ValidateExecutionNp();
	//----------------------------------------------------------------------------

	//Cuando estamos en Monothread, la inicializacion acaba llamando a Run, pero no hay 
	//nadie bloqueado en ningun semaforo.
	//Por lo tanto NO hay que hace un UnLock del ningun semaforo!.
	//--if (GetpLicenseManager() != NULL) {
	//--	GetpLicenseManager()->WaitingValidateExecutionNpUnLock();
	//--}
}

eStateValidateExecutionNp_t ClExecuteNpMonoThread::GetStateValidateExecutionNp ( void )
{	
	//Importante:
	//En monothread, el que hace GetState y el que hace Validate es el mismo thread,
	//por lo tanto no haría falta blqouear con secciones criticas.
	//De todas formas ponemos el mutex aunque un thread no se autobloquea en un mismo
	//mutex.
	ClMutBlock oCriticalSection (m_oSyncroState);

	//Importante!:
	//No deberiamos llamar a GetStateValidateExecutionNp si haber ejecutado previamente la 
	//validacion!!!.
	//La validacion se lanza con Run. Este a su vez con StartExecute. Y éste se invoca en 
	//la iniciacializacion!.

	//En monothread tenemos que simular el crono para volver a lanzar la select.
	long lElapsedTime = m_oClock.GetDifference();

	//Nos ha devuelto el tiempo en ms, y lo tenemos que comparar con segundos.
	if (M4MODKEY_TIME_FREQUENCY_EXECUTION_NP <= (lElapsedTime / 1000)) {
		//Nos hemos pasado de la frecuencia, es necesario lanzar la validacion de nuevo.
		//Se actualizara el estado.
		_ValidateExecutionNp( );

		//Reseteamos el crono para la nueva ocasion.
		m_oClock.Start();		
	}
	
	return m_eStateValidateExecutionNp;
}

//----------------------------------------------
//FIN Clase ClExecuteNpMonoThread
//----------------------------------------------

//----------------------------------------------
//Clase ClExecuteNpMultiThread
//----------------------------------------------
ClExecuteNpMultiThread::ClExecuteNpMultiThread( ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManagerSystem, const ClLicense * ai_pClLicense, ClExecuteNPNotifier *ai_pNotifier )
	:ClExecuteNp( ai_pLicenseManager, ai_pClChannelManagerSystem, ai_pClLicense, ai_pNotifier )
{
}

ClExecuteNpMultiThread::~ClExecuteNpMultiThread( )
{
}

m4return_t ClExecuteNpMultiThread::StartExecute ( m4pchar_t ai_pId )
{
	//Invoca al start del thread. El Start a su vez invocará al Run!.
	return M4Thread::Start(ai_pId);
}

m4bool_t ClExecuteNpMultiThread::IsRunningExecute ( void )
{
	//Nos preguntamos si el thread todavia esta ejecutandose.
	return M4Thread::IsRunning();
}

m4return_t ClExecuteNpMultiThread::FinishExecute ( void )
{
	//Queremos que finalice el thread.
	return M4Thread::Terminate();
}

void ClExecuteNpMultiThread::Run (void )
{
	//----------------------------------------------------------------------------
	//Creamos el metacanal y el canal del sistema que sera el que utilizaremos para lanzar las select.
	if (_CreateMetaChannelAndSystemChannel() == M4_ERROR) {
		return;
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//Periodicamente debe lanzar la validacion.
	while (IsTerminate() == M4_FALSE)
	{
		_ValidateExecutionNp();		
		m4sleep(M4MODKEY_TIME_FREQUENCY_EXECUTION_NP);		
	}
	//----------------------------------------------------------------------------
}

eStateValidateExecutionNp_t ClExecuteNpMultiThread::GetStateValidateExecutionNp ( void )
{
	ClMutBlock oCriticalSection (m_oSyncroState);

	//El init garantiza que seguro que se ha lanzado una vez la validacion!.
	return m_eStateValidateExecutionNp;
}

//----------------------------------------------
//FIN Clase ClExecuteNpMultiThread
//----------------------------------------------

//----------------------------------------------
//Clase ClLicenseThread
//----------------------------------------------
ClLicenseThread::ClLicenseThread( m4bool_t ai_bMultiThread, ILicenseManager * ai_pLicenseManager, ClChannelManager * ai_pClChannelManager )
{
	m_bMultiThread = ai_bMultiThread;
	m_pLicenseManager = ai_pLicenseManager;
	m_pClChannelManager = ai_pClChannelManager;
	_Init();
}

ClLicenseThread::~ClLicenseThread( )
{
	_End();
	m_pLicenseManager = NULL;
	m_pClChannelManager = NULL;
}

void ClLicenseThread::_Init( void )
{
	m_pClEncryption = new ClEncryption();
	m_pIXmlAnalyzer = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_LICENSE );
	m_pClExecuteNp = NULL;
	m_pClLicense = NULL;
	m_pClProperties = NULL;
}

void ClLicenseThread::_End( void )
{

	//Si estamos en monothread no es necesario esperar nada IsRunningExecute sera false!. 

	//-------------------------------------------------------
	if (m_pClExecuteNp != NULL) {
		//Mando al thread que termine!.
		m_pClExecuteNp->FinishExecute();
		//Me quedo esperando a que el thread se muera!.
		while (m_pClExecuteNp->IsRunningExecute() == M4_TRUE) {
			m4sleep(M4MODKEY_TIME_WAIT_THREAD);
		}
		delete m_pClExecuteNp;
		m_pClExecuteNp = NULL;
	}
	//-------------------------------------------------------

	if (m_pClEncryption != NULL) {
		delete m_pClEncryption;
		m_pClEncryption = NULL;
	}

	if (m_pClProperties != NULL) {
		delete m_pClProperties;
		m_pClProperties = NULL;
	}

	ClXmlAnalyzerFactory::DestroyInstanceXmlAnalyzer(m_pIXmlAnalyzer);
	m_pClLicense = NULL;
}

void ClLicenseThread::_Reset( void )
{
	_End();
	_Init();
}

m4return_t ClLicenseThread::Init( const m4char_t * ai_pLicenseEncrypted, m4int32_t ai_iSizeLicenseEncrypted, ClExecuteNPNotifier *ai_pNotifier )
{
	//Importante!:
	//Solo se debería admitir un init pq el analisis es único.
	//Ademas, no puede estar creado el thread!.
	if ((ai_pLicenseEncrypted == NULL) || (m_pIXmlAnalyzer == NULL) || (m_pClExecuteNp != NULL)) {
		return M4_ERROR;
	}

	//Despues de esta llamada, en el analizador está toda la informacion de la licencia.
	//Se invoca al analizador, pasando la licencia encriptada, me devolverá un puntero
	//a la licencia.
	const ClLicense * pClLicense = m_pIXmlAnalyzer->GetpClLicense(ai_pLicenseEncrypted, ai_iSizeLicenseEncrypted);
	if (pClLicense == NULL) {
		_Reset();
		return M4_ERROR;
	}

	//Si el analisis ha ido bien indicamos la licencia.
	//Creamos e iniciamos el thread. (Seguro que no esta creado todavía).
	m_pClLicense = pClLicense;

	//Creamos las propiedades a partir de la licencia.
	ClModKeyProperties * pClProperties = NULL;
	m_pClLicense->CreatePropertiesByLicense(pClProperties);
	if (pClProperties == NULL) {
		_Reset();
		return M4_ERROR;		
	}

	m_pClProperties = pClProperties;

	if (m_bMultiThread == M4_TRUE) {
		m_pClExecuteNp = new ClExecuteNpMultiThread(m_pLicenseManager, m_pClChannelManager, m_pClLicense, ai_pNotifier);
	}
	else {
		m_pClExecuteNp = new ClExecuteNpMonoThread(m_pLicenseManager, m_pClChannelManager, m_pClLicense, ai_pNotifier);
	}

	if (m_pClExecuteNp->StartExecute("Np Launcher") == M4_ERROR) {
		_Reset();		
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

eStateValidateExecutionNp_t ClLicenseThread::GetStateValidateExecutionNp ( void )
{
	if (m_pClExecuteNp != NULL) {
		return m_pClExecuteNp->GetStateValidateExecutionNp();
	}
	return STATE_VALIDATE_EXECUTION_NP_RED;
}

m4bool_t ClLicenseThread::ValidateLicense( void )
{
	if (m_pClExecuteNp != NULL) {
		return m_pClExecuteNp->ValidateLicense();
	}
	return M4_FALSE;
}

string ClLicenseThread::GetLicenseSummaryEncrypted( void )
{		
	//Importante:
	//Estamos asumiendo que el encriptado no tiene el nulo!.
	string sLicenseSummaryEncrypted("");
	if (m_pIXmlAnalyzer != NULL) {		
		const ClLicense * pClLicense = m_pIXmlAnalyzer->GetpClLicense();
		if (pClLicense != NULL) {
			m4pchar_t pLicenseSummaryEncrypted = NULL;
			m4int32_t iSizeLicenseSummaryEncrypted = 0;
			if (pClLicense->GetLicenseSummaryEncrypted(pLicenseSummaryEncrypted, iSizeLicenseSummaryEncrypted) == M4_SUCCESS) {			
				//Si ha ido bien tendremos un puntero que tendremos que liberar.
				sLicenseSummaryEncrypted.append(pLicenseSummaryEncrypted);
				delete [] pLicenseSummaryEncrypted;				
			}
		}
	}

	//Se estará retornando una copia.
	return sLicenseSummaryEncrypted;
}

m4pcchar_t ClLicenseThread::GetPropertyValueById( m4pcchar_t ai_pPropertyId )
{
	if (m_pClProperties != NULL) {
		return m_pClProperties->GetPropertyValueById(ai_pPropertyId);
	}
	return NULL;
}


//----------------------------------------------
//FIN Clase ClLicenseThread
//----------------------------------------------
