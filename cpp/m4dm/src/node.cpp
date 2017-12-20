#include "dmres.hpp"

  //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             node.cpp
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
#include "dm_def.hpp"
#include "m4define.hpp"
#include "node.hpp"
#include "access.hpp"
#include "itemconn.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "cldefine.hpp"
#include "bnihil.hpp"
#include "node_knl.hpp"
#include "px_block.hpp"
#include "anoddir.hpp"
#include "handles.hpp"
#include "conitinf.hpp"
#include "m4mdrt.hpp"
#include "checkmem.hpp"
#include "blockref.hpp"
#include "computil.hpp"
#include "sizerutils.hpp"
#include "laneinfo.hpp"
#include "regi_smp.hpp"

//## end module.additionalDeclarations


///////////////////////////////////////////////////////////////////////////////
//
// ClNode
//


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

void *ClNode::operator new (size_t, void *ai_pbuffer)
{
    return ai_pbuffer;
}

void ClNode::operator delete (void *)
{
}

ClNode::ClNode (ClAccess *ai_paccess) : RecordSet (ai_paccess), Item (ai_paccess, M4DM_SCOPE_NODE)
{
    #ifdef _DEBUG
        g_StMemInfo.iNode_Access++;
    #endif

    m_pClAccess = ai_paccess;
    m_pClNode_Data = 0;
    m_poLane_Info = 0;
}

ClNode::~ClNode ()
{
    #ifdef _DEBUG
        g_StMemInfo.iNode_Access--;
    #endif

    if (m_poLane_Info)
    {
        GetpAccess()->GetpChannel()->GetpChannelManager ()->LaneManager.DisConnectIt (0, this);

        delete m_poLane_Info;
        m_poLane_Info = 0;
    }
    else
    {
        if (GetpNode_Data()->IsLaned ())
        {
            GetpAccess()->GetpChannel()->GetpChannelManager ()->LaneManager.DisConnectIt (this, 0);
        }
    }
}

//-----------------------------------------------------------------------------
// _Init
//      sólo llamada desde el access
//-----------------------------------------------------------------------------

void ClNode::Attach (ClNode_Data *ai_pnode_data)
{
// Zona critica para el carril nos desconectamos temporalmente hasta que se haya hecho el
// cambio de Channel_Data
    if (GetpAccess()->GetpChannel()->GetpChannelManager())
        GetpAccess()->GetpChannel()->GetpChannelManager()->LaneManager.DisConnectItTemp (0, this);

    m_pClNode_Data = ai_pnode_data;
    M4_ASSERT (m_pClNode_Data);

//inicializamos la propiedad de autoload
    M4_ASSERT (GetpExtInfo ()->IsValid ());
    ClAutoLoad_PlugIn::SetAutoLoadMode (GetpExtInfo ()->AutoLoad ());

    Item.Attach (m_pClNode_Data);
    RecordSet.Attach (m_pClNode_Data);

//inicializamos el valor IsFree
    m_bIsFree = m_pClNode_Data->GetpExtInfo ()->IsFree_ForgetingLanes ();

    RecordSet.Register.Filter->Notify_ReleaseAll_Before (&RecordSet.Register);
    RecordSet.Current.Filter->Notify_ReleaseAll_Before (&RecordSet.Register);

// Nos volvemos a conectar
    if (GetpAccess()->GetpChannel()->GetpChannelManager())
        GetpAccess()->GetpChannel()->GetpChannelManager()->LaneManager.ReConnectIt (0, this);
}

