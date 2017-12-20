//## begin module%34B4B5BF029C.cm preserve=no
//## end module%34B4B5BF029C.cm

//## begin module%34B4B5BF029C.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%34B4B5BF029C.cp

//## Module: ClCache%34B4B5BF029C; Package body
//## Subsystem: M4Cache::src%3796F4B100FD
//## Source file: D:\Work\v500\M4Cache\src\clcache.cpp

//## begin module%34B4B5BF029C.additionalIncludes preserve=no
//## end module%34B4B5BF029C.additionalIncludes

//## begin module%34B4B5BF029C.includes preserve=yes
#include "m4stl.hpp"
#include "m4cacheres.hpp"
#include "m4log.hpp"
#include "m4magicn.hpp"
#include "chlog.hpp"
//	Rose 98 Body Include Dependences
#include "blocksynchronization.hpp"
//## end module%34B4B5BF029C.includes

// syncro
#include <syncro.hpp>
// dumpcache
#include <dumpcache.hpp>
// ClCache
#include <clcache.hpp>
// ClSelfPersistiableObject
#include <clprsstb.hpp>
// ClSerializableObject
#include <clsrlzbl.hpp>
// m4date
#include <m4date.hpp>


//## begin module%34B4B5BF029C.declarations preserve=no
//## end module%34B4B5BF029C.declarations

//## begin module%34B4B5BF029C.additionalDeclarations preserve=yes

//static ClMutex g_oMutex(M4_TRUE);
#define M4_CACHE_VER	( 1 + ( 190 << 7 ) )
//## end module%34B4B5BF029C.additionalDeclarations


// Class ClCacheInterface 

ClCacheInterface::ClCacheInterface ()
  //## begin ClCacheInterface::ClCacheInterface%896947319.hasinit preserve=no
  //## end ClCacheInterface::ClCacheInterface%896947319.hasinit
  //## begin ClCacheInterface::ClCacheInterface%896947319.initialization preserve=yes
  //## end ClCacheInterface::ClCacheInterface%896947319.initialization
{
  //## begin ClCacheInterface::ClCacheInterface%896947319.body preserve=yes
  //## end ClCacheInterface::ClCacheInterface%896947319.body
}



//## Other Operations (implementation)
m4return_t ClCacheInterface::PutObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheInterface::PutObject%886420473.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::PutObject%886420473.body
}

m4return_t ClCacheInterface::NoUpdateObject (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheInterface::NoUpdateObject%894362622.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::NoUpdateObject%894362622.body
}

m4return_t ClCacheInterface::UpdateObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheInterface::UpdateObject%886420475.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::UpdateObject%886420475.body
}

m4return_t ClCacheInterface::GetObjectCopy (ClCacheKey *ai_pKey, ClCacheableObject *&ao_pObject)
{
  //## begin ClCacheInterface::GetObjectCopy%886420474.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::GetObjectCopy%886420474.body
}

m4return_t ClCacheInterface::RemoveAll ()
{
  //## begin ClCacheInterface::RemoveAll%886420483.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::RemoveAll%886420483.body
}

m4return_t ClCacheInterface::RemoveObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheInterface::RemoveObject%886420476.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::RemoveObject%886420476.body
}

m4return_t ClCacheInterface::Refresh (ClSelfPersistiableObject *m_poSelfPersistor)
{
  //## begin ClCacheInterface::Refresh%897316884.body preserve=yes
	return M4_SUCCESS;

  //## end ClCacheInterface::Refresh%897316884.body
}

m4return_t ClCacheInterface::ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject)
{
  //## begin ClCacheInterface::ReplaceObject%945346492.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheInterface::ReplaceObject%945346492.body
}

// Additional Declarations
  //## begin ClCacheInterface%3496E1DD00AB.declarations preserve=yes
  //## end ClCacheInterface%3496E1DD00AB.declarations

// Class ClCacheKey 




ClCacheKey::ClCacheKey (m4pchar_t ai_pszKey)
  //## begin ClCacheKey::ClCacheKey%886420471.hasinit preserve=no
  //## end ClCacheKey::ClCacheKey%886420471.hasinit
  //## begin ClCacheKey::ClCacheKey%886420471.initialization preserve=yes
  //## end ClCacheKey::ClCacheKey%886420471.initialization
{
  //## begin ClCacheKey::ClCacheKey%886420471.body preserve=yes
	if (ai_pszKey == NULL)
	{
		m_pszKey = NULL;
		m_uiSize = 0;
		return;
	}

	m_uiSize=strlen(ai_pszKey) + 1;
	m_pszKey = new m4char_t [m_uiSize];
	if (m_pszKey == NULL)
		return;
	strcpy(m_pszKey,ai_pszKey);
  //## end ClCacheKey::ClCacheKey%886420471.body
}


ClCacheKey::~ClCacheKey ()
{
  //## begin ClCacheKey::~ClCacheKey%892796790.body preserve=yes
	if (m_pszKey != NULL)
	{
		delete []m_pszKey;
		m_pszKey=NULL;
		m_uiSize = 0;
	}
  //## end ClCacheKey::~ClCacheKey%892796790.body
}



//## Other Operations (implementation)
m4return_t ClCacheKey::Serialize (ClGenericIODriver &  IOD)
{
  //## begin ClCacheKey::Serialize%893688336.body preserve=yes
	m4return_t returnIOD;

	returnIOD=IOD.Write((void*)m_pszKey,m_uiSize);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCacheKey::Serialize%893688336.body
}

m4return_t  ClCacheKey::DeSerialize (ClGenericIODriver &  IOD)
{
  //## begin ClCacheKey::DeSerialize%893688337.body preserve=yes
	m4return_t returnIOD;

	returnIOD=IOD.ReadSize(m_uiSize);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_pszKey=new char[m_uiSize];

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	returnIOD=IOD.ReadBuffer((void *)m_pszKey);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCacheKey::DeSerialize%893688337.body
}

m4pchar_t ClCacheKey::Getm_pszKey ()
{
  //## begin ClCacheKey::Getm_pszKey%894362626.body preserve=yes
	return m_pszKey;
  //## end ClCacheKey::Getm_pszKey%894362626.body
}

m4return_t ClCacheKey::Dump (ClDumpCacheVisitor &ai_oClDumpCacheVisitor)
{
  //## begin ClCacheKey::Dump%917950244.body preserve=yes
	ai_oClDumpCacheVisitor<< m_pszKey<< EndDumpCacheLn;

	return M4_SUCCESS;
  //## end ClCacheKey::Dump%917950244.body
}

// Additional Declarations
  //## begin ClCacheKey%3496E226038A.declarations preserve=yes
  //## end ClCacheKey%3496E226038A.declarations

// Class ClCacheableObject 










ClCacheableObject::ClCacheableObject (ClCachePolicy *ai_poPolicy, m4uint32_t  ai_iSizeObject, ClCacheKey *  ai_poClCacheKey)
  //## begin ClCacheableObject::ClCacheableObject%886420462.hasinit preserve=no
  //## end ClCacheableObject::ClCacheableObject%886420462.hasinit
  //## begin ClCacheableObject::ClCacheableObject%886420462.initialization preserve=yes
  //## end ClCacheableObject::ClCacheableObject%886420462.initialization
{
  //## begin ClCacheableObject::ClCacheableObject%886420462.body preserve=yes
	m_poPolicy=ai_poPolicy;

	m_iRefCounter=0;

	m_bIsDisabled = M4_FALSE;

	m_iSizeObject=ai_iSizeObject;

	m_bDisk=M4_FALSE;

	m_bRam=M4_TRUE;

	m_poLRUDate=new M4ClTimeStamp;

	m_poClCacheKey=ai_poClCacheKey;

	m_pEnvId = 0;
  //## end ClCacheableObject::ClCacheableObject%886420462.body
}


ClCacheableObject::~ClCacheableObject ()
{
  //## begin ClCacheableObject::~ClCacheableObject%886595261.body preserve=yes
	delete m_poLRUDate;

	m_poLRUDate=NULL;

	delete m_poPolicy;

	m_poPolicy=NULL;

	if (m_pEnvId)
		delete m_pEnvId;

	m_pEnvId = 0;
  //## end ClCacheableObject::~ClCacheableObject%886595261.body
}



//## Other Operations (implementation)
m4return_t ClCacheableObject::SetPolicy (ClCachePolicy *  ai_poPolicy)
{
  //## begin ClCacheableObject::SetPolicy%888398565.body preserve=yes
	m_poPolicy = ai_poPolicy;

	return M4_SUCCESS;
  //## end ClCacheableObject::SetPolicy%888398565.body
}

m4return_t ClCacheableObject::SetKey (ClCacheKey *ai_poClCacheKey)
{
  //## begin ClCacheableObject::SetKey%888398552.body preserve=yes
	m_poClCacheKey=ai_poClCacheKey;

	return M4_SUCCESS;
  //## end ClCacheableObject::SetKey%888398552.body
}

m4return_t ClCacheableObject::GetKey (ClCacheKey * &  ao_poClCacheKey)
{
  //## begin ClCacheableObject::GetKey%886497412.body preserve=yes
	ao_poClCacheKey=m_poClCacheKey;

	return M4_SUCCESS;
  //## end ClCacheableObject::GetKey%886497412.body
}

m4return_t ClCacheableObject::GetPolicy (ClCachePolicy * &  ao_poPolicy)
{
  //## begin ClCacheableObject::GetPolicy%886420463.body preserve=yes
	ao_poPolicy = m_poPolicy;

	return M4_SUCCESS;
  //## end ClCacheableObject::GetPolicy%886420463.body
}

ClCacheableObject & ClCacheableObject::operator = (ClCacheableObject &ai_oObject)
{
  //## begin ClCacheableObject::operator=%886420464.body preserve=yes
  return ai_oObject;
  //## end ClCacheableObject::operator=%886420464.body
}

m4bool_t ClCacheableObject::IsReferenced ()
{
  //## begin ClCacheableObject::IsReferenced%886420465.body preserve=yes
	return m_iRefCounter ? M4_TRUE : M4_FALSE;
  //## end ClCacheableObject::IsReferenced%886420465.body
}

m4uint32_t ClCacheableObject::GetNumOfRef ()
{
  //## begin ClCacheableObject::GetNumOfRef%886509525.body preserve=yes
	return m_iRefCounter;
  //## end ClCacheableObject::GetNumOfRef%886509525.body
}

void ClCacheableObject::Lock ()
{
  //## begin ClCacheableObject::Lock%888566514.body preserve=yes
  //## end ClCacheableObject::Lock%888566514.body
}

void ClCacheableObject::Unlock ()
{
  //## begin ClCacheableObject::Unlock%888566515.body preserve=yes
  //## end ClCacheableObject::Unlock%888566515.body
}

void ClCacheableObject::Enable ()
{
  //## begin ClCacheableObject::Enable%892745123.body preserve=yes
	m_bIsDisabled = M4_FALSE;
  //## end ClCacheableObject::Enable%892745123.body
}

void ClCacheableObject::Disable ()
{
  //## begin ClCacheableObject::Disable%892745124.body preserve=yes
	m_bIsDisabled = M4_TRUE;
  //## end ClCacheableObject::Disable%892745124.body
}

m4bool_t ClCacheableObject::IsDisabled ()
{
  //## begin ClCacheableObject::IsDisabled%892745125.body preserve=yes
	return m_bIsDisabled;
  //## end ClCacheableObject::IsDisabled%892745125.body
}

