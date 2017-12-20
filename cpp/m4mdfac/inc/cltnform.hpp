
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnform.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary formula of a nitem
//
//
//==============================================================================


#include "m4mdfac_dll.hpp"
#include "mdfacglb.hpp"

#include "sliarray.hpp"
#include "m4mdrtde.hpp"
#include "cldefine.hpp"
#include "vmmcrdef.hpp"


#ifndef __CLTNFORM_HPP__
#define __CLTNFORM_HPP__


class ClTempNitem;
class ClTempNode;
class ClTempTi ;
class ClTempItemFormula ;
class ClCompiledMCR;	//Only for JIT
class ClInstruction ;
class IFuncTableDesc ;
class IAtributeFuncTable ;
class ClAccessRecordSet ;
class ClMdStatistics ;

#include "m4stl.hpp"
typedef vector<m4int32_t> VectorLabels;
typedef VectorLabels::iterator itVectorLabels;



// ============================================================================

// Tipos de regla según su creacion
#define	M4_MDFAC_RULE_CREATION_NORMAL						0
#define	M4_MDFAC_RULE_CREATION_TOTAL						1
#define	M4_MDFAC_RULE_CREATION_SYSTEM_METARULE				2
#define	M4_MDFAC_RULE_CREATION_SYS_LOAD						3
#define	M4_MDFAC_RULE_CREATION_SYS_PERSIST					4
#define	M4_MDFAC_RULE_CREATION_EXECUTEREPORT				5

#define M4_MDFAC_MAX_DIGITS_INT_SIDE						9


#define M4CL_VM_EMB_INS_READCOMMENT							0
#define M4CL_VM_EMB_INS_INCOND_JUMP							1
#define	M4CL_VM_EMB_INS_COND_JUMP							2
#define	M4CL_VM_EMB_INS_ASSIGN								3
#define	M4CL_VM_EMB_INS_READ								4
#define	M4CL_VM_EMB_INS_PURGE								5
#define	M4CL_VM_EMB_INS_RETURN								6
#define	M4CL_VM_EMB_INS_ASSIGN_VAR_ARGS						7
#define	M4CL_VM_EMB_INS_READ_VAR_ARGS						8

//Explicación de los 16 primeros bytes de una instruccion embebida
//01234567 89abcdef
//0: 1->embebida
//1,2,3,4: Instrucción ( segun la siguente tabla)
//5:1->Variant int, 0=Variant Double en intrucciones variant
//6:1->Instrucción simple embedida
//7:Nivel (0,1)
//Los bytes 16-31 en las de control enbebidas es el argumento, en las simples, el nº de función


const m4uint8_t g_aiFuncToFlag[]={

	(M4CL_VM_EMB_INS_READCOMMENT<<3)		,//M4CL_VM_FT_READCOMMENT
	(M4CL_VM_EMB_INS_INCOND_JUMP<<3)		,//M4CL_VM_FT_INCONDITIONALJUMP
	(M4CL_VM_EMB_INS_COND_JUMP<<3)			,//M4CL_VM_FT_CONDITIONALJUMP
	(M4CL_VM_EMB_INS_ASSIGN<<3)				,//M4CL_VM_FT_ASSIGNVAR
	(M4CL_VM_EMB_INS_READ<<3)				,//M4CL_VM_FT_READVAR
	(M4CL_VM_EMB_INS_PURGE<<3)				,//M4CL_VM_FT_PURGESTACK
	(M4CL_VM_EMB_INS_RETURN<<3)				,//M4CL_VM_FT_RETURN
	(M4CL_VM_EMB_INS_ASSIGN_VAR_ARGS<<3)	,//M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM
	(M4CL_VM_EMB_INS_READ_VAR_ARGS<<3)		//M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM

	//Los OR: 80->1000 0000 -> Instrucción enbebida, Nivel 0, instr
	//		  81->1000 0001 -> Instrucción enbebida, Nivel 1, instr
};



//                   ========================================
// formulas de nitem ========================================
//					 ========================================



class	M4_DECL_M4MDFAC	ClTempNitemFormula : public ClSlicedElement
{

public:

	// Funciones de inicialización
	
		ClTempNitemFormula( void ) ;
		~ClTempNitemFormula( void ) ;

	m4return_t	SetPolish( m4pchar_t ai_pcPolish ) ;

	m4return_t	Init( ClTempNitem *ai_poNitem, ClTempItemFormula *ai_poItemFormula, m4uint8_t ai_iLevel ) ;
	m4return_t	Init( ClTempNitem *ai_poNitem, ClTempNode *ai_poNode, m4int8_t ai_iType, m4pchar_t ai_pcPolish, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccMethod, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4uint8_t ai_iCreationType, m4uint8_t ai_iLevel ) ;


	void	Clear( void ) ;

	m4return_t	InstallLN4			(void);
	m4return_t	PreInstallLN4		(void);
	m4return_t	PreInstallCPP		(void);
	m4return_t	PreinstallRemote	(void);
	m4return_t	PreinstallExternal	(void);

	const ClInstruction	*GetInstructionsList( void ) const {
		return m_pInstructions;
	}
	m4uint16_t			GetNumVars(void) const {
		return m_iNumLocalVars;
	}
	m4int8_t			GetType(void) const {
		return m_iType;
	}
	m4int8_t			GetOrigin(void) const {
		return m_iOrigin;
	}

//////////////////////////JIT
	m4return_t CountInstructionsLN4forJIT( m4pchar_t ai_pszPolish,m4int32_t &ao_iNumInstruct);
	void InitforJIT( IFuncTableDesc *ai_poFunctionTable, IAtributeFuncTable *ai_oAtributeMethodTable, m4uint32_t ai_hItem, m4uint8_t ai_iLevel, ClAccessRecordSet *ai_poRecordSet,ClCompiledMCR *ai_poCMCR,m4uint8_t ai_iType,m4uint8_t ai_iPriority,m4date_t ai_dStart,m4date_t ai_dEnd);

/////////////////////////////

   	void    SetMustReturn( m4uint8_t ai_iMustReturn )
    {
        m_iMustReturn = ai_iMustReturn ;
    }

   	m4uint8_t   GetMustReturn( void ) const
    {
        return( m_iMustReturn ) ;
    }

   	m4uint8_t   GetPriority( void ) const
    {
        return( m_iPriority ) ;
    }

	m4uint8_t   GetIsMetarule( void ) const
    {
		return( m_iIsMetarule ) ;
    }

    m4uint8_t   GetIsRetroactivity( void ) const
    {
		return( m_iIsRetroactivity ) ;
    }

    m4uint8_t   GetBatchType( void ) const
    {
		return( m_iBatchType ) ;
    }

    m4uint8_t	GetIsEvent( void ) const
    {
		return( m_iIsEvent ) ;
    }

    m4int32_t   GetDBOrder( void ) const
    {
		return( m_iDBOrder ) ;
    }

	m4bool_t	GetIsDatabase( void ) const
	{
		return( m_bIsDatabase ) ;
	}

    m4uint8_t   GetCreationType( void ) const
    {
		return( m_iCreationType ) ;
    }

	m4pchar_t	GetPolish( void ) const
	{
		return( m_pcPolish ) ;
	}


	m4return_t	GenerateConnectingCodePre	(m4pchar_t ao_pszConnectingCodePre , m4int32_t ai_BufferSize);
	m4return_t	GenerateConnectingCodePost	(m4pchar_t ao_pszConnectingCodePost , m4int32_t ai_BufferSize);
	
	
	//------------------------------------------------------------------------
	//Interfaz para Runtime Generated Code, como por ejemplo los executeGroup
	//------------------------------------------------------------------------
	m4return_t	SetInstructionsRTGC (ClInstruction *ai_pInstructions, m4uint32_t ai_iNumInstructions,  m4uint32_t *ai_piInsTypes, m4uint32_t ai_iInsTypesNum);
	void		SetInfoRTGC			(m4uint16_t ai_iNumLocalVars, m4int8_t ai_iType,	m4int8_t ai_iOrigin, m4int8_t ai_iMustReturn, m4int8_t ai_iPrior){
					m_iNumLocalVars	=ai_iNumLocalVars;
					m_iType			=ai_iType;
					m_iOrigin		=ai_iOrigin;
					m_iMustReturn	=ai_iMustReturn;
					m_iPriority		=ai_iPrior;
	};

