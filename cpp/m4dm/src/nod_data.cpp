//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             node_dir.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             08/09/97
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
//----------------------------------------------------------------------------

#include "nod_data.hpp"
#include "node_knl.hpp"
#include "cldefine.hpp"
#include "px_block.hpp"
//#include "chan_knl.hpp"
#include "chandef.hpp"

#include "bnihil.hpp"
//## end module.includes preserve=yes


///////////////////////////////////////////////////////////////////////////////
//
// ClNodeDirectory_Data
//


ClNodeDirectory_Data::ClNodeDirectory_Data ()
{
    m_pbuffer = 0;
}

ClNodeDirectory_Data::~ClNodeDirectory_Data ()
{
    Destroy ();
}

void ClNodeDirectory_Data::Destroy (void)
{
    if (m_pbuffer) {
        delete []m_pbuffer;
        m_pbuffer = 0;
    }
}

m4return_t ClNodeDirectory_Data::Attach (ClChannel_Data *ai_pchannel, ClChannelDef *ai_pchandef)
{
    m4return_t res = M4_SUCCESS;

    M4_ASSERT (ai_pchannel);
    M4_ASSERT (ai_pchandef);

    m4uint16_t nnodes;
    nnodes = ai_pchandef->NumberOfNodes ();

    // provisionalmente alocamos aquí:
	if (nnodes == 0)
	{
		m_pbuffer = 0;
	}
	else
	{
		m_pbuffer = new ClNode_Data[nnodes];
		M4_ASSERT (m_pbuffer);
		ClNodeDirectory<ClNode_Data>::Attach (ai_pchandef, m_pbuffer);
	}

    ClNodeIndex i;    

    for (i = 0; i < nnodes; i++) {
        res = operator [](ClNodeIndex (i)).Init1 (ai_pchannel, i);
        if (res == M4_ERROR)
            return res;
    }

    for (i = 0; i < nnodes; i++) {
        res = operator [](ClNodeIndex (i)).Init2 (*this);
        if (res == M4_ERROR)
            return res;
    }

    //y otra pasadita más:
    for (i = 0; i < nnodes; i++) {
        operator [](ClNodeIndex (i)).InitBlockStore ();
    }

    return res;
}

//----------------------------------------------------------------------------
// Funciones cutres, todo lo que hacen es iterar y aplicar una función
// a ver cuando pones aquí un for_each + functor molón:
//----------------------------------------------------------------------------

// Serializacion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
m4return_t ClNodeDirectory_Data::SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
{
	const m4uint32_t n = Count();

    m4return_t res = M4_SUCCESS;

    for (ClNodeIndex i = 0; i < n; i++) {
        res = operator [](i).SetKeepStatus (ai_bKeepIt, ai_iVectorCount);
		if (res != M4_SUCCESS) break;
    }
	
    return res;
}

void ClNodeDirectory_Data::ResetSerialMarks(void)
{
    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++)
        operator [](i).ResetSerialMarks();
}
// Serializacion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

m4return_t ClNodeDirectory_Data::FindRecordAndSetVersion(const m4uint32_t ai_iRecordId,
                                                         const m4date_t   ai_dNewRecordVersion)
{
    m4bool_t bContinue = M4_TRUE;
    m4uint32_t n = Count ();
    
    for (ClNodeIndex i = 0; i < n && bContinue == M4_TRUE; i++) {
        operator [](i).FindRecordAndSetVersion(ai_iRecordId, ai_dNewRecordVersion, bContinue);
    }

    // Si no he encontrado el registro devuelvo error
    if (bContinue == M4_TRUE)
        return M4_ERROR;

    return M4_SUCCESS;
}

m4return_t ClNodeDirectory_Data::SetCheckPointMode (m4bool_t ai_bnewmode)
{
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).SetCheckPointMode (ai_bnewmode);
    }

    return res;
}

m4return_t ClNodeDirectory_Data::SetSliceMode (m4bool_t ai_bnewmode)
{
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).GetpExtInfo ()->ColItemDef_Kernel.SetSliceMode (ai_bnewmode);
    }

    return res;
}

m4return_t ClNodeDirectory_Data::InitSecurity (void)
{
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).GetpExtInfo ()->ColItemDef_Kernel.InitSecurity ();
        res = operator [](i).GetpExtInfo ()->InitSecurity ();
    }

    return res;
}

m4return_t ClNodeDirectory_Data::CheckPoint (void)
{
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).CheckPoint ();
        if (res != M4_SUCCESS)
            break;
    }

    return res;
}

m4return_t ClNodeDirectory_Data::Undo (void)
{
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).Undo ();
        if (res != M4_SUCCESS)
            break;
    }

    return res;
}

