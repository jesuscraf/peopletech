//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             pl_data.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             18/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter declaration
//
//
//==============================================================================

#ifndef __PL_DATA_HPP__
#define __PL_DATA_HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4var.hpp"



typedef map< m4uint32_t, long, less<m4uint32_t> > M4PLFileMap_t ;




class M4_DECL_M4DM ClPartialLoadData
{
public:
    ClPartialLoadData(m4uint32_t ai_iMaxRegm);

    ~ClPartialLoadData(void);

    m4int32_t	GetFirstVisible(void) const;
    m4int32_t	GetLastVisible(void) const;
    m4int32_t	GetNextFV(void) const;
    m4int32_t	GetNextLV(void) const;
    m4int32_t	GetFirstToWrite(void) const;
    m4int32_t	GetNumberToWrite(void) const;
    m4int32_t	GetFirstToRead(void) const;
    m4int32_t	GetReadRegister(void) const;
    m4int32_t	GetCurrentRegister(void) const;
    m4int32_t	GetEnd(void) const;
    m4int32_t	GetPrepareIndexPL(void) const;
    m4int32_t	GetMaxRegisters(void) const;
    m4bool_t	GetLocked(void) const;
    m4bool_t	GetEndOfRegisters(void) const;
    m4bool_t	GetFirstWindowLoaded(void) const;
    m4bool_t	GetLoadFromPageFault(void) const;

    M4PLFileMap_t&	GetFileMap(void);

    void	SetFirstVisible(m4int32_t ai_iFV);
    void	SetLastVisible(m4int32_t ai_iLV);
    void	SetNextFV(m4int32_t ai_iFV);
    void	SetNextLV(m4int32_t ai_iLV);
    void	SetFirstToWrite(m4int32_t ai_iFW);
    void	SetNumberToWrite(m4int32_t ai_iNW);
    void	SetFirstToRead(m4int32_t ai_iFR);
    void	SetReadRegister(m4int32_t ai_iRR);
    void	SetCurrentRegister(m4int32_t ai_iCR);
    void	SetEnd(m4int32_t ai_iEnd);
    void	SetPrepareIndexPL(m4int32_t ai_iPrep);
    void	SetMaxRegisters(void);
    void	SetLocked(m4bool_t ai_bLocked);
    void	SetEndOfRegisters(m4bool_t ai_bEOR);
    void	SetFirstWindowLoaded(m4bool_t ai_bFirstWindowLoaded);
    void	SetLoadFromPageFault(m4bool_t ai_bLoadFromPageFault);

    // Serialization
    m4return_t Serialize(ClGenericIODriver& IOD, m4bool_t ai_bWritePLFileMap);
    m4return_t DeSerialize(ClGenericIODriver& IOD);

	m4return_t GetSize(ClSize &ao_size);

private:

   m4int32_t m_iFirstVisible;
   m4int32_t m_iLastVisible;
   m4int32_t m_iNextFV;
   m4int32_t m_iNextLV;
   m4int32_t m_iFirstToWrite;
   m4int32_t m_iNumberToWrite;
   m4int32_t m_iFirstToRead;
   m4int32_t m_iReadRegister;
   m4int32_t m_iCurrentRegister;
   m4int32_t m_iEnd;
   m4int32_t m_iPrep;
   m4int32_t m_iMaxRegisters;	// Máximo número de registros de base de datos

   m4bool_t m_bLocked;
   m4bool_t m_bEOR;
   m4bool_t m_bFirstWindowLoaded;
   /* Bug 0070883
   Hay que poner un flag de si la carga viene por paginación.
   */
   m4bool_t m_bLoadFromPageFault;

   // Mapa con los registros salvados y su posición en el fichero
   M4PLFileMap_t m_oFileMap;

};

#endif // __REG_PLF_HPP__
