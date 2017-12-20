//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cldinarr.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef __CLDINARR_HPP__
#define __CLDINARR_HPP__


#include "defmain.hpp"
#include "cltblmem.hpp"
#include "m4string.hpp"


class ClRegister;
class ClCompiledMCR;
class ClLConn;
class ClGlobVars;
class ClMetaParam;
class ClMetaItemCol;
class ClRegister_Pipe;

typedef ClRegister* PClRegister;
typedef m4pcchar_t m4id_t;

enum eEmulateNVL_t{
	M4LDB_NO_NVL = 0,
	M4LDB_NVL_FIRST,
	M4LDB_NVL_BYPASS
};


struct stIdField {
	m4id_t m_pIdField;

	stIdField (void) {
		m_pIdField = NULL;
	}

	stIdField (const struct stIdField &ai_stCopy) {
		m_pIdField = ai_stCopy.m_pIdField;
	}

	friend m4bool_t operator < (const struct stIdField &lhc, const struct stIdField &rhc) {
		return ((strcmpi (lhc.m_pIdField, rhc.m_pIdField) < 0) ? M4_TRUE : M4_FALSE);
	}

	friend m4bool_t operator ==(const struct stIdField &lhc, const struct stIdField &rhc) {
		return ((!strcmpi (lhc.m_pIdField, rhc.m_pIdField)) ? M4_TRUE : M4_FALSE);
	}
};


typedef map<struct stIdField, m4int32_t, less<struct stIdField> >	MpIdData;
typedef MpIdData*													PMpIdData;
typedef MpIdData::const_iterator									itMpIdData;



class ClDinArr;
class ClPDContainer;

class ClPDEncryptedValue
{
	friend class ClDinArr;
	friend class ClPDContainer;

	m4pchar_t m_pEncryptedData;
	// UNICODE DB
	m4int32_t m_lMaxLen;
	m4int32_t m_iRealLen;
	m4bool_t m_bIsFile;


public:
	ClPDEncryptedValue (void)
	{
		m_pEncryptedData = NULL;
		m_lMaxLen = -1;
		m_iRealLen = -1;
		m_bIsFile = M4_FALSE;
	}
	// No copia el dato. Copia la estructura.
	ClPDEncryptedValue(ClPDEncryptedValue &ai_Encrypted);

	~ClPDEncryptedValue (void)
	{
		Clear();
	}
	m4return_t	Encrypt(m4pcchar_t ai_pData,m4uint32_t ai_iLen);
	m4return_t	Decrypt(m4pchar_t & ai_pData,m4int32_t &ao_Len);
	m4return_t 	ReserveBuffer(m4int32_t ai_size);
	
	void		Clear();
	void		Reset();
	void		SetBlob(m4pchar_t ai_pBuffer,m4int32_t ai_lSize,m4int32_t ai_lFictSize);
	
	m4int32_t	GetMaxLen(void)
	{
		return (m_lMaxLen);
	}
	
	m4pint32_t	GetMaxLenPointer(void)
	{
		return &m_lMaxLen;
	}

	m4pint32_t	GetpLength(void)
	{
		return (m4pint32_t)(&m_iRealLen);
	}
	
	m4pchar_t	GetDataPointer(void)
	{
		return m_pEncryptedData;
	}
	
	m4pchar_t*	GetDataPointerPointer(void)
	{
		return &m_pEncryptedData;
	}

};


class ClPDValue
{
	friend class ClDinArr;
	friend class ClPDContainer;

	m4int32_t m_lLength;
	m4pchar_t m_pData;

	// UNICODE DB
	m4pchar_t* m_ppDataPointer;
	m4int32_t m_lMaxLen;

	ClPDEncryptedValue *m_pPDEncryptedValue;
	ClTblMem *m_pTblMem;

public:

	ClPDValue (void)
	{
		m_lLength = M4LDB_SQL_NULL_DATA;
		m_pData = NULL;
		m_ppDataPointer = NULL;
		m_lMaxLen = -1;
		m_pPDEncryptedValue = NULL;
		m_pTblMem = NULL;
	}
	~ClPDValue (void)
	{
		if (m_pPDEncryptedValue)
		{
			delete m_pPDEncryptedValue;
		}
		m_pPDEncryptedValue = NULL;
	}

