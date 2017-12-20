//## begin module%3911AA29029D.cm preserve=no
//## end module%3911AA29029D.cm

//## begin module%3911AA29029D.cp preserve=no
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
//## end module%3911AA29029D.cp

//## Module: clprsstb%3911AA29029D; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\clprsstb.cpp

//## begin module%3911AA29029D.additionalIncludes preserve=no
//## end module%3911AA29029D.additionalIncludes

//## begin module%3911AA29029D.includes preserve=yes
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
//    Define el interface para objetos persistibles, y las clases para formar
//    identificadores de persistencia para los objetos
//
//==============================================================================

#include <string.h>

#include "clprsstb.hpp"


// ---- Class ClPersistId -------------------------------------------


//## end module%3911AA29029D.includes

// clsrlzbl
#include <clsrlzbl.hpp>
// cldntfbl
#include <cldntfbl.hpp>
// m4types
#include <m4types.hpp>
//## begin module%3911AA29029D.declarations preserve=no
//## end module%3911AA29029D.declarations

//## begin module%3911AA29029D.additionalDeclarations preserve=yes


// ---- Class ClPersistiableObject ----------------------------------

/*
ClPersistiableObject::ClPersistiableObject(void):
   m_poPersistId(0)
{
}
*/

/*
ClPersistiableObject::ClPersistiableObject(ClObjectId  *ai_poObjectId,
                                           ClPersistId *ai_poPersistId):
   ClIdentifiableObject(ai_poObjectId),     // El constructor de mi padre
   m_poPersistId       (ai_poPersistId)
{
}
*/


/*
m4pchar_t ClPersistiableObject::GetPersistId(void) const
{
   if (m_poPersistId)
      return m_poPersistId->GetPersistId();
   else
      return 0;
}


ClPersistId *ClPersistiableObject::GetObjPersistId(void) const
{
   return m_poPersistId;
}
*/

//## end module%3911AA29029D.additionalDeclarations


// Class ClPersistId 


ClPersistId::ClPersistId (void )
  //## begin ClPersistId::ClPersistId%2141709434.hasinit preserve=no
  //## end ClPersistId::ClPersistId%2141709434.hasinit
  //## begin ClPersistId::ClPersistId%2141709434.initialization preserve=yes
:
   m_persistId(0)
  //## end ClPersistId::ClPersistId%2141709434.initialization
{
  //## begin ClPersistId::ClPersistId%2141709434.body preserve=yes
  //## end ClPersistId::ClPersistId%2141709434.body
}

ClPersistId::ClPersistId (const ClPersistId& ai_roPersistId)
  //## begin ClPersistId::ClPersistId%-910944005.hasinit preserve=no
  //## end ClPersistId::ClPersistId%-910944005.hasinit
  //## begin ClPersistId::ClPersistId%-910944005.initialization preserve=yes
  //## end ClPersistId::ClPersistId%-910944005.initialization
{
  //## begin ClPersistId::ClPersistId%-910944005.body preserve=yes
   m_persistId=new char [strlen(ai_roPersistId.GetPersistId())+1];
   strcpy(m_persistId, ai_roPersistId.GetPersistId());
  //## end ClPersistId::ClPersistId%-910944005.body
}

ClPersistId::ClPersistId (const m4pchar_t ai_string)
  //## begin ClPersistId::ClPersistId%1614162924.hasinit preserve=no
  //## end ClPersistId::ClPersistId%1614162924.hasinit
  //## begin ClPersistId::ClPersistId%1614162924.initialization preserve=yes
  //## end ClPersistId::ClPersistId%1614162924.initialization
{
  //## begin ClPersistId::ClPersistId%1614162924.body preserve=yes
   m_persistId=new char [strlen(ai_string)+1];
   strcpy(m_persistId, ai_string);
  //## end ClPersistId::ClPersistId%1614162924.body
}


ClPersistId::~ClPersistId (void )
{
  //## begin ClPersistId::~ClPersistId%1142821737.body preserve=yes
   if (m_persistId) delete[] m_persistId;
  //## end ClPersistId::~ClPersistId%1142821737.body
}



//## Other Operations (implementation)
m4bool_t ClPersistId::operator == (const ClPersistId& ai_roPersistId) const
{
  //## begin ClPersistId::operator ==%1194998261.body preserve=yes
   return (strcmp(m_persistId, ai_roPersistId.GetPersistId())==0)? M4_TRUE : M4_FALSE;
  //## end ClPersistId::operator ==%1194998261.body
}

