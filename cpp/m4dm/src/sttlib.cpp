//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           sttlib.cpp
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
//   
////
//==============================================================================

#include "sttlib.hpp"
#include "sttlib.inl"
#include "sttchn.hpp"
#include "channel.hpp"
#include "cldates.hpp"
#include "dmres.hpp" 
#include "clfileiod.hpp" 
#include "m4compress.hpp"


#ifdef _WINDOWS
double ClSttClock::m_dClockSelfTime = 0;
LARGE_INTEGER ClSttClock::m_iClockFrequency = ClSttClock::InitTimer();
#endif


// Versión de la serialización de estadísticas
#define	M4CH_STT_SERIALIZATION_VERSION		1

m4uint16_t	ClVMStatistics::GetSerializationVersion( void )
{
	return( M4CH_STT_SERIALIZATION_VERSION ) ;
}


ClVMStatistics::ClVMStatistics(m4bool_t	ai_bIsClient)
{
	m_iInit = M4_VM_STATISTICS_NO_INIT;

	m_vLabel.SetString(M4_LABEL_EMPTY_LABEL);

	m_pSttStack = new tpFixedStack<ClSttState*>(/*M4CL_STT_RUNTIME_STACK_SIZE, M4CL_STT_STACK_MAX_SIZE*/);

	m_pActiveState = NULL;

	m_pSttChn = new ClVMStatisticsChannel();

	m_bIsClient = ai_bIsClient;

	m_poIODriver = NULL ;
	m_pcIntermediateFile = NULL ;

}

ClVMStatistics::~ClVMStatistics(void)
{
	m_iInit = M4_VM_STATISTICS_DISABLED;

	if (m_pSttStack != NULL)
	{
		delete m_pSttStack;
		m_pSttStack=NULL;
	}

	if (m_pActiveState != NULL)
	{
		delete m_pActiveState;
		m_pActiveState = NULL;
	}

	if (m_pSttChn != NULL)
	{
		delete m_pSttChn;
	}

	_DumpIntermediateFile() ;
}

m4return_t ClVMStatistics::sttInit( ClChannelManager * ai_pChannelManager )
{
	m4return_t res = M4_SUCCESS;

	if (m_iInit != M4_VM_STATISTICS_NO_INIT)
		return M4_SUCCESS;

	m_iInit = M4_VM_STATISTICS_NO_INIT; 

	res = m_pSttChn->sttChnInit(ai_pChannelManager);

	if (M4_SUCCESS == res)
	{
		m_iInit = M4_VM_STATISTICS_ENABLED;
	}
	
	return res;
}

m4return_t ClVMStatistics::sttInit( ClChannel *ai_poSttChannel )
{
	m4return_t res = M4_SUCCESS;

	if (m_iInit != M4_VM_STATISTICS_NO_INIT)
		return M4_SUCCESS;

	m_iInit = M4_VM_STATISTICS_NO_INIT; 

	res = m_pSttChn->sttChnInit(ai_poSttChannel);

	if (M4_SUCCESS == res)
	{
		m_iInit = M4_VM_STATISTICS_ENABLED;
	}

	return res;
}


m4return_t ClVMStatistics::sttPersistToFile(m4pchar_t ai_pcFileName)
{
	m4return_t res = M4_ERROR;

	if (m_iInit & M4_VM_STATISTICS_ENABLED)
	{
		if (m_pActiveState != NULL)
		{	
			//Se lo ponemos al estado.
			ClSttState * pPersistState = new ClSttState();
			pPersistState->m_eSpecialItem = M4_VM_STT_SPECIAL_ITEM_STT_PERSIST;
			pPersistState->m_vIdItem.SetString(ai_pcFileName);
			m_pActiveState->AddChildList(pPersistState);

			return M4_SUCCESS;
		}
	}

	ClChannel * pChannel = m_pSttChn->sttGetChannel ();

	if (pChannel)
	{
		res = pChannel->PersistToFile(ai_pcFileName, M4_TRUE);
	}

	return res;
}


