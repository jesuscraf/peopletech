//## begin module%3911AA2900C7.cm preserve=no
//## end module%3911AA2900C7.cm

//## begin module%3911AA2900C7.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%3911AA2900C7.cp

//## Module: cldntfbl%3911AA2900C7; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\cldntfbl.cpp

//## begin module%3911AA2900C7.additionalIncludes preserve=no
//## end module%3911AA2900C7.additionalIncludes

//## begin module%3911AA2900C7.includes preserve=yes
//==============================================================================
//
// (c) Copyright  199_-199_ Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Define el interface para objetos identificables, y las clases para formar
//    identificadores de objetos
//
//==============================================================================

#include <string.h>

#include "cldntfbl.hpp"


// ---- Class ClObjectType ------------------------------------------


//## end module%3911AA2900C7.includes

// clsrlzbl
#include <clsrlzbl.hpp>
// m4types
#include <m4types.hpp>
//## begin module%3911AA2900C7.declarations preserve=no
//## end module%3911AA2900C7.declarations

//## begin module%3911AA2900C7.additionalDeclarations preserve=yes

/* #define DO_TYPE_CASE(X,Y) { \
	case X : Y << #X << endl; \
	break; \
	}
*/

#define DO_TYPE_CASE(X,Y) { case X : Y << #X << endl; break; }


ostream & operator << (ostream &ao_Output, ClObjectType &ai_oClObjectType)
{
	m4return_t iRes = ai_oClObjectType.DumpInfo(ao_Output);
	if (iRes != M4_SUCCESS)
		ao_Output << "     Error dumping Object Type Object." << endl;
	return ao_Output;
}


// ---- Class ClEnvironmentId ---------------------------------------




/*
ClObjectId::ClObjectId(      eObjectType ai_eObjectType,
                       m4pcchar_t   ai_string)
{
   // Set the type of the object
   m_oObjectType.SetType(ai_eObjectType);

   // Set de id of the object
   m_id=new char [strlen(ai_string)+1];
   strcpy(m_id, ai_string);
}
*/



ostream & operator << (ostream &ao_Output, ClObjectId &ai_oClObjectId)
{
	m4return_t iRes = ai_oClObjectId.DumpInfo(ao_Output);
	if (iRes != M4_SUCCESS)
		ao_Output << "     Error dumping ObjectID Object." << endl;
	return ao_Output;
}


// ---- Class ClIdentifiableObject ----------------------------------

/*
ClIdentifiableObject::ClIdentifiableObject():
   m_poObjectId  (0)
{
}
*/

//## end module%3911AA2900C7.additionalDeclarations


// Class ClObjectType 



ClObjectType::ClObjectType (void )
  //## begin ClObjectType::ClObjectType%-1955862339.hasinit preserve=no
  //## end ClObjectType::ClObjectType%-1955862339.hasinit
  //## begin ClObjectType::ClObjectType%-1955862339.initialization preserve=yes
:
   m_type(UndefinedObjectType)
  //## end ClObjectType::ClObjectType%-1955862339.initialization
{
  //## begin ClObjectType::ClObjectType%-1955862339.body preserve=yes
  //## end ClObjectType::ClObjectType%-1955862339.body
}

ClObjectType::ClObjectType (const ClObjectType& ai_roObjectType)
  //## begin ClObjectType::ClObjectType%-1754057246.hasinit preserve=no
  //## end ClObjectType::ClObjectType%-1754057246.hasinit
  //## begin ClObjectType::ClObjectType%-1754057246.initialization preserve=yes
  //## end ClObjectType::ClObjectType%-1754057246.initialization
{
  //## begin ClObjectType::ClObjectType%-1754057246.body preserve=yes
   m_type=ai_roObjectType.GetType();
  //## end ClObjectType::ClObjectType%-1754057246.body
}

ClObjectType::ClObjectType (enum eObjectType ai_eObjectType)
  //## begin ClObjectType::ClObjectType%356133023.hasinit preserve=no
  //## end ClObjectType::ClObjectType%356133023.hasinit
  //## begin ClObjectType::ClObjectType%356133023.initialization preserve=yes
:
   m_type(ai_eObjectType)
  //## end ClObjectType::ClObjectType%356133023.initialization
{
  //## begin ClObjectType::ClObjectType%356133023.body preserve=yes
  //## end ClObjectType::ClObjectType%356133023.body
}



//## Other Operations (implementation)
m4bool_t ClObjectType::operator == (const ClObjectType& ai_roObjectType) const
{
  //## begin ClObjectType::operator ==%1425800807.body preserve=yes
   return (m_type==ai_roObjectType.GetType())? M4_TRUE : M4_FALSE;
  //## end ClObjectType::operator ==%1425800807.body
}

