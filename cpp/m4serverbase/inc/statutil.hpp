//## begin module%345898D60074.cm preserve=no
//## end module%345898D60074.cm

//## begin module%345898D60074.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Statistics
//	 File:				StatUtil.hpp
//	 Project:			M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				971030
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
//## end module%345898D60074.cp

//## Module: StatUtil%345898D60074; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: d:\eduardoma\m4serverbase\version\inc\statutil.hpp

#ifndef statutil_h
#define statutil_h 1

//## begin module%345898D60074.additionalIncludes preserve=no
//## end module%345898D60074.additionalIncludes

//## begin module%345898D60074.includes preserve=yes
#include <stdio.h>
//## end module%345898D60074.includes

// m4types
#include <m4types.hpp>
//## begin module%345898D60074.declarations preserve=no
//## end module%345898D60074.declarations

//## begin module%345898D60074.additionalDeclarations preserve=yes
const m4size_t	Def_Avg_Size = 50;
//## end module%345898D60074.additionalDeclarations


//## Class: M4ClosedArray%3458970402B7; Parameterized Class
//	This template class provides the functionality to store a stream of data
//	used as a circular array.
//	The public interface of M4ClosedArray shows a behavior similar to a shift
//	register. Acceptable actions to be performed upon objects of this class
//	include append and random access operations as well as queries to retrieve
//	the size of the stream of data
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3458AAA401CC;m4size_t { -> }

template <class Val>
class M4ClosedArray 
{
  public:
    //## Constructors (specified)
      //## Operation: M4ClosedArray%932983270; C++
      //	Parameterized constructor for class M4ClosedArray.
      M4ClosedArray ();

      //## Operation: M4ClosedArray%878205112; C++
      //	Parameterized constructor for class M4ClosedArray.
      M4ClosedArray (m4size_t ai_uiLength	// Upperbound for the array.
      );

    //## Destructor (specified)
      //## Operation: ~M4ClosedArray%878205126; C++
      //	Default destructor for class M4ClosedArray.
      ~M4ClosedArray ();


    //## Other Operations (specified)
      //## Operation: operator[]%878205113; C++
      //	Random access operator.
      //	Retrieves the entry at the specified position.
      Val & operator [] (m4size_t ai_uiEntry	// Entry to be accessed.
      );

      //## Operation: Append%878205114; C++
      //	Inserts an element and shifts the array if the current size is greater than
      //	the upperbound. Shifted elements are lost.
      void Append (Val &ai_uValue);

      //## Operation: First%878205115; C++
      //	Returns the first element in the array.
      Val & First ();

      //## Operation: Last%878205116; C++
      //	Returns the last element in the array.
      Val & Last ();

      //## Operation: Size%878205117; C++
      //	Returns the number of entries in the array.
      m4size_t Size () const;

      //## Operation: GetHistory%878294075; C++
      //	Returns the number of elements appended to the array since it was
      //	constructed.
      m4size_t GetHistory () const;

      //## Operation: Reset%879851142
      //	Empties the array and sets history back to 0.
      virtual void Reset ();

      //## Operation: SetSize%932983269; C++
      m4return_t SetSize (m4size_t ai_uSize);

      //## Operation: UpperBound%932983271
      m4size_t UpperBound () const;

  protected:

    //## Other Operations (specified)
      //## Operation: _Next%878205127
      //	Shifts the pointer to the current last element in the physical array.
      m4size_t _Next ();

      //## Operation: _First%878205128
      //	Returns the pointer to the first elemet in the physical array.
      m4size_t _First ();

    // Data Members for Class Attributes

      //## Attribute: m_bFull%3458A9D20120
      //	This boolean attribute holds a value of M4_TRUE if the number of elements
      //	inserted is equal or greater than the maximun number of entries.
      //## begin M4ClosedArray::m_bFull%3458A9D20120.attr preserve=no  protected: m4bool_t {UA} M4_FALSE
      m4bool_t m_bFull;
      //## end M4ClosedArray::m_bFull%3458A9D20120.attr

      //## Attribute: m_uiLast%3458A9EE0305
      //	This unsigned attribute holds the index of the last element inserted in the
      //	closed array.
      //## begin M4ClosedArray::m_uiLast%3458A9EE0305.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_uiLast;
      //## end M4ClosedArray::m_uiLast%3458A9EE0305.attr

      //## Attribute: m_uiLength%3458AAEB01DC
      //	This unsigned attribute holds the maximum number of entries in the array.
      //## begin M4ClosedArray::m_uiLength%3458AAEB01DC.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_uiLength;
      //## end M4ClosedArray::m_uiLength%3458AAEB01DC.attr

