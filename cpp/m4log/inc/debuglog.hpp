//## begin module%377C88460005.cm preserve=no
//## end module%377C88460005.cm

//## begin module%377C88460005.cp preserve=no
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
//## end module%377C88460005.cp

//## Module: debuglog%377C88460005; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\inc\debuglog.hpp

#ifndef _DEBUGLOG_HPP_
#define _DEBUGLOG_HPP_ 1

//## begin module%377C88460005.additionalIncludes preserve=no
//## end module%377C88460005.additionalIncludes

//## begin module%377C88460005.includes preserve=yes
#include "m4date.hpp"
#include "m4stl.hpp"
//#include "perslog.hpp"
//## end module%377C88460005.includes

// perslog
#include <perslog.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377C88460005.declarations preserve=no
//## end module%377C88460005.declarations

//## begin module%377C88460005.additionalDeclarations preserve=yes

#define M4LOG_GETMETHOD_FIFO		 1	// Forma de cola
#define M4LOG_GETMETHOD_FILO		 2	// Forma de pila
#define M4LOG_GETMETHOD_FIFO_ITER	 3	// n-esimo forma de cola 
#define M4LOG_GETMETHOD_FILO_ITER	 4	// n-esimo forma de pila
#define M4LOG_GETMETHOD_MARKED		 5	// marcado

#define M4LOG_MESSAGE_CODE_ITEM			1
#define M4LOG_MESSAGE_TEXT_ITEM			2
#define M4LOG_MESSAGE_SEVERITY_ITEM		3
#define M4LOG_MESSAGE_TIME_ITEM			4

//class ClPersistentStorage;

class ClStorageList;


//## end module%377C88460005.additionalDeclarations


//## begin ClAppItem%377C88460073.preface preserve=yes
//## end ClAppItem%377C88460073.preface

//## Class: ClAppItem%377C88460073
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C88460135;m4int16_t { -> }

class ClAppItem 
{
  //## begin ClAppItem%377C88460073.initialDeclarations preserve=yes
  //## end ClAppItem%377C88460073.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppItem%1846685448; C++
      //	Espacio que se va a ocupar para guardar el tamaño que ocupa la serialización
      //	completa y la serializacion de cada uno de los errores
      ClAppItem ();

      //## Operation: ClAppItem%582279797; C++
      ClAppItem (m4int32_t lerrCode, m4uint32_t sflags, m4char_t creturn, m4bool_t btime, M4ClString strType, M4ClString text = "");

    //## Destructor (specified)
      //## Operation: ~ClAppItem%-800315767; C++
      ~ClAppItem ();


    //## Other Operations (specified)
      //## Operation: GetErrCode%1993055475; C++
      m4int32_t GetErrCode ();

      //## Operation: GetStrErrCode%451046032; C++
      M4ClString GetStrErrCode ();

      //## Operation: GetErrType%371738280; C++
      M4ClString GetErrType ();

      //## Operation: SetErrCode%748674036; C++
      void SetErrCode (m4int32_t lerrCode);

      //## Operation: GetCReturn%473772215; C++
      m4char_t GetCReturn ();

      //## Operation: SetCReturn%1059605662; C++
      void SetCReturn (m4char_t creturn);

      //## Operation: GetBuffer%-532599070; C++
      m4int16_t GetBuffer (M4ClString& text);

      //## Operation: GetBuffer%2072103689; C++
      M4ClString GetBuffer ();

      //## Operation: BufferSize%-1088959851; C++
      m4int32_t BufferSize ();

      //## Operation: AddText%-1440165114; C++
      void AddText (const M4ClString& text);

      //## Operation: AddTextHead%-1136147123; C++
      void AddTextHead (const M4ClString& stext);

      //## Operation: ClearBuffer%1707966713; C++
      m4int16_t ClearBuffer ();

      //## Operation: CopyAppItem%-1700431489; C++
      ClAppItem* CopyAppItem ();

      //## Operation: IsActive_Time%1012462751; C++
      m4bool_t IsActive_Time ();

      //## Operation: GetTime%1551244818; C++
      M4ClString GetTime ();

      //## Operation: SetTime%301760979; C++
      void SetTime (const M4ClString& strTime);

      //## Operation: FlagAnd%2066443180; C++
      //	Hace el AND lógico bit a bit de en short que se pasa como
      //	parámetro y nuestro m_sflags. El resultado lo vuelca en m_sflags
      //	Salida:
      //	M4_ERROR si sflags < 0
      //	M4_SUCCESS todo correcto
      m4uint32_t FlagAnd (m4uint32_t sflags);

