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


#ifndef __CLTBLMEM_HPP__
#define __CLTBLMEM_HPP__


#include "defmain.hpp"

class ClLdb;

class ClTblMem
{
private:

    eStateType_t m_eState;

	m4pchar_t m_pcFirstData;
	m4pchar_t m_pcCurrentData;

	m4pint32_t m_plFirstOffset; 
	m4pint32_t m_plCurrentOffset;

	m4pint32_t m_paIdent;
	m4pint8_t m_paType;
	m4pbool_t m_paIsPK;

	m4int32_t m_lNumCol;
	m4int32_t m_lMaxData;
	m4int32_t m_lMaxOffset;

	m4int32_t m_lNextCol;
	ClLdb	  *m_pLdb;

protected:	
	m4pchar_t GetCurrentPointer (void);
    m4return_t NextCurrent (m4int32_t ai_lLenData);
	m4return_t ReallocData (m4int32_t ai_lNewSize = 0);

public:
	ClTblMem (void);
	virtual ~ClTblMem (void);

	m4return_t Init (ClLdb	 *ai_pLdb,m4int32_t ai_lNumCol, m4int32_t ai_lDefRow = 2, m4int32_t ai_lDefData = 512);
	m4return_t Close (void);

	m4return_t SetM4Type (m4int32_t ai_lCol, m4int8_t ai_cM4Type);
	m4int8_t GetM4Type (m4int32_t ai_lCol);

	m4return_t SetIdent (m4int32_t ai_lCol, m4int32_t ai_lIdent);
	m4int32_t GetIdent (m4int32_t ai_lCol);
	m4int32_t FindIndexByIdent (m4int32_t ai_lIdent);

	m4return_t SetIsPK (m4int32_t ai_lCol, m4bool_t ai_bIsPK);
	m4bool_t GetIsPK (m4int32_t ai_lCol);
	m4bool_t ComparePKs (ClTblMem *ai_pTblMem, m4int32_t ai_lRow);

	m4int32_t GetNumCol (void);
	m4int32_t GetNumRow (void);
	m4int32_t GetDataSize (void);
	m4int32_t GetDataUsed (void);
	m4int32_t GetDataFree (void);
	m4int32_t GetOffsetSize (void);
	m4int32_t GetOffsetUsed (void);
	m4int32_t GetOffsetFree (void);

	m4return_t ReserveCurrentData (m4int32_t ai_lLenData);
	m4return_t SetCurrentData (m4pcvoid_t ai_pcData, m4int32_t ai_lLenData);
	m4return_t AppendCurrentData (m4pcvoid_t ai_pcData, m4int32_t ai_lLenData);

	m4return_t SetData (m4int32_t ai_lRow, m4int32_t ai_lCol, m4pcvoid_t ai_pcData);
	m4pcchar_t GetData (m4int32_t ai_lRow, m4int32_t ai_lCol);
	m4int32_t GetLength (m4int32_t ai_lRow, m4int32_t ai_lCol);
	m4pchar_t GetDataPointer (m4int32_t ai_lRow, m4int32_t ai_lCol);

	m4return_t Clear (void);
	m4return_t Release (void);

	m4return_t Compare (ClTblMem *ai_pTblMem);
};

typedef ClTblMem*	PClTblMem;


#endif

