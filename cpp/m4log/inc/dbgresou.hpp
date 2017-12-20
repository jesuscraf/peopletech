//## begin module%377C883E00F3.cm preserve=no
//## end module%377C883E00F3.cm

//## begin module%377C883E00F3.cp preserve=no
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
//## end module%377C883E00F3.cp

//## Module: dbgresou%377C883E00F3; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\version\inc\dbgresou.hpp

#ifndef _DBGRESOU_HPP_
#define _DBGRESOU_HPP_ 1

//## begin module%377C883E00F3.additionalIncludes preserve=no
//## end module%377C883E00F3.additionalIncludes

//## begin module%377C883E00F3.includes preserve=yes
#include "m4stl.hpp"
#include "m4string.hpp"
//## end module%377C883E00F3.includes

// m4log
#include <m4log.hpp>

class M4_DECL_M4LOG ClStaticResourceList;
class M4_DECL_M4LOG ClOutputPattern;
class M4_DECL_M4LOG ClModuleInfo;

//## begin module%377C883E00F3.declarations preserve=no
//## end module%377C883E00F3.declarations

//## begin module%377C883E00F3.additionalDeclarations preserve=yes
//## end module%377C883E00F3.additionalDeclarations


//## begin ClModuleInfo%377C883F0164.preface preserve=yes
//## end ClModuleInfo%377C883F0164.preface

//## Class: ClModuleInfo%377C883F0164
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C8840002E;M4ClString { -> }
//## Uses: <unnamed>%377C88400033;m4bool_t { -> }
//## Uses: <unnamed>%377C88400043;m4return_t { -> }
//## Uses: <unnamed>%377C88400044;m4int32_t { -> }

class M4_DECL_M4LOG ClModuleInfo 
{
  //## begin ClModuleInfo%377C883F0164.initialDeclarations preserve=yes
  //## end ClModuleInfo%377C883F0164.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClModuleInfo%-945366519; C++
      ClModuleInfo ();

    //## Destructor (specified)
      //## Operation: ~ClModuleInfo%-1381154582; C++
      ~ClModuleInfo ();


    //## Other Operations (specified)
      //## Operation: operator +=%-929270123; C++
      ClModuleInfo& operator += (const ClModuleInfo& ai_ModuleInfo);

      //## Operation: GetModuleName%343302820; C++
      M4ClString GetModuleName ()
      {
        //## begin ClModuleInfo::GetModuleName%343302820.body preserve=yes

 return m_sModuleName;

        //## end ClModuleInfo::GetModuleName%343302820.body
      }

      //## Operation: GetInitCodeNumber%-2110243151; C++
      m4uint32_t GetInitCodeNumber ()
      {
        //## begin ClModuleInfo::GetInitCodeNumber%-2110243151.body preserve=yes

 return m_InitCode;	

        //## end ClModuleInfo::GetInitCodeNumber%-2110243151.body
      }

      //## Operation: GetFinalCodeNumber%-535392632; C++
      m4uint32_t GetFinalCodeNumber ()
      {
        //## begin ClModuleInfo::GetFinalCodeNumber%-535392632.body preserve=yes

 return m_FinalCode;	

        //## end ClModuleInfo::GetFinalCodeNumber%-535392632.body
      }

      //## Operation: GetModuleState%-1525849202; C++
      m4bool_t GetModuleState ()
      {
        //## begin ClModuleInfo::GetModuleState%-1525849202.body preserve=yes

 return m_ModuleState;	

        //## end ClModuleInfo::GetModuleState%-1525849202.body
      }

      //## Operation: SetModuleName%-1971265633; C++
      void SetModuleName (const M4ClString& ai_ModuleName)
      {
        //## begin ClModuleInfo::SetModuleName%-1971265633.body preserve=yes

 m_sModuleName = ai_ModuleName;

        //## end ClModuleInfo::SetModuleName%-1971265633.body
      }

      //## Operation: SetInitCodeNumber%-659563754; C++
      void SetInitCodeNumber (m4uint32_t ai_InitCode)
      {
        //## begin ClModuleInfo::SetInitCodeNumber%-659563754.body preserve=yes

 m_InitCode	= ai_InitCode;	

        //## end ClModuleInfo::SetInitCodeNumber%-659563754.body
      }