      //## Attribute: m_uiCount%3459C174033C
      //	This unsigned attribute holds the number of insertions performed upon the
      //	closed array.
      //## begin M4ClosedArray::m_uiCount%3459C174033C.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_uiCount;
      //## end M4ClosedArray::m_uiCount%3459C174033C.attr

      //## Attribute: m_pContainer%3458AE9A00E2
      //	This attribute points to an array of entries.
      //## begin M4ClosedArray::m_pContainer%3458AE9A00E2.attr preserve=no  protected: Val * {UA} 
      Val *m_pContainer;
      //## end M4ClosedArray::m_pContainer%3458AE9A00E2.attr

  private:
  private: //## implementation
};

//## Class: M4TermAvg%3458A1A2018E; Parameterized Class
//	This abstract template class provides the functionality to store a stream of
//	samples to obtain average values.
//	The public interface of M4TermAvg consist of mechanisms to store new samples
//	and update average data and methods to retrieve average values of the last n
//	samples.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3458AA6A01EB;m4puint16_t { -> }

template <class Val>
class M4TermAvg 
{
  public:
    //## Constructors (specified)
      //## Operation: M4TermAvg%878205122; C++
      //	Parameterized constructor for class M4TermAvg.
      M4TermAvg (m4size_t ai_uiNEntries = 0	// Upperbound for the array.
      );

    //## Destructor (specified)
      //## Operation: ~M4TermAvg%915791253
      virtual ~M4TermAvg ();


    //## Other Operations (specified)
      //## Operation: GetAvg%878294081; C++
      //	Retrives the average value for the last ai_uiDelta samples.
      //	Return Value
      //	The mean value if ai_uiDelta is valid or the global mean otherwise.
      virtual Val GetAvg (m4size_t ai_uiDelta = 0, 	// Number of samples upon which the mean value is to be computed.
      m4size_t ai_uUnitSize = 1) = 0;

      //## Operation: Update%878205124; C++
      //	Adds a new sample and update the objects mean value.
      //	Return Values
      //	Returns the updated mean.
      virtual Val Update (Val &ai_uNewSample) = 0;

      //## Operation: GetSize%878294076; C++
      //	Retrieves the total number of samples inserted in the object.
      //	Return Values
      //	Returns the number of elements.
      virtual m4size_t GetSize () const = 0;

      //## Operation: GetUpperBound%878294079; C++
      //	Returns the maximun number of samples that can be stored in the objects
      //	container.
      m4size_t GetUpperBound () const;

      //## Operation: Reset%879851140
      //	Resets the data stored and computed in the statistics object.
      virtual void Reset ();

  protected:

    //## Other Operations (specified)
      //## Operation: _ExtendAvg%878567807; C++
      Val _ExtendAvg (Val &ai_lastAvg, m4size_t ai_uiSize, Val &ai_newItem);

      //## Operation: _SlideAvg%878567808; C++
      Val _SlideAvg (Val &ai_lastAvg, m4size_t ai_uiSize, Val &ai_oldItem, Val &ai_newItem);

      //## Operation: _SumAvg%933172011; C++
      Val _SumAvg (Val &ai_1stAvg, m4size_t ai_uiSize1, Val &ai_2ndAvg, m4size_t ai_uiSize2);

    // Data Members for Class Attributes

      //## Attribute: m_uLastAvg%3458A70F02A7
      //	This unsigned attribute holds the average value updated with the latest
      //	insertion
      //## begin M4TermAvg::m_uLastAvg%3458A70F02A7.attr preserve=no  protected: Val {UA} 0
      Val m_uLastAvg;
      //## end M4TermAvg::m_uLastAvg%3458A70F02A7.attr

      //## Attribute: m_uiLength%3458A728021A
      //	This unsigned attribute holds the maximum number of entries in the container.
      //## begin M4TermAvg::m_uiLength%3458A728021A.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_uiLength;
      //## end M4TermAvg::m_uiLength%3458A728021A.attr

  private:
  private: //## implementation
};

//## Class: M4PeriodicTermAvg%3459AE9201F4; Parameterized Class
//	This abstract template class provides the functionality to store a stream of
//	samples to obtain average values. The container structure for this class
//	behaves as a Shift Register appending new data whenever the maximun number
//	of elements has been reached.
//	The public interface of M4PeriodicTermAvg provides new operations for
//	managing periodic samples. When the total number of elements stored in the
//	object is multiple of the maximum size, a new average is computed
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class Val>
class M4PeriodicTermAvg : public M4TermAvg<Val>  //## Inherits: <unnamed>%3459AE920271
{
  public:
    //## Constructors (specified)
      //## Operation: M4PeriodicTermAvg%878294071; C++
      //	Parameterized constructor for class M4PeriodicTermAvg.
      M4PeriodicTermAvg (m4size_t ai_uiPeriod = 0);

