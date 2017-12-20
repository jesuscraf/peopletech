//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             channel.hpp
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
#ifndef __CHANNEL__HPP__
#define __CHANNEL__HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "property.h"
#include "colprop.hpp"
#include "acc_dir.hpp"
#include "chanprop.hpp"
#include "level2.hpp"
#include "groupinfo.hpp"
#include "not_cli.hpp"
#include "autoload.hpp"
#include "mcrowner.hpp"
#include "accevents.hpp"
#include "channelprocess.hpp"
#include "dm.hpp"
#include "m4stl.hpp"


class ClChannelDef;
class ClChannel_Data;
class ClVMBaseEnv;
class ClChannelManager;
class ClAccess;
class ClCompiledMCR;
class m4VariantType;
class ClLaneArgumnent;
class ClLConn_Interface;
class ClChannelProcess;
class ClCsTraceSize;
class ClNotifier;
class ClCursor;
class ClCodeJIT;
class ClFileTable;
class ClGenericIODriver;
class ClLogonAdaptor;
class ClVMStatistics;
class ClChannel;
class IObjDataCache;
class M4JavaContext;


// Notificaciones
#define M4_NOTIF_CHANNEL_DESTROY				0x00000001+M4_NOTIF_CHANNEL_BASE


typedef map< m4int32_t, ClAccessBookMark*, less<m4int32_t> >	ClBookMarkMap ;
typedef ClBookMarkMap::iterator									itBookMarkMap ;


// -- Level2 and Channel manager consistency
class ClConsState
{
private:
	m4int32_t iNRef;
	m4bool_t m_bIsVisited;

public:

	ClConsState(m4int32_t ai_iNRef = 0, m4bool_t ai_bIsVisited=M4_FALSE)
		: iNRef(ai_iNRef), m_bIsVisited(ai_bIsVisited)
	{
	}


	void Add(m4int32_t ai_iNRef=1, m4bool_t ai_bIsVisited=M4_TRUE) 
	{
		iNRef += ai_iNRef; 
		if (m_bIsVisited == M4_FALSE)
			m_bIsVisited = ai_bIsVisited;
	}

	void Decr(m4int32_t ai_iNRef=1)
	{
		iNRef -= ai_iNRef;
	}

	m4bool_t IsVisited()
	{
		return m_bIsVisited;
	}

	m4int32_t GetNRef ()
	{
		return iNRef;
	}
};



class ClChannelConsistency
{
	public:
		~ClChannelConsistency();

		typedef map< ClChannel * , ClConsState*, less<ClChannel*> > ClConsistencyMap;
		typedef ClConsistencyMap::iterator itConsistencyMap;

		void AddRef(ClChannel *ai_poRef, m4int32_t ai_iNRef=1, m4bool_t ai_bIsVisited=M4_TRUE);
		void DecrRef(ClChannel *ai_poRef, m4int32_t ai_iNRef=1)
		{
			AddRef(ai_poRef, -ai_iNRef, M4_FALSE);
		}
		m4return_t Check(ClChannelManager *ai_pcm);
		m4bool_t IsVisited(ClChannel *ai_poRef);
	
	private:
		ClConsistencyMap m_oMap;
};



class M4_DECL_M4DM ClChannel : public ClAutoLoad_PlugIn, public _AClClient_Notifier
{
protected:
	~ClChannel(); //Hay que llamar al Destroy

public:
    ClChannel( m4uint32_t ai_iHandle );
    ClChannel( ClChannelManager *ai_pcm, m4uint32_t ai_iHandle );

//---------------------------------------------
//colección de propiedades:

    ClColProp						Property ;				//Agrupación de las propiedades    

    ClGenericProperty				ApplyMappings	 ;		//Si se deben aplicar mapeos o no
    ClGenericProperty				MetaIdentifier ;		//Identificador lógico

