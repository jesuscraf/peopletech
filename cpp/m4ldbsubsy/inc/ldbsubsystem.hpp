//## begin module%34CE2CC40182.cm preserve=no
//## end module%34CE2CC40182.cm

//## begin module%34CE2CC40182.cp preserve=no
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
//## end module%34CE2CC40182.cp

//## Module: LDBSubsystem%34CE2CC40182; Package specification
//## Subsystem: M4LDBSubsystem::inc%379D76DE0185
//## Source file: b:\M4LDBSubsy\inc\ldbsubsystem.hpp

#ifndef ldbsubsystem_h
#define ldbsubsystem_h 1

//## begin module%34CE2CC40182.additionalIncludes preserve=no
//## end module%34CE2CC40182.additionalIncludes

//## begin module%34CE2CC40182.includes preserve=yes

// LDBSubsystem properties

#define	LDB_CONNECTION_STRING			"CONNECTION_STRING"
#define	LDB_MAX_NUM_CONN				"MAX_NUM_CONN"
#define	LDB_MAX_NUM_CURSOR				"MAX_NUM_CURSOR"
#define	LDB_CON_EXEC					"CON_EXEC"
#define	LDB_MAX_NUM_STATMNT_CACHE		"MAX_NUM_STATMNT_CACHE"
#define	LDB_MAX_NUM_LOGIC_OBJECT		"MAX_NUM_LOGIC_OBJECT"
#define	LDB_ISOLATION_LEVEL				"ISOLATION_LEVEL"
#define	LDB_DATABASE_TYPE				"DATABASE_TYPE"
#define	LDB_NUM_CONN_IN_USE				"NUM_CONN_IN_USE"
#define	LDB_NUM_LOGIC_OBJECT_IN_USE		"NUM_LOGIC_OBJECT_IN_USE"
#define	LDB_NUM_STATMNT_IN_USE			"NUM_STATMNT_IN_USE"
#define LDB_DEBUG_LEVEL					"LDB_DEBUG_LEVEL"
#define LDB_DEBUG_DETAIL_LEVEL			"LDB_DEBUG_DETAIL_LEVEL"

#define LDB_ATTEMPTS_NUMBER				"ATTEMPTS_NUMBER"

#define ACTION_FLUSH_DICTIONARY_CACHE	"ACTION_FLUSH_DICTIONARY_CACHE"
#define LAST_FLUSH_DICTIONARY_CACHE		"LAST_FLUSH_DICTIONARY_CACHE"

//## end module%34CE2CC40182.includes

#include "m4stl.hpp"
// Statistics
#include <statistics.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// m4event
#include <m4event.hpp>
// m4notifier
#include <m4notifier.hpp>
#include "m4ldbsubsy_dll.hpp"
//## begin module%34CE2CC40182.declarations preserve=no
//## end module%34CE2CC40182.declarations




//## begin ClLDBVisitor%34CE2A7B02CC.preface preserve=yes
//## end ClLDBVisitor%34CE2A7B02CC.preface

//## Class: ClLDBVisitor%34CE2A7B02CC
//## Category: M4LDBSubsystem%3518E38A029F
//## Subsystem: M4LDBSubsystem::inc%379D76DE0185
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClLDBVisitor : public ClVisitor  //## Inherits: <unnamed>%34CE2AA700F8
{
  //## begin ClLDBVisitor%34CE2A7B02CC.initialDeclarations preserve=yes
  //## end ClLDBVisitor%34CE2A7B02CC.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitLDB%885927764
      void VisitLDB (void *ai_oLDB);

    // Additional Public Declarations
      //## begin ClLDBVisitor%34CE2A7B02CC.public preserve=yes
      //## end ClLDBVisitor%34CE2A7B02CC.public

  protected:
    // Additional Protected Declarations
      //## begin ClLDBVisitor%34CE2A7B02CC.protected preserve=yes
      //## end ClLDBVisitor%34CE2A7B02CC.protected

  private:
    // Additional Private Declarations
      //## begin ClLDBVisitor%34CE2A7B02CC.private preserve=yes
      //## end ClLDBVisitor%34CE2A7B02CC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLDBVisitor%34CE2A7B02CC.implementation preserve=yes
      //## end ClLDBVisitor%34CE2A7B02CC.implementation

};

//## begin ClLDBVisitor%34CE2A7B02CC.postscript preserve=yes
//## end ClLDBVisitor%34CE2A7B02CC.postscript

//## begin VcClEventIntInt%3921769A0392.preface preserve=yes
//## end VcClEventIntInt%3921769A0392.preface

//## Class: VcClEventIntInt%3921769A0392; Instantiated Class
//## Category: M4LDBSubsystem%3518E38A029F
//## Subsystem: M4LDBSubsystem::inc%379D76DE0185
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3921863101EC;ClEventIntInt { -> }

typedef vector< ClEventIntInt > VcClEventIntInt;

//## begin VcClEventIntInt%3921769A0392.postscript preserve=yes
//## end VcClEventIntInt%3921769A0392.postscript

//## begin ClLDBSubsystem%34CDDADC00B2.preface preserve=yes
//## end ClLDBSubsystem%34CDDADC00B2.preface

//## Class: ClLDBSubsystem%34CDDADC00B2
//	Esta clase  proporciona un API que permite manejar subsistemas de la LDB
//	Las principales fuciones son Initialize, que permite inicializar la LDB, y
//	ShutDown que echa abajo dicha LDB.
//## Category: M4LDBSubsystem%3518E38A029F
//## Subsystem: M4LDBSubsystem::inc%379D76DE0185
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3518CFF802FD; { -> }
//## Uses: <unnamed>%35235A2503E6; { -> }

