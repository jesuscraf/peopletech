//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             aregister.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "dm_types.hpp"

#include "operation.hpp"
#include "item.hpp"
#include "handles.hpp"
#include "mmindex.hpp"

#include "filter_i.hpp"

//#include "aregwrite.hpp"

#include "accesspr.hpp"

#include "not_cli.hpp"
#include "checkmem.hpp"
//## end module.includes preserve=yes

#ifndef __AREGISTER__HPP__
#define __AREGISTER__HPP__

class ClChannel_Data;
class ClRecordSet;
class ClRegister;
class ClAccess;
class ClRowIndex;
class ClCompiledMCR;
class ClBlock_Channel_Proxy;
class ClAccess;
class ClNode;
class ClNodeDef;
class ClRegister_Pipe;
class ClNotifier_Channel_Data;
class ClNodeDef;
class ClChannelDef;
class ClRegisterBookMark;
class ClFileName;

class ClRegister_Insert;
class ClRegister_Delete;

template<class T>
class ClChildNodes;

template<class T, class S,class R>
class ClTotalize;

#include "mmindex.hpp"

class ClBlockReference;

///////////////////////////////////////////////////////////////////////////////
//
// ClLastUpdated
//

class _ClRegister_Base;

class M4_DECL_M4DM ClLastUpdated
{
public:
    m4return_t Get (VersionMark &ai_vm);
    m4return_t Set (VersionMark ai_vm);

	m4return_t GetSize(ClSize &ao_size);

private:
    ClLastUpdated ();
    void Attach (_ClRegister_Base *ai_pr);

    _ClRegister_Base *m_pClRegister;

friend class _ClRegister_Base;
};

///////////////////////////////////////////////////////////////////////////////
//
// ClPersist_Register
//

class M4_DECL_M4DM ClPersist_Register
{
public:
    m4return_t Get (m4bool_t &ai_bool);
    m4return_t Set (m4bool_t ai_bool);

	m4return_t GetSize(ClSize &ao_size);

private:
    ClPersist_Register ();
    void Attach (_ClRegister_Base *ai_pr);

    _ClRegister_Base *m_pClRegister;

friend class _ClRegister_Base;
};


///////////////////////////////////////////////////////////////////////////////
//
// _ClRegister_Base
//


class M4_DECL_M4DM _ClRegister_Base : public ClAccessProvider, public _AClClient_Notifier
{
public:
    _ClRegister_Base (ClAccess *ai_paccess, m4bool_t ai_autoloadmode = M4_FALSE) :
        Item (ai_paccess, M4DM_SCOPE_ALL), 
        ClAccessProvider (ai_paccess),
        m_pClNode_Data (0),
        Index (M4DM_EOF_INDEX),
        MMIndex (ClMMIndex_EOF)
    {
        m_bautoloadmode = ai_autoloadmode;
        _CommonInit ();
    }

    _ClRegister_Base (const _ClRegister_Base &ai_reg) :
        Item (ai_reg.GetpAccess (), M4DM_SCOPE_ALL), 
        ClAccessProvider (ai_reg.GetpAccess ()),
        m_pClNode_Data (0)
    {
        _CommonInit ();

        _Copy (ai_reg);
    }

    virtual ~_ClRegister_Base ();

    _ClRegister_Base &operator= (const _ClRegister_Base &ai_register)
    {
        _Copy (ai_register);
        return *this;
    }

    ClItem                  Item;
    ClRegisterIndex         Index;

// Nuevo: Identificador unico de registros:
    m4return_t GetRecordID (m4uint32_t &ao_iUniqueId);
	m4return_t SetRecordID (m4uint32_t ai_iUniqueId);
    m4return_t GetIndexOf (m4uint32_t ai_iUniqueId, ClRegisterIndex& ao_riIndex);


    m4return_t UpdateIndex(void);

    //-------------    
    ClLastUpdated           LastUpdated;    // a extinguir
    m4return_t GetLastUpdated (VersionMark &ao_vm);
    m4return_t SetLastUpdated (VersionMark &ai_vm);
    //-------------

    //-------------
    ClFilterInterface Filter;
    //-------------

