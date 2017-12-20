//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             AdminCommands.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/11/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#ifndef __ADMINCOMMANDS__HPP__
#define __ADMINCOMMANDS__HPP__





// CREATION INSTANCES COMMANDS

#define		CREATE_ACCEPTOR		1
#define		CREATE_CONNMANAGER	2
#define		CREATE_SCHEDULER		3
#define		CREATE_SENDER		4
#define		CREATE_RECEIVER		5
#define		CREATE_DISTRIBUTOR	6
#define		CREATE_LAUNCHER		7
#define		CREATE_SERVICE		8


// DELETE INSTANCES COMMANDS

#define		SHUTDOWN_ACCEPTOR	51
#define		SHUTDOWN_CONNMANAGER	52
#define		SHUTDOWN_SCHEDULER	53
#define		SHUTDOWN_SENDER		54
#define		SHUTDOWN_RECEIVER	55
#define		SHUTDOWN_DISTRIBUTOR	56
#define		SHUTDOWN_LAUNCHER	57
#define		SHUTDOWN_SERVICE		58


// PARAMETERS FOR CEATE SERVICE COMMAND

#define		CREATE_SERVICE_NUMBER		101
#define		NUMBER_BATCH_LAUNCHERS		102
#define		NUMBER_GENERAL_LAUNCHERS	103
#define		MAX_QUEUE_BATCH_ITEMS		104
#define		MAX_QUEUE_GENETAL_ITEMS		104


//PARAMETERS FOR SHUTDOWN SERVICE COMMAND

#define		SHUT_DOWN_SERVICE_NUMBER	201

//PARAMETERS FOR CREATE ACCEPTOR COMMAND


#define		ACCEPT_PORT					250

//PARAMETERS FOR CREATE_RECEIVER COMMAND

#define		RECEIVER_CONTROL_PORT		260

//PARAMETERS FOR CREATE SENDER COMMAND

#define		SENDER_CONTROL_PORT			270

//PARAMETERS FOR CREATE AND SHUT DOWN LAUNCHER COMMANDS

#define		LAUNCHER_SERVICE_NUMBER		281
#define		LAUNCHER_TYPE				282


//PARAMETERS FOR CREATESENDER COMMAND


#endif
