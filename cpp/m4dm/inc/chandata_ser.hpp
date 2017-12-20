//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                chandata_ser.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC 
//
//
// Definition:
//
//    This module defines the channel data serialization class
//
//
//==============================================================================


#ifndef __CHANDATA_SER_HPP__
#define __CHANDATA_SER_HPP__

#include "m4var.hpp"
#include "m4dm_dll.hpp"

class ClCMCRFactory;
class ClCompiledMCR;
class ClLaneArgumnent; 
class ClChannel_Data;


class M4_DECL_M4DM ClChannelDataSerializer
{
public:
	ClChannelDataSerializer(void);
	~ClChannelDataSerializer(void);

	m4return_t Serialize(ClGenericIODriver& IOD, ClChannel_Data* ai_poChannelData, m4bool_t ai_bSerializeMCR, m4uint8_t ai_iRecSerMod, m4uint8_t ai_iIteSerMod);
	m4return_t DeSerialize(ClGenericIODriver& IOD, ClCMCRFactory *ai_poFactory, ClCompiledMCR *ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data* ao_poChannelData);

	m4uint8_t	GetSerializeMCR( void ) const
	{
		return( m_iSerializeMCR ) ;
	}

private:

	void Reset();
	m4return_t Init( ClChannel_Data* ai_poChannelData, m4bool_t ai_bSerializeMCR );
	m4return_t Init( m4uint8_t ai_iSerializeMCR, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, m4pcchar_t ai_pccMask, 
					 m4date_t ai_dVersion, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4date_t ai_dCorDate);

	m4return_t DeSerializeInfoData(ClGenericIODriver& IOD);
	m4return_t SerializeInfoData(ClGenericIODriver& IOD);

	m4uint8_t m_iSerializeMCR;
	m4pchar_t m_pcChannelId;
	m4uint8_t m_iCsType;
	m4uint8_t m_iLanguage;
	m4pchar_t m_pcMask;
	m4date_t m_dVersion;
	m4date_t m_dStartDate;
	m4date_t m_dEndDate;
	m4date_t m_dCorDate; 
};
	
#endif

