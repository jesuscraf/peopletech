//## begin module%3948844C0316.cm preserve=no
//## end module%3948844C0316.cm

//## begin module%3948844C0316.cp preserve=no
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
//## end module%3948844C0316.cp

//## Module: dspactions%3948844C0316; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\dspactions.hpp

#ifndef dspactions_h
#define dspactions_h 1

//## begin module%3948844C0316.additionalIncludes preserve=no
//## end module%3948844C0316.additionalIncludes

//## begin module%3948844C0316.includes preserve=yes
//## end module%3948844C0316.includes

// clbaseaction
#include <clbaseaction.hpp>
//## begin module%3948844C0316.declarations preserve=no
//## end module%3948844C0316.declarations

//## begin module%3948844C0316.additionalDeclarations preserve=yes
//## end module%3948844C0316.additionalDeclarations


//## begin ClPostReadPDUAction%3947BCFC0324.preface preserve=yes
//## end ClPostReadPDUAction%3947BCFC0324.preface

//## Class: ClPostReadPDUAction%3947BCFC0324
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostReadPDUAction : public ClBaseAction  //## Inherits: <unnamed>%3948825803A3
{
  //## begin ClPostReadPDUAction%3947BCFC0324.initialDeclarations preserve=yes
  //## end ClPostReadPDUAction%3947BCFC0324.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostReadPDUAction%961054339
      ClPostReadPDUAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054340
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostReadPDUAction%3947BCFC0324.public preserve=yes
      //## end ClPostReadPDUAction%3947BCFC0324.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostReadPDUAction%3947BCFC0324.protected preserve=yes
      //## end ClPostReadPDUAction%3947BCFC0324.protected

  private:
    // Additional Private Declarations
      //## begin ClPostReadPDUAction%3947BCFC0324.private preserve=yes
      //## end ClPostReadPDUAction%3947BCFC0324.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostReadPDUAction%3947BCFC0324.implementation preserve=yes
      //## end ClPostReadPDUAction%3947BCFC0324.implementation

};

//## begin ClPostReadPDUAction%3947BCFC0324.postscript preserve=yes
//## end ClPostReadPDUAction%3947BCFC0324.postscript

//## begin ClCloseConnectionAction%396C95CF03A6.preface preserve=yes
//## end ClCloseConnectionAction%396C95CF03A6.preface

//## Class: ClCloseConnectionAction%396C95CF03A6
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCloseConnectionAction : public ClBaseAction  //## Inherits: <unnamed>%396C95DD00E9
{
  //## begin ClCloseConnectionAction%396C95CF03A6.initialDeclarations preserve=yes
  //## end ClCloseConnectionAction%396C95CF03A6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCloseConnectionAction%963417631
      ClCloseConnectionAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%963417632
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCloseConnectionAction%396C95CF03A6.public preserve=yes
      //## end ClCloseConnectionAction%396C95CF03A6.public

  protected:
    // Additional Protected Declarations
      //## begin ClCloseConnectionAction%396C95CF03A6.protected preserve=yes
      //## end ClCloseConnectionAction%396C95CF03A6.protected

  private:
    // Additional Private Declarations
      //## begin ClCloseConnectionAction%396C95CF03A6.private preserve=yes
      //## end ClCloseConnectionAction%396C95CF03A6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCloseConnectionAction%396C95CF03A6.implementation preserve=yes
      //## end ClCloseConnectionAction%396C95CF03A6.implementation

};

//## begin ClCloseConnectionAction%396C95CF03A6.postscript preserve=yes
//## end ClCloseConnectionAction%396C95CF03A6.postscript

//## begin ClUpdateConfigAction%396358090068.preface preserve=yes
//## end ClUpdateConfigAction%396358090068.preface

//## Class: ClUpdateConfigAction%396358090068
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClUpdateConfigAction : public ClBaseAction  //## Inherits: <unnamed>%3963581702DF
{
  //## begin ClUpdateConfigAction%396358090068.initialDeclarations preserve=yes
  //## end ClUpdateConfigAction%396358090068.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUpdateConfigAction%962811787
      ClUpdateConfigAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962811788
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClUpdateConfigAction%396358090068.public preserve=yes
      //## end ClUpdateConfigAction%396358090068.public

  protected:
    // Additional Protected Declarations
      //## begin ClUpdateConfigAction%396358090068.protected preserve=yes
      //## end ClUpdateConfigAction%396358090068.protected

  private:
    // Additional Private Declarations
      //## begin ClUpdateConfigAction%396358090068.private preserve=yes
      //## end ClUpdateConfigAction%396358090068.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUpdateConfigAction%396358090068.implementation preserve=yes
      //## end ClUpdateConfigAction%396358090068.implementation

};

