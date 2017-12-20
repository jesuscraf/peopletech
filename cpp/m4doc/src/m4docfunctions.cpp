//
//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc
// File:             functions.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             18-October-1999
// Language:         C++
// Operating System: Windows 32, UNIX - Solaris 
// Design Document:  
//
//
// Definition:
//
//		This file comprises the declarations of the following functions:
//				DeleteDoc   LoadDoc   NewDoc   SaveDoc	
//				build_appropriate_dll_name check_nulls_in_input_parameters
//
//==============================================================================

// General Meta4 headers.
#include "m4types.hpp"
#include "m4define.hpp"
#include "m4log.hpp"
#include "m4loadll.hpp"
#include "m4extsearch.hpp"			// Extended Search.

#include "m4filemanager.hpp"  //M4FileManagger access
#include "file_misc.hpp"
#include "file_finder.hpp"

//This project headers.
#include "m4docmultipla.hpp"
#include "m4docfunctions.hpp"

// File definitions
#include "m4odmdef.hpp"

// Error definitions.
#include "m4docerrors.hpp"

//CODE FOR DLL GLOBAL MAP
#include "cldllmap.hpp"

// Encriptacióm.
#include "m4cryptc.hpp"

#include "execontx.hpp"
#include "recorset.hpp"
#include "item.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"

// date
#include "m4date.hpp"
extern "C" {
#include "zipapi.h"
}

// access
#include "access.hpp"
#include "m4mdrt.hpp"

#ifdef _UNIX
#include <errno.h>
#endif


#define	M4DOC_BUFFER_SIZE					4096

#define MAX_LINE_LENGTH						1024

extern char	M4_DECL_M4UNICODE	acUTF16BOM[ M4UNI_UTF16_BOM_SIZE ] ;



/* -----------------------------------------------------------------------
 * GLOBALES
 * ------------------------------------------------------------------------
*/
ClDllMap g_oDllMap;

// Objeto global contenedor de Búsquedas Extendidas.
ClExtSearchManager oExtSearchMng;


#ifdef _WINDOWS
	#include <windows.h> //sleep
#endif

/* ****************************************************************************** */
// Function: DeleteDoc
// Module M4DOC.DLL
// Arguments:				
//		IN:(Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//			sDocId			  identificador del documento
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
// Description: This function deletes the document identified by docId.
/* ****************************************************************************** */
// M4DOC.DLL - DeleteDoc
m4return_t DeleteDoc ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// todos los errores de este método son criticos y deben para la 
	// ejecución del códgio LN4, asi que devolvemo M4_ERROR en el return
	// para que el ejecutor de método detenga la ejecución de codigo LN4

	// variables
	string          sConnectString, 
		            sDocId, 
		            sDllName, 
		            sErrorMessage;
	M4ClLoaDll      oDllLoaded;	// Object to handle the DLL.
	m4return_t      tReturn,
					tLN4Return; 


	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
		sDllName       = m4pchar_t(ai_pvArg[0]);
		sConnectString = m4pchar_t(ai_pvArg[1]);
		sDocId         = m4pchar_t(ai_pvArg[2]);


	// el nombre completo de la Dll es
	GetFullDllName (sDllName);

	// Empezamos
	tReturn    = M4_SUCCESS;
	tLN4Return = M4_SUCCESS;

	
	M4ClLoaDll*     poDllLoaded;
	m4typefunreturn fpM4function;
	DMSDELETEDOC    fpProc;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	// Get Process Address
	if ( poDllLoaded -> GetFunc ("DeleteDoc", fpM4function) != M4_SUCCESS ) 
	{
		sErrorMessage = "Dll: <" + sDllName + ".DeleteDoc>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 

		// es un error serio		
    	tReturn = M4_ERROR; 
	}
	
	//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
	if ( (tReturn != M4_ERROR) && ( (fpProc=(DMSDELETEDOC)fpM4function) == NULL) )
	{
		sErrorMessage = "Dll: <" + sDllName + ".(Cast)DeleteDoc>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 

		// es un error serio		
    	tReturn = M4_ERROR; 
	}
	
	// External DLL function call.	
	if ( (tReturn != M4_ERROR) && (fpProc ((m4pchar_t)sConnectString.c_str(),(m4pchar_t)sDocId.c_str()) != M4_SUCCESS) )
	{
		// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
		// ejecutó correctamente y el LN4 falló.
		sErrorMessage = "Dll: <" + sDllName + ".DeleteDoc(IdDoc= " + sDocId + ")>";
		M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());

		// es un error funcional
		tLN4Return = M4_ERROR;
		tReturn = M4_SUCCESS; 
	} 
	
	// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}

/* ****************************************************************************** */
// Function: LoadDoc															
// Module M4DOC.DLL
// Arguments:																	
//
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//			sDocId			  identificador del documento
//			sTempFile		  path a un archivo temporal donde meter el documento
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
// Description: This function loads the document identified by docId in the DMS
//              to the temporary file specified in the path.
/* ****************************************************************************** */
// M4DOC.DLL - LoadDoc
m4return_t LoadDoc ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
		 
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
		
	// Recogemos los valores de la llamada
	string	sDllName       = m4pchar_t(ai_pvArg[0]);
	string	sConnectString = m4pchar_t(ai_pvArg[1]);
	string	sDocId         = m4pchar_t(ai_pvArg[2]);
	
	string	sTempFile;
	string	slistaCampos;
	string	slistaValores;


	// File, FieldNameList and FieldValueList may be NULL;
	if(ai_pvArg[3].GetType()==M4CL_CPP_TYPE_STRING_VAR)
		sTempFile          = m4pchar_t(ai_pvArg[3]);

	if (ai_iLongArg >= 6)
	{
		if(ai_pvArg[4].GetType()==M4CL_CPP_TYPE_STRING_VAR)
			slistaCampos   = m4pchar_t(ai_pvArg[4]);
		if(ai_pvArg[5].GetType()==M4CL_CPP_TYPE_STRING_VAR)
			slistaValores  = m4pchar_t(ai_pvArg[5]);
	}


	// el nombre completo de la Dll es
	GetFullDllName (sDllName);

	// Empezamos
	m4return_t tReturn    = M4_SUCCESS;
	m4return_t tLN4Return = M4_SUCCESS;


	M4ClLoaDll*     poDllLoaded;
	m4typefunreturn fpM4function;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	string sErrorMessage;
	if ( slistaCampos.empty () )
	{
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("LoadDoc", fpM4function) != M4_SUCCESS) )
		{
			sErrorMessage = "Dll: <" + sDllName + ".LoadDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
		DMSLOADDOC      fpProc;
		if ( (tReturn != M4_ERROR) && ( (fpProc=(DMSLOADDOC)fpM4function) == NULL) )
		{
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)LoadDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		if ( (tReturn != M4_ERROR) && 
			(fpProc ((m4pchar_t)sConnectString.c_str(), 
		          (m4pchar_t)sDocId.c_str(), 
				  (m4pchar_t)sTempFile.c_str()       ) != M4_SUCCESS) )
		{
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".LoadDoc(IdDoc= " + sDocId + ", TempFile= " + sTempFile + ")>" ;
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		} 
	}
	else
	{
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("LoadDocAd", fpM4function) != M4_SUCCESS) )
		{
			sErrorMessage = "Dll: <" + sDllName + ".LoadDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
		DMSLOADDOCAD      loaddocad;
		if ( (tReturn != M4_ERROR) && ( (loaddocad=(DMSLOADDOCAD)fpM4function) == NULL) )
		{
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)LoadDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		m4char_t slistaV[5000];
		if ( (tReturn != M4_ERROR) && 
			(loaddocad ((char*)sConnectString.c_str(),(char*)sDocId.c_str(),(char*)sTempFile.c_str(),(char*)slistaCampos.c_str(), slistaV) != M4_SUCCESS) )
		{
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".LoadDoc(IdDoc= " + sDocId + ", TempFile= " + sTempFile + ")>" ;
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		}

		// Devolvemos siempre algo, aunque sea ""
		ai_pvArg[5].SetSysString(slistaV);

	}


// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}


// *****************************************************************************
// Function: NewDoc
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//			sFile			  el archivo a añadir
//			sFormat			  el formato del archivo (para algunos DMS)
//
//		OUT		
//			sDocId
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function creates a new document, identified by newDocId, with
//               the document content of the temporary file.   				
// ***************************************************************************** 
// M4DOC.DLL - NewDoc
m4return_t NewDoc ( m4VariantType *ai_pvArg,  m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn,  m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[3].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
		 
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada
	string	sListaCampos,sListaValores,sFile;
	string	sDllName       = m4pchar_t(ai_pvArg[0]);
	string	sConnectString = m4pchar_t(ai_pvArg[1]);
	string	sDocFormat     = m4pchar_t(ai_pvArg[3]);


	// File, FieldNameList and FIeldValueList may be NULL;
	if(ai_pvArg[2].GetType() == M4CL_CPP_TYPE_STRING_VAR)
		sFile          = m4pchar_t(ai_pvArg[2]);
	if(ai_pvArg[5].GetType() == M4CL_CPP_TYPE_STRING_VAR)
		sListaCampos   = m4pchar_t(ai_pvArg[5]);
	if(ai_pvArg[6].GetType() == M4CL_CPP_TYPE_STRING_VAR)
		sListaValores  = m4pchar_t(ai_pvArg[6]);


	// el nombre completo de la Dll es
	GetFullDllName (sDllName);

	// Empezamos
	m4return_t tReturn    = M4_SUCCESS;
	m4return_t tLN4Return = M4_SUCCESS;

	M4ClLoaDll*     poDllLoaded;
	m4typefunreturn fpM4function;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}
	string sErrorMessage;
	if ( sListaCampos.empty () )
	{
		DMSNEWDOC		fpProc;
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("NewDoc", fpM4function) != M4_SUCCESS) )
		{
			
			sErrorMessage = "Dll: <" + sDllName + ".NewDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
		if ( (tReturn != M4_ERROR) && 
			( (fpProc=(DMSNEWDOC)fpM4function) == NULL) ) {
			
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)NewDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		// inicializamos el valor de retorno
		m4pchar_t    	pDocId;
		pDocId = new m4char_t[M4_M4DOC_DOCID_MAX + 1];
		memset(pDocId, 0, M4_M4DOC_DOCID_MAX + 1 );
		if ( (tReturn != M4_ERROR) && 
			(fpProc ((m4pchar_t)sConnectString.c_str(), 
		          (m4pchar_t)sFile.c_str(), 
				  (m4pchar_t)sDocFormat.c_str(), pDocId) != M4_SUCCESS) ) {
			
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".NewDoc(File= " + sFile + ", DocFormat= " + sDocFormat + ")>";
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		}
		
		// Devolvemos siempre algo, aunque sea ""
		ai_pvArg[4].SetSysString(pDocId);
		
		/* Juanma 20-Jun
		#ifdef M4CL_CPP_TYPE_STRING_VAR_SYS
		ai_pvArg[4].SetSysString(pDocId);
		#else
		ai_pvArg[4].SetString(pDocId);
		#endif
		*/
		
		// liberamos lo asignado
		delete [] pDocId;       
		pDocId = NULL;
	}
	else
	{
		DMSNEWDOCAD		newdocad;
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("NewDocAd", fpM4function) != M4_SUCCESS) )
		{
			
			sErrorMessage = "Dll: <" + sDllName + ".NewDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
		if ( (tReturn != M4_ERROR) && 
			( (newdocad=(DMSNEWDOCAD)fpM4function) == NULL) ) {
			
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)NewDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		// inicializamos el valor de retorno
//		string    	sDocId;
		m4char_t   	sDocId[500];
		if ( (tReturn != M4_ERROR) && (newdocad ((char*)sConnectString.c_str(),(char*)sFile.c_str(),(char*)sDocFormat.c_str(),sDocId,(char*)sListaCampos.c_str(),(char*)sListaValores.c_str()) != M4_SUCCESS) ) {
			
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".NewDoc(File= " + sFile + ", DocFormat= " + sDocFormat + ")>";
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		}

		// Devolvemos siempre algo, aunque sea ""
//		ai_pvArg[4].SetSysString((char* const)sDocId.c_str ());
		ai_pvArg[4].SetSysString(sDocId);
	}


	// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}