void ClNode::Reset (void)
{
	M4_ASSERT (m_pClAccess);
	
	// al cambiar mi bloque activo, los bloques de mis hijos RB también cambian:
	//RecordSet.Current.MoveToEOF ();
	
	// aviso a mis hermanos, bloques con relación BB, para ponerlos en current:                
	m4uint16_t nchildren = GetpExtInfo ()->ChildNodeBB.Count ();
	
	if (nchildren) {
		
		ClNodeIndex i = 0;
		
		// hacemos un tratamiento especial en caso de que el bloque sea nihil:
		// este código es análogo al del current register (programación orientada al copy-paste)
		ClBlockReference br;
		if (RecordSet.GetCurrentBlock ()->IsNihil ()){
			do {
				br.Set (&g_oNihilBlock);
				_UpdateAccess_BB (i, br);
				
				i++;
				nchildren--;
			} while (nchildren);
		}
		else {
			do {
				br.Set (RecordSet.GetChildBlock (i));
				_UpdateAccess_BB (i, br);
				
				i++;
				nchildren--;
			} while (nchildren);
		}
	}
}

/**
 * Hace un reset de verdad. Es decir, deja los BB de "this" apuntatndo a un proxy nil.
 * Este método se mete para arreglar el bug 0047176.
 * Por ahora sólo se hace la corrección del proceso del reset del access sobre la propagación
 * a los BB, pero no se descarta que exista el mismo problema para los RB.
 * Este método sólo debe ser llamado desde el Reset del access.
 */
void ClNode::RealReset(void)
{
	M4_ASSERT (m_pClAccess);
	
	const m4uint16_t nchildren = GetpExtInfo()->ChildNodeBB.Count ();
	if (nchildren) {
		// hacemos un tratamiento especial en caso de que el bloque sea nihil:
		// este código es análogo al del current register (programación orientada al copy-paste)
		ClBlockReference br;
		br.Set (&g_oNihilBlock);
		for (ClNodeIndex i = 0; i < nchildren; i++) {
			_UpdateAccess_BB(i, br);
		}
	}
}

//----------------------------------------------------------------------------
// Actualizaciones del contexto, teniendo en cuenta el tema del carril:
//----------------------------------------------------------------------------

void ClNode::_UpdateAccess_BB (ClChildIndex ai_childindex, const ClBlockReference &ai_br)
{
    _UpdateAccessNodeData (GetpExtInfo ()->ChildNodeBB, ai_childindex, ai_br);
}

void ClNode::_UpdateAccess_RB (ClChildIndex ai_childindex, const ClBlockReference &ai_br)
{
    _UpdateAccessNodeData (GetpExtInfo ()->ChildNodeRB, ai_childindex, ai_br);
}

void ClNode::_UpdateAccessNodeData (ClChildNodes<ClNode_Data> &ai_pchildarray, ClChildIndex ai_childindex, const ClBlockReference &ai_br)
{
    ClNode_Data *pchildnode_ch;
    pchildnode_ch = ai_pchildarray[ai_childindex].GetAddress ();  // devuelve un node_channel

    //si el nodo hijo es de este mismo canal, hago lo de toda la vida: avisar al ClNode_Access
    m_pClAccess->Node.Get (pchildnode_ch->GetpExtInfo ()->Index)->RecordSet.SetCurrentBlock (ai_br);
}

void ClNode::_UpdateAccessNode (ClChildNodes<ClNode> &ai_pchildarray, ClChildIndex ai_childindex, const ClBlockReference &ai_br)
{
    ClNode *pchildnode_ch;
    pchildnode_ch = ai_pchildarray[ai_childindex].GetAddress ();  // devuelve un node_channel

    pchildnode_ch->RecordSet.SetCurrentBlock (ai_br);
}

//----------------------------------------------------------------------------
// funciones get cutres:
//----------------------------------------------------------------------------

ClAccess *ClNode::GetpAccess (void) const
{
    return m_pClAccess;    
}

ClNodeDef *ClNode::GetpNodeDef (void) const
{
    return GetpExtInfo ();
}

ClNode_Kernel* ClNode::GetpExtInfo (void) const
{
    M4_ASSERT (GetpNode_Data ());

    return GetpNode_Data ()->GetpExtInfo ();
}

//----------------------------------------------------------------------------
// Er carrí
//----------------------------------------------------------------------------

