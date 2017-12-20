//## begin module%3CF4A8EF0309.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CF4A8EF0309.cm

//## begin module%3CF4A8EF0309.cp preserve=no
//## end module%3CF4A8EF0309.cp

//## Module: sessioncontext%3CF4A8EF0309; Package specification
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Source file: F:\integration\m4serverbase\inc\sessioncontext.h

#ifndef sessioncontext_h
#define sessioncontext_h 1

//## begin module%3CF4A8EF0309.additionalIncludes preserve=no
//## end module%3CF4A8EF0309.additionalIncludes

//## begin module%3CF4A8EF0309.includes preserve=yes
#include <sesscontextserial.hpp>
#include <m4cacertmngr_dll.hpp>
//## end module%3CF4A8EF0309.includes

//## begin module%3CF4A8EF0309.declarations preserve=no
//## end module%3CF4A8EF0309.declarations

//## begin module%3CF4A8EF0309.additionalDeclarations preserve=yes
#define M4_SESSION_CONTEXT_SERIALIZER_UNINITIALIZED		-2
//## end module%3CF4A8EF0309.additionalDeclarations


//## begin ClSessionContext%3CF4BEA5021A.preface preserve=yes
//## end ClSessionContext%3CF4BEA5021A.preface

//## Class: ClSessionContext%3CF4BEA5021A
//## Category: M4ServerBase::session context%3CF3B3120047
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACERTMNGR ClSessionContext 
{
  //## begin ClSessionContext%3CF4BEA5021A.initialDeclarations preserve=yes
  //## end ClSessionContext%3CF4BEA5021A.initialDeclarations

  public:
    //## begin ClSessionContext::eSerializer%3CF4DCBB02F2.preface preserve=yes
    //## end ClSessionContext::eSerializer%3CF4DCBB02F2.preface

    //## Class: eSerializer%3CF4DCBB02F2
    //## Category: M4ServerBase::session context%3CF3B3120047
    //## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    //## Constructors (generated)
      ClSessionContext();

    //## Destructor (generated)
      ~ClSessionContext();


    //## Other Operations (specified)
      //## Operation: SetAtribute%1022692987
      m4return_t SetAtribute (m4string_t ai_strAtribute, m4string_t ai_strValue);

      //## Operation: Serialize%1022692988
      m4return_t Serialize (m4pchar_t & ao_pBuffer, m4uint32_t & ao_iBufLen);

      //## Operation: Deserialize%1022692989
      m4return_t Deserialize (m4pchar_t ai_pBuffer );

      //## Operation: GetAtribute%1022692990
      m4return_t GetAtribute (m4string_t ai_strAtribute, m4string_t &ao_strValue);

	  m4return_t GetError () ;

  public:
    // Additional Public Declarations
      //## begin ClSessionContext%3CF4BEA5021A.public preserve=yes
	  SessionIterator GetIterator();
      //## end ClSessionContext%3CF4BEA5021A.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessionContext%3CF4BEA5021A.protected preserve=yes
      //## end ClSessionContext%3CF4BEA5021A.protected

  private:
    //## Get and Set Operations for Associations (generated)


    // Additional Private Declarations
      //## begin ClSessionContext%3CF4BEA5021A.private preserve=yes
      //## end ClSessionContext%3CF4BEA5021A.private

  private: //## implementation
    // Data Members for Associations

      //## Association: M4ServerBase::session context::<unnamed>%3CF4BEBF0087
      //## begin ClSessionContext::m_poSessionSerializer%3CF4BEBF03A8.role preserve=no  private: ISessContextSerial { -> RHN}
      ISessContextSerial *m_poSessionSerializer;
      //## end ClSessionContext::m_poSessionSerializer%3CF4BEBF03A8.role

    // Additional Implementation Declarations
      //## begin ClSessionContext%3CF4BEA5021A.implementation preserve=yes
	  m4return_t m_iError ;
      //## end ClSessionContext%3CF4BEA5021A.implementation

};

//## begin ClSessionContext%3CF4BEA5021A.postscript preserve=yes
//## end ClSessionContext%3CF4BEA5021A.postscript

// Class ClSessionContext 


//## begin module%3CF4A8EF0309.epilog preserve=yes
//## end module%3CF4A8EF0309.epilog


#endif