	m4pint32_t	GetMaxLenPointer(m4int32_t ai_lPrec);

	m4return_t	EncryptValue(m4int8_t ai_m4Type, m4bool_t ai_bIsUnicode);
	m4return_t	DecryptValue(m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale);
	m4return_t	ReserveForEncrypted(m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4bool_t ai_bIsUnicode);

	void Release (void)
	{
		if( m_pTblMem != NULL )
		{
			delete m_pTblMem;
		}
		else
		{
			/* Bug 0125302
			Hay que comprobar que no sea nulo
			*/
			if( m_ppDataPointer != NULL && *m_ppDataPointer != NULL )
			{
				delete *m_ppDataPointer;
				*m_ppDataPointer = NULL;
			}
		}

		m_pTblMem = NULL;
		m_pData = NULL;
	}
};


class ClPDMTData
{
	friend class ClDinArr;
	friend class ClPDContainer;

	m4int32_t m_hItem;
	m4int32_t m_lPrec;
	m4int32_t m_lScale;
	m4int8_t m_m4Type;
	m4bool_t m_bIsUnicode;
	m4bool_t m_bIsExtern;
	eParamType_t m_eParamType;
	eEmulateNVL_t m_eEmulateNVL;
	m4int32_t m_lPrecFld;
	m4int32_t m_iOrgLevel;
	m4bool_t m_bInheritsData;

public:
	ClPDMTData (void) {
		m_hItem = 0;
		m_lPrec = 0;
		m_lScale = 0;
		m_m4Type = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING );
		m_bIsUnicode = M4_FALSE;
		m_bIsExtern = M4_FALSE;
		m_eParamType = M4LDB_PAR_INTERN;
		m_eEmulateNVL = M4LDB_NO_NVL;
		m_lPrecFld = -1;
		m_iOrgLevel = 0;
		m_bInheritsData = M4_FALSE;
	}
};


class ClPDMTDataExtended
{
	friend class ClDinArr;
	friend class ClPDContainer;

	m4bool_t m_bIsPK;
	m4bool_t m_bIsNotNull;
	m4bool_t m_bIsCtrlRep;
	m4bool_t m_bIsReadOnly;
	m4bool_t m_bHasSecurity;
	//Bug 0087478
	m4bool_t m_bHasSecurityReadInPersist;
	eFuncType_t m_idDefFunc;
	eFuncType_t m_idConstFunc;
	eFldType_t m_idIntFld;
	m4pcchar_t m_pDefArgs;
	m4pcchar_t m_pConstArgs;
	m4pcchar_t m_pRealTable;
	m4pcchar_t m_pRealField;
	m4id_t m_idField;
	m4id_t m_idTranslation;
	eConvFunc_t m_idConvFunc;
	m4int32_t m_iCurrentLang;	// Guarda el indice del campo traducido que corresponde al idioma actual.
								// Solo se modifica para los campos del lenguaje por defecto.
	
public:
	ClPDMTDataExtended (void)
	{
		m_bIsPK = M4_FALSE;
		m_bIsNotNull = M4_FALSE;
		m_bIsCtrlRep = M4_FALSE;
		m_bIsReadOnly = M4_FALSE;
		m_bHasSecurity = M4_FALSE;
		m_bHasSecurityReadInPersist = M4_FALSE;
		m_idDefFunc = M4LDB_FTN_NONE;
		m_idConstFunc = M4LDB_FTN_NONE;
		m_idIntFld = M4LDB_NORMAL;
		m_pDefArgs = NULL;
		m_pConstArgs = NULL;
		m_pRealTable = NULL;
		m_pRealField = NULL;
		m_idField = NULL;
		m_idTranslation = NULL;
		m_idConvFunc = M4LDB_CONV_FUNC_NONE;
		m_iCurrentLang = M4LDB_NULL_INDEX;
	}
};


class ClPDContainer {

	friend class ClDinArr;

