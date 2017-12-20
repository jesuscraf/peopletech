//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              adminmoncommands.hpp
//	 Project:           M4AdminClient
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
//	==============================================================================
//## end module.cp

//## Module: MonitorCommands; Package specification
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\inc\adminmoncommands.hpp

#ifndef monitorcommands_h
#define monitorcommands_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes

// module for export/import dll.
#include "m4adminclient_dll.hpp"

//## end module.includes

#include <m4string.hpp>
// m4string
#include <m4string.hpp>
// MonitorCache
#include <adminmoncache.hpp>
// PairIdCommand
#include <pairidcommand.hpp>

class ClAdminMonClient;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#define M4_ADMIN_COMMAND_PARAM_SEPARATOR 1
#define M4_ADMIN_COMMAND_SEPARATOR 2
//## end module.additionalDeclarations


//## Class: ClAdminCommandExecutor
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminCommandExecutor 
{
  //## begin ClAdminCommandExecutor.initialDeclarations preserve=yes
  //## end ClAdminCommandExecutor.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminCommandExecutor%917513511
      ClAdminCommandExecutor ();

    //## Destructor (specified)
      //## Operation: ~ClAdminCommandExecutor%917513492
      virtual ~ClAdminCommandExecutor ();


    //## Other Operations (specified)
      //## Operation: Execute%917339813
      virtual m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

      //## Operation: GetNextDataUnit%917522116
      //	Esta funcion recupera la informacion de la siguiente
      //	dataUnit del dataStorage de entrada. Se ha implementado
      //	debido a los cambios hechos en el interface de las PDUS.
      m4return_t GetNextDataUnit (M4DataStorage *ai_dataStorage, char *&ao_data, int &ao_dataSize);

    // Additional Public Declarations
      //## begin ClAdminCommandExecutor.public preserve=yes
      //## end ClAdminCommandExecutor.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_AdminCommandId
      //## begin ClAdminCommandExecutor::m_AdminCommandId.attr preserve=no  protected: char * {UA} NULL
      char *m_AdminCommandId;
      //## end ClAdminCommandExecutor::m_AdminCommandId.attr

    // Additional Protected Declarations
      //## begin ClAdminCommandExecutor.protected preserve=yes
      //## end ClAdminCommandExecutor.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminCommandExecutor.private preserve=yes
      //## end ClAdminCommandExecutor.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminCommandExecutor.implementation preserve=yes
      //## end ClAdminCommandExecutor.implementation

};

//## begin ClAdminCommandExecutor.postscript preserve=yes
//## end ClAdminCommandExecutor.postscript



//## Class: ClAdminCommandExecutorCreator
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminCommandExecutor { -> }

class ClAdminCommandExecutorCreator 
{
  //## begin ClAdminCommandExecutorCreator.initialDeclarations preserve=yes
  //## end ClAdminCommandExecutorCreator.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Create%917339812
      ClAdminCommandExecutor * Create (const char *ai_pcCommandId = NULL);

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36ADACA60099
      //## Role: ClAdminCommandExecutorCreator::<m_pClAdminMonitorCommand>
      //## begin ClAdminCommandExecutorCreator::<m_pClAdminMonitorCommand>.role preserve=no  public: ClAdminMonitorCommand { -> RHGA}
      //## end ClAdminCommandExecutorCreator::<m_pClAdminMonitorCommand>.role

    // Additional Public Declarations
      //## begin ClAdminCommandExecutorCreator.public preserve=yes
      //## end ClAdminCommandExecutorCreator.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminCommandExecutorCreator.protected preserve=yes
      //## end ClAdminCommandExecutorCreator.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminCommandExecutorCreator.private preserve=yes
      //## end ClAdminCommandExecutorCreator.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminCommandExecutorCreator.implementation preserve=yes
      //## end ClAdminCommandExecutorCreator.implementation

};

//## begin ClAdminCommandExecutorCreator.postscript preserve=yes
//## end ClAdminCommandExecutorCreator.postscript



//## Class: ClAdminMonitorCommand
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminMonClient { -> F}

