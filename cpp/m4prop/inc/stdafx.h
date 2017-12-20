// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__ECBA54F7_1A61_11D2_9697_00C04FC2444B__INCLUDED_)
#define AFX_STDAFX_H__ECBA54F7_1A61_11D2_9697_00C04FC2444B__INCLUDED_

#define ASNBOOLEAN			0x1				// cod BOOLEAN
#define ASNINTEGER			0x2				// cod INTEGER
#define ASNGeneralString	0x1B			// cod STRING
#define ASNUnicodeString	0x1F			// cod STRING in Unicode utf8
#define ASNSequence			0x30			// cod SEQUENCE

#define PERSIST_ASN_CHUNK		128000
#define PERSIST_ASN_LANG_CHUNK	32000
#define BLOCK_DEFPROP_SIZE		80000
#define HASH_SIZE				17

#define RELEASE(p) if (p) {p->Release(); p= NULL;}
#define RELEASE_BSTR(p) if (p) {::SysFreeString(p); p = NULL;}
#define COPYBSTR(bSource,bDest) {::SysFreeString(bDest);if (bSource){bDest = SysAllocString(bSource);} else { bDest = SysAllocString(L"");};}

#import "..\..\public\msxml\msxml.dll" named_guids raw_interfaces_only

struct M4PROP__POSITION { };
typedef M4PROP__POSITION* M4PROP_POSITION;


class CCPropBagDef;
#define tagCPropBagDef &CCPropBagDef


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (_MSC_VER < 1900)
#define _WIN32_WINNT 0x0400
#endif

#define _ATL_APARTMENT_THREADED


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include "mybstr.h"
#include "myvariant.h"

// Necesario para tener las referencias adecuadas
#include "definitions.h"
#include "m4unicode.hpp"

typedef 
enum ProccessReturnValue
    {	
		RET_ERROR = -1,
		RET_NEWCLASS = 0,
		RET_ENDCLASS = 1,
		RET_PROPERTY = 2,
		RET_EOF = 3
    }	ProccessReturnValue;


typedef 
enum DataType
    {	
		PROP_IDCHANNEL = 0,
		PROP_IDNODE = 1,
		PROP_IDITEM = 2,
		PROP_IDCHANNELALIAS = 3,
		PROP_IDCHANNELKEY = 4,
		PROP_IDACCESSALIAS = 5,
		PROP_IDPRESENTATION = 6,
		PROP_GRANTS = 7,
		PROP_STARTPOS = 8
    }	DataType;

struct DefProp
{
	long			nIdProperty;
	char *			Key;
	union ValueTag
	{
		BOOL		boolValue;
		long		lValue;
		IDispatch * pdispValue;
		char *		pstrValue;
	} Value;
	union DefValueTag
	{
		BOOL		boolValue;
		long		lValue;
		IDispatch * pdispValue;
		char *		pstrValue;
	} DefValue;
	M4PropertyType	Type;
	DefProp *		pNext;
	DefProp *		pNextSeq;
	DefProp *		pNextHash;
};

struct DataPropMap
{
	long nId;
	char * strValue;
	DataPropMap * pNextHash;
	DataPropMap * pNextSeq;
};

struct _FE_PROP_ENTRY
{
	char * pstrProperty;
	long	nIndex;
	_FE_PROP_ENTRY * pNextHash;
	_FE_PROP_ENTRY * pNextSeq;
};

// Hacemos hueco para poder mantener más información
#define FE_BEGIN_PROP_MAP(x) static _FE_PROP_ENTRY x[] = {
#define FE_END_PROP_MAP()   {NULL, -1, NULL, NULL}};
#define FE_PROP_ENTRY(char, long) {&char, long},

// Por Bits
// 000000000000000010000000XXXXXXXX    Propiedad de Datos (Idchannel, Iditem, ....)
// 000000000000000000001000XXXXXXXX    Propiedad de Font (Fontname, Fontbold, ....)

#include "propmap.h"

void FreeBlock(DefProp * pBlock);
DefProp * GetFreeBlock();
long GetIdFromPropName(char * pstrName);
const char * GetNameFromPropId(long nId);
UINT HashKey(char * Key);
HRESULT GetValueFromDefProp(DefProp * pDefProp, VARIANT * pValue);
HRESULT GetDefValueFromDefProp(DefProp * pDefProp, VARIANT * pValue);
HRESULT PutDefValueInDefProp(DefProp * pDefProp, VARIANT * pValue);
HRESULT PutValueInDefProp(DefProp * pDefProp, VARIANT * pValue);
BOOL DefPropEqual(DefProp * pDefProp);
void GetUniqueKey(char ** pNewID);
void GetUniqueRef(char ** pNewID);
char * GetcharFromId(long nId);
BSTR GetBSTRFromId(long nId);
long GetIdFromBSTR(BSTR bstrName);
long GetIdFromChar(char * cName);
long AddProp2Map(char * cName);
void CopyDefProperty(DefProp * pDest, DefProp * pSource);




// Marchando una de definiciones

// Obtenemos el Puntero de datos (con un AddRef)
IDataProperties * GetDataProperties(ICPropBagDef * pPropBagDef);

// Obtenemos Coleción ChildColDef
ICChildColDef * GetChildColDef(ICPropBagDef * pPropBagDef);

// Informa a los objetos contenidos que ha cambiado su execorder
void InformExec(IContainedObjects * m_pContObj, ICChildColDef * pChildColDef);

// Añade las propiedades de una PropertyBag a una PropCol (destino, el visor de properties)
void AddPBDef2PC(IPropCol * pPropCol, ICPropBagDef * pPropBag);

// Añade una PropertyDef a la PropCol
void AddDefProp2PC(ICDefProp * pDefProp, IPropCol * pPropCol);

// Copia las propiedades desde una PBDef Source a una PBDef Destino
void CopyPBD2PBDProp(ICPropBagDef * pbDest,ICPropBagDef * pbSource);

// Copia Una Propiedad a una PropBagDef
void CopyProp2PBD(ICPropBagDef * pbDest, ICDefProp * pDefProp);

// Copia entre propiedades
void CopyProperty(ICDefProp * pDestProp, ICDefProp * pDefProp);

// Paso de propiedades desde una PropCol a una PropBagDef
HRESULT GetPBDefFromPC(ICPropBagDef * pPBDest, IPropCol * pPropCol);

// Aplicamos herencia
HRESULT ApplyInheritance(IDesignObject * pDesignObject, IDataProperties * pNewData);

// Copia de propiedades de datos
void CopyDataProps(IDataProperties * pDest, IDataProperties * pSource);

// Código Central de la aplicación de herencia
void ProcessInheritance(IDesignObject * pChildObject, IDataProperties * pParentData, IDataProperties * pNewData3);



class M4CComBSTR : public CComBSTR
{
public:

	M4CComBSTR(LPCSTR pSrc)
	{
		m_str = M4CppToVB(pSrc);
	}

	~M4CComBSTR()
	{
		::SysFreeString(m_str);
	}
};


class M4CComVariant : public CComVariant
{
public:

	M4CComVariant(LPCSTR lpszSrc)
	{
		vt = VT_BSTR;
		bstrVal = M4CppToVB(lpszSrc);
		if (bstrVal == NULL && lpszSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}
	}

	~M4CComVariant()
	{
		Clear();
	}
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ECBA54F7_1A61_11D2_9697_00C04FC2444B__INCLUDED)
