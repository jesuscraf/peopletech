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


#ifndef __CHAN_KNL__HPP__
#define __CHAN_KNL__HPP__

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "operation.hpp"
#include "property.h"
#include "chanprop.hpp"
#include "nod_data.hpp"
#include "clprsstb.hpp"
#include "cldates.hpp"
#include "mcrowner.hpp"
#include "changelistener.hpp"
//## end module.includes preserve=yes


class ClAccess;
class ClChannel;
class ClChannelManager;
class m4VariantType;
class ClAccessDirectory;
class ClCacheableObject;
class ClLaneArgumnent;
class ClProxyFactory;
class _ClMemoryManager;
class ClCsTraceSize;
class ClGenericIODriver;
class ClNotifier_Channel_Data;
class ClSize;
class ClChannelProcess;


///////////////////////////////////////////////////////////////////////////////
//
// ClChannel_Kernel
//

class M4_DECL_M4DM ClChannel_Kernel : public ClChannelDef
{
public:
    ClChannel_Kernel ();
   ~ClChannel_Kernel ();    
    

    m4return_t Init (void);
    m4return_t Destroy (void);


    //ahora este señor es dueño de su MCR:
    ClMCROwner MCR;

    m4uint32_t GetVersion (void) const {return 0;}

	m4return_t GetSize(ClSize &ao_size);

//-------------------
//Version
    class ClVersion
    {
    public:
        ClVersion () : m_Version (0.0) {}

        m4date_t Get (void) const
        {
            return m_Version;
        }

        void Set (m4date_t ai_version)
        {
            m_Version = ai_version;
        }
		m4return_t GetSize(ClSize &ao_size)
		{
			ao_size+=sizeof(ClVersion);
			return M4_SUCCESS;
		}
        
    private:
        m4date_t m_Version;
    } Version;
//-------------------

};


///////////////////////////////////////////////////////////////////////////////
//
// ClChannel_Data
//

class M4_DECL_M4DM ClChannel_Data : public ClPersistiableObject
{
public:
	ClChannel_Data ();
   ~ClChannel_Data ();

    m4return_t Init (void);
    m4return_t Destroy (m4bool_t ai_bRemoveFromCache /*= M4_FALSE*/);

    ClChannelDef *GetpChannelDef (void) const
    {
        return GetpExtInfo ();
    }
    
    ClChannel_Kernel *GetpExtInfo (void) const
    {
        M4_ASSERT (m_pClChannel_Kernel);
        return m_pClChannel_Kernel;
    }


public:
    ClNodeDirectory_Data            Node;

    m4return_t SetSliceMode (m4bool_t ai_bnewmode);

// El segundo parametro es un parche para que no GP si he destruido el documento nates del carril
    m4return_t ReleaseAll (m4bool_t ai_bresetnodeitems = M4_TRUE, m4bool_t ai_bDestructorCall = M4_FALSE);

    m4return_t InitSecurity (void);

//************************************************************
// FUNCIONES DE MARCAS A NIVEL DE CANAL
//************************************************************

    ClOperation_User                Operation;      //Flags con las operaciones

    void       _NotifyOperation     (m4uint8_t ai_operation);
    m4return_t SetCheckPointMode    (m4bool_t ai_bnewmode);
    m4return_t CheckPoint           (void);
    m4return_t Undo                 (void);
    m4return_t RefreshFlags         (void);
    m4return_t CommitPersisted      (VersionMark &ai_version);
    m4return_t RollbackPersisted    (void);

//************************************************************

// jcr
    m4return_t CheckModify( m4bool_t &ao_bModified ) ;

private:
    ClChannel_Kernel *m_pClChannel_Kernel;    

//******************PLUGINS************************

//-----------------
//Notifier
public:
    //ClNotifier                      Notifier;       //Objeto notificador, funcionalidad de eventos
    ClNotifier_Channel_Data *GetpNotifier () const
    {
        return m_pClNotifier;
    }

    m4uint32_t GetUniqueRowId (void)
    {
        // de momento doy Id = 1 al primer registro
        return ++ m_iUniqueRowId;
    }

    _ClMemoryManager * GetpMemoryManager (void)
    {
        return m_poMemoryManager;
    }

private:
    _ClMemoryManager * m_poMemoryManager;

    ClNotifier_Channel_Data *m_pClNotifier;

