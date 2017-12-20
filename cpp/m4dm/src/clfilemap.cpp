//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           clfilemap.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             26 jan 98
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//   Map of Opened Files.
////
//==============================================================================

#include "clfilemap.hpp"




m4return_t	ClFileTable::CloseFiles (m4int32_t ai_iNumFiles )
{
	itFileTable itTemp = m_oFileTable.begin();

	while (ai_iNumFiles)
	{

		switch ( (*itTemp).second -> CloseFileWODeleting() )
		{
		case M4_SUCCESS:

			itTemp ++;
			ai_iNumFiles--;
			m_iNumOpenFiles--;
			break;

		case M4_WARNING:
			// El fichero ya estaba cerrado.

			itTemp++;
			break;

		case M4_ERROR:
///	
			return M4_ERROR;
			break;
		}
	}


	return M4_SUCCESS;


}

