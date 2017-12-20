//## begin module%377C885003B6.cm preserve=no
//## end module%377C885003B6.cm

//## begin module%377C885003B6.cp preserve=no
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
//## end module%377C885003B6.cp

//## Module: m4log%377C885003B6; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\version\inc\m4log.hpp

#ifndef _M4LOG_HPP_
#define _M4LOG_HPP_ 1

//## begin module%377C885003B6.additionalIncludes preserve=no
//## end module%377C885003B6.additionalIncludes

//## begin module%377C885003B6.includes preserve=yes
//## end module%377C885003B6.includes

#include "m4log_dll.hpp"
#include <m4types.hpp>
#include <m4string.hpp>
#include <stdio.h>

class ClLogManager;
class ClLogSystem;



//## begin module%377C885003B6.declarations preserve=yes
//#include <m4date.hpp> //mio
//## end module%377C885003B6.declarations

//## begin module%377C885003B6.additionalDeclarations preserve=yes


#ifdef _UNIX
	#include  <stdarg.h>
#endif


#ifdef	_WINDOWS
  #ifndef _STDCALL
    #define _STDCALL  _stdcall
  #endif
#else	 // UNIX
    #define _STDCALL
#endif


const m4int16_t ERRORLOG = -1;
const m4int16_t	WARNINGLOG = 1;
const m4int16_t DEBUGINFOLOG = 0;
const m4int16_t TRACEINFOLOG = 2;

const m4int8_t STORAGE1 = 8;
const m4int8_t STORAGE2 = 4;
const m4int8_t STORAGE3 = 2;
const m4int8_t STORAGE4 = 1;
const m4int32_t ALL_STORAGES = 0xFFFFFFFF ;

const m4int8_t	MODE_LOGTHREAD = 0;
const m4int8_t	MODE_LOGOWNER = 1;
const m4int8_t	MODE_LOGREFERENCE = 2;

const char RETORNO_CARRO = '\n';



// Clase interfaz para escuchar los eventos de log
class M4_DECL_M4LOG ILogListener 
{
public:
    virtual	m4return_t	LogEvent( m4pcchar_t ai_pccEventType, m4uint32_t ai_iEventId, m4pcchar_t ai_pccEventMessage ) = 0 ;
};



M4_DECL_M4LOG   m4pchar_t	_STDCALL GET_DEFAULT_LOGPATH();

M4_DECL_M4LOG	m4int16_t	_STDCALL CREATELOGMANAGER();
M4_DECL_M4LOG	ClLogManager	*GETLOGMANAGER();
M4_DECL_M4LOG	m4int16_t	_STDCALL DESTROYLOGMANAGER();
// M4_DECL_M4LOG	m4int16_t	INITIALIZELOGMANAGER( M4ClString snnameFile );
M4_DECL_M4LOG	m4int16_t _STDCALL INITIALIZELOGMANAGER( m4pchar_t ai_pFileName );

M4_DECL_M4LOG	ClLogSystem *   GETPTHREADLOGSYSTEM();

M4_DECL_M4LOG	ClLogSystem *   SETLOGSYSTEM(ClLogSystem *ai_LogSystem);

M4_DECL_M4LOG m4return_t _STDCALL DESTROYLOGSYSTEM();

// funciones de control de tiempo
M4_DECL_M4LOG		void	    CSTART( m4int16_t itype );
M4_DECL_M4LOG   m4double_t	CSTOP( m4int16_t itype );
M4_DECL_M4LOG   m4double_t	CPAUSE( m4int16_t itype );
M4_DECL_M4LOG   m4int16_t	GETCRONOINFO( M4ClString &scronoInfo );
M4_DECL_M4LOG   m4int16_t	GETCRONOINFO( m4int16_t itype, M4ClString &scronoInfo );
M4_DECL_M4LOG   m4int16_t	GETLASTSTARTTIME( m4int16_t itype, M4ClString &stime );


M4_DECL_M4LOG   m4int16_t	_STDCALL STARTCODE( m4int32_t icode   // codigo del mensaje de error
					, m4int16_t irango  // WARNINGLOG, ERRORLOG, DEBUGINFOLOG, TRACEINFOLOG
					);


M4_DECL_M4LOG   m4int16_t	_STDCALL ADDTEXT( m4pchar_t textoError );
M4_DECL_M4LOG   m4int16_t	_STDCALL ADDTEXTF( m4pchar_t cadFormato, ... );

M4_DECL_M4LOG   m4int16_t	_STDCALL SETCODE( );


M4_DECL_M4LOG   m4int16_t	_STDCALL SETCODE( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t pmessage = NULL) ;

M4_DECL_M4LOG   m4int16_t	_STDCALL SETCODEF( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t cadFormato, ... ) ;

