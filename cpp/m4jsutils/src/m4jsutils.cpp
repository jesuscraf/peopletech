//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//	=========================================================
//	====================

#include <m4trace.hpp>
#include <access.hpp>
#include <dm.hpp>
#include <channel.hpp>
#include <execontx.hpp>
#include <m4jsres.hpp>
#include <m4srvres.hpp>
#include <m4log.hpp>
#include <executor.hpp>
#include <file_adv.hpp>
#include <m4date.hpp>
#include <m4logsys.hpp>
#include <regedit.hpp>
#include <file_manager.hpp>
#include <m4compress.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <m4jsutils.hpp>
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "m4regdef.hpp"

#define CODE_ERROR -1
#define CODE_SUCCESS 0
#define M4_MINOR_DATE  657061.00
#define M4_MAX_VALUE_RAND 32767
#define MAX_LEN           10000




#define SIZE_LOG_ERROR			 1024
// 4.0 
// #define CANAL_EJECUCIONES		 "CH_JS_LIGHT_DEMO"
// #define CANAL_RECURSOS			 "CH_JS_DOCS"
// #define NODO_JOB_GROUP_EXECS	 "N_JOB_GROUP_EXECS"
// #define NODO_JOB_EXECS			 "N_JOB_EXECS"
//#define NODO_DOCS_EXECS			 "N_DOCS_EXECS"
#define CANAL_EJECUCIONES		 "C4_JS_TASK_EXE"
#define CANAL_RECURSOS			 "C4_JS_DOCS"
#define NODO_JOB_GROUP_EXECS	 "N4_JS_TASK_EXE"
#define NODO_JOB_EXECS			 "N4_JS_SUBTASK_EXE"
#define NODO_RES_EXECS			 "N4_JS_RESOURCE_EXE"
#define NODO_DOCS_EXECS			 "N4_JS_DOCS"
#define METODO_FILENAME_BLOB	 "GET_BLOB_FILENAME"
#define METHOD_LOAD_CHANNEL      "READ_EXEC_WITH_DATE"
#define CAMPO_LOG                "LOG"
#define NODO_INFO_TASK           "N_JS_LCD_TASK_INFO"
#define LOG_TRANSLATE            "LOG_TRANSLATE"


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

/*Esta función permite ejecutar cualquier comando que se le pase 
por parámetro . Esta función será invocada desde un método LN4 que 
lo cual permitirá ejecutar cualquier comando desde un tarea del Job Scheduler
*/

m4return_t ExecuteProgram (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	//Si no nos pasan el comando devolvemos un error y no continuamos
	if (ai_iLongArg != 1) {

		SETCODEF(M4_POP_STACK_ERROR,ERRORLOG,"Error while getting information about the path to externa program");
		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=CODE_ERROR;
		return M4_ERROR;
	}

	//Obtenemos el comando que vamos a ejecutar
	m4VariantType program = ai_pvArg[0];

	if (program.Type='\0')
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"The path to external program is not correct");

		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=CODE_ERROR;

		return M4_ERROR;
	}

	//Ejecutamos el comando
	m4return_t ret=system(program.Data.PointerChar);

	if (CODE_ERROR  == ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"There is an error launching an external program");

		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=CODE_ERROR;

		return M4_ERROR;
	}


	//Devolvemos un código informando de que la ejecución ha sido correcta
	ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData=CODE_SUCCESS;

	return M4_SUCCESS;
}

//**************************************************************************


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

