//## begin module%408D091702CC.cm preserve=no
//## end module%408D091702CC.cm

//## begin module%408D091702CC.cp preserve=no
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
//## end module%408D091702CC.cp

//## Module: snmptables%408D091702CC; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmptables.cpp

//## begin module%408D091702CC.additionalIncludes preserve=no
//## end module%408D091702CC.additionalIncludes

#include "m4types.hpp"
#include "snmpxmlutil.hpp"
#include <snmptables.hpp>
#include "m4unicode.hpp"

#include <xercesc/dom/DOMNode.hpp> 
#include <xercesc/dom/DOMNodeList.hpp> 
#include <xercesc/dom/DOMNamedNodeMap.hpp> 

//## begin module%408D091702CC.declarations preserve=no
//## end module%408D091702CC.declarations

//## begin module%408D091702CC.additionalDeclarations preserve=yes
//## end module%408D091702CC.additionalDeclarations


// Class M4MibStaticTable 

M4MibStaticTable::M4MibStaticTable (const m4string_t& ai_classId, m4int32_t ai_indexSize)
  //## begin M4MibStaticTable::M4MibStaticTable%1083571298.hasinit preserve=no
  //## end M4MibStaticTable::M4MibStaticTable%1083571298.hasinit
  //## begin M4MibStaticTable::M4MibStaticTable%1083571298.initialization preserve=yes
  : M4MibTable(ai_classId, ai_indexSize)
  //## end M4MibStaticTable::M4MibStaticTable%1083571298.initialization
{
  //## begin M4MibStaticTable::M4MibStaticTable%1083571298.body preserve=yes
  //## end M4MibStaticTable::M4MibStaticTable%1083571298.body
}


// Additional Declarations
  //## begin M4MibStaticTable%408D0854025D.declarations preserve=yes
  //## end M4MibStaticTable%408D0854025D.declarations

// Class M4MibDynamicTable 

M4MibDynamicTable::M4MibDynamicTable (const m4string_t& ai_classId, m4int32_t ai_indexSize)
  //## begin M4MibDynamicTable::M4MibDynamicTable%1082986497.hasinit preserve=no
  //## end M4MibDynamicTable::M4MibDynamicTable%1082986497.hasinit
  //## begin M4MibDynamicTable::M4MibDynamicTable%1082986497.initialization preserve=yes
  : M4MibTable(ai_classId, ai_indexSize)
  //## end M4MibDynamicTable::M4MibDynamicTable%1082986497.initialization
{
  //## begin M4MibDynamicTable::M4MibDynamicTable%1082986497.body preserve=yes
  //## end M4MibDynamicTable::M4MibDynamicTable%1082986497.body
}


// Additional Declarations
  //## begin M4MibDynamicTable%408D085C03DB.declarations preserve=yes
  //## end M4MibDynamicTable%408D085C03DB.declarations

// Class M4MibTable 


M4MibTable::M4MibTable (const m4string_t& ai_classId, m4int32_t ai_indexSize)
  //## begin M4MibTable::M4MibTable%1083571295.hasinit preserve=no
      : m_rowIndex(1)
  //## end M4MibTable::M4MibTable%1083571295.hasinit
  //## begin M4MibTable::M4MibTable%1083571295.initialization preserve=yes
  ,MibTable(ai_classId.c_str(), ai_indexSize)
  //## end M4MibTable::M4MibTable%1083571295.initialization
{
  //## begin M4MibTable::M4MibTable%1083571295.body preserve=yes
    // insert a column for the index.
    add_col(new Agentpp::MibLeaf("1", Agentpp::READONLY, new SnmpInt32(0), VMODE_NONE));
  //## end M4MibTable::M4MibTable%1083571295.body
}



//## Other Operations (implementation)
void M4MibTable::AddCol (Agentpp::MibLeaf* ai_poSnmpObject)
{
  //## begin M4MibTable::AddCol%1083571296.body preserve=yes
    add_col(ai_poSnmpObject);
  //## end M4MibTable::AddCol%1083571296.body
}

Agentpp::MibTableRow* M4MibTable::AddRow (const Oid& ai_index)
{
  //## begin M4MibTable::AddRow%1083571297.body preserve=yes
    // creates the row.
	Agentpp::MibTableRow* poRow = add_row(ai_index);
    
    return poRow;
  //## end M4MibTable::AddRow%1083571297.body
}

