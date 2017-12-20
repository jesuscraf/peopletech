#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           access.cpp
// File:             access.cpp
// Project:          CVM-DM
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
#include "dm_def.hpp"
#include "m4define.hpp"
#include "dmres.hpp"
#include "access.hpp"
#include "cldefine.hpp"
#include "m4mdrt.hpp"
#include "node.hpp"
#include "node_knl.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "handles.hpp"
#include "index.hpp"
#include "m4objreg.hpp"
#include "checkmem.hpp"
#include "iexecutor.hpp"
#include "blockref.hpp"
#include "dm.hpp"
#include "m4notifier.hpp"
#include "m4event.hpp"
#include "cldmcontext.hpp"

//## end module.includes preserve=yes


// ==========================================================================
// Array para los nombres de las propiedades fijas de Acceso
// Se emplea para optimizar la serialización

m4pchar_t   g_acAccessProperties[ M4_DM_ACCESS_PROPERTIES_NUMBER ] =
{
	M4CMPROP_ACCESS_END_DATE,
	M4CMPROP_ACCESS_EXECUTION_DATE,
	M4CMPROP_ACCESS_RUN_SLICE,
	M4CMPROP_ACCESS_START_DATE
} ;



///////////////////////////////////////////////////////////////////////////////
//
// ClAccess
//

// el access lleva asociado un array de nodos, voy a alocar de una vez ese
// espacio:

void *ClAccess::operator new (size_t ai_size, m4uint16_t ai_nodescount)
{
    return ::new m4uint8_t [ai_size + ai_nodescount * sizeof (ClNode)];
}


//----------------------------------------------------------------------------
// Constructor / Destructor
//----------------------------------------------------------------------------

ClAccess::ClAccess (m4uint32_t ai_iHandle)
	: ExecutionDate			( M4_PROP_STATIC_MAPPED )
	, ExecutionStartDate	( M4_PROP_STATIC_MAPPED )
	, ExecutionEndDate		( M4_PROP_STATIC_MAPPED )
	, RunSliceMode			( M4_PROP_STATIC_MAPPED )
	, m_pDestroy_Notifier	(new ClNotifier) //Como mola.
	, m_iRemoteId			(0)
	, m_eDebugMode			(ClChannel::IS_NOT_DEBUG)

{
    #ifdef _DEBUG
        g_StMemInfo.iAccess++;
    #endif

	m_iHandle = ai_iHandle;

	// Add the properties to the collection of properties:
    Property.AddProperty( M4CMPROP_ACCESS_EXECUTION_DATE,	&ExecutionDate ) ;
    Property.AddProperty( M4CMPROP_ACCESS_START_DATE,		&ExecutionStartDate ) ;
    Property.AddProperty( M4CMPROP_ACCESS_END_DATE,			&ExecutionEndDate ) ;
    Property.AddProperty( M4CMPROP_ACCESS_RUN_SLICE,		&RunSliceMode ) ;

    m_iLaneCount = 0;
}

ClAccess::~ClAccess ()
{
    #ifdef _DEBUG
        g_StMemInfo.iAccess--;
    #endif

    Node.Destroy ();

    if (m_pClChannel) {
        m_pClChannel->Access._Remove (this);
        m_pClChannel = 0;
    }
	
	if (m_pDestroy_Notifier) {
		delete m_pDestroy_Notifier;
		m_pDestroy_Notifier = 0;
	}
}

m4return_t ClAccess::Destroy (void)
{
	ClEvent oEvent(this,M4_NOTIF_ACCESS_DESTROY);
	m_pDestroy_Notifier->Signal(oEvent);

    delete this;    // me suicido

    return M4_SUCCESS;
}


//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------

