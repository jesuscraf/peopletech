//
// m4launcher.cpp
//
#include "m4stl.hpp"
#include "m4trace.hpp"
#include "cmdlnlauncher.hpp"
#include "clm4applauncher.hpp"
#include <m4date.hpp>
#include "m4srvtrace.h"
#include "m4memdbg.h"
#include "basiclog.hpp"

#ifdef _UNIX
#include "m4ini.hpp"
#endif


#define M4_SRV_TRACE_COMP_ID "LNCHR"

// prototypes.
void WelcomeMessage();

int main(int argc, char **argv)
{
	M4SrvConfigDevelopEnvByEnvVble( M4_SRV_TRACE_COMP_ID) ;

	// print welcome message.
	WelcomeMessage();

#ifdef _UNIX
	SetCaseSensitive (M4_TRUE) ;
#endif

	m4sleep (30);
	cCmdlnLauncher* InputOptions = cCmdlnLauncher::Instance(argc, argv);
	if (NULL == InputOptions)
	{
		M4_SRV_ERROR_N( 0, "m4launcher parse error." );
		return -1;
	}

    //
    // these are the values for each option after the parse.

	//M4_SRV_DEBUG_BLOCK_N( Blk, "User options: ");

    //M4_SRV_DEBUG( "AppServer configuration file: %0:s ", InputOptions->m_configFile.value() );
	//M4_SRV_DEBUG( "AppServer process name: %0:s", InputOptions->m_name.value() );
	//M4_SRV_DEBUG( "M4AppCtl Control Port: %0:s", InputOptions->m_port.value() );
    //M4_SRV_DEBUG( "AppServer to M4Appctl connection time: %0:s", InputOptions->m_time.value() );
    //M4_SRV_DEBUG( "AppServer command: %0:s", InputOptions->m_command.value() );
	//M4_SRV_DEBUG( "Run mode: %0:s", InputOptions->m_mode.value() );
	//M4_SRV_DEBUG( "User: %0:s", InputOptions->m_user.value() );
	//M4_SRV_DEBUG( "Password: %0:s", InputOptions->m_password.value() );
	//Blk.End() ;

	ClM4appLauncher myProcess(InputOptions, "server");

	if (myProcess.Run())
	{
		M4_SRV_ERROR( 0, "Can't exec  %0:s", InputOptions->m_configFile.value()  )  ;
		return M4_ERROR;
	}

    return 0;
}



void WelcomeMessage()
{
	M4_SRV_GENERAL_N( "m4launcher..." );
}
