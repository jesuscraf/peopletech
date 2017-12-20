//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cltblmem.hpp   
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


#include "defmain.hpp"
#include "cltblmem.hpp"
#include "clldb.hpp"
#include "cllconn.hpp"
#include "ldb_log.hpp"

#include "cltblmem.inl"


m4return_t ClTblMem::Init (ClLdb	*ai_pLdb,m4int32_t ai_lNumCol, m4int32_t ai_lDefRow, m4int32_t ai_lDefData)
{
	m_pLdb = ai_pLdb;

	if (m_eState == M4LDB_CLOSE) {
	
		m_lNumCol = ai_lNumCol;
		m_lMaxOffset = ai_lDefRow * ai_lNumCol + 1;
		m_lMaxData = ai_lDefData;

		m_paType = new m4int8_t[m_lNumCol];
		m_paIdent = new m4int32_t[m_lNumCol];
		m_paIsPK = new m4bool_t[m_lNumCol];

		m4int32_t indCol;

		for (indCol=0; indCol<m_lNumCol; indCol++)
		{
			m_paIdent[indCol] = M4LDB_NULL_INDEX;
			m_paType[indCol] = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING );
			m_paIsPK[indCol] = M4_FALSE;
		}
	}
	
	if (!m_plFirstOffset) {
		m_plFirstOffset = new m4int32_t[m_lMaxOffset];
	}

	m_plCurrentOffset = m_plFirstOffset;

	if (!m_pcFirstData) {
		m_pcFirstData = new m4char_t[m_lMaxData];
	}

	m_pcCurrentData = m_pcFirstData;

	*m_plFirstOffset = 0;

	m_lNextCol = 0;

	m_eState = M4LDB_OPEN;

	return (Clear ());
}


m4return_t ClTblMem::Close (void)
{
	Release ();

	if (m_paType) {
		delete [] m_paType;
	}
	m_paType = NULL;

	if (m_paIdent) {
		delete [] m_paIdent;
	}
	m_paIdent = NULL;

	if (m_paIsPK) {
		delete [] m_paIsPK;
	}
	m_paIsPK = NULL;

	m_eState = M4LDB_CLOSE;

	return (M4_SUCCESS);
}


m4return_t ClTblMem::Release (void)
{
	if (m_plFirstOffset) {
		delete [] m_plFirstOffset;
	}
	m_plFirstOffset = NULL;

	if (m_pcFirstData) {
		delete [] m_pcFirstData;
	}
	m_pcFirstData = NULL;

	return (M4_SUCCESS);
}


m4int32_t ClTblMem::FindIndexByIdent (m4int32_t ai_lIdent)
{
	m4int32_t indCol;

	for (indCol=0; indCol<m_lNumCol; indCol++) {
		if (m_paIdent[indCol] == ai_lIdent) {
			return (indCol);
		}
	}

	return (M4LDB_NULL_INDEX);
}


m4return_t ClTblMem::NextCurrent (m4int32_t ai_lLenData)
{
	m4int32_t lenOff = *m_plCurrentOffset;

	if (GetOffsetFree () <= 1) {

		m4int32_t lNewSize = m_lMaxOffset * 2;

		m4pint32_t plNewOffset = new m4int32_t[lNewSize];

		memcpy (plNewOffset, m_plFirstOffset, m_lMaxOffset * sizeof (m4int32_t));

		m_plCurrentOffset = plNewOffset + GetOffsetUsed ();

		delete [] m_plFirstOffset;

		m_plFirstOffset = plNewOffset;

		m_lMaxOffset = lNewSize;
	}
	
	m_plCurrentOffset++;

	*m_plCurrentOffset = ai_lLenData + lenOff;

	m_pcCurrentData += ai_lLenData;

	return (M4_SUCCESS);
}


m4return_t ClTblMem::ReallocData (m4int32_t ai_lNewSize)
{
	m4int32_t lNewSize = ((ai_lNewSize) ? ai_lNewSize : m_lMaxData * 2);

	if (lNewSize <= m_lMaxData) {
		lNewSize += m_lMaxData;
	}

	m4pchar_t pcNewData = new m4char_t[lNewSize];

	memset (pcNewData, '\0', lNewSize * sizeof (m4char_t));

	memcpy (pcNewData, m_pcFirstData, m_lMaxData * sizeof (m4char_t));

	m_pcCurrentData = pcNewData + *m_plCurrentOffset;

	delete [] m_pcFirstData;

	m_pcFirstData = pcNewData;

	m_lMaxData = lNewSize;

	return (M4_SUCCESS);
}


