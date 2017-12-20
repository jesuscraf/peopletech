//=========================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                cldictio.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================


#include "dm.hpp"
#include "dm_types.hpp"
#include "cllgadap.hpp"

#include "defmain.hpp"
#include "clldb.hpp"
#include "cllconn.hpp"
#include "clmtsent.hpp"
#include "clstatem.hpp"
#include "cldinarr.hpp"
#include "ldb_log.hpp"
#include "m4mdfac.hpp"
#include "access.hpp"
#include "ldbcontext.hpp"
#include "blocksynchronization.hpp"
#include "m4lgadapdef.hpp"
#include "clload.hpp"
#include "iexecutor.hpp"
#include "chan_knl.hpp"

m4cint8_t g_aM4ADM[] = "M4ADM";

#ifdef _M4LDB_DEBUG
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebug
#endif




m4return_t ClLConn::CreateSysAccess (void)
{
	m4return_t result;

	if (!m_pAcsDict)
	{
		ClAccess *pAcsTr = NULL;
		result = m_pLdb->m_pFactory->MDFac_BuildDictAccess(m_pAcsDict,pAcsTr);
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		result = m_oNameTrans.Init(pAcsTr,this);
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

        // Vale ya tenemos el access, pero nos aseguramos de que tiene todos
        // los nodos a los que vamos a acceder.
        if (m_pAcsDict->Node.Count() < M4LDB_SYS_NODES_TOTAL)
		{
        	return (M4_ERROR);
        }
		
		m_paRS[M4LDB_SYS_DICT_DDBBS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DDBBS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_TRANS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_TRANS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_ERRORS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ERRORS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_FIELDS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FIELDS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_CLS_DATE] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_CLS_DATE].RecordSet;
		m_paRS[M4LDB_SYS_DICT_AUD_LOBJ] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_AUD_LOBJ].RecordSet;
		m_paRS[M4LDB_SYS_DICT_FOREIGNS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FOREIGNS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_ORG_TREE] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_TREE].RecordSet;
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_SEC_LOBJ].RecordSet;
		m_paRS[M4LDB_SYS_DICT_FUNCTIONS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FUNCTIONS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_ORG_FIELDS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_FIELDS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_ORG_LEVELS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_LEVELS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_SEC_FIELDS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_SEC_FIELDS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_CONNECTIONS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_CONNECTIONS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_INHERITANCE] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_INHERITANCE].RecordSet;
		m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_REAL_OBJECTS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_SEC_LOBJ_FIL].RecordSet;
		m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_TRANS_FIELDS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_GLOBAL_PARAMS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_INSERT_PARAMS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_INSERT_PARAMS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_LOGIC_OBJECTS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_EXTENDED_TYPES].RecordSet;
		m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_FOREIGN_PARAMS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_MIRROR_OBJECTS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_CHILD_ORG_FIELDS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_DEFAULT_LANGUAGE] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DEFAULT_LANGUAGE].RecordSet;
		m_paRS[M4LDB_SYS_DICT_ORG_LEVELS_TRANS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_ORG_LEVELS_TRANS].RecordSet;
		m_paRS[M4LDB_SYS_DICT_DYNAMIC_LANGUAGES] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_DYNAMIC_LANGUAGES].RecordSet;
		m_paRS[M4LDB_SYS_DICT_NEXT_ID_EXECUTION] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_NEXT_ID_EXECUTION].RecordSet;
		m_paRS[M4LDB_SYS_DICT_APPLICATION_PARAMS] = &m_pAcsDict->Node[(ClNodeIndex) M4LDB_SYS_DICT_APPLICATION_PARAMS].RecordSet;
		
		if (SetCurrentDictIndexes()!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
    
	return (M4_SUCCESS);
}

m4return_t ClLConn::SetCurrentDictIndexes(void)
{
	m_paRS[M4LDB_SYS_DICT_AUD_LOBJ]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_ERRORS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_FIELDS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_DDBBS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_FUNCTIONS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_TRANS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_GLOBAL_PARAMS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_DYNAMIC_LANGUAGES]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_APPLICATION_PARAMS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_ORG_TREE]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_ORG_LEVELS]->SetCurrentSearchIndexByPos (0);
	m_paRS[M4LDB_SYS_DICT_ORG_LEVELS_TRANS]->SetCurrentSearchIndexByPos (0);

	m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS)->SetCurrentSearchIndexByPos (0);
	m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS)->SetCurrentSearchIndexByPos (0);
	m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS_TRANS)->SetCurrentSearchIndexByPos (0);

	return M4_SUCCESS;
}


m4return_t ClLConn::ReserveDictionary(void)
{
	// Se fuerza el bloqueo del channeldata
	m_pAcsDict->GetpChannel()->GetpChannel_Data()->ForceLock();

	// Le damos el doble para que tenga un colchón, por si acaso.
	m4int32_t iMaxLogicObject = m_pLdb->GetNumObject() * 2;

	// Redimensionado del vector punteros a registros de nivel cero
	m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->GetCurrentBlock()->Reserve( iMaxLogicObject );

	// Redimensionado de los vectores de punteros a bloque de primer nivel
	m_paRS[M4LDB_SYS_DICT_TRANS           ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_AUD_LOBJ        ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_CLS_DATE        ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_FOREIGNS        ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_SEC_LOBJ        ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_ORG_FIELDS      ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_INHERITANCE     ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS    ]->GetpNode_Data()->Reserve( iMaxLogicObject );
	m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS  ]->GetpNode_Data()->Reserve( iMaxLogicObject );

	// Se fuerza el bloqueo de los mapas de bloques de segundo nivel
	m_paRS[M4LDB_SYS_DICT_FIELDS          ]->GetpNode_Data()->ForceLock();
	m_paRS[M4LDB_SYS_DICT_SEC_FIELDS      ]->GetpNode_Data()->ForceLock();
	m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL    ]->GetpNode_Data()->ForceLock();
	m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS    ]->GetpNode_Data()->ForceLock();
	m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS  ]->GetpNode_Data()->ForceLock();
	m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS]->GetpNode_Data()->ForceLock();

	return M4_SUCCESS;
}


m4return_t ClLConn::ReserveDictionaryTrans(void)
{
	// Le damos el doble para que tenga un colchón, por si acaso.
	m4int32_t iMaxLogicObject = m_pLdb->GetNumObjectTrans() * 2;

	// Redimensionado del vector punteros a registros de nivel cero
	m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_LOGIC_OBJECTS)->GetCurrentBlock()->Reserve( iMaxLogicObject );

	// Redimensionado de los vectores de punteros a bloque de primer nivel
	m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS)->GetpNode_Data()->Reserve( iMaxLogicObject );

	if( m_pLdb->DynamicLanguages() > 0 )
	{
		m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS)->GetpNode_Data()->Reserve( iMaxLogicObject );
		m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS_TRANS       )->GetpNode_Data()->Reserve( iMaxLogicObject );
	}

	return M4_SUCCESS;
}


m4return_t ClLConn::CreateField( m4pcchar_t ai_pccId, m4pcchar_t ai_pccRealName, m4uint16_t ai_iPosPk, m4pcchar_t ai_pccType, m4uint16_t ai_iInternalType, m4bool_t ai_bNotNull, m4bool_t ai_bControlRepetition )
{
	m4VariantType	vAux; 

	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Add();

	// Se dan los valores
	vAux.SetStringRef( (const m4pchar_t)ai_pccId );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetStringRef( (const m4pchar_t)ai_pccRealName );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetDouble( ai_iPosPk );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetStringRef( (const m4pchar_t)ai_pccType );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetDouble( ai_iInternalType );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetDouble( ai_bNotNull );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_NOT_NULL].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetDouble( ai_bControlRepetition );
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_CTRL_REPETITION].Value.SetWithOutSec(vAux, M4_FALSE);

	return M4_SUCCESS;
}


m4return_t ClLConn::CloneTranslationField( m4pcchar_t ai_pccName )
{
	m4return_t	iResult = M4_ERROR;
	m4pcchar_t	pccLogicName = NULL;
	m4pcchar_t	pccRealName = NULL;

	// Se leen los valores del registro actual a clonar
	if( ai_pccName != NULL )
	{
		pccLogicName = ai_pccName;
		pccRealName = ai_pccName;
	}
	else
	{
		pccLogicName = M4LDB_ID(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec());
		pccRealName = M4LDB_ID(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec());
	}

	m4uint16_t iPosPk = M4LDB_NUMBER(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec());
	m4pcchar_t pccType = M4LDB_ID(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec());
	m4uint16_t iInternalType = M4LDB_NUMBER(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec());
	m4bool_t bNotNull = ((M4LDB_NUMBER(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_NOT_NULL].Value.GetWithOutSec())) ? M4_TRUE : M4_FALSE);
	m4bool_t bControlRepetition = ((M4LDB_NUMBER(m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_CTRL_REPETITION].Value.GetWithOutSec())) ? M4_TRUE : M4_FALSE);

	// Nos movemos a la tabla de lenguajes
	ClRegisterIndex oldTable = m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Index;
	ClRegisterIndex oldField = m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Index;

	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.End();

	iResult = CreateField( pccLogicName, pccRealName, iPosPk, pccType, iInternalType, bNotNull, bControlRepetition );

	// Y nos reposicionamos
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[ oldTable ];
	m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[ oldField ];

	if( iResult != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t ClLConn::CreateTranslationMetaData( m4uint32_t ai_iTableNumber, m4uint32_t ai_iTranslatedNumber, m4uint32_t ai_iDynamicLangNumber, m4pcchar_t ai_pccTransRealObject, m4uint8_t ai_iMirrorType )
{

	m4bool_t		bAdded = M4_FALSE;
	m4uint16_t		iLanguage = 0;
	m4uint32_t		iLength = 0;
	m4uint32_t		iFieldNumber = 0;
	m4pcchar_t		pccMirrorTransRealObject = NULL;
	m4pcchar_t		pccTransBaseName = NULL;
	m4pcchar_t		pccTransPattern = NULL;
	m4pcchar_t		pccField = NULL;
	m4char_t		acBuffer[ 128 + 1 ];
	m4VariantType	vAux;

	ClRegisterIndex	indTrans(0);
	ClRegisterIndex	indTable(0);
	ClRegisterIndex	indField(0);
	ClRegisterIndex indLang(0);


	// Si añade la tabla física
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register.Add();

	vAux.SetStringRef( (const m4pchar_t) ai_pccTransRealObject );
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.SetWithOutSec(vAux, M4_FALSE);

	vAux.SetDouble(1);
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.SetWithOutSec(vAux, M4_FALSE);

	#ifdef _M4LDB_DEBUG
		m4int32_t lSysDebugLevel = m_lSysDebugLevel;
		m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
		M4LDB_PRINT_DEBUG (this, "\n\nTranslation: SYS_DICT_REAL_OBJECTS");
		M4LDB_PRINT_DEBUG (this, m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]);
		m_lSysDebugLevel = lSysDebugLevel;
	#endif

	if( ai_iMirrorType == 1 )
	{
		// Se añade la tabla real espejo de traducción
		pccMirrorTransRealObject = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Register[0].Item[M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_MIRROR_TRANS_REAL_OBJECT].Value.GetWithOutSec() );

		m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Register.Add();

		vAux.SetStringRef( (const m4pchar_t) ai_pccTransRealObject );
		m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Register.Item[M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_REAL_OBJECT].Value.SetWithOutSec(vAux, M4_FALSE);

		vAux.SetStringRef( (const m4pchar_t) pccMirrorTransRealObject );
		m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Register.Item[M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.SetWithOutSec(vAux, M4_FALSE);

		#ifdef _M4LDB_DEBUG
			m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
			M4LDB_PRINT_DEBUG (this, "\n\nTranslation: SYS_DICT_MIRROR_OBJECTS");
			M4LDB_PRINT_DEBUG (this, m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]);
			m_lSysDebugLevel = lSysDebugLevel;
		#endif
	}


	// Se añaden los campos de la pk
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo( ai_iTableNumber - 1 );

	iFieldNumber = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
	
	for( indField = 0; indField < iFieldNumber; indField++ )
	{
		m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indField];

		if( M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec()) != 0 )
		{
			CloneTranslationField();
		}
	}


	// Se añaden los campos traducibles
	for( indTrans = 0; indTrans < ai_iTranslatedNumber; indTrans++ )
	{
		m_paRS[M4LDB_SYS_DICT_TRANS]->Current[indTrans];
		
		pccTransBaseName = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_TRANS]->Current.Item[M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD_AUX].Value.GetWithOutSec() );
		iLength = strlen( pccTransBaseName );
		memcpy( acBuffer, pccTransBaseName, iLength );
		memcpy( acBuffer + iLength, "#", 2 );

		// Traducciones de los campos
		for( indLang = 0; indLang < ai_iDynamicLangNumber; indLang++ )
		{
			m_paRS[M4LDB_SYS_DICT_DYNAMIC_LANGUAGES]->Register[indLang];

			m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register.Add();

			iLanguage = M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_DYNAMIC_LANGUAGES]->Register.Item[M4LDB_ITEM_SYS_DICT_DYNAMIC_LANGUAGES_ID_LANGUAGE].Value.GetWithOutSec() );
			vAux.SetDouble( iLanguage );
			m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_LANG].Value.SetWithOutSec(vAux, M4_FALSE);

			vAux.SetStringRef( acBuffer );
			m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);
		}

		#ifdef _M4LDB_DEBUG
			m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
			M4LDB_PRINT_DEBUG (this, "\n\nTranslation: SYS_DICT_TRANS_FIELDS");
			M4LDB_PRINT_DEBUG (this, m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]);
			m_lSysDebugLevel = lSysDebugLevel;
		#endif

		// Campos
		pccTransPattern = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register[0].Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec() );
		bAdded = M4_FALSE;

		for( indTable = 0; indTable < ai_iTableNumber && bAdded == M4_FALSE; indTable++ )
		{
			m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indTable];

			iFieldNumber = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
			
			for( indField = 0; indField < iFieldNumber && bAdded == M4_FALSE; indField++ )
			{
				m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indField];

				pccField = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec() );

				if( pccField != NULL && strcmpi( pccField, pccTransPattern ) == 0 )
				{
					CloneTranslationField( acBuffer );
					bAdded = M4_TRUE;
				}
			}
		}
	}

	#ifdef _M4LDB_DEBUG
		m_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2;
		M4LDB_PRINT_DEBUG (this, "\n\nTranslation: SYS_DICT_FIELDS");
		m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.End();
		M4LDB_PRINT_DEBUG (m_pLdb->m_pSysLConn, m_paRS[M4LDB_SYS_DICT_FIELDS]);
		m_lSysDebugLevel = lSysDebugLevel;
	#endif

	return M4_SUCCESS;
}


