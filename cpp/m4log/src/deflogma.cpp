//## begin module.includes preserve=yes

#include "m4log.hpp"
#include "m4thread.hpp"
#include "logmanag.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes

 

#define M4_MAX_CAD_FORMAT	8192 //8 * 1024

ClLogManager *g_pLogManager = NULL;
m4int16_t ManagerCount = 0;
ClMutex	g_oMutexLogManager ( M4_TRUE ) ;


m4int16_t	_STDCALL CREATELOGMANAGER()
{	
	
	if ( g_pLogManager )
		return M4_ERROR;
	
	g_pLogManager = new ClLogManager();
	return M4_SUCCESS;
}


ClLogSystem	*GETNEWLOGSYSTEM()
{
	m4int32_t	ithread;
	M4ClCrono	crono;

	if ( ! g_pLogManager )
		return NULL;

	if ( g_pLogManager -> TestCrono() )
		crono.Start();

	ClLogSystem	*plogSystem;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread);
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( g_pLogManager -> TestCrono() )
		g_pLogManager -> AddTime( crono.Stop() );


	return( plogSystem ) ;
}


ClLogSystem *SETLOGSYSTEM(ClLogSystem *ai_LogSystem)
{
	
	if ( !g_pLogManager )
		return NULL;
	
	return g_pLogManager -> SetLogSystem(ai_LogSystem);
	
}



ClLogSystem *GETPTHREADLOGSYSTEM()
{
	ClLogSystem	*plogSys;

	m4int32_t	lhthread;
	M4ClCrono	crono;

	if ( !g_pLogManager )
		return NULL;
	
	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	lhthread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if	( ! g_pLogManager -> ExistLogSystem( lhthread ) )
		g_pLogManager -> CreateLogSystem( lhthread );

	if ( ( plogSys = g_pLogManager -> GetCurrentLogSystem() ) == NULL )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return NULL;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return plogSys;
}



ClLogManager	*GETLOGMANAGER()
{
	return g_pLogManager;
}


m4int16_t	_STDCALL DESTROYLOGMANAGER()
{
	
	g_oMutexLogManager.Lock();

	ManagerCount--;
    if (ManagerCount) {
#ifdef _DEBUG
		printf("\nLogManager destruction canceled: %d", ManagerCount);
#endif
		g_oMutexLogManager.Unlock();
		return M4_WARNING;
	}
	if ( ! g_pLogManager )
	{
		g_oMutexLogManager.Unlock();
		return M4_ERROR;
	}
	if ( g_pLogManager )
	{
		delete g_pLogManager;
		g_pLogManager = NULL;
	}
	g_oMutexLogManager.Unlock();
	return M4_SUCCESS;
}


M4_DECL_M4LOG m4int16_t _STDCALL INITIALIZELOGMANAGER( m4pchar_t ai_pFileName )
{

	if (ai_pFileName==NULL) return M4_ERROR;

	
	g_oMutexLogManager.Lock();

	if (ManagerCount) {
		ManagerCount++;
		g_oMutexLogManager.Unlock();
		return M4_WARNING;
	}
	
	M4ClCrono	crono;

	if ( ! g_pLogManager ) {
		ManagerCount++;
		CREATELOGMANAGER();
	}

	if ( g_pLogManager -> TestCrono() )
		crono.Start();
	
	if ( g_pLogManager -> Initialize() == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );
		g_oMutexLogManager.Unlock();
		return M4_ERROR;
	}

