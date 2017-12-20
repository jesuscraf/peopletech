//## begin module%35643B25019C.cm preserve=no
//## end module%35643B25019C.cm

//## begin module%35643B25019C.cp preserve=no
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
//## end module%35643B25019C.cp

//## Module: CommandRequest%35643B25019C; Subprogram specification
//## Subsystem: M4Request::inc%379439BF01B6
//## Source file: d:\eduardoma\M4Request\version\inc\commandrequest.hpp

#ifndef commandrequest_h
#define commandrequest_h 1

//## begin module%35643B25019C.additionalIncludes preserve=no
#include <m4request_dll.hpp>
#include <m4types.hpp>
#include <baserequest.hpp>
#include <logsys.hpp>
//## end module%35643B25019C.declarations

//## begin module%35643B25019C.additionalDeclarations preserve=yes
#define BUFFER_SIZE		32
//## end module%35643B25019C.additionalDeclarations


//## begin ClBinaryBlock%35895DEB0079.preface preserve=yes
//## end ClBinaryBlock%35895DEB0079.preface

//## Class: ClBinaryBlock%35895DEB0079
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4REQUEST ClBinaryBlock 
{
  //## begin ClBinaryBlock%35895DEB0079.initialDeclarations preserve=yes
  //## end ClBinaryBlock%35895DEB0079.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBinaryBlock%898195370
      ClBinaryBlock ();

      //## Operation: ClBinaryBlock%898195371; C++
      ClBinaryBlock (m4pchar_t  ai_pBlock, m4uint32_t ai_uiLength);

    //## Destructor (specified)
      //## Operation: ~ClBinaryBlock%898195372
      ~ClBinaryBlock ();


    //## Other Operations (specified)
      //## Operation: GetContent%898195374; C++
      m4return_t GetContent (m4pchar_t &ao_pBlock, m4uint32_t &ao_uiLength) const;

      //## Operation: SetContent%898195375; C++
      m4return_t SetContent (m4pchar_t  ai_pBlock, m4uint32_t ai_uiLength);

      //## Operation: Clear%898195373
      m4return_t Clear ();

    // Additional Public Declarations
      //## begin ClBinaryBlock%35895DEB0079.public preserve=yes
      //## end ClBinaryBlock%35895DEB0079.public

  protected:
    // Data Members for Associations

      //## Association: M4Request::<unnamed>%35895F2D027C
      //## Role: ClBinaryBlock::m_pContent%35895F2E00C7
      //## begin ClBinaryBlock::m_pContent%35895F2E00C7.role preserve=no  protected: m4pchar_t {0..n -> 1VHAN}
      m4pchar_t m_pContent;
      //## end ClBinaryBlock::m_pContent%35895F2E00C7.role

      //## Association: M4Request::<unnamed>%35895F52004A
      //## Role: ClBinaryBlock::m_uiSize%35895F52023E
      //## begin ClBinaryBlock::m_uiSize%35895F52023E.role preserve=no  protected: m4uint32_t {0..n -> 1VHAN}
      m4uint32_t m_uiSize;
      //## end ClBinaryBlock::m_uiSize%35895F52023E.role

    // Additional Protected Declarations
      //## begin ClBinaryBlock%35895DEB0079.protected preserve=yes
      //## end ClBinaryBlock%35895DEB0079.protected

  private:
    // Additional Private Declarations
      //## begin ClBinaryBlock%35895DEB0079.private preserve=yes
      //## end ClBinaryBlock%35895DEB0079.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBinaryBlock%35895DEB0079.implementation preserve=yes
      //## end ClBinaryBlock%35895DEB0079.implementation

};

//## begin ClBinaryBlock%35895DEB0079.postscript preserve=yes
//## end ClBinaryBlock%35895DEB0079.postscript

//## begin ClCommandRequest%35507CEF031C.preface preserve=yes
//## end ClCommandRequest%35507CEF031C.preface

