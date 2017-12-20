//## begin module%396C358D01C2.cm preserve=no
//## end module%396C358D01C2.cm

//## begin module%396C358D01C2.cp preserve=no
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
//## end module%396C358D01C2.cp

//## Module: adminactions%396C358D01C2; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\adminactions.hpp

#ifndef adminactions_h
#define adminactions_h 1

//## begin module%396C358D01C2.additionalIncludes preserve=no
//## end module%396C358D01C2.additionalIncludes

//## begin module%396C358D01C2.includes preserve=yes
//## end module%396C358D01C2.includes

// m4condition
#include <m4condition.hpp>
// clbaseaction
#include <dspactions.hpp>
//## begin module%396C358D01C2.declarations preserve=no
//## end module%396C358D01C2.declarations

//## begin module%396C358D01C2.additionalDeclarations preserve=yes
//## end module%396C358D01C2.additionalDeclarations


//## begin ClStartConfigAction%3959C00100E8.preface preserve=yes
//## end ClStartConfigAction%3959C00100E8.preface

//## Class: ClStartConfigAction%3959C00100E8
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStartConfigAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3959C021006C
{
  //## begin ClStartConfigAction%3959C00100E8.initialDeclarations preserve=yes
  //## end ClStartConfigAction%3959C00100E8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStartConfigAction%962179778
      ClStartConfigAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962179779
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClStartConfigAction%3959C00100E8.public preserve=yes
      //## end ClStartConfigAction%3959C00100E8.public

  protected:
    // Additional Protected Declarations
      //## begin ClStartConfigAction%3959C00100E8.protected preserve=yes
      //## end ClStartConfigAction%3959C00100E8.protected

  private:
    // Additional Private Declarations
      //## begin ClStartConfigAction%3959C00100E8.private preserve=yes
      //## end ClStartConfigAction%3959C00100E8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStartConfigAction%3959C00100E8.implementation preserve=yes
      //## end ClStartConfigAction%3959C00100E8.implementation

};

//## begin ClStartConfigAction%3959C00100E8.postscript preserve=yes
//## end ClStartConfigAction%3959C00100E8.postscript

//## begin ClStopConfigAction%3959C00D029E.preface preserve=yes
//## end ClStopConfigAction%3959C00D029E.preface

//## Class: ClStopConfigAction%3959C00D029E
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStopConfigAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3959C02301EC
{
  //## begin ClStopConfigAction%3959C00D029E.initialDeclarations preserve=yes
  //## end ClStopConfigAction%3959C00D029E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStopConfigAction%962179776
      ClStopConfigAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962179777
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClStopConfigAction%3959C00D029E.public preserve=yes
      //## end ClStopConfigAction%3959C00D029E.public

  protected:
    // Additional Protected Declarations
      //## begin ClStopConfigAction%3959C00D029E.protected preserve=yes
      //## end ClStopConfigAction%3959C00D029E.protected

  private:
    // Additional Private Declarations
      //## begin ClStopConfigAction%3959C00D029E.private preserve=yes
      //## end ClStopConfigAction%3959C00D029E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStopConfigAction%3959C00D029E.implementation preserve=yes
      //## end ClStopConfigAction%3959C00D029E.implementation

};

//## begin ClStopConfigAction%3959C00D029E.postscript preserve=yes
//## end ClStopConfigAction%3959C00D029E.postscript

class ClRemoveConfigAction : public ClDspAuthenticatedAction
{
   public:
      ClRemoveConfigAction (ClBaseActionExecutive *ai_poExecutive);
      m4return_t Execute ();
};

//## begin ClCheckConsistencyAction%396AEE8D01A3.preface preserve=yes
class ClCloseConnectionAdminAction : public ClBaseAction
{
   public:
      ClCloseConnectionAdminAction (ClBaseActionExecutive *ai_poExecutive);
      m4return_t Execute ();
};

//## end ClCheckConsistencyAction%396AEE8D01A3.preface

//## Class: ClCheckConsistencyAction%396AEE8D01A3
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCheckConsistencyAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%396AEEAA0082
{
  //## begin ClCheckConsistencyAction%396AEE8D01A3.initialDeclarations preserve=yes
  //## end ClCheckConsistencyAction%396AEE8D01A3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCheckConsistencyAction%963302391
      ClCheckConsistencyAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%963302392
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCheckConsistencyAction%396AEE8D01A3.public preserve=yes
      //## end ClCheckConsistencyAction%396AEE8D01A3.public

  protected:
    // Additional Protected Declarations
      //## begin ClCheckConsistencyAction%396AEE8D01A3.protected preserve=yes
      //## end ClCheckConsistencyAction%396AEE8D01A3.protected

  private:
    // Additional Private Declarations
      //## begin ClCheckConsistencyAction%396AEE8D01A3.private preserve=yes
      //## end ClCheckConsistencyAction%396AEE8D01A3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCheckConsistencyAction%396AEE8D01A3.implementation preserve=yes
      //## end ClCheckConsistencyAction%396AEE8D01A3.implementation

};

//## begin ClCheckConsistencyAction%396AEE8D01A3.postscript preserve=yes
//## end ClCheckConsistencyAction%396AEE8D01A3.postscript

//## begin ClConfigurationsQueryAction%3948E5F50347.preface preserve=yes
//## end ClConfigurationsQueryAction%3948E5F50347.preface