M4ClString snnameFile;

	//ADD FJSM 990716. leemos lobgase.ini del workdir
	m4pchar_t	 LogBasePath;
	LogBasePath = getenv("M4_LOGBASE_PATH");
	if (LogBasePath==NULL) {
		snnameFile=ai_pFileName;
	
		#ifdef _UNIX
			size_t pos = snnameFile.rfind("/");
		#else
			size_t pos = snnameFile.rfind("\\");
		#endif
		
		snnameFile.erase(pos+1, snnameFile.size());
		snnameFile+="logbase.ini";
	}
	else {
		snnameFile = LogBasePath;
	}


	if ( g_pLogManager -> updateLogConfiguration(snnameFile)  == M4_ERROR ) {
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );

		char *psnnameFile = snnameFile;
		SETCODEF(0, ERRORLOG, "-- Internal M4Log error.\n\t\t M4Log cannot find the file \"%s\" unable to update log configuration.", psnnameFile);
		g_oMutexLogManager.Unlock();

		//	Para soportar multiconfiguracion.
		//	return M4_ERROR;
	}
	//END ADD

	/*DEL FJSM 990716 ya que se realiza en updateLogConfiguration(...)
	if ( g_pLogManager -> SetIniFile( snnameFile ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );
		g_oMutexLogManager.Unlock();
		return M4_ERROR;
	}
	*/
	snnameFile = ai_pFileName;


	if ( g_pLogManager -> updateLogConfiguration(snnameFile)  == M4_ERROR ) {
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );
	
		char *psnnameFile = snnameFile;
		SETCODEF(0, ERRORLOG, "-- Internal M4Log error.\n\t\t M4Log cannot find the file \"%s\" unable to update log configuration.", psnnameFile);
		g_oMutexLogManager.Unlock();
		return M4_ERROR;
	}


	if ( g_pLogManager -> TestCrono() )
		g_pLogManager -> AddTime( crono.Stop() );

	g_oMutexLogManager.Unlock();


	/* Bug 0108256
	Hay que reinicializar los settings del log actual por si se ha creado
	*/
	m4int32_t	ithread;
	ClLogSystem	*plogSystem;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if( plogSystem != NULL )
	{
		plogSystem->ResetOverwriteMask( g_pLogManager ) ;
	}


	return M4_SUCCESS;
}



m4int16_t	_STDCALL STARTCODE( m4int32_t icode   // codigo del mensaje de error
					, m4int16_t irango  // WARNINGLOG, ERRORLOG, DEBUGINFOLOG, TRACEINFOLOG
					)
{

	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if ( ! ( g_pLogManager -> TestErrorMode( irango ) ) )
		return M4_ERROR;

	M4ClCrono	crono;
	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	
	if ( plogSystem -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;	
}


m4int16_t	_STDCALL ADDTEXT( m4pchar_t pmessage )
{
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;

	if ( ! g_pLogManager )
		return M4_ERROR;

	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );		

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	M4ClString	smessage;

	smessage = pmessage;

	if ( plogSystem -> AddText( smessage ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}
	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;
}



m4int16_t	_STDCALL ADDTEXTF( m4pchar_t cadFormato, ... )
{
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;

	if ( !g_pLogManager )
		return M4_ERROR;
	
	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );		

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}
	
	va_list marker;
	m4char_t	buffer[3200];
	M4ClString	smessage;

	va_start( marker, cadFormato );     /* Initialize variable arguments. */

	vsprintf( buffer, cadFormato, marker );

	smessage = buffer;
	
	if ( g_pLogManager -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = g_pLogManager -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;
		      ipos != M4_STR_NPOS
			; ipos = smessage.find( RETORNO_CARRO ) )//Aqui tenemos que añadir al npos M4ClString cuando se solucione lo del npos
		{
			smessage.replace( ipos, 1, ncret );
		}
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem -> AddText( smessage ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}
	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;
}


m4int16_t	_STDCALL SETCODE( )
{
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;

	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if ( g_pLogManager -> TestCrono() )
		crono.Start();

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );		

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem -> SetCode( ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;
}


