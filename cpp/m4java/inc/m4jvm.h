//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4jvm.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             13/08/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    This module manages the Java Virtual Machine
//
//
//==============================================================================

#ifndef _M4JVM_H_
#define _M4JVM_H_

// Java Native Interface
#include "jni.h"

#include "syncro.hpp"
#include "m4thread.hpp"
#include "m4stl.hpp"


#if defined ( _SOLARIS)  || defined ( _AIX )
#include <dlfcn.h>
typedef  void* M4LibHandle;
#elif defined( _WINDOWS)
typedef HINSTANCE M4LibHandle;
#endif

class M4JVM 
{
private:

	static JavaVM *m_jvm;			 // Puntero a la Java VM
	static JNIEnv *m_env;			 // Puntero al interfaz de métodos nativos (contexto) del thread principal 
	static jint	m_Status;			 // Estado JVM
	static M4LibHandle m_HandleJVM;  // Instancia de la librería JVM
	static ClMutex m_oMutex;		 // Mutex para inicialización única de la JVM
	static m4int32_t m_iThreadCount; // Número de threads que está asociados a la JVM
	static m4iden_t m_iOwnerThread;	 // Id del Thread propietario

	static string m_sRegPath;		 // Path raiz de opciones Java
	static string m_sOldPath;		 // Path de librerias original para el proceso
	static string m_sTempPath;		 // Directorio temporal

public:
	// Retorna el contexto de ejecución Java asociado al thread llamanta
	static JNIEnv * GetJavaEnv();
	// Desacopla el thread llamante con la JVM. Si se trata del thread que inició
	// la máquina virtual, la libera
	static jint FreeJVM();
	// Retorna el número de threads que están asociados a la máquina virtual de Java
	// (incluye también el thread que inició la JVM)
	static m4int32_t ThreadsCount();
	
private:
	// Retorna un puntero a la JVM. La inicializa si es necesario
	static JavaVM * GetJavaVM();
	// Carga la JVM (Java Virtual Machine)
	static jint LoadJVM();
	// Construye los argumentos de inicialización de la JVM
	static jint BuildJVMArgs(JavaVMInitArgs &vm_args);
	// Inicializa la JVM
	static jint InitJVM();
	// Destruye la máquina virtual de Java
	static jint DestroyJVM();
	// Devuelve la ruta raiz de opciones JAVA
	static string GetRegPath();
	// Devuelve el directorio temporal de la aplicación
	static string GetM4TempPath();
	// Comprueba y resuelve los paths relativos
	static string ResolvePath(m4pchar_t ai_sPath);
	// Devuelve el ClassPath que debe utilizar la JVM
	static string ResolveClassPath();
	// Devuelve los argumentos que debe utilizar la JVM
	static vector<string> ResolveJavaArgs();
	// Parsea los argumentos de JVM separados por ";".
	static void ParseJavaArgs(const string & ai_sJavaArgs, vector<string> & ao_vJavaArgs, m4pcchar_t ai_pccSeparator);
};

class M4FinishThreadEventHandler:public M4IThreadEventHandler
{
	public:
		void EndThread() { M4JVM::FreeJVM(); }
};

#endif