m4return_t ClAccess::Init (ClChannel *ai_pClChannel)
{
    m4return_t res;

    M4_ASSERT (ai_pClChannel);

    m_pClChannel = ai_pClChannel;

    // se añade a la lista de access del canal:
    
    m_pClChannel->Access._Add (this);


    // pillo valores por defecto para las propiedades:
    m4VariantType vDate;

    // execution date: valor por defecto del canal
        m_pClChannel->VM_ExecutionDate.Get (vDate);
        ExecutionDate.Set (vDate);

    // start date: la misma del canal
        m_pClChannel->VM_StartDate.Get (vDate);
        ExecutionStartDate.Set (vDate);

    // end date: la misma del canal
        m_pClChannel->VM_EndDate.Get (vDate);
        ExecutionEndDate.Set (vDate);
    
		RunSliceMode.Set ( m4VariantType( (m4double_t ) M4_FALSE) );
    //Stack.Init (m_pExecutor); //quitar EN BREVE

//Autoload:
    ClAutoLoad_PlugIn::SetAutoLoadMode (m_pClChannel->ClAutoLoad_PlugIn::GetAutoLoadMode ());

    // el array de nodos:
    Node.Init (this, m_pClChannel->GetpChannelDef ()->NumberOfNodes ());
    Node.Attach (m_pClChannel->GetpChannelDef (), &m_pClChannel->GetpChannel_Data ()->Node);

    // reset del access:
    res = Reset ();

    res = M4_SUCCESS;

    return res;
}


IExecutor *ClAccess::GetpExecutor (void) const {
	ClChannel *pChannel = GetpChannel();
	if ( pChannel ) {
		ClChannelManager *pChannelManager = pChannel->GetpChannelManager();
		if ( pChannelManager )
			return pChannelManager->GetpExecutor();
	}
	return 0;
}


//----------------------------------------------------------------------------
// Recorre todos los nodos y resetea el bloque current:
//      si el nodo es un raiz (o un free) pone como bloque activo el proxy
//      en caso contrario pone como activo el nihil.
//----------------------------------------------------------------------------

m4return_t ClAccess::Reset (void)
{
    m4uint16_t nnodes;
    nnodes = Node.Count ();

    ClNodeIndex i = 0;

    // iteramos por los roots y les asignamos como bloque activo un proxy
    // en _ResetCurrentBlock
    ClNode *pnode;
	ClBlockReference br;

    for (i = 0; i < nnodes; i++) {
        pnode = Node.Get (i);
        M4_ASSERT (pnode);
        
        m4bool_t manualreset;
        
        if (pnode->IsFree ()) { //ojo --> se lo preguntamos al nodo del access:

            pnode->GetpNode_Data ()->GetpUniqueBlock (br);

            //hacemos lo siguiente: asignamos el bloque sin contemplaciones al
            //recordset:
            /*
            pnode->RecordSet.SetCurrentBlock (pb);
                //cuando se llama por primera vez puede no estar asignado

            //y llamamos a reset en el nodo, para que ponga todo a eof
            //esto itera recursivamente por los nodos hijos poniendolos a nihil
            pnode->Reset ();
            */
            manualreset = M4_BOOL(!pnode->RecordSet.SetCurrentBlock (br));
        }
        else {
            manualreset = pnode->GetpExtInfo ()->IsFree_ForgetingLanes ();
        }

        if (manualreset) {
            //cambio manual:

            pnode->RecordSet.Register.MoveToEOF ();
            pnode->RecordSet.Current.MoveToEOF ();

            //y llamamos a reset en el nodo, para que ponga todo a eof
            //esto itera recursivamente por los nodos hijos poniendolos a nihil
            pnode->RealReset ();
        }

    }

    return M4_SUCCESS;
}

//
// Function: ClAccess::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//    To serialize an access we need to serialize each ClNode
//    which the access has in its directory
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t ClAccess::Serialize(ClGenericIODriver& IOD)
{
    IOD.StoreAddress(this);
    
    // Serializamos la autoload.
    m4return_t iRet = IOD.Write( GetAutoLoadMode() );
    
    
    m4uint8_t iIteSerMod = m_pClChannel->GetItemSerializationMode();

    // Serializamos las propiedades dinamicas (creadas por el usuario) >>>>>>>>>>>>>>>>>
    if (iRet == M4_SUCCESS)
        iRet = Property.Serialize(IOD, iIteSerMod );
    // Serializamos las propiedades dinamicas (creadas por el usuario) <<<<<<<<<<<<<<<<<

    // Iteramos por los roots, se itera recursivamente por los nodos hijos del llamado
    ClNode*    pNode;
    ClNode_Data* pNode_Data;
    m4uint16_t nnodes = Node.Count ();
    
    for (ClNodeIndex i = 0; i < nnodes && iRet == M4_SUCCESS; i++) 
    {
        pNode = Node.Get(i);
        if (pNode->GetpExtInfo()->IsFree_ForgetingLanes() == M4_TRUE)
        {
            // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
            pNode_Data = pNode->GetpNode_Data();
            if (pNode_Data->SerializationRequired(pNode_Data, iIteSerMod) == M4_FALSE)
                continue;
            
            iRet = pNode->Serialize(IOD);
        }
    }

    return iRet;
}

