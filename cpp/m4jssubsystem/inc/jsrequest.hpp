//## begin module%37DF948E01B1.cm preserve=no
//## end module%37DF948E01B1.cm

//## begin module%37DF948E01B1.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%37DF948E01B1.cp

//## Module: jsRequest%37DF948E01B1; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\inc\jsrequest.hpp

#ifndef jsrequest_h
#define jsrequest_h 1

//## begin module%37DF948E01B1.additionalIncludes preserve=no
//## end module%37DF948E01B1.additionalIncludes

//## begin module%37DF948E01B1.includes preserve=yes
#include <m4stl.hpp>
#include <m4jsres.hpp>
#include <basiclog.hpp>
//## end module%37DF948E01B1.includes

// BaseRequest
#include <baserequest.hpp>
// FastQ
#include <fastfifoq.hpp>
// m4string
#include <m4string.hpp>
//## begin module%37DF948E01B1.declarations preserve=no
//## end module%37DF948E01B1.declarations

//## begin module%37DF948E01B1.additionalDeclarations preserve=yes
//////////////////////////////
//Request de Ejecutor al administrador
//////////////////////////////
#define M4JS_REQ_BEGIN_JOB				1
#define M4JS_REQ_END_JOB				2
#define M4JS_REQ_EXECUTOR_CREATED		3
	/* Param1: Executor
	   Param2: 0 -> Executor de jobs
			  1 -> Cancelador */
#define M4JS_REQ_EXECUTOR_DELETED		4
	/* Param1: Executor
	   Param2: 0 -> Executor de jobs
			   1 -> Cancelador */

//////////////////////////////
//Request del Recuperador al administrador
//////////////////////////////
#define M4JS_REQ_END_NEW_JOBS			10
#define M4JS_REQ_NEW_JOB				11
#define M4JS_REQ_CONTROLED_INTERRUPT	12
#define M4JS_REQ_UNCONTROLED_INTERRUPT	13
#define M4JS_REQ_SHUTDOWN_OK			14

//////////////////////////////
//Request del Administrador al recuperador
//////////////////////////////
#define M4JS_REQ_FIND_CANCEL			20
#define M4JS_REQ_FIND_EXPIRED			21
#define M4JS_REQ_FIND_RES_EXPIRED		22
#define M4JS_REQ_FIND_JOBS				23
#define M4JS_REQ_UNMARK_JOB				24
#define M4JS_REQ_INITIALIZE				25
	/* Param1: 0 -> 1ª Inicializacion
			   1 -> Siguientes inicializaciones */

//////////////////////////////
//Request del Administrador al ejecutor
//////////////////////////////
#define M4JS_REQ_EXECUTE_JOB			30
#define M4JS_REQ_DEL_EXECUTOR			31

//////////////////////////////
//Request del Subsistema o tarea del sistema al administrador
//////////////////////////////
#define M4JS_REQ_PET_NUM_EXECUTOR		50
	// Parm1 : Nº de ejecutores a los que queremos llegar
#define M4JS_REQ_SHUTDOWN				51
#define M4JS_REQ_BEGIN_JS				52
#define M4JS_REQ_END_JS					53
#define M4JS_REQ_GP						54
	/* Param1: Executor
	   Param2: 0 -> Executor de jobs
			   1 -> Cancelador */
#define M4JS_REQ_MEMORY_STATE			55
#define M4JS_REQ_DEL_START_DATE			56
#define M4JS_REQ_DEL_STOP_DATE			57
	/* Nos indica que ya el servidor ha finalizado su arranque*/
#define M4JS_REQ_SERVER_READY			58

//## end module%37DF948E01B1.additionalDeclarations


//## begin structRequestParam%37FB39600308.preface preserve=yes
//## end structRequestParam%37FB39600308.preface

//## Class: structRequestParam%37FB39600308
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct structRequestParam 
{
  //## begin structRequestParam%37FB39600308.initialDeclarations preserve=yes
  //## end structRequestParam%37FB39600308.initialDeclarations

    //## begin structRequestParam::eParamType%37FB436203A3.preface preserve=yes
    //## end structRequestParam::eParamType%37FB436203A3.preface

    //## Class: eParamType%37FB436203A3
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {REQ_PARAM_UINT32, REQ_PARAM_VOID} eParamType;

    //## begin structRequestParam::eParamType%37FB436203A3.postscript preserve=yes
    //## end structRequestParam::eParamType%37FB436203A3.postscript

    // Data Members for Class Attributes

      //## Attribute: m_oValue%37FB3975013C
      //## begin structRequestParam::m_oValue%37FB3975013C.attr preserve=no  public: m4pvoid_t {UA} 
      m4pvoid_t m_oValue;
      //## end structRequestParam::m_oValue%37FB3975013C.attr

      //## Attribute: m_uiType%37FB39B10323
      //## begin structRequestParam::m_uiType%37FB39B10323.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_uiType;
      //## end structRequestParam::m_uiType%37FB39B10323.attr

    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%3804C9730004
      //## Role: structRequestParam::m_oDescription%3804C974027D
      //## begin structRequestParam::m_oDescription%3804C974027D.role preserve=no  public: M4ClString { -> VHAN}
      M4ClString m_oDescription;
      //## end structRequestParam::m_oDescription%3804C974027D.role

  public:
    // Additional Public Declarations
      //## begin structRequestParam%37FB39600308.public preserve=yes
      //## end structRequestParam%37FB39600308.public

  protected:
    // Additional Protected Declarations
      //## begin structRequestParam%37FB39600308.protected preserve=yes
      //## end structRequestParam%37FB39600308.protected

  private:
    // Additional Private Declarations
      //## begin structRequestParam%37FB39600308.private preserve=yes
      //## end structRequestParam%37FB39600308.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin structRequestParam%37FB39600308.implementation preserve=yes
      //## end structRequestParam%37FB39600308.implementation

};

