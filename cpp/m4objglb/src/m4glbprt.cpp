//## begin module%3912A6BD0351.cm preserve=no
//## end module%3912A6BD0351.cm

//## begin module%3912A6BD0351.cp preserve=no
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
//## end module%3912A6BD0351.cp

//## Module: m4glbprt%3912A6BD0351; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\m4glbprt.cpp

//## begin module%3912A6BD0351.additionalIncludes preserve=no
//## end module%3912A6BD0351.additionalIncludes

//## begin module%3912A6BD0351.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                m4glbprt.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las funciones globales de impresión
//
//
//
//==============================================================================



#include "m4objglb.hpp"

#include "cldefine.hpp"
//## end module%3912A6BD0351.includes

//## begin module%3912A6BD0351.declarations preserve=no
//## end module%3912A6BD0351.declarations

//## begin module%3912A6BD0351.additionalDeclarations preserve=yes
//## end module%3912A6BD0351.additionalDeclarations


//## begin module%3912A6BD0351.epilog preserve=yes

m4pcchar_t  M4ClConnectedNodeType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_CONNECTOR_TYPE_RB :
            iResult = "Register Block" ;
            break ;

        case    M4CL_NODE_CONNECTOR_TYPE_BB :
            iResult = "Block Block" ;
            break ;

        case    M4CL_NODE_CONNECTOR_TYPE_FREE :
            iResult = "Free" ;
            break ;

        case    M4CL_NODE_CONNECTOR_TYPE_AUTO :
            iResult = "Auto" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClCppType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CPP_TYPE_NULL :
            iResult = "Null" ;
            break ;

        case    M4CL_CPP_TYPE_NUMBER :
            iResult = "Number" ;
            break ;

        case    M4CL_CPP_TYPE_DATE :
            iResult = "Date" ;
            break ;

        case    M4CL_CPP_TYPE_STRING_VAR :
        case    M4CL_CPP_TYPE_STRING_SYS :
        case    M4CL_CPP_TYPE_STRING :
        case    M4CL_CPP_TYPE_STRING_POOL :

            iResult = "String" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClM4Type( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_M4_TYPE_NULL :
            iResult = "NULL" ;
            break ;

        case    M4CL_M4_TYPE_FIXED_STRING :
            iResult = "Fixed String" ;
            break ;

        case    M4CL_M4_TYPE_UNI_FIXED_STRING :
            iResult = "Unicode Fixed String" ;
            break ;

        case    M4CL_M4_TYPE_VAR_STRING :
            iResult = "Variable String" ;
            break ;

        case    M4CL_M4_TYPE_UNI_VAR_STRING :
            iResult = "Unicode Variable String" ;
            break ;

        case    M4CL_M4_TYPE_LONG :
            iResult = "Long" ;
            break ;

        case    M4CL_M4_TYPE_UNI_LONG :
            iResult = "Unicode Long" ;
            break ;

        case    M4CL_M4_TYPE_DATE :
            iResult = "Date" ;
            break ;

        case    M4CL_M4_TYPE_TIMESTAMP :
            iResult = "Timestamp" ;
            break ;

        case    M4CL_M4_TYPE_HOUR :
            iResult = "Time" ;
            break ;

        case    M4CL_M4_TYPE_TIME_SPAN :
            iResult = "Time interval" ;
            break ;

        case    M4CL_M4_TYPE_NUMBER :
            iResult = "Number" ;
            break ;

        case    M4CL_M4_TYPE_CURRENCY :
            iResult = "Currency" ;
            break ;

        case    M4CL_M4_TYPE_VARIANT :
            iResult = "Variant" ;
            break ;

        case    M4CL_M4_TYPE_UNI_VARIANT :
            iResult = "Unicode Variant" ;
            break ;

        case    M4CL_M4_TYPE_NUM_VARIANT :
            iResult = "Number Variant" ;
            break ;

        case    M4CL_M4_TYPE_BLOB :
            iResult = "Blob" ;
            break ;

        case    M4CL_M4_TYPE_BINARY_STRING :
            iResult = "Binary String" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemScope( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_SCOPE_NODE :
            iResult = "Node" ;
            break ;

        case    M4CL_ITEM_SCOPE_BLOCK :
            iResult = "Block" ;
            break ;

        case    M4CL_ITEM_SCOPE_REGISTER :
            iResult = "Register" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClCsType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CSTYPE_UNKNOWN :
            iResult = "Unknown" ;
            break ;

        case    M4CL_CSTYPE_FRONT :
            iResult = "Front" ;
            break ;

        case    M4CL_CSTYPE_BACK :
            iResult = "Back" ;
            break ;

        case    M4CL_CSTYPE_BOTH :
            iResult = "Both" ;
            break ;

        case    M4CL_CSTYPE_DEBUG :
            iResult = "Debug" ;
            break ;

        case    M4CL_CSTYPE_MAPPED :
            iResult = "Mapped" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClConnCsType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CSTYPE_FRONT :
            iResult = "Front" ;
            break ;

        case    M4CL_CSTYPE_BACK :
            iResult = "Back" ;
            break ;

        case    M4CL_CSTYPE_BOTH :
            iResult = "Both" ;
            break ;

        case    M4CL_CSTYPE_MAPPED :
            iResult = "Insite" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClOrganizationType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ORGANIZATION_TYPE_NONE :
            iResult = "No Organization" ;
            break ;

        case    M4CL_ORGANIZATION_TYPE_MONO :
            iResult = "Mono Organization" ;
            break ;

        case    M4CL_ORGANIZATION_TYPE_MULTI :
            iResult = "Multi Organization" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClCsExeType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CSEXETYPE_OLTP :
            iResult = "Oltp" ;
            break ;

        case    M4CL_CSEXETYPE_PROXY :
            iResult = "Proxy" ;
            break ;

        case    M4CL_CSEXETYPE_AUTO :
            iResult = "Delta" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClCacheable( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CACHEABLE_FALSE :
            iResult = "False" ;
            break ;

        case    M4CL_CACHEABLE_TRUE :
            iResult = "True" ;
            break ;

        case    M4CL_CACHEABLE_VOLATILE :
            iResult = "Volatile" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_TYPE_METHOD :
            iResult = "Method" ;
            break ;

        case    M4CL_ITEM_TYPE_PROPERTY :
            iResult = "Property" ;
            break ;

        case    M4CL_ITEM_TYPE_FIELD :
            iResult = "Field" ;
            break ;

        case    M4CL_ITEM_TYPE_CONCEPT :
            iResult = "Concept" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}



m4pcchar_t  M4ClLanguageType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_LANGUAGE_TYPE_LOGON :
            iResult = "Logon" ;
            break ;

        case    M4CL_LANGUAGE_TYPE_DEVELOPMENT :
            iResult = "Development" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClNodeCapacity( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_CAPACITY_TYPE_MONOBLOCK :
            iResult = "Monoblock" ;
            break ;

        case    M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK :
            iResult = "Multiblock" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClNodeTemporality( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_TEMPORALITY_EVERGREEN :
            iResult = "Evergreen" ;
            break ;

        case    M4CL_NODE_TEMPORALITY_1_P :
            iResult = "1-p" ;
            break ;

        case    M4CL_NODE_TEMPORALITY_N_P :
            iResult = "n-p" ;
            break ;

        case    M4CL_NODE_TEMPORALITY_N_P_NO_AUTOMATIC :
            iResult = "n-p no automatic" ;
            break ;

        case    M4CL_NODE_TEMPORALITY_N_P_WITH_OVERLAPED :
            iResult = "n-p with overlaped" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClNodeCompleteness( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_COMPLETE_NOT_COMPLETE :
            iResult = "Not complete" ;
            break ;

        case    M4CL_NODE_COMPLETE_COMPLETE_NULL :
            iResult = "Complete Null" ;
            break ;

        case    M4CL_NODE_COMPLETE_COMPLETE_NOT_NULL :
            iResult = "Complete not Null" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClNodeAutoFilter( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_AUTO_FILTER_APPLICATION :
            iResult = "Application" ;
            break ;

        case    M4CL_NODE_AUTO_FILTER_CORRECTION :
            iResult = "Correction" ;
            break ;

        case    M4CL_NODE_AUTO_FILTER_APP_CORR :
            iResult = "Application and Correction" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemSliceBhrv( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_SLICE_BHRV_BASE_VALUE :
            iResult = "Base Value" ;
            break ;

        case    M4CL_ITEM_SLICE_BHRV_FINAL_VALUE :
            iResult = "Final Value" ;
            break ;

        case    M4CL_ITEM_SLICE_BHRV_UNIT :
            iResult = "Unit" ;
            break ;

        case    M4CL_ITEM_SLICE_BHRV_INCIDENCE :
            iResult = "Incidence" ;
            break ;

        case    M4CL_ITEM_SLICE_BHRV_PERIOD_FINAL_VALUE :
            iResult = "Period Final Value" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemSliceSplit( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_SLICE_SPLIT_LINEAR :
            iResult = "Linear" ;
            break ;

        case    M4CL_ITEM_SLICE_SPLIT_NON_LINEAR :
            iResult = "Non Linear" ;
            break ;

        case    M4CL_ITEM_SLICE_SPLIT_NO_SLICES :
            iResult = "No Slices" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClBool( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    1 :
            iResult = "True" ;
            break ;

        case    0 :
            iResult = "False" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClIdTotalize( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_TOTALIZE_NONE :
            iResult = "None" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_COUNT :
            iResult = "Count" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_SUM :
            iResult = "Sum" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_AVG :
            iResult = "Average" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_MAX :
            iResult = "Maximun" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_MIN :
            iResult = "Minimun" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_FIRST :
            iResult = "First" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_LAST :
            iResult = "Last" ;
            break ;

        case    M4CL_ITEM_TOTALIZE_WEIGHTED_SUM :
            iResult = "Weighted Sum" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemTransactionMode( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION :
            iResult = "Need Trancsaction" ;
            break ;

        case    M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION :
            iResult = "Need New Transaction" ;
            break ;

        case    M4CL_ITEM_TRAN_MODE_TRANSACTION_SUPPORTED :
            iResult = "Trancsaction Supported" ;
            break ;

        case    M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED :
            iResult = "Trancsaction No Supported" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemModifyData( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_MODIFY_NONE :
            iResult = "No modify" ;
            break ;

        case    M4CL_ITEM_MODIFY_REGISTER :
            iResult = "Modify Register" ;
            break ;

        case    M4CL_ITEM_MODIFY_BLOCK :
            iResult = "Modify Block" ;
            break ;

        case    M4CL_ITEM_MODIFY_CHILD :
            iResult = "Modify child nodes" ;
            break ;

        case    M4CL_ITEM_MODIFY_M4OBJECT :
            iResult = "Modify m4object" ;
            break ;

        case    M4CL_ITEM_MODIFY_ALL :
            iResult = "Modify all m4objects" ;
            break ;

        case    M4CL_ITEM_MODIFY_LANE :
            iResult = "Modify lane" ;
            break ;
			
        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemInternalType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_INTERNAL_TYPE_NULL :
            iResult = "Null" ;
            break ;

        case    M4CL_INTERNAL_TYPE_NORMAL :
            iResult = "Normal" ;
            break ;

        case    M4CL_INTERNAL_TYPE_START_DATE :
            iResult = "Star Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_END_DATE :
            iResult = "End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_COR_START_DATE :
            iResult = "Correction Start Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_COR_END_DATE :
            iResult = "Correction End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_T_DATE :
            iResult = "Last Update" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PROJECTION :
            iResult = "Projection Flag" ;
            break ;

        case    M4CL_INTERNAL_TYPE_TRANSLATION_PTR :
            iResult = "Translation" ;
            break ;

        case    M4CL_INTERNAL_TYPE_TRANSLATION_FIELD :
            iResult = "Translation Field" ;
            break ;

        case    M4CL_INTERNAL_TYPE_FILTER_START_DATE :
            iResult = "Filter Start Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_FILTER_END_DATE :
            iResult = "Filter End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_FILTER_COR_START_DATE :
            iResult = "Filter Correction Start Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_FILTER_COR_END_DATE :
            iResult = "Filter Correction End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CURRENCY_TYPE :
            iResult = "Currency Type" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IS_CURRENCY :
            iResult = "Is Currency" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE_DATE :
            iResult = "Currency Exchange Rate" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DMD_CHANGE_REASON :
            iResult = "Dmd Change Reason" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DMD_COMPONENT :
            iResult = "DMD Component" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DMD_VALUE :
            iResult = "Dmd Value" ;
            break ;

        case    M4CL_INTERNAL_TYPE_LOAD_SQL :
            iResult = "Load SQL" ;
            break ;

        case    M4CL_INTERNAL_TYPE_VARIANT_TYPE :
            iResult = "Variant Type" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PRIORITY :
            iResult = "Priority" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IS_VARIANT :
            iResult = "Is Variant" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IMPUTE_DATE :
            iResult = "Impute Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAY_DATE :
            iResult = "Pay Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAY_TYPE :
            iResult = "Pay Pay Type" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAY_FREQUENCY :
            iResult = "Pay Pay Frequency" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CHANNEL_ITEM :
            iResult = "Channel Item" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE_TYPE :
            iResult = "Currency Exchange Type" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SECURITY_USER :
            iResult = "Security User" ;
            break ;

        case    M4CL_INTERNAL_TYPE_APP_ROLE :
            iResult = "App Role" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CLOSING_DATE :
            iResult = "Closing date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYS_PARAM :
            iResult = "System Params" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE :
            iResult = "Dynamic Sys Sentence" ;
            break ;

        case    M4CL_INTERNAL_TYPE_EXECUTE_REPORT :
            iResult = "Execute Report" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IMPUTE_PAY_TYPE :
            iResult = "Impute Pay Type" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IMPUTE_PAY_FREQUENCY :
            iResult = "Impute Pay Frequency" ;
            break ;

        case    M4CL_INTERNAL_TYPE_IS_ATTACHMENT :
            iResult = "Is Attachment" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAR_INIT_DATE :
            iResult = "IPar Init Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAR_END_DATE :
            iResult = "Par End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYSTEM_LOAD :
            iResult = "System Load" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYSTEM_PERSIST :
            iResult = "System Persist" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYSTEM_NAVIGATION :
            iResult = "System Navigation" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYSTEM_GENERAL :
            iResult = "System General" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ID_EXECUTION :
            iResult = "Execution Identifier" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ID_ORDER :
            iResult = "Order Identifier" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ID_OPERATION :
            iResult = "Operation Identifier" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ID_TYPE_UPDATE :
            iResult = "Update Type Identifier" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DT_OPERATION :
            iResult = "Operation Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ADD_INFO :
            iResult = "Add Info" ;
            break ;

        case    M4CL_INTERNAL_TYPE_BDL_CHECKS :
            iResult = "BDL Checks" ;
            break ;

        case    M4CL_INTERNAL_TYPE_EXECUTE_REPORT_ODBC :
            iResult = "Execute Report ODBC" ;
            break ;

        case    M4CL_INTERNAL_TYPE_M4OBJ_ALLOCATION_DATE :
            iResult = "Meta4Object Allocation Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_M4OBJ_PAYMENT_DATE :
            iResult = "Meta4Object Payment Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_M4OBJ_CURRENCY :
            iResult = "Meta4Object Currency" ;
            break ;

        case    M4CL_INTERNAL_TYPE_M4OBJ_EXCHANGE_RATE :
            iResult = "Meta4Object Exchange Rate" ;
            break ;

        case    M4CL_INTERNAL_TYPE_M4OBJ_CHANGE_DATE :
            iResult = "Meta4Object Change Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ALLOCATION_START_DATE :
            iResult = "Allocation Start Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ALLOCATION_END_DATE :
            iResult = "Allocation End Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SLICE_MODE :
            iResult = "Slice Mode" ;
            break ;

        case    M4CL_INTERNAL_TYPE_YTD_QUERY :
            iResult = "YTD Query" ;
            break ;

        case    M4CL_INTERNAL_TYPE_RAM_ORDER_BY :
            iResult = "RAM Order By" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE :
            iResult = "Currency Exchange" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ID_ORGANIZATION :
            iResult = "Organization Identifier" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DYNAMIC_SYS_OBJECTS :
            iResult = "Dynamic System Objects" ;
            break ;

        case    M4CL_INTERNAL_TYPE_OPERATION :
            iResult = "Operation Item" ;
            break ;

        case    M4CL_INTERNAL_TYPE_ERASE_ALL_L2_IN_SERVER :
            iResult = "Erase All Level 2 in Server" ;
            break ;

        case    M4CL_INTERNAL_TYPE_NONE_IN_BUFFER_FIELDS :
            iResult = "Buffer Field None" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CURRENCY_IN_BUFFER_FIELD :
            iResult = "Buffer Field Currency" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PAYROLL_DATE :
            iResult = "Payroll Date" ;
            break ;

        case    M4CL_INTERNAL_TYPE_DO_NOT_DELETE :
            iResult = "Do Not Delete" ;
            break ;

        case    M4CL_INTERNAL_TYPE_QUERY_REPORT_NO_VIEW :
            iResult = "No View In Query Report" ;
            break ;

        case    M4CL_INTERNAL_TYPE_PIVOT_VALUE :
            iResult = "Pivot Value" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_NAME :
            iResult = "Crosstab Concept Name" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_VALUE :
            iResult = "Crosstab Concept Value" ;
            break ;

        case    M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_TOTAL :
            iResult = "Crosstab Concept Total" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYS_HINT :
            iResult = "System Hint" ;
            break ;

        case    M4CL_INTERNAL_TYPE_UNUSED_ITEM :
            iResult = "Unused item" ;
            break ;

        case    M4CL_INTERNAL_TYPE_SYSDB_ITEM :
            iResult = "System Database" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClItemConvertFunction( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CONV_FUNCT_NONE :
            iResult = "None" ;
            break ;

        case    M4CL_CONV_FUNCT_UPPER_CASE :
            iResult = "Upper Case" ;
            break ;

        case    M4CL_CONV_FUNCT_LOWER_CASE :
            iResult = "Lower Case" ;
            break ;

        case    M4CL_CONV_FUNCT_FIRST_UPPER_CASE :
            iResult = "First Capital Letter" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClArgumentType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_ARGUMENT_TYPE_INPUT :
            iResult = "Input" ;
            break ;

        case    M4CL_ITEM_ARGUMENT_TYPE_REFERENCE :
            iResult = "Reference" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClConnectedItemPrecedence( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_CONNECTOR_BEFORE :
            iResult = "Before" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_AFTER :
            iResult = "After" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClConnectedItemSpin( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_CONNECTOR_UPWARDS :
            iResult = "Upwards" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_DOWNWARDS :
            iResult = "Downwards" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClConnectedItemRelation( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ITEM_CONNECTOR_EQUALITY :
            iResult = "Equality" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_BIGGER :
            iResult = "Bigger" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_SMALLER :
            iResult = "Smaller" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_BIGGER_OR_EQUAL :
            iResult = "Bigger or Equal" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_SMALLER_OR_EQUAL :
            iResult = "Smaller or Equal" ;
            break ;

        case    M4CL_ITEM_CONNECTOR_DISTINCT :
            iResult = "Distinct" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClConnectedItemContext( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_VM_TRIGGER_ALL :
            iResult = "All" ;
            break ;

        case    M4CL_VM_TRIGGER_INSERTED :
            iResult = "Inserted" ;
            break ;

        case    M4CL_VM_TRIGGER_UPDATEDDB :
            iResult = "Update" ;
            break ;

        case    M4CL_VM_TRIGGER_DELETED :
            iResult = "Deleted" ;
            break ;

        case    M4CL_VM_TRIGGER_CURRENT :
            iResult = "Current" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClSecurity( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_SEC_ENABLE :
            iResult = "Enabled" ;
            break ;

        case    M4CL_SEC_DISABLE :
            iResult = "Disabled" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClFormType ( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
		case    M4CL_NO_LENGUAJE :
			iResult = "No language" ;
			break ;
			
		case    M4CL_LENGUAGE_LN4 :
			iResult = "LN4" ;
			break ;
			
		case    M4CL_LENGUAGE_CPP :
			iResult = "C++" ;
			break ;
			
		case M4CL_INTERN_OBJECT_DLL :
			iResult = "Fix dll Sth" ;
			break ;
    }

    return( iResult ) ;
}


m4pcchar_t  M4ClAutoloadMode( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_AUTOLOAD_OFF :
            iResult = "No Autoload" ;
            break ;

        case    M4CL_AUTOLOAD_BLOCK :
            iResult = "Autoload Block" ;
            break ;

        case    M4CL_AUTOLOAD_PRG :
            iResult = "Autoload Propagate" ;
            break ;

        case    M4CL_AUTOLOAD_NODESAYS :
            iResult = "Node Autoload" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClSyncMode( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4_SYNC_NONE :
            iResult = "None" ;
            break ;

        case    M4_SYNC_DOCUMENT :
            iResult = "Document" ;
            break ;

        case    M4_SYNC_DOCUMENT_PERSIST :
            iResult = "Document Persist" ;
            break ;

        case    M4_SYNC_BLOCK :
            iResult = "Block" ;
            break ;

        case    M4_SYNC_BLOCK_BRANCH :
            iResult = "Block and Children" ;
            break ;

        case    M4_SYNC_REGISTER :
            iResult = "Record" ;
            break ;

        case    M4_SYNC_REGISTER_BRANCH :
            iResult = "Record and Children" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClOpenMode( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_T3ALIAS_OPEN_MODE_NO_LOAD :
            iResult = "No Load" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_MODE_LOAD :
            iResult = "Load" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_BLOCK :
            iResult = "Autoload Block" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_PRG :
            iResult = "Autoload Propagate" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_NODESAYS :
            iResult = "Node Autoload" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClInstanceScope( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED :
            iResult = "New" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED :
            iResult = "Local" ;
            break ;

        case    M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED :
            iResult = "Global" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClInstanceCsAccess( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_CSTYPE_L2_FRONT :
            iResult = "Front" ;
            break ;

        case    M4CL_CSTYPE_L2_BACK :
            iResult = "Back" ;
            break ;

        case    M4CL_CSTYPE_L2_BOTH :
            iResult = "Both" ;
            break ;

        case    M4CL_CSTYPE_L2_SITE :
            iResult = "Site Dependent" ;
            break ;

        case    M4CL_CSTYPE_L2_FATHER :
            iResult = "Father" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}



m4pcchar_t  M4ClNodeDBReload( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_NODE_DB_RELOAD_NONE :
            iResult = "No Load" ;
            break ;

        case    M4CL_NODE_DB_RELOAD_LOAD_BLK :
            iResult = "Load Block" ;
            break ;

        case    M4CL_NODE_DB_RELOAD_LOAD_PRG :
            iResult = "Load Propagate" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}


m4pcchar_t  M4ClInstanceOrganizationType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_ORGANIZATION_L2_TYPE_NONE :
            iResult = "No Organización" ;
            break ;

        case    M4CL_ORGANIZATION_L2_TYPE_MONO :
            iResult = "Mono Organization" ;
            break ;

        case    M4CL_ORGANIZATION_L2_TYPE_MULTI :
            iResult = "Multi Organization" ;
            break ;

        case    M4CL_ORGANIZATION_L2_TYPE_FATHER :
            iResult = "Father Organization" ;
            break ;

        case    M4CL_ORGANIZATION_L2_TYPE_ANY :
            iResult = "Any" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}



m4pcchar_t  M4ClFilterBehaviourType( m4uint8_t ai_iValue )
{

    m4pchar_t   iResult ;


    switch( ai_iValue )
    {
        case    M4CL_FILTER_BEHAVIOUR_QBF :
            iResult = "Simple (QBF)" ;
            break ;

        case    M4CL_FILTER_BEHAVIOUR_MULTI_ALL :
            iResult = "Multiselection and all filter" ;
            break ;

        case    M4CL_FILTER_BEHAVIOUR_ALL :
            iResult = "All filter" ;
            break ;

        default:
            iResult = "Undefined" ;
            break ;

    }

    return( iResult ) ;
}

//## end module%3912A6BD0351.epilog
