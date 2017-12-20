//## begin module%377C885701E0.cm preserve=no
//## end module%377C885701E0.cm

//## begin module%377C885701E0.cp preserve=no
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
//## end module%377C885701E0.cp

//## Module: m4log%377C885701E0; Package body
//## Subsystem: M4Log::src%377C88560044
//	d:\compon\m4log\version\src
//## Source file: d:\compon\m4log\version\src\m4log.cpp

//## begin module%377C885701E0.additionalIncludes preserve=no
//## end module%377C885701E0.additionalIncludes

//## begin module%377C885701E0.includes preserve=yes

//#include "m4log.hpp"
#include "logmanag.hpp"
//#include "m4date.hpp" //mio
/*#include <stdarg.h>//puede que no exista en UNIX mirarlo
#include "m4date.hpp"*/



//## end module%377C885701E0.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// stdio
#include <stdio.h>

class ClLogSystem;

//## begin module%377C885701E0.declarations preserve=no
//## end module%377C885701E0.declarations

//## begin module%377C885701E0.additionalDeclarations preserve=yes
//## end module%377C885701E0.additionalDeclarations


// Class ClLog 




ClLog::ClLog (m4int8_t slogMode)
  //## begin ClLog::ClLog%-1796335679.hasinit preserve=no
  //## end ClLog::ClLog%-1796335679.hasinit
  //## begin ClLog::ClLog%-1796335679.initialization preserve=yes
  //## end ClLog::ClLog%-1796335679.initialization
{
  //## begin ClLog::ClLog%-1796335679.body preserve=yes


	if ( ( slogMode < 0 ) || ( slogMode > 2 ) )
		slogMode = MODE_LOGTHREAD;

	m_slogMode = slogMode;

	ClLogManager	*plogManager = NULL;

	
	plogManager = GETLOGMANAGER();
	if  ( plogManager )
	{
		if ( plogManager -> TestCrono() )
		{
			m_bcrono = M4_TRUE;
		}
		else
		{
			m_bcrono = M4_FALSE;
		}
	}
	else
	{
		m_bcrono = M4_FALSE;
	}


	switch ( slogMode )
	{
	case MODE_LOGOWNER:
		m_plogSys = GETNEWLOGSYSTEM();
		break;
	case MODE_LOGREFERENCE:
		m_plogSys = NULL;
		break;
	default:
		m_plogSys = GETPTHREADLOGSYSTEM();		
		break;
	}


  //## end ClLog::ClLog%-1796335679.body
}

ClLog::ClLog (ClLogSystem* plogSys)
  //## begin ClLog::ClLog%483001396.hasinit preserve=no
  //## end ClLog::ClLog%483001396.hasinit
  //## begin ClLog::ClLog%483001396.initialization preserve=yes
  //## end ClLog::ClLog%483001396.initialization
{
  //## begin ClLog::ClLog%483001396.body preserve=yes


	ClLogManager	*plogManager;
	if  ( plogManager = GETLOGMANAGER() )
	{
		if ( plogManager -> TestCrono() )
		{
			m_bcrono = M4_TRUE;
		}
		else
		{
			m_bcrono = M4_FALSE;
		}
	}
	else
	{
		m_bcrono = M4_FALSE;
	}


	if( plogSys == NULL )
	{
		m_plogSys = GETNEWLOGSYSTEM() ;
		m_slogMode = MODE_LOGREFERENCE;
	}
	else
	{
		m_plogSys = plogSys;
		m_slogMode = MODE_LOGREFERENCE;
	}



  //## end ClLog::ClLog%483001396.body
}


ClLog::~ClLog ()
{
  //## begin ClLog::~ClLog%1044996127.body preserve=yes

	if ( m_slogMode == MODE_LOGOWNER )
		if ( m_plogSys )
			delete m_plogSys;
//	if ( m_pcrono )
//		delete m_pcrono;

  //## end ClLog::~ClLog%1044996127.body
}



