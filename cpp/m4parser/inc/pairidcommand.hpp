//## begin module%349BE75A0265.cm preserve=no
//## end module%349BE75A0265.cm

//## begin module%349BE75A0265.cp preserve=no
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
//## end module%349BE75A0265.cp

//## Module: PairIdCommand%349BE75A0265; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\pairidcommand.hpp

#ifndef pairidcommand_h
#define pairidcommand_h 1

//## begin module%349BE75A0265.additionalIncludes preserve=no
//## end module%349BE75A0265.additionalIncludes

#include <m4parser_dll.hpp>
#include <basepdus.hpp>
#include <parserdefs.hpp>
#include <clcommand.hpp>

class ClStatistic;


//## begin module%349BE75A0265.declarations preserve=no
//## end module%349BE75A0265.declarations

//## begin module%349BE75A0265.additionalDeclarations preserve=yes
//## end module%349BE75A0265.additionalDeclarations


//## Class: ClPairIdCommand%3497A34B032D
//	Abstract class used to derive executors which will execute commands analysed
//	by the parser
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B3AAE40014;m4command_t { -> }

class M4_DECL_M4PARSER ClPairIdCommand : public ClCommand  //## Inherits: <unnamed>%3497A38B022B
{
  public:
    //## Constructors (specified)
      //## Operation: ClPairIdCommand%882623309
      ClPairIdCommand (m4command_t ai_pCommandId = NULL, void *ai_pManager = NULL);

    //## Destructor (specified)
      //## Operation: ~ClPairIdCommand%884190881
      ~ClPairIdCommand ();


    //## Other Operations (specified)
      //## Operation: GetId%882623310
      m4command_t GetId ();

      //## Operation: Execute%889434846
      virtual m4return_t Execute (m4parsercommandargs_t ai_pInput, M4DataStorage *aio_pIOputData, ClStatistic *aio_poStats);

      //## Operation: Execute%899143378
      virtual m4return_t Execute (m4parsercommandargs_t ai_pInput, M4DataStorage *aio_pIOputData);

      //## Operation: Execute%933074743
      virtual m4return_t Execute (void*& exit, ClCmdArguments *args = NULL);

      //## Operation: Execute%933074744
      virtual ClPDU* Execute (m4parsercommandargs_t ai_pInput) = 0;

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pCommandId%3498167600B1
      //## begin ClPairIdCommand::m_pCommandId%3498167600B1.attr preserve=no  protected: m4command_t {UA} 
      m4command_t m_pCommandId;
      //## end ClPairIdCommand::m_pCommandId%3498167600B1.attr

  private:
  private: //## implementation
};

// Class ClPairIdCommand 

//## begin module%349BE75A0265.epilog preserve=yes
//## end module%349BE75A0265.epilog


#endif
