//## begin module%3992D03400A4.cm preserve=no
//## end module%3992D03400A4.cm

//## begin module%3992D03400A4.cp preserve=no
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
//## end module%3992D03400A4.cp

//## Module: conninstance%3992D03400A4; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\conninstance.hpp

#ifndef conninstance_h
#define conninstance_h 1

//## begin module%3992D03400A4.additionalIncludes preserve=no
//## end module%3992D03400A4.additionalIncludes

//## begin module%3992D03400A4.includes preserve=yes
#include <m4serverbase_dll.hpp>
#include <m4string.hpp>
//## end module%3992D03400A4.includes

// Protocol
#include <protocol.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// ExVariables
#include <exvariables.hpp>
//## begin module%3992D03400A4.declarations preserve=no
//## end module%3992D03400A4.declarations

//## begin module%3992D03400A4.additionalDeclarations preserve=yes
#define PENDING				0
#define CHECK_HOST			1
#define CHECK_USER			2
#define CHECK_PERMISIONS	3
#define ACTIVE				4
#define CLOSSING			5
#define CLOSED				6
#define BROKEN				7

//Client protocols
#define M4_HTTP_PROTOCOL	0
#define M4_PROTOCOL			1
// Bg 134521
#define M4_PROTOCOL_TC		2

//## end module%3992D03400A4.additionalDeclarations


//## begin eConnProtocol%39A540DA0344.preface preserve=yes
//## end eConnProtocol%39A540DA0344.preface

//## Class: eConnProtocol%39A540DA0344
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {M4_CONNECTION_ORIENTED_PROTOCOL = 0 , M4_NON_CONNECTION_ORIENTED_PROTOCOL } eConnProtocol;

//## begin eConnProtocol%39A540DA0344.postscript preserve=yes
//## end eConnProtocol%39A540DA0344.postscript

//## begin ClConnInstance%39A5365C034C.preface preserve=yes
//## end ClConnInstance%39A5365C034C.preface

//## Class: ClConnInstance%39A5365C034C
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39A5427D0184;eConnProtocol { -> }

class M4_DECL_M4SERVERBASE ClConnInstance : public ClBaseObject  //## Inherits: <unnamed>%39A537180193
{
  //## begin ClConnInstance%39A5365C034C.initialDeclarations preserve=yes
  //## end ClConnInstance%39A5365C034C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConnInstance%967450714
      ClConnInstance (ClConnInstance *ai_poConnection);

    //## Destructor (specified)
      //## Operation: ~ClConnInstance%967113589
      virtual ~ClConnInstance ();


    //## Other Operations (specified)
      //## Operation: GetCompressionType%967113590
      m4uint32_t GetCompressionType ();

      //## Operation: GetCompressionlevel%967113591
      m4uint32_t GetCompressionlevel ();

      //## Operation: SetCompressionType%967113592
      void SetCompressionType (m4uint32_t ai_lCompressionType);

      //## Operation: SetCompressionLevel%967113593
      void SetCompressionLevel (m4uint32_t ai_lCompressionLevel);

      //## Operation: GetConnProtocol%967113594
      eConnProtocol GetConnProtocol ();

      //## Operation: SetConnProtocol%967450715
      void SetConnProtocol (eConnProtocol ai_lConnProtocol);

      //## Operation: SetHost%967450709
      //	Set the host atribute
      void SetHost (m4string_t ai_szHostName);

      //## Operation: GetHost%967450710
      //	Returns the host atribute value
      m4pcchar_t GetHost ();

      //## Operation: SetClientType%967450711
      void SetClientType (m4uint32_t ai_lClientType);

      //## Operation: GetClientType%967450712
      m4uint32_t GetClientType ();

      //## Operation: SetStatus%967450720
      virtual m4return_t SetStatus (m4uint32_t ai_iValue);

      //## Operation: GetStatus%967450721
      m4uint32_t GetStatus ();

      //## Operation: SetIP%1003855114
      void SetIP (m4string_t ai_szIP);

      //## Operation: GetIP%1003855115
      m4pcchar_t GetIP ();

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%39A53E9C0059
      //## Role: ClConnInstance::m_oProtocol%39A53E9C029F
      //## begin ClConnInstance::m_oProtocol%39A53E9C029F.role preserve=no  public: ClProtocol { -> VHAN}
      ClProtocol m_oProtocol;
      //## end ClConnInstance::m_oProtocol%39A53E9C029F.role

