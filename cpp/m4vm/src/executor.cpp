

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             executor.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    class executor, a vmachine wrapper
//
//
//==============================================================================

#include "executor.hpp"

#include "exertinfo.hpp"
#include "vmtypes.hpp"
#include "vmres.hpp"
#include "vmachin2.h"

#include "access.hpp"
#include "channel.hpp"
#include "item.hpp"
#include "m4mdrt.hpp"
#include "node_knl.hpp"
#include "chlog.hpp"

#include "cldmcontext.hpp"
#include "iexecadaptor.hpp"
#include "m4mdfac.hpp"



ClExecutor::ClExecutor ():IExecutor (m_Stack) {

	// inicialización del recubrimiento de la pila:
    m_poVMachine= new ClVMachine2;
	m_Stack.Init (m_poVMachine);
	m_bInit=M4_FALSE;
	m_poContext1=NULL;
	m_poContext2=NULL;
	m_iInsideCallBack=0;
}

ClExecutor::~ClExecutor ()
{
	delete m_poVMachine;
}


m4return_t ClExecutor::Init (ClVMCSContext *ai_poContext1, ClVMCSContext *ai_poContext2/*=0*/ ) 
{
	m_bInit=M4_TRUE;
	m_poVMachine->SetContext(ai_poContext1);
	
	m_poContext1=ai_poContext1;
	
	if (m_poContext1->IsSM()){
		m_poContext2=ai_poContext2;
	}
	else{
		m_poContext2=0;
	}

	return M4_SUCCESS;
}


ClVMStatisticsWrapper *ClExecutor::GetSttWrap(void){

	if (!m_bInit) return 0;
	return m_poVMachine->GetVMCSContext()->GetSttWrap();
}

//-------------------------
//Serializaciones
//-------------------------
m4return_t ClExecutor::Serialize(ClGenericIODriver& IOD)
{
	if (!m_bInit) return M4_ERROR;
    return m_poVMachine->Serialize(IOD);
}

m4return_t ClExecutor::DeSerialize(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager)
{
	if (!m_bInit) return M4_ERROR;
    return m_poVMachine->DeSerialize(IOD, ai_pChannelManager);
}



//------------------------
//Fechas
//------------------------
m4return_t ClExecutor::GetExecutionDate(m4VariantType &ao_var){

	m4VMState_t temp;
	
	if (!m_bInit) return M4_ERROR;
	
	if (IsRunning (temp)){
		ao_var.Data.DoubleData=temp.m_dDate;
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_SUCCESS;
	}
	else{
		ao_var.Data.DoubleData=0.0; //de momento 0
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_ERROR;
	}

}

m4return_t ClExecutor::GetStartDate(m4VariantType &ao_var){
	
	m4VMState_t temp;

	if (!m_bInit) return M4_ERROR;

	if (IsRunning (temp)){
		ao_var.Data.DoubleData=temp.m_dBeginDate;
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_SUCCESS;
	}
	else{
		ao_var.Data.DoubleData=0.0; //de momento 0
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_ERROR;
	}
}

m4return_t ClExecutor::GetEndDate(m4VariantType &ao_var){

	m4VMState_t temp;

	if (!m_bInit) return M4_ERROR;

	if (IsRunning (temp)){
		ao_var.Data.DoubleData=temp.m_dEndDate;
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_SUCCESS;
	}
	else{
		ao_var.Data.DoubleData=0.0; //de momento 0
		ao_var.Type=M4CL_CPP_TYPE_DATE;
		return M4_ERROR;
	}

}

//-------------------------
//Para la ejecución
//-------------------------

m4return_t	ClExecutor::Call(ClItem *ai_poItem, m4int32_t ai_iArgs/*=-1*/, m4bool_t ai_bEnableDebug/*=M4_FALSE*/, m4bool_t ai_bNoInsiteRec/*=M4_FALSE*/){

	m4VMState_t oState;
	m4VariantType aux;
	m4return_t ret;
	m4bool_t bActive=M4_FALSE;
	m4bool_t bStepper;
	m4bool_t bReenterInactive;
	ClVMStepper *poStepper = NULL ;


	if (!m_bInit) return M4_ERROR;

	
	if (ai_poItem == NULL || (ai_poItem->GetIndex() == M4DM_EOF_INDEX) ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "call");
		return ( M4_ERROR );
	}

	m4int32_t n = Stack.Count ();
	m4int32_t iNumArgs;

	if (ai_poItem->ItemDef.VariableArguments ()==1){
		
		if (ai_iArgs != -1) { //nos indican a pelo el número de argumetnos
			iNumArgs=ai_iArgs;
		}
		else{ //nos fiamos de la pila->error si hay reentrada
			iNumArgs=n;
		}
		//si los argumentos que vamos a usar en el item de arg. variables
		//bien los que nos dicen, bien todos, son menos que los fijos
		//que debe tener el item->le soltamos un error
		if (iNumArgs <ai_poItem->ItemDef.NumberOfArguments() ){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_ITEM_CALL_PARAM_NUM) <<ai_poItem->ItemDef.Id() <<(m4uint32_t)ai_poItem->ItemDef.NumberOfArguments ()<<n<<EndLog;
			return( M4_ERROR ) ;
		}
	}
	else{ //de argumetnso fijos
		iNumArgs=ai_poItem->ItemDef.NumberOfArguments() ;
	}

	//comprobamos que el número de arg. sea correcto
    // jesus. Lo pongo para comprobar los argumentos

    M4_ASSERT (iNumArgs< 256); // para que el casting de luego no dé problemas

	
	if( n < iNumArgs )
    {
		g_oChLog<<BeginError(M4_CH_VM_WRONG_ITEM_CALL_PARAM_NUM) <<ai_poItem->ItemDef.Id() <<(m4uint32_t)ai_poItem->ItemDef.NumberOfArguments ()<<n<<EndLog;
        return( M4_ERROR ) ;
    }