//## begin structRequestParam%37FB39600308.postscript preserve=yes
//## end structRequestParam%37FB39600308.postscript

//## begin paramList_t%37FB3BC101D8.preface preserve=yes
//## end paramList_t%37FB3BC101D8.preface

//## Class: paramList_t%37FB3BC101D8; Instantiated Class
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37FB3BD4037A;structRequestParam { -> }

typedef vector< structRequestParam * > paramList_t;

//## begin paramList_t%37FB3BC101D8.postscript preserve=yes
//## end paramList_t%37FB3BC101D8.postscript

//## begin ClJSRequest%37DCF0820109.preface preserve=yes
//## end ClJSRequest%37DCF0820109.preface

//## Class: ClJSRequest%37DCF0820109
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClJSRequest : public ClBaseRequest  //## Inherits: <unnamed>%37FB81CA02B5
{
  //## begin ClJSRequest%37DCF0820109.initialDeclarations preserve=yes
  //## end ClJSRequest%37DCF0820109.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSRequest%937399897; C++
      ClJSRequest (m4uint32_t ai_Type);

    //## Destructor (specified)
      //## Operation: ~ClJSRequest%937399898
      virtual ~ClJSRequest ();


    //## Other Operations (specified)
      //## Operation: AddParameter%937912501
      m4return_t AddParameter (m4uint32_t ai_value);

      //## Operation: AddParameter%937912502
      m4return_t AddParameter (m4pvoid_t ai_value);

      //## Operation: GetParameter%937912503
      m4return_t GetParameter (m4uint32_t ai_numParam, m4pvoid_t &ao_value);

      //## Operation: GetParameter%937912507
      m4return_t GetParameter (m4uint32_t ai_numParam, m4uint32_t &ao_value);

      //## Operation: Recycle%941813358; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

      //## Operation: CanRecycle%941813359
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%941813360
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: CanDestroy%941813361
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClJSRequest%37DCF0820109.public preserve=yes
      //## end ClJSRequest%37DCF0820109.public

  protected:
    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%37FB3C31008E
      //## Role: ClJSRequest::m_oParamList%37FB3C320389
      //## begin ClJSRequest::m_oParamList%37FB3C320389.role preserve=no  protected: paramList_t { -> VHAN}
      paramList_t m_oParamList;
      //## end ClJSRequest::m_oParamList%37FB3C320389.role

    // Additional Protected Declarations
      //## begin ClJSRequest%37DCF0820109.protected preserve=yes
      //## end ClJSRequest%37DCF0820109.protected

  private:
    // Additional Private Declarations
      //## begin ClJSRequest%37DCF0820109.private preserve=yes
      //## end ClJSRequest%37DCF0820109.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSRequest%37DCF0820109.implementation preserve=yes
      //## end ClJSRequest%37DCF0820109.implementation

};

//## begin ClJSRequest%37DCF0820109.postscript preserve=yes
//## end ClJSRequest%37DCF0820109.postscript

//## Class: ClJSQueue%37D7D4D20021; Instantiated Class
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37DF945A017A;ClJSRequest { -> }
//## Uses: <unnamed>%3805BE5A0292;ClJSRequest { -> }

typedef FastQueue< ClJSRequest * > ClJSQueue;

// Class structRequestParam 

// Class ClJSRequest 

inline ClJSRequest::ClJSRequest (m4uint32_t ai_Type)
  //## begin ClJSRequest::ClJSRequest%937399897.hasinit preserve=no
  //## end ClJSRequest::ClJSRequest%937399897.hasinit
  //## begin ClJSRequest::ClJSRequest%937399897.initialization preserve=yes
  : ClBaseRequest(ai_Type)
  //## end ClJSRequest::ClJSRequest%937399897.initialization
{
  //## begin ClJSRequest::ClJSRequest%937399897.body preserve=yes
  //## end ClJSRequest::ClJSRequest%937399897.body
}


