//
// clappinfo.cpp
//

#include "basiclog.hpp"
#include <string.h>
#include "clappinfo.hpp"
#include "m4trace.hpp"
#include "m4ini.hpp"
#include "cmdlnlauncher.hpp"
#include <m4launchersaxhandler.hpp>

#include "m4srvtrace.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#define M4_SRV_TRACE_COMP_ID "LNCHR"


#define PARAM_DEFAULT_VALUE		"0"


ClAppInfo* ClAppInfo::m_instance=NULL;

ClAppInfo* ClAppInfo::Instance(cCmdlnLauncher* ai_InputOptions)
{
	// if input file is NULL, return the unique instance.
	if (NULL == ai_InputOptions)
	{
		return m_instance;
	}

	// create the unique instance.
	if( ! m_instance )
	{
		m_instance = new ClAppInfo(ai_InputOptions);
	}

	return m_instance;
}


ClAppInfo::ClAppInfo(cCmdlnLauncher* ai_InputOptions)
{
	m_InputOptions = ai_InputOptions;

	SetValue ( "baseport", "0") ;

}


m4return_t ClAppInfo::GetValue(char* ai_key, char* ao_value ,  m4uint32_t ai_iBufferSize, m4bool_t SwAllowConfigFilesFind)
{
	ParametersMap::iterator it ;
	m4pchar_t pCnfFile ;
	m4char_t szValue [APP_INFO_MAX_BUFFER];
	m4pchar_t pValue = szValue;
	m4char_t szCommand [100];


	it = m_oParamMap.find ( ai_key ) ;

	if (it != m_oParamMap.end() )
	{

		if ( ai_iBufferSize < strlen( (*it).second.c_str() ) )
		{
			M4_SRV_ERROR( 0, "Buffer to small for %0:s", (*it).second.c_str() ) ;
			return M4_ERROR ;
		}

		strcpy ( ao_value , (*it).second.c_str() ) ;

		return M4_SUCCESS ;
	}
	else
	{
		//Para no caer en recursion infinita

		if ( ! stricmp ( "command" , ai_key ) )	{
			return M4_ERROR ;	
		} ;

		//Para no caer en recursion infinita

		if ( ! SwAllowConfigFilesFind ) return M4_ERROR ;

		pCnfFile  = m_InputOptions -> m_configFile.value() ;

		if ( M4_ERROR == ReadConfigurationFile ("global" , ai_key, pValue, pCnfFile  ) )
		{
			if ( M4_ERROR == ReadConfigurationFile ("environment" , ai_key, pValue, pCnfFile  ) )
			{
				if ( M4_ERROR == ReadConfigurationFile ("server" , ai_key, pValue, pCnfFile  ) )
				{
					if ( M4_SUCCESS == GetValue ( "command" , szCommand , 100 ) )
					{
						if ( M4_ERROR == ReadConfigurationFile (szCommand , ai_key, pValue, pCnfFile  ) )
						{
							strcpy ( ao_value , "" ) ;
							return M4_SUCCESS ;
						}
					}
				}
			}
		}
	}

	SetValue ( ai_key , pValue ) ;
	if (   ai_iBufferSize < ( strlen ( pValue ) +1 ) )
	{
		M4_SRV_ERROR(  0, "Buffer to small for %0:s", pValue )  ;
		return M4_ERROR ;
	}

	strcpy ( ao_value , pValue ) ;

	return M4_SUCCESS ;
}


m4return_t ClAppInfo::Init()
{
	m4uint32_t iBufferLen ;


    try
    {
         XMLPlatformUtils::Initialize();
    }
    catch (const XMLException&)
    {
         M4_SRV_ERROR_N( 0, "Error during initialization!" ) ;
//              << StrX(toCatch.getMessage()) << endl;
         return 1;
    }
	
	
	SAXParser parser;
	M4LauncherSAXHandler oHandler ;



    parser.setDocumentHandler(&oHandler);
    parser.setErrorHandler(&oHandler);


	iBufferLen = strlen ((m_InputOptions -> m_xml).value()) +1 ;

	MemBufInputSource * poMemBuffer = 
        new MemBufInputSource((const XMLByte *) (m_InputOptions -> m_xml).value(),
                              iBufferLen,
                              "generic",
                              false);


    try
    {
        parser.parse( * poMemBuffer );
    }

    catch (const XMLException&)
    {
        M4_SRV_ERROR( 0, "File not found: %0:s", (m_InputOptions -> m_xml).value() ) ; 
        return -1;
    }

	if ( M4_ERROR == oHandler.GetError () )
		return M4_ERROR ;

	return M4_SUCCESS ;
}



