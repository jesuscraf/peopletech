//## begin module%37E7B051039F.cm preserve=no
//## end module%37E7B051039F.cm

//## begin module%37E7B051039F.cp preserve=no
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
//## end module%37E7B051039F.cp

//## Module: clmemorylevelss%37E7B051039F; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clmemorylevelss.hpp

#ifndef clmemorylevelss_h
#define clmemorylevelss_h 1

//## begin module%37E7B051039F.additionalIncludes preserve=no
//## end module%37E7B051039F.additionalIncludes

//## begin module%37E7B051039F.includes preserve=yes
#define		M4_MATCH_LEVEL		0
#define		M4_BELLOW_LEVEL		1
#define		M4_ABOVE_LEVEL		2
//## end module%37E7B051039F.includes

// clssimplementacion
#include "m4stl.hpp"
#include <clssimplementacion.hpp>
//## begin module%37E7B051039F.declarations preserve=no
//## end module%37E7B051039F.declarations

//## begin module%37E7B051039F.additionalDeclarations preserve=yes
//## end module%37E7B051039F.additionalDeclarations


//## begin eMemoryColor%37E7AD33029D.preface preserve=yes
//## end eMemoryColor%37E7AD33029D.preface

//## Class: eMemoryColor%37E7AD33029D
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {MEMORY_LEVEL_GREEN = 0 , MEMORY_LEVEL_YELLOW , MEMORY_LEVEL_RED , MEMORY_LEVEL_DARK_BLACK} eMemoryColor;

//## begin eMemoryColor%37E7AD33029D.postscript preserve=yes
//## end eMemoryColor%37E7AD33029D.postscript

//## begin eMemoryPolicy%37E7BA0C01B6.preface preserve=yes
//## end eMemoryPolicy%37E7BA0C01B6.preface

//## Class: eMemoryPolicy%37E7BA0C01B6
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum { ABS_MEMORY = 0 , PERCENT_MEMORY , ANY_MEMORY } eMemoryPolicy;

//## begin eMemoryPolicy%37E7BA0C01B6.postscript preserve=yes
//## end eMemoryPolicy%37E7BA0C01B6.postscript

//## begin ClMemoryLevelSS%37E7B51C0303.preface preserve=yes
//## end ClMemoryLevelSS%37E7B51C0303.preface

//## Class: ClMemoryLevelSS%37E7B51C0303
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E7BAB101D2;eMemoryPolicy { -> }
//## Uses: <unnamed>%37E7ADD902BA;eMemoryColor { -> }

class ClMemoryLevelSS : public ClSSImplementation  //## Inherits: <unnamed>%37E7B5610082
{
  //## begin ClMemoryLevelSS%37E7B51C0303.initialDeclarations preserve=yes
  //## end ClMemoryLevelSS%37E7B51C0303.initialDeclarations

  public:
    //## begin ClMemoryLevelSS::eMemoryLevelCommand%37EB5A50027C.preface preserve=yes
    //## end ClMemoryLevelSS::eMemoryLevelCommand%37EB5A50027C.preface

    //## Class: eMemoryLevelCommand%37EB5A50027C
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { IS_MATCHING_LEVEL = ClSSImplementation::TOTAL_COMMANDS , GET_LEVEL , CHANGE_LEVEL , GET_LEVEL_CONFIG , TOTAL_COMMANDS } eMemoryLevelCommand;

    //## begin ClMemoryLevelSS::eMemoryLevelCommand%37EB5A50027C.postscript preserve=yes
    //## end ClMemoryLevelSS::eMemoryLevelCommand%37EB5A50027C.postscript

    //## begin ClMemoryLevelSS::eMemoryLevelAttributes%38318990013A.preface preserve=yes
    //## end ClMemoryLevelSS::eMemoryLevelAttributes%38318990013A.preface

    //## Class: eMemoryLevelAttributes%38318990013A
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {ID_COLOR= ClSSImplementation::TOTAL_ATTRIBS  ,  ID_ABS_HIGH_MEMORY , ID_ABS_LOW_MEMORY , ID_PERCENT_HIGH_MEMORY ,  ID_PERCENT_LOW_MEMORY, TOTAL_ATTRIBS } eMemoryLevelAttributes;

    //## begin ClMemoryLevelSS::eMemoryLevelAttributes%38318990013A.postscript preserve=yes
    //## end ClMemoryLevelSS::eMemoryLevelAttributes%38318990013A.postscript

    //## Constructors (specified)
      //## Operation: ClMemoryLevelSS%937897778
      ClMemoryLevelSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClMemoryLevelSS%937897779
      ~ClMemoryLevelSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%937897780
      //	A virtual method that only calls to the derived class from ClSSStats
      //	Container Initialize method
      virtual m4return_t Initialize ();

