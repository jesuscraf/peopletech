
#include "sqlojtrans.hpp"
#include <stdio.h>

m4return_t		
ClOrderByTranslator_Move::MoveLocalFieldsToBegining(string &ao_stTrans, string ai_stLocalPlace)
{
	m4return_t ret;
	string stBlank(" "),stASC(" ASC"), stDESC(" DESC"), stALL("ALL "), stDISTINCT("DISTINCT ");
	m4int32_t iPos = RFindNotInQuote(ao_stTrans,ai_stLocalPlace);

	if (iPos != M4_STR_NPOS)
	{
		string stSelectPlace("SELECT ");
		string stLocal, stSelect;
		m4int32_t iBeginLocal=0, iEndLocal=0;
		m4int32_t iBeginSelect=0, iEndSelect=0;
		m4int32_t iAuxBegin=0, iAuxEnd=0, iAuxSize=0;
		iBeginLocal = iPos + ai_stLocalPlace.size();

		//Pillamos los campos del Local
		ret = GetNextCompleteToken(ao_stTrans.c_str()+iBeginLocal, iEndLocal); 

		if (M4_SUCCESS != ret)
			return ret;

		stLocal.assign(ao_stTrans, iBeginLocal, iEndLocal);

		//Pillamos los campos de la select
		iPos = ao_stTrans.find(stSelectPlace);

		if (iPos == M4_STR_NPOS)
		{
			return M4_ERROR;
		}

		iBeginSelect = iPos + stSelectPlace.size();

		ret = GetNextCompleteToken(ao_stTrans.c_str()+iBeginSelect, iEndSelect); 

		if (M4_SUCCESS != ret)
			return ret;

		//Quitamos ALL o DISTINCT
		iEndSelect += iBeginSelect;
		DelStrFromBE(ao_stTrans,iBeginSelect,iEndSelect,stBlank,0x1);
		DelStrFromBE(ao_stTrans,iBeginSelect,iEndSelect,stALL,0x1);
		DelStrFromBE(ao_stTrans,iBeginSelect,iEndSelect,stDISTINCT,0x1);
		iEndSelect -= iBeginSelect;

		stSelect.assign(ao_stTrans, iBeginSelect, iEndSelect);

		
		//Ahora iremos cambiando stLocal y stSelect para ir pasando campos del uno al otro.
		m4int32_t iLocalFieldBegin=0, iLocalFieldEnd=0;
		do
		{
			ret = GetNextCompleteToken(stLocal.c_str()+iLocalFieldBegin, iLocalFieldEnd, ',');

			if (M4_SUCCESS != ret)
				return ret;

			if (iLocalFieldEnd != 0)
			{
				iLocalFieldEnd = iLocalFieldEnd + iLocalFieldBegin;

				//le quitamos los blancos, " ASC" y " DESC" para comparar
				iAuxBegin=iLocalFieldBegin;
				iAuxEnd=iLocalFieldEnd;

				DelStrFromBE(stLocal,iAuxBegin,iAuxEnd,stBlank,0x3);
				DelStrFromBE(stLocal,iAuxBegin,iAuxEnd,stASC,0x2);
				DelStrFromBE(stLocal,iAuxBegin,iAuxEnd,stDESC,0x2);
				DelStrFromBE(stLocal,iAuxBegin,iAuxEnd,stBlank,0x2);
				iAuxSize = iAuxEnd-iAuxBegin;

				ret = _MoveOneLocalFieldToSelect(stLocal, iAuxBegin, iAuxEnd, stSelect);

				if (M4_SUCCESS != ret)
					return ret;

				//El tamaño de stLocal pudo cambiar
				iLocalFieldBegin = iLocalFieldEnd-iAuxSize+(iAuxEnd-iAuxBegin);
			}
			else
			{
				//veamos si estamos en un ',' o un ' '
				while (*(stLocal.c_str()+iLocalFieldBegin) == ' ')
				{
					iLocalFieldBegin++;
				}

				if (*(stLocal.c_str()+iLocalFieldBegin) == ',')
				{
					iLocalFieldEnd = ++iLocalFieldBegin;
				}
			}
		}
		while (iLocalFieldEnd != 0);

		//Finalmente sustituimos en ao_stTrans
		ao_stTrans.replace(iBeginLocal, iEndLocal, stLocal);
		ao_stTrans.replace(iBeginSelect, iEndSelect, stSelect);
	}

	return M4_SUCCESS;
}

m4int32_t
ClOrderByTranslator_Move::RFindNotInQuote(string &ai_stString, string &ai_stFind, m4uint32_t aio_iStartPos)
{
	m4int32_t iRetPos = 0, iPos = 0;

	iPos = FindNotInQuote(ai_stString, ai_stFind, iPos);

	iRetPos = iPos;

	while (M4_STR_NPOS != iPos)
	{
		iPos = FindNotInQuote(ai_stString, ai_stFind, iPos+1);

		if (M4_STR_NPOS != iPos)
			iRetPos = iPos;
	}

	return iRetPos;
}

