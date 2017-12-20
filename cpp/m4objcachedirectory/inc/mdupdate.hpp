//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:	mdupdate.hpp   
// Author:	
// Date:	
// Definition:
//    
// Reference:
//    
//

#ifndef __MDUPDATE_HPP__
#define __MDUPDATE_HPP__

#include "m4objcachedirectory_dll.hpp"
#include "m4define.hpp"
#include "clgeniod.hpp"
#include "m4stl.hpp"
#include "m4var.hpp"
#include "syncro.hpp"

class ClChannelManager;
class ClChannel;
class ClAccess;


//MD_TYPES
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_CMCR				1
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_CSCR				2
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_PRES				3
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_MAP				4
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_DATA				5
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_TABLE				6
#define M4_ITEM_MD_VERSION_ID_MD_TYPE_LITERALS			7

class M4_DECL_M4OBJCACHEDIRECTORY ClMDVersion
{
public:
	ClMDVersion(void);
	ClMDVersion(m4VariantType & ai_vMDVersion, m4VariantType & ai_vMDType, m4VariantType & ai_vMDId);
	~ClMDVersion(void);

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);

	m4uint16_t m_iMDType;
	m4VariantType  m_vMDId;
	m4date_t   m_dMDVersion;
};

typedef list< ClMDVersion* > ClMDVersionList;

class M4_DECL_M4OBJCACHEDIRECTORY ClMDUpdateDeltasList
{
public:
	ClMDUpdateDeltasList(void);
	ClMDUpdateDeltasList(m4date_t ai_dLastModificationDate, m4VariantType &ai_vRepositoryKey);
	~ClMDUpdateDeltasList(void);

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD);

	m4date_t GetLastModificationDate()
	{
		return m_dLastModificationDate;
	}

	m4VariantType& GetRepositoryKey()
	{
		return m_vRepositoryKey;
	}

	ClMDVersionList m_oVersionList;
private:
	m4date_t		m_dLastModificationDate;
	m4VariantType	m_vRepositoryKey;
};


class M4_DECL_M4OBJCACHEDIRECTORY ClMDUpdateList
{
public:
	ClMDUpdateList(m4date_t	ai_dLastModificationDate, m4uint32_t ai_iListMaxSize);
	~ClMDUpdateList(void);

	m4return_t ClearList(void);

	m4return_t RefreshList(ClChannelManager * ai_pChannelManager);
	m4return_t RefreshLastModificationDate(ClChannelManager * ai_pChannelManager);
	m4return_t CutList(void);
	m4return_t CreateUpdateDeltasList( m4date_t ai_dLastModificationDate, m4VariantType &ai_vRepositoryKey, ClMDUpdateDeltasList *& ao_oMDUpdateDeltasList);

	m4date_t GetLastModificationDate()
	{
		return m_dLastModificationDate;
	}

	m4date_t GetFirstModificationDate()
	{
		return m_dFirstModificationDate;
	}

	m4VariantType& GetRepositoryKey()
	{
		return m_vRepositoryKey;
	}

	void SetFirstModificationDate(m4date_t ai_dFirstModificationDate)
	{
		m_dFirstModificationDate = ai_dFirstModificationDate;
	}

	m4uint32_t GetListMaxSize()
	{
		return m_iListMaxSize;
	}

	void SetListMaxSize(m4uint32_t ai_iListMaxSize)
	{
		m_iListMaxSize = ai_iListMaxSize;
	}

private:
	m4return_t CompactList(void);
	m4return_t LoadListByDate(m4uint32_t & ao_iNumRecords);
	m4return_t LoadRepositoryKey(void);
	m4return_t LoadLastModificationDate(void);

	ClChannel *		m_poChList;
	ClAccess  *		m_poAccListByVer;
	ClAccess  *		m_poAccListById;

	m4date_t		m_dLastModificationDate;
	m4date_t		m_dFirstModificationDate;
	m4VariantType	m_vRepositoryKey;
	m4uint32_t		m_iListMaxSize;

	ClMutex			m_oMutex;
};

#endif

