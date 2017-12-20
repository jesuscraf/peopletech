//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clfuvar.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             5/05/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class FilterUtilityVariableArguments
//
//==============================================================================

#include "clargume.hpp"
#include "sizer.hpp"

ClNo_Argument _ClArgumentList_Base::sNoArgs;

ClArgument::ClArgument(void)
{
    m_pNameArgument = NULL;
}

ClArgument::~ClArgument(void)
{
    if (m_pNameArgument != NULL)
	{
        delete [] m_pNameArgument;
		m_pNameArgument = NULL ;
	}
}

m4return_t ClArgument::Set(m4VariantType &ai_vArg)
{
	// Se hace siempre una copia ;
	m_vArgument.Set( ai_vArg, M4_TRUE ) ;
    return M4_SUCCESS;
}

m4return_t ClArgument::Get(m4VariantType & ai_vArg)
{
    ai_vArg = m_vArgument;
    return M4_SUCCESS;
}

m4return_t ClArgument::SetName(m4pchar_t ai_pNameArg)
{
    if (m_pNameArgument != NULL)
	{
        delete [] m_pNameArgument;
		m_pNameArgument = NULL ;
	}

	if( ai_pNameArg != NULL )
	{
		m_pNameArgument = new m4char_t[strlen (ai_pNameArg) + 1];
		strcpy (m_pNameArgument, ai_pNameArg);
	}

    return M4_SUCCESS;
}

m4return_t ClArgument::GetName(m4pchar_t & ai_pNameArg)
{
    ai_pNameArg = m_pNameArgument;
    return M4_SUCCESS;
}






// Lista de argumentos ========================================================

ClArgumentList::ClArgumentList(void)
{
    m_pClArgumentList = 0;
    m_iNumArguments = 0;
}

ClArgumentList::~ClArgumentList(void)
{
	DeleteAll() ;
}

m4return_t ClArgumentList::CreateList(m4int32_t ai_iNumArgs)
{
	DeleteAll() ;

    m_pClArgumentList = new ClArgument[ai_iNumArgs];
    m_iNumArguments = ai_iNumArgs;

    return M4_SUCCESS;
}

m4return_t ClArgumentList::DeleteAll(void)
{
    if (m_pClArgumentList != NULL )
	{
        delete [] m_pClArgumentList;
		m_pClArgumentList = NULL ;
	}

    m_iNumArguments = 0;

    return M4_SUCCESS;
}

m4int32_t ClArgumentList::Count(void)
{
    return m_iNumArguments;
}

_AClArgument & ClArgumentList::operator [] (m4int32_t ai_iNumArg)
{
    if (ai_iNumArg < m_iNumArguments)
        return m_pClArgumentList[ai_iNumArg];
    else return ClArgumentList::sNoArgs;
}

_AClArgument & ClArgumentList::operator [] (m4pchar_t ai_pNameArg)
{
    M4_ASSERT(ai_pNameArg);

    m4int32_t i = 0;
    m4pchar_t pName = 0;

    while (i < m_iNumArguments)
    {
        m_pClArgumentList[i].GetName(pName);
        if (pName)
        {
            if (!(strcmp(pName, ai_pNameArg)))
                break;
        }

        i++;
    }

    if (i < m_iNumArguments)
        return m_pClArgumentList[i];
    else return ClArgumentList::sNoArgs;
}

void _ClArgumentList_Base::operator = (_ClArgumentList_Base & ai_ClArgList)
{
    m4int32_t i;
    m4pchar_t pNameArg;
    m4VariantType vv;

    if (ai_ClArgList.m_iNumArguments)
    {
        CreateList(ai_ClArgList.m_iNumArguments);

        for (i = 0; i < m_iNumArguments; i ++)
        {
            ai_ClArgList[i].GetName(pNameArg);
//            m_pClArgumentList[i].SetName(pNameArg);
            ai_ClArgList[i].Get(vv);
            m_pClArgumentList[i].Set(vv);
        }
    }
}

void ClArgumentList::operator = (_ClArgumentList_Base & ai_ClArgList)
{
    m4int32_t i;
    m4pchar_t pNameArg;
    m4VariantType vv;

    if (ai_ClArgList.m_iNumArguments)
    {
        CreateList(ai_ClArgList.m_iNumArguments);

        for (i = 0; i < m_iNumArguments; i ++)
        {
            ai_ClArgList[i].GetName(pNameArg);
            m_pClArgumentList[i].SetName(pNameArg);
            ai_ClArgList[i].Get(vv);
            m_pClArgumentList[i].Set(vv);
        }
    }
}

m4return_t 
_ClArgumentList_Base::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(_ClArgumentList_Base);
	if (m_pClArgumentList){
		for (m4int32_t i=0;i<m_iNumArguments;++i){
			if (m_pClArgumentList[i].GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		ao_size+=sizeof(int); // array de clases con destructor. Se reserva espaciopara el tamaño.
	}

	return M4_SUCCESS;
}

m4return_t 
ClArgumentList::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClArgumentList);
	    ao_size-=sizeof(_ClArgumentList_Base);

	if (_ClArgumentList_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

m4return_t 
ClArgumentList_Nihil::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClArgumentList_Nihil);
	    ao_size-=sizeof(_ClArgumentList_Base);

	if (_ClArgumentList_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

m4return_t 
ClNo_Argument::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNo_Argument);

	return M4_SUCCESS;
}

m4return_t 
ClArgument::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClArgument);
	if (m_pNameArgument != NULL){
		ao_size+=strlen(m_pNameArgument)+1;
	}
	if (m_vArgument.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}




