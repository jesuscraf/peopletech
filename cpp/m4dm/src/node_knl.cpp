#include "dsres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_node.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             04/09/97
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

#include "node_knl.hpp"

#include "blockref.hpp"
#include "bnihil.hpp"
#include "ch_block.hpp"
#include "chan_knl.hpp"
#include "checkmem.hpp"
#include "chlog.hpp"
#include "clbsearch.hpp"
#include "cldefine.hpp"
#include "clgeniod.hpp"
#include "colitemdefit.hpp"
#include "compst_common.hpp"
#include "compst_fac.hpp"
#include "compstit_common.hpp"
#include "computil.hpp"
#include "conitinf.hpp"
#include "emd.hpp"
#include "gds.hpp"
#include "itemconn.hpp"
#include "itemdfit.hpp"
#include "item.hpp"
#include "lanecon.hpp"
#include "mchtoemd.hpp"
#include "not_cdat.hpp"
#include "px_block.hpp"
#include "rec_tran.hpp"
#include "sizerutils.hpp"
#include "m4mdrt.hpp"

#include "node_x.hpp"
#include "access.hpp"

//## end module.includes preserve=yes

ClRegister_Insert       ClNode_Kernel::ms_ClRegInsert;
ClRegister_InsertNihil  ClNode_Kernel::ms_ClRegInsertNihil;

ClRegister_Delete       ClNode_Kernel::ms_ClRegDelete;
ClRegister_DeleteNihil  ClNode_Kernel::ms_ClRegDeleteNihil;


///////////////////////////////////////////////////////////////////////////////
//
// ClNodeParent
//

ClNodeParent::ClNodeParent ()
{
    m_pNotFree = 0;
	m_vpparents.reserve(1);
}

ClNodeParent::~ClNodeParent ()
{
    parentvectorit start, end, it;

    start = m_vpparents.begin();
    end   = m_vpparents.end();

    for (it = start; it != end; it++) {
        delete (*it);
    }

    m_vpparents.clear ();
    m_pNotFree = 0;
}

ClNode_Connector &ClNodeParent::operator [] (m4uint32_t ai_index)
{
    M4_ASSERT (m_vpparents.size () > ai_index);

    return *(m_vpparents [ai_index]);
}

void ClNodeParent::Add (ClNode_Data *ai_pParentNode, ClHandle  ai_handle, m4uint8_t ai_itype)
{
    ClNode_Connector *p = new ClNode_Connector (ai_pParentNode, ai_handle, ai_itype);
    m_vpparents.push_back (p);

    if (ai_itype != M4CL_NODE_CONNECTOR_TYPE_FREE) {
// jcr. Esto ya no debería pasar nunca porque lo controlo yo al hacer el build
//        M4_ASSERT (0 == m_pNotFree);
        m_pNotFree = p;
    }
}

void ClNodeParent::Remove (ClNode_Data *ai_pParentNode)
{
    parentvectorit start, end, it;

    start = m_vpparents.begin();
    end   = m_vpparents.end();

    for (it = start; it != end; it++)
    {
        if ((*it)->GetAddress() == ai_pParentNode)
        {
            delete (*it);
            m_vpparents.erase(it);
            break;
        }
    }
/*
    if (Count())
    {
        p = m_vpparents.back ();
        m_pNotFree = p;
    }
    else
*/
// En principio deberia quedarse a 0 porque sino, no habria podido añadir un padre.
    m_pNotFree = 0;
}

m4uint32_t ClNodeParent::Count (void)
{
    return m_vpparents.size ();
}

///////////////////////////////////////////////////////////////////////////////
//
// ClNode_Kernel
//

//-----------------------------------------------------------------------------
// Constructor / Destructor
//-----------------------------------------------------------------------------

ClNode_Kernel::ClNode_Kernel ()
{
    #ifdef _DEBUG
        g_StMemInfo.iNode_Channel++;
    #endif

	ItemProxyMutex = NULL;
	m_bIsLaned = M4_FALSE;
}

ClNode_Kernel::~ClNode_Kernel ()
{
    #ifdef _DEBUG
        g_StMemInfo.iNode_Channel--;
    #endif

	if( ItemProxyMutex != NULL )
	{
		delete ItemProxyMutex;
		ItemProxyMutex = NULL;
	}
}

