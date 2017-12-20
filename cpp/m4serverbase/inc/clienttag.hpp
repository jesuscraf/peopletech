//## begin module%358FEA96027E.cm preserve=no
//## end module%358FEA96027E.cm

//## begin module%358FEA96027E.cp preserve=no
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
//## end module%358FEA96027E.cp

//## Module: Clienttag%358FEA96027E; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\version\inc\clienttag.hpp

#ifndef clienttag_h
#define clienttag_h 1

//## begin module%358FEA96027E.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
#include "m4stl.hpp"
#include <m4servervt.hpp>

class M4DataStorage;

//## begin module%358FEA96027E.declarations preserve=no
//## end module%358FEA96027E.declarations

//## begin module%358FEA96027E.additionalDeclarations preserve=yes
//## end module%358FEA96027E.additionalDeclarations


//## begin LessString%358FCD7C0249.preface preserve=yes
//## end LessString%358FCD7C0249.preface

//## Class: LessString%358FCD7C0249
//## Category: M4ServerBase::ClTagList_package%358A4049013C
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class LessString 
{
  //## begin LessString%358FCD7C0249.initialDeclarations preserve=yes
  //## end LessString%358FCD7C0249.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator()%898625521; c++
      bool operator () (m4pchar_t ai_pcStr1, m4pchar_t ai_pcStr2) const;

    // Additional Public Declarations
      //## begin LessString%358FCD7C0249.public preserve=yes
      //## end LessString%358FCD7C0249.public

  protected:
    // Additional Protected Declarations
      //## begin LessString%358FCD7C0249.protected preserve=yes
      //## end LessString%358FCD7C0249.protected

  private:
    // Additional Private Declarations
      //## begin LessString%358FCD7C0249.private preserve=yes
      //## end LessString%358FCD7C0249.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin LessString%358FCD7C0249.implementation preserve=yes
      //## end LessString%358FCD7C0249.implementation

};

//## begin LessString%358FCD7C0249.postscript preserve=yes
//## end LessString%358FCD7C0249.postscript

//## begin Tag_Map%3587EB9A022D.preface preserve=yes
//## end Tag_Map%3587EB9A022D.preface

//## Class: Tag_Map%3587EB9A022D; Instantiated Class
//## Category: M4ServerBase::ClTagList_package%358A4049013C
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%358FF2B30022;LessString { -> }

typedef map< m4pchar_t,m4ServerVT,LessString > Tag_Map;

//## begin Tag_Map%3587EB9A022D.postscript preserve=yes
//## end Tag_Map%3587EB9A022D.postscript

//## begin ClTagList%358553B303A7.preface preserve=yes
//## end ClTagList%358553B303A7.preface

//## Class: ClTagList%358553B303A7
//## Category: M4ServerBase::ClTagList_package%358A4049013C
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36DBDEE002E2;m4ServerVT { -> }
//## Uses: <unnamed>%36DBE06D012B;m4ServerVT { -> }

class M4_DECL_M4SERVERBASE ClTagList 
{
  //## begin ClTagList%358553B303A7.initialDeclarations preserve=yes
  //## end ClTagList%358553B303A7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTagList%897932191
      //	Inicializo todo y apunto el tamaño maximo que
      //	puede tener el map.
      ClTagList ();

    //## Destructor (specified)
      //## Operation: ~ClTagList%897932190
      //	Destruye todo.
      ~ClTagList ();


    //## Other Operations (specified)
      //## Operation: SetTag%897932192
      //	Incluye en el map la tag  ai_pcTag devolviendo M4_SUCCESS.
      //
      //	En el caso en que no puediera hacerse (existe un tamaño maximo para los map)
      //	devuelve M4_ERROR.
      //
      //	Ahora mismo le ponesmo el Data
      m4return_t SetTag (m4pchar_t ai_pcTag);

      //## Operation: GetArg%897932193; C++
      //	Obtiene el valor en un VariantType ao_vArg de  la tag  ai_pcTag devolviendo
      //	M4_SUCCESS.
      //
      //	En el caso en que no pudiera hacerlo (no existe) devuelve M4_ERROR.
      m4return_t GetArg (m4pchar_t ai_pcTag, m4ServerVT &ao_vArg);

      //## Operation: DelTag%898005170
      //	Borra la tag  dada y su valor devolviendo M4_SUCCESS.
      //	Si no puediera hacerlo devuelve M4_ERROR.
      m4return_t DelTag (m4pchar_t ai_pcTag);

      //## Operation: Clear%898005171
      //	Borra todas las Tags
      void Clear ();

      //## Operation: SetArg%898080527
      //	Guarda el tag con su arg devolviendo M4_SUCCESS.
      //
      //	Si no pudiera hacerlo devuelve M4_ERROR.
      m4return_t SetArg (m4pchar_t ai_pcTag, m4ServerVT ai_vArg);

      //## Operation: SetArg%911836552; C++
      //	Guarda el tag con su arg en formato string  devolviendo M4_SUCCESS.
      //
      //	Si no pudiera hacerlo devuelve M4_ERROR.
      m4return_t SetArg (m4pchar_t ai_pcString);

      //## Operation: SetArg%911836553; C++
      //	Guarda el tag con su arg que es un string  devolviendo M4_SUCCESS.
      //
      //	Si no pudiera hacerlo devuelve M4_ERROR.
      m4return_t SetArg (m4pchar_t ai_pcTag, m4pchar_t ai_pcString);