	m4bool_t m_bIsOwn;

public:
	ClPDValue *m_pPDValue;
	ClPDValue *m_pPDOldValue;
	ClPDMTData *m_pPDMTData;
	ClPDMTDataExtended *m_pPDMTDataExtended;

	ClPDContainer (ClPDValue *ai_pPDValue, ClPDMTData *ai_pPDMTData, ClPDMTDataExtended *ai_pPDMTDataExtended = NULL, ClPDValue *ai_pPDOldValue = NULL)
	{
		m_bIsOwn = M4_TRUE;
		m_pPDValue = ai_pPDValue;
		m_pPDOldValue = ai_pPDOldValue;
		m_pPDMTData = ai_pPDMTData;
		m_pPDMTDataExtended = ai_pPDMTDataExtended;
	}
	ClPDContainer (const class ClPDContainer *ai_oPDContainer)
	{
		m_bIsOwn = M4_FALSE;
		m_pPDValue = ai_oPDContainer->m_pPDValue;
		if (ai_oPDContainer->m_pPDOldValue) {
			// Cuidado con esto. El OldValue es el NewValue.
			m_pPDValue = ai_oPDContainer->m_pPDOldValue; 
		}
		m_pPDOldValue = NULL;
		m_pPDMTData = ai_oPDContainer->m_pPDMTData;
		m_pPDMTDataExtended = ai_oPDContainer->m_pPDMTDataExtended;
	}
	~ClPDContainer (void)
	{
		if (m_bIsOwn) {
			if (m_pPDValue) {
				if ((m_pPDMTData->m_eParamType != M4LDB_PAR_MINUS_INF && 
					 m_pPDMTData->m_eParamType != M4LDB_PAR_PLUS_INF && 
					 m_pPDMTData->m_m4Type != M4CL_M4_TYPE_LONG &&
					 m_pPDMTData->m_m4Type != M4CL_M4_TYPE_UNI_LONG &&
					 m_pPDMTData->m_m4Type != M4CL_M4_TYPE_BINARY_STRING && 
					!m_pPDMTData->m_bIsExtern) || 
					m_pPDValue->m_pTblMem) {
					m_pPDValue->Release ();
				}
				delete m_pPDValue;
			}
			m_pPDValue = NULL;

			if (m_pPDOldValue) {
				m_pPDOldValue->Release ();
				delete m_pPDOldValue;
			}
			m_pPDOldValue = NULL;

			if (m_pPDMTData) {
				delete m_pPDMTData;
			}
			m_pPDMTData = NULL;

			if (m_pPDMTDataExtended) {
				delete m_pPDMTDataExtended;
			}
			m_pPDMTDataExtended = NULL;
		}
	}
};

typedef ClPDContainer* PClPDContainer;


const m4int32_t M4LDB_ID_MAX_SIZE = 32;
class ClParam {
public:
	eFldType_t m_eFldType;
	m4pchar_t m_pData;
	m4int32_t m_lPrec;
	m4int32_t m_m4Type;
	m4int32_t m_lScale;
	m4pint32_t m_plLength;
	m4int32_t m_lPrecFld;

	ClParam( void );
	~ClParam();

	m4return_t SetBindParam( eFldType_t ai_eFldType, m4pvoid_t ai_pValue, m4int32_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale );
};

typedef vector<ClParam *>		VcClParam;
typedef VcClParam::iterator		itVcClParam;


class ClDinArr
{
private:

	ClGlobVars *m_pGlobVars;
	PClPDContainer *m_poPDContainer;
	m4int32_t m_lNumData;
	m4int32_t m_lMaxNumData;
	m4int32_t m_lReferences;
	PMpIdData m_pmpIdData;
	m4bool_t m_bHasLongItem;
	ClRegister_Pipe *m_RegPipe;
	VcClParam * m_pVcClParam;


public:
	ClDinArr (ClGlobVars *ai_pGlobVars);
	~ClDinArr (void);

	ClDinArr (ClDinArr *ai_pParam);

	m4return_t Init (m4int32_t ai_lMaxNumData, m4bool_t ai_bIsOnlySel = M4_FALSE);
	m4return_t Close (void);