//-----------------------------------------------------------------------------
// Seguridad
//-----------------------------------------------------------------------------
m4return_t ClNode_Kernel::InitSecurity(void)
{
    if ((SecWrite())&&(NumRows() == 0))
        m_pRegInsert = &ms_ClRegInsert;
    else
        m_pRegInsert = &ms_ClRegInsertNihil;

    if ((SecDelete())&&(NumRows() == 0))
        m_pRegDelete = &ms_ClRegDelete;
    else
        m_pRegDelete = &ms_ClRegDeleteNihil;

    return M4_SUCCESS;
}

_ClRegister_Insert * ClNode_Kernel::GetpRegInsert(void)
{
    return m_pRegInsert;
}

_ClRegister_Delete * ClNode_Kernel::GetpRegDelete(void)
{
    return m_pRegDelete;
}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------

m4return_t ClNode_Kernel::Init1 (ClCompiledMCR *ai_pCMCR, ClNodeIndex ai_index)
{
    m4return_t res;

//Index, me lo guardo como un atajo
    Index = ai_index;

// nodedef:
    ClNodeDef::Attach (ai_pCMCR, ai_index);

//inicializamos las búsquedas:
    res = ClSearchIndex_List::Init (this, ai_pCMCR);

    return res;
}

m4return_t ClNode_Kernel::InitItemProxy_Array (void)
{
    m4return_t res;

	if( ClNodeDef::GetpCMCR()->GetChannelIsCacheable() == 1 )
	{
		ItemProxyMutex = new ClMutex( M4_TRUE );
	}

//ahora el array de proxys:
    res = ItemProxy_Array.Init (*this, ClNodeDef::GetpCMCR (), &ColItemDef_Kernel);

    return res;
}

m4return_t ClNode_Kernel::Init2 (ClNodeDirectory_Data &ai_NodeDir)
{
    m4return_t res;

//1, inicialización de los arrays de punteros a los hijos
    ChildNodeRB._Init (ClNodeDef::GetNodeRBFirstConnector (), ClNodeDef::GetNodeNumberOfRBConnectors (), ai_NodeDir, *this);
    ChildNodeBB._Init (ClNodeDef::GetNodeBBFirstConnector (), ClNodeDef::GetNodeNumberOfBBConnectors (), ai_NodeDir, *this);

//inicialización de la información del padre:
    m4uint16_t nic;
    nic = ClNodeDef::GetNodeNumberOfInvConnectors ();

    m4uint16_t nic_notfree = 0;

    if (nic) {
        // se asume que el conector activo es el 0:
        m4uint16_t i = 0;
        ClHandle hcon;
        ClHandle hn;
        ClNode_Data *pfather;
        m4uint8_t itype;

        do {
            hcon = ClNodeDef::GetNodeInvConnectorHandleByPosition (i);            
            M4_ASSERT (hcon);

            hn = ClNodeDef::GetNodeConnectorFatherNodeHandle (hcon);
            M4_ASSERT (hn);

            pfather = ai_NodeDir.Get (hn);            
            itype = ClNodeDef::GetNodeConnectorType (hcon);

            //dentro se comprueba que sólo tiene un padre no-free:
            Parent.Add (pfather, hcon, itype);

            nic--;
            i++;
        } while (nic);
    }

//2, incialización del nodedef, 2ª parte (se inicializa el ColItemDef)
    // se necesita que ChildNodeRB y ChildNodeBB ya estén inicializados.
    //aquí se requiere que la colección de nodos del canal este montada

    res = ColItemDef_Kernel.Init (*this, ai_NodeDir);
    if (res == M4_ERROR) {
        return res;
    }
    
    return InitItemProxy_Array ();
}

m4bool_t ClNode_Kernel::IsFree (void)
{
    //si no tenemos entradas en ParentConnector, es que somos free, y viceversa.

    return (Parent.GetpNotFree ())? M4_FALSE : M4_TRUE;
}

