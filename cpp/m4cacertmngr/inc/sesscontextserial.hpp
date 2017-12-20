//## begin module%3CF4AE200219.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CF4AE200219.cm

//## begin module%3CF4AE200219.cp preserve=no
//## end module%3CF4AE200219.cp

//## Module: sesscontextserial%3CF4AE200219; Package specification
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Source file: F:\integration\m4serverbase\inc\sesscontextserial.h

#ifndef sesscontextserial_h
#define sesscontextserial_h 1

//## begin module%3CF4AE200219.additionalIncludes preserve=no
//## end module%3CF4AE200219.additionalIncludes

//## begin module%3CF4AE200219.includes preserve=yes
#include <m4types.hpp>
#include <m4string.hpp>
#include "m4cacertmngr_dll.hpp"
//## end module%3CF4AE200219.includes

//## begin module%3CF4AE200219.declarations preserve=no
//## end module%3CF4AE200219.declarations

//## begin module%3CF4AE200219.additionalDeclarations preserve=yes
//## end module%3CF4AE200219.additionalDeclarations


//## begin ISessContextSerial%3CF4BD8D0109.preface preserve=yes

class M4Referenceable {

public:
	M4Referenceable()
	{
		m_iNRef = 0;
	}

	virtual m4uint32_t AddRef()
	{
		return ++m_iNRef;
	}

	virtual m4uint32_t DelRef()
	{
		if (m_iNRef == 0) {
			return 0;
		}
		return --m_iNRef;
	}

protected:
	m4uint32_t m_iNRef;
};


// iterator class for serialized contextes
class ISessionIterator : public M4Referenceable {

public:
	// general functions
	virtual m4pcchar_t GetKey() = 0;
	virtual m4pcchar_t GetValue() = 0;
	virtual void Next() = 0;
};

// iterator class for serialized contextes
class M4_DECL_M4CACERTMNGR SessionIterator {

public:
	SessionIterator(const SessionIterator &ai_pIterator);
	SessionIterator(ISessionIterator *ai_poIterator);

	~SessionIterator();

	// copy operator
	SessionIterator & operator = (SessionIterator &ai_oIterator);
	
	// general functions
	m4pcchar_t GetKey();
	m4pcchar_t GetValue();
	void Next();

	// internals
protected:
	void Reset();
	void AddRef();

private:
	// internal members
	ISessionIterator *m_poIterator;
};

//## end ISessContextSerial%3CF4BD8D0109.preface

//## Class: ISessContextSerial%3CF4BD8D0109; Abstract
//## Category: M4ServerBase::session context%3CF3B3120047
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ISessContextSerial 
{
  //## begin ISessContextSerial%3CF4BD8D0109.initialDeclarations preserve=yes
  //## end ISessContextSerial%3CF4BD8D0109.initialDeclarations

  public:


    //## Other Operations (specified)
      //## Operation: AddAtribute%1022692995
      virtual m4return_t AddAtribute (m4string_t ai_strAtribute, m4string_t ai_strValue) = 0;

      //## Operation: Serialize%1022692996
      virtual m4return_t Serialize (m4pchar_t & ao_pBuffer, m4uint32_t & ao_bufLen) = 0;

      //## Operation: Deserialize%1022692997
      virtual m4return_t Deserialize (m4pchar_t ai_pBuffer, m4uint32_t ai_iBufLen) = 0;

      //## Operation: GetAtribute%1022692998
      virtual m4return_t GetAtribute (m4string_t ai_strAtribute, m4string_t &ao_strValue) = 0;

    // Additional Public Declarations
      //## begin ISessContextSerial%3CF4BD8D0109.public preserve=yes
	  virtual SessionIterator GetIterator() = 0;
      //## end ISessContextSerial%3CF4BD8D0109.public

  protected:
    // Additional Protected Declarations
      //## begin ISessContextSerial%3CF4BD8D0109.protected preserve=yes
      //## end ISessContextSerial%3CF4BD8D0109.protected

  private:
    // Additional Private Declarations
      //## begin ISessContextSerial%3CF4BD8D0109.private preserve=yes
      //## end ISessContextSerial%3CF4BD8D0109.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ISessContextSerial%3CF4BD8D0109.implementation preserve=yes
      //## end ISessContextSerial%3CF4BD8D0109.implementation

};

//## begin ISessContextSerial%3CF4BD8D0109.postscript preserve=yes

//## end ISessContextSerial%3CF4BD8D0109.postscript

// Class ISessContextSerial 

//## begin module%3CF4AE200219.epilog preserve=yes
//## end module%3CF4AE200219.epilog


#endif
