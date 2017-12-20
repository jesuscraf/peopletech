//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLdbFile.cpp
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


#include "cldbfile.hpp"
#include "cldbfiles.hpp"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#if(defined(CYGWIN_BUILD)||defined(LINUX_BUILD))
#include <unistd.h>
#include <limits.h>
#define _fullpath(a,b,c) realpath(b,a)
#else
#include <windows.h>
#endif

CLdbFile::CLdbFile(const string &path, const CLdbFiles &group) :
	m_group(group),
	m_lTimeStamp(0),
	m_lSize(0),
	m_lTick(0)
{
	m_sPath = path;
	size_t found = path.find_last_of("/\\");
	if (found == string::npos) {
		m_sFilename = path;
		char *tmp = _fullpath(NULL, path.c_str(), MAX_PATH);
		if (tmp != NULL) {
			m_sPath = tmp;
			free(tmp);
		}
	} else {
		m_sFilename = path.substr(found+1);
	}
}

CLdbFile::~CLdbFile() {
}

bool CLdbFile::HasChanged() {
	// Open file to force update of stat information
	FILE *fp = fopen(m_sPath.c_str(), "r");
	if (fp == NULL)
		return false;
	fclose(fp);

	struct stat stat_p;
	if ( -1 ==  stat(m_sPath.c_str(), &stat_p))
		return false;
	long lTimeStamp = (long) stat_p.st_mtime;
	// Check st_size in addition to st_mtime because the latter is not always up-to-date
	long lSize = (long) stat_p.st_size;
	if ((m_lTimeStamp == lTimeStamp) && (m_lSize == lSize))
		return false;
	m_lTimeStamp = lTimeStamp;
	m_lSize = lSize;
	return true;
}

void CLdbFile::SetTick(unsigned long long lTick) {
	m_lTick = lTick;
}

long CLdbFile::Load(char **buffer) const {

	FILE *fp = fopen(m_sPath.c_str(), "rb");
	if (fp == NULL)
		return -1;

	fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
	rewind(fp);
	*buffer = (char*) malloc(sizeof(char)*(size+1));
	fread(*buffer, 1, size, fp);
	fclose(fp);
	(*buffer)[size]='\0';
	return size;
}

unsigned long long CLdbFile::GetTick() const {
	return m_lTick;
}

const string& CLdbFile::GetServer() const {
	if (m_group.GetLabel().empty())
		return m_sPath;
	return m_group.GetLabel();
}

ostream& operator<< (ostream &os, const CLdbFile &file) {
	if (file.m_group.GetLabel().empty())
		return os << file.m_sPath;
	return os << file.m_group << '\\' << file.m_sFilename;
}
