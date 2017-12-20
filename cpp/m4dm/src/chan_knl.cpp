//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             channel.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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
#include <errno.h>

#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "chan_knl.hpp"
#include "access.hpp"
#include "node_knl.hpp"
#include "clgeniod.hpp"
#include "px_block.hpp"
#include "checkmem.hpp"
#include "datacache.hpp"
#include "not_cdat.hpp"
#include "prxyfact.hpp"
#include "memman.hpp"
#include "file_manager.hpp"
#include "nodedef.hpp"
#include "m4mdrt.hpp"




///////////////////////////////////////////////////////////////////////////////
//
// ClChannel_Kernel
//

//----------------------------------------------------------------------------
// Constructor / Destructor
//----------------------------------------------------------------------------

ClChannel_Kernel::ClChannel_Kernel ()
{
    #ifdef _DEBUG
        g_StMemInfo.iChannel++;
    #endif

    m4VariantType var;

}

//----------------------------------------------------------------------------
// _InitCommon
//----------------------------------------------------------------------------

//el destructor sólo es accesible desde el ClChannel
//y desde mi Destroy
//el proceso de borrado de un ClChannel_Kernel es
//complicado
ClChannel_Kernel::~ClChannel_Kernel ()
{
    #ifdef _DEBUG
        g_StMemInfo.iChannel--;
    #endif

    Destroy ();
}


// susrutina que se llama desde el destructor:
m4return_t ClChannel_Kernel::Destroy (void)
{
    //ahora sí destruimos el mcr:
    MCR.Destroy ();
    return M4_SUCCESS;
}




//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------




//////////////////////////////////////////////////////////////////////////////
//
// ClChannel_Data
//

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ClChannel_Data::ClChannel_Data ()
: ClPersistiableObject ( 0 )
{
//Serializacion
    // Por defecto, cuando hagamos la serializacion, vamos a decir al remoto que 
    // no utilize deltas (trabajamos con absolutos)
    m_KeepStatusOnRemote = M4_FALSE;
	m_KeepStatus = M4_FALSE;

    m_pClChannel_Kernel = new ClChannel_Kernel;
    M4_ASSERT (m_pClChannel_Kernel);
    m_pProxyFactory = new ClProxyFactory;

	m_pCO = 0;

//notifier:
    m_pClNotifier = new ClNotifier_Channel_Data;
    M4_ASSERT (m_pClNotifier);

// UniqueRowId
    m_iUniqueRowId = 0;

// Gestor De Memoria
    m_poMemoryManager = new ClMemoryManager();

// Carga parcial
	*m_acPLFile = '\0';
}


ClChannel_Data::~ClChannel_Data ()
{
//aviso a la gente:
    m_pClNotifier->DestroyChannelData_Before ();

    ReleaseAll (M4_FALSE, M4_TRUE);

    //ResetCMCR ();
    
    GetpExtInfo ()->Destroy ();

//kernel:
    M4_ASSERT (m_pClChannel_Kernel);
    delete m_pClChannel_Kernel;
    m_pClChannel_Kernel = 0;

//destruyo los nodos antes del notifier (sí, se mandan eventos)
    M4_ASSERT (m_pClNotifier);
    Node.Destroy ();

//notifier:
    M4_ASSERT (m_pClNotifier);
    delete m_pClNotifier;
    m_pClNotifier = 0;

    delete m_pProxyFactory;

// Gestor De Memoria
    delete m_poMemoryManager;

// Carga parcial
// Al salir hay que liberar el fichero asociado a la carga parcial
	if( *m_acPLFile != '\0' )
	{
		ClFileManager::Instance()->UnSetInterestInFile( m_acPLFile ) ;
	}

}

m4return_t ClChannel_Kernel::Init (void)
{
    m4return_t res;

    //precondición: que el MCR esté ya inicializado
    M4_ASSERT (MCR.IsBuilt ());

    //En la clase ClChannel se ha hecho parte de inicialización: la
    //de las fechas que en un caso hay que cogerlas del LogonAdap.
    // esto antes que nada, en las siguientes lineas se utiliza:
    res = ClChannelDef::Attach (MCR.GetpCMCR ());

    //Version.Set (MCR.GetpCMCR ()->GetVersion ());

    return res;
}

//Llamada por ClChannel:
m4return_t ClChannel_Data::Init (void)
{
    m4return_t res;

    res = GetpExtInfo ()->Init ();
    if (res == M4_ERROR)
        return res;

	if( m_pProxyFactory != NULL && m_pClChannel_Kernel->GetpCMCR()->GetChannelIsCacheable() == 1 )
	{
		m_pProxyFactory->ForceLock();
	}

    //
    res = Node.Attach (this, GetpChannelDef ());
    if (res == M4_ERROR)
        return res;

//después de haber inicializado el nodo:
    res = SetSliceMode (M4_TRUE);   //por defecto a true:

//Inicializacion de la seguridad del nodo:
    if (res == M4_SUCCESS)
        res = InitSecurity ();

    return res;
}


m4return_t ClChannel_Data::Destroy (m4bool_t ai_bRemoveFromCache)
{
	// Si esta en la cache (m_pCO != NULL) quita una referencia al objecto,
	// ademas si ai_bRemoveFromCache es TRUE, se elimina el objeto de la cache y se le pasa a la cola de borrado.
	// Si no esta en la cache, lo destruye directamente.

	if( m_pCO != NULL )
    {
		if (ai_bRemoveFromCache == M4_TRUE)
		{
			((ClChannelDataContainer*)m_pCO)->RemoveObjectFromCache();
		}
		((ClChannelDataContainer*)m_pCO)->FreeObjectFromCache();
    }
	else
	{
		//me suicido
		delete this;
	}

    return M4_SUCCESS;
}

