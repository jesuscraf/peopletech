//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              adminpduexec.hpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the objects to process the administration PDUs
//
//
//	==============================================================================
//## end module.cp

//## Module: adminPDUExecutor; Package specification
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\inc\adminpduexec.hpp

#ifndef adminpduexecutor_h
#define adminpduexecutor_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4eventhandlers.hpp>
//## end module.includes

// controlpdus
#include <controlpdus.hpp>
// servicepdus
#include <servicepdus.hpp>
// Protocol
#include <protocol.hpp>
// admonpdus
//#include <admonpdus.hpp>
// ClAdminTag
#include <cladmintag.hpp>

class ClSystemInterface;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
class ClCSAdministrator;
//## end module.additionalDeclarations


//## Class: ClAdminPDUExecutor; Abstract
//	Abstract class. Each derived class from this procces an
//	administration PDU type.
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClProtocol { -> }
//## Uses: <unnamed>; ClAdminTagList { -> }
//## Uses: <unnamed>; ClSystemInterface { -> F}

class ClAdminPDUExecutor 
{
  //## begin ClAdminPDUExecutor.initialDeclarations preserve=yes
  //## end ClAdminPDUExecutor.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminPDUExecutor%918150009
      ClAdminPDUExecutor (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%897569278
      //	This method analizes the input PDU, get the answer, and
      //	builds the output PDU.
      //	The memory for the output PDU is allocated here. This
      //	memory must be deleted in the function this method is
      //	called from.
      virtual m4return_t Execute (ClPDU *ai_poInputPDU) = 0;

      //## Operation: DumpInputPDU%897907989
      //	This method is for debug only.
      //	It send to standard output the contents of the
      //	dataStorage in the input PDU.
      void DumpInputPDU (ClPDU *ai_poInputPDU);

      //## Operation: GetNextDataUnit%917522117
      m4return_t GetNextDataUnit (M4DataStorage *ai_dataStorage, char *&ao_data, int &ao_dataSize);

      //## Operation: SendAckPDU%918150010
      m4return_t SendAckPDU (m4uint32_t ai_iRequestId, ClTagList *ai_poTagList=NULL);

      //## Operation: SendErrorPDU%918150011
      m4return_t SendErrorPDU (m4uint32_t ai_iRequestId);

      //## Operation: SendReplyPDU%918150012
      m4return_t SendReplyPDU (m4uint32_t ai_iRequestId, M4DataStorage *ai_poDataStorage = NULL);

    // Data Members for Class Attributes

      //## Attribute: m_poAdministrator
      //## begin ClAdminPDUExecutor::m_poAdministrator.attr preserve=no  public: ClCSAdministrator * {UA} 
      ClCSAdministrator *m_poAdministrator;
      //## end ClAdminPDUExecutor::m_poAdministrator.attr

    // Additional Public Declarations
      //## begin ClAdminPDUExecutor.public preserve=yes

      //## end ClAdminPDUExecutor.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminPDUExecutor.protected preserve=yes
      //## end ClAdminPDUExecutor.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminPDUExecutor.private preserve=yes
      //## end ClAdminPDUExecutor.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminPDUExecutor.implementation preserve=yes
      //## end ClAdminPDUExecutor.implementation

};

//## begin ClAdminPDUExecutor.postscript preserve=yes
//## end ClAdminPDUExecutor.postscript



//## Class: ClAdminAuthenticationPDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAckPDU { -> }

class ClAdminAuthenticationPDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminAuthenticationPDU.initialDeclarations preserve=yes
  //## end ClAdminAuthenticationPDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminAuthenticationPDU%918150013
      ClAdminAuthenticationPDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%897639336
      m4return_t Execute (ClPDU *ai_poInputPDU);

    // Additional Public Declarations
      //## begin ClAdminAuthenticationPDU.public preserve=yes
      //## end ClAdminAuthenticationPDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminAuthenticationPDU.protected preserve=yes
      //## end ClAdminAuthenticationPDU.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminAuthenticationPDU.private preserve=yes
      //## end ClAdminAuthenticationPDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminAuthenticationPDU.implementation preserve=yes
      //## end ClAdminAuthenticationPDU.implementation

};