      //## Association: M4ServerBase::<unnamed>%39A645180219
      //## Role: ClConnInstance::m_oNumRequest%39A6451900C6
      //## begin ClConnInstance::m_oNumRequest%39A6451900C6.role preserve=no  public: ClNumRequest { -> VHAN}
      ClNumRequest m_oNumRequest;
      //## end ClConnInstance::m_oNumRequest%39A6451900C6.role

    // Additional Public Declarations
      //## begin ClConnInstance%39A5365C034C.public preserve=yes
      //## end ClConnInstance%39A5365C034C.public

  protected:
    //## Constructors (specified)
      //## Operation: ClConnInstance%967113588
      ClConnInstance (m4objid_t ai_lIdConnection, eConnProtocol ai_lConnProtocol);

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%39A643EF0385
      //## Role: ClConnInstance::m_oStatus%39A643F001CD
      //## begin ClConnInstance::m_oStatus%39A643F001CD.role preserve=no  protected: ClStatus { -> VHAN}
      ClStatus m_oStatus;
      //## end ClConnInstance::m_oStatus%39A643F001CD.role

    // Additional Protected Declarations
      //## begin ClConnInstance%39A5365C034C.protected preserve=yes
      //## end ClConnInstance%39A5365C034C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lCompressionType%39A536DA03C5
      //## begin ClConnInstance::m_lCompressionType%39A536DA03C5.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_lCompressionType;
      //## end ClConnInstance::m_lCompressionType%39A536DA03C5.attr

      //## Attribute: m_lCompressionLevel%39A536DA03E3
      //## begin ClConnInstance::m_lCompressionLevel%39A536DA03E3.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_lCompressionLevel;
      //## end ClConnInstance::m_lCompressionLevel%39A536DA03E3.attr

      //## Attribute: m_lConnProtocol%39A53FDF03B1
      //## begin ClConnInstance::m_lConnProtocol%39A53FDF03B1.attr preserve=no  private: eConnProtocol {UA} 
      eConnProtocol m_lConnProtocol;
      //## end ClConnInstance::m_lConnProtocol%39A53FDF03B1.attr

      //## Attribute: m_szHost%39AA1F96003A
      //	Host of the client machine of that connection.
      //## begin ClConnInstance::m_szHost%39AA1F96003A.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szHost;
      //## end ClConnInstance::m_szHost%39AA1F96003A.attr

      //## Attribute: m_lClientType%39AA1FD10305
      //## begin ClConnInstance::m_lClientType%39AA1FD10305.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_lClientType;
      //## end ClConnInstance::m_lClientType%39AA1FD10305.attr

      //## Attribute: m_szIP%3BD59A8D035B
      //## begin ClConnInstance::m_szIP%3BD59A8D035B.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szIP;
      //## end ClConnInstance::m_szIP%3BD59A8D035B.attr

    // Additional Private Declarations
      //## begin ClConnInstance%39A5365C034C.private preserve=yes
      //## end ClConnInstance%39A5365C034C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnInstance%39A5365C034C.implementation preserve=yes
      //## end ClConnInstance%39A5365C034C.implementation

};

//## begin ClConnInstance%39A5365C034C.postscript preserve=yes
//## end ClConnInstance%39A5365C034C.postscript

// Class ClConnInstance 


//## Other Operations (inline)
inline m4uint32_t ClConnInstance::GetCompressionType ()
{
  //## begin ClConnInstance::GetCompressionType%967113590.body preserve=yes
	return m_lCompressionType ;
  //## end ClConnInstance::GetCompressionType%967113590.body
}

inline m4uint32_t ClConnInstance::GetCompressionlevel ()
{
  //## begin ClConnInstance::GetCompressionlevel%967113591.body preserve=yes
	return m_lCompressionLevel ;
  //## end ClConnInstance::GetCompressionlevel%967113591.body
}

inline void ClConnInstance::SetCompressionType (m4uint32_t ai_lCompressionType)
{
  //## begin ClConnInstance::SetCompressionType%967113592.body preserve=yes
	m_lCompressionType = ai_lCompressionType ;
  //## end ClConnInstance::SetCompressionType%967113592.body
}

inline void ClConnInstance::SetCompressionLevel (m4uint32_t ai_lCompressionLevel)
{
  //## begin ClConnInstance::SetCompressionLevel%967113593.body preserve=yes
	m_lCompressionLevel = ai_lCompressionLevel ;
  //## end ClConnInstance::SetCompressionLevel%967113593.body
}

//## begin module%3992D03400A4.epilog preserve=yes
//## end module%3992D03400A4.epilog


#endif