void ClObjectType::SetType (enum eObjectType ai_eObjectType)
{
  //## begin ClObjectType::SetType%-1398515932.body preserve=yes
   m_type=ai_eObjectType;
  //## end ClObjectType::SetType%-1398515932.body
}

m4bool_t ClObjectType::operator != (const ClObjectType& ai_roObjectType) const
{
  //## begin ClObjectType::operator !=%-893442710.body preserve=yes
   return (m_type!=ai_roObjectType.GetType())? M4_TRUE : M4_FALSE;
  //## end ClObjectType::operator !=%-893442710.body
}

enum eObjectType ClObjectType::GetType (void ) const
{
  //## begin ClObjectType::GetType%1042897194.body preserve=yes
   return m_type;
  //## end ClObjectType::GetType%1042897194.body
}

m4return_t ClObjectType::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClObjectType::Serialize%1051096846.body preserve=yes
   return ai_roIOD.Write((m4uint8_t)m_type);
  //## end ClObjectType::Serialize%1051096846.body
}

m4return_t ClObjectType::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClObjectType::DeSerialize%-1996810521.body preserve=yes
   m4uint8_t iType;

   if (ai_roIOD.Read(iType)!=M4_SUCCESS)
      return M4_ERROR;

   m_type=(eObjectType)iType;

   return M4_SUCCESS;
  //## end ClObjectType::DeSerialize%-1996810521.body
}

m4return_t ClObjectType::DumpInfo (ostream& ai_oStream)
{
  //## begin ClObjectType::DumpInfo%-23255759.body preserve=yes
	ai_oStream << endl << "     Starting Object type dump info" << endl;
	switch(m_type)
	{
			DO_TYPE_CASE(UndefinedObjectType,ai_oStream) ;
			DO_TYPE_CASE(RequestObjectType,ai_oStream) ;
			DO_TYPE_CASE(ResponseObjectType,ai_oStream) ;
			DO_TYPE_CASE(EnvironmentObjectType,ai_oStream) ;
			DO_TYPE_CASE(ProxyspaceObjectType,ai_oStream) ;
			DO_TYPE_CASE(MetaChannelObjectType,ai_oStream) ;
			DO_TYPE_CASE(ChannelObjectType,ai_oStream) ;
			DO_TYPE_CASE(ChannelDataObjectType,ai_oStream) ;
			DO_TYPE_CASE(ObjDataObjectType,ai_oStream) ;
			DO_TYPE_CASE(AccessObjectType,ai_oStream) ;
			DO_TYPE_CASE(NodeObjectType,ai_oStream) ;
			DO_TYPE_CASE(BlockObjectType,ai_oStream) ;
			DO_TYPE_CASE(OLTPExecutionContextType,ai_oStream) ;
			DO_TYPE_CASE(ClientServerResultObjectType,ai_oStream) ;
			DO_TYPE_CASE(CacheObjectType,ai_oStream) ;
			DO_TYPE_CASE(MetaSecurityObjectType,ai_oStream) ;
			DO_TYPE_CASE(PresentationObjectType,ai_oStream) ;
			DO_TYPE_CASE(LastObjectType,ai_oStream) ;
			DO_TYPE_CASE(ExecutorStateObjectType,ai_oStream) ;
	default:
		ai_oStream << "INVALID TYPE DETECTED" << endl;
	}

	ai_oStream << "     Finishing Object type dump info" << endl;

	return M4_SUCCESS;
  //## end ClObjectType::DumpInfo%-23255759.body
}

// Additional Declarations
  //## begin ClObjectType%3911AA240282.declarations preserve=yes
  //## end ClObjectType%3911AA240282.declarations

// Class ClEnvironmentId 


ClEnvironmentId::ClEnvironmentId (void )
  //## begin ClEnvironmentId::ClEnvironmentId%-1648070787.hasinit preserve=no
  //## end ClEnvironmentId::ClEnvironmentId%-1648070787.hasinit
  //## begin ClEnvironmentId::ClEnvironmentId%-1648070787.initialization preserve=yes
:
   m_id(0)
  //## end ClEnvironmentId::ClEnvironmentId%-1648070787.initialization
{
  //## begin ClEnvironmentId::ClEnvironmentId%-1648070787.body preserve=yes
  //## end ClEnvironmentId::ClEnvironmentId%-1648070787.body
}

ClEnvironmentId::ClEnvironmentId (const ClEnvironmentId& ai_roEnvironmentId)
  //## begin ClEnvironmentId::ClEnvironmentId%1914995882.hasinit preserve=no
  //## end ClEnvironmentId::ClEnvironmentId%1914995882.hasinit
  //## begin ClEnvironmentId::ClEnvironmentId%1914995882.initialization preserve=yes
  //## end ClEnvironmentId::ClEnvironmentId%1914995882.initialization
{
  //## begin ClEnvironmentId::ClEnvironmentId%1914995882.body preserve=yes
   m_id=new m4char_t [strlen(ai_roEnvironmentId.GetId())+1];
   strcpy(m_id, ai_roEnvironmentId.GetId());
  //## end ClEnvironmentId::ClEnvironmentId%1914995882.body
}

