//## begin module%38205BF3034D.cm preserve=no
//## end module%38205BF3034D.cm

//## begin module%38205BF3034D.cp preserve=no
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
//## end module%38205BF3034D.cp

//## Module: garbageableinterface%38205BF3034D; Package specification
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Source file: D:\eduardoma\m4serverbase\inc\garbageableinterface.hpp

#ifndef garbageableinterface_h
#define garbageableinterface_h 1

//## begin module%38205BF3034D.additionalIncludes preserve=no
//## end module%38205BF3034D.additionalIncludes

//## begin module%38205BF3034D.includes preserve=yes
#include <m4types.hpp>
#include <m4serverbase_dll.hpp>
//## end module%38205BF3034D.includes

//## begin module%38205BF3034D.declarations preserve=no
//## end module%38205BF3034D.declarations

//## begin module%38205BF3034D.additionalDeclarations preserve=yes
//	Esto no debe estar aquí!!
const m4uint32_t REQUEST_OBJECT_TYPE	= 1 ;
const m4uint32_t SESSION_OBJECT_TYPE	= 2 ;
const m4uint32_t USER_OBJECT_TYPE		= 3 ;
const m4uint32_t SERVICE_OBJECT_TYPE	= 4 ;
const m4uint32_t SUBSYSTEM_OBJECT_TYPE	= 5 ;
//## end module%38205BF3034D.additionalDeclarations


//## Class: IRecycleable%382050CF02C3
//	This interface extends the Garbageable one providing a means to enable
//	recycling of objects, this is, the object releases resources and gets ready
//	for a new initialization.
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE IRecycleable
{
  public:

    //## Other Operations (specified)
      //## Operation: Recycle%941644995; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle () = 0;

      //## Operation: CanRecycle%941644996
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle () = 0;

      //## Operation: GetType%955440308
      m4uint32_t GetType ()
      {
        //## begin IRecycleable::GetType%955440308.body preserve=yes
		return m_lType ;
        //## end IRecycleable::GetType%955440308.body
      }

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_lType%38F311EC0149
      //## begin IRecycleable::m_lType%38F311EC0149.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_lType;
      //## end IRecycleable::m_lType%38F311EC0149.attr

  private:
  private: //## implementation
};

//## Class: IGarbageable%382050CF02BF
//	This class offers a destruction interface that performs the clean-up
//	regarding all resources owned by the object. Some recursive destruction on
//	garbageable data members may be enforced.
//
//	  Default destructor should call the Destruction API.
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE IGarbageable : public IRecycleable  //## Inherits: <unnamed>%382050CF02C0
{
  public:

    //## Other Operations (specified)
      //## Operation: Destroy%941644997
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy () = 0;

      //## Operation: CanDestroy%941644998
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy () = 0;

  protected:
  private:
  private: //## implementation
};

// Class IRecycleable 


// Class IGarbageable 

//## begin module%38205BF3034D.epilog preserve=yes
//## end module%38205BF3034D.epilog


#endif
