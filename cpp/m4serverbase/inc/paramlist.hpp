//## begin module%3904571402A1.cm preserve=no
//## end module%3904571402A1.cm

//## begin module%3904571402A1.cp preserve=no
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
//## end module%3904571402A1.cp

//## Module: ParamList%3904571402A1; Package specification
//## Subsystem: M4ServerBase::ParamList::inc%390457300160
//## Source file: e:\mybuild\m4serverbase\inc\paramlist.hpp

#ifndef paramlist_h
#define paramlist_h 1

//## begin module%3904571402A1.additionalIncludes preserve=no
//## end module%3904571402A1.additionalIncludes

//## begin module%3904571402A1.includes preserve=yes
//## end module%3904571402A1.includes

// m4servervt
#include <m4servervt.hpp>
// Clienttag
#include <clienttag.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%3904571402A1.declarations preserve=no
//## end module%3904571402A1.declarations

//## begin module%3904571402A1.additionalDeclarations preserve=yes
//## end module%3904571402A1.additionalDeclarations


//## begin Param_map%39044F2B00D2.preface preserve=yes
//## end Param_map%39044F2B00D2.preface

//## Class: Param_map%39044F2B00D2; Instantiated Class
//## Category: M4ServerBase::ParamList%39044E070169
//## Subsystem: M4ServerBase::ParamList::inc%390457300160
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39044F68029D;LessString { -> }
//## Uses: <unnamed>%390450500304;m4ServerVT { -> }

typedef map< m4pchar_t,m4ServerVT *,LessString > Param_map;

//## begin Param_map%39044F2B00D2.postscript preserve=yes
//## end Param_map%39044F2B00D2.postscript

//## begin ClParamList%39044EBE03CF.preface preserve=yes
//## end ClParamList%39044EBE03CF.preface

