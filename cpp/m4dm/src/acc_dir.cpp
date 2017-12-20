//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             acc_dir.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             05/09/97
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
#include "m4stl.hpp"
#include "acc_dir.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "sizerutils.hpp"
//## end module.includes preserve=yes

//## begin module.additionalDeclarations preserve=yes
ClAccess *ClAccessDirectory::ms_pNihilAccess = 0;
//## end module.additionalDeclarations

///////////////////////////////////////////////////////////////////////////////
//
// ClAccessDirectory
//

ClAccessDirectory::ClAccessDirectory ()
{
//## begin ClAccessDirectory::ClAccessDirectory%-1932564562.body preserve=yes
	m_vpaccess.reserve(2);
//## end ClAccessDirectory::ClAccessDirectory%-1932564562.body
}

ClAccessDirectory::~ClAccessDirectory ()
{
//## begin ClAccessDirectory::~ClAccessDirectory%33037792.body preserve=yes
    _DeleteAll ();
//## end ClAccessDirectory::~ClAccessDirectory%33037792.body
}

ClAccess *ClAccessDirectory::operator [] (const ClAccessIndex ai_index)
{
//## begin ClAccessDirectory::operator []%-755511929.body preserve=yes
    if (MoveTo (ai_index))
        return m_vpaccess[Index];
    else
        return ms_pNihilAccess;
//## end ClAccessDirectory::operator []%-755511929.body
}

ClAccess &ClAccessDirectory::operator * (void)
{
	if (Index != M4DM_EOF_INDEX) {
		return *m_vpaccess[Index];
	}
	return *ms_pNihilAccess;
}

ClAccess *ClAccessDirectory::operator -> (void)
{
	if (Index != M4DM_EOF_INDEX) {
		return m_vpaccess[Index];
	}
        return ms_pNihilAccess;
}

// isnoteof

m4bool_t ClAccessDirectory::MoveTo (const ClAccessIndex ai_index)
{
//## begin ClAccessDirectory::MoveTo%605739753.body preserve=yes
    m4bool_t bisnoteof;

    // conversión a entero con signo para la comparación:
    if ( (m4int32_t)ai_index < (m4int32_t)Count ()) {
        Index = ai_index;
        bisnoteof = M4_TRUE;
    }
    else {
        Index = M4DM_EOF_INDEX;
        bisnoteof = M4_FALSE;
    }

    return bisnoteof;
//## end ClAccessDirectory::MoveTo%605739753.body
}

m4bool_t ClAccessDirectory::Begin (void)
{
//## begin ClAccessDirectory::Begin%85878526.body preserve=yes
    return MoveTo(0);
//## end ClAccessDirectory::Begin%85878526.body
}

m4bool_t ClAccessDirectory::End (void)
{
//## begin ClAccessDirectory::End%1865491638.body preserve=yes

    return MoveTo ( Count() - 1 );

//## end ClAccessDirectory::End%1865491638.body
}

m4bool_t ClAccessDirectory::Prev (void)
{
//## begin ClAccessDirectory::Prev%-1699235637.body preserve=yes

    return MoveTo ( Index - 1 );

//## end ClAccessDirectory::Prev%-1699235637.body
}

m4bool_t ClAccessDirectory::Next (void)
{
//## begin ClAccessDirectory::Next%-1130602295.body preserve=yes

    return MoveTo ( Index + 1 );

//## end ClAccessDirectory::Next%-1130602295.body
}

m4uint32_t ClAccessDirectory::Count (void)
{
//## begin ClAccessDirectory::Count%1038662385.body preserve=yes

    return m_vpaccess.size ();

//## end ClAccessDirectory::Count%1038662385.body
}

m4return_t ClAccessDirectory::_Add (ClAccess *ai_paccess)
{
//## begin ClAccessDirectory::_Add%590031896.body preserve=yes

    m_vpaccess.push_back (ai_paccess);

    return M4_SUCCESS;

//## end ClAccessDirectory::_Add%590031896.body
}

m4return_t ClAccessDirectory::_Remove (ClAccess *ai_paccess)
{
//## begin ClAccessDirectory::_Remove%-67675433.body preserve=yes
    accessvectorit start, end, it;

    start = m_vpaccess.begin();
    end   = m_vpaccess.end();

    it = find (start, end, ai_paccess);

	/* Bug 0099344
	Hay que controlar que exista lo que se borra
	*/
	if( it != m_vpaccess.end() )
	{
		m_vpaccess.erase(it);
	}

    return M4_SUCCESS;
//## end ClAccessDirectory::_Remove%-67675433.body
}

void ClAccessDirectory::_DetachAll (void)
{
//## begin ClAccessDirectory::_DeleteAll%-1262138170.body preserve=yes
    m_vpaccess.clear ();
	m_vpaccess.reserve(2);
//## end ClAccessDirectory::_DeleteAll%-1262138170.body
}

m4return_t ClAccessDirectory::_DeleteAll (void)
{
//## begin ClAccessDirectory::_DeleteAll%-1262138170.body preserve=yes
	// GILES & STUART:
    // Hacer el casting porque for_each devuelve
	// un puntero a un funcion ...
	//(void) for_each (m_vpaccess.begin(), m_vpaccess.end(), _s_f);
    // EDU:
    // gracias
    accessvectorit start, end, it;

    start = m_vpaccess.begin();
    end   = m_vpaccess.end();

    for (it = start; it != end; it++) {
        (*it)->Destroy ();  // llama al delete
    }

    m_vpaccess.clear ();

    return M4_SUCCESS;
//## end ClAccessDirectory::_DeleteAll%-1262138170.body
}

m4return_t ClAccessDirectory::SetAutoLoadMode (m4uint8_t ai_iautoload)
{
    m4return_t res = M4_SUCCESS;

    accessvectorit start, end, it;

    start = m_vpaccess.begin();
    end   = m_vpaccess.end();

    for (it = start; it != end; it++) {
        res = (*it)->SetAutoLoadMode (ai_iautoload);
        if (M4_ERROR == res)
            break;
    }

    return res;
}

m4return_t ClAccessDirectory::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClAccessDirectory);

	ao_size-=sizeof(m_vpaccess);
	return GetVectorSize(m_vpaccess, ao_size);
}