/********************************************************************************/
// Function: SaveDoc						
// Module M4DOC.DLL
// Arguments:								
//
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//			sDocId			  el Id del documento
//			sFile			  el fichero a asociar
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
// Description: This functions saves the content of the file in the document. 
/********************************************************************************/
// M4DOC.DLL - SaveDoc
m4return_t SaveDoc ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
		 
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// File, FieldNameList and FIeldValueList may be NULL;

	
	

	// Recogemos los valores de la llamada
	string	sDllName       = m4pchar_t(ai_pvArg[0]);
	string	sConnectString = m4pchar_t(ai_pvArg[1]);
	string	sDocId         = m4pchar_t(ai_pvArg[2]);
	string	sFile;
	string	slistaCampos;
	string	slistaValores;

	if(ai_pvArg[3].GetType()==M4CL_CPP_TYPE_STRING_VAR)
		sFile          = m4pchar_t(ai_pvArg[3]);
	if(ai_pvArg[4].GetType()==M4CL_CPP_TYPE_STRING_VAR)
		slistaCampos   = m4pchar_t(ai_pvArg[4]);
	if(ai_pvArg[5].GetType()==M4CL_CPP_TYPE_STRING_VAR)
		slistaValores  = m4pchar_t(ai_pvArg[5]);

	// Empezamos
	m4return_t tReturn    = M4_SUCCESS;
	m4return_t tLN4Return  = M4_SUCCESS;

	// el nombre completo de la Dll es
	GetFullDllName (sDllName);


	M4ClLoaDll*     poDllLoaded;
	m4typefunreturn fpM4function;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	string sErrorMessage;
	if ( slistaCampos.empty () )
	{
		// Get Process Address
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("SaveDoc", fpM4function) != M4_SUCCESS) ) 	{
			
			sErrorMessage = "Dll: <" + sDllName + ".SaveDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}

		DMSSAVEDOC      fpProc;
		if ( (tReturn != M4_ERROR) && 
			( (fpProc=(DMSSAVEDOC)fpM4function) == NULL) ) {
			
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)SaveDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		if ( (tReturn != M4_ERROR) && 
			(fpProc ((m4pchar_t)sConnectString.c_str(), 
		          (m4pchar_t)sDocId.c_str(),
				  (m4pchar_t)sFile.c_str()            ) != M4_SUCCESS) ) {
			
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".SaveDoc(DocId= " + sDocId + "File= " + sFile + ")>";
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		} 
	}
	else
	{
		// Get Process Address
		if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("SaveDocAd", fpM4function) != M4_SUCCESS) ) 	{
			
			sErrorMessage = "Dll: <" + sDllName + ".SaveDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}

		DMSSAVEDOCAD      savedocad;
		if ( (tReturn != M4_ERROR) && 
			( (savedocad=(DMSSAVEDOCAD)fpM4function) == NULL) ) {
			
			sErrorMessage = "Dll: <" + sDllName + ".(Cast)SaveDoc>";
			M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
			
			// es un error serio		
			tReturn = M4_ERROR; 
		}
		
		// External DLL function call.	
		if ( (tReturn != M4_ERROR) && 
			(savedocad ((char*)sConnectString.c_str(),(char*)sDocId.c_str(),(char*)sFile.c_str(),(char*)slistaCampos.c_str(),(char*)slistaValores.c_str()) != M4_SUCCESS) )
		{
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".SaveDoc(DocId= " + sDocId + "File= " + sFile + ")>";
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		} 
	}


// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}

/* ****************************************************************************** */
// Function: M4CloseDatabase															
// Module M4DOC.DLL
// Arguments:																	
//
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
// Description: This function closes the LotusNotes Database 
//              previously opened using connect string
/* ****************************************************************************** */
// M4DOC.DLL - LoadDoc
m4return_t M4CloseDatabase( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
		 
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
		
	// Recogemos los valores de la llamada
	string	sDllName       = m4pchar_t(ai_pvArg[0]);
	string	sConnectString = m4pchar_t(ai_pvArg[1]);
	
	
	
	// el nombre completo de la Dll es
	GetFullDllName (sDllName);

	// Empezamos
	m4return_t tReturn    = M4_SUCCESS;
	m4return_t tLN4Return = M4_SUCCESS;


	M4ClLoaDll*     poDllLoaded;
	m4typefunreturn fpM4function;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	string sErrorMessage;
	
	if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("CloseDatabase", fpM4function) != M4_SUCCESS) )
	{
		sErrorMessage = "Dll: <" + sDllName + ".M4CloseDatabase>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
		
		// es un error serio		
		tReturn = M4_ERROR; 
	}
		
		//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
	DMSCLOSEDATABASE      fpProc;
	if ( (tReturn != M4_ERROR) && ( (fpProc=(DMSCLOSEDATABASE)fpM4function) == NULL) )
	{
		sErrorMessage = "Dll: <" + sDllName + ".(Cast)LoadDoc>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
	
		// es un error serio		
		tReturn = M4_ERROR; 
	}
		
		// External DLL function call.	
		if ( (tReturn != M4_ERROR) && 
			(fpProc ((m4pchar_t)sConnectString.c_str()) != M4_SUCCESS) )
		{
			// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
			// ejecutó correctamente y el LN4 falló.
			sErrorMessage = "Dll: <" + sDllName + ".CloseDatabase(ConnecString= " + sConnectString + ")>" ;
			M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str());
			
			// es un error funcional
			tLN4Return = M4_ERROR;
			tReturn = M4_SUCCESS; 
		} 
	

// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}

/* **************************************************************************** */
// Function: build_appropriate_dll_name
// Arguments:
//			IN
//					plainDllName (e.g. m4files)
//			OUT
//					fullDllName (e.g libm4files.so or m4files.dll)
// Description: 
//			The name of the dll in the data base doesn't have an extension or prefix.
//			This function adds the appropriate tags depending on the operating system.
//
/* **************************************************************************** */
void GetFullDllName(string& sDllName)
{
	// variables
//	string sFullName; 

	//Windows.
	#ifdef _WINDOWS
		//The Windows dll name passed to this function does not include the dll extension. 
		//This is appended to the name.
		//Copy name.
		sDllName += ".dll";
	#endif


	//UNIX
	#ifdef _UNIX
		//The UNIX dynamic library name passed to this function does not include the 
		//'lib' prefix or the '.so' or '.sl' suffixes.
		//These are appended to the name.
		//Note: gfhgfhThe full library name MUST be lowercase in UNIX (e.g. libm4my_lib.so or libm4my_lib.sl).

		sDllName = "lib" + sDllName;
		//Append extension. (.so if Solaris or .sl if HP).
		//Solaris or aix.
		#if defined (_SOLARIS) || defined (AIX)
			sDllName += ".so";
		#endif
		//HP.
		#ifdef _HP
			sDllName += ".sl";
		#endif
	#endif

}