m4return_t UpdateLog (m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) 
{
	
	// Función que obtiene el canal de ejecuciones de la máquina virtual que se le pasa
	// como parámetro, obtiene la log asociada al thread que lo llama y guarda la log  
	// en el item correspondiente del canal de ejecuciones.

	SETCODEF(M4_JSUTILS_DEBUG_INFO, DEBUGINFOLOG, "Call UpdateLog");

	// Obtengo el access al canal que se ejecuta ahora.
	ClAccess * poAccess;
	poAccess = ((ClVMRunContext *)ai_pvContext)->m_pAccess;
	if (!poAccess)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG, "Error while getting information to update the job's log.");
		return M4_ERROR;
	}

	// Obtengo el canal asociado al acceso.
	ClChannel * pChannel = poAccess -> GetpChannel();
	if (!pChannel)
	{
		SETCODEF(M4_POINTER_NULL, ERRORLOG, "Error while getting information to update the job's log.");
		return M4_ERROR;
	}

	//Obtengo el channel-manager a partir del canal que obtengo por el access
	ClChannelManager * pChannelManager =  pChannel -> GetpChannelManager();
	if (!pChannelManager)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG, "Error while getting information to update the job's log.");
		return M4_ERROR;
	}

	// Me recorro la lista de canales hasta que encuentro el canal que deseo
	// que es el C4_JS_TASK_EXE.
	ClChannelDirectory *	pChanDir;
	ClChannel *				pChanAux;
	m4pcchar_t				pcAuxChanId = NULL;
	m4uint32_t				iNumChan, i;

	pChanDir = &(pChannelManager->Channel);
	iNumChan = pChanDir->Count();

	ClChannel * pChannelExecution = NULL;

	for (i = 0; (i < iNumChan) && (!pChannelExecution); i++)
	{
		pChanAux = (*pChanDir)[i];

		if (pChanAux->IsBuilt())
			pcAuxChanId = pChanAux->GetpChannelDef()->Id();
		else
			pcAuxChanId = NULL;

		if ((pcAuxChanId) && (stricmp(pcAuxChanId, CANAL_EJECUCIONES) == 0))
		{
			// Encontrado
			pChannelExecution = pChanAux;
		}
	}

	if (!pChannelExecution)
	{
		// No esta el canal, luego metemos en la pila que ha habido error, pero
		// retornamos SUCCESS para que siga la ejecución de MV.
		SETCODEF(M4_JSUTILS_ERROR_INFO, WARNINGLOG, "Meta4Object %s not found", CANAL_EJECUCIONES);

		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		return M4_SUCCESS;
	}

	////////////////////////////////////////////
	// Hemos encontrado el canal de ejecuciones.
	////////////////////////////////////////////

	// Creo un acceso a ese canal.
	ClAccess * pAccessExecution = pChannelExecution->CreateAccess();
	if (!pAccessExecution)
	{

		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Can't create access to Meta4Object %s", CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Retornamos SUCCESS para que siga la ejecución de MV.
		return M4_SUCCESS;
	}

	// Me posicion sobre el nodo que deseo.
	ClNode * poNode = &(pAccessExecution->Node[NODO_JOB_GROUP_EXECS]); //nodo raiz
	if (!poNode)
	{
		// No esta el canal, luego metemos en la pila que ha habido error, pero
		// retornamos SUCCESS para que siga la ejecución de MV.
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Node %s of Meta4Object %s not found", NODO_JOB_GROUP_EXECS, CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_SUCCESS;
	}

	if (poNode->RecordSet.Current.Begin() != M4_TRUE)
	{
		// No hay registros, luego metemos en la pila que ha habido error, pero
		// retornamos SUCCESS para que siga la ejecución de MV.

		// No tenemos registros.
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There aren't registers in node %s of Meta4Object %s.", NODO_JOB_GROUP_EXECS, CANAL_EJECUCIONES);

		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_SUCCESS;
	}

	ClNode * poNodeChild = &(pAccessExecution->Node[NODO_JOB_EXECS]); //nodo hijo
	if (!poNodeChild)
	{
		// No esta el canal, luego metemos en la pila que ha habido error, pero
		// retornamos SUCCESS para que siga la ejecución de MV.
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Node %s of Meta4Object %s not found", NODO_JOB_EXECS, CANAL_EJECUCIONES);

		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_SUCCESS;
	}

	if (poNodeChild->RecordSet.Current.Begin() != M4_TRUE)
	{
		// No hay registros, luego metemos en la pila que ha habido error, pero
		//retornamos SUCCESS para que siga la ejecución de MV.

		// No tenemos registros.
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There aren't registers in node %s of Meta4Object %s.", NODO_JOB_EXECS, CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_SUCCESS;
	}

	if (poNode->RecordSet.Current.Count() != 1)
	{
		// Hay mas de un registro, luego metemos en la pila que ha habido error, pero
		// retornamos SUCCESS para que siga la ejecución de MV.

		// Hay mas de un registro
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There are serveral registers in node %s of Meta4Object %s.", NODO_JOB_GROUP_EXECS, CANAL_EJECUCIONES);

		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_SUCCESS;
	}
	
	// Obtengo el valor del item que contiene la log actual.
	m4VariantType LogItemValue;
	poNodeChild->RecordSet.Current.Item["LOG_MESSAGES"].Value.Get(LogItemValue);
	string szBufferTotal = "";
	if (LogItemValue.Type != M4CL_M4_TYPE_NULL)
		szBufferTotal = (m4pchar_t)LogItemValue.Data.PointerChar;
	
	// Variables para ir obteniendo los datos de la log.
	m4int32_t iSizeBuffer = SIZE_LOG_ERROR;
	m4int32_t iSizeRead;
	m4pchar_t szBuffer = new char[iSizeBuffer];
	if (!szBuffer)
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There isn't enought memory.");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();

		return M4_ERROR;
	}
	
	m4return_t ret_Mark = ISVALIDMARK();
	while (ret_Mark == M4_SUCCESS)
	{
		iSizeRead = 0;
		do
		{
			if (iSizeRead == iSizeBuffer)
			{
				// No han entrado todo los datos, luego aumentamos.
				iSizeBuffer *= 2;
				delete[] szBuffer;
				szBuffer = new char[iSizeBuffer];
				if (!szBuffer)
				{
					SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There isn't enought memory.");
					ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
					ai_pvReturn.Data.DoubleData = CODE_ERROR;

					// Limpiamos la memoria.
					pAccessExecution->Destroy();

					return M4_ERROR;
				}
			}
		}
		while(GETH_MARKEDERRORTYPE(szBuffer, iSizeBuffer, iSizeRead) == M4_ERROR);
		szBufferTotal = szBufferTotal + szBuffer;

		iSizeRead = 0;
		do
		{
			if (iSizeRead == iSizeBuffer)
			{
				// No han entrado todo los datos, luego aumentamos.
				iSizeBuffer *= 2;
				delete[] szBuffer;
				szBuffer = new char[iSizeBuffer];
				if (!szBuffer)
				{
					SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There isn't enought memory.");
					ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
					ai_pvReturn.Data.DoubleData = CODE_ERROR;

					// Limpiamos la memoria.
					pAccessExecution->Destroy();

					return M4_ERROR;
				}
			}
		}
		while(GETH_MARKEDERRORCODE(szBuffer, iSizeBuffer, iSizeRead) == M4_ERROR);
		szBufferTotal = szBufferTotal + " " + szBuffer;
		
		iSizeRead = 0;
		do
		{
			if (iSizeRead == iSizeBuffer)
			{
				// No han entrado todo los datos, luego aumentamos.
				iSizeBuffer *= 2;
				delete[] szBuffer;
				szBuffer = new char[iSizeBuffer];
				if (!szBuffer)
				{
					SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "There isn't enought memory.");
					ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
					ai_pvReturn.Data.DoubleData = CODE_ERROR;

					// Limpiamos la memoria.
					pAccessExecution->Destroy();

					return M4_ERROR;
				}
			}
		}
		while(GETH_MARKEDERRORTEXT(szBuffer, iSizeBuffer, iSizeRead) == M4_ERROR);
		szBufferTotal = szBufferTotal + " " + szBuffer + "\r\n";

		ret_Mark = MOVEMARK();
	}
	
	// Este método nos situa en la subtarea correspondiente en la que
	// se quiere grabar el log.
	if (poNode->RecordSet.Current.Item["UPDATE_LOG"].Call() == M4_ERROR )
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Can't execute LN4 method %s of Meta4Object %s.", "UPDATE_LOG", CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
	
		// Limpiamos la memoria.
		pAccessExecution->Destroy();
		delete[] szBuffer;
	
		return M4_SUCCESS;
	}

	// Sacamos el resultado de la ejecución del método.
	m4VariantType vResult;
	pAccessExecution->GetpExecutor()->Stack.Pop(vResult);

	// Verificar el resultado que devuelve el método.
	if (vResult.Data.DoubleData == M4_ERROR)
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Error executing LN4 method %s of Meta4Object %s.", "UPDATE_LOG", CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();
		delete[] szBuffer;

		return M4_SUCCESS;
	}
		
	// Guardamos la log.
	LogItemValue.Type = M4CL_CPP_TYPE_STRING_VAR;
	LogItemValue.Data.PointerChar = (m4pchar_t) szBufferTotal.c_str();
	
	if (poNodeChild->RecordSet.Current.Item["LOG_MESSAGES"].Value.Set(LogItemValue) == M4_ERROR)
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Can't set value of item LOG of node %s of Meta4Object %s.", NODO_JOB_EXECS, CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
	
		// Limpiamos la memoria.
		pAccessExecution->Destroy();
		delete[] szBuffer;
	
		return M4_SUCCESS;
	}

	if (pChannelExecution->Persist() == M4_ERROR)
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Can't persist Meta4Object %s.", CANAL_EJECUCIONES);
		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;

		// Limpiamos la memoria.
		pAccessExecution->Destroy();
		delete[] szBuffer;

		return M4_SUCCESS;
	}

	// Limpiamos la memoria.
	pAccessExecution->Destroy();
	delete[] szBuffer;

	return M4_SUCCESS;
}