M4_DECL_M4LOG   m4int16_t	_STDCALL SETCODEVF( m4int32_t icode // codigo del mensaje de error
						, m4int16_t irango
						, m4pcchar_t cadFormato
						, va_list marker) ;

M4_DECL_M4LOG m4int16_t  _STDCALL SETSERIALCODE( m4int32_t ai_Size, m4pchar_t ai_SerialCad);
M4_DECL_M4LOG m4int16_t  _STDCALL SETSERIALCODESTRING(m4pchar_t ai_SerialCad);


M4_DECL_M4LOG	m4int16_t	_STDCALL ACTIVATE_LOGUNIT();
M4_DECL_M4LOG	m4int16_t	_STDCALL DEACTIVATE_LOGUNIT();

M4_DECL_M4LOG	m4int16_t	_STDCALL STORELOGS( m4int8_t sstorages = 15 );
M4_DECL_M4LOG	m4int16_t	TRASPASEAPPOINTS( ClLogSystem *plogUnit );

M4_DECL_M4LOG	m4double_t	_STDCALL TOTALTIME();  // tiempo usado en la unidad de log
M4_DECL_M4LOG	m4double_t	_STDCALL UNITTIME(); // tiempo usado por el logManager


M4_DECL_M4LOG	m4return_t    _STDCALL ENABLE_DEBUGINFO_LOGS();
M4_DECL_M4LOG m4return_t    _STDCALL ENABLE_ERROR_LOGS();
M4_DECL_M4LOG	m4return_t    _STDCALL ENABLE_WARNING_LOGS();
M4_DECL_M4LOG	m4return_t	 _STDCALL ENABLE_TRACEINFO_LOGS();
M4_DECL_M4LOG	m4int16_t    _STDCALL ENABLE_REMOTE_LOG();

M4_DECL_M4LOG	m4return_t    _STDCALL DISABLE_DEBUGINFO_LOGS();
M4_DECL_M4LOG	m4return_t    _STDCALL DISABLE_ERROR_LOGS();
M4_DECL_M4LOG	m4return_t    _STDCALL DISABLE_WARNING_LOGS();
M4_DECL_M4LOG	m4return_t	 _STDCALL DISABLE_TRACEINFO_LOGS();
M4_DECL_M4LOG	m4int16_t    _STDCALL DISABLE_REMOTE_LOG();

M4_DECL_M4LOG	m4bool_t    _STDCALL GET_REMOTE_LOG();

M4_DECL_M4LOG	m4return_t    _STDCALL ENABLE_ALL_LOGS();
M4_DECL_M4LOG	m4return_t    _STDCALL DISABLE_ALL_LOGS();



M4_DECL_M4LOG m4int32_t _STDCALL GET_ERROR_QUEUE_SIZE();

