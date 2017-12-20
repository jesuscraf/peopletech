
#ifndef __M4JSRES_HPP__
#define __M4JSRES_HPP__


#include <m4res.hpp>

///////////////////////////
//ERRORES DE JOB SCHEDULER
/////////////////////////////

#define M4_ERROR_GET_NAME_PARAMETER                            0x0001  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_INITIALIZE_ENV_ERROR                                0x0002  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_INITIALIZE_CHANNELM_ERROR                           0x0003  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DESTROY_CHANNELM_ERROR                              0x0004  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DESTROY_ENV_ERROR                                   0x0005  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_NAME_META4OBJECT                          0x0006  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_BUILD_CHANNEL_ERROR                                 0x0007  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_LOAD_CHANNEL_ERROR                                  0x0008  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_FIND_JOB_DEFINITION_ERROR                           0x0009  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DESTROY_ACCESS_ERROR                                0x000A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DESTROY_CHANNEL_ERROR                               0x000B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_PUSH_STACK_ERROR                                    0x000C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_FIND_JOB_GROUP_DEF_ERROR                            0x000D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_MOVE_FIRST_POSITION_ERROR                           0x000E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_EXECUTE_METHOD_ERROR                                0x000F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_POINTER_NULL                                        0x0010  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_ACCESS_NULL                                   0x0011  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CHANNEL_NULL                                  0x0012  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_NAME_NODE                                 0x0013  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_WARNING_PARAMETER_INFO                              0x0014  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_POP_STACK_ERROR                                     0x0015  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_VALUE_ERROR                                     0x0016  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ADD_REGISTER_ERROR                                  0x0017  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_SET_VALUE_ERROR                                     0x0018  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_PERSIST_CHANNEL                                     0x0019  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_NUMBER_PARAM_ERROR                                  0x001A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_TYPE_PARM_ERROR                                     0x001B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_CHANGE_CHANNEL_USER_ERROR                           0x001C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_CHANGE_CONNECTION_ERROR                             0x001D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_CHANGE_CONNECTION_ERROR                             0x001D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_LOAD_ALL_META4OBJECTS						   0x001E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_LOAD_ALL_META4OBJECTS                         0x001F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_TYPE_PARAMETER                            0x0020  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_REQUEST_CANCEL_JOB                            0x0021  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_VALUE_PARAMETER						   0x0022  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_REQUEST_ABORT_EXECUTOR                        0x0023  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_REQUEST_ABORT_ENVIROMENT                      0x0024  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_WARNING_OVERWRITE_PARAM_VALUE                       0x0025  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERR_JS_CLEANUP									   0x0026  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_INIT_EXECUTION								   0x0027  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_EXECUTION                                     0x0028  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DELETE_FILE_EXTERNAL                                0x0029  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_FIND_FILE_EXTERNAL                                  0x002A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JSUTILS_DEBUG_INFO								   0x002B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JSUTILS_ERROR_INFO								   0x002C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_META4OBJECT                            0x002D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_ACCESS                                 0x002E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_NODE                                   0x002F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_CREATE_META4OBJECT                            0x0030  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_META4OBJECT_EXEC                       0X0031  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_BUILD_META4OBJECT                             0x0032  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_CREATE_ACCESS                                 0x0033  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_ACCESS_EXEC                            0x0034  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_GET_NODE                                      0x0035  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_NODE_EXEC                              0x0036  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_FILTER_META4OBJECT                            0x0037  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_SET_VALUE_EXEC_ERROR                                0x0038  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_LOAD_META4OBJECT                              0x0039  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_LOAD_CHANNEL_EXEC_ERROR                             0x003A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_GET_JOBDEF_POSITION                           0x003B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_IS_LAST_IN_BOX                                0x003C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_VALUE_EXEC_ERROR                                0x003D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_NULL_VALUE                                      0x003E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_FIND_JGEXEC                                   0x003F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_ADD_NEW_EXECUTION                             0x0040  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_GET_INFORMATION_SECURITY                      0x0041  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_NULL_CREDENTIAL                                     0x0042  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_CHANGE_SECURITY											0x0043  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CHANGE_SECURITY                               0x0044  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_DESTROY_ACCESS_EXEC                           0x0045  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_DESTROY_META4OBJECT_EXEC                      0x0046  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_FINISH_EXECUTION                              0x0047  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PARAMETER_NAME                               0x0048  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_DESTROY_META4OBJECTS                          0x0049  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_FINISH_SUCCESS                                0x004A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_EXECUTE_METHODLN4_EXEC                        0x004B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PUSH_STACK_EXEC                               0x004C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_FIRST_POSITION_FINDJOB                        0x004D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PUSH_STACK_FINDJOB                            0x004E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_EXECUTE_METHODLN4_FINDJOB                     0x004F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_POP_STACK_FINDJOB                             0x0050  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ERROR_FIND_JOBGROUP_DEFINITION                      0x0051  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ERROR_GET_ITEM_FINDJOB										0x0052  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ERROR_SET_ITEM_FINDJOB                              0x0053  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ERROR_PERSIST_META4OBJECT_FINDJOB                   0x0054  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_INTERRUPTED_JOBGROUP                                0x0055  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ABORTED_JOBGROUP                                    0x0056  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_EXPIRED_JOBGROUP                                    0x0057  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_REPITED_JOBGROUP                                    0x0058  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_STATUS_UNKNOWN                                      0x0059  +  M4_SRV_JOB_SCHEDULER_ERROR 
#define M4_ERROR_STATUS_WAITING                                0x005A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_STATUS_EXECUTED                               0x005B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_ITEM_EXECUTEJOB                           0x005C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CLIENT_STRUCTURES                             0x005D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREATE_NODE_EXECUTEJOB                        0x005E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PUSH_STACK_EXECUTEJOB                         0x005F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_POP_STACK_EXECUTEJOB                          0x0060  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CLIENT_RECOVER_EXECUTION                      0x0061  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_CLIENT_ABORTED_JOB												0x0062  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_SET_ITEM_EXECUTEJOB                           0x0063  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_EXECUTE_METHODLN4_EXECUTEJOB                  0x0064  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_TYPE_ERRORLOG                             0x0065  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_CODE_ERRORLOG                             0x0066  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_TEXT_ERRORLOG                             0x0067  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_VALUE_NULL_EXECUTEJOB                         0x0068  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_TYPE_PARAM_UNKNOWN                            0x0069  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PARAM_REPEAT                                  0x006A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_NAME_FILE                                 0x006B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_SIZE_FILE                                 0x006C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_RELEASE_META4OBJECT                           0x006D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_LOAD_META4OBJECT                              0x006E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_FIND_JOBGROUP_RELOAD                          0x006F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_GET_PARAMETER_COMMAND                         0x0070  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_BUILD_META4OBJECT                             0x0071  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_MOVE_NODE_EXECUTEJOB                          0x0072  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CLIENT_RECOVER_LOG_DATE_END                   0x0073  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CREDENTIAL_STRING28                           0x0074  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_FIND_STRING_LOGICAL_CONNECTION                0x0075  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_FREE_CONNECTION                               0x0076  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_SECURITY_STRING_30                            0x0077  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_OPEN_NEW_SESSION                              0x0078  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_TICKET_SESSION                            0x0079  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_DEFAULTCHANNELUSER                        0x007A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_ACCESS_META4OBJECTUSER                        0x007B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PARAMETERS_META4OBJECTUSER                    0x007C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_METADATA_JS                                   0x007D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_SET_CONNECTION                                0x007E  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_FIND_IDENTIFIER_LOGICAL                       0x007F  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_POP_STACK_EXEC											0x0080  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_NO_JOB_IN_BOX											0x0081  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CLIENT_PARAMETERHANDLER								0x0082  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_CLIENT_RECOVER_PARAMETERHANDLER						0x0083  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PARAM_NULL												0x0084  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_PARAM_NAME_NULL											0x0085  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_PARAM_TYPE_NULL											0x0086  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_GET_PARAM_VALUE_NULL											0x0087  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_ADD_PARAMETER_JSREQUEST								0x0088  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_GET_PARAMETER_JSREQUEST								0x0089  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_CLIENT_TIMEOUT_JOB												0x008A  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_JS_SET_TIMEOUT											0x008B  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_ERROR_PARAMETER_TYPE                                         0x008C  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_EXECUTE_METHOD_FUNC_ERROR                                    0x008D  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_DEBUG_ASOCIATED_RESOURCES		                            0x008E  +  M4_SRV_JOB_SCHEDULER_ERROR