      //## Operation: SetFinalCodeNumber%-1892026359; C++
      void SetFinalCodeNumber (m4uint32_t ai_FinalCode)
      {
        //## begin ClModuleInfo::SetFinalCodeNumber%-1892026359.body preserve=yes

 m_FinalCode	= ai_FinalCode;

        //## end ClModuleInfo::SetFinalCodeNumber%-1892026359.body
      }

      //## Operation: SetModuleState%332347050; C++
      void SetModuleState (m4bool_t ai_State)
      {
        //## begin ClModuleInfo::SetModuleState%332347050.body preserve=yes

 m_ModuleState = ai_State;		

        //## end ClModuleInfo::SetModuleState%332347050.body
      }

      //## Operation: SetOutputPattern%-1699606366; C++
      void SetOutputPattern (ClOutputPattern* ai_pattern)
      {
        //## begin ClModuleInfo::SetOutputPattern%-1699606366.body preserve=yes

 m_pattern		= ai_pattern;	

        //## end ClModuleInfo::SetOutputPattern%-1699606366.body
      }

      //## Operation: GetCodeExceptions%2017224066; C++
      ClStaticResourceList* GetCodeExceptions ();

      //## Operation: GetOutputPattern%-597407052; C++
      ClOutputPattern* GetOutputPattern ();

      //## Operation: AddCodeException%-1020459840; C++
      m4return_t AddCodeException (m4int32_t ai_code, m4uint32_t ai_level);

      //## Operation: FindCodeException%929810155; C++
      m4uint32_t FindCodeException (m4uint32_t ai_Code);

      //## Operation: IsInModule%-1289820936; C++
      m4bool_t IsInModule (m4uint32_t ai_Code);

      //## Operation: operator =%-192709965; C++
      ClModuleInfo& operator = (const ClModuleInfo& ai_ModuleInfo);

    // Additional Public Declarations
      //## begin ClModuleInfo%377C883F0164.public preserve=yes
      //## end ClModuleInfo%377C883F0164.public

  protected:
    // Additional Protected Declarations
      //## begin ClModuleInfo%377C883F0164.protected preserve=yes
      //## end ClModuleInfo%377C883F0164.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C8840001C
      //## Role: ClModuleInfo::m_sModuleName%377C8840001E
      //## begin ClModuleInfo::m_sModuleName%377C8840001E.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_sModuleName;
      //## end ClModuleInfo::m_sModuleName%377C8840001E.role

      //## Association: M4Log::<unnamed>%377C8840001F
      //## Role: ClModuleInfo::m_InitCode%377C88400021
      //## begin ClModuleInfo::m_InitCode%377C88400021.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_InitCode;
      //## end ClModuleInfo::m_InitCode%377C88400021.role

      //## Association: M4Log::<unnamed>%377C88400022
      //## Role: ClModuleInfo::m_FinalCode%377C88400024
      //## begin ClModuleInfo::m_FinalCode%377C88400024.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_FinalCode;
      //## end ClModuleInfo::m_FinalCode%377C88400024.role

      //## Association: M4Log::<unnamed>%377C88400025
      //## Role: ClModuleInfo::m_ModuleState%377C88400027
      //## begin ClModuleInfo::m_ModuleState%377C88400027.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_ModuleState;
      //## end ClModuleInfo::m_ModuleState%377C88400027.role

      //## Association: M4Log::<unnamed>%377C88400028
      //## Role: ClModuleInfo::m_pCodeExceptions%377C8840002A
      //## begin ClModuleInfo::m_pCodeExceptions%377C8840002A.role preserve=no  private: ClStaticResourceList {1 -> 1RFHAPN}
      ClStaticResourceList *m_pCodeExceptions;
      //## end ClModuleInfo::m_pCodeExceptions%377C8840002A.role

      //## Association: M4Log::<unnamed>%377C8840002B
      //## Role: ClModuleInfo::m_pattern%377C8840002D
      //## begin ClModuleInfo::m_pattern%377C8840002D.role preserve=no  private: ClOutputPattern {1 -> 1RFHAPN}
      ClOutputPattern *m_pattern;
      //## end ClModuleInfo::m_pattern%377C8840002D.role

    // Additional Private Declarations
      //## begin ClModuleInfo%377C883F0164.private preserve=yes
      //## end ClModuleInfo%377C883F0164.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClModuleInfo%377C883F0164.implementation preserve=yes
      //## end ClModuleInfo%377C883F0164.implementation

};

//## begin ClModuleInfo%377C883F0164.postscript preserve=yes


