//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                VDelta.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Control de deltas en un VRB
//
//==============================================================================

#include "vdelta.hpp"
#include "vdelta.inl"

#include <string.h>

#include "clgeniod.hpp"
#include "sizer.hpp"



// ----------------------------------------------------------------------------
// class ClDeltaInfo
// ----------------------------------------------------------------------------

ClDeltaInfo::ClDeltaInfo(void)
{
   m_iDeltaInfo = VD_NEW_RECORD;
}

m4return_t ClDeltaInfo::Serialize(ClGenericIODriver& IOD)
{
   return IOD.Write(m_iDeltaInfo);
}

m4return_t ClDeltaInfo::DeSerialize(ClGenericIODriver& IOD)
{
   return IOD.Read(m_iDeltaInfo);
}

m4return_t ClDeltaInfo::GetSize(ClSize& ao_size) const
{
    ao_size += sizeof(ClDeltaInfo);
    return M4_SUCCESS;
}


// ----------------------------------------------------------------------------
// class ClVectorDelta
// ----------------------------------------------------------------------------


ClVectorDelta::ClVectorDelta(void):
   m_index(0),
   m_iSize(0),
   m_iCount(0),
   m_iGrowRecords(0),
   m_bAnyChange(M4_FALSE)
{
}

/**
 * Inicializa el vector de deltas.
 * El vector de deltas nunca disminuye de tamaño.
 *
 * @param ai_iInitialRecords	Número inicial de registros que va a tener el vector de deltas.
 * @param ai_iGrowRecords		En cuantos registros debe crecer el vector de deltas,
 *								cuando se acabe su espacio.
 * @return m4return_t.
 */
m4return_t ClVectorDelta::Init(const regindex_t ai_iInitialRecords, const regindex_t ai_iGrowRecords)
{
	M4_MSG_ASSERT(ai_iInitialRecords < VD_NEW_RECORD, "Demasiados registros");
	
	m_bAnyChange = M4_FALSE;

	if ((ai_iInitialRecords + ai_iGrowRecords) > m_iSize) {
		delete[] m_index;
		m_index = 0;

		m_iSize = ai_iInitialRecords + ai_iGrowRecords;
		if (m_iSize) {
			m_index = new ClDeltaInfo[m_iSize];
			if (!m_index) {
				m_iSize = 0;
				m_iCount = 0;
				m_iGrowRecords = 0;
				return M4_ERROR;
			}
		}
	}
	m_iCount       = ai_iInitialRecords;
	m_iGrowRecords = ai_iGrowRecords;

	for (regindex_t i = 0; i < m_iCount; i++) {
		m_index[i].ClientIndex(i);
	}
	
	return M4_SUCCESS;
}

ClVectorDelta::~ClVectorDelta(void)
{
   delete[] m_index;
}

m4return_t ClVectorDelta::Copy(ClVectorDelta& ao_VDelta)
{
   if (ao_VDelta.Init(m_iCount, m_iGrowRecords) != M4_SUCCESS)
      return M4_ERROR;

   for (regindex_t i = 0; i < m_iCount; i++)
      ao_VDelta.Get(i) = m_index[i];
      
   return M4_SUCCESS;
}

void ClVectorDelta::DeleteByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index,"No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");

   m_iCount--;

   // Reuse the index
   if (ai_Index < m_iCount)
      memmove(&m_index[ai_Index],
              &m_index[ai_Index + 1],
              sizeof(ClDeltaInfo) * (m_iCount - ai_Index));

   m_bAnyChange = M4_TRUE;
}

m4return_t ClVectorDelta::InsertByPos(const regindex_t ai_Index, const regindex_t ai_iNumber)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index <= m_iCount, "Index fuera de rango");

   if (m_iCount + ai_iNumber > m_iSize &&
       GrowIndex(ai_iNumber + m_iGrowRecords) != M4_SUCCESS)
      return  M4_ERROR;

   // Move the neccesary indexes
   if (ai_Index < m_iCount)
      memmove(&m_index[ai_Index + ai_iNumber],
              &m_index[ai_Index],
              sizeof(ClDeltaInfo) * (m_iCount - ai_Index));

   for (regindex_t i = ai_Index; i < ai_Index + ai_iNumber; i++)
      m_index[i].ClientIndex(VD_NEW_RECORD);

   m_iCount += ai_iNumber;

   m_bAnyChange = M4_TRUE;

   return M4_SUCCESS;
}

m4return_t ClVectorDelta::GrowIndex(const regindex_t ai_grow)
{
   ClDeltaInfo* newIndex;

   newIndex = new ClDeltaInfo[m_iSize + ai_grow];
   if (newIndex == 0)
      return M4_ERROR;

   // Copy of the previous indexes
   memcpy(newIndex, m_index, sizeof(ClDeltaInfo) * m_iCount);

   m_iSize += ai_grow;

   delete[] m_index;
   m_index = newIndex;

   return M4_SUCCESS;
}


