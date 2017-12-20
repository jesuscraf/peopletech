//## begin module%377C884E0186.cm preserve=no
//## end module%377C884E0186.cm

//## begin module%377C884E0186.cp preserve=no
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
//## end module%377C884E0186.cp

//## Module: logsys%377C884E0186; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\Work\v600\m4log\inc\logsys.hpp

#ifndef _LOGSYS_HPP_
#define _LOGSYS_HPP_ 1

//## begin module%377C884E0186.additionalIncludes preserve=no
//## end module%377C884E0186.additionalIncludes

//## begin module%377C884E0186.includes preserve=yes


//#include "m4string.hpp"

//#include "debuglog.hpp"
//#include "dbgresou.hpp"
//#include "perslog.hpp"
//#include "m4log.hpp"
//## end module%377C884E0186.includes

// logmanag
#include <logmanag.hpp>
// debuglog
#include <debuglog.hpp>
// perslog
#include <perslog.hpp>
// dbgresou
#include <dbgresou.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377C884E0186.declarations preserve=no
//## end module%377C884E0186.declarations

//## begin module%377C884E0186.additionalDeclarations preserve=yes




#define	M4LOG_OVER_ERROR					0x01
#define	M4LOG_OVER_WARNING					0x02
#define	M4LOG_OVER_DEBUG					0x04
#define	M4LOG_OVER_TRACE					0x08
#define	M4LOG_OVER_DEFAULT_ERROR			0x10
#define	M4LOG_OVER_DEFAULT_WARNING			0x20
#define	M4LOG_OVER_DEFAULT_DEBUG			0x40
#define	M4LOG_OVER_DEFAULT_TRACE			0x80

#define	M4LOG_OVER_NONE						0x00
#define	M4LOG_OVER_ALL						0xFF




const m4uint32_t  MAX_BUFFER_SIZE = (8*1024)+ 128;	// Tamaño maximo de buffer 8Kb + (32*4) atributos serializados


class ClLogManager;


//## end module%377C884E0186.additionalDeclarations


//## begin ClLogSystem%377C884F02D8.preface preserve=yes
//## end ClLogSystem%377C884F02D8.preface

//## Class: ClLogSystem%377C884F02D8
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C8850017A;m4int16_t { -> }
//## Uses: <unnamed>%377C8850017B;m4pchar_t { -> }
//## Uses: <unnamed>%377C8850017C;m4uint32_t { -> }
//## Uses: <unnamed>%377C88500181;ClLogManager { -> }
//## Uses: <unnamed>%377C88500182;m4bool_t { -> }
//## Uses: <unnamed>%377C88500187;m4return_t { -> }
//## Uses: <unnamed>%377C8850018B;ClStaticResourceList { -> }
//## Uses: <unnamed>%377C88500190;ClFileStorage { -> }
//## Uses: <unnamed>%377C8850019A;m4double_t { -> }
//## Uses: <unnamed>%377C8850019E;M4ClString { -> }
//## Uses: <unnamed>%377C885001AB;m4int32_t { -> }
//## Uses: <unnamed>%377C885001BA;ClAppointments { -> }
//## Uses: <unnamed>%377C885001BE;m4char_t { -> }
//## Uses: <unnamed>%377C885001C5;m4int8_t { -> }
//## Uses: <unnamed>%377C885001CC;ClPersistentStorage { -> }

class M4_DECL_M4LOG ClLogSystem 
{
  //## begin ClLogSystem%377C884F02D8.initialDeclarations preserve=yes
  //## end ClLogSystem%377C884F02D8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLogSystem%857762628; C++
      //	#include "m4date.hpp"
      ClLogSystem ();