m4int16_t	_STDCALL SETCODE( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t pmessage )
{
	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if ( ! ( g_pLogManager -> TestErrorMode( irango ) ) )
		return M4_ERROR;

	
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;
	if ( g_pLogManager -> TestCrono() )
		crono.Start();

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();


	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread);
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );

		return M4_ERROR;
	}

	if ( plogSystem -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}


	M4ClString	smessage;

	if ( ! pmessage )
		smessage = "";
	else
		smessage = pmessage;

	if ( plogSystem -> AddText( smessage ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem -> SetCode() == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;
}


m4int16_t   GetDistance(m4pchar_t &cad, m4uint32_t Size)
{

	m4uint16_t ret=0, cnt=0;


	while (1) {
	  if (cnt>Size) return -1;
	  if ((*(cad+cnt)==1)||(*(cad+cnt)==2)) {
		break;
	  } else {
        cnt++;
	    ret++;
	  };
    };
	
	return ret;

}


m4int16_t	_STDCALL SETSERIALCODE( m4int32_t ai_Size, m4pchar_t ai_SerialCad )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> SetSerialCode( ai_Size, ai_SerialCad );
 }

m4int16_t	_STDCALL SETSERIALCODESTRING( m4pchar_t ai_SerialCad )
{


	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;


	// Se desregistran los listener porque esto es un error que se mete en deserialización
	ILogListener *poListener = plogSystem->RegisterListener( NULL ) ;
	m4int16_t iResult = plogSystem -> SetSerialCodeString( ai_SerialCad);
	plogSystem->RegisterListener( poListener ) ;

	return( iResult ) ;
}


m4int16_t	_STDCALL SETCODEF( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t cadFormato, ... )
{
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;

	if ( ! g_pLogManager )
		return M4_ERROR;

	if ( g_pLogManager -> TestCrono() )
		crono.Start();

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread);
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );

		return M4_ERROR;
	}

	if ( plogSystem -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}



	
/// (R.A.C) 19.11.98  FJSM: 99-06-23 //Eliminacion de malloc y free
/// ------------------------------------------------------------------------
/// Modificacion en el volcado de errores de la log.
/// Problema => No sabemos el tamaño real de la cadena de formatos
/// Resolucion => Si la cadena pasa de 8Kb la truncamos y se dev. warning

	

	va_list marker;

	m4int16_t iRET = 0;
	m4return_t ret = M4_SUCCESS;

	m4char_t	buffer[M4_MAX_CAD_FORMAT] = ""; //8*1024;
	M4ClString	smessage;

	if ( cadFormato )
	{
		va_start( marker, cadFormato );     /* Initialize variable arguments. */

		#if defined	(_WINDOWS)
			iRET = _vsnprintf( buffer, M4_MAX_CAD_FORMAT, cadFormato, marker );
		#else
			iRET = vsnprintf( buffer, M4_MAX_CAD_FORMAT, cadFormato, marker );
		#endif
		if (iRET == -1 ) {
			ret = M4_WARNING;
			buffer[M4_MAX_CAD_FORMAT-1] = 0;
		} else	ret = M4_SUCCESS;
		

		va_end( marker );
	}

	smessage = buffer;
	
