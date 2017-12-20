//=============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ini.cpp
// Project:          libini
// Author:           Meta Software M.S. , S.A
// Date:             18-Feb-1998
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


//__________________________________________________________
//
//  Includes
//__________________________________________________________

#include "m4ini.hpp"

#include "m4types.hpp"
#include "m4trace.hpp"
#include "m4unicode.hpp"
#include "m4stl.hpp"
#include "syncro.hpp"
#include "blocksynchronization.hpp"


extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}


//__________________________________________________________
//
//  Defines
//__________________________________________________________

#define IGNORE_TABUL(X)  while( *X==' ' || *X=='\t') X++;
#define IGNORE_TABUL_BACK(X)  while( *X==' ' || *X=='\t') X--; X++;
#define PASSUNTIL(X, Y)  while( *X!='\0' && *X!=Y) X++;

#define MAX_TAM_LINE  2048 // jcr con 2K vale para no desbordar la pila



m4bool_t g_bLowerCaseVariables = M4_TRUE ;

///  Funciones generales de tratamiento de cadenas
/////////////////////////
void SetCaseSensitive ( m4bool_t ai_bCaseSensitive ) 
{
	g_bLowerCaseVariables = ! ai_bCaseSensitive ;
}


m4return_t ReadName (m4char_t *name, M4ClString &ao_name)
{
  m4return_t retcode = M4_ERROR;
  m4char_t *init = NULL;
  m4char_t *end = NULL;


  init = name;
  while (*init != ';' && *init != '[' && *init != '\0' && *init != '\'')
    {
      init ++;
    }

  if (*init == '[')
    {
      init++;	 /// quitamos el primer corchete
      /// quitamos también el segundo corchete
      end = init;
      while (*end != ']' && *end != '\0')
	{
	  end++;
	}
	
      if ( *end == ']')
	{
	  *end = '\0';
	  ao_name = init;
	  retcode = M4_SUCCESS;
	}
      else
	{
	  retcode = M4_ERROR;
	}
    }
  if (*init == ';' || *init == '\0' || *init == '\'')
    retcode = M4_WARNING;

  return retcode;
}


m4return_t ReadAnElement(m4char_t *pcbuffer, M4ClString &ao_name, M4ClString &ao_field)
{
  m4char_t *name_init;
  m4char_t *field_init;
  m4char_t *pcPunt = NULL;
  m4bool_t bfield;   /// dice si existe campo field o no
  m4return_t retcode = M4_SUCCESS;
	
  /// quitamos todos los tabuladores
  pcPunt = pcbuffer;
  IGNORE_TABUL(pcPunt);
  name_init = pcPunt;

  PASSUNTIL(pcPunt, '=');
  field_init = pcPunt;
  bfield = (*field_init == '=') ? M4_TRUE: M4_FALSE;
  *pcPunt = '\0';   /// cortamos el nombre en el sitio justo
	
  if (*name_init == '[')
    {
      /// acabamos de encontrar otra cabecera
      return M4_WARNING;
    }
  else if (strlen(name_init) > 0)
    {    /// ajustamos el extremo final y grabamos
      pcPunt--;
      IGNORE_TABUL_BACK(pcPunt);
      *pcPunt = '\0';

      ao_name  = name_init;
    }
  else
    {
      retcode = M4_WARNING;
    }

  if (bfield)
    {
      field_init++;
      IGNORE_TABUL(field_init);
      pcPunt = field_init;

      //      PASSUNTIL(pcPunt, ';');
      PASSUNTIL(pcPunt, '\0');
      pcPunt--;
      IGNORE_TABUL_BACK(pcPunt);
      *pcPunt = '\0';   // cortamos el nombre en el sitio justo

      /// ya tenemos bien delimitada la cadena
	
      ao_field = field_init;
    }
  return retcode;
}



//_______________________
//
//    Clini
//_______________________


m4return_t Clini::reset()
{  
	m4return_t retcode = M4_SUCCESS;
	m4blocks_t::iterator block;
	ClBlock::iterator property;


	for (block = begin(); block != end(); block++)
	  ((*block).second).erase(((*block).second).begin(),
				  ((*block).second).end());
	  erase(begin(), end());

	m_name = "" ;

	return retcode;
}

