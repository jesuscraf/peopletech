
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapexecutor.h"
#include "wrapdef.h"
#include "m4types.hpp"
#include "cvdate.h"
#include "wrapitit.h"
#include "gstring.h"
#include "iexecutor.hpp"
#include "executor.hpp"
#include "wrapchannel.h"
#include "wrapcvm.h"
#include "newinterface.h"
#include "sttlib.hpp"
#include "clfactm4objservice.hpp"
#include "sttwrap.hpp"
#include "rerror.h"
#include "wores.hpp"
#include "vmstepper.hpp"
#include "wrapbreakpoint.h"
#include "wraprunninginfo.h"
#include "vmmcrdef.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapExecutor

STDMETHODIMP CWrapExecutor::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapExecutor };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

//--------------------
//SliceMode
//--------------------
STDMETHODIMP CWrapExecutor::get_RunSliceMode(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_RunSliceMode);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_RunSliceMode, E_UNEXPECTED);
	}

	m4VariantType tmpVar;

	//m_poExecutor->GetRunSliceMode(tmpVar);

	if ((m4double_t)tmpVar)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapExecutor::get_RunSliceMode, S_OK);
}

STDMETHODIMP CWrapExecutor::put_RunSliceMode(VARIANT_BOOL newVal)
{
	ENTER_METHOD(CWrapExecutor::put_RunSliceMode);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapExecutor::put_RunSliceMode" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapExecutor::put_RunSliceMode, S_OK);
}


//--------------------
//Dates
//--------------------
STDMETHODIMP CWrapExecutor::get_ExecutionDate(DATE * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_ExecutionDate);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_ExecutionDate, E_UNEXPECTED);
	}
	
	m4VariantType tmpVar;
	
	ret = m_poExecutor->GetExecutionDate(tmpVar);
	
	if (ret == M4_SUCCESS)
	{
		*pVal = M4dateToDate(tmpVar);
		
		TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);
		
		LEAVE_METHOD(CWrapExecutor::get_ExecutionDate, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_ExecutionDate, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapExecutor::get_StartDate(DATE * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_StartDate);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_StartDate, E_UNEXPECTED);
	}

	m4VariantType tmpVar;
	
	ret = m_poExecutor->GetStartDate(tmpVar);
	
	if (ret == M4_SUCCESS)
	{
		*pVal = M4dateToDate(tmpVar);
		
		TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);
		LEAVE_METHOD(CWrapExecutor::get_StartDate, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_StartDate, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapExecutor::get_EndDate(DATE * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_EndDate);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_EndDate, E_UNEXPECTED);
	}

	m4VariantType tmpVar;
	
	ret = m_poExecutor->GetEndDate(tmpVar);
	
	if (ret == M4_SUCCESS)
	{
		*pVal = M4dateToDate(tmpVar);
		
		TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);
		LEAVE_METHOD(CWrapExecutor::get_EndDate, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_EndDate, E_UNEXPECTED);
	}
}


//--------------------
//Execution flow
//--------------------
STDMETHODIMP CWrapExecutor::Call(IWrapItIt * ai_powItem, short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Call);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Call, E_UNEXPECTED);
	}

	ClItem * poItem;
	
	((CWrapItIt*)ai_powItem)->UnWrap (&poItem);

	*ao_RetVal = m_poExecutor->Call( poItem,-1,M4_TRUE ); //-1: no son dicen los argumentos, M4_TRUE->Enable debug

	TRACE2("Executor: Call returns %d", *ao_RetVal);
	LEAVE_METHOD(CWrapExecutor::Call, S_OK);
}

STDMETHODIMP CWrapExecutor::Abort(short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Abort);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Abort, E_UNEXPECTED);
	}

	*ao_RetVal= m_poExecutor->Abort();

	LEAVE_METHOD(CWrapExecutor::Abort, S_OK);
}

STDMETHODIMP CWrapExecutor::Continue(short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Continue);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Continue, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->Continue();

	LEAVE_METHOD(CWrapExecutor::Continue, S_OK);
}

STDMETHODIMP CWrapExecutor::StepInto(short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::StepInto);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::StepInto, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->StepInto();

	LEAVE_METHOD(CWrapExecutor::StepInto, S_OK);
}

