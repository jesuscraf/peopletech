//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         CVM - DM
// File:           totalize.hpp
// Project:        CVM
// Author:         Meta Software M.S. , S.A
// Date:            27 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//      ClMMIndex: 'Real' index, used in the memory manager
//      class used to allow a stronger type checking
//==============================================================================

#ifndef __MMINDEX_HPP__
#define __MMINDEX_HPP__
#include "m4dm_dll.hpp"
	#ifndef ClMMIndex

	#include "m4types.hpp"
    #include "index.hpp"

        extern M4_DECL_M4DM m4uint32_t ClMMIndex_EOF;

		#if 0
        //nunca entra aquí, desactivado

		class ClMMIndex
		{
		public:            
            ClMMIndex (m4uint32_t ai_i)
			{
				m_i = ai_i;
			}
            
            ClMMIndex (void)
			{
				m_i = ClMMIndex_EOF;
			}            

		   ~ClMMIndex () {}            
    
			operator m4uint32_t ()
			{
				return m_i;
			}

        const ClMMIndex &operator++ ()
        {
            m_i++;
            return *this;
        }

        const ClMMIndex &operator-- ()
        {
            m_i--;
            return *this;
        }

        const ClMMIndex &operator = (const m4uint32_t ai_iNewIndex)
        {
            m_i = ai_iNewIndex;
            return *this;
        }

		m4return_t GetSize(ClSize &ao_size)
		{
			ao_size+=sizeof(m_i);
			return M4_SUCCESS;
		}

        private:
			m4uint32_t m_i;
		};

		#else

		typedef m4uint32_t ClMMIndex;

		#endif

	#endif

#endif // __MMINDEX_HPP__