//dates:
    // execution date
	ai_poItem->GetpAccess()->ExecutionDate.Get (aux);
    oState.m_dDate = aux.Data.DoubleData;
     
    // start date
	ai_poItem->GetpAccess()->ExecutionStartDate.Get (aux);
    oState.m_dBeginDate = aux.Data.DoubleData;

    // end date
	ai_poItem->GetpAccess()->ExecutionEndDate.Get (aux);
    oState.m_dEndDate = aux.Data.DoubleData;


//seguimos rellenando el estado:
    // hitem
    oState.m_hItem = ai_poItem->ItemDef.GetHandle ();

    // irule
    oState.m_iRule = 0;

    // los flags a cero salvo si nos dicen que ya se ejecutaron los insite
    oState.m_Flags = 0;

	if( ai_bNoInsiteRec == M4_TRUE )
	{
		M4CL_SET_STATE_NO_INSITE_FLAG_REC( oState.m_Flags ) ;
	}

    // access
    oState.m_poAccess = ai_poItem->GetpAccess();

    //AccessRecordSet:
    ClNodeIndex ni = ai_poItem->GetpNode_ExtInfo ()->Index;
    oState.m_poRecordSet = &(oState.m_poAccess->Node.Get (ni)->RecordSet);

    // indice del registro
    oState.m_oRegisterIndex = ai_poItem->GetRegisterIndex ();

	//Si estamos en SM (y es un ejecutor generico: con dos contextos)
	//hay que poner el contexto según el item esté en un channel manager cliente o server
	if (m_poContext1->IsSM() && m_poContext2){
		
		//ponemos el contexto según sea el item
		if (oState.m_poAccess->GetpChannel()->GetpChannelManager()->GetDMContext()->ExecutingInClient()){
			m_poVMachine->SetContext(m_poContext1);//cliente
		}
		else{
			m_poVMachine->SetContext(m_poContext2);//server
		}
	}

	if (!ai_bEnableDebug){

		poStepper = GetpStepper() ;
		bStepper=poStepper->GetActive();

		if( bStepper == M4_TRUE )
		{
			bReenterInactive=poStepper->GetReenterInactive();
			poStepper->SetActive(M4_FALSE);

			if (m_poVMachine->GetStateStack()->GetPos()!=-1){
				poStepper->SetReenterInactive(M4_TRUE);
			}
		}
	}

	// La ejecución se realiza en medio de la señalización de ejecución de VM
	m_poVMachine->GetVMCSContext()->NotifyVMExecution(M4_TRUE);
    ret= m_poVMachine->Execute(oState, (m4uint16_t)iNumArgs );
	m_poVMachine->GetVMCSContext()->NotifyVMExecution(M4_FALSE);

#ifdef _WINDOWS
	/*
	Ponemos la callback al stepper en un sólo punto
	*/
	if( ret == M4CL_EXECUTION_INTERRUPTION )
	{
		long (CALLBACK *pfCallBack)(void) = NULL ;

		ClAccess *poAccess = ai_poItem->GetpAccess() ;

		if( poAccess != NULL )
		{
			ClChannel *poChannel = poAccess->GetpChannel() ;

			if( poChannel != NULL )
			{
				ClChannelManager *poChannelManager = poChannel->GetpChannelManager() ;

				if( poChannelManager != NULL )
				{
					pfCallBack = (long(CALLBACK *)(void)) poChannelManager->GetCallBack() ;
				}
			}
		}

		if( pfCallBack != NULL )
		{
			m_iInsideCallBack++;
			long lResult = (*pfCallBack)() ;
			m_iInsideCallBack--;

			ret = (m4return_t) lResult ;
		}
	}
#endif

	if (!ai_bEnableDebug){

		// Restauramos el modo de stepper
		if( bStepper == M4_TRUE )
		{
			poStepper->SetActive(M4_TRUE);
			poStepper->SetReenterInactive(bReenterInactive);
		}
	}

	return ret;

}