      //## Operation: ClLogSystem%1779839184; C++
      ClLogSystem (ClLogManager* plogManager, m4bool_t ai_bshowTime, m4bool_t ai_Shared = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~ClLogSystem%163138753; C++
      ~ClLogSystem ();


    //## Other Operations (specified)
      //## Operation: SetLevelInfo%1837704381; C++
      m4return_t SetLevelInfo (m4uint32_t ilevelInfo);

      //## Operation: StorageAlways%-1351991195; C++
      m4bool_t StorageAlways ()
      {
        //## begin ClLogSystem::StorageAlways%-1351991195.body preserve=yes
		return m_bStorageAlways;
        //## end ClLogSystem::StorageAlways%-1351991195.body
      }

      //## Operation: UnitTime%1178713168; C++
      m4double_t UnitTime ()
      {
        //## begin ClLogSystem::UnitTime%1178713168.body preserve=yes
		 return m_dUnitTime;
        //## end ClLogSystem::UnitTime%1178713168.body
      }

      //## Operation: AddTime%721590391; C++
      void AddTime (m4double_t dTime);

      //## Operation: SetAttributeValue%817204118; C++
      m4return_t SetAttributeValue (const M4ClString& ai_attName, const M4ClString& ai_attValue);

      //## Operation: GetAttributeValue%-1155364905; C++
      M4ClString* GetAttributeValue (const M4ClString& ai_attName);

      //## Operation: IsActive%-1455066329; C++
      m4bool_t IsActive ();

      //## Operation: SetActive%1242350385; C++
      void SetActive ();

      //## Operation: SetDesactive%-1136887259; C++
      void SetDesactive ();

      //## Operation: StartCode%-478318419; C++
      m4return_t StartCode (m4int32_t icode, m4int16_t irango);

      //## Operation: AddText%1816164496; C++
      m4return_t AddText (const M4ClString& text);

      //## Operation: SetCode%-1971197118; C++
      m4int16_t SetCode ();

      //## Operation: SetCode%940267791
      m4return_t SetCode (m4int32_t ai_icode, m4int16_t ai_irango, const M4ClString& ai_text);

      //## Operation: StoreAppoints%725848232; C++
      m4int16_t StoreAppoints (m4uint32_t sstorages);

      //## Operation: StoreAppoint%1122231137; C++
      m4int16_t StoreAppoint (m4uint32_t sstorages);

      //## Operation: AddAppoints%-485397852; C++
      m4int16_t AddAppoints (ClAppointments* pappoints);

      //## Operation: GetAppointments%1565900245; C++
      ClAppointments* GetAppointments ();

      //## Operation: GetCReturn%517091994; C++
      m4char_t GetCReturn ();

      //## Operation: SetCReturn%-505633360; C++
      void SetCReturn (m4char_t creturn);

      //## Operation: SetStorageFlags%-1931512826; C++
      void SetStorageFlags (m4int8_t sstorages);

      //## Operation: GetStorageFlags%-496079555; C++
      m4int8_t GetStorageFlags ();

      //## Operation: AddStorage%-60102562; C++
      m4int16_t AddStorage (ClPersistentStorage* pstorage);

      //## Operation: getShowTime%1275998738; C++
      m4bool_t getShowTime ();

      //## Operation: setShowTime%1678935944; C++
      void setShowTime (m4bool_t ai_bshowTime);

      //## Operation: EnableDebugInfoLogs%-1855435205; C++
      void EnableDebugInfoLogs ();

      //## Operation: EnableErrorLogs%-1725523882; C++
      void EnableErrorLogs ();

      //## Operation: EnableWarningLogs%921707927; C++
      void EnableWarningLogs ();

      //## Operation: EnableTraceInfo%-375840880; C++
      void EnableTraceInfo ();

      //## Operation: DisableDebugInfoLogs%-818867976; C++
      void DisableDebugInfoLogs ();

      //## Operation: DisableErrorLogs%1806787717; C++
      void DisableErrorLogs ();

      //## Operation: DisableWarningLogs%-453396481; C++
      void DisableWarningLogs ();

      //## Operation: DisableTraceInfo%649385769; C++
      void DisableTraceInfo ();

      //## Operation: EnableAllLogs%-1090641485; C++
      void EnableAllLogs ();

      //## Operation: DisableAllLogs%-2043793796; C++
      void DisableAllLogs ();

      //## Operation: EnableLocalQueue%-1767120967; C++
      void EnableLocalQueue ();

      //## Operation: DisableLocalQueue%2050630872; C++
      void DisableLocalQueue ();

      //## Operation: LocalQueue%1795830953; C++
      m4bool_t LocalQueue ();

      //## Operation: GetErrorQueueSize%-489249013; C++
      m4int32_t GetErrorQueueSize ();

      //## Operation: GetErrorTime%-49906718; C++
      m4return_t GetErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorTime%930282884; C++
      m4return_t GetLastErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorTime%1695511415; C++
      m4return_t GetMarkedErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorTime%-1892375191; C++
      m4return_t Get_n_ErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorTime%-1060953250; C++
      m4return_t Get_n_LastErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorCode%-206691613; C++
      m4return_t GetErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorCode%580516220; C++
      m4return_t GetErrorCode (m4int32_t& ao_lErrorCode);

      //## Operation: GetLastErrorCode%-1974388481; C++
      m4return_t GetLastErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorCode%121857368; C++
      m4return_t GetLastErrorCode (m4int32_t& ao_lErrorCode);

      //## Operation: GetMarkedErrorCode%-2081079982; C++
      m4return_t GetMarkedErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorCode%304627155; C++
      m4return_t Get_n_ErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorCode%-83220392; C++
      m4return_t Get_n_ErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode);

