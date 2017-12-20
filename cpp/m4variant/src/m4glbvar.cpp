
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4variant.dll
// File:                m4glbvar.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene funciones de comparación
//
//
//
//==============================================================================



#include "m4objglb.hpp"
#include "clregexp.hpp"
#include "m4variant.hpp"
#include "m4unicode.hpp"



m4CompareVariant_f g_apfM4VariantCompareFunctions[ M4CL_MAX_VARIANT_COMPARE_FUNCTION ] =
{
    M4VariantIsEqual,
    M4VariantIsDistinct,
    M4VariantIsBigger,
    M4VariantIsSmaller,
    M4VariantIsBiggerOrEqual,
    M4VariantIsSmallerOrEqual,
	M4VariantIsEqualOrNull,
    M4VariantIsDistinctOrNull,
    M4VariantIsBiggerOrNull,
    M4VariantIsSmallerOrNull,
    M4VariantIsBiggerOrEqualOrNull,
    M4VariantIsSmallerOrEqualOrNull,

    M4VariantCaseIsEqual,
    M4VariantCaseIsDistinct,
    M4VariantCaseIsBigger,
    M4VariantCaseIsSmaller,
    M4VariantCaseIsBiggerOrEqual,
    M4VariantCaseIsSmallerOrEqual,
	M4VariantCaseIsEqualOrNull,
    M4VariantCaseIsDistinctOrNull,
    M4VariantCaseIsBiggerOrNull,
    M4VariantCaseIsSmallerOrNull,
    M4VariantCaseIsBiggerOrEqualOrNull,
    M4VariantCaseIsSmallerOrEqualOrNull,

	M4VariantMatchRegex,
	M4VariantMatchRegexOrNull,
	M4VariantCaseMatchRegex,
	M4VariantCaseMatchRegexOrNull


} ;


// Funciones de comparacion de variants =======================================================

int M4VariantIsEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) == 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsDistinct( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 1 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) != 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsBigger( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) > 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsSmaller( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) < 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsBiggerOrEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) >= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsSmallerOrEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) <= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}

//comparaciones incluyendo null
int M4VariantIsEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type &&  pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) == 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsDistinctOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type || pstVariant1->Type == M4CL_CPP_TYPE_NULL)
    {
        iResult = 1 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) != 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsBiggerOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) > 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsSmallerOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) < 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsBiggerOrEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) >= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantIsSmallerOrEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) <= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}



// ahora las case sensitive

int M4VariantCaseIsEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) == 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsDistinct( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 1 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) != 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsBigger( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) > 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsSmaller( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) < 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsBiggerOrEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) >= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsSmallerOrEqual( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type )
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) <= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}

//comparaciones incluyendo null
int M4VariantCaseIsEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type &&  pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) == 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsDistinctOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type || pstVariant1->Type == M4CL_CPP_TYPE_NULL)
    {
        iResult = 1 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				
				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) != 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsBiggerOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData > pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) > 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsSmallerOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 0 : 1 ;
				if (iResult == 1) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData < pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) < 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsBiggerOrEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData >= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) >= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}


int M4VariantCaseIsSmallerOrEqualOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;


    if( pstVariant1->Type != pstVariant2->Type && pstVariant1->Type != M4CL_CPP_TYPE_NULL)
    {
        iResult = 0 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                iResult = 1 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_MATH_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_DATE :

				iResult = fabs ( pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData ) <= M4_LN4_DATE_PRECISION ? 1 : 0 ;
				if (iResult == 0) {	// Ya sabemos que no son iguales
					iResult = ( pstVariant1->Data.DoubleData <= pstVariant2->Data.DoubleData ) ;
				}
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                iResult = ( M4StrCmp( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) <= 0 ) ;
                break ;

            default :

                iResult = 0 ;
                break ;
        }
    }

    return( iResult ) ;
}

//---------------Funciones de comparacion de variants con regex

