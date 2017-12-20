//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cltotacum.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//		Class used to totalize in Acummulate TIs   
//
//
//==============================================================================
#include "dmtypes.hpp"
#include "cltotacum.hpp"
#include "totalize.hpp"
#include "register.hpp"
#include "cldates.hpp"
#include "access.hpp"




ClRgAc_Base::ClRgAc_Base(ClRegister *ai_poRegister,StFunctionCmp_t * ai_pfcomp,
						 VIndAndTotType *ai_ItemIndex,m4int32_t ai_iNumElem,
						 m4uint8_t ai_TotType, ClItemIndex ai_idxFirst, 
						 ClItemIndex ai_idxSecond, 
						 ClItemIndex ai_iStartDateItemIndex, ClItemIndex ai_iEndDateItemIndex,
						 ClRegisterIndex ai_Index) : m_oBoundedIterator (ai_poRegister , ai_TotType)
{

	m_poRegister = ai_poRegister;
	m_paItemIndex = ai_ItemIndex;
	m_poRegister -> Begin();

	Index = m_poRegister->Index;

	m_vVariantTemp.Type = M4CL_CPP_TYPE_NULL;
	m_uiTotType = ai_TotType;
		
	m_pstfcomp = ai_pfcomp;
	
	m_bMustStop = M4_FALSE;
	
	m_iNumElemIndex = ai_iNumElem;
	m_bAnyRegisterFound = M4_FALSE;

	m_idxFirst=ai_idxFirst;
	m_idxSecond=ai_idxSecond;

	m_iStartDateItemIndex = ai_iStartDateItemIndex;
	m_iEndDateItemIndex = ai_iEndDateItemIndex; 
}

ClRgAc_Base::ClRgAc_Base(ClRgAc_Base *ai_pBase)
					:m_oBoundedIterator(ai_pBase->m_poRegister , ai_pBase->m_uiTotType){

	m_poRegister = ai_pBase->m_poRegister;
	m_paItemIndex = ai_pBase->m_paItemIndex;
	m_pstfcomp = ai_pBase->m_pstfcomp;

	m_poRegister -> Begin();
	Index = m_poRegister->Index;

	m_bMustStop = M4_FALSE;

	m_iNumElemIndex = ai_pBase->m_iNumElemIndex;
	m_bAnyRegisterFound = M4_FALSE;

	m_vVariantTemp.Type = M4CL_CPP_TYPE_NULL;

	m_idxFirst=ai_pBase->m_idxFirst;
	m_idxSecond=ai_pBase->m_idxSecond;

	m_iStartDateItemIndex = ai_pBase->m_iStartDateItemIndex;
	m_iEndDateItemIndex = ai_pBase->m_iEndDateItemIndex; 

	m_oBoundedIterator=ai_pBase->m_oBoundedIterator;
}

m4bool_t ClRgAc_Base::Next (void)
{
	m4bool_t ret = M4_FALSE, ao_bStop = M4_FALSE;
	m4return_t ret2 = M4_SUCCESS;
	m4VariantType	vValue;



	while (m_poRegister->Next() == M4_TRUE)
	{
		if ( (_CheckReg(ao_bStop)) == M4_TRUE )
		{
//			LastIndex = Index;
//Con los cambios hechos en MoveTo y en Begin no es necesario guardar la posición anterior.
			Index = m_poRegister->Index;

			if (ao_bStop == M4_TRUE )
			{
				return M4_FALSE;
			}
		
			return M4_TRUE;
		}


	}
	
	return M4_FALSE;

	
	
}

m4bool_t ClRgAc_Base::Begin(void)
{
	m4bool_t bTemp;

	if (m_poRegister->Index != M4DM_EOF_INDEX && !(_CheckReg(bTemp))) {

		while ( (m_poRegister->Next()) && !(_CheckReg(bTemp)) ) ;
	}


	return (m_poRegister->Index != M4DM_EOF_INDEX);

	// Lo colocamos en el primer registro a totalizar.
	// Para el caso de una segunda totalización, Index debe estar bien colocado.
}

