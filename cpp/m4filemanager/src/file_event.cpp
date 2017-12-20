//## begin module%374E96100091.cm preserve=no
//## end module%374E96100091.cm

//## begin module%374E96100091.cp preserve=no
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
//## end module%374E96100091.cp

//## Module: file_event%374E96100091; Package body
//## Subsystem: M4FileManager::src%374E96100087
//	d:\fuentesservidor\m4filemanager\version\src
//## Source file: d:\evol\m4filemanager\version\src\file_event.cpp

//## begin module%374E96100091.additionalIncludes preserve=no
//## end module%374E96100091.additionalIncludes

//## begin module%374E96100091.includes preserve=yes
#include <stdio.h>
#include <syncro.hpp>
#ifdef _WINDOWS
#include <process.h>
#endif
//## end module%374E96100091.includes

// file_misc
#include <file_misc.hpp>
// file_event
#include <file_event.hpp>
// m4types
#include <m4types.hpp>
#include "m4unicode.hpp"


//## begin module%374E96100091.declarations preserve=no
//## end module%374E96100091.declarations

//## begin module%374E96100091.additionalDeclarations preserve=yes
#ifdef _WINDOWS

	#ifdef _DEBUG
		#define LOG_FILE	"c:\\temp\\m4filemanager.log"
		static FILE *ficLog;
		#define REMOVE_LOGFILE unlink(LOG_FILE)
		// UNICODE FILE
		#define WRITE_LOGFILE(x) { eUniFileEncodingType_t eEncoding = M4UniNative; ficLog=M4Fopen(LOG_FILE, M4UniAppend, eEncoding); if (ficLog!=NULL) { fprintf(ficLog,"ClFileEventBroker: %s\n",x); fclose(ficLog); } }
	#else
		#define REMOVE_LOGFILE
		#define WRITE_LOGFILE(x)
	#endif

#else		// no _WINDOWS
	#define REMOVE_LOGFILE
	#define WRITE_LOGFILE(x)

#endif	// _WINDOWS


#ifdef _WINDOWS
#define FILE_NOTIFY_M4FM \
	FILE_NOTIFY_CHANGE_FILE_NAME	| \
	FILE_NOTIFY_CHANGE_DIR_NAME	| \
	FILE_NOTIFY_CHANGE_ATTRIBUTES	| \
	FILE_NOTIFY_CHANGE_SIZE			| \
	FILE_NOTIFY_CHANGE_LAST_WRITE

	ClMutex oMutexFileEventBroker(M4_TRUE);

	m4bool_t ClFileEventBroker::m_bInitialized = M4_FALSE;
	m4bool_t ClFileEventBroker::m_bExit = M4_FALSE;
	uintptr_t ClFileEventBroker::m_dwThreadStatus = -1L;

	void __cdecl ClFileEventBroker::ChangesWatcher(LPVOID arg)
	{
	 	ClFileEventBroker *broker = (ClFileEventBroker *) arg;
		broker->WatchChanges();
		/* Bug 0072372
		Hay que notificar que el thread se muere
		*/
		SetEvent(broker->m_hShutDownEvent);
		_endthread();
	}
#endif	// _WINDOWS
//## end module%374E96100091.additionalDeclarations


// Class ClFileEventBroker 

//## begin ClFileEventBroker::m_oInstance%374EC3E60152.attr preserve=no  protected: static ClFileEventBroker {VA} ClFileEventBroker()
ClFileEventBroker ClFileEventBroker::m_oInstance = ClFileEventBroker();
//## end ClFileEventBroker::m_oInstance%374EC3E60152.attr