m4return_t ClCacheableObject::Serialize (ClGenericIODriver &   IOD)
{
  //## begin ClCacheableObject::Serialize%893688329.body preserve=yes
	m4return_t returnIOD;
	m4bool_t bExists=M4_FALSE;
	m4time_t NumSeconds;

	//Flag IsDisabled
	returnIOD=IOD.Write(m_bIsDisabled);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Tamaño del objeto
	returnIOD=IOD.Write(m_iSizeObject);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Política.
	returnIOD=IOD.Write(m_poPolicy,&bExists);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	if(!bExists)
	{
		returnIOD=m_poPolicy->Serialize(IOD);
	}

	//Fecha de Inserción
	m_oInsertionDate.get(NumSeconds);
	returnIOD=IOD.Write(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Fecha de última utilización (LRU)
	m_poLRUDate->get(NumSeconds);
	returnIOD=IOD.Write(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	return returnIOD;
  //## end ClCacheableObject::Serialize%893688329.body
}

m4return_t ClCacheableObject::DeSerialize (ClGenericIODriver &  IOD)
{
  //## begin ClCacheableObject::DeSerialize%893688330.body preserve=yes
	m4return_t returnIOD;
	m4bool_t bExists=M4_FALSE;
	m4time_t NumSeconds;

	//Flag IsDisabled
	returnIOD=IOD.Read(m_bIsDisabled);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Tamaño del objeto
	returnIOD=IOD.Read(m_iSizeObject);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Política.
	returnIOD=IOD.Read((void **)&m_poPolicy,&bExists);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	if(!bExists)
	{
		m_poPolicy=new ClCachePolicy;

		returnIOD=IOD.StoreAddress(m_poPolicy);

		if(returnIOD==M4_ERROR)
		{
			return M4_ERROR;
		}

		returnIOD=m_poPolicy->DeSerialize(IOD);

		if(returnIOD==M4_ERROR)
		{
			return M4_ERROR;
		}
	}

	//Fecha de Inserción
	returnIOD=IOD.Read(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_oInsertionDate.set(NumSeconds);

	//Fecha de última utilización (LRU)
	returnIOD=IOD.Read(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_poLRUDate->set(NumSeconds);

	return returnIOD;
  //## end ClCacheableObject::DeSerialize%893688330.body
}

m4uint8_t  ClCacheableObject::GetType (void  )
{
  //## begin ClCacheableObject::GetType%893688331.body preserve=yes
	return M4_CO_FACTORY_BASIC_CACHEABLE_OBJECT;
  //## end ClCacheableObject::GetType%893688331.body
}

m4uint32_t  ClCacheableObject::GetSize ()
{
  //## begin ClCacheableObject::GetSize%893952352.body preserve=yes
	return m_iSizeObject;
  //## end ClCacheableObject::GetSize%893952352.body
}

void  ClCacheableObject::SetSize (m4uint32_t  ai_iSizeObject)
{
  //## begin ClCacheableObject::SetSize%893952353.body preserve=yes
	m_iSizeObject = ai_iSizeObject;
  //## end ClCacheableObject::SetSize%893952353.body
}

m4uint8_t  ClCacheableObject::GetPersistianceLevel ()
{
  //## begin ClCacheableObject::GetPersistianceLevel %893952355.body preserve=yes
	return 0;
  //## end ClCacheableObject::GetPersistianceLevel %893952355.body
}

m4return_t  ClCacheableObject::SetLRUDate (M4ClTimeStamp *   ai_poLRUDate)
{
  //## begin ClCacheableObject::SetLRUDate %894447701.body preserve=yes
	m_poLRUDate = ai_poLRUDate;
	return M4_SUCCESS;
  //## end ClCacheableObject::SetLRUDate %894447701.body
}

M4ClTimeStamp *  ClCacheableObject::GetLRUDate ()
{
  //## begin ClCacheableObject::GetLRUDate %894447702.body preserve=yes
	return m_poLRUDate;
  //## end ClCacheableObject::GetLRUDate %894447702.body
}

m4return_t ClCacheableObject::Setm_bDisk (m4bool_t  ai_bDisk)
{
  //## begin ClCacheableObject::Setm_bDisk%894639698.body preserve=yes
	m_bDisk=ai_bDisk;
	return M4_SUCCESS;
  //## end ClCacheableObject::Setm_bDisk%894639698.body
}

m4bool_t ClCacheableObject::Getm_bDisk ()
{
  //## begin ClCacheableObject::Getm_bDisk%894639699.body preserve=yes
	return m_bDisk;
  //## end ClCacheableObject::Getm_bDisk%894639699.body
}

m4return_t ClCacheableObject::LoadValueFromDisk ()
{
  //## begin ClCacheableObject::LoadValueFromDisk%894884181.body preserve=yes
	return M4_ERROR;
  //## end ClCacheableObject::LoadValueFromDisk%894884181.body
}

m4return_t ClCacheableObject::SaveValueToDisk ()
{
  //## begin ClCacheableObject::SaveValueToDisk%894884182.body preserve=yes
	return M4_ERROR;
  //## end ClCacheableObject::SaveValueToDisk%894884182.body
}

m4return_t ClCacheableObject::DeleteValueFromDisk ()
{
  //## begin ClCacheableObject::DeleteValueFromDisk%894884183.body preserve=yes
	return M4_ERROR;
  //## end ClCacheableObject::DeleteValueFromDisk%894884183.body
}

m4return_t ClCacheableObject::DeleteValueFromRam ()
{
  //## begin ClCacheableObject::DeleteValueFromRam%896181065.body preserve=yes
	return M4_ERROR;
  //## end ClCacheableObject::DeleteValueFromRam%896181065.body
}

M4ClTimeStamp  ClCacheableObject::GetInsertionDate ()
{
  //## begin ClCacheableObject::GetInsertionDate%895764959.body preserve=yes
	return m_oInsertionDate;
  //## end ClCacheableObject::GetInsertionDate%895764959.body
}

m4return_t ClCacheableObject::SetInsertionDate (M4ClTimeStamp  ai_oInsertionDate)
{
  //## begin ClCacheableObject::SetInsertionDate%895764960.body preserve=yes
	m_oInsertionDate=ai_oInsertionDate;

	return M4_SUCCESS;
  //## end ClCacheableObject::SetInsertionDate%895764960.body
}

m4return_t ClCacheableObject::Setm_bRam (m4bool_t ai_bRam)
{
  //## begin ClCacheableObject::Setm_bRam%896181066.body preserve=yes
	m_bRam=ai_bRam;

	return M4_SUCCESS;
  //## end ClCacheableObject::Setm_bRam%896181066.body
}

m4bool_t ClCacheableObject::Getm_bRam ()
{
  //## begin ClCacheableObject::Getm_bRam%896181067.body preserve=yes
	return m_bRam;
  //## end ClCacheableObject::Getm_bRam%896181067.body
}

m4return_t ClCacheableObject::Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor)
{
  //## begin ClCacheableObject::Dump%917950245.body preserve=yes
	M4ClString Date;

	ai_oClDumpCacheVisitor << "\t\tKey: ";

	m_poClCacheKey->Dump(ai_oClDumpCacheVisitor);
	ai_oClDumpCacheVisitor << "\t\tFlag IsDisabled: "<< m_bIsDisabled << EndDumpCacheLn;
	
	if ( ai_oClDumpCacheVisitor.DumpSizes() == M4_TRUE )
	{
		ai_oClDumpCacheVisitor << "\t\tObject Size: "<< m_iSizeObject << EndDumpCacheLn;
	}
	
	if( ai_oClDumpCacheVisitor.DumpDates() == M4_TRUE )
	{
		ai_oClDumpCacheVisitor << "\t\tPolicy: " << EndDumpCacheLn;
		m_poPolicy->Dump(ai_oClDumpCacheVisitor);
		
		m_oInsertionDate.get(Date);
		ai_oClDumpCacheVisitor << "\t\tInsertion Date: "<<(m4pchar_t)Date << EndDumpCacheLn;
		
		m_poLRUDate->get(Date);
		ai_oClDumpCacheVisitor << "\t\tLRU Date: "<< (m4pchar_t)Date << EndDumpCacheLn;
	}

	ai_oClDumpCacheVisitor << "\t\tCounter of References: "<< m_iRefCounter << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tFlag m_bDisk: "<< m_bDisk << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tFlag m_bRam: "<< m_bRam << EndDumpCacheLn;
	
	return M4_SUCCESS;
  //## end ClCacheableObject::Dump%917950245.body
}

m4bool_t ClCacheableObject::MatchCondition (void *  ai_pCondition)
{
  //## begin ClCacheableObject::MatchCondition%943372022.body preserve=yes
	return M4_FALSE;
  //## end ClCacheableObject::MatchCondition%943372022.body
}

// Additional Declarations
  //## begin ClCacheableObject%3496E277009C.declarations preserve=yes
m4return_t ClCacheableObject::NotifyCacheDestroy ()
{
	//La clase base no hace nada con este evento.
	return M4_TRUE;
}

void ClCacheableObject::SetEnvId(m4pchar_t ai_pcEnvId)
{
	if (m_pEnvId)
		delete m_pEnvId;

	m_pEnvId = new ClEnvironmentId(ai_pcEnvId);
}

m4pchar_t ClCacheableObject::GetEnvId(void)
 {
	if (m_pEnvId)
		return m_pEnvId->GetId();
	else
		return 0;
}

  //## end ClCacheableObject%3496E277009C.declarations
// Class ClCachePolicy 







ClCachePolicy::ClCachePolicy (m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod)
  //## begin ClCachePolicy::ClCachePolicy%886420470.hasinit preserve=no
  //## end ClCachePolicy::ClCachePolicy%886420470.hasinit
  //## begin ClCachePolicy::ClCachePolicy%886420470.initialization preserve=yes
  //## end ClCachePolicy::ClCachePolicy%886420470.initialization
{
  //## begin ClCachePolicy::ClCachePolicy%886420470.body preserve=yes
	//OJO con la clase M4ClTimeStamp!!
	//Ojo, el operador += no es igual que +
	//m_dMaxUnusedPeriod es m4double_t y espera un m4time_t. es un poco feo, pero por dentro, ambos son doubles.
	m_oStartDate.now();
	m_oEndDate=m_oStartDate;
	m_oEndDate+=(m4time_t)ai_dMaxPeriod;

	m_dMaxUnusedPeriod = ai_dMaxUnusedPeriod;
	m_oUnusedEndDate = m_oStartDate;
	m_oUnusedEndDate+= (m4time_t)m_dMaxUnusedPeriod;
  //## end ClCachePolicy::ClCachePolicy%886420470.body
}

ClCachePolicy::ClCachePolicy ()
  //## begin ClCachePolicy::ClCachePolicy%959345793.hasinit preserve=no
  //## end ClCachePolicy::ClCachePolicy%959345793.hasinit
  //## begin ClCachePolicy::ClCachePolicy%959345793.initialization preserve=yes
  //## end ClCachePolicy::ClCachePolicy%959345793.initialization
{
  //## begin ClCachePolicy::ClCachePolicy%959345793.body preserve=yes
   m_dMaxUnusedPeriod = 0;
  //## end ClCachePolicy::ClCachePolicy%959345793.body
}


ClCachePolicy::~ClCachePolicy ()
{
  //## begin ClCachePolicy::~ClCachePolicy%888398554.body preserve=yes
  //## end ClCachePolicy::~ClCachePolicy%888398554.body
}



//## Other Operations (implementation)
m4bool_t ClCachePolicy::IsValid (M4ClTimeStamp &  ai_oActualTime)
{
  //## begin ClCachePolicy::IsValid%888398555.body preserve=yes
	if (M4_FALSE == IsValidMaxDate(ai_oActualTime))
		return M4_FALSE;
		
	if (M4_FALSE == IsValidUnusedPeriod(ai_oActualTime))
		return M4_FALSE;

	return M4_TRUE;
  //## end ClCachePolicy::IsValid%888398555.body
}

m4return_t ClCachePolicy::Serialize (ClGenericIODriver &  IOD)
{
  //## begin ClCachePolicy::Serialize%893835147.body preserve=yes
	m4return_t returnIOD;
	m4time_t NumSeconds;

	//Fecha de inicio.
	m_oStartDate.get(NumSeconds);
	returnIOD=IOD.Write(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}
	
	//Fecha de fin.
	m_oEndDate.get(NumSeconds);
	returnIOD=IOD.Write(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//De momento no usado.
	m_oUnusedEndDate.get(NumSeconds);
	returnIOD=IOD.Write(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//De momento no usado.
	returnIOD=IOD.Write(m_dMaxUnusedPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}
	
	return returnIOD;
  //## end ClCachePolicy::Serialize%893835147.body
}

m4return_t ClCachePolicy::DeSerialize (ClGenericIODriver &  IOD)
{
  //## begin ClCachePolicy::DeSerialize%893835148.body preserve=yes
	m4return_t returnIOD;
	m4time_t NumSeconds;

	//Fecha de inicio.
	returnIOD=IOD.Read(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_oStartDate.set(NumSeconds);

	//Fecha de fin.
	returnIOD=IOD.Read(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_oEndDate.set(NumSeconds);

	//De momento no usado.
	returnIOD=IOD.Read(NumSeconds);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	m_oUnusedEndDate.set(NumSeconds);

	//De momento no usado.
	returnIOD=IOD.Read(m_dMaxUnusedPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	return returnIOD;
  //## end ClCachePolicy::DeSerialize%893835148.body
}

m4return_t ClCachePolicy::Dump (ClDumpCacheVisitor &  ai_pClDumpCacheVisitor)
{
  //## begin ClCachePolicy::Dump%917950246.body preserve=yes
	M4ClString Date;
	
	m_oStartDate.get(Date);
	ai_pClDumpCacheVisitor << "\t\t\tStart date: " << (m4pchar_t)Date << EndDumpCacheLn;

	m_oEndDate.get(Date);
	ai_pClDumpCacheVisitor << "\t\t\tEnd date: " << (m4pchar_t)Date << EndDumpCacheLn ;

	m_oUnusedEndDate.get(Date);
	ai_pClDumpCacheVisitor << "\t\t\tUnused End date: " << (m4pchar_t)Date << EndDumpCacheLn;

	ai_pClDumpCacheVisitor << "\t\t\tMax unused period: " << m_dMaxUnusedPeriod << EndDumpCacheLn;

	return M4_SUCCESS;
  //## end ClCachePolicy::Dump%917950246.body
}

m4bool_t ClCachePolicy::IsValidUnusedPeriod (M4ClTimeStamp &ai_oActualTime)
{
  //## begin ClCachePolicy::IsValidUnusedPeriod%959345794.body preserve=yes
	if  ((m_dMaxUnusedPeriod == 0) || (m_oUnusedEndDate == m_oStartDate))
		return M4_TRUE;

	if (ai_oActualTime > m_oUnusedEndDate)
		return M4_FALSE;

	return M4_TRUE;
  //## end ClCachePolicy::IsValidUnusedPeriod%959345794.body
}

m4bool_t ClCachePolicy::IsValidMaxDate (M4ClTimeStamp &ai_oActualTime)
{
  //## begin ClCachePolicy::IsValidMaxDate%959345795.body preserve=yes
	if( m_oStartDate == m_oEndDate )
	{
		//objeto con persistencia infinita en la cache.
		return M4_TRUE;
	}

	if( m_oStartDate > m_oEndDate)
	{
		//caso de error, si Start>End, el objeto no es válido.
		return M4_FALSE;
	}
	else if ( (ai_oActualTime < m_oStartDate) || (ai_oActualTime > m_oEndDate ) )
	{
		//La fecha actual está fuera del rango.
		return M4_FALSE;
	}

	return M4_TRUE;
  //## end ClCachePolicy::IsValidMaxDate%959345795.body
}

m4return_t ClCachePolicy::ActivateUnusedPeriod ()
{
  //## begin ClCachePolicy::ActivateUnusedPeriod%959345796.body preserve=yes
	if (m_dMaxUnusedPeriod != 0)
	{
		m_oUnusedEndDate.now();
		m_oUnusedEndDate += (m4time_t)m_dMaxUnusedPeriod;
	}
	return M4_SUCCESS;
  //## end ClCachePolicy::ActivateUnusedPeriod%959345796.body
}

m4return_t ClCachePolicy::DesactivateUnusedPeriod ()
{
  //## begin ClCachePolicy::DesactivateUnusedPeriod%959345797.body preserve=yes
	if (m_dMaxUnusedPeriod != 0)
	{
		m_oUnusedEndDate = m_oStartDate;
	}
	return M4_SUCCESS;
  //## end ClCachePolicy::DesactivateUnusedPeriod%959345797.body
}

// Additional Declarations
  //## begin ClCachePolicy%3496E28B02DE.declarations preserve=yes
  //## end ClCachePolicy%3496E28B02DE.declarations

// Class ClCache 



ClCache::ClCache (ClCacheStyle *ai_poClCacheStyle)
  //## begin ClCache::ClCache%886497410.hasinit preserve=no
  //## end ClCache::ClCache%886497410.hasinit
  //## begin ClCache::ClCache%886497410.initialization preserve=yes
  //## end ClCache::ClCache%886497410.initialization
{
  //## begin ClCache::ClCache%886497410.body preserve=yes
	m_poClCacheStyle=ai_poClCacheStyle;
	m_poPersistor=NULL;
  //## end ClCache::ClCache%886497410.body
}

ClCache::ClCache ()
  //## begin ClCache::ClCache%886595255.hasinit preserve=no
  //## end ClCache::ClCache%886595255.hasinit
  //## begin ClCache::ClCache%886595255.initialization preserve=yes
  //## end ClCache::ClCache%886595255.initialization
{
  //## begin ClCache::ClCache%886595255.body preserve=yes
	m_poClCacheStyle=NULL;
	m_poPersistor=NULL;
  //## end ClCache::ClCache%886595255.body
}


ClCache::~ClCache ()
{
  //## begin ClCache::~ClCache%886420484.body preserve=yes
  //## end ClCache::~ClCache%886420484.body
}



//## Other Operations (implementation)
m4return_t ClCache::Serialize (ClGenericIODriver &  IOD)
{
  //## begin ClCache::Serialize%893835151.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->Serialize(IOD);
  //## end ClCache::Serialize%893835151.body
}

m4return_t ClCache::DeSerialize (ClGenericIODriver &  IOD)
{
  //## begin ClCache::DeSerialize%893835152.body preserve=yes
	ClObjectId* pObj = NULL;
	m4pchar_t pcEnvId = NULL;

	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	pObj = GetObjId();
	if( pObj != NULL )
	{
		pcEnvId = pObj->GetEnvironmentId();
	}

	return m_poClCacheStyle->DeSerialize(IOD, pcEnvId);
  //## end ClCache::DeSerialize%893835152.body
}

m4return_t ClCache::Persist ()
{
  //## begin ClCache::Persist%897034127.body preserve=yes
	if(!m_poPersistor)
	{
		g_oChLog<<BeginDebug(M4_CACHE_PERSIST)<<EndLog;			
		return M4_ERROR;
	}
	
	if ((m_poPersistor->Persist(*this)) == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;

  //## end ClCache::Persist%897034127.body
}

m4return_t ClCache::DePersist ()
{
  //## begin ClCache::DePersist%897034128.body preserve=yes
	if(!m_poPersistor)
	{
		g_oChLog<<BeginError(M4_CACHE_DEPERSIST)<<EndLog;			
		return M4_ERROR;
	}
	
	if((m_poPersistor->DePersist(*this))==M4_ERROR)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCache::DePersist%897034128.body
}

m4return_t ClCache::Delete ()
{
  //## begin ClCache::Delete%897034129.body preserve=yes
	if(!m_poPersistor)
	{
		g_oChLog<<BeginWarning(M4_CACHE_DELETE)<<EndLog;			
		return M4_ERROR;
	}
	
	if((m_poPersistor->Delete(*this))==M4_ERROR)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCache::Delete%897034129.body
}

ClPersistor *  ClCache::Getm_poPersistor ()
{
  //## begin ClCache::Getm_poPersistor%897034130.body preserve=yes
	return m_poPersistor;
  //## end ClCache::Getm_poPersistor%897034130.body
}

m4return_t ClCache::Setm_poPersistor (ClPersistor *  ai_poPersistor)
{
  //## begin ClCache::Setm_poPersistor%897034131.body preserve=yes
	m_poPersistor=ai_poPersistor;
	return M4_SUCCESS;
  //## end ClCache::Setm_poPersistor%897034131.body
}

m4return_t ClCache::Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor)
{
  //## begin ClCache::Dump%917950247.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->Dump(ai_oClDumpCacheVisitor);
  //## end ClCache::Dump%917950247.body
}

m4uint32_t ClCache::GetNObjInCache ()
{
  //## begin ClCache::GetNObjInCache%921662495.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->GetNObjInCache();
  //## end ClCache::GetNObjInCache%921662495.body
}

m4uint32_t ClCache::GetSizeCache ()
{
  //## begin ClCache::GetSizeCache%945090197.body preserve=yes
	if (m_poClCacheStyle != NULL)
		return m_poClCacheStyle->GetSizeCache();
	return 0;
  //## end ClCache::GetSizeCache%945090197.body
}

ClMutex & ClCache::GetMutex ()
{
  //## begin ClCache::GetMutex%1003764701.body preserve=yes
	return m_poClCacheStyle->GetMutex();
  //## end ClCache::GetMutex%1003764701.body
}

// Additional Declarations
  //## begin ClCache%34D5A9D7002E.declarations preserve=yes
  //## end ClCache%34D5A9D7002E.declarations

// Class ClCacheTest 


ClCacheTest::ClCacheTest (ClCacheStyle *ai_poClCacheStyle)
  //## begin ClCacheTest::ClCacheTest%886420479.hasinit preserve=no
  //## end ClCacheTest::ClCacheTest%886420479.hasinit
  //## begin ClCacheTest::ClCacheTest%886420479.initialization preserve=yes
  //## end ClCacheTest::ClCacheTest%886420479.initialization
{
  //## begin ClCacheTest::ClCacheTest%886420479.body preserve=yes
	m_poClCacheStyle=ai_poClCacheStyle;
  //## end ClCacheTest::ClCacheTest%886420479.body
}

ClCacheTest::ClCacheTest ()
  //## begin ClCacheTest::ClCacheTest%886595256.hasinit preserve=no
  //## end ClCacheTest::ClCacheTest%886595256.hasinit
  //## begin ClCacheTest::ClCacheTest%886595256.initialization preserve=yes
  //## end ClCacheTest::ClCacheTest%886595256.initialization
{
  //## begin ClCacheTest::ClCacheTest%886595256.body preserve=yes
  //## end ClCacheTest::ClCacheTest%886595256.body
}


ClCacheTest::~ClCacheTest ()
{
  //## begin ClCacheTest::~ClCacheTest%886420480.body preserve=yes
	delete m_poClCacheStyle;
	m_poClCacheStyle=NULL;
  //## end ClCacheTest::~ClCacheTest%886420480.body
}



//## Other Operations (implementation)
m4return_t ClCacheTest::SetCacheStyle (ClCacheStyle *ai_poClCacheStyle)
{
  //## begin ClCacheTest::SetCacheStyle%886595259.body preserve=yes
	m_poClCacheStyle=ai_poClCacheStyle;
	return M4_SUCCESS;
  //## end ClCacheTest::SetCacheStyle%886595259.body
}

m4return_t ClCacheTest::PutObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheTest::PutObject%888398556.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return 	m_poClCacheStyle->PutObject(ai_pObject);
  //## end ClCacheTest::PutObject%888398556.body
}

m4return_t ClCacheTest::RemoveObject (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTest::RemoveObject%888398557.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return 	m_poClCacheStyle->RemoveObject(ai_pObject);
  //## end ClCacheTest::RemoveObject%888398557.body
}

m4return_t ClCacheTest::UpdateObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheTest::UpdateObject%888398558.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	if(m_poClCacheStyle->Refresh(this)==M4_ERROR)
	{
		return M4_ERROR;
	}

	return 	m_poClCacheStyle->UpdateObject(ai_pObject);
  //## end ClCacheTest::UpdateObject%888398558.body
}

m4return_t ClCacheTest::GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode, m4int32_t  ai_iTimeOut)
{
  //## begin ClCacheTest::GetObject%892745126.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->GetObject(ai_pKey,ao_pObject,ai_eGetMode,ai_iTimeOut);
  //## end ClCacheTest::GetObject%892745126.body
}

m4return_t ClCacheTest::UpdateObjectKey (ClCacheableObject *ai_poClCacheableObject, ClCacheKey *ai_poClCacheKey)
{
  //## begin ClCacheTest::UpdateObjectKey%892745129.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->UpdateObjectKey(ai_poClCacheableObject,ai_poClCacheKey);
  //## end ClCacheTest::UpdateObjectKey%892745129.body
}

m4return_t ClCacheTest::SetCOFactory (ClCacheableObjectFactory *  ai_poClCOFactory)
{
  //## begin ClCacheTest::SetCOFactory%893835149.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->SetCOFactory (ai_poClCOFactory);
  //## end ClCacheTest::SetCOFactory%893835149.body
}

ClCacheableObjectFactory *  ClCacheTest::GetCOFactory ()
{
  //## begin ClCacheTest::GetCOFactory%893835150.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return NULL;
	}

	return m_poClCacheStyle->GetCOFactory ();
  //## end ClCacheTest::GetCOFactory%893835150.body
}

m4return_t ClCacheTest::NoUpdateObject (ClCacheableObject *   ai_pObject)
{
  //## begin ClCacheTest::NoUpdateObject%894362623.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheTest::NoUpdateObject%894362623.body
}

m4return_t ClCacheTest::Recycle ()
{
  //## begin ClCacheTest::Recycle%894985404.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->Recycle ();
  //## end ClCacheTest::Recycle%894985404.body
}

m4return_t ClCacheTest::RemoveAll (M4ClTimeStamp *  ai_poLimitDate)
{
  //## begin ClCacheTest::RemoveAll%895824025.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->RemoveAll(ai_poLimitDate);
  //## end ClCacheTest::RemoveAll%895824025.body
}

m4return_t ClCacheTest::RemoveAll (m4pchar_t ai_pcPortionKey, m4puint32_t ai_piRemovedCount)
{
  //## begin ClCacheTest::RemoveAll%895824026.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->RemoveAll(ai_pcPortionKey, ai_piRemovedCount);
  //## end ClCacheTest::RemoveAll%895824026.body
}

m4return_t ClCacheTest::RemoveAll ()
{
  //## begin ClCacheTest::RemoveAll%896690042.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->RemoveAll();
  //## end ClCacheTest::RemoveAll%896690042.body
}

m4return_t ClCacheTest::GetNext (ClCacheableObject * &  ao_poClCacheableObject, void * &  ai_poIterator)
{
  //## begin ClCacheTest::GetNext%910956612.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->GetNext(ao_poClCacheableObject,ai_poIterator);
  //## end ClCacheTest::GetNext%910956612.body
}

m4return_t ClCacheTest::SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs)
{
  //## begin ClCacheTest::SetMaxNOfObjs%910956613.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->SetMaxNOfObjs (ai_ulMaxNOfObjs);
  //## end ClCacheTest::SetMaxNOfObjs%910956613.body
}

m4return_t ClCacheTest::SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache)
{
  //## begin ClCacheTest::SetMaxSizeCache%910956614.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->SetMaxSizeCache (ai_ulMaxSizeCache);
  //## end ClCacheTest::SetMaxSizeCache%910956614.body
}

m4return_t ClCacheTest::RemoveByCondition (void *  ai_pCondition)
{
  //## begin ClCacheTest:: RemoveByCondition %943372023.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->RemoveByCondition(ai_pCondition);
  //## end ClCacheTest:: RemoveByCondition %943372023.body
}

m4return_t ClCacheTest::ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject)
{
  //## begin ClCacheTest::ReplaceObject%945346493.body preserve=yes
	if(!m_poClCacheStyle)
	{
		return M4_ERROR;
	}

	return m_poClCacheStyle->ReplaceObject(ai_pOldObject, ai_pNewObject);
  //## end ClCacheTest::ReplaceObject%945346493.body
}

// Additional Declarations
  //## begin ClCacheTest%34D5AA2002E6.declarations preserve=yes
  //## end ClCacheTest%34D5AA2002E6.declarations

// Class ClCacheStyle 
























ClCacheStyle::ClCacheStyle (m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod, m4uint32_t ai_ulMaxSizeCache, m4uint32_t ai_ulMaxNOfObjs, m4uint32_t ai_ulRefreshRatio)
  //## begin ClCacheStyle::ClCacheStyle%886420478.hasinit preserve=no
      : m_oHitsRatioCache(0), m_uiTotalHitsCache(0), m_uiTotalAccessCache(0), m_ulNumOfObjInDisk(0)
  //## end ClCacheStyle::ClCacheStyle%886420478.hasinit
  //## begin ClCacheStyle::ClCacheStyle%886420478.initialization preserve=yes
  //## end ClCacheStyle::ClCacheStyle%886420478.initialization
{
  //## begin ClCacheStyle::ClCacheStyle%886420478.body preserve=yes
	//Mutex....
	m_oMutexCounter.Init();
	m_oMutex.Init();

	//Contadores de objetos.
	m_ulNumOfObjInCache=0;
    m_ulNumOfObjInDelQueue=0;
	m_ulNumOfObjInLRU=0;
	m_ulNumOfObjectsDisabled=0;
	m_ulNumOfObjInDisk=0;

	//Referencias totales.
    m_ulTotalNumOfRef=0;

	//Hit ratio.
	m_oHitsRatioCache=0;
    m_uiTotalHitsCache=0;
    m_uiTotalAccessCache=0;

	//Refresh ratio.
	m_ulRefreshRatio=ai_ulRefreshRatio;
	//Contador para alcanzar el refresh ratio.
	m_ulNumOfUpdateObjs=0;

	//Política de la caché.
	m_dMaxPeriod=ai_dMaxPeriod;
	m_dMaxUnusedPeriod = ai_dMaxUnusedPeriod;

	//Tamaño en ram de la caché.
	m_ulSizeCache=0;

	//Límte de Ram en caché.
	m_ulMaxSizeCache=ai_ulMaxSizeCache;

	//Límite de num objs en la caché.
	m_ulMaxNOfObjects=ai_ulMaxNOfObjs;

	//Se utiliza para borrar objetos al superar el limite máximo de memoria, hasta llegar a m_ulMaxSizeCache-m_ulDelRatioMem. (Un 20% menos del límite).
	m_ulDelRatioMem=(ai_ulMaxSizeCache * 2)/10;

	//Se utiliza para borrar objetos al superar el limite máximo, hasta llegar a m_ulMaxNOfObjects-m_ulDelRatioObjs. (Un 20% menos del límite).
	m_ulDelRatioObjs=(ai_ulMaxNOfObjs * 2)/10;

	m_pcCacheName = NULL;	
  //## end ClCacheStyle::ClCacheStyle%886420478.body
}


ClCacheStyle::~ClCacheStyle ()
{
  //## begin ClCacheStyle::~ClCacheStyle%886420485.body preserve=yes
  //## end ClCacheStyle::~ClCacheStyle%886420485.body
}



//## Other Operations (implementation)
m4return_t ClCacheStyle::Lock ()
{
  //## begin ClCacheStyle::Lock%886497414.body preserve=yes
	//No utilizado.
	return	M4_SUCCESS;
  //## end ClCacheStyle::Lock%886497414.body
}

m4return_t ClCacheStyle::Unlock ()
{
  //## begin ClCacheStyle::Unlock%886497415.body preserve=yes
	//No utilizado.
	return	M4_SUCCESS;
  //## end ClCacheStyle::Unlock%886497415.body
}

m4return_t ClCacheStyle::Transfer (ClCacheStyle *ai_poClCacheStyle)
{
  //## begin ClCacheStyle::Transfer%886497413.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheStyle::Transfer%886497413.body
}

m4return_t ClCacheStyle::RemoveReference (ClCacheableObject *ai_poClCacheableObject)
{
  //## begin ClCacheStyle::RemoveReference%886501490.body preserve=yes
	return	M4_SUCCESS;
  //## end ClCacheStyle::RemoveReference%886501490.body
}

m4uint32_t ClCacheStyle::IncrNumOfRef (ClCacheableObject *aio_poClCacheableObject)
{
  //## begin ClCacheStyle::IncrNumOfRef%886509528.body preserve=yes
	ClMutBlock oBlock(m_oMutexCounter);

	//DANIEL. Quitar los ASSERT.
	M4_ASSERT(aio_poClCacheableObject->m_iRefCounter >= 0);

	m4uint32_t ulCounter;

	if (aio_poClCacheableObject->m_iRefCounter == 0)
	{
		ClCachePolicy * pCachePolicy = NULL;
		aio_poClCacheableObject->GetPolicy(pCachePolicy);

		//Desactivamos el UnusedPeriod: estamos cogiendo el objeto sin refs de la cache
		if (pCachePolicy)
			pCachePolicy->DesactivateUnusedPeriod();
	}

	ulCounter = ++aio_poClCacheableObject->m_iRefCounter;

	++m_ulTotalNumOfRef;

	return ulCounter;
  //## end ClCacheStyle::IncrNumOfRef%886509528.body
}

m4uint32_t ClCacheStyle::DecrNumOfRef (ClCacheableObject *aio_poClCacheableObject)
{
  //## begin ClCacheStyle::DecrNumOfRef%886509529.body preserve=yes
	ClMutBlock oBlock(m_oMutexCounter);

	M4_ASSERT(aio_poClCacheableObject->m_iRefCounter > 0);

	m4uint32_t ulCounter;

	if (aio_poClCacheableObject->m_iRefCounter == 1)
	{
		ClCachePolicy * pCachePolicy = NULL;
		aio_poClCacheableObject->GetPolicy(pCachePolicy);

		//Activamos el UnusedPeriod: estamos soltando el objeto sin refs a la cache
		if (pCachePolicy)
			pCachePolicy->ActivateUnusedPeriod();
	}

	if (aio_poClCacheableObject->m_iRefCounter)
	{
		ulCounter = --aio_poClCacheableObject->m_iRefCounter;
		--m_ulTotalNumOfRef;
	}
	
	return ulCounter;
  //## end ClCacheStyle::DecrNumOfRef%886509529.body
}

m4return_t ClCacheStyle::SetMaxPeriod (m4double_t ai_dMaxPeriod)
{
  //## begin ClCacheStyle::SetMaxPeriod%888398559.body preserve=yes
	m_dMaxPeriod = ai_dMaxPeriod;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetMaxPeriod%888398559.body
}

m4double_t ClCacheStyle::GetMaxPeriod ()
{
  //## begin ClCacheStyle::GetMaxPeriod%888398560.body preserve=yes
	return m_dMaxPeriod;
  //## end ClCacheStyle::GetMaxPeriod%888398560.body
}

m4return_t ClCacheStyle::SetMaxUnusedPeriod (m4double_t ai_dMaxUnusedPeriod)
{
  //## begin ClCacheStyle::SetMaxUnusedPeriod%960820227.body preserve=yes
	m_dMaxUnusedPeriod = ai_dMaxUnusedPeriod;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetMaxUnusedPeriod%960820227.body
}

m4double_t ClCacheStyle::GetMaxUnusedPeriod ()
{
  //## begin ClCacheStyle::GetMaxUnusedPeriod%959587676.body preserve=yes
	return m_dMaxUnusedPeriod;
  //## end ClCacheStyle::GetMaxUnusedPeriod%959587676.body
}

void  *  ClCacheStyle::GetCacheStats ()
{
  //## begin ClCacheStyle::GetCacheStats%889461538.body preserve=yes
	return m_poClCacheStats;
  //## end ClCacheStyle::GetCacheStats%889461538.body
}

m4return_t ClCacheStyle::SetCacheStats (void *   ai_poClCacheStats)
{
  //## begin ClCacheStyle::SetCacheStats%893330111.body preserve=yes
	m_poClCacheStats=ai_poClCacheStats;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetCacheStats%893330111.body
}

m4uint32_t ClCacheStyle::GetNOfRef ()
{
  //## begin ClCacheStyle::GetNOfRef%889694501.body preserve=yes
	return m_ulTotalNumOfRef;
  //## end ClCacheStyle::GetNOfRef%889694501.body
}

m4uint32_t ClCacheStyle::GetNObjInQueue ()
{
  //## begin ClCacheStyle::GetNObjInQueue%889694502.body preserve=yes
	return m_ulNumOfObjInDelQueue;
  //## end ClCacheStyle::GetNObjInQueue%889694502.body
}

m4uint32_t ClCacheStyle::GetNObjInCache ()
{
  //## begin ClCacheStyle::GetNObjInCache%889694503.body preserve=yes
	return m_ulNumOfObjInCache;
  //## end ClCacheStyle::GetNObjInCache%889694503.body
}

m4float_t ClCacheStyle::GetHitsRatio ()
{
  //## begin ClCacheStyle::GetHitsRatio%889863962.body preserve=yes
	if(m_uiTotalAccessCache)
	{
		return m_oHitsRatioCache=(m_uiTotalHitsCache*100)/m_uiTotalAccessCache;
	}
	else
	{
		return 0;
	}
  //## end ClCacheStyle::GetHitsRatio%889863962.body
}

m4return_t ClCacheStyle::Serialize (ClGenericIODriver &  IOD)
{
  //## begin ClCacheStyle::Serialize%893751386.body preserve=yes
	m4return_t returnIOD;
	m4bool_t bExists=M4_FALSE;

	//Num Obj en cache.
	returnIOD=IOD.Write(m_ulNumOfObjInCache);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}
	
	//Política por defecto.
	/*
	returnIOD=IOD.Write(m_dMaxPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	returnIOD=IOD.Write(m_dMaxUnusedPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}
	*/

	return returnIOD;
  //## end ClCacheStyle::Serialize%893751386.body
}

