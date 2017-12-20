//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                clmcrfac.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================

#include "clcofac.hpp"
#include "m4cacheres.hpp"
#include "chlog.hpp"

//## end module.includes preserve=yes
ClBaseCacheableObjectFactory::ClBaseCacheableObjectFactory()
{
//## begin ClBaseCacheableObjectFactory::ClBaseCacheableObjectFactory%-1333115467.body preserve=yes
//## end ClBaseCacheableObjectFactory::ClBaseCacheableObjectFactory%-1333115467.body
}

ClBaseCacheableObjectFactory::~ClBaseCacheableObjectFactory()
{
//## begin ClBaseCacheableObjectFactory::~ClBaseCacheableObjectFactory%-897622612.body preserve=yes
//## end ClBaseCacheableObjectFactory::~ClBaseCacheableObjectFactory%-897622612.body
}

m4return_t ClBaseCacheableObjectFactory::SerializeObject(ClCacheableObject * ai_poObject, ClGenericIODriver* IOD, m4uint8_t ai_iMode)
{
//## begin ClBaseCacheableObjectFactory::SerializeObject%-1747352502.body preserve=yes
	m4return_t ret;
	m4uint8_t iType, iPersLevel;

	iPersLevel = ai_poObject->GetPersistianceLevel();

	iType = ai_poObject->GetType();

	ret = IOD->Write(iType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	//Serializo la Clase Derivada
	ret = ai_poObject->Serialize(*IOD);

	return ret;
//## end ClBaseCacheableObjectFactory::SerializeObject%-1747352502.body
}

m4return_t ClBaseCacheableObjectFactory::DeSerializeObject(ClCacheableObject * &aio_pObject, ClGenericIODriver* IOD, m4uint8_t ai_iMode)
{
//## begin ClBaseCacheableObjectFactory::DeSerializeObject%-1973364335.body preserve=yes
	m4return_t ret;
	m4uint8_t iType;

	ret = IOD->Read(iType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	//Creamos el objeto
	ret = CreateObject(iType, aio_pObject);

	if (ret == M4_ERROR)
		return M4_ERROR;

	m4uint8_t iRealType = aio_pObject->GetType();
	if (iRealType != iType)
	{
		//El objeto generado no es del tipo esperado.
		delete aio_pObject;
		aio_pObject = 0;
		g_oChLog<<BeginError(M4_CACHE_BAD_CO_TYPE)<<iRealType<<EndLog;
		return M4_ERROR;
	}
	
	//DeSerializamos la clase derivada
	ret = aio_pObject->DeSerialize(*IOD);

	return ret;
//## end ClBaseCacheableObjectFactory::DeSerializeObject%-1973364335.body
}
 