#define SIN_ERROR				0
#define ERROR_PARAM				1   //Nº de parametros incorrectos
#define ERROR_TIPO				2	//Tipo del recurso incorrecto
#define ERROR_ESTRUCURA			3	//No se tiene acceso, falta nodo,.....
#define ERROR_REGEDIT			4	//Error en  alguna entrada del regedit o en su valor
#define ERROR_RECURSO			5	//El recurso no puede ser accedido o copiado
#define SIZE_DATE_ERROR			6   //No se puede obtener el tamaño o la fecha de última modificación
								//del fichero ha habido un error
#define ERROR_SOURCE_FILE		7	//El fichero fuente no existe o no puede leerse
#define ERROR_DESTINATION_FILE	8	//El fichero destino no puede crearse
#define ERROR_COMPRESS			9	//Error en la compresión
#define ERROR_CODE_UNKNOWN      10  //El código de error que buscamo es desconocido
#define ERROR_STORAGE_LOG       11  //No se puede almacena la log traducida 
								   //en el campo propiedad
#define ERROR_ID_RESOURCE		12	// RESOURCE_ID incorrecto.

#define M4_NEW_REGEDIT_KEY_FTP				"FTP"
#define M4_NEW_REGEDIT_KEY_FTP_SHARE_PATH	"SharePath"
#define M4_NEW_REGEDIT_KEY_FTP_PATH			"FTPPath"
#define M4_NEW_REGEDIT_KEY_FTP_PWD			"Pwd"
#define M4_NEW_REGEDIT_KEY_FTP_USER			"User"
#define M4_NEW_REGEDIT_KEY_FTP_SERVER		"Server"
#define M4_MAX_PATH_REGEDIT					255

//**************************************************************************


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

m4return_t CheckFTPConfiguration (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	/*************************************************************************************
	Funcionalidad:
	--------------
	Esta funcion verifica que la configuración para FTP es correcta.
	Se chequea que estén rellenas las entradas Server y User.
	El resto de las entradas del registro no se consideran obligatorias.

	Retorno:
	--------
	0 -> Configuración FTP correcta.
	<0 -> Configuración FTP incorrecta.

	**************************************************************************************/

	// Se obtienen los datos necesarios del registro para FTP para realizar el chequeo de 
	// la configuración (Server y User).
	m4char_t szPathRegedit[M4_MAX_PATH_REGEDIT + 1];
	m4uint32_t tamPathRegeditOut;
	M4ComposeRegistryPath(M4_NEW_REGEDIT_KEY_SERVER, M4_NEW_REGEDIT_KEY_FTP, szPathRegedit, M4_MAX_PATH_REGEDIT, tamPathRegeditOut);
	
	m4char_t szServerFTP[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_SERVER, szServerFTP) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
		return M4_SUCCESS;
	}

	m4char_t szUserFTP[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_USER, szUserFTP) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
		return M4_SUCCESS;
	}

	m4char_t szSharePath[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_SHARE_PATH, szSharePath) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
		return M4_SUCCESS;
	}

	// Se comprueba que tenemos un server, un usuario configurado. y el directorio que comparten
	// el servidor de aplicaciones y el servidor de FTP.
	string sServerFTP = szServerFTP;
	string sUserFTP = szUserFTP;
	string sSharePath = szSharePath;
	if (sServerFTP.empty() == M4_TRUE || sUserFTP.empty() == M4_TRUE || sSharePath.empty() == M4_TRUE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = CODE_ERROR;
		return M4_SUCCESS;
	}

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = CODE_SUCCESS;
	return M4_SUCCESS;
}


