//## begin module%37DE02D3034A.cm preserve=no
//## end module%37DE02D3034A.cm

//## begin module%37DE02D3034A.cp preserve=no
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
//## end module%37DE02D3034A.cp

//## Module: Generic_M4XML%37DE02D3034A; Package specification
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Source file: D:\Work\v400\m4exeforchannel\inc\exeform4xml.hpp

#ifndef generic_m4xml_h
#define generic_m4xml_h 1

//## begin module%37DE02D3034A.additionalIncludes preserve=no
//## end module%37DE02D3034A.additionalIncludes

//## begin module%37DE02D3034A.includes preserve=yes
#define M4_EXEFORM4XML_DATACACHE		"DATA_CACHE"
#define M4XML_MAX_INFO				    128

#include "m4exeforchannel_dll.hpp"
#include <executiveforoe.hpp>
#include <m4xmlutil.hpp>

class  ClSSInterfaz;
class ClConfiguration;
class ClM4XMLExecInfo;
class ClExecutorStateManager;


//## begin module%37DE02D3034A.declarations preserve=no
//## end module%37DE02D3034A.declarations

//## begin module%37DE02D3034A.additionalDeclarations preserve=yes
//## end module%37DE02D3034A.additionalDeclarations


//## begin ClExecutiveForM4XML%37DDFDE60166.preface preserve=yes
//## end ClExecutiveForM4XML%37DDFDE60166.preface