m4return_t ClCacheStyle::DeSerialize (ClGenericIODriver &  IOD, m4pchar_t ai_pcEnvId)
{
  //## begin ClCacheStyle::DeSerialize%893751387.body preserve=yes
	m4return_t returnIOD;

	//Num Obj en cache.
	returnIOD=IOD.Read(m_ulNumOfObjInCache);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//Política por defecto.
	/*
	returnIOD=IOD.Read(m_dMaxPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	returnIOD=IOD.Read(m_dMaxUnusedPeriod);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}
	*/

	return M4_SUCCESS;
  //## end ClCacheStyle::DeSerialize%893751387.body
}

m4uint32_t ClCacheStyle::GetSizeCache ()
{
  //## begin ClCacheStyle::GetSizeCache%895680864.body preserve=yes
	return m_ulSizeCache;
  //## end ClCacheStyle::GetSizeCache%895680864.body
}

m4uint32_t ClCacheStyle::GetMaxNOfObjs ()
{
  //## begin ClCacheStyle::GetMaxNOfObjs%897498322.body preserve=yes
	return m_ulMaxNOfObjects;
  //## end ClCacheStyle::GetMaxNOfObjs%897498322.body
}

m4uint32_t ClCacheStyle::GetMaxSizeCache ()
{
  //## begin ClCacheStyle::GetMaxSizeCache%897498323.body preserve=yes
	return m_ulMaxSizeCache;
  //## end ClCacheStyle::GetMaxSizeCache%897498323.body
}

