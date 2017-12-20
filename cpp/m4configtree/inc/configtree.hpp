//## begin module%36792AA3039D.cm preserve=no
//## end module%36792AA3039D.cm

//## begin module%36792AA3039D.cp preserve=no
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
//## end module%36792AA3039D.cp

//## Module: ConfigTree%36792AA3039D; Package specification
//## Subsystem: M4ConfigTree::inc%379C728400FE
//## Source file: F:\integration\M4ConfigTree\inc\configtree.hpp

#ifndef configtree_h
#define configtree_h 1

//## begin module%36792AA3039D.additionalIncludes preserve=no
//## end module%36792AA3039D.additionalIncludes

//## begin module%36792AA3039D.includes preserve=yes
#include <m4configtree_dll.hpp>
//## end module%36792AA3039D.includes

// syncro
#include <syncro.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// properties
#include <properties.hpp>

class M4_DECL_M4CONFIGTREE ClM4ConfigTree;

//## begin module%36792AA3039D.declarations preserve=no
//## end module%36792AA3039D.declarations

//## begin module%36792AA3039D.additionalDeclarations preserve=yes
//## end module%36792AA3039D.additionalDeclarations


//## begin ClPropertyBlocks%36761FC603D9.preface preserve=yes
//## end ClPropertyBlocks%36761FC603D9.preface

//## Class: ClPropertyBlocks%36761FC603D9
//## Category: M4ConfigTree%3678E41E00B9
//## Subsystem: M4ConfigTree::inc%379C728400FE
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%367927E5022B;ClM4ConfigTree { -> F}

typedef map < m4uint32_t , ClM4ConfigTree * , less < m4uint32_t > > ClPropertyBlocks;

//## begin ClPropertyBlocks%36761FC603D9.postscript preserve=yes
//## end ClPropertyBlocks%36761FC603D9.postscript

//## begin ClM4ConfigTree%3678E58F01E6.preface preserve=yes
//## end ClM4ConfigTree%3678E58F01E6.preface

