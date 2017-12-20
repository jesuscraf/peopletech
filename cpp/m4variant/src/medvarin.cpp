//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         VM
// File:           medvarin.cpp
// Project:        Component Name
// Author:         Meta Software M.S. , S.A
// Date:            15/07
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    This module defines...
////
//==============================================================================


#include "medvarin.hpp"
#include "m4system.hpp"
#include "cldates.hpp"
#include "m4varres.hpp"
#include "chlog.hpp"


m4return_t ClVariantConversor::ToDouble(m4VariantType &aio_oVariant){

	m4return_t ret;
	m4pchar_t	pszTemp;
    m4double_t  dTemp;

	ret=M4_SUCCESS;
	m_bDateOverflow=M4_FALSE;
	aio_oVariant.IsUser=M4_FALSE;

	switch (aio_oVariant.Type){
	case M4CL_CPP_TYPE_NUMBER:
		{
			break;
		}
	case M4CL_CPP_TYPE_DATE:
		{
			aio_oVariant.Type=M4CL_CPP_TYPE_NUMBER;
			break;
		}
	case M4CL_CPP_TYPE_STRING_POOL:
		{
			if (m_poPool){
				pszTemp=m_poPool->GetString (aio_oVariant.Data.CharData); //sacamos la cadena
				m4char_t *stop;
				dTemp=strtod (pszTemp, &stop);			
				m_poPool->DeleteString(aio_oVariant.Data.CharData); //borramos al cadena
				aio_oVariant.Type=M4CL_CPP_TYPE_NUMBER;
				aio_oVariant.Data.DoubleData=dTemp;
			}
			else{
				ret=M4_ERROR;
			}

			break;
		}
			
	case M4CL_CPP_TYPE_STRING_VAR:
	case M4CL_CPP_TYPE_STRING_SYS:	
		{
			
			pszTemp=aio_oVariant.Data.PointerChar;
			m4char_t *stop;
			dTemp=strtod (pszTemp, &stop);
			aio_oVariant.FreeData();
			aio_oVariant.Type=M4CL_CPP_TYPE_NUMBER;
			aio_oVariant.Data.DoubleData=dTemp;
			break;
						
		}
    default: 
        {
			aio_oVariant.Type=M4CL_CPP_TYPE_NUMBER;
			aio_oVariant.Data.DoubleData=0.0;
        }
	}

	return ret;
}


m4return_t ClVariantConversor::ToDate(m4VariantType &aio_oVariant){

	m4return_t ret;
	m4pchar_t	pszTemp;
	m4double_t dTemp;

	ret=M4_SUCCESS;
	m_bDateOverflow=M4_FALSE;
	aio_oVariant.IsUser=M4_FALSE;

	switch (aio_oVariant.Type){
	case M4CL_CPP_TYPE_DATE:
		{
			break;
		}
	case M4CL_CPP_TYPE_NUMBER:
		{
			/* Bug 0065665
			Hay que controlar los rangos y sino se da warning
			*/
			if (aio_oVariant.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_PLUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			} else if (aio_oVariant.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_MINUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			}

			aio_oVariant.Type=M4CL_CPP_TYPE_DATE;
			break;
		}
	case M4CL_CPP_TYPE_STRING_POOL:
		{
			if (m_poPool){
				pszTemp=m_poPool->GetString (aio_oVariant.Data.CharData); //sacamos la cadena            			
				ClStringToDate (pszTemp, dTemp); //intentamos convertir a fecha			

				/* Bug 0065665
				Hay que controlar los rangos y sino se da warning
				*/
				if (dTemp>M4CL_PLUS_INFINITE_IN_JULIAN){
					DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, pszTemp << M4CL_PLUS_INFINITE_DATE ) ;
					dTemp=M4CL_PLUS_INFINITE_IN_JULIAN;
					m_bDateOverflow=M4_TRUE;
				} else if (dTemp<M4CL_MINUS_INFINITE_IN_JULIAN){
					DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, pszTemp << M4CL_MINUS_INFINITE_DATE ) ;
					dTemp=M4CL_MINUS_INFINITE_IN_JULIAN;
					m_bDateOverflow=M4_TRUE;
				}

				m_poPool->DeleteString(aio_oVariant.Data.CharData); //borramos al cadena
				aio_oVariant.Type=M4CL_CPP_TYPE_DATE;
				aio_oVariant.Data.DoubleData = dTemp;
			}
			else{
				ret=M4_ERROR;
			}

			break;	
		}
			
	case M4CL_CPP_TYPE_STRING_VAR:
	case M4CL_CPP_TYPE_STRING_SYS:
		{
			
			pszTemp=aio_oVariant.Data.PointerChar;
            ClStringToDate (pszTemp , dTemp); //intentamos convertir a fecha

			/* Bug 0065665
			Hay que controlar los rangos y sino se da warning
			*/
			if (dTemp>M4CL_PLUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, pszTemp << M4CL_PLUS_INFINITE_DATE ) ;
				dTemp=M4CL_PLUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			} else if (dTemp<M4CL_MINUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, pszTemp << M4CL_MINUS_INFINITE_DATE ) ;
				dTemp=M4CL_MINUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			}

			aio_oVariant.FreeData();
			aio_oVariant.Type=M4CL_CPP_TYPE_DATE;
			aio_oVariant.Data.DoubleData = dTemp;
			
			break;	
		}
    default: 
        {
			aio_oVariant.Type=M4CL_CPP_TYPE_DATE;
			aio_oVariant.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;

        }
	}

	return ret;
}



