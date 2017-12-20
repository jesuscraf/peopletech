//priorStack.hpp
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                priorStack.cpp  
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the stack for dates slices with solve piority algorithm.
//
//
//==============================================================================
#include "priorstack.hpp"


//básicas
ClPriorSliceStack::ClPriorSliceStack(){
	m_dStartDate=0;
	m_dEndDate	=0;
	m_dUnit=0;
	m_bIsSorted	=M4_TRUE;
}



ClPriorSliceStack::ClPriorSliceStack(ClPriorSliceStack &rhs):m_oElements(rhs.m_oElements) {
	m_dStartDate	=rhs.m_dStartDate;
	m_dEndDate		=rhs.m_dEndDate;
	m_bIsSorted		=rhs.m_bIsSorted;
	//los elementos se copian por el constructor copia del storage
	
}



ClPriorSliceStack::~ClPriorSliceStack(){
}


//manejo
void ClPriorSliceStack::Init  (m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4date_t ai_dUnit, m4uint32_t ai_iSize){
	m_dStartDate	=ai_dStartDate;
	m_dEndDate		=ai_dEndDate;
	m_dUnit			=ai_dUnit;
	m_oElements.reserve(ai_iSize);
	m_bIsSorted		=M4_TRUE;
}

void ClPriorSliceStack::Clear (){
	m_dStartDate=0;
	m_dEndDate	=0;
	m_dUnit	=0;
	m_oElements.erase(m_oElements.begin(),m_oElements.end());
	m_bIsSorted		=M4_TRUE;
}

void ClPriorSliceStack::Clear (m4date_t ai_dStartDate,m4date_t ai_dEndDate, m4date_t ai_dUnit){
	m_dStartDate	=ai_dStartDate;
	m_dEndDate		=ai_dEndDate;
	m_dUnit			=ai_dUnit;
	m_oElements.erase(m_oElements.begin(),m_oElements.end());
	m_bIsSorted		=M4_TRUE;

}


m4return_t ClPriorSliceStack::AddSlice( const ClPriorSlice &ai_oElement ){
	
	if (ai_oElement.m_dEnd < m_dStartDate || ai_oElement.m_dStart > m_dEndDate) {
		return M4_SUCCESS; //no se mete y ya esta
	}
	
	ClPriorSlice oSlice;
	//miramos a ver si hay que cortar 
	oSlice.m_dStart	=max (ai_oElement.m_dStart,m_dStartDate);
	oSlice.m_dEnd	=min (ai_oElement.m_dEnd,m_dEndDate);
	oSlice.m_iPrior =ai_oElement.m_iPrior;

	m_bIsSorted		=M4_FALSE;
	m_oElements.push_back(oSlice);
	return M4_SUCCESS;
}

m4return_t ClPriorSliceStack::GetSlice( m4uint32_t ai_iPosition, ClPriorSlice &ai_oElement ) const {
	
	if (ai_iPosition>m_oElements.size()){ //comprobación de integridad
		return M4_ERROR;
	}
	ai_oElement=m_oElements[ai_iPosition];
	return M4_SUCCESS;
}

//operadores

ClPriorSliceStack& ClPriorSliceStack::operator=  (const ClPriorSliceStack& rhs){
	m_dStartDate	=rhs.m_dStartDate;
	m_dEndDate		=rhs.m_dEndDate;
	m_bIsSorted		=rhs.m_bIsSorted;
	m_dUnit			=rhs.m_dUnit;

	m_oElements.assign ( rhs.m_oElements.begin(), rhs.m_oElements.end() );

	return *this;
}

ClPriorSliceStack& ClPriorSliceStack::operator+= (const ClPriorSliceStack& rhs){
	
	ClPriorSliceStorageCIt it;

	for (it=rhs.m_oElements.begin();it!=rhs.m_oElements.end();++it){
		AddSlice ( *it );
	}

	return *this;
}

//algoritmos
m4return_t ClPriorSliceStack::Sort		(){
	if (!m_bIsSorted){
		sort(m_oElements.begin() , m_oElements.end() ) ;
		m_bIsSorted=M4_TRUE;
	}
	return M4_SUCCESS;
}

