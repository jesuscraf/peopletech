//## begin module%3912A6BD0238.cm preserve=no
//## end module%3912A6BD0238.cm

//## begin module%3912A6BD0238.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A6BD0238.cp

//## Module: vmmcrdef%3912A6BD0238; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\vmmcrdef.hpp

#ifndef VMMCD_DEF_HPP
#define VMMCD_DEF_HPP 1

//## begin module%3912A6BD0238.additionalIncludes preserve=no
//## end module%3912A6BD0238.additionalIncludes

//## begin module%3912A6BD0238.includes preserve=yes
//## end module%3912A6BD0238.includes

//## begin module%3912A6BD0238.declarations preserve=no
//## end module%3912A6BD0238.declarations

//## begin module%3912A6BD0238.additionalDeclarations preserve=yes
//## end module%3912A6BD0238.additionalDeclarations


//## begin module%3912A6BD0238.epilog preserve=yes


//Tabla de Funciones cond Id's "cableados" -> ClFuncTable::InitLN4Semantic (tablfunc.cpp)
#define M4CL_VM_FT_LOGICALNOT						2		//"LogicalNot"
#define M4CL_VM_FT_LOGICALGREATEREQUAL				6		//"LogicalGreaterEqual"

#define M4CL_VM_FT_READVAR							14		//"ReadVar"
#define M4CL_VM_FT_PUSHNUM							15		//"PushNum"
#define M4CL_VM_FT_PUSHSTRING						16		//"PushString"
#define M4CL_VM_FT_PUSHDATE							17		//"PushDate"
#define M4CL_VM_FT_PURGESTACK						18		//"PurgeStack"
#define M4CL_VM_FT_ASSIGNVAR						19		//"AssignVar"
#define M4CL_VM_FT_CONDITIONALJUMP					20		//"ConditionalJump"
#define M4CL_VM_FT_INCONDITIONALJUMP				21		//"InconditionalJump"
#define M4CL_VM_FT_RETURN							22		//"Return"
#define M4CL_VM_FT_EXECUTEITEM						23		//"ExecuteItem"
#define M4CL_VM_FT_ASSIGNITEM						24		//"AssignItem"
#define M4CL_VM_FT_PUSHITEM							25		//"PushItem"
#define M4CL_VM_FT_READATTRIBUTE					26		//"ReadAttribute"
#define M4CL_VM_FT_EXECUTEATTRIBUTE					27		//"ExecuteAttribute"
#define M4CL_VM_FT_EXECUTEITEM2						28		//"ExecuteItem2"
#define M4CL_VM_FT_ASSIGNITEM2						29		//"AssignItem2"
#define M4CL_VM_FT_PUSHITEM2						30		//"PushItem2"
#define M4CL_VM_FT_READATTRIBUTE2					31		//"ReadAttribute2"
#define M4CL_VM_FT_EXECUTEATTRIBUTE2				32		//"ExecuteAttribute2"
#define M4CL_VM_FT_EXECUTECHANNELFUNCTION2			33		//"ExecuteChannelFunction2"

#define M4CL_VM_FT_INCREMENT						235		//"Increment"

#define M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM	286		//"AssignFixedArgumentInVarArgsItem"
#define M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM	287		//"ReadFixedArgumentInVarArgsItem"

#define M4CL_VM_FT_READCOMMENT						307		//"ReadComment"



//compiler
#define M4CL_MAX_LINEA_POLACA		256
#define M4CL_NO_SYMBOL				-120
#define M4CL_MAX_LEN_ITEM			100
#define M4CL_MAX_LEN_NODE			100
#define M4CL_MAX_LEN_RULE			30
#define M4CL_MAX_LEN_CHANNEL		30
#define M4CL_NO_ATRIBUTE_METHOD		-1
#define M4CL_NO_ATRIBUTE			-1
#define M4CL_INIT_MAX_LABELS		16




#define M4CL_CHANNEL_FUNC			0
#define M4CL_SEMANTIC_FUNC			1
#define M4CL_INTERNAL_FUNC			2
#define M4CL_ATRIBUTE_FUNC			3
#define M4CL_PROPAGATION_FUNC		4
#define M4CL_SEMANTIC_FUNC1			5
#define M4CL_SEMANTIC_FUNC2			6