ClFileEventBroker::ClFileEventBroker ()
  //## begin ClFileEventBroker::ClFileEventBroker%1709503812.hasinit preserve=no
  //## end ClFileEventBroker::ClFileEventBroker%1709503812.hasinit
  //## begin ClFileEventBroker::ClFileEventBroker%1709503812.initialization preserve=yes
  //## end ClFileEventBroker::ClFileEventBroker%1709503812.initialization
{
  //## begin ClFileEventBroker::ClFileEventBroker%1709503812.body preserve=yes
#ifdef _WINDOWS
	REMOVE_LOGFILE;

	m_bInitialized=M4_TRUE;

	m_hBeginUpdateEvent=CreateEvent(NULL, FALSE, FALSE, NULL); // Update event
	m_hEndUpdateEvent=CreateEvent(NULL, FALSE, FALSE, NULL); // Update event
	m_hShutDownEvent=CreateEvent(NULL, FALSE, FALSE, NULL); // Update event
	m_hMutexMap=CreateMutex(NULL,FALSE,NULL);
	m_hLockEventsMutex=CreateMutex(NULL,FALSE,NULL);

	m_bEventsLocked=M4_FALSE;
	m_vPendingEvents.reserve(10);

	// The first object is the update event
	m_cWaitObjects=1;
	m_phWaitObjects[0] = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (	m_hBeginUpdateEvent==NULL	||
			m_hEndUpdateEvent==NULL		||
			m_hShutDownEvent==NULL		||
			m_hMutexMap==NULL			||
			m_hLockEventsMutex==NULL	||
			m_phWaitObjects[0]==NULL)
		m_bInitialized=M4_FALSE;
#endif	// _WINDOWS
  //## end ClFileEventBroker::ClFileEventBroker%1709503812.body
}


ClFileEventBroker::~ClFileEventBroker ()
{
  //## begin ClFileEventBroker::~ClFileEventBroker%1223090414.body preserve=yes
#ifdef _WINDOWS

#ifdef _DEBUG
	itMapFile_t itMapFile;
	itMapClient_t itMapClient;
	m4char_t szText[M4_MAX_PATH+30];
	m4pcchar_t pccFile = NULL;
#endif

	itMapDir_t itMapDir;

	if (m_bInitialized==M4_TRUE) {	// Remove dirs

		WaitForSingleObject(m_hMutexMap,INFINITE); // Begin working with mapDir


		/* Bug 0072372
		Hay que esperar a que el thread se muera, aunque a estas alturas debería estar muerto
		*/
		_EndThread() ;


		// Kill all dirs handles
		for (itMapDir=mapDir.begin(); itMapDir!=mapDir.end(); itMapDir++) {

			if ((*itMapDir).second.hDir != INVALID_HANDLE_VALUE) // Close handles
				FindCloseChangeNotification((*itMapDir).second.hDir);
#ifdef _DEBUG
			for (itMapFile=(*itMapDir).second.mapFile.begin(); // Each file
				itMapFile!=(*itMapDir).second.mapFile.end(); itMapFile++) {

				pccFile = (*itMapFile).first.GetFileName();

				for (itMapClient=(*itMapFile).second.mapClient.begin(); // Each client
					itMapClient!=(*itMapFile).second.mapClient.end(); itMapClient++) {

						sprintf(szText,"File %s had still %u references by client %p.",
									pccFile, (*itMapClient).second.cRef,(*itMapClient).first);
						WRITE_LOGFILE(szText);
				}
			}
#endif
		}

		ReleaseMutex(m_hMutexMap);	// End working with mapDir
	}

	if (m_hBeginUpdateEvent!=NULL)
		CloseHandle(m_hBeginUpdateEvent);

	if (m_hEndUpdateEvent!=NULL)
		CloseHandle(m_hEndUpdateEvent);

	if (m_hShutDownEvent!=NULL)
		CloseHandle(m_hShutDownEvent);

	if (m_hMutexMap!=NULL)
		CloseHandle(m_hMutexMap);

	if (m_hLockEventsMutex!=NULL)
		CloseHandle(m_hLockEventsMutex);

	if (m_phWaitObjects[0]!=NULL)
		CloseHandle(m_phWaitObjects[0]);

#endif	// _WINDOWS
  //## end ClFileEventBroker::~ClFileEventBroker%1223090414.body
}



