//## begin module%3438FAEF0173.cm preserve=no
//## end module%3438FAEF0173.cm

//## begin module%3438FAEF0173.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExecutiveTest.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
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
//## end module%3438FAEF0173.cp

//## Module: Executive_Test%3438FAEF0173; Package specification
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Source file: d:\eduardoma\M4ExeFactory\version\inc\exefortest.hpp

#ifndef executive_test_h
#define executive_test_h 1

//## begin module%3438FAEF0173.additionalIncludes preserve=yes
//## end module%3438FAEF0173.additionalIncludes

//## begin module%3438FAEF0173.includes preserve=yes
#include <m4stl.hpp>
#include <clcache.hpp>
//## end module%3438FAEF0173.includes

// Executive
#include <executive.hpp>
//## begin module%3438FAEF0173.declarations preserve=no
//## end module%3438FAEF0173.declarations

//## begin module%3438FAEF0173.additionalDeclarations preserve=yes
//## end module%3438FAEF0173.additionalDeclarations


//## begin ClEngineCacheForTest%3438F90B0089.preface preserve=yes
//## end ClEngineCacheForTest%3438F90B0089.preface

//## Class: ClEngineCacheForTest%3438F90B0089
//## Category: M4ExeFactory::Exec Test%3438F53902CB
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClEngineCacheForTest 
{
  //## begin ClEngineCacheForTest%3438F90B0089.initialDeclarations preserve=yes
  //## end ClEngineCacheForTest%3438F90B0089.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEngineCacheForTest%876139083
      ClEngineCacheForTest (const m4uint16_t ai_uiTimer = 0);


    //## Other Operations (specified)
      //## Operation: GetLauncher%876139084
      m4uint16_t GetLauncher ();

      //## Operation: SetLauncher%876139085; C++
      void SetLauncher (m4uint32_t ai_uiLauncher);

    // Additional Public Declarations
      //## begin ClEngineCacheForTest%3438F90B0089.public preserve=yes
      //## end ClEngineCacheForTest%3438F90B0089.public

  protected:
    // Additional Protected Declarations
      //## begin ClEngineCacheForTest%3438F90B0089.protected preserve=yes
      //## end ClEngineCacheForTest%3438F90B0089.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_uiLauncher%343904850116
      //## begin ClEngineCacheForTest::m_uiLauncher%343904850116.attr preserve=no  private: m4uint32_t {UA} ai_uiTimer
      m4uint32_t m_uiLauncher;
      //## end ClEngineCacheForTest::m_uiLauncher%343904850116.attr

    // Additional Private Declarations
      //## begin ClEngineCacheForTest%3438F90B0089.private preserve=yes
      //## end ClEngineCacheForTest%3438F90B0089.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEngineCacheForTest%3438F90B0089.implementation preserve=yes
      //## end ClEngineCacheForTest%3438F90B0089.implementation

};

//## begin ClEngineCacheForTest%3438F90B0089.postscript preserve=yes
//## end ClEngineCacheForTest%3438F90B0089.postscript

//## begin ClExecutiveForTest%343DDF5E0344.preface preserve=yes
//## end ClExecutiveForTest%343DDF5E0344.preface

//## Class: ClExecutiveForTest%343DDF5E0344
//## Category: M4ExeFactory::Exec Test%3438F53902CB
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClExecutiveForTest : public ClExecutive  //## Inherits: <unnamed>%343DDF6100D3
{
  //## begin ClExecutiveForTest%343DDF5E0344.initialDeclarations preserve=yes
  //## end ClExecutiveForTest%343DDF5E0344.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForTest%876470324; C++
      ClExecutiveForTest (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForTest%884769286; C++
      ~ClExecutiveForTest ();


    //## Other Operations (specified)
      //## Operation: StartUp%876470325
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%876470326
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: ShutDown%876470327
      m4return_t ShutDown ();

      //## Operation: Reset%884854924; C++
      m4return_t Reset ();

      //## Operation: GetNDumpLevels%921077499
      m4int16_t GetNDumpLevels () const;

      //## Operation: DumpLevel%921077498; C++
      m4return_t DumpLevel (const m4int16_t ai_iLevel);

    // Additional Public Declarations
      //## begin ClExecutiveForTest%343DDF5E0344.public preserve=yes
      //## end ClExecutiveForTest%343DDF5E0344.public

  protected:
    // Additional Protected Declarations
      //## begin ClExecutiveForTest%343DDF5E0344.protected preserve=yes
      //## end ClExecutiveForTest%343DDF5E0344.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iDelay%366E3B590335
      //## begin ClExecutiveForTest::m_iDelay%366E3B590335.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iDelay;
      //## end ClExecutiveForTest::m_iDelay%366E3B590335.attr

    // Additional Private Declarations
      //## begin ClExecutiveForTest%343DDF5E0344.private preserve=yes
      //## end ClExecutiveForTest%343DDF5E0344.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForTest%343DDF5E0344.implementation preserve=yes
      //## end ClExecutiveForTest%343DDF5E0344.implementation

};

//## begin ClExecutiveForTest%343DDF5E0344.postscript preserve=yes
//## end ClExecutiveForTest%343DDF5E0344.postscript

//## begin ClEngineForTest%3458584D0314.preface preserve=yes
//## end ClEngineForTest%3458584D0314.preface

//## Class: ClEngineForTest%3458584D0314
//## Category: M4ExeFactory::Exec Test%3438F53902CB
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClEngineForTest : public ClEngine  //## Inherits: <unnamed>%3458585702C6
{
  //## begin ClEngineForTest%3458584D0314.initialDeclarations preserve=yes
  //## end ClEngineForTest%3458584D0314.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEngineForTest%878205110; C++
      ClEngineForTest ();


    //## Other Operations (specified)
      //## Operation: Process%878205111
      bool Process (ClImage &a_oImage);

      //## Operation: Init%893773032
      m4return_t Init ();

      //## Operation: End%893773033
      m4return_t End ();

    // Additional Public Declarations
      //## begin ClEngineForTest%3458584D0314.public preserve=yes
      //## end ClEngineForTest%3458584D0314.public

  protected:
    // Additional Protected Declarations
      //## begin ClEngineForTest%3458584D0314.protected preserve=yes
      //## end ClEngineForTest%3458584D0314.protected

  private:
    // Additional Private Declarations
      //## begin ClEngineForTest%3458584D0314.private preserve=yes
      //## end ClEngineForTest%3458584D0314.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEngineForTest%3458584D0314.implementation preserve=yes
      //## end ClEngineForTest%3458584D0314.implementation

};

//## begin ClEngineForTest%3458584D0314.postscript preserve=yes
//## end ClEngineForTest%3458584D0314.postscript

// Class ClEngineCacheForTest 

// Class ClExecutiveForTest 


//## Other Operations (inline)
inline m4int16_t ClExecutiveForTest::GetNDumpLevels () const
{
  //## begin ClExecutiveForTest::GetNDumpLevels%921077499.body preserve=yes
	return 0;
  //## end ClExecutiveForTest::GetNDumpLevels%921077499.body
}

// Class ClEngineForTest 

//## begin module%3438FAEF0173.epilog preserve=yes
//## end module%3438FAEF0173.epilog


#endif
