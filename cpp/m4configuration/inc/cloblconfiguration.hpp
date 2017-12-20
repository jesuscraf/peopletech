//## begin module%3493E77000F6.cm preserve=no
//## end module%3493E77000F6.cm

//## begin module%3493E77000F6.cp preserve=no
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
//## end module%3493E77000F6.cp

//## Module: ClOblConfiguration%3493E77000F6; Package specification
//## Subsystem: M4Configuration::inc%379DD22F012C
//## Source file: F:\integration\M4Configuration\inc\cloblconfiguration.hpp

#ifndef cloblconfiguration_h
#define cloblconfiguration_h 1

//## begin module%3493E77000F6.additionalIncludes preserve=no
#include <m4configuration_dll.hpp>
#include <syncro.hpp>
#include <clconfiguration.hpp>
#include <directory.hpp>

//## begin module%3493E77000F6.declarations preserve=no
//## end module%3493E77000F6.declarations

//## begin module%3493E77000F6.additionalDeclarations preserve=yes
#define		OBL_PROPERTY_MAX_LEN			100
#define		OBL_PROPVALUE_MAX_LEN			200
#define		OBL_PATHVALUE_MAX_LEN			512


//## end module%3493E77000F6.additionalDeclarations


//## begin ClOblConfiguration%348E5D8C0173.preface preserve=yes
//## end ClOblConfiguration%348E5D8C0173.preface

//## Class: ClOblConfiguration%348E5D8C0173
//## Category: M4Configuration%35F3EFFC0108
//## Subsystem: M4Configuration::inc%379DD22F012C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CONFIGURATION ClOblConfiguration : public ClConfiguration, public ClExObl  //## Inherits: <unnamed>%348E5F360130
{
  //## begin ClOblConfiguration%348E5D8C0173.initialDeclarations preserve=yes
  //## end ClOblConfiguration%348E5D8C0173.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClOblConfiguration%884203093
      ClOblConfiguration (UINT ai_hHandle);

      //## Operation: ClOblConfiguration%884203094
      ClOblConfiguration ();

      //## Operation: ClOblConfiguration%913835326
      ClOblConfiguration (m4pchar_t ai_szOblString);

    //## Destructor (specified)
      //## Operation: ~ClOblConfiguration%903009694
      ~ClOblConfiguration ();


    //## Other Operations (specified)
      //## Operation: GetHandle%882264760; C++
      int GetHandle (void );

      //## Operation: RetireHandle%882264761; C++
      int RetireHandle (void );

      //## Operation: GetPrecision%882264762; C++
      int GetPrecision (void ) const;

      //## Operation: SetPrecision%882264763; C++
      void SetPrecision (int a_iPrecision);

      //## Operation: GetInstance%882264738; C++
      int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: GetNumericValue%882264739; C++
      int GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetNumericValue%882264740; C++
      int GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetStringValue%882264741; C++
      int GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetStringValue%882264742; C++
      int GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetDateValue%882264743; C++
      int GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate);

      //## Operation: GetDateValue%882264744; C++
      int GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate);

      //## Operation: SetNumericValue%882264745; C++
      int SetNumericValue (char* a_pszPath, char* a_pszName, double a_dNumber);

      //## Operation: SetNumericValue%882264746; C++
      int SetNumericValue (UINT ai_hBlockHandle, char* a_pszName, double a_dNumber);

      //## Operation: SetStringValue%882264747; C++
      int SetStringValue (char* a_pszPath, char* a_pszName, char* a_pszString);

      //## Operation: SetStringValue%882264748; C++
      int SetStringValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszString);

      //## Operation: SetDateValue%882264749; C++
      int SetDateValue (char* a_pszPath, char* a_pszName, char* a_pszDate);

      //## Operation: SetDateValue%882264750; C++
      int SetDateValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate);

      //## Operation: AddNumericProperty%882264751; C++
      int AddNumericProperty (char* a_pszPath, char* a_pszName, double a_dNumber);

      //## Operation: AddNumericProperty%882264752; C++
      int AddNumericProperty (UINT a_hBlockHandle, char* a_pszName, double a_dNumber);

      //## Operation: AddStringProperty%882264753; C++
      int AddStringProperty (char* a_pszPath, char* a_pszName, char* a_pszString);

      //## Operation: AddStringProperty%882264754; C++
      int AddStringProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszString);

      //## Operation: AddDateProperty%882264755; C++
      int AddDateProperty (char* a_pszPath, char* a_pszName, char* a_pszDate);

      //## Operation: AddDateProperty%882264756; C++
      int AddDateProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszDate);

      //## Operation: AddBlock%882264757; C++
      int AddBlock (char* a_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle);

      //## Operation: WriteOut%882264758; C++
      int WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut);

      //## Operation: GetConfigvariables%882264759
      void GetConfigvariables ();

      //## Operation: GetAllMatchingBlocks%882350088; C++
      int GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength);

      //## Operation: GetPathBlq%882350090; C++
      int GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq);

      //## Operation: GetBloqConfiguration%884203091
      ClConfiguration * GetBloqConfiguration (m4uint32_t ai_hBlq);

      //## Operation: GetType%884255523
      m4return_t GetType (m4pchar_t ao_szType, m4uint32_t ai_iLenBuffer);

      //## Operation: Init%916218709
      int Init (const char *ai_szFile, size_t ai_iReadSize, size_t ai_iWriteSize);

      //## Operation: GetFirstProp%917598968
      m4return_t GetFirstProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &ao_iHndProp, void * & ao_iHndMatch);

      //## Operation: GetNextProp%917598971
      m4return_t GetNextProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &aio_iHndProp, void * & aio_iHndMatch);

      //## Operation: GetClass%959065913; C++
      int GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

    // Data Members for Associations

      //## Association: M4Configuration::<unnamed>%367E163F0390
      //## Role: ClOblConfiguration::m_oMutex%367E16400279
      //## begin ClOblConfiguration::m_oMutex%367E16400279.role preserve=no  public: static ClMutex { -> VHAN}
      static ClMutex m_oMutex;
      //## end ClOblConfiguration::m_oMutex%367E16400279.role

    // Additional Public Declarations
      //## begin ClOblConfiguration%348E5D8C0173.public preserve=yes
      //## end ClOblConfiguration%348E5D8C0173.public

  protected:
    // Additional Protected Declarations
      //## begin ClOblConfiguration%348E5D8C0173.protected preserve=yes
      //## end ClOblConfiguration%348E5D8C0173.protected

  private:
    // Additional Private Declarations
      //## begin ClOblConfiguration%348E5D8C0173.private preserve=yes
      //## end ClOblConfiguration%348E5D8C0173.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClOblConfiguration%348E5D8C0173.implementation preserve=yes
      //## end ClOblConfiguration%348E5D8C0173.implementation

};

