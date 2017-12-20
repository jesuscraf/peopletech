//## begin module%3433D307015F.cm preserve=no
//## end module%3433D307015F.cm

//## begin module%3433D307015F.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Statistics
//	 File:				Statistics.hpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971020
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
//## end module%3433D307015F.cp

//## Module: Statistics%3433D307015F; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: d:\eduardoma\m4serverbase\version\inc\statistics.hpp

#ifndef statistics_h
#define statistics_h 1

//## begin module%3433D307015F.additionalIncludes preserve=no
//## end module%3433D307015F.additionalIncludes

//## begin module%3433D307015F.includes preserve=yes
#include <m4serverbase_dll.hpp>
//## end module%3433D307015F.includes

// m4types
#include <m4types.hpp>
// m4date
#include <m4date.hpp>
// StatUtil
#include <statutil.hpp>
// DataStorages
#include <datastorages.hpp>
//## begin module%3433D307015F.declarations preserve=no
//## end module%3433D307015F.declarations

//## begin module%3433D307015F.additionalDeclarations preserve=yes
#define	DEF_STRING_SIZE	20
//## end module%3433D307015F.additionalDeclarations


//## Class: ClVisitor%344F51C9007D
//	This class provides an abstract public interface for accessing statistical
//	information from any Active Server object
//	.
//	This approach is based on the visitor behavioral pattern as described in
//	[GAM].
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClVisitor 
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClVisitor%877616167
      //	Destructor for class "ClVisitor".
      virtual ~ClVisitor ();

  protected:
    //## Constructors (specified)
      //## Operation: ClVisitor%877616166; C++
      //	Constructor for class ClVisitor.
      ClVisitor ();

  private:
  private: //## implementation
};

//## Class: ClStatHandle%3469A5F6022E; Metaclass
//	This class encapsulate an identifier to serve as an index for a particular
//	statistic value.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3469AAB7020F;m4handle_t { -> }
//## Uses: <unnamed>%34D893D60239;m4int32_t { -> }

class ClStatHandle 
{
  public:
    //## Constructors (specified)
      //## Operation: ClStatHandle%886596298; C++
      ClStatHandle (m4uint32_t ai_uiVal = 0);

      //## Operation: ClStatHandle%886596299; C++
      ClStatHandle (m4uint16_t ai_uiHighVal, m4uint16_t ai_uiLowVal);


    //## Other Operations (specified)
      //## Operation: SetLowID%886596300; C++
      void SetLowID (m4uint16_t ai_uiVal);

      //## Operation: SetHighID%886596301; C++
      void SetHighID (m4uint16_t ai_uiVal);

      //## Operation: GetLowID%886596302
      m4uint16_t GetLowID ();

      //## Operation: GetHighID%886596303
      m4uint16_t GetHighID ();

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_uiLValue%34D892AB0110
      //## begin ClStatHandle::m_uiLValue%34D892AB0110.attr preserve=no  private: m4uint16_t {UA} 
      m4uint16_t m_uiLValue;
      //## end ClStatHandle::m_uiLValue%34D892AB0110.attr

      //## Attribute: m_uiHValue%34D899720343
      //## begin ClStatHandle::m_uiHValue%34D899720343.attr preserve=no  private: m4uint16_t {UA} 
      m4uint16_t m_uiHValue;
      //## end ClStatHandle::m_uiHValue%34D899720343.attr

  private: //## implementation
};

//## Class: ClStatistic%3459CF5E0148
//	This class provides a general-purpose container container where statistical
//	data is to be stored. A specific visitor can retrieve such data and process
//	them conveniently.
//
//	Access to the contents of this class is performed on an ID basis and through
//	the appropriate get/set functions.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B6393401C3;ClStatHandle { -> }
//## Uses: <unnamed>%34D8ADDF00E1;ClDataUnit { -> }

class M4_DECL_M4SERVERBASE ClStatistic 
{
  protected:
    //## Class: eStatsID%34CF0A580114; protected
    //## Category: M4ServerBase::Statistics%341FE6E202CA
    //## Subsystem: M4ServerBase::inc%378B07C10202
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {TOTAL_NUM} eStatsID;

  public:
    //## Destructor (specified)
      //## Operation: ~ClStatistic%915791252
      virtual ~ClStatistic ();


    //## Other Operations (specified)
      //## Operation: GetStat%879346821; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      virtual m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData) = 0;

      //## Operation: Reset%879346829; C++
      //	This operation resets every value in the statistics object. In this class
      //	this operation is abstract and must be defined in derived classes.
      virtual m4return_t Reset () = 0;

      //## Operation: ResetStat%879346820
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      virtual m4return_t ResetStat (ClStatHandle &ai_hStatId) = 0;

      //## Operation: GetNumberOfStats%885983749; C++
      m4uint32_t GetNumberOfStats () const;

  protected:
    //## Constructors (specified)
      //## Operation: ClStatistic%879346819
      //	Default constructor for class ClStatistics.
      ClStatistic ();


