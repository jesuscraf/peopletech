// ConvertPropBag.h : Declaration of the CConvertPropBag

#ifndef __CONVERTPROPBAG_H_
#define __CONVERTPROPBAG_H_


#include "resource.h"       // main symbols
#include "dataproperties.h"
#include "myvariant.h"
#include "logmanager.h"
#include "m4prop_dll.hpp"

#define ESP_TABULADOR		4
#define MAX_MEM			64000


class ClChannelManager;

/////////////////////////////////////////////////////////////////////////////
// CConvertPropBag
class ATL_NO_VTABLE CConvertPropBag : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConvertPropBag, &CLSID_ConvertPropBag>,
	public IDispatchImpl<IConvertPropBag, &IID_IConvertPropBag, &LIBID_M4PROPLib>
{


public:
	CConvertPropBag() 
	{
		contMem = 0;
		m_bPersistIdHelpTopic = -1;
		maxMem = MAX_MEM;
		initPoint = NULL;
		m_nNumTab = ESP_TABULADOR;
		m_bPersistLog = 0;  // False

		m_pstrIdChannel = (char*)malloc(2);
		m_pstrIdChannelAlias = (char*)malloc(2);
		m_pstrIdChannelKey = (char*)malloc(2);
		m_pstrIdAccessAlias = (char*)malloc(2);
		m_pstrIdItem = (char*)malloc(2);
		m_pstrIdNode = (char*)malloc(2);

		m_charBuffer = (char*) malloc(16384);

		strcpy(m_pstrIdChannel, "");
		strcpy(m_pstrIdChannelAlias, "");
		strcpy(m_pstrIdChannelKey, "");
		strcpy(m_pstrIdAccessAlias, "");
		strcpy(m_pstrIdItem, "");
		strcpy(m_pstrIdNode, "");

		m_nGrants = 31;
		m_nIdPresentation = -1;
		m_nStartPos = -6;
		m_nRefCount = 0;
		m_pAnle = NULL;
		m_bTranslatePB = false;
		m_pTranslate = NULL;
#ifdef PROP_STAT
		m_pPropStat = NULL;
#endif

	}

	~CConvertPropBag()
	{
		free(m_pstrIdChannel);
		free(m_pstrIdChannelAlias);
		free(m_pstrIdChannelKey);
		free(m_pstrIdAccessAlias);
		free(m_pstrIdItem);
		free(m_pstrIdNode);
		free(m_charBuffer);
		RELEASE(m_pAnle);
		RELEASE(m_pTranslate);
#ifdef PROP_STAT
		RELEASE(m_pPropStat);
#endif
	}



DECLARE_REGISTRY_RESOURCEID(IDR_CONVERTPROPBAG)

BEGIN_COM_MAP(CConvertPropBag)
	COM_INTERFACE_ENTRY(IConvertPropBag)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IConvertPropBag
public:
	STDMETHOD(PB2XML)(/*[in]*/ ICPropBag *poPropBag, /*[out, retval]*/ SAFEARRAY **abXML);
	STDMETHOD(PBDef2StringOBLHide)(/*[in]*/ ICPropBagDef * pPBDef, /*[out, retval]*/ BSTR * strOBL);
	STDMETHOD(ResetTradIdentifiers)(ICPropBagDef * pPBD);
	STDMETHOD(ConstructPresentationFromArrayVar)(/*[in]*/ VARIANT vArray,/*[in]*/ Iveranle *pAnle, /*[out, retval]*/ VARIANT * pVal);
	STDMETHOD(PBDef2XMLString)(/*[in]*/ ICPropBagDef * pPBDef, /*[out, retval]*/ BSTR * bsRet);
	STDMETHOD(PBDef2XMLFile)(/*[in]*/ ICPropBagDef * pPbDef, /*[in]*/ BSTR sFileName);
	STDMETHOD(ConstructPresentationFromArray)(/*[in]*/ VARIANT vArray, /*[in]*/ Iveranle * pAnle, /*[out, retval]*/ ICPropBagDef ** pPBDef);
	STDMETHOD(get_PersistIdHelpTopic)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_PersistIdHelpTopic)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(CopyPBDef2PBDefProp)(/*[in]*/ ICPropBagDef * pPBDDest, /*[in]*/ ICPropBagDef * pPBDSource);
	STDMETHOD(ClonePBDefRec)(/*[in]*/ICPropBagDef * pPBDSource, /*[in]*/ Iveranle * pAnle, /*[out,retval]*/ ICPropBagDef ** pPBDDest);
	STDMETHOD(get_PropStat)(/*[out, retval]*/ ICPropCol * *pVal);
	STDMETHOD(PBDef2PB)(/*[in]*/ ICPropBagDef * pPBDSource, /*[in]*/ VARIANT_BOOL bTranslate, /*[in]*/ VARIANT vIndex, ITranslate * pTranslate, /*[out, retval]*/ ICPropBag * * pPBRet);
	STDMETHOD(PBFromASNString)(/*[in]*/ BSTR bstrIn, /*[out, retval]*/ ICPropBag * * ppPBRet);
	STDMETHOD(ConstructPresentation)(/*[in]*/ IPersistObject * pPO, /*[in,out]*/ Iveranle * pAnle, /*[out, retval]*/ ICPropBagDef * * pPBDef);
	STDMETHOD(AccessViolation)();
	STDMETHOD(get_LogData)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_UsePersistLog)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UsePersistLog)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(PersistPBDProccess)(/*[in,out]*/ ICPropBagDef * pPBD, /*[in,out]*/ IPersistObject * pPersistObject);
	STDMETHOD(PBFromStr)(/*[in]*/ BSTR strIn, /*[out,retval]*/ ICPropBag ** pb);
	STDMETHOD(PrePersistProcess)(/*[in,out]*/ ICPropBag ** pb);
	STDMETHOD(Testeo)(BSTR strName);
	STDMETHOD(PB2ASN)(/*[in]*/ ICPropBag * pb, /*[out, result]*/ BSTR * bstrOut);
	STDMETHOD(GetPBLength)(ICPropBag * pPB, long ** pVal);
	STDMETHOD(Array2PB)(/*[in, out]*/ SAFEARRAY ** aArray, /*[out, retval]*/ ICPropBag ** pPB);
	STDMETHOD(PB2StringOBL)(/*[in]*/ ICPropBag * pb, /*[out, retval]*/ BSTR *strOBL);
	STDMETHOD(PB2FileOBL)(/*[in]*/ ICPropBag * pb, /*[in]*/ BSTR nomFile);
	STDMETHOD(get_NumEspTab)(/*[out, retval]*/ int *num);
	STDMETHOD(put_NumEspTab)(/*[in]*/ int num);
	STDMETHOD(PBDef2StringOBL)(/*[in]*/ ICPropBagDef * pb, /*[out, retval]*/ BSTR *strOBL);
	STDMETHOD(PBDef2FileOBL)(/*[in]*/ ICPropBagDef * pb, /*[in]*/ BSTR nomFile);
	STDMETHOD(Array2PBDef)(/*[in, out]*/ SAFEARRAY ** aArray, /*[out, retval]*/ ICPropBagDef ** pPBDef);
	
	
