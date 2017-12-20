//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cltotacum.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//		Class used to totalize in Acummulate TIs   
//
//
//==============================================================================
#ifndef __CLTOTACUM_HPP__
#define __CLTOTACUM_HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "totalize.hpp"
#include "valh_rs.hpp"
#include "index.hpp"
#include "register.hpp"
#include "vectvar.hpp"
#include "m4var.hpp"
#include "dmtypes.hpp"

class ClRgAc_Base;
class ClRegister;
class ClRgAc;


typedef ClTotalize <ClRgAc_Base, ClVectorVariants, ClGetValue>				ClAcumTITotalizer_Base;

typedef struct stIndexAndTotType_tag
{
	ClItemIndex m_ItemIndex;
	m4char_t m_TotType;
	stIndexAndTotType_tag ();
	stIndexAndTotType_tag (ClItemIndex ai_II,m4char_t ai_TT ) : m_ItemIndex (ai_II), m_TotType (ai_TT) {};
}stIndexAndTotType_t;


typedef vector <stIndexAndTotType_t>										VIndAndTotType;



class ClRgBoundedAc{
public:
	//contructores
	ClRgBoundedAc(ClRegister *ai_poRegister, m4uint8_t ai_uiTotType);
	ClRgBoundedAc(const ClRgBoundedAc &ai_roRgBoundedAc);


	//para el cálculo de los válidos para iteras
	m4return_t	Init(ClItemIndex ai_idxFirst, ClItemIndex ai_idxSecond, ClRegisterIndex ai_iFirstRecord,
					ClItemIndex ai_iStartDateItemIndex, ClItemIndex ai_iEndDateItemIndex);
	void		Reset(void);

	// Funciones para Totalizacion
	virtual m4bool_t	Begin(void);
	virtual m4bool_t	End(void);
	virtual m4bool_t	Next(void);
	virtual m4uint32_t	Count(void);
	virtual m4bool_t	MoveTo(const m4uint32_t ai_iIndex);
	virtual m4uint8_t	GetTotType(void) { return m_uiTotType; }

	//para leer los valores
	m4return_t MoveItem(ClItemIndex ai_idxItem);
	m4return_t Get(m4VariantType &ao_vValue);
	m4return_t GetWeightedValue(m4VariantType &ao_vValue);

	m4uint32_t Index;

protected:
	ClRegisterIndex m_idxStart, m_idxEnd;
	ClRegister		*m_poRegister;
	m4uint8_t 		m_uiTotType;
	ClItemIndex		m_idxItem;

	//Índices de los elementos fecha inicio, fecha fin del registro.
	ClItemIndex		m_iStartDateItemIndex; 
	ClItemIndex		m_iEndDateItemIndex; 

	//Divisor para ponderar valores en una totalización de tipo suma ponderada
	m4date_t		m_dDivider;

};
	
typedef ClTotalize <ClRgBoundedAc, m4VariantType, ClGetValue>		ClRgBoundedTotalizer;


class M4_DECL_M4DM ClRgAc_Base{

public:
	ClRgAc_Base(ClRegister *ai_poRegister,StFunctionCmp_t * ai_pfcomp,VIndAndTotType *ai_ItemIndex,
				m4int32_t ai_iNumElem,m4uint8_t ai_TotType,ClItemIndex ai_idxFirst, ClItemIndex ai_idxSecond, 
				ClItemIndex ai_iStartDateItemIndex, ClItemIndex ai_iEndDateItemIndex, 
				ClRegisterIndex ai_Index);
	ClRgAc_Base(ClRgAc_Base *ai_pRgAc_Base);

	// Funciones para Totalizacion
	virtual m4bool_t	Begin(void);
	virtual m4bool_t	End(void);
	virtual m4bool_t	Next(void);
	virtual m4uint32_t	Count(void) { return 0; }
	virtual m4uint8_t	GetTotType(void) { return m_uiTotType; }
	virtual m4bool_t	MoveTo(const m4uint32_t ai_iIndex);


	void		UpdateTotType (m4uint8_t ai_Tot){m_uiTotType = ai_Tot;}
	m4return_t	GetValueByIndexPos (ClVectorVariants &ao_vTotVal);

	m4uint32_t Index;

	m4int32_t GetNumRows () { return m_paItemIndex->size(); }
	void SetMustStop () { m_bMustStop = M4_TRUE ;}
	m4bool_t FoundRegs() {return m_bAnyRegisterFound;}

protected :
	m4bool_t		_CheckReg(m4bool_t &ao_bStop);
	ClRegister		*m_poRegister;
	m4uint8_t 		m_uiTotType;

	ClRgBoundedAc   m_oBoundedIterator;
	ClItemIndex		m_idxFirst, m_idxSecond;

	//Índices de los elementos fecha inicio, fecha fin del registro.
	ClItemIndex		m_iStartDateItemIndex; 
	ClItemIndex		m_iEndDateItemIndex; 

private:
	StFunctionCmp_t	*m_pstfcomp;
	m4VariantType	m_vVariantTemp; //por si tenemos que parar antes de llegar al final ( emula el group by)
	
	m4bool_t		m_bMustStop ;
	VIndAndTotType	*m_paItemIndex;	//array de Items a totalizar, vienen dados por el índice.
	m4int32_t		m_iNumElemIndex;
	m4bool_t		m_bAnyRegisterFound;
};





typedef ClTotalize <ClRgAc, ClVectorVariants, ClGetValue>  ClAcumTITotalizer;


class M4_DECL_M4DM ClRgAc : public ClRgAc_Base
{


public:
	ClRgAc(ClAcumTITotalizer_Base *ai_ptotal,ClRgAc_Base * ai_pRgAc_Base,m4uint8_t ai_TotType)
		: ClRgAc_Base (ai_pRgAc_Base)
	{
		m_uiTotType = ai_TotType;
		m_ptotal = ai_ptotal;
	}

	//// Funciones para totalización.
	m4bool_t		Begin(){ return M4_TRUE; }
	m4bool_t		End() { return M4_TRUE; }
	m4bool_t		MoveTo(const m4uint32_t ai_iIndex) { return M4_TRUE; }
	m4bool_t		Next();
	m4uint32_t		Count(void);

	ClAcumTITotalizer_Base	*m_ptotal;


private:

};
/////////////  Lo pongo aquí porque sólo se utiliza para las TIs de Acumulado.
/////////////  Tiene un carácter separador, particular para este caso.

int M4_DECL_M4DM M4SearchVarStringInVarString(const m4VariantType *vTemp1, const m4VariantType *vTemp2, void* ai_pvAux = 0);

#endif