m4return_t ClLConn::CreateTramitationView( ClRegisterIndex ai_iIndRealTbl, m4int32_t ai_iNumFields, m4pcchar_t ai_pccMirrorBase, m4pcchar_t ai_pccMirrorRealObject, m4pcchar_t ai_pccRealObject, m4bool_t ai_bIsTranslation )
{

	m4return_t		iResult = M4_ERROR;
	m4bool_t		bNPJoin = M4_FALSE;
	m4bool_t		bIsLong = M4_FALSE;
	m4int8_t		iM4Type = 0;
	eFldType_t		eFldType = M4LDB_NORMAL;
	m4int32_t		iIndType = M4LDB_NULL_INDEX;
	m4int32_t		iPosPk = 0;
	m4VariantType	vAux; 
	m4pcchar_t		pccIdField = NULL;
	m4pcchar_t		pccIdType = NULL;
	ClRegisterIndex i = 0;

	string sView;
	string sValues;
	string sTramitValues;
	string sUnionTramitValues;
	string sLongValues;
	string sPk;
	string sBasePk;
	string sBasePkUnion;

	sView.reserve( 1000 ) ;
	sValues.reserve( 200 ) ;
	sTramitValues.reserve( 50 ) ;
	sUnionTramitValues.reserve( 50 ) ;
	sLongValues.reserve( 100 ) ;
	sPk.reserve( 200 ) ;
	sBasePk.reserve( 200 ) ;
	sBasePkUnion.reserve( 200 ) ;

	sView = "" ;
	sValues = "" ;
	sTramitValues = "" ;
	sUnionTramitValues = "" ;
	sLongValues = "" ;
	sPk = "" ;
	sBasePk = "" ;
	sBasePkUnion = "" ;

	if( ai_bIsTranslation == M4_TRUE )
	{
		sValues = ", D.ID_LANGUAGE#" ;
		sPk = " AND DD.ID_LANGUAGE# = D.ID_LANGUAGE#" ;
	}

	/* Se calcula la vista, por ejemplo
	SELECT T.ID_TRAMITATION ID_DATASET, D.PK, D.DATA, D.DT_LAST_UPDATE, D.ID_APPROLE, D.ID_SECUSER FROM ZZ_JESUS_TRAMIT D, M4RTR_TRAMITATION T WHERE NOT EXISTS (SELECT 1 FROM ZZ_JESUS_TRAMIT_M DD WHERE DD.ID_DATASET = T.ID_TRAMITATION AND DD.PK = D.PK) UNION ALL SELECT D.ID_DATASET, D.PK, D.DATA, D.DT_LAST_UPDATE, D.ID_APPROLE, D.ID_SECUSER FROM ZZ_JESUS_TRAMIT_M D WHERE D.TRAMIT_OPERATION <> 0
	*/

	for( i = 0 ; i < ai_iNumFields ; i++ )
	{
		m_paRS[ M4LDB_SYS_DICT_FIELDS ]->Current[ i ] ;

		pccIdField = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec() ) ;
		iPosPk = M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec() ) ;
		eFldType = (eFldType_t) M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec() ) ;
		pccIdType = M4LDB_ID( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[ M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE ].Value.GetWithOutSec() ) ;
		iIndType = FindIndexById( M4LDB_SYS_DICT_EXTENDED_TYPES, pccIdType ) ;

		if( iPosPk != 0 )
		{
			sBasePk.append( " AND B." ) ;
			sBasePk.append( pccIdField ) ;
			sBasePk.append( " = DD." ) ;
			sBasePk.append( pccIdField ) ;

			sBasePkUnion.append( " AND B." ) ;
			sBasePkUnion.append( pccIdField ) ;
			sBasePkUnion.append( " = D." ) ;
			sBasePkUnion.append( pccIdField ) ;

			// Si es fecha de inicio se filtra por los de tramitación
			if( ai_iIndRealTbl != 0 && eFldType == M4LDB_DT_START )
			{
				// Si no es la base hay que hacer el join con la base para la fecha de inicio
				sPk.append( " AND B." ) ;
				bNPJoin = M4_TRUE ;
			}
			else
			{
				sPk.append( " AND DD." ) ;
			}

			if( eFldType == M4LDB_DT_START )
			{
				sPk.append( "TRAMIT_DT_START" ) ;
			}
			else
			{
				sPk.append( pccIdField ) ;
			}

			sPk.append( " = D." ) ;
			sPk.append( pccIdField ) ;
		}

		bIsLong = M4_FALSE ;
		if( iIndType != M4LDB_NULL_INDEX )
		{
			iM4Type = M4LDB_M4_TYPE( (m4int8_t) M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register.Item[ M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE ].Value.GetWithOutSec() ) ) ;

			if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BLOB || iM4Type == M4CL_M4_TYPE_BINARY_STRING )
			{
				bIsLong = M4_TRUE ;
			}
		}

		if( bIsLong == M4_TRUE )
		{
			sLongValues.append( ", D." ) ;
			sLongValues.append( pccIdField ) ;
		}
		else
		{
			sValues.append( ", D." ) ;
			sValues.append( pccIdField ) ;

			// Se pone la fecha de inicio de tramitación
			if( iPosPk != 0 && eFldType == M4LDB_DT_START )
			{
				sTramitValues.append( ", D." ) ;
				sTramitValues.append( pccIdField ) ;
				sTramitValues.append( " TRAMIT_DT_START" ) ;

				if( ai_iIndRealTbl != 0 )
				{
					sUnionTramitValues.append( ", B." ) ;
				}
				else
				{
					sUnionTramitValues.append( ", D." ) ;
				}
				sUnionTramitValues.append( "TRAMIT_DT_START" ) ;
			}

			// Se pone la fecha de actualización de tramitación
			if( iPosPk == 0 && eFldType == M4LDB_DT_LAST_UPDATE )
			{
				sTramitValues.append( ", D." ) ;
				sTramitValues.append( pccIdField ) ;
				sTramitValues.append( " TRAMIT_DT_LAST_UPDATE" ) ;

				if( ai_iIndRealTbl != 0 )
				{
					// En teoría por aquí no pasa porque el DT_LAST_UPDATE no está en las tablas secundarias
					sUnionTramitValues.append( ", B." ) ;
				}
				else
				{
					sUnionTramitValues.append( ", D." ) ;
				}
				sUnionTramitValues.append( "TRAMIT_DT_LAST_UPDATE" ) ;
			}
		}
	}

	sView.append( "(SELECT T.ID_TRAMITATION ID_DATASET" ) ;
	sView.append( sValues ) ;
	sView.append( sTramitValues ) ;
	sView.append( sLongValues ) ;
	sView.append( " FROM " ) ;
	sView.append( ai_pccRealObject ) ;
	sView.append( " D, M4RTR_TRAMITATION T WHERE NOT EXISTS (SELECT 1 FROM " ) ;
	sView.append( ai_pccMirrorRealObject ) ;
	sView.append( " DD" ) ;

	if( bNPJoin == M4_TRUE )
	{
		// Si hay join se pone la base
		sView.append( ", " ) ;
		sView.append( ai_pccMirrorBase ) ;
		sView.append( " B" ) ;
	}

	sView.append( " WHERE DD.ID_DATASET = T.ID_TRAMITATION" ) ;

	if( bNPJoin == M4_TRUE )
	{
		// Si hay join se pone la base
		sView.append( sBasePk ) ;
		/* Bug 0156815
		Hay que hacer el join por ID_TRAMITATION
		*/
		sView.append( " AND B.ID_DATASET = DD.ID_DATASET" ) ;
	}

	sView.append( sPk ) ;
	sView.append( ") UNION ALL SELECT D.ID_DATASET" ) ;
	sView.append( sValues ) ;
	sView.append( sUnionTramitValues ) ;
	sView.append( sLongValues ) ;
	sView.append( " FROM " ) ;
	sView.append( ai_pccMirrorRealObject ) ;
	sView.append( " D" ) ;

	if( ai_iIndRealTbl != 0 )
	{
		// Si no es base no tiene OPERATION, por lo que se hace el join
		sView.append( ", " ) ;
		sView.append( ai_pccMirrorBase ) ;
		sView.append( " B" ) ;
	}

	sView.append( " WHERE " ) ;

	if( ai_iIndRealTbl != 0 )
	{
		sView.append( "B" ) ;
	}
	else
	{
		sView.append( "D" ) ;
	}

	sView.append( ".TRAMIT_OPERATION <> 0" ) ;

	if( ai_iIndRealTbl != 0 )
	{
		sView.append( sBasePkUnion ) ;
		/* Bug 0156815
		Hay que hacer el join por ID_TRAMITATION
		*/
		sView.append( " AND B.ID_DATASET = D.ID_DATASET" ) ;
	}

	sView.append( ")" ) ;

	vAux.SetStringRef( (const m4pchar_t) sView.c_str() );
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW].Value.SetWithOutSec(vAux, M4_FALSE);


	// Se añade el campo ID_DATASET
	iResult = CreateField( "ID_DATASET", "ID_DATASET", 0, "DESCRIPTION", M4CL_INTERNAL_TYPE_ID_DATASET, M4_FALSE, M4_FALSE );

	if( iResult != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	return( M4_SUCCESS );
}