class M4_DECL_M4LDBSUBSY ClLDBSubsystem : public ClSSImplementation, //## Inherits: <unnamed>%37E0BEFE03AC
    	public ClNotifiable  //## Inherits: <unnamed>%392175B30079
{
  //## begin ClLDBSubsystem%34CDDADC00B2.initialDeclarations preserve=yes
  //## end ClLDBSubsystem%34CDDADC00B2.initialDeclarations

  public:
    //## begin ClLDBSubsystem::eLDBCommandsID%37FE24EF0397.preface preserve=yes
    //## end ClLDBSubsystem::eLDBCommandsID%37FE24EF0397.preface

    //## Class: eLDBCommandsID%37FE24EF0397
    //## Category: M4LDBSubsystem%3518E38A029F
    //## Subsystem: M4LDBSubsystem%3518E0C500DD
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { SET_DEBUG_LEVEL = ClSSImplementation :: TOTAL_COMMANDS, SET_DEBUG_DETAIL_LEVEL, FLUSH_SENTENCE_CACHE, FLUSH_DICTIONARY_CACHE, RETRY_CONNECTION} eLDBCommandsID;

    //## begin ClLDBSubsystem::eLDBCommandsID%37FE24EF0397.postscript preserve=yes
    //## end ClLDBSubsystem::eLDBCommandsID%37FE24EF0397.postscript

    //## begin ClLDBSubsystem::eLDBParamsID%37FE26110019.preface preserve=yes
    //## end ClLDBSubsystem::eLDBParamsID%37FE26110019.preface

    //## Class: eLDBParamsID%37FE26110019
    //## Category: M4LDBSubsystem%3518E38A029F
    //## Subsystem: M4LDBSubsystem%3518E0C500DD
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { DEBUG_LEVEL, DEBUG_DETAIL_LEVEL } eLDBParamsID;

    //## begin ClLDBSubsystem::eLDBParamsID%37FE26110019.postscript preserve=yes
    //## end ClLDBSubsystem::eLDBParamsID%37FE26110019.postscript

    //## Constructors (specified)
      //## Operation: ClLDBSubsystem%885977163
      ClLDBSubsystem (ClConfiguration *ai_poObl, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClLDBSubsystem%885977164
      ~ClLDBSubsystem ();


    //## Other Operations (specified)
      //## Operation: ExecuteCommand%885927759
      //	Este método  permitirá en el futuro comandos de la  LDB.Le pasamos el
      //	comando que se habrá codificado mediante un char,los parámetros de entrad
      //	para el comando y un parámetro donde se devuelve el resultado de ejecutar el
      //	comando.
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Synchronize%885927760
      m4return_t Synchronize ();

      //## Operation: GetCommandsId%885927761
      //	Método que permite  obtener los identificadores del subsistema  que  nos
      //	servirán  para ejecutar ese comando posteriormente con la función Execute
      //	Command.
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: Initialize%885927762
      //	Esta función permite inicializar la LDB  . Si no es capaz de realizar la
      //	inicialización correctamente  se devuelve un M4_ERROR , en otro caso se
      //	devuelve un M4_SUCCESS.
      m4return_t Initialize ();

      //## Operation: ShutDown%885927763
      //	Finaliza la LDB . Si no puede finalizarse con éxito  se devuelve un M4_ERROR
      //	en otro caso se devuelve un M4_SUCCESS.
      m4return_t ShutDown ();

      //## Operation: GetCommandParams%891421985
      //	Esta función devuelve los parámetros necesarios para ejecutar  un comando
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: Update%919863071
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: GetSubsystemAttribValue%941557940
      m4return_t GetSubsystemAttribValue (m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage);

      //## Operation: Notify%958495556
      m4bool_t Notify (ClEvent &ai_oEvent);

  public:
    // Additional Public Declarations
      //## begin ClLDBSubsystem%34CDDADC00B2.public preserve=yes
      //## end ClLDBSubsystem%34CDDADC00B2.public

  protected:

    //## Other Operations (specified)
      //## Operation: RetryConnection%958495555
      m4return_t RetryConnection ();

    // Additional Protected Declarations
      //## begin ClLDBSubsystem%34CDDADC00B2.protected preserve=yes
      //## end ClLDBSubsystem%34CDDADC00B2.protected

  private:

    //## Other Operations (specified)
      //## Operation: InitializeAttributes%939655437
      m4return_t InitializeAttributes ();

    // Data Members for Class Attributes

      //## Attribute: m_nRetry%39218121001C
      //## begin ClLDBSubsystem::m_nRetry%39218121001C.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_nRetry;
      //## end ClLDBSubsystem::m_nRetry%39218121001C.attr

    // Data Members for Associations

      //## Association: M4LDBSubsystem::m_vcRetry%392180880347
      //## Role: ClLDBSubsystem::m_vcEvent%3921808A01E1
      //## begin ClLDBSubsystem::m_vcEvent%3921808A01E1.role preserve=no  private: VcClEventIntInt { -> VHAN}
      VcClEventIntInt m_vcEvent;
      //## end ClLDBSubsystem::m_vcEvent%3921808A01E1.role

    // Additional Private Declarations
      //## begin ClLDBSubsystem%34CDDADC00B2.private preserve=yes
      //## end ClLDBSubsystem%34CDDADC00B2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLDBSubsystem%34CDDADC00B2.implementation preserve=yes
      //## end ClLDBSubsystem%34CDDADC00B2.implementation

};

//## begin ClLDBSubsystem%34CDDADC00B2.postscript preserve=yes
//## end ClLDBSubsystem%34CDDADC00B2.postscript

// Class ClLDBVisitor 

// Class ClLDBSubsystem 

//## begin module%34CE2CC40182.epilog preserve=yes
//## end module%34CE2CC40182.epilog


#endif