ClBlock::iterator  Clini::find(const M4ClString &block_name,
							   const M4ClString &property_name)
{
	m4blocks_t::iterator block;
	ClBlock::iterator property;
	
	
	if (block_name.size() && property_name.size()) {
		if (block_name == "*") {
			for (block = begin(); (block != end()) &&
				((property = ((*block).second).find(property_name)) != 
				((*block).second).end()) ; block++);
		}
		else {
			
			// Search the requested block
			block = m4blocks_t::find(block_name);
			
			if (block != end())
				
				// Obtain the iterator to the requested property
				property = (*block).second.find(property_name);
			else {
				block--;
				property = (*block).second.end();
			}
		}
	}
	else {
		
		// The defect value of "property" is the "end()" of the
		// last block
		block = end();
		block--;
		property = (*block).second.end();
	}
	return property;
}

// Check the iterator returned by the previous function "find" and
// return the value of the searched property

m4bool_t Clini::return_value(const M4ClString &block_name,
					  ClBlock::iterator &property, M4ClString &value)
{
    Im4blocks_t block;
    m4bool_t result = M4_FALSE;
	
	
    if (block_name.size()) {
		if (block_name == "*") {
			block = begin();
			if ((*block).second.end() != property) {
				value = (*property).second;
				result = M4_TRUE;
			}
		}
		else {
			block = m4blocks_t::find(block_name);
			if (block != end()) {
				
				// Obtain the value of the property
				if (property != (*block).second.end()) {
					value = (*property).second;
					result = M4_TRUE;
				}
			} // if (block != end())
		} // else
    } // if (block_name.size())
    return result;
}

// añade o modifica un elemento
//  m4return_t Put_element( m4char_t*, m4char_t*, m4char_t*) {
m4return_t Clini::Put_element(M4ClString &block_name, M4ClString &property_name, 
					   M4ClString &value_name)
{
    m4return_t result = M4_SUCCESS;
    Im4blocks_t block_iterator;
    char block_str[512];
    char property_str[512];
    char value_str[512];
	
	
    if (block_name.size() && property_name.size() && value_name.size()) {
		
		// Chage the names to lower case 
		strcpy(block_str, block_name);
		change_to_lower(block_str);
		
		strcpy(property_str, property_name);
		change_to_lower(property_str);
		
		// Change the value to lower case if there is not double quotes
		
		strcpy(value_str, value_name.c_str());
		
		if (value_name.find_first_of("\"") == M4_STR_NPOS)
		change_to_lower(value_str);
		
		if ((block_iterator = m4blocks_t::find(block_str)) != end()) {
			
			// Insert property and value in the block
			(*block_iterator).second.insert(ClBlock::value_type(property_str,
				value_str));
		}
		else {
			
			// Create a new block
			ClBlock block;
			
			// Insert property and value in the block
			
			block.insert(ClBlock::value_type(property_str, value_str));
			
			// Insert the new block in the map
			insert(m4blocks_t::value_type(block_str, block));	
		}
    }
    else
		result = M4_ERROR;
    return result;    
}

// Añade o modifica un elemento (solo un bloque sin propiedades ni valores
//  m4return_t Put_element( m4char_t*, m4char_t*, m4char_t*) {
m4return_t Clini::Put_element(M4ClString &block_name)
{
    m4return_t result = M4_SUCCESS;
    Im4blocks_t block_iterator;
    char block_str[512];
    //char property_str[512];
    //char value_str[512];
	
	
    if (block_name.size()) {
		
		
		// Chage the names to lower case 
		strcpy(block_str, block_name);
		change_to_lower(block_str);
		
		if ((block_iterator = m4blocks_t::find(block_str)) == end()) {
			
			// Create a new block
			ClBlock block;
			
			// Insert the new block in the map
			
			insert(m4blocks_t::value_type(block_str, block));	
		}
    }
    else
		result = M4_ERROR;
    return result;    
}