	void Attach (void);
	void Detach (void);
	m4int32_t GetReferences (void);
	VcClParam * GetVcClParam( void );
	m4return_t CreateData (m4int32_t ai_lIndData, ClRegister *ai_pReg, m4int32_t ai_hItem, m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale, m4pchar_t ai_pMemory, m4pchar_t* ai_ppMemoryPointer, eParamType_t ai_eParamType, m4bool_t ai_bWithPipe);
	m4return_t CreateData (m4int32_t ai_lIndData, ClPDContainer *ai_poPDContainer);

	ClPDContainer *GetRefData (m4int32_t ai_lIndData);

	ClRegister_Pipe *GetPipe(void);

    m4bool_t HasLongItem(void);

	m4int32_t GetNumData (void);

	m4return_t SetData (m4int32_t ai_lIndData, m4pcchar_t ai_pcData = NULL, m4int32_t ai_lLength = 0, m4int8_t ai_m4Type = 0);
	m4return_t SetNull (m4int32_t ai_lIndData);
	m4pcchar_t GetData (m4int32_t ai_lIndData);
	m4int32_t GetLength (m4int32_t ai_lIndData);

	m4pchar_t GetDataPointer (m4int32_t ai_lIndData);
	m4pchar_t* GetDataPointerPointer (m4int32_t ai_lIndData);
	m4pint32_t GetMaxLenPointer (m4int32_t ai_lIndData);
	m4pint32_t GetMaxLenDataPointer (m4int32_t ai_lIndData);
	m4pint32_t GetLengthPointer (m4int32_t ai_lIndData);

	m4return_t CreateOldData (m4int32_t ai_lIndData);

	m4return_t SetOldData (m4int32_t ai_lIndData, m4pcchar_t ai_pcData = NULL, m4int32_t ai_lLength = 0, m4int8_t ai_m4Type = 0);

	m4pcchar_t GetOldData (m4int32_t ai_lIndData);
	m4pchar_t GetOldDataPointer (m4int32_t ai_lIndData);
	m4pchar_t* GetOldDataPointerPointer (m4int32_t ai_lIndData);
	m4int32_t GetOldLength (m4int32_t ai_lIndData);
	m4pint32_t GetOldLengthPointer (m4int32_t ai_lIndData);

	m4int32_t GetItemHandle (m4int32_t ai_lIndData);
	void SetItemHandle (m4int32_t ai_lIndData, m4int32_t ai_hItem);
	//Bug 0092565.
	void SetParamType (m4int32_t ai_lIndData, eParamType_t ai_eParamType);

	m4int8_t GetInfoM4Type (m4int32_t ai_lIndData);
	void SetInfoM4Type (m4int32_t ai_lIndData, m4int8_t ai_m4Type);

	// Investigar en que casos nos interesa utilizar en encriptación GetInfoM4Type y GetInfoRealM4Type.
	m4int8_t GetInfoRealM4Type (m4int32_t ai_lIndData);

	void		SetCurrentLanguageIndex (m4int32_t ai_lIndData, m4int32_t ai_lIndDefLang);
	m4int32_t	GetCurrentLanguageIndex	(m4int32_t ai_lIndData );

	void			SetEmulateNVL (m4int32_t ai_lIndData,eEmulateNVL_t ai_eEmulateNVL);
	eEmulateNVL_t	GetEmulateNVL (m4int32_t ai_lIndData);

