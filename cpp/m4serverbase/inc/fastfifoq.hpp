//## begin module%34FAF55A0111.cm preserve=no
//## end module%34FAF55A0111.cm

//## begin module%34FAF55A0111.cp preserve=no
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
//## end module%34FAF55A0111.cp

//## Module: FastQ%34FAF55A0111; Package specification
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Source file: D:\eduardoma\m4serverbase\inc\fastfifoq.hpp

#ifndef fastq_h
#define fastq_h 1

//## begin module%34FAF55A0111.additionalIncludes preserve=no
//## end module%34FAF55A0111.additionalIncludes

//## begin module%34FAF55A0111.includes preserve=yes
//## end module%34FAF55A0111.includes

// m4types
#include <m4types.hpp>
// boolcondition
#include <boolcondition.hpp>
//## begin module%34FAF55A0111.declarations preserve=no
//## end module%34FAF55A0111.declarations

//## begin module%34FAF55A0111.additionalDeclarations preserve=yes
const m4uint32_t	FastDefUpperBound = 1024;
//## end module%34FAF55A0111.additionalDeclarations


//## begin FastQueue%34ED665000E2.preface preserve=yes
//## end FastQueue%34ED665000E2.preface

//## Class: FastQueue%34ED665000E2; Parameterized Class
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3745539702F1;m4return_t { -> }

template <class _Ty>
class FastQueue 
{
  //## begin FastQueue%34ED665000E2.initialDeclarations preserve=yes
  //## end FastQueue%34ED665000E2.initialDeclarations

  public:
    //## Class: value_t%34ED6811016F
    //## Category: M4ServerBase::ServerQ%341D51F4022A
    //## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef _Ty value_t;

    //## Class: size_t%34ED6848016F
    //## Category: M4ServerBase::ServerQ%341D51F4022A
    //## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef m4uint32_t size_t;

    //## Constructors (specified)
      //## Operation: FastQueue%888226321; C++
      //	Parameterized constructor for class AutoQueue.
      //	Parameters
      //	ai_lUpperBound: A m4uint16_t value to establish an upper bound on the number
      //	of elements to be stored by the queue.
      explicit FastQueue (const m4size_t ai_lUpperBound = FastDefUpperBound);

    //## Destructor (specified)
      //## Operation: ~FastQueue%887968981; C++
      ~FastQueue ();


