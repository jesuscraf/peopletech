
#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwraprgitfilter.h"
#include "wrapdef.h"
#include "rerror.h"
#include "filter_i.hpp"
#include "medvarin.hpp"
#include "regi_smp.hpp"


/////////////////////////////////////////////////////////////////////////////
// CIWrapRgItFilter


STDMETHODIMP CIWrapRgItFilter::Add(VARIANT ai_SourceOper, /*[in]*/int ai_type, /*[in]*/VARIANT ai_var, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::Add);

    gstring tmpGName;
    if (* ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t) 0;

    if (ai_SourceOper.vt == VT_BSTR)
    {
        gstring tmpGstring(ai_SourceOper.bstrVal);

        if (tmpGstring)
        {
		    TRACE2("Trying to add a filter with code %s", m4pchar_t(tmpGstring));

            SAFEARRAY *ai_saArguments = 0;

            //comprobamos que el variant sea un array:
            switch (ai_var.vt) {
                case VT_BYREF|VT_ARRAY|VT_BSTR:
                    ai_saArguments = *ai_var.pparray;
                    break;

                case VT_ARRAY|VT_BSTR:
                    ai_saArguments = ai_var.parray;
                    break;

                default:
                    TRACE("No array with arguments names detected");
            }

            m4pchar_t * pargsnames;
            gstring *pxxx;
            m4uint32_t iSize;

            *ao_RetVal = M4_SUCCESS;

            //no pasa nada, se mantiene el orden de evaluación:
            if (ai_saArguments != 0) {

                BSTR VVar;
		        m4uint32_t i,j;
                iSize = ai_saArguments->rgsabound[0].cElements;
		        m4uint32_t iLBound = ai_saArguments->rgsabound[0].lLbound;
		        m4uint32_t iUBound = iLBound + iSize;        

		        //array temporal:
                //m4VariantType *pvArguments = new m4VariantType [iSize];
                pxxx = new gstring [iSize];
                pargsnames = new m4pchar_t [iSize];            

		        for (j = 0, i = iLBound; i < iUBound; i++, j++) {
			        HRESULT hresult = SafeArrayGetElement(ai_saArguments, (m4int32_t *) &i, &VVar);

			        if (FAILED(hresult)) {
				        //Poner error
				        *ao_RetVal = M4_ERROR;
				        break;
			        }

                    pxxx[j] = VVar;
			        //pvArguments[j] = VVar;

                    /*mejora:
                    ClVariantConversor s_oVC;
                    m4return_t res = s_oVC.ToCharString (pvArguments[j]);
	                if (res != M4_SUCCESS) {
		                FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
				        //Poner error
				        *ao_RetVal = M4_ERROR;
                        break;
	                }
                    */

                    //pargsnames[j] = pvArguments[j].Data.PointerChar;
                    pargsnames[j] = pxxx[j];
				}
            }
            else {
                pargsnames = 0;
                pxxx = 0;
                iSize = 0;
            }

		    if (*ao_RetVal == M4_SUCCESS) {
                *ao_RetVal = m_poFI->AddNew(tmpGstring, (ClFilterInterface::FilterType)ai_type, iSize, pargsnames, NULL, tmpGName);
		    }

            if (pargsnames)
                delete [] pargsnames;

            if (pxxx)
                delete [] pxxx;

            if (*ao_RetVal == M4_SUCCESS) {
                LEAVE_METHOD(CIWrapRgItFilter::Add, S_OK);
            }
            else {
		        LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
            }
	    }
        else
        {
		    FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		    LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
    	}
    }
    else if (ai_SourceOper.vt == VT_UI1)
    {
        *ao_RetVal = m_poFI->AddNew((m4uint8_t)(1 + ai_SourceOper.bVal), (ClFilterInterface::FilterType)ai_type, tmpGName);
        if (*ao_RetVal == M4_SUCCESS) {
            LEAVE_METHOD(CIWrapRgItFilter::Add, S_OK);
        }
    	LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
    }
    else if (ai_SourceOper.vt == VT_I2)
    {
        *ao_RetVal = m_poFI->AddNew((m4uint8_t)(1 + ai_SourceOper.iVal), (ClFilterInterface::FilterType)ai_type, tmpGName);
        if (*ao_RetVal == M4_SUCCESS) {
            LEAVE_METHOD(CIWrapRgItFilter::Add, S_OK);
        }
    	LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
    }
    else if (ai_SourceOper.vt == VT_I4)
    {
        *ao_RetVal = m_poFI->AddNew((m4uint8_t)(1 + ai_SourceOper.lVal), (ClFilterInterface::FilterType)ai_type, tmpGName);
        if (*ao_RetVal == M4_SUCCESS) {
            LEAVE_METHOD(CIWrapRgItFilter::Add, S_OK);
        }
    	LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
    }
    else
    {
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert create filter");
		LEAVE_METHOD(CIWrapRgItFilter::Add, E_UNEXPECTED);
    }
}