m4return_t
ClOrderByTranslator_Move::_MoveOneLocalFieldToSelect(string &ao_stLocal, m4int32_t &aio_iLocalFieldBegin, m4int32_t &aio_iLocalFieldEnd, string &ao_stSelect)
{
	//parseamos stselect, 
	//si no encontramos el campo la añadimos y nos quedamos con el index, 
	//en caso contrario nos quedamos con el index donde lo hemos encontrado.
	//Finalmente en Local sustituimos el campo por su index.
	m4return_t ret;
	m4int32_t iSelectFieldBegin=0, iSelectFieldEnd=0;
	m4int32_t iAuxBegin=0, iAuxEnd=0;
	m4int32_t iIndex = 1;
	string stBlank(" ");

	do
	{
		ret = GetNextCompleteToken(ao_stSelect.c_str()+iSelectFieldBegin, iSelectFieldEnd, ',');

		if (M4_SUCCESS != ret)
			return ret;

		if (iSelectFieldEnd != 0)
		{
			//le quitamos los blancos para comparar
			iAuxBegin=iSelectFieldBegin;
			iAuxEnd=iSelectFieldEnd+iAuxBegin;
			DelStrFromBE(ao_stSelect,iAuxBegin,iAuxEnd,stBlank,0x3);
			string sAux = ao_stSelect.substr(iAuxBegin,iAuxEnd-iAuxBegin);
			string sAux2 = ao_stLocal.substr(aio_iLocalFieldBegin,aio_iLocalFieldEnd-aio_iLocalFieldBegin);
			if (sAux.compare(sAux2)==0) // o al reves el compare!!
				break;

			iSelectFieldBegin += iSelectFieldEnd;

			iIndex++;
		}
		else
		{
			//veamos si estamos en un ',' o un ' '
			while (*(ao_stSelect.c_str()+iSelectFieldBegin) == ' ')
			{
				iSelectFieldBegin++;
			}

			if (*(ao_stSelect.c_str()+iSelectFieldBegin) == ',')
			{
				iSelectFieldEnd = ++iSelectFieldBegin;
			}

			if (iSelectFieldEnd == 0)
			{
				//No hay mas. Inserta el nuevo campo.
				ao_stSelect.append(",");
				ao_stSelect.append(ao_stLocal,aio_iLocalFieldBegin,aio_iLocalFieldEnd-aio_iLocalFieldBegin);
				break;			
			}
		}
	}
	while (iSelectFieldEnd != 0);

	//sustituimos LocalField por iIndex.
	m4char_t pcAuxIndex[256];
	sprintf(pcAuxIndex,"%d",iIndex);
	string stIndex(pcAuxIndex);

	ao_stLocal.replace(aio_iLocalFieldBegin, aio_iLocalFieldEnd-aio_iLocalFieldBegin,stIndex);
	aio_iLocalFieldEnd = aio_iLocalFieldBegin+stIndex.size();

	return M4_SUCCESS;
}
m4return_t
ClOrderByTranslator_Move::DelStrFromBE(string &ai_stString, m4int32_t &aio_iBegin, m4int32_t &aio_iEnd, string &ai_stFind, m4int8_t ai_iWhere)
{
	//Quita ai_stFind del principio (ai_iWhere & 0x1) y final (ai_iWhere & 0x2) de la cadena.
	m4int32_t iSize = ai_stFind.size();
	m4int32_t res;

	string sAux;
	//Al principio
	if (ai_iWhere & 0x1)
	{
		if ((aio_iEnd-aio_iBegin) < iSize)
			 return M4_SUCCESS;
		sAux = ai_stString.substr(aio_iBegin, iSize);
		res = sAux.compare(ai_stFind);

		while ((res == 0) && (aio_iBegin<aio_iEnd))
		{
			aio_iBegin += iSize;

			if ((aio_iEnd-aio_iBegin) < iSize)
				 return M4_SUCCESS;
			sAux = ai_stString.substr(aio_iBegin, iSize);
			res = sAux.compare(ai_stFind);
		}
	}

	//Al final
	if (ai_iWhere & 0x2)
	{
		if ((aio_iEnd-aio_iBegin) < iSize)
			 return M4_SUCCESS;

		sAux = ai_stString.substr(aio_iEnd-iSize, iSize);
		res = sAux.compare(ai_stFind);

		while ((res == 0) && (aio_iBegin<aio_iEnd))
		{
			aio_iEnd -= iSize;

			if ((aio_iEnd-aio_iBegin) < iSize)
				return M4_SUCCESS;

			sAux = ai_stString.substr(aio_iEnd-iSize, iSize);
			res = sAux.compare(ai_stFind);
		}
	}

	return M4_SUCCESS;
}