    ClGenericProperty				StartDate ;				//MetaData, se usa en la buildfromid. No esta en la coleccion de propiedades
    ClGenericProperty				EndDate ;				//MetaData, se usa en la buildfromid. No esta en la coleccion de propiedades
    ClGenericProperty				CorrectionDate ;		//MetaData, se usa en la buildfromid. No esta en la coleccion de propiedades

    ClGenericProperty				VM_StartDate ;			//VM, se usa en el CreateAccess.
    ClGenericProperty				VM_EndDate ;			//VM, se usa en el CreateAccess.
    ClGenericProperty				VM_ExecutionDate ;		//VM, se usa en el CreateAccess.

// ya no están mapeadas
    ClGenericProperty				Data_StartDate ;		//Data, para filtros en hístoricos, entre otras cosas
    ClGenericProperty				Data_EndDate ;			//Data, para filtros en hístoricos, entre otras cosas
    ClGenericProperty				Data_CorrectionDate ;	//Data, para filtros en hístoricos, entre otras cosas

    ClGenericProperty				Data_Language ;			//Lenguaje de datos
    ClGenericProperty				Meta_Language ;			//Lenguaje de metadatos

    ClGenericProperty				UnitDate ;

    ClGenericProperty				LDB_LastLogicConn ;

    ClGenericProperty				CsType ;				//tipo de canal
    ClGenericProperty				RSM_ID ;				//RSM ya no es privada
    ClGenericProperty				Role_ID ;				//Role
    ClOrganizationProperty			Organization ;			//sociedad
    ClGenericProperty				OrganizationType ;		//tipo de sociedad

    ClGenericProperty				Tramitation ;			//Tramitación

    ClGenericProperty				RoundCurrency ;			//Redondear monedas a la precisión
    ClGenericProperty				RoundNumber ;			//Redondear números a la precisión
    ClGenericProperty				CurrencyPrecision ;		//Precisión de las monedas
    ClGenericProperty				NumberPrecision ;		//Precisión de los números

    ClGenericProperty				ExecuteRealSQLMode ;	//Permisos de ejecución del RealSQL

//---------------------------------------------

	ClNotifier	*m_pDestroy_Notifier;			// Notificacion de destruccion de canal.
//---------------------------------------------

    m4return_t CommitPersisted (VersionMark &ai_version);
    m4return_t RollbackPersisted (void);
	m4return_t ReLoad(void);

// jcr
    m4return_t CheckModify( m4bool_t &ao_bModified ) ;

//---------------------------------------------


//autoload:
    m4return_t SetAutoLoadMode (m4uint8_t ai_mode);

    m4return_t Load (ClAccess *ai_paccess = 0, m4bool_t ai_bFromWrapper=M4_FALSE);
    m4return_t Persist (ClAccess *ai_paccess = 0, m4bool_t ai_bFromWrapper=M4_FALSE);
    m4return_t UnLoad (m4bool_t ai_bRemoveCache = M4_TRUE);
    m4return_t CacheLoad (m4bool_t &ao_bLoaded);
    m4return_t CachePut (void);

	m4return_t LoadFromIOD(ClGenericIODriver& IOD);
	m4return_t LoadFromSttIOD(ClGenericIODriver& IOD, m4pcchar_t ai_pcIntermediateFile);
	m4return_t PersistToIOD(ClGenericIODriver& IOD, m4bool_t ai_bSerializeMCR);
	m4return_t LoadFromFile (m4pchar_t ai_pcFileName);
	m4return_t LoadFromSttFile (m4pchar_t ai_pcFileName);
	m4return_t PersistToFile (m4pchar_t ai_pcFileName, m4bool_t ai_bSerializeMCR);
	m4return_t PersistToTempFile (m4pchar_t ao_pcFileName, m4uint32_t ai_iSize);

    ClChannelDef *GetpChannelDef (void) const;

    m4bool_t IsBuilt (void) const;

    m4uint32_t GetHandle( void ) const;


//access factory:
    ClAccess *CreateAccess (void);    // obsoleta
    m4return_t CreateAccess (ClAccess * &ao_paccess);

