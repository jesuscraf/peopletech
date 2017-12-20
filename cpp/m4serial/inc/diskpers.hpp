//## begin module%3911AA280346.cm preserve=no
//## end module%3911AA280346.cm

//## begin module%3911AA280346.cp preserve=no
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
//## end module%3911AA280346.cp

//## Module: diskpers%3911AA280346; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\diskpers.hpp

#ifndef diskpers_h
#define diskpers_h 1

//## begin module%3911AA280346.additionalIncludes preserve=no
//## end module%3911AA280346.additionalIncludes

//## begin module%3911AA280346.includes preserve=yes

#include "m4serial_dll.hpp"
#include "clprsstb.hpp"
//## end module%3911AA280346.includes

//## begin module%3911AA280346.declarations preserve=no
//## end module%3911AA280346.declarations

//## begin module%3911AA280346.additionalDeclarations preserve=yes

class ClFileIODriver;

//## end module%3911AA280346.additionalDeclarations


//## begin ClDiskPersistor%3911AA280350.preface preserve=yes
//## end ClDiskPersistor%3911AA280350.preface

//## Class: ClDiskPersistor%3911AA280350
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClDiskPersistor : public ClPersistor  //## Inherits: <unnamed>%3911AA2803A2
{
  //## begin ClDiskPersistor%3911AA280350.initialDeclarations preserve=yes
  //## end ClDiskPersistor%3911AA280350.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDiskPersistor%2024729955; C++
      //	#include "chlog.hpp"
      ClDiskPersistor (void );

    //## Destructor (specified)
      //## Operation: ~ClDiskPersistor%546693214; C++
      //	Persist and Depersist a persistiable object
      //	-- {AddDecl: 535} region.unprotectedFunction [886..930]
      ~ClDiskPersistor (void );


    //## Other Operations (specified)
      //## Operation: Persist%-2109917672; C++
      //	-- {AddDecl: 536} region.unprotectedFunction [931..1237]
      virtual m4return_t Persist (ClPersistiableObject& ai_oPersistiableObj);

      //## Operation: DePersist%-788918971; C++
      //	-- {AddDecl: 537} region.unprotectedFunction [1238..1545]
      virtual m4return_t DePersist (ClPersistiableObject& ai_oPersistiableObj);

      //## Operation: Delete%119342329; C++
      //	-- {AddDecl: 538} region.unprotectedFunction [1546..1888]
      virtual m4return_t Delete (ClPersistiableObject& ai_oPersistiableObj);

      //## Operation: InitIODFromObject%1113201133; C++
      //	-- {AddDecl: 539} region.unprotectedFunction [1889..2524]
      m4return_t InitIODFromObject (ClPersistiableObject& ai_oPersistiableObj, ClFileIODriver& ai_IOD, m4uint32_t ai_iMode);

    // Additional Public Declarations
      //## begin ClDiskPersistor%3911AA280350.public preserve=yes
      //## end ClDiskPersistor%3911AA280350.public

  protected:
    // Additional Protected Declarations
      //## begin ClDiskPersistor%3911AA280350.protected preserve=yes
      //## end ClDiskPersistor%3911AA280350.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetFileNameFromObject%-526635167; C++
      //	-- {AddDecl: 540} region.unprotectedFunction [2525..4592]
      m4pchar_t GetFileNameFromObject (ClPersistiableObject& ai_oPersistiableObj, m4bool_t& ao_bMustExist);

      //## Operation: MkDirFromObject%1585648303; C++
      //	-- {AddDecl: 541} region.unprotectedFunction [4593..6036]
      m4return_t MkDirFromObject (ClPersistiableObject& ai_oPersistiableObj);

    // Additional Private Declarations
      //## begin ClDiskPersistor%3911AA280350.private preserve=yes
      //## end ClDiskPersistor%3911AA280350.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDiskPersistor%3911AA280350.implementation preserve=yes
      //## end ClDiskPersistor%3911AA280350.implementation

};

//## begin ClDiskPersistor%3911AA280350.postscript preserve=yes
//## end ClDiskPersistor%3911AA280350.postscript

// Class ClDiskPersistor 

//## begin module%3911AA280346.epilog preserve=yes
//## end module%3911AA280346.epilog


#endif
