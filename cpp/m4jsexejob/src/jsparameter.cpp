//## begin module%37E601CC0258.cm preserve=no
//## end module%37E601CC0258.cm

//## begin module%37E601CC0258.cp preserve=no
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
//## end module%37E601CC0258.cp

//## Module: jsparameter%37E601CC0258; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\src\jsparameter.cpp

//## begin module%37E601CC0258.additionalIncludes preserve=no
//## end module%37E601CC0258.additionalIncludes

//## begin module%37E601CC0258.includes preserve=yes
#include <jsparameterconnector.hpp>
//## end module%37E601CC0258.includes

// m4string
#include <m4string.hpp>
// jsparameter
#include <jsparameter.hpp>


//## begin module%37E601CC0258.declarations preserve=no
//## end module%37E601CC0258.declarations

//## begin module%37E601CC0258.additionalDeclarations preserve=yes
//## end module%37E601CC0258.additionalDeclarations


// Class ClJSParameter 










ClJSParameter::ClJSParameter (M4ClString &ai_stName, M4ClString &ai_stLocalName, eParameterType ai_eType, eParameterFlowType ai_eFlowType)
  //## begin ClJSParameter::ClJSParameter%938766923.hasinit preserve=no
  //## end ClJSParameter::ClJSParameter%938766923.hasinit
  //## begin ClJSParameter::ClJSParameter%938766923.initialization preserve=yes
  //## end ClJSParameter::ClJSParameter%938766923.initialization
{
  //## begin ClJSParameter::ClJSParameter%938766923.body preserve=yes
	m_stName = ai_stName;
	m_stLocalName = ai_stLocalName;
	m_eType = ai_eType;
	m_eFlowType = ai_eFlowType;
	m_bIsValueSaved = M4_TRUE;
  //## end ClJSParameter::ClJSParameter%938766923.body
}


ClJSParameter::~ClJSParameter ()
{
  //## begin ClJSParameter::~ClJSParameter%939632584.body preserve=yes
	ConnectorList::iterator iterador ;/* Iterador sobre la lista de conectores */

	iterador = m_lConnectors.begin();

	while ( iterador != m_lConnectors.end() )
	{
		delete (* iterador);
		iterador ++;
	}

	m_lConnectors.clear();

  //## end ClJSParameter::~ClJSParameter%939632584.body
}



//## Other Operations (implementation)
m4return_t ClJSParameter::AddConnector (ClJSParameterConnector *ai_poConnector)
{
  //## begin ClJSParameter::AddConnector%933158864.body preserve=yes
	m_lConnectors.insert( m_lConnectors.begin(), ai_poConnector );

   return M4_SUCCESS;
  //## end ClJSParameter::AddConnector%933158864.body
}

m4return_t ClJSParameter::PropagateValue ()
{
  //## begin ClJSParameter::PropagateValue%939135439.body preserve=yes
	ConnectorList::iterator iterador;
	m4uint_t uiNumErrores = 0   ;/* Contador de propagaciones erróneas        */
	m4return_t res = M4_SUCCESS ;/* Resultado del método							  */

	for (iterador = m_lConnectors.begin(); iterador != m_lConnectors.end(); iterador ++)
	{
		if ( (*iterador)->PropagateValue() != M4_SUCCESS)
			uiNumErrores ++;
	}

	if (uiNumErrores > 0) 
		res = M4_WARNING;

	return res;
  //## end ClJSParameter::PropagateValue%939135439.body
}

m4return_t ClJSParameter::SetValue (const M4ClString &ai_stValue)
{
  //## begin ClJSParameter::SetValue%933158863.body preserve=yes
	m_stValue = ai_stValue;

	PropagateValue();

   return M4_SUCCESS;
  //## end ClJSParameter::SetValue%933158863.body
}

m4return_t ClJSParameter::GetValue (M4ClString &ao_stValue)
{
  //## begin ClJSParameter::GetValue%933158865.body preserve=yes
	ao_stValue = m_stValue;

   return M4_SUCCESS;
  //## end ClJSParameter::GetValue%933158865.body
}

m4return_t ClJSParameter::SetName (const M4ClString &ai_stName)
{
  //## begin ClJSParameter::SetName%939045075.body preserve=yes
	m_stName = ai_stName;

	return M4_SUCCESS;
  //## end ClJSParameter::SetName%939045075.body
}