/********************************************************************************/
// Function: M4ShellExecute						
// Module M4DOC.DLL
// Arguments:								
//
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sFilePath		  el fichero que debe ser invocado
//			sVerboOLE         la acción a realizar, por defecto "open"
//			iShowMode		  SW_SHOWNORMAL, SW_NORMAL
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
// Description: Hacemos una llamada al Shell para invocar el verbo de acción OLE sobre el fichero
//				Lo más común en pasar "Open", "Print". 
//				En caso de ser una página HTML y dado que se reutiliza el browser que ya esté levantado
//				y que en el caso de el Cliente Java es el mismo que hizo la llamada se llama como programa + parámetros
//				que como servicio Ole de la extensión
/********************************************************************************/
m4return_t M4ShellExecute(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	// variables
	string sVerboOLE, 
		   sFilePath, 
		   sParameters,
		   sErrorMessage;

	m4int_t iShowMode;

		// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR))
		 // (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||				pueden ser nulos
		 // (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_NUMBER) 	 )
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada
	sFilePath = m4pchar_t(ai_pvArg[0]);

	if (ai_pvArg[1].GetType()!= M4CL_CPP_TYPE_NULL) {
		sVerboOLE = m4pchar_t(ai_pvArg[1]);
	}
	else {
		sVerboOLE = "";
	}

	if (ai_pvArg[2].GetType()!= M4CL_CPP_TYPE_NULL) {
		iShowMode = m4int_t(m4double_t(ai_pvArg[2]));
	}
	else {
		iShowMode = 0;
	}

	// por defecto
	if (sVerboOLE.length() == 0) {
		sVerboOLE = "open";
	}

	
	// trazas
	//		"Información. ID Estruc. Nodo: %0:s. ID Item: %1:s. %2:s"
	sErrorMessage = "#*s0*# # #SHELLEXECUTEINFO (sVerbOLE, sFile) = " + sVerboOLE + ", " + sFilePath + "#";
	SETCODE (7864430, TRACEINFOLOG, sErrorMessage.c_str());


	// WINDOWS
	// ------------
	#ifdef _WINDOWS

		// el modo de visualización no permitimos el Hide, SW_HIDE = 0 
		if (iShowMode == 0) {
			iShowMode = SW_SHOWNORMAL;
		}

		// Bug 0303268 Control de longitud de la cadena
		if ( sFilePath.length() >= 5 ) {

			// si el fichero a mostrar es una página HTML ....
			if ( stricmp (sFilePath.substr(sFilePath.length()-4, 4).c_str(), ".htm") == 0  ||
				 stricmp (sFilePath.substr(sFilePath.length()-5, 5).c_str(), ".html") == 0 ||
				 stricmp (sFilePath.substr(sFilePath.length()-4, 4).c_str(), ".htw") == 0  ||
				 stricmp (sFilePath.substr(sFilePath.length()-4, 4).c_str(), ".plg") == 0 )   	{

				// ... el explorardor que queremos usar es otro que nuestro
				// cliente Java, ya que si se reutiliza el explorador se pierde el Applet. El problema es que el IExplorer
				// siempre reutiliza en el caso de llamada por OLE (DDE) el último explorador abierto.
				// Por eso nuestra función cambia y en vez de llama con el nombre de la página llamamos con "IExplorer c:\temp\Doc.html"
				// lo único que nos queda es averiguar cual es el Browser web por defecto

				m4char_t sPathPrograma [1024];
				if (FindExecutable(sFilePath.c_str(), "", sPathPrograma) > HINSTANCE (32) ) {
					// tenemos el path al browser
					sParameters = sFilePath;
					sFilePath = sPathPrograma;

					// por si al jodido le da por levantarse invisible
					iShowMode = SW_SHOWMAXIMIZED;
				}
				// por si algún listo pregunta por qué 32:
				//			Returns a value greater than 32 if successful, or a value less than or equal to 32 otherwise. 
				// si no hay browser pues llamamos como siempre

			}
			// Hacemos lo mismo para los ficheros .doc
			// no hay realmente ninguna razón, solo que de la forma que estaba
			// el Word no se cerraba si era invocado desde el cliente Java
			// (no sabemos aún porqué), mientras que si lo hacemos así 
			// funciona correctamente
			else if ( stricmp (sFilePath.substr(sFilePath.length()-4, 4).c_str(), ".doc") == 0)
			{

				
				m4char_t sPathPrograma [1024];
				if (FindExecutable(sFilePath.c_str(), "", sPathPrograma) > HINSTANCE (32) ) {
					// tenemos el path al Word (o lo que fuese)

					// ojo a los nombres con blancos en medio!
					sParameters = "\"" + sFilePath + "\"";
					sFilePath = sPathPrograma;

					// por si al jodido le da por levantarse invisible
					iShowMode = SW_SHOWMAXIMIZED;
				}
				// por si algún listo pregunta por qué 32:
				//			Returns a value greater than 32 if successful, or a value less than or equal to 32 otherwise. 
				// si no hay browser pues llamamos como siempre

			}
		}

		// todo sacado del MSDN

		/* Bug 0174509
		Hay que pasar de CPP a UTF16
		*/
		int iLength = -1 ;

		iLength = -1 ;
		M4XMLCh* pwcVerbose = M4CppToUtf16( sVerboOLE.c_str(), iLength ) ;
		iLength = -1 ;
		M4XMLCh* pwcFile = M4CppToUtf16( sFilePath.c_str(), iLength ) ;
		iLength = -1 ;
		M4XMLCh* pwcParameters = M4CppToUtf16( sParameters.c_str(), iLength ) ;

		SHELLEXECUTEINFOW sei;
		ZeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.lpVerb = (LPCWSTR)pwcVerbose;
		sei.lpFile = (LPCWSTR)pwcFile;
		sei.lpParameters = (LPCWSTR)pwcParameters;
		sei.nShow = iShowMode;
		
		BOOL b = ShellExecuteExW( &sei ) ;
		delete pwcVerbose;
		delete pwcFile;
		delete pwcParameters;

		if (b == false) {
			// se la ha pegado
	
			LPVOID lpMsgBuf;

			FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
						   NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					       (LPWSTR)&lpMsgBuf, 0, NULL);

			// Display the string. Hay que pasarlo de UTF16 a CPP
			iLength = -1 ;
			m4pchar_t pcError = M4Utf16ToCpp( (M4XMLCh*) lpMsgBuf, iLength ) ;
			// Free the buffer.
			LocalFree(lpMsgBuf); 
			sErrorMessage = pcError ;
			delete pcError ;

			sErrorMessage = "SHELLEXECUTEINFO (sVerbOLE, sFile, sParam) = '" + sVerboOLE + "', '" + sFilePath + "', '" + sParameters + "'. System error: " + sErrorMessage;
			M4DOCErrorHandling( M4DOCERROR_OTHER, sErrorMessage.c_str());

			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
			
			// este error no es si es de la llamada al método C, por lo tanto es de LN4
			return M4_SUCCESS;

		};

		// paramos un instante antes de continuar para dar tiempo al proceso, mseg
		Sleep (1000);

		// ha ido como la seda
		ai_pvReturn.SetDouble(M4_SUCCESS); 
			
		// este error no es si es de la llamada al método C, por lo tanto es de LN4
		return M4_SUCCESS;


	// UNIX
	// ------------------
	#elif defined(UNIX) || defined(_UNIX) || defined(SOLARIS)

		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
			
		// este error no es si es de la llamada al método C, por lo tanto es de LN4
		return M4_SUCCESS;

	#endif
}


// *****************************************************************************
// Function: NewDoc
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDllName		  el nombre de la Dll que gestiona el repositorio externo
//			sConnectString    la cadena de conexión para ese repositorio
//			sFile			  el archivo a añadir
//			sFormat			  el formato del archivo (para algunos DMS)
//
//		OUT		
//			sDocId
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function creates a new document, identified by newDocId, with
//               the document content of the temporary file.   				
// ***************************************************************************** 
// M4DOC.DLL - NewDoc
m4return_t GetAllPKDoc ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_STRING_VAR)  )
		
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	string	sDllName       = m4pchar_t(ai_pvArg[0]);
	string	sConnectString = m4pchar_t(ai_pvArg[1]);
	string	sformulae	   = m4pchar_t(ai_pvArg[2]);
	
	// el nombre completo de la Dll es
	GetFullDllName (sDllName);
	
	// Empezamos
	m4return_t tReturn    = M4_SUCCESS;
	m4return_t tLN4Return = M4_SUCCESS;
	

	M4ClLoaDll*     poDllLoaded;
	if ( (poDllLoaded = g_oDllMap.GetDllObject (sDllName)) == 0 )
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	// Load library.
	string sErrorMessage;
	
	// Get Process Address
	m4typefunreturn fpM4function;
	GETALLPK		getallpk;
	if ( (tReturn != M4_ERROR) && (poDllLoaded -> GetFunc("GetAllPKDoc", fpM4function) != M4_SUCCESS) )
	{
		
		sErrorMessage = "Dll: <" + sDllName + ".NewDoc>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
		
		// es un error serio		
		tReturn = M4_ERROR; 
	}
	
	//  Hacemos que nuestro puntero tenga la forma de la función DeleteDoc genérica
	if ( (tReturn != M4_ERROR) && ( (getallpk = (GETALLPK)fpM4function) == NULL) )
	{
		sErrorMessage = "Dll: <" + sDllName + ".(Cast)NewDoc>";
		M4DOCErrorHandling(M4DOCERROR_PROCADDRSSS, sErrorMessage.c_str ()); 
		
		// es un error serio		
		tReturn = M4_ERROR; 
	}
	
	// External DLL function call.	
	// inicializamos el valor de retorno
	string    	allPK;
	if ( (tReturn != M4_ERROR) && (getallpk (sConnectString,sformulae,allPK) != M4_SUCCESS) )
	{
		// un error en la llamada a la funcionalidad, debe entenderse como que el método C++ se 
		// ejecutó correctamente y el LN4 falló.
		sErrorMessage = "Dll: <" + sDllName + ".GetAllPKDoc";
		M4DOCErrorHandling(M4DOCERROR_EXTERNDLL, sErrorMessage.c_str ());
		
		// es un error funcional
		tLN4Return = M4_ERROR;
		tReturn = M4_SUCCESS; 
	}
	
	// Devolvemos siempre algo, aunque sea ""
	ai_pvArg[3].SetSysString((char* const)allPK.c_str ());
	
	// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return tReturn;
}


// *****************************************************************************
// Function: M4SetFileManagerOwnerOfTheFile
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//          sItemBlobFile           Fichero asociado al item Blob para el que establecer el owner
//                                  (valor del item blob) 
//			bFileManagerIsOwner		booleano indicando si el FileManager debe ser
//                                  propietario o no del fichero.

//		OUT		
//			sDocId
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function is used to stablish the FileManager as owner o not owner
//               of a file associated to and item blob (sItemBlobFile).
//               if the FileManager is owner of the file it will delete the file
//               when the file has no referencies (ie Channel destruction). 
//               For the case of download asynchrous of files we need to mantain
//               the file until the user sesion is closed, so we use this function
//               to stablish ownership to false

// ***************************************************************************** 
// M4DOC.DLL - M4SetFileManagerOwnerOfTheFile
m4return_t M4SetFileManagerOwnerOfTheFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_NUMBER) )		 
		
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	m4bool_t bSetFileManagerOwner = (m4bool_t) m4double_t( ai_pvArg[1] );

	return ClFileManager::Instance()->SetFileManagerOwnerOfTheFile((m4pchar_t)ai_pvArg[0], bSetFileManagerOwner);

}

// *****************************************************************************
// Function: M4GetFileManagerOwnerOfTheFile
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//          sItemBlobFile           Fichero asociado al item Blob para el que establecer el owner
//                                   (valor del item blob) 
//      OUT:  
//			bFileManagerIsOwner		booleano indicando si el FileManager debe ser
//                                  propietario o no del fichero.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function is used to obtain if the FileManager as owner o not owner
//               of a file associated to and item blob (sItemBlobFile).
//               if the FileManager is owner of the file it will delete the file
//               when the file has no referencies (ie Channel destruction). 
//               For the case of download asynchrous of files we need to mantain
//               the file until the user sesion is closed, so we use this function
//               to stablish ownership to false

// ***************************************************************************** 
// M4DOC.DLL - M4GetFileManagerOwnerOfTheFile
m4return_t M4GetFileManagerOwnerOfTheFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	m4bool_t bGetFileManagerOwner;
										   
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
		 
		
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
	// Recogemos el nombre del fichero
	bGetFileManagerOwner =  ClFileManager::Instance()->GetFileManagerOwnerOfTheFile((m4pchar_t)ai_pvArg[0]);

	//Devolvemos el valir
	ai_pvArg[1] = bGetFileManagerOwner;


	return M4_SUCCESS;

}

// *****************************************************************************
// Function: M4CreatePath
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sPathToCreate		  Path a crear
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function creates a path, starts from the begin and create
//               from the first subdirectory that doesn´t exist.  				
// ***************************************************************************** 
// M4DOC.DLL - M4CreatePath

m4return_t M4CreatePath (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	m4return_t rReturn;

// Check input parameters.
	if (( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_SYS ))
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Crea el path.
	rReturn = ClFileManager::Instance()->CreatePath(m4pchar_t(ai_pvArg[0]));

	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (rReturn); 
	return M4_SUCCESS;
}



