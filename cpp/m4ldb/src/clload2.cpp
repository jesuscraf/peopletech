#include "cldates.hpp"
#include "cllgadap.hpp"
#include "tfres.hpp"
#include "channel.hpp"
#include "medvarin.hpp"
#include "m4mdrt.hpp"
#include "clldb.hpp"
#include "clload.hpp"
#include "defmain.hpp"
#include "cldinarr.hpp"
#include "cltblmem.hpp"
#include "cllconn.hpp"
#include "clstatem.hpp"
#include "ldb_log.hpp"
#include "cursor.hpp"
#include "regpipe.hpp"
#include "px_block.hpp"
#include "access.hpp"
#include "blockref.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "pl_data.hpp"
#include "m4var.hpp"
#include "file_manager.hpp"
#include "m4objglb.hpp"
#include "m4lgadapdef.hpp"
#include "ldbglobals.hpp"
#include "m4unicode.hpp"



m4bool_t ClLoad::NewCursor (m4int32_t ai_iFirstData)
{
	StPartialLoad *pPL = m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ().FindPartialLoad (GetCurrentCursor (), m_pGlobVars->GetpLConn ()->GetIdLConn ());

	if (pPL) {
		void *pBlock = (m_pGlobVars->GetpAccessRecordSet ()) ? (void *) m_pGlobVars->GetpAccessRecordSet ()->GetCurrentBlock () : NULL;
		
		if ((pPL->GetOpenCursor ()) && 
			(pBlock == pPL->GetBlock ()) && 
			(ai_iFirstData >= pPL->GetReadRegister ()) && 
			(GetCurrentCursor ()->IsCursorActivated ())) {
			// No es necesario reejecutar el cursor.
			return (M4_FALSE);
		}

		if (pPL->GetOpenCursor ()) {
			Close ();
		}
		if (pBlock != pPL->GetBlock()){
			ClChannel *pChannel = m_pGlobVars->GetpAccessRecordSet ()->GetpAccess()->GetpChannel();
			pPL->SetChannel (pChannel);
//			pChannel->AddCursor (GetCurrentCursor ());
			pChannel->m_pDestroy_Notifier->Attach(&(m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ()));
		}
		pPL->SetOpenCursor (M4_TRUE);
		pPL->SetReadRegister (0);
		pPL->SetBlock (pBlock);
	}
	
	return (M4_TRUE);
}