ClEnvironmentId::ClEnvironmentId (const ClEnvironment& ai_roEnvironment)
  //## begin ClEnvironmentId::ClEnvironmentId%-1503812114.hasinit preserve=no
  //## end ClEnvironmentId::ClEnvironmentId%-1503812114.hasinit
  //## begin ClEnvironmentId::ClEnvironmentId%-1503812114.initialization preserve=yes
  //## end ClEnvironmentId::ClEnvironmentId%-1503812114.initialization
{
  //## begin ClEnvironmentId::ClEnvironmentId%-1503812114.body preserve=yes
   // ??? Como no se como es un entorno no se como crear un Id basandomen en él
   // ??? por ahora lo dejo a 0
   m_id=0;
  //## end ClEnvironmentId::ClEnvironmentId%-1503812114.body
}

ClEnvironmentId::ClEnvironmentId (m4pcchar_t ai_string)
  //## begin ClEnvironmentId::ClEnvironmentId%946509623.hasinit preserve=no
  //## end ClEnvironmentId::ClEnvironmentId%946509623.hasinit
  //## begin ClEnvironmentId::ClEnvironmentId%946509623.initialization preserve=yes
  //## end ClEnvironmentId::ClEnvironmentId%946509623.initialization
{
  //## begin ClEnvironmentId::ClEnvironmentId%946509623.body preserve=yes
   m_id=new m4char_t [strlen(ai_string)+1];
   strcpy(m_id, ai_string);
  //## end ClEnvironmentId::ClEnvironmentId%946509623.body
}


ClEnvironmentId::~ClEnvironmentId (void )
{
  //## begin ClEnvironmentId::~ClEnvironmentId%-357777085.body preserve=yes
   if (m_id) delete[] m_id;
  //## end ClEnvironmentId::~ClEnvironmentId%-357777085.body
}



//## Other Operations (implementation)
m4bool_t ClEnvironmentId::operator == (const ClEnvironmentId& ai_roEnvironmentId) const
{
  //## begin ClEnvironmentId::operator ==%1014829916.body preserve=yes
   return (strcmp(m_id, ai_roEnvironmentId.GetId())==0)? M4_TRUE : M4_FALSE;
  //## end ClEnvironmentId::operator ==%1014829916.body
}

m4bool_t ClEnvironmentId::operator != (const ClEnvironmentId& ai_roEnvironmentId) const
{
  //## begin ClEnvironmentId::operator !=%810395447.body preserve=yes
   return (strcmp(m_id, ai_roEnvironmentId.GetId())!=0)? M4_TRUE : M4_FALSE;
  //## end ClEnvironmentId::operator !=%810395447.body
}

m4return_t ClEnvironmentId::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClEnvironmentId::Serialize%1325898263.body preserve=yes
   return ai_roIOD.Write(m_id, (m4uint32_t)strlen(m_id));
  //## end ClEnvironmentId::Serialize%1325898263.body
}

m4return_t ClEnvironmentId::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClEnvironmentId::DeSerialize%-1445095051.body preserve=yes
   m4uint32_t iSize;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_id=new m4char_t [iSize+1];
   if (!m_id)
      return M4_ERROR;     // No hay memoria

   if (ai_roIOD.ReadBuffer(m_id)!=M4_SUCCESS)
      return M4_ERROR;
   m_id[iSize]=0;          // El nil del final

   return M4_SUCCESS;
  //## end ClEnvironmentId::DeSerialize%-1445095051.body
}

m4pchar_t ClEnvironmentId::GetId (void ) const
{
  //## begin ClEnvironmentId::GetId%1562027569.body preserve=yes
   return m_id;
  //## end ClEnvironmentId::GetId%1562027569.body
}

// Additional Declarations
  //## begin ClEnvironmentId%3911AA240393.declarations preserve=yes
  //## end ClEnvironmentId%3911AA240393.declarations

// Class ClTypedId 


ClTypedId::ClTypedId (void )
  //## begin ClTypedId::ClTypedId%-930413434.hasinit preserve=no
  //## end ClTypedId::ClTypedId%-930413434.hasinit
  //## begin ClTypedId::ClTypedId%-930413434.initialization preserve=yes
:
   m_id(0)
  //## end ClTypedId::ClTypedId%-930413434.initialization
{
  //## begin ClTypedId::ClTypedId%-930413434.body preserve=yes
  //## end ClTypedId::ClTypedId%-930413434.body
}

