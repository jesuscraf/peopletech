//## begin module%3993DA040056.cm preserve=no
//## end module%3993DA040056.cm

//## begin module%3993DA040056.cp preserve=no
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
//## end module%3993DA040056.cp

//## Module: ussession40%3993DA040056; Package specification
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Source file: F:\integration\m4userinterface\inc\ussession40.hpp

#ifndef ussession40_h
#define ussession40_h 1

//## begin module%3993DA040056.additionalIncludes preserve=no
//## end module%3993DA040056.additionalIncludes

//## begin module%3993DA040056.includes preserve=yes
//## end module%3993DA040056.includes

#include <usinterface.hpp>
#include <ussession.hpp>
//## begin module%3993DA040056.declarations preserve=no
//## end module%3993DA040056.declarations

//## begin module%3993DA040056.additionalDeclarations preserve=yes
//## end module%3993DA040056.additionalDeclarations


//## begin ClUSSession40%3993D1C20010.preface preserve=yes
//## end ClUSSession40%3993D1C20010.preface

//## Class: ClUSSession40%3993D1C20010
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3993D3D70233;ClUSSessionStats { -> }

class M4_DECL_M4USERINTERFACE ClUSSession40 : public ClUSSessionInterface  //## Inherits: <unnamed>%39DA0F4A0321
{
  //## begin ClUSSession40%3993D1C20010.initialDeclarations preserve=yes
  //## end ClUSSession40%3993D1C20010.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSSession40%965977091
      ClUSSession40 (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession);

    //## Destructor (specified)
      //## Operation: ~ClUSSession40%965977092
      ~ClUSSession40 ();


    //## Other Operations (specified)
      //## Operation: AddRequest%965977093; C++
      void AddRequest ();

      //## Operation: RemoveRequest%965977094; C++
      void RemoveRequest ();

      //## Operation: Recycle%965977095; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%965977096
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%965977097
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%965977098
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClUSSession40%3993D1C20010.public preserve=yes
      //## end ClUSSession40%3993D1C20010.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSSession40%3993D1C20010.protected preserve=yes
      //## end ClUSSession40%3993D1C20010.protected

  private:
    // Additional Private Declarations
      //## begin ClUSSession40%3993D1C20010.private preserve=yes
      //## end ClUSSession40%3993D1C20010.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSSession40%3993D1C20010.implementation preserve=yes
      //## end ClUSSession40%3993D1C20010.implementation

};

//## begin ClUSSession40%3993D1C20010.postscript preserve=yes
//## end ClUSSession40%3993D1C20010.postscript

// Class ClUSSession40 

//## begin module%3993DA040056.epilog preserve=yes
//## end module%3993DA040056.epilog


#endif