//## Other Operations (implementation)
ClFileEventBroker* ClFileEventBroker::Instance ()
{
  //## begin ClFileEventBroker::Instance%-792904493.body preserve=yes
#ifdef _WINDOWS
	// Check if instance doesn't exist (it must be unique).
	if (m_dwThreadStatus == -1L) {
		// Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutexFileEventBroker.Lock();
		if (m_dwThreadStatus == -1L) { // Initialization
			if (m_bInitialized==M4_TRUE) {
				m_dwThreadStatus = _beginthread(ChangesWatcher, 0, (LPVOID)&m_oInstance);
				if (m_dwThreadStatus == -1L)
					m_bInitialized=M4_FALSE;
			}
		}
		oMutexFileEventBroker.Unlock();
	}
#endif	// _WINDOWS
	return &m_oInstance;
  //## end ClFileEventBroker::Instance%-792904493.body
}

m4return_t ClFileEventBroker::SetInterestInFile (const ClFileName& ai_oFileName, ClFileEventsClientId* ai_oClientId)
{
  //## begin ClFileEventBroker::SetInterestInFile%-1728418723.body preserve=yes
#ifdef _WINDOWS
	ClFileName oDirName;
	MapDirSecond_t mapDirSecond;
	MapFileSecond_t mapFileSecond;
	MapClientSecond_t mapClientSecond;
	itMapDir_t itMapDir;
	itMapFile_t itMapFile;
	itMapClient_t itMapClient;

	if (m_bInitialized==M4_FALSE)
		return M4_SUCCESS;

	ai_oFileName.GetDir(oDirName); // If invalid name, nothing happens
	if (oDirName.IsNull())
		return M4_SUCCESS;

	WaitForSingleObject(m_hMutexMap,INFINITE); // Begin working with mapDir

	// If dir doesn't exist
	itMapDir = mapDir.find(oDirName);
	if (itMapDir==mapDir.end()) {	

		// Set dir info
		mapDirSecond.hDir=SetNotificationHandle(oDirName);
		itMapDir=mapDir.insert(MapDir_t::value_type(oDirName,mapDirSecond)).first;

		// Set file info
		mapFileSecond.oFileState.SetFileName(ai_oFileName); // Set ClFileState
		itMapFile=(*itMapDir).second.mapFile.insert(MapFile_t::value_type(ai_oFileName,mapFileSecond)).first;

		// Set client info
		mapClientSecond.cRef=1; // Set default MapClientSecond_t
		(*itMapFile).second.mapClient.insert(MapClient_t::value_type(ai_oClientId,mapClientSecond));

	// If dir exists
	} else {	

		// If filename doesn't exist
		itMapFile = (*itMapDir).second.mapFile.find(ai_oFileName);
		if (itMapFile == (*itMapDir).second.mapFile.end()) {

			// Set file info
			mapFileSecond.oFileState.SetFileName(ai_oFileName); // Set ClFileState
			itMapFile=(*itMapDir).second.mapFile.insert(MapFile_t::value_type(ai_oFileName,mapFileSecond)).first;

			// Set client info
			mapClientSecond.cRef=1; // Set default MapClientSecond_t
			(*itMapFile).second.mapClient.insert(MapClient_t::value_type(ai_oClientId,mapClientSecond));

		// If filename exists
		} else {

			// If client doesn't exist
			itMapClient = (*itMapFile).second.mapClient.find(ai_oClientId);
			if (itMapClient == (*itMapFile).second.mapClient.end()) {
				// Set client info
				mapClientSecond.cRef=1;
				(*itMapFile).second.mapClient.insert(MapClient_t::value_type(ai_oClientId,mapClientSecond));

			// If client exist
			} else {
				(*itMapClient).second.cRef++; // Increment references
			}

		}

	}

	ReleaseMutex(m_hMutexMap);	// End working with mapDir
	return M4_SUCCESS;
#else
	return M4_SUCCESS;
#endif	// _WINDOWS
  //## end ClFileEventBroker::SetInterestInFile%-1728418723.body
}

