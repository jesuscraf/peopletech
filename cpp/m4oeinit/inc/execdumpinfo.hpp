//## begin module%38F48FC30263.cm preserve=no
//## end module%38F48FC30263.cm

//## begin module%38F48FC30263.cp preserve=no
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
//## end module%38F48FC30263.cp

//## Module: ExecDumpInfo%38F48FC30263; Subprogram specification
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Source file: D:\Work\v500\m4oeinit\inc\execdumpinfo.hpp

#ifndef execdumpinfo_h
#define execdumpinfo_h 1

//## begin module%38F48FC30263.additionalIncludes preserve=no
//## end module%38F48FC30263.additionalIncludes

//## begin module%38F48FC30263.includes preserve=yes
#include "m4oeinit_dll.hpp"
#include "m4stl.hpp"
//## end module%38F48FC30263.includes

// m4string
#include <m4string.hpp>

class M4_DECL_M4OEINIT ClProxySpace;
class ClCMObjectDirectory;
class ClChannelManager;
class IExecutor;

//## begin module%38F48FC30263.declarations preserve=no
//## end module%38F48FC30263.declarations

//## begin module%38F48FC30263.additionalDeclarations preserve=yes
//## end module%38F48FC30263.additionalDeclarations


//## Class: ExecDumpInfo%38F484AE01CD
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OEINIT ExecDumpInfo : public ostrstream  //## Inherits: <unnamed>%38F4BAE503C9
{
  public:
    //## Constructors (specified)
      //## Operation: ExecDumpInfo%955537151
      ExecDumpInfo ()
        //## begin ExecDumpInfo::ExecDumpInfo%955537151.hasinit preserve=no
            : m_uiDumpStage(0), m_uiRequestId(0), m_pExecutor(NULL), m_pChannelManager(NULL), m_pObjectDirectory(NULL), m_pProxySpace(NULL)
        //## end ExecDumpInfo::ExecDumpInfo%955537151.hasinit
        //## begin ExecDumpInfo::ExecDumpInfo%955537151.initialization preserve=yes
        //## end ExecDumpInfo::ExecDumpInfo%955537151.initialization
      {
        //## begin ExecDumpInfo::ExecDumpInfo%955537151.body preserve=yes
        //## end ExecDumpInfo::ExecDumpInfo%955537151.body
      }

    //## Destructor (specified)
      //## Operation: ~ExecDumpInfo%955537152
      ~ExecDumpInfo ()
      {
        //## begin ExecDumpInfo::~ExecDumpInfo%955537152.body preserve=yes
		  clear();
        //## end ExecDumpInfo::~ExecDumpInfo%955537152.body
      }


    //## Other Operations (specified)
      //## Operation: SetCurrentStage%955537153
      void SetCurrentStage (const m4string_t &ai_strDesc, m4int16_t ai_iLevel = -1)
      {
        //## begin ExecDumpInfo::SetCurrentStage%955537153.body preserve=yes
		SetCurrentStage(ai_iLevel);

		m_strStageDesc = ai_strDesc;
        //## end ExecDumpInfo::SetCurrentStage%955537153.body
      }

      //## Operation: SetCurrentStage%955537154
      void SetCurrentStage (m4int16_t ai_iLevel = -1)
      {
        //## begin ExecDumpInfo::SetCurrentStage%955537154.body preserve=yes
		if (0 > ai_iLevel)
			m_uiDumpStage++;
		else
			m_uiDumpStage = ai_iLevel;

		if (0 == ai_iLevel)
			clear();
		else
			m_strStageDesc.erase();
        //## end ExecDumpInfo::SetCurrentStage%955537154.body
      }

      //## Operation: GetCurrentStage%955537158
      void GetCurrentStage (m4string_t &ai_strStage)
      {
        //## begin ExecDumpInfo::GetCurrentStage%955537158.body preserve=yes
		ai_strStage = m_strStageDesc;
        //## end ExecDumpInfo::GetCurrentStage%955537158.body
      }

      //## Operation: clear%955537160; C++
      void clear ()
      {
        //## begin ExecDumpInfo::clear%955537160.body preserve=yes
		m_pObjectDirectory = NULL;
		m_pExecutor = NULL;
		m_pChannelManager = NULL;
		m_pProxySpace = NULL;
		m_uiDumpStage = 0;
		m_uiRequestId = 0;
		m_strStageDesc.erase();
		ostrstream::clear();
        //## end ExecDumpInfo::clear%955537160.body
      }

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%38F5D4FC00FD
      //## Role: ExecDumpInfo::m_pExecutor%38F5D4FC0338
      //## begin ExecDumpInfo::m_pExecutor%38F5D4FC0338.role preserve=no  public: IExecutor { -> 1RFHAN}
      IExecutor *m_pExecutor;
      //## end ExecDumpInfo::m_pExecutor%38F5D4FC0338.role

      //## Association: M4ExeForChannel::<unnamed>%38F5D56003DC
      //## Role: ExecDumpInfo::m_pChannelManager%38F5D56102E3
      //## begin ExecDumpInfo::m_pChannelManager%38F5D56102E3.role preserve=no  public: ClChannelManager { -> 1RFHAN}
      ClChannelManager *m_pChannelManager;
      //## end ExecDumpInfo::m_pChannelManager%38F5D56102E3.role

      //## Association: M4OeInit::<unnamed>%38CE1A6B0173
      //## Role: ExecDumpInfo::m_pObjectDirectory%38CE1A6B03C2
      //## begin ExecDumpInfo::m_pObjectDirectory%38CE1A6B03C2.role preserve=no  public: ClCMObjectDirectory { -> 1RFHAN}
      ClCMObjectDirectory *m_pObjectDirectory;
      //## end ExecDumpInfo::m_pObjectDirectory%38CE1A6B03C2.role

      //## Association: M4Common_Channel::<unnamed>%3B42FD750196
      //## Role: ExecDumpInfo::m_pProxySpace%3B42FD760024
      //## begin ExecDumpInfo::m_pProxySpace%3B42FD760024.role preserve=no  public: ClProxySpace { -> 1RFHAN}
      ClProxySpace *m_pProxySpace;
      //## end ExecDumpInfo::m_pProxySpace%3B42FD760024.role

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiDumpStage%38F487C101B1
      //## begin ExecDumpInfo::m_uiDumpStage%38F487C101B1.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_uiDumpStage;
      //## end ExecDumpInfo::m_uiDumpStage%38F487C101B1.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%38CE117203A1
      //## Role: ExecDumpInfo::m_strStageDesc%38CE117301F4
      //## begin ExecDumpInfo::m_strStageDesc%38CE117301F4.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strStageDesc;
      //## end ExecDumpInfo::m_strStageDesc%38CE117301F4.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_uiRequestId%3B4358B302BF
      //## begin ExecDumpInfo::m_uiRequestId%3B4358B302BF.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiRequestId;
      //## end ExecDumpInfo::m_uiRequestId%3B4358B302BF.attr

  private: //## implementation
};

// Class ExecDumpInfo 









//## begin module%38F48FC30263.epilog preserve=yes
//## end module%38F48FC30263.epilog


#endif
