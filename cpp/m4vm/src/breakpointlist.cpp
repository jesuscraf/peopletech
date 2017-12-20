//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             breakpointlist.cpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Execution breakpoint list
//
//
//==============================================================================

#include "breakpointlist.hpp"
#include "clgeniod.hpp"
	
//------------------
//Add
//------------------
m4return_t	ClBreakpointList::Add(const ClBreakpoint &ai_oBP){

	m4bool_t	bResult = M4_TRUE ;


	bResult = (m_oSet.insert (ai_oBP)).second ;

	if( bResult == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


//------------------
//Remove (by ID's)
//------------------
m4return_t  ClBreakpointList::Remove(const ClBreakpoint &ai_oBP){
	
	m4int_t res=m_oSet.erase(ai_oBP);
	
	if (res==0) return M4_ERROR;
#ifdef _DEBUG
	if (res>1) return M4_WARNING; //ha borrado + de uno que cumple ¿como?
#endif
	return M4_SUCCESS;
}

//------------------
//Get (by ID's)
//------------------
ClBreakpoint*	ClBreakpointList::Get(const ClBreakpoint &ai_oBP){
	
	BreakpointSetIt_t it;
	it=m_oSet.find (ai_oBP);
	if (it == m_oSet.end() ){
		return NULL;
	}
	return (ClBreakpoint*) (&(*it));
}
	
//------------------
//Remove (by pos)
//------------------
m4return_t ClBreakpointList::Remove(m4uint32_t ai_iId){
	BreakpointSetIt_t it;

	if (ai_iId>m_oSet.size ()-1){
		return M4_ERROR;
	}
	
	it=m_oSet.begin ();
	advance(it,ai_iId);
	
	m_oSet.erase (it);
	return M4_SUCCESS;
}

//------------------
//Get (by pos)
//------------------
m4bool_t ClBreakpointList::Get(m4uint32_t ai_iId, ClBreakpoint &ao_oBP){
	BreakpointSetIt_t it;

	if (ai_iId>m_oSet.size ()-1){
		return M4_FALSE;
	}
	
	it=m_oSet.begin ();
	advance(it,ai_iId);

	ao_oBP=*(it);
	return M4_TRUE;

}

//------------------
//RemoveAll
//------------------
m4return_t ClBreakpointList::RemoveAll(void){
	m_oSet.clear();
	return M4_SUCCESS;
}

//------------------
//GetSize
//------------------
m4uint32_t ClBreakpointList::GetSize(void){
	return m_oSet.size ();
}


m4return_t ClBreakpointList::Serialize(ClGenericIODriver& IOD) {
	
	BreakpointSetIt_t it=m_oSet.begin();
	m4return_t ret=M4_SUCCESS;

	IOD.Write(GetSize()); //metemos el tamaño

	while (it!=m_oSet.end()){
		ret=(*it).Serialize(IOD);
		if (ret!=M4_SUCCESS) {
			return ret;
		}
		++it;
	}
	return ret;

}
m4return_t ClBreakpointList::DeSerialize(ClGenericIODriver& IOD){
	m4return_t ret;
	m4uint32_t iNumBPs,i;
	ClBreakpoint oBP;
    
	if ( (ret = IOD.Read(iNumBPs) ) != M4_SUCCESS){
        return ret;
	}
	
	/* Bug 0080915
	El borrado debe hacerse en la deserialización
	*/
	m_oSet.clear();

	for (i=0; i<iNumBPs; ++i){
		//Deserializamos en un BP temporal
		ret=oBP.DeSerialize(IOD);
		if (ret!=M4_SUCCESS){
			return ret;
		}

		//lo metemos en la lista
		if ( (m_oSet.insert (oBP)).second != M4_TRUE ){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}