m4return_t ClLoad::Execute (m4VariantType *ai_pParam, m4int32_t ai_numParam, ClRegister *ai_pReg, m4int32_t ai_iFirstData, m4uint32_t ai_iTimeOut)
{
	m4return_t result;
	m4int32_t indParam = 0,indVParam;
	m4double_t dValue = 0;
	m4int32_t hItem = 0;
	m4int32_t indExtPar = 0;

	if ((IsPartialLoad()) && 
		(!NewCursor (ai_iFirstData))) {
		return (M4_SUCCESS);
	}
			
	m4bool_t bRetry = M4_FALSE;
	
	do {
		m4int32_t numParam = m_oVParam.size ();

		for (indVParam=0; indVParam<numParam; indVParam++) {
			
			indParam = m_oVParam[indVParam];

			if (!m_pParam->GetInfoIsOwn (indParam)) {
				continue;
			}

			m4VariantType vAux;
			m4VariantType *pvAux = &vAux;

			result = M4_SUCCESS;

			m4int8_t m4Type = m_pParam->GetInfoM4Type (indParam);
			hItem = m_pParam->GetItemHandle (indParam);
			eParamType_t eParamType = m_pParam->GetInfoParamType (indParam);

			switch (eParamType) {

				case M4LDB_PAR_ITEM:
					if (!hItem) {
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClLoad::Execute" << "Null Handle in Param " << LogCat << indParam);
						return (M4_ERROR);
					}

					if (!ai_pReg) {
						result = GetParamValue (hItem, pvAux);
					}
					else {
						if ((ai_pReg->Index == M4LDB_NULL_INDEX) && 
							(m_pGlobVars->GetItemScope (hItem) == M4CL_ITEM_SCOPE_REGISTER)) {
							DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
							return (M4_ERROR);
						}

						ai_pReg->Item[(ClHandle) hItem].Value.Get (vAux);
					}
					break;

				case M4LDB_PAR_SESSION:
					// Lo intento leer del canal.
					if (_ReadChannelPropertyOrSessionItem(hItem,vAux)!=M4_SUCCESS){
						return (M4_ERROR);
					}

					break;

				case M4LDB_PAR_EXTERN:
					if (indExtPar >= ai_numParam) {
						DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ARGUMENT_NUMBER);
						return (M4_ERROR);
					}
					// El indice del parametro viene dado por el handle del Item.
					pvAux = &ai_pParam[hItem-1];

					indExtPar++;
					break;

				case M4LDB_PAR_AUTO_FILTER:
					{
						switch (hItem)
						{
						case M4CL_INTERNAL_TYPE_FILTER_START_DATE:
							hItem = m_pGlobVars->GetNodeFilterStartDateItem ();

							if (hItem) {
								result = GetParamValue (hItem, pvAux);
							}

							if (vAux.GetType () == M4CL_CPP_TYPE_NULL) {
								m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Data_StartDate.Get (vAux);
							}

							/* Bug 0275318
							Si es sólo fecha se quitan las horas
							*/
							if (vAux.Type == M4CL_CPP_TYPE_DATE && m4Type == M4CL_M4_TYPE_DATE)
							{
								vAux.Data.DoubleData = (int)vAux.Data.DoubleData;
							}
							break;

						case M4CL_INTERNAL_TYPE_FILTER_END_DATE:
							hItem = m_pGlobVars->GetNodeFilterEndDateItem ();

							if (hItem) {
								result = GetParamValue (hItem, pvAux);
							}

							if (vAux.GetType () == M4CL_CPP_TYPE_NULL) {
								m_pGlobVars->GetpAccessRecordSet ()->GetpAccess()->GetpChannel()->Data_EndDate.Get (vAux);
							}

							/* Bug 0275318
							Si es sólo fecha se quitan las horas
							*/
							if (vAux.Type == M4CL_CPP_TYPE_DATE && m4Type == M4CL_M4_TYPE_DATE)
							{
								vAux.Data.DoubleData = (int)vAux.Data.DoubleData;
							}
							break;

						case M4CL_INTERNAL_TYPE_FILTER_COR_START_DATE:
							hItem = m_pGlobVars->GetNodeFilterStartCorDateItem ();

							if (hItem) {
								result = GetParamValue (hItem, pvAux);
							}

							if (vAux.GetType () == M4CL_CPP_TYPE_NULL) {
								m_pGlobVars->GetpAccessRecordSet ()->GetpAccess()->GetpChannel()->Data_CorrectionDate.Get (vAux);
							}

							/* Bug 0275318
							Si es sólo fecha se quitan las horas
							*/
							if (vAux.Type == M4CL_CPP_TYPE_DATE && m4Type == M4CL_M4_TYPE_DATE)
							{
								vAux.Data.DoubleData = (int)vAux.Data.DoubleData;
							}
							break;

						case M4CL_INTERNAL_TYPE_FILTER_COR_END_DATE:
							hItem = m_pGlobVars->GetNodeFilterEndCorDateItem ();

							if (hItem) {
								result = GetParamValue (hItem, pvAux);
							}

							if (vAux.GetType () == M4CL_CPP_TYPE_NULL) {
								m_pGlobVars->GetpAccessRecordSet ()->GetpAccess()->GetpChannel()->Data_CorrectionDate.Get (vAux);
							}

							/* Bug 0275318
							Si es sólo fecha se quitan las horas
							*/
							if (vAux.Type == M4CL_CPP_TYPE_DATE && m4Type == M4CL_M4_TYPE_DATE)
							{
								vAux.Data.DoubleData = (int)vAux.Data.DoubleData;
							}
							break;

						case M4CL_INTERNAL_TYPE_ID_ORGANIZATION:
							{
								ClChannel *pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

								if (pChannel->Organization.Get(vAux)!=M4_SUCCESS || vAux.Type!=M4CL_CPP_TYPE_STRING_VAR || vAux.Data.PointerChar == NULL)
								{
									DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_TF_BAD_ID_ORGANIZATION_IN_CHANNEL, pChannel->GetpChannelDef()->Id());
									return M4_ERROR;
								}

								m4pcchar_t	pccOrg = vAux.Data.PointerChar ;
								m4bool_t bInheritsData = m_pParam->GetInfoInheritsData( indParam ) ;
								m4int32_t iTableLevel = m_pParam->GetInfoOrgLevel( indParam ) ;

								if( bInheritsData == M4_FALSE )
								{
									// Si no es de herencia hay que obtener la sociedad del nivel de la tabla
									pccOrg = m_pGlobVars->GetpLConn()->FindParentOrg( pccOrg, iTableLevel, M4_FALSE ) ;
								}
								else
								{
									// Si es de herencia y la sociedad es menor que la de la tabla también hay que obtener la sociedad del nivel de la tabla
									m4int32_t iOrgLevel = m_pGlobVars->GetpLConn()->FindOrgLevel( pccOrg ) ;

									if( iTableLevel > iOrgLevel )
									{
										pccOrg = m_pGlobVars->GetpLConn()->FindParentOrg( pccOrg, iTableLevel, M4_FALSE ) ;
									}
								}

								if( pccOrg != NULL )
								{
									/* Bug 0162739
									Si no se ha modificado la sociedad no hace falta asignar
									*/
									if( strcmpi( pccOrg, vAux.Data.PointerChar ) != 0 )
									{
										vAux.SetString( (const m4pchar_t) pccOrg ) ;
									}
								}
								else
								{
									// No se retorna error en lectura
									vAux.SetNull();
								}
							}
							break;

						case M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL:
							{
								// Se pone el nivel de la sociedad del canal
								ClChannel *pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

								if (pChannel->Organization.Get(vAux)!=M4_SUCCESS || vAux.Type!=M4CL_CPP_TYPE_STRING_VAR || vAux.Data.PointerChar == NULL)
								{
									DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_TF_BAD_ID_ORGANIZATION_IN_CHANNEL, pChannel->GetpChannelDef()->Id());
									return M4_ERROR;
								}

								m4int32_t iOrgLevel = m_pGlobVars->GetpLConn()->FindOrgLevel( vAux.Data.PointerChar ) ;
								m4bool_t bInheritsData = m_pParam->GetInfoInheritsData( indParam ) ;

								if( bInheritsData == M4_TRUE )
								{
									// Si es de herencia y la sociedad es menor que la de la tabla hay poner el nivel de la tabla
									m4int32_t iTableLevel = m_pParam->GetInfoOrgLevel( indParam ) ;

									if( iTableLevel > iOrgLevel )
									{
										iOrgLevel = iTableLevel ;
									}
								}
								vAux.SetDouble( iOrgLevel ) ;
							}
							break;

						case M4CL_INTERNAL_TYPE_ID_DATASET:
							{
								// Se pone la tramitación del canal
								ClChannel *pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

								pChannel->Tramitation.Get(vAux);
							}
							break;

						case M4CL_INTERNAL_TYPE_RANGE_START:

							vAux.SetDouble(m_pGlobVars->GetpAccessRecordSet()->GetCurrentBlock()->GetRangeStart() + 1);
							break;

						case M4CL_INTERNAL_TYPE_RANGE_END:

							vAux.SetDouble(m_pGlobVars->GetpAccessRecordSet()->GetCurrentBlock()->GetRangeStart() + m_pGlobVars->GetpAccessRecordSet()->GetCurrentBlock()->GetRangeSize());
							break;

						default:
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClLoad::Execute" << "Wrong Internal Type in Param " << LogCat << indParam);
							return (M4_ERROR);
							break;

						}
					}
					break;

				default:
					continue;
			}

			#ifdef _M4LDB_DEBUG
				if ((eParamType == M4LDB_PAR_SESSION) || 
					(eParamType == M4LDB_PAR_ITEM)) {
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Itm[%ld] = ", indParam + 1, hItem);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\tData[%ld] Itm[%ld] = ", indParam + 1, hItem);
				}
				else {
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] = ", indParam + 1);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\tData[%ld] = ", indParam + 1);
				}
			#endif

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			m4pvoid_t pValue = ConvertValue (pvAux, m4Type);

			if (!pValue) {
				m_pParam->SetNull(indParam);
				#ifdef _M4LDB_DEBUG
					DisplayData (indParam);
				#endif
				m4id_t iItem = M4LDB_EMPTY;
				m4id_t nItem = M4LDB_EMPTY;
				if (m_pParam->GetInfoParamType (indParam) == M4LDB_PAR_ITEM) {
					nItem = m_pGlobVars->GetpMCR ()->nItemI (m_pParam->GetItemHandle (indParam));
					if (!*nItem) {
						nItem = m_pGlobVars->GetpMCR ()->iItemI (m_pParam->GetItemHandle (indParam));
					}

					iItem = m_pGlobVars->GetpMCR ()->iItemI (m_pParam->GetItemHandle (indParam));

					DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PARAMETER_VALUE, 
						LOG_CAT_AND (iItem, nItem));
				}
				else {
					DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PARAMETER_VALUE, 
						indParam + 1);
				}
			}
			else {
				switch (m4Type) {

					default:
						m_pParam->SetData (indParam, (m4pcchar_t) pValue);
						break;

					case M4CL_M4_TYPE_DATE:
						dValue = (m4date_t) M4LDB_NUMBER (pValue);
						m_pParam->SetData (indParam, (m4pchar_t) &dValue);
						break;

					case M4CL_M4_TYPE_TIMESTAMP:
						dValue = M4LDB_DOUBLE (pValue);
						m_pParam->SetData (indParam, (m4pchar_t) &dValue);
						break;

					case M4CL_M4_TYPE_NUMBER:
						dValue = M4LDB_DOUBLE (pValue);
						m_pParam->SetData (indParam, (m4pchar_t) &dValue);
						break;
				}
				#ifdef _M4LDB_DEBUG
					DisplayData (indParam);
				#endif
			}
		}

		result = ClStmt::Execute(ai_iTimeOut);
		
		bRetry = M4_FALSE;

		if (m_pGlobVars->IsRetry ()) {
			m_pGlobVars->SetIsRetry (M4_FALSE);
			if (m_pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX) {
				if (m_pGlobVars->GetNumberOfAttempts ()) {
					bRetry = M4_TRUE;
					indExtPar = 0;
				}
				m_pGlobVars->SetNumberOfAttempts (m_pGlobVars->GetNumberOfAttempts ()-1);
			}
		}

	} while (bRetry);

	return (result);
}


