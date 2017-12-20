//## begin module%34BA0062031C.cm preserve=no
//## end module%34BA0062031C.cm

//## begin module%34BA0062031C.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExecutiveChannel.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%34BA0062031C.cp

//## Module: Executive_Channel%34BA0062031C; Package specification
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Source file: D:\eduardoma\m4exeforchannel\inc\exeforchannel.hpp

#ifndef executive_channel_h
#define executive_channel_h 1

//## begin module%34BA0062031C.additionalIncludes preserve=no
//## end module%34BA0062031C.additionalIncludes

//## begin module%34BA0062031C.includes preserve=yes
#include "m4exeforchannel_dll.hpp"

#define M4_EXEFORCHANNEL_DATACACHE		"DATA_CACHE"
#define OLTP_MAX_INFO				    128
//## end module%34BA0062031C.includes

// executiveforoe
#include <executiveforoe.hpp>
//## begin module%34BA0062031C.declarations preserve=no
//## end module%34BA0062031C.declarations

//## begin module%34BA0062031C.additionalDeclarations preserve=yes

M4_DECL_M4EXEFORCHANNEL m4uint32_t     ExecCheckSum(void *ai_p, m4uint32_t size, m4uint32_t &sum_old);
M4_DECL_M4EXEFORCHANNEL m4uint32_t     ExecCheckSum(M4DataStorage *ai_poDataStorage, m4uint32_t &sum_old);
M4_DECL_M4EXEFORCHANNEL M4DataStorage* ExecNewDataStorage(M4DataStorage *ai_poDataStorage);
M4_DECL_M4EXEFORCHANNEL M4DataStorage* ExecNewDataStorage(m4puint8_t pBuffer, m4uint32_t lSize);
M4_DECL_M4EXEFORCHANNEL void           ExecCopyToBuffer(m4puint8_t aio_pBuffer, M4DataStorage *ai_poDataStorage);

// general

// inits a buffer from a channel
m4return_t M4_DECL_M4EXEFORCHANNEL ChannelInitBufferFromID(const m4char_t *ai_pcChannelName,
															   void *ao_pOutputBuffer);
m4return_t M4_DECL_M4EXEFORCHANNEL ChannelDumpBuffer(const m4char_t *ai_pcChannelName,
														 const void *ao_pOutputBuffer);
m4return_t M4_DECL_M4EXEFORCHANNEL ChannelInit(const m4bool_t a_ibInitLog=M4_FALSE);
m4return_t M4_DECL_M4EXEFORCHANNEL ChannelShutdown();

// data in particular buffer
class M4_DECL_M4EXEFORCHANNEL ClExecSerializableObjectData
{
	// public members
public:
	m4char_t *m_acChannelName;
	m4char_t *m_acNodeName;
	m4int32_t m_iRegisterN;
	m4int32_t m_iNRequest;
	m4int32_t m_iNResult;
	m4char_t *m_acItemName;
	m4char_t **m_aacOutput;
};

// specilized serialized class
class M4_DECL_M4EXEFORCHANNEL ClExecSerializableObjectTestServer : public ClExecSerializableObject
{
	// public members
public:
	ClExecSerializableObjectTestServer(iostream*);
	~ClExecSerializableObjectTestServer();
	
	m4return_t Serialize();
	m4return_t Deserialize();

	// data structure
	ClExecSerializableObjectData m_oData;
};

class M4_DECL_M4EXEFORCHANNEL ClExecSerializableObjectTestClient : public ClExecSerializableObject
{
	// public members
public:
	ClExecSerializableObjectTestClient(iostream*);
	~ClExecSerializableObjectTestClient();
	m4return_t Serialize();
	m4return_t Deserialize();

	// data structure
	ClExecSerializableObjectData m_oData;
};

// class init BDL
class M4_DECL_M4EXEFORCHANNEL ClExecutiveInitBDL
{
public:
	ClExecutiveInitBDL();
	~ClExecutiveInitBDL();

private:
	void * m_oEnv;
};
//## end module%34BA0062031C.additionalDeclarations


//## Class: ClExecutiveForChannel%34B9FE4D001F
//## Category: M4ExeForChannel%3507BDA601C6
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F5FC230163;ExecDumpInfo { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForChannel : public ClExecutiveForOE  //## Inherits: <unnamed>%38F365900218
{
  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForChannel%884623036; C++
      ClExecutiveForChannel (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForChannel%884605453
      ~ClExecutiveForChannel ();


    //## Other Operations (specified)
      //## Operation: StartUp%884605454
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%884605455
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: ExecuteQuit%921058575
      m4return_t ExecuteQuit ();

      //## Operation: ExecuteCommand%922189098
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

  protected:

    //## Other Operations (specified)
      //## Operation: _SetSessionInfo%957346237
      m4return_t _SetSessionInfo ();

    // Data Members for Class Attributes

      //## Attribute: m_poConfiguration%39105679005E
      //## begin ClExecutiveForChannel::m_poConfiguration%39105679005E.attr preserve=no  protected: ClConfiguration * {UA} NULL
      ClConfiguration *m_poConfiguration;
      //## end ClExecutiveForChannel::m_poConfiguration%39105679005E.attr

  private:
  private: //## implementation
};

// Class ClExecutiveForChannel 

//## begin module%34BA0062031C.epilog preserve=yes
//## end module%34BA0062031C.epilog


#endif