//## Class: ClExecutiveForM4XML%37DDFDE60166
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37DDFE6E031A; { -> }
//## Uses: <unnamed>%3855261502D2;ClM4XMLExecInfo { -> F}
//## Uses: <unnamed>%392AB3A201DD;ClMutBlock { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForM4XML : public ClExecutiveForOE  //## Inherits: <unnamed>%37DDFE1303B9
{
  //## begin ClExecutiveForM4XML%37DDFDE60166.initialDeclarations preserve=yes
  //## end ClExecutiveForM4XML%37DDFDE60166.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForM4XML%937296809; C++
      ClExecutiveForM4XML (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForM4XML%937296810
      ~ClExecutiveForM4XML ();


    //## Other Operations (specified)
      //## Operation: StartUp%937296811
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%937296812
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: ShutDown%937296813
      m4return_t ShutDown ();

      //## Operation: ExecuteQuit%937296816
      m4return_t ExecuteQuit ();

      //## Operation: ExecuteCommand%937296819
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: SetExecInfo%945092093
      m4return_t SetExecInfo (m4pcchar_t ai_pcInfo);

    // Additional Public Declarations
      //## begin ClExecutiveForM4XML%37DDFDE60166.public preserve=yes
      //## end ClExecutiveForM4XML%37DDFDE60166.public

  protected:
    // Additional Protected Declarations
      //## begin ClExecutiveForM4XML%37DDFDE60166.protected preserve=yes
      //## end ClExecutiveForM4XML%37DDFDE60166.protected

  private:

    //## Other Operations (specified)
      //## Operation: SetSessionInfo%937296815
      m4return_t SetSessionInfo ();

    // Data Members for Class Attributes

      //## Attribute: m_poParser%37DE2AF200E6
      //## begin ClExecutiveForM4XML::m_poParser%37DE2AF200E6.attr preserve=no  private: void * {UA} 
      void *m_poParser;
      //## end ClExecutiveForM4XML::m_poParser%37DE2AF200E6.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%384664690352
      //## Role: ClExecutiveForM4XML::m_poStateManager%3846646A01E1
      //## begin ClExecutiveForM4XML::m_poStateManager%3846646A01E1.role preserve=no  private: ClExecutorStateManager {n -> 1RFHAN}
      ClExecutorStateManager *m_poStateManager;
      //## end ClExecutiveForM4XML::m_poStateManager%3846646A01E1.role

      //## Association: M4ExeForChannel::XML::<unnamed>%385515B90212
      //## Role: ClExecutiveForM4XML::m_poConfig%385515BA015F
      //## begin ClExecutiveForM4XML::m_poConfig%385515BA015F.role preserve=no  private: ClConfiguration {1 -> 1RFHAN}
      ClConfiguration *m_poConfig;
      //## end ClExecutiveForM4XML::m_poConfig%385515BA015F.role

      //## Association: M4ExeForChannel::XML::<unnamed>%39339CFF00AB
      //## Role: ClExecutiveForM4XML::m_poM4XMLSSInterface%39339D000020
      //## begin ClExecutiveForM4XML::m_poM4XMLSSInterface%39339D000020.role preserve=no  private: ClSSInterfaz {n -> 1RFHAN}
      ClSSInterfaz *m_poM4XMLSSInterface;
      //## end ClExecutiveForM4XML::m_poM4XMLSSInterface%39339D000020.role

      //## Association: M4ExeForChannel::XML::<unnamed>%39E33EAE03D8
      //## Role: ClExecutiveForM4XML::m_oCMInitValues%39E33EAF020D
      //## begin ClExecutiveForM4XML::m_oCMInitValues%39E33EAF020D.role preserve=no  private: ClCMInitParameters { -> VHAN}
      ClCMInitParameters m_oCMInitValues;
      //## end ClExecutiveForM4XML::m_oCMInitValues%39E33EAF020D.role

    // Additional Private Declarations
      //## begin ClExecutiveForM4XML%37DDFDE60166.private preserve=yes
      //## end ClExecutiveForM4XML%37DDFDE60166.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForM4XML%37DDFDE60166.implementation preserve=yes
      //## end ClExecutiveForM4XML%37DDFDE60166.implementation

};

//## begin ClExecutiveForM4XML%37DDFDE60166.postscript preserve=yes
//## end ClExecutiveForM4XML%37DDFDE60166.postscript

//## begin ClM4XMLExecInfo%38551BFF0320.preface preserve=yes
//## end ClM4XMLExecInfo%38551BFF0320.preface

//## Class: ClM4XMLExecInfo%38551BFF0320
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4XMLExecInfo : public ClM4XMLInfo  //## Inherits: <unnamed>%38551C0F03D7
{
  //## begin ClM4XMLExecInfo%38551BFF0320.initialDeclarations preserve=yes
  //## end ClM4XMLExecInfo%38551BFF0320.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLExecInfo%945092096
      ClM4XMLExecInfo (ClExecutiveForM4XML* ai_poExecutor, ClImage *aio_pImage);

    //## Destructor (specified)
      //## Operation: ~ClM4XMLExecInfo%945092099
      virtual ~ClM4XMLExecInfo ();


    //## Other Operations (specified)
      //## Operation: SetInfo%945092097
      virtual m4return_t SetInfo (m4pcchar_t ai_pcInfo);

      //## Operation: SetDebugInfo%971105389
      virtual m4return_t SetDebugInfo (m4pcchar_t ai_pcInfo);

      //## Operation: SetFileInfo%971105391
      virtual m4return_t SetFileInfo (m4pcchar_t ai_pcInfo);

      //## Operation: SetFileName%971105390
      static m4return_t SetFileName (m4pcchar_t ai_pcFileName);

    // Additional Public Declarations
      //## begin ClM4XMLExecInfo%38551BFF0320.public preserve=yes
      //## end ClM4XMLExecInfo%38551BFF0320.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4XMLExecInfo%38551BFF0320.protected preserve=yes
      //## end ClM4XMLExecInfo%38551BFF0320.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sBenchPK%39E340C700D7
      //## begin ClM4XMLExecInfo::m_sBenchPK%39E340C700D7.attr preserve=no  private: string {UA} "OFF"
      string m_sBenchPK;
      //## end ClM4XMLExecInfo::m_sBenchPK%39E340C700D7.attr

      //## Attribute: m_iLocalPK%39E340C70127
      //## begin ClM4XMLExecInfo::m_iLocalPK%39E340C70127.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iLocalPK;
      //## end ClM4XMLExecInfo::m_iLocalPK%39E340C70127.attr

      //## Attribute: m_iInternalCounter%39E340C70177
      //## begin ClM4XMLExecInfo::m_iInternalCounter%39E340C70177.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iInternalCounter;
      //## end ClM4XMLExecInfo::m_iInternalCounter%39E340C70177.attr

      //## Attribute: m_poImage%39E340C701D2
      //## begin ClM4XMLExecInfo::m_poImage%39E340C701D2.attr preserve=no  private: ClImage * {UA} aio_pImage
      ClImage *m_poImage;
      //## end ClM4XMLExecInfo::m_poImage%39E340C701D2.attr

      //## Attribute: m_oOutFile%39E340C7022C
      //## begin ClM4XMLExecInfo::m_oOutFile%39E340C7022C.attr preserve=no  private: static ofstream {UA} 
      static ofstream m_oOutFile;
      //## end ClM4XMLExecInfo::m_oOutFile%39E340C7022C.attr

      //## Attribute: m_oOutMut%39E340C70286
      //## begin ClM4XMLExecInfo::m_oOutMut%39E340C70286.attr preserve=no  private: static ClMutex {UA} 
      static ClMutex m_oOutMut;
      //## end ClM4XMLExecInfo::m_oOutMut%39E340C70286.attr

      //## Attribute: m_iCounter%39E340C702EA
      //## begin ClM4XMLExecInfo::m_iCounter%39E340C702EA.attr preserve=no  private: static m4uint32_t {UA} 0
      static m4uint32_t m_iCounter;
      //## end ClM4XMLExecInfo::m_iCounter%39E340C702EA.attr

      //## Attribute: m_iTimeOff%39E340C70344
      //## begin ClM4XMLExecInfo::m_iTimeOff%39E340C70344.attr preserve=no  private: static m4millisec_t {UA} 0
      static m4millisec_t m_iTimeOff;
      //## end ClM4XMLExecInfo::m_iTimeOff%39E340C70344.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%38551C7D01B8
      //## Role: ClM4XMLExecInfo::m_poExecutor%38551C7D0317
      //## begin ClM4XMLExecInfo::m_poExecutor%38551C7D0317.role preserve=no  private: ClExecutiveForM4XML {1 -> 1RFHAN}
      ClExecutiveForM4XML *m_poExecutor;
      //## end ClM4XMLExecInfo::m_poExecutor%38551C7D0317.role

    // Additional Private Declarations
      //## begin ClM4XMLExecInfo%38551BFF0320.private preserve=yes
      //## end ClM4XMLExecInfo%38551BFF0320.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLExecInfo%38551BFF0320.implementation preserve=yes
      //## end ClM4XMLExecInfo%38551BFF0320.implementation

};

//## begin ClM4XMLExecInfo%38551BFF0320.postscript preserve=yes
//## end ClM4XMLExecInfo%38551BFF0320.postscript

// Class ClExecutiveForM4XML 

// Class ClM4XMLExecInfo 


//## Other Operations (inline)
inline m4return_t ClM4XMLExecInfo::SetInfo (m4pcchar_t ai_pcInfo)
{
  //## begin ClM4XMLExecInfo::SetInfo%945092097.body preserve=yes
	if (m_poExecutor == NULL)
		return M4_SUCCESS;
	return m_poExecutor->SetExecInfo(ai_pcInfo);
  //## end ClM4XMLExecInfo::SetInfo%945092097.body
}

inline m4return_t ClM4XMLExecInfo::SetFileInfo (m4pcchar_t ai_pcInfo)
{
  //## begin ClM4XMLExecInfo::SetFileInfo%971105391.body preserve=yes
	if (m_poImage == NULL || IsInit() == M4_FALSE || m_oOutFile.rdbuf()->is_open() == 0)
		return M4_SUCCESS;

	ClMutBlock oBlock(&m_oOutMut);

	m_oOutFile
		<< m_sBenchPK << "\t"
		<< m_iLocalPK << "\t"
		<< m_iInternalCounter++ << "\t"
		<< dec;
	m_oOutFile << (m4double_t)(ClPrecisionTicker::GetTimeTick() - m_iTimeOff) << ai_pcInfo << endl;
	
	return M4_SUCCESS;
  //## end ClM4XMLExecInfo::SetFileInfo%971105391.body
}

//## begin module%37DE02D3034A.epilog preserve=yes
//## end module%37DE02D3034A.epilog


#endif
