// DGenericObject.cpp : Implementation of CDGenericObject
#include "stdafx.h"
#include "m4prop.h"
#include "propcol.h"
#include "containedobjects.h"
#include "dgenericobject.h"

/////////////////////////////////////////////////////////////////////////////
// CDGenericObject
STDMETHODIMP CDGenericObject::Construct(IDispatch ** oFact, IDispatch** DesControl, BSTR* strItemAlias)
{
	// La Factoría no nos interesa, nos interesa el DesControl y el Alias
	// Toca:
	//
	//	ResetProps
	//  m_pbGeneral.Alias = strItemAlias
	//  Set m_DesignObject = DesControl
	//  Set m_ContObj = new ContainedObjects   - Ya creado en el Constructor

	if ((DesControl == NULL) || (*DesControl == NULL) || (strItemAlias == NULL))
	{
		// No vale
		return E_INVALIDARG;
	}
	// Vamos a protejernos de la asignación de un ClassName, es indispensable para
	// poder continuar.

	//if (wcscmp(m_strClassName, L"") == 0) return E_INVALIDARG;

	HRESULT hr = S_OK;
	
	RELEASE(m_pFEDesControl);
	hr = (*DesControl)->QueryInterface(IID_IFEDesControl, (void**)&m_pFEDesControl);
	//Iveranle * pAnle = NULL;
	if (hr == S_OK)
	{
		//ResetProps();
		//if (m_pbGeneral == NULL) return E_INVALIDARG;
		//m_pbGeneral->put_Alias(*strItemAlias);
		RELEASE(m_pContainer);
		IDispatch * pDisp = NULL;
		m_pFEDesControl->GetObjectByKey(&m_strContainer, &pDisp);
		if (pDisp)
		{
			pDisp->QueryInterface(IID_IDesignObject, (void**)&m_pContainer);
			RELEASE(pDisp);
		}
	}

	return hr;
}

STDMETHODIMP CDGenericObject::Destruct(IDispatch ** oFact)
{

	DestructChild();
	RELEASE(m_pContainer);
	RELEASE(m_pFEDesControl);
	RELEASE(m_pContObj);

	return S_OK;
}


STDMETHODIMP CDGenericObject::Persist2(ICPropBagDef** pbd,VARIANT_BOOL* bPersist)
{
	return S_OK;
}

STDMETHODIMP CDGenericObject::get_Alias(BSTR* bstrAlias)
{
	if (m_pbGeneral)
		m_pbGeneral->get_Alias(bstrAlias);

	return S_OK;
}