//Abort
m4return_t	ClExecutor::Abort(void){

	m4VMState_t temp;
	m4bool_t bIsRunning;
	m4return_t ret;
	
	if (!m_bInit) return M4_ERROR;

	bIsRunning=IsRunning (temp);

	if (!bIsRunning){ //si no esta embarazada, el aborto es sencillo
		DUMP_CHLOG_WARNINGF(M4_CH_VM_NOT_RUNNING, "abort");
		return M4_SUCCESS;
	}

	//acabar transaccion
	ret = m_poVMachine->RecoverFromError(temp.m_poAccess);

	if (ret!=M4_ERROR){ //no se puede recuperar del error
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_RECOVERED,">2");
		return M4_WARNING; //nos hemos conseguido recuperar de la transaccion->le avisamos con un warning
	}
	return M4_SUCCESS; //abortamos la ejecución bien (no se recupero de la transacción)
}


// ============================================================================
// El continue fija el estado actual de la pila
// Después continua la ejecución
// Al acabar devuelve el estado del stepper y de la pila a normal
// ============================================================================
m4return_t	ClExecutor::_Continue(void)
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){
		m_poVMachine->SetStepperStates() ;

		m4return_t ret=Execute() ; //que siga

		m_poVMachine->ResetStepper() ;
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}

m4return_t	ClExecutor::Continue( void )
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){
		GetpStepper()->SetMode( StepperModeNormal ) ;
		m4return_t ret=_Continue();
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}

// ============================================================================
// Continua la ejecución hasta la siguiente linea
// Lo que hace es establecer el modo y continuar
// ============================================================================
m4return_t	ClExecutor::StepInto( void )
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){
		GetpStepper()->SetMode( StepperModeStepInto ) ;
		m4return_t ret=_Continue();
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}

// ============================================================================
// Continua la ejecución hasta la siguiente linea de la función
// Lo que hace es establecer el modo y continuar
// ============================================================================
m4return_t	ClExecutor::StepOver( void )
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){
		GetpStepper()->SetMode( StepperModeStepOver ) ;
		m4return_t ret=_Continue();
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}

// ============================================================================
// Continua la ejecución hasta salir de la función
// Lo que hace es establecer el modo y continuar
// ============================================================================
m4return_t	ClExecutor::StepOut( void )
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){
		GetpStepper()->SetMode( StepperModeStepOut ) ;
		m4return_t ret=_Continue();
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}


// ============================================================================
// Continua la ejecución hasta el cursor
// El cursor viene dado por los parámetros de un breakpoint
// Lo que hace es establecer el breakpoint con un valor de 1 y continuar
// Al acabar, en cualquier caso borra el breakpoint
// ============================================================================
m4return_t	ClExecutor::RunToCursor( ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory )
{
	if (!m_bInit) return M4_ERROR;

	if (IsRunning ()){

		ClVMStepper *poStepper = GetpStepper() ;

		poStepper->SetMode( StepperModeNormal ) ;
		ai_oBP.SetValue( 1 ) ;
		AddBreakPoint( ai_oBP, ai_poFactory ) ;

		m4return_t ret=_Continue();

		poStepper->GetBPs().Remove( ai_oBP ) ;
		return ret;
	}
	else {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "continue");
		return M4_ERROR;
	}
}


//Execute
m4return_t	ClExecutor::Execute(void){

	if (!m_bInit) return M4_ERROR;

	m_poVMachine->RegisterUserAuditEvents() ;

	return m_poVMachine->Execute() ; //que siga

}


// ============================================================================
// Añade un breakpoint en la siguiente línea válida
// Devuelve la línea en la que puso el breakpoint
// Si no ha podido ponerlo devuelve -1
// ============================================================================
m4int32_t	ClExecutor::AddBreakPoint( ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4int32_t	iLine = -1 ;


	iLine = NextValidBreakPoint( ai_oBP, ai_poFactory ) ;

	if( iLine != -1 )
	{
		ai_oBP.SetLine( iLine ) ;

		iResult = GetpStepper()->GetBPs().Add( ai_oBP ) ;

		if( iResult != M4_SUCCESS )
		{
			return( -1 ) ;
		}
	}

	return( iLine ) ;
}


// ============================================================================
// Borra el breakpoint de la siguiente línea válida
// Devuelve la línea de la que se borró el breakpoint
// Si no ha podido borrar devuelve -1
// ============================================================================
m4int32_t	ClExecutor::DeleteBreakPoint( ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory)
{

	m4return_t			iResult = M4_SUCCESS ;
	m4int32_t			iLine = -1 ;
	ClBreakpointList&	rBPs = GetpStepper()->GetBPs() ;


	/* Bug 0077747
	Si existe un BP en esta linea lo borramos aunque no sea una línea válida
	*/
	if( rBPs.Get( ai_oBP ) != NULL )
	{
		iResult = GetpStepper()->GetBPs().Remove( ai_oBP ) ;

		if( iResult != M4_SUCCESS )
		{
			return( -1 ) ;
		}

		return( ai_oBP.GetLine() ) ;
	}


	iLine = NextValidBreakPoint( ai_oBP, ai_poFactory ) ;

	if( iLine != -1 && iLine != m4int32_t( ai_oBP.GetLine() ) )
	{
		ai_oBP.SetLine( iLine ) ;

		iResult = rBPs.Remove( ai_oBP ) ;

		if( iResult != M4_SUCCESS )
		{
			return( -1 ) ;
		}
	}

	return( iLine ) ;
}