//## begin ClAdminAuthenticationPDU.postscript preserve=yes
//## end ClAdminAuthenticationPDU.postscript



//## Class: ClAdminPDUExecutorCreator
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminPDUExecutor { -> }

class ClAdminPDUExecutorCreator 
{
  //## begin ClAdminPDUExecutorCreator.initialDeclarations preserve=yes
  //## end ClAdminPDUExecutorCreator.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Create%897569279
      //	This method return a PDU executor object for the PDU id
      //	specified in the input argument.
      ClAdminPDUExecutor * Create (ClCSAdministrator *ai_poAdministrator, m4int16_t ai_iPDUId);

    // Data Members for Associations

      //## Association: M4ComSrv::Administrator::<unnamed>%35800AEE029F
      //## Role: ClAdminPDUExecutorCreator::<m_pClCSAdministrator>
      //## begin ClAdminPDUExecutorCreator::<m_pClCSAdministrator>.role preserve=no  public: ClCSAdministrator {1 -> RHGA}
      //## end ClAdminPDUExecutorCreator::<m_pClCSAdministrator>.role

    // Additional Public Declarations
      //## begin ClAdminPDUExecutorCreator.public preserve=yes
      //## end ClAdminPDUExecutorCreator.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminPDUExecutorCreator.protected preserve=yes
      //## end ClAdminPDUExecutorCreator.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminPDUExecutorCreator.private preserve=yes
      //## end ClAdminPDUExecutorCreator.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminPDUExecutorCreator.implementation preserve=yes
      //## end ClAdminPDUExecutorCreator.implementation

};

//## begin ClAdminPDUExecutorCreator.postscript preserve=yes
//## end ClAdminPDUExecutorCreator.postscript



//## Class: ClAdminConnectPDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAckPDU { -> }

class ClAdminConnectPDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminConnectPDU.initialDeclarations preserve=yes
  //## end ClAdminConnectPDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminConnectPDU%918150014
      ClAdminConnectPDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%899977157
      m4return_t Execute (ClPDU *ai_poInputPDU);

    // Additional Public Declarations
      //## begin ClAdminConnectPDU.public preserve=yes
      //## end ClAdminConnectPDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminConnectPDU.protected preserve=yes
      //## end ClAdminConnectPDU.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminConnectPDU.private preserve=yes
      //## end ClAdminConnectPDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminConnectPDU.implementation preserve=yes
      //## end ClAdminConnectPDU.implementation

};

//## begin ClAdminConnectPDU.postscript preserve=yes
//## end ClAdminConnectPDU.postscript



//## Class: ClAdminRequestPDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClConsulParamPDU { -> }
//## Uses: <unnamed>; ClRequestPDU { -> }

class ClAdminRequestPDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminRequestPDU.initialDeclarations preserve=yes
  //## end ClAdminRequestPDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminRequestPDU%918150015
      ClAdminRequestPDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%897639333
      m4return_t Execute (ClPDU *ai_poInputPDU);

      //## Operation: GetAdminCommandId%899816501
      const m4pchar_t GetAdminCommandId (ClPDU *ai_poInputPDU = NULL);

      //## Operation: SerializeCommand%900950039
      m4return_t SerializeCommand (ClSystemInterface *ai_posystem, ClAdminTagList &ai_oCommandList, M4ClString &ao_oSerializedString);

      //## Operation: SerializeCommandParams%901092590
      m4return_t SerializeCommandParams (ClSystemInterface *ai_posystem, ClAdminTagList &ai_oCommandList, M4ClString &ao_oSerializedString);

    // Additional Public Declarations
      //## begin ClAdminRequestPDU.public preserve=yes
      //## end ClAdminRequestPDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminRequestPDU.protected preserve=yes
      //## end ClAdminRequestPDU.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetSystemInfo%898002523
      m4return_t GetSystemInfo (ClPDU *ai_poInputPDU);

      //## Operation: GetSubsystemInfo%898002524
      m4return_t GetSubsystemInfo (ClPDU *ai_poInputPDU);

      //## Operation: GetSubsystemInfoValues%898002525
      m4return_t GetSubsystemInfoValues (ClPDU *ai_poInputPDU);

      //## Operation: GetCommandsInfo%898002526
      m4return_t GetCommandsInfo (ClPDU *ai_poInputPDU);

      //## Operation: ExecuteCommand%898002529
      m4return_t ExecuteCommand (ClPDU *ai_poInputPDU);

      //## Operation: ShutDown%898002530
      m4return_t ShutDown (ClPDU *ai_poInputPDU);

    // Additional Private Declarations
      //## begin ClAdminRequestPDU.private preserve=yes
      //## end ClAdminRequestPDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminRequestPDU.implementation preserve=yes
      //## end ClAdminRequestPDU.implementation

};

