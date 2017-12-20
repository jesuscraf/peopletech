
#include "m4jnisoapclient.hpp"
#include "m4jnisoapclientres.hpp"
#include "m4java.h"
#include "m4jcontext.hpp"
#include "execontx.hpp"
#include "access.hpp"
#include "m4objglb.hpp"
#include "chlog.hpp"



#define __S__(x)				#x
#define __L__(x)				__S__(x)
#define __SOURCE__				__FILE__ "(" __L__(__LINE__) ")"


#define	SOAPCLIENT_CLASSNAME	"com.meta4.soapclient.M4JNISoapClient"	



/****************************************************************************
*	Name:			M4GenerateSoapClient
*	Description:	This function generates a jar file containing required
*					classes for a SOAP client from a WSDL file.
*
*	Parameters:
*  	Input:			0		(Input) -> WSDL file.
*
* 	Output:			1		(Output) -> Jar file.
*					2		(Output) -> Jre version.
*					3		(Output) -> Axis version.
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t M4GenerateSoapClient(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	m4return_t			iRet = M4_SUCCESS;
	string				sIdClient;
	string				sWSDLFile;
	string				sJarFile;
	string				sJreVersion;
	string				sAxisVersion;
	M4JavaObject		vResult;
	M4JavaObject		vArguments[3];
	M4JavaContext		oContext(NULL, ((ClVMRunContext*)ai_pvContext)->m_pAccess->GetpExecutor());

	
	// Por defecto devolvemos error
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;
	
	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF(ai_iLongArg != 5, M4_SUCCESS, M4JSOAPCL_ERROR_BAD_NUMBER_ARGUMENTS, "M4GenerateSoapClient" << ai_iLongArg << 1);

	// Identificador del cliente.
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JSOAPCL_ERROR_BAD_ARGUMENT_TYPE, "Client Id" << "M4GenerateSoapClient" << M4ClCppType(ai_pvArg[0].Type) << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR));
	CHECK_CHLOG_ERRORF(ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0' , M4_SUCCESS, M4JSOAPCL_ERROR_NULL_ARGUMENT, "Client Id" << "M4GenerateSoapClient");
	sIdClient = ai_pvArg[0].Data.PointerChar;

	// Fichero WSDL.
	CHECK_CHLOG_ERRORF(ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JSOAPCL_ERROR_BAD_ARGUMENT_TYPE, "WSDL file" << "M4GenerateSoapClient" << M4ClCppType(ai_pvArg[1].Type) << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR));
	CHECK_CHLOG_ERRORF(ai_pvArg[1].Data.PointerChar == NULL || *(ai_pvArg[1].Data.PointerChar) == '\0' , M4_SUCCESS, M4JSOAPCL_ERROR_NULL_ARGUMENT, "WSDL File" << "M4GenerateSoapClient");
	sWSDLFile = ai_pvArg[1].Data.PointerChar;

	// Fichero Jar.
	CHECK_CHLOG_ERRORF(ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JSOAPCL_ERROR_BAD_ARGUMENT_TYPE, "Jar file" << "M4GenerateSoapClient" << M4ClCppType(ai_pvArg[2].Type) << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR));
	CHECK_CHLOG_ERRORF(ai_pvArg[2].Data.PointerChar == NULL || *(ai_pvArg[2].Data.PointerChar) == '\0' , M4_SUCCESS, M4JSOAPCL_ERROR_NULL_ARGUMENT, "Jar File" << "M4GenerateSoapClient");
	sJarFile = ai_pvArg[2].Data.PointerChar;

	
	iRet = oContext.Open(((ClVMRunContext*)ai_pvContext)->m_pAccess->GetEnvironment(), "m4jnisoapclient", NULL, NULL);
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JSOAPCL_ERROR_CANNOT_GENERATE_SOAP_CLIENT, sWSDLFile.c_str());

	// Crear fichero jar que contiene las clases necesarias para un client SOAP.
	vArguments[ 0 ].SetString(sIdClient.c_str(), M4_FALSE); 
	vArguments[ 1 ].SetString(sWSDLFile.c_str(), M4_FALSE); 
	vArguments[ 2 ].SetString(sJarFile.c_str(), M4_FALSE); 

	iRet = oContext.InvokeMethod(SOAPCLIENT_CLASSNAME, "generateSoapClient", vArguments, 3, vResult, M4_TRUE, __SOURCE__ );
	CHECK_CHLOG_ERRORF(iRet != M4_SUCCESS, M4_SUCCESS, M4JSOAPCL_ERROR_CANNOT_GENERATE_SOAP_CLIENT, sWSDLFile.c_str());

	// Obtener la version de la máquina virtual de Java.
	iRet = oContext.InvokeMethod(SOAPCLIENT_CLASSNAME, "getJreVersion", NULL, 0, vResult, M4_TRUE, __SOURCE__ );
	
	if (iRet != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4JSOAPCL_ERROR_GET_JRE_VERSION);
	}
	else
	{
		sJreVersion = vResult.GetString();
	}

	// Obtener la version de axis.
	iRet = oContext.InvokeMethod(SOAPCLIENT_CLASSNAME, "getAxisVersion", NULL, 0, vResult, M4_TRUE, __SOURCE__ );
	
	if (iRet != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4JSOAPCL_ERROR_GET_AXIS_VERSION);
	}
	else
	{
		sAxisVersion = vResult.GetString();
	}
	
	// Establecer argumentos de salida.
	// Jre version.
	ai_pvArg[3].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[3].Data.PointerChar = new m4char_t[strlen(sJreVersion.c_str()) + 1];
	strcpy(ai_pvArg[3].Data.PointerChar, sJreVersion.c_str());

	// Axis version.
	ai_pvArg[4].Type = M4CL_CPP_TYPE_STRING_VAR;
	ai_pvArg[4].Data.PointerChar = new m4char_t[strlen(sAxisVersion.c_str()) + 1];
	strcpy(ai_pvArg[4].Data.PointerChar, sAxisVersion.c_str());

	// Establecer retorno del método.
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

