//	=============================================================================
//
//	 (c) Copyright  2006 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            CLDBParser
//	 File:              CLDBParser.cpp
//	 Project:           CLDBParser
//	 Author:            Meta4 Software M.S. , S.A
//	 Date:				31/05/06
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//	 Definition: this class represents the parser of the LDB file
//
//	 
//	=============================================================================

#include "cldbparser.hpp"
#include "cldbreg.hpp"
#include "cbasicfile.hpp"

#include <iostream>
#include <set>

#include <string.h>
#include <stdlib.h>

#ifdef _UNIX
#include <unistd.h>
#else
#include <windows.h>
#endif

using namespace std;

CLdbParser::CLdbParser(const CLdbFile &inputFile, CBasicFile &outputFile, int iMinSize, int iMinTime, set<string> &setExclusions, set<string> &setIncludes, CBasicFile &traceFile, CBasicFile &criticalFile, MapReg &oPendingOperations) :
	m_iMinSize(iMinSize),
	m_iMinTime(iMinTime),
	m_lLastTick(0),
	m_setExclusions(setExclusions),
	m_setIncludes(setIncludes),
	m_inputFile(inputFile),
	m_outputFile(outputFile),
	m_criticalFile(criticalFile),
	//m_partialFile(gNullFile),
	m_traceFile(traceFile),
	m_oMap(oPendingOperations)
{
}

CLdbParser::~CLdbParser()
{
}

//--------------------------------------------------------------------------------------------
// DESCRIPCION:
// Reads the content of the ldb insp file selecting those registers 
// objeto. Si ya hay una sesión abierta con los mismos argumentos se devuelve su handler 
// y se incrementa la variable miembro 'm_iNumTimesOpened' que almacena el número de veces
// que se ha abierto una sesión.
//
// ENTRADA:
//		const string& ai_sLocale			->	Idioma de Verity.
//		const string& ai_sCharmap			->	Mapa de caractees de Verity
//		const string& ai_sKnowledgeBase		->	KnowledgeBase de Verity.
//
// SALIDA:
//		m4int64_t& ao_hSession				->	Devuelve el handler a la sesión.
//
//--------------------------------------------------------------------------------------------

void CLdbParser::setNumciclos (int op){
    numciclos = op;
}

