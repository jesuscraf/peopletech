//## begin module%390563F2023D.cm preserve=no
//## end module%390563F2023D.cm

//## begin module%390563F2023D.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%390563F2023D.cp

//## Module: m4servervt%390563F2023D; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: e:\mybuild\m4serverbase\src\m4servervt.cpp

//## begin module%390563F2023D.additionalIncludes preserve=no
//## end module%390563F2023D.additionalIncludes

//## begin module%390563F2023D.includes preserve=yes
#include <string.h>
#include <m4trace.hpp>
#include <basiclog.hpp>
#include <m4srvtrace.h>
#include <m4types.hpp>
#include <m4string.hpp>
#include <m4servervt.hpp>

#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%390563F2023D.includes

//## begin module%390563F2023D.declarations preserve=no
//## end module%390563F2023D.declarations

//## begin module%390563F2023D.additionalDeclarations preserve=yes
#ifdef _AIX
#define DENOMINADOR 4294967296LL
#else
#define DENOMINADOR 4294967296
#endif
//## end module%390563F2023D.additionalDeclarations


// Class m4ServerVT 




m4ServerVT::m4ServerVT (void )
  //## begin m4ServerVT::m4ServerVT%2057418851.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%2057418851.hasinit
  //## begin m4ServerVT::m4ServerVT%2057418851.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%2057418851.initialization
{
  //## begin m4ServerVT::m4ServerVT%2057418851.body preserve=yes
	Data.PointerChar=NULL;
  //## end m4ServerVT::m4ServerVT%2057418851.body
}

m4ServerVT::m4ServerVT (const m4ServerVT& ai_var)
  //## begin m4ServerVT::m4ServerVT%-1660970746.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%-1660970746.hasinit
  //## begin m4ServerVT::m4ServerVT%-1660970746.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%-1660970746.initialization
{
  //## begin m4ServerVT::m4ServerVT%-1660970746.body preserve=yes
	Set ( ai_var );
  //## end m4ServerVT::m4ServerVT%-1660970746.body
}

m4ServerVT::m4ServerVT (const m4double_t& ai_double)
  //## begin m4ServerVT::m4ServerVT%361154961.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%361154961.hasinit
  //## begin m4ServerVT::m4ServerVT%361154961.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%361154961.initialization
{
  //## begin m4ServerVT::m4ServerVT%361154961.body preserve=yes
	SetDouble(ai_double);
  //## end m4ServerVT::m4ServerVT%361154961.body
}

m4ServerVT::m4ServerVT (const m4pchar_t& ai_char, m4bool_t ai_bCopy)
  //## begin m4ServerVT::m4ServerVT%-1347296588.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%-1347296588.hasinit
  //## begin m4ServerVT::m4ServerVT%-1347296588.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%-1347296588.initialization
{
  //## begin m4ServerVT::m4ServerVT%-1347296588.body preserve=yes
	m_bOwner = ( (ai_bCopy && ai_char) ? M4_TRUE : M4_FALSE );
	if ( m_bOwner )
		SetString( ai_char ,M4_FALSE);
	else
		SetPointer( ai_char );
  //## end m4ServerVT::m4ServerVT%-1347296588.body
}

m4ServerVT::m4ServerVT (m4int_t ai_int)
  //## begin m4ServerVT::m4ServerVT%956649482.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%956649482.hasinit
  //## begin m4ServerVT::m4ServerVT%956649482.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%956649482.initialization
{
  //## begin m4ServerVT::m4ServerVT%956649482.body preserve=yes
	SetInt(ai_int);
  //## end m4ServerVT::m4ServerVT%956649482.body
}

