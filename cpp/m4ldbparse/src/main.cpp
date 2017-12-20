//	=============================================================================
//
//	 (c) Copyright  2006 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              Main.cpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				31/05/06
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//	 Definition:
//
//	 This program parses the content of the ldb inspector file
//	 producing a simplified output, showing one line per transaction
//	 that matches user-given criteria (number of register greater than 
//   a given amount or execution time greater than a given amount of seconds).
//   It can operate in cyclic mode, where it analyses the content of two files
//   waiting a given time between calls, in order to produce a consolidated output.
//
//	=============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <limits>

#include "cldbparser.hpp"
#include "cldbfiles.hpp"
#include "cldbfile.hpp"
#include "ctracefile.hpp"
#include "coutputfile.hpp"

using namespace std;

#ifdef _UNIX
#include <unistd.h>
#else
#include <windows.h>
#endif

// Program Parameters
int iMinTime = (std::numeric_limits<int>::max)();
int iMinSize = (std::numeric_limits<int>::max)();
string sInputFile;
string sAlternateFile;
string sOutputFile;
//string sPartialFile;
string sTraceFile;
string sExceptionFile;
string sIncludeFile;
string sCriticalFile;
vector<CLdbFiles*> vpLdbFiles;
set<string> setExclusions;
set<string> setIncludes;

int iCycle=0;
bool bCycle = true;