unsigned long long CLdbParser::parse(unsigned long long ai_lFromTick)
{

	// Initial situation	

	// HOST and SERVER
	char pcUser[MAX_LENGTH];
	char *sLinea;
	char sEmptyLine[2] = "\n";
	char *p1, *p2, *p3;
	char *pM4Obj, *pNode, *pDate, *pTick, *pThread, *pTime, *pUser, *pConnection;
	unsigned long long lTick;
	int iTime, iConnection;
	bool bFistNumberOfRecordsOccurrence = true;
	string line;
	m_sHost = "";
	m_sServer = "";
	m_sCurrentUser = "";
	m_oCurrentReg = NULL;
	m_lLastTick = 0;
	m_iCurrentOperation = NO_OPERATION; 
	m_iCurrentConnection = -10;

	// Trace
	M4OUT(m_traceFile) << "Read file " << m_inputFile << endl;

	// Get ldbinsp into memory
	char *buffer;
    long lSize = m_inputFile.Load(&buffer);
	if (lSize == -1) {
		cout << "Error opening " << m_inputFile << endl;
		return -1;
	}

	if (!m_outputFile.Open()) {
		cout << "Error opening " << m_outputFile << endl;
		return -1;
	}

	
	// -----------------------------------------------
	// Set Host and Server

	// HOST
	char bufHost[1024];

#ifdef _UNIX
	if (!gethostname(bufHost, 1024))
#else
	DWORD dwCompNameLen = 1024;
	if (0 != GetComputerName(bufHost, &dwCompNameLen))
#endif
		m_sHost = bufHost;

	// SERVER
	m_sServer = m_inputFile.GetServer();
	// -----------------------------------------------

	// -----------------------------------------------
	char *newLine, *lastLine;
	lastLine = buffer;
	newLine = strchr(lastLine, '\n');
	// -----------------------------------------------

	// Trace
	M4OUT(m_traceFile) << "Process" << endl;

	// Show progress in case of single execution.
	long maximo = lSize;
	//////rewind(m_fpIn);
	double porcentaje=0.0;
	int limite = -1;
	long pos = 0;

    // Loop over file
	while (newLine != NULL){
		try {
			size_t iSize = newLine - lastLine;

			if ((iSize)>0) {
				sLinea = lastLine;
				sLinea[iSize] = '\0';
			}
			else {
				sLinea = sEmptyLine;
			}
			lastLine = newLine + 1;
			if (lastLine >= buffer + lSize)
				newLine = NULL;
			else {
				newLine = strchr(lastLine, '\n');
				if (newLine == NULL)
					newLine = buffer + lSize;
			}
			if ((newLine > lastLine) && (*(newLine - 1) == '\r')) *(newLine - 1) = '\0';

			// Show progress in case of single execution.
			pos += (long)iSize;
			porcentaje = ((double)pos / maximo) * 100;
			if (porcentaje >= limite && !numciclos) {
				//cout << (limite+1) << " --> " << m_inputFile << endl;
				cout << "Parsing " << m_inputFile << ": " << (limite + 1) << "%" << endl;
				limite += 10;
			}

			if (sLinea[strlen(sLinea) - 1] == '\n')
				sLinea[strlen(sLinea) - 1] = 0;
	

			//Connection tag: determine whether there is a pending operation 
			//Get ID Connection and ID User
			if (strstr(sLinea, CONNECTION_TAG) != NULL) {

				//User tag: when a user is using a connection no other user can
				p1 = strstr(sLinea, USER_TAG);
				//Bugfix: some connection lines are cut: the User tag is missing (like >>>C3<<<). p1 would be null and cause a crash
				pUser = NULL;
				if (p1 != NULL)
				{
					pUser = strtok(p1 + STRLEN(USER_TAG), " :");
				}
				
				if (pUser == NULL || pUser[0] == 0x0d) {
					pUser = (char *)"-";
				}
				else if (pUser[strlen(pUser) - 1] == 0x0d) {
					pUser[strlen(pUser) - 1] = '\0';
				}
				strcpy(pcUser, pUser);
				m_sCurrentUser = pcUser;

				p2 = strstr(sLinea, CONNECTION_TAG);
				pConnection = strtok(p2 + STRLEN(CONNECTION_TAG), "C");
				iConnection = atoi(pConnection);
				m_iCurrentConnection = iConnection;
				m_iCurrentOperation = NO_OPERATION;
				




				MapReg::iterator ite;
				ite = m_oMap.find(m_iCurrentConnection);
				if (ite != m_oMap.end())
				{
					//pending operation
					m_oCurrentReg = (CLdbReg *)(*ite).second;
					m_iCurrentOperation = m_oCurrentReg->GetCurrentOperation();
				}
			}

			//No dealing with Real SQL. 

			if (strstr(sLinea, EXECUTE_LOAD_TAG) == NULL && strstr(sLinea, EXECUTE_UPDATE_TAG) == NULL &&
				strstr(sLinea, EXECUTE_DELETE_TAG) == NULL && strstr(sLinea, EXECUTE_INSERT_TAG) == NULL &&
				m_iCurrentOperation == NO_OPERATION) {
				continue;
			}

			if (strstr(sLinea, CALCULATING_TAG) != NULL) {
				continue;
			}
			
			//Execute Tag
			//Determine the operation type; add new operation to the map; and get M4Object and Node 
			if (m_iCurrentOperation == NO_OPERATION && (strstr(sLinea, EXECUTE_LOAD_TAG) != NULL ||
				strstr(sLinea, EXECUTE_UPDATE_TAG) != NULL || strstr(sLinea, EXECUTE_DELETE_TAG) != NULL || 
				strstr(sLinea, EXECUTE_INSERT_TAG) != NULL)) 
			{
				if (strstr(sLinea, M4OBJ_TAG) ==  NULL || strstr(sLinea, NODE_TAG) == NULL)
				{
					M4OUT(m_traceFile) << "Inconsistent Excecute line: missing Meta4Object or Node. Last tick = " + to_string(m_lLastTick) << endl;
					continue;
				}
				
				pM4Obj = strstr(sLinea, M4OBJ_TAG);
				pM4Obj = pM4Obj + STRLEN(M4OBJ_TAG);
				pNode = strstr(pM4Obj, NODE_TAG);
				pNode = pNode + STRLEN(NODE_TAG);
				pM4Obj = strtok(pM4Obj, ". ");
				pNode = strtok(pNode, ". ");
				//Check for exceptions and do not process them
				set<string>::iterator it;
				it = m_setExclusions.find((string)pM4Obj);
				if (it == m_setExclusions.end())
				{
					//new operation
					m_oCurrentReg = new CLdbReg();
					m_oCurrentReg->SetConnection(m_iCurrentConnection);
					m_oCurrentReg->SetHost(m_sHost);
					m_oCurrentReg->SetServer(m_sServer);
					m_oCurrentReg->SetUser(m_sCurrentUser);
					m_oMap.insert(make_pair(m_iCurrentConnection, m_oCurrentReg));

					if (strstr(sLinea, EXECUTE_LOAD_TAG) != NULL)
					{
						m_iCurrentOperation = LOAD_OPERATION;
					}
					else if (strstr(sLinea, EXECUTE_UPDATE_TAG) != NULL)
					{
						m_iCurrentOperation = UPDATE_OPERATION;
					}
					else if (strstr(sLinea, EXECUTE_DELETE_TAG) != NULL)
					{
						m_iCurrentOperation = DELETE_OPERATION;
					}
					else if (strstr(sLinea, EXECUTE_INSERT_TAG) != NULL)
					{
						m_iCurrentOperation = INSERT_OPERATION;
						//Insert affects only one record by default unless INSERT INTO ... SELECT ... statement
						m_oCurrentReg->SetNumRecords(1);
					}

					m_oCurrentReg->SetCurrentOperation(m_iCurrentOperation);
					m_oCurrentReg->SetM4Obj(pM4Obj);
					m_oCurrentReg->SetNode(pNode);
				}
				else 
				{
					pM4Obj = 0;
					pNode = 0;
					continue;
				}
			}
			
			//DB Connection tag: date and tick
			if (strstr(sLinea, DB_CONN_TAG) != NULL) {

				//Bugfix: in some cases file swap cuts the DB Connectionline, in such cases it is necssary to prevent a crash
				pDate = strstr(sLinea, DATE_TAG);
				if (pDate != NULL)
				{
					pDate = pDate + STRLEN(DATE_TAG);
					pTick = strstr(sLinea, TICK_TAG);
					if (pTick != NULL)
					{
						pTick = pTick + STRLEN(TICK_TAG);
						pDate = strtok(pDate, ".");
						pTick = strtok(pTick, ".");
						lTick = stoll(pTick, 0, 10);
						m_lLastTick = lTick;
						//Set Start Tick and date of the first Execute Stmt action for the whole operation
						if (m_oCurrentReg->GetStartTick() == 0) {
							m_oCurrentReg->SetStartTick(lTick);
							m_oCurrentReg->SetDate(pDate);
						}
						//Subsequent DB Connection tag occurrences 
						m_oCurrentReg->SetEndTick(lTick);
						m_oCurrentReg->SetNumSentences(m_oCurrentReg->GetNumSentences() + 1);
					}
				}
			}

			//Execute Stmt tag: only the main sentence for all the operation
			if (strstr(sLinea, EXECUTE_STMT_TAG) != NULL) {
				/*Bug: sometimes a sentence may contain a \0D\0D\0A sequence and will dump a blank line in the output file*/
				line = sLinea;
				size_t found = line.find('\r');
				if (found != string::npos) 
				{
					line = line.substr(0, found);
				}
				switch (m_iCurrentOperation)
				{
					case LOAD_OPERATION: 
						if (m_oCurrentReg->GetMainSentence() == "")
						{
							m_oCurrentReg->SetMainSentence(line.substr(STRLEN(EXECUTE_STMT_TAG) + 1));
						}
						break;
					case INSERT_OPERATION: {
						if (m_oCurrentReg->GetMainSentence() == "" && strstr(sLinea, EXECUTE_STMT_INSERT_TAG) != NULL)
						{
							m_oCurrentReg->SetMainSentence(line.substr(STRLEN(EXECUTE_STMT_TAG) + 1));
						}
						break;
					}
					case UPDATE_OPERATION:
						//Fist update sentence in a block
						if (m_oCurrentReg->GetMainSentence() == "" && strstr(sLinea, EXECUTE_STMT_UPDATE_TAG) != NULL)
						{
							m_oCurrentReg->SetMainSentence(line.substr(STRLEN(EXECUTE_STMT_TAG) + 1));
						}
						break;
					case DELETE_OPERATION:
						if (m_oCurrentReg->GetMainSentence() == "" && strstr(sLinea, EXECUTE_STMT_DELETE_TAG) != NULL)
						{
							m_oCurrentReg->SetMainSentence(line.substr(STRLEN(EXECUTE_STMT_TAG) + 1));
						}
						break;
					default:
						break;
				}
			}

			//Number of records
			if (strstr(sLinea, NO_DATA_FOUND_TAG) != NULL && m_iCurrentOperation == LOAD_OPERATION)
			{
				m_oCurrentReg->SetNumRecords(0);
			}

			if (strstr(sLinea, RECORDS_TAG) != NULL) 
			{
				if (m_iCurrentOperation == LOAD_OPERATION)
				{
					line = sLinea;
					m_oCurrentReg->SetNumRecords(atoi((line.substr(STRLEN(RECORDS_TAG) + 1, strlen(sLinea))).c_str()));
				}
				else if (bFistNumberOfRecordsOccurrence && (m_iCurrentOperation != LOAD_OPERATION))
				{
					line = sLinea;
					m_oCurrentReg->SetNumRecords(atoi((line.substr(STRLEN(RECORDS_TAG) + 1, strlen(sLinea))).c_str()));
					bFistNumberOfRecordsOccurrence = false;
				}
			}

			
			// Tick, thread, time
			if (((p1 = strstr(sLinea, TICK_TAG)) != NULL) && ((p2 = strstr(sLinea, THREAD_TAG)) != NULL) && ((p3 = strstr(sLinea, TIME_TAG)) != NULL)) {
				pTick = strtok(p1 + STRLEN(TICK_TAG), " ");
				pThread = strtok(p2 + STRLEN(THREAD_TAG), " ");
				pTime = strtok(p3 + STRLEN(TIME_TAG), " ");
				lTick = stoll(pTick, 0, 10);
				iTime = atoi(pTime);
				m_oCurrentReg->SetThread(pThread);
				m_oCurrentReg->SetTimeTick(lTick);
				m_oCurrentReg->SetEndTick(lTick);
				m_oCurrentReg->SetTime(iTime);
				m_lLastTick = lTick;
				if (m_iCurrentOperation == LOAD_OPERATION)
				{
					dumpTrace(ai_lFromTick);
					bFistNumberOfRecordsOccurrence = true;
					continue;
				}
			}
			
			//End transaction: commit/rollback
			if (strstr(sLinea, COMMIT_TAG) != NULL)
			{
				line = sLinea;
				m_oCurrentReg->SetEndTransaction("COMMIT");
				dumpTrace(ai_lFromTick);
				bFistNumberOfRecordsOccurrence = true;
			}
			else if (strstr(sLinea, ROLLBACK_TAG) != NULL)
			{
				line = sLinea;
				m_oCurrentReg->SetEndTransaction("ROLLBACK");
				dumpTrace(ai_lFromTick);
				bFistNumberOfRecordsOccurrence = true;
			}
		}
		catch (...) {
			continue;
		}
	}
	//close files
	m_outputFile.Close();
	m_criticalFile.Close();
	// -----------------------------------------------
	free(buffer);
	// -----------------------------------------------
	cout << "Finished parsing " << m_inputFile << endl;

	return m_lLastTick;
}