// ============================================================================
// Cambia el breakpoint de la siguiente línea válida
// Si no existe un breakpoint lo añade
// Si existe un breakpoint lo quita
// Devuelve la línea en la que puso o quitó el breakpoint
// Si no ha podido ponerlo o quitarlo devuelve -1
// ============================================================================
m4int32_t	ClExecutor::ToggleBreakPoint( ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4int32_t	iLine = -1 ;


	ClBreakpointList&	rBPs = GetpStepper()->GetBPs() ;


	/* Bug 0077747
	Si existe un BP en esta linea lo borramos aunque no sea una línea válida
	*/
	if( rBPs.Get( ai_oBP ) != NULL )
	{
		iResult = GetpStepper()->GetBPs().Remove( ai_oBP ) ;

		if( iResult != M4_SUCCESS )
		{
			return( -1 ) ;
		}

		return( ai_oBP.GetLine() ) ;
	}


	iLine = NextValidBreakPoint( ai_oBP, ai_poFactory ) ;

	if( iLine != -1 )
	{
		ai_oBP.SetLine( iLine ) ;

		iResult = rBPs.Add( ai_oBP ) ;

		if( iResult != M4_SUCCESS )
		{
			iResult = rBPs.Remove( ai_oBP ) ;

			if( iResult != M4_SUCCESS )
			{
				return( -1 ) ;
			}
		}
	}

	return( iLine ) ;
}


// ============================================================================
// Calcula cual va a ser la siguiente linea válida a partir de una dada
// Para ello busca primero en el metacanal front y después en el back.
// Si no existe una linea válida mayor que la que se pasa devuelve -1
// Si no se pasa una factoría válida devuelve la línea actual sin comprobarla
// ============================================================================
m4int32_t	ClExecutor::NextValidBreakPoint( const ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4int32_t			iLine = -1 ;
	ClCompiledMCR		*poCMCR = NULL ;
	ClBuidParameters	oParameters ;


	if( ai_poFactory == NULL )
	{
		return( ai_oBP.GetLine() ) ;
	}


	oParameters.m_poLane = NULL ;
	oParameters.m_pccM4ObjId = ai_oBP.GetChannelId() ;
	oParameters.m_pccIdOrg = NULL ;
	oParameters.m_pccRSM = NULL ;
	oParameters.m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	oParameters.m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	oParameters.m_dCorDate = ClActualDate() ;
//	oParameters.m_iCsType = M4CL_CSTYPE_BOTH ;
	oParameters.m_iLanguage = M4CL_LANGUAGE_NONE ;
	oParameters.m_bConvertRSM = M4_FALSE ;
	oParameters.m_bBuildSecurity = M4_FALSE ;
	oParameters.m_bApplyMappings = M4_FALSE ;	// En bps no se aplican mapeos

	// Se mira primero la parte front
	oParameters.m_iCsType = M4CL_CSTYPE_FRONT ;

	iResult = ai_poFactory->MDFac_BuildFromId( oParameters, poCMCR ) ;

	/* Bug 0071148
	Hay que dejar un error claro de porque no se consigue poner el breakpoint
	*/
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, -1, M4_VM_BAD_M4OBJ_FOR_BP, ai_oBP.GetLine() << ai_oBP.GetRuleId() << ai_oBP.GetItemId() << ai_oBP.GetNodeId() << ai_oBP.GetChannelId() ) ;

	iLine = poCMCR->GetNextCodeLine(
						ai_oBP.GetNodeId(),
						ai_oBP.GetItemId(),
						ai_oBP.GetRuleId(),
						ai_oBP.GetStartDate(),
						ai_oBP.GetEndDate(),
						ai_oBP.GetLine() ) ;

	poCMCR->Destroy() ;

	if( iLine != -1 )
	{
		return( iLine ) ;
	}

	// Se mira luego la parte back
	oParameters.m_iCsType = M4CL_CSTYPE_BACK ;

	iResult = ai_poFactory->MDFac_BuildFromId( oParameters, poCMCR ) ;

	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, -1, M4_VM_BAD_M4OBJ_FOR_BP, ai_oBP.GetLine() << ai_oBP.GetRuleId() << ai_oBP.GetItemId() << ai_oBP.GetNodeId() << ai_oBP.GetChannelId() ) ;

	iLine = poCMCR->GetNextCodeLine(
							ai_oBP.GetNodeId(),
							ai_oBP.GetItemId(),
							ai_oBP.GetRuleId(),
							ai_oBP.GetStartDate(),
							ai_oBP.GetEndDate(),
							ai_oBP.GetLine() ) ;

	poCMCR->Destroy() ;

	if( iLine != -1 )
	{
		return( iLine ) ;
	}

	return( -1 ) ;
}



