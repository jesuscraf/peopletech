//## begin module%406BE419036C.cm preserve=no
//## end module%406BE419036C.cm

//## begin module%406BE419036C.cp preserve=no
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
//## end module%406BE419036C.cp

//## Module: snmpadmin%406BE419036C; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmpadmin.cpp

//## begin module%406BE419036C.additionalIncludes preserve=no
//## end module%406BE419036C.additionalIncludes

#include "agent_pp/mib.h"
#include "agent_pp/v3_mib.h"
#include "agent_pp/vacm.h"

#include "snmpxmlutil.hpp"
#include "snmpobject.hpp"
#include "snmptables.hpp"
#include "xmlstreamgenerator.hpp"
#include "snmpres.hpp"

#include <m4thread.hpp>
#include <cloblconfiguration.hpp>
#include <snmpadmin.hpp>
#include <snmpinterfaces.hpp>
#include "m4unicode.hpp"
#include "blocksynchronization.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMDocument.hpp>



//## begin module%406BE419036C.declarations preserve=no
//## end module%406BE419036C.declarations

//## begin module%406BE419036C.additionalDeclarations preserve=yes
//## end module%406BE419036C.additionalDeclarations


// Class M4SnmpAdministrator 

// bg 305785 (extends bg 180285)
ClMutex	g_oMutex( M4_TRUE );



M4SnmpAdministrator::M4SnmpAdministrator()
  //## begin M4SnmpAdministrator::M4SnmpAdministrator%.hasinit preserve=no
      : m_snmpRootOid(""), m_snmpPort(-1)
  //## end M4SnmpAdministrator::M4SnmpAdministrator%.hasinit
  //## begin M4SnmpAdministrator::M4SnmpAdministrator%.initialization preserve=yes
  //## end M4SnmpAdministrator::M4SnmpAdministrator%.initialization
{
  //## begin M4SnmpAdministrator::M4SnmpAdministrator%.body preserve=yes
    m_pRequestList = NULL;
    m_pMib = NULL;
  //## end M4SnmpAdministrator::M4SnmpAdministrator%.body
}


M4SnmpAdministrator::~M4SnmpAdministrator()
{
  //## begin M4SnmpAdministrator::~M4SnmpAdministrator%.body preserve=yes
    Destroy();
  //## end M4SnmpAdministrator::~M4SnmpAdministrator%.body
}



//## Other Operations (implementation)
m4void_t M4SnmpAdministrator::Run ()
{
  //## begin M4SnmpAdministrator::Run%1080808685.body preserve=yes
    while (! IsTerminate() )
    {
        this->_Run();			
    }
  //## end M4SnmpAdministrator::Run%1080808685.body
}

M4ISnmpObject* M4SnmpAdministrator::RegisterSubsystem (M4ISnmpObject* ai_pParentSnmpObject, M4ISSApi4Snmp* ai_pSSApi, m4pcchar_t ai_type, m4pcchar_t ai_name)
{
  //## begin M4SnmpAdministrator::RegisterSubsystem%1081341182.body preserve=yes
    if (NULL == ai_type)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_type" << "null");
        return NULL;
    }
    if (NULL == ai_name)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_name" << "null");
        return NULL;
    }

    // gets the parent class id and the snmp parent subsystem.
    m4string_t snmpParentClassId;
    M4SnmpSubsystem* poSnmpParentSubsystem = NULL;
    if (NULL == ai_pParentSnmpObject)
    {
        snmpParentClassId = this->m_snmpRootOid;
    }
    else
    {
        // the snmp parent of a subsystem is always a snmp subsystem.
        poSnmpParentSubsystem = (M4SnmpSubsystem*)ai_pParentSnmpObject;
        snmpParentClassId = ai_pParentSnmpObject->GetClassId();
    }

	const DOMNode *n = NULL;
	M4SnmpSubsystem* poSnmpObject;
	{
		ClMutBlock	oBlock( &g_oMutex ) ;

		// get subsystem node.
		n = SnmpDOMUtils::findNodeByAtt(m_snmpDOMDoc->getElementsByTagName(M4_XERCES_LITERAL("subsystem")), M4_XERCES_LITERAL("type"), ai_type);
		if (n == NULL)
		{
			/* Bug 0161772
			No hay que volcar error. Puede ser que uno de los subsistemas no tenga snmp
			DUMP_CHLOG_ERRORF(M4_SNMP_SUBSYSTEM_NOT_EXPORTED, ai_type);
			*/
			return NULL;
		}

		poSnmpObject = new M4SnmpSubsystem(ai_name, n, snmpParentClassId, (M4SnmpSubsystem*)ai_pParentSnmpObject, ai_pSSApi);
		if (NULL == poSnmpObject)
		{
			DUMP_CHLOG_ERRORF(M4_SNMP_OBJECT_NOT_CREATED, ai_name);
			return NULL;
		}
	}

    M4MibTable* poTable = NULL;
	Agentpp::MibTableRow* poTableRow = NULL;
    Oid rowIndex;

    switch(poSnmpObject->GetMibType())
    {
    case M4_MIB_LEAF: 
        // Currently, we don't registered the subsystems.
        //m_pMib->add(poSnmpObject);
        break;
    case M4_MIB_STATIC_TABLE_ENTRY: 
    case M4_MIB_DYNAMIC_TABLE_ENTRY: 
		
		// bg 306048.
		m_pMib->lock_mib();

        poTable = M4MibTableContainer::GetTable(poSnmpObject->GetType());
        if (NULL == poTable)
        {
            poTable = M4MibTableContainer::CreateTable(poSnmpObject);
            if (NULL == poTable)
            {
				m_pMib->unlock_mib();

   			    DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_CREATE_TABLE, ai_type);
                return NULL;
            }
            m_pMib->add(poTable);
        }
        poSnmpObject->SetMibTable(poTable);

        // get the index for the next row.
        if (NULL != poSnmpParentSubsystem)
        {
            rowIndex = poSnmpParentSubsystem->GetMibTableRowIndex();
        }

		poTable->start_synch();

        rowIndex += poTable->GetNextRowIndex();

        // add a row for this subsystem in the table.
        poTableRow = poTable->AddRow(rowIndex);

		poTable->end_synch();
		m_pMib->unlock_mib();

        if (NULL != poTableRow)
        {
            poSnmpObject->SetMibTableRow(poTableRow);

            // update the harcoded column index with the last subidentifier of the row index.
            unsigned long rowLastIndex = poTableRow->get_index().last();
            poTableRow->get_nth(0)->replace_value(new SnmpInt32(rowLastIndex));
        }
        break;
    default:
        // Unknown type.
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_MIB_TYPE, poSnmpObject->GetMibType() << poSnmpObject->GetType());
        break;
    }

    return poSnmpObject;
  //## end M4SnmpAdministrator::RegisterSubsystem%1081341182.body
}

