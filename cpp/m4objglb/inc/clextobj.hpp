//## begin module%3912A6B90107.cm preserve=no
//## end module%3912A6B90107.cm

//## begin module%3912A6B90107.cp preserve=no
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
//## end module%3912A6B90107.cp

//## Module: clextobj%3912A6B90107; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clextobj.hpp

#ifndef __CLEXTOBJ__HPP__
#define __CLEXTOBJ__HPP__ 1

//## begin module%3912A6B90107.additionalIncludes preserve=no
//## end module%3912A6B90107.additionalIncludes

//## begin module%3912A6B90107.includes preserve=yes

#include "m4objglb_dll.hpp"
// COMMON...
#include "m4types.hpp"
#include "m4stl.hpp"

#include "m4win.hpp"

#include "m4string.hpp"

#include "m4system.hpp"
#include "m4define.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"
//## end module%3912A6B90107.includes

//## begin module%3912A6B90107.declarations preserve=no
//## end module%3912A6B90107.declarations

//## begin module%3912A6B90107.additionalDeclarations preserve=yes

class	m4VariantType ;



//## end module%3912A6B90107.additionalDeclarations


//## begin M4ClExternObject%3912A6B90106.preface preserve=yes
//## end M4ClExternObject%3912A6B90106.preface

//## Class: M4ClExternObject%3912A6B90106; Abstract
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB M4ClExternObject 
{
  //## begin M4ClExternObject%3912A6B90106.initialDeclarations preserve=yes
  //## end M4ClExternObject%3912A6B90106.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClExternObject%1715157947; C++
      M4ClExternObject ()
        //## begin M4ClExternObject::M4ClExternObject%1715157947.hasinit preserve=no
        //## end M4ClExternObject::M4ClExternObject%1715157947.hasinit
        //## begin M4ClExternObject::M4ClExternObject%1715157947.initialization preserve=yes
        //## end M4ClExternObject::M4ClExternObject%1715157947.initialization
      {
        //## begin M4ClExternObject::M4ClExternObject%1715157947.body preserve=yes
        //## end M4ClExternObject::M4ClExternObject%1715157947.body
      }

    //## Destructor (specified)
      //## Operation: ~M4ClExternObject%-2049858250; C++
      virtual ~M4ClExternObject ()
      {
        //## begin M4ClExternObject::~M4ClExternObject%-2049858250.body preserve=yes
        //## end M4ClExternObject::~M4ClExternObject%-2049858250.body
      }


    //## Other Operations (specified)
      //## Operation: Init%-518085603; C++
      //	-- {AddDecl: 50} region.unprotectedFunction [1054..1085]
      virtual m4return_t Init (m4pcchar_t ai_pccObjectName) = 0;

      //## Operation: Execute%-1362236650; C++
      virtual m4return_t Execute (m4pcchar_t ai_pccMethodName, m4VariantType* ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType& ai_pvReturn, m4pvoid_t ai_pvContext) = 0;

    // Additional Public Declarations
      //## begin M4ClExternObject%3912A6B90106.public preserve=yes
      //## end M4ClExternObject%3912A6B90106.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClExternObject%3912A6B90106.protected preserve=yes
      //## end M4ClExternObject%3912A6B90106.protected

  private:
    // Additional Private Declarations
      //## begin M4ClExternObject%3912A6B90106.private preserve=yes
      //## end M4ClExternObject%3912A6B90106.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClExternObject%3912A6B90106.implementation preserve=yes
      //## end M4ClExternObject%3912A6B90106.implementation

};

//## begin M4ClExternObject%3912A6B90106.postscript preserve=yes
//## end M4ClExternObject%3912A6B90106.postscript

//## begin M4ClExternObjectMapEntry%3912A6BA01E4.preface preserve=yes
//## end M4ClExternObjectMapEntry%3912A6BA01E4.preface

