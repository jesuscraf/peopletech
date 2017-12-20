/* -----------------------------------------------------------------------
 *
 *	(c) Copyright  1991-1998 Meta Software M.S., S.A
 *	All rights reserved.
 *	
 *	Module:           M4Doc libray
 *	File:             ClDllMap.cpp
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



/* -----------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------
*/

#include "cldllmap.hpp"
#include "m4docerrors.hpp"


/* ----------------------------------
 * ClDllItem:  Es la información de una Dll
 *             Tiene su nombre y el objeto que contiene los hendled a la carga
 *
 * ---------------------------------------
*/

/* ----------------------------------
 * ClDllItem::Construccion
 *      Nos apuntamos el nombre de la Dll
 * ---------------------------------------
*/
ClDllItem::ClDllItem(const string& ai_sDllName)
{
		
	m_poDllLoaded = 0;
	m_sDllName = ai_sDllName;

};

/* ----------------------------------
 * ClDllItem::Destructor
 *      Si tenemos una Dll cargada la descargamos
 *      si falla seguimos hacia adelante puesto que no hay nada que hacer
 * ---------------------------------------
*/
ClDllItem::~ClDllItem ()
{
	
	if (m_poDllLoaded != 0 )
	{
		// Free library - unload.
		if (m_poDllLoaded->Close() != M4_SUCCESS ) {
			string sErrorMessage = "Unload Dll: <" + m_sDllName + ">";
			M4DOCErrorHandling(M4DOCERROR_LIBRARY, sErrorMessage.c_str ());
		}

		//liberamos el objeto
		delete m_poDllLoaded;
		m_poDllLoaded = 0;
	}
};


/* ----------------------------------
 * ClDllItem::LoadDll
 *      Intentamos cargar la Dll a la que representamos
 * ---------------------------------------
*/
m4return_t ClDllItem::LoadDll ()
{

	//Veamos
	m_poDllLoaded = new M4ClLoaDll;
	if (m_poDllLoaded->Open(m_sDllName.c_str ()) != M4_SUCCESS ) {
		// pues vamos bien		
		string sErrorMessage = "Load Dll: <" + m_sDllName + " >";
		M4DOCErrorHandling(M4DOCERROR_LIBRARY, sErrorMessage.c_str ()); 

		// liberamos
		delete m_poDllLoaded;
		m_poDllLoaded = 0;

		// es un error serio		
    	return M4_ERROR; 
	}

	return M4_SUCCESS;

}



/* ----------------------------------
 * ClDllMap:  Mantiene un mapa de las Dll cargadas, representado por ClDllItem. 
 *            Estas se descargan cuando el objeto es destruido
 *            Con lo que si se define estático se descargarán las Dll
 *            cuando lo haga la M4Doc
 *
 * ---------------------------------------
*/



/* ----------------------------------
 * ClDllMap::Destructor
 *      Descargamos todos los objetos del vector lo que provocal
 *      la descarga de las Dlls
 * ---------------------------------------
*/
ClDllMap::~ClDllMap()
{

	// variables
	TP_VECTOR_ClDllItem::iterator oIterador;
	ClDllItem*  poDllItem = 0;


	for (oIterador = m_oVectorDllsItems.begin();
	     oIterador != m_oVectorDllsItems.end(); oIterador++)
	{
		poDllItem = (ClDllItem *)*oIterador;
		delete poDllItem;
		poDllItem = 0;
	}
};


/* ----------------------------------
 * ClDllMap::GetDllObject
 *      Recibimos:
 *              ai_sDllName    el nombre de una Dll a cargar
 *      Devolvemos:
 *              El objeto ClDllItem que representa a la Dll
 *				En caso de fallo se devuelve 0
 *
 *      Si la Dll ya fué cargada se devuelve y ya está, y si no 
 *      se carga y se devuelve.
 * ---------------------------------------
*/
M4ClLoaDll* ClDllMap::GetDllObject(const string& ai_sDllName)
{
	// variables
	TP_VECTOR_ClDllItem::iterator oIterador;
	ClDllItem*           poNewDllItem = 0;
	M4ClLoaDll*          dll  = 0;

	m_oMutex.Lock ();

	for (oIterador = m_oVectorDllsItems.begin();
	     oIterador != m_oVectorDllsItems.end(); oIterador++)
	{
		poNewDllItem = (*oIterador);
		if (poNewDllItem->GetDllName() == ai_sDllName)
		{
			dll = poNewDllItem -> GetDllObject();
		
			m_oMutex.Unlock ();

			return dll;
		}
	}


	// si llegamos aquí es que la Dll no estaba, asi que 
	// creamos el DllItem y la cargamos la Dll
	poNewDllItem = new ClDllItem(ai_sDllName);
	if ( poNewDllItem->LoadDll () != M4_SUCCESS) 
	{
		// pues vamos bien		
		// liberamos
		delete poNewDllItem;
		poNewDllItem = 0;

		m_oMutex.Unlock ();

		// es un error serio		
    	return 0; 
	}


	// anotamos el nuevo ItemDll en el mapa
	m_oVectorDllsItems.push_back(poNewDllItem);
//	cout << "Num dll: " << m_oVectorDllsItems.size () << endl; cout.flush ();

	dll =  poNewDllItem->GetDllObject();

	m_oMutex.Unlock ();

	return dll;
}



