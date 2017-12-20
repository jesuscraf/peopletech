//## begin module%377C8856033D.cm preserve=no
//## end module%377C8856033D.cm

//## begin module%377C8856033D.cp preserve=no
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
//## end module%377C8856033D.cp

//## Module: debuglog%377C8856033D; Package body
//## Subsystem: M4Log::src%377C88560044
//	d:\compon\m4log\version\src
//## Source file: D:\ricardo\m4log\src\debuglog.cpp

//## begin module%377C8856033D.additionalIncludes preserve=no
//## end module%377C8856033D.additionalIncludes

//## begin module%377C8856033D.includes preserve=yes

//#include "debuglog.hpp"
//#include "perslog.hpp"
//## end module%377C8856033D.includes

// perslog
#include <perslog.hpp>
// debuglog
#include <debuglog.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377C8856033D.declarations preserve=no
//## end module%377C8856033D.declarations

//## begin module%377C8856033D.additionalDeclarations preserve=yes


#include "logmanag.hpp"

#define M4_ERROR_NOT_ENOUGH_SPACE  -1;

const m4char_t  strIntraMessage = 1;
const m4char_t  strInterMessage = 2;

const m4uint16_t MARKED_OK = 0;
const m4uint16_t NO_MARKED = 1;
const m4uint16_t MARKED_EMPTY = 2;

const m4uint32_t SIZE_OF_SIZE = 16;
//Espacio que se va a ocupar para guardar el tamaño que ocupa la serialización
//completa y la serializacion de cada uno de los errores



//## end module%377C8856033D.additionalDeclarations


// Class ClAppItem 









ClAppItem::ClAppItem ()
  //## begin ClAppItem::ClAppItem%1846685448.hasinit preserve=no
  //## end ClAppItem::ClAppItem%1846685448.hasinit
  //## begin ClAppItem::ClAppItem%1846685448.initialization preserve=yes
  //## end ClAppItem::ClAppItem%1846685448.initialization
{
  //## begin ClAppItem::ClAppItem%1846685448.body preserve=yes

	m_lerrCode = 0;
	m_strBuffer = "";
	m_creturn = 10;
	m_sflags = 0;
	m_btime = M4_FALSE;

	m_strErrType = "";

  //## end ClAppItem::ClAppItem%1846685448.body
}

ClAppItem::ClAppItem (m4int32_t lerrCode, m4uint32_t sflags, m4char_t creturn, m4bool_t btime, M4ClString strType, M4ClString text)
  //## begin ClAppItem::ClAppItem%582279797.hasinit preserve=no
  //## end ClAppItem::ClAppItem%582279797.hasinit
  //## begin ClAppItem::ClAppItem%582279797.initialization preserve=yes
  //## end ClAppItem::ClAppItem%582279797.initialization
{
  //## begin ClAppItem::ClAppItem%582279797.body preserve=yes

	m_lerrCode = lerrCode;
	m_strBuffer = text;
	m_creturn = creturn;
	m_sflags = sflags;
	m_btime = btime;
	m_strErrType = strType;

  //## end ClAppItem::ClAppItem%582279797.body
}


ClAppItem::~ClAppItem ()
{
  //## begin ClAppItem::~ClAppItem%-800315767.body preserve=yes
  //## end ClAppItem::~ClAppItem%-800315767.body
}



//## Other Operations (implementation)
m4int32_t ClAppItem::GetErrCode ()
{
  //## begin ClAppItem::GetErrCode%1993055475.body preserve=yes

	return m_lerrCode;

  //## end ClAppItem::GetErrCode%1993055475.body
}

M4ClString ClAppItem::GetStrErrCode ()
{
  //## begin ClAppItem::GetStrErrCode%451046032.body preserve=yes

	m4char_t	pcode[12];
	M4ClString	scode;

	sprintf(pcode,"%lx",m_lerrCode);

	scode = "0x";
	scode += pcode;

	return scode;

  //## end ClAppItem::GetStrErrCode%451046032.body
}

M4ClString ClAppItem::GetErrType ()
{
  //## begin ClAppItem::GetErrType%371738280.body preserve=yes

	return m_strErrType;

  //## end ClAppItem::GetErrType%371738280.body
}

void ClAppItem::SetErrCode (m4int32_t lerrCode)
{
  //## begin ClAppItem::SetErrCode%748674036.body preserve=yes

	m_lerrCode = lerrCode;

  //## end ClAppItem::SetErrCode%748674036.body
}