m4return_t M4SnmpAdministrator::RegisterProperty (m4pcchar_t ai_name, m4int32_t ai_propertyIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig)
{
  //## begin M4SnmpAdministrator::RegisterProperty%1081341183.body preserve=yes
	ClMutBlock	oBlock( &g_oMutex ) ;

    m4return_t retCode = M4_ERROR;

    if (NULL == ai_name)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_name" << "null");
        return NULL;
    }
    if (NULL == ai_poParentSnmpObject)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_poParentSnmpObject" << "null");
        return NULL;
    }

    // the parentSnmpObject is always an M4SnmpSubsystem.
    M4SnmpSubsystem* poSnmpParentSubsystem = (M4SnmpSubsystem*)ai_poParentSnmpObject;

    // get parent DOM node.
    const DOMNode *n = poSnmpParentSubsystem->GetDOMPropertiesNode();
    if (n == NULL)
    {
        // this subsystem has not exported snmp properties.
        return M4_SUCCESS;
    }

    // find property node.
    const DOMNode *propertyNode = SnmpDOMUtils::findNodeByAtt(n->getChildNodes(), M4_XERCES_LITERAL("name"), ai_name);
    if (propertyNode == NULL)
    {
        // this subsystem has not exported this property as snmp property.
        return M4_SUCCESS;
    }

    // get subclass id for this property
    m4pcchar_t pcSnmpClassId = SnmpDOMUtils::getAttVal(propertyNode, M4_XERCES_LITERAL("oid"));
    if (NULL == pcSnmpClassId)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)propertyNode->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "oid" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
    

    M4SnmpProperty* poSnmpObject = NULL;
	Agentpp::MibTableRow* poTableRow = NULL;
	Agentpp::MibLeaf* poSnmpObjectInTable = NULL;
    int rowIndex = 0;
	Agentpp::Oidx snmpObjectOID;
    switch(poSnmpParentSubsystem->GetMibType())
    {
    case M4_MIB_LEAF:
        // adds the object to mib database.
        poSnmpObject = new M4SnmpProperty(ai_name, ai_propertyIndex, pcSnmpClassId, propertyNode, poSnmpParentSubsystem);
        if (NULL == poSnmpObject)
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_OBJECT_NOT_CREATED, ai_name);
            return M4_ERROR;
        }
        m_pMib->add(poSnmpObject);
        
        // adds the object to parent object repository.
        // When we destroy a subsystem we need to delete all his properties in the mib.
        poSnmpParentSubsystem->AddChildObject(poSnmpObject);

        retCode = M4_SUCCESS;
        break;
    case M4_MIB_STATIC_TABLE_ENTRY:
    case M4_MIB_DYNAMIC_TABLE_ENTRY:
        poTableRow = poSnmpParentSubsystem->GetMibTableRow();        
        snmpObjectOID = Agentpp::Oidx(poSnmpParentSubsystem->GetClassId().c_str()) + Agentpp::Oidx(pcSnmpClassId) + poTableRow->get_index();

        rowIndex = poTableRow->index_of(snmpObjectOID);
        if (rowIndex != -1)
        {
            poSnmpObject = (M4SnmpProperty*)poTableRow->get_element(snmpObjectOID);
            poSnmpObject->SetSnmpSubsystem(poSnmpParentSubsystem);
            poSnmpObject->SetPropertyIndex(ai_propertyIndex);
            poSnmpObject->SetName(ai_name);
        }
        else
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_FIND_ROW, poSnmpParentSubsystem->GetType());
            retCode = M4_ERROR;
        }
        break;
    default:
        // Unknown type.
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_MIB_TYPE, poSnmpObject->GetMibType() << poSnmpObject->GetName());
        retCode = M4_ERROR;
        break;
    }

    return retCode;
  //## end M4SnmpAdministrator::RegisterProperty%1081341183.body
}