m4bool_t ClNode_Kernel::IsFree_ForgetingLanes (void)
{
	ClNode_Connector* pNodeConector = Parent.GetpNotFree();
    if (pNodeConector) {
        //tengo en cuenta el tema del carril, aunque me diga que no es free, 
        //no me fio, si el padre es de otro canal, digo que es free:
        ClNode_Data *pnd = pNodeConector->GetAddress ();
        M4_ASSERT (pnd);

		// Si los metacanales de los dos nodos son el mismo, entonces no es free
		// (los dos nodos pertenecen al mismo canal)
        //damos un rodeo de tres pueblos:
        if (pnd->GetpChannel_Data()->GetpChannelDef()->GetpCMCR() == ClNodeDef::GetpCMCR())
            return M4_FALSE;
    }

    return M4_TRUE;
}

//----------------------------------------------------------------------------
// er carri
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void ClNode_Data::_NotifyOperation (m4uint8_t ai_operation)
{
    if (Operation.Set (ai_operation))   // devuelve true si es necesario seguir propagando
        m_pClChannel_Data->_NotifyOperation (ai_operation);
}

ClRecordWithTransactions *ClNode_Data::GetpRWT (void)
{
    //de momento a cañon
    return m_poRecordWithTransaction;
}


///////////////////////////////////////////////////////////////////////////////
//
// ClNode_Data
//

ClNode_Data::ClNode_Data () :
    m_pSMDataNodo(NULL),
    m_pSMDataBlock(NULL),
    m_pSMDataRecord(NULL)
{
    m_poRecordWithTransaction = new ClRecordWithTransactions();
    //m_pProxy = 0;
    m_pBlockStore = 0;
    //m_pProxyMonoBlock = 0;
    m_pClNode_Kernel = 0;
    
    m_bNeedSerial = M4_FALSE;
    m_bBranchMode = M4_FALSE;

    Operation.Attach(this);
}

ClNode_Data::~ClNode_Data ()
{
    //no!, si estoy encarrilado no he destruido todos
    /*
    if (m_pBlockStore->GetType () == _AClComponentStore::MONO) {
        //me cepillo todos los bloques:
        ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneFast);
        m_pBlockStore->RemoveAll ();
    }
    else {
        //tenemos que haber llamado antes al ReleaseAll del canal
        M4_ASSERT (0 == GetpBlockStore ()->Count ());            
    }
    */

    //me cepillo todos los bloques:
    ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneFast);
    m_pBlockStore->RemoveAll ();

    delete m_pBlockStore;   //sí, no borra los bloques
    m_pBlockStore = 0;


    //borro el kernel:
    if (GetpExtInfo ()) {
        delete GetpExtInfo ();
        m_pClNode_Kernel = 0;
    }

    delete m_poRecordWithTransaction;

    // Borro los StaticMetaData de nodo, bloque, y registro
    ClExtendedMetaData::DestroyMetaData(m_pSMDataRecord);
    ClExtendedMetaData::DestroyMetaData(m_pSMDataBlock);
    ClExtendedMetaData::DestroyMetaData(m_pSMDataNodo);
}

m4return_t ClNode_Data::Init1 (ClChannel_Data *ai_pClChannel_Data, ClNodeIndex ai_nodeindex)
{
    m4return_t res;

    M4_ASSERT (ai_pClChannel_Data);
    m_pClChannel_Data = ai_pClChannel_Data;

    m_pClNode_Kernel = new ClNode_Kernel ();
    M4_ASSERT (GetpExtInfo ());
    res = GetpExtInfo ()->Init1 (m_pClChannel_Data->GetpChannelDef ()->GetpCMCR (), ai_nodeindex);

    return res;
}