m4bool_t ClRgAc_Base::End(void)
{
	m4bool_t ao_bStop = M4_FALSE;
	m4VariantType  vTemp  ;

	m_poRegister->End();

	while ( _CheckReg(ao_bStop) != M4_TRUE && m_poRegister -> Prev()) ;
	
	if ( m_bMustStop == M4_TRUE )
	{
		m_poRegister -> Item[ m_pstfcomp[0].m_iIndex ].Value.GetpSliceFooter().Get (vTemp);
		
		while (vTemp == m_vVariantTemp && m_poRegister -> Prev() )

			m_poRegister -> Item[ m_pstfcomp[0].m_iIndex ].Value.GetpSliceFooter().Get (vTemp);

		/* Bug 0091933
		Si hemos pasado a EOF por debajo, hay que hacer un begin, porque el next no hace nada
		*/
		if( m_poRegister->Index == M4DM_EOF_INDEX )
		{
			m_poRegister->Begin() ;
		}
		else
		{
			m_poRegister ->Next(); //para que se quede justo donde debe empezar a leer.
		}
	}

	return m_poRegister->Index != M4DM_EOF_INDEX;

}

m4bool_t ClRgAc_Base::MoveTo(const m4uint32_t ai_Index)
{
	//return	m_poRegister->MoveTo((ClRegisterIndex)ai_Index);
	//Si me mueve al sitio inicial cuando termina, nunca voy a saber desde fuera
	//si ha llegadp o no al EOF.
	//No necesito que se mueva ala sitio inicial.

	//// En el caso de que haya totalización de nivel más alto y esta sea FIRST
	//// estamos en el caso de Varias Pagas ( y Cualquier Paga ). Se debe colocar el Registro en el primero
	//// que sea distinto de m_vVariantTemp, de forma que nos altemos los que debemos 
	if ( m_bMustStop == M4_TRUE && m_vVariantTemp.Type !=M4CL_CPP_TYPE_NULL )
	{
		m4VariantType vTemp = m_vVariantTemp;
		while ( vTemp == m_vVariantTemp && m_poRegister->Next() ) {
			m_poRegister->Item[m_pstfcomp[0].m_iIndex].Value.GetpSliceFooter().Get(vTemp);
		}
	}

	return M4_TRUE;
}


m4bool_t ClRgAc_Base::_CheckReg (m4bool_t &ao_bStop)
{
	m4return_t ret=M4_SUCCESS;

	
	m4bool_t ret2=M4_TRUE;
	m4int32_t i=0;
	m4VariantType vTemp;

	while (m_pstfcomp[i].m_pfcomp!=NULL)
	{
		ret = (m_poRegister->Item[m_pstfcomp[i].m_iIndex].Value.GetpSliceFooter().Get(vTemp));
		if (ret != M4_SUCCESS)
		{
			return M4_FALSE ;
		}

		if ( (!i) && m_vVariantTemp.Type != M4CL_CPP_TYPE_NULL && vTemp != m_vVariantTemp && m_uiTotType != M4CL_ITEM_TOTALIZE_FIRST)
		{
			ao_bStop = m_bMustStop;
			m_vVariantTemp.Type = M4CL_CPP_TYPE_NULL ;
			return M4_TRUE;
		}

		////// En caso de que haya que comparar una columna con otra :
		if (m_pstfcomp[i].m_vVariant.Type == M4CL_CPP_TYPE_NULL)
		{
			m4VariantType vTemp2;
			ret = (m_poRegister->Item[m_pstfcomp[i].m_iSecondIndex].Value.GetpSliceFooter().Get(vTemp2));
			if (ret != M4_SUCCESS)	return M4_FALSE;
			ret2 = (m_pstfcomp[i].m_pfcomp((const m4VariantType *)&vTemp,(const m4VariantType *)&vTemp2,0)) ? M4_TRUE : M4_FALSE ;

		}
		// Si lo que hay que comparar es una columna con un valor especificado :
		else
		{
			ret2 = m_pstfcomp[i].m_pfcomp((const m4VariantType *)&vTemp,(const m4VariantType *)&(m_pstfcomp[i].m_vVariant),0) ? M4_TRUE : M4_FALSE ;
		}		
		
		if (ret2 != M4_TRUE) 	
			return M4_FALSE;

		
		i++;
	}

	if (m_bMustStop == M4_TRUE )
	{

		(m_poRegister->Item[m_pstfcomp[0].m_iIndex].Value.GetpSliceFooter().Get(vTemp));

		m_vVariantTemp = vTemp;

	}
	m_bAnyRegisterFound = M4_TRUE;

	return M4_TRUE;
}


