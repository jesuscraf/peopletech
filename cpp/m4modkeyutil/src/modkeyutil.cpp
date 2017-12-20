//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkeyutil
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#include "globdec.hpp"
#include "arguments.hpp"

//*********** Funcion para lanzar ejemplos ****************************
m4return_t _Examples()
{
	m4return_t iReturn = M4_SUCCESS;

	//-------------------------------------
	//Ejemplo de encriptacion de objetos.
	{
		ClOptionArguments oArgs("m4modkeyutil -O -E -i modules.xml -o -w modules_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo de desencriptacion de objetos.
	{
		ClOptionArguments oArgs("m4modkeyutil -O -D -i modules_xxx.txt -o -w modules_bis.xml -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}		
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo generacion de licencia (dejandola encriptada).
	{
		ClOptionArguments oArgs("m4modkeyutil -G -E -i modules.xml -i lic_info.xml -o -w license2_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//ACCESO TOTAL CON CADUCIDAD.
	//Ejemplo generacion de licencia (dejandola encriptada).
	{
		ClOptionArguments oArgs("m4modkeyutil -G -E -i modules.xml -i lic_info_caduca.xml -o -w licensetotalcaduca_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo generacion de licencia (dejandola desencriptada).
	{
		ClOptionArguments oArgs("m4modkeyutil -G -D -i modules.xml -i lic_info.xml -o -w license1.xml -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//Ejemplo de encriptacion de licencia generada.
	//Si vemos el dump, podemos extraer un summary valido.
	{
		ClOptionArguments oArgs("m4modkeyutil -L -E -i license1.xml -o -w licchkgen1_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo de desencriptacion de licencia generada.
	{		
		ClOptionArguments oArgs("m4modkeyutil -L -D -i license2_xxx.txt -o -w licchkgen1.xml -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}		
	}
	//-------------------------------------

	//-------------------------------------	
	//OJO. El fichero de partida es una licencia (no tendría pq haber estado generada).
	//Ejemplo de encriptacion de licencia.
	{
		ClOptionArguments oArgs("m4modkeyutil -L -E -i licchkgen1.xml -o -w lic2_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//OJO. El fichero de partida es una licencia (no tendría pq haber estado generada).
	//Ejemplo de desencriptacion de licencia.
	{
		ClOptionArguments oArgs("m4modkeyutil -L -D -i lic2_xxx.txt -o -w lic21.xml -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//Ejemplo de volcado de información de licencia encriptada.
	//Si vemos el dump, podemos extraer un summary valido.
	{
		ClOptionArguments oArgs("m4modkeyutil -I -i lic2_xxx.txt -o dumpinfo.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//Ejemplo de volcado de información de licencia encriptada en pantalla.
	{
		ClOptionArguments oArgs("m4modkeyutil -I -i lic2_xxx.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//Ejemplo de volcado del resumen información de licencia encriptada.
	{
		ClOptionArguments oArgs("m4modkeyutil -S -i summary_xxx.txt -o -w summary.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------	
	//Ejemplo de volcado del resumen información de licencia encriptada. Por pantalla.
	{
		ClOptionArguments oArgs("m4modkeyutil -S -i summary_xxx.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo generacion de licencia para tecnologia (dejandola encriptada).
	{
		ClOptionArguments oArgs("m4modkeyutil -G -E -i tech_modules.xml -i tech_lic_info.xml -o -w tech_license_xxx.txt -v dump.txt");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	//-------------------------------------
	//Ejemplo de error de argumentos.
	{
		ClOptionArguments oArgs("m4modkeyutil");
		if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
			oArgs.DumpUsage();
		}
	}
	//-------------------------------------

	return iReturn;
}
//*********** FIN Funcion para lanzar ejemplos ************************


//***************************************************************
//********************* PRINCIPAL  ******************************
m4return_t main (int argc, char ** argv)
{
	m4return_t iReturn = M4_SUCCESS;

	//----------------------------------------------------------------
	//Metemos aqui las pruebas.
	//m4return_t iRetExamples = _Examples();
	//----------------------------------------------------------------

	ClOptionArguments oArgs(argc, argv);
	if ((iReturn = oArgs.StartParse()) == M4_ERROR) {
		oArgs.DumpUsage();
		return iReturn;
	}

	return iReturn;
}
//********************* FIN PRINCIPAL  ******************************
//*******************************************************************



