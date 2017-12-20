//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             level2.cpp
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

#include "level2.hpp"
#include "dmres.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "dm.hpp"
#include "chan_dir.hpp"
#include "chlog.hpp"
#include "cllgadap.hpp"
#include "m4lgadapdef.hpp"
#include "sizerutils.hpp"
#include "chan_knl.hpp"
#include "iexecutor.hpp"
#include "cldmcontext.hpp"
#include "m4mdrt.hpp"
#include "iproxymanager.hpp"
#include "m4mdfac.hpp"


ClLevel2::ClLevel2 (void)
{
	m_pSelfAccess = 0;
}

ClLevel2::~ClLevel2 (void)
{
	if (!m_oChannelInstJIT.empty())
	{
		m_oChannelInstJIT.clear();
	}

	DestroySelfAccess();
}

void ClLevel2::Init (ClChannel * ai_pChannel)
{
	m_pChannel = ai_pChannel;
}


m4return_t ClLevel2::CreateSelfAccess()
{
	if (m_pSelfAccess)
	{
		return M4_SUCCESS;
	}

	if (!m_pChannel)
	{
		return M4_ERROR;
	}
	
	m_pSelfAccess = m_pChannel->CreateAccess();

	if (m_pSelfAccess == 0)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

void ClLevel2::DestroySelfAccess()
{
	if (m_pSelfAccess)
	{
		m_pSelfAccess->Destroy();
		m_pSelfAccess = 0;
	}
}

void ClLevel2::SetSelfAcces(ClAccess* ai_pAccess)
{
	if( m_pSelfAccess == NULL && ai_pAccess->GetpChannel() == m_pChannel )
	{
		m_pSelfAccess = ai_pAccess;
	}
}

m4bool_t ClLevel2::IsSelfAccessCreated()
{
	return m_pSelfAccess ? M4_TRUE : M4_FALSE;
}

//Para conseguir el acceso de otro canal. 
//Maneja el m_oInstanceMap, buscando y almacenando nuevas instancias de canal con toda la política asociada.
m4return_t ClLevel2::GetL2Access ( m4pcchar_t ai_ChannelInstance, ClAccess** ao_pAccess, m4bool_t ai_bJIT)
{
	m4return_t res = M4_SUCCESS;

	m4bool_t bCreateChannel = M4_TRUE;	
	m4bool_t bCheckRole = M4_TRUE;	
	m4bool_t bAutoLoad = M4_TRUE, bJITFind = M4_FALSE;
	
	m4pcchar_t pcChannelId, pcLocalChannel;
	m4pcchar_t pcRole, pcOrg;

	m4char_t acChannelId[ M4CL_MAX_T3_ID + 1 ];
	m4char_t acRole[ M4CL_MAX_ROLE_ID + 1 ];
	m4char_t acOrg[ M4CL_MAX_ORG_ID + 1 ];
		 
	m4uint32_t hChannelInstance;
	m4uint8_t iScope, iOpenMode, iCSAccessType, iOrgType;
	itMapChannInstJIT itMapCI;
	ClCompiledMCR* poCMCR;

	m4language_t iDataLanguage;

	// Buscamos el alias. en el mapa del ejecutor 
	res = m_oInstanceMap.Find ( ai_ChannelInstance, ao_pAccess );

    if ( res != M4_ERROR ) // está en la tabla. Todo OK.
		return res;

	////JIT. Por si es de nivel 2.
	///////////	Buscamos primero si ha definido una instancia en RunTime, para JIT.
	if ( (ai_bJIT) && ( (itMapCI = m_oChannelInstJIT.find (ai_ChannelInstance) ) != m_oChannelInstJIT.end()) )
	{
		// bg 284493
		// Nos quedamos una copia porque se podría llegar
		// a reubicar el elemento dentro del mapa.
		strcpy( acChannelId, (*itMapCI).second.m_acChannel );
		pcChannelId = acChannelId;

		iScope = (*itMapCI).second.m_iShared;

		iOpenMode = (*itMapCI).second.m_iLoad;

		iCSAccessType = (*itMapCI).second.m_iCsAccess;

		//El valor por defecto es NULL y en ese caso se coge el Role del padre.
		strncpy( acRole, (*itMapCI).second.m_acRole, M4CL_MAX_ROLE_ID );
		acRole[ M4CL_MAX_ROLE_ID ] = NULL;
		pcRole = acRole;

		strncpy( acOrg, (*itMapCI).second.m_acOrg, M4CL_MAX_ORG_ID );
		acOrg[ M4CL_MAX_ORG_ID ] = NULL;
		pcOrg = acOrg;

		iOrgType = (*itMapCI).second.m_iOrgType;

		bCheckRole = M4_TRUE;

		iDataLanguage = (*itMapCI).second.m_iDataLanguage;


		// jcr *** pedazo de apaño ***************
		// Si es el canal sesión lo ponemos todo cableado
		if( strcmpi( pcChannelId, M4_SESSION_CHANNEL_ID ) == 0 )
		{
			iScope = M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED ;
			iCSAccessType = M4CL_CSTYPE_L2_SITE ;
			
			// En SM es Both de verdad. En el resto según el sitio

			if (M4_TRUE == m_pChannel->GetpChannelManager()->GetDMContext()->IsSM())
			{
				iCSAccessType = M4CL_CSTYPE_L2_BOTH ;
			}

			bCheckRole = M4_FALSE ;
		}
	}
	else 
	// Si es una instancia normal o es de JIT pero no se encontró
	{
		// Leemos los datos Runtime del metacanal.
		poCMCR = m_pChannel->GetpChannelDef ()->GetpCMCR();

		hChannelInstance = poCMCR->GetChannelInstanceHandleById ( ai_ChannelInstance );

		if (hChannelInstance == 0)
		{
			pcLocalChannel = m_pChannel->GetpChannelDef()->Id();

			/* Bug 0105179
			Si  viene cadena vacía es que es el propio canal
			*/
			if( *ai_ChannelInstance == '\0' || strcmpi( ai_ChannelInstance, pcLocalChannel ) == 0 )
			{
				//La instancia no existe como tal y se corresponde con el nombre del canal local
				//Devolvemos un access al canal local como acces de nivel 2.
				if (CreateSelfAccess() == M4_SUCCESS)
				{
					*ao_pAccess = m_pSelfAccess;
					return M4_SUCCESS;
				}
				else
				{
					return M4_ERROR;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF(M4_CH_DM_WRONG_INSTANCE_HANDLE, ai_ChannelInstance << M4Obj(m_pChannel->GetpCMCR()) );
				return M4_ERROR;	
			}
		}	
	
		pcChannelId = poCMCR->GetInstanceT3Id ( hChannelInstance );
	
		iOpenMode = poCMCR->GetInstanceOpenMode ( hChannelInstance );

		iScope = poCMCR->GetInstanceScope ( hChannelInstance );

		iCSAccessType = poCMCR->GetInstanceCsAccess(hChannelInstance);

		pcRole = poCMCR->GetInstanceRoleId( hChannelInstance );

		pcOrg = poCMCR->GetInstanceOrganization( hChannelInstance );

		iOrgType = poCMCR->GetInstanceOrganizationType( hChannelInstance );

// ver si hay que usar el Role o no
		if( poCMCR->GetInstanceUseRole( hChannelInstance ) == 1 )
		{
			bCheckRole = M4_TRUE ;
		}
		else
		{
			bCheckRole = M4_FALSE ;
		}

		iDataLanguage = M4CL_LANGUAGE_TYPE_NONE; //No existe una función GetInstanceDataLanguage...
	}

	res = InsertL2Instance ( ai_ChannelInstance, pcChannelId, iScope, iOpenMode, iCSAccessType, ao_pAccess, (const m4pchar_t )pcRole, (const m4pchar_t )pcOrg, iOrgType, bCheckRole , iDataLanguage);

	return res; 
}


m4return_t ClLevel2::InsertL2Instance ( m4pcchar_t ai_ChannelInstance, m4pcchar_t ai_pcChannelId, 
									  m4uint8_t ai_iScope, m4uint8_t ai_iOpenMode, 
									  m4uint8_t ai_iCSAccessType, ClAccess** ao_pAccess,
									  m4pchar_t ai_pcRole,
									  m4pchar_t ai_pcOrg,
									  m4uint8_t ai_iOrgType,
									  m4bool_t ai_bCheckRole,
									  m4language_t ai_iDataLanguage)
{
	m4return_t res = M4_SUCCESS;

	m4bool_t bCreateChannel = M4_TRUE;	
	m4bool_t bAutoLoad = M4_TRUE;
	ClChannel * pChannel;
	ClAccess * pAccess;
	ClChannelManager * pChanMan;
	m4uint8_t iRealCSAccessType;
	m4pchar_t pcTramitation = NULL;
	m4VariantType vAux;


	// Obtenemos el ChannelManager
	pChanMan = m_pChannel->GetpChannelManager();


	// Si no viene el role en la llamada de nivel2 ponemos el del padre
	if( ai_pcRole == NULL || *ai_pcRole == '\0' )
	{
		m_pChannel->Role_ID.Get( ai_pcRole ) ;
	}

	// Si no viene la sociedad en la llamada de nivel2 ponemos la del padre
	if( ai_pcOrg == NULL || *ai_pcOrg == '\0' )
	{
		m_pChannel->Organization.Get( ai_pcOrg ) ;
	}

	// Se lee la tramitación del padre
	m_pChannel->Tramitation.Get( pcTramitation ) ;


	switch (ai_iCSAccessType)
	{
		case M4CL_CSTYPE_L2_FRONT : // FRONT !!!
			iRealCSAccessType = M4CL_CSTYPE_FRONT;
			break;

		case M4CL_CSTYPE_L2_BACK : // BACK !!!
			iRealCSAccessType = M4CL_CSTYPE_BACK;
			break;

		case M4CL_CSTYPE_L2_BOTH : // BOTH !!!
			iRealCSAccessType = M4CL_CSTYPE_BOTH;
			break;

		case M4CL_CSTYPE_L2_FATHER : // El del padre
			m_pChannel->CsType.Get( vAux ) ;
			iRealCSAccessType = m4uint8_t( vAux.Data.DoubleData ) ;
			break;

		case M4CL_CSTYPE_L2_SITE : // Según el sitio !!!
        default:
			if (M4_TRUE == m_pChannel->GetpChannelManager()->GetDMContext()->ExecutingInClient())
			{
				iRealCSAccessType = M4CL_CSTYPE_FRONT; // Esta es la parte front (cliente)
			}
			else
			{
				iRealCSAccessType = M4CL_CSTYPE_BACK;
			}
			break;
	}

	// Si es el tipo de sociedad del padre lo buscamos
	if( ai_iOrgType == M4CL_ORGANIZATION_L2_TYPE_FATHER )
	{
		m_pChannel->OrganizationType.Get( vAux ) ;
		ai_iOrgType = m4uint8_t( vAux.Data.DoubleData ) ;
	}

	// No lo tenemos en la tabla. Averigüemos que hacer.	
	if ( ( ai_iScope == M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED ) || 
		 ( ai_iScope == M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED ) )
	{
		m4pchar_t pcMappingId = NULL;
		m4char_t acM4ObjId[ M4CL_MAX_T3_ID + 1 ] ;

		strcpy( acM4ObjId, ai_pcChannelId ) ;

		if( ai_pcOrg != NULL )
		{
			pcMappingId = m_pChannel->GetpFactory()->MDFac_GetMappingById( "M4OBJECT_MAPPINGS", ai_pcOrg, acM4ObjId, M4_TRUE ) ;

			if( pcMappingId != NULL )
			{
				strcpy( acM4ObjId, pcMappingId ) ;
				delete [] pcMappingId ;
			}
		}

		res = m_oInstanceMap.FindChannel( acM4ObjId, pcTramitation, iRealCSAccessType, ai_pcRole, ai_iOrgType, ai_pcOrg, ai_bCheckRole, m_pChannel, &pChannel ) ;

		if( res != M4_ERROR )
		{
			// Encontrado
			bCreateChannel = M4_FALSE;
		}
		else if ( ai_iScope == M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED )
		{
			//Buscamos el canal en el ChannelManager
			ClChannelDirectory * pChanDir;
			m4uint32_t iNumChan, i;

			pChanDir = &( pChanMan->Channel );
			iNumChan = pChanDir->Count ();

			for ( i = 0; ( ( i < iNumChan ) && ( bCreateChannel ) ); i++ )
			{
				pChannel = (*pChanDir)[i];

				if (pChannel != m_pChannel)
				{
					if( pChannel->IsEqual( acM4ObjId, pcTramitation, iRealCSAccessType, ai_pcRole, ai_iOrgType, ai_pcOrg, ai_bCheckRole) == M4_TRUE )
					{
						bCreateChannel = M4_FALSE ;
					}
				}
			}
		}
	}


	if( bCreateChannel )
	{
        res = pChanMan->CreateChannel( pChannel ) ;

		// Le ponemos las propiedades

		vAux.SetDouble( iRealCSAccessType ) ;
	    pChannel->CsType.Set(vAux) ;  
		
		if( ai_pcRole != NULL )
		{
			pChannel->Role_ID.Set( ai_pcRole ) ;
		}

		if( ai_pcOrg != NULL )
		{
			pChannel->Organization.Set( ai_pcOrg ) ;
		}

		if( ai_iDataLanguage != M4CL_LANGUAGE_TYPE_NONE )
		{
			pChannel->Data_Language.Set( ai_iDataLanguage ) ;
		}
		else
		{
			/* Bug 0111115
			Se pasa el lenguaje de datos del canal padre si no viene
			*/
			res = m_pChannel->Data_Language.Get( vAux ) ;

			if( res == M4_SUCCESS && vAux.Type == M4CL_CPP_TYPE_NUMBER )
			{
				pChannel->Data_Language.Set( vAux ) ;
			}
		}


		// jcr. Si es any no ponemos nada y ya tomará la del canal
		// Ahora funcionaba de churro, porque si sale fuera del rango yo pongo la del canal
		if( ai_iOrgType != M4CL_ORGANIZATION_L2_TYPE_ANY )
		{
			vAux.SetDouble( ai_iOrgType ) ;
			pChannel->OrganizationType.Set( vAux ) ;
		}

		// jcr le ponemos de nivel 2
		pChannel->SetIsLevel2() ;

		// Se pone la tramitación del padre
		if( pcTramitation != NULL )
		{
			pChannel->Tramitation.Set( pcTramitation ) ;
		}

		res = pChannel->BuildFromId( ai_pcChannelId ) ;

		if (res==M4_ERROR)
		{
			pChannel->Destroy();
			DUMP_CHLOG_DEBUGF(M4_CH_DM_BUILD_FROM_ID, ai_pcChannelId );
			return M4_ERROR;
		}
	}

	//Creamos el access
	pAccess = pChannel->CreateAccess();

	/* Bug 0203297
	Se inserta primero el acceso para que si hay error no se pierda referencia
	*/
	m_oInstanceMap.Insert( ai_ChannelInstance, pAccess, pChannel, M4_TRUE );

	if( bCreateChannel )
	{
		if ( ai_iOpenMode == M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_BLOCK )
		{
			pChannel->WantsAutoLoad.Set (M4CL_AUTOLOAD_BLOCK); 
		}
		else if ( ai_iOpenMode == M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_PRG )
		{
			pChannel->WantsAutoLoad.Set (M4CL_AUTOLOAD_PRG); 
		}
		else if ( ai_iOpenMode == M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_NODESAYS )
		{
			pChannel->WantsAutoLoad.Set (M4CL_AUTOLOAD_NODESAYS);
		}
		else if ( ai_iOpenMode == M4CL_T3ALIAS_OPEN_MODE_LOAD )
		{
			//Hacemos la Load del canal
			res = pChannel->Load ();

			if (res==M4_ERROR)
			{
				DUMP_CHLOG_DEBUGF(M4_CH_DM_ERROR_LOAD, M4Obj(pChannel->GetpCMCR()) );
				return M4_ERROR;
			}
		}
		//else el modo será M4CL_T3ALIAS_OPEN_MODE_NO_LOAD
	}

	*ao_pAccess = pAccess;

	return M4_SUCCESS;

}


//Crea el Access del canal y lo inserta en la tabla de instancias L2.
m4return_t ClLevel2::InsertL2Channel ( m4pcchar_t ai_ChannelInstance, m4int32_t ai_hChannel )
{

	m4return_t	iResult = M4_SUCCESS ;
	ClAccess	*pAccess = NULL ;
	ClChannel	*pChannel = NULL ;
	ClChannel	*poOldChannel = NULL ;


	// Se comprueba que el handle sea válido
	pChannel = m_pChannel->GetpChannelManager()->GetChannelByHandle( ai_hChannel ) ;
	CHECK_CHLOG_ERRORF( pChannel == NULL, M4_ERROR, M4_CH_DM_NO_SUCH_META4OBJ, ai_hChannel << ai_ChannelInstance << M4Obj( m_pChannel->GetpCMCR() ) ) ;

	// Buscamos el alias. en el mapa del ejecutor 
	iResult = m_oInstanceMap.Find( ai_ChannelInstance, &pAccess, &poOldChannel );

	if( iResult != M4_ERROR )
	{
		/* Bug 
		Si existe se comprueba si es nulo es la misma instancia
		*/
		if( poOldChannel == pChannel )
		{
			return( M4_SUCCESS ) ;
		}

		DUMP_CHLOG_ERRORF( M4_CH_DM_REPEATED_INSTANCE, ai_ChannelInstance << M4Obj( m_pChannel->GetpCMCR() ) ) ;
		return( M4_ERROR ) ;
	}

	// Se crea el access y se añade a las instancias de nivel 2 como no propietario
	pAccess = pChannel->CreateAccess() ;
	iResult = m_oInstanceMap.Insert( ai_ChannelInstance, pAccess, pChannel, M4_TRUE ) ;

	return( iResult ) ;
}

//Obtiene el canal del Access e inserta todo en la tabla de instancias de L2.
m4return_t ClLevel2::InsertL2Access ( m4pcchar_t ai_ChannelInstance, ClAccess* ai_pAccess, m4bool_t ai_bIsOwner )
{
	ClAccess * pAuxAccess = NULL;
	ClChannel * pChannel = NULL;
	m4return_t res = M4_SUCCESS;

	// Buscamos el alias. en el mapa del ejecutor 
	res = m_oInstanceMap.Find ( ai_ChannelInstance, &pAuxAccess );

    if ( res != M4_ERROR ) // está en la tabla. Error.
		return M4_ERROR;

	pChannel = ai_pAccess->GetpChannel();

	m_oInstanceMap.Insert( ai_ChannelInstance, ai_pAccess, pChannel, ai_bIsOwner );

	return M4_SUCCESS;
}

m4pchar_t ClLevel2::GetChannelInstanceforJIT ( m4pchar_t ai_pccInstance )
{

	itMapChannInstJIT	itMap;

	if ((itMap = m_oChannelInstJIT.find ( ai_pccInstance ) ) != m_oChannelInstJIT.end() )
	{
		return (*itMap).second.m_acChannel;

	}
	
	return NULL;

}

// Quita la entrada de nivel 2 del mapa y destruye el Access (siempre). 
// Si ai_bDestroyChannel destruye tambien el channel
m4return_t ClLevel2::EraseL2Instance ( m4pcchar_t ai_ChannelInstance, m4bool_t ai_bDestroyChannel )
{
	return m_oInstanceMap.Erase ( ai_ChannelInstance, ai_bDestroyChannel );
}

// Quita todas las entrada de nivel 2 del mapa y destruye el Access (siempre). 
// Si ai_bDestroyChannel destruye tambien el channel
m4return_t ClLevel2::EraseAllL2Instances ( m4bool_t ai_bDestroyChannel, m4bool_t ai_bRemoteSync, ClChannelManager* ai_pChannelMamager )
{
	m4return_t ret;

	// EFV >>>
	DestroySelfAccess();
	// EFV <<<

	ret = m_oInstanceMap.EraseAll ( ai_bDestroyChannel, ai_pChannelMamager );

	/* Bug 0072297
	La llamada al EraseAllL2Instances ahora se hace por un servicio proxy
	y no por ejecución de un item mapped.
	*/
	if( ai_bRemoteSync == M4_TRUE && m_pChannel != NULL )
	{
		m4uint8_t iCsExeType = m_pChannel->GetpChannelDef()->CsExeType() ;
		m4uint8_t iCsType = m_pChannel->GetpChannelDef()->CsType() ;

		if( iCsExeType == M4CL_CSEXETYPE_PROXY && iCsType == M4CL_CSTYPE_FRONT )
		{
			ClChannelManager *poChannelManager = m_pChannel->GetpChannelManager() ;

			if( poChannelManager != NULL )
			{
				IProxyManager *poProxyManager = poChannelManager->GetProxyManager() ;

				if( poProxyManager != NULL )
				{
					m4uint32_t iRemoteId = m_pChannel->GetRemoteId() ;

					if( iRemoteId != 0 )
					{
						ret = poProxyManager->EraseAllL2Instances( "Proxy", iRemoteId ) ;
					}
				}
			}
		}
	}

	return ret;
}

m4return_t 
ClLevel2::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLevel2); 

		ao_size-=sizeof(m_oInstanceMap);
	if (m_oInstanceMap.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

		ao_size-=sizeof(m_oChannelInstJIT);
	if (GetMapStructureSize(m_oChannelInstJIT,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	itMapChannInstJIT	it;
	for (it = m_oChannelInstJIT.begin();it!=m_oChannelInstJIT.end();++it){

		ao_size+=((*it).first).capacity();
			// ya esta contada.
			ao_size-=sizeof((*it).second);
		if (((*it).second).GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}



