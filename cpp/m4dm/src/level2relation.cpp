//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             level2relation.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/02/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "level2relation.hpp"

#include "clgeniod.hpp"


ClLevel2Relation::ClLevel2Relation(void) :
    m_iParentIndex(-1),
    m_iChildIndex(-1),
    m_iAccessIndex(-1),
    m_sInstance(0)
{
}

ClLevel2Relation::ClLevel2Relation(const ClLevel2Relation& ai_source)
{
	// Copiamos la parte estática.
	m_iParentIndex = ai_source.m_iParentIndex;
	m_iChildIndex  = ai_source.m_iChildIndex;
	m_iAccessIndex = ai_source.m_iAccessIndex;
		
	// Copiamos la cadena
	if (!ai_source.m_sInstance) {
		m_sInstance = 0;
		return;
	}
	
	m4uint16_t iInstanceLength = strlen(ai_source.m_sInstance);
	m_sInstance = new m4char_t [iInstanceLength + 1];
	M4_ASSERT(m_sInstance);

	strcpy((m4pchar_t)m_sInstance, ai_source.m_sInstance);
}

ClLevel2Relation::~ClLevel2Relation(void)
{
    delete[] m_sInstance;
}

ClLevel2Relation& ClLevel2Relation::operator= (const ClLevel2Relation& ai_source)
{
	if (this == &ai_source)
		return *this;
	
	// Copiamos la parte estatica
	m_iParentIndex = ai_source.m_iParentIndex;
	m_iChildIndex  = ai_source.m_iChildIndex;
	m_iAccessIndex = ai_source.m_iAccessIndex;
		
	// Copiamos la cadena
	if (!ai_source.m_sInstance) {
		delete[] m_sInstance;

		m_sInstance = 0;
		return *this;
	}
	
	m4uint16_t iInstanceLength = strlen(ai_source.m_sInstance);
	
	m4int16_t iThisInstanceLength = (m_sInstance) ? strlen(m_sInstance) : -1;
	if ((m4int32_t)iInstanceLength > iThisInstanceLength) {
		delete[] m_sInstance;
		m_sInstance = new m4char_t [iInstanceLength + 1];
	}

	strcpy(m_sInstance, ai_source.m_sInstance);
	
	return *this;
}

void ClLevel2Relation::SetInstance(const m4char_t* const ai_sInstance)
{
	m4uint16_t iInstanceLength = strlen(ai_sInstance);
	
	m4int16_t iThisInstanceLength = (m_sInstance) ? strlen(m_sInstance) : -1;
	if ((m4int32_t)iInstanceLength > iThisInstanceLength) {
		delete[] m_sInstance;
		m_sInstance = new m4char_t [iInstanceLength + 1];
	}

	strcpy(m_sInstance, ai_sInstance);
}

const m4char_t* const ClLevel2Relation::GetInstance(void) const
{
    return m_sInstance;
}

m4return_t ClLevel2Relation::Serialize(ClGenericIODriver& IOD) const
{
	m4return_t iRet = IOD.Write(m_iParentIndex);
	if (iRet != M4_SUCCESS)	return iRet;

	iRet = IOD.Write(m_iChildIndex);
	if (iRet != M4_SUCCESS)	return iRet;

	iRet = IOD.Write(m_iAccessIndex);
	if (iRet != M4_SUCCESS)	return iRet;

	iRet = IOD.Write((void*)m_sInstance, strlen(m_sInstance));

	return iRet;
}

m4return_t ClLevel2Relation::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t iRet = IOD.Read(m_iParentIndex);
	if (iRet != M4_SUCCESS)	return iRet;
	
	iRet = IOD.Read(m_iChildIndex);
	if (iRet != M4_SUCCESS)	return iRet;
	
	iRet = IOD.Read(m_iAccessIndex);
	if (iRet != M4_SUCCESS)	return iRet;

	m4uint32_t iSize;
	iRet = IOD.ReadSize(iSize);
	if (iRet != M4_SUCCESS)	return iRet;
	
	m4int16_t iThisInstanceLength = (m_sInstance) ? strlen(m_sInstance) : -1;
	if ((m4int32_t)iSize > iThisInstanceLength) {
		delete[] m_sInstance;
		m_sInstance = new m4char_t [iSize + 1];
	}

	if (!m_sInstance) return M4_ERROR;

	iRet = IOD.ReadBuffer((void*)m_sInstance);
	m_sInstance[iSize] = 0;

	return iRet;
}

m4return_t ClLevel2RealtionVector::Serialize(ClGenericIODriver& IOD) const
{
	m4return_t iRet = IOD.Write((m4uint16_t)size());
	if (iRet != M4_SUCCESS) return iRet;

	vector<ClLevel2Relation>::const_iterator i = begin();

	for (; i != end(); i++) {
		iRet = (*i).Serialize(IOD);
		if (iRet != M4_SUCCESS) return iRet;
	}

	return iRet;
}

m4return_t ClLevel2RealtionVector::DeSerialize(ClGenericIODriver& IOD)
{
	clear();	// Antes de nada limpio el vector.

	m4uint16_t iCount;
	m4return_t iRet = IOD.Read(iCount);
	if (iRet != M4_SUCCESS)	return iRet;
	
	ClLevel2Relation oLevel2Relation;
	
	for (m4uint16_t i = 0; i < iCount; i++) {
		push_back(oLevel2Relation);
		
		iRet = back().DeSerialize(IOD);
		if (iRet != M4_SUCCESS)	return iRet;
	}
	
	return iRet;
}