    //## Destructor (specified)
      //## Operation: ~M4PeriodicTermAvg%915791254
      ~M4PeriodicTermAvg ();


    //## Other Operations (specified)
      //## Operation: GetAvg%878205123; C++
      //	Retrives the average value for the last ai_uiDelta samples.
      //	Return Value
      //	The mean value if ai_uiDelta is valid or the global mean otherwise.
      virtual Val GetAvg (m4size_t ai_uiDelta = 0, m4size_t ai_uUnitSize = 1);

      //## Operation: GetAvgFromInit%878294073; C++
      //	Retrives the average value for the last ai_uiDelta samples since the last
      //	start of the period.
      //	Return Value
      //	The mean value if ai_uiDelta is valid or the global mean otherwise.
      Val GetAvgFromInit ();

      //## Operation: Update%878294074; C++
      //	Adds a new sample and update the objects mean value.
      //	Return Values
      //	Returns the updated mean
      Val Update (Val &ai_uNewSample);

      //## Operation: GetSize%878294082; C++
      //	Retrieves the total number of samples inserted in the object.
      //	Return Values
      //	Returns the number of elements.
      m4size_t GetSize () const;

      //## Operation: GetSizeFromInit%878294077; C++
      //	Retrieves the number of samples inserted in the object since the start of
      //	the last cycle.
      //	Return Values
      //	Returns the number of elements.
      m4size_t GetSizeFromInit () const;

      //## Operation: Reset%879851141
      //	Resets the data stored and computed in the statistics object.
      virtual void Reset ();

      //## Operation: Initialize%932983273; C++
      m4return_t Initialize (m4size_t ai_uiNEntries = 0	// Upperbound for the array.
      );

  protected:

    //## Other Operations (specified)
      //## Operation: _Avg%878205125
      Val _Avg (m4size_t _start, m4size_t _end);

      //## Operation: _InterpolateAvg%933172012; C++
      Val _InterpolateAvg (m4size_t _start, m4size_t _offset, m4size_t _unitsize);

    // Data Members for Class Attributes

      //## Attribute: m_uLastAvgFromInit%3459B4FA00DA
      //	This unsigned attribute holds the average value from the begining of the
      //	term updated with the latest insertion.
      //## begin M4PeriodicTermAvg::m_uLastAvgFromInit%3459B4FA00DA.attr preserve=no  protected: Val {UA} 0
      Val m_uLastAvgFromInit;
      //## end M4PeriodicTermAvg::m_uLastAvgFromInit%3459B4FA00DA.attr

  private:
    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%3458A3F803A1
      //## Role: M4PeriodicTermAvg::m_oContainer%3458A3F90259
      //	Container for values involved in the calculation of the mean value.
      //## begin M4PeriodicTermAvg::m_oContainer%3458A3F90259.role preserve=no  private: M4ClosedArray {1 -> 1VHAN}
      M4ClosedArray<Val> m_oContainer;
      //## end M4PeriodicTermAvg::m_oContainer%3458A3F90259.role

  private: //## implementation
};

//## Class: M4FixedTermAvg%379C2FD802F3; Parameterized Class
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class Val>
class M4FixedTermAvg : public M4TermAvg<Val>  //## Inherits: <unnamed>%379C30030237
{
  public:
    //## Constructors (specified)
      //## Operation: M4FixedTermAvg%932983263; C++
      M4FixedTermAvg (m4size_t ai_uPrecision, m4size_t ai_uMaxLength);

    //## Destructor (specified)
      //## Operation: ~M4FixedTermAvg%932983264
      ~M4FixedTermAvg ();


    //## Other Operations (specified)
      //## Operation: GetAvg%932983265; C++
      //	Retrives the average value for the last ai_uiDelta samples.
      //	Return Value
      //	The mean value if ai_uiDelta is valid or the global mean otherwise.
      Val GetAvg (m4size_t ai_uiDelta = 0, 	// Number of samples upon which the mean value is to be computed.
      m4size_t ai_uUnitSize = 1);

      //## Operation: Update%932983266; C++
      //	Adds a new sample and update the objects mean value.
      //	Return Values
      //	Returns the updated mean.
      Val Update (Val &ai_uNewSample);

      //## Operation: GetSize%932983267; C++
      //	Retrieves the total number of samples inserted in the object.
      //	Return Values
      //	Returns the number of elements.
      m4size_t GetSize () const;

