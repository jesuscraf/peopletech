//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             register.hpp
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
#include "m4types.hpp"
//#include "m4var.hpp"
#include "handles.hpp"
//## end module.includes preserve=yes

#ifndef __ITEMDEF__HPP__
#define __ITEMDEF__HPP__
#include "m4dm_dll.hpp"
//## begin module.additionalDeclarations preserve=yes
class ClCompiledMCR;
class ClColItemDef;
class ClConnectItemInfo;
class _AClItemProxy;
class ClRTItemIterator;
class ClNoCallableItem;
class m4VariantType;
class ClConnectItemInfo;
class ClLongString ;
//## end module.additionalDeclarations

///////////////////////////////////////////////////////////////////////////////
//
// ClItemDef
//

class M4_DECL_M4DM ClItemDef
{
public:
    ClItemDef () :
        m_Handle (0),
        m_pCMCR (0)
    {
    }

    void Attach (ClCompiledMCR *ai_pCMCR);
    void Attach (const ClItemDef &);
    

    m4pcchar_t Id (void) const;
    m4pcchar_t TiId (void) const;
    m4int32_t  Synonym (void) const;
    m4pcchar_t Name (void) const;
    m4pcchar_t Dmd (void) const;
    m4pcchar_t DmdComponent (void) const;
    m4pcchar_t DmdCrossTab (void) const;
    m4pcchar_t DmdField (void) const;
    m4int32_t  Precision (void) const;
    m4int32_t  MaxSize (void) const;
    m4int32_t  RealSize (void) const;
    m4pcchar_t ReadObjectAlias (void) const;
    m4pcchar_t WriteObjectAlias (void) const;
    m4pcchar_t ReadFieldId (void) const;
    m4pcchar_t WriteFieldId (void) const;
    m4pcchar_t ReadObjectId (void) const;
    m4pcchar_t WriteObjectId (void) const;

    m4int16_t Index (void) const;

    m4uint8_t Type (void) const;
    m4uint8_t TheoricScope (void) const;
    m4uint8_t M4Type (void) const;
    m4uint8_t CppType (void) const;
    m4uint8_t Scale (void) const;
    m4uint8_t SliceBhrv (void) const;
    m4uint8_t SliceSplit (void) const;
    m4uint8_t IdTotalize (void) const;
    m4uint8_t SliceTotalize (void) const;
    m4uint8_t TransactionMode (void) const;
    m4uint8_t MethodLevel (void) const;
    m4uint8_t IsVariableLength (void) const;
    m4uint8_t SentTotType (void) const;
    m4uint8_t Scope (void) const;
    m4uint8_t InternalType( void ) const ;
    m4uint8_t ConvertFunction( void ) const ;
    m4uint8_t IsInherited( void ) const ;
    m4uint8_t CsType( void ) const ;
    m4uint8_t SyncType( void ) const ;
    m4uint8_t IsParameter( void ) const ;
    m4uint8_t CheckNotNull( void ) const ;
    m4uint8_t ClientNotNull( void ) const ;
    m4uint8_t NotNull( void ) const ;
    m4uint8_t AffectsDB( void ) const ;
    m4uint8_t GenerateSlices( void ) const ;
    m4uint8_t IsPK( void ) const ;
    m4uint8_t IsNodalItem( void ) const ;
    m4uint8_t IsEventItem( void ) const ;
	


    m4uint16_t NumberOfConnectedItems (void) const;
    m4uint32_t ConnectorHandleByPosition (m4uint16_t ai_pos) const;
	m4pcchar_t ConnectorFatherNodeByPosition( m4uint16_t ai_iPosition ) const ;
	m4pcchar_t ConnectorFatherItemByPosition( m4uint16_t ai_iPosition ) const ;
	m4pcchar_t ConnectorSonNodeByPosition( m4uint16_t ai_iPosition ) const ;
	m4pcchar_t ConnectorSonItemByPosition( m4uint16_t ai_iPosition ) const ;
	m4uint8_t  ConnectorPrecedenceByPosition( m4uint16_t ai_iPosition ) const ;
	m4uint8_t  ConnectorSpinByPosition( m4uint16_t ai_iPosition ) const ;
	m4uint8_t  ConnectorRelationByPosition( m4uint16_t ai_iPosition ) const ;
	m4uint8_t  ConnectorContextByPosition( m4uint16_t ai_iPosition ) const ;
	m4uint8_t  ConnectorCsTypeByPosition( m4uint16_t ai_iPosition ) const ;