/// ------------------------------------------------------------------------

	
	if ( g_pLogManager -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = g_pLogManager -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;

		      ipos != M4_STR_NPOS
			; ipos = smessage.find( RETORNO_CARRO ) )//Tenemos que volver a poner M4ClString cuando se solucione lo de npos
		{
			smessage.replace( ipos, 1, ncret );
		}
	}

	if ( plogSystem -> AddText( smessage ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}
	
	if ( plogSystem -> SetCode() == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return ret;
}


m4int16_t	_STDCALL SETCODEVF( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t cadFormato, va_list marker)
{
	m4int32_t	ithread;
	
	ClLogSystem	*plogSystem;

	M4ClCrono	crono;

	if ( ! g_pLogManager )
		return M4_ERROR;

	if ( g_pLogManager -> TestCrono() )
		crono.Start();

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
			g_pLogManager -> AddTime( crono.Stop() );

		return M4_ERROR;
	}

	if ( plogSystem -> StartCode( icode, irango ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}


	m4char_t	buffer[3200];
	M4ClString	smessage;

	if ( cadFormato )
		vsprintf( buffer, cadFormato, marker );
	else
		strcpy( buffer, "" );

	smessage = buffer;
	
	if ( g_pLogManager -> GetCReturn() != RETORNO_CARRO )
	{
		m4char_t	ncret[2];
		ncret[0] = g_pLogManager -> GetCReturn();
		ncret[1] = 0;
		for ( size_t ipos = smessage.find( RETORNO_CARRO )
			;

ipos != M4_STR_NPOS

			; ipos = smessage.find( RETORNO_CARRO ) )//Aqui igual que antes
	{
			smessage.replace( ipos, 1, ncret );
		}
	}

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem -> AddText( smessage ) == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}
	
	if ( plogSystem -> SetCode() == M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;
}


m4int16_t	_STDCALL STORELOGS( m4int8_t sstorages )
{

	return M4_SUCCESS;
	//Esto ya no es necesario, pues cada vez que se realiza un SETCODEF, se
	//vuelca a fichero...
/*	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	M4ClCrono	crono;
	
	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}		
		return M4_ERROR;
	}

	if ( plogSystem -> StoreAppoints( sstorages ) )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;*/
	
}


M4_DECL_M4LOG m4return_t _STDCALL SET_MODULE_PATTERN ( m4pchar_t ai_pszModuleName, m4pchar_t ai_pszPatternName )
{

	ClModuleInfo	*auxModuleInfo;
	ClOutputPattern *auxPattern;

	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if (ai_pszModuleName==NULL)
		return M4_ERROR;

	if (ai_pszPatternName==NULL)
		return M4_ERROR;

	auxModuleInfo = g_pLogManager -> GetModuleList() -> GetModuleByName ( ai_pszModuleName );
	auxPattern	  = g_pLogManager -> GetModuleList() -> getOutputPatternMap() -> getOutputPatternByName (ai_pszPatternName);	

	if (auxModuleInfo == NULL ) return M4_ERROR;
	if (auxPattern	  == NULL ) return M4_ERROR;

	auxModuleInfo -> SetOutputPattern (auxPattern);
	return M4_SUCCESS;
}


m4return_t _STDCALL SET_SEVERITY_MODULE_FLAGS ( m4pchar_t ai_pszModuleName,  m4int16_t ai_siSeverity, m4int32_t ai_iLevel)
{

/*
	ClModuleInfo *auxModuleInfo;

	if ( ! g_pLogManager )
		return M4_ERROR;
	
	if (ai_pszModuleName==NULL)
		return M4_ERROR;

	if ((ai_iLevel>8)||(ai_iLevel<0))
		return M4_ERROR;
	
	if (!strcmp(ai_pszModuleName,"DefaultModuleOptions")) {
		
			switch ( ai_siSeverity )
				{
					case WARNINGLOG:
					g_pLogManager -> GetModuleList() -> SetDefaultWarningLevel(ai_iLevel);
					return M4_SUCCESS;
					break;
				
					case DEBUGINFOLOG:
					g_pLogManager -> GetModuleList() -> SetDefaultDebugLevel(ai_iLevel);
					return M4_SUCCESS;
					break;
					
					case ERRORLOG:
					g_pLogManager -> GetModuleList() -> SetDefaultErrorLevel(ai_iLevel);
					return M4_SUCCESS;
					break;

					case TRACEINFOLOG:
					g_pLogManager -> GetModuleList() -> SetDefaultTraceLevel(ai_iLevel);
					return M4_SUCCESS;
					break;

					default:
					return M4_ERROR;
				}

	} else {

		auxModuleInfo = g_pLogManager -> GetModuleList() -> GetModuleByName ( ai_pszModuleName );
		
		if (auxModuleInfo == NULL ) return M4_ERROR;
		
		switch ( ai_siSeverity )
			{
				case WARNINGLOG:
				auxModuleInfo -> SetWarningLevel(ai_iLevel);
				return M4_SUCCESS;
				break;
				
				case DEBUGINFOLOG:
				auxModuleInfo -> SetDebugLevel(ai_iLevel);
				return M4_SUCCESS;
				break;
				
				case ERRORLOG:
				auxModuleInfo -> SetErrorLevel(ai_iLevel);
				return M4_SUCCESS;
				break;

				case TRACEINFOLOG:
				auxModuleInfo -> SetTraceLevel(ai_iLevel);
				return M4_SUCCESS;
				break;

				default:
				return M4_ERROR;
			}
	}
	
*/	
	return M4_ERROR;
}


M4_DECL_M4LOG ILogListener* _STDCALL REGISTER_LISTENER ( ILogListener *ai_poListener )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return NULL;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return NULL;

	return plogSystem -> RegisterListener( ai_poListener );
}


