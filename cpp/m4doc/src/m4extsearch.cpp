//	=============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4dmsVerity
//	 File:              m4extsearch.cpp
//	 Project:           DMS Verity API
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================


#include "m4docerrors.hpp"
#include "m4extsearch.hpp"

#define M4_MAX_NUM_EXT_SEARCH	500


// Class ClExtSearchManager
//-------------------------------------------------------------------------------------------
//	Constructor.
//-------------------------------------------------------------------------------------------
ClExtSearchManager::ClExtSearchManager ()
{
	// Inicializa las variables miembro.
	m_hExtSearchCount = 0;
	
	// Inicializa el mutex para el bloqueo de threads en secciones criticas.
	m_oExtSearchMutex.Init ();
}


//-------------------------------------------------------------------------------------------
//	Destructor.
//-------------------------------------------------------------------------------------------
ClExtSearchManager::~ClExtSearchManager ()
{
	m_oExtSearchMutex.Lock ();		////////////////// SECCION CRITICA ///////////////////
	
	MapExtSearch::iterator ite;
	MapExtSearch::iterator stop = m_mapExtSearch.end ();
	ClExtSearch *oExtSearch = 0;

	// Destruye tods los objetos contenidos en el mapa y después lo vacía.
	for (ite = m_mapExtSearch.begin(); ite != stop; ++ite)
	{
		// Puntero al objeto Extended Search.
		oExtSearch = (*ite).second;

		if(oExtSearch != 0)
		{
			// Destruimos el objeto.
			delete oExtSearch;
			oExtSearch = 0;
		}
	}
	// Vaciamos el mapa contenedor de estructuras sobre búsquedas extendidas.
	m_mapExtSearch.clear();

	m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////////

}


//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Se crea una nueva estructura de búsqueda extendida y se introduce en el mapa con su
// handle correspondiente.
//
// ENTRADA:
//		const string& ai_sBasePath			->	Directorio donde se realiza la búsqueda.
//		const string& ai_sFileName			->	Condición de búsqueda del nombre.
//		const m4date_t& ai_tLastModified	->	Condición de búsqueda de fecha.
//		const m4bool_t ai_bRecurse			->	Flag para búsqueda recursiva por los direct.
//		const m4bool_t ai_bDirOnly			->	Flag para la búsqueda de directorios.
//
// SALIDA:
//		m4int64_t& ao_hExtSearchHandle		->	Devuelve el handle a búsqueda extendida.
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearchManager::OpenExtSearch (const string& ai_sBasePath, const string& ai_sFileName, const m4date_t& ai_tLastModified, const m4bool_t ai_bDirOnly, const m4int32_t ai_iMaxDeep, m4int64_t& ao_hExtSearchHdl)
{
	// Inicializa el handle a Búsqueda extendida a cero.
	ao_hExtSearchHdl = 0;
	

	// Crea una nueva estructura de Búsqueda extendida.
	ClExtSearch *oExtSearch = new ClExtSearch;

	// Inicializa la Abre la sesión.
	if( oExtSearch->InitExtSearch (ai_sBasePath, ai_sFileName, ai_tLastModified, ai_bDirOnly, ai_iMaxDeep) == M4_ERROR )
	{
		// Destruye el objeto creado.
		delete oExtSearch;
		return M4_ERROR;
	}

		
	m_oExtSearchMutex.Lock ();		////////////////// SECCION CRITICA ///////////////////
	// Comprueba que no estén abiertas en número máximo Búsquedas extendidas.
	if( m_mapExtSearch.size () == M4_MAX_NUM_EXT_SEARCH )
	{
		m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////////
		// Destruye el objeto creado.
		delete oExtSearch;
		M4DOCErrorHandling (M4DOCERROR_MAX_NUM_EXT_SEARCH, "");
		return M4_ERROR;
	}
		

	// Se incrementa el contador de handles hasta que se encuentre uno libre.
	// Se trata de un contador circular.
	MapExtSearch::iterator ite;
	do
	{
		// Incremanta el valor del contador de Handles.
		if( m_hExtSearchCount < M4_MAX_NUM_EXT_SEARCH )	m_hExtSearchCount++;				
		else											m_hExtSearchCount = 1;
		
		// Comprueba que el handler no este asociado a ninguna otra Búsqueda extendida.
		ite = m_mapExtSearch.find (m_hExtSearchCount);
	}
	while( ite != m_mapExtSearch.end () ); // Mientras el handle pertenezca a otra BE.
	
	// Devuelve el handler a la búsqueda extendida creada.
	ao_hExtSearchHdl = m_hExtSearchCount;

	// Inserta en el mapa la nueva Búsqueda extendida (BE).
	m_mapExtSearch.insert ( MapExtSearch::value_type(m_hExtSearchCount, oExtSearch) ) ;
		
	m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////////
	
	return M4_SUCCESS;
}



