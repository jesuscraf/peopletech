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

#include "m4jvm.h"
#include "m4stl.hpp"
#include "regedit.hpp"
#include "m4buildversion.hpp"
#include "file_finder.hpp"
#include "m4regdef.hpp"


// Depuración JAVA
// #define JAVA_DEBUG

// Se define particularidades de los sistemas operativos
#ifdef _UNIX
#include <dirent.h>
#include <unistd.h>
#endif

#ifdef _WINDOWS
#define DIR_SEPARATOR  '\\'
#define	PATH_SEPARATOR ";"
#define DIR_RELATIVE   ".\\"
#define PATH_ENV	   "PATH"
#define JVM_LIB		   "jvm.dll"
#endif
#ifdef _SOLARIS
#define DIR_SEPARATOR  '/'
#define	PATH_SEPARATOR ":"
#define DIR_RELATIVE   "./"
#define PATH_ENV	   "LD_LIBRARY_PATH"
#define JVM_LIB		   "libjvm.so"
#endif
#ifdef _AIX
#define DIR_SEPARATOR  '/'
#define	PATH_SEPARATOR ":"
#define DIR_RELATIVE   "./"
#define PATH_ENV	   "LIBPATH"
#define JVM_LIB		   "libjvm.a"  // Java 1.4
#define JVM_LIB2	   "libjvm.so" // Java 5 y superior
#endif

JavaVM * M4JVM::m_jvm = NULL;				// Puntero a la Java VM
JNIEnv * M4JVM::m_env = NULL;				// Puntero al interfaz de métodos nativos (contexto) del thread principal
jint M4JVM::m_Status = JNI_OK;				// Estado JVM
M4LibHandle M4JVM::m_HandleJVM = NULL;		// Instancia de la librería JVM
ClMutex M4JVM::m_oMutex = ClMutex(M4_TRUE);	// Mutex para inicialización única de la JVM
m4int32_t M4JVM::m_iThreadCount = 0;		// Número de threads que está asociados a la JVM
m4iden_t M4JVM::m_iOwnerThread = 0;			// Id del Thread propietario

string M4JVM::m_sRegPath = "";				// Path raiz de opciones Java
string M4JVM::m_sOldPath = "";				// Path de librerias original para el proceso
string M4JVM::m_sTempPath = "";				// Directorio temporal

// Esto evita la inclusión de la librería jvm (jvm.lib en Windows)
// Puntero a la función JNI_CreateJavaVM
typedef jint (JNICALL CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);

JavaVM * M4JVM::GetJavaVM()
{
	if (!m_jvm && JNI_OK == m_Status)
	{
		m_Status = InitJVM();
	}

	return m_jvm;
}

JNIEnv * M4JVM::GetJavaEnv()
{
	JNIEnv * env = NULL;
	M4IThreadEventHandler * oEventHandler = NULL;

	// Exclusión mutua para evitar posible inicialización de la JVM en paralelo
	m_oMutex.Lock();

	if (!m_jvm && JNI_OK == m_Status)
	{
		m_Status = InitJVM();
		m_iOwnerThread = M4Thread::GetCurrentIdThread();
	}

	if (JNI_OK == m_Status)
	{
		// Se obtiene el contexto asociado al thread
		// (Podría utilizarse directamente AttachCurrenThread ya que devuelve
		// el contexto. Si ya está vinculado no hace nada más y si no lo está vincula el thread a la JVM)
		if (JNI_EDETACHED == m_jvm->GetEnv((void **)&env, JNI_VERSION_1_4))
		{
			// El thread no está vinculado a la JVM. De momento no usamos
			// el último parámetro que permite especificar la versión de JNI y nombre del thread
#ifdef JAVA_DEBUG
			cout << "Attach (In). Thread ID: " << M4Thread::GetCurrentIdThread() << " Thread number: " << ThreadsCount() << endl;
#endif
			m_jvm->AttachCurrentThread((void **)& env, NULL);
			oEventHandler = new M4FinishThreadEventHandler();
			M4ThreadEventDispatcher::AddHandler(oEventHandler);
			if (env) m_iThreadCount++;
#ifdef JAVA_DEBUG
			cout << "Attach (Out). Thread number: " << ThreadsCount() << endl;
#endif
		}
	}

	m_oMutex.Unlock();

	return env;
}

