//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           operation.hpp
// File:             operation.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             17/06/97
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

#ifndef __OPERATION__HPP__
#define __OPERATION__HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "dm_def.hpp"
#include "clgeniod.hpp"

class _ClRegister_Base;
class _ClRecordSet_Base;
class ClNode_Data;

class ClSize;

//////////////////////////////////////////////////////////////////////////////
//
// _ClOperation_Base
//

class M4_DECL_M4DM _ClOperation_Base
{
public:

    virtual m4bool_t IsModified    (void) const = 0;
    virtual m4bool_t IsNew         (void) const = 0;
    virtual m4bool_t IsUpdated     (void) const = 0;
    virtual m4bool_t IsDeleted     (void) const = 0;

    virtual m4bool_t IsModifiedByDDBB (void) const = 0;
    virtual m4bool_t IsNewByDDBB      (void) const = 0;
    virtual m4bool_t IsUpdatedByDDBB  (void) const = 0;
    virtual m4bool_t IsDeletedByDDBB  (void) const = 0;

    
    virtual m4return_t GetSize(ClSize &ao_size);

private:
};

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_User
//

class M4_DECL_M4DM ClOperation_User : public _ClOperation_Base
{
public:
    ClOperation_User ()
    {
        Reset ();
	}

    ~ClOperation_User () {
	}

    m4bool_t IsModified (void) const
    {
        return (m_Operation != M4DM_OPERATION_NONE)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsNew (void) const
    {
        return (m_Operation & M4DM_OPERATION_INSERT)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsUpdated (void) const
    {
        return (m_Operation & M4DM_OPERATION_UPDATE)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsDeleted (void) const
    {
        return (m_Operation & M4DM_OPERATION_DELETE)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsModifiedByDDBB (void) const
    {
        return (m_Operation >= M4DM_OPERATION_DELETE_BY_DDBB)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsNewByDDBB (void) const
    {
        return (m_Operation & M4DM_OPERATION_INSERT_BY_DDBB)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsUpdatedByDDBB (void) const
    {
        return (m_Operation & M4DM_OPERATION_UPDATE_BY_DDBB)? M4_TRUE : M4_FALSE;
	}

    m4bool_t IsDeletedByDDBB (void) const
    {
        return (m_Operation & M4DM_OPERATION_DELETE_BY_DDBB)? M4_TRUE : M4_FALSE;
	}

	m4return_t Serialize(ClGenericIODriver& IOD)
	{
		return IOD.Write(m_Operation);
	}

	m4return_t DeSerialize(ClGenericIODriver& IOD)
	{
		return IOD.Read(m_Operation);
	}
	m4return_t GetSize(ClSize &ao_size);

    m4bool_t Set (m4uint8_t ai_operation)
    {
        if (m_Operation & ai_operation) {
            // ya estaba asignada la op.
            return M4_FALSE;
        }
        else {
            m_Operation |= ai_operation;
            return M4_TRUE;
        }
	}

    void Reset_NoT_BY_DDBB (void)
    {
        m_Operation ^= M4DM_OPERATION_DELETE;
        m_Operation ^= M4DM_OPERATION_INSERT;
        m_Operation ^= M4DM_OPERATION_UPDATE;
	}

    void ReseT_BY_DDBB (void)
    {
        m_Operation ^= M4DM_OPERATION_DELETE_BY_DDBB;
        m_Operation ^= M4DM_OPERATION_INSERT_BY_DDBB;
        m_Operation ^= M4DM_OPERATION_UPDATE_BY_DDBB;
	}

    void Reset (void)
    {
        m_Operation = M4DM_OPERATION_NONE;
	}

private:
    m4uint8_t m_Operation;

};

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_Register
//

class M4_DECL_M4DM ClOperation_Register : public _ClOperation_Base
{
public:
    ClOperation_Register ();
    ClOperation_Register &operator = (ClOperation_Register &ai_reg);

    m4bool_t IsModified		(void) const;
    m4bool_t IsNew			(void) const;
    m4bool_t IsUpdated		(void) const;
    m4bool_t IsDeleted		(void) const;
    m4bool_t IsNewAndDeleted(void) const;

    m4bool_t IsModifiedByDDBB (void) const;
    m4bool_t IsNewByDDBB      (void) const;
    m4bool_t IsUpdatedByDDBB  (void) const;
    m4bool_t IsDeletedByDDBB  (void) const;

    m4bool_t   IsValid          (void) const;
    m4return_t SetUpdated       (void);
    void       Attach           (_ClRegister_Base *ai_pClRegister);
	m4return_t GetSize          (ClSize &ao_size);

private:
    _ClRegister_Base *m_pClRegister;
};

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_RecordSet
//

class M4_DECL_M4DM ClOperation_RecordSet : public _ClOperation_Base
{
public:
    ClOperation_RecordSet ();
    ClOperation_RecordSet &operator = (ClOperation_RecordSet &ai_reg);

    m4bool_t IsModified    (void) const;
    m4bool_t IsNew         (void) const;
    m4bool_t IsUpdated     (void) const;
    m4bool_t IsDeleted     (void) const;

    m4bool_t IsModifiedByDDBB (void) const;
    m4bool_t IsNewByDDBB      (void) const;
    m4bool_t IsUpdatedByDDBB  (void) const;
    m4bool_t IsDeletedByDDBB  (void) const;

// VRB de bloque (items de bloque)
    m4bool_t BIsModified    (void) const;
    m4bool_t BIsNew         (void) const;
    m4bool_t BIsUpdated     (void) const;
    m4bool_t BIsDeleted     (void) const;

    m4bool_t BIsModifiedByDDBB (void) const;
    m4bool_t BIsNewByDDBB      (void) const;
    m4bool_t BIsUpdatedByDDBB  (void) const;
    m4bool_t BIsDeletedByDDBB  (void) const;


    void       Attach           (_ClRecordSet_Base *ai_prs);
	m4return_t GetSize          (ClSize &ao_size);

private:
    _ClRecordSet_Base *m_pClRecordSet;
};

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_Node
//

class M4_DECL_M4DM ClOperation_Node : public ClOperation_User
{
public:

    ClOperation_Node ();

    m4bool_t IsModified    (void) const;
    m4bool_t IsNew         (void) const;
    m4bool_t IsUpdated     (void) const;
    m4bool_t IsDeleted     (void) const;

    m4bool_t IsModifiedByDDBB (void) const;
    m4bool_t IsNewByDDBB      (void) const;
    m4bool_t IsUpdatedByDDBB  (void) const;
    m4bool_t IsDeletedByDDBB  (void) const;

// RWT de nodo (items de nodo)
    m4bool_t NIsModified    (void) const;
    m4bool_t NIsNew         (void) const;
    m4bool_t NIsUpdated     (void) const;
    m4bool_t NIsDeleted     (void) const;

    m4bool_t NIsModifiedByDDBB (void) const;
    m4bool_t NIsNewByDDBB      (void) const;
    m4bool_t NIsUpdatedByDDBB  (void) const;
    m4bool_t NIsDeletedByDDBB  (void) const;


    void       Attach  (ClNode_Data *ai_pClNode_Data);
	m4return_t GetSize (ClSize &ao_size);

private:
    ClNode_Data * m_pClNode_Data;
};


#endif  //__OPERATION__HPP__ 
