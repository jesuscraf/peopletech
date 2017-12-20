// veranle.h : Declaration of the Cveranle

#ifndef __VERANLE_H_
#define __VERANLE_H_

#include "resource.h"       // main symbols
#include "dinarray.h"
#include "lectura.h"
#include "escritura.h"
#include "pila.h"
#include "translate.h"

#include <afxtempl.h>


/*struct infEvent{
	CString nameEvent;
	long IdRes;
};
*/
/////////////////////////////////////////////////////////////////////////////
// Cveranle
//##ModelId=3973074B004C
class ATL_NO_VTABLE Cveranle : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Cveranle, &CLSID_veranle>,
	public IDispatchImpl<Iveranle, &IID_Iveranle, &LIBID_ANLELib>
{
public:
	//##ModelId=3973074D0257
	Cveranle();

	//##ModelId=3973074D024D
	~Cveranle();

DECLARE_REGISTRY_RESOURCEID(IDR_VERANLE)

BEGIN_COM_MAP(Cveranle)
	COM_INTERFACE_ENTRY(Iveranle)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Iveranle
public:
	//##ModelId=3973074D0225
	STDMETHOD(get_TemplateOnly)(BSTR bstrClassName, /*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=3973074D0211
	STDMETHOD(GetTypesGroups)(/*[out, retval]*/ ICPropBag ** pRet);
	//##ModelId=3973074D01F3
	STDMETHOD(GetObjectGroups)(/*[out, retval]*/ ICPropBag ** pRet);
	//##ModelId=3973074D01DF
	STDMETHOD(GetPropertyGroups)(/*[out, retval]*/ ICPropBag ** pRet);
	//##ModelId=3973074D01C1
	STDMETHOD(GetStringFromResource)(/*[in]*/ long nResId, /*[out, retval]*/ BSTR * bsRet);
	//##ModelId=3973074D01AD
	STDMETHOD(get_MetalengResourceID)(/*[out, retval]*/ long *pVal);
	//##ModelId=3973074D0199
	STDMETHOD(put_MetalengResourceID)(/*[in]*/ long newVal);
	//##ModelId=3973074D0185
	STDMETHOD(InitFromResource)(long idResource);
	//##ModelId=3973074D0171
	STDMETHOD(InitLangFromArray)(/*[in]*/ VARIANT vArray);
	//##ModelId=3973074D015D
	STDMETHOD(get_ObjectPB)(/*[out, retval]*/ ICPropBag * * pVal);
	//##ModelId=3973074D0117
	STDMETHOD(IsEvent)(/*[in]*/ BSTR ClassName, /*[out, retval]*/ VARIANT_BOOL * pResult);
	//##ModelId=3973074D00EF
	STDMETHOD(GetContainedObjectsList)(/*[in]*/ BSTR bstrClassName, /*[out,retval]*/ ICPropBag ** ppRetVal);
	//##ModelId=3973074D00B3
	STDMETHOD(IsContained)(/*[in]*/ BSTR bstrDestClassName, /*[in]*/ BSTR bstrContainedClassname, /*[out,retval]*/ VARIANT_BOOL * pReturn);
	//##ModelId=3973074D0094
	STDMETHOD(get_InToolbar)(/*[in]*/ BSTR bstrClassName, /*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=3973074D006C
	STDMETHOD(get_HideFromInsert)(/*[in]*/ BSTR bstrClassName, /*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=3973074D004E
	STDMETHOD(get_IconString)(/*[in]*/ BSTR bstrClassName, /*[out, retval]*/ BSTR *pVal);
	//##ModelId=3973074D0026
	STDMETHOD(get_IsControl)(/*[in]*/ BSTR bstrClassName, /*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=3973074D0008
	STDMETHOD(InitLang)(long nLang, _Collection * colLang);
	//##ModelId=3973074C03D2
	STDMETHOD(get_ResultPB)(ICPropBag * * PB, /*[out, retval]*/ short *pVal);
	//##ModelId=3973074C03AA
	STDMETHOD(get_CompileMode)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=3973074C0396
	STDMETHOD(put_CompileMode)(/*[in]*/ BSTR newVal);
	//##ModelId=3973074C0382
	STDMETHOD(put_Errorlevel)(/*[in]*/ short newVal);
	//##ModelId=3973074C0346
	STDMETHOD(get_AnalyzeProperty)(BSTR Object, BSTR Property, BSTR Value, /*[out, retval]*/ short *pVal);
	//##ModelId=3973074C0332
	STDMETHOD(get_ErrorCount)(/*[out, retval]*/ long *pVal);
	//##ModelId=3973074C031E
	STDMETHOD(put_ErrorCount)(/*[in]*/ long newVal);
	//##ModelId=3973074C0314
	STDMETHOD(Clear)();
	//##ModelId=3973074C0300
	STDMETHOD(Analyze)(BSTR fichero);
	//##ModelId=3973074C02E2
	STDMETHOD(get_GenerateFile)(/*[out, retval]*/ BOOL *pVal);
	//##ModelId=3973074C02CE
	STDMETHOD(put_GenerateFile)(/*[in]*/ BOOL newVal);
	//##ModelId=3973074C02BA
	STDMETHOD(get_Filename)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=3973074C02A6
	STDMETHOD(put_Filename)(/*[in]*/ BSTR newVal);
	//##ModelId=3973074C0288
	STDMETHOD(get_PropBagDefFromClassname)(/*[in]*/ BSTR classname, /*[out, retval]*/ ICPropBagDef * * pPBDef);
	//##ModelId=3973074C0260
	STDMETHOD(get_ListEventsFromClassname)(/*[in]*/ BSTR classname, /*[out, retval]*/ ICPropBag * * PB);
	//##ModelId=3973074C0242
	STDMETHOD(Geterror)(long error, VARIANT * estruct);
	//##ModelId=3973074C022E
	STDMETHOD(get_Maxerror)(/*[out, retval]*/ long *pVal);
	//##ModelId=3973074C0210
	STDMETHOD(put_Maxerror)(/*[in]*/ long newVal);
	//##ModelId=3973074C01FC
	STDMETHOD(get_Msg)(/*[out, retval]*/ ICPropBag * * pVal);
	//##ModelId=3973074C01BF
	STDMETHOD(MaxMinNumberOfObjects)(/*[in]*/ BSTR bstrClassName, /*[in]*/ BSTR ClassNameChild, /*[out, retval]*/ long *pValMax, /*[out, retval]*/ long *pValMin);
	//##ModelId=3973074C0197
	STDMETHOD(get_UniqueUID)(/*[in]*/ BSTR bstrClassName, /*[out, retval]*/ VARIANT_BOOL *pVal);

	STDMETHOD(get_ErrorLanguage)(/*[out, retval]*/ long *pVal);

	STDMETHOD(put_ErrorLanguage)(/*[in]*/ long newVal);

private:
	//##ModelId=3973074C0179
	void TransformPBDef(ICPropBag * pbDef, ICPropBagDef * pbRes);
	//##ModelId=3973074C0152
	void AddPropertiesOfGroup (BSTR nameGroup, ICPropBagDef *pbRes);
	//##ModelId=3973074C0151
	bool IsIniMetaLenguage();
private:
	// Datos miembro
	//##ModelId=3973074B0381
	CString			m_FicheroLog;		//Nombre del fichero .log
	//##ModelId=3973074B036D
	BOOL			m_GenerarLog;		//Indica si se genera o no el fichero .log
	//##ModelId=3973074B0359
	long			m_max_errores;		//Indica numero máximo de errores a generar
	//##ModelId=3973074B033B
	long			m_nerror;			//Indica el número de errores encontrados.
	//##ModelId=3973074B0327
	short			m_Errorlevel;		// Nivel de error para analizar.
	//##ModelId=3973074B0313
	long			m_nLangNumber;
	//##ModelId=3973074B0301
	CPilaErrores	m_pilaerror;		//Pila de los errores generados.	
	//##ModelId=3973074B0182
	ICPropBag *		m_pPB;
	//##ModelId=3973074B016E
	ICPropBag *		m_pPBTypes;
	//##ModelId=3973074B0152
	ICPropBag *		m_pPBPropertiesgroup;
	//##ModelId=3973074B013E
	ICPropBag *		m_pPBObjectsgroup;
	//##ModelId=3973074B0128
	ICPropBag *		m_pPBObjects;
	//##ModelId=3973074B0116
	ICPropBag *		m_pPBMessages;
	//##ModelId=3973074B0102
	ICPropBag *		m_pPBCache;
	//##ModelId=3973074B00EE
	VARIANT			m_vInitialize;
	//##ModelId=3973074B00D1
	long			m_nMetalengResourceID;
	//##ModelId=3973074B00D0
	unsigned short*	m_pBuffer;

	Translate *		m_pTranslate;

private: 
	//##ModelId=3973074C011F
	HRESULT CloneTraductProp(CComVariant * pComVariant);
	//##ModelId=3973074C010B
	HRESULT CloneDataProps(CComVariant * pValue);
	//##ModelId=3973074C00F7
	HRESULT CloneM4FontHolder(CComVariant * pComVariant);
	//##ModelId=3973074C00D9
	void CopyDefProp2PBD(ICPropBagDef * pbDest, ICDefProp * pDefProp);
	//##ModelId=3973074C00BB
	void CopyProperties(ICPropBagDef * pDest, ICPropBagDef * pSource);
	//##ModelId=3973074C0093
	HRESULT GetFromCache(BSTR ClassName, ICPropBagDef * * pPB);
	//##ModelId=3973074C0089
	long GetResourceByClassName(BSTR ClassName);
	//##ModelId=3973074D013F
	BOOL IsEvent(BSTR ClassName);
	//##ModelId=3973074C0061
	void ProcessAggregate(ICPropBagDef * pPBDef, BSTR bstrAggName);
	//##ModelId=3973074C002F
	void ProcessProperty(ICPropBagDef * pPBDest, ICPropCol * pPropCol, BSTR bstrName);
	//##ModelId=3973074C0011
	void AddPropertyToPropBag(ICPropBagDef * pPBDest, ICPropBag * pPBProp);
	//##ModelId=3973074B03DB
	BOOL TestObjectInPB(ICPropBag *pPB, BSTR bstrClassName);
	//##ModelId=3973074B03BD
	VARIANT_BOOL TestThisContainedObject(ICPropBag *pPB, BSTR bstrClassName);
	//##ModelId=3973074B0395
	BOOL IsInGroup(BSTR bstrGroupName, BSTR bstrClassName);
	//tema de eventos y de los jodidos Wizards
	//infEvent   event_map[21];
};

#endif //__VERANLE_H_
