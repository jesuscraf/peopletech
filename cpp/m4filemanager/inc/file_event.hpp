//## begin module%374E95FD03B7.cm preserve=no
//## end module%374E95FD03B7.cm

//## begin module%374E95FD03B7.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%374E95FD03B7.cp

//## Module: file_event%374E95FD03B7; Package specification
//## Subsystem: M4FileManager::inc%374E95FD03B5
//	d:\fuentesservidor\m4filemanager\version\inc
//## Source file: d:\evol\m4filemanager\version\inc\file_event.hpp

#ifndef _FILE_EVENT_HPP_
#define _FILE_EVENT_HPP_ 1

//## begin module%374E95FD03B7.additionalIncludes preserve=no
//## end module%374E95FD03B7.additionalIncludes

//## begin module%374E95FD03B7.includes preserve=yes
#include <m4stl.hpp>
#ifdef _WINDOWS
#include <windows.h>
#endif
//## end module%374E95FD03B7.includes

// file_misc
#include <file_misc.hpp>
// m4types
#include <m4types.hpp>
//## begin module%374E95FD03B7.declarations preserve=no
//## end module%374E95FD03B7.declarations

//## begin module%374E95FD03B7.additionalDeclarations preserve=yes
//## end module%374E95FD03B7.additionalDeclarations


//## begin ClFileEventDescription%374E95FF020A.preface preserve=yes
//## end ClFileEventDescription%374E95FF020A.preface

//## Class: ClFileEventDescription%374E95FF020A
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClFileEventDescription 
{
  //## begin ClFileEventDescription%374E95FF020A.initialDeclarations preserve=yes
  //## end ClFileEventDescription%374E95FF020A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClFileEventDescription%-1649160918; C++
      //	For rename events
      //	-- {AddDecl: 67} region.unprotectedFunction [1475..1560]
      ClFileEventDescription ()
        //## begin ClFileEventDescription::ClFileEventDescription%-1649160918.hasinit preserve=no
        //## end ClFileEventDescription::ClFileEventDescription%-1649160918.hasinit
        //## begin ClFileEventDescription::ClFileEventDescription%-1649160918.initialization preserve=yes
        //## end ClFileEventDescription::ClFileEventDescription%-1649160918.initialization
      {
        //## begin ClFileEventDescription::ClFileEventDescription%-1649160918.body preserve=yes
        //## end ClFileEventDescription::ClFileEventDescription%-1649160918.body
      }

    // Data Members for Associations

      //## Association: M4FileManager::<unnamed>%374EAE9103A8
      //## Role: ClFileEventDescription::m_oDiffMask%374EAE920151
      //## begin ClFileEventDescription::m_oDiffMask%374EAE920151.role preserve=no  public: ClComparisonResultMask {1 -> VHAN}
      ClComparisonResultMask m_oDiffMask;
      //## end ClFileEventDescription::m_oDiffMask%374EAE920151.role

      //## Association: M4FileManager::<unnamed>%374EAEB0033F
      //## Role: ClFileEventDescription::m_oNewFileName%374EAEB10174
      //## begin ClFileEventDescription::m_oNewFileName%374EAEB10174.role preserve=no  public: ClFileName {1 -> 1VHAN}
      ClFileName m_oNewFileName;
      //## end ClFileEventDescription::m_oNewFileName%374EAEB10174.role

      //## Association: M4FileManager::<unnamed>%374EAEC60079
      //## Role: ClFileEventDescription::m_oFileName%374EAEC60232
      //## begin ClFileEventDescription::m_oFileName%374EAEC60232.role preserve=no  public: ClFileName {1 -> 1VHAN}
      ClFileName m_oFileName;
      //## end ClFileEventDescription::m_oFileName%374EAEC60232.role

    // Additional Public Declarations
      //## begin ClFileEventDescription%374E95FF020A.public preserve=yes
      //## end ClFileEventDescription%374E95FF020A.public

  protected:
    // Additional Protected Declarations
      //## begin ClFileEventDescription%374E95FF020A.protected preserve=yes
      //## end ClFileEventDescription%374E95FF020A.protected

  private:
    // Additional Private Declarations
      //## begin ClFileEventDescription%374E95FF020A.private preserve=yes
      //## end ClFileEventDescription%374E95FF020A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileEventDescription%374E95FF020A.implementation preserve=yes
      //## end ClFileEventDescription%374E95FF020A.implementation

};

//## begin ClFileEventDescription%374E95FF020A.postscript preserve=yes
/*************************************************************************
NAME: ClFileEventHandler
*************************************************************************/
//## end ClFileEventDescription%374E95FF020A.postscript

//## begin ClFileEventHandler%374E95FF0232.preface preserve=yes
//## end ClFileEventHandler%374E95FF0232.preface

//## Class: ClFileEventHandler%374E95FF0232; Abstract
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%374E95FF023B;ClFileEventDescription { -> }