m4int16_t TRASPASEAPPOINTS( ClLogSystem *plogUnit )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	M4ClCrono	crono;
	
	if ( ! g_pLogManager )
		return M4_ERROR;

	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
	{		
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem == plogUnit )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( plogSystem -> AddAppoints( plogUnit -> GetAppointments() )
				== M4_ERROR )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			plogSystem -> AddTime( crono.Pause() );
			g_pLogManager -> AddTime( crono.Stop() );
		}	
		return M4_ERROR;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return M4_SUCCESS;;
	
}


m4int16_t	_STDCALL ACTIVATE_LOGUNIT()
{
	return M4_ERROR;
}


m4int16_t	_STDCALL DEACTIVATE_LOGUNIT()
{
	return M4_ERROR;
}


m4double_t	_STDCALL UNITTIME()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	M4ClCrono	crono;
	if ( g_pLogManager -> TestCrono() )
	{
		crono.Start();
	}

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
	{
		if ( g_pLogManager -> TestCrono() )
		{
			g_pLogManager -> AddTime( crono.Stop() );
		}		
		return -1;
	}

	if ( g_pLogManager -> TestCrono() )
	{
		plogSystem -> AddTime( crono.Pause() );
		g_pLogManager -> AddTime( crono.Stop() );
	}	
	return plogSystem -> UnitTime();


}


m4double_t	_STDCALL TOTALTIME()
{
	if ( g_pLogManager )
		return ( g_pLogManager -> TotalTime() );

	return -1;
}



m4return_t _STDCALL ENABLE_DEBUGINFO_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> EnableDebugInfoLogs();
	return M4_SUCCESS;

}


m4return_t _STDCALL ENABLE_ERROR_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if ( ! g_pLogManager )
		return M4_ERROR;

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> EnableErrorLogs();
	return M4_SUCCESS;

}




m4return_t _STDCALL DESTROYLOGSYSTEM()
{

	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();


	return g_pLogManager -> DestroyLogSystem();
	
}

m4return_t _STDCALL ENABLE_WARNING_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	plogSystem -> EnableWarningLogs();
	return M4_SUCCESS;
}


m4return_t _STDCALL ENABLE_TRACEINFO_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	plogSystem -> EnableTraceInfo();
	return M4_SUCCESS;
}


m4return_t _STDCALL ENABLE_ALL_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> EnableAllLogs();
	return M4_SUCCESS;

}



m4return_t _STDCALL DISABLE_DEBUGINFO_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> DisableDebugInfoLogs();
	return M4_SUCCESS;

}



m4return_t _STDCALL DISABLE_TRACEINFO_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> DisableTraceInfo();
	return M4_SUCCESS;

}

m4return_t _STDCALL DISABLE_ERROR_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}


	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> DisableErrorLogs();
	return M4_SUCCESS;	

}


m4return_t _STDCALL DISABLE_WARNING_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}


	if ( ! plogSystem )
	{
		return M4_ERROR;
	}

	plogSystem -> DisableWarningLogs();
	return M4_SUCCESS;	

}



