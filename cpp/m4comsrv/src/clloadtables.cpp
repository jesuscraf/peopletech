//## begin module%351B789A022F.cm preserve=no
//## end module%351B789A022F.cm

//## begin module%351B789A022F.cp preserve=no
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
//## end module%351B789A022F.cp

//## Module: ClLoadTables%351B789A022F; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clloadtables.cpp

//## begin module%351B789A022F.additionalIncludes preserve=no
//## end module%351B789A022F.additionalIncludes

//## begin module%351B789A022F.includes preserve=yes
#include <fileutil.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <m4date.hpp>
#include <propertiestable.hpp>
//## end module%351B789A022F.includes

// ClLoadTables
#include <clloadtables.hpp>
//## begin module%351B789A022F.declarations preserve=no
//## end module%351B789A022F.declarations

//## begin module%351B789A022F.additionalDeclarations preserve=yes
#include <cloblconfiguration.hpp>
//## end module%351B789A022F.additionalDeclarations


// Class ClLoadTables 


//## Other Operations (implementation)
m4return_t ClLoadTables::LoadIds ()
{
  //## begin ClLoadTables::LoadIds%890993633.body preserve=yes
	UINT hHandle ;
	UINT hProp ;
	UINT hMatch ;
	m4return_t iRet ;
	m4char_t szName [50] ;
	m4char_t szValue [OBL_PATHVALUE_MAX_LEN ] ;
	UINT iLenValue , iLenName ;


	hHandle = m_oTable -> GetHandle ( ) ;

	iRet = M4AOblFirstHndProp ( hHandle , ATTRIBUTES_IDS , "*" , & hProp , &hMatch ) ;

	if (iRet != OBL_SUCCESS )
	{
		return M4_ERROR;
	}

	if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read property id: %s " ,szName ) ;
		return M4_ERROR ;
	}

	while (iRet == OBL_SUCCESS )
	{
		m_oIds.SetProperty ( szName ,szValue , strlen ( szValue ) ) ;

		iRet = M4AOblNextHndProp ( hHandle , hMatch , & hProp ) ;
		

		if ( iRet == OBL_SUCCESS )
		{

			if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
			{
				SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read obl property id: %s." ,szName ) ;
				return M4_ERROR ;
			}
			
		}
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::LoadIds%890993633.body
}

m4return_t ClLoadTables::LoadTypes ()
{
  //## begin ClLoadTables::LoadTypes%890993634.body preserve=yes
	UINT hHandle ;
	UINT hProp ;
	UINT hMatch ;
	m4return_t iRet ;
	m4char_t szName [50] ;
	m4char_t szValue [OBL_PATHVALUE_MAX_LEN] ;
	UINT iLenValue , iLenName ;


	hHandle = m_oTable -> GetHandle ( ) ;

	iRet = M4AOblFirstHndProp ( hHandle , ATTRIBUTES_TYPE , "*" , & hProp , &hMatch ) ;

	if (iRet != OBL_SUCCESS )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read subsystem type: %s " ,szName ) ;
		return M4_ERROR ;
	}

	while (iRet == OBL_SUCCESS )
	{
		m_oTypes.SetProperty ( szName , szValue , strlen ( szValue) ) ;	


		iRet = M4AOblNextHndProp ( hHandle , hMatch , & hProp ) ;
		

		if ( iRet == OBL_SUCCESS )
		{

			if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
			{
				SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read subsystem type %s." ,szName ) ;
				return M4_ERROR ;
			}
			
		}
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::LoadTypes%890993634.body
}