m4return_t ClJSParameter::GetName (M4ClString &ao_stName)
{
  //## begin ClJSParameter::GetName%939045076.body preserve=yes
	ao_stName = m_stName;

	return M4_SUCCESS;
  //## end ClJSParameter::GetName%939045076.body
}

m4return_t ClJSParameter::GetLocalName (M4ClString &ao_stLocalName)
{
  //## begin ClJSParameter::GetLocalName%939045079.body preserve=yes
	ao_stLocalName = m_stLocalName;
	return M4_SUCCESS;
  //## end ClJSParameter::GetLocalName%939045079.body
}

m4return_t ClJSParameter::GetType (eParameterType &ao_eType)
{
  //## begin ClJSParameter::GetType%939045080.body preserve=yes
	ao_eType = m_eType;
	return M4_SUCCESS;
  //## end ClJSParameter::GetType%939045080.body
}

m4return_t ClJSParameter::GetFlowType (eParameterFlowType &ao_eFlowType)
{
  //## begin ClJSParameter::GetFlowType%939045081.body preserve=yes
	ao_eFlowType = m_eFlowType;
	return M4_SUCCESS;
  //## end ClJSParameter::GetFlowType%939045081.body
}

m4return_t ClJSParameter::GetTypeAsString (M4ClString &ao_stParamType)
{
  //## begin ClJSParameter::GetTypeAsString%941737615.body preserve=yes
  m4return_t res = M4_SUCCESS;
  
	switch (m_eType)
	{
		case TypeNumber:
			ao_stParamType = "NUMBER";
			break;
		case TypeString:
	   	ao_stParamType = "STRING";
		   break;
		case TypeDate:
			ao_stParamType = "DATE";
			break;
		default:
			res = M4_ERROR;
			break;
	}
		
	return res;
  //## end ClJSParameter::GetTypeAsString%941737615.body
}

ostream & operator << (ostream &aio_ostream, ClJSParameter &ai_oParameter)
{
  //## begin ClJSParameter::operator <<%939045078.body preserve=yes
	M4ClString pStCadena          ;
	ClJSParameter::eParameterType eTipo         ;
	ClJSParameter::eParameterFlowType eTipoFlujo;

	if ( aio_ostream.opfx() )
	{
		ai_oParameter.GetName( pStCadena);
		aio_ostream << pStCadena << " (" ;
		ai_oParameter.GetLocalName( pStCadena);
		aio_ostream << pStCadena << ") - ";

		ai_oParameter.GetType( eTipo);

		switch (eTipo)
		{
			case ClJSParameter::TypeNumber:
				aio_ostream << "Numero - ";
				break;
			case ClJSParameter::TypeString:
				aio_ostream << "Cadena - ";
				break;
			case ClJSParameter::TypeDate:
				aio_ostream << "Fecha  - ";
				break;
			default:
				aio_ostream << "Desconocido - ";
				break;
		}

		ai_oParameter.GetFlowType( eTipoFlujo);

		switch (eTipoFlujo)	
		{
			case ClJSParameter::FlowInput:
				aio_ostream << "Entrada = ";
				break;
			case ClJSParameter::FlowOutput:
				aio_ostream << "Salida  = ";
				break;
			default:
				aio_ostream << "Desconocido = ";
				break;
		}

		ai_oParameter.GetValue( pStCadena);
		if (!(pStCadena.empty()))
		   aio_ostream << pStCadena;
		
		aio_ostream << endl;
		aio_ostream.osfx();
	}

	return aio_ostream;
  //## end ClJSParameter::operator <<%939045078.body
}

m4bool_t ClJSParameter::IsValueSaved ()
{
  //## begin ClJSParameter::IsValueSaved%941561493.body preserve=yes
  return m_bIsValueSaved;
  //## end ClJSParameter::IsValueSaved%941561493.body
}

m4return_t ClJSParameter::SetValueSaved (m4bool_t ai_bIsValueSaved)
{
  //## begin ClJSParameter::SetValueSaved%941619583.body preserve=yes
	m_bIsValueSaved = ai_bIsValueSaved;

	return M4_SUCCESS;
  //## end ClJSParameter::SetValueSaved%941619583.body
}

// Additional Declarations
  //## begin ClJSParameter%375BE7E0031E.declarations preserve=yes
  //## end ClJSParameter%375BE7E0031E.declarations

//## begin module%37E601CC0258.epilog preserve=yes
//## end module%37E601CC0258.epilog
