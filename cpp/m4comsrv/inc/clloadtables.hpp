//## begin module%351B7897011C.cm preserve=no
//## end module%351B7897011C.cm

//## begin module%351B7897011C.cp preserve=no
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
//## end module%351B7897011C.cp

//## Module: ClLoadTables%351B7897011C; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clloadtables.hpp

#ifndef clloadtables_h
#define clloadtables_h 1

//## begin module%351B7897011C.additionalIncludes preserve=no
//## end module%351B7897011C.additionalIncludes

//## begin module%351B7897011C.includes preserve=yes
//## end module%351B7897011C.includes

// propertiestable
#include <propertiestable.hpp>
//## begin module%351B7897011C.declarations preserve=no
//## end module%351B7897011C.declarations

//## begin module%351B7897011C.additionalDeclarations preserve=yes
//## end module%351B7897011C.additionalDeclarations


//## begin ClLoadTables%351B70E6017F.preface preserve=yes
//## end ClLoadTables%351B70E6017F.preface

//## Class: ClLoadTables%351B70E6017F
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClLoadTables : public ClTables  //## Inherits: <unnamed>%351B743A0257
{
  //## begin ClLoadTables%351B70E6017F.initialDeclarations preserve=yes
  //## end ClLoadTables%351B70E6017F.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Initialize%890993636
      //	Initializes the obl object with the file and loads the values into the maps.
      m4return_t Initialize (m4pchar_t ai_szTables, m4pchar_t ai_szLiterals);

      //## Operation: InitializeLiterals%944061593
      m4return_t InitializeLiterals (m4pchar_t ai_szLiterals);

      //## Operation: InitializeProperties%944061594
      m4return_t InitializeProperties (m4pchar_t ai_szProperties);

      //## Operation: InitializeTables%944061595
      m4return_t InitializeTables (m4pchar_t ai_szTables);

    // Additional Public Declarations
      //## begin ClLoadTables%351B70E6017F.public preserve=yes
      //## end ClLoadTables%351B70E6017F.public

  protected:

    //## Other Operations (specified)
      //## Operation: LoadIds%890993633
      //	It takes the pairs literal and id from the obl "tables" and inserts them
      //	into the Ids member map
      m4return_t LoadIds ();

      //## Operation: LoadTypes%890993634
      //	It takes the pairs literal and type from the obl "tables" and inserts them
      //	into the Ids member map.
      //	Each type is an integer that is refered to an especified c++ type.
      m4return_t LoadTypes ();

      //## Operation: LoadScale%890993635
      //	It takes the pairs literal and scale from the obl "tables" and inserts them
      //	into the Ids member map.
      //	Each scale value is an integer that is refered to the importance of the
      //	attribute, statistic or whatever it is
      m4return_t LoadScale ();

      //## Operation: LoadLiterals%890993637
      m4return_t LoadLiterals (m4pchar_t ai_szLiterals);

    // Additional Protected Declarations
      //## begin ClLoadTables%351B70E6017F.protected preserve=yes
      //## end ClLoadTables%351B70E6017F.protected

  private:
    // Additional Private Declarations
      //## begin ClLoadTables%351B70E6017F.private preserve=yes
      //## end ClLoadTables%351B70E6017F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLoadTables%351B70E6017F.implementation preserve=yes
      //## end ClLoadTables%351B70E6017F.implementation

};

//## begin ClLoadTables%351B70E6017F.postscript preserve=yes
//## end ClLoadTables%351B70E6017F.postscript

// Class ClLoadTables 

//## begin module%351B7897011C.epilog preserve=yes
//## end module%351B7897011C.epilog


#endif