// obtiene los datos de un fichero
// argumento el nombre del fichero
m4return_t Clini::LoadFile( m4char_t *ai_pcfile)
{
  //MOD FJSM 99/06/16 
  //Permitir abrir varios ficheros ini.
  
  m4return_t retcode = M4_SUCCESS;

  if (strcmp(m_name.c_str(), ai_pcfile) != 0) 
  {
  	  ifstream fin;
  	//Tenemos que abrir un nuevo fichero
  	//Miramos si hay uno abierto, y si es asi descargamos
	  if (load_file_flag == M4_TRUE) {
	  	m4Trace (printf("Cerrando fichero ini, para abrir uno nuevo\n"));
		reset();
	  }
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		M4Open( fin, ai_pcfile, M4UniRead, eEncoding );

	    if (fin)
	    {
	      (void) Put_name(ai_pcfile);
	      load_file_flag = M4_TRUE;
	      fin >> *this;
	      fin.close();
	    }
	    else
		{
	      retcode = M4_ERROR;
		}
  }
  //DEL FJSM 99/06/16 
/*  else if (strcmp(m_name.c_str(), ai_pcfile) != 0) {
    retcode = M4_ERROR;
    cerr << "\nM4INI>> Error. There is a previous ini file loaded (file " 
	 << flush;
    cerr << ai_pcfile << ")\n\n" << flush;
  }*/
  return retcode;
}


// Vuelca los datos a un fichero
m4return_t Clini::FlushFile( m4char_t *ai_pcfile)
{
  m4return_t retcode = M4_SUCCESS;
  ofstream fout;


  // Truncamos todo lo que hubiera en ese fichero
  // UNICODE FILE
  eUniFileEncodingType_t eEncoding = M4UniANSI ;
  M4Open( fout, ai_pcfile, M4UniWrite, eEncoding );

  if (!fout)
    {
      retcode = M4_ERROR;
      goto END_LOAD;

      // Comentado por angellg
      //      BYE(END_LOAD, ai_pcfile);
    }

  fout << *this;

  fout.close();

END_LOAD:
  return retcode;
}


//_______________
//
//  streams
//_______________

// lee los ficheros .ini

istream& operator >> (istream& ai_is, Clini &ai_ini)
{
  m4char_t pcbuffer[MAX_TAM_LINE];
  m4return_t retcode = M4_SUCCESS;
  M4ClString name_block;
  M4ClString name_field;
  M4ClString value_field;


  ai_is.getline(pcbuffer, MAX_TAM_LINE);

  if( ai_is.eof())  retcode = M4_ERROR;
  while (!ai_is.eof() && retcode != M4_ERROR)
    {
      retcode = ReadName(pcbuffer, name_block);
      while ((retcode != M4_ERROR) && retcode != M4_WARNING)
	{
	  ai_is.getline(pcbuffer, MAX_TAM_LINE);

	  if( *pcbuffer != '\0' && *pcbuffer != ';' && *pcbuffer != '\'')   
	    {
	      // Recogemos todos los elementos de un bloque
	      retcode = ReadAnElement( pcbuffer, name_field, value_field);
	      if (retcode != M4_ERROR && retcode != M4_WARNING)
		ai_ini.Put_element( name_block, name_field, value_field);
	      else if (retcode != M4_ERROR && retcode == M4_WARNING &&
		       name_block.size())
		ai_ini.Put_element(name_block);
	    }
	  if( ai_is.eof())  
	    {
	      retcode = M4_ERROR;
	    }
	}
      if (retcode == M4_WARNING && *pcbuffer != '[')
	ai_is.getline(pcbuffer, MAX_TAM_LINE);
    }
  return ai_is;
}


ostream& operator << (ostream& ai_os, Clini &ini_obj)
{
  Clini::iterator block_iterator;
  ClBlock::iterator property_iterator;


  // Show all the blocks of the object "ini"
  for (block_iterator = ini_obj.begin(); block_iterator != ini_obj.end();
       block_iterator++) {
    ai_os << '[' << (*block_iterator).first << ']' << "\n";

    // Show all the properties of each block
    for (property_iterator = (*block_iterator).second.begin();
	 property_iterator != (*block_iterator).second.end();
	 property_iterator++) {
      ai_os << (*property_iterator).first 
	    << '=' << (*property_iterator).second;
      ai_os << '\n';
    }
  }
  return ai_os;
}



