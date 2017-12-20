//## begin module%396C25BF02B5.cm preserve=no
//## end module%396C25BF02B5.cm

//## begin module%396C25BF02B5.cp preserve=no
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
//## end module%396C25BF02B5.cp

//## Module: executiveagent%396C25BF02B5; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\executiveagent.hpp

#ifndef executiveagent_h
#define executiveagent_h 1

//## begin module%396C25BF02B5.additionalIncludes preserve=no
//## end module%396C25BF02B5.additionalIncludes

//## begin module%396C25BF02B5.includes preserve=yes
//## end module%396C25BF02B5.includes

// dspagent
#include <dspagent.hpp>
// actionworker
#include <actionworker.hpp>
//## begin module%396C25BF02B5.declarations preserve=no
//## end module%396C25BF02B5.declarations

//## begin module%396C25BF02B5.additionalDeclarations preserve=yes
//## end module%396C25BF02B5.additionalDeclarations


//## begin ClDspWorker%3975656502E6.preface preserve=yes
//## end ClDspWorker%3975656502E6.preface

//## Class: ClDspWorker%3975656502E6
//## Category: M4Dispatcher::Worker%39F4477E015B
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDspWorker : public ClActionWorker  //## Inherits: <unnamed>%3975658C02A6
{
  //## begin ClDspWorker%3975656502E6.initialDeclarations preserve=yes
  //## end ClDspWorker%3975656502E6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDspWorker%963989741
      ClDspWorker (ClActionsQ &ai_poActionsQ, ClActionsQ &ai_poRecycleBin);


    //## Other Operations (specified)
      //## Operation: ExecuteAction%963989740
      void ExecuteAction (ClBaseAction &ai_oAction);

    // Additional Public Declarations
      //## begin ClDspWorker%3975656502E6.public preserve=yes
      //## end ClDspWorker%3975656502E6.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%397568B8024A
      //## Role: ClDspWorker::m_poRecycleBin%397568B9020F
      //## begin ClDspWorker::m_poRecycleBin%397568B9020F.role preserve=no  protected: ClActionsQ { -> RHAN}
      ClActionsQ *m_poRecycleBin;
      //## end ClDspWorker::m_poRecycleBin%397568B9020F.role

    // Additional Protected Declarations
      //## begin ClDspWorker%3975656502E6.protected preserve=yes
      //## end ClDspWorker%3975656502E6.protected

  private:
    // Additional Private Declarations
      //## begin ClDspWorker%3975656502E6.private preserve=yes
      //## end ClDspWorker%3975656502E6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspWorker%3975656502E6.implementation preserve=yes
      //## end ClDspWorker%3975656502E6.implementation

};

//## begin ClDspWorker%3975656502E6.postscript preserve=yes
//## end ClDspWorker%3975656502E6.postscript

//## begin ClDspExecutiveAgent%396C24DE009E.preface preserve=yes
//## end ClDspExecutiveAgent%396C24DE009E.preface

//## Class: ClDspExecutiveAgent%396C24DE009E
//## Category: M4Dispatcher::Agents%39F451CA0292
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDspExecutiveAgent : public ClDspAgent  //## Inherits: <unnamed>%39756E7C02B0
{
  //## begin ClDspExecutiveAgent%396C24DE009E.initialDeclarations preserve=yes
  //## end ClDspExecutiveAgent%396C24DE009E.initialDeclarations

  public:
    //## begin ClDspExecutiveAgent::ExecutiveActions%39756B210048.preface preserve=yes
    //## end ClDspExecutiveAgent::ExecutiveActions%39756B210048.preface

    //## Class: ExecutiveActions%39756B210048
    //## Category: M4Dispatcher::Agents%39F451CA0292
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { GARBAGE_COLLECTION_ID = 0} ExecutiveActions;

    //## begin ClDspExecutiveAgent::ExecutiveActions%39756B210048.postscript preserve=yes
    //## end ClDspExecutiveAgent::ExecutiveActions%39756B210048.postscript

    //## Constructors (specified)
      //## Operation: ClDspExecutiveAgent%963388992
      ClDspExecutiveAgent ();

    //## Destructor (specified)
      //## Operation: ~ClDspExecutiveAgent%963388993
      ~ClDspExecutiveAgent ();


    //## Other Operations (specified)
      //## Operation: ProcessAction%963388994
      m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: Start%963388995
      m4return_t Start (m4uint32_t ai_iNumWorkers);

      //## Operation: PushAction%963388996
      void PushAction (ClBaseAction *ai_poAction);

      //## Operation: GarbageCollection%963989742
      void GarbageCollection ();

      //## Operation: Terminate%972919261
      //	Metodo sincrono. Manda la señal de Terminate a los Workers y se queda a la
      //	espera de que estos atiendan a la peticion y que acaben.
      //	Solo cuando todos han salido, se retorna del metodo.
      //	Se destruyen los Workers.
      void Terminate ();

  public:
    // Additional Public Declarations
      //## begin ClDspExecutiveAgent%396C24DE009E.public preserve=yes
      //## end ClDspExecutiveAgent%396C24DE009E.public

  protected:

    //## Other Operations (specified)
      //## Operation: Start%1000984419
      m4return_t Start ();

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%396C25410155
      //## Role: ClDspExecutiveAgent::m_oActionsQ%396C25420066
      //## begin ClDspExecutiveAgent::m_oActionsQ%396C25420066.role preserve=no  protected: ClActionsQ { -> VHAN}
      ClActionsQ m_oActionsQ;
      //## end ClDspExecutiveAgent::m_oActionsQ%396C25420066.role

      //## Association: M4Dispatcher::<unnamed>%396C25550031
      //## Role: ClDspExecutiveAgent::m_oWorkerList%396C255502A9
      //## begin ClDspExecutiveAgent::m_oWorkerList%396C255502A9.role preserve=no  protected: ClActionWorkerList { -> VHAN}
      ClActionWorkerList m_oWorkerList;
      //## end ClDspExecutiveAgent::m_oWorkerList%396C255502A9.role

      //## Association: M4Dispatcher::<unnamed>%3975691901A9
      //## Role: ClDspExecutiveAgent::m_oRecycleBin%3975691903DA
      //## begin ClDspExecutiveAgent::m_oRecycleBin%3975691903DA.role preserve=no  protected: ClActionsQ { -> VHAN}
      ClActionsQ m_oRecycleBin;
      //## end ClDspExecutiveAgent::m_oRecycleBin%3975691903DA.role

    // Additional Protected Declarations
      //## begin ClDspExecutiveAgent%396C24DE009E.protected preserve=yes
      //## end ClDspExecutiveAgent%396C24DE009E.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iNumWorkers%397571CE03CB
      //## begin ClDspExecutiveAgent::m_iNumWorkers%397571CE03CB.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iNumWorkers;
      //## end ClDspExecutiveAgent::m_iNumWorkers%397571CE03CB.attr

    // Additional Private Declarations
      //## begin ClDspExecutiveAgent%396C24DE009E.private preserve=yes
      //## end ClDspExecutiveAgent%396C24DE009E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspExecutiveAgent%396C24DE009E.implementation preserve=yes
      //## end ClDspExecutiveAgent%396C24DE009E.implementation

};

//## begin ClDspExecutiveAgent%396C24DE009E.postscript preserve=yes
//## end ClDspExecutiveAgent%396C24DE009E.postscript

// Class ClDspWorker 

// Class ClDspExecutiveAgent 

//## begin module%396C25BF02B5.epilog preserve=yes
//## end module%396C25BF02B5.epilog


#endif