      //## Operation: IsMachingLevel%937897781
      m4return_t IsMachingLevel (m4uint32_t ai_iAbsMemory, m4uint32_t ai_iPercentMemory, eMemoryPolicy ai_iPolicy);

      //## Operation: ExecuteCommand%938162700
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%938162701
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%938162702
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: CheckInitialConfiguration%941621153
      void CheckInitialConfiguration ();

      //## Operation: GetSubsystemAttribsId%942768436
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (M4DataStorage *ao_poDataStorage);

  public:
    // Additional Public Declarations
      //## begin ClMemoryLevelSS%37E7B51C0303.public preserve=yes
      //## end ClMemoryLevelSS%37E7B51C0303.public

  protected:
    // Additional Protected Declarations
      //## begin ClMemoryLevelSS%37E7B51C0303.protected preserve=yes
		m4return_t   _ChangeValue ( eMemoryPolicy policy , m4uint32_t iLow , m4uint32_t iHigh ) ;
      //## end ClMemoryLevelSS%37E7B51C0303.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iColor%37E7B5C20027
      //## begin ClMemoryLevelSS::m_iColor%37E7B5C20027.attr preserve=no  private: eMemoryColor {UA} 
      eMemoryColor m_iColor;
      //## end ClMemoryLevelSS::m_iColor%37E7B5C20027.attr

      //## Attribute: m_iLowAbsMemory%37E7B79E019E
      //## begin ClMemoryLevelSS::m_iLowAbsMemory%37E7B79E019E.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iLowAbsMemory;
      //## end ClMemoryLevelSS::m_iLowAbsMemory%37E7B79E019E.attr

      //## Attribute: m_iHighAbsMemory%37E7B847022D
      //## begin ClMemoryLevelSS::m_iHighAbsMemory%37E7B847022D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iHighAbsMemory;
      //## end ClMemoryLevelSS::m_iHighAbsMemory%37E7B847022D.attr

      //## Attribute: m_iLowPercentMemory%37E7B85F012D
      //## begin ClMemoryLevelSS::m_iLowPercentMemory%37E7B85F012D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iLowPercentMemory;
      //## end ClMemoryLevelSS::m_iLowPercentMemory%37E7B85F012D.attr

      //## Attribute: m_iHighPercentMemory%37E7B8870117
      //## begin ClMemoryLevelSS::m_iHighPercentMemory%37E7B8870117.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iHighPercentMemory;
      //## end ClMemoryLevelSS::m_iHighPercentMemory%37E7B8870117.attr

    // Additional Private Declarations
      //## begin ClMemoryLevelSS%37E7B51C0303.private preserve=yes
      //## end ClMemoryLevelSS%37E7B51C0303.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMemoryLevelSS%37E7B51C0303.implementation preserve=yes
      //## end ClMemoryLevelSS%37E7B51C0303.implementation

};

//## begin ClMemoryLevelSS%37E7B51C0303.postscript preserve=yes
//## end ClMemoryLevelSS%37E7B51C0303.postscript

// Class ClMemoryLevelSS 

//## begin module%37E7B051039F.epilog preserve=yes
//## end module%37E7B051039F.epilog

typedef enum {	MAX_ABS_LOW_GREEN_MEMORY = 0 , MIN_ABS_LOW_GREEN_MEMORY, MAX_ABS_LOW_YELLOW_MEMORY, MIN_ABS_LOW_YELLOW_MEMORY, MAX_ABS_LOW_RED_MEMORY, MIN_ABS_LOW_RED_MEMORY, \
				MAX_ABS_HIGH_GREEN_MEMORY, MIN_ABS_HIGH_GREEN_MEMORY, MAX_ABS_HIGH_YELLOW_MEMORY, MIN_ABS_HIGH_YELLOW_MEMORY, MAX_ABS_HIGH_RED_MEMORY, MIN_ABS_HIGH_RED_MEMORY, ENUM_NUM_ITEMS } eMemoryLimit;

class ClAbsoluteMemoryLimits
{
    public:
        
		~ClAbsoluteMemoryLimits();

		static ClAbsoluteMemoryLimits& getInstance();
		
		int operator[]( int );

	private:

		vector< int >*	m_pvMemoryLimits;

    private:

        ClAbsoluteMemoryLimits();

		ClAbsoluteMemoryLimits( ClAbsoluteMemoryLimits const& );	// Not implemented. Avoid more than 1 intance.

        void operator=( ClAbsoluteMemoryLimits const& );			// Not implemented. Avoid more than 1 intance.
		
		m4return_t	Initialize();

		int	getCeiledPercent( int ai_iPercent, int ai_iMaxValue );
};

#endif
