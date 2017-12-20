//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             ClVmStepper.cpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    The stepper
//
//
//==============================================================================

#include "vmstepper.hpp"
#include "vmtypes.hpp"
#include "m4mdrt.hpp"
#include "access.hpp"
#include "clgeniod.hpp" // To Serialize



ClVMStepper::ClVMStepper( void )
{
	m_eMode = StepperModeNormal ;
	m_bActive = M4_FALSE ;
	m_bReenterInactive = M4_FALSE ;
}

ClVMStepper::~ClVMStepper ( void )
{
}



// ============================================================================
// Devuelve el BP si hay que para en el y sino nulo
// ============================================================================

ClBreakpoint*	ClVMStepper::CheckForBP( const m4VMState_t &ai_roState )
{

	m4return_t		iResult = M4_SUCCESS ;
	ClBreakpoint	*pAuxBP = NULL ;


	// Si es JIT o formula nula, salimos sin parar la ejecución
	if( ai_roState.m_poFormula.IsNull() == M4_TRUE || M4CL_HAS_STATE_EXE_JIT_FLAG( ai_roState.m_Flags ) )
	{
		return( NULL ) ;
	}


	ClBreakpoint	oCurrentBP ;


	// Rellenamos el bp con los datos pertinentes a partir del state
	oCurrentBP.Fill( ai_roState ) ;

	// Comprobamos que se deba parar
	pAuxBP = m_oBPs.Get( oCurrentBP ) ;

	if( pAuxBP != NULL && pAuxBP->GetActive() == M4_TRUE )
	{
		return( pAuxBP ) ;
	}

	return( NULL ) ;
}

	
// ============================================================================
// Primero se comprueba si estamos en un BP activo. Si es así se decrementa
// el BP y se para
// Sino se mira si estamos en modo StepInto. Siempre se para
// Sino se mira si estamos en modo StepOver. Se para si estamos en un nivel
// actual o padre
// Sino se mira si estamos en modo StepOut. Se para si estamos en un nivel
// padre
// ============================================================================

m4bool_t ClVMStepper::CheckForStop( const m4VMState_t &ai_roState )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4int32_t		iValue = 0 ;
	ClBreakpoint	*pAuxBP = NULL ;


	// Si es JIT o formula nula, salimos sin parar la ejecución
	if( ai_roState.m_poFormula.IsNull() == M4_TRUE || M4CL_HAS_STATE_EXE_JIT_FLAG( ai_roState.m_Flags ) )
	{
		return( M4_FALSE ) ;
	}


// Chequeo de breakpoint ======================================================

	pAuxBP = CheckForBP( ai_roState ) ;

	if( pAuxBP != NULL )
	{
		iValue = pAuxBP->GetValue() ;

		if( iValue > 0 )
		{
			iValue-- ;

			if( iValue == 0 )
			{
				m_oBPs.Remove( *pAuxBP ) ;
			}
			else
			{
				pAuxBP->SetValue( iValue ) ;
			}
		}

		return( M4_TRUE ) ;
	}


// Chequeo de StepInto ========================================================

	if( m_eMode == StepperModeStepInto )
	{
		return( M4_TRUE ) ;
	}
	
// Chequeo de StepOver ========================================================
	
	if( m_eMode == StepperModeStepOver )
	{
		if( ai_roState.m_iStepperType == StepperTypeActual || ai_roState.m_iStepperType == StepperTypeParent )
		{
			return( M4_TRUE ) ;
		}
	}

// Chequeo de StepOut =========================================================
	
	if( m_eMode == StepperModeStepOut )
	{
		if( ai_roState.m_iStepperType == StepperTypeParent )
		{
			return( M4_TRUE ) ;
		}
	}


	return( M4_FALSE ) ;
}


	
//---------------------------
//Serializacion
//---------------------------
m4return_t  ClVMStepper::Serialize(ClGenericIODriver& IOD){
	
	m4return_t ret=M4_SUCCESS;
	
	
	if (IOD.Write(m_bActive)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (IOD.Write(m_bReenterInactive)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (IOD.Write(m4int32_t(m_eMode))!=M4_SUCCESS){
		return M4_ERROR;
	}

	ret=m_oBPs.Serialize(IOD);	
	if (ret!=M4_SUCCESS) {
		return ret;
	}

	return ret;
}

m4return_t  ClVMStepper::DeSerialize (ClGenericIODriver& IOD){
	
	m4int32_t iMode;
	m4return_t ret=M4_SUCCESS;
	
	ret = IOD.Read(m_bActive);
	if (ret != M4_SUCCESS) return ret;

	ret = IOD.Read(m_bReenterInactive);
	if (ret != M4_SUCCESS) return ret;

	ret = IOD.Read(iMode);
	if (ret != M4_SUCCESS) return ret;
	m_eMode=eVMStepperMode(iMode) ;
	
	ret=m_oBPs.DeSerialize(IOD);	
	if (ret!=M4_SUCCESS) {
		return ret;
	}

	return ret;
}