//## Class: ClCommandRequest%35507CEF031C
//	This class holds the contents of a creational request to the USService.
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4REQUEST ClCommandRequest : public ClControlRequest  //## Inherits: <unnamed>%356942D000CA
{
  //## begin ClCommandRequest%35507CEF031C.initialDeclarations preserve=yes
  //## end ClCommandRequest%35507CEF031C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommandRequest%894446851; C++
      ClCommandRequest (m4handle_t ai_uiReqId = 0, ClLogSystem *ai_poLog = NULL, ClStatistic *ai_poStatistic = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCommandRequest%894446852
      ~ClCommandRequest ();


    //## Other Operations (specified)
      //## Operation: SetCommand%894446853; C++
      m4return_t SetCommand (m4uint32_t ai_uiCommandId);

      //## Operation: GetCommand%894446854; C++
      m4return_t GetCommand (m4uint32_t &ao_uiCommandId) const;

      //## Operation: GetNumberOfArgs%894446855
      m4int32_t GetNumberOfArgs () const;

      //## Operation: GetArg%894446856; C++
      m4return_t GetArg (m4uint32_t ai_uiIndex, m4pchar_t &ao_pArg, m4uint32_t &ao_uiSize) const;

      //## Operation: AddArg%894446857; C++
      m4return_t AddArg (m4pchar_t ai_pArg, m4uint32_t ai_uiSize);

      //## Operation: SetLog%927645671; C++
      m4return_t SetLog (ClLogSystem *ai_poLog);

      //## Operation: GetLog%927645672; C++
      m4return_t GetLog (ClLogSystem *&ao_polog) const;

    // Additional Public Declarations
      //## begin ClCommandRequest%35507CEF031C.public preserve=yes
      //## end ClCommandRequest%35507CEF031C.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiCommandId%35508972009C
      //## begin ClCommandRequest::m_uiCommandId%35508972009C.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiCommandId;
      //## end ClCommandRequest::m_uiCommandId%35508972009C.attr

      //## Attribute: m_uiNumberOfArgs%3550B07C030D
      //## begin ClCommandRequest::m_uiNumberOfArgs%3550B07C030D.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiNumberOfArgs;
      //## end ClCommandRequest::m_uiNumberOfArgs%3550B07C030D.attr

    // Data Members for Associations

      //## Association: M4Request::<unnamed>%374AC8F102D1
      //## Role: ClCommandRequest::m_poReqLog%374AC8F201BA
      //## begin ClCommandRequest::m_poReqLog%374AC8F201BA.role preserve=no  protected: ClLogSystem {1 -> 1RHAN}
      ClLogSystem *m_poReqLog;
      //## end ClCommandRequest::m_poReqLog%374AC8F201BA.role

      //## Association: M4Request::<unnamed>%3550AECC000F
      //## Role: ClCommandRequest::m_rInit%3550AECC034B
      //## begin ClCommandRequest::m_rInit%3550AECC034B.role preserve=no  protected: m4return_t {1 -> 1VHAN}
      m4return_t m_rInit;
      //## end ClCommandRequest::m_rInit%3550AECC034B.role

      //## Association: M4Request::<unnamed>%3550B0350167
      //## Role: ClCommandRequest::m_pArglist%3550B0350399
      //## begin ClCommandRequest::m_pArglist%3550B0350399.role preserve=no  protected: ClBinaryBlock {1 -> 0..nRHN}
      ClBinaryBlock **m_pArglist;
      //## end ClCommandRequest::m_pArglist%3550B0350399.role

    // Additional Protected Declarations
      //## begin ClCommandRequest%35507CEF031C.protected preserve=yes
      //## end ClCommandRequest%35507CEF031C.protected

  private:
    // Additional Private Declarations
      //## begin ClCommandRequest%35507CEF031C.private preserve=yes
      //## end ClCommandRequest%35507CEF031C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommandRequest%35507CEF031C.implementation preserve=yes
      //## end ClCommandRequest%35507CEF031C.implementation

};

//## begin ClCommandRequest%35507CEF031C.postscript preserve=yes
//## end ClCommandRequest%35507CEF031C.postscript

// Class ClBinaryBlock 


//## Other Operations (inline)
inline m4return_t ClBinaryBlock::GetContent (m4pchar_t &ao_pBlock, m4uint32_t &ao_uiLength) const
{
  //## begin ClBinaryBlock::GetContent%898195374.body preserve=yes
	ao_pBlock = m_pContent;
	ao_uiLength = m_uiSize;

	return M4_SUCCESS;
  //## end ClBinaryBlock::GetContent%898195374.body
}

// Class ClCommandRequest 


//## Other Operations (inline)
inline m4return_t ClCommandRequest::SetCommand (m4uint32_t ai_uiCommandId)
{
  //## begin ClCommandRequest::SetCommand%894446853.body preserve=yes
	if ( M4_ERROR == m_rInit )
	{
		m_uiCommandId = ai_uiCommandId;
		m_rInit = M4_SUCCESS;
	}
	else
		m_rInit = M4_ERROR;

	return m_rInit;
  //## end ClCommandRequest::SetCommand%894446853.body
}

inline m4return_t ClCommandRequest::GetCommand (m4uint32_t &ao_uiCommandId) const
{
  //## begin ClCommandRequest::GetCommand%894446854.body preserve=yes
	if ( M4_ERROR != m_rInit )
	{
		ao_uiCommandId = m_uiCommandId;
	}
	
	return m_rInit;
  //## end ClCommandRequest::GetCommand%894446854.body
}

inline m4int32_t ClCommandRequest::GetNumberOfArgs () const
{
  //## begin ClCommandRequest::GetNumberOfArgs%894446855.body preserve=yes
	if ( M4_ERROR == m_rInit )
		return -1;

	return m_uiNumberOfArgs;
  //## end ClCommandRequest::GetNumberOfArgs%894446855.body
}

inline m4return_t ClCommandRequest::GetArg (m4uint32_t ai_uiIndex, m4pchar_t &ao_pArg, m4uint32_t &ao_uiSize) const
{
  //## begin ClCommandRequest::GetArg%894446856.body preserve=yes
	ClBinaryBlock		*pBlock;
	m4return_t			iRet;

	if ( M4_ERROR != m_rInit )
	{
		if ( ai_uiIndex >= m_uiNumberOfArgs)
			return M4_ERROR;

		pBlock = m_pArglist[ai_uiIndex];
		M4_ASSERT(NULL != pBlock);
	}

	iRet = m_rInit;
	iRet |= pBlock->GetContent(ao_pArg, ao_uiSize);

	return iRet;
  //## end ClCommandRequest::GetArg%894446856.body
}

inline m4return_t ClCommandRequest::AddArg (m4pchar_t ai_pArg, m4uint32_t ai_uiSize)
{
  //## begin ClCommandRequest::AddArg%894446857.body preserve=yes
	if ( M4_ERROR != m_rInit )
	{
		if (0 == (m_uiNumberOfArgs % BUFFER_SIZE))
			m_pArglist = (ClBinaryBlock **)realloc(m_pArglist, (m_uiNumberOfArgs + BUFFER_SIZE)*sizeof(ClBinaryBlock *));

		m_pArglist[m_uiNumberOfArgs++] = new ClBinaryBlock(ai_pArg, ai_uiSize);
	}

	return m_rInit;
  //## end ClCommandRequest::AddArg%894446857.body
}

inline m4return_t ClCommandRequest::SetLog (ClLogSystem *ai_poLog)
{
  //## begin ClCommandRequest::SetLog%927645671.body preserve=yes
	m_poReqLog = ai_poLog;

	return M4_SUCCESS;
  //## end ClCommandRequest::SetLog%927645671.body
}

inline m4return_t ClCommandRequest::GetLog (ClLogSystem *&ao_polog) const
{
  //## begin ClCommandRequest::GetLog%927645672.body preserve=yes
	ao_polog = m_poReqLog;

	return M4_SUCCESS;
  //## end ClCommandRequest::GetLog%927645672.body
}

//## begin module%35643B25019C.epilog preserve=yes
//## end module%35643B25019C.epilog


#endif
