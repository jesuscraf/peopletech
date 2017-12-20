//	=============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4dmsVerity
//	 File:              m4extsearch.hpp
//	 Project:           DMS Verity API
//	 Author:            Meta Software M.S. , S.A
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

#ifndef __HPP_EXTENDED_SEARCH__
	#define __HPP_EXTENDED_SEARCH__

#include <m4types.hpp>
#include "m4stl.hpp"
#include <syncro.hpp>
#include "file_finder.hpp"


#ifdef _UNIX 
	#define SEPARATOR	"/"
	#define SEPARATOR_CHAR	'/'
#else	// _WINDOWS
	#define SEPARATOR	"\\"
	#define SEPARATOR_CHAR	'\\'
#endif


class  ClExtSearch
{

	public:
		ClExtSearch ();
		
		~ClExtSearch ();


		///////////////////////////////////////////////////////
		// Métodos
		///////////////////////////////////////////////////////
		m4return_t InitExtSearch (const string& ai_sBasePath, const string& ai_sFileName, const m4date_t& ai_tLastModified, const m4bool_t ai_bDirOnly, const m4int32_t ai_iMaxDeep);
		
		m4return_t GetNextFile (string& ao_sNextFileName);

		m4return_t CreateFileFinder (const string& ai_sPath);

		m4return_t NewDirFinder (const string& ai_sPath);


	private:

		// Directorio sobre el que se realiza la búsqueda.
		string m_sBasePath;
		
		// Condición del nombre para realizar la búsqueda.
		string m_sFileName;
		
		// Condición de fecha para realizar la búsqueda.
		m4date_t m_tLastModified;
		
		// Flag que indica que la búsqueda se ha de realizar recursivamente por todos
		// los directorios.
		//m4bool_t m_bRecurse;
		
		// Flarg que indica que sólo se deben devolver los nombres de los directorios.
		m4bool_t m_bDirOnly;

		// Profundidad de directorios en que se ha de entrar.
		m4int32_t m_iMaxDeep;

		// Contador de profundidad de directorios en que se va entrando.
		m4int32_t m_iDeepCount;

		// Estructura de búsqueda de ficheros.
		ClFileFinder *m_oFileFinder;

		// Vector de punteros a objetos de búsqueda. (Uno por cada subdirectorio.)
		typedef vector <ClFileFinder *>	vectFileFinder;
		vector <ClFileFinder *> m_vectFileFinder;

};




class  ClExtSearchManager
{

	public:
		ClExtSearchManager ();
		
		~ClExtSearchManager ();


		///////////////////////////////////////////////////////
		// Métodos
		///////////////////////////////////////////////////////
		m4return_t OpenExtSearch (const string& ai_sBasePath, const string& ai_sFileName, const m4date_t& ai_tLastModified, const m4bool_t ai_bDirOnly, const m4int32_t ai_iMaxDeep, m4int64_t& ao_hExtSearchHdl);
		
		m4return_t CloseExtSearch (const m4int64_t ai_hExtSearchHdl);
		
		ClExtSearch* AskExtSearch (const m4int64_t ai_hExtSearchHdl);

		m4return_t GetNextFile (const m4int64_t ai_hExtSearchHdl, string& ao_sNextFileName);


	private:

		// Contador de handlers a las estructuras de búsqueda extendida
		m4int64_t m_hExtSearchCount;
		
		// Mapa contenedor de estructuras de búsqueda extendida. map <handler, *puntero>
		typedef map< m4int64_t, ClExtSearch*, less<m4int64_t> > MapExtSearch;
		map< m4int64_t, ClExtSearch*, less<m4int64_t> > m_mapExtSearch;

		// Mutex para el bloqueo de threads en el acceso al mapa.
		ClMutex m_oExtSearchMutex;

};









#endif