void change_to_lower(char *p)
{
  int i = 0;

  //Este cambio es para que podamos usar variables sin convertirlas a minúsculas
  if ( ! g_bLowerCaseVariables )
	  return ;


  for (i = 0; p[i] != '\0'; i++) 
    switch (p[i]) {
    case 'A':
      p[i] = 'a';
      break;
    case 'B':
      p[i] = 'b';
      break;
    case 'C':
      p[i] = 'c';
      break;
    case 'D':
      p[i] = 'd';
      break;
    case 'E':
      p[i] = 'e';
      break;
    case 'F':
      p[i] = 'f';
      break;
    case 'G':
      p[i] = 'g';
      break;
    case 'H':
      p[i] = 'h';
      break;
    case 'I':
      p[i] = 'i';
      break;
    case 'J':
      p[i] = 'j';
      break;
    case 'K':
      p[i] = 'k';
      break;
    case 'L':
      p[i] = 'l';
      break;
    case 'M':
      p[i] = 'm';
      break;
    case 'N':
      p[i] = 'n';
      break;
    case 'Ñ':
      p[i] = 'ñ';
      break;
    case 'O':
      p[i] = 'o';
      break;
    case 'P':
      p[i] = 'p';
      break;
    case 'Q':
      p[i] = 'q';
      break;
    case 'R':
      p[i] = 'r';
      break;
    case 'S':
      p[i] = 's';
      break;
    case 'T':
      p[i] = 't';
      break;
    case 'U':
      p[i] = 'u';
      break;
    case 'V':
      p[i] = 'v';
      break;
    case 'W':
      p[i] = 'w';
      break;
    case 'X':
      p[i] = 'x';
      break;
    case 'Y':
      p[i] = 'y';
      break;
    case 'Z':
      p[i] = 'z';
      break;
    case 'Á':
      p[i] = 'á';
      break;
    case 'É':
      p[i] = 'é';
      break;
    case 'Í':
      p[i] = 'í';
      break;
    case 'Ó':
      p[i] = 'ó';
      break;
    case 'Ú':
      p[i] = 'ú';
      break;
    case 'Ä':
      p[i] = 'ä';
      break;
    case 'Ë':
      p[i] = 'ë';
      break;
    case 'Ï':
      p[i] = 'ï';
      break;
    case 'Ö':
      p[i] = 'ö';
      break;
    case 'Ü':
      p[i] = 'ü';
      break;
    default:
      break;
    }
}

 
//_______________________
//
//    Windows ".ini" Interface
//_______________________


#ifdef _UNIX

typedef map< string, Clini*, less<string> >	MapInis_t;

// Factoría de Clini
class  CliniProvider
{

public:

		CliniProvider( void ) ;
		~CliniProvider( void ) ;

	Clini*	Get( m4pchar_t ai_pccFile ) ;

protected:

	MapInis_t	m_oInis;
	ClMutex		m_oMutex;
};

CliniProvider	s_oIniProvider;



CliniProvider::CliniProvider( void )
{
	m_oMutex.Init();
}

CliniProvider::~CliniProvider( void )
{
	MapInis_t::iterator it = m_oInis.begin();

	while( it !=  m_oInis.end() )
	{
		delete it->second;
		it++ ;
	}
	m_oInis.clear();
}


Clini*	CliniProvider::Get( m4pchar_t ai_pccFile )
{
	Clini *poIni = NULL;

	if( ai_pccFile == NULL || *ai_pccFile == '\0' )
	{
		return NULL;
	}

	ClMutBlock	oBlock( &m_oMutex ) ;

	MapInis_t::iterator it = m_oInis.find( ai_pccFile );

	if( it != m_oInis.end() )
	{
		// Si existe ya se tiene
		poIni = it->second;
	}
	else
	{
		// Si no existe se crea y se carga
		poIni = new Clini();

		if( poIni == NULL )
		{
			cerr <<  "\nM4INI>> Error: There is no memory to allocate " << sizeof( Clini ) << "bytes " << endl;
			return NULL;
		}

		poIni->LoadFile( ai_pccFile );
		m_oInis.insert( MapInis_t::value_type( ai_pccFile, poIni) );
	}

	if( poIni->IsLoaded() == M4_TRUE && poIni->size() > 0 )
	{
		// Sólo se devuelve el objeto si tiene datos
		return poIni;
	}

	return NULL;
}