m4return_t ClFileEventBroker::UnSetInterestInFile (const ClFileName& ai_oFileName, ClFileEventsClientId* ai_oClientId)
{
  //## begin ClFileEventBroker::UnSetInterestInFile%-1880183684.body preserve=yes
#ifdef _WINDOWS
	ClFileName oDirName;
	itMapDir_t itMapDir;
	itMapFile_t itMapFile;
	itMapClient_t itMapClient;
	m4return_t ret=M4_ERROR;

	if (m_bInitialized==M4_FALSE)
		return M4_SUCCESS;

	ai_oFileName.GetDir(oDirName); // If invalid name, nothing happens
	if (oDirName.IsNull())
		return M4_SUCCESS;

	WaitForSingleObject(m_hMutexMap,INFINITE); // Begin working with mapDir

	// If dir  exist
	itMapDir = mapDir.find(oDirName);
	if (itMapDir!=mapDir.end()) {	

		// If filename  exist
		itMapFile = (*itMapDir).second.mapFile.find(ai_oFileName);
		if (itMapFile != (*itMapDir).second.mapFile.end()) {

			// If client doesn't exist
			itMapClient = (*itMapFile).second.mapClient.find(ai_oClientId);
			if (itMapClient != (*itMapFile).second.mapClient.end()) {
				ret=M4_SUCCESS;
				if (--((*itMapClient).second.cRef)==0) { // If client ref == 0
					(*itMapFile).second.mapClient.erase(itMapClient);
					if ((*itMapFile).second.mapClient.size()==0) { // No more clients in the file
						(*itMapDir).second.mapFile.erase(itMapFile); // Erase file
						if ((*itMapDir).second.mapFile.size()==0) { // No more files in the dir
							UnsetNotificationHandle((*itMapDir).second.hDir);
							mapDir.erase(itMapDir); // Erase dir
						}
					}
				}
			}

		}

	}

	ReleaseMutex(m_hMutexMap);	// End working with mapDir

	return ret;
#else
	return M4_SUCCESS;
#endif	// _WINDOWS
  //## end ClFileEventBroker::UnSetInterestInFile%-1880183684.body
}


void ClFileEventBroker::LockEvents(void)
{
#ifdef _WINDOWS
	WaitForSingleObject(m_hLockEventsMutex,INFINITE);
	m_bEventsLocked=M4_TRUE;
	ReleaseMutex(m_hLockEventsMutex);
#endif	// _WINDOWS
}


void ClFileEventBroker::UnlockEvents(void)
{
#ifdef _WINDOWS

	m4uint32_t	i;
	size_t		iLength;


	/* Bug 0099171
	Hay que bloquear primero el mutex del mapa de ficheros, porque la WatchChanges
	bloquea primero ese mutex antes de llamar a la TryNotifyChanges
	que también bloquea al mutex del mapa de eventos pendientes
	*/
	WaitForSingleObject(m_hMutexMap,INFINITE); // Begin working with mapDir
	WaitForSingleObject(m_hLockEventsMutex,INFINITE);

	m_bEventsLocked=M4_FALSE;

	iLength=m_vPendingEvents.size();

	if (iLength>0)
	{
		for (i=0; i<iLength; i++)
		{
			NotifyChanges(m_phWaitObjects[m_vPendingEvents[i]]);
		}

		m_vPendingEvents.clear();
	}

	ReleaseMutex(m_hLockEventsMutex);
	ReleaseMutex(m_hMutexMap);	// End working with mapDir

#endif	// _WINDOWS
}


/* Bug 0077666
Para poder cerrar el thread antes de llegar al destructor del objeto estático
*/
void ClFileEventBroker::EndThread( void )
{
#ifdef _WINDOWS
	if( m_bInitialized==M4_TRUE && m_dwThreadStatus!=-1L )
	{
		m_oInstance._EndThread() ;
	}
#endif	// _WINDOWS
}




