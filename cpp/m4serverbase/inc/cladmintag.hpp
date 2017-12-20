//## begin module%35AF75DD014C.cm preserve=no
//## end module%35AF75DD014C.cm

//## begin module%35AF75DD014C.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              cladmintag.hpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines classes to proccess the attributes in the 
//		administration commands.
//
//
//	=============================================================================
//## end module%35AF75DD014C.cp

//## Module: ClAdminTag%35AF75DD014C; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: e:\source\m4serverbase\version\inc\cladmintag.hpp

#ifndef cladmintag_h
#define cladmintag_h 1

//## begin module%35AF75DD014C.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
//## end module%35AF75DD014C.additionalIncludes

//## begin module%35AF75DD014C.includes preserve=yes
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module%35AF75DD014C.includes

class M4_DECL_M4SERVERBASE ClAdminTag;
//## begin module%35AF75DD014C.declarations preserve=no
//## end module%35AF75DD014C.declarations

//## begin module%35AF75DD014C.additionalDeclarations preserve=yes
#include <m4string.hpp>
//## end module%35AF75DD014C.additionalDeclarations


//## begin ClAdminParamType%35AB84CD0150.preface preserve=yes
//## end ClAdminParamType%35AB84CD0150.preface

//## Class: ClAdminParamType%35AB84CD0150; implementation
//	Esta clase es un enumerado con los tipos de datos soportados por el
//	front-end VisualBasic del monitor de  administración.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {invalidType=-1, adminLong=1, adminShort=2, admindouble=3, adminText=4, adminTimeStamp=5, adminSystemDescriptor=100,adminLog=200} ClAdminParamType;

//## begin ClAdminParamType%35AB84CD0150.postscript preserve=yes
//## end ClAdminParamType%35AB84CD0150.postscript

//## begin ClAdminTag%35AB7B4C00BC.preface preserve=yes
//## end ClAdminTag%35AB7B4C00BC.preface

//## Class: ClAdminTag%35AB7B4C00BC
//	Esta clase es un identificador de los atributos que se pasan en los comandos
//	de administracion. El identificador es válido para todos los comandos, bien
//	sean de consulta o de ejecución.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35AB857B001A;ClAdminParamType { -> }

class M4_DECL_M4SERVERBASE ClAdminTag 
{
  //## begin ClAdminTag%35AB7B4C00BC.initialDeclarations preserve=yes
  //## end ClAdminTag%35AB7B4C00BC.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClAdminTag%900494632
      virtual ~ClAdminTag ();


    //## Other Operations (specified)
      //## Operation: GetNewAdminTag%901526259; C++
      static ClAdminTag * GetNewAdminTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, m4bool_t ai_tagWithParams = M4_FALSE, m4bool_t ai_bRetValue = M4_FALSE);

    // Data Members for Class Attributes

      //## Attribute: m_tagId%35AB845B008E
      //	Identificador del comando
      //## begin ClAdminTag::m_tagId%35AB845B008E.attr preserve=no  public: m4int32_t {UA} 
      m4int32_t m_tagId;
      //## end ClAdminTag::m_tagId%35AB845B008E.attr

      //## Attribute: m_tagStringId%35AB848E039E
      //	Identificador del atributo en formato string. Este identificador es el que
      //	sirve para localizar el atributo en el fichero literals.obl y traducirlo a
      //	la cadena que recibirá el cliente del administrador.
      //## begin ClAdminTag::m_tagStringId%35AB848E039E.attr preserve=no  public: m4pchar_t {UA} NULL
      m4pchar_t m_tagStringId;
      //## end ClAdminTag::m_tagStringId%35AB848E039E.attr

      //## Attribute: m_tagType%35AB97D00235
      //	Este atributo indica de que tipo es el parametro que se almacena. El tipo es
      //	uno de los soportados por el cliente del administrador, y están definidos en
      //	el enumerado ClAdminParamType.
      //## begin ClAdminTag::m_tagType%35AB97D00235.attr preserve=no  public: ClAdminParamType {UA} invalidType
      ClAdminParamType m_tagType;
      //## end ClAdminTag::m_tagType%35AB97D00235.attr

      //## Attribute: m_tagValue%35ACA23A01DF
      //	Este atributo es opcional. Actualmente sólo tiene sentido en los parámetros
      //	de los comandos de los subsistemas.
      //	Aqui el subsistema almacena el valor por defecto para ese parámetro cuando
      //	se solicita la lista de parámetros. En la ejecución del comando el
      //	administrador pasa el valor para ese parametro.
      //	Independientemente del tipo del parámetro, que va indicado en m_tagType, el
      //	valor siempre se alcamacena como string.
      //## begin ClAdminTag::m_tagValue%35ACA23A01DF.attr preserve=no  public: m4pchar_t {UA} NULL
      m4pchar_t m_tagValue;
      //## end ClAdminTag::m_tagValue%35ACA23A01DF.attr

      //## Attribute: m_tagWithParameters%35B5A7990253
      //## begin ClAdminTag::m_tagWithParameters%35B5A7990253.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t m_tagWithParameters;
      //## end ClAdminTag::m_tagWithParameters%35B5A7990253.attr

      //## Attribute: m_tagRetValue%380C8D1B030A
      //## begin ClAdminTag::m_tagRetValue%380C8D1B030A.attr preserve=no  public: m4bool_t  {UA} M4_FALSE
      m4bool_t  m_tagRetValue;
      //## end ClAdminTag::m_tagRetValue%380C8D1B030A.attr

    // Additional Public Declarations
      //## begin ClAdminTag%35AB7B4C00BC.public preserve=yes
      //## end ClAdminTag%35AB7B4C00BC.public
  protected:
    //## Constructors (specified)
      //## Operation: ClAdminTag%900494633
      ClAdminTag (const ClAdminTag &ai_tag);

      //## Operation: ClAdminTag%901005788; C++ 
      ClAdminTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, m4bool_t ai_tagWithParams = M4_FALSE, m4bool_t ai_bRetValue = M4_FALSE);


    //## Other Operations (specified)
      //## Operation: operator=%900494634
      ClAdminTag & operator = (const ClAdminTag &ai_tag);

    // Additional Protected Declarations
      //## begin ClAdminTag%35AB7B4C00BC.protected preserve=yes
      //## end ClAdminTag%35AB7B4C00BC.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminTag%35AB7B4C00BC.private preserve=yes
      //## end ClAdminTag%35AB7B4C00BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminTag%35AB7B4C00BC.implementation preserve=yes
      //## end ClAdminTag%35AB7B4C00BC.implementation

};

