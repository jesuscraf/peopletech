//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Executer
// File:                FixedStack.h
// Project:        CLC
// Author:         Meta Software M.S. , S.A
// Date:            27 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    This module una pila de tamaño fijo (sin reserva dinámica)
////
//==============================================================================



#ifndef _FIXED_STACK_H
#define _FIXED_STACK_H


#include "m4glbres.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"

// EFV Support to aix 6.1
#include "chlog.hpp"


template <class T>
class tpFixedStack{

public:
	tpFixedStack(m4int32_t ai_iSize=M4CL_DEFAULT_STACK_SIZE, m4int32_t ai_maxsize=M4CL_DEFAULT_STACK_MAX_SIZE);
	virtual ~tpFixedStack();
	m4return_t Push(const T &ai_tElement);
	m4return_t FicticiusPush(m4int32_t ai_iNumElements);
	m4return_t FicticiusPop(m4int32_t ai_iNumElements);
	m4return_t Top(T &ao_tElement, m4int32_t ai_iElement=0)const; //cuantos elemntos por arriba Top=0
 	m4return_t Pop(void);
	m4return_t GetElement(T &ao_tElement,m4int32_t ai_iElement) const;
	m4return_t SetElement (const T &ai_tElement, m4int32_t ai_iPos);
	T *GetPointer (m4int32_t ai_iPos) const;
	m4return_t SetReference(m4int32_t ai_iRef);
	m4int32_t GetReference(void) const;

	m4int32_t GetPos(void);
	m4bool_t IsEmpty(void);

	void Dump(void);

    m4uint32_t GetSize (void) const { return m_iMax * sizeof (T); }

private:

	T* m_tData;
	m4int32_t m_iTop;
	m4int32_t m_iMax;
	m4int32_t m_iRef;
	m4int32_t m_iMaxAllowedSize;

	m4return_t Resize(m4int32_t ai_iSize);

};


template <class T>
tpFixedStack<T>::tpFixedStack(m4int32_t ai_iSize, m4int32_t ai_maxsize){

	m_tData=new T[ai_iSize];
	m_iTop=0;
	m_iMax=ai_iSize;
	m_iRef=0;
	m_iMaxAllowedSize=ai_maxsize;
}
	


template <class T>
tpFixedStack<T>::~tpFixedStack(){

	if (m_tData!=NULL) delete [] m_tData;
	m_tData=NULL;
	m_iTop=0;
}
	

template <class T>
m4return_t tpFixedStack<T>::Push(const T &ai_tElement){
	
	m4return_t ret;
	
	if(m_iTop==m_iMax){
		ret=Resize(m_iMax*2);
		if (ret!=M4_SUCCESS){
			g_oChLog<<BeginError ( M4_OBJGLB_CANT_RESIZE_STACK )<<EndLog;
			return ret;
		}
	}

	
	m_tData[m_iTop]=ai_tElement;
	m_iTop++;
	ret=M4_SUCCESS;
	
	return ret;
}
	

template <class T>
m4int32_t tpFixedStack<T>::GetPos(void){
	
	return m_iTop-1;
}
	


template <class T>
m4return_t tpFixedStack<T>::Top(T &ao_tElement, m4int32_t ai_iElement) const { //cuantos elemntos por arriba Top=0

	m4return_t ret;
	if (m_iTop-ai_iElement > 0)	{
		ao_tElement=m_tData[m_iTop-ai_iElement-1];
		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}
	return ret;
}
	

template <class T>
m4return_t tpFixedStack<T>::Pop(void){
	
	m4return_t ret;

	if (m_iTop>0){ //hay elementos
		m_iTop--;
		//si el top se queda por debajo de  la ref, actualizamos la referencia
	//*	if (m_iTop>0) {//no era el último
	//*		m_iRef=(m_iRef>m_iTop-1 ? m_iTop-1: m_iRef); 
	//*	}
	//*	else m_iRef=0;
		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}

	return ret;
}
	


template <class T>
m4return_t tpFixedStack<T>::FicticiusPop(m4int32_t ai_iNumElements){
	
	m4return_t ret;

	if (m_iTop>=ai_iNumElements){ //hay elementos
		m_iTop-=ai_iNumElements;
		//si el top se queda por debajo de  la ref, actualizamos la referencia
//*		if (m_iTop>0) {//no era el último
//*			m_iRef=(m_iRef>m_iTop-1 ? m_iTop-1: m_iRef); 
//*		}
//*		else m_iRef=0;

		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}

	return ret;
}