ClTypedId::ClTypedId (const ClTypedId& ai_roTypedId)
  //## begin ClTypedId::ClTypedId%1582595709.hasinit preserve=no
  //## end ClTypedId::ClTypedId%1582595709.hasinit
  //## begin ClTypedId::ClTypedId%1582595709.initialization preserve=yes
  //## end ClTypedId::ClTypedId%1582595709.initialization
{
  //## begin ClTypedId::ClTypedId%1582595709.body preserve=yes
   m_id=new m4char_t [strlen(ai_roTypedId.GetId())+1];
   strcpy(m_id, ai_roTypedId.GetId());
  //## end ClTypedId::ClTypedId%1582595709.body
}

ClTypedId::ClTypedId (m4pcchar_t ai_string)
  //## begin ClTypedId::ClTypedId%-701555389.hasinit preserve=no
  //## end ClTypedId::ClTypedId%-701555389.hasinit
  //## begin ClTypedId::ClTypedId%-701555389.initialization preserve=yes
  //## end ClTypedId::ClTypedId%-701555389.initialization
{
  //## begin ClTypedId::ClTypedId%-701555389.body preserve=yes
   m_id=new m4char_t [strlen(ai_string)+1];
   strcpy(m_id, ai_string);
  //## end ClTypedId::ClTypedId%-701555389.body
}


ClTypedId::~ClTypedId (void )
{
  //## begin ClTypedId::~ClTypedId%507446288.body preserve=yes
   if (m_id) delete[] m_id;
  //## end ClTypedId::~ClTypedId%507446288.body
}



//## Other Operations (implementation)
m4bool_t ClTypedId::operator == (const ClTypedId& ai_roTypedId) const
{
  //## begin ClTypedId::operator ==%667498224.body preserve=yes
   return (strcmp(m_id, ai_roTypedId.GetId())==0)? M4_TRUE : M4_FALSE;
  //## end ClTypedId::operator ==%667498224.body
}

m4bool_t ClTypedId::operator != (const ClTypedId& ai_roTypedId) const
{
  //## begin ClTypedId::operator !=%370000431.body preserve=yes
   return (strcmp(m_id, ai_roTypedId.GetId())!=0)? M4_TRUE : M4_FALSE;
  //## end ClTypedId::operator !=%370000431.body
}

m4return_t ClTypedId::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClTypedId::Serialize%-1555423904.body preserve=yes
   return ai_roIOD.Write(m_id, (m4uint32_t)strlen(m_id));
  //## end ClTypedId::Serialize%-1555423904.body
}

m4return_t ClTypedId::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClTypedId::DeSerialize%-1505914124.body preserve=yes
   m4uint32_t iSize;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_id=new m4char_t [iSize+1];
   if (!m_id)
      return M4_ERROR;     // No hay memoria

   if (ai_roIOD.ReadBuffer(m_id)!=M4_SUCCESS)
      return M4_ERROR;
   m_id[iSize]=0;          // El nil del final

   return M4_SUCCESS;
  //## end ClTypedId::DeSerialize%-1505914124.body
}

m4pchar_t ClTypedId::GetId (void ) const
{
  //## begin ClTypedId::GetId%-90383977.body preserve=yes
   return m_id;
  //## end ClTypedId::GetId%-90383977.body
}

// Additional Declarations
  //## begin ClTypedId%3911AA250062.declarations preserve=yes
  //## end ClTypedId%3911AA250062.declarations

// Class ClUniqueKey 


ClUniqueKey::ClUniqueKey (void )
  //## begin ClUniqueKey::ClUniqueKey%780596879.hasinit preserve=no
  //## end ClUniqueKey::ClUniqueKey%780596879.hasinit
  //## begin ClUniqueKey::ClUniqueKey%780596879.initialization preserve=yes
:
   m_key(0)
  //## end ClUniqueKey::ClUniqueKey%780596879.initialization
{
  //## begin ClUniqueKey::ClUniqueKey%780596879.body preserve=yes
  //## end ClUniqueKey::ClUniqueKey%780596879.body
}

ClUniqueKey::ClUniqueKey (const ClUniqueKey& ai_roUniqueKey)
  //## begin ClUniqueKey::ClUniqueKey%1449227943.hasinit preserve=no
  //## end ClUniqueKey::ClUniqueKey%1449227943.hasinit
  //## begin ClUniqueKey::ClUniqueKey%1449227943.initialization preserve=yes
  //## end ClUniqueKey::ClUniqueKey%1449227943.initialization
{
  //## begin ClUniqueKey::ClUniqueKey%1449227943.body preserve=yes
   m_key=new m4char_t [strlen(ai_roUniqueKey.GetKey())+1];
   strcpy(m_key, ai_roUniqueKey.GetKey());
  //## end ClUniqueKey::ClUniqueKey%1449227943.body
}

