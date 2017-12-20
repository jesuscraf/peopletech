
#include "wores.hpp"
#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include "logcomobject.h"
#include "m4unicode.hpp"


/*
 * ComLogTrace - Sends COM metric traces to various debug devices.
 */
void ComLogTrace(const char* format, ...)
{
#ifndef _M4_COM_METRICS
	(void)format;
#else
	va_list marker;
	va_start(marker, format);
	char buf[2048];
	_vsnprintf(buf, sizeof(buf)-1, format, marker);
	va_end(marker);

	// Send the output to the debug device
	//ATLTRACE("%s", buf);

	static FILE* gLogFile = NULL;

	if (gLogFile == NULL)
	{
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		gLogFile = M4Fopen("c:\\temp\\dcom.log", M4UniWrite, eEncoding);

		if (gLogFile == NULL)
		{
			SETCODE(M4_CH_WO_COM_METRICS, WARNINGLOG, "Could not open 'c:\\temp\\dcom.log'");
		}
	}
	// Send the output to the m4 trace file
	if (gLogFile)
	{
		fprintf(gLogFile, "%s", buf);
		fflush(gLogFile);
	}
#endif
}