m4return_t ClLConn::CreateInheritView( ClRegisterIndex ai_iIndRealTbl, m4int32_t ai_iNumFields, m4pcchar_t ai_pccBase, m4pcchar_t ai_pccRealObject, m4bool_t ai_bIsTranslation )
{

	m4return_t		iResult = M4_ERROR;
	m4bool_t		bIsLong = M4_FALSE;
	m4int8_t		iM4Type = 0;
	eFldType_t		eFldType = M4LDB_NORMAL;
	m4int32_t		iIndType = M4LDB_NULL_INDEX;
	m4int32_t		iPosPk = 0;
	m4VariantType	vAux; 
	m4pcchar_t		pccIdField = NULL;
	m4pcchar_t		pccIdType = NULL;
	ClRegisterIndex i = 0;

	string sView;
	string sValues;
	string sLongValues;
	string sPk;
	string sBasePk;

	sView.reserve( 1000 ) ;
	sValues.reserve( 200 ) ;
	sLongValues.reserve( 100 ) ;
	sPk.reserve( 200 ) ;
	sBasePk.reserve( 200 ) ;

	sView = "" ;
	sValues = "" ;
	sLongValues = "" ;
	sPk = "" ;
	sBasePk = "" ;

	if( ai_bIsTranslation == M4_TRUE )
	{
		sValues = ", D.ID_LANGUAGE#" ;
		sPk = " AND DD.ID_LANGUAGE# = D.ID_LANGUAGE#" ;
	}

	/* Se calcula la vista, por ejemplo
	SELECT O.ID_LEVEL ID_ORG_LEVEL, T.ID_ORGANIZATION, D.PK, D.DATA, D.ID_APPROLE, D.ID_SECUSER, D.DT_LAST_UPDATE FROM ZZ_JESUS_MULTI_INH D, M4VCH_ORG_TREE T, M4RCH_ORGANIZATION O WHERE T.ID_PAR_ORGANIZATIO = D.ID_ORGANIZATION AND O.ID_ORGANIZATION = T.ID_ORGANIZATION AND D.OPERATION <> 0 AND T.DISTANCE = (SELECT MIN(TD.DISTANCE) FROM ZZ_JESUS_MULTI_INH DD, M4VCH_ORG_TREE TD WHERE TD.ID_PAR_ORGANIZATIO = DD.ID_ORGANIZATION AND DD.PK = D.PK AND TD.ID_ORGANIZATION = T.ID_ORGANIZATION)
	*/

	for( i = 0 ; i < ai_iNumFields ; i++ )
	{
		m_paRS[ M4LDB_SYS_DICT_FIELDS ]->Current[ i ] ;

		pccIdField = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec() ) ;
		iPosPk = M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec() ) ;
		eFldType = (eFldType_t) M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec() ) ;

		if( iPosPk != 0 )
		{
			sBasePk.append( " AND B." ) ;
			sBasePk.append( pccIdField ) ;
			sBasePk.append( " = D." ) ;
			sBasePk.append( pccIdField ) ;
		}

		if( eFldType == M4LDB_ID_ORGANIZATION )
		{
			// La organización se toma del árbol de sociedades
			// y la operación aunque no hace falta se mete por si alguien la lee
			continue;
		}

		if( iPosPk != 0 )
		{
			sPk.append( " AND DD." ) ;
			sPk.append( pccIdField ) ;
			sPk.append( " = D." ) ;
			sPk.append( pccIdField ) ;
		}

		pccIdType = M4LDB_ID( m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[ M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE ].Value.GetWithOutSec() ) ;
		iIndType = FindIndexById( M4LDB_SYS_DICT_EXTENDED_TYPES, pccIdType ) ;

		bIsLong = M4_FALSE ;
		if( iIndType != M4LDB_NULL_INDEX )
		{
			iM4Type = M4LDB_M4_TYPE( (m4int8_t) M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register.Item[ M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE ].Value.GetWithOutSec() ) ) ;

			if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BLOB || iM4Type == M4CL_M4_TYPE_BINARY_STRING )
			{
				bIsLong = M4_TRUE ;
			}
		}

		if( bIsLong == M4_TRUE )
		{
			sLongValues.append( ", D." ) ;
			sLongValues.append( pccIdField ) ;
		}
		else
		{
			sValues.append( ", D." ) ;
			sValues.append( pccIdField ) ;
		}
	}

	sView.append( "(SELECT O.ID_LEVEL ID_ORG_LEVEL, T.ID_ORGANIZATION" ) ;
	sView.append( sValues ) ;
	sView.append( sLongValues ) ;
	sView.append( " FROM " ) ;
	sView.append( ai_pccRealObject ) ;
	sView.append( " D, " ) ;

	if( ai_iIndRealTbl != 0 )
	{
		// Si no es base y no tiene OPERATION, por lo que se hace el join
		sView.append( ai_pccBase ) ;
		sView.append( " B, " ) ;
	}

	sView.append( "M4VCH_ORG_TREE T, M4RCH_ORGANIZATION O WHERE T.ID_PAR_ORGANIZATIO = D.ID_ORGANIZATION AND O.ID_ORGANIZATION = T.ID_ORGANIZATION" ) ;

	if( ai_iIndRealTbl != 0 )
	{
		// Si no es base y no tiene OPERATION, por lo que se hace el join
		sView.append( sBasePk ) ;
		sView.append( " AND B" ) ;
	}
	else
	{
		sView.append( " AND D" ) ;
	}

	sView.append( ".OPERATION <> 0 AND T.DISTANCE = (SELECT MIN(TD.DISTANCE) FROM " ) ;
	sView.append( ai_pccRealObject ) ;
	sView.append( " DD, M4VCH_ORG_TREE TD WHERE TD.ID_PAR_ORGANIZATIO = DD.ID_ORGANIZATION" ) ;
	sView.append( sPk ) ;
	sView.append( " AND TD.ID_ORGANIZATION = T.ID_ORGANIZATION))" ) ;

	vAux.SetStringRef( (const m4pchar_t) sView.c_str() );
	m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW].Value.SetWithOutSec(vAux, M4_FALSE);


	// Se añade el campo ID_ORG_LEVEL
	iResult = CreateField( "ID_ORG_LEVEL", "ID_ORG_LEVEL", 0, "DEC_6_2_DEFAULT_1", M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL, M4_FALSE, M4_FALSE );

	if( iResult != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	return( M4_SUCCESS );
}