m4return_t ClNode_Data::Init2 (ClNodeDirectory_Data &ai_nodedir)
{
    ClNode_Kernel* pExtInfo = GetpExtInfo();
    M4_ASSERT (pExtInfo);

    m4return_t res = pExtInfo->Init2 (ai_nodedir);
    
    // Creamos los StaticMetaData. Uno para los items a nivel de nodo,
    // otro para los items a nivel bloque y otro para los items a nivel de registro
    ClMetaChannelToExtendedMetaData convertidor;
    
    ClCompiledMCR* pMetaChannel = pExtInfo->GetpCMCR();
    m4uint32_t     iNodeHandle  = pExtInfo->GetHandle();

    if (res == M4_SUCCESS)
        res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                      iNodeHandle,
                                                                      M4CL_ITEM_SCOPE_NODE,
                                                                      m_pSMDataNodo);
    M4_ASSERT(m_pSMDataNodo != NULL);

    if (res == M4_SUCCESS)
        res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                      iNodeHandle,
                                                                      M4CL_ITEM_SCOPE_BLOCK,
                                                                      m_pSMDataBlock);
    M4_ASSERT(m_pSMDataBlock != NULL);

    if (res == M4_SUCCESS)
        res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                      iNodeHandle,
                                                                      M4CL_ITEM_SCOPE_REGISTER,
                                                                      m_pSMDataRecord);
    M4_ASSERT(m_pSMDataRecord != NULL);

    if (res == M4_SUCCESS)
    {
        m_poRecordWithTransaction->SetpMemoryManager(GetpChannel_Data()->GetpMemoryManager());
        m_poRecordWithTransaction->Init(m_pSMDataNodo);
    }

    return res;
}

//-----------------------------------------------------------------------------
//inicializamos el blockstore, dependiendo del tipo de nodo:
//-----------------------------------------------------------------------------


void ClNode_Data::InitBlockStore (void)
{
    M4_ASSERT (m_pBlockStore == 0); //tenemos que estar limpitos

	ClComponentStore_Factory::CreateComponentStore (this);
/*
    //si el nodo el free, o las relaciones que mantiene hasta llegar a su padre son todas BB,
    //entonces sólo va a tener un bloque:
    ClNode_Data *pnd = this;
    m4bool_t bmono;
    ClNode_Connector *pnc;

    for (;;) {
        if (pnd->GetpExtInfo ()->IsFree ()) {
            bmono = M4_TRUE;
            break;
        }

        //tiene que tener un padre:
        pnc = pnd->GetpExtInfo ()->Parent.GetpNotFree ();
        M4_ASSERT (pnc);
        if (pnc->GetType () != M4CL_NODE_CONNECTOR_TYPE_BB) {
            bmono = M4_FALSE;
            break;
        }

        //continuamos ...
        pnd = pnc->GetAddress ();
        M4_ASSERT (pnd);
    }

    if (bmono) {
        m_pBlockStore = new ClComponentStore_Mono ();
    }
    else {
        //queda pendiente una optimización, hacer un store especializado en relaciones BB
        m_pBlockStore = new ClComponentStore_RB ();
    }
*/
    M4_ASSERT (m_pBlockStore);
}

void ClNode_Data::ResetBlockStore (void)
{
    if (m_pBlockStore) {    //podemos tener más de una inicialización --> tema carril

        //DeleteBlock:
        ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneFast);
        m_pBlockStore->RemoveAll ();

        delete m_pBlockStore;
        m_pBlockStore = 0;
    }
}

m4return_t ClNode_Data::SetCheckPointMode (m4bool_t ai_bnewmode)
{
    m4return_t res;

    res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::SetCheckPointModeOne, &ai_bnewmode);

    if (res != M4_ERROR)
    {
        res = m_poRecordWithTransaction->UseCheckPoints (ai_bnewmode);
    }

    return res;
}

m4return_t ClNode_Data::CheckPoint (void)
{
    m4return_t res;

    res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::CheckPointOne);

    if (res != M4_ERROR)
    {
        res = m_poRecordWithTransaction->CheckPoint ();
        if (res != M4_ERROR) {
            Operation.Reset ();
        }
    }

    GetpChannel_Data()->RefreshFlags();

    return res;
}

m4return_t ClNode_Data::Undo (void)
{
    m4return_t res;

    res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::UndoOne);

    if (res != M4_ERROR)
    {
        res = m_poRecordWithTransaction->Rollback ();
        if (res != M4_ERROR) {
            Operation.Reset ();
        }
    }

    GetpChannel_Data()->RefreshFlags();

    return res;
}