m4char_t ClAppItem::GetCReturn ()
{
  //## begin ClAppItem::GetCReturn%473772215.body preserve=yes

	return m_creturn;

  //## end ClAppItem::GetCReturn%473772215.body
}

void ClAppItem::SetCReturn (m4char_t creturn)
{
  //## begin ClAppItem::SetCReturn%1059605662.body preserve=yes

	m_creturn = creturn;

  //## end ClAppItem::SetCReturn%1059605662.body
}

m4int16_t ClAppItem::GetBuffer (M4ClString& text)
{
  //## begin ClAppItem::GetBuffer%-532599070.body preserve=yes

	text = m_strBuffer;	

	if ( text.empty() )
		return M4_WARNING;
	return M4_SUCCESS;

  //## end ClAppItem::GetBuffer%-532599070.body
}

M4ClString ClAppItem::GetBuffer ()
{
  //## begin ClAppItem::GetBuffer%2072103689.body preserve=yes

	return m_strBuffer;

  //## end ClAppItem::GetBuffer%2072103689.body
}

m4int32_t ClAppItem::BufferSize ()
{
  //## begin ClAppItem::BufferSize%-1088959851.body preserve=yes

	return (m4int32_t) m_strBuffer.size();

  //## end ClAppItem::BufferSize%-1088959851.body
}

void ClAppItem::AddText (const M4ClString& text)
{
  //## begin ClAppItem::AddText%-1440165114.body preserve=yes

	m_strBuffer += text;

  //## end ClAppItem::AddText%-1440165114.body
}

void ClAppItem::AddTextHead (const M4ClString& stext)
{
  //## begin ClAppItem::AddTextHead%-1136147123.body preserve=yes

	M4ClString	bufferTemp;

	bufferTemp = m_strBuffer;
	m_strBuffer = stext;
	m_strBuffer += bufferTemp;

  //## end ClAppItem::AddTextHead%-1136147123.body
}

m4int16_t ClAppItem::ClearBuffer ()
{
  //## begin ClAppItem::ClearBuffer%1707966713.body preserve=yes

	m_strBuffer = "";
	return M4_SUCCESS;

  //## end ClAppItem::ClearBuffer%1707966713.body
}

ClAppItem* ClAppItem::CopyAppItem ()
{
  //## begin ClAppItem::CopyAppItem%-1700431489.body preserve=yes

	ClAppItem	*pappItem;
	pappItem = new ClAppItem( m_lerrCode, m_sflags, m_creturn , m_btime, m_strErrType, m_strBuffer );

	return pappItem;

  //## end ClAppItem::CopyAppItem%-1700431489.body
}

m4bool_t ClAppItem::IsActive_Time ()
{
  //## begin ClAppItem::IsActive_Time%1012462751.body preserve=yes

	return m_btime;

  //## end ClAppItem::IsActive_Time%1012462751.body
}

M4ClString ClAppItem::GetTime ()
{
  //## begin ClAppItem::GetTime%1551244818.body preserve=yes

	return	m_strTime;

  //## end ClAppItem::GetTime%1551244818.body
}

void ClAppItem::SetTime (const M4ClString& strTime)
{
  //## begin ClAppItem::SetTime%301760979.body preserve=yes

	m_strTime = strTime;

  //## end ClAppItem::SetTime%301760979.body
}

m4uint32_t ClAppItem::FlagAnd (m4uint32_t sflags)
{
  //## begin ClAppItem::FlagAnd%2066443180.body preserve=yes

	if ( sflags < 0 )
		return M4_ERROR;

	m4uint32_t	sflags1 = 0;

	sflags1 = ( sflags & m_sflags );

	m_sflags = sflags1;
	return M4_SUCCESS;

  //## end ClAppItem::FlagAnd%2066443180.body
}

m4uint32_t ClAppItem::GetFlags ()
{
  //## begin ClAppItem::GetFlags%-226561052.body preserve=yes

	return m_sflags;

  //## end ClAppItem::GetFlags%-226561052.body
}

// Additional Declarations
  //## begin ClAppItem%377C88460073.declarations preserve=yes
  //## end ClAppItem%377C88460073.declarations

// Class ClAppointments 













ClAppointments::ClAppointments (m4bool_t ai_shared)
  //## begin ClAppointments::ClAppointments%535977362.hasinit preserve=no
  //## end ClAppointments::ClAppointments%535977362.hasinit
  //## begin ClAppointments::ClAppointments%535977362.initialization preserve=yes
  //## end ClAppointments::ClAppointments%535977362.initialization
{
  //## begin ClAppointments::ClAppointments%535977362.body preserve=yes

	m_plstorages = NULL;
	m_uiNoMark = NO_MARKED;
	m_bShared = ai_shared;
	m_uiMessageCounter = 0;
//	m_bShared=M4_TRUE;
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Init();
	

  //## end ClAppointments::ClAppointments%535977362.body
}

