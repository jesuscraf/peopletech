

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:               vmauto_ptr_array
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//   vmauto_ptr_array: auto pointer para arrays, sencillo a más no poder
////
//==============================================================================

template <class T>
class vmauto_ptr_array{

public:
	vmauto_ptr_array( T* ai_paData){
		m_tData=ai_paData;
	}
	~vmauto_ptr_array(){
		delete [] m_tData;
	}
	void Unbind(void){
		m_tData=0;
	}
private:
	T* m_tData;	
};