m4return_t ClLoad::GetData (m4int32_t *ao_iNumData, m4int32_t ai_iFirstData, m4int32_t ai_indFirstReg)
{

	m4return_t			result = M4_ERROR;
	m4bool_t			bNew = M4_TRUE;
	m4int32_t			iMaxNumReg = 0;
	m4int32_t			indLastUpdate = m_pMtSent->GetIndDtLastUpdate();
	StPartialLoad		*pPL = NULL;
	ClAccessRecordSet	*poRecordSet = m_pGlobVars->GetpAccessRecordSet();

#ifdef _M4LDB_DEBUG
	m4char_t			aValue[M4LDB_SIZE_TIMESTAMP + 1];
#endif


	if (ao_iNumData)
	{
		iMaxNumReg = *ao_iNumData;
	}

	if (IsPartialLoad())
	{
		pPL = m_pGlobVars->GetpLConn ()->m_pLdb->GetVectorPartialLoad ().FindPartialLoad (GetCurrentCursor (), m_pGlobVars->GetpLConn ()->GetIdLConn ());
	}

	ClDinArr *pColumns = GetpColumns();

	if (ai_indFirstReg == M4LDB_NULL_INDEX)
	{
		if (m_pAuxLoad)
		{
			ai_indFirstReg = poRecordSet->Register.GetFirstToWrite ();
		}
	}
	else
	{
		bNew = M4_FALSE;
	}
	
	m4int32_t indCol;
	m4int32_t numCol = m_oVColumns.size();
	m4int32_t iNumData = 0;
	m4int32_t iTotalData = 0;
	
	if (ai_iFirstData != M4LDB_NULL_INDEX)
	{
		// Recorrer los datos primeros que no se quieren
		m4int32_t iReadRegister = (pPL) ? pPL->GetReadRegister () : 0;

		for (; iReadRegister <= ai_iFirstData ; iReadRegister++)
		{
			result = Fetch ();
			
			if (result != M4LDB_SQL_NO_DATA_FOUND && result != M4_ERROR)
			{
				iTotalData++;
			}
			
			if (result == M4LDB_SQL_NO_DATA_FOUND)
			{
				Close ();
				
				if (!ao_iNumData) {
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t No Data Found");
						M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t No Data Found");
					#endif
				}
				else {
					#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t The first data asked is number %ld and there are only %ld.", ai_iFirstData, (pPL) ? pPL->GetReadRegister () : 0);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t The first data asked is number %ld and there are only %ld.", ai_iFirstData, (pPL) ? pPL->GetReadRegister () : 0);
					#endif
					*ao_iNumData = 0;
				}

				AuditRows( iTotalData );

				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), "\n\t\tNumber of records: %ld.", iTotalData);
				#endif

				return (M4_SUCCESS);
			}
			
			if (result == M4_ERROR)
			{
				if (ao_iNumData)
				{
					*ao_iNumData = 0;
				}
				return (M4_ERROR);
			}
		}

		if (pPL)
		{
			pPL->SetReadRegister (iReadRegister);
		}
	}
	else
	{
		result = Fetch();

		if (result != M4LDB_SQL_NO_DATA_FOUND && result != M4_ERROR)
		{
			iTotalData++;
		}
		
		if (result == M4LDB_SQL_NO_DATA_FOUND)
		{
			if (pPL)
			{
				pPL->Close ();
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t No Data Found");
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t No Data Found");
			#endif

			if (ao_iNumData)
			{
				*ao_iNumData = 0;
			}

			AuditRows( iTotalData );

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), "\n\t\tNumber of records: %ld.", iTotalData);
			#endif

			return (M4_SUCCESS);
		}
	}
	
	// Cuando salga del bucle estará en el primer registro que se
	// quiere traer
	while (result != M4LDB_SQL_NO_DATA_FOUND && result!=M4_ERROR)
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t");
		#endif

		// Si hubiera que desencriptar, este es el momento.
		if (pColumns->DecryptAll()!=M4_SUCCESS)
		{
			return M4_ERROR;
		}

		// Puede
		if (!ao_iNumData || ai_iFirstData == M4LDB_NULL_INDEX)
		{
			result = pColumns->GetPipe()->SetLotOfItems (&(poRecordSet->Register),m_InitCol,m_EndCol,bNew);
		}
		else
		{
			result = pColumns->GetPipe()->SetLotOfItemsPartial (&(poRecordSet->Register),m_InitCol,m_EndCol,bNew);
		}

        if (result == M4_ERROR)
		{
			Close ();
            break;
        }

		m4int32_t indVCol;
		for (indVCol=0; indVCol<numCol; indVCol++)
		{
			indCol = m_oVColumns[indVCol];
			
			if (indCol == indLastUpdate)
			{
				m4int32_t lLength = pColumns->GetLength (indLastUpdate);
				m4pcchar_t pValue = pColumns->GetData (indLastUpdate);

				if (lLength != M4LDB_SQL_NULL_DATA && pValue && *(m4date_t *)(pValue))
				{
					VersionMark dDate;
					dDate.Date = *(m4date_t *)(pValue);

					if (!ao_iNumData || ai_iFirstData == M4LDB_NULL_INDEX)
					{
						poRecordSet->Register.LastUpdated.Set(dDate);
					}
					else
					{
						/* Bug 0081692
						En carga parcial hay que asignar según el índice real,
						porque el otro no está posicionado
						*/
						poRecordSet->Register.GetCurrentBlock()->R_SetVersion(poRecordSet->Register.GetMMIndex(), dDate);
					}
				}
			}
			
			m4int8_t m4Type = pColumns->GetInfoM4Type (indCol);
			
			if (m4Type == M4CL_M4_TYPE_LONG || m4Type == M4CL_M4_TYPE_UNI_LONG || m4Type == M4CL_M4_TYPE_BINARY_STRING)
			{
				ClHandle hItem (pColumns->GetItemHandle (indCol));
				
				result = GetLongData (indVCol, m4Type, &poRecordSet->Register.Item[hItem]);
				
				if (result == M4_ERROR)
				{
					if (ao_iNumData)
					{
						*ao_iNumData = 0;
					}
					return (M4_ERROR);
				}
			}
			else
			{ 
				if (m4Type == M4CL_M4_TYPE_BLOB)
				{
					ClHandle hItem (pColumns->GetItemHandle (indCol));
					
					result = GetBlobData(indVCol, m4Type, &poRecordSet->Register.Item[hItem]);
					
					if (result == M4_ERROR)
					{
						if (ao_iNumData)
						{
							*ao_iNumData = 0;
						}
						return (M4_ERROR);
					}
				}
				
				#ifdef _M4LDB_DEBUG
					else {				
						M4LDB_BEGIN_DEBUG;
						if (pColumns->GetLength (indCol) != M4LDB_SQL_NULL_DATA)
						{
							m4pcchar_t pValue = pColumns->GetDataPointer (indCol);
							
							switch (m4Type)
							{
							default:
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", pValue);
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " \"%s\" ; ", pValue);
								break;
								
							case M4CL_M4_TYPE_LONG:
							case M4CL_M4_TYPE_UNI_LONG:
							case M4CL_M4_TYPE_BINARY_STRING:
								break;
								
							case M4CL_M4_TYPE_DATE:
							case M4CL_M4_TYPE_TIMESTAMP:
								ClDateToString (M4LDB_DOUBLE (pValue), aValue, M4LDB_SIZE_TIMESTAMP + 1);
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								break;
								
							case M4CL_M4_TYPE_NUMBER:
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " %.20G ; ", M4LDB_DOUBLE (pValue));
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " %.20G ; ", M4LDB_DOUBLE (pValue));
								break;
							}
						}
						else
						{
							M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " <null> ; ");
							M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " <null> ; ");
						}
						M4LDB_END_DEBUG;
					}
				#endif
			}
		}

		// Comprobar cuantos se quieren leer
		if (ao_iNumData)
		{
			// Comprobar si hay que seguir       
			iNumData++;
			
			if (iNumData == *ao_iNumData)
			{
				if (ai_iFirstData == M4LDB_NULL_INDEX)
				{
					/* Bug 0158771
					Si hay que chequear el total de registro se lee la información y se hace el fetch
					*/
					m4bool_t		bDisableMessage = M4_FALSE;
					m4uint32_t		i = 0;
					m4uint32_t		iNumDataLimit = -1;
					m4VariantType	vValue;
					ClAccess		*poAccess = NULL;
					ClChannel		*poChannel = NULL;
					ClChannel		*poSessionChannel = NULL;
					ClLogonAdaptor	*poLogonAdaptor = NULL;
					
					// Se busca el parámetro global
					bDisableMessage = M4_FALSE;
					iNumDataLimit = -1;

					poAccess = poRecordSet->GetpAccess();

					if( poAccess != NULL )
					{
						poChannel = poAccess->GetpChannel();

						if( poChannel != NULL )
						{
							if( poChannel->Property.GetPropValue( M4CMPROP_CHANNEL_N_ROWS_DB_LIMIT, vValue ) == M4_SUCCESS )
							{
								if( vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData > 0 )
								{
									iNumDataLimit = (m4int32_t) vValue.Data.DoubleData;
								}
							}

							if( poChannel->Property.GetPropValue( M4CMPROP_CHANNEL_DISABLE_MAX_RECORDS_MESSAGE, vValue ) == M4_SUCCESS )
							{
								if( vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData != 0 )
								{
									bDisableMessage = M4_TRUE;
								}
							}
						}
					}

					if( iNumDataLimit == -1 )
					{
						poLogonAdaptor = m_pGlobVars->GetpLConn()->m_pLogonAdaptor ;

						if( poLogonAdaptor != NULL )
						{
							poSessionChannel = poLogonAdaptor->GetSessionChannelPointer();

							if( poSessionChannel != NULL )
							{
								iNumDataLimit = M4LDB_NUMBER( poLogonAdaptor->GetSessionRecordSet()->Item[ M4_ITEM_NUM_ROWS_DB_LIMIT ].Value.GetWithOutSec() );
							}
						}
					}

					if( iNumDataLimit <= *ao_iNumData )
					{
						// Mensaje antiguo
						if( bDisableMessage == M4_FALSE )
						{
							DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_TF_RESULT_TRUNCATED, *ao_iNumData);
						}
						poRecordSet->SetLastLoadStatus( 2, iNumData, iNumData ) ;
					}
					else
					{
						// Se itera hasta alcanzar el límite (más 1 por si justo hay el número de registros pedidos)
						for( i = *ao_iNumData ; i < iNumDataLimit + 1; i++ )
						{
							result = Fetch() ;

							if( result == M4LDB_SQL_NO_DATA_FOUND )
							{
								// Si no hay más registros se acaba
								break;
							}

							if( result == M4_ERROR )
							{
								// Si hay error se da el mensaje de completo
								i = iNumDataLimit ;
							}
						}

						if( i == iNumDataLimit + 1 )
						{
							if( bDisableMessage == M4_FALSE )
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_TF_RESULT_TRUNCATED_OF_LIMIT, *ao_iNumData << iNumDataLimit);
							}
							poRecordSet->SetLastLoadStatus( 4, iNumData, iNumDataLimit ) ;
						}
						else
						{
							if( bDisableMessage == M4_FALSE )
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_TF_RESULT_TRUNCATED_OF_NUMBER, *ao_iNumData << i);
							}
							poRecordSet->SetLastLoadStatus( 3, iNumData, i ) ;
						}
					}
					Close ();
				}
				
				if (!AllowCursorOpened ())
				{				
					Close ();
				}

				break;
			}
		}
		
        // Caso de haber superado el máximo número de registros pore bloque.
		if (pColumns->GetPipe()->m_MaxRecords)
		{
#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t Maximum number of records reached");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t Maximum number of records reached");
#endif
			Close ();
			break;
		}


		result = Fetch();
		
		if (result != M4LDB_SQL_NO_DATA_FOUND && result != M4_ERROR)
		{
			iTotalData++;
		}
		
		if (result != M4LDB_SQL_NO_DATA_FOUND)
		{
			if (pPL)
			{
				pPL->SetReadRegister (pPL->GetReadRegister () + 1);
			}
		}
		else
		{
			if (pPL)
			{
				pPL->Close ();
			}
		}
	}

	if (result == M4_ERROR)
	{
		if (ao_iNumData)
		{
			*ao_iNumData = 0;
		}
		return (M4_ERROR);
	}

	if (ao_iNumData)
	{
		if (ai_iFirstData != M4LDB_NULL_INDEX || iNumData < *ao_iNumData)
		{
			poRecordSet->SetLastLoadStatus( 1, iNumData, iTotalData ) ;
		}

		// Si hay puntero hay que devolver cuantos se han leido
		*ao_iNumData = iNumData;
	}
	else
	{
		poRecordSet->SetLastLoadStatus( 1, iTotalData, iTotalData ) ;
	}

    if (m_pAuxLoad)
	{
		if (!IsPartialLoad() || m_pAuxLoad->NewCursor (ai_iFirstData))
		{
			result = m_pAuxLoad->Execute ();

			if (result == M4_ERROR)
			{
				if (ao_iNumData)
				{
				   *ao_iNumData = 0;
				}
				return (M4_ERROR);
			}
		}

        poRecordSet->Register.SetFirstToWrite (ai_indFirstReg);

        if (ao_iNumData)
		{
            poRecordSet->Register.SetEndOfRegisters (M4_TRUE);
		}

		// Vuelvo a pasar el máximo numero de registros.
        if (ao_iNumData)
		{
			result = m_pAuxLoad->GetData(&iMaxNumReg, ai_iFirstData, ai_indFirstReg);
		}
		else
		{
			result = m_pAuxLoad->GetData(ao_iNumData, ai_iFirstData, ai_indFirstReg);
		}

		// Si la auxiliar devolviera distinto numero de registros, tendríamos un problema.
		// Se han cambiado registros entre una y otra.
		if (ao_iNumData && iMaxNumReg != *ao_iNumData)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_WRONG_SEPARATED_LOAD_NUMREG,iMaxNumReg<<(*ao_iNumData));
			result = M4_ERROR;
		}

        poRecordSet->Register.SetFirstToWrite (ai_indFirstReg);
    }

	AuditRows( iTotalData );

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t\tNumber of records: %ld.", iTotalData);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), "\n\t\tNumber of records: %ld.", iTotalData);
	#endif

    return (result);
}