STDMETHODIMP CWrapExecutor::StepOver(short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::StepOver);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::StepOver, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->StepOver();

	LEAVE_METHOD(CWrapExecutor::StepOver, S_OK);
}

STDMETHODIMP CWrapExecutor::StepOut(short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::StepOut);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::StepOut, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->StepOut();

	LEAVE_METHOD(CWrapExecutor::StepOut, S_OK);
}

STDMETHODIMP CWrapExecutor::RunToCursor(IWrapBreakpoint *ai_poBP, short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::RunToCursor);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::RunToCursor, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	*ao_RetVal = (short) m_poExecutor->RunToCursor( *poBP, m_poFactory ) ;

	LEAVE_METHOD(CWrapExecutor::RunToCursor,S_OK);
}


//--------------------
//Level
//--------------------
STDMETHODIMP CWrapExecutor::get_Level(short * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Level);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Level, E_UNEXPECTED);
	}

	*pVal = m_poExecutor->GetLevel();

	LEAVE_METHOD(CWrapExecutor::get_Level, S_OK);
}


//--------------------
//Arguments
//--------------------
STDMETHODIMP CWrapExecutor::get_HasVariableArguments(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_HasVariableArguments);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_HasVariableArguments, E_UNEXPECTED);
	}
	
	if (m_poExecutor->IsVariableArgs() ){
		*pVal=VARIANT_TRUE;
	}
	else{
		*pVal=VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapExecutor::get_HasVariableArguments, S_OK);
}


STDMETHODIMP CWrapExecutor::get_NumberOfArguments(short * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_NumberOfArguments);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_NumberOfArguments, E_UNEXPECTED);
	}

	*pVal = m_poExecutor->GetNumberOfArguments();

	LEAVE_METHOD(CWrapExecutor::get_NumberOfArguments, S_OK);
}

STDMETHODIMP CWrapExecutor::get_Argument(short ai_iPos, VARIANT * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Argument);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Argument, E_UNEXPECTED);
	}

	m_poExecutor->GetArgument( (m4uint8_t) ai_iPos ).GetVARIANT( *pVal );

	LEAVE_METHOD(CWrapExecutor::get_Argument, S_OK);
}

STDMETHODIMP CWrapExecutor::put_Argument(short ai_iPos, VARIANT newVal)
{
	ENTER_METHOD(CWrapExecutor::put_Argument);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::put_Argument, E_UNEXPECTED);
	}

	ret = m_poExecutor->SetArgument (newVal, (m4uint8_t) ai_iPos);
	
	if (ret==M4_SUCCESS){
		LEAVE_METHOD(CWrapExecutor::put_Argument, S_OK);
	}
	else{
		LEAVE_METHOD(CWrapExecutor::put_Argument, E_UNEXPECTED);
	}
}


//---------------------------
//Names
//---------------------------
STDMETHODIMP CWrapExecutor::get_ItemId(BSTR * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_ItemId);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_ItemId, E_UNEXPECTED);
	}

	gstring tmpGstring( m_poExecutor->GetItemId() );

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapExecutor::get_ItemId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_ItemId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapExecutor::get_NodeId(BSTR * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_NodeId);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_NodeId, E_UNEXPECTED);
	}

	gstring tmpGstring( m_poExecutor->GetNodeId() );

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapExecutor::get_NodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_NodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapExecutor::get_ChannelId(BSTR * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_ChannelId);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_ChannelId, E_UNEXPECTED);
	}

	gstring tmpGstring( m_poExecutor->GetChannelId() );

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapExecutor::get_ChannelId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapExecutor::get_ChannelId, E_UNEXPECTED);
	}
}


//----------------------
//What we are executing
//----------------------
STDMETHODIMP CWrapExecutor::get_Operation(short * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Operation);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Operation, E_UNEXPECTED);
	}

	*pVal=m_poExecutor->GetOperation();

	LEAVE_METHOD(CWrapExecutor::get_Operation, S_OK);
}