void ClVectorDelta::UpdateIndex (m4uint32_t *ai_pindex_qtc)
{    
   M4_ASSERT (m_iCount > 0);

//esto es una mierda de algoritmo
//nos pasan un array con el mapa de como debe quedar el índice
//  ej: si se tratara de invertir el índice el mapa sería: [3][2][1][0]

   ClDeltaInfo* paux = new ClDeltaInfo[m_iSize];

   register m4uint32_t   i = m_iCount - 1;    
   register ClDeltaInfo* p = paux + i;

   do
   {
      *p = m_index [ ai_pindex_qtc[i] ];
      p--;
   } while (i--);

   delete[] m_index;
   m_index = paux;

   m_bAnyChange = M4_TRUE;
}

regindex_t ClVectorDelta::Count(void) const
{
   return m_iCount;
}

regindex_t ClVectorDelta::GrowRecords(void) const
{
   return m_iGrowRecords;
}

m4return_t ClVectorDelta::Serialize(ClGenericIODriver& IOD)
{
	m4return_t iRet = IOD.Write(m_bAnyChange);
	if (iRet != M4_SUCCESS) return iRet;
	
	if (m_bAnyChange == M4_TRUE) {
		/* No lo necesito, es igual que el del VRB
		if (IOD.Write(m_count) != M4_SUCCESS)
			return M4_ERROR;
		*/
		for (regindex_t i = 0; i < m_iCount; i++) {
			iRet = m_index[i].Serialize(IOD);
			if (iRet != M4_SUCCESS) return iRet;
		}
	}
	return iRet;
}

m4return_t ClVectorDelta::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t iRet = IOD.Read(m_bAnyChange);
	if (iRet != M4_SUCCESS) return iRet;
	
	if (m_bAnyChange == M4_TRUE) {
		/* No lo necesito, es igual que el del VRB
		if (IOD.Read(m_count) != M4_SUCCESS)
			return M4_ERROR;
		*/
		for (regindex_t i = 0; i < m_iCount; i++) {
			iRet = m_index[i].DeSerialize(IOD);
			if (iRet != M4_SUCCESS) return iRet;
		}
	}
	return iRet;
}

m4return_t ClVectorDelta::GetSize(ClSize& ao_size) const
{
    ao_size += sizeof(ClVectorDelta);

    if (m_index != NULL)
    {
        ClSize deltaInfoSize;
        if (m_index[0].GetSize(deltaInfoSize) != M4_SUCCESS)
            return M4_ERROR;
        
        ao_size += deltaInfoSize * m_iSize;
    }
    return M4_SUCCESS;
}


// ----------------------------------------------------------------------------
// class ClVVectorDelta
// ----------------------------------------------------------------------------


ClVVectorDelta::ClVVectorDelta(void):
   m_index(0),
   m_iSize(0)
{
}

/**
 * Inicializa el vector de vectores de deltas.
 * El vector de vectores de deltas nunca disminuye el número de vectores de delta que gestiona.
 *
 * @param ai_iInitialRecords	Número inicial de registros que van a tener los vectores de deltas.
 * @param ai_iGrowRecords		En cuantos registros deben crecer los vectores de deltas,
 *								cuando acaben su espacio.
 * @param ai_iVectorDeltaCount	Número de vectores de delta que queremos crear, o resetear.
 * @return m4return_t.
 */
m4return_t ClVVectorDelta::Init(const regindex_t ai_iInitialRecords,
                                const regindex_t ai_iGrowRecords,
                                const m4uint8_t  ai_iVectorDeltaCount)
{
	M4_ASSERT(ai_iVectorDeltaCount < 3);

	m4return_t iRet = M4_SUCCESS;

	if (ai_iVectorDeltaCount > m_iSize) {
		delete[] m_index;
		m_index = 0;
		m_iSize = 0;
		
		m_index = new ClVectorDelta[ai_iVectorDeltaCount];
		if (!m_index) return M4_ERROR;
		
		m_iSize = ai_iVectorDeltaCount;
	}
	
	// Ojo, tengo que usar ai_iVectorDeltaCount y no m_iSize, ya que este puede ser
	// mayor que aquel, y sólo se quiere hacer el reset de los que me han dicho.
	for (m4uint8_t i = 0; i < ai_iVectorDeltaCount; i++) {
		iRet = m_index[i].Init(ai_iInitialRecords, ai_iGrowRecords);
		if (iRet != M4_SUCCESS) {
			delete[] m_index;
			m_index = 0;
			m_iSize = 0;
			return iRet;
		}
	}

	return iRet;
}

