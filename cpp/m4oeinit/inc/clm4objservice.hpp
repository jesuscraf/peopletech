//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservice.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la Definición
//	  de la Maquina Virtual (M4Object & Data Services)
//
//
//==============================================================================

//Esta definición es común para Client, Server y SM

#ifndef _CLM4OBJSERVICE_HPP_
#define _CLM4OBJSERVICE_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"

class ClFactM4ObjService;
class ClCache;
class ILdb;
class ClCMCRFactory;
class ClChannelManager;
class ClCacheDirectory;
class ClPresentationFactory;
class ClLogonAdaptor;
class IExecutor;
class ClLConn_Provider;
class ClVMBaseEnv;
class ClM4ObjServiceContext;
class ClVMStatisticsWrapper;
class ClCsTrace;
class IExecAdaptor;
class ClDMContext;
class ClVMCSContext;
class ClCacheFactory;
class ClAppClientAdap;
class ILicenseManager;
class ClLicenseManagerBase;

/**
 * Definición de la Máquina Virtual (M4Object & Data Services).
 */
class M4_DECL_M4OEINIT ClM4ObjService {
	friend class ClLicenseManagerBase;
public:
//////////////////////////////////
//Métodos de devolución de atributos
	virtual ClCMCRFactory* const			GetpCMCRFactory() const			{ return m_poFactory; }
	virtual ClChannelManager* const			GetpChannelManager() const		{ return m_poChannelManager; }
	virtual ClCacheDirectory* const			GetpCacheDirectory() const		{ return m_poCacheDirectory; }
	virtual ClPresentationFactory* const	GetpPresFactory() const			{ return m_poPresentationFac; }
	virtual ClLogonAdaptor* const			GetpLogonAdaptor() const		{ return m_pLogonAdaptor; }
	virtual IExecutor* const				GetpExecutor() const			{ return m_pExecutor; }
	virtual ClLConn_Provider* const			GetpConnProvider() const		{ return m_poConnProvider; }
	virtual ClVMBaseEnv* const				GetpRegistry() const			{ return m_poRegistry; }
	virtual ClM4ObjServiceContext* const	GetpM4ObjServiceContext() const	{ return m_poM4ObjServiceContext; }
	virtual ClVMStatisticsWrapper* const	GetpStatisticsWrap() const		{ return m_poStatisticsWrap; }
	virtual ClCsTrace* const				GetpCsTrace() const				{ return m_poCsTrace; }
	virtual ILdb* const						GetpLDB() const					{ return m_pLDB; }
	virtual ILicenseManager* 				GetpLicenseManager() 			{ return m_pLicenseManager; }
	virtual ClCacheFactory* const			GetpCacheFactory() const		{ return m_poCacheFactory; }
	virtual ClAppClientAdap* const			GetpAppClientAdap() const		{ return m_pClientAPI; }

protected:
	/** Factoria de caches */
	ClCacheFactory*			m_poCacheFactory;

	/** Factoría de metadatos. */
	ClCMCRFactory* m_poFactory;

	/** Direcotrio de caches. */
	ClCacheDirectory* m_poCacheDirectory;

	/** Factoría de presentaciones. */
	ClPresentationFactory* m_poPresentationFac;

	/** Adaptador para el logon. */
	ClLogonAdaptor* m_pLogonAdaptor;

	/** Adaptador para el Ejecutor. */
	IExecAdaptor				*m_poExecAdap;

	/** Data Manager (DM). */
	ClChannelManager* m_poChannelManager;

	/** Contexto del Data Manager (DM). */
	ClDMContext					*m_poDMContext;

	/** Ejecutor de canales. */
	IExecutor* m_pExecutor;

	/** Contexto del Ejecutor. */
	ClVMCSContext				*m_pVMCSContext;

	/** Proveedor de conexiones. */
	ClLConn_Provider* m_poConnProvider;

	/** Información de Registry. */
	ClVMBaseEnv					*m_poRegistry;

	/** Contexto del M4ObjService. */
	ClM4ObjServiceContext		*m_poM4ObjServiceContext;

	/** Estadísticas. */
	ClVMStatisticsWrapper		*m_poStatisticsWrap;

	/** Trazas. */
	ClCsTrace					*m_poCsTrace;

	/** Logical DataBase (LDB). */
	ILdb						*m_pLDB;

	ILicenseManager * m_pLicenseManager;

	/** Crea un nuevo ClM4ObjService. Sólo puede crear objetos la factoria. */
	ClM4ObjService(){
		m_pLDB					=	0;
		m_pLicenseManager		=	0;
		m_poFactory				=	0;
		m_poChannelManager		=	0;
		m_poCacheDirectory		=	0;
		m_poPresentationFac		=	0;
		m_pLogonAdaptor			=	0;		
		m_pExecutor				=	0;
		m_poConnProvider		=	0;
		m_poRegistry			=	0;
		m_poM4ObjServiceContext	=	0;
		m_poStatisticsWrap		=	0;
		m_poCsTrace				=	0;
		m_poExecAdap			=	0;
		m_poDMContext			=	0;
		m_pVMCSContext			=	0;
		m_poCacheFactory		=	0;
		m_pClientAPI			=	0;
	}

	/** Destructor. Permito derivaciones. */
	virtual ~ClM4ObjService() {}

	/** Finaliza un ClM4ObjService.
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t End() = 0;

	ClAppClientAdap*		m_pClientAPI;

	friend class ClFactM4ObjService; //Para que pueda crear objetos.
};

#endif	// _CLM4OBJSERVICE_HPP_