ClUniqueKey::ClUniqueKey (m4pcchar_t ai_string)
  //## begin ClUniqueKey::ClUniqueKey%1888177138.hasinit preserve=no
  //## end ClUniqueKey::ClUniqueKey%1888177138.hasinit
  //## begin ClUniqueKey::ClUniqueKey%1888177138.initialization preserve=yes
  //## end ClUniqueKey::ClUniqueKey%1888177138.initialization
{
  //## begin ClUniqueKey::ClUniqueKey%1888177138.body preserve=yes
   m_key=new m4char_t [strlen(ai_string)+1];
   strcpy(m_key, ai_string);
  //## end ClUniqueKey::ClUniqueKey%1888177138.body
}


ClUniqueKey::~ClUniqueKey (void )
{
  //## begin ClUniqueKey::~ClUniqueKey%-777551699.body preserve=yes
   if (m_key) delete[] m_key;
  //## end ClUniqueKey::~ClUniqueKey%-777551699.body
}



//## Other Operations (implementation)
m4bool_t ClUniqueKey::operator == (const ClUniqueKey& ai_roUniqueKey) const
{
  //## begin ClUniqueKey::operator ==%402743725.body preserve=yes
   return (strcmp(m_key, ai_roUniqueKey.GetKey())==0)? M4_TRUE : M4_FALSE;
  //## end ClUniqueKey::operator ==%402743725.body
}

m4bool_t ClUniqueKey::operator != (const ClUniqueKey& ai_roUniqueKey) const
{
  //## begin ClUniqueKey::operator !=%1915945393.body preserve=yes
   return (strcmp(m_key, ai_roUniqueKey.GetKey())!=0)? M4_TRUE : M4_FALSE;
  //## end ClUniqueKey::operator !=%1915945393.body
}

m4return_t ClUniqueKey::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClUniqueKey::Serialize%-675907019.body preserve=yes
   return ai_roIOD.Write(m_key, (m4uint32_t)strlen(m_key));
  //## end ClUniqueKey::Serialize%-675907019.body
}

m4return_t ClUniqueKey::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClUniqueKey::DeSerialize%1437480156.body preserve=yes
   m4uint32_t iSize;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_key=new m4char_t [iSize+1];
   if (!m_key)
      return M4_ERROR;     // No hay memoria

   if (ai_roIOD.ReadBuffer(m_key)!=M4_SUCCESS)
      return M4_ERROR;
   m_key[iSize]=0;         // El nil del final

   return M4_SUCCESS;
  //## end ClUniqueKey::DeSerialize%1437480156.body
}

m4pchar_t ClUniqueKey::GetKey (void ) const
{
  //## begin ClUniqueKey::GetKey%10258674.body preserve=yes
   return m_key;
  //## end ClUniqueKey::GetKey%10258674.body
}

// Additional Declarations
  //## begin ClUniqueKey%3911AA2500E7.declarations preserve=yes
  //## end ClUniqueKey%3911AA2500E7.declarations

// Class ClObjectId 







ClObjectId::ClObjectId (void )
  //## begin ClObjectId::ClObjectId%-1970467006.hasinit preserve=no
  //## end ClObjectId::ClObjectId%-1970467006.hasinit
  //## begin ClObjectId::ClObjectId%-1970467006.initialization preserve=yes
:
   m_id           (NULL),
   m_environmentId(NULL),
   m_typedId      (NULL),
   m_uniqueKey    (NULL)

  //## end ClObjectId::ClObjectId%-1970467006.initialization
{
  //## begin ClObjectId::ClObjectId%-1970467006.body preserve=yes
   m_oObjectType.SetType(UndefinedObjectType);
  //## end ClObjectId::ClObjectId%-1970467006.body
}

ClObjectId::ClObjectId (const ClObjectId& ai_roObjectId)
  //## begin ClObjectId::ClObjectId%1581107277.hasinit preserve=no
  //## end ClObjectId::ClObjectId%1581107277.hasinit
  //## begin ClObjectId::ClObjectId%1581107277.initialization preserve=yes
  //## end ClObjectId::ClObjectId%1581107277.initialization
{
  //## begin ClObjectId::ClObjectId%1581107277.body preserve=yes
   // Set the type of the object
   m_oObjectType.SetType(ai_roObjectId.GetType());

   // Set de id of the object
   m_id=new m4char_t [strlen(ai_roObjectId.GetId())+1];
   strcpy(m_id, ai_roObjectId.GetId());

   m_environmentId=new m4char_t [strlen(ai_roObjectId.GetEnvironmentId())+1];
   strcpy(m_environmentId, ai_roObjectId.GetEnvironmentId());

   m_typedId=new m4char_t [strlen(ai_roObjectId.GetTypedId())+1];
   strcpy(m_typedId, ai_roObjectId.GetTypedId());

   m_uniqueKey=new m4char_t [strlen(ai_roObjectId.GetUniqueKey())+1];
   strcpy(m_uniqueKey, ai_roObjectId.GetUniqueKey());
  //## end ClObjectId::ClObjectId%1581107277.body
}

