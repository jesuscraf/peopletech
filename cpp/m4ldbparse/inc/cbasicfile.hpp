//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CBasicFile.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				24/09/2014
//	 Language:          C++
//	 Operating System:  WINDOWS , LINUX (maybe others too)
//
//	 Definition: this class represents a generic output file
//
//
//	=============================================================================

#ifndef _CBASICFILE_HPP
#define _CBASICFILE_HPP

#include <string>
#include <fstream>

using namespace std;

class CBasicFile
{
	public:
		CBasicFile(const string&);
		~CBasicFile();

		// opens the file for writing, creating it if it does not exist yet
		// returns true on success, false otherwise
		bool Open();

		// returns true if the path is non empty, false otherwise
		bool IsEnabled() const;

		// returns true if the path is non empty and the file was successfully opened
		bool IsValid() const;

		// close the file
		void Close();

		// returns on ouput stream for the file
		ostream& Out();

	protected:
		// this is called by Out() before returning the stream
		// override this to add preprocessing on each Out() invocation
		// for example to add a prefix to each line
		virtual ostream& prepareOutput(ostream&);

		// this is called by Open(), after creating the file if it didn't exist
		// override this to add preprocessing on creation
		// for example to add a header to the file
		virtual ostream& onCreateFile(ostream&);

	private:
		string m_sPath;
		fstream *m_pOut;

	// allow access to internal fields to the stream operator
	friend ostream& operator<< (ostream&, const CBasicFile&);
};

// overrides the standard stream operator
// simply outputs the file path
ostream& operator<< (ostream&, const CBasicFile&);

// use this macro to get the output stream and avoid overhead in case of a disabled file
#define M4OUT(file) if (!file.IsValid()) {} else file.Out()

// global null sink
extern CBasicFile gNullFile;

#endif