m4return_t M4SnmpAdministrator::Init (ClConfiguration* ai_config)
{
  //## begin M4SnmpAdministrator::Init%1081341184.body preserve=yes
    m4return_t retCode = M4_ERROR;
    const DOMNode *n = Obl2Xml(ai_config);
    if (n == NULL)
    {
        DUMP_CHLOG_ERROR(M4_SNMP_OBL2XML_ERROR);
        return M4_ERROR;
    }

    retCode = Init(n);

    return retCode;
  //## end M4SnmpAdministrator::Init%1081341184.body
}

m4return_t M4SnmpAdministrator::LoadXmlConfig (m4pcchar_t ai_pFile, m4pcchar_t ai_product)
{
  //## begin M4SnmpAdministrator::LoadXmlConfig%1081862805.body preserve=yes
    // Initialize the XML4C system
    XMLPlatformUtils::Initialize();
 
    // Instantiate the DOM parser.
    m_oParser.setValidationScheme(XercesDOMParser::Val_Never);

    // And create our error handler and install it
    SnmpErrorHandler errorHandler;
    m_oParser.setErrorHandler(&errorHandler);

    // parse the document.
    m_oParser.parse(ai_pFile);
    if (M4_TRUE == errorHandler.hasErrors())
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSING_ERROR, ai_pFile);
        return M4_ERROR;
    }

    // get document.
    m_snmpDOMDoc = m_oParser.getDocument();

    // read product oid (classid)
    const DOMNode *n = SnmpDOMUtils::findNodeByAtt(m_snmpDOMDoc->getElementsByTagName(M4_XERCES_LITERAL("product")), M4_XERCES_LITERAL("name"), ai_product);
    if (n == 0)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_NODE_BY_ATT, "name" << ai_product);
        return M4_ERROR;
    }
    m4pcchar_t pcOid = SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("oid"));
    if (NULL == pcOid)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)n->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "oid" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
    m_snmpRootOid = pcOid;

    return M4_SUCCESS;
  //## end M4SnmpAdministrator::LoadXmlConfig%1081862805.body
}

m4return_t M4SnmpAdministrator::Start ()
{
  //## begin M4SnmpAdministrator::Start%1081862821.body preserve=yes
    return M4Thread::Start("SNMP Administrator");  
  //## end M4SnmpAdministrator::Start%1081862821.body
}

m4return_t M4SnmpAdministrator::Stop ()
{
  //## begin M4SnmpAdministrator::Stop%1081862822.body preserve=yes
    m4return_t retCode = M4_SUCCESS;
    retCode = this->Terminate();

	return retCode;
  //## end M4SnmpAdministrator::Stop%1081862822.body
}

m4return_t M4SnmpAdministrator::Destroy ()
{
  //## begin M4SnmpAdministrator::Destroy%1081862823.body preserve=yes

    // stops the administrator.
    Stop();

    // destroy derived class.
    _Destroy();

    // delete mib database.
    if (NULL != m_pMib) delete m_pMib;
    m_pMib = NULL;
    if (NULL != m_pRequestList) delete m_pRequestList;
    m_pRequestList = NULL;

    return M4_SUCCESS;
  //## end M4SnmpAdministrator::Destroy%1081862823.body
}