//## begin ClUpdateConfigAction%396358090068.postscript preserve=yes
//## end ClUpdateConfigAction%396358090068.postscript

//## begin ClPostReadXMLAction%3947BD14001B.preface preserve=yes
//## end ClPostReadXMLAction%3947BD14001B.preface

//## Class: ClPostReadXMLAction%3947BD14001B
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostReadXMLAction : public ClBaseAction  //## Inherits: <unnamed>%3948825B037F
{
  //## begin ClPostReadXMLAction%3947BD14001B.initialDeclarations preserve=yes
  //## end ClPostReadXMLAction%3947BD14001B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostReadXMLAction%961054341
      ClPostReadXMLAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054342
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostReadXMLAction%3947BD14001B.public preserve=yes
      //## end ClPostReadXMLAction%3947BD14001B.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostReadXMLAction%3947BD14001B.protected preserve=yes
      //## end ClPostReadXMLAction%3947BD14001B.protected

  private:
    // Additional Private Declarations
      //## begin ClPostReadXMLAction%3947BD14001B.private preserve=yes
      //## end ClPostReadXMLAction%3947BD14001B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostReadXMLAction%3947BD14001B.implementation preserve=yes
      //## end ClPostReadXMLAction%3947BD14001B.implementation

};

//## begin ClPostReadXMLAction%3947BD14001B.postscript preserve=yes
//## end ClPostReadXMLAction%3947BD14001B.postscript

//## begin ClPostAcceptPDUAction%3947BD2301B8.preface preserve=yes
//## end ClPostAcceptPDUAction%3947BD2301B8.preface

//## Class: ClPostAcceptPDUAction%3947BD2301B8
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostAcceptPDUAction : public ClBaseAction  //## Inherits: <unnamed>%3948826201E5
{
  //## begin ClPostAcceptPDUAction%3947BD2301B8.initialDeclarations preserve=yes
  //## end ClPostAcceptPDUAction%3947BD2301B8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostAcceptPDUAction%961054343
      ClPostAcceptPDUAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054344
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostAcceptPDUAction%3947BD2301B8.public preserve=yes
      //## end ClPostAcceptPDUAction%3947BD2301B8.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostAcceptPDUAction%3947BD2301B8.protected preserve=yes
      //## end ClPostAcceptPDUAction%3947BD2301B8.protected

  private:
    // Additional Private Declarations
      //## begin ClPostAcceptPDUAction%3947BD2301B8.private preserve=yes
      //## end ClPostAcceptPDUAction%3947BD2301B8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostAcceptPDUAction%3947BD2301B8.implementation preserve=yes
      //## end ClPostAcceptPDUAction%3947BD2301B8.implementation

};

//## begin ClPostAcceptPDUAction%3947BD2301B8.postscript preserve=yes
//## end ClPostAcceptPDUAction%3947BD2301B8.postscript

//## begin ClPostAcceptXMLAction%3947BD330391.preface preserve=yes
//## end ClPostAcceptXMLAction%3947BD330391.preface

//## Class: ClPostAcceptXMLAction%3947BD330391
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostAcceptXMLAction : public ClBaseAction  //## Inherits: <unnamed>%3948826501C1
{
  //## begin ClPostAcceptXMLAction%3947BD330391.initialDeclarations preserve=yes
  //## end ClPostAcceptXMLAction%3947BD330391.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostAcceptXMLAction%961054345
      ClPostAcceptXMLAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054346
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostAcceptXMLAction%3947BD330391.public preserve=yes
      //## end ClPostAcceptXMLAction%3947BD330391.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostAcceptXMLAction%3947BD330391.protected preserve=yes
      //## end ClPostAcceptXMLAction%3947BD330391.protected

  private:
    // Additional Private Declarations
      //## begin ClPostAcceptXMLAction%3947BD330391.private preserve=yes
      //## end ClPostAcceptXMLAction%3947BD330391.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostAcceptXMLAction%3947BD330391.implementation preserve=yes
      //## end ClPostAcceptXMLAction%3947BD330391.implementation

};

//## begin ClPostAcceptXMLAction%3947BD330391.postscript preserve=yes
//## end ClPostAcceptXMLAction%3947BD330391.postscript

