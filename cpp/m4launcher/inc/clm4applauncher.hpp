//
// clm4applauncher.hpp
//

#ifndef _clm4applauncher_hpp_
#define _clm4applauncher_hpp_

#include "cmdlnparser.hpp"
#include <m4types.hpp>
#include <cmdlnlauncher.hpp>

const int ENVIRONMENT_MAX_SIZE=64;
const int ENVIRONMENT_VALUE_MAX_SIZE=256;
const int ARGV_VALUE_MAX_SIZE=2048;

const m4uint32_t MAX_PARAM_LEN = 1000 ;


class ClM4appLauncher
{
private:
	char *m_name;
	char *m_workingDirectory;
	char *m_inputArguments;
	char m_cEnvPath[65536];

	cCmdlnLauncher* m_InputOptions ;
//	char m_processName[ENVIRONMENT_VALUE_MAX_SIZE];

	char* m_argv[ENVIRONMENT_MAX_SIZE];
	//char m_argv[ENVIRONMENT_MAX_SIZE][ENVIRONMENT_VALUE_MAX_SIZE];
//	char m_environment[ENVIRONMENT_MAX_SIZE][ENVIRONMENT_VALUE_MAX_SIZE];
//	int m_environmentSize;

public:
	ClM4appLauncher(cCmdlnLauncher* InputOptions, const char* ai_processName);
	~ClM4appLauncher();
	int Run();

	m4return_t GetError () 
	{return iError;	}
private:
	int SetName();
	int SetWorkingDirectory();
	int SetArgv();
	int SetEnvironment();

	int BuildArgv();

	m4return_t iError ;
};


#endif // _clm4applauncher_hpp_