	void		ResetInstructionsRTGC (void){
		m_pInstructions=0;
		m_NumInstructions=0;
	}

	//Utiles diversos
	static	m4pcchar_t GetNextLine( m4pcchar_t ai_pszBuffer);
	
// Para optimizar tiempos

	static	m4uint32_t	_ReadPositive( m4pcchar_t ai_pccString, m4char_t ai_cChar )
	{

		m4uint32_t	iResult ;

		if( *( ai_pccString + 1 ) == ai_cChar || *( ai_pccString + 1 ) == '\r') //lo del \r es para Tom C. Mulledy
		{
			iResult = *ai_pccString - '0' ;
		}
		else if( *( ai_pccString + 2 ) == ai_cChar || *( ai_pccString + 2 ) == '\r')
		{
			iResult = *( ai_pccString + 1 ) - '0' + ( *ai_pccString - '0' ) * 10 ;
		}
		else if( *( ai_pccString + 3 ) == ai_cChar || *( ai_pccString + 3 ) == '\r')
		{
			iResult = *( ai_pccString + 2 ) - '0' + ( *( ai_pccString + 1 ) - '0' ) * 10 + ( *ai_pccString - '0' ) * 100 ;
		}
		else
		{
			iResult = atoi( ai_pccString ) ;
		}

		return( iResult ) ;
	}

private:	

	static	m4int32_t	_ReadNegative( m4pcchar_t ai_pccString, m4char_t ai_cChar )
	{

		m4int32_t	iResult ;
		m4pcchar_t	pccString ;

		if( *ai_pccString == '-' )
		{
			pccString = ai_pccString + 1 ;
		}
		else
		{
			pccString = ai_pccString ;
		}

		iResult = _ReadPositive( pccString, ai_cChar ) ;
		
		if( *ai_pccString == '-' )
		{
			iResult = -iResult ;
		}

		return( iResult ) ;
	}


	static	m4uint32_t	_ReadString( m4pcchar_t ai_pccString, m4char_t ai_cChar, m4pchar_t ao_pcOut )
	{
		m4uint32_t	iResult ;
		m4char_t	c ;

		iResult = 0 ;
		while( ( c = *ai_pccString++ ) != '\n' && c != ai_cChar && c != '\r')
		{
			*ao_pcOut++ = c ;
			iResult++ ;
		}
		*ao_pcOut = '\0' ;

		return( iResult ) ;
	}
	
	static	m4return_t _CheckDouble ( m4pcchar_t ai_pccString, m4bool_t &ao_bDouble){
		
		ao_bDouble=M4_FALSE;
		m4uint32_t iDigitosParteEntera=0;

		
		while(*ai_pccString!='.' && *ai_pccString!='\n'){
			ai_pccString++; //buscamos el . o el final
			iDigitosParteEntera++;
		}
		if (iDigitosParteEntera>M4_MDFAC_MAX_DIGITS_INT_SIDE){
			ao_bDouble=M4_TRUE;
		}
		else{
			if (*ai_pccString++=='.'){	//si es punto
				while (*ai_pccString!='\n') { //Hasta el final
					if (*ai_pccString!='0'){
						ao_bDouble=M4_TRUE;
						break;
					}
					ai_pccString++;
				}
			}
		}
		return M4_SUCCESS;
	}