Agentpp::Vacm* M4SnmpAdministrator::CreateVacm (const DOMNode* ai_config)
{
  //## begin M4SnmpAdministrator::CreateVacm%1082032854.body preserve=yes
    // create the Vacm.
	Agentpp::Vacm* pVacm = new Agentpp::Vacm(*m_pMib);
    if (NULL == pVacm)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new Vacm(*m_pMib)");
        return NULL;
    }

    // initialize security information
    pVacm->addNewContext("");    
	
	// get snmp.v1v2c state.
    const char* szV1V2cState = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v1v2c_state"));
    if (NULL == szV1V2cState)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v1v2c_state" << pcNodeName);
		delete [] pcNodeName ;
        return NULL;
    }
    else if (!stricmp(szV1V2cState, "Enabled"))
    { //Only if SNMPv1/v2c allowed.
		// get snmp.read/write communities.
		const char* szReadCommunity  = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_read_community"));
		const char* szWriteCommunity = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_write_community"));
		if (NULL == szReadCommunity)
		{	
			// UNICODE XML
			m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_read_community" << pcNodeName);
			delete [] pcNodeName ;
			return NULL;
		}
		if (NULL == szWriteCommunity)
		{	
			// UNICODE XML
			m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_write_community" << pcNodeName);
			delete [] pcNodeName ;
			return NULL;
		}
		
		if (*szReadCommunity!=0)
		{	
            // Add new entries to the SecurityToGroupTable.	
			pVacm->addNewGroup(SecurityModel_v1, szReadCommunity,  "v1v2ReadGroup",  storageType_volatile);
			pVacm->addNewGroup(SecurityModel_v2, szReadCommunity,  "v1v2ReadGroup",  storageType_volatile);
			// Set access rights of groups.
			pVacm->addNewAccessEntry("v1v2ReadGroup",  "", SecurityModel_v1, SecurityLevel_noAuthNoPriv, match_exact, "v1v2ReadView", NULL, NULL, storageType_nonVolatile);
			pVacm->addNewAccessEntry("v1v2ReadGroup",  "", SecurityModel_v2, SecurityLevel_noAuthNoPriv, match_exact, "v1v2ReadView", NULL, NULL, storageType_nonVolatile);
		}
		if (*szWriteCommunity!=0)
		{	
            // Add new entries to the SecurityToGroupTable.	
			pVacm->addNewGroup(SecurityModel_v1, szWriteCommunity, "v1v2WriteGroup", storageType_volatile);
			pVacm->addNewGroup(SecurityModel_v2, szWriteCommunity, "v1v2WriteGroup", storageType_volatile);
			// Set access rights of groups.
			pVacm->addNewAccessEntry("v1v2WriteGroup", "", SecurityModel_v1, SecurityLevel_noAuthNoPriv, match_exact, "v1v2ReadView", "v1v2WriteView", "v1v2WriteView", storageType_nonVolatile);
			pVacm->addNewAccessEntry("v1v2WriteGroup", "", SecurityModel_v2, SecurityLevel_noAuthNoPriv, match_exact, "v1v2ReadView", "v1v2WriteView", "v1v2WriteView", storageType_nonVolatile);
		}
		
		// Defining Views   
		pVacm->addNewView("v1v2ReadView",  SNMP_TREE_ROOT, "", view_included, storageType_nonVolatile);
		pVacm->addNewView("v1v2WriteView", SNMP_TREE_ROOT, "", view_included, storageType_nonVolatile);
	}

	// SNMPv3 public user
	// get snmp.public user state.
    const char* szV3PublicUsr = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v3_public_usr"));
    if (NULL == szV3PublicUsr)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v3_public_usr" << pcNodeName);
		delete [] pcNodeName ;
        return NULL;
    }
    else if (!stricmp(szV3PublicUsr,"Enabled"))
    {
		// get snmp.public user name.
		const char* szV3PublicUsrName = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v3_public_usr_name"));
		if (NULL == szV3PublicUsrName)
		{	
			// UNICODE XML
			m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v3_public_usr_name" << pcNodeName);
			delete [] pcNodeName ;
			return NULL;
		}

		if (*szV3PublicUsrName!=0)
		{
			pVacm->addNewGroup(SecurityModel_USM, szV3PublicUsrName, "v3ReadGroup", storageType_volatile);
			pVacm->addNewAccessEntry("v3ReadGroup", "", SecurityModel_USM, SecurityLevel_authPriv, match_prefix, "v3ReadView", "", "", storageType_nonVolatile);
		}
	}

	// SNMPv3 user views
	pVacm->addNewView("v3ReadView",  SNMP_TREE_ROOT, "", view_included, storageType_nonVolatile);	
	
    return pVacm;	
  //## end M4SnmpAdministrator::CreateVacm%1082032854.body
}

v3MP* M4SnmpAdministrator::CreateV3mp (const DOMNode* ai_config)
{
  //## begin M4SnmpAdministrator::CreateV3mp%1082032855.body preserve=yes
	// get snmp.boots file.
	
	const char* szV3BootsFile = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v3_boots_file"));
	if (NULL == szV3BootsFile)
	{
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v3_boots_file" << pcNodeName);
		delete [] pcNodeName ;
		return NULL;
	}

    const char *filename = szV3BootsFile;
    unsigned int snmpEngineBoots = 0;
    int status = 0;
	Snmp_pp::OctetStr engineId(Agentpp::SnmpEngineID::create_engine_id(m_snmpPort));

    // you may use your own methods to load/store this counter
    status = getBootCounter(filename, engineId, snmpEngineBoots);
    if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR)) 
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_SNMP_PP_LIBRARY_ERROR, "Error loading snmpEngineBoots counter (status)" << status);
		return NULL;
	}

    snmpEngineBoots++;
    status = saveBootCounter(filename, engineId, snmpEngineBoots);
    if (status != SNMPv3_OK) 
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_SNMP_PP_LIBRARY_ERROR, "Error saving snmpEngineBoots counter (status)" << status);
		return NULL;
	}

	int stat;
    v3MP* pV3mp = new v3MP(engineId, snmpEngineBoots, stat);
    if (NULL == pV3mp)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new v3MP(engineId, snmpEngineBoots, stat)");
        return NULL;
    }
	
    return pV3mp;	
  //## end M4SnmpAdministrator::CreateV3mp%1082032855.body
}

