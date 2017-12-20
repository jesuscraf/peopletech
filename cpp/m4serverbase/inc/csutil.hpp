//## begin module%341FFCBB0105.cm preserve=no
//## end module%341FFCBB0105.cm

//## begin module%341FFCBB0105.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Utilities
//	 File:				CSUtil.hpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971002
//	 Language:			C++
//	 Operating System:	WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%341FFCBB0105.cp

//## Module: CSUtil%341FFCBB0105; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: d:\eduardoma\m4serverbase\inc\csutil.hpp

#ifndef csutil_h
#define csutil_h 1

//## begin module%341FFCBB0105.additionalIncludes preserve=no
//## end module%341FFCBB0105.additionalIncludes

//## begin module%341FFCBB0105.includes preserve=yes
//## end module%341FFCBB0105.includes

// m4types
#include <m4types.hpp>
// m4thread
#include <m4thread.hpp>
// Statistics
#include <statistics.hpp>

class M4_DECL_M4SERVERBASE ClActiveClass;

//## begin module%341FFCBB0105.declarations preserve=no
//## end module%341FFCBB0105.declarations

//## begin module%341FFCBB0105.additionalDeclarations preserve=yes
const m4int_t	SelectMaxRetries = 10;
//## end module%341FFCBB0105.additionalDeclarations


//## begin ClActiveClassStats%346221120006.preface preserve=yes
//## end ClActiveClassStats%346221120006.preface

//## Class: ClActiveClassStats%346221120006
//	This class provides a  container where Service Execution statistical data is
//	to be stored. A specific visitor can retrieve such data and process them
//	conveniently.
//
//	Access to the contents of this class is performed on an ID basis and through
//	the appropriate get/set functions.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B65B7A0185;ClStatAverageType { -> }

class M4_DECL_M4SERVERBASE ClActiveClassStats : public ClStatistic  //## Inherits: <unnamed>%34B39E6E02FD
{
  //## begin ClActiveClassStats%346221120006.initialDeclarations preserve=yes
  //## end ClActiveClassStats%346221120006.initialDeclarations

  public:
    //## Class: eActiveClassStatsID%34CF0B4703C3
    //## Category: M4ServerBase%35EEAFAA0123
    //## Subsystem: M4ServerBase::inc%378B07C10202
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {PROC_ITEMS_ID = ClStatistic::TOTAL_NUM, AVG_PROC_ITEMS_ID, TOTAL_NUM} eActiveClassStatsID;

    //## Constructors (specified)
      //## Operation: ClActiveClassStats%884336013
      //	Default constructor for class ClStatistics.
      ClActiveClassStats ();

    //## Destructor (specified)
      //## Operation: ~ClActiveClassStats%884336014
      //	Default constructor for class ClStatistics.
      ~ClActiveClassStats ();


    //## Other Operations (specified)
      //## Operation: GetAvgProcessedItems%884336011; C++
      ClStatAverageType GetAvgProcessedItems (m4uint16_t ai_uiPeriod = 2);

      //## Operation: AddAvgProcessedItems%884336012; C++
      void AddAvgProcessedItems (ClStatAverageType &ai_avgValue);

      //## Operation: Reset%879346831; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%886502776; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%886502778
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: nProcessedItems%347D44B102EB
      //	Stores the number of items processed by this thread.
      const ClStatNumericalType& GetnProcessedItems () const;
      void AddnProcessedItems (const ClStatNumericalType& value);

    // Data Members for Class Attributes

      //## begin ClActiveClassStats::nProcessedItems%347D44B102EB.attr preserve=no  public: ClStatNumericalType {UA} 0
      ClStatNumericalType m_nProcessedItems;
      //## end ClActiveClassStats::nProcessedItems%347D44B102EB.attr

  public:
    // Additional Public Declarations
      //## begin ClActiveClassStats%346221120006.public preserve=yes
      //## end ClActiveClassStats%346221120006.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::Utilities::<unnamed>%34B6584B02EC
      //## Role: ClActiveClassStats::m_avgProcessedItems%34B6584C0118
      //## begin ClActiveClassStats::m_avgProcessedItems%34B6584C0118.role preserve=no  protected: ClStatAverageContainer {1 -> 1VHAN}
      ClStatAverageContainer m_avgProcessedItems;
      //## end ClActiveClassStats::m_avgProcessedItems%34B6584C0118.role