// *****************************************************************************
// Function: M4OpenExtSearch
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sBasePath			Directorio donde se realiza la búsqueda.
//			sFileName			Condición de búsqueda del nombre.
//			tLastModified		Condición de búsqueda de fecha.
//			bRecurse			Flag para búsqueda recursiva por los direct.
//			bDirOnly			Flag para la búsqueda de directorios.
//			iMaxDeep			Máxima profundidad en directorios.
//			hExtSearchHandle	Devuelve el handle a búsqueda extendida.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función crea una nueva estructura de búsqueda extendida.
//               y devuelve el handle a dicha búsqueda.
// ***************************************************************************** 
// M4DOC.DLL - M4OpenExtSearch
m4return_t M4OpenExtSearch (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ((( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_SYS )) ||
		(( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_SYS )) ||
		(ai_pvArg[3].GetType() != M4CL_CPP_TYPE_NUMBER) ||
		(ai_pvArg[5].GetType() != M4CL_CPP_TYPE_NUMBER) )		 
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "M4OpenExtSearch");
		return M4_ERROR;
	}
	
	// Si el argumento 2 no es de tipo fecha lo inicializa a cero y le cambia el tipo.
	if ((ai_pvArg[2].GetType() != M4CL_CPP_TYPE_DATE))
	{
		// Pone la fecha a cero.
		ai_pvArg[2].SetDouble (0);
		ai_pvArg[2].Type = M4CL_CPP_TYPE_DATE;
	}
	// Si el argumento 4 no es de tipo numérico lo inicializa a cero y le cambia el tipo.
	if ((ai_pvArg[4].GetType() != M4CL_CPP_TYPE_NUMBER))
	{
		// Pone la fecha a cero.
		ai_pvArg[4].SetDouble (0);
	}
	
	// Recogemos los valores de la llamada
	string sBasePath = m4pchar_t(ai_pvArg[0]);
	string sFileName = m4pchar_t(ai_pvArg[1]);
	m4date_t tLastModified = (m4date_t) m4double_t (ai_pvArg[2]);
	m4bool_t bDirOnly = (m4bool_t) m4double_t (ai_pvArg[3]);
	m4int32_t iMaxDeep = (m4int32_t) m4double_t (ai_pvArg[4]);
	m4int64_t hExtSearchHdl = (m4int64_t) m4double_t (ai_pvArg[5]);

	
	// Comprueba si existe el path.
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( sBasePath.c_str (), iLength ) ;
	int iAccess = access( pcFileName, 0 ) ;
	delete [] pcFileName ;
	
	if( iAccess != 0 )
	{
		// El path dado no existe.
		M4DOCErrorHandling (M4DOCERROR_NO_VALID_PATH, sBasePath.c_str());
		return M4_ERROR;
	}
	
	if( oExtSearchMng.OpenExtSearch (sBasePath, sFileName, tLastModified, bDirOnly, iMaxDeep, hExtSearchHdl) == M4_ERROR )
	{
		// Salida.
		ai_pvArg[5].SetDouble ((m4double_t)0); 
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	ai_pvArg[5].SetDouble ((m4double_t)hExtSearchHdl); 
	
	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}



// *****************************************************************************
// Function: M4CloseExtSearch
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			hExtSearchHandle	Handle a búsqueda extendida.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función elimina la estructura de búsqueda extendida dada
//               por su handle.
// ***************************************************************************** 
// M4DOC.DLL - M4CloseExtSearch
m4return_t M4CloseExtSearch (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_NUMBER )		 
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "M4CloseExtSearch");
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	m4int64_t hExtSearchHdl = (m4int64_t) m4double_t (ai_pvArg[0]);

	if( oExtSearchMng.CloseExtSearch (hExtSearchHdl) == M4_ERROR )
	{
		// Salida.
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	
	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}




// *****************************************************************************
// Function: M4GetNextFile
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			hExtSearchHandle	Handle a búsqueda extendida.
//			sNextFileName		Devuelve el nombre del siguiente fichero encontrado o
//								"" si no se encuentra ninguno más.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función devuelve el siguiente fichero localizado en la búsqueda
//				 dada por su handle.
// ***************************************************************************** 
// M4DOC.DLL - M4GetNextFile
m4return_t M4GetNextFile (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ((ai_pvArg[0].GetType() != M4CL_CPP_TYPE_NUMBER) ||
		(( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_SYS )) )		 
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "M4GetNextFile");
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	m4int64_t hExtSearchHdl = (m4int64_t) m4double_t (ai_pvArg[0]);
	string sNextFileName = m4pchar_t(ai_pvArg[1]);

	if( oExtSearchMng.GetNextFile (hExtSearchHdl, sNextFileName) == M4_ERROR )
	{
		// Salida.
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	ai_pvArg[1].SetSysString ( (m4pchar_t)sNextFileName.c_str () );	

	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}



// *****************************************************************************
// Function: M4GetFileSize
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sFilePath		Path del fichero.
//			iFileSize		Devuelve el tamaño del fichero dado.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función devuelve el tamaño del fichero dado por su Path.
// ***************************************************************************** 
// M4DOC.DLL - M4GetFileSize
m4return_t M4GetFileSize (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ((( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_SYS )) ||
		(ai_pvArg[1].GetType() != M4CL_CPP_TYPE_NUMBER))
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "M4GetFileSize");
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	string sFilePath = m4pchar_t(ai_pvArg[0]);
	m4uint32_t iFileSize = 0;

	if( GetFileSize (sFilePath, iFileSize) == M4_ERROR )
	{
		// Salida.
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	ai_pvArg[1].SetDouble ((m4double_t)iFileSize); 
	
	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}



//-------------------------------------------------------------------------------------------
// Función que devuelve el tamaño del fichero dado por el path 'ai_sFilePath'.
//
// ARGUMENTOS:
//		const string& ai_sFilePath	->	Fichero del que se quiere el tamaño.
//		m4uint32_t& ao_iFileSize	->	Devuelve el tamaño del fichero dado.
//
// RETORNO:
//		m4return_t	->	Devuelve SUCCESS o ERROR dependiendo de si se ha podido realizar.
//
//-------------------------------------------------------------------------------------------
m4return_t GetFileSize (const string& ai_sFilePath, m4uint32_t& ao_iFileSize)
{
	ao_iFileSize = 0;

	ClFile oFile (ai_sFilePath.c_str());

	if (oFile.IsFile() == M4_FALSE)
	{
		// No existe el fichero dado.
		M4DOCErrorHandling (M4DOCERROR_FILENOEXISTS, ai_sFilePath.c_str());
		return M4_ERROR;
	}
	
	ao_iFileSize = oFile.GetSize();
	return M4_SUCCESS;
}



// *****************************************************************************
// Function: M4GetMD5Key
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			sDocPath	Path del fichero al que se ha de calcular la clave MD5.
//			sMD5Key		Devuelve la clave MD5 del fichero dado.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función devuelve la clave MD5 del fichero dado por su Path.
// ***************************************************************************** 
// M4DOC.DLL - M4GetMD5Key
m4return_t M4GetMD5Key (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ((( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_SYS )) ||
		(( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_SYS )) )		 
	{
		M4DOCErrorHandling (M4DOCERROR_CHECKNULLS, "M4GetMD5Key");
		return M4_ERROR;
	}
	
	// Recogemos los valores de la llamada
	string sDocPath = m4pchar_t(ai_pvArg[0]);
	string sMD5Key = m4pchar_t(ai_pvArg[1]);


	if( GetMD5KeyFromPath (sDocPath, sMD5Key) == M4_ERROR )
	{
		// Salida.
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	ai_pvArg[1].SetSysString ( (m4pchar_t)sMD5Key.c_str () );	
	
	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}



//-------------------------------------------------------------------------------------------
// Función que pasa el fichero dado por 'ai_sDocPath' a un buffer de memoria y le calcula 
// la clave MD5, llamando a la función 'EncryptOneWay()' de la DLL 'm4cryptc.dll' 
//
// ARGUMENTOS:
//		const string& ai_sDocPath	->	Fichero del que se quiere calcular la clave MD5.
//		string& ao_sMD5Key			->	Devuelve la clave MD5.
//
// RETORNO:
//		m4return_t	->	Devuelve SUCCESS o ERROR dependiendo de si se ha podido realizar.
//
//-------------------------------------------------------------------------------------------
m4return_t GetMD5KeyFromPath (const string& ai_sDocPath, string& ao_sMD5Key)
{
	//----------------------------------------------------------------------------------------
	// Se abre el fichero fuente para lectura en modo binario. 
	// Si no existe el fichero da error.

	ifstream SourceFile;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    M4Open( SourceFile, ai_sDocPath.c_str(), M4UniReadBinary, eEncoding );

	if( !SourceFile )
	{
		// No se ha podido abrir el fichero fuente.
		M4DOCErrorHandling (M4DOCERROR_FILENOEXISTS, ai_sDocPath.c_str());
		return M4_ERROR;
	}

	// Bug 0216533. Posicionarse en el principio del fichero para no perder el BOM en 
	// caso de ficheros UTF8.
	SourceFile.seekg(0, ios::beg);
	
	// Toma llongitud del fichero en bytes.
	m4uint32_t uiLen = ClFileState(ai_sDocPath.c_str()).GetSize();

	// Reserva un buffe de memoria de tamaño igual al del fichero.
	m4char_t *cBuffer = new m4char_t[uiLen + 1];

	if( cBuffer == 0 )
	{
		// Cierra el fichero.
		SourceFile.close ();
		// No hay memoria suficiente para almacenar el fichero dado.
		M4DOCErrorHandling (M4DOCERROR_MEMORY, "");
		return M4_ERROR;
	}
	
	m4uint32_t i=0;
	m4char_t cAux = 0;
	while( SourceFile.get (cAux) )
	{
		// Guarda en el buffer el caracter leido.
		cBuffer[i] = cAux;
		i++;
	}
	cBuffer[i] = '\0';

	// Se ha producido un error de lectura.
	if( !SourceFile.eof () )
	{
		// Cierra el fichero.
		SourceFile.close ();
		// Libera la memoria reservada para guardar el fichero.
		delete [] cBuffer;
		
		M4DOCErrorHandling (M4DOCERROR_READING_FILE, ai_sDocPath.c_str());
		return M4_ERROR;
	}

	// Cierra el fichero.
	SourceFile.close ();
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------
	// Calcula la clave MD5 del buffer.
	// Bug 0216533. Para soportar ficheros UTF16, llamamos directamente a la función de encriptar
	// de m4cryptc para poder pasar la longitud del buffer de entrada.
	m4char_t	cDest[2 * M4_MAX_LEN_MD5 + 1] = "";
	m4int_t		iLength = -1;
	EncryptOneWayMD5(cBuffer, i, cDest, 2 * M4_MAX_LEN_MD5, iLength);
	//----------------------------------------------------------------------------------------

	// Libera la memoria reservada para guardar el fichero.
	delete [] cBuffer;

	// Devuelve la clave MD5.
	ao_sMD5Key = (string)cDest;

	return M4_SUCCESS;
}



// FUnciones auxiliares para la ejecución del SPider de Verity

// *****************************************************************************
// Function: M4ExecuteCommand
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			[0]:				comando a ejecutar, incluidos los parámetros.
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función lanza un system para ejecutar un comando en el sistema operativo.
// ***************************************************************************** 
m4return_t M4ExecuteCommand(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,  m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR))
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"M4ExecuteCommand");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	int iLength = -1 ;
	char* pcCommand = M4CppToANSI( ai_pvArg[0].Data.PointerChar, iLength ) ;
	m4return_t ret = system (pcCommand);
	delete pcCommand ;

	if (ret != M4_SUCCESS)
	{
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_ERROR;
	}

	ai_pvReturn.SetDouble(M4_SUCCESS); 
	return M4_SUCCESS;

}

// *****************************************************************************
// Function: M4DeleteFile
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			[0]:				path del archivo a borrar
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función borra un archivo
// ***************************************************************************** 
m4return_t M4DeleteFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR))
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"M4DeleteFile");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	// Borramos el archivo
	int	iRet = M4RemoveFile( ai_pvArg[0].Data.PointerChar ) ;
	
	// Ya está
	if (iRet==0)
		ai_pvReturn.SetDouble(M4_SUCCESS); 
	else
		ai_pvReturn.SetDouble(M4_ERROR); 
	
	return M4_SUCCESS;

}




