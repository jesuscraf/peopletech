//## begin module%356568620303.cm preserve=no
//## end module%356568620303.cm

//## begin module%356568620303.cp preserve=no
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
//## end module%356568620303.cp

//## Module: servenv%356568620303; Package specification
//## Subsystem: M4ServEnv::inc%378B0D25012A
//## Source file: F:\integration\m4servenv\inc\servenv.hpp

#ifndef servenv_h
#define servenv_h 1

//## begin module%356568620303.additionalIncludes preserve=no
//## end module%356568620303.additionalIncludes

//## begin module%356568620303.includes preserve=yes
#include <m4types.hpp>
//## end module%356568620303.includes

//## begin module%356568620303.declarations preserve=no
//## end module%356568620303.declarations

//## begin module%356568620303.additionalDeclarations preserve=yes
#include "m4servenv_dll.hpp"

//## end module%356568620303.additionalDeclarations

class ClNavigatorFirewall;

//## begin ClServEnv%356568E0020A.preface preserve=yes
//## end ClServEnv%356568E0020A.preface

//## Class: ClServEnv%356568E0020A
//	This class is used into a dll. Its used is get from outside the server to
//	get an 'enviroment' variable. That enviroment property is in deed an 'obl'
//	configuration property.
//	It uses a reference to an special class called 'navigatosFirewall' to do
//	that, but that reference is instantiate as a derived class object. That
//	reference is an static attribute because this class is instantiated each
//	time somebody want to get an environ variable and it's not wanted to
//	initialize the navigator each time the object is created.
//## Category: M4ServEnv%35655B40010F
//## Subsystem: M4ServEnv::inc%378B0D25012A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClServEnv 
{
  //## begin ClServEnv%356568E0020A.initialDeclarations preserve=yes
  //## end ClServEnv%356568E0020A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServEnv%895835641
      ClServEnv ();

    //## Destructor (specified)
      //## Operation: ~ClServEnv%895835642
      ~ClServEnv ();


    //## Other Operations (specified)
      //## Operation: GetEnviron%895835643
      //	This function returns error or success as the result of looking for a
      //	property in a path (subsystem)
      //	The path means the name of the subsystem
      static m4return_t GetEnviron (m4pchar_t ai_szPath, 	// That parameter is used to reference a subsistem object in witch the
      	// propertyis stored
      m4pchar_t ai_szProperty, 	// The name of the property to look for
      m4pchar_t ao_szValue, 	// The value of the property looked for, it's an output parameter
      m4uint32_t ai_iBufflen	// An input value that means the length of the buffer for output value
      );

      //## Operation: SetNavigator%895835644
      //	Static method to initialize its navigator static attribute.
      //	The method receives as parameter a reference to a NavigatorFirewall class,
      //	but that class is abstract so the passed reference is a pointer  to Navigator
      //	SS class
      static void SetNavigator (ClNavigatorFirewall *ai_poNavigator);

      //## Operation: SetEnviron%898068452
      m4return_t SetEnviron (m4pchar_t ai_szPath, 	// That parameter is used to reference a subsistem object in witch the
      	// propertyis stored
      m4pchar_t ai_szProperty, 	// The name of the property to look for
      m4pchar_t ai_szValue, 	// The value of the property looked for, it's an output parameter
      m4bool_t ai_bPersistable = M4_FALSE);

      //## Operation: GetEnviron%962703854
      //	This function returns error or success as the result of looking for a
      //	property in a path (subsystem)
      //	The path means the name of the subsystem
      static m4return_t GetEnviron (m4pchar_t ai_szPath, 	// That parameter is used to reference a subsistem object in witch the
      	// propertyis stored
      m4pchar_t ai_szProperty, 	// The name of the property to look for
      m4double_t &ao_dValue	// The value of the property looked for, it's an output parameter
      );

    // Data Members for Associations

      //## Association: M4ServEnv::<unnamed>%35656A9F009A
      //## Role: ClServEnv::<m_pClNavigatorFirewall>%35656AA0018C
      //## begin ClServEnv::<m_pClNavigatorFirewall>%35656AA0018C.role preserve=no  public: static ClNavigatorFirewall { -> RHAN}
      static ClNavigatorFirewall *m_pClNavigatorFirewall;
      //## end ClServEnv::<m_pClNavigatorFirewall>%35656AA0018C.role

    // Additional Public Declarations
      //## begin ClServEnv%356568E0020A.public preserve=yes
      //## end ClServEnv%356568E0020A.public

  protected:
    // Additional Protected Declarations
      //## begin ClServEnv%356568E0020A.protected preserve=yes
      //## end ClServEnv%356568E0020A.protected

  private:
    // Additional Private Declarations
      //## begin ClServEnv%356568E0020A.private preserve=yes
      //## end ClServEnv%356568E0020A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServEnv%356568E0020A.implementation preserve=yes
      //## end ClServEnv%356568E0020A.implementation

};

//## begin ClServEnv%356568E0020A.postscript preserve=yes
//## end ClServEnv%356568E0020A.postscript

// Class ClServEnv 

//## begin module%356568620303.epilog preserve=yes
//## end module%356568620303.epilog


#endif
