//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clrgstrs.h
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             30/04/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class StaticFilter
//
//==============================================================================

//## begin module.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module.cm

//## begin module.cp preserve=no
//## end module.cp

//## Module: ClRegisterStaFilter; Pseudo Package specification
//## Subsystem: inc
//## Source file: d:\myprojects\working\channel\chlib\version\inc\ClRgstrS.h

#ifndef ClRgstrS_h
#define ClRgstrS_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// regi_smp
#include "regi_smp.hpp"
#include "mmindex.hpp"
#include "index.hpp"
#include "mmarray.hpp"
//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations

class _ClFilterUtility;
class ClBSearchIndex;
class ClNode_Kernel;
class ClChannel;
class ClAccessProvider;

class ClBlockReference;

template <class T>
class ClComponentMap;
//## Class: ClRegisterStaFilter
//## Category: inc
//## Subsystem: inc
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRegisterStaFilter : public _ClRegisterFilter_Generic  //## Inherits: <unnamed>
{
  //## begin ClRegisterStaFilter.initialDeclarations preserve=yes
  //## end ClRegisterStaFilter.initialDeclarations

public:
    //## Constructors (generated)
    //LN4
    ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4pchar_t ai_pName = 0);
    ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4int32_t ai_pHandleForm, m4pchar_t ai_pName = 0);
    //Copia
    ClRegisterStaFilter (_ClFilterUtility * ai_pClFU, m4pchar_t ai_pName = 0);
    //Operation
    ClRegisterStaFilter (m4uint8_t ai_OpType, m4pchar_t ai_pName = 0);
    //Sort
    ClRegisterStaFilter (_ClRegister_Base * ai_pClRegister, ClBSearchIndex * ai_pBSearchIndex, m4pchar_t ai_pName = 0);
    ClRegisterStaFilter (_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders, m4pchar_t ai_pName = 0);
    //Fast
    ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4pchar_t ai_pName = 0);
    ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4uint32_t ai_iOperation, m4pchar_t ai_pName = 0);

   ~ClRegisterStaFilter ();
    // Additional Public Declarations
    m4return_t InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs);

    m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    void Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_ReleaseAll_Before (ClAccessProvider *ai_pap);

    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

	m4return_t UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t UpdateThisFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    //Compruebo que si un registro es visible o no:
    //si: lo dejo -> true
    //no: lo elimino -> false
    //no esta -> false
    //## begin ClRegisterStaFilter.public preserve=yes
    //## end ClRegisterStaFilter.public

    _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal = M4_FALSE);

    _ClFilterUtility * operator -> (void) const
    {
        return m_pClFilterUtility;
    }

    Type_e Type (void) const { return _ClRegisterFilter::StaticFilter; }
	
	m4return_t GetSize(ClSize &ao_size);

    //Le paso el register a los filtros por si alguno lo necesita
    void AttachRegister(_ClRegister_Base * ai_pReg);

  protected:
    // Data Members for Associations

    // Additional Private Declarations
      //## begin ClRegisterDynFilter::m_pClSt.attr preserve=no  protected: _AClComponentStore * {V} 0
      ClComponentMap <ClMMArray> *m_pClCompMap;
      //## end ClRegisterDynFilter::m_pClSt.attr

    // Additional Implementation Declarations
      //## begin ClRegisterDynFilter.implementation preserve=yes
      //## end ClRegisterDynFilter.implementation

private:
    void _init (void);
};



//## begin module.epilog preserve=yes
//## end module.epilog


#endif
