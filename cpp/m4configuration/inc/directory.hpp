//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Directory
//	 File:				Directory.hpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971003
//	 Language:			C++
//	 Operating System:	WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module.cp

//## Module: Directory; Package specification
//## Subsystem: CPM4Configuration
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4Configuration\version\inc\directory.hpp

#ifndef directory_h
#define directory_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4configuration_dll.hpp>
#include <syncro.hpp>
#include <m4cppobl.hpp>
#include <m4syncronization.hpp>

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClExObl
//	This class provides an exclusive-accesscontainer for
//	tagged values in OBL format.
//## Category: M4Configuration
//## Subsystem: CPM4Configuration
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CONFIGURATION ClExObl 
{
  //## begin ClExObl.initialDeclarations preserve=yes
  //## end ClExObl.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExObl%875521753; C++
      ClExObl (void );

      //## Operation: ClExObl%875521754; C++
      ClExObl (UINT a_uiHandle);

    //## Destructor (specified)
      //## Operation: ~ClExObl%875521755; C++
      ~ClExObl (void );


    //## Other Operations (specified)
      //## Operation: Init%875521756; C++
      int Init (UINT a_uiHandle, int a_iAllocated = 0 );

      //## Operation: Init%875521757; C++
      int Init (M4ClObl& a_rclObl);

      //## Operation: Init%875521758; C++
      int Init (char** a_ppszText, int a_iLength);

      //## Operation: Init%875521759; C++
      int Init (const char* ca_pszFile, size_t a_szReadSize, size_t a_szWriteSize);

      //## Operation: GetHandle%875521790; C++
      int GetHandle (void ) const;

      //## Operation: RetireHandle%875521791; C++
      int RetireHandle (void );

      //## Operation: GetPrecision%875521792; C++
      int GetPrecision (void ) const;

      //## Operation: SetPrecision%875521793; C++
      void SetPrecision (int a_iPrecision);

      //## Operation: GetClass%875521760; C++
      int GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: GetInstance%875521761; C++
      int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: GetNumericValue%875521762; C++
      int GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetNumericValue%875521763; C++
      int GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetStringValue%875521764; C++
      int GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetStringValue%875521765; C++
      int GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetDateValue%875521766; C++
      int GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate);

      //## Operation: GetDateValue%875521767; C++
      int GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate);

      //## Operation: SetNumericValue%875521770; C++
      int SetNumericValue (char* a_pszPath, char* a_pszName, double a_dNumber);

      //## Operation: SetNumericValue%875521771; C++
      int SetNumericValue (UINT ai_hBlockHandle, char* a_pszName, double a_dNumber);

      //## Operation: SetStringValue%875521772; C++
      int SetStringValue (char* a_pszPath, char* a_pszName, char* a_pszString);

      //## Operation: SetStringValue%875521773; C++
      int SetStringValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszString);

      //## Operation: SetDateValue%875521774; C++
      int SetDateValue (char* a_pszPath, char* a_pszName, char* a_pszDate);

      //## Operation: SetDateValue%875521775; C++
      int SetDateValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate);

      //## Operation: GetHandleToBlock%875521776; C++
      int GetHandleToBlock (char* a_pszPath, UINT& ao_rhHandle);

      //## Operation: GetBlockInstance%875521777; C++
      int GetBlockInstance (UINT ai_hBlockHandle, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: AddNumericProperty%875521779; C++
      int AddNumericProperty (char* a_pszPath, char* a_pszName, double a_dNumber);

      //## Operation: AddNumericProperty%875521780; C++
      int AddNumericProperty (UINT a_hBlockHandle, char* a_pszName, double a_dNumber);

      //## Operation: AddStringProperty%875521781; C++
      int AddStringProperty (char* a_pszPath, char* a_pszName, char* a_pszString);

      //## Operation: AddStringProperty%875521782; C++
      int AddStringProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszString);

      //## Operation: AddDateProperty%875521783; C++
      int AddDateProperty (char* a_pszPath, char* a_pszName, char* a_pszDate);

      //## Operation: AddDateProperty%875521784; C++
      int AddDateProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszDate);

      //## Operation: AddBlock%875521785; C++
      int AddBlock (char* a_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle);

      //## Operation: GetAllMatchingBlocks%875521786; C++
      int GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength);

      //## Operation: WriteOut%875521787; C++
      int WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut);

      //## Operation: GetPathBlq%875521788; C++
      int GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq);

      //## Operation: operator =%875521789; C++
      //	int WriteOut(char* ao_szTextOut, UINT ai_iLenBuf, UINT &
      //	ao_iLenOut);
      void operator = (M4ClObl& a_rclObl);

    // Data Members for Associations

      //## Association: M4ComSrv::Subsystem::<unnamed>%34CF59A4002B
      //## Role: ClExObl::<m_pClTables>
      //## begin ClExObl::<m_pClTables>.role preserve=no  public: ClTables { -> RHGA}
      //## end ClExObl::<m_pClTables>.role

      //## Association: M4ComSrv::Subsystem::<unnamed>%34D6009F0109
      //## Role: ClExObl::<m_pClTables>
      //## begin ClExObl::<m_pClTables>.role preserve=no  public: static ClTables { -> RHGA}
      //## end ClExObl::<m_pClTables>.role

    // Additional Public Declarations
      //## begin ClExObl.public preserve=yes
      //## end ClExObl.public

  protected:
    // Data Members for Associations

      //## Association: M4Configuration::<unnamed>%35F3FB90038B
      //## Role: ClExObl::m_oM4Obl
      //## begin ClExObl::m_oM4Obl.role preserve=no  protected: M4ClObl { -> UHAN}
      M4ClObl m_oM4Obl;
      //## end ClExObl::m_oM4Obl.role

      //## Association: M4Configuration::<unnamed>%35F3FD9B038D
      //## Role: ClExObl::m_oFilter
      //## begin ClExObl::m_oFilter.role preserve=no  protected: ClWriterReaders { -> UHAN}
      ClMutex m_oFilter;
      //## end ClExObl::m_oFilter.role

    // Additional Protected Declarations
      //## begin ClExObl.protected preserve=yes
      //## end ClExObl.protected

  private:
    // Additional Private Declarations
      //## begin ClExObl.private preserve=yes
      //## end ClExObl.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClExObl.implementation preserve=yes
      //## end ClExObl.implementation

};