    m4uint16_t NumberOfArguments (void) const;
	m4uint16_t NumberOfReferedArguments (void) const;
	m4uint32_t NodeHandle (void) const;

    // Edu, hazme esto bonito, tú puedes
        m4uint32_t ConnectorSonItemHandle (m4uint32_t ai_iItemConnectorHandle) const;

    m4uint8_t VariableArguments (void) const;

    m4int16_t Order(void) const;
    m4uint8_t IsVisible(void) const;
    m4uint8_t ModifyData(void) const;


    m4pcchar_t ArgumentIdByPosition( m4uint16_t ai_pos ) const ;
    m4pcchar_t ArgumentNameByPosition( m4uint16_t ai_pos ) const ;
    m4uint8_t  ArgumentTypeByPosition( m4uint16_t ai_pos ) const ;
    m4uint8_t  ArgumentM4TypeByPosition( m4uint16_t ai_pos ) const ;
    m4uint16_t ArgumentPrecisionByPosition( m4uint16_t ai_pos ) const ;
    m4int16_t  ArgumentPositionById( m4pcchar_t ai_pccArgumentId ) const ;

    m4uint16_t NumberOfConditionants (void) const;
    m4int16_t  ConditionantIndexByPosition( m4uint16_t ai_pos ) const ;
    m4int16_t  ConditionantNodeIndexByPosition( m4uint16_t ai_pos ) const ;
    m4pcchar_t ConditionantIdByPosition( m4uint16_t ai_pos ) const ;
    m4pcchar_t ConditionantNodeIdByPosition( m4uint16_t ai_pos ) const ;

    m4uint16_t NumberOfDependents (void) const;
    m4int16_t  DependentIndexByPosition( m4uint16_t ai_pos ) const ;
    m4int16_t  DependentNodeIndexByPosition( m4uint16_t ai_pos ) const ;
    m4pcchar_t DependentIdByPosition( m4uint16_t ai_pos ) const ;
    m4pcchar_t DependentNodeIdByPosition( m4uint16_t ai_pos ) const ;

    m4pcchar_t  SliceItem( void ) const ;
    m4pcchar_t  AuxiliarItem( void ) const ;
    m4pcchar_t  AuxiliarItemProrat( void ) const ;

    m4uint16_t  SliceItemIndex ( void ) const ;
    m4uint16_t  AuxiliarItemIndex ( void ) const ;
    m4uint16_t  AuxiliarItemProratIndex ( void ) const ;


    m4bool_t	IsConnected( void ) const ;
    m4pcchar_t  FatherItem( void ) const ;
    m4pcchar_t  FatherItemNode( void ) const ;

//funciones utilizadas por el Value:
    
	m4return_t Precision (m4int32_t &ao_precc) const;
    m4return_t MaxSize (m4int32_t &ao_maxsize) const;
    m4return_t RealSize (m4int32_t &ao_realsize) const;
	m4return_t GetItemDefaultValue ( m4VariantType &ao_rvValue ) const ;
    m4return_t SliceBhrv (m4uint8_t &ao_slibhrv) const;
    m4return_t CppType (m4uint8_t &ao_cpp) const;
    m4return_t SliceTotalize (m4uint8_t &ao_slitot) const;

// mas cosas

    m4uint16_t  NumberOfAssigned( void ) const;
    m4int16_t   AssignedIndex( m4uint16_t ai_AssignedPosition ) const ;
    m4int16_t   AssignedNodeIndex( m4uint16_t ai_AssignedPosition ) const ;
    m4pcchar_t  AssignedId( m4uint16_t ai_AssignedPosition ) const ;
    m4pcchar_t  AssignedNodeId( m4uint16_t ai_AssignedPosition ) const ;

	m4uint16_t	NumberOfAssignedDependents	(void) const;
    m4int16_t	AssignedDependentIndex		( m4uint16_t ai_pos ) const ;
    m4int16_t	AssignedDependentNodeIndex	( m4uint16_t ai_pos ) const ;
    m4pcchar_t	AssignedDependentId			( m4uint16_t ai_pos ) const ;
    m4pcchar_t	AssignedDependentNodeId		( m4uint16_t ai_pos ) const ;

    m4uint16_t  NumberOfExecuted( void ) const;
    m4int16_t   ExecutedIndex( m4uint16_t ai_ExecutedPosition ) const ;
    m4int16_t   ExecutedNodeIndex( m4uint16_t ai_ExecutedPosition ) const ;
    m4pcchar_t  ExecutedId( m4uint16_t ai_ExecutedPosition ) const ;
    m4pcchar_t  ExecutedNodeId( m4uint16_t ai_ExecutedPosition ) const ;