//***********************************************
// FLAGS DE OPERACION

m4return_t ClChannel_Data::RefreshFlags (void)
{
    m4uint8_t ioperation = M4DM_OPERATION_NONE;
    m4return_t res;

    res = Node.RefreshFlags (ioperation);

    Operation.Reset();
    Operation.Set(ioperation);

    return res;
}

void ClChannel_Data::_NotifyOperation (m4uint8_t ai_operation)
{
    Operation.Set (ai_operation);
    // termina la cadena de propagación
}

m4return_t ClChannel_Data::CheckPoint (void)
{
    m4return_t res = Node.CheckPoint ();

    if (res != M4_ERROR)
        RefreshFlags();

    return res;
}

m4return_t ClChannel_Data::Undo (void)
{
    m4return_t res = Node.Undo ();
    if (res != M4_ERROR)
        RefreshFlags();

    return res;
}

m4return_t ClChannel_Data::CommitPersisted (VersionMark &ai_version)
{
	// pase lo que pase reseteamos el flag del canal
    m4return_t res = Node.CommitPersisted (ai_version);
    if (res != M4_ERROR)
        RefreshFlags();

    return res;
}

m4return_t ClChannel_Data::RollbackPersisted (void)
{
    m4return_t res = Node.RollbackPersisted ();
    if (res != M4_ERROR)
        RefreshFlags();

    return res;
}

// jcr
m4return_t	ClChannel_Data :: CheckModify( m4bool_t &ao_bModified )
{
	ao_bModified = M4_FALSE ;
    return( Node.CheckModify( ao_bModified ) ) ;
}


//***********************************************

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

// Borra todos los datos del canal, repito: sólo datos
// ai_bresetnodeitems: por defecto a true:

m4return_t ClChannel_Data::ReleaseAll (m4bool_t ai_bresetnodeitems, m4bool_t ai_bDestructorCall)
{
    m4return_t res;

    Operation.Reset ();

    GetpNotifier ()->ReleaseAll_Before ();

    res = Node.ReleaseAll (ai_bresetnodeitems, ai_bDestructorCall);
    
    if (M4_SUCCESS == res) {
        //historia truculenta donde las haya
        //Nos acabamos de cepillar todos los proxys, pero si consideramos el
        //carril, tenemos que crear tantos proxys como registros haya en el mismo,
        //pq sino caemos en inconsistencias

        //esto lo solucionamos cuando nos carguemos los proxys
    }

    return res;
}

m4return_t ClChannel_Data::SetCheckPointMode (m4bool_t ai_bnewmode)
{
    return Node.SetCheckPointMode (ai_bnewmode);
}

m4return_t ClChannel_Data::SetSliceMode (m4bool_t ai_bnewmode)
{
    return Node.SetSliceMode (ai_bnewmode);
}


void ClChannel_Data::ForceLock( void )
{
	if( m_pProxyFactory != NULL )
	{
		m_pProxyFactory->ForceLock() ;
	}
}


// ============================================================================
// Carga parcial
// ============================================================================

m4pcchar_t  ClChannel_Data::GetPLFile( void ) const
{
	return( m_acPLFile ) ;
}


/*
Crea el fichero asociado a este bloque
*/
m4return_t  ClChannel_Data::CreatePLFile( const ClNodeDef *ai_pcoNodeDef, m4pcchar_t& ao_rpPlFile )
{

	m4return_t	iResult = M4_ERROR ;
	ClFileName	oName ;


	ao_rpPlFile = NULL ;

	if( *m_acPLFile == '\0' )
	{
		iResult = ClFileManager::Instance()->CreateTempFile( oName, "~pl", "chn" ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_NO_OPEN_PL_FILE, "~pl*.chn" << "w" << strerror( errno ) << M4ObjNodeN( ai_pcoNodeDef->GetpCMCR(), ai_pcoNodeDef->GetHandle() ) ) ;

		oName.GetFileName( m_acPLFile ) ;
	}

	ao_rpPlFile = m_acPLFile ;

	return( M4_SUCCESS ) ;
}



//----------------------------------------------------------------------------
//Seguridad
//----------------------------------------------------------------------------

m4return_t ClChannel_Data::InitSecurity (void)
{
    return Node.InitSecurity ();
}


//para la cache:

m4uint32_t ClChannel_Data::GetSize (void)
{
	ClSize iSize;
    GetSize(iSize);

	return iSize;
}

m4return_t 
ClChannel_Data::GetSize(ClSize &ao_size)
{

	ao_size += sizeof(ClChannel_Data);
// Clases Base.
/*	ao_size -= sizeof(ClPersistiableObject);

	if (ClPersistiableObject::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
*/

// Objetos.
		ao_size -= sizeof(Operation);
	if (Operation.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

//		ao_size -= sizeof(ProxyFactory);
	if (m_pProxyFactory->GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

		ao_size -= sizeof(Node);
	if (Node.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

// Referencias a objetos

	if (m_pClChannel_Kernel){
		if (m_pClChannel_Kernel->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	if (m_pClNotifier){
		if (m_pClNotifier->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}


	return M4_SUCCESS;
}


m4return_t 
ClChannel_Kernel::GetSize(ClSize &ao_size)
{
	m4return_t ret =M4_SUCCESS;

//  Propiedades
	ao_size += sizeof(ClChannel_Kernel);
	
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}

//Clases Base
		ao_size -= sizeof(ClChannelDef);
	if (ClChannelDef::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

//Objetos
		ao_size -= sizeof(MCR);
	if (MCR.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
