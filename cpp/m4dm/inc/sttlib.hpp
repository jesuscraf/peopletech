//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                sttlib.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Clase del interfaz de estadisticas
////
//==============================================================================
// COMMON...

#ifndef _STTLIB_HPP
#define _STTLIB_HPP

#include "m4dm_dll.hpp"
#include "m4string.hpp"
#include "m4define.hpp"
#include "m4types.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"
#include "fixstack.hpp"
#include "m4var.hpp"
#include "sttcommon.hpp"

class ClSttState;
class ClChannel;
class ClAccess;
class ClVMStatisticsChannel;
struct m4VMState_t;
class ClChannelManager;
class ClFileIODriver;
class ClItem;

//Masks
#define M4_VM_STATISTICS_DISABLED						0	 // No Stt
#define M4_VM_STATISTICS_ENABLED_ITEMS_TIME				0x1  //  000000001 T a nivel de Items
#define M4_VM_STATISTICS_ENABLED_TIMES_TIME				0x2	 //  000000010 T a nivel de Times 
#define M4_VM_STATISTICS_ENABLED_DBTIMES_TIME			0x4	 //  000000100 T a nivel de DbTimes 
#define M4_VM_STATISTICS_ENABLED_SQL_TIME 				0x8	 //  000001000 T en SQL
#define M4_VM_STATISTICS_ENABLED_VALUES	 				0x10 //  000010000 Values
#define M4_VM_STATISTICS_ENABLED_HISTORY 				0x20 //  000100000 History
#define M4_VM_STATISTICS_ENABLED_READS	 				0x40 //  001000000 Reads

#define M4_VM_STATISTICS_NO_INIT						0x40 //  001000000
#define M4_VM_STATISTICS_ENABLED						0x3F //  000111111
#define M4_VM_STATISTICS_ENABLED_TIME					0x9  //  000001001 T (Items o SQL)
#define M4_VM_STATISTICS_ENABLED_DB						0xC  //  000001100 T Bases datos (DbTimes o SQL)
#define M4_VM_STATISTICS_ENABLED_PARAMS					0x22 //  000100010 Params (Times o History)
#define M4_VM_STATISTICS_PAUSED			 				0x100//  100000000 Paused

class M4_DECL_M4DM ClVMStatistics{
public:
	ClVMStatistics(m4bool_t	ai_bIsClient);
	~ClVMStatistics(void);

	m4bool_t IsClient(void){return m_bIsClient;}

	m4return_t sttInit( ClChannelManager * ai_pChannelManager );
	m4return_t sttInit( ClChannel *ai_poSttChannel );

	ClChannel * sttGetChannel (void);

	static	m4uint16_t	GetSerializationVersion( void ) ;

	//---------------------------------------------------------
	//Funciones de manejo del estado interno
	m4uint16_t sttGetInitState(void);

	void sttSetInitState(m4uint16_t ai_iState);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funciones de manejo de la Label
	m4pchar_t  sttGetLabel(void);

	m4return_t sttSetLabel(m4pchar_t ai_pcLabel);
	//---------------------------------------------------------

	//Persistir
	m4return_t sttPersistToFile(m4pchar_t ai_pcFileName);

	// Guardar y recuperar al fichero intermedio
	m4return_t sttSetIntermediateFile( m4pcchar_t ai_pccIntermediateFile ) ;

	m4return_t sttAddActiveStateAll (void);

	//---------------------------------------------------------
	// Estados internos de las estadisticas. para cada estado en el que se desee 
	// tomar estadisticas debe haber un sttStateBegin y un sttStateEnd
	m4return_t sttStateBegin (const m4VMState_t& ai_roState);

	m4return_t sttStateBegin (eSpecialItems_t ai_eItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType);

	m4return_t sttStateEnd (m4uint32_t ai_hItem);

	m4return_t sttStateEndAll (void);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funciones de Tiempos con Reloj interno de BD
	m4return_t sttStartDBClock (void);

	m4return_t sttEndDBClock (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement);