//cmcr::hplug
//  handle del plug, que se obtiene a partir de:
//      - handle del nodo hijo (doc)
//      - handle del que va a ser nodo padre (carril) + puntero al cmcr del padre
//
//  se puede obtener a partir de él ...
//      - nombre del canal padre
//      - nombre del nodo padre
//      - número de items conectados
//      - array de conectores de items, consultable por posición
//          - nombre del item padre
//          - handle del item hijo
//          - tipo de relación (>, <, etc...)
//
// Tareas a hacer para conectar dos canales:
// Lo que nos van a indicar siempre es la conexión de un nodo de un access con
// un nodo de otro access, entonces ...
//
//      A) Búscamos si los canales ya estan conectados
//         Si no lo estuvieran tenemos que poner a punto la conexión entre canales.
//         Se trata de lo siguiente:
//              1) Actualizar la lista de items conectados a uno dado, esto se hace 
//                 en la clase ClConnectItemInfo (conitinf.cpp). Esta lista se utiliza en:
//                      - _NotifyChanges del items, que propaga el valor asignado
//                      - _UpdateConnectedItems de block_proxy, que viene a hacer lo mismo,
//                        asigna valor en un bloque recien creado a los items conectados
//              2) Actualizar la lista de nodos hijos de uno dado (ChildNodeRB y BB), que se
//                 utiliza para la GetChildBlock (que se usa en _NotifyChanges, propagación, etc.)
//              3) El nodo hijo tiene una lista de nodos padres que hay que actualizar:
//              4) Cambiar la estructura del bloque padre para que soporte más punteros a bloques
//                 hijos, 
//              5) y cambiar la estructura del bloque hijo --> cambiar el tipo de los items conectados 
//                 (p.ej. si el item es de registro ahora pasaría a ser de bloque).
//      B) Conectar los access
//              1) La idea es que cada nodo del access (del padre, vamos del carril) tenga la lista de
//                 access conectados, de forma que en la propagación de los current se llame a los nodos
//                 de otros canales.
//                 Esto se traduce sencillamente en actualizar el m_AccessList del ClNode añadiendo
//                 el nuevo access a la lista.
//              2) El nodo hijo tiene un puntero al current parent que hay que actualizar

//
// Function: ClNode::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//   To serialize a node we need to serialize the item iterator 
//   which will serialize the index and the value index (for slices)
//   of the items at the Node level, and then serialize the  
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t ClNode::Serialize(ClGenericIODriver& IOD)
{
    // Serializamos la autoload.
    m4return_t RetVal = IOD.Write( ClAutoLoad_PlugIn::GetAutoLoadMode() );
    
    // Serialize the items at the level of the node
	/*
	Mejora de ancho de banda. No se serializa la posición del item
    if (RetVal == M4_SUCCESS)
        RetVal = Item.Serialize(IOD);
	*/
    
    // Serialize the record set
    if (RetVal == M4_SUCCESS)
        RetVal = RecordSet.Serialize(IOD);
    
    m4uint16_t n;
    ClChildIndex i;
    ClNode_Data* pChildNode;
    m4uint8_t iIteSerMod = m_pClAccess->GetpChannel()->GetItemSerializationMode();
    
    // Serialize the child BB node of this node
    n = GetpExtInfo ()->ChildNodeBB.Count();
    for (i = 0; RetVal == M4_SUCCESS && i < n; i++)
    {
        pChildNode = GetpExtInfo()->ChildNodeBB[i].GetAddress();
        
        // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
        if (pChildNode->SerializationRequired(m_pClNode_Data, iIteSerMod) == M4_FALSE)
            continue;
        
        RetVal = GetpAccess()->Node.Get(pChildNode->GetpExtInfo()->Index)->Serialize(IOD);
    }
    
    // Serialize the child RB nodes of this node
    n = GetpExtInfo ()->ChildNodeRB.Count();
    for (i = 0; RetVal == M4_SUCCESS && i < n; i++)
    {
        pChildNode = GetpExtInfo()->ChildNodeRB[i].GetAddress();
        
        // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
        if (pChildNode->SerializationRequired(m_pClNode_Data, iIteSerMod) == M4_FALSE)
            continue;
        
        RetVal = GetpAccess()->Node.Get(pChildNode->GetpExtInfo()->Index)->Serialize(IOD);
    }
    
    return RetVal;
}

