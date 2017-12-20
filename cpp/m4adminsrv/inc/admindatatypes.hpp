//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              admindatatypes.hpp
//	 Project:           m4adminsrv
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the data types used to manage administration
//		information.
//
//	==============================================================================
//## end module.cp

//## Module: AdminDataTypes; Package specification
//	This component holds the administration data types
//	typedef and classes used to manage every subsystem.
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\inc\admindatatypes.hpp

#ifndef admindatatypes_h
#define admindatatypes_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
// m4types
#include <m4types.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: M4AdminAttrType
//	This enum defines values for M4 property type of
//	administration attributes.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {m4_admin_att_integer=0, m4_admin_att_double, m4_admin_att_string, m4_admin_att_timestamp, m4_admin_att_statistic, admin_subsystem_descriptor} M4AdminAttrType;



//## Class: M4AdminAttrAccessMode
//	This enum defines values for M4 property access-mode of
//	administration attributes.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {m4_admin_att_read_only=0, m4_admin_att_read_write} M4AdminAttrAccessMode;



//## Class: M4AdminAttrAccessPermissions
//	This enum defines values for M4 property
//	access-permissions of administration attributes.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {m4_admin_att_full_access=0, m4_admin_att_restricted_access, m4_admin_att_not_accessible} M4AdminAttrAccessPermissions;



//## Class: M4AdminAttrDescriptor
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef int M4AdminAttrDescriptor;



//## Class: ClAdminMemoryManagement
//	This class implements operators new and delete to use in
//	administration attributes memory allocation. This is a
//	workaround for the MSVC bug "wrong operator delete
//	called for exported class".
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminMemoryManagement 
{
  //## begin ClAdminMemoryManagement.initialDeclarations preserve=yes
  //## end ClAdminMemoryManagement.initialDeclarations

  public:
    //## Destructor (generated)
      virtual ~ClAdminMemoryManagement() = 0;

    //## Storage Management Operations (specified)
      //## Operation: operator new%919702367
      void * operator new (size_t ai_size);

      //## Operation: operator new%919702368
      void * operator new (size_t ai_size, void *ao_p);

      //## Operation: operator delete%919702369
      void operator delete (void *ai_p);

    // Additional Public Declarations
      //## begin ClAdminMemoryManagement.public preserve=yes
      //## end ClAdminMemoryManagement.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminMemoryManagement.protected preserve=yes
      //## end ClAdminMemoryManagement.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminMemoryManagement.private preserve=yes
      //## end ClAdminMemoryManagement.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminMemoryManagement.implementation preserve=yes
      //## end ClAdminMemoryManagement.implementation

};

//## begin ClAdminMemoryManagement.postscript preserve=yes
//## end ClAdminMemoryManagement.postscript



//## Class: ClAdminAttributeSyntax
//	This class holds the syntax definition of a subsystem
//	attribute.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminAttributeSyntax : public ClAdminMemoryManagement  //## Inherits: <unnamed>
{
  //## begin ClAdminAttributeSyntax.initialDeclarations preserve=yes
  //## end ClAdminAttributeSyntax.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminAttributeSyntax%919702361
      //	Constructor.
      ClAdminAttributeSyntax (M4AdminAttrDescriptor ai_attDescriptor, const M4ClString &ai_attName, M4AdminAttrType ai_attType, M4AdminAttrAccessMode ai_attAccessMode = m4_admin_att_read_only, M4AdminAttrAccessPermissions ai_attAccessPermissions = m4_admin_att_full_access);

    //## Destructor (specified)
      //## Operation: ~ClAdminAttributeSyntax%919702362
      virtual ~ClAdminAttributeSyntax ();

    // Data Members for Associations

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2E0000D
      //## Role: ClAdminAttributeSyntax::m_accessMode
      //## begin ClAdminAttributeSyntax::m_accessMode.role preserve=no  public: M4AdminAttrAccessMode { -> UHAN}
      M4AdminAttrAccessMode m_accessMode;
      //## end ClAdminAttributeSyntax::m_accessMode.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2E3009E
      //## Role: ClAdminAttributeSyntax::m_type
      //## begin ClAdminAttributeSyntax::m_type.role preserve=no  public: M4AdminAttrType { -> UHAN}
      M4AdminAttrType m_type;
      //## end ClAdminAttributeSyntax::m_type.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2E60138
      //## Role: ClAdminAttributeSyntax::m_accessPermissions
      //## begin ClAdminAttributeSyntax::m_accessPermissions.role preserve=no  public: M4AdminAttrAccessPermissions { -> UHAN}
      M4AdminAttrAccessPermissions m_accessPermissions;
      //## end ClAdminAttributeSyntax::m_accessPermissions.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2E9001A
      //## Role: ClAdminAttributeSyntax::m_name
      //## begin ClAdminAttributeSyntax::m_name.role preserve=no  public: M4ClString { -> UHAN}
      M4ClString m_name;
      //## end ClAdminAttributeSyntax::m_name.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2EC0263
      //## Role: ClAdminAttributeSyntax::m_value
      //## begin ClAdminAttributeSyntax::m_value.role preserve=no  public: M4ClString { -> UHAN}
      M4ClString m_value;
      //## end ClAdminAttributeSyntax::m_value.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D1A2F60145
      //## Role: ClAdminAttributeSyntax::m_id
      //## begin ClAdminAttributeSyntax::m_id.role preserve=no  public: M4AdminAttrDescriptor { -> UHAN}
      M4AdminAttrDescriptor m_id;
      //## end ClAdminAttributeSyntax::m_id.role

    // Additional Public Declarations
      //## begin ClAdminAttributeSyntax.public preserve=yes
      //## end ClAdminAttributeSyntax.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminAttributeSyntax.protected preserve=yes
      //## end ClAdminAttributeSyntax.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminAttributeSyntax.private preserve=yes
      //## end ClAdminAttributeSyntax.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminAttributeSyntax.implementation preserve=yes
      //## end ClAdminAttributeSyntax.implementation

};