m4return_t M4SnmpAdministrator::RegisterStatistic (m4pcchar_t ai_name, m4int32_t ai_statisticIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig)
{
  //## begin M4SnmpAdministrator::RegisterStatistic%1083310097.body preserve=yes
	ClMutBlock	oBlock( &g_oMutex ) ;

    m4return_t retCode = M4_ERROR;

    if (NULL == ai_name)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_name" << "null");
        return NULL;
    }
    if (NULL == ai_poParentSnmpObject)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_poParentSnmpObject" << "null");
        return NULL;
    }

    // the parentSnmpObject is always a M4SnmpSubsystem.
    M4SnmpSubsystem* poSnmpParentSubsystem = (M4SnmpSubsystem*)ai_poParentSnmpObject;

    // get parent DOM node.
    const DOMNode *n = poSnmpParentSubsystem->GetDOMStatisticsNode();
    if (n == NULL)
    {
        // this subsystem has not exported snmp statistics.
        return M4_SUCCESS;
    }

    // find statistic node.
    const DOMNode *statisticNode = SnmpDOMUtils::findNodeByAtt(n->getChildNodes(), M4_XERCES_LITERAL("name"), ai_name);
    if (statisticNode == NULL)
    {
        // this subsystem has not exported this statistic as snmp property.
        return M4_SUCCESS;
    }

    // get subclass id for this statistic
    m4pcchar_t pcSnmpClassId = SnmpDOMUtils::getAttVal(statisticNode, M4_XERCES_LITERAL("oid"));
    if (NULL == pcSnmpClassId)
    {
        // cannot find oid for this statistic.
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)statisticNode->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "oid" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
    
    // create and register snmp object.
    M4SnmpStatistic* poSnmpObject = NULL;
	Agentpp::MibTableRow* poTableRow = NULL;
	Agentpp::MibLeaf* poSnmpObjectInTable = NULL;
    int rowIndex = 0;
	Agentpp::Oidx snmpObjectOID;
    switch(poSnmpParentSubsystem->GetMibType())
    {
    case M4_MIB_LEAF:
        // adds the object to mib database.
        poSnmpObject = new M4SnmpStatistic(ai_name, ai_statisticIndex, pcSnmpClassId, statisticNode, poSnmpParentSubsystem);
        if (NULL == poSnmpObject)
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_OBJECT_NOT_CREATED, ai_name);
            return M4_ERROR;
        }
        m_pMib->add(poSnmpObject);

        // adds the object to parent object repository.
        // When we destroy a subsystem we need to delete all his properties in the mib.
        poSnmpParentSubsystem->AddChildObject(poSnmpObject);

        retCode = M4_SUCCESS;
        break;
    case M4_MIB_STATIC_TABLE_ENTRY:
    case M4_MIB_DYNAMIC_TABLE_ENTRY:
        poTableRow = poSnmpParentSubsystem->GetMibTableRow();        
        snmpObjectOID = Agentpp::Oidx(poSnmpParentSubsystem->GetClassId().c_str()) + Agentpp::Oidx(pcSnmpClassId) + poTableRow->get_index();
        rowIndex = poTableRow->index_of(snmpObjectOID);
        if (rowIndex != -1)
        {
            poSnmpObject = (M4SnmpStatistic*)poTableRow->get_element(snmpObjectOID);
            poSnmpObject->SetSnmpSubsystem(poSnmpParentSubsystem);
            poSnmpObject->SetPropertyIndex(ai_statisticIndex);
        }
        else
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_FIND_ROW, poSnmpParentSubsystem->GetType());
            retCode = M4_ERROR;
        }
        break;
    default:
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_MIB_TYPE, poSnmpObject->GetMibType() << poSnmpObject->GetName());
        retCode = M4_ERROR;
        break;
    }

    return retCode;
  //## end M4SnmpAdministrator::RegisterStatistic%1083310097.body
}

m4return_t M4SnmpAdministrator::UnregisterSubsystem (M4ISnmpObject* ai_poSnmpObject)
{
  //## begin M4SnmpAdministrator::UnregisterSubsystem%1083666427.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // the SnmpObject is always a M4SnmpSubsystem.
    M4SnmpSubsystem* poSnmpSubsystem = (M4SnmpSubsystem*)ai_poSnmpObject;

    // remove all children of this subsystem.
    const M4SnmpObjectVector v = poSnmpSubsystem->GetChildrenVector();
    for (unsigned int i=0; i<v.size(); i++)
    {
        M4SnmpObject* poSnmpObject = v[i];
        if (NULL != poSnmpObject)
        {
            m_pMib->remove(poSnmpObject->get_oid());
        }
    }

    // remove the subsystem
	M4MibTable* poTable = NULL;
	Agentpp::MibTableRow* poTableRow = NULL;
    switch(poSnmpSubsystem->GetMibType())
    {
    case M4_MIB_LEAF: 
        m_pMib->remove(poSnmpSubsystem->get_oid());
        retCode = M4_SUCCESS;
        break;
    case M4_MIB_STATIC_TABLE_ENTRY: 
    case M4_MIB_DYNAMIC_TABLE_ENTRY: 

		// bg 306048.
		m_pMib->lock_mib();

        // get the parent table.
        poTable = poSnmpSubsystem->GetMibTable();
        if (NULL == poTable)
        {
			m_pMib->unlock_mib();

            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_FIND_TABLE, poSnmpSubsystem->GetType());
            return M4_ERROR;
        }

        // get the row for this subsistem in the table.
        poTableRow = poSnmpSubsystem->GetMibTableRow();
        if (NULL == poTableRow)
        {
			m_pMib->unlock_mib();

            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_FIND_ROW, poSnmpSubsystem->GetType());
            return M4_ERROR;
        }

		poTable->start_synch();
		
        poTable->remove_row(poTableRow->get_index());
		
		poTable->end_synch();
		m_pMib->unlock_mib();

		// bg 179508
		delete poSnmpSubsystem ;
		
        retCode = M4_SUCCESS;
        break;
    default:
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_MIB_TYPE, poSnmpSubsystem->GetMibType() << poSnmpSubsystem->GetType());
        retCode = M4_ERROR;
        break;
    }

    return retCode;
  //## end M4SnmpAdministrator::UnregisterSubsystem%1083666427.body
}