      //## Operation: GetFlags%-226561052; C++
      m4uint32_t GetFlags ();

    // Additional Public Declarations
      //## begin ClAppItem%377C88460073.public preserve=yes
      //## end ClAppItem%377C88460073.public

  protected:
    // Additional Protected Declarations
      //## begin ClAppItem%377C88460073.protected preserve=yes
      //## end ClAppItem%377C88460073.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C88460109
      //## Role: ClAppItem::m_strBuffer%377C8846010B
      //## begin ClAppItem::m_strBuffer%377C8846010B.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_strBuffer;
      //## end ClAppItem::m_strBuffer%377C8846010B.role

      //## Association: M4Log::<unnamed>%377C8846010C
      //## Role: ClAppItem::m_strTime%377C8846010E
      //## begin ClAppItem::m_strTime%377C8846010E.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_strTime;
      //## end ClAppItem::m_strTime%377C8846010E.role

      //## Association: M4Log::<unnamed>%377C8846010F
      //## Role: ClAppItem::m_strErrType%377C88460111
      //## begin ClAppItem::m_strErrType%377C88460111.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_strErrType;
      //## end ClAppItem::m_strErrType%377C88460111.role

      //## Association: M4Log::<unnamed>%377C88460113
      //## Role: ClAppItem::m_lerrCode%377C88460115
      //## begin ClAppItem::m_lerrCode%377C88460115.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_lerrCode;
      //## end ClAppItem::m_lerrCode%377C88460115.role

      //## Association: M4Log::<unnamed>%377C88460116
      //## Role: ClAppItem::m_creturn%377C88460118
      //## begin ClAppItem::m_creturn%377C88460118.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t m_creturn;
      //## end ClAppItem::m_creturn%377C88460118.role

      //## Association: M4Log::<unnamed>%377C88460119
      //## Role: ClAppItem::m_sflags%377C8846011B
      //## begin ClAppItem::m_sflags%377C8846011B.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_sflags;
      //## end ClAppItem::m_sflags%377C8846011B.role

      //## Association: M4Log::<unnamed>%377C8846011C
      //## Role: ClAppItem::m_btime%377C8846011E
      //## begin ClAppItem::m_btime%377C8846011E.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_btime;
      //## end ClAppItem::m_btime%377C8846011E.role

    // Additional Private Declarations
      //## begin ClAppItem%377C88460073.private preserve=yes
      //## end ClAppItem%377C88460073.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppItem%377C88460073.implementation preserve=yes
      //## end ClAppItem%377C88460073.implementation

};

//## begin ClAppItem%377C88460073.postscript preserve=yes



//typedef deque<ClAppItem* >  APPOINTQUEUE;



//## end ClAppItem%377C88460073.postscript

//## begin APPOINTQUEUE%377C884701D3.preface preserve=yes
//## end APPOINTQUEUE%377C884701D3.preface

//## Class: APPOINTQUEUE%377C884701D3
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _Ty%377C8847027E;ClAppItem { -> }
//## Uses: <unnamed>%37820908037F;deque { -> }

typedef deque < ClAppItem * > APPOINTQUEUE;

//## begin APPOINTQUEUE%377C884701D3.postscript preserve=yes
//## end APPOINTQUEUE%377C884701D3.postscript

//## begin ClAppointments%377C88460147.preface preserve=yes
//## end ClAppointments%377C88460147.preface

//## Class: ClAppointments%377C88460147
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C884701F8;ClStorageList { -> }
//## Uses: <unnamed>%377C884701FE;ClAppItem { -> }
//## Uses: <unnamed>%377C88470201;m4uint32_t { -> }
//## Uses: <unnamed>%377C88470206;m4int16_t { -> }
//## Uses: <unnamed>%377C88470209;m4int32_t { -> }
//## Uses: <unnamed>%377C8847020B;m4return_t { -> }
//## Uses: <unnamed>%377C8847020C;m4pchar_t { -> }
//## Uses: <unnamed>%377C88470275;m4bool_t { -> }

class M4_DECL_M4LOG ClAppointments 
{
  //## begin ClAppointments%377C88460147.initialDeclarations preserve=yes
  //## end ClAppointments%377C88460147.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppointments%535977362; C++
      ClAppointments (m4bool_t ai_shared = M4_FALSE);