//
// Function: ClAccess::DeSerialize
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
m4return_t ClAccess::DeSerialize(ClGenericIODriver& IOD)
{
    IOD.StoreAddress(this);

    // Serializamos la autoload.
    m4uint8_t iAutoLoadMode;
    m4return_t iRet = IOD.Read(iAutoLoadMode);
    if (iRet == M4_SUCCESS)
        SetAutoLoadMode(iAutoLoadMode);

    
    // Serializamos las propiedades dinamicas (creadas por el usuario) >>>>>>>>>>>>>>>>>
    if (iRet == M4_SUCCESS)
        iRet = Property.DeSerialize(IOD, 2);
    // Serializamos las propiedades dinamicas (creadas por el usuario) <<<<<<<<<<<<<<<<<

    // Iteramos	 por los roots, se itera recursivamente por los nodos hijos del llamado
    ClNode*    pNode;
    ClNode_Data* pNode_Data;
    m4uint16_t nnodes = Node.Count ();

    for (ClNodeIndex i = 0; i < nnodes && iRet == M4_SUCCESS; i++) 
    {
        pNode = Node.Get(i);
        if (pNode->GetpExtInfo()->IsFree_ForgetingLanes() == M4_TRUE)
        {
            // Un nodo de un access se serializa si hay que serializar el correspondiente nodo del canal
            pNode_Data = pNode->GetpNode_Data();
            if (pNode_Data->SerializationRequired(pNode_Data, m_pClChannel->GetItemSerializationMode()) == M4_FALSE)
                continue;

            iRet = pNode->DeSerialize(IOD);
        }
    }

    return iRet;
}



m4uint32_t ClAccess::GetHandle ( void ) const
{
	return ( m_iHandle );
}

void ClAccess::Follow (ClAccess &ai_access)
{
	M4_ASSERT (GetpChannel () == ai_access.GetpChannel ());
	
	m4uint16_t nnodes = Node.Count ();
	ClNode *pnode_source_access = 0;
	ClNode *pnode_dest_access = 0;
	
	for (ClNodeIndex i = 0; i < nnodes; i++) {
		//asignamos a saco el bloque:
		pnode_source_access = ai_access.Node.Get (i);
		pnode_dest_access = Node.Get (i);
		
		pnode_dest_access->RecordSet.FollowSimple (pnode_source_access->RecordSet);
		
		// Corrige el bug 51424. Vamos a copiar también el modo de autoload.
		const m4uint8_t iNodeAutoLoadMode = pnode_source_access->GetAutoLoadMode();
		pnode_dest_access->SetPlugInAutoLoadMode (iNodeAutoLoadMode);
	}

	/* Corrige el bug 51424.
	 * Vamos a copiar también el modo de autoload. De esta forma el acceso queda igual que el original.
	 */
	const m4uint8_t iAccesAutoloadMode = ai_access.GetAutoLoadMode();
	SetAutoLoadMode(iAccesAutoloadMode);

	CopyProperties(ai_access);
}

