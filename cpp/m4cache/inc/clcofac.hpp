//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                clcofac.hpp
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
//    Es una clase un tanto especial. Se encarga de de/serializar un objeto derivado de la clase
//ClCacheableObject.
//	Puede de/serializar la clase en el IOD que se pasa o en un FileIOD distinto. Lo principal
//es que conoce la clase que va a utilizar y por ello sabe que partes de/serializa (de este modo
//puede enviar parte a un IOD y parte a otro) y además hace el new del objeto cuando sea necesario.
//
//==============================================================================


#ifndef __CLCOFAC_HPP__
#define __CLCOFAC_HPP__
//## begin module.includes preserve=yes

#include "clcache.hpp"
#include "clgeniod.hpp"

//## end module.includes preserve=yes
class M4_DECL_M4CACHE ClBaseCacheableObjectFactory : public ClCacheableObjectFactory
{
public:
	ClBaseCacheableObjectFactory(void);

	~ClBaseCacheableObjectFactory(void);

	virtual m4return_t SerializeObject(ClCacheableObject * ai_poObject, ClGenericIODriver* IOD, m4uint8_t ai_iMode);

	virtual m4return_t DeSerializeObject(ClCacheableObject * &aio_pObject, ClGenericIODriver* IOD, m4uint8_t ai_iMode);

private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject) = 0;
};

#endif
 