//
// Function: ClNode::DeSerialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we deserialize from
//
// Returns: m4return_t
//
// Description:
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t ClNode::DeSerialize(ClGenericIODriver& IOD)
{
    // Deserializamos la autoload.
    m4uint8_t iAutoLoadMode;
    m4return_t RetVal = IOD.Read(iAutoLoadMode);
    
    if (RetVal == M4_SUCCESS)
        ClAutoLoad_PlugIn::SetAutoLoadMode (iAutoLoadMode);
    
    // DeSerialize the items at the level of the node
	/*
	Mejora de ancho de banda. No se deserializa la posición del item
    if (RetVal == M4_SUCCESS)
        RetVal = Item.DeSerialize(IOD);
	*/
    
    // DeSerialize the record set
    if (RetVal == M4_SUCCESS)
        RetVal = RecordSet.DeSerialize(IOD);
    
    m4uint16_t n = GetpExtInfo ()->ChildNodeBB.Count();
    ClChildIndex i;
    ClNode_Data* pChildNode;
    ClNode* pChildNodeAccess;
    m4uint8_t iIteSerMod = m_pClAccess->GetpChannel()->GetItemSerializationMode();
    
    // Serialize the child BB node of this node
    n = GetpExtInfo()->ChildNodeBB.Count();
    for (i = 0; RetVal == M4_SUCCESS && i < n; i++)
    {
        pChildNode = GetpExtInfo()->ChildNodeBB[i].GetAddress();

        pChildNodeAccess = GetpAccess()->Node.Get(pChildNode->GetpExtInfo()->Index);

        // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
        if (pChildNode->SerializationRequired(m_pClNode_Data, iIteSerMod) == M4_FALSE)
            continue;

        RetVal = pChildNodeAccess->DeSerialize(IOD);
    }
    
    // Serialize the child RB nodes of this node
    n = GetpExtInfo()->ChildNodeRB.Count();
    for (i = 0; RetVal == M4_SUCCESS && i < n; i++)
    {
        pChildNode = GetpExtInfo()->ChildNodeRB[i].GetAddress();

        pChildNodeAccess = GetpAccess()->Node.Get(pChildNode->GetpExtInfo()->Index);

        // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
        if (pChildNode->SerializationRequired(m_pClNode_Data, iIteSerMod) == M4_FALSE)
            continue;

        RetVal = pChildNodeAccess->DeSerialize(IOD);
    }

    return RetVal;
}

/*
 * Function: ClNode::SendNodeMark
 *
 * Arguments:
 *      ai_bBranchMode - si es M4_TRUE se serializa todo lo que cuelga del nodo
 *
 * Description:
 *      Marca para serializar el nodo y la jerarquia de registros que lo soporta
 */
void ClNode::SendNodeMark(const m4bool_t ai_bBranchMode)
{
    m_pClNode_Data->MarkNodeAndParentRecordsForSerialize(ai_bBranchMode);
}
 
m4uint8_t ClNode::GetAutoLoadRealMode (void)
{
    m4uint8_t iautoloadmode;

    iautoloadmode = GetpAccess ()->ClAutoLoad_PlugIn::GetAutoLoadMode ();

    //le pregunto al nodo:
    if (iautoloadmode == M4CL_AUTOLOAD_NODESAYS) {
        // preguntamos al nodo:
        iautoloadmode = ClAutoLoad_PlugIn::GetAutoLoadMode ();
    }

    return iautoloadmode;
}