m4uint32_t ClCacheStyle::GetRefreshRatio ()
{
  //## begin ClCacheStyle::GetRefreshRatio%897498324.body preserve=yes
	return m_ulRefreshRatio;
  //## end ClCacheStyle::GetRefreshRatio%897498324.body
}

m4return_t ClCacheStyle::SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache)
{
  //## begin ClCacheStyle::SetMaxSizeCache%897553670.body preserve=yes
	m_ulMaxSizeCache=ai_ulMaxSizeCache;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetMaxSizeCache%897553670.body
}

m4return_t ClCacheStyle::SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs)
{
  //## begin ClCacheStyle::SetMaxNOfObjs%897553671.body preserve=yes
	m_ulMaxNOfObjects=ai_ulMaxNOfObjs;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetMaxNOfObjs%897553671.body
}

m4return_t ClCacheStyle::SetRefreshRatio (m4uint32_t ai_ulRefreshRatio)
{
  //## begin ClCacheStyle::SetRefreshRatio%897553672.body preserve=yes
	m_ulRefreshRatio=ai_ulRefreshRatio;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetRefreshRatio%897553672.body
}

m4uint32_t ClCacheStyle::GetNOfObjInLRU ()
{
  //## begin ClCacheStyle::GetNOfObjInLRU%910956611.body preserve=yes
		return m_ulNumOfObjInLRU;
  //## end ClCacheStyle::GetNOfObjInLRU%910956611.body
}

