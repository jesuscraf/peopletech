#ifndef _APICHN_DF_INCLUDED_
#define _APICHN_DF_INCLUDED_

#define StMIT_APIChn_TaskEnv StENG_APIChn_TaskEnv  

#define ClMIT_MetaChn		 ClENG_MetaChn 
#define ClMIT_ItemExternProp ClENG_ItemExternProp 
#define ClMIT_ChnNode        ClENG_ChnNode
#define ClMIT_Chn            ClENG_Chn
#define ClMIT_ChnItem        ClENG_ChnItem
#define ClMIT_ChnValue       ClENG_ChnValue
#define StMIT_NULLDisplayDef StENG_NULLDisplayDef	
#define ClMIT_MVC            ClENG_MVC 
#define ClMIT_CacheFactory   ClENG_CacheFactory 
#define g_oMITCacheFactory   g_oENGCacheFactory

class ClMIT_ItemExternProp ;
class ClMIT_ChnNode        ;
class ClMIT_Chn            ;
class ClMIT_ChnItem        ;
class ClMIT_EngValue	   ;
struct StMIT_NULLDisplayDef	;
class ClMIT_MetaChn ;
class ClMIT_MVC ;
class ClMIT_CacheFactory ;

#define TM_GET_APICHN_TASK_ENV(TM) (StMIT_APIChn_TaskEnv *) &(TM_GET_TASK_ENV(TM)->m_APIChn)
#define GET_APICHN_TASK_ENV()      (StMIT_APIChn_TaskEnv *) &(TM_GET_TASK_ENV(GET_TM())->m_APIChn)

#ifdef  RPT_ENGINE_TRACE
#define APICHN_TRACE
#endif

#define MIT_TGI_APICHN		 ENG_TGI_APICHN
#define MIT_TGI_CHN_VALUE    ENG_TGI_CHN_VALUE
#define MIT_TGI_METACHN      ENG_TGI_METACHN
#define MIT_TGI_SESSCHN      ENG_TGI_SESSCHN

#define MIT_TGB_APICHN     ENG_TGB_APICHN
#define MIT_TGB_APICHN_EX  ENG_TGB_APICHN_EX
#define MIT_TGB_CHN_VALUE  ENG_TGB_CHN_VALUE
#define MIT_TGB_METACHN    ENG_TGB_METACHN
#define MIT_TGB_SESSCHN    ENG_TGB_SESSCHN

#define MIT_APICHN_MSG_ARRAY		g_stENG_ApiChnMsg
#define MIT_APICHN_MSG_ARRAY_EX		g_stENG_ApiChnMsgEx
#define MIT_CHN_VALUE_MSG_ARRAY		g_stENG_ChnValueMsg
#define MIT_METACHN_MSG_ARRAY		g_stENG_MetaChnMsg 
#define MIT_SESSCHN_MSG_ARRAY		g_stENG_SessChnMsg 


#define MIT_MGI_APICHN     ENG_MGI_APICHN

#endif