      //## Operation: GetUpperBound%932983268; C++
      //	Returns the maximun number of samples that can be stored in the objects
      //	container.
      m4size_t GetUpperBound () const;

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%379C31370168
      //## Role: M4FixedTermAvg::m_oExactAvg%379C313702A8
      //## begin M4FixedTermAvg::m_oExactAvg%379C313702A8.role preserve=no  protected: M4PeriodicTermAvg { -> 1VHAN}
      M4PeriodicTermAvg <Val> m_oExactAvg;
      //## end M4FixedTermAvg::m_oExactAvg%379C313702A8.role

      //## Association: M4ServerBase::Statistics::<unnamed>%379C31830199
      //## Role: M4FixedTermAvg::m_oDerivedAvg%379C31830353
      //## begin M4FixedTermAvg::m_oDerivedAvg%379C31830353.role preserve=no  protected: M4PeriodicTermAvg { -> 1VHAN}
      M4PeriodicTermAvg <Val> m_oDerivedAvg;
      //## end M4FixedTermAvg::m_oDerivedAvg%379C31830353.role

  private:
  private: //## implementation
};

// Parameterized Class M4ClosedArray 


//## Other Operations (inline)
template <class Val>
inline m4size_t M4ClosedArray<Val>::Size () const
{
  //## begin M4ClosedArray::Size%878205117.body preserve=yes
	if (m_bFull)
		return m_uiLength;
	else
		return m_uiLast;
  //## end M4ClosedArray::Size%878205117.body
}

template <class Val>
inline m4size_t M4ClosedArray<Val>::GetHistory () const
{
  //## begin M4ClosedArray::GetHistory%878294075.body preserve=yes
	return m_uiCount;
  //## end M4ClosedArray::GetHistory%878294075.body
}

template <class Val>
inline m4size_t M4ClosedArray<Val>::UpperBound () const
{
  //## begin M4ClosedArray::UpperBound%932983271.body preserve=yes
	return m_uiLength;
  //## end M4ClosedArray::UpperBound%932983271.body
}

// Parameterized Class M4TermAvg 


//## Other Operations (inline)
template <class Val>
inline m4size_t M4TermAvg<Val>::GetUpperBound () const
{
  //## begin M4TermAvg::GetUpperBound%878294079.body preserve=yes
	if (0 == m_uiLength)
		return 0xffff;

	return m_uiLength;
  //## end M4TermAvg::GetUpperBound%878294079.body
}

// Parameterized Class M4PeriodicTermAvg 


//## Other Operations (inline)
template <class Val>
inline m4size_t M4PeriodicTermAvg<Val>::GetSize () const
{
  //## begin M4PeriodicTermAvg::GetSize%878294082.body preserve=yes
	if (0 == m_uiLength)
		return 0xffff;

	return m_oContainer.Size();
  //## end M4PeriodicTermAvg::GetSize%878294082.body
}

template <class Val>
inline m4size_t M4PeriodicTermAvg<Val>::GetSizeFromInit () const
{
  //## begin M4PeriodicTermAvg::GetSizeFromInit%878294077.body preserve=yes
	m4size_t		_sizeFromInit, _history;

	if (0 == m_uiLength)
		return 0xffff;

	_history = m_oContainer.GetHistory();
	if (0 == _history)
		return 0;
	_sizeFromInit = _history % m_uiLength;
	if (0 == _sizeFromInit)
		return m_uiLength;
	else
		return _sizeFromInit;
  //## end M4PeriodicTermAvg::GetSizeFromInit%878294077.body
}

// Parameterized Class M4FixedTermAvg 


//## Other Operations (inline)
template <class Val>
inline m4size_t M4FixedTermAvg<Val>::GetSize () const
{
  //## begin M4FixedTermAvg::GetSize%932983267.body preserve=yes
	m4size_t	uSize, uUpperbound;


	uUpperbound = m_oExactAvg.GetUpperBound();
	uSize = uUpperbound * m_oDerivedAvg.GetSize();
	if (0 == uSize)
		uSize = m_oExactAvg.GetSize();
	else
		uSize += m_oExactAvg.GetSizeFromInit();

	return uSize;
  //## end M4FixedTermAvg::GetSize%932983267.body
}

template <class Val>
inline m4size_t M4FixedTermAvg<Val>::GetUpperBound () const
{
  //## begin M4FixedTermAvg::GetUpperBound%932983268.body preserve=yes
	m4size_t	uSize;

	uSize = m_oExactAvg.GetUpperBound();
	uSize += uSize * m_oDerivedAvg.GetUpperBound();

	return uSize;
  //## end M4FixedTermAvg::GetUpperBound%932983268.body
}

// Parameterized Class M4ClosedArray 