// Additional Declarations
  //## begin ClFileEventBroker%374E95FF0245.declarations preserve=yes
#ifdef _WINDOWS

/* Bug 0077666
Para poder cerrar el thread antes de llegar al destructor del objeto estático
Si el thread está vivo se pone el flag de cerrar y se despierta.
Después se espera a que el thread esté muerto físicamente
*/
void ClFileEventBroker::_EndThread( void )
{
	DWORD lpExitCode ;

	if( m_bInitialized==M4_TRUE && m_dwThreadStatus!=-1L )
	{
		if( GetExitCodeThread( (HANDLE) m_dwThreadStatus, &lpExitCode ) == 1 && lpExitCode == STILL_ACTIVE )
		{
			m_bExit=M4_TRUE; // Tell thread to exit
			SetEvent(m_phWaitObjects[0]);

			/*
			Hacemos un bucle que espere 100 milisegundos cada vez
			Para comprobar si el thread está vivo o no hay que usar la función GetExitCodeThread,
			que devuleve 1 si va bien y en el valor de retorno del thread STILL_ACTIVE si el thread
			está todavía vivo.
			Si el handle no es válido la función retorna 0.
			*/
			while( GetExitCodeThread( (HANDLE) m_dwThreadStatus, &lpExitCode ) == 1 && lpExitCode == STILL_ACTIVE )
			{
				WaitForSingleObject( m_hShutDownEvent, 100 ) ;
			}

			/* Bug 0080711
			hay que dejar el estado de la variable global como al principio
			*/
			m_bExit = M4_FALSE ;
		}

		m_dwThreadStatus=(DWORD)-1;
	}
}


void ClFileEventBroker::TryNotifyChanges(m4uint32_t ai_iObject)
{

	m4uint32_t	i;
	size_t		iLength;
	m4bool_t	bFound;


	WaitForSingleObject(m_hLockEventsMutex,INFINITE);

	if (m_bEventsLocked==M4_TRUE)
	{
		bFound=M4_FALSE;
		iLength=m_vPendingEvents.size();

		for (i=0; i<iLength && bFound==M4_FALSE; i++)
		{
			if (m_vPendingEvents[i]==ai_iObject)
			{
				bFound=M4_TRUE;
			}
		}

		if (bFound==M4_FALSE)
		{
			m_vPendingEvents.push_back(ai_iObject);
		}
	}
	else
	{
		NotifyChanges(m_phWaitObjects[ai_iObject]); // Notify changes in the directory
	}

	ReleaseMutex(m_hLockEventsMutex);
}


void ClFileEventBroker::NotifyChanges(HANDLE hDir)
{
	// We own m_hMutexMap, so we can access mapDir

	ClFileState oNewFileState;
	ClFileEventDescription oDescription;

	itMapDir_t itMapDir;
	itMapFile_t itMapFile;
	itMapClient_t itMapClient;


	for (itMapDir=mapDir.begin(); itMapDir!=mapDir.end(); itMapDir++) 
		if ((*itMapDir).second.hDir==hDir) {	// The directory

			for (itMapFile=(*itMapDir).second.mapFile.begin(); // Each file
              itMapFile!=(*itMapDir).second.mapFile.end(); itMapFile++) {

				oNewFileState.SetFileName((*itMapFile).first);
				(*itMapFile).second.oFileState.CompareAndSetMask(
                                                  oNewFileState,
                                                  oDescription.m_oDiffMask);
				oDescription.m_oFileName = (*itMapFile).first;

				// Detect changes
				if (oDescription.m_oDiffMask.AnyChange()) {
					(*itMapFile).second.oFileState=oNewFileState;

					for (itMapClient=(*itMapFile).second.mapClient.begin(); // Each client
                    itMapClient!=(*itMapFile).second.mapClient.end(); itMapClient++) 
						((*itMapClient).first)->FileEventHandler(oDescription);
				}
			}
			break;
		}
}


