/* -----------------------------------------------------------------------
 *
 *	(c) Copyright  1991-1998 Meta Software M.S., S.A
 *	All rights reserved.
 *	
 *	Module:           M4Doc libray
 *	File:             ClDllMap.hpp
 *	Project:          Document Management
 *	Author:           Meta Software M.S. , S.A
 *	Date:             20-Julio-2000
 *	Language:         C++
 *	Operating System: Windows 32, UNIX - Solaris 
 *	Design Document:  
 *
 * ----------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------
 * Descripción
 *			Es un mapa de las Dll cargadas, para evitar la carga y descarga indiscriminada				
 * ----------------------------------------------------------------------------
*/



#ifndef _CL_DLL_MAP
	#define _CL_DLL_MAP




/* -----------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------
 */

// General Meta4 headers.
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4loadll.hpp"
#include <m4string.hpp>

#include "m4thread.hpp"		//por el mutex


/* -----------------------------------------------------------------------
 * TIPOS
 * ------------------------------------------------------------------------
*/
class ClDllItem;
typedef vector<ClDllItem*>   TP_VECTOR_ClDllItem;



/* ----------------------------------
 * ClDllItem:  Es la información de una Dll
 *             Tiene su nombre y el objeto que contiene los hendled a la carga
 *
 * ---------------------------------------
*/

class ClDllItem
{
private:
	  string       m_sDllName;
	  M4ClLoaDll*  m_poDllLoaded;	// Object to handle the DLL.

public:

	ClDllItem (const string& ai_sDllName);
   ~ClDllItem();

   /* 
    * Acceso a las propiedades
    */
   const string&   GetDllName   () const {return  m_sDllName;};
   M4ClLoaDll*     GetDllObject () {return  m_poDllLoaded;};

   /* 
    * LoadDll: Cargamos la Dll a la que representamos
    */
   m4return_t LoadDll ();

};


/* ----------------------------------
 * ClDllMap:  Mantiene un mapa de las Dll cargadas, representado por ClDllItem.
 *            Estas se descargan cuando el objeto es destruido
 *            Con lo que si se define estático se descargarán las Dll
 *            cuando lo haga la M4Doc
 *
 * ---------------------------------------
 */
class ClDllMap
{
private:
	  TP_VECTOR_ClDllItem m_oVectorDllsItems;
	  ClMutex	m_oMutex;


public:

	ClDllMap() { m_oMutex.Init (); }
   ~ClDllMap();

   /* 
    * GetDllObject: Devolvemos el Objeto que permite el acceso a la DLL
    */
   M4ClLoaDll* GetDllObject(const string& ai_sDllName);

};

#endif // _CL_DLL_MAP