    //-------------
    ClPersist_Register      FlagPersist;    // a extinguir
    m4return_t GetFlagPersisted (m4bool_t &ao_persist);
    m4return_t SetFlagPersisted (m4bool_t ai_persist);
    //-------------

    //-------------
    ClOperation_Register    Operation;      // a extinguir
    m4return_t GetStatus  (RegisterStatus_t &ao_riStatus);
    m4return_t IsModified (m4bool_t &ao_is);
    m4return_t IsNew      (m4bool_t &ao_is);
    m4return_t IsUpdated  (m4bool_t &ao_is);
    m4return_t IsDeleted  (m4bool_t &ao_is);
    m4return_t IsValid    (m4bool_t &ao_is);
    m4return_t IsModifiedForDDBB (m4bool_t &ao_is);
    m4return_t SetUpdated (void);
    //-------------

	//////
	//
	// Define the operators as virtual - SAM
	//
    virtual _ClRegister_Base& BaseIndex(ClRegisterIndex ai_regindex) = 0;
	virtual _ClRegister_Base* Clone(m4bool_t ai_bTotal = M4_FALSE) = 0;
	//
	//////

    //----------------------------
    // comparaciones
    m4int8_t Compare (const _ClRegister_Base &ai_reg) const;
    m4int8_t _compare_with_order (_ClRegister_Base &ai_reg);
    m4bool_t operator <  (const _ClRegister_Base &ai_reg) const;
    m4bool_t operator >  (const _ClRegister_Base &ai_reg) const;
    m4bool_t operator <= (const _ClRegister_Base &ai_reg) const;
    m4bool_t operator >= (const _ClRegister_Base &ai_reg) const;
    m4bool_t operator == (const _ClRegister_Base &ai_reg) const;
    m4bool_t operator != (const _ClRegister_Base &ai_reg) const;
    //----------------------------

    //----------------------------
    //Iterator interface:
    virtual m4bool_t     Begin (void);
    virtual m4bool_t     End (void);
    virtual m4bool_t     Next (void);
    virtual m4bool_t     Prev (void);    
    virtual m4bool_t     MoveTo (ClRegisterIndex ai_newindex);
    virtual m4bool_t	 MoveToMM (ClMMIndex ai_MMIndex);
    virtual void         MoveToEOF (void);
    virtual m4bool_t     MoveToMandatory (ClRegisterIndex ai_newindex);
    //----------------------------

    //----------------------------
    // Write y Delete

    virtual m4return_t Add (m4return_t * ao_pDone = 0);
    virtual m4return_t Insert (m4return_t * ao_pDone = 0);

    m4return_t         Delete (m4return_t * ao_pDone = 0);
    m4return_t         Destroy (m4return_t * ao_pDone = 0);

    m4return_t         DeleteAll (m4return_t * ao_pDone = 0);
    m4return_t         DestroyAll (m4return_t * ao_pDone = 0);

    m4bool_t           GetNewNotPersist(void) const;
    m4return_t         SetNewNotPersist(const m4bool_t ai_bNewNotPersist);

    void         DestroyRBChilds (ClMMIndex ai_regindex);
    //----------------------------

    m4return_t CopyRecordWithSerialize(ClFileName & ai_oFileName);

    m4return_t PasteRecordWithDeSerialize(const m4bool_t     ai_bNewRecord,
                                          const m4bool_t     ai_bKeepMarks,
                                          const ClFileName & ai_oFileName,
                                          const m4bool_t     ai_bDeleteFile,
                                          const m4bool_t     ai_bDestroyRecords = M4_FALSE,
                                          const m4char_t*    ai_vpExclusionNodeName[] = 0,
                                          const m4uint16_t   ai_iNumExclusionNodes = 0,
                                          const m4bool_t     ai_bKeepAccesses = M4_FALSE);


    m4return_t ReviewNewRegisters (m4uint32_t & ao_index);

    m4bool_t     BlockIsLoaded(void);
    m4uint32_t   Count (void);

    m4bool_t     Find (m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions = 0);