//## begin ClExObl.postscript preserve=yes
//## end ClExObl.postscript



//## Class: ClDirectory
//## Category: M4Configuration
//## Subsystem: CPM4Configuration
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClExObl { -> }

class M4_DECL_M4CONFIGURATION ClDirectory 
{
  //## begin ClDirectory.initialDeclarations preserve=yes
  //## end ClDirectory.initialDeclarations

  public:
    //## Class: e_DirElements
    //## Category: <Top Level>
    //## Subsystem: <Top Level>
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {Host_ID, User_ID, Service_ID} e_DirElements;



  public:
    //## Constructors (specified)
      //## Operation: ClDirectory%877020690
      ClDirectory ();

    //## Destructor (specified)
      //## Operation: ~ClDirectory%877020691
      ~ClDirectory ();


    //## Other Operations (specified)
      //## Operation: FindHost%877020692; C++
      //	Returns True if the element is in the repository, False
      //	otherwise.
      m4bool_t FindHost (m4uint16_t ai_iHostId);

      //## Operation: FindUser%877020693; C++
      //	Returns True if the element is in the repository, False
      //	otherwise.
      m4bool_t FindUser (m4uint16_t ai_iHostId);

      //## Operation: FindUser%880396146; C++
      //	Returns True if the element is in the repository, False
      //	otherwise.
      m4bool_t FindUser (m4pchar_t &ai_pUserId);

      //## Operation: Find%877020694; C++
      //	Returns True if the element is in the repository, False
      //	otherwise.
      m4bool_t Find (m4uint16_t ai_iItemId, ClDirectory::e_DirElements ai_eItemType);

      //## Operation: CheckLogin%880396147; C++
      //	This operation checks if the user exists, and if so,
      //	tests whether the password provided is the one stored as
      //	login information.
      //	Parameters
      //	ai_pUser: Null-terminated string holding the username.
      //	ai_pPasswd: Null-terminated string holding the password.
      //	Return Values
      //	M4_TRUE if the login is correct, M4_FALSE otherwise.
      m4bool_t CheckLogin (m4pchar_t ai_pUser, m4pchar_t ai_pPasswd);

    // Additional Public Declarations
      //## begin ClDirectory.public preserve=yes
      //## end ClDirectory.public

  protected:
    // Additional Protected Declarations
      //## begin ClDirectory.protected preserve=yes
      //## end ClDirectory.protected

  private:
    // Additional Private Declarations
      //## begin ClDirectory.private preserve=yes
      //## end ClDirectory.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClDirectory.implementation preserve=yes
      //## end ClDirectory.implementation

};

//## begin ClDirectory.postscript preserve=yes
//## end ClDirectory.postscript



// Class ClExObl 


//## Other Operations (inline)
inline int ClExObl::GetHandle (void ) const
{
  //## begin ClExObl::GetHandle%875521790.body preserve=yes

	return m_oM4Obl.GetHandle ( ) ;
  //## end ClExObl::GetHandle%875521790.body
}

inline int ClExObl::RetireHandle (void )
{
  //## begin ClExObl::RetireHandle%875521791.body preserve=yes
	return m_oM4Obl.RetireHandle ( ) ;
  //## end ClExObl::RetireHandle%875521791.body
}

inline int ClExObl::GetPrecision (void ) const
{
  //## begin ClExObl::GetPrecision%875521792.body preserve=yes
	return m_oM4Obl.GetPrecision ( ) ;
  //## end ClExObl::GetPrecision%875521792.body
}

inline void ClExObl::SetPrecision (int a_iPrecision)
{
  //## begin ClExObl::SetPrecision%875521793.body preserve=yes
	m_oM4Obl.SetPrecision ( a_iPrecision ) ;
  //## end ClExObl::SetPrecision%875521793.body
}







// Class ClDirectory 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