class ClAdminMonitorCommand : public ClPairIdCommand  //## Inherits: <unnamed>
{
  //## begin ClAdminMonitorCommand.initialDeclarations preserve=yes
  //## end ClAdminMonitorCommand.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminMonitorCommand%917274541
      ClAdminMonitorCommand (const char* ai_pcCommandId = NULL);

    //## Destructor (specified)
      //## Operation: ~ClAdminMonitorCommand%917274542
      ~ClAdminMonitorCommand ();


    //## Other Operations (specified)
      //## Operation: Execute%917274544
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

      //## Operation: Execute%917339811
      ClPDU* Execute (m4parsercommandargs_t ai_pInput);

      //## Operation: Execute%917426672
      //	Esta funcion se declara porque hay dependencias con la
      //	clase ClCommand, definida en M4Comsrv.
      m4return_t Execute (void*& exit, ClCmdArguments *args = NULL);

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36ADACA60099
      //## Role: ClAdminMonitorCommand::m_AdminCommandExecutorCreator
      //## begin ClAdminMonitorCommand::m_AdminCommandExecutorCreator.role preserve=no  public: ClAdminCommandExecutorCreator { -> UHAN}
      ClAdminCommandExecutorCreator m_AdminCommandExecutorCreator;
      //## end ClAdminMonitorCommand::m_AdminCommandExecutorCreator.role

    // Additional Public Declarations
      //## begin ClAdminMonitorCommand.public preserve=yes
      //## end ClAdminMonitorCommand.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminMonitorCommand.protected preserve=yes
      //## end ClAdminMonitorCommand.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminMonitorCommand.private preserve=yes
      //## end ClAdminMonitorCommand.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminMonitorCommand.implementation preserve=yes
      //## end ClAdminMonitorCommand.implementation

};

//## begin ClAdminMonitorCommand.postscript preserve=yes
//## end ClAdminMonitorCommand.postscript



//## Class: ClACGetSubsystemInfo
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; ClAdminInfoTagCache { -> }

class ClACGetSubsystemInfo : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACGetSubsystemInfo.initialDeclarations preserve=yes
  //## end ClACGetSubsystemInfo.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACGetSubsystemInfo%917513493
      ClACGetSubsystemInfo (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339814
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACGetSubsystemInfo.public preserve=yes
      //## end ClACGetSubsystemInfo.public

  protected:
    // Additional Protected Declarations
      //## begin ClACGetSubsystemInfo.protected preserve=yes
      //## end ClACGetSubsystemInfo.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetAnswerFromCache%917513494
      m4bool_t GetAnswerFromCache (const char *ai_path, const char *ai_firstItem, const char *ai_secondItem, M4ClString &ao_Answer);

      //## Operation: GetAnswerFromServer%917513495
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path);

      //## Operation: ResetCache%917513496
      void ResetCache (const char *ai_newPath);

    // Data Members for Class Attributes

      //## Attribute: m_Path
      //## begin ClACGetSubsystemInfo::m_Path.attr preserve=no  private: static M4ClString {UA} 
      static M4ClString m_Path;
      //## end ClACGetSubsystemInfo::m_Path.attr

      //## Attribute: m_Ids
      //## begin ClACGetSubsystemInfo::m_Ids.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
      static ClAdminInfoTagCache m_Ids;
      //## end ClACGetSubsystemInfo::m_Ids.attr

      //## Attribute: m_Attributes
      //## begin ClACGetSubsystemInfo::m_Attributes.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
      static ClAdminInfoTagCache m_Attributes;
      //## end ClACGetSubsystemInfo::m_Attributes.attr

      //## Attribute: m_Statistics
      //## begin ClACGetSubsystemInfo::m_Statistics.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
      static ClAdminInfoTagCache m_Statistics;
      //## end ClACGetSubsystemInfo::m_Statistics.attr

    // Additional Private Declarations
      //## begin ClACGetSubsystemInfo.private preserve=yes
      //## end ClACGetSubsystemInfo.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACGetSubsystemInfo.implementation preserve=yes
      //## end ClACGetSubsystemInfo.implementation

};