//## Class: ClConfigurationsQueryAction%3948E5F50347
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConfigurationsQueryAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3948E6200295
{
  //## begin ClConfigurationsQueryAction%3948E5F50347.initialDeclarations preserve=yes
  //## end ClConfigurationsQueryAction%3948E5F50347.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConfigurationsQueryAction%961054360
      ClConfigurationsQueryAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054361
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClConfigurationsQueryAction%3948E5F50347.public preserve=yes
      //## end ClConfigurationsQueryAction%3948E5F50347.public

  protected:
    // Additional Protected Declarations
      //## begin ClConfigurationsQueryAction%3948E5F50347.protected preserve=yes
      //## end ClConfigurationsQueryAction%3948E5F50347.protected

  private:
    // Additional Private Declarations
      //## begin ClConfigurationsQueryAction%3948E5F50347.private preserve=yes
      //## end ClConfigurationsQueryAction%3948E5F50347.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConfigurationsQueryAction%3948E5F50347.implementation preserve=yes
      //## end ClConfigurationsQueryAction%3948E5F50347.implementation

};

//## begin ClConfigurationsQueryAction%3948E5F50347.postscript preserve=yes
//## end ClConfigurationsQueryAction%3948E5F50347.postscript

//## begin ClStopDpchAction%39FEE8550055.preface preserve=yes
//## end ClStopDpchAction%39FEE8550055.preface

//## Class: ClStopDpchAction%39FEE8550055
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStopDpchAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%39FEE87401F4
{
  //## begin ClStopDpchAction%39FEE8550055.initialDeclarations preserve=yes
  //## end ClStopDpchAction%39FEE8550055.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStopDpchAction%973007655
      ClStopDpchAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%973007656
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClStopDpchAction%39FEE8550055.public preserve=yes
      //## end ClStopDpchAction%39FEE8550055.public

  protected:
    // Additional Protected Declarations
      //## begin ClStopDpchAction%39FEE8550055.protected preserve=yes
      //## end ClStopDpchAction%39FEE8550055.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::Actions::Admin Actions::<unnamed>%39FEF088014D
      //## Role: ClStopDpchAction::m_poDpchStopper%39FEF0890091
      //	Puede ser NULL
      //## begin ClStopDpchAction::m_poDpchStopper%39FEF0890091.role preserve=no  private: ClCondition { -> RHAN}
      ClCondition *m_poDpchStopper;
      //## end ClStopDpchAction::m_poDpchStopper%39FEF0890091.role

    // Additional Private Declarations
      //## begin ClStopDpchAction%39FEE8550055.private preserve=yes
      //## end ClStopDpchAction%39FEE8550055.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStopDpchAction%39FEE8550055.implementation preserve=yes
      //## end ClStopDpchAction%39FEE8550055.implementation

};

//## begin ClStopDpchAction%39FEE8550055.postscript preserve=yes
//## end ClStopDpchAction%39FEE8550055.postscript

//## begin ClSingalAction%3B1CFF3301A9.preface preserve=yes
//## end ClSingalAction%3B1CFF3301A9.preface

//## Class: ClSingalAction%3B1CFF3301A9
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSingalAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3B1CFF5F03C9
{
  //## begin ClSingalAction%3B1CFF3301A9.initialDeclarations preserve=yes
  //## end ClSingalAction%3B1CFF3301A9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSingalAction%991756196
      ClSingalAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%991756197
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClSingalAction%3B1CFF3301A9.public preserve=yes
      //## end ClSingalAction%3B1CFF3301A9.public

  protected:
    // Additional Protected Declarations
      //## begin ClSingalAction%3B1CFF3301A9.protected preserve=yes
      //## end ClSingalAction%3B1CFF3301A9.protected

  private:
    // Additional Private Declarations
      //## begin ClSingalAction%3B1CFF3301A9.private preserve=yes
      //## end ClSingalAction%3B1CFF3301A9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSingalAction%3B1CFF3301A9.implementation preserve=yes
      //## end ClSingalAction%3B1CFF3301A9.implementation

};

//## begin ClSingalAction%3B1CFF3301A9.postscript preserve=yes
//## end ClSingalAction%3B1CFF3301A9.postscript

//## begin ClRegisterTargetAction%3B1F60F60138.preface preserve=yes
//## end ClRegisterTargetAction%3B1F60F60138.preface

//## Class: ClRegisterTargetAction%3B1F60F60138
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRegisterTargetAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3B1F61380011
{
  //## begin ClRegisterTargetAction%3B1F60F60138.initialDeclarations preserve=yes
  //## end ClRegisterTargetAction%3B1F60F60138.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRegisterTargetAction%991912264
      ClRegisterTargetAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%991912265
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClRegisterTargetAction%3B1F60F60138.public preserve=yes
      //## end ClRegisterTargetAction%3B1F60F60138.public

  protected:
    // Additional Protected Declarations
      //## begin ClRegisterTargetAction%3B1F60F60138.protected preserve=yes
      //## end ClRegisterTargetAction%3B1F60F60138.protected

  private:
    // Additional Private Declarations
      //## begin ClRegisterTargetAction%3B1F60F60138.private preserve=yes
      //## end ClRegisterTargetAction%3B1F60F60138.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRegisterTargetAction%3B1F60F60138.implementation preserve=yes
      //## end ClRegisterTargetAction%3B1F60F60138.implementation

};

//## begin ClRegisterTargetAction%3B1F60F60138.postscript preserve=yes
class ClUpdateAppCtlConfigAction : public ClDspAuthenticatedAction 
{
 public:
      ClUpdateAppCtlConfigAction (ClBaseActionExecutive *ai_poExecutive);


      m4return_t Execute ();

 };

//## end ClRegisterTargetAction%3B1F60F60138.postscript

// Class ClStartConfigAction 

// Class ClStopConfigAction 

// Class ClCheckConsistencyAction 

// Class ClConfigurationsQueryAction 

// Class ClStopDpchAction 

// Class ClSingalAction 

// Class ClRegisterTargetAction 

//## begin module%396C358D01C2.epilog preserve=yes
//## end module%396C358D01C2.epilog


#endif