//typedef map< M4ClString  , ClModuleInfo* , less<M4ClString> > MODULEMAP;



//## end ClModuleInfo%377C883F0164.postscript

//## begin ClOutputPattern%377C883F0388.preface preserve=yes
//## end ClOutputPattern%377C883F0388.preface

//## Class: ClOutputPattern%377C883F0388
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4LOG ClOutputPattern 
{
  //## begin ClOutputPattern%377C883F0388.initialDeclarations preserve=yes
  //## end ClOutputPattern%377C883F0388.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetOutputPatternName%-161833025; C++
      M4ClString GetOutputPatternName ()
      {
        //## begin ClOutputPattern::GetOutputPatternName%-161833025.body preserve=yes

 return m_sOutputPatternName;

        //## end ClOutputPattern::GetOutputPatternName%-161833025.body
      }

      //## Operation: GetErrorLevel%1135028648; C++
      m4uint32_t GetErrorLevel ()
      {
        //## begin ClOutputPattern::GetErrorLevel%1135028648.body preserve=yes

 return m_ErrorLevel;	

        //## end ClOutputPattern::GetErrorLevel%1135028648.body
      }

      //## Operation: GetWarningLevel%1383128668; C++
      m4uint32_t GetWarningLevel ()
      {
        //## begin ClOutputPattern::GetWarningLevel%1383128668.body preserve=yes

 return m_WarningLevel;

        //## end ClOutputPattern::GetWarningLevel%1383128668.body
      }

      //## Operation: GetDebugLevel%-979738006; C++
      m4uint32_t GetDebugLevel ()
      {
        //## begin ClOutputPattern::GetDebugLevel%-979738006.body preserve=yes

 return m_DebugLevel;	

        //## end ClOutputPattern::GetDebugLevel%-979738006.body
      }

      //## Operation: GetTraceLevel%-396849595; C++
      m4uint32_t GetTraceLevel ()
      {
        //## begin ClOutputPattern::GetTraceLevel%-396849595.body preserve=yes

 return m_TraceLevel;	

        //## end ClOutputPattern::GetTraceLevel%-396849595.body
      }

      //## Operation: SetOutputPatternName%-1777756523; C++
      void SetOutputPatternName (const M4ClString& ai_OutputPatternName)
      {
        //## begin ClOutputPattern::SetOutputPatternName%-1777756523.body preserve=yes

 m_sOutputPatternName = ai_OutputPatternName;

        //## end ClOutputPattern::SetOutputPatternName%-1777756523.body
      }

      //## Operation: SetErrorLevel%-1083565761; C++
      void SetErrorLevel (m4uint32_t ai_errorLevel)
      {
        //## begin ClOutputPattern::SetErrorLevel%-1083565761.body preserve=yes

 m_ErrorLevel	= ai_errorLevel;

        //## end ClOutputPattern::SetErrorLevel%-1083565761.body
      }

      //## Operation: SetWarningLevel%855990477; C++
      void SetWarningLevel (m4uint32_t ai_warningLevel)
      {
        //## begin ClOutputPattern::SetWarningLevel%855990477.body preserve=yes

 m_WarningLevel= ai_warningLevel;

        //## end ClOutputPattern::SetWarningLevel%855990477.body
      }

      //## Operation: SetDebugLevel%360035548; C++
      void SetDebugLevel (m4uint32_t ai_debugLevel)
      {
        //## begin ClOutputPattern::SetDebugLevel%360035548.body preserve=yes

 m_DebugLevel	= ai_debugLevel;

        //## end ClOutputPattern::SetDebugLevel%360035548.body
      }

      //## Operation: SetTraceLevel%-1582287392; C++
      void SetTraceLevel (m4uint32_t ai_traceLevel)
      {
        //## begin ClOutputPattern::SetTraceLevel%-1582287392.body preserve=yes

 m_TraceLevel	= ai_traceLevel;

        //## end ClOutputPattern::SetTraceLevel%-1582287392.body
      }

    // Additional Public Declarations
      //## begin ClOutputPattern%377C883F0388.public preserve=yes
      //## end ClOutputPattern%377C883F0388.public

  protected:
    // Additional Protected Declarations
      //## begin ClOutputPattern%377C883F0388.protected preserve=yes
      //## end ClOutputPattern%377C883F0388.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C8840004E
      //## Role: ClOutputPattern::m_sOutputPatternName%377C88400050
      //## begin ClOutputPattern::m_sOutputPatternName%377C88400050.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_sOutputPatternName;
      //## end ClOutputPattern::m_sOutputPatternName%377C88400050.role

      //## Association: M4Log::<unnamed>%377C88400051
      //## Role: ClOutputPattern::m_ErrorLevel%377C88400053
      //## begin ClOutputPattern::m_ErrorLevel%377C88400053.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_ErrorLevel;
      //## end ClOutputPattern::m_ErrorLevel%377C88400053.role

      //## Association: M4Log::<unnamed>%377C88400054
      //## Role: ClOutputPattern::m_WarningLevel%377C88400056
      //## begin ClOutputPattern::m_WarningLevel%377C88400056.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_WarningLevel;
      //## end ClOutputPattern::m_WarningLevel%377C88400056.role

      //## Association: M4Log::<unnamed>%377C88400057
      //## Role: ClOutputPattern::m_DebugLevel%377C88400059
      //## begin ClOutputPattern::m_DebugLevel%377C88400059.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_DebugLevel;
      //## end ClOutputPattern::m_DebugLevel%377C88400059.role

      //## Association: M4Log::<unnamed>%377C8840005A
      //## Role: ClOutputPattern::m_TraceLevel%377C8840005C
      //## begin ClOutputPattern::m_TraceLevel%377C8840005C.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_TraceLevel;
      //## end ClOutputPattern::m_TraceLevel%377C8840005C.role

    // Additional Private Declarations
      //## begin ClOutputPattern%377C883F0388.private preserve=yes
      //## end ClOutputPattern%377C883F0388.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClOutputPattern%377C883F0388.implementation preserve=yes
      //## end ClOutputPattern%377C883F0388.implementation

};