m4return_t ClVariantConversor::ToPoolString(m4VariantType &aio_oVariant){

	m4return_t ret;
	m4uint32_t	hChar;

	m_bDateOverflow=M4_FALSE;
	if (!m_poPool){
		return M4_ERROR;
	}

	ret=M4_SUCCESS;

	switch (aio_oVariant.Type){

	case M4CL_CPP_TYPE_STRING_POOL:
		{
			break;
		}

	case M4CL_CPP_TYPE_STRING_VAR:
	case M4CL_CPP_TYPE_STRING_SYS:
		{
			ret=m_poPool->AddString(aio_oVariant.Data.PointerChar, hChar);
			if (ret==M4_SUCCESS){
				aio_oVariant.FreeData();
				aio_oVariant.Data.CharData= hChar;	
				aio_oVariant.Type=M4CL_CPP_TYPE_STRING_POOL;
			}

			break;
		}

	case M4CL_CPP_TYPE_DATE:
		{
			/* Bug 0065665
			Hay que controlar los rangos y sino se da warning
			*/
			if (aio_oVariant.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_PLUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			} else if (aio_oVariant.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_MINUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			}

			ClDateToString (aio_oVariant.Data.DoubleData, m_acBuffer, M4CL_MEDVARINT_MAX_STRING ); 
            m_acBuffer[M4CL_MEDVARINT_MAX_STRING-1]=0; //por si acaso

			ret=m_poPool->AddString(m_acBuffer, hChar);
			if (ret==M4_SUCCESS){
				aio_oVariant.Data.CharData= hChar;	
				aio_oVariant.Type=M4CL_CPP_TYPE_STRING_POOL;
			}
			break;
		}

	case M4CL_CPP_TYPE_NUMBER:
		{
			sprintf (m_acBuffer, "%.8lf",aio_oVariant.Data.DoubleData);
			ret=m_poPool->AddString(m_acBuffer, hChar);
			if (ret==M4_SUCCESS){
				aio_oVariant.Data.CharData= hChar;	
				aio_oVariant.Type=M4CL_CPP_TYPE_STRING_POOL;
			}
			
			break;
		}

    default: 
        {
			ret=m_poPool->AddString("", hChar);
			if (ret==M4_SUCCESS){
				aio_oVariant.Data.CharData= hChar;
				aio_oVariant.Type=M4CL_CPP_TYPE_STRING_POOL;
			}
        }
	}

	return ret;
}