m4return_t PrepareResourceForFTP (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	/********************
	Funcionalidad:
	--------------
	Esta funcion prepara un recurso que se desea recuperar mediante ftp.
	Para ello se mira si es por referencia o copia
	Si es por referencia, se accede al fichero origen y se copia a un directorio
	donde un servidor de ftp pueda acceder.
	Si es por copia se accede al blob y se procede de la misma forma

	Parametros:
	-----------
	0(Entrada) -> Tipo del recurso: Copia (C) o Referencia(R).
	1(Entrada) -> Identificador del recurso.
	2(Entrada) -> Path al fichero.
	3(Salida) -> Identificador del servidor de FTP
	4(Salida) -> El User
	5(Salida) -> El Password
	6(Salida) -> Ruta de acceso a traves de ftp
	7(Salida) -> Nombre del fichero creado
	8(Salida) -> Tamaño del fichero creado


	Retorno:
	--------
	0 -> Sin error
	>0 -> Error

	********************/

	/////////////////////
	// Leo los argumentos.
	/////////////////////
	if (ai_iLongArg != 9)
	{  
		// 3 de entrada y 6 de salida.
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_PARAM;
		return M4_SUCCESS;
	}

	string szTypeRes;
	if ((ai_pvArg[0].Type != M4CL_M4_TYPE_NULL) && (ai_pvArg[0].Data.PointerChar != NULL))
		szTypeRes = (m4pchar_t)ai_pvArg[0].Data.PointerChar;
	else
	{
		// No sabemos el tipo del recurso.
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_TIPO;
		return M4_SUCCESS;
	}

	m4VariantType vResId;
	if ((ai_pvArg[1].Type != M4CL_M4_TYPE_NULL) && (ai_pvArg[1].Data.PointerChar != NULL))
		vResId = ai_pvArg[1];
	else
	{
		// No sabemos el identificador del recurso.
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_ID_RESOURCE;
		return M4_SUCCESS;
	}

	////////////////////////////////////////////
	// Obtengo los datos necesarios del registro.
	////////////////////////////////////////////
	m4char_t szPathRegedit[M4_MAX_PATH_REGEDIT + 1];
	m4uint32_t tamPathRegeditOut;

	M4ComposeRegistryPath(M4_NEW_REGEDIT_KEY_SERVER, M4_NEW_REGEDIT_KEY_FTP, szPathRegedit, M4_MAX_PATH_REGEDIT, tamPathRegeditOut);
	
	m4char_t DestinationPath[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_SHARE_PATH, DestinationPath) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_REGEDIT;
		return M4_SUCCESS;
	}

	m4char_t FTPPath[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_PATH, FTPPath) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_REGEDIT;
		return M4_SUCCESS;
	}

	m4char_t PwdFTP[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_PWD, PwdFTP) == FALSE)
	{
		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=ERROR_REGEDIT;
		return M4_SUCCESS;
	}

	m4char_t UserFTP[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_USER, UserFTP) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_REGEDIT;
		return M4_SUCCESS;
	}

	m4char_t ServerFTP[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_SERVER, ServerFTP) == FALSE)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_REGEDIT;
		return M4_SUCCESS;
	}

	// Consigo el ChannelMannager para poder crear el canal que necesito
	// y asi acceder a la información del recurso.
	ClAccess * poAccessExe;

	// Obtengo el access al canal que se ejecuta ahora: C4_JS_TASK_EXE.
	poAccessExe = ((ClVMRunContext *)ai_pvContext)->m_pAccess;
	if (!poAccessExe)
	{
		SETCODEF(M4_POINTER_NULL, ERRORLOG, "Error while getting information to prepare FTP");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
		return M4_ERROR;
	}

	// Obtengo el nodo de recursos, y ya estoy posicionado sobre el recurso que se desea preparar
	ClChannel * pChannelExe = NULL;
	ClChannelManager * pChannelManager = NULL;
	ClAccess * pClAccessDocs = NULL;
	ClChannel * pChannelDocs = NULL;
	
	// Actuo en funcion del tipo del recurso.
	string szOriginalPath = "";
	if (szTypeRes == "R")
	{
		// Recurso por Referencia obtengo el path del fichero.
		if ((ai_pvArg[2].Type != M4CL_M4_TYPE_NULL) && (ai_pvArg[2].Data.PointerChar != NULL))
			szOriginalPath = (m4pchar_t)ai_pvArg[2].Data.PointerChar;
	} 
	else
	{
		if (szTypeRes == "C")
		{
			// Recurso por copia, accedo al canal C4_JS_DOCS 
			// para conseguir el blob.

			// Obtengo el canal asociado al acceso de ejecuciones.
			pChannelExe = poAccessExe->GetpChannel();
			if (!pChannelExe)
			{
				SETCODEF(M4_POINTER_NULL,ERRORLOG, "Error while getting information to prepare FTP.");
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				return M4_ERROR;
			}

			// Obtengo el channel-manager a partir del canal que 
			// obtengo por el access ejecuciones.
			pChannelManager =  pChannelExe->GetpChannelManager();
			if (!pChannelManager)
			{
				SETCODEF(M4_POINTER_NULL, ERRORLOG, "Error while getting information to prepare FTP.");
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				return M4_ERROR;
			}

			pChannelManager->CreateChannel(pChannelDocs);
			if (pChannelDocs == 0)
			{
				SETCODE (M4_EXECUTE_METHOD_ERROR, ERRORLOG, "Error in PrepareResourceForFTP in CreateChannel.");
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				return M4_ERROR;
			}

			if (pChannelDocs->BuildFromId(CANAL_RECURSOS) == M4_ERROR)
			{
				SETCODE (M4_EXECUTE_METHOD_ERROR, ERRORLOG, "Error in PrepareResourceForFTP in BuildFromId.");
				pChannelDocs->Destroy();
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				return M4_ERROR;
			}

			pClAccessDocs = pChannelDocs->CreateAccess();
			if (pClAccessDocs == 0)
			{
				SETCODE(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "Error in PrepareResourceForFTP in CreateAccess.");
				pChannelDocs->Destroy();
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				return M4_ERROR;
			}

			// Obtengo el nodo de recursos del canal de CANAL_RECURSOS.
			ClNode * pNodeDocs = &(pClAccessDocs->Node[NODO_DOCS_EXECS]);
			if (!pNodeDocs)
			{
				// No esta el canal, luego metemos en la pila que ha habido error, pero
				// retornamos SUCCESS para que siga la ejecución de MV.
				SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Node %s of Meta4Object %s not found", NODO_DOCS_EXECS, CANAL_RECURSOS);
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				pChannelDocs->Destroy();
				pClAccessDocs->Destroy();
				return M4_ERROR;
			}
			
			// Ejecutamos el método para obtener el nombre 
			// del fichero temporal que contiene los datos del blob.
			// Parametros de entrada.
			m4VariantType ItemValue;

			// Identificador del recurso.
			pClAccessDocs -> GetpExecutor () -> Stack.Push (vResId);

			// Parámetros de salida del metodo (path del blob).
			pClAccessDocs -> GetpExecutor () -> Stack.Push (ItemValue);

			if (pNodeDocs -> RecordSet.Current.Item[METODO_FILENAME_BLOB].Call() == M4_ERROR )
			{
				SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Node %s of Meta4Object %s can't execute %s", NODO_DOCS_EXECS, CANAL_RECURSOS, METODO_FILENAME_BLOB);
				ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData = ERROR_ESTRUCURA;
				pChannelDocs->Destroy();
				pClAccessDocs->Destroy();
				return M4_ERROR;
			}

			// Sacamos el parametro de salida, que contiene el nombre del fichero 
			// temporal, donde esta el blob.
			pClAccessDocs->GetpExecutor()->Stack.Pop(ItemValue);
			if ((ItemValue.Type != M4CL_M4_TYPE_NULL) && ItemValue.Data.PointerChar)
				szOriginalPath = (m4pchar_t)ItemValue.Data.PointerChar;

			// Sacamos el resultado.
			pClAccessDocs->GetpExecutor()->Stack.Pop(ItemValue);

		} //fin del if de recurso por copia
		else
		{
			// No sabemos el tipo del recurso.
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = ERROR_TIPO;
			return M4_SUCCESS;
		}
	} //fin del else del if del recurso por referencia

	// Comprobamos que tenemos un path.
	if (szOriginalPath == "")
	{
		m4Trace(printf("El recurso no puede ser accedido\n"));
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_RECURSO;
		if (pClAccessDocs)
			pClAccessDocs->Destroy();
		if (pChannelDocs)
			pChannelDocs->Destroy();
		return M4_SUCCESS;
	}

	// Copio el recurso al destino desde szOriginalPath (incluye 
	// el nombre del fichero) hasta DestinationPath (no incluye el 
	// nombre del fichero).
 	m4char_t DestinationFileName[M4_MAX_PATH] = "";
	if (ClTemporalFileCreator::CreateTempFile(DestinationFileName, "js", "tmp", DestinationPath) == M4_ERROR)
	{
		m4Trace(printf("No se puede crear el fichero temporal\n"));
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_DESTINATION_FILE;
		if (pClAccessDocs)
			pClAccessDocs->Destroy();
		if (pChannelDocs)
			pChannelDocs->Destroy();
		return M4_SUCCESS;
	}
	
	if (M4CompressFile((char*)szOriginalPath.c_str(), DestinationFileName) != M4_SUCCESS)
	{
		m4Trace(printf("Error al comprimir de %s --> %s\n", szOriginalPath.c_str(), DestinationFileName));
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_COMPRESS;
		if (pClAccessDocs)
			pClAccessDocs->Destroy();
		if (pChannelDocs)
			pChannelDocs->Destroy();
		return M4_SUCCESS;
	}

	// Borrado despues de la copia, para que en caso 
	// de recurso por copia al descargar el canal, perdemos el fichero temporal.
	if (pClAccessDocs)
		pClAccessDocs->Destroy();
	if (pChannelDocs)
		pChannelDocs->Destroy();

	// Rellenamos los parametros de salida.
	ai_pvArg[3].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[3].Data.PointerChar = new m4char_t[strlen(ServerFTP) + 1];
	strcpy(ai_pvArg[3].Data.PointerChar, ServerFTP);

	ai_pvArg[4].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[4].Data.PointerChar = new m4char_t[strlen(UserFTP) + 1];
	strcpy(ai_pvArg[4].Data.PointerChar, UserFTP);

	ai_pvArg[5].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[5].Data.PointerChar = new m4char_t[strlen(PwdFTP) + 1];
	strcpy(ai_pvArg[5].Data.PointerChar, PwdFTP);

	ai_pvArg[6].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[6].Data.PointerChar = new m4char_t[strlen(FTPPath) + 1];
	strcpy(ai_pvArg[6].Data.PointerChar, FTPPath);

	// Obtenemos el tamaño del fichero creado
	ClFile DestFile(DestinationFileName);
	m4uint32_t SizeFile = DestFile.GetSize();

	// DestinationFileName tiene la ruta completa, y solo deseamos el nombre
	string sDestinationFileName;
	if (DestFile.GetShortName(sDestinationFileName) != M4_SUCCESS)
	{
		sDestinationFileName = DestinationFileName;
	}

	ai_pvArg[7].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[7].Data.PointerChar = new m4char_t[strlen(sDestinationFileName.c_str()) + 1];
	strcpy(ai_pvArg[7].Data.PointerChar, sDestinationFileName.c_str());

	ai_pvArg[8].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[8].Data.DoubleData = SizeFile;

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = SIN_ERROR;

	return M4_SUCCESS;
}