//**************Flags de las instrucciones (16 bits)
#define M4CL_FLAG_TRAME					0x0001	//0000 0000 0000 0001
#define M4CL_FLAG_BIT_START_LEVEL		1
#define M4CL_FLAG_LEVEL_MASK			0x003E	//0000 0000 0011 1110 es decir 5 bits para el nivel
#define M4CL_FLAG_ROW					0x0040	//0000 0000 0100 0000
#define M4CL_FLAG_FATHER				0x0080	//0000 0000 1000 0000
#define M4CL_FLAG_ITEM					0x0100	//0000 0001 0000 0000

//**************Flags de los estados (16 bits)
#define M4CL_FLAG_STATE_PURGE			0x0001
#define M4CL_FLAG_STATE_CONCEPT			0x0002
#define M4CL_FLAG_STATE_COMPACT			0x0004
#define M4CL_FLAG_STATE_PURGE_REF		0x0008
#define M4CL_FLAG_STATE_HIDE			0x0010
#define M4CL_FLAG_STATE_MM_POSITION		0x0020
#define M4CL_FLAG_STATE_INIT			0x0040
#define M4CL_FLAG_STATE_NOT_EXE			0x0080
#define M4CL_FLAG_STATE_REFRESH			0x0100
#define M4CL_FLAG_STATE_EXE_JIT			0x0200	//**************Flags del compilador JIT
#define M4CL_FLAG_STATE_PREP_JIT		0x0400
#define M4CL_FLAG_STATE_NO_INSITE		0x0800
#define M4CL_FLAG_STATE_NO_INSITE_REC	0x1000

			
//macros de interrogación sobre los flags

#define M4CL_HAS_TRAME_FLAG(x)			((x) & M4CL_FLAG_TRAME)
#define M4CL_SET_TRAME_FLAG(x)			( (x)=((x) | M4CL_FLAG_TRAME) )
#define M4CL_RESET_TRAME_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_TRAME) ) )

#define M4CL_HAS_ROW_FLAG(x)			((x) & M4CL_FLAG_ROW)
#define M4CL_SET_ROW_FLAG(x)			( (x)=((x) | M4CL_FLAG_ROW) )
#define M4CL_RESET_ROW_FLAG(x)			( (x)=((x) & (~M4CL_FLAG_ROW) ) )

#define M4CL_HAS_FATHER_FLAG(x)			((x) & M4CL_FLAG_FATHER)
#define M4CL_SET_FATHER_FLAG(x)			( (x)=((x) | M4CL_FLAG_FATHER) )
#define M4CL_RESET_FATHER_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_FATHER) ) )

#define M4CL_HAS_ITEM_FLAG(x)			((x) & M4CL_FLAG_ITEM)
#define M4CL_SET_ITEM_FLAG(x)			( (x)=((x) | M4CL_FLAG_ITEM) )
#define M4CL_RESET_ITEM_FLAG(x)			( (x)=((x) & (~M4CL_FLAG_ITEM) ) )

#define M4CL_SET_LEVEL_FLAG(x,i)		((x)=((x) & (~M4CL_FLAG_LEVEL_MASK) )  |  (M4CL_FLAG_LEVEL_MASK& (m4int16_t(i<<M4CL_FLAG_BIT_START_LEVEL)) ) )
#define M4CL_GET_LEVEL_FLAG(x)			((M4CL_FLAG_LEVEL_MASK &  x)>> M4CL_FLAG_BIT_START_LEVEL )
#define M4CL_IS_LEVEL0_FLAG(x)			(!(M4CL_FLAG_LEVEL_MASK &  x))

//************Control de los flags de state
#define M4CL_HAS_STATE_PURGE_FLAG(x)			((x) & M4CL_FLAG_STATE_PURGE)
#define M4CL_SET_STATE_PURGE_FLAG(x)			( (x)=((x) | M4CL_FLAG_STATE_PURGE) )
#define M4CL_RESET_STATE_PURGE_FLAG(x)			( (x)=((x) & (~M4CL_FLAG_STATE_PURGE) ) )

#define M4CL_HAS_STATE_CONCEPT_FLAG(x)			((x) & M4CL_FLAG_STATE_CONCEPT)
#define M4CL_SET_STATE_CONCEPT_FLAG(x)			( (x)=((x) | M4CL_FLAG_STATE_CONCEPT) )
#define M4CL_RESET_STATE_CONCEPT_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_STATE_CONCEPT) ) )

