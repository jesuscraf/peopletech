#include "m4objglb.hpp"
#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           clbsearch.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    
////
//==============================================================================

#include "clbsearch.hpp"
#include "chlog.hpp"
#include "m4mdrt.hpp"
#include "aregister.hpp"
#include "access.hpp"
#include "m4sysalloc.hpp"
#include "nodedef.hpp"
#include "m4unicode.hpp"


inline m4bool_t M4VarTestNULL (const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, m4int8_t &ai_res)
{
	m4bool_t bResult = M4_TRUE;

    if (pstVariant1->Type == M4CL_CPP_TYPE_NULL) {
        if (pstVariant2->Type == M4CL_CPP_TYPE_NULL) {
            ai_res = 0;
        }
        else {
            ai_res = -1;
        }
    }
    else {		
        if (pstVariant2->Type == M4CL_CPP_TYPE_NULL) {			
			ai_res = +1;
        } else {
			bResult = M4_FALSE;
		}
    }

    return bResult;
}

m4int8_t M4VarStrCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 )
{
    m4int8_t res;

    if (M4VarTestNULL (pstVariant1, pstVariant2, res)) {
        return res;
    }

    M4_ASSERT (pstVariant1->Data.PointerChar);
    M4_ASSERT (pstVariant2->Data.PointerChar);

	// UNICODE LOCALE
    int iDiff = M4StrCmpi( pstVariant1->Data.PointerChar, pstVariant2->Data.PointerChar ) ;
    
    if ( iDiff == 0 ) 
		return 0;
	else if ( iDiff > 0 )
		return +1;
	else 
		return -1;
}

m4int8_t M4VarDoubleCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 )
{
    m4int8_t res;

    if (M4VarTestNULL (pstVariant1, pstVariant2, res)) {
        return res;
    }

	m4double_t dDiff;

	dDiff = pstVariant1->Data.DoubleData - pstVariant2->Data.DoubleData;

	if ( fabs (dDiff) <= M4_LN4_MATH_PRECISION )	//if ( dDiff == 0 ) 
		return 0;
	else if ( dDiff > 0 )
		return +1;
	else 
		return -1;
}

/*inline

m4int8_t ClBSItemIndex::VarCompare( const m4VariantType &pstVariant1, const m4VariantType &pstVariant2 ) const
{
	return pfCompareVar (&pstVariant1, &pstVariant2);
}

ClHandle ClBSItemIndex::GetItemHandle (void) const
{
    return hHandle;
}

m4int8_t ClBSItemIndex::GetOrder (void) const
{
    return iIndexOrder;
}

m4int8_t ClBSItemIndex::GetFunction (void) const
{
    return iIndexFunction;
}
*/

//-----------------------------------------------------------------------------

ClBSearchIndex::ClBSearchIndex ( void )
{
	m_poItemIndex = NULL;
	//ai_pstArguments = NULL;
	m_iNumItemIndex = 0;
    m_Id = 0;
}

ClBSearchIndex::~ClBSearchIndex ( void )
{
	if ( m_poItemIndex != NULL )
		delete [] m_poItemIndex;
}

ClBSItemIndex *ClBSearchIndex::Get (m4uint32_t ai_iPos) const
{
    return m_poItemIndex + ai_iPos;
}

m4uint32_t ClBSearchIndex::Count (void) const
{
    return m_iNumItemIndex;
}

//llamado por el tema del carril, pq los indices pueden haberse quedado
//desactualizados:
m4return_t ClBSearchIndex::Update (ClCompiledMCR *poCMCR)
{
	m4uint16_t i;

    for ( i = 0; i < m_iNumItemIndex; i++ )
	{
        m_poItemIndex[i].iIndex = poCMCR->GetItemIndex (m_poItemIndex[i].hHandle);
    }

    return M4_SUCCESS;
}