//-------------------------
//args. del item ejecutandose
//-------------------------
m4bool_t	ClExecutor::IsVariableArgs(void){

	m4VMState_t temp;
	m4uint8_t   iVarArgs;
	ClDecodeInstruction	*poIns;
	m4uint32_t hItem;

	if (!m_bInit) return M4_FALSE;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		
		//handle de item
		hItem=GetItemHandleByIds (temp.m_poAccess, poIns);
		if (hItem==0) return M4_FALSE;

		//Dato a devolver
		iVarArgs=temp.m_poAccess->GetpCMCR()->GetItemVariableArguments (hItem);

		//devolvemos el dato chachi
		return (iVarArgs!=0 ? M4_TRUE: M4_FALSE );
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get variable args");
		return M4_FALSE;
	}

}


//GetNumberOfArguments
m4uint8_t	ClExecutor::GetNumberOfArguments(void){

	m4VMState_t temp;
	m4uint8_t   iNumberOfArgs, iVarArgs;
	ClDecodeInstruction	*poIns;
	m4uint32_t hItem;

	if (!m_bInit) return M4_ERROR;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		
		//handle de item
		hItem=GetItemHandleByIds (temp.m_poAccess, poIns);
		if (hItem==0) return M4_FALSE;

		//Dato a devolver
		iVarArgs=temp.m_poAccess->GetpCMCR()->GetItemVariableArguments (hItem);

		if (iVarArgs!=0){ //hay que leer de la pila
			//leemos el  valor de la pila
			m4VariantType vNumArgs;
			m_poVMachine->m_oRunTimeStack->GetElement(vNumArgs, -1);
			
			//lo metemos en la variable que retornamos
			iNumberOfArgs= (m4uint8_t) vNumArgs.Data.DoubleData;
		}
		else{
			iNumberOfArgs=(m4uint8_t) temp.m_poAccess->GetpCMCR()->GetItemNumberOfArguments (hItem);
		}
		//devolvemos el dato chachi
		return iNumberOfArgs;
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get number of arguments");
		return 0;
	}

}



//GetArgument

m4VariantType	ClExecutor::GetArgument(m4uint8_t ai_iPos){
	
	m4VMState_t temp;
	m4VariantType vRet;

	m4uint8_t   iNumberOfArgs, iVarArgs;
	ClDecodeInstruction	*poIns;
	m4uint32_t hItem;
	m4return_t ret;

	if (!m_bInit) return M4_ERROR;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return vRet; //retorna nulo
		}
		
		//handle de item
		hItem=GetItemHandleByIds (temp.m_poAccess, poIns);
		if (hItem==0) return vRet; //retorna nulo

		//Dato a devolver
		iVarArgs=temp.m_poAccess->GetpCMCR()->GetItemVariableArguments (hItem);

		if (iVarArgs!=0){ //hay que leer de la pila
			//leemos el  valor de la pila
			m4VariantType vNumArgs;
			m_poVMachine->m_oRunTimeStack->GetElement(vNumArgs, -1);
			
			//lo metemos en la variable que retornamos
			iNumberOfArgs= (m4uint8_t) vNumArgs.Data.DoubleData;
		}
		else{
			iNumberOfArgs= (m4uint8_t)temp.m_poAccess->GetpCMCR()->GetItemNumberOfArguments (hItem);
		}
		
		//comprobamos que se pueda leer ese arg
		if (ai_iPos > iNumberOfArgs){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_PARAM_POSITION)<<"GetArgument"<<ai_iPos<<1<<iNumberOfArgs<<"<null>"<<EndLog;
			return vRet; //retorna nulo
		}

		//ajuste
		if (iVarArgs != 0)	--ai_iPos; //para que lea del sitio correcto, ya que el -1 es el número de argumentos
		
		//leemos argumento
		ret=m_poVMachine->m_oRunTimeStack->GetElement(vRet, -m4int32_t(iNumberOfArgs-ai_iPos+1)  );
		
		if (ret!= M4_SUCCESS){
			m4VariantType vNulo;
			return vNulo; //retorna nulo
		}

		//devolvemos el dato chachi
		return vRet;
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get argument value");
		return vRet; //retorna nulo
	}
}


