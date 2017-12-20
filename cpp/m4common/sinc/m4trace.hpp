//==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 File:              m4trace.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//	    This module defines trace macros to display information on
//		standard output only in debug mode.
//
//		The user can define the trace level setting the environment
//		variable M4_TRACE_LEVEL to an integer value. The default trace
//		level is 1. If trace level is set to 0 none message is sent to
//		the output.
//
//	 Samples:
//		m4Trace(cout << "my message.." << endl);
//			sends "my message.." to standard output if M4_TRACE_LEVEL>=1
//		m4TraceLevel(5, cout << "my message.." << endl);
//			sends "my message.." to standard output if M4_TRACE_LEVEL>=5
//
//		m4TraceHeader(cout << "my message.." << endl);
//			sends "[#1, main.cpp(31)]: my message.." to standard output if M4_TRACE_LEVEL>=1
//		m4TraceHeaderLevel(5, cout << "my message.." << endl);
//			sends "[#5, main.cpp(31)]: my message.." to standard output if M4_TRACE_LEVEL>=1
//
//		if use the next trace as first line in a function you get traces
//		when in an out in the function.
//		m4AutoTrace(cout << "myfunction.." << endl);
//			sends ">>in: myfunction.." to standard output if M4_TRACE_LEVEL>=1
//			sends "<<out: myfunction.." to standard output if M4_TRACE_LEVEL>=1
//		m4AutoTraceLevel(5, cout << "my message.." << endl);
//			sends ">>in: myfunction.." to standard output if M4_TRACE_LEVEL>=5
//			sends "<<out: myfunction.." to standard output if M4_TRACE_LEVEL>=5
//
//=============================================================================


#ifndef _M4TRACE_HPP_
#define _M4TRACE_HPP_

#include "m4types.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "m4stl.hpp"


#if defined(_M4_DEBUG) || defined(_DEBUG)
#	define ENABLE_M4_TRACE_MACROS
#endif



//===================================================================
// define trace macros when trace is enable.
//===================================================================
#ifdef ENABLE_M4_TRACE_MACROS

//-------------------------------------------------------------------
// Class:  M4TraceHandler
// description: set the default trace level reading the environment
//		variable M4_TRACE_LEVEL. If this variable is not defined the 
//		trace level is set to 1.
//-------------------------------------------------------------------
class M4TraceHandler
{
public:
	int m4TraceLevel;
	M4TraceHandler()
	{
		const char *environmentTraceLevel = getenv("M4_TRACE_LEVEL");
		m4TraceLevel = environmentTraceLevel == NULL  ? 1 : atoi(environmentTraceLevel);
	}
};


//-------------------------------------------------------------------
// declare a trace object for this compilation unit.
//-------------------------------------------------------------------
static M4TraceHandler _m4TraceObject;


//-------------------------------------------------------------------
// macroName: m4TraceEnabled
// description: check the trace level.
//-------------------------------------------------------------------
/* #define m4TraceEnabled(level) \
	((level) <= _m4TraceObject.m4TraceLevel)
*/
#define m4TraceEnabled(level) 	((level) <= _m4TraceObject.m4TraceLevel)

//-------------------------------------------------------------------
// macroName: m4TraceHeader
// description: display a header information with the filename and
//		line number where the trace is written.
//-------------------------------------------------------------------
/* #define m4TraceHeaderStatement(level) \
	(cout << "[#" << level << ", " __FILE__ << "(" << __LINE__ << ")]: ")
*/
#define m4TraceHeaderStatement(level) (cout << "[#" << level << ", " __FILE__ << "(" << __LINE__ << ")]: ")

//-------------------------------------------------------------------
// macroName: m4NewLineTrace
// description: display a new line in the standard output.
//-------------------------------------------------------------------
/* #define m4NewLineTrace \
	(cout << endl)
*/
#define m4NewLineTrace 	(cout << endl)


//-------------------------------------------------------------------
// macroName: m4Trace (outputStatement)
// parameters:
//	- outputStatement: statement to be executed when this macro is
//		invoked. (e.g.: cout << "Press any key..." << endl );
// description: display the output produced by the given 
//		outputStatement.
//-------------------------------------------------------------------
/* #define m4Trace(outputStatement) \
{ \
	if (m4TraceEnabled(1)) \
	{ \
		(outputStatement); \
	} \
}
*/
 #define m4Trace(outputStatement) { if (m4TraceEnabled(1)) 	{ (outputStatement); }}



