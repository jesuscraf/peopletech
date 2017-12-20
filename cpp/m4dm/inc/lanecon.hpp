//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             lanecon.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             31/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Conection between two channels (the lane)
//
//==============================================================================

#include "m4types.hpp"
#include "m4stl.hpp"
#include "handles.hpp"

#ifndef __LANECON__HPP__
#define __LANECON__HPP__

#include "m4dm_dll.hpp"
class ClChannel_Data;
class ClNode;
class ClLaneMannager;
class ClNode_Data;
class ClLaneManager;
class ClAccess;

class M4_DECL_M4DM ClLaneConnection
{
public:
    ClLaneConnection ();    // necesitan ser públicos para la <list> puede acceder
   ~ClLaneConnection ();

    ClNode *GetpDocNode (void)
    {
        return Doc.pClNode;
    }

    ClNode *GetpLaneNode (void)
    {
        return Lane.pClNode;
    }

    m4uint16_t NumberOfItemsConnected (void);

    ClHandle GetChildItemHandle (m4uint16_t ai_index);

    m4pcchar_t GetFatherItemName (m4uint16_t ai_index);

    m4uint8_t Type (void);

    m4bool_t ChannelIsEqual (ClNode *ai_pndoc, ClNode *ai_pnlane);
    m4bool_t AccessIsEqual  (ClNode *ai_pndoc, ClNode *ai_pnlane);

    // estoy hasta los guevos del VC++
    m4bool_t operator == (const ClLaneConnection &ai_accesscon) const;
    m4bool_t operator != (const ClLaneConnection &ai_accesscon) const;
    m4bool_t operator >  (const ClLaneConnection &ai_accesscon) const;
    m4bool_t operator <  (const ClLaneConnection &ai_accesscon) const;

    m4return_t Init (ClChannel_Data *ai_pchdoc,  ClNode *ai_pndoc,
                     ClChannel_Data *ai_pchlane, ClNode *ai_pnlane);
    m4return_t Init (ClNode *ai_pndoc, ClNode *ai_pnlane, ClLaneConnection * ai_poCon);

    m4return_t Destroy (void);
    m4return_t DestroyTemp (void);

// Contandor del numero de referencias a esta conexion. Si hay mas de una, cuando se
// se destruye no se puede desconectar el nodo documento.
    m4uint32_t * GetpRefConections(void);
    void DecRefConections(void);

    m4uint32_t * m_piRefConections;

    struct StOneSidePlugInfo
    {
        ClChannel_Data*      pClChannel;
        ClNode * pClNode;
		m4return_t GetSize(ClSize &ao_size){
			ao_size+=sizeof(StOneSidePlugInfo);
			return M4_SUCCESS;
		}

    };

	m4return_t GetSize(ClSize &ao_size);

    StOneSidePlugInfo Lane;    // el padre (carril)
    StOneSidePlugInfo Doc;     // el hijo (documento)

private:

    m4uint32_t m_hPlug; // handle del plug del cmcr
};

#endif // __LANECON__HPP__
