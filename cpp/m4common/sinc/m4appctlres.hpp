//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4appctl
// File:                m4appctlres.hpp   
// Project:             mind4.x      
// Author:              Meta4 Spain, S.A
// Date:                Aug-2000
// Language:            C++
// Operating System:    WINDOWS NT, UNIX
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel
//
//
//==============================================================================


#ifndef __M4APPCTLRES_HPP__
#define __M4APPCTLRES_HPP__


#include <m4res.hpp>

//----------------------------------------------------------------------------
//                         APPLICATION CONTROLER
//----------------------------------------------------------------------------

#define M4_ERR_APPCTL_GENERICMSG				0x0010 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_REGISTE_SERVICE			0x0011 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_COMMAND_LINE_OPTION		0x0012 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_CONFIG_FILE_OPTION		0x0013 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_UNKNOWN_CFG_FILE_PROP		0x0014 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_PARSE_ERROR				0x0015 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_WRONG_VAR_VALUE			0x0016 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_PORT_ERROR				0x0017 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_ACCEPTING_PORT			0x0018 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_NO_COMM_PARAM				0x0019 + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_STOP_LISTENING			0x001A + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_STARTING_PROCESS			0x001B + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_NOCONFIGREGISTERED		0x001C + M4_APPCTL_ERROR_BASE
#define M4_ERR_APPCTL_NOCONFIGFILE				0x001D + M4_APPCTL_ERROR_BASE

#endif
