// WrapExecutor.h : Declaration of the CWrapExecutor

#ifndef __WRAPEXECUTOR_H_
#define __WRAPEXECUTOR_H_

#include "resource.h"       // main symbols
#include "m4types.hpp"

#include "executor.hpp"
/////////////////////////////////////////////////////////////////////////////
// CWrapExecutor
class ATL_NO_VTABLE CWrapExecutor : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapExecutor, &CLSID_WrapExecutor>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapExecutor, &IID_IWrapExecutor, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IWrapStack, &IID_IWrapStack, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IWrapStepper, &IID_IWrapStepper, &LIBID_CHANNELWRAPPERLib>


{
public:
	CWrapExecutor( void )
	{
		m_poExecutor = NULL ;
		m_bOwner = M4_FALSE ;
		m_poFactory = NULL ;
	}

	~CWrapExecutor( void ){
		if (m_bOwner && m_poExecutor!= 0 ){
			delete m_poExecutor; //lo hemos creado nosotros y nosotros nos lo cepillamos
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPEXECUTOR)

BEGIN_COM_MAP(CWrapExecutor)
	COM_INTERFACE_ENTRY(IWrapExecutor)
	COM_INTERFACE_ENTRY(IWrapStack)
	COM_INTERFACE_ENTRY(IWrapStepper)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapExecutor)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapStack)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapStepper)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapExecutor
public:
	STDMETHOD(ClearExcludeFunctions)(/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(UnsetExcludeFunction)(/*[in]*/ BSTR ai_pccFunction, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(SetExcludeFunction)(/*[in]*/ BSTR ai_pccFunction, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(NextValidBreakPoint)(/*[in]*/ IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(Toggle)(/*[in]*/ IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(RunToCursor)(/*[in]*/ IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(StepOut)(/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(StepOver)(/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(StepInto)(/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(SetArgumentValue)(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[in]*/ VARIANT ai_poVar, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetArgumentValue)(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[out]*/ VARIANT *ao_poVar, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetNumberOfArguments)(/*[in]*/ int ai_iCallStackPos, /*[out, retval]*/ int *ao_RetVal);
	STDMETHOD(get_Stack)(/*[out, retval]*/ IWrapStack* *pVal);
	STDMETHOD(get_HasRegister)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_HasSlice)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_RuleNumber)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ChannelId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NodeId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ItemId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Argument)(/*[in]*/short ai_iPos, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Argument)(/*[in]*/short ai_iPos, /*[in]*/ VARIANT newVal);
	STDMETHOD(get_NumberOfArguments)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_HasVariableArguments)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Level)(/*[out, retval]*/ short *pVal);
	STDMETHOD(Continue)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Abort)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Call)(/*[in]*/IWrapItIt *ai_powItem, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(get_EndDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_StartDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_ExecutionDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_RunSliceMode)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_RunSliceMode)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Reset)();
	STDMETHOD(Push)(/*[in]*/ VARIANT ai_varref, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Pop)(/*[out]*/ VARIANT* ao_varref, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(GetSttStatus)(/*[out, retval]*/ short *pVal);
	STDMETHOD(SetSttStatus)(/*[in]*/ short newVal, /*[in]*/ IWrapCVM * ai_pIWapCVM);
	STDMETHOD(SetSttLabel)(/*[in]*/ BSTR ai_pstName);
	STDMETHOD(GetSttLabel)(/*[out, retval]*/ BSTR* ao_pstLabel);
	STDMETHOD(GetSttChannel)(/*[out, retval]*/ IWrapChannel** pVal);
	STDMETHOD(FreeSttChannel)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(GetSttPause)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(SetSttPause)(/*[in]*/ VARIANT_BOOL newVal);

	//metodos de executor relacionados con stepper
	STDMETHOD(get_Stepper)(/*[out, retval]*/ IWrapStepper* *pVal);
		
	STDMETHOD(GetCallStackSize)(/*[out, retval]*/ int * ao_RetVal);
	STDMETHOD(GetCallStackInfo)(/*[in]*/ int ai_iCallStackPos, /*[out]*/ IWrapRunningFormInfo **ao_poInfo ,/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(GetVariableValue)(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[out]*/ VARIANT *ao_poVar ,/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(SetVariableValue)(/*[in]*/ int ai_iCallStackPos, /*[in]*/ int ai_iPos, /*[in]*/ VARIANT ai_poVar ,/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(GetSymbolTable)  (/*[in]*/ IWrapItIt *ai_poItem, /*[in]*/ BSTR ai_pstCode,/*[in]*/ int ai_iNumVars, /*[out]*/ SAFEARRAY** ao_poSymbols ,/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(GetNumberOfVariables) (/*[in]*/ int ai_iCallStackPos,/*[out, retval]*/ int * ao_RetVal);

	//Métodos del interfaz de stepper
	STDMETHOD(get_Active)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Active)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Mode)(/*[out, retval]*/ STEPPERMODE *pVal);
	STDMETHOD(put_Mode)(/*[in]*/ STEPPERMODE newVal);
	STDMETHOD(RemoveAll)(/*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(GetBPSize)(/*[out, retval]*/ int * ao_RetVal);
	STDMETHOD(Add)(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(RemoveByID)(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(RemoveByPos)(/*[in]*/int ai_iBP, /*[out, retval]*/ short * ao_RetVal);
	STDMETHOD(GetByPos)(/*[in]*/int ai_iBP, /*[out, retval]*/ IWrapBreakpoint** pVal);
	STDMETHOD(GetByID)(/*[in]*/IWrapBreakpoint *ai_poBP, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(SetStepOverReference)(/*[in]*/ int ai_iCallStackPos,/*[out, retval]*/ short * ao_RetVal);

	STDMETHOD(ResetExecutor)(void);
	STDMETHOD(ResetDebugging)(void);


	void	SetFactory( ClCMCRFactory *ai_poFactory )
	{
		m_poFactory = ai_poFactory ;
	}

	m4return_t Init (ClExecutor *ai_poExecutor);
private:
	ClExecutor  *m_poExecutor;
	m4bool_t	m_bOwner;

	// Para los breakpoints
	ClCMCRFactory *m_poFactory ;
};

#endif //__WRAPEXECUTOR_H_
