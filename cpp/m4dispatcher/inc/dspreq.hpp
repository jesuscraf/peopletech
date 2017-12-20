//## begin module%396C4E0903A6.cm preserve=no
//## end module%396C4E0903A6.cm

//## begin module%396C4E0903A6.cp preserve=no
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
//## end module%396C4E0903A6.cp

//## Module: dspreq%396C4E0903A6; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\dspreq.hpp

#ifndef dspreq_h
#define dspreq_h 1

//## begin module%396C4E0903A6.additionalIncludes preserve=no
//## end module%396C4E0903A6.additionalIncludes

//## begin module%396C4E0903A6.includes preserve=yes
#include <sitenodeinfo.hpp>
//## end module%396C4E0903A6.includes

// m4types
#include <m4types.hpp>
// logsys
#include <logsys.hpp>

class ClDspAnswerGen;

//## begin module%396C4E0903A6.declarations preserve=no
//## end module%396C4E0903A6.declarations

//## begin module%396C4E0903A6.additionalDeclarations preserve=yes
//## end module%396C4E0903A6.additionalDeclarations


//## begin ClDspRequest%396D796103E7.preface preserve=yes
//## end ClDspRequest%396D796103E7.preface

//## Class: ClDspRequest%396D796103E7
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%397310F401A9;ClDspAnswerGen { -> F}

class ClDspRequest 
{
  //## begin ClDspRequest%396D796103E7.initialDeclarations preserve=yes
  //## end ClDspRequest%396D796103E7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDspRequest%963825074
      ClDspRequest (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID, ClLogSystem *ai_poLog = NULL);

    //## Destructor (specified)
      //## Operation: ~ClDspRequest%963989739
      virtual ~ClDspRequest ();


    //## Other Operations (specified)
      //## Operation: BuildReqReply%963825075
      virtual m4return_t BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen) = 0;

      //## Operation: GetBufferOut%963825086
      void * GetBufferOut ();

      //## Operation: GetBufferIn%963906294
      void * GetBufferIn ();

      //## Operation: GetConnectionID%963906295
      m4uint32_t GetConnectionID ();

      //## Operation: GetPetitionID%963906296
      m4uint64_t GetPetitionID ();

      //## Operation: GetLog%979719939
      ClLogSystem * GetLog ();

      //## Operation: SetResult%980953961
      void SetResult (m4return_t ai_iResult);

      //## Operation: GetResult%980953962
      m4return_t GetResult ();

    // Additional Public Declarations
      //## begin ClDspRequest%396D796103E7.public preserve=yes
      //## end ClDspRequest%396D796103E7.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%396C49CF00ED
      //## Role: ClDspRequest::m_poBufferIn%396C49CF02BB
      //## begin ClDspRequest::m_poBufferIn%396C49CF02BB.role preserve=no  protected: m4void_t { -> RHAN}
      m4void_t *m_poBufferIn;
      //## end ClDspRequest::m_poBufferIn%396C49CF02BB.role

      //## Association: M4Dispatcher::<unnamed>%396C49F5026E
      //## Role: ClDspRequest::m_poBufferOut%396C49F60071
      //## begin ClDspRequest::m_poBufferOut%396C49F60071.role preserve=no  protected: m4void_t { -> RHAN}
      m4void_t *m_poBufferOut;
      //## end ClDspRequest::m_poBufferOut%396C49F60071.role

    // Additional Protected Declarations
      //## begin ClDspRequest%396D796103E7.protected preserve=yes
      //## end ClDspRequest%396D796103E7.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iConnectionID%396DA301037B
      //## begin ClDspRequest::m_iConnectionID%396DA301037B.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iConnectionID;
      //## end ClDspRequest::m_iConnectionID%396DA301037B.attr

      //## Attribute: m_iRequestID%396DA3580236
      //## begin ClDspRequest::m_iRequestID%396DA3580236.attr preserve=no  private: m4uint64_t {UA} 
      m4uint64_t m_iRequestID;
      //## end ClDspRequest::m_iRequestID%396DA3580236.attr

      //## Attribute: m_iRet%3A782B580353
      //## begin ClDspRequest::m_iRet%3A782B580353.attr preserve=no  private: m4return_t {UA} 
      m4return_t m_iRet;
      //## end ClDspRequest::m_iRet%3A782B580353.attr

      //## Attribute: m_bOwnLog%3B81277B00E5
      //## begin ClDspRequest::m_bOwnLog%3B81277B00E5.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bOwnLog;
      //## end ClDspRequest::m_bOwnLog%3B81277B00E5.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::Agents::Request::<unnamed>%3A6564CD0338
      //## Role: ClDspRequest::m_poLog%3A6564CE0163
      //## begin ClDspRequest::m_poLog%3A6564CE0163.role preserve=no  private: ClLogSystem { -> RHAN}
      ClLogSystem *m_poLog;
      //## end ClDspRequest::m_poLog%3A6564CE0163.role

    // Additional Private Declarations
      //## begin ClDspRequest%396D796103E7.private preserve=yes
      //## end ClDspRequest%396D796103E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspRequest%396D796103E7.implementation preserve=yes
      //## end ClDspRequest%396D796103E7.implementation

};

//## begin ClDspRequest%396D796103E7.postscript preserve=yes
//## end ClDspRequest%396D796103E7.postscript

// Class ClDspRequest 


//## Other Operations (inline)
inline void * ClDspRequest::GetBufferOut ()
{
  //## begin ClDspRequest::GetBufferOut%963825086.body preserve=yes
	return m_poBufferOut ;
  //## end ClDspRequest::GetBufferOut%963825086.body
}

inline void * ClDspRequest::GetBufferIn ()
{
  //## begin ClDspRequest::GetBufferIn%963906294.body preserve=yes
	return m_poBufferIn ;
  //## end ClDspRequest::GetBufferIn%963906294.body
}

inline m4uint32_t ClDspRequest::GetConnectionID ()
{
  //## begin ClDspRequest::GetConnectionID%963906295.body preserve=yes
	return m_iConnectionID ;
  //## end ClDspRequest::GetConnectionID%963906295.body
}

inline m4uint64_t ClDspRequest::GetPetitionID ()
{
  //## begin ClDspRequest::GetPetitionID%963906296.body preserve=yes
	return m_iRequestID ;
  //## end ClDspRequest::GetPetitionID%963906296.body
}

inline void ClDspRequest::SetResult (m4return_t ai_iResult)
{
  //## begin ClDspRequest::SetResult%980953961.body preserve=yes
	m_iRet = ai_iResult ;
  //## end ClDspRequest::SetResult%980953961.body
}

inline m4return_t ClDspRequest::GetResult ()
{
  //## begin ClDspRequest::GetResult%980953962.body preserve=yes
	return m_iRet ;
  //## end ClDspRequest::GetResult%980953962.body
}

//## begin module%396C4E0903A6.epilog preserve=yes
//## end module%396C4E0903A6.epilog


#endif
