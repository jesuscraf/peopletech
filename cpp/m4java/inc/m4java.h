//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4jvm.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             29/09/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			
//	Modifications:	
//
// Definition:
//
//    Library API for Java Virtual Machine Interface
//
//
//==============================================================================

#ifndef _M4JAVA_H_
#define _M4JAVA_H_

#include "m4java_dll.hpp"

// Java Native Interface
#include "jni.h"

#include "m4stl.hpp"
#include "m4var.hpp"
#include "m4types.hpp"

// ClMutex
#include "syncro.hpp"

// ClChannelManager
class ClChannelManager;

//=================================================================================
// Lista de variants.
//=================================================================================
typedef list<m4VariantType> VariantList;

//=================================================================================
// Lista de objectos Java.
//=================================================================================
typedef list<jobject> jobjectList_t;

//=================================================================================
// Mapa de objetos.
//=================================================================================
typedef map< m4uint32_t,jobjectList_t *,less <m4uint32_t> > jObjectMap_t;

//=================================================================================
// Lista de objetos Java indexados por M4Object id.
//=================================================================================
static jObjectMap_t s_jobjectMap;



M4_DECL_M4JAVA JNIEnv * M4JVM_GetJavaEnv();

M4_DECL_M4JAVA jint M4JVM_FreeJVM();

M4_DECL_M4JAVA m4int32_t M4JVM_ThreadsCount();

M4_DECL_M4JAVA jint M4JVM_Detach();


M4_DECL_M4JAVA jstring M4JVM_NewJString(JNIEnv * env, const char * ai_pcText, size_t ai_szNChar = -1);

M4_DECL_M4JAVA void M4JVM_ReleaseJString(JNIEnv * env, jstring jText);

M4_DECL_M4JAVA char * M4JVM_GetString(JNIEnv * env, jstring ai_jString);

M4_DECL_M4JAVA m4bool_t M4JVM_CheckExceptions();
M4_DECL_M4JAVA m4return_t M4JVM_CreateObject(m4uint32_t ai_uiM4OHandle, vector<string> ai_vIdJar, vector<string> ai_vIdJarVersionList, vector<string> ai_vJarFileList, const string & ai_sClassName, const VariantList & ai_oParamList, jobject & ao_jNewObject);
M4_DECL_M4JAVA m4return_t M4JVM_InvokeMethod(m4uint32_t ai_uiM4OHandle, jobject ai_joObject, const string & ai_sMethodName, const VariantList & ai_oMethodParameters, m4VariantType & ao_oVariant);
M4_DECL_M4JAVA m4return_t M4JVM_DestroyObject(m4uint32_t ai_uiM4OHandle, jobject ai_joObject);
M4_DECL_M4JAVA m4return_t M4JVM_DestroyAllObjects(m4uint32_t ai_uiM4OHandle);

m4void_t _GetExceptionMessage(JNIEnv * ai_poJNIEnv, jthrowable ai_jsException, string & ao_sExceptionMessage);
m4void_t _GetExceptionCause(JNIEnv * ai_poJNIEnv, jthrowable ai_jsException, string & ao_sExceptionCause);
m4void_t _GetExceptionStackTrace(JNIEnv * ai_poJNIEnv, jthrowable ai_jsException, string & ao_sExceptionMessage);
m4return_t _CreateJarInfoArray(vector<string> ai_vIdJar, vector<string> ai_vIdJarVersionList, vector<string> ai_oJarFileList, jobjectArray & ao_joJarInfoList);
m4return_t _ConvertM4VariantToJNI(m4uint32_t ai_uiM4OHandle, VariantList ai_oParameters, jobjectArray & ao_joParameters);
m4return_t _ConvertJNIToM4Variant(m4uint32_t ai_uiM4OHandle, jobject ai_jObject, m4VariantType & ao_oVariant);
m4return_t _ReleaseJArrayElement(jobject ai_jObject, m4bool_t ai_bObjectSaved);

m4return_t _MakeJDouble(m4double_t ai_dDouble, jobject & ao_joDouble);
m4return_t _MakeJString(m4pcchar_t ai_pcString, jobject & ao_joString);
m4return_t _MakeJDate(m4double_t ai_dJulianDate, jobject & ao_joCalendar);

m4return_t _MakeDouble(jobject ai_jDouble, m4double_t & ao_dDouble);
m4return_t _MakeString(jobject ai_joString, m4char_t * & ao_pString);
m4return_t _MakeDate(jobject aio_joCalendar, m4double_t & ao_dJulianDate);

m4return_t _AddJObject(m4uint32_t ai_uiM4OHandle, jobject ai_jObject);
m4return_t _DeleteJObject(JNIEnv * ai_poJNIEnv, m4uint32_t ai_uiM4OHandle, jobject ai_jObject);
m4return_t _GetJObjectList(m4uint32_t ai_uiM4OHandle, jobjectList_t *& ao_pjObjectList);
m4return_t _FindJObject(jobjectList_t * ai_pjObjectList, jobject ai_jObject, m4bool_t & ao_bFound);
m4return_t _FindJObject(m4uint32_t ai_uiM4OHandle, jobject ai_jObject, m4bool_t & ao_bFound);


#endif