m4return_t DeleteFTPFile (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	/*************************************************************************************
	Funcionalidad:
	--------------
	Esta funcion borra el fichero temporal que se copia en el directorio FTP.
	
	Parametros:
	-----------
	0 (Entrada) -> Nombre del fichero.
	
	Retorno:
	--------
	0 -> Se ha borrado el fichero.
	<> 0 -> No se ha podido borrar el fichero.
	**************************************************************************************/

	// Recuperación de argumentos.
	if (ai_iLongArg != 1)
	{  
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG,"Error while getting input parameters to delete FTP file.");		
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_PARAM;
		return M4_SUCCESS;
	}

	string sFileName;
	if ((ai_pvArg[0].Type != M4CL_M4_TYPE_NULL) && (ai_pvArg[0].Data.PointerChar != NULL))
	{
		sFileName = (m4pchar_t) ai_pvArg[0].Data.PointerChar;
	}
	else
	{
		// No sabemos el tipo del recurso.
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Input argument type is not correct.");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_TIPO;
		return M4_SUCCESS;
	}

	// Se obtiene el directorio en el que se ha copiado el fichero para el FTP.
	// Este directorio debe ser un directorio compartido por el servidor de FTP
	// y por el servidor de aplicaciones.
	m4char_t szPathRegedit[M4_MAX_PATH_REGEDIT + 1];
	m4uint32_t tamPathRegeditOut;
	M4ComposeRegistryPath(M4_NEW_REGEDIT_KEY_SERVER, M4_NEW_REGEDIT_KEY_FTP, szPathRegedit, M4_MAX_PATH_REGEDIT, tamPathRegeditOut);
	
	m4char_t szFTPPath[M4_MAX_PATH_REGEDIT + 1];
	if (GetRegKeyData(szPathRegedit, M4_NEW_REGEDIT_KEY_FTP_SHARE_PATH, szFTPPath) == FALSE)
	{
		SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Error when trying to access %s registry entry.", M4_NEW_REGEDIT_KEY_FTP_SHARE_PATH);
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = ERROR_REGEDIT;
		return M4_SUCCESS;
	}
	
	// Construir path al fichero.
	string sFilePath;
	sFilePath.append(szFTPPath);

	// Verificar si se ha añadido la barra al final del path.
	m4uint32_t iPos = sFilePath.find_last_of(M4_BACK_SLASH);
	if (iPos != sFilePath.size() - 1)
	{
		sFilePath.append(M4_BACK_SLASH);
	}

	sFilePath.append(sFileName);

	int iRet = M4RemoveFile(sFilePath.c_str());

	switch (iRet)
	{
		// Si el fichero no existe o si todo se ejecuta correctamente.
		case 1:
		case 0:
		{
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = SIN_ERROR;
		}
		break;
		// Si no se puede borrar el fichero
		case -1:
		{
			// Si no podemos borrar el fichero devolvemos el código -1.
			SETCODEF(M4_JSUTILS_ERROR_INFO, ERRORLOG, "Error when trying to delete file %s", sFilePath.c_str());
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = CODE_ERROR;
		}
		break;
	}
	
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = SIN_ERROR;
	return M4_SUCCESS;
}

//*********************************************************************************

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
m4return_t ExistFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	/*Esta función permite averiguar si el recurso sobre cuyo registro estmos
	situado existe aún o fue borrado desde que almacenamos su referencia
	*/

		  if (2 !=ai_iLongArg)//Comprobamos que nos pasan el número de parámetros esperados
		  {
			ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData=ERROR_PARAM;

			return M4_SUCCESS;
		  }

			m4VariantType PathFile;

		//Obtenemos el parámetro en el que está almacenado en la ruta al fichero

		  PathFile=ai_pvArg[0];

		  if ((PathFile.Type==M4CL_CPP_TYPE_NULL)||(PathFile.Data.PointerChar==NULL))
		  {
			  ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			  ai_pvReturn.Data.DoubleData=ERROR_TIPO;

			  return M4_SUCCESS;
		  }

		  if (ClFileState(PathFile.Data.PointerChar).IsFile() == M4_TRUE)
		  {
				ai_pvArg[1].Type=M4CL_CPP_TYPE_NUMBER;
				ai_pvArg[1].Data.DoubleData=1;//almacenamos en el parámetro
						//de salida que indica si el fichero existe o no  
						// 1 que es el código que indica que existe
		  }
		  else
		  {
			   //Si no existe el fichero en disco almacenamos en el 
			 //parámetros de salida un 0 que es el código correspondiente a no existencia

			    ai_pvArg[1].Type=M4CL_CPP_TYPE_NUMBER;
				ai_pvArg[1].Data.DoubleData=0;
		  }

		  ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		  ai_pvReturn.Data.DoubleData=CODE_SUCCESS;

		  return M4_SUCCESS;
}


//*********************************************************************************

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