m4return_t ClNode::SetAutoLoadMode (m4uint8_t ai_mode)
{
//esta es la pública, que se puede llamar desde cualquier sitio:
    m4return_t res;
    m4uint8_t accessmode = GetpAccess ()->ClAutoLoad_PlugIn::GetAutoLoadMode ();

    if (M4CL_AUTOLOAD_NODESAYS != accessmode) {
        //mal, tengo que hacer caso al access:
        ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();

        DUMP_CHLOG_ERRORF (M4_CH_DM_AUTOLOADMANDATORY, accessmode << ai_mode <<
            M4ObjNodeN(pmcr, NodeHandle));

        return M4_ERROR;
    }

    res = _SetAutoLoadMode (ai_mode);

    if (M4_SUCCESS == res)
        res = ClAutoLoad_PlugIn::SetAutoLoadMode (ai_mode);

    return res;
}

m4return_t ClNode::SetPlugInAutoLoadMode (m4uint8_t ai_mode)
{
    return ClAutoLoad_PlugIn::SetAutoLoadMode (ai_mode);
}

m4return_t ClNode::SetAutoLoadMode_fromAccess (m4uint8_t ai_mode)
{
//me llaman desde el access:    
    if (M4CL_AUTOLOAD_NODESAYS == ai_mode) {
        //yo mando:
        //me está llamando el access, me dice que ponga el modo que tenga asignado:
        ai_mode = ClAutoLoad_PlugIn::GetAutoLoadMode ();
    }

    return _SetAutoLoadMode (ai_mode);
}