//## begin ClPostWritePDUAction%3947BD460013.preface preserve=yes
//## end ClPostWritePDUAction%3947BD460013.preface

//## Class: ClPostWritePDUAction%3947BD460013
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostWritePDUAction : public ClBaseAction  //## Inherits: <unnamed>%3948826A0254
{
  //## begin ClPostWritePDUAction%3947BD460013.initialDeclarations preserve=yes
  //## end ClPostWritePDUAction%3947BD460013.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostWritePDUAction%961054347
      ClPostWritePDUAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054348
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostWritePDUAction%3947BD460013.public preserve=yes
      //## end ClPostWritePDUAction%3947BD460013.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostWritePDUAction%3947BD460013.protected preserve=yes
      //## end ClPostWritePDUAction%3947BD460013.protected

  private:
    // Additional Private Declarations
      //## begin ClPostWritePDUAction%3947BD460013.private preserve=yes
      //## end ClPostWritePDUAction%3947BD460013.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostWritePDUAction%3947BD460013.implementation preserve=yes
      //## end ClPostWritePDUAction%3947BD460013.implementation

};

//## begin ClPostWritePDUAction%3947BD460013.postscript preserve=yes
//## end ClPostWritePDUAction%3947BD460013.postscript

//## begin ClPostWriteXMLAction%3947BD5D02B5.preface preserve=yes
//## end ClPostWriteXMLAction%3947BD5D02B5.preface

//## Class: ClPostWriteXMLAction%3947BD5D02B5
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPostWriteXMLAction : public ClBaseAction  //## Inherits: <unnamed>%3948826D0209
{
  //## begin ClPostWriteXMLAction%3947BD5D02B5.initialDeclarations preserve=yes
  //## end ClPostWriteXMLAction%3947BD5D02B5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPostWriteXMLAction%961054349
      ClPostWriteXMLAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054350
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClPostWriteXMLAction%3947BD5D02B5.public preserve=yes
      //## end ClPostWriteXMLAction%3947BD5D02B5.public

  protected:
    // Additional Protected Declarations
      //## begin ClPostWriteXMLAction%3947BD5D02B5.protected preserve=yes
      //## end ClPostWriteXMLAction%3947BD5D02B5.protected

  private:
    // Additional Private Declarations
      //## begin ClPostWriteXMLAction%3947BD5D02B5.private preserve=yes
      //## end ClPostWriteXMLAction%3947BD5D02B5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPostWriteXMLAction%3947BD5D02B5.implementation preserve=yes
      //## end ClPostWriteXMLAction%3947BD5D02B5.implementation

};

//## begin ClPostWriteXMLAction%3947BD5D02B5.postscript preserve=yes
//## end ClPostWriteXMLAction%3947BD5D02B5.postscript

//## begin ClParseXMLAction%3948ACCE000A.preface preserve=yes
//## end ClParseXMLAction%3948ACCE000A.preface

class ClDspAuthenticatedAction : public ClBaseAction  //## Inherits: <unnamed>%3948ACE00150
{

  public:
    //## Constructors (specified)
      ClDspAuthenticatedAction (ClBaseActionExecutive *ai_poExecutive);

  protected:
	  // checks if we are in authenticated mode
	  m4bool_t m_bAutoClose;
	  m4bool_t m_bHaveToAuthenticate;
	  m4return_t CheckAuthentication();
	  m4return_t ReplyAuthError(m4uint32_t ai_iConnectionId);
};

//## Class: ClParseXMLAction%3948ACCE000A
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClParseXMLAction : public ClDspAuthenticatedAction  //## Inherits: <unnamed>%3948ACE00150
{
  //## begin ClParseXMLAction%3948ACCE000A.initialDeclarations preserve=yes
  //## end ClParseXMLAction%3948ACCE000A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClParseXMLAction%961054355
      ClParseXMLAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054351
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClParseXMLAction%3948ACCE000A.public preserve=yes
      //## end ClParseXMLAction%3948ACCE000A.public

  protected:
    // Additional Protected Declarations
      //## begin ClParseXMLAction%3948ACCE000A.protected preserve=yes
      //## end ClParseXMLAction%3948ACCE000A.protected

  private:
    // Additional Private Declarations
      //## begin ClParseXMLAction%3948ACCE000A.private preserve=yes
      //## end ClParseXMLAction%3948ACCE000A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClParseXMLAction%3948ACCE000A.implementation preserve=yes
      //## end ClParseXMLAction%3948ACCE000A.implementation

};