m4pchar_t ClPersistId::GetPersistId (void ) const
{
  //## begin ClPersistId::GetPersistId%-1246418680.body preserve=yes
   return m_persistId;
  //## end ClPersistId::GetPersistId%-1246418680.body
}

m4bool_t ClPersistId::operator != (const ClPersistId& ai_roPersistId) const
{
  //## begin ClPersistId::operator !=%784434613.body preserve=yes
   return (strcmp(m_persistId, ai_roPersistId.GetPersistId())!=0)? M4_TRUE : M4_FALSE;
  //## end ClPersistId::operator !=%784434613.body
}

m4return_t ClPersistId::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClPersistId::Serialize%731478525.body preserve=yes
   return ai_roIOD.Write(m_persistId, (m4uint32_t)strlen(m_persistId));
  //## end ClPersistId::Serialize%731478525.body
}

m4return_t ClPersistId::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClPersistId::DeSerialize%701944922.body preserve=yes
   m4uint32_t iSize;

   if (ai_roIOD.ReadSize(iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_persistId=new char [iSize+1];
   if (!m_persistId)
      return M4_ERROR;     // No hay memoria

   if (ai_roIOD.ReadBuffer(m_persistId)!=M4_SUCCESS)
      return M4_ERROR;
   m_persistId[iSize]=0;   // El nil del final

   return M4_SUCCESS;
  //## end ClPersistId::DeSerialize%701944922.body
}

// Additional Declarations
  //## begin ClPersistId%3911AA27024A.declarations preserve=yes
  //## end ClPersistId%3911AA27024A.declarations

// Class ClPersistiableObject 

ClPersistiableObject::ClPersistiableObject (const ClPersistiableObject& ai_roPersistiableObject)
  //## begin ClPersistiableObject::ClPersistiableObject%-1172167449.hasinit preserve=no
  //## end ClPersistiableObject::ClPersistiableObject%-1172167449.hasinit
  //## begin ClPersistiableObject::ClPersistiableObject%-1172167449.initialization preserve=yes
:
   ClIdentifiableObject((ClIdentifiableObject)ai_roPersistiableObject)     // El constructor de mi padre
  //## end ClPersistiableObject::ClPersistiableObject%-1172167449.initialization
{
  //## begin ClPersistiableObject::ClPersistiableObject%-1172167449.body preserve=yes
   //m_poPersistId=new ClPersistId(*(ai_roPersistiableObject.GetObjPersistId()));
  //## end ClPersistiableObject::ClPersistiableObject%-1172167449.body
}

ClPersistiableObject::ClPersistiableObject (const ClIdentifiableObject& ai_roIdentifiableObject)
  //## begin ClPersistiableObject::ClPersistiableObject%1316295238.hasinit preserve=no
  //## end ClPersistiableObject::ClPersistiableObject%1316295238.hasinit
  //## begin ClPersistiableObject::ClPersistiableObject%1316295238.initialization preserve=yes
:
   ClIdentifiableObject(ai_roIdentifiableObject)
   //,m_poPersistId       (0)
  //## end ClPersistiableObject::ClPersistiableObject%1316295238.initialization
{
  //## begin ClPersistiableObject::ClPersistiableObject%1316295238.body preserve=yes
  //## end ClPersistiableObject::ClPersistiableObject%1316295238.body
}

ClPersistiableObject::ClPersistiableObject (ClObjectId* ai_poObjectId)
  //## begin ClPersistiableObject::ClPersistiableObject%1869755369.hasinit preserve=no
  //## end ClPersistiableObject::ClPersistiableObject%1869755369.hasinit
  //## begin ClPersistiableObject::ClPersistiableObject%1869755369.initialization preserve=yes
:
ClIdentifiableObject(ai_poObjectId)
  //## end ClPersistiableObject::ClPersistiableObject%1869755369.initialization
{
  //## begin ClPersistiableObject::ClPersistiableObject%1869755369.body preserve=yes
  //## end ClPersistiableObject::ClPersistiableObject%1869755369.body
}


ClPersistiableObject::~ClPersistiableObject (void )
{
  //## begin ClPersistiableObject::~ClPersistiableObject%-704253074.body preserve=yes
//   delete m_poPersistId;
  //## end ClPersistiableObject::~ClPersistiableObject%-704253074.body
}



//## Other Operations (implementation)
m4return_t ClPersistiableObject::Serialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClPersistiableObject::Serialize%-1317523330.body preserve=yes
   // Serializo el miembro de mi padre (ObjectId)
   // Lo tengo que hacer yo porque mi padre no es serializable y yo si
   if (ai_roIOD.Write((void*)m_poObjectId)!=M4_SUCCESS)
      return M4_ERROR;
   if (m_poObjectId->Serialize(ai_roIOD)!=M4_SUCCESS)
      return M4_ERROR;

   // Serializo mi miembro dato (el PersistId)
   /*
   if (ai_roIOD.Write((void*)m_poPersistId)!=M4_SUCCESS)
      return M4_ERROR;
   if (m_poPersistId->Serialize(ai_roIOD)!=M4_SUCCESS)
      return M4_ERROR;
	*/

   return M4_SUCCESS;
  //## end ClPersistiableObject::Serialize%-1317523330.body
}

