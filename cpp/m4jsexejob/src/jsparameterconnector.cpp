//## begin module%37E601A4037D.cm preserve=no
//## end module%37E601A4037D.cm

//## begin module%37E601A4037D.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%37E601A4037D.cp

//## Module: jsparameterconnector%37E601A4037D; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\version\src\jsparameterconnector.cpp

//## begin module%37E601A4037D.additionalIncludes preserve=no
//## end module%37E601A4037D.additionalIncludes

//## begin module%37E601A4037D.includes preserve=yes
//## end module%37E601A4037D.includes

// jsparameter
#include <jsparameter.hpp>
// jsparameterconnector
#include <jsparameterconnector.hpp>
//## begin module%37E601A4037D.declarations preserve=no
//## end module%37E601A4037D.declarations

//## begin module%37E601A4037D.additionalDeclarations preserve=yes
//## end module%37E601A4037D.additionalDeclarations


// Class ClJSParameterConnector 



ClJSParameterConnector::ClJSParameterConnector (ClJSParameter *ai_poOrigin, ClJSParameter *ai_poDestination)
  //## begin ClJSParameterConnector::ClJSParameterConnector%933158857.hasinit preserve=no
  //## end ClJSParameterConnector::ClJSParameterConnector%933158857.hasinit
  //## begin ClJSParameterConnector::ClJSParameterConnector%933158857.initialization preserve=yes
  //## end ClJSParameterConnector::ClJSParameterConnector%933158857.initialization
{
  //## begin ClJSParameterConnector::ClJSParameterConnector%933158857.body preserve=yes
	m_poOrigin = ai_poOrigin;
	m_poDestination = ai_poDestination;
  //## end ClJSParameterConnector::ClJSParameterConnector%933158857.body
}



//## Other Operations (implementation)
m4return_t ClJSParameterConnector::PropagateValue ()
{
  //## begin ClJSParameterConnector::PropagateValue%933158860.body preserve=yes
	m4return_t res         ;/* Resultado del método                           */
	M4ClString stTemporal  ;/* String para paso de valores de parámetros      */

	if (m_poOrigin->GetValue( stTemporal) == M4_SUCCESS)
	{
		res = m_poDestination->SetValue( stTemporal);
		if (res == M4_SUCCESS)
			m_poDestination->SetValueSaved(M4_FALSE);
	}	
	else
		res = M4_ERROR;

   return res;
  //## end ClJSParameterConnector::PropagateValue%933158860.body
}

// Additional Declarations
  //## begin ClJSParameterConnector%375BE8C903CE.declarations preserve=yes
  //## end ClJSParameterConnector%375BE8C903CE.declarations

//## begin module%37E601A4037D.epilog preserve=yes
//## end module%37E601A4037D.epilog
