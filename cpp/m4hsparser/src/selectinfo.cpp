//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                selectinfo.cpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Definición de una select
//
//
//==============================================================================

#include "selectinfo.hpp"
#include "m4hsparser_errors.hpp"

#include "basiclog.hpp"
#include "elementfactory.hpp"


SelectInfo::SelectInfo( ClElementFactory* ai_poFactory )
{
	m_bHasDistinct = M4_FALSE;
	m_bIsSubselect = M4_FALSE;
	m_poGroupBy = 0;
	m_poHaving = 0;
	m_poWhere = 0;
	m_poUnion = 0;
	m_poIntersect = 0;

	m_eType = SELECTINFO;

	m_poFactory = ai_poFactory ;
}


SelectInfo::~SelectInfo()
{
}


m4void_t SelectInfo::SetHasDistinct (m4bool_t ai_bHasDistinct)
{
	m_bHasDistinct = ai_bHasDistinct;
}


m4void_t SelectInfo::AddColumn (ElementInfo * ai_poColumnInfo)
{
	m_vpoColumns.push_back (ai_poColumnInfo);
}


m4void_t SelectInfo::AddTable (TableInfo * ai_poTableInfo)
{
	m_vpoTables.push_back ( ai_poTableInfo );
}


m4void_t SelectInfo::SetWhere (ElementInfo * ai_poWhere)
{
	m_poWhere = ai_poWhere;
}


m4void_t SelectInfo::SetIsSubselect (m4bool_t ai_bIsSubselect)
{
	m_bIsSubselect = ai_bIsSubselect;
}


m4void_t SelectInfo::MyDump (ostream & ai_oStream)
{
	if (m_bIsSubselect)
		ai_oStream << " (";

	ai_oStream << " SELECT";
	
	if (m_bHasDistinct)
		ai_oStream << " DISTINCT";
	m4uint_t i;
	for (i = 0; i < m_vpoColumns.size(); i++)
	{
		m_vpoColumns[i]->Dump (ai_oStream);

		if (i < m_vpoColumns.size() - 1)
				ai_oStream << ",";
	}
	
	ai_oStream << " FROM";

	for (i = 0; i < m_vpoTables.size(); i++)
	{
		if ( m_vpoTables[i] != 0)
		{
			m_vpoTables[i]->Dump (ai_oStream);

			if (i < m_vpoTables.size() - 1)
					ai_oStream << ",";
		}
	}

	if (m_poWhere != 0)
	{
		ai_oStream << " WHERE";
		m_poWhere->Dump (ai_oStream);
	}

	if (m_poGroupBy != 0) 
	{
		ai_oStream << " GROUP BY";
		m_poGroupBy->Dump (ai_oStream);
	}

	if (m_poHaving != 0) 
	{
		ai_oStream << " HAVING";
		m_poHaving->Dump (ai_oStream);
	}
		
	if (m_vpoOrderBy.size() > 0) 
	{
		ai_oStream << " ORDER BY";
		for (i = 0; i < m_vpoOrderBy.size(); i++)
		{
				m_vpoOrderBy[i]->Dump (ai_oStream);
				if ( i < m_vpoOrderBy.size() - 1)
					ai_oStream << ",";
		}
	}

	if (m_poUnion != 0)
	{
		ai_oStream << " UNION";
		m_poUnion->Dump (ai_oStream);
	}

	if (m_poIntersect != 0)
	{
		ai_oStream << " INTERSECT";
		m_poIntersect->Dump (ai_oStream);
	}

	if (m_bIsSubselect)
		ai_oStream << " )";

}


m4void_t SelectInfo::SetGroupBy (ElementInfo * ai_poGroupBy)
{
	m_poGroupBy = ai_poGroupBy;
}


m4void_t SelectInfo::AddOrderBy (ElementInfo * ai_poOrderBy)
{
	m_vpoOrderBy.push_back (ai_poOrderBy);
}


m4void_t SelectInfo::SetHaving (ElementInfo * ai_poHaving)
{
	m_poHaving = ai_poHaving;
}


m4void_t SelectInfo::AddUnion (SelectInfo * ai_poUnion)
{
	// Si hay UNION ya, se lo añadimos a la select del union existente ya recursivamente
	if (m_poUnion)
		m_poUnion->AddUnion (ai_poUnion);
	else 
		m_poUnion = ai_poUnion;
}