//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Se destruye el objeto dado por el handle 'ai_hExtSearchHdl' y se elimina del mapa
// contenedor de búsquedas extendidas.
//
// ENTRADA:
//		const m4int64_t ai_hExtSearchHandle	->	Handle a búsqueda extendida.
//
// SALIDA:
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearchManager::CloseExtSearch (const m4int64_t ai_hExtSearchHdl)
{
	// Comprueba que sea un handle válido.
	ClExtSearch *oExtSearch = AskExtSearch (ai_hExtSearchHdl);
	if( oExtSearch == 0 )
	{
		return M4_ERROR;
	}

	m_oExtSearchMutex.Lock ();		////////////////// SECCION CRITICA ///////////////////

	// Elimina el handle y la ExtSearch del mapa contenedor.
	m_mapExtSearch.erase (ai_hExtSearchHdl);
	// Destruye la instancia del objeto dado por el handler 'ai_hExtSearchHdl'.
	delete oExtSearch;
	oExtSearch = 0;
	m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////////

	return M4_SUCCESS;
}




//-------------------------------------------------------------------------------------------
// DESCRIPCION:
// Comprueba que el handler a la Extended Search sea un handle válido.
// Devuelve M4_TRUE en caso de ser válido.
//
// ENTRADA:
//		const m4int64_t ai_hExtSearchHandle	->	Handle a búsqueda extendida.
//
// SALIDA:
//
//-------------------------------------------------------------------------------------------
ClExtSearch* ClExtSearchManager::AskExtSearch (const m4int64_t ai_hExtSearchHdl)
{

	ClExtSearch *oExtSearch = 0;

	m_oExtSearchMutex.Lock ();		////////////////// SECCION CRITICA ///////////////////

	// Compruebo si la búsqueda extendida está contenida en el mapa de sesiones.
	MapExtSearch::iterator ite;
	ite = m_mapExtSearch.find (ai_hExtSearchHdl);
	if ( ite == m_mapExtSearch.end () )
	{
		m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////
		// El handle a ExtSearch no es válido
		m4char_t cErrorMsg[50];sprintf (cErrorMsg, "%.16g", (m4double_t)ai_hExtSearchHdl);
		M4DOCErrorHandling (M4DOCERROR_EXTSEARCH_HDL_NO_VALID, cErrorMsg);
		return 0;
	}

	// Puntero al objeto Extended Search.
	oExtSearch = (*ite).second;

	m_oExtSearchMutex.Unlock ();	//////////////////////////////////////////////////////
	
	return oExtSearch;
}




//-------------------------------------------------------------------------------------------
// DESCRIPCION:
// Comprueba que el handle a la búsqueda extendida sea un handle válido y llama a la
// correspondiente función de la clase 'ClExtSearch'.
// Devuelve el siguiente fichero.
//
// ENTRADA:
//		const m4int64_t ai_hExtSearchHandle	->	Handle a búsqueda extendida.
//
// SALIDA:
//		string& ao_sNextFileName			->	Devuelve el siguiente fichero encontrado.
//
//-------------------------------------------------------------------------------------------
m4return_t ClExtSearchManager::GetNextFile (const m4int64_t ai_hExtSearchHdl, string& ao_sNextFileName)
{
	// Comprueba que sea un handle válido.
	ClExtSearch *oExtSearch = AskExtSearch (ai_hExtSearchHdl);
	if( oExtSearch == 0 )
	{
		return M4_ERROR;
	}

	// Obtiene el siguiente fichero.
	if( oExtSearch->GetNextFile (ao_sNextFileName) == M4_ERROR )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}







