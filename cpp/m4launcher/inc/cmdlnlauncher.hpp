//
// cmdlnlauncher.cpp
//

#ifndef _cmdlnlauncher_hpp_
#define _cmdlnlauncher_hpp_

#include "cmdlnparser.hpp"


//
// my command line parser class
class cCmdlnLauncher: public cCommandLineParser
{
public:
	//
	// these are the options for my program.
	cOption<char *> m_xml;
	cOption<char *> m_configFile;
	//cOption<char *> m_name;
//	cOption<char *> m_command;
//	cOption<char *> m_port;
//	cOption<char *> m_time;
//	cOption<char *> m_mode;
//	cOption<char *> m_user;
//	cOption<char *> m_password;
//	cOption<char *> m_file;
//	cOption<char *> m_obl;




    //-----------------------------------------------------------------------
    // Method name: Instance
    // Description: return the unique instance of this class.
    // Parameters:	The input arguments to main function.
    // Return:		None.
    //-----------------------------------------------------------------------
	static cCmdlnLauncher* Instance(int argc=0, char** argv=NULL);


    //-----------------------------------------------------------------------
    // Method name: Usage
    // Description:	This method print a help program usage in the screen
    //		and exit. It's is called from the onError method.
	//		The user can be rewrite this method in this derived class.
    // Parameters:
    //		ai_programName	- The executable name of the program.
    // Return:		None.
    //-----------------------------------------------------------------------
    //virtual void Usage(const char *ai_programName);

private:
    //
    // In the cCommandLineParser constructor I added the flags to my program.
	// Two flags are possible:
	//	IgnoreUnknownOptions
	//	NeedArguments (arguments are any value that is not an option)
	//
    // In the myClass constructor only initialize the class members.
	// The parameters in the option constructor are:
	//	char name, T value, char *helpMessage, m4bool_t mandatory=M4_FALSE
	cCmdlnLauncher() : cCommandLineParser(),
		m_xml ( "xml" , " " , "xml file" , M4_TRUE ) ,
//		m_command	("command", " ","AppServer command (startup|shutdown)",M4_FALSE),
		m_configFile("config", " ","AppServer configuration file",M4_FALSE)
//		m_port	("port", " ","M4Appctl control port",M4_FALSE),
//		m_time	("time", " ","AppServer to M4Appctl connect time",M4_FALSE)
//	    m_mode		("mode", " ","Run mode (debug|release)",M4_FALSE),
//		m_user		("user", " ","User",M4_FALSE),
//		m_obl	("obl", " ","Obl",M4_FALSE),
//		m_password	("password", " ","Password",M4_FALSE),
//		m_file	("file", " ","Encrypted file",M4_FALSE)
	{}
};


#endif // _cmdlnlauncher_hpp_