	m4return_t			GenerateInstructionPush			(m4pchar_t ao_pszConnectingCode , m4int32_t ai_iNumVar);
	m4return_t			GenerateInstructionPushNum		(m4pchar_t ao_pszConnectingCode , m4int32_t ai_iNum);
	m4return_t			GenerateInstructionPushString	(m4pchar_t ao_pszConnectingCode , m4pcchar_t ai_string);
	m4return_t			GenerateInstructionFunctionCall	(m4pchar_t ao_pszConnectingCode,  m4pcchar_t i_pszFuncName);
	m4return_t			GenerateInstructionPurge		(m4pchar_t ao_pszConnectingCode, m4uint32_t ai_iNumPurges);
	m4return_t			GenerateInstructionAssign		(m4pchar_t ao_pszConnectingCode, m4int32_t ai_iNumVar);
	m4return_t			GenerateInstructionReturn		(m4pchar_t ao_pszConnectingCode);
	m4return_t      	GenerateInstructionItemLevelCall(m4pchar_t ao_pszConnectingCode);
	void				PreGetNumVar				( m4pcchar_t ai_Polish);
	

	
//JIT
	ClCompiledMCR		*m_poCMCRforJIT;
	ClAccessRecordSet	*m_poRecordSet;
	m4return_t			GetNItemHandleforJIT( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId,m4uint32_t &ao_iItemHandle) const ;
//
	ClInstruction		*m_pInstructions;
	m4uint16_t			m_iNumLocalVars;
	m4int8_t			m_iOrigin;
	IFuncTableDesc		*m_poFunctionTable;
	IAtributeFuncTable	*m_oAtributeMethodTable; //tabla de atributos-metodo que existen
	m4bool_t			m_bInstalled; 
	m4int32_t			m_NumInstructions;
	m4int32_t			m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES];
	m4uint32_t			m_iNumVariantsAsInt;
	m4char_t			*m_pszTotalPolish;

	m4uint8_t			m_iMustReturn ;
	m4uint8_t			m_iPriority ;
	m4bool_t			m_bRemote;

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

	ClTempTi			*m_poTi ;
	m4char_t			m_acFormulaId[ M4CL_MAX_NITEM_FORMULA_ID + 2 ] ;
	
	m4int8_t			m_iType;
	m4uint8_t			m_iLevel ;
	m4uint8_t			m_iIsMetarule ;
	m4uint8_t			m_iIsRetroactivity ;
	m4uint8_t			m_iBatchType ;
	m4uint8_t			m_iIsEvent ;
	m4int32_t			m_iDBOrder ;
	m4bool_t			m_bIsDatabase ;

	m4pcchar_t		    m_pccObject ;
    m4pcchar_t          m_pccMethod ;   //nombre de func en c++
	m4pchar_t		    m_pcPolish ;	//polaca en Ln4,
	m4bool_t			m_bPolishOwner ;
	m4date_t		    m_dStartDate ;
	m4date_t		    m_dEndDate ;
	m4uint32_t			m_iIdHandle ;

	m4uint8_t			m_iCreationType ;

    m4uint16_t			m_iOrder ;
	m4uint32_t			m_hItemJIT;
    ClTempNitem			*m_poFormulaNitem ;

    m4uint8_t			m_iIsToCompile ;

 
	
	VectorLabels		m_aiLabels;
	m4uint32_t			m_iLabelsSize;


	m4return_t	InstallObject		( m4pcchar_t ai_pszString, ClInstruction *ao_oIntruction);
	m4return_t	ScanLine			( m4pcchar_t ai_pszString, ClInstruction *ao_oInstruccion, m4bool_t ai_bNeedRule);
	m4return_t  CountInstructionsLN4( m4pcchar_t i_Polish);
	
	//información sobre items, nodos...

    m4return_t  GetNitem( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, ClTempNitem* &ai_rpoNitem ) const ;
    m4return_t  GetNitem( ClTempNode* ai_poNode , m4pcchar_t ai_pccItemId, ClTempNitem* &ai_rpoNitem ) const ;

    m4return_t  GetNode( m4pcchar_t ai_pccNodeId, ClTempNode* &ai_rpoNode ) const ;
    m4return_t  GetRuleOrder( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, m4pcchar_t ai_pccRuleId, m4bool_t ai_bFather, m4uint16_t &ao_riOrder ) const ;
	m4return_t  GetRuleOrder( ClTempNitem* ai_poNitem , m4pcchar_t ai_pccRuleId, m4bool_t ai_bFather, m4uint16_t &ao_riOrder ) const ;
    
	m4return_t  GetThisChannelIdHandle( m4uint32_t &ao_riChannelId ) const ;
    m4return_t	GetNodeIdHandle		( m4pcchar_t ai_pccNodeId, m4uint32_t &ao_riHandle ) const ;
	m4return_t	GetNodeAndIdHandle	( m4pcchar_t ai_pccNodeId, ClTempNode* &ai_rpoNode, m4uint32_t &ao_riHandle ) const ;
	m4return_t  GetT3AliasInstance( m4pcchar_t ai_pccT3Alias, m4pcchar_t &ao_pccT3AliasInstance ) const;
	m4return_t  GenerateExternMethodCode(void);
    void		IncrementConditionantsDelta( void ) ;
    m4return_t  AddConditionant( ClTempNitem *ai_poConditionant ) ;
	m4return_t  AddAssignedItem( ClTempNitem *ai_poConditionant ) ;
	void        IncrementAssignedItemDelta ( void ) ;
    m4return_t  AddExecutedConcept( ClTempNitem *ai_poConditionant );
    void        IncrementExecutedConceptDelta( void );
    
	m4pcchar_t  GetThisNodeId( ClTempNode* ai_poNode=0) const;
	m4pcchar_t  GetThisChannelId( void ) const;
	m4pcchar_t  GetThisItemId (void) const;

	m4bool_t	HasLN4VariableArgs(void);

	// Tamaño de la Formula Instalada
	m4uint32_t	_GetFormulaSize( void ) ; 
	m4return_t	CheckCppDefinition (void) ;
	
	m4return_t  _PreinstalConnectors	( m4pcchar_t ai_pccMidCode, m4bool_t ai_bConvertToLN4Always, m4bool_t ai_bRePushVars ) ;

	m4return_t	PreinstallUpperLevels	(void);
	m4return_t	PreinstalInternalLevels	(void);

	m4return_t  CheckForRemoteExecution	(m4bool_t &ao_bRemote);
	m4return_t  RePushVars				(m4pchar_t ai_pszMidCode);

	void		DumpError				(m4uint32_t ai_errorCode, m4pcchar_t ai_pccMessage1, m4pcchar_t ai_pccMessage2 = NULL, m4bool_t ai_bShowItem = M4_TRUE) const;
	void		DumpDebugInfo			(m4uint32_t ai_errorCode, m4pcchar_t ai_pccMessage) const;

	m4uint8_t	CalcDatesByte(void);


