//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clargume.hpp
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
//    class arguments
//
//==============================================================================

#ifndef __CLARGUM_HPP__
#define __CLARGUM_HPP__
#include "m4dm_dll.hpp"
#include "m4var.hpp"

class M4_DECL_M4DM _AClArgument
{
public:

    virtual m4return_t Set(m4VariantType &ai_vArg) = 0;
    virtual m4return_t Get(m4VariantType &ai_vArg) = 0;

    virtual m4return_t GetName(m4pchar_t & ai_pNameArg) = 0;

	virtual m4return_t GetSize(ClSize &ao_size)=0;

protected:

    virtual m4return_t SetName(m4pchar_t ai_pNameArg) = 0;

friend class ClArgumentList;
friend class ClFilterUtility;
};

class M4_DECL_M4DM ClArgument : public _AClArgument
{
public:

    ClArgument(void);
   ~ClArgument(void);

    m4return_t Set(m4VariantType &ai_vArg);
    m4return_t Get(m4VariantType &ai_vArg);

    m4return_t GetName(m4pchar_t & ai_pNameArg);
	
	m4return_t GetSize(ClSize &ao_size);
protected:

    m4return_t SetName(m4pchar_t ai_pNameArg);

    m4VariantType m_vArgument;
    m4pchar_t m_pNameArgument;

friend class ClArgumentList;
friend class ClFilterUtility;
};

class M4_DECL_M4DM ClNo_Argument : public _AClArgument
{
public:

    m4return_t Set(m4VariantType &ai_vArg) { return M4_ERROR; }
    m4return_t Get(m4VariantType &ai_vArg){ return M4_ERROR; }

    m4return_t GetName(m4pchar_t & ai_pNameArg){ return M4_ERROR; };

	m4return_t GetSize(ClSize &ao_size);

protected:

    m4return_t SetName(m4pchar_t ai_pNameArg){ return M4_ERROR; }

friend class ClArgumentList;
friend class ClFilterUtility;
};

class M4_DECL_M4DM _ClArgumentList_Base
{
public:

    virtual m4int32_t Count(void) = 0;

    virtual _AClArgument & operator [] (m4int32_t ai_iNumArg) = 0;
    virtual _AClArgument & operator [] (m4pchar_t ai_pNameArg) = 0;

    virtual void operator = (_ClArgumentList_Base & ai_ClArgList) = 0;
    //iguala numero de argumentos y nombres

	virtual m4return_t GetSize(ClSize &ao_size);

    static ClNo_Argument sNoArgs;

    m4int32_t m_iNumArguments;
    ClArgument * m_pClArgumentList;

protected: 

    virtual m4return_t CreateList(m4int32_t ai_iNumArgs) = 0;
    virtual m4return_t DeleteAll(void) = 0;
};

class M4_DECL_M4DM ClArgumentList : public _ClArgumentList_Base
{
public:

    ClArgumentList(void);
   ~ClArgumentList(void);

    m4int32_t Count(void);

    _AClArgument & operator [] (m4int32_t ai_iNumArg);
    _AClArgument & operator [] (m4pchar_t ai_pNameArg);

    void operator = (_ClArgumentList_Base & ai_ClArgList); //iguala numero de argumentos y nombres
	
	m4return_t GetSize(ClSize &ao_size);
protected: 

    m4return_t CreateList(m4int32_t ai_iNumArgs);
    m4return_t DeleteAll(void);

friend class ClRegisterStaFilter;
friend class ClRegisterDynFilter;
friend class ClFilterUtility;
};

class M4_DECL_M4DM ClArgumentList_Nihil : public _ClArgumentList_Base
{
public:

    m4int32_t Count(void)
    {
        return 0;
    }

    _AClArgument & operator [] (m4int32_t ai_iNumArg)
    {
        return sNoArgs;
    }

    _AClArgument & operator [] (m4pchar_t ai_pNameArg)
    {
        return sNoArgs;
    }

    void operator = (_ClArgumentList_Base & ai_ClArgList) //iguala numero de argumentos y nombres
    {
        return ;
    }
	
	m4return_t GetSize(ClSize &ao_size);
protected: 

    m4return_t CreateList(m4int32_t ai_iNumArgs)
    {
        return M4_SUCCESS;
    }

    m4return_t DeleteAll(void)
    {
        return M4_SUCCESS;
    }
};

#endif //__CLARGUM_HPP__