m4return_t	ClRgAc_Base::GetValueByIndexPos (ClVectorVariants &ao_vTotVal){
	
	m4int32_t iNumRows;
	m4return_t ret;
	m4bool_t bOldWay=M4_FALSE;
	ClRgBoundedTotalizer oTotalizer;

	iNumRows = GetNumRows();	

	if (iNumRows){//leemos el número de tramos del primer item del registro
		//si han hecho un unflatten (old way), todos los items deben tener tramos
		//y por tanto vamos por el totalizador antiguo
		if (m_poRegister->Item[ (*m_paItemIndex)[0].m_ItemIndex].Value.Count()){
			bOldWay=M4_TRUE;
		}
	}
	//iniciamos el iterador del totalizador
	ret = m_oBoundedIterator.Init(m_idxFirst, m_idxSecond, m_poRegister->Index, m_iStartDateItemIndex, m_iEndDateItemIndex);

	//asignamos los items fecha inicio, fecha fin
	//m_iStartDateItemIndex 
	//m_iEndDateItemIndex 

	if ( bOldWay  || ret==M4_ERROR /*|| m_oBoundedIterator.Count()==1 */){ //si los indices de first, second son ambos nulos o algun item tiene tramos o solo hay un registro, usamos el totalizador de slice clásico:COMPATIBILIDAD
		for ( m4int32_t k = 0; k < iNumRows ; k++){
			ret = 	m_poRegister->Item[ (*m_paItemIndex)[k].m_ItemIndex].Value.GetpSliceFooter().Get (ao_vTotVal[k], (*m_paItemIndex)[k].m_TotType);
			if (ret != M4_SUCCESS ) break;
		}
	}
	else{//si tenemos que usar el nuevo totalizador
		//iniciamos el totalizador
		oTotalizer.Init(&m_oBoundedIterator);
		
		for ( m4int32_t k = 0; k < iNumRows ; k++){
			ret= m_oBoundedIterator.MoveItem( (*m_paItemIndex)[k].m_ItemIndex);
			if (ret!=M4_SUCCESS) break;

			ret = 	oTotalizer.Get(ao_vTotVal[k], (*m_paItemIndex)[k].m_TotType);
			if (ret != M4_SUCCESS ) break;
		}
		m_oBoundedIterator.End();
		//m_poRegister->Next();
		m_oBoundedIterator.Reset();
		
	}
	return ret;	
}




m4bool_t ClRgAc::Next(void)
{
	// Realmente el Begin nos mueve al primero valido desde donde estamos.
	return ClRgAc_Base::Begin();
}

/* Bug 0077048
Hay que implementar el count, que devuelve el número de registros
No vale el count, hay que iterar para ver los que pasan el filtro
*/
m4uint32_t	ClRgAc::Count( void )
{

	m4uint32_t	iResult = 0 ;
	m4bool_t	bIsNotEof = M4_FALSE ;


	bIsNotEof = ClRgAc_Base::Begin() ;

	while( bIsNotEof == M4_TRUE )
	{
		iResult++ ;
		bIsNotEof = ClRgAc_Base::Next() ;
	}

	return( iResult ) ;
}


int M4SearchVarStringInVarString(const m4VariantType *vTemp1,const m4VariantType *vTemp2,void* ai_pvAux)
{
	m4pchar_t pacTotal,pacParcial,pacTemp;
	m4int16_t iLenParcial,iLenTotal;
	
	if ((pacParcial=vTemp1->Data.PointerChar) == NULL) return M4_FALSE;
	if ((pacTotal=vTemp2->Data.PointerChar) == NULL)	return M4_FALSE;

	iLenParcial = strlen (pacParcial);
	iLenTotal = strlen (pacTotal);

	pacTemp = pacTotal ;
	while ((pacTemp=strstr(pacTemp,pacParcial))!=NULL)
	{
	
		if ( (*(pacTemp-1) == M4_ACUMULATE_TIS_SEPARATOR || (pacTemp) == pacTotal )	&& 
			(*(pacTemp + iLenParcial) == M4_ACUMULATE_TIS_SEPARATOR || pacTemp + strlen(pacParcial) == pacTotal + iLenTotal ))
		{
			return M4_TRUE;
		}
		pacTemp++;
	}

	return M4_FALSE;
}
void StFunctionCmp_t::Fill(ClItemIndex ai_Index,m4VariantType &ai_vVariant,m4CompareVariant_f ai_pfcomp,ClItemIndex ai_iSecondIndex)

