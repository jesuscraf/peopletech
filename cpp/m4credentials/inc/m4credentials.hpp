//## begin module%3959D0D102B8.cm preserve=no
//## end module%3959D0D102B8.cm

//## begin module%3959D0D102B8.cp preserve=no
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
//## end module%3959D0D102B8.cp

//## Module: m4credentials%3959D0D102B8; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: F:\integration\m4credentials\inc\m4credentials.hpp

#ifndef m4credentials_h
#define m4credentials_h 1

//## begin module%3959D0D102B8.additionalIncludes preserve=no
//## end module%3959D0D102B8.additionalIncludes

//## begin module%3959D0D102B8.includes preserve=yes
#include "m4credentials_dll.hpp"
//## end module%3959D0D102B8.includes

// m4string
#include <m4string.hpp>
#include "m4stl.hpp"
//## begin module%3959D0D102B8.declarations preserve=no
//## end module%3959D0D102B8.declarations

//## begin module%3959D0D102B8.additionalDeclarations preserve=yes
//## end module%3959D0D102B8.additionalDeclarations


//## begin ClRoleInfo%3959C6BA0368.preface preserve=yes
//## end ClRoleInfo%3959C6BA0368.preface

//## Class: ClRoleInfo%3959C6BA0368
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3959DE61002F;m4string_t { -> }

class M4_DECL_M4CREDENTIALS ClRoleInfo
{
  //## begin ClRoleInfo%3959C6BA0368.initialDeclarations preserve=yes
  //## end ClRoleInfo%3959C6BA0368.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRoleInfo%962187522
      ClRoleInfo (const m4string_t &ai_strRoleID, const m4string_t &ai_strRSMID);

      //## Operation: ClRoleInfo%962187528
      ClRoleInfo (const ClRoleInfo &ai_oRoleInfo);

    //## Destructor (specified)
      //## Operation: ~ClRoleInfo%962187524
      ~ClRoleInfo ();

    // Data Members for Class Attributes

      //## Attribute: m_strRoleID%3959C6D803BB
      //## begin ClRoleInfo::m_strRoleID%3959C6D803BB.attr preserve=no  public: m4string_t {VAC} 
      const m4string_t  m_strRoleID;
      //## end ClRoleInfo::m_strRoleID%3959C6D803BB.attr

      //## Attribute: m_strRSMID%3959C6FE037A
      //## begin ClRoleInfo::m_strRSMID%3959C6FE037A.attr preserve=no  public: m4string_t {UAC} 
      const m4string_t  m_strRSMID;
      //## end ClRoleInfo::m_strRSMID%3959C6FE037A.attr

    // Additional Public Declarations
      //## begin ClRoleInfo%3959C6BA0368.public preserve=yes
      //## end ClRoleInfo%3959C6BA0368.public

  protected:
    // Additional Protected Declarations
      //## begin ClRoleInfo%3959C6BA0368.protected preserve=yes
      //## end ClRoleInfo%3959C6BA0368.protected

  private:
    //## Constructors (specified)
      //## Operation: ClRoleInfo%962187523
      ClRoleInfo ();


    //## Other Operations (specified)
      //## Operation: operator=%962187529
      ClRoleInfo & operator = (const ClRoleInfo &ai_oRHSRoleInfo);

    // Additional Private Declarations
      //## begin ClRoleInfo%3959C6BA0368.private preserve=yes
      //## end ClRoleInfo%3959C6BA0368.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRoleInfo%3959C6BA0368.implementation preserve=yes
      //## end ClRoleInfo%3959C6BA0368.implementation

};

//## begin ClRoleInfo%3959C6BA0368.postscript preserve=yes
//## end ClRoleInfo%3959C6BA0368.postscript

//## begin M4Credentials%3959C68E0328.preface preserve=yes
//## end M4Credentials%3959C68E0328.preface

//## Class: M4Credentials%3959C68E0328
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3959DE5D005C;m4string_t { -> }

class M4_DECL_M4CREDENTIALS M4Credentials
{
  //## begin M4Credentials%3959C68E0328.initialDeclarations preserve=yes
  //## end M4Credentials%3959C68E0328.initialDeclarations

  protected:
    //## begin M4Credentials::TRoleInfoList%3959CDCA03CC.preface preserve=yes
    //## end M4Credentials::TRoleInfoList%3959CDCA03CC.preface

    //## Class: TRoleInfoList%3959CDCA03CC; protected; Instantiated Class
    //## Category: M4Credentials%3959C61A006F
    //## Subsystem: M4Credentials::inc%3959D0BB013A
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    //## Uses: <unnamed>%3959C7A60399;ClRoleInfo { -> }

    typedef list< ClRoleInfo > TRoleInfoList;

    //## begin M4Credentials::TRoleInfoList%3959CDCA03CC.postscript preserve=yes
    //## end M4Credentials::TRoleInfoList%3959CDCA03CC.postscript

  public:
    //## Class: TRoleInfoListIterator%3959CE6E0396
    //## Category: M4Credentials%3959C61A006F
    //## Subsystem: M4Credentials::inc%3959D0BB013A
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef TRoleInfoList::const_iterator TRoleInfoListIterator;

  protected:
    //## begin M4Credentials::TInternalListIterator%395A119D02CE.preface preserve=yes
    //## end M4Credentials::TInternalListIterator%395A119D02CE.preface

    //## Class: TInternalListIterator%395A119D02CE; protected
    //## Category: M4Credentials%3959C61A006F
    //## Subsystem: M4Credentials%3959C62403A8
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef TRoleInfoList::iterator TInternalListIterator;

