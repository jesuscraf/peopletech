//## begin module%377768450199.cm preserve=no
//## end module%377768450199.cm

//## begin module%377768450199.cp preserve=no
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
//## end module%377768450199.cp

//## Module: windowstsapimpl%377768450199; Package specification
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//	d:\compon\m4tsap\version\inc\win32
//## Source file: d:\compon\m4tsap\version\inc\win32\windowstsapimpl.hpp

#ifndef windowstsapimpl_h
#define windowstsapimpl_h 1

//## begin module%377768450199.additionalIncludes preserve=no
//## end module%377768450199.additionalIncludes

//## begin module%377768450199.includes preserve=yes


#include "tsap.hpp"
#include "tsapimpl.hpp"
#include "selectimpl.hpp"

//## end module%377768450199.includes

// m4types
#include <m4types.hpp>
//## begin module%377768450199.declarations preserve=no
//## end module%377768450199.declarations

//## begin module%377768450199.additionalDeclarations preserve=yes
//## end module%377768450199.additionalDeclarations


//## begin ClWindowsTSAPImpl%3777684501AD.preface preserve=yes
//## end ClWindowsTSAPImpl%3777684501AD.preface

//## Class: ClWindowsTSAPImpl%3777684501AD
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3777684501D5;ClTSAPImpl { -> }
//## Uses: <unnamed>%3777684501D8;ClInetAddress { -> }
//## Uses: <unnamed>%3777684501E3;m4bool_t { -> }

class ClWindowsTSAPImpl : public ClTSAPImpl  //## Inherits: <unnamed>%3777684501F3
{
  //## begin ClWindowsTSAPImpl%3777684501AD.initialDeclarations preserve=yes
  //## end ClWindowsTSAPImpl%3777684501AD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClWindowsTSAPImpl%310246555; C++
      ClWindowsTSAPImpl ();


    //## Other Operations (specified)
      //## Operation: GetSystemDescriptor%-903148913; C++
      int GetSystemDescriptor ()
      {
        //## begin ClWindowsTSAPImpl::GetSystemDescriptor%-903148913.body preserve=yes
return m_iSocket;
        //## end ClWindowsTSAPImpl::GetSystemDescriptor%-903148913.body
      }

      //## Operation: Accept%1350106493; C++
      int Accept (ClTSAPImpl* ao_poTSAPImpl);

      //## Operation: Bind%1402584086; C++
      int Bind (ClInetAddress* ai_poInetAddress, int lport);

      //## Operation: Close%1860826927; C++
      void Close ();

      //## Operation: Connect%-1851496694; C++
      int Connect (const char* ai_pcHostname, int ai_iPort);

      //## Operation: Connect%-1754837781; C++
      int Connect (ClInetAddress* ai_InetAddress, int ai_iPort);

      //## Operation: Create%2049243730; C++
      int Create (m4bool_t ai_bStream = M4_TRUE);

      //## Operation: Listen%1786549073; C++
      int Listen (int ai_bBacklog);

      //## Operation: Write%-1549279123; C++
      int Write (const void* src, int len);

      //## Operation: Read%1555076021; C++
      int Read (void* dst, int len);

      //## Operation: SetBlockingIO%1696209715; C++
      int SetBlockingIO (m4bool_t ai_bBlocking = M4_TRUE);

      //## Operation: GetIdTSAP%-394359780; C++
      int GetIdTSAP ()
      {
        //## begin ClWindowsTSAPImpl::GetIdTSAP%-394359780.body preserve=yes
		//Esta función debiera reemplazarse por GetSystemDescriptor.
		return m_iSocket;
	
        //## end ClWindowsTSAPImpl::GetIdTSAP%-394359780.body
      }

    // Additional Public Declarations
      //## begin ClWindowsTSAPImpl%3777684501AD.public preserve=yes
      //## end ClWindowsTSAPImpl%3777684501AD.public

  protected:
    // Additional Protected Declarations
      //## begin ClWindowsTSAPImpl%3777684501AD.protected preserve=yes
      //## end ClWindowsTSAPImpl%3777684501AD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iSocket%3777684501CD
      //## begin ClWindowsTSAPImpl::m_iSocket%3777684501CD.attr preserve=no  private: int {VA} 
      int m_iSocket;
      //## end ClWindowsTSAPImpl::m_iSocket%3777684501CD.attr

    // Additional Private Declarations
      //## begin ClWindowsTSAPImpl%3777684501AD.private preserve=yes
      //## end ClWindowsTSAPImpl%3777684501AD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWindowsTSAPImpl%3777684501AD.implementation preserve=yes
      //## end ClWindowsTSAPImpl%3777684501AD.implementation

};

//## begin ClWindowsTSAPImpl%3777684501AD.postscript preserve=yes
//## end ClWindowsTSAPImpl%3777684501AD.postscript

//## begin module%377768450199.epilog preserve=yes
//## end module%377768450199.epilog


#endif