jint M4JVM::FreeJVM()
{
	JNIEnv * env = NULL;
	jint retValue = m_Status;

#ifdef JAVA_DEBUG
	cout << "FreeJVM: (In) Thread number:" << ThreadsCount() << endl;
	cout << "Thread ID:" << M4Thread::GetCurrentIdThread() << " Main Thread ID: "<< m_iOwnerThread << endl;
#endif

	// Exclusión mutua para realizar la liberación de forma ordenada.
	if (m_jvm && JNI_OK == m_Status)
	{
		if (m_iOwnerThread == M4Thread::GetCurrentIdThread())
		{
			if (m_iThreadCount > 1)
			{
#ifdef JAVA_DEBUG
				cout << "DestroyJVM Error. Threads pending"  << endl;
#endif
				// Hay Threads que no han realizado el Detach. No ejecutamos DestroyJVM para
				// evitar quedar bloqueada la aplicación.
				retValue = JNI_ERR;
			}
			else
			{
				// La invocación la está realizando el Thread propietario.
				// Por lo tanto debe realizar el DestroyJVM
				m_iOwnerThread = 0;
#ifdef JAVA_DEBUG
				cout << "DestroyJVM In"  << endl;
#endif
				retValue = DestroyJVM();
				m_oMutex.Lock();
				m_iThreadCount--;
				m_oMutex.Unlock();
#ifdef JAVA_DEBUG
				cout << "DestroyJVM Out" << endl;
#endif
			}
		}
		else
		{
			if (!m_jvm)
			{
#ifdef JAVA_DEBUG
				cout << "Error JVM" << endl;
#endif
				// Nunca debería entrar por aquí
				retValue = JNI_ERR;
			}
			// Se comprueba si se realizó el attach del thread actual
			else if (JNI_OK == m_jvm->GetEnv((void **)&env, JNI_VERSION_1_4))
			{
#ifdef JAVA_DEBUG
				cout << "Thread detach JVM In" << endl;
#endif
				retValue = m_jvm->DetachCurrentThread();
#ifdef JAVA_DEBUG
				cout << "Thread detach JVM Out" << endl;
#endif
				m_oMutex.Lock();
				if (JNI_OK == retValue)	m_iThreadCount--;
				m_oMutex.Unlock();
			}
		}
	}

#ifdef JAVA_DEBUG
	cout << "FreeJVM: (Out) Thread number:" << ThreadsCount() << endl;
#endif

	return retValue;
}

m4int32_t M4JVM::ThreadsCount()
{
	return m_iThreadCount;
}

string M4JVM::GetRegPath()
{
	m4char_t sRegPath[1024];
	m4uint32_t iLenOut;

	if (m_sRegPath.length() == 0)
	{
		M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, "JAVA", sRegPath, 1024, iLenOut );
		m_sRegPath = sRegPath;
	}

	return m_sRegPath;
}

string M4JVM::GetM4TempPath()
{
	m4char_t sRegPath[1024];
	m4char_t sTempPath[1024];
	m4uint32_t iLenOut;

	if (m_sTempPath.length() == 0)
	{
		if (getenv ("M4SERVER_PATH"))
		{
			// Server
			M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_SERVER, "CVM", sRegPath, 1024, iLenOut );
		}
		else
		{
			// Client
			M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, "CVM", sRegPath, 1024, iLenOut );
		}

		GetRegKeyData( sRegPath, "TempDir", sTempPath);

		m_sTempPath = sTempPath;
	}

	return m_sTempPath;
}

string M4JVM::ResolvePath(m4pchar_t ai_sPath)
{
	string sRetPath = "";

	if (ai_sPath)
	{
		if (0 == strncmp(ai_sPath, DIR_RELATIVE, 2)) // Path relativo
		{
#ifdef _WINDOWS
			m4char_t sMainPath[1024];

			if (GetModuleFileName(0, sMainPath, 1024) > 0)
			{
				*strrchr(sMainPath, DIR_SEPARATOR) = '\0'; // Se elimina el nombre del ejecutable
				sRetPath = (string)sMainPath;
				sRetPath+= (ai_sPath + 1); // Se elimina el punto de ruta relativa
			}
#else
			m4pchar_t sMainPath;

			sMainPath = getenv("APPHOME");
			if (sMainPath)
			{
				sRetPath = (string)sMainPath;
				sRetPath+= (ai_sPath + 1); // Se elimina el punto de ruta relativa
			}
#endif
		}
		else // Path absoluto
		{
			sRetPath = ai_sPath;
		}

	}

	return sRetPath;
}