m4return_t M4SnmpAdministrator::Init (const DOMNode* ai_config)
{
  //## begin M4SnmpAdministrator::Init%1084791066.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // initialize snmp port.
    const char* pcSnmpPort = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_port"));
    if (NULL == pcSnmpPort)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_port" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
    m_snmpPort = atoi(pcSnmpPort);
    if (0 == m_snmpPort)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "m_snmpPort" << m_snmpPort);
        return M4_ERROR;
    }

    // get snmp.xml filename.
    const char* szXmlFile  = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_configfile"));
    if (NULL == szXmlFile)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_configfile" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }

    // get snmp.product name.
    const char* szProductName  = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_productname"));
    if (NULL == szProductName)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_productname" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }

    // parse the xml file.
    retCode = LoadXmlConfig(szXmlFile, szProductName);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSING_ERROR, szXmlFile);
        return M4_ERROR;
    }

    // initialize SNMP library.
    retCode = SnmpPPInit(ai_config);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_SNMP_PP_LIBRARY_ERROR, "SnmpPPInit()" << retCode);
        return M4_ERROR;
    }

    // initialize derived class.
    retCode = _Init(ai_config);
    if (M4_ERROR == retCode)
    {
        Destroy();
        return M4_ERROR;
    }

    return M4_SUCCESS;
  //## end M4SnmpAdministrator::Init%1084791066.body
}

m4return_t M4SnmpAdministrator::SnmpPPInit (const DOMNode* ai_config)
{
  //## begin M4SnmpAdministrator::SnmpPPInit%1084791690.body preserve=yes
    // creates the snmp request list.
    if (NULL == m_pRequestList) m_pRequestList = new Agentpp::RequestList();
    if (NULL == m_pRequestList)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new RequestList()");
        Destroy();
        return M4_ERROR;
    }

    // creates the snmp mib and register requestList for outgoing request.
    if (NULL == m_pMib) m_pMib = new Agentpp::Mib();
    if (NULL == m_pMib)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new Mib()");
        Destroy();
        return M4_ERROR;
    }
	m_pMib->init();
    m_pMib->set_request_list(m_pRequestList);

    // initilize the v3MpP.
    v3MP* pv3mp = CreateV3mp(ai_config);
    if (NULL == pv3mp)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "CreateV3mp(ai_config)");
        Destroy();
        return M4_ERROR;
    }
    m_pRequestList->set_v3mp(pv3mp);

	
	// Create public SNMPv3 user if enabled.
	// get snmp.public user state.
    const char* szV3PublicUsr = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v3_public_usr"));
    if (NULL == szV3PublicUsr)
    {	
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v3_public_usr" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
    else if (!stricmp(szV3PublicUsr, "Enabled"))
    {
		const char* szV3PublicUsrName = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_v3_public_usr_name"));
		if (NULL == szV3PublicUsrName)
		{	
			// UNICODE XML
			m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_v3_public_usr_name" << pcNodeName);
			delete [] pcNodeName ;
			return M4_ERROR;
		}
        else
        {
			Agentpp::UsmUserTable *uut = new Agentpp::UsmUserTable();
			uut->addNewRow(szV3PublicUsrName, SNMPv3_usmHMACMD5AuthProtocol, SNMPv3_usmDESPrivProtocol, szV3PublicUsrName, szV3PublicUsrName);
	        m_pMib->add(new Agentpp::usm_mib(uut));
		}
	}


    // initilize the Vacm.
	Agentpp::Vacm* pVacm = CreateVacm(ai_config);
    if (NULL == pVacm)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "CreateVacm(ai_config)");
        Destroy();
        return M4_ERROR;
    }
    m_pRequestList->set_vacm(pVacm);	

    return M4_SUCCESS;
  //## end M4SnmpAdministrator::SnmpPPInit%1084791690.body
}