STDMETHODIMP CIWrapRgItFilter::AddLN4 (/*[in]*/BSTR ai_stSourceCode, /*[in]*/int ai_type, /*[in]*/VARIANT ai_var, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddLN4);

	gstring tmpGstring(ai_stSourceCode);

    gstring tmpGName;
    if (* ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t)  0;

	if (tmpGstring)
	{
		TRACE2("Trying to add a filter with code %s", m4pchar_t(tmpGstring));

        SAFEARRAY *ai_saArguments = 0;

        //comprobamos que el variant sea un array:
        switch (ai_var.vt) {
            case VT_BYREF|VT_ARRAY|VT_BSTR:
                ai_saArguments = *ai_var.pparray;
                break;

            case VT_ARRAY|VT_BSTR:
                ai_saArguments = ai_var.parray;
                break;
            default:
                TRACE("No array with arguments names detected");
        }

        m4pchar_t * pargsnames;
        gstring *pxxx;
        m4uint32_t iSize;

        *ao_RetVal = M4_SUCCESS;

        //no pasa nada, se mantiene el orden de evaluación:
        if (ai_saArguments != 0) {

            BSTR VVar;
		    m4uint32_t i,j;
            iSize = ai_saArguments->rgsabound[0].cElements;
		    m4uint32_t iLBound = ai_saArguments->rgsabound[0].lLbound;
		    m4uint32_t iUBound = iLBound + iSize;        

            pxxx = new gstring [iSize];
            pargsnames = new m4pchar_t [iSize];            

		    for (j = 0, i = iLBound; i < iUBound; i++, j++) {
			    HRESULT hresult = SafeArrayGetElement(ai_saArguments, (m4int32_t *) &i, &VVar);
			    if (FAILED(hresult)) {
				    //Poner error
				    *ao_RetVal = M4_ERROR;
				    break;
			    }

                pxxx[j] = VVar;
                pargsnames[j] = pxxx[j];
		    }
        }
        else {
            pargsnames = 0;
            pxxx = 0;
            iSize = 0;
        }

		if (*ao_RetVal == M4_SUCCESS) {
            *ao_RetVal = m_poFI->AddNew(tmpGstring, (ClFilterInterface::FilterType)ai_type, iSize, pargsnames, NULL, tmpGName);
		}
        if (pargsnames)
            delete [] pargsnames;
       if (pxxx)
            delete [] pxxx;
        LEAVE_METHOD(CIWrapRgItFilter::AddLN4, S_OK);
	}
	else {
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CIWrapRgItFilter::AddLN4, E_UNEXPECTED);
	}
}

