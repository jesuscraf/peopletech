#ifndef __COMPUTIL_HPP__
#define __COMPUTIL_HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             computil.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             23/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Utilities over class ComponentStore
//
//
//==============================================================================

#include "m4dm_dll.hpp"
#include "cldefine.hpp"
#include "dm_types.hpp"


class _AClComponentStore;
class ClBlock_Channel_Proxy;
class ClBlockReference;


// ------------------------------------------------------------------
// class ClCStoreUtilities
// ------------------------------------------------------------------

class M4_DECL_M4DM ClCStoreUtilities
{
public:
	typedef m4return_t (* pf_t)(ClBlockReference &ai_blockref, void *ai_extraarg);

    static m4return_t ForEach (_AClComponentStore *ai_pcstore, pf_t ai_pf, void *ai_parg = 0);

    static m4return_t           DeleteOneSafe (ClBlockReference &ai_blockref, void *);
    static m4return_t           DeleteOneFast (ClBlockReference &ai_blockref, void *);
    static m4return_t    SetCheckPointModeOne (ClBlockReference &ai_blockref, void *);
    static m4return_t           CheckPointOne (ClBlockReference &ai_blockref, void *);
    static m4return_t                 UndoOne (ClBlockReference &ai_blockref, void *);
    static m4return_t        SetKeepStatusOne (ClBlockReference &ai_blockref, void *);
    static m4return_t        ResetSerialMarks (ClBlockReference &ai_blockref, void *);
    static m4return_t      CommitPersistedOne (ClBlockReference &ai_blockref, void *);
    static m4return_t    RollbackPersistedOne (ClBlockReference &ai_blockref, void *);
    static m4return_t                ResetOne (ClBlockReference &ai_blockref, void *);
	static m4return_t               ReLoadOne (ClBlockReference &ai_blockref, void *);
	static m4return_t             CheckModify (ClBlockReference &ai_blockref, void *);
	static m4return_t            RefreshFlags (ClBlockReference &ai_blockref, void *);
    static m4return_t FindRecordAndSetVersion (ClBlockReference &ai_blockref, void *);
    static m4return_t MarkBlockAndParentRecordsForSerialize(ClBlockReference &ai_blockref, void *);
};


// ------------------------------------------------------------------
// class ClChangeRecordDateInfo
// ------------------------------------------------------------------
// Esta clase se utiliza para pasar los parametros en la funcion FindRecordAndSetVersion

class ClChangeRecordVersionInfo
{
public:
    ClChangeRecordVersionInfo(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion)
        : m_iRecordId(ai_iRecordId)
        , m_dNewVersion(ai_dNewVersion)
        , m_bContinue(M4_TRUE)
    {}
    
    m4uint32_t m_iRecordId;
    m4date_t   m_dNewVersion;
    m4bool_t   m_bContinue;
};

class ClKeepStatusInfo
{
public:
	ClKeepStatusInfo(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
		: m_bKeepIt(ai_bKeepIt)
		, m_iVectorCount(ai_iVectorCount)
	{}

	/** Si hay que mantener deltas o no. */
	m4bool_t m_bKeepIt;

	/** En caso de mantener deltas, cuantos vectores queremos. */
	m4uint8_t m_iVectorCount;
};

#endif //__COMPUTIL_HPP__