#define M4CL_HAS_STATE_COMPACT_FLAG(x)			((x) & M4CL_FLAG_STATE_COMPACT )
#define M4CL_SET_STATE_COMPACT_FLAG(x)			( (x)=((x) | M4CL_FLAG_STATE_COMPACT) )
#define M4CL_RESET_STATE_COMPACT_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_STATE_COMPACT) ) )

#define M4CL_HAS_STATE_PURGE_REF_FLAG(x)		((x) & M4CL_FLAG_STATE_PURGE_REF)
#define M4CL_SET_STATE_PURGE_REF_FLAG(x)		( (x)=((x) | M4CL_FLAG_STATE_PURGE_REF) )
#define M4CL_RESET_STATE_PURGE_REF_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_STATE_PURGE_REF) ) )

#define M4CL_HAS_STATE_HIDE_FLAG(x)				( (x) & M4CL_FLAG_STATE_HIDE)
#define M4CL_SET_STATE_HIDE_FLAG(x)				( (x)=((x) | M4CL_FLAG_STATE_HIDE) )
#define M4CL_RESET_STATE_HIDE_FLAG(x)			( (x)=((x) & (~M4CL_FLAG_STATE_HIDE) ) )

#define M4CL_HAS_STATE_MM_POSITION_FLAG(x)		( (x) & M4CL_FLAG_STATE_MM_POSITION)
#define M4CL_SET_STATE_MM_POSITION_FLAG(x)		( (x)=((x) | M4CL_FLAG_STATE_MM_POSITION) )
#define M4CL_RESET_STATE_MM_POSITION_FLAG(x)	( (x)=((x) & (~M4CL_FLAG_STATE_MM_POSITION) ) )

#define M4CL_HAS_STATE_INIT_FLAG(x)				( (x) & M4CL_FLAG_STATE_INIT)
#define M4CL_SET_STATE_INIT_FLAG(x)				( (x)=((x) | M4CL_FLAG_STATE_INIT) )
#define M4CL_RESET_STATE_INIT_FLAG(x)			( (x)=((x) & (~M4CL_FLAG_STATE_INIT) ) )

#define M4CL_HAS_STATE_NOT_EXE_FLAG(x)			( (x) & M4CL_FLAG_STATE_NOT_EXE)
#define M4CL_SET_STATE_NOT_EXE_FLAG(x)			( (x)=((x) | M4CL_FLAG_STATE_NOT_EXE) )
#define M4CL_RESET_STATE_NOT_EXE_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_STATE_NOT_EXE) ) )

#define M4CL_HAS_STATE_REFRESH_FLAG(x)			( (x) & M4CL_FLAG_STATE_REFRESH)
#define M4CL_SET_STATE_REFRESH_FLAG(x)			( (x)=((x) | M4CL_FLAG_STATE_REFRESH) )
#define M4CL_RESET_STATE_REFRESH_FLAG(x)		( (x)=((x) & (~M4CL_FLAG_STATE_REFRESH) ) )

#define M4CL_HAS_STATE_EXE_JIT_FLAG(x)			( (x) & M4CL_FLAG_STATE_EXE_JIT)
#define M4CL_SET_STATE_EXE_JIT_FLAG(x)			( (x) = ((x) | M4CL_FLAG_STATE_EXE_JIT) )
#define M4CL_RESET_STATE_EXE_JIT_FLAG(x)		( (x) = ((x) & (~M4CL_FLAG_STATE_EXE_JIT) ) )

#define M4CL_HAS_STATE_PREP_JIT_FLAG(x)			( (x) & M4CL_FLAG_STATE_PREP_JIT)
#define M4CL_SET_STATE_PREP_JIT_FLAG(x)			( (x) = ((x) | M4CL_FLAG_STATE_PREP_JIT) )
#define M4CL_RESET_STATE_PREP_JIT_FLAG(x)		( (x) = ((x) & (~M4CL_FLAG_STATE_PREP_JIT) ) )

#define M4CL_HAS_STATE_NO_INSITE_FLAG(x)		( (x) & M4CL_FLAG_STATE_NO_INSITE)
#define M4CL_SET_STATE_NO_INSITE_FLAG(x)		( (x) = ((x) | M4CL_FLAG_STATE_NO_INSITE) )
#define M4CL_RESET_STATE_NO_INSITE_FLAG(x)		( (x) = ((x) & (~M4CL_FLAG_STATE_NO_INSITE) ) )