m4return_t ClNode_Data::RefreshFlags (m4uint8_t &ao_operation)
{
    m4uint8_t   ioperation = M4DM_OPERATION_NONE;
    m4return_t  res = M4_SUCCESS;

    ao_operation = M4DM_OPERATION_NONE;

    _AClComponentStore_ForwardIt *pfi;

    pfi = m_pBlockStore->CreateIterator ();

    ClBlockReference blockref;
	m4bool_t isnoteof = pfi->Begin (blockref);

    while (isnoteof)
    {
        res = blockref->RefreshFlags(ioperation);
        if (res == M4_ERROR)
            break;

        ao_operation |= ioperation;
        isnoteof = pfi->Next (blockref);
    }
    
    m_pBlockStore->DestroyIterator (pfi);

    m_poRecordWithTransaction->RefreshStatus();

    if (Operation.NIsNew ())
        ao_operation |= M4DM_OPERATION_INSERT;
    if (Operation.NIsUpdated ())
        ao_operation |= M4DM_OPERATION_UPDATE;
    if (Operation.NIsDeleted ())
        ao_operation |= M4DM_OPERATION_DELETE;

    if (Operation.NIsNewByDDBB ())
        ao_operation |= M4DM_OPERATION_INSERT_BY_DDBB;
    if (Operation.NIsUpdatedByDDBB ())
        ao_operation |= M4DM_OPERATION_UPDATE_BY_DDBB;
    if (Operation.NIsDeletedByDDBB ())
        ao_operation |= M4DM_OPERATION_DELETE_BY_DDBB;

    Operation.Reset ();
    Operation.Set (ao_operation);

    return res;
}

m4return_t ClNode_Data::CommitPersisted (VersionMark *ai_version)
{
    m4return_t res;

    res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::CommitPersistedOne, (void *)ai_version);

// pase lo que pase reseteamos el flag del nodo
    Operation.Reset() ;

    //GetpChannel_Data()->RefreshFlags();

    return res;
}



m4return_t ClNode_Data::RollbackPersisted (void)
{
    m4return_t res;
    res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::RollbackPersistedOne);

    GetpChannel_Data()->RefreshFlags();

    return res;
}


m4return_t	ClNode_Data :: CheckModify( m4bool_t &ao_bModified )
{

	m4return_t	iResult ;
	m4uint32_t	iNumRows ;
	m4uint8_t	iAffectsDB ;


	ao_bModified = M4_FALSE ;

	if( m_pClNode_Kernel != NULL )
	{
		iAffectsDB = m_pClNode_Kernel->AffectsDB() ;
		iNumRows = m_pClNode_Kernel->NumRows() ;

		if( iAffectsDB == 1 && iNumRows == 0 )
		{
			iResult = ClCStoreUtilities :: ForEach( m_pBlockStore, ClCStoreUtilities::CheckModify, ( void* ) &ao_bModified ) ;
		}
		else
		{
			// es de carga parcial, no hacemos nada
			iResult = M4_SUCCESS ;
		}
	}
	else
	{
		iResult = M4_SUCCESS ;
	}

	return( iResult ) ;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void ClNode_Data::GetpUniqueBlock (ClBlockReference &ao_br)
{
    // sólo se llama en el caso de los nodos raiz:
    if (GetpExtInfo ()->IsFree ()) {
        M4_ASSERT (m_pBlockStore);

        //si el bloque proxy no está creado, se crea:
        m_pBlockStore->Get (0, 0, ao_br, 0, 0);
    }
    else {
        //caso especial del carril:
        //si osan llamar aquí y no somos free, devolvemos el especial nihil
        //devuelvo el nihil a mala leche:
		ao_br.Set (&g_oNihilBlock);
    }

    M4_ASSERT (ao_br);
}


m4return_t ClNode_Data::ReleaseAll (m4bool_t ai_bresetnodeitems)
{
    m4return_t res;

//borro los bloques:

    Operation.Reset ();


//EDU-------------->>
//    if (GetpExtInfo ()->IsFree ()) {
    if (m_pBlockStore->GetType () == _AClComponentStore::MONO) {
//EDU-------------<<
        //borro el bloque asociado al proxy, pero no destruyo el proxy !
		ClBlockReference br;
        m_pBlockStore->Get (0, 0, br);  //que no le cree
        if (br)
            br->ResetProxy ();

        res = M4_SUCCESS;
    }
    else {
        //si estoy encarrilado, sólo hago reset de los proxys:
        if (IsLaned ()) {
            res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::ResetOne);
        }
        else {
            //caso normal:
            //me cepillo todos los bloques:
            res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneFast);
            m_pBlockStore->RemoveAll ();
        }
    }

    /*
    //además puedo estar 'encarrilado' de mala manera:
    if (m_pProxy)
        m_pProxy->ResetProxy ();
    */