    ClAccessDirectory               Access;         //colección de Access 'atachados' a mi	

//Level2
    ClLevel2                        Level2;         //funcionalidad de nivel 2

// Soporte para transacciones con múltiples canales
    ClGroupInfo                     m_oGroupInfo;

    
    ClChannel_Data *GetpChannel_Data (void) const
    {
        return m_pClChannel_Data;
    }

    ClCompiledMCR *GetpCMCR (void);

    m4return_t ReleaseAll (m4bool_t ai_bresetnodeitems = M4_TRUE);
    m4return_t Destroy (void);

//builds:
    m4return_t BuildFromMCR (ClCompiledMCR *ai_pcmcrfront, ClLaneArgumnent *ai_poLane = NULL);
    m4return_t BuildBackFromMCR (ClCompiledMCR *ai_pcmcrback);
    m4return_t BuildFromFile (m4pcchar_t ai_path);
    m4return_t BuildFromId (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane = NULL, m4uint8_t ai_iLanguage = M4CL_LANGUAGE_NONE );
    m4return_t BuildAsMetachannel (void);
    m4return_t BuildFromAccess (ClAccess* ai_poAccess);   
	m4return_t BuildFromChannelData (ClChannel_Data *ai_poChannelData);

//Comparación
	m4bool_t IsEqual( m4pcchar_t ai_pcChannelId, m4pcchar_t ai_pccTramitation, m4uint8_t ai_iCSAccessType, m4pchar_t ai_pcRole, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4bool_t ai_bCheckRole );

private:
    // private version for auditory
    m4return_t _BuildFromId (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane, m4uint8_t ai_iLanguage, m4bool_t ai_bCheckSec);
    m4return_t BuildFromIdNoSec (m4pcchar_t ai_pId, ClLaneArgumnent *ai_poLane = NULL, m4uint8_t ai_iLanguage = M4CL_LANGUAGE_NONE );

    ClChannel_Data *m_pClChannel_Data;

    void _InitCommon( m4uint32_t ai_iHandle );

    m4return_t CreateChannel_Data (ClChannel_Data *ai_poChannelData);
    ClChannel_Data *NewChannel_Data (void);
    m4return_t CommonInitAfterBuild (void);    
    m4return_t InitProperties (void);    
    
    m4return_t BuildChannelParamId (ClCompiledMCR * ai_pCMCR, m4VariantType * &ao_rpVarParamsId, m4uint32_t &ao_iNumParams);    

	// jcrizacion
	// Función peligrosa para cambiar al acanal sesión de un channelmanager a otro
	// Sólo la debe utilizar el appclien de emulación
	m4return_t	_TeleTransportTo( ClChannelManager *ai_poNewChannelManager ) ;


//******************PLUGINS************************

//----------------------------
//ChannelManager
	public:

		m4return_t GetSize(ClSize &ao_size);

		ClChannelManager* GetpChannelManager (void) const
		{
			return m_poClChannelManager;
		}

		ClVMBaseEnv* GetEnvironment (void) const
		{
			if (m_poClChannelManager)
			{
				return m_poClChannelManager->GetEnvironment();
			}
			else
			{
				return NULL;
			}
		}

		m4return_t Attach (ClChannelManager *ai_poChannelManager);

		m4return_t Detach(void)
		{
			return _Detach(M4_TRUE, M4_TRUE);
		}

		m4return_t DetachAndLeaveChannelManager(void)
		{
			return _Detach(M4_FALSE, M4_FALSE);
		}

		m4return_t DetachAndKeepChannel(void)
		{
			return _Detach(M4_FALSE, M4_TRUE);
		}

	private:
		ClChannelManager* m_poClChannelManager;

		ClCMCRFactory *GetpFactory (void);

		m4return_t DetachFromChannelManager(void)
		{
			return _Detach(M4_TRUE, M4_FALSE);
		}