	m4return_t sttEndDBClock (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funcion de Tiempos de Comm
	m4return_t sttAddComTime (m4double_t ai_dComTime);
	//---------------------------------------------------------	

	//---------------------------------------------------------
	//Funcion de Tiempos de Comm
	m4return_t sttAddLDBTime (m4double_t ai_dLDBTime);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funcion de tamanos de PDU
	m4return_t sttAddOutputPDUSize (m4double_t ai_dOutputPDUSize, m4double_t ai_dSttOutputPDUSize = 0);

	m4return_t sttAddInputPDUSize (m4double_t ai_dInputPDUSize, m4double_t ai_dSttInputPDUSize = 0);
	//---------------------------------------------------------

	//----------------------------------------------------------
	//Funciones de Client/Server
	m4return_t sttAddGeneralData (m4pcchar_t ai_pcGeneralData);

	m4return_t sttAddService (m4pcchar_t ai_pcService);
	//----------------------------------------------------------

	//----------------------------------------------------------
	//Funciones de Client/Server
	m4return_t sttAddCacheHit (m4bool_t ai_bFoundInCache);
	//----------------------------------------------------------

	//---------------------------------------------------------
	//Funciones de Values
	m4return_t sttAddValue (ClItem& ai_roItem, m4VariantType &ai_vValue, priority_t ai_iPriority, m4bool_t ai_bAssigned, m4uint8_t ai_iSource, const m4VMState_t& ai_roState, m4pcchar_t ai_pccCallStack);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funciones de Reads
	m4return_t sttAddRead (ClItem& ai_roItem, m4VariantType &ai_vValue, m4date_t ai_dStartDate, m4date_t ai_dEndDate, const m4VMState_t& ai_roState, m4pcchar_t ai_pccCallStack);
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Funciones de ExecParams
	m4return_t sttAddExecParam (m4VariantType &ai_vParam);
	m4return_t sttAddExecParam (m4double_t ai_dParam);
	m4return_t sttAddExecParam (m4pchar_t ai_pcParam);
	//---------------------------------------------------------
	
	//Funciones de Serialización
	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
	//----------------------------------------------------------------------------------------------------------------------------------------

	m4return_t GetSize(ClSize &ao_size);

private:
	/////////////////////////////////////
	//Internas
	m4return_t sttAddStateAll (ClSttState * ai_pState, ClSttState * ai_pParentState);
	m4return_t sttAddState (ClSttState * ai_pState, ClSttState * ai_pParentState);
	
	m4return_t GetActiveItemIds (m4VariantType &ao_ActiveIdT3, m4VariantType &ao_ActiveIdNode, m4VariantType &ao_ActiveIdItem, m4VariantType &ao_ActiveIdRule);	
	m4return_t GetParentItemIds (m4VariantType &ao_ParentIdT3, m4VariantType &ao_ParentIdNode, m4VariantType &ao_ParentIdItem, m4VariantType &ao_ParentIdRule);	
	m4return_t PropagateTimes (void);

	// Guardar al fichero intermedio
	m4return_t _SendToIntermediateFile( void ) ;
	m4return_t _DumpIntermediateFile( void ) ;
	/////////////////////////////////////
	
	m4uint16_t		m_iInit; 

	tpFixedStack<ClSttState*>	*m_pSttStack;
	
	ClSttState		*m_pActiveState;

	m4VariantType	m_vLabel;

	ClSttClock		m_oSttClock;

	ClVMStatisticsChannel * m_pSttChn;

	m4bool_t		m_bIsClient;

	// Para volcar a fichero en vez del al canal de estadísticas
	ClFileIODriver	*m_poIODriver ;
	m4pchar_t		m_pcIntermediateFile;
};


class M4_DECL_M4DM ClSttStateTimer{
private:
	ClVMStatistics * m_poStatistics;
public:
	ClSttStateTimer(ClVMStatistics * ai_poStatistics, m4uint16_t ai_iStatisticsLevel, eSpecialItems_t ai_eItem);

	~ClSttStateTimer();

	m4bool_t GetSttEnabled()
	{
		return (m_poStatistics ? M4_TRUE : M4_FALSE);
	}

	ClVMStatistics * GetStatistics()
	{
		return m_poStatistics;
	}

};

#endif