inline ClJSRequest::~ClJSRequest ()
{
  //## begin ClJSRequest::~ClJSRequest%937399898.body preserve=yes
	paramList_t::iterator it;
	m4uint32_t *tempvalue;
	structRequestParam *Param;
	it = m_oParamList.begin();
	while (it!=m_oParamList.end()) {
		Param = *it;
		if (Param->m_uiType == structRequestParam::REQ_PARAM_UINT32) {
			tempvalue = (m4uint32_t *)Param->m_oValue;
			delete tempvalue;
		}
		delete Param;
		m_oParamList.erase(it);
		it = m_oParamList.begin();
	}
  //## end ClJSRequest::~ClJSRequest%937399898.body
}



//## Other Operations (inline)
inline m4return_t ClJSRequest::AddParameter (m4uint32_t ai_value)
{
  //## begin ClJSRequest::AddParameter%937912501.body preserve=yes
	structRequestParam *newParam = new structRequestParam();
	m4uint32_t *valor = new m4uint32_t;
	if (newParam == NULL || valor == NULL) {
		BL_ERRORF(M4_JSADM_REQ , "AddParameter-int-");
		return M4_ERROR;
	}
	*valor = ai_value;
	newParam->m_oValue = valor;
	newParam->m_uiType = structRequestParam::REQ_PARAM_UINT32;
	m_oParamList.push_back(newParam);
	return M4_SUCCESS;
  //## end ClJSRequest::AddParameter%937912501.body
}

inline m4return_t ClJSRequest::AddParameter (m4pvoid_t ai_value)
{
  //## begin ClJSRequest::AddParameter%937912502.body preserve=yes
	structRequestParam *newParam = new structRequestParam();
	if (newParam==NULL) {
		BL_ERRORF(M4_JSADM_REQ , "AddParameter-void-");
		return M4_ERROR;
	}
	newParam->m_oValue = ai_value;
	newParam->m_uiType = structRequestParam::REQ_PARAM_VOID;
	m_oParamList.push_back(newParam);

	return M4_SUCCESS;
  //## end ClJSRequest::AddParameter%937912502.body
}

inline m4return_t ClJSRequest::GetParameter (m4uint32_t ai_numParam, m4pvoid_t &ao_value)
{
  //## begin ClJSRequest::GetParameter%937912503.body preserve=yes
	m4return_t ret = M4_ERROR;
	structRequestParam *Param;
	if (ai_numParam<m_oParamList.size()) {
		Param = m_oParamList[ai_numParam];
		if (Param->m_uiType ==  structRequestParam::REQ_PARAM_VOID) {
			ao_value = Param->m_oValue;
			ret = M4_SUCCESS;
		}
	}
	if (ret == M4_ERROR)
		BL_WARNINGF(M4_JSADM_REQ , "GetParameter-void-");
	return ret;
  //## end ClJSRequest::GetParameter%937912503.body
}

inline m4return_t ClJSRequest::GetParameter (m4uint32_t ai_numParam, m4uint32_t &ao_value)
{
  //## begin ClJSRequest::GetParameter%937912507.body preserve=yes
	m4return_t ret = M4_ERROR;
	structRequestParam *Param;
	if (ai_numParam<m_oParamList.size()) {
		Param = m_oParamList[ai_numParam];
		if (Param->m_uiType ==  structRequestParam::REQ_PARAM_UINT32) {
			ao_value = *((m4uint32_t*)Param->m_oValue);
			ret = M4_SUCCESS;
		}
	}
	if (ret == M4_ERROR)
		BL_WARNINGF(M4_JSADM_REQ , "GetParameter-int-");
	return ret;
  //## end ClJSRequest::GetParameter%937912507.body
}

inline m4return_t ClJSRequest::Recycle ()
{
  //## begin ClJSRequest::Recycle%941813358.body preserve=yes
	return M4_ERROR ;
  //## end ClJSRequest::Recycle%941813358.body
}

inline m4bool_t ClJSRequest::CanRecycle ()
{
  //## begin ClJSRequest::CanRecycle%941813359.body preserve=yes
	return M4_FALSE ;
  //## end ClJSRequest::CanRecycle%941813359.body
}

inline m4return_t ClJSRequest::Destroy ()
{
  //## begin ClJSRequest::Destroy%941813360.body preserve=yes
	return M4_ERROR ;
  //## end ClJSRequest::Destroy%941813360.body
}

inline m4bool_t ClJSRequest::CanDestroy ()
{
  //## begin ClJSRequest::CanDestroy%941813361.body preserve=yes
	return M4_FALSE ;
  //## end ClJSRequest::CanDestroy%941813361.body
}

//## begin module%37DF948E01B1.epilog preserve=yes
//## end module%37DF948E01B1.epilog


#endif
