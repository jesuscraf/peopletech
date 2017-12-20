//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4var.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//
//
//==============================================================================


#include "m4var.hpp"
#include "cldates.hpp"
#include "clgeniod.hpp"
#include "m4stl.hpp"
#include "sizer.hpp"
#include "chlog.hpp"
#include "m4varres.hpp"
#include "m4unicode.hpp"


#ifdef _WINDOWS
	#include "cvdate.h"
#endif



m4return_t	m4VariantType::GetSize( ClSize &ao_size ) const
{
	ao_size += sizeof( m4VariantType );

	if( m_bOwner == M4_TRUE )
	{
		if( Type == M4CL_CPP_TYPE_STRING ||
			Type == M4CL_CPP_TYPE_STRING_VAR ||
			Type == M4CL_CPP_TYPE_STRING_SYS )
		{
			if( Data.PointerChar != NULL )
			{
				ao_size += strlen( Data.PointerChar ) + 1;
			}
		}
	}

	return M4_SUCCESS;
}


ostream & operator<< ( ostream &ai_ofs, const m4VariantType &ai_v1 )
{
	if( ai_v1.IsUser == M4_TRUE )
	{
		ai_ofs << "[IsUser]";
	}

    switch( ai_v1.Type )
	{
		case M4CL_CPP_TYPE_NULL:
			ai_ofs << "<NULL>";
			break;

		case M4CL_CPP_TYPE_NUMBER:
			ai_ofs << "(n)";
			break;

		case M4CL_CPP_TYPE_STRING:
			ai_ofs << "(s)";
			break;

		case M4CL_CPP_TYPE_DATE:
			ai_ofs << "(d)";
			break;

		case M4CL_CPP_TYPE_STRING_POOL:
			ai_ofs << "(sp)";
			break;

		case M4CL_CPP_TYPE_STRING_VAR:
			ai_ofs << "(sv)";
			break;

		case M4CL_CPP_TYPE_STRING_SYS:
			ai_ofs << "(ss)";
			break;

		default:
			ai_ofs << ai_v1.Type << "*** tipo no válido! ***";
    }

    switch( ai_v1.Type )
	{
		case M4CL_CPP_TYPE_NULL:
			break;

		case M4CL_CPP_TYPE_NUMBER:
			ai_ofs << ai_v1.Data.DoubleData;
			break;

		case M4CL_CPP_TYPE_DATE:
			m4char_t	acString[ 21 ] ;
			ClDateToString( ai_v1.Data.DoubleData, acString, 20 ) ;
			ai_ofs << acString;
			break;

		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
			if( ai_v1.Data.PointerChar != NULL )
			{
				ai_ofs << ai_v1.Data.PointerChar;
			}
			else
			{
				ai_ofs << "NULL";
			}
			break;
    }

    return ai_ofs;
}


m4return_t	m4VariantType::Serialize( ClGenericIODriver& IOD	)
{
	m4return_t RetVal = IOD.Write( IsUser );

	if( RetVal == M4_SUCCESS )
	{
		RetVal = IOD.Write( Type );

		if( RetVal == M4_SUCCESS )
		{
			switch( Type )
			{
				case M4CL_CPP_TYPE_NUMBER:
				case M4CL_CPP_TYPE_DATE:
					RetVal = IOD.Write( Data.DoubleData );
					break;

				case M4CL_CPP_TYPE_STRING_VAR:
				case M4CL_CPP_TYPE_STRING_SYS:
					if( Data.PointerChar != NULL )
					{
						RetVal = IOD.Write( (void*)Data.PointerChar,strlen( Data.PointerChar ) + 1 );
					}
					else
					{
						RetVal = IOD.Write( (void*)Data.PointerChar,(m4uint32_t)0 );
					}
					break;

				case M4CL_CPP_TYPE_NULL:
					// No serializamos nada
					break;

				case M4CL_CPP_TYPE_STRING_POOL:
					M4_ASSERT(0);
					break;

				default:
					M4_ASSERT(0);
					break;
			}
		}
	}

	return RetVal;
}