		// internal
		m4return_t _Detach (m4bool_t ai_bdetach, m4bool_t ai_bDoRecursive);
		m4return_t _AttachRecursive (ClChannelManager *ai_poChannelManager);
		m4return_t _DetachRecursive (m4bool_t ai_bdetach, m4bool_t ai_bDoRecursive, ClChannelConsistency* ai_pConsMap);


//----------------------------

//----------------------------
//interface _AClClient_Notifier obligatorio:
//por defecto no hace nada:
	public:
		void DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
		void DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);

		void AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
		void InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);

		void DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
		void DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
		void DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
		void ReleaseAll_Before (void);
		void DestroyChannelData_Before (void);
        void DestroyChannel_Before (ClChannel * ai_poCh);

		void Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc);

		void ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh);
        ClChannel * GetpChannel (void);
//----------------------------

//-------------------------------------------------
//Serial:
    public:
       // Nos permite preguntar por la version de serializacion que se esta usando
       static m4uint16_t GetSerializationVersion(void);

       m4return_t Serialize(ClGenericIODriver& IOD, ClChannelProcess* const ai_pChannelProcess, ClCsTraceSize* const ai_pCsTraceSize, const m4uint32_t ai_iBlockIndex = 0, const m4uint32_t ai_iRegisterIndex = 0);

       // Deserializa el bloque 0, 0 del root
       m4return_t DeSerialize(ClGenericIODriver& IOD);
       m4return_t DeSerialize(ClGenericIODriver& IOD, m4uint32_t& ai_iBlockIndex, m4uint32_t& ai_iRegisterIndex, ClCsTraceSize* const ai_pCsTraceSize, m4bool_t ai_bSetSttLevel);

       m4return_t MarkForPersistSerial(const m4uint32_t ai_iBlockIndex, const m4uint32_t ai_iRegisterIndex);

       m4uint8_t GetRecordSerializationMode(void)
	   {
		   return m_iRecSerMod;
	   }
       
	   m4uint8_t GetItemSerializationMode(void)
	   {
		   return m_iIteSerMod;
	   }

       void SetRecordSerializationMode(const m4uint8_t ai_iRecSerMod)
	   {
		   m_iRecSerMod = ai_iRecSerMod;
	   }

       void SetItemSerializationMode(const m4uint8_t ai_iIteSerMod)
	   {
		   m_iIteSerMod = ai_iIteSerMod;
	   }

    protected:
       // Indica el modo de serializacion (ALL, MAPPED, ...) que se quiere utilizar,
       // tanto en el canal como en sus accesos, ...
       m4uint8_t m_iRecSerMod;
       m4uint8_t m_iIteSerMod;

	   /**
	    * Comprueba el estado de depuración, y en función de este determina el número de vectores de deltas
	    * que hay que utilizar. Estos vectores de deltas se crearán al terminar la deserialización.
		* Si en el cliente el estado de depuración es IS_DEBUG_LAST_TIME lo cambia por IS_NOT_DEBUG.
	    *
	    * @return Número de vectores de deltas que hay que usar, en función del modo de depuración.
	    */
	   m4uint8_t CheckDebugModeAndGetVectorDeltaCount(void);

	   /**
	    * Comprueba el estado de depuración, y en función de este determina el vectores de deltas
		* que hay que utilizar durante la serialización.
		*
		* @return Índice del vector de deltas que hay que usar durante la serialización.
		*/
	   m4uint8_t CheckDebugModeAndGetVectorDeltaToSerialize(void) const;

//-------------------------------------------------

//-------------------------------------------------
// New Record Date
    public:
        m4return_t SetNewRecordVersion(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion);

//-------------------------------------------------

//-------------------------------------------------
//Statistics
    public:
       m4return_t SetStatisticsLevel (m4uint16_t ai_iInit);  //Activar-Desactivar estadisticas.
       m4uint16_t  GetStatisticsLevel (void);

       m4return_t SetStatisticsLabel(m4pchar_t ai_pcLabel);
       m4pchar_t  GetStatisticsLabel(void);

    private:
	   m4uint16_t			m_iSttInit;
	   m4VariantType		m_vLabel;

