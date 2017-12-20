//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo> 
// File:             m4loadll.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:6/28/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Definition:
//
//    This module defines...
//
//

// MCP: comentarios al codigo encerrado entre las marcas
//	/* MCP: ver nota en encabezado */
//	/**/

//	La idea de esta libreria es la carga dinamica de dll's, independientemente
//	del sistema operativo.
//	Basicamente, para cargar una dll, pasamos el nombre, y en funcion de la
//	plataforma, este codigo annade la extension correspondiente a la dll 
//	(.dll. .so. .sl)
//	En Unix, las librerias normalmente empiezan por el prefijo lib, pero en este
//	caso, se decidio no annadir el prefijo aqui, sino que se supone que el nombre
//	viene bien dado como parametro de entrada.
//	Ahora ha surgido un problema, tenemos librerias que se linkan en tiempo de
//	compilacion (con lo que necesitan que se llamen lib...) y ademas se cargan
//	dinamicamente con loaddll en tiempo ejecucion (cuando se cargan desde un
//	meta4object.
//	Inicialmente, hemos generado estas nuevas dll en UNIX sin el prefijo lib, y
//	forzamos al ejecutable que se linke con ellas. El problema esta qeu en 
//	ejecucion, al no haberse linkado de la forma estandar -lname, estas dll no
//	son buscadas en el entorno (LD_LIBRARY_PATH o equivalente) con lo que no
//	se consigue arrancar el ejecutable.
//	Lo que hemos hecho ahora, es que esta libreria, la loadll, intente cargar
//	la dll con el procedimiento de siempre, y en caso de fallo, hace un segundo
//	intento concatenando al nombre de la dll el prefijo lib.
//	De esta forma creemos que mantenemos la compatibilidad con el codigo
//	que teniamos hasta ahora.
//
//==============================================================================
//## begin module.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module.cm

//## begin module.cp preserve=no
//## end module.cp

//## Module: m4loadll; Package body
//## Subsystem: <Top Level>
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\maite\m4loadll\src\win32\m4loadll.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// m4loadll
#include "m4loadll.hpp"
//## begin module.declarations preserve=no
//## end module.declarations

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

//## begin module.additionalDeclarations preserve=yes
#if defined ( _SOLARIS) || defined ( _AIX )
#include <ospace/std/iostream>
#endif
#ifdef _HP
#include <string.h>
#include <strings.h>
#endif
//## end module.additionalDeclarations


// Class M4ClLoaDll 



M4ClLoaDll::M4ClLoaDll ()
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.hasinit preserve=no
  //## end M4ClLoaDll::M4ClLoaDll%884371568.hasinit
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.initialization preserve=yes
  //## end M4ClLoaDll::M4ClLoaDll%884371568.initialization
{
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.body preserve=yes
	m_bIsOpenDll=M4_FALSE;
	m_hDllHandle = NULL;
  //## end M4ClLoaDll::M4ClLoaDll%884371568.body
}


M4ClLoaDll::~M4ClLoaDll ()
{
  //## begin M4ClLoaDll::~M4ClLoaDll%884371569.body preserve=yes

	if (m_bIsOpenDll == M4_TRUE)
	{
//JRM Antes llamaba a dllclose. Si ya tenemos la función Close, debemos usarla
		Close();
//JRM En un destructor, asignar valores a miebors del objeto 
// que se está destruyendo no tiene mucho sentido. Por tanto, comento este código
//		m_hDllHandle=NULL;
//		m_bIsOpenDll=M4_FALSE;
	}
			
  //## end M4ClLoaDll::~M4ClLoaDll%884371569.body
}



