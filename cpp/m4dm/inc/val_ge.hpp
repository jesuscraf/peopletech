//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_ge.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             28/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//	Keep the Status and Control of "Value"
//
//==============================================================================

#ifndef __VAL_GE__HPP__
#define __VAL_GE__HPP__

#include "m4types.hpp"
#include "index.hpp"
#include "m4stl.hpp"

#include "item_prx.hpp"
#include "mmindex.hpp"
#include "v_contxt.hpp"
#include "bnihil.hpp"
#include "m4dm_dll.hpp"
class _ClVirtual_Value_R;
class _ClVirtual_Value_W;
class ClNode_Data;
class m4VariantType ;
class ClBlockReference;
class ClDateStack;
class ClBlock_Channel_Proxy;
class ClItem;
class ClAccess;

class M4_DECL_M4DM ClValue_State
{
public:

//*************************************************************************
// CONSTRUCTORES / DESTRUCTOR
//*************************************************************************

    ClValue_State (_ClGM_Interface *ai_pMemory_Interface,
        ClItemDef *ai_pitemdef) :

        m_oContext (&g_oNihilBlock,
            &s_oClItemProxy_Nihil,
            ai_pMemory_Interface,
            ai_pitemdef),

        Index (M4DM_EOF_INDEX)
    {
    }

    ClValue_State (const ClValue_State &ai_value) :
        m_oContext (0, 0, 0, 0)    //se puede optimizar
    {
        _Copy (ai_value);
    }

    virtual ~ClValue_State ()
    {
    }

//*************************************************************************
// OPERADORES BASICOS
//*************************************************************************

    ClValue_State &operator=  (const ClValue_State &ai_value);
    ClValue_State &operator[] (ClSliceIndex ai_sliceindex);
    ClValue_State &operator[] (m4date_t ai_date);
    ClValue_State &operator++ ();     //pre
    ClValue_State &operator-- ();     //pre

    m4bool_t Begin (void);
    m4bool_t End (void);
    m4bool_t Prev (void);
    m4bool_t Next (void);
    m4bool_t MoveTo (ClSliceIndex ai_index);
    m4bool_t MoveTo (m4date_t ai_date);
    m4bool_t MoveTo (m4date_t ai_startdate, m4date_t ai_enddate);
    void     MoveToEOF (void);

//*************************************************************************
// NUEVAS FUNCIONES QUE SE SALTAN LA SEGURIDAD
//*************************************************************************

    void *     GetWithOutSec (m4return_t * ao_pdone = 0);
    m4return_t GetWithOutSec (m4VariantType &ao_var, m4return_t * ao_pdone = 0);
    m4return_t GetWithOutSec (m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone = 0);

    m4return_t SetWithOutSec (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus);
    m4return_t SetWithOutSec (const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus);
    m4return_t SetNotUpdated (const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser);

    m4return_t SetNULLWithOutSec (void);
    m4return_t SetDefaultWithOutSec (void);
    m4return_t SetPrioritySafe (priority_t ai_prior);

//*************************************************************************
// FUNCIONES DE OPERACION INDEPENDIENTES DE LA SEGURIDAD
//*************************************************************************

    m4bool_t   CheckSlice (m4date_t ai_dstart, m4date_t ai_dend);
    m4return_t FillDateStack (ClDateStack *ai_stack);

    m4date_t GetStartDate (void);
    m4date_t GetEndDate (void);

    m4date_t GetFirstStartDate (void);
    m4date_t GetLastEndDate (void);

    ClSliceIndex GetFirstIndex (void);
    ClSliceIndex GetLastIndex (void);

    m4uint32_t Count (void);
    m4uint32_t Size (void);
    m4return_t IsNull (m4bool_t &ao_isnull);
    m4bool_t   HasSlices (void);
	priority_t GetPriority (void);

//*************************************************************************
// FUNCIONES PARA EL MANTENIMIENTO DEL ESTADO y VOLCADO DE ERRORES
//*************************************************************************

    ClBlock_Channel_Proxy  * GetCurrentBlock    (void) const;
	const ClBlockReference & GetCurrentBlockRef (void) const;
    void                     _SetCurrentBlock   (const ClBlockReference &ai_br);
    void                     _SetItemProxy      (_AClItemProxy *ai_pnewitemproxy);

    void _Attach27 (ClItem *ai_pii, ClAccess * ai_poAccess);
    void _Attach1  (_ClRegister_Base *ai_pr);
    void _Attach1  (ClMMIndex ai_index);
    void _Attach2  (ClChannel_Kernel *ai_pdii, ClNode_Data *ai_prwt);
    void _AttachX  (const _ClValue_Read_Slices &ai_value, ClItem *ai_pii);

    StKernelContext * _GetContext(void);
    ClItem          * _GetpItem_Interface(void);

    //desde item y ejecutor, para saber a que indice está asociado el item
    ClRegisterIndex GetRegisterIndex (void) const;

    //se utiliza desde el itemcon: dado un item hace
    //falta saber a que registro pertenece:
    ClMMIndex GetMMIndex (void) const;

    _ClRegister_Base *GetpRegister () const;

    _ClVirtual_Value_R *GetpKernelR  (void);
    _ClVirtual_Value_W *GetpKernelRW (void);

    void _Copy (const ClValue_State &ai_value);    

	m4return_t GetSize(ClSize &ao_size);

//*************************************************************************
// INTERFACE PARA EL TOTALIZADOR DE TRAMOS
//*************************************************************************

    ClValue_State * GetValueAddr     (void);
    m4uint8_t       GetTotType       (void) const;

//*************************************************************************
// MIEMBROS DE LA CLASE: PUBLICOS / PRIVADOS, ESTATICOS
//*************************************************************************

    static m4uint32_t s_idefault;
    ClSliceIndex Index;        // indice del tramo

    ClItem          *m_pItem_Interface; // se utiliza en el _Notify
    StKernelContext m_oContext;

//*************************************************************************
// FUNCIONES AUXILIARES DE UTILIDAD PARA OTRAS PUBLICAS
//*************************************************************************

    m4return_t _Get_OneSlice (m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, ClSliceIndex ai_i1);
    m4return_t _GetSubString ( m4pchar_t &ao_pstring, void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize);
    m4bool_t   _GetRoundToPrecision(m4int32_t &ao_riPrecision);
    m4return_t _GetWithDates(m4date_t *ai_pDates, const m4VariantType &ai_var, m4VariantType &ao_var, m4bool_t ai_bOwner = M4_FALSE);
    m4return_t _GetDefaultWithDates( m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4VariantType &ao_rvDefault ) ;

    m4return_t _GetToVariant(void * ai_pAddress, m4VariantType &ao_var);
    m4return_t _GetToVoid (const m4VariantType &ai_var, m4uint32_t &ao_size, void *& ao_paddress);
    m4return_t _Convert (m4VariantType &ai_var);
    m4return_t _Notify (void);

    m4return_t _SetNoNotify (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus);
    m4return_t _SetVariant (const m4VariantType &ai_var, m4bool_t ai_bChangeStatus);
    m4return_t _AddSlices(m4uint32_t & ao_index, m4date_t ai_startdate, m4date_t ai_enddate);

    m4return_t _Compact_BaseValue (_ClValue_Read_Slices * ai_pClVal);        
    m4return_t _Compact_Ratios (_ClValue_Read_Slices * ai_pClVal);
    m4return_t _SameValues (_ClValue_Read_Slices * ai_pClVal, void *ai_p1, void *ai_p2, m4bool_t &ao_bresult);

};

#ifdef _M4_USING_INLINE
#include "val_ge.inl"
#endif

#endif //__VALH_GE__HPP__