// The GetPrivateProfileInt function retrieves an integer
// associated with a key in the specified section of the
// given initialization file

UINT GetPrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName,
			  INT nDefault, LPCTSTR lpFileName )
{
  //UINT result = nDefault;
  int result = nDefault;
  //Clini ini_str;
  ClBlock::iterator property;
  M4ClString value;
  m4char_t section_str[MAX_TAM_LINE];
  m4char_t property_str[MAX_TAM_LINE];


  // If there are parameters, go on 
  if (lpAppName && lpKeyName && lpFileName) {

	Clini *IniFilePtr = s_oIniProvider.Get( (m4pchar_t)lpFileName );

    // If the ini file is loaded and it is no void, go on
    if( IniFilePtr != NULL )
	{
      // Change the section name to lower case
      (void) strcpy(section_str, lpAppName);
      change_to_lower(section_str);

	  // Change the key name to lower case
      (void) strcpy(property_str, lpKeyName);
		change_to_lower(property_str);
	  
	  // Search section and key in the ini file
      property = IniFilePtr->find(M4ClString(section_str),
			      M4ClString(property_str));
	  
      if (IniFilePtr->return_value(section_str, property, value) == M4_TRUE)
	  {
	// RAC 270799
		 result = atoi(value.c_str());
      }
    } 
  }

  // The same behavior than Windows NT
  else if (lpAppName == NULL || lpKeyName == NULL)
    result = 0;

  return(UINT(result));
}




// The GetPrivateProfileSection function retrieves all
// of the keys and values for the specified section from
// an initialization file

DWORD GetPrivateProfileSection(LPCTSTR lpAppName, LPTSTR lpReturnedString,
			       DWORD nSize, LPCTSTR lpFileName )
{
  // BORRAR
  //  DWORD i = 0;
  DWORD nchar = 0;
  //Clini ini_str;
  m4blocks_t::iterator block;
  ClBlock::iterator property;
  DWORD str_size = 0;
  // BORRAR
  // m4char_t *p = NULL;
  m4char_t section_str[MAX_TAM_LINE];
  LPTSTR Property_ptr = NULL;


  // If there are parameters, go on 
  if (lpAppName && lpReturnedString && nSize && lpFileName) {

	Clini *IniFilePtr = s_oIniProvider.Get( (m4pchar_t)lpFileName );

    // If the ini file is loaded and it is no void, go on
    if( IniFilePtr != NULL )
	{
      // Change the section name to lower case
      (void) strcpy(section_str, lpAppName);
      change_to_lower(section_str);

      // Search the requested section
      block = IniFilePtr->m4blocks_t::find(section_str);

      if (block != IniFilePtr->end()) {

	// Take each property
	for (property = (*block).second.begin(), nchar = 0;
	     property != (*block).second.end() && nchar < nSize - 1;
	     property++) {
	  
	  // Copy the key name and its value

	  // At the begin, copy ...
	  //	  if (property == (*block).second.begin()) {

	  if ((str_size = (*property).first.size() + 1) < nSize - nchar) {
	    (void) strcpy(lpReturnedString + nchar, (*property).first.c_str());

	    // Pointer to the property to concatenate later the evalue
	    Property_ptr = lpReturnedString + nchar;

	    nchar += str_size;
	  }

	  // Fill buffer, because this buffer is not large enough
	  else if (nSize - nchar > 1) {

	    (void) strncpy(lpReturnedString + nchar, (*property).first.c_str(),
			   nSize - nchar - 1);

	    // Finish the string
	    lpReturnedString[nSize - 2] = '\0';
	    nchar = nSize - 1;

	    break;
	  }

	  // Copy the "=" character
	  if (nSize - nchar >= 2) {

	    (void) strcat(Property_ptr, "=");
	    //(void) strcat(lpReturnedString + nchar, "=");
	    nchar += 1;

	    //Finish the total string 
	    if (nSize - 1 - nchar == 0) {
	      lpReturnedString[nchar - 1] = '\0';
	      break;
	    }

	    // Copy the value string
	    if ((str_size = (*property).second.size()) <= nSize - 1 - nchar) {

	      (void) strcat(Property_ptr,
	      	    (*property).second.c_str());


	      // (void) strcat(lpReturnedString + nchar,
	      //	    (*property).second.c_str());
	      nchar += str_size;
	    }
	    else if (nSize - nchar > 1) {

	      (void) strncat(Property_ptr,
			     (*property).second.c_str(),
			     nSize - nchar - 1);


	      //	      (void) strncat(lpReturnedString + nchar,
	      //		    (*property).second.c_str(),
	      //			 nSize - nchar - 1);

	      // Finish the string
	      lpReturnedString[nSize - 2] = '\0';
	      nchar = nSize - 1;

	      break;
	    }	    
	  }
	}
	
	// Finish the string that contains all pairs "key=value"
	if (nchar <= nSize - 1)
	  lpReturnedString[nchar] = '\0';

	// If the buffer size is not enough, report it
	if (property != (*block).second.end())
	  nchar = nSize - 2;
      }
    }
  }
  return(nchar);
}


