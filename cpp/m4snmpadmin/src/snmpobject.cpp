//## begin module%406BEFC6012F.cm preserve=no
//## end module%406BEFC6012F.cm

//## begin module%406BEFC6012F.cp preserve=no
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
//## end module%406BEFC6012F.cp

//## Module: snmpobject%406BEFC6012F; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmpobject.cpp

//## begin module%406BEFC6012F.additionalIncludes preserve=no
//## end module%406BEFC6012F.additionalIncludes

#include "snmpxmlutil.hpp"
#include "snmptables.hpp"
#include "cloblconfiguration.hpp"
#include "snmpres.hpp"
#include <snmpobject.hpp>
#include <snmpinterfaces.hpp>
#include "blocksynchronization.hpp"


//## begin module%406BEFC6012F.declarations preserve=no
//## end module%406BEFC6012F.declarations

//## begin module%406BEFC6012F.additionalDeclarations preserve=yes
#define MAX_SNMP_PARAMETER_VALUE_SIZE 1024
//## end module%406BEFC6012F.additionalDeclarations


extern  ClMutex	g_oMutex;

// Class M4SnmpObject 







M4SnmpObject::M4SnmpObject()
  //## begin M4SnmpObject::M4SnmpObject%.hasinit preserve=no
      : m_status(CREATED)
  //## end M4SnmpObject::M4SnmpObject%.hasinit
  //## begin M4SnmpObject::M4SnmpObject%.initialization preserve=yes
  //## end M4SnmpObject::M4SnmpObject%.initialization
{
	m_domNode = NULL;
  //## begin M4SnmpObject::M4SnmpObject%.body preserve=yes
  //## end M4SnmpObject::M4SnmpObject%.body
}

M4SnmpObject::M4SnmpObject(const M4SnmpObject &right)
  //## begin M4SnmpObject::M4SnmpObject%copy.hasinit preserve=no
      : m_status(CREATED)
  //## end M4SnmpObject::M4SnmpObject%copy.hasinit
  //## begin M4SnmpObject::M4SnmpObject%copy.initialization preserve=yes
  ,MibLeaf(right)
  //## end M4SnmpObject::M4SnmpObject%copy.initialization
{
  //## begin M4SnmpObject::M4SnmpObject%copy.body preserve=yes
    m_classId = right.m_classId;
    m_instanceId = right.m_instanceId;
    m_domNode = right.m_domNode;
    m_iSnmpSyntax = right.m_iSnmpSyntax;
    m_mibType = right.m_mibType;
    m_status = right.m_status;
  //## end M4SnmpObject::M4SnmpObject%copy.body
}


M4SnmpObject::~M4SnmpObject()
{
  //## begin M4SnmpObject::~M4SnmpObject%.body preserve=yes
  //## end M4SnmpObject::~M4SnmpObject%.body
}



//## Other Operations (implementation)
m4string_t M4SnmpObject::GetClassId ()
{
  //## begin M4SnmpObject::GetClassId%1081862807.body preserve=yes
    return m_classId;
  //## end M4SnmpObject::GetClassId%1081862807.body
}

m4string_t M4SnmpObject::GetInstanceId ()
{
  //## begin M4SnmpObject::GetInstanceId%1081862808.body preserve=yes
    return m_instanceId;
  //## end M4SnmpObject::GetInstanceId%1081862808.body
}

m4string_t M4SnmpObject::GetOID ()
{
  //## begin M4SnmpObject::GetOID%1081862809.body preserve=yes
    m4string_t sOid = m_classId;

    if (m_instanceId != "")
    {
        sOid += "." + m_instanceId;
    }

    return  sOid;
  //## end M4SnmpObject::GetOID%1081862809.body
}

void M4SnmpObject::SetSnmpAccess ()
{
  //## begin M4SnmpObject::SetSnmpAccess%1082361886.body preserve=yes
    int eAccess = Agentpp::READONLY;

    if (m_domNode != 0)
    {
        const char* pcAccess = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("access"));

        if (NULL != pcAccess)
        {
            eAccess = atoi(pcAccess);

            switch(eAccess)
            {
            case Agentpp::NOACCESS: 
            case Agentpp::READONLY:
            case Agentpp::READWRITE:
            case Agentpp::READCREATE:
                // ok;
                break;
            default:
                DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "SnmpAccess" << pcAccess);
                break;
            }
        }
    }

    set_access((Agentpp::mib_access)eAccess);
  //## end M4SnmpObject::SetSnmpAccess%1082361886.body
}