m4return_t ClLoad::GetLongData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClItem *ai_pItem)
{
	m4return_t	result;
	m4int32_t	lOffsetLen = 0;
	m4int32_t	lLastLen = 0;
	m4int32_t	lLen = 0;
	m4int32_t	lLength = M4LDB_SQL_NULL_DATA;
	m4bool_t	bByPass = M4_FALSE;
	m4bool_t	bFirst = M4_TRUE;
	m4int32_t	iMaxLen = 0;
	m4pchar_t	pcValue = NULL;

	ClDinArr *pColumns = GetpColumns();
	m4int32_t indCol = m_oVColumns[ai_indCol];

	if (pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_BYPASS)
	{
		// Leemos el valor, pero no lo utilizamos.
		bByPass = M4_TRUE;
	}

	iMaxLen = M4LDB_SIZE_LONG * M4UNI_FACTOR;
	pcValue = new char[ iMaxLen + 1 ];

	do {
		lLen = 0;

		// UNICODE DB
		result = ClStmt::GetData (ai_indCol+1, &pcValue, &iMaxLen, ai_lM4Type, &lLen, bByPass);
	
		if (result == M4_ERROR)
		{
			delete [] pcValue ;
			return (M4_ERROR);
		}

		if( lLen > 0 && bByPass == M4_FALSE )
		{
			lLength = lLen;

			ai_pItem->Value.SetNotUpdated( pcValue, lLength + 1, lOffsetLen, NULL, M4_TRUE, M4_FALSE );
			lOffsetLen += lLength;

			#ifdef _M4LDB_DEBUG
				if ((lOffsetLen > M4LDB_SIZE_DEFAULT_STRING) && 
					(m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel () < M4LDB_DEBUG_LEVEL_3)) {

					/* Bug 0146180
					Se cambia el formato de impresión, aunque en este caso, por ser null terminados no haría falta
					*/
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pcValue, &pcValue[lLength-32], lLength);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pcValue, &pcValue[lLength-32], lLength);
				}
				else {
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n \"%s\" ; ", pcValue);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n \"%s\" ; ", pcValue);
				}
			#endif
		}

		if (lLen == M4LDB_SQL_NULL_DATA) {
			if (lLastLen == M4LDB_SQL_NULL_DATA) {
				result = M4LDB_SQL_NO_DATA_FOUND;
			}
			lLastLen = M4LDB_SQL_NULL_DATA;
		}

		if (bFirst == M4_TRUE && lLen != M4LDB_SQL_NULL_DATA && pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_FIRST)
		{
			// Si tenemos que emular el NVL, y había datos en el primero, marcamos el segundo  para que pase.
			if (pColumns->GetNumData() > (indCol+1))
			{
				pColumns->SetEmulateNVL(indCol+1,M4LDB_NVL_BYPASS);
			}
		}
		bFirst = M4_FALSE;

	} while (result != M4LDB_SQL_NO_DATA_FOUND);

	delete [] pcValue ;


	// Vamos a desencriptar :
	if (lLength != M4LDB_SQL_NULL_DATA && pColumns->GetIsEncrypted(indCol)){
		// Voy a coger la mitad de la longitud que tenga el item.
		m4int_t		retLen = 0;
		m4pchar_t	pTemp = NULL;
		m4pchar_t	pValue = (m4pchar_t)ai_pItem->Value.Get ();
		
		if (M4DataDeConvert(pValue,lOffsetLen,pTemp,retLen)!=M4_SUCCESS){
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_DECRYPTING_DATA,pValue);
			return M4_ERROR;
		}
		
		#ifdef _M4LDB_DEBUG
			if ((retLen > M4LDB_SIZE_DEFAULT_STRING) && 
				(m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel () < M4LDB_DEBUG_LEVEL_3)) {

				/* Bug 0146180
				Se cambia el formato de impresión, aunque en este caso, por ser null terminados no haría falta
				*/
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pTemp, &pTemp[retLen-32], retLen);
				M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pTemp, &pTemp[retLen-32], retLen);
			}
		#endif
		ai_pItem->Value.SetNotUpdated((m4pvoid_t) pTemp, retLen+1, 0, NULL, M4_TRUE, M4_FALSE);
		delete [] pTemp;
	}

	if (pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_BYPASS)
	{
		// Restauramos el valor de NO NVL para que no afecte a distintas filas.
		pColumns->SetEmulateNVL(indCol,M4LDB_NO_NVL);
	}

	#ifdef _M4LDB_DEBUG
		if (lLength == M4LDB_SQL_NULL_DATA) {
			M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " <null> ; ");
			M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " <null> ; ");
		}
	#endif

	return (M4_SUCCESS);
}