template <class Val>
M4ClosedArray<Val>::M4ClosedArray ()
  //## begin M4ClosedArray::M4ClosedArray%932983270.hasinit preserve=no
      : m_bFull(M4_FALSE), m_uiLast(0), m_uiLength(0), m_uiCount(0)
  //## end M4ClosedArray::M4ClosedArray%932983270.hasinit
  //## begin M4ClosedArray::M4ClosedArray%932983270.initialization preserve=yes
	, m_pContainer(NULL)
  //## end M4ClosedArray::M4ClosedArray%932983270.initialization
{
  //## begin M4ClosedArray::M4ClosedArray%932983270.body preserve=yes
  //## end M4ClosedArray::M4ClosedArray%932983270.body
}

template <class Val>
M4ClosedArray<Val>::M4ClosedArray (m4size_t ai_uiLength)
  //## begin M4ClosedArray::M4ClosedArray%878205112.hasinit preserve=no
      : m_bFull(M4_FALSE), m_uiLast(0), m_uiLength(0), m_uiCount(0)
  //## end M4ClosedArray::M4ClosedArray%878205112.hasinit
  //## begin M4ClosedArray::M4ClosedArray%878205112.initialization preserve=yes
  //## end M4ClosedArray::M4ClosedArray%878205112.initialization
{
  //## begin M4ClosedArray::M4ClosedArray%878205112.body preserve=yes
	m_uiLength = ai_uiLength;
	m_pContainer = new Val[m_uiLength];
  //## end M4ClosedArray::M4ClosedArray%878205112.body
}


template <class Val>
M4ClosedArray<Val>::~M4ClosedArray ()
{
  //## begin M4ClosedArray::~M4ClosedArray%878205126.body preserve=yes
	delete []m_pContainer;
  //## end M4ClosedArray::~M4ClosedArray%878205126.body
}



//## Other Operations (implementation)
template <class Val>
Val & M4ClosedArray<Val>::operator [] (m4size_t ai_uiEntry)
{
  //## begin M4ClosedArray::operator[]%878205113.body preserve=yes
	m4size_t		_firstItem;

	if (0 == ai_uiEntry || ai_uiEntry > m_uiLength)
		return m_pContainer[m_uiLast];

	_firstItem = _First();
	_firstItem += ai_uiEntry;
	_firstItem = _firstItem % m_uiLength;
	
	return m_pContainer[_firstItem];
  //## end M4ClosedArray::operator[]%878205113.body
}

template <class Val>
void M4ClosedArray<Val>::Append (Val &ai_uValue)
{
  //## begin M4ClosedArray::Append%878205114.body preserve=yes
	m_uiLast = _Next();
	m_pContainer[m_uiLast] = ai_uValue;
	m_uiCount++;
  //## end M4ClosedArray::Append%878205114.body
}

template <class Val>
Val & M4ClosedArray<Val>::First ()
{
  //## begin M4ClosedArray::First%878205115.body preserve=yes
	m4size_t	_first;

	_first = _First();
	return m_pContainer[_first];
  //## end M4ClosedArray::First%878205115.body
}

template <class Val>
Val & M4ClosedArray<Val>::Last ()
{
  //## begin M4ClosedArray::Last%878205116.body preserve=yes
	return m_pContainer[m_uiLast];
  //## end M4ClosedArray::Last%878205116.body
}

template <class Val>
m4size_t M4ClosedArray<Val>::_Next ()
{
  //## begin M4ClosedArray::_Next%878205127.body preserve=yes
	m4size_t	lastCopy = m_uiLast;
	
	lastCopy++;
	if (lastCopy == m_uiLength)
	{
		m_bFull = M4_TRUE;
		return 0;
	}
	else
		return lastCopy;
  //## end M4ClosedArray::_Next%878205127.body
}

template <class Val>
m4size_t M4ClosedArray<Val>::_First ()
{
  //## begin M4ClosedArray::_First%878205128.body preserve=yes
	m4size_t	lastCopy = m_uiLast;

	lastCopy++;
	if (!m_bFull || lastCopy == m_uiLength)
		return 0;
	else 
		return lastCopy;
  //## end M4ClosedArray::_First%878205128.body
}

template <class Val>
void M4ClosedArray<Val>::Reset ()
{
  //## begin M4ClosedArray::Reset%879851142.body preserve=yes
  //## end M4ClosedArray::Reset%879851142.body
}

template <class Val>
m4return_t M4ClosedArray<Val>::SetSize (m4size_t ai_uSize)
{
  //## begin M4ClosedArray::SetSize%932983269.body preserve=yes
	if (NULL != m_pContainer || 0 != m_uiLength || 0 == ai_uSize)
		return M4_ERROR;

	m_uiLength = ai_uSize;
	m_pContainer = new Val[m_uiLength];

	return M4_SUCCESS;
  //## end M4ClosedArray::SetSize%932983269.body
}