m4void_t SelectInfo::AddIntersect (SelectInfo * ai_poIntersect)
{
	// Si hay INTERSECT ya, se lo añadimos a la select del intersect existente ya recursivamente
	if (m_poIntersect)
		m_poIntersect->AddIntersect (ai_poIntersect);
	else
		m_poIntersect = ai_poIntersect;
}


m4return_t SelectInfo::Merge (SelectInfo * ai_poParent)
{
	// Al hacer el Merge este objeto se queda con la propiedad
	// de la memoria de todos los objetos de la select padre, 
	// va vaciando los elementos del padre y quedándoselos él 
	// con los punteros a ellos.

	// Renombramos todos los ALIAS de las tablas para evitar coincidencias
	// unos en un sentido y otros en otro. 
	// Renombramos internamente, luego volveremos a renombrar externamente.
	ai_poParent->RenameAllAlias(M4_TRUE, M4_FALSE);
	RenameAllAlias(M4_TRUE, M4_TRUE);

	/* Bug 0105403
	Si el padre tiene DISTINCT en la mezcla también debe estar
	Ahora el DISTINCT no se sobreescribe con el de la hija
	*/
	MergeDistinct (ai_poParent);

	// Mezclamos la parte FROM
	if ( MergeFrom (ai_poParent) == M4_ERROR )
		return M4_ERROR;
		
	// Mezclamos los ORDER BY
	MergeOrderBy (ai_poParent);

	// Fusionamos los GROUP BY
	if ( MergeGroupBy (ai_poParent) == M4_ERROR )
		return M4_ERROR;

	// Mezclamos la parte SELECT
	MergeSelect (ai_poParent);

	// Mezclamos la parte HAVING utilizando un AND
	MergeHaving (ai_poParent);

	// Mezclamos la parte WHERE utilizando un AND
	MergeWhere (ai_poParent);

	// Mezclamos los union e intersect
	MergeUnionIntersect (ai_poParent);

	// Renombramos alias de nuevo para uso externo en la select resultante
	RenameAllAlias(M4_FALSE, M4_FALSE);

	return M4_SUCCESS;
}


m4void_t SelectInfo::MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{

	// Cambiamos los ALIAS de lo que afecta la select,
	// es decir, de todo menos del FROM.
	// Tampoco se cambia el de los elemntos que cuelga de UNION o INTERSECT

	// Cambiamos ALIAS en parte SELECT
	m4int_t j;
	for (j = m_vpoColumns.size()-1; j >= 0; j--)
			m_vpoColumns[j]->ChangeAlias ( ai_pccOldAlias, ai_pccNewAlias);

	// Cambiamos ALIAS en parte WHERE
	if ( m_poWhere != 0)
			m_poWhere->ChangeAlias (ai_pccOldAlias, ai_pccNewAlias);

	// Cambiamos ALIAS en parte ORDER BY	
	for (j = m_vpoOrderBy.size()-1; j >= 0; j--)
			m_vpoOrderBy[j]->ChangeAlias (ai_pccOldAlias, ai_pccNewAlias);

	// Cambiamos ALIAS en parte GROUP BY
	if (m_poGroupBy != 0)
			m_poGroupBy->ChangeAlias (ai_pccOldAlias, ai_pccNewAlias);

	// Cambiamos ALIAS en parte HAVING
	if (m_poHaving != 0)
			m_poHaving->ChangeAlias (ai_pccOldAlias, ai_pccNewAlias);

	// Cambiamos ALIAS en la parte FROM
	for (m4int_t k = m_vpoTables.size()-1; k >= 0; k--)
			m_vpoTables[k]->ChangeAlias ( ai_pccOldAlias, ai_pccNewAlias );
}