void M4SnmpObject::SetSnmpSyntax (m4pcchar_t ai_value)
{
  //## begin M4SnmpObject::SetSnmpSyntax%1082361888.body preserve=yes
    SnmpSyntax* poSyntax = NULL;
    if (m_domNode != 0)
    {
        const char* pcSyntax = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("syntax"));
        if (NULL != pcSyntax)
        {
            switch(atoi(pcSyntax))
            {
            case sNMP_SYNTAX_INT32: 
                poSyntax = new SnmpInt32(atoi(ai_value));
                m_iSnmpSyntax = sNMP_SYNTAX_INT32;
                break;
            default:
                poSyntax = new OctetStr(ai_value);
                m_iSnmpSyntax = sNMP_SYNTAX_OCTETS;
                break;
            }
        }
        else
        {
            poSyntax = new OctetStr(ai_value);
			m_iSnmpSyntax = sNMP_SYNTAX_OCTETS;
        }
    }
    else
    {
        poSyntax = new OctetStr(ai_value);
    }

    replace_value(poSyntax);
  //## end M4SnmpObject::SetSnmpSyntax%1082361888.body
}

const DOMNode* M4SnmpObject::GetDOMNode ()
{
  //## begin M4SnmpObject::GetDOMNode%1082042651.body preserve=yes
    return m_domNode;
  //## end M4SnmpObject::GetDOMNode%1082042651.body
}

void M4SnmpObject::SetDOMNode (const DOMNode *ai_node)
{
  //## begin M4SnmpObject::SetDOMNode%1082042652.body preserve=yes
    m_domNode = ai_node;
  //## end M4SnmpObject::SetDOMNode%1082042652.body
}

void M4SnmpObject::SetSnmpOid ()
{
  //## begin M4SnmpObject::SetSnmpOid%1082361890.body preserve=yes
    set_oid(Oid(GetOID().c_str()));
  //## end M4SnmpObject::SetSnmpOid%1082361890.body
}

void M4SnmpObject::SetMibType ()
{
  //## begin M4SnmpObject::SetMibType%1082986488.body preserve=yes
    const char* pcType = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("mibtype"));
    M4MibType eType = M4_MIB_LEAF;
    int iType = -1;
    if (NULL != pcType)
    {
        iType = atoi(pcType);
        switch(iType)
        {
        case M4_MIB_LEAF: 
        case M4_MIB_STATIC_TABLE_ENTRY: 
        case M4_MIB_DYNAMIC_TABLE_ENTRY: 
            eType = (M4MibType)iType;
            break;
        default:
            // Unknown type. Set default type.
            DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "mibtype" << iType);
            eType = M4_MIB_LEAF;
            break;
        }
    }

    m_mibType = eType;
  //## end M4SnmpObject::SetMibType%1082986488.body
}

M4MibType M4SnmpObject::GetMibType ()
{
  //## begin M4SnmpObject::GetMibType%1082986489.body preserve=yes
    return m_mibType;
  //## end M4SnmpObject::GetMibType%1082986489.body
}

M4SnmpObjectStatus M4SnmpObject::GetStatus ()
{
  //## begin M4SnmpObject::GetStatus%1082986507.body preserve=yes
    return m_status;
  //## end M4SnmpObject::GetStatus%1082986507.body
}

void M4SnmpObject::SetStatus (M4SnmpObjectStatus ai_status)
{
  //## begin M4SnmpObject::SetStatus%1082986508.body preserve=yes
    m_status = ai_status;
  //## end M4SnmpObject::SetStatus%1082986508.body
}

// Additional Declarations
  //## begin M4SnmpObject%406BEAB202CB.declarations preserve=yes
  //## end M4SnmpObject%406BEAB202CB.declarations

// Class M4SnmpSubsystem 