//## begin ClACGetSubsystemInfo.postscript preserve=yes
//## end ClACGetSubsystemInfo.postscript



//## Class: ClACGetSystemInfo
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClACGetSystemInfo : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACGetSystemInfo.initialDeclarations preserve=yes
  //## end ClACGetSystemInfo.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACGetSystemInfo%917513497
      ClACGetSystemInfo (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339826
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

      //## Operation: GetAnswerFromServer%917513498
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACGetSystemInfo.public preserve=yes
      //## end ClACGetSystemInfo.public

  protected:
    // Additional Protected Declarations
      //## begin ClACGetSystemInfo.protected preserve=yes
      //## end ClACGetSystemInfo.protected

  private:
    // Additional Private Declarations
      //## begin ClACGetSystemInfo.private preserve=yes
      //## end ClACGetSystemInfo.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACGetSystemInfo.implementation preserve=yes
      //## end ClACGetSystemInfo.implementation

};

//## begin ClACGetSystemInfo.postscript preserve=yes
//## end ClACGetSystemInfo.postscript



//## Class: ClACGetSubsystemInfoValues
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; deque { -> }

class ClACGetSubsystemInfoValues : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACGetSubsystemInfoValues.initialDeclarations preserve=yes
  //## end ClACGetSubsystemInfoValues.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACGetSubsystemInfoValues%917513499
      ClACGetSubsystemInfoValues (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339827
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACGetSubsystemInfoValues.public preserve=yes
      //## end ClACGetSubsystemInfoValues.public

  protected:
    // Additional Protected Declarations
      //## begin ClACGetSubsystemInfoValues.protected preserve=yes
      //## end ClACGetSubsystemInfoValues.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetAnswerFromServer%917513500
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path);

      //## Operation: GetAnswerFromCache%917513501
      m4bool_t GetAnswerFromCache (const char *ai_path, const char *ai_firstItem, const char *ai_attId, M4ClString &ao_Answer);

      //## Operation: ResetCache%917513502
      void ResetCache (const char *ai_newPath);

    // Data Members for Class Attributes

      //## Attribute: m_Path
      //## begin ClACGetSubsystemInfoValues::m_Path.attr preserve=no  private: static M4ClString {UA} 
      static M4ClString m_Path;
      //## end ClACGetSubsystemInfoValues::m_Path.attr

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36B0233D014A
      //## Role: ClACGetSubsystemInfoValues::m_AttributeValues
      //## begin ClACGetSubsystemInfoValues::m_AttributeValues.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
      static deque<ClAdminInfoTagCache> m_AttributeValues;
      //## end ClACGetSubsystemInfoValues::m_AttributeValues.role

      //## Association: M4AdminClient::<unnamed>%36B0238B0156
      //## Role: ClACGetSubsystemInfoValues::m_StatisticValues
      //## begin ClACGetSubsystemInfoValues::m_StatisticValues.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
      static deque<ClAdminInfoTagCache> m_StatisticValues;
      //## end ClACGetSubsystemInfoValues::m_StatisticValues.role

    // Additional Private Declarations
      //## begin ClACGetSubsystemInfoValues.private preserve=yes
      //## end ClACGetSubsystemInfoValues.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACGetSubsystemInfoValues.implementation preserve=yes
      //## end ClACGetSubsystemInfoValues.implementation

};

//## begin ClACGetSubsystemInfoValues.postscript preserve=yes
//## end ClACGetSubsystemInfoValues.postscript



//## Class: ClACGetCommandsInfo
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; ClAdminInfoTagCache { -> }
//## Uses: <unnamed>; deque { -> }

