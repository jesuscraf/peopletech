//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           vmlabel.cpp
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
//    Implementacion de la clase label. Esta clase es por si en un futuro en lugar
//    de usa etiqueta=cahr 255 cambia la estructura
////
//==============================================================================

#include "vmlabel.hpp"

#include "blocksynchronization.hpp"
//#include "syncro.hpp"

//Creamos las variables static
static ClMutex s_oVMMutStatusLabel(M4_TRUE);


m4return_t ClVMStatusLabel::SetLabel(m4pcchar_t ai_pszLabel){

	ClMutBlock VMMutSetBlock(s_oVMMutStatusLabel);

	strncpy (m_acLabel, ai_pszLabel, M4CL_MAX_STRING-1);
	m_acLabel[M4CL_MAX_STRING-1]=0; //pa porsi
	return M4_SUCCESS;

}


m4return_t ClVMStatusLabel::GetLabel(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize){

	ClMutBlock VMMutSetBlock(s_oVMMutStatusLabel);

	strncpy (ao_pszLabel, m_acLabel, ai_iSize-1);
	ao_pszLabel[ai_iSize-1]=0; //pa porsi
	return M4_SUCCESS;
}