M4SnmpSubsystem::M4SnmpSubsystem (const m4string_t& ai_name, const DOMNode *ai_domNode, const m4string_t& ai_parentClassId, M4SnmpSubsystem* ai_poParentSubsystem, M4ISSApi4Snmp* ai_poSSApi)
  //## begin M4SnmpSubsystem::M4SnmpSubsystem%1081862813.hasinit preserve=no
      : m_poMibTableRow(NULL), m_poMibTable(NULL), m_poParentSubsystem(NULL)
  //## end M4SnmpSubsystem::M4SnmpSubsystem%1081862813.hasinit
  //## begin M4SnmpSubsystem::M4SnmpSubsystem%1081862813.initialization preserve=yes
  //## end M4SnmpSubsystem::M4SnmpSubsystem%1081862813.initialization
{
  //## begin M4SnmpSubsystem::M4SnmpSubsystem%1081862813.body preserve=yes
    m_instanceId = m4string_t("0");
    SetDOMNode(ai_domNode);
    m_sstype = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("type"));
    m_poParentSubsystem = ai_poParentSubsystem;
    m_pM4ISSApi4Snmp = ai_poSSApi;

    
    // build subsystem classid.
    m4pcchar_t pcSnmpClassId = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("oid"));
    if (NULL != pcSnmpClassId)
    {
        m_classId = ai_parentClassId + "." + m4string_t(pcSnmpClassId);
    }
    else
    {
        m_classId = ai_parentClassId;
    }

    // SNMP++ initialization.
    SetSnmpOid();
    SetSnmpAccess();
    SetSnmpSyntax(ai_name.c_str());
    SetMibType();
  //## end M4SnmpSubsystem::M4SnmpSubsystem%1081862813.body
}


M4SnmpSubsystem::~M4SnmpSubsystem()
{
  //## begin M4SnmpSubsystem::~M4SnmpSubsystem%.body preserve=yes
    if (NULL != m_pM4ISSApi4Snmp) delete m_pM4ISSApi4Snmp;
    m_pM4ISSApi4Snmp = NULL;
  //## end M4SnmpSubsystem::~M4SnmpSubsystem%.body
}



//## Other Operations (implementation)
const DOMNode* M4SnmpSubsystem::GetDOMPropertiesNode ()
{
  //## begin M4SnmpSubsystem::GetDOMPropertiesNode%1082108038.body preserve=yes
    return m_domPropertiesNode;
  //## end M4SnmpSubsystem::GetDOMPropertiesNode%1082108038.body
}

const DOMNode* M4SnmpSubsystem::GetDOMStatisticsNode ()
{
  //## begin M4SnmpSubsystem::GetDOMStatisticsNode%1082108039.body preserve=yes
    return m_domStatisticsNode;
  //## end M4SnmpSubsystem::GetDOMStatisticsNode%1082108039.body
}

void M4SnmpSubsystem::SetDOMNode (const DOMNode *ai_node)
{
  //## begin M4SnmpSubsystem::SetDOMNode%1082361889.body preserve=yes
    M4SnmpObject::SetDOMNode(ai_node);
    m_domPropertiesNode = SnmpDOMUtils::findChildNodeByName(m_domNode, "properties");
    m_domStatisticsNode = SnmpDOMUtils::findChildNodeByName(m_domNode, "statistics");
  //## end M4SnmpSubsystem::SetDOMNode%1082361889.body
}

const M4ISSApi4Snmp* M4SnmpSubsystem::GetSSApi ()
{
  //## begin M4SnmpSubsystem::GetSSApi%1082361897.body preserve=yes
    return m_pM4ISSApi4Snmp;
  //## end M4SnmpSubsystem::GetSSApi%1082361897.body
}

M4MibTable* M4SnmpSubsystem::GetMibTable ()
{
  //## begin M4SnmpSubsystem::GetMibTable%1082986496.body preserve=yes
    return m_poMibTable;
  //## end M4SnmpSubsystem::GetMibTable%1082986496.body
}

Agentpp::MibTableRow* M4SnmpSubsystem::GetMibTableRow ()
{
  //## begin M4SnmpSubsystem::GetMibTableRow%1083080474.body preserve=yes
    return m_poMibTableRow;
  //## end M4SnmpSubsystem::GetMibTableRow%1083080474.body
}

void M4SnmpSubsystem::SetMibTableRow (Agentpp::MibTableRow* ai_poRow)
{
  //## begin M4SnmpSubsystem::SetMibTableRow%1083080475.body preserve=yes
    m_poMibTableRow = ai_poRow;
  //## end M4SnmpSubsystem::SetMibTableRow%1083080475.body
}

void M4SnmpSubsystem::AddChildObject (M4SnmpObject* ai_poSnmpObject)
{
  //## begin M4SnmpSubsystem::AddChildObject%1083666428.body preserve=yes
    if (NULL != ai_poSnmpObject)
    {
        m_pM4SnmpObjectVector.push_back(ai_poSnmpObject);
    }
  //## end M4SnmpSubsystem::AddChildObject%1083666428.body
}

const M4SnmpObjectVector& M4SnmpSubsystem::GetChildrenVector ()
{
  //## begin M4SnmpSubsystem::GetChildrenVector%1083666429.body preserve=yes
    return m_pM4SnmpObjectVector;
  //## end M4SnmpSubsystem::GetChildrenVector%1083666429.body
}