	m4return_t   FindSec (m4VariantType *ai_pstArguments, m4uint16_t ai_iArguments, m4int32_t *ao_iRegister = 0);

    m4bool_t     Exist (m4VariantType *ai_pstArguments);

	void		 SetFirstToWrite (m4uint32_t ai_index);
    m4uint32_t   GetFirstToWrite (void);
    // Caso especial: Carga parcial y Selects divididas.
	void		 SetEndOfRegisters (m4bool_t ai_bEOR);

	//----------------
	//a extinguir
    ClBlock_Channel_Proxy *GetChildBlock (ClHandle ai_handle);
    ClBlock_Channel_Proxy *GetChildBlock (m4uint16_t ai_childindex);
    ClBlock_Channel_Proxy *GetChildBlock (m4pchar_t ai_childid);
    ClBlock_Channel_Proxy *GetCurrentBlock (void) const;
	void SetCurrentBlock (ClBlock_Channel_Proxy *ai_pblock);

	//nuevas:
    m4return_t GetChildBlock (ClHandle ai_handle, ClBlockReference &ai_br);
    m4return_t GetChildBlock (m4uint16_t ai_childindex, ClBlockReference &ai_br);
    m4return_t GetChildBlock (m4pchar_t ai_childid, ClBlockReference &ai_br);
	const ClBlockReference &GetCurrentBlockRef (void) const
    {
        return Item.GetCurrentBlockRef ();
    }
	void SetCurrentBlock (const ClBlockReference &ai_br);
	//----------------
    
    //redefinidas en _ClRegister_Free:
    //void Attach (ClAccess *ai_paccess);
    virtual void Attach (ClNode_Data *ai_pnc);
    virtual void Detach (void);

    m4bool_t   IsAttached (void);
    
    m4return_t CopyDataFrom (_ClRegister_Base &ai_reg);
    _ClRegister_Base &operator <<(_ClRegister_Base &ai_reg);

	//interface para el totalizador:
	ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue>* GetValueAddr(void);
	m4bool_t _IsReadyForValue (void);
	m4uint8_t GetTotType ( void ) const;

    // Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);

    void SendRegisterMark(const m4bool_t ai_bBranchMode);
    // Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    ClMMIndex GetMMIndex (void) const
    {
        return MMIndex;
    }

//----------------------------
//interface _AClClient_Notifier:
    void DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
    void DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
    
    void AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
    void InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);

    void DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc);
    void DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void ReleaseAll_Before (void);
    void DestroyChannelData_Before (void);
    void DestroyChannel_Before (ClChannel * ai_poCh);

    void ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh);
//    ClChannel * GetpChannel (void);

/*
    void Detached_Before (ClNotifier_Channel_Data *ai_poldnotifier)
    {
        m_pClNotifier = 0;
    }

    void Attached_After (ClNotifier_Channel_Data *ai_pnewnotifier)
    {
        m_pClNotifier = ai_pnewnotifier;
    }
*/

//a ver que pasa con estas:
    void Notify_DeleteRegister_After (void);
    void Notify_DeleteAllRegister_After (void);
//----------------------------

    void Follow (const _ClRegister_Base &ai_reg);    

	m4return_t _UndoCascade (void) ;
	m4return_t Undo(void);

    m4return_t CheckPoint(void);

	m4return_t GetSize(ClSize &ao_size);

    //m4return_t SetAutoLoadMode (m4uint8_t ai_mode);

//-------------------------
//with node_data ref.
    public:
        ClNode_Data *GetpNode_Data (void) const
        {
            return m_pClNode_Data;
        }

        ClNode * GetpNode (void) const;
        ClNode_Kernel *GetpNode_ExtInfo (void) const;
        ClNodeDef *GetpNodeDef (void) const;
        ClChannelDef *GetpChannelDef (void) const;

    private:
        void _SetpNode_Data (ClNode_Data *ai_pnd)
        {
            m_pClNode_Data = ai_pnd;
        }

        ClNode_Data *m_pClNode_Data;
//-------------------------

    public:
        ClChannel *GetpChannel (void);

