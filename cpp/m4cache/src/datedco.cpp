//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                datedco.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================


#include "datedco.hpp"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//## end module.includes preserve=yes
ClCacheDatedKey::ClCacheDatedKey (m4pchar_t ai_pszKey) 
//## begin ClCacheDatedKey::ClCacheDatedKey%1741536843.initialization preserve=yes
: ClCacheKey (ai_pszKey)
//## end ClCacheDatedKey::ClCacheDatedKey%1741536843.initialization
{

//## begin ClCacheDatedKey::ClCacheDatedKey%1741536843.body preserve=yes
	m_StartWorkDate = 0;
	m_EndWorkDate = 0;
	m_StartCorrDate = 0;
	m_EndCorrDate = 0;
//## end ClCacheDatedKey::ClCacheDatedKey%1741536843.body
}

ClCacheDatedKey::ClCacheDatedKey (m4pchar_t ai_pszKey,
			     m4date_t ai_StartWorkDate, m4date_t ai_EndWorkDate,
				 m4date_t ai_StartCorrDate, m4date_t ai_EndCorrDate) 
//## begin ClCacheDatedKey::ClCacheDatedKey%202538219.initialization preserve=yes
: ClCacheKey (ai_pszKey)
//## end ClCacheDatedKey::ClCacheDatedKey%202538219.initialization
{

//## begin ClCacheDatedKey::ClCacheDatedKey%202538219.body preserve=yes
	SetDates (ai_StartWorkDate, ai_EndWorkDate,
			  ai_StartCorrDate, ai_EndCorrDate);
//## end ClCacheDatedKey::ClCacheDatedKey%202538219.body
}


ClCacheDatedKey::~ClCacheDatedKey ()
{
//## begin ClCacheDatedKey::~ClCacheDatedKey%-1802091074.body preserve=yes
//## end ClCacheDatedKey::~ClCacheDatedKey%-1802091074.body
}

m4return_t ClCacheDatedKey::Serialize(ClGenericIODriver& IOD)
{
//## begin ClCacheDatedKey::Serialize%1523483998.body preserve=yes
	m4return_t ret;

	//Clase Base
	ClCacheKey::Serialize(IOD);

	//Fechas
	ret = IOD.Write(m_StartWorkDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_EndWorkDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_StartCorrDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_EndCorrDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
//## end ClCacheDatedKey::Serialize%1523483998.body
}

m4return_t ClCacheDatedKey::DeSerialize(ClGenericIODriver& IOD)
{
//## begin ClCacheDatedKey::DeSerialize%1980694992.body preserve=yes
	m4return_t ret;

	//Clase Base
	ClCacheKey::DeSerialize(IOD);

	//Fechas
	ret = IOD.Read(m_StartWorkDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Read(m_EndWorkDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Read(m_StartCorrDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Read(m_EndCorrDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
//## end ClCacheDatedKey::DeSerialize%1980694992.body
}


void ClCacheDatedKey::SetDates ( m4date_t ai_StartWorkDate, m4date_t ai_EndWorkDate,
								 m4date_t ai_StartCorrDate, m4date_t ai_EndCorrDate)
{
//## begin ClCacheDatedKey::SetDates%-1366936452.body preserve=yes
	m_StartWorkDate = ai_StartWorkDate;
	m_EndWorkDate = ai_EndWorkDate;
	m_StartCorrDate = ai_StartCorrDate;
	m_EndCorrDate = ai_EndCorrDate;
//## end ClCacheDatedKey::SetDates%-1366936452.body
}

m4bool_t ClCacheDatedKey::Contains (ClCacheDatedKey &ai_oDatedKey)
{
//## begin ClCacheDatedKey::Contains%-1060881017.body preserve=yes
	if ( (m_StartWorkDate <= ai_oDatedKey.m_StartWorkDate) &&
		 (m_EndWorkDate >= ai_oDatedKey.m_EndWorkDate) &&
		 (m_StartCorrDate <= ai_oDatedKey.m_StartCorrDate) &&
		 (m_EndCorrDate >= ai_oDatedKey.m_EndCorrDate) )
		return M4_TRUE;
	else
		return M4_FALSE;
		
//## end ClCacheDatedKey::Contains%-1060881017.body
}


m4return_t ClCacheDatedKey::Dump(ClDumpCacheVisitor& ai_oVisitor)
{
//## begin ClCacheDatedKey::Dump%1305217339.body preserve=yes
	ClCacheKey::Dump(ai_oVisitor);

	if (M4_TRUE == ai_oVisitor.DumpDates())
	{
		ai_oVisitor << "\t\t\tStartWorkDate: " << m_StartWorkDate << EndDumpCacheLn;
		ai_oVisitor << "\t\t\tEndWorkDate: " << m_EndWorkDate << EndDumpCacheLn;
		ai_oVisitor << "\t\t\tStartCorrDate: " << m_StartCorrDate << EndDumpCacheLn;
		ai_oVisitor << "\t\t\tEndCorrDate: " << m_EndCorrDate << EndDumpCacheLn;
	}

	return M4_SUCCESS;
//## end ClCacheDatedKey::Dump%1305217339.body
}
 