m4void_t SelectInfo::RenameAllAlias (m4bool_t ai_bInternal, m4bool_t ai_bReverse)
{
	// Se distinguen alias para manejo interno (evitan conflictos)
	// y uso externo (se ven en la select resultado)
	// Ambos se pueden generar en sentido ascendente o descendente
	
	// Alias internos generados del tipo 00, 11, 22 ... o 77, 88, 99 ...
	// Alias externos (alfabéticos) generados del tipo AA, BB, CC ... o XX, YY, ZZ ...

	// Generamos un alias temporal
	m4char_t pcNewAlias[3] = "  ";
	
	if (ai_bInternal)
	{
		if (ai_bReverse)
		{
			pcNewAlias[0] = '9';
			pcNewAlias[1] = 'Z';
		}
		else
		{
			pcNewAlias[0] = '0';
			pcNewAlias[1] = 'A';
		}
	}
	else
	{
		if (ai_bReverse)
		{
			pcNewAlias[0] = 'Z';
			pcNewAlias[1] = 'Z';
		}
		else
		{
			pcNewAlias[0] = 'A';
			pcNewAlias[1] = 'A';
		}
	}

	// Renombramos los ALIAS de todas las tablas
	for (m4int_t i = m_vpoTables.size()-1; i >= 0; i--)
	{
		m4pchar_t pcOldAlias = strdup(m_vpoTables[i]->GetAlias());

		ChangeAlias( pcOldAlias, pcNewAlias );

		free( pcOldAlias );

		if (ai_bReverse)
		{
			if (i % 26 == 0)
			{
				pcNewAlias[0]--;
				pcNewAlias[1] = 'Z';
			}
			else
				pcNewAlias[1]--;
		}
		else
		{
			if (i % 26 == 0)
			{
				pcNewAlias[0]++;
				pcNewAlias[1] = 'A';
			}
			else
				pcNewAlias[1]++; 
		}
	}
}


m4return_t SelectInfo::AddTotalization (m4pcchar_t ai_pccTotalizator)
{

	// Añadimos el totalizador que nos pasen a todos los elementos de la parte SELECT
	// que están a la derecha del igual
	for (m4int_t i = m_vpoColumns.size()-1; i >= 0; i--)
	{
		TextInfo * poTotalizator = m_poFactory->CreateTextInfo (ai_pccTotalizator);
		TextInfo * poLeft = m_poFactory->CreateTextInfo ("(");
		TextInfo * poRight = m_poFactory->CreateTextInfo (")");

		ElementInfo * poRealElement = m_vpoColumns[i]->GetRight();
		if (poRealElement == 0)
		{
			// Error no es una estructura del tipo @item=xxxxxx
			// No hay elemento en la derecha
			BL_ERRORF (M4HSPARSER_E_ITEMSINTAX, ""); 
			return M4_ERROR;
		}

		poRealElement->PushLeft (poLeft);
		poRealElement->PushRight (poRight);
		poTotalizator->AddRight (poRealElement);

		m_vpoColumns[i]->AddRight (poTotalizator);
	}

	return M4_SUCCESS;
}


m4void_t SelectInfo::AddJoin (JoinInfo * ai_poJoin)
{
	m_vpoJoins.push_back (ai_poJoin);
}


m4return_t SelectInfo::MergeFrom (SelectInfo * ai_poParent)
{

	// Reducir la tabla base
	if ( ReduceBaseTable(ai_poParent) == M4_ERROR)
		return M4_ERROR;
	
	// Reducir a nivel de Joins 
	if ( ReduceJoins (ai_poParent) == M4_ERROR)
		return M4_ERROR;

	// Reducir a nivel de Tablas
	ReduceTables (ai_poParent);

	// Añadimos las tablas que quedan del padre a la hija y se las quitamos al padre
	m_vpoTables.insert (m_vpoTables.begin(), 
						ai_poParent->m_vpoTables.begin(),
						ai_poParent->m_vpoTables.end() );

	ai_poParent->m_vpoTables.clear();

	return M4_SUCCESS;
}


TableInfo * SelectInfo::FindBaseTable ()
{
	for (m4uint_t i = 0; i < m_vpoTables.size(); i++)
		if (m_vpoTables[i]->IsBase())
			return m_vpoTables[i];
	
	return 0;
}


m4return_t SelectInfo::ReduceBaseTable (SelectInfo * ai_poParent)
{
	TableInfo * poChildBase;
	TableInfo * poParentBase;

	poChildBase = FindBaseTable ();
	if (poChildBase == 0)
	{
		//Error, no tiene tabla base la hija
		BL_ERRORF (M4HSPARSER_E_NOBASE, "");
		return M4_ERROR;
	}

	poParentBase = ai_poParent->FindBaseTable ();
	if (poParentBase == 0)
	{
		//Error, no tiene tabla base la hija
		BL_ERRORF (M4HSPARSER_E_NOBASE, "");
		return M4_ERROR;
	}

	// Cambiamos el nombre de la tabla base del padre 
	// No afecta a demás elementos de la select ya que el alias no se modifica
	poParentBase->SetTable (poChildBase->GetTable());

	return M4_SUCCESS;
}


