//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             regtot.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    register totalizer
//
//
//==============================================================================


#include "regtot.hpp"

#include "dmres.hpp"
#include "handles.hpp"
#include "aregister.hpp"
#include "nodedef.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"



m4return_t ClRegTotalize::Get( m4VariantType &ao_vTotValOut, m4char_t TotType )
{
	m4return_t	iResult = _Get( ao_vTotValOut, TotType, M4_TRUE ) ;
	return( iResult ) ;
}


m4return_t ClRegTotalize::GetNoConcat( m4VariantType &ao_vTotValOut, m4char_t TotType )
{
	m4return_t	iResult = _Get( ao_vTotValOut, TotType, M4_FALSE ) ;
	return( iResult ) ;
}


m4return_t ClRegTotalize::Count( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Count" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base, m4VariantType, ClGetValue>::Count( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Count( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::Sum( m4VariantType &ao_vTotValOut, m4bool_t ai_bConcatStrings )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Sum" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Sum( ao_vTotValOut, ai_bConcatStrings );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Sum( ao_vTotValOut, ai_bConcatStrings );
	}

}

m4return_t ClRegTotalize::Avg( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Average" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base, m4VariantType, ClGetValue>::Avg( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base, m4VariantType, ClGetValue>::Avg( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::Max( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Maximun" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Max( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Max( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::Min( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Minimum" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Min( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Min( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::First( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "First" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::First( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::First( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::Last( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "Last" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Last( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Last( ao_vTotValOut );
	}

}

m4return_t ClRegTotalize::WeightedSum( m4VariantType &ao_vTotValOut )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, "WeightedSum" << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::WeightedSum( ao_vTotValOut );

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::WeightedSum( ao_vTotValOut );
	}

}



ClRegTotalize & ClRegTotalize::operator[] (const ClItemIndex &ai_itemindex)
{
	Index = ai_itemindex;
	return *this;
}

ClRegTotalize & ClRegTotalize::operator[] (const ClHandle &ai_itemhandle)
{
	ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

	Index = m_tIterator->Item [ ai_itemhandle ].GetIndex();

	m_tIterator->Item.MoveTo( CurrentIndex );

	return *this;
}

ClRegTotalize & ClRegTotalize::operator[] (m4pcchar_t ai_itemid)
{
	ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

	Index = m_tIterator->Item [ ai_itemid ].GetIndex();

	m_tIterator->Item.MoveTo( CurrentIndex );

	return *this;
}


m4return_t ClRegTotalize::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(Index);
	return ClTotalize <_ClRegister_Base, m4VariantType, ClGetValue>::GetSize(ao_size);
}


m4return_t ClRegTotalize::_Get( m4VariantType &ao_vTotValOut, m4char_t TotType, m4bool_t ai_bConcatStrings )
{
	m4return_t res;


	/* Bugs 0080430, 0080442, 0080443
	No se dejan hacer totalizaciones en carga parcial
	*/
	ClNodeDef	*poNodeDef = ( ( _ClRegister_Base* )m_tIterator )->GetpNodeDef() ;
    CHECK_CHLOG_ERRORF( poNodeDef->NumRows() > 0, M4_ERROR, M4_CH_DM_PARTIAL_LOAD_TOTALIZE, M4ClIdTotalize( TotType ) << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;


	if(Index != M4DM_EOF_INDEX)
	{
		ClItemIndex CurrentIndex = m_tIterator->Item.GetIndex();

		if(! m_tIterator->Item.MoveTo( Index ))
		{
			m_tIterator->Item.MoveTo( CurrentIndex );

			return M4_ERROR;
		}
		else
		{
			if( ai_bConcatStrings == M4_TRUE )
			{
				res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Get( ao_vTotValOut, TotType );
			}
			else
			{
				res = ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::GetNoConcat( ao_vTotValOut, TotType );
			}

			m_tIterator->Item.MoveTo( CurrentIndex );

			return res;
		}
	}
	else
	{
		if( ai_bConcatStrings == M4_TRUE )
		{
			return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Get( ao_vTotValOut, TotType );
		}
		else
		{
			return ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::GetNoConcat( ao_vTotValOut, TotType );
		}
	}
}
