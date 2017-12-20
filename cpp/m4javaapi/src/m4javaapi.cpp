//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4javaapi.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             12/03/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			
//	Modifications:	
//
// Definition:
//
//    Library API for Java.
//
//
//==============================================================================

#include "m4javaapi.hpp"
#include "m4javares.hpp"
#include "chlog.hpp"
#include "m4objglb.hpp"
#include "execontx.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "m4jmanager.hpp"
#include "m4jobject.hpp"
#include "m4jconfiguration.hpp"
#include "m4thread.hpp"
#include "file_manager.hpp"
#include "m4unicode.hpp"
#include "iexecutor.hpp"
#include "m4objreg.hpp"

#include <sys/stat.h>

void	M4SetThreadMinPriority( void ) ;




//=============================================================================
//	Name:			_PrintExecutionLine
//	Description:	prints execution line information into a buffer.
//
//	Parameters:
//  Input:			ai_pcBuffer:	Record to get jar pk from and to set jar file.
//  Input:			ai_pvContext:	Execution context.
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

void	_PrintExecutionLine( m4pchar_t ai_pcBuffer, ClVMRunContext* ai_pvContext )
{

	m4return_t	iResult = M4_ERROR ;
	m4int32_t	iLine = -1 ;
	m4pcchar_t	pccItemId = NULL ;
	m4pcchar_t	pccItemName = NULL ;
	m4pcchar_t	pccNodeId = NULL ;
	m4pcchar_t	pccNodeName = NULL ;
	m4pcchar_t	pccM4ObjId = NULL ;
	m4pcchar_t	pccM4ObjName = NULL ;


	iResult = GetExecutionLine( ai_pvContext, iLine, pccItemId, pccItemName, pccNodeId, pccNodeName, pccM4ObjId, pccM4ObjName ) ;

	if( iResult == M4_SUCCESS )
	{
		sprintf( ai_pcBuffer, "%s!%s.%s(%d)", pccM4ObjId, pccNodeId, pccItemId, iLine ) ;
	}
	else
	{
		sprintf( ai_pcBuffer, "Undefined" ) ;
	}
}