//## begin ClParseXMLAction%3948ACCE000A.postscript preserve=yes
//## end ClParseXMLAction%3948ACCE000A.postscript

//## begin ClProcessPDUAction%3948AD2D038C.preface preserve=yes
//## end ClProcessPDUAction%3948AD2D038C.preface

//## Class: ClProcessPDUAction%3948AD2D038C
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClProcessPDUAction : public ClBaseAction  //## Inherits: <unnamed>%3948AD3A01B4
{
  //## begin ClProcessPDUAction%3948AD2D038C.initialDeclarations preserve=yes
  //## end ClProcessPDUAction%3948AD2D038C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClProcessPDUAction%961054353
      ClProcessPDUAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%961054354
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClProcessPDUAction%3948AD2D038C.public preserve=yes
      //## end ClProcessPDUAction%3948AD2D038C.public

  protected:
    // Additional Protected Declarations
      //## begin ClProcessPDUAction%3948AD2D038C.protected preserve=yes
      //## end ClProcessPDUAction%3948AD2D038C.protected

  private:
    // Additional Private Declarations
      //## begin ClProcessPDUAction%3948AD2D038C.private preserve=yes
      //## end ClProcessPDUAction%3948AD2D038C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProcessPDUAction%3948AD2D038C.implementation preserve=yes
      //## end ClProcessPDUAction%3948AD2D038C.implementation

};

//## begin ClProcessPDUAction%3948AD2D038C.postscript preserve=yes
//## end ClProcessPDUAction%3948AD2D038C.postscript

//## begin ClSendPDUAction%395876230321.preface preserve=yes
//## end ClSendPDUAction%395876230321.preface

//## Class: ClSendPDUAction%395876230321
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSendPDUAction : public ClBaseAction  //## Inherits: <unnamed>%3958766300AC
{
  //## begin ClSendPDUAction%395876230321.initialDeclarations preserve=yes
  //## end ClSendPDUAction%395876230321.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSendPDUAction%962094000
      ClSendPDUAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962094001
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClSendPDUAction%395876230321.public preserve=yes
      //## end ClSendPDUAction%395876230321.public

  protected:
    // Additional Protected Declarations
      //## begin ClSendPDUAction%395876230321.protected preserve=yes
      //## end ClSendPDUAction%395876230321.protected

  private:
    // Additional Private Declarations
      //## begin ClSendPDUAction%395876230321.private preserve=yes
      //## end ClSendPDUAction%395876230321.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSendPDUAction%395876230321.implementation preserve=yes
      //## end ClSendPDUAction%395876230321.implementation

};

//## begin ClSendPDUAction%395876230321.postscript preserve=yes
//## end ClSendPDUAction%395876230321.postscript

//## begin ClSendXMLAction%3958764E00FC.preface preserve=yes
//## end ClSendXMLAction%3958764E00FC.preface

//## Class: ClSendXMLAction%3958764E00FC
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSendXMLAction : public ClBaseAction  //## Inherits: <unnamed>%39587665022C
{
  //## begin ClSendXMLAction%3958764E00FC.initialDeclarations preserve=yes
  //## end ClSendXMLAction%3958764E00FC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSendXMLAction%962094002
      ClSendXMLAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962094003
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClSendXMLAction%3958764E00FC.public preserve=yes
      //## end ClSendXMLAction%3958764E00FC.public

  protected:
    // Additional Protected Declarations
      //## begin ClSendXMLAction%3958764E00FC.protected preserve=yes
      //## end ClSendXMLAction%3958764E00FC.protected

  private:
    // Additional Private Declarations
      //## begin ClSendXMLAction%3958764E00FC.private preserve=yes
      //## end ClSendXMLAction%3958764E00FC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSendXMLAction%3958764E00FC.implementation preserve=yes
      //## end ClSendXMLAction%3958764E00FC.implementation

};

//## begin ClSendXMLAction%3958764E00FC.postscript preserve=yes
//## end ClSendXMLAction%3958764E00FC.postscript

//## begin ClSendXMLToAppCtl%396C38470194.preface preserve=yes
//## end ClSendXMLToAppCtl%396C38470194.preface