STDMETHODIMP CWrapExecutor::get_RuleNumber(short * pVal)
{

	ENTER_METHOD(CWrapExecutor::get_RuleNumber);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_RuleNumber, E_UNEXPECTED);
	}

	*pVal=m_poExecutor->GetRuleNumber();

	LEAVE_METHOD(CWrapExecutor::get_RuleNumber, S_OK);
}


STDMETHODIMP CWrapExecutor::get_HasSlice(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_HasSlice);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_HasSlice, E_UNEXPECTED);
	}
	
	if (m_poExecutor->HasSlice() ){
		*pVal=VARIANT_TRUE;
	}
	else{
		*pVal=VARIANT_FALSE;
	}
	
	LEAVE_METHOD(CWrapExecutor::get_HasSlice, S_OK);
}


STDMETHODIMP CWrapExecutor::get_HasRegister(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_HasRegister);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_HasRegister, E_UNEXPECTED);
	}
	
	if (m_poExecutor->HasRegister() ){
		*pVal=VARIANT_TRUE;
	}
	else{
		*pVal=VARIANT_FALSE;
	}
	
	LEAVE_METHOD(CWrapExecutor::get_HasRegister, S_OK);
}


//----------------------------------
//AS a Stack
//----------------------------------
STDMETHODIMP CWrapExecutor::get_Stack(IWrapStack * * pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Stack);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Stack, E_UNEXPECTED);
	}

	// Just cast this and return a pointer to
	// the stack
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapStack*) this;

	TRACE("Casting Executor to a stack");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapExecutor::get_Stack,S_OK); 
}


STDMETHODIMP CWrapExecutor::Pop(VARIANT* ao_varref, short* ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Pop);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Pop, E_UNEXPECTED);
	}

	m4VariantType var;
	*ao_RetVal = m_poExecutor->Stack.Pop(var);

	var.GetVARIANT(*ao_varref);

	if (*ao_RetVal == M4_SUCCESS) {
		LEAVE_METHOD(CWrapExecutor::Pop, S_OK);
	}

	LEAVE_METHOD(CWrapExecutor::Pop, E_UNEXPECTED);
}

STDMETHODIMP CWrapExecutor::Push(VARIANT ai_varref, short* ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Push);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Push, E_UNEXPECTED);
	}

	m4VariantType pvar(ai_varref);

	*ao_RetVal = m_poExecutor->Stack.Push(pvar);

	LEAVE_METHOD(CWrapExecutor::Push, S_OK);
}

STDMETHODIMP CWrapExecutor::Reset()
{
	ENTER_METHOD(CWrapExecutor::Reset);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Reset, E_UNEXPECTED);
	}

	m_poExecutor->ResetStack();

	LEAVE_METHOD(CWrapExecutor::Reset, S_OK);
}

//-------------------Private
m4return_t CWrapExecutor::Init(ClExecutor *ai_poExecutor)
{
	if (ai_poExecutor == 0)
	{
		if (m_poExecutor != 0)
		{
			return M4_SUCCESS; //ya tenemos uno
		}
		else
		{ //creamos uno nuevo
			m_poExecutor=(ClExecutor*)ClFactM4ObjService::CreateExecutor();

			if (!m_poExecutor)
			{
				//si no podemos crear el ejecutor->Excepción a VB
				return M4_ERROR;
			}
			m_bOwner=M4_TRUE;
		}
	}
	else
	{ //usamos el que nos dicen
		if (m_bOwner && m_poExecutor!= 0)
		{
			
			ClFactM4ObjService::DestroyExecutor(m_poExecutor);//lo hemos creado nosotros y nos dicen que usemos otro
			m_bOwner=M4_FALSE;
		}
		m_poExecutor = ai_poExecutor;
	}

	return M4_SUCCESS;
}


STDMETHODIMP	CWrapExecutor::GetSttStatus(short *pVal)
{
	ENTER_METHOD(CWrapExecutor::GetSttStatus);

	*pVal = m_poExecutor->GetSttWrap()->GetStatisticsLevel();

	LEAVE_METHOD(CWrapExecutor::GetSttStatus, S_OK);
}