m4return_t ClCacheStyle::Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor)
{
  //## begin ClCacheStyle::Dump%917950248.body preserve=yes
	m4bool_t bExists=M4_FALSE;

	ai_oClDumpCacheVisitor << "\tStatistics:"  << EndDumpCacheLn ;
	ai_oClDumpCacheVisitor << "\t\tNumber of objects in cache: "<< m_ulNumOfObjInCache << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tDefault Max Period: "  << m_dMaxPeriod << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tDefault Max Unused Period: "  << m_dMaxUnusedPeriod << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tNumber of objects in deletion queue:  " << m_ulNumOfObjInDelQueue << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tNumber of references: " << m_ulTotalNumOfRef << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tHits Ratio: " << m_uiTotalHitsCache << "/" << m_uiTotalAccessCache << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tCache size: " << m_ulSizeCache << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tMaximum cache size: " << m_ulMaxSizeCache << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tMaximum number of objects in cache: " << m_ulMaxNOfObjects << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tRefresh ratio: " << m_ulRefreshRatio << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tNumber of objects in LRU list: " << m_ulNumOfObjInLRU << EndDumpCacheLn;
	ai_oClDumpCacheVisitor << "\t\tNumber UpdateObjects: " << m_ulNumOfUpdateObjs << EndDumpCacheLn;

	return M4_SUCCESS;
  //## end ClCacheStyle::Dump%917950248.body
}

m4return_t ClCacheStyle::SetCacheName (m4pchar_t ai_pcCacheName)
{
  //## begin ClCacheStyle::SetCacheName%943372019.body preserve=yes
	m_pcCacheName = ai_pcCacheName;
	return M4_SUCCESS;
  //## end ClCacheStyle::SetCacheName%943372019.body
}

// Additional Declarations
  //## begin ClCacheStyle%34D5AA5B0218.declarations preserve=yes
  //## end ClCacheStyle%34D5AA5B0218.declarations

// Class ClCacheTree 






ClCacheTree::ClCacheTree (m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod, m4uint32_t ai_ulMaxSizeCache, m4uint32_t ai_ulMaxNOfObjs, m4uint32_t ai_ulRefreshRatio)
  //## begin ClCacheTree::ClCacheTree%886420481.hasinit preserve=no
      : m_poClCacheableObjectFactory(NULL)
  //## end ClCacheTree::ClCacheTree%886420481.hasinit
  //## begin ClCacheTree::ClCacheTree%886420481.initialization preserve=yes
  ,ClCacheStyle(ai_dMaxPeriod,ai_dMaxUnusedPeriod,ai_ulMaxSizeCache,ai_ulMaxNOfObjs,ai_ulRefreshRatio)
  //## end ClCacheTree::ClCacheTree%886420481.initialization
{
  //## begin ClCacheTree::ClCacheTree%886420481.body preserve=yes
  //## end ClCacheTree::ClCacheTree%886420481.body
}


ClCacheTree::~ClCacheTree ()
{
  //## begin ClCacheTree::~ClCacheTree%886420482.body preserve=yes
	DestroyAll();
  //## end ClCacheTree::~ClCacheTree%886420482.body
}



//## Other Operations (implementation)
m4return_t ClCacheTree::Recycle ()
{
  //## begin ClCacheTree::Recycle%886595260.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	// Creo una variable iterator para recorrer el map y otra que recorre el vector	(es un multimapa)
	// y buscar los objetos que están fuera de fecha. Los que lo esten se pasan a la cola de borrado.	
	// Despues se evalua la cola de borrado. Los objetos que estén en ella con su número de referencias	a 0
	// se eliminan definitivamente
		
	CacheableObject2Map::iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2List::iterator IterObjectDeletionQueue,IterObjectDeletionQueueNext;
	CacheableObject2LRUMap::iterator IterLRUMap;
	ClCachePolicy * ao_poPolicy;
	m4uint32_t ulSizeLRUMap;
	M4ClTimeStamp  ai_oActualTime;

	ulSizeLRUMap=LRUDateMap.size();	
	ai_oActualTime.now();
	
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects = IterCacheOfObjectsNext)
	{
		(*IterCacheOfObjects).second->GetPolicy(ao_poPolicy);
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;
	
		if(!(ao_poPolicy->IsValid(ai_oActualTime)))
		{
			//LRU.
			if(!(*IterCacheOfObjects).second->IsReferenced() )//&& (*IterCacheOfObjects).second->Getm_bRam())
			{ 
				// Si no tiene referncias tiene que estar en LRUMap. Lo quitamos.
				M4ClTimeStamp * LRUDate;
				m4uint32_t ulNumberOfLRUSameKey;

				LRUDate=(*IterCacheOfObjects).second->GetLRUDate();
				ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);

				IterLRUMap=LRUDateMap.find(LRUDate);

				//Multimapa. Segunda dimensión (LRU con misma clave).
				for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
				{
					if((*IterCacheOfObjects).second==(*IterLRUMap).second)
					{
						LRUDateMap.erase(IterLRUMap);
						m_ulNumOfObjInLRU--;
						break;
					}

					IterLRUMap++;
				}
			}
			//LRU.

			//Borramos el objeto de la caché.
			ClCacheableObject * ai_pObject;
			ai_pObject=(*IterCacheOfObjects).second;

			if((RemoveObjectNonBlock((*IterCacheOfObjects).second))==M4_ERROR)
			{
				return M4_ERROR;
			}

			if(!CacheOfObjects.size())
				break;				
		}
	}
	
	
	// Con la variable iterator recorro el vector de la cola de borrado y busco los objetos
	// cacheables que tienen la referencia a cero para eliminarlos.
	for(IterObjectDeletionQueue=ObjectDeletionQueue.begin();
		IterObjectDeletionQueue!=ObjectDeletionQueue.end();
		IterObjectDeletionQueue=IterObjectDeletionQueueNext)
	{
		IterObjectDeletionQueueNext=IterObjectDeletionQueue;
		IterObjectDeletionQueueNext++;

		if(!(*IterObjectDeletionQueue)->IsReferenced())
		{	
			ClCacheableObject * DeletingObject;
			DeletingObject=(*IterObjectDeletionQueue);

			if((*IterObjectDeletionQueue)->Getm_bRam()==M4_TRUE)
				m_ulSizeCache-=(*IterObjectDeletionQueue)->GetSize();

			delete DeletingObject;

			ObjectDeletionQueue.erase(IterObjectDeletionQueue);

			--m_ulNumOfObjInDelQueue;
		}
	}

	if(m_ulMaxSizeCache)
	{
		if(SizeControl()==M4_ERROR)
			return M4_ERROR;
	}
	
	return M4_SUCCESS;
  //## end ClCacheTree::Recycle%886595260.body
}

m4return_t ClCacheTree::PutObject (ClCacheableObject *ai_pObject)
{
  //## begin ClCacheTree::PutObject%888398561.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	CacheableObject2Map::iterator IterCacheOfObjects;
	CacheableObject2List::iterator IterObjectDeletionQueue;
	m4bool_t m_bIsInCache;

	if(ai_pObject==NULL)
	{
		return M4_ERROR;	
	}

	if(!ai_pObject->IsReferenced())
	{
		return M4_ERROR;	
	}

 	DecrNumOfRef(ai_pObject);

	if( ai_pObject->IsDisabled() == M4_TRUE )
	{
		m_ulNumOfObjectsDisabled--;
		ai_pObject->Enable();
	}

	ClCacheKey * poCacheKey;
	ai_pObject->GetKey(poCacheKey);

	IterCacheOfObjects=CacheOfObjects.find(poCacheKey);
	m_bIsInCache=M4_FALSE;


	//Hasta aquí hemos actualizado las refs del objeto.
	//Combrobamos si está en la cola de borrado o si tiene que ir a la LRU.

	if(!ai_pObject->IsReferenced() && IterCacheOfObjects!=CacheOfObjects.end())
	{
		//Si no está referenciado y está en la caché. Lo metemos a la cola LRU.
		m4uint32_t ulNumberOfObjsSameKey=CacheOfObjects.count(poCacheKey);

		//DANIEL. Revisar este bucle (se podría eliminar¿? y dejar solo la inserción en LRU.
		for(m4uint32_t Counter=0;Counter<ulNumberOfObjsSameKey;Counter++)
		{
			//LRU.
			if((*IterCacheOfObjects).second==ai_pObject)
			{
				M4ClTimeStamp * poLRUDate;
				poLRUDate=ai_pObject->GetLRUDate();
				if((poLRUDate->now())==M4_ERROR)
				{
					return M4_ERROR;
				}

				ai_pObject->SetLRUDate(poLRUDate);

				LRUDateMap.insert( CacheableObject2LRUMap::value_type(poLRUDate,ai_pObject));

				m_ulNumOfObjInLRU++;
				m_bIsInCache=M4_TRUE;
				break;
			}
			//LRU.

			IterCacheOfObjects++;						
		}
	}

	if( !ai_pObject->IsReferenced() && m_bIsInCache==0)
	{
		// No tiene referencias y esta en la cola de borrado-> lo elemino
		if(ai_pObject->Getm_bRam()==M4_TRUE)
			m_ulSizeCache-=ai_pObject->GetSize();
				
		ObjectDeletionQueue.remove(ai_pObject);
		delete ai_pObject;
		--m_ulNumOfObjInDelQueue;
	}

	return M4_SUCCESS;
  //## end ClCacheTree::PutObject%888398561.body
}

m4return_t ClCacheTree::RemoveObject (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTree::RemoveObject%888398562.body preserve=yes
	ClMutBlock oBlock(m_oMutex);
	return RemoveObjectInternalNonBlock(ai_pObject);
  //## end ClCacheTree::RemoveObject%888398562.body
}

m4return_t ClCacheTree::UpdateObject (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTree::UpdateObject%888398563.body preserve=yes
	ClMutBlock oBlock(m_oMutex);
	return UpdateObjectInternalNonBlock(ai_pObject);
  //## end ClCacheTree::UpdateObject%888398563.body
}

m4return_t ClCacheTree::RemoveObjectNonBlock ( ClCacheableObject *ai_pObject)
{
  //## begin ClCacheTree::RemoveObjectNonBlock%888566517.body preserve=yes
	CacheableObject2Map::iterator IterCacheOfObjects;
	m4uint32_t ulNumOfObjSameKey,Counter;
	ClCacheKey *poCacheKey;

	ai_pObject->GetKey(poCacheKey);

	if(ulNumOfObjSameKey=CacheOfObjects.count(poCacheKey))
	{
		IterCacheOfObjects = CacheOfObjects.find(poCacheKey);

		//Es un multimapa. Tenemos que buscar el objeto concreto.
		for (Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++)
		{
			if(ai_pObject==(*IterCacheOfObjects).second)
			{
				if((*IterCacheOfObjects).second->Getm_bDisk()) 
				{
					//Está en disco. Lo borramos de disco.
					//DANIEL. Que pasa si ulNumOfObjSameKey>1 ¿alguien perdió su imagen en disco?!!
					if(((*IterCacheOfObjects).second->DeleteValueFromDisk())==M4_ERROR)
					{
						//SETCODEF ( M4_OP_CACHE_ERROR , WARNINGLOG , "Cache operation: Cannot delete %s from disk",poCacheKey->Getm_pszKey()) ;
					}
					// Uno menos en disco
					--m_ulNumOfObjInDisk;
				}
					
				CacheOfObjects.erase(IterCacheOfObjects);

				if(ai_pObject->IsReferenced())
				{
					//Está referenciado-> A la cola de borrado.
					ObjectDeletionQueue.push_back(ai_pObject);
					++m_ulNumOfObjInDelQueue;
				}
				else
				{
					//No está referenciado-> Lo eliminamos.
					if(ai_pObject->Getm_bRam()==M4_TRUE)
						m_ulSizeCache-=ai_pObject->GetSize();
					delete ai_pObject;				
				}

				m_ulNumOfObjInCache = CacheOfObjects.size();

				return M4_SUCCESS;
			}
			else
			{
				IterCacheOfObjects++;
			}
		}
	}

	return M4_ERROR;
  //## end ClCacheTree::RemoveObjectNonBlock%888566517.body
}