ClAppointments::ClAppointments (ClStorageList* plstorages, m4bool_t ai_shared)
  //## begin ClAppointments::ClAppointments%-883074347.hasinit preserve=no
  //## end ClAppointments::ClAppointments%-883074347.hasinit
  //## begin ClAppointments::ClAppointments%-883074347.initialization preserve=yes
  //## end ClAppointments::ClAppointments%-883074347.initialization
{
  //## begin ClAppointments::ClAppointments%-883074347.body preserve=yes


	m_plstorages = plstorages;
	m_uiNoMark = NO_MARKED;
	m_bShared = ai_shared;
	m_uiMessageCounter = 0;
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Init();

  //## end ClAppointments::ClAppointments%-883074347.body
}


ClAppointments::~ClAppointments ()
{
  //## begin ClAppointments::~ClAppointments%-723426398.body preserve=yes

	APPOINTQUEUE::iterator	iter; //, iter1;
	ClAppItem	*pappoint;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	
	iter = m_qappoints.begin();
	m_plstorages = NULL;

	while( iter != m_qappoints.end() )
	{
		pappoint = *iter;
		
		if ( pappoint )
			delete pappoint;
		m_qappoints.erase( iter );
		iter = m_qappoints.begin();
	}

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();

  //## end ClAppointments::~ClAppointments%-723426398.body
}



//## Other Operations (implementation)
m4char_t ClAppointments::GetCReturn ()
{
  //## begin ClAppointments::GetCReturn%-1053035615.body preserve=yes

	return m_creturn;

  //## end ClAppointments::GetCReturn%-1053035615.body
}

void ClAppointments::SetCReturn (m4char_t creturn)
{
  //## begin ClAppointments::SetCReturn%352196340.body preserve=yes

	m_creturn = creturn;

  //## end ClAppointments::SetCReturn%352196340.body
}

ClStorageList* ClAppointments::GetStorageList ()
{
  //## begin ClAppointments::GetStorageList%1033586053.body preserve=yes

	return m_plstorages;

  //## end ClAppointments::GetStorageList%1033586053.body
}

void ClAppointments::SetStorageList (ClStorageList* plstorages)
{
  //## begin ClAppointments::SetStorageList%-847460015.body preserve=yes

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	m_plstorages = plstorages;
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();

  //## end ClAppointments::SetStorageList%-847460015.body
}

void ClAppointments::AddAppoint (ClAppItem* pappoint)
{
  //## begin ClAppointments::AddAppoint%-806800384.body preserve=yes

	
	if ( pappoint != NULL ) {
		if ( GetSize() >= GetMaxQueueSize () ) 
			Pop();
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Lock();
		
		m_qappoints.push_back( pappoint );
		
		m_uiMessageCounter++;
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Unlock();
	}
	
  //## end ClAppointments::AddAppoint%-806800384.body
}

void ClAppointments::StoreAppoints (m4uint32_t sflags)
{
  //## begin ClAppointments::StoreAppoints%-1208000047.body preserve=yes


	// Tira todos los appoints a los almacenamientos y los saca de la pila.

	APPOINTQUEUE::iterator	iter;
	ClAppItem	*pappoint;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();

	while ( m_qappoints.size() )
	{

		iter = m_qappoints.begin();
		pappoint = * iter;
		m_qappoints.pop_front();

	
		m_plstorages -> StoreTo( sflags, pappoint );

		if ( pappoint ) {
			if (m_bShared==M4_TRUE) 
				m_AccessMutex.Unlock();
			
			delete pappoint;
		}
	}

		//	ADD	EMN	990305
	m_oMark = m_qappoints.end();
	if (m_uiNoMark == MARKED_OK) m_uiNoMark = MARKED_EMPTY;
	
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();
	
	//	END ADD

  //## end ClAppointments::StoreAppoints%-1208000047.body
}

void ClAppointments::StoreAppoint (m4uint32_t sflags)
{
  //## begin ClAppointments::StoreAppoint%1526745902.body preserve=yes


	// Tira el appoint a los almacenamientos y NO lo saca de la pila
	
	APPOINTQUEUE::iterator	iter;
	ClAppItem	*pappoint;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	if ( m_qappoints.size() )
	{

		iter = m_qappoints.begin();
		pappoint = * iter;

	
		m_plstorages -> StoreTo( sflags, pappoint );

	}

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();

  //## end ClAppointments::StoreAppoint%1526745902.body
}

