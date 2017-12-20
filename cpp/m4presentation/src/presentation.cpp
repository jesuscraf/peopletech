//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             presentation.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CInternalPresentationFactory class.
//
//
//==============================================================================

#include "asndef.h"
#include "_base64.h"
#include "mdupdate.hpp"		//Para los defines
#include "channel.hpp"

#include "access.hpp"
#include "channel.hpp"
#include "dm.hpp"
#include "m4mdrt.hpp"
#include "clpresen.hpp"
#include "presfac.hpp"
#include "m4presres.hpp"

#define M4_NOTFOUND    1


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CInternalPresentationFactory::CInternalPresentationFactory(ClPresentationFactory * ai_pPresentationFactory, ClChannelManager *poCManager = NULL)
{
	m_pPresentationFactory = ai_pPresentationFactory;

	m_poChannelManager = poCManager;
	m_pmChannel_map = new tMap;

	CreateChannel();
	//tengo hecha la creacion y Build del canal PRESENTATION_FACTORY
}


m4return_t CInternalPresentationFactory::CreateChannel()
{
	m4int_t ret = m_poChannelManager->CreateChannel(m_poChannel);

	if ((ret == M4_ERROR) || (m_poChannel == 0))
	{
		m_poChannel = 0;
		return M4_ERROR;
	}

	m4VariantType cs_type;
	cs_type.Type = M4CL_CPP_TYPE_NUMBER;
	cs_type.Data.DoubleData = M4CL_CSTYPE_BOTH;

	m_poChannel->CsType.Set(cs_type);	
	ret = m_poChannel->BuildFromId("PRESENTATION_FACTORY", NULL);	
	if (ret == M4_ERROR)
	{
		m_poChannel->Destroy();
		m_poChannel = 0;
		return ret;
	}

	return ret;
}

CInternalPresentationFactory::~CInternalPresentationFactory()
{
	
	Destroy_map();
	delete m_pmChannel_map;

	if (m_poChannel)
	{
		m_poChannel->Destroy();
		m_poChannel = 0;
	}

	m_pPresentationFactory = NULL;
}

m4uint32_t CInternalPresentationFactory::know_length(m4uchar_t *asn)
{
	m4uchar_t l_total;
	m4uint32_t aux=0,total;
	m4int_t i;

	if (memcmp("",asn,1) == 0)
	{
		return 0;
	}

	l_total = asn[1];
	total = l_total;
	if (l_total>127)
	{
		l_total = l_total - 128;

		for(i=1;i<=l_total;i++)
		{
			aux += (m4int32_t)asn[1+i]*(m4int32_t)pow((m4double_t)256,(m4double_t)(l_total-i));
		}
		total = l_total + aux;
	}
	return (total+1+1); // sumo TAG y byte donde se indica longitud
}

