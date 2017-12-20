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

#include "m4java.h"
#include "m4jvm.h"
#include "m4unicode.hpp"
#include "m4javares.hpp"

// m4date
#include "m4date.hpp"

// log
#include "chlog.hpp"

// Bug 0155054. Inicialización del mutex.
static ClMutex s_oObjMutex(M4_TRUE);


JNIEnv * M4JVM_GetJavaEnv()
{
	return M4JVM::GetJavaEnv();
}

jint M4JVM_FreeJVM()
{
	return M4JVM::FreeJVM();
}

m4int32_t M4JVM_ThreadsCount()
{
	return M4JVM::ThreadsCount();
}

jstring M4JVM_NewJString(JNIEnv * env, const char * ai_pcText, size_t ai_szNChar)
{
	jchar * TextUTF16 = M4CppToUtf16(ai_pcText, (int&) ai_szNChar);
	jstring jText = env->NewString(TextUTF16, ai_szNChar);
	delete [] TextUTF16;

	return jText;
}

void M4JVM_ReleaseJString(JNIEnv * env, jstring jText)
{
	env->DeleteLocalRef(jText);
}


/****************************************************************************
*	Name:			M4JVM_GetString
*	Description:	This function obtains pointer to the contents of a 
*					jstring.
*
*	Parameters:
*  	Input:			ai_joEnv:		the JNIEnv interface pointer.
*					ai_jString:		string object whose elements are to
*									be accessed.
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4pchar_t M4JVM_GetString(JNIEnv * ai_joEnv, jstring ai_jString)
{
	const jchar *	pcUtf16String = NULL;
	m4pchar_t		pcString = NULL;
	m4int_t			iLength = -1;
	jboolean		bIsCopy = M4_FALSE;

	if (ai_jString == NULL)
	{
		return NULL;
	}

	pcUtf16String = ai_joEnv->GetStringChars(ai_jString, &bIsCopy);
	pcString = M4Utf16ToCpp((M4XMLCh*)pcUtf16String, iLength);

	ai_joEnv->ReleaseStringChars(ai_jString, pcUtf16String);
	
	return pcString;
}

/****************************************************************************
*	Name:			M4JVM_CreateObject
*	Description:	This function creates a Java object from a given class.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle:			M4Object handle.
*					ai_oIdJar:				Jar id. list.
*					ai_oIdJarVersionList:	Jar id. version list.
*					ai_oJarFileList:		Jar file list.
*					ai_sClassName:			Class name.
*					ai_oParamList:			Class constructor parameter list [optional].
*					ao_jNewObject:			New Java object created.
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t M4JVM_CreateObject(m4uint32_t ai_uiM4OHandle, vector<string> ai_vIdJar, vector<string> ai_vIdJarVersionList, vector<string> ai_vJarFileList, const string & ai_sClassName, const VariantList & ai_oParamList, jobject & ao_jNewObject)
{
	jclass					jJNIClass = NULL;
	jmethodID				jCreateObjectMethod = NULL;
	jstring					jsClassName = NULL;
	jstring					jsM4OInstanceId = NULL;
	char*					pcM4OInstanceId = NULL;
	jobjectArray			joParameters = NULL;
	jobjectArray			joJarInfo = NULL;
	
	// Inicialización.
	ao_jNewObject = NULL;

    // Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener clase Java M4JNI.
	jJNIClass = poJNIEnv->FindClass("com/meta4/jni/M4JNI");
    if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	if (jJNIClass == NULL)
    {
        return M4_ERROR;
    }

	// Obtener método "CreateObject" de la clase M4JNI para crear un objeto Java.
    jCreateObjectMethod = poJNIEnv->GetStaticMethodID(jJNIClass, "createObject", "(Ljava/lang/String;Ljava/lang/String;[Lcom/meta4/jni/M4JarInfo;[Ljava/lang/Object;)Ljava/lang/Object;");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}

	if (jCreateObjectMethod == NULL)
    {
		poJNIEnv->DeleteLocalRef(jJNIClass);
        return M4_ERROR;
    }

	// convert the m4o instance id to a string
	pcM4OInstanceId = new char[32];
	sprintf(pcM4OInstanceId,"%ud", ai_uiM4OHandle);
	jsM4OInstanceId = M4JVM_NewJString(poJNIEnv, pcM4OInstanceId);
	if (jsM4OInstanceId == NULL)
	{
		return M4_ERROR;
	}

	// Crear un array de M4JarInfo.
	if (_CreateJarInfoArray(ai_vIdJar, ai_vIdJarVersionList, ai_vJarFileList, joJarInfo) != M4_SUCCESS)
	{
		poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}

	// Convertir los parámetros del constructor de la clase de m4variants a tipos JNI.
	if (_ConvertM4VariantToJNI(ai_uiM4OHandle, ai_oParamList, joParameters) != M4_SUCCESS)
	{
		poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
		poJNIEnv->DeleteLocalRef(jJNIClass);
		poJNIEnv->DeleteLocalRef(joJarInfo);
		return M4_ERROR;
	}

	// Obtener el nombre de la clase que se quiere invocar.
	jsClassName = M4JVM_NewJString(poJNIEnv, ai_sClassName.c_str());
	if (jsClassName == NULL)
	{
		poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
		poJNIEnv->DeleteLocalRef(jJNIClass);
		poJNIEnv->DeleteLocalRef(joJarInfo);
		poJNIEnv->DeleteLocalRef(joParameters);
		return M4_ERROR;
	}

	// Ejecutar el método "createObject" de la clase M4JNI.
	ao_jNewObject = poJNIEnv->CallStaticObjectMethod(jJNIClass, jCreateObjectMethod, jsM4OInstanceId, jsClassName, joJarInfo, joParameters);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
		poJNIEnv->DeleteLocalRef(jJNIClass);
		poJNIEnv->DeleteLocalRef(joJarInfo);
		poJNIEnv->DeleteLocalRef(joParameters);
		poJNIEnv->DeleteLocalRef(jsClassName);
		return M4_ERROR;
	}

	// Añadir el jobject previamente creado.
	if (_AddJObject(ai_uiM4OHandle, ao_jNewObject) != M4_SUCCESS)
	{
		poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
		poJNIEnv->DeleteLocalRef(jJNIClass);
		poJNIEnv->DeleteLocalRef(joJarInfo);
		poJNIEnv->DeleteLocalRef(joParameters);
		poJNIEnv->DeleteLocalRef(jsClassName);
		return M4_ERROR;
	}
	
	// Liberar recursos.
	poJNIEnv->DeleteLocalRef(jsM4OInstanceId);
	poJNIEnv->DeleteLocalRef(jJNIClass);
	poJNIEnv->DeleteLocalRef(joJarInfo);
	poJNIEnv->DeleteLocalRef(joParameters);
	poJNIEnv->DeleteLocalRef(jsClassName);
	

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			M4JVM_InvokeMethod
*	Description:	This function invokes a method from a given Java object.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle:			M4Object handle.
*					ai_joObject:			Java object handle.	
*					ai_sMethodName:			Method name.
*					ai_oMethodParameters:	Method parameters [optional].
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t M4JVM_InvokeMethod(m4uint32_t ai_uiM4OHandle, jobject ai_joObject, const string & ai_sMethodName, const VariantList & ai_oMethodParameters, m4VariantType & ao_oVariant)
{
	jclass			jJNIClass = NULL;
	jmethodID		jInvokeMethod = NULL;
	jstring			jsMethodName = NULL;
	jobjectArray	joParameters = NULL;
	jobject			joReturnObject = NULL;
	m4bool_t		bObjectFound = M4_FALSE;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

    // Obtener clase M4JNI.
    jJNIClass = poJNIEnv->FindClass("com/meta4/jni/M4JNI");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}

	if (jJNIClass == NULL)
    {
        return M4_ERROR;
    }
	
	// Obtener método "invokeObject".
    jInvokeMethod = poJNIEnv->GetStaticMethodID(jJNIClass, "invokeMethod", "(Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}

	if (jInvokeMethod == NULL)
    {
		poJNIEnv->DeleteLocalRef(jJNIClass);
        return M4_ERROR;
    }

	// Verificar el objeto Java sobre el que se debe ejecutar el método debe haber
	// sido previamente creado.
	if (_FindJObject(ai_uiM4OHandle, ai_joObject, bObjectFound) != M4_SUCCESS)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}

	if (bObjectFound == M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}
	
	// Convertir los parámetros del método de m4variants a tipos JNI.
	if (_ConvertM4VariantToJNI(ai_uiM4OHandle, ai_oMethodParameters, joParameters) != M4_SUCCESS)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}
	
	// Obtener el nombre del método que se quiere invocar.
	jsMethodName = M4JVM_NewJString(poJNIEnv, ai_sMethodName.c_str());
	if (jsMethodName == NULL)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		return M4_ERROR;
	}

	// Ejecutar método InvokeMethod de la clase M4JNI.
	joReturnObject = poJNIEnv->CallStaticObjectMethod(jJNIClass, jInvokeMethod, ai_joObject, jsMethodName, joParameters);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIClass);
		poJNIEnv->DeleteLocalRef(jsMethodName);
		poJNIEnv->DeleteLocalRef(joParameters);
		return M4_ERROR;
	}

	poJNIEnv->DeleteLocalRef(jJNIClass);
	poJNIEnv->DeleteLocalRef(jsMethodName);
	poJNIEnv->DeleteLocalRef(joParameters);

	// Convertir el resultado de la ejecución del método tipos JNI a m4variant.
	if (_ConvertJNIToM4Variant(ai_uiM4OHandle, joReturnObject, ao_oVariant) != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			M4JVM_DestroyObject
*	Description:	This function release a given Java object from a M4Object.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle:	M4Object handle.
*					ai_joObject:	Java object handle.
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t M4JVM_DestroyObject(m4uint32_t ai_uiM4OHandle, jobject ai_joObject)
{
	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}
	
	// Liberar el objeto java.
	if (_DeleteJObject(poJNIEnv, ai_uiM4OHandle, ai_joObject) != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			M4JVM_DestroyAllObjects
*	Description:	This function releases all Java objects associated to a 
*					concrete M4Object.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle:	M4Object handle.
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t M4JVM_DestroyAllObjects(m4uint32_t ai_uiM4OHandle)
{
	jObjectMap_t::iterator	m4objectIterator;
	jobjectList_t::iterator oIterator;
	jobjectList_t *			poObjectList = NULL;
	jobject					joObject = NULL;
	m4pcchar_t				pccServer = NULL;
	jclass					jJNIClass = NULL;
	
	// Si la lista está vacía, ya no se continúa.
	s_oObjMutex.Lock();
	if (s_jobjectMap.empty() == M4_TRUE)
	{
		s_oObjMutex.Unlock();
		return M4_SUCCESS;
	}
	
	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
	if (poJNIEnv == NULL)
	{
		s_oObjMutex.Unlock();
		return M4_ERROR;
	}

	// Buscar el M4Object en el mapa.
	m4objectIterator = s_jobjectMap.find(ai_uiM4OHandle);
	if (m4objectIterator != s_jobjectMap.end())
	{
		poObjectList = (*m4objectIterator).second;
		if (poObjectList != NULL)
		{
			for (oIterator = poObjectList->begin(); oIterator != poObjectList->end(); oIterator++)
			{
				// Liberar el objeto Java.
				joObject = *oIterator;
				if (joObject != NULL)
				{
					poJNIEnv->DeleteLocalRef(joObject);
				}
			}
			
			poObjectList->clear();
			delete poObjectList;
			poObjectList = NULL;
		}
		
		s_jobjectMap.erase(m4objectIterator);
	}

	s_oObjMutex.Unlock();

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			M4JVM_CheckExceptions
*	Description:	This function checks Java exceptions.
*
*	Parameters:
*  	Input:
*
* 	Output:
*
*	Return:			0	No error.
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t M4JVM_CheckExceptions()
{
	// JNI Exception handling.
	jthrowable		jsException = NULL;
	m4bool_t		bExceptions = M4_FALSE;
	string			sExceptionMessage = "";
	
	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv != NULL)
	{
		jsException = poJNIEnv->ExceptionOccurred();
		if (jsException != NULL)
		{
			poJNIEnv->ExceptionDescribe();
			poJNIEnv->ExceptionClear();
			bExceptions = M4_TRUE;
			
			_GetExceptionStackTrace(poJNIEnv, jsException, sExceptionMessage);
			if (sExceptionMessage.empty() == M4_FALSE)
			{
				DUMP_CHLOG_ERRORF(M4JAVAAPI_ERROR_JNI_EXCEPTION, sExceptionMessage.c_str());
			}
		}
	}

	return bExceptions;
}

/****************************************************************************
*	Name:			_GetExceptionStackTrace
*	Description:	This function creates a M4JarInfo object array.
*
*	Parameters:
*  	Input:			ai_poJNIEnv:	JVM environment.
*					ai_jsException:	exception object (java.lang.throwable).
*					ao_sExceptionMessage: exception message.
* 	Output:		
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4void_t _GetExceptionStackTrace(JNIEnv * ai_poJNIEnv, jthrowable ai_jsException, string & ao_sExceptionMessage)
{
	jclass			jM4JNIUtilsClass = NULL;
	jclass			jThrowableClass = NULL;
	jmethodID		jGetStackTraceMethodID = NULL;
	jstring			jExceptionMessage = NULL;
	m4pchar_t		pExceptionMessage = NULL;
	
	// Inicializaciones.
	ao_sExceptionMessage = "";

	if (ai_jsException == NULL)
	{
		return;
	}

	jM4JNIUtilsClass = ai_poJNIEnv->FindClass("com/meta4/jni/M4JNIUtils");
	if (jM4JNIUtilsClass == NULL)
	{
		return;
	}
	
	jThrowableClass = ai_poJNIEnv->FindClass("java/lang/Throwable");
	if (jThrowableClass == NULL)
	{
		return;
	}

	jGetStackTraceMethodID = ai_poJNIEnv->GetStaticMethodID(jM4JNIUtilsClass, "getStackTrace", "(Ljava/lang/Throwable;)Ljava/lang/String;");
	if (jGetStackTraceMethodID == NULL)
	{
		ai_poJNIEnv->DeleteLocalRef(jM4JNIUtilsClass);
		ai_poJNIEnv->DeleteLocalRef(jThrowableClass);
		return;
		
	}
	
	ai_poJNIEnv->DeleteLocalRef(jM4JNIUtilsClass);
	
	jExceptionMessage = (jstring) ai_poJNIEnv->CallStaticObjectMethod(jM4JNIUtilsClass, jGetStackTraceMethodID, ai_jsException);
	if (jExceptionMessage == NULL)
	{
		ai_poJNIEnv->DeleteLocalRef(jThrowableClass);
		return;
	}
	
	ai_poJNIEnv->DeleteLocalRef(jThrowableClass);
	
	// Devolver mensaje.
	pExceptionMessage = M4JVM_GetString(ai_poJNIEnv, jExceptionMessage);
	if (pExceptionMessage == NULL)
	{
		return;
	}
	
	ao_sExceptionMessage = pExceptionMessage;
	delete [] pExceptionMessage;
	
	return;
}


/****************************************************************************
*	Name:			_CreateJarInfoArray
*	Description:	This function creates a M4JarInfo object array.
*
*	Parameters:
*  	Input:			ai_vIdJar:				Jar id. list.
*					ai_vIdJarVersionList:	Jar id. version list.
*					ai_vJarFileList:		Jar file list.
*					ao_joJarInfoList:		M4JarInfo object array.	
* 	Output:		
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _CreateJarInfoArray(vector<string> ai_vIdJar, vector<string> ai_vIdJarVersionList, vector<string> ai_vJarFileList, jobjectArray & ao_joJarInfoList)
{
	jclass					jM4JarInfoClass = NULL;
	jmethodID				jConstructorId = NULL;
	m4int32_t				iJarNum = 0;
	m4int32_t				iPos = 0;
	jobject					jObject = NULL;
	string					sIdJar;
	string					sIdJarVersion;
	string					sIdJarFilePath;
	jstring					jsIdJar;
	jstring					jsIdJarVersion;
	jstring					jsIdJarFilePath;

	// Inicializaciones.
	ao_joJarInfoList = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener clase Java Object.
	jM4JarInfoClass = poJNIEnv->FindClass("com/meta4/jni/M4JarInfo");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	// Recuperar el constructor de la clase M4JarInfo.
	jConstructorId = poJNIEnv->GetMethodID(jM4JarInfoClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jM4JarInfoClass);
		return M4_ERROR;
	}
	
	if (jConstructorId == NULL)
	{
		poJNIEnv->DeleteLocalRef(jM4JarInfoClass);
		return M4_ERROR;
	}

	// Crear un jobjectArray para la lista de parámetros.
	iJarNum = ai_vIdJar.size();
	ao_joJarInfoList = poJNIEnv->NewObjectArray(iJarNum, jM4JarInfoClass, NULL);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jM4JarInfoClass);
		return M4_ERROR;
	}

	// Rellenar el jobjectArray.
	for (iPos = 0; iPos < iJarNum; iPos++)
	{
		// Crear el objeto M4JarInfo.
		sIdJar = ai_vIdJar[iPos];
		sIdJarVersion = ai_vIdJarVersionList[iPos];
		sIdJarFilePath = ai_vJarFileList[iPos];

		jsIdJar = M4JVM_NewJString(poJNIEnv, sIdJar.c_str());
		jsIdJarVersion = M4JVM_NewJString(poJNIEnv, sIdJarVersion.c_str());
		jsIdJarFilePath = M4JVM_NewJString(poJNIEnv, sIdJarFilePath.c_str());

		jObject = poJNIEnv->NewObject(jM4JarInfoClass, jConstructorId, jsIdJar, jsIdJarVersion, jsIdJarFilePath);
		
		poJNIEnv->DeleteLocalRef(jsIdJar);
		poJNIEnv->DeleteLocalRef(jsIdJarVersion);
		poJNIEnv->DeleteLocalRef(jsIdJarFilePath);

		if (M4JVM_CheckExceptions() != M4_FALSE)
		{	
			poJNIEnv->DeleteLocalRef(jM4JarInfoClass);
			poJNIEnv->DeleteLocalRef(jObject);
			return M4_ERROR;
		}

		poJNIEnv->SetObjectArrayElement(ao_joJarInfoList, iPos, jObject);
		if (M4JVM_CheckExceptions() != M4_FALSE)
		{
			poJNIEnv->DeleteLocalRef(jM4JarInfoClass);
			poJNIEnv->DeleteLocalRef(jObject);
			return M4_ERROR;
		}

		poJNIEnv->DeleteLocalRef(jObject);
	}

	poJNIEnv->DeleteLocalRef(jM4JarInfoClass);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_ConvertM4VariantToJNI
*	Description:	This function convert a list of parameters of variant type
*					to the corresponding JNI types.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle: M4Object handle.
*					ai_oParameters: m4variant parameter list.
*
* 	Output:			jobjectArray:	JNI object array.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _ConvertM4VariantToJNI(m4uint32_t ai_uiM4OHandle, VariantList ai_oParameters, jobjectArray & ao_joParameters)
{
	jclass					jObjectClass = NULL;
	VariantList::iterator	it;
	m4VariantType			vParam;
	m4int32_t				iPos = 0;
	m4int32_t				iParamNum = 0;
	VariantList				vDateValue;
	jobject					joParameter = NULL;
	jobject					jObject = NULL;
	m4bool_t				bObjectFound = M4_FALSE;
	
	// Inicializaciones.
	ao_joParameters = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener clase Java Object.
	jObjectClass = poJNIEnv->FindClass("java/lang/Object");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	// Crear un jobjectArray para la lista de parámetros.
	iParamNum = ai_oParameters.size();
	ao_joParameters = poJNIEnv->NewObjectArray(iParamNum, jObjectClass, NULL);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jObjectClass);
		return M4_ERROR;
	}

	poJNIEnv->DeleteLocalRef(jObjectClass);
	
	// Rellenar el jobjectArray.
	for (it = ai_oParameters.begin(); it != ai_oParameters.end(); it++)
	{
		vParam = (*it);
		switch(vParam.Type)
		{
			case M4CL_CPP_TYPE_STRING_VAR:
				if (_MakeJString(vParam.Data.PointerChar, joParameter) != M4_SUCCESS)
				{
					_ReleaseJArrayElement(joParameter, bObjectFound);
					return M4_ERROR;
				}
				break;
			case M4CL_CPP_TYPE_NUMBER:

				// Verificar si el double que se manda es el handle de un objeto
				// Java previamente creado.
				jObject = (jobject)(jint)vParam.Data.DoubleData;
				if (_FindJObject(ai_uiM4OHandle, jObject, bObjectFound) != M4_SUCCESS)
				{
					_ReleaseJArrayElement(joParameter, bObjectFound);
					return M4_ERROR;
				}

				// Si es un objeto Java válido, se manda en el array de argumentos
				// del método como un jobject.
				// Si no es un objeto Java válido, se asume que es un número tal cual.
				if (bObjectFound == M4_TRUE)
				{
					joParameter = jObject;
				}
				else
				{
					if (_MakeJDouble(vParam.Data.DoubleData, joParameter) != M4_SUCCESS)
					{
						_ReleaseJArrayElement(joParameter, bObjectFound);
						return M4_ERROR;
					}
				}
				break;
			case M4CL_CPP_TYPE_DATE:
				if (_MakeJDate(vParam.Data.DoubleData, joParameter) != M4_SUCCESS)
				{
					_ReleaseJArrayElement(joParameter, bObjectFound);
					return M4_ERROR;
				}
				break;
			case M4CL_CPP_TYPE_NULL:
				joParameter = NULL;
				break;
			default:
				// Error.
				_ReleaseJArrayElement(joParameter, bObjectFound);
				return M4_ERROR;
		}
		
		poJNIEnv->SetObjectArrayElement(ao_joParameters, iPos, joParameter);
		if (M4JVM_CheckExceptions() != M4_FALSE)
		{
			// Liberar referencias. Sólo en el caso de que no sea un objeto Java previamente creado.
			_ReleaseJArrayElement(joParameter, bObjectFound);
			return M4_ERROR;
		}
		
		// Liberar referencias. Sólo en el caso de que no sea un objeto Java previamente creado.
		_ReleaseJArrayElement(joParameter, bObjectFound);
		iPos++;
	}

	return M4_SUCCESS;	
}

/****************************************************************************
*	Name:			_ReleaseJArrayElement
*	Description:	This function release a given jobject array element.
*
*	Parameters:
*  	Input:			ai_jObject: jobject to release.
*					ai_bObjectSaved: is java object previously saved.
*
* 	Output:			
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _ReleaseJArrayElement(jobject ai_jObject, m4bool_t ai_bObjectSaved)
{
	jclass	jStringClass = NULL;

	// Comprobar que el jobject no es nulo.
	if (ai_jObject == NULL)
	{
		return M4_SUCCESS;
	}

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	jStringClass = poJNIEnv->FindClass("java/lang/String");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	// Comprobar si es un string.
	if (ai_bObjectSaved == M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(ai_jObject);
	}

	poJNIEnv->DeleteLocalRef(jStringClass);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_ConvertJNIToM4Variant
*	Description:	This function convert a JNI object 
*					to m4variant object.
*
*	Parameters:		
*  	Input:			ai_uiM4OHandle: M4Object handle.
*					ai_jObject:		JNI object.
*
* 	Output:			aio_oVariant:	m4variant object.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _ConvertJNIToM4Variant(m4uint32_t ai_uiM4OHandle, jobject ai_jObject, m4VariantType & ao_oVariant)
{
	jclass			jDoubleClass = NULL;
	jclass			jStringClass = NULL;
	jclass			jDateClass = NULL;
	jclass			jCalendarClass = NULL;
	m4bool_t		bIsObject = M4_FALSE;

	// Inicializaciones.
	ao_oVariant.Type = M4CL_CPP_TYPE_NULL;

	// Verificar que el jobject no es nulo.
	if (ai_jObject == NULL)
	{
		return M4_SUCCESS;
	}

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener las clases Java que se manejan.
	jDoubleClass = poJNIEnv->FindClass("java/lang/Double");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
		
	jStringClass = poJNIEnv->FindClass("java/lang/String");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jDoubleClass);
		return M4_ERROR;
	}
		
	jCalendarClass = poJNIEnv->FindClass("java/util/Calendar");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jDoubleClass);
		poJNIEnv->DeleteLocalRef(jStringClass);
		return M4_ERROR;
	}

	// Averiguar el tipo del objecto JNI y construir el m4variant correspondiente.
	if (((m4bool_t)poJNIEnv->IsInstanceOf(ai_jObject, jDoubleClass)) == M4_TRUE)
	{
		ao_oVariant.Type = M4CL_CPP_TYPE_NUMBER;
		if (_MakeDouble(ai_jObject, ao_oVariant.Data.DoubleData) != M4_SUCCESS)
		{
			poJNIEnv->DeleteLocalRef(jDoubleClass);
			poJNIEnv->DeleteLocalRef(jStringClass);
			poJNIEnv->DeleteLocalRef(jCalendarClass);
			return M4_ERROR;
		}
	}
	else if (((m4bool_t)poJNIEnv->IsInstanceOf(ai_jObject, jStringClass)) == M4_TRUE)
	{
		ao_oVariant.Type = M4CL_CPP_TYPE_STRING_VAR;
		if (_MakeString(ai_jObject, ao_oVariant.Data.PointerChar) != M4_SUCCESS)
		{
			poJNIEnv->DeleteLocalRef(jDoubleClass);
			poJNIEnv->DeleteLocalRef(jStringClass);
			poJNIEnv->DeleteLocalRef(jCalendarClass);
			return M4_ERROR;
		}
	}
	else if (((m4bool_t)poJNIEnv->IsInstanceOf(ai_jObject, jCalendarClass)) == M4_TRUE)
	{
		ao_oVariant.Type = M4CL_CPP_TYPE_DATE;
		if (_MakeDate(ai_jObject, ao_oVariant.Data.DoubleData) != M4_SUCCESS)
		{
			poJNIEnv->DeleteLocalRef(jDoubleClass);
			poJNIEnv->DeleteLocalRef(jStringClass);
			poJNIEnv->DeleteLocalRef(jCalendarClass);
			return M4_ERROR;
		}
	}
	else
	{
		// Es un un objeto de la clase Object. Se devuelve el handle al objeto
		// como un double.
		ao_oVariant.Type = M4CL_CPP_TYPE_NUMBER;
		ao_oVariant.Data.DoubleData = (jint) ai_jObject;
		bIsObject = M4_TRUE;
	}

	poJNIEnv->DeleteLocalRef(jDoubleClass);
	poJNIEnv->DeleteLocalRef(jStringClass);
	poJNIEnv->DeleteLocalRef(jCalendarClass);

	// Liberar el objeto devuelto por Java, sólo en el caso de que sea un Double, 
	// String o Calendar.
	if (bIsObject == M4_TRUE)
	{
		// Añadir el jobject previamente creado.
		if (_AddJObject(ai_uiM4OHandle, ai_jObject) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	else
	{
		poJNIEnv->DeleteLocalRef(ai_jObject);
	}

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_MakeJDouble
*	Description:	This function builds a Java Double object from a C double.
*
*	Parameters:		
*  	Input:			ai_dDouble:		double value.
*
* 	Output:			ao_joDouble:	Double object.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeJDouble(m4double_t ai_dDouble, jobject & ao_joDouble)
{
	jclass		jDoubleClass = NULL;
	jmethodID	jDoubleConstructor = NULL;
	
	// Inicializaciones.
	ao_joDouble = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	jDoubleClass = poJNIEnv->FindClass("java/lang/Double");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	jDoubleConstructor = poJNIEnv->GetMethodID(jDoubleClass, "<init>", "(D)V");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jDoubleClass);
		return M4_ERROR;
	}
	
	ao_joDouble = poJNIEnv->NewObject(jDoubleClass, jDoubleConstructor, (jdouble)ai_dDouble);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jDoubleClass);
		return M4_ERROR;
	}

	poJNIEnv->DeleteLocalRef(jDoubleClass);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_MakeJString
*	Description:	This functions takes a C string with the UTF-8 format 
*					and constructs a java.lang.String instance. 
*					The newly constructed java.lang.String instance represents 
*					the same sequence of Unicode characters as the given 
*					UTF-8 C string. 
*
*
*	Parameters:		
*  	Input:			ai_sString:		string value.
*
* 	Output:			ao_joString:	String object.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeJString(m4pcchar_t ai_pcString, jobject & ao_joString)
{
	// Inicializaciones.
	ao_joString = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener el jstring a partir del C string.
	ao_joString = M4JVM_NewJString(poJNIEnv, ai_pcString);
	if (ao_joString == NULL)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_MakeJDate
*	Description:	This function builds a Java Date object from a julian date.
*
*	Parameters:		
*  	Input:			ai_dJulianDate:		julian date.
*
* 	Output:			ao_joCalendar:		Calendar object.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeJDate(m4double_t ai_dJulianDate, jobject & ao_joCalendar)
{
	jclass			jJNIUtilsClass = NULL;
	jmethodID		jStringToCalendarMethod = NULL;
	M4ClTimeStamp	oDate;
	M4ClString		sDate;
	jstring			jsDate = NULL;

	// Inicializaciones.
	ao_joCalendar = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener la clase M4JNIUtils.
	jJNIUtilsClass = poJNIEnv->FindClass("com/meta4/jni/M4JNIUtils");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	// Obtener método "stringToCalendar".
    jStringToCalendarMethod = poJNIEnv->GetStaticMethodID(jJNIUtilsClass, "stringToCalendar", "(Ljava/lang/String;)Ljava/util/Calendar;");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
		return M4_ERROR;
	}

	if (jStringToCalendarMethod == NULL)
    {
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
        return M4_ERROR;
    }

    // Convertir de fecha juliana a cadena.
	oDate.set(ai_dJulianDate);
	oDate.get(sDate);

	// Obtener el nombre del método que se quiere invocar.
	jsDate = M4JVM_NewJString(poJNIEnv, sDate.c_str());
	if (jsDate == NULL)
	{
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
		return M4_ERROR;
	}

	// Ejecutar método stringToCalendar de la clase M4JNIUtils.
	ao_joCalendar = poJNIEnv->CallStaticObjectMethod(jJNIUtilsClass, jStringToCalendarMethod, jsDate);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
		poJNIEnv->DeleteLocalRef(jsDate);
		return M4_ERROR;
	}
	
	poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
	poJNIEnv->DeleteLocalRef(jsDate);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_MakeDouble
*	Description:	This function obtians a C double from a Double Java object.

*	Parameters:		
*  	Input:			ai_joDouble:	Double object.
*
* 	Output:			ao_dDouble:		double value.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeDouble(jobject ai_joDouble, m4double_t & ao_dDouble)
{
	jclass		jDoubleClass = NULL;
	jmethodID	jDoubleValueMethod = NULL;
	
	// Inicializaciones.
	ao_dDouble = 0;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	jDoubleClass = poJNIEnv->FindClass("java/lang/Double");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	jDoubleValueMethod = poJNIEnv->GetMethodID(jDoubleClass, "doubleValue", "()D");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jDoubleClass);
		return M4_ERROR;
	}
	
	poJNIEnv->DeleteLocalRef(jDoubleClass);

	ao_dDouble = poJNIEnv->CallDoubleMethod(ai_joDouble, jDoubleValueMethod);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


/****************************************************************************
*	Name:			_MakeString
*	Description:	This function obtains a C string object from a java String. 
*
*	Parameters:		
*  	Input:			ai_joString:	String object.
*
* 	Output:			ao_pString:		C string.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeString(jobject ai_joString, m4char_t * & ao_pString)
{
	m4pchar_t	pParam = NULL;
	
	// Inicializaciones.
	ao_pString = NULL;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener el C string a partir del jstring.
	pParam = M4JVM_GetString(poJNIEnv, (jstring)ai_joString);
	if (pParam == NULL)
	{
		return M4_ERROR;
	}

	// Devolver el resultado.
	ao_pString = new m4char_t[strlen(pParam) + 1];
	strcpy(ao_pString, pParam);

	delete [] pParam;
	
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_MakeDate
*	Description:	This function obtains a julian date (double) from a Java
*					Calendar object.
*					type.
*
*	Parameters:		
*  	Input:			aio_joCalendar:		Calendar object
*
* 	Output:			ao_dJulianDate:		julian date.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _MakeDate(jobject aio_joCalendar, m4double_t & ao_dJulianDate)
{
	jclass			jJNIUtilsClass = NULL;
	jmethodID		jCalendarToStringMethod = NULL;
	jstring			jsDate = NULL;
	m4pchar_t		pDate = NULL;
	M4ClTimeStamp	oDateConvert;
	
	// Inicializaciones.
	ao_dJulianDate = 0;

	// Obtener JVM.
	JNIEnv * poJNIEnv = M4JVM_GetJavaEnv();
    if (poJNIEnv == NULL)
	{
        return M4_ERROR;
	}

	// Obtener la clase M4JNIUtils.
	jJNIUtilsClass = poJNIEnv->FindClass("com/meta4/jni/M4JNIUtils");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		return M4_ERROR;
	}
	
	// Obtener método "stringToCalendar".
    jCalendarToStringMethod = poJNIEnv->GetStaticMethodID(jJNIUtilsClass, "calendarToString", "(Ljava/util/Calendar;)Ljava/lang/String;");
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
		return M4_ERROR;
	}

	if (jCalendarToStringMethod == NULL)
    {
        return M4_ERROR;
    }

    // Ejecutar método stringToCalendar de la clase M4JNIUtils.
	jsDate = (jstring)poJNIEnv->CallStaticObjectMethod(jJNIUtilsClass, jCalendarToStringMethod, aio_joCalendar);
	if (M4JVM_CheckExceptions() != M4_FALSE)
	{
		poJNIEnv->DeleteLocalRef(jJNIUtilsClass);
		return M4_ERROR;
	}
	
	poJNIEnv->DeleteLocalRef(jJNIUtilsClass);

	// Obtener la fecha en formato cadena a partir del jstring.
	pDate = M4JVM_GetString(poJNIEnv, jsDate);
	if (pDate == NULL)
	{
		poJNIEnv->DeleteLocalRef(jsDate);
		return M4_ERROR;
	}

	poJNIEnv->DeleteLocalRef(jsDate);
	
	// Convertir la fecha en formato cadena a fecha juliana.
	oDateConvert.set(pDate);
	oDateConvert.get(ao_dJulianDate);

	delete [] pDate;

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_AddJObject
*	Description:	This function adds a given Java object to map.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle: M4Object handle.
*					ai_jObject:		object.	
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _AddJObject(m4uint32_t ai_uiM4OHandle, jobject ai_jObject)
{
	jobjectList_t *	poObjectList = NULL;
	m4bool_t		bFound = M4_TRUE;

	// Verificar si existe una lista de objetos Java para el M4Object
	// previamente creada.
	if (_GetJObjectList(ai_uiM4OHandle, poObjectList) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Si la lista en nula, se crea una nueva lista de jobjects y se inserta
	// el objeto Java correspondiente.
	if (poObjectList == NULL)
	{
		poObjectList = new jobjectList_t;
		if (poObjectList == NULL)
		{
			return M4_ERROR;
		}
	}

	s_oObjMutex.Lock();

	poObjectList->push_back(ai_jObject);
	s_jobjectMap.insert(jObjectMap_t::value_type(ai_uiM4OHandle, poObjectList));

	s_oObjMutex.Unlock();

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_DeleteJObject
*	Description:	This function deletes a given Java object from map.
*
*	Parameters:
*  	Input:			ai_poJNIEnv:	JVM environment.
*					ai_uiM4OHandle: M4Object handle.
*					ai_jObject:		java object.	
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _DeleteJObject(JNIEnv * ai_poJNIEnv, m4uint32_t ai_uiM4OHandle, jobject ai_jObject)
{
	jobjectList_t *	poObjectList = NULL;
	m4bool_t		bObjectFound = M4_FALSE;

	// Verificar si existe una lista de objetos Java para el M4Object
	// previamente creada.
	if (_GetJObjectList(ai_uiM4OHandle, poObjectList) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Bug 0157095.
	// Verificar que si existe el objeto Java en la lista de objetos de este M4Object.
	if (poObjectList != NULL)
	{
		if (_FindJObject(poObjectList, ai_jObject, bObjectFound) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	
	// Liberar el objeto java.
	if (bObjectFound == M4_TRUE)
	{
		// Liberar el objeto Java.
		ai_poJNIEnv->DeleteLocalRef(ai_jObject);

		// Borrar el jobject de la lista.
		s_oObjMutex.Lock();
		poObjectList->remove(ai_jObject);
		s_oObjMutex.Unlock();
	}
	else
	{
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_GetJObjectList
*	Description:	This function returns a jobject list for a given M4Object
*					handle.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle: M4Object handle.	
*
* 	Output:			ao_pjObjectList: Java object list.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _GetJObjectList(m4uint32_t ai_uiM4OHandle, jobjectList_t *& ao_pjObjectList)
{
	jObjectMap_t::iterator m4objectIterator;
	
	// Inicializaciones.
	ao_pjObjectList = NULL;

	// Primero se busca si ya existe una entrada en el mapa
	// para este M4Object.
	m4objectIterator = s_jobjectMap.find(ai_uiM4OHandle);
	if (m4objectIterator != s_jobjectMap.end())
	{
		ao_pjObjectList = (*m4objectIterator).second;
	}

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_FindJObject
*	Description:	This function returns if a given jobject is found in a 
*					given jobject list.
*
*	Parameters:
*  	Input:			ai_pjObjectList: Java object list.
* 	Output:			
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _FindJObject(jobjectList_t * ai_pjObjectList, jobject ai_jObject, m4bool_t & ao_bFound)
{
	jobjectList_t::iterator oIterator;
	jobject					joObject = NULL;

	// Inicializaciones.
	ao_bFound = M4_FALSE;

	// Verificar que la lista que nos pasan no es nula.
	if (ai_pjObjectList == NULL)
	{
		return M4_SUCCESS;
	}

	s_oObjMutex.Lock();

	// Recorrer la lista y ver si está el jobject.
	for (oIterator = ai_pjObjectList->begin(); oIterator != ai_pjObjectList->end(); oIterator++)
	{
		joObject = *oIterator;
		if (joObject == ai_jObject)
		{
			ao_bFound = M4_TRUE;
			break;
		}	
	}

	s_oObjMutex.Unlock();

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_FindJObject
*	Description:	This function returns if a given jobject is found in a 
*					given jobject list.
*
*	Parameters:
*  	Input:			ai_uiM4OHandle:		M4Object handle.
*					ai_jObject:			java object.
*					
* 	Output:			ao_bFound:			java object found.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _FindJObject(m4uint32_t ai_uiM4OHandle, jobject ai_jObject, m4bool_t & ao_bFound)
{
	jobjectList_t *	poObjectList = NULL;

	// Inicializaciones.
	ao_bFound = M4_FALSE;

	// Verificar si existe una lista de objetos Java para el M4Object
	// previamente creada.
	if (_GetJObjectList(ai_uiM4OHandle, poObjectList) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Si la lista en nula, se crea una nueva lista de jobjects y se inserta
	// el objeto Java correspondiente.
	if (poObjectList == NULL)
	{
		return M4_SUCCESS;
	}
	else
	{
		// Verificar que si existe el objeto Java en la lista de objetos de este M4Object.
		if (_FindJObject(poObjectList, ai_jObject, ao_bFound) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

