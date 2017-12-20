//=============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4ini.hpp
// Project:          libini
// Author:           Meta Software M.S. , S.A
// Date:             Oct-7-1997
// Language:         C++
// Operating System: UNIX (SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//=============================================================================


#ifndef  _INI_HPP_
#define  _INI_HPP_


//__________________________________________________________
//
//  Includes
//__________________________________________________________


#include "m4ini_dll.hpp"
#include "m4stl.hpp"
#include "m4string.hpp"
#include "m4win.hpp"



//__________________________________________________________
//
//  Defines
//__________________________________________________________


//__________________________________________________________
//
//  Functions
//__________________________________________________________


void change_to_lower(char *p);

void SetCaseSensitive ( m4bool_t ai_bCaseSensitive ) ;

//__________________________________________________________
//
//   Clase Field
//
//	 Consiste en un nombre de campo, y un conjunto 
//   de nombres de propiedades y su valor correspondiente
//__________________________________________________________

typedef less<M4ClString> lessM4ClString;

typedef multimap< M4ClString, M4ClString, lessM4ClString > ClBlock;

typedef ClBlock::iterator IClBlock;

//____________________________________________________________
//
//
// NOTE: This ClCursor_sql class is just given as an example and all
// possible operations on a ClCursor_sql have not been defined.
//____________________________________________________________

typedef map<M4ClString, ClBlock, lessM4ClString>  m4blocks_t;

// Iterator type
typedef m4blocks_t::iterator Im4blocks_t;




// Store the ini file
class M4_DECL_M4INI Clini:public m4blocks_t
{
protected:
  
  M4ClString m_name;
  m4bool_t load_file_flag;

protected:

  // Funciones privadas


  // Funciones publicas
  //_____________________

public:
    
  Clini() 
  {
    load_file_flag = M4_FALSE;
  };


  virtual m4return_t reset();

  m4bool_t IsLoaded( void ) const
  {
	  return load_file_flag;
  }

  ClBlock::iterator find(const M4ClString &block_name,
			 const M4ClString &property_name);
 

  // Check the iterator returned by the previous function "find" and
  // return the value of the searched property

  m4bool_t return_value(const M4ClString &block_name,
			ClBlock::iterator &property, M4ClString &value);

  virtual ~Clini()  { reset(); }

  //_______________
  //
  //  Obtener miembros
  //_______________

  virtual void *Get (const m4char_t* ai_param) const
  {   
    if ( strcmp(ai_param, "ClBlock::m_name") == 0)
      return (void *)(&m_name);
    else
      return NULL; 
  }


  virtual m4return_t Set (const m4char_t* ai_param, void *ai_value)
  {   
    if ( strcmp(ai_param, "ClBolck::m_name") == 0) {
      m_name  = ((m4char_t*)(ai_value));
      return M4_SUCCESS;
    }
    else
      return M4_ERROR;
  }


  //_______________
  //
  //  Funciones de comparación
  //_______________

	
  //_______________
  //
  //  Funciones propias
  //_______________

  
  // añade o modifica un elemento
  //  m4return_t Put_element( m4char_t*, m4char_t*, m4char_t*) {
  m4return_t Put_element(M4ClString &block_name, M4ClString &property_name, 
			 M4ClString &value_name);

  // Añade o modifica un elemento (solo un bloque sin propiedades ni valores
  //  m4return_t Put_element( m4char_t*, m4char_t*, m4char_t*) {
  m4return_t Put_element(M4ClString &block_name);

  // obtiene los datos de un fichero
  // argumento el nombre del fichero
  m4return_t LoadFile( m4char_t* );
  
  // vuelca los datos a un fichero
  m4return_t FlushFile( m4char_t*);
  
  m4return_t Put_name( m4char_t* ai_name)
  {
    m_name = ai_name;
    return M4_SUCCESS;
  }
  

  //_______________
  //
  //  streams
  //_______________

  friend istream& operator >> (istream&, Clini &);
  friend ostream& operator << (ostream&, Clini &);

}; 



#ifdef _UNIX

//__________________________________________________________
//
//  Functions for Windows ".ini" file on UNIX
//__________________________________________________________


// The GetPrivateProfileInt function retrieves an integer
// associated with a key in the specified section of the
// given initialization file

M4_DECL_M4INI UINT GetPrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName,
			 INT nDefault, LPCTSTR lpFileName );


// The GetPrivateProfileSection function retrieves all
// of the keys and values for the specified section from
// an initialization file

M4_DECL_M4INI DWORD GetPrivateProfileSection(LPCTSTR lpAppName, LPTSTR lpReturnedString,
			       DWORD nSize, LPCTSTR lpFileName);


// The GetPrivateProfileSectionNames function retrieves
// the names of all sections in an initialization file

M4_DECL_M4INI DWORD GetPrivateProfileSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize, 
				    LPCTSTR lpFileName);


// The GetPrivateProfileString function retrieves a
// string from the specified section in an initialization file

M4_DECL_M4INI DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, 
			      LPCTSTR lpDefault, LPTSTR lpReturnedString, 
			      DWORD nSize, LPCTSTR lpFileName);

#endif	//_UNIX

#endif	//_INI_HPP_



