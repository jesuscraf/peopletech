//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ndir_knl.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             24/05/98
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
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "handles.hpp"
#include "anoddir.hpp"
#include "dm_types.hpp"
#include "node_knl.hpp"
//## end module.includes preserve=yes


#ifndef __NOD_DATA_HPP__
#define __NOD_DATA_HPP__

class ClChannel_Data;
class ClCompiledMCR;
class ClSize;


class M4_DECL_M4DM ClNodeDirectory_Data : public ClNodeDirectory<ClNode_Data>
{
public:
    ClNodeDirectory_Data ();
   ~ClNodeDirectory_Data ();

    void Destroy (void);
    m4return_t Attach (ClChannel_Data *ai_pchannel, ClChannelDef *ai_pchandef);
    m4return_t SetCheckPointMode (m4bool_t ai_bnewmode);
    m4return_t SetSliceMode (m4bool_t ai_bnewmode);

    m4return_t InitSecurity (void);

    m4return_t RefreshFlags (m4uint8_t &ao_operation);
    m4return_t CommitPersisted (VersionMark &ai_version);
    m4return_t RollbackPersisted (void);

    m4return_t FindRecordAndSetVersion(const m4uint32_t ai_iRecordId,
                                       const m4date_t   ai_dNewRecordVersion);

// jcr
    m4return_t CheckModify( m4bool_t &ao_bModified ) ;

    // Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    /**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
    m4return_t SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount);

	/** Borra las marcas de la serialización avanzada. */
    void ResetSerialMarks(void);
    // Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    //m4return_t SetAutoLoadMode (m4uint32_t ai_iautoload);

    m4return_t ReleaseAll (m4bool_t ai_bresetnodeitems, m4bool_t ai_bDestructorCall = M4_FALSE);
    m4return_t CheckPoint (void);
    m4return_t Undo (void);

	m4return_t GetSize(ClSize &ao_size);

	/**
	 * Hace un reset de los contadores de referencias de cada uno de los ClRTItemIterator de cada nodo.
	 * Este método sólo debe llamarse desde el ClChannel::_DetachRecursive, para forzar una desconexión.
	 * Con esto conseguimos que cuando se borre el canal no aparezca un assert indicando que el número
	 * de referencias no es correcto (por no haber destruido los accesos).
	 */
	void ResetRTItemIteratorsCountReferences(void);

private:
    ClNode_Data *m_pbuffer;
};

#endif // __NOD_DATA_HPP__