m4return_t ClLConn::LoadDictionaryChannel (m4id_t ai_idLgcTbl, m4int32_t *ai_indLgcTbl, m4bool_t ai_bDumpError)
{

	m4return_t		result;
	m4bool_t		bInheritsData = M4_FALSE;
	m4bool_t		bApplyFilters = M4_TRUE;
	m4uint8_t		iMirrorType = 0;
	eOrgType_t		eOrgType = M4LDB_UNDEF_ORGANIZATION;
	m4uint32_t		iDynamicLanguages = 0;
	m4pcchar_t		pccBase = NULL;
	m4pcchar_t		pccMirrorBase = NULL;
	m4pcchar_t		pccRealObject = NULL;
	m4pcchar_t		pccTransRealObject = NULL;
	m4pcchar_t		pccMirrorRealObject = NULL;
	m4pcchar_t		pccIdType = NULL;
	m4VariantType	vAux;
	ClRegister_Current *pCurrentLogicObjects = NULL;

	ClRegisterIndex	indTrans(0);
	ClRegisterIndex	indRealTbl(0);
	ClRegisterIndex	indOrgField(0);
	ClRegisterIndex	indChildOrgField(0);
	ClRegisterIndex	indMirrorTbl(0);
	ClRegisterIndex	indLgcTbl(0);
	ClRegisterIndex	indField(0);
	ClRegisterIndex	indSecLObj(0);

	
	// Buscamos el idLgcTbl en el canal del diccionario para ver si ya existe.
	indLgcTbl = m_pLdb->FindIndexByIdObject (ai_idLgcTbl);
	
	if (indLgcTbl == M4LDB_NULL_INDEX)
	{
		// No existe, lo cargamos.
		ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexLoadDict);
		
		indLgcTbl = m_pLdb->FindIndexByIdObject (ai_idLgcTbl);
		
		if (indLgcTbl != M4LDB_NULL_INDEX)
		{
			// Esto significa que mientras éste esperaba, el anterior ha cargado la tabla.
			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];

			if (ai_indLgcTbl)
			{
				*ai_indLgcTbl = indLgcTbl;
			}
			return (M4_SUCCESS);
		}

		// Cargamos la tabla en el canal del diccionario.
		vAux.SetStringRef( (const m4pchar_t) ai_idLgcTbl );
		m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT_AUX].Value.SetWithOutSec(vAux, M4_FALSE);
		
		m4int32_t numOldLgcTbl = m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Count ();

		result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS], m_poStatistics);
		
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
		
		m4int32_t numCurLgcTbl = m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Count ();
		
		if (numOldLgcTbl == numCurLgcTbl)
		{
			if( ai_bDumpError == M4_TRUE )
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_LOGIC_OBJECT, ai_idLgcTbl);
			}
			return (M4_ERROR);
		}

		// Si no reordenamos, esto es perfectamente válido.
		indLgcTbl = numCurLgcTbl - 1;
		m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];
		pCurrentLogicObjects = &(m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current);

		eOrgType = (eOrgType_t) M4LDB_NUMBER (pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec ());

		//Enh 0116664:
		//El usuario de bd está marcado para no aplicar filtros de organización
		if( eOrgType == M4LDB_MULTI_ORGANIZATION )
		{
			bApplyFilters = m_pLdb->GetApplyOrganizationFilters() ;

			if (bApplyFilters == M4_FALSE)
			{
				//Si la tabla es multi la ponemos a mono para no aplicar filtros.
				eOrgType = M4LDB_MONO_ORGANIZATION;
				vAux.SetDouble( M4LDB_MONO_ORGANIZATION );
				result = pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.SetWithOutSec(vAux, M4_FALSE);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}
		
		if( eOrgType != M4LDB_MULTI_ORGANIZATION )
		{
			// Una tabla que no sea multi no puede ser de herencia
			vAux.SetDouble( 0 );
			pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.SetWithOutSec(vAux, M4_FALSE);
		}

		bInheritsData = M4LDB_BOOL(pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec());

		if( bInheritsData == M4_TRUE )
		{
			// Una tabla que sea de herencia no puede ser de tramitación
			vAux.SetDouble( 0 );
			pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.SetWithOutSec(vAux, M4_FALSE);
		}

		// Traducciones =======================================================

		result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_TRANS], m_poStatistics);
		
		if (result == M4_ERROR)
		{
			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
			return (M4_ERROR);
		}
		
		m4uint32_t numTrans = m_paRS[M4LDB_SYS_DICT_TRANS]->Count ();

		if (numTrans)
		{
			// Reordenamos el nodo.
			m_paRS[M4LDB_SYS_DICT_TRANS]->Arrange ();
			
			for (indTrans=0; indTrans<numTrans; indTrans++)
			{
				m_paRS[M4LDB_SYS_DICT_TRANS]->Current[indTrans];
				
				result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS], m_poStatistics);
				
				if (result == M4_ERROR) {
					m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
					return (M4_ERROR);
				}
				
				m4int32_t numTrnFld = m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Count ();
				
				if (!numTrnFld)
				{
					m4id_t idField = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_TRANS]->Current.Item[M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD].Value.GetWithOutSec ());
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_TRANS_FIELDS, idField << ai_idLgcTbl);
					m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
					return (M4_ERROR);
				}
			}
		}
		
		
		// Tablas reales ======================================================

		result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS], m_poStatistics);
		
		if (result == M4_ERROR)
		{
			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
			return (M4_ERROR);
		}

		m4uint32_t numRealTbl = m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();
		
		if (!numRealTbl)
		{
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_REAL_OBJECTS, ai_idLgcTbl);
			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
			return (M4_ERROR);
		}	
		

		// Tablas reales espejo de tramitación ================================

		iMirrorType = M4LDB_NUMBER(pCurrentLogicObjects->Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec());

		if( iMirrorType == 1 )
		{
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS], m_poStatistics);
			
			if (result == M4_ERROR)
			{
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
			
			m4uint32_t numMirrorRealTbl = m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Count ();
			
			if (numMirrorRealTbl != numRealTbl)
			{
				m4id_t nLgcTbl = GetnLgcTbl( indLgcTbl ) ;
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_BAD_MIRROR_OBJECT, LOG_CAT_AND( ai_idLgcTbl, nLgcTbl ));
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
		}

		
		// Campos =============================================================

		for (indRealTbl = 0; indRealTbl < numRealTbl; indRealTbl++)
		{
			m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_FIELDS], m_poStatistics);
			
			if (result == M4_ERROR)
			{
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}

			m4uint32_t numFld = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
			
			if (!numFld)
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_LOGIC_FIELDS, ai_idLgcTbl << (m4int32_t) indRealTbl);
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
		}


		// Campos organización ================================================

		if (m_pLdb->GetMultiOrgAccessLevel() > 0)
		{
			if (strcmpi(ai_idLgcTbl, "SCH_ORGANIZATION_LEVEL") == 0)
			{
				m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Register.Add();

				vAux.SetStringRef("ID_ORGANIZATION");
				m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);

				vAux.SetDouble(1);
				m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_POS_PK].Value.SetWithOutSec(vAux, M4_FALSE);
				m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Register.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_INTERNAL_FIELD].Value.SetWithOutSec(vAux, M4_FALSE);

				m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Arrange ();
			}
			else
			{
				result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_ORG_FIELDS], m_poStatistics);
				
				if (result == M4_ERROR)
				{
					m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
					return (M4_ERROR);
				}

				m4uint32_t numOrgFields = m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Count ();
				
				if (numOrgFields) {
					// Reordenamos el nodo.
					m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Arrange ();

					for (indOrgField=0; indOrgField<numOrgFields; indOrgField++)
					{
						m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Current[indOrgField];

						m4bool_t bIsPK = M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_POS_PK].Value.GetWithOutSec() ) ? M4_TRUE : M4_FALSE;

						if (bIsPK == M4_TRUE)
						{
							result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS], m_poStatistics);
							
							if (result == M4_ERROR) {
								m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
								return (M4_ERROR);
							}
							
							m4uint32_t numChildOrgFields = m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS]->Count ();

							if (numChildOrgFields) {
								// Reordenamos el nodo.
								m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS]->Arrange ();

								for (indChildOrgField=0; indChildOrgField<numChildOrgFields; indChildOrgField++)
								{
									m4id_t idChildField = M4LDB_ID( m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS]->Register[indChildOrgField].Item[M4LDB_ITEM_SYS_DICT_CHILD_ORG_FIELDS_ID_CHILD_FIELD].Value.GetWithOutSec() );

									m4int32_t indParentField = FindIndexById( M4LDB_SYS_DICT_ORG_FIELDS, idChildField );

									if( indParentField != M4LDB_NULL_INDEX )
									{
										vAux.SetDouble(1);
										m_paRS[M4LDB_SYS_DICT_ORG_FIELDS]->Register[indParentField].Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_HAS_PARENT].Value.SetWithOutSec(vAux, M4_FALSE);
									}
								}
							}
						}
					}
				}
			}
		}


		// Si hay lenguajes dinámicos y tiene campos traducibles debería tener una tabla física de traducciones
		iDynamicLanguages = m_pLdb->DynamicLanguages();

		if( iDynamicLanguages > 0 && numTrans > 0 )
		{
			pccTransRealObject = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TRANS_REAL_OBJECT].Value.GetWithOutSec ());

			if( pccTransRealObject != NULL && *pccTransRealObject != '\0' )
			{
				result = CreateTranslationMetaData( numRealTbl, numTrans, iDynamicLanguages, pccTransRealObject, iMirrorType );

				if( result == M4_ERROR )
				{
					return( M4_ERROR );
				}
			}
		}


		if( iMirrorType == 1 )
		{
			// Se ordenan las tablas mirror después de rellenar las traducciones
			m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Arrange ();
		}


		// Se hacen los ajustes de campos.
		numRealTbl = m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

		for (indRealTbl = 0; indRealTbl < numRealTbl; indRealTbl++)
		{
			m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			pccRealObject = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec() ) ;
			m4bool_t bIsTranslation = M4LDB_NUMBER( m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() ) ? M4_TRUE : M4_FALSE;

			m4uint32_t numFld = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();

			/* Bug 0116664, 0155228
			El usuario de bd está marcado para no aplicar filtros de organización
			hay que cambiar el tipo de la columna que sea multiorganization a mono.
			Este cambio hay que hacerlo independientemente del tipo de tabla si no hay que aplicar filtros
			Con la marca de bApplyFilters ya es suficiente
			*/
			if (bApplyFilters == M4_FALSE)
			{
				for (indField= 0; indField<numFld; indField++)
				{
					m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indField];

					pccIdType = (m4pcchar_t) m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ();

					if (pccIdType != NULL && strcmpi(pccIdType, "ID_ORG_MULTI") == 0)
					{
						vAux.SetStringRef( "ID_ORG_MONO" );
						result = m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.SetWithOutSec(vAux, M4_FALSE);

						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
					}
				}
			}

			// Si es de tramitación se calculan las tablas reales espejo, se compone la vista y se le añaden los campos adicionales
			if( iMirrorType == 1 )
			{
				// Se busca la tabla real espejo
				indMirrorTbl = FindIndexById( M4LDB_SYS_DICT_MIRROR_OBJECTS, pccRealObject ) ;

				if( indMirrorTbl == M4LDB_NULL_INDEX )
				{
					m4id_t nLgcTbl = GetnLgcTbl( indLgcTbl ) ;
					DUMP_LDBINSP_ERRORF( this, M4_CH_LDB_NO_MIRROR_REAL_OBJECT, pccRealObject << LOG_CAT_AND( ai_idLgcTbl, nLgcTbl ) );
					return( M4_ERROR ) ;
				}

				pccMirrorRealObject = M4LDB_PCSTR( m_paRS[M4LDB_SYS_DICT_MIRROR_OBJECTS]->Register[indMirrorTbl].Item[M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.GetWithOutSec() );

				vAux.SetStringRef( (const m4pchar_t) pccMirrorRealObject );
				m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.SetWithOutSec(vAux, M4_FALSE);

				if( indRealTbl == 0 )
				{
					// El primero es la base
					pccMirrorBase = pccMirrorRealObject ;
				}

				result = CreateTramitationView( indRealTbl, numFld, pccMirrorBase, pccMirrorRealObject, pccRealObject, bIsTranslation );

				if( result == M4_ERROR )
				{
					return( M4_ERROR );
				}
			}
			

			// Si es de herencia de datos se compone la vista y se le añade el campo ID_ORG_LEVEL
			if( bInheritsData == M4_TRUE )
			{
				if( indRealTbl == 0 )
				{
					// El primero es la base
					pccBase = pccRealObject ;
				}

				result = CreateInheritView( indRealTbl, numFld, pccBase, pccRealObject, bIsTranslation );

				if( result == M4_ERROR )
				{
					return( M4_ERROR );
				}
			}
			
			// Reordenamos el nodo.
			m_paRS[M4LDB_SYS_DICT_FIELDS]->Arrange ();
		}
		

		// Seguridad ==========================================================
		
		m4bool_t bHasSecurity = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_SECURITY].Value.GetWithOutSec ());
		
		if (bHasSecurity)
		{
			// Cargamos la seguridad del objeto logico.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ], m_poStatistics);
			
			if (result == M4_ERROR)
			{
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
			
			m4uint32_t numSecLObj = m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Count ();
			
			if (numSecLObj)
			{
				// Reordenamos el nodo.
				m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Arrange ();

				//Bug 0084850
				for (indSecLObj=0; indSecLObj<numSecLObj; indSecLObj++)
				{
					//Recorremos.
					m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current[indSecLObj];
				}
			}
		}


		// Auditoria ==========================================================
		
		m4bool_t bHasAuditory = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_AUDITORY].Value.GetWithOutSec ());
		
		if (bHasAuditory)
		{
			// Cargamos la auditoria del objeto logico.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_AUD_LOBJ], m_poStatistics);
			
			if (result == M4_ERROR) {
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
			
			m4uint32_t numAudLObj = m_paRS[M4LDB_SYS_DICT_AUD_LOBJ]->Count ();
			
			if (numAudLObj) {
				// Reordenamos el nodo.
				m_paRS[M4LDB_SYS_DICT_AUD_LOBJ]->Arrange ();				
			}
		}


		// Herencia ===========================================================

		m4uint32_t numOldIhn = 0;
		m4bool_t bHasInheritance = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE].Value.GetWithOutSec ());
		
		if (bHasInheritance)
		{
			// Cargamos la herencia del objeto logico.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_INHERITANCE], m_poStatistics, 0);
			
			if (result == M4_ERROR)
			{
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}

			numOldIhn = m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Count ();

			if (!numOldIhn)
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_INHERITANCE, ai_idLgcTbl);
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
		}


		m4id_t idClsDate = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_CLS_DATE].Value.GetWithOutSec ());

		if (*idClsDate)
		{
			// Cargamos la fecha de cierre del objeto logico.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_CLS_DATE], m_poStatistics);
			
			if (result == M4_ERROR)
			{
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
		}

		/* Bug 0188940
		La primera vez se reserva espacio suficiente para que no haya problemas de multithread
		*/
		if( numOldLgcTbl == 0 )
		{
			ReserveDictionary();
		}

		// Ahora si es buen momento para añadirlo al mapa.

		/* Bug 0201307
		Hay que pasar el valor del item porque el argumento puede desaparecer
		*/
		m4id_t idLgcTbl = M4LDB_ID( m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec() );
		m_pLdb->InsertObject(idLgcTbl, indLgcTbl);
	}
	else
	{
		m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];
	}

	if (ai_indLgcTbl)
	{
		*ai_indLgcTbl = indLgcTbl;
	}

	return (M4_SUCCESS);
}


m4return_t ClLConn::LoadDictionaryChannelForeigns (void)
{
	m4return_t result;
	ClRegisterIndex indFrn(0);
	ClRegisterIndex indInd(0);

	m4bool_t bCompleteLoad = IsForeignLoaded();
	
	if (!bCompleteLoad)
	{
		// Cargamos las Foreigns.
		ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexLoadDict);

		bCompleteLoad = IsForeignLoaded();

		if (!bCompleteLoad)
		{
			// Cargamos las tablas foraneas.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_FOREIGNS], m_poStatistics);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			m4uint32_t numFrn = m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();
						
			// Cargamos los campos de las tablas foraneas.
			for (indFrn=0; indFrn<numFrn; indFrn++)
			{
				m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFrn];

				eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());

				//En el nodo M4LDB_SYS_DICT_FOREIGNS se cargan dos tipo de relaciones: Las relaciones
				//con su padre, solo habra una (el tipo sera M4LDB_PARENT_RELATION, es 1). Como las relaciones con sus hijos, 
				//pueden haber varias (seran de tipo M4LDB_CHILD_RELATION, es 2).

				//-------------------------------------------------
				//En el nodo de M4LDB_SYS_DICT_FOREIGNS tendremos:
				//Si el tipo de relacion es M4LDB_PARENT_RELATION significa que estamos en un registro donde la 
				//información que nos da el la siguiente:
				//Dado un objeto ID tendremos en Select ID_PARENT_OBJECT Where ID_OBJECT = ID, por lo tanto estaremos 
				//cargando el padre, solo puede haber uno de tipo padre.
				//Si el tipo de relacion es M4LDB_CHILD_RELATION significa que estamos en un registro donde la 
				//información que nos da el la siguiente:	
				//Dado un objeto ID tendremos Select ID_OBJECT Where ID_PARENT_OBJECT = ID, por lo tanto estamos 
				//cargando los hijos de ID.
				//-------------------------------------------------

				//Lo que no tiene sentido es que la relación sea de padre y tengamos un DC!!
				//Pero tendríamos que modificar los m2.
				//No lo haremos de momento.
				////m4bool_t bDeleteOnCascade = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DELETE_CASCADE].Value.GetWithOutSec ());
				////if ( (eRelType == M4LDB_PARENT_RELATION) && (bDeleteOnCascade == M4_TRUE) ){
				////	m4id_t idLgcTbl = NULL;
				////	m4id_t idForLgcTbl = NULL;
				////	m4id_t idRelation = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());
				////	idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_OBJECT].Value.GetWithOutSec ());
				////	idForLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());

				////	M4LDB_PRINT_DEBUG (this, "\n\t Error: ID Relacion: %s , Padre: %s Hija: %s", idRelation, idLgcTbl, idForLgcTbl);
				////	return M4_ERROR;
				////}

				result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS], m_poStatistics, (eRelType == M4LDB_PARENT_RELATION) ? 0 : 1);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				m4int32_t numPar = m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Count ();

				if (!numPar)
				{
					m4id_t idLgcTbl = NULL;
					m4id_t idForLgcTbl = NULL;
					m4id_t idRelation = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());

					if (eRelType == M4LDB_PARENT_RELATION)
					{
						idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_OBJECT].Value.GetWithOutSec ());
						idForLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
					}
					else
					{
						idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
						idForLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_OBJECT].Value.GetWithOutSec ());
					}
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_RELATION_FIELDS, idLgcTbl << idRelation);
					return (M4_ERROR);
				}

			}

			SetForeignLoaded();
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClLConn::LoadDictionaryChannelRSM (void)
{
	m4return_t result;
	ClRegisterIndex indFrn(0);
	ClRegisterIndex indInd(0);

	m4bool_t bCompleteLoad = IsRSMLoaded();
	
	if (!bCompleteLoad)
	{
		// Cargamos la información del RSM.
		ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexLoadDict);

		bCompleteLoad = IsRSMLoaded();

		if (!bCompleteLoad)
		{
			// Cargamos los filtros.
			result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL], m_poStatistics);
			
			if (result == M4_ERROR) {
				m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
				return (M4_ERROR);
			}
			
			m4bool_t bHasSecurityFld = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_HAS_SECURITY_FLD].Value.GetWithOutSec ());
			
			if (bHasSecurityFld)
			{
				// Cargamos los datos de seguridad de los campos.
				result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_SEC_FIELDS], m_poStatistics);
				
				if (result == M4_ERROR)
				{
					m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Destroy ();
					return (M4_ERROR);
				}
				
				m4int32_t numSecField = m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->Count ();
				
				if (numSecField)
				{
					// Reordenamos el nodo.
					m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->Arrange ();
				}
			}

			SetRSMLoaded();
		}
	}

	return (M4_SUCCESS);
}


