#ifndef __HUGE_STRING_H__
#define __HUGE_STRING_H__

#if !defined(_WINDOWS) && !defined(_UNIX)
/*----------------------------------------------------------------------------------------------*/

/*---   INCLUDES MICROSOFT   ---*/

#include "m4strobl.hpp"   
#include "m4winobl.hpp"      


/*----------------------------------------------------------------------------------------------*/




/*---   DEFINICIÓN DE LA CLASE  "M4ClHString" (huge string)  ---*/

class M4ClHString
{
private:
	char huge *phCadena;
	HGLOBAL hCadena;
	int m_iUsos;
public:
	//  Constructor.
	M4ClHString () { phCadena = NULL;  m_iUsos = 0 ;};
	~M4ClHString();
	//  Destructor.
	
	//  Conversor a const char *
	operator char *()
	{             
		return (char huge *)phCadena; 
	} ;    
	
    
	
	
	char operator [](DWORD index)
	{ 
		if ( index < 0 ) 
		{
			index = 0 ;
		}
		return *((char huge *)phCadena + index);
	};
	
	//  Funciones para copiar de un string a otro.
	
	friend DWORD OBLstrcpy (M4ClHString & destino, M4ClHString origen);
	friend DWORD OBLstrcpy (M4ClHString & destino, const char *origen);
	friend DWORD OBLstrncpy (char  * destino,char huge *  origen,size_t tam);
	
	//  Funciones para concatenar.
	friend DWORD OBLstrcat(M4ClHString  & destino , const char *origen);
	// friend DWORD OBLstrcat ( M4ClHString & destino, const char huge * cadena); 
	
	friend DWORD OBLstrlen ( M4ClHString & origen ) ;
	
	
};
/*---   FIN DE LA DEFINICIÓN DE LA CLASE  "M4ClHString"   ---*/




/*----------------------------------------------------------------------------------------------*/

#endif		// no en Win32 y UNIX

#endif

