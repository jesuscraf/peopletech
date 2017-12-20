//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLdbFile.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				24/09/2014
//	 Language:          C++
//	 Operating System:  WINDOWS , LINUX (maybe others too)
//
//	 Definition: this class represents an input LDBInsp file.
//
//
//	=============================================================================

#ifndef _CLDBFILE_HPP
#define _CLDBFILE_HPP

#include <string>
#include <ostream>

using namespace std;

// forward declaration to avoid checken-and-egg problem
class CLdbFiles;

class CLdbFile
{
public:
	CLdbFile(const string &, const CLdbFiles &);
	~CLdbFile();
private:
	string m_sPath;
	string m_sFilename;
	const CLdbFiles &m_group;
	long m_lTimeStamp;
	long m_lSize;
	unsigned long long m_lTick;

public:
	// returns whether this file has changed since last time it was loaded
	bool HasChanged();

	// stores the last tick read from the file
	void SetTick(unsigned long long);

	// loads the content of the file into a newly allocated buffer and returns its length
	long Load(char **) const;

	// returns the previously stored tick
	unsigned long long GetTick() const;

	// returns the value to use as server name
	// if a label has been defined for the containing CLdbFiles object, it is returned,
	// else it returns the path
	const string& GetServer() const;

	// allow access to internal fields to the stream operator
	friend ostream& operator<< (ostream&, const CLdbFile&);
};

// overrides the standard stream operator
// if a label has been defined for the containing CLdbFiles object, it outputs LABEL\path,
// else it ouputs the path
ostream& operator<< (ostream&, const CLdbFile&);
#endif
