//## begin module%37E7AE0B03A2.cm preserve=no
//## end module%37E7AE0B03A2.cm

//## begin module%37E7AE0B03A2.cp preserve=no
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
//## end module%37E7AE0B03A2.cp

//## Module: controlmemoryss%37E7AE0B03A2; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\controlmemoryss.hpp

#ifndef controlmemoryss_h
#define controlmemoryss_h 1

//## begin module%37E7AE0B03A2.additionalIncludes preserve=no
//## end module%37E7AE0B03A2.additionalIncludes

//## begin module%37E7AE0B03A2.includes preserve=yes
//## end module%37E7AE0B03A2.includes

// clmemorylevelss
#include <clmemorylevelss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%37E7AE0B03A2.declarations preserve=no
//## end module%37E7AE0B03A2.declarations

//## begin module%37E7AE0B03A2.additionalDeclarations preserve=yes
//## end module%37E7AE0B03A2.additionalDeclarations


//## begin ClCotrolMemorySS%37E7A9A50213.preface preserve=yes
//## end ClCotrolMemorySS%37E7A9A50213.preface

//## Class: ClCotrolMemorySS%37E7A9A50213
//	Subsistema que tiene por acción la medición de los niveles de memoria del
//	server y de la máquina en la que este se encuentra. Además de determinar a
//	partir de estas mediciones obtenidas el grado de consuno de memoria en el
//	que se encuentra el servidor.
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E7BAC2037B;ClMemoryLevelSS { -> }

class ClCotrolMemorySS : public ClSSImplementation  //## Inherits: <unnamed>%37E7A9C000EF
{
  //## begin ClCotrolMemorySS%37E7A9A50213.initialDeclarations preserve=yes
  //## end ClCotrolMemorySS%37E7A9A50213.initialDeclarations

  public:
    //## begin ClCotrolMemorySS::eMemoryControlCommands%37E8A923000F.preface preserve=yes
    //## end ClCotrolMemorySS::eMemoryControlCommands%37E8A923000F.preface

    //## Class: eMemoryControlCommands%37E8A923000F
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {CHECK_MEMORY = ClSSImplementation::TOTAL_COMMANDS , ACTIVATE_CONTROL_MEMORY , DESACTIVATE_CONTROL_MEMORY , CHANGE_ABS_MEMORY_LEVELS , CHANGE_PERCENT_MEMORY_LEVELS ,  TOTAL_COMMANDS } eMemoryControlCommands;

    //## begin ClCotrolMemorySS::eMemoryControlCommands%37E8A923000F.postscript preserve=yes
    //## end ClCotrolMemorySS::eMemoryControlCommands%37E8A923000F.postscript

    //## Constructors (specified)
      //## Operation: ClCotrolMemorySS%937897772
      ClCotrolMemorySS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClCotrolMemorySS%937897773
      ~ClCotrolMemorySS ();


    //## Other Operations (specified)
      //## Operation: Initialize%937897774
      //	A virtual method that only calls to the derived class from ClSSStats
      //	Container Initialize method
      virtual m4return_t Initialize ();

      //## Operation: Start%937897775
      virtual m4return_t Start ();

      //## Operation: Stop%937897776
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: CheckMemoryLevel%937897777
      //	Realiza l amedición de los parámetros d ememori alcanzados por el proceso y
      //	la máquina en la que este se encuentra y con ellos determinar el nivel de
      //	memoria en el que nos encontramos.
      m4return_t CheckMemoryLevel (eMemoryColor ao_iMemoryColor);

      //## Operation: GenerateSubsystem%937897782
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: ExecuteCommand%937994349
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%937994350
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%937994351
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: ActivateControlMemory%948896969
      m4return_t ActivateControlMemory (m4bool_t ai_bActivate);

  public:
    // Additional Public Declarations
      //## begin ClCotrolMemorySS%37E7A9A50213.public preserve=yes
      //## end ClCotrolMemorySS%37E7A9A50213.public

  protected:
    // Additional Protected Declarations
      //## begin ClCotrolMemorySS%37E7A9A50213.protected preserve=yes
	  m4return_t _ChangeMemoryLevels ( eMemoryPolicy ai_iPolicy , m4uint32_t ai_HighGreenLevel , m4uint32_t ai_iLowYellowLevel , m4uint32_t ai_iHighYlleowLevel , m4uint32_t ai_iLowRedLevel ) ;
	  m4return_t _GetMemoryLevels ( eMemoryPolicy ai_iPolicy , m4uint32_t &ao_HighGreenLevel , m4uint32_t &ao_iLowYellowLevel , m4uint32_t &ao_iHighYlleowLevel , m4uint32_t &ao_iLowRedLevel ) ;
     //## end ClCotrolMemorySS%37E7A9A50213.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iColor%37E8A86500B6
      //## begin ClCotrolMemorySS::m_iColor%37E8A86500B6.attr preserve=no  private: eMemoryColor {UA} 
      eMemoryColor m_iColor;
      //## end ClCotrolMemorySS::m_iColor%37E8A86500B6.attr

      //## Attribute: m_iActualLevel%37EB390302E1
      //## begin ClCotrolMemorySS::m_iActualLevel%37EB390302E1.attr preserve=no  private: INTERFACESMAP::iterator {UA} 
      INTERFACESMAP::iterator m_iActualLevel;
      //## end ClCotrolMemorySS::m_iActualLevel%37EB390302E1.attr

    // Additional Private Declarations
      //## begin ClCotrolMemorySS%37E7A9A50213.private preserve=yes
      //## end ClCotrolMemorySS%37E7A9A50213.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCotrolMemorySS%37E7A9A50213.implementation preserve=yes
      //## end ClCotrolMemorySS%37E7A9A50213.implementation

};

//## begin ClCotrolMemorySS%37E7A9A50213.postscript preserve=yes
//## end ClCotrolMemorySS%37E7A9A50213.postscript

// Class ClCotrolMemorySS 

//## begin module%37E7AE0B03A2.epilog preserve=yes
//## end module%37E7AE0B03A2.epilog


#endif