m4return_t ClAppInfo::Parse(char* aio_value, m4uint32_t ai_BufferSize)
{
	const char * ini_default_value = "ini_default_value";
	const int buffer_size = APP_INFO_MAX_BUFFER;
	char Value[buffer_size];
	char * pValue = Value;

	char before[APP_INFO_MAX_BUFFER], variable[APP_INFO_MAX_BUFFER], after[APP_INFO_MAX_BUFFER];

	while (strchr(aio_value,'%'))
	{

		memset( before, '\0' , APP_INFO_MAX_BUFFER );

		memset( variable, '\0' , APP_INFO_MAX_BUFFER );

		memset( after, '\0' , APP_INFO_MAX_BUFFER );

		SplitParseVariable( aio_value , before, variable, after);
		// get the variable value.
		//buscamos el valor de la variable 1º en el entorno, despues en la sección, 
		//en la sección global y por último en la sección environment


		if ( M4_ERROR == GetValue ( variable ,Value , buffer_size ) )
		{
			M4_SRV_ERROR( 0, "Variable %0:s not found !" , variable ) ;
			return M4_ERROR ;
		}


		//  printf( "Original LIB variable is: %s\n", libvar );
		// set the new output string.
		sprintf(aio_value, "%s%s%s",before, Value, after);
	}


	return M4_SUCCESS ;
}


m4return_t ClAppInfo::SplitParseVariable( char* aio_value, char* ao_before, char* ao_variable, char* ao_after)
{
	m4pchar_t token = NULL;

	const int buffer_size = APP_INFO_MAX_BUFFER;

	m4char_t Buffer[buffer_size];

	m4char_t BufferAux[buffer_size];

	memset(BufferAux,'\0', buffer_size);

	memset(Buffer,'\0', buffer_size);

	strcpy ( Buffer , aio_value );

	int i=0;

	for(i ; i < buffer_size ; i++)
	{
		if(Buffer[i]!='%')
		{
			BufferAux[i] = Buffer[i];
		}
		else
			break;
	}

	strcpy( ao_before , BufferAux );

	token = strtok( Buffer , "%" );
		
	if(Buffer[0]!='%')
		token = strtok( NULL , "%" );

	if ( !token )

		return M4_ERROR; // no hay %

	else
	{
		strcpy(ao_variable,token);
		// busco token en el entorno, seccion o global
		token = strtok( NULL , "\0");

		if(token) strcpy( ao_after, token );

	}

	return M4_SUCCESS;
}


	m4return_t ClAppInfo::SetValue ( m4pcchar_t ai_key, m4pchar_t ai_value ) 
	{
		ParametersMap::iterator it ;

		if ( ai_value [0] )
		{
			it = m_oParamMap.find ( ai_key ) ;

			if ( it != m_oParamMap.end () )
			{
				m_oParamMap.erase (it ) ;
			}

			m_oParamMap.insert ( ParametersMap::value_type ( ai_key , ai_value ) ) ; //ai_key  , ai_value  ) ) ;
		}

		return M4_SUCCESS ;
	}