m4int16_t ClAppointments::AddAppointsTo (ClAppointments* pappoints)
{
  //## begin ClAppointments::AddAppointsTo%-1281544253.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem	*pappoint;

// si los códigos de retorno no son iguales no añado nada.
	if ( pappoints -> GetCReturn() != m_creturn )
		return M4_ERROR;

	if ( this == pappoints )
		return M4_ERROR;

// copio toda la lista de apuntes

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();

	for ( iter = m_qappoints.begin(); iter != m_qappoints.end()
					; ++iter )
	{
		pappoint =  *iter;

		pappoints -> AddAppoint( pappoint -> CopyAppItem() );

	}

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();

	return M4_SUCCESS;

  //## end ClAppointments::AddAppointsTo%-1281544253.body
}

m4int32_t ClAppointments::GetErrorQueueSize ()
{
  //## begin ClAppointments::GetErrorQueueSize%724450828.body preserve=yes


	m4int32_t iret;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	iret = (m4int32_t) m_qappoints.size();
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock();

	return iret;

  //## end ClAppointments::GetErrorQueueSize%724450828.body
}

m4return_t ClAppointments::GetErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetErrorTime%-891402757.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();

	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Unlock(); 
		
		return M4_ERROR;
	}
	iter = m_qappoints.begin();
	pData = ( * iter );
	stData = pData -> GetTime();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Unlock(); 
		
		return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock(); 
	
	return M4_SUCCESS;

  //## end ClAppointments::GetErrorTime%-891402757.body
}

m4return_t ClAppointments::GetLastErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetLastErrorTime%-126725663.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Unlock(); 
		
		return M4_ERROR;
	}
	iter = m_qappoints.end();
	iter--;
	pData = ( * iter );
	stData = pData -> GetTime();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetLastErrorTime%-126725663.body
}

m4return_t ClAppointments::GetMarkedErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetMarkedErrorTime%435707202.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	
	if ( m_uiNoMark == NO_MARKED )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	
	if (m_oMark == m_qappoints.end())
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
    }

	iter = m_oMark;
	pData = ( * iter );

	if (pData == NULL)
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
    }

	stData = pData -> GetTime();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetMarkedErrorTime%435707202.body
}

m4return_t ClAppointments::Get_n_ErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_ErrorTime%2045075710.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	
	iter = m_qappoints.begin();
	iter+= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	
	stData = pData -> GetTime();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_ErrorTime%2045075710.body
}

m4return_t ClAppointments::Get_n_LastErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_LastErrorTime%1044764995.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) 
			m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	iter-= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetTime();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_LastErrorTime%1044764995.body
}

m4return_t ClAppointments::GetErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetErrorCode%1649465942.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	
	if ( m_qappoints.size() == 0)
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	iter = m_qappoints.begin();
	pData = ( * iter );
	stData = pData -> GetStrErrCode();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetErrorCode%1649465942.body
}

m4return_t ClAppointments::GetErrorCode (m4int32_t& ao_lErrorCode)
{
  //## begin ClAppointments::GetErrorCode%2126423985.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;


	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	iter = m_qappoints.begin();
	pData = ( * iter );
	ao_lErrorCode = pData -> GetErrCode();
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
	return M4_SUCCESS;

  //## end ClAppointments::GetErrorCode%2126423985.body
}

m4return_t ClAppointments::GetLastErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetLastErrorCode%-364837482.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	pData = ( * iter );
	stData = pData -> GetStrErrCode();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetLastErrorCode%-364837482.body
}

m4return_t ClAppointments::GetLastErrorCode (m4int32_t& ao_lErrorCode)
{
  //## begin ClAppointments::GetLastErrorCode%-50544292.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;


	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	iter = m_qappoints.end();
	iter--;
	pData = ( * iter );
	ao_lErrorCode = pData -> GetErrCode();
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetLastErrorCode%-50544292.body
}

m4return_t ClAppointments::GetMarkedErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetMarkedErrorCode%497573662.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	
	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_uiNoMark == NO_MARKED )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	if (m_oMark == m_qappoints.end())
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
    }

	iter = m_oMark;
	pData = ( * iter );

	if (pData == NULL)
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
    }

	stData = pData -> GetStrErrCode();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
	    char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetMarkedErrorCode%497573662.body
}

m4return_t ClAppointments::Get_n_ErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_ErrorCode%-575868519.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	iter = m_qappoints.begin();
	iter+= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetStrErrCode();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_ErrorCode%-575868519.body
}