m4return_t ClCacheTree::GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode, m4int32_t  ai_iTimeOut)
{
  //## begin ClCacheTree::GetObject%892745127.body preserve=yes
    m4bool_t bDoControl = M4_FALSE;
	ClMutBlock oBlock(m_oMutex);

	if(ai_pKey==NULL)
	{
		return M4_ERROR;
	}

	CacheableObject2LRUMap::iterator IterLRUMap;
	m4uint32_t NumElementsCache;

	NumElementsCache=CacheOfObjects.size();
	ao_pObject = Find(ai_pKey,ai_eGetMode);
	
//	if 	(ao_pObject != NULL && ai_eGetMode == GET_EXNBLOCK)
//		ao_pObject->Disable();

	++m_uiTotalAccessCache;

	if(ao_pObject!=NULL)
	{
		ClCachePolicy * poPolicy;
		M4ClTimeStamp ai_oActualTime;
		ai_oActualTime.now();

		ao_pObject->GetPolicy(poPolicy);

		if((poPolicy->IsValid(ai_oActualTime))==M4_FALSE)
		{
			//No es válido.
			if(!ao_pObject->IsReferenced())
			{ 
				//LRU. Lo quitamos de la LRU.
				m4uint32_t ulNumberOfLRUSameKey;
				M4ClTimeStamp * LRUDate;

				LRUDate=ao_pObject->GetLRUDate();
				ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);

				IterLRUMap=LRUDateMap.find(LRUDate);

				for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
				{	
					if(ao_pObject==(*IterLRUMap).second)
					{		
						LRUDateMap.erase(IterLRUMap);
						m_ulNumOfObjInLRU--;
						break;
					}

					IterLRUMap++;					
				}
			}

			//Lo paso a la cola de borrado
			RemoveObjectNonBlock(ao_pObject);

			ao_pObject=NULL;

			return M4_ERROR;
		}

		if(!ao_pObject->IsReferenced())
		{
			//Si es válido y no está referenciado. Lo quitamos de la LRU.
			CacheableObject2LRUMap::iterator IterLRUDateMap2;
			M4ClTimeStamp * LRUDate;
			m4uint32_t ulNumberOfLRUSameKey;

			LRUDate=ao_pObject->GetLRUDate();
			ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);

			IterLRUMap=LRUDateMap.find(LRUDate);

			for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
			{
				if(ao_pObject==(*IterLRUMap).second)
				{	
					LRUDateMap.erase(IterLRUMap);
					m_ulNumOfObjInLRU--;
					break;
				}

				IterLRUMap++;
			}
		}

		if ( !ao_pObject->Getm_bRam())
		{
			//No está en RAM, lo cargamos de disco y aumentamos el Tamaño en RAM de la caché.
			if((ao_pObject->LoadValueFromDisk())==M4_ERROR)
			{
				CacheableObject2Map::iterator IterCacheOfObjects;
				ClCacheKey *poCacheKey;

				ao_pObject->GetKey(poCacheKey);
				IterCacheOfObjects=CacheOfObjects.find(poCacheKey);

				if(ao_pObject!=(*IterCacheOfObjects).second)
				{
					//Es un problema: CacheOfObjectses un multimapa y find nos da el primero con la clave q buscamos, 
					//no necesariamente el mismo objeto.
					//DANIEL. Arreglar esto navegando por el multimapa.
					return M4_ERROR;
				}

				CacheOfObjects.erase(IterCacheOfObjects);
				m_ulNumOfObjInCache = CacheOfObjects.size();

				delete ao_pObject;
				ao_pObject=NULL;

				return M4_ERROR;
			}

			m4uint32_t ulSizeObject;

			ulSizeObject= ao_pObject->GetSize();
			m_ulSizeCache += ulSizeObject;

            bDoControl = M4_TRUE;
			
		}
				
        IncrNumOfRef(ao_pObject);

        // bugid 0101514.
        // El chequeo de memoria se hace después del incremento de referencias.
        if (M4_TRUE == bDoControl)
        {
            //Comprobamos el límite de RAM.
			if(m_ulMaxSizeCache)
			{
				if(SizeControl()==M4_ERROR)
                {
                    DecrNumOfRef(ao_pObject);
					return M4_ERROR;
                }
			}
            
	        // bugid 0102949.
            // comprobamos el número de objetos.
            if( m_ulMaxNOfObjects )
	        {
		        if(NumOfObjectsControl()==M4_ERROR)
                {
                    DecrNumOfRef(ao_pObject);
			        return M4_ERROR;
                }
	        }
        }

		++m_uiTotalHitsCache;

		if(ai_eGetMode == GET_EXNBLOCK)
		{
			ao_pObject->Disable();
			m_ulNumOfObjectsDisabled++;
		}

		return M4_SUCCESS;	
	}

	return	M4_ERROR;
  //## end ClCacheTree::GetObject%892745127.body
}

m4return_t ClCacheTree::UpdateObjectKey (ClCacheableObject *ai_poClCacheableObject, ClCacheKey *ai_poClCacheKey)
{
  //## begin ClCacheTree::UpdateObjectKey%892745130.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	if(ai_poClCacheableObject==NULL || ai_poClCacheKey==NULL)
	{
		return M4_ERROR;
	}

	CacheableObject2Map::iterator IterCacheOfObjects;
	ClCacheKey *poOldCacheKey;

	ai_poClCacheableObject->GetKey(poOldCacheKey);
	IterCacheOfObjects=CacheOfObjects.find(poOldCacheKey);
	CacheOfObjects.erase(IterCacheOfObjects);

	// sets the key new key
	ai_poClCacheableObject->SetKey(ai_poClCacheKey);

	// puts the data in the cache
	CacheOfObjects.insert(CacheableObject2Map::value_type(ai_poClCacheKey,ai_poClCacheableObject));
	
	return M4_SUCCESS;
  //## end ClCacheTree::UpdateObjectKey%892745130.body
}

ClCacheableObject * ClCacheTree::Find (ClCacheKey *ai_poClCacheKey, eGetMode ai_eGetMode)
{
  //## begin ClCacheTree::Find%892796789.body preserve=yes
	CacheableObject2Map::const_iterator IterCacheOfObjects;
	m4uint32_t ulNumOfObjSameKey;

	IterCacheOfObjects = CacheOfObjects.find(ai_poClCacheKey);
	if (IterCacheOfObjects == CacheOfObjects.end())
		return NULL;
	
	// searches the list
#ifdef _DEBUG
	ClCacheKeyLess Compare;
#endif

	ulNumOfObjSameKey=CacheOfObjects.count(ai_poClCacheKey);
	
	for (m4uint32_t Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++)
	{
		// checks the disable flag
		ClCacheableObject *poClCacheableObject = (*IterCacheOfObjects).second;
		
		// Si no está deshabilitado, o el modo es SHARED, o el modo es EXCLUSIVO NO BLOQUENTAE y no está referenciado, devuelve el objeto.
		if (poClCacheableObject->IsDisabled() == M4_FALSE
			&& (ai_eGetMode == GET_SHARED || 
				(ai_eGetMode == GET_EXNBLOCK && poClCacheableObject->IsReferenced() == M4_FALSE)))
			return poClCacheableObject;	
	
		// tests STL algorithm
		M4_ASSERT(Compare(ai_poClCacheKey,(*IterCacheOfObjects).first) == M4_FALSE
			&& Compare((*IterCacheOfObjects).first,ai_poClCacheKey) == M4_FALSE);

		// checks for the next same key 
		IterCacheOfObjects++;
	}

	return NULL;
  //## end ClCacheTree::Find%892796789.body
}

m4return_t ClCacheTree::SetCOFactory (ClCacheableObjectFactory  *  ai_poClCOFactory)
{
  //## begin ClCacheTree::SetCOFactory%893688334.body preserve=yes
	m_poClCacheableObjectFactory=ai_poClCOFactory;
	return M4_SUCCESS;
  //## end ClCacheTree::SetCOFactory%893688334.body
}

ClCacheableObjectFactory *  ClCacheTree::GetCOFactory ()
{
  //## begin ClCacheTree::GetCOFactory%893688335.body preserve=yes
	return m_poClCacheableObjectFactory;
  //## end ClCacheTree::GetCOFactory%893688335.body
}

m4return_t ClCacheTree::Serialize (ClGenericIODriver &  IOD)
{
  //## begin ClCacheTree::Serialize%893751388.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	m4return_t returnIOD;
	m4bool_t bExists=M4_FALSE;
	CacheableObject2Map::const_iterator IterCacheOfObjects;

	//Serializo el magic number.
	returnIOD=	IOD.Write((m4uint32_t)M4_CH_MAGIC_NUMBER_CAC);

	if(returnIOD==M4_ERROR)
	{		
		return M4_ERROR;
	}

	//Serializo el numero de version.
	returnIOD=	IOD.Write((m4uint16_t)M4_CACHE_VER);

	if(returnIOD==M4_ERROR)
	{		
		return M4_ERROR;
	}

	//Serializo los atributos de ClCacheStyle.
	returnIOD=ClCacheStyle::Serialize(IOD);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//*******Serializo la cache y la cola de borrado**********

	//Serializo todos los objetos menos los que están deshabilitados.
	//DANIEL. ¿Correcto?
	m4uint32_t ulSizeCache, ulNumOfObjsToSerialize;
	ulSizeCache = CacheOfObjects.size();
	ulNumOfObjsToSerialize = ulSizeCache - m_ulNumOfObjectsDisabled;
	
	//Numero de objetos serializados.
	returnIOD= IOD.Write( ulNumOfObjsToSerialize );

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	ClCacheableObject *poClCacheableObject = 0;

	//Recorro el mapa para serializar los objetos cacheados
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects++)
	{
		if(m_poClCacheableObjectFactory==NULL) 
		{
			return M4_ERROR;
		}

		poClCacheableObject = (*IterCacheOfObjects).second;

		if( poClCacheableObject->IsDisabled() == M4_FALSE ) {
			if ( poClCacheableObject->Getm_bDisk() == M4_FALSE) {
				// Uno mas a disco
				m_ulNumOfObjInDisk++;
			}
			m_poClCacheableObjectFactory->SerializeObject( poClCacheableObject, &IOD, 0 );
		}
	}

	return M4_SUCCESS;
  //## end ClCacheTree::Serialize%893751388.body
}

m4return_t ClCacheTree::DeSerialize (ClGenericIODriver &  IOD, m4pchar_t ai_pcEnvId )
{
  //## begin ClCacheTree::DeSerialize%893751389.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	m4return_t returnIOD=M4_SUCCESS;
	m4bool_t bExists=M4_FALSE;
	m4uint32_t CacheSize,Counter;
	m4uint16_t CacheVersion;
	m4uint32_t iMagicNumber;
	ClCacheableObject * pCacheableObject;

	//Deserializo el magic number
	returnIOD=IOD.Read(iMagicNumber);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	if(iMagicNumber!=M4_CH_MAGIC_NUMBER_CAC)
	{
		return M4_ERROR;
	}

	//Deserializo el numero de version
	returnIOD=IOD.Read(CacheVersion);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	if(CacheVersion!=M4_CACHE_VER)
	{
		return M4_ERROR;
	}

	//Deserializo ClCacheStyle
	returnIOD=ClCacheStyle::DeSerialize(IOD, ai_pcEnvId);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	//***** DeSerializacion de ClCacheTree ( Cache y cola de borrado)***
	//Numero de objetos serializados.
	returnIOD=IOD.Read(CacheSize);

	if(returnIOD==M4_ERROR)
	{
		return M4_ERROR;
	}

	for (Counter=0;Counter<CacheSize;Counter++)
	{
		//DANIEL. Sacar esta condición fuera del bucle!!
		if(m_poClCacheableObjectFactory==NULL) 
		{
			m_ulNumOfObjInCache = 0;
			return M4_ERROR;
		}

		pCacheableObject=0;
				
		if(m_poClCacheableObjectFactory->DeSerializeObject(pCacheableObject,&IOD,0)==M4_ERROR)
		{
			m_ulNumOfObjInCache=CacheOfObjects.size();				
			return M4_ERROR;
		}

		if( ai_pcEnvId != NULL )
		{
			pCacheableObject->SetEnvId( ai_pcEnvId );
		}

		ClCacheKey * Key;
		M4ClTimeStamp * poLRUDate;
		m4time_t NumSeconds;

		poLRUDate=pCacheableObject->GetLRUDate();
		poLRUDate->get(NumSeconds);
		
		if(!NumSeconds)
		{
			if((poLRUDate->now())==M4_ERROR)
			{
				return M4_ERROR;
			}

			pCacheableObject->SetLRUDate(poLRUDate);
		}

		pCacheableObject->GetKey(Key);

		//Insertamos el objeto en la caché y el la LRU (no tiene referencias).
		CacheOfObjects.insert(CacheableObject2Map::value_type(Key, pCacheableObject));
		LRUDateMap.insert( CacheableObject2LRUMap::value_type(poLRUDate,pCacheableObject));

		if ( pCacheableObject->Getm_bDisk() == M4_TRUE) {
			// Uno mas que esta en disco
			m_ulNumOfObjInDisk++;
		}
		m_ulNumOfObjInLRU++;
	}

	return M4_SUCCESS;
  //## end ClCacheTree::DeSerialize%893751389.body
}

m4return_t  ClCacheTree::RemoveAll ()
{
  //## begin ClCacheTree::RemoveAll%894362621.body preserve=yes
	//Borra todos los elementos de la caché (o los pasa a la cola de borrado).
	ClMutBlock oBlock(m_oMutex);

	CacheableObject2Map::iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2LRUMap::iterator IterLRUMapBegin,IterLRUMapEnd;
	ClCacheableObject * po_Object;
	m4uint32_t ulNumOfElemInCache,Counter;

	IterCacheOfObjects=CacheOfObjects.begin();
	ulNumOfElemInCache=CacheOfObjects.size();

	for (Counter = 0 ; Counter < ulNumOfElemInCache ; Counter++)
	{
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;
		po_Object=(*IterCacheOfObjects).second;

		if(po_Object->Getm_bDisk())
		{
			if(po_Object->DeleteValueFromDisk()==M4_ERROR)
			{
				/*ClCacheKey * poKey;
				po_Object->GetKey(poKey);
				SETCODEF ( M4_OP_CACHE_ERROR , WARNINGLOG , "Cache operation: Cannot delete %s from disk",poKey->Getm_pszKey()) ;*/
			}
			// Uno menos en disco
			--m_ulNumOfObjInDisk;
		}
					
		CacheOfObjects.erase(IterCacheOfObjects);

		if(po_Object->IsReferenced())
		{
			//Está referenciado->A la cola de borrado.
			ObjectDeletionQueue.push_back(po_Object);
			++m_ulNumOfObjInDelQueue;
		}
		else
		{
			//No está referenciado->Lo destruimos.
			if(po_Object->Getm_bRam()==M4_TRUE)
			{
				m_ulSizeCache-=po_Object->GetSize();
			}

			delete po_Object;
		}

		IterCacheOfObjects=IterCacheOfObjectsNext;

		//DANIEL.  Esto sobraría, basta con hacerlo después del bucle.
		m_ulNumOfObjInCache = CacheOfObjects.size();
	}

	//Borramos de la LRU.
	IterLRUMapBegin=LRUDateMap.begin();
	IterLRUMapEnd=LRUDateMap.end();
	
	LRUDateMap.erase(IterLRUMapBegin,IterLRUMapEnd);

	m_ulNumOfObjInLRU=0;

	return M4_SUCCESS;
  //## end ClCacheTree::RemoveAll%894362621.body
}

