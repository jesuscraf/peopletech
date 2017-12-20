 //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             recorset.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    At last the ClRecordSet !!
//
//
//==============================================================================

//## begin module.includes preserve=yes
//## end module.includes preserve=yes

#ifndef __RECORSET_HPP__
#define __RECORSET_HPP__

#include "m4dm_dll.hpp"
#include "arecset.hpp"
#include "register.hpp"
#include "m4stl.hpp"
#include "changelistener.hpp"


class ClNode;
class ClBlock_Channel;
class ClNotifier;
class ClAccessRecordSetBookMark;
class ClBlockReference;


class M4_DECL_M4DM ClAccessRecordSet : public ClRecordSet, public ClChangeListener
{
public:
    ClAccessRecordSet (ClAccess *ai_paccess)
		: ClRecordSet (ai_paccess, M4_FALSE), Current (ai_paccess)
	{
		Current.Filter.SetChangeListener( &m_oFilterChangeListener ) ;
	}

   ~ClAccessRecordSet () {}

    ClRegister_Current  Current;
    
    _ClRegister_Base *GetpRegister (void) const
    {
        return (_ClRegister_Base *)&Register;
    }

    void Attach (ClNode_Data *ai_pClNode_Channel/*, ClNotifier *ai_pnotifier = 0*/);
    void Detach (void);

    void FollowSimple (const ClAccessRecordSet &);  //no virtual

    ClRegTotalize &GetFooter(void);

    m4bool_t  IsEOF (void);    

    // Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    m4return_t Serialize(ClGenericIODriver& IOD);
    m4return_t DeSerialize(ClGenericIODriver& IOD);

    void SendBlockMark(const m4bool_t ai_bBranchMode);
    // Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Coloca el recordset con el mismo bloque activo que el recordset
    // pasado como argumento. Lo hace de forma que el access queda consistente.
    m4return_t Follow (const _ClRecordSet_Base &ai_recordset);
    
	//a extingur:
	m4return_t Follow (ClBlock_Channel_Proxy *ai_pb);

	//nueva:
	m4return_t Follow (const ClBlockReference &ai_br);

	m4return_t	GetLastLoadStatus( m4uint8_t &ao_riLastLoadStatus, m4uint32_t &ao_riLastLoadRows, m4uint32_t &ao_riLastLoadFetchs ) const ;
	m4return_t	SetLastLoadStatus( m4uint8_t ai_iLastLoadStatus, m4uint32_t ai_iLastLoadRows, m4uint32_t ai_iLastLoadFetchs ) ;

	m4uint64_t GetNodeTimeStamp( void ) const ;
	m4uint64_t GetBlockTimeStamp( void ) const ;
	m4uint64_t GetRecordTimeStamp( void ) const ;
	m4uint64_t GetCurrentTimeStamp( void ) const ;

	m4return_t GetSize(ClSize &ao_size);

private:
    //ClNode y ClAccess:
	virtual m4bool_t SetCurrentBlock (const ClBlockReference &ai_br);

    m4return_t _follow_s1 (const ClBlockReference &ai_br, m4uint32_t ai_index);

    //ClAccessRecordSetBookMark
    void _SetCurrentBlock (const ClBlockReference &ai_br);

	ClChangeListener	m_oFilterChangeListener;

friend class ClNode;
friend class ClAccess;
friend class ClAccessBookMark;
friend class ClOldAccessRecordSetBookMark;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClAccessRecordSet &ai_recset);

#endif // __RECORSET_HPP__ 
