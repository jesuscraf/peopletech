//	=============================================================================
//
//	 (c) Copyright  2006 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLDBParser.cpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				31/05/06
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//	 Definition: this class represents the parser of the LDB file
//
//	 
//	=============================================================================

#ifndef _CLDBPARSER_HPP
#define _CLDBPARSER_HPP

//
#include <string>
#include <set>
#include "cldbparsedefine.hpp"
#include "cbasicfile.hpp"
#include "cldbfile.hpp"
#include "cldbfiles.hpp"

using namespace std;


class CLdbParser
{
public:
	CLdbParser(const CLdbFile&, CBasicFile&, int, int, set<string>&, set<string>&, CBasicFile&, CBasicFile&, MapReg&);
public:
	~CLdbParser();
private:
	int		m_iMinSize;
	int		m_iMinTime;
	int numciclos;
	unsigned long long m_lLastTick;
	string m_sHost;
	string m_sServer;
	int m_iCurrentConnection = -10;
	int m_iCurrentOperation = NO_OPERATION;
	string m_sCurrentUser;
	CLdbReg *m_oCurrentReg;
	set<string> m_setExclusions;
	set<string> m_setIncludes;

	// Input file
	const CLdbFile &m_inputFile;
	// Output file
	CBasicFile &m_outputFile;
	// Critical file
	CBasicFile &m_criticalFile;
	// Partial records files
	//CBasicFile &m_partialFile;
	// Trace file
	CBasicFile &m_traceFile;
	// Map of pending registers
	MapReg &m_oMap;
	void dumpTrace(unsigned long long &aio_lFromTick);

public: 
	unsigned long long parse(unsigned long long ai_lFromTick);
	
	void setNumciclos(int op);
};

#endif

