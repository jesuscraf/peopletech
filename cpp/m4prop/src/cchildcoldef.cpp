// CChildColDef.cpp : Implementation of CCChildColDef
#include "stdafx.h"
#include "m4prop.h"
#include "cpropcoldef.h"
#include "cchildcoldef.h"
#include "cpropbagdef.h"

/////////////////////////////////////////////////////////////////////////////
// CCChildColDef

M4PROP_POSITION * CCChildColDef::GetHeadNode()
{
	return (M4PROP_POSITION*)m_pNodeHead;
}

ICPropBagDef * CCChildColDef::GetNext(M4PROP_POSITION **pNode)
{
	ICPropBagDef * pPBD = NULL;
	
	CMinNode * pINode = (CMinNode*)*pNode;
	if (pINode)
	{
		pPBD = (ICPropBagDef*)(pINode)->pData;
		*pNode = (M4PROP_POSITION*)(pINode)->pNext;
	}
	return pPBD;
}