//## begin ClOutputPattern%377C883F0388.postscript preserve=yes



//typedef map< M4ClString  , ClOutputPattern* , less<M4ClString> > PATTERNSMAP;



//## end ClOutputPattern%377C883F0388.postscript

//## begin ClDebugResource%377C8840007F.preface preserve=yes
//## end ClDebugResource%377C8840007F.preface

//## Class: ClDebugResource%377C8840007F
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C88400127;m4return_t { -> }

class M4_DECL_M4LOG ClDebugResource 
{
  //## begin ClDebugResource%377C8840007F.initialDeclarations preserve=yes
  //## end ClDebugResource%377C8840007F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDebugResource%806095081; C++
      ClDebugResource ();

      //## Operation: ClDebugResource%1744602673; C++
      ClDebugResource (m4uint32_t ilevelInfo);

    //## Destructor (specified)
      //## Operation: ~ClDebugResource%192280011; C++
      ~ClDebugResource ();


    //## Other Operations (specified)
      //## Operation: GetLevelInfo%119451585; C++
      m4uint32_t GetLevelInfo ();

      //## Operation: SetLevelInfo%235718619; C++
      m4return_t SetLevelInfo (m4uint32_t ilevelInfo);

    // Additional Public Declarations
      //## begin ClDebugResource%377C8840007F.public preserve=yes
      //## end ClDebugResource%377C8840007F.public

  protected:
    // Additional Protected Declarations
      //## begin ClDebugResource%377C8840007F.protected preserve=yes
      //## end ClDebugResource%377C8840007F.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C8840011E
      //## Role: ClDebugResource::m_ilevelInfo%377C88400120
      //## begin ClDebugResource::m_ilevelInfo%377C88400120.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_ilevelInfo;
      //## end ClDebugResource::m_ilevelInfo%377C88400120.role

    // Additional Private Declarations
      //## begin ClDebugResource%377C8840007F.private preserve=yes
      //## end ClDebugResource%377C8840007F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDebugResource%377C8840007F.implementation preserve=yes
      //## end ClDebugResource%377C8840007F.implementation

};

//## begin ClDebugResource%377C8840007F.postscript preserve=yes




//typedef map< m4int32_t  , ClDebugResource* , less<m4int32_t> > RESOURCEMAP;



//## end ClDebugResource%377C8840007F.postscript

//## begin RESOURCEMAP%377C883F009C.preface preserve=yes
//## end RESOURCEMAP%377C883F009C.preface

//## Class: RESOURCEMAP%377C883F009C
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _Ty%377C88410062;ClDebugResource { -> }

typedef map < m4int32_t , ClDebugResource * , less < m4int32_t > > RESOURCEMAP;