      //## Operation: SetArg%911836554; C++
      //	Guarda el tag con su arg que es un double  devolviendo M4_SUCCESS.
      //
      //	Si no pudiera hacerlo devuelve M4_ERROR.
      m4return_t SetArg (m4pchar_t ai_pcTag, m4double_t ai_dNumber);

      //## Operation: GetArg%912423223; C++
      //	Obtiene el valor en unca cadena ao_pcArg de  la tag  ai_pcTag devolviendo M4_
      //	SUCCESS.
      //
      //	En el caso en que no pudiera hacerlo (no existe) devuelve M4_ERROR.
      m4return_t GetArg (m4pchar_t ai_pcTag, m4pchar_t &ao_pcArg, m4uint32_t ai_iSizeBuffer);

      //## Operation: GetNextTag%913214647; C++
      //	Permite iterar a traves de el mapa de Tag.
      //	Antes de iterar es necesario inicializar la iteracion con
      //	StartIteration()
      m4return_t GetNextTag (m4pchar_t ao_pcTagName, m4uint32_t ai_iBufferSize, m4ServerVT& ao_ovArg);

      //## Operation: GetSize%913214648; C++
      //	Nos devuelve el tamaño del mapa de tag usado actualmente
      m4int32_t GetSize ();

      //## Operation: StartIteration%913214649
      //	Inicializa la iteracion sobre el mapa de Tag's
      m4return_t StartIteration ();

      //## Operation: GetNextTag%913214650; C++
      m4return_t GetNextTag (m4pchar_t ao_pcTagArg, m4uint32_t ai_uiBufferSize);

      //## Operation: TagList2DataStorage%913291087; C++
      //	Inserta el contenido de la TagList dentro del DataStorage
      m4return_t TagList2DataStorage (M4DataStorage *&ao_DataStorage);

      //## Operation: DataStorage2TagList%913291088; C++
      //	Inserta el contenido  del DataStorage en la TagList
      m4return_t DataStorage2TagList (M4DataStorage *ai_DataStorage);

      //## Operation: Dump%913717804; C++
      //	Imprime por pantalla todas las Tags.
      //	Realiza un StartIteration()
      m4return_t Dump ();

      //## Operation: GetArg%920306529; C++
      m4return_t GetArg (m4pchar_t ai_pcTag, m4double_t &ao_dNumber);

    // Additional Public Declarations
      //## begin ClTagList%358553B303A7.public preserve=yes
      //## end ClTagList%358553B303A7.public

  protected:
    // Additional Protected Declarations
      //## begin ClTagList%358553B303A7.protected preserve=yes
      //## end ClTagList%358553B303A7.protected

  private:

    //## Other Operations (specified)
      //## Operation: TagArg2Str%913214651; C++
      //	Convierte un par Tag Arg a una cadena.
      //	Actua como serializacion.
      m4return_t TagArg2Str (m4pchar_t ai_pcTagName, m4ServerVT ai_oVariant, m4pchar_t ao_pcBuffer, m4uint32_t ai_uiBufferSize);

      //## Operation: TagArg2Str%913214652; C++
      //	Convierte un par Tag Arg a una cadena.
      //	Actua como serializacion.
      m4return_t TagArg2Str (Tag_Map::iterator ai_iterator, m4pchar_t ao_pcBuffer, m4uint32_t ai_uiBufferSize);

      //## Operation: Str2TagArg%913214653; C++
      //	Convierte una cadena en un par Tag VariantType
      m4return_t Str2TagArg (m4pchar_t ai_pcBuffer, m4pchar_t ao_pcTagName, m4uint32_t ai_uiTagNameSize, m4ServerVT &ao_oVariant);

    // Data Members for Class Attributes

      //## Attribute: m_uMapMaxSize%3586777F0168
      //	Sera el tamaño maximo que puede tener el map.
      //## begin ClTagList::m_uMapMaxSize%3586777F0168.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uMapMaxSize;
      //## end ClTagList::m_uMapMaxSize%3586777F0168.attr

      //## Attribute: m_uMapSize%3587E886019E
      //## begin ClTagList::m_uMapSize%3587E886019E.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uMapSize;
      //## end ClTagList::m_uMapSize%3587E886019E.attr

      //## Attribute: m_TagMapIterator%366E8820029F
      //## begin ClTagList::m_TagMapIterator%366E8820029F.attr preserve=no  private: Tag_Map::iterator {UA} 
      Tag_Map::iterator m_TagMapIterator;
      //## end ClTagList::m_TagMapIterator%366E8820029F.attr

    // Data Members for Associations

      //## Association: M4ServerBase::ClTagList_package::<unnamed>%358FF0BE00D6
      //## Role: ClTagList::m_oTagMap%358FF0BE034C
      //## begin ClTagList::m_oTagMap%358FF0BE034C.role preserve=no  private: Tag_Map {1..n -> 1VHAN}
      Tag_Map m_oTagMap;
      //## end ClTagList::m_oTagMap%358FF0BE034C.role

    // Additional Private Declarations
      //## begin ClTagList%358553B303A7.private preserve=yes
      //## end ClTagList%358553B303A7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTagList%358553B303A7.implementation preserve=yes
      //## end ClTagList%358553B303A7.implementation

};

//## begin ClTagList%358553B303A7.postscript preserve=yes
//## end ClTagList%358553B303A7.postscript

// Class LessString 

// Class ClTagList 

//## begin module%358FEA96027E.epilog preserve=yes
//## end module%358FEA96027E.epilog


#endif