#define M4CL_HAS_STATE_NO_INSITE_FLAG_REC(x)	( (x) & M4CL_FLAG_STATE_NO_INSITE_REC)
#define M4CL_SET_STATE_NO_INSITE_FLAG_REC(x)	( (x) = ((x) | M4CL_FLAG_STATE_NO_INSITE_REC) )
#define M4CL_RESET_STATE_NO_INSITE_FLAG_REC(x)	( (x) = ((x) & (~M4CL_FLAG_STATE_NO_INSITE_REC) ) )


#define M4CL_HAS_FLAG(x,y)				( (x) & (y) )
#define M4CL_SET_FLAG(x,y)				( (x)=((x) | (y)) )
#define M4CL_RESET_FLAG(x,y)			( (x)=((x) & (~(y)) ) )

#define M4CL_APPLY_MASK(x,y)			( (x) & (y) )

//INSTRUCCIONES EMBEBIDAS
#define M4CL_EMB_FLAG_EMB						0x80000000	//flag en el offset que indica si es embebida
#define M4CL_EMB_FLAG_SIMPLE					0x02000000	//flag en el offset que indica si es simple
#define M4CL_EMB_FLAG_LEVEL_MASK				0x007f0000	//flags en el offset que indica el nivel 0000 0000 01111111 00000000 00000000
#define	M4CL_EMB_FLAG_BIT_START_LEVEL			16
#define M4CL_EMB_FLAG_CONTROL_FUNC_MASK			0x78000000	//0111 1000 00000000 00000000 00000000
#define M4CL_EMB_FLAG_CONTROL_ARG_MASK			0x0000ffff
#define M4CL_EMB_FLAG_CONTROL_SIGN_MASK			0x00800000  //0000 0000 10000000 00000000 00000000
#define M4CL_EMB_FLAG_SIMPLE_FUNC_MASK			0x0000ffff
#define M4CL_EMB_FLAG_VARIANT_AS_INT_MASK		0x04000000	//flag que indica si es variant como entero o no

#define M4CL_EMB_OFFSET_MASK					0x00ffffff	//Máscara para el offset (anulamos el primer byte, que puede tener flag de variant_as_int). Solo para ins. no embebidos

#define M4CL_MCR_INS_EMB_OFFSET(x)				( (x) & M4CL_EMB_OFFSET_MASK)
 
#define M4CL_MCR_INS_EMB_IS_EMBEBBED(x)			( (x) & M4CL_EMB_FLAG_EMB )
#define M4CL_MCR_INS_EMB_IS_SIMPLE(x)			( (x) & M4CL_EMB_FLAG_SIMPLE )
#define M4CL_MCR_INS_EMB_IS_CONTROL(x)			( M4CL_MCR_INS_EMB_IS_EMBEBBED(x) && !(M4CL_MCR_INS_EMB_IS_SIMPLE(x)) )  //Es de control si no es simple

#define M4CL_MCR_INS_EMB_GET_CONTROL_FUNC(x)	(m4uint32_t( (M4CL_EMB_FLAG_CONTROL_FUNC_MASK & (x)) >>27) )
#define M4CL_MCR_INS_EMB_GET_CONTROL_ARG(x)		( (x) & M4CL_EMB_FLAG_CONTROL_ARG_MASK)

#define M4CL_MCR_INS_EMB_SET_CONTROL_SIGN(x)	( (x) = ((x) | M4CL_EMB_FLAG_CONTROL_SIGN_MASK))
#define M4CL_MCR_INS_EMB_HAS_CONTROL_SIGN(x)	( (x) & M4CL_EMB_FLAG_CONTROL_SIGN_MASK)

#define M4CL_MCR_INS_EMB_SET_VARIANT_INT(x)		( (x) = ((x) | M4CL_EMB_FLAG_VARIANT_AS_INT_MASK))
#define M4CL_MCR_INS_EMB_HAS_VARIANT_INT(x)		( (x) & M4CL_EMB_FLAG_VARIANT_AS_INT_MASK)

#define M4CL_MCR_INS_EMB_SET_IS_CONTROL(x)		( (x) = (x)|M4CL_EMB_FLAG_EMB ) 
#define M4CL_MCR_INS_EMB_SET_IS_SIMPLE(x)		( (x) = (x)|M4CL_EMB_FLAG_EMB|M4CL_EMB_FLAG_SIMPLE)