m4return_t  ClCacheTree::NoUpdateObject (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTree::NoUpdateObject%894362624.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheTree::NoUpdateObject%894362624.body
}

m4return_t ClCacheTree::DestroyAll ()
{
  //## begin ClCacheTree::DestroyAll%895764957.body preserve=yes
	//No hay semáforo (ojo), solo se debe llamar serializado (1 sólo thread).
	//Borra todos los elementos de la caché y de la cola de borrado.
	CacheableObject2Map::iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2List::iterator IterObjectDeletionQueue,IterObjectDeletionQueueNext;

	//Borramos todos los objetos de la caché.
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects = IterCacheOfObjectsNext)
	{
		ClCacheableObject * pObject;
		m4uint32_t CacheSize=CacheOfObjects.size();	
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;

		pObject=(*IterCacheOfObjects).second;

		if (M4_FALSE == pObject->IsReferenced())
		{
			delete (*IterCacheOfObjects).second;
		}
		else
		{
			//Está referenciado. No podemos destruirlo... quitamos el objeto simplemente.
			ClCacheKey * pObjectKey;
			pObject->GetKey(pObjectKey);
			pObject->NotifyCacheDestroy();
			g_oChLog<<BeginWarning(M4_CACHE_DESTROY_REFER_OBJECT)<<pObjectKey->Getm_pszKey()<<EndLog;			
		}

		CacheOfObjects.erase(IterCacheOfObjects);
	}	
	
	//Borramos todos los objetos de la cola de borrado.
	for(IterObjectDeletionQueue=ObjectDeletionQueue.begin();
		IterObjectDeletionQueue!=ObjectDeletionQueue.end();
		IterObjectDeletionQueue=IterObjectDeletionQueueNext)
	{
		ClCacheableObject * pObject;
		IterObjectDeletionQueueNext=IterObjectDeletionQueue;
		IterObjectDeletionQueueNext++;
		pObject=(*IterObjectDeletionQueue);

		if (M4_FALSE == pObject->IsReferenced())
		{		
			delete (*IterObjectDeletionQueue);
		}
		else
		{
			//Está referenciado. No podemos destruirlo... quitamos el objeto simplemente.
			ClCacheKey * pObjectKey;
			pObject->GetKey(pObjectKey);
			g_oChLog<<BeginWarning(M4_CACHE_DESTROY_REFER_OBJECT)<<pObjectKey->Getm_pszKey()<<EndLog;			
		}
			
		ObjectDeletionQueue.erase(IterObjectDeletionQueue);
	}
	
	m_ulNumOfObjInCache=CacheOfObjects.size();
	m_ulNumOfObjInDelQueue=0;

	//DANIEL. ¿La LRU queda inconsistente? Revisar.

	return M4_SUCCESS;
  //## end ClCacheTree::DestroyAll%895764957.body
}

