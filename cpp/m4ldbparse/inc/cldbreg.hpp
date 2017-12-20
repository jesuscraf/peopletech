//	=============================================================================
//
//	 (c) Copyright  2006 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLDBReg.cpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				31/05/06
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//	 Definition: this class represents a transaction register.
//
//
//	=============================================================================
#ifndef _CLDBREG_HPP
#define _CLDBREG_HPP

#include <string>
#include <ostream>
#include <queue>
#include "cldbparsedefine.hpp"

using namespace std;

class CLdbReg
{
public:
	CLdbReg();
public:
	~CLdbReg();
private:
	string m_pcHost;
	string m_pcServer;
	string m_pcUser;
	int m_iConnection;
	string m_pcM4Obj;
	string m_pcNode;
	string m_pcMainSentence;
	int m_iTime;
	int m_iNumRecords;
	string m_pcThread;
	unsigned long long m_lStartTick;
	unsigned long long m_lTimeTick;
	unsigned long long m_lEndTick;
	string m_pcDate;
	int m_iNumSentences;
	int m_iCurrentOperation;
	string m_pcEndTransaction;
public:
	void SetHost(string ai_pcHost);
	void SetServer(string ai_pcServer);
	void SetUser(string ai_pcUser);
	void SetConnection(int ai_iConnection);
	void SetM4Obj(string ai_pcM4Obj);
	void SetNode(string ai_pcNode);
	void SetMainSentence(string m_pcMainSentence);
	void SetTime(int ai_iTime);
	void SetNumRecords(int ai_iNumRecords);
	void SetThread(std::string ai_pcThread);
	void SetStartTick(unsigned long long ai_lTick);
	void SetTimeTick(unsigned long long ai_lTick);
	void SetEndTick(unsigned long long ai_lTick);
	void SetDate(string ai_pcDate);
	void SetNumSentences(int ai_iNumSentences);
	void SetCurrentOperation(int ai_iCurrentOperation);
	void SetEndTransaction(string ai_iEndTransaction);
	string GetHost();
	string GetServer();
	string GetUser();
	int GetConnection();
	string GetM4Obj();
	string GetNode();
	string GetMainSentence();
	int GetTime();
	int GetNumRecords();
	string GetThread();
	unsigned long long GetStartTick();
	unsigned long long GetTimeTick();
	unsigned long long GetEndTick();
	string GetDate();
	int GetNumSentences();
	int GetCurrentOperation();
	string GetEndTransaction();
	friend ostream& operator<< (ostream&, const CLdbReg&);
};

ostream& operator<< (ostream&, const CLdbReg&);
#endif