HANDLE ClFileEventBroker::SetNotificationHandle(const ClFileName& ai_oDirName)
{
	HANDLE hRet;

	SetEvent(m_phWaitObjects[0]); // Want to change handle array
	WaitForSingleObject(m_hBeginUpdateEvent,INFINITE); // Wait until thread is ready

	if (m_cWaitObjects==MAXIMUM_WAIT_OBJECTS)
	{
		hRet=INVALID_HANDLE_VALUE;
	}
	else
	{
		int iLength = -1;
		m4pchar_t pcANSIFileName = M4CppToANSI( ai_oDirName.GetFileName(), iLength ) ;

		hRet=FindFirstChangeNotification(pcANSIFileName,FALSE, FILE_NOTIFY_M4FM);
		delete( pcANSIFileName ) ;

		if (hRet != INVALID_HANDLE_VALUE) {
			m_phWaitObjects[m_cWaitObjects] = hRet;
			m_cWaitObjects++;
		}
	}

	SetEvent(m_hEndUpdateEvent); // Notify thread we have finished update
	return hRet;
}


void ClFileEventBroker::UnsetNotificationHandle(HANDLE hDir)
{
	m4uint32_t cCont;

	SetEvent(m_phWaitObjects[0]); // Want to change handle array
	WaitForSingleObject(m_hBeginUpdateEvent,INFINITE); // Wait until thread is ready

	if (hDir!=INVALID_HANDLE_VALUE)	// An invalid handle
		for (cCont=1; cCont<m_cWaitObjects; cCont++) 
			if (hDir==m_phWaitObjects[cCont]) {
				FindCloseChangeNotification(hDir);
				while (cCont<m_cWaitObjects-1) {
					m_phWaitObjects[cCont] = m_phWaitObjects[cCont+1];
					cCont++;
				}
				m_cWaitObjects--;
				break;
			}

	SetEvent(m_hEndUpdateEvent); // Notify thread we have finished update
}


// Thread waiting for changes
void ClFileEventBroker::WatchChanges()
{
	DWORD dwWaitState;
	m4uint32_t uObject;

	HANDLE phNotification[2]; // To send notifications
	phNotification[0] = m_hMutexMap; 
	phNotification[1] = m_phWaitObjects[0];

	while(M4_TRUE) {

		dwWaitState=WaitForMultipleObjects(m_cWaitObjects, m_phWaitObjects, FALSE, INFINITE);

		// An error condition
		if (dwWaitState<WAIT_OBJECT_0 || dwWaitState>WAIT_OBJECT_0+m_cWaitObjects-1)
			break;

		uObject = dwWaitState-WAIT_OBJECT_0;

		if (uObject==0) { //  If an update event is fired.
			if (m_bExit==M4_TRUE)
				return;
			SetEvent(m_hBeginUpdateEvent); // Ready to change handle array
			WaitForSingleObject(m_hEndUpdateEvent, INFINITE); // Wait while updating handle array
		} else {	// A directory has been updated.

			// Begin working with mapDir
			dwWaitState=WaitForMultipleObjects(2,phNotification,FALSE, INFINITE);

			if (dwWaitState==WAIT_OBJECT_0) {	// Mutex
				FindNextChangeNotification(m_phWaitObjects[uObject]);
				TryNotifyChanges(uObject); // Notify changes in the directory
				ReleaseMutex(m_hMutexMap);	// End working with mapDir
			}

			if (dwWaitState==WAIT_OBJECT_0+1) {	// If an update event is fired.
				if (m_bExit==M4_TRUE)
					return;
				SetEvent(m_hBeginUpdateEvent); // Ready to change handle array
				WaitForSingleObject(m_hEndUpdateEvent, INFINITE); // Wait while updating handle array
			}
		}
	}
}



#endif //	_WINDOWS
  //## end ClFileEventBroker%374E95FF0245.declarations
//## begin module%374E96100091.epilog preserve=yes
//## end module%374E96100091.epilog
