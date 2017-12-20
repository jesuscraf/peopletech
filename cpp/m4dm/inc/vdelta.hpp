#ifndef __VECTORDELTA__HPP__
#define __VECTORDELTA__HPP__

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                VDelta.hpp
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

#include "m4dm_dll.hpp"
#include "dm_types.hpp"

class ClGenericIODriver;
class ClSize;


// ----------------------------------------------------------------------------
// class ClDeltaInfo
// ----------------------------------------------------------------------------

// Si un delta tiene este valor en clientIndex, indica que se trata de un nuevo registro
const m4int32_t VD_NEW_RECORD = 0x1FFFFFFF;   

// Mascaras para el tratamiento de ClDeltaInfo, como entero de 32 bits
const m4uint32_t VD_MODIFIED     = 0x80000000;
const m4uint32_t VD_DELETED      = 0x40000000;
const m4uint32_t VD_INVALID      = 0x20000000;
const m4uint32_t VD_CLIENT_INDEX = VD_NEW_RECORD;


class ClDeltaInfo
{
   public:
      ClDeltaInfo(void);

      void OnModified(void);
      void OnDeleted (void);
      void OnInvalid (void);
      
      void OffModified(void);
      void OffDeleted (void);
      void OffInvalid (void);

      m4bool_t IsInserted(void) const;
      m4bool_t IsModified(void) const;
      m4bool_t IsDeleted (void) const;
      m4bool_t IsInvalid (void) const;

      regindex_t ClientIndex(void) const;
      void ClientIndex(const regindex_t ai_clientIndex);

      m4return_t Serialize(ClGenericIODriver& IOD);
      m4return_t DeSerialize(ClGenericIODriver& IOD);

      // UsedMemory
      m4return_t GetSize(ClSize& ao_size) const;

   private:
      // El primer bit por la izquierda indica si esta modifiacado,
      // el segundo si esta borrado
      // el tercero si el registro es invalido (para la serializacion avanzada)
      // los otros 29 indican el clienteIndex,
      // reservando el valor más alto (0x1FFFFFFF) para indicar nuevo registro
      m4uint32_t m_iDeltaInfo;
};


// ----------------------------------------------------------------------------
// class ClVectorDelta
// ----------------------------------------------------------------------------

class M4_DECL_M4DM ClVectorDelta
{
public:
	ClVectorDelta(void);

	/**
	 * Inicializa el vector de deltas.
	 * El vector de deltas nunca disminuye de tamaño.
	 *
	 * @param ai_iInitialRecords	Número inicial de registros que va a tener el vector de deltas.
	 * @param ai_iGrowRecords		En cuantos registros debe crecer el vector de deltas,
	 *								cuando se acabe su espacio.
	 * @return m4return_t.
	 */
	m4return_t Init(const regindex_t ai_iInitialRecords, const regindex_t ai_iGrowRecords);

	~ClVectorDelta(void);

	m4return_t Copy(ClVectorDelta& ao_VDelta);

	void OnModifiedByPos(const regindex_t ai_Index);
	void OnDeletedByPos (const regindex_t ai_Index);

	void OffModifiedByPos(const regindex_t ai_Index);
	void OffDeletedByPos (const regindex_t ai_Index);

	m4bool_t GetInsertedByPos(const regindex_t ai_Index);
	m4bool_t GetModifiedByPos(const regindex_t ai_Index);
	m4bool_t GetDeletedByPos (const regindex_t ai_Index);

	regindex_t ClientIndex(regindex_t ai_Index);

	void DeleteByPos(const regindex_t ai_Index);

	m4return_t InsertByPos(const regindex_t ai_Index, const regindex_t ai_iNumber);
	
	void UpdateIndex (m4uint32_t* ai_pindex_qtc);
	
	// Si nos devuelve M4_FALSE es que no se ha producido ningun cambio en el VRB
	m4bool_t ThereIsAnyChange(void) const;
	
	void ThereIsAnyChange(const m4bool_t ai_bAnyChange);
	
	ClDeltaInfo& Get(const regindex_t ai_Index);
	
	regindex_t Count(void) const;
	regindex_t GrowRecords(void) const;
	
	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
	
	// UsedMemory
	m4return_t GetSize(ClSize& ao_size) const;
	
private:
	m4return_t GrowIndex(const regindex_t ai_grow);
	
	ClDeltaInfo* m_index;
	regindex_t   m_iSize;
	regindex_t   m_iCount;
	regindex_t   m_iGrowRecords;
	m4bool_t     m_bAnyChange;
};


// ----------------------------------------------------------------------------
// class ClVVectorDelta
// ----------------------------------------------------------------------------

class M4_DECL_M4DM ClVVectorDelta
{
public:
	ClVVectorDelta(void);

	m4return_t Init(const regindex_t ai_iInitialRecords, const regindex_t ai_iGrowRecords, const m4uint8_t ai_iVectorDeltaCount);

	~ClVVectorDelta(void);

	// Añade un nuevo vector de deltas. El nuevo vector de deltas tiene el
	// mismo tamaño y factor de crecimiento que los otros vectores de deltas
	m4uint8_t AddVector(void);

	// Devuelve el vector de deltas situado en la posicion especificada
	ClVectorDelta* GetVector(const m4uint8_t ai_iVector);

	m4uint8_t Count(void) const;

	m4return_t ResetVector(const m4uint8_t ai_iVector);

	//
	// Las siguientes funciones trabajan sobre todos los vectores de deltas
	//
	void OnModifiedByPos(const regindex_t ai_Index);
	void OnDeletedByPos (const regindex_t ai_Index);

	void OffModifiedByPos(const regindex_t ai_Index);
	void OffDeletedByPos (const regindex_t ai_Index);

	void DeleteByPos(const regindex_t ai_Index);

	m4return_t InsertByPos(const regindex_t ai_Index, const regindex_t ai_iNumber);

	void UpdateIndex (m4uint32_t *ai_pindex_qtc);

	//
	// Las siguientes funciones trabajan sobre el vector activo de deltas que se indique
	// Por defecto trajan con el primero, esto es cierto en la mayoria de los casos
	//
	m4bool_t GetInsertedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector = 0) const;

	m4bool_t GetModifiedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector = 0) const;

	m4bool_t GetDeletedByPos(const regindex_t ai_Index, const m4uint8_t ai_iVector = 0) const;

	regindex_t ClientIndex(const regindex_t ai_Index, const m4uint8_t ai_iVector = 0) const;

	m4bool_t ThereIsAnyChange(const m4uint8_t ai_iVector = 0) const;

	void ThereIsAnyChange(const m4bool_t ai_bAnyChange, const m4uint8_t ai_iVector = 0);

	m4return_t Serialize(ClGenericIODriver& IOD, const m4uint8_t ai_iVector = 0);

	m4return_t DeSerialize(ClGenericIODriver& IOD, const m4uint8_t ai_iVector = 0);

	// UsedMemory
	m4return_t GetSize(ClSize& ao_size) const;

private:
	ClVectorDelta* m_index;
	m4uint8_t      m_iSize;
};

#ifdef _M4_USING_INLINE
#include "vdelta.inl"
#endif

#endif   // __VECTORDELTA__HPP__