m4return_t ClLoadTables::LoadScale ()
{
  //## begin ClLoadTables::LoadScale%890993635.body preserve=yes
	UINT hHandle ;
	UINT hProp ;
	UINT hMatch ;
	m4return_t iRet ;
	m4char_t szName [50] ;
	m4char_t szValue [OBL_PATHVALUE_MAX_LEN] ;
	UINT iLenValue , iLenName ;


	hHandle = m_oTable -> GetHandle ( ) ;

	iRet = M4AOblFirstHndProp ( hHandle , ATTRIBUTES_SCALE , "*" , & hProp , &hMatch ) ;

	if (iRet != OBL_SUCCESS )
	{
		return M4_ERROR;
	}

	if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read property scale: %s " ,szName ) ;
		return M4_ERROR ;
	}

	while (iRet == OBL_SUCCESS )
	{
		m_oScales.SetProperty ( szName , szValue , strlen ( szValue ) ) ;

		iRet = M4AOblNextHndProp ( hHandle , hMatch , & hProp ) ;
		

		if ( iRet == OBL_SUCCESS )
		{

			if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
			{
				SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read property scale: %s" ,szName ) ;
				return M4_ERROR ;
			}
			
		}
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::LoadScale%890993635.body
}

m4return_t ClLoadTables::Initialize (m4pchar_t ai_szTables, m4pchar_t ai_szLiterals)
{
  //## begin ClLoadTables::Initialize%890993636.body preserve=yes
	


	return M4_SUCCESS ; 
  //## end ClLoadTables::Initialize%890993636.body
}

m4return_t ClLoadTables::LoadLiterals (m4pchar_t ai_szLiterals)
{
  //## begin ClLoadTables::LoadLiterals%890993637.body preserve=yes
	UINT hHandle ;
	UINT hProp ;
	UINT hMatch ;
	m4return_t iRet ;
	m4uint32_t iFileLen ;
	m4char_t szName [50] ;
	m4char_t szValue [OBL_PATHVALUE_MAX_LEN] ;
	UINT iLenValue , iLenName ;
//	ClOblConfiguration * poLiterals = new ClOblConfiguration ;
	ClExObl * poLiterals = new ClExObl ;
	size_t iLen ;
	m4char_t szInstance [OBL_PROPVALUE_MAX_LEN ] ;


	GetSizeFile ( ai_szLiterals , iFileLen ) ;

	iRet =  poLiterals  -> Init ( ai_szLiterals , iFileLen + 100 , iFileLen + 100)  ;


	if ( iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Error at literals obl: %s" , ai_szLiterals ) ;

		cout << "Error at literals obl: " << ai_szLiterals << flush ;
		return M4_ERROR ;
	}


	if ( M4_ERROR == poLiterals -> GetInstance ( szInstance , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		return M4_ERROR ;
	}

	if (  stricmp( szInstance , LITERALS_INSTANCE ) )
	{
		cout << ai_szLiterals<<": Bad OBL instance" << flush ;

		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "%s:: Bad OBL instance" , ai_szLiterals ) ;

		m4sleep (3 ) ;

		return  M4_ERROR ;
	}


	hHandle = poLiterals -> GetHandle ( ) ;

	iRet = M4AOblFirstHndProp ( hHandle , "" , "*" , & hProp , &hMatch ) ;

	if (iRet != OBL_SUCCESS )
	{
		return M4_ERROR;
	}

	if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read property literal: %s, in file %s." ,szName , ai_szLiterals) ;

		return M4_ERROR ;
	}


	while (iRet == OBL_SUCCESS )
	{
		m_oLiterals.SetProperty ( szName ,szValue , strlen ( szValue ) ) ;

		iRet = M4AOblNextHndProp ( hHandle , hMatch , & hProp ) ;
		

		if ( iRet == OBL_SUCCESS )
		{

			if ( M4_ERROR == ( iRet = M4AOblGetPropHandle ( hHandle , hProp , szName , 50 , &iLenName , szValue , OBL_PATHVALUE_MAX_LEN ,&iLenValue ) ) )
			{
				SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't read property literal: %s, in file %s." ,szName , ai_szLiterals) ;
				return M4_ERROR ;
			}
			
		}
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::LoadLiterals%890993637.body
}

