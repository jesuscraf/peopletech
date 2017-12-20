//## begin module%3819AFAA000F.cm preserve=no
//## end module%3819AFAA000F.cm

//## begin module%3819AFAA000F.cp preserve=no
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
//## end module%3819AFAA000F.cp

//## Module: chaninfo%3819AFAA000F; Package specification
//## Subsystem: M4ObjEngine::inc%3819B125003C
//## Source file: D:\ULTIMOSERVER\m4objengine\inc\chaninfo.hpp

#ifndef chaninfo_h
#define chaninfo_h 1

//## begin module%3819AFAA000F.additionalIncludes preserve=no
//## end module%3819AFAA000F.additionalIncludes

//## begin module%3819AFAA000F.includes preserve=yes
//## end module%3819AFAA000F.includes

#include "m4oeinit_dll.hpp"

// m4types
#include <m4types.hpp>
#include "m4stl.hpp"

class ClCMObjectDirectory;
class ClChannelDirectory;
class ClChannel;

//## begin module%3819AFAA000F.declarations preserve=no
//## end module%3819AFAA000F.declarations

//## begin module%3819AFAA000F.additionalDeclarations preserve=yes
//## end module%3819AFAA000F.additionalDeclarations


//## Class: ClChannelsInfo%3819926D03C4
//	Esta clase recolecta información sobre los canales que hay en un ClChannel
//	Directory, para luego poder mostrarla en la Herramienta de Administración.
//
//	Esta clase es necesaria ya que el ciclo de vida de esta información no es el
//	mismo que el del ClChannelDirectory. Por ejemplo en una ejecución OLTP el Cl
//	ChannelDirectory se resetea antes de que se muestre la información en la
//	Herramienta de Administración.
//## Category: M4ObjEngine%38199213031A
//## Subsystem: M4ObjEngine::inc%3819B125003C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3819A6E900F3;m4uint32_t { -> }
//## Uses: <unnamed>%3819B52F0007;ClChannelDirectory { -> F}
//## Uses: <unnamed>%381ED0F60247;ClCompiledMCR { -> F}

class M4_DECL_M4OEINIT ClChannelsInfo 
{
  public:
    //## Class: ClChanInfo%3819A0D40357
    //	Guarda los atributos del ClChannel que nos interesa mostrar en la
    //	Herramienta de Administración.
    //## Category: M4ObjEngine%38199213031A
    //## Subsystem: M4ObjEngine::inc%3819B125003C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    //## Uses: <unnamed>%381ED11B002E;ClChannel { -> F}

    class ClChanInfo 
    {
      public:
        //## Constructors (specified)
          //## Operation: ClChanInfo%941208377
          ClChanInfo (ClChannel& ai_oChannel);

        //## Destructor (specified)
          //## Operation: ~ClChanInfo%941208378
          ~ClChanInfo ();


        //## Other Operations (specified)
          //## Operation: GetId%941208379
          const m4char_t* const GetId () const;

          //## Operation: GetCsType%941208380
          m4uint8_t GetCsType () const;

          //## Operation: ConvertToString%941533947
          //	Nos devuelve la información de un canale, formateada en un stringz.
          //
          //	El formato es el siguiente:
          //	nombre_de_canal (cs_type)
          void ConvertToString (m4char_t* aio_psz) const;

        // Data Members for Class Attributes

          //## Attribute: MAX_STR_LEN%381EB7320289
          //## begin ClChannelsInfo::ClChanInfo::MAX_STR_LEN%381EB7320289.attr preserve=no  public: static m4uint16_t {UAC} M4CL_MAX_T3_ID + 11
          static const m4uint16_t  MAX_STR_LEN;
          //## end ClChannelsInfo::ClChanInfo::MAX_STR_LEN%381EB7320289.attr

      protected:
      private:
        // Data Members for Associations

          //## Association: M4ObjEngine::<unnamed>%3819A7EE0044
          //## Role: ClChanInfo::m_iCsType%3819A7EE03CA
          //## begin ClChannelsInfo::ClChanInfo::m_iCsType%3819A7EE03CA.role preserve=no  private: m4uint8_t { -> 1VHAN}
          m4uint8_t m_iCsType;
          //## end ClChannelsInfo::ClChanInfo::m_iCsType%3819A7EE03CA.role

          //## Association: M4ObjEngine::<unnamed>%3819A7BE02F8
          //## Role: ClChanInfo::m_pszId%3819A7BF01AF
          //## begin ClChannelsInfo::ClChanInfo::m_pszId%3819A7BF01AF.role preserve=no  private: m4char_t { -> 1RFHAN}
          m4char_t *m_pszId;
          //## end ClChannelsInfo::ClChanInfo::m_pszId%3819A7BF01AF.role

      private: //## implementation
    };

  private:
    //## Class: vectorOfClChanInfo%3819B8B5016C; private; Instantiated Class
    //## Category: M4ObjEngine%38199213031A
    //## Subsystem: M4ObjEngine::inc%3819B125003C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    //## Uses: <unnamed>%3819A1700379;ClChanInfo { -> }

    typedef vector< ClChanInfo* > vectorOfClChanInfo;

  public:
    //## Constructors (specified)
      //## Operation: ClChannelsInfo%941208381
      ClChannelsInfo (ClChannelDirectory& ai_oChannelDirectory);

      //## Operation: ClChannelsInfo%943284669
      ClChannelsInfo (ClCMObjectDirectory& ai_oObjectDirectory);

    //## Destructor (specified)
      //## Operation: ~ClChannelsInfo%941208382
      ~ClChannelsInfo ();


    //## Other Operations (specified)
      //## Operation: GetCount%941208383
      m4uint32_t GetCount () const;

      //## Operation: GetChanInfo%941208384
      const ClChannelsInfo::ClChanInfo* const GetChanInfo (const m4uint32_t ai_iIndex) const;

      //## Operation: ConvertToString%941208385
      //	Nos devuelve la información de todos los canales, formateada en un stringz.
      //
      //	El formato es el siguiente:
      //	nombre_de_canal (cs_type), nombre_de_canal (cs_type), ...
      const m4char_t* const ConvertToString ();

  protected:
  private:
    // Data Members for Associations

      //## Association: M4ObjEngine::<unnamed>%3819A70B02DD
      //## Role: ClChannelsInfo::m_pszFormatedString%3819A70C0234
      //## begin ClChannelsInfo::m_pszFormatedString%3819A70C0234.role preserve=no  private: m4char_t { -> 1RFHAN}
      m4char_t *m_pszFormatedString;
      //## end ClChannelsInfo::m_pszFormatedString%3819A70C0234.role

      //## Association: M4ObjEngine::<unnamed>%3819A1D903A2
      //## Role: ClChannelsInfo::m_vectorOfChanInfo%3819A1DA02C9
      //## begin ClChannelsInfo::m_vectorOfChanInfo%3819A1DA02C9.role preserve=no  private: ClChannelsInfo::vectorOfClChanInfo { -> 1VHAN}
      vectorOfClChanInfo m_vectorOfChanInfo;
      //## end ClChannelsInfo::m_vectorOfChanInfo%3819A1DA02C9.role

  private: //## implementation
};

// Class ClChannelsInfo::ClChanInfo 

// Class ClChannelsInfo 

//## begin module%3819AFAA000F.epilog preserve=yes
//## end module%3819AFAA000F.epilog


#endif