    //## begin M4Credentials::TInternalListIterator%395A119D02CE.postscript preserve=yes
    //## end M4Credentials::TInternalListIterator%395A119D02CE.postscript

  public:
    //## Constructors (specified)
      //## Operation: M4Credentials%962266646
      M4Credentials (const M4Credentials &ai_oCredentials);

    //## Destructor (specified)
      //## Operation: ~M4Credentials%962187530
      ~M4Credentials ();


    //## Other Operations (specified)
      //## Operation: GetRoleListBegin%962187525; C++
      M4Credentials::TRoleInfoListIterator GetRoleListBegin () const;

      //## Operation: GetRoleListEnd%962187526; C++
      M4Credentials::TRoleInfoListIterator GetRoleListEnd () const;

      //## Operation: Serialize%962187520
      m4return_t Serialize (m4string_t &ao_strBuffer) const;

      //## Operation: Deserialize%962187521
      static M4Credentials * Deserialize (const m4string_t &ai_strBuffer);

      //## Operation: IsUserInRole%963823177
      m4bool_t IsUserInRole (const m4string_t &ai_strRoleID) const;

      //## Operation: IsUserInRSM%963823178
      m4bool_t IsUserInRSM (const m4string_t &ai_strRSMID) const;

      //## Operation: GetSessId%985334637
      m4uint64_t GetSessId ();

      //## Operation: SetSessId%985334638
      void SetSessId (m4uint64_t ai_lSessId);

    // Data Members for Class Attributes

      //## Attribute: m_strLogin%3959CF5900D6
      //## begin M4Credentials::m_strLogin%3959CF5900D6.attr preserve=no  public: m4string_t {VAC} 
      const m4string_t  m_strLogin;
      //## end M4Credentials::m_strLogin%3959CF5900D6.attr

      //## Attribute: m_strSessionID%3959CF73028C
      //## begin M4Credentials::m_strSessionID%3959CF73028C.attr preserve=no  public: m4string_t {UAC} 
      const m4string_t  m_strSessionID;
      //## end M4Credentials::m_strSessionID%3959CF73028C.attr

      //## Attribute: M4CredTokenSeparator%396C7F3500FD
      //## begin M4Credentials::M4CredTokenSeparator%396C7F3500FD.attr preserve=no  public: static m4char_t {VAC} 1
      static const m4char_t  M4CredTokenSeparator;
      //## end M4Credentials::M4CredTokenSeparator%396C7F3500FD.attr

      //## Attribute: M4CredEndSeparator%396C7F800187
      //## begin M4Credentials::M4CredEndSeparator%396C7F800187.attr preserve=no  public: static m4char_t {UAC} 2
      static const m4char_t  M4CredEndSeparator;
      //## end M4Credentials::M4CredEndSeparator%396C7F800187.attr

    // Additional Public Declarations
      //## begin M4Credentials%3959C68E0328.public preserve=yes
      //## end M4Credentials%3959C68E0328.public

  protected:
    // Data Members for Associations

      //## Association: M4Credentials::<unnamed>%3959C7D8026F
      //## Role: M4Credentials::m_oRoleInfoList%3959C7D900CD
      //## begin M4Credentials::m_oRoleInfoList%3959C7D900CD.role preserve=no  protected: M4Credentials::TRoleInfoList { -> 1VHAN}
      TRoleInfoList m_oRoleInfoList;
      //## end M4Credentials::m_oRoleInfoList%3959C7D900CD.role

    // Additional Protected Declarations
      //## begin M4Credentials%3959C68E0328.protected preserve=yes
      //## end M4Credentials%3959C68E0328.protected

  private:
    //## Constructors (specified)
      //## Operation: M4Credentials%962187518
      M4Credentials ();

      //## Operation: M4Credentials%962187519
      M4Credentials (const m4string_t &ai_strLogin, const TRoleInfoList &ai_oRoleList, const m4string_t &ai_strSessionID);

    // Data Members for Class Attributes

      //## Attribute: m_lSessId%3ABB02DB0006
      //## begin M4Credentials::m_lSessId%3ABB02DB0006.attr preserve=no  private: m4uint64_t {UA} 
      m4uint64_t m_lSessId;
      //## end M4Credentials::m_lSessId%3ABB02DB0006.attr

    // Additional Private Declarations
      //## begin M4Credentials%3959C68E0328.private preserve=yes
      //## end M4Credentials%3959C68E0328.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4Credentials%3959C68E0328.implementation preserve=yes
      //## end M4Credentials%3959C68E0328.implementation

  //## begin M4Credentials%3959C68E0328.friends preserve=no
    friend class ClCredentialsFactory;
  //## end M4Credentials%3959C68E0328.friends
};

//## begin M4Credentials%3959C68E0328.postscript preserve=yes
//## end M4Credentials%3959C68E0328.postscript

// Class ClRoleInfo 

// Class M4Credentials 


//## Other Operations (inline)
inline M4Credentials::TRoleInfoListIterator M4Credentials::GetRoleListBegin () const
{
  //## begin M4Credentials::GetRoleListBegin%962187525.body preserve=yes
	return m_oRoleInfoList.begin();
  //## end M4Credentials::GetRoleListBegin%962187525.body
}

inline M4Credentials::TRoleInfoListIterator M4Credentials::GetRoleListEnd () const
{
  //## begin M4Credentials::GetRoleListEnd%962187526.body preserve=yes
	return m_oRoleInfoList.end();
  //## end M4Credentials::GetRoleListEnd%962187526.body
}

//## begin module%3959D0D102B8.epilog preserve=yes
//## end module%3959D0D102B8.epilog


#endif