//## begin ClAdminTag%35AB7B4C00BC.postscript preserve=yes
//## end ClAdminTag%35AB7B4C00BC.postscript

//## begin ClAdminList%35B59A2D02F2.preface preserve=yes
//## end ClAdminList%35B59A2D02F2.preface

//## Class: ClAdminList%35B59A2D02F2; Parameterized Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class _Ty>
class M4_DECL_M4SERVERBASE ClAdminList : public vector<_Ty>  //## Inherits: <unnamed>%35B59B9802F4
{
  //## begin ClAdminList%35B59A2D02F2.initialDeclarations preserve=yes
  //## end ClAdminList%35B59A2D02F2.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: m_subsystemPath%35B59B5F0284
      //## begin ClAdminList::m_subsystemPath%35B59B5F0284.attr preserve=no  public: m4char_t[1024] {UA} 
      m4char_t m_subsystemPath[1024];
      //## end ClAdminList::m_subsystemPath%35B59B5F0284.attr

    // Additional Public Declarations
      //## begin ClAdminList%35B59A2D02F2.public preserve=yes
      //## end ClAdminList%35B59A2D02F2.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminList%35B59A2D02F2.protected preserve=yes
      //## end ClAdminList%35B59A2D02F2.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminList%35B59A2D02F2.private preserve=yes
      //## end ClAdminList%35B59A2D02F2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminList%35B59A2D02F2.implementation preserve=yes
      //## end ClAdminList%35B59A2D02F2.implementation

};

//## begin ClAdminList%35B59A2D02F2.postscript preserve=yes
//## end ClAdminList%35B59A2D02F2.postscript

//## begin ClAdminTagList%35AC748E022D.preface preserve=yes
//## end ClAdminTagList%35AC748E022D.preface

//## Class: ClAdminTagList%35AC748E022D; Instantiated Class
//	Es un vector de punteros a objetos ClAdminTag.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef ClAdminList< ClAdminTag * > ClAdminTagList;

//## begin ClAdminTagList%35AC748E022D.postscript preserve=yes
//## end ClAdminTagList%35AC748E022D.postscript

//## begin ClAdminCommandTag%35AC83EC002B.preface preserve=yes
//## end ClAdminCommandTag%35AC83EC002B.preface

//## Class: ClAdminCommandTag%35AC83EC002B
//	Esta clase es muy similar a su clase base. La diferencia es que incluye un
//	vector de objetos de la clase base, con lo que cuando se pide la información
//	de comandos de un subsistema se puede devolver al mismo tiempo la
//	información de los parámetros de ese comando.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClAdminCommandTag : public ClAdminTag  //## Inherits: <unnamed>%35AC840002A1
{
  //## begin ClAdminCommandTag%35AC83EC002B.initialDeclarations preserve=yes
  //## end ClAdminCommandTag%35AC83EC002B.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetNewAdminCommandTag%901526260; C++
      static ClAdminCommandTag * GetNewAdminCommandTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, ClAdminParamType ai_tagType, const m4pchar_t ai_tagValue, m4bool_t ai_bRetValue = M4_FALSE);

    // Additional Public Declarations
      //## begin ClAdminCommandTag%35AC83EC002B.public preserve=yes
      //## end ClAdminCommandTag%35AC83EC002B.public

  protected:
    //## Constructors (specified)
      //## Operation: ClAdminCommandTag%901005790
      ClAdminCommandTag (m4int32_t ai_tagId, const m4pchar_t ai_tagStringId, ClAdminParamType ai_tagType, const m4pchar_t ai_tagValue, m4bool_t ai_bRetValue = M4_FALSE);

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%3797645F01F4
      //## Role: ClAdminCommandTag::m_parametersList%3797645F03CC
      //## begin ClAdminCommandTag::m_parametersList%3797645F03CC.role preserve=no  protected: ClAdminTagList { -> VHAN}
      ClAdminTagList m_parametersList;
      //## end ClAdminCommandTag::m_parametersList%3797645F03CC.role

    // Additional Protected Declarations
      //## begin ClAdminCommandTag%35AC83EC002B.protected preserve=yes
      //## end ClAdminCommandTag%35AC83EC002B.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminCommandTag%35AC83EC002B.private preserve=yes
      //## end ClAdminCommandTag%35AC83EC002B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminCommandTag%35AC83EC002B.implementation preserve=yes
      //## end ClAdminCommandTag%35AC83EC002B.implementation

};

