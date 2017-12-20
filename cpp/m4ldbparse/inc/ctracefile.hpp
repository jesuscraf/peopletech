//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CTraceFile.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				24/09/2014
//	 Language:          C++
//	 Operating System:  WINDOWS , LINUX (maybe others too)
//
//	 Definition: this class represents a trace file for logging parser events
//
//
//	=============================================================================

#ifndef _CTRACEFILE_HPP
#define _CTRACEFILE_HPP

#include "cbasicfile.hpp"

#ifdef _UNIX
unsigned long long GetTickCount();
#endif

using namespace std;

class CTraceFile : public CBasicFile
{
	public:
		CTraceFile(const string&);

	protected:
		// prefix each line with the tick and the date
		ostream& prepareOutput(ostream&);

		// adds the header of file creation
		ostream& onCreateFile(ostream&);
};
#endif