ClObjectId::ClObjectId (enum eObjectType ai_eObjectType, m4pcchar_t ai_pcEnvironmentId, m4pcchar_t ai_pcTypedId, m4pcchar_t ai_pcUniqueKey)
  //## begin ClObjectId::ClObjectId%-543994594.hasinit preserve=no
  //## end ClObjectId::ClObjectId%-543994594.hasinit
  //## begin ClObjectId::ClObjectId%-543994594.initialization preserve=yes
:
   m_id(NULL),
   m_environmentId(NULL),
   m_typedId(NULL),
   m_uniqueKey(NULL)

  //## end ClObjectId::ClObjectId%-543994594.initialization
{
  //## begin ClObjectId::ClObjectId%-543994594.body preserve=yes
   m_oObjectType.SetType(ai_eObjectType);

   // Copiamos los "subidentificadores"
   if (ai_pcEnvironmentId)
   {
      m_environmentId=new m4char_t [strlen(ai_pcEnvironmentId)+1];
      strcpy(m_environmentId, ai_pcEnvironmentId);
   }

   if (ai_pcTypedId)
   {
      m_typedId=new m4char_t [strlen(ai_pcTypedId)+1];
      strcpy(m_typedId, ai_pcTypedId);
   }

   if (ai_pcUniqueKey)
   {
      m_uniqueKey=new m4char_t [strlen(ai_pcUniqueKey)+1];
      strcpy(m_uniqueKey, ai_pcUniqueKey);
   }

   // Concatenamos los "subidentificadores"
   _oidcat();
  //## end ClObjectId::ClObjectId%-543994594.body
}

ClObjectId::ClObjectId (enum eObjectType ai_eObjectType, const ClEnvironmentId* ai_poEnvironmentId, const ClTypedId* ai_poTypedId, const ClUniqueKey* ai_poUniqueKey)
  //## begin ClObjectId::ClObjectId%-996875636.hasinit preserve=no
  //## end ClObjectId::ClObjectId%-996875636.hasinit
  //## begin ClObjectId::ClObjectId%-996875636.initialization preserve=yes
:
   m_id(NULL),
   m_environmentId(NULL),
   m_typedId(NULL),
   m_uniqueKey(NULL)
  //## end ClObjectId::ClObjectId%-996875636.initialization
{
  //## begin ClObjectId::ClObjectId%-996875636.body preserve=yes
   // Fijamos el tipo
   m_oObjectType.SetType(ai_eObjectType);

   // Copiamos los "subidentificadores"
   if (ai_poEnvironmentId)
   {
      m_environmentId=new m4char_t [strlen(ai_poEnvironmentId->GetId())+1];
      strcpy(m_environmentId, ai_poEnvironmentId->GetId());
   }

   if (ai_poTypedId)
   {
      m_typedId=new m4char_t [strlen(ai_poTypedId->GetId())+1];
      strcpy(m_typedId, ai_poTypedId->GetId());
   }

   if (ai_poUniqueKey)
   {
      m_uniqueKey=new m4char_t [strlen(ai_poUniqueKey->GetKey())+1];
      strcpy(m_uniqueKey, ai_poUniqueKey->GetKey());
   }

   // Concatenamos los "subidentificadores"
   _oidcat();
  //## end ClObjectId::ClObjectId%-996875636.body
}


ClObjectId::~ClObjectId (void )
{
  //## begin ClObjectId::~ClObjectId%446757455.body preserve=yes
   if (m_id) delete[] m_id;
   if (m_environmentId) delete[] m_environmentId;
   if (m_typedId) delete[] m_typedId;
   if (m_uniqueKey) delete[] m_uniqueKey;
  //## end ClObjectId::~ClObjectId%446757455.body
}



//## Other Operations (implementation)
m4bool_t ClObjectId::operator == (const ClObjectId& ai_roObjectId) const
{
  //## begin ClObjectId::operator ==%-1789238716.body preserve=yes
   // Para que sean iguales tienen que ser iguales el tipo y el id
   return (m_oObjectType.GetType()            ==ai_roObjectId.GetType() &&
           strcmp(m_id, ai_roObjectId.GetId())==0)? M4_TRUE : M4_FALSE;
  //## end ClObjectId::operator ==%-1789238716.body
}