    // Identificador unico de registros
    m4uint32_t m_iUniqueRowId;

//-----------------

//-------------------------------------------------
//Serialización
public:
	/**
	 * Serializa un canal en un IODriver.
	 *
	 * @param IOD					IODriver donde se va a hacer la serialización.
	 * @param ai_iRecSerMod			Modo de serializacion de los registros.
	 * @param ai_iIteSerMod			Modo de serializacion de los ítems.
	 * @param ai_pChannelProcess	Procesos que hay que ejecutar antes y despues de la serialización de cada nivel.
	 * @param ai_pCsTraceSize		Para sacar información sobre los tamaños de serialización.
	 * @param ai_iBlockIndex		En caso de estar encarrilado indica el bloque que hay que serializar.
	 * @param ai_iRegisterIndex		En caso de estar encarrilado indica el registro que hay que serializar.
	 * @param ai_iVectorDelta		Índice del vector de deltas que se tiene que utilizar en la serialización.
	 * @return m4return_t.
	 */
	m4return_t Serialize(ClGenericIODriver&      IOD,
						 const m4uint8_t         ai_iRecSerMod,
						 const m4uint8_t         ai_iIteSerMod,
						 ClChannelProcess* const ai_pChannelProcess,
						 ClCsTraceSize*    const ai_pCsTraceSize,
						 const m4uint32_t        ai_iBlockIndex = 0,
						 const m4uint32_t        ai_iRegisterIndex = 0,
						 const m4uint8_t         ai_iVectorDelta = 0);
	
	m4return_t DeSerialize(ClGenericIODriver& IOD);
	
	m4return_t DeSerialize(ClGenericIODriver&   IOD,
						   m4uint8_t&           ai_iIteSerMod,
						   m4uint32_t&          aio_iBlockIndex,
						   m4uint32_t&          aio_iRegisterIndex,
						   ClCsTraceSize* const ai_pCsTraceSize,
						   m4uint8_t	        ai_iVectorDeltaCount);
	
	/**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
	m4return_t SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount);

	m4bool_t IsKeepStatus(void) const;

	// Cuando el canal se serializa indica al otro exrtemo que use deltas o no los use
	void SetKeepStatusOnRemote(const m4bool_t ai_bKeepIt);

	// Para saber lo que le vamos a pedir al remoto
	m4bool_t IsKeepStatusOnRemote(void) const;

	// Nos permite preguntar por la version de serializacion que se esta usando
	static m4uint16_t GetSerializationVersion(void);

	// Serializacion avanzada: nos permite marcar concretamente que es lo que queremos serializar
	void ResetSerialMarks(void);
	m4return_t MarkForPersistSerial(const m4uint32_t ai_iBlockIndex, const m4uint32_t ai_iRegisterIndex);

private:
	// Esta miembro determina si, cuando serializamos, queremos que en el otro extremo se usen deltas
	// (es decir, si queremos que el otro extremo nos devuelva deltas). Por defecto se inicializa a
	// M4_FALSE, de forma que si no se indica lo contrario siempre se trabaja con serializacion absolutos
	m4bool_t m_KeepStatusOnRemote;       
	
	m4bool_t m_KeepStatus;
//-------------------------------------------------


//-------------------------------------------------
//Caches
	public:

		m4return_t GetSize(ClSize &ao_size);

		m4uint32_t	GetSize( void );

		m4date_t    GetStartDate( void ) const
		{
			return( ClMinusInfiniteDate() ) ; 
		}

		m4date_t    GetEndDate( void ) const
		{
			return( ClPlusInfiniteDate() ) ;
		}

		m4date_t    GetCorStartDate( void ) const
		{
			return( ClMinusInfiniteDate() ) ;
		}

		m4date_t    GetCorEndDate( void ) const
		{
			return( ClPlusInfiniteDate() ) ;
		}

		void SetCO( ClCacheableObject * ai_pCO )
		{
			m_pCO = ai_pCO;
		}

		ClCacheableObject*	GetCO( void ) const
		{
			return( m_pCO ) ;
		}

		m4date_t GetVersion( void ) const
		{
			return( ClMinusInfiniteDate() ) ;
		}

	private:
		ClCacheableObject * m_pCO;
		
		
//-------------------------------------------------


//-----------------
//ProxyFactory
public:
    ClProxyFactory * m_pProxyFactory;

    void ForceLock( void );

//-----------------


// ============================================================================
// Carga parcial
// ============================================================================
public:

	m4pcchar_t	GetPLFile( void ) const ;
	m4return_t	CreatePLFile( const ClNodeDef *ai_pcoNodeDef, m4pcchar_t& ao_rpPlFile ) ;

protected:

	// Nombre completo del fichero donde se guardan los registros que salen de scope
	m4char_t	m_acPLFile[ M4_MAX_PATH + 1 ] ;

// ============================================================================



//*************************************************
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, ClChannel_Data &ai_channel);

#endif  //__CHAN_KNL__HPP__ 