m4int32_t ClLConn::FindIndexById (m4int32_t ai_idNode, m4id_t ai_idValue)
{
    m4VariantType arg;
    arg.SetStringRef( (m4pchar_t) ai_idValue );

	ClRegister *pReg = &m_paRS[ai_idNode]->Register;

    if( pReg->Begin() == M4_TRUE )
	{
		pReg->Find (&arg);
	}

	return (pReg->Index);
}


m4int32_t ClLConn::FindIndexById (m4int32_t ai_idNode, m4double_t ai_dValue)
{
    m4VariantType arg;
    arg.SetDouble( ai_dValue );

	ClRegister *pReg = &m_paRS[ai_idNode]->Register;

    if( pReg->Begin() == M4_TRUE )
	{
		pReg->Find (&arg);
	}

	return (pReg->Index);
}


m4int32_t ClLConn::FindIndexByIds (m4int32_t ai_idNode, m4id_t ai_idValue, m4double_t ai_dValue)
{
    m4VariantType arg[2];
    arg[0].SetStringRef( (m4pchar_t) ai_idValue );
    arg[1].SetDouble( ai_dValue );

	ClRegister *pReg = &m_paRS[ai_idNode]->Register;

    if( pReg->Begin() == M4_TRUE )
	{
		pReg->Find(arg);
	}

	return (pReg->Index);
}

m4int32_t ClLConn::FindIndexByIds (m4int32_t ai_idNode, m4double_t ai_dValue1, m4double_t ai_dValue2)
{
    m4VariantType arg[2];
    arg[0].SetDouble( ai_dValue1 );
    arg[1].SetDouble( ai_dValue2 );

	ClRegister *pReg = &m_paRS[ai_idNode]->Register;

    if( pReg->Begin() == M4_TRUE )
	{
		pReg->Find(arg);
	}

	return (pReg->Index);
}

m4int32_t ClLConn::FindIndexByIdField (m4id_t ai_idField)
{
	ClRegisterIndex indReg(0);

	// Obtenemos el número de registros que contiene el bloque.
	m4uint32_t numReg = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
	
	for (indReg=0; indReg<numReg; indReg++)
	{
		// Comparamos el valor dado con el valor del item especificado para cada registro.
		m4id_t idField = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indReg].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

		if (strcmpi(idField, ai_idField ) == 0)
		{
			return (indReg);
		}
	}

	return (M4LDB_NULL_INDEX);
}

m4int32_t ClLConn::FindIndexByIdInternalField (m4int32_t ai_idIntFld)
{
	ClRegisterIndex indReg(0);

	// Obtenemos el número de registros que contiene el bloque.
	m4uint32_t numReg = m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
	
	for (indReg=0; indReg<numReg; indReg++)
	{
		// Comparamos el valor dado con el valor del item especificado para cada registro.
		m4int32_t lVal = M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indReg].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

		if (lVal == ai_idIntFld)
		{
			return (indReg);
		}
	}

	return (M4LDB_NULL_INDEX);
}


m4return_t ClLConn::AnalizeRealSqlMode (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet, m4bool_t ai_bCheckAuditory)
{
	m4return_t result = M4_ERROR;
	
	//Por defecto suponemos que no hay auditoría.
	eRealSqlExecutionMode_t eRealSqlExecutionMode = M4LDB_EXEC_REALSQL_NO_AUDIT;

	if ((ai_pRecordSet) 
		&& (ai_pRecordSet->GetpAccess ())
		&& (ai_pRecordSet->GetpAccess ()->GetpChannel()) )
	{
		m4VariantType vValue;
		if ( (ai_pRecordSet->GetpAccess ()->GetpChannel()->ExecuteRealSQLMode.Get (vValue) == M4_SUCCESS)
			&& (vValue.Type == M4CL_CPP_TYPE_NUMBER) 
			&& (vValue.Data.DoubleData >= 0) )
		{
			eRealSqlExecutionMode = (eRealSqlExecutionMode_t) ((m4int32_t) vValue.Data.DoubleData);
		}
	}

	switch (eRealSqlExecutionMode)
	{
	case M4LDB_EXEC_REALSQL_NO_AUDIT:
		result = M4_SUCCESS;
		break;

	//Vamos a auditar la sentencia real.
	case M4LDB_EXEC_REALSQL_AUDIT:

		if (ai_bCheckAuditory == M4_TRUE)
		{
			result = AnalizeAuditoryRealSql (ai_pLoad, ai_pRecordSet);
			//Si hay error analizando auditoría es pq algo raro-malo ha pasado. Pero no mostramos error a este nivel.
		}
		else
		{
			result = M4_SUCCESS;
		}
		break;

	case M4LDB_EXEC_REALSQL_EXECUTION_NOT_ALLOWDED:
		result = M4_ERROR;
		DUMP_LDBINSP_ERROR (this, M4_CH_LDB_REAL_STATEMENT_EXEC_NOT_ALLOWED);
		break;

	default:
		//Mal rollo, tendría que ser un valor de los anteriores.
		//Esto NUNCA debería pasar pq la inicialización es siempre sobre un valor conocido.
		result = M4_ERROR;
		DUMP_LDBINSP_ERROR (this, M4_CH_LDB_REAL_STATEMENT_EXEC_UNKNOWN_MODE);
		break;
	}

	return result;
}


m4return_t ClLConn::AnalizeAuditoryRealSql (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet)
{
	m4return_t result;	

	if (!ai_pLoad)
	{
		return M4_ERROR;
	}

	//La metasentencia tiene información de los from...
	ClMetaSentence *pMtSent = ai_pLoad->GetMetaSentence();

	//Obtenemos la fecha actual en date.
	m4date_t	dNow;
	ClActualDate (dNow);

	//Metemos a fuego esta cadena constante.
	m4pcchar_t idObject = "REAL_STATEMENT";
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\t\tAuditory in Real Statement analyzing.", "");
	#endif

	//-------------------------
	//Inicializamos valores de usuario y role que se guardan en auditoría.
	m4id_t pIdUser = g_aM4ADM;
	m4id_t pIdRole = g_aM4ADM;
	m4id_t pIdRSM =  g_aM4ADM;

	//m_pLogonAdaptor se ha obtenido en la load.
	//Utilizado para la obtención de usuario...
	ClLogonAdaptor * pLogonAdaptor = m_pLogonAdaptor;

	if (!pLogonAdaptor)
	{
		return M4_ERROR;
	}

	//Del logon obtengo el usuario...
	if (pLogonAdaptor->GetSessionChannelPointer ())
	{
		//pIdUser = M4LDB_ID (pLogonAdaptor->GetSessionRecordSet ()->Current.Item[M4_ITEM_USER].Value.GetWithOutSec ());
		//Bug 0071725.
		//En la inicializacion puede ocurrir que exista ChannelPointer
		//pero que no exista el usuario, ... Se quedará con el valor por defecto M4ADM.
		if ( pLogonAdaptor->GetUsername() )
		{
			pIdUser = pLogonAdaptor->GetUsername();
		}
		if ( pLogonAdaptor->GetRole() )
		{
			pIdRole = pLogonAdaptor->GetRole();
		}
		if ( pLogonAdaptor->GetRSM() )
		{
			pIdRSM = pLogonAdaptor->GetRSM();
		}
	}

	//Utilizado en la costruccion del identificador de sentencia.			
	ClCompiledMCR *pMCR = m_pLogMCR;
	if (!pMCR)
	{
		return M4_ERROR;
	}
	//----------------------------

	if (ai_pLoad == NULL)
	{
		return M4_ERROR;
	}
	
	//Obtenemos el texto de la sentencia que se lanza.
	//Hemos hecho publica la funcion que devuelve el cursor.
	string pSQL ("");			

	if (ai_pLoad->GetCurrentCursor() == NULL)
	{
		return M4_ERROR;
	}
	
	ai_pLoad->GetCurrentCursor()->GetStmt(pSQL);

	//Son los argumentos que esperará la funcion M4_LDB_AUDIT_ADD_ITEM
	//que hay en el canal de auditoría.
	//Recordar que hay un ordinal que se autoincrementa (no se pasa).
	//Ademas en ese orden!!!!.
	int nNumParameters = 6;
	m4VariantType *pParams = new m4VariantType[nNumParameters];
	
	pParams[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	pParams[0].Data.PointerChar = (m4pchar_t) pIdRSM;
	pParams[1].Type = M4CL_CPP_TYPE_STRING_VAR;
	pParams[1].Data.PointerChar = (m4pchar_t) pIdRole;
	pParams[2].Type = M4CL_CPP_TYPE_STRING_VAR;
	pParams[2].Data.PointerChar = (m4pchar_t) pIdUser;
	pParams[3].Type = M4CL_CPP_TYPE_STRING_VAR;
	pParams[3].Data.PointerChar = (m4pchar_t) idObject;
	pParams[4].Type = M4CL_CPP_TYPE_DATE;
	pParams[4].Data.DoubleData = dNow;
	pParams[5].Type = M4CL_CPP_TYPE_STRING_VAR;
	pParams[5].Data.PointerChar = (m4pchar_t) pSQL.c_str();

	result = AnalizeAuditorySelectToTable(ai_pRecordSet, pParams, nNumParameters);

	//La auditoria ya crea espacio para los variants, los elimino
	if (pParams != NULL)
	{
		delete [] pParams;
	}

	if (result == M4_ERROR)
	{
		return result;
	}

	//Si todo ha ido bien, 
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (this, "\n\t\tAuditory in Real Statement analyzed.");
	#endif

	return (M4_SUCCESS);
}


m4return_t ClLConn::AnalizeAuditorySelect (ClLoad * ai_pLoad, ClAccessRecordSet *ai_pRecordSet)
{
	//<<< Pedro. Auditoria Select	
	m4return_t result;	
	m4int32_t lAudMask;

	if (!ai_pLoad)
	{
		return M4_ERROR;
	}

	//La metasentencia tiene información de los from...
	ClMetaSentence *pMtSent = ai_pLoad->GetMetaSentence();

	m4int32_t lMask = 1 << (((m4int32_t) pMtSent->GetStmType ()) - 1);

	//Obtenemos la fecha actual en date.
	m4date_t	dNow;
	ClActualDate (dNow);

	// Recordar que la mascara se contruye:
	// (1*MASK_DEL)+(2*MASK_INS)+(4*MASK_UPD)+(8*MASK_SEL)+
	// (16*MASK_DTL_DEL)+(32*MASK_DTL_INS)+(64*MASK_DTL_UPD)+
	// (128*AUDIT_SQL)
	
	//Obtenemos el número de objetos lógicos que hay en la select.
	m4int32_t numStmObj = pMtSent->GetNumObject ();

	//Para cada objeto analizeremos su auditoría.
	for (m4int32_t indStmObj=0; indStmObj<numStmObj; indStmObj++)
	{
		ClMetaObject * pLogicObject = pMtSent->GetObjectByPos (indStmObj);
		m4id_t idObject = pMtSent->GetIdObjectByPos (indStmObj);
		ClRegisterIndex indLgcTbl(0);
		//m4bool_t bFreeTransaction;

		if (pMtSent->GetStmType () == M4LDB_SELECT)
		{
			indLgcTbl = pLogicObject->m_indLogicTable;
		}

		//Si no hay objeto lógico retornamos error.
		if (indLgcTbl == M4LDB_NULL_INDEX)
		{
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_LOGIC_OBJECT, idObject);
			return (M4_ERROR);
		}

		//Analizamos la auditoria para el objeto lógico.
		result = AnalizeAuditoryObject (pMtSent->GetpGlobVars ()->GetIdRSM (), indLgcTbl, lAudMask);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		//Recordar que el tipo de Stm es: Del=1, Ins=2, Upd=3, Sel = 4
		//En este caso, el tipo de clausula es Select, por lo tanto aplicamos M4LDB_SELECT.
		//Al sumar 3 estamos haciendo shift de 4..7 posiciones, es decir
		//iMask = 16 (Del)
		//iMask = 32 (Ins)
		//iMask = 64 (Upd)
		//Ojo, con esto estamos sobreescribiendo el valor que teníamos previamente
		//para SQL.
		//iMask = 128 (Sel)
		//En vez de tomar AUDIT_SQL (bit 8 de la máscara de los M2). Shift 7
		//se va a tomar el campo MASK_SEL (bit 4). Shitf 3. Por lo tanto restamos 1
		//para que tome el bit 4, es desplazar a la izquierda tres veces 1.
		m4int32_t iMask = 1 << (((m4int32_t) M4LDB_SELECT) - 1);

		//El objeto tiene la mascara de auditoría de select, hay que auditar.
		if (lAudMask & iMask)
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n\t\tSelect auditory for logic object = %s analyzing", idObject);
			#endif

			//-------------------------
			//Inicializamos valores de usuario y role que se guardan en auditoría.
			m4id_t pIdUser = g_aM4ADM;
			m4id_t pIdRole = g_aM4ADM;
			m4id_t pIdRSM =  g_aM4ADM;

			//m_pLogonAdaptor se ha obtenido en la load.
			//Utilizado para la obtención de usuario...
			ClLogonAdaptor * pLogonAdaptor = m_pLogonAdaptor;
			if (!pLogonAdaptor)
			{
				return M4_ERROR;
			}

			//Del logon obtengo el usuario...
			if (pLogonAdaptor->GetSessionChannelPointer ())
			{
				//pIdUser = M4LDB_ID (pLogonAdaptor->GetSessionRecordSet ()->Current.Item[M4_ITEM_USER].Value.GetWithOutSec ());
				//Bug 0071725.
				//En la inicializacion puede ocurrir que exista ChannelPointer
				//pero que no exista el usuario, ... Se quedará con el valor por defecto M4ADM.
				if ( pLogonAdaptor->GetUsername() )
				{
					pIdUser = pLogonAdaptor->GetUsername();
				}
				if ( pLogonAdaptor->GetRole() )
				{
					pIdRole = pLogonAdaptor->GetRole();
				}
				if ( pLogonAdaptor->GetRSM() )
				{
					pIdRSM = pLogonAdaptor->GetRSM();
				}
			}

			//Utilizado en la costruccion del identificador de sentencia.			
			ClCompiledMCR *pMCR = m_pLogMCR;
			if (!pMCR)
			{
				return M4_ERROR;
			}

			if (ai_pLoad == NULL)
			{
				return M4_ERROR;
			}
			
			//Obtenemos el texto de la sentencia que se lanza.
			//Hemos hecho publica la funcion que devuelve el cursor.
			string pSQL ("");			

			if (ai_pLoad->GetCurrentCursor() == NULL)
			{
				return M4_ERROR;
			}
			
			ai_pLoad->GetCurrentCursor()->GetStmt(pSQL);

			//Son los argumentos que esperará la funcion M4_LDB_AUDIT_ADD_ITEM
			//que hay en el canal de auditoría.
			//Recordar que hay un ordinal que se autoincrementa (no se pasa).
			//Ademas en ese orden!!!!.
			int nNumParameters = 6;
			m4VariantType *pParams = new m4VariantType[nNumParameters];
			
			//ojo. Para levantar el m4o los tipos son:
			//M4CL_CPP_TYPE_STRING_VAR vs M4CL_CPP_TYPE_STRING
			//M4CL_CPP_TYPE_DATE vs M4CL_CPP_TYPE_TIMESTAMP
			pParams[0].Type = M4CL_CPP_TYPE_STRING_VAR;
			pParams[0].Data.PointerChar = (m4pchar_t) pIdRSM;
			pParams[1].Type = M4CL_CPP_TYPE_STRING_VAR;
			pParams[1].Data.PointerChar = (m4pchar_t) pIdRole;
			pParams[2].Type = M4CL_CPP_TYPE_STRING_VAR;
			pParams[2].Data.PointerChar = (m4pchar_t) pIdUser;
			pParams[3].Type = M4CL_CPP_TYPE_STRING_VAR;
			pParams[3].Data.PointerChar = (m4pchar_t) idObject;
			pParams[4].Type = M4CL_CPP_TYPE_DATE;
			pParams[4].Data.DoubleData = dNow;
			pParams[5].Type = M4CL_CPP_TYPE_STRING_VAR;
			pParams[5].Data.PointerChar = (m4pchar_t) pSQL.c_str();

			result = AnalizeAuditorySelectToTable(ai_pRecordSet, pParams, nNumParameters);

			//La auditoria ya crea espacio para los variants, los elimino
			if (pParams != NULL)
			{
				delete [] pParams;
			}

			if (result == M4_ERROR)
			{
				return result;
			}

			//Si todo ha ido bien, 
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (this, "\n\t\tSelect auditory for logic object = %s analyzed", idObject);
			#endif

		} // if (lAudMask & iMask) {
	}	//Fin del for.

	return (M4_SUCCESS);
}