//## Other Operations (implementation)
m4return_t M4ClLoaDll::Open (m4pcchar_t ai_pccNameDll)
{
  //## begin M4ClLoaDll::Open%884371570.body preserve=yes
/*
- Según S.O, sustituir carácter separador de directorio por el adecuado .
- Separar en path, nombre y extensión En Unix, se usan funciones basename y dirname, 
	y  el nombre del fichero se separa  en nombre y extensión. En NT, 
	se usa _splitpath.
- Si estamos en Unix, y si el path comienza por "x:"  siendo "x" cualquier carácter, 
	se sustituye por ".".
- Si no hay extensión, dependiendo de los defines indicadores de sistema 
	(_HP, _SOLARIS, …), se elije  la extensión adecuada. 
- Separar el path en directorios. Sustituir aquellos directorios que comienzen 
	por "$" por lo que contenga la variable de entorno de su mismo nombre, 
	(excluída la letra $). Tanto en Unix como en NT, se usa la función getenv.
- Construir el nombre completo de la librería a partir de los directorios, 
	nombre de librerìa y extensión. En la construcción se tiene en cuenta que 
	el nombre de un directorio puede ser vacío, como consecuencia de que haya 
	una referencia a una variable de entorno que no exista o vacía.
-  Seguir con el código original, pasándole como nombre de librería el resultante 
	de los pasos anteriores. 
*/
#ifndef MAX_DIRECTORIES
#define MAX_DIRECTORIES	40
#endif

#ifdef _HP
	//En HP, FILENAME_MAX = 14
	char		path_buffer[1024];   
	char		final_path_buffer[1024];   
	char		dir[1024];
	char		fname[1024];   

	/* MCP: ver nota en encabezado */
	char		inputdirname[1024];		// nombre del directorio origen   
	char		inputfname[1024];		// nombre de la dll   
	/**/

	char		*directories[MAX_DIRECTORIES];
	int			num_directories = 0;
	m4bool_t	bFullPath;
	char		*last;
#else
	char		path_buffer[FILENAME_MAX];   
	char		final_path_buffer[FILENAME_MAX];   
	char		dir[FILENAME_MAX];
	char		fname[FILENAME_MAX];   

	/* MCP: ver nota en encabezado */
	char		inputdirname[FILENAME_MAX];		// nombre del directorio origen   
	char		inputfname[FILENAME_MAX];		// nombre de la dll   
	/**/
	
	char		*directories[MAX_DIRECTORIES];
	int			num_directories = 0;
	char		*last;
	m4bool_t	bFullPath;
#endif

	if (m_bIsOpenDll == M4_TRUE)
	{
		return M4_ERROR;
	}

#ifdef _HP
	if (strlen(ai_pccNameDll) >= 1024)
#else
	if (strlen(ai_pccNameDll) >= FILENAME_MAX)
#endif
	{
#ifdef _DEBUG
		cerr << "Nombre demasiado largo" <<endl;
#endif
		return M4_ERROR;
	}

	if (strlen(ai_pccNameDll) > 2 && ai_pccNameDll[1] == ':')
	{
		if (ai_pccNameDll[2] == '/' || ai_pccNameDll[2] == '\\')
		{
			sprintf(path_buffer, ".%s", &ai_pccNameDll[2]);
		}
		else
		{
			sprintf(path_buffer, "./%s", &ai_pccNameDll[2]);
		}
	}
	else
	{
		strcpy(path_buffer, ai_pccNameDll);
	}

	char	*p = path_buffer;
	while ((p = strchr(p, '\\'))!= NULL)
	{
		*p = '/';
	}

	//Usar basename y dirname sólo si nos pasan el path en el nombre
	if (strchr(path_buffer, '/') != NULL)
	{
		//Esta función es de tipo void
		strcpy(fname, basename(path_buffer));
		strcpy(dir, dirname(path_buffer));
	}
	else
	{
		strcpy(fname, path_buffer);
		strcpy(dir, "");
	}

	//Si tenemos variables de entorno, están todas en dir
	if (dir[0] == '/')
	{
		bFullPath = M4_TRUE;
	}
	else
	{
		bFullPath = M4_FALSE;
	}

	if ((p = strtok_r(dir, "/", &last)) != NULL)
	{
		directories[num_directories++] = p;

		while((p = strtok_r(NULL, "/", &last)) != NULL && num_directories < MAX_DIRECTORIES)
		{
			directories[num_directories++] = p;
		}
	}

	//Sustituir variables de entorno
	int	i;

	for (i = 0; i < num_directories; i++)
	{
		if (directories[i][0] == '$')
		{
			directories[i] = getenv(&directories[i][1]);
		}
	}

	final_path_buffer[0] = '\0';

	//Rehacer path completo:
	if (bFullPath == M4_TRUE)
	{
		strcpy(final_path_buffer, "/");
	}

	for (i = 0; i < num_directories; i++)
	{
		if (directories[i] != NULL && directories[i][0] != '\0')
		{
			strcat(final_path_buffer, directories[i]);
			strcat(final_path_buffer, "/");
		}
	}
	
	if (num_directories == 0 && strlen(fname) > 2 && fname[1] == ':') 
	{
		/* MCP: ver nota en encabezado */
		// guardo los nombres para el segundo intento.
		sprintf(inputdirname, "%s./", final_path_buffer);
		strcpy(inputfname, &fname[2]);
		/**/

		sprintf(final_path_buffer,"./%s", &fname[2]);		
	}
	else
	{
		/* MCP: ver nota en encabezado */
		// guardo los nombres para el segundo intento.
		strcpy(inputdirname, final_path_buffer);
		strcpy(inputfname, fname);
		/**/
		
		strcat(final_path_buffer, fname);
	}

	if (strchr(fname, '.') == NULL)
	{
#if defined ( _SOLARIS)
		strcat(final_path_buffer, ".so");
#endif
#if defined ( _AIX )
		strcat(final_path_buffer, ".a");
#endif
#ifdef _HP
		strcat(final_path_buffer, ".sl");
#endif
	}

#if defined ( _SOLARIS) || defined ( _AIX )
	m_hDllHandle = dlopen(final_path_buffer, RTLD_LAZY | RTLD_GLOBAL);
#endif
#ifdef _HP
	m_hDllHandle = shl_load(final_path_buffer, BIND_DEFERRED, 0L);
#endif

	if (m_hDllHandle == NULL)
	{
	// Bg 127160. No lo mostramos en release puesto que aún no es un error.
#ifdef _DEBUG
		cerr << "Cannot open: " << final_path_buffer << ". " << strerror( errno ) << endl ;
#endif
		/* MCP: ver nota en encabezado */
		// Ahora hacemos el segundo intento, concatenando el prefijo lib a la dll
		// creo el nuevo nombre.
		sprintf (final_path_buffer, "%slib%s", inputdirname, inputfname);

		if (strchr(final_path_buffer, '.') == NULL)
		{
#if defined ( _SOLARIS)
			strcat(final_path_buffer, ".so");
#endif
#if defined ( _AIX )
			strcat(final_path_buffer, ".a");
#endif
#ifdef _HP
			strcat(final_path_buffer, ".sl");
#endif
		}

#ifdef _DEBUG
		cerr << "Trying: " << final_path_buffer << endl;
#endif
		
		// vuelvo a intentar cargar la dll.
#if defined ( _SOLARIS) || defined ( _AIX )
		m_hDllHandle = dlopen(final_path_buffer, RTLD_LAZY | RTLD_GLOBAL);
#endif
#ifdef _HP
		m_hDllHandle = shl_load(final_path_buffer, BIND_DEFERRED, 0L);
#endif

		if (m_hDllHandle == NULL)
		{
			cerr << "Cannot open: " << final_path_buffer << ". " << strerror( errno ) << endl ;
			return M4_ERROR;
		}
		/**/
	}

	m_bIsOpenDll = M4_TRUE;

	return M4_SUCCESS ;

  //## end M4ClLoaDll::Open%884371570.body
}