// Parameterized Class M4TermAvg 



template <class Val>
M4TermAvg<Val>::M4TermAvg (m4size_t ai_uiNEntries)
  //## begin M4TermAvg::M4TermAvg%878205122.hasinit preserve=no
      : m_uLastAvg(0), m_uiLength(0)
  //## end M4TermAvg::M4TermAvg%878205122.hasinit
  //## begin M4TermAvg::M4TermAvg%878205122.initialization preserve=yes
  //## end M4TermAvg::M4TermAvg%878205122.initialization
{
  //## begin M4TermAvg::M4TermAvg%878205122.body preserve=yes
	m_uiLength = ai_uiNEntries;
  //## end M4TermAvg::M4TermAvg%878205122.body
}


template <class Val>
M4TermAvg<Val>::~M4TermAvg ()
{
  //## begin M4TermAvg::~M4TermAvg%915791253.body preserve=yes
  //## end M4TermAvg::~M4TermAvg%915791253.body
}



//## Other Operations (implementation)
template <class Val>
Val M4TermAvg<Val>::_ExtendAvg (Val &ai_lastAvg, m4size_t ai_uiSize, Val &ai_newItem)
{
  //## begin M4TermAvg::_ExtendAvg%878567807.body preserve=yes
	Val		_newAvg;

	if (0 == m_uiLength)
		return (Val)0;

	_newAvg = (ai_uiSize++) * ai_lastAvg;
	_newAvg += ai_newItem;
	_newAvg /= ai_uiSize;

	return _newAvg;
  //## end M4TermAvg::_ExtendAvg%878567807.body
}

template <class Val>
Val M4TermAvg<Val>::_SlideAvg (Val &ai_lastAvg, m4size_t ai_uiSize, Val &ai_oldItem, Val &ai_newItem)
{
  //## begin M4TermAvg::_SlideAvg%878567808.body preserve=yes
	Val		_newAvg;

	if (0 == m_uiLength)
		return (Val)0;

	if (ai_newItem < ai_oldItem)
		_newAvg = ai_oldItem - ai_newItem;
	else
		_newAvg = ai_newItem - ai_oldItem;
	_newAvg /= ai_uiSize;
	if (ai_newItem < ai_oldItem)
		_newAvg = ai_lastAvg - _newAvg;
	else
		_newAvg += ai_lastAvg;

	return _newAvg;
  //## end M4TermAvg::_SlideAvg%878567808.body
}

template <class Val>
Val M4TermAvg<Val>::_SumAvg (Val &ai_1stAvg, m4size_t ai_uiSize1, Val &ai_2ndAvg, m4size_t ai_uiSize2)
{
  //## begin M4TermAvg::_SumAvg%933172011.body preserve=yes
	Val			_Sum1, _Sum2, _Total;
	m4size_t	uSize;

	_Sum1 = (Val)ai_1stAvg * ai_uiSize1;
	_Sum2 = (Val)ai_2ndAvg * ai_uiSize2;
	uSize = (ai_uiSize1 + ai_uiSize2);
	if (0 == uSize)
		_Total = (Val)0;
	else
		_Total = (_Sum1 + _Sum2)/(Val)uSize;

	return _Total;
  //## end M4TermAvg::_SumAvg%933172011.body
}

template <class Val>
void M4TermAvg<Val>::Reset ()
{
  //## begin M4TermAvg::Reset%879851140.body preserve=yes
  //## end M4TermAvg::Reset%879851140.body
}

// Parameterized Class M4PeriodicTermAvg 






template <class Val>
M4PeriodicTermAvg<Val>::M4PeriodicTermAvg (m4size_t ai_uiPeriod)
  //## begin M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.hasinit preserve=no
      : m_uLastAvgFromInit(0)
  //## end M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.hasinit
  //## begin M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.initialization preserve=yes
  //## end M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.initialization
{
  //## begin M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.body preserve=yes
	if (0 != ai_uiPeriod)
		Initialize(ai_uiPeriod);
  //## end M4PeriodicTermAvg::M4PeriodicTermAvg%878294071.body
}


template <class Val>
M4PeriodicTermAvg<Val>::~M4PeriodicTermAvg ()
{
  //## begin M4PeriodicTermAvg::~M4PeriodicTermAvg%915791254.body preserve=yes
  //## end M4PeriodicTermAvg::~M4PeriodicTermAvg%915791254.body
}