#define M4_JS_REC_BUILDING_M4O											0x00C0  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_ACCESSING_NODE											0x00C1  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_PUSHING_PARAMETERS									0x00C2  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_CALLING_METHOD											0x00C3  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_EXECUTING_METHOD										0x00C4  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_POPPING_PARAMETERS									0x00C5  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_BUILDING_INSTANCE										0x00C6  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_BUILDING_INSTANCE_STRUCTURE						0x00C7  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_CREATING_REQUEST										0x00C8  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_PUSHING_REQUEST										0x00C9  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_INVALID_INSTANCE										0x00CA  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_REC_MISSING_PARAMETER										0x00CB  +  M4_SRV_JOB_SCHEDULER_ERROR

#define M4_JS_JOB_INVALID_ITERATOR										0x00E0  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_JOB_PARAMETER_NOT_FOUND									0x00E1  +  M4_SRV_JOB_SCHEDULER_ERROR

#define M4_JSADM_MEMORY  													0x00F0  +  M4_SRV_JOB_SCHEDULER_ERROR //Problemas de memoria. Operacion .... no procesada.
#define M4_JSADM_JOB 														0x00F1  +  M4_SRV_JOB_SCHEDULER_ERROR //Problemas con Jobs en la operación: ....
#define M4_JSADM_JS  														0x00F2  +  M4_SRV_JOB_SCHEDULER_ERROR //Realizando operación: ....  (Begin, End, Shutdown)
#define M4_JSADM_MAP 														0x00F3  +  M4_SRV_JOB_SCHEDULER_ERROR //Operacion: ... (AddExecutor .....)
#define M4_JSADM_INTJOB 													0x00F4  +  M4_SRV_JOB_SCHEDULER_ERROR // Interrupt Job: Tipo interrupcion, Job.
#define M4_JSADM_REQ 														0x00F5  +  M4_SRV_JOB_SCHEDULER_ERROR //Error en Request realizando: 
#define M4_JS_EXEC_COMMAND													0x00F6  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_EXEC_COMMAND_PARAM											0x00F7  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_EXEC_COMMAND_FORMAT_PARAM										0x00F8  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE								0x00F9  +  M4_SRV_JOB_SCHEDULER_ERROR

#define M4_JS_STT_NO_ACTIVATE											0x00FA  +  M4_SRV_JOB_SCHEDULER_ERROR
#define M4_JS_STT_NO_SAVE												0x00FB  +  M4_SRV_JOB_SCHEDULER_ERROR


#define M4_ERROR_GET_EMAIL_RECIPIENTS									0x010E  +  M4_SRV_JOB_SCHEDULER_ERROR

#endif


