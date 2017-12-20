 //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           vm
// File:             vmcycle.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             17/06/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines a class to detect cycles in graphs
//
//
//==============================================================================

#include "vmcycle.hpp"

#ifdef _DEBUG
	#include "m4stl.hpp"
#endif
/*Funcion que detecta si hay o no ciclos en un grafo dirigido.
 */

/*Descripción del Algoritmo:

  . Llenamos un array de arrays de enteros con los hijos de cada nodo y el grado
  . En la pila metemos los que tengan grado 0 y nodos_sin_grado_0 -= los que metamos
  . mientras la pila no este vacia
		Sacamos el del tope de la pila
		A los hijos : grado --
		Si algún hijo tiene ahora grado == 0 ->al tope de la pila, se marca como orden topológico i
			y nodos_sin_grado_0--
  . fin mientras
  . si nodos_sin_grado_0!=0 ->CICLO
*/
m4return_t ClVMCycle::DetectCyclesOrdering(void){


	
	if (CreateAdjacentList()!= M4_SUCCESS) return M4_ERROR;
	
	vectorInt	oZeroGrade;
	m4uint32_t	iRest=m_iCount;
	m4int32_t	iCurrent,iAdjacentToDecrement;
	m4uint32_t	i,j;
	oZeroGrade.reserve(m_iCount);


	for (i=0; i<m_iCount; ++i){
		if (m_oGrade[i]==0){
			oZeroGrade.push_back(i);
			m_oOrdering.push_back (i);
			--iRest;
		}
	}

	while (oZeroGrade.size()){
		iCurrent=oZeroGrade.back();
		oZeroGrade.pop_back();
		
		//para los adjacentes de iCurrent
		for (j=0;j<m_oAdjacent[iCurrent].size();++j){
			
			iAdjacentToDecrement=m_oAdjacent[iCurrent][j]; //Vemos cual es el adjacente j
			
			m_oGrade[iAdjacentToDecrement]--;	//le decrementamos le grado
			
			if(m_oGrade[iAdjacentToDecrement]==0){ //si el grado es cero->metemos los adjacentes en la pila
				oZeroGrade.push_back(iAdjacentToDecrement);
				m_oOrdering.push_back (iAdjacentToDecrement);
				--iRest;
			}
		}

	}
	if (iRest) return M4_ERROR; //CICLO


	return M4_SUCCESS;
}


/*
Los datos del grafo vienen de la forma:
 m_piGraph[i]=j significa que el apdre del nodo i es j
*/
/*función que calcula si ahy ciclos o no el determina el orden topológico*/
m4return_t	ClVMCycleSimple::CreateAdjacentList (void){

	m4uint32_t i,iStartSize;

	for (iStartSize=m_iCount,i=0; iStartSize; ++i,iStartSize>>=1);

	vectorInt temp;
	temp.reserve(i-1); 

	m_oAdjacent.clear();
	//m_oGrade.clear();

	for (i=0; i<m_iCount; ++i){//para que cada vector del vector no tenga de tamaño una página por defecto lo iniciamos
		m_oAdjacent.push_back(temp);
		m_oGrade[i]=0;
	}

	for (i=0; i<m_iCount; ++i){
		if (m_piGraph[i]>=0){
			m_oAdjacent[ m_piGraph[i] ].push_back(i);
			m_oGrade[i]++;
		}
	}

	return M4_SUCCESS;
}



#ifdef _DEBUG
void ClVMCycle::DumpAdjacentList(ostream &os){
	
	m4uint32_t i,j;
	
	
	for (i=0; i<m_oAdjacent.size(); i++){
		os<<i<<"(grado="<<m_oGrade[i]<<"): ";
		
		for (j=0;j<m_oAdjacent[i].size();++j){
			os<<m_oAdjacent[i][j]<<" ";
		}
		os<<endl;
		
	}

}

void ClVMCycle::DumpOrdering(ostream &os){
	m4uint32_t i;
	for (i=0;i<m_oOrdering.size();++i){
		os<<m_oOrdering[i]<<" ";
	}
	os<<endl;
}

#endif