//-------------------------------------------------

// -- Logical connections

	// private logical connection
	private:
		ClLConn_Interface *m_poMainLConn;
	public:
		ClLConn_Interface *GetMainLConn()
		{
			return m_poMainLConn;
		}
		
		void SetMainLConn(ClLConn_Interface *ai_poClLConn)
		{
			m_poMainLConn = ai_poClLConn;
		}

//-------------------------------------------------

	private:
		m4return_t Check_L2DM_Consistency(ClChannelManager *ai_pCM, ClChannelConsistency* ai_pConsMap=0);

//-------------------------------------------------
//proxys, EFV
	public:
		/**
		 * Valores posibles para el estado de depuración.
		 * CHANNEL_IS_NOT_DEBUG: Es lo normal. No se trata de un canal de depuración.
		 * CHANNEL_IS_DEBUG_FIRST_TIME: El canal está en modo depuración y todavía no ha
		 *		completado su primera transacción con el serivor.
		 * CHANNEL_IS_DEBUG: El canal está en modo depuración y ya ha realizado, al menos,
		 *		una transacción con el serivor.
		 * IS_DEBUG_LAST_TIME: El canal esté en modo de depuración y está haciendo su última
		 *		transacción al servidor. Esta sirve para sicronizar la vista back con la front.
		 * IS_DEBUG_VIEW: La vista es una levantada con el ProxyDebugView. Es una vista estática.
		 *      Soporte para depuración (inspector).
		 */
		enum eDebugMode_t { IS_NOT_DEBUG = 0, IS_DEBUG_FIRST_TIME, IS_DEBUG_SECOND_TIME, IS_DEBUG, IS_DEBUG_LAST_TIME, IS_DEBUG_VIEW };

		/** Indica si esta activado o desactivado el modo de depuración. */
		m4bool_t IsDebugEnabled(void) const;

		/**
		 * Activa el modo de depuración del canal (este módo de depuración se mantiene mientras dura la ejecución del método remoto).
		 * Al activar el modo de depuración se crea el espacio proxy de depuración, si es que este no existía.
		 * La desactivación del modo de depuración se hace automáticamente cuando se termina la ejecución del método remoto.
		 *
		 * @param m4return_t.
		 */
		m4return_t SetDebugEnabled(void);

		/** Geter para el modo de depuración */
		ClChannel::eDebugMode_t GetDebugMode(void) const
		{
			return m_eDebugMode;
		}

		/** Seter para el modo de depuración */
		void SetDebugMode(ClChannel::eDebugMode_t ai_eDebugMode)
		{
			m_eDebugMode = ai_eDebugMode;
		}
		
		/** Fija el identificador remoto del canal. Este me servirá para buscar el canal en futuras transacciones. */
		void SetRemoteId(const m4uint32_t ai_iRemoteId);

		m4uint32_t GetRemoteId(void) const
		{
			return m_iRemoteId;
		}

	private:
		/** Para identificar el canal en el servidor. */
		m4uint32_t m_iRemoteId;

		/** Indica el estado de depuración del canal. */
		eDebugMode_t m_eDebugMode;


		m4return_t GetProxyContext(void);
		m4return_t ReleaseProxyContext(void);

		void _SetDatesFromLA (ClLogonAdaptor * pLA);
//-------------------------------------------------


//-------------------------------------------------
// jesus. MCR back para los canales dinámicos
    ClMCROwner BackMCR;

    public:
        ClCompiledMCR* GetpBackMCR (void) const
        {
            return BackMCR.GetpCMCR();
        }
//-------------------------------------------------