m4return_t ClLoad::GetBlobData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClItem *ai_pItem)
{
	m4return_t	result;
	m4int32_t	lLastLen = 0;
	m4int32_t	lLen;
	m4int32_t	lLength = M4LDB_SQL_NULL_DATA;
	m4bool_t	bByPass = M4_FALSE;
	m4bool_t	bFirst = M4_TRUE;
	FILE*		pFile = NULL;
	m4bool_t	bHaveFile = M4_FALSE;
	m4int32_t	lengthExt;
	m4uint32_t	filesize = 0;
	m4char_t	pcNameFile[M4_MAX_PATH+1] = "";
	m4char_t	BufferDescr[M4_MAX_PATH+1] = "";
	m4char_t	BufferExt[M4_MAX_PATH+1] = "";
	m4pchar_t	pcDesription = NULL;
	m4pchar_t	pcExtension = NULL;
	m4pchar_t	pDesc = NULL;
	m4int32_t	iMaxLen = 0;
	m4pchar_t	pcValue = NULL;
	ClFileName	oFileName;
	ClFileManager *poFileManager = NULL;

	ClDinArr *pColumns = GetpColumns();
	m4int32_t indCol = m_oVColumns[ai_indCol];
	
	if (pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_BYPASS)
	{
		// Leemos el valor, pero no lo utilizamos.
		bByPass = M4_TRUE;
	}

	iMaxLen = M4LDB_SIZE_LONG * M4UNI_FACTOR;
	pcValue = new char[ iMaxLen + 1 ];

	do {
		lLen = 0;

		// UNICODE DB
		result = ClStmt::GetData (ai_indCol+1, &pcValue, &iMaxLen, ai_lM4Type, &lLen, bByPass);
		
		if (result == M4_ERROR)
		{
			delete [] pcValue;
			return (M4_ERROR);
		}
		
		if( lLen > 0 && bByPass == M4_FALSE )
		{
			lLength = lLen;
			
			if (bHaveFile)
			{
				fwrite (pcValue, sizeof (m4char_t), lLength, pFile);
				filesize+=lLength;
			}
			else
			{
				//Bug 0080757
				//En el driver de DataDirect 4.1 esta dando problemas la desencriptacion.
				//Tenemos una select con blob, long, si la select trae datos en este orden con el driver tenemos un problema:
				//Se trae un nulo y se indica un tamaño 1. Es incorrecto pq en caso de obtener un nulo, el driver debería
				//informarnos un tamaño -1 (SQL_NULL_DATA). El problema viene en la desencriptación de un nulo.
				//Probado con driver para Oracle Isolv 3.11 no hay problemas. El driver de SQLServer 2000 tampoco está dando problemas.
				//Workaround:
				//1.- La sentencia haga la select en el orden long, blob.
				//2.- La solución a nivel de código supone reordenar los campos long para que el orden en la select
				//sea long, blob. Es decir, dejar los blob para el final de todo.
				//3.- Comprobar que el contenido del puntero es un NULL, en ese caso no podemos desencriptar.
				//Se añaden más opciones al if!!.
				if (pColumns->GetIsEncrypted(indCol) && pcValue != NULL && *pcValue != NULL)
				{
					// Me viene : CabeceraEncriptada+0+ExtensionEncriptada+0+FicheroEncriptado.
					
					m4uint32_t SizeExt = 0, SizeHeader = 0;
					m4pchar_t pHeader = 0,pExt = 0;
					m4int_t   retLen;
					
					// Desencriptamos la cabecera

					pHeader  = pcValue;

					SizeHeader = strlen (pHeader);

					if (M4DataDeConvert(pHeader,SizeHeader,pcDesription,retLen)!=M4_SUCCESS)
					{
						delete [] pcValue;
						return M4_ERROR;
					}

					memcpy(BufferDescr, pcDesription, retLen + 1);
					delete [] pcDesription;

					pExt = pcValue + SizeHeader + 1;

					SizeExt = strlen (pExt);

					pDesc = BufferDescr+M4LDB_BLOB_DESCR_LEN;

					if (M4DataDeConvert(pExt,SizeExt,pcExtension,retLen)!=M4_SUCCESS)
					{
						delete [] pcValue;
						return M4_ERROR;
					}
					
					memcpy(BufferExt, pcExtension, retLen + 1);
					delete pcExtension;

					lengthExt = SizeExt+1+SizeHeader+1;
				}
				else
				{
					// Cuidado. La extension tiene que venir como Ext\0
					lengthExt = strlen (pcValue);
					
					if (lengthExt == lLength)
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BLOB_WITHOUT_EXTENSION, ai_indCol);
						delete [] pcValue;
						return (M4_ERROR);
					}
					
					// Sumamos el '\0'.
					lengthExt++;

					// Comprobamos en nuevo formato blob
					if (!strncmp(pcValue,M4LDB_BLOB_DESCR_PREFIX,M4LDB_BLOB_DESCR_LEN))
					{
						m4pchar_t buffer = pcValue + M4LDB_BLOB_DESCR_LEN;
						strcpy(BufferDescr,buffer);
						buffer += strlen(buffer) + 1;
						strcpy(BufferExt,buffer);
						buffer += strlen(buffer) + 1;

						// corregimos la longitud
						lengthExt = buffer - pcValue;
					}
					else
					{
						// generamos descripción por defecto
						sprintf(BufferDescr,"UNKNOWN.%s",pcValue);
						strcpy(BufferExt,pcValue);
					}

					pDesc = BufferDescr;
				}

				poFileManager = ClFileManager::Instance();

				m4return_t iRes = poFileManager->CreateTempFile (oFileName, M4LDB_BLOB_PREFIX, BufferExt, NULL);

				if (iRes == M4_SUCCESS)
				{
					iRes = oFileName.GetFileName(pcNameFile);
				}

				if (M4_SUCCESS != iRes)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_BLOB_FILE, ai_indCol);
					delete [] pcValue;
					return (M4_ERROR);
				}
				
				// UNICODE FILE
				eUniFileEncodingType_t eEncoding = M4UniANSI ;
				pFile = M4Fopen (pcNameFile, M4UniWriteBinary, eEncoding);
				
				if (!pFile)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_BLOB_FILE, ai_indCol);
					delete [] pcValue;
					return (M4_ERROR);
				}
				
				lLength -= lengthExt;
				
				if (lLength < 1)
				{
					// Fichero vacio
					break;
				}
				
				fwrite (pcValue + lengthExt, sizeof (m4char_t), lLength, pFile);
				filesize+=lLength;
					
				fflush (pFile);

				bHaveFile = M4_TRUE;
			}
		}

		if (lLen == M4LDB_SQL_NULL_DATA)
		{
			if (lLastLen == M4LDB_SQL_NULL_DATA)
			{
				result = M4LDB_SQL_NO_DATA_FOUND;
			}
			lLastLen = M4LDB_SQL_NULL_DATA;
		}

		if (bFirst == M4_TRUE && lLen != M4LDB_SQL_NULL_DATA && pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_FIRST)
		{
			// Si tenemos que emular el NVL, y había datos en el primero, marcamos el segundo  para que pase.
			if (pColumns->GetNumData() > (indCol+1))
			{
				pColumns->SetEmulateNVL(indCol+1, M4LDB_NVL_BYPASS);
			}
		}
		bFirst = M4_FALSE;

	} while (result != M4LDB_SQL_NO_DATA_FOUND);

	delete [] pcValue;

	if (pColumns->GetEmulateNVL(indCol) == M4LDB_NVL_BYPASS)
	{
		// Restauramos el valor de NO NVL para que no afecte a distintas filas.
		pColumns->SetEmulateNVL(indCol,M4LDB_NO_NVL);
	}

	#ifdef _M4LDB_DEBUG
		if (lLength == M4LDB_SQL_NULL_DATA)
		{
			M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " <null> ; ");
			M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " <null> ; ");
		}
		else
		{
			M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " %s ; ", pcNameFile);
			M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " %s ; ", pcNameFile);
		}
	#endif
	
	if (pFile)
	{
		fclose (pFile);
	}

	// Hay que desencriptar antes de hacer el Set.
	if (lLength != M4LDB_SQL_NULL_DATA&& pColumns->GetIsEncrypted(indCol))
	{
		// Desencripto el fichero sobre si mismo.
		if (M4DeConvertFile(pcNameFile,pcNameFile)!=M4_SUCCESS)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_DECRYPTING_DATA,pcNameFile);
			return M4_ERROR;
		}

	}

	if (pFile)
	{
		ai_pItem->Value.SetNotUpdated(pcNameFile, strlen(pcNameFile) + 1, 0, NULL, M4_TRUE, M4_FALSE);
		ai_pItem->Value.SetDescr(pDesc);
		if (poFileManager != NULL)
		{
			if (poFileManager->UnSetInterestInFile(oFileName) != M4_SUCCESS)
			{
				return M4_ERROR;
			}
		}
	}
	
	return (M4_SUCCESS);
}
 


