//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cltblmem.inl
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


ClTblMem::ClTblMem (void)
{ 
	m_lNumCol = 0;

	m_lMaxOffset = 0;

	m_lMaxData = 0;

	m_paIdent = NULL;

	m_paType = NULL;

	m_paIsPK = NULL;

	m_plFirstOffset = NULL;

	m_plCurrentOffset = NULL;

	m_lNextCol = 0;

	m_pcFirstData = NULL;

	m_pcCurrentData = NULL;

	m_eState = M4LDB_CLOSE;

	m_pLdb = 0;
}


ClTblMem::~ClTblMem (void)
{
	Close ();
}


m4return_t ClTblMem::Clear (void)
{
	memset (m_pcFirstData, '\0', m_lMaxData * sizeof (m4char_t));

	return (M4_SUCCESS);
}


m4return_t ClTblMem::SetM4Type (m4int32_t ai_lCol, m4int8_t ai_cM4Type)
{
	m_paType[ai_lCol] = ai_cM4Type;

	return (M4_SUCCESS);
}


m4int8_t ClTblMem::GetM4Type (m4int32_t ai_lCol)
{
	return (m_paType[ai_lCol]);
}


m4return_t ClTblMem::SetIdent (m4int32_t ai_lCol, m4int32_t ai_lIdent)
{
	m_paIdent[ai_lCol] = ai_lIdent;

	return (M4_SUCCESS);
}


m4int32_t ClTblMem::GetIdent (m4int32_t ai_lCol)
{
	return (m_paIdent[ai_lCol]);
}


m4pchar_t ClTblMem::GetCurrentPointer (void)
{
	return (m_pcCurrentData);
}


m4int32_t ClTblMem::GetNumCol (void)
{
	return (m_lNumCol);
}


m4int32_t ClTblMem::GetNumRow (void)
{
	return (((m4int32_t) (m_plCurrentOffset - m_plFirstOffset)) / m_lNumCol);
}


m4int32_t ClTblMem::GetDataSize (void)
{
	return (m_lMaxData);
}


m4int32_t ClTblMem::GetDataUsed (void)
{
	return ((m4int32_t) (m_pcCurrentData - m_pcFirstData));
}


m4int32_t ClTblMem::GetDataFree (void)
{
	return (m_lMaxData - (m4int32_t) (m_pcCurrentData - m_pcFirstData));
}


m4int32_t ClTblMem::GetOffsetSize (void)
{
	return (m_lMaxOffset);
}


m4int32_t ClTblMem::GetOffsetUsed (void)
{
	return ((m4int32_t) (m_plCurrentOffset - m_plFirstOffset));
}


m4int32_t ClTblMem::GetOffsetFree (void)
{
	return (m_lMaxOffset - (m4int32_t) (m_plCurrentOffset - m_plFirstOffset));
}


m4pcchar_t ClTblMem::GetData (m4int32_t ai_lRow, m4int32_t ai_lCol)
{
	return (GetDataPointer (ai_lRow, ai_lCol));
}


m4return_t ClTblMem::SetIsPK (m4int32_t ai_lCol, m4bool_t ai_bIsPK)
{
	m_paIsPK[ai_lCol] = ai_bIsPK;

	return (M4_SUCCESS);
}


m4bool_t ClTblMem::GetIsPK (m4int32_t ai_lCol)
{
	return (m_paIsPK[ai_lCol]);
}


