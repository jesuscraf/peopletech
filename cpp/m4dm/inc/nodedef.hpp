//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             nodedef.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             09/08/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines class ClNodeDef, static info about a node
//
//
//==============================================================================

#ifndef __NODEDEF__HPP__
#define __NODEDEF__HPP__
//## begin module.includes preserve=yes

#include "m4types.hpp"
#include "handles.hpp"
#include "index.hpp"
//## end module.includes preserve=yes
//## begin module.additionalDeclarations preserve=yes
#include "m4dm_dll.hpp"
class ClCompiledMCR;
class ClRTItemIterator;
class ClNode_Channel;
class ClNode;

//## end module.additionalDeclarations
class M4_DECL_M4DM ClNodeDef
{
public:
    ClNodeDef ();
   ~ClNodeDef ();

    void Attach (ClCompiledMCR *ai_pCMCR, ClNodeIndex ai_index);

    ClHandle GetHandle (void) const
    {
        return Handle;
    }    

    m4pcchar_t Id (void) const;
    m4pcchar_t Name(void) const;
    m4pcchar_t TiId (void) const;
    m4pcchar_t M4ObjId (void) const;
    m4pcchar_t ParentId (void) const;

    m4pcchar_t GroupObjects( void ) const ;
    m4pcchar_t SentenceGroupObjects( void ) const ;
    m4pcchar_t ReadSentenceApiSql( void ) const ;

    m4uint32_t NumRows (void) const;
    m4uint32_t NumRowsDB (void) const;
    m4int32_t  NumKeepRows (void) const;

    m4uint16_t NumberOfItems (void) const;
    m4uint16_t NumberOfNodeItems (void) const;
    m4uint16_t NumberOfBlockItems (void) const;
    m4uint16_t NumberOfRegisterItems (void) const;

    m4uint16_t Index (void) const;
    m4uint16_t NodeType (void) const;
    m4uint16_t NodeOrder (void) const;

    m4uint8_t Capacity (void) const;
    m4uint8_t IsRoot (void) const;
    m4uint8_t IsRealRoot (void) const;
    m4uint8_t AutoLoad (void) const;
    m4uint8_t IsOrdered (void) const;
    m4uint8_t IsVisible(void) const;
    m4uint8_t DynFilter(void) const;
    m4uint8_t Temporality (void) const;
    m4uint8_t Completeness (void) const;
    m4uint8_t PeriodCorrected (void) const;

//------------------------------------------------------------------------
// Parametros para la carga parcial

	m4bool_t IsPartialLoad (void) const;//Es un nodo con carga parcial o no.
	m4uint32_t MaxReg (void) const; //Maximo numero de registros a ver
	m4uint32_t RegToConserve (void) const;
	//Numero de registros que se deben conservar despues de un fallo.

    //------------------------------------------------------------------------
    // Edu, hazme esto bonito, tú puedes
        m4uint16_t GetNodeNumberOfInvConnectors (void) const;
        m4uint32_t GetNodeInvConnectorHandleByPosition (m4uint16_t ai_iInvConnectedPosition) const;

        m4uint8_t  GetNodeConnectorType                ( m4uint32_t ai_iNodeConnectorHandle ) const ;
        m4uint32_t GetNodeConnectorFatherNodeHandle    ( m4uint32_t ai_iNodeConnectorHandle ) const ;

        m4uint32_t GetNodeConnectorHandleByPosition (m4uint16_t ai_iConnectedPosition) const ;
        m4uint32_t GetNodeConnectorSonNodeHandle ( m4uint32_t ai_iNodeConnectorHandle ) const ;
        m4uint8_t GetNodeConnectorExpandDelete ( m4uint32_t ai_iNodeConnectorHandle ) const ;
        m4uint8_t GetNodeConnectorExpandUndo ( m4uint32_t ai_iNodeConnectorHandle ) const ;

        m4uint16_t GetNodeNumberOfRBConnectors (void) const;
        m4uint16_t GetNodeRBFirstConnector (void) const;

        m4uint16_t GetNodeNumberOfBBConnectors (void) const;
        m4uint16_t GetNodeBBFirstConnector (void) const;

        m4uint16_t GetNodeNumberOfNRConnectors (void) const;
        m4uint16_t GetNodeNRFirstConnector (void) const;

        m4uint16_t GetNodeNumberOfAutoConnectors (void) const;

    //------------------------------------------------------------------------

    // para los ordenes de ejecución . jesus

    m4uint16_t  GetNumberOfGroups( void ) const ;
    m4pcchar_t  GetGroupIdByPosition( m4uint16_t ai_iGroupPosition ) const ;
    m4uint16_t  GetGroupNumberOfItems( m4uint16_t ai_iGroupPosition ) const ;
    m4pcchar_t  GetGroupItemIdByPosition( m4uint16_t ai_iGroupPosition, m4uint16_t ai_iItemPosition ) const ;