m4return_t ClBSearchIndex::Init (_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders)
{
	m4uint16_t i;
	m4uint32_t hItem;	
	m4uint8_t iCppType;
	
	m_iNumItemIndex = ai_iNumberItems;
	
	if (m_iNumItemIndex) {
		m_poItemIndex = new ClBSItemIndex[m_iNumItemIndex];
		
		for ( i = 0; i < m_iNumItemIndex; i++ )
		{
			hItem = ai_pClRegister->Item[ai_pItemsNames[i]].ItemDef.GetHandle();
			
			if( hItem == 0 )
			{
				DUMP_CHLOG_ERRORF(M4_CH_DM_SORT_FILTER_NO_ITEM, ai_pItemsNames[i] <<
					M4ObjNodeN((ai_pClRegister->Item.GetpAccess())->GetpCMCR(),
					ai_pClRegister->GetpNodeDef()->GetHandle()));
				
				return M4_ERROR;
			}
			
			m_poItemIndex[i].hHandle = hItem;
			m_poItemIndex[i].iIndex = ai_pClRegister->Item.GetIndex();
			
			m_poItemIndex[i].iIndexFunction = M4CL_INDEX_FUNCTION_EQUAL;
			
			if (ai_pOrders[i] == M4CL_INDEX_SEARCH_WAY_ASC)
				m_poItemIndex[i].iIndexOrder = +1;
			else if (ai_pOrders[i] == M4CL_INDEX_SEARCH_WAY_DESC)
				m_poItemIndex[i].iIndexOrder = -1;
			else
			{            
				DUMP_CHLOG_DEBUGF(M4_CH_DM_SORT_FILTER_BAD_ORDER,
					M4ObjNodeItemI(ai_pClRegister->Item.ItemDef.GetpCMCR(),
					ai_pClRegister->Item.ItemDef.GetHandle()));
				
				m_poItemIndex[i].iIndexOrder = -1;
			}
			
			iCppType = ai_pClRegister->Item.ItemDef.CppType();
			
			switch (iCppType)
			{
			case    M4CL_CPP_TYPE_NUMBER :
			case    M4CL_CPP_TYPE_DATE :
				m_poItemIndex[i].pfCompareVar = M4VarDoubleCompare;
				break ;
				
			case    M4CL_CPP_TYPE_STRING_VAR :
			case	M4CL_CPP_TYPE_STRING:
				m_poItemIndex[i].pfCompareVar = M4VarStrCompare;
				break ;
				
			default :
				DUMP_CHLOG_ERRORF( M4_CH_DM_SORT_FILTER_ITEM_TYPE_MISMATCH, M4ObjNodeItemI((ai_pClRegister->Item.GetpAccess())->GetpCMCR(), hItem));
				//ERROR No tipos correctos
				return M4_ERROR ;
				break ;
			}
		}
	}
	
	return M4_SUCCESS;
}

m4return_t ClBSearchIndex::Init (ClCompiledMCR *poCMCR, m4uint32_t hIndex, m4uint32_t ai_id)
{
	m4uint16_t i;
	m4uint32_t hItem;	
	m4uint8_t iWay, iCppType, iIndexFunction;

	//ai_pstArguments = ai_pstArguments;

	m_iNumItemIndex = poCMCR->GetIndexNumberOfItems (hIndex);

    m_Id = ai_id;

if (m_iNumItemIndex) {

	m_poItemIndex = new ClBSItemIndex[m_iNumItemIndex];

	for ( i = 0; i < m_iNumItemIndex; i++ )
	{
		hItem = poCMCR->GetIndexItemHandleByPosition (hIndex, i);

		if( hItem == 0 )
		{
			DUMP_CHLOG_ERRORF( M4_CH_DM_BAD_ITEM_INDEX, i<<poCMCR->GetIndexId(hIndex) );
			return M4_ERROR;
		}

		m_poItemIndex[i].hHandle = hItem;
        m_poItemIndex[i].iIndex = poCMCR->GetItemIndex (hItem);

		iIndexFunction = poCMCR->GetIndexFunctionByPosition (hIndex, i);
		
		if (( iIndexFunction != M4CL_INDEX_FUNCTION_EQUAL ) && ( i != m_iNumItemIndex - 1 ))
		{
			DUMP_CHLOG_ERRORF( M4_CH_DM_BAD_OPERATION_ITEM_INDEX, M4ObjNodeItemI(poCMCR,hItem)<<poCMCR->GetIndexId(hIndex) );
			//Solo puede ser distinto de EQUAL si es el último item.
			return M4_ERROR;
		}

		m_poItemIndex[i].iIndexFunction = iIndexFunction;

		iWay = poCMCR->GetIndexWayByPosition (hIndex, i);

		if (iWay == M4CL_INDEX_SEARCH_WAY_ASC )  //Ascendente
			m_poItemIndex[i].iIndexOrder = +1;
		else									//Descendente
			m_poItemIndex[i].iIndexOrder = -1;
        
		iCppType = poCMCR->GetItemCppType (hItem);

        /*
		if ( ai_pstArguments[i].Type != iCppType )
		{
			//SETCODEF( M4_CH_VM_TYPE_MISMATCH, M4_ERROR, "Type mismatch between the BSearch argument and the Item = <%d>", hItem ) ;
			DUMP_CHLOG_ERRORF( M4_CH_VM_TYPE_MISMATCH, M4ObjNodeItemI(poCMCR,hItem) ) ;
			//No coinciden tipos
			return M4_ERROR;
		}
        */

		switch (iCppType)
		{
			case    M4CL_CPP_TYPE_NUMBER :
            case    M4CL_CPP_TYPE_DATE :
				m_poItemIndex[i].pfCompareVar = M4VarDoubleCompare;
                break ;

            case    M4CL_CPP_TYPE_STRING_VAR :
			case	M4CL_CPP_TYPE_STRING:
				m_poItemIndex[i].pfCompareVar = M4VarStrCompare;
                break ;

            default :
				DUMP_CHLOG_ERRORF( M4_CH_DM_SORT_FILTER_ITEM_TYPE_MISMATCH,
                    M4ObjNodeItemI(poCMCR, hItem));
				//ERROR No tipos correctos
                return M4_ERROR ;
                break ;
        }
	}
}

	return M4_SUCCESS;
}