m4return_t PropertiesFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	/*Esta función averigua si un recurso dado por referencia aún existe en disco
	o ya ha sido borrado.Si existe aún el recurso determina su tamaño actual y la 
	última fecha en la que se modificó y almacena los resultados obtenidos en campos 
	de tipo propiedad. El path al recurso se obtiene del registro sobre el que estamos 
	situados*/

		if (4 !=ai_iLongArg)//Comprobamos que nos pasan el número de parámetros esperados
		{
			ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData=ERROR_PARAM;

			return M4_SUCCESS;
		}

			m4VariantType PathFile;

		//Obtenemos el parámetro en el que está almacenado en la ruta al fichero

		  PathFile=ai_pvArg[0];

		  if ((PathFile.Type==M4CL_CPP_TYPE_NULL)||(PathFile.Data.PointerChar==NULL))
		  {
			  ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			  ai_pvReturn.Data.DoubleData=ERROR_TIPO;

			  return M4_SUCCESS;
		  }


		  //Creamos un objeto de tipo ClFileState que nos proporciona toda la 
		  //información que necesitamos respecto al fichero: si existe,su
		  //tamaño y su fecha de última modificacióN

		  ClFileState  oFileState(PathFile.Data.PointerChar);

		  if (oFileState.IsFile() == M4_TRUE)
		  {
				//Como el fichero existe determinamos el tamaño
				m4uint32_t uiSizeFile = oFileState.GetSize();

				//Por último determinamos la fecha de última modificación del fichero
				m4double_t dDateLastModified = oFileState.GetLastGmtWrite();

				//Almacenamos en el segundo parámetro el código 1 que indica 
				//que el fichero existe y se han podido obtener sus propiedades 
				//con exito
				ai_pvArg[1].Type=M4CL_CPP_TYPE_NUMBER;
				ai_pvArg[1].Data.DoubleData=1;

				
				//Almacenamos en el tercer parámetro el tamaño del fichero
				ai_pvArg[2].Type=M4CL_CPP_TYPE_NUMBER;
				ai_pvArg[2].Data.DoubleData=uiSizeFile;
		

				//Almacenamos en el cuarto parámetro la fecha de última modificación
				ai_pvArg[3].Type=M4CL_CPP_TYPE_DATE;
				ai_pvArg[3].Data.DoubleData=dDateLastModified;

		  }
		  else
		  {
			  //Si no existe el fichero almacenamos en el segundo parámetro
			  //que indica si el fichero existe o no almacenamos un 0
			  ai_pvArg[1].Type=M4CL_CPP_TYPE_NUMBER;
			  ai_pvArg[1].Data.DoubleData=0;

			  //Almacemamos como tamaño del fichero un valor negativo
			  ai_pvArg[2].Type=M4CL_CPP_TYPE_NUMBER;
			  ai_pvArg[2].Data.DoubleData=-1;

			  //Almacenamos como fecha de última modificación del fichero 
			  //la fecha inicial
			  ai_pvArg[3].Type=M4CL_CPP_TYPE_DATE;
			  ai_pvArg[3].Data.DoubleData=M4_MINOR_DATE;
		  }


		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=CODE_SUCCESS;

		return M4_SUCCESS;
}

//**********************************************************************************

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

m4return_t EraseFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
		//Esta función borra el fichero que se encuentra en el path
	    //que se proporciona por un parámetro de entrada
	    //Los valores que devuelve la función son:
	    //-1-> Cuando no podemos borrar el fichero , pero existe
	    //0->  Cuando podemos borrar el fichero
	    //1->  Cuando el fichero no existe
		
		if (1 !=ai_iLongArg)//Comprobamos que nos pasan el número de parámetros esperados
		{
			ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData=ERROR_PARAM;

			return M4_SUCCESS;
		}

			m4VariantType PathFile;

		//Obtenemos el parámetro en el que está almacenado en la ruta al fichero

		  PathFile=ai_pvArg[0];

		  if ((PathFile.Type==M4CL_CPP_TYPE_NULL)||(PathFile.Data.PointerChar==NULL))
		  {
			  ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			  ai_pvReturn.Data.DoubleData=ERROR_TIPO;

			  return M4_SUCCESS;
		  }


			int ret = M4RemoveFile(PathFile.Data.PointerChar);

			switch (ret)
			{
               //Si el fichero no existe 
			   case 1:
				   {
					   //Si el fichero no existe devolvemos 
					   //un código 1
					   ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
					   ai_pvReturn.Data.DoubleData=1;
				   }
				   break;
				//Si no se puede borrar el fichero
			   case -1:
				   {
						//Si no podemos borrar el fichero 
					   //devolvemos el código -1
					   ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
					   ai_pvReturn.Data.DoubleData=-1;
				   }
				   break;
			 //Si todo se ejecuta correctamente
			   case 0:
				   {
					   //Si podemos borrar el fichero 
					   //devolvemos el códgo 0
					   ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
					   ai_pvReturn.Data.DoubleData=0;
				   }
				   break;
			}


			return M4_SUCCESS;

}


