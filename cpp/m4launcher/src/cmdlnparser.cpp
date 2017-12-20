/****************************************************************************
* File:			clparser.cpp
* Author:       Marcos Cacabelos Prol
* Date:			97.08.16
* Description:	Source file for the classes cBaseOption, template cOption
*				and cCommandLineParser.
* Compilers:	DJGPP v2.0
* History:
*		97.08.16	Initial version.
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdlnparser.hpp"

#include "m4stl.hpp"
#include "m4srvtrace.h"
#include "basiclog.hpp"

#define M4_SRV_TRACE_COMP_ID "LNCHR"

#ifdef _UNIX
#include <ctype.h>
#include <unix/m4uxstr.hpp>

#endif

//===========================================================================
// CLASS        cCommandLineParser
//===========================================================================
//---------------------------------------------------------------------------
// Description: Static variables for cCommandLineParser class
//---------------------------------------------------------------------------
int cCommandLineParser::argc;
char cCommandLineParser::argv[MAX_OPTIONS][MAX_OPT_LENGH];
m4bool_t cCommandLineParser::m_ignoreUnknownOptions;
m4bool_t cCommandLineParser::m_needArguments;
int cCommandLineParser::m_optionsCounter;
cBaseOption *cCommandLineParser::m_options[MAX_OPTIONS];

cCommandLineParser* cCommandLineParser::m_instance=NULL;


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::cCommandLineParser
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
cCommandLineParser::cCommandLineParser(int flags)
{
	//
    // init flags members.
    if (flags & IgnoreUnknownOptions)
        m_ignoreUnknownOptions = M4_TRUE;
    else
        m_ignoreUnknownOptions = M4_FALSE;

    if (flags & NeedArguments)
        m_needArguments = M4_TRUE;
    else
        m_needArguments = M4_FALSE;

	//
    // init class members.
    m_optionsCounter = 0;
}


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::AddOption
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
void cCommandLineParser::AddOption (cBaseOption *arg)
{
    if ((arg != NULL) && (m_optionsCounter < MAX_OPTIONS))
		m_options[m_optionsCounter++] = arg;
}


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::Parser
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
int cCommandLineParser::Parser(int argc, char** argv)
{
    int argvCounter;

	// set program Name.
	char *programName = argv[0];
	char* slash = NULL;

	// find the last slash in the programName to delete the path.
	if ((slash = strrchr(programName, '/')) != NULL)
    	programName = slash+1;
    else if ((slash = strrchr(programName, '\\')) != NULL)
    	programName = slash+1;
	strcpy(m_programName, programName);

    //
    // do a loop over each input argument.
    for (argvCounter=1; argvCounter<argc; argvCounter++) {
    	char *userOpt = argv[argvCounter];
        if (userOpt[0] != '-') 
		{
		    //
		    // set class variables argc and argv.
            cCommandLineParser::argc = argc - argvCounter + 1;
            strcpy(cCommandLineParser::argv[0], argv[0]);
            for (int i=1; i<cCommandLineParser::argc; i++)
			{
            	strcpy(cCommandLineParser::argv[i], argv[argvCounter+i-1]);
			}
            strcpy(cCommandLineParser::argv[cCommandLineParser::argc], "\0");
	
			if (CheckMandatoryOptions())
			{
				return OnError();
			}
            return 0;
        }

        //
        // do a loop over each letter in this input argument.
        int argvPosition = 1;
        char * optName;
        while ((optName = &userOpt[argvPosition])[0] != '\0') 
		{
			argvPosition += strlen ( optName ) ;
        	cBaseOption *opt;
        	m4bool_t optFound = M4_FALSE;
	        //
    	    // search this option in the options array.
        	for (int i=0; (i<m_optionsCounter) && (!optFound); i++) {
        		opt = m_options[i];
            	if ( !stricmp ( optName , opt->name()) )
				{
            		optFound = M4_TRUE;
                }
            }
            if (!optFound) {
            	if (!m_ignoreUnknownOptions) 
				{
					return OnError();
				}
            }
            else {
            	//
                // if this option is mandatory, set to M4_FALSE,
                // because it was found.
                if (opt->isMandatory())
                	opt->setMandatory(M4_FALSE);
                //
                // set value for this option
                if (strcmp(opt->type(), "<m4bool_t>")) {
                	//
                	// this option needs a value.
                    if (userOpt[argvPosition] == '\0') {
                        //
                        // value is the next argument
                        argvCounter++;
                        if ((argv[argvCounter] == NULL) || (argv[argvCounter][0] == '-'))
						{
							return OnError();
						}
                        else {
                        	void *newValue = (void *)argv[argvCounter];
                        	opt->setValue(newValue);
                        }
					}
                    else {
                    	//
                    	// value is in this argument.
                        void *newValue = (void *)&(userOpt[argvPosition]);
                        opt->setValue(newValue);
                    }
                    //
                    // exit from while loop
                    break;
                }
                else {
                    //
                    // this option is a flag (m4bool_t) option
                	void *newValue=(void *)M4_TRUE;
                	opt->setValue(newValue);
                }
            }
        }
    }

	if (CheckMandatoryOptions())
	{
		return OnError();
	}

    //
    // if flag m_needArguments is M4_TRUE, check that arguments exist.
    if (m_needArguments) 
	{
    	if (argvCounter == argc)
 		{
			return OnError();
		}
	}

    //
    // set class variables argc and argv.
    cCommandLineParser::argc = 1;
    strcpy(cCommandLineParser::argv[0], argv[0]);
    strcpy(cCommandLineParser::argv[cCommandLineParser::argc], "\0");

	return 0;
}


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::CheckMandatoryOptions
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
int cCommandLineParser::CheckMandatoryOptions()
{
    //
    // check if any mandatory option was not found.
    cBaseOption *opt;
    for (int i=0; i<m_optionsCounter; i++) {
    	opt = m_options[i];
        if (opt->checkMandatory())
		{
			return 1;
		}
    }
	return 0;
}


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::OnError
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
int cCommandLineParser::OnError()
{
	Usage(m_programName);
	return -1;
}


//---------------------------------------------------------------------------
// Function/Method name: cCommandLineParser::Usage
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
void cCommandLineParser::Usage(const char *ai_programName)
{
    cBaseOption *opt;
	int i;

    //
    // print syntax
	M4_SRV_GENERAL_BLOCK( Blk, "Usage: %0:s" , ai_programName ) ;
    for (i=0; i<m_optionsCounter; i++) {
    	opt = m_options[i];
        M4_SRV_GENERAL( "%0:s ", opt->syntax() ) ;
    }
    if (m_needArguments) {
    	M4_SRV_GENERAL_N( "<arguments>" ) ;
    } else {
    	M4_SRV_GENERAL_N( "" ) ;
	} ;

    //
    // print help
    for (i=0; i<m_optionsCounter; i++) {
    	opt = m_options[i];
        M4_SRV_GENERAL(  "    %0:s", opt->helpMessage() ) ;
    }
}






//===========================================================================
// CLASS		cBaseOption
//===========================================================================
//---------------------------------------------------------------------------
// Function/Method name:
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
char *cBaseOption::helpMessage()
{
	static char message[128];

    sprintf (message, "-%-15s    %s", m_name, m_helpMessage);
    return message;
}


//===========================================================================
// CLASS		cOption
//===========================================================================
//---------------------------------------------------------------------------
// Function/Method name: cOption<T>::type
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
template<> 
char *cOption<char *>::type()
{
	return ("<char *>");
}

template<> 
char *cOption<m4bool_t>::type()
{
	return ("<m4bool_t>");
}

template<> 
char *cOption<int>::type()
{
	return ("<int>");
}

template<> 
char *cOption<double>::type()
{
	return ("<double>");
}


//---------------------------------------------------------------------------
// Function/Method name: cOption<T>::setValue
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
template<> 
void cOption<char *>::setValue(void *newValue)
{
	m_value = (char *)newValue;
}

template<> 
void cOption<m4bool_t>::setValue(void *newValue)
{
	m_value = (m4bool_t)newValue;
}

template<> 
void cOption<int>::setValue(void *newValue)
{
	m_value = atoi((char *)newValue);
}

template<> 
void cOption<double>::setValue(void *newValue)
{
	m_value = atof((char *)newValue);
}

//---------------------------------------------------------------------------
// Function/Method name: cOption<T>::getValue
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------



template<> 
void cOption<char *>::getValue(void * & value)
{
	value = m_value;
}

//---------------------------------------------------------------------------
// Function/Method name: cOption<T>::syntax
// Description:	See cmdlnparser.hpp file for additional information
//---------------------------------------------------------------------------
template<> 
char *cOption<char *>::syntax()
{
    if (isMandatory())
    	sprintf (m_syntax, "-%s <string>", name());
    else
    	sprintf (m_syntax, "[-%s <string>]", name());

    return m_syntax;
}

template<> 
char *cOption<m4bool_t>::syntax()
{
    if (isMandatory())
    	sprintf (m_syntax, "-%s", m_name);
    else
    	sprintf (m_syntax, "[-%s]", m_name);

    return m_syntax;
}

template<> 
char *cOption<int>::syntax()
{
    if (isMandatory())
    	sprintf (m_syntax, "-%s <number>", m_name);
    else
    	sprintf (m_syntax, "[-%s <number>]", m_name);

    return m_syntax;
}

template<> 
char *cOption<double>::syntax()
{
    if (isMandatory())
    	sprintf (m_syntax, "-%s <number>", m_name);
    else
    	sprintf (m_syntax, "[-%s <number>]", m_name);

    return m_syntax;
}