m4return_t ClNode::_SetAutoLoadMode (m4uint8_t ai_mode)
{
    m4return_t res;

    if (M4CL_AUTOLOAD_OFF == ai_mode || 
        M4CL_AUTOLOAD_BLOCK == ai_mode || 
        M4CL_AUTOLOAD_PRG == ai_mode) 
    {
        //llamamos a los register que tenemos:
        res = RecordSet.Register.Filter.SetAutoLoadMode (ai_mode);
        if (M4_ERROR == res)
            return res;
        res = RecordSet.Current.Filter.SetAutoLoadMode (ai_mode);
        if (M4_ERROR == res)
            return res;        
    }
    else {
        //error
        ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();

        DUMP_CHLOG_ERRORF (M4_CH_DM_INVALIDAUTOLOADMODE_NODE, ai_mode <<
            M4ObjNodeN(pmcr, NodeHandle));

        res = M4_ERROR;
    }

    return res;    
}
m4return_t ClNode::ReLoad (void)
{
	// Preguntamos bloque por bloque y luego vamos con los hijos.

	if (ClCStoreUtilities::ForEach (GetpNode_Data ()->GetpBlockStore(),ClCStoreUtilities::ReLoadOne, &RecordSet) != M4_SUCCESS )
		return M4_ERROR;
	
	//Vamos con los hijos.
	ClNode_Kernel * pndk = GetpNode_Data()->GetpExtInfo();
	
	m4int32_t nBB = pndk->ChildNodeBB.Count();
	m4int32_t nRB = pndk->ChildNodeRB.Count();
	m4uint32_t childnodehandle = 0;

	ClNode * pnodeBB=0,*pnodeRB=0;

	// Hijos BB.
	for (--nBB;nBB>=0;--nBB)
	{
		childnodehandle = pndk->ChildNodeBB[(ClChildIndex)(nBB)].GetAddress()->GetpNodeDef()->GetHandle();
		pnodeBB= GetpAccess()->Node.Get((ClHandle)childnodehandle);
		M4_ASSERT (pnodeBB);	

		if (pnodeBB->ReLoad()!=M4_SUCCESS)
			return M4_ERROR;
	}
	// Hijos RB
	for (--nRB;nRB>=0;--nRB)
	{
		childnodehandle = pndk->ChildNodeRB[(ClChildIndex)(nRB)].GetAddress()->GetpNodeDef()->GetHandle();
		pnodeRB= GetpAccess()->Node.Get((ClHandle)childnodehandle);
		M4_ASSERT (pnodeRB);

		if (pnodeRB->ReLoad()!=M4_SUCCESS)
			return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t ClNode::LaneConnectToNode_Doc (ClLaneConnection &ai_lanecon)
{
    if (m_poLane_Info == 0)
        m_poLane_Info = new ClLane_Info();

    ClHandle hcon = 0;
    ClNode *pchildnode = ai_lanecon.GetpDocNode (); // puntero al nodo 'hijo'
    ClItemIndex oldDoc = pchildnode->RecordSet.Register.Item.GetIndex();
    ClItemIndex oldLane = RecordSet.Register.Item.GetIndex();

// 1) Actualizar la lista de nodos hijos de uno dado (ChildNodeRB y BB), que se
//    utiliza para la GetChildBlock (que se usa en _NotifyChanges, propagación, etc.)
    
    m_poLane_Info->ChildNodeRB.Add (pchildnode, hcon);

// 2) Actualizar la lista de items conectados a uno dado, esto se hace 

    m4pcchar_t pfatheritemname; // nombre del item 'padre'
    ClHandle hfatheritem; // handle del item 'padre'    
    ClHandle hchilditem; // handle al item 'hijo'    

    // en pruebas, de momento soportamos solo RB    
    m4uint8_t iconnectiontype = M4CL_NODE_CONNECTOR_TYPE_RB;
    
    m4uint16_t ichildnodepos;
    //indice de la conexión:
//    res = m_poLane_Info->ChildNodeRB.GetPos (pchildnode->GetpExtInfo ()->GetHandle (), ichildnodepos);
    ichildnodepos = m_poLane_Info->ChildNodeRB.Count() - 1;

    m4uint16_t i, n;
    n = ai_lanecon.NumberOfItemsConnected ();

    m4uint32_t doc_index = m_poLane_Info->AddNewDoc(n);

    for (i = 0; i < n; i++) {
        //actualizamos la lista de items 'hijos' del item del carril:

        //localizo al padre
        pfatheritemname = ai_lanecon.GetFatherItemName (i);
		RecordSet.Register.Item [pfatheritemname];

        m_poLane_Info->GetpLane_Item_Info_by_Doc(doc_index)->m_pHandles[i] =
            RecordSet.Register.Item.ItemDef.GetHandle();

        //localizo al hijo
        hchilditem = ai_lanecon.GetChildItemHandle (i);
		pchildnode->RecordSet.Register.Item [hchilditem];

        //obtengo la lista:

        _AClConnectorImplementation *ppp = 0;   //importante inicializar a cero !!

        m_poLane_Info->GetpLane_Item_Info_by_Doc(doc_index)->m_pConnectItemInfo[i].Add (pchildnode->RecordSet.Register.Item.ItemDef.Scope (),
            RecordSet.Register.Item.ItemDef.Scope (), iconnectiontype,
            M4CL_ITEM_CONNECTOR_EQUALITY, ppp);

        ppp->Init (pchildnode->GetpNode_Data(), hchilditem,
            ichildnodepos + GetpExtInfo()->ChildNodeRB.Count());

        //inicializo la información del padre, en el item hijo:
        ClConnectItemInfo *pciichild;
        pciichild = pchildnode->RecordSet.Register.Item.GetpConnectItemInfo ();
        
        pciichild->Parent.Init (GetpNode_Data(), RecordSet.Register.Item.GetIndex());
    }

    pchildnode->RecordSet.Register.Item.MoveTo(oldDoc);
    RecordSet.Register.Item.MoveTo(oldLane);

// 2) meter el access en la lista
//    m_AccessList.push_back (pchildnode->GetpAccess ());


// 3) asignamos el bloque activo que corresponde al nodedoc:    

    //ojo --> al carril podrían haberse conectados varios canales!

    m4uint16_t ipos;

    m_poLane_Info->ChildNodeRB.GetPos (pchildnode->GetpExtInfo ()->GetHandle (), ipos);
	ClBlockReference br;
// Si le paso un hijo con indice mayor que el numero de hijos reales que tiene
// un nodo, se le pide al ClNode, es decir, a los hijos documento:
//                                   ___________________________________________
    RecordSet.Current.GetChildBlock (ipos + GetpExtInfo()->ChildNodeRB.Count(), br);
    pchildnode->RecordSet.SetCurrentBlock (br);    

// 4) actualizo el valor Free del doc:
    pchildnode->m_bIsFree = M4_FALSE;

// 5) cuenta de conexiones en los access:
    pchildnode->GetpAccess ()->IncLaneCount ();
    GetpAccess ()->IncLaneCount ();

    return M4_SUCCESS;
}

m4return_t ClNode::LaneConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
    return GetpNode_Data()->LaneConnectToNode_Lane (ai_lanecon);
}

m4return_t ClNode::LaneDisConnectToNode_Doc (ClLaneConnection &ai_lanecon)
{
    ClNode * pchildnode = ai_lanecon.GetpDocNode ();

// 1) borro la conexion
    if ((m_poLane_Info)&&(pchildnode))
    {
        m_poLane_Info->DeleteDoc(pchildnode);
        if (m_poLane_Info->m_iDocs == 0)
        {
            delete m_poLane_Info;
            m_poLane_Info = 0;
        }

// 2) quitar el access de la lista
//        m_AccessList.pop_back ();


// 3) asignamos el bloque nihil al documento:

		ClBlockReference br;
	    br.Set (&g_oNihilBlock);
        pchildnode->RecordSet.SetCurrentBlock (br);    

// 4) actualizo el valor Free del doc:
        pchildnode->m_bIsFree = M4_TRUE;

// 5) cuenta de conexiones en los access:
        pchildnode->GetpAccess ()->DecLaneCount ();
        GetpAccess ()->DecLaneCount ();

        return M4_SUCCESS;
    }

    return M4_ERROR;
}

m4return_t ClNode::LaneDisConnectToNode_Lane (ClLaneConnection &ai_lanecon)
{
	/*
	 * Corrección del bug 0051161.
	 * Actualiza la lista de ítems 'hijos' del ítem del carril.
	 *
	 * Pretende restaurar los valores que se modificaron en "ClNode::LaneConnectToNode_Doc" (en este método se
	 * estable quienes son los padres de los ítems conectados al hacer el encarrilamiento).
	 * Debe hacerse sólo cuando se está borrando el último carril sobre el canal (es el momento de restaurar la
	 * meta información de los ítems).
	 *
	 * Básicamente lo que hace es poner toda la información de los padres a 0.
	 */
	ClNode* const pChildNode = ai_lanecon.GetpDocNode();
	ClHandle hChildItem = 0;
	ClConnectItemInfo* pConnectItemInfoChild = 0;
	const m4uint16_t n = ai_lanecon.NumberOfItemsConnected();
	
	for (m4uint16_t i = 0; i < n; i++) {
		// Localizo al hijo.
		hChildItem = ai_lanecon.GetChildItemHandle(i);
		pChildNode->RecordSet.Register.Item [hChildItem];
		
		// Inicializo la información del padre, en el ítem hijo.
		pConnectItemInfoChild = pChildNode->RecordSet.Register.Item.GetpConnectItemInfo();
		pConnectItemInfoChild->Parent.Init(0, M4DM_EOF_INDEX);
	}
	/* Fin de la corrección. */
	
	return GetpNode_Data()->LaneDisConnectToNode_Lane (ai_lanecon);
}

m4return_t 
ClNode::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNode);

	ao_size-=sizeof(ClAutoLoad_PlugIn);
		if (ClAutoLoad_PlugIn::GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	ao_size-=sizeof(Item);
		if (Item.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	ao_size-=sizeof(RecordSet);
		if (RecordSet.GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
		}
	
	if (m_poLane_Info){
		if (m_poLane_Info->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}
m4return_t
ClNode::GetTotalSize(ClSize &ao_size)
{
	if (GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (m_pClNode_Data->GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