protected:

	HRESULT _PBDef2StringOBL(ICPropBagDef * pPBDef, BSTR *strOBL, bool ai_bHidePosition);

	ICPropBagDef * ConstructPBDefFromASN1(unsigned char * tmpBuffer);
	void WritePBInStringOBL(ICPropBag * pPB,char ** myOBL,long level);
	void WriteLevelInString(DefProp * pDefProp,char ** myOBL,long level);
	void WriteLevelInString(char *prop,VARIANT value,char ** myOBL,long level);
	void WriteLevelProperty(char *pstrPropName, char*pstrPropValue, char **ppstrOBL, long nLevel);
	void WriteTabInString(int num, char ** myOBL);
	void CheckLengthMemory(char **strOBL, long incr);
	void NewBlockOfMemory();
	void WritePBDefInStringOBL(ICPropBagDef * pPB,char ** myOBL, CDataProperties *pData,long level, bool ai_bHidePosition, const char* ai_pccParent);
	bool _IsHiddenProperty( DefProp * ai_pDefProp );

protected:
	VARIANT_BOOL	m_bPersistIdHelpTopic;
	unsigned long	contMem;
	unsigned long	maxMem;
	char *			initPoint;
	int				m_nNumTab;
	CComVariant		m_vIndex;

	char *			m_pstrIdChannel;
	char *			m_pstrIdChannelAlias;
	char *			m_pstrIdChannelKey;
	char *			m_pstrIdAccessAlias;
	char *			m_pstrIdItem;
	char *			m_pstrIdNode;
	char *			m_charBuffer;
	short			m_nGrants;
	long			m_nIdPresentation;
	long			m_nStartPos;
	long			m_nRefCount;
	Iveranle *		m_pAnle;
	bool			m_bTranslatePB;
	ITranslate *	m_pTranslate;

#ifdef PROP_STAT	
	ICPropCol *		m_pPropStat;

#endif





private:
//	void CopyPBD2PBDProp(CCPropBagDef * pCPDest, CCPropBagDef * pCPSource);
	HRESULT InnerClonePBDefRec(ICPropBagDef * pSource, ICPropBagDef ** pDest);
	void AddProperty2PB(ICDefProp * pProp, ICPropBag * pPBDest, CDataProperties * pData);
	HRESULT ProcessPBD2PB(ICPropBagDef * pPBDSource, ICPropBag * pPBDest, CDataProperties * pInhData);
	long GetPBDefFromASN(BYTE * * theBuffer,ICPropBagDef ** ppPBDef);
	ICPropBagDef * ConstructPBDefFromASN(unsigned char * theBuffer);
	VARIANT_BOOL m_bPersistLog;
	CLogManager m_Log;
	void WriteTraductProp(ITraductProp *pTraduct, char ** ppstrOBL, int nLevel);
	void WriteFont(DefProp * pDefProp, char ** ppstrOBL, int nLevel);
	void WriteDataProp(CDataProperties *pInnerData, CDataProperties *pInheritData, char** ppstrOBL, int nLevel);
};


M4_DECL_M4PROP ICPropBag* ConstructPBFromASN1(unsigned char * tmpBuffer, const char* ai_pccOrganization, ClChannelManager *ai_poChannelManager);




#endif //__CONVERTPROPBAG_H_
