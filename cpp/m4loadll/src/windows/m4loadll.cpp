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

//## begin module.additionalDeclarations preserve=yes
#include <stdio.h>
#include <stdlib.h>
//## end module.additionalDeclarations


// Class M4ClLoaDll 



M4ClLoaDll::M4ClLoaDll ()
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.hasinit preserve=no
  //## end M4ClLoaDll::M4ClLoaDll%884371568.hasinit
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.initialization preserve=yes
  //## end M4ClLoaDll::M4ClLoaDll%884371568.initialization
{
  //## begin M4ClLoaDll::M4ClLoaDll%884371568.body preserve=yes
	m_bIsOpenDll =  M4_FALSE;
	m_hDllHandle = NULL;
  //## end M4ClLoaDll::M4ClLoaDll%884371568.body
}


M4ClLoaDll::~M4ClLoaDll ()
{
  //## begin M4ClLoaDll::~M4ClLoaDll%884371569.body preserve=yes

	if (m_bIsOpenDll == M4_TRUE)
	{
		Close();
	}

	//## end M4ClLoaDll::~M4ClLoaDll%884371569.body
}




//## Other Operations (implementation)
m4return_t M4ClLoaDll::Open (m4pcchar_t ai_pccNameDll)
{
  //## begin M4ClLoaDll::Open%884371570.body preserve=yes
/*
- Seg�n S.O, sustituir car�cter separador de directorio por el adecuado .
- Separar en path, nombre y extensi�n En Unix, se usan funciones basename y dirname, y  el nombre del fichero se separa  en nombre y extensi�n. En NT, se usa _splitpath.
- Si estamos en Unix, y si el path comienza por "x:"  siendo "x" cualquier car�cter, se sustituye por ".".
- Si no hay extensi�n, dependiendo de los defines indicadores de sistema (_HP, _SOLARIS, �), se elije  la extensi�n adecuada. 
- Separar el path en directorios. Sustituir aquellos directorios que comienzen por "$" por lo que contenga la variable de entorno de su mismo nombre, (exclu�da la letra $). Tanto en Unix como en NT, se usa la funci�n getenv.
- Construir el nombre completo de la librer�a a partir de los directorios, nombre de librer�a y extensi�n. En la construcci�n se tiene en cuenta que el nombre de un directorio puede ser vac�o, como consecuencia de que haya una referencia a una variable de entorno que no exista o vac�a.
-  Seguir con el c�digo original, pas�ndole como nombre de librer�a el resultante de los pasos anteriores. 
*/
#ifndef MAX_DIRECTORIES
#define MAX_DIRECTORIES	40
#endif

	char		path_buffer[_MAX_PATH];   
	char		final_path_buffer[_MAX_PATH];   
	char		drive[_MAX_DRIVE];   
	char		dir[_MAX_DIR];
	char		fname[_MAX_FNAME];   
	char		ext[_MAX_EXT];
	char		*directories[MAX_DIRECTORIES];
	int			num_directories = 0;
	m4bool_t	bFullPath;

	if (m_bIsOpenDll == M4_TRUE)
	{
		return M4_ERROR;
	}

	if (strlen(ai_pccNameDll) >= _MAX_PATH)
	{
#ifdef DEBUG
		cerr << "Nombre demasiado largo" <<endl;
#endif
		return M4_ERROR;
	}

	strcpy(path_buffer, ai_pccNameDll);

	char	*p = path_buffer;
	while ((p = strchr(p, '/'))!= NULL)
	{
		*p = '\\';
	}

	//Esta funci�n es de tipo void
	_splitpath(path_buffer, drive, dir, fname, ext);

	//Si tenemos variables de entorno, est�n todas en dir
	if (dir[0] == '\\')
	{
		bFullPath = M4_TRUE;
	}
	else
	{
		bFullPath = M4_FALSE;
	}

	if ((p = strtok(dir, "\\")) != NULL)
	{
		directories[num_directories++] = p;
		
		while((p = strtok(NULL, "\\")) != NULL && num_directories < MAX_DIRECTORIES)
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

	//Rehacer path completo
	strcpy(final_path_buffer, drive);
	if (bFullPath == M4_TRUE)
	{
		strcat(final_path_buffer, "\\");
	}

	for (i = 0; i < num_directories; i++)
	{
		if (directories[i] != NULL && directories[i][0] != '\0')
		{
			strcat(final_path_buffer, directories[i]);
			strcat(final_path_buffer, "\\");
		}
	}

	strcat(final_path_buffer, fname);

	if (strlen(ext) != 0)
	{
		strcat(final_path_buffer, ext);
	}
	else
	{
		strcat(final_path_buffer, ".dll");
	}

	m_hDllHandle=LoadLibrary(final_path_buffer);

	if (m_hDllHandle==NULL)
	{
		return M4_ERROR;
	}

	m_bIsOpenDll=M4_TRUE;

	return M4_SUCCESS ;

  //## end M4ClLoaDll::Open%884371570.body
}

m4return_t M4ClLoaDll::Close ()
{
  //## begin M4ClLoaDll::Close%884371571.body preserve=yes

	if (m_bIsOpenDll != M4_TRUE)
	{
		return M4_ERROR;
	}

	BOOL ret;

	ret=FreeLibrary(m_hDllHandle);

	if (ret == M4_FALSE)
	{
		return M4_ERROR;
	}

	m_hDllHandle = NULL;

	m_bIsOpenDll = M4_FALSE;

	return M4_SUCCESS;

  //## end M4ClLoaDll::Close%884371571.body
}

m4return_t  M4ClLoaDll::GetFunc (m4pcchar_t ai_pccNameFunc, m4typefunreturn   &ao_AddressFunc)
{
  //## begin M4ClLoaDll::GetFunc%884371572.body preserve=yes

	if (m_hDllHandle==NULL)
	{
		return M4_ERROR;
	}

	ao_AddressFunc=GetProcAddress(m_hDllHandle , ai_pccNameFunc);

	if (ao_AddressFunc==NULL)
	{
		return  M4_ERROR;
	}

	return M4_SUCCESS;

  //## end M4ClLoaDll::GetFunc%884371572.body
}

// Additional Declarations
  //## begin M4ClLoaDll.declarations preserve=yes
  //## end M4ClLoaDll.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
