//## begin module%3912A6B9014C.cm preserve=no
//## end module%3912A6B9014C.cm

//## begin module%3912A6B9014C.cp preserve=no
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
//## end module%3912A6B9014C.cp

//## Module: clextman%3912A6B9014C; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clextman.hpp

#ifndef __CLEXTMAN__HPP__
#define __CLEXTMAN__HPP__ 1

//## begin module%3912A6B9014C.additionalIncludes preserve=no
//## end module%3912A6B9014C.additionalIncludes

//## begin module%3912A6B9014C.includes preserve=yes

#include "m4objglb_dll.hpp"
#include "clextobj.hpp"

//-------- El Gestor
//## end module%3912A6B9014C.includes

//## begin module%3912A6B9014C.declarations preserve=no
//## end module%3912A6B9014C.declarations

//## begin module%3912A6B9014C.additionalDeclarations preserve=yes
//## end module%3912A6B9014C.additionalDeclarations


//## begin M4ClExternObjectManager%3912A6B90156.preface preserve=yes
//## end M4ClExternObjectManager%3912A6B90156.preface

//## Class: M4ClExternObjectManager%3912A6B90156
//	-------- El Gestor
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB M4ClExternObjectManager 
{
  //## begin M4ClExternObjectManager%3912A6B90156.initialDeclarations preserve=yes
  //## end M4ClExternObjectManager%3912A6B90156.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClExternObjectManager%1757527291; C++
      M4ClExternObjectManager (void );

    //## Destructor (specified)
      //## Operation: ~M4ClExternObjectManager%-526924627; C++
      //	-- {AddDecl: 238} region.unprotectedFunction [770..832]
      ~M4ClExternObjectManager ();


    //## Other Operations (specified)
      //## Operation: LoadObject%-355168904; C++
      //	-- {AddDecl: 239} region.unprotectedFunction [833..1523]
      m4return_t LoadObject (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType);

      //## Operation: GetObject%1000563077; C++
      //	-- {AddDecl: 240} region.unprotectedFunction [1524..2244]
      M4ClExternObject* GetObject (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType);

      //## Operation: FreeObject%932442653; C++
      //	-- {AddDecl: 241} region.unprotectedFunction [2245..2647]
      m4return_t FreeObject (m4pcchar_t ai_pccObjectName);

      //## Operation: FreeObject%-1687646626; C++
      //	-- {AddDecl: 242} region.unprotectedFunction [2648..3159]
      m4return_t FreeObject (M4ClExternObject* ai_poObject);

	static void	SetCallBack( long ai_lfCallBack ) ;

	static m4return_t	RunCallBack( m4pcchar_t ai_pccObjectName ) ;

    // Additional Public Declarations
      //## begin M4ClExternObjectManager%3912A6B90156.public preserve=yes
      //## end M4ClExternObjectManager%3912A6B90156.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClExternObjectManager%3912A6B90156.protected preserve=yes
      //## end M4ClExternObjectManager%3912A6B90156.protected

	// Función de callback para el download en rich client
	static	long	m_lfCallBack;

  private:
    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6BA018D
      //## Role: M4ClExternObjectManager::m_ExtObjMap%3912A6BA018F
      //## begin M4ClExternObjectManager::m_ExtObjMap%3912A6BA018F.role preserve=no  private: static M4ClExternObjectMap {1 -> 1VHAPN}
      static M4ClExternObjectMap m_ExtObjMap;
      //## end M4ClExternObjectManager::m_ExtObjMap%3912A6BA018F.role

    // Additional Private Declarations
      //## begin M4ClExternObjectManager%3912A6B90156.private preserve=yes
      //## end M4ClExternObjectManager%3912A6B90156.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClExternObjectManager%3912A6B90156.implementation preserve=yes
      //## end M4ClExternObjectManager%3912A6B90156.implementation

};

//## begin M4ClExternObjectManager%3912A6B90156.postscript preserve=yes
//## end M4ClExternObjectManager%3912A6B90156.postscript

// Class M4ClExternObjectManager 

//## begin module%3912A6B9014C.epilog preserve=yes
//## end module%3912A6B9014C.epilog


#endif
