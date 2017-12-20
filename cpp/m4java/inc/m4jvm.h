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
//	Owner:			Antonio Dur�n
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
	static JNIEnv *m_env;			 // Puntero al interfaz de m�todos nativos (contexto) del thread principal 
	static jint	m_Status;			 // Estado JVM
	static M4LibHandle m_HandleJVM;  // Instancia de la librer�a JVM
	static ClMutex m_oMutex;		 // Mutex para inicializaci�n �nica de la JVM
	static m4int32_t m_iThreadCount; // N�mero de threads que est� asociados a la JVM
	static m4iden_t m_iOwnerThread;	 // Id del Thread propietario

	static string m_sRegPath;		 // Path raiz de opciones Java
	static string m_sOldPath;		 // Path de librerias original para el proceso
	static string m_sTempPath;		 // Directorio temporal

public:
	// Retorna el contexto de ejecuci�n Java asociado al thread llamanta
	static JNIEnv * GetJavaEnv();
	// Desacopla el thread llamante con la JVM. Si se trata del thread que inici�
	// la m�quina virtual, la libera
	static jint FreeJVM();
	// Retorna el n�mero de threads que est�n asociados a la m�quina virtual de Java
	// (incluye tambi�n el thread que inici� la JVM)
	static m4int32_t ThreadsCount();
	
private:
	// Retorna un puntero a la JVM. La inicializa si es necesario
	static JavaVM * GetJavaVM();
	// Carga la JVM (Java Virtual Machine)
	static jint LoadJVM();
	// Construye los argumentos de inicializaci�n de la JVM
	static jint BuildJVMArgs(JavaVMInitArgs &vm_args);
	// Inicializa la JVM
	static jint InitJVM();
	// Destruye la m�quina virtual de Java
	static jint DestroyJVM();
	// Devuelve la ruta raiz de opciones JAVA
	static string GetRegPath();
	// Devuelve el directorio temporal de la aplicaci�n
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