class ClACGetCommandsInfo : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACGetCommandsInfo.initialDeclarations preserve=yes
  //## end ClACGetCommandsInfo.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACGetCommandsInfo%917513503
      ClACGetCommandsInfo (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339828
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACGetCommandsInfo.public preserve=yes
      //## end ClACGetCommandsInfo.public

  protected:
    // Additional Protected Declarations
      //## begin ClACGetCommandsInfo.protected preserve=yes
      //## end ClACGetCommandsInfo.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetAnswerFromServer%917513504
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path);

      //## Operation: GetAnswerFromCache%917513505
      m4bool_t GetAnswerFromCache (const char *ai_firstItem, const char *ai_secondItem, const char *ai_thirdItem, const char *ai_fourthItem, M4ClString &ao_Answer);

      //## Operation: ResetCache%917513506
      void ResetCache (const char *ai_newPath);

    // Data Members for Class Attributes

      //## Attribute: m_Path
      //## begin ClACGetCommandsInfo::m_Path.attr preserve=no  private: static M4ClString {UA} 
      static M4ClString m_Path;
      //## end ClACGetCommandsInfo::m_Path.attr

      //## Attribute: m_Commands
      //## begin ClACGetCommandsInfo::m_Commands.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
      static ClAdminInfoTagCache m_Commands;
      //## end ClACGetCommandsInfo::m_Commands.attr

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%36B023C602AF
      //## Role: ClACGetCommandsInfo::m_CommandParams
      //## begin ClACGetCommandsInfo::m_CommandParams.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
      static deque<ClAdminInfoTagCache> m_CommandParams;
      //## end ClACGetCommandsInfo::m_CommandParams.role

    // Additional Private Declarations
      //## begin ClACGetCommandsInfo.private preserve=yes
      //## end ClACGetCommandsInfo.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACGetCommandsInfo.implementation preserve=yes
      //## end ClACGetCommandsInfo.implementation

};

//## begin ClACGetCommandsInfo.postscript preserve=yes
//## end ClACGetCommandsInfo.postscript



//## Class: ClACShutdown
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClACShutdown : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACShutdown.initialDeclarations preserve=yes
  //## end ClACShutdown.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACShutdown%917513507
      ClACShutdown (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339829
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

      //## Operation: GetAnswerFromServer%917513508
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACShutdown.public preserve=yes
      //## end ClACShutdown.public

  protected:
    // Additional Protected Declarations
      //## begin ClACShutdown.protected preserve=yes
      //## end ClACShutdown.protected

  private:
    // Additional Private Declarations
      //## begin ClACShutdown.private preserve=yes
      //## end ClACShutdown.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACShutdown.implementation preserve=yes
      //## end ClACShutdown.implementation

};

//## begin ClACShutdown.postscript preserve=yes
//## end ClACShutdown.postscript



//## Class: ClACExecuteCommand
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClACExecuteCommand : public ClAdminCommandExecutor  //## Inherits: <unnamed>
{
  //## begin ClACExecuteCommand.initialDeclarations preserve=yes
  //## end ClACExecuteCommand.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClACExecuteCommand%917513509
      ClACExecuteCommand (const char *ai_AdminCommandId);


    //## Other Operations (specified)
      //## Operation: Execute%917339830
      m4return_t Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer);

      //## Operation: GetAnswerFromServer%917513510
      m4return_t GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path, const char *ai_commandId, const char **ai_items, int ai_nItems, M4ClString &ao_Answer);

    // Additional Public Declarations
      //## begin ClACExecuteCommand.public preserve=yes
      //## end ClACExecuteCommand.public

  protected:
    // Additional Protected Declarations
      //## begin ClACExecuteCommand.protected preserve=yes
      //## end ClACExecuteCommand.protected

  private:
    // Additional Private Declarations
      //## begin ClACExecuteCommand.private preserve=yes
      //## end ClACExecuteCommand.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClACExecuteCommand.implementation preserve=yes
      //## end ClACExecuteCommand.implementation

};

//## begin ClACExecuteCommand.postscript preserve=yes
//## end ClACExecuteCommand.postscript



// Class ClAdminCommandExecutor 





// Class ClAdminCommandExecutorCreator 





// Class ClAdminMonitorCommand 





// Class ClACGetSubsystemInfo 





// Class ClACGetSystemInfo 





// Class ClACGetSubsystemInfoValues 





// Class ClACGetCommandsInfo 





// Class ClACShutdown 





// Class ClACExecuteCommand 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