//## begin ClAdminAttributeSyntax.postscript preserve=yes
//## end ClAdminAttributeSyntax.postscript



//## Class: M4AdminCommandDescriptor
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef int M4AdminCommandDescriptor;



//## Class: ClAdminCommandSyntax
//	This class holds the syntax of a subsystem command.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminCommandSyntax : public ClAdminMemoryManagement  //## Inherits: <unnamed>
{
  //## begin ClAdminCommandSyntax.initialDeclarations preserve=yes
  //## end ClAdminCommandSyntax.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminCommandSyntax%919702372
      ClAdminCommandSyntax (M4AdminCommandDescriptor ai_attDescriptor, const M4ClString &ai_attName, m4bool_t ai_hasParameters = M4_FALSE, M4AdminAttrAccessPermissions ai_attAccessPermissions = m4_admin_att_full_access);

    //## Destructor (specified)
      //## Operation: ~ClAdminCommandSyntax%919702373
      ~ClAdminCommandSyntax ();

    // Data Members for Associations

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D28A13021D
      //## Role: ClAdminCommandSyntax::m_accessPermissions
      //## begin ClAdminCommandSyntax::m_accessPermissions.role preserve=no  public: M4AdminAttrAccessPermissions { -> UHAN}
      M4AdminAttrAccessPermissions m_accessPermissions;
      //## end ClAdminCommandSyntax::m_accessPermissions.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D28A4F01C9
      //## Role: ClAdminCommandSyntax::m_id
      //## begin ClAdminCommandSyntax::m_id.role preserve=no  public: M4AdminCommandDescriptor { -> UHAN}
      M4AdminCommandDescriptor m_id;
      //## end ClAdminCommandSyntax::m_id.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D28A8500C2
      //## Role: ClAdminCommandSyntax::m_name
      //## begin ClAdminCommandSyntax::m_name.role preserve=no  public: M4ClString { -> UHAN}
      M4ClString m_name;
      //## end ClAdminCommandSyntax::m_name.role

      //## Association: M4AdminSrv::Administration DataTypes::<unnamed>%36D28A950364
      //## Role: ClAdminCommandSyntax::m_hasParameters
      //## begin ClAdminCommandSyntax::m_hasParameters.role preserve=no  public: m4bool_t { -> UHAN}
      m4bool_t m_hasParameters;
      //## end ClAdminCommandSyntax::m_hasParameters.role

    // Additional Public Declarations
      //## begin ClAdminCommandSyntax.public preserve=yes
      //## end ClAdminCommandSyntax.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminCommandSyntax.protected preserve=yes
      //## end ClAdminCommandSyntax.protected

  private:
    // Additional Private Declarations
      //## begin ClAdminCommandSyntax.private preserve=yes
      //## end ClAdminCommandSyntax.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminCommandSyntax.implementation preserve=yes
      //## end ClAdminCommandSyntax.implementation

};

//## begin ClAdminCommandSyntax.postscript preserve=yes
//## end ClAdminCommandSyntax.postscript



//## Class: ClAdmindCommandList
//	This is a vector of subsystem commands.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminCommandSyntax { -> }
//## Uses: <unnamed>; vector { -> }

typedef vector<ClAdminCommandSyntax *> ClAdmindCommandList;



//## Class: ClAdminAttributeList
//	This is a vector of susbystem attributes.
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; vector { -> }
//## Uses: <unnamed>; ClAdminAttributeSyntax { -> }

typedef vector<ClAdminAttributeSyntax *> ClAdminAttributeList;



//## Class: search_by_id_typedef; Instantiated Class
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef unary_function< ClAdminAttributeSyntax*,m4bool_t > search_by_id_typedef;

//## begin search_by_id_typedef.postscript preserve=yes
//## end search_by_id_typedef.postscript



//## Class: search_by_id
//## Category: M4AdminSrv::Administration DataTypes
//## Subsystem: CPM4AdminSrv
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminAttributeSyntax { -> }

class search_by_id : public search_by_id_typedef  //## Inherits: <unnamed>
{
  //## begin search_by_id.initialDeclarations preserve=yes
  //## end search_by_id.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: search_by_id%919702374
      search_by_id (M4AdminAttrDescriptor ai_id);


    //## Other Operations (specified)
      //## Operation: operator()%919702375
      m4bool_t operator () (const ClAdminAttributeSyntax *ai_poAdminAtt) const;

    // Additional Public Declarations
      //## begin search_by_id.public preserve=yes
      //## end search_by_id.public

  protected:
    // Additional Protected Declarations
      //## begin search_by_id.protected preserve=yes
      //## end search_by_id.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_key
      //	This class is used to find in a list of attributes by
      //	the attribute id.
      //## begin search_by_id::m_key.attr preserve=no  private: M4AdminAttrDescriptor {UA} 
      M4AdminAttrDescriptor m_key;
      //## end search_by_id::m_key.attr

    // Additional Private Declarations
      //## begin search_by_id.private preserve=yes
      //## end search_by_id.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin search_by_id.implementation preserve=yes
      //## end search_by_id.implementation

};

//## begin search_by_id.postscript preserve=yes
//## end search_by_id.postscript



















// Class ClAdminMemoryManagement 





// Class ClAdminAttributeSyntax 









// Class ClAdminCommandSyntax 

















// Class search_by_id 



















//## begin module.epilog preserve=yes
//## end module.epilog
#endif
