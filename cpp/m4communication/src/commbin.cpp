//## begin module%3991771A0024.cm preserve=no
//## end module%3991771A0024.cm

//## begin module%3991771A0024.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3991771A0024.cp

//## Module: CommBin%3991771A0024; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commbin.cpp

//## begin module%3991771A0024.additionalIncludes preserve=no
//## end module%3991771A0024.additionalIncludes

//## begin module%3991771A0024.includes preserve=yes
//## end module%3991771A0024.includes

// m4types
#include <m4types.hpp>
// CommBin
#include <commbin.hpp>
//## begin module%3991771A0024.declarations preserve=no
//## end module%3991771A0024.declarations

//## begin module%3991771A0024.additionalDeclarations preserve=yes
#include <m4thread.hpp>
// No funciona el BodyReference
#include <commtask.hpp>
//## end module%3991771A0024.additionalDeclarations


// Class ClCommRecyclable 


ClCommRecyclable::ClCommRecyclable (m4int_t ai_iRecycleObjID)
  //## begin ClCommRecyclable::ClCommRecyclable%967625198.hasinit preserve=no
      : m_iRecyclableObjID(-1)
  //## end ClCommRecyclable::ClCommRecyclable%967625198.hasinit
  //## begin ClCommRecyclable::ClCommRecyclable%967625198.initialization preserve=yes
  //## end ClCommRecyclable::ClCommRecyclable%967625198.initialization
{
  //## begin ClCommRecyclable::ClCommRecyclable%967625198.body preserve=yes
	m_iRecyclableObjID=ai_iRecycleObjID;
  //## end ClCommRecyclable::ClCommRecyclable%967625198.body
}


ClCommRecyclable::~ClCommRecyclable ()
{
  //## begin ClCommRecyclable::~ClCommRecyclable%967625196.body preserve=yes
  //## end ClCommRecyclable::~ClCommRecyclable%967625196.body
}



//## Other Operations (implementation)
m4int_t ClCommRecyclable::GetRecycleObjID ()
{
  //## begin ClCommRecyclable::GetRecycleObjID%967625197.body preserve=yes
	return m_iRecyclableObjID;
  //## end ClCommRecyclable::GetRecycleObjID%967625197.body
}

// Additional Declarations
  //## begin ClCommRecyclable%399174ED02E0.declarations preserve=yes
  //## end ClCommRecyclable%399174ED02E0.declarations

// Class ClCommBin 



//## Other Operations (implementation)
ClCommRecyclable * ClCommBin::Get (m4int_t ai_iObjectType)
{
  //## begin ClCommBin::Get%965834587.body preserve=yes
//M4_COMM_WRITTING_TASK		0
//M4_COMM_READING_TASK		1
//M4_COMM_OPENPORT_TASK		2
//M4_COMM_CLOSEPORT_TASK		3
//M4_COMM_OPENINGSOCKET_TASK    4
//M4_COMM_CLOSESOCKET_TASK    5
//M4_COMM_SHUTDOWN_TASK			6

	ClCommRecyclable * theObject=NULL;
	switch (ai_iObjectType)
	{
	case M4_COMM_WRITTING_TASK:
		m4TraceLevel(3,cout <<"WRITTING_TASK"<<endl);
		theObject=new ClCommWrittingTask;
		break;
	case M4_COMM_READING_TASK:
		m4TraceLevel(3,cout <<"READING_TASK"<<endl);
		theObject = new ClCommReadingTask;
		break;
	case M4_COMM_OPENSOCKET_TASK:
		m4TraceLevel(3,cout <<"M4_COMM_OPENSOCKET_TASK"<<endl);
		theObject = new ClCommOpeningSocketTask;
		break;
	case M4_COMM_OPENPORT_TASK:
		m4TraceLevel(3,cout <<"M4_COMM_OPENPORT_TASK"<<endl);
		theObject = new ClCommOpenPortTask ;
		break;
	case M4_COMM_CLOSESOCKET_TASK:
		m4TraceLevel(3,cout <<"M4_COMM_CLOSESOCKET_TASK"<<endl);
		theObject =new ClCommCloseSocketTask;
		break;
	case M4_COMM_SHUTDOWN_TASK:
		m4TraceLevel(3,cout <<"M4_COMM_SHUTDOWN_TASK"<<endl);
		theObject =new ClCommShutDownTask;
		break;
	case M4_COMM_STOPREADING_TASK:
		m4TraceLevel(3,cout <<"M4_COMM_STOPREADING_TASK"<<endl);
		theObject =new ClCommStopReadingTask;
		break;
	default:
		m4Trace(cerr << "Unkown Type "<< ai_iObjectType<<endl);
		break;
	}
//m4TraceLevel(2,cerr <<"thread:"<<M4Thread::GetCurrentIdThread()<<" Pide:"<<ai_iObjectType<< ":"<<theObject<<endl);
	return theObject;
  //## end ClCommBin::Get%965834587.body
}

m4return_t ClCommBin::Release (ClCommRecyclable *ai_pRecyclable)
{
  //## begin ClCommBin::Release%965834588.body preserve=yes
	m4TraceLevel(2,cerr <<"thread:"<<M4Thread::GetCurrentIdThread()<<" borra:"<<ai_pRecyclable<<endl);
	delete ai_pRecyclable;
	return M4_SUCCESS;
  //## end ClCommBin::Release%965834588.body
}

// Additional Declarations
  //## begin ClCommBin%399175B70308.declarations preserve=yes
  //## end ClCommBin%399175B70308.declarations

//## begin module%3991771A0024.epilog preserve=yes
//## end module%3991771A0024.epilog
