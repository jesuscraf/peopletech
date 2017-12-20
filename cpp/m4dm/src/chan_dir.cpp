//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             chan_dir.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             17/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

//## begin module.includes preserve=yes
//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
#include "m4stl.hpp"
//----------------------------------------------------------------------------

#include "chan_dir.hpp"
#include "channel.hpp"

#include "sizerutils.hpp"
//## end module.includes preserve=yes

///////////////////////////////////////////////////////////////////////////////
//
// ClChannelDirectory
//

ClChannelDirectory::ClChannelDirectory ()
{
//## begin ClChannelDirectory::ClChannelDirectory%68252199.body preserve=yes
//## end ClChannelDirectory::ClChannelDirectory%68252199.body
}

ClChannelDirectory::~ClChannelDirectory ()
{
//## begin ClChannelDirectory::~ClChannelDirectory%244449862.body preserve=yes
    Clear ();
//## end ClChannelDirectory::~ClChannelDirectory%244449862.body
}

ClChannel *ClChannelDirectory::operator [] (m4uint32_t ai_index)
{
//## begin ClChannelDirectory::operator []%-773314923.body preserve=yes
	return m_ListChannel[ai_index];
//## end ClChannelDirectory::operator []%-773314923.body
}
//## begin module.additionalDeclarations preserve=yes


//## end module.additionalDeclarations

m4return_t ClChannelDirectory::Attach (ClChannel *ai_oChannel, m4bool_t ai_bFirst)
{
	if( ai_bFirst == M4_TRUE )
	{
		m_ListChannel.insert(m_ListChannel.begin(), ai_oChannel);
	}
	else
	{
		m_ListChannel.push_back(ai_oChannel);
	}

    return M4_SUCCESS;
}

m4return_t ClChannelDirectory::Detach (ClChannel *ai_oChannel)
{
	LISTCHANNEL::iterator start = m_ListChannel.begin();
	LISTCHANNEL::iterator end   = m_ListChannel.end();
	
	LISTCHANNEL::iterator it = find (start, end, ai_oChannel);
	
	if (it == end) {
		return M4_WARNING;	// no se ha encontrado, puede que ya estuviera detachado
	}

	m_ListChannel.erase(it);

	// resets group dir
	// Se avisa a todos los canales que borren su relación de amistad con el canal que está saliendo del ChannelManager.
	_NotifyGroup(ai_oChannel, M4_TRUE);
	
	return M4_SUCCESS;
}

m4return_t ClChannelDirectory::Clear (void)
{
//## begin ClChannelDirectory::Clear%2016295409.body preserve=yes

    // the list is not the channels owner:
    /*
    while (m_ListChannel.size ()) {
        ClChannel* pChannel = *(m_ListChannel.begin());

        M4_ASSERT (pChannel);

		delete pChannel;
	}
    */

    m_ListChannel.clear ();

    return M4_SUCCESS;
//## end ClChannelDirectory::Clear%2016295409.body
}

m4uint32_t ClChannelDirectory::Count (void)
{
//## begin ClChannelDirectory::Count%663640981.body preserve=yes
    return m_ListChannel.size ();
//## end ClChannelDirectory::Count%663640981.body
}
 
void ClChannelDirectory::_NotifyGroup(ClChannel * const ai_pChannel, m4bool_t const ai_bDelete)
{
	if (ai_pChannel == NULL)
		return;

	ClChannel *pChannel = NULL;
    LISTCHANNEL::iterator iterator = m_ListChannel.begin();

	while( iterator != m_ListChannel.end() )
	{
		pChannel = *iterator ;

		if( pChannel != ai_pChannel )
		{
			if( ai_bDelete )
			{
				pChannel->m_oGroupInfo.DelPartner(ai_pChannel, M4_TRUE) ;
			}
			else
			{
				pChannel->m_oGroupInfo.AddPartner(ai_pChannel) ;
			}
		}
		iterator++;
	}
}

m4return_t ClChannelDirectory::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClChannelDirectory);
	
	ao_size-=sizeof(m_ListChannel);

	if (GetVectorSize(m_ListChannel,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4uint32_t ClChannelDirectory::GetNumberOfChannels()
{
	return (m4uint32_t) m_ListChannel.size();
}