m4return_t ClLoadTables::InitializeLiterals (m4pchar_t ai_szLiterals)
{
  //## begin ClLoadTables::InitializeLiterals%944061593.body preserve=yes
	if ( M4_ERROR == LoadLiterals ( ai_szLiterals ) )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS;
  //## end ClLoadTables::InitializeLiterals%944061593.body
}

m4return_t ClLoadTables::InitializeProperties (m4pchar_t ai_szProperties)
{
  //## begin ClLoadTables::InitializeProperties%944061594.body preserve=yes
	size_t iLen ;
	m4uint32_t iFileLen ;
	m4return_t iRet ;
	m4char_t szInstance [OBL_PROPVALUE_MAX_LEN ] ;
	


	if ( M4_ERROR ==  GetSizeFile ( ai_szProperties, iFileLen ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't open obl file %s. File Does not exist" , ai_szProperties) ;
		cout << "Couldn't open obl file " << ai_szProperties<<". File Does not exist" << flush ;
		return M4_ERROR ;
	}

	iRet =  m_oBook -> Init ( ai_szProperties, iFileLen + 100 , iFileLen + 100 ) ;

	if ( iRet == M4_ERROR )
	{
		cout << "Couldn't open obl file " << ai_szProperties<<". Bad OBL format" << flush ;

		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't open obl file %s. Bad OBL format" , ai_szProperties) ;

		m4sleep (3 ) ;

		return  M4_ERROR ;
	}

	if ( M4_ERROR == m_oBook -> GetInstance ( szInstance , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		return M4_ERROR ;
	}

	if (  stricmp( szInstance , PROPERTIES_INSTANCE ) )
	{
		cout << ai_szProperties<<": Bad OBL instance" << flush ;

		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "%s:: Bad OBL instance" , ai_szProperties) ;

		m4sleep (3 ) ;

		return  M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::InitializeProperties%944061594.body
}

m4return_t ClLoadTables::InitializeTables (m4pchar_t ai_szTables)
{
  //## begin ClLoadTables::InitializeTables%944061595.body preserve=yes
	size_t iLen ;
	m4uint32_t iFileLen ;
	m4return_t iRet ;
	m4char_t szInstance [OBL_PROPVALUE_MAX_LEN ] ;
	


	if ( M4_ERROR ==  GetSizeFile ( ai_szTables , iFileLen ) )
	{
		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't open obl file %s. File Does not exist" , ai_szTables ) ;
		cout << "Couldn't open obl file " << ai_szTables <<". File Does not exist" << flush ;
		return M4_ERROR ;
	}

	iRet =  m_oTable -> Init ( ai_szTables , iFileLen + 100 , iFileLen + 100 ) ;

	if ( iRet == M4_ERROR )
	{
		cout << "Couldn't open obl file " << ai_szTables <<". Bad OBL format" << flush ;

		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "Couldn't open obl file %s. Bad OBL format" , ai_szTables ) ;

		m4sleep (3 ) ;

		return  M4_ERROR ;
	}

	if ( M4_ERROR == m_oTable -> GetInstance ( szInstance , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		return M4_ERROR ;
	}

	if (  stricmp( szInstance , TABLES_INSTANCE ) )
	{
		cout << ai_szTables <<": Bad OBL instance" << flush ;

		SETCODEF (	M4_SRV_SS_OBL_ERROR , ERRORLOG , "%s:: Bad OBL instance" , ai_szTables ) ;

		m4sleep (3 ) ;

		return  M4_ERROR ;
	}

	if ( M4_ERROR ==LoadIds ( ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == LoadTypes ( ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == LoadScale ( ) )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLoadTables::InitializeTables%944061595.body
}

// Additional Declarations
  //## begin ClLoadTables%351B70E6017F.declarations preserve=yes
  //## end ClLoadTables%351B70E6017F.declarations

//## begin module%351B789A022F.epilog preserve=yes
//## end module%351B789A022F.epilog