m4return_t ClCacheTree::RemoveAll (M4ClTimeStamp *  ai_poLimitDate)
{
  //## begin ClCacheTree::RemoveAll%895824027.body preserve=yes
	//Borra todos los elementos de la caché (o los pasa a la cola de borrado) cuya fecha de inserción sea más vieja que ai_poLimitDate.

	//Si la fecha es NULL, se borran todos.
	if (ai_poLimitDate==NULL) 
		return RemoveAll();

	ClMutBlock oBlock(m_oMutex);
	
	CacheableObject2Map::const_iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2LRUMap::iterator IterLRUMap;
	m4uint32_t NumOfObjsIncache=CacheOfObjects.size();	
		
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects = IterCacheOfObjectsNext)
	{
		M4ClTimeStamp  InsertionDate;
		
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;
		InsertionDate=(*IterCacheOfObjects).second->GetInsertionDate();
	
		if((* ai_poLimitDate)>InsertionDate)
		{
			//Borramos el objeto.
			if(!(*IterCacheOfObjects).second->IsReferenced() )//&& (*IterCacheOfObjects).second->Getm_bRam())
			{ 
				// Si no tiene referncias tiene que estar en LRUMap 
				m4uint32_t ulNumberOfLRUSameKey;
				M4ClTimeStamp * LRUDate;
				LRUDate=(*IterCacheOfObjects).second->GetLRUDate();
				ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);
				IterLRUMap=LRUDateMap.find(LRUDate);

				for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
				{
					if((*IterCacheOfObjects).second==(*IterLRUMap).second)
					{
						LRUDateMap.erase(IterLRUMap);
						m_ulNumOfObjInLRU--;
						break;
					}

					IterLRUMap++;
				}
			}

			if((RemoveObjectNonBlock((*IterCacheOfObjects).second))==M4_ERROR)
			{
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
  //## end ClCacheTree::RemoveAll%895824027.body
}

m4return_t ClCacheTree::RemoveAll (m4pchar_t ai_pcPortionKey, m4puint32_t ai_piRemovedCount)
{
  //## begin ClCacheTree::RemoveAll%895824028.body preserve=yes
	//Borra todos los elementos de la caché (o los pasa a la cola de borrado) cuya clave comience por ai_pcPortionKey.
	//Si ai_piRemovedCount!=0 devuelve el número de objetos borrados.
	if (ai_piRemovedCount)
	{
		*ai_piRemovedCount = 0;
	}

	//Si la ai_pcPortionKey es NULL, se borran todos.
	if (ai_pcPortionKey==NULL)
	{
		if (ai_piRemovedCount)
		{
			*ai_piRemovedCount = CacheOfObjects.size();
		}
		return RemoveAll();
	}

	ClMutBlock oBlock(m_oMutex);
	
	CacheableObject2Map::const_iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2LRUMap::iterator IterLRUMap;
	m4uint32_t NumOfObjsIncache=CacheOfObjects.size();	
		
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects = IterCacheOfObjectsNext)
	{
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;
		ClCacheKey * pObjectKey;
		(*IterCacheOfObjects).second->GetKey(pObjectKey);

		//Se cumple si ai_pcPortionKey se encuentra al principio de pObjectKey->Getm_pszKey()!!
		if(strstr(pObjectKey->Getm_pszKey(),ai_pcPortionKey) == pObjectKey->Getm_pszKey())		
		{
			//Lo hemos encontrado.
			if (ai_piRemovedCount)
			{
				(*ai_piRemovedCount)++;
			}

			if(!(*IterCacheOfObjects).second->IsReferenced() )//&& (*IterCacheOfObjects).second->Getm_bRam())
			{ 
				// Si no tiene referncias tiene que estar en LRUMap 
				m4uint32_t ulNumberOfLRUSameKey;
				M4ClTimeStamp * LRUDate;
				LRUDate=(*IterCacheOfObjects).second->GetLRUDate();
				ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);
				IterLRUMap=LRUDateMap.find(LRUDate);

				for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
				{
					if((*IterCacheOfObjects).second==(*IterLRUMap).second)
					{
						LRUDateMap.erase(IterLRUMap);
						m_ulNumOfObjInLRU--;
						break;
					}

					IterLRUMap++;
				}
			}

			if((RemoveObjectNonBlock((*IterCacheOfObjects).second))==M4_ERROR)
			{
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
  //## end ClCacheTree::RemoveAll%895824028.body
}

m4return_t ClCacheTree::Refresh (ClSelfPersistiableObject *   ai_poSelfPersistor)
{
  //## begin ClCacheTree::Refresh%897292450.body preserve=yes
	m4bool_t bDoRecycle = M4_FALSE;

	{
		//Zona del Mutex
	 	ClMutBlock oBlock(m_oMutex);
		m_ulNumOfUpdateObjs++;

		if(m_ulNumOfUpdateObjs>m_ulRefreshRatio)
		{
			m_ulNumOfUpdateObjs=0;
			bDoRecycle = M4_TRUE;
		}
	}

	if(bDoRecycle)
	{
		Recycle();
		ai_poSelfPersistor->Persist();
	}

	return M4_SUCCESS;	
  //## end ClCacheTree::Refresh%897292450.body
}

m4return_t ClCacheTree::GetNext (ClCacheableObject *  &  ao_poClCacheableObject, void * &  ai_poIterator)
{
  //## begin ClCacheTree::GetNext%911234942.body preserve=yes

	// IMPORTANT: the user has to finish the cycle or the cache will keep blocked
	// this will be optimized in next release

	CacheableObject2Map::iterator * IterCacheOfObjects1= NULL;

	if (ao_poClCacheableObject==NULL)
	{
		// locks the current cache object
		// m_oMutex.Lock();

		if (CacheOfObjects.begin() == CacheOfObjects.end())
		{
			// m_oMutex.Unlock();
			return M4_SUCCESS;
		}

		IterCacheOfObjects1= new CacheableObject2Map::iterator;
		(*IterCacheOfObjects1)=CacheOfObjects.begin();
	}
	else 
	{
		IterCacheOfObjects1=(CacheableObject2Map::iterator *) ai_poIterator;

		if((*IterCacheOfObjects1)==CacheOfObjects.end())
		{
			// ok the user reached the end of the chain and we release the mutex
			ao_poClCacheableObject=NULL;
			delete IterCacheOfObjects1;
			// m_oMutex.Unlock();
			return M4_SUCCESS;
		}
		
		(*IterCacheOfObjects1)--;

		ClCacheableObject * ai_pObject=(*(*IterCacheOfObjects1)).second;

		(*IterCacheOfObjects1)++;
	}

	ai_poIterator=(void *)IterCacheOfObjects1;
	ao_poClCacheableObject=(*(*IterCacheOfObjects1)).second;

	++m_uiTotalAccessCache;
	++m_uiTotalHitsCache;

	(*IterCacheOfObjects1)++;

	// lets the cache blocked
	return M4_SUCCESS;
  //## end ClCacheTree::GetNext%911234942.body
}

m4return_t ClCacheTree::Dump (ClDumpCacheVisitor &  ai_pClDumpCacheVisitor)
{
  //## begin ClCacheTree::Dump%917950249.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	m4bool_t bExists=M4_FALSE;
	CacheableObject2Map::const_iterator IterCacheOfObjects;

	ai_pClDumpCacheVisitor << "\tCache Version: "  << (m4uint16_t)M4_CACHE_VER << EndDumpCacheLn;

	if(ai_pClDumpCacheVisitor.DumpStatistics()==M4_TRUE)
	{
		ClCacheStyle::Dump(ai_pClDumpCacheVisitor);
	}

	m4uint32_t SizeCache;
	SizeCache=CacheOfObjects.size();
	ai_pClDumpCacheVisitor << EndDumpCacheLn << "\tNumber of objects in cache: "  << SizeCache << EndDumpCacheLn;

		
	//Recorro el mapa para volcar los objetos cacheados
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects++)
	{
		ai_pClDumpCacheVisitor << EndDumpCacheLn;
		(*IterCacheOfObjects).second->Dump(ai_pClDumpCacheVisitor);
	}

	//Recorro la cola de borrado
	if(ai_pClDumpCacheVisitor.DumpDelQueue()==M4_TRUE)
	{
		CacheableObject2List::iterator IterObjectDeletionQueue,IterObjectDeletionQueueNext;
		m4uint32_t SizeCache;

		SizeCache=ObjectDeletionQueue.size();
		ai_pClDumpCacheVisitor << EndDumpCacheLn << "\tNumber of objects in deletion queue: "  << SizeCache << EndDumpCacheLn;

		for(IterObjectDeletionQueue=ObjectDeletionQueue.begin();
			IterObjectDeletionQueue!=ObjectDeletionQueue.end();
			IterObjectDeletionQueue++)
		{
			ai_pClDumpCacheVisitor << EndDumpCacheLn;
			(*IterObjectDeletionQueue)->Dump(ai_pClDumpCacheVisitor);
		}
	}

	return M4_SUCCESS;
  //## end ClCacheTree::Dump%917950249.body
}

m4return_t ClCacheTree::SizeControl ()
{
  //## begin ClCacheTree::SizeControl%943372020.body preserve=yes
	//Se comprueba el tamaño de la cache en RAM y si se sobrepasa el límite, 
	//se intentan guardar a disco los objetos no referenciados (LRU)
	CacheableObject2LRUMap::iterator IterLRUMap,IterLRUMapNext;

	if( m_ulSizeCache > m_ulMaxSizeCache )
	{
		for(IterLRUMap=LRUDateMap.begin();
			IterLRUMap!=LRUDateMap.end();
			IterLRUMap = IterLRUMapNext)
		{
		
			IterLRUMapNext=IterLRUMap;
			IterLRUMapNext++;

			if((*IterLRUMap).second->Getm_bRam()==M4_TRUE)
			{
				m4bool_t bDisk = (*IterLRUMap).second->Getm_bDisk();

				if (bDisk == M4_FALSE)
				{
					(*IterLRUMap).second->SaveValueToDisk();
					bDisk = (*IterLRUMap).second->Getm_bDisk();
					// Uno mas en disco
					++m_ulNumOfObjInDisk;
				}

				if (bDisk == M4_TRUE)
				{		
					if(((*IterLRUMap).second->DeleteValueFromRam())==M4_ERROR)
					{
						return M4_ERROR;
					}

					m4uint32_t ulSizeObject=(*IterLRUMap).second->GetSize();
					m_ulSizeCache -= ulSizeObject;
				}
			}

			//Cuando alcanzamos el valor m_ulMaxSizeCache-m_ulDelRatioMem ya no hace falta seguir borrando de disco.
			if ((m_ulSizeCache <=(m_ulMaxSizeCache-m_ulDelRatioMem))|| !LRUDateMap.size())
				break;
		}

		if( m_ulMaxSizeCache && m_ulSizeCache > m_ulMaxSizeCache )
		{
			g_oChLog<<BeginWarning(M4_CACHE_OVER_SIZE_LIMIT)<<EndLog;			
		}
	}	

	return M4_SUCCESS;
  //## end ClCacheTree::SizeControl%943372020.body
}

m4return_t ClCacheTree::NumOfObjectsControl ()
{
  //## begin ClCacheTree::NumOfObjectsControl%943372021.body preserve=yes
	//Se comprueba el número de objetos en caché y si se sobrepasa el límite, 
	//se intentan borrar los objetos no referenciados (LRU)
	CacheableObject2LRUMap::iterator IterLRUMap,IterLRUMapNext;

	if( m_ulNumOfObjInCache > m_ulMaxNOfObjects )
	{
		for(IterLRUMap=LRUDateMap.begin();
			IterLRUMap!=LRUDateMap.end();
			IterLRUMap = IterLRUMapNext)
		{
			IterLRUMapNext=IterLRUMap;
			IterLRUMapNext++;
	
			ClCacheableObject * pObject;
			pObject=(*IterLRUMap).second;

			if((RemoveObjectNonBlock(pObject))==M4_ERROR)
			{
				return M4_ERROR;
			}

			LRUDateMap.erase(IterLRUMap);
			m_ulNumOfObjInLRU--;

			//Cuando alcanzamos el valor m_ulMaxNOfObjects-m_ulDelRatioObjs ya no hace falta seguir borrando de disco.
			if((m_ulNumOfObjInCache <= (m_ulMaxNOfObjects - m_ulDelRatioObjs))|| !LRUDateMap.size())
				break;
		}
	}	

	return M4_SUCCESS;
  //## end ClCacheTree::NumOfObjectsControl%943372021.body
}

m4return_t ClCacheTree::RemoveByCondition (void *  ai_pCondition)
{
  //## begin ClCacheTree::RemoveByCondition%943372024.body preserve=yes
	//Borra lso objetos cuyo método MatchCondition(ai_pCondition) devuelva TRUE.
	ClMutBlock oBlock(m_oMutex);
	
	CacheableObject2Map::const_iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2LRUMap::iterator IterLRUMap;

	m4uint32_t NumOfObjsIncache=CacheOfObjects.size();	
		
	for(IterCacheOfObjects=CacheOfObjects.begin();
		IterCacheOfObjects!=CacheOfObjects.end();
		IterCacheOfObjects = IterCacheOfObjectsNext)
	{
		IterCacheOfObjectsNext=IterCacheOfObjects;
		IterCacheOfObjectsNext++;

		//Borra los que cumplen la condicion
		if(M4_TRUE == (*IterCacheOfObjects).second->MatchCondition(ai_pCondition))		
		{
			if(!(*IterCacheOfObjects).second->IsReferenced() )
			{ 
				// Si no tiene referncias tiene que estar en LRUMap 
				M4ClTimeStamp * LRUDate;
				LRUDate=(*IterCacheOfObjects).second->GetLRUDate();
				m4uint32_t ulNumberOfLRUSameKey;
				ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);
				IterLRUMap=LRUDateMap.find(LRUDate);

				for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
				{
					if((*IterCacheOfObjects).second==(*IterLRUMap).second)
					{
						LRUDateMap.erase(IterLRUMap);
						m_ulNumOfObjInLRU--;
						break;
					}

					IterLRUMap++;
				}
			}

			if((RemoveObjectNonBlock((*IterCacheOfObjects).second))==M4_ERROR)
			{
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
  //## end ClCacheTree::RemoveByCondition%943372024.body
}

m4return_t ClCacheTree::ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject)
{
  //## begin ClCacheTree::ReplaceObject%945346494.body preserve=yes
	ClMutBlock oBlock(m_oMutex);

	m4return_t ret;

	ret = RemoveObjectInternalNonBlock(ai_pOldObject);
	
	ret = UpdateObjectInternalNonBlock(ai_pNewObject);

	return ret;
  //## end ClCacheTree::ReplaceObject%945346494.body
}

m4return_t ClCacheTree::RemoveObjectInternalNonBlock (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTree::RemoveObjectInternalNonBlock%945346495.body preserve=yes
	CacheableObject2Map::iterator IterCacheOfObjects,IterCacheOfObjectsNext;
	CacheableObject2LRUMap::iterator IterLRUMap;

	if(ai_pObject==NULL)
	{
		return M4_ERROR;
	}

	m4uint32_t ulNumOfObjSameKey,Counter1;
	ClCacheKey * poKey;
	ai_pObject->GetKey(poKey);

	if(ulNumOfObjSameKey=CacheOfObjects.count(poKey))
	{
		IterCacheOfObjects = CacheOfObjects.find(poKey);
		ClCacheableObject *po_ClCacheableObject;
					
		for (Counter1=0 ; Counter1 < ulNumOfObjSameKey ; Counter1++)
		{
			po_ClCacheableObject = (*IterCacheOfObjects).second;

			if (po_ClCacheableObject==ai_pObject)
			{
				if(!(*IterCacheOfObjects).second->IsReferenced() && (*IterCacheOfObjects).second->Getm_bRam())
				{ 
					// Si no tiene referencias y esta en RAM tiene que estar en LRUMap 
					m4uint32_t ulNumberOfLRUSameKey;
					M4ClTimeStamp * LRUDate;

					LRUDate=(*IterCacheOfObjects).second->GetLRUDate();
					ulNumberOfLRUSameKey=LRUDateMap.count(LRUDate);

					IterLRUMap=LRUDateMap.find(LRUDate);

					for(m4uint32_t Counter=0;Counter<ulNumberOfLRUSameKey;Counter++)
					{
						if((*IterCacheOfObjects).second==(*IterLRUMap).second)
						{
							LRUDateMap.erase(IterLRUMap);
							m_ulNumOfObjInLRU--;
							break;
						}

						IterLRUMap++;
					}
				}

				if((*IterCacheOfObjects).second->Getm_bDisk()) 
				{
					if((*IterCacheOfObjects).second->DeleteValueFromDisk()==M4_ERROR)
					{
						//SETCODEF ( M4_OP_CACHE_ERROR , WARNINGLOG , "Cache operation: Cannot delete %s from disk",poKey->Getm_pszKey());
					}
					// Uno menos en disco
					--m_ulNumOfObjInDisk;
				}
						
				CacheOfObjects.erase(IterCacheOfObjects);

				m_ulNumOfObjInCache=CacheOfObjects.size();

				if(ai_pObject->IsReferenced())
				{
					//Está referenciado->A la cola de borrado.
					ObjectDeletionQueue.push_back(ai_pObject);
					++m_ulNumOfObjInDelQueue;
				}
				else
				{
					//No está referenciado->lo destruimos.
					if(ai_pObject->Getm_bRam()==M4_TRUE)
					{
						m_ulSizeCache-=ai_pObject->GetSize();
					}

					delete ai_pObject;
				}

				return M4_SUCCESS;
			}

			IterCacheOfObjects++;
		}
	}
	
	return M4_ERROR;
  //## end ClCacheTree::RemoveObjectInternalNonBlock%945346495.body
}

m4return_t ClCacheTree::UpdateObjectInternalNonBlock (ClCacheableObject *  ai_pObject)
{
  //## begin ClCacheTree::UpdateObjectInternalNonBlock%945346496.body preserve=yes
	if(ai_pObject==NULL)
	{
		return M4_ERROR;
	}

	m4uint32_t ulSizeObject;

	if (ai_pObject -> IsReferenced())
	{
		return M4_SUCCESS;
	}
	
	ClCachePolicy *ao_poPolicy;
	
	ai_pObject->GetPolicy(ao_poPolicy);
	
	if(ao_poPolicy==NULL)
	{
		//Le ponemos política (la de defecto).
		ClCachePolicy * poDefaultPolicy = new ClCachePolicy(GetMaxPeriod(), GetMaxUnusedPeriod());
		ai_pObject->SetPolicy(poDefaultPolicy);
	}

	m4bool_t bRam;
	ClCacheKey *Key;
	
	ai_pObject->GetKey(Key);
	bRam=ai_pObject->Getm_bRam();

	if(bRam==M4_TRUE)
	{
		ulSizeObject=ai_pObject->GetSize();
		m_ulSizeCache+=ulSizeObject;
	}

	//Le añado la fecha de inserción al clcacheableobject
	M4ClTimeStamp  InsertionDate;

	InsertionDate=ai_pObject->GetInsertionDate();

	if((InsertionDate.now())==M4_ERROR)
	{
		return M4_ERROR;
	}

	ai_pObject->SetInsertionDate(InsertionDate);

	//Control de nº de elementos
	if( m_ulMaxNOfObjects )
	{
		if(NumOfObjectsControl()==M4_ERROR)
			return M4_ERROR;
	}

	//Control de tamaño en RAM
	if(bRam==M4_TRUE)
	{
		if(m_ulMaxSizeCache)
		{
			if(SizeControl()==M4_ERROR)
				return M4_ERROR;
		}
	}

	//Insertamos el objeto en la caché.
	CacheOfObjects.insert(CacheableObject2Map::value_type(Key, ai_pObject));

	m_ulNumOfObjInCache=CacheOfObjects.size();

	IncrNumOfRef(ai_pObject);
	
	return M4_SUCCESS;
  //## end ClCacheTree::UpdateObjectInternalNonBlock%945346496.body
}

// Additional Declarations
  //## begin ClCacheTree%34D5ABD4018E.declarations preserve=yes
  //## end ClCacheTree%34D5ABD4018E.declarations

// Class ClCacheKeyLess 


//## Other Operations (implementation)
m4bool_t ClCacheKeyLess::operator ( ) (const ClCacheKey *ai_pKeyA, const ClCacheKey *  ai_pKeyB) const
{
  //## begin ClCacheKeyLess::operator ( )%888398564.body preserve=yes
	return strcmp(ai_pKeyA->m_pszKey,ai_pKeyB->m_pszKey)<0 ? M4_TRUE:M4_FALSE;
  //## end ClCacheKeyLess::operator ( )%888398564.body
}

// Additional Declarations
  //## begin ClCacheKeyLess%34E03F430191.declarations preserve=yes
  //## end ClCacheKeyLess%34E03F430191.declarations

// Class ClCacheableObjectFactory 



//## Other Operations (implementation)
m4return_t ClCacheableObjectFactory::SerializeObject (ClCacheableObject *  ai_poObject, ClGenericIODriver *  IOD, m4uint8_t  ai_iMode)
{
  //## begin ClCacheableObjectFactory::SerializeObject%893688332.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheableObjectFactory::SerializeObject%893688332.body
}

m4return_t  ClCacheableObjectFactory::DeSerializeObject (ClCacheableObject * &  aio_poObject, ClGenericIODriver *  IOD, m4uint8_t  ai_iMode)
{
  //## begin ClCacheableObjectFactory::DeSerializeObject%893688333.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheableObjectFactory::DeSerializeObject%893688333.body
}

m4return_t ClCacheableObjectFactory::Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor)
{
  //## begin ClCacheableObjectFactory::Dump%917950250.body preserve=yes
	return M4_SUCCESS;
  //## end ClCacheableObjectFactory::Dump%917950250.body
}

// Additional Declarations
  //## begin ClCacheableObjectFactory%3544ABEC03B3.declarations preserve=yes
  //## end ClCacheableObjectFactory%3544ABEC03B3.declarations

// Class ClLRUDateLess 


//## Other Operations (implementation)
m4bool_t  ClLRUDateLess::operator () ( M4ClTimeStamp *     ai_oLRUDateA,  M4ClTimeStamp  *  ai_oLRUDateB) const
{
  //## begin ClLRUDateLess::operator ()%894362625.body preserve=yes
	if ((*ai_oLRUDateA) < (*ai_oLRUDateB)) 
		return M4_TRUE;

	return M4_FALSE;
	//return ai_oLRUDateA < ai_oLRUDateB ? M4_TRUE:M4_FALSE;
  //## end ClLRUDateLess::operator ()%894362625.body
}

// Additional Declarations
  //## begin ClLRUDateLess%354F1BC200B9.declarations preserve=yes
  //## end ClLRUDateLess%354F1BC200B9.declarations

//## begin module%34B4B5BF029C.epilog preserve=yes
//## end module%34B4B5BF029C.epilog