    //------------------------------------------------------------------------


    m4uint16_t GetRBChildPos (ClHandle ai_handle);
    m4uint16_t GetBBChildPos (ClHandle ai_hnode);

    ClCompiledMCR *GetpCMCR (void) const
    {
        return m_pCMCR;
    }

    //por compatibilidad cutre hacia atrás, eliminar
    ClCompiledMCR *_GetpCMCR (void) const
    {
        return m_pCMCR;
    }

    m4uint16_t GetNodeItemType (m4uint16_t ai_iItemPosition) const;
    m4uint16_t GetNodeItemScope (m4uint16_t ai_iItemPosition) const;
    m4uint8_t  GetNodeItemInternalType (m4uint16_t ai_iItemPosition) const;

    m4uint32_t GetNodeItemHandle (m4uint16_t ai_iItemPosition) const;
    m4uint32_t GetNodeItemHandle (m4pcchar_t ai_pccItemId) const;

    m4int16_t  GetItemIndex (m4uint32_t ai_iItemHandle) const;

    m4pcchar_t  StartDateItem (void) const;
    m4pcchar_t  EndDateItem (void) const;
    m4pcchar_t  StartCorDateItem( void ) const;
    m4pcchar_t  EndCorDateItem( void ) const;
    m4pcchar_t  VirtualFlagItem( void ) const;
    m4pcchar_t  FilterStartDateItem( void ) const ;
    m4pcchar_t  FilterEndDateItem( void ) const ;
    m4pcchar_t  FilterStartCorDateItem( void ) const ;
    m4pcchar_t  FilterEndCorDateItem( void ) const ;
    m4pcchar_t  CurrencyTypeItem( void ) const ;
    m4pcchar_t  CurrencyExchDateItem( void ) const ;
    m4pcchar_t  ChangeReasonItem( void ) const ;
    m4pcchar_t  DmdComponentItem( void ) const ;
    m4pcchar_t  DmdValueItem( void ) const ;
    m4pcchar_t  LoadSQLItem( void ) const ;


	// All new Carril features...
	m4uint16_t  GetNumberOfPlugs(void) const;
	m4pcchar_t  GetPlugFatherChanneIdByPosition(m4uint16_t ai_iPlugPosition) const;
	m4pcchar_t  GetPlugFatherNodeIdByPosition(m4uint16_t ai_iPlugPosition) const;
	m4uint8_t   GetPlugTypeByPosition(m4uint16_t ai_iPlugPosition) const;
	m4uint8_t   GetPlugIsMultiselectionByPosition(m4uint16_t ai_iPlugPosition) const;
	m4uint8_t   GetPlugIsFilterByPosition(m4uint16_t ai_iPlugPosition) const;
    m4uint8_t	PlugRelationTypeId( m4uint16_t ai_iPlugPosition ) const ;
    m4uint8_t	PlugIsApplicable( m4uint16_t ai_iPlugPosition ) const ;