m4return_t
_ClOuterJoinReductor::ReduceOuterJoins(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bHasChanges)
{
	m4bool_t bBigConflict = M4_FALSE;
	if (_LookForConflicts(ai_vRelat,ai_pMtSent,bBigConflict)){
		if (bBigConflict==M4_FALSE){
			return _SolveConflicts(ai_vRelat,ai_pMtSent,ai_bHasChanges);
		}
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4bool_t 
_ClOuterJoinReductor::_LookForConflicts(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bBigConflict){
	// Me recorro el vector a ver si hay algun Outer join.
	itVcClMtRelat it = ai_vRelat.begin();
	itVcClMtRelat itend = ai_vRelat.end();

	m4bool_t bOuter = M4_FALSE;
	m4bool_t bNormal = M4_FALSE;

	ClMetaRelat * pRel = 0;
	ai_bBigConflict = M4_FALSE;


	for (;it!=itend;++it){
		pRel = (*it);
		if (pRel->m_eJoinType == M4LDB_OUTER_JOIN){
			bOuter = M4_TRUE;
			
			if (bNormal){
				break;
			}
		}
		if (pRel->m_eJoinType == M4LDB_NORMAL_JOIN){
			bNormal = M4_TRUE;
			if (bOuter){
				break;
			}
		}
	}


	if (bOuter){
		// Tengo que mirar el sentido de los Outer Joins que pueda haber, porque debe coincidir,
		m4int32_t indObjDominator = M4LDB_NULL_INDEX;	// indice en el diccionario
		m4int32_t indObj = M4LDB_NULL_INDEX;	// indice en el vector de Objetos
		it = ai_vRelat.begin();
		for (;it!=itend;++it){
			pRel = (*it);
			if (pRel->m_eJoinType == M4LDB_OUTER_JOIN){
				indObj = ai_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField1->m_indAlias);
				if (indObjDominator==M4LDB_NULL_INDEX){
					// Es el primero.Lo calculo.
					indObjDominator = ai_pMtSent->GetObjectByPos(indObj)->m_indLogicTable;
				}
				else{
					if (indObjDominator != ai_pMtSent->GetObjectByPos(indObj)->m_indLogicTable){
						// Tenemos Outer Joins de distinto sentido. Damos error.
						ai_bBigConflict = M4_TRUE;
						return M4_TRUE;
					}
				}
			}
		}

	}
	if (bNormal && bOuter){
		// Hay conflictos.
		return M4_TRUE;
	}
	return M4_FALSE;
}
m4return_t
ClOuterJoinReductor_Oracle::_SolveConflicts(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bHasChanges)
{
	// Los pasamos a Outer Joins.
	itVcClMtRelat it = ai_vRelat.begin();
	itVcClMtRelat itend = ai_vRelat.end();
	// Tengo que buscar el sentido del Outer Join. ( Los campos me puden venir al revés en el Normal.)
	m4pcchar_t pidObject2 = 0;
	m4int32_t indObj = M4LDB_NULL_INDEX;
	ClMetaRelat * pRel = 0;
	
	ai_bHasChanges = M4_FALSE;

	for (;it!=itend;++it){
		pRel = (*it);
		if (pRel->m_eJoinType==M4LDB_OUTER_JOIN){
			// Saco el Id_Object de la dominada.
			indObj = ai_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias);
			ClMetaObject * pObj = ai_pMtSent->GetObjectByPos(indObj);
			pidObject2 = pObj->m_stObject.c_str();
		}
	}
	
	if (pidObject2==0){
		// No habia Outer Join.
		return M4_SUCCESS;
	}
	// Me recorro el vector buscando los Inner Joins y actualizandolos.
	it = ai_vRelat.begin();	
	pRel = 0;
	for (;it!=itend;++it){
		pRel = (*it);
		if (pRel->m_eJoinType==M4LDB_NORMAL_JOIN){
			// Buscamos el sentido correcto.
			indObj = ai_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias);
			ClMetaObject * pObj = ai_pMtSent->GetObjectByPos(indObj);
			if (strcmp (pidObject2,pObj->m_stObject.c_str())!=0){
				//Tenemos que cambiar el orden de los joins.
				ClMetaField * pTemp = pRel->m_pMtField1;
				pRel->m_pMtField1 = pRel->m_pMtField2;
				pRel->m_pMtField2 = pTemp;
				// Bug 0298474 Ha habido cambios
				ai_bHasChanges = M4_TRUE;

				indObj = ai_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias);
				pObj = ai_pMtSent->GetObjectByPos(indObj);
			}

			pRel->m_eJoinType=M4LDB_OUTER_JOIN;
			// Ahora debo marcar el objeto asociado al alias como que tiene Outer Join.
			pObj->m_eJoinType=M4LDB_OUTER_JOIN;
		}
	}
	return M4_SUCCESS;
}