//## begin RESOURCEMAP%377C883F009C.postscript preserve=yes
//## end RESOURCEMAP%377C883F009C.postscript

//## begin ClStaticResourceList%377C883E0360.preface preserve=yes
//## end ClStaticResourceList%377C883E0360.preface

//## Class: ClStaticResourceList%377C883E0360
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C883F0092;m4return_t { -> }
//## Uses: <unnamed>%377C883F0093;m4int32_t { -> }
//## Uses: <unnamed>%377C883F0094;m4uint32_t { -> }

class M4_DECL_M4LOG ClStaticResourceList 
{
  //## begin ClStaticResourceList%377C883E0360.initialDeclarations preserve=yes
  //## end ClStaticResourceList%377C883E0360.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStaticResourceList%-1021373867; C++
      ClStaticResourceList ();

    //## Destructor (specified)
      //## Operation: ~ClStaticResourceList%-2107676040; C++
      ~ClStaticResourceList ();


    //## Other Operations (specified)
      //## Operation: GetLevelInfo%455248537; C++
      m4return_t GetLevelInfo (m4int32_t icode, m4uint32_t& ilevelInfo);

      //## Operation: AddResource%-1463548443; C++
      m4return_t AddResource (m4int32_t icode, m4uint32_t ilevelInfo);

      //## Operation: UpdateResource%-115869996; C++
      m4return_t UpResource (m4int32_t icode, m4uint32_t ilevelInfo);

      //## Operation: DeleteResource%411999185; C++
      m4return_t DeleteResource (m4int32_t icode);

      //## Operation: DeleteAllResources%-430054; C++
      m4return_t DeleteAllResources ();

      //## Operation: GetResources%-1627191486; C++
      RESOURCEMAP* GetResources ()
      {
        //## begin ClStaticResourceList::GetResources%-1627191486.body preserve=yes

		return &m_resources;
	

        //## end ClStaticResourceList::GetResources%-1627191486.body
      }

      //## Operation: AddResourceList%1574313136; C++
      m4return_t AddResourceList (const ClStaticResourceList& ai_ResourceList);

      //## Operation: operator +=%-1307957918; C++
      ClStaticResourceList& operator += (const ClStaticResourceList& ai_ResourceList);

    // Additional Public Declarations
      //## begin ClStaticResourceList%377C883E0360.public preserve=yes
      //## end ClStaticResourceList%377C883E0360.public

  protected:
    // Additional Protected Declarations
      //## begin ClStaticResourceList%377C883E0360.protected preserve=yes
      //## end ClStaticResourceList%377C883E0360.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883F00A0
      //## Role: ClStaticResourceList::m_resources%377C883F00A2
      //## begin ClStaticResourceList::m_resources%377C883F00A2.role preserve=no  private: RESOURCEMAP {1 -> 1VHAPN}
      RESOURCEMAP m_resources;
      //## end ClStaticResourceList::m_resources%377C883F00A2.role

    // Additional Private Declarations
      //## begin ClStaticResourceList%377C883E0360.private preserve=yes
      //## end ClStaticResourceList%377C883E0360.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStaticResourceList%377C883E0360.implementation preserve=yes
      //## end ClStaticResourceList%377C883E0360.implementation

};

//## begin ClStaticResourceList%377C883E0360.postscript preserve=yes
//## end ClStaticResourceList%377C883E0360.postscript

//## begin MODULEMAP%377C883F0148.preface preserve=yes
//## end MODULEMAP%377C883F0148.preface

//## Class: MODULEMAP%377C883F0148; Instantiated Class
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _K%377C88450356;M4ClString { -> F}
//## Uses: _Ty%377C88450357;ClModuleInfo { -> F}

typedef map < M4ClString , ClModuleInfo * , less < M4ClString > > MODULEMAP;

//## begin MODULEMAP%377C883F0148.postscript preserve=yes
//## end MODULEMAP%377C883F0148.postscript

//## begin PATTERNSMAP%377C883F0380.preface preserve=yes
//## end PATTERNSMAP%377C883F0380.preface

//## Class: PATTERNSMAP%377C883F0380; Instantiated Class
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _K%377C8845034C;M4ClString { -> F}
//## Uses: _Ty%377C8845034D;ClOutputPattern { -> F}

typedef map < M4ClString , ClOutputPattern * , less < M4ClString > > PATTERNSMAP;