STDMETHODIMP	CWrapExecutor::SetSttStatus(short newVal, IWrapCVM *ai_pIWapCVM)
{
	ENTER_METHOD(CWrapExecutor::SetSttStatus);

	m_poExecutor->GetSttWrap()->SetStatisticsLevel((m4uint16_t)newVal, ((CWrapCVM*)ai_pIWapCVM)->GetpChannelManager(), M4_TRUE);

	LEAVE_METHOD(CWrapExecutor::SetSttStatus, S_OK);
}


STDMETHODIMP	CWrapExecutor::GetSttLabel(BSTR *ao_pstLabel)
{
	ENTER_METHOD(CWrapExecutor::GetSttLabel);

	m4pchar_t pclabel = m_poExecutor->GetSttWrap()->GetStatisticsLabel();

	if( pclabel == NULL )
	{
		pclabel = "";
	}

	gstring tmpGstring(pclabel);

	if( tmpGstring != NULL )
	{
		*ao_pstLabel = (BSTR)tmpGstring;
	}

	LEAVE_METHOD(CWrapExecutor::GetSttLabel, S_OK);
}


STDMETHODIMP	CWrapExecutor::SetSttLabel(BSTR ai_pstName)
{
	ENTER_METHOD(CWrapExecutor::SetSttLabel);

	gstring tmpGstring(ai_pstName);

	if( tmpGstring != NULL )
	{
		m_poExecutor->GetSttWrap()->SetStatisticsLabel(tmpGstring, M4_TRUE);
	}
	else
	{
		m_poExecutor->GetSttWrap()->SetStatisticsLabel("", M4_TRUE);
	}

	LEAVE_METHOD(CWrapExecutor::SetSttLabel, S_OK);
}


STDMETHODIMP CWrapExecutor::GetSttChannel(IWrapChannel** pVal)
{
	ENTER_METHOD(CWrapExecutor::GetSttChannel);

	ClVMStatistics *poStatistics = m_poExecutor->GetSttWrap()->GetStatistics();

	if( poStatistics != NULL )
	{
		if( NewInterface(CLSID_WrapChannel, IID_IWrapChannel, (IUnknown**)pVal) != M4_SUCCESS )
		{
			LEAVE_METHOD(CWrapExecutor::GetSttChannel, E_UNEXPECTED);
		}

		ClChannel *pSttChannel = poStatistics->sttGetChannel();

		if( pSttChannel != NULL )
		{
			((CWrapChannel*)(*pVal))->Init(pSttChannel);
			m_poExecutor->GetSttWrap()->IncStatisticsRef();
		}
	}

	LEAVE_METHOD(CWrapExecutor::GetSttChannel, S_OK);
}


STDMETHODIMP	CWrapExecutor::FreeSttChannel(short* ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::FreeSttChannel);

	m_poExecutor->GetSttWrap()->DecStatisticsRef();
	*ao_RetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapExecutor::FreeSttChannel, S_OK);
}


STDMETHODIMP	CWrapExecutor::GetSttPause(VARIANT_BOOL *pVal)
{
	ENTER_METHOD(CWrapExecutor::GetSttPause);

	if( m_poExecutor->GetSttWrap()->GetStatisticsPause() == M4_TRUE )
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapExecutor::GetSttPause, S_OK);
}


STDMETHODIMP	CWrapExecutor::SetSttPause(VARIANT_BOOL newVal)
{
	ENTER_METHOD(CWrapExecutor::SetSttPause);

	m4bool_t bPause = newVal == VARIANT_TRUE ? M4_TRUE : M4_FALSE;

	m_poExecutor->GetSttWrap()->SetStatisticsPause(bPause);

	LEAVE_METHOD(CWrapExecutor::SetSttPause, S_OK);
}


//--------------------------------------------
//metodos de executor relacionados con stepper
//--------------------------------------------
STDMETHODIMP CWrapExecutor::get_Stepper (/*[out, retval]*/ IWrapStepper* *pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Stepper);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Stepper, E_UNEXPECTED);
	}

	// Just cast this and return a pointer to
	// the stack
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapStepper*) this;

	TRACE("Casting Executor to a stepper");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapExecutor::get_Stepper,S_OK); 
}
		

STDMETHODIMP CWrapExecutor::GetCallStackSize(/*[out, retval]*/ int * ao_RetVal){
	
	ENTER_METHOD(CWrapExecutor::GetCallStackSize);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetCallStackSize, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetCallStackSize();

	LEAVE_METHOD(CWrapExecutor::GetCallStackSize,S_OK);
}