// Class ClExtSearch
//-------------------------------------------------------------------------------------------
//	Constructor.
//-------------------------------------------------------------------------------------------
ClExtSearch::ClExtSearch ()
{
	// Inicializa las variables miembro.
	
	// Puntero a la estructura de búsqueda de ficheros.
	m_oFileFinder = 0;
	
	// Contador de profundidad en directorios.
	m_iDeepCount = 0;

	// Condición de fecha.
	m_tLastModified = 0;
	
	// Flag de búsqueda de directorios.
	m_bDirOnly = M4_FALSE;

}



//-------------------------------------------------------------------------------------------
//	Destructor.
//-------------------------------------------------------------------------------------------
ClExtSearch::~ClExtSearch ()
{
	// Recorre todos las estructuras de búsqueda del mapa y las destruye.
	m4int32_t iFin = m_vectFileFinder.size ();
	for (m4int32_t i=0; i<iFin; i++)
	{
		// Toma el puntero a la estructura de búsqueda.
		ClFileFinder *oFileFinder = m_vectFileFinder[i];
		// Destruye el puntero a la estructura de búsqueda.
		delete oFileFinder;
		oFileFinder = 0;
	}
	
	// Vacía el vector contenedor de estructuras de búsqueda.
	m_vectFileFinder.clear ();

	// Destruye la estructura de búsqueda de ficheros.
	if(m_oFileFinder != 0)
		delete m_oFileFinder;
}



//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Inicializa los parámetros para realizar la búsqueda extendida.
//
// ENTRADA:
//		const string& ai_sBasePath			->	Directorio donde se realiza la búsqueda.
//		const string& ai_sFileName			->	Condición de búsqueda del nombre.
//		const m4date_t& ai_tLastModified	->	Condición de búsqueda de fecha.
//		const m4bool_t ai_bRecurse			->	Flag para búsqueda recursiva por los direct.
//		const m4bool_t ai_bDirOnly			->	Flag para la búsqueda de directorios.
//
// SALIDA:
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearch::InitExtSearch (const string& ai_sBasePath, const string& ai_sFileName, const m4date_t& ai_tLastModified, const m4bool_t ai_bDirOnly, const m4int32_t ai_iMaxDeep)
{
	// Directorio sobre el que se realiza la búsqueda.
	m_sBasePath = ai_sBasePath;
	
	// Condición de búsqueda sobre el nombre.
	if (ai_sFileName != "")
		m_sFileName = ai_sFileName;
	else
		m_sFileName = "*";
	
	// Condición de fecha.
	m_tLastModified = ai_tLastModified;
	
	// Flag de búsqueda de directorios.
	m_bDirOnly = ai_bDirOnly;

	// Máxima profundidad de directorios.
	if(ai_iMaxDeep >= 0)
		m_iMaxDeep = ai_iMaxDeep;
	else
		m_iMaxDeep = 0;


	// Crea una estructura de búsqueda en el path base.
	CreateFileFinder (m_sBasePath);

	// Introduce en el vector de búsqueda de directorios la estruct. de búsqueda sobre el
	// directorio base.
	NewDirFinder (m_sBasePath);

	return M4_SUCCESS;
}