//## Other Operations (implementation)
template <class Val>
Val M4PeriodicTermAvg<Val>::_Avg (m4size_t _start, m4size_t _end)
{
  //## begin M4PeriodicTermAvg::_Avg%878205125.body preserve=yes
	Val			_sum = 0;

	if (0 == m_uiLength)
		return (Val)0;

	if (_start > _end || _end > m_uiLength)
		return m_uLastAvg;

	if (_start == _end)
		return m_oContainer[_end];

	m4size_t	_dist = _end -_start;
	for (m4size_t i = _start; i < _end; i++)
		_sum += m_oContainer[i];

	_sum /= (Val)_dist;

	return _sum;
  //## end M4PeriodicTermAvg::_Avg%878205125.body
}

template <class Val>
Val M4PeriodicTermAvg<Val>::_InterpolateAvg (m4size_t _start, m4size_t _offset, m4size_t _unitsize)
{
  //## begin M4PeriodicTermAvg::_InterpolateAvg%933172012.body preserve=yes
	m4size_t	uSize;	
	Val			_avg1, _avg2, _linealInter;

	if (0 == m_uiLength)
		return (Val)0;

	uSize = m_oContainer.Size();
	if (_start > uSize)
		return (Val)0;

	if (_start == uSize)
		return m_oContainer[_start];

	_avg1 = m_oContainer[_start];
	_avg2 = m_oContainer[_start + 1];
	_linealInter = (_avg2 -_avg1)/_unitsize;
	_linealInter *= _offset;
	_linealInter += _avg1;

	return _linealInter;
  //## end M4PeriodicTermAvg::_InterpolateAvg%933172012.body
}

template <class Val>
Val M4PeriodicTermAvg<Val>::GetAvg (m4size_t ai_uiDelta, m4size_t ai_uUnitSize)
{
  //## begin M4PeriodicTermAvg::GetAvg%878205123.body preserve=yes
	m4size_t	size, qDelta, rDelta;
	Val			_TotAvg, _iDelta;

	if (0 == m_uiLength)
		return (Val)0;

	if (0 == ai_uiDelta)
		return m_uLastAvg;

	if (0 == ai_uUnitSize)
	{
		qDelta = ai_uiDelta;
		rDelta = 0;
	}
	else
	{
		qDelta = ai_uiDelta / ai_uUnitSize;
		rDelta = ai_uiDelta % ai_uUnitSize;
	}

	size = m_oContainer.Size();
	if (qDelta > size)
		return m_uLastAvg;

	qDelta = size - qDelta;
	_TotAvg = _Avg(qDelta, size);

	if (qDelta < size && 0 != rDelta)
	{
		rDelta = ai_uUnitSize - rDelta;
		_iDelta = _InterpolateAvg(qDelta, rDelta, ai_uUnitSize);
		_TotAvg = _SumAvg(_TotAvg, size-qDelta, _iDelta, rDelta);
	}
	
	return _TotAvg;
  //## end M4PeriodicTermAvg::GetAvg%878205123.body
}

template <class Val>
Val M4PeriodicTermAvg<Val>::GetAvgFromInit ()
{
  //## begin M4PeriodicTermAvg::GetAvgFromInit%878294073.body preserve=yes
	if (0 == m_uiLength)
		return (Val)0;

	return m_uLastAvgFromInit;
  //## end M4PeriodicTermAvg::GetAvgFromInit%878294073.body
}

template <class Val>
Val M4PeriodicTermAvg<Val>::Update (Val &ai_uNewSample)
{
  //## begin M4PeriodicTermAvg::Update%878294074.body preserve=yes
	m4size_t	size, history;
	Val			first;

	if (0 == m_uiLength)
		return (Val)0;

	history = m_oContainer.GetHistory() % m_uiLength;
	size = m_oContainer.Size();
	if (0 == history)
	{
		m_uLastAvgFromInit = ai_uNewSample;
	}
	else
	{
		m_uLastAvgFromInit = _ExtendAvg(m_uLastAvgFromInit, history, ai_uNewSample);
	}
	
	if (size < m_uiLength)
	{
		m_uLastAvg = m_uLastAvgFromInit;
	}
	else
	{
		first = m_oContainer.First();

		m_uLastAvg = _SlideAvg(m_uLastAvg, m_uiLength, first, ai_uNewSample);
	}
	
	m_oContainer.Append(ai_uNewSample);

	return m_uLastAvg;
  //## end M4PeriodicTermAvg::Update%878294074.body
}

template <class Val>
void M4PeriodicTermAvg<Val>::Reset ()
{
  //## begin M4PeriodicTermAvg::Reset%879851141.body preserve=yes
  //## end M4PeriodicTermAvg::Reset%879851141.body
}