STDMETHODIMP CWrapExecutor::GetCallStackInfo(/*[in]*/ int ai_iCallStackPos, /*[out]*/ IWrapRunningFormInfo **ao_poInfo ,/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetCallStackInfo);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetCallStackInfo, E_UNEXPECTED);
	}

	ClRunningFormInfo  oInfo;

	ret = m_poExecutor->GetCallStackInfo(ai_iCallStackPos,oInfo);

	if (ret!=M4_SUCCESS)
	{
		FormatError(M4_CH_WO_NO_SUCH_INFO,"No such info");
		LEAVE_METHOD(CWrapExecutor::GetCallStackInfo, E_UNEXPECTED);
	}

	if(NewInterface(CLSID_WrapRunningFormInfo, IID_IWrapRunningFormInfo,(IUnknown**)ao_poInfo) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetCallStackInfo, E_UNEXPECTED);
	}

	((CWrapRunningFormInfo*)(*ao_poInfo))->Init(&oInfo);

	LEAVE_METHOD(CWrapExecutor::GetCallStackInfo,S_OK);
}


STDMETHODIMP CWrapExecutor::GetNumberOfVariables (/*[in]*/ int ai_iCallStackPos,/*[out, retval]*/ int * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetNumberOfVariables);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetNumberOfVariables, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetNumberOfVariables(ai_iCallStackPos);

	LEAVE_METHOD(CWrapExecutor::GetNumberOfVariables,S_OK);
}

STDMETHODIMP CWrapExecutor::GetVariableValue(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[out]*/ VARIANT *ao_poVar ,/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetVariableValue);
	
	*ao_RetVal=M4_ERROR;

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetVariableValue, E_UNEXPECTED);
	}
	
	m4VariantType vVar;
	ret = m_poExecutor->GetVariableValue(ai_iCallStackPos, ai_iPos, vVar);

	if (ret == M4_SUCCESS)
	{
		vVar.GetVARIANT( *ao_poVar ) ;
		*ao_RetVal=M4_SUCCESS;	
		LEAVE_METHOD(CWrapExecutor::GetVariableValue,	S_OK);
	}

	LEAVE_METHOD(CWrapExecutor::GetVariableValue, E_UNEXPECTED);
}

STDMETHODIMP CWrapExecutor::SetVariableValue(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[in]*/ VARIANT ai_poVar ,/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::SetVariableValue);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::SetVariableValue, E_UNEXPECTED);
	}

	m4VariantType vVar(ai_poVar);

	*ao_RetVal=m_poExecutor->SetVariableValue(ai_iCallStackPos, ai_iPos, vVar);

	LEAVE_METHOD(CWrapExecutor::SetVariableValue, S_OK);
}

STDMETHODIMP CWrapExecutor::GetNumberOfArguments(int ai_iCallStackPos, int *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetNumberOfArguments);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetNumberOfArguments, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetNumberOfArguments(ai_iCallStackPos);

	LEAVE_METHOD(CWrapExecutor::GetNumberOfArguments,S_OK);
}

STDMETHODIMP CWrapExecutor::GetArgumentValue(int ai_iCallStackPos, int ai_iPos, VARIANT *ao_poVar, short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetArgumentValue);
	
	*ao_RetVal=M4_ERROR;

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetArgumentValue, E_UNEXPECTED);
	}
	
	m4VariantType vVar;
	ret = m_poExecutor->GetArgumentValue(ai_iCallStackPos, ai_iPos, vVar);
	
	if (ret == M4_SUCCESS)
	{
		vVar.GetVARIANT( *ao_poVar ) ;
		*ao_RetVal=M4_SUCCESS;	
		LEAVE_METHOD(CWrapExecutor::GetArgumentValue,	S_OK);
	}

	LEAVE_METHOD(CWrapExecutor::GetArgumentValue, E_UNEXPECTED);
}