//## begin ClOblConfiguration%348E5D8C0173.postscript preserve=yes
//## end ClOblConfiguration%348E5D8C0173.postscript

// Class ClOblConfiguration 


//## Other Operations (inline)
inline int ClOblConfiguration::GetHandle (void )
{
  //## begin ClOblConfiguration::GetHandle%882264760.body preserve=yes
	return ClExObl::GetHandle ( ) ;
  //## end ClOblConfiguration::GetHandle%882264760.body
}

inline int ClOblConfiguration::RetireHandle (void )
{
  //## begin ClOblConfiguration::RetireHandle%882264761.body preserve=yes
	return ClExObl::RetireHandle ( ) ;
  //## end ClOblConfiguration::RetireHandle%882264761.body
}

inline int ClOblConfiguration::GetPrecision (void ) const
{
  //## begin ClOblConfiguration::GetPrecision%882264762.body preserve=yes
	return ClExObl::GetPrecision ( ) ;
  //## end ClOblConfiguration::GetPrecision%882264762.body
}

inline void ClOblConfiguration::SetPrecision (int a_iPrecision)
{
  //## begin ClOblConfiguration::SetPrecision%882264763.body preserve=yes
	ClExObl::SetPrecision ( a_iPrecision ) ;
  //## end ClOblConfiguration::SetPrecision%882264763.body
}

//## begin module%3493E77000F6.epilog preserve=yes
//## end module%3493E77000F6.epilog


#endif