{
		m_iIndex=ai_Index;
		m_vVariant=ai_vVariant;
		m_pfcomp=ai_pfcomp;
		m_iSecondIndex=ai_iSecondIndex;
}


//----------------------------------------------------------------------------------------
//Iterador para totalizador acotado en movimientos- para totalizar los slices en registros
//----------------------------------------------------------------------------------------


//-----------------
//Constructores
//-----------------
ClRgBoundedAc::ClRgBoundedAc(ClRegister *ai_poRegister, m4uint8_t ai_uiTotType)
	: m_idxStart(M4DM_EOF_INDEX)
	, m_idxEnd(M4DM_EOF_INDEX)
	, m_poRegister(ai_poRegister)
	, m_uiTotType(ai_uiTotType)
{
}

ClRgBoundedAc::ClRgBoundedAc(const ClRgBoundedAc &ai_roRgBoundedAc){
	m_poRegister=ai_roRgBoundedAc.m_poRegister;
	m_idxStart=ai_roRgBoundedAc.m_idxStart;
	m_idxEnd=ai_roRgBoundedAc.m_idxEnd;
	m_uiTotType=ai_roRgBoundedAc.m_uiTotType;
}


//-------------------------
//Especifico-Inicializacion
//-------------------------
m4return_t	ClRgBoundedAc::Init(ClItemIndex ai_idxFirst, ClItemIndex ai_idxSecond, ClRegisterIndex ai_iFirstRecord
								, ClItemIndex ai_iStartDateItemIndex, ClItemIndex ai_iEndDateItemIndex)
{
	m4VariantType	pk1, pk2, pkTemp;
	m4return_t		ret;

	m4VariantType	vStartDate;
	m4VariantType	vEndDate;
	m4VariantType	vUnitDate;
	m4double_t		dUnitDate;

	m_dDivider = 1;

	m_iStartDateItemIndex = ai_iStartDateItemIndex ;
	m_iEndDateItemIndex = ai_iEndDateItemIndex ;

	Index=ai_iFirstRecord;

	//Unit Date, lo sacamos del metacanal
	m_poRegister->GetpAccess()->GetpChannel()->UnitDate.Get( vUnitDate ) ;
	dUnitDate = vUnitDate.Data.DoubleData ;
		
	//si los indice son válidos
	if (ai_idxFirst != M4DM_EOF_INDEX && ai_idxSecond != M4DM_EOF_INDEX) {

		if (!m_poRegister->MoveTo(ai_iFirstRecord)){
			return M4_ERROR;
		}
		
		m_idxStart=ai_iFirstRecord;

		//Fecha inicio, fecha fin del valor
		if( m_iStartDateItemIndex != -1 && m_iEndDateItemIndex != -1 )
		{
			m_poRegister->Item[m_iStartDateItemIndex].Value.Get(vStartDate);
			m_poRegister->Item[m_iEndDateItemIndex].Value.Get(vEndDate);

			//Divisor para ponderar los valores
			m_dDivider = dUnitDate + vEndDate.Data.DoubleData - vStartDate.Data.DoubleData ;
		}

		//leemos el valor de iFirst e iSecond
		ret=m_poRegister->Item[ai_idxFirst].Value.Get(pk1);
		if (ret!=M4_SUCCESS){
			return M4_ERROR;
		}
		ret=m_poRegister->Item[ai_idxSecond].Value.Get(pk2);
		if (ret!=M4_SUCCESS){
			return M4_ERROR;
		}
	
		m_idxEnd=m_poRegister->Index;
		m_poRegister->Next();
		
		//mientras los valores sean los mismos incrementando
		while (m_poRegister->Index != M4DM_EOF_INDEX){
			
			ret=m_poRegister->Item[ai_idxFirst].Value.Get(pkTemp);
			if (ret!=M4_SUCCESS){
				return M4_ERROR;
			}
			if (pkTemp!=pk1){
				break;
			}
			else{
				ret=m_poRegister->Item[ai_idxSecond].Value.Get(pkTemp);
				if (ret!=M4_SUCCESS){
					return M4_ERROR;
				}
				if (pkTemp!=pk2){
					break;				
				}

				if( m_iStartDateItemIndex != -1 && m_iEndDateItemIndex != -1 )
				{
					m_poRegister->Item[m_iStartDateItemIndex].Value.Get(vStartDate);
					m_poRegister->Item[m_iEndDateItemIndex].Value.Get(vEndDate);

					//Divisor para ponderar los valores
					m_dDivider += dUnitDate + vEndDate.Data.DoubleData - vStartDate.Data.DoubleData ;
				}

				m_idxEnd=m_poRegister->Index;
				m_poRegister->Next();
			}
			
		}
		//nos guardamos los índices del rimer registro y último que cumple
		//m_idxEnd=m_poRegister->Index-1;

	}
	else{//si no son válidos, salimos con error
		return M4_ERROR;
	}
	m_poRegister->MoveTo(ai_iFirstRecord);
	return M4_SUCCESS;
}