STDMETHODIMP CWrapExecutor::SetArgumentValue(int ai_iCallStackPos, int ai_iPos, VARIANT ai_poVar, short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::SetArgumentValue);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::SetArgumentValue, E_UNEXPECTED);
	}

	m4VariantType vVar(ai_poVar);

	*ao_RetVal = m_poExecutor->SetArgumentValue(ai_iCallStackPos, ai_iPos, vVar);

	LEAVE_METHOD(CWrapExecutor::SetArgumentValue, S_OK);
}

STDMETHODIMP CWrapExecutor::GetSymbolTable(/*[in]*/IWrapItIt *ai_poItem, /*[in]*/ BSTR ai_pstCode,/*[in]*/ int ai_iNumVars, /*[out]*/ SAFEARRAY** ao_poSymbols ,/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetSymbolTable);
	
	USES_CONVERSION;

	m4char_t **ppszVars;
	m4int_t i;

	gstring tmpGstring(ai_pstCode);

	if (tmpGstring){
		
		//creamos el array para que nos metan los símbolos
		ppszVars=new m4char_t*[ai_iNumVars];
		for (i=0;i<ai_iNumVars;++i){
			ppszVars[i]=new m4char_t [M4CL_MAX_LINEA_POLACA];
			ppszVars[i][0]=0;
		}
		
		ClItem * poItem;
		
		((CWrapItIt*)ai_poItem)->UnWrap (&poItem);
		
		//obtenemos los símbolos
		*ao_RetVal=m_poExecutor->GetSymbolTable	(poItem, tmpGstring, ppszVars,ai_iNumVars, M4CL_MAX_LINEA_POLACA);
			
		//si todo ha ido bien, al safaarray
		if (*ao_RetVal==M4_SUCCESS){
			
			BSTR *pbstrTemp;
			do{
				//creamos el safe array y retornamos la info
				if (SafeArrayDestroy(*ao_poSymbols)!=S_OK){ // Error freeing array
					*ao_RetVal=M4_ERROR;
					break;
				}
				
				if ((*ao_poSymbols=SafeArrayCreateVector(VT_BSTR,0,ai_iNumVars))==NULL) { 
					*ao_RetVal=M4_ERROR;
					break;
				}
				
				//para acceder al SafeArray como si fuera un array
				if (SafeArrayAccessData(*ao_poSymbols, (void HUGEP* FAR*)&pbstrTemp)) {
					*ao_RetVal=M4_ERROR;
					break;
				}
				
				for (i=0; i<ai_iNumVars; ++i){
					pbstrTemp[i]=SysAllocString(A2OLE(ppszVars[i]) );
				}
				
				SafeArrayUnaccessData(*ao_poSymbols); //desbloqueamos
			
			}while(0);
		}
		
		for (i=0;i<ai_iNumVars;++i){
			delete []ppszVars[i];
		}
		delete []ppszVars;
		
	}
	else{
		*ao_RetVal=M4_ERROR;
	}

	LEAVE_METHOD(CWrapExecutor::GetSymbolTable, S_OK);
}

//--------------------------------------------
//Métodos del interfaz de stepper
//--------------------------------------------
STDMETHODIMP CWrapExecutor::get_Active(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Active);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Active, E_UNEXPECTED);
	}
	
	m4bool_t bIsActive = m_poExecutor->GetpStepper()->GetActive();

	if (bIsActive){
		*pVal = VARIANT_TRUE;
	}
	else{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapExecutor::get_Active, S_OK);
}

STDMETHODIMP CWrapExecutor::put_Active(/*[in]*/ VARIANT_BOOL newVal)
{
	ENTER_METHOD(CWrapExecutor::put_Active);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::put_Active, E_UNEXPECTED);
	}

	m_poExecutor->GetpStepper()->SetActive(newVal == VARIANT_TRUE);

	LEAVE_METHOD(CWrapExecutor::put_Active, S_OK);
}

STDMETHODIMP CWrapExecutor::get_Mode(/*[out, retval]*/ STEPPERMODE *pVal)
{
	ENTER_METHOD(CWrapExecutor::get_Mode);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::get_Mode, E_UNEXPECTED);
	}
	
	eVMStepperMode eMode=m_poExecutor->GetpStepper()->GetMode();

	*pVal=(STEPPERMODE)eMode;
	
	LEAVE_METHOD(CWrapExecutor::get_Mode, S_OK);
}