class M4_DECL_M4FILEMANAGER ClFileEventHandler 
{
  //## begin ClFileEventHandler%374E95FF0232.initialDeclarations preserve=yes
  //## end ClFileEventHandler%374E95FF0232.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: FileEventHandler%-143329254; C++
      virtual void FileEventHandler (const ClFileEventDescription& ai_oEventDescription) = 0;

    // Additional Public Declarations
      //## begin ClFileEventHandler%374E95FF0232.public preserve=yes
      //## end ClFileEventHandler%374E95FF0232.public

  protected:
    // Additional Protected Declarations
      //## begin ClFileEventHandler%374E95FF0232.protected preserve=yes
      //## end ClFileEventHandler%374E95FF0232.protected

  private:
    // Additional Private Declarations
      //## begin ClFileEventHandler%374E95FF0232.private preserve=yes
      //## end ClFileEventHandler%374E95FF0232.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileEventHandler%374E95FF0232.implementation preserve=yes
      //## end ClFileEventHandler%374E95FF0232.implementation

};

//## begin ClFileEventHandler%374E95FF0232.postscript preserve=yes
//## end ClFileEventHandler%374E95FF0232.postscript

//## begin ClFileEventsClientId%374E95FF023E.preface preserve=yes
//## end ClFileEventsClientId%374E95FF023E.preface

//## Class: ClFileEventsClientId%374E95FF023E
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClFileEventsClientId : public ClFileEventHandler  //## Inherits: <unnamed>%374E95FF023F
{
  //## begin ClFileEventsClientId%374E95FF023E.initialDeclarations preserve=yes
  //## end ClFileEventsClientId%374E95FF023E.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClFileEventsClientId%374E95FF023E.public preserve=yes
      //## end ClFileEventsClientId%374E95FF023E.public

  protected:
    // Additional Protected Declarations
      //## begin ClFileEventsClientId%374E95FF023E.protected preserve=yes
      //## end ClFileEventsClientId%374E95FF023E.protected

  private:
    // Additional Private Declarations
      //## begin ClFileEventsClientId%374E95FF023E.private preserve=yes
      //## end ClFileEventsClientId%374E95FF023E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileEventsClientId%374E95FF023E.implementation preserve=yes
      //## end ClFileEventsClientId%374E95FF023E.implementation

};

//## begin ClFileEventsClientId%374E95FF023E.postscript preserve=yes
//## end ClFileEventsClientId%374E95FF023E.postscript

//## begin ClFileControllableObject%374E95FF0240.preface preserve=yes
//## end ClFileControllableObject%374E95FF0240.preface

//## Class: ClFileControllableObject%374E95FF0240
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4FILEMANAGER ClFileControllableObject : public ClFileEventsClientId  //## Inherits: <unnamed>%374E95FF0241
{
  //## begin ClFileControllableObject%374E95FF0240.initialDeclarations preserve=yes
  //## end ClFileControllableObject%374E95FF0240.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClFileControllableObject%374E95FF0240.public preserve=yes
      //## end ClFileControllableObject%374E95FF0240.public

  protected:
    // Additional Protected Declarations
      //## begin ClFileControllableObject%374E95FF0240.protected preserve=yes
      //## end ClFileControllableObject%374E95FF0240.protected

  private:
    // Additional Private Declarations
      //## begin ClFileControllableObject%374E95FF0240.private preserve=yes
      //## end ClFileControllableObject%374E95FF0240.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileControllableObject%374E95FF0240.implementation preserve=yes
      //## end ClFileControllableObject%374E95FF0240.implementation

};

//## begin ClFileControllableObject%374E95FF0240.postscript preserve=yes
//## end ClFileControllableObject%374E95FF0240.postscript

//## begin ClFileEventBroker%374E95FF0245.preface preserve=yes
//## end ClFileEventBroker%374E95FF0245.preface

//## Class: ClFileEventBroker%374E95FF0245
//	Calls made to FileEventHandler (ClFileEventsClientId) are made serialized by
//	a unique secondary thread, so FileEventHandler doesn't need to be reentrant
//	(if the primary thread or other thread don't enter FileEventHandler). The
//	order the clients are called is not defined.
//## Category: M4FileManager%374E95FF0209
//## Subsystem: M4FileManager::inc%374E95FD03B5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%374E95FF0295;m4return_t { -> }
//## Uses: <unnamed>%374E95FF0296;ClFileName { -> }
//## Uses: <unnamed>%374E95FF0297;ClFileEventsClientId { -> }

class M4_DECL_M4FILEMANAGER ClFileEventBroker 
{
  //## begin ClFileEventBroker%374E95FF0245.initialDeclarations preserve=yes
  //## end ClFileEventBroker%374E95FF0245.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClFileEventBroker%1223090414; C++
      //	-- {AddDecl: 135} region.unprotectedFunction [7355..8986]
      virtual ~ClFileEventBroker ();


    //## Other Operations (specified)
      //## Operation: Instance%-792904493; C++
      //	Inheritance can be done
      //	-- {AddDecl: 133} region.unprotectedFunction [6128..6742]
      static ClFileEventBroker* Instance ();