template <class T>
m4return_t tpFixedStack<T>::FicticiusPush(m4int32_t ai_iNumElements){
	
	m4return_t ret;

	while (m_iTop+ai_iNumElements>m_iMax){
		ret=Resize(m_iMax*2);
		if (ret!=M4_SUCCESS){
			g_oChLog<<BeginError ( M4_OBJGLB_CANT_RESIZE_STACK )<<EndLog;
			return ret;
		}
	}
	m_iTop+=ai_iNumElements;
	ret=M4_SUCCESS;
	return ret;
}

	

template <class T>
m4return_t tpFixedStack<T>::GetElement( T &ao_tElement, m4int32_t ai_iPos) const {
	
	m4return_t ret;

	if (ai_iPos+m_iRef<m_iTop && ai_iPos+m_iRef>=0){
		ao_tElement=m_tData[ai_iPos+m_iRef];
		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}
	return ret;

}
	

template <class T>
m4return_t tpFixedStack<T>::SetElement (const T &ai_tElement, m4int32_t ai_iPos ){
	
	m4return_t ret;

	if (ai_iPos+m_iRef>=0 && ai_iPos+m_iRef<m_iTop){
		m_tData[ai_iPos+m_iRef]=ai_tElement;
		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}
	return ret;
}

	

template <class T>
T *tpFixedStack<T>::GetPointer (m4int32_t ai_iPos) const {
	
	

	if (ai_iPos+m_iRef>=0 && ai_iPos+m_iRef<m_iTop){
		return m_tData+ai_iPos+m_iRef;
		
	}
	return NULL;

}


template <class T>
m4return_t tpFixedStack<T>::SetReference(m4int32_t ai_iRef){
	
	m4return_t ret;
	
	//if (ai_iRef<m_iTop && ai_iRef>=0){
	if (ai_iRef>=0 && ai_iRef <=m_iMax){
		m_iRef=ai_iRef;
		ret=M4_SUCCESS;
	}
	else{
		ret=M4_ERROR;
	}

	return ret;
}



template <class T>
m4int32_t tpFixedStack<T>::GetReference(void) const{
	
	return m_iRef;
}



template <class T>
m4return_t tpFixedStack<T>::Resize(m4int32_t ai_iSize){
	
	m4return_t ret;
	T *TempBase;
	//m4int32_t iTemp;
	
	if (ai_iSize > m_iMaxAllowedSize){ //no podemos redimensionar
		//SETCODEF (M4_CH_VM_INSUFICIENT_SPACE, ERRORLOG, "Can't resize stack to %d. It exceeds higher limit %d", ai_iSize, m_iMaxAllowedSize);
		//g_oChLog<<BeginError ( M4_OBJGLB_CANT_RESIZE_STACK )<<EndLog;
		return M4_ERROR;
	}

	TempBase=new T[ai_iSize];
	if (TempBase!=NULL) { //hay sitio
		/*iTemp=0;
		while (iTemp<m_iTop){ //copiamos elemento a elemento
			TempBase[iTemp]=m_tData[iTemp];
			iTemp++;
		}*/
		//en lugar du usar el operador = para cadda elemento usamos un memcopy (por rapidez)
		//Esto impide usar este template con clases que tengan redefinido el op. = de distinta forma
		//que el de defecto (copair memoria)
		memcpy (TempBase, m_tData, sizeof (T) * m_iTop);

		m_iMax=ai_iSize;
		if (m_tData!=NULL) delete [] m_tData;
		m_tData=TempBase;
		ret=M4_SUCCESS;
#ifdef _DEBUG
        g_oChLog<<BeginDebug(M4_OBJGLB_TRACE_VM_L1)<<"Insuficient space in stack. Resizing it to "<<LogCat<<ai_iSize<<EndLog;
#endif
	}
	else {
#ifdef _DEBUG
        g_oChLog<<BeginError(M4_OBJGLB_INTERNAL_ERROR_WITH_INFO)<<__LINE__<< __FILE__<<"Not enough memory while resizing stack to "<<LogCat<<ai_iSize<<EndLog;
#endif
		ret=M4_ERROR;
	}

	return ret;
}


template <class T>
m4bool_t tpFixedStack<T>::IsEmpty(void){
	
	if (m_iTop>0){ //hay elementos
		return M4_FALSE;
	}
	else{
		return M4_TRUE;
	}
	
}


template <class T>
void tpFixedStack<T>::Dump(void){
	 
	m4int32_t i,ref;

	ref=GetReference();
	for (i=0;i<m_iTop;i++){
	
		if (i==ref)
			cerr<<m_tData[i]<<"<---Referencia"<<endl;
		else
			cerr<<m_tData[i]<<endl;

	}

}




#endif 