void M4MibTable::AddCols (const DOMNode *ai_domNode)
{
  //## begin M4MibTable::AddCols%1083666430.body preserve=yes
    // insert the columns in the table.
    const DOMNode *pNode = NULL, *cNode = NULL;
    DOMNodeList *children = NULL;
    DOMNamedNodeMap *nnm = NULL;
    DOMNode *attNameNode = NULL, *attOidNode = NULL;
    m4pchar_t attName, attOid;
    M4MibTable *poTable = NULL;

    // for each property in this snmpobject, add a column.
    pNode = SnmpDOMUtils::findChildNodeByName(ai_domNode, "properties");
    if (pNode != NULL)
    {
        children = pNode->getChildNodes();
        for (m4uint32_t i=0; i<children->getLength(); i++)
        {
            cNode = children->item(i);
            if (DOMNode::ELEMENT_NODE == cNode->getNodeType())
            {
                nnm = cNode->getAttributes();

                // get property name.
                attNameNode = nnm->getNamedItem(M4_XERCES_LITERAL("name"));
				// UNICODE XML
                attName = M4XMLToCpp( (M4XMLCh*)attNameNode->getNodeValue() );

                // get oid.
                attOidNode = nnm->getNamedItem(M4_XERCES_LITERAL("oid"));
				// UNICODE XML
                attOid = M4XMLToCpp( (M4XMLCh*)attOidNode->getNodeValue() );

                // insert column in table for this property.
                AddCol(new M4SnmpProperty(attName, -1, attOid, cNode, NULL));
                delete attName;
                delete attOid;
            }
        }
    }

    // for each statistic in this snmpobject, add a column.
    pNode = SnmpDOMUtils::findChildNodeByName(ai_domNode, "statistics");
    if (pNode != NULL)
    {
        children = pNode->getChildNodes();
        for (m4uint32_t i=0; i<children->getLength(); i++)
        {
            cNode = children->item(i);
            if (DOMNode::ELEMENT_NODE == cNode->getNodeType())
            {
                nnm = cNode->getAttributes();

                // get statistic name.
                attNameNode = nnm->getNamedItem(M4_XERCES_LITERAL("name"));
				// UNICODE XML
                attName = M4XMLToCpp( (M4XMLCh*)attNameNode->getNodeValue() );

                // get oid.
                attOidNode = nnm->getNamedItem(M4_XERCES_LITERAL("oid"));
				// UNICODE XML
                attOid = M4XMLToCpp( (M4XMLCh*)attOidNode->getNodeValue() );

                // insert column in table for this property.
                AddCol(new M4SnmpStatistic(attName, -1, attOid, cNode, NULL));
                delete attName;
                delete attOid;
            }
        }
    }
  //## end M4MibTable::AddCols%1083666430.body
}

Oid M4MibTable::GetNextRowIndex ()
{
  //## begin M4MibTable::GetNextRowIndex%1083741955.body preserve=yes
    // get next available index in this table.
    m4uint32_t iIndex = m_rowIndex++;
    m4char_t cIndex[32];
    sprintf (cIndex, "%u", iIndex);

    return Oid(cIndex);
  //## end M4MibTable::GetNextRowIndex%1083741955.body
}

// Additional Declarations
  //## end M4MibTable%4096132C0277.declarations

// Class M4MibTableContainer 

//## begin M4MibTableContainer::<m_container>%4098C2BC03DF.role preserve=no  private: static M4MibTableMap_t { -> VHAN}
M4MibTableMap_t M4MibTableContainer::m_container;
//## end M4MibTableContainer::<m_container>%4098C2BC03DF.role


//## Other Operations (implementation)
M4MibTable* M4MibTableContainer::GetTable (const m4string_t& ai_sstype)
{
  //## begin M4MibTableContainer::GetTable%1083741951.body preserve=yes
    M4MibTableMap_t::iterator it;
    M4MibTable* poTable = NULL;

    it = m_container.find(ai_sstype);
    if (m_container.end() != it)
    {
        poTable = (*it).second;
    }

    return poTable;
  //## end M4MibTableContainer::GetTable%1083741951.body
}

void M4MibTableContainer::AddTable (const m4string_t& ai_sstype, M4MibTable* ai_poTable)
{
  //## begin M4MibTableContainer::AddTable%1083741952.body preserve=yes
    m_container.insert(M4MibTableMap_t::value_type(ai_sstype, ai_poTable));
  //## end M4MibTableContainer::AddTable%1083741952.body
}

M4MibTable* M4MibTableContainer::CreateTable (M4SnmpSubsystem* ai_poSnmpSubsystem)
{
  //## begin M4MibTableContainer::CreateTable%1083741954.body preserve=yes
    // create the index info struct. For each level in the subsystem tree we have one index.
    int indexSize = 0;
    /*
    index_info tableIndex = new index_info[indexSize];
    for (int i=0; i<indexSize; i++)
    {
        tableIndex[i] = index_info(sNMP_SYNTAX_INT, FALSE, 1, MAX_INTEGER);
    }
    */

    M4MibDynamicTable* poTable = new M4MibDynamicTable(ai_poSnmpSubsystem->GetClassId().c_str(), indexSize);
    if (NULL != poTable)
    {
        poTable->AddCols(ai_poSnmpSubsystem->GetDOMNode());
        M4MibTableContainer::AddTable(ai_poSnmpSubsystem->GetType(), poTable);
    }

    return poTable;
  //## end M4MibTableContainer::CreateTable%1083741954.body
}

// Additional Declarations
  //## begin M4MibTableContainer%4098C29F01DE.declarations preserve=yes
  //## end M4MibTableContainer%4098C29F01DE.declarations

// Class M4MibTableKeyLess 


//## Other Operations (implementation)
m4bool_t M4MibTableKeyLess::operator () (const m4string_t& ai_right, const m4string_t& ai_left) const
{
  //## begin M4MibTableKeyLess::operator()%1083741953.body preserve=yes
    int iRetCode = -1;
    m4bool_t bRetCode = M4_FALSE;
    iRetCode = stricmp(ai_right.c_str(), ai_left.c_str());
    if (iRetCode < 0)
    {
        bRetCode = M4_TRUE;
    }

    return bRetCode;
  //## end M4MibTableKeyLess::operator()%1083741953.body
}

// Additional Declarations
  //## begin M4MibTableKeyLess%4098E27400B9.declarations preserve=yes
  //## end M4MibTableKeyLess%4098E27400B9.declarations

//## begin module%408D091702CC.epilog preserve=yes
//## end module%408D091702CC.epilog