m4return_t GenerateNumber (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{
	//Función que genera un número aleatorio a partir de una semilla

		m4VariantType   MaxLimit; // En este valor almacenamos el limite superior
			         //que puede tomar el valor que generamos

		//Obtenemos del parámetro de entrada este valo máximo
		 MaxLimit=ai_pvArg[0];

		 //Generamos un número aleatorio

		 //Establecemos la semilla
		 srand((unsigned)time( NULL )  );


		 //Generamos el número a partir de la semilla
		m4double_t uinumber=rand();

		//Convertimos el número a un decimal 
		m4double_t uiDecNumber =uinumber/M4_MAX_VALUE_RAND;

		//Lo multiplicamos por el valor máximo permitido
		uinumber=(m4uint32_t)(uiDecNumber * MaxLimit);

		//Almacenamos el valor en el parámetro de salida
		ai_pvArg[1].Type=M4CL_CPP_TYPE_NUMBER;
	    ai_pvArg[1].Data.DoubleData=uinumber;

		//Devolvemos el valor de retorno de la pila
		ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData=0;

		return M4_SUCCESS;
}


m4return_t TranslateLogMessage (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext)
{

	//Esta función será la encargada de traducir los mensajes de traza 
	//que se vuelca en base de datos asociados a cada tarea en mensajes
	//comprensibles para el usuario cuando se muestren a este a través
	//del cliente ligero

	//El método no necesita  parámetros 

	//Lo primero que tenemos que hacer es recuperar los mensajes que
	//se hayan almacenado hasta ese momento en la base de datos en el 
	//campo LOG del nodo N_JS_LCD_TASK_INFO del canal CH_JS_LIGHT_DEMO
	//Obtengo el access al canal que se ejecuta ahora.

	//Tenemos que recuperar el puntero al channel manager para poder
	//realizar operaciones de canales 

	//Obtenemos el acceso al canal 
	ClAccess *poAccess=((ClVMRunContext *)ai_pvContext)->m_pAccess;

	if (poAccess == NULL) 
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	//Comprobamos que el canal es correcto
	//es decir es el canal CH_JS_LIGHT_DEMO

	//Para ello primero obtenemos el canal
	ClChannel *poChannel = poAccess -> GetpChannel();

	if (poChannel== NULL) 
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	//Obtengo el nombre del canal
	m4pcchar_t pcNameChannel =  poChannel->GetpChannelDef()->Id();

	if (pcNameChannel == NULL)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	if (strcmp(CANAL_EJECUCIONES,pcNameChannel )!=0)
	{
		ai_pvReturn.Type=M4CL_CPP_TYPE_STRING_VAR;
		ai_pvReturn.Data.PointerChar="El contexto de obtención de los errores no es correcto";

		return M4_SUCCESS;
	}

	//A continuación accedemos al nodo donde se encuentra el campo
	ClNode *poNode=&(poAccess->Node[NODO_INFO_TASK]);

	if (NULL == poNode)
	{
		SETCODEF(M4_ERROR_CREATE_NODE,ERRORLOG,"#*S1*#%s#%s#",NODO_INFO_TASK,CANAL_EJECUCIONES);
		return M4_ERROR;
	}


	
			
	//Y al campo
	 m4VariantType arg;
	 m4VariantType arglog;

	m4return_t ret=poNode->RecordSet.Current.Item[CAMPO_LOG].Value.Get(arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#%s#",CAMPO_LOG,NODO_INFO_TASK,CANAL_EJECUCIONES);

		return M4_ERROR;
	}

	//Comprobamos si hay trazas
	if (arg.Type != M4CL_CPP_TYPE_NULL)
	{
		if (strcmp(arg.Data.PointerChar,"No log trace")==0)
		{
			arglog=arg;
		}
		else
		{

			//Ahora tenemos que ir parseando la cadena para poder obtener
			//los mensajes que componen las trazas y poderlas traducir

			m4pchar_t pcTrazas=new m4char_t[strlen(arg.Data.PointerChar)+2];

			strcpy(pcTrazas,arg.Data.PointerChar);

			m4pchar_t *pcArray=NULL;
			m4uint32_t uiLongArray=0;

			
			//Lo inicializamos a 2 porque no nos interesa 
			//analizar los dos primero carácteres de la linea
			//que ya sabemos que son \r\n 
			if (strlen(pcTrazas)>4)
			{
				
				m4uint32_t  MaxLogSize=0;
				m4pchar_t pcTipo;
				m4pchar_t pcCodigo;
				m4pchar_t pcTexto;
				m4pchar_t pcCursor=pcTrazas+4;


				while ( pcCursor[0]!= '\0')
				{	
			

					//Obtenemos el tipo de error

					//La cadena termina cuando encotremos el último _
					//ejemplo _warning_

					m4uint32_t uiContTipo=strcspn(pcCursor,"_");

					if (pcCursor[uiContTipo] !='\0')
					{
						pcTipo=pcCursor;

						pcCursor[uiContTipo]='\0';

						pcCursor=pcCursor+uiContTipo+2;
					}

					

					//Obtenemos el código de error
					//que viene delimitado por espacios en blanco
					m4uint32_t uiContCodigo=strcspn(pcCursor," ");

					if (pcCursor[uiContCodigo] !='\0')
					{
						pcCodigo=pcCursor;

						pcCursor[uiContCodigo]='\0';

						pcCursor=pcCursor+uiContCodigo+1;
					}
			
			
					//Obtenemos el texto del mensaje de trazas
			
					m4bool_t bFinalTexto=M4_TRUE;
		

					while(bFinalTexto == M4_TRUE)
					{
						//Establecemos la condición para ver si ha llegado
						//al fin del mensaje de error

						m4uint32_t uiContTexto=strcspn(pcCursor,"\r");

						if (pcCursor[uiContTexto] =='\0')
						{
							bFinalTexto=M4_FALSE;

							pcTexto=pcCursor;
	
							pcCursor=pcCursor+uiContTexto;
						}
						else
						{
							//Determinamos si efectivamente hemos llegado al final
							//de un mensaje de error o estamos aún dentro del mensaje
							//para ello vemos si los carácteres que siguen al \r\n son
							//las cadena " _warning_" , " _error_" o " _debug_" en cuyo
							//caso podemos asegurar que hemos llegado al final del mansaje 
							//de error
							m4pchar_t pcCursorAux=pcCursor+uiContTexto;

							if ((strncmp(pcCursorAux,"\r\n _warning_",10)==0)
						     || (strncmp(pcCursorAux,"\r\n _error_",8)==0) || ((strncmp(pcCursorAux,"\r\n _debug_",8)!=0)))
							{
								//Si el texto que sigue no coincide con alguna de 
								//las cadena anteriores significa que aún hemos finalizado
								//la cadena de error
								pcTexto=pcCursor;

								pcCursor[uiContTexto]='\0';

								pcCursor=pcCursorAux+4;

								bFinalTexto=M4_FALSE;
							}
							else
							{
								pcCursor=pcCursorAux+1;
							}
								
						}
					}

			
					//Convertimos la cadena que contiene el código de error
					//en un número
					m4uint32_t numCharFilled;
					m4uint32_t  numCode;
					m4pchar_t   pcFinCodigo;

					numCode=strtol(pcCodigo,&pcFinCodigo,16);

					//Llamamos a la función que traduce los mensajes
					ret=FormatErrorCode(numCode,pcTexto,1,NULL,0,&numCharFilled);

					m4pchar_t pcTraduccion=new m4char_t[numCharFilled+2];

					ret=FormatErrorCode(numCode,pcTexto,1,pcTraduccion,numCharFilled+2,&numCharFilled);

					if (M4_SUCCESS == ret)
					{
						

						//Una vez que hemos traducido la cadena vamos a formatearla
						//a html
						m4pchar_t pcSentence;
						m4pchar_t pcCursorTraducido=pcTraduccion;
						m4uint32_t uiContTraducido=0;
						m4uint32_t uiContSentence;
						m4bool_t  bContinue=M4_TRUE;
						m4uint32_t	numreturn=0;

						while (pcTraduccion[uiContTraducido]!='\0')
						{
							if (pcTraduccion[uiContTraducido]=='\n')
							{
								numreturn++;
							}

							uiContTraducido++;
						}

						m4pchar_t pcLog=new m4char_t[100+numreturn*6+strlen(pcTipo)+100+strlen(pcTraduccion)];

						pcLog[0]='\0';

						m4uint32_t MaxLogSize=0;

						uiContTraducido=0;

						do
						{
							uiContSentence=strcspn(pcCursorTraducido,"\n");

							if (pcCursorTraducido[uiContSentence]!='\0')
							{
								pcSentence=pcCursorTraducido;

								pcCursorTraducido[uiContSentence]='\0';

								
								if (uiContTraducido==0)
								{

									pcCursorTraducido=pcCursorTraducido+uiContSentence+1;

						
									sprintf(&(pcLog[MaxLogSize]),"<P> <FONT COLOR=BLUE> <B> %s ( %d ) </FONT> <BR> %s </B> <BR> ",pcTipo,numCode,pcSentence);

									uiContTraducido=1;
						

							
								}
								else
								{
									pcCursorTraducido=pcCursorTraducido+uiContSentence+1;

									sprintf(&(pcLog[MaxLogSize]),"%s <BR> ",pcSentence);
		
								}

								
							}
							else
							{
								sprintf(&(pcLog[MaxLogSize]),"%s",pcCursorTraducido);

								bContinue=M4_FALSE;
							}

							MaxLogSize=strlen(pcLog);
						}
						while(bContinue == M4_TRUE);

						pcArray=ArrayInsertarString (pcArray,uiLongArray,pcLog);

						delete [] pcTraduccion;
				}
			}

			m4pchar_t pcStringHTML=ArrayConcatenar ( pcArray, uiLongArray);

			arglog.Type=M4CL_CPP_TYPE_STRING_VAR;
		    arglog.Data.PointerChar=pcStringHTML;


			delete [] pcTrazas;
			ArrayBorrar (pcArray,uiLongArray);

			free (pcArray);

			ret=poNode->RecordSet.Current.Item[LOG_TRANSLATE].Value.Set(arglog);

			if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#%s#",LOG_TRANSLATE,NODO_INFO_TASK,CANAL_EJECUCIONES);
	
				ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
				ai_pvReturn.Data.DoubleData=ERROR_STORAGE_LOG;
	
				return M4_ERROR;
			}

			free(pcStringHTML);

			return M4_SUCCESS;

			
		}
		else
		{
			arglog=arg;
		}
	  }



		//Por último almacenamos la cadena que hemos construido en el campo propiedad
		//LOG_TRANSLATE

	
	
		ret=poNode->RecordSet.Current.Item[LOG_TRANSLATE].Value.Set(arglog);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#%s#",LOG_TRANSLATE,NODO_INFO_TASK,CANAL_EJECUCIONES);

			ai_pvReturn.Type=M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData=ERROR_STORAGE_LOG;
	
			return M4_ERROR;
		}

		
	
	}


	return M4_SUCCESS;
}