jint M4JVM::LoadJVM()
{
	string sFullPath;
	string sRootPath;

	m4pchar_t sOldPath;

	m4char_t sJavaPath[1024];

	m4pcchar_t pccServer = NULL;

	if (NULL != m_HandleJVM)
	{
		return JNI_OK;
	}

	// Verificar si estamos en cliente o en server consultando la variable de entorno
	// M4SERVER_PATH.
	pccServer = getenv( "M4SERVER_PATH" ) ;
	if (pccServer == NULL)
	{
		// Se ajusta en caliente la variable de entorno PATH
		if (GetRegKeyData((m4pchar_t) GetRegPath().c_str(), "JavaHome", sJavaPath))
		{
			// Bug 179861. Si JavaHome es vacío puede tirar la aplicación ya que
			// sRootPath[sRootPath.length() - 1] accedería a una posición no válida
			if ((NULL == sJavaPath) || strlen(sJavaPath) == 0)
			{
				return (JNI_ERR);
			}

			sRootPath = ResolvePath(sJavaPath);
			if (DIR_SEPARATOR != sRootPath[sRootPath.length() - 1])
			{
				// Se concatena el separador de directorio puesto que no lo incluía
				sRootPath+= DIR_SEPARATOR;
			}

			sOldPath = getenv(PATH_ENV);

			sFullPath = (string)PATH_ENV + "=";
			sFullPath+= sRootPath + "bin\\client;";
			sFullPath+= sRootPath + "bin;";
			sFullPath+= sOldPath;

			m_sOldPath = sOldPath;

			// No es problemático en WINDOWS asignar directamente el buffer
			putenv((m4pchar_t) sFullPath.c_str());
		}
	}
	else
	{
		// Si no está configurada la variable de entorno M4JAVAHOME no se intenta cargar la máquina virtual
		sOldPath = getenv("M4JAVAHOME");
		if ((NULL == sOldPath) || strlen(sOldPath) == 0)
		{
			return (JNI_ERR);
		}
	}

#if defined ( _SOLARIS)  || defined ( _AIX )
	//Load the jvm DLL (the jvm !)
	m_HandleJVM = dlopen(JVM_LIB, RTLD_NOW | RTLD_GLOBAL);
 #if defined ( _AIX )
 	// A para de la JRE 5 de IBM la librería es libjvm.so en lugar de libjvm.a
	if (NULL == m_HandleJVM)
	{
		m_HandleJVM = dlopen(JVM_LIB2, RTLD_NOW | RTLD_GLOBAL);
	}
 #endif
#elif defined ( _WINDOWS )
	m_HandleJVM = LoadLibrary(JVM_LIB);
#endif

	return (NULL == m_HandleJVM ? JNI_ERR : JNI_OK);
}

