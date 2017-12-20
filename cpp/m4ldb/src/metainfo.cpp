#include "m4stl.hpp"
#include "metainfo.hpp"
#include "cldinarr.hpp"
#include "clglbvar.hpp"
#include "cllgadap.hpp"


m4pcchar_t ParamType(eParamType_t ai_Param)
{
	switch (ai_Param)
	{
	case M4LDB_PAR_ITEM:
		return "ITEM";

	case M4LDB_PAR_EXTERN:
		return "EXTERN";

	case M4LDB_PAR_SESSION:
		return "SESSION ITEM";

	case M4LDB_PAR_INTERN:
		return "INTERN";

	case M4LDB_PAR_MINUS_INF:
	case M4LDB_PAR_PLUS_INF:
		return "CONST";

	case M4LDB_PAR_UNDEF:
	default:
		return "UNDEF";
	}
}


void ClMetaField::Dump(ofstream &ofs)
{
	ofs << "\t\tField :"<<m_stField<<".Alias: "<<m_indAlias<<".Filter:"<<m_iFilter;
}


void ClMetaRelat::Dump(ofstream &ofs)
{
	ofs << "\t\tRelation."<< endl;
	if (m_pMtField1)
	{
		ofs <<"\t";
		m_pMtField1->Dump(ofs);
		ofs <<endl;

	}
	if (m_pMtField2)
	{
		ofs <<"\t";
		m_pMtField2->Dump(ofs);
	}
}


void ClMetaItemCol::Dump(ofstream &ofs)
{

	ofs << "\t\tColumn :"<< m_hItem;
}

void ClMetaParam::Dump(ofstream &ofs)
{
	ofs << "\t\tParam :"<< ParamType(GetType());
}


void ClMetaObject::Dump(ofstream &ofs)
{
	ofs << "\t\tObject : " << m_stObject<<".Filter: "<<m_iFilter<<".Level: "<<m_iLevel;
}


void ClMetaObject::CleanSecurity( void )
{
	m_sSecurityView.clear();
	_CleanSecurityParams() ;
}

void ClMetaObject::SetSecurityParams( PVcClMtParam ai_pcvParams )
{

	m4uint32_t		iParams = 0 ;
	itVcClMtParam	it ;


	_CleanSecurityParams() ;

	if( ai_pcvParams == NULL )
	{
		return ;
	}

	for( it = ai_pcvParams->begin() ; it != ai_pcvParams->end() ; it++ )
	{
		if( (*it)->IsMetaData() == M4_TRUE )
		{
			iParams++ ;
		}
	}

	if( iParams > 0 )
	{
		m_pvcSecurityParams = new VcClMtParam() ;
		m_pvcSecurityParams->reserve( iParams ) ;

		for( it = ai_pcvParams->begin() ; it != ai_pcvParams->end() ; it++ )
		{
			if( (*it)->IsMetaData() == M4_TRUE )
			{
				m_pvcSecurityParams->push_back( (ClMetaParam*) (*it)->Clone() ) ;
			}
		}
	}
}

m4bool_t ClMetaObject::MetaObjectIntoInheritanceChain( const ClMetaObject * ai_pDestinationObject )
{
	if (this == ai_pDestinationObject)
	{
		//Fin de la recursion.
		return M4_TRUE;
	}

	for (itVcClMtObject it = m_oVcChildObject.begin(); it != m_oVcChildObject.end(); it++)
	{
		ClMetaObject * pLgcObject = *it;
		if (pLgcObject != NULL)
		{
			if (pLgcObject->MetaObjectIntoInheritanceChain( ai_pDestinationObject ) == M4_TRUE)
			{
				return M4_TRUE;
			}
		}		
	}
	return M4_FALSE;
}

void ClMetaObject::_CleanSecurityParams( void )
{
	itVcClMtParam	it ;

	if( m_pvcSecurityParams != NULL )
	{
		for( it = m_pvcSecurityParams->begin() ; it != m_pvcSecurityParams->end() ; it++ )
		{
			delete( *it ) ;
		}
		delete( m_pvcSecurityParams ) ;
		m_pvcSecurityParams = NULL ;
	}
}


m4int8_t ClMetaItem::GetM4Type(ClGlobVars *ai_pGlbVars)
{
	return ai_pGlbVars->GetItemM4Type(m_hItem);
}


m4int32_t ClMetaItem::GetPrecision(ClGlobVars *ai_pGlbVars)
{
	return ai_pGlbVars->GetItemDBPrecision(m_hItem);
}


m4int32_t ClMetaItem::GetScale(ClGlobVars *ai_pGlbVars)
{
	return ai_pGlbVars->GetItemScale(m_hItem);
}


m4int8_t ClMetaItemSess::GetM4Type(ClGlobVars *ai_pGlbVars)
{
	ClCompiledMCR *pMCR = ai_pGlbVars->GetpLConn()->m_pLogonAdaptor->GetSessionRecordSet()->GetpNodeDef()->_GetpCMCR();
	return ai_pGlbVars->GetItemM4Type(m_hItem, pMCR);
}


m4int32_t ClMetaItemSess::GetPrecision(ClGlobVars *ai_pGlbVars)
{
	ClCompiledMCR *pMCR = ai_pGlbVars->GetpLConn()->m_pLogonAdaptor->GetSessionRecordSet()->GetpNodeDef()->_GetpCMCR();
	return pMCR->GetItemDBPrecision(m_hItem);
}


m4int32_t ClMetaItemSess::GetScale(ClGlobVars *ai_pGlbVars)
{
	ClCompiledMCR *pMCR = ai_pGlbVars->GetpLConn()->m_pLogonAdaptor->GetSessionRecordSet()->GetpNodeDef()->_GetpCMCR();
	return pMCR->GetItemScale(m_hItem);
}