m4return_t ClLConn::AnalizeAuditorySelectToTable (ClAccessRecordSet *ai_pRecordSet, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength)
{	
	//ClChannelManager * pChannelManager = m_pLogAccess->GetpChannel()->GetpChannelManager();
	if ( ai_pRecordSet != NULL && 
		( ai_pRecordSet->GetpChannel() ) &&
		( ai_pRecordSet->GetpChannel()->GetpChannelManager() )
		)
	{
		ClChannelManager * pChannelManager = ai_pRecordSet->GetpChannel()->GetpChannelManager();
		//El procedimiento es:
		//1.- Añadir argumentos.
		//2.- Ejecutar auditoria. Se hace cuando estemos saliendo de la BDL

		return m_pSelectAuditory->AddAudit(pChannelManager, ai_pvArguments, ai_iLength);
	}
	
	return M4_ERROR;
}

m4return_t ClLConn::AnalizeAuditoryObject (m4id_t ai_idRSM, m4int32_t ai_indLgcTbl, m4int32_t &ao_lAudMask)
{
	ClRegisterIndex indLgcTbl(0);
	ClRegisterIndex indRSM(M4LDB_NULL_INDEX);
	
	ao_lAudMask = M4LDB_SUPERROLE_AUD_MASK;

	// Recordar que la mascara se contruye:
	// (1*MASK_DEL)+(2*MASK_INS)+(4*MASK_UPD)+(8*MASK_SEL)+(16*MASK_DTL_DEL)+(32*MASK_DTL_INS)+(64*MASK_DTL_UPD)+(128*AUDIT_SQL)

  	// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en el registro indicado por el indice.
	m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_indLgcTbl];

	m4bool_t bHasAuditory = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_AUDITORY].Value.GetWithOutSec ());

	if (!bHasAuditory)
	{
		return (M4_SUCCESS);
	}

	if (!*ai_idRSM)
	{
		m4id_t nLgcTbl = GetnLgcTbl (ai_indLgcTbl);
		m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

		// Bug 0300555 Se pasa de error a warning
		DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_AUDITORY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << "<null>");
		return (M4_SUCCESS);
	}

	indRSM = FindIndexById (M4LDB_SYS_DICT_AUD_LOBJ, ai_idRSM);

	if (indRSM == M4LDB_NULL_INDEX)
	{
		m4id_t nLgcTbl = GetnLgcTbl (ai_indLgcTbl);
		m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

		DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_AUDITORY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << ai_idRSM);
		return (M4_SUCCESS);
	}

	// Situamos el current del nodo M4LDB_SYS_DICT_AUD_LOBJ en el registro actual.
	m_paRS[M4LDB_SYS_DICT_AUD_LOBJ]->Current[indRSM];

	ao_lAudMask = M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_AUD_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_AUD_LOBJ_MASK].Value.GetWithOutSec ());

	return (M4_SUCCESS);
}