m4return_t ClNodeDirectory_Data::RefreshFlags (m4uint8_t &ao_operation)
{
    m4return_t res = M4_SUCCESS;
    m4uint8_t ioperation;

    ao_operation = M4DM_OPERATION_NONE;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).RefreshFlags (ioperation);
        if (res != M4_SUCCESS)
            break;
        ao_operation |= ioperation;
    }

    return res;
}


// se itera a lo largo de todos los nodos, para cada nodo:
//  - se borran todos los bloques
//  - se resetea el proxy (puesta a cero del m_pClBlock)

m4return_t ClNodeDirectory_Data::ReleaseAll (m4bool_t ai_bresetnodeitems, m4bool_t ai_bDestructorCall)
{
//## begin ClNodeDirectory_Data::ReleaseAll%964021265.body preserve=yes
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i, j;
    m4uint32_t n = Count ();
    ClNode_Data *pnc, * pnchild;
    m4uint32_t childs;

    for (i = 0; i < n; i++) {
        pnc = &(operator [](i));
        
        // delete de todos los channel_block en el block_dir:
        res = pnc->ReleaseAll (ai_bresetnodeitems);

        if (!ai_bDestructorCall)
        {
		    childs = pnc->GetpExtInfo ()->ChildNodeRB.Count ();
		    for (j = 0; j < childs; j++)
            {
			    pnchild = pnc->GetpExtInfo ()->ChildNodeRB[j].GetAddress ();
                if (pnchild->GetpChannel_Data() != pnc->GetpChannel_Data())
                {
                    pnchild->ReleaseDoc(ai_bresetnodeitems);
                }
            }
        }

        //pnc->RecordSet.SetCurrentBlock (&g_oNihilBlock);

        if (res == M4_ERROR)
            break;
    }

    return res;
//## end ClNodeDirectory_Data::ReleaseAll%964021265.body
}

m4return_t ClNodeDirectory_Data::CommitPersisted (VersionMark &ai_version)
{
//## begin ClNodeDirectory_Data::CommitPersisted%-1546385077.body preserve=yes
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).CommitPersisted (&ai_version);
        if (res != M4_SUCCESS)
            break;
    }

    return res;
//## end ClNodeDirectory_Data::CommitPersisted%-1546385077.body
}

m4return_t ClNodeDirectory_Data::RollbackPersisted (void)
{
//## begin ClNodeDirectory_Data::RollbackPersisted%1028596649.body preserve=yes
    m4return_t res = M4_SUCCESS;

    ClNodeIndex i;
    m4uint32_t n = Count ();

    for (i = 0; i < n; i++) {
        res = operator [](i).RollbackPersisted ();
        if (res != M4_SUCCESS)
            break;
    }

    return res;
//## end ClNodeDirectory_Data::RollbackPersisted%1028596649.body
}


// jcr
m4return_t	ClNodeDirectory_Data :: CheckModify( m4bool_t &ao_bModified )
{

    m4return_t	iResult = M4_SUCCESS ;
    m4uint32_t	n ;
    ClNodeIndex	i ;


	ao_bModified = M4_FALSE ;

	n = Count() ;
    for( i = 0 ; i < n ; i++ )
	{
        iResult = operator[]( i ).CheckModify( ao_bModified ) ;

        if( iResult != M4_SUCCESS || ao_bModified == M4_TRUE ) 
		{
            break ;
		}
    }

    return( iResult ) ;
}


m4return_t 
ClNodeDirectory_Data::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNodeDirectory_Data);

		ao_size-=sizeof(ClNodeDirectory<ClNode_Data>);
	if (ClNodeDirectory<ClNode_Data>::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pbuffer){
		// putadita.
		// Cuando se hace un new de un vector, se reserva un entero para indicar el tamaño.
		// Esto ocurre solo si la clase tiene destructor ( se utiliza para llamar al destructor de cada
		// objeto, cuando se hace delete [] ... )
		ao_size+=sizeof(int);
	}
//Clases Base

	return M4_SUCCESS;
}

/**
 * Hace un reset de los contadores de referencias de cada uno de los ClRTItemIterator de cada nodo.
 * Este método sólo debe llamarse desde el ClChannel::_DetachRecursive, para forzar una desconexión.
 * Con esto conseguimos que cuando se borre el canal no aparezca un assert indicando que el número
 * de referencias no es correcto (por no haber destruido los accesos).
 */
void ClNodeDirectory_Data::ResetRTItemIteratorsCountReferences(void)
{
	ClNode_Data* pNodeData = 0;
	ClNode_Kernel* pNodeKernel = 0;
	ClNodeIndex nodeCount = Count();
	for (ClNodeIndex nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {
		pNodeData = Get(nodeIndex);
		pNodeKernel = pNodeData->GetpNodeDef();
		pNodeKernel->ItemProxy_Array.ResetRTItemIteratorsCountReferences();
	}
}