//## begin PATTERNSMAP%377C883F0380.postscript preserve=yes
//## end PATTERNSMAP%377C883F0380.postscript

//## begin ClPatternsMap%377C883F0142.preface preserve=yes
//## end ClPatternsMap%377C883F0142.preface

//## Class: ClPatternsMap%377C883F0142
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C883F0387;m4return_t { -> }
//## Uses: <unnamed>%377C883F0389;ClOutputPattern { -> }
//## Uses: <unnamed>%377C883F038C;M4ClString { -> }

class M4_DECL_M4LOG ClPatternsMap 
{
  //## begin ClPatternsMap%377C883F0142.initialDeclarations preserve=yes
  //## end ClPatternsMap%377C883F0142.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPatternsMap%-430534877; C++
      ClPatternsMap ();

    //## Destructor (specified)
      //## Operation: ~ClPatternsMap%-1564202931; C++
      ~ClPatternsMap ();


    //## Other Operations (specified)
      //## Operation: addOutputPattern%1079849859; C++
      m4return_t addOutputPattern (ClOutputPattern* ai_pattern);

      //## Operation: getOutputPatternByName%783728752; C++
      ClOutputPattern* getOutputPatternByName (const M4ClString& ai_patternName);

      //## Operation: Dump%2057842898; C++
      void Dump ();

    // Additional Public Declarations
      //## begin ClPatternsMap%377C883F0142.public preserve=yes
      //## end ClPatternsMap%377C883F0142.public

  protected:
    // Additional Protected Declarations
      //## begin ClPatternsMap%377C883F0142.protected preserve=yes
      //## end ClPatternsMap%377C883F0142.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883F0384
      //## Role: ClPatternsMap::m_patternsMap%377C883F0386
      //## begin ClPatternsMap::m_patternsMap%377C883F0386.role preserve=no  private: PATTERNSMAP {1 -> 1VHAPN}
      PATTERNSMAP m_patternsMap;
      //## end ClPatternsMap::m_patternsMap%377C883F0386.role

    // Additional Private Declarations
      //## begin ClPatternsMap%377C883F0142.private preserve=yes
      //## end ClPatternsMap%377C883F0142.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPatternsMap%377C883F0142.implementation preserve=yes
      //## end ClPatternsMap%377C883F0142.implementation

};

//## begin ClPatternsMap%377C883F0142.postscript preserve=yes
//## end ClPatternsMap%377C883F0142.postscript

//## begin ClStaticModuleList%377C883E0393.preface preserve=yes
//## end ClStaticModuleList%377C883E0393.preface

//## Class: ClStaticModuleList%377C883E0393
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C883F013B;m4return_t { -> }
//## Uses: <unnamed>%377C883F013C;m4int32_t { -> }
//## Uses: <unnamed>%377C883F015C;ClStaticResourceList { -> }
//## Uses: <unnamed>%377C883F015E;ClPatternsMap { -> }
//## Uses: <unnamed>%377C883F0165;ClModuleInfo { -> }
//## Uses: <unnamed>%377C883F0168;m4pchar_t { -> }
//## Uses: <unnamed>%3782376200BA;ClLog { -> }

class ClStaticModuleList 
{
  //## begin ClStaticModuleList%377C883E0393.initialDeclarations preserve=yes

  //## end ClStaticModuleList%377C883E0393.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStaticModuleList%219287769; C++
      ClStaticModuleList ();

      //## Operation: ClStaticModuleList%1772360769; C++
      ClStaticModuleList (m4uint32_t ai_Modules);

    //## Destructor (specified)
      //## Operation: ~ClStaticModuleList%-1535370729; C++
      ~ClStaticModuleList ();


    //## Other Operations (specified)
      //## Operation: GetLevelInfo%-489667911; C++
      m4return_t GetLevelInfo (const ClLogSystem *ai_pcoLogSystem, m4int32_t ai_Sever, m4int32_t ai_code, m4uint32_t& ilevelInfo);

      //## Operation: operator =%-1144838330; C++
      ClStaticModuleList& operator = (const ClStaticModuleList& );

      //## Operation: GetDefaultCodeExceptions%-1594431959; C++
      ClStaticResourceList* GetDefaultCodeExceptions ()
      {
        //## begin ClStaticModuleList::GetDefaultCodeExceptions%-1594431959.body preserve=yes

 return m_pDefaultCodeExceptions;

        //## end ClStaticModuleList::GetDefaultCodeExceptions%-1594431959.body
      }

