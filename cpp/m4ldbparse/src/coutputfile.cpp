//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              COutputFile.cpp
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

#include "coutputfile.hpp"

COutputFile::COutputFile(const string &path) :
	CBasicFile(path)
{
}

ostream& COutputFile::onCreateFile(ostream &os) {
	return os << "Host\tServer\tDate\tStart Tick\tTime Tick\tEnd Tick\tUser\tConnection\tThread ID\tM4Object\tNode\tNum Records\tTime\tNum Sentences\tCommit/Rollback\tSentence" << endl;
}
