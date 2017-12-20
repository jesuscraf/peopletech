//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLdbFiles.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				24/09/2014
//	 Language:          C++
//	 Operating System:  WINDOWS , LINUX (maybe others too)
//
//	 Definition: this class represents a couple of input LDBInsp files.
//
//
//	=============================================================================

#ifndef _CLDBFILES_HPP
#define _CLDBFILES_HPP

#include <string>
#include <ostream>
#include <map>
#include "cldbreg.hpp"

using namespace std;

// forward declaration to avoid checken-and-egg problem
class CLdbFile;

typedef map< int, CLdbReg*, less<int> > MapReg;

class CLdbFiles
{
public:
	CLdbFiles();
	~CLdbFiles();
private:
	CLdbFile *m_pLdb[2];
	int m_iCurrent;
	string m_sLabel;
	//Map for pending operations not yet processed on connection change. Must be preserved between file swap and cycles to avoid data loss
	MapReg *m_pMapPendingOperations;

public:
	// define the first input file
	void SetMain(const string&);

	// define the second intput file
	void SetAlternate(const string&);

	// returns the input file from which to read during the next cycle
	// if the file from the previous cycle has changed, it is returned
	// else if the other file has changed, it is returned,
	// else returns NULL
	CLdbFile* Current();
	
	//retuns the file specidied by the index
	CLdbFile* GetFile(int index);

	//retuns the map for pending operations
	MapReg* GetPendingOperations();

		// define the label associated with this group of files
	void SetLabel(const string&);

	// returns the label
	const string& GetLabel() const;

	// allow access to internal fields to the stream operator
	friend ostream& operator<< (ostream&, const CLdbFiles&);
};

// overrides the standard stream operator
// simply outputs the label
ostream& operator<< (ostream&, const CLdbFiles&);

#endif