m4return_t ClTblMem::ReserveCurrentData (m4int32_t ai_lLenData)
{
	m4return_t result;
	m4int32_t lNewLenData = ai_lLenData;

	m_lNextCol++;

	if (m_lNextCol >= m_lNumCol) {
		m_lNextCol = 0;
	}

	m4int8_t m4Type = GetM4Type (m_lNextCol);

	if (m4Type == M4CL_M4_TYPE_NUMBER || 
		m4Type == M4CL_M4_TYPE_DATE || 
		m4Type == M4CL_M4_TYPE_TIMESTAMP) {
		lNewLenData = M4_ROUND_TO_8 (*m_plCurrentOffset + ai_lLenData) - *m_plCurrentOffset;
	}

	while (GetDataFree () < lNewLenData) {

		result = ReallocData ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	if (ai_lLenData > 0) {

		result = NextCurrent (lNewLenData);
	}
	else {
		if (lNewLenData > 0) {
			m4pint32_t pCur = m_plCurrentOffset;

			m4int32_t len = 0;

			do {
				len = *pCur - *(pCur - 1);

				*pCur = *pCur + lNewLenData;

				pCur--;

			} while (!len);

			m_pcCurrentData += lNewLenData;
		}

		result = NextCurrent (0);
	}

	return (result);
}


m4return_t ClTblMem::SetCurrentData (m4pcvoid_t ai_pcData, m4int32_t ai_lLenData)
{
	m4return_t result;
	m4int32_t lNewLenData = ai_lLenData;

	m_lNextCol++;

	if (m_lNextCol >= m_lNumCol) {
		m_lNextCol = 0;
	}

	m4int8_t m4Type = GetM4Type (m_lNextCol);

	if (m4Type == M4CL_M4_TYPE_NUMBER || 
		m4Type == M4CL_M4_TYPE_DATE || 
		m4Type == M4CL_M4_TYPE_TIMESTAMP) {
		lNewLenData = M4_ROUND_TO_8 (*m_plCurrentOffset + ai_lLenData) - *m_plCurrentOffset;
	}

	while (GetDataFree () < lNewLenData) {

		result = ReallocData ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	if ((ai_pcData) && 
		(ai_lLenData > 0)) {

		memcpy (m_pcCurrentData, ai_pcData, ai_lLenData);

		result = NextCurrent (lNewLenData);
	}
	else {
		if (lNewLenData > 0) {
			m4pint32_t pCur = m_plCurrentOffset;

			m4int32_t len = 0;

			do {
				len = *pCur - *(pCur - 1);

				*pCur = *pCur + lNewLenData;

				pCur--;

			} while (!len);

			m_pcCurrentData += lNewLenData;
		}

		result = NextCurrent (0);
	}

	return (result);
}


m4return_t ClTblMem::AppendCurrentData (m4pcvoid_t ai_pcData, m4int32_t ai_lLenData)
{
	m4return_t result;

	if ((ai_pcData) && 
		(ai_lLenData > 0)) {

		while (GetDataFree () < ai_lLenData) {

			result = ReallocData ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		/* Bug 0150086
		Ya no se pone el nulo final
		*/
		memcpy (m_pcCurrentData, ai_pcData, ai_lLenData);

		*m_plCurrentOffset = ai_lLenData + *m_plCurrentOffset;

		m_pcCurrentData += ai_lLenData;
	}

	return (M4_SUCCESS);
}


m4return_t ClTblMem::SetData (m4int32_t ai_lRow, m4int32_t ai_lCol, m4pcvoid_t ai_pcData)
{
	m4pint32_t plOffset = m_plFirstOffset + (ai_lRow * m_lNumCol) + ai_lCol;

	m4pchar_t pcData = m_pcFirstData + *plOffset;

	if ((pcData > m_pcCurrentData) || 
		(pcData < m_pcFirstData)) {
		DUMP_LDBINSP_ERRORF (m_pLdb->GetSysLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClTblMem::SetData" << "Out of range");
		return (M4_ERROR);
	}

	m4int32_t lLength = *(plOffset + 1) - *plOffset;

	if (lLength > 0) {
		memcpy (pcData, ai_pcData, lLength);
	}

	return (M4_SUCCESS);
}


m4int32_t ClTblMem::GetLength (m4int32_t ai_lRow, m4int32_t ai_lCol)
{
	m4pint32_t plOffset = m_plFirstOffset + (ai_lRow * m_lNumCol) + ai_lCol;
	
	if (plOffset >= m_plCurrentOffset) {
		DUMP_LDBINSP_ERRORF (m_pLdb->GetSysLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClTblMem::GetLength" << "Out of range");
		return (M4LDB_SQL_NULL_DATA);
	}

	m4int32_t lLength = *(plOffset + 1) - *plOffset;

	if (!lLength) {
		return (M4LDB_SQL_NULL_DATA);
	}

	return (lLength);
}


m4pchar_t ClTblMem::GetDataPointer (m4int32_t ai_lRow, m4int32_t ai_lCol)
{
	m4pint32_t plOffset = m_plFirstOffset + (ai_lRow * m_lNumCol) + ai_lCol;

	m4pchar_t pcData = m_pcFirstData + *plOffset;

	if ((pcData > m_pcCurrentData) || 
		(pcData < m_pcFirstData)) {
		DUMP_LDBINSP_ERRORF (m_pLdb->GetSysLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClTblMem::GetDataPointer" << "Out of range");
		return (NULL);
	}

	m4int32_t lLength = *(plOffset + 1) - *plOffset;

	if (!lLength) {
		return (NULL);
	}

	return (pcData);
}


m4return_t ClTblMem::Compare (ClTblMem *ai_pTblMem)
{
	if (ai_pTblMem->GetNumCol () != GetNumCol ()) {
#ifdef _M4LDB_DEBUG
		m_pLdb->GetSysLConn ()->m_pInspector->PrintFileDebug (m_pLdb->GetSysLConn (), "\n ***** ERROR *****  \n Different number of columns. Old = %ld, New = %ld", ai_pTblMem->GetNumCol (), GetNumCol ());
#endif
		return (M4_ERROR);
	}

	if (ai_pTblMem->GetNumRow () != GetNumRow ()) {
#ifdef _M4LDB_DEBUG
		m_pLdb->GetSysLConn ()->m_pInspector->PrintFileDebug (m_pLdb->GetSysLConn (), "\n ***** ERROR *****  \n Different number of rows. Old = %ld, New = %ld", ai_pTblMem->GetNumRow (), GetNumRow ());
#endif
		return (M4_ERROR);
	}

	if (ai_pTblMem->GetDataUsed () != GetDataUsed ()) {
#ifdef _M4LDB_DEBUG
		m_pLdb->GetSysLConn ()->m_pInspector->PrintFileDebug (m_pLdb->GetSysLConn (), "\n ***** ERROR *****  \n Different number of character read. Old = %ld, New = %ld", ai_pTblMem->GetDataUsed (), GetDataUsed ());
#endif
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4bool_t ClTblMem::ComparePKs (ClTblMem *ai_pTblMem, m4int32_t ai_lRow)
{
	// Recuerda, para que esto funcione las PKs de ambas tablas tienen que estar en el mismo orden.
	m4int32_t indCol1 = 0;
	m4int32_t indCol2 = 0;
	m4int32_t numCol1 = m_lNumCol;
	m4int32_t numCol2 = ai_pTblMem->m_lNumCol;

	for (; indCol1<numCol1; indCol1++) {

		m4bool_t bOK = M4_FALSE;

		if (m_paIsPK[indCol1]) {

			for (; ((indCol2<numCol2) && (!bOK)); indCol2++) {

				if (ai_pTblMem->m_paIsPK[indCol2]) {

					m4pcchar_t pData1 = GetData (1, indCol1);
					// Esto es por si hay OldValue.
					if (!pData1) {
						 pData1 = GetData (0, indCol1);
					}
					
					if (!M4LDB_COMPARE_VALUES (pData1, ai_pTblMem->GetData (ai_lRow, indCol2), m_paType[indCol1])) {
						bOK = M4_TRUE;
					}
					if (!bOK) {
						return (M4_FALSE);
					}
				}
			}
			if (!bOK) {
				return (M4_FALSE);
			}
		}
	}

	return (M4_TRUE);
}