template <class Val>
m4return_t M4PeriodicTermAvg<Val>::Initialize (m4size_t ai_uiNEntries)
{
  //## begin M4PeriodicTermAvg::Initialize%932983273.body preserve=yes
	if (0 != m_uiLength)
		return M4_ERROR;

	m_uiLength = ai_uiNEntries;
	return m_oContainer.SetSize(ai_uiNEntries);
  //## end M4PeriodicTermAvg::Initialize%932983273.body
}

// Parameterized Class M4FixedTermAvg 



template <class Val>
M4FixedTermAvg<Val>::M4FixedTermAvg (m4size_t ai_uPrecision, m4size_t ai_uMaxLength)
  //## begin M4FixedTermAvg::M4FixedTermAvg%932983263.hasinit preserve=no
  //## end M4FixedTermAvg::M4FixedTermAvg%932983263.hasinit
  //## begin M4FixedTermAvg::M4FixedTermAvg%932983263.initialization preserve=yes
  :	M4TermAvg<Val> (), m_oExactAvg(ai_uPrecision)
  //## end M4FixedTermAvg::M4FixedTermAvg%932983263.initialization
{
  //## begin M4FixedTermAvg::M4FixedTermAvg%932983263.body preserve=yes
	if ((ai_uMaxLength > ai_uPrecision) && (0 == (ai_uMaxLength % ai_uPrecision)))
	{
		m_oDerivedAvg.Initialize(ai_uMaxLength / ai_uPrecision);
		m_uiLength = ai_uMaxLength + ai_uPrecision;
	}
  //## end M4FixedTermAvg::M4FixedTermAvg%932983263.body
}


template <class Val>
M4FixedTermAvg<Val>::~M4FixedTermAvg ()
{
  //## begin M4FixedTermAvg::~M4FixedTermAvg%932983264.body preserve=yes
  //## end M4FixedTermAvg::~M4FixedTermAvg%932983264.body
}



//## Other Operations (implementation)
template <class Val>
Val M4FixedTermAvg<Val>::GetAvg (m4size_t ai_uiDelta, m4size_t ai_uUnitSize)
{
  //## begin M4FixedTermAvg::GetAvg%932983265.body preserve=yes
	m4size_t	size, upperBound, dDelta;
	Val				_Total,
					_ExactTotal,
					_DerivedTotal;

	if (0 == m_uiLength || 1 != ai_uUnitSize)
		return (Val)0;

	size = GetSize();
	if (0 == ai_uiDelta || size < ai_uiDelta)
		return m_uLastAvg;

	size = m_oExactAvg.GetSize();
	if (ai_uiDelta <= size)
		_Total = m_oExactAvg.GetAvg(ai_uiDelta);
	else
	{
		_ExactTotal = m_oExactAvg.GetAvgFromInit();
		size = m_oExactAvg.GetSizeFromInit();

		dDelta = ai_uiDelta - size;
		upperBound = m_oExactAvg.GetUpperBound(); 
		_DerivedTotal = m_oDerivedAvg.GetAvg(dDelta, upperBound);
		_Total = _SumAvg(_ExactTotal, size, _DerivedTotal, dDelta);
	}
	
	return _Total;
  //## end M4FixedTermAvg::GetAvg%932983265.body
}

template <class Val>
Val M4FixedTermAvg<Val>::Update (Val &ai_uNewSample)
{
  //## begin M4FixedTermAvg::Update%932983266.body preserve=yes
	m4size_t		uSize1, uSize2, uUpperBound;
	Val				_PrevExactTotal,
					_ExactTotal,
					_DerivedTotal;

	uSize1 = m_oExactAvg.GetSize();
	uUpperBound = m_oExactAvg.GetUpperBound();
	if (uSize1 < uUpperBound)
		m_uLastAvg = m_oExactAvg.Update(ai_uNewSample);
	else
	{
		uSize1 = m_oExactAvg.GetSizeFromInit();
		if (uSize1 == uUpperBound)
		{
			_PrevExactTotal = m_oExactAvg.GetAvg();
			_ExactTotal = m_oExactAvg.Update(ai_uNewSample);
			_DerivedTotal = m_oDerivedAvg.Update(_PrevExactTotal);
			uSize2 = uUpperBound * m_oDerivedAvg.GetSize();
			m_uLastAvg = _SumAvg(ai_uNewSample, 1, _DerivedTotal, uSize2);
		}
		else
		{
			_ExactTotal = m_oExactAvg.Update(ai_uNewSample);
			uSize2 = GetSize();
			m_uLastAvg = _ExtendAvg(m_uLastAvg, uSize2 - 1 , ai_uNewSample);
		}
	}

	return m_uLastAvg;
  //## end M4FixedTermAvg::Update%932983266.body
}

//## begin module%345898D60074.epilog preserve=yes
//## end module%345898D60074.epilog


#endif