m4bool_t ClObjectId::operator != (const ClObjectId& ai_roObjectId) const
{
  //## begin ClObjectId::operator !=%296438034.body preserve=yes
   // Para que sean distintos basta con que lo sea el tipo o el id
   return (m_oObjectType.GetType()            !=ai_roObjectId.GetType() ||
           strcmp(m_id, ai_roObjectId.GetId())!=0)? M4_TRUE : M4_FALSE;
  //## end ClObjectId::operator !=%296438034.body
}

enum eObjectType ClObjectId::GetType (void ) const
{
  //## begin ClObjectId::GetType%-2044762307.body preserve=yes
   return m_oObjectType.GetType();
  //## end ClObjectId::GetType%-2044762307.body
}

m4return_t ClObjectId::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClObjectId::Serialize%-1353828062.body preserve=yes
   // El tipo
   m_oObjectType.Serialize(ai_roIOD);

   // Los "subidentificadores"
   m4uint16_t  iSize;

   if (m_environmentId)
      iSize=(m4uint16_t)strlen(m_environmentId);
   else
      iSize=0;
   if (ai_roIOD.Write(m_environmentId, iSize) != M4_SUCCESS)
      return M4_ERROR;

   if (m_typedId)
      iSize=(m4uint16_t)strlen(m_typedId);
   else
      iSize=0;
   if (ai_roIOD.Write(m_typedId, iSize) != M4_SUCCESS)
      return M4_ERROR;

   if (m_uniqueKey)
      iSize=(m4uint16_t)strlen(m_uniqueKey);
   else
      iSize=0;
   return ai_roIOD.Write(m_uniqueKey, iSize);
  //## end ClObjectId::Serialize%-1353828062.body
}

m4return_t ClObjectId::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClObjectId::DeSerialize%1664292358.body preserve=yes
   // El tipo
   m_oObjectType.DeSerialize(ai_roIOD);

   // Los "subidentificadores"
   m4uint32_t iSize;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;
   if (iSize)
   {
      m_environmentId=new m4char_t [iSize+1];
      if (!m_environmentId)
         return M4_ERROR;        // No hay memoria

      if (ai_roIOD.ReadBuffer(m_environmentId)!=M4_SUCCESS)
         return M4_ERROR;
      m_environmentId[iSize]=0;  // El nil del final
   }
   else
      m_environmentId=NULL;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;
   if (iSize)
   {
      m_typedId=new m4char_t [iSize+1];
      if (!m_typedId)
         return M4_ERROR;        // No hay memoria

      if (ai_roIOD.ReadBuffer(m_typedId)!=M4_SUCCESS)
         return M4_ERROR;
      m_typedId[iSize]=0;        // El nil del final
   }
   else
      m_typedId=NULL;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;
   if (iSize)
   {
      m_uniqueKey=new m4char_t [iSize+1];
      if (!m_uniqueKey)
         return M4_ERROR;        // No hay memoria

      if (ai_roIOD.ReadBuffer(m_uniqueKey)!=M4_SUCCESS)
         return M4_ERROR;
      m_uniqueKey[iSize]=0;      // El nil del final
   }
   else
      m_uniqueKey=NULL;

   // Concatenamos los "subidentificadores"
   _oidcat();

   return M4_SUCCESS;
  //## end ClObjectId::DeSerialize%1664292358.body
}

m4return_t ClObjectId::DumpInfo (ostream& ai_oStream)
{
  //## begin ClObjectId::DumpInfo%201084953.body preserve=yes
	ai_oStream << endl << "     Starting Object ID dump info" << endl;	
	
	if (m_id != NULL)
		ai_oStream     << "     m_id            = <" << m_id << ">" << endl;

	if (m_environmentId != NULL)
		ai_oStream     << "     m_environmentId = <" << m_environmentId << ">" << endl;
	
	if (m_typedId != NULL)
		ai_oStream     << "     m_typedId       = <" << m_typedId << ">" << endl;
	
	if (m_uniqueKey != NULL)
		ai_oStream     << "     m_uniqueKey     = <" << m_uniqueKey << ">" << endl;

	ai_oStream << m_oObjectType;
		
	ai_oStream << "     Finishing Object ID dump info" << endl;	
	
	return M4_SUCCESS;
  //## end ClObjectId::DumpInfo%201084953.body
}

m4pchar_t ClObjectId::GetId (void ) const
{
  //## begin ClObjectId::GetId%505784152.body preserve=yes
   return m_id;
  //## end ClObjectId::GetId%505784152.body
}

m4pchar_t ClObjectId::GetEnvironmentId (void ) const
{
  //## begin ClObjectId::GetEnvironmentId%1775905917.body preserve=yes
   return m_environmentId;
  //## end ClObjectId::GetEnvironmentId%1775905917.body
}

m4pchar_t ClObjectId::GetTypedId (void ) const
{
  //## begin ClObjectId::GetTypedId%-1128067222.body preserve=yes
   return m_typedId;
  //## end ClObjectId::GetTypedId%-1128067222.body
}