//ser argument
m4return_t	ClExecutor::SetArgument(m4VariantType ai_vValue ,m4uint8_t ai_iPos){

	m4VMState_t temp;

	m4uint8_t   iNumberOfArgs, iVarArgs;
	ClDecodeInstruction	*poIns;
	m4uint32_t hItem;
	m4return_t ret;

	if (!m_bInit) return M4_ERROR;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		
		//handle de item
		hItem=GetItemHandleByIds (temp.m_poAccess, poIns);
		if (hItem==0) return M4_FALSE;

		//Dato a devolver
		iVarArgs=temp.m_poAccess->GetpCMCR()->GetItemVariableArguments (hItem);

		if (iVarArgs!=0){ //hay que leer de la pila
			//leemos el  valor de la pila
			m4VariantType vNumArgs;
			m_poVMachine->m_oRunTimeStack->GetElement(vNumArgs, -1);
			
			//lo metemos en la variable que retornamos
			iNumberOfArgs= (m4uint8_t) vNumArgs.Data.DoubleData;
		}
		else{
			iNumberOfArgs= (m4uint8_t) temp.m_poAccess->GetpCMCR()->GetItemNumberOfArguments (hItem);
		}
		
		//comprobamos que se pueda leer ese arg
		if (ai_iPos > iNumberOfArgs){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_PARAM_POSITION)<<"SetArgument"<<ai_iPos<<1<<iNumberOfArgs<<"<null>"<<EndLog;

			return M4_ERROR;
		}

		//ajuste
		if (iVarArgs != 0)	--ai_iPos; //para que lea del sitio correcto, ya que el -1 es el número de argumentos
		
		//leemos argumento
		ret=m_poVMachine->m_oRunTimeStack->SetElement(ai_vValue, -m4int32_t(iNumberOfArgs-ai_iPos+1)  );
		
		//devolvemos el dato chachi
		return ret;
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "set argument value");
		return M4_ERROR;
	}
}

//-------------------------
//info del item
//-------------------------
m4pcchar_t	ClExecutor::GetItemId(void){
	
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return "";

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return "";
		}
		//el nombre
		return temp.m_poAccess->GetpCMCR()->GetPointer (poIns->GetNumItem());
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get item Id");
		return "";
	}
}

m4pcchar_t	ClExecutor::GetNodeId(void){
	
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return "";

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return "";
		}
		//el nombre
		return temp.m_poAccess->GetpCMCR()->GetPointer (poIns->GetNumNodo());
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get node Id");
		return "";
	}
}

m4pcchar_t	ClExecutor::GetChannelId(void){

		
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return "";

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		//el nombre
		return temp.m_poAccess->GetpCMCR()->GetPointer (poIns->GetNameChannel());
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get channel Id");
		return "";
	}

}

//-------------------------
//Sobre lo que estamos ejecutando
//-------------------------

//operation
m4uint16_t	ClExecutor::GetOperation(void){
	
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return 0;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		//el nombre
		return poIns->GetExeFunc();
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get operation");
		return M4CL_L3_NO_OPERATION;
	}
}

//rule number
m4uint16_t	ClExecutor::GetRuleNumber(void){
	
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return 0;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		//el nombre
		return poIns->GetNumRegla();
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get rule number");
		return 0;
	}

}


//has slice
m4bool_t	ClExecutor::HasSlice(void){

		
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return M4_FALSE;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		//el nombre
		return M4_BOOL( M4CL_HAS_TRAME_FLAG(poIns->GetFlags()) );
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get slice-flag");
		return M4_FALSE;
	}

}

//has register
m4bool_t	ClExecutor::HasRegister(void){
	
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return M4_FALSE;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return M4_FALSE;
		}
		//el nombre
		return ( M4CL_HAS_ROW_FLAG(poIns->GetFlags())!=0 ? M4_TRUE : M4_FALSE );
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get record flag");
		return M4_FALSE;
	}

}


//GetLevel
m4uint8_t	ClExecutor::GetLevel(void){

		
	m4VMState_t temp;
	ClDecodeInstruction	*poIns;
	
	if (!m_bInit) return 0;

	if (IsRunning (temp)){
		
		//pinstruccion
		poIns=GetInstruction(temp);
		if (poIns==0){
			DUMP_CHLOG_ERROR(M4_CH_VM_CANT_GET_INS);
			return 0;
		}
		//el nombre
		return M4CL_GET_LEVEL_FLAG(poIns->GetFlags());
	}
	else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NOT_RUNNING, "get slice-flag");
		return 0;
	}

}

//-------------------------------
//Internas
//-------------------------------
m4bool_t	ClExecutor::IsRunning(m4VMState_t &ao_State){

	m4return_t ret=m_poVMachine->m_oStateStack->Top (ao_State);
	return M4_BOOL( ret==M4_SUCCESS ) ;
}

m4bool_t	ClExecutor::IsRunning(void){

	return M4_BOOL( !(m_poVMachine->m_oStateStack->IsEmpty () ) );
	
}

ClDecodeInstruction*	ClExecutor::GetInstruction(m4VMState_t &ai_State){

	if (!IsRunning()) return 0;
	return ( (ai_State.m_poFormula)[ai_State.m_iCurrent] );
}

m4uint32_t ClExecutor::GetItemHandleByIds (ClAccess *ai_poAccess, ClDecodeInstruction* ai_poIns){

	m4uint32_t hNode, hItem;
	m4pcchar_t pszNode;
	m4pcchar_t pszItem;

	pszNode = ai_poAccess->GetpCMCR()->GetPointer (ai_poIns->GetNumNodo());
	pszItem = ai_poAccess->GetpCMCR()->GetPointer (ai_poIns->GetNumItem());

	hNode=ai_poAccess->GetpCMCR()->GetChannelNodeHandleById (pszNode);

	if (hNode==0) {
		g_oChLog<<BeginError (M4_CH_VM_WRONG_NODE_NAME)<< pszNode<<LogCat<<"."<<LogCat<<pszItem <<"Executor.GetInstruction (L>2)"<<EndLog;
		return 0;
	}
	
	hItem=ai_poAccess->GetpCMCR()->GetNodeItemHandleById (hNode, pszItem,M4_TRUE);

	if (hItem==0){
		g_oChLog<<BeginError (M4_CH_VM_ITEM_NOT_FOUND)<< pszItem<<M4ObjNodeN(ai_poAccess->GetpCMCR(),hNode)<<"Executor.GetInstruction (L>2)"<<EndLog;
	}

	return hItem;
}