void CLdbParser::dumpTrace(unsigned long long& aio_lFromTick)
{
	bool bDumpAlways = false;
	//Do not dump the operation if both start and end tick are earlier than the one is passed to this method 
	if (m_oCurrentReg->GetEndTick() > aio_lFromTick || m_oCurrentReg->GetStartTick() > aio_lFromTick) {
		//Dump all operations from here
		aio_lFromTick = 0;
		if (m_setIncludes.size() > 0)
		{
			set<string>::iterator it;
			it = m_setIncludes.find(m_oCurrentReg->GetM4Obj());
			if (it != m_setIncludes.end())
				bDumpAlways = true;
    	}

		if (m_oCurrentReg->GetTime() >= m_iMinTime || m_oCurrentReg->GetNumRecords() >= m_iMinSize || bDumpAlways){
			if (m_oCurrentReg->GetEndTick() == 0)
			{
				m_oCurrentReg->SetEndTick(m_lLastTick);
			}
			M4OUT(m_outputFile) << *m_oCurrentReg;
			// Flush Critical trace if needed
			if(m_criticalFile.IsEnabled()){
				if(
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG1)==NULL)
					&&
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG2)==NULL)
					&&
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG3)==NULL)
					&&
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG4)==NULL)
					&&
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG5)==NULL)
					&&
					(strstr(m_oCurrentReg->GetMainSentence().c_str(),ID_ORG_TAG6)==NULL)
					&&
					(m_oCurrentReg->GetMainSentence()!="?")
				){
						//Write to critical file
						if (m_criticalFile.Open()) {
							M4OUT(m_criticalFile) << *m_oCurrentReg;
						} else {
							cerr << "Error opening critical file " << m_criticalFile << endl;
						}
				}
			
			}
		}
	}
	//Remove dumped operation fom the map and reset m_oCurrentReg;
	m_oMap.erase(m_oCurrentReg->GetConnection());
	delete m_oCurrentReg;
	m_iCurrentOperation = NO_OPERATION;
}