m4void_t SelectInfo::ReduceTables (SelectInfo * ai_poParent)
{
	// Reducir individualmente las tablas que son factibles de reducción
	
	// Con itParent nos recorremos la sentencia padre eliminando lo que podamos reducir
	vector <TableInfo*>::iterator itParent;

	// Por cada tabla de la sentencia hija intentamos cargarnos todas las posibles del padre
	// Para ello ambas deben estar marcadas como reducibles
	for (m4uint_t i = 0; i < m_vpoTables.size(); i++) 
	
		if ( m_vpoTables[i]->IsReductible() || m_vpoTables[i]->IsBase())
		{
			itParent = ai_poParent->m_vpoTables.begin();
			while (itParent != ai_poParent->m_vpoTables.end())
			{
				// Si encontramos una tabla igual nos la cargamos
				if ( (((*itParent)->IsReductible()) || ((*itParent)->IsBase()) ) && 
					 (!strcmp ((*itParent)->GetTable(), m_vpoTables[i]->GetTable())) )
				{
					ai_poParent->ChangeAlias ((*itParent)->GetAlias(), m_vpoTables[i]->GetAlias());
					ai_poParent->m_vpoTables.erase (itParent);
				}
				else
					itParent++;
			}
		}

}


m4return_t SelectInfo::ReduceJoins (SelectInfo * ai_poParent)
{
	// Con itParent nos recorremos la sentencia padre eliminando lo que podamos reducir
	vector <JoinInfo*>::iterator itParent;

	// Resolvemos los joins de ambas sentencias para comenzar la reducción
	if ( ResolveJoinTables () == M4_ERROR )
		return M4_ERROR;
	
	if ( ai_poParent->ResolveJoinTables () == M4_ERROR)
		return M4_ERROR;

	//Vector donde vamos guardano los alias que hay que borrar
	vector <m4pcchar_t> vpccAliasToDelete;

	for (m4uint_t i = 0; i < m_vpoJoins.size(); i++)
	{
		itParent = ai_poParent->m_vpoJoins.begin();
		while (itParent != ai_poParent->m_vpoJoins.end())
		{
		  	  if ( **itParent == *(m_vpoJoins[i])  )
			  {
				  // Si son iguales los joins el join de la padre tendrá la información de 
				  // matching del join.
				  // Nos cargamos las tablas del padre y hacemos la sustitución de alias
				  // usando la información del matching
				  
				  M4HS_JOIN_MATCHING_T oMatching = (*itParent)->GetMatching();
				  
				  M4HS_JOIN_MATCHING_T::iterator itMatching;
				  for (itMatching = oMatching.begin(); itMatching != oMatching.end(); itMatching++)
				  {
					  // Cambiamos los alias del padre
					  m4pchar_t pcOldAlias = strdup( (*itMatching).first->GetAlias() );

					  ai_poParent->ChangeAlias ( pcOldAlias, (*itMatching).second->GetAlias() );

					  free( pcOldAlias );

					  // Añadimos el alias a borrar al vector de alias a borrar
					  // No lo podemos borrar aquí por si acaso la tabla se usa luego en otro join
					  vpccAliasToDelete.push_back ((*itMatching).second->GetAlias());
				  }

				  oMatching.clear();
				  ai_poParent->m_vpoJoins.erase (itParent);
			  }
			  else
				  itParent++;
		}
	}

	// Nos cargamos las tablas del padre
	ai_poParent->DeleteTablesByAlias ( vpccAliasToDelete );

	// Limpiamos el vector
	vpccAliasToDelete.clear();

	return M4_SUCCESS;
}


m4return_t SelectInfo::ResolveJoinTables ()
{
	// Prepara todos los joins para la reducción
	for (m4uint_t i = 0; i < m_vpoJoins.size(); i++)
		if ( m_vpoJoins[i]->ResolveTables (m_vpoTables) == M4_ERROR )
			return M4_ERROR;
	
	return M4_SUCCESS;
}