m4return_t	m4VariantType::DeSerialize( ClGenericIODriver& IOD )
{
	FreeData();

	m4return_t RetVal = IOD.Read( IsUser );

	if( RetVal == M4_SUCCESS )
	{
		RetVal = IOD.Read( Type );

		if( RetVal == M4_SUCCESS )
		{
			m4uint32_t iSize;
			switch( Type )
			{
			case M4CL_CPP_TYPE_NUMBER:
			case M4CL_CPP_TYPE_DATE:
				RetVal = IOD.Read( Data.DoubleData );
				break;
				
			case M4CL_CPP_TYPE_STRING_VAR:
				RetVal = IOD.ReadSize( iSize );
				if( RetVal == M4_SUCCESS )
				{
					if( iSize )
					{
						Data.PointerChar = new m4char_t[iSize];
						RetVal = IOD.ReadBuffer( Data.PointerChar );
						if( RetVal == M4_SUCCESS )
						{
							m_bOwner = M4_TRUE;
						}
					}
					else
					{
						//Cosa rara.
						//Type = M4CL_CPP_TYPE_NULL;  Comentada por dar problemas de asimetria en serializacion
						Data.PointerChar = NULL;
					}
				}
				break;

			case M4CL_CPP_TYPE_STRING_SYS:

				RetVal = IOD.ReadSize( iSize );
				if( RetVal == M4_SUCCESS )
				{
					if( iSize )
					{
						Data.PointerChar = M4SysAllocString( iSize );
						RetVal = IOD.ReadBuffer( Data.PointerChar );
						if( RetVal == M4_SUCCESS )
						{
							m_bOwner = M4_TRUE;
						}
					}
					else
					{
						//Cosa rara.
						//Type = M4CL_CPP_TYPE_NULL;  Comentada por dar problemas de asimetria en serializacion
						Data.PointerChar = NULL;
					}
				}
				break;
				
			case M4CL_CPP_TYPE_NULL:
				// No deserializamos nada
				break;

			case M4CL_CPP_TYPE_STRING_POOL:
				M4_ASSERT(0);
				break;

			default:
				M4_ASSERT(0);
				break;
			}
		}
	}

	return RetVal;
}


m4VariantType&	m4VariantType::Sum( m4VariantType &ai_var, m4bool_t ai_bConcatStrings )
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:			
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
			if( ai_var.Type == Type )
			{
				/* Bug 0082332
				Las cadenas no se suman y nos quedamos con la primera salvo que
				explícitamente se pida que se concatene
				*/
				if( ai_bConcatStrings == M4_TRUE )
				{
					AllocCatString( ai_var );
				}
			}
			break;

        case M4CL_CPP_TYPE_NUMBER:
			if( ai_var.Type == Type )
			{
				Data.DoubleData += ai_var.Data.DoubleData;
			}
			break;

        case M4CL_CPP_TYPE_DATE:
			if( ai_var.Type == Type )
			{
				if( ai_var.Data.DoubleData > Data.DoubleData )
				{
					Data.DoubleData = ai_var.Data.DoubleData;
				}
			}			
            break;

		default:
			Set (ai_var);
			break;
	}

	return *this;
}


m4VariantType&	m4VariantType::Dec( m4VariantType &ai_var )
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:			
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
        case M4CL_CPP_TYPE_DATE:
			break;

        case M4CL_CPP_TYPE_NUMBER:
			if( ai_var.Type == Type )
			{
				Data.DoubleData -= ai_var.Data.DoubleData;
			}
			break;

		default:
			if( ai_var.Type == M4CL_CPP_TYPE_NUMBER )
			{
				Set( ai_var );
				Data.DoubleData *= -1;
			}
			break;
	}

	return *this;
}


m4VariantType&	m4VariantType::Mult( m4VariantType &ai_var )
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:			
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
        case M4CL_CPP_TYPE_DATE:
			break;

        case M4CL_CPP_TYPE_NUMBER:
			if( ai_var.Type == Type )
			{
				Data.DoubleData *= ai_var.Data.DoubleData;
			}
			break;

		default:
			//Set (ai_var);
			break;
	}

	return *this;
}


m4VariantType&	m4VariantType::Div( m4VariantType &ai_var )
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:			
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
        case M4CL_CPP_TYPE_DATE:
			break;

        case M4CL_CPP_TYPE_NUMBER:
			if ( ai_var.Type == Type )
			{
				Data.DoubleData /= ai_var.Data.DoubleData;
			}
			break;

		default:
			//Set (ai_var);
			break;
	}

	return *this;
}