    // Additional Protected Declarations
      //## begin ClActiveClassStats%346221120006.protected preserve=yes
      //## end ClActiveClassStats%346221120006.protected

  private:
    // Additional Private Declarations
      //## begin ClActiveClassStats%346221120006.private preserve=yes
      //## end ClActiveClassStats%346221120006.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActiveClassStats%346221120006.implementation preserve=yes
      //## end ClActiveClassStats%346221120006.implementation

};

//## begin ClActiveClassStats%346221120006.postscript preserve=yes
//## end ClActiveClassStats%346221120006.postscript

//## begin ClActiveClassVisitor%34BBA64D0212.preface preserve=yes
//## end ClActiveClassVisitor%34BBA64D0212.preface

//## Class: ClActiveClassVisitor%34BBA64D0212
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BBA6A10231;ClActiveClass { -> F}
//## Uses: <unnamed>%34BBA6C50260;ClActiveClassStats { -> }

class M4_DECL_M4SERVERBASE ClActiveClassVisitor : public ClVisitor  //## Inherits: <unnamed>%34BBA66502BE
{
  //## begin ClActiveClassVisitor%34BBA64D0212.initialDeclarations preserve=yes
  //## end ClActiveClassVisitor%34BBA64D0212.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClActiveClassVisitor%884712762; C++
      //	Constructor for class ClVisitor.
      ClActiveClassVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitActiveClass%884712763; C++
      //	This virtual operation accesses ClCSACtiveClass information from the
      //	instance provided through the ai_oActiveClass argument.
      //	Remarks
      virtual void VisitActiveClass (ClActiveClass &ai_oActiveClass);

    // Additional Public Declarations
      //## begin ClActiveClassVisitor%34BBA64D0212.public preserve=yes
      //## end ClActiveClassVisitor%34BBA64D0212.public

  protected:
    // Additional Protected Declarations
      //## begin ClActiveClassVisitor%34BBA64D0212.protected preserve=yes
      //## end ClActiveClassVisitor%34BBA64D0212.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_nLastProcessedItems%34BCEEAA00D7
      //## begin ClActiveClassVisitor::m_nLastProcessedItems%34BCEEAA00D7.attr preserve=no  private: ClStatNumericalType {UA} 0
      ClStatNumericalType m_nLastProcessedItems;
      //## end ClActiveClassVisitor::m_nLastProcessedItems%34BCEEAA00D7.attr

    // Additional Private Declarations
      //## begin ClActiveClassVisitor%34BBA64D0212.private preserve=yes
      //## end ClActiveClassVisitor%34BBA64D0212.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActiveClassVisitor%34BBA64D0212.implementation preserve=yes
      //## end ClActiveClassVisitor%34BBA64D0212.implementation

};

//## begin ClActiveClassVisitor%34BBA64D0212.postscript preserve=yes
//## end ClActiveClassVisitor%34BBA64D0212.postscript

//## begin ClActiveClass%341FFAA60059.preface preserve=yes
//## end ClActiveClass%341FFAA60059.preface

//## Class: ClActiveClass%341FFAA60059
//	This class provides an abstract public interface for active classes that
//	conforms to the following constraints:
//	 - Has a set of threads on property.
//	 - Has a ClCSStatistics derived object on property.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BBA6A90203;ClActiveClassVisitor { -> }
//## Uses: <unnamed>%34BBA6F20280;ClActiveClassStats { -> }