void M4SnmpSubsystem::SetMibTable (M4MibTable* ai_table)
{
  //## begin M4SnmpSubsystem::SetMibTable%1083741949.body preserve=yes
    m_poMibTable = ai_table;
  //## end M4SnmpSubsystem::SetMibTable%1083741949.body
}

m4string_t M4SnmpSubsystem::GetType ()
{
  //## begin M4SnmpSubsystem::GetType%1083741950.body preserve=yes
    return m_sstype;
  //## end M4SnmpSubsystem::GetType%1083741950.body
}

Oid M4SnmpSubsystem::GetMibTableRowIndex ()
{
  //## begin M4SnmpSubsystem::GetMibTableRowIndex%1083741956.body preserve=yes
    Oid index;

    // get the index of my parent.
    if (NULL != m_poParentSubsystem)
    {
        index += m_poParentSubsystem->GetMibTableRowIndex();
    }

    // get my index.
    if (NULL != m_poMibTableRow)
    {
        unsigned long rowLastIndex = m_poMibTableRow->get_index().last();
        m4char_t cIndex[32];
        sprintf (cIndex, "%u", rowLastIndex);
        index += cIndex;
    }

    return index;
  //## end M4SnmpSubsystem::GetMibTableRowIndex%1083741956.body
}

// Additional Declarations
  //## begin M4SnmpSubsystem%406BEB54025F.declarations preserve=yes
  //## end M4SnmpSubsystem%406BEB54025F.declarations

// Class M4SnmpProperty 




M4SnmpProperty::M4SnmpProperty(const M4SnmpProperty &right)
  //## begin M4SnmpProperty::M4SnmpProperty%copy.hasinit preserve=no
  //## end M4SnmpProperty::M4SnmpProperty%copy.hasinit
  //## begin M4SnmpProperty::M4SnmpProperty%copy.initialization preserve=yes
  : M4SnmpObject(right)
  //## end M4SnmpProperty::M4SnmpProperty%copy.initialization
{
  //## begin M4SnmpProperty::M4SnmpProperty%copy.body preserve=yes
    m_pM4SnmpSubsystem = right.m_pM4SnmpSubsystem;
    m_name = right.m_name;
    m_propertyIndex = right.m_propertyIndex;
  //## end M4SnmpProperty::M4SnmpProperty%copy.body
}

M4SnmpProperty::M4SnmpProperty (const m4string_t& ai_name, m4int32_t ai_propertyIndex, const m4string_t& ai_classid, const DOMNode *ai_domNode, M4SnmpSubsystem* ai_poParentSnmp)
  //## begin M4SnmpProperty::M4SnmpProperty%1082108040.hasinit preserve=no
  //## end M4SnmpProperty::M4SnmpProperty%1082108040.hasinit
  //## begin M4SnmpProperty::M4SnmpProperty%1082108040.initialization preserve=yes
  //## end M4SnmpProperty::M4SnmpProperty%1082108040.initialization
{
  //## begin M4SnmpProperty::M4SnmpProperty%1082108040.body preserve=yes
    m_name = ai_name;
    m_propertyIndex = ai_propertyIndex;
    m_pM4SnmpSubsystem = ai_poParentSnmp;
    SetDOMNode(ai_domNode);

    // SNMP++ initialization.
    if ((NULL != m_pM4SnmpSubsystem) && (M4_MIB_LEAF == m_pM4SnmpSubsystem->GetMibType()))
    {
        m_classId = m_pM4SnmpSubsystem->GetClassId() + "." + m4string_t(ai_classid);
        m_instanceId = m4string_t("0");
    }
    else
    {
        m_classId = m4string_t(ai_classid);
        m_instanceId = m4string_t("");
    }

    SetSnmpOid();
    SetSnmpAccess();
    SetSnmpSyntax("uninitialized-value");
  //## end M4SnmpProperty::M4SnmpProperty%1082108040.body
}


M4SnmpProperty::~M4SnmpProperty()
{
  //## begin M4SnmpProperty::~M4SnmpProperty%.body preserve=yes
  //## end M4SnmpProperty::~M4SnmpProperty%.body
}