// The GetPrivateProfileSectionNames function retrieves
// the names of all sections in an initialization file

DWORD GetPrivateProfileSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize, 
				    LPCTSTR lpFileName )
{
  // BORRAR
  //  DWORD i = 0;
  DWORD nchar = 0;
  //Clini ini_str;
  m4blocks_t::iterator block;
  // BORRAR
  //  LPCSTR p = NULL;
  DWORD str_size = 0;

  
  // If there are parameters, go on 
  if (lpszReturnBuffer && nSize) {

	Clini *IniFilePtr = s_oIniProvider.Get( (m4pchar_t)lpFileName );

    // If the ini file is loaded and it is no void, go on
    if( IniFilePtr != NULL || ( IniFilePtr = s_oIniProvider.Get( "WIN.INI" ) ) != NULL )
	{
      // Take each block of the INI file
      for (block = IniFilePtr->begin(); block != IniFilePtr->end(); block++) {

	// Copy block name

	if ((str_size = (*block).first.size() + 1) < nSize - nchar) {
	  (void) strcpy(lpszReturnBuffer + nchar, (*block).first.c_str());
	  nchar += str_size;
	}
	else if (nSize - nchar > 1) {
	  (void) strncpy(lpszReturnBuffer + nchar, (*block).first.c_str(),
			 nSize - nchar - 1);

	  // Finish the string
	  lpszReturnBuffer[nSize - 2] = '\0';
	  nchar = nSize - 1;

	  break;
	}
      }
      
      // Finish the string that contains all section names
      if (nchar <= nSize - 1)
	lpszReturnBuffer[nchar] = '\0';

      // If the buffer size is not enough, report it
      if (block != IniFilePtr->end())
	nchar = nSize - 2;
    }    
  }
  return(nchar);
}


// The GetPrivateProfileString function retrieves a
// string from the specified section in an initialization file

DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, 
			      LPCTSTR lpDefault, LPTSTR lpReturnedString, 
			      DWORD nSize, LPCTSTR lpFileName )
{
  // BORRAR
  //  DWORD i = 0;
  DWORD nchar = 0;
  //Clini ini_str;
  m4blocks_t::iterator block;
  ClBlock::iterator property;
  DWORD str_size = 0;
  // BORRAR
  //m4char_t *p = NULL;
  m4char_t section_str[MAX_TAM_LINE];
  m4char_t property_str[MAX_TAM_LINE];
  M4ClString value;


  if (lpDefault && lpReturnedString && nSize && lpFileName) {

	Clini *IniFilePtr = s_oIniProvider.Get( (m4pchar_t)lpFileName );

    // Copy all section names in the ini file to the
    // supplied buffer
    if (lpAppName == NULL) {
      nchar = GetPrivateProfileSectionNames(lpReturnedString, nSize,
					    lpFileName );
    }

    // All key names in the section specified by the
    // lpAppName parameter are copied to the buffer specified
    else if (lpKeyName == NULL) {

      // If the ini file is loaded and it is no void, go on
	  if( IniFilePtr != NULL )
	  {

	// Change the section name to lower case
	(void) strcpy(section_str, lpAppName);
	change_to_lower(section_str);
	
	// Search the requested section
	block = IniFilePtr->m4blocks_t::find(section_str);
	
	if (block != IniFilePtr->end()) {

	  // Take each property
	  for (property = (*block).second.begin();
	       property != (*block).second.end() && nchar < nSize - 1;
	       property++) {
	    
	    // Copy the key name

	    if ((str_size = (*property).first.size() + 1) < nSize - nchar) {
	      (void) strcpy(lpReturnedString + nchar, 
			    (*property).first.c_str());
	      nchar += str_size;
	    }

	    // Fill buffer, because this buffer is not large enough
	    else if (nSize - nchar > 1) {
	      (void) strncpy(lpReturnedString + nchar, 
			     (*property).first.c_str(), nSize - nchar - 1);

	      // Finish the string
	      lpReturnedString[nSize - 2] = '\0';
	      nchar = nSize - 1;

	      break;
	    }	      
	  }

	  // Finish the string that contains all keys
	  if (nchar <= nSize - 1)
	    lpReturnedString[nchar] = '\0';
	  
	  // If the buffer size is not enough, report it
	  if (property != (*block).second.end())
	    nchar = nSize - 2;
	}  
      }
    } // else if (lpKeyName == NULL)

    // Search section name and key name
    else {

      // If the ini file is loaded and it is no void, go on
	  if( IniFilePtr != NULL )
	  {
	// Change the section name to lower case
	(void) strcpy(section_str, lpAppName);
	change_to_lower(section_str);
	
	// Change the key name to lower case
	(void) strcpy(property_str, lpKeyName);
		change_to_lower(property_str);
	
	// Search section and key in the ini file
	property = IniFilePtr->find(section_str, property_str);
	
	if (IniFilePtr->return_value(section_str, property, value) == M4_TRUE) {

	  // Discard single or double quotation marks

	  if (value.find_first_of("'") != M4_STR_NPOS) {
	    value.erase(value.find_first_of("'"), 1);
	    if (value.find_last_of("'") != M4_STR_NPOS)
	      value.erase(value.find_last_of("'"), 1);
	  }
	  else if (value.find_first_of("\"") != M4_STR_NPOS) {
	    value.erase(value.find_first_of("\""), 1);
	    if (value.find_last_of("\"") != M4_STR_NPOS)
	      value.erase(value.find_last_of("\""), 1);
	  }


	  // Copy the key value to the buffer
	  if (value.size() + 1 < nSize) {
	    lpReturnedString = strcpy(lpReturnedString, value.c_str());

	    // Not consider the '\0' character 
	    nchar = strlen(value.c_str());

	    lpReturnedString[nchar] = '\0';
	  }
	  else {
	    lpReturnedString = strncpy(lpReturnedString, value.c_str(),
				       nSize - 2);
	    nchar = nSize - 1;
	    lpReturnedString[nSize - 2] = '\0';
	    lpReturnedString[nSize - 1] = '\0';
	  }
	}

	// Section and key not found
	else {

	  // Copy the default string to the buffer
	  if (strlen(lpDefault) + 1 < nSize) {
	    lpReturnedString = strcpy(lpReturnedString, lpDefault);
	    nchar = strlen(lpDefault);
	    lpReturnedString[nchar] = '\0';
	  }
	  else {
	    lpReturnedString = strncpy(lpReturnedString, lpDefault,
				       nSize - 2);
	    nchar = nSize - 1;
	    lpReturnedString[nSize - 2] = '\0';
	    lpReturnedString[nSize - 1] = '\0';
	  }
	}
      }
	  else {
	  
		 // Copy the default string to the buffer
	  if (strlen(lpDefault) + 1 < nSize) {
	    lpReturnedString = strcpy(lpReturnedString, lpDefault);
	    nchar = strlen(lpDefault);
	    lpReturnedString[nchar] = '\0';
	  }
	  else {
	    lpReturnedString = strncpy(lpReturnedString, lpDefault,
				       nSize - 2);
	    nchar = nSize - 1;
	    lpReturnedString[nSize - 2] = '\0';
	    lpReturnedString[nSize - 1] = '\0';
	  }
	  
	  }


    } // else (Search section name and key name)
  } // if (lpDefault, lpReturnedString, nSize, lpFileName)
  return(nchar);
}


#endif