m4return_t ClLoad::GetParamValue (m4int32_t ai_hItem, m4VariantType *ao_pValue)
{
    m4int32_t hNode = m_pGlobVars->GetItemNodeHandle (ai_hItem);    

	if (hNode == m_pGlobVars->GethNode ()) {
        // estamos en el mismo nodo

        m4int8_t lScope = m_pGlobVars->GetItemScope (ai_hItem);

        if (lScope == M4CL_ITEM_SCOPE_REGISTER) {
            // si es de registro, pues se lo pido al padre:
            // ojo: me busco la vida para obtener el handle del item 'padre'

            m4int32_t hItemInvConector = m_pGlobVars->GetItemInverseConnector (ai_hItem);

            m4int32_t hItemFather = m_pGlobVars->GetItemHandleConnectorFatherItem (hItemInvConector);

            return (GetFatherValue (hItemFather, ao_pValue));
        }
        else {
			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
		    m4return_t iResult = m_pGlobVars->GetpAccessRecordSet ()->Item[(ClHandle) ai_hItem].Value.Get (*ao_pValue);

			return( iResult ) ;
        }
	}
	else {
        // es un item de otro nodo (tema de los filtros de conector), pedimos
        // el valor al nodo padre:
        
		if (hNode == m_pGlobVars->GetNodeFatherHandle ()) {
            return (GetFatherValue (ai_hItem, ao_pValue));
		}
		else {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ITEM, ai_hItem);
			return (M4_ERROR);
		}
	}

    return (M4_SUCCESS);
}