int M4VariantMatchRegex( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{

    int iResult ;
	m4bool_t	bMustDelete=M4_FALSE;
	m4char_t	*pszRes;

	ClRegExp *poReg=(ClRegExp*)ai_pvAux;

	if (!poReg){
		poReg=new ClRegExp;
		bMustDelete=M4_TRUE;
	}

	iResult=0;

	if ( pstVariant2->Type!=pstVariant1->Type  ||  pstVariant1->Type!=M4CL_CPP_TYPE_STRING_VAR) {
		iResult=0;
	}
	else{
		//ponemso el case
		poReg->SetSensitive(M4_FALSE);
		
		//compilamos
		pszRes=poReg->CompilePattern(pstVariant2->Data.PointerChar, strlen(pstVariant2->Data.PointerChar) );
		
		if (!pszRes){
			//damos a buscar
			m4uint32_t iSize=strlen(pstVariant1->Data.PointerChar);
			iResult = (poReg->Search(pstVariant1->Data.PointerChar, iSize ,0, iSize ) >= 0 ? 1 : 0) ;
		}
	}

	if (bMustDelete) delete poReg;

	return iResult;
}


int M4VariantMatchRegexOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{
    int iResult ;
	m4bool_t	bMustDelete=M4_FALSE;
	m4char_t	*pszRes;

	ClRegExp *poReg=(ClRegExp*)ai_pvAux;

	if (!poReg){
		poReg=new ClRegExp;
		bMustDelete=M4_TRUE;
	}

	iResult=0;

	if ( (pstVariant2->Type!=pstVariant1->Type  ||  pstVariant1->Type!=M4CL_CPP_TYPE_STRING_VAR) && pstVariant1->Type!=M4CL_CPP_TYPE_NULL){
		iResult=0;
	}
	else{
		if (pstVariant1->Type == M4CL_CPP_TYPE_NULL){
			iResult=1;
		}
		else{
			//ponemso el case
			poReg->SetSensitive(M4_FALSE);
			
			//compilamos
			pszRes=poReg->CompilePattern(pstVariant2->Data.PointerChar, strlen(pstVariant2->Data.PointerChar) );
			
			if (!pszRes){
				//damos a buscar
				m4uint32_t iSize=strlen(pstVariant1->Data.PointerChar);
				iResult = (poReg->Search(pstVariant1->Data.PointerChar, iSize ,0 , iSize) >= 0 ? 1 : 0) ;
			}
		}
	}

	if (bMustDelete) delete poReg;

	return iResult;
}

int M4VariantCaseMatchRegex( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{
    int iResult ;
	m4bool_t	bMustDelete=M4_FALSE;
	m4char_t	*pszRes;

	ClRegExp *poReg=(ClRegExp*)ai_pvAux;

	if (!poReg){
		poReg=new ClRegExp;
		bMustDelete=M4_TRUE;
	}

	iResult=0;

	if (pstVariant2->Type!=pstVariant1->Type  ||  pstVariant1->Type!=M4CL_CPP_TYPE_STRING_VAR) {
		iResult=0;
	}
	else{
		//ponemso el case
		poReg->SetSensitive(M4_TRUE);
		
		//compilamos
		pszRes=poReg->CompilePattern(pstVariant2->Data.PointerChar, strlen(pstVariant2->Data.PointerChar) );
		
		if (!pszRes){
			//damos a buscar
			m4uint32_t iSize=strlen(pstVariant1->Data.PointerChar);
			iResult = (poReg->Search(pstVariant1->Data.PointerChar, iSize ,0 ,iSize) >= 0 ? 1 : 0) ;
		}
	}

	if (bMustDelete) delete poReg;

	return iResult;
}

int M4VariantCaseMatchRegexOrNull( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux )
{
    int iResult ;
	m4bool_t	bMustDelete=M4_FALSE;
	m4char_t	*pszRes;

	ClRegExp *poReg=(ClRegExp*)ai_pvAux;

	if (!poReg){
		poReg=new ClRegExp;
		bMustDelete=M4_TRUE;
	}

	iResult=0;

	if ( (pstVariant2->Type!=pstVariant1->Type  ||  pstVariant1->Type!=M4CL_CPP_TYPE_STRING_VAR) && pstVariant1->Type!=M4CL_CPP_TYPE_NULL){
		iResult=0;
	}
	else{
		if (pstVariant1->Type == M4CL_CPP_TYPE_NULL){
			iResult=1;
		}
		else{
			//ponemso el case
			poReg->SetSensitive(M4_TRUE);
			
			//compilamos
			pszRes=poReg->CompilePattern(pstVariant2->Data.PointerChar, strlen(pstVariant2->Data.PointerChar) );
			
			if (!pszRes){
				//damos a buscar
				m4uint32_t iSize=strlen(pstVariant1->Data.PointerChar);
				iResult = (poReg->Search(pstVariant1->Data.PointerChar, iSize ,0 ,iSize) >= 0 ? 1 : 0) ;
			}
		}
	}

	if (bMustDelete) delete poReg;

	return iResult;
}

//Nueva para la BSearch
m4int8_t M4VariantCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 )
{
	m4double_t dDiff;


	//Suponer que los tipos llegan ok.
    if( pstVariant1->Type != pstVariant2->Type )
    {
        return -2 ;
    }
    else
    {
        switch( pstVariant1->Type )
        {
            case    M4CL_CPP_TYPE_NULL :

                return 0 ;
                break ;

            case    M4CL_CPP_TYPE_NUMBER :
				dDiff = pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData;
				dDiff = ( fabs (dDiff) <= M4_LN4_MATH_PRECISION ) ? 0.0 : dDiff ;
				break;
            case    M4CL_CPP_TYPE_DATE :
				dDiff = pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData;
				dDiff = ( fabs (dDiff) <= M4_LN4_DATE_PRECISION ) ? 0.0 : dDiff ;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :

				// UNICODE LOCALE
                dDiff = M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) ;
                break ;

            default :
	//Suponer que los tipos llegan ok.
                return -2 ;
                break ;
        }

        if ( dDiff == 0 ) 
			return 0;
		else if ( dDiff > 0 )
			return +1;
		else 
			return -1;

    }

}
