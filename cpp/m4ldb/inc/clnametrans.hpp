//==============================================================================
//
//  (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clnametrans.hpp
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
//    This module obtains the translated name in the required language.
//
//
//==============================================================================


#ifndef _CLNAMETRANS_HPP_
#define _CLNAMETRANS_HPP_

#include "defmain.hpp"	

class ClAccess;
class ClLConn;
class ClAccessRecordSet;

class ClNameTranslator
{
public:
	ClNameTranslator(void);

	~ClNameTranslator(void)
	{
		End();
	}

	m4return_t	Init(ClAccess	*ai_pAccess,ClLConn *ai_pLConn);

	m4return_t	End(void)
	{
		m_pAcsTrans = 0;
		m_pLConn=0;
		return M4_SUCCESS;
	}

	ClAccess *GetAccess(void)
	{
		return m_pAcsTrans;
	}

	ClAccessRecordSet	*GetRecordSet(m4uint8_t	ai_NodeIndex)
	{		
		return m_paRS[ai_NodeIndex];
	}

	m4pcchar_t GetnObject(m4pcchar_t ai_pidObject, m4int32_t ai_Language, m4int32_t ai_DefLanguage);
	m4pcchar_t GetnField(m4pcchar_t ai_pidField, m4pcchar_t ai_pidObject, m4int32_t ai_Language, m4int32_t ai_DefLanguage);

protected:
	m4bool_t	_MoveToIdObject(m4pcchar_t ai_pidObject);
	m4bool_t	_MoveToIdField(m4pcchar_t ai_pidField);
	m4pcchar_t  _GetnValue(m4uint8_t ai_NodeIndex, m4int32_t ai_ItemIndex);

	ClAccess	*m_pAcsTrans;
	ClLConn		*m_pLConn;
	ClAccessRecordSet	* m_paRS[M4LDB_SYS_TRANS_NODE_NUMBER];
private:
};


#endif

