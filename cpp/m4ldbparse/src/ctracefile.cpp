//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CTraceFile.cpp
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

#include "ctracefile.hpp"

#include <ctime>
#ifdef _UNIX
#include <unistd.h>
unsigned long long GetTickCount() {
	struct timespec ts;
	return (clock_gettime(CLOCK_MONOTONIC,&ts) != 0) ? 0 : ts.tv_sec*1000+ts.tv_nsec/1000000;
}
#else
#include <windows.h>
#endif

CTraceFile::CTraceFile(const string &path)
	: CBasicFile(path)
{
}

ostream& CTraceFile::onCreateFile(ostream &os) {
	return os << "Tick\tLocal Time\tLog" << endl;
}

ostream& CTraceFile::prepareOutput(ostream &os) {
	time_t tnow = time(0);
	char ltbuf[200];
	strftime(ltbuf, 200, "%d-%m-%Y %H:%M:%S", localtime(&tnow));
	return os << GetTickCount() << '\t' << ltbuf << '\t';
}