//## Class: M4ClExternObjectMapEntry%3912A6BA01E4
//	-- {AddDecl: 52} class [1258..1929]
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB M4ClExternObjectMapEntry 
{
  //## begin M4ClExternObjectMapEntry%3912A6BA01E4.initialDeclarations preserve=yes
  //## end M4ClExternObjectMapEntry%3912A6BA01E4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClExternObjectMapEntry%716586242; C++
      M4ClExternObjectMapEntry (void );

    //## Destructor (specified)
      //## Operation: ~M4ClExternObjectMapEntry%-56678893; C++
      //	-- {AddDecl: 54} region.unprotectedFunction [1548..1618]
      //	-- {AddDecl: 248} region.unprotectedFunction [1176..1430]
      ~M4ClExternObjectMapEntry (void );


    //## Other Operations (specified)
      //## Operation: Init%-1727756540; C++
      //	-- {AddDecl: 247} region.unprotectedFunction [736..1175]
      m4return_t Init (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType);

      //## Operation: GetExtObj%-1027349334; C++
      M4ClExternObject* GetExtObj (void )
      {
        //## begin M4ClExternObjectMapEntry::GetExtObj%-1027349334.body preserve=yes
		return m_pExtObj;
	
        //## end M4ClExternObjectMapEntry::GetExtObj%-1027349334.body
      }

      //## Operation: GetObjectType%-661609160; C++
      m4int32_t GetObjectType (void )
      {
        //## begin M4ClExternObjectMapEntry::GetObjectType%-661609160.body preserve=yes
		return m_iObjectType;
	
        //## end M4ClExternObjectMapEntry::GetObjectType%-661609160.body
      }

      //## Operation: GetLinks%167420768; C++
      m4int32_t GetLinks (void )
      {
        //## begin M4ClExternObjectMapEntry::GetLinks%167420768.body preserve=yes
		return m_iLinks;
	
        //## end M4ClExternObjectMapEntry::GetLinks%167420768.body
      }

      //## Operation: AddLink%-222677334; C++
      m4int32_t AddLink (void )
      {
        //## begin M4ClExternObjectMapEntry::AddLink%-222677334.body preserve=yes
		return ++m_iLinks;
	
        //## end M4ClExternObjectMapEntry::AddLink%-222677334.body
      }

      //## Operation: SubstractLink%271816672; C++
      m4int32_t SubstractLink (void )
      {
        //## begin M4ClExternObjectMapEntry::SubstractLink%271816672.body preserve=yes
		if (m_iLinks == 0)
			return 0;
		else
			return --m_iLinks;
	
        //## end M4ClExternObjectMapEntry::SubstractLink%271816672.body
      }

    // Additional Public Declarations
      //## begin M4ClExternObjectMapEntry%3912A6BA01E4.public preserve=yes
      //## end M4ClExternObjectMapEntry%3912A6BA01E4.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClExternObjectMapEntry%3912A6BA01E4.protected preserve=yes
      //## end M4ClExternObjectMapEntry%3912A6BA01E4.protected

  private:
    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6BA021D
      //## Role: M4ClExternObjectMapEntry::m_pExtObj%3912A6BA021F
      //## begin M4ClExternObjectMapEntry::m_pExtObj%3912A6BA021F.role preserve=no  private: M4ClExternObject {1 -> 0..1RHAPN}
      M4ClExternObject *m_pExtObj;
      //## end M4ClExternObjectMapEntry::m_pExtObj%3912A6BA021F.role

      //## Association: m4objglb::<unnamed>%3912A6BA022B
      //## Role: M4ClExternObjectMapEntry::m_iObjectType%3912A6BA022D
      //## begin M4ClExternObjectMapEntry::m_iObjectType%3912A6BA022D.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iObjectType;
      //## end M4ClExternObjectMapEntry::m_iObjectType%3912A6BA022D.role

      //## Association: m4objglb::<unnamed>%3912A6BA022E
      //## Role: M4ClExternObjectMapEntry::m_iLinks%3912A6BA0230
      //## begin M4ClExternObjectMapEntry::m_iLinks%3912A6BA0230.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iLinks;
      //## end M4ClExternObjectMapEntry::m_iLinks%3912A6BA0230.role

    // Additional Private Declarations
      //## begin M4ClExternObjectMapEntry%3912A6BA01E4.private preserve=yes
      //## end M4ClExternObjectMapEntry%3912A6BA01E4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClExternObjectMapEntry%3912A6BA01E4.implementation preserve=yes
      //## end M4ClExternObjectMapEntry%3912A6BA01E4.implementation

};

//## begin M4ClExternObjectMapEntry%3912A6BA01E4.postscript preserve=yes
//## end M4ClExternObjectMapEntry%3912A6BA01E4.postscript

//## begin M4ClExternObjectMap%3912A6BA0181.preface preserve=yes
//## end M4ClExternObjectMap%3912A6BA0181.preface

//## Class: M4ClExternObjectMap%3912A6BA0181; Instantiated Class
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< string,M4ClExternObjectMapEntry*,less< string > > M4ClExternObjectMap;

//## begin M4ClExternObjectMap%3912A6BA0181.postscript preserve=yes
//## end M4ClExternObjectMap%3912A6BA0181.postscript

//## begin module%3912A6B90107.epilog preserve=yes
typedef M4ClExternObjectMap::iterator itM4ClExternObjectMap;
//## end module%3912A6B90107.epilog


#endif
