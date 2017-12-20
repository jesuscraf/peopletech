//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             main.cpp (Test del cliente)
// Project:
// Author:           Meta Software M.S. , S.A
// Date:
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include <stdio.h>

#include <clienttag.hpp>
#include <clientapi.hpp>
#include <srvping.hpp>
#include <oblutil.hpp>
#include <regedit.hpp>
#include <m4date.hpp>
#include <m4log.hpp>
#include "m4xmlreg.hpp"
#include "m4buildversion.hpp"
#include "m4regdef.hpp"



#ifdef _UNIX
	#define LOGFILE "logsys.ini"
	#define CACERT  "cacert.pem"
#else
	#define LOGFILE "c:\\temp\\logsys.ini"
	#define CACERT  "c:\\temp\\cacert.pem"
#endif

#define DEFAULT_PORT 3002
#define DEFAULT_SSL_PORT 3003
#define DEFAULT_NSESSIONS 1

m4bool_t	bSSL=M4_FALSE;




m4return_t GetServersFronRegisty ( ClServers & oServers )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		iSize = 0 ;
	m4uint32_t		iPort = 0 ;
	m4pchar_t		pcConcatServer = NULL ;
	m4pchar_t		pcKeyName = NULL ;
	m4pchar_t		pcPortKey = NULL ;
	m4pchar_t		pcData = NULL ;
    m4char_t		acTempBuffer[ 1024 + 1 ] ;
	list<m4pchar_t> lSubKeys ;

	list<m4pchar_t>::iterator	itIterator ;


	if ( bSSL )
	{
		pcPortKey = "IdSSLPort" ;
	}
	else
	{
		pcPortKey = "IdPort" ;
	}

	iResult = M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, "Servers\\", acTempBuffer, 1024, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se lee del nuevo API del registro
	iResult = M4XmlRegistry::Instance()->GetSubKeyNames( acTempBuffer, lSubKeys ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	pcConcatServer = acTempBuffer + strlen( acTempBuffer ) ;

	itIterator = lSubKeys.begin() ;

	while( itIterator != lSubKeys.end() )
	{
		pcKeyName = *itIterator ;
		strcpy( pcConcatServer, pcKeyName ) ;
		delete [] pcKeyName ;

		iResult = M4XmlRegistry::Instance()->GetValue( acTempBuffer, pcPortKey, pcData ) ;

		if( iResult == M4_SUCCESS )
		{
			iPort = atoi ( pcData ) ;
			delete [] pcData ;

			iResult = M4XmlRegistry::Instance()->GetValue( acTempBuffer, "Server", pcData ) ;

			if( iResult == M4_SUCCESS )
			{
				oServers.AddServer( pcData, iPort ) ;
				delete [] pcData ;
			}
		}

		itIterator++ ;
	}
	lSubKeys.clear() ;

	return M4_SUCCESS ;
}


m4return_t GetOblAcceptorPort ( const m4pchar_t ai_szObl , m4uint32_t & iPort ) 
{
	ClOblUtil oObl ( ai_szObl ) ;
	m4char_t szPort [10] ;
	m4char_t szAcceptorType [20] ;

	if ( bSSL )
	{
		strcpy ( szAcceptorType , "\"TSAP_SSL\"" ) ;
	}
	else
	{
		strcpy ( szAcceptorType , "\"TSAP\"" ) ;
	}


	if ( oObl.Error () )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == oObl.GetProperty ( szAcceptorType , "ACCEPTOR_PORT" , szPort , 10 ) ) 
	{
		return M4_ERROR ;
	}

	if (! ( iPort = atoi ( szPort ) ) )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
}


void PosiblesInvocaciones(m4pchar_t *argv)
{
	cerr <<"Usage:\n";
	cerr <<"  "<<argv[0]<<" [-p port | -o obl] [-t nconex] [-c] [-r | server]" <<endl;
	cerr <<"-c use SSL"<<endl;
	cerr <<"-r Connect to all servers in registry (client)"<<endl;
	cerr <<"Default server host: localhost"<<endl;
	cerr <<"Default port:"<<DEFAULT_PORT<<"\nDefault secure port:"<<DEFAULT_SSL_PORT<<""<<endl;
	cerr <<"-T time timeout"<<endl;
	cerr <<"If nconex == 0 endless loop"<<endl;
}


