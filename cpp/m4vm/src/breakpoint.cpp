//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             breakpoint.hpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Execution breakpoint
//
//
//==============================================================================


#include "breakpoint.hpp"
#include "clgeniod.hpp" // To serialize



m4return_t ClBreakpoint::Serialize	(ClGenericIODriver& IOD) const{

	//orden de serialización

	//m_acChannel
	//m_acNode
	//m_acItem
	//m_acRule
	//m_acRuleTI
	//m_iLine
	//m_dStart
	//m_dEnd
	//m_iValue
	//m_bActive

	m4return_t ret;

    if ( (ret = IOD.Write(m_acChannel) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_acNode) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_acItem) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_acRule) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_acRuleTI) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_iLine) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_dStart) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_dEnd) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_iValue) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Write(m_bActive) ) != M4_SUCCESS){
        return ret;
	}
	return M4_SUCCESS;
}

m4return_t ClBreakpoint::DeSerialize	(ClGenericIODriver& IOD){

	//m_acChannel
	//m_acNode
	//m_acItem
	//m_acRule
	//m_acRuleTI
	//m_iLine
	//m_dStart
	//m_dEnd
	//m_iValue
	//m_bActive
	m4return_t ret;

    if ( (ret = IOD.Read(m_acChannel) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_acNode) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_acItem) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_acRule) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_acRuleTI) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_iLine) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_dStart) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_dEnd) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_iValue) ) != M4_SUCCESS){
        return ret;
	}
    if ( (ret = IOD.Read(m_bActive) ) != M4_SUCCESS){
        return ret;
	}
	return M4_SUCCESS;
}
