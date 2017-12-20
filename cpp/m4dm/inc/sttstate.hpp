//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                sttstate.hpp
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
//    Clases de los estados de estdísticas
////
//==============================================================================
// COMMON...

#ifndef _STTSTATE_HPP
#define _STTSTATE_HPP
#include "m4dm_dll.hpp"
#include "m4stl.hpp"

#include "m4string.hpp"
#include "m4define.hpp"
#include "m4types.hpp"
#include "cldefine.hpp"
#include "cltypes.hpp"
#include "m4var.hpp"
#include "m4mdrt.hpp"
#include "channel.hpp"
#include "sttcommon.hpp"
#include "access.hpp"
#include "item.hpp"
#include "m4vmstate_t.hpp"



//Execution params
typedef list <m4VariantType*> m4VariantList;
typedef m4VariantList::iterator itm4VariantList;


//values
class ClSttValues{
public:
	m4VariantType	m_vDate;

	m4VariantType	m_vValue;
	m4date_t		m_dSliceStart;
	m4date_t		m_dSliceEnd;
	priority_t		m_iPriority;
	m4bool_t		m_bAssigned;
	m4uint8_t		m_iSource;
	m4uint8_t		m_iM4Type;

	m4VariantType	m_vIdT3;
	m4VariantType	m_vIdNode;
	m4VariantType	m_vIdItem;
	m4VariantType	m_vIdRule;
	m4date_t		m_dNorm;
	m4int32_t		m_iLine;
	m4VariantType	m_vCallStack;

	m4uint8_t		m_iCSType;

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
};

typedef list <ClSttValues*> ClSttValuesList;
typedef ClSttValuesList::iterator itClSttValuesList;


//values
class ClSttReads{
public:
	m4VariantType	m_vDate;

	m4VariantType	m_vValue;
	m4date_t		m_dSliceStart;
	m4date_t		m_dSliceEnd;
	m4uint8_t		m_iM4Type;

	m4VariantType	m_vIdT3;
	m4VariantType	m_vIdNode;
	m4VariantType	m_vIdItem;
	m4VariantType	m_vIdRule;
	m4date_t		m_dNorm;
	m4int32_t		m_iLine;
	m4VariantType	m_vCallStack;

	m4uint8_t		m_iCSType;

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
};

typedef list <ClSttReads*> ClSttReadsList;
typedef ClSttReadsList::iterator itClSttReadsList;


//BDTimes
class ClDBTimes{
public:
	m4int32_t	m_idLConn;

	m4double_t	m_dPrepareTime;
	m4double_t	m_dExecuteTime;
	m4double_t	m_dFetchTime;
	m4uint32_t	m_iNumPrepare;
	m4uint32_t	m_iNumExecute;
	m4uint32_t	m_iNumFetch;

	m4int16_t	m_eOp;
	m4uint8_t	m_IsInternal; 
	m4pchar_t	m_pcStatement;

	ClDBTimes(void);
	ClDBTimes(m4int32_t ai_idLConn, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement);
	~ClDBTimes();

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
};

typedef list< ClDBTimes* > ClDBTimesList;


//BDCache
class ClDBCache{
public:

	ClDBTimesList	m_DBTimesList;
	ClDBTimes		*m_pDBTimesActive;

	ClDBCache();

	~ClDBCache();

	m4return_t sttAddDBTime (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement, m4double_t ai_dTime);

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
};


//ExecData
class M4_DECL_M4DM ClSttExecData
{
public:	
	//Tiempos
	m4double_t	m_dTotalTime; 
	m4double_t	m_dComTime;
	m4double_t	m_dDBTime;
	m4double_t	m_dSttTime;

	m4double_t	m_dSelfTotalTime; 
	m4double_t	m_dSelfComTime;
	m4double_t	m_dSelfDBTime;
	m4double_t	m_dSelfSttTime; 