      //## Operation: getOutputPatternMap%-1832169331; C++
      ClPatternsMap* getOutputPatternMap ()
      {
        //## begin ClStaticModuleList::getOutputPatternMap%-1832169331.body preserve=yes

 return m_patternsMap;	

        //## end ClStaticModuleList::getOutputPatternMap%-1832169331.body
      }

      //## Operation: GetModuleListSize%-328154895; C++
      m4uint32_t GetModuleListSize ();

      //## Operation: SetFlagsLevels%-1436605533; C++
      m4return_t SetFlagsLevels (m4uint32_t* ai_FlagsLevels);

      //## Operation: AddModule%-663784900; C++
      m4return_t AddModule (ClModuleInfo* ai_Module);

      //## Operation: GetModuleByName%-39832169; C++
      ClModuleInfo* GetModuleByName (m4pchar_t ai_ModuleName);

    // Additional Public Declarations
      //## begin ClStaticModuleList%377C883E0393.public preserve=yes
      //## end ClStaticModuleList%377C883E0393.public

  protected:
    // Additional Protected Declarations
      //## begin ClStaticModuleList%377C883E0393.protected preserve=yes
      //## end ClStaticModuleList%377C883E0393.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_ModuleList%377C883F0152
      //## begin ClStaticModuleList::m_ModuleList%377C883F0152.attr preserve=no  private: ClModuleInfo** {VA} 
      ClModuleInfo** m_ModuleList;
      //## end ClStaticModuleList::m_ModuleList%377C883F0152.attr

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883F0143
      //## Role: ClStaticModuleList::m_patternsMap%377C883F0145
      //## begin ClStaticModuleList::m_patternsMap%377C883F0145.role preserve=no  private: ClPatternsMap {1 -> 1RHAPN}
      ClPatternsMap *m_patternsMap;
      //## end ClStaticModuleList::m_patternsMap%377C883F0145.role

      //## Association: M4Log::<unnamed>%377C883F014C
      //## Role: ClStaticModuleList::m_ModuleMap%377C883F014E
      //## begin ClStaticModuleList::m_ModuleMap%377C883F014E.role preserve=no  private: MODULEMAP {1 -> 1VHAPN}
      MODULEMAP m_ModuleMap;
      //## end ClStaticModuleList::m_ModuleMap%377C883F014E.role

      //## Association: M4Log::<unnamed>%377C883F014F
      //## Role: ClStaticModuleList::m_Size%377C883F0151
      //## begin ClStaticModuleList::m_Size%377C883F0151.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_Size;
      //## end ClStaticModuleList::m_Size%377C883F0151.role

      //## Association: M4Log::<unnamed>%377C883F0153
      //## Role: ClStaticModuleList::m_FlagsLevels%377C883F0155
      //## begin ClStaticModuleList::m_FlagsLevels%377C883F0155.role preserve=no  private: m4uint32_t {1 -> 1RHAPN}
      m4uint32_t *m_FlagsLevels;
      //## end ClStaticModuleList::m_FlagsLevels%377C883F0155.role

      //## Association: M4Log::<unnamed>%377C883F0156
      //## Role: ClStaticModuleList::m_AddIterator%377C883F0158
      //## begin ClStaticModuleList::m_AddIterator%377C883F0158.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_AddIterator;
      //## end ClStaticModuleList::m_AddIterator%377C883F0158.role

      //## Association: M4Log::<unnamed>%377C883F0159
      //## Role: ClStaticModuleList::m_pDefaultCodeExceptions%377C883F015B
      //## begin ClStaticModuleList::m_pDefaultCodeExceptions%377C883F015B.role preserve=no  private: ClStaticResourceList {1 -> 1RHAPN}
      ClStaticResourceList *m_pDefaultCodeExceptions;
      //## end ClStaticModuleList::m_pDefaultCodeExceptions%377C883F015B.role

    // Additional Private Declarations
      //## begin ClStaticModuleList%377C883E0393.private preserve=yes
      //## end ClStaticModuleList%377C883E0393.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStaticModuleList%377C883E0393.implementation preserve=yes
      //## end ClStaticModuleList%377C883E0393.implementation

};

//## begin ClStaticModuleList%377C883E0393.postscript preserve=yes
//## end ClStaticModuleList%377C883E0393.postscript

//## begin module%377C883E00F3.epilog preserve=yes
//## end module%377C883E00F3.epilog


#endif