//## Other Operations (implementation)
void M4SnmpProperty::get_request (Agentpp::Request* ai_req, int ai_ind)
{
  //## begin M4SnmpProperty::get_request%1082361891.body preserve=yes
	ClMutBlock	oBlock( &g_oMutex ) ;

    if (NULL != m_pM4SnmpSubsystem)
    {
        // read configuration.
        m4string_t szValue;
        m4return_t retCode = GetValue(szValue);

        // set the value of snmp object with value in configuration.
        if (M4_ERROR != retCode)
	    {
		    SetSnmpSyntax(szValue.c_str());
	    }
    }
        
    // call parent function.
    MibLeaf::get_request(ai_req, ai_ind);
  //## end M4SnmpProperty::get_request%1082361891.body
}

int M4SnmpProperty::set_value (const Agentpp::Vbx& ai_vb)
{
  //## begin M4SnmpProperty::set_value%1082361893.body preserve=yes
    // SNMP_ERROR_SUCCESS if the new value has been set, or SNMP_ERROR_BAD_VALUE otherwise.
	ClMutBlock	oBlock( &g_oMutex ) ;

    int iSetRetCode = SNMP_ERROR_BAD_VALUE; 

    if (NULL != m_pM4SnmpSubsystem)
    {
        const M4ISSApi4Snmp* poSSApi = m_pM4SnmpSubsystem->GetSSApi();
        if (NULL != poSSApi)
        {
            // read node configuration.
	        m4pchar_t commandId = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("cmdid"));
	        m4pchar_t paramId = SnmpDOMUtils::getAttVal(m_domNode, M4_XERCES_LITERAL("paramid"));

            // read param value.
            unsigned long len;
            unsigned long maxlen = MAX_SNMP_PARAMETER_VALUE_SIZE;
            m4char_t pcValue[MAX_SNMP_PARAMETER_VALUE_SIZE];
            long iValue = 0;
            int iGetRetCode = SNMP_CLASS_ERROR;

            switch(m_iSnmpSyntax)
            {
            case sNMP_SYNTAX_INT32: 
                iGetRetCode = ai_vb.get_value(iValue);
                sprintf(pcValue, "%ld", iValue);
                break;
            case sNMP_SYNTAX_OCTETS: 
                iGetRetCode = ai_vb.get_value((unsigned char*)pcValue, len, maxlen, true);
                if (len > MAX_SNMP_PARAMETER_VALUE_SIZE) 
				{
                    DUMP_CHLOG_ERRORF(M4_SNMP_SNMP_PP_LIBRARY_ERROR, "get_value(): " << "len > MAX_SNMP_PARAMETER_VALUE_SIZE");
					iGetRetCode = SNMP_CLASS_ERROR;
				}
                break;
            default:
                iGetRetCode = SNMP_CLASS_ERROR;
                break;
            }

            // execute command
            if (SNMP_CLASS_SUCCESS == iGetRetCode)
            {
                m4return_t retCode = poSSApi->ExecuteCommand(commandId, paramId, pcValue);
                if (M4_ERROR != retCode)
	            {
		            iSetRetCode = SNMP_ERROR_SUCCESS;
	            }
            }

            delete commandId;
	        delete paramId;
        }
    }

    // call parent method. Discard the return code.
    MibLeaf::set_value(ai_vb);

    // the return code is set by meta4 operation an not by mib operation.
    return iSetRetCode;
  //## end M4SnmpProperty::set_value%1082361893.body
}

M4SnmpSubsystem* M4SnmpProperty::GetSnmpSubsystem ()
{
  //## begin M4SnmpProperty::GetSnmpSubsystem%1083080478.body preserve=yes
    return m_pM4SnmpSubsystem;
  //## end M4SnmpProperty::GetSnmpSubsystem%1083080478.body
}

void M4SnmpProperty::SetSnmpSubsystem (M4SnmpSubsystem* ai_poSubsystem)
{
  //## begin M4SnmpProperty::SetSnmpSubsystem%1083080479.body preserve=yes
    m_pM4SnmpSubsystem = ai_poSubsystem;
  //## end M4SnmpProperty::SetSnmpSubsystem%1083080479.body
}

Agentpp::MibEntryPtr M4SnmpProperty::clone ()
{
  //## begin M4SnmpProperty::clone%1083080480.body preserve=yes
	ClMutBlock	oBlock( &g_oMutex ) ;

    return new M4SnmpProperty(*this);
  //## end M4SnmpProperty::clone%1083080480.body
}

void M4SnmpProperty::SetName (const m4string_t& ai_name)
{
  //## begin M4SnmpProperty::SetName%1083080481.body preserve=yes
    m_name = ai_name;
  //## end M4SnmpProperty::SetName%1083080481.body
}