//borro (a mano) los items de nodo:
    
    if (ai_bresetnodeitems) {
        RefreshRWT();
/*
        // me creo un item temporal:
        ClItem item (0, M4DM_SCOPE_NODE);
        item.Attach ((ClNode_Data *)this);
        item.Begin ();
        while (item.Index.IsNotEOF ()) {
            // se pone a null
            if (item.ItemDef.Type() != M4DM_TYPE_METHOD)
                item.Value.SetDefault ();
            item.Next ();
        }

        res = m_poRecordWithTransaction->Commit();
*/
        /*
	    if (res != M4_SUCCESS) {
		    // MODIFICADO POR CARLOS ROLDAN PARA SERIAL ABSOLUTO !!!
            return res;
        }
        */
    }

    return res;
}

m4return_t ClNode_Data::ReleaseDoc (m4bool_t ai_bresetnodeitems)
{
	//nuevo metodo: borra los bloques de un documento cuando hago releaseall del carril

	Operation.Reset ();
	
	m4return_t res = ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneSafe);
	m_pBlockStore->RemoveAll ();
	
	if (ai_bresetnodeitems) {    
		// me creo un item temporal:
		ClNoCallableItem item (M4DM_SCOPE_NODE);
		item.Attach ((ClNode_Data *)this);
		item.Begin ();
		while (item.GetIndex() != M4DM_EOF_INDEX) {
			// se pone a null
			if (item.ItemDef.Type() != M4DM_TYPE_METHOD) {
				item.Value.SetDefault ();
			}
			item.Next ();
		}

		res = m_poRecordWithTransaction->Commit();
	}

	return res;
}

ClBlock_Channel *ClNode_Data::CreateBlock (void/*ClBlock_Channel_Proxy *ai_pproxy*/)
{
    ClBlock_Channel *pdb = 0;

    pdb = new ClBlock_Channel (GetpChannel_Data()->GetpMemoryManager());
    pdb->Attach (this);

    // de momento Joseco se queda sin los Id de bloque
    //pdb->Index = Block.Count ();        // se inicializa el índice, el único sitio

    if (GetpExtInfo ()->Capacity () == M4CL_NODE_CAPACITY_TYPE_MONOBLOCK) {
        
        //monobloque, compruebo que no esté creado
        //me recorro el store a la caza de un bloque instanciado:
        _AClComponentStore_ForwardIt *pfi;

        pfi = m_pBlockStore->CreateIterator ();
        M4_ASSERT (pfi);

        //ClBlock_Channel_Proxy *pb;
		ClBlockReference br;
		m4bool_t isnoteof = pfi->Begin (br);
        while (isnoteof) {
			M4_ASSERT (br);
            if (br->IsValid ())
                //encontrado, a por él !!!!
                break;

            isnoteof = pfi->Next (br);
        }
    
        m_pBlockStore->DestroyIterator (pfi);

        if (br) {
            m4uint32_t NodeHandle = GetpNodeDef()->GetHandle();
            ClCompiledMCR * pmcr  = GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();

            DUMP_CHLOG_DEBUGF(M4_CH_DM_DEBUGMONOBLOCK, M4ObjNodeN(pmcr, NodeHandle));

            br->DestroyBlock ();
            // fulminante, borra recursivamente hacia abajo --> importante

            // ahora tengo que poner a todos los reg que estuvieran apuntando
            // a este bloque a eof -> van a apuntar a un nuevo bloque que por ser
            // nuevo no tiene registros y por tanto todos los reg tienen que quedarse
            // en eof
            GetpChannel_Data ()->GetpNotifier ()->DestroyBlock_After (this, br);
        }
    }
    //EDU---------<<

    return pdb;
}