m4ServerVT::m4ServerVT (const m4pcchar_t& ai_char, m4bool_t ai_bCopy)
  //## begin m4ServerVT::m4ServerVT%956825070.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%956825070.hasinit
  //## begin m4ServerVT::m4ServerVT%956825070.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%956825070.initialization
{
  //## begin m4ServerVT::m4ServerVT%956825070.body preserve=yes
	m_bOwner = ( (ai_bCopy && ai_char) ? M4_TRUE : M4_FALSE );
	if ( m_bOwner )
		SetString( (char* const)ai_char ,M4_FALSE);
	else
		SetPointer( (void*)ai_char );
  //## end m4ServerVT::m4ServerVT%956825070.body
}

m4ServerVT::m4ServerVT (m4int64_t ai_i64Value)
  //## begin m4ServerVT::m4ServerVT%972896440.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%972896440.hasinit
  //## begin m4ServerVT::m4ServerVT%972896440.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%972896440.initialization
{
  //## begin m4ServerVT::m4ServerVT%972896440.body preserve=yes
	SetInt64(ai_i64Value);
  //## end m4ServerVT::m4ServerVT%972896440.body
}

m4ServerVT::m4ServerVT (m4return_t ai_m4ret)
  //## begin m4ServerVT::m4ServerVT%972896445.hasinit preserve=no
      : Type(M4_SVT_TYPE_NULL), m_bOwner(M4_FALSE)
  //## end m4ServerVT::m4ServerVT%972896445.hasinit
  //## begin m4ServerVT::m4ServerVT%972896445.initialization preserve=yes
  //## end m4ServerVT::m4ServerVT%972896445.initialization
{
  //## begin m4ServerVT::m4ServerVT%972896445.body preserve=yes
	SetM4return(ai_m4ret);
  //## end m4ServerVT::m4ServerVT%972896445.body
}


m4ServerVT::~m4ServerVT (void )
{
  //## begin m4ServerVT::~m4ServerVT%-497375751.body preserve=yes
	FreeData();
  //## end m4ServerVT::~m4ServerVT%-497375751.body
}



//## Other Operations (implementation)
void m4ServerVT::FreeData (void )
{
  //## begin m4ServerVT::FreeData%428367957.body preserve=yes
	if ( m_bOwner )
	{
		if (( Type == M4_SVT_TYPE_STRING_VAR ) || ( Type == M4_SVT_TYPE_STRING ))
		{
			if(this->Data.PointerChar)
				delete [] this->Data.PointerChar;
			this->Data.PointerChar = NULL;
			m_bOwner = M4_FALSE;
		}
	}
  //## end m4ServerVT::FreeData%428367957.body
}

m4ServerVT& m4ServerVT::operator = (const m4ServerVT& ai_var)
{
  //## begin m4ServerVT::operator =%-449823595.body preserve=yes
	Set ( ai_var );
	
	return *this;
  //## end m4ServerVT::operator =%-449823595.body
}

m4char_t m4ServerVT::GetType (void )
{
  //## begin m4ServerVT::GetType%575041740.body preserve=yes
	return Type;
  //## end m4ServerVT::GetType%575041740.body
}

void m4ServerVT::SetDouble (m4double_t ai_double)
{
  //## begin m4ServerVT::SetDouble%1397363735.body preserve=yes
    FreeData();
    Type = M4_SVT_TYPE_DOUBLE;
    Data.DoubleData = ai_double;
  //## end m4ServerVT::SetDouble%1397363735.body
}

void m4ServerVT::SetString (const m4pchar_t ai_char, m4bool_t ai_bFree)
{
  //## begin m4ServerVT::SetString%-1556363982.body preserve=yes
	if(ai_bFree)
	{
		// Liberamos el antiguo Variant
		FreeData();
	}
	// Establecemos el nuevo Variant como String con Owner
	Type	 = M4_SVT_TYPE_STRING;
	m_bOwner = M4_TRUE;
	// Reservamos memoria y copiamos
	m4int32_t iStrSize = strlen ( ai_char ) + 1;
	this->Data.PointerChar = new char[ iStrSize ];
	memcpy ( this->Data.PointerChar, ai_char, iStrSize );
  //## end m4ServerVT::SetString%-1556363982.body
}