public:

// Funciones de escritura

    void    SetOrder( m4uint16_t ai_iOrder )
    {
        m_iOrder = ai_iOrder ;
    }

    void    SetIdHandle( m4uint32_t ai_iIdHandle )
    {
        m_iIdHandle = ai_iIdHandle ;
    }

	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetIsToCompile( void )
    {
        return( m_iIsToCompile ) ;
    }

// Funciones de lectura

	m4date_t	GetStartDate( void ) const
	{
		return( m_dStartDate ) ;
	}

	m4date_t	GetEndDate( void ) const
	{
		return( m_dEndDate ) ;
	}

	m4uint32_t	GetIdHandle( void ) const
	{
		return( m_iIdHandle ) ;
	}
		
	m4int16_t	GetOrder( void ) const
	{
		return( m_iOrder ) ;
	}
    

// Serialización
	m4uint32_t	GetHandle( void ) const
	{
		return( m_iHandle ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

	void	SetNoUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poNoUpperPool = ai_poPool ;
	}

	m4return_t  SetFormulaId( m4pcchar_t ai_pccFormulaId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccFormulaId, m_acFormulaId, M4CL_MAX_NITEM_FORMULA_ID, ai_bConvert ) ) ;
	}

	m4pcchar_t  GetFormulaId( void ) const
	{
		return( m_acFormulaId + 1 ) ;
	}


	m4return_t	PreGetSize( void ) ;
    m4return_t	GetSize   ( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile   ( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


// Funciones varias ===========================================================

	m4bool_t	IsEqual( ClSlicedElement *ai_poElement ) ;
	m4return_t	CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo ) ;
	m4return_t	CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo ) ;

} ;





#endif


