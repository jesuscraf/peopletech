// OBLParser.h : Declaration of the COBLParser

#ifndef __OBLPARSER_H_
#define __OBLPARSER_H_

#include "resource.h"       // main symbols
#define BUFF_TEMP_SIZE 64
#define BUFF_CLASSNAME_INIT_SIZE 64
#define BUFF_ALIAS_INIT_SIZE 64
#define BUFF_PROPNAME_INIT_SIZE 64
#define BUFF_PROPVALUE_INIT_SIZE 64

/////////////////////////////////////////////////////////////////////////////
// COBLParser
class ATL_NO_VTABLE COBLParser : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COBLParser, &CLSID_OBLParser>,
	public ISupportErrorInfo,
	public IDispatchImpl<IOBLParser, &IID_IOBLParser, &LIBID_M4PROPLib>
{
public:
	COBLParser()
	{
		m_pcClassName = (char*)malloc(BUFF_CLASSNAME_INIT_SIZE+1);
		m_pcAlias = (char*)malloc(BUFF_ALIAS_INIT_SIZE+1);
		m_pcPropName = (char*)malloc(BUFF_PROPNAME_INIT_SIZE+1);
		m_pcPropValue = (char*)malloc(BUFF_PROPVALUE_INIT_SIZE+1);
		*m_pcClassName = 0;
		*m_pcAlias = 0;
		*m_pcPropName = 0;
		*m_pcPropValue = 0;

		m_nClassSize = BUFF_CLASSNAME_INIT_SIZE;
		m_nAliasSize = BUFF_ALIAS_INIT_SIZE;
		m_nPropNameSize = BUFF_PROPNAME_INIT_SIZE;
		m_nPropValueSize = BUFF_PROPVALUE_INIT_SIZE;
		m_nTempBufferSize = BUFF_TEMP_SIZE;

		m_pText = NULL;
		m_pTextActual = NULL;
		m_pTextFinal = NULL;
		m_pBufferTemp = (char*)malloc(BUFF_TEMP_SIZE + 1);
		*(m_pBufferTemp + BUFF_TEMP_SIZE) = 0;
		memset(m_pBufferTemp, 0, BUFF_TEMP_SIZE);


		m_pAnle = NULL;
	}
	~COBLParser()
	{
		Free();
		free(m_pcClassName);
		free(m_pcAlias);
		free(m_pcPropName);
		free(m_pcPropValue);
		free(m_pBufferTemp);
		RELEASE(m_pAnle);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OBLPARSER)

BEGIN_COM_MAP(COBLParser)
	COM_INTERFACE_ENTRY(IOBLParser)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IOBLParser
public:
	STDMETHOD(PropBagDefFromOBL)(/*[in]*/ Iveranle * pAnle, BSTR bstrText,/*[out, retval]*/ ICPropBagDef * * pRetPropBag);
	char * m_pcClassName;
	char * m_pcAlias;
	char * m_pcPropName;
	char * m_pcPropValue;

	char * m_pText;
	char * m_pTextActual;
	char * m_pTextFinal;
	char * m_pBufferTemp;

	long m_nTempBufferSize;
	long m_nClassSize;
	long m_nAliasSize;
	long m_nPropNameSize;
	long m_nPropValueSize;
	Iveranle * m_pAnle;

private:
	void ExpandTempBuffer();
	void ReadTraductProp(ICPropBagDef * pPBD);
	void InsertGeneralProperty(ICPropBagDef * pPBD);
	void InsertFontProperty(ICPropBagDef * pPBD);
	void InsertDataProperty(DataType eType,ICPropBagDef * pPBD);
	void InsertProperty(ICPropBagDef * pPBD);
	void RecProcess(ICPropBagDef * pPBD);
	ICPropBagDef * CreateNewPropBagDef(char * pcClassName, char * pcAlias);
	void ExpandPropValueBuffer();
	void ExpandPropNameBuffer();
	void ExpandAliasBuffer();
	void ExpandClassNameBuffer();
	void CutLine(char ** ppcBuffer);
	void ProcessProperty(char * pcBuffer);
	void ProcessNewClass(char * pcBuffer);
	ProccessReturnValue ProcessLine(char * pcBuffer);
	char * NextLine();
	void Free();
};

#endif //__OBLPARSER_H_