	m4uint16_t  PlugNumberOfItems( m4uint16_t ai_iPlugPosition ) const;
	m4pcchar_t  PlugFatherItem(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const;
	m4pcchar_t  PlugSonItem(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const;
	m4uint8_t   PlugItemRelation(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const;

    m4uint32_t  GetStartDateItem (void) const;
    m4uint32_t  GetEndDateItem (void) const;

    m4uint32_t  GetChangeReasonItemIndex (void) const;
    m4uint32_t  GetFilterStartDateItemIndex (void) const;
    m4uint32_t  GetFilterEndDateItemIndex  (void) const;
    m4uint32_t  GetDmdComponentItemIndex  (void) const;

	m4pcchar_t GetNodeReadSentenceId(void) const;
	m4pcchar_t GetNodeWriteSentenceId(void) const;
	m4pcchar_t GetNodeReadObjectId(void) const;
	m4pcchar_t GetNodeWriteObjectId(void) const;

	m4uint16_t NumberOfConnectors(void) const;
	m4uint16_t NumberOfRBConnectors(void) const;
	m4uint16_t NumberOfBBConnectors(void) const;
	m4uint16_t NumberOfNRConnectors(void) const;
	m4uint16_t RBFirstConnector(void) const;
	m4uint16_t BBFirstConnector(void) const;
	m4uint16_t NRFirstConnector(void) const;

	m4uint32_t GetConnectorHandleByPosition(m4uint16_t ai_iConnectedPosition) const;


// mas cosas

    m4pcchar_t  PriorityItem( void ) const ;
    m4pcchar_t  ImputeDateItem( void ) const ;
    m4pcchar_t  PayDateItem( void ) const ;
    m4pcchar_t  PayTypeItem ( void ) const ;
    m4pcchar_t  PayFrequencyItem( void ) const ;
    m4pcchar_t  ImputePayTypeItem ( void ) const ;
    m4pcchar_t  ImputePayFrequencyItem( void ) const ;
    m4pcchar_t  ClosingDateItem( void ) const ;
    m4pcchar_t  SysParamsItem( void ) const ;
    m4pcchar_t  BDLChecksItem( void ) const ;
    m4pcchar_t  RAMOrderByItem( void ) const ;
    m4pcchar_t  OrganizationIdItem( void ) const ;

    m4uint16_t  NumberOfSysSentences( void ) const ;
    m4pcchar_t  SysSentenceNode( m4uint16_t ai_iPosition ) const ;
    m4pcchar_t  SysSentenceItem( m4uint16_t ai_iPosition ) const ;
    m4uint32_t  DefaultIndex( void ) const ;

    m4uint8_t   IdAutoFilter( void ) const ;
    m4uint8_t	CsType( void ) const ;
    m4uint8_t	SyncType( void ) const ;

    m4uint16_t  NumberOfIndexes( void ) const ;
    m4uint32_t  IndexId( m4uint16_t ai_iIndexPosition ) const ;
    m4uint16_t  IndexNumberOfItems( m4uint16_t ai_iIndexPosition ) const ;
    m4pcchar_t  IndexItemId( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const ;
    m4uint8_t   IndexItemFunction( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const ;
    m4uint8_t   IndexItemWay( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const ;

    m4uint16_t  NumberOfAlias( void ) const ;
    m4pcchar_t  AliasId( m4uint16_t ai_iAliasPosition ) const ;
    m4pcchar_t  AliasNodeId( m4uint16_t ai_iAliasPosition ) const ;

    m4uint16_t  NumberOfT3Alias( void ) const ;
    m4pcchar_t  T3AliasId( m4uint16_t ai_iAliasPosition ) const ;
    m4pcchar_t  T3AliasInstance( m4uint16_t ai_iAliasPosition ) const ;

    m4uint16_t  NumberOfDmds( void ) const ;
    m4pcchar_t  DmdId( m4uint16_t ai_iDmdPosition ) const ;
    m4pcchar_t  DmdComponentId( m4uint16_t ai_iDmdPosition ) const ;
    m4pcchar_t  DmdFieldId( m4uint16_t ai_iDmdPosition ) const ;

    m4uint16_t  NumberOfQBFFilters( void ) const ;
    m4pcchar_t  QBFFilterId( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  QBFFilterT3Id( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  QBFFilterNodeId( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  QBFFilterSysSentenceItem( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  QBFFilterSysParamItem( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	QBFFilterBehaviourType( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	QBFFilterIsOwnNode( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	QBFFilterIsApplicable( m4uint16_t ai_iFilterPosition ) const ;

    m4uint16_t  NumberOfFindFilters( void ) const ;
    m4pcchar_t  FindFilterId( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  FindFilterT3Id( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  FindFilterNodeId( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  FindFilterSysSentenceItem( m4uint16_t ai_iFilterPosition ) const ;
    m4pcchar_t  FindFilterSysParamItem( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	FindFilterBehaviourType( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	FindFilterIsOwnNode( m4uint16_t ai_iFilterPosition ) const ;
    m4uint8_t	FindFilterIsApplicable( m4uint16_t ai_iFilterPosition ) const ;

// security

    m4uint8_t   SecRead( void ) const ;
    m4uint8_t   SecWrite( void ) const ;
    m4uint8_t   SecDelete( void ) const ;
    m4uint8_t   SecUpdate( void ) const ;
    m4uint8_t   SecExecute( void ) const ;
    m4uint8_t   SecDynFilter( void ) const ;
    m4pcchar_t	SecFilter( void ) const ;

    
	m4uint8_t	IsExternal (void) const;
	m4uint8_t	IsQBFFilter (void) const;
	m4uint8_t	AffectsDB (void) const;
	m4uint8_t	DBReload (void) const;
	m4uint8_t	TiInheritLevel (void) const;
	m4uint8_t	IsOverWritten (void) const;
	m4uint32_t	OleDispId (void) const;
	m4pcchar_t	OleNodeInterId (void) const;
	m4uint32_t	MaxNumBlocks (void) const;
	m4uint32_t	MaxNumRecords (void) const;


    m4bool_t IsValid (void) const
    {
        return (Handle)? M4_TRUE : M4_FALSE;
    }

    //añadimos información sobre los items conectados:
    //ClColItemDef_Kernel ColItemDef_Kernel;
	m4return_t GetSize(ClSize &ao_size);

private:
    ClCompiledMCR *m_pCMCR;

    ClHandle Handle;

friend class ClNode_Channel;  // contiene un ClNodeDef y (logicamente) llama al _Init
};

// User actions auditory.
m4return_t GetItemIndex( ClCompiledMCR *ai_poCMCR, m4uint32_t ai_iNode, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClItemIndex &ao_iItemIndex ) ;


#endif // __NODEDEF__HPP__ 