m4return_t ClExecutor::RequestAbort(m4bool_t ai_bRequest/*=M4_TRUE*/) {
 	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->RequestAbort(ai_bRequest);
};

m4return_t ClExecutor::SetStatusLabel(m4pcchar_t ai_pszLabel){ 
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->SetStatusLabel(ai_pszLabel);
};

m4return_t ClExecutor::GetStatusLabel(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetStatusLabel(ao_pszLabel, ai_iSize);
};

void ClExecutor::Reset		(void){

	m_poVMachine->Reset();
}

void ClExecutor::ResetStack( void )
{
	// Corrige bug 0060747.
	// Al hacer reset de la pila mientras se repinta la ventana del stepper al terminar la depuración
	// del método, se estaban perdiendo los valores de retorno del método (resultado y argumentos por referencia).
	// Es problema de VB que no gestiona correctamente la pila, pero lo arreglamos aquí.
	if( m_iInsideCallBack == 0 )
	{
		m_Stack.Reset() ;
	}
}

void ClExecutor::ResetDebugging	(void){

	if (!IsRunning()) return;

	m4uint32_t hItem=-1;
	m4uint32_t i=0;
	m4VMState_t oState;
	m4uint32_t iNumPush=0;
	m4int32_t iRef;
	//averiguamos cuantos argumentos por referencia tiene el item que inició la ejecución
	iRef=m_poVMachine->GetStateStack()->GetReference();
	m_poVMachine->GetStateStack()->SetReference(0); //Referencia a 0 para usar GetElement sde forma absoluta
	
	while ( m_poVMachine->GetStateStack()->GetElement(oState,i) == M4_SUCCESS){
		if (oState.m_hItem!= -1){
			hItem=oState.m_hItem;
			break; //hemos encontrado el estado
		}
		i++;
	}
	m_poVMachine->GetStateStack()->SetReference(iRef);//restauramos ref. despues de usar GetElement

	if (hItem!=-1){
		iNumPush=oState.m_poAccess->GetpCMCR()->GetItemNumberOfReferedArguments (oState.m_hItem) + 1;
	}


	//Reseteamos
	m_poVMachine->Reset();

	//metemos en la pila tanto valores como arg por referencia+1 (resultado)
	m4VariantType vTemp(M4_ERROR);
	for (i=0;i<iNumPush;++i){
		m_poVMachine->GetRuntimeStack()->Push(vTemp);
	}
	m_poVMachine->GetVMCSContext()->GetExecAdaptor()->EndDebugMethod();
	return;
}


void ClExecutor::SetpCancelStatus(ClStatus *ai_poCancelStatus)
{
	m_poVMachine->SetpCancelStatus(ai_poCancelStatus);
}

m4bool_t ClExecutor::GetCancelStatus()
{
	if (m_poVMachine) {
		return m_poVMachine->GetCancelStatus();
	}
	else {
		return M4_FALSE;
	}
}
/**
 * Petición para abortar una ejecución.
 * Es distinto del RequestToAbort porque tiene en cuenta si tenemos asignado un ClStatus
 * (por ejemplo en el caso de estar ejecutando en el servidor). En tal caso la petición para
 * abortar se hace a través de este objeto.
 *
 * @param ai_bRequestToAbort    M4_TRUE  --> se pide que se aborte la ejecución.
 *								M4_FALSE --> se pide que la ejecución continue con normalidad.
 */
void ClExecutor::RequestToAbortWithClStatus(const m4bool_t ai_bRequestToAbort) {
	m_poVMachine->RequestToAbortWithClStatus(ai_bRequestToAbort);
}

/**
 * Nos devuelve si ha sido abortada una ejecución.
 */
m4bool_t ClExecutor::GetRequestToAbortWithClStatus(void) const{
	return m_poVMachine->GetRequestToAbortWithClStatus();
}