#define M4CL_MCR_INS_EMB_GET_SIMPLE_FUNC(x)		( (x) & M4CL_EMB_FLAG_SIMPLE_FUNC_MASK)

#define M4CL_MCR_INS_EMB_GET_LEVEL(x)			((M4CL_EMB_FLAG_LEVEL_MASK &  x)>> M4CL_EMB_FLAG_BIT_START_LEVEL )
#define M4CL_MCR_INS_EMB_SET_LEVEL(x,i)			((x)=((x) & (~M4CL_EMB_FLAG_LEVEL_MASK) )  |  (M4CL_EMB_FLAG_LEVEL_MASK& (i<<M4CL_EMB_FLAG_BIT_START_LEVEL) ) )

//para las fechas en la formula
#define M4CL_MCR_EMB_SD_PLUS_INF				0x10
#define M4CL_MCR_EMB_SD_MINUS_INF				0x20
#define M4CL_MCR_EMB_SD_HAS_DAYS				0x40
#define M4CL_MCR_EMB_SD_HAS_SECONDS				0x80

#define M4CL_MCR_EMB_ED_PLUS_INF				0x01
#define M4CL_MCR_EMB_ED_MINUS_INF				0x02
#define M4CL_MCR_EMB_ED_HAS_DAYS				0x04
#define M4CL_MCR_EMB_ED_HAS_SECONDS				0x08


#define M4CL_MCR_INS_SET_START_PLUS_INF(x)			((x)=(x)|M4CL_MCR_EMB_SD_PLUS_INF)
#define M4CL_MCR_INS_SET_START_MINUS_INF(x)			((x)=(x)|M4CL_MCR_EMB_SD_MINUS_INF)
#define M4CL_MCR_INS_SET_START_DAYS(x)				((x)=(x)|M4CL_MCR_EMB_SD_HAS_DAYS)
#define M4CL_MCR_INS_SET_START_DAYS_AND_SECONDS(x)	((x)=(x)|M4CL_MCR_EMB_SD_HAS_DAYS|M4CL_MCR_EMB_SD_HAS_SECONDS)

#define M4CL_MCR_INS_SET_END_PLUS_INF(x)			((x)=(x)|M4CL_MCR_EMB_ED_PLUS_INF)
#define M4CL_MCR_INS_SET_END_MINUS_INF(x)			((x)=(x)|M4CL_MCR_EMB_ED_MINUS_INF)
#define M4CL_MCR_INS_SET_END_DAYS(x)				((x)=(x)|M4CL_MCR_EMB_ED_HAS_DAYS)
#define M4CL_MCR_INS_SET_END_DAYS_AND_SECONDS(x)	((x)=(x)|M4CL_MCR_EMB_ED_HAS_DAYS|M4CL_MCR_EMB_ED_HAS_SECONDS)
									
#define M4CL_MCR_INS_IS_START_PLUS_INF(x)			((x) & M4CL_MCR_EMB_SD_PLUS_INF)
#define M4CL_MCR_INS_IS_START_MINUS_INF(x)			((x) & M4CL_MCR_EMB_SD_MINUS_INF)
#define M4CL_MCR_INS_HAS_START_DAYS(x)				((x) & M4CL_MCR_EMB_SD_HAS_DAYS)
#define M4CL_MCR_INS_HAS_START_DAYS_AND_SECONDS(x)	( ((x) & M4CL_MCR_EMB_SD_HAS_DAYS) && ((x) & M4CL_MCR_EMB_SD_HAS_SECONDS) )

#define M4CL_MCR_INS_IS_END_PLUS_INF(x)				((x) & M4CL_MCR_EMB_ED_PLUS_INF)
#define M4CL_MCR_INS_IS_END_MINUS_INF(x)			((x) & M4CL_MCR_EMB_ED_MINUS_INF)
#define M4CL_MCR_INS_HAS_END_DAYS(x)				((x) & M4CL_MCR_EMB_ED_HAS_DAYS)
#define M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS(x)	( ((x) & M4CL_MCR_EMB_ED_HAS_DAYS) && ((x) & M4CL_MCR_EMB_ED_HAS_SECONDS) )


//FLAGS de las funciones
#define M4CL_FLAG_FN_LOCAL_TEST_DISABLED		0x01
#define M4CL_FLAG_FN_ITEM_NOT_MAPPEABLE		    0x02
#define M4CL_FLAG_FN_NO_RETURN				    0x04


//## end module%3912A6BD0238.epilog


#endif