m4int8_t ClBSearchIndex::CompareIndex ( _ClRegister_Base & ai_poRegister, m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions)
{
/*CompareIndex devuelve:
		-1 significa que el registro en el que buscamos está por debajo del valor buscado
		+1 significa que el registro en el que buscamos está por encima del valor buscado
		 0 significa que el registro en el que buscamos es el buscado
*/

//La variable IndexOrder es ASC = 1 y DESC = -1, se utiliza para cambiar el valor de las 
//comparaciones y se busque en el sentido adecuado: el resultado de vuelta indicará hacia 
//donde mover el limite para una nueva busqueda.
//Mediante esta multiplicación introducimos el modificador del Orden directamente.
//Conseguimos que devuelva -1 o +1 en función del orden ASC o DESC
//comp = comp * iIndexOrder;--> Invierte el valor de comp.

	m4int32_t ai_iPos = ai_poRegister.Index;    //el registro ya me viene posicionado

	m4uint32_t i,j;
	m4int8_t comp = 0, iIndexOrder;
	m4VariantType vValue;
	m4bool_t bVerify;
    m4uint8_t iCppType;

    // muevo el registro:
    //ai_poRegister.MoveTo (ai_iPos);

    for (i = 0; i < m_iNumItemIndex; i++)
    {
        // me muevo al item:
        ai_poRegister.Item.MoveTo (m_poItemIndex[i].iIndex);
        M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[i].hHandle);

        // comparación del tipo
        iCppType = ai_poRegister.Item.ItemDef.CppType ();

		if (iCppType == M4CL_CPP_TYPE_STRING)
		{
			iCppType = M4CL_CPP_TYPE_STRING_VAR;
		}

		/* Bug 0155434
		Si el argumento es nulo no hay que hacer la comprobación de tipos
		*/
		if( ai_pstArguments[i].Type != M4CL_CPP_TYPE_NULL && ai_pstArguments[i].Type != iCppType )
		{
			DUMP_CHLOG_ERRORF( M4_CH_DM_ITEM_TYPE_MISMATCH, M4ObjNodeItemI((ai_poRegister.Item.GetpAccess())->GetpCMCR(), ai_poRegister.Item.ItemDef.GetHandle()) << "BSearch"<<M4ClCppType(iCppType)<<M4ClCppType(ai_pstArguments[i].Type) ) ;
			//No coinciden tipos
			return M4_ERROR;
		}

		iIndexOrder = m_poItemIndex[i].iIndexOrder;

		ai_poRegister.Item.Value.GetpSliceFooter().Get(vValue);

        comp = m_poItemIndex[i].VarCompare ( vValue, ai_pstArguments[i] );

        m4uint8_t ifunction;
        if (ai_pfunctions) {    // si nos pasan un array con funciones lo utilizamos
            ifunction = ai_pfunctions[i];
            if (ifunction == 0) {   // si la función es 0, cogemos la función por defecto
                ifunction = m_poItemIndex[i].GetFunction ();
            }
        }
        else {
            ifunction = m_poItemIndex[i].GetFunction ();    // cogemos el valor por defecto
        }

        switch (ifunction)
        {						
            case M4CL_INDEX_FUNCTION_EQUAL:
                 if (comp!=0)
                 {
                    return comp * iIndexOrder; 
                 }
                 break;

            case M4CL_INDEX_FUNCTION_SMALLER:
                 //se busca el menor más cercano
                 //Por definicion de esta busqueda estamos en el ultimo argumento
                  switch (comp)
                  {
                       case 0:
                       case +1:
                            return +1 * iIndexOrder;
                       case -1:   
                           //El valor encontrado es menor que el buscado                        
						   //Veremos si es el más cercano
                           bVerify=M4_TRUE;

						   //if ( ai_poRegister[ ai_iPos + iIndexOrder ].Index == EOF )
                           if ( ai_poRegister.MoveTo (ai_iPos + iIndexOrder) == M4_FALSE )
                           {
                                ai_poRegister.MoveTo (ai_iPos);
								return 0;
                           }

                           for ( j = 0; j < m_iNumItemIndex - 1; j++)
                           { 
                               //Se comprueba que todos los argumentos anteriores al que estamos verifica
                               //las condiciones de Busqueda
                                ai_poRegister.Item.MoveTo ( m_poItemIndex[j].iIndex );
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[j].hHandle);
						   		ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[j].VarCompare ( vValue, ai_pstArguments[j] );

								if (comp!=0)
								{
									bVerify=M4_FALSE;
									break;
								}
                            }  
                            if  (bVerify==M4_FALSE)
                            { 
								//Encontrado
                                ai_poRegister.MoveTo (ai_iPos);
                                return 0;
                            }
                            else
                            {  
								 //comparamos con el siguiente registro el campo actual
						   		ai_poRegister.Item.MoveTo ( m_poItemIndex[i].iIndex );
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[i].hHandle);
                                ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[i].VarCompare ( vValue, ai_pstArguments[i] );

								//comp = comp * iIndexOrder; //1->DESC  -1->ASC

								if (comp<0)
								{
									return -1 * iIndexOrder;  
								}
								else
								{ 
                                    ai_poRegister.MoveTo (ai_iPos);
									return 0;
								}
                            }
                    }// end del switch   
                    break;

            case M4CL_INDEX_FUNCTION_SMALLER_OR_EQUAL:
                 //se busca el menor igual más cercano
                 //Por definicion de esta busqueda estamos en el ultimo argumento
				  switch (comp)
                  {
                       case 0:
						   return 0;
                       case +1:
                            return +1 * iIndexOrder;
                       case -1:   
                           //El valor encontrado es menor que el buscado
						   //Veremos si es el más cercano
                           bVerify=M4_TRUE; 

						   //if ( ai_poRegister[ ai_iPos + iIndexOrder ].Index == EOF )
                           if ( ai_poRegister.MoveTo (ai_iPos + iIndexOrder) == M4_FALSE )
                           {
                                ai_poRegister.MoveTo (ai_iPos);
								return 0;
                           }

                           for ( j = 0; j < m_iNumItemIndex - 1; j++)
                           { 
                               //Se comprueba que todos los argumentos anteriores al que estamos verifica
                               //las condiciones de Busqueda
						   		ai_poRegister.Item.MoveTo ( m_poItemIndex[j].iIndex );
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[j].hHandle);
                                ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[j].VarCompare ( vValue, ai_pstArguments[j] );

								if (comp!=0)
								{
									bVerify=M4_FALSE;
									break;
								}
                            }  
                            if  (bVerify==M4_FALSE)
                            { 
								//Encontrado
                                ai_poRegister.MoveTo (ai_iPos);
                                return 0;
                            }
                            else
                            {  
								 //comparamos con el siguiente registro el campo actual
                                ai_poRegister.Item.MoveTo ( m_poItemIndex[i].iIndex );
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[i].hHandle);
						   		ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[i].VarCompare ( vValue, ai_pstArguments[i] );

								//comp = comp * iIndexOrder; //1->DESC  -1->ASC

								if (comp<=0)
	                            {
		                             return -1 * iIndexOrder;  
			                    }
				                else
					            { 
                                     ai_poRegister.MoveTo (ai_iPos);
						             return 0;
							    }
                            }
                    }// end del switch   
                    break;

            case M4CL_INDEX_FUNCTION_BIGGER:
                 //se busca el mayor más cercano
                 //Por definicion de esta busqueda estamos en el ultimo argumento
                  switch (comp)
                  {
                       case 0:
                       case -1:
                            return -1 * iIndexOrder;
                       case +1:   
                           //El valor encontrado es mayor que el buscado
						   //Veremos si es el más cercano
                           bVerify=M4_TRUE; 

						   //if ( ai_poRegister[ ai_iPos - iIndexOrder ].Index == EOF )
                           if ( ai_poRegister.MoveTo (ai_iPos - iIndexOrder) == M4_FALSE )
                           {
                                ai_poRegister.MoveTo (ai_iPos);
								return 0;
                           }

                           for ( j = 0; j < m_iNumItemIndex - 1; j++)
                           { 
                               //Se comprueba que todos los argumentos anteriores al que estamos verifica
                               //las condiciones de Busqueda
                                ai_poRegister.Item.MoveTo (m_poItemIndex[j].iIndex);
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[j].hHandle);
						   		ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[j].VarCompare ( vValue, ai_pstArguments[j] );

								if (comp!=0)
								{
									bVerify=M4_FALSE;
									break;
								}

                            }  
                            if  (bVerify==M4_FALSE)
                            { 
								//Encontrado
                                ai_poRegister.MoveTo (ai_iPos);
                                return 0;
                            }
                            else
                            {  
								 //comparamos con el siguiente registro el campo actual
						   		ai_poRegister.Item.MoveTo (m_poItemIndex[i].iIndex);
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[i].hHandle);
                                ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[i].VarCompare ( vValue, ai_pstArguments[i] );

								//comp = comp * iIndexOrder; //1->DESC  -1->ASC

								if (comp>0)
								{
								     return +1 * iIndexOrder;  
								}
								else
								{ 
                                     ai_poRegister.MoveTo (ai_iPos);
								     return 0;
								}
                            }
                    }// end del switch   
                    break;

			case M4CL_INDEX_FUNCTION_BIGGER_OR_EQUAL:
                    
                 //se busca el mayor más cercano
                 //Por definicion de esta busqueda estamos en el ultimo argumento
                  switch (comp)
                  {
                       case 0:
							return 0;
                       case -1:
                            return -1 * iIndexOrder;
                       case +1:   
                           //El valor encontrado es mayor que el buscado
                           //Veremos si es el más cercano
                           bVerify=M4_TRUE; 

						   //if ( ai_poRegister[ ai_iPos - iIndexOrder ].Index == EOF )
                           if ( ai_poRegister.MoveTo (ai_iPos - iIndexOrder) == M4_FALSE )
                           {
                                ai_poRegister.MoveTo (ai_iPos);
								return 0;
                           }

                           for ( j = 0; j < m_iNumItemIndex - 1; j++)
                           { 
                               //Se comprueba que todos los argumentos anteriores al que estamos verifica
                               //las condiciones de Busqueda
						   		ai_poRegister.Item.MoveTo ( m_poItemIndex[j].iIndex );
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[j].hHandle);
                                ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[j].VarCompare ( vValue, ai_pstArguments[j] );

								if (comp!=0)
								{
									bVerify=M4_FALSE;
									break;
								}
                            }  
                            if  (bVerify==M4_FALSE)
                            { 
								//Encontrado
                                ai_poRegister.MoveTo (ai_iPos);
                                return 0;
                            }
                            else
                            {  
								 //comparamos con el siguiente registro el campo actual
						   		ai_poRegister.Item.MoveTo (m_poItemIndex[i].iIndex);
                                M4_ASSERT (ai_poRegister.Item.ItemDef.GetHandle() == m_poItemIndex[i].hHandle);
                                ai_poRegister.Item.Value.Get(vValue);

								comp = m_poItemIndex[i].VarCompare ( vValue, ai_pstArguments[i] );

								//comp = comp * iIndexOrder; //1->DESC  -1->ASC

								if (comp>=0)
								{
								    return +1 * iIndexOrder;  
								}
								else
								{ 
                                    ai_poRegister.MoveTo (ai_iPos);
									return 0;
								}
                            }
                    }// end del switch   
                    break;
            };   		
		 if (comp != 0)
			 return comp * iIndexOrder;
	}
	return comp * iIndexOrder;

}

m4return_t ClBSItemIndex::GetSize(ClSize &ao_size)
{
	
	ao_size += sizeof(ClBSItemIndex);
	ao_size -= sizeof(hHandle);
		
	return hHandle.GetSize(ao_size);
}

m4return_t ClBSearchIndex::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClBSearchIndex);

	if (m_poItemIndex){
		for (m4uint32_t i=0;i<m_iNumItemIndex;++i){
			if (m_poItemIndex[i].GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		// No es necesario porque ClBSItemIndex no tiene destructor.
		// ao_size+=sizeof(int); // array de clases con destructor. Se reserva espacio para el tamaño.
	}

	return M4_SUCCESS;
}