//## Other Operations (implementation)
m4int16_t ClLog::STARTCODE (m4int32_t icode, m4int16_t irango)
{
  //## begin ClLog::STARTCODE%-73387955.body preserve=yes

	M4ClCrono	crono;
	ClLogManager	*plogManager;


	if ( m_bcrono )
		crono.Start();

	if ( ! ( ( plogManager = GETLOGMANAGER() ) -> TestErrorMode( irango ) ) )
	{
		if ( m_bcrono )
		{
			plogManager -> AddTime( crono.Stop() );
		}

		return M4_ERROR;
	}

	if ( ! m_plogSys )
	{
		if ( ! ( plogManager ) )
		{
			if ( m_bcrono )
			{
				plogManager -> AddTime( crono.Stop() );
			}
			return M4_ERROR;
		}
		else
		{
			switch ( m_slogMode )
			{
			case MODE_LOGOWNER:
				m_plogSys = GETNEWLOGSYSTEM();
				break;
			case MODE_LOGREFERENCE:
				m_plogSys = NULL;
				if ( m_bcrono )
				{
					plogManager -> AddTime( crono.Stop() );
				}
				return M4_ERROR;
				break;
			default:
				if ( ! ( m_plogSys = GETPTHREADLOGSYSTEM() ) )
				{
					if ( m_bcrono )
					{
						plogManager -> AddTime( crono.Stop() );
					}
					return M4_ERROR;
				}
				break;
			}
		}
	}

	if ( m_plogSys -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}
	if ( m_bcrono )
	{
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;

  //## end ClLog::STARTCODE%-73387955.body
}

m4int16_t ClLog::ADDTEXT (m4pchar_t textoError)
{
  //## begin ClLog::ADDTEXT%1792325199.body preserve=yes

	if ( ! m_plogSys )
	{
		return M4_ERROR;
	}

	M4ClCrono	crono;
	ClLogManager	*plogManager = GETLOGMANAGER();

	if ( m_bcrono )
		crono.Start();
	
	m_plogSys -> AddText( textoError );

	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}

	return M4_SUCCESS;

  //## end ClLog::ADDTEXT%1792325199.body
}

