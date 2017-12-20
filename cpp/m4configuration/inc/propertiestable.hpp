//## begin module%34D5CAB70097.cm preserve=no
//## end module%34D5CAB70097.cm

//## begin module%34D5CAB70097.cp preserve=no
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
//## end module%34D5CAB70097.cp

//## Module: propertiestable%34D5CAB70097; Package specification
//## Subsystem: M4Configuration::inc%379DD22F012C
//## Source file: F:\integration\M4Configuration\version\inc\propertiestable.hpp

#ifndef propertiestable_h
#define propertiestable_h 1

//## begin module%34D5CAB70097.additionalIncludes preserve=no
//## end module%34D5CAB70097.additionalIncludes

//## begin module%34D5CAB70097.includes preserve=yes

#ifdef _KCC
using std::vector;
using std::map;
using std::less;
#endif
//## end module%34D5CAB70097.includes

// properties
#include <properties.hpp>
// Directory
#include <directory.hpp>
//## begin module%34D5CAB70097.declarations preserve=no
//## end module%34D5CAB70097.declarations

//## begin module%34D5CAB70097.additionalDeclarations preserve=yes
#define ATTRIBUTES_IDS		"ATTRIBUTES.IDS"
#define ATTRIBUTES_SCALE	"ATTRIBUTES.SCALE"
#define ATTRIBUTES_TYPE		"ATTRIBUTES.TYPE"

const m4pchar_t TABLES_INSTANCE		=	"TABLES" ;
const m4pchar_t PROPERTIES_INSTANCE	=	"PROPERTIES" ;
const m4pchar_t LITERALS_INSTANCE	=	"LITERALS" ;
//## end module%34D5CAB70097.additionalDeclarations


//## begin ClTables%34D5B45103C7.preface preserve=yes
//## end ClTables%34D5B45103C7.preface

//## Class: ClTables%34D5B45103C7
//	This class is a group of tables that  are used to look for subsystem
//	configuration datas, as properties ids, properties types, statistics
//	literals, etc.
//## Category: M4Configuration%35F3EFFC0108
//## Subsystem: M4Configuration::inc%379DD22F012C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CONFIGURATION ClTables 
{
  //## begin ClTables%34D5B45103C7.initialDeclarations preserve=yes
  //## end ClTables%34D5B45103C7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTables%886431194
      //	Default constructor
      ClTables ();

    //## Destructor (specified)
      //## Operation: ~ClTables%886431195
      //	Destructor
      ~ClTables ();


    //## Other Operations (specified)
      //## Operation: GetId%886431199
      //	Gets the ID in integer format from the property name string parameter
      m4int16_t GetId (m4pchar_t ai_szProperty);

      //## Operation: GetAttrib%886431200
      //	Gets the type and importance (scale) in integer format from the property
      //	name string parameter
      m4return_t GetAttrib (m4pchar_t ai_szProperty, m4uint16_t &ao_iType, m4char_t &ao_iScale, m4pchar_t &ao_szLiteral);

      //## Operation: GetLiteral%889693443
      m4pchar_t GetLiteral (m4pchar_t ai_szProperty);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%34CF59A4002B
      //## Role: ClTables::m_oBook%34CF59A50203
      //## begin ClTables::m_oBook%34CF59A50203.role preserve=no  public: ClExObl { -> RHN}
      ClExObl *m_oBook;
      //## end ClTables::m_oBook%34CF59A50203.role

      //## Association: Subsystem::<unnamed>%34D5C2560075
      //## Role: ClTables::m_oIds%34D5C25700DA
      //## begin ClTables::m_oIds%34D5C25700DA.role preserve=no  public: ClProperties { -> VHAN}
      ClProperties m_oIds;
      //## end ClTables::m_oIds%34D5C25700DA.role

      //## Association: Subsystem::<unnamed>%34D5C2590015
      //## Role: ClTables::m_oTypes%34D5C25B0235
      //## begin ClTables::m_oTypes%34D5C25B0235.role preserve=no  public: ClProperties { -> VHAN}
      ClProperties m_oTypes;
      //## end ClTables::m_oTypes%34D5C25B0235.role

      //## Association: Subsystem::<unnamed>%34D5C25D030A
      //## Role: ClTables::m_oScales%34D5C25E024D
      //## begin ClTables::m_oScales%34D5C25E024D.role preserve=no  public: ClProperties { -> VHAN}
      ClProperties m_oScales;
      //## end ClTables::m_oScales%34D5C25E024D.role

      //## Association: Subsystem::<unnamed>%34D6009F0109
      //## Role: ClTables::m_oTable%34D600A001D3
      //## begin ClTables::m_oTable%34D600A001D3.role preserve=no  public: ClExObl { -> RHAN}
      ClExObl *m_oTable;
      //## end ClTables::m_oTable%34D600A001D3.role

      //## Association: Subsystem::<unnamed>%34FEA5E101E6
      //## Role: ClTables::m_oLiterals%34FEA5E20197
      //## begin ClTables::m_oLiterals%34FEA5E20197.role preserve=no  public: ClProperties { -> VHAN}
      ClProperties m_oLiterals;
      //## end ClTables::m_oLiterals%34FEA5E20197.role

    // Additional Public Declarations
      //## begin ClTables%34D5B45103C7.public preserve=yes
      //## end ClTables%34D5B45103C7.public

  protected:
    // Additional Protected Declarations
      //## begin ClTables%34D5B45103C7.protected preserve=yes
      //## end ClTables%34D5B45103C7.protected

  private:
    // Additional Private Declarations
      //## begin ClTables%34D5B45103C7.private preserve=yes
      //## end ClTables%34D5B45103C7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTables%34D5B45103C7.implementation preserve=yes
      //## end ClTables%34D5B45103C7.implementation

};

//## begin ClTables%34D5B45103C7.postscript preserve=yes
//## end ClTables%34D5B45103C7.postscript

// Class ClTables 

//## begin module%34D5CAB70097.epilog preserve=yes
//## end module%34D5CAB70097.epilog


#endif