	m4int32_t GetInfoPrec (m4int32_t ai_lIndData);
	m4int32_t GetInfoScale (m4int32_t ai_lIndData);
	m4bool_t GetInfoIsPK (m4int32_t ai_lIndData);
	void SetInfoIsPK (m4int32_t ai_lIndData, m4bool_t ai_bIsPK);
	m4bool_t GetInfoIsNotNull (m4int32_t ai_lIndData);
	m4bool_t GetInfoIsCtrlRep (m4int32_t ai_lIndData);
	eFuncType_t GetInfoIdDefaultFunc (m4int32_t ai_lIndData);
	eFuncType_t GetInfoIdConstraintFunc (m4int32_t ai_lIndData);
	eFldType_t GetInfoIdInternalField (m4int32_t ai_lIndData);
	eParamType_t GetInfoParamType (m4int32_t ai_lIndData);
	m4pcchar_t GetInfoDefaultArgs (m4int32_t ai_lIndData);
	m4pcchar_t GetInfoConstraintArgs (m4int32_t ai_lIndData);
	m4id_t GetInfoIdField (m4int32_t ai_lIndData);
	m4pcchar_t GetInfoRealTable (m4int32_t ai_lIndData);
	m4pcchar_t GetInfoRealField (m4int32_t ai_lIndData);
	m4int32_t GetInfoPrecFld (m4int32_t ai_lIndData);
	m4int32_t GetInfoOrgLevel (m4int32_t ai_lIndData);
	void SetInfoOrgLevel (m4int32_t ai_lIndData, m4int32_t ai_iOrgLevel);
	m4bool_t GetInfoInheritsData (m4int32_t ai_lIndData);
	void SetInfoInheritsData (m4int32_t ai_lIndData, m4bool_t ai_bInheritsData);

	m4int32_t FindIndexByIdField (m4id_t ai_idField);
	m4int32_t FindIndexByIdInternalField (eFldType_t ai_idIntFld);
	m4int32_t FindIndexByItemHandle (m4int32_t ai_hItem);

	m4bool_t IsNull (m4int32_t ai_lIndData);
	m4bool_t IsOldNull (m4int32_t ai_lIndData);
	m4bool_t IsEmptyString (m4int32_t ai_lIndData);
	eDiffType_t Difference (m4int32_t ai_lIndData);
	m4return_t Clear (void);
	m4return_t ClearLongs (void);
	m4return_t ClearEncryptedBlobs (void);

	m4return_t SetInfoIsReadOnly (m4int32_t ai_lIndData, m4bool_t ai_bIsReadOnly);
	m4bool_t GetInfoIsReadOnly (m4int32_t ai_lIndData);

	m4bool_t GetInfoIsOwn (m4int32_t ai_lIndData);

	m4return_t Backup (ClTblMem *ai_pTbl);
	m4return_t Restore (ClTblMem *ai_pTbl);
	m4return_t Merge (ClTblMem *ai_pTbl, m4int32_t ai_indRow, m4pcchar_t ai_pccTramitation);

	m4return_t SetLengthReadOnly (void);
	
	eConvFunc_t GetInfoIdConvertFunc	(m4int32_t ai_lIndData);
	m4return_t	ConvertData				(m4int32_t ai_lIndData);

	m4bool_t	GetIsEncrypted			(m4int32_t ai_lIndData);
	m4bool_t	GetHasOldValue			(m4int32_t ai_lIndData);

	void SetInfoIdDefaultFunc (m4int32_t ai_lIndData, eFuncType_t ai_eFuncType);

	m4bool_t GetInfoHasSecurity (m4int32_t ai_lIndData);
	void SetInfoHasSecurity (m4int32_t ai_lIndData);

	m4bool_t GetInfoHasSecurityReadInPersist (m4int32_t ai_lIndData);
	void SetInfoHasSecurityReadInPersist (m4int32_t ai_lIndData);

	m4id_t GetInfoTranslation (m4int32_t ai_lIndData);
	void SetInfoTranslation (m4int32_t ai_lIndData, m4id_t ai_idTranslation);

	m4return_t DecryptAll(void);

	m4return_t CreateData	(m4int32_t ai_lIndData, ClMetaParam	*ai_pPar);

	void SetInternalLong (m4int32_t ai_lIndData, ClTblMem *ai_pTblMem);

	m4return_t InsertIdData (m4id_t ai_idField, m4int32_t ai_lIndData);

private:
	m4return_t _ConvertToUpperLower		(m4int32_t ai_lIndData,m4bool_t ai_ToUpper = M4_TRUE);
	m4return_t _ConvertFirstCapLetter	(m4pchar_t ai_pData);
	void	_CreateVcClParam (void);
};


#endif