string M4JVM::ResolveClassPath()
{
	string sExpandedPath;
	string sClassPath;
	m4pchar_t sRegClassPath;
	m4pcchar_t pccServer = NULL;
	vector <string> vPaths;

	pccServer = getenv( "M4SERVER_PATH" ) ;
	if (pccServer == NULL)
	{
		sRegClassPath = new m4char_t[2048];
		// Se lee del registro en Windows
		if (GetRegKeyData((m4pchar_t) GetRegPath().c_str(), "ClassPath", sRegClassPath))
		{
			// Se rellena el array con todos los paths definidos
			ParseJavaArgs(sRegClassPath, vPaths, ";");
		}
		delete [] sRegClassPath;
	}
	else
	{
		// Se lee de variable de entorno en Unix
		m4pchar_t sRegClassPath = getenv("M4CLASSPATH");
		if (sRegClassPath)
		{
			// Se rellena el array con todos los paths definidos
			ParseJavaArgs(sRegClassPath, vPaths, PATH_SEPARATOR);
		}
	}

	sClassPath = "";

	// Se unifican todos los paths en sClassPath extendiendo
	// los posibles directorios relativos
	for (m4int32_t i = 0; i<vPaths.size(); i++)
	{
		sExpandedPath = ResolvePath((m4pchar_t) vPaths[i].c_str());

		m4uint32_t	iLength = sExpandedPath.size() ;

		if( iLength > 2 && sExpandedPath.at( iLength - 1 ) == '*' && ( sExpandedPath.at( iLength - 2 ) == '\\' || sExpandedPath.at( iLength - 2 ) == '/' ) )
		{
			// Se añaden los jar del directorio del asterisco
			m4pcchar_t		pccFileName = NULL ;
			ClFile			oFile ;
			ClFileFinder	oFileFinder( sExpandedPath.c_str() ) ;

			while( oFileFinder.FindNext( oFile ) != M4_ERROR )
			{
				if( oFile.IsValid() == M4_FALSE )
				{
					continue ;
				}
				
				pccFileName = oFile.GetFileName() ;
				iLength = strlen( pccFileName ) ;

				if( oFile.IsFile() == M4_TRUE )
				{
					if( iLength > 4 && strcmpi( pccFileName + iLength - 4, ".jar" ) == 0 )
					{
						if (sClassPath.size()>0) sClassPath+= ";";
						sClassPath+= pccFileName;
					}
				}
				else
				{
					if( iLength > 10 && strcmpi( pccFileName + iLength - 10, "properties" ) == 0 && ( pccFileName[ iLength - 11 ] == '\\' || pccFileName[ iLength - 11 ] == '/' ) )
					{
						if (sClassPath.size()>0) sClassPath+= ";";
						sClassPath+= sExpandedPath.substr(0, sExpandedPath.size()-2);
					}
				}
			}
		}
		else
		{
			// Se añaden los path sin asterisco
			if (sClassPath.size()>0) sClassPath+= ";";
			sClassPath+= sExpandedPath;
		}
	}

	return sClassPath;
}

vector<string> M4JVM::ResolveJavaArgs()
{
	vector <string> vJavaArgs;
	m4pchar_t		pcRegJavaArgs = NULL;
	m4pcchar_t		pccServer = NULL;

	pccServer = getenv("M4SERVER_PATH");
	if (pccServer == NULL)
	{
		pcRegJavaArgs = new m4char_t[1024];

		// Se lee del registro en Windows
		if (GetRegKeyData((m4pchar_t) GetRegPath().c_str(), "JavaArgs", pcRegJavaArgs) == M4_TRUE)
		{
			// Rellenar el vector con las distintas opciones de la máquina virtual de Java.
			ParseJavaArgs(pcRegJavaArgs, vJavaArgs, ";");
		}
		delete [] pcRegJavaArgs;
	}
	else
	{
		// Se lee de variable de entorno en Unix
		pcRegJavaArgs = getenv("M4JAVAARGS");
		if (pcRegJavaArgs != NULL && pcRegJavaArgs != "")
		{
			// Rellenar el vector con las distintas opciones de la máquina virtual de Java.
			ParseJavaArgs(pcRegJavaArgs, vJavaArgs, ";");
		}
	}

	return vJavaArgs;
}

void M4JVM::ParseJavaArgs(const string & ai_sJavaArgs, vector<string> & ao_vJavaArgs, m4pcchar_t ai_pccSeparator)
{
	m4pchar_t	pToken =  NULL;
	string		sJavaArgs = "";
	string		sParsedJavaArgs = "";
	m4uint32_t	iLength = 0;

	// Inicializaciones.
	sJavaArgs = ai_sJavaArgs;
	iLength = sJavaArgs.size();
	
	if (iLength == 0)
	{
		return;
	}
	
	sParsedJavaArgs = sJavaArgs;
	if (iLength > 2)
	{
		if (sJavaArgs[0] == '"' && sJavaArgs[iLength - 1] == '"')
		{
			sParsedJavaArgs = sJavaArgs.substr(1, iLength - 2);
		}
	}

	pToken = strtok((m4pchar_t)sParsedJavaArgs.c_str(), ai_pccSeparator);
	while (pToken != NULL)
	{
		ao_vJavaArgs.push_back(pToken);
		pToken = strtok(NULL, ai_pccSeparator);
	}
	
	return;
}