//=============================================================================
//	Name:			_DownloadJar
//	Description:	Downloads jar and fills JAR_FILE with it.
//					Invokes method to download jar.
//					Updates JAR_CRC if necessary.
//
//	Parameters:
//  Input:			ai_oRegister:	Record to get jar pk from and to set jar file.
//  Input:			ai_poAccess:	Access to M4JAVA_JAR_LOADER.
//  Output:			ao_iCrc:		Jar file Crc.
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t	_DownloadJar( _ClRegister_Base& ai_oRegister, ClAccess *ai_poAccess, m4uint32_t& ao_iCrc )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	vValue ;
	IStackInterface	*poStack = NULL ;
	ClNode			*poNode = NULL ;


	ao_iCrc = 0 ;

	poNode = &( ai_poAccess->Node[ "M4JAVA_JAR_LOADER"] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	poStack = &( ai_poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = ai_oRegister.Item[ "ID_JAR" ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ai_oRegister.Item[ "ID_JAR_VER_AUX" ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetNull() ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.Item[ "LOAD_JAR" ].Call( NULL, 3 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData > 0 )
	{
		ao_iCrc = m4uint32_t( vValue.Data.DoubleData ) ;
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ai_oRegister.Item[ "JAR_FILE" ].Value.Set( vValue ) ;
	ai_oRegister.CheckPoint() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


//=============================================================================
//	Name:			_DownloadJar
//	Description:	Downloads jar and fills JAR_FILE with it.
//					Updates JAR_CRC if necessary.
//
//	Parameters:
//  Input:			ai_oRegister:			Record to get jar pk from and to set jar file.
//  Input:			ai_poChannelManager:	ChannelManager to build channels.
//  Output:			ao_iCrc:				Jar file Crc.
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t	_DownloadJar( _ClRegister_Base& ai_oRegister, ClChannelManager *ai_poChannelManager, m4uint32_t& ao_iCrc )
{

	m4return_t	iResult = M4_ERROR ;
	ClAccess	*poAccess = NULL ;
	ClChannel	*poChannel = NULL ;


	ao_iCrc = 0 ;

	// Se crea una instancia del M4JAVA_JAR_LOADER
	iResult = ai_poChannelManager->CreateChannel( poChannel ) ;

	if( iResult != M4_SUCCESS || poChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poChannel->BuildFromId( "M4JAVA_JAR_LOADER" ) ;

	if( iResult != M4_SUCCESS )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	iResult = poChannel->CreateAccess( poAccess ) ;

	if( iResult != M4_SUCCESS || poAccess == NULL )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	iResult = _DownloadJar( ai_oRegister, poAccess, ao_iCrc ) ;

	poAccess->Destroy() ;
	poChannel->Destroy() ;

	return( M4_SUCCESS ) ;
}






//=============================================================================
// ============================ Modelo viejo ==================================
//=============================================================================

#include "m4java.h"
#include "m4stl.hpp"

m4return_t GetM4ObjecHandle(m4pvoid_t ai_pvContext, m4uint32_t & ao_iM4ObjectHandle)
{
	ClAccess *	poAccess = NULL;
	ClChannel *	poChannel = NULL;
	
	// Se obtiene el acceso al M4Object.
	poAccess = ((ClVMRunContext *)ai_pvContext)->m_pAccess;
	if (poAccess == NULL) 
	{
		return M4_ERROR;
	}

	// Se obtiene el channel.
	poChannel = poAccess->GetpChannel();
	if (poChannel == NULL) 
	{
		return M4_ERROR;
	}

	// Se obtiene el M4Object handle.
	ao_iM4ObjectHandle = poChannel->GetHandle();

	return M4_SUCCESS;
}


m4return_t GetJarInfo(m4pvoid_t ai_pvContext, vector<string> & ao_vIdJarList, vector<string> & ao_vIdJarVersionList, vector<string> & ao_vJarFileList)
{
	ClAccess			* poAccess = NULL;
	ClAccessRecordSet	* oRecordSet = NULL;
	m4VariantType		vIdJar;
	m4VariantType		vIdJarVersion;
	m4VariantType		vJarFilePath;
	string				sIdJar;
	string				sIdJarVersion;
	string				sJarFilePath;
	int					iCount = 0;
		
	// Se obtiene el acceso al M4Object.
	poAccess = ((ClVMRunContext *)ai_pvContext)->m_pAccess;
	if (poAccess == NULL) 
	{
		return M4_ERROR;
	}

	// Bug 0162268.
	// Obtener el recordset del nodo actual.
	oRecordSet = ((ClVMRunContext *)ai_pvContext)->m_pRecordSet;
	
	// Si no hay registros en el nodo, no se hace nada.
	iCount = oRecordSet->Count();
	if (iCount == 0)
	{
		return M4_SUCCESS;
	}
	
	// Posicionarse.
	if (oRecordSet->Current.Begin() != M4_TRUE)
	{
		return M4_ERROR;
	}

	// Recorrer registros del nodo.
	while (oRecordSet->Current.Index != M4DM_EOF_INDEX)
	{
		// Se obtiene el identificador del jar.
		if (oRecordSet->Current.Item["ID_JAR"].Value.Get(vIdJar) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		
		if (vIdJar.Type != M4CL_M4_TYPE_NULL && vIdJar.Data.PointerChar != NULL)
		{
			sIdJar = vIdJar.Data.PointerChar;
		}

		// Se obtiene la versión del jar.
		if (oRecordSet->Current.Item["ID_JAR_VER"].Value.Get(vIdJarVersion) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		
		if (vIdJarVersion.Type != M4CL_M4_TYPE_NULL)
		{
			sIdJarVersion = vIdJarVersion.Data.PointerChar;
		}

		// Hay que bajarse el jar
		m4uint32_t iCrc = 0;
		if (_DownloadJar(oRecordSet->Current, oRecordSet->GetpChannel()->GetpChannelManager(), iCrc) != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		// Se obtiene el path a jar.
		if (oRecordSet->Current.Item["JAR_FILE"].Value.Get(vJarFilePath) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		
		if (vJarFilePath.Type != M4CL_M4_TYPE_NULL && vJarFilePath.Data.PointerChar != NULL)
		{
			sJarFilePath = vJarFilePath.Data.PointerChar;
		}

		// Guardar en los vectores de salida.
		ao_vIdJarList.push_back(sIdJar);
		ao_vIdJarVersionList.push_back(sIdJarVersion);
		ao_vJarFileList.push_back(sJarFilePath);

		oRecordSet->Current.Next();
	}

	return M4_SUCCESS;
}


m4return_t _Old_M4CreateObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4uint32_t			uiM4OHandle = 0;
	string				sClassName;
	VariantList			oConstructorArgList;
	vector<string>		vIdJarList;
	vector<string>		vIdJarVersionList;
	vector<string>		vJarFileList;
	m4return_t			iRet = M4_SUCCESS;
	jobject				jNewObject = NULL;
	
	// Inicializaciones.
	vIdJarList.reserve(10);
	vIdJarVersionList.reserve(10);
	vJarFileList.reserve(10);

	// Handle al objeto que se va a crear, como retorno del método.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = -1;
	
	// Retorno del método, como argumento de salida.
	ai_pvArg[1].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[1].Data.DoubleData = M4_ERROR;

	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF(ai_iLongArg < 2, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4CreateObject4J" << ai_iLongArg << 2);

	// Nombre de clase para crear el objeto.
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Class Name" << "M4CreateObject4J" << M4ClCppType(ai_pvArg[0].Type) << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR));
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0' , M4_SUCCESS, M4JAVAAPI_ERROR_NULL_ARGUMENT, "Class Name" << "M4CreateObject4J");

	sClassName = ai_pvArg[0].Data.PointerChar;

	// Posibles parámetros para el constructor de la clase (opcional).
	for (int i = 2; i < ai_iLongArg; i++)
	{
		oConstructorArgList.push_back(ai_pvArg[i]);
	}

	// Obtener el id. M4Object desde el que nos llaman.
	iRet = GetM4ObjecHandle(ai_pvContext, uiM4OHandle);
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_CHANNEL_HANDLE, "M4CreateObject4J");

	// Obtener la lista de jars que se necesitan para invocar al objeto Java.
	iRet = GetJarInfo(ai_pvContext, vIdJarList, vIdJarVersionList, vJarFileList);
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_JAR_FILES, "M4CreateObject4J");

	// Crear objeto Java.
	iRet = M4JVM_CreateObject(uiM4OHandle, vIdJarList, vIdJarVersionList, vJarFileList, sClassName, oConstructorArgList, jNewObject);
	if (iRet != M4_SUCCESS)
	{
		// Bug 179139. Se resuelve la casuística concreta indicada en el bug
		// (si no se puede crear una clase java por no haber sido posible
		//  inicializar la JVM se indica esta última casuística).
		// Es la solución más simple que cumple con lo solicitado.
		// Si en algún momento es necesario dar más detalles de este u otro
		// error entonces la m4java debería, o bien, manejar directamente la log
		// o bien, devolver más información de retorno aparte de M4_SUCCESS y M4_ERROR
		if (NULL == M4JVM_GetJavaEnv())
		{
			// No hay JVM
			DUMP_CHLOG_ERROR(M4JAVAAPI_ERROR_JVM_NO_INIT);
		}
		else
		{
			DUMP_CHLOG_ERRORF(M4JAVAAPI_ERROR_CANNOT_CREATE_OBJECT, sClassName.c_str());
		}
	}

	// Borrar listas.
	oConstructorArgList.clear();
	vIdJarList.clear();
	vIdJarVersionList.clear();
	vJarFileList.clear();

	// Establecer el argumento de salida, si se ha podido ejecutar o ha habido errores.
	ai_pvArg[1].Data.DoubleData = iRet;
	
	// Establecer retorno del método, handle al objecto creado.
	ai_pvReturn.Data.DoubleData = (jint)jNewObject;

	return M4_SUCCESS;
}


m4return_t _Old_M4InvokeMethod4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t		iRet = M4_SUCCESS;
	jobject			hObject = 0;
	string			sMethodName;
	VariantList		oMethodArgList;
	m4uint32_t		uiM4OHandle = 0;

	// Inicializaciones.
	// Handle al objeto que se devulve como resultado de la ejecución del método Java
	// (como retorno del método).
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = -1;
	
	// Retorno del método, como argumento de salida.
	ai_pvArg[2].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[2].Data.DoubleData = M4_ERROR;

	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF(ai_iLongArg < 3, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4InvokeMethod4J" << ai_iLongArg << 3);

	// Handle al objeto Java.
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Java Object" << "M4InvokeMethod4J" << M4ClCppType(ai_pvArg[0].Type) << M4ClCppType(M4CL_CPP_TYPE_NUMBER));
	
	hObject = (jobject)((m4uint32_t)ai_pvArg[0].Data.DoubleData);

	// Nombre del método a ejecutar.
	CHECK_CHLOG_ERRORF(ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Method Name" << "M4InvokeMethod4J" << M4ClCppType(ai_pvArg[1].Type) << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR));
	CHECK_CHLOG_ERRORF(ai_pvArg[1].Data.PointerChar == NULL || *(ai_pvArg[1].Data.PointerChar) == '\0' , M4_SUCCESS, M4JAVAAPI_ERROR_NULL_ARGUMENT, "Method Name" << "M4InvokeMethod4J");

	sMethodName = ai_pvArg[1].Data.PointerChar;

	// Posibles parámetros para el método (opcional).
	for (int i = 3; i < ai_iLongArg; i++)
	{
		oMethodArgList.push_back(ai_pvArg[i]);
	}

	// Obtener el id. M4Object desde el que nos llaman.
	iRet = GetM4ObjecHandle(ai_pvContext, uiM4OHandle);
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_CHANNEL_HANDLE, "M4InvokeMethod4J");

	// Invocar método Java.
	iRet = M4JVM_InvokeMethod(uiM4OHandle, hObject, sMethodName, oMethodArgList, ai_pvReturn);
	if (iRet != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4JAVAAPI_ERROR_CANNOT_INVOKE_METHOD, sMethodName.c_str());
	}

	// Establecer el argumento de salida, si se ha podido ejecutar o ha habido errores.
	ai_pvArg[2].Data.DoubleData = iRet;

	return M4_SUCCESS;
}

  
m4return_t _Old_M4DestroyObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t		iRet = M4_SUCCESS;
	jobject			hObject = 0;
	m4uint32_t		uiM4OHandle = 0;
	
	// Inicializaciones.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;
	
	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF(ai_iLongArg < 1, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4DestroyObject4J" << ai_iLongArg << 1);

	// Handle al objeto Java.
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Java Object" << "M4DestroyObject4J" << M4ClCppType(ai_pvArg[0].Type) << M4ClCppType(M4CL_CPP_TYPE_NUMBER));
	hObject = (jobject)((m4uint32_t)ai_pvArg[0].Data.DoubleData);

	// Obtener el id. M4Object desde el que nos llaman.
	iRet = GetM4ObjecHandle(ai_pvContext, uiM4OHandle);
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_CHANNEL_HANDLE, "M4DestroyObject4J" << "M4DestroyObject4J");

	// Destruir objecto Java.
	iRet = M4JVM_DestroyObject(uiM4OHandle, hObject);
	if (iRet != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4JAVAAPI_ERROR_CANNOT_DESTROY_OBJECT, hObject);
	}

	// Establecer el retorno del método.
	ai_pvReturn.Data.DoubleData = iRet;

	return M4_SUCCESS;
}


//=============================================================================
// =========================== Fin modelo viejo ===============================
//=============================================================================





#ifdef _UNIX
	#define	M4_JAVA_MAX_PATH				PATH_MAX
	#define	M4_JAVA_PATH_SEPARATOR			":"
	#define	M4_DIRECTORY_SEPARATOR			"/"
#else
	#define	M4_JAVA_MAX_PATH				MAX_PATH
	#define	M4_JAVA_PATH_SEPARATOR			";"
	#define	M4_DIRECTORY_SEPARATOR			"\\"
#endif



extern "C" M4_THREAD_RETURN	M4DecompressPath( m4pvoid_t ai_pvArgument )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bNeedToDecompress = M4_FALSE ;
	m4pchar_t			pcPath = NULL ;
	string				sPath ;
	M4JavaConfiguration	*poConfiguration = NULL ;


	M4SetThreadMinPriority() ;

	pcPath = (m4pchar_t)ai_pvArgument ;

	if( pcPath != NULL )
	{
		poConfiguration = M4JavaConfiguration::Instance( NULL ) ;

		if( poConfiguration != NULL )
		{
			iResult = poConfiguration->NormalizePath( pcPath, M4_TRUE, bNeedToDecompress, sPath ) ;
		}

		iResult = ClFileManager::Instance()->UnSetInterestInFile( pcPath ) ;
		delete( pcPath ) ;
	}

	return( 0 ) ;
}



//=============================================================================
//	Name:			_GetChannel
//	Description:	This function returns context channel.
//
//	Parameters:
//  Input:			ai_pvContext:		context.
//
//	Return:			channel		No error.	
//					NULL		Error.
//
//  Additional Information:
//
//=============================================================================

ClChannel*	_GetChannel( m4pvoid_t ai_pvContext )
{
	ClAccess	*poAccess = ((ClVMRunContext *)ai_pvContext)->m_pAccess ;

	if( poAccess != NULL )
	{
		return( poAccess->GetpChannel() ) ;
	}
	return( NULL ) ;
}


//=============================================================================
//	Name:			_GetModel
//	Description:	This function returns java execution model.
//
//	Parameters:
//  Input:			ai_pvContext:		context.
//
//	Return:			0	Old model.
//					1	New model (out process).
//
//  Additional Information:
//
//=============================================================================

m4uint8_t	_GetModel( m4pvoid_t ai_pvContext )
{

	m4return_t			iResult = M4_ERROR ;
	m4VariantType		vValue ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClAccess			*poAccess = NULL ;
	ClVMBaseEnv			*poEnvironment = NULL ;


	poRecordSet = ((ClVMRunContext *)ai_pvContext)->m_pRecordSet ;
	CHECK_CHLOG_ERRORF( poRecordSet == NULL, 0, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	vValue.SetNull() ;
	iResult = poRecordSet->Item[ "MODEL" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 0 )
	{
		poAccess = ((ClVMRunContext *)ai_pvContext)->m_pAccess ;

		if( poAccess != NULL )
		{
			poEnvironment = poAccess->GetEnvironment() ;

			if( poEnvironment != NULL )
			{
				if( poEnvironment->GetJavaAllowInProcess() == M4_FALSE )
				{
					vValue.SetDouble( 1 ) ;
					poRecordSet->Item[ "MODEL" ].Value.Set( vValue ) ;
					DUMP_CHLOG_ERROR( M4JAVA_IN_PROCESS_NOT_ALLOWED ) ;
					return( 1 ) ;
				}
			}
		}

		return( 0 ) ;
	}

	return( 1 ) ;
}


//=============================================================================
//	Name:			_GetClassPath
//	Description:	This function returns claspath.
//
//	Parameters:
//  Input:			ai_pvContext:		context.
//
// 	Output:			ao_rsClassPath:		classpath.
// 	Output:			ao_rsClassCrc:		classCRC.
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t	_GetClassPath( m4pvoid_t ai_pvContext, string& ao_rsClassPath, string& ao_rsClassCrc )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bIsNotEof = M4_FALSE ;
	m4bool_t			bNeedToDecompress = M4_FALSE ;
	int					iStatus = -1 ;
	m4uint32_t			iCrc = 0 ;
	struct stat			stStatus ;
	m4pchar_t			pcUserPath = NULL ;
	m4char_t			acJarDirectory[ M4_JAVA_MAX_PATH + 1 ] ;
	m4char_t			acCrc[ 128 + 1 ] ;
	m4VariantType		vValue ;
	m4VariantType		vOtherValue ;
	string				sUserPath ;
	string				sUserCrc ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClChannel			*poChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	ClVMBaseEnv			*poEnvironment = NULL ;
	M4JavaConfiguration	*poConfiguration = NULL ;
	M4Thread			*poThread = NULL ;


	ao_rsClassPath = "";
	ao_rsClassCrc = "" ;

	poRecordSet = ((ClVMRunContext *)ai_pvContext)->m_pRecordSet ;
	CHECK_CHLOG_ERRORF( poRecordSet == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ClRegister&	oRegister = poRecordSet->Register ;

	vValue.SetNull() ;
	iResult = poRecordSet->Item[ "CLASSPATH" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL && *vValue.Data.PointerChar != '\0' )
	{
		vOtherValue.SetNull() ;
		iResult = poRecordSet->Item[ "CLASSCRC" ].Value.Get( vOtherValue ) ;

		if( iResult == M4_SUCCESS && vOtherValue.Type == M4CL_CPP_TYPE_STRING_VAR && vOtherValue.Data.PointerChar != NULL && *vOtherValue.Data.PointerChar != '\0' )
		{
			ao_rsClassPath = vValue.Data.PointerChar ;
			ao_rsClassCrc = vOtherValue.Data.PointerChar ;
			return( M4_SUCCESS ) ;
		}
	}

	bIsNotEof = oRegister.Begin() ;

	if( bIsNotEof == M4_TRUE )
	{
		poChannel = poRecordSet->GetpChannel() ;
		CHECK_CHLOG_ERRORF( poChannel == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poChannelManager = poChannel->GetpChannelManager() ;
		CHECK_CHLOG_ERRORF( poChannelManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poEnvironment = poChannelManager->GetEnvironment() ;
		CHECK_CHLOG_ERRORF( poEnvironment == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConfiguration = M4JavaConfiguration::Instance( poEnvironment ) ;
		CHECK_CHLOG_ERRORF( poConfiguration == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		while( bIsNotEof == M4_TRUE )
		{
			sUserPath = "" ;
			sUserCrc = "" ;
			iCrc = 0 ;

			iResult = oRegister.Item[ "JAR_CRC" ].Value.Get( vValue ) ;

			if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData > 0 )
			{
				iCrc = m4uint32_t( vValue.Data.DoubleData ) ;
				sprintf( acJarDirectory, "%s%s%lu.jar", poConfiguration->GetJarDirectory(), M4_DIRECTORY_SEPARATOR, iCrc ) ;

				iStatus = M4FileStatus( acJarDirectory, stStatus ) ;

				if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) != 0 )
				{
					// Si existe y es un directorio se ha encontrado
					sUserPath = acJarDirectory ;
					sprintf( acCrc, "%lu", iCrc ) ;
					sUserCrc = acCrc;
				}
			}

			if( sUserPath.size() == 0 )
			{
				// Hay que bajarse el jar
				iResult = _DownloadJar( oRegister, poChannelManager, iCrc ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				iResult = oRegister.Item[ "JAR_FILE" ].Value.Get( vValue ) ;

				if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL && *vValue.Data.PointerChar != '\0' )
				{
					iResult = poConfiguration->NormalizePath( vValue.Data.PointerChar, M4_FALSE, bNeedToDecompress, sUserPath ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}

					sprintf( acCrc, "%lu", iCrc ) ;
					sUserCrc = acCrc;

					if( bNeedToDecompress == M4_TRUE )
					{
						// Se expande en otro thread
						pcUserPath = new m4char_t[ sUserPath.size() + 1 ] ;
						CHECK_CHLOG_ERRORF( pcUserPath == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( ( sUserPath.size() + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
						memcpy( pcUserPath, sUserPath.c_str(), sUserPath.size() + 1 ) ;

						poThread = new M4Thread() ;
						CHECK_CHLOG_ERRORF( poThread == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( sizeof( M4Thread ) ) << __LINE__ << __FILE__ ) ;

						ClFileManager::Instance()->SetInterestInFile( pcUserPath ) ;
						iResult = poThread->Create( M4DecompressPath, pcUserPath ) ;
						delete( poThread ) ;
						poThread = NULL ;
					}
				}
			}

			if( sUserPath.size() > 0 )
			{
				if( ao_rsClassPath.size() > 0 )
				{
					ao_rsClassPath.append( M4_JAVA_PATH_SEPARATOR ) ;
					ao_rsClassCrc.append( M4_JAVA_PATH_SEPARATOR ) ;
				}
				ao_rsClassPath.append( sUserPath.c_str() ) ;
				ao_rsClassCrc.append( sUserCrc.c_str() ) ;
			}

			bIsNotEof = oRegister.Next();
		}
	}

	vValue.SetStringRef( (m4pchar_t)ao_rsClassPath.c_str() ) ;
	iResult = poRecordSet->Item[ "CLASSPATH" ].Value.Set( vValue ) ;

	vValue.SetStringRef( (m4pchar_t)ao_rsClassCrc.c_str() ) ;
	iResult = poRecordSet->Item[ "CLASSCRC" ].Value.Set( vValue ) ;

	return( M4_SUCCESS );
}


//=============================================================================
//	Name:			M4CreateObject4J
//	Description:	This function creates a Java object from a given class.
//
//	Parameters:
//  Input:			0		(Input)  -> Class name.
//					1		(Input)  -> method returned.
//					2...n	(Input)  -> Class constructor parameters [optional].
//					
//
// 	Output:			handle of the new object created.
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t _M4CreateObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	m4return_t			iResult = M4_SUCCESS;
	m4uint32_t			i = 0 ;
	m4uint32_t			iLength = 0 ;
	m4double_t			dHandle = 0 ;
	m4pcchar_t			pccClassName = NULL ;
	m4char_t			acBuffer[ M4CL_MAX_T3_ID + M4CL_MAX_NODE_ID + M4CL_MAX_ITEM_ID + 128 + 4 + 1 ] ;
	string				sClassPath ;
	string				sClassCrc ;
	ClChannel			*poChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	M4JavaManager		*poJavaManager = NULL ;
	M4JavaObject		*pvArguments = NULL ;
	

	// Handle al objeto que se va a crear, como retorno del método.
	ai_pvReturn.SetDouble( -1 ) ;
	
	// Retorno del método, como argumento de salida.
	ai_pvArg[ 1 ].SetDouble( M4_ERROR ) ;

	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF( ai_iLongArg < 2, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4CreateObject4J" << ai_iLongArg << 2 ) ;

	// Nombre de clase para crear el objeto.
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Class Name" << "M4CreateObject4J" << M4ClCppType( ai_pvArg[0].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0', M4_SUCCESS, M4JAVAAPI_ERROR_NULL_ARGUMENT, "Class Name" << "M4CreateObject4J" ) ;

	poChannel = _GetChannel( ai_pvContext ) ;
	CHECK_CHLOG_ERRORF( poChannel == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poChannelManager = poChannel->GetpChannelManager() ;
	CHECK_CHLOG_ERRORF( poChannelManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poJavaManager = poChannelManager->GetJavaManager() ;
	CHECK_CHLOG_ERRORF( poJavaManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = _GetClassPath( ai_pvContext, sClassPath, sClassCrc ) ;
	CHECK_CHLOG_ERROR( iResult != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_JAR_FILES ) ;

	iLength = ai_iLongArg - 2 ;

	if( iLength > 0 )
	{
		pvArguments = new M4JavaObject[ iLength ] ;
		CHECK_CHLOG_ERRORF( pvArguments == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( ( iLength ) * sizeof( M4JavaObject ) ) << __LINE__ << __FILE__ ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			pvArguments[ i ].SetVariant( ai_pvArg[ i + 2 ], M4_FALSE, M4_FALSE  ) ;
		}
	}

	_PrintExecutionLine( acBuffer, ((ClVMRunContext *)ai_pvContext ) ) ;
	iResult = poJavaManager->CreateObject( poChannel, sClassPath.c_str(), sClassCrc.c_str(), ai_pvArg[0].Data.PointerChar, pvArguments, iLength, dHandle, acBuffer ) ;

	if( pvArguments != NULL )
	{
		delete [] pvArguments ;
		pvArguments = NULL ;
	}

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4JAVA_CREATE_OBJECT_ERROR, ai_pvArg[0].Data.PointerChar << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
	}

	// Establecer el argumento de salida, si se ha podido ejecutar o ha habido errores.
	ai_pvReturn.SetDouble( dHandle ) ;
	ai_pvArg[ 1 ].SetDouble( iResult ) ;

	return( M4_SUCCESS ) ;
}


m4return_t M4CreateObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	if( _GetModel( ai_pvContext ) == 1 )
	{
		return( _M4CreateObject4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
	}
	return( _Old_M4CreateObject4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
}


//=============================================================================
//	Name:			M4DestroyObject4J
//	Description:	This function destroys a Java object.
//
//	Parameters:
// 	Input:			0		(Input) -> Object.
//
//	Output:
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t _M4DestroyObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	m4return_t			iResult = M4_SUCCESS;
	m4pcchar_t			pccClassName = NULL ;
	m4char_t			acBuffer[ M4CL_MAX_T3_ID + M4CL_MAX_NODE_ID + M4CL_MAX_ITEM_ID + 128 + 4 + 1 ] ;
	ClChannel			*poChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	M4JavaManager		*poJavaManager = NULL ;


	// Inicializaciones.
	ai_pvReturn.SetDouble( M4_ERROR ) ;
	
	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF( ai_iLongArg < 1, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4DestroyObject4J" << ai_iLongArg << 1 ) ;

	// Handle al objeto Java.
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Java Object" << "M4DestroyObject4J" << M4ClCppType( ai_pvArg[0].Type ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) ) ;

	poChannel = _GetChannel( ai_pvContext ) ;
	CHECK_CHLOG_ERRORF( poChannel == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poChannelManager = poChannel->GetpChannelManager() ;
	CHECK_CHLOG_ERRORF( poChannelManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poJavaManager = poChannelManager->GetJavaManager() ;
	CHECK_CHLOG_ERRORF( poJavaManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	pccClassName = poJavaManager->GetJavaClassName( ai_pvArg[0].Data.DoubleData ) ;

	_PrintExecutionLine( acBuffer, ((ClVMRunContext *)ai_pvContext ) ) ;
	iResult = poJavaManager->DestroyObject( poChannel, ai_pvArg[0].Data.DoubleData, acBuffer ) ;

	if( iResult != M4_SUCCESS )
	{
		if( pccClassName != NULL )
		{
			DUMP_CHLOG_ERRORF( M4JAVA_DESTROY_OBJECT_ERROR, ai_pvArg[0].Data.DoubleData << pccClassName << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
		}
		else
		{
			DUMP_CHLOG_ERRORF( M4JAVA_DESTROY_OBJECT_ERROR_NO_CLASS, ai_pvArg[0].Data.DoubleData << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
		}
	}

	// Establecer el retorno del método.
	ai_pvReturn.SetDouble( iResult ) ;

	return( M4_SUCCESS ) ;
}


m4return_t M4DestroyObject4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	if( _GetModel( ai_pvContext ) == 1 )
	{
		return( _M4DestroyObject4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
	}
	return( _Old_M4DestroyObject4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
}


//=============================================================================
//	Name:			M4InvokeMethod4J
//	Description:	This function invokes a method from a Java object.
//
//	Parameters:
//  Input:			0		(Input) -> Java object handle o java class name.
//					1		(Input) -> method name.
//					2		(Input) -> method returned.
//					3...n	(Input) -> Class constructor arguments [optional].
//
// 	Output:
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t _M4InvokeMethod4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	m4return_t			iResult = M4_SUCCESS ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iLength = 0 ;
	m4pcchar_t			pccClassName = NULL ;
	m4char_t			acBuffer[ M4CL_MAX_T3_ID + M4CL_MAX_NODE_ID + M4CL_MAX_ITEM_ID + 128 + 4 + 1 ] ;
	string				sClassPath ;
	string				sClassCrc ;
	ClChannel			*poChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	M4JavaManager		*poJavaManager = NULL ;
	M4JavaObject		*pvArguments = NULL ;
	M4JavaObject		vReturn ;


	// Handle al objeto que se va a crear, como retorno del método.
	ai_pvReturn.SetDouble( -1 ) ;
	
	// Retorno del método, como argumento de salida.
	ai_pvArg[ 2 ].SetDouble( M4_ERROR ) ;

	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF( ai_iLongArg < 3, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4InvokeMethod4J" << ai_iLongArg << 3 ) ;

	// Handle al objeto Java o nombre de la clase.
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER && ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Java Object" << "M4InvokeMethod4J" << M4ClCppType( ai_pvArg[0].Type ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) ) ;
	
	// Nombre del método a ejecutar.
	CHECK_CHLOG_ERRORF( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE, "Method Name" << "M4InvokeMethod4J" << M4ClCppType( ai_pvArg[1].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[1].Data.PointerChar == NULL || *(ai_pvArg[1].Data.PointerChar) == '\0' , M4_SUCCESS, M4JAVAAPI_ERROR_NULL_ARGUMENT, "Method Name" << "M4InvokeMethod4J" ) ;

	poChannel = _GetChannel( ai_pvContext ) ;
	CHECK_CHLOG_ERRORF( poChannel == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poChannelManager = poChannel->GetpChannelManager() ;
	CHECK_CHLOG_ERRORF( poChannelManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poJavaManager = poChannelManager->GetJavaManager() ;
	CHECK_CHLOG_ERRORF( poJavaManager == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iLength = ai_iLongArg - 3 ;

	if( iLength > 0 )
	{
		pvArguments = new M4JavaObject[ iLength ] ;
		CHECK_CHLOG_ERRORF( pvArguments == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( ( iLength ) * sizeof( M4JavaObject ) ) << __LINE__ << __FILE__ ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			pvArguments[ i ].SetVariant( ai_pvArg[ i + 3 ], M4_FALSE, M4_FALSE ) ;
		}
	}

	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER )
	{
		_PrintExecutionLine( acBuffer, ((ClVMRunContext *)ai_pvContext ) ) ;
		iResult = poJavaManager->InvokeMethod( poChannel, ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.PointerChar, pvArguments, iLength, vReturn, acBuffer ) ;

		if( iResult != M4_SUCCESS )
		{
			pccClassName = poJavaManager->GetJavaClassName( ai_pvArg[0].Data.DoubleData ) ;

			if( pccClassName != NULL )
			{
				DUMP_CHLOG_ERRORF( M4JAVA_OBJECT_METHOD_ERROR, ai_pvArg[1].Data.PointerChar << ai_pvArg[0].Data.DoubleData << pccClassName << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
			}
			else
			{
				DUMP_CHLOG_ERRORF( M4JAVA_OBJECT_METHOD_ERROR_NO_CLASS, ai_pvArg[1].Data.PointerChar << ai_pvArg[0].Data.DoubleData << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
			}
		}
	}
	else
	{
		iResult = _GetClassPath( ai_pvContext, sClassPath, sClassCrc ) ;
		CHECK_CHLOG_ERROR( iResult != M4_SUCCESS, M4_SUCCESS, M4JAVAAPI_ERROR_CANNOT_GET_JAR_FILES ) ;

		_PrintExecutionLine( acBuffer, ((ClVMRunContext *)ai_pvContext ) ) ;
		iResult = poJavaManager->InvokeMethod( poChannel, sClassPath.c_str(), sClassCrc.c_str(), ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, pvArguments, iLength, vReturn, acBuffer ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4JAVA_CLASS_METHOD_ERROR, ai_pvArg[1].Data.PointerChar << ai_pvArg[0].Data.PointerChar << DumpVMContext( (ClVMRunContext *)ai_pvContext ) ) ;
		}
	}

	if( pvArguments != NULL )
	{
		delete [] pvArguments ;
		pvArguments = NULL ;
	}

	// Establecer el argumento de salida, si se ha podido ejecutar o ha habido errores.
	vReturn.GetVariant( ai_pvReturn, M4_FALSE, M4_TRUE ) ;
	ai_pvArg[ 2 ].SetDouble( iResult ) ;

	return( M4_SUCCESS ) ;
}



m4return_t M4InvokeMethod4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	if( _GetModel( ai_pvContext ) == 1 )
	{
		return( _M4InvokeMethod4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
	}
	return( _Old_M4InvokeMethod4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
}



//=============================================================================
//	Name:			M4ReleaseContexts4J
//	Description:	This function releases all java context of the invoking meta4object.
//
//	Parameters:
//  Input:			None
//
// 	Output:
//
//	Return:			M4_SUCCESS	No error.
//					M4_ERROR	Error.
//
//  Additional Information:
//
//=============================================================================

m4return_t _M4ReleaseContexts4J(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	ClChannel	*poChannel = NULL ;


	// Siempre se retorna success.
	ai_pvReturn.SetDouble( 0 ) ;
	
	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF( ai_iLongArg > 0, M4_SUCCESS, M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS, "M4ReleaseContexts4J" << ai_iLongArg << 0 ) ;

	poChannel = _GetChannel( ai_pvContext ) ;
	CHECK_CHLOG_ERRORF( poChannel == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poChannel->ReleaseJavaContexts( M4_FALSE ) ;

	return( M4_SUCCESS ) ;
}


m4return_t M4ReleaseContexts4J( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	if( _GetModel( ai_pvContext ) == 1 )
	{
		return( _M4ReleaseContexts4J( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext ) ) ;
	}

	ai_pvReturn.SetDouble( 0 ) ;
	return( M4_SUCCESS ) ;
}
