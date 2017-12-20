//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              COutputFile.hpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				24/09/2014
//	 Language:          C++
//	 Operating System:  WINDOWS , LINUX (maybe others too)
//
//	 Definition: this class represents an output file for parser results
//
//
//	=============================================================================

#ifndef _COUTPUTFILE_HPP
#define _COUTPUTFILE_HPP

#include "cbasicfile.hpp"

using namespace std;

class COutputFile : public CBasicFile
{
	public:
		COutputFile(const string&);

	protected:
		// adds the header of file creation
		ostream& onCreateFile(ostream&);
};
#endif
