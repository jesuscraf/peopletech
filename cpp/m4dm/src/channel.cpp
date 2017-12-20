
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

#include "channel.hpp"

#include "cldmcontext.hpp"
#include "m4facdef.hpp"
#include "cllgadap.hpp"
#include "dm_def.hpp"
#include "m4define.hpp"
#include "dm.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "m4var.hpp"
#include "clcodejit.hpp"
#include "clfilemap.hpp"
#include "iexecutor.hpp"
#include "m4mdfac.hpp"
#include "m4mdrt.hpp"
#include "m4objreg.hpp"
#include "curopennotify.hpp"
#include "iproxymanager.hpp"
#include "m4lang.hpp"
#include "sttlib.hpp"
#include "sttwrap.hpp"
#include "sttchn.hpp"
#include "datacache.hpp"
#include "clfileiod.hpp"
#include "chandata_ser.hpp"
#include "file_misc.hpp"
#include "file_manager.hpp"
#include "not_cdat.hpp"
#include "m4notifier.hpp"
#include "m4event.hpp"
#include "m4objglb.hpp"
#include "m4compress.hpp"
#include "objdatacacheinterface.hpp"
#include "m4java.h"
#include "m4lgadapdef.hpp"
#include "bookmark.hpp"
#include "m4jcontext.hpp"
#include "m4jmanager.hpp"


typedef struct StChannelRefs_tag
{
	m4uint32_t	m_iReferences ;
	m4bool_t	m_bProcessed ;

} StChannelRefs_t; 

typedef map< ClChannel*, StChannelRefs_t, less<const ClChannel*> >	MapChannelRefs;
typedef MapChannelRefs::iterator									itMapChannelRefs;



// ==========================================================================
// Array para los nombres de las propiedades fijas de M4Obj
// Se emplea para optimizar la serialización
// Tienen que estar ordenadas por ID !!!!!!!!!!!!!!!!!!!!!!!!

m4pchar_t   g_acM4ObjProperties[ M4_DM_M4OBJ_PROPERTIES_NUMBER ] =
{
	M4CMPROP_CHANNEL_APPLY_MAPPINGS,			// APPLY_MAPPINGS
	M4CMPROP_CHANNEL_CURRENCY_PRECISION,		// CURRENCY_PRECISION
	M4CMPROP_CHANNEL_DATA_CORRECTION_DATE,		// DATA_CORRECTION_DATE
	M4CMPROP_CHANNEL_DATA_END_DATE,				// DATA_END_DATE
	M4CMPROP_CHANNEL_DATA_LANGUAGE,				// DATA_LANGUAGE
	M4CMPROP_CHANNEL_DATA_START_DATE,			// DATA_START_DATE
	M4CMPROP_CHANNEL_EXECUTE_REALSQL_MODE,		// EXECUTE_REALSQL_MODE
	M4CMPROP_CHANNEL_EXECUTION_DATE,			// EXECUTION_DATE
	M4CMPROP_CHANNEL_EXECUTION_END_DATE,		// EXECUTION_END_DATE
	M4CMPROP_CHANNEL_EXECUTION_START_DATE,		// EXECUTION_START_DATE
	M4CMPROP_CHANNEL_ID_APP_ROLE,				// ID_APP_ROLE
	M4CMPROP_CHANNEL_ID_ORGANIZATION,			// ID_ORGANIZATION
	M4CMPROP_CHANNEL_ID_ORGANIZATION_TYPE,		// ID_ORGANIZATION_TYPE
	M4CMPROP_CHANNEL_ID_RSM,					// ID_RSM
	M4CMPROP_CHANNEL_ID_TRAMITATION,			// ID_TRAMITATION
	M4CMPROP_CHANNEL_CORRECTION_DATE,			// META_DATA_CORRECTION_DATE
	M4CMPROP_CHANNEL_END_DATE,					// META_DATA_END_DATE
	M4CMPROP_CHANNEL_START_DATE,				// META_DATA_START_DATE
	M4CMPROP_CHANNEL_META_IDENTIFIER,			// META_IDENTIFIER
	M4CMPROP_CHANNEL_NUMBER_PRECISION,			// NUMBER_PRECISION
	M4CMPROP_CHANNEL_ROUND_CURRENCY,			// ROUND_CURRENCY
	M4CMPROP_CHANNEL_ROUND_NUMBER,				// ROUND_NUMBER
	M4CMPROP_CHANNEL_UNIT_DATE					// UNIT_DATE
} ;



// ==========================================================================

ClChannel::ClChannel( m4uint32_t ai_iHandle )

:	ApplyMappings		( M4_PROP_STATIC_MAPPED    )
,	MetaIdentifier		( M4_PROP_STATIC_MAPPED    )
,	StartDate			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	EndDate				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	CorrectionDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_StartDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_EndDate			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_ExecutionDate	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_StartDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_EndDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_CorrectionDate	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_Language		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Meta_Language		( M4_PROP_STATIC_NO_MAPPED | M4_PROP_SESSION )
,	UnitDate			( M4_PROP_STATIC_MAPPED    )
,	LDB_LastLogicConn	( M4_PROP_STATIC_NO_MAPPED )
,	CsType				( M4_PROP_STATIC_NO_MAPPED )
,	RSM_ID				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Role_ID				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Organization		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	OrganizationType	( M4_PROP_STATIC_MAPPED    )
,	Tramitation			( M4_PROP_STATIC_MAPPED    )
,	RoundCurrency		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	RoundNumber			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	CurrencyPrecision	( M4_PROP_STATIC_MAPPED    )
,	NumberPrecision		( M4_PROP_STATIC_MAPPED    )
,	ExecuteRealSQLMode	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )

,	m_pDestroy_Notifier (new ClNotifier)

{    
    m_poClChannelManager = 0;

    _InitCommon( ai_iHandle );
}

ClChannel::ClChannel( ClChannelManager *ai_pcm, m4uint32_t ai_iHandle )

