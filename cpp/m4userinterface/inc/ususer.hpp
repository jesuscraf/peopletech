//## begin module%342240650323.cm preserve=no
//## end module%342240650323.cm

//## begin module%342240650323.cp preserve=no
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
//## end module%342240650323.cp

//## Module: USUser%342240650323; Package specification
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Source file: D:\eduardoma\m4userinterface\inc\ususer.hpp

#ifndef ususer_h
#define ususer_h 1

//## begin module%342240650323.additionalIncludes preserve=no
#include <m4userinterface_dll.hpp>
#include <statistics.hpp>
#include <usinterface.hpp>
//## begin module%342240650323.declarations preserve=no
//## end module%342240650323.declarations

//## begin module%342240650323.additionalDeclarations preserve=yes
#define SYSTEMUSER		"System User"
#define	SYSTEMPASSWORD	"System Password"
#define SYSTEMHOST		"System Host"
//## end module%342240650323.additionalDeclarations


//## begin ClUSUserStats%34E9A7FB028C.preface preserve=yes
//## end ClUSUserStats%34E9A7FB028C.preface

//## Class: ClUSUserStats%34E9A7FB028C
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4USERINTERFACE ClUSUserStats : public ClStatistic  //## Inherits: <unnamed>%34E9AA27039A
{
  //## begin ClUSUserStats%34E9A7FB028C.initialDeclarations preserve=yes
  //## end ClUSUserStats%34E9A7FB028C.initialDeclarations

  public:
    //## begin ClUSUserStats::eUserStats%378B48160087.preface preserve=yes
    //## end ClUSUserStats::eUserStats%378B48160087.preface

    //## Class: eUserStats%378B48160087
    //## Category: M4UserInterface%35EFA1CC011D
    //## Subsystem: M4UserInterface::inc%3795DCBF0108
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {ACTIVE_SESSIONS = ClStatistic::TOTAL_NUM ,  TOTAL_NUM } eUserStats;

    //## begin ClUSUserStats::eUserStats%378B48160087.postscript preserve=yes
    //## end ClUSUserStats::eUserStats%378B48160087.postscript

    //## Constructors (specified)
      //## Operation: ClUSUserStats%887707934
      ClUSUserStats ();

    //## Destructor (specified)
      //## Operation: ~ClUSUserStats%887707935
      ~ClUSUserStats ();


    //## Other Operations (specified)
      //## Operation: Reset%887707940; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%887707941; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%887707942
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: nTotalSessions%350DBB1D0071
      const ClStatNumericalType& GetnTotalSessions () const;
      void AddnTotalSessions (const ClStatNumericalType& value);

    // Data Members for Class Attributes

      //## begin ClUSUserStats::nTotalSessions%350DBB1D0071.attr preserve=no  public: ClStatNumericalType {UA} 0
      ClStatNumericalType m_nTotalSessions;
      //## end ClUSUserStats::nTotalSessions%350DBB1D0071.attr

  public:
    // Additional Public Declarations
      //## begin ClUSUserStats%34E9A7FB028C.public preserve=yes
      //## end ClUSUserStats%34E9A7FB028C.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSUserStats%34E9A7FB028C.protected preserve=yes
      //## end ClUSUserStats%34E9A7FB028C.protected

  private:
    // Additional Private Declarations
      //## begin ClUSUserStats%34E9A7FB028C.private preserve=yes
      //## end ClUSUserStats%34E9A7FB028C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSUserStats%34E9A7FB028C.implementation preserve=yes
      //## end ClUSUserStats%34E9A7FB028C.implementation

};

//## begin ClUSUserStats%34E9A7FB028C.postscript preserve=yes
//## end ClUSUserStats%34E9A7FB028C.postscript

//## begin ClUSUser%353DE72200D2.preface preserve=yes
//## end ClUSUser%353DE72200D2.preface

//## Class: ClUSUser%353DE72200D2
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373B1C580164;ClUSUserStats { -> }

class M4_DECL_M4USERINTERFACE ClUSUser : public ClUSUserInterface  //## Inherits: <unnamed>%353DE75903C0
{
  //## begin ClUSUser%353DE72200D2.initialDeclarations preserve=yes
  //## end ClUSUser%353DE72200D2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSUser%893349374; C++
      //	Default constructor
      ClUSUser (const m4string_t &ai_strUserID, const m4string_t &ai_strName, ClStatistic *ai_poStatistic, const m4string_t ai_strUserAlias = "", const m4string_t ai_strUserImpersonator = "");


    //## Other Operations (specified)
      //## Operation: Recycle%941721551; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721552
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721553
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721554
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClUSUser%353DE72200D2.public preserve=yes
      //## end ClUSUser%353DE72200D2.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSUser%353DE72200D2.protected preserve=yes
      //## end ClUSUser%353DE72200D2.protected

  private:
    // Additional Private Declarations
      //## begin ClUSUser%353DE72200D2.private preserve=yes
      //## end ClUSUser%353DE72200D2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSUser%353DE72200D2.implementation preserve=yes
      //## end ClUSUser%353DE72200D2.implementation

};

//## begin ClUSUser%353DE72200D2.postscript preserve=yes
//## end ClUSUser%353DE72200D2.postscript

//## Class: ClUSUserVisitor%344F53FC0069
//	The user visitor will visit the userinfo object and will calculate its
//	statistics if
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E9A83703D3;ClUSUserStats { -> }
//## Uses: <unnamed>%373B1C5A003B;ClUSUser { -> }

class M4_DECL_M4USERINTERFACE ClUSUserVisitor : public ClVisitor  //## Inherits: <unnamed>%344F54180037
{
  public:
    //## Constructors (specified)
      //## Operation: ClUSUserVisitor%893330502
      ClUSUserVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitUser%878029377
      void VisitUser (ClUSUserInterface &ai_oUserInfo);

  protected:
  private:
  private: //## implementation
};

// Class ClUSUserStats 

//## Get and Set Operations for Class Attributes (inline)

inline const ClStatNumericalType& ClUSUserStats::GetnTotalSessions () const
{
  //## begin ClUSUserStats::GetnTotalSessions%350DBB1D0071.get preserve=no
  return m_nTotalSessions;
  //## end ClUSUserStats::GetnTotalSessions%350DBB1D0071.get
}

inline void ClUSUserStats::AddnTotalSessions (const ClStatNumericalType& value)
{
  //## begin ClUSUserStats::AddnTotalSessions%350DBB1D0071.set preserve=yes
  m_nTotalSessions += value;
  //## end ClUSUserStats::AddnTotalSessions%350DBB1D0071.set
}

// Class ClUSUser 

// Class ClUSUserVisitor 

//## begin module%342240650323.epilog preserve=yes
//## end module%342240650323.epilog


#endif