STDMETHODIMP CDGenericObject::put_Alias(BSTR* bstrAlias)
{
	if (m_pbGeneral)
		m_pbGeneral->put_Alias(*bstrAlias);

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_Path(BSTR* bstrPath)
{
	if (m_pFEDesControl)
	{
		IDesignObject * pMe = static_cast<IDesignObject*>(this);
		m_pFEDesControl->GetPathByDesignObject(&pMe, bstrPath );
	}
	
	return S_OK;
}

STDMETHODIMP CDGenericObject::get_ClassName(BSTR* bstrClassName)
{
	if (m_pbGeneral)
		return m_pbGeneral->get_ClassName(bstrClassName);

	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_ClassName(BSTR* bstrClassName)
{
	if (m_pbGeneral)
		return m_pbGeneral->put_ClassName(*bstrClassName);
	
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_Key(BSTR* bstrKey)
{
	if (m_pbGeneral)
		return m_pbGeneral->get_Key(bstrKey);

	return E_INVALIDARG;
}

STDMETHODIMP CDGenericObject::put_Key(BSTR* bstrKey)
{
	return E_INVALIDARG;
}

STDMETHODIMP CDGenericObject::get_Container(BSTR* bstrContainer)
{
	if (bstrContainer)
		*bstrContainer = ::SysAllocString(m_strContainer);
	else
		*bstrContainer = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDGenericObject::put_Container(BSTR* bstrContainer)
{
	RELEASE_BSTR(m_strContainer);
	if (*bstrContainer)
		m_strContainer = ::SysAllocString(*bstrContainer);

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_ProgId(BSTR* bstrProgId)
{
	*bstrProgId = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDGenericObject::SetInitialPBDef(ICPropBagDef * pPBD)
{

	RELEASE(m_pbGeneral);
	RELEASE(m_pData);
	if (pPBD)
	{
		m_pbGeneral = pPBD;
		m_pbGeneral->AddRef();
		m_pData = GetDataProperties(m_pbGeneral);
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_ExecOrder(short* nExecOrder)
{

	if (nExecOrder)
		*nExecOrder = (short) m_nExecOrder;
	return S_OK;
}

STDMETHODIMP CDGenericObject::put_ExecOrder(short* nExecOrder)
{
	if (m_nExecOrder != *nExecOrder)
	{
		m_nExecOrder = *nExecOrder;
		if (m_pContainer)
		{
			// Obtenemos la Key
			BSTR bstrKey = NULL;
			m_pbGeneral->get_Key(&bstrKey);
			m_pFEDesControl->OrderExecOrder(&m_pContainer, &bstrKey, nExecOrder);
			::SysFreeString(bstrKey);
		}
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_PropertyCol(IPropCol** ppPropCol)
{

	HRESULT hr = E_INVALIDARG;

	if (ppPropCol)
	{
		if (*ppPropCol)
		{
			RELEASE((*ppPropCol));
		}
		CComObject<CPropCol> * pPropCol;
		if (CComObject<CPropCol>::CreateInstance(&pPropCol) == S_OK)
		{
			hr = pPropCol->QueryInterface(IID_IPropCol, (void**)ppPropCol);	
			if (hr == S_OK)
			{
				AddPBDef2PC(*ppPropCol, m_pbGeneral);
			}
		}
	}
	
	return hr;
}

STDMETHODIMP CDGenericObject::put_PropertyCol(IPropCol** ppPropCol)
{

	// Primero. Posibilidad de cambio de Alias
	HRESULT hr = E_FAIL;
	ICProperty * pProp = NULL;
	CComVariant vValue;
	
	hr = (*ppPropCol)->get_Item(M4CComVariant("Alias"), &pProp);
	if (hr == S_OK)
	{
		pProp->get_Value(&vValue);
		RELEASE(pProp);
		if (vValue.vt == VT_BSTR)
		{
			m_pbGeneral->put_Alias(vValue.bstrVal);	
		}
	}
	vValue.Clear();

	if (m_pFEDesControl)
	{
		IDesignObject *	pMe = static_cast<IDesignObject*>(this);
		IDataProperties * pData = NULL;
		ICProperty * pSP = NULL;
		if ((*ppPropCol)->get_Item(M4CComVariant("DataProps"), &pSP) == S_OK)
		{
			pSP->get_Value(&vValue);
			if (vValue.vt == VT_DISPATCH)
			{
				pData = (IDataProperties*)vValue.pdispVal;
				m_pFEDesControl->ApplyInheritance(&pMe, &pData);
			}
			RELEASE(pSP);
		}
	}
	vValue.Clear();
	GetPBDefFromPC(m_pbGeneral, *ppPropCol);
	
	// Modificamos el ExecOrder
	hr = (*ppPropCol)->get_Item(M4CComVariant("Execorder"), &pProp);
	if (hr == S_OK)
	{
		vValue.Clear();
		short nExec = 0;
		pProp->get_Value(&vValue);
		
		if (vValue.vt == VT_I4)
		{
			nExec = (short)vValue.lVal;
		}
		else if (vValue.vt == VT_I2)
		{
			nExec = (short)vValue.iVal;
		}
		else if (vValue.vt == VT_UI1)
		{
			nExec = (short)vValue.bVal;
		}
		put_ExecOrder(&nExec);
		RELEASE(pProp);
	}

	RELEASE(m_pData);
	m_pData = GetDataProperties(m_pbGeneral);

	if (m_pFEDesControl)
	{
		BSTR strKey = NULL;
		m_pbGeneral->get_Key(&strKey);
		m_pFEDesControl->UpdateTree(&strKey);
		::SysFreeString(strKey);
		VARIANT_BOOL bMod = -1;
		m_pFEDesControl->put_Modified(&bMod);
	}

	return S_OK;
}


STDMETHODIMP CDGenericObject::get_Toolstring(BSTR* bstrToolString)
{
	return S_OK;
}

STDMETHODIMP CDGenericObject::ShowProp()
{
	if (m_pFEDesControl)
	{
		BSTR strKey = NULL;
		m_pbGeneral->get_Key(&strKey);
		m_pFEDesControl->ShowGenericProperties(&strKey);
		::SysFreeString(strKey);
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_InToolbar(VARIANT_BOOL* bInToolbar)
{
	// Nunca se encuentra en la Toolbar
	if (bInToolbar)
	{
		*bInToolbar = 0;
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_ContainedObjects(IContainedObjects** ppContainedObjects)
{
	if (ppContainedObjects == NULL)
	{
		return E_INVALIDARG;
	}

	if (m_pContObj)
	{
		*ppContainedObjects = m_pContObj;
		m_pContObj->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_Count(short* pnCount)
{

	long nCount = 0;

	if (m_pContObj)
	{
		m_pContObj->get_Count(&nCount);
		if (pnCount)
		{
			*pnCount = (short)nCount;
		}
	}
	return S_OK;
}

STDMETHODIMP CDGenericObject::AddControl(IDispatch** newValue,BSTR* strKey)
{
	
	if ((newValue == NULL) || (*newValue == NULL) || (wcscmp(*strKey, L"") == 0))
	{
		return E_INVALIDARG;
	}

	IDesignObject * pDesObject = NULL;

	HRESULT hr = (*newValue)->QueryInterface(IID_IDesignObject, (void**)&pDesObject);
	if (hr == S_OK)
	{
		hr = m_pContObj->Add(*newValue, CComVariant(*strKey));
		long nNewExec = 0;
		m_pContObj->get_Count(&nNewExec);
		pDesObject->put_ExecOrder((short*)&nNewExec);
		RELEASE(pDesObject);
	}

	return hr;
}

STDMETHODIMP CDGenericObject::RemoveControl(BSTR* strKey)
{

	if (m_pContObj)
	{
		m_pContObj->Remove(CComVariant(*strKey));
	}
	if (m_pbGeneral)
	{
		ICChildColDef * pChildCol = GetChildColDef(m_pbGeneral);
		if (pChildCol)
		{
			pChildCol->Remove(CComVariant(*strKey));
			RELEASE(pChildCol);
		}
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::GetNextExecOrder(short* pnExec)
{
	long nCount = 0;
	
	if (m_pContObj)
	{
		m_pContObj->get_Count(&nCount);
		if (pnExec)
		{
			*pnExec = (short) nCount;
		}
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::DestructChild()
{

	IEnumVARIANT	* pEnum;
	IUnknown		* pUnk;
	HRESULT hr = E_INVALIDARG;
	IDesignObject	* pObject;

	if (m_pContObj)
	{
		hr = m_pContObj->get__NewEnum(&pUnk);
		if (hr == S_OK)
		{
			hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			if (hr == S_OK)
			{
				unsigned long num = 0;
				CComVariant vIn;
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);
				while ((vIn.vt==VT_DISPATCH)&&(num==1))
				{
					
					hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_IDesignObject, (void**)&pObject);
					if (hr == S_OK)
					{
						pObject->Destruct(NULL);					
						if (m_pFEDesControl)
						{
							BSTR bstrKey = NULL;
							pObject->get_Key(&bstrKey);
							m_pFEDesControl->RemoveControl(&bstrKey);
							::SysFreeString(bstrKey);
						}
						RELEASE(pObject);
					}
					pEnum->Next(1,&vIn,&num);
				}
				RELEASE(pEnum);
			}
			RELEASE(pUnk);
		}
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::PostConstruct()
{
	// Nada de nada

	return S_OK;
}

STDMETHODIMP CDGenericObject::ValidateInsertion(BSTR* strClassName,VARIANT_BOOL* bValidateInsert)
{
	if (bValidateInsert)
	{
		*bValidateInsert = -1;
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_VisualName(BSTR* bstrVisualName)
{
	if (m_pbGeneral)
		m_pbGeneral->get_Alias(bstrVisualName);	
	else
		*bstrVisualName = ::SysAllocString(L"");

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_CanRemoveNow(VARIANT_BOOL* bCanRemoveNow)
{
	// Este objeto es (en principio) eliminable en cualquier momento
	if (bCanRemoveNow)
		*bCanRemoveNow = -1;

	return S_OK;
}

STDMETHODIMP CDGenericObject::RefreshView()
{
	// Na de na

	return S_OK;
}


STDMETHODIMP CDGenericObject::Activate(IDispatch** pFactor,IDispatch** theDesign)
{
	// No se hace nada de Nada
	return S_OK;
}

STDMETHODIMP CDGenericObject::Deactivate()
{
	// No aplicable

	return S_OK;
}

STDMETHODIMP CDGenericObject::Constructed(VARIANT_BOOL* bConstructed)
{
	// Nunca tiene capa Visual
	if (bConstructed)
		*bConstructed = 0;

	return S_OK;
}

STDMETHODIMP CDGenericObject::CheckState(IDesignObject** ADesignObject)
{

	if (m_pContainer)
	{
		IDesignObject *	pMe = static_cast<IDesignObject*>(this);
		pMe->AddRef();
		m_pContainer->CheckState(&pMe);
		RELEASE(pMe);
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_ReferencedObjects(IContainedObjects** ppContainedObjects)
{
	// Na de na

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_PropBagDef(ICPropBagDef** ppPropBagDef)
{
	if (ppPropBagDef == NULL)
	{
		return E_INVALIDARG;
	}

	if (m_pbGeneral)
	{
		*ppPropBagDef = m_pbGeneral;
		m_pbGeneral->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CDGenericObject::get_DataProps(IDataProperties** ppDataProperties)
{
	if (ppDataProperties == NULL)
	{
		return E_INVALIDARG;
	}

	if (m_pData)
	{
		*ppDataProperties = m_pData;
		m_pData->AddRef();
	}
	
	return S_OK;
}
    


STDMETHODIMP CDGenericObject::AssignInheritedProps(ICPropBag ** pb)
{

/*    ' Nos autoasignamos la interface para acceder a las propiedades
    Dim tmpIDataProps As IDataProps

    Set tmpIDataProps = Me
    
    tmpIDataProps.Grants = pb.Property(PT_PRO_GRANTS, 31)
    tmpIDataProps.IdAccessAlias = pb.Property(PT_PRO_IDACCESSKEY, "")
    tmpIDataProps.IdChannel = pb.Property(PT_PRO_IDCHANNEL, "")
    tmpIDataProps.IdChannelAlias = pb.Property(PT_PRO_IDCHANNELALIAS, "")
    tmpIDataProps.IdChannelKey = pb.Property(PT_PRO_IDCHANNELKEY, "")
    tmpIDataProps.IdItem = pb.Property(PT_PRO_IDITEM, "")
    tmpIDataProps.IdNode = pb.Property(PT_PRO_IDNODE, "")
    tmpIDataProps.IdPresentation = pb.Property(PT_PRO_IDPRESENTATION, -1)
    
    Set tmpIDataProps = Nothing*/
	
	if (m_pData && *pb)
	{
		CComVariant vValue, vDefValue;
		
		vDefValue = (long) 31;
		(*pb)->Property(L"Grants", &vDefValue, &vValue);
		if (vValue.vt == VT_UI1)
		{
			m_pData->put_Grants((short)vValue.iVal);
		}
		else if (vValue.vt == VT_I2)
		{
			m_pData->put_Grants((short)vValue.bVal);
		}
		else if (vValue.vt == VT_I4)
		{
			m_pData->put_Grants((short)vValue.lVal);
		}

		vDefValue = (long) -6;
		(*pb)->Property(L"Startpos", &vDefValue, &vValue);
		if (vValue.vt == VT_UI1)
		{
			m_pData->put_Grants((short)vValue.iVal);
		}
		else if (vValue.vt == VT_I2)
		{
			m_pData->put_Grants((short)vValue.bVal);
		}
		else if (vValue.vt == VT_I4)
		{
			m_pData->put_Grants((short)vValue.lVal);
		}

		vDefValue = (long) -1;
		(*pb)->Property(L"Idpresentation", &vDefValue, &vValue);
		if (vValue.vt == VT_UI1)
		{
			m_pData->put_Grants((short)vValue.iVal);
		}
		else if (vValue.vt == VT_I2)
		{
			m_pData->put_Grants((short)vValue.bVal);
		}
		else if (vValue.vt == VT_I4)
		{
			m_pData->put_Grants((short)vValue.lVal);
		}

		vDefValue = L"";
		(*pb)->Property(L"Idaccessalias", &vDefValue, &vValue);
		m_pData->put_IdAccessAlias(vValue.bstrVal);

		vDefValue = L"";
		(*pb)->Property(L"Idchannel", &vDefValue, &vValue);
		m_pData->put_IdChannel(vValue.bstrVal);

		vDefValue = L"";
		(*pb)->Property(L"Idchannelalias", &vDefValue, &vValue);
		m_pData->put_IdChannelAlias(vValue.bstrVal);

		vDefValue = L"";
		(*pb)->Property(L"Idchannelkey", &vDefValue, &vValue);
		m_pData->put_IdChannelKey(vValue.bstrVal);

		vDefValue = L"";
		(*pb)->Property(L"Iditem", &vDefValue, &vValue);
		m_pData->put_IdItem(vValue.bstrVal);

		vDefValue = L"";
		(*pb)->Property(L"IdNode", &vDefValue, &vValue);
		m_pData->put_IdNode(vValue.bstrVal);
	}
	return S_OK;
}


STDMETHODIMP CDGenericObject::ResetProps()
{
	if (m_pbGeneral)
	{
		// La reseteamos de forma interna
		m_pbGeneral->Reset();
		RELEASE(m_pData);
		m_pData = GetDataProperties(m_pbGeneral);
	}
	return S_OK;
}


STDMETHODIMP CDGenericObject::get_IdChannel(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdChannel(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdChannel(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdChannel(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdChannelAlias(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdChannelAlias(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdChannelAlias(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdChannelAlias(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdChannelKey(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdChannelKey(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdChannelKey(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdChannelKey(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdNode(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdNode(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdNode(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdNode(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdItem(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdItem(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdItem(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdItem(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdAccessAlias(BSTR * pVal)
{
	if (m_pData)
		return m_pData->get_IdAccessAlias(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdAccessAlias(BSTR *newVal)
{
	if (m_pData)
		return m_pData->put_IdAccessAlias(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_IdPresentation(long * pVal)
{
	if (m_pData)
		return m_pData->get_IdPresentation(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_IdPresentation(long *newVal)
{
	if (m_pData)
		return m_pData->put_IdPresentation(*newVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::get_Grants(short * pVal)
{
	if (m_pData)
		return m_pData->get_Grants(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_Grants(short * newVal)
{
	if (m_pData)
		return m_pData->put_Grants(*newVal);
	return S_OK;
}


STDMETHODIMP CDGenericObject::get_StartPos(long * pVal)
{
	if (m_pData)
		return m_pData->get_StartPos(pVal);
	return E_FAIL;
}

STDMETHODIMP CDGenericObject::put_StartPos(long newVal)
{
	if (m_pData)
		return m_pData->put_StartPos(newVal);
	return E_FAIL;
}


STDMETHODIMP CDGenericObject::InitializePopPup(IMenuPopPup * tpp)
{

	long i = 0;
	IMenuItem * pMenuItem = NULL;
	HRESULT hr = S_OK;

	if (tpp)
	{
		tpp->get_Count(&i);
		for (long j = 1; j<= i; j++)
		{
			BSTR bstrKey = NULL;
			hr = tpp->get_Item(CComVariant(j), &pMenuItem);
			if (hr == S_OK)
			{
				pMenuItem->get_Key(&bstrKey);
				if (wcsncmp( bstrKey,L"ShowForm", 8) == 0)
				{
					pMenuItem->put_Enabled(0);
				}
				RELEASE_BSTR(bstrKey);
				RELEASE(pMenuItem);
			}
		}
	}
	else
		return E_FAIL;

	return S_OK;
}


STDMETHODIMP CDGenericObject::ExecuteMenu(BSTR* Tag)
{
	if (Tag)
	{
		if (*Tag)
		{
			if (wcscmp(*Tag, L"PROPERTIES")==0)
			{
				ShowProp();
			}
		}
	}

	return S_OK;
}
