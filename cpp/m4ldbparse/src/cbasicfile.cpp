//	=============================================================================
//
//	 (c) Copyright  2014 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CBasicFile.cpp
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

#include "cbasicfile.hpp"

CBasicFile::CBasicFile(const string &path) :
	m_sPath(path),
	m_pOut(NULL)
{
}

bool CBasicFile::Open() {

	// Already opened
	if (m_pOut != NULL)
		return true;

	// File is disabled
	if (m_sPath == "")
		return true;

	// First try to open for reading to check if the file already exists
	bool exists = false;
	m_pOut = new fstream(m_sPath.c_str(), ios_base::in);
	if (m_pOut->good()) {
		exists = true;
	}
	m_pOut->close();

	// Now try to open for writing
	m_pOut->open(m_sPath.c_str(), ios_base::out | ios_base::app);
	if (m_pOut->fail()) {
		delete m_pOut;
		m_pOut = NULL;
		return false;
	}

	// If this is a new file, prepare it
	if (!exists)
		onCreateFile(*m_pOut);

	return true;
}

bool CBasicFile::IsEnabled() const {
	return !m_sPath.empty();
}

bool CBasicFile::IsValid() const {
	return (m_pOut != NULL) && m_pOut->good();
}

CBasicFile::~CBasicFile() {
	Close();
}

void CBasicFile::Close() {
	if (m_pOut != NULL) {
		m_pOut->close();
		delete m_pOut;
		m_pOut = NULL;
	}
}

ostream& CBasicFile::Out() {
	return prepareOutput(*m_pOut);
}

ostream& operator<< (ostream &os, const CBasicFile &file) {
	return os << (file.m_sPath.empty() ? "<disabled>" : file.m_sPath);
}

ostream& CBasicFile::onCreateFile(ostream &os) {
	return os;
}

ostream& CBasicFile::prepareOutput(ostream &os) {
	return os;
}

CBasicFile gNullFile("");
