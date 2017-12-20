//## begin module%35582A1D0350.cm preserve=no
//## end module%35582A1D0350.cm

//## begin module%35582A1D0350.cp preserve=no
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
//## end module%35582A1D0350.cp

//## Module: Executive_MDServer%35582A1D0350; Package specification
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Source file: f:\v400\m4exeforchannel\inc\exeformd.hpp

#ifndef executive_mdserver_h
#define executive_mdserver_h 1

//## begin module%35582A1D0350.additionalIncludes preserve=no
//## end module%35582A1D0350.additionalIncludes

//## begin module%35582A1D0350.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
#include <executiveforoe.hpp>
#include <usinterface.hpp>

class ClNavigatorFirewall;
class ClSSInterfaz;


#define M4_EXEFORMD_DATACACHE		"DATA_CACHE"


//## begin module%35582A1D0350.declarations preserve=no
//## end module%35582A1D0350.declarations

//## begin module%35582A1D0350.additionalDeclarations preserve=yes
//## end module%35582A1D0350.additionalDeclarations


//## Class: ClExecutiveForMetaData%355806A1016D
//## Category: M4ExeForChannel::Metadata%392274DC01D8
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3781E5A70010;private: ClUSSessionInterface { -> }
//## Uses: <unnamed>%39227E3503AF;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%39227EF800E9;ClChannel { -> F}
//## Uses: <unnamed>%39227F1E006B;IExecutor { -> F}
//## Uses: <unnamed>%39227FE9015E;ClLConn_Provider { -> F}
//## Uses: <unnamed>%392AB40C012B;ClMutBlock { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForMetaData : public ClExecutiveForOE  //## Inherits: <unnamed>%38F5FBF901DB
{
  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForMetaData%894978816
      ClExecutiveForMetaData (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForMetaData%894978817
      ~ClExecutiveForMetaData ();


    //## Other Operations (specified)
      //## Operation: StartUp%894978812
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%894978813
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: ShutDown%894978814
      m4return_t ShutDown ();

      //## Operation: ExecuteQuit%921232519
      m4return_t ExecuteQuit ();

      //## Operation: ExecuteCommand%922189096
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Reset%958560869; C++
      m4return_t Reset ();

  protected:
    // Data Members for Associations

      //## Association: M4ExeForChannel::Metadata::<unnamed>%39227B3C02E3
      //## Role: ClExecutiveForMetaData::m_pMetaDataSubSystem%39227B3D00F0
      //## begin ClExecutiveForMetaData::m_pMetaDataSubSystem%39227B3D00F0.role preserve=no  protected: ClSSInterfaz { -> 1RFHAN}
      ClSSInterfaz *m_pMetaDataSubSystem;
      //## end ClExecutiveForMetaData::m_pMetaDataSubSystem%39227B3D00F0.role

  private:

    //## Other Operations (specified)
      //## Operation: SetSessionInfo%921232510
      m4return_t SetSessionInfo ();

  private: //## implementation
};

// Class ClExecutiveForMetaData 

//## begin module%35582A1D0350.epilog preserve=yes
//## end module%35582A1D0350.epilog


#endif