      //## Operation: SetInterestInFile%-1728418723; C++
      //	Singleton
      //	-- {AddDecl: 131} region.unprotectedFunction [2168..4560]
      m4return_t SetInterestInFile (const ClFileName& ai_oFileName, ClFileEventsClientId* ai_oClientId);

      //## Operation: UnSetInterestInFile%-1880183684; C++
      //	-- {AddDecl: 132} region.unprotectedFunction [4561..6127]
      m4return_t UnSetInterestInFile (const ClFileName& ai_oFileName, ClFileEventsClientId* ai_oClientId);

      /* Bug 0069358
      Hay que permitir bloquear la notificación de eventos.
      Un caso es mientras se está haciendo una llamada remota
      para que no se desincronice la serialización.
	  Los eventos se guardan para aplicarlos después.
      */
      void LockEvents(void);

      void UnlockEvents(void);

	  /* Bug 0077666
	  Para poder cerrar el thread antes de llegar al destructor del objeto estático
	  */
	  static void EndThread( void ) ;

    // Additional Public Declarations
      //## begin ClFileEventBroker%374E95FF0245.public preserve=yes
      //## end ClFileEventBroker%374E95FF0245.public

  protected:
    //## Constructors (specified)
      //## Operation: ClFileEventBroker%1709503812; C++
      //	-- {AddDecl: 134} region.unprotectedFunction [6743..7354]
      ClFileEventBroker ();

    // Data Members for Class Attributes

      //## Attribute: m_oInstance%374EC3E60152
      //## begin ClFileEventBroker::m_oInstance%374EC3E60152.attr preserve=no  protected: static ClFileEventBroker {VA} ClFileEventBroker()
      static ClFileEventBroker m_oInstance;
      //## end ClFileEventBroker::m_oInstance%374EC3E60152.attr

    // Additional Protected Declarations
      //## begin ClFileEventBroker%374E95FF0245.protected preserve=yes

#ifdef _WINDOWS

		static void __cdecl ChangesWatcher(LPVOID arg);
		static m4bool_t m_bInitialized;
		static m4bool_t m_bExit;
		static uintptr_t m_dwThreadStatus;


		// m_phWaitObjects[0] is special, it is an event we fire.
		HANDLE m_phWaitObjects[MAXIMUM_WAIT_OBJECTS];	// Wait handles
		m4uint32_t m_cWaitObjects;

		// To control handle array changes
		HANDLE m_hBeginUpdateEvent;
		HANDLE m_hEndUpdateEvent;

		/* Bug 0072372
		Para esperar a que el thread se muera
		*/
		HANDLE m_hShutDownEvent;

		HANDLE m_hMutexMap; // Mutex to access map

		/* Bug 0069358
		Hay que permitir bloquear la notificación de eventos.
		Un caso es mientras se está haciendo una llamada remota
		para que no se desincronice la serialización.
		Los eventos se guardan para aplicarlos después.
		*/
		typedef vector<m4uint32_t>	EventsVector_t;

		HANDLE			m_hLockEventsMutex;
		m4bool_t		m_bEventsLocked;
		EventsVector_t	m_vPendingEvents;


		void WatchChanges(); // Thread that watch changes

		/* Bug 0077666
		Para poder cerrar el thread antes de llegar al destructor del objeto estático
		*/
		void _EndThread( void ) ;

		void TryNotifyChanges(m4uint32_t ai_iObject);

		void NotifyChanges(HANDLE hDir);

		HANDLE SetNotificationHandle(const ClFileName& ai_oDirName);
		void UnsetNotificationHandle(HANDLE hDir);
		
		typedef struct MapClientSecond_t {
			m4uint32_t cRef;
		};

		typedef map<ClFileEventsClientId*,MapClientSecond_t,less<ClFileEventsClientId*> > MapClient_t;

		typedef struct MapFileSecond_t {
			ClFileState oFileState;
			MapClient_t mapClient;
		};

		typedef map<ClFileName,MapFileSecond_t,less<ClFileName> > MapFile_t;
		
		typedef struct MapDirSecond_t {
			HANDLE hDir;
			MapFile_t mapFile;
		};

		typedef map<ClFileName,MapDirSecond_t,less<ClFileName> > MapDir_t;

		typedef MapDir_t::iterator itMapDir_t;
		typedef MapFile_t::iterator itMapFile_t;
		typedef MapClient_t::iterator itMapClient_t;

		typedef pair<MapDir_t,bool > pairMapDir_t;

		MapDir_t mapDir;
#endif	// _WINDOWS

      //## end ClFileEventBroker%374E95FF0245.protected
  private:
    // Additional Private Declarations
      //## begin ClFileEventBroker%374E95FF0245.private preserve=yes
      //## end ClFileEventBroker%374E95FF0245.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileEventBroker%374E95FF0245.implementation preserve=yes
      //## end ClFileEventBroker%374E95FF0245.implementation

};

//## begin ClFileEventBroker%374E95FF0245.postscript preserve=yes
//## end ClFileEventBroker%374E95FF0245.postscript

//## begin module%374E95FD03B7.epilog preserve=yes
//## end module%374E95FD03B7.epilog



#endif