//## Class: ClSendXMLToAppCtl%396C38470194
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSendXMLToAppCtl : public ClBaseAction  //## Inherits: <unnamed>%396C385A0065
{
  //## begin ClSendXMLToAppCtl%396C38470194.initialDeclarations preserve=yes
  //## end ClSendXMLToAppCtl%396C38470194.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSendXMLToAppCtl%963388997
      ClSendXMLToAppCtl (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%963388998
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClSendXMLToAppCtl%396C38470194.public preserve=yes
      //## end ClSendXMLToAppCtl%396C38470194.public

  protected:
    // Additional Protected Declarations
      //## begin ClSendXMLToAppCtl%396C38470194.protected preserve=yes
      //## end ClSendXMLToAppCtl%396C38470194.protected

  private:
    // Additional Private Declarations
      //## begin ClSendXMLToAppCtl%396C38470194.private preserve=yes
      //## end ClSendXMLToAppCtl%396C38470194.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSendXMLToAppCtl%396C38470194.implementation preserve=yes
      //## end ClSendXMLToAppCtl%396C38470194.implementation

};

//## begin ClSendXMLToAppCtl%396C38470194.postscript preserve=yes
//## end ClSendXMLToAppCtl%396C38470194.postscript

//## begin ClConfigureComms%39FE9E5703AE.preface preserve=yes
//## end ClConfigureComms%39FE9E5703AE.preface

//## Class: ClConfigureComms%39FE9E5703AE
//## Category: M4Dispatcher::Actions::Comm Actions%39F442740341
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConfigureComms : public ClBaseAction  //## Inherits: <unnamed>%39FE9E6A0062
{
  //## begin ClConfigureComms%39FE9E5703AE.initialDeclarations preserve=yes
  //## end ClConfigureComms%39FE9E5703AE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConfigureComms%972986735
      ClConfigureComms (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%972986736
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClConfigureComms%39FE9E5703AE.public preserve=yes
      //## end ClConfigureComms%39FE9E5703AE.public

  protected:
    // Additional Protected Declarations
      //## begin ClConfigureComms%39FE9E5703AE.protected preserve=yes
      //## end ClConfigureComms%39FE9E5703AE.protected

  private:
    // Additional Private Declarations
      //## begin ClConfigureComms%39FE9E5703AE.private preserve=yes
      //## end ClConfigureComms%39FE9E5703AE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConfigureComms%39FE9E5703AE.implementation preserve=yes
      //## end ClConfigureComms%39FE9E5703AE.implementation

};

//## begin ClConfigureComms%39FE9E5703AE.postscript preserve=yes
//## end ClConfigureComms%39FE9E5703AE.postscript

//## begin ClAppCtlReplyAction%3A810C29038C.preface preserve=yes
//## end ClAppCtlReplyAction%3A810C29038C.preface

//## Class: ClAppCtlReplyAction%3A810C29038C
//## Category: M4Dispatcher::Actions::Admin Actions%39F445BA036E
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAppCtlReplyAction : public ClBaseAction  //## Inherits: <unnamed>%3A810C420283
{
  //## begin ClAppCtlReplyAction%3A810C29038C.initialDeclarations preserve=yes
  //## end ClAppCtlReplyAction%3A810C29038C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppCtlReplyAction%981535907
      ClAppCtlReplyAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%981535908
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClAppCtlReplyAction%3A810C29038C.public preserve=yes
      //## end ClAppCtlReplyAction%3A810C29038C.public

  protected:
    // Additional Protected Declarations
      //## begin ClAppCtlReplyAction%3A810C29038C.protected preserve=yes
      //## end ClAppCtlReplyAction%3A810C29038C.protected

  private:
    // Additional Private Declarations
      //## begin ClAppCtlReplyAction%3A810C29038C.private preserve=yes
      //## end ClAppCtlReplyAction%3A810C29038C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppCtlReplyAction%3A810C29038C.implementation preserve=yes
      //## end ClAppCtlReplyAction%3A810C29038C.implementation

};

//## begin ClAppCtlReplyAction%3A810C29038C.postscript preserve=yes
//## end ClAppCtlReplyAction%3A810C29038C.postscript

// Class ClPostReadPDUAction 

// Class ClCloseConnectionAction 

// Class ClUpdateConfigAction 

// Class ClPostReadXMLAction 

// Class ClPostAcceptPDUAction 

// Class ClPostAcceptXMLAction 

// Class ClPostWritePDUAction 

// Class ClPostWriteXMLAction 

// Class ClParseXMLAction 

// Class ClProcessPDUAction 

// Class ClSendPDUAction 

// Class ClSendXMLAction 

// Class ClSendXMLToAppCtl 

// Class ClConfigureComms 

// Class ClAppCtlReplyAction 

//## begin module%3948844C0316.epilog preserve=yes
//## end module%3948844C0316.epilog


#endif