m4pchar_t ClObjectId::GetUniqueKey (void ) const
{
  //## begin ClObjectId::GetUniqueKey%1429022585.body preserve=yes
   return m_uniqueKey;
  //## end ClObjectId::GetUniqueKey%1429022585.body
}

m4pchar_t ClObjectId::_oidcat (void )
{
  //## begin ClObjectId::_oidcat%1074933201.body preserve=yes
   // Suma de las longitudes
   m4uint16_t length=0;

   if (m_environmentId)
      length+= (m4uint16_t)strlen(m_environmentId);

   if (m_typedId)
      length+=(m4uint16_t)strlen(m_typedId);

   if (m_uniqueKey)
      length+=(m4uint16_t)strlen(m_uniqueKey);

   // Concatenación de todos los "subidentificadores"
   if (length)
   {
      if (m_id) delete[] m_id;   // Por si acaso
      m_id=new m4char_t [length+1];
      m_id[0]=0;     // Para que funcionene las concatenaciones si m_environmentId=NULL

      if (m_environmentId)
         strcpy(m_id, m_environmentId);

      if (m_typedId)
         strcat(m_id, m_typedId);

      if (m_uniqueKey)
         strcat(m_id, m_uniqueKey);
   }

   return m_id;
  //## end ClObjectId::_oidcat%1074933201.body
}

// Additional Declarations
  //## begin ClObjectId%3911AA250169.declarations preserve=yes
  //## end ClObjectId%3911AA250169.declarations

// Class ClIdentifiableObject 


ClIdentifiableObject::ClIdentifiableObject (const ClIdentifiableObject& ai_roIdentifiableObject)
  //## begin ClIdentifiableObject::ClIdentifiableObject%-695804480.hasinit preserve=no
  //## end ClIdentifiableObject::ClIdentifiableObject%-695804480.hasinit
  //## begin ClIdentifiableObject::ClIdentifiableObject%-695804480.initialization preserve=yes
  //## end ClIdentifiableObject::ClIdentifiableObject%-695804480.initialization
{
  //## begin ClIdentifiableObject::ClIdentifiableObject%-695804480.body preserve=yes
   m_poObjectId=new ClObjectId(*(ai_roIdentifiableObject.GetObjId()));
  //## end ClIdentifiableObject::ClIdentifiableObject%-695804480.body
}

ClIdentifiableObject::ClIdentifiableObject (ClObjectId* ai_poObjectId)
  //## begin ClIdentifiableObject::ClIdentifiableObject%1313486248.hasinit preserve=no
  //## end ClIdentifiableObject::ClIdentifiableObject%1313486248.hasinit
  //## begin ClIdentifiableObject::ClIdentifiableObject%1313486248.initialization preserve=yes
:
   m_poObjectId  (ai_poObjectId)
  //## end ClIdentifiableObject::ClIdentifiableObject%1313486248.initialization
{
  //## begin ClIdentifiableObject::ClIdentifiableObject%1313486248.body preserve=yes
  //## end ClIdentifiableObject::ClIdentifiableObject%1313486248.body
}


ClIdentifiableObject::~ClIdentifiableObject (void )
{
  //## begin ClIdentifiableObject::~ClIdentifiableObject%-21800869.body preserve=yes
	if (m_poObjectId)
		delete m_poObjectId;
  //## end ClIdentifiableObject::~ClIdentifiableObject%-21800869.body
}



//## Other Operations (implementation)
m4pchar_t ClIdentifiableObject::GetId (void ) const
{
  //## begin ClIdentifiableObject::GetId%-2008249606.body preserve=yes
   if (m_poObjectId)	
		return m_poObjectId->GetId();
   else
		return 0;
  //## end ClIdentifiableObject::GetId%-2008249606.body
}

ClObjectId* ClIdentifiableObject::GetObjId (void ) const
{
  //## begin ClIdentifiableObject::GetObjId%-1583760229.body preserve=yes
   return m_poObjectId;
  //## end ClIdentifiableObject::GetObjId%-1583760229.body
}

void ClIdentifiableObject::SetObjId (ClObjectId* ai_poObjectId)
{
  //## begin ClIdentifiableObject::SetObjId%949787626.body preserve=yes
   if (m_poObjectId)
	   delete m_poObjectId;

   m_poObjectId = ai_poObjectId;
  //## end ClIdentifiableObject::SetObjId%949787626.body
}

// Additional Declarations
  //## begin ClIdentifiableObject%3911AA25022E.declarations preserve=yes
  //## end ClIdentifiableObject%3911AA25022E.declarations

//## begin module%3911AA2900C7.epilog preserve=yes
//## end module%3911AA2900C7.epilog
