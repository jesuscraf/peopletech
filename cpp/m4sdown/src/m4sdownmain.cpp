//==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            AdminmonClient
//	 File:              servershutdown.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This program stop the server using the administrator
//		capabilities.
//
//=============================================================================

#include <stdio.h>
#include <m4types.hpp>
#include <m4cryptstore.hpp>  
#include <m4win.hpp>
#include <tsap.hpp>
#include <clientapi.hpp>
#include <adminmoncmdsid.hpp>
#include <oblutil.hpp>
#include <m4date.hpp>
#include <clienttag.hpp>
#include <fileutil.hpp>
#include "m4xmlreg.hpp"
#include "basepdus.hpp"



m4return_t DecryptUserPassword ( m4pchar_t ai_szFile , m4pchar_t ao_szUser , m4int_t , m4pchar_t ao_szPassword , m4int_t) ;

#define ADMIN_PORT_OFFSET	2 

m4return_t GetOblPort ( m4pchar_t szObl, m4uint32_t & iPort ) ;
void Usage ( m4pchar_t ai_szProc ) ;


int main(int argc, char* argv[])
{
	m4uint32_t iPort = 0 ;
	m4uint32_t iBasePort = 0 ;
	m4return_t retCode;
	M4ClString strUser ;
	M4ClString strPassword ;
	M4ClString strHost;
	m4pchar_t szUser  = NULL ;
	m4pchar_t szPassword = NULL ;
	m4pchar_t szObl = NULL ;
	ClTagList otagList;

	m4pchar_t szHost = NULL ;
	m4pchar_t szFile = NULL ;
	m4uint32_t index = 0 ;

	/* Bug 0113374
	Se establece el número de descriptores a 1024
	*/
	SetFileDescriptorsLimit( 1024 ) ;

	//-----	------------------------------------------------------------------
	// check input parameters.
	//-----------------------------------------------------------------------
	while(argv[index] )
	{
		if ( argv[index][0]=='-' )
		{
			switch ( argv[index][1] )
			{
			case 'o' :
			case 'O' :
				if ( ! stricmp ( "OBL" , &argv[index][1] ) )
				{
					argc--;
					if (  argv[++index] )
					{
						if (  argv[index][0] != '-' )
						{
							szObl = new m4char_t [ strlen (argv [index ] ) +1 ] ;
							strcpy ( szObl , argv[index++] );
							argc--;
						}
					}
				}
				break ;
			case 'h' :
			case 'H' :
				if ( ! stricmp ( "host" , &argv[index][1] ) )
				{
					argc--;
					if (  argv[++index] )
					{
						if (  argv[index][0] != '-' )
						{
							szHost = new m4char_t [ strlen (argv [index ] ) +1 ] ;
							strcpy ( szHost , argv[(index)++] );
							argc--;
						}
					}
				}
				break ;
			case 'f' :
			case 'F' :
				if ( ! stricmp ( "file" , &argv[index][1] ) )
				{
					argc--;
					if (  argv[++index] )
					{
						if (  argv[index][0] != '-' )
						{
							szFile = new m4char_t [ strlen (argv [index] ) +1 ] ;
							strcpy ( szFile , argv[(index)++] );
							argc--;
						}
					}
				}
				break ;
			case 'p' :
			case 'P' :
				if ( ! stricmp ( "password" , &argv[index][1] ) )
				{
					argc--;
					if (  argv[++index] )
					{
						if (  argv[index][0] != '-' )
						{
							szPassword = new m4char_t [ strlen (argv [index] ) +1 ] ;
							strcpy ( szPassword , argv[index++] );
							argc--;
						}
					}
				}
				break ;
			case 'b' :
			case 'B' :
				if ( ! stricmp ( "baseport" , &argv[index][1] ) )
				{
					++index ;
					argc--;
					if (  argv[index] )
					{
						if (  argv[index][0] != '-' )
						{
							if ( iBasePort = atoi ( argv[index] ) )
							{
								argc--;
								index++;
							}
						}
					}
					break ;
				}
				break ;
			case 'u' :
			case 'U' :

				if ( ! stricmp ( "user" , &argv[index][1] ) )
				{
					argc--;
					if (  argv[++index] )
					{
						if (  argv[index][0] != '-' )
						{
							szUser = new m4char_t [ strlen (argv [index] ) +1 ] ;
							strcpy ( szUser , argv[index++] );
							argc--;
						}
						break ;
					}
				} ;
				break ;
			default :
				break;
			}

		}
		else index ++ ;
	}

	if ( (!szFile ) && ( ! ( szUser && szPassword )) )
	{
		cout << "Incompatible parameters: file AND ( user OR password) \n" <<flush ;
		Usage (argv[0]) ;
	}

	if ( szFile && ((  szUser ) || ( szPassword )) )
	{
		cout << "Incompatible parameters: file AND ( user OR password) \n" <<flush ;
		Usage (argv[0]) ;
	}
	else
	{
		if (szFile )
		{
			szUser =new m4char_t [300];
			szPassword = new m4char_t [300];

			if (M4_SUCCESS != DecryptUserPassword (szFile , szUser ,300,szPassword , 300) )
			{
				cout << endl << "Error getting user or password from file: " << szFile << endl ;			
					flush (cout ) ;

				m4sleep ( 4) ;

				return -1 ;
			}
		}
	}

	if ( !szObl && !iBasePort )
	{
		cout << "Incompatible parameters: Neither obl  Nor baseport\n" <<flush ;
		Usage (argv[0]) ;
	}

	if ( ! iBasePort )
	{
		if ( M4_ERROR == GetOblPort ( szObl, iBasePort ) )
		{
			cout <<"Error reading ADMIN_ACEPTOR_PORT from :" << szObl <<" file" ;
			m4sleep ( 4  ) ;
			return -1 ;
		}

	}

	iPort = iBasePort + ADMIN_PORT_OFFSET;	

	if ( ! szHost)
	{
		szHost = new m4char_t [ 10] ;
		strcpy ( szHost , "localhost" ) ;
	}

	cout << "Shutting down M4 Mind Server..." << endl;

	// Bug 0203364. Init registry.
	M4XmlRegistry::Instance();

	//-----------------------------------------------------------------------
	// create administrator.
	//-----------------------------------------------------------------------
	ClClientAPI	administrator("");
	administrator.Init();
	m4handle_t adminSessionId = 0;
	otagList.SetArg ("M4_ADMINISTRATION_SERVICE", "M4_ADMINISTRATION_SERVICE");

	// Bug 0203364. Set client encoding.
	otagList.SetArg ("M4_SET_CLIENT_ENCODING", 1);
	
	//-----------------------------------------------------------------------
	// open the session.
	//-----------------------------------------------------------------------
	strHost = szHost ;
	strUser = szUser ;
	strPassword = szPassword ;

	retCode = administrator.OpenSession(strHost, iPort, strUser, strPassword, M4_TRUE , adminSessionId, &otagList);
	if (M4_SUCCESS != retCode)
	{
		cerr << "Can't OpenSession" << endl;
		m4sleep (4 ) ;
		return M4_ERROR;
	}
	
	//-----------------------------------------------------------------------
	// open the administration service.
	//-----------------------------------------------------------------------
	M4ClString szService = "ADMIN" ;
	m4handle_t hContext = 0; 
	retCode = administrator.OpenService(adminSessionId, szService, hContext);
	if (M4_SUCCESS != retCode)
	{
		cerr << "Can't OpenService" << endl;
		administrator.CloseSession(adminSessionId);
		m4sleep (4 ) ;
		return M4_ERROR;
	}

	//-----------------------------------------------------------------------
	// enable the administrator role.
	//-----------------------------------------------------------------------
	retCode = administrator.EnableRole(adminSessionId);
	if (M4_SUCCESS != retCode)
	{
		cerr << "Can't EnableRole" << endl;
		administrator.CloseSession(adminSessionId);
		m4sleep (4 ) ;
		return M4_ERROR;
	}
	
	//-----------------------------------------------------------------------
	// send the shutdown command.
	//-----------------------------------------------------------------------
	//m4int32_t iAnswerSize = 1024;
	//m4char_t szAnswer[iAnswerSize];
	const m4pchar_t szCommand = M4_ADM_SHUTDOWN;
	//retCode = administrator.Execute(szCommand, szAnswer, iAnswerSize-1);


	m4int32_t iStateFlags = 0;
	m4int32_t iFlags = 0;
	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage(); 
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage(); 
	ClDataUnit *poInputDataUnit;
	m4handle_t hRequestId;	
	m4pchar_t pBufferData = 0;

	// build the dataStorage.
	poInputDataStorage->GetNewDataUnit(poInputDataUnit);
	if (NULL == poInputDataUnit)
	{
		cerr << "Can't build request." << endl;
		administrator.CloseSession(adminSessionId);
		m4sleep (4 ) ;
		return M4_ERROR;
	}
	poInputDataUnit->SetSize(strlen(szCommand)+1);
	strcpy(poInputDataUnit->GetBuffer(), szCommand);

	retCode = administrator.SyncRequest(adminSessionId, hContext, 
		iStateFlags, iFlags, 
		poInputDataStorage, poOutputDataStorage, hRequestId);


	//-----------------------------------------------------------------------
	// close the session.
	// there is not necessary, due to the server is down.
	//-----------------------------------------------------------------------
	/*
	retCode = administrator.CloseSession(adminSessionId);
	if (M4_SUCCESS != retCode)
	{
		cerr << "Can't CloseSession" << endl;
		return M4_ERROR;
	}
	*/
	
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	cout << "Shutdown executed. Wait for Meta4 Mind Application Server to finish" << endl;
	return 0;

}