/*
Inicializa el estado de mandar a fichero en vez de al canal
*/
m4return_t	ClVMStatistics::sttSetIntermediateFile( m4pcchar_t ai_pccIntermediateFile )
{

	size_t	iLength = 0 ;


	// Primero se resetean los valores actuales para que el fichero se sobreescriba a la próxima vez
	// Da igual el resultado
	_DumpIntermediateFile() ;

	// Si viene un fichero se hace una copia a un nombre temporal porque se va a comprimir
	if( ai_pccIntermediateFile != NULL )
	{
		iLength = strlen( ai_pccIntermediateFile ) + 1 ;

		m_pcIntermediateFile = new m4char_t[ iLength + 4 ] ;
		CHECK_CHLOG_WARNINGF( m_pcIntermediateFile == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( ( iLength + 4 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;

		memcpy( m_pcIntermediateFile, ai_pccIntermediateFile, iLength * sizeof( m4char_t ) ) ;
		strcat( m_pcIntermediateFile, ".tmp" ) ;
	}

	return( M4_SUCCESS ) ;
}


/*
Vuelca a fichero el estado
*/
m4return_t	ClVMStatistics::_SendToIntermediateFile( void )
{

	m4return_t	iResult = M4_SUCCESS ;


	if( m_poIODriver == NULL )
	{
		CHECK_CHLOG_ERRORF( m_pcIntermediateFile == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		m_poIODriver = new ClFileIODriver( GEN_IO_MODE_WRITE, m_pcIntermediateFile ) ;
		CHECK_CHLOG_WARNINGF( m_poIODriver == NULL, M4_ERROR, M4_CH_DM_STT_NO_WRITE_INTERMEDIATE, m_pcIntermediateFile ) ;
	}


	iResult = m_poIODriver->Write( (m4uint16_t) M4CH_STT_SERIALIZATION_VERSION ) ;
	CHECK_CHLOG_WARNINGF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_WRITE_INTERMEDIATE, m_pcIntermediateFile ) ;


	iResult = Serialize( *m_poIODriver ) ;

	// Para optimizar la liberación de memoria
	if( m_pActiveState != NULL )
	{
		delete m_pActiveState;	
		m_pActiveState = NULL;
	}

	CHECK_CHLOG_WARNINGF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_WRITE_INTERMEDIATE, m_pcIntermediateFile ) ;

	return( M4_SUCCESS ) ;
}


/*
Resetea los valores del fichero, lo cierra y lo comprime
*/
m4return_t	ClVMStatistics::_DumpIntermediateFile( void )
{

	m4return_t	iResult = M4_SUCCESS ;
	size_t		iLength = 0 ;
	m4pchar_t	pccDestination = NULL ;


	if( m_poIODriver != NULL )
	{
		// Se marca el final del fichero
		iResult = m_poIODriver->Write( (m4uint16_t) -1 ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_WARNINGF( M4_CH_DM_STT_NO_WRITE_INTERMEDIATE, m_pcIntermediateFile ) ;
		}

		// Se cierra el fichero
		m_poIODriver->End( M4_TRUE ) ;


		// Se comprime el fichero
		iLength = strlen( m_pcIntermediateFile ) - 4 ;

		pccDestination = new m4char_t[ iLength + 1 ] ;

		if( pccDestination == NULL )
		{
			DUMP_CHLOG_WARNINGF( M4_CH_DM_NO_MEMORY, m4uint32_t( ( iLength + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
			iResult = M4_ERROR ;
		}
		else
		{
			memcpy( pccDestination, m_pcIntermediateFile, iLength * sizeof( m4char_t ) ) ;
			pccDestination[ iLength ] = '\0' ;

			iResult = M4CompressFile( m_pcIntermediateFile, pccDestination ) ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_WARNINGF( M4_CH_DM_STT_NO_COMPRESS_INTERMEDIATE, m_pcIntermediateFile << pccDestination ) ;
			}

			delete( pccDestination ) ;
		}

		remove( m_pcIntermediateFile ) ;

		delete( m_poIODriver ) ;
		m_poIODriver = NULL ;
	}

	if( m_pcIntermediateFile != NULL )
	{
		delete( m_pcIntermediateFile ) ;
		m_pcIntermediateFile = NULL ;
	}

	return( iResult ) ;
}

m4pchar_t ClVMStatistics::sttGetLabel(void)
{
	return m_vLabel.Data.PointerChar;
}


m4return_t ClVMStatistics::sttSetLabel(m4pchar_t ai_pclabel)
{
	if (m_iInit & M4_VM_STATISTICS_ENABLED)
	{
		if (m_pActiveState != NULL)
		{	
			//Se lo ponemos al estado.
			ClSttState * pLabelState = new ClSttState();
			pLabelState->m_eSpecialItem = M4_VM_STT_SPECIAL_ITEM_STT_SET_LABEL;
			if (ai_pclabel == NULL)
			{
				pLabelState->m_vIdItem.SetString(M4_LABEL_EMPTY_LABEL);
			}
			else
			{
				pLabelState->m_vIdItem.SetString(ai_pclabel);
			}
			m_pActiveState->AddChildList(pLabelState);

			return M4_SUCCESS;
		}
	}

	if (ai_pclabel == NULL)
	{
		m_vLabel.SetString(M4_LABEL_EMPTY_LABEL);
	}
	else
	{
		m_vLabel.SetString(ai_pclabel);
	}


	return M4_SUCCESS;
}

ClChannel * ClVMStatistics::sttGetChannel (void)
{
	return m_pSttChn->sttGetChannel();
}

m4return_t ClVMStatistics::sttAddActiveStateAll(void)
{
	ClSttState * pParentState;

	m4return_t ret = m_pSttStack->Top(pParentState);
	
	if (ret == M4_ERROR)
	{
		pParentState = NULL;
	}

	ret = sttAddStateAll(m_pActiveState, pParentState);

	// Para optimizar la liberación de memoria
	if( m_pActiveState != NULL )
	{
		delete m_pActiveState;	
		m_pActiveState = NULL;
	}

	return( ret ) ;
}

m4return_t ClVMStatistics::sttAddStateAll(ClSttState * ai_pState, ClSttState * ai_pParentState)
{
	if (ai_pState == NULL)
	{
		g_oChLog<<BeginError(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddStateAll"<<EndLog;
		return M4_ERROR;
	}

	m4return_t ret;

	//Añadimos el item al historico en orden de ejecucion
	ret = sttAddState(ai_pState, ai_pParentState);

	//Lo añadimos a times en orden inverso
	if (ai_pState->m_poChildList != NULL)
	{
		//Si tiene hijos
		itClSttStateList it;

		for (it = ai_pState->m_poChildList->begin();it != ai_pState->m_poChildList->end();++it)
		{
			if( *it != NULL )
			{
				//Añadimos el hijo
				ret = sttAddStateAll((*it), ai_pState);

				// Para optimizar la liberación de memoria
				delete( *it ) ;
				*it = NULL ;
			}
		}

		delete( ai_pState->m_poChildList ) ;
		ai_pState->m_poChildList = NULL ;
	}

	return ret;	
}


m4return_t ClVMStatistics::sttAddState(ClSttState * ai_pState, ClSttState * ai_pParentState)
{
	m4return_t ret = M4_SUCCESS;
	m4VariantType vIdT3, vIdNode, vIdItem, vIdRule; 
	m4VariantType vParentIdT3, vParentIdNode, vParentIdItem, vParentIdRule; 

	if (ai_pState == NULL)
	{
		g_oChLog<<BeginError(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddState"<<EndLog;
		return M4_ERROR;
	}

	//Items especiales de Stt
	if (ai_pState->m_eSpecialItem == M4_VM_STT_SPECIAL_ITEM_STT_SET_LABEL)
	{
		//cambiamos la label.
		m_vLabel.Set(ai_pState->m_vIdItem, M4_TRUE);
		return M4_SUCCESS;
	}

	if (ai_pState->m_eSpecialItem == M4_VM_STT_SPECIAL_ITEM_STT_PERSIST)
	{
		ClChannel * pChannel = m_pSttChn->sttGetChannel ();

		if (pChannel)
		{
			pChannel->PersistToFile(ai_pState->m_vIdItem.Data.PointerChar, M4_TRUE);
		}

		return M4_SUCCESS;
	}

	if (m_iInit & M4_VM_STATISTICS_ENABLED)
	{
		m4uint16_t iAuxInit = m_iInit;
		m_iInit = M4_VM_STATISTICS_DISABLED;
		ret = m_pSttChn->sttAddLabel(m_vLabel.Data.PointerChar);
		m_iInit = iAuxInit;

		if (M4_ERROR == ret) 
			return M4_ERROR;
	}

	if (m_iInit & M4_VM_STATISTICS_ENABLED_SQL_TIME)
	{
		if (ai_pState->m_pDBCahe)
		{	
			m4uint16_t iAuxInit = m_iInit;
			m_iInit = M4_VM_STATISTICS_DISABLED;
			ret = m_pSttChn->sttAddSQL(ai_pState->m_pDBCahe);
			m_iInit = iAuxInit;
		}
	}

	if ( (m_iInit & M4_VM_STATISTICS_ENABLED_ITEMS_TIME) || 
		 (m_iInit & M4_VM_STATISTICS_ENABLED_HISTORY) ||
		 (m_iInit & M4_VM_STATISTICS_ENABLED_VALUES) ||
		 (m_iInit & M4_VM_STATISTICS_ENABLED_READS) ) 
	{
		ai_pState->GetItemIds (vIdT3, vIdNode, vIdItem, vIdRule);

		if (ai_pParentState == NULL)
		{
			vParentIdT3.Type = vParentIdNode.Type = vParentIdItem.Type = vParentIdRule.Type = M4CL_CPP_TYPE_STRING_VAR;
			
			vParentIdT3.Data.PointerChar = "NONE";
			vParentIdNode.Data.PointerChar = "NONE";
			vParentIdItem.Data.PointerChar = "NONE";
			vParentIdRule.Data.PointerChar = "NONE";
		}
		else
		{
			ai_pParentState->GetItemIds(vParentIdT3, vParentIdNode, vParentIdItem, vParentIdRule);
		}

		if (m_iInit & M4_VM_STATISTICS_ENABLED_VALUES)
		{
			if (ai_pState->m_poValuesList)
			{
				itClSttValuesList it;
				ClSttValues * pValue;

				for (it = ai_pState->m_poValuesList->begin();it != ai_pState->m_poValuesList->end();++it)
				{
					pValue = (*it);

					m4uint16_t iAuxInit = m_iInit;
					m_iInit = M4_VM_STATISTICS_DISABLED;
					ret = m_pSttChn->sttAddValue( pValue, vIdT3.Data.PointerChar, vIdNode.Data.PointerChar, vIdItem.Data.PointerChar, vIdRule.Data.PointerChar, ai_pState->m_uiCSType );
					m_iInit = iAuxInit;
				}
			}
		}

		if (m_iInit & M4_VM_STATISTICS_ENABLED_READS)
		{
			if (ai_pState->m_poReadsList)
			{
				itClSttReadsList it;
				ClSttReads * pRead;

				for (it = ai_pState->m_poReadsList->begin();it != ai_pState->m_poReadsList->end();++it)
				{
					pRead = (*it);

					m4uint16_t iAuxInit = m_iInit;
					m_iInit = M4_VM_STATISTICS_DISABLED;
					ret = m_pSttChn->sttAddRead( pRead, vIdT3.Data.PointerChar, vIdNode.Data.PointerChar, vIdItem.Data.PointerChar, vIdRule.Data.PointerChar, ai_pState->m_uiCSType );
					m_iInit = iAuxInit;
				}
			}
		}

		if (m_iInit & M4_VM_STATISTICS_ENABLED_HISTORY)
		{
			m4uint16_t iAuxInit = m_iInit;
			m_iInit = M4_VM_STATISTICS_DISABLED;
			ret = m_pSttChn->sttAddHistory(vIdT3.Data.PointerChar, vIdNode.Data.PointerChar, vIdItem.Data.PointerChar, vIdRule.Data.PointerChar, 
								vParentIdT3.Data.PointerChar, vParentIdNode.Data.PointerChar, vParentIdItem.Data.PointerChar, vParentIdRule.Data.PointerChar,
								ai_pState->m_vDate, ai_pState->m_uiChannelCSType );
			m_iInit = iAuxInit;

			if (M4_SUCCESS == ret)
			{
				if (ai_pState->m_poExecParamsList)
				{
					itm4VariantList it;
					m4VariantType * pParam;

					for (it = ai_pState->m_poExecParamsList->begin (); it != ai_pState->m_poExecParamsList->end(); it++) 
					{
						pParam = *it;
						m4uint16_t iAuxInit = m_iInit;
						m_iInit = M4_VM_STATISTICS_DISABLED;
						m_pSttChn->sttAddHistoryParam(*pParam);
						m_iInit = iAuxInit;
					}
				}
			}
		}

		if (m_iInit & M4_VM_STATISTICS_ENABLED_ITEMS_TIME)
		{
			m4uint16_t iAuxInit = m_iInit;
			m_iInit = M4_VM_STATISTICS_DISABLED;
			ret = m_pSttChn->sttAddItem ( vIdT3.Data.PointerChar, vIdNode.Data.PointerChar, vIdItem.Data.PointerChar, ai_pState->m_vDate, ai_pState->m_uiCSType, &(ai_pState->m_oExecData) );
			m_iInit = iAuxInit;

			if (M4_SUCCESS == ret)
			{
				if (m_iInit & M4_VM_STATISTICS_ENABLED_TIMES_TIME)
				{
					m4uint16_t iAuxInit = m_iInit;
					m_iInit = M4_VM_STATISTICS_DISABLED;
					ret = m_pSttChn->sttAddTime ( ai_pState, vParentIdT3.Data.PointerChar, vParentIdNode.Data.PointerChar, vParentIdItem.Data.PointerChar, vParentIdRule.Data.PointerChar );
					m_iInit = iAuxInit;				

					if (M4_SUCCESS == ret)
					{
						if (m_iInit & M4_VM_STATISTICS_ENABLED_DBTIMES_TIME)
						{
							if (ai_pState->m_pDBCahe)
							{	
								m4uint16_t iAuxInit = m_iInit;
								m_iInit = M4_VM_STATISTICS_DISABLED;
								ret = m_pSttChn->sttInsertBDTimes(ai_pState->m_pDBCahe);
								m_iInit = iAuxInit;				
							}
						}

						if (ai_pState->m_poExecParamsList)
						{
							itm4VariantList it;
							m4VariantType * pParam;

							for (it = ai_pState->m_poExecParamsList->begin (); it != ai_pState->m_poExecParamsList->end(); it++) 
							{
								pParam = *it;
								m4uint16_t iAuxInit = m_iInit;
								m_iInit = M4_VM_STATISTICS_DISABLED;
								m_pSttChn->sttAddTimesParam(*pParam);
								m_iInit = iAuxInit;				
							}
						}
					}
				}
			}
		}
	}

	return M4_SUCCESS;
}


//Serialización
m4return_t ClVMStatistics::Serialize(ClGenericIODriver& IOD)
{
	m4double_t dSttOutputPDUSize = 0, dStartOffset = 0;

	if (M4_TRUE == m_bIsClient)
	{
		dStartOffset = IOD.GetActualOffset();
	}

	if (m_pActiveState!=NULL)
	{
		//Se serializan los Tiempos totales del ultimo estado
		if ( M4_ERROR == IOD.Write((m4bool_t)M4_TRUE) )
			return M4_ERROR;

		if (M4_TRUE == m_bIsClient)
		{
			m_pActiveState->AddFrontInterface();
		}

		if ( M4_ERROR == m_pActiveState->Serialize(IOD) )
			return M4_ERROR;

		//m_pActiveState->m_ClockLocal.Start();
	}
	else
	{
		if ( M4_ERROR == IOD.Write((m4bool_t) M4_FALSE) )
			return M4_ERROR;
	}

	m_vLabel.Serialize(IOD);

	if (M4_TRUE == m_bIsClient)
	{
		 dSttOutputPDUSize = IOD.GetActualOffset() - dStartOffset;
		 sttAddOutputPDUSize(0, dSttOutputPDUSize);
	}

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::DeSerialize(ClGenericIODriver& IOD)
{
	m4bool_t bReadState;
	m4double_t dSttInputPDUSize = 0, dStartOffset = 0;

	if (M4_TRUE == m_bIsClient)
	{
		dStartOffset = IOD.GetActualOffset();
	}

	if ( M4_ERROR == IOD.Read(bReadState) )
		return M4_ERROR;

	if (M4_TRUE == bReadState)
	{
		//Se DeSerializan los Tiempos totales en el ultimo estado
		if (m_pActiveState==NULL)
		{
			ClSttState * pSttState = new ClSttState();

			pSttState->DeSerialize(IOD);
			m_pActiveState = pSttState;
		}
		else
		{
			m_pActiveState->DeSerialize(IOD);

/*
			Descomentar para unificar FRONT_INTERFACE con su hijo!!

			if (M4_TRUE == m_bIsClient)
			{
				if (m_pActiveState->m_poChildList)
				{
					if (m_pActiveState->m_poChildList->size() == 1)
					{
						//Eliminamos el hijo del actual (REAL_MAPPED) para q no aparezcan dos items por cada
						//ejecución mapeada a server.
						//A todos los efectos, el padre y el hijo suman sus valores.
						ClSttState * pAuxState = *(m_pActiveState->m_poChildList->begin());

						m_pActiveState->m_pDBCahe = pAuxState->m_pDBCahe;
						pAuxState->m_pDBCahe = 0;

						m_pActiveState->m_poChildList = pAuxState->m_poChildList;
						pAuxState->m_poChildList = 0;

						m_pActiveState->m_poValuesList = pAuxState->m_poValuesList;
						pAuxState->m_poValuesList = 0;

						m_pActiveState->m_poExecParamsList = pAuxState->m_poExecParamsList;
						pAuxState->m_poExecParamsList = 0;

						m_pActiveState->m_oExecData.m_dSelfTotalTime += pAuxState->m_oExecData.m_dSelfTotalTime;
						m_pActiveState->m_oExecData.m_dSelfDBTime += pAuxState->m_oExecData.m_dSelfDBTime;
						m_pActiveState->m_oExecData.m_dSelfComTime += pAuxState->m_oExecData.m_dSelfComTime;
						m_pActiveState->m_oExecData.m_dSelfSttTime += pAuxState->m_oExecData.m_dSelfSttTime;
						m_pActiveState->m_oExecData.m_dSelfLDBTime += pAuxState->m_oExecData.m_dSelfLDBTime;

						delete pAuxState;						
					}
					else
					{
						//Error
						int a = 0;
					}
				}
			}*/

		}
	}

	m_vLabel.DeSerialize(IOD);

	if (M4_TRUE == m_bIsClient)
	{
		 dSttInputPDUSize = IOD.GetActualOffset() - dStartOffset;
		 sttAddInputPDUSize(0, dSttInputPDUSize);
	}

	return M4_SUCCESS;
}