//## begin ClAdminCommandTag%35AC83EC002B.postscript preserve=yes
//## end ClAdminCommandTag%35AC83EC002B.postscript

//## begin search_by_tag_id_typedef%35BDB1DA01DA.preface preserve=yes
//## end search_by_tag_id_typedef%35BDB1DA01DA.preface

//## Class: search_by_tag_id_typedef%35BDB1DA01DA; Instantiated Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef unary_function< ClAdminTag *,m4bool_t > search_by_tag_id_typedef;

//## begin search_by_tag_id_typedef%35BDB1DA01DA.postscript preserve=yes
//## end search_by_tag_id_typedef%35BDB1DA01DA.postscript

//## begin search_by_tag_id%35BDB1DC00EC.preface preserve=yes
//## end search_by_tag_id%35BDB1DC00EC.preface

//## Class: search_by_tag_id%35BDB1DC00EC
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%369C745A000A;ClAdminTag { -> F}

class M4_DECL_M4SERVERBASE search_by_tag_id : public search_by_tag_id_typedef  //## Inherits: <unnamed>%35BDB31202A1
{
  //## begin search_by_tag_id%35BDB1DC00EC.initialDeclarations preserve=yes
  //## end search_by_tag_id%35BDB1DC00EC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: search_by_tag_id%901621763
      search_by_tag_id (m4int32_t ai_tagId);


    //## Other Operations (specified)
      //## Operation: operator()%901621764
      m4bool_t operator () (const ClAdminTag *ai_poAdminTag) const;

    // Additional Public Declarations
      //## begin search_by_tag_id%35BDB1DC00EC.public preserve=yes
      //## end search_by_tag_id%35BDB1DC00EC.public

  protected:
    // Additional Protected Declarations
      //## begin search_by_tag_id%35BDB1DC00EC.protected preserve=yes
      //## end search_by_tag_id%35BDB1DC00EC.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_key%35BDB53A01C8
      //## begin search_by_tag_id::m_key%35BDB53A01C8.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_key;
      //## end search_by_tag_id::m_key%35BDB53A01C8.attr

    // Additional Private Declarations
      //## begin search_by_tag_id%35BDB1DC00EC.private preserve=yes
      //## end search_by_tag_id%35BDB1DC00EC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin search_by_tag_id%35BDB1DC00EC.implementation preserve=yes
      //## end search_by_tag_id%35BDB1DC00EC.implementation

};

//## begin search_by_tag_id%35BDB1DC00EC.postscript preserve=yes
//## end search_by_tag_id%35BDB1DC00EC.postscript

// Class ClAdminTag 

// Parameterized Class ClAdminList 

// Class ClAdminCommandTag 

// Class search_by_tag_id 

inline search_by_tag_id::search_by_tag_id (m4int32_t ai_tagId)
  //## begin search_by_tag_id::search_by_tag_id%901621763.hasinit preserve=no
  //## end search_by_tag_id::search_by_tag_id%901621763.hasinit
  //## begin search_by_tag_id::search_by_tag_id%901621763.initialization preserve=yes
  : m_key(ai_tagId)
  //## end search_by_tag_id::search_by_tag_id%901621763.initialization
{
  //## begin search_by_tag_id::search_by_tag_id%901621763.body preserve=yes
  //## end search_by_tag_id::search_by_tag_id%901621763.body
}



//## Other Operations (inline)
inline m4bool_t search_by_tag_id::operator () (const ClAdminTag *ai_poAdminTag) const
{
  //## begin search_by_tag_id::operator()%901621764.body preserve=yes
//	MOD EMN	981111
	m4bool_t	bRet;

	bRet = (m4bool_t)(ai_poAdminTag->m_tagId == m_key);
	return bRet;
  //## end search_by_tag_id::operator()%901621764.body
}

//## begin module%35AF75DD014C.epilog preserve=yes

//
//
/*
class search_by_tag_id : public unary_function<ClAdminTag *, m4bool_t>
{
private:
	m4int32_t m_key;
public:
	search_by_tag_id(m4int32_t ai_key) : m_key(ai_key) {}
	m4bool_t operator()(const ClAdminTag *ai_tag) const
	{
		return (ai_tag->m_tagId == m_key);
	}
};
*/
//## end module%35AF75DD014C.epilog


#endif