m4return_t ClLConn::GetAuditTranslationNode( ClChannelManager *ai_poChannelManager, ClNode* &ao_rpoNode )
{

	m4return_t	iResult = M4_ERROR ;
	ClChannel	*poChannel = NULL ;
	ClAccess	*poAccess = NULL ;


	ao_rpoNode = NULL ;
	
	if( m_poAuditTranslationNode == NULL )
	{
		// La primera vez se construte el canal de auditoría
		iResult = ai_poChannelManager->CreateChannel( poChannel ) ;

		if( iResult != M4_SUCCESS || poChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		poChannel->Role_ID.Set( M4CL_SEC_SUPER_USER ) ;
		poChannel->CsType.Set (M4CL_CSTYPE_BOTH) ;

		iResult = poChannel->BuildFromIdNoSec( "SDC_LDB_AUDIT_TRANS" ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		// Se crea el acceso
		poAccess = poChannel->CreateAccess() ;

		if( poAccess == NULL )
		{
			return( M4_ERROR ) ;
		}

		// Se apunta al nodo
		m_poAuditTranslationNode = &( poAccess->Node[ "SDC_LDB_AUDIT_TRANS" ] ) ;

		if( m_poAuditTranslationNode == NULL )
		{
			return( M4_ERROR ) ;
		}
	}
	else
	{
		poChannel = m_poAuditTranslationNode->GetpAccess()->GetpChannel() ;

		if( poChannel->GetpChannelManager() != ai_poChannelManager )
		{
			// El channel manager ha cambiado
			iResult = poChannel->_AttachRecursive( ai_poChannelManager ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	ao_rpoNode = m_poAuditTranslationNode ;
	return( M4_SUCCESS ) ;
}


m4return_t ClLConn::PersistAuditTranslation( void )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	vVariant ;
	ClChannel		*poChannel = NULL ;
	ClItem			*poItem = NULL ;


	if( m_poAuditTranslationNode != NULL )
	{
		poChannel = m_poAuditTranslationNode->GetpAccess()->GetpChannel() ;

		if( poChannel->GetpChannelManager() != NULL )
		{
			if( m_poAuditTranslationNode->RecordSet.Count() > 0 )
			{
				// Ejecución del item si hay datos
				poItem = &( m_poAuditTranslationNode->RecordSet.Item[ "PERSIST" ] ) ;

				iResult = poItem->Call( NULL, 0 ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				iResult = poItem->StackPop( vVariant ) ;

				if( iResult != M4_SUCCESS || vVariant.Type != M4CL_CPP_TYPE_NUMBER || vVariant.Data.DoubleData != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClLConn::ResetAuditTranslation( void )
{

	ClChannel	*poChannel = NULL ;

	
	if( m_poAuditTranslationNode != NULL)
	{
		poChannel = m_poAuditTranslationNode->GetpAccess()->GetpChannel() ;

		if( poChannel->GetpChannelManager() != NULL )
		{
			if( m_poAuditTranslationNode->RecordSet.Count() > 0 )
			{
				// Se borran los datos si hay
				m_poAuditTranslationNode->RecordSet.Destroy() ;
			}

			// Se quita del channel manager
			poChannel->DetachFromChannelManager() ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClLConn::InitAuditory (void)
{
	m4return_t result = M4_SUCCESS;
	m4bool_t bFree = M4_FALSE;

	if (!m_idExecution)
	{
		ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexLoadDict);

		//Bug 0072356. Embebemos codigo.
		//La llamada tiene un problema con las estadísticas, se pierde el puntero
		//y da un GP pq estadisticas están activadas pero el puntero es nulo.
		//Aqui hacemos la llamada a SysLoad sin perder el puntero.
		//Estamos en un mutex por lo tanto garantizamos que el id se incrementa
		//cada vez que alguien invoca a InitAuditory.		
		//---------------------------------------------
		// Limpiamos el recordset.
		m_paRS[M4LDB_SYS_DICT_NEXT_ID_EXECUTION]->Register.DestroyAll ();

		if( m_pLdb->GetContext()->MustAttachConnection( -1 ) == M4_FALSE )
		{
			result = m_pLdb->m_pSysLConn->BeginTransaction(m_paRS[M4LDB_SYS_DICT_NEXT_ID_EXECUTION]);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_NEXT_ID_EXECUTION], m_poStatistics, M4LDB_NULL_INDEX, M4LDB_DEBUG_LEVEL_1);

		if( m_pLdb->GetContext()->MustAttachConnection( -1 ) == M4_FALSE )
		{
			if (result == M4_ERROR)
			{
				result = m_pLdb->m_pSysLConn->EndTransaction(NULL, M4_FALSE, bFree);
				return (M4_ERROR);
			}
		}

		m_idExecution = (M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_NEXT_ID_EXECUTION]->Register[(ClRegisterIndex) 0].Item[M4LDB_ITEM_SYS_DICT_NEXT_ID_EXECUTION_ID_EXECUTION].Value.GetWithOutSec ()) + 1);
		//---------------------------------------------

		m4VariantType idExecution;
		idExecution.Type = M4CL_CPP_TYPE_NUMBER;
		idExecution.Data.DoubleData = m_idExecution - 1;

		// Le damos nuestro LogonAdaptor.
		m_pLdb->m_pSysLConn->m_pLogonAdaptor = m_pLogonAdaptor;

		// Esto es un acceso a escritura en el diccionario por lo que tiene que ser thread-safe.
		m4VariantType vAux( idExecution );
		m_paRS[M4LDB_SYS_DICT_INSERT_PARAMS]->Current.Item[M4LDB_ITEM_SYS_DICT_INSERT_PARAMS_ID_EXECUTION].Value.SetWithOutSec(vAux, M4_FALSE);

		// Esta SysPersist se realiza sobre la conexión del sistema para dejar constancia de la operación.
		result = m_pLdb->m_pSysLConn->SysLoad (m_paRS[M4LDB_SYS_DICT_INSERT_PARAMS], m_poStatistics, M4LDB_NULL_INDEX, M4LDB_DEBUG_LEVEL_1);

		/* Bug 0164085
		En SM la conexión es la misma y se está borrando el logon adaptor
		*/
		if( m_pLdb->m_pSysLConn != this )
		{
			m_pLdb->m_pSysLConn->m_pLogonAdaptor = NULL;
		}

		if( m_pLdb->GetContext()->MustAttachConnection( -1 ) == M4_FALSE )
		{
			if (result == M4_ERROR)
			{
				result = m_pLdb->m_pSysLConn->EndTransaction(NULL, M4_FALSE, bFree);
				return (M4_ERROR);
			}

			result = m_pLdb->m_pSysLConn->EndTransaction(NULL, M4_TRUE, bFree);
		}
	}

	return (result);
}


m4id_t ClLConn::GetnLgcTbl (m4int32_t ai_indLgcTbl)
{
	if (ai_indLgcTbl == M4LDB_NULL_INDEX)
	{
		return (M4LDB_EMPTY);
	}

	ClRegisterIndex indLgcTbl (ai_indLgcTbl);
	m4id_t idLgcTbl = M4LDB_EMPTY;

	m4uint32_t lDefLangSystem = m_pLdb->GetDefaultLanguage ();
	m4double_t dLang = M4CL_LANGUAGE_DEFAULT;
	m_pLogAccess->GetpChannel()->Data_Language.Get (dLang);
	m4uint32_t lEnvLang = (m4uint32_t) dLang;
	
	// Cogemos el ID_OBJECT del diccionario y buscamos la traduccion en el canal de traducciones.
	// Le pasamos el lenguaje actual y el lenguaje por defecto.
	
	idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_indLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

	return m_oNameTrans.GetnObject(idLgcTbl,lEnvLang,lDefLangSystem);
}


m4id_t ClLConn::GetnField (ClStmt *ai_pStmt, m4int32_t ai_indParam)
{
	m4id_t nField = M4LDB_EMPTY;

	ClDinArr *pParam = ai_pStmt->GetpParams ();

	m4int32_t hItem = pParam->GetItemHandle (ai_indParam);

	if (pParam->GetInfoParamType (ai_indParam) == M4LDB_PAR_ITEM && hItem > 0)
	{
		nField = M4LDB_ID (ai_pStmt->GetpGlobVars ()->GetpMCR ()->nItemI (hItem));
		
		if (!*nField)
		{
			nField = M4LDB_ID (ai_pStmt->GetpGlobVars ()->GetpMCR ()->iItemI (hItem));
		}
	}

	// Si ya lo tenemos salimos.
	if (*nField)
	{
		return nField;
	}

	//	Lo pedimos al canal de traducciones.
	//	Nos hace falta el ID_FIELD.
	//		El ID_FIELD	lo sacamos del ClDinArr de Parametros.
	//	Le pasamos el lenguaje actual y el lenguaje por defecto.
	
	m4uint32_t lDefLangSystem = m_pLdb->GetDefaultLanguage ();
	m4double_t dLang = M4CL_LANGUAGE_DEFAULT;
	m_pLogAccess->GetpChannel()->Data_Language.Get (dLang);
	m4uint32_t lEnvLang = (m4uint32_t) dLang;

	m4id_t idFld = M4LDB_ID (pParam->GetInfoIdField (ai_indParam));
	m4pcchar_t idObject = ai_pStmt->GetIdLgcTbl();

	return m_oNameTrans.GetnField(idFld,idObject,lEnvLang,lDefLangSystem);
}


m4return_t ClLConn::LoadDictionaryTrans (m4id_t ai_pidLgcObject, m4int32_t &ai_indLgcObj)
{
	m4int32_t indLgcTbl= m_pLdb->FindIndexTransByIdObject(ai_pidLgcObject);

	if (indLgcTbl== M4LDB_NULL_INDEX)
	{
		// No existe, lo cargamos.
		ClMutBlock oMutexBlock (m_pLdb->GetContext()->m_poMutexLoadDict);
		
		indLgcTbl= m_pLdb->FindIndexTransByIdObject(ai_pidLgcObject);
		
		if (indLgcTbl!= M4LDB_NULL_INDEX)
		{
			// Esto significa que mientras éste esperaba, el anterior ha cargado la tabla.
			ai_indLgcObj = indLgcTbl;
			return (M4_SUCCESS);
		}

		ClAccessRecordSet	*pRSTransLgcObj = m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_LOGIC_OBJECTS);

		m4VariantType vAux( (const m4pchar_t) ai_pidLgcObject );
		pRSTransLgcObj->Register.Item[M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_ID_OBJECT_AUX].Value.SetWithOutSec(vAux, M4_FALSE);

		m4int32_t numOldLgcTbl = pRSTransLgcObj->Count ();
		
		if (m_pLdb->m_pSysLConn->SysLoad (pRSTransLgcObj, m_poStatistics)!=M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		m4int32_t numCurLgcTbl = pRSTransLgcObj->Count ();

		if (numOldLgcTbl == numCurLgcTbl)
		{
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_WRONG_LOGIC_OBJECT, ai_pidLgcObject);
			return (M4_ERROR);
		}

		indLgcTbl = numCurLgcTbl - 1;

		pRSTransLgcObj->Current[indLgcTbl];
		
		// Cargamos sus campos.
		ClAccessRecordSet	*pRSTransFields = m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS);

		if (m_pLdb->m_pSysLConn->SysLoad (pRSTransFields, m_poStatistics)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}

		if( m_pLdb->DynamicLanguages() > 0 )
		{
			// Si hay lenguajes dinámicos se cargan las traducciones dinámicas
			ClAccessRecordSet	*pRSTransLgcObjTrans = m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS);
			ClAccessRecordSet	*pRSTransFieldsTrans = m_oNameTrans.GetRecordSet(M4LDB_SYS_TRANS_FIELDS_TRANS);

			/* Bug 0198984
			No se chequea el error por si la tabla de idiomas no está creada
			*/
			m_pLdb->m_pSysLConn->SysLoad (pRSTransLgcObjTrans, m_poStatistics);
			m_pLdb->m_pSysLConn->SysLoad (pRSTransFieldsTrans, m_poStatistics);
		}

		/* Bug 0188940
		La primera vez se reserva espacio suficiente para que no haya problemas de multithread
		*/
		if( numOldLgcTbl == 0 )
		{
			ReserveDictionaryTrans();
		}

		/* Bug 0201307
		Hay que pasar el valor del item porque el argumento puede desaparecer
		*/
		m4id_t idLgcObject = M4LDB_ID( pRSTransLgcObj->Current.Item[M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec() );
		m_pLdb->InsertTransObject(idLgcObject, indLgcTbl);
	}
	
	ai_indLgcObj = indLgcTbl;

	return M4_SUCCESS;
}

//----------------------------------

//*****
//Pasos:
//1.- Atachar channel al CM.
//2.- Crear access.
//3.- Ejecutar.
//4.- Eliminar access.
//5.- Detachar el canal del CM.

//Si el CM que entro la segunda vez es el mismo que creo el canal, no debo 
//hacer attach, porque ya estoy attachado.
//*****

//La conexion tiene un objeto miembro de la clase ClSelectAuditory que se crea 
//para cada conexión y se destruira con la conexión.

//El M4O de auditoria tiene tabla base la tabla de auditoría y como pk un ordinal
//autoincrementativo.

//El M4O tiene dos metodos, uno añade y otro persiste.

//Crea el canal. Hace push de los argumentos del metodo LN4.
//Cuando la BDL retorna el control a DM ejecuta la persistencia.


//Ojo.
//Al crear el M4O de auditoria (BuildFromId), produce una reentrada en la BDL.

ClSelectAuditory::ClSelectAuditory( ClLConn * ai_pLConn )
{
	m_pLConn = ai_pLConn;
	m_poChannelManager = NULL;
	m_poChannel = NULL;
	m_poNode = NULL;
	m_iAddIndex = m_iExecuteIndex = 0;
	m_bHaveToExecute = M4_FALSE;
}

ClSelectAuditory::~ClSelectAuditory( void )
{
	m_pLConn = NULL;
	DestroyAuditChannel();
	m_poChannelManager = NULL;
}


// execute de alto nivel
// fix bug 74704
m4return_t	ClSelectAuditory::ExecuteAudit( )
{
	m4return_t ret = M4_SUCCESS;
	IExecutor *poExecutor = NULL;
	m4VariantType vValue;

	//Bug 0084804
	if ( (m_pLConn->IsReenter() == M4_FALSE) && (m_pLConn->GetNumTrans() <= 0) )
	{
		// Bug 0280890 Se añade un valor artificial en la pila para no descuadrar la referencia y luego se quita
		if( m_poChannelManager != NULL )
		{
			poExecutor = m_poChannelManager->GetpExecutor() ;

			if( poExecutor != NULL )
			{
				vValue.SetDouble( 0 ) ;
				poExecutor->Stack.Push( vValue ) ;
			}
		}

		ret = _ExecuteAudit();

		if( poExecutor != NULL )
		{
			poExecutor->Stack.Pop( vValue ) ;
		}

		_Reset();
	}

	return ret;

}

// execute de alto nivel
void ClSelectAuditory::_Reset( )
{
	// desconectamos el canal del CM
	if (m_poChannel != NULL)
	{
		m_poChannel->DetachFromChannelManager();
	}

	// hace un reset para evitar problemas de MT
	m_poChannelManager = NULL;
}


//La primera vez al auditar, añadimos los argumentos en la lista, 
//pero el canal no está creado.
//El punto de entrada es el miembro ExecuteAudit.
//En ese momento se llama a ejecutar auditoria, donde crearemos el canal por primera vez.
//Este canal permanecerá sempieternamente vivo.
//Se necesita el ChannelManager (CM) para que  se pueda crear el canal, buildfromid, ..
m4return_t	ClSelectAuditory::_ExecuteAudit()
{
	//Si no me quedan transacciones abiertas es cuando puedo persistir la auditoría.
	//Si hay transacciones pendientes nos esperamos a que se vayan cerrando.
	//Si no hay más transacciones (sera 0) deberé guardar la auditoría (si hubiera algo que guardar).
	//Si el número de transacciones es > 0 siginifica que hay abierta una transacción, por lo tanto
	//no se ejecutara la auditoria. Solo se ejecutara la auditoria si estamos fuera de la transacción.
	if (m_pLConn->GetNumTrans() > 0)
	{
		return M4_SUCCESS;
	}
	
	// checks LC reentering
	if (m_pLConn->IsReenter())
	{
		return M4_SUCCESS;
	}

	if (m_bHaveToExecute == M4_FALSE || m_poChannelManager == NULL) 
	{
		return M4_SUCCESS;
	}

	//Se resetea el flag porque se está ejecutando la auditoria.
	//Es reentrante, con este flag conseguimos controlar la reentrada que produce el buildfromid.
	m_bHaveToExecute = M4_FALSE;

	m4return_t iResult = M4_SUCCESS;
	m4bool_t bDoList = M4_FALSE;

	if (m_poChannel == NULL) 
	{
		//Se crea por primera vez el canal.
		iResult = CreateAuditChannel ();
		if (iResult == M4_ERROR) 
		{
			return M4_ERROR;
		}

		//Se ha creado por primera vez.
		//Por lo tanto los argumentos estan en la lista.
		bDoList = M4_TRUE;
	}

	//Desencadenamos la creación del access. Podré obtener nodo e items del canal
	//de auditoria SDC_LDB_AUDIT.
	//La creación del access supondra que tendremos el nodo y que los items se han creado
	//por lo tanto existen los items add (M4_LDB_AUDIT_ADD_ITEM) y execute M4_LDB_AUDIT_EXECUTE_ITEM.
	iResult = CreateAuditAccess();
	if (iResult == M4_ERROR) 
	{
		return M4_ERROR;
	}

	//Para hacer el Pop de la pila de resultados.
	m4VariantType vValue;

	//Obtenemos el item add que se ha creado cuando creamos el access.
	ClItem *poAddItem = &m_poNode->RecordSet.Item[m_iAddIndex];
	
	//Si este fla es true significa que en la lista tengo los parametros.
	m4bool_t bError = M4_FALSE;
	if (bDoList == M4_TRUE) 
	{
		AuditListIt_t it;
		//Recorro los parámetros y se los voy pasando al item de añadir registro.
		//Se hace el Push
		//Cuando me viene un parametro que es NULL significa que se ha terminado
		//con la primera tanda (pueden haber varios AddRegister antes de persistir)
		//Cada AddRegister necesita las columnas (RSM,...) de cada registro.
		//Cuando encuentro el NULL añado registro.
		//Después saco con Pop el resultado del item que hace el AddRegister.
		//Si ese item tuviese argumentos por referencia tendría que hacer Pops.
		for (it = m_oList.begin(); it != m_oList.end() ; it++)
		{
			m4VariantType *pvTemp = *it;
			if (pvTemp != NULL) 
			{
				iResult = poAddItem->StackPush(*pvTemp);
				if (iResult == M4_ERROR) 
				{
					bError = M4_TRUE;
					break;
				}
				continue;
			}

			iResult = poAddItem->Call();
			if (iResult == M4_ERROR) 
			{
				bError = M4_TRUE;
				break;
			}

			iResult = poAddItem->StackPop( vValue ) ;
			if (iResult == M4_ERROR) 
			{
				bError = M4_TRUE;
				break;
			}
		}
	}

	//Se vaciara el vector de variants y se limpia la lista de argumentos.
	if (bDoList == M4_TRUE) 
	{
		AuditListIt_t it;
		for (it = m_oList.begin(); it != m_oList.end() ; it++)
		{
			m4VariantType *pvTemp = *it;
			if (pvTemp != NULL) {
				delete pvTemp;
			}
		}
		m_oList.clear();
	}

	//Limpiamos el set.
	m_oTableSet.clear();

	if (bError == M4_TRUE)
	{
		return M4_ERROR;
	}

	//Invocamos al método que ejecuta la persistencia.
	//Y como siempre recuperamos su valor de retorno.
	ClItem *poExecuteItem = &m_poNode->RecordSet.Item[m_iExecuteIndex];
	iResult = poExecuteItem->Call();
	if (iResult == M4_ERROR) 
	{
		return M4_ERROR;
	}

	iResult = poExecuteItem->StackPop( vValue ) ;
	if (iResult == M4_ERROR) 
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


//Con esta función añadimos los variants a una lista.
//Posteriormente la lista será usada para que se ejecute auditoria.
//Esta funcion es invocada cuando efectivamente tenemos una tabla que tiene que
//ser auditada, se iran añadiendo los registros que a la salida de la BDL
//se persistiran. Pero la persistencia no se hace con AddAudit.
m4return_t	ClSelectAuditory::AddAudit( ClChannelManager *ai_poChannelManager, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength )
{
	m4return_t iResult = M4_SUCCESS;
	m4uint16_t	i = 0 ;

	// checks: if the table is already includes we just return
	if (ai_iLength < M4_LDB_AUDIT_TABLE_INDEX) 
	{
		return M4_ERROR;
	}

	// registra el channel manager para el execute
	m_poChannelManager = ai_poChannelManager;

	//Tenemos un set de strings donde introducimos el id de la tabla.
	//De esta forma, sólo se insertará un registro en la tabla de auditoria
	//para un mismo id objeto lógico.
	//M4_LDB_AUDIT_TABLE_INDEX es el índice en el vector de parametros del id objeto lógico.
	m4VariantType *pTableIndex = &ai_pvArguments[M4_LDB_AUDIT_TABLE_INDEX];
	if (pTableIndex == NULL || pTableIndex->Type != M4CL_CPP_TYPE_STRING_VAR || pTableIndex->Data.PointerChar == NULL)
	{
		return M4_ERROR;
	}
	else
	{
		//Creamos el string
		string sTableId(pTableIndex->Data.PointerChar);
		//Lo buscamos i devuelve un iterador.
		AuditTableSetIt_t oIt = m_oTableSet.find(sTableId);
		//Si ha llegado al final retornamos pq se encuentra.
		if (oIt != m_oTableSet.end())
		{
			return M4_SUCCESS;
		}
		//Lo introducimos en el set.
		m_oTableSet.insert(sTableId);
	}

	//Si por algun motivo el canal no existe no pasa nada (es la primera vez),
	//nosotros vamos guardando los argumentos que forman cada registro,
	//recordar que es una lista del tipo:
	//arg1,...,argN,NULL, arg1,...argN,NULL
	//Según lo anterior se harían dos AddRegister con sus correspondientes argumentos.

	//Aunque el canal sea nulo la información la guardamos porque al salir de BDL
	//se procedera a la ejecucion (que si no esta creado el canal se creara).
	if ( m_poChannel == NULL ) 
	{
		for( i = 0 ; i < ai_iLength ; i++ )
		{
			m4VariantType *pvTemp = new m4VariantType();
			pvTemp->Set(ai_pvArguments[i],M4_TRUE);
			m_oList.push_back(pvTemp);
		}
		m_oList.push_back(NULL);
	}
	//El canal ya estaba creado, pero puede ser que el acces se haya quedado invalido
	//porque el access fue creado por un CM pero por algún motivo, debido a la reentrada
	//en BDL el CM haya cambiado.
	//La funcion que crea al access de auditoria ya contempla la posibilidad de
	//volver a crear access...
	else
	{
		iResult = CreateAuditAccess();
		if (iResult == M4_ERROR) 
		{
			return M4_ERROR;
		}

		//En este punto se repite la jugada:
		//1.- Se obtiene el item (el que hace el AddRegister) que estará creado porque 
		//aquí tenemos access de auditoria y por lo tanto nodo e items.
		//2.- Se apilan los argumentos del item.
		//3.- Se invoca al item.
		//4.- Se saca de la pila el retorno.

		//Obtenemos el item donde está el código ln4 que añade registro.
		//Push de todos los argumentos.
		ClItem *poAddItem = &m_poNode->RecordSet.Item[m_iAddIndex];
		for( i = 0 ; i < ai_iLength ; i++ )
		{
			iResult = poAddItem->StackPush(ai_pvArguments[i]);
			if (iResult == M4_ERROR) 
			{
				return M4_ERROR;
			}
		}
		
		//Llamamos al item que hace el AddRegister.
		iResult = poAddItem->Call();
		if (iResult == M4_ERROR) 
		{
			return M4_ERROR;
		}

		m4VariantType vValue;
		iResult = poAddItem->StackPop(vValue);
		if (iResult == M4_ERROR) 
		{
			return M4_ERROR;
		}
	}

	//El miembro en el que estamos se ejecuta cuando en verdad se ha encontrado
	//una tabla para auditar y se estan volcando los argumentos en el canal, por lo
	//tanto al salir de BDL se tendra que pesistir el canal.
	//Con este flag indicamos que hay que persistir.
	m_bHaveToExecute = M4_TRUE;

	return( iResult ) ;
}


//Creación unicamente del canal de auditoria.
//El canal se creará en realidad una sola vez.
m4return_t	ClSelectAuditory::CreateAuditChannel( )
{
	m4return_t	iResult = M4_SUCCESS ;

	if (m_poChannelManager == NULL)
	{
		return M4_ERROR;
	}

	//Si el nodo ya está creado no hace falta volver a crear nada.
	if( m_poNode != NULL )
	{
		return( M4_SUCCESS ) ;
	}

	//El nodo no existe, creamos el canal
	ClChannel *poChannel = NULL;
	iResult = m_poChannelManager->CreateChannel( poChannel );
	if ((!poChannel) || (iResult == M4_ERROR))
	{
		return M4_ERROR;
	}

	//Resetea la mascara a Role de superusuario
	//es necesario para la construcción del canal.
	//Sólo el super podría tocar este canal.
	poChannel->Role_ID.Set (M4CL_SEC_SUPER_USER);

	//El canal lo levantamos both para que no haga transaccion en la persistencia.	
	//La persistencia se hará en front o back, dependiendo de dónde esté el canal
	//que hace la load que finalmente necesitará persitir la auditoría.
	poChannel->CsType.Set (M4CL_CSTYPE_BOTH) ;


	// prevents reenter and build the channel
	iResult = poChannel->BuildFromIdNoSec( M4_LDB_AUDIT_CHANNEL_ID ) ;

	if (iResult == M4_ERROR)
	{
		DUMP_LDBINSP_ERRORF (m_pLConn, M4_CH_LDB_AUDIT_BUILD_CHANNEL, M4_LDB_AUDIT_CHANNEL_ID );
		return M4_ERROR;
	}

	// reset internal member
	m_poChannel = poChannel;

	return iResult;
}

//Creacion del access si es necesario.
m4return_t	ClSelectAuditory::CreateAuditAccess()
{
	m4return_t	iResult = M4_SUCCESS ;
	ClAccess	*poAccess = NULL ;

	if (m_poChannelManager == NULL) {
		return M4_ERROR;
	}

	//No puede crearse un access si el canal no ha sido creado previamente.
	if (m_poChannel == NULL) 
	{
		DUMP_LDBINSP_ERROR (m_pLConn, M4_CH_LDB_AUDIT_INTERNAL_NO_CHANNEL );
		return M4_ERROR;
	}

	if (m_poNode != NULL) 
	{
		//Si el nodo ya existe pero el CM es el mismo es porque la reentrada
		//no me ha variado el CM.
		//Por lo tanto no tengo que crear access, está todo creado.
		if (m_poChannel->GetpChannelManager() == m_poChannelManager)
		{
			return M4_SUCCESS;
		}

		//Nos ligamos al nuevo CM.
		iResult = m_poChannel->_AttachRecursive(m_poChannelManager);
		
		// Bg 136566
		return iResult;
	}

	//Estamos ligados al CM correcto, ahora creamos el access
	iResult = m_poChannel->CreateAccess( poAccess ) ;
	if ((iResult == M4_ERROR) || (poAccess == NULL) )
	{
		return M4_ERROR;
	}

	
	//Nos situamos en el nodo de auditoria.
	m_poNode = &( poAccess->Node[ M4_LDB_AUDIT_NODE_ID ] ) ;

	// resets item handles
	m_iAddIndex     = 	m_poNode->RecordSet.Item[M4_LDB_AUDIT_ADD_ITEM].GetIndex();
	m_iExecuteIndex = 	m_poNode->RecordSet.Item[M4_LDB_AUDIT_EXECUTE_ITEM].GetIndex();

	return( M4_SUCCESS ) ;
}


m4return_t	ClSelectAuditory::DestroyAuditAccess( )
{
	m4return_t	iResult = M4_SUCCESS ;
	ClAccess	*poAccess = NULL ;

	//Si no hay canal o no hay nodo no hay nada que destruir.
	if (m_poChannel == NULL || m_poNode == NULL) 
	{
		return M4_SUCCESS;
	}

	//Si hay nodo es porque tenemos access. Lo cogemos.
	poAccess = m_poNode->GetpAccess();
	if (poAccess == NULL)
	{
		return M4_SUCCESS;
	}

	//Destruimos el access.
	iResult = poAccess->Destroy();
	if (iResult == M4_ERROR)
	{
		return M4_ERROR;
	}

	//El canal se desvincula del CM
	iResult = m_poChannel->Detach();
	if (iResult == M4_ERROR)
	{
		return M4_ERROR;
	}

	m_poNode = NULL;
	m_iAddIndex = m_iExecuteIndex = 0;

	return( M4_SUCCESS ) ;
}

//Esta funcion será invocada desde el destructor. Se eliminara el access
//(y por consiguiente los items) y despues elimino el canal.
m4return_t	ClSelectAuditory::DestroyAuditChannel(  )
{
	m4return_t iResult = M4_SUCCESS;

	if( m_poChannel == NULL )
	{
		return( M4_SUCCESS ) ;
	}

	iResult = DestroyAuditAccess();
	if (iResult == M4_ERROR)
	{
		return M4_ERROR;
	}

	iResult = m_poChannel->Destroy();
	m_poChannel = NULL;

	return( iResult ) ;
}
//----------------------------------