      //## Operation: ClAppointments%-883074347; C++
      ClAppointments (ClStorageList* plstorages, m4bool_t ai_shared = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~ClAppointments%-723426398; C++
      ~ClAppointments ();


    //## Other Operations (specified)
      //## Operation: GetCReturn%-1053035615; C++
      m4char_t GetCReturn ();

      //## Operation: SetCReturn%352196340; C++
      void SetCReturn (m4char_t creturn);

      //## Operation: GetStorageList%1033586053; C++
      ClStorageList* GetStorageList ();

      //## Operation: SetStorageList%-847460015; C++
      void SetStorageList (ClStorageList* plstorages);

      //## Operation: AddAppoint%-806800384; C++
      void AddAppoint (ClAppItem* pappoint);

      //## Operation: StoreAppoints%-1208000047; C++
      //	esto está de forma que solo guarda en un medio y después elimina
      //	el objeto de ClAppItem de memoria.
      void StoreAppoints (m4uint32_t sflags);

      //## Operation: StoreAppoint%1526745902; C++
      void StoreAppoint (m4uint32_t sflags);

      //## Operation: AddAppointsTo%-1281544253; C++
      //	Añade a pappoints la lista de apuntes que contenemos en m_pappoints
      //	return:
      //	M4_ERROR si el carácter de retorno de las lista no coincide
      //	M4_SUCCESS	si la lista ha sido copiada.
      m4int16_t AddAppointsTo (ClAppointments* pappoints);

      //## Operation: GetErrorQueueSize%724450828; C++
      m4int32_t GetErrorQueueSize ();

      //## Operation: GetErrorTime%-891402757; C++
      m4return_t GetErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorTime%-126725663; C++
      m4return_t GetLastErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorTime%435707202; C++
      m4return_t GetMarkedErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorTime%2045075710; C++
      m4return_t Get_n_ErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorTime%1044764995; C++
      m4return_t Get_n_LastErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorCode%1649465942; C++
      m4return_t GetErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorCode%2126423985; C++
      m4return_t GetErrorCode (m4int32_t& ao_lErrorCode);

      //## Operation: GetLastErrorCode%-364837482; C++
      m4return_t GetLastErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorCode%-50544292; C++
      m4return_t GetLastErrorCode (m4int32_t& ao_lErrorCode);

      //## Operation: GetMarkedErrorCode%497573662; C++
      m4return_t GetMarkedErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorCode%-575868519; C++
      m4return_t Get_n_ErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorCode%1279298605; C++
      m4return_t Get_n_ErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode);

