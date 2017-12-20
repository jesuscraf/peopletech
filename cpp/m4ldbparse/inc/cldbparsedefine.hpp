//	=============================================================================
//
//	 (c) Copyright  2017 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLdbParseDefine.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				03/03/2017
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//	 Definition: common macro definitions
//
//	 
//	=============================================================================

#define MAX_LENGTH_RECORD 32500
#define MAX_LENGTH 32500
#define EXECUTE_LOAD_TAG "Execute Load."
#define EXECUTE_UPDATE_TAG "Execute Update."
#define EXECUTE_DELETE_TAG "Execute Delete."
#define EXECUTE_INSERT_TAG "Execute Insert."
#define EXECUTE_STMT_TAG "Execute Stmt = "
#define EXECUTE_STMT_SELECT_TAG "Execute Stmt = SELECT"
#define EXECUTE_STMT_UPDATE_TAG "Execute Stmt = UPDATE"
#define EXECUTE_STMT_DELETE_TAG "Execute Stmt = DELETE"
#define EXECUTE_STMT_INSERT_TAG "Execute Stmt = INSERT"
#define ROLLBACK_TAG "#-#----------- ROLLBACK -----------#-#"
#define COMMIT_TAG "#-#----------- COMMIT -----------#-#"
#define M4OBJ_TAG "Meta4Object = "					
#define NODE_TAG "Node = "							
#define DATE_TAG "Date = "
#define TICK_TAG "Tick = "
#define DB_CONN_TAG "DB Connection(" 
#define THREAD_TAG "Thread ID = "					
#define TIME_TAG "Time = "							
#define RECORDS_TAG "Number of records: "
#define NO_DATA_FOUND_TAG "No Data Found"
#define USER_TAG "<<< User"							
#define CONNECTION_TAG ">>>"						
#define CALCULATING_TAG "Calculating Time."
#define ID_ORG_TAG1 "ID_ORGANIZATION ="
#define ID_ORG_TAG2 "ID_ORGANIZATION="
#define ID_ORG_TAG3 "ID_ORGANIZATION (+) ="
#define ID_ORG_TAG4 "ID_ORGANIZATION(+)="
#define ID_ORG_TAG5 "ID_ORGANIZATION (+)="
#define ID_ORG_TAG6 "ID_ORGANIZATION(+) ="
#define ID_ORG_TAG7 "ID_ORGANIZATION IS NOT NULL"
#define	NO_OPERATION 0
#define LOAD_OPERATION 1
#define INSERT_OPERATION 2
#define UPDATE_OPERATION 3
#define DELETE_OPERATION 4
#define STRLEN(s) (sizeof(s)/sizeof(s[0])-1)