	//De momento no exportados al canal
	m4double_t	m_dLDBTime;
	m4double_t	m_dSelfLDBTime;

	//tamaños PDUs
	m4double_t	m_dOutputPDUSize; 
	m4double_t	m_dInputPDUSize; 
	m4double_t	m_dSelfOutputPDUSize; 
	m4double_t	m_dSelfInputPDUSize; 
	m4double_t	m_dSelfSttOutputPDUSize;
	m4double_t	m_dSelfSttInputPDUSize;
	//

	//CS
	m4pchar_t	m_pcGeneralData;
	m4pchar_t	m_pcService;

	//Cache
	eCacheHit_t	m_eCacheHit;

	ClSttExecData()
	{
		Reset();
	};

	~ClSttExecData()
	{
		if (m_pcGeneralData)
			delete [] m_pcGeneralData;

		if (m_pcService)
			delete [] m_pcService;

	}

	void Reset()
	{
		m_dTotalTime = m_dComTime = m_dDBTime = m_dSttTime = 0;
		m_dSelfTotalTime = m_dSelfComTime = m_dSelfDBTime = m_dSelfSttTime = 0;
		m_dLDBTime = m_dSelfLDBTime = 0;
		m_dOutputPDUSize = m_dInputPDUSize = m_dSelfOutputPDUSize = m_dSelfInputPDUSize = 0; 
		m_dSelfSttOutputPDUSize = m_dSelfSttInputPDUSize = 0;
		m_pcGeneralData = m_pcService = 0;
		m_eCacheHit = M4_VM_STT_CACHE_NO_CACHEABLE;
	};

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);
};


//StateList
class ClSttState;
typedef list <ClSttState*> ClSttStateList;
typedef ClSttStateList::iterator itClSttStateList;


//State
class M4_DECL_M4DM ClSttState{
public:

	m4VariantType	m_vDate;

	m4uint32_t		m_hItem;
	m4VariantType	m_vIdT3;
	m4VariantType	m_vIdNode;
	m4VariantType	m_vIdItem;
	m4VariantType	m_vIdRule;
	m4date_t		m_dNorm;

	m4uint8_t		m_uiCSType;
	m4uint8_t		m_uiChannelCSType;

	//Items especiales
	eSpecialItems_t	m_eSpecialItem;

	ClSttClock		m_ClockGlobal;
	ClSttClock		m_ClockLocal;

	ClSttExecData	m_oExecData;
	ClDBCache		*m_pDBCahe;

	ClSttStateList	*m_poChildList;
	ClSttValuesList	*m_poValuesList;
	ClSttReadsList	*m_poReadsList;
	m4VariantList	*m_poExecParamsList;

	ClSttState();

	ClSttState(const m4VMState_t& ai_roState);

	ClSttState(eSpecialItems_t ai_eSpecialItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType);

	~ClSttState();

	void Reset();
	void Reset(m4uint32_t ai_hItem, eSpecialItems_t ai_eSpecialItem);

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);

	m4return_t GetItemIds (m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_IdRule);

	//estaticas para poder ser usadas sin instanciar el objeto
	static m4return_t GetItemIds (m4uint32_t ai_hItem, m4int32_t ai_iRule, ClCompiledMCR *ai_pCMCR, m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_pIdRule);
	static m4return_t GetItemIds (eSpecialItems_t ai_eItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType, m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_IdRule);

	//estaticas para poder ser usadas sin instanciar el objeto
	m4bool_t IsRealMapped();
	m4return_t AddFrontInterface();

	void DestroyChildList(void);
	void AddChildList(ClSttState * ai_pChildState);

	void DestroyValuesList(void);
	void AddValuesList(ClSttValues * ai_pValue);

	void DestroyReadsList(void);
	void AddReadsList(ClSttReads * ai_pRead);

	void DestroyExecParamsList(void);
	void AddExecParamsList(m4VariantType * ai_pParam);
};


#endif


