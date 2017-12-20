
#include "clnametrans.hpp"
#include "cllconn.hpp"
#include "access.hpp"

ClNameTranslator::ClNameTranslator(void)
{
	m_pAcsTrans=0;
	m_pLConn=0;
	for (m4int8_t i = 0;i<M4LDB_SYS_TRANS_NODE_NUMBER;++i)
	{
		m_paRS[i]=0;
	}
}

m4return_t ClNameTranslator::Init(ClAccess	*ai_pAccess,ClLConn *ai_pLConn)
{
	if (ai_pAccess == 0)
	{
		return M4_ERROR;
	}

	m_pAcsTrans = ai_pAccess;
	m_pLConn = ai_pLConn;
	for (m4int8_t i = 0;i<M4LDB_SYS_TRANS_NODE_NUMBER;++i)
	{
		m_paRS[i]=&(m_pAcsTrans->Node[(ClNodeIndex)i].RecordSet);
	}

	return M4_SUCCESS;
}

m4pcchar_t ClNameTranslator::GetnObject(m4pcchar_t ai_pidObject, m4int32_t ai_Language, m4int32_t ai_DefLanguage)
{
	if (_MoveToIdObject(ai_pidObject) == M4_FALSE)
	{
		// ¿Que ha pasado?. Está en el diccionario, pero no lo hemos podido cargar. Retorno el ID_OBJECT como nObject.
		return ai_pidObject;
	}

	m4id_t pnObject = NULL;

	// Primero se busca como dinámico
	ClRegister_Current *pReg = &(m_paRS[M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS]->Current);

	if( pReg->Begin() == M4_TRUE )
	{
		m4VariantType vLanguage( ai_Language );
		pReg->Find( &vLanguage );

		if( pReg->Index != M4DM_EOF_INDEX )
		{
			pnObject = _GetnValue(M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS, M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_TRANS_ID_TRANS_OBJ);

			if (pnObject)
			{
				return pnObject;
			}
		}
	}

	// Luego como estático en el lenguaje que se pide
	if( ai_Language > 1 && ai_Language < M4CL_LANGUAGE_NUMBER )
	{
		pnObject = _GetnValue(M4LDB_SYS_TRANS_LOGIC_OBJECTS, M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_FIRST_LANGUAGE + ai_Language - 2);

		if (pnObject)
		{
			return pnObject;
		}
	}

	// Luego como estático en el lenguaje por defecto
	if( ai_DefLanguage != ai_Language && ai_DefLanguage > 1 && ai_DefLanguage < M4CL_LANGUAGE_NUMBER )
	{
		pnObject = _GetnValue(M4LDB_SYS_TRANS_LOGIC_OBJECTS, M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_FIRST_LANGUAGE + ai_DefLanguage - 2);

		if (pnObject)
		{
			return pnObject;
		}
	}

	// Si no hay traducción se devuelve el identificador
	return ai_pidObject;
}

m4pcchar_t ClNameTranslator::GetnField(m4pcchar_t ai_pidField, m4pcchar_t ai_idObject, m4int32_t ai_Language, m4int32_t ai_DefLanguage)
{
	if (_MoveToIdObject(ai_idObject) == M4_FALSE)
	{
		return ai_pidField;
	}

	m4id_t pnField = NULL;

	// Primero se busca como dinámico
	ClRegister_Current *pReg = &(m_paRS[M4LDB_SYS_TRANS_FIELDS_TRANS]->Current);

	if( pReg->Begin() == M4_TRUE )
	{
		m4VariantType avValues[ 2 ];
		avValues[ 0 ].SetStringRef( (m4pchar_t) ai_pidField );
		avValues[ 1 ].SetDouble( ai_Language );
		pReg->Find( avValues );

		if( pReg->Index != M4DM_EOF_INDEX )
		{
			pnField = _GetnValue(M4LDB_SYS_TRANS_FIELDS_TRANS, M4LDB_ITEM_SYS_TRANS_FIELDS_TRANS_ID_TRANS_FLD);

			if (pnField)
			{
				return pnField;
			}
		}
	}

	if (_MoveToIdField(ai_pidField) == M4_FALSE)
	{
		// ¿Que ha pasado?. Está en el diccionario, pero no lo hemos podido cargar. Retorno el ID_FIELD como nObject.
		return ai_pidField;
	}

	// Luego como estático en el lenguaje que se pide
	if( ai_Language > 1 && ai_Language < M4CL_LANGUAGE_NUMBER )
	{
		pnField = _GetnValue(M4LDB_SYS_TRANS_FIELDS, M4LDB_ITEM_SYS_TRANS_FIELDS_FIRST_LANGUAGE + ai_Language - 2);

		if (pnField)
		{
			return pnField;
		}
	}

	// Luego como estático en el lenguaje por defecto
	if( ai_DefLanguage != ai_Language && ai_DefLanguage > 1 && ai_DefLanguage < M4CL_LANGUAGE_NUMBER )
	{
		pnField= _GetnValue(M4LDB_SYS_TRANS_FIELDS, M4LDB_ITEM_SYS_TRANS_FIELDS_FIRST_LANGUAGE + ai_DefLanguage - 2);

		if (pnField)
		{
			return pnField;
		}
	}

	// Si no hay traducción se devuelve el identificador
	return ai_pidField;
}

m4bool_t ClNameTranslator::_MoveToIdObject(m4pcchar_t ai_pidObject)
{
	m4int32_t Index=M4LDB_NULL_INDEX;

	if (m_pLConn->LoadDictionaryTrans(ai_pidObject,Index) != M4_SUCCESS)
	{
		// El error lo saca la SysLoad.
		return M4_FALSE;
	}
	return m_paRS[M4LDB_SYS_TRANS_LOGIC_OBJECTS]->Current.MoveTo(Index);
}

m4bool_t ClNameTranslator::_MoveToIdField(m4pcchar_t ai_pidField)
{
	// Suponemos que estamos colocados en el objeto correcto.
    m4VariantType arg;
    arg.SetStringRef( (m4pchar_t) ai_pidField );

	ClRegister_Current *pReg = &(m_paRS[M4LDB_SYS_TRANS_FIELDS]->Current);

    if( pReg->Begin() == M4_TRUE )
	{
		pReg->Find (&arg);
	}

	return (pReg->Index != M4DM_EOF_INDEX);
}

m4pcchar_t ClNameTranslator::_GetnValue(m4uint8_t  ai_NodeIndex,m4int32_t ai_ItemIndex)
{
	return (m4pcchar_t) m_paRS[ai_NodeIndex]->Current.Item[(ClItemIndex)ai_ItemIndex].Value.Get();
}