/*****************************************************************************/
/* FUNCION: ArrayBorrar                                                      */
/* OBJETIVO: Libera la memoria las cadenas que componen el array             */
/*****************************************************************************/
/* P. ENTRADA: m4charp_t ppszArray - Puntero al array a liberar              */
/*             uint uiLongitud  - Número de cadenas en el array              */
/*                                                                           */
/* RESULTADO: void                                                           */
/*****************************************************************************/
void ArrayBorrar ( m4pchar_t * ppszArray, m4uint32_t uiLongitud)
{
   register unsigned int i  ;/* Contador para el bucle de borrado                     */

   if (ppszArray) 
   {
      for (i = 1 ; i < uiLongitud; i++)
      {
         free( ppszArray[i]);
      } /* endfor */
   } /* endif */

   return;
}

/*****************************************************************************/
/* FUNCION: ArrayInsertarString                                              */
/* OBJETIVO: Inserta un nuevo elemento en el array                           */
/*****************************************************************************/
/* P. ENTRADA: m4charp_t ppszAnterior - Array inicial                        */
/*             uint & shLongitudAnterior - Longitud inicial de la cadena     */
/*             m4charp_t - Cadena a añadir                                   */
/*                                                                           */
/* RESULTADO: m4pchar_t * ppszNuevo - Nuevo array de punteros                */
/*****************************************************************************/
m4pchar_t * ArrayInsertarString (m4pchar_t * ppszAnterior, m4uint32_t & shLongitudAnterior, m4pchar_t pszCadena)
{
   m4pchar_t * ppszNuevo = NULL ;/* Nuevo array de punteros                  */

   ppszNuevo = (m4pchar_t *)malloc( (shLongitudAnterior + 1) * sizeof(m4pchar_t));

   if (shLongitudAnterior > 0)
      memcpy (ppszNuevo, ppszAnterior, shLongitudAnterior * sizeof( m4pchar_t));
      
   ppszNuevo[shLongitudAnterior] = pszCadena;

   shLongitudAnterior += 1;

   return ppszNuevo;
}

/*****************************************************************************/
/* FUNCION: ArrayConcatenar                                                  */
/* OBJETIVO: Genera una cadena con los elementos del array concatenados      */
/*****************************************************************************/
/* P. ENTRADA: m4charp_t ppszArray - Array a concatenar                      */
/*             uint shLongitudArray - Longitud del array                     */
/*                                                                           */
/* RESULTADO: m4charp_t pszCadena                                            */
/*****************************************************************************/
m4pchar_t ArrayConcatenar ( m4pchar_t *ppszArray, m4uint32_t shLongitudArray)
{
   m4uint32_t ulLongitud ;/* Longitud de la cadena generada                  */
   m4uint32_t shParcial  ;/* Longitud de cada cadena                         */
   m4pchar_t pszCadena   ;/* Cadena generada                                 */
   m4pchar_t pszParcial  ;/* Puntero para ir escribiendo cadenas             */
   register unsigned short i      ;/* Contador de bucles                              */

   if (ppszArray)
   {
      ulLongitud = 0;

      /* Calculamos la longitud final de la cadena                           */
      for (i = 0; i < shLongitudArray; i++)
      {
         if (ppszArray[i] != NULL)
         ulLongitud += strlen(ppszArray[i]);
      } /* endfor */

	  /* Reservamos el espacio y apuntamos pszParcial al principio           */
      pszCadena =(m4pchar_t ) malloc( ulLongitud + 1);
      pszCadena[ulLongitud] = 0;
      pszParcial = pszCadena;

      /* Para cada cadena de la lista ...                                    */
      for (i = 0; i < shLongitudArray; i++)
      {
         /* Comprobamos que hay algo                                         */
         if ( ppszArray[i] != NULL)
         {
            /* Lo copiamos                                                   */
            shParcial = strlen ( ppszArray[i]);
            memcpy( pszParcial, ppszArray[i], shParcial);
            pszParcial += shParcial;
         }
      } /* endfor */

   } /* endif */
   else 
      pszCadena = NULL;

   return(pszCadena);
}