//----------------------------------------------------------------------------
// A) Conectar los canales
//      -> el 'this' aquí es el nodo padre <-
//----------------------------------------------------------------------------

m4return_t ClNode_Kernel::LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
    if (IsLaned())
        Parent.Remove(Parent.GetpNotFree()->GetAddress());

// 1) El nodo hijo tiene una lista de nodos padres que hay que actualizar:
    Parent.Add (ai_lanecon.Lane.pClNode->GetpNode_Data(), 0,
        M4CL_NODE_CONNECTOR_TYPE_RB);

	// sets the is laned flag
	SetIsLaned(M4_TRUE);

// 2) y cambiar la estructura del bloque hijo --> cambiar el tipo de los items conectados 
//    (p.ej. si el item es de registro ahora pasaría a ser de bloque).

    ColItemDef_Kernel.Refresh (ai_lanecon.Doc.pClNode->GetpNode_Data());

    // y actualizo las listas de punteros a items 
    ItemProxy_Array.Refresh (this);

// 3) actualización de los indices de búsqueda:
    ClSearchIndex_List::Update (GetpCMCR ());

    return M4_SUCCESS;
}

m4return_t ClNode_Kernel::LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
// 1) El nodo hijo tiene una lista de nodos padres que hay que actualizar:
    Parent.Remove (ai_lanecon.Lane.pClNode->GetpNode_Data());

	// sets the is laned flag
	SetIsLaned(M4_FALSE);

// 2) y cambiar la estructura del bloque hijo --> cambiar el tipo de los items conectados 
    ColItemDef_Kernel.Refresh (ai_lanecon.Doc.pClNode->GetpNode_Data());

    // y actualizo las listas de punteros a items 
    ItemProxy_Array.Refresh (this);

// 3) actualización de los indices de búsqueda:
    ClSearchIndex_List::Update (GetpCMCR ());

    return M4_SUCCESS;
}

m4return_t ClNode_Data::LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
    GetpExtInfo()->LaneConnectToNode_Lane (ai_lanecon);

    if (GetpBlockStore ())
    {
        //antes de llamar a InitBlockStore, el childnode tiene que saber que tiene un padre,
        //y por tanto que es no-free
        if (GetpExtInfo ()->IsFree () == M4_FALSE)
        {
            //bug/mejora:
            //tenemos mucho cuidado de destruir el proxy pb, porque algún objeto temporal
            //(recordset, register) puede seguir usandolo.
            //lo que hacemos ahora es respetarle, le colocamos como primer elemento del nuevo store
    
            ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneSafe);
            ClComponentStore_Factory::Update (this);
        }
    }

    return RefreshRWT();
}

m4return_t ClNode_Data::LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
    GetpExtInfo()->LaneDisConnectToNode_Lane (ai_lanecon);

    if (GetpBlockStore ())
    {
        //antes de llamar a InitBlockStore, el childnode tiene que saber que tiene NO un padre,
        //y por tanto que es free
        if (GetpExtInfo ()->IsFree () == M4_TRUE)
        {
            //bug/mejora:
            //tenemos mucho cuidado de destruir el proxy pb, porque algún objeto temporal
            //(recordset, register) puede seguir usandolo.
            //lo que hacemos ahora es respetarle, le colocamos como primer elemento del nuevo store
    
            ClCStoreUtilities::ForEach (m_pBlockStore, ClCStoreUtilities::DeleteOneSafe);
            ClComponentStore_Factory::Update (this);
        }
    }

    return RefreshRWT();
}

m4bool_t ClNode_Data::IsLaned (void)
{
	return GetpExtInfo ()->IsLaned();
}

m4uint64_t ClNode_Data::GetNodeTimeStamp( void ) const
{

	m4uint64_t	iTimeStamp = 0 ;


	if( m_poRecordWithTransaction != NULL )
	{
		iTimeStamp = m_poRecordWithTransaction->GetTimeStamp() ;
	}

	return( iTimeStamp ) ;
}