m4string_t M4SnmpProperty::GetName ()
{
  //## begin M4SnmpProperty::GetName%1083080482.body preserve=yes
    return m_name;
  //## end M4SnmpProperty::GetName%1083080482.body
}

m4return_t M4SnmpProperty::GetValue (m4string_t& ao_value)
{
  //## begin M4SnmpProperty::GetValue%1083310094.body preserve=yes
    m4return_t retCode = M4_ERROR;
    const M4ISSApi4Snmp* poSSApi = m_pM4SnmpSubsystem->GetSSApi();

    if (NULL != poSSApi)
    {
        // read configuration.
        retCode = poSSApi->GetPropertyValue(m_name, m_propertyIndex, ao_value);
    }

    return retCode;
  //## end M4SnmpProperty::GetValue%1083310094.body
}

void M4SnmpProperty::SetPropertyIndex (m4int32_t ai_propertyIndex)
{
  //## begin M4SnmpProperty::SetPropertyIndex%1084894576.body preserve=yes
    m_propertyIndex = ai_propertyIndex;
  //## end M4SnmpProperty::SetPropertyIndex%1084894576.body
}

// Additional Declarations
  //## begin M4SnmpProperty%406BEB66016B.declarations preserve=yes
  //## end M4SnmpProperty%406BEB66016B.declarations

// Class M4SnmpStatistic 

M4SnmpStatistic::M4SnmpStatistic(const M4SnmpStatistic &right)
  //## begin M4SnmpStatistic::M4SnmpStatistic%copy.hasinit preserve=no
  //## end M4SnmpStatistic::M4SnmpStatistic%copy.hasinit
  //## begin M4SnmpStatistic::M4SnmpStatistic%copy.initialization preserve=yes
  : M4SnmpProperty(right)
  //## end M4SnmpStatistic::M4SnmpStatistic%copy.initialization
{
  //## begin M4SnmpStatistic::M4SnmpStatistic%copy.body preserve=yes
  //## end M4SnmpStatistic::M4SnmpStatistic%copy.body
}

M4SnmpStatistic::M4SnmpStatistic (const m4string_t& ai_name, m4int32_t ai_propertyIndex, const m4string_t& ai_classid, const DOMNode *ai_domNode, M4SnmpSubsystem* ai_poParentSnmp)
  //## begin M4SnmpStatistic::M4SnmpStatistic%1084894577.hasinit preserve=no
  //## end M4SnmpStatistic::M4SnmpStatistic%1084894577.hasinit
  //## begin M4SnmpStatistic::M4SnmpStatistic%1084894577.initialization preserve=yes
  :M4SnmpProperty(ai_name, ai_propertyIndex, ai_classid, ai_domNode, ai_poParentSnmp)
  //## end M4SnmpStatistic::M4SnmpStatistic%1084894577.initialization
{
  //## begin M4SnmpStatistic::M4SnmpStatistic%1084894577.body preserve=yes
  //## end M4SnmpStatistic::M4SnmpStatistic%1084894577.body
}


M4SnmpStatistic::~M4SnmpStatistic()
{
  //## begin M4SnmpStatistic::~M4SnmpStatistic%.body preserve=yes
  //## end M4SnmpStatistic::~M4SnmpStatistic%.body
}



//## Other Operations (implementation)
m4return_t M4SnmpStatistic::GetValue (m4string_t& ao_value)
{
  //## begin M4SnmpStatistic::GetValue%1083310095.body preserve=yes
    m4return_t retCode = M4_ERROR;
    const M4ISSApi4Snmp* poSSApi = m_pM4SnmpSubsystem->GetSSApi();

    if (NULL != poSSApi)
    {
        // read configuration.
        retCode = poSSApi->GetStatisticValue(m_name, m_propertyIndex, ao_value);
    }

    return retCode;
  //## end M4SnmpStatistic::GetValue%1083310095.body
}

Agentpp::MibEntryPtr M4SnmpStatistic::clone ()
{
  //## begin M4SnmpStatistic::clone%1084977518.body preserve=yes
	ClMutBlock	oBlock( &g_oMutex ) ;

    return new M4SnmpStatistic(*this);
  //## end M4SnmpStatistic::clone%1084977518.body
}

// Additional Declarations
  //## begin M4SnmpStatistic%4091FF7503B3.declarations preserve=yes
  //## end M4SnmpStatistic%4091FF7503B3.declarations

//## begin module%406BEFC6012F.epilog preserve=yes
//## end module%406BEFC6012F.epilog