//## Class: ClM4ConfigTree%3678E58F01E6
//## Category: M4ConfigTree%3678E41E00B9
//## Subsystem: M4ConfigTree::inc%379C728400FE
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CONFIGTREE ClM4ConfigTree : public ClConfiguration  //## Inherits: <unnamed>%367927FE00E6
{
  //## begin ClM4ConfigTree%3678E58F01E6.initialDeclarations preserve=yes
  //## end ClM4ConfigTree%3678E58F01E6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4ConfigTree%917549077
      ClM4ConfigTree (ClConfiguration *ai_poConfiguration);

      //## Operation: ClM4ConfigTree%913982492
      ClM4ConfigTree (m4pchar_t ai_szProperties);

      //## Operation: ClM4ConfigTree%913982493
      ClM4ConfigTree ();


    //## Other Operations (specified)
      //## Operation: GetNumericValue%913910690; C++
      int GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetNumericValue%913910691; C++
      int GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber);

      //## Operation: GetStringValue%913910692; C++
      virtual int GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetStringValue%913910693; C++
      int GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

      //## Operation: GetDateValue%913910694; C++
      int GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate);

      //## Operation: GetDateValue%913910695; C++
      int GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate);

      //## Operation: SetNumericValue%913910696; C++
      int SetNumericValue (char* ai_pszPath, char* ai_pszName, double ai_dNumber);

      //## Operation: SetNumericValue%913910697; C++
      int SetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double ai_dNumber);

      //## Operation: SetStringValue%913910698; C++
      int SetStringValue (char* ai_pszPath, char* ai_pszName, char* ai_pszString);

      //## Operation: SetStringValue%913910699; C++
      int SetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszString);

      //## Operation: SetDateValue%913910700; C++
      int SetDateValue (char* ai_pszPath, char* ai_pszName, char* ai_pszDate);

      //## Operation: SetDateValue%913910701; C++
      int SetDateValue (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszDate);

      //## Operation: AddNumericProperty%913910702; C++
      int AddNumericProperty (char* ai_pszPath, char* ai_pszName, double ai_dNumber);

      //## Operation: AddNumericProperty%913910703; C++
      int AddNumericProperty (UINT ai_hBlockHandle, char* ai_pszName, double ai_dNumber);

      //## Operation: AddStringProperty%913910704; C++
      int AddStringProperty (char* ai_pszPath, char* ai_pszName, char* ai_pszString);

      //## Operation: AddStringProperty%913910705; C++
      int AddStringProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszString);

      //## Operation: AddDateProperty%913910706; C++
      int AddDateProperty (char* ai_pszPath, char* ai_pszName, char* ai_pszDate);

      //## Operation: AddDateProperty%913910707; C++
      int AddDateProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszDate);

      //## Operation: AddBlock%913910708; C++
      int AddBlock (char* ai_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle);

      //## Operation: WriteOut%913910709; C++
      int WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut);

      //## Operation: GetAllMatchingBlocks%913910710; C++
      int GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength);

      //## Operation: GetPathBlq%913910711; C++
      int GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq);

      //## Operation: GetBloqConfiguration%913910712
      ClConfiguration * GetBloqConfiguration (m4uint32_t ai_hBlq);

      //## Operation: GetType%913910713
      m4return_t GetType (m4pchar_t ao_szType, m4uint32_t ai_iLenBuffer);

      //## Operation: GetHandle%913910714; C++
      int GetHandle (void );

      //## Operation: Init%916218711
      int Init (const char *ai_szFile, size_t ai_iReadSize, size_t ai_iWriteSize);

      //## Operation: GetFirstProp%917598967
      m4return_t GetFirstProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &ao_iHndProp, void * & ao_iHndMatch);

      //## Operation: GetNextProp%917598970
      m4return_t GetNextProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &aio_iHndProp, void * & aio_iHndMatch);

      //## Operation: SetClassHinst%918583520
      void SetClassHinst (m4pchar_t ai_szClass, m4pchar_t ai_szHist);

      //## Operation: GetInstance%942315721; C++
      int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: GetClass%959065914; C++
      int GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

    // Data Members for Associations

      //## Association: M4ConfigTree::<unnamed>%367925F402F8
      //## Role: ClM4ConfigTree::m_oPropertyBlocks%367925F600A2
      //## begin ClM4ConfigTree::m_oPropertyBlocks%367925F600A2.role preserve=no  public: ClPropertyBlocks { -> VHAN}
      ClPropertyBlocks m_oPropertyBlocks;
      //## end ClM4ConfigTree::m_oPropertyBlocks%367925F600A2.role

      //## Association: M4ConfigTree::<unnamed>%367926360339
      //## Role: ClM4ConfigTree::m_oSynchro%367926370381
      //## begin ClM4ConfigTree::m_oSynchro%367926370381.role preserve=no  public: ClMutex { -> VHAN}
      ClMutex m_oSynchro;
      //## end ClM4ConfigTree::m_oSynchro%367926370381.role

      //## Association: M4ConfigTree::<unnamed>%367926D70268
      //## Role: ClM4ConfigTree::m_oProperties%367926D80382
      //## begin ClM4ConfigTree::m_oProperties%367926D80382.role preserve=no  public: ClProperties { -> VHAN}
      ClProperties m_oProperties;
      //## end ClM4ConfigTree::m_oProperties%367926D80382.role

    // Additional Public Declarations
      //## begin ClM4ConfigTree%3678E58F01E6.public preserve=yes
      //## end ClM4ConfigTree%3678E58F01E6.public

  protected:
    // Data Members for Associations

      //## Association: M4ConfigTree::<unnamed>%36C0866D000A
      //## Role: ClM4ConfigTree::m_szHinst%36C0866F0035
      //## begin ClM4ConfigTree::m_szHinst%36C0866F0035.role preserve=no  protected: PropertyName { -> VHAN}
      PropertyName m_szHinst;
      //## end ClM4ConfigTree::m_szHinst%36C0866F0035.role

      //## Association: M4ConfigTree::<unnamed>%36C08695038D
      //## Role: ClM4ConfigTree::m_szClass%36C0869601EA
      //## begin ClM4ConfigTree::m_szClass%36C0869601EA.role preserve=no  protected: PropertyName { -> VHAN}
      PropertyName m_szClass;
      //## end ClM4ConfigTree::m_szClass%36C0869601EA.role

    // Additional Protected Declarations
      //## begin ClM4ConfigTree%3678E58F01E6.protected preserve=yes
      //## end ClM4ConfigTree%3678E58F01E6.protected

  private:
    // Additional Private Declarations
      //## begin ClM4ConfigTree%3678E58F01E6.private preserve=yes
      //## end ClM4ConfigTree%3678E58F01E6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4ConfigTree%3678E58F01E6.implementation preserve=yes
      //## end ClM4ConfigTree%3678E58F01E6.implementation

};

//## begin ClM4ConfigTree%3678E58F01E6.postscript preserve=yes
//## end ClM4ConfigTree%3678E58F01E6.postscript

// Class ClM4ConfigTree 


//## Other Operations (inline)
inline int ClM4ConfigTree::GetHandle (void )
{
  //## begin ClM4ConfigTree::GetHandle%913910714.body preserve=yes
	return m_hBlock ;
  //## end ClM4ConfigTree::GetHandle%913910714.body
}

//## begin module%36792AA3039D.epilog preserve=yes
//## end module%36792AA3039D.epilog


#endif