m4return_t	ClAccess::CopyProperties(ClAccess &ai_access) {
	m4VariantType vPropertyValue;

	if(ai_access.ExecutionDate.Get(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ExecutionDate.Set(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ai_access.ExecutionStartDate.Get(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ExecutionStartDate.Set(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ai_access.ExecutionEndDate.Get(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ExecutionEndDate.Set(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(ai_access.RunSliceMode.Get(vPropertyValue) == M4_ERROR)
		return M4_ERROR;
	if(RunSliceMode.Set(vPropertyValue) == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

ClCompiledMCR* ClAccess::GetpCMCR (void) const
{
    return m_pClChannel->GetpChannelDef ()->GetpCMCR ();
}


m4return_t ClAccess::Load (m4bool_t ai_bFromWrapper/*=M4_FALSE*/)
{
    m4return_t res = M4_ERROR;
	m4bool_t bLoaded = M4_FALSE;

	M4_ASSERT (m_pClChannel);

//Descometar cacheLoad y cache Put cuando esten en channel.hpp

	res = m_pClChannel->CacheLoad(bLoaded);

	if (M4_FALSE == bLoaded)
	{
	    res = _RootExecution( M4_TRUE, ai_bFromWrapper );

		if (M4_SUCCESS == res)
		{
			res = m_pClChannel->CachePut();

			if (M4_ERROR == res)
			{
                DUMP_CHLOG_WARNINGF(M4_CH_DM_CACHE_PUT_CHANNEL, M4Obj(GetpCMCR()));
				res = M4_SUCCESS;
			}
		}
        else if (M4_ERROR == res)
        {
            DUMP_CHLOG_DEBUGF (M4_CH_DM_ACCESSLOAD, M4Obj(GetpCMCR()));
        }
	}

    return res;
}

m4return_t ClAccess::UnLoad (void)
{
	M4_ASSERT (m_pClChannel);

    return m_pClChannel->UnLoad();
}

m4return_t ClAccess::Persist (m4bool_t ai_bFromWrapper/*=M4_FALSE*/)
{
    m4return_t res = _RootExecution( M4_FALSE, ai_bFromWrapper );
    if (M4_ERROR == res)
    {
        DUMP_CHLOG_DEBUGF (M4_CH_DM_ACCESSPERSIST, M4Obj(GetpCMCR()));
    }
    return res;
}

// ojo: cableada para la Load y la Persist -> saca a capón un elemento
// de la pila después de cada Call

m4return_t ClAccess::LoadFromFile (m4pchar_t ai_pcFileName)
{
    M4_ASSERT (m_pClChannel);

    return m_pClChannel->LoadFromFile(ai_pcFileName);
}

m4return_t ClAccess::LoadFromSttFile (m4pchar_t ai_pcFileName)
{
    M4_ASSERT (m_pClChannel);

    return m_pClChannel->LoadFromSttFile(ai_pcFileName);
}

m4return_t ClAccess::PersistToFile (m4pchar_t ai_pcFileName, m4bool_t ai_bSerializeMCR)
{
    M4_ASSERT (m_pClChannel);

	return m_pClChannel->PersistToFile(ai_pcFileName, ai_bSerializeMCR);
}

m4return_t ClAccess::PersistToTempFile (m4pchar_t ao_pcFileName, m4uint32_t ai_iSize)
{
	M4_ASSERT (m_pClChannel);

	return m_pClChannel->PersistToTempFile(ao_pcFileName, ai_iSize);
}


m4return_t ClAccess::_RootConnectedExecution( m4pcchar_t ai_pccMethod, m4uint8_t ai_iPrecedence, m4bool_t ai_bFromWrapper )
{

    m4return_t			iResult = M4_SUCCESS ;
    m4uint8_t			iQBF = 0 ;
	m4uint8_t			iPrecedence = 0 ;
	m4uint8_t			iCsType = 0 ;
	m4uint8_t			iType = 0 ;
	m4uint8_t			iScope = 0 ;
	m4uint8_t			iVarArgs = 0 ;
    m4uint16_t			i = 0 ;
	m4uint16_t			j = 0 ;
    m4uint16_t			iNodes = 0 ;
    m4uint16_t			iConnected = 0 ;
	m4uint16_t			iArguments = 0 ;
    m4uint32_t			iNode = 0 ;
    m4uint32_t			iItem = 0 ;
    m4uint32_t			iSon = 0 ;
    m4uint32_t			iConnector = 0 ;
	m4VariantType		vValue ;
	ClNode				*poNode = NULL ;
	ClCompiledMCR		*poCMCR = NULL ;


	poCMCR = m_pClChannel->GetpChannelDef()->GetpCMCR() ;
	iNodes = poCMCR->GetChannelNumberOfRoots() ;

	for( i = 0 ; i < iNodes ; i++ )
	{
		iNode = poCMCR->GetChannelRootHandleByPosition( i ) ;
		iQBF = poCMCR->GetNodeIsQBFFilter( iNode ) ;

		if( iQBF == 1 )
		{
			continue ;
		}

		iItem = poCMCR->GetNodeItemHandleById( iNode, ai_pccMethod ) ;

		if( iItem == 0 )
		{
			continue ;
		}

		poNode = Node.Get( ClHandle( iNode ) ) ;

		iConnected = poCMCR->GetItemNumberOfConnectedItems( iItem ) ;

		for( j = 0 ; j < iConnected ; j++ )
		{
			iConnector = poCMCR->GetItemConnectorHandleByPosition( iItem, j ) ;

			iPrecedence = poCMCR->GetItemConnectorPrecedence( iConnector ) ;
			iCsType = poCMCR->GetItemConnectorCsType( iConnector ) ;
			iSon = poCMCR->GetItemConnectorSonItemHandle( iConnector ) ;
			iType = poCMCR->GetItemType( iSon ) ;
			iScope = poCMCR->GetItemScope( iSon ) ;
			iArguments = poCMCR->GetItemNumberOfArguments( iSon ) ;

			if(
					iPrecedence != ai_iPrecedence			// No tiene la misma precedencia
				||	iCsType != M4CL_CSTYPE_MAPPED			// No es insite
				||	iScope == M4CL_ITEM_SCOPE_REGISTER		// Es de registro
				||	iArguments > 0							// Tiene argumentos
				||	iType == M4CL_ITEM_TYPE_PROPERTY		// Es propiedad o campo
				||	iType == M4CL_ITEM_TYPE_FIELD
				)
			{
				continue ;
			}

			iVarArgs = poCMCR->GetItemVariableArguments( iSon ) ;

			if( iVarArgs == 1 )
			{
				vValue.Type = M4CL_CPP_TYPE_NUMBER ;
				vValue.Data.DoubleData = 0 ;

				iResult = poNode->RecordSet.Item.StackPush( vValue ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}

			iResult = poNode->RecordSet.Item[ ClHandle( iSon ) ].Call( NULL, 0, ai_bFromWrapper, M4_FALSE ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = poNode->RecordSet.Item.StackPop( vValue ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

    return( M4_SUCCESS ) ;
}


m4return_t ClAccess::_RootExecution( m4bool_t ai_bLoad, m4bool_t ai_bFromWrapper )
{

    m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bInClient = M4_FALSE ;
	m4uint8_t			iCsType = 0 ;
    m4uint8_t			iQBF = 0 ;
    m4uint16_t			i = 0 ;
    m4uint16_t			iNodes = 0 ;
	m4uint32_t			iMethod = 0 ;
    m4uint32_t			iNode = 0 ;
	m4pcchar_t			pccMethod = NULL ;
	m4VariantType		vValue ;
	ClNode				*poNode = NULL ;
	ClCompiledMCR		*poCMCR = NULL ;


	poCMCR = m_pClChannel->GetpChannelDef()->GetpCMCR() ;

	iMethod = 0 ;

	if( ai_bLoad == M4_TRUE )
	{
		pccMethod = M4CL_ITEM_ROOT_LOAD ;
		iMethod = poCMCR->GetChannelSysLoadItem() ;
	}
	else
	{
		pccMethod = M4CL_ITEM_PERSIST_TREE ;
		iMethod = poCMCR->GetChannelSysPersistItem() ;
	}


// Si no hay items del sistema se hace como antes
	if( iMethod == 0 )
	{
		iNodes = poCMCR->GetChannelNumberOfRoots() ;

		for( i = 0 ; i < iNodes ; i++ )
		{
			iNode = poCMCR->GetChannelRootHandleByPosition( i ) ;
			iQBF = poCMCR->GetNodeIsQBFFilter( iNode ) ;

			if( iQBF == 1 )
			{
				continue ;
			}

			poNode = Node.Get( ClHandle( iNode ) ) ;

			iResult = poNode->RecordSet.Execute( pccMethod, ai_bFromWrapper ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

	    return( M4_SUCCESS ) ;
	}


// Si hay items del sistema se hace de la nueva forma

	iCsType = poCMCR->GetChannelCsType() ;

	if( iCsType == 1.0 )
	{
		bInClient = M4_TRUE ;
	}
	else
	{
		bInClient = M4_FALSE ;
	}


	// En cliente se hacen los pre insite
	if( bInClient == M4_TRUE )
	{
		iResult = _RootConnectedExecution( pccMethod, M4CL_ITEM_CONNECTOR_BEFORE, ai_bFromWrapper ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	// Luego se ejecuta el método y en cliente se le dice que no ejecute los insite de los llamados
	iNode = poCMCR->GetItemNodeHandle( iMethod ) ;
	poNode = Node.Get( ClHandle( iNode ) ) ;

	iResult = poNode->RecordSet.Item[ ClHandle( iMethod ) ].Call( NULL, 0, ai_bFromWrapper, bInClient ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.Item.StackPop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	// En cliente se ejecutan los post insite
	if( bInClient == M4_TRUE )
	{
		iResult = _RootConnectedExecution( pccMethod, M4CL_ITEM_CONNECTOR_AFTER, ai_bFromWrapper ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    return( M4_SUCCESS ) ;
}



m4return_t ClAccess::SetAutoLoadMode (m4uint8_t ai_mode)
{
    m4return_t res;

    if (ai_mode == M4CL_AUTOLOAD_OFF || 
        ai_mode == M4CL_AUTOLOAD_BLOCK || 
        ai_mode == M4CL_AUTOLOAD_PRG || 
        ai_mode == M4CL_AUTOLOAD_NODESAYS) 
    {
        //llamamos a todos los nodos:
        res = Node.SetAutoLoadMode (ai_mode);
    
        if (M4_ERROR != res)
            //se queda como estuviera:
            ClAutoLoad_PlugIn::SetAutoLoadMode (ai_mode);
    }
    else {
        DUMP_CHLOG_ERRORF(M4_CH_DM_INVALIDAUTOLOADMODE, ai_mode << M4Obj(GetpCMCR()));

        res = M4_ERROR;
    }

    return res;
}
m4return_t ClAccess::ReLoad (void)
{
	ClNode *	pnode =0;
	m4uint32_t	hRoot =0 ;// Handle para nodos roots.
	m4uint32_t	hconn = 0;//	para conectores.
	m4int16_t	nic = 0;  // n of inv conn
	m4uint32_t	conntype = 0; 
	m4bool_t	bIsFree = M4_TRUE;

	ClChannelDef *	pcdf= GetpChannel()->GetpChannel_Data()->GetpExtInfo();
	ClNodeDef *		pndf = 0;
	m4int16_t		nRoots = pcdf->NumberOfRoots();
	m4int16_t		nNodes = Node.Count();


	// Primero con los Roots.
	for (--nRoots;nRoots>=0;--nRoots)
	{
		// A cada uno de los Roots le digo que se haga ReLoad.
		hRoot = pcdf->RootHandle(nRoots);

		pnode = Node.Get((ClHandle)hRoot);
		M4_ASSERT (pnode);

		if (pnode->ReLoad()!=M4_SUCCESS)
			return M4_ERROR;
	}


	// Ahora con los Frees.
	// Itero nodo a nodo. Si el nodo solo tiene conectores Free se pueden dar dos casos :
	//  - Que no tenga ningun item de persistencia conectado			=> Estaria entre los Roots.
	//	- Que tenga algun item de persistencia conectado con otro nodo	=> No estaria entre los Roots.
	// Luego si solo tiene conectores Free y no esta entre los Roots, se me ha escapado en la pasada anterior.

	for (--nNodes;nNodes >=0;--nNodes)
	{
		bIsFree = M4_TRUE;
		pnode = Node.Get((ClNodeIndex)nNodes);
		M4_ASSERT (pnode);
		pndf = pnode ->GetpExtInfo();
		nic = pndf->GetNodeNumberOfInvConnectors();
		for (--nic;nic>=0;--nic)
		{
			hconn = pndf->GetNodeInvConnectorHandleByPosition(nic);
			conntype = pndf->GetNodeConnectorType(hconn);
			if (conntype!=M4CL_NODE_CONNECTOR_TYPE_FREE && conntype!=M4CL_NODE_CONNECTOR_TYPE_AUTO)
			{
				bIsFree = M4_FALSE;
				break;
			}
		}
		if (bIsFree && !pndf->IsRoot())
		{
			// Si es Free y no esta entre los Roots
			if (pnode->ReLoad()!=M4_SUCCESS)
				return M4_ERROR;

		}

	}
	return M4_SUCCESS;
}


m4return_t ClAccess::GetSize(ClSize &ao_size)
{
	m4return_t ret = M4_SUCCESS;

	ao_size += sizeof(ClAccess);
		ao_size -= sizeof(ClAutoLoad_PlugIn);
		ao_size -= sizeof(ExecutionDate);
		ao_size -= sizeof(ExecutionStartDate);
		ao_size -= sizeof(ExecutionEndDate);
		ao_size -= sizeof(RunSliceMode);
		ao_size -= sizeof(Property);
		ao_size -= sizeof(Node);


	ret |= ExecutionDate.GetSize(ao_size);
    ret |= ExecutionStartDate.GetSize(ao_size);
    ret |= ExecutionEndDate.GetSize(ao_size);
    ret |= RunSliceMode.GetSize(ao_size);
	
	if (ret != M4_SUCCESS){
		return M4_ERROR;
	}

	if (ClAutoLoad_PlugIn::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	if (Property.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (Node.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


