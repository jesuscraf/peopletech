//
// clappinfo.hpp
//

#ifndef _clappinfo_hpp_
#define _clappinfo_hpp_


#include <stdio.h>
#include "m4stl.hpp"
#include <cmdlnparser.hpp>
#include <cmdlnlauncher.hpp>
#include <xercesc/parsers/SAXParser.hpp>





const int APP_INFO_MAX_BUFFER=4096;
#define GLOBAL "global"
#define ENVIRONMENT "environment"


#define passwordstr "password"
#define userstr "user"
#define namestr "name"


typedef map<string,string,less<string> > ParametersMap ;



class ClAppInfo
{
public:
    //-----------------------------------------------------------------------
    // Method name: Instance
    // Description: return the unique instance of this class.
    // Parameters:	Filename where data are readed.
    // Return:		None.
    //-----------------------------------------------------------------------
	static ClAppInfo* Instance(cCmdlnLauncher* ai_InputOptions=NULL);

    //-----------------------------------------------------------------------
    // Method name: GetValue
    // Description: return the value for the input key.
    // Return:		0 on success
	//				!0 on error.
    //-----------------------------------------------------------------------
	m4return_t GetValue( char* ai_key, char* ao_value , m4uint32_t ai_iBufferSize, 
		m4bool_t SwAllowConfigFilesFind=M4_TRUE );

    //-----------------------------------------------------------------------
    // Method name: GetSection
    // Description: return all the keys for the input section.
    // Return:		0 on success
	//				!0 on error.
    //-----------------------------------------------------------------------
	m4return_t GetSection(const char* ai_section, char *ao_value[APP_INFO_MAX_BUFFER], int& ao_bufferSize);

	m4return_t Init() ;



	m4return_t SetValue ( m4pcchar_t ai_key, m4pchar_t ai_value ) ;
	 //-----------------------------------------------------------------------
    // Method name: Parse
    // Description: replace all values between %%.
    // Return:		0 on success
	//				!0 on error.
    //-----------------------------------------------------------------------
	m4return_t Parse(char* aio_value, m4uint32_t ai_BufferSize);

private:

	ParametersMap m_oParamMap ;	
	//-----------------------------------------------------------------------
    // Method name: SplitParseVariable
    // Description: Split the input string in 3 string:
	//		a string before the %% variable.
	//		the %% variable.
	//		a string after the %% variable.
    // Return:		0 on success
	//				!0 on error.
    //-----------------------------------------------------------------------
	m4return_t SplitParseVariable( char* aio_value, char* ao_before, char* ao_variable, char* ao_after);

    m4return_t ReadConfigurationFile (m4char_t *  ai_block_name, m4char_t *  ai_property_name, m4char_t * &  ao_pcvalue, m4pchar_t ai_pcConfigutationFile);

	m4return_t ReadSectionConfigurationFile (m4pcchar_t  ai_block_name, m4char_t * &  ao_pcvalue , m4uint32_t ai_iBufferLen , m4pchar_t ai_pcConfigutationFile , m4uint32_t & ao_iLen ) ;

	ClAppInfo(cCmdlnLauncher* ai_InputOptions);
	static ClAppInfo* m_instance;


	cCmdlnLauncher* m_InputOptions ;
};


#endif // _clappinfo_hpp_