m4return_t GetOblPort ( m4pchar_t szObl, m4uint32_t & iPort ) 
{
	m4pchar_t szPort = new m4char_t [100];
	m4pchar_t szProp = NULL ;
	ClOblUtil oObl ( szObl ) ;

	if ( oObl.Error () )
	{
		delete szPort ;
		return M4_ERROR ;
	}

	if ( M4_ERROR == oObl.GetProperty ( "BASE_PORT" , szPort , 100 ) ) 
	{
		delete szPort ;
		return M4_ERROR ;
	}

	if (! ( iPort = atoi ( szPort ) ) )
	{
		delete szPort ;
		return M4_ERROR ;
	}


	delete szPort ;

	return M4_SUCCESS ;
}

void Usage ( m4pchar_t ai_szProc )
{

		cerr << "Usage: " << endl;
		cerr << "Either   " << ai_szProc<< " [-host serverHostName] -baseport portNumber -user user -password password" << endl;
		cerr << "Or       " << ai_szProc<< " [-host serverHostName] -baseport portNumber -file passwords-file" << endl;
		cerr << "Or		  " << ai_szProc<< " [-host serverHostName] -obl startup.obl -user user -password password" << endl;
		cerr << "Or       " << ai_szProc<< " [-host serverHostName] -obl startup.obl -file passwords-file" << endl;
		cerr << "Or		  " << ai_szProc<< " [-host serverHostName] -baseport portNumber -obl startup.obl -user user -password password" << endl;
		cerr << "Or       " << ai_szProc<< " [-host serverHostName] -baseport portNumber -obl startup.obl -file passwords-file" << endl;

		exit ( -1 );
}