const DOMNode* M4SnmpAdministrator::Obl2Xml (ClConfiguration* ai_config)
{
  //## begin M4SnmpAdministrator::Obl2Xml%1084894578.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // build XML from OBL.
    /* XML
    <?xml version="1.0" encoding="UTF8"?>
        <snmp
            snmp_active = "false"
            snmp_port = "161" 
            snmp_admintype = "SNMP_STANDALONE" 
            snmp_configfile = "Z:\build\win32\m4serversite\m4dispatcher\conf\snmp.xml" 
            snmp_productname = "dispatcher"
            snmp_v1v2c_state = "disabled"
            snmp_read_community = "public"
            snmp_write_community = "private"
            snmp_v3_public_usr = "disabled"
            snmp_v3_public_usr_name = "v3publicuser"
            snmp_v3_boots_file = "v3bootcounter"
         />
    */

    // get snmp port.
    m4double_t dSnmpPort;
    m4char_t szSnmpPort[OBL_PROPVALUE_MAX_LEN];
    retCode = ai_config->GetNumericValue("", "SNMP_PORT", dSnmpPort);
 	if(retCode == M4_ERROR)
	{
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_PORT");
		return NULL;
	}
    m_snmpPort = (m4int32_t)dSnmpPort;
    sprintf(szSnmpPort, "%d", m_snmpPort);

    // get snmp.xml filename.
    m4size_t iLen;
    m4char_t szXmlFile [OBL_PROPVALUE_MAX_LEN];
    retCode = ai_config->GetStringValue("", "SNMP_CONFIG_FILE", szXmlFile, OBL_PROPVALUE_MAX_LEN, iLen);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_CONFIG_FILE");
        return NULL;
    }

    // get snmp.product name.
    m4char_t szProductName [OBL_PROPVALUE_MAX_LEN];
    retCode = ai_config->GetStringValue("", "SNMP_PRODUCT_NAME", szProductName, OBL_PROPVALUE_MAX_LEN, iLen);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_PRODUCT_NAME");
        return NULL;
    }

    // get snmp.v1v2c state.
    m4char_t szV1V2cState [OBL_PROPVALUE_MAX_LEN];
	m4char_t szReadCommunity  [OBL_PROPVALUE_MAX_LEN];
	m4char_t szWriteCommunity [OBL_PROPVALUE_MAX_LEN];
	
	memset (szReadCommunity,  0, OBL_PROPVALUE_MAX_LEN);
	memset (szWriteCommunity, 0, OBL_PROPVALUE_MAX_LEN);

    retCode = ai_config->GetStringValue("", "SNMP_V1V2C_STATE", szV1V2cState , OBL_PROPVALUE_MAX_LEN, iLen);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V1V2C_STATE");
        return NULL;
    }
    else if (!stricmp(szV1V2cState,"Enabled"))
    {
	    // get snmp.read community.		
		retCode = ai_config->GetStringValue("", "SNMP_READ_COMMUNITY", szReadCommunity , OBL_PROPVALUE_MAX_LEN, iLen);
		if (retCode == M4_ERROR)
		{	
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_READ_COMMUNITY");
			return NULL;
		}
        else if (iLen==0)
		{	
            DUMP_CHLOG_WARNINGF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_READ_COMMUNITY");
			memset (szReadCommunity,  0, OBL_PROPVALUE_MAX_LEN);			
		}

		// get snmp.write community.		
		retCode = ai_config->GetStringValue("", "SNMP_WRITE_COMMUNITY", szWriteCommunity , OBL_PROPVALUE_MAX_LEN, iLen);
		if (retCode == M4_ERROR)
		{	
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_WRITE_COMMUNITY");
			return NULL;
		}
        else if (iLen==0)
		{	
            DUMP_CHLOG_WARNINGF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_WRITE_COMMUNITY");
			memset (szWriteCommunity,  0, OBL_PROPVALUE_MAX_LEN);			
		}
	}

    // get snmp.public user state.
    m4char_t szV3PublicUsr [OBL_PROPVALUE_MAX_LEN];
	m4char_t szV3PublicUsrName [OBL_PROPVALUE_MAX_LEN];
	memset (szV3PublicUsr, 0, OBL_PROPVALUE_MAX_LEN);
	memset (szV3PublicUsrName, 0, OBL_PROPVALUE_MAX_LEN);
    retCode = ai_config->GetStringValue("", "SNMP_V3_PUBLIC_USR", szV3PublicUsr, OBL_PROPVALUE_MAX_LEN, iLen);

    if (retCode == M4_ERROR)
    {	
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V3_PUBLIC_USR");
        return NULL;
    }
    else if (!stricmp(szV3PublicUsr,"Enabled"))
    {
		// get snmp.public user name.		
		retCode = ai_config->GetStringValue("", "SNMP_V3_PUBLIC_USR_NAME", szV3PublicUsrName , OBL_PROPVALUE_MAX_LEN, iLen);
		if (retCode == M4_ERROR)
		{	
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V3_PUBLIC_USR_NAME");
			return NULL;
		}
        else if (iLen==0)
		{	
            DUMP_CHLOG_WARNINGF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V3_PUBLIC_USR_NAME");
			memset (szV3PublicUsrName,  0, OBL_PROPVALUE_MAX_LEN);			
		}
	}

    // get snmp.boots file.
    m4char_t szV3BootsFile [OBL_PROPVALUE_MAX_LEN];
    retCode = ai_config->GetStringValue("", "SNMP_V3_BOOTS_FILE", szV3BootsFile, OBL_PROPVALUE_MAX_LEN, iLen);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V3_BOOTS_FILE");
        return NULL;
    }
    else if (iLen==0)
	{	
        DUMP_CHLOG_WARNINGF(M4_SNMP_CANNOT_GET_OBL_ITEM, "SNMP_V3_BOOTS_FILE");
		memset (szV3BootsFile, 0, OBL_PROPVALUE_MAX_LEN);
		strcpy (szV3BootsFile, "snmpV3BootCounter");		
	}


    M4XMLStreamGenerator oXMLGenerator;
    m4pchar_t pcXml = NULL;
	
	oXMLGenerator.MakeXMLDeclaration();
    oXMLGenerator.MakeElement("snmp");
	oXMLGenerator.AddElementAttribute("snmp_port",            szSnmpPort);
	oXMLGenerator.AddElementAttribute("snmp_configfile",      szXmlFile);
	oXMLGenerator.AddElementAttribute("snmp_productname",     szProductName);
	oXMLGenerator.AddElementAttribute("snmp_v1v2c_state",      szV1V2cState);
	oXMLGenerator.AddElementAttribute("snmp_read_community",   szReadCommunity);
	oXMLGenerator.AddElementAttribute("snmp_write_community",  szWriteCommunity);
	oXMLGenerator.AddElementAttribute("snmp_v3_public_usr",     szV3PublicUsr);
	oXMLGenerator.AddElementAttribute("snmp_v3_public_usr_name", szV3PublicUsrName);
	oXMLGenerator.AddElementAttribute("snmp_v3_boots_file",     szV3BootsFile);	
	oXMLGenerator.CloseSingleClosedElement();

    retCode = oXMLGenerator.GetXMLStream(pcXml, -1);
    if ((M4_ERROR==retCode) || (NULL == pcXml))
    {
        DUMP_CHLOG_ERROR(M4_SNMP_OBL2XML_ERROR);
        return NULL;
    }
   
    // Initialize the XML4C system
    XMLPlatformUtils::Initialize();
 
    // Instantiate the DOM parser.
    m_oParser.setValidationScheme(XercesDOMParser::Val_Never);

    // And create our error handler and install it
    SnmpErrorHandler errorHandler;
    m_oParser.setErrorHandler(&errorHandler);

    // parse the document.
    m_oParser.parse(MemBufInputSource((const XMLByte *)pcXml, strlen(pcXml), "generic"));

    // free xml buffer.
    if (NULL != pcXml)
    {
        delete pcXml;
    }

    if (M4_TRUE == errorHandler.hasErrors())
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSING_ERROR, "");
        return NULL;
    }

    // get document.
    xercesc::DOMDocument *d  = m_oParser.getDocument();

    // get params node.
    const DOMNode *n2 = SnmpDOMUtils::findChildNodeByName(d, "snmp");
    if (n2 == NULL)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_NODE_BY_NAME, "snmp");
        return NULL;
    }

    return n2;
  //## end M4SnmpAdministrator::Obl2Xml%1084894578.body
}