m4return_t ClVariantConversor::ToCharString(m4VariantType &aio_oVariant){

	m4return_t ret;
	m4pchar_t	pszTemp;
	
	m_bDateOverflow=M4_FALSE;

	ret=M4_SUCCESS;

	switch (aio_oVariant.Type){

	case M4CL_CPP_TYPE_STRING_VAR:
		{
			break;
		}
	case M4CL_CPP_TYPE_STRING_SYS:
		{
			pszTemp=new char [strlen(aio_oVariant.Data.PointerChar)+1];
			strcpy (pszTemp,aio_oVariant.Data.PointerChar);
			aio_oVariant.SetString(pszTemp, aio_oVariant.IsUser);
			delete[] pszTemp;
			break;

		}

	case M4CL_CPP_TYPE_STRING_POOL:
		{
			if (m_poPool){
				m4uint32_t uiCharData = aio_oVariant.Data.CharData;
				pszTemp=m_poPool->GetString(uiCharData);
				aio_oVariant.SetString(pszTemp, aio_oVariant.IsUser);
				m_poPool->DeleteString(uiCharData);
			}
			else{
				ret=M4_ERROR;
			}

			break;
		}

	case M4CL_CPP_TYPE_DATE:
		{		
			/* Bug 0065665
			Hay que controlar los rangos y sino se da warning
			*/
			if (aio_oVariant.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_PLUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			} else if (aio_oVariant.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_MINUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			}

			ClDateToString (aio_oVariant.Data.DoubleData, m_acBuffer, M4CL_MEDVARINT_MAX_STRING ); 
            m_acBuffer[M4CL_MEDVARINT_MAX_STRING-1]=0; //por si acaso
			aio_oVariant.SetString(m_acBuffer);
			break;
		}

	case M4CL_CPP_TYPE_NUMBER:
		{			
			sprintf (m_acBuffer, "%.8lf",aio_oVariant.Data.DoubleData);
            m_acBuffer[M4CL_MEDVARINT_MAX_STRING-1]=0; //por si acaso
    		aio_oVariant.SetString(m_acBuffer);
			break;
		}

    default: 
        {
			aio_oVariant.SetString("");
			
        }
	}

	return ret;
}

m4return_t ClVariantConversor::ToCharSysString(m4VariantType &aio_oVariant){

	m4return_t ret;
	m4pchar_t	pszTemp;
	
	m_bDateOverflow=M4_FALSE;

	ret=M4_SUCCESS;

	switch (aio_oVariant.Type){

	case M4CL_CPP_TYPE_STRING_VAR:
		{
			pszTemp=new char [strlen(aio_oVariant.Data.PointerChar)+1];
			strcpy (pszTemp,aio_oVariant.Data.PointerChar);
			aio_oVariant.SetSysString(pszTemp);
			delete[] pszTemp;
			break;
		}
	case M4CL_CPP_TYPE_STRING_SYS:
		{
			break;
		}

	case M4CL_CPP_TYPE_STRING_POOL:
		{
			if (m_poPool){
				m4uint32_t uiCharData = aio_oVariant.Data.CharData;
				pszTemp=m_poPool->GetString(uiCharData);
				aio_oVariant.SetSysString(pszTemp);
				m_poPool->DeleteString(uiCharData);
			}
			else{
				ret=M4_ERROR;
			}

			break;
		}

	case M4CL_CPP_TYPE_DATE:
		{			
			/* Bug 0065665
			Hay que controlar los rangos y sino se da warning
			*/
			if (aio_oVariant.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_PLUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			} else if (aio_oVariant.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
				DUMP_CHLOG_WARNINGF( M4_VARIANT_DATE_OVERFLOW, aio_oVariant.Data.DoubleData << M4CL_MINUS_INFINITE_DATE ) ;
				aio_oVariant.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
				m_bDateOverflow=M4_TRUE;
			}

			ClDateToString (aio_oVariant.Data.DoubleData, m_acBuffer, M4CL_MEDVARINT_MAX_STRING ); 
            m_acBuffer[M4CL_MEDVARINT_MAX_STRING-1]=0; //por si acaso
			aio_oVariant.SetSysString(m_acBuffer);
			break;
		}

	case M4CL_CPP_TYPE_NUMBER:
		{			
			sprintf (m_acBuffer, "%.8lf",aio_oVariant.Data.DoubleData);
            m_acBuffer[M4CL_MEDVARINT_MAX_STRING-1]=0; //por si acaso
    		aio_oVariant.SetSysString(m_acBuffer);
			break;
		}

    default: 
        {
			aio_oVariant.SetSysString("");
			
        }
	}

	return ret;
}


m4bool_t ClVariantConversor::IsDateOverflow( void )
{
	return m_bDateOverflow;
}