m4return_t	ClNode_Data::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClNode_Data);

		ao_size -= sizeof(Operation);
	if (Operation.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

		ao_size -= sizeof(ClSearchIndex_Current);
	if (ClSearchIndex_Current::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
/*
	ao_size -= sizeof(m_oRecordWithTransaction);
*/
	if (m_poRecordWithTransaction)
    {
        if (m_poRecordWithTransaction->GetSize(ao_size)!=M4_SUCCESS)
        {
		    return M4_ERROR;
	    }
    }

    if (m_pBlockStore){
		if (m_pBlockStore->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	}
	if (m_pClNode_Kernel){
		if (m_pClNode_Kernel->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}


m4return_t ClNode_Kernel::GetSize(ClSize &ao_size)
{
	ClSizerLabel oSL(ao_size,"ClNode_Kernel");
	
	ao_size+=sizeof(ClNode_Kernel);
	
	ao_size-=sizeof(ClSearchIndex_List);
	if (ClSearchIndex_List::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(ClNodeDef);
	if (ClNodeDef::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(Parent);
	if (Parent.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(ChildNodeBB);
	if (ChildNodeBB.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(ChildNodeRB);
	if (ChildNodeRB.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(ColItemDef_Kernel);
	if (ColItemDef_Kernel.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(ItemProxy_Array);
	return ItemProxy_Array.GetSize(ao_size);
	
	// Apuntan a miembros estaticos
	/*	if (m_pRegInsert){
	if (m_pRegInsert->GetSize(ao_size)!=M4_SUCCESS){
	return M4_ERROR;
	}
	}
	if (m_pRegDelete){
	if (m_pRegDelete->GetSize(ao_size)!=M4_SUCCESS){
	return M4_ERROR;
	}
	}
	*/
}

m4return_t ClNodeParent::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClNodeParent);
		ao_size -= sizeof(m_vpparents);

	if (GetVectorSize(m_vpparents,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

m4return_t ClNode_Connector::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClNode_Connector);
	
	ao_size -= sizeof(m_Handle);
	return m_Handle.GetSize(ao_size);
}

m4return_t ClNode_Data::RefreshRWT(void)
{
    m4return_t res;

	// EFV >>>>> borro el RWT antes de destruir los EMD
    delete m_poRecordWithTransaction;
	// EFV <<<<< borro el RWT antes de destruir los EMD

	// Borro los StaticMetaData de nodo, bloque, y registro
    ClExtendedMetaData::DestroyMetaData(m_pSMDataRecord);
    ClExtendedMetaData::DestroyMetaData(m_pSMDataBlock);
    ClExtendedMetaData::DestroyMetaData(m_pSMDataNodo);

    // Creamos los StaticMetaData. Uno para los items a nivel de nodo,
    // otro para los items a nivel bloque y otro para los items a nivel de registro
    ClMetaChannelToExtendedMetaData convertidor;
    
    ClCompiledMCR* pMetaChannel = GetpExtInfo()->GetpCMCR();
    m4uint32_t     iNodeHandle  = GetpExtInfo()->GetHandle();

    res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                  iNodeHandle,
                                                                  M4CL_ITEM_SCOPE_NODE,
                                                                  m_pSMDataNodo);
    if (res == M4_SUCCESS)
        res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                      iNodeHandle,
                                                                      M4CL_ITEM_SCOPE_BLOCK,
                                                                      m_pSMDataBlock);
    if (res == M4_SUCCESS)
        res = convertidor.ConvertMetaChannelToExtendedMetaDataByScope(*pMetaChannel,
                                                                      iNodeHandle,
                                                                      M4CL_ITEM_SCOPE_REGISTER,
                                                                      m_pSMDataRecord);
    if (res == M4_SUCCESS)
    {
        m_poRecordWithTransaction = new ClRecordWithTransactions();
        m_poRecordWithTransaction->SetpMemoryManager(GetpChannel_Data()->GetpMemoryManager());
        m_poRecordWithTransaction->Init(m_pSMDataNodo);
    }

    return res;
}


m4return_t ClNode_Data::Reserve (m4uint32_t ai_lSize)
{
    return GetpBlockStore()->Reserve(ai_lSize);
}

m4return_t ClNode_Data::ForceLock( void )
{
    return GetpBlockStore()->ForceLock();
}