m4return_t ClAppInfo::ReadConfigurationFile (m4char_t *  ai_block_name, m4char_t *  ai_property_name, m4char_t * &  ao_pcvalue, m4pchar_t ai_pcConfigutationFile)
{

	const m4pchar_t ini_default_value = "ini_default_value";
	const int buffer_size = APP_INFO_MAX_BUFFER;
	m4char_t Value[buffer_size];
	m4pchar_t pValue = Value;
	m4int_t iRet;

	iRet=GetPrivateProfileString(ai_block_name, ai_property_name, ini_default_value , ao_pcvalue, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

	if(iRet<=0)
	{
//		SETCODEF( 1030 , ERRORLOG , "Error en funcion GetPrivateProfileString, retorno %d",iRet );

		return M4_ERROR;
	}

	if ( ! stricmp ( ini_default_value , ao_pcvalue ) )
	{
//		SETCODEF( 1030 , ERRORLOG , "Error en funcion GetPrivateProfileString, Valor %s no encontrado",ai_property_name ) ;
		return M4_ERROR ;
	}

	m4char_t before[APP_INFO_MAX_BUFFER], variable[APP_INFO_MAX_BUFFER], after[APP_INFO_MAX_BUFFER];

	while (strchr(ao_pcvalue,'%'))
	{

		memset( before, '\0' , APP_INFO_MAX_BUFFER );

		memset( variable, '\0' , APP_INFO_MAX_BUFFER );

		memset( after, '\0' , APP_INFO_MAX_BUFFER );

		SplitParseVariable( ao_pcvalue , before, variable, after);
		//get the variable value.
		//buscamos el valor de la variable 1º en el entorno, despues en la sección y 
		//por último en el global
		/* Get the value of the environment variable. */
		pValue = getenv( variable );   

		//M4_SRV_DEBUG( "getenv(%0:s)=%1:s" , variable << (pValue ? pValue : "Not Found") )  ;

		if( pValue == NULL )
		{
			if ( M4_ERROR == GetValue ( variable , Value , APP_INFO_MAX_BUFFER
				, stricmp( ai_property_name, variable )!=0  ) )
			{
				M4_SRV_ERROR( 0, "Can not to expand the variable %0:s, its value contains the undefined variable %1:s",
					ai_property_name << variable )  ;
				return M4_ERROR ;
			}
			/*
			//buscamos en la seccion server
			iRet = GetPrivateProfileString(
				"server", variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

			if(!strcmp(Value,"ini_default_value"))
			{
				//buscamos en la seccion 
				iRet = GetPrivateProfileString(
					ai_block_name, variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

				if(!strcmp(Value,"ini_default_value"))
				{
					// buscamos en la global
					iRet = GetPrivateProfileString(
						"global", variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

					if(!strcmp(Value,"ini_default_value"))
					{
						iRet = GetPrivateProfileString(
						"environment", variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

						if(!strcmp(Value,"ini_default_value"))
							return M4_ERROR;

					}

				}

			}
			*/
		}
		else strcpy( Value, pValue);

		// set the new output string.
		sprintf(ao_pcvalue, "%s%s%s",before, Value, after);
	}

	return M4_SUCCESS;


}

m4return_t ClAppInfo::ReadSectionConfigurationFile (m4pcchar_t  ai_block_name, m4char_t * &  ao_pcvalue, m4uint32_t ai_iBufferLen , m4pchar_t ai_pcConfigutationFile, m4uint32_t & ao_iLen )
{

	const m4pchar_t ini_default_value = "ini_default_value";
	const int buffer_size = APP_INFO_MAX_BUFFER;
	m4char_t Value[buffer_size];
	m4pchar_t pValue = Value;
	m4int_t iRet;
	


	iRet = GetPrivateProfileSection(ai_block_name, ao_pcvalue, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile );

	if(iRet<=0)
	{

		return M4_ERROR;
	}

	ao_iLen = iRet ;	

	return M4_SUCCESS;
}

	m4return_t ClAppInfo::GetSection(const char* ai_section, char * ao_value[APP_INFO_MAX_BUFFER], int& ao_bufferSize)
	{
		m4uint32_t iLenString = 0, iLen = 0, bufflen = 0;
		m4pchar_t pCnfFile = NULL;
		m4pchar_t pStart = NULL;
		m4pchar_t pcvalue = new m4char_t [APP_INFO_MAX_BUFFER] ;

		pCnfFile  = m_InputOptions -> m_configFile.value() ;
		if ( M4_ERROR == ReadSectionConfigurationFile (ai_section,pcvalue, APP_INFO_MAX_BUFFER , pCnfFile , iLenString) )
		{
			return M4_ERROR ;
		}

		ao_bufferSize = 0 ;
		while ( iLen < iLenString )
		{

			pStart = pcvalue + iLen ;

			iLen += strlen ( pStart ) ;

			bufflen = APP_INFO_MAX_BUFFER ;

			if ( strlen (pStart) +1 > bufflen )
				bufflen = strlen (pStart) +1 ;

			ao_value [ao_bufferSize] = new m4char_t [ bufflen ];

			strcpy ( ao_value [ao_bufferSize ] , pStart ) ;

			Parse(ao_value [ao_bufferSize ++] , bufflen ) ;

			iLen ++ ;
		}
		
		return M4_SUCCESS ;
	}
