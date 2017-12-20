//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLdbFiles.cpp
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

#include "cldbfiles.hpp"
#include "cldbfile.hpp"


CLdbFiles::CLdbFiles() {
	m_iCurrent = 0;
	m_sLabel = "";
	m_pLdb[0] = NULL;
	m_pLdb[1] = NULL;
	m_pMapPendingOperations = new MapReg();
}

CLdbFiles::~CLdbFiles() {
	if (m_pLdb[0] != NULL)
		delete m_pLdb[0];
	if (m_pLdb[1] != NULL)
		delete m_pLdb[1];
	m_pMapPendingOperations = NULL;
}

void CLdbFiles::SetMain(const string &path) {
	m_pLdb[0] = new CLdbFile(path, *this);
}

void CLdbFiles::SetAlternate(const string &path) {
	m_pLdb[1] = new CLdbFile(path, *this);
}

CLdbFile* CLdbFiles::Current() {
	if (m_pLdb[m_iCurrent]->HasChanged())
		return m_pLdb[m_iCurrent];
	if ((m_pLdb[1] != NULL) && m_pLdb[1-m_iCurrent]->HasChanged()) {
		m_iCurrent = 1-m_iCurrent;
		return m_pLdb[m_iCurrent];
	}
	return NULL;
}


CLdbFile* CLdbFiles::GetFile(int index) {
	return m_pLdb[index];
}

MapReg* CLdbFiles::GetPendingOperations() {
	return (m_pMapPendingOperations);
}

void CLdbFiles::SetLabel(const string &label) {
	m_sLabel = label;
}

const string& CLdbFiles::GetLabel() const {
	return m_sLabel;
}

ostream& operator<< (ostream &os, const CLdbFiles &files) {
	return os << files.m_sLabel;
}