m4bool_t CInternalPresentationFactory::Decode(m4pchar_t ai_pcPresentation, m4puchar_t& ao_rpBuffer, m4uint32_t& ao_riLength)
{
	if( *ai_pcPresentation == '0' )
	{
		// Codificado en binario
		ao_riLength = know_length( (m4uchar_t*) ai_pcPresentation );

		if( ao_riLength != 0 )
		{
			ao_rpBuffer = new m4uchar_t[ ao_riLength ];
			memcpy( ao_rpBuffer, ai_pcPresentation, ao_riLength );
		}
		return M4_FALSE;
	}

	if( *ai_pcPresentation == 'M' )
	{
		// codificado en Base64
		long iLength;
		m4pchar_t pOutput;
		C_Base64 Base64;

		Base64.Decode( ai_pcPresentation, pOutput, &iLength );

		ao_rpBuffer = (m4puchar_t)pOutput;
		ao_riLength = iLength;
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4return_t CInternalPresentationFactory::GetPresentation(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation)
{
	m4VariantType vAutoLoad;
	vAutoLoad.Type = M4CL_CPP_TYPE_NUMBER;
	vAutoLoad.Data.DoubleData = 0;
   	m_poChannel->WantsAutoLoad.Set( vAutoLoad );

	m4return_t iRet = _GetPresentation( ai_ClPresentationAttributes, ai_bDefinition, ao_pBasicPresentation );
	return iRet;
}

m4return_t CInternalPresentationFactory::_GetPresentation(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation)
{
	m4return_t iRet = M4_ERROR;

	VcInheritCBufferASN vInheritCBufferASN;
	CBufferASN * pBasicPresentation = NULL;
	m4date_t dVersionMax = M4CL_MINUS_INFINITE_IN_JULIAN;
	m4uint16_t iStyle = 0;

	if( ao_pBasicPresentation != NULL )
	{
		return M4_ERROR;
	}

	CHECK_CHLOG_ERRORF( m_poChannel == NULL, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	ClAccess	*poAccess = m_poChannel->CreateAccess() ;
	CHECK_CHLOG_ERRORF( poAccess == NULL, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	// Se carga la presentación invocando el método LOAD_DATA
	ClNode	*poNode = &( poAccess->Node[ "DATA" ] ) ;
	CHECK_CHLOG_ERRORF( poNode == NULL, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	ClItem	*poItem = &poNode->RecordSet.Current.Item[ "LOAD_DATA" ] ;
	CHECK_CHLOG_ERRORF( poItem->GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	m4VariantType	vValue ;

	vValue.SetStringRef( (m4pchar_t) ai_ClPresentationAttributes.m_pIdPresentation );
	iRet = poItem->StackPush( vValue ) ;
	CHECK_CHLOG_ERRORF( iRet != M4_SUCCESS, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	
	vValue.SetDouble( ai_ClPresentationAttributes.m_iLanguage );
	if( ai_bDefinition == M4_TRUE )
	{
		vValue.SetDouble( -1 );
	}
	iRet = poItem->StackPush( vValue ) ;
	CHECK_CHLOG_ERRORF( iRet != M4_SUCCESS, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	
	m4return_t iReturn = poItem->Call( NULL, 2 ) ;

	// El resultado es el id de la presentación. De momento no se usa
	iRet = poItem->StackPop( vValue ) ;
	CHECK_CHLOG_ERRORF( iRet != M4_SUCCESS, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	CHECK_CHLOG_ERRORF( iReturn != M4_SUCCESS, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	m4pcchar_t pcLanguage = vValue.Data.PointerChar;

	ClNode	*poLanguagesNode = &( poAccess->Node[ "LANGUAGES" ] ) ;
	CHECK_CHLOG_ERRORF( poLanguagesNode == NULL, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;

	m4pcchar_t pcDefaultLanguage = (m4char_t *)poLanguagesNode->RecordSet.Item["DEFAULT_LANGUAGE"].Value.Get();

	m4uint32_t references = 0;
	if( ai_bDefinition == M4_TRUE )
	{
		// Se lee el número de lenguajes
		references = poLanguagesNode->RecordSet.Count();
	}

	ClNode	*poDynNode = &( poAccess->Node[ "DYN_DATA" ] ) ;
	CHECK_CHLOG_ERRORF( poDynNode == NULL, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;


	m4uint32_t iNumPresentations = poNode->RecordSet.Count();

	if( iNumPresentations > 0 )
	{
		iRet = M4_SUCCESS;
		for( m4uint32_t i = 0; i < iNumPresentations && iRet == M4_SUCCESS; i++ )
		{
			//El nodo está ordenado de forma que el primer registro es el mayor ascendente (root).
			//Es decir, está ordenado descendentemente por nivel (registro 0:abuela, 1:padre, 2:hija).
			poNode->RecordSet.Current.MoveTo(i);

			pBasicPresentation = new CBufferASN( references );
			vInheritCBufferASN.push_back( pBasicPresentation );			

			//Obtengo la version.
			m4VariantType vVersion;
			if( poNode->RecordSet.Current.Item["DT_MD_VERSION"].Value.Get(vVersion) == M4_SUCCESS && vVersion.Type == M4CL_CPP_TYPE_DATE )
			{
				pBasicPresentation->version = vVersion.Data.DoubleData;
			}
			else
			{
				// Si no tiene verisón se pone más infinito
				pBasicPresentation->version = M4CL_PLUS_INFINITE_IN_JULIAN;
			}

			if( pBasicPresentation->version >= dVersionMax )
			{
				dVersionMax = pBasicPresentation->version;
			}

			//Obtengo el estilo
			m4VariantType vStyle;
			if( poNode->RecordSet.Current.Item["PRESENTATION_STYLE"].Value.Get(vStyle) == M4_SUCCESS && vStyle.Type == M4CL_CPP_TYPE_NUMBER )
			{
				pBasicPresentation->style = (m4uint16_t)vStyle.Data.DoubleData;
			}
			iStyle = pBasicPresentation->style ;

			// obtengo la presentacion y la referencia en formato Base64 o binario
			m4pchar_t pDBPresentation = (m4char_t *)poNode->RecordSet.Current.Item["PACKAGE"].Value.Get();

			if( pDBPresentation != NULL && *pDBPresentation != '\0' )
			{
				m4bool_t bEnd = Decode( pDBPresentation, pBasicPresentation->asn, pBasicPresentation->length ) ;

				if( bEnd == M4_FALSE )
				{
					// Información de multiidioma
					if( ai_bDefinition == M4_FALSE )
					{
						pDBPresentation = NULL;

						// Recogemos el Item apropiado en función del idioma que nos indican
						if( poDynNode->RecordSet.Current.Begin() == M4_TRUE )
						{
							// Primero se busca como dinámico
							pDBPresentation = (m4char_t *)poDynNode->RecordSet.Current.Item["PKG_LNG"].Value.Get();
						}

						if( pDBPresentation == NULL || *pDBPresentation == '\0' )
						{
							// Después en la columna que se devuelve
							poItem = &poNode->RecordSet.Current.Item[pcLanguage];

							if( poItem->GetIndex() != M4DM_EOF_INDEX )
							{
								pDBPresentation = (m4char_t *)poItem->Value.Get();
							}
						}

						if( pDBPresentation == NULL || *pDBPresentation == '\0' )
						{
							if( strcmp( pcDefaultLanguage, pcLanguage ) != 0 )
							{
								// Finalmente en la de por defecto
								poItem = &poNode->RecordSet.Current.Item[pcDefaultLanguage];

								if( poItem->GetIndex() != M4DM_EOF_INDEX )
								{
									pDBPresentation = (m4char_t *)poItem->Value.Get();
								}
							}
						}

						if( pDBPresentation != NULL && *pDBPresentation != '\0' )
						{
							bEnd = Decode( pDBPresentation, pBasicPresentation->ref, pBasicPresentation->lengthRef ) ;
						}
						else
						{
							bEnd = M4_TRUE;
						}
					}
					else
					{
						for( m4uint32_t j = 0; j < pBasicPresentation->references && bEnd == M4_FALSE; j++ )
						{
							poLanguagesNode->RecordSet.Current.MoveTo( j );

							pcLanguage = (m4char_t *)poLanguagesNode->RecordSet.Current.Item["LANG_COLUMN"].Value.Get();
							pBasicPresentation->ppcCodes[j] = (m4char_t *)poLanguagesNode->RecordSet.Current.Item["LANG_CODE"].Value.Get();
							pDBPresentation = NULL ;

							m4VariantType vDynamic;
							iReturn = poLanguagesNode->RecordSet.Current.Item["IS_DYNAMIC"].Value.Get(vDynamic);

							if( iReturn == M4_SUCCESS && vDynamic.Type == M4CL_CPP_TYPE_NUMBER && vDynamic.Data.DoubleData == 1 )
							{
								// Es dinámico
								m4int32_t	iRegister = -1;
								m4VariantType avParam[ 3 ] ;

								avParam[ 0 ].SetStringRef( "ID_LANGUAGE" );
								iReturn = poLanguagesNode->RecordSet.Current.Item["ID_LANGUAGE"].Value.Get( avParam[1] );
								avParam[ 2 ].SetDouble( M4CL_EQUAL_FUNCTION );

								iReturn = poDynNode->RecordSet.Register.FindSec( avParam, 3, &iRegister ) ;

								if( iReturn == M4_SUCCESS && iRegister != -1 )
								{
									pDBPresentation = (m4char_t *)poDynNode->RecordSet.Register[iRegister].Item["PKG_LNG"].Value.Get();
								}
							}
							else
							{
								// Es estático
								poItem = &poNode->RecordSet.Current.Item[pcLanguage];

								if( poItem->GetIndex() != M4DM_EOF_INDEX )
								{
									pDBPresentation = (m4char_t *)poItem->Value.Get();
								}
							}

							if( pDBPresentation == NULL || *pDBPresentation == '\0' )
							{
								if( strcmp( pcDefaultLanguage, pcLanguage ) != 0 )
								{
									// Finalmente en la de por defecto
									poItem = &poNode->RecordSet.Current.Item[pcDefaultLanguage];

									if( poItem->GetIndex() != M4DM_EOF_INDEX )
									{
										pDBPresentation = (m4char_t *)poItem->Value.Get();
									}
								}
							}

							if( pDBPresentation != NULL && *pDBPresentation != '\0' )
							{
								bEnd = Decode( pDBPresentation, pBasicPresentation->ppcRefs[j], pBasicPresentation->ppiLengthRefs[j] ) ;
							}
							else
							{
								bEnd = M4_TRUE;
							}
						}
					}
				}

				if( bEnd == M4_TRUE )
				{
					iRet = M4_ERROR;
				}
			}
			else
			{
				iRet = M4_ERROR;
			}
		}
	}
	else
	{
		ao_pBasicPresentation = new CBufferASN( references );
		iRet = M4_NOTFOUND;
	}

	// Borrar los datos y destruir el acceso al canal
	poNode->RecordSet.Destroy();
	poAccess->Destroy();

	//--------------------------------------------------------------------------------------------------------------
	//Herencia de presentaciones.
	//Vamos a tratar la herencia, mezclando arbolillos.
	//Cuando el arbolillo está mezclado le damos el cambiazo, como si fuera la presentación que realmente se ha leido de BD.
	//El canal MD_VERSION se encargará de invalidar una presentacion si cualquiera de sus padres ha cambiado.
	
	//Antes de tratar cualquier include hay que resolver la herencia (TODA la cadena de herencia).
	//Se empieza por una presentacion que cargará todos los ascendientes y fusionando descendientemente (Padre a hijo)!.	

	pBasicPresentation = NULL;

	if (iRet == M4_SUCCESS)
	{
		m4int32_t iNumInheritPresentations = vInheritCBufferASN.size();
		//Si hay mas de uno es pq hay herencia.
		if (iNumInheritPresentations > 1)
		{
			//Convertirmos en arbolitos.
			VcInheritCAsn vInheritCAsn;
			iRet = _CreateCAsnInheritancePresentationChain( vInheritCBufferASN, ai_bDefinition, vInheritCAsn );
			if (iRet != M4_ERROR) {
				//Se han convertido arbolitos, ahora empieza la fusion. La fusion elimina arbolitos innecesarios.
				//Obtendremos un buffer que es el merge de todos los arbolitos.
				CBufferASN * pMergedBufferASN = NULL;
				iRet = _AnalizeInheritancePresentationChain( vInheritCAsn, ai_bDefinition, pMergedBufferASN );
								
				if ((iRet != M4_ERROR) && (pMergedBufferASN != NULL)) {
					//El analisis de herencia nos da un buffer con la mezcla de la herencia, pero
					//tenemos que poner la version maxima!.
					pBasicPresentation = pMergedBufferASN;
					pBasicPresentation->version = dVersionMax;
					pBasicPresentation->style = iStyle;
					ao_pBasicPresentation = pBasicPresentation;									
				}
			}
		}
		else
		{
			//En realidad el único valor posible es 1 (menor de 1 no es posible).
			if (iNumInheritPresentations == 1)
			{
				//Hay una unica presentacion. Nos la quedamos y la quitamos del vector.
				pBasicPresentation = vInheritCBufferASN.back();
				vInheritCBufferASN.pop_back();
				ao_pBasicPresentation = pBasicPresentation;
			}
			else
			{
				iRet = M4_ERROR;
			}
		}
	}

	//La informacion la tenemos en arbolitos, no necesitamos el vector de presentaciones.
	CInternalPresentationFactory::RemoveInListAllCBufferASN( vInheritCBufferASN, M4_TRUE );

	//--------------------------------------------------------------------------------------------------------------
	
	return iRet;
}


ClChannel * CInternalPresentationFactory::FindChannel(m4pcchar_t name_channel, const ClPresentationAttributes & ai_ClPresentationAttributes, m4return_t *ret)
{
	ClChannel *ptoChannel;
	
	*ret = M4_SUCCESS;
	m4VariantType vValue;


	if (!m_pmChannel_map->empty() && m_pmChannel_map->find(name_channel) != m_pmChannel_map->end())
	{
		return (*m_pmChannel_map)[name_channel];
	}

	m_poChannelManager->CreateChannel(ptoChannel);
	
	// Se asigna el cstype
	vValue.Type = M4CL_CPP_TYPE_NUMBER;
	vValue.Data.DoubleData=M4CL_CSTYPE_FRONT;
	ptoChannel->CsType.Set(vValue);

	// Se asigna la sociedad
	if( ai_ClPresentationAttributes.m_pOrganization != NULL )
	{
		vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vValue.Data.PointerChar = (m4pchar_t) ai_ClPresentationAttributes.m_pOrganization ;

		ptoChannel->Organization.Set( vValue ) ;
	}

	// Se asigna el role
	if( ai_ClPresentationAttributes.m_pRole != NULL )
	{
		vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vValue.Data.PointerChar = (m4pchar_t) ai_ClPresentationAttributes.m_pRole ;

		ptoChannel->Role_ID.Set( vValue ) ;
	}

	*ret = ptoChannel->BuildFromId(name_channel, NULL, ai_ClPresentationAttributes.m_iLanguage);

	if (*ret == M4_SUCCESS)
	{
		(*m_pmChannel_map)[name_channel] = ptoChannel;
	}
	else
	{
		ptoChannel->Destroy();
		ptoChannel = 0;
	}

	return ptoChannel;
}

void CInternalPresentationFactory::Destroy_map()
{
	ClChannel *ptoChannel;

	tMap::iterator p = m_pmChannel_map->begin();
	for (; p!=m_pmChannel_map->end(); ++p)
	{
		ptoChannel = (*p).second;
		ptoChannel->Destroy();
	}
	m_pmChannel_map->clear();
}

m4int_t CInternalPresentationFactory::AskSegurityItem(ClCompiledMCR *poCompiled,m4char_t *name_item,m4char_t *name_node)
{
	m4uint32_t	m_NodeHandle,m_ItemHandle;
	m4int_t		r,u,e;
	m4int_t		mask=0;

	m_NodeHandle = poCompiled->GetChannelNodeHandleById(name_node);
	if (m_NodeHandle == 0)  // el nodo no pertenece a ese canal
	{
		mask = 0;
	}
	else
	{	
		m_ItemHandle = poCompiled->GetNodeItemHandleById(m_NodeHandle,name_item);
		if (m_ItemHandle == 0)  // el nodo no pertenece a ese canal
		{
			mask = 0;
		}
		else
		{
			r = poCompiled->GetItemSecRead(m_ItemHandle);
			mask += r;

			u = poCompiled->GetItemSecUpdate(m_ItemHandle);
			mask += u*4;
			mask += u*2;

			e = poCompiled->GetItemSecExecute(m_ItemHandle);
			mask += e*16;
		}
	}
	return mask;
}

m4int_t CInternalPresentationFactory::AskSegurityNode(ClCompiledMCR *poCompiled,m4char_t *name_node)
{
	
	m4uint32_t	m_NodeHandle;
	m4int_t		w,d;
	m4int_t		mask=0;

	m_NodeHandle = poCompiled->GetChannelNodeHandleById(name_node);
	if (m_NodeHandle == 0)  // el nodo no pertenece a ese canal
	{
		mask = 0;
	}
	else
	{
  		w = poCompiled->GetNodeSecWrite(m_NodeHandle);
		mask += w*2;

		d = poCompiled->GetNodeSecDelete(m_NodeHandle);
		mask += d*8;
	}

	// La mascara queda de la siguiente manera:
	//		1 Delete 1 Write 1
	return (mask+21);
}



m4return_t CInternalPresentationFactory::AskSegurity_Indirection(CComponent *seq, CAccessSeq *anterior,ClCompiledMCR *poCompiled, const ClPresentationAttributes & ai_ClPresentationAttributes, CBufferASN *infPresent)
{
	m4uint32_t				i;
	m4int_t				mask=0;
	ClChannel			*ptoChannel;
    m4char_t            *name_node = NULL;
	m4return_t			res;

	if ((seq->m_access)->lengthChannel != 0)   
	{	
		m4char_t *m_channel = new m4char_t[((seq->m_access)->lengthChannel)+1];
		memcpy(m_channel,(seq->m_access)->channel,(seq->m_access)->lengthChannel);
		m_channel[(seq->m_access)->lengthChannel] = '\0';

		ptoChannel = FindChannel(m_channel, ai_ClPresentationAttributes, &res);

		if (res == M4_ERROR)
		{
			return M4_ERROR;
		}

		if (ptoChannel != NULL)
		{
			poCompiled = (ptoChannel->GetpChannelDef())->GetpCMCR();
		}

		if (poCompiled!=NULL)
		{	
			m4int_t posChannel = 0;

			if (ptoChannel != NULL)
			{
				posChannel = infPresent->ExistVersionOfT3(m_channel);
				if (posChannel > -1)
				{
					infPresent->listVersionT3[posChannel] = poCompiled->GetChannelVersion();
				}
			}

			if (((seq->m_access)->item != NULL)&&((seq->m_access)->node != NULL))
			{
				m4char_t *name_item = new m4char_t[((seq->m_access)->lengthItem)+1];
				memcpy(name_item,(seq->m_access)->item,(seq->m_access)->lengthItem);
				name_item[(seq->m_access)->lengthItem] = '\0';

				name_node = new m4char_t[((seq->m_access)->lengthNode)+1];
				memcpy(name_node,(seq->m_access)->node,(seq->m_access)->lengthNode);
				name_node[(seq->m_access)->lengthNode] = '\0';

				mask = AskSegurityItem(poCompiled,name_item,name_node);
				if (name_item!=NULL) delete name_item;
			}
			else
			{
				if ((seq->m_access)->node != NULL)
				{
					name_node = new m4char_t[((seq->m_access)->lengthNode)+1];
					memcpy(name_node,(seq->m_access)->node,(seq->m_access)->lengthNode);
					name_node[(seq->m_access)->lengthNode] = '\0';
					mask = AskSegurityNode(poCompiled,name_node);
				}
				else
				{
					mask = 31;  // todo es NULL y esntonces Grants tiene todos los permisos
				}
			}
		}
		else
		{
			mask = 31; // todos los permisos por defecto
		}

		if (m_channel!=NULL) delete m_channel;
	}
	else
	{
		mask = 31;// todos los permisos por defecto
	}

	(seq->m_access)->grant &= mask;
	
	i = 0;
	while (i<(seq->propBag)->GetNumComponent())
	{
		CComponent *comp;
		comp = (seq->propBag)->View(i);
		if (comp->GetTypeProp() == ASNSequence)
		{
			AskSegurity_Indirection(comp,seq->m_access, poCompiled, ai_ClPresentationAttributes, infPresent);
		}
		else
		// analizar indirecciones
		{
			if (comp->GetTypeProp() == ASNGeneralString)
			{
				m4uint32_t m_lengthString;

				m_lengthString = comp->GetLengthProp();
				m4char_t *nombre = new m4char_t[m_lengthString + 1];
				comp->GetStringValueProp(nombre);

				m4uint32_t longi = comp->GetLengthProp();

				m4uint32_t sizeResult = longi * 5;		// Valor que había

				if( sizeResult < 2048 )
				{
					sizeResult = 2048;					// En unicode puede que se necesite más
				}

				m4char_t *result = new m4char_t[sizeResult + 1];

				m4uint32_t lengthResult = 0;
				
				if (TranslateIndirection(nombre,result,poCompiled,comp,longi,name_node,&lengthResult,M4_FALSE,this, ai_ClPresentationAttributes) == M4_TRUE)
				{
					comp->length = 1+1+comp->GetLengthNameProp()+1; //tag str del nomb de la prop
																	//tag str del valor de la prop
					comp->SetStringValueProp(result,lengthResult);

					m4uchar_t n_bytes;
					m4uint32_t aux;
					m4int_t i;

					i=0;
					if (lengthResult >127)
					{
						aux = lengthResult;
						while (aux != 0)
						{
							i++;
							aux >>= 8;
						}
						n_bytes = i;
					}
					else
					{
						n_bytes = 0;
					}
					comp->length += 1+n_bytes; //añado bytes para especificar longitud de la cadena
					comp->length_prop = lengthResult;
				}

				if (result!=NULL) delete result;
				if (nombre!= NULL) delete nombre;

			} // end GeneralSTRING
		} // endELSE  
		i++;
	}

	if (name_node!= NULL) delete name_node;
	return M4_SUCCESS;

}

m4return_t CInternalPresentationFactory::TranslateIndirection(m4char_t *strIni, m4char_t *strFin,ClCompiledMCR *poCompiled, CComponent *comp, m4uint32_t length,m4char_t *name_node, m4uint32_t *lenTotal, m4bool_t copy, CInternalPresentationFactory *aux, const ClPresentationAttributes & ai_ClPresentationAttributes)
{
	m4uint32_t lenItem = 0,lenNode, lenChannel, indice = 0;
	m4int_t indirection = -1, res;
	m4uint32_t beginIndir = 0;
	m4char_t *strFinIni;
	m4char_t *item=NULL;
	m4char_t *node=NULL;
	m4char_t *channel=NULL;
	m4return_t ret = M4_TRUE;

	strFinIni = strFin;

	indirection = comp->analiceIndirection(strIni,&indice,length, &beginIndir);
		
	if (indirection != -1)
	{
		memcpy(strFin,strIni,beginIndir);
		strFin += beginIndir;
		(*lenTotal) +=beginIndir;
	}
	else
	{
		if ((indirection == -1) && (copy))
		{
			memcpy(strFin,strIni,indice);
			(*lenTotal) += indice;
		}
		else
		{
			ret= M4_FALSE;
		}
	}

	switch(indirection)
	{
	case 0: // ITEM
		res = comp->FindIndirectionItem(m_poChannelManager, poCompiled, strIni, item, &indice, &lenItem, name_node, length);
		switch (res)
		{
		case 0 :memcpy(strFin,item,lenItem);
				strFin += lenItem;				
				(*lenTotal) += lenItem;
				indice++;  // eliminar ]
				if (item != NULL) delete item;
				break;
		case 1: memcpy(strFin,"ITEM NOT DEFINED",16);
				strFin += 16;
				(*lenTotal) += 16;
				indice++;  // eliminar ]
				break;
		//default: //cerr<<"No existe correspondencia"<<endl;
		}
	break;

	case 1: //NODE
		res = comp->FindIndirectionNode(m_poChannelManager, poCompiled, strIni, node, &indice, &lenNode, length);
		switch (res)
		{
		case 0 :memcpy(strFin,node,lenNode);
				strFin += lenNode;	
				(*lenTotal) += lenNode;
				indice++;  // eliminar ]
				if (node != NULL) delete node;
				break;
		case 1: memcpy(strFin,"NODE NOT DEFINED",16);
				strFin += 16;
				(*lenTotal) += 16;
				indice++;  // eliminar ]
				break;
	//	default: //cerr<<"No existe correspondencia"<<endl;
		}
	break;

	case 2: //CHANNEL
		res = comp->FindIndirectionChannel(m_poChannelManager, poCompiled, strIni, channel, &indice, &lenChannel, length, aux, m_pPresentationFactory, ai_ClPresentationAttributes);
		switch (res)
		{
		case 0 :memcpy(strFin,channel,lenChannel);
				strFin += lenChannel;
				(*lenTotal) += lenChannel;
				indice++;  // eliminar ]
				if (channel != NULL) delete channel;
				break;
		case 1: // la indireccion se corresponde con NULO
				memcpy(strFin,"CHANNEL NOT DEFINED",19);
				strFin += 19;
				(*lenTotal) += 19;
				indice++;  // eliminar ]
				break;
		//default: //cerr<<"No existe correspondencia"<<endl;
		}
		break;
	} // endSWITCH
			
	
	if (ret != M4_FALSE)
	{
		if (length>indice)
		{
			m4char_t *masIndirec = strIni + indice;
			TranslateIndirection(masIndirec,strFin,poCompiled,comp,length-indice,name_node,lenTotal,M4_TRUE,aux, ai_ClPresentationAttributes);
			ret= M4_TRUE;
		}
		else
		{
			ret= M4_TRUE;
		}
	}
	strFin = strFinIni;
	return ret;
}

m4return_t CInternalPresentationFactory::BuildBasicPresentationFromId( const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation )
{	
	m4return_t iRet = M4_SUCCESS;
	m4int_t i = 0;

	//Es un argumento es de salida y vamos a obligar que el fenomeno programador lo inicialice a nulo. 
	if (ao_pBasicPresentation != NULL)
	{
		return M4_ERROR;
	}

	if (m_poChannel == NULL)
	{
		return M4_ERROR;
	}

	//Miramos cuanto heap tenemos.
	//***** CComponent::DumpMemoryInfo(ai_ClPresentationAttributes.m_pIdPresentation, M4_TRUE);

	// Obtener codigo ASN1 de la presentacion del canal PRESENTATION_FACTORY
	iRet = GetPresentation( ai_ClPresentationAttributes, ai_bDefinition, ao_pBasicPresentation );
	if (ao_pBasicPresentation == NULL)
	{
		return M4_ERROR;
	}

	//Vemos la diferencia de heap.
	//***** CComponent::DumpMemoryInfo(ai_ClPresentationAttributes.m_pIdPresentation, M4_FALSE);

	if (iRet == M4_NOTFOUND) 
	{	
		// Reseteamos la presentación básica
		ao_pBasicPresentation->asn = new m4uchar_t[M4PRESENTATION_NOT_FOUND_SIZE];
		memcpy(ao_pBasicPresentation->asn, M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE);
		ao_pBasicPresentation->ref = new m4uchar_t[M4PRESENTATION_NOT_FOUND_SIZE];
		memcpy(ao_pBasicPresentation->ref, M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE);
		ao_pBasicPresentation->length = M4PRESENTATION_NOT_FOUND_SIZE;
		ao_pBasicPresentation->lengthRef = M4PRESENTATION_NOT_FOUND_SIZE;
		// [design time]
		for (int i=0; i<ao_pBasicPresentation->references; i++) 
		{
			ao_pBasicPresentation->ppcRefs[i] = new m4uchar_t[M4PRESENTATION_NOT_FOUND_SIZE];
			memcpy( ao_pBasicPresentation->ppcRefs[i], M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE );				
			ao_pBasicPresentation->ppiLengthRefs[i] = M4PRESENTATION_NOT_FOUND_SIZE;
		}

		iRet = M4_SUCCESS;
	}

    return iRet;
	// IMP: es necesario liberar el BasicPresentation fuera
}

m4bool_t CInternalPresentationFactory::_IsPresentationRoot( CAsn * ai_pCAsn )
{
	m4bool_t bIsPresentationRoot = M4_TRUE;
	CComponent * pPresent = NULL;
	CListComponent	* pPropBag = NULL;
	if ((ai_pCAsn != NULL)
		&& ((pPresent = ai_pCAsn->present) != NULL)
		&& ((pPropBag = pPresent->propBag) != NULL) )
	{
		//De ser presentacion, la primera componente me indica que es una presentacion.
		CComponent * pView = pPropBag->View(0);
		if (pView != NULL) {
			//Comprobamos que es una presentacion. Hay muchos tipos de presentacion, devolvemos falso
			//si se trata de una presentacion tipo include!.
			if (pView->GetTypeProp() == ASNGeneralString)
			{
				if ((pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE)
					&& ((pView->IsValueProp(M4PRESENTATION_INCLUDE_TYPE_PRESENTATION_INCLUDE) == M4_TRUE)
						||(pView->IsValueProp(M4PRESENTATION_INCLUDE_TYPE_PRESENTATION_INCLUDE_EVENT) == M4_TRUE)) )
				{
					bIsPresentationRoot = M4_FALSE;
				}
			}		
		}
	}

	return bIsPresentationRoot;
}

//-----------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------

m4return_t CInternalPresentationFactory::_AnalizeIncludesRoot( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsnParent, CAsn * ai_pCAsnIncludeToAnalize, const string & ai_sIdInclude, const ClPresentationAttributes & ai_ClPresentationAttributes )
{
	//Es la primera llamada, necesitamos la componente que es root del arbol.
	
	//Ejemplo:
	//BEGIN Presentation Pres2
		//Idchannel = "ZZ_PEDRO_PRESENTATION"
		//Idnode = "ZZ_PEDRO_PRESENTATION"
		//BEGIN Action_dataprops StepAutoLoadWorkDates
		//	Autoload = 0
		//END
		//BEGIN Action_preload StepPreload
		//	Cancelif = "Notpreload"
		//	Availablesteps = 31
		//END
		//BEGIN Form FormMain
		//	Left = 17
		//	Top = 17
		//	BEGIN Menubar mnubMain1
		//	...
		//	END
		//	BEGIN Menubar mnubMain2
		//	...
		//	END
		//END
	//END

	//En el ejemplo anterior, tenemos 7 componentes (con el nombre de la propiedad y su valor)!.
	//0:Classname	-	Presentation
	//1:Alias		-	Pres2
	//2:IdChannel	-	ZZ_PEDRO_PRESENTATION
	//3:IdNode		-	ZZ_PEDRO_PRESENTATION
	//4:Sequence	-	Corresponde al subarbol de Action_dataprops (Tendra 3 componentes).
	//5:Sequence	-	Corresponde al subarbol de Action_preload (Tendra 4 componentes).
	//6:Sequence	-	Corresponde al subarbol de Form (Tendra 6 componentes, dos son a su vez sequence).

	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------
	//Recorremos el arbol de componentes en busca de includes.
	CComponent * pPresent = NULL;
	CListComponent	* pPropBag = NULL;
	if ((ai_pCAsnIncludeToAnalize != NULL)
		&& ((pPresent = ai_pCAsnIncludeToAnalize->present) != NULL)
		&& ((pPropBag = pPresent->propBag) != NULL) )
	{
		//Ojito pq en el caso inicial el padre (ai_pCAsnParent) es NULL.
		//ai_pCAsnIncludeToAnalize es el asn que se va a analizar (cuyo id include es ai_sIdInclude). 

		//-------------------------------------------------------------
		//Para controlar una recursión infinita de includes: A->B->C->B... (sería infinito pq C<->B)	
		if (ai_pCBufferASNRoot != NULL) {
			iRet = ai_pCBufferASNRoot->CheckInfiniteRecursionPush( ai_sIdInclude );
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
		}
		//-------------------------------------------------------------

		//Recorremos las componentes.
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);
			iRet = _AnalizeIncludes( ai_pCBufferASNRoot, ai_pCAsnIncludeToAnalize, pView, ai_ClPresentationAttributes );
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}			
		}
	}
	//-----------------------------------------------------------------------------

	//-------------------------------------------------------------
	//Cuando se haya analizado un subarbol quitamos de la lista!.
	if (ai_pCBufferASNRoot != NULL) {
		iRet = ai_pCBufferASNRoot->CheckInfiniteRecursionPop( );
		if (iRet == M4_ERROR) {
			return M4_ERROR;
		}
	}
	//-------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_AnalizeIncludes( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsn, CComponent * ai_pCComponent, const ClPresentationAttributes & ai_ClPresentationAttributes )
{
	//-----------------------------------------
	//Ejemplo:
	//BEGIN Presentation Pres2
		//Idchannel = "ZZ_PEDRO_PRESENTATION"
		//Idnode = "ZZ_PEDRO_PRESENTATION"
		//BEGIN Includecall xxx
		//	Idinclude = "PresInclude1"
		//	BEGIN Argument Arg1
		//		Value = "Valorxx"
		//	END
		//	BEGIN Argument Arg2
		//		Value = "PresInclude2"
		//	END
		//END
	//END
	//-----------------------------------------

	//-----------------------------------------
	//La presentacion (PresInclude1) seria:
	//BEGIN Includepanel xxxx
		//Cadena = "##ARG[Arg1]"
		//BEGIN Includecall xxx
		//	Idinclude = "##ARG[Arg2]"
		//	BEGIN Argument Arg1
		//		Value = "##ARG[Arg1]"
		//	END
		//	BEGIN Argument Arg2
		//		Value = "Valorzz"
		//	END
		//	BEGIN Argument Arg3
		//		Value = 5
		//	END
		//END
	//END
	//-----------------------------------------

	//-----------------------------------------
	//La presentacion (PresInclude2) seria:
	//BEGIN Includepanel zzzz
		//Height = "##ARG[Arg3]"
	//END
	//-----------------------------------------

	//-----------------------------------------
	//Importante:
	//Vamos a permitir que los identificadores de includes puedan ser argumentos:
	//BEGIN Includepanel ##ARG[Arg4]
		//Height = "##ARG[Arg3]"
	//END
	//-----------------------------------------

	//Buscamos todos los includes que tiene una presentacion. El que primero que invoca le pasa el root del arbol.

	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------
	//Recorremos el arbol de componentes en busca de includes.
	CListComponent	* pPropBag = NULL;
	if ((ai_pCComponent != NULL)
		&& ((pPropBag = ai_pCComponent->propBag) != NULL))
	{
		//Recorremos las componentes.		
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);			
			if (pView->GetTypeProp() == ASNGeneralString) {

				if ((pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE)
					&& (pView->IsValueProp(M4PRESENTATION_INCLUDE_TYPE_INCLUDECALL) == M4_TRUE) )
				{
					//Estamos haciendo una llamada a un include.
					
					//Ahora podremos extraer los argumentos que la presentacion padre
					//le quiere pasar a la presentacion Include.
					//Importante:
					//Pasamos la componente padre, pq alias y argumentos son componentes hermanas de pView!.
					//Esperamos una componente que sera la raiz del arbol del include.
					CComponent * pIncludeRootComponent = NULL;
					iRet = _AnalizeIncludeBlock( ai_pCBufferASNRoot, ai_pCAsn, ai_pCComponent, ai_ClPresentationAttributes, pIncludeRootComponent );

					//Cuando volvemos de analizar el include, necesitamos enlazar el root del include 
					//(presentacion include) con el que desencadeno la llamada al include (presentacion padre).
					if (iRet != M4_ERROR) {
						iRet = _MergeFatherAndChildIncludeRoot( ai_pCAsn, pIncludeRootComponent );

						//Importante.
						//El metodo _AnalizeIncludeBlock vuelve a recorrer el nodo en que estamos por lo tanto
						//despues del Merge debemos salir pq pueda haber poda del propBag.

						return iRet;
					}
					
				}

				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
			}
			else
			{
				//Si es una nodo con mas hijos debo comprobar los hijos.
				if (pView->GetTypeProp() == ASNSequence) {
					iRet = _AnalizeIncludes( ai_pCBufferASNRoot, ai_pCAsn, pView, ai_ClPresentationAttributes );
					if (iRet == M4_ERROR) {
						return M4_ERROR;
					}
				}				
			}
		}
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_MergeFatherAndChildIncludeRoot( CAsn * ai_pCAsn, CComponent * ai_pIncludeRootComponent )
{
	m4return_t iRet = M4_SUCCESS;

	//Vamos a buscar en el arbol el subarbol a podar y sustituir por el subarbol del include.

	//Se trata de una busqueda recursiva de la componente, la componente debe ser una SEQ.

	//Recorremos el arbol de componentes en busca de includes.
	CComponent * pPresent = NULL;
	CListComponent	* pPropBag = NULL;
	if ((ai_pCAsn != NULL)
		&& ((pPresent = ai_pCAsn->present) != NULL)
		&& ((pPropBag = pPresent->propBag) != NULL) )
	{
		//Recorremos las componentes.
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);			
			//Pasamos el padre, el iesimo hijo, la componente raiz del include y nos dira si se ha podado el subarbol
			//de la presentacion principal.
			m4bool_t bPruned = M4_FALSE;
			iRet = _MergeFatherAndChildInclude(pPresent, i, pView, ai_pIncludeRootComponent, bPruned);
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}	
			//Si el el subarbol se ha podado, no continuamos haciendo merge.
			if (bPruned == M4_TRUE) {
				return M4_SUCCESS;
			}
		}
	}

	return iRet;
}

m4return_t CInternalPresentationFactory::_MergeFatherAndChildInclude( CComponent * ai_pParentComponent, m4uint32_t ai_iChildPosition, CComponent * ai_pCComponent, CComponent * ai_pIncludeRootComponent, m4bool_t & ao_bPruned )
{
	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------
	//Recorremos el arbol de componentes en busca del subarbol a podar.
	CListComponent	* pPropBag = NULL;
	if ((ai_pCComponent != NULL)
		&& ((pPropBag = ai_pCComponent->propBag) != NULL))
	{
		//Continuamos en al busqueda por el arbol.
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);			
			//Si es una nodo con mas hijos debo comprobar los hijos.
			//------------------------------------------------------------------------
			if (pView->GetTypeProp() == ASNSequence) {
				iRet = _MergeFatherAndChildInclude( ai_pCComponent, i, pView, ai_pIncludeRootComponent, ao_bPruned );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//Si el el subarbol se ha podado, no continuamos haciendo merge.
				if (ao_bPruned == M4_TRUE) {
					return M4_SUCCESS;
				}
			}
			else {
				//----------------
				//Comprobamos si es la llamada al include pq tenemos que sustituirlo.
				if (pView->GetTypeProp() == ASNGeneralString) {

					if ((pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE)
						&& (pView->IsValueProp(M4PRESENTATION_INCLUDE_TYPE_INCLUDECALL) == M4_TRUE) )
					{
						//Esto hace finalizar la recursión. Hacemos la sustitución.

						//-----------------------------------------------------------------
						//Importante:
						//La operacion de Merge y de Free estan relacionadas.
						//Si se hace merge de un nodo
						//-----------------------------------------------------------------
						
						//En la posicion ai_iChildPosition de la lista de la componente ai_pParentComponent tenemos
						//un subarbol que debera sustituirse por elementos del subarbol apuntado por ai_pIncludeRootComponent. 							
						iRet = ai_pParentComponent->MergeChildIncludeByPos( ai_iChildPosition, ai_pIncludeRootComponent );
						if (iRet == M4_ERROR) {
							return M4_ERROR;
						}

						//-----------------------------------------------------------------
						//En este punto hemos pasado la parte de subarbol del include a la presentacion padre (la que
						//ha invocado al include).
						
						//Ademas, el include ya no tiene nodos pq se ha pasado a la principal.

						//El arbol del include pasa a pertenecer por MergeChildIncludeByPos a la presentacion
						//padre!. Pero es necesario quedarnos en la presentacion padre con los elementos 
						//que realmente queremos de la presentacion include, por eso hacemos un clear de subarboles.
						iRet = _ClearSubTreeComponentByRoot( ai_pIncludeRootComponent );
						if (iRet == M4_ERROR) {
							return M4_ERROR;
						}

						//Ahora nos cargamos el subarbol donde estaba la llamada al include. Pq en su sitio
						//hemos puesto el código del include.
						iRet = _FreeSubTreeComponentByRoot( ai_pCComponent );
						if (iRet == M4_ERROR) {
							return M4_ERROR;
						}
						//-----------------------------------------------------------------

						//Hemos conseguido podar el subarbol de la presentacion principal.
						ao_bPruned = M4_TRUE;
						return M4_SUCCESS;
					}
				}
				//----------------
			}
			//------------------------------------------------------------------------
		}	
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_AnalizeIncludeBlock( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsn, CComponent * ai_pCComponent, const ClPresentationAttributes & ai_ClPresentationAttributes, CComponent * & ao_pIncludeRootComponent )
{
	//Analiza el bloque que tiene el Includecall. Deberemos extraer los argumentos y
	//la presentacion que queremos incluir.

	m4return_t iRet = M4_SUCCESS;

	//Nos han pasado la componente padre donde se encuentra la llamada al include.
	//Vamos a extraer:
	//La presentacion include que deseamos abrir y los argumentos que le queremos pasar.

	//Este mapa lo poblara el analizador de argumentos.
	MpArguments mapArguments;

	//-----------------------------------------------------------------------------
	//Recorremos el arbol de componentes en busca de includes.
	CListComponent	* pPropBag = NULL;
	if ((ai_pCComponent != NULL)
		&& ((pPropBag = ai_pCComponent->propBag) != NULL))
	{
		m4char_t * pIdInclude = NULL;
		m4char_t * pIdAlias = NULL;		

		//Recorremos las componentes.		
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);			
			if (pView->GetTypeProp() == ASNGeneralString) {

				m4char_t * pValue = new m4char_t[pView->GetLengthProp() + 1];
				pView->GetStringValueProp(pValue);
				pValue[pView->GetLengthProp()] = NULL;

				if (pView->IsNameProp(M4PRESENTATION_INCLUDE_PROP_IDINCLUDE) == M4_TRUE)
				{
					if (pIdInclude != NULL) {
						delete [] pIdInclude;
						DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
						return M4_ERROR;
					}
					//En value tengo el Include que deseamos abrir.
					pIdInclude = new m4char_t [strlen(pValue) + 1];
					strcpy(pIdInclude, pValue);										
				}
				if (pView->IsNameProp(M4PRESENTATION_INCLUDE_PROP_INCLUDEALIAS) == M4_TRUE)				
				{
					//Tambien podremos especificar un alias para el include, de esta forma forzamos que tenga un
					//determinado identificador el include.
					if (pIdAlias != NULL) {
						delete [] pIdAlias;
						DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
						return M4_ERROR;
					}
					//En value tengo el Include que deseamos abrir.
					pIdAlias = new m4char_t [strlen(pValue) + 1];
					strcpy(pIdAlias, pValue);										
				}

				delete [] pValue;

			}
			else {
				//Puede ser que sea un nodo con argumentos!.
				if (pView->GetTypeProp() == ASNSequence) {					
					iRet = _AnalizeIncludeArgument( pView, mapArguments );
					if (iRet == M4_ERROR) {
						if (pIdInclude != NULL) delete [] pIdInclude;
						if (pIdAlias != NULL) delete [] pIdAlias;
						return M4_ERROR;
					}
				}				
			}
		}

		//Si estamos aqui es pq ya sabemos el include que deseamos y tambien tenemos los argumentos
		//que queremos pasarle al include.

		if (pIdInclude != NULL) {
			iRet = _AnalizeLoadIncludePresentation( ai_pCBufferASNRoot, pIdInclude, mapArguments, ai_pCAsn, ai_ClPresentationAttributes, ao_pIncludeRootComponent );
			if (iRet == M4_ERROR) {
				if (pIdInclude != NULL) {
					DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INCLUDE, ai_ClPresentationAttributes.m_pIdPresentation << pIdInclude ) ;
				}
				if (pIdInclude != NULL) delete [] pIdInclude;
				if (pIdAlias != NULL) delete [] pIdAlias;				
				return M4_ERROR;
			}

			//Si se especifico un alias es necesario renombrar el alias del root del include.
			if ((pIdAlias != NULL) && (ao_pIncludeRootComponent != NULL)) {
				iRet = _RenameAliasIncludeRoot( ao_pIncludeRootComponent, pIdAlias);
			}

			if (pIdInclude != NULL) delete [] pIdInclude;
			if (pIdAlias != NULL) delete [] pIdAlias;
		}
		else {
			//Tiene que haber un include a levantar.
			DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			return M4_ERROR;
		}
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_AnalizeIncludeArgument( CComponent * ai_pCComponent, MpArguments & ao_mArguments )
{
	//Analizamos la componente donde estan los argumentos (El valor puede ser de varios tipos):
	//BEGIN Argument xxx
	//	Value = "5"
	//	Value = 5 
	//	Value = "##ARG[Arg1]"
	//END

	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------	
	CListComponent	* pPropBag = NULL;
	if ((ai_pCComponent != NULL)
		&& ((pPropBag = ai_pCComponent->propBag) != NULL))
	{
		m4bool_t bArgumentSection = M4_FALSE;
		string sIdArgument("");
		//Recorremos las componentes.		
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);	
			m4uchar_t iTypeProp = pView->GetTypeProp();
			if ((iTypeProp == ASNBOOLEAN) 
				||(iTypeProp == ASNINTEGER)
				||(iTypeProp == ASNGeneralString))
			{
				//En valor solo me vale para saber si estoy en una seccion de argumentos.
				if (iTypeProp == ASNGeneralString) {
					m4char_t * pValue = new m4char_t[pView->GetLengthProp() + 1];
					pView->GetStringValueProp(pValue);
					pValue[pView->GetLengthProp()] = NULL;

					//Comprobamos que estamos en la seccion de declaracion de argumentos.
					if ((pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE)
						&& (pView->IsValueProp(M4PRESENTATION_INCLUDE_TYPE_ARGUMENT) == M4_TRUE) )
					{
						bArgumentSection = M4_TRUE;
					}

					if (bArgumentSection == M4_TRUE) {
						//Comprobamos que estamos en la seccion de declaracion de argumentos.
						//Cuando tenemos el alias, el valor es el id del arguemento.
						if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE)						
						{
							//En el valor tenemos el identificador del argumento.
							sIdArgument.append(pValue);
						}
					}
					delete [] pValue;
				}

				if (bArgumentSection == M4_TRUE) {
					if (pView->IsNameProp(M4PRESENTATION_INCLUDE_PROP_VALUEARG) == M4_TRUE)
					{
						//Tenemos que añadir el argumento en el mapa!. Se añade el id y la componente donde está el valor.
						iRet = _InsertIncludeArgument(ao_mArguments, sIdArgument, pView);			
					}
				}

				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
			}
		}
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_InsertIncludeArgument( MpArguments & ao_mArguments, const string & ai_sIdArgument, CComponent * ai_pCComponent )
{
	//El valor del argumento puede ser otro argumento ##ARG[ArgId2], pero tambien puede venir
	//una cadena o numero. De la componente se podrá extraer el valor correcto.

	//Una cosa es el identificador del argumento y otra es el valor que tiene.
	
	if ((ai_sIdArgument.size() > 0) 
		&& (ai_pCComponent != NULL))
	{
		pair<itMpArguments, bool> oMapArgumentPair;
		oMapArgumentPair = ao_mArguments.insert( MpArguments::value_type ( ai_sIdArgument, ai_pCComponent ) );

		//CHECK_CHLOG_ERRORF( oMapArgumentPair.second != M4_TRUE, M4_ERROR, M4_CH_PRES_BUILD_INTERNAL_ERROR,  __FILE__ << __LINE__ ) ;
		if (oMapArgumentPair.second != M4_TRUE) {
			//Hay un id de argumento repetido.
			DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_AnalizeLoadIncludePresentation( CBufferASN * ai_pCBufferASNRoot, m4char_t * ai_pIdPresentation, const MpArguments & ai_mArguments, CAsn * ai_pCAsn, const ClPresentationAttributes & ai_ClPresentationAttributes, CComponent * & ao_pIncludeRootComponent )
{
	//Carga y analiza la presentacion include que queremos cargar.

	m4return_t iRet = M4_SUCCESS;

	//--------------------------------------------------------
	ClPresentation * poPresentation = NULL;	

	//Cargamos la presentacion que hay que incluir
	iRet = _LoadIncludePresentation( ai_pCBufferASNRoot, poPresentation, ai_pIdPresentation, ai_ClPresentationAttributes );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}			

	//Si estamos aqui es pq se ha podido cargar la presentacion include, vamos a generar el arbol de componentes.

	//En la parte asn está la presentacion include resuelta!.
	CAsn * pAsnInclude = new CAsn();
	CInputStream * pInputStreamInclude = new CInputStream( poPresentation->GetBufferASN()->asn, poPresentation->GetBufferASN()->length );

	// Codificar las REFERENCIAS de ASN1 a objeto
	iRet = pAsnInclude->code( m_poChannelManager, M4_TRUE, pInputStreamInclude, NULL, M4_FALSE, 0, NULL, NULL, NULL );			
	delete pInputStreamInclude;
	pInputStreamInclude = NULL;

	//Independientemente si se ha producido un error, le decimos a la cache que liberamos la presentacion.
	//NO la queremos en la cache!!.
	if (poPresentation != NULL) {
		poPresentation->Destroy();
		poPresentation = NULL;
	}

	//Si ha habido problemas,
	if (iRet == M4_ERROR) {
		delete pAsnInclude;
		return M4_ERROR;
	}

	//Tenemos la presentacion include, pero necesitamos resolver los argumentos que le 
	//pasa la presentacion padre!. Recorremos el arbol de nodos para solucionar los argumentos.
	iRet = _SolveIncludeArgumentsRoot( pAsnInclude, ai_mArguments );
	if (iRet == M4_ERROR) {
		delete pAsnInclude;
		return M4_ERROR;
	}

	//-------------------------------------------------------------
	//Importante!
	//La presentacion Include puede tener a su vez mas includes, es necesario analizarlos
	//antes de hacer el mix!.
	//Pasamos el Asn del padre y el del include detectado que se va a analizar.	
	//Es el include que vamos a analizar.
	string sIdInclude(ai_pIdPresentation);
	iRet = _AnalizeIncludesRoot(ai_pCBufferASNRoot, ai_pCAsn, pAsnInclude, sIdInclude, ai_ClPresentationAttributes);
	if (iRet == M4_ERROR) {
		delete pAsnInclude;
		return M4_ERROR;
	}
	//-------------------------------------------------------------

	//***** pAsnInclude->present->DumpInfoRoot(M4_TRUE, ai_pIdPresentation, 0);

	//-------------------------------------------------------------
	//Importante:	
	//Si estamos aqui es pq el include ha resuelto todos los subinclude que tiene, es decir ya tenemos el include
	//planchado. Y lo vamos a pasar a la presentacion que invoco al include.

	//Antes de destruir el asn del include necesitamos desasociar el nodo raiz del include que se va a quedar
	//la presentacion padre!.
	//Desasociamos pq ao_pIncludeRootComponent es lo mismo que 	pAsnInclude->present
	iRet = _ClearIncludeTreeComponent( pAsnInclude, ao_pIncludeRootComponent );
	if (iRet == M4_ERROR) {
		delete pAsnInclude;
		return M4_ERROR;
	}
	//-------------------------------------------------------------

	delete pAsnInclude;

	return iRet;
}

m4return_t CInternalPresentationFactory::_LoadIncludePresentation( CBufferASN * ai_pCBufferASNRoot, ClPresentation * & ao_pPresentation, m4pcchar_t ai_pIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes )
{
	//Obtenemos la presentacion include y se analiza, en el argumento que pasamos por referencia
	//está la presentacion del include analizada!.
	//El invocante tiene la responsabilidad de liberar memoria.

	m4return_t iRet = M4_SUCCESS;
	ao_pPresentation = NULL;

	//Nos pasan la presentacion a analizar.
	if ((ai_pIdPresentation == NULL) || (m_pPresentationFactory == NULL)){
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Necesitamos invocar a la factoria y analizaremos el asn de la presentacion.
	//Es decir, crear el arbol de componentes. Retornará la presentacion.	
	iRet = m_pPresentationFactory->BuildPresentationFromId( (m4pchar_t) ai_pIdPresentation, ai_ClPresentationAttributes.m_pOrganization, ai_ClPresentationAttributes.m_pRole, ai_ClPresentationAttributes.m_iLanguage, M4_FALSE, M4_FALSE, ao_pPresentation );
	if (iRet == M4_ERROR) {		
		return M4_ERROR;
	}

	//En este punto se ha podido cargar el include y tenemos su version, se lo indicamos a la presentacion principal.
	if ((ai_pCBufferASNRoot != NULL) && (ao_pPresentation != NULL)) {
		const string sIdPresentation(ai_pIdPresentation);
		if (ai_pCBufferASNRoot->AddInfoIncludeInBufferASN(sIdPresentation, ao_pPresentation->GetVersion()) == M4_ERROR) {
			return M4_ERROR;
		}
	}

	//No liberamos la subpresentacion pq ahora pasa a pertencer a la superpresentacion.
	return iRet;
}

m4return_t CInternalPresentationFactory::_SolveIncludeArgumentsRoot( CAsn * ai_pCAsn, const MpArguments & ai_mArguments )
{
	//Recorremos el arbol para sustituir los argumentos por el valor correcto.

	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------
	CComponent * pPresent = NULL;
	CListComponent	* pPropBag = NULL;
	if ((ai_pCAsn != NULL)
		&& ((pPresent = ai_pCAsn->present) != NULL)
		&& ((pPropBag = pPresent->propBag) != NULL) )
	{
		//Recorremos las componentes.
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);
			iRet = _SolveIncludeArguments(pView, ai_mArguments);
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}			
		}
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_SolveIncludeArguments( CComponent * ai_pCComponent, const MpArguments & ai_mArguments )
{
	//Value = "##ARG[Arg1]"
	//BEGIN Component xxx
	//	Value = "##ARG[Arg1]"
	//END

	//Este es el recursivo.

	m4return_t iRet = M4_SUCCESS;

	//-----------------------------------------------------------------------------
	if (ai_pCComponent != NULL) {

		//Solo comprobamos elementos que cuyo valor sea cadena, es la única forma de especificar un argumento.

		if (ai_pCComponent->GetTypeProp() == ASNGeneralString) {
			//Reservamos memoria para el nombre y su valor.
			m4char_t * pName = new m4char_t[ai_pCComponent->GetLengthNameProp() + 1];
			ai_pCComponent->GetNameProp(pName);
			pName[ai_pCComponent->GetLengthNameProp()] = NULL;

			m4char_t * pValue = new m4char_t[ai_pCComponent->GetLengthProp() + 1];
			ai_pCComponent->GetStringValueProp(pValue);
			pValue[ai_pCComponent->GetLengthProp()] = NULL;

			//Vamos a comprobar si el valor es un argumento que tendremos que sustituir.			
			SetIdArguments oSetIdArguments;
			iRet = _GetIdArgument(pValue, oSetIdArguments);
			if ((iRet != M4_ERROR) && (oSetIdArguments.empty() == M4_FALSE)){
				//La funcion no es erronea pero vamos a comprobar el id del argumento.
				iRet = _SolveIncludeOneArgument(oSetIdArguments, ai_pCComponent, ai_mArguments);
			}
			delete [] pName;
			delete [] pValue;

			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
		}

		else {
			if (ai_pCComponent->GetTypeProp() == ASNSequence)
			{
				//Si es una secuencia iteramos para cada uno de sus hijos.
				CListComponent	* pPropBag = NULL;
				if ((pPropBag = ai_pCComponent->propBag) != NULL) {
					//Recorremos las componentes.		
					CComponent * pView = NULL;
					m4uint32_t iNumComponent = pPropBag->GetNumComponent();
					for (m4uint32_t i=0; i<iNumComponent; i++) {
						pView = pPropBag->View(i);
						iRet = _SolveIncludeArguments( pView, ai_mArguments );
						if (iRet == M4_ERROR) {
							return M4_ERROR;
						}
					}
				} //if ((pPropBag = ai_pCComponent->propBag) != NULL) {
			}
		}
	} //if ((ai_pCComponent != NULL)
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_SolveIncludeOneArgument( const SetIdArguments & ai_oSetIdArguments, CComponent * ai_pCComponent, const MpArguments & ai_mArguments )
{
	for (constitSetIdArguments itSetIdArgument = ai_oSetIdArguments.begin(); itSetIdArgument != ai_oSetIdArguments.end(); itSetIdArgument++) 
	{
		const string & sIdArgument = *itSetIdArgument;
		string sPattern("##ARG[");
		sPattern.append(sIdArgument);
		sPattern.append("]");

		CComponent * pSource = NULL;

		//Busca el patron que identifica un argumento y lo compara con los que hay en el mapa.
		constitMpArguments it = ai_mArguments.find(sIdArgument);
		if (it != ai_mArguments.end()) {
			//Lo hemos encontrado.
			//Tenemos que dar el cambiazo, ahora la componente tiene que tener el valor del valor que viene en el mapa.
			pSource = (*it).second;
			if ((pSource == NULL) || (ai_pCComponent == NULL)) {
				DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				return M4_ERROR;
			}
		}
		else {
			//No se ha encontrado, vamos a asumir que queremos blanco!.
			if (ai_pCComponent->ReplaceValuePropSearchingPattern( sPattern, "" ) == M4_ERROR) {					
				return M4_ERROR;
			}
			continue;
		}

		//Ahora tenemos que dar el tipo de propiedad que indica el origen!.
		//Para el tipo del VALOR de la propiedad tenemos (en decimal):
		//0=T_BOOL, 1=T_INT, 2=T_STRING, 3=T_SEQ
		m4uchar_t iTypeProp = pSource->GetTypeProp();
		switch (iTypeProp) {			

			case ASNGeneralString:
				{ //Bloque
					m4uint32_t iLengthStringProp = 0;
					m4char_t * pValueStringProp = NULL;

					iLengthStringProp = pSource->GetLengthProp();
					//Ojito pq NO es null terminada. Se guardan sin el null.
					pValueStringProp = new m4char_t [iLengthStringProp + 1];
					pSource->GetStringValueProp(pValueStringProp);

					m4char_t * pNewString = new m4char_t [iLengthStringProp + 1];
					memcpy(pNewString, pValueStringProp, iLengthStringProp);
					pNewString[iLengthStringProp] = NULL;
					string sNewString(pNewString);
					delete [] pValueStringProp;
					delete [] pNewString;
					
					if (ai_pCComponent->ReplaceValuePropSearchingPattern( sPattern, sNewString ) == M4_ERROR) {					
						return M4_ERROR;
					}
				} //Fin bloque

				break;

			default:
				//No permitimos que los argumentos puedan ser enteros o booleanos, la razon es pq
				//no puede especificarse pe Top = "##ARG[Arg1]" pq Top está tipado como entero.

				//Tampoco pueden llegar sequencias. Los argumentos son siempre hojas.
				DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_GetIdArgument( m4char_t * ai_pValue, SetIdArguments & ao_oSetIdArguments )
{
	//Buscamos el patron y retornamos el idenitificador del argumento.
	//	"##ARG[Arg1]"

	m4return_t iRet = M4_SUCCESS;
	ao_oSetIdArguments.clear();

	if (ai_pValue == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Vamos a insertar en el vector de salida los argumentos que se pueden encontrar.
	string sValue(ai_pValue);
	m4int32_t iPos = 0;
	m4int32_t iPosBegin = 0;
	m4int32_t iPosEnd = 0;
	const string sPattern("##ARG[");

	while ((iPosBegin = sValue.find("##ARG[", iPos)) != -1)
	{
		//Se ha encontrado el patron, ahora buscamos el fin del corchete.
		iPosBegin = iPosBegin + sPattern.size();
		iPosEnd = sValue.find("]", iPosBegin);
		if (iPosEnd == -1) {
			DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			return M4_ERROR;
		}
		iPos = iPosBegin;

		//Lo que queda entre iPosBegin + strlen(patron) y iPosEnd es el identificador del argumento.
		string sIdArgument = sValue.substr(iPosBegin, iPosEnd - iPosBegin);				
		ao_oSetIdArguments.insert(sIdArgument);
	}

	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_RenameAliasIncludeRoot( CComponent * ai_pIncludeRootComponent, m4char_t * ai_pIdAlias )
{
	m4return_t iRet = M4_SUCCESS;

	//El root obligatoriamente es de un Includepanel o Includeevent, por lo tanto podemos estar seguros que
	//el alias a sustituir será el del IncludePanel o Includeevent.
	//De todas formas, lo que hacemos es permitir que el Root no tenga que ser un IncludePanel,
	//el alias lo cambiamos siempre, de esta forma podríamos tener llamadas a otras presentaciones,
	//sin necesidad de que fueran IncludePanel o Includeevent.

	//-----------------------------------------------------------------------------	
	CListComponent	* pPropBag = NULL;
	if (((pPropBag = ai_pIncludeRootComponent->propBag) != NULL) )
	{	
		CComponent * pView = NULL;
		m4bool_t bIsIncludePanel = M4_FALSE;
		m4int32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4int32_t i=0; i<iNumComponent; i++) {
			pView = pPropBag->View(i);
			if (pView->GetTypeProp() == ASNGeneralString) 
			{				
				//El alias siempre es una hijo del root y es cadena.				
				//Cuando el tenemos el alias del panel, en Value tendremos el id del include que queremos
				if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
					//Sustituimos el valor.
					iRet = pView->ReplaceValueProp(ai_pIdAlias);					
					return iRet;
				}
			}
		}
	}
	//-----------------------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_ClearIncludeTreeComponent( CAsn * ai_pCAsn, CComponent * & ao_pIncludeRootComponent )
{	
	ao_pIncludeRootComponent = NULL;

	if (ai_pCAsn != NULL) {		
		//Devolvemos la componente raiz del include.
		ao_pIncludeRootComponent = ai_pCAsn->present;
		ai_pCAsn->present = NULL;	
	}
	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_ClearSubTreeComponentByRoot( CComponent * ai_pIncludeRootComponent )
{
	//Aqui se entra con el nodo raiz de la presentacion include. Y cuando se ha analizado totalmente.
	//Determinamos qué parte es la que vamos a pasar a la presentacion principal.

	m4return_t iRet = M4_SUCCESS;

	if (ai_pIncludeRootComponent == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	
	//IMPORTANTE:
	//Pordríamos desasociar los nodos del include que no vayan a pasar a la principal, pero la decision
	//es pasar TODO el arbol de la include a la principal, por lo tanto:
	//NO hay que hacer nada!!!!!!!!!!!!!!!!!!!!!!!!!!.

		//El raiz del include siempre es una SEQ, podemos desasociar los hijos de primer nivel que NO sean SEQ!!!.
		//Pq son lo que se pasaran a la presentacion padre principal.	
			//BEGIN Includepanel zzzz		//--- DESASOCIAMOS.
			//	Height = 3					//--- DESASOCIAMOS.
			//	BEGIN Textbox yyyy			//--- Este subarbol NO lo tocamos pq se pasara a la principal.
			//		Height = 10
			//	END
			//	BEGIN Textbox vvvv			//--- Este subarbol NO lo tocamos pq se pasara a la principal.
			//		Height = 20
			//	END
			//END

	return iRet;
}

m4return_t CInternalPresentationFactory::_FreeSubTreeComponentByRoot( CComponent * & aio_pCComponent )
{
	if (aio_pCComponent != NULL) {
		delete aio_pCComponent;
		aio_pCComponent = NULL;		
	}
	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::BuildPresentation( m4pcchar_t ai_pIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, CBufferASN * ai_pBasicPresentation, m4bool_t ai_bDefinition, CBufferASN * & ao_pParticularPresentation )
{
	m4return_t iRet = M4_SUCCESS;

	// Se resetea la variable de salida
	ao_pParticularPresentation = NULL;

	if (ai_pBasicPresentation == NULL) {
		return M4_ERROR;
	}

	if( ai_pBasicPresentation->length == 0 ) {
		return M4_ERROR;	
	}

	// Se reserva memoria para el buffer de ao_pParticularPresentation
	// En tiempo de diseño no se utiliza pero se inicializa para que todo quede igual
	ao_pParticularPresentation = new CBufferASN();
	ao_pParticularPresentation->version = ai_pBasicPresentation->version;
	ao_pParticularPresentation->style = ai_pBasicPresentation->style;

	// Si en la presentación básica no pone más que "NOT_FOUND" entonces reseteamos todo. 
	if( memcmp( ai_pBasicPresentation->asn, M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE ) == 0 )
	{
		ao_pParticularPresentation->asn = new m4uchar_t[M4PRESENTATION_NOT_FOUND_SIZE];
		ao_pParticularPresentation->ref = new m4uchar_t[M4PRESENTATION_NOT_FOUND_SIZE];
			
		memcpy( ao_pParticularPresentation->asn, M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE ) ;
		memcpy( ao_pParticularPresentation->ref, M4PRESENTATION_NOT_FOUND, M4PRESENTATION_NOT_FOUND_SIZE );

		ao_pParticularPresentation->length = M4PRESENTATION_NOT_FOUND_SIZE;
		ao_pParticularPresentation->lengthRef = M4PRESENTATION_NOT_FOUND_SIZE;	

		return M4_SUCCESS;
	}

	//------------------------------------------------------------------------------
	//Creamos un arbolillo a partir del buffer.
	CAsn * pAsn = NULL;	
	iRet = _CreateCAsn( ai_pBasicPresentation, ai_bDefinition, pAsn );
	if (iRet == M4_ERROR) {		
		delete ao_pParticularPresentation;
		ao_pParticularPresentation = NULL;					
		return M4_ERROR;
	}

	// Si estamos aqui, ya tenemos el arbolillo en pAsn.
	//------------------------------------------------------------------------------

	// En tiempo de diseño no se debe hacer nada de esto
	// Nos lo saltamos por bug id 0128589
	
	//---------------------------------------------------------------
	// Veamos si se trata de una presentacion principal.
	
	if (_IsPresentationRoot(pAsn) == M4_TRUE && ai_bDefinition == M4_FALSE) {

		//Pasamos ao_pParticularPresentation para que vayan añadiendo la lista de includes y su version.
		//Se trata del raiz, no hay presentacion padre.
		iRet = _AnalizeIncludesRoot( ao_pParticularPresentation, NULL, pAsn, ai_pIdPresentation, ai_ClPresentationAttributes );
		if (iRet == M4_ERROR) {
			delete pAsn;
			delete ao_pParticularPresentation;
			ao_pParticularPresentation = NULL;
			return M4_ERROR;
		}

		//Cuando volvemos, tenemos la lista de includes y sus versiones en ao_pParticularPresentation!!.

		//***** pAsn->present->DumpInfoRoot(M4_TRUE, ai_pIdPresentation, 0);

		//1.- AnalizarSequence:
		//			Por defecto toda componente tiene un m_access con grant 31.
		//			Se setea el m_access a partir de la componente (Para grants, idcanal,...).
		//			Si hay componente grant se indicará en el m_access que es grantsByUser y se setea el grant 
		//			del m_access con el valor que dice la componente.
		//			Si no hay componente grant se pone el valor 31 al m_access.
		//2.- AskSegurity_Indirection:
		//			El m_access tiene el canal, se extrae la seguridad de canal y setea el m_access de la componente
		//			con grant del m_access & máscara de permisos del canal.		
		//3.- AddSegurity:
		//			Si el usuario NO puso grant (grantsByUser es M4_FALSE), se añade componente grant a partir del 
		//			grant del m_access.	
		//4.- decode:
		//			En la decodificación, si se encuentra componente grant se envia al buffer el grant del m_access. Pero
		//			este código se modifica para que ahora se envie el valor grant de la componente en vez del valor
		//			grant del m_access.

		//Calcular de la SEQ principal la jerarquia Channel-Node-Item y la propiedad de seguridad (Grants).
		pAsn->present->AnalizarSequence(M4_TRUE, ao_pParticularPresentation);
	
		// Dependiendo del Channel-Node-Item de cada SEQ calcular el valor de Grants (mascara de seguridad).
		iRet = AskSegurity_Indirection( pAsn->present, NULL, NULL, ai_ClPresentationAttributes, ao_pParticularPresentation);
		
		Destroy_map();

		if (iRet == M4_ERROR) {
			delete pAsn;
			delete ao_pParticularPresentation;
			ao_pParticularPresentation = NULL;
			return M4_ERROR;
		}

		pAsn->present->AddSegurity();
	}
	//---------------------------------------------------------------

	COutputStream oCOutputStream;

	//Despues de todos los cambios hechos, recalculamos el valor verdadero de la sequence raiz.
	pAsn->present->length = 1;
	pAsn->present->RecalculeLength( pAsn->present->propBag );

	//Decodificar de jerarquia de objetos a ASN1
	pAsn->present->decode( &oCOutputStream, M4_FALSE );

	//Copiar codigo ASN1 en ao_pParticularPresentation
	m4uint32_t iLength = oCOutputStream.GetLength() ;
	ao_pParticularPresentation->asn = new m4uchar_t[iLength];

	for(m4uint32_t i=0; i<iLength; i++) {
		ao_pParticularPresentation->asn[i] = oCOutputStream.m_stream[i];
	}
	ao_pParticularPresentation->length = iLength;

	delete pAsn;

	return M4_SUCCESS;
}

//-----------------------------------------------
// Herencia
//-----------------------------------------------

m4return_t CInternalPresentationFactory::_CreateCAsnInheritancePresentationChain( const VcInheritCBufferASN & ai_vInheritCBufferASN, m4bool_t ai_bDefinition, VcInheritCAsn & ao_vInheritCAsn )
{
	m4return_t iRet = M4_SUCCESS;

	//Transforma cada CBufferASN en arbolito!.
	//Obligamos a que no tengamos nada en la lista de salida.
	if (ao_vInheritCAsn.empty() == M4_FALSE) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	for (constitVcInheritCBufferASN it = ai_vInheritCBufferASN.begin(); it != ai_vInheritCBufferASN.end(); it++) 
	{
		CBufferASN * pCurrentCBufferASN = (*it);
		if (pCurrentCBufferASN != NULL) {
			CAsn * pCAsn = NULL;
			iRet = _CreateCAsn( pCurrentCBufferASN, ai_bDefinition, pCAsn );
			if (iRet ==	M4_ERROR) {
				//Dejamos de transformar en arbolitos!!.
				break;
			}
			//Lo ha transformado en arbolito!.
			ao_vInheritCAsn.push_back( pCAsn );
		}
	}

	//-----------------------------------------------------------------
	//Si ha habido error tenemos que eliminar los arbolitos generados!.
	if (iRet == M4_ERROR) {
		CInternalPresentationFactory::RemoveInListAllCAsn( ao_vInheritCAsn, M4_TRUE );
	}
	//-----------------------------------------------------------------

	return iRet;
}

m4return_t CInternalPresentationFactory::_CreateCAsn( CBufferASN * ai_pCBufferASN, m4bool_t ai_bDefinition, CAsn * & ao_pCAsn )
{
	m4return_t iRet = M4_SUCCESS;

	if (ai_pCBufferASN == NULL || ao_pCAsn != NULL)
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Creamos la variable de salida.
	//Importante: En caso de error nos aseguraremos de destruirla.
	ao_pCAsn = new CAsn();

	//------ Creamos el CAsn (arbolillo) a partir del buffer. ------------
	CInputStream oCInputStream( ai_pCBufferASN->asn, ai_pCBufferASN->length );

	if (ai_bDefinition == M4_FALSE) 
	{
		// Begin Runtime 
		if (ai_pCBufferASN->lengthRef != 0)
		{
			CAsn oCAsnREF;
			CInputStream oCInputStreamREF( ai_pCBufferASN->ref, ai_pCBufferASN->lengthRef );

			//Se codifica con idioma
			iRet = oCAsnREF.code( m_poChannelManager, M4_TRUE, &oCInputStreamREF, NULL, M4_FALSE, 0, NULL, NULL, NULL );

			if (iRet == M4_ERROR)
			{		
				delete ao_pCAsn;
				ao_pCAsn = NULL;
				DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				return M4_ERROR;	
			}
			
			//Se codifica junto con el resultado del idioma.
			iRet = ao_pCAsn->code( m_poChannelManager, M4_FALSE, &oCInputStream, &oCAsnREF, M4_FALSE, 0, NULL, NULL, NULL );
		}
		else
		{
			//No hay idioma, por lo tanto no tiene que codificarse con el idioma.
			iRet = ao_pCAsn->code( m_poChannelManager, M4_FALSE, &oCInputStream, NULL, M4_FALSE, 0, NULL, NULL, NULL );
		}
	}
	else
	{			
		// Begin Design Time
		// Si de la base de datos ha venido algun idioma relleno, lo meto en el oCInputStreamRef
		m4bool_t bResolveRefs = M4_FALSE; 		
		for (m4int32_t j=0; j<ai_pCBufferASN->references; j++)
		{
			if (ai_pCBufferASN->ppiLengthRefs[j] != 0)
			{
				bResolveRefs = M4_TRUE; 
			}
		}

		if (bResolveRefs) 
		{
			CAsn *poCAsnREF = NULL;							
			poCAsnREF = new CAsn[ai_pCBufferASN->references]; 

			// Sacamos el arbolillo de las referencias. 
			for (m4int32_t j=0; j<ai_pCBufferASN->references; j++)
			{
				poCAsnREF[j];
				CInputStream oCInputStreamREF( ai_pCBufferASN->ppcRefs[j], ai_pCBufferASN->ppiLengthRefs[j] ); 
   				iRet = poCAsnREF[j].code( m_poChannelManager, M4_TRUE, &oCInputStreamREF, NULL, M4_TRUE, 0, NULL, NULL, NULL );
				if (iRet == M4_ERROR)
				{
					delete ao_pCAsn;
					ao_pCAsn = NULL;
					delete []poCAsnREF; 
					DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;	
				}
			}
			
			// Sacamos el otro arbolillo, a partir del arbolillo de las referencias
			iRet = ao_pCAsn->code( m_poChannelManager, M4_FALSE, &oCInputStream, NULL, M4_TRUE, ai_pCBufferASN->references, poCAsnREF, ai_pCBufferASN->ppcCodes, NULL );

			delete []poCAsnREF; 
		}
		else
		{
			//No hay idioma, por lo tanto no tiene que codificarse con el idioma.
			iRet = ao_pCAsn->code( m_poChannelManager, M4_FALSE, &oCInputStream, NULL, M4_TRUE, 0, NULL, NULL, NULL );
		}
	}
	//------ FIN Creamos el CAsn (arbolillo) a partir del buffer. ------------

	if (iRet == M4_ERROR) {
		delete ao_pCAsn;
		ao_pCAsn = NULL;
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Si ha ido bien se devuelve el arbolito.
	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_AnalizeInheritancePresentationChain( VcInheritCAsn & aio_vInheritCAsn, m4bool_t ai_bDefinition, CBufferASN * & ao_pMergedBufferASN )

{
	//Obligamos a que nos den nulo.
	if (ao_pMergedBufferASN != NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	if (aio_vInheritCAsn.empty() == M4_TRUE) {
		//Por lo menos tiene que haber una!.
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	ao_pMergedBufferASN = NULL;

	CAsn * pCAsnChild = NULL;
	CAsn * pCAsnParent = NULL;
	m4int32_t iCurrent = 0;

	for (itVcInheritCAsn it = aio_vInheritCAsn.begin(); it != aio_vInheritCAsn.end(); it++) 
	{
		CAsn * pCurrentCAsn = (*it);
		if (pCurrentCAsn != NULL) {
			pCAsnChild = pCurrentCAsn;
			
			if (ai_bDefinition) 
			{ 
				// Este es el truco de pre-marcar todas las presentaciones excepto la n-1 ésima
				// con Inheritstatus a Inherited (3). El resto (última hija) será Normal (1). 
				// Como va a ir copiando del padre a la hija, lo que se copie del padre sera Inherited.
				// Excepto el caso que tenemos interceptado de que haya sobreescritura. 
				if (iCurrent < aio_vInheritCAsn.size() - 1 ) 
				{ 							
					_SetInheritanceFlag ( pCAsnChild->present, M4PRESENTATION_INHERITSTATUS_VALUE_INHERITED ); 					
				}
				else
				{
					_SetInheritanceFlag ( pCAsnChild->present, M4PRESENTATION_INHERITSTATUS_VALUE_NORMAL );
				}
			}

			// Es la primera vez que paso. Es la primera de la jerarquia porque no tiene padre
			if (_AnalizeInheritancePresentationRootByAsn( pCAsnParent, pCAsnChild ) == M4_ERROR) {				
				if (pCAsnParent != NULL) 
				{
					// Si hay un error, en el vector se dejo un nulo por lo tanto necesitamos
					// liberar a mano el padre pq al eliminar el vector no hay puntero al objeto referenciado por padre.
					delete pCAsnParent;
				}
				return M4_ERROR;
			}			
			pCAsnParent = pCAsnChild;



			//Se ha analizado la current.
			aio_vInheritCAsn[iCurrent] = NULL;
		}
		

		iCurrent++;
	}

	// Cuando se sale del bucle el puntero padre e hijo son el mismo.
	pCAsnParent = NULL;

	//--------------------------------------------------------------------------------------------
	//NOTA:
	//Este paso podría sobrar si VB los eliminara, pero nosotros nos curamos en salud. Pero esto es costoso!!!.
	//Si ha ido bien la mezcla de arbolitos, vamos a revisar el arbol resultado.
	//Este paso se hace para controlar el siguiente escenario:
	//La presentacion hija tiene objetos a borrar (por ##DEFAULT_VALUE## o por Deletedobject) y que no
	//había correspondencia con objeto en la padre. Se van a eliminar esos componentes.
	if ((pCAsnChild != NULL) && (pCAsnChild->present != NULL)) {
		if (pCAsnChild->present->AnalizeChildToDeleteObjectNotExistsInParentRoot() == M4_ERROR) {			
			delete pCAsnChild;
			pCAsnChild = NULL;
			return M4_ERROR;
		}
	}
	//--------------------------------------------------------------------------------------------

	// Aqui ya hemos hecho la mezcla de la herencia
	// =============================================
	// En tiempo de diseño, queremos pasar por todo el arbolito poniendo la propiedad
	// Inheritstatus según cómo haya quedado el miembro InheritStatus de la secuencia
	// después de pasar por el algoritmo de la herencia. Y borrar los PositionAfter. 

	if (ai_bDefinition) 
	{
		m4int32_t iRet = _ResetInheritanceMarks ( pCAsnChild->present ); 
		if ( iRet == M4_ERROR ) 
		{
			return M4_ERROR;
		}
	}

	//------------------------------------------------------------
	//Ahora convertimos el arbolito resultante en CBufferASN.	
	CBufferASN * pCBufferASNChild = NULL;
	
	if ((pCAsnChild != NULL) && (pCAsnChild->present != NULL)) {
		COutputStream oCOutputStreamChild;

		//Recalculamos tamaños reales. Empieza con uno por el tag inicial.
		pCAsnChild->present->length = 1;
		pCAsnChild->present->RecalculeLength( pCAsnChild->present->propBag );

		//Convertimos el arbolillo en un output stream.
		pCAsnChild->present->decode( &oCOutputStreamChild, M4_FALSE );
		m4int32_t iOutputStreamLength = oCOutputStreamChild.GetLength();

		//Copiamos el codigo ASN del buffer de salida al CBufferAsn del hijo!. 
		pCBufferASNChild = new CBufferASN();
		pCBufferASNChild->asn = new m4uchar_t[iOutputStreamLength];
		
		for (m4int32_t i=0; i<iOutputStreamLength; i++) {
			pCBufferASNChild->asn[i] = oCOutputStreamChild.m_stream[i];
		}
		pCBufferASNChild->length = iOutputStreamLength;

		//Ya no lo necesitamos.
		delete pCAsnChild;
		pCAsnChild = NULL;
	}
	else {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	//------------------------------------------------------------

	//Hemos terminado con la herencia, nos lo vamos a guardar!.
	ao_pMergedBufferASN = pCBufferASNChild;
	
	//Truculento!!!.
	//El vector aio_vInheritCAsn ha debido de analizarse por lo tanto todo lo que quede en ese vector 
	//que no sea nulo habrá que liberarlo de alguna forma!!.
	//Realmente no debería quedar nada distinto de nulo.

	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_AnalizeInheritancePresentationRootByAsn( CAsn * & aio_pCAsnParent, CAsn * & aio_pCAsnChild)
{
	m4return_t iRet = M4_SUCCESS;

	//La primera llamada no hay hijo que mezclar. Se empieza por la primera en ascendencia (pe abuela).
	//Despues indicaríamos abuela-padre.
	//Por último padre-yo.
	if (aio_pCAsnParent == NULL) {
		return M4_SUCCESS;
	}

	//El hijo NO puede ser nulo.
	if (aio_pCAsnChild == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Hay herencia, tenemos mas de una presentacion, podemos hacer Merge!.			
	iRet = aio_pCAsnChild->MergeInheritPresentationRoot( aio_pCAsnParent );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}

	//En el proceso de merge, el argumento de entrada estará modificado. Se habrá liberado y el hijo tendrá
	//la informacion de la fusion padre-hijo.	

	return iRet;
}

void CInternalPresentationFactory::RemoveInListAllCAsn( VcInheritCAsn & aio_vInheritCAsn, m4bool_t ai_bDeleteElement )
{
	//Esta funcion es STATIC. Elimina y tambien puede liberar los elementos de una lista de CAsn.

	for (itVcInheritCAsn it = aio_vInheritCAsn.begin(); it != aio_vInheritCAsn.end(); it++) {
		CAsn * pCAsn = (*it);
		if ((pCAsn != NULL) && (ai_bDeleteElement == M4_TRUE)) {
			delete pCAsn;
		}
	}
	aio_vInheritCAsn.clear();
}



void CInternalPresentationFactory::RemoveInListAllCBufferASN( VcInheritCBufferASN & aio_vInheritCBufferASN, m4bool_t ai_bDeleteElement )
{
	//Esta funcion es STATIC. Elimina y tambien puede liberar los elementos de una lista de CBufferASN.

	for (itVcInheritCBufferASN it = aio_vInheritCBufferASN.begin(); it != aio_vInheritCBufferASN.end(); it++) {
		CBufferASN * pCBufferASN = (*it);
		if ((pCBufferASN != NULL) && (ai_bDeleteElement == M4_TRUE)) {
			delete pCBufferASN;
		}
	}
	aio_vInheritCBufferASN.clear();
}

m4return_t CInternalPresentationFactory::_SetInheritanceFlag( CComponent * ai_oComponent, m4int8_t ai_bInheritStatus )
{
	// esta funcion marca esta componente y sus hijas como heredadas o como normales
	// se usa para marcar toda la jerarquía ancestral a "inherited" antes de la mezcla de la herencia

	ai_oComponent ->SetInheritStatus(ai_bInheritStatus);
	
	CListComponent	* pPropBag = NULL;
	if ((pPropBag = ai_oComponent->propBag) != NULL) 
	{
		// empiezo a atravesar
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) 
		{
			pView = pPropBag->View(i);
			// me parece q esto no hace falta pq estoy recursionando
			pView->SetInheritStatus(ai_bInheritStatus);
			 // recursion
			_SetInheritanceFlag (pView, ai_bInheritStatus); 
		}
	}
	return M4_SUCCESS;
}

m4return_t CInternalPresentationFactory::_ResetInheritanceMarks( CComponent * ai_oComponent )
{

	m4return_t iRet  = M4_SUCCESS; 

	m4uchar_t	cTypeProp		= ai_oComponent->GetTypeProp();
	m4uint32_t	iInheritStatus	= ai_oComponent->GetInheritStatus();
		
	// Si es una secuencia, inserto un atributo de tipo entero llamado inheritstatus
	if (cTypeProp == ASNSequence)
	{

		CComponent *oInheritStatus  = new CComponent();

		oInheritStatus->SetTypeNameProp();
		oInheritStatus->SetLengthNameProp(M4PRESENTATION_INHERITSTATUS_SIZE);
		oInheritStatus->SetNameProp(M4PRESENTATION_INHERITSTATUS, M4PRESENTATION_INHERITSTATUS_SIZE);

		oInheritStatus->SetTypeProp(T_INT);
		oInheritStatus->SetLengthProp(M4_FALSE,T_INT,iInheritStatus);
		oInheritStatus->SetIntValueProp(iInheritStatus); 

		ai_oComponent->propBag->Add(oInheritStatus);
		
	}
	
	CListComponent	* pPropBag = NULL;
	if ((pPropBag = ai_oComponent->propBag) != NULL) 
	{

		// empiezo a atravesar la cosa
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pPropBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) 
		// Pendiente ver que pasa con esta iteración y el número de componentes.
		{
			pView = pPropBag->View(i);

			// hacer la recursión.
			iRet = _ResetInheritanceMarks (pView) ; 
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}

			if (pView->IsNameProp(M4PRESENTATION_INHERIT_PROP_POSITIONAFTER) == M4_TRUE)
			{
				// si el atributo se llama "positionafter" lo borramos.  
				iRet = ai_oComponent->_RemoveComponentInListByPos( i, M4_TRUE );

				/* Bug 0144493
				Si se borra hay que recalcular la posición para que no se salte elementos
				*/
				if( iRet == M4_SUCCESS )
				{
					iNumComponent-- ;
					i-- ;
				}
			}
		}
	}
	
	return M4_SUCCESS; 
	
}