m4return_t ClLoad::GetFatherValue (m4int32_t ai_hItem, m4VariantType *ao_pValue)
{
    ClNode_Connector *pnc = m_pGlobVars->GetpAccessRecordSet ()->GetpNode_ExtInfo ()->Parent.GetpNotFree ();

	if (!pnc) {
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
		return (M4_ERROR);
	}

	ClBlockReference br;
	m_pGlobVars->GetpAccessRecordSet ()->GetParentBlock (br);

	if (!m_pAuxRS) {
		m_pAuxRS = new ClRecordSet (0);
	}

    switch (pnc->GetType ()) {

        case M4CL_NODE_CONNECTOR_TYPE_RB:
            {
	            m_pAuxRS->SetCurrentBlock (br);
	            ClRegister &reg = m_pAuxRS->Register;

                m4uint32_t inumrows = reg.GetpNodeDef()->NumRows();
                if (inumrows)
                {
                    m4uint32_t iFV = reg.GetCurrentBlock ()->GetpPL_Data()->GetFirstVisible();
                    m4uint32_t aux_Index = iFV + m_pGlobVars->GetpAccessRecordSet ()->GetCurrentBlock ()->ID.RegisterId;
                    reg.MoveTo (aux_Index);
                }
                else
	            reg.MoveTo (m_pGlobVars->GetpAccessRecordSet ()->GetCurrentBlock ()->ID.RegisterId);

                if (reg.Index == M4DM_EOF_INDEX) {
					DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
	                if (m_pAuxRS)
                        m_pAuxRS->Detach();
					return (M4_ERROR);
				}

	            reg.Item.MoveTo ((ClHandle) ai_hItem);
				if (reg.Item.GetIndex() == M4DM_EOF_INDEX) {
					DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
	                if (m_pAuxRS)
                        m_pAuxRS->Detach();
					return (M4_ERROR);
				}

	            reg.Item.Value.Get (*ao_pValue);
            }
            break;

        case M4CL_NODE_CONNECTOR_TYPE_BB:
            {
	            ClRecordSet &rs = *m_pAuxRS;
	            rs.SetCurrentBlock (br);

	            rs.Item.MoveTo ((ClHandle) ai_hItem);
				if (rs.Item.GetIndex() == M4DM_EOF_INDEX) {
					DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
	                if (m_pAuxRS)
                        m_pAuxRS->Detach();
					return (M4_ERROR);
				}

	            rs.Item.Value.Get (*ao_pValue);
            }
            break;

        default:
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
	        if (m_pAuxRS)
                m_pAuxRS->Detach();
			return (M4_ERROR);
    }

	if (m_pAuxRS)
        m_pAuxRS->Detach();

    return (M4_SUCCESS);
}