      //## Operation: Get_n_LastErrorCode%-1270666693; C++
      m4return_t Get_n_LastErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorCode%101542522; C++
      m4return_t Get_n_LastErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode);

      //## Operation: GetErrorType%-395443388; C++
      m4return_t GetErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorType%-1716724223; C++
      m4return_t GetLastErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorType%-1768173730; C++
      m4return_t GetMarkedErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorType%1599231282; C++
      m4return_t Get_n_ErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorType%429777790; C++
      m4return_t Get_n_LastErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetErrorText%1659764382; C++
      m4return_t GetErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetLastErrorText%1187655810; C++
      m4return_t GetLastErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetMarkedErrorText%-1085108578; C++
      m4return_t GetMarkedErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_ErrorText%491644706; C++
      m4return_t Get_n_ErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: Get_n_LastErrorText%1596771516; C++
      m4return_t Get_n_LastErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: SetMark%1502639630; C++
      m4return_t SetMark ();

      //## Operation: MoveMark%1639088559; C++
      m4return_t MoveMark ();

      //## Operation: IsValidMark%1637253055; C++
      m4return_t IsValidMark ();

      //## Operation: Pop%1940072994; C++
      m4return_t Pop ();

      //## Operation: Pop_back%600430664; C++
      m4return_t Pop_back ();

      //## Operation: GetSize%750365623; C++
      m4uint32_t GetSize ();

      //## Operation: Empty%-2076019487; C++
      m4bool_t Empty ();

      //## Operation: GetSerialStringSizeFromMessage%938426073
      m4return_t GetSerialStringSizeFromMessage (m4uint32_t& ao_StringSize, m4uint32_t ai_MessageCounter = 0);

      //## Operation: GetSerialStringFromMessage%938426074
      m4return_t GetSerialStringFromMessage (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter, m4uint32_t ai_MessageCounter = 0);

      //## Operation: GetMessageCounter%938426075
      m4int32_t GetMessageCounter ();

      //## Operation: GetMaxQueueSize%644034027; C++
      m4uint32_t GetMaxQueueSize ();

      //## Operation: SetMaxQueueSize%-104986863; C++
      void SetMaxQueueSize (m4uint32_t ai_MaxQueueSize);

      //## Operation: GetMessageItem%938705723
      m4return_t GetMessageItem (const m4pchar_t ai_getMethodTag, m4uint32_t ai_itemOffset, const m4pchar_t ai_itemTag, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied);

      //## Operation: GetAppoint%944066182
      ClAppItem* GetAppoint (m4uint32_t ai_iterator);

    // Additional Public Declarations
      //## begin ClAppointments%377C88460147.public preserve=yes
      //## end ClAppointments%377C88460147.public

  protected:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C884601BD
      //## Role: ClAppointments::m_creturn%377C884601BF
      //## begin ClAppointments::m_creturn%377C884601BF.role preserve=no  protected: m4char_t {1 -> 1VHAPN}
      m4char_t m_creturn;
      //## end ClAppointments::m_creturn%377C884601BF.role

      //## Association: M4Log::<unnamed>%377C884701D5
      //## Role: ClAppointments::m_qappoints%377C884701D7
      //## begin ClAppointments::m_qappoints%377C884701D7.role preserve=no  protected: APPOINTQUEUE {1 -> 1VHAPN}
      APPOINTQUEUE m_qappoints;
      //## end ClAppointments::m_qappoints%377C884701D7.role

      //## Association: M4Log::<unnamed>%377C884701D8
      //## Role: ClAppointments::m_plstorages%377C884701DA
      //## begin ClAppointments::m_plstorages%377C884701DA.role preserve=no  protected: ClStorageList {1 -> 1RHAPN}
      ClStorageList *m_plstorages;
      //## end ClAppointments::m_plstorages%377C884701DA.role

      //## Association: M4Log::<unnamed>%377C884701F2
      //## Role: ClAppointments::m_uiNoMark%377C884701F4
      //## begin ClAppointments::m_uiNoMark%377C884701F4.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_uiNoMark;
      //## end ClAppointments::m_uiNoMark%377C884701F4.role

      //## Association: M4Log::<unnamed>%377C884701F5
      //## Role: ClAppointments::m_MaxQueueSize%377C884701F7
      //## begin ClAppointments::m_MaxQueueSize%377C884701F7.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_MaxQueueSize;
      //## end ClAppointments::m_MaxQueueSize%377C884701F7.role

    // Additional Protected Declarations
      //## begin ClAppointments%377C88460147.protected preserve=yes
      //## end ClAppointments%377C88460147.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oMark%37820AAF03CD
      //## begin ClAppointments::m_oMark%37820AAF03CD.attr preserve=no  private: APPOINTQUEUE::iterator {UA} 
      APPOINTQUEUE::iterator m_oMark;
      //## end ClAppointments::m_oMark%37820AAF03CD.attr

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%37E9E9670195
      //## Role: ClAppointments::m_AccessMutex%37E9E9670380
      //## begin ClAppointments::m_AccessMutex%37E9E9670380.role preserve=no  private: ClMutex { -> VHAN}
      ClMutex m_AccessMutex;
      //## end ClAppointments::m_AccessMutex%37E9E9670380.role

      //## Association: M4Log::<unnamed>%37EA2BC50308
      //## Role: ClAppointments::m_bShared%37EA2BC60179
      //## begin ClAppointments::m_bShared%37EA2BC60179.role preserve=no  private: m4bool_t { -> VHAN}
      m4bool_t m_bShared;
      //## end ClAppointments::m_bShared%37EA2BC60179.role

      //## Association: M4Log::<unnamed>%37EF3E8B0218
      //## Role: ClAppointments::m_uiMessageCounter%37EF3E8C0089
      //## begin ClAppointments::m_uiMessageCounter%37EF3E8C0089.role preserve=no  private: m4uint32_t { -> VHAN}
      m4uint32_t m_uiMessageCounter;
      //## end ClAppointments::m_uiMessageCounter%37EF3E8C0089.role

    // Additional Private Declarations
      //## begin ClAppointments%377C88460147.private preserve=yes
      //## end ClAppointments%377C88460147.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppointments%377C88460147.implementation preserve=yes
      //## end ClAppointments%377C88460147.implementation

};

//## begin ClAppointments%377C88460147.postscript preserve=yes
//## end ClAppointments%377C88460147.postscript

//## begin module%377C88460005.epilog preserve=yes
//## end module%377C88460005.epilog


#endif