m4return_t ClAppointments::Get_n_ErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode)
{
  //## begin ClAppointments::Get_n_ErrorCode%1279298605.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.begin();
	iter+= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	ao_lErrorCode = pData -> GetErrCode();
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	return M4_SUCCESS;

  //## end ClAppointments::Get_n_ErrorCode%1279298605.body
}

m4return_t ClAppointments::Get_n_LastErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_LastErrorCode%-1270666693.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	iter-= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetStrErrCode();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_LastErrorCode%-1270666693.body
}

m4return_t ClAppointments::Get_n_LastErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode)
{
  //## begin ClAppointments::Get_n_LastErrorCode%101542522.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	iter-= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	ao_lErrorCode = pData -> GetErrCode();
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
	return M4_SUCCESS;

  //## end ClAppointments::Get_n_LastErrorCode%101542522.body
}

m4return_t ClAppointments::GetErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetErrorType%-395443388.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.begin();
	pData = ( * iter );
	stData = pData -> GetErrType();
	szData = (m4int32_t) stData.size();

	if ( szData > ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetErrorType%-395443388.body
}

m4return_t ClAppointments::GetLastErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetLastErrorType%-1716724223.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	iter = m_qappoints.end();
	iter--;
	pData = ( * iter );
	stData = pData -> GetErrType();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetLastErrorType%-1716724223.body
}

m4return_t ClAppointments::GetMarkedErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetMarkedErrorType%-1768173730.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_uiNoMark == NO_MARKED )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	if (m_oMark == m_qappoints.end())
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
    }

	iter = m_oMark;
	pData = ( * iter );

	if (pData == NULL)
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
    }

	stData = pData -> GetErrType();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetMarkedErrorType%-1768173730.body
}

m4return_t ClAppointments::Get_n_ErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_ErrorType%1599231282.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}
	
	iter = m_qappoints.begin();
	iter+= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetErrType();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_ErrorType%1599231282.body
}

m4return_t ClAppointments::Get_n_LastErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_LastErrorType%429777790.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	iter-= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetErrType();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_LastErrorType%429777790.body
}

m4return_t ClAppointments::GetErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetErrorText%1659764382.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	iter = m_qappoints.begin();
	pData = ( * iter );
	stData = pData -> GetBuffer();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;

	pData = NULL;

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetErrorText%1659764382.body
}

m4return_t ClAppointments::GetLastErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetLastErrorText%1187655810.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	pData = ( * iter );
	stData = pData -> GetBuffer();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetLastErrorText%1187655810.body
}

m4return_t ClAppointments::GetMarkedErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetMarkedErrorText%-1085108578.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_uiNoMark == NO_MARKED )
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR;
	}

	if (m_oMark == m_qappoints.end())
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR;
    }

	iter = m_oMark;
	pData = ( * iter );

	if (pData == NULL)
	{
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR;
    }

	stData = pData -> GetBuffer();
	szData = (m4int32_t) stData.size();

	// checks the query mode
	// To support JS: Fix JS Bug 0072915

	if (ai_szBuffer == NULL) {
		ao_lSizeCopied = szData + 1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;
	}

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::GetMarkedErrorText%-1085108578.body
}

m4return_t ClAppointments::Get_n_ErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_ErrorText%491644706.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	iter = m_qappoints.begin();
	iter+= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetBuffer();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_ErrorText%491644706.body
}

m4return_t ClAppointments::Get_n_LastErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::Get_n_LastErrorText%1596771516.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData;
    M4ClString stData;
	m4int32_t  szData;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	if ( m_qappoints.size() == 0 || (m4uint32_t)ai_lIterator >= m_qappoints.size()) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.end();
	iter--;
	iter-= ai_lIterator;
	pData = ( * iter );

	if (pData == NULL) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); 
		return M4_ERROR;
	}

	stData = pData -> GetBuffer();
	szData = (m4int32_t) stData.size();

	if ( szData >= ai_lBufferSize )
	{
		char *aux = stData;
		//	MOD	EMN	990305		memcpy( ai_szBuffer, aux, ai_lBufferSize );
		memcpy( ai_szBuffer, aux, ai_lBufferSize-1 );
		ai_szBuffer [ai_lBufferSize-1] = '\0';
		ao_lSizeCopied = ai_lBufferSize-1;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_ERROR_NOT_ENOUGH_SPACE;
	}
	strcpy( ai_szBuffer, stData );
	ao_lSizeCopied = szData;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock(); return M4_SUCCESS;

  //## end ClAppointments::Get_n_LastErrorText%1596771516.body
}