void usage();
int checkArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{

	// Check args
	if(checkArgs(argc, argv)!=0){
		usage();
		return 0;
	}


	// -------------------------------
	// Trace
	CTraceFile traceFile(sTraceFile);
	if (!traceFile.Open())
		cerr << "Error opening file " << sTraceFile;
	M4OUT(traceFile) << "Init LDBParse" << endl;
	// -------------------------------


	// -------------------------------
	// M4O exceptions and includes are incompatible
	if (!sExceptionFile.empty() && !sIncludeFile.empty()) {
		// Write to trace
		M4OUT(traceFile) << "ERROR: there cannot be exclusions and inclusions at the same time" << endl;

		// Exit program with message
		cout << "\nEXCEPTIONS AND INCLUSIONS ARE INCOMPATIBLE!" << endl;
		return 0;
	}
	// -------------------------------

	// -------------------------------
	// M4O exception file
	FILE	*m_fpExceptionIn;
	//string	sExceptions = "|";

	// Open M4O Exception file if provided.
	if (!sExceptionFile.empty())
	{
		m_fpExceptionIn = fopen(sExceptionFile.c_str(), "r");
		if (m_fpExceptionIn==NULL){
		}
		else{
			char sLinea[MAX_LENGTH_RECORD];
			fseek(m_fpExceptionIn, 0, SEEK_END);
			long maximo = ftell(m_fpExceptionIn);
			rewind(m_fpExceptionIn);

			while (maximo > ftell(m_fpExceptionIn)){
				fgets(sLinea, MAX_LENGTH_RECORD-1, m_fpExceptionIn);
				if (sLinea[strlen(sLinea) - 1] == '\n' || sLinea[strlen(sLinea)] ==  '\0')
				{
					string entry = (sLinea[strlen(sLinea) - 1] == '\n') ? ((string)sLinea).substr(0, strlen(sLinea) - 1) : sLinea;
					set<string>::iterator it;
					it = setExclusions.find(entry);
					if (it == setExclusions.end())
					{
						setExclusions.insert(entry);
					}
				}
			}
		}
	}
	// -------------------------------


	// -------------------------------
	// M4O include file
	FILE	*m_fpIncludeIn;
	//string	sIncludes = "|";

	// Open M4O Include file if provided.
	if (!sIncludeFile.empty())
	{
		m_fpIncludeIn = fopen(sIncludeFile.c_str(), "r");
		if (m_fpIncludeIn==NULL){
		}
		else{
			char sLinea[MAX_LENGTH_RECORD];
			fseek(m_fpIncludeIn, 0, SEEK_END);
			long maximo2 = ftell(m_fpIncludeIn);
			rewind(m_fpIncludeIn);

			while (maximo2 > ftell(m_fpIncludeIn)){
				fgets(sLinea, MAX_LENGTH_RECORD-1, m_fpIncludeIn);
				if (sLinea[strlen(sLinea) - 1] == '\n' || sLinea[strlen(sLinea)] == '\0')
				{
					string entry = (sLinea[strlen(sLinea) - 1] == '\n') ? ((string)sLinea).substr(0, strlen(sLinea) - 1) : sLinea;
					set<string>::iterator it;
					it = setIncludes.find(entry);
					if (it == setIncludes.end())
					{
						setIncludes.insert(entry);
					}
				}
			}
		}
	}
	// -------------------------------


	COutputFile outputFile(sOutputFile);
	//COutputFile partialFile(sPartialFile);
	COutputFile criticalFile(sCriticalFile);

	if (iCycle == 0)
		bCycle = false;

	do
	{
		if (!traceFile.Open())
			cerr << "Error opening file " << sTraceFile;
		if (bCycle == true) {
			M4OUT(traceFile) << "START cycle: " << endl;
		}
		else {
			M4OUT(traceFile) << "START single execution: " << endl;
		}
		for (unsigned int i = 0; i < vpLdbFiles.size(); i++) {
			//each vpLdbFiles item has an array of two CLdbFile objects. One of the array elements may be null
			for (unsigned int j = 0; j < 2; j++)
			{
				CLdbFile *file = vpLdbFiles[i]->GetFile(j);
				if (file == NULL || !file->HasChanged()) {
					if (file != NULL) {
						M4OUT(traceFile) << "SKIP file: " << *file << endl;
					}

					continue;
				}
				MapReg* pendingOperations = vpLdbFiles[i]->GetPendingOperations();
				// Analyse file
				CLdbParser *oParser = new CLdbParser(*file, outputFile, iMinSize, iMinTime, setExclusions, setIncludes, traceFile, criticalFile, *pendingOperations);
				oParser->setNumciclos(iCycle == 0 ? 0 : 1);

				// ---------------------------------------
				// If there is an exception parsing ldbinsp file, the program must remain in execution
				// The exception is traced
				try {
					file->SetTick(oParser->parse(file->GetTick()));
				}
				catch (...)
				{
					M4OUT(traceFile) << "ERROR parsing ldbinsp file " << *file << endl;
				}
				// ---------------------------------------

				delete oParser;
			}
				
		}

		if (bCycle == true) {
			M4OUT(traceFile) << "FINISH cycle: " << endl;
		}
		else {
			M4OUT(traceFile) << "FINISH single execution: " << endl;
		}
		// close files
		traceFile.Close();

		if (bCycle)
		{
			// Only sleep necessary time in order to reach next cycle execution time
			unsigned long long endTick = GetTickCount();
			unsigned long long nextCycle = GetTickCount() + iCycle * 1000;
			if (endTick < nextCycle) {
				// Wait specified number of seconds
#ifdef _UNIX
				sleep(((unsigned long)(nextCycle - endTick)) / 1000);
#else
				Sleep((unsigned long)(nextCycle - endTick));// In Windows Sleeps takes number of miliseconds
#endif
			}
		}

	} while (bCycle);

	return 0;

}


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
	printf( "This program analyses the content of the LDB inspector file (ldbinsp)\n"  );
    printf( "in order to detect particularly heavy transactions\n" );
    printf( "\nUsage:\n" );
    printf( "    LDBParse [options]\n\n" );
    printf( "Options:\n" );
    printf( "	-l=label		Label identifying server instance.\n" );
    printf( "	-f=file			Input file.\n" );
	printf(	"	-a=altfile		Alternate Input file (for ldbinsp swap).\n" );
    printf( "	-t=time			Minimum execution time for a transaction.\n" );
    printf( "	-s=size			Minimum number of register for a transaction.\n" );
    printf( "	-o=output		Output file.\n" );
	//printf( "	-p=partialfile	Output partial records to this file at the end of parsing.\n" );
	printf( "	-n=tracefile	Trace file name.\n" );
	printf( "	-x=critical		Critical trace file path.\n" );
	printf(	"	-c=cycle		Period (number of seconds) for cyclic execution.\n" );
	printf(	"	-e=exceptions	List of channels not to be included in parsing.\n" );
	printf(	"	-i=includes		List of channels to be included in parsing.\n" );
	printf( "	-?				Show this help.\n\n" );
	printf(	"	-f= and -o= are mandatory, -a= is mandatory if using -c=, -e= and -i= are incompatible\n" );
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int checkArgs(int argC, char* argV[])
{
	size_t iMainPos = 0;
	size_t iLabelPos = 0;
	size_t iAlternatePos = 0;

    int argInd;


	if (argC<3)
		return -1;

	for (argInd = 1; argInd < argC; argInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[argInd][0] != '-')
        {
            return 1;
        }

        // Watch for special case help request
        if (!strcmp(argV[argInd], "-?"))
        {
            return 1;
        }
		// input file
        else if (!strncmp(argV[argInd], "-f=", 3)
              ||  !strncmp(argV[argInd], "-F=", 3))
        {
            const char* const parm = &argV[argInd][3];
			if (strcmp(parm, "")!=0) {
				CLdbFiles *files;
				if (vpLdbFiles.size() == iMainPos) {
					files = new CLdbFiles;
					vpLdbFiles.push_back(files);
				} else {
					files = vpLdbFiles[iMainPos];
				}
				files->SetMain(parm);
				iMainPos++;
			}
        }
		// label
		else if (!strncmp(argV[argInd], "-l=", 3)
			  ||  !strncmp(argV[argInd], "-L=", 3))
		{
            const char* const parm = &argV[argInd][3];
			if (strcmp(parm, "")!=0) {
				CLdbFiles *files;
				if (vpLdbFiles.size() == iLabelPos) {
					files = new CLdbFiles;
					vpLdbFiles.push_back(files);
				} else {
					files = vpLdbFiles[iLabelPos];
				}
				files->SetLabel(parm);
				iLabelPos++;
			}
		}
		// alternate file
        else if (!strncmp(argV[argInd], "-a=", 3)
              ||  !strncmp(argV[argInd], "-A=", 3))
        {
            const char* const parm = &argV[argInd][3];
			if (strcmp(parm, "")!=0) {
				CLdbFiles *files;
				if (vpLdbFiles.size() == iAlternatePos) {
					files = new CLdbFiles;
					vpLdbFiles.push_back(files);
				} else {
					files = vpLdbFiles[iAlternatePos];
				}
				files->SetAlternate(parm);
				iAlternatePos++;
			}
        }

				// output file
        else if (!strncmp(argV[argInd], "-o=", 3)
              ||  !strncmp(argV[argInd], "-O=", 3))
        {
            const char* const parm = &argV[argInd][3];
			sOutputFile = parm;
        }
		//		// Partial records file name
		//else if (!strncmp(argV[argInd], "-p=", 3)
  //            ||  !strncmp(argV[argInd], "-P=", 3))
  //      {
  //          const char* const parm = &argV[argInd][3];
		//	sPartialFile = parm;
  //      }
				// Min Time
        else if (!strncmp(argV[argInd], "-t=", 3)
              ||  !strncmp(argV[argInd], "-T=", 3))
        {
            const char* const parm = &argV[argInd][3];
			iMinTime = atoi(parm);
        }
				// Min size
		else if (!strncmp(argV[argInd], "-s=", 3)
              ||  !strncmp(argV[argInd], "-S=", 3))
        {
            const char* const parm = &argV[argInd][3];
			iMinSize = atoi(parm);
        }
				// Cycle length
		else if (!strncmp(argV[argInd], "-c=", 3)
              ||  !strncmp(argV[argInd], "-C=", 3))
        {
            const char* const parm = &argV[argInd][3];
			iCycle = atoi(parm);
        }
				// M4O-nodes exceptions
		else if (!strncmp(argV[argInd], "-e=", 3)
              ||  !strncmp(argV[argInd], "-E=", 3))
        {
            const char* const parm = &argV[argInd][3];
			sExceptionFile = parm;
        }
				// M4O-nodes includes
		else if (!strncmp(argV[argInd], "-i=", 3)
              ||  !strncmp(argV[argInd], "-I=", 3))
        {
            const char* const parm = &argV[argInd][3];
			sIncludeFile = parm;
        }
				// Trace file name
		else if (!strncmp(argV[argInd], "-n=", 3)
              ||  !strncmp(argV[argInd], "-N=", 3))
        {
            const char* const parm = &argV[argInd][3];
			sTraceFile = parm;
        }
				// Critical trace file
		else if (!strncmp(argV[argInd], "-x=", 3)
              ||  !strncmp(argV[argInd], "-X=", 3))
        {
            const char* const parm = &argV[argInd][3];
			sCriticalFile = parm;
        }
    }
			// Check needed params
	if (vpLdbFiles.empty() || sOutputFile.empty())
		return 2;
	// Number of labels doesn't match number of input files
	else if ((iLabelPos > 0) && (iLabelPos != iMainPos))
		return 3;
	else if(iCycle!=0){
		// Number of alternate files doesn't match number of input files
		//if ((iAlternatePos != iMainPos) || !sPartialFile.empty())
		if (iAlternatePos != iMainPos)
			return 4;
		else
			return 0;
	}
	else

		return 0;
}