m4return_t	ClExecutor::GetRTInfo(ClExecutorRuntimeInfo *ai_poRTInfo){

	m4VMState_t oState;
	m4VariantType vTemp;
	m4uint32_t iNumArg, i;
	m4return_t ret;
	
	
	if (!m_bInit) return M4_ERROR;


	//si está ejecutando
	
	if (IsRunning(oState)){
		
		//miramos si el handle es válido y asignamos handle+mcr
		if (oState.m_hItem== -1){
			//alguien ha parado la ejecución a mala leche en un mal sitio
			//no podemos dar información
			return M4_ERROR;
		}
		ClCompiledMCR * poCMCR=oState.m_poAccess->GetpCMCR();
		ai_poRTInfo->SetItem(poCMCR, oState.m_hItem);

		/* Bug 0087961
		Si el stepper está activo la información de los argumentos puede que
		no esté. Como no podemos detectar si realmente está o no está no la
		mostramos si el stepper está activo.
		*/
		if( GetpStepper()->GetActive() == M4_TRUE )
		{
			iNumArg=0;
		}
		else
		{
			iNumArg=m_poVMachine->GetRunningItemArgumentNumber(oState, m_poVMachine->GetRuntimeStack()->GetReference() );
		}

		ai_poRTInfo->SetArgNum (iNumArg);
		for (i=1;i<=iNumArg;++i){
			ret=m_poVMachine->GetRunningItemArgumentByPosition(oState, m_poVMachine->GetRuntimeStack()->GetReference(), i , vTemp); 
			if (ret!=M4_SUCCESS){
				return M4_ERROR;
			}
			ret=ai_poRTInfo->SetArg (vTemp,i-1);
			if (ret!=M4_SUCCESS){
				return M4_ERROR;
			}
		}

	}
	else{
		return M4_ERROR;
	}
	return M4_SUCCESS;
}
//------
//varias
//------
m4uint32_t ClExecutor::GetSize (void) const { 
	if (!m_bInit) return 0;
	return m_poVMachine->GetSize(); 
}

//Volcar pila de llamadas de la VM

string	ClExecutor::DumpCallStack(void) { 

	if (!m_bInit) return "";
	return m_poVMachine->DumpCallStack(); 
}	


//---------------------
//Stepper
//---------------------
ClVMStepper *ClExecutor::GetpStepper(void){
	if (!m_bInit) return 0;
	return m_poVMachine->GetpStepper()	;
}


//replica del interfaz de vmachine en lo relacionado a stepper (desde aquí solo se llama a vmachine, tal cual)
m4return_t ClExecutor::GetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetVariableValue(ai_iCallStackPos, ai_ivar, ao_vValue);
}

m4return_t ClExecutor::SetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->SetVariableValue(ai_iCallStackPos, ai_ivar, ai_vValue);
}

m4uint32_t ClExecutor::GetNumberOfVariables(m4uint32_t ai_iCallStackPos){
	if (!m_bInit) return 0;
	return m_poVMachine->GetNumberOfVariables(ai_iCallStackPos);
}

m4return_t ClExecutor::GetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetArgumentValue(ai_iCallStackPos, ai_ivar, ao_vValue);
}

m4return_t ClExecutor::SetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->SetArgumentValue(ai_iCallStackPos, ai_ivar, ai_vValue);
}

m4uint32_t ClExecutor::GetNumberOfArguments(m4uint32_t ai_iCallStackPos){
	if (!m_bInit) return 0;
	return m_poVMachine->GetNumberOfArguments(ai_iCallStackPos);
}

m4uint32_t ClExecutor::GetCallStackSize (void){
	if (!m_bInit) return 0;
	return m_poVMachine->GetCallStackSize();
}

m4return_t ClExecutor::GetCallStackInfo (m4uint32_t ai_iCallStackPos, ClRunningFormInfo &ao_oInfo){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetCallStackInfo(ai_iCallStackPos,ao_oInfo);
}

m4return_t ClExecutor::GetSymbolTable	(ClItem *ai_poItem, m4char_t *ai_pszCode, m4char_t **ao_ppszSymbols, m4uint32_t ai_iSizeTable, m4uint32_t ai_iSizeSymbol){//ojo. asigna/libera el que llama
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetSymbolTable(ai_poItem, ai_pszCode, ao_ppszSymbols, ai_iSizeTable,ai_iSizeSymbol);
}


m4uint32_t ClExecutor::GetNumAccessToSerialize(void){
	if (!m_bInit) return 0;
	return m_poVMachine->GetNumAccessToSerialize();

}

m4return_t ClExecutor::GetAccessToSerialize (ClAccess **ai_ppAcesss, m4uint32_t ai_iNumAccess){
	if (!m_bInit) return M4_ERROR;
	return m_poVMachine->GetAccessToSerialize(ai_ppAcesss,ai_iNumAccess);
}


//----------------------------------------
// Exclusión de funciones
//----------------------------------------
m4return_t	ClExecutor::SetExcludeFunction( m4pcchar_t ai_pccFunction )
{
	if( m_bInit == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	return( m_poVMachine->SetExcludeFunction( ai_pccFunction ) ) ;
}


m4return_t	ClExecutor::UnsetExcludeFunction( m4pcchar_t ai_pccFunction )
{
	if( m_bInit == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	return( m_poVMachine->UnsetExcludeFunction( ai_pccFunction ) ) ;
}


m4return_t	ClExecutor::ClearExcludeFunctions( void )
{
	if( m_bInit == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	return( m_poVMachine->ClearExcludeFunctions() ) ;
}