m4void_t SelectInfo::DeleteTablesByAlias (vector <m4pcchar_t> & ai_vpccAliasToDelete)
{
	// Eliminar la tabla cuyo alias se indica
	vector <TableInfo*>::iterator it;

	for ( m4uint_t i = 0; i < ai_vpccAliasToDelete.size(); i++)
	{
		it = m_vpoTables.begin();
		while (it != m_vpoTables.end())
		{
			// Si encontramos una tabla con ese alias nos la cargamos
			if (!strcmp ((*it)->GetAlias(), ai_vpccAliasToDelete[i]))
			{
				m_vpoTables.erase (it);
			}
			else
				it++;
		}
	}

}


m4void_t SelectInfo::MergeOrderBy (SelectInfo * ai_poParent)
{
	// Mezclamos los ORDER BY
	m_vpoOrderBy.insert (m_vpoOrderBy.begin(), 
					ai_poParent->m_vpoOrderBy.begin(),
					ai_poParent->m_vpoOrderBy.end() );

	ai_poParent->m_vpoOrderBy.clear();
}


m4return_t SelectInfo::MergeGroupBy (SelectInfo * ai_poParent)
{
	// Fusionamos los GROUP BY
	// Si la hija añade un GROUP BY a la padre que no tenía 
	// totalizamos la parte SELECT de la padre
	if (m_poGroupBy == 0)
			SetGroupBy (ai_poParent->m_poGroupBy);
	else if (ai_poParent->m_poGroupBy != 0)
	{
		TextInfo * poTextInfo = m_poFactory->CreateTextInfo (",");
		poTextInfo->AddLeft (ai_poParent->m_poGroupBy);
		poTextInfo->AddRight (m_poGroupBy);
		SetGroupBy (poTextInfo);
	}
	else 
	{
		// Totalizamos con un MAX
		if ( ai_poParent->AddTotalization("#MAX") == M4_ERROR)
			return M4_ERROR;
	}

	ai_poParent->SetGroupBy (0);

	return M4_SUCCESS;
}


m4void_t SelectInfo::MergeWhere (SelectInfo * ai_poParent)
{
	// Mezclamos la parte WHERE utilizando un AND
	if (m_poWhere == 0)
			SetWhere (ai_poParent->m_poWhere);
	else if (ai_poParent->m_poWhere != 0)
	{
		TextInfo * poTextInfo = m_poFactory->CreateTextInfo ("AND");
		poTextInfo->AddLeft (ai_poParent->m_poWhere);
		poTextInfo->AddRight (m_poWhere);
		SetWhere (poTextInfo);
	}

	ai_poParent->SetWhere (0);
}


m4void_t SelectInfo::MergeHaving (SelectInfo * ai_poParent)
{
	// Mezclamos la parte HAVING utilizando un AND
	if (m_poHaving == 0)
			SetHaving (ai_poParent->m_poHaving);
	else if (ai_poParent->m_poHaving != 0)
	{
		TextInfo * poTextInfo = m_poFactory->CreateTextInfo ("AND");
		poTextInfo->AddLeft (ai_poParent->m_poHaving);
		poTextInfo->AddRight (m_poHaving);
		SetHaving (poTextInfo);
	}

	ai_poParent->SetHaving(0);
}


m4void_t SelectInfo::MergeUnionIntersect (SelectInfo * ai_poParent)
{
	// Añadimos los union e intersect del padre a la hija si los tiene
	if (ai_poParent->m_poUnion)
		AddUnion (ai_poParent->m_poUnion);

	ai_poParent->m_poUnion = 0;

	if (ai_poParent->m_poIntersect)
		AddIntersect (ai_poParent->m_poIntersect);

	ai_poParent->m_poIntersect = 0;
}


m4void_t SelectInfo::MergeSelect (SelectInfo * ai_poParent)
{
	// Mezclamos la parte SELECT
	m_vpoColumns.insert (m_vpoColumns.begin(), 
						ai_poParent->m_vpoColumns.begin(),
						ai_poParent->m_vpoColumns.end() );

	ai_poParent->m_vpoColumns.clear();
}


m4void_t SelectInfo::MergeDistinct (SelectInfo * ai_poParent)
{
	/* Bug 0105403
	Si el padre tiene DISTINCT en la mezcla también debe estar
	*/
	if( ai_poParent->m_bHasDistinct == M4_TRUE )
	{
		m_bHasDistinct = M4_TRUE ;
	}
}