m4return_t ClPersistiableObject::DeSerialize (ClGenericIODriver& ai_roIOD)
{
  //## begin ClPersistiableObject::DeSerialize%-1437971505.body preserve=yes
   m4bool_t pExist;

   // DeSerializo el miembro de mi padre (ObjectId)
   // Lo tengo que hacer yo porque mi padre no es serializable y yo si
   if (ai_roIOD.Read((void**)&m_poObjectId, &pExist)!=M4_SUCCESS)
      return M4_ERROR;

   if (m_poObjectId==0)       // Es la primera vez que lo deserializo
      if (pExist==M4_FALSE)   // El objeto viene a continuación
      {
         m_poObjectId=new ClObjectId;
         if (m_poObjectId)
         {
            if (m_poObjectId->DeSerialize(ai_roIOD)!=M4_SUCCESS)
               return M4_ERROR;
         }
         else
            return M4_ERROR;  // No hay memoria
      }

   // Deserializo mi PersistId
	/*
   if (ai_roIOD.Read((void**)&m_poPersistId, &pExist)!=M4_SUCCESS)
      return M4_ERROR;

   if (m_poPersistId==0)      // Es la primera vez que lo deserializo
      if (pExist==M4_FALSE)   // El objeto viene a continuación
      {
         m_poPersistId=new ClPersistId;
         if (m_poPersistId)
         {
            if (m_poPersistId->DeSerialize(ai_roIOD)!=M4_SUCCESS)
               return M4_ERROR;
         }
         else
            return M4_ERROR;  // No hay memoria
      }
	*/

   return M4_SUCCESS;
  //## end ClPersistiableObject::DeSerialize%-1437971505.body
}

// Additional Declarations
  //## begin ClPersistiableObject%3911AA27030A.declarations preserve=yes
  //## end ClPersistiableObject%3911AA27030A.declarations

// Class ClSelfPersistiableObject 

ClSelfPersistiableObject::ClSelfPersistiableObject (void )
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.hasinit preserve=no
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.hasinit
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.initialization preserve=yes
:
   ClPersistiableObject((ClObjectId*)0)     // El constructor de mi padre
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.initialization
{
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.body preserve=yes
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2010852052.body
}

ClSelfPersistiableObject::ClSelfPersistiableObject (const ClPersistiableObject& ai_roPersistiableObject)
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.hasinit preserve=no
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.hasinit
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.initialization preserve=yes
:
   ClPersistiableObject(ai_roPersistiableObject)     // El constructor de mi padre
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.initialization
{
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.body preserve=yes
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1838470577.body
}

ClSelfPersistiableObject::ClSelfPersistiableObject (const ClIdentifiableObject& ai_roIdentifiableObject)
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.hasinit preserve=no
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.hasinit
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.initialization preserve=yes
:
   ClPersistiableObject(ai_roIdentifiableObject)
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.initialization
{
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.body preserve=yes
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%1299958730.body
}

ClSelfPersistiableObject::ClSelfPersistiableObject (ClObjectId* ai_poObjectId)
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.hasinit preserve=no
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.hasinit
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.initialization preserve=yes
:
ClPersistiableObject(ai_poObjectId)
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.initialization
{
  //## begin ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.body preserve=yes
  //## end ClSelfPersistiableObject::ClSelfPersistiableObject%2110413596.body
}


ClSelfPersistiableObject::~ClSelfPersistiableObject (void )
{
  //## begin ClSelfPersistiableObject::~ClSelfPersistiableObject%1975116357.body preserve=yes
  //## end ClSelfPersistiableObject::~ClSelfPersistiableObject%1975116357.body
}


// Additional Declarations
  //## begin ClSelfPersistiableObject%3911AA2703A1.declarations preserve=yes
  //## end ClSelfPersistiableObject%3911AA2703A1.declarations

//## begin module%3911AA29029D.epilog preserve=yes
//## end module%3911AA29029D.epilog
