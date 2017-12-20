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

#include "cldbreg.hpp"

using namespace std;

CLdbReg::CLdbReg()
{
	m_pcHost="";
	m_pcServer="";
	m_pcUser="";
	m_iConnection=-1;
	m_pcM4Obj="";
	m_pcNode="";
	m_pcMainSentence ="";
	m_iTime=0;
	m_iNumRecords=0;
	m_pcThread="";
	m_lStartTick =0;
	m_lTimeTick = 0;
	m_lEndTick = 0;
	m_pcDate="";
	m_iNumSentences = 0;
	m_pcEndTransaction = "";
	m_iCurrentOperation = NO_OPERATION;
}


CLdbReg::~CLdbReg()
{
}
void CLdbReg::SetHost(string ai_pcHost){m_pcHost=ai_pcHost;}
void CLdbReg::SetServer(string ai_pcServer){m_pcServer=ai_pcServer;}
void CLdbReg::SetUser(string ai_pcUser){m_pcUser=ai_pcUser;}
void CLdbReg::SetConnection(int ai_iConnection){m_iConnection=ai_iConnection;}
void CLdbReg::SetM4Obj(string ai_pcM4Obj){m_pcM4Obj=ai_pcM4Obj;}
void CLdbReg::SetNode(string ai_pcNode){m_pcNode=ai_pcNode;}
void CLdbReg::SetMainSentence(string ai_pcMainSentence){ m_pcMainSentence =ai_pcMainSentence;}
void CLdbReg::SetTime(int ai_iTime){m_iTime=ai_iTime;}
void CLdbReg::SetNumRecords(int ai_iNumRecords){m_iNumRecords=ai_iNumRecords;}
void CLdbReg::SetThread(string ai_pcThread){m_pcThread=ai_pcThread;}
void CLdbReg::SetStartTick(unsigned long long ai_lTick){ m_lStartTick =ai_lTick;}
void CLdbReg::SetTimeTick(unsigned long long ai_lTick) { m_lTimeTick = ai_lTick; }
void CLdbReg::SetEndTick(unsigned long long ai_lTick) { m_lEndTick = ai_lTick; }
void CLdbReg::SetDate(string ai_pcDate){m_pcDate=ai_pcDate;}
void CLdbReg::SetNumSentences(int ai_iNumSentences) { m_iNumSentences = ai_iNumSentences;}
void CLdbReg::SetCurrentOperation(int ai_iCurrentOperation) { m_iCurrentOperation = ai_iCurrentOperation; }
void CLdbReg::SetEndTransaction(string ai_iEndTransaction) { m_pcEndTransaction = ai_iEndTransaction; }
string CLdbReg::GetHost(){return m_pcHost;}
string CLdbReg::GetServer(){return m_pcServer;}
string CLdbReg::GetUser(){return m_pcUser;}
string CLdbReg::GetM4Obj(){return m_pcM4Obj;}
string CLdbReg::GetNode(){return m_pcNode;}
string CLdbReg::GetMainSentence(){return m_pcMainSentence;}
string CLdbReg::GetThread(){return m_pcThread;}
int CLdbReg::GetConnection(){return m_iConnection;}
int CLdbReg::GetTime(){return m_iTime;}
int CLdbReg::GetNumRecords(){return m_iNumRecords;}
unsigned long long CLdbReg::GetStartTick(){return m_lStartTick;}
unsigned long long CLdbReg::GetTimeTick() { return m_lTimeTick; }
unsigned long long CLdbReg::GetEndTick() { return m_lEndTick; }
string CLdbReg::GetDate(){return m_pcDate;}
int CLdbReg::GetNumSentences() { return m_iNumSentences; }
int CLdbReg::GetCurrentOperation() { return m_iCurrentOperation; }
string CLdbReg::GetEndTransaction() { return m_pcEndTransaction; }
ostream& operator<< (ostream &os, const CLdbReg &reg) {
	string out = reg.m_pcHost + "\t"
		+ reg.m_pcServer + "\t"
		+ reg.m_pcDate + "\t"
		+ to_string(reg.m_lStartTick) + "\t"
		+ ((reg.m_lTimeTick == 0) ? "-" : to_string(reg.m_lTimeTick)) + "\t"
		+ to_string(reg.m_lEndTick) + "\t"
		+ ((reg.m_pcUser == "") ? "-" : reg.m_pcUser) + "\t"
		+ ((reg.m_iConnection == -10) ? "-" : to_string(reg.m_iConnection)) + "\t"
		+ ((reg.m_pcThread == "") ? "-" : reg.m_pcThread) + "\t"
		+ reg.m_pcM4Obj + "\t"
		+ reg.m_pcNode + "\t"
		+ to_string(reg.m_iNumRecords) + "\t"
		+ ((reg.m_iTime == 0) ? "-" : to_string(reg.m_iTime)) + "\t"
		+ to_string(reg.m_iNumSentences) + "\t"
		+ ((reg.m_pcEndTransaction == "") ? "-" : reg.m_pcEndTransaction) + "\t"
		+ ((reg.m_pcMainSentence == "") ? "-" : reg.m_pcMainSentence);
	os << out.substr(0,3000) << endl;
	return os;
}
