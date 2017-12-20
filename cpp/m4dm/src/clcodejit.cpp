
#include "clcodejit.hpp"
#include "m4mdfac.hpp"
#include "channel.hpp"
#include "chlog.hpp"
#include "m4mdrt.hpp"
#include "sizerutils.hpp"


ClCodeJIT::~ClCodeJIT ()
{
	while ( ! m_oCodeTable.empty() )
	{
		delete [] (* m_oCodeTable.begin ()).second;
		m_oCodeTable.erase ( m_oCodeTable.begin () );
	}

	ClCompiledMCR * pMCR = 0;

	while ( ! m_oMCforJIT.empty() )
	{
		pMCR = (*m_oMCforJIT.begin()).second;
		
		M4_ASSERT(pMCR); //Por si las moscas
		pMCR->Destroy();

		m_oMCforJIT.erase ( m_oMCforJIT.begin () );
	}
}


m4return_t ClCodeJIT::GetMetaChannel ( m4pcchar_t ai_pccChannelId, ClCompiledMCR *&ao_poCMCR )
{
	M4ClString	sChannelName(ai_pccChannelId);
	m4uint8_t	CSType = 0;
	m4bool_t	bFound = M4_FALSE;


	CSType = m_poChannel -> GetpCMCR () -> GetChannelCsType ();

	// No busco en el mapa de nivel 2, porque si hay caches de MC, voy a obtener la misma instancia.
	//

	itMapChannelJIT		itChannel;

	///Buscamos en el mapa de metacanales.
	if ( (itChannel = m_oMCforJIT.find(sChannelName) ) != m_oMCforJIT.end())
	{
#ifdef _DEBUG
		if ( (*itChannel).second -> GetChannelIsSeparable() )
		{

		// ¡¡¡¡¡	Solo en caso de que el canal que hemos levantado sea separable
		// ¡¡¡¡¡	si no siempre se levanta el Both.
		
		//			No podemos tener una referenecia a un metacanal con distinto
		//			CSType, porque todas las hemos creado desde esta instancia del canal.
		// 
			M4_ASSERT ( CSType == (*itChannel).second -> GetChannelCsType () );
		}
#endif

		ao_poCMCR = (*itChannel).second;
		return M4_SUCCESS;
	}



// Si no la hemos encontrado, levantamos el metacanal.

// jcr los parámetros
	ClCMCRFactory		*pCMCRFactory;
	ClBuidParameters	oParameters ;

	
	pCMCRFactory = m_poChannel->GetpChannelManager()->GetpFactory();


	oParameters.m_poLane = NULL ;				// No hay carril
	oParameters.m_pccM4ObjId = ai_pccChannelId ;
	oParameters.m_pccIdOrg = NULL ;
	oParameters.m_pccRSM = NULL ;				// No hay máscara
	oParameters.m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	oParameters.m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	oParameters.m_dCorDate = ClActualDate() ;
	oParameters.m_iCsType = CSType ;
	oParameters.m_iLanguage = M4CL_LANGUAGE_NONE ;
	oParameters.m_bConvertRSM = M4_FALSE ;		// No hay que convertir la máscara
	oParameters.m_bBuildSecurity = M4_FALSE ;	// No construimos seguridad
	oParameters.m_bApplyMappings = M4_FALSE ;	// Sin mapeos


	if( pCMCRFactory->MDFac_BuildFromId( oParameters, ao_poCMCR ) )
	{		
		return M4_ERROR;
	}


	m4bool_t bInsert;
	
	bInsert = ( m_oMCforJIT.insert (MapMetaChannelforJIT::value_type(sChannelName,ao_poCMCR)).second)? M4_TRUE : M4_FALSE ;

	if (bInsert != M4_TRUE )
	{
		//Hay que mirar a ver, porque ya tenemos lo que queríamos.
	}

	return M4_SUCCESS;
}
m4return_t 
ClCodeJIT::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClCodeJIT);

	ClDecodeFormula oDF; //para poder calcular la longitud de la formula
	
		ao_size-=sizeof(m_oCodeTable);
	if (GetMapStructureSize(m_oCodeTable,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	itMapCodeJIT	it;
	for (it=m_oCodeTable.begin();it!=m_oCodeTable.end();++it)
	{
		oDF.Init((*it).second);
		ao_size+=oDF.GetFormulaSize();
	}

		ao_size-=sizeof(m_oMCforJIT);
	if (GetMapStructureSize(m_oMCforJIT,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	itMapChannelJIT it2;
	for (it2=m_oMCforJIT.begin();it2!=m_oMCforJIT.end();++it2)
	{
		ao_size+=((*it2).first).capacity();
		// ¿Que hago con los metacanales?
	}

	return M4_SUCCESS;
}