//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Cada llamada a esta función devuelve el nombre del siguiente fichero localizado que cumple
// las condiciones de nombre y fecha, en las estructuras de búsqueda almacenadas en el vector.
//
// ENTRADA:
//
// SALIDA:
//		string& ao_sNextFileName	->	Devuelve el siguiente fichero encontrado.
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearch::GetNextFile (string& ao_sNextFileName)
{
	// Inicializa el string de salida a "".
	ao_sNextFileName.erase ();
	
	// Objeto auxiliar donde se recoje información sobre el fichero actual.
	ClFile oFile;
	
	// Busca el archivo a directorio siguiente mientras no se acaben ni sea un directorio.
	while (ao_sNextFileName == "")
	{
		// Comprueba si ya no queda ningún fichero en el directorio actual.
		if (m_oFileFinder->FindNext (oFile) == M4_ERROR)
		{
			// Comprueba si quedan directorios por mirar.
			if (m_vectFileFinder.size () == 0)
				break;

			// Toma la útima estructura de búsqueda del vector.
			ClFileFinder *oDirFinder = m_vectFileFinder.back ();
				
			// Comprueba si quedan más directorios por mirar.		
			if (oDirFinder->FindNext (oFile) == M4_ERROR || (m_iMaxDeep != 0 && m_iDeepCount >= m_iMaxDeep))
			{
				// Decrementa el contador de profundidad de directorios.
				m_iDeepCount--;
				// Cierra la estructura de búsqueda y la elimina del mapa.
				delete oDirFinder;
				m_vectFileFinder.pop_back ();
			}
			else
			{
				// Comprueba si se trata de un fichero o un subdirectorio.
				if (oFile.IsDir() == M4_TRUE)
				{
					// Es un directorio.
					// Crea una estructura de búsqueda en el path dado.
					CreateFileFinder (oFile.GetFileName());

					// Introduce en el vector de búsqueda de directorios la estruct. de 
					// búsqueda sobre el directorio dado.
					NewDirFinder (oFile.GetFileName());
				}
			}
		}
		else
		{
			// Comprueba que la fecha de escritura del fichero sea posterior a la dada.
			if (oFile.GetLastWrite() >= m_tLastModified )
			{
				// Comprueba si se ha de devolver ficheros.
				if (m_bDirOnly == M4_FALSE)
				{
					// Comprueba si se trata de un fichero o un subdirectorio.
					if (oFile.IsDir() == M4_FALSE)
					{
						// Es un fichero, devuelve el nombre del fichero.
						ao_sNextFileName = oFile.GetFileName();
					}
				}
				else
				{
					// Comprueba si se trata de un fichero o un subdirectorio.
					if (oFile.IsDir() == M4_TRUE)
					{
						// Es un directorio, devuelve el nombre del fichero.
						ao_sNextFileName = oFile.GetFileName();
					}
				}
			}
		}
	}
	return M4_SUCCESS;
}






//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Crea una nueva estructura de búsqueda en el path especificado con la condición de búsqueda 
// dada por 'm_sFileName'.
//
// ENTRADA:
//		const string& ai_sPath		->	Path donde se crea la estructura de búsqueda.
//
// SALIDA:
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearch::CreateFileFinder (const string& ai_sPath)
{
	//----------------------------------------------------------------------------------------
	// Estructura de búsqueda.
	//----------------------------------------------------------------------------------------
	// Compone el nombre del path mas la condición de búsqueda.
	string sPathName = ai_sPath + SEPARATOR + m_sFileName;

	// Path en donde se realiza la búsqueda.
	ClFileName oFileName (sPathName.c_str ());

	// Destruye la estructura de búsqueda y crea una nueva.
	if(m_oFileFinder != 0)
	{
		delete m_oFileFinder;
	}

	// Crea la nueva estructura de búsqueda en el subdirectorio.
	m_oFileFinder = new ClFileFinder(oFileName.GetFileName());
	//----------------------------------------------------------------------------------------
	
	return M4_SUCCESS;
}





//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Crea una nueva estructura de búsqueda en el path especificado con la condición de búsqueda 
// dada por 'ai_sFileName'. 'ai_sFileName' es un argumento que por defecto es "*".
// Crea una nueva estructura de búsqueda sobre directorios en el path especificado y la 
// inserta en el vector de la clase.
//
// ENTRADA:
//		const string& ai_sPath		->	Path donde se crea la estructura de búsqueda.
//
// SALIDA:
//
//--------------------------------------------------------------------------------------------
m4return_t ClExtSearch::NewDirFinder (const string& ai_sPath)
{
	// Incrementa el contador de profundidad de directorios.
	m_iDeepCount++;

	//----------------------------------------------------------------------------------------
	// Estructura de búsqueda.
	//----------------------------------------------------------------------------------------
	// Compone el nombre del path mas la condición de búsqueda de directorios.
	string sPathName = ai_sPath + SEPARATOR + "*";

	// Path en donde se realiza la búsqueda.
	ClFileName oFileName (sPathName.c_str ());
				
	// Crea una nueva estructura de búsqueda en el subdirectorio y la introduce en el vector.
	ClFileFinder *oNewFileFinder = new ClFileFinder(oFileName.GetFileName());
	m_vectFileFinder.push_back (oNewFileFinder);
	//----------------------------------------------------------------------------------------
	
	return M4_SUCCESS;
}