//## Class: ClParamList%39044EBE03CF
//## Category: M4ServerBase::ParamList%39044E070169
//## Subsystem: M4ServerBase::ParamList::inc%390457300160
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClParamList : public IGarbageable  //## Inherits: <unnamed>%39AF781F03BF
{
  //## begin ClParamList%39044EBE03CF.initialDeclarations preserve=yes
  //## end ClParamList%39044EBE03CF.initialDeclarations

  public:
    //## begin ClParamList::eParamListState%39AF81B7006D.preface preserve=yes
    //## end ClParamList::eParamListState%39AF81B7006D.preface

    //## Class: eParamListState%39AF81B7006D
    //## Category: M4ServerBase::ParamList%39044E070169
    //## Subsystem: M4ServerBase::ParamList%390456FE000A
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { PARAM_IN_USE , PARAM_ASIGNED , PARAM_AVAILABLE } eParamListState;

    //## begin ClParamList::eParamListState%39AF81B7006D.postscript preserve=yes
    //## end ClParamList::eParamListState%39AF81B7006D.postscript

    //## Constructors (specified)
      //## Operation: ClParamList%956586087
      ClParamList ();

    //## Destructor (specified)
      //## Operation: ~ClParamList%956586088
      ~ClParamList ();


    //## Other Operations (specified)
      //## Operation: SetPointer%956586089; C++
      m4return_t SetPointer (m4pchar_t ai_szParamName, const void *ai_Pointer);

      //## Operation: SetString%956586090; C++
      m4return_t SetString (m4pcchar_t ai_szParamName, m4pcchar_t ai_pcString);

      //## Operation: SetBinaryChunk%956586091; C++
      m4return_t SetBinaryChunk (m4pcchar_t ai_szParamName, m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize);

      //## Operation: GetPointer%956586092; C++
      m4return_t GetPointer (m4pcchar_t ai_szParamName, void * &ao_Pointer);

      //## Operation: GetString%956586093; C++
      m4return_t GetString (m4pcchar_t ai_szParamName, m4pchar_t &ao_pcString);

      //## Operation: GetBinaryChunk%956586094; C++
      m4return_t GetBinaryChunk (m4pchar_t ai_szParamName, m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize);

      //## Operation: SetDouble%956586095; C++
      m4return_t SetDouble (m4pcchar_t ai_szParamName, const m4double_t ai_dValue);

      //## Operation: GetDouble%956586096; C++
      m4return_t GetDouble (m4pchar_t ai_szParamName, m4double_t &ao_dValue);

      //## Operation: GetNumberParams%956586099
      m4int_t GetNumberParams ();

      //## Operation: DeleteParam%956592445; C++
      m4return_t DeleteParam (m4pchar_t ai_szParamName);

      //## Operation: SetInt%956649480; C++
      m4return_t SetInt (m4pcchar_t ai_szParamName, const m4int_t ai_iValue);

      //## Operation: GetInt%956649481; C++
      m4return_t GetInt (m4pcchar_t ai_szParamName, m4int_t &ao_iValue);

      //## Operation: SetM4Return%958463329; C++
      m4return_t SetM4Return (m4pcchar_t ai_szParamName, m4return_t ai_retValue);

      //## Operation: GetM4Return%958463330; C++
      m4return_t GetM4Return (m4pcchar_t ai_szParamName, m4return_t &ao_retValue);

      //## Operation: Dump%958548779
      m4return_t Dump ();

      //## Operation: Recycle%968315705; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

      //## Operation: CanRecycle%968315706
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%968315707
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: CanDestroy%968315708
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

      //## Operation: Clear%968315709
      void Clear ();

      //## Operation: GetInt64%972896441; C++
      m4return_t GetInt64 (m4pcchar_t ai_szParamName, m4int64_t &ao_i64Value);

      //## Operation: SetInt64%972896442; C++
      m4return_t SetInt64 (m4pcchar_t ai_szParamName, const m4int64_t ai_i64Value);

      //## Operation: Serialize%972896443; C++
      m4return_t Serialize (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

      //## Operation: Deserialize%972896444; C++
      m4return_t Deserialize (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

  public:
    // Additional Public Declarations
      //## begin ClParamList%39044EBE03CF.public preserve=yes
      //## end ClParamList%39044EBE03CF.public

  protected:

    //## Other Operations (specified)
      //## Operation: Set%956586097; C++
      m4return_t Set (m4pcchar_t ai_szParamName, m4ServerVT *ai_poServerVT);

      //## Operation: Get%956586098; C++
      m4return_t Get (m4pcchar_t ai_szParamName, m4ServerVT *&ao_poServerVT);

    // Data Members for Associations

      //## Association: M4ServerBase::ParamList::<unnamed>%390450230011
      //## Role: ClParamList::m_ParamMap%39045023029B
      //## begin ClParamList::m_ParamMap%39045023029B.role preserve=no  protected: Param_map { -> 1VHAN}
      Param_map m_ParamMap;
      //## end ClParamList::m_ParamMap%39045023029B.role

    // Additional Protected Declarations
      //## begin ClParamList%39044EBE03CF.protected preserve=yes
      //## end ClParamList%39044EBE03CF.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iState%39AF81990309
      //## begin ClParamList::m_iState%39AF81990309.attr preserve=no  private: eParamListState {UA} 
      eParamListState m_iState;
      //## end ClParamList::m_iState%39AF81990309.attr

    // Additional Private Declarations
      //## begin ClParamList%39044EBE03CF.private preserve=yes
      //## end ClParamList%39044EBE03CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClParamList%39044EBE03CF.implementation preserve=yes
      //## end ClParamList%39044EBE03CF.implementation

};

//## begin ClParamList%39044EBE03CF.postscript preserve=yes
//## end ClParamList%39044EBE03CF.postscript

// Class ClParamList 

//## begin module%3904571402A1.epilog preserve=yes
//## end module%3904571402A1.epilog


#endif