jint M4JVM::BuildJVMArgs(JavaVMInitArgs &vm_args)
{
	string			sClassPath;
	string			sOptStr;
	vector<string>	vJavaArgs;
	m4int32_t		i = 0;
	m4int32_t		iArgNum = 0;

	if (m_HandleJVM == NULL)
	{
		// No hay instancia JVM cargada
		return JNI_ERR;
	}

	// Primer parámetro: java.class.path (Ruta donde va a buscar los jars)
	sClassPath = ResolveClassPath();
	if (sClassPath.length() == 0)
	{
		// No hay definido un ClassPath
		return JNI_ERR;
	}

	// Segundo parámetro: parámetros adicionales para añadir en el arranque de la máquina
	// virtual Java.
	vJavaArgs = ResolveJavaArgs();
	iArgNum = vJavaArgs.size();

	// Configurar los argumentos.
	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_4;

	vm_args.nOptions = iArgNum + 1;
	vm_args.ignoreUnrecognized = JNI_TRUE;
	vm_args.options = (JavaVMOption*) calloc(vm_args.nOptions, sizeof(JavaVMOption));
	for (i = 0; i < vm_args.nOptions; i++)
	{
		memset(vm_args.options, i + 1, (vm_args.nOptions) * sizeof(JavaVMOption));
	}

	sOptStr = "-Djava.class.path=" + (string)sClassPath;
	vm_args.options[0].optionString = (char*)malloc(sOptStr.size() + 1);
	strcpy(vm_args.options[0].optionString, sOptStr.c_str());

	// Especificar el resto argumentos para la máquina virtual Java.
	for (i = 0; i < iArgNum; i++)
	{
		sOptStr = vJavaArgs[i];
		vm_args.options[i + 1].optionString = (char*)malloc(sOptStr.size() + 1);
		strcpy(vm_args.options[i + 1].optionString, sOptStr.c_str());
	}

	return JNI_OK;
}

jint M4JVM::InitJVM()
{
	jint retValue;

	CreateJavaVM_t * pfnCreateJavaVM; //Función de punto de entrada a JNI

	JavaVMInitArgs vm_args; // Argumentos de inicialización de JDK 1.4 VM


	// 1. Se carga la librería de la JVM
	retValue = LoadJVM();
	if (JNI_OK != retValue)
	{
		// No se ha podido cargar la librería de la JVM
		return retValue;
	}

	// 2. Se busca el punto de entrada a la función de creación de la JVM JNI_CreateJavaVM
#if defined ( _SOLARIS) || defined ( _AIX )
	pfnCreateJavaVM =
		(CreateJavaVM_t *) dlsym(m_HandleJVM, "JNI_CreateJavaVM");
#endif
#ifdef _WINDOWS
	pfnCreateJavaVM =
		(CreateJavaVM_t *) GetProcAddress(m_HandleJVM, "JNI_CreateJavaVM");
#endif

	if (NULL == pfnCreateJavaVM)
	{
		// No se ha podido cargar el punto de entrada JNI_CreateJavaVM
		return JNI_ERR;
	}

	// 3. Definimos los argumentos de la JVM
	retValue = BuildJVMArgs(vm_args);
	if (JNI_OK != retValue)
	{
		// No se ha podido definir los argumentos
		return retValue;
	}

	// 4. Se crea la JVM
	retValue = pfnCreateJavaVM(&m_jvm, (void**)&m_env, &vm_args);

	// 5. Se libera la memoria utilizada en los argumentos de la JVM
	free(vm_args.options);

	// 7. Si todo ha ido correcto incrementamos el thread principal
	if (JNI_OK == retValue) m_iThreadCount++;

	return retValue;
}


jint M4JVM::DestroyJVM()
{
	jint retValue = JNI_OK;

	if (m_jvm)	// Si no estaba creada devolvemos directamente JNI_OK
	{
		// Se destruye la JVM
		retValue = m_jvm->DestroyJavaVM();
		m_jvm = NULL;
		m_env = NULL;
	}

	return retValue;
}