//-------------------------------------------------------------------
// macroName: m4TraceLevel (level, outputStatement)
// description: same as m4Trace, but the user specifies the level.
//-------------------------------------------------------------------
/* #define m4TraceLevel(level, outputStatement) \
{ \
	if (m4TraceEnabled(level)) \
	{ \
		(outputStatement); \
	} \
} */

#define m4TraceLevel(level, outputStatement) { 	if (m4TraceEnabled(level)) 	{ (outputStatement); } }


//-------------------------------------------------------------------
// macroName: m4TraceHeader (outputStatement)
// parameters:
//	- outputStatement: statement to be executed when this macro is
//		invoked. (e.g.: cout << "Press any key..." << endl );
// description: display the output produced by the given 
//		outputStatement, with a trace header.
//-------------------------------------------------------------------
/* #define m4TraceHeader(outputStatement) \
{ \
	if (m4TraceEnabled(1)) \
	{ \
		(m4TraceHeaderStatement(1)); \
		(outputStatement); \
	} \
}
*/

#define m4TraceHeader(outputStatement) { if (m4TraceEnabled(1)) { (m4TraceHeaderStatement(1)); (outputStatement); 	} }


//-------------------------------------------------------------------
// macroName: m4TraceHeaderLevel (level, outputStatement)
// description: same as m4TraceHeader, but the user specifies the level.
//-------------------------------------------------------------------
/* #define m4TraceHeaderLevel(level, outputStatement) \
{ \
	if (m4TraceEnabled(level)) \
	{ \
		(m4TraceHeaderStatement(level)); \
		(outputStatement); \
	} \
} 
*/

#define m4TraceHeaderLevel(level, outputStatement) { if (m4TraceEnabled(level)) { (m4TraceHeaderStatement(level)); 	(outputStatement); 	} } 



//-------------------------------------------------------------------
// Class:  M4AutoTrace
// description: this class is intended to write an message when the
//		object is created and destroyed. Normally is used creating
//		a M4AutoTrace object as first line in a function.
//-------------------------------------------------------------------
class M4AutoTrace
{
	char *m_message;
	int m_level;
public:
	M4AutoTrace(int ai_level, const char *ai_message) : m_level(ai_level)
	{
		if (m4TraceEnabled(m_level))
		{
			if (ai_message != NULL)
			{
				m_message = new char [strlen(ai_message)+1];
				if (m_message != NULL)
				{
					strcpy(m_message, ai_message);
					cout << ">>in: " << m_message << endl;
				}
			}
			else
			{
				m_message = NULL;
			}
		}
	}
	
	~M4AutoTrace()
	{
		if (m4TraceEnabled(m_level))
		{		
			if (m_message != NULL)
			{
				cout << "<<out: " << m_message << endl;
				delete m_message;
			}
		}
	}
};


//-------------------------------------------------------------------
// macroName: m4AutoTrace (message)
// description: display the message when in and out in a function.
// parameters:
//	- message: message to send stdout
//-------------------------------------------------------------------
/* #define m4AutoTrace(message) \
	M4AutoTrace _m4AutoTraceObject(1, message)
*/

#define m4AutoTrace(message) M4AutoTrace _m4AutoTraceObject(1, message)

//-------------------------------------------------------------------
// macroName: m4AutoTraceLevel (message, level)
// description: same as m4AutoTrace, but the user specifies the level.
//-------------------------------------------------------------------
/* #define m4AutoTraceLevel(level, message) \
	M4AutoTrace _m4AutoTraceObject ## __LINE__ (level, message)
*/
#define m4AutoTraceLevel(level, message) 	M4AutoTrace _m4AutoTraceObject ## __LINE__ (level, message)

//-------------------------------------------------------------------
// macroName: m4DebugExecute (command)
// description: execute the command only if m4TraceEnabled.
//-------------------------------------------------------------------

#define m4DebugExecute(outputStatement) { if (m4TraceEnabled(1)) 	{ outputStatement; }}

//===================================================================
// if not defines ENABLE_M4_TRACE_MACROS declare trace macros like 
// a NULL statement
//===================================================================
#else // ENABLE_M4_TRACE_MACROS

#define m4Trace(outputStatement)					;
#define m4TraceLevel(level, outputStatement)		;
#define m4TraceHeader(outputStatement)				;
#define m4TraceHeaderLevel(level, outputStatement)	;
#define m4AutoTrace(message)						;
#define m4AutoTraceLevel(level, message)			;
#define m4DebugExecute(outputStatement)				;

#endif // ENABLE_M4_TRACE_MACROS



#endif // _M4TRACE_HPP_