//-------------------------------------------------------------------------------------------
// Función que borra el directorio dado por el path 'ai_sDocPath' según el modo dado por el
// argumento ai_iMode:
//						0 se borra solo el directorio (rmdir)
//						1 se borra el directorio y los archivos
//						2 se borra el directorio y los subdirectorios (deltree)
//
// ARGUMENTOS:
//		const string& ai_sDirPath	->	Path del directorio a borrar.
//		const m4int32_t ai_iMode	->	Modo de borrado.
//
// RETORNO:
//		m4return_t	->	Devuelve SUCCESS o ERROR dependiendo de si se ha podido realizar.
//
//-------------------------------------------------------------------------------------------
m4return_t RemoveDir (const string& ai_sDirPath, const m4int32_t ai_iMode)
{

	int iResult = -1 ;

	switch (ai_iMode)
	{
		// Caso 0.
		case 0:
			// Borra el directorio si existe y está vacio.
			iResult = M4RemoveDirectory( ai_sDirPath.c_str() );

			if ( iResult != 0)
			{
				// El directorio dado no está vacio o no existe.
				M4DOCErrorHandling (M4DOCERROR_DIR_NOT_EMPTY, ai_sDirPath.c_str());
				return M4_ERROR;
			}
			break;


		// Caso 1.
		case 1:
			// Elimina los archivos que contiene el directorio.
			if ( ClFileDirOperations::RemoveDirectory (ai_sDirPath.c_str(), M4_FALSE) != M4_SUCCESS )
			{
				return M4_ERROR;
			}
			break;


		// Caso 2.
		case 2:
		default:
			// Elimina los archivos y directorios que contiene el directorio dado.
			if ( ClFileDirOperations::RemoveDirectory (ai_sDirPath.c_str(), M4_TRUE) != M4_SUCCESS )
			{
				M4DOCErrorHandling (M4DOCERROR_DIR_NOT_EMPTY, ai_sDirPath.c_str());
				return M4_ERROR;
			}
			break;
			
	}

	return M4_SUCCESS;
}



// *****************************************************************************
// Function: M4RemoveDir
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//
//			[0]:				path del directorio a borrar
//			[1]:				modo de borrado
//									0 se borra solo el directorio (rmdir)
//									1 se borra el directorio y los archivos
//									2 se borra el directorio y los subdirectorios (deltree)
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: Esta función borra un archivo
// ***************************************************************************** 
m4return_t M4RemoveDir(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Check input parameters.
	if ((( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR ) && ( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_SYS )) ||
		(ai_pvArg[1].GetType() != M4CL_CPP_TYPE_NUMBER))
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"M4RemoveDir");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	
	}

	// Recogemos los valores de la llamada
	string sDocPath = m4pchar_t(ai_pvArg[0]);
	m4int32_t iMode = (m4int32_t) m4double_t (ai_pvArg[1]);


	if( RemoveDir (sDocPath, iMode) == M4_ERROR )
	{
		// Salida.
		ai_pvReturn.SetDouble (M4_ERROR); 
		return M4_SUCCESS;
	}

	// Salida.
	
	// Devuelve M4_SUCCESS.
	ai_pvReturn.SetDouble (M4_SUCCESS); 
	return M4_SUCCESS;
}




// ============================================================================
//
// Copia el contenido de un fichero a un item de tipo binario
//
// ============================================================================