void m4ServerVT::SetNull (void )
{
  //## begin m4ServerVT::SetNull%-1597179984.body preserve=yes
    FreeData();
    Type = M4_SVT_TYPE_NULL;
    Data.DoubleData = 0;
  //## end m4ServerVT::SetNull%-1597179984.body
}

void m4ServerVT::Set (const m4ServerVT& ai_var)
{
  //## begin m4ServerVT::Set%-685867871.body preserve=yes
	FreeData ();

	
	m_bOwner = (ai_var.m_bOwner && ai_var.Data.PointerChar)? M4_TRUE:M4_FALSE;

	switch(ai_var.Type)
	{
		case M4_SVT_TYPE_NULL:
			SetNull();
			break;
		case M4_SVT_TYPE_STRING_VAR :
		case M4_SVT_TYPE_STRING :
			if ( m_bOwner )
				SetString(ai_var.Data.PointerChar,M4_FALSE);
			else
				SetPointer(ai_var.Data.PointerChar);
			break;
		case M4_SVT_TYPE_POINTER:
			SetPointer(ai_var.Data.PointerChar);
			break;
		case M4_SVT_TYPE_DOUBLE:
			SetDouble(ai_var.Data.DoubleData);
			break;
		case M4_SVT_TYPE_INT:
			SetInt(ai_var.Data.IntData);
			break;
		case M4_SVT_TYPE_M4RETURN:	
			SetM4return((m4return_t)ai_var.Data.Int64Data);
			break;
		case M4_SVT_TYPE_INT64:
			SetInt64(ai_var.Data.Int64Data);
			break;

	}




  //## end m4ServerVT::Set%-685867871.body
}

m4bool_t m4ServerVT::operator == (m4ServerVT ai_right)
{
  //## begin m4ServerVT::operator ==%920379248.body preserve=yes
 		if (Type!=ai_right.GetType())
			return M4_FALSE;
		m4bool_t bResult;
        switch( Type )
        {
            case    M4_SVT_TYPE_NULL :
                bResult = M4_TRUE ;
                break ;
            case    M4_SVT_TYPE_DOUBLE :
				bResult = (Data.DoubleData == ai_right.Data.DoubleData)  ? M4_TRUE : M4_FALSE ;
				break;
            case    M4_SVT_TYPE_INT :
				bResult = (Data.IntData == ai_right.Data.IntData)  ? M4_TRUE : M4_FALSE ;
				break;
			case    M4_SVT_TYPE_INT64:
				bResult = (Data.Int64Data == ai_right.Data.Int64Data)  ? M4_TRUE : M4_FALSE ;
				break;
			case M4_SVT_TYPE_STRING_VAR  :
			case M4_SVT_TYPE_STRING  :
				bResult = strcmp(Data.PointerChar, ai_right.Data.PointerChar) ?  M4_FALSE:M4_TRUE ;
            default :
				M4_SRV_ERROR(0,  "Operator == , Error in type %0:s", Type );
                bResult = M4_FALSE;
                break ;
        }
		return bResult;
  //## end m4ServerVT::operator ==%920379248.body
}

m4bool_t m4ServerVT::operator != (m4ServerVT ai_right)
{
  //## begin m4ServerVT::operator !=%920379249.body preserve=yes
	return (! (*this==ai_right))?M4_TRUE:M4_FALSE;
  //## end m4ServerVT::operator !=%920379249.body
}