m4int16_t ClLog::ADDTEXTF (m4pchar_t cadFormato, ... )
{
  //## begin ClLog::ADDTEXTF%-1259627095.body preserve=yes


	if ( ! m_plogSys )
		return M4_ERROR;

	M4ClCrono	crono;
	ClLogManager	*plogManager = GETLOGMANAGER();
	
	if ( ! (m_plogSys -> IsActive() ) )
	{
		return M4_ERROR;
	}

	if ( m_bcrono )
		crono.Start();

	va_list marker;
	m4char_t	buffer[3200];
	M4ClString	smessage;

	va_start( marker, cadFormato );     /* Initialize variable arguments. */

	vsprintf( buffer, cadFormato, marker );

	smessage = buffer;

	if ( m_plogSys -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = m_plogSys -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;
		      ipos != M4_STR_NPOS
					; ipos = smessage.find( RETORNO_CARRO ) )
		{
			smessage.replace( ipos, 1, ncret );
		}
	}
	if ( m_plogSys -> AddText( smessage ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;

  //## end ClLog::ADDTEXTF%-1259627095.body
}

m4int16_t ClLog::SETCODE ()
{
  //## begin ClLog::SETCODE%-2103572020.body preserve=yes


	if ( ! m_plogSys )
		return M4_ERROR;
	
	M4ClCrono	crono;
	ClLogManager *plogManager = GETLOGMANAGER();

	if ( m_bcrono )
		crono.Start();

	if ( m_plogSys -> SetCode( ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( m_plogSys -> StorageAlways() )
	{
		m_plogSys -> StoreAppoints( ALL_STORAGES );
	}

	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;

  //## end ClLog::SETCODE%-2103572020.body
}

m4int16_t ClLog::SETCODE (m4int32_t icode, m4int16_t irango, m4pcchar_t pmessage) const
{
  //## begin ClLog::SETCODE%-1864508977.body preserve=yes

	M4ClCrono	crono;
	ClLogManager *plogManager = GETLOGMANAGER();
	ClLogSystem	*plogSys;

	if ( ! plogManager )
		return M4_ERROR;

	if ( m_bcrono )
		crono.Start();

//	if ( ! ( plogManager -> TestErrorMode( irango ) ) )
//	{
//		if ( m_bcrono )
//		{	
//			plogManager -> AddTime( crono.Stop() );
//		}
//
//		return M4_ERROR;
//	}

	
	if ( ! m_plogSys )
	{
		if ( m_slogMode == MODE_LOGTHREAD )
		{
			plogSys = GETPTHREADLOGSYSTEM();
		}
		else
		{
			if ( m_bcrono )
			{	
				plogManager -> AddTime( crono.Stop() );
			}
			return M4_ERROR;
		}
	}
	else
	{
		plogSys = m_plogSys;
	}
	if ( plogSys -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}


	M4ClString	smessage;

	if ( pmessage == NULL )
		smessage = "";
	else
		smessage = pmessage;
	
	if ( plogSys -> AddText( smessage ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( plogSys -> SetCode() == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( plogSys -> StorageAlways() )
	{
		plogSys -> StoreAppoints( ALL_STORAGES );
	}


	
	if ( m_bcrono )
	{	
		plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;

  //## end ClLog::SETCODE%-1864508977.body
}

m4int16_t ClLog::SETCODEF (m4int32_t icode, m4int16_t irango, m4pcchar_t cadFormato, ... ) const
{
  //## begin ClLog::SETCODEF%1594254150.body preserve=yes


	ClLogManager *plogManager = GETLOGMANAGER();
	if ( ! plogManager )
		return M4_ERROR;

//	if ( ! ( plogManager -> TestErrorMode( irango ) ) )
//		return M4_ERROR;

	M4ClCrono	crono;
	ClLogSystem	*plogSys;

	if ( m_bcrono )
		crono.Start();


	if ( ! m_plogSys )
	{
		if ( m_slogMode == MODE_LOGTHREAD )
		{
			plogSys = GETPTHREADLOGSYSTEM();
		}
		else
		{
			if ( m_bcrono )
			{	
				plogManager -> AddTime( crono.Stop() );
			}
			return M4_ERROR;
		}
	}
	else
	{
		plogSys = m_plogSys;
	}
		

	if ( plogSys -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}


	va_list marker;
	m4char_t	buffer[3200];
	M4ClString	smessage;

	if ( cadFormato )
	{
		va_start( marker, cadFormato );     /* Initialize variable arguments. */

		vsprintf( buffer, cadFormato, marker );
		va_end( marker );
	}
	else
	{
		strcpy( buffer, "" );
	}
	smessage = buffer;

	m4int32_t	iret1 = plogManager -> GetCReturn();
	m4int32_t	iret2 = RETORNO_CARRO;
	if ( plogManager -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = plogManager -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;
		      ipos != M4_STR_NPOS
					; ipos = smessage.find( RETORNO_CARRO ) )
		{
			smessage.replace( ipos, 1, ncret );
		}
	}

	
	if ( plogSys -> AddText( smessage ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;	
	}
	
	if ( plogSys -> SetCode( ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( plogSys -> StorageAlways() )
	{
		plogSys -> StoreAppoints( ALL_STORAGES );
	}

	
	if ( m_bcrono )
	{	
		plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}

	return M4_SUCCESS;	

  //## end ClLog::SETCODEF%1594254150.body
}

m4int16_t ClLog::SETCODEVF (m4int32_t icode, m4int16_t irango, m4pcchar_t cadFormato, va_list marker) const
{
  //## begin ClLog::SETCODEVF%1850331264.body preserve=yes


	ClLogManager *plogManager = GETLOGMANAGER();
	if ( ! plogManager )
		return M4_ERROR;

//	if ( ! ( plogManager -> TestErrorMode( irango ) ) )
//		return M4_ERROR;

	M4ClCrono	crono;
	ClLogSystem	*plogSys;

	if ( m_bcrono )
		crono.Start();


	if ( ! m_plogSys )
	{
		if ( m_slogMode == MODE_LOGTHREAD )
		{
			plogSys = GETPTHREADLOGSYSTEM();
		}
		else
		{
			if ( m_bcrono )
			{	
				plogManager -> AddTime( crono.Stop() );
			}
			return M4_ERROR;
		}
	}
	else
	{
		plogSys = m_plogSys;
	}
		

	if ( plogSys -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}


	m4char_t	buffer[3200];
	M4ClString	smessage;

	if ( cadFormato )
	{
		vsprintf( buffer, cadFormato, marker );
	}
	else
	{
		strcpy( buffer, "" );
	}
	smessage = buffer;

	m4int32_t	iret1 = plogManager -> GetCReturn();
	m4int32_t	iret2 = RETORNO_CARRO;
	if ( plogManager -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = plogManager -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;
		      ipos != M4_STR_NPOS
					; ipos = smessage.find( RETORNO_CARRO ) )
		{
			smessage.replace( ipos, 1, ncret );
		}
	}

	
	if ( plogSys -> AddText( smessage ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;	
	}
	
	if ( plogSys -> SetCode( ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}

	if ( plogSys -> StorageAlways() )
	{
		plogSys -> StoreAppoints( ALL_STORAGES );
	}

	
	if ( m_bcrono )
	{	
		plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}

	return M4_SUCCESS;	

  //## end ClLog::SETCODEVF%1850331264.body
}

m4int16_t ClLog::SETSERIALCODE (m4int32_t ai_Size, m4pchar_t ai_SerialCad)
{
  //## begin ClLog::SETSERIALCODE%1736846040.body preserve=yes
		return 0;
  //## end ClLog::SETSERIALCODE%1736846040.body
}

m4int16_t ClLog::SETSERIALCODESTRING (m4pchar_t ai_SerialCad)
{
  //## begin ClLog::SETSERIALCODESTRING%1886185757.body preserve=yes
		return 0;
  //## end ClLog::SETSERIALCODESTRING%1886185757.body
}

m4int16_t ClLog::ACTIVATE_LOGUNIT ()
{
  //## begin ClLog::ACTIVATE_LOGUNIT%192635576.body preserve=yes
		return 0;
  //## end ClLog::ACTIVATE_LOGUNIT%192635576.body
}

m4int16_t ClLog::DEACTIVATE_LOGUNIT ()
{
  //## begin ClLog::DEACTIVATE_LOGUNIT%-1106997895.body preserve=yes
	return 0;
  //## end ClLog::DEACTIVATE_LOGUNIT%-1106997895.body
}

m4int16_t ClLog::STORELOGS (m4int8_t sstorages)
{
  //## begin ClLog::STORELOGS%-341751241.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	M4ClCrono	crono;
	if ( m_bcrono )
		crono.Start();

	if ( m_plogSys -> StoreAppoints( sstorages ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}
	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;


  //## end ClLog::STORELOGS%-341751241.body
}

m4int16_t ClLog::TRASPASEAPPOINTS (ClLogSystem* plogUnit)
{
  //## begin ClLog::TRASPASEAPPOINTS%407394338.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	M4ClCrono	crono;
	ClLogManager	*plogManager = GETLOGMANAGER();
	if ( m_bcrono )
		crono.Start();

	if ( m_plogSys -> AddAppoints( plogUnit -> GetAppointments() )
							== M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return M4_ERROR;
	}
	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return M4_SUCCESS;

  //## end ClLog::TRASPASEAPPOINTS%407394338.body
}

m4double_t ClLog::TOTALTIME ()
{
  //## begin ClLog::TOTALTIME%1205405339.body preserve=yes

	return ::TOTALTIME();

  //## end ClLog::TOTALTIME%1205405339.body
}

m4double_t ClLog::UNITTIME ()
{
  //## begin ClLog::UNITTIME%-1944858855.body preserve=yes

	if ( ! m_plogSys )
		return 0;
	return m_plogSys -> UnitTime();

  //## end ClLog::UNITTIME%-1944858855.body
}

m4return_t ClLog::GETH_ERRORTYPE (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize)
{
  //## begin ClLog::GETH_ERRORTYPE%1988868113.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	
	return m_plogSys -> GetErrorType( aio_szBuffer, ai_lBufferSize, ao_lBufferSize );



  //## end ClLog::GETH_ERRORTYPE%1988868113.body
}

m4return_t ClLog::GETH_ERRORCODE (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize)
{
  //## begin ClLog::GETH_ERRORCODE%289729613.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	
	return m_plogSys -> GetErrorCode( aio_szBuffer, ai_lBufferSize, ao_lBufferSize );


  //## end ClLog::GETH_ERRORCODE%289729613.body
}

m4return_t ClLog::GETH_ERRORTIME (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize)
{
  //## begin ClLog::GETH_ERRORTIME%-1679065199.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();

	return m_plogSys -> GetErrorTime( aio_szBuffer, ai_lBufferSize, ao_lBufferSize );

  //## end ClLog::GETH_ERRORTIME%-1679065199.body
}

m4return_t ClLog::GETH_ERRORTEXT (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize)
{
  //## begin ClLog::GETH_ERRORTEXT%1228860288.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	
	return m_plogSys -> GetErrorText( aio_szBuffer, ai_lBufferSize, ao_lBufferSize );

  //## end ClLog::GETH_ERRORTEXT%1228860288.body
}

m4return_t ClLog::POP ()
{
  //## begin ClLog::POP%-1582425221.body preserve=yes

	if ( ! m_plogSys )
		return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	
	return m_plogSys -> Pop();

  //## end ClLog::POP%-1582425221.body
}

m4bool_t ClLog::EMPTY ()
{
  //## begin ClLog::EMPTY%281501670.body preserve=yes

	if ( ! m_plogSys )
		return M4_TRUE;	//return M4_ERROR;

	ClLogManager	*plogManager = GETLOGMANAGER();
	
	return m_plogSys -> Empty();

  //## end ClLog::EMPTY%281501670.body
}

m4int16_t ClLog::GetDistance (m4pchar_t& cad, m4uint32_t Size)
{
  //## begin ClLog::GetDistance%-1294852202.body preserve=yes
		return 0;
  //## end ClLog::GetDistance%-1294852202.body
}

ClLog* ClLog::operator += (ClLog* plog)
{
  //## begin ClLog::operator +=%1641217550.body preserve=yes

	if ( ! m_plogSys )
		return NULL;

	M4ClCrono	crono;
	ClLogManager	*plogManager;

	plogManager = GETLOGMANAGER();
	if ( ! plogManager )
		return NULL;

	if ( m_bcrono )
		crono.Start();

	if ( ( m_plogSys -> AddAppoints( plog -> GETPLOG() -> GetAppointments() ) ) == M4_ERROR )
	{
		if ( m_bcrono )
		{	
			m_plogSys -> AddTime( crono.Pause() );
			plogManager -> AddTime( crono.Stop() );
		}
		return NULL;
	}

	if ( m_bcrono )
	{	
		m_plogSys -> AddTime( crono.Pause() );
		plogManager -> AddTime( crono.Stop() );
	}
	return this;

  //## end ClLog::operator +=%1641217550.body
}

void ClLog::SETPLOG (ClLogSystem* plog)
{
  //## begin ClLog::SETPLOG%1904179874.body preserve=yes

	if ( m_plogSys )
	{
		if ( m_slogMode == MODE_LOGOWNER )
		{
			delete m_plogSys;
		}
	}
	m_plogSys = plog;
	m_slogMode = MODE_LOGREFERENCE;

  //## end ClLog::SETPLOG%1904179874.body
}

ClLogSystem* ClLog::GETNEWLOGSYSTEM (void )
{
  //## begin ClLog::GETNEWLOGSYSTEM%1820130930.body preserve=yes

	M4ClCrono	crono;

	ClLogManager *plogManager = NULL;
	ClLogSystem	*plogSystem = NULL;


	if ( !( plogManager = GETLOGMANAGER() ) )
	{
		return NULL;
	}

	if ( m_bcrono )
		crono.Start();


	plogSystem = new ClLogSystem( plogManager, plogManager -> getShowTime() );

	if ( m_bcrono )
	{
		plogManager -> AddTime( crono.Stop() );
	}
	return plogSystem;

  //## end ClLog::GETNEWLOGSYSTEM%1820130930.body
}

// Additional Declarations
  //## begin ClLog%377C88510014.declarations preserve=yes
  //## end ClLog%377C88510014.declarations

//## begin module%377C885701E0.epilog preserve=yes
//## end module%377C885701E0.epilog