    //## Other Operations (specified)
      //## Operation: Push%888853673; C++
      //	Inserts a new element at the back of the queue.
      //
      //	Parameters
      //	ao_pValue_type: A reference to a variable of class _Ty to push the last
      //	element.
      //	ai_uiTimeOut: Tiempo máximo de espera en la cola para poner un elemento. Si
      //	vale 0 la espera es infinita.
      //	Return Values:
      //	M4_SUCCESS: Se pone el valor
      //	M4_WARNNING: Se ha producido un time_out
      //	M4_ERROR: La cola esta bloqueada
      //
      //	Remarks
      //	This function is intended to block if the queue is full.
      m4return_t Push (const _Ty &ai_pValueType, m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Push%932544247; C++
      //	Inserts a new element at the back of the queue.
      //
      //	Parameters
      //	ao_pValue_type: A reference to a variable of class _Ty to push the last
      //	element.
      //	ai_bLockAware: Si vale M4_TRUE no ponemos el valor si la cola esta bloqueada.
      //	ai_uiTimeOut: Tiempo máximo de espera en la cola para poner un elemento. Si
      //	vale 0 la espera es infinita.
      //
      //	Return Values:
      //	M4_SUCCESS: Se pone el valor
      //	M4_WARNNING: Se ha producido un time_out
      //	M4_ERROR: La cola esta bloqueada
      //
      //	Remarks
      //	This function is intended to block if the queue is full.
      m4return_t Push (const _Ty &ai_pValueType, m4bool_t ai_bLockAware, m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Pop%888853675; C++
      //	Removes the first element in the queue.
      //
      //	Parameters
      //	ao_pValue_type: A reference to a variable of class _Ty to hold the first
      //	element.
      //	ai_uiTimeOut: Tiempo máximo de espera en la cola por un elemento. Si vale 0
      //	la espera es infinita.
      //	Return Values:
      //	M4_SUCCESS: Se devuelve un valor
      //	M4_WARNNING: Se ha producido un time_out
      //	M4_ERROR: La cola esta bloqueada
      //
      //	Remarks
      //	This function is intended to block if the queue is empty.
      m4return_t Pop (_Ty &ao_pValueType, m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Pop%932544248; C++
      //	Removes the first element in the queue.
      //
      //	Parameters
      //	ao_pValue_type: A reference to a variable of class _Ty to hold the first
      //	element.
      //	ai_bLockAware: Si vale M4_TRUE no recogemos el valor si la cola esta
      //	bloqueada.
      //	ai_uiTimeOut: Tiempo máximo de espera en la cola por un elemento. Si vale 0
      //	la espera es infinita.
      //	Return Values:
      //	M4_SUCCESS: Se devuelve un valor
      //	M4_WARNNING: Se ha producido un time_out
      //	M4_ERROR: La cola esta bloqueada
      //
      //	Remarks
      //	This function is intended to block if the queue is empty.
      m4return_t Pop (_Ty &ao_pValueType, m4bool_t ai_bLockAware, m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Size%888853677; C++
      //	Returns the number of elements in the queue.
      //	Return Values
      //	The number of elements in the list. This value must be of class queue<_
      //	Ty>::size_type.
      //	Remarks
      //	No remarks.
      m4uint32_t Size ();

      //## Operation: Lock%927279257
      m4return_t Lock ();

      //## Operation: Unlock%927279258
      m4return_t Unlock ();

      //## Operation: IsLocked%927279259
      m4bool_t IsLocked ();

      //## Operation: ReleaseAll%932544249
      m4return_t ReleaseAll ();

    // Data Members for Class Attributes

      //## Attribute: m_lUpperBound%34ED67D50085
      //	This attributes sets an upper bound on the number of elements that the queue
      //	can store.
      //## begin FastQueue::m_lUpperBound%34ED67D50085.attr preserve=no  public: m4uint32_t {UAC} ai_lUpperBound
      const m4uint32_t  m_lUpperBound;
      //## end FastQueue::m_lUpperBound%34ED67D50085.attr

      //## Attribute: M4DefaultSize%3950CA770178
      //## begin FastQueue::M4DefaultSize%3950CA770178.attr preserve=no  public: static m4size_t {VAC} 1024
      static const m4size_t  M4DefaultSize;
      //## end FastQueue::M4DefaultSize%3950CA770178.attr

  public:
    // Additional Public Declarations
      //## begin FastQueue%34ED665000E2.public preserve=yes
      //## end FastQueue%34ED665000E2.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_lFirst%34ED67D602B7
      //	This attributes sets an upper bound on the number of elements that the queue
      //	can store.
      //## begin FastQueue::m_lFirst%34ED67D602B7.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_lFirst;
      //## end FastQueue::m_lFirst%34ED67D602B7.attr

      //## Attribute: m_lLast%34ED67D800C3
      //	This attributes sets an upper bound on the number of elements that the queue
      //	can store.
      //## begin FastQueue::m_lLast%34ED67D800C3.attr preserve=no  protected: m4uint32_t {UA} 0xffffffff
      m4uint32_t m_lLast;
      //## end FastQueue::m_lLast%34ED67D800C3.attr

      //## Attribute: m_bIsLocked%37455E84031B
      //## begin FastQueue::m_bIsLocked%37455E84031B.attr preserve=no  protected: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsLocked;
      //## end FastQueue::m_bIsLocked%37455E84031B.attr

    // Data Members for Associations

      //## Association: M4ServerBase::ServerQ::<unnamed>%36D5735901D7
      //## Role: FastQueue::m_poEmpty%36D5735901D8
      //## begin FastQueue::m_poEmpty%36D5735901D8.role preserve=no  protected: ClBooleanCondition {1 -> 1RHAN}
      ClBooleanCondition *m_poEmpty;
      //## end FastQueue::m_poEmpty%36D5735901D8.role

      //## Association: M4ServerBase::ServerQ::<unnamed>%36D5735C0023
      //## Role: FastQueue::m_poFull%36D5735C0024
      //## begin FastQueue::m_poFull%36D5735C0024.role preserve=no  protected: ClBooleanCondition {1 -> 1RHAN}
      ClBooleanCondition *m_poFull;
      //## end FastQueue::m_poFull%36D5735C0024.role

    // Additional Protected Declarations
      //## begin FastQueue%34ED665000E2.protected preserve=yes
	  ClMutex	m_pUpdate;
      //## end FastQueue%34ED665000E2.protected
  private:
    // Data Members for Class Attributes

      //## Attribute: m_pTy%34ED678E0150
      //## begin FastQueue::m_pTy%34ED678E0150.attr preserve=no  private: _Ty * {UA} NULL
      _Ty *m_pTy;
      //## end FastQueue::m_pTy%34ED678E0150.attr

    // Additional Private Declarations
      //## begin FastQueue%34ED665000E2.private preserve=yes
      //## end FastQueue%34ED665000E2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin FastQueue%34ED665000E2.implementation preserve=yes
      //## end FastQueue%34ED665000E2.implementation

};

//## begin FastQueue%34ED665000E2.postscript preserve=yes
//## end FastQueue%34ED665000E2.postscript

//## Class: FastMixQ%34ED60480382; Parameterized Class
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class _K, class _Ty>
class FastMixQ : public FastQueue<_Ty>  //## Inherits: <unnamed>%34ED610B022B
{
  public:
    //## begin FastMixQ::value_type%34ED61820150.preface preserve=yes
    //## end FastMixQ::value_type%34ED61820150.preface

    //## Class: value_type%34ED61820150
    //## Category: M4ServerBase::ServerQ%341D51F4022A
    //## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef _Ty value_type;

    //## begin FastMixQ::value_type%34ED61820150.postscript preserve=yes
    //## end FastMixQ::value_type%34ED61820150.postscript

    //## begin FastMixQ::key_type%34ED61A40315.preface preserve=yes
    //## end FastMixQ::key_type%34ED61A40315.preface

    //## Class: key_type%34ED61A40315
    //## Category: M4ServerBase::ServerQ%341D51F4022A
    //## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef _K key_type;

    //## begin FastMixQ::key_type%34ED61A40315.postscript preserve=yes
    //## end FastMixQ::key_type%34ED61A40315.postscript

    //## Constructors (specified)
      //## Operation: FastMixQ%887968974; C++
      //	Parameterized constructor for class AutoQueue.
      //	Parameters
      //	ai_lUpperBound: A m4uint16_t value to establish an upper bound on the number
      //	of elements to be stored by the queue.
      explicit FastMixQ (const m4uint32_t ai_lUpperBound = FastDefUpperBound);


    //## Other Operations (specified)
      //## Operation: Push%887968973; C++
      //	Inserts a new element at the back of the queue.
      //	Return Values
      //	No return value.
      //	Remarks
      //	This function is intended to block if the queue is full
      m4return_t Push (const typename FastMixQ<_K,_Ty>::key_type &ai_oKey, typename FastMixQ<_K,_Ty>::value_type &ai_oEntry);

  public:
  protected:
  private:
  private: //## implementation
};

// Parameterized Class FastQueue 

template <class _Ty>
inline FastQueue<_Ty>::FastQueue (const m4size_t ai_lUpperBound)
  //## begin FastQueue::FastQueue%888226321.hasinit preserve=no
      : m_pTy(NULL), m_lUpperBound(ai_lUpperBound), m_lFirst(0), m_lLast(0xffffffff), m_bIsLocked(M4_FALSE), m_poEmpty(NULL), m_poFull(NULL)
  //## end FastQueue::FastQueue%888226321.hasinit
  //## begin FastQueue::FastQueue%888226321.initialization preserve=yes
  //## end FastQueue::FastQueue%888226321.initialization
{
  //## begin FastQueue::FastQueue%888226321.body preserve=yes
	m4return_t	iRet;

	iRet = m_pUpdate.Init();

	m_poFull = new ClBooleanCondition(&m_pUpdate);
	m_poEmpty = new ClBooleanCondition(&m_pUpdate);

	iRet = m_poFull->Init();
	iRet = m_poEmpty->Init();

	m_pTy = new _Ty[m_lUpperBound];
  //## end FastQueue::FastQueue%888226321.body
}


template <class _Ty>
inline FastQueue<_Ty>::~FastQueue ()
{
  //## begin FastQueue::~FastQueue%887968981.body preserve=yes
	if (m_poEmpty) {
		delete m_poEmpty;
		m_poEmpty = NULL;
	}
	if (m_poFull) {
		delete m_poFull;
		m_poFull = NULL;
	}
	if (m_pTy) {
		delete [] m_pTy;
		m_pTy = NULL;
	}
  //## end FastQueue::~FastQueue%887968981.body
}


// Parameterized Class FastMixQ 

template <class _K, class _Ty>
inline FastMixQ<_K,_Ty>::FastMixQ (const m4uint32_t ai_lUpperBound)
  //## begin FastMixQ::FastMixQ%887968974.hasinit preserve=no
  //## end FastMixQ::FastMixQ%887968974.hasinit
  //## begin FastMixQ::FastMixQ%887968974.initialization preserve=yes
  //## end FastMixQ::FastMixQ%887968974.initialization
{
  //## begin FastMixQ::FastMixQ%887968974.body preserve=yes
  //## end FastMixQ::FastMixQ%887968974.body
}


// Parameterized Class FastQueue 






//## begin FastQueue::M4DefaultSize%3950CA770178.attr preserve=no  public: static m4size_t {VAC} 1024
template <class _Ty>
const m4size_t  FastQueue<_Ty>::M4DefaultSize = 1024;
//## end FastQueue::M4DefaultSize%3950CA770178.attr




//## Other Operations (implementation)
template <class _Ty>
m4return_t FastQueue<_Ty>::Push (const _Ty &ai_pValueType, m4uint32_t ai_uiTimeOut)
{
  //## begin FastQueue::Push%888853673.body preserve=yes
	return Push(ai_pValueType, M4_TRUE, ai_uiTimeOut);
  //## end FastQueue::Push%888853673.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::Push (const _Ty &ai_pValueType, m4bool_t ai_bLockAware, m4uint32_t ai_uiTimeOut)
{
  //## begin FastQueue::Push%932544247.body preserve=yes
	m4uint32_t		key;
	m4return_t		ret = M4_SUCCESS;

	m_poFull->Lock();

	while (m_lFirst + m_lUpperBound == ( m_lLast + 1 ))
	{
		if (m_bIsLocked && ai_bLockAware)
		{
			m_poFull->Unlock();
			return	M4_ERROR;
		}
		else {
			ret = m_poFull->Wait(ai_uiTimeOut);
			if (ret	!= M4_SUCCESS) {
				//siempre me despierto con el mutex cogido si no hay error
				if (ret != M4_ERROR)
					m_poFull->Unlock();
				return ret;
			}
		}
	}

	if (m_bIsLocked && ai_bLockAware)
	{
		m_poFull->Unlock();
		return	M4_ERROR;
	}

	key = (++m_lLast) % m_lUpperBound;
		
	m_pTy[key] = ai_pValueType;

	m_poEmpty->Notify();
	m_poFull->Unlock();

	return M4_SUCCESS;
  //## end FastQueue::Push%932544247.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::Pop (_Ty &ao_pValueType, m4uint32_t ai_uiTimeOut)
{
  //## begin FastQueue::Pop%888853675.body preserve=yes
	return Pop (ao_pValueType, M4_TRUE, ai_uiTimeOut);
  //## end FastQueue::Pop%888853675.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::Pop (_Ty &ao_pValueType, m4bool_t ai_bLockAware, m4uint32_t ai_uiTimeOut)
{
  //## begin FastQueue::Pop%932544248.body preserve=yes
	m4uint32_t	key;
	m4return_t ret = M4_SUCCESS;

	m_poEmpty->Lock();

	while ( m_lFirst ==( m_lLast + 1 ))
	{
		if (m_bIsLocked && ai_bLockAware)
		{
			m_poEmpty->Unlock();
			return	M4_ERROR;
		}
		else 
		{
			ret = m_poEmpty->Wait(ai_uiTimeOut);
			if (ret	!= M4_SUCCESS) {
				//siempre me despierto con el mutex cogido si no hay error
				if (ret != M4_ERROR)
					m_poEmpty->Unlock();
				return ret;
			}
		}
	}

	if (m_bIsLocked && ai_bLockAware)
	{
		m_poEmpty->Unlock();
		return	M4_ERROR;
	}

	key = (m_lFirst++) % m_lUpperBound;
	if ( m_lFirst == m_lUpperBound )
	{
		m_lLast -= m_lUpperBound;
		m_lFirst = 0;
	}
	ao_pValueType = m_pTy[key];

	m_poFull->Notify();
	m_poEmpty->Unlock();

	return M4_SUCCESS;
  //## end FastQueue::Pop%932544248.body
}

template <class _Ty>
m4uint32_t FastQueue<_Ty>::Size ()
{
  //## begin FastQueue::Size%888853677.body preserve=yes
	m4uint32_t		size;
	m_pUpdate.Lock();

	size = (m_lLast - m_lFirst) + 1;

	m_pUpdate.Unlock();
	return size;
  //## end FastQueue::Size%888853677.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::Lock ()
{
  //## begin FastQueue::Lock%927279257.body preserve=yes
	m4return_t	ret;

	m_pUpdate.Lock();
	if (m_bIsLocked)
		ret = M4_ERROR;
	else
	{
		m_bIsLocked = M4_TRUE;
		ret = M4_SUCCESS;
	}
	m_pUpdate.Unlock();

	return ret;
  //## end FastQueue::Lock%927279257.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::Unlock ()
{
  //## begin FastQueue::Unlock%927279258.body preserve=yes
	m4return_t	ret;

	m_pUpdate.Lock();
	if (!m_bIsLocked)
		ret = M4_ERROR;
	else
	{
		m_bIsLocked = M4_FALSE;
		ret = M4_SUCCESS;
	}
	m_pUpdate.Unlock();

	return ret;
  //## end FastQueue::Unlock%927279258.body
}

template <class _Ty>
m4bool_t FastQueue<_Ty>::IsLocked ()
{
  //## begin FastQueue::IsLocked%927279259.body preserve=yes
	m4bool_t	ret;

	m_pUpdate.Lock();
	ret = m_bIsLocked;
	m_pUpdate.Unlock();

	return ret;
  //## end FastQueue::IsLocked%927279259.body
}

template <class _Ty>
m4return_t FastQueue<_Ty>::ReleaseAll ()
{
  //## begin FastQueue::ReleaseAll%932544249.body preserve=yes
	m4return_t		iRet = M4_ERROR;

	m_pUpdate.Lock();
	if (M4_TRUE == m_bIsLocked)
	{
		iRet = m_poEmpty->Notify(M4_TRUE);
		if (M4_ERROR == iRet)
			return iRet;
		iRet = m_poFull->Notify(M4_TRUE);
	}

	m_pUpdate.Unlock();

	return iRet;
  //## end FastQueue::ReleaseAll%932544249.body
}

// Additional Declarations
  //## begin FastQueue%34ED665000E2.declarations preserve=yes
  //## end FastQueue%34ED665000E2.declarations

// Parameterized Class FastMixQ 


//## Other Operations (implementation)
template <class _K, class _Ty>
m4return_t FastMixQ<_K,_Ty>::Push (const typename FastMixQ<_K,_Ty>::key_type &ai_oKey, typename FastMixQ<_K,_Ty>::value_type &ai_oEntry)
{
  //## begin FastMixQ::Push%887968973.body preserve=yes
  //## end FastMixQ::Push%887968973.body
}

//## begin module%34FAF55A0111.epilog preserve=yes
//## end module%34FAF55A0111.epilog


#endif
