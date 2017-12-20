// PBDDecode.h: interface for the CPBDDecode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PBDDECODE_H__EF4652F1_4FB6_11D3_BD0C_00C04F71652D__INCLUDED_)
#define AFX_PBDDECODE_H__EF4652F1_4FB6_11D3_BD0C_00C04F71652D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPBDDecode  
{
public:
	ICPropBagDef * InitFromArray(SAFEARRAY * pArray);
	ICPropCol * GetPropStat();
	ICPropBagDef * DecodeMainString(IPersistObject * pPO);
	void SetAnle(Iveranle * pAnle);
	void SetConvert(CConvertPropBag * pConvert);
	HRESULT InitFromPersistObject(IPersistObject * pPO);
	CPBDDecode();
	virtual ~CPBDDecode();
private:
	bool InsertPropInDataProps(ICPropBagDef * pParent,long nId,long nData);
	bool InsertFontPropInFont(ICPropBagDef * pParent,long nId, long nData);
	bool InsertFontPropInFont(ICPropBagDef * pParent,long nId, VARIANT_BOOL bData);
	bool InsertFontPropInFont(ICPropBagDef * pParent,long nId,char * pcValue);
	bool InsertPropInDataProps(ICPropBagDef * pParent, long nId, char * pcValue);
	void InsertBooleanProperty2PropBagDef(ICPropBagDef * pPB,char * pcName, VARIANT_BOOL bData);
	long ReadBoolean(BYTE ** bBuffer, VARIANT_BOOL * bValue);
	long ReadNumber(BYTE ** bBuffer, long * nData);
	void InsertNumberProperty2PropBagDef(ICPropBagDef * pPB, char * pcName, long nData);
	void InsertValueOfTraductProp(char * pcValue, ITraductProp * pTraduct);
	void InsertStringProperty2PropBagDef(ICPropBagDef * pParent, char * tmpName, char * tmpValue);
	long ReadProperty(BYTE * * bBuffer, ICPropBagDef * pParent, char type);
	long ReadASNRec(BYTE ** bBuffer, long nBytesToRead, ICPropBagDef * pParent);
	long ReadString(BYTE ** bBuffer, long nTempLength, char** tmpBuffer, char type);
	long ReadLength(BYTE ** bBuffer, long * nLength);
	long GetPBDefFromASN(BYTE * * bBuffer, ICPropBagDef * * ppPBResult);
	long ReadType(BYTE ** bBuffer, BYTE * bType);
	void FreeData();
	void FillNullTraductProp(char * pcValue, ITraductProp * pTraduct);
	ICPropBag ** m_ppTransPB;
	long m_nLangNumber;
	CConvertPropBag * m_pConvert;   // Usado sin añadir referencias
									// la vida de éste objeto (PBDDecode) siempre
									// está por supeditado al ConvertPropBag
	Iveranle * m_pAnle;

#ifdef PROP_STAT
	ICPropCol	* m_pPropCol;
#endif

	char  m_iStringBaseType;
};

#endif // !defined(AFX_PBDDECODE_H__EF4652F1_4FB6_11D3_BD0C_00C04F71652D__INCLUDED_)
