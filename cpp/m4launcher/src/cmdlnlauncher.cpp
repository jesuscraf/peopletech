//
// cmdlnlauncher.cpp
//

#include "cmdlnlauncher.hpp"

#include "m4srvtrace.h"
#include "basiclog.hpp"

#define M4_SRV_TRACE_COMP_ID "LNCHR"

//-----------------------------------------------------------------------
// Method name: Instance
//-----------------------------------------------------------------------
cCmdlnLauncher* cCmdlnLauncher::Instance(int argc, char** argv)
{
	if (NULL != m_instance)
	{
		return (cCmdlnLauncher*)m_instance;
	}
	
	m_instance = new cCmdlnLauncher();
	if (NULL != m_instance)
	{
		if (0 != m_instance->Parser(argc, argv))
		{
			delete m_instance;
			m_instance = NULL;
		}
	}
	return (cCmdlnLauncher*)m_instance;
}


//-----------------------------------------------------------------------
// Method name: OnError
//-----------------------------------------------------------------------
//virtual int OnError();


//-----------------------------------------------------------------------
// Method name: Usage
//-----------------------------------------------------------------------
//virtual void Usage(const char *ai_programName);
