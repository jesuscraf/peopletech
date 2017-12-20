//## begin module%369CCF510132.cm preserve=no
//## end module%369CCF510132.cm

//## begin module%369CCF510132.cp preserve=no
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
//## end module%369CCF510132.cp

//## Module: SystemInterface%369CCF510132; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\systeminteface.hpp

#ifndef systeminterface_h
#define systeminterface_h 1

//## begin module%369CCF510132.additionalIncludes preserve=no
//## end module%369CCF510132.additionalIncludes

//## begin module%369CCF510132.includes preserve=yes
#include <clbaseobject.hpp>
//## end module%369CCF510132.includes

// ClAdminTag
#include <cladmintag.hpp>
// DataStorages
#include <datastorages.hpp>
//## begin module%369CCF510132.declarations preserve=no
//## end module%369CCF510132.declarations

//## begin module%369CCF510132.additionalDeclarations preserve=yes
//## end module%369CCF510132.additionalDeclarations


//## begin ClSystemInterface%369CDCD70311.preface preserve=yes
//## end ClSystemInterface%369CDCD70311.preface

//## Class: ClSystemInterface%369CDCD70311
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C832A020A;M4DataStorage { -> }
//## Uses: <unnamed>%377C83430329;ClAdminTagList { -> }

class ClSystemInterface 
{
  //## begin ClSystemInterface%369CDCD70311.initialDeclarations preserve=yes
  //## end ClSystemInterface%369CDCD70311.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetId%916246406
      //	Returns the subsystem id
      virtual m4objid_t GetId () = 0;

      //## Operation: ShutDown%916246407
      //	Shuts down this subsystem and all the subsystems matched to it
      virtual m4return_t ShutDown () = 0;

      //## Operation: GetCommandsIdRoot%916246413
      virtual m4return_t GetCommandsIdRoot (m4pchar_t ai_szPath, 	// The path to the node that it wanted to process the final function. The path
      	// is compossed by all the identifiers of the subsystems witch are in the tree
      	// path to the wanted subsystem, separated all of then by point and finished
      	// by point and coma.
      ClAdminTagList &aio_oCommandList) = 0;

      //## Operation: GetCommandParamsRoot%916246414
      virtual m4return_t GetCommandParamsRoot (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oParamList) = 0;

      //## Operation: ExecuteCommand%916246416
      virtual m4return_t ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList) = 0;

      //## Operation: RemoveSubsystem%916246417
      //	This method in this class only dispach the same method to the apropiate
      //	subsystem after looking for it, of course only if the serching is
      //	successfull.
      virtual m4return_t RemoveSubsystem (m4objid_t ai_lIdBase, 	// That is the subsystem id from witch the subsystem is going to be removed.
      	// First of all the system does a finding of that subsystem and then call its
      	// own RemoveSubsystem method
      m4objid_t ai_lSubsystemId) = 0;

      //## Operation: GetLiteral%916246418
      virtual m4pchar_t GetLiteral (m4pchar_t ai_tag) = 0;

      //## Operation: GetStringSpecificationValue%916246419
      virtual m4return_t GetStringSpecificationValue (char *ai_szpath, char *ai_szProperty, char *ao_szValue, size_t ai_iLenBuffer, size_t &ao_iLenOut) = 0;

      //## Operation: GetSubsystemsId%920376401
      //	This method in this class only dispach the same method to the apropiate
      //	subsystem after looking for it, of course only if the serching is
      //	successfull.
      virtual m4return_t GetSubsystemsId (m4pchar_t ai_szPath, 	// The path to the node that it wanted to process the final function. The path
      	// is compossed by all the identifiers of the subsystems witch are in the tree
      	// path to the wanted subsystem, separated all of then by point and finished
      	// by point and coma.
      vector <m4objid_t > &ao_poTags) = 0;

      //## Operation: Exit%929984612
      virtual m4return_t Exit (m4uint32_t ai_lDeferedTime = 0) = 0;

      //## Operation: GetSubsystemAttribValue%994933972
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage) = 0;

      //## Operation: GetSubsystemAttribsId%994933973
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage) = 0;

      //## Operation: GetStatisticAttribValue%994933974
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage) = 0;

      //## Operation: GetStatisticAttribsId%994933975
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage) = 0;

    // Additional Public Declarations
      //## begin ClSystemInterface%369CDCD70311.public preserve=yes
      //## end ClSystemInterface%369CDCD70311.public

  protected:
    // Additional Protected Declarations
      //## begin ClSystemInterface%369CDCD70311.protected preserve=yes
      //## end ClSystemInterface%369CDCD70311.protected

  private:
    // Additional Private Declarations
      //## begin ClSystemInterface%369CDCD70311.private preserve=yes
      //## end ClSystemInterface%369CDCD70311.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSystemInterface%369CDCD70311.implementation preserve=yes
      //## end ClSystemInterface%369CDCD70311.implementation

};

//## begin ClSystemInterface%369CDCD70311.postscript preserve=yes
//## end ClSystemInterface%369CDCD70311.postscript

// Class ClSystemInterface 

//## begin module%369CCF510132.epilog preserve=yes
//## end module%369CCF510132.epilog


#endif