m4return_t ClLoad::GetData (ClTblMem *ao_pTbl, m4int32_t *aio_piNumData)
{
	m4return_t result;
	m4int32_t indCol;
	m4int32_t numCol;
	m4int32_t length;
#ifdef _M4LDB_DEBUG
	m4char_t aValue[M4LDB_SIZE_TIMESTAMP + 1];
#endif

	m4int32_t iNumData = 0;
	m4int32_t iTotalData = 0;

	if (m_pColumns==0)
	{
		m_pColumns  = new ClDinArr (m_pGlobVars);
		numCol = GetNumCol ();

		if (numCol == M4LDB_NULL_INDEX)
		{
			return (M4_ERROR);
		}

		m_pColumns ->Init (numCol, M4_TRUE);
		m_pColumns ->Attach ();

		for (indCol=0; indCol<numCol; indCol++)
		{
			m4int32_t lPrec = 0;
			m4int8_t m4Type = 0;

			result = DescribeCol (indCol + 1, &lPrec, &m4Type);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = m_pColumns->CreateData (indCol, NULL, 0, m4Type, lPrec, 0, NULL, NULL, M4LDB_PAR_INTERN, M4_FALSE);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
			
			// UNICODE DB
			result = BindResult (indCol + 1, m_pColumns->GetDataPointerPointer(indCol), m_pColumns->GetMaxLenPointer(indCol), m_pColumns ->GetInfoM4Type (indCol), m_pColumns ->GetLengthPointer (indCol));

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	numCol = m_pColumns->GetNumData ();

	result = Fetch();

	if (result != M4LDB_SQL_NO_DATA_FOUND && result != M4_ERROR)
	{
		iTotalData++;
	}

    if (result == M4_ERROR)
	{
        return (M4_ERROR);
	}

    if (result == M4LDB_SQL_NO_DATA_FOUND)
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t No data found.");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t No data found.");
		#endif
	}

	if (ao_pTbl)
	{
		// Inicializamos la tabla en memoria en la que pondremos el bloque.
		ao_pTbl->Init (m_pGlobVars->GetpLConn()->m_pLdb,numCol);

		for (indCol=0; indCol<numCol; indCol++)
		{
			ao_pTbl->SetM4Type (indCol, m_pColumns->GetInfoM4Type (indCol));
		}
	}

	while (result != M4LDB_SQL_NO_DATA_FOUND)
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t");
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t");
		#endif
		
			// Si hubiera que desencriptar, este es el momento.
		if (m_pColumns->DecryptAll() != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		for (indCol=0; indCol<numCol; indCol++) {
			
			result = M4_SUCCESS;
			
			m4int8_t m4Type = m_pColumns->GetInfoM4Type (indCol);

			if (m4Type != M4CL_M4_TYPE_LONG && 
				m4Type != M4CL_M4_TYPE_UNI_LONG && 
				m4Type != M4CL_M4_TYPE_BLOB && 
				m4Type != M4CL_M4_TYPE_BINARY_STRING)
			{
				m4int32_t lLength = m_pColumns->GetLength (indCol);

				if (lLength != M4LDB_SQL_NULL_DATA)
				{
					m4pcchar_t pValue = m_pColumns->GetDataPointer (indCol);

					switch (m4Type) {

						default:
							if (ao_pTbl)
							{
								result = ao_pTbl->SetCurrentData (pValue, lLength + 1);
							}
							#ifdef _M4LDB_DEBUG
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", pValue);
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " \"%s\" ; ", pValue);
							#endif
							break;

						case M4CL_M4_TYPE_DATE:
							if (ao_pTbl)
							{
								result = ao_pTbl->SetCurrentData (pValue, M4LDB_SIZE_DOUBLE);
							}
							#ifdef _M4LDB_DEBUG
								M4LDB_BEGIN_DEBUG;
								ClDateToString (M4LDB_DOUBLE (pValue), aValue, M4LDB_SIZE_DATE + 1);
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								M4LDB_END_DEBUG;
							#endif
							break;

						case M4CL_M4_TYPE_TIMESTAMP:
							if (ao_pTbl)
							{
								result = ao_pTbl->SetCurrentData (pValue, M4LDB_SIZE_DOUBLE);
							}
							#ifdef _M4LDB_DEBUG
								M4LDB_BEGIN_DEBUG;
								ClDateToString (M4LDB_DOUBLE (pValue), aValue, M4LDB_SIZE_TIMESTAMP + 1);
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
								M4LDB_END_DEBUG;
							#endif
							break;

						case M4CL_M4_TYPE_NUMBER:
							if (ao_pTbl)
							{
								result = ao_pTbl->SetCurrentData (pValue, M4LDB_SIZE_DOUBLE);
							}
							#ifdef _M4LDB_DEBUG
								M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " %.20G ; ", M4LDB_DOUBLE (pValue));
								M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " %.20G ; ", M4LDB_DOUBLE (pValue));
							#endif
							break;
					}
					
					if (result == M4_ERROR) {
						return (M4_ERROR);
					}
				}
				else
				{
					if (ao_pTbl)
					{
						result = ao_pTbl->ReserveCurrentData (0);
					}

					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}

					#ifdef _M4LDB_DEBUG
						M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " <null> ; ");
						M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " <null> ; ");
					#endif
				}
			}
			else
			{
				if (ao_pTbl)
				{
					result = GetLongData (indCol, m4Type, ao_pTbl);
				}
				else
				{
					ClTblMem *pTblMem = new ClTblMem;
					pTblMem->Init (m_pGlobVars->GetpLConn()->m_pLdb,1);

					result = GetLongData (indCol, m4Type, pTblMem);

					if (result == M4_ERROR) {
						delete pTblMem;
						return (M4_ERROR);
					}

					length = pTblMem->GetLength(0, 0);

					if (length != M4LDB_SQL_NULL_DATA)
					{
						m_pColumns->SetInternalLong (indCol, pTblMem);
						result = m_pColumns->SetData (indCol, pTblMem->GetDataPointer(0, 0), length);
					}
					else
					{
						delete pTblMem;
					}
				}

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}

		// Comprobar cuantos se quieren leer
		if (aio_piNumData)
		{
			// Comprobar si hay que seguir       
			iNumData++;
			
			if (iNumData == *aio_piNumData)
			{
				//Vamos a mostrar un mennsaje en ldbinsp para que tengamos en cuenta que se alcanzó
				//el número de registros que pretendíamos leer.
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t Loading Data: Maximum number %ld of records reached.", iNumData);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t Loading Data: Maximum number %ld of records reached.", iNumData);
				break;
			}
		}

		result = Fetch ();

		// bg 174862
		if( result == M4_ERROR )
		{
			return M4_ERROR ;
		}

		if ( result != M4LDB_SQL_NO_DATA_FOUND )
		{
			iTotalData++;
		}
	}

	if (aio_piNumData)
	{
		// Si hay puntero hay que devolver cuantos se han leido
		*aio_piNumData = iNumData;
	}

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	AuditRows( iTotalData );

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_EXECUTION, m_pGlobVars->GetpLConn (), "\n\t\tNumber of records: %ld.", iTotalData);
	#endif

	return (M4_SUCCESS);
}


void ClLoad::AuditRows( m4uint32_t iTotalRows )
{
	m4uint32_t	iAuditRows = m_pGlobVars->GetpLConn()->GetpLdb()->GetAuditRows() ;

	if( iAuditRows > 0 && iTotalRows > iAuditRows )
	{
		m4pcchar_t	pccInformer = m_pGlobVars->GetpLConn()->GetInformer();

		DUMP_CHLOG_ERRORF( M4_CH_AUDIT_ROWS_EXCEEDED, ( pccInformer != NULL ? pccInformer : "" ) << iTotalRows << iAuditRows << DumpLogAndLdb(m_pGlobVars->GetpLConn()) ) ;
	}
}


m4return_t ClLoad::GetLongData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClTblMem *ai_pTbl)
{
	m4return_t result;
	m4return_t result2;
	m4int32_t lLastLen = 0;
	m4int32_t lLen;
	m4int32_t lOffsetLen = 0;
	m4int32_t lLength = M4LDB_SQL_NULL_DATA;
	m4bool_t bFirst = M4_TRUE;
	m4int32_t iMaxLen = 0;
	m4pchar_t pcValue = NULL;


	iMaxLen = M4LDB_SIZE_LONG * M4UNI_FACTOR;
	pcValue = new char[ iMaxLen + 1 ];

	do {
		lLen = 0;

		// UNICODE DB
		result = ClStmt::GetData (ai_indCol + 1, &pcValue, &iMaxLen, ai_lM4Type, &lLen);
	
		if (result == M4_ERROR)
		{
			delete [] pcValue ;
			return (M4_ERROR);
		}

		if( lLen > 0 )
		{
			lLength = lLen;

			/* Bug 0150086
			No hay que poner el nulo final
			*/
			if (!bFirst)
			{
				result2 = ai_pTbl->AppendCurrentData (pcValue, lLength);
			}
			else
			{
				bFirst = M4_FALSE;
				result2 = ai_pTbl->SetCurrentData (pcValue, lLength);
			}

			if (result2 == M4_ERROR) {
				delete [] pcValue ;
				return (M4_ERROR);
			}

			lOffsetLen += lLength;

			#ifdef _M4LDB_DEBUG
				if ((lOffsetLen > M4LDB_SIZE_DEFAULT_STRING) && 
					(m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel () < M4LDB_DEBUG_LEVEL_3)) {

					/* Bug 0146180
					Se cambia el formato de impresión, aunque en este caso, por ser null terminados no haría falta
					*/
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pcValue, &pcValue[lLength-32], lLength);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n \"%.32s...%.32s\" Size = %ld ; ", pcValue, &pcValue[lLength-32], lLength);
				}
				else {
					M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n \"%s\" ; ", pcValue);
					M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n \"%s\" ; ", pcValue);
				}
			#endif
		}

		if (lLen == M4LDB_SQL_NULL_DATA) {
			if (lLastLen == M4LDB_SQL_NULL_DATA) {
				result = M4LDB_SQL_NO_DATA_FOUND;
			}
			lLastLen = M4LDB_SQL_NULL_DATA;
		}

	} while (result != M4LDB_SQL_NO_DATA_FOUND);

	delete [] pcValue ;


	if (lLength == M4LDB_SQL_NULL_DATA) {

		result2 = ai_pTbl->ReserveCurrentData (0);

		if (result2 == M4_ERROR) {
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " <null> ; ");
			M4LDB_PRINTV_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), " <null> ; ");
		#endif
	}

	return (M4_SUCCESS);
}