//-------------------------------------------------
//Seguridad, jesus
	public :

		m4pchar_t	GetIDRole( void )
		{
			m4pchar_t	pcResult ;

			Role_ID.Get( pcResult ) ;
			return( pcResult ) ;
		}

		m4pchar_t	GetIDRSM( void )
		{
			m4pchar_t	pcResult ;

			RSM_ID.Get( pcResult ) ;
			return( pcResult ) ;
		}

		m4bool_t	GetIsLevel2( void ) const
		{
			return( m_iIsLevel2 ) ;
		}

        m4return_t	AttachRSM( void ) ;

	private :
        m4return_t	_ApplySecurity( void ) ;
        m4return_t	_GetSessionInfo( m4bool_t ai_bCheckSec ) ;

		void		SetIsLevel2( void )
		{
			m_iIsLevel2 = M4_TRUE ;
		}

		void		SetSerialized( void )
		{
			m_iIsSerialized = M4_TRUE ;
		}


		m4bool_t	m_iIsLevel2 ;			// Para saber si viene por nivel 2
		m4bool_t	m_iIsSerialized ;		// Para saber si viene por serialización

//-------------------------------------------------


//-------------------------------------------------
//JIT, Richard
    private:
        ClCodeJIT *_InitJIT (void);
	public:
        ClCodeJIT *GetpCodeJIT (void) 
        {
            return _InitJIT();
        }

        ///	Código JIT, instalado. Sólo se puede ejecutar en este canal.        
        ClCodeJIT *m_poCodeJIT;
//-------------------------------------------------


//-------------------------------------------------
//File Table, Richard	
    public:
	    ClFileTable	*m_poFileTable;
	    void InitFileTable(void);
//-------------------------------------------------


protected:

	// Identificador único para el rich client
	m4uint32_t	m_iHandle ;

	// Objeto cache para los canales auto
	IObjDataCache	*m_pIObjDataCache ;

	m4bool_t m_bIsSynchronized;

	// Si se stá haciendo ya el destroy
	m4bool_t m_bIsDestroying;

	// Mapa de bookmarks
	ClBookMarkMap m_oBookMarks;

// Objetos de java ============================================================

	// Set de contextos java utilizados
	set< M4JavaContext*, less<M4JavaContext*> > m_aoJavaContexts;

	void	ReferenceJavaContext( M4JavaContext *ai_poContext ) ;

// Objetos genéricos ==========================================================

	map< m4uint32_t, void*, less<m4uint32_t> > m_aoPointers;

public:

	m4uint32_t	NewBookMark( m4uint8_t ai_iType, ClAccess *ai_poAccess );
	m4uint32_t	ApplyBookMark( m4uint32_t ai_iBookMark, ClAccess *ai_poAccess );
	m4uint32_t	ReleaseBookMark( m4uint32_t ai_iBookMark );

	void	ReleaseJavaContexts( m4bool_t ai_bOnlyEmpty ) ;

	m4uint32_t	AddPointer( void* ai_poPointer ) ;
	m4bool_t	RemovePointer( m4uint32_t ai_iHandle ) ;
	void*		GetPointer( m4uint32_t ai_iHandle ) ;
	m4uint32_t	GetPointerKey( void* ai_poPointer ) ;

	void	SetObjDataCache( IObjDataCache *ai_pIObjDataCache )
	{
		m_pIObjDataCache = ai_pIObjDataCache ;
	}

//*************************************************

friend class ClLevel2 ;
friend class ClCMObjectDirectory ;
friend class ClAppClientAdapSM ;
friend class ClProxySpace ;
friend class ClSelectAuditory;
friend class ClInstanceMap;
friend class ClUserActionsAudit;
friend class ClLicenseChannelLoader;
friend class ClMDUpdateList;
friend class ClMappings;
friend class ClLConn;
friend class ClExecutiveForChannel;
friend class ClT3Access;
friend class ClT3AccessProvider;
friend class ClChannelManager;
friend class M4JavaManager;
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClChannel &ai_channel);


#endif  //__CHANNEL__HPP__