m4return_t M4SnmpAdministrator::RegisterUser (m4pcchar_t ai_name, m4pcchar_t ai_password)
{
  //## begin M4SnmpAdministrator::RegisterUser%1084894580.body preserve=yes
	if (ai_password==NULL)
	{
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_name" << "null");
		return M4_ERROR;
	}
	if (ai_password==NULL)
	{
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "ai_password" << "null");
		return M4_ERROR;
	}
    if (*ai_name==0)
	{
        DUMP_CHLOG_WARNINGF(M4_SNMP_INVALID_VALUE, "ai_name" << "null");
		return M4_WARNING;
	}
    if (*ai_password==0)
	{
        DUMP_CHLOG_WARNINGF(M4_SNMP_INVALID_VALUE, "ai_password" << "null");
		return M4_WARNING;
	}


	Agentpp::UsmUserTable *uut = new Agentpp::UsmUserTable();
	uut->addNewRow(ai_name, SNMPv3_usmHMACMD5AuthProtocol, SNMPv3_usmDESPrivProtocol, ai_password, ai_password);
	m_pMib->add(new Agentpp::usm_mib(uut));

	Agentpp::Vacm* pVacm = m_pRequestList->get_vacm();
	pVacm->addNewGroup(SecurityModel_USM, ai_name, "v3WriteGroup", storageType_volatile);
	pVacm->addNewAccessEntry("v3WriteGroup", "", SecurityModel_USM, SecurityLevel_authPriv, match_prefix, "v3WriteView", "v3WriteView", "v3WriteView", storageType_nonVolatile);
	pVacm->addNewView("v3WriteView", SNMP_TREE_ROOT, "", view_included, storageType_nonVolatile);

    return M4_SUCCESS;
  //## end M4SnmpAdministrator::RegisterUser%1084894580.body
}

// Additional Declarations
  //## begin M4SnmpAdministrator%406BD9E60047.declarations preserve=yes
  //## end M4SnmpAdministrator%406BD9E60047.declarations

//## begin module%406BE419036C.epilog preserve=yes
//## end module%406BE419036C.epilog