void m4ServerVT::TraceSVT (m4string_t &ao_oTraceStr)
{
  //## begin m4ServerVT::TraceSVT%920908742.body preserve=yes

	m4char_t AuxStr[200] ;	//+que suficiente para sprintf de numeros

	switch (Type)  
	{
    case M4_SVT_TYPE_NULL:
        ao_oTraceStr = "<NULL>";
        break;
    case M4_SVT_TYPE_DOUBLE:
		sprintf( AuxStr, "(d) %.*f", (int)2, (double)Data.DoubleData  ) ;
		ao_oTraceStr = AuxStr ;
        break;
	case M4_SVT_TYPE_INT:
		sprintf( AuxStr, "(i) %i", (int)Data.IntData  ) ;
		ao_oTraceStr = AuxStr ;
		break;
	case M4_SVT_TYPE_INT64:
        {
		int iHigh=(m4int_t)(Data.Int64Data/DENOMINADOR) ;

		if (iHigh) {
			sprintf( AuxStr, "(i64) %i*DENOMINADOR+%i", 
				(int)iHigh << (int)(Data.Int64Data%DENOMINADOR)  ) ;
		} else {
			sprintf( AuxStr, "(i64) %i", 
				(int)(Data.Int64Data%DENOMINADOR)  ) ;
		} ;
		ao_oTraceStr = AuxStr ;

		}
		break;
    case M4_SVT_TYPE_STRING:
		ao_oTraceStr	=  "(s) " ;
		ao_oTraceStr	+= Data.PointerChar ;
        break;
    case M4_SVT_TYPE_STRING_VAR:
		ao_oTraceStr	=  "(sv) " ;
		ao_oTraceStr	+= Data.PointerChar ;
        break;
	case M4_SVT_TYPE_M4RETURN:
	    switch(Data.IntData)
		{
		case M4_SUCCESS:
			ao_oTraceStr =  "(m4ret) M4_SUCCESS" ;
			break;
		case M4_ERROR:
			ao_oTraceStr =  "(m4ret) M4_ERROR" ;
			break;
		case M4_WARNING:
			ao_oTraceStr =  "(m4ret) M4_WARNING"  ;
			break;
		default:
			sprintf( AuxStr, "(m4ret) %i", (int)Data.IntData  ) ;
			ao_oTraceStr = AuxStr ;
			break;
		}
        break;
	case M4_SVT_TYPE_POINTER:
		sprintf( AuxStr, "(pointer) %i", (int)Data.PointerChar  ) ;
		ao_oTraceStr = AuxStr ;
		break;
    default:
		sprintf( AuxStr, "(%i) Invalid type!", (int)Type ) ;
		ao_oTraceStr = AuxStr ;
    }

    return ;
  //## end m4ServerVT::TraceSVT%920908742.body
}

void m4ServerVT::SetPointer (void *ai_Pointer)
{
  //## begin m4ServerVT::SetPointer%956592443.body preserve=yes
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	Type	 = M4_SVT_TYPE_POINTER;
	m_bOwner = M4_FALSE;

	this->Data.PointerChar = (char *)ai_Pointer;
  //## end m4ServerVT::SetPointer%956592443.body
}

void m4ServerVT::SetInt (m4int_t ai_int)
{
  //## begin m4ServerVT::SetInt%956649483.body preserve=yes
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	Type	 = M4_SVT_TYPE_INT;
	m_bOwner = M4_FALSE;

	this->Data.IntData = ai_int;
  //## end m4ServerVT::SetInt%956649483.body
}

void m4ServerVT::SetInt64 (m4int64_t ai_i64Value)
{
  //## begin m4ServerVT::SetInt64%972896439.body preserve=yes
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	Type	 = M4_SVT_TYPE_INT64;
	m_bOwner = M4_FALSE;

	this->Data.Int64Data = ai_i64Value;

  //## end m4ServerVT::SetInt64%972896439.body
}

void m4ServerVT::SetM4return (m4return_t ai_ret)
{
  //## begin m4ServerVT::SetM4return%972896446.body preserve=yes
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	Type	 = M4_SVT_TYPE_M4RETURN;
	m_bOwner = M4_FALSE;

	this->Data.IntData = (m4int_t)ai_ret;
  //## end m4ServerVT::SetM4return%972896446.body
}

// Additional Declarations
  //## begin m4ServerVT%36DBD83B03C3.declarations preserve=yes
  //## end m4ServerVT%36DBD83B03C3.declarations

//## begin module%390563F2023D.epilog preserve=yes
//## end module%390563F2023D.epilog