ClVVectorDelta::~ClVVectorDelta(void)
{
   delete[] m_index;
}

m4uint8_t ClVVectorDelta::AddVector(void)
{
    M4_ASSERT(m_iSize > 0);
    
    // Siempre se añade al final
    
    ClVectorDelta* auxIndex = new ClVectorDelta[m_iSize + 1];
    if (auxIndex == 0)
        return (m4uint8_t)-1;
    m4uint8_t i;
    for (i = 0; i < m_iSize; i++)
    {
        if (m_index[i].Copy(auxIndex[i]) != M4_SUCCESS)
        {
            delete[] auxIndex;
            return (m4uint8_t)-1;
        }
    }
    
    if (auxIndex[i].Init(m_index[0].Count(), m_index[0].GrowRecords()) != M4_SUCCESS)
    {
        delete[] auxIndex;
        return (m4uint8_t)-1;
    }
    
    delete m_index;
    m_index = auxIndex;
    
    m_iSize++;
    
    return m_iSize - 1;
}

ClVectorDelta* ClVVectorDelta::GetVector(const m4uint8_t ai_iVector)
{
    if (ai_iVector < m_iSize)
        return &m_index[ai_iVector];
    else
        return NULL;
}

m4uint8_t ClVVectorDelta::Count(void) const
{
   return m_iSize;
}

m4return_t ClVVectorDelta::ResetVector(const m4uint8_t ai_iVector)
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].Init(m_index[ai_iVector].Count(), m_index[ai_iVector].GrowRecords());
}

void ClVVectorDelta::OnModifiedByPos(const regindex_t ai_Index)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].OnModifiedByPos(ai_Index);
}

void ClVVectorDelta::OnDeletedByPos (const regindex_t ai_Index)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].OnDeletedByPos(ai_Index);
}

void ClVVectorDelta::OffModifiedByPos(const regindex_t ai_Index)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].OffModifiedByPos(ai_Index);
}

void ClVVectorDelta::OffDeletedByPos (const regindex_t ai_Index)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].OffDeletedByPos(ai_Index);
}

void ClVVectorDelta::DeleteByPos(const regindex_t ai_Index)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].DeleteByPos(ai_Index);
}

m4return_t ClVVectorDelta::InsertByPos(const regindex_t ai_Index, const regindex_t ai_iNumber)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      if (m_index[i].InsertByPos(ai_Index, ai_iNumber) != M4_SUCCESS)
         return M4_ERROR;

   return M4_SUCCESS;
}

void ClVVectorDelta::UpdateIndex(m4uint32_t *ai_pindex_qtc)
{
   M4_ASSERT(m_iSize > 0);

   for (m4uint8_t i = 0; i < m_iSize; i++)
      m_index[i].UpdateIndex (ai_pindex_qtc);
}

m4bool_t ClVVectorDelta::GetInsertedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector) const
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].GetInsertedByPos(ai_Index);
}

m4bool_t ClVVectorDelta::GetModifiedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector) const
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].GetModifiedByPos(ai_Index);
}

m4bool_t ClVVectorDelta::GetDeletedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector) const
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].GetDeletedByPos(ai_Index);
}

regindex_t ClVVectorDelta::ClientIndex(const regindex_t ai_Index, const m4uint8_t ai_iVector) const
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].ClientIndex(ai_Index);
}

m4bool_t ClVVectorDelta::ThereIsAnyChange(const m4uint8_t ai_iVector) const
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector >= 0 && ai_iVector < m_iSize);

   return m_index[ai_iVector].ThereIsAnyChange();
}

void ClVVectorDelta::ThereIsAnyChange(const m4bool_t ai_bAnyChange, const m4uint8_t ai_iVector)
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   m_index[ai_iVector].ThereIsAnyChange(ai_bAnyChange);
}

m4return_t ClVVectorDelta::Serialize(ClGenericIODriver& IOD, const m4uint8_t ai_iVector)
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].Serialize(IOD);
}

m4return_t ClVVectorDelta::DeSerialize(ClGenericIODriver& IOD, const m4uint8_t ai_iVector)
{
   M4_ASSERT(m_iSize > 0);
   M4_ASSERT(ai_iVector < m_iSize);

   return m_index[ai_iVector].DeSerialize(IOD);
}

m4return_t ClVVectorDelta::GetSize(ClSize& ao_size) const
{
	ao_size += sizeof(ClVVectorDelta);

	if (m_index) {
		for (m4uint8_t i = 0; i < m_iSize; i++) {
			if (m_index[i].GetSize(ao_size) != M4_SUCCESS)
				return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}