M4_DECL_M4LOG m4return_t _STDCALL GETH_ERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_LASTERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_MARKEDERRORTYPE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_ERRORTYPE( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_LASTERRORTYPE( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );


M4_DECL_M4LOG m4return_t _STDCALL GETH_ERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_LASTERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_MARKEDERRORCODE( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_ERRORCODE( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_LASTERRORCODE( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );



M4_DECL_M4LOG m4return_t _STDCALL GETH_ERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_LASTERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_MARKEDERRORTIME( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_ERRORTIME( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_LASTERRORTIME( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );


M4_DECL_M4LOG m4return_t _STDCALL GETH_ERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_LASTERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_MARKEDERRORTEXT( m4pchar_t aio_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_BufferFilled );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_ERRORTEXT( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );
M4_DECL_M4LOG m4return_t _STDCALL GETH_N_LASTERRORTEXT( m4int32_t ai_lIterator, m4pchar_t ai_szBuffer
		, m4int32_t ai_lBufferSize, m4int32_t &ao_lSizeCopied );


M4_DECL_M4LOG m4return_t  _STDCALL GETH_DECIMAL_ERRORCODE( m4int32_t &ao_lErrorCode );
M4_DECL_M4LOG m4return_t  _STDCALL GETH_DECIMAL_LASTERRORCODE( m4int32_t &ao_lErrorCode );
M4_DECL_M4LOG m4return_t  _STDCALL GETH_DECIMAL_N_ERRORCODE( m4int32_t ai_lIterator, m4int32_t &ao_lErrorCode );
M4_DECL_M4LOG m4return_t  _STDCALL GETH_DECIMAL_N_LASTERRORCODE( m4int32_t ai_lIterator, m4int32_t &ao_lErrorCode );


M4_DECL_M4LOG m4return_t _STDCALL GET_SERIALIZE_ERRORS( m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter);
M4_DECL_M4LOG m4return_t _STDCALL GET_SERIAL_STRING_SIZE(m4uint32_t& ao_StringSize);


M4_DECL_M4LOG m4return_t	_STDCALL CLEAN();
M4_DECL_M4LOG m4return_t	_STDCALL POP();
M4_DECL_M4LOG m4return_t	_STDCALL POP_BACK();

M4_DECL_M4LOG m4bool_t	_STDCALL EMPTY();

M4_DECL_M4LOG m4return_t _STDCALL SETMARK();
M4_DECL_M4LOG m4return_t _STDCALL MOVEMARK();
M4_DECL_M4LOG m4return_t _STDCALL ISVALIDMARK();

M4_DECL_M4LOG m4return_t _STDCALL SET_DEFAULT_FILE_STORAGE( m4pchar_t ai_pFileName, m4int32_t ai_BufferSize);
M4_DECL_M4LOG m4return_t _STDCALL SET_DEFAULT_TCP_STORAGE( m4int16_t ai_Port, m4int32_t ai_BufferSize);
M4_DECL_M4LOG m4return_t _STDCALL SET_DEFAULT_ERRORTAIL_STORAGE();

M4_DECL_M4LOG m4return_t _STDCALL SET_DEFAULT_SEBERITY_FLAG( m4int8_t sFlag);


M4_DECL_M4LOG m4return_t _STDCALL DUMP_CURRENT_LOGSYSTEM_TO_FILE ( m4pchar_t ai_FileName );

M4_DECL_M4LOG m4uint32_t _STDCALL GET_GLOBAL_MESSAGES_COUNTER ();

M4_DECL_M4LOG m4return_t _STDCALL SET_SEVERITY_MODULE_FLAGS ( m4pchar_t ai_pszModuleName,  m4int16_t ai_siSeverity, m4int32_t ai_iLevel);

M4_DECL_M4LOG m4return_t _STDCALL SET_MODULE_PATTERN ( m4pchar_t ai_pszModuleName, m4pchar_t ai_pszPatternName );

M4_DECL_M4LOG ILogListener* _STDCALL REGISTER_LISTENER ( ILogListener *ai_poListener );


// class M4ClCrono;


//## end module%377C885003B6.additionalDeclarations


//## begin ClLog%377C88510014.preface preserve=yes
//## end ClLog%377C88510014.preface

//## Class: ClLog%377C88510014
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C885100B5;m4int16_t { -> }
//## Uses: <unnamed>%377C885100B6;m4int32_t { -> }
//## Uses: <unnamed>%377C885100BA;m4pchar_t { -> }
//## Uses: <unnamed>%377C885100C1;m4pcchar_t { -> }
//## Uses: <unnamed>%377C88510341;va_list { -> }
//## Uses: <unnamed>%377C88510356;m4double_t { -> }
//## Uses: <unnamed>%377C88510359;m4return_t { -> }
//## Uses: <unnamed>%377C8851037D;m4uint32_t { -> }
//## Uses: <unnamed>%3781DF3800B6;M4ClString { -> }

class M4_DECL_M4LOG ClLog 
{
  //## begin ClLog%377C88510014.initialDeclarations preserve=yes
  //## end ClLog%377C88510014.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLog%-1796335679; C++
      ClLog (m4int8_t slogMode = MODE_LOGTHREAD);

      //## Operation: ClLog%483001396; C++
      ClLog (ClLogSystem* plogSys);

    //## Destructor (specified)
      //## Operation: ~ClLog%1044996127; C++
      ~ClLog ();


    //## Other Operations (specified)
      //## Operation: STARTCODE%-73387955; C++
      m4int16_t STARTCODE (m4int32_t icode, 	// codigo del mensaje de error
      m4int16_t irango	// WARNINGLOG, ERRORLOG, DEBUGINFOLOG, TRACEINFOLOG
      );

      //## Operation: ADDTEXT%1792325199; C++
      m4int16_t ADDTEXT (m4pchar_t textoError);

      //## Operation: ADDTEXTF%-1259627095; C++
      m4int16_t ADDTEXTF (m4pchar_t cadFormato, ... );

      //## Operation: SETCODE%-2103572020; C++
      m4int16_t SETCODE ();

      //## Operation: SETCODE%-1864508977; C++
      m4int16_t SETCODE (m4int32_t icode, m4int16_t irango, m4pcchar_t pmessage = NULL) const;

      //## Operation: SETCODEF%1594254150; C++
      m4int16_t SETCODEF (m4int32_t icode, m4int16_t irango, m4pcchar_t cadFormato, ... ) const;

      //## Operation: SETCODEVF%1850331264; C++
      m4int16_t SETCODEVF (m4int32_t icode, m4int16_t irango, m4pcchar_t cadFormato, va_list marker) const;

      //## Operation: SETSERIALCODE%1736846040; C++
      m4int16_t SETSERIALCODE (m4int32_t ai_Size, m4pchar_t ai_SerialCad);

      //## Operation: SETSERIALCODESTRING%1886185757; C++
      m4int16_t SETSERIALCODESTRING (m4pchar_t ai_SerialCad);

      //## Operation: ACTIVATE_LOGUNIT%192635576; C++
      m4int16_t ACTIVATE_LOGUNIT ();

      //## Operation: DEACTIVATE_LOGUNIT%-1106997895; C++
      m4int16_t DEACTIVATE_LOGUNIT ();

      //## Operation: STORELOGS%-341751241; C++
      //	almacena la información en medio permanente.
      m4int16_t STORELOGS (m4int8_t sstorages = ALL_STORAGES);

      //## Operation: TRASPASEAPPOINTS%407394338; C++
      //	copia todos los AppItems contenidos en la lista Appointments de
      //	plogUnit al m_lappointments de nuestra unidad de logs.
      m4int16_t TRASPASEAPPOINTS (ClLogSystem* plogUnit);

      //## Operation: TOTALTIME%1205405339; C++
      //	tiempo usado en la unidad de log
      m4double_t TOTALTIME ();

      //## Operation: UNITTIME%-1944858855; C++
      m4double_t UNITTIME ();

      //## Operation: GETH_ERRORTYPE%1988868113; C++
      //	tiempo usado por el logManager
      m4return_t GETH_ERRORTYPE (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize);

      //## Operation: GETH_ERRORCODE%289729613; C++
      m4return_t GETH_ERRORCODE (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize);

      //## Operation: GETH_ERRORTIME%-1679065199; C++
      m4return_t GETH_ERRORTIME (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize);

      //## Operation: GETH_ERRORTEXT%1228860288; C++
      m4return_t GETH_ERRORTEXT (m4pchar_t aio_szBuffer, m4int32_t ai_lBufferSize, m4int32_t ao_lBufferSize);

      //## Operation: POP%-1582425221; C++
      m4return_t POP ();

      //## Operation: EMPTY%281501670; C++
      m4bool_t EMPTY ();

      //## Operation: GetDistance%-1294852202; C++
      m4int16_t GetDistance (m4pchar_t& cad, m4uint32_t Size);

      //## Operation: operator +=%1641217550; C++
      ClLog* operator += (ClLog* plog);

      //## Operation: GETPLOG%-1549707977; C++
      ClLogSystem* GETPLOG ()
      {
        //## begin ClLog::GETPLOG%-1549707977.body preserve=yes

 return m_plogSys;

        //## end ClLog::GETPLOG%-1549707977.body
      }

      //## Operation: SETPLOG%1904179874; C++
      void SETPLOG (ClLogSystem* plog);

    // Additional Public Declarations
      //## begin ClLog%377C88510014.public preserve=yes
      //## end ClLog%377C88510014.public

  protected:
    // Additional Protected Declarations
      //## begin ClLog%377C88510014.protected preserve=yes
      //## end ClLog%377C88510014.protected

  private:

    //## Other Operations (specified)
      //## Operation: GETNEWLOGSYSTEM%1820130930; C++
      ClLogSystem* GETNEWLOGSYSTEM (void );

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C8851036D
      //## Role: ClLog::m_plogSys%377C8851036F
      //## begin ClLog::m_plogSys%377C8851036F.role preserve=no  private: ClLogSystem {1 -> 1RFHAPN}
      ClLogSystem *m_plogSys;
      //## end ClLog::m_plogSys%377C8851036F.role

      //## Association: M4Log::<unnamed>%377C88510370
      //## Role: ClLog::m_slogMode%377C88510372
      //## begin ClLog::m_slogMode%377C88510372.role preserve=no  private: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_slogMode;
      //## end ClLog::m_slogMode%377C88510372.role

      //## Association: M4Log::<unnamed>%377C88510373
      //## Role: ClLog::m_bcrono%377C88510375
      //## begin ClLog::m_bcrono%377C88510375.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bcrono;
      //## end ClLog::m_bcrono%377C88510375.role

    // Additional Private Declarations
      //## begin ClLog%377C88510014.private preserve=yes
      //## end ClLog%377C88510014.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLog%377C88510014.implementation preserve=yes
      //## end ClLog%377C88510014.implementation

};

//## begin ClLog%377C88510014.postscript preserve=yes
//## end ClLog%377C88510014.postscript

//## begin module%377C885003B6.epilog preserve=yes
//## end module%377C885003B6.epilog


#endif