m4VariantType&	m4VariantType::Max( m4VariantType &ai_var )
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:			
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
			if( ai_var.Type == Type )
			{
				if( strcmpi( Data.PointerChar, ai_var.Data.PointerChar ) < 0 )
				{
					Set( ai_var );
				}
			}
			break;

        case M4CL_CPP_TYPE_NUMBER:
        case M4CL_CPP_TYPE_DATE:
			if( ai_var.Type == Type )
			{
				if( ai_var.Data.DoubleData > Data.DoubleData )
				{
					Data.DoubleData = ai_var.Data.DoubleData;
				}
			}
			break;

		default:
			Set( ai_var );
			break;
	}

	return *this;
}


m4VariantType&	m4VariantType::Min( m4VariantType &ai_var )        
{
	switch( Type )
	{
        case M4CL_CPP_TYPE_STRING:
		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
			if( ai_var.Type == Type )
			{
				if( strcmpi( Data.PointerChar, ai_var.Data.PointerChar ) > 0 )
				{
					Set( ai_var );
				}
			}
			break;

        case M4CL_CPP_TYPE_NUMBER:
        case M4CL_CPP_TYPE_DATE:
			if ( ai_var.Type == Type )
			{
				if( ai_var.Data.DoubleData < Data.DoubleData )
				{
					Data.DoubleData = ai_var.Data.DoubleData;
				}
			}
			break;

		default:
			Set( ai_var );
			break;
	}

	return *this;
}



m4bool_t operator == ( const m4VariantType &ai_left, const m4VariantType &ai_right )
{
    if( ai_left.Type == ai_right.Type && ( ai_right.Type==M4CL_CPP_TYPE_STRING_VAR || ai_right.Type == M4CL_CPP_TYPE_STRING_SYS ) )
	{
        return( !strcmp( ai_left.Data.PointerChar, ai_right.Data.PointerChar ) ? M4_TRUE : M4_FALSE );
    }
    else
	{
		if( ai_left.Type != ai_right.Type )
		{
			return M4_FALSE;
		}

		m4bool_t bResult;

        switch( ai_left.Type )
        {
            case    M4CL_CPP_TYPE_NULL :
                bResult = M4_TRUE ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				bResult = ( fabs (ai_left.Data.DoubleData - ai_right.Data.DoubleData) <= M4_LN4_MATH_PRECISION ) ? M4_TRUE : M4_FALSE ;
				break;

            case    M4CL_CPP_TYPE_DATE :
				bResult = ( fabs (ai_left.Data.DoubleData - ai_right.Data.DoubleData) <= M4_LN4_DATE_PRECISION ) ? M4_TRUE : M4_FALSE ;
                break ;

            default :
				//Suponer que los tipos llegan ok.
                bResult = M4_TRUE ;
                break ;
        }
		
		return bResult;
    }
}


m4bool_t operator != (const m4VariantType &ai_left, const m4VariantType &ai_right)
{
    if( ai_left.Type == ai_right.Type && (ai_right.Type==M4CL_CPP_TYPE_STRING_VAR || ai_right.Type == M4CL_CPP_TYPE_STRING_SYS) )
	{
        return( (strcmp(ai_left.Data.PointerChar, ai_right.Data.PointerChar) != 0 ) ? M4_TRUE : M4_FALSE );
    }
    else
	{
		if( ai_left.Type != ai_right.Type )
		{
			return M4_TRUE;
		}

		m4bool_t bResult;

        switch( ai_left.Type )
        {
            case    M4CL_CPP_TYPE_NULL :
                bResult = M4_FALSE ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				bResult = ( fabs (ai_left.Data.DoubleData - ai_right.Data.DoubleData) <= M4_LN4_MATH_PRECISION ) ? M4_FALSE : M4_TRUE ;
				break;

            case    M4CL_CPP_TYPE_DATE :
				bResult = ( fabs (ai_left.Data.DoubleData - ai_right.Data.DoubleData) <= M4_LN4_DATE_PRECISION ) ? M4_FALSE : M4_TRUE ;
                break ;

            default :
				//Suponer que los tipos llegan ok.
                bResult = M4_TRUE ;
                break ;
        }
		
		return bResult;
    }
}