m4return_t M4FileToBinary( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t		iResult = M4_ERROR ;
	int				iClose = -1 ;
	size_t			iRead = 0 ;
	size_t			iPosition = 0 ;
	ClItem			*poItem = NULL ;
	m4char_t		acBuffer[ M4DOC_BUFFER_SIZE + 1 ] ;
	FILE			*pfFile = NULL ;
	ClVMRunContext	*poContext = NULL ;


	ai_pvReturn.SetDouble( M4_ERROR ) ; 


	// Se controla el número y el tipo de los argumentos
	CHECK_CHLOG_ERRORF( ai_iLongArg != 2, M4_ERROR, M4DOCERROR_BAD_PARAM_NUMBER, "M4FileToBinary" << 2 << ai_iLongArg ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "file" << "M4FileToBinary" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "item" << "M4FileToBinary" << M4ClCppType( ai_pvArg[ 1 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;


	poContext = (ClVMRunContext*) ai_pvContext ;

	// Se obtiene el item
	poItem = &( poContext->m_pRecordSet->Register[ poContext->m_oRegIndex ].Item[ ai_pvArg[ 1 ].Data.PointerChar ] ) ;

	// Se controla que el item exista y sea de tipo binario
	CHECK_CHLOG_ERRORF( poItem->GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4DOCERROR_NO_SUCH_ITEM, ai_pvArg[ 1 ].Data.PointerChar << "M4FileToBinary" ) ;
	CHECK_CHLOG_ERRORF( poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING, M4_ERROR, M4DOCERROR_ITEM_TYPE_MISMATCH, ai_pvArg[ 1 ].Data.PointerChar << M4ClM4Type( poItem->ItemDef.M4Type() ) << M4ClM4Type( M4CL_M4_TYPE_BINARY_STRING ) << "M4FileToBinary" ) ;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( ai_pvArg[ 0 ].Data.PointerChar, M4UniReadBinary, eEncoding );
	CHECK_CHLOG_ERRORF( pfFile == NULL, M4_ERROR, M4DOCERROR_NO_OPEN_FILE, ai_pvArg[ 0 ].Data.PointerChar << "read" << strerror( errno ) << "M4FileToBinary" ) ;

	// Bug 0192840. Posicionarse al principio del fichero para no perder el BOM UTF8.
	fseek( pfFile, 0, SEEK_SET ) ;
	iPosition = 0 ;

	do
	{
		iRead = fread( acBuffer, 1, M4DOC_BUFFER_SIZE, pfFile ) ;

		if( iRead > 0 )
		{
			iResult = poItem->Value.SetNotUpdated( acBuffer, iRead, iPosition, NULL, M4_TRUE, M4_TRUE );

			// Se controla que se haya podido establecer el valor
			if( iResult == M4_ERROR )
			{
				iClose = fclose( pfFile ) ;
				DUMP_CHLOG_ERRORF( M4DOCERROR_NO_SET_VALUE, ai_pvArg[ 1 ].Data.PointerChar << "M4FileToBinary" ) ;
				return( M4_ERROR ) ;
			}
			iPosition += iRead ;
		}

	} while( iRead > 0 ) ;

	// Bug 0167005, 0207835. Añadir un byte a nulo más porque luego, al persistir, la BDL lo quita para los campos
	// binarios (M4CL_M4_TYPE_BINARY_STRING).
	acBuffer[0] = '\0';
	iRead = 1;
	iResult = poItem->Value.SetNotUpdated( acBuffer, iRead, iPosition, NULL, M4_TRUE, M4_TRUE );
	
	// Se controla que se haya podido establecer el valor
	if( iResult == M4_ERROR )
	{
		iClose = fclose( pfFile ) ;
		DUMP_CHLOG_ERRORF( M4DOCERROR_NO_SET_VALUE, ai_pvArg[ 1 ].Data.PointerChar << "M4FileToBinary" ) ;
		return( M4_ERROR ) ;
	}
	
	iClose = fclose( pfFile ) ;

	ai_pvReturn.SetDouble( M4_SUCCESS ) ; 

	return( M4_SUCCESS ) ;
}



// ============================================================================
//
// Copia el contenido de un item de tipo binario a un fichero
//
// bg 173734.  
// Debido al comportamiento de bdl con los tipos de datos m4 3, 11 y 14, por
// el que se elimina el NULL final (si existe) al grabar en bbdd y siempre se
// añade al leer (asimetría), tratamos de minimizar las ocasiones en las que
// se añade innecesariamente dicho NULL. Para ello recibimos un nuevo argumento
// en el que se indique el tipo de contenido, para en caso de ser texto y contar 
// con un NULL al final, eliminarlo, asumiendo que bdl lo añadió.
// ============================================================================

m4return_t M4BinaryToFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	int				iClose = -1 ;
	size_t			iWrite = 0 ;
	m4uint32_t		iSize = 0 ;
	m4pvoid_t		pvValue = NULL ;
	ClItem			*poItem = NULL ;
	FILE			*pfFile = NULL ;
	ClVMRunContext	*poContext = NULL ;
	m4bool_t		bDecSize = M4_FALSE ;
	m4int32_t		iVersionContent = 0;


	ai_pvReturn.SetDouble( M4_ERROR ) ; 


	// Se controla el número y el tipo de los argumentos
	CHECK_CHLOG_ERRORF( ai_iLongArg < 2, M4_ERROR, M4DOCERROR_BAD_PARAM_NUMBER, "M4BinaryToFile" << 2 << ai_iLongArg ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "item" << "M4BinaryToFile" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "file" << "M4BinaryToFile" << M4ClCppType( ai_pvArg[ 1 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;

	// bg 173734
	if( ai_iLongArg > 2 && ai_pvArg[ 2 ].Type != M4CL_CPP_TYPE_NULL )
	{
		CHECK_CHLOG_ERRORF( ai_pvArg[ 2 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "type" << "M4BinaryToFile" << M4ClCppType( ai_pvArg[ 2 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
		bDecSize = !strcmpi( ai_pvArg[ 2 ].Data.PointerChar, "TEXT" ) ;
	}

	// Bug 0167005, 0207835. Nuevo parámetro VERSION_CONTENT para diferenciar los documentos antiguos y documentos
	// grabados a partir del arreglo de estos bugs.
	// VERSION_CONTENT = 0 -> documentos antiguos.
	// VERSION_CONTENT = 1 -> documentos nuevos.
	if( ai_iLongArg > 3 && ai_pvArg[ 3 ].Type != M4CL_CPP_TYPE_NULL )
	{
		CHECK_CHLOG_ERRORF( ai_pvArg[ 3 ].Type != M4CL_CPP_TYPE_NUMBER, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "type" << "M4BinaryToFile" << M4ClCppType( ai_pvArg[ 3 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) ) ;
		iVersionContent = ai_pvArg[ 3 ].Data.DoubleData;
	}

	poContext = (ClVMRunContext*) ai_pvContext ;

	// Se obtiene el item
	poItem = &( poContext->m_pRecordSet->Register[ poContext->m_oRegIndex ].Item[ ai_pvArg[ 0 ].Data.PointerChar ] ) ;

	// Se controla que el item exista y sea de tipo binario
	CHECK_CHLOG_ERRORF( poItem->GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4DOCERROR_NO_SUCH_ITEM, ai_pvArg[ 0 ].Data.PointerChar << "M4BinaryToFile" ) ;
	CHECK_CHLOG_ERRORF( poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING, M4_ERROR, M4DOCERROR_ITEM_TYPE_MISMATCH, ai_pvArg[ 0 ].Data.PointerChar << M4ClM4Type( poItem->ItemDef.M4Type() ) << M4ClM4Type( M4CL_M4_TYPE_BINARY_STRING ) << "M4BinaryToFile" ) ;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( ai_pvArg[ 1 ].Data.PointerChar, M4UniWriteBinary, eEncoding );
	CHECK_CHLOG_ERRORF( pfFile == NULL, M4_ERROR, M4DOCERROR_NO_OPEN_FILE, ai_pvArg[ 1 ].Data.PointerChar << "write" << strerror( errno ) << "M4BinaryToFile" ) ;

	iSize = poItem->Value.Size() ;
	pvValue = poItem->Value.Get() ;

	// Eliminar el último byte nulo, si existe, porque ha sido añadido por bdl, sólo para los documentos nuevos.
	if (iVersionContent == 0)
	{
		// bg 173734
		if( bDecSize != M4_FALSE && iSize > 0 )
		{
			// Bug 0192835. Se pierde el último carácter cuando el contenido del campo es UTF16.
			// Si el contenido es UTF16, el tamaño es impar y el último byte es nulo, lo quitamos.
			if (memcmp(pvValue, acUTF16BOM, M4UNI_UTF16_BOM_SIZE) == 0)
			{
				if ((iSize % 2 != 0) && ((char *) pvValue)[iSize - 1] == NULL)
				{
					--iSize ;
				}
			}
			else
			{
				if( ((char*)pvValue)[ iSize - 1 ] == NULL )
				{
					--iSize ;
				}
			}
		}
	}
	else
	{
		// Bug 0304222. Comprobar que el fichero no viene vacío.
		if( iSize > 0 )
		{
			if( ((char *) pvValue)[ iSize - 1 ] == NULL )
			{
				--iSize ;
			}
		}
	}

	iWrite = fwrite( pvValue, 1, iSize, pfFile ) ;
	iClose = fclose( pfFile ) ;
	CHECK_CHLOG_ERRORF( iWrite != iSize, M4_ERROR, M4DOCERROR_NO_WRITE_FILE, ai_pvArg[ 1 ].Data.PointerChar << strerror( errno ) << "M4BinaryToFile" ) ;


	ai_pvReturn.SetDouble( M4_SUCCESS ) ; 

	return( M4_SUCCESS ) ;
}





// ============================================================================
//
// Copia el contenido de una cadena a un item de tipo binario en formato UTF-16
//
// ============================================================================

m4return_t M4StringToBinary( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t		iResult = M4_ERROR ;
	int				iLength = -1 ;
	M4XMLCh			*pwcString ;
	ClItem			*poItem = NULL ;
	ClVMRunContext	*poContext = NULL ;


	ai_pvReturn.SetDouble( M4_ERROR ) ; 


	// Se controla el número y el tipo de los argumentos
	CHECK_CHLOG_ERRORF( ai_iLongArg != 2, M4_ERROR, M4DOCERROR_BAD_PARAM_NUMBER, "M4StringToBinary" << 2 << ai_iLongArg ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "string" << "M4StringToBinary" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "item" << "M4StringToBinary" << M4ClCppType( ai_pvArg[ 1 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;


	poContext = (ClVMRunContext*) ai_pvContext ;

	// Se obtiene el item
	poItem = &( poContext->m_pRecordSet->Register[ poContext->m_oRegIndex ].Item[ ai_pvArg[ 1 ].Data.PointerChar ] ) ;

	// Se controla que el item exista y sea de tipo binario
	CHECK_CHLOG_ERRORF( poItem->GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4DOCERROR_NO_SUCH_ITEM, ai_pvArg[ 1 ].Data.PointerChar << "M4StringToBinary" ) ;
	CHECK_CHLOG_ERRORF( poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING, M4_ERROR, M4DOCERROR_ITEM_TYPE_MISMATCH, ai_pvArg[ 1 ].Data.PointerChar << M4ClM4Type( poItem->ItemDef.M4Type() ) << M4ClM4Type( M4CL_M4_TYPE_BINARY_STRING ) << "M4StringToBinary" ) ;

	iLength = -1 ;
	pwcString = M4CppToUtf16( ai_pvArg[ 0 ].Data.PointerChar, iLength ) ;

	// Se escribe la cabecera utf-16
	iResult = poItem->Value.SetNotUpdated( acUTF16BOM, M4UNI_UTF16_BOM_SIZE, 0, NULL, M4_TRUE, M4_TRUE );
	CHECK_CHLOG_ERRORF( iResult == M4_ERROR, M4_ERROR, M4DOCERROR_NO_SET_VALUE, ai_pvArg[ 1 ].Data.PointerChar << "M4StringToBinary" ) ;

	// Se escribe el contenido en utf-16
	iResult = poItem->Value.SetNotUpdated( pwcString, ( iLength + 1 ) * 2, M4UNI_UTF16_BOM_SIZE, NULL, M4_TRUE, M4_TRUE );
	CHECK_CHLOG_ERRORF( iResult == M4_ERROR, M4_ERROR, M4DOCERROR_NO_SET_VALUE, ai_pvArg[ 1 ].Data.PointerChar << "M4StringToBinary" ) ;

	ai_pvReturn.SetDouble( M4_SUCCESS ) ; 

	return( M4_SUCCESS ) ;
}



// ============================================================================
//
// Copia el contenido de un item de tipo binario en formato UTF-16 a una cadena
//
// ============================================================================

m4return_t M4BinaryToString( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	int				iLength = -1 ;
	m4uint32_t		iSize = 0 ;
	m4pchar_t		pcString = NULL ;
	m4pvoid_t		pvValue = NULL ;
	ClItem			*poItem = NULL ;
	ClVMRunContext	*poContext = NULL ;


	ai_pvReturn.SetDouble( M4_ERROR ) ; 


	// Se controla el número y el tipo de los argumentos
	CHECK_CHLOG_ERRORF( ai_iLongArg != 2, M4_ERROR, M4DOCERROR_BAD_PARAM_NUMBER, "M4BinaryToString" << 2 << ai_iLongArg ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "item" << "M4BinaryToString" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;


	poContext = (ClVMRunContext*) ai_pvContext ;

	// Se obtiene el item
	poItem = &( poContext->m_pRecordSet->Register[ poContext->m_oRegIndex ].Item[ ai_pvArg[ 0 ].Data.PointerChar ] ) ;

	// Se controla que el item exista y sea de tipo binario
	CHECK_CHLOG_ERRORF( poItem->GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4DOCERROR_NO_SUCH_ITEM, ai_pvArg[ 0 ].Data.PointerChar << "M4BinaryToString" ) ;
	CHECK_CHLOG_ERRORF( poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING, M4_ERROR, M4DOCERROR_ITEM_TYPE_MISMATCH, ai_pvArg[ 0 ].Data.PointerChar << M4ClM4Type( poItem->ItemDef.M4Type() ) << M4ClM4Type( M4CL_M4_TYPE_BINARY_STRING ) << "M4BinaryToString" ) ;

	iSize = poItem->Value.Size() ;
	pvValue = poItem->Value.Get() ;

	// Se comprueba que empiece por la cabecera utf-16 y que sean bytes pares
	CHECK_CHLOG_ERRORF(
		iSize < M4UNI_UTF16_BOM_SIZE
		||
		( iSize % 2 ) != 0
		||
		(
			memcmp( pvValue, acUTF16WinBOM, M4UNI_UTF16_BOM_SIZE ) != 0
			&&
			memcmp( pvValue, acUTF16UnixBOM, M4UNI_UTF16_BOM_SIZE ) != 0
		),
		M4_ERROR, M4DOCERROR_NO_UTF16_FORMAT, ai_pvArg[ 0 ].Data.PointerChar << "M4BinaryToString" ) ;

	/* Bug 0146694
	Si la cabecera es de la plataforma contraria se hace swap de los bytes
	*/
	if( memcmp( pvValue, acUTF16BOM, M4UNI_UTF16_BOM_SIZE ) != 0 )
	{
		M4SwapUtf16( (M4XMLCh*) pvValue, iSize / sizeof( unsigned short ) ) ;
	}

	iLength = -1 ;
	pcString = M4Utf16ToCpp( (M4XMLCh*) ( (m4pchar_t) pvValue + M4UNI_UTF16_BOM_SIZE ), iLength ) ;

	ai_pvArg[ 1 ].SetSysString( pcString ) ;
	delete [] pcString ;

	ai_pvReturn.SetDouble( M4_SUCCESS ) ; 

	return( M4_SUCCESS ) ;
}


// ============================================================================
// Function: M4ExistDir 
// Arguments:																	
//		IN:
//				filePath - directory
//
// Check if a given directory exists.
//
// ============================================================================
m4return_t M4ExistDir( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Inicializaciones.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Argumento de salida, indicando si el fichero existe.
	ai_pvArg[1].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[1].Data.DoubleData = M4_FALSE;

	// Verificar el número de argumentos que se le pasan al método.
	CHECK_CHLOG_ERROR(ai_iLongArg != 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// Obtener la ruta al directorio.
	CHECK_CHLOG_ERROR( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);
	CHECK_CHLOG_ERROR( ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0', M4_SUCCESS, M4DOCERROR_CHECKNULLS);

	// Comprobar que es un directorio.
	if (ClFileName(ai_pvArg[0].Data.PointerChar).IsDir() == M4_TRUE)
	{
		ai_pvArg[1].Data.DoubleData = M4_TRUE;
	}

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

// ============================================================================
// Function: M4ExistFile 
// Arguments:																	
//		IN:
//				fileName - file path
//
// Check if file exists.
//
// ============================================================================
m4return_t M4ExistFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// Inicializaciones.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Argumento de salida, indicando si el fichero existe.
	ai_pvArg[1].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[1].Data.DoubleData = M4_FALSE;

	// Verificar el número de argumentos que se le pasan al método.
	CHECK_CHLOG_ERROR(ai_iLongArg != 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// Obtener la ruta al fichero.
	CHECK_CHLOG_ERROR(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);
	CHECK_CHLOG_ERROR(ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0', M4_SUCCESS, M4DOCERROR_CHECKNULLS);

	// Verificar si el fichero existe.
	if (ClFileName(ai_pvArg[0].Data.PointerChar).IsFile() == M4_TRUE)
	{
		ai_pvArg[1].Data.DoubleData = M4_TRUE;
	}

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

// ============================================================================
// Function: M4GetDir 
// Arguments:																	
//		IN:
//				filepath - file path
//
// Check if file exists.
//
// ============================================================================
m4return_t M4GetDir( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	ClFileName	oDirName;

	// Inicializaciones.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Argumento de salida, indicando el directorio al que apunta el fichero especificado.
	ai_pvArg[1].SetNull();

	// Verificar el número de argumentos que se le pasan al método.
	CHECK_CHLOG_ERROR(ai_iLongArg != 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// Obtener la ruta al fichero.
	CHECK_CHLOG_ERROR(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);
	CHECK_CHLOG_ERROR(ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0', M4_SUCCESS, M4DOCERROR_CHECKNULLS);

	// Verificar si la ruta proporcionada es un directorio o un fichero.
	if (ClFileName(ai_pvArg[0].Data.PointerChar).IsDir() == M4_TRUE)
	{
		ai_pvArg[1].SetSysString(ai_pvArg[0].Data.PointerChar);
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;
		return M4_SUCCESS;
	}
	
	// Verificar que el directorio del fichero existe.
	if (ClFileName(ai_pvArg[0].Data.PointerChar).GetDir(oDirName) != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	if (oDirName.IsDir() == M4_FALSE)
	{
		return M4_SUCCESS;
	}
	
	// Obtener el nombre del fichero.
	ai_pvArg[1].SetSysString((m4pchar_t)oDirName.GetFileName());
	
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

/********************************************************************************/
// Function: M4OpenDefBrowser						
// Module m4doc.dll
// Arguments:								
//
//		Input:
//			sURL:			Web URL to open
//
//		Return:
//			M4_SUCCESS / M4_ERROR
/********************************************************************************/
m4return_t M4OpenDefBrowser( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
#ifdef _UNIX
	
	M4DOCErrorHandling( M4DOCERROR_NOT_SUPPORTED, "M4OpenDefBrowser" ) ;
	return M4_ERROR ;

#else

	int			iLength ;
	M4XMLCh*	pwcURL ;
	BOOL		bRet ;


	// Input argument checking.
	if( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR )
	{
		M4DOCErrorHandling( M4DOCERROR_CHECKNULLS, "M4OpenDefBrowser" ) ;
		return M4_ERROR ;
	}


	iLength = -1 ;
	pwcURL = M4CppToUtf16( ai_pvArg[0].Data.PointerChar, iLength ) ;

	
	// Starts the browser.
	SHELLEXECUTEINFOW sei = { sizeof(sei), 0, NULL, L"open", (LPCWSTR) pwcURL, NULL, NULL, SW_SHOWNORMAL, NULL } ;
	bRet = ShellExecuteExW( &sei ) ;
	
	delete pwcURL;


	if( bRet == false )
	{
		LPVOID	lpMsgBuf ;
		DWORD	dwErrorCode = GetLastError() ;

		FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
						NULL, dwErrorCode, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), 
						(LPWSTR)&lpMsgBuf, 0, NULL) ;

		iLength = -1 ;
		m4pchar_t pcError = M4Utf16ToCpp( (M4XMLCh*) lpMsgBuf, iLength ) ;

		SETCODEF( M4DOCERROR_DEF_BROWSER, ERRORLOG, "#*s1*#%d#%s#", dwErrorCode, pcError ) ;

		LocalFree( lpMsgBuf ) ;
		delete pcError ;

		ai_pvReturn.SetDouble( M4_ERROR ) ;
		return M4_SUCCESS ;
	};


	// a short delay while browser starts.
	//Sleep( 1000 ) ;

	ai_pvReturn.SetDouble( M4_SUCCESS ) ;
	return M4_SUCCESS ;

#endif
}

// *****************************************************************************
// Function: M4GenerateZip
// Module M4DOC.DLL
// Arguments:							
//		IN: (As the function can be called from LN4 the first argument is a array of variants
//
//			[0]:				path to output file to be generated
//			[1]..[n]:			input files to be included (variable length)
//
//		RETURN:
//			M4_SUCCESS or M4_ERROR
//
//  Description: This function compress a file
// ***************************************************************************** 
m4return_t M4GenerateZip(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	int		iLength = -1;
	char	* pcFile = NULL;

	// Initialization
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Checks number of arguments
	CHECK_CHLOG_ERROR(ai_iLongArg < 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// check input types
	int i;
	for (i=0; i<ai_iLongArg; ++i)
	{
		CHECK_CHLOG_ERROR(ai_pvArg[i].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);
		CHECK_CHLOG_ERROR(ai_pvArg[i].Data.PointerChar == NULL || *(ai_pvArg[i].Data.PointerChar) == '\0', M4_SUCCESS, M4DOCERROR_CHECKNULLS);
	}

	// gets the input file list
	const m4int32_t N_ARGS = 2;
	m4pchar_t *vpcInputList = new m4pchar_t[ai_iLongArg + N_ARGS];

	// options
	int baseArg = 0;
	vpcInputList[baseArg++] = "local"; // the file name
	vpcInputList[baseArg++] = "-j"; // ignore path
	
	for (i=0; i<ai_iLongArg; ++i)
	{
		// UNICODE FILE. Bug 0267063.
		iLength = -1;
		pcFile = M4CppToANSI(ai_pvArg[i].Data.PointerChar, iLength);
		vpcInputList[i + baseArg] = new m4char_t[iLength + 1];
		memcpy(vpcInputList[i + baseArg], pcFile, iLength + 1);
		delete pcFile;
	}

	// calls the compressor
	unsigned errorCode = 0;
	char errorParameter[1025];
	int ret = M4GenerateZipInternal(ai_iLongArg + baseArg, vpcInputList, &errorCode, errorParameter, sizeof(errorParameter));

	if (errorCode != 0)
	{
		SETCODEF( errorCode, ERRORLOG, "#*s1*#%s#%s#", "M4GenerateZip", errorParameter ) ;
	}
	else
	{
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	}

	delete [] vpcInputList;

	// ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

// *****************************************************************************
// Function: M4ExpandZip
// Module M4DOC.DLL
// Arguments:							
//		IN: (As the function can be called from LN4 the first argument is a array of variants
//
//			[0]:				path to input zip file
//			[1]:				output path
//
//		RETURN:
//			M4_SUCCESS or M4_ERROR
//
//  Description: This function unzip a file in a specified output directory
// ***************************************************************************** 
m4return_t M4ExpandZip(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	int		iLength = -1;

	// Initialization
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Checks number of arguments
	CHECK_CHLOG_ERROR(ai_iLongArg < 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// check input types
	int i;
	for (i=0; i<ai_iLongArg; ++i)
	{
		CHECK_CHLOG_ERROR(ai_pvArg[i].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);
		CHECK_CHLOG_ERROR(ai_pvArg[i].Data.PointerChar == NULL || *(ai_pvArg[i].Data.PointerChar) == '\0', M4_SUCCESS, M4DOCERROR_CHECKNULLS);
	}

	// Gets the input zip file
	// UNICODE FILE. Bug 0267063.
	m4pchar_t pcInputFile = M4CppToANSI(ai_pvArg[0].Data.PointerChar, iLength);

	// Gets the output directory
	// UNICODE FILE. Bug 0267063.
	iLength = -1;
	m4pchar_t pcOutputDir = M4CppToANSI(ai_pvArg[1].Data.PointerChar, iLength);

	// gets the input file list
	const m4int32_t N_ARGS = 5;
	m4pchar_t *vpcInputList = new m4pchar_t[ai_iLongArg + N_ARGS - 2];

	// options
	int baseArg = 0;
	vpcInputList[baseArg++] = "local"; // the file name
	vpcInputList[baseArg++] = "-o"; // overwrite
	vpcInputList[baseArg++] = pcInputFile; // the zip file
	vpcInputList[baseArg++] = "-d"; // prefix to directory param
	vpcInputList[baseArg++] = pcOutputDir; // the output directory
	
	for (i=2; i<ai_iLongArg; ++i)
	{
		vpcInputList[i + baseArg] = ai_pvArg[i].Data.PointerChar;
	}

	// calls the decompressor
	unsigned errorCode = 0;
	char errorParameter[1025];
	int ret = M4ExpandZipInternal(ai_iLongArg + baseArg - 2, vpcInputList, &errorCode, errorParameter, sizeof(errorParameter));

	if (errorCode != 0)
	{
		SETCODEF( errorCode, ERRORLOG, "#*s1*#%s#%s#", "M4ExpandZip", errorParameter ) ;
	}
	else
	{
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	}

	delete [] pcInputFile;
	delete [] pcOutputDir;
	delete [] vpcInputList;

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

// *****************************************************************************
// Function: M4GenerateCRC
// Module M4DOC.DLL
// Arguments:							
//		IN: (As the function can be called from LN4 the first argument is a array of variants
//
//			[0]:				path to input file
//			[1]:				output CRC 32 bits Value
//
//		RETURN:
//			M4_SUCCESS or M4_ERROR
//
//  Description: This function computes a 32 bits CRC valur from input file
// ***************************************************************************** 
m4return_t M4GenerateCRC(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	int		iLength = -1;

	// Initialization
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// Checks number of arguments
	CHECK_CHLOG_ERROR(ai_iLongArg != 2, M4_SUCCESS, M4DOCERROR_BAD_PARAM_NUMBER);

	// check input types
	CHECK_CHLOG_ERROR(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4DOCERROR_BAD_PARAM_TYPE);

	// Gets the input file path
	// UNICODE FILE. Bug 0267063.
	m4pchar_t pcInputFile = M4CppToANSI(ai_pvArg[0].Data.PointerChar, iLength);

	// calls the compressor
	unsigned errorCode = 0;
	char errorParameter[1025];
	unsigned long crc = 0;

	ai_pvArg[1] = -1.0;
	int ret = M4GetFileCrcInternal(pcInputFile, NULL, 0, &crc, &errorCode, errorParameter, sizeof(errorParameter));

	if (errorCode != 0)
	{
		SETCODEF( errorCode, ERRORLOG, "#*s1*#%s#%s#", "M4GenerateCRC", errorParameter) ;
	}
	else
	{
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;
		ai_pvArg[1] = crc;
	}

	delete [] pcInputFile;

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;

	return M4_SUCCESS;
}

string _UnEscapeChars(const string & ai_sText)
{
	string		sUnEscaped = "";
	m4uint32_t	i = 0;
	m4char_t	c = '\0';

	for (i = 0 ; i < ai_sText.size(); i++)
	{
		c = ai_sText[i];
		switch (c)
		{
		case '"':
			sUnEscaped += c;
			
			// Avanzamos un caracter porque la comilla doble viene 
			// escapada (dos comillas dobles).
			i++;

			break;	
		default:
			sUnEscaped += c;
			break;
		}
	}

	return sUnEscaped;
}

m4uint32_t _SplitLines(string ai_sString, const string & ai_sDelimiter, vector<string> & ao_vLines)
{
	string		sSubString = "";
	string		sLine = "";
	m4int32_t	iPos = 0;
	m4int32_t	iPosAux = 0;
	m4uint32_t	iLen = 0;
	m4uint32_t	iTotalLen = 0;	
	
	// Verificar que el string no está vacio.
	if (ai_sString.length() == 0)
	{
		return iTotalLen;
	}

	// Buscar el caracter delimitador en el string.
	iPos = ai_sString.find(ai_sDelimiter);
	if (iPos != -1)
	{
		iTotalLen = iPos + ai_sDelimiter.size();
	}

	while (iPos != -1) 
	{
		sLine = ai_sString.substr(0, iPos);
		ao_vLines.push_back(sLine);
		
		iPosAux = iPos + ai_sDelimiter.size();
		iLen = ai_sString.length();
		sSubString = ai_sString.substr(iPosAux, iLen - iPosAux);
		ai_sString.assign(sSubString);
		
		iPos = ai_sString.find(ai_sDelimiter);
		if (iPos != -1)
		{
			iTotalLen += iPos + ai_sDelimiter.size();
		}
	}

	return iTotalLen;
}

m4return_t _Tokenize(const string & ai_sString, const string & ai_sDelimiter, vector<string> & ao_vStrings)
{
	char	* pToken = NULL;
	string	sString = "";
	string	sSubStringInit = "";
	string	sSubStringEnd = "";
	string	sToken = "";
	string	sEscapedToken = "";
	string	sTokenEnd = "";
	int		iPos = 0;
	int		iLen = 0;
	int		iTokenEndLen = 0;

	// Verificar que el string no está vacio.
	if (ai_sString.length() == 0)
	{
		return M4_SUCCESS;
	}

	// Inicializaciones.
	sString = ai_sString;
	iLen = sString.length();
	sTokenEnd = "\"" + ai_sDelimiter;
	iTokenEndLen = strlen(sTokenEnd.c_str());

	// Buscar el caracter delimitador en el string.
	iPos = sString.find(ai_sDelimiter);
	while (iPos != -1) 
	{
		sSubStringInit = sString.substr(0, iPos);
		sSubStringEnd = sString.substr(iPos + 1, iLen - iPos);
		
		// Comprobar caracteres escapados dentro del csv.
		sToken = sSubStringInit;
		if (sSubStringInit.length() != 0)
		{
			// Cadenas escapadas en csv, vienen entrecomilladas.
			if (sSubStringInit[0] == '"')
			{
				// Leer hasta final del campo csv.
				iPos = sString.find(sTokenEnd);
				sSubStringEnd = sString.substr(iPos + iTokenEndLen, iLen - iPos + iTokenEndLen);
				if (iPos != -1)
				{
					sEscapedToken = sString.substr(1, iPos - 1);
					
					// Desescapar los caracteres del csv (rfc4180).
					// Fields containing line breaks (CRLF), double quotes, and commas [delimiter char]
					// should be enclosed in double-quotes.
					sToken = _UnEscapeChars(sEscapedToken);
				}
			}
		}

		ao_vStrings.push_back(sToken);
		
		sString.assign(sSubStringEnd);
		iPos = sString.find(ai_sDelimiter);
	}

	// Añadir lo que queda en caso de que no esté vacio.
	// Si los valores vienen entrecomillados, eliminar las comillas.
	if (sString.length() != 0)
	{
		sToken = sString;
		if (sString[0] == '"' && sString[sString.length() - 1] == '"')
		{
			sEscapedToken = sString.substr(1, sString.length() - 2);
			
			// Desescapar los caracteres del csv (rfc4180).
			// Fields containing line breaks (CRLF), double quotes, and commas [delimiter char]
			// should be enclosed in double-quotes.
			sToken = _UnEscapeChars(sEscapedToken);
		}
		
		ao_vStrings.push_back(sToken);
	}
	
	return M4_SUCCESS;
}


/****************************************************************************
*	Name:			LoadCSVFile
*	Description:	Dumps cell values to given node channel.
*	Parameters:
*  	Input:			ai_channel_handle: channel handle.
*					ai_node_name: node name. 
*					ai_sCSVFile: CSV file to be loaded.
*					ai_sDelimiter: character delimiter for CSV file values.
*
* 	Output:
*
*	Return:			0	No error.	
*					1   Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t LoadCSVFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	ClChannel *				pChannel = NULL;
	ClAccess *				poAccess = NULL;
	ClNode *				poNode = NULL;
	string					sCSVFilePath = "";
	string					sDelimiter = "";
	string					sLine = "";
	m4pchar_t				pcBuffer = NULL;
	ifstream				oCSVFile;
	m4VariantType			vItemValue;
	vector<string>			vColumnNames;
	vector<string>			vColumnValues;
	vector<string>			vLines;
	string					sItemName = "";
	string					sItemValue = "";
	m4return_t				iRet = M4_SUCCESS;
	eUniFileEncodingType_t	eEncoding = M4UniANSI;
	m4uint8_t				uiCppType = 0;
	m4uint8_t				uiM4Type = 0;
	m4uint32_t				uiNodeHandle = 0;
	m4uint32_t				uiItemHandle = M4CL_M4_TYPE_NULL;
	M4ClTimeStamp			oTimeStamp;
	string					sTimeStamp;
	string					sLineTerminator = "####META4_EOF####";
	m4int32_t				iLineNum = 0;
	m4int32_t				iReadLen = 0;
	m4int32_t				iTotalReadLen = 0;
	m4int32_t				iBomSize = 0;

	// Recuperar contexto.
	ClVMRunContext *  pvmContext = (ClVMRunContext *) ai_pvContext;
	
	// Establecer el valor de retorno del método.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	CHECK_CHLOG_ERRORF( ai_iLongArg != 4, M4_ERROR, M4DOCERROR_BAD_PARAM_NUMBER, "LoadCSVFile" << 4 << ai_iLongArg ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_NUMBER, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "node handle" << "LoadCSVFile" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "node" << "LoadCSVFile" << M4ClCppType( ai_pvArg[ 1 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 2 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "CSV file" << "LoadCSVFile" << M4ClCppType( ai_pvArg[ 2 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 3 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_ERROR, M4DOCERROR_BAD_PARAM_TYPE, "delimiter" << "LoadCSVFile" << M4ClCppType( ai_pvArg[ 3 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;

	// Inicializaciones
	vColumnNames.reserve(10);
	vColumnValues.reserve(10);
	vLines.reserve(10);

	// Obtener el handle al M4Object.
	pChannel = pvmContext->m_pAccess->GetpChannel()->GetpChannelManager()->GetChannelByHandle(ai_pvArg[0].Data.DoubleData);
	if (pChannel == NULL)
	{
		SETCODEF(M4DOCERROR_M4OBJECT_NOT_FOUND, ERRORLOG, "#*s1*#%s#%d#", "LoadCSVFile", (int) ai_pvArg[0].Data.DoubleData);
		return M4_ERROR;
	}

	poAccess = pChannel->CreateAccess();
	if (poAccess == NULL)
	{
		SETCODEF(M4DOCERROR_M4OBJECT_NOT_FOUND, ERRORLOG, "#*s1*#%s#%d#", "LoadCSVFile", (int) ai_pvArg[0].Data.DoubleData);
		return M4_ERROR;
	}

	// Obtener el nombre del nodo.
	poNode = &(poAccess->Node[ai_pvArg[1].Data.PointerChar]);
	if (poNode == NULL)
	{
		SETCODEF(M4DOCERROR_NODE_NOT_FOUND, ERRORLOG, "#*s1*#%s#%s#%s#", "LoadCSVFile", ai_pvArg[1].Data.PointerChar, pChannel->GetpChannelDef()->Id());
		poAccess->Destroy();
		return M4_ERROR;
	}
	
	// Obtener la ruta al fichero CSV.
	sCSVFilePath = ai_pvArg[2].Data.PointerChar;

	// Carácter delimitador de los valores en el fichero CSV.
	sDelimiter = ai_pvArg[3].Data.PointerChar;

	// Alocar buffer para leer las líneas del fichero.
	pcBuffer = new m4char_t[50 * MAX_LINE_LENGTH + 1];
	
	// Abrir el fichero CSV y recorrer sus filas. La primera fila es el nombre de las columnas.
	M4Open(oCSVFile, sCSVFilePath.c_str(), M4UniReadBinary, eEncoding);
	if (eEncoding == M4UniUtf8)
	{
		iBomSize = M4UNI_UTF8_BOM_SIZE;
	}
	
	if (oCSVFile.rdbuf()->is_open() == M4_TRUE)
	{
		while (oCSVFile.eof() == M4_FALSE)
		{
			// Bug 0261212. Inicializar buffer de lectura.
			memset(pcBuffer, '\0', 50 * MAX_LINE_LENGTH + 1);
			
			// Bug 0258730: Para evitar conflictos con los campos del fichero CSV que contienen
			// retornos de carro, cuando se serializa a CSV se ha cambiado el retorno de carro
			// como separador de líneas por la cadena "####META4_EOF####".
			oCSVFile.read(pcBuffer, 50 * MAX_LINE_LENGTH);
			
			// Comprobar que se ha leído algo.
			if (*pcBuffer == '\0')
			{
				break;
			}
			
			// Extraer las líneas, separadas por el token "####META4_EOF####".
			iReadLen = _SplitLines(pcBuffer, sLineTerminator, vLines);
			
			iTotalReadLen += iReadLen;

			// Posicionar el buffer del stream hasta la última líneas leída.
			oCSVFile.seekg(iTotalReadLen + iBomSize, ios::beg);
			
			for (m4int32_t i = 0; i < vLines.size(); i++)
			{
				sLine = vLines[i];
				if (iLineNum == 0)
				{
					// La primera línea son los nombres de las columnas de las celdas.
					_Tokenize(sLine, sDelimiter, vColumnNames);
				}
				else
				{
					if (poNode->RecordSet.Register.Add() != M4_SUCCESS)
					{
						oCSVFile.close();
						delete [] pcBuffer;
						SETCODEF(M4DOCERROR_ADD_REGISTER, ERRORLOG, "#*s1*#%s#%s#", ai_pvArg[1].Data.PointerChar, "LoadCSVFile");
						poAccess->Destroy();
						return M4_ERROR;
					}
					
					// Los valores de las columnas.
					vColumnValues.clear();
					_Tokenize(sLine, sDelimiter, vColumnValues);
					
					// Set the item values.
					for (m4int32_t j = 0; j < vColumnValues.size(); j++)
					{
						sItemName = vColumnNames[j];
						sItemValue = vColumnValues[j];
						
						// Obtener el handle del nodo.
						uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_pvArg[1].Data.PointerChar);
						
						// Obtener el handle del item.
						uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, sItemName.c_str());
						
						// Obtener el tipo cpp.
						uiCppType = poAccess->GetpCMCR()->GetItemCppType(uiItemHandle);
						
						if (uiCppType == M4CL_CPP_TYPE_NUMBER)
						{
							vItemValue.Type = M4CL_CPP_TYPE_NUMBER;
							vItemValue.Data.DoubleData = atof(sItemValue.c_str());
						}
						else if (uiCppType == M4CL_CPP_TYPE_STRING || uiCppType == M4CL_CPP_TYPE_STRING_POOL || uiCppType == M4CL_CPP_TYPE_STRING_VAR || uiCppType == M4CL_CPP_TYPE_STRING_SYS)
						{
							vItemValue.Type = M4CL_CPP_TYPE_STRING_VAR;
							vItemValue.Data.PointerChar = (m4pchar_t) sItemValue.c_str();
						}
						else if (uiCppType == M4CL_CPP_TYPE_DATE)
						{
							vItemValue.Type = M4CL_CPP_TYPE_DATE;
							
							// Obtener el tipo m4.
							uiM4Type = poAccess->GetpCMCR()->GetItemM4Type(uiItemHandle);
							if (uiM4Type == M4CL_M4_TYPE_TIMESTAMP)
							{
								sTimeStamp = sItemValue;
							}
							else if (uiM4Type == M4CL_M4_TYPE_DATE)
							{
								sTimeStamp = sItemValue + " 00:00:00";
							}
							else if (uiM4Type == M4CL_M4_TYPE_HOUR)
							{
								sTimeStamp = "1970/09/25 " + sItemValue;
							}
							
							iRet = oTimeStamp.set(sTimeStamp.c_str());
							oTimeStamp.get(vItemValue.Data.DoubleData);	 
						}
						
						if (poNode->RecordSet.Register.Item[sItemName.c_str()].Value.Set(vItemValue) != M4_SUCCESS)
						{
							oCSVFile.close();
							delete [] pcBuffer;
							SETCODEF(M4DOCERROR_NO_SET_VALUE, ERRORLOG, "#*s1*#%s#%s#", sItemName.c_str(), "LoadCSVFile");
							poAccess->Destroy();
							return M4_ERROR;
						}
					}
				}
				iLineNum++;
			}
			vLines.clear();
		}
		oCSVFile.close();
	}
	else
	{
		iRet = M4_ERROR;
	}
	
	delete [] pcBuffer;

	vLines.clear();
	vColumnNames.clear();
	vColumnValues.clear();

	// bg 256884
	poAccess->Destroy();

	ai_pvReturn.Data.DoubleData = iRet;
	
	return M4_SUCCESS;
}
