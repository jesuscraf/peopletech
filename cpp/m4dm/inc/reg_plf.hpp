//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_plf.hpp
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

#ifndef __REG_PLF_HPP__
#define __REG_PLF_HPP__
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "mmindex.hpp"

#include "regi_smp.hpp"

class ClBlock_Channel_Proxy;
class ClAccessProvider;
class ClBlockReference;
class ClNodeDef;
class ClFileIODriver;
class ClChannel_Data;



class M4_DECL_M4DM ClRegisterPartialLoad : public _ClRegisterFilter_Terminator
{
public:
    ClRegisterPartialLoad ();
   ~ClRegisterPartialLoad ();

    m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    Type_e Type (void) const { return _ClRegisterFilter::PartialLoad; }

    m4return_t PrepareIndexPL(m4bool_t ai_bUp, const ClBlockReference &ai_br);

private:
	//Carga de la base de datos
	m4return_t LoadRegisters(ClRegisterIndex ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
	m4double_t PartialLoad(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t LoadWindow(const ClBlockReference &ai_br, ClAccessProvider *ai_pap,
                         m4uint32_t ai_iFR, m4uint32_t ai_iFW, m4uint32_t ai_iNW, 
                         m4uint32_t ai_iFV, m4uint32_t ai_iLV);

	m4return_t	_StoreWindow(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
	m4return_t	_RestoreWindow(const ClBlockReference &ai_br, ClAccessProvider *ai_pap, m4bool_t& ao_rbRestored, m4int32_t& ao_iStoredRegisters);
	m4return_t	_RestoreRegisters(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
	m4return_t	_OpenFile( ClChannel_Data *ai_poChannelData, ClFileIODriver& ai_roIOD, m4uint32_t ai_iMode, const ClNodeDef *ai_pcoNodeDef ) ;


    //ClAccess *m_pAccess;
};

#endif // __REG_PLF_HPP__