//////////////////////////////////////
//
// Windows specific
//
#ifdef _WINDOWS

void m4VariantType::SetVARIANT( const VARIANT& ai_VARIANT )
{
	FreeData();

	IsUser = M4_TRUE;
	m_bOwner = M4_FALSE;
	
	switch( ai_VARIANT.vt )
	{
		case VT_I2:
			Type = M4CL_CPP_TYPE_NUMBER; 
			VarR8FromI2( ai_VARIANT.iVal, &Data.DoubleData );
			break;

		case VT_I4:
			Type = M4CL_CPP_TYPE_NUMBER; 
			VarR8FromI4( ai_VARIANT.lVal, &Data.DoubleData );
			break;

		case VT_R4:
			Type = M4CL_CPP_TYPE_NUMBER; 
			VarR8FromR4( ai_VARIANT.fltVal, &Data.DoubleData ); 
			break;

		case VT_R8:
			Type = M4CL_CPP_TYPE_NUMBER; 
			Data.DoubleData = ai_VARIANT.dblVal;
			break;

		case VT_BOOL:
			Type = M4CL_CPP_TYPE_NUMBER; 
			VarR8FromBool( ai_VARIANT.boolVal, &Data.DoubleData );
			break;

		case VT_UI1:
			Type = M4CL_CPP_TYPE_NUMBER; 
			VarR8FromUI1( ai_VARIANT.bVal, &Data.DoubleData );
			break;

		case VT_DATE:
			Type = M4CL_CPP_TYPE_DATE;
			Data.DoubleData = DateToM4date( ai_VARIANT.date );
			break;

		case VT_BSTR:
			Type = M4CL_CPP_TYPE_STRING_VAR;
				
			// UNICODE VB
			Data.PointerChar = M4VBToCpp( ai_VARIANT.bstrVal ) ;
			if( Data.PointerChar != NULL )
			{
				m_bOwner = M4_TRUE;
			}
			else
			{
				// mejor "" que un NULL, que luego casca.
				Data.PointerChar = "";
			}
			break;
			
		case VT_CY:
			Type = M4CL_CPP_TYPE_NUMBER;
			VarR8FromCy( ai_VARIANT.cyVal, &Data.DoubleData );
			break;
			
		case VT_NULL:
		default:
			Type = M4CL_CPP_TYPE_NULL;
			break;
	}
}


/**
 * Da el valor del m4VariantType al VARIANT que se pasa como parámetro por referencia.
 *
 * @param ao_VARIANT	VARIANT por referencia al que se le da el valor del m4VariantType.
 * @return void.
 */
void m4VariantType::GetVARIANT( VARIANT& ao_VARIANT ) const
{
	VariantClear( &ao_VARIANT );

	switch( Type )
	{
		case M4CL_CPP_TYPE_NULL:
			ao_VARIANT.vt = VT_NULL;
			break;

		case M4CL_CPP_TYPE_NUMBER:
			ao_VARIANT.vt = VT_R8;
			ao_VARIANT.dblVal = Data.DoubleData;
			break;

		case M4CL_CPP_TYPE_DATE:
			// jcr. Si no es fecha válida devolvemos nulo
			if( Data.DoubleData < M4CL_MINUS_INFINITE_IN_JULIAN || Data.DoubleData > M4CL_PLUS_INFINITE_IN_JULIAN )
			{
				ao_VARIANT.vt = VT_NULL;
			}
			else
			{
				ao_VARIANT.vt = VT_DATE;
				ao_VARIANT.date = M4dateToDate(Data.DoubleData);
			}
			break;

		case M4CL_CPP_TYPE_STRING_VAR:
		case M4CL_CPP_TYPE_STRING_SYS:
			ao_VARIANT.vt = VT_BSTR;
			
			// UNICODE VB
			ao_VARIANT.bstrVal = M4CppToVB( Data.PointerChar ) ;
			if( !ao_VARIANT.bstrVal)
			{
				// El puntero nunca debería de ser nulo.
				ao_VARIANT.bstrVal = SysAllocString(L"");
			}
			break;

		default:
			// Error, tipo no válido.
			M4_ASSERT(0);
			ao_VARIANT.vt = VT_NULL;
			break;
	}
}

#endif
//
// End of Windows specific
/////////////////////