STDMETHODIMP CWrapExecutor::put_Mode(/*[in]*/ STEPPERMODE newVal)
{
	ENTER_METHOD(CWrapExecutor::put_Mode);

	/* Esto ya no se debe utilizar
	m4return_t ret= Init(0);
	if (ret!=M4_SUCCESS){
		LEAVE_METHOD(CWrapExecutor::put_Mode, E_UNEXPECTED);
	}

	m_poExecutor->GetpStepper()->SetMode(eVMStepperMode(newVal));
	*/

	LEAVE_METHOD(CWrapExecutor::put_Mode, S_OK);
}

STDMETHODIMP CWrapExecutor::RemoveAll(/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::RemoveAll);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::RemoveAll, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetpStepper()->GetBPs().RemoveAll();

	LEAVE_METHOD(CWrapExecutor::RemoveAll, S_OK);
}

STDMETHODIMP CWrapExecutor::GetBPSize(/*[out, retval]*/ int * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::GetBPSize);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetBPSize, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetpStepper()->GetBPs().GetSize();

	LEAVE_METHOD(CWrapExecutor::GetBPSize, S_OK);
}

STDMETHODIMP CWrapExecutor::Add(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Add);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Add, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	*ao_RetVal = (short) m_poExecutor->AddBreakPoint( *poBP, m_poFactory ) ;

	LEAVE_METHOD(CWrapExecutor::Add, S_OK);
}

STDMETHODIMP CWrapExecutor::RemoveByID(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::RemoveByID);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::RemoveByID, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	*ao_RetVal = (short) m_poExecutor->DeleteBreakPoint( *poBP, m_poFactory ) ;

	LEAVE_METHOD(CWrapExecutor::RemoveByID, S_OK);
}

STDMETHODIMP CWrapExecutor::RemoveByPos(/*[in]*/int ai_iBP, /*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::RemoveByPos);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::RemoveByPos, E_UNEXPECTED);
	}

	*ao_RetVal = m_poExecutor->GetpStepper()->GetBPs().Remove(ai_iBP);

	LEAVE_METHOD(CWrapExecutor::RemoveByPos,S_OK);
}

STDMETHODIMP CWrapExecutor::Toggle(IWrapBreakpoint *ai_poBP, short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::Toggle);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::Toggle, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	*ao_RetVal = (short) m_poExecutor->ToggleBreakPoint( *poBP, m_poFactory ) ;

	LEAVE_METHOD(CWrapExecutor::Toggle, S_OK);
}

STDMETHODIMP CWrapExecutor::NextValidBreakPoint(IWrapBreakpoint *ai_poBP, short *ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::NextValidBreakPoint);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::NextValidBreakPoint, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	*ao_RetVal = (short) m_poExecutor->NextValidBreakPoint( *poBP, m_poFactory ) ;

	LEAVE_METHOD(CWrapExecutor::NextValidBreakPoint, S_OK);
}

STDMETHODIMP CWrapExecutor::GetByPos(/*[in]*/int ai_iBP, /*[out, retval]*/ IWrapBreakpoint** pVal)
{
	ENTER_METHOD(CWrapExecutor::GetByPos);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetByPos, E_UNEXPECTED);
	}

	ClBreakpoint oBP;

	m4bool_t bExist=m_poExecutor->GetpStepper()->GetBPs().Get(ai_iBP, oBP);

	if (!bExist)
	{
		FormatError(M4_CH_WO_NO_SUCH_BP,"No such BreakPoint");
		LEAVE_METHOD(CWrapExecutor::GetByPos, E_UNEXPECTED);
	}

	if(NewInterface(CLSID_WrapBreakpoint, IID_IWrapBreakpoint,(IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetByPos, E_UNEXPECTED);
	}

	((CWrapBreakpoint*)(*pVal))->Init(&oBP);

	LEAVE_METHOD(CWrapExecutor::GetByPos, S_OK);
}

STDMETHODIMP CWrapExecutor::GetByID(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapExecutor::GetByID);

	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::GetByID, E_UNEXPECTED);
	}

	ClBreakpoint *poBP;
	
	((CWrapBreakpoint*)ai_poBP)->UnWrap (&poBP);

	if(m_poExecutor->GetpStepper()->GetBPs().Get(*poBP) != NULL){
		*pVal = VARIANT_TRUE;
	}
	else{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapExecutor::GetByID, S_OK);
}