    //## Other Operations (specified)
      //## Operation: _CheckStatID%886596297; C++
      m4return_t _CheckStatID (ClStatHandle &ai_hStatId, m4uint16_t ai_uiUniqueID);

    // Data Members for Class Attributes

      //## Attribute: m_uiNumberOfStats%34CF02B8001A
      //## begin ClStatistic::m_uiNumberOfStats%34CF02B8001A.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiNumberOfStats;
      //## end ClStatistic::m_uiNumberOfStats%34CF02B8001A.attr

      //## Attribute: m_uiTypeID%34CF02CB03D3
      //## begin ClStatistic::m_uiTypeID%34CF02CB03D3.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiTypeID;
      //## end ClStatistic::m_uiTypeID%34CF02CB03D3.attr

  private:
  private: //## implementation
};

//## Class: ClStatTimeType%3469A66B021F
//	This class is a return type for statistic values based on absolute or
//	relative time.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3469AAD20328;M4ClTimeStamp { -> }
//## Uses: <unnamed>%350FB17001DF;m4double_t { -> }

typedef m4double_t ClStatTimeType;

//## Class: ClStatNumericalType%3469A6F5021F
//	This class is a return type for numerical statistic values.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3469AAF002AB;m4int32_t { -> }

typedef m4int32_t ClStatNumericalType;

//## Class: ClStatAverageType%3469A77102CB
//	This class is a return type for average numerical statistic values.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3469AB090338;m4float_t { -> }

typedef m4float_t ClStatAverageType;

//## Class: ClStatAverageContainer%34B61D3E028F; Instantiated Class
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B637740108;ClStatAverageType { -> }

typedef M4FixedTermAvg< ClStatAverageType > ClStatAverageContainer;

//## Class: ClStatPercentType%35056F7E0374
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35056FD501A2;m4float_t { -> }

typedef m4float_t ClStatPercentType;

// Class ClVisitor 

// Metaclass ClStatHandle 

inline ClStatHandle::ClStatHandle (m4uint32_t ai_uiVal)
  //## begin ClStatHandle::ClStatHandle%886596298.hasinit preserve=no
  //## end ClStatHandle::ClStatHandle%886596298.hasinit
  //## begin ClStatHandle::ClStatHandle%886596298.initialization preserve=yes
  //## end ClStatHandle::ClStatHandle%886596298.initialization
{
  //## begin ClStatHandle::ClStatHandle%886596298.body preserve=yes
	m4uint32_t	tmpVal;

	tmpVal = ai_uiVal;
	tmpVal >>= 16;
	m_uiLValue = (m4uint16_t)(ai_uiVal & 0xffff);
	m_uiHValue = (m4uint16_t)(tmpVal & 0xffff);
  //## end ClStatHandle::ClStatHandle%886596298.body
}

inline ClStatHandle::ClStatHandle (m4uint16_t ai_uiHighVal, m4uint16_t ai_uiLowVal)
  //## begin ClStatHandle::ClStatHandle%886596299.hasinit preserve=no
  //## end ClStatHandle::ClStatHandle%886596299.hasinit
  //## begin ClStatHandle::ClStatHandle%886596299.initialization preserve=yes
  :  m_uiHValue(ai_uiHighVal), m_uiLValue(ai_uiLowVal)
  //## end ClStatHandle::ClStatHandle%886596299.initialization
{
  //## begin ClStatHandle::ClStatHandle%886596299.body preserve=yes
  //## end ClStatHandle::ClStatHandle%886596299.body
}



//## Other Operations (inline)
inline void ClStatHandle::SetLowID (m4uint16_t ai_uiVal)
{
  //## begin ClStatHandle::SetLowID%886596300.body preserve=yes
	m_uiLValue = ai_uiVal;
  //## end ClStatHandle::SetLowID%886596300.body
}

inline void ClStatHandle::SetHighID (m4uint16_t ai_uiVal)
{
  //## begin ClStatHandle::SetHighID%886596301.body preserve=yes
	m_uiHValue = ai_uiVal;
  //## end ClStatHandle::SetHighID%886596301.body
}

inline m4uint16_t ClStatHandle::GetLowID ()
{
  //## begin ClStatHandle::GetLowID%886596302.body preserve=yes
	return m_uiLValue;
  //## end ClStatHandle::GetLowID%886596302.body
}

inline m4uint16_t ClStatHandle::GetHighID ()
{
  //## begin ClStatHandle::GetHighID%886596303.body preserve=yes
	return m_uiHValue;
  //## end ClStatHandle::GetHighID%886596303.body
}

// Class ClStatistic 


//## Other Operations (inline)
inline m4uint32_t ClStatistic::GetNumberOfStats () const
{
  //## begin ClStatistic::GetNumberOfStats%885983749.body preserve=yes
	return m_uiNumberOfStats;
  //## end ClStatistic::GetNumberOfStats%885983749.body
}

// Metaclass ClStatHandle 



//## begin module%3433D307015F.epilog preserve=yes
//## end module%3433D307015F.epilog


#endif
