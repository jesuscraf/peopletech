//## begin module%39B7B9D702B7.cm preserve=no
//## end module%39B7B9D702B7.cm

//## begin module%39B7B9D702B7.cp preserve=no
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
//## end module%39B7B9D702B7.cp

//## Module: garbagecontainer%39B7B9D702B7; Package body
//## Subsystem: M4ServerBase::garbagecollector::src%382154160271
//## Source file: F:\integration\m4serverbase\src\garbagecontainer.cpp

//## begin module%39B7B9D702B7.additionalIncludes preserve=no
//## end module%39B7B9D702B7.additionalIncludes

//## begin module%39B7B9D702B7.includes preserve=yes
#include <garbageableinterface.hpp>
#include <blocksynchronization.hpp>
//## end module%39B7B9D702B7.includes

// CriticalSection
#include <criticalsection.hpp>
// FastQ
#include <fastfifoq.hpp>
// garbagecontainer
#include <garbagecontainer.hpp>
// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>


//## begin module%39B7B9D702B7.declarations preserve=no
//## end module%39B7B9D702B7.declarations

//## begin module%39B7B9D702B7.additionalDeclarations preserve=yes
//## end module%39B7B9D702B7.additionalDeclarations


// Class ClGarbageContainer 



ClGarbageContainer::ClGarbageContainer (m4int_t ai_iContainerSize)
  //## begin ClGarbageContainer::ClGarbageContainer%968401157.hasinit preserve=no
  //## end ClGarbageContainer::ClGarbageContainer%968401157.hasinit
  //## begin ClGarbageContainer::ClGarbageContainer%968401157.initialization preserve=yes
  :m_oGarbageContainer ( ai_iContainerSize ) , m_oBlocker(M4_TRUE)
  //## end ClGarbageContainer::ClGarbageContainer%968401157.initialization
{
  //## begin ClGarbageContainer::ClGarbageContainer%968401157.body preserve=yes
  //## end ClGarbageContainer::ClGarbageContainer%968401157.body
}


ClGarbageContainer::~ClGarbageContainer ()
{
  //## begin ClGarbageContainer::~ClGarbageContainer%968401158.body preserve=yes
  //## end ClGarbageContainer::~ClGarbageContainer%968401158.body
}



//## Other Operations (implementation)
m4return_t ClGarbageContainer::GetElement (IRecycleable *&ao_poElement)
{
  //## begin ClGarbageContainer::GetElement%968401153.body preserve=yes
	IGarbageable *poToDestroy = NULL ;
	ClMutBlock	oSectionBlocker(m_oBlocker);
	m4uint32_t iGarbageSize ;
	m4return_t iRet = M4_ERROR ;


	ao_poElement = NULL ;

	iGarbageSize = m_oGarbageContainer.Size() ;

	if ( iGarbageSize > 0 )
	{
		m_oGarbageContainer.Pop(poToDestroy);

		if ( M4_SUCCESS == poToDestroy -> Recycle () )
		{
			ao_poElement = poToDestroy;

			iRet = M4_SUCCESS ;
		}
	}


	return iRet ;
  //## end ClGarbageContainer::GetElement%968401153.body
}

m4return_t ClGarbageContainer::SetElement (IRecycleable &ai_oElement)
{
  //## begin ClGarbageContainer::SetElement%968401154.body preserve=yes
	ClMutBlock	oSectionBlocker(m_oBlocker);

	m4uint32_t iSize = m_oGarbageContainer.Size() ;
	if (m_oGarbageContainer.m_lUpperBound == iSize )
	{
		return M4_ERROR ;
	}

	return m_oGarbageContainer.Push((IGarbageable*)&ai_oElement);
  //## end ClGarbageContainer::SetElement%968401154.body
}

m4return_t ClGarbageContainer::DisposeOf (IGarbageable &ai_oGarbage)
{
  //## begin ClGarbageContainer::DisposeOf%968401155.body preserve=yes
	ClMutBlock	oSectionBlocker(m_oBlocker);

	if (m_oGarbageContainer.m_lUpperBound == m_oGarbageContainer.Size())
		_Purge();

	return m_oGarbageContainer.Push(&ai_oGarbage);
  //## end ClGarbageContainer::DisposeOf%968401155.body
}

m4uint32_t ClGarbageContainer::Purge ()
{
  //## begin ClGarbageContainer::Purge%968401156.body preserve=yes
	ClMutBlock	oSectionBlocker(m_oBlocker);

	return _Purge();
  //## end ClGarbageContainer::Purge%968401156.body
}

m4uint32_t ClGarbageContainer::_Purge ()
{
  //## begin ClGarbageContainer::_Purge%968401159.body preserve=yes
	IGarbageable *poToDestroy;
	m4uint32_t iSize , i;
    m4uint32_t iTotalSize = 0;

	while (0 < (iSize = m_oGarbageContainer.Size()))
	{
        iTotalSize += iSize;
		for (i = 0 ; i< iSize ;i ++ )
		{
			m_oGarbageContainer.Pop(poToDestroy);
			poToDestroy->Destroy();
		}
	}

    return iTotalSize;
  //## end ClGarbageContainer::_Purge%968401159.body
}

// Additional Declarations
  //## begin ClGarbageContainer%39B8D825019B.declarations preserve=yes
  //## end ClGarbageContainer%39B8D825019B.declarations

//## begin module%39B7B9D702B7.epilog preserve=yes
//## end module%39B7B9D702B7.epilog