m4int16_t _STDCALL DISABLE_REMOTE_LOG()
{

	if ( ! g_pLogManager )
		return M4_ERROR;
	return g_pLogManager -> DisableRemoteLog();

}



m4int16_t _STDCALL ENABLE_REMOTE_LOG()
{

	if ( ! g_pLogManager )
		return M4_ERROR;
	return g_pLogManager -> EnableRemoteLog();

}



m4bool_t _STDCALL GET_REMOTE_LOG()
{

	if ( ! g_pLogManager )
		return M4_FALSE;
	return g_pLogManager -> GetRemoteLog();

}



m4return_t _STDCALL DISABLE_ALL_LOGS()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}


	if ( ! plogSystem )
		return M4_ERROR;

	plogSystem -> DisableAllLogs();
	return M4_SUCCESS;	
}



m4int32_t _STDCALL GET_ERROR_QUEUE_SIZE()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}


	if ( ! plogSystem )
		return M4_ERROR;
	
	return plogSystem -> GetErrorQueueSize();	
}



m4return_t _STDCALL GETH_ERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetErrorType( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_LASTERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetLastErrorType( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_MARKEDERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetMarkedErrorType( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_ERRORTYPE( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_ErrorType( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_LASTERRORTYPE( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_LastErrorType( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}

m4return_t _STDCALL GETH_ERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetErrorCode( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


M4_DECL_M4LOG m4return_t _STDCALL GETH_DECIMAL_ERRORCODE( m4int32_t &ao_lErrorCode )
{

	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetErrorCode ( ao_lErrorCode );
}


  
M4_DECL_M4LOG m4return_t _STDCALL GETH_DECIMAL_LASTERRORCODE( m4int32_t &ao_lErrorCode )
{

	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetLastErrorCode ( ao_lErrorCode );
}



M4_DECL_M4LOG m4return_t _STDCALL GETH_DECIMAL_N_ERRORCODE(m4int32_t ai_lIterator, m4int32_t &ao_lErrorCode )
{

	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_ErrorCode ( ai_lIterator, ao_lErrorCode );
}



M4_DECL_M4LOG m4return_t _STDCALL GETH_DECIMAL_N_LASTERRORCODE(m4int32_t ai_lIterator, m4int32_t &ao_lErrorCode )
{

	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_LastErrorCode ( ai_lIterator, ao_lErrorCode );
}

m4return_t _STDCALL GETH_LASTERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetLastErrorCode( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_MARKEDERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetMarkedErrorCode( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_ERRORCODE( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_ErrorCode( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_LASTERRORCODE( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_LastErrorCode( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}

m4return_t _STDCALL GETH_ERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetErrorTime( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_LASTERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetLastErrorTime( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_MARKEDERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetMarkedErrorTime( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_ERRORTIME( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_ErrorTime( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_N_LASTERRORTIME( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;


	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_LastErrorTime( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_ERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetErrorText( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_LASTERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetLastErrorText( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_MARKEDERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetMarkedErrorText( aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}



m4return_t _STDCALL GETH_N_ERRORTEXT( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_ErrorText( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}


m4return_t _STDCALL GETH_N_LASTERRORTEXT( m4int32_t ai_lIterator, m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Get_n_LastErrorText( ai_lIterator, aio_szBuffer, ai_lBufferSize, ao_BufferFilled );
}

m4return_t _STDCALL GET_SERIAL_STRING_SIZE(m4uint32_t& ao_StringSize)
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetSerialStringSizeFromMessage(ao_StringSize,0);
}



m4return_t _STDCALL GET_SERIALIZE_ERRORS( m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter)
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> GetSerialStringFromMessage( ai_szBuffer, ai_lBufferSize, ao_lastMessageIter, 0);
}

m4return_t	_STDCALL POP()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Pop();
}


m4return_t	_STDCALL POP_BACK()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> Pop_back();
}



m4bool_t	_STDCALL EMPTY()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_TRUE;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_TRUE;

	return plogSystem -> Empty();
}


M4_DECL_M4LOG m4return_t _STDCALL DUMP_CURRENT_LOGSYSTEM_TO_FILE ( m4pchar_t aio_String, m4uint32_t ai_StringSize )
{

	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;
 
	return plogSystem -> toString(aio_String, ai_StringSize);
}


M4_DECL_M4LOG m4return_t	_STDCALL CLEAN()
{

	m4return_t ret = M4_SUCCESS;
	
	if ( ! g_pLogManager )
		return M4_ERROR;

	while ( EMPTY() != M4_TRUE ) {
			
		ret = POP();
	}

	return ret;
}


m4return_t _STDCALL SETMARK( )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	if (g_pLogManager -> GetLogSystem( ithread, plogSystem ) == M4_ERROR) {
		g_pLogManager -> CreateLogSystem( ithread );
		g_pLogManager -> GetLogSystem( ithread, plogSystem );
	}

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> SetMark();
}


m4return_t _STDCALL MOVEMARK( )
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> MoveMark();
}


m4return_t _STDCALL ISVALIDMARK()
{
	ClLogSystem	*plogSystem;
	m4int32_t	ithread;

	if ( ! g_pLogManager )
		return M4_ERROR;

	ithread = (m4int32_t)M4Thread::GetCurrentIdThread();

	g_pLogManager -> GetLogSystem( ithread, plogSystem );

	if ( ! plogSystem )
		return M4_ERROR;

	return plogSystem -> IsValidMark();
}

m4return_t _STDCALL SET_DEFAULT_FILE_STORAGE( m4pchar_t ai_pFileName, m4int32_t ai_BufferSize)

{

	if ( ! g_pLogManager )
		return M4_ERROR;

	ClDefaultConfiguration *DefaultConfig = ClDefaultConfiguration::Instance();

	DefaultConfig -> m_sType = 0;
	DefaultConfig -> m_iFileName = ai_pFileName;
	DefaultConfig -> m_lBufferSize = ai_BufferSize;


	return M4_SUCCESS;

};


m4uint32_t _STDCALL GET_GLOBAL_MESSAGES_COUNTER ()

{

	if ( ! g_pLogManager )
		return 0;

	return g_pLogManager -> GetMessageCounter();

};


m4return_t _STDCALL SET_DEFAULT_TCP_STORAGE( m4int16_t ai_Port, m4int32_t ai_BufferSize)

{

	if ( ! g_pLogManager )
		return M4_ERROR;

	ClDefaultConfiguration *DefaultConfig = ClDefaultConfiguration::Instance();

	DefaultConfig -> m_sType = 1;
	DefaultConfig -> m_iPort = ai_Port;
	DefaultConfig -> m_lBufferSize = ai_BufferSize;

	return M4_SUCCESS;

};



m4return_t _STDCALL SET_DEFAULT_ERRORTAIL_STORAGE()

{

	if ( ! g_pLogManager )
		return M4_ERROR;

	ClDefaultConfiguration *DefaultConfig = ClDefaultConfiguration::Instance();

	DefaultConfig -> m_sType = 2;

	return M4_SUCCESS;

};

m4return_t _STDCALL SET_DEFAULT_SEBERITY_FLAG( m4int8_t sFlag)
{

	if ( ! g_pLogManager )
		return M4_ERROR;
	
	ClDefaultConfiguration *DefaultConfig = ClDefaultConfiguration::Instance();

	if (sFlag&1) DefaultConfig -> m_sOpDeb = 1; else DefaultConfig -> m_sOpDeb = 0;
	if (sFlag&2) DefaultConfig -> m_sOpWar = 1; else DefaultConfig -> m_sOpWar = 0;
	if (sFlag&4) DefaultConfig -> m_sOpErr = 1; else DefaultConfig -> m_sOpErr = 0;


	return M4_SUCCESS;

}


 
//## end module.epilog