//Modo de uso:
//Se añaden todos los tramos que tengamos que meter (vienen de AV)
//Se van añadiendo todos los que ya tiene el item (con reduce)
//Al final nos quedarán aquellos que tengamos que generar

//--------------------------------------------------------------------------

//4 posibilidades entre el remain y el tramo de la pila
//cuando la prioridad de lo que hay en la pila es menor que la prioridad
//del tramo ya existente.

//a)  |----------------------| (iguales->quitamos el de la pila)
//    |----------------------|

//b)  |----------------------| (por la izq->adelantamos la fecha inicio)
//    |--------|

//c)  |----------------------| (por la derecha->retrasamos la fecha fin)
//              |------------|

//d)  |----------------------| (incluido->partimos )
//          |-----|

//--------------------------------------------------------------------------

m4return_t ClPriorSliceStack::Reduce	(const ClPriorSlice &ai_oSlice){

	
	ClPriorSlice oSlice;
	ClPriorSliceStorageIt it, itEnd;
	m4date_t dStartToUse,dEndToUse;
	m4date_t dRemainStart,dRemainEnd;

	dRemainStart=ai_oSlice.m_dStart;
	dRemainEnd=ai_oSlice.m_dEnd;

	//1.- Ordenamos
	Sort();

	//2.- si hay alguno que se superpone recortamos el segundo viendo si quedan a long. 0
	it=m_oElements.begin();
	
	//2.1 Lo que queda por resolver es lo que nos viene
	while (dRemainStart<=dRemainEnd){
	//2.2 mientras nos quede algo por resolver vamos buscando el tramo cuya fecha de inicio corresponde
		itEnd=m_oElements.end();
		if (it==itEnd) return M4_SUCCESS;
		
		oSlice=*it;
		
		if (oSlice.m_dStart>dRemainEnd) return M4_SUCCESS;

		while (oSlice.m_dEnd<dRemainStart){
			++it;
			if (it==itEnd) return M4_SUCCESS;
			oSlice=*it;
		}

		dStartToUse=max( oSlice.m_dStart, dRemainStart );
		dEndToUse=min( oSlice.m_dEnd, dRemainEnd );

		//2.3resolvemos prioridad y añadimos tramos si es pertienente
		if (ai_oSlice.m_iPrior < oSlice.m_iPrior){
			
			//hay que quitar algo de los que hay en la pila			
			
			//a)
			if (oSlice.m_dStart == dStartToUse && oSlice.m_dEnd == dEndToUse){
				//borramos el tramos
				m_oElements.erase(it);
			}
			else if (oSlice.m_dStart == dStartToUse){ //b)
				(*it).m_dStart=dEndToUse+m_dUnit;
			}
			else if (oSlice.m_dEnd == dEndToUse){  //c)
				(*it).m_dEnd=dStartToUse-m_dUnit;
			}
			else{ //caso d
				(*it).m_dEnd=dStartToUse-m_dUnit;
				m_oElements.insert(++it, ClPriorSlice(dEndToUse+m_dUnit, oSlice.m_dEnd, oSlice.m_iPrior) );
			}
		}
		
		//actualizamos lo que nos queda por mirar
		dRemainStart=dEndToUse+m_dUnit;
	}


	//fin de mientras, el siguiente tramo que buscaremos será mínimo el siguiente al tratado
	
	return M4_SUCCESS;
}

		

#ifdef _DEBUG

#include "m4stl.hpp"

void ClPriorSliceStack::DumpStack(ostream *os)
{
	ClPriorSliceStorageIt it=m_oElements.begin();

	while (it!=m_oElements.end()){
		(*it).Dump(os);
		++it;
	}
}

void ClPriorSlice::Dump(ostream *os)
{
	(*os)<<"StartDate = "<<m_dStart<<"\t EndDate = "<<m_dEnd<<"\t Prior = "<<(int)m_iPrior<<endl;
}

#endif