      //## Operation: Get_n_LastErrorCode%488314268; C++
      m4return_t Get_n_LastErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorCode%-1926845934; C++
      m4return_t Get_n_LastErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode);

      //## Operation: GetErrorType%1253421162; C++
      m4return_t GetErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorType%1736829055; C++
      m4return_t GetLastErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorType%-1860410302; C++
      m4return_t GetMarkedErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorType%1716573295; C++
      m4return_t Get_n_ErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorType%1411860021; C++
      m4return_t Get_n_LastErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorText%-1356696851; C++
      m4return_t GetErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorText%355415495; C++
      m4return_t GetLastErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorText%1092053750; C++
      m4return_t GetMarkedErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorText%-1521933552; C++
      m4return_t Get_n_ErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorText%341476931; C++
      m4return_t Get_n_LastErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: SetSerialCode%-1544686540; C++
      m4int16_t SetSerialCode (m4int32_t ai_Size, m4pchar_t ai_SerialCad);

      //## Operation: SetSerialCodeString%533173649; C++
      m4int16_t SetSerialCodeString (m4pchar_t ai_SerialCad);

      //## Operation: SetMark%-467947494; C++
      m4return_t SetMark ();

      //## Operation: MoveMark%-1001893605; C++
      m4return_t MoveMark ();

      //## Operation: IsValidMark%1941229095; C++
      m4return_t IsValidMark ();

      //## Operation: Pop%1104823095; C++
      m4return_t Pop ();

      //## Operation: Pop_back%-771935752; C++
      m4return_t Pop_back ();

      //## Operation: Empty%-1359050984; C++
      m4bool_t Empty ();

      //## Operation: toString%241386615; C++
      m4return_t toString (m4pchar_t aio_FileName, m4uint32_t ai_stringSize);

      //## Operation: GetSerialStringSizeFromMessage%938426076
      m4return_t GetSerialStringSizeFromMessage (m4uint32_t& ao_StringSize, m4uint32_t ai_MessageCounter = 0);

      //## Operation: GetSerialStringFromMessage%938426077
      m4return_t GetSerialStringFromMessage (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter, m4uint32_t ai_MessageCounter = 0);

      //## Operation: GetMessageCounter%938426078
      m4int32_t GetMessageCounter ();

	  void		ResetUserTimeAudit();

      m4pchar_t	GetSessionKey();
      
	  m4pchar_t	GetServerName();
      
	  int		GetServerPort();
      
	  int		GetSessionId();
      
	  int		GetClickNum();

	  m4pchar_t	GetReqUniqueId();
      
      void		SetSessionKey( m4pchar_t ai_pcSessionKey );

      void		SetServerName( m4pcchar_t ai_pcServerName );

      void		SetServerPort( int ai_iServerPort );

      void		SetSessionId( int ai_iSessionId );

      void		SetClickNum( int ai_iClickNum );

	  void		SetReqUniqueId( m4pchar_t ai_pcReqUniqueId );

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: IdThread%3FC4868200F7
      const m4uint32_t& GetIdThread () const
      {
        //## begin ClLogSystem::GetIdThread%3FC4868200F7.get preserve=no
        return m_IdThread;
        //## end ClLogSystem::GetIdThread%3FC4868200F7.get
      }

      void SetIdThread (const m4uint32_t& value)
      {
        //## begin ClLogSystem::SetIdThread%3FC4868200F7.set preserve=no
        m_IdThread = value;
        //## end ClLogSystem::SetIdThread%3FC4868200F7.set
      }

	  m4uint8_t	GetOverwriteMask( void ) const
	  {
		  return( m_iOverwriteMask ) ;
	  }

	  void	SetOverwriteMask( m4uint8_t ai_iOverwriteMask )
	  {
		  m_iOverwriteMask = ai_iOverwriteMask ;
	  }

	  m4return_t	ResetOverwriteMask( ClLogManager* ai_pLogManager ) ;

	  ILogListener*	RegisterListener( ILogListener *ai_poListener )
	  {
		  ILogListener* poListener = m_poListener;
		  m_poListener = ai_poListener ;
		  return( poListener ) ;
	  }

    // Additional Public Declarations
      //## begin ClLogSystem%377C884F02D8.public preserve=yes
      //## end ClLogSystem%377C884F02D8.public

  protected:
    // Data Members for Class Attributes

      //## begin ClLogSystem::IdThread%3FC4868200F7.attr preserve=no  public: m4uint32_t {U} 0
      m4uint32_t m_IdThread;
      //## end ClLogSystem::IdThread%3FC4868200F7.attr

    // Additional Protected Declarations
      //## begin ClLogSystem%377C884F02D8.protected preserve=yes
      //## end ClLogSystem%377C884F02D8.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetDistance%1658248374; C++
      m4int16_t GetDistance (m4pchar_t& cad, m4uint32_t Size);

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C88500149
      //## Role: ClLogSystem::m_bactivated%377C8850014B
      //## begin ClLogSystem::m_bactivated%377C8850014B.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bactivated;
      //## end ClLogSystem::m_bactivated%377C8850014B.role

      //## Association: M4Log::<unnamed>%377C8850014C
      //## Role: ClLogSystem::m_ilevelInfo%377C8850014E
      //## begin ClLogSystem::m_ilevelInfo%377C8850014E.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_ilevelInfo;
      //## end ClLogSystem::m_ilevelInfo%377C8850014E.role

      //## Association: M4Log::<unnamed>%377C8850014F
      //## Role: ClLogSystem::m_icode%377C88500151
      //## begin ClLogSystem::m_icode%377C88500151.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_icode;
      //## end ClLogSystem::m_icode%377C88500151.role

      //## Association: M4Log::<unnamed>%377C88500153
      //## Role: ClLogSystem::m_creturn%377C88500155
      //## begin ClLogSystem::m_creturn%377C88500155.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t m_creturn;
      //## end ClLogSystem::m_creturn%377C88500155.role

      //## Association: M4Log::<unnamed>%377C88500156
      //## Role: ClLogSystem::m_cEndMessage%377C88500158
      //## begin ClLogSystem::m_cEndMessage%377C88500158.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t m_cEndMessage;
      //## end ClLogSystem::m_cEndMessage%377C88500158.role

      //## Association: M4Log::<unnamed>%377C88500159
      //## Role: ClLogSystem::m_sstorages%377C8850015B
      //## begin ClLogSystem::m_sstorages%377C8850015B.role preserve=no  private: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sstorages;
      //## end ClLogSystem::m_sstorages%377C8850015B.role

      //## Association: M4Log::<unnamed>%377C8850015C
      //## Role: ClLogSystem::m_sTypeFlags%377C8850015E
      //## begin ClLogSystem::m_sTypeFlags%377C8850015E.role preserve=no  private: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sTypeFlags;
      //## end ClLogSystem::m_sTypeFlags%377C8850015E.role

      //## Association: M4Log::<unnamed>%377C8850015F
      //## Role: ClLogSystem::m_sLocalQueue%377C88500161
      //## begin ClLogSystem::m_sLocalQueue%377C88500161.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_sLocalQueue;
      //## end ClLogSystem::m_sLocalQueue%377C88500161.role

      //## Association: M4Log::<unnamed>%377C88500162
      //## Role: ClLogSystem::m_bshowTime%377C88500164
      //## begin ClLogSystem::m_bshowTime%377C88500164.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bshowTime;
      //## end ClLogSystem::m_bshowTime%377C88500164.role

      //## Association: M4Log::<unnamed>%377C88500165
      //## Role: ClLogSystem::m_UserId%377C88500167
      //## begin ClLogSystem::m_UserId%377C88500167.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_UserId;
      //## end ClLogSystem::m_UserId%377C88500167.role

      //## Association: M4Log::<unnamed>%377C88500168
      //## Role: ClLogSystem::m_dUnitTime%377C8850016A
      //## begin ClLogSystem::m_dUnitTime%377C8850016A.role preserve=no  private: m4double_t {1 -> 1VHAPN}
      m4double_t m_dUnitTime;
      //## end ClLogSystem::m_dUnitTime%377C8850016A.role

      //## Association: M4Log::<unnamed>%377C8850016B
      //## Role: ClLogSystem::m_bStorageAlways%377C8850016D
      //## begin ClLogSystem::m_bStorageAlways%377C8850016D.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bStorageAlways;
      //## end ClLogSystem::m_bStorageAlways%377C8850016D.role

      //## Association: M4Log::<unnamed>%377C8850016E
      //## Role: ClLogSystem::m_plogManager%377C88500170
      //## begin ClLogSystem::m_plogManager%377C88500170.role preserve=no  private: ClLogManager {1 -> 1RHAPN}
      ClLogManager *m_plogManager;
      //## end ClLogSystem::m_plogManager%377C88500170.role

      //## Association: M4Log::<unnamed>%377C88500171
      //## Role: ClLogSystem::m_plstorages%377C88500173
      //## begin ClLogSystem::m_plstorages%377C88500173.role preserve=no  private: ClStorageList {1 -> 1RHAPN}
      ClStorageList *m_plstorages;
      //## end ClLogSystem::m_plstorages%377C88500173.role

      //## Association: M4Log::<unnamed>%377C88500174
      //## Role: ClLogSystem::m_pappoint%377C88500176
      //## begin ClLogSystem::m_pappoint%377C88500176.role preserve=no  private: ClAppItem {1 -> 1RHAPN}
      ClAppItem *m_pappoint;
      //## end ClLogSystem::m_pappoint%377C88500176.role

      //## Association: M4Log::<unnamed>%377C88500177
      //## Role: ClLogSystem::m_lappoints%377C88500179
      //## begin ClLogSystem::m_lappoints%377C88500179.role preserve=no  private: ClAppointments {1 -> 1VHAPN}
      ClAppointments m_lappoints;
      //## end ClLogSystem::m_lappoints%377C88500179.role

      //## Association: M4Log::<unnamed>%380B580B011A
      //## Role: ClLogSystem::m_AccessMutex%380B580B026E
      //## begin ClLogSystem::m_AccessMutex%380B580B026E.role preserve=no  private: ClMutex { -> VHAN}
      ClMutex m_AccessMutex;
      //## end ClLogSystem::m_AccessMutex%380B580B026E.role

      //## Association: M4Log::<unnamed>%380B599C013E
      //## Role: ClLogSystem::m_bShared%380B599C031F
      //## begin ClLogSystem::m_bShared%380B599C031F.role preserve=no  private: m4bool_t { -> VHAN}
      m4bool_t m_bShared;
      //## end ClLogSystem::m_bShared%380B599C031F.role

	  m4uint8_t m_iOverwriteMask ;

	  // Listener de eventos
	  ILogListener *m_poListener ;

	  // User time auditory attributes.
	  m4pchar_t	m_pcSessionKey;

	  m4pchar_t	m_pcServerName;

	  int		m_iServerPort;

	  int		m_iSessionId;

	  int		m_iClickNum;

	  m4pchar_t	m_pcReqUniqueId;

    // Additional Private Declarations
      //## begin ClLogSystem%377C884F02D8.private preserve=yes
      //## end ClLogSystem%377C884F02D8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLogSystem%377C884F02D8.implementation preserve=yes
      //## end ClLogSystem%377C884F02D8.implementation

};

//## begin ClLogSystem%377C884F02D8.postscript preserve=yes
//## end ClLogSystem%377C884F02D8.postscript

//## begin module%377C884E0186.epilog preserve=yes
//## end module%377C884E0186.epilog


#endif