STDMETHODIMP CWrapExecutor::SetStepOverReference(/*[in]*/ int ai_iCallStackPos,/*[out, retval]*/ short * ao_RetVal)
{
	ENTER_METHOD(CWrapExecutor::SetStepOverReference);
	
	/* Esto ya no se debe utilizar
	m4return_t ret= Init(0);
	if (ret!=M4_SUCCESS){
		LEAVE_METHOD(CWrapExecutor::SetStepOverReference, E_UNEXPECTED);
	}

	// jcr Apaño hasta que VB utilice el nuevo interfaz del stepper
	m_poExecutor->GetpStepper()->SetMode( StepperModeStepOut ) ;
	*/
	*ao_RetVal = M4_SUCCESS ;

	LEAVE_METHOD(CWrapExecutor::SetStepOverReference, S_OK);
}


STDMETHODIMP CWrapExecutor::ResetExecutor(void)
{
	ENTER_METHOD(CWrapExecutor::ResetExecutor);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::ResetExecutor, E_UNEXPECTED);
	}

	m_poExecutor->Reset();

	LEAVE_METHOD(CWrapExecutor::ResetExecutor, S_OK);
}

STDMETHODIMP CWrapExecutor::ResetDebugging(void)
{
	ENTER_METHOD(CWrapExecutor::ResetDebugging);
	
	m4return_t ret= Init(0);

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapExecutor::ResetDebugging, E_UNEXPECTED);
	}

	m_poExecutor->ResetDebugging();

	LEAVE_METHOD(CWrapExecutor::ResetDebugging, S_OK);
}


STDMETHODIMP CWrapExecutor::SetExcludeFunction(BSTR ai_pccFunction, short *ao_RetVal)
{
	ENTER_METHOD( CWrapExecutor::SetExcludeFunction ) ;
	
	m4pchar_t	pcFunction ;
	gstring		tmpFunction( ai_pccFunction ) ;
	
	
	if( tmpFunction )
	{
		*ao_RetVal = Init( 0 ) ;

		if( *ao_RetVal == M4_SUCCESS )
		{
			pcFunction = m4pchar_t( tmpFunction ) ;
			strupr( pcFunction ) ;
			
			*ao_RetVal = m_poExecutor->SetExcludeFunction( pcFunction ) ;
		}
	}
	else
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create in string" ) ;
		*ao_RetVal = M4_ERROR ;
	}

	LEAVE_METHOD( CWrapExecutor::SetExcludeFunction,S_OK ) ;
}

STDMETHODIMP CWrapExecutor::UnsetExcludeFunction(BSTR ai_pccFunction, short *ao_RetVal)
{
	ENTER_METHOD( CWrapExecutor::UnsetExcludeFunction ) ;
	

	m4pchar_t	pcFunction ;
	gstring		tmpFunction( ai_pccFunction ) ;
	
	
	if( tmpFunction )
	{
		*ao_RetVal = Init( 0 ) ;

		if( *ao_RetVal == M4_SUCCESS )
		{
			pcFunction = m4pchar_t( tmpFunction ) ;
			strupr( pcFunction ) ;
			
			*ao_RetVal = m_poExecutor->UnsetExcludeFunction( pcFunction ) ;
		}
	}
	else
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create in string" ) ;
		*ao_RetVal = M4_ERROR ;
	}


	LEAVE_METHOD( CWrapExecutor::UnsetExcludeFunction,S_OK ) ;
}

STDMETHODIMP CWrapExecutor::ClearExcludeFunctions(short *ao_RetVal)
{
	ENTER_METHOD( CWrapExecutor::ClearExcludeFunctions ) ;
	
	*ao_RetVal = Init( 0 ) ;

	if( *ao_RetVal == M4_SUCCESS )
	{
		*ao_RetVal = m_poExecutor->ClearExcludeFunctions() ;
	}

	LEAVE_METHOD( CWrapExecutor::ClearExcludeFunctions,S_OK ) ;
}