:	ApplyMappings		( M4_PROP_STATIC_MAPPED    )
,	MetaIdentifier		( M4_PROP_STATIC_MAPPED    )
,	StartDate			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	EndDate				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	CorrectionDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_StartDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_EndDate			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	VM_ExecutionDate	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_StartDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_EndDate		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_CorrectionDate	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Data_Language		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Meta_Language		( M4_PROP_STATIC_NO_MAPPED | M4_PROP_SESSION )
,	UnitDate			( M4_PROP_STATIC_MAPPED    )
,	LDB_LastLogicConn	( M4_PROP_STATIC_NO_MAPPED )
,	CsType				( M4_PROP_STATIC_NO_MAPPED )
,	RSM_ID				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Role_ID				( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	Organization		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	OrganizationType	( M4_PROP_STATIC_MAPPED    )
,	Tramitation			( M4_PROP_STATIC_MAPPED    )
,	RoundCurrency		( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	RoundNumber			( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )
,	CurrencyPrecision	( M4_PROP_STATIC_MAPPED    )
,	NumberPrecision		( M4_PROP_STATIC_MAPPED    )
,	ExecuteRealSQLMode	( M4_PROP_STATIC_MAPPED    | M4_PROP_SESSION )

,	m_pDestroy_Notifier (new ClNotifier)

{    
	m_poClChannelManager = 0;	//antes de llamar a Attach
	Attach (ai_pcm);

    _InitCommon( ai_iHandle );
}



void ClChannel::_InitCommon( m4uint32_t ai_iHandle )
{

	m4VariantType	vValue ;
	ClLogonAdaptor	*pLA = NULL ;
	ClVMBaseEnv		*poEnviroment = NULL ;


	m_iHandle = ai_iHandle ;
	m_pIObjDataCache = NULL ;

	m_bIsSynchronized = M4_TRUE;
	m_bIsDestroying = M4_FALSE;

    m_pClChannel_Data = 0;

	m_oGroupInfo.Init(this);

	m_poMainLConn = NULL;

	// for proxy's
	m_iRemoteId = 0;

	m_eDebugMode = IS_NOT_DEBUG;

    m_iRecSerMod = M4CL_RECORD_SERIAL_DELTA;
    m_iIteSerMod = M4CL_ITEM_SERIAL_MAPPED;

//Level2
	Level2.Init (this);


// Inicialización de las propiedades ======================

    //WantsAutoLoad
    //se inicializa sola

	// Los datos de mapeo
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;
	vValue.Data.DoubleData = 1 ;	// Por defecto se aplican mapeos
	ApplyMappings.Set( vValue ) ;

	MetaIdentifier.SetNull() ;


	// Cogemos las fechas y la sociedad del canal sesión
	if( m_poClChannelManager != NULL && m_poClChannelManager->GetSessionChannel() )
	{
		pLA = m_poClChannelManager->GetLogonAdaptor() ;
	}

    vValue.Type = M4CL_CPP_TYPE_DATE;

	if( pLA != NULL )
	{
		vValue.Data.DoubleData = pLA->GetStartDate() ;
		StartDate.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetEndDate() ;
		EndDate.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetCorrDate() ;
		CorrectionDate.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetVMStartDate() ;
		VM_StartDate.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetVMEndDate() ;
		VM_EndDate.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetVMCorrDate() ;
		VM_ExecutionDate.Set( vValue ) ;

        vValue.Data.DoubleData = pLA->GetLDBStartDate() ;
		Data_StartDate.Set( vValue ) ;

        vValue.Data.DoubleData = pLA->GetLDBEndDate() ;
		Data_EndDate.Set( vValue ) ;

        vValue.Data.DoubleData = pLA->GetLDBCorrDate() ;
		Data_CorrectionDate.Set( vValue ) ;
	}
	else
	{
		vValue.Data.DoubleData = ClMinusInfiniteDate() ;
		StartDate.Set( vValue ) ;
		VM_StartDate.Set( vValue ) ;
		Data_StartDate.Set( vValue ) ;

		vValue.Data.DoubleData = ClPlusInfiniteDate() ;
		EndDate.Set( vValue ) ;
		VM_EndDate.Set( vValue ) ;
		Data_EndDate.Set( vValue ) ;

		vValue.Data.DoubleData = ClActualDate() ;
		CorrectionDate.Set( vValue ) ;
		VM_ExecutionDate.Set( vValue ) ;
		Data_CorrectionDate.Set( vValue ) ;
	}


	// Los lenguajes se inicializan a null para que se coja lo que pone en base de datos
	// si es que no se sobreescribe

	Data_Language.SetNull() ;
	Meta_Language.SetNull() ;


	// Las fechas de datos y la unidad ya NO se inicializan en el channel_data

	//UnitDate
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;
	vValue.Data.DoubleData = M4_ONE_DAY ;   // valor por defecto
	UnitDate.Set( vValue ) ;


	// La última conexión
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;
	vValue.Data.DoubleData = -1 ;   // valor por defecto
	LDB_LastLogicConn.Set( vValue ) ;

	// EL cs type
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;
	if( m_poClChannelManager != NULL )
	{
		vValue.Data.DoubleData = m_poClChannelManager->GetDMContext()->GetCSType();
	}
	else
	{
		vValue.Data.DoubleData = M4CL_CSTYPE_BOTH ;
	}

	CsType.Set( vValue ) ;

	// El RSM y el Role
	RSM_ID.SetNull() ;
	Role_ID.SetNull() ;

	// Sociedad y tipo
	Organization.SetNull() ;
	OrganizationType.SetNull() ;

	// Tramitación
	Tramitation.SetNull() ;

	// El redondeo a precisión y la precisión
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;

	vValue.Data.DoubleData = -1 ;
	CurrencyPrecision.Set( vValue ) ;
	NumberPrecision.Set( vValue ) ;

	vValue.Data.DoubleData = 0 ;
	RoundCurrency.Set( vValue ) ;
	RoundNumber.Set( vValue ) ;

	if( pLA != NULL )
	{
		vValue.Data.DoubleData = pLA->GetRoundCurrency() ;
		RoundCurrency.Set( vValue ) ;

		vValue.Data.DoubleData = pLA->GetRoundNumber() ;
		RoundNumber.Set( vValue ) ;
	}


	// Los permisos de ejecución del RealSQL
	vValue.Type = M4CL_CPP_TYPE_NUMBER ;

	if( pLA != NULL )
	{
		vValue.Data.DoubleData = pLA->GetExecuteRealSQLMode() ;
	}
	else
	{
		vValue.Data.DoubleData = 0 ;
	}

	ExecuteRealSQLMode.Set( vValue ) ;


	// Atachamos la de la sociedad
    Organization.Attach( this ) ;

	// Las ponemos en la colección (salvo la del lastlogicconnection)
    Property.AddProperty( M4CMPROP_CHANNEL_WANTS_AUTOLOAD,			&(this->ClAutoLoad_PlugIn::WantsAutoLoad) ) ;

	Property.AddProperty( M4CMPROP_CHANNEL_APPLY_MAPPINGS,			&ApplyMappings ) ;
	Property.AddProperty( M4CMPROP_CHANNEL_META_IDENTIFIER,			&MetaIdentifier ) ;

	Property.AddProperty( M4CMPROP_CHANNEL_START_DATE,				&StartDate ) ;
	Property.AddProperty( M4CMPROP_CHANNEL_END_DATE,				&EndDate ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_CORRECTION_DATE,			&CorrectionDate ) ;

	Property.AddProperty( M4CMPROP_CHANNEL_EXECUTION_START_DATE,	&VM_StartDate ) ;
	Property.AddProperty( M4CMPROP_CHANNEL_EXECUTION_END_DATE,		&VM_EndDate ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_EXECUTION_DATE,			&VM_ExecutionDate ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_DATA_START_DATE,			&Data_StartDate ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_DATA_END_DATE,			&Data_EndDate ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_DATA_CORRECTION_DATE,	&Data_CorrectionDate ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_DATA_LANGUAGE,			&Data_Language ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_META_LANGUAGE,			&Meta_Language ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_UNIT_DATE,				&UnitDate ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_ID_RSM,					&RSM_ID ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_ID_APP_ROLE,				&Role_ID ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_ID_ORGANIZATION,			&Organization ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_ID_ORGANIZATION_TYPE,	&OrganizationType ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_ID_TRAMITATION,			&Tramitation ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_ROUND_CURRENCY,			&RoundCurrency ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_ROUND_NUMBER,			&RoundNumber ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_CURRENCY_PRECISION,		&CurrencyPrecision ) ;
    Property.AddProperty( M4CMPROP_CHANNEL_NUMBER_PRECISION,		&NumberPrecision ) ;

    Property.AddProperty( M4CMPROP_CHANNEL_EXECUTE_REALSQL_MODE,	&ExecuteRealSQLMode ) ;

// ====================================================

// Security
	m_iIsLevel2 = M4_FALSE ;
	m_iIsSerialized = M4_FALSE ;

//JIT
	m_poCodeJIT = 0;

//FileTable
	m_poFileTable = 0;

//Statistics
   m_iSttInit = M4_VM_STATISTICS_DISABLED;
}



ClChannel::~ClChannel ()
{
	// Si es un cliente auto con remoto hay que guardar para borrar en server
	SetRemoteId( 0 ) ;

	// Si se tiene cache de auto hay que guardarlo
	if( m_pClChannel_Data != NULL && m_pIObjDataCache != NULL )
	{
		m4pcchar_t pccChannelId = GetpChannelDef()->Id() ;
		m_pIObjDataCache->PutObject( m_iHandle, pccChannelId, m_pClChannel_Data ) ;
	}

	// Destruir los objetos Java asociados al canal.
	M4JVM_DestroyAllObjects(m_iHandle);

	ReleaseJavaContexts( M4_FALSE ) ;

	ClSttStateTimer * poSttStateTimer = 0;
	
	if (M4_TRUE == IsBuilt())
	{
		//Inicio toma tiempos Stt
		poSttStateTimer = new ClSttStateTimer(m_poClChannelManager ? m_poClChannelManager->GetSttWrapper()->GetStatistics() : 0, 
			m_poClChannelManager ? m_poClChannelManager->GetSttWrapper()->GetStatisticsLevel() : 0, 
			M4_VM_STT_SPECIAL_ITEM_DESTROY_CHANNEL);
		
		if ( M4_TRUE == poSttStateTimer->GetSttEnabled() )
		{
			if (poSttStateTimer->GetStatistics()->sttGetInitState() & M4_VM_STATISTICS_ENABLED_PARAMS)
			{
				m4VariantType vValue;
				
				m4pcchar_t pcChannelId = GetpChannelDef()->Id();
				poSttStateTimer->GetStatistics()->sttAddExecParam((m4pchar_t)pcChannelId);
				
				Meta_Language.Get( vValue ) ;
				poSttStateTimer->GetStatistics()->sttAddExecParam(vValue.Data.DoubleData);
				
				CsType.Get( vValue ) ;
				poSttStateTimer->GetStatistics()->sttAddExecParam(vValue.Data.DoubleData);
			}
		}
	}
	
	//comprobación provisional:
	M4_ASSERT (Access.Count () == 0);	// Deleting a channel with living Accesses.
	
	// proxy's. Sólo en la parte cliente (se comprueba dentro).
	ReleaseProxyContext();
	
	//proceso estandar de borrado de un kernel:
	if (m_pClChannel_Data)
	{
		m_pClChannel_Data->GetpNotifier()->Detach(this);	// Me borro del notifier del channel_data.

		// if the channel is modified we have to disable de cache entry
		m4bool_t bRemoveFromCache = M4_FALSE;

		if( m_pClChannel_Data->GetCO() != NULL )
		{
			CheckModify( bRemoveFromCache );
		}

		// releases channel data
		m_pClChannel_Data->Destroy(bRemoveFromCache);
		m_pClChannel_Data = 0;
	}
	
	//metacanal back
	BackMCR.Destroy ();
	
	//JIT
	if ( m_poCodeJIT )
	{
		delete m_poCodeJIT;
		m_poCodeJIT = NULL;
	}
	
	//FileTable
	if ( m_poFileTable )
	{
		delete m_poFileTable;
		m_poFileTable = NULL;
	}
	
	//Fin toma tiempos de Stt
	if (poSttStateTimer)
		delete poSttStateTimer;
	
	if (m_poClChannelManager)
	{
		if (m_iSttInit != M4_VM_STATISTICS_DISABLED)
		{
			m_poClChannelManager->GetSttWrapper()->DecStatisticsRef();
		}
	}
	
	DetachFromChannelManager ();
	
	if (m_pDestroy_Notifier) {
		delete m_pDestroy_Notifier;
		m_pDestroy_Notifier = 0;
	}

	itBookMarkMap	it ;

	while( !m_oBookMarks.empty() )
	{
		it = m_oBookMarks.begin() ;
		delete( it->second ) ;
		m_oBookMarks.erase( it ) ;
	}
}



//----------------------------------------------------------------------------
// Destroy
//      siempre que se cree un canal con ClChannelManager::CreateChannel, la destrucción
//      se debe hacer llamando a ClChannel_Kernel::Destroy, nunca directamente al destructor:
//----------------------------------------------------------------------------

/* Bug 0130799
Función que añade de manera recursiva las referencias de los accesos de nivel 2
*/
m4return_t	_AddReferences( ClChannel* ai_poChannel, MapChannelRefs& ai_roReferences )
{

	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			i = 0 ;
	m4uint32_t			j = 0 ;
	m4uint32_t			iLength = 0 ;
	m4uint32_t			iChannels = 0 ;
	ClChannel			*poChannel = NULL ;
	ClChannel			*poUsedChannel = NULL ;
	ClAccess			*poAccess = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	StChannelRefs_t		stReferences ;
	itMapChannelRefs	it ;


	// Si el objeto ya está en el mapa no se procesa
	it = ai_roReferences.find( ai_poChannel ) ;

	if( it != ai_roReferences.end() )
	{
		return( M4_SUCCESS ) ;
	}


	iLength = ai_poChannel->Access.Count() ;
	poAccess = ai_poChannel->Level2.GetSelfAccess() ;

	if( poAccess != NULL )
	{
		iLength-- ;
	}

	// Se añade el elemento con sus referencias menos la propia
	stReferences.m_iReferences = iLength ;
	stReferences.m_bProcessed = M4_FALSE ;
	ai_roReferences.insert( MapChannelRefs::value_type( ai_poChannel, stReferences ) ) ;

	if( iLength == 0 )
	{
		// Si nadie lo referencia no hace falta buscar los referenciadores
		return( M4_SUCCESS ) ;
	}

	poChannelManager = ai_poChannel->GetpChannelManager() ;

	if( poChannelManager == NULL )
	{
		return( M4_SUCCESS ) ;
	}

	// Se recorren los canales añadiendo los que referencian a este
	iChannels = poChannelManager->Channel.Count() ;

	for( i = 0 ; i < iChannels ; i++ )
	{
		poChannel = poChannelManager->Channel[ i ] ;
		iLength = poChannel->Level2.m_oInstanceMap.Count() ;

		for( j = 0 ; j < iLength ; j++ )
		{
			poAccess = poChannel->Level2.m_oInstanceMap.GetAccess( j ) ;
			poUsedChannel = poAccess->GetpChannel() ;

			if( poUsedChannel == ai_poChannel )
			{
				iResult = _AddReferences( poChannel, ai_roReferences ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}

	return( M4_SUCCESS ) ;
}


/* Bug 0130799
Función que decrementa de manera recursiva las referencias de los accesos de nivel 2
*/
m4return_t	_DeleteReferences( ClChannel* ai_poChannel, MapChannelRefs& ai_roReferences )
{

	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iLength = 0 ;
	ClChannel			*poChannel = NULL ;
	ClAccess			*poAccess = NULL ;
	itMapChannelRefs	it ;


	// Si el objeto no está en el mapa no se procesa
	it = ai_roReferences.find( ai_poChannel ) ;

	if( it == ai_roReferences.end() )
	{
		return( M4_SUCCESS ) ;
	}

	// Si está y ya se ha procesado no se vuelve a procesar
	if( it->second.m_bProcessed == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	// Se marca como procesado
	it->second.m_bProcessed = M4_TRUE ;


	// Se recorren los canales referenciados por este decrementando sus referencias
	iLength = ai_poChannel->Level2.m_oInstanceMap.Count() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poAccess = ai_poChannel->Level2.m_oInstanceMap.GetAccess( i ) ;
		poChannel = poAccess->GetpChannel() ;

		// Se descuentan las referencias
		it = ai_roReferences.find( poChannel ) ;

		if( it != ai_roReferences.end() )
		{
			it->second.m_iReferences-- ;

			if( it->second.m_bProcessed == M4_FALSE )
			{
				iResult = _DeleteReferences( poChannel, ai_roReferences ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}

	return( M4_SUCCESS ) ;
}


/* Bug 0130799
Función que calcula si un canal tendría referencias pendientes borrando sus niveles 2
*/
m4return_t	_ProcessReferences( ClChannel* ai_poChannel, m4bool_t& ao_rbCanDelete )
{

	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			iReferences = 0 ;
	ClChannel			*poChannel = NULL ;
	MapChannelRefs		oReferences ;
	MapChannelRefs		oDeletes ;
	itMapChannelRefs	it ;


	ao_rbCanDelete = M4_FALSE ;

	// Se añaden recursivamente las referencias
	iResult = _AddReferences( ai_poChannel, oReferences ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se restan recursivamente las referencias
	iResult = _DeleteReferences( ai_poChannel, oReferences ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Si alguna no tiene 0 referencias no se puede borrar
	it = oReferences.begin();

	while( it != oReferences.end() )
	{
		iReferences = it->second.m_iReferences ;

		if( iReferences != 0 )
		{
			return( M4_SUCCESS ) ;
		}

		it++ ;
	}

	ao_rbCanDelete = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


m4return_t ClChannel::Destroy (void)
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bCanDelete = M4_FALSE ;
	m4uint32_t	iLength = 0 ;


	// Si ya se está en proceso de destrucción no se vuelve a pasar
	if( m_bIsDestroying == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	ClEvent oEvent(this, M4_NOTIF_CHANNEL_DESTROY);
	m_pDestroy_Notifier->Signal(oEvent);

	/* Bug 0130799
	Se comprueba si se borrasen todos los accesos de nivel 2 recursivamente
	que no quedarían referencias a ningún canal
	Si no quedan se puede destruir el canal
	Primero se borrar los niveles 2
	*/
	iLength = Access.Count() ;

	if( iLength > 0 )
	{
		// Se procesa recursivamente las referencias
		iResult = _ProcessReferences( this, bCanDelete ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( bCanDelete == M4_FALSE )
		{
			DUMP_CHLOG_DEBUGF( M4_CH_DM_DESTROYWACCESS, M4Obj(GetpChannelDef()->GetpCMCR()) << iLength ) ;
			return( M4_ERROR ) ;
		}
	}

	m_bIsDestroying = M4_TRUE ;
	Level2.EraseAllL2Instances( M4_TRUE, M4_FALSE, NULL ) ;
	m_bIsDestroying = M4_FALSE ;

	// Por si acaso finalmente no se puede borrar
	iLength = Access.Count() ;

	if( iLength > 0 )
	{
		DUMP_CHLOG_DEBUGF( M4_CH_DM_DESTROYWACCESS, M4Obj(GetpChannelDef()->GetpCMCR()) << iLength ) ;
		return( M4_ERROR ) ;
	}
	
	// me suicido:
	delete this;
	
	return M4_SUCCESS;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ClChannel_Data *ClChannel :: NewChannel_Data( void )
{

    ClChannel_Data	*pcd ;


    pcd = new ClChannel_Data() ;

	CHECK_CHLOG_DEBUGF( pcd == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClChannel_Data ) ) << __LINE__ << __FILE__ ) ;

    return( pcd ) ;
}


//----------------------------------------------------------------------------
// CommonInitAfterBuild
//      Inicialización común después de hacer la build
//      Esta función es llamada por el resto de funciones 'build'
//----------------------------------------------------------------------------

m4return_t ClChannel :: CommonInitAfterBuild( void )
{

    m4return_t	iResult ;


    //inicializo el channel_data
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_pClChannel_Data->Init() ;

    if( M4_ERROR == iResult )
	{
        return( iResult ) ;
	}

	/* Bug 0102652
	Las propiedades se inicializan ahora en una función separada
	*/
	iResult = InitProperties() ;

    if( M4_ERROR == iResult )
	{
        return( iResult ) ;
	}

	// proxy's. Esto sólo hace falta cuando estoy en cliente (se comprueba dentro)
	iResult = GetProxyContext() ;

    return( iResult ) ;
}



m4return_t ClChannel :: InitProperties( void )
{

	m4uint8_t		iType ;
	m4uint8_t		iNoWrite ;
	m4language_t	lLanguage ;
	m4language_t	lMetaLanguage ;
	m4double_t		dType ;
	m4pcchar_t		pccThisT3Id ;
	m4VariantType	vValue ;
	ClLogonAdaptor	*poLogonAdaptor ;
    ClCompiledMCR   *poCMCR ;



//Danielb.
//En el método LoadFromIOD se hacen los mismos pasos de inicialización de las props de organización.
//Si se modifica este código, comprobar en LoadFromIOD.


    poCMCR = GetpChannelDef()->GetpCMCR() ;


// Si las metapropiedades no están rellenas las rellenamos con los valores del metacanal
	MetaIdentifier.Get( vValue ) ;

	if( vValue.Type == M4CL_CPP_TYPE_NULL )
	{
		pccThisT3Id = poCMCR->GetChannelId() ;
		MetaIdentifier.Set( (m4pchar_t) pccThisT3Id ) ;
	}


// Si el tipo de sociedad es nulo es que nadie lo ha rellenado y cogemos el del
// metacanal. Esto puede ser un problema en el futuro
// Si se sale de rango tb lo cogemos del metacanal

	OrganizationType.Get( vValue ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		dType = poCMCR->GetChannelOrganizationType() ;
		OrganizationType.Set( dType ) ;

		if( vValue.Type != M4CL_CPP_TYPE_NULL )
		{
			DUMP_CHLOG_WARNINGF( M4_CH_DM_BAD_ORG_TYPE_TYPE, m4uint8_t( vValue.Type ) << LogCat << " - " << LogCat << M4ClCppType( vValue.Type ) <<  M4ClOrganizationType( m4uint8_t( dType ) ) ) ;
		}
	}
	else if( vValue.Data.DoubleData < M4CL_ORGANIZATION_TYPE_NONE || vValue.Data.DoubleData > M4CL_ORGANIZATION_TYPE_MULTI )
	{
		dType = poCMCR->GetChannelOrganizationType() ;
		OrganizationType.Set( dType ) ;

		DUMP_CHLOG_WARNINGF( M4_CH_DM_BAD_ORG_TYPE, m4uint8_t( vValue.Data.DoubleData ) << m4uint8_t( M4CL_ORGANIZATION_TYPE_NONE ) << LogCat << " - " << LogCat << M4ClOrganizationType( M4CL_ORGANIZATION_TYPE_NONE ) << m4uint8_t( M4CL_ORGANIZATION_TYPE_MULTI ) << LogCat <<  " - " << LogCat << M4ClOrganizationType( M4CL_ORGANIZATION_TYPE_MULTI ) << M4ClOrganizationType( m4uint8_t( dType ) ) ) ;
	}


// Ponemos los idiomas
// El de metadatos es con el que acaba el metacanal

	lMetaLanguage = poCMCR->GetChannelLanguage() ;
	Meta_Language.Set( m4double_t( lMetaLanguage ) ) ;

// El de datos es el que venía y sino venía es el del canal sesión que diga el metacanal
// Si no hay canal sesión le pongo el de metadatos
	Data_Language.Get( vValue ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		lLanguage = lMetaLanguage ;

		if( m_poClChannelManager != NULL )
		{
			poLogonAdaptor = m_poClChannelManager->GetLogonAdaptor() ;

			if( poLogonAdaptor != NULL )
			{
				pccThisT3Id = poCMCR->GetChannelId() ;
				iType = poCMCR->GetChannelDataLanguage() ;

				if( iType == M4CL_LANGUAGE_TYPE_LOGON )
				{
					lLanguage = poLogonAdaptor->GetSessionLanguage( pccThisT3Id, M4CL_LANGUAGE_NONE ) ;
				}
				else
				{
					lLanguage = poLogonAdaptor->GetSessionDevLanguage( pccThisT3Id, M4CL_LANGUAGE_NONE ) ;
				}
			}
		}
	}
	else
	{
		lLanguage = (m4language_t) vValue.Data.DoubleData ;
	}

	/* Bug 0069089
	Hay que pasar un lenguaje válido
	*/
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( lLanguage ) ;

	Data_Language.Set( m4double_t( lLanguage ) ) ;
 
// Seteamos las propiedades a valores de sólo lectura
	iNoWrite = M4_PROP_WRITE | M4_PROP_WRITE_COL ;

	ApplyMappings.UnSetFlags( iNoWrite ) ;
	MetaIdentifier.UnSetFlags( iNoWrite ) ;
	StartDate.UnSetFlags( iNoWrite ) ;
	EndDate.UnSetFlags( iNoWrite ) ;
	CorrectionDate.UnSetFlags( iNoWrite ) ;
	Meta_Language.UnSetFlags( iNoWrite ) ;
	CsType.UnSetFlags( iNoWrite ) ;
	RSM_ID.UnSetFlags( iNoWrite ) ;
	Role_ID.UnSetFlags( iNoWrite ) ;
	OrganizationType.UnSetFlags( iNoWrite ) ;
	ExecuteRealSQLMode.UnSetFlags( iNoWrite ) ;

    return( M4_SUCCESS ) ;
}



m4return_t	ClChannel::CreateChannel_Data (ClChannel_Data *ai_poChannelData)
{
	/* Bug 0070857
	No se deja hacer una build sobre un canal que ya está construido
	*/
	CHECK_CHLOG_ERROR( m_pClChannel_Data != NULL, M4_ERROR, M4_CH_DM_M4OBJ_AREADY_BUILT ) ;

	if( ai_poChannelData != NULL )
	{
		m_pClChannel_Data = ai_poChannelData;
	}
	else
	{
		m_pClChannel_Data = NewChannel_Data ();
	}

	CHECK_CHLOG_ERRORF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    //me asocio al notifier del nuevo channel_data:
    m_pClChannel_Data->GetpNotifier ()->Attach (this);

	return( M4_SUCCESS ) ;
}


m4return_t ClChannel::ReleaseProxyContext(void)
{
	// checks the channel manager member
	if (m_poClChannelManager == NULL) return M4_SUCCESS;

	if (!m_poClChannelManager->GetDMContext()->ExecutingInClient()) return M4_SUCCESS;

	// gets the channel manager and checks the proxy space
	IProxyManager* pProxyManager = m_poClChannelManager->GetProxyManager();

	//ShutDown correcto en Single Machine con canales Proxy (11/5/01)
	if (pProxyManager == NULL) return M4_SUCCESS;

	// Obtengo el espacio proxy. No necesito id porque en cliente sólo hay uno.
	// Le digo que no quiero que me cree uno nuevo en caso de que no exista.
	ClProxySpace* pClProxySpace = pProxyManager->GetSpace("Proxy", M4_FALSE);

	// if we don´t have a proxy space we just leave the game
	if (pClProxySpace == NULL) return M4_SUCCESS;

	// checks for server mode (debug only)
	if (pProxyManager->IsServerMode() == M4_TRUE) return M4_SUCCESS;

	// scans the channel manager for proxy's channels
	ClChannelDirectory& channelDirectory = m_poClChannelManager->Channel;
	m4uint32_t iNChannels = channelDirectory.Count();
	m4bool_t bKeepSpace = M4_FALSE;

	for (m4uint32_t lCounter = 0; lCounter < iNChannels; lCounter++) {
		ClChannel *poClChannel = channelDirectory[lCounter];
		if (poClChannel == this || poClChannel->IsBuilt() == M4_FALSE) continue;

		if (poClChannel->GetpChannelDef()->CsExeType() == M4CL_CSEXETYPE_PROXY) {
			bKeepSpace = M4_TRUE;
			break;
		}
	}

	/* Bug 0074293
	La referencia se tiene que borrar aunque falle el release space
	*/
	// releases the reference
	m_iRemoteId = 0;

	// erases the remote space
	if (bKeepSpace == M4_FALSE) {
		m4return_t iRet = pProxyManager->ReleaseSpace(pClProxySpace);
		if (iRet != M4_SUCCESS) return iRet;
	}

	return M4_SUCCESS;
}


/* Bug 0102671
Cuando se pasa de tener remoteid a tener otro o no tenerlo hay que hacer un
tratamiento especial. Si el canal es auto eso significa que pierde contacto
con la parte servidora y hay que marcar el auto del server para borrar.
*/
void ClChannel::SetRemoteId(const m4uint32_t ai_iRemoteId)
{
	// Si no cambia el remoteid no se hace nada
	if( m_iRemoteId == ai_iRemoteId )
	{
		return ;
	}

	if( m_poClChannelManager == NULL )
	{
		return ;
	}

	if( m_poClChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
	{
		return ;
	}

	/* Bug 0116384
	Si es un canal de depuración no hay que matar la parte server, porque
	ya el canal original la borrará cuando se muera. Hay dos canales apuntando
	al remoto, pero no pertenece al de depuración
	*/
	if( m_iRemoteId != 0 && m_pClChannel_Data != NULL && m_eDebugMode == IS_NOT_DEBUG )
	{
		ClCompiledMCR	*poCMCR = m_pClChannel_Data->GetpChannelDef()->GetpCMCR() ;

		if( poCMCR != NULL && poCMCR->GetChannelCsExeType() == M4CL_CSEXETYPE_AUTO )
		{
			m_poClChannelManager->AddDeletedAuto( m_iRemoteId ) ;
		}
	}

	m_iRemoteId = ai_iRemoteId ;
}


m4return_t ClChannel::GetProxyContext(void)
{
	if (!m_pClChannel_Data)
		return M4_ERROR;

	// checks for proxy mode
	ClChannelDef* pChannelDef = GetpChannelDef();
	if (pChannelDef->CsExeType() != M4CL_CSEXETYPE_PROXY)
		return M4_SUCCESS;

	if (m_poClChannelManager == NULL)
		return M4_SUCCESS;

	if (!m_poClChannelManager->GetDMContext()->ExecutingInClient())
		return M4_SUCCESS;
	
	// checks the IS_SEPARABLE flag
	if (!pChannelDef->IsSeparable())
		return M4_SUCCESS;
	
	// Gets the channel manager and checks the proxy space
	IProxyManager* pProxyManager = m_poClChannelManager->GetProxyManager();

	// gets the proxy space - it should come from meta data
	if (pProxyManager->GetSpace("Proxy") == NULL)
		return M4_ERROR;

	return M4_SUCCESS;
}
	
m4return_t ClChannel::BuildFromMCR (ClCompiledMCR *ai_pcmcrfront, ClLaneArgumnent *ai_poLane)
{
    m4return_t		res;

    res = CreateChannel_Data (NULL);

	if( res != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    res = m_pClChannel_Data->GetpExtInfo ()->MCR.BuildFromMCR (ai_pcmcrfront, ai_poLane);
    
    if (M4_ERROR != res) {
        res = CommonInitAfterBuild ();
    }    

    return res;
}


m4return_t ClChannel::BuildBackFromMCR( ClCompiledMCR *ai_pcmcrback )
{

    m4return_t	iResult ;


    iResult = BackMCR.BuildFromMCR( ai_pcmcrback ) ;

	return( iResult ) ;
}


m4return_t ClChannel::BuildFromFile (m4pcchar_t ai_path)
{
    m4return_t		res;
	m4uint8_t		iCsType ;
    m4VariantType	vCsType;

	CsType.Get( vCsType ) ;
	iCsType = m4uint8_t( vCsType.Data.DoubleData ) ;

    res = CreateChannel_Data (NULL);

	if( res != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    res = m_pClChannel_Data->GetpExtInfo ()->MCR.BuildFromFile (GetpFactory (), iCsType, ai_path);

	if( iCsType == M4CL_CSTYPE_FRONT )
	{
        res = BackMCR.BuildFromFile (GetpFactory (), M4CL_CSTYPE_BACK, ai_path);
		if (M4_ERROR == res)
			return res;
	}

    if (M4_ERROR != res) {
        res = CommonInitAfterBuild ();
    }    

    return res;
}

m4return_t ClChannel::BuildFromId (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane, m4uint8_t ai_iLanguage)
{
	return _BuildFromId (ai_pId,ai_poLane,ai_iLanguage,M4_TRUE);
}

m4return_t ClChannel::BuildFromIdNoSec (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane, m4uint8_t ai_iLanguage)
{
	return _BuildFromId (ai_pId,ai_poLane,ai_iLanguage,M4_FALSE);
}

m4return_t ClChannel::_BuildFromId (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane, m4uint8_t ai_iLanguage, m4bool_t ai_bCheckSec)
{

    m4return_t			res;
	m4bool_t			bApplyMappings ;
	m4uint8_t			iCsType ;
	m4pchar_t			pcRole, pcOrg = NULL ;
	m4pchar_t			pcRSM ;
	m4double_t			dValue ;
	m4language_t		iLanguage ;
	ClBuidParameters	oParameters ;
	ClUserAction		oUserAction ;
	m4pchar_t			pcOrgId ;


    res = _GetSessionInfo(ai_bCheckSec) ;

    if (res == M4_ERROR)
        return res;

    res = CreateChannel_Data (NULL);

	if( res != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	ApplyMappings.Get( dValue );
	if( dValue == 1.0 )
	{
		bApplyMappings = M4_TRUE ;
	}
	else
	{
		bApplyMappings = M4_FALSE ;
	}

	Organization.Get( pcOrg );
	RSM_ID.Get( pcRSM ) ;
	Role_ID.Get ( pcRole ) ;
	CsType.Get( dValue ) ;
	iCsType = m4uint8_t( dValue ) ;

	// Si no se especifica el idioma cogemos el que venga en la propiedad
	if( ai_iLanguage == M4CL_LANGUAGE_NONE )
	{
		Meta_Language.Get( dValue ) ;

		if( dValue != 0 )
		{
			/* Bug 0075632
			Esto no es la corrección del bug, pero esta relacionado con el manejo del lenguaje en cachés
			Si aquí se pasa un idioma incorrecto no se encuentra en la caché cliente
			y se pide siempre el metacanal al server, que lo devuelve en el de por defecto
			*/
			iLanguage = m4language_t( dValue ) ;
			M4ClLangInfo::Instance()->CheckAndSetValidLangId( iLanguage ) ;
			ai_iLanguage = m4uint8_t( iLanguage ) ;
		}
	}

	
	oParameters.m_poLane = ai_poLane ;		// No hay carril
	oParameters.m_pccM4ObjId = ai_pId ;
	oParameters.m_pccIdOrg = pcOrg ;
	oParameters.m_pccRSM = pcRSM ;
	StartDate.Get( oParameters.m_dStartDate ) ;
	EndDate.Get( oParameters.m_dEndDate ) ;
	CorrectionDate.Get( oParameters.m_dCorDate ) ;
	oParameters.m_iCsType = iCsType ;
	oParameters.m_iLanguage = ai_iLanguage ;
	oParameters.m_bConvertRSM = M4_TRUE ;
	oParameters.m_bBuildSecurity = M4_TRUE ;
	oParameters.m_bApplyMappings = bApplyMappings ;


    res = m_pClChannel_Data->GetpExtInfo()->MCR.BuildFromId( GetpFactory(), oParameters ) ;

    if( M4_ERROR != res )
	{
		if( ai_pId != NULL )
		{
			MetaIdentifier.Set( (m4pchar_t) ai_pId ) ;
		}

        res = CommonInitAfterBuild() ;
    }
    

    if (res == M4_ERROR) {
        return res;
    }

    //ahora ya tenemos creado el kernel perfectamente:
    //asignación directa de las propiedades de seguridad:
    res = _ApplySecurity() ;


	// User actions auditory.
	if( ai_bCheckSec == M4_TRUE )
	{
		// Se bueca sólo si no se viene desde un BuildFromIdNoSec
		m4pcchar_t pccName = m_pClChannel_Data->GetpChannelDef()->Name();

		if( pccName == NULL || *pccName == '\0' )
		{
			pccName = ai_pId;
		}

		Organization.Get( pcOrgId ) ;
		m_poClChannelManager->UserActionsAudit.AddAction( pcOrgId, pcRole, ClUserAction::EV_M4OBJECT, ai_pId, pccName, NULL, M4_FALSE ) ;
	}


	m_poClChannelManager->DumpChannels("Channel::BuildFromId");

    return res;
}



m4return_t ClChannel::BuildFromAccess (ClAccess* ai_poAccess)
{
    m4return_t		res;
	m4uint8_t		iCsType ;
	m4uint8_t		iLanguage ;
	m4double_t		dValue ;


    res = _GetSessionInfo( M4_TRUE ) ;
    if (res == M4_ERROR)
        return res;

	CsType.Get( dValue ) ;
	iCsType = m4uint8_t( dValue ) ;


	// Cogemos el idioma que venga en la propiedad
	Meta_Language.Get( dValue ) ;

	if( dValue != 0 )
	{
		iLanguage = m4uint8_t( dValue ) ;
	}
	else
	{
		iLanguage = M4CL_LANGUAGE_NONE ;
	}


    res = CreateChannel_Data (NULL);

	if( res != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    res = m_pClChannel_Data->GetpExtInfo ()->MCR.BuildFromAccess (GetpFactory (), iCsType, iLanguage, ai_poAccess);
    if (M4_ERROR == res)
        return res;

	if( iCsType == M4CL_CSTYPE_FRONT )
	{
		res = BackMCR.BuildFromAccess (GetpFactory (), M4CL_CSTYPE_BACK, iLanguage, ai_poAccess);
		if (M4_ERROR == res)
			return res;
	}

    res = CommonInitAfterBuild ();

    if (res == M4_ERROR) {
        return res;
    }

    //ahora ya tenemos creado el kernel perfectamente:
    //asignación directa de las propiedades de seguridad:
    res = _ApplySecurity() ;


	m_poClChannelManager->DumpChannels( "Channel::BuildFromAccess" ) ;

    return res;
}


m4return_t ClChannel::BuildFromChannelData( ClChannel_Data *ai_poChannelData )
{

    m4return_t	iResult = M4_SUCCESS ;


    iResult = _GetSessionInfo( M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = CreateChannel_Data( ai_poChannelData ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	/* Bug 0102652
	Las propiedades si que se deben inicializar
	*/
	iResult = InitProperties() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	m_poClChannelManager->DumpChannels( "Channel::BuildFromChannelData" ) ;

    return( iResult ) ;
}


m4return_t ClChannel::BuildAsMetachannel (void)
{
    return( BuildFromFile( M4MDFAC_FILE_M4LOAD_M2 ) ) ;
}

m4bool_t ClChannel::IsDebugEnabled(void) const
{
	return (m_eDebugMode == IS_NOT_DEBUG || m_eDebugMode == IS_DEBUG_VIEW) ? M4_FALSE : M4_TRUE;
}


/**
 * Activa el modo de depuración del canal (este módo de depuración se mantiene mientras dura la ejecución del método remoto).
 * Al activar el modo de depuración se crea el espacio proxy de depuración, si es que este no existía.
 * La desactivación del modo de depuración se hace automáticamente cuando se termina la ejecución del método remoto.
 *
 * @param m4return_t.
 */
m4return_t ClChannel::SetDebugEnabled()
{
	if (m_eDebugMode == IS_DEBUG_VIEW) {
		return M4_ERROR;
	}

	if (m_eDebugMode == IS_NOT_DEBUG) {
		// Si se trata de un canal OLTP o AUTO preparo el espacio proxy para depuración.
		// Pedir el espacio proxy para depuración de OLTP sólo tiene sentido si estamos en el lado cliente.

		m4bool_t bExecutingInClient = m_poClChannelManager->GetDMContext()->ExecutingInClient();

		if (bExecutingInClient == M4_TRUE)
		{
			m4uint8_t csExeType = GetpChannelDef()->CsExeType();

			if (csExeType == M4CL_CSEXETYPE_OLTP || csExeType == M4CL_CSEXETYPE_AUTO)
			{
				ClProxySpace* pProxySpace = m_poClChannelManager->GetProxyManager()->GetSpace("OLTPD", M4_TRUE);
				if (!pProxySpace) return M4_ERROR;
			}
		}
		m_eDebugMode = IS_DEBUG_FIRST_TIME;
	}
	return M4_SUCCESS;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4return_t ClChannel::ReleaseAll (m4bool_t ai_bresetnodeitems)
{
    m4return_t res = M4_SUCCESS;

    if (m_pClChannel_Data)
        res = m_pClChannel_Data->ReleaseAll (ai_bresetnodeitems);


	// Para arreglar el bug 0066910
	// Hay que resetear todos las referencias a los bloques para que se
	// inicialicen correctamente los BB
    ClAccessIndex	i, iAccessLength ;
    ClNodeIndex		j, iNodeLength ;
	ClAccess		*pAccess ;
    ClNode			*pNode ;


    iAccessLength = Access.Count() ;

    for( i = 0; i < iAccessLength; i++ )
    {
		pAccess = Access[ i ] ;

		if( pAccess != NULL )
		{
			iNodeLength = pAccess->Node.Count() ;

			for( j = 0; j < iNodeLength; j++ )
			{
				pNode = pAccess->Node.Get( j ) ;

				if( pNode != NULL )
				{
					pNode->Reset() ;
				}
			}
		}
    }


    return res;
}

//----------------------------------------------------------------------------
//Estadísticas
//----------------------------------------------------------------------------

m4return_t	ClChannel::SetStatisticsLevel (m4uint16_t ai_iInit)
{
	if( m_iSttInit == ai_iInit )
	{
		return M4_SUCCESS;
	}

	if( m_poClChannelManager != NULL )
	{
		ClVMStatisticsWrapper *poWrapper = m_poClChannelManager->GetSttWrapper();

		if( poWrapper != NULL )
		{
			if( ai_iInit == M4_VM_STATISTICS_DISABLED )
			{
				poWrapper->DecStatisticsRef();
			}
			else if( m_iSttInit == M4_VM_STATISTICS_DISABLED )
			{
				poWrapper->IncStatisticsRef();
			}

			if( m_poClChannelManager->GetDMContext()->ExecutingInClient() == M4_TRUE )
			{
				if( ai_iInit != M4_VM_STATISTICS_DISABLED )
				{
					//parche para la prueba
					ClVMStatistics *poStatistics = poWrapper->GetStatistics();

					if( poStatistics == NULL )
					{
						poWrapper->CreateStatistics( m_poClChannelManager );
						poStatistics = poWrapper->GetStatistics();
					}

					if( poStatistics != NULL )
					{
						ClChannel *poChannel = poStatistics->sttGetChannel();

						if( poChannel != NULL )
						{
							Level2.InsertL2Channel( M4_INSTANCE_STATISTICS, poChannel->GetHandle() );
						}
					}
				}
				else
				{
					Level2.EraseL2Instance( M4_INSTANCE_STATISTICS, M4_TRUE );
				}
			}
		}
	}

	m_iSttInit = ai_iInit;

	return M4_SUCCESS;
}

m4uint16_t ClChannel::GetStatisticsLevel (void)
{
	return m_iSttInit;
}


m4pchar_t  ClChannel::GetStatisticsLabel(void)
{
    if (m_vLabel.Type != M4CL_CPP_TYPE_STRING_VAR)
		return NULL;

	return m_vLabel.Data.PointerChar;
}

m4return_t ClChannel::SetStatisticsLabel(m4pchar_t ai_pcLabel)
{
	if (ai_pcLabel)
		m_vLabel.SetString(ai_pcLabel);
	else
		m_vLabel.SetNull();

	return M4_SUCCESS;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

m4return_t ClChannel::CacheLoad (m4bool_t &ao_bLoaded)
{

	m4return_t	res = M4_SUCCESS;
	m4pchar_t	pcRSM  = NULL;


	ao_bLoaded = M4_FALSE;

    ClCompiledMCR* pcmcr = GetpChannelDef()->GetpCMCR();

	if ( (!pcmcr) || (!m_pClChannel_Data) )
	{
        // el canal no está construido!
        DUMP_CHLOG_ERROR(M4_CH_DM_CHNOTBUILT);
		return M4_ERROR;
	}

	m4bool_t bIsCacheable = (pcmcr->GetChannelIsCacheable() ? M4_TRUE : M4_FALSE);
	
	ClDataCache *poDataCache = 0;
	
	if (m_poClChannelManager)	
		poDataCache = m_poClChannelManager->m_poDataCache;

	if (bIsCacheable == M4_TRUE)
	{
		enum ClMCROwner::BuildType_e	iType = m_pClChannel_Data->GetpExtInfo()->MCR.BuildType() ;

		if (iType != ClMCROwner::ID && iType != ClMCROwner::Data)
		{
			g_oChLog<<BeginWarning(M4_CH_DM_CACHE_GET_TEMPORAL_CHANNEL)<<M4Obj(GetpCMCR())<<EndLog;
			return M4_SUCCESS;
		}

		/* Bug 0118646
		Ya no se chequea que el canal sea proxy
		*/
		if (poDataCache)
		{
			ClChannel_Data * pClChannel_Data = 0;
			m4uint8_t		iCsType ;
			m4uint8_t		iLanguage;
			m4VariantType	vDateStart, vDateEnd, vDateCor, vAux;
			m4pcchar_t		pcChannelId = pcmcr->GetChannelId();
			m4uint8_t		iOrgType;
			m4pchar_t		pcOrg;

			StartDate.Get (vDateStart);
			EndDate.Get (vDateEnd);
			CorrectionDate.Get (vDateCor);

			Data_Language.Get( vAux ) ;
			iLanguage = m4uint8_t( vAux.Data.DoubleData ) ;

			CsType.Get( vAux ) ;
			iCsType = m4uint8_t( vAux.Data.DoubleData ) ;

			RSM_ID.Get( pcRSM ) ;

			/* Bug 0142992
			La pk de la cache debe ser RSM y no mask
			*/
			if( pcRSM == NULL )
			{
				pcRSM = "";
			}

			OrganizationType.Get( vAux ) ;
			iOrgType = m4uint8_t( vAux.Data.DoubleData ) ;

			if (iOrgType != M4CL_ORGANIZATION_TYPE_MONO)
			{
				pcOrg = "";
			}
			else
			{
				Organization.Get(pcOrg);

				if( pcOrg == NULL )
				{
					pcOrg = "";
				}
			}

			//Obtener parametros
			m4VariantType * pVarParams = 0;
			m4uint32_t iNumParams = 0;

			BuildChannelParamId (GetpChannelDef()->GetpCMCR(), pVarParams, iNumParams);    

			ClCacheableObject* poCO = m_pClChannel_Data->GetCO();

			if( poCO != NULL )
			{
				ClCacheKey* poClCacheKey;
				poCO->GetKey(poClCacheKey);

				/* Bug 0188618
				Se comprueba si han cambiado los parámetros
				*/
				if( poClCacheKey != NULL )
				{
					m4pchar_t pcId = poClCacheKey->Getm_pszKey();
					m4pchar_t pcDataId = poDataCache->ComposeStringKey(iCsType, pcChannelId, pcRSM, iLanguage, iOrgType, pcOrg, pVarParams, iNumParams);

					if( strncmp( pcId, pcDataId, strlen( pcDataId ) ) == 0 )
					{
						//Si está cacheado el ChannelData con la misma pk, devolvemos TRUE. NO hace falta buscar.
						ao_bLoaded = M4_TRUE;
						delete [] pcDataId;
						
						if( pVarParams != NULL )
						{
							delete [] pVarParams;
							pVarParams = 0;
						}
						return M4_SUCCESS;
					}
					delete [] pcDataId;
				}
			}

			//Se busca el objeto en la cache. Si esta res == M4_SUCCESS
			m4bool_t bFound = M4_FALSE;

			//Antes del get object le paso el channelmanager al oCOFactory para que la cache de datos pueda
			//desrializar canales (de disco).
			//para evitar problemas de multith bloqueo con un mutex.
			((ClDataCacheableObjectFactory*)poDataCache->GetCOFactory())->m_oMutex.Lock();
			((ClDataCacheableObjectFactory*)poDataCache->GetCOFactory())->SetChannelManager(m_poClChannelManager);

			res = poDataCache->GetObject(iCsType, pcChannelId, pcRSM, iLanguage, iOrgType, pcOrg, pVarParams, iNumParams, vDateStart, vDateEnd, vDateCor, pClChannel_Data, bFound);

			((ClDataCacheableObjectFactory*)poDataCache->GetCOFactory())->SetChannelManager(0);
			((ClDataCacheableObjectFactory*)poDataCache->GetCOFactory())->m_oMutex.Unlock();

			if( pVarParams != NULL )
			{
				delete [] pVarParams;
				pVarParams = 0;
			}
			
			if (bFound == M4_TRUE)
			{
				//Comprobamos las fechas
				/* Bug 0271932
				Hay que comparar teniendo en cuenta la precisión
				*/
				if (fabs(pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR()->GetChannelVersion() - m_pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR()->GetChannelVersion()) < M4_LN4_DATE_PRECISION)
				{
                    //si son difirentes ...
                    if (m_pClChannel_Data != pClChannel_Data) {
                        
                        //... pegamos el cambiazo:

                        //avisamos a la gente de que se va a cambiar el channel_data:
                        m_pClChannel_Data->GetpNotifier ()->ChannelDataChanged_Before (pClChannel_Data, pClChannel_Data->GetpNotifier (), this);

                        //aquí ya estoy borrado del notifier del channel_data actual
                        //y estoy apuntado al del nuevo

                        //destruimos el channel_data actual:
						m_pClChannel_Data->Destroy(M4_FALSE);

                        //y nos quedamos con la referencia al nuevo
                        m_pClChannel_Data = pClChannel_Data;
                    }
					else
					{
						//Quitamos referencia
						pClChannel_Data->Destroy(M4_FALSE);
					}

					ao_bLoaded = M4_TRUE;
				}
				else
				{
					// El objeto cacheado es antiguo
					//A la cola de borrado y Quitamos referencia (TRUE)
					pClChannel_Data->Destroy(M4_TRUE);

					ao_bLoaded = M4_FALSE;
				}

			}
			else
			{
				ao_bLoaded = M4_FALSE;
			}
		}
	}

    return res;
}




m4return_t ClChannel::CachePut (void)
{
// Por ahora solo se puede hacer CachePut si antes se llamo a CacheLoad, ya que internamente
//  mos quedamos con la key de la operación. 
//   Esto se arreglaría si en el PutObject pasáramos todos los datos necesarios para construir la clave

    m4return_t res = M4_SUCCESS;
	ClCompiledMCR* pcmcr = GetpChannelDef()->GetpCMCR();

	if ( (!pcmcr) || (!m_pClChannel_Data) )
	{
        // el canal no está construido!
        DUMP_CHLOG_ERROR(M4_CH_DM_CHNOTBUILT);
		return M4_ERROR;
	}

	m4bool_t bIsCacheable = (pcmcr->GetChannelIsCacheable() ? M4_TRUE : M4_FALSE);

	ClDataCache *poDataCache = 0;
	
	if (m_poClChannelManager)	
		poDataCache = m_poClChannelManager->m_poDataCache;

	if (bIsCacheable == M4_TRUE)
	{
		enum ClMCROwner::BuildType_e	iType = m_pClChannel_Data->GetpExtInfo()->MCR.BuildType() ;

		if (iType != ClMCROwner::ID && iType != ClMCROwner::Data)
		{
			g_oChLog<<BeginWarning(M4_CH_DM_CACHE_PUT_TEMPORAL_CHANNEL)<<M4Obj(GetpCMCR())<<EndLog;
			return M4_SUCCESS;
		}

		/* Bug 0118646
		Ya no se chequea que el canal sea proxy
		*/

		if (poDataCache)
		{
			ClChannel_Data * pClChannel_Data = 0;
			m4uint8_t		iCsType ;
			m4uint8_t		iLanguage;
			m4VariantType	vAux;
			m4pcchar_t		pcChannelId = pcmcr->GetChannelId();
			m4pchar_t		pcRSM = NULL;
			m4uint8_t		iOrgType;
			m4pchar_t		pcOrg;

			Data_Language.Get( vAux ) ;
			iLanguage = m4uint8_t( vAux.Data.DoubleData ) ;

			CsType.Get( vAux ) ;
			iCsType = m4uint8_t( vAux.Data.DoubleData ) ;

			RSM_ID.Get( pcRSM ) ;

			/* Bug 0142992
			La pk de la cache debe ser RSM y no mask
			*/
			if( pcRSM == NULL )
			{
				pcRSM = "";
			}

			OrganizationType.Get( vAux ) ;
			iOrgType = m4uint8_t( vAux.Data.DoubleData ) ;

			if (iOrgType != M4CL_ORGANIZATION_TYPE_MONO)
			{
				pcOrg = "";
			}
			else
			{
				Organization.Get(pcOrg);

				if( pcOrg == NULL )
				{
					pcOrg = "";
				}
			}

			//Obtener parametros
			m4VariantType * pVarParams = 0;
			m4uint32_t iNumParams = 0;

			BuildChannelParamId (GetpChannelDef()->GetpCMCR(), pVarParams, iNumParams);    

			m4date_t ai_dMaxPeriod = pcmcr->GetChannelCacheMaxPeriod();
			if (!ai_dMaxPeriod)
			{
				//Ponemos el periodo por defecto.
				ai_dMaxPeriod = GetEnvironment ()->GetDataCacheDefaultMaxPeriod();

			}

			ai_dMaxPeriod /= GetEnvironment ()->GetDataCacheExpiryRatio();

			res = poDataCache->PutObject (iCsType, pcChannelId, pcRSM, iLanguage, iOrgType, pcOrg, pVarParams, iNumParams, m_pClChannel_Data, ai_dMaxPeriod);

			if( pVarParams != NULL )
			{
				delete [] pVarParams;
				pVarParams = 0;
			}
		}
	}

    return res;
}



m4return_t ClChannel::Load (ClAccess *ai_paccess/*= 0*/, m4bool_t ai_bFromWrapper/*=M4_FALSE*/)
{
    m4return_t res = M4_ERROR;

	if (0 == ai_paccess) {
		//me creo uno
		CreateAccess (ai_paccess);
		M4_ASSERT (ai_paccess);
		res = ai_paccess->Load(ai_bFromWrapper);
		ai_paccess->Destroy();
	}
	else
		res = ai_paccess->Load(ai_bFromWrapper);

    return res;
}


m4return_t ClChannel::UnLoad (m4bool_t ai_bRemoveCache)
{	
    m4return_t res = M4_SUCCESS;

	if (m_pClChannel_Data)
	{
	    ClCompiledMCR* pcmcr = GetpChannelDef()->GetpCMCR();

		m4bool_t bIsCacheable = (pcmcr->GetChannelIsCacheable() ? M4_TRUE : M4_FALSE);

		/* Bug 0085119
		Si no hay caché tampoco hay que hacer nada más que borrar los datos.
		Se cuenta con que si la caché de datos está activada también lo está
		la de metadatos. Eso se garantiza en la m4objreg
		*/
		ClDataCache *poDataCache = NULL ;
		
		if( m_poClChannelManager != NULL )
		{
			poDataCache = m_poClChannelManager->m_poDataCache ;
		}

	
		if (bIsCacheable == M4_TRUE && poDataCache != NULL)
		{
			//edu: me quedo primero con la referencia al mcr
			//por si acaso al decirle a la caché que ya no quiero el Channel_Data, va y
			//me lo destruye:

			//tenemos una limitación en la inicialización del nuevo channel_data
			//sólo si el mcr se creo con buildfromid o de cache de autos
			enum ClMCROwner::BuildType_e	iType = m_pClChannel_Data->GetpExtInfo()->MCR.BuildType() ;

			M4_ASSERT (iType == ClMCROwner::ID || iType == ClMCROwner::Data);

			//ok, cojo el mcr actual:
			ClCompiledMCR *pcmcr = GetpChannelDef()->GetpCMCR();
			M4_ASSERT (pcmcr);

			//me creo el nuevo channel_data:
			ClChannel_Data *pcd = NewChannel_Data ();


			//inicializo con un nuevo buildfromid el canal:
			//(está es la limitación)
        
			ClBuidParameters	oParameters ;


// jcr lo sacamos todo del canal
			oParameters.m_poLane = NULL ;			// No hay carril
			oParameters.m_pccM4ObjId = pcmcr->GetChannelId() ;
			oParameters.m_pccIdOrg = NULL ;			// De momento nulo !!!!!!!!
			oParameters.m_pccRSM = pcmcr->GetSecurityMaskId() ;
			oParameters.m_dStartDate = pcmcr->GetChannelStartDate();
			oParameters.m_dEndDate = pcmcr->GetChannelEndDate() ;
			oParameters.m_dCorDate = pcmcr->GetChannelCorStartDate() ;
			oParameters.m_iCsType = pcmcr->GetCacheCsType() ;
			oParameters.m_iLanguage = pcmcr->GetChannelLanguage() ;
			oParameters.m_bConvertRSM = M4_FALSE ;
			oParameters.m_bBuildSecurity = M4_TRUE ;
			oParameters.m_bApplyMappings = M4_FALSE ;	// De momento no

	
			res = pcd->GetpExtInfo()->MCR.BuildFromId( GetpFactory(), oParameters ) ;

			if (M4_ERROR == res)    //error muy feo
				return res;

			//seguimos inicializando:
			res = pcd->Init ();
			if (M4_ERROR == res)    //error muy feo
				return res;

			//avisamos a la gente de que se va a cambiar el channel_data:
			m_pClChannel_Data->GetpNotifier ()->ChannelDataChanged_Before (pcd, pcd->GetpNotifier (), this);

			//aquí ya estoy borrado del notifier del channel_data actual
			//y estoy apuntado al del nuevo


			//A la cola de borrado y Quitamos referencia (si ai_bRemoveCache == M4_TRUE)
            m_pClChannel_Data->Destroy(ai_bRemoveCache);

			m_pClChannel_Data = pcd;
		}
		else
		{
			ReleaseAll();
		}
	}

	return res;
}

m4return_t ClChannel::Persist (ClAccess *ai_paccess/*= 0*/, m4bool_t ai_bFromWrapper/*=M4_FALSE*/)
{
    m4return_t res;

    if (0 == ai_paccess) {
        //me creo uno
        CreateAccess (ai_paccess);
        M4_ASSERT (ai_paccess);
        res = ai_paccess->Persist(ai_bFromWrapper);
        ai_paccess->Destroy();
    }
    else
        res = ai_paccess->Persist(ai_bFromWrapper);

    return res;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4return_t ClChannel::LoadFromIOD(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;
	
	ClChannelDataSerializer oChannelDataSerializer;
					
	ClChannel_Data * pClChannel_Data = 0;

	pClChannel_Data = new ClChannel_Data();

	ret = oChannelDataSerializer.DeSerialize(IOD, GetpFactory(), NULL, NULL, pClChannel_Data);

	if (ret != M4_ERROR) 
	{                      
		//... pegamos el cambiazo:
		if (m_pClChannel_Data)
		{
			//Comprobar Id, Version, CSType, lenguaje,etc. Si no el canal de disco es impatible con el que intenta cargarlo

			/* Bug 0089067
			Sólo se comprueba la versión si no se ha serializado el metacanal
			*/
			if( oChannelDataSerializer.GetSerializeMCR() == M4_FALSE && M4_FALSE == pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR()->IsEqual( m_pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR()) )
			{
				//Error, los metacanales no son iguales
				g_oChLog<<BeginError(M4_CH_DM_DIFFERENT_CHANNEL_LOADFROMFILE)<<M4ObjExtended(pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR())<<M4ObjExtended(m_pClChannel_Data->GetpExtInfo()->MCR.GetpCMCR())<<EndLog;
				pClChannel_Data->Destroy(M4_FALSE);
				return M4_ERROR;
			}

			//avisamos a la gente de que se va a cambiar el channel_data:
			m_pClChannel_Data->GetpNotifier ()->ChannelDataChanged_Before (pClChannel_Data, pClChannel_Data->GetpNotifier (), this);

			//aquí ya estoy borrado del notifier del channel_data actual
			//y estoy apuntado al del nuevo

			//destruimos el channel_data actual:
			m_pClChannel_Data->Destroy(M4_FALSE);

			//y nos quedamos con la referencia al nuevo
			m_pClChannel_Data = pClChannel_Data;
		}
		else
		{
			//Si no estaba construido el metacanal, se lee todo de disco. Ver CommonInitAfterBuild.
			//Vamos a dejar el canal exactamente igual que si hubiera pasado por el BuildFromId

			//Hacemos el attach
			pClChannel_Data->GetpNotifier ()->Attach (this);

			//y nos quedamos con la referencia al nuevo
			m_pClChannel_Data = pClChannel_Data;

			// TODO: Alex. Por que se hace esto.
			// Inicilizamos la parte de los proxys (si es proxy).
			// Sólo en la parte cliente (se comprueba dentro).
			GetProxyContext();

			//Propiedades de Organization.
			m4uint8_t		iType ;
			m4uint8_t		iNoWrite ;
			m4language_t	lLanguage ;
			m4language_t	lMetaLanguage ;
			m4double_t		dType ;
			m4pcchar_t		pccThisT3Id ;
			m4VariantType	vValue ;
			ClLogonAdaptor	*poLogonAdaptor ;
			ClCompiledMCR   *poCMCR ;


		    poCMCR = GetpChannelDef()->GetpCMCR() ;

			OrganizationType.Get( vValue ) ;

			if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
			{
				dType = poCMCR->GetChannelOrganizationType() ;
				OrganizationType.Set( dType ) ;

				if( vValue.Type != M4CL_CPP_TYPE_NULL )
				{
					DUMP_CHLOG_WARNINGF( M4_CH_DM_BAD_ORG_TYPE_TYPE, m4uint8_t( vValue.Type ) << LogCat << " - " << LogCat << M4ClCppType( vValue.Type ) <<  M4ClOrganizationType( m4uint8_t( dType ) ) ) ;
				}
			}
			else if( vValue.Data.DoubleData < M4CL_ORGANIZATION_TYPE_NONE || vValue.Data.DoubleData > M4CL_ORGANIZATION_TYPE_MULTI )
			{
				dType = poCMCR->GetChannelOrganizationType() ;
				OrganizationType.Set( dType ) ;

				DUMP_CHLOG_WARNINGF( M4_CH_DM_BAD_ORG_TYPE, m4uint8_t( vValue.Data.DoubleData ) << m4uint8_t( M4CL_ORGANIZATION_TYPE_NONE ) << LogCat << " - " << LogCat << M4ClOrganizationType( M4CL_ORGANIZATION_TYPE_NONE ) << m4uint8_t( M4CL_ORGANIZATION_TYPE_MULTI ) << LogCat <<  " - " << LogCat << M4ClOrganizationType( M4CL_ORGANIZATION_TYPE_MULTI ) << M4ClOrganizationType( m4uint8_t( dType ) ) ) ;
			}

			// Ponemos los idiomas
			// El de metadatos es con el que acaba el metacanal

			lMetaLanguage = poCMCR->GetChannelLanguage() ;
			Meta_Language.Set( m4double_t( lMetaLanguage ) ) ;

			// El de datos es el que venía y sino venía es el del canal sesión que diga el metacanal
			// Si no hay canal sesión le pongo el de metadatos
			Data_Language.Get( vValue ) ;

			if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
			{
				if( m_poClChannelManager != NULL )
				{
					poLogonAdaptor = m_poClChannelManager->GetLogonAdaptor() ;

					if( poLogonAdaptor != NULL )
					{
						pccThisT3Id = poCMCR->GetChannelId() ;
						iType = poCMCR->GetChannelDataLanguage() ;

						if( iType == M4CL_LANGUAGE_TYPE_LOGON )
						{
							lLanguage = poLogonAdaptor->GetSessionLanguage( pccThisT3Id, M4CL_LANGUAGE_NONE ) ;
						}
						else
						{
							lLanguage = poLogonAdaptor->GetSessionDevLanguage( pccThisT3Id, M4CL_LANGUAGE_NONE ) ;
						}
					}
				}
				else
				{
					lLanguage = lMetaLanguage ;
				}

				Data_Language.Set( m4double_t( lLanguage ) ) ;
			}

			// Seteamos las propiedades a valores de sólo lectura
			iNoWrite = M4_PROP_WRITE | M4_PROP_WRITE_COL ;

			ApplyMappings.UnSetFlags( iNoWrite ) ;
			MetaIdentifier.UnSetFlags( iNoWrite ) ;
			StartDate.UnSetFlags( iNoWrite ) ;
			EndDate.UnSetFlags( iNoWrite ) ;
			CorrectionDate.UnSetFlags( iNoWrite ) ;
			Meta_Language.UnSetFlags( iNoWrite ) ;
			CsType.UnSetFlags( iNoWrite ) ;
			RSM_ID.UnSetFlags( iNoWrite ) ;
			Role_ID.UnSetFlags( iNoWrite ) ;
			OrganizationType.UnSetFlags( iNoWrite ) ;
			ExecuteRealSQLMode.UnSetFlags( iNoWrite ) ;
		}
	}
	else
	{
		//Error. No pudo deserializar.		
		pClChannel_Data->Destroy(M4_FALSE);
	}

	/* Bug 0118646
	El channel data se ha quedado descuadrado
	Sin embargo, no se puede marcar porque sino el proxy dará error la transacción
	m_bIsSynchronized = M4_FALSE ;
	*/

	return ret;
}



m4return_t ClChannel::LoadFromSttIOD( ClGenericIODriver& ai_IODriver, m4pcchar_t ai_pcIntermediateFile )
{

	m4return_t		iResult = M4_SUCCESS;
	m4uint16_t		iVersion = 0 ;
	m4uint16_t		iActualVersion = 0 ;
	ClVMStatistics	oStatistics( M4_FALSE ) ; ;


	iResult = oStatistics.sttInit( this ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ai_IODriver.Read( iVersion ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_READ_INTERMEDIATE, ai_pcIntermediateFile ) ;


	// Se lee hasta el fin del fichero
	while( iVersion != m4uint16_t( -1 ) )
	{
		iActualVersion = ClVMStatistics::GetSerializationVersion() ;
		CHECK_CHLOG_ERRORF( iVersion != iActualVersion, M4_ERROR, M4_CH_DM_STT_BAD_INTERMEDIATE_VERSION, ai_pcIntermediateFile << iVersion << iActualVersion ) ;

		iResult = oStatistics.DeSerialize( ai_IODriver ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_READ_INTERMEDIATE, ai_pcIntermediateFile ) ;

		iResult = oStatistics.sttAddActiveStateAll() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		iResult = ai_IODriver.Read( iVersion ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_READ_INTERMEDIATE, ai_pcIntermediateFile ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClChannel::PersistToIOD(ClGenericIODriver& IOD, m4bool_t ai_bSerializeMCR)
{
	m4return_t ret = M4_SUCCESS;

	if (!m_pClChannel_Data)
	{
		//Error no hay nada que persistir
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_PERSIST_CHNOTBUILT)<<"IOD"<<EndLog;
		ret = M4_ERROR;
	}
	else
	{
		ClChannelDataSerializer oChannelDataSerializer;

		ret = oChannelDataSerializer.Serialize(IOD, m_pClChannel_Data, ai_bSerializeMCR, M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_ALL);
	}
	
	return ret;
}

m4return_t ClChannel::LoadFromFile (m4pchar_t ai_pcFileName)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pcFileName)
	{
		//Error no hay nombre de fichero
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_LOADFILE)<<"NULL"<<EndLog;
		return M4_ERROR;
	}

	ClFileIODriver IOD;

	ret = IOD.Init(GEN_IO_MODE_READ, ai_pcFileName);

	if (ret != M4_ERROR)
	{
		ret = LoadFromIOD(IOD);

		IOD.End(M4_TRUE);

		if (M4_ERROR == ret)
		{
			//error no se pudo recuperar de disco
			if (m_pClChannel_Data)
			{
				g_oChLog<<BeginError(M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE)<<ai_pcFileName<<M4Obj(GetpCMCR())<<EndLog;
			}
			else
			{
				g_oChLog<<BeginError(M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE)<<ai_pcFileName<<EndLog;
			}
		}
	}
	else
	{
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_LOADFILE)<<ai_pcFileName<<EndLog;
		ret = M4_ERROR;
	}
				
	return ret;
}


m4return_t ClChannel::LoadFromSttFile (m4pchar_t ai_pcFileName)
{
	m4return_t		ret = M4_SUCCESS ;
	size_t			iLength = 0 ;
	m4pchar_t		pccDestination = NULL ;
	ClFileIODriver	IOD ;


	if (!ai_pcFileName)
	{
		//Error no hay nombre de fichero
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_LOADFILE)<<"NULL"<<EndLog;
		return M4_ERROR;
	}

	// Si no es el canal de estadísticas hay que dar error
	CHECK_CHLOG_ERRORF( strcmpi( GetpChannelDef()->Id(), M4_CHANNEL_STATISTICS ) != 0, M4_ERROR, M4_CH_DM_STT_BAD_STT_M4OBJ, M4Obj( GetpCMCR() ) ) ;

	iLength = strlen( ai_pcFileName ) + 1 ;
	CHECK_CHLOG_ERRORF( iLength < 5, M4_ERROR, M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE, ai_pcFileName << M4Obj( GetpCMCR() ) ) ;

	pccDestination = new m4char_t[ iLength + 4 ] ;
	CHECK_CHLOG_ERRORF( pccDestination == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( ( iLength + 4 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;

	memcpy( pccDestination, ai_pcFileName, iLength * sizeof( m4char_t ) ) ;
	strcat( pccDestination, ".tmp" ) ;

	ret = M4DecompressFile( ai_pcFileName, pccDestination ) ;
	CHECK_CHLOG_ERRORF( ret != M4_SUCCESS, M4_ERROR, M4_CH_DM_STT_NO_DECOMPRESS_INTERMEDIATE, ai_pcFileName << pccDestination ) ;


	ret = IOD.Init(GEN_IO_MODE_READ, pccDestination);

	if (ret != M4_ERROR)
	{
		ret = LoadFromSttIOD(IOD, ai_pcFileName);

		IOD.End(M4_TRUE);

		if (M4_ERROR == ret)
		{
			//error no se pudo recuperar de disco
			if (m_pClChannel_Data)
			{
				g_oChLog<<BeginError(M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE)<<ai_pcFileName<<M4Obj(GetpCMCR())<<EndLog;
			}
			else
			{
				g_oChLog<<BeginError(M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE)<<ai_pcFileName<<EndLog;
			}
		}
	}
	else
	{
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_LOADFILE)<<ai_pcFileName<<EndLog;
		ret = M4_ERROR;
	}
				
	remove( pccDestination ) ;
	delete( pccDestination ) ;

	return ret;
}


m4return_t ClChannel::PersistToFile (m4pchar_t ai_pcFileName, m4bool_t ai_bSerializeMCR)
{
	m4return_t ret = M4_SUCCESS;

	if (!m_pClChannel_Data)
	{
		//Error no hay nada que persistir
		g_oChLog<<BeginError(M4_CH_DM_CANNOT_PERSIST_CHNOTBUILT)<<(ai_pcFileName?ai_pcFileName:"NULL")<<EndLog;
		ret = M4_ERROR;
	}
	else
	{
		if (!ai_pcFileName || *ai_pcFileName == '\0')
		{
			//Error no hay nombre de fichero
			g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_PERSISTFILE)<<"NULL"<<M4Obj(GetpCMCR())<<EndLog;
			return M4_ERROR;
		}

		ClFileIODriver IOD;

		ret = IOD.Init(GEN_IO_MODE_WRITE, ai_pcFileName);

		if (ret != M4_ERROR)
		{
			ret = PersistToIOD(IOD, ai_bSerializeMCR);

			IOD.End(M4_TRUE);

			if (ret == M4_ERROR)
			{
				//Borramos el fichero
				remove(ai_pcFileName);
				g_oChLog<<BeginError(M4_CH_DM_CANNOT_PERSIST_CHANNEL_TO_FILE)<<ai_pcFileName<<M4Obj(GetpCMCR())<<EndLog;
			}
		}
		else
		{
			g_oChLog<<BeginError(M4_CH_DM_CANNOT_OPEN_PERSISTFILE)<<ai_pcFileName<<M4Obj(GetpCMCR())<<EndLog;
			ret = M4_ERROR;
		}
	}
	
	return ret;
}

m4return_t ClChannel::PersistToTempFile (m4pchar_t ao_pcFileName, m4uint32_t ai_iSize)
{
	//Si no se pasa el nombre del fichero, creamos un temporal y devolvemos el nombre.
	m4return_t ret = M4_SUCCESS;
	ClFileName oFileName;
	ClFileManager* pFileManager = ClFileManager::Instance(); // Singleton

	if (pFileManager->CreateTempFile(oFileName, "dm", "chn", NULL) == M4_SUCCESS)
	{
		if 	(M4_FALSE == oFileName.IsNull())
		{
			if (ai_iSize < oFileName.GetLength() + 1)
			{
				ret = M4_ERROR;
			}
			else
			{
				oFileName.GetFileName(ao_pcFileName);

				ret = PersistToFile(ao_pcFileName, M4_FALSE);
			}
		}
	}

	return ret;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ClAccess *ClChannel::CreateAccess (void)    // obsoleta
{
    ClAccess *paccess = 0;
    CreateAccess (paccess);

    return paccess;
}

m4return_t ClChannel::CreateAccess (ClAccess * &ao_paccess)
{
 	// Construye el identificador único a partir de un contador
	static	m4uint32_t	s_iHandle = 0;
	static	ClMutex		s_oMutex(M4_TRUE);

	m4return_t	res = M4_ERROR;
	m4uint32_t	iHandle = 0;

    if (IsBuilt ()) {

		// Construye el identificador único a partir de un contador
		s_oMutex.Lock();
		iHandle = ++s_iHandle;
		s_oMutex.Unlock();

        ao_paccess = new (GetpChannelDef ()->NumberOfNodes ()) ClAccess (iHandle);

        if (ao_paccess) {            
            res = ao_paccess->Init (this);
        } else {
            DUMP_CHLOG_ERRORF( M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClAccess ) ) << __LINE__ << __FILE__ ) ;
        }
    } else {
        // el canal no está construido!
        DUMP_CHLOG_ERROR(M4_CH_DM_CHNOTBUILT);
    }

    return res;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ClChannelDef *ClChannel::GetpChannelDef (void) const
{
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, NULL, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    return (ClChannelDef *)m_pClChannel_Data->GetpChannelDef ();
}

m4uint32_t ClChannel::GetHandle( void ) const
{
	return ( m_iHandle );
}


m4bool_t ClChannel::IsBuilt (void) const
{
    m4bool_t bIsBuilt = ((m_pClChannel_Data != 0)&&
        (m_pClChannel_Data->GetpChannelDef ()->GetpCMCR () != 0)) ? M4_TRUE : M4_FALSE;
    return bIsBuilt;

}

m4return_t ClChannel::SetAutoLoadMode (m4uint8_t ai_mode)
{
    m4return_t res;
    
    //llama a los access que son los que hacen el curro:
    res = Access.SetAutoLoadMode (ai_mode);
    
    if (M4_ERROR != res)
        //se queda como estuviera:
        ClAutoLoad_PlugIn::SetAutoLoadMode (ai_mode);

    return res;
}



//----------------------------------------------------------------------------
//Seguridad
//----------------------------------------------------------------------------

m4return_t ClChannel :: _GetSessionInfo(m4bool_t ai_bCheckSec)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4uint8_t		iValid ;
	m4uint16_t		iLength ;
	m4pchar_t		pcRole = NULL ;
	m4pchar_t		pcSessionRole = NULL ;
	m4pchar_t		pcOrg = NULL ;
	m4char_t		acString[ M4CL_MAX_RSM_ID + 1 ] ;
	m4pchar_t		pcRSM = acString ;
	ClLogonAdaptor	*poLogonAdaptor = NULL ;
	ClChannel		*poSessionChannel ;


// Se rellena el Role, el RSM y la sociedad
// Siempre se toma el miembro del role y sociedad que haya. Si no hay se cogen del canal sesión
// Para el propio canal sesión el role se tiene que haber puesto a NULL antes de llamarnos
// El RSM se rellena a partir de la pareja
// Se hace un control de tamaños


// Si viene serializado se devuleve éxito
// El RSM se rellena en la deserialización

	if( m_iIsSerialized == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	if( m_poClChannelManager != NULL )
	{
		poLogonAdaptor = m_poClChannelManager->GetLogonAdaptor() ;
		poSessionChannel = m_poClChannelManager->GetSessionChannel() ;
	}
	else
	{
		poLogonAdaptor = NULL ;
		poSessionChannel = NULL ;
	}


// Conseguimos la sociedad

	Organization.Get( pcOrg ) ;

	/* Bug 0165968
	Si no hay que chequear seguridad tampoco se mete sociedad para que no se lance el CheckProjectTreeValidation
	*/
	if( pcOrg == NULL && ai_bCheckSec == M4_TRUE )
	{
		// Si viene a NULL vamos al canal sesión
		if( poLogonAdaptor != NULL && poSessionChannel != NULL )
		{
			pcOrg = poLogonAdaptor->GetOrganization() ;
		}

		// La sociedad nula es válida
		if( pcOrg != NULL )
		{
			Organization.Set( pcOrg ) ;
		}
		else
		{
			Organization.SetNull() ;
		}
	}


// Conseguimos la seguridad a partir del role

	Role_ID.Get( pcRole ) ;

	if( pcRole == NULL )
	{
		// Si viene a NULL vamos al canal sesión
		if( poLogonAdaptor != NULL && poSessionChannel != NULL )
		{
			pcSessionRole = poLogonAdaptor->GetRole() ;
			pcRole = pcSessionRole ;
		}

		// El role no puede ser nulo
		if( pcRole != NULL )
		{
			Role_ID.Set( pcRole ) ;
		}
		else
		{
			DUMP_CHLOG_ERROR( M4_CH_DM_NULL_ROLE ) ;
			iResult = M4_ERROR ;
		}
	}


// Control de tamaños

	if( pcOrg != NULL )
	{
		iLength = strlen( pcOrg ) ;

		if( iLength > M4CL_MAX_ORG_ID )
		{
			memcpy( acString, pcOrg, M4CL_MAX_ORG_ID ) ;
			acString[ M4CL_MAX_ORG_ID ] = '\0' ;
			DUMP_CHLOG_WARNINGF( M4_CH_DM_ATTR_TRUNCATED, M4CMPROP_CHANNEL_ID_ORGANIZATION << pcOrg << M4CL_MAX_ORG_ID << acString ) ;
			Organization.Set( acString ) ;
			Organization.Get( pcOrg ) ;
		}
	}

	if( pcRole != NULL )
	{
		iLength = strlen( pcRole ) ;

		if( iLength > M4CL_MAX_ROLE_ID )
		{
			memcpy( acString, pcRole, M4CL_MAX_ROLE_ID ) ;
			acString[ M4CL_MAX_ROLE_ID ] = '\0' ;
			DUMP_CHLOG_WARNINGF( M4_CH_DM_ATTR_TRUNCATED, M4CMPROP_CHANNEL_ID_APP_ROLE << pcRole << M4CL_MAX_ROLE_ID << acString ) ;
			Role_ID.Set( acString ) ;
			Role_ID.Get( pcRole ) ;
		}
	}

// Control de validez

	if( iResult == M4_SUCCESS )
	{
		// Si no estamos creados decimos que tire para alante
		iValid = 1 ;

		// Bug 0317381 El RSM por defecto debe ser M4ADM
		memcpy( pcRSM, M4CL_SEC_SUPER_USER, strlen( M4CL_SEC_SUPER_USER ) + 1 ) ;

		if( ai_bCheckSec && poLogonAdaptor != NULL && poSessionChannel != NULL && poSessionChannel->GetpChannelManager() != NULL )
		{
			// Conseguimos el role del canal sesión si no lo teníamos ya
			if( pcSessionRole == NULL )
			{
				pcSessionRole = poLogonAdaptor->GetRole() ;
			}

			/* Bug 0116998
			Si la sociedad es "" es como si fuese null
			*/
			if( pcOrg != NULL && *pcOrg == '\0' )
			{
				pcOrg = NULL ;
			}

			// Sólo si viene sociedad o el role no es M4ADM o el role del canal sesión no es M4ADM
			if(    pcOrg != NULL
				|| pcSessionRole == NULL
				|| strcmpi( pcRole, M4CL_SEC_SUPER_USER ) != 0
				|| strcmpi( pcSessionRole, M4CL_SEC_SUPER_USER ) != 0 )
			{
// Comment for compatibility !!!!!!!!!!!!!!!!!
				iValid = poLogonAdaptor->CheckRoleAndOrg( pcRole, pcOrg, &pcRSM, M4CL_MAX_RSM_ID, m_iIsLevel2 ) ;
			}
		}

		if( iValid != 1 )
		{
			// mensaje de error ya lo suelta javi o yo si es -1
			iResult = M4_ERROR ;
		}
		else
		{
			RSM_ID.Set( pcRSM ) ;
		}
	}

	if( iResult != M4_SUCCESS )
	{
		Organization.SetNull() ;
		RSM_ID.SetNull() ;
		Role_ID.SetNull() ;
	}

    return( iResult ) ;
}



m4return_t ClChannel :: _ApplySecurity( void )
{
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    return m_pClChannel_Data->InitSecurity() ;
}



m4return_t ClChannel :: AttachRSM( void )
{

    m4return_t		iResult ;
	m4pchar_t		pcRSM ;
    ClCompiledMCR   *poCMCR ;


	if( IsBuilt() == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

    poCMCR = GetpChannelDef()->GetpCMCR() ;

// le quitamos la protección
// y lo ponemos el role a nulo para que coja el del canal sesión

	RSM_ID.SetFlags( M4_PROP_WRITE ) ;
	Role_ID.SetFlags( M4_PROP_WRITE ) ;

	Role_ID.SetNull() ;

    iResult = _GetSessionInfo( M4_TRUE ) ;

	RSM_ID.UnSetFlags( M4_PROP_WRITE ) ;
	Role_ID.UnSetFlags( M4_PROP_WRITE ) ;

    if( iResult == M4_ERROR )
	{
        return( iResult ) ;
	}

	RSM_ID.Get( pcRSM ) ;

    iResult = GetpFactory()->MDFac_AttachRSM( poCMCR, pcRSM ) ;

    if( iResult == M4_ERROR )
    {
// ya sale el error por debajo
        return( M4_ERROR ) ;
    }

    iResult = _ApplySecurity() ;

    return( iResult ) ;
}

//----------------------------------------------------------------------------
//Seguridad
//----------------------------------------------------------------------------








m4return_t ClChannel::CommitPersisted (VersionMark &ai_version)
{
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    return m_pClChannel_Data->CommitPersisted (ai_version);
}


m4return_t ClChannel::RollbackPersisted (void)
{
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    return m_pClChannel_Data->RollbackPersisted ();
}


m4return_t ClChannel::ReLoad (void)
{

	// Me creo un Access auxiliar para recorrerme el canal.
	ClAccess * pAcc=CreateAccess();

	M4_ASSERT  (pAcc);

	m4return_t ret = pAcc->ReLoad();

	pAcc->Destroy();

	return ret ;
}


m4return_t	ClChannel :: CheckModify( m4bool_t &ao_bModified )
{
	ao_bModified = M4_FALSE ;
    return( m_pClChannel_Data->CheckModify( ao_bModified ) ) ;
}


ClCompiledMCR *ClChannel::GetpCMCR (void)
{
    return m_pClChannel_Data->GetpChannelDef ()->GetpCMCR ();
}

//----------------------------------------------------------------------------
//JIT
//----------------------------------------------------------------------------

ClCodeJIT *ClChannel::_InitJIT (void)
{
	if ( !m_poCodeJIT )
		m_poCodeJIT = new ClCodeJIT(this);
	return m_poCodeJIT;
}

void ClChannel::InitFileTable (void)
{
	if ( !m_poFileTable )
		m_poFileTable = new ClFileTable;
}



//--------------------------------------------------------------------------------
//Parametros para el cacheo
//--------------------------------------------------------------------------------

m4return_t ClChannel::BuildChannelParamId (ClCompiledMCR * ai_pCMCR, m4VariantType * &ao_rpVarParamsId, m4uint32_t &ao_iNumParams)
{
	ClHandle hItem, hNode;
	ClNoCallableItem oItemAux;
	m4return_t ret = M4_SUCCESS;
	
	m4uint16_t iParams = ai_pCMCR->GetChannelNumberOfParameters();
	
	ao_iNumParams = iParams;
	
	if (!iParams)
	{
		ao_rpVarParamsId = 0;
		return M4_SUCCESS;
	}    
	
	ao_rpVarParamsId = new m4VariantType[iParams];
	
	CHECK_CHLOG_DEBUGF( m_pClChannel_Data == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	
	for (m4uint16_t i = 0; i < iParams; i++)
	{
		hItem = ai_pCMCR->GetChannelParameterHandleByPosition(i);
		
		if (0 == hItem)
		{
			//ERROR
			ret = M4_ERROR;
			break;
		}
		
		hNode = ai_pCMCR->GetItemNodeHandle(hItem);
		
		if (0 == hNode)
		{
			//ERROR
			ret = M4_ERROR;
			break;
		}
		
		M4_ASSERT (m_pClChannel_Data->Node.Get(hNode));
		oItemAux.Attach(m_pClChannel_Data->Node.Get(hNode));
		
		oItemAux.MoveTo (hItem);
		M4_ASSERT (oItemAux.GetIndex() != M4DM_EOF_INDEX);
		ret = oItemAux.Value.Get(ao_rpVarParamsId[i]);
		
		if (M4_ERROR == ret)
		{
			//ERROR
			break;
		}
	}
	
	if (M4_ERROR == ret) {
		//ERROR
		delete [] ao_rpVarParamsId;
		ao_rpVarParamsId = 0;
		ao_iNumParams = 0;
	}
	
	return ret;
}


// jcrizacion ====================================================================
// Función peligrosa para cambiar al acanal sesión de un channelmanager a otro
// Sólo la debe utilizar el appclien de emulación
// jcrizacion ====================================================================

m4return_t	ClChannel :: _TeleTransportTo( ClChannelManager *ai_poNewChannelManager )
{
	if( m_poClChannelManager != NULL )
	{
		m_poClChannelManager->Channel.Detach( this ) ;
	}

	m_poClChannelManager = ai_poNewChannelManager ;

	if( m_poClChannelManager != NULL )
	{
		/* Bug 0271606
		Se reinserta el canal sesión al principio
		*/
		m_poClChannelManager->Channel.Attach( this, M4_TRUE ) ;
	}

	return( M4_SUCCESS ) ;
}



// checks consistency
m4return_t ClChannel::Check_L2DM_Consistency(ClChannelManager *ai_pCM, ClChannelConsistency* ai_pConsMap /*=0*/)
{
	ClChannelConsistency* pConsMap = ai_pConsMap;
	m4return_t res = M4_SUCCESS;

	if (!pConsMap) {
		pConsMap = new ClChannelConsistency;
	} else {
		if (pConsMap->IsVisited(this)) return res;
	}

	if (m_poClChannelManager != ai_pCM) {
		if (m_poClChannelManager) {
			DUMP_CHLOG_ERRORF(M4_CH_DM_INCONSISTENCY_REFERENCE, M4Obj(GetpCMCR()));
		} else {
			DUMP_CHLOG_ERROR(M4_CH_DM_INCONSISTENCY_REFNULL);
		}
		res = M4_ERROR;
	}

	/* Bug 0155093
	Los accesos al propio canal no deben contar como referencias
	*/
	m4uint32_t iLength = Access.Count() ;

	if( iLength > 0 && Level2.GetSelfAccess() != NULL )
	{
		iLength-- ;
	}

	pConsMap->AddRef(this, iLength);

	m4int32_t iL2 = Level2.m_oInstanceMap.Count();
	ClChannel* poChannel = 0;

	while (iL2--) {
		poChannel = Level2.m_oInstanceMap.GetAccess(iL2)->GetpChannel();
		pConsMap->DecrRef(poChannel, 1);
		res = poChannel->Check_L2DM_Consistency(ai_pCM, pConsMap);
		if (res != M4_SUCCESS)
			break;
	}

	if (!ai_pConsMap) {
		if (res == M4_SUCCESS) {
			res = pConsMap->Check(ai_pCM);
			if (res != M4_SUCCESS) {
				DUMP_CHLOG_ERROR(M4_CH_DM_EXTERNAL_ACCESS_DETECTED);
			}
		}

		delete pConsMap;
	}

	return res;
}


//ChannelManager:

m4return_t ClChannel::Attach (ClChannelManager *ai_poChannelManager)
{
	m4return_t res = Check_L2DM_Consistency(0);
	if (res != M4_SUCCESS) 
		return res;

	if (!ai_poChannelManager)
		return M4_SUCCESS;

	return _AttachRecursive(ai_poChannelManager);
}

m4return_t ClChannel::_AttachRecursive (ClChannelManager *ai_pcm)
{
	m4return_t res = M4_SUCCESS;

	if (m_poClChannelManager != ai_pcm) {
	
		m_poClChannelManager = ai_pcm;

		if (m_poClChannelManager) {
			res = m_poClChannelManager->Channel.Attach (this);
			if (res != M4_SUCCESS)
	return res;

			m4int32_t iL2 = Level2.m_oInstanceMap.Count();
			while (iL2--) {
				res = Level2.m_oInstanceMap.GetAccess(iL2)->GetpChannel()->_AttachRecursive(ai_pcm);
				if (res != M4_SUCCESS)
					return res;
			}
		}
	}

	return res;
}


m4return_t ClChannel::_Detach (m4bool_t ai_bdetach/* M4_TRUE*/, m4bool_t ai_bDoRecursive /* M4_TRUE */)
{
	m4return_t res = M4_SUCCESS;

	if (ai_bdetach && ai_bDoRecursive) {
		res = Check_L2DM_Consistency(m_poClChannelManager);
		if (res != M4_SUCCESS) return res;
	}

	return _DetachRecursive(ai_bdetach,ai_bDoRecursive, NULL);
}


m4return_t ClChannel::_DetachRecursive (m4bool_t ai_bdetach, m4bool_t ai_bDoRecursive, ClChannelConsistency* ai_pConsMap)
{
	m4return_t res = M4_SUCCESS;
	ClChannelConsistency* pConsMap = ai_pConsMap;
	ClChannel* poChannel = NULL;

	if (ai_bdetach && ai_bDoRecursive)
	{
		m4int32_t iL2 = Level2.m_oInstanceMap.Count();

		if( iL2 > 0 )
		{
			/* Bug 0203297
			Se evitan los ciclos
			*/
			if (!pConsMap) {
				pConsMap = new ClChannelConsistency;
			}
			pConsMap->AddRef(this, iL2);

			while (iL2--)
			{
				poChannel = Level2.m_oInstanceMap.GetAccess(iL2)->GetpChannel();

				if (pConsMap->IsVisited(poChannel)) {
					continue;
				}

				res = poChannel->_DetachRecursive(ai_bdetach, ai_bDoRecursive, pConsMap);
				if (res != M4_SUCCESS){
					return res;
				}
			}

			if (!ai_pConsMap) {
				delete pConsMap;
			}
		}
	}

	if (m_poClChannelManager && ai_bdetach) {
		// _DetachL2(); Done implicitly in ClChannelConsistency::Check() EFV
		m_poClChannelManager->Channel.Detach (this);
	}

	SetMainLConn(NULL);

	// is a forced release ? (DetachAndLeaveChannelManager())
	if (ai_bdetach == M4_FALSE && ai_bDoRecursive == M4_FALSE) {
		Access._DetachAll();

		// Como he borrado el vector de accesos a pelo (los accesos no se han destruido)
		// tengo que hacer un reset de los contadores de referencias de los ClRTItemIterator de los nodos
		// para que cuando se borre el canal no salte al assert en el destructor del _ClSmartPointer.
		m_pClChannel_Data->Node.ResetRTItemIteratorsCountReferences();
	}

	m_poClChannelManager = 0;

	return res;
}

ClCMCRFactory *ClChannel::GetpFactory (void)
{
	CHECK_CHLOG_ERROR( m_poClChannelManager == NULL, NULL, M4_CH_DM_CHANNELMANAGERNULL ) ;

	ClCMCRFactory *poFactory = m_poClChannelManager->GetpFactory() ;
	CHECK_CHLOG_ERROR( poFactory == NULL, NULL, M4_CH_DM_FACTORYNULL ) ;

    return poFactory;
}

m4return_t
ClChannel::GetSize(ClSize &ao_size)
{
	m4return_t ret = M4_SUCCESS;

	ao_size += sizeof(ClChannel);
		
// Propiedades
		ao_size -= sizeof(ApplyMappings);
	ret |= ApplyMappings.GetSize(ao_size);
		ao_size -= sizeof(MetaIdentifier);
	ret |= MetaIdentifier.GetSize(ao_size);

		ao_size -= sizeof(StartDate);
	ret |= StartDate.GetSize(ao_size);
		ao_size -= sizeof(EndDate);
	ret |= EndDate.GetSize(ao_size);
		ao_size -= sizeof(CorrectionDate);
	ret |= CorrectionDate.GetSize(ao_size);

		ao_size -= sizeof(VM_StartDate);
	ret |= VM_StartDate.GetSize(ao_size);
		ao_size -= sizeof(VM_EndDate);
	ret |= VM_EndDate.GetSize(ao_size);
		ao_size -= sizeof(VM_ExecutionDate);
	ret |= VM_ExecutionDate.GetSize(ao_size);

		ao_size -= sizeof(Data_StartDate);
	ret |= Data_StartDate.GetSize(ao_size);
		ao_size -= sizeof(Data_EndDate);
	ret |= Data_EndDate.GetSize(ao_size);
		ao_size -= sizeof(Data_CorrectionDate);
	ret |= Data_CorrectionDate.GetSize(ao_size);

		ao_size -= sizeof(Data_Language);
	ret |= Data_Language.GetSize(ao_size);
		ao_size -= sizeof(Meta_Language);
	ret |= Meta_Language.GetSize(ao_size);

		ao_size -= sizeof(UnitDate);
	ret |= UnitDate.GetSize(ao_size);

		ao_size -= sizeof(LDB_LastLogicConn);
	ret |= LDB_LastLogicConn.GetSize(ao_size);
		ao_size -= sizeof(CsType);
	ret |= CsType.GetSize(ao_size);

		ao_size -= sizeof(RSM_ID);
	ret |= RSM_ID.GetSize(ao_size);
		ao_size -= sizeof(Role_ID);
	ret |= Role_ID.GetSize(ao_size);
		ao_size -= sizeof(Organization);
	ret |= Organization.GetSize(ao_size);
		ao_size -= sizeof(OrganizationType);
	ret |= OrganizationType.GetSize(ao_size);

		ao_size -= sizeof(Tramitation);
	ret |= Tramitation.GetSize(ao_size);

		ao_size -= sizeof(RoundCurrency);
	ret |= RoundCurrency.GetSize(ao_size);
		ao_size -= sizeof(RoundNumber);
	ret |= RoundNumber.GetSize(ao_size);
		ao_size -= sizeof(CurrencyPrecision);
	ret |= CurrencyPrecision.GetSize(ao_size);
		ao_size -= sizeof(NumberPrecision);
	ret |= NumberPrecision.GetSize(ao_size);

		ao_size -= sizeof(ExecuteRealSQLMode);
	ret |= ExecuteRealSQLMode.GetSize(ao_size);

	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}

// Clases Base
		ao_size -= sizeof(ClAutoLoad_PlugIn);
	if (ClAutoLoad_PlugIn::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

		ao_size -= sizeof(_AClClient_Notifier);
	if (_AClClient_Notifier	::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}


// Objetos
		ao_size -= sizeof(Property);
	if (Property.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
		ao_size -= sizeof(Level2);
	if (Level2.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
		ao_size -= sizeof(m_oGroupInfo);
	if (m_oGroupInfo.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
		ao_size -= sizeof(BackMCR);
	if (BackMCR.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

// Referencias a objetos
	if (m_pClChannel_Data){
		if (m_pClChannel_Data->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	if (m_poCodeJIT){
		if (m_poCodeJIT->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

		ao_size -= sizeof(Access);
	if (Access.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t ClChannel::SetNewRecordVersion(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion)
{
    ClRecordVersionProperty* pRecordVersionProperty = new ClRecordVersionProperty;
    m4return_t iRet = pRecordVersionProperty == NULL ? M4_ERROR : M4_SUCCESS;

    if (iRet == M4_SUCCESS)
    {
        iRet = Property.AddProperty(M4CMPROP_CHANNEL_NEW_RECORD_VERSION, pRecordVersionProperty);
        pRecordVersionProperty->Set(ai_iRecordId, ai_dNewVersion);
    }

    if (iRet != M4_SUCCESS)
        delete pRecordVersionProperty;

    return iRet;
}

m4bool_t ClChannel::IsEqual( m4pcchar_t ai_pcChannelId, m4pcchar_t ai_pccTramitation, m4uint8_t ai_iCSAccessType, m4pchar_t ai_pcRole, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4bool_t ai_bCheckRole )
{

	m4return_t iResult = M4_ERROR;
	m4bool_t bIsEqual = M4_FALSE;
	m4pchar_t pcRole, pcOrg;
	m4pcchar_t pccChanId = NULL; 
	m4pchar_t pcTramitation = NULL; 
	m4VariantType vAux;


	if( IsBuilt() == M4_TRUE )
	{
		pccChanId = GetpChannelDef()->Id() ;
	}
	else
	{
		pccChanId = NULL ;
	}

	if( pccChanId == NULL )
	{
		return M4_FALSE;
	}

	// Si no se llaman igual no lo pillamos
	if( strcmpi( ai_pcChannelId, pccChanId ) != 0 )
	{
		return M4_FALSE;
	}


	CsType.Get( vAux ) ;

	// Si no tienen el mismo Cs Type no lo pillamos
	if( ai_iCSAccessType != vAux.Data.DoubleData )
	{
		return M4_FALSE;
	}


	// Si no tienen la misma tramitación no lo pillamos, salvo que sea el canal sesión
	if( strcmpi( pccChanId, M4_SESSION_CHANNEL_ID ) != 0 )
	{
		Tramitation.Get( pcTramitation ) ;

		if( ai_pccTramitation != NULL )
		{
			if( pcTramitation != NULL )
			{
				if( strcmpi( ai_pccTramitation, pcTramitation ) != 0 )
				{
					// Son distintos
					return M4_FALSE;
				}
				else
				{
					// Son iguales, no se hace nada
				}
			}
			else
			{
				// Uno nulo y otro no
				return M4_FALSE;
			}
		}
		else
		{
			if( pcTramitation != NULL )
			{
				// Uno nulo y otro no
				return M4_FALSE;
			}
			else
			{
				// Son iguales por null, no se hace nada
			}
		}
	}


	// Si no hay que chequear más lo pillamos
	if( ai_bCheckRole == M4_FALSE )
	{
		return M4_TRUE;
	}


	// Tienen que tener una sociedad de construcción equivalente
	Organization.Get( pcOrg ) ;

	if( ai_pcOrg != NULL )
	{
		if( pcOrg != NULL )
		{
			iResult = GetpFactory()->MDFac_IsEqualOrg( "M4OBJECT_MAPPINGS", ai_pcOrg, pcOrg, ai_pcChannelId, M4_TRUE, bIsEqual ) ;

			if( iResult == M4_SUCCESS && bIsEqual == M4_TRUE )
			{
				// Son iguales, no se hace nada
			}
			else
			{
				// Son distintos
				return M4_FALSE;
			}
		}
		else
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
	}
	else
	{
		if( pcOrg != NULL )
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
		else
		{
			// Son iguales por null, no se hace nada
		}
	}


	// Tienen que tener el mismo role
	Role_ID.Get( pcRole ) ;

	if( ai_pcRole != NULL )
	{
		if( pcRole != NULL )
		{
			if( strcmpi( ai_pcRole, pcRole ) != 0 )
			{
				// Son distintos
				return M4_FALSE;
			}
			else
			{
				// Son iguales, no se hace nada
			}
		}
		else
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
	}
	else
	{
		if( pcRole != NULL )
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
		else
		{
			// Son iguales por null, no se hace nada
		}
	}


	// Si es cualquier tipo de sociedad lo pillamos
	if( ai_iOrgType == M4CL_ORGANIZATION_L2_TYPE_ANY )
	{
		return M4_TRUE;
	}

	OrganizationType.Get( vAux ) ;

	// Si no tienen el mismo tipo de organización no lo pillamos
	if( ai_iOrgType != vAux.Data.DoubleData )
	{
		return M4_FALSE;
	}

	// Si es distinto de mono sociedad lo pillamos
	if( ai_iOrgType != M4CL_ORGANIZATION_L2_TYPE_MONO )
	{
		return M4_TRUE;
	}


//	Organization.Get( pcOrg ) ;

	if( ai_pcOrg != NULL )
	{
		if( pcOrg != NULL )
		{
			if( strcmpi( ai_pcOrg, pcOrg ) != 0 )
			{
				// Son distintos
				return M4_FALSE;
			}
			else
			{
				// Son iguales, no se hace nada
			}
		}
		else
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
	}
	else
	{
		if( pcOrg != NULL )
		{
			// Uno nulo y otro no
			return M4_FALSE;
		}
		else
		{
			// Son iguales por null, no se hace nada
		}
	}

	return M4_TRUE;
}


m4uint32_t	ClChannel::NewBookMark( m4uint8_t ai_iType, ClAccess *ai_poAccess )
{

    m4return_t      iResult = M4_ERROR ;
	m4uint32_t		iHandle = -1 ;
	ClAccessBookMark *poBookMark ;


	poBookMark = new ClAccessBookMark() ;

	if( ai_iType == M4CL_BOOKMARK_BY_PK )
	{
		iResult = poBookMark->GetFrom_PK( *ai_poAccess ) ;
	}
	else
	{
		iResult = poBookMark->GetFrom( *ai_poAccess ) ;
	}

	if( iResult == M4_ERROR )
	{
		delete( poBookMark ) ;
		return -1 ;
	}

	iHandle = AddPointer( poBookMark );
	m_oBookMarks.insert( ClBookMarkMap::value_type( iHandle, poBookMark ) ) ;

	return iHandle ;
}


m4uint32_t	ClChannel::ApplyBookMark( m4uint32_t ai_iBookMark, ClAccess *ai_poAccess )
{

    m4return_t      iResult = M4_ERROR ;
	itBookMarkMap	it ;


	it = m_oBookMarks.find( ai_iBookMark ) ;

	if( it == m_oBookMarks.end() )
	{
		return M4_ERROR ;
	}

	iResult = it->second->ApplyTo( *ai_poAccess ) ;

	if( iResult == M4_ERROR )
	{
		return M4_ERROR ;
	}

	return iResult ;
}


m4uint32_t	ClChannel::ReleaseBookMark( m4uint32_t ai_iBookMark )
{

	itBookMarkMap	it ;

	// Liberar el bookmark.
	RemovePointer( ai_iBookMark );

	it = m_oBookMarks.find( ai_iBookMark ) ;

	if( it == m_oBookMarks.end() )
	{
		return M4_ERROR ;
	}

	delete( it->second ) ;
	m_oBookMarks.erase( it ) ;

	return M4_SUCCESS ;
}


// Funciones de java ==========================================================

void	ClChannel::ReferenceJavaContext( M4JavaContext *ai_poContext )
{
	if( ai_poContext != NULL )
	{
		pair< set< M4JavaContext*, less<M4JavaContext*> >::iterator, bool >	p = m_aoJavaContexts.insert( ai_poContext );

		if( p.second == true )
		{
			ai_poContext->AddReference() ;
		}
	}
}


void	ClChannel::ReleaseJavaContexts( m4bool_t ai_bOnlyEmpty )
{

	size_t					iLength = 0 ;
	M4JavaManager			*poJavaManager = NULL ;
	list< M4JavaContext* >	oDeleted ;

	set< M4JavaContext*, less<M4JavaContext*> >::iterator	it ;
	list< M4JavaContext* >::iterator						itDeleted ;


	iLength = m_aoJavaContexts.size() ;

	if( iLength > 0 )
	{
		if( m_poClChannelManager != NULL )
		{
			poJavaManager = m_poClChannelManager->GetJavaManager() ;
		}

		it = m_aoJavaContexts.begin() ;

		while( it != m_aoJavaContexts.end() )
		{
			if( ai_bOnlyEmpty == M4_FALSE || (*it)->IsEmpty() == M4_TRUE )
			{
				oDeleted.push_back( *it ) ;
			}
			it++ ;
		}

		// Bug 0312509 Sólo se eliminan las que hay que dejar de referenciar
		itDeleted = oDeleted.begin() ;

		while( itDeleted != oDeleted.end() )
		{
			m_aoJavaContexts.erase( *itDeleted ) ;

			(*itDeleted)->RemoveReference() ;

			if( poJavaManager != NULL )
			{
				poJavaManager->UnReferenceJavaContext( *itDeleted ) ;
			}
			itDeleted++ ;
		}
	}
}


m4uint32_t	ClChannel::AddPointer( void* ai_poPointer )
{
	// Construye el identificador único a partir de un contador
	static	m4uint32_t	s_iHandle = 0 ;
	static	ClMutex		s_oMutex( M4_TRUE ) ;

	m4uint32_t	iHandle = 0 ;

	s_oMutex.Lock() ;
	iHandle = ++s_iHandle ;
	s_oMutex.Unlock() ;

	m_aoPointers.insert( map< m4uint32_t, void*, less<m4uint32_t> >::value_type( iHandle, ai_poPointer ) ) ;
	return( iHandle ) ;
}


m4bool_t	ClChannel::RemovePointer( m4uint32_t ai_iHandle )
{
	map< m4uint32_t, void*, less<m4uint32_t> >::iterator it = m_aoPointers.find( ai_iHandle ) ;

	if( it != m_aoPointers.end() )
	{
		m_aoPointers.erase( it ) ;
		return( M4_TRUE) ;
	}
	return( M4_FALSE ) ;
}


void*	ClChannel::GetPointer( m4uint32_t ai_iHandle )
{
	map< m4uint32_t, void*, less<m4uint32_t> >::iterator it = m_aoPointers.find( ai_iHandle ) ;

	if( it != m_aoPointers.end() )
	{
		return( it->second ) ;
	}
	return( NULL ) ;
}


m4uint32_t	ClChannel::GetPointerKey( void* ai_poPointer )
{
	for( map< m4uint32_t, void*, less<m4uint32_t> >::iterator it = m_aoPointers.begin(); it != m_aoPointers.end(); it++ )
	{
		if( it->second == ai_poPointer )
		{
			return it->first;
		}
	}

	return 0;
}


// ----------------------------------------------------------------
// class ClChannelConsistency
// ----------------------------------------------------------------

// Consistency check implementation
ClChannelConsistency::~ClChannelConsistency()
{
	for (itConsistencyMap it = m_oMap.begin(); it != m_oMap.end(); ++it)
		delete (*it).second;
}

void ClChannelConsistency::AddRef(ClChannel *ai_poRef, m4int32_t ai_iNRef, m4bool_t ai_bIsVisited)
{
	itConsistencyMap it = m_oMap.find(ai_poRef);

	if (it != m_oMap.end()) {
		(*it).second->Add(ai_iNRef,ai_bIsVisited);
		return;
	}

	ClConsState *poState = new ClConsState(ai_iNRef,ai_bIsVisited);
	m_oMap.insert(ClConsistencyMap::value_type(ai_poRef,poState));
}

m4return_t ClChannelConsistency::Check(ClChannelManager *ai_pcm)
{
	if (ai_pcm != NULL)
	{
		m4uint32_t iChannels = ai_pcm->Channel.Count();
		while (iChannels--)
		{
			ClChannel *pChannel = ai_pcm->Channel[iChannels];
			
			if (IsVisited(pChannel))
				continue;
			
			m4uint32_t iL2 = pChannel->Level2.m_oInstanceMap.Count();
			while (iL2--)
			{
				ClAccess *pAccess = pChannel->Level2.m_oInstanceMap.GetAccess(iL2);
				if (pAccess != NULL && IsVisited(pAccess->GetpChannel()))
					DecrRef(pAccess->GetpChannel(),1);
			}
		}
	}
		
	for (itConsistencyMap it = m_oMap.begin(); it != m_oMap.end(); ++it)
		if ((*it).second->GetNRef())
			return M4_ERROR;


	// this should be not copied here. Now here to save changes in channel.hpp (EFV)	
	// CHANGE XXXXXXXXXXXXXXXX
	if (ai_pcm != NULL)
	{
		m4uint32_t iChannels = ai_pcm->Channel.Count();
		while (iChannels--)
		{
			ClChannel *pChannel = ai_pcm->Channel[iChannels];
			
			if (IsVisited(pChannel))
				continue;
			
			m4uint32_t iL2 = pChannel->Level2.m_oInstanceMap.Count();
			while (iL2--)
			{
				m4pcchar_t pcInstance;
				ClAccess *pAccess;
				
				if (pChannel->Level2.m_oInstanceMap.GetInstanceAndAccess(iL2,pcInstance,pAccess) != M4_SUCCESS)
					return M4_ERROR;

				if (pAccess != NULL && IsVisited(pAccess->GetpChannel()))
					if (pChannel->Level2.EraseL2Instance(pcInstance,M4_FALSE) != M4_SUCCESS)
						return M4_ERROR;
			}
		}
	}
	// CHANGE XXXXXXXXXXXXXXXX
		
	return M4_SUCCESS;
}

m4bool_t ClChannelConsistency::IsVisited(ClChannel *ai_poRef)
{
	for (itConsistencyMap it = m_oMap.begin(); it != m_oMap.end(); ++it)
		if ((*it).first == ai_poRef)
			return (*it).second->IsVisited();
	return M4_FALSE;
}