//## begin ClAdminRequestPDU.postscript preserve=yes
//## end ClAdminRequestPDU.postscript



//## Class: ClAdminUnknownPDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAckPDU { -> }

class ClAdminUnknownPDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminUnknownPDU.initialDeclarations preserve=yes
  //## end ClAdminUnknownPDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminUnknownPDU%918150016
      ClAdminUnknownPDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%897639338
      m4return_t Execute (ClPDU *ai_poInputPDU);

    // Additional Public Declarations
      //## begin ClAdminUnknownPDU.public preserve=yes
      //## end ClAdminUnknownPDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminUnknownPDU.protected preserve=yes
      //## end ClAdminUnknownPDU.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminUnknownPDU.private preserve=yes
      //## end ClAdminUnknownPDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminUnknownPDU.implementation preserve=yes
      //## end ClAdminUnknownPDU.implementation

};

//## begin ClAdminUnknownPDU.postscript preserve=yes
//## end ClAdminUnknownPDU.postscript



//## Class: ClAdminDisconnectPDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAckPDU { -> }

class ClAdminDisconnectPDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminDisconnectPDU.initialDeclarations preserve=yes
  //## end ClAdminDisconnectPDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminDisconnectPDU%918150017
      ClAdminDisconnectPDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%898072222
      m4return_t Execute (ClPDU *ai_poInputPDU);

    // Additional Public Declarations
      //## begin ClAdminDisconnectPDU.public preserve=yes
      //## end ClAdminDisconnectPDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminDisconnectPDU.protected preserve=yes
      //## end ClAdminDisconnectPDU.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminDisconnectPDU.private preserve=yes
      //## end ClAdminDisconnectPDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminDisconnectPDU.implementation preserve=yes
      //## end ClAdminDisconnectPDU.implementation

};

//## begin ClAdminDisconnectPDU.postscript preserve=yes
//## end ClAdminDisconnectPDU.postscript



//## Class: ClAdminSetrolePDU
//## Category: M4AdminSrv
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAckPDU { -> }

class ClAdminSetrolePDU : public ClAdminPDUExecutor  //## Inherits: <unnamed>
{
  //## begin ClAdminSetrolePDU.initialDeclarations preserve=yes
  //## end ClAdminSetrolePDU.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminSetrolePDU%918150018
      ClAdminSetrolePDU (ClCSAdministrator *ai_poAdministrator);


    //## Other Operations (specified)
      //## Operation: Execute%897639337
      m4return_t Execute (ClPDU *ai_poInputPDU);

    // Additional Public Declarations
      //## begin ClAdminSetrolePDU.public preserve=yes
      //## end ClAdminSetrolePDU.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminSetrolePDU.protected preserve=yes
      //## end ClAdminSetrolePDU.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminSetrolePDU.private preserve=yes
      //## end ClAdminSetrolePDU.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminSetrolePDU.implementation preserve=yes
      //## end ClAdminSetrolePDU.implementation

};

//## begin ClAdminSetrolePDU.postscript preserve=yes
//## end ClAdminSetrolePDU.postscript



// Class ClAdminPDUExecutor 





// Class ClAdminAuthenticationPDU 





// Class ClAdminPDUExecutorCreator 





// Class ClAdminConnectPDU 





// Class ClAdminRequestPDU 





// Class ClAdminUnknownPDU 





// Class ClAdminDisconnectPDU 





// Class ClAdminSetrolePDU 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