class M4_DECL_M4SERVERBASE ClActiveClass : public M4Thread  //## Inherits: <unnamed>%342101D50314
{
  //## begin ClActiveClass%341FFAA60059.initialDeclarations preserve=yes
  //## end ClActiveClass%341FFAA60059.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClActiveClass%875808403; C++
      //	Parameterized constructor for class "ClActiveClass".
      //## Concurrency: Synchonous
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ClActiveClass (ClStatistic *ai_pStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClActiveClass%875808404; C++
      //	Default destructor for class "ClActiveClass".
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ~ClActiveClass ();


    //## Other Operations (specified)
      //## Operation: Accept%877356006; C++
      //	Allows a visitor to enter the class and retrieve statistics.
      virtual void Accept (ClActiveClassVisitor &ai_oStats);

      //## Operation: GetStats%879346830; C++
      //	Retrieves a pointer to a "ClStatistics" derived object to hold statistics.
      ClStatistic * GetStats ();

      //## Operation: SetStats%893927989; C++
      //	Retrieves the previous pointer to a "ClStatistics" derived object to hold
      //	statistics and repleces it with a new one.
      ClStatistic * SetStats (ClStatistic *ai_pStats);

      //## Operation: Start%901194407; C++
      m4int16_t Start (m4pchar_t ai_pszName = NULL);

    // Additional Public Declarations
      //## begin ClActiveClass%341FFAA60059.public preserve=yes
      //## end ClActiveClass%341FFAA60059.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::Utilities::<unnamed>%3433CC720344
      //## Role: ClActiveClass::m_pVisitor%3433CC720345
      //## begin ClActiveClass::m_pVisitor%3433CC720345.role preserve=no  protected: ClActiveClassVisitor {1 -> 1RHAN}
      ClActiveClassVisitor *m_pVisitor;
      //## end ClActiveClass::m_pVisitor%3433CC720345.role

      //## Association: M4ServerBase::Utilities::<unnamed>%34C4D0DF0136
      //## Role: ClActiveClass::m_pStats%34C4D0DF0201
      //## begin ClActiveClass::m_pStats%34C4D0DF0201.role preserve=no  protected: ClStatistic {1 -> 1RHAN}
      ClStatistic *m_pStats;
      //## end ClActiveClass::m_pStats%34C4D0DF0201.role

      //## Association: M4ServerBase::Utilities::<unnamed>%35B721C80277
      //## Role: ClActiveClass::m_pszName%35B721C900C2
      //## begin ClActiveClass::m_pszName%35B721C900C2.role preserve=no  protected: m4char_t {1 -> 64VHAN}
      m4char_t m_pszName[64];
      //## end ClActiveClass::m_pszName%35B721C900C2.role

    // Additional Protected Declarations
      //## begin ClActiveClass%341FFAA60059.protected preserve=yes
      //## end ClActiveClass%341FFAA60059.protected

  private:
    // Additional Private Declarations
      //## begin ClActiveClass%341FFAA60059.private preserve=yes
      //## end ClActiveClass%341FFAA60059.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActiveClass%341FFAA60059.implementation preserve=yes
      //## end ClActiveClass%341FFAA60059.implementation

};

//## begin ClActiveClass%341FFAA60059.postscript preserve=yes
//## end ClActiveClass%341FFAA60059.postscript

// Class ClActiveClassStats 

//## Get and Set Operations for Class Attributes (inline)

inline const ClStatNumericalType& ClActiveClassStats::GetnProcessedItems () const
{
  //## begin ClActiveClassStats::GetnProcessedItems%347D44B102EB.get preserve=yes
  return m_nProcessedItems;
  //## end ClActiveClassStats::GetnProcessedItems%347D44B102EB.get
}

inline void ClActiveClassStats::AddnProcessedItems (const ClStatNumericalType& value)
{
  //## begin ClActiveClassStats::AddnProcessedItems%347D44B102EB.set preserve=yes
  m_nProcessedItems += value;
  //## end ClActiveClassStats::AddnProcessedItems%347D44B102EB.set
}

// Class ClActiveClassVisitor 

// Class ClActiveClass 


//## Other Operations (inline)
inline void ClActiveClass::Accept (ClActiveClassVisitor &ai_oStats)
{
  //## begin ClActiveClass::Accept%877356006.body preserve=yes
	ai_oStats.VisitActiveClass(*this);
  //## end ClActiveClass::Accept%877356006.body
}

//## begin module%341FFCBB0105.epilog preserve=yes
//## end module%341FFCBB0105.epilog


#endif