	m4uint16_t	NumberOfExecutedDependents	(void) const;
    m4int16_t	ExecutedDependentIndex		( m4uint16_t ai_pos ) const ;
    m4int16_t	ExecutedDependentNodeIndex	( m4uint16_t ai_pos ) const ;
    m4pcchar_t	ExecutedDependentId			( m4uint16_t ai_pos ) const ;
    m4pcchar_t	ExecutedDependentNodeId		( m4uint16_t ai_pos ) const ;


// Para el tratamiento de fórmulas y código

    m4uint16_t  NumberOfFormIds( void ) const ;
    m4pcchar_t  FormId( m4uint16_t ai_iFormIdPosition ) const ;

	m4int16_t	MaxCodes( void ) const ;
	m4return_t	GetNextCode( m4int16_t &ai_rioNext ) const ;

	m4pcchar_t	CodeRuleId				( m4int16_t ai_iPosition ) const ;
	m4date_t	CodeStartDate			( m4int16_t ai_iPosition ) const ;
	m4date_t	CodeEndDate				( m4int16_t ai_iPosition ) const ;
	m4pcchar_t	CodeRuleTiId			( m4int16_t ai_iPosition ) const ;
	m4uint32_t	CodeNumberOfVariales	( m4int16_t ai_iPosition ) const ;
	m4uint32_t	CodeNumberOfInstructions( m4int16_t ai_iPosition ) const ;
	m4uint32_t	CodeSize				( m4int16_t ai_iPosition ) const ;
	m4uint8_t	CodeType				( m4int16_t ai_iPosition ) const ;
	m4uint8_t	CodeMustAssign			( m4int16_t ai_iPosition ) const ;
	m4uint8_t	CodePriority			( m4int16_t ai_iPosition ) const ;
	m4uint8_t	CodeAutoGenerated		( m4int16_t ai_iPosition ) const ;

	m4return_t	CodeSource				( m4int16_t ai_iPosition, ClLongString &ao_roString ) const ;



// security

    m4uint8_t   SecRead( void ) const ;
    m4uint8_t   SecWrite( void ) const ;
    m4uint8_t   SecDelete( void ) const ;
    m4uint8_t   SecUpdate( void ) const ;
    m4uint8_t   SecExecute( void ) const ;
    m4uint8_t   SecContinue( void ) const ;

// ole

    m4uint8_t   IsPublic( void ) const ;    
    
    ClHandle GetHandle (void) const
    {
        return m_Handle;
    }

    ClCompiledMCR *GetpCMCR (void) const
    {
        return m_pCMCR;
    }

    //m4uint16_t GetPos (void) const;
    //void SetpItemProxy (_AClItemProxy *);
    //ClConnectItemInfo *GetpConnectItemInfo (void) const;

    m4bool_t IsValid (void) const
    {
        return (m_pCMCR != 0 && m_Handle != 0)? M4_TRUE : M4_FALSE;
    }

    void SetEOF (void)
    {
        m_Handle = 0;
        //m_pClItemProxy = 0;
    }

    //asignación llamada por ClNoCallableItem
    ClItemDef& operator= (const ClItemDef &ai_item)
    {
        m_pCMCR = ai_item.m_pCMCR;
        m_Handle = ai_item.m_Handle;
        //m_pClConnectItemInfo = ai_item.m_pClConnectItemInfo;
        //m_pClItemProxy = ai_item.m_pClItemProxy;

        return *this;
    }

    /*
    _AClItemProxy *GetpItemProxy (void) const
    {
        return m_pClItemProxy;
    }
    */

    void SetHandle (ClHandle ai_h)
    {
        m_Handle = ai_h;
    }

	m4return_t GetSize(ClSize &ao_size);

private:
    ClCompiledMCR *m_pCMCR;
    //_AClItemProxy *m_pClItemProxy;
    

    //void _SetItemProxy (_AClItemProxy *);
    //void _SetEOF (void);

    //Digamos que tengo el handle cacheado:
    ClHandle m_Handle;

    //añadimos información sobre los items conectados:
    //ClConnectItemInfo *m_pClConnectItemInfo;

//friend class ClRTItemIterator;    //el papel de iterador #1
//friend class ClColItemDef;        //el papel de iterador #2
//friend class ClNoCallableItem;
};

#endif //__ITEMDEF__HPP__