m4return_t M4ClLoaDll::Close ()
{
  //## begin M4ClLoaDll::Close%884371571.body preserve=yes

/* JRM NO SE DEBE USAR BOOL EN NINGÚN CASO.
 *	Se debe usar m4bool_t. Si por alguna extraña razón no se 
 * pudiera usar m4bool_t, se debería usar bool, que existe en C++.
 * BOOL es específico de Windows.
 * Además, en esta rutina no tiene sentido usar una variable 
 * booleana de retorno.
 */
//	BOOL ret;
	if (m_bIsOpenDll != M4_TRUE)
	{
		return M4_ERROR;
	}

#if defined ( _SOLARIS)  || defined ( _AIX )
	if (dlclose(m_hDllHandle) != 0)
	{
		return M4_ERROR;
	}
#endif
#ifdef _HP
	if (shl_unload(m_hDllHandle) == -1)
	{
		return M4_ERROR;
	}
#endif

/*JRM Comento este código ya que es incorrecto. 
 *	En Solaris, si dlclose devuelve 0, es que ha funcionado 
 * bien, En HPUX, si devuelve != -1, es que ha funcionado bien.
 * Además, ret almacenaba el retorno de una llamada a dllclose,
 * que devuelve un entero, no un booleano. Por tanto, la asignación estaba 
 * mal.
 */
/*
	ret = dlclose(m_hDllHandle);
	if (ret==M4_FALSE)
	{
		return M4_ERROR;
	}
*/

	m_hDllHandle = NULL;

	m_bIsOpenDll = M4_FALSE;

	return M4_SUCCESS;

  //## end M4ClLoaDll::Close%884371571.body
}

m4return_t  M4ClLoaDll::GetFunc (m4pcchar_t ai_pccNameFunc, 
								 m4typefunreturn   &ao_AddressFunc)
{
  //## begin M4ClLoaDll::GetFunc%884371572.body preserve=yes

	if (m_hDllHandle == NULL)
	{
		return M4_ERROR;
	}

#if defined ( _SOLARIS) || defined ( _AIX )
	if ((ao_AddressFunc = dlsym(m_hDllHandle , ai_pccNameFunc)) == NULL)
	{
		return M4_ERROR;
	}
#endif
#ifdef _HP
	if (shl_findsym(&m_hDllHandle, ai_pccNameFunc, TYPE_PROCEDURE, &ao_AddressFunc) == -1)
	{
		return M4_ERROR;
	}
#endif

	return M4_SUCCESS;

  //## end M4ClLoaDll::GetFunc%884371572.body
}

// Additional Declarations
  //## begin M4ClLoaDll.declarations preserve=yes
  //## end M4ClLoaDll.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