m4return_t DecryptUserPassword ( m4pchar_t ai_szFile , m4pchar_t ao_szUser , m4int_t ai_iBuffUser , m4pchar_t ao_szPassword , m4int_t ai_iBuffPassword )
{
	m4char_t szPin [200];
	ClCryptStore * cStore;
	m4return_t iRet ;
	m4int_t iUserLen , iPasswordLen ;
	
#ifdef _WINDOWS
	SocketInit ( ) ;
	//Conseguir el Pin
	gethostname ( szPin, 200 );
#else
    m4uint32_t hostid = gethostid ( ) ;
	sprintf ( szPin , "%u" , hostid ) ;
#endif


	cStore = new ClCryptStore ( ai_szFile , szPin , strlen (szPin) ) ;


		iRet = cStore -> ExtractData( "USER", 4,
			ao_szUser,ai_iBuffUser , iUserLen );
		if ( iRet == M4_ERROR )
			return M4_ERROR ;

		ao_szUser [iUserLen ] = 0 ;

		if ( iRet != M4_SUCCESS )
		{
			printf ( "\nUSER not set in file : %s\n" , ai_szFile ) ;

			return M4_ERROR ;
		}

		iRet = cStore -> ExtractData( "PASSWORD", 8,
			ao_szPassword,ai_iBuffPassword , iPasswordLen);
		if (iRet == M4_ERROR )
			return M4_ERROR ;

		ao_szPassword [iPasswordLen ] = 0 ;

		if ( iRet != M4_SUCCESS )
		{
			printf ( "\nPASSWORD not set in file %s\n" , ai_szFile ) ;

			return M4_ERROR;
		}

		return M4_SUCCESS ;
}