void ClRgBoundedAc::Reset(void)
{
	m_idxStart = M4DM_EOF_INDEX;
	m_idxEnd = M4DM_EOF_INDEX;
}


//-------------------
//para iterar
//-------------------
m4bool_t ClRgBoundedAc::Begin(void)
{
	m_poRegister->MoveTo(m_idxStart);
	Index=m_poRegister->Index;
	return m_poRegister->Index != M4DM_EOF_INDEX;
}

m4bool_t ClRgBoundedAc::End(void)
{
	m_poRegister->MoveTo(m_idxEnd);
	Index=m_poRegister->Index;
	return m_poRegister->Index != M4DM_EOF_INDEX;
}

m4bool_t ClRgBoundedAc::Next(void)
{
	if (m_poRegister->Index < m_idxEnd) {
		m_poRegister->Next();
		Index=m_poRegister->Index;
		return M4_TRUE;
	}
	Index = M4DM_EOF_INDEX;
	return M4_FALSE;
}

m4uint32_t ClRgBoundedAc::Count(void)
{
	if (m_idxStart != M4DM_EOF_INDEX) {
		return m_idxEnd - m_idxStart + 1;
	} else {
		return 0;
	}
}

m4bool_t ClRgBoundedAc::MoveTo(const m4uint32_t ai_iIndex)
{
	if (ai_iIndex >= m_idxStart && ai_iIndex <= m_idxEnd) {
		
		m_poRegister->MoveTo(ai_iIndex);
		
		Index = m_poRegister->Index;
		
		return M4_TRUE;
	}
	return  M4_FALSE;
}
//-------------------
//Lectura de valores
//-------------------
m4return_t ClRgBoundedAc::MoveItem (ClItemIndex ai_idxItem){
	if (m_poRegister->Item.MoveTo(ai_idxItem)){
		m_idxItem=ai_idxItem;
		return M4_SUCCESS;
	}
	return M4_ERROR;
}

m4return_t ClRgBoundedAc::Get(m4VariantType &ao_vValue)
{
	return m_poRegister->Item[m_idxItem].Value.Get(ao_vValue);
}

m4return_t ClRgBoundedAc::GetWeightedValue(m4VariantType &ao_vValue)
{
	
	m4VariantType	vStartDate;
	m4VariantType	vEndDate;
	m4VariantType	vUnitDate;
	m4double_t		dUnitDate;
	m4double_t		dDividend;

	m_poRegister->Item[m_idxItem].Value.Get(ao_vValue);

	if( m_iStartDateItemIndex != -1 && m_iEndDateItemIndex != -1 )
	{
		//Unit Date, lo sacamos del metacanal
		m_poRegister->GetpAccess()->GetpChannel()->UnitDate.Get( vUnitDate ) ;
		dUnitDate = vUnitDate.Data.DoubleData ;	

		m_poRegister->Item[m_iStartDateItemIndex].Value.Get(vStartDate);
		m_poRegister->Item[m_iEndDateItemIndex].Value.Get(vEndDate);

		dDividend = dUnitDate + vEndDate.Data.DoubleData - vStartDate.Data.DoubleData;

		if (m_dDivider != dDividend)
		{
			ao_vValue.Data.DoubleData *= dDividend / m_dDivider ; 
		}
	}

	return M4_SUCCESS; 
}