m4return_t ClAppointments::SetMark ()
{
  //## begin ClAppointments::SetMark%1502639630.body preserve=yes

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	
	if ( m_qappoints.size() == 0 )
	{		
		m_uiNoMark = MARKED_EMPTY;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_SUCCESS;
	}
	
	m_uiNoMark = MARKED_OK;
	m_oMark = m_qappoints.end();
//	m_oMark--; DEL FJSM 990624 Antes de hacer un movemark se utiliza
//  isvalidmark(), y directamente se lee....
	
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	return M4_SUCCESS;


  //## end ClAppointments::SetMark%1502639630.body
}

m4return_t ClAppointments::MoveMark ()
{
  //## begin ClAppointments::MoveMark%1639088559.body preserve=yes

	// Si no has puesto marca.
	
	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	
	if ( m_uiNoMark == NO_MARKED ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	if (m_oMark == m_qappoints.end()){
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	// Antes de poner la marca no habia errores, luego cogemos el primero.
	if ( m_uiNoMark == MARKED_EMPTY )
	{
		m_uiNoMark = MARKED_OK;
		m_oMark = m_qappoints.begin();
	}
	else
		m_oMark++;

	if ( m_oMark == m_qappoints.end() ){
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();

	return M4_SUCCESS;

  //## end ClAppointments::MoveMark%1639088559.body
}

m4return_t ClAppointments::IsValidMark ()
{
  //## begin ClAppointments::IsValidMark%1637253055.body preserve=yes

	// Si no has puesto marca.
	
	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();

	if ( m_uiNoMark == NO_MARKED ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	// Antes de poner la marca no habia errores, luego cogemos el primero.
	if ( m_uiNoMark == MARKED_EMPTY )
	{
		m_uiNoMark = MARKED_OK;
		m_oMark = m_qappoints.begin();
	}

	if (m_oMark == m_qappoints.end()){
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	
	return M4_SUCCESS;

  //## end ClAppointments::IsValidMark%1637253055.body
}

m4return_t ClAppointments::Pop ()
{
  //## begin ClAppointments::Pop%1940072994.body preserve=yes

	APPOINTQUEUE::iterator	iter;
	ClAppItem *pData = NULL;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();

	if ( m_qappoints.size() == 0 ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.begin();
	pData = ( *iter );
	m_qappoints.pop_front();
	
	if ( m_uiNoMark != 1 )
		m_oMark = m_qappoints.begin();
	
	if ( pData != NULL)
		delete pData;	

	
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();

	return M4_SUCCESS;

  //## end ClAppointments::Pop%1940072994.body
}

m4return_t ClAppointments::Pop_back ()
{
  //## begin ClAppointments::Pop_back%600430664.body preserve=yes

    APPOINTQUEUE::iterator	iter;
	ClAppItem *pData = NULL;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();

	if ( m_qappoints.size() == 0 ) {
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return M4_ERROR;
	}

	iter = m_qappoints.end();

	iter--;
	pData = ( *iter );
	m_qappoints.pop_back();
	if ( pData != NULL)
		delete pData;	

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	
	return M4_SUCCESS;

  //## end ClAppointments::Pop_back%600430664.body
}

m4uint32_t ClAppointments::GetSize ()
{
  //## begin ClAppointments::GetSize%750365623.body preserve=yes

	m4uint32_t iret;

//	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	iret = (m4uint32_t) m_qappoints.size();
//	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	
	return iret; 

  //## end ClAppointments::GetSize%750365623.body
}

m4bool_t ClAppointments::Empty ()
{
  //## begin ClAppointments::Empty%-2076019487.body preserve=yes

	size_t iret;
	
	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	iret = m_qappoints.size();
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();

	if ( iret == 0 )
		return M4_TRUE;

	return M4_FALSE;;

  //## end ClAppointments::Empty%-2076019487.body
}

m4return_t ClAppointments::GetSerialStringSizeFromMessage (m4uint32_t& ao_StringSize, m4uint32_t ai_MessageCounter)
{
  //## begin ClAppointments::GetSerialStringSizeFromMessage%938426073.body preserve=yes
  

	APPOINTQUEUE::iterator	iter;
	ClAppItem *	pData;
	m4uint32_t	totalSize = SIZE_OF_SIZE;                // 8 primeros bytes para el tamaño total
	M4ClString	sType,sTime,sBuffer;
	size_t		sizeType,sizeTime,sizeBuffer,sizeCode;
	m4uint32_t	firstMessage,lastMessage;
	m4return_t	iret = M4_SUCCESS;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();

	lastMessage = m_uiMessageCounter;
	firstMessage = lastMessage - GetSize();

	// se han perdido mensajes.
	if (ai_MessageCounter<firstMessage) iret = M4_WARNING;
	
	// no hay nuevos mensajes, o se pregunta por mensajes que no se han producido.
	if (ai_MessageCounter>=lastMessage) {
		iret = M4_SUCCESS;
		ao_StringSize = 0;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return iret;
	}

	if ( m_qappoints.size() == 0 ) {
		iret = M4_SUCCESS;
		ao_StringSize = 0;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return iret;
	}
	

	iter =  m_qappoints.begin();
	if (ai_MessageCounter>firstMessage)
		iter += (ai_MessageCounter-firstMessage);
	
	while (iter!=m_qappoints.end()) {

		pData = ( * iter );

		sType = pData -> GetErrType();
		sTime = pData -> GetTime();
		sBuffer = pData -> GetBuffer();
	
		sizeType = sType.size();
		sizeTime = sTime.size();
		sizeCode = SIZE_OF_SIZE;				// Long
		sizeBuffer = sBuffer.size();

		totalSize += SIZE_OF_SIZE;             // + 8 por el tamaño local.

		totalSize += (m4uint32_t) (sizeType + sizeTime + sizeCode + sizeBuffer + 4);				// + 4 por cada uno de los separadores.	
		iter++;
	}

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();

	ao_StringSize = totalSize;
	return iret;
	
	
	
  //## end ClAppointments::GetSerialStringSizeFromMessage%938426073.body
}

m4return_t ClAppointments::GetSerialStringFromMessage (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter, m4uint32_t ai_MessageCounter)
{
  //## begin ClAppointments::GetSerialStringFromMessage%938426074.body preserve=yes
  	// Serializamos la pila PROPIA del ClLogSystem.

	APPOINTQUEUE::iterator	iter;
	ClAppItem *				pData;
	m4uint32_t				IndividualSize = 0;
	m4uint32_t				cnt=0;					
	m4pchar_t				aux = ai_szBuffer;
	M4ClString				sType,sTime,sBuffer;
	size_t					sizeType,sizeTime,sizeBuffer,sizeCode;
   
	m4return_t				iret = M4_SUCCESS;
	m4bool_t				beginSerial = M4_FALSE; 
	m4uint32_t				firstMessage,lastMessage, iterCount;
	
	char *auxcad;

	//                     SIZE_OF_SIZE   SIZE_OF_SIZE              TamañoLocal
	//
	// Serializacion -->   TamañoTotal    TamañoLocal    Typo-1-Tiempo-1-Codigo-1-Texto-2
    //								      TamañoLocal    Typo-1-Tiempo-1-Codigo-1-Texto-2 	
	//										.        .     .      .     .
	//										.        .     .      .     .

	m4char_t  sIntCad[SIZE_OF_SIZE];
	iterCount = 0;

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();

	lastMessage = m_uiMessageCounter;
	firstMessage = lastMessage - GetSize();

	// se han perdido mensajes.
	if (ai_MessageCounter<firstMessage) iret = M4_WARNING;
	
	// no hay nuevos mensajes, o se pregunta por mensajes que no se han producido.
	if ((ai_MessageCounter>=lastMessage)||( m_qappoints.size() == 0 )) {
		iret = M4_SUCCESS;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return iret;
	}	

	if ((aux==NULL)||(ai_lBufferSize<=SIZE_OF_SIZE)) {
		iret = M4_ERROR;
		if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
		return iret;
	}	

//	sprintf(sIntCad,"%d",ai_lBufferSize);
//	memcpy(aux+cnt,sIntCad,SIZE_OF_SIZE);
	cnt+=SIZE_OF_SIZE;

	iter =  m_qappoints.begin();

	if (ai_MessageCounter>firstMessage)
		iter += (ai_MessageCounter-firstMessage);
	
	while (iter!=m_qappoints.end()) {

		pData = ( * iter );

		sType = pData -> GetErrType();
		sTime = pData -> GetTime();
		sBuffer = pData -> GetBuffer();
	
		sizeType = sType.size();
		sizeTime = sTime.size();
		sizeCode = SIZE_OF_SIZE;				
		sizeBuffer = sBuffer.size();

		IndividualSize = (m4uint32_t) (sizeType + sizeTime + sizeCode + sizeBuffer + 4);							 // Uno para cada separador.
		
		// Primero el tamaño local

		if (cnt+IndividualSize>ai_lBufferSize) {
			iret = M4_ERROR;
			break;
		}
		
		beginSerial = M4_TRUE;

		memset(sIntCad,0,SIZE_OF_SIZE);
		sprintf(sIntCad,"%d",IndividualSize);
		memcpy(aux+cnt,sIntCad,SIZE_OF_SIZE);
		cnt+=SIZE_OF_SIZE;

		// Luego el Tipo
		if ( sizeType != 0 ) {
          auxcad = sType;
          memcpy(aux+cnt,auxcad,sizeType);
          cnt+=(m4uint32_t) sizeType;
		};

		memcpy(aux+cnt,&strIntraMessage,1); cnt++;   // Cambio el caracter 0 por 1

		// El tiempo
		if ( sizeTime != 0) {
          auxcad = sTime;
          memcpy(aux+cnt,auxcad,sizeTime);
          cnt+=(m4uint32_t) sizeTime;
		};

		memcpy(aux+cnt,&strIntraMessage,1); cnt++;   // Cambio el caracter 0 por 1
		// El Codigo
		memset(sIntCad,0,SIZE_OF_SIZE);
		sprintf(sIntCad,"%d", pData -> GetErrCode());
		memcpy(aux+cnt,sIntCad,SIZE_OF_SIZE);
		cnt+=SIZE_OF_SIZE;

		memcpy(aux+cnt,&strIntraMessage,1); cnt++;   // Cambio el caracter 0 por 1
		// Y el mensaje
		if ( sizeBuffer != 0) {
          auxcad = sBuffer;
		  memcpy(aux+cnt,auxcad,sizeBuffer);
          cnt+= (m4uint32_t) sizeBuffer;
		};

		memcpy(aux+cnt, &strInterMessage,1); cnt++;   // Cambio el caracter 0 por 2
		iter++;
		iterCount++;
	}
	
	if (beginSerial == M4_TRUE) {
		memset(sIntCad,0,SIZE_OF_SIZE);
		sprintf(sIntCad,"%d",cnt);
		memcpy(aux,sIntCad,SIZE_OF_SIZE);
	}

	if (ai_MessageCounter>firstMessage) 
		ao_lastMessageIter = ai_MessageCounter + iterCount;
	else 
		ao_lastMessageIter = firstMessage + iterCount;

	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();
	
	return iret;

  //## end ClAppointments::GetSerialStringFromMessage%938426074.body
}

m4int32_t ClAppointments::GetMessageCounter ()
{
  //## begin ClAppointments::GetMessageCounter%938426075.body preserve=yes
  
	return m_uiMessageCounter;
	
  //## end ClAppointments::GetMessageCounter%938426075.body
}

m4uint32_t ClAppointments::GetMaxQueueSize ()
{
  //## begin ClAppointments::GetMaxQueueSize%644034027.body preserve=yes

//  Modificacion momentanea para i_180.04.00.17.04.00
	return ((ClLogManager *)GETLOGMANAGER())->GetMaxQueueSize();
	
//	return m_MaxQueueSize;



  //## end ClAppointments::GetMaxQueueSize%644034027.body
}

void ClAppointments::SetMaxQueueSize (m4uint32_t ai_MaxQueueSize)
{
  //## begin ClAppointments::SetMaxQueueSize%-104986863.body preserve=yes

	if (m_bShared==M4_TRUE) m_AccessMutex.Lock();
	m_MaxQueueSize = ai_MaxQueueSize;
	if (m_bShared==M4_TRUE) m_AccessMutex.Unlock();

  //## end ClAppointments::SetMaxQueueSize%-104986863.body
}

m4return_t ClAppointments::GetMessageItem (const m4pchar_t ai_getMethodTag, m4uint32_t ai_itemOffset, const m4pchar_t ai_itemTag, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClAppointments::GetMessageItem%938705723.body preserve=yes
 
	return M4_ERROR;
	
	
  //## end ClAppointments::GetMessageItem%938705723.body
}

ClAppItem* ClAppointments::GetAppoint (m4uint32_t ai_iterator)
{
  //## begin ClAppointments::GetAppoint%944066182.body preserve=yes
 
	m4uint32_t size=GetErrorQueueSize();

	if (ai_iterator>=size) 
		return NULL;
	else 
		return m_qappoints[ai_iterator];
	
  //## end ClAppointments::GetAppoint%944066182.body
}

// Additional Declarations
  //## begin ClAppointments%377C88460147.declarations preserve=yes
  //## end ClAppointments%377C88460147.declarations

//## begin module%377C8856033D.epilog preserve=yes
//## end module%377C8856033D.epilog