//------------------------------------
//seguridad:
public:
    //m4return_t InitSecurity(void);

    //static ClRegister_Insert ms_ClRegInsert;
    //static ClRegister_InsertNihil ms_ClRegInsertNihil;

    //static ClRegister_Delete ms_ClRegDelete;
    //static ClRegister_DeleteNihil ms_ClRegDeleteNihil;

friend class ClRegister_Insert;
friend class ClRegister_Delete;
//------------------------------------
// Gestion d Errores vuelca el nombre e id de nodo y canal


//----------------------------
//pipe    
    protected:
        m4return_t   SetLotOfItems (ClRegister_Pipe * ai_pRegPipe, m4bool_t ai_bAddReg = M4_TRUE);
	    m4return_t	 SetLotOfItemsPartial (ClRegister_Pipe * ai_pRegPipe, m4bool_t ai_bAuxLoad = M4_FALSE);

    friend class ClRegister_Pipe;
//----------------------------

public:
    void _Copy (const _ClRegister_Base &ai_reg, m4bool_t ai_bTotal = M4_FALSE);
    ClMMIndex       MMIndex;

protected:
    m4return_t _DeleteCascade (void);
    m4uint8_t _GetAutoLoadMode (void);

private:
    //_ClRegister_Insert * m_pClRegInsert;
    //_ClRegister_Delete * m_pClRegDelete;

    m4bool_t m_bautoloadmode;

    void _CommonInit (void)
    {
        #ifdef _DEBUG
            g_StMemInfo.iRegister++;
        #endif

        // Init del item
        Item.Attach ((_ClRegister_Base *)this);

        FlagPersist.Attach (this);
        LastUpdated.Attach (this);
        Operation.Attach (this);
        Filter.Attach (this);
    }


///////////////////////
// esta es la más bestia de todas, asigna el índice y ya está
// se ha utilizado por el tema de las ordenaciones, por ahora
// se llama sólo en g_arrange_sub_refreshcurrent y g_destroy_sub_setaccesstoindex
// y ahora desde ClRegisterBookMark
    void _SetIndex (ClRegisterIndex ai_newindex);

friend class ClOldRegisterBookMark;
friend class ClFilterUtility;
///////////////////////
};

//para los objetos temporales, etc:

class M4_DECL_M4DM _ClRegister_Free : public _ClRegister_Base
{
public:
    _ClRegister_Free (ClAccess *ai_paccess, m4bool_t ai_bNofierEnabled = M4_TRUE) : 
        _ClRegister_Base (ai_paccess), 
        //m_pClNotifier (0)
        m_bNotifierEnabled (ai_bNofierEnabled)
    {
    }
    
    ~_ClRegister_Free ()
    {
        NotifierDetach ();
    }

    _ClRegister_Free (const _ClRegister_Base &ai_register);
    _ClRegister_Free (const _ClRegister_Free &ai_register);

    _ClRegister_Free &operator= (const _ClRegister_Base &ai_register)
    {
        _ClRegister_Base::operator= (ai_register);
        //_ClRegister_Base::_Copy (ai_reg);
        //_Copy (ai_register);
        return *this;
    }

    _ClRegister_Free &operator= (const _ClRegister_Free &ai_register)
    {
        _ClRegister_Base::operator= (ai_register);
        //_ClRegister_Base::_Copy (ai_reg);
        //_Copy (ai_register);
        return *this;
    }

    void NotifierDetach (void);
    void NotifierAttach (ClNotifier_Channel_Data *ai_pnotifier);
    ClNotifier_Channel_Data *GetpNotifier (void) const;

    void Attach (ClAccess *ai_paccess);
    //virtuales en _ClRegister_Base
    void Attach (ClNode_Data *ai_pnc);
    void Detach (void);    

	m4return_t GetSize(ClSize &ao_size);
private:
    //void _Copy (const _ClRegister_Free &ai_reg);
    //void _Copy (const _ClRegister_Base &ai_reg);

    //ClNotifier_Channel_Data *m_pClNotifier; //el notifier al que está asociado
    m4bool_t m_bNotifierEnabled;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, _ClRegister_Base &ai_register);

#endif  //__AREGISTER__HPP__  
