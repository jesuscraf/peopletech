//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           clfilemap.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             26 jan 98
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Map of Opened Files.
////
//==============================================================================


#ifndef __CLFILEMAP_HPP
#define	__CLFILEMAP_HPP



#include "m4stl.hpp"
#include "m4dm_dll.hpp"
#include "m4unicode.hpp"

class ClOpenFile;
class ClSize;


#define M4_MAX_NUMBER_OF_OPEN_FILES							20


typedef map < m4int32_t, ClOpenFile *, less < m4int32_t > >	FileTable ;
typedef FileTable::iterator									itFileTable ;





class M4_DECL_M4DM ClOpenFile {


	ifstream				*m_pfile;
	m4pchar_t				m_pszPath;
	streampos				m_iPos;
	eUniFileEncodingType_t	m_eEncoding;

public:
	
	ClOpenFile (ifstream *ai_pfile, m4pcchar_t ai_pszPath, eUniFileEncodingType_t ai_eEncoding )
	{
		m_pfile = ai_pfile;
		m_pszPath = new m4char_t [strlen(ai_pszPath) + 1];
		strcpy( m_pszPath, ai_pszPath );
		m_iPos = 0;
		m_eEncoding = ai_eEncoding;
	}

	~ClOpenFile()
	{
		M4_ASSERT(m_pszPath);
	
		delete [] m_pszPath;
		if(m_pfile)	
		{
			m_pfile->close();
			delete	m_pfile;
		}
	}

	ifstream *GetFile()
	{
		return m_pfile;
	}

	eUniFileEncodingType_t	GetEncoding()
	{
		return m_eEncoding;
	}

	ifstream *ReOpenFile()
	{
		m_pfile = new ifstream();
		// ios::in, hay que ponerlo por la implementacion en SOLARIS de los streams

		// UNICODE FILE
		m_eEncoding = M4UniANSI ;

		/* Bug 0130342
		Antes se abría en binario
		*/
		M4Open( *m_pfile, m_pszPath, M4UniReadBinary, m_eEncoding ) ;
			 
		if( m_pfile ->rdbuf()->is_open() )
		{
			m_pfile->ignore(m_iPos);
		}
		else 
		{
			delete m_pfile;
			m_pfile = NULL;
		}
		return m_pfile;

	}
	
	m4return_t CloseFileWODeleting()
	{
		if(m_pfile)
		{
			m_iPos = m_pfile->tellg() ;
			m_pfile->close();
			delete m_pfile;
			m_pfile = NULL;

			return M4_SUCCESS ;
		}
		else
		{
			return M4_WARNING;
		}
	}
};




class M4_DECL_M4DM ClFileTable {

	FileTable	m_oFileTable;
#ifdef _DEBUG
	m4int32_t	m_iNumHandles ;
#endif
	m4int32_t	m_iNumOpenFiles ;
	m4int32_t	m_iLastHandle;



	m4return_t	CloseFiles(m4int32_t ai_iNumFiles = 1);


public:

	ClFileTable()
	{
#ifdef _DEBUG
		m_iNumHandles =0;
#endif
		m_iNumOpenFiles =0;
		m_iLastHandle = 1;
	}


	m4return_t GetFile(m4int32_t ai_handle, ifstream* &ao_rpfFfile, eUniFileEncodingType_t& ao_reEncoding)
	{
		m4return_t	ret = M4_ERROR;
		itFileTable	itTemp;

		ao_reEncoding = M4UniANSI ;

		if( ( itTemp = m_oFileTable.find( ai_handle ) ) != m_oFileTable.end() )
		{
			if( (ao_rpfFfile = (*itTemp).second->GetFile()) != NULL )
			{
				ao_reEncoding = (*itTemp).second->GetEncoding() ;
				ret = M4_SUCCESS ;
			}
			else 
			{
				CloseFiles();
		
				ret = ( ao_rpfFfile = (*itTemp).second->ReOpenFile()  ) ? M4_SUCCESS : M4_ERROR ;

				if(ret == M4_SUCCESS)
				{
					ao_reEncoding = (*itTemp).second->GetEncoding() ;
					m_iNumOpenFiles++;
				}
				else 
				{
					/// ERROR
				}
			}
		}
		else 
		{
			/// ERROR
		}

		return ret ;
	}

	m4return_t InsertFile(m4pcchar_t ai_path, m4int32_t &ai_handle)
	{
		m4return_t ret = M4_ERROR;

		while( m_iNumOpenFiles > M4_MAX_NUMBER_OF_OPEN_FILES )
		{
			/// Si hay más ficheros abiertos de los permitidos, cerramos los que se abrieron primero.
			if( CloseFiles(m_iNumOpenFiles - M4_MAX_NUMBER_OF_OPEN_FILES	) == M4_ERROR ) // Si devuelve error, no podemos insertar más. Razón desconocida
			{
				/// ERROR
				return M4_ERROR ;
			}
		}


		ifstream * pfile = new ifstream();
		
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;

		/* Bug 0130342
		Antes se abría en binario
		*/
		M4Open( *pfile, ai_path, M4UniReadBinary, eEncoding ) ;
				
		if( pfile->rdbuf()->is_open() )
		{
			ClOpenFile	*pOpenFile = new ClOpenFile( pfile, ai_path, eEncoding );

//			No puedo devolver el handle del fichero, porque si lo cierro y lo
//			vuelvo  a abrir, me cambia.
//			ai_handle = pfile->fd();
			ai_handle = m_iLastHandle;

			if( m_oFileTable.insert( FileTable::value_type(ai_handle, pOpenFile) ).second == M4_TRUE )
			{
				ret = M4_SUCCESS ;
				m_iNumOpenFiles ++;
#ifdef _DEBUG
				m_iNumHandles++;
#endif
				m_iLastHandle++;
			}
			else 
			{
				/// ERROR
			}	
		}
		else 
		{
			/// ERROR
		}

		return ret ;
	}

	m4return_t RemoveFile(m4int32_t ai_handle)
	{
		m4return_t ret = M4_ERROR;
		itFileTable		itTemp;

		if( ( itTemp = m_oFileTable.find( ai_handle ) ) != m_oFileTable.end() )
		{
			if( (*itTemp).second->GetFile() != NULL)
			{
				m_iNumOpenFiles--;
			}

			delete(*itTemp).second ;
			
			m_oFileTable.erase(itTemp) ;

#ifdef _DEBUG
			m_iNumHandles--;
#endif
			ret = M4_SUCCESS ;
		}
		return ret ;
	}

	~ClFileTable()
	{
		itFileTable	itTemp;
		while( !m_oFileTable.empty() )
		{
			itTemp = m_oFileTable.begin();

#ifdef _DEBUG
			m_iNumHandles--;
			if( (*itTemp).second->GetFile() != NULL )
			{
				m_iNumOpenFiles--;
			}
#endif
			delete (*itTemp).second ;
			m_oFileTable.erase(itTemp);
		}

#ifdef _DEBUG
		M4_ASSERT( !m_iNumHandles ) ;
#endif
		M4_ASSERT( !m_iNumOpenFiles ) ;
	}

	m4return_t GetSize(ClSize &ao_size);

};

#endif