int main(int argc, m4char_t **argv)
{

	ClServers oServers ;
	M4ClString strAddress ;
	m4pchar_t szObl = NULL ;
	m4return_t iRet ;
	m4pchar_t pServer ;

	m4int32_t	index = 1;

	m4uint32_t		srvPort = DEFAULT_PORT;
	m4bool_t		bDefaultPort=M4_TRUE;
	
	m4uint32_t NSessiones,uiControlTimeout=60000;

	m4bool_t	bRegistry=M4_FALSE;

	NSessiones=DEFAULT_NSESSIONS;

	while(argv[index] && argv[index][0]=='-')
	{
		switch(argv[index][1])
		{
		case 'c':
			bSSL=M4_TRUE;
			if(bDefaultPort)
				srvPort=DEFAULT_SSL_PORT;
			argc--;
			index++;
			break;
		case 'r':
			bRegistry=M4_TRUE;
			argc--;
			index++;
			break;
		case 'o':
			szObl= argv[(++index)++] ;
			argc-=2;
			break;
		case 't':
			NSessiones=atoi(argv[(++index)++]);
			argc-=2;
			break;
		case 'p':
			srvPort=atoi(argv[(++index)++]);
			argc-=2;
			bDefaultPort=M4_FALSE;
			break;
		case 'h':
			PosiblesInvocaciones(argv);
			exit(-1);
			break;
		case 'T':
			argc-=2;
			uiControlTimeout=atoi(argv[(++index)++]);
			break;
		default:
			cerr << "Ignoring flag '" <<argv[index]<<"'.\nSkipping it. Use -h for help."<<endl;
			argc--;
			index++;
			break;
		}
	}

	switch(argc)
	{
	case 1:
		strAddress = "localhost" ;
		break ;
	case 2:
		strAddress = argv[index];
		cout << "Server:" << strAddress << endl;
		cout << "Port:" << srvPort << endl;
		m4DebugExecute(if(NSessiones!=1) cout <<"Opening "<<NSessiones<<" sessions."<<endl);
		break;
	default:
		cerr <<"Invalid options" <<endl;
		PosiblesInvocaciones(argv);
		exit(-1);
		break;
	}

	if ( bRegistry )
	{
		if ( strcmp( strAddress.c_str(), "localhost" ) != 0 )
		{
			cout << "Invalid Options " << "-r AND server" << endl ;
			PosiblesInvocaciones(argv);
			exit(-1);
		}

		if ( szObl )
		{
			cout << "Invalid Options " << "-r AND -o" << endl ;
			PosiblesInvocaciones(argv);
			exit(-1);
		}


		if ( M4_ERROR == GetServersFronRegisty ( oServers ) )
		{
			cout << "Error reading fron registry" << endl ;
			exit (-1) ;
		}
	}
	else
	{
		if ( szObl )
		{
			if ( ! bDefaultPort )
			{
				cout << "Invalid Options " << "-p AND -o" << endl ;
				PosiblesInvocaciones(argv);
				exit(-1);
			}

			if ( M4_ERROR == GetOblAcceptorPort ( szObl , srvPort ) )
			{
				cout << "Error reading ACCEPTOR_PORT fron " << szObl <<endl ;
				exit (-1) ;
			}
		}

		oServers.AddServer (strAddress , srvPort ) ;
	}
//	m4char_t	hostName[128];

	m4return_t ret, retping=M4_SUCCESS;


	m4bool_t bLog=M4_TRUE;
	m4Trace(cout <<"Starting log manager ..."<<flush);
	
	ret = INITIALIZELOGMANAGER(LOGFILE);
	if (M4_ERROR == ret)
	{
		m4Trace(cerr << "Error using logfile"<< LOGFILE <<endl);
		bLog=M4_FALSE;
	}
	else
	{
		cout << "Running" <<endl;
		bLog=M4_TRUE;
	}

	if(!NSessiones)
	{
		m4Trace(cout << "Multisession: Endless loop"<<endl);
	}
	else
		m4Trace(cout << "Multisession:"<<NSessiones<<endl);


	iRet = oServers.GetFirstServer ( pServer , srvPort ) ;
	
	while ( iRet == M4_SUCCESS)
	{

		ClClientAPI	theAPI(pServer );
		m4Trace(cout <<"Using Timeout="<<uiControlTimeout<<endl);
		ret = theAPI.Init(uiControlTimeout);

		if(M4_ERROR == ret)
		{
			cerr << "Error starting Communication API" <<endl;
			if (bLog) 
				cerr <<"More details in logfile." << endl;
			return ret;
		}
		else 
			m4TraceLevel(2,cout << "API Started" <<endl);

		m4handle_t		sessionId	= 0;
		m4pchar_t		arg = "";

		ClTagList       theArgs;

		theArgs.SetTag(M4_PROP_SERVER_ARCHITECTURE);
		theArgs.SetTag(M4_PROP_SERVER_VERSION);

	

		for(m4uint32_t Nses=0;(Nses <NSessiones) || (!NSessiones);Nses++)
		{
//			m4Trace(cout << "Openning session");
			if(NSessiones!=1)
			{
				m4Trace(cout << " n " << Nses );
			}

			m4Trace(cout <<endl);

			strAddress = pServer ;

			retping = theAPI.Ping(strAddress, srvPort, &theArgs,bSSL,CACERT);

			if (M4_SUCCESS != retping)
			{
				if (M4_ERROR == retping)
				{
					cerr<<"Error in ping:"<<(int)retping<< " Server: " << pServer << ", Port : " << srvPort ;
					if ( bSSL )
						cerr << " SSL connection" << endl;
					continue;
				}
				else	// Tenemos un warning
				{
			//	Tambien podria haber producido un warning si las tags no fueran buenas.
		//		cerr<<"Error in ping:"<<(int)ret<<endl;
					continue;
				}
			}		
			else
			{
				cout  << "Ping ok"<< " Server : " << pServer << ", Port : " << srvPort <<endl;
				if ( bSSL )
					cerr << " SSL connection" << endl;
			}

#ifndef TEST_CLOSE_TSAP

			m4ServerVT oVT1,oVT2;
			ret=theArgs.GetArg(M4_PROP_SERVER_ARCHITECTURE,oVT1);
			if(M4_ERROR==ret)
			{
				m4Trace(cerr <<"Error reading " << M4_PROP_SERVER_ARCHITECTURE<<endl);
			}
			else
			{
				if (NULL != oVT1.Data.PointerChar)
				{
					cout <<"Ping ok"<<endl;
					m4Trace(cout<<"Server architecture " <<oVT1.Data.PointerChar <<endl);
				}
/*
				else 
					m4Trace(cerr<<"Couldn't obtain server architecture" <<endl);
					*/
			}				
		

			ret=theArgs.GetArg(M4_PROP_SERVER_VERSION,oVT1);
			if(M4_ERROR==ret)
			{
//				m4Trace(cerr <<"Error reading " << M4_PROP_SERVER_VERSION<<endl);
			}
			else
			{
				if (NULL != oVT1.Data.PointerChar)
				{
					m4Trace(cout<<"Server version " <<oVT1.Data.PointerChar <<endl);
				}
				else 
				{
//					m4Trace(cerr <<"Error reading " << M4_PROP_SERVER_VERSION<<endl);
				}
			}				

#endif //TEST_CLOSE_TSAP

		}
		
		iRet = oServers.GetNextServer ( pServer , srvPort ) ;

	} // Numero de sesiones
	
	m4sleep (5) ;

	return 0;
}


