 //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         clc
// File:           VMAttrib.h 
// Project:        clc
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Declaración de la clase VMAttribute que contiene las funciones de acceso a las propiedades
////
//==============================================================================



#ifndef _ClVMAttribute_H
#define _ClVMAttribute_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "chlog.hpp"

class ClAccess;
class ClValue;
class ClOldValue;
class ClItem;



class ClVMachine0;

class ClVMItemAttribute :public ClVVM{


public:

     ClVMItemAttribute (void){ 
        _Init();
    }


	//Todo el contexto necesario. 
	void SetContext ( ClAccess *ai_poAccess, m4uint16_t ai_iFlags, m4uint32_t ai_hNodo, m4uint32_t ai_hItem, 
		              m4bool_t ai_bRunSliceMode, m4double_t ai_dBeginDate, m4double_t ai_dEndDate){

		m_poAccess = ai_poAccess;
		m_iFlags = ai_iFlags;
		m_hItem = ai_hItem;
        m_hNodo = ai_hNodo;  
		m_bRunSliceMode = ai_bRunSliceMode;
		m_dBeginDate = ai_dBeginDate;
		m_dEndDate = ai_dEndDate;
	}



    //get/run
    m4return_t  Get(m4uint8_t ai_iNumAttrib){
        if (ai_iNumAttrib >= M4CL_NUM_ATT_GET){ //por los atrib indirectos
			return M4_ERROR; 
		}
        //DEBUG_CHECK_RES( ai_iNumAttrib < M4CL_NUM_ATT_GET, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, LOG_CAT_BLOCK(__FILE__<<__LINE__<<"Invalid attribute number") );
        return (this->*(m_apfGet[ai_iNumAttrib ]) ) () ;
    }
    m4return_t  Run(m4uint8_t ai_iNumAttrib){
        if (ai_iNumAttrib >= M4CL_NUM_ATT_RUNNABLE){
			return M4_ERROR;
		}
        //DEBUG_CHECK_RES(ai_iNumAttrib < M4CL_NUM_ATT_RUNNABLE, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, LOG_CAT_BLOCK(__FILE__<<__LINE__<<"Invalid attribute number") );
        return (this->*(m_apfRun[ai_iNumAttrib]) ) ();
    }

private:
    
    void _Init(void);
    //lecture de atributos 
    m4return_t GetSysSliceNumber     (void);
    m4return_t GetSysStartDate       (void);
    m4return_t GetSysEndDate         (void);
    m4return_t GetSysFirstSlice      (void);
    m4return_t GetSysLastSlice       (void);
    m4return_t GetSysOldValue		 (void);
    m4return_t GetSysBlobDescription (void);
    m4return_t GetSysBlobMask	     (void);
	m4return_t GetSysAuxItemId	     (void);

    //Atributos-método
    m4return_t SysAddNewSlice       (void);
    m4return_t SysGetItemName       (void);
    m4return_t SysCreateSlice       (void);
    m4return_t SysSplitSlice        (void);
	m4return_t SysTotalizeItems		(void);
	m4return_t SysTotalizeSlices	(void);
	m4return_t SysSetBlobDescription(void);
	m4return_t SysCreateBlobFile	(void);



    //auxiliares
    virtual m4return_t GetItemValue     (ClValue* &ao_poValue, m4bool_t &ao_bRow, m4bool_t &ao_bSlice, ClItem* &ao_rpoItem);
    virtual m4return_t GetItemOldValue  (ClOldValue* &ao_poValue, m4bool_t &ao_bRow, m4bool_t &ao_bSlice);
    virtual m4return_t GetItem          (ClItem* &ao_poItem, m4bool_t ai_bPositioned);

	/*
	Necesitamos	(para VMAtrib y otras cosas):

		Access del nuevo canal   v
		NumItem (Handle)         desde CurrentInst
		CurrentInst              v

  		Flags					 desde CurrentInst
		NumAtrib				 desde CurrentInst

		PilaDatos				 desde m_ de VM0
		Mediator				 desde m_ de VM0

	Además devolvemos (para VM2 exclusivamente):

		HandleItem a Ejecutar    v
	*/

    ClAccess    *m_poAccess;
	m4uint32_t m_hItem;
    m4uint32_t m_hNodo;

	m4bool_t m_bRunSliceMode;
	m4double_t m_dBeginDate;
	m4double_t m_dEndDate;
	m4int16_t  m_iFlags;

    
	//ojo con el typedef->Ver comentario en vmtypes.hpp (No meter funciones virtulaes en esta clase)
	//o cambiar el typedef.
    pmfAttrib  m_apfRun[M4CL_NUM_ATT_RUNNABLE];
    pmfAttrib  m_apfGet[M4CL_NUM_ATT_GET];
};


#endif