STDMETHODIMP CIWrapRgItFilter::AddOperation (/*[in]*/int ai_OperType, /*[in]*/int ai_type, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddOperation);

    gstring tmpGName;
    if (* ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t)  0;

    ai_OperType ++; //Estupidez mia al dar los valores en los defines. Sorry.

    *ao_RetVal = m_poFI->AddNew((m4uint8_t)ai_OperType, (ClFilterInterface::FilterType)ai_type, tmpGName);
    LEAVE_METHOD(CIWrapRgItFilter::AddOperation, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::AddSortById (/*[in]*/ long ai_IndexId, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddSortById);

    NamedSortById (ai_IndexId, 0, ao_RetVal);

    LEAVE_METHOD(CIWrapRgItFilter::AddSortById, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::AddSortByPos(/*[in]*/ short ai_iPos, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddSorByPost);

    NamedSortByPos(ai_iPos, 0, ao_RetVal);

    LEAVE_METHOD(CIWrapRgItFilter::AddSortByPos, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::AddSort(VARIANT ai_Items, VARIANT ai_Orders, short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddSort);

    NamedSort(ai_Items, ai_Orders, 0, ao_RetVal);

    LEAVE_METHOD(CIWrapRgItFilter::AddSort, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::NamedSortById (/*[in]*/ long ai_IndexId, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::NamedSortById);

    gstring tmpGName;
    if (ai_stName && * ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t)  0;

    *ao_RetVal = m_poFI->AddSort((m4uint32_t)ai_IndexId, tmpGName);

    LEAVE_METHOD(CIWrapRgItFilter::NamedSortById, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::NamedSortByPos(/*[in]*/ short ai_iPos, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::NamedSortByPos);

    gstring tmpGName;
    if (ai_stName && * ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t)  0;

    *ao_RetVal = m_poFI->AddSort((m4uint16_t)ai_iPos, tmpGName);

    LEAVE_METHOD(CIWrapRgItFilter::NamedSortByPos, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::NamedSort(VARIANT ai_Items, VARIANT ai_Orders, /*[in]*/BSTR ai_stName, short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::NamedSort);

    SAFEARRAY *ItemNames = 0;
    SAFEARRAY *Orders = 0;

    gstring tmpGName;
    if (ai_stName && * ai_stName)
	    tmpGName = ai_stName;
    else 
        tmpGName = (m4pchar_t)  0;

    switch (ai_Items.vt) {
        case VT_BYREF|VT_ARRAY|VT_BSTR:
            ItemNames = *ai_Items.pparray;
            break;

        case VT_ARRAY|VT_BSTR:
            ItemNames = ai_Items.parray;
            break;

        default:
            TRACE("No array with arguments names detected");
    }

    switch (ai_Orders.vt) {
        case VT_BYREF|VT_ARRAY|VT_I2:
            Orders = *ai_Orders.pparray;
            break;

        case VT_ARRAY|VT_I2:
            Orders = ai_Orders.parray;
            break;

        case VT_BYREF|VT_ARRAY|VT_I4:
            Orders = *ai_Orders.pparray;
            break;

        case VT_ARRAY|VT_I4:
            Orders = ai_Orders.parray;
            break;

        default:
            TRACE("No array with arguments names detected");
    }

    *ao_RetVal = M4_SUCCESS;

    //no pasa nada, se mantiene el orden de evaluación:
    if ((ItemNames != 0)&&(Orders != 0))
    {
        m4pchar_t * pItemsNames = 0;
        gstring *pIN = 0;
        m4uint32_t iSize;
        m4uint8_t * pOr = 0;

        BSTR VIN;
        m4uint32_t VOr;
		m4uint32_t i,j;
        iSize = ItemNames->rgsabound[0].cElements;
		m4uint32_t iLBound = ItemNames->rgsabound[0].lLbound;
		m4uint32_t iUBound = iLBound + iSize;
        pIN = new gstring [iSize];
        pOr = new m4uint8_t [iSize];
        pItemsNames = new m4pchar_t [iSize];

	    for (j = 0, i = iLBound; i < iUBound; i++, j++)
        {
		    HRESULT hresult1 = SafeArrayGetElement(ItemNames, (m4int32_t *) &i, &VIN);
		    HRESULT hresult2 = SafeArrayGetElement(Orders, (m4int32_t *) &i, &VOr);

		    if (FAILED(hresult1)||FAILED(hresult2)) {
			    //Poner error
			    *ao_RetVal = M4_ERROR;
			    break;
		    }

            pIN[j] = VIN;
            pItemsNames[j] = pIN[j];
            pOr[j] = (m4uint8_t)VOr;
	    }
		if (*ao_RetVal == M4_SUCCESS) {
            *ao_RetVal = m_poFI->AddSort((m4int16_t)iSize, pItemsNames, pOr, tmpGName);
		}

        if (pIN)
            delete [] pIN;
        if (pOr)
            delete [] pOr;
        if (pItemsNames)
            delete [] pItemsNames;

        LEAVE_METHOD(CIWrapRgItFilter::NamedSort, S_OK);
    }
    else
    {
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CIWrapRgItFilter::NamedSort, E_UNEXPECTED);
    }
}

STDMETHODIMP CIWrapRgItFilter::AddFast(BSTR ai_pItemName, VARIANT ai_Value, int ai_type, BSTR ai_stName, short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::AddFast);

    gstring tmpGFilterName;
    if (* ai_stName)
	    tmpGFilterName = ai_stName;
    else 
        tmpGFilterName = (m4pchar_t)  0;

    gstring tmpGItemName;
    if (* ai_pItemName)
	    tmpGItemName = ai_pItemName;
    else 
        tmpGItemName = (m4pchar_t)  0;

	*ao_RetVal = m_poFI->AddFastFilter (tmpGItemName, (m4VariantType) ai_Value, (ClFilterInterface::FilterType)ai_type, tmpGFilterName);

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::AddFast, S_OK);
}


STDMETHODIMP CIWrapRgItFilter::DeleteLast(short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::DeleteLast);

	*ao_RetVal = m_poFI->DeleteLast ();

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::DeleteLast, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::Delete(BSTR ai_stName, short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::Delete);

    _ClRegisterFilter *pfilter = 0;

	gstring tmpGstring(ai_stName);
	pfilter = &(*m_poFI)[tmpGstring];

	*ao_RetVal = pfilter->Delete();

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::Delete, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::Clear(short* ao_RetVal)
{
	// Se expone en COM el método Clear (en lugar de RemoveAll) ya que en .NET aunque aparece no ejecuta nada en la invocación
	ENTER_METHOD(CIWrapRgItFilter::Clear);

	*ao_RetVal = m_poFI->DeleteAll ();

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::Clear, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::Copy(IWrapRgItFilter ** ai_RgItFilter, short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::Copy);

	*ao_RetVal = m_poFI->CopyFilter ( *( ((CIWrapRgItFilter*)*ai_RgItFilter)->m_poFI ) );

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::Copy, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::get_Count (short* ao_iCount)
{
	ENTER_METHOD(CIWrapRgItFilter::get_Count);

	*ao_iCount = m_poFI->Count ();

	TRACE2("Returning a count of %d", *ao_iCount);
	LEAVE_METHOD(CIWrapRgItFilter::get_Count, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::Item (VARIANT ai_var, IWrapFilter** ao_poFilter)
{
	ENTER_METHOD(CIWrapRgItFilter::Item);

    _ClRegisterFilter *pfilter = 0;

	switch (ai_var.vt)
	{
	case VT_BSTR:
        {
            gstring tmpGstring(ai_var.bstrVal);
		    pfilter = &(*m_poFI)[tmpGstring];
        }
        break;

	case VT_UI1:
        pfilter = &(*m_poFI)[ai_var.bVal];
		break;

	case VT_I2:
        pfilter = &(*m_poFI)[ai_var.iVal];
		break;

	case VT_I4:
        pfilter = &(*m_poFI)[(m4int16_t)(ai_var.lVal)];
		break;

    default:
        {
		FormatError(M4_CH_WO_INVALID_GET_ITEM, 
				"Filter %i don't exist",
				ai_var.vt);
		LEAVE_METHOD(CIWrapRgItFilter::Item, E_UNEXPECTED);
        }
    }

	*ao_poFilter = this;

    ((CIWrapFilter*)(*ao_poFilter))->Init (pfilter);
	((IUnknown*)*ao_poFilter)->AddRef();

    LEAVE_METHOD(CIWrapRgItFilter::Item, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::RefreshAll(short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgItFilter::RefreshAll);

	*ao_RetVal = m_poFI->UpdateFilter ();

    TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgItFilter::RefreshAll, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::Exists(BSTR ai_stName, VARIANT_BOOL* ao_pbVal)
{
	ENTER_METHOD(CIWrapRgItFilter::Exists);

    gstring tmpGstring(ai_stName);
	_ClRegisterFilter *pfilter = &(*m_poFI)[tmpGstring];

	if (pfilter->IsTerminator())
	{
		*ao_pbVal = VARIANT_FALSE;
		TRACE("Returning False");
	}
	else
	{
		*ao_pbVal = VARIANT_TRUE;
		TRACE("Returning True");
	}

	LEAVE_METHOD(CIWrapRgItFilter::Exists, S_OK);
}

STDMETHODIMP CIWrapRgItFilter::NameByPos(short ai_iPos, BSTR* ao_pstName)
{
	ENTER_METHOD(CIWrapRgItFilter::NameByPos);

	_ClRegisterFilter *pfilter = &(*m_poFI)[ai_iPos];
    m4VariantType v;
    m4return_t res = pfilter->GetName (v);

    if (res == M4_SUCCESS) {
	    gstring tmpGstring (v.Data.PointerChar);

		*ao_pstName = (BSTR)tmpGstring;
	}
	else
	{
		*ao_pstName = NULL;
	}

	LEAVE_METHOD(CIWrapRgItFilter::NameByPos, S_OK);
}

//----------------------------------------------------------------------------

CIWrapRgItFilter::CIWrapRgItFilter ()
{
    m_poFI = 0;
}

void CIWrapRgItFilter::Init (ClFilterInterface *ai_pofi)
{
    m_poFI = ai_pofi;
}

STDMETHODIMP CIWrapRgItFilter::BFind(SAFEARRAY * * ai_saArguments, VARIANT_BOOL * ao_pbFound, VARIANT_BOOL * ao_pbVal)
{
	HRESULT hresult;
	VARIANT VVar;
	m4VariantType * pvArguments;
	m4bool_t bOK = M4_TRUE;
	
	ENTER_METHOD(CIWrapRgItFilter::BFind);
	
	if ((*ai_saArguments)->cDims != 1)
	{
		//Poner error
		bOK = M4_FALSE;
	}
	else
	{
		m4uint32_t i,j;
		m4uint32_t iSize = (*ai_saArguments)->rgsabound[0].cElements;
		m4uint32_t iLBound = (*ai_saArguments)->rgsabound[0].lLbound;
		m4uint32_t iUBound = iLBound + iSize;

		//pequeña chapuza con los variants en tanto se unifican en una sola clase.
		//little  chapuzation with the variants.
		pvArguments = new m4VariantType [iSize];
		//ppTempVar = new ClTmpVar* [iSize];

		for (j = 0, i = iLBound; i < iUBound; i++, j++)
		{
			hresult = SafeArrayGetElement(*ai_saArguments,(m4int32_t *) &i, &VVar);

			if (FAILED(hresult))
			{
				//Poner error
				bOK = M4_FALSE;
				break;
			}

			//Convert de VARIANT
			pvArguments[j] = VVar;

		}

		if (bOK) 
		{
			bOK = m_poFI->Find (pvArguments, 0);
		}

		//delete de intermediate arrays
		delete 	[] pvArguments; 
	}

	if (bOK)
	{
		*ao_pbVal = VARIANT_TRUE;
		*ao_pbFound = VARIANT_TRUE;
	}
	else
	{
		*ao_pbVal = VARIANT_FALSE;
		*ao_pbFound = VARIANT_FALSE;
	}

	LEAVE_METHOD(CIWrapRgItFilter::BFind, S_OK);
}

