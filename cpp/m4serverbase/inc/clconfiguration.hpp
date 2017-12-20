//## begin module%3493E6C1020D.cm preserve=no
//## end module%3493E6C1020D.cm

//## begin module%3493E6C1020D.cp preserve=no
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
//## end module%3493E6C1020D.cp

//## Module: ClConfiguration%3493E6C1020D; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\clconfiguration.hpp

#ifndef clconfiguration_h
#define clconfiguration_h 1

//## begin module%3493E6C1020D.additionalIncludes preserve=no
//## end module%3493E6C1020D.additionalIncludes

//## begin module%3493E6C1020D.includes preserve=yes
#include <m4win.hpp>
#include <m4types.hpp>
//## end module%3493E6C1020D.includes

//## begin module%3493E6C1020D.declarations preserve=no
//## end module%3493E6C1020D.declarations

//## begin module%3493E6C1020D.additionalDeclarations preserve=yes
const m4size_t MAX_CONFIG_PATH_LEN = 1024 ;
#define PROP_EXECUTION_DESCRIPTION "EXECUTION_DESCRIPTION"
#define PROP_OBJECT_LIST "OBJECT_LIST"
//## end module%3493E6C1020D.additionalDeclarations


//## begin ClConfiguration%348D876F01D6.preface preserve=yes
//## end ClConfiguration%348D876F01D6.preface

//## Class: ClConfiguration%348D876F01D6; Abstract
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConfiguration 
{
  //## begin ClConfiguration%348D876F01D6.initialDeclarations preserve=yes
  //## end ClConfiguration%348D876F01D6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConfiguration%881747989
      ClConfiguration ();

    //## Destructor (specified)
      //## Operation: ~ClConfiguration%881747990
      virtual ~ClConfiguration ();


    //## Other Operations (specified)
      //## Operation: GetNumericValue%881747996; C++
      virtual int GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber) = 0;

      //## Operation: GetNumericValue%881747997; C++
      virtual int GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber) = 0;

      //## Operation: GetStringValue%881747998; C++
      virtual int GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength) = 0;

      //## Operation: GetStringValue%881747999; C++
      virtual int GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength) = 0;

      //## Operation: GetDateValue%881748000; C++
      virtual int GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate) = 0;

      //## Operation: GetDateValue%881748001; C++
      virtual int GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate) = 0;

      //## Operation: SetNumericValue%881748002; C++
      virtual int SetNumericValue (char* a_pszPath, char* a_pszName, double a_dNumber) = 0;

      //## Operation: SetNumericValue%881748003; C++
      virtual int SetNumericValue (UINT ai_hBlockHandle, char* a_pszName, double a_dNumber) = 0;

      //## Operation: SetStringValue%881748004; C++
      virtual int SetStringValue (char* a_pszPath, char* a_pszName, char* a_pszString) = 0;

      //## Operation: SetStringValue%881748005; C++
      virtual int SetStringValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszString) = 0;

      //## Operation: SetDateValue%881748006; C++
      virtual int SetDateValue (char* a_pszPath, char* a_pszName, char* a_pszDate) = 0;

      //## Operation: SetDateValue%881748007; C++
      virtual int SetDateValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate) = 0;

      //## Operation: AddNumericProperty%881748008; C++
      virtual int AddNumericProperty (char* a_pszPath, char* a_pszName, double a_dNumber) = 0;

      //## Operation: AddNumericProperty%881748009; C++
      virtual int AddNumericProperty (UINT a_hBlockHandle, char* a_pszName, double a_dNumber) = 0;

      //## Operation: AddStringProperty%881748010; C++
      virtual int AddStringProperty (char* a_pszPath, char* a_pszName, char* a_pszString) = 0;

      //## Operation: AddStringProperty%881748011; C++
      virtual int AddStringProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszString) = 0;

      //## Operation: AddDateProperty%881748012; C++
      virtual int AddDateProperty (char* ai_pszPath, char* ai_pszName, char* ai_pszDate) = 0;

      //## Operation: AddDateProperty%881748013; C++
      virtual int AddDateProperty (UINT ai_hBlockHandle, char* ai_pszName, char* ai_pszDate) = 0;

      //## Operation: AddBlock%881748014; C++
      virtual int AddBlock (char* ai_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle) = 0;

      //## Operation: WriteOut%881748015; C++
      virtual int WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut) = 0;

      //## Operation: GetAllMatchingBlocks%882350089; C++
      virtual int GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength) = 0;

      //## Operation: GetPathBlq%882350091; C++
      virtual int GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq) = 0;

      //## Operation: GetBloqConfiguration%884203092
      virtual ClConfiguration * GetBloqConfiguration (m4uint32_t ai_hBlq) = 0;

      //## Operation: GetType%884255524
      virtual m4return_t GetType (m4pchar_t ao_szType, m4uint32_t ai_iLenBuffer) = 0;

      //## Operation: GetHandle%913910715
      virtual int GetHandle () = 0;

      //## Operation: Init%916218710
      virtual int Init (const char *ai_szFile, size_t ai_iReadSize, size_t ai_iWriteSize) = 0;

      //## Operation: GetFirstProp%917598966
      virtual m4return_t GetFirstProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &ao_iHndProp, void * & ao_iHndMatch) = 0;

      //## Operation: GetNextProp%917598969
      virtual m4return_t GetNextProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &aio_iHndProp, void * & aio_iHndMatch) = 0;

      //## Operation: GetInstance%942315720; C++
      virtual int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput) = 0;

      //## Operation: GetClass%959065912; C++
      virtual int GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput) = 0;

    // Data Members for Class Attributes

      //## Attribute: m_hBlock%348E6199016A
      //## begin ClConfiguration::m_hBlock%348E6199016A.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_hBlock;
      //## end ClConfiguration::m_hBlock%348E6199016A.attr

    // Additional Public Declarations
      //## begin ClConfiguration%348D876F01D6.public preserve=yes
      //## end ClConfiguration%348D876F01D6.public

  protected:
    // Additional Protected Declarations
      //## begin ClConfiguration%348D876F01D6.protected preserve=yes
      //## end ClConfiguration%348D876F01D6.protected

  private:
    // Additional Private Declarations
      //## begin ClConfiguration%348D876F01D6.private preserve=yes
      //## end ClConfiguration%348D876F01D6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConfiguration%348D876F01D6.implementation preserve=yes
      //## end ClConfiguration%348D876F01D6.implementation

};

//## begin ClConfiguration%348D876F01D6.postscript preserve=yes
//## end ClConfiguration%348D876F01D6.postscript

// Class ClConfiguration 

inline ClConfiguration::ClConfiguration ()
  //## begin ClConfiguration::ClConfiguration%881747989.hasinit preserve=no
  //## end ClConfiguration::ClConfiguration%881747989.hasinit
  //## begin ClConfiguration::ClConfiguration%881747989.initialization preserve=yes
  //## end ClConfiguration::ClConfiguration%881747989.initialization
{
  //## begin ClConfiguration::ClConfiguration%881747989.body preserve=yes
	m_hBlock = 0 ;
  //## end ClConfiguration::ClConfiguration%881747989.body
}


inline ClConfiguration::~ClConfiguration ()
{
  //## begin ClConfiguration::~ClConfiguration%881747990.body preserve=yes
  //## end ClConfiguration::~ClConfiguration%881747990.body
}


//## begin module%3493E6C1020D.epilog preserve=yes
//## end module%3493E6C1020D.epilog


#endif
