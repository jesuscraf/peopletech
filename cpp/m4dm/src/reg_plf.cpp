#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_plf.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter code
//
//
//==============================================================================

#include <errno.h>

#include "reg_plf.hpp"
#include "arecset.hpp"

#include "pl_data.hpp"

#include "arecset.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "m4log.hpp"
#include "px_block.hpp"

#include "accesspr.hpp"
#include "access.hpp"

#include "blockref.hpp"
#include "m4mdrt.hpp"

#include "cldmcontext.hpp"
#include "m4objreg.hpp"
#include "clfileiod.hpp"
#include "ch_block.hpp"




ClRegisterPartialLoad::ClRegisterPartialLoad ()
{
}

ClRegisterPartialLoad::~ClRegisterPartialLoad ()
{
}

m4return_t ClRegisterPartialLoad::Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m4return_t res;

    m4uint32_t CurrentIndex = ai_Index;

    if (aio_MMIndex != ClMMIndex_EOF)
	{
        ai_Index++;
		res = LoadRegisters(ai_Index, aio_MMIndex, ai_br, ai_pap);

		if (res == M4_ERROR)
        {
            // Ya no vuelco mensaje de error aqui.
            aio_MMIndex = ClMMIndex_EOF;
            return M4_ERROR;
        }
	}

    ai_Index = CurrentIndex;

    return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m4uint32_t CurrentIndex = ai_Index;

	if (ai_Index != M4DM_EOF_INDEX)
	{
		if (ai_Index > 0)
		{
			ai_Index--;
			m4return_t res = LoadRegisters(ai_Index, aio_MMIndex, ai_br, ai_pap);
			if (res == M4_ERROR)
			{
				aio_MMIndex = ClMMIndex_EOF;
				return M4_ERROR;
			}
		}
		else
			aio_MMIndex = ClMMIndex_EOF;
	}
	
	ai_Index = CurrentIndex;
	
	return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	/* Bug 0070555
	No hay que posicionarse antes de haber hecho la carga
	*/
	m4return_t res = LoadRegisters(0, aio_MMIndex, ai_br, ai_pap);

	if (res == M4_ERROR)
	{
		ai_Index = M4DM_EOF_INDEX;
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClCompiledMCR * pmcr  = ai_br->GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
    ClHandle NodeHandle = ai_br->GetpNode_Data()->GetpNodeDef()->GetHandle();

    DUMP_CHLOG_WARNINGF(M4_CH_DM_PARTIAL_LOAD_END, M4ObjNodeN(pmcr, NodeHandle));

    return M4_ERROR;
}

m4return_t ClRegisterPartialLoad::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m4return_t res;

	res = LoadRegisters(ai_newindex, aio_MMIndex, ai_br, ai_pap);

	if (res == M4_ERROR)
    {
        aio_MMIndex = ClMMIndex_EOF;
        return M4_ERROR;
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    aio_MMIndex = ClMMIndex_EOF;
    return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::Count (m4uint32_t &ao_count, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (ai_br->IsNihil())
    {
        ao_count = 0;
        return M4_SUCCESS;
    }

    ao_count = ai_br->GetpPL_Data()->GetLastVisible() - 1;

    return M4_SUCCESS;
}



m4return_t ClRegisterPartialLoad::LoadRegisters(ClRegisterIndex ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m4return_t res = M4_SUCCESS;
	
	// Si no existe el bloque es un error
	if (ai_br->IsNihil())
	{
		ClAccess *paccess = ai_pap->GetpAccess();
		ClCompiledMCR * pmcr  = paccess->GetpCMCR();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_PARTIAL_LOAD_NHIHIL, M4Obj(pmcr));
		
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}
	
	ClPartialLoadData	*poPartial = ai_br->GetpPL_Data() ;

	ClRegisterIndex FirstVisible = poPartial->GetFirstVisible();
	ClRegisterIndex LastVisible = poPartial->GetLastVisible();
	m4int32_t MaxReg  = ai_br->GetpNode_Data()->GetpExtInfo()->NumRows();
	m4int32_t PrevReg  = ai_br->GetpNode_Data()->GetpExtInfo()->NumKeepRows();
	

	// Si el índice es negativo es un error
	if ((m4int32_t)ai_Index < 0)
	{
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}

	// Si el índice ya está en la ventana se sale correctamente sin hacer nada
	if ((ai_Index >= FirstVisible)&&(ai_Index < LastVisible))
	{
		aio_MMIndex = ai_Index - FirstVisible;
		return M4_SUCCESS;
	}

	/* Bug 0070557
	Si estamos en el final hay que devolver error
	*/
	if( ai_Index >= LastVisible && poPartial->GetEndOfRegisters() == M4_TRUE )
	{
		aio_MMIndex = ClMMIndex_EOF ;
		return( M4_ERROR ) ;
	}

	// Caso de que lo que hay cargado es menor que el tamaño de la ventana.
	// esto se debe a que hay menos registros en base de datos que NumRows
	m4uint32_t icount = ai_br->Count();
	if ((icount < (m4uint32_t) MaxReg)&&((FirstVisible != 0) || (LastVisible != 0)))
	{
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}
	
	// Si la carga parcial está bloqueada es un error
	if (poPartial->GetLocked())
	{
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}

	if ((FirstVisible == LastVisible)&&(LastVisible == 0)) //primera vez.
	{
		poPartial->SetFirstToRead(0);
		poPartial->SetFirstToWrite(0);
		poPartial->SetNumberToWrite(MaxReg);
		
		poPartial->SetFirstVisible(0);
		poPartial->SetLastVisible(0);
		
		FirstVisible = 0;
		res = LoadWindow(ai_br, ai_pap, 0, 0, MaxReg, 0, 0);
	}
	else if (ai_Index < FirstVisible)
	{
		m4uint32_t Distance = FirstVisible - ai_Index;//normalmente es 1

		if ( Distance > (m4uint32_t)(MaxReg - PrevReg))
		{
			poPartial->SetPrepareIndexPL (0); //0 no hay reordenacion
			//si el nuevo indice esta muy lejos, lo colocamos como el primero visible
			res = LoadWindow(ai_br, ai_pap, ai_Index, 0, MaxReg, ai_Index, ai_Index);
			// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible

			if (res == M4_SUCCESS)
			{
				FirstVisible = ai_Index;
			}
		}
		else
		{
			poPartial->SetPrepareIndexPL (1); //1 es  M4_FALSE

			if ((m4int32_t)FirstVisible > (MaxReg - PrevReg))
			{
				m4uint32_t NewFV = FirstVisible - (MaxReg - PrevReg);
				//colocamos el que verifique los valores de maxreg y prev
				res = LoadWindow(ai_br, ai_pap, NewFV, PrevReg, MaxReg - PrevReg, NewFV, NewFV + PrevReg);
				// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible
			}
			else
			{
				//colocamos el que verifique los valores de maxreg y prev
				res = LoadWindow(ai_br, ai_pap, 0, MaxReg - FirstVisible, FirstVisible,	0, PrevReg);
				// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible
			}
			
			if (res == M4_SUCCESS)
			{
				FirstVisible = poPartial->GetFirstVisible();//actualizo
			}
		}
	}
	else
	{
		if ((!poPartial->GetEnd())||((m4int32_t)ai_Index < poPartial->GetEnd()))
		{
			if (PrevReg >= 0) //(ai_Index > LastVisible) 
			{
				m4uint32_t Distance = ai_Index - LastVisible;//normalmente es 1

				if ( Distance >= (m4uint32_t)(MaxReg - PrevReg))
				{
					poPartial->SetPrepareIndexPL (0); //0 No hay que ordenar
					//si el nuevo indice esta muy lejos, lo colocamos como el primero visible
					res = LoadWindow(ai_br, ai_pap, ai_Index, 0, MaxReg, ai_Index, ai_Index);
					// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible
					
					if (res == M4_SUCCESS)
					{
						FirstVisible = ai_Index;//actualizo
					}
				}
				else
				{
					poPartial->SetPrepareIndexPL (2); //3 es  M4_TRUE
					//y hace falta calular el FirstVisible
					//colocamos el que verifique los valores de maxreg y prev
					m4uint32_t NewFV = FirstVisible + MaxReg;
					res = LoadWindow(ai_br, ai_pap, NewFV, 0, MaxReg - PrevReg, NewFV - PrevReg, NewFV);
					// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible
					
					if (res == M4_SUCCESS)
					{
						FirstVisible = poPartial->GetFirstVisible();
					}
				}
			}
			else
			{
				res = LoadWindow(ai_br, ai_pap, LastVisible, LastVisible, MaxReg, FirstVisible, LastVisible);
				// FirstToRead FirstToWrite NumberToWrite FirstVisible LastVisible
			}
		}
		else
		{
			aio_MMIndex = ClMMIndex_EOF;
			return M4_ERROR;
		}
	}
	// Ahora lasvisible se actualiza en aregister.cpp en ***SetLotOfItemPartial***
	
	if (res != M4_SUCCESS)
	{
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}
	
	LastVisible = poPartial->GetLastVisible();
	
	if ((ai_Index >= FirstVisible)&&(ai_Index < LastVisible)) //volvemos a comprobar 
	{
		aio_MMIndex = ai_Index - FirstVisible;
		return M4_SUCCESS;
	}

	return M4_ERROR;
}



/*
Se ha añadido la funcionalidad de guardar y recuperar registros de fichero.
Esta funcionalidad sólo está activa con el NumKeepRows mayor que 0
Para ello, para cargas que no son la primera se almacenan los registros que
se quedan fuera de scope y después se verifica si se puede cargar la nueva
ventana de fichero. Si es así se carga de disco. De otra manera se hace la carga
normal.
Si se ha hecho la carga normal aún así se intentan recuperar de disco registros
sueltos que correspondan a los que se han leido de base de datos, por si no se
pudo cargar de fichero por que sólo estaban algunos de los registros.
También después de una carga normal hay que ajustar el máximo número de registros
*/

m4double_t ClRegisterPartialLoad::PartialLoad(const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bFirstWindowLoaded = M4_FALSE ;
	m4bool_t			bRestored = M4_FALSE ;
	m4int32_t			iStoredRegisters = -1 ;
	m4int32_t			iReadRegisters = -1 ;
	m4int32_t			iNumKeepRows = -1;
	m4pcchar_t			pccTemp = NULL ;
    ClAccess			*poAccess = NULL ;
	ClNode_Data			*poNodeData = NULL ;
	ClPartialLoadData	*poPartial = NULL ;


	if( !(m4bool_t)ai_br || ai_pap == NULL )
	{
		return( -1 ) ;
	}

    poAccess = ai_pap->GetpAccess() ;
	poNodeData = ai_br->GetpNode_Data() ;
	poPartial = ai_br->GetpPL_Data() ;

	if( poAccess == NULL || poPartial == NULL || poNodeData == NULL )
	{
		return( -1 ) ;
	}


	iStoredRegisters = 0 ;

	iNumKeepRows = poNodeData->GetpExtInfo()->NumKeepRows() ;

/*
Para pruebas
static m4bool_t bPLFile = M4_FALSE ;
if( bPLFile == M4_FALSE ) { iNumKeepRows = 0 ; }
*/

	/* Bug 0098536
	El NumKeepRows puede ser negativo, lo que indica que no se descarten
	registros. En ese caso no hay que guardar nada en disco ni recuperar.
	*/
	if( iNumKeepRows > 0 )
	{
		bFirstWindowLoaded = poPartial->GetFirstWindowLoaded() ;

		if( bFirstWindowLoaded == M4_TRUE )
		{
			// Se guardan los ficheros. Si falla no pasa nada, se sigue
			iResult = _StoreWindow( ai_br, ai_pap ) ;

			iResult = _RestoreWindow( ai_br, ai_pap, bRestored, iStoredRegisters ) ;

			if( iResult != M4_SUCCESS )
			{
				return( -1 ) ;
			}

			if( bRestored == M4_TRUE )
			{
				iReadRegisters = poPartial->GetReadRegister() ;
				return( iReadRegisters ) ;
			}
		}
	}


    //consigo el accessrecordset, sobre el que voy a hacer la load:
    ClNodeIndex ni = poNodeData->GetpExtInfo()->Index;
    ClNode *pn = poAccess->Node.Get(ni);

    if( pn == NULL || pn->RecordSet.GetCurrentBlockRef() != ai_br )
	{
        return( -1 ) ;
	}

    poPartial->SetLocked(M4_TRUE);
    /* Bug 0070557
    Hay que poner el flag de que la carga viene por paginación.
    */
    poPartial->SetLoadFromPageFault(M4_TRUE);

    m4uint8_t iautoloadmode = pn->GetAutoLoadRealMode ();

    if( iautoloadmode == M4CL_AUTOLOAD_PRG )
	{
        iResult = pn->RecordSet.Load_Prg() ;
	}
    else
	{
        iResult = pn->RecordSet.Load_Block() ;
	}

	/* Bug 0285352
	Puede que se haya destruido el bloque
	*/
	poPartial = ai_br->GetpPL_Data() ;

	if( poPartial == NULL )
	{
		return( -1 ) ;
	}

    /* Bug 0070557
    Hay que resetear el flag de que la carga viene por paginación.
    */
    poPartial->SetLoadFromPageFault(M4_FALSE);
    poPartial->SetLocked(M4_FALSE);

	poPartial->SetMaxRegisters() ;
	
    if( iResult != M4_SUCCESS )
    {        
        ClCompiledMCR * pmcr  = poNodeData->GetpChannel_Data()->GetpChannelDef()->GetpCMCR();
        ClHandle NodeHandle = poNodeData->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_DEBUGF(M4_CH_DM_PARTIAL_LOAD_PL, M4ObjNodeN(pmcr, NodeHandle));
    }


	if( iStoredRegisters > 0 )
	{
		iResult = _RestoreRegisters( ai_br, ai_pap ) ;

		if( iResult != M4_SUCCESS )
		{
			return( -1 ) ;
		}
	}


	/* Bug 0076455
	Hay que devolver siempre lo que se ha cargado, porque no sabemos si ha
	fallado la carga del bloque o de los hijos, u otra cosa.
	*/
	iReadRegisters = poPartial->GetReadRegister() ;

    return( iReadRegisters ) ;
}


m4bool_t ClRegisterPartialLoad::IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //devuelve siempre cierto (¿no?)
    return M4_TRUE;
}

m4return_t ClRegisterPartialLoad::UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4int32_t FirstVisible = ai_br->GetpPL_Data()->GetFirstVisible();
    aio_Index = ai_MMIndex + FirstVisible; //por ahora

    return M4_SUCCESS;
}

m4return_t ClRegisterPartialLoad::PrepareIndexPL(m4bool_t ai_bUp, const ClBlockReference &ai_br)
{
	m4uint32_t iRR = ai_br->GetpPL_Data()->GetReadRegister();
    return ai_br->R_PrepareIndexPL(ai_bUp, iRR);
}

m4return_t ClRegisterPartialLoad::LoadWindow(const ClBlockReference &ai_br, 
                                             ClAccessProvider *ai_pap,
                                             m4uint32_t ai_iFR, m4uint32_t ai_iFW,
                                             m4uint32_t ai_iNW, m4uint32_t ai_iFV,
                                             m4uint32_t ai_iLV)
{

	ClPartialLoadData	*poPartial = ai_br->GetpPL_Data() ;

    m4uint32_t oldfirst = poPartial->GetFirstVisible();
    m4uint32_t oldlast  = poPartial->GetLastVisible();
    m4bool_t   oldend  = poPartial->GetEndOfRegisters();

    poPartial->SetFirstToRead(ai_iFR);
    poPartial->SetFirstToWrite(ai_iFW);
    poPartial->SetNumberToWrite(ai_iNW);

    poPartial->SetNextFV(ai_iFV);
    poPartial->SetNextLV(ai_iLV);

	//Bug:0063261     Begin  ----------------------------------------
	//Desactivamos la carga de una ventana nueva en los canales que se están depurando.
	// Se producía un GP al intentar cargarse una ventana en un canal Back en la parte cliente. Esto sucede
	// al hacerse un MoveTo sobre un registro fuera de la ventana actual cuando se está deserializando el Access de un canal Back en la parte cliente.
	if(ai_pap->GetpAccess()) {
		ClChannel * pChannel = ai_pap->GetpAccess()->GetpChannel();
		if( (pChannel->GetpChannelManager()->GetDMContext()->ExecutingInClient()) &&  ((pChannel->GetDebugMode() == ClChannel::IS_DEBUG_VIEW) || (pChannel->GetDebugMode() == ClChannel::IS_DEBUG)) )
			return M4_SUCCESS;
	}
	//Bug:0063261     End    ----------------------------------------

    m4double_t LoadedRegister = PartialLoad(ai_br, ai_pap);

    if (LoadedRegister <= 0)
    {
        poPartial->SetFirstVisible(oldfirst);
        poPartial->SetLastVisible(oldlast);

		/* Bug 0084867
		Si se cargan 0 registros no hay que restaurar el estado de final
		*/
		if( LoadedRegister < 0 )
		{
			poPartial->SetEndOfRegisters(oldend);
		}

        return M4_ERROR;
    }

    return M4_SUCCESS;
}


// Revisar los comentarios con la reescritura de fichero

/*
Esta función guarda los registros que se salen de scope almacenándolos en disco.
Los registros se almacenan en un único fichero.
El algoritmo de guardarlo es el que proporciona el ClRamdomFileIODriver, de modo
que se reaprovecha el espacio si ese mismo registro ya se había almacenado y se
crean más bloques de datos si no cabe en el tamaño del anterior.
Además se mantiene el mapa de correspondencia entre registros y su posición en el
fichero. De esta forma, si la referencia no existía se añade al mapa y si existía
se mantine, quepa o no quepa, porque el comienzo de los datos si que se mantiene.

Pasos:

	Se comprueban los argumentos de entrada
	Se obtienen los objetos necesarios a partir de los argumentos y se comprueban
	Se calculan los registros a escribir, que son los que se van a salir de scope.
		El primero es el primer registro que se va a escribir (índice físico) a partir
		del comienzo de la ventana actual (índice lógico), de modo que es
		FirstToWrite + FirstVisible
		El último es ese primero calculado más el número de ellos que se van a leer,
		de modo que es First + NumberToWrite
	Se inicializa el driver en modo escritura asociado al fichero de carga parcial
	Se itera por los registros
	Para cada iteración se mira si el registro ya existe o no y si existe se obtiene
		su posición. Se hace que el driver comience a escribir en esa posición si
		existía el registro o en un bloque de datos nuevo si no existía. Se serializa
		el registro a disco. Al acabar de tratar cada registro se finaliza la escritura
		de ese registro en el driver para obtener el comienzo del bloque de datos.
		Si el registro no existía se añade la referencia con el comienzo del bloque
		de datos
	Al acabar la iteración se cierra el driver y su fichero
*/
m4return_t ClRegisterPartialLoad::_StoreWindow( const ClBlockReference &ai_br, ClAccessProvider *ai_pap )
{

	m4return_t				iResult = M4_ERROR ;
	m4bool_t				bExist = M4_FALSE ;
	m4uint16_t				iChildCount = 0 ;
	m4int32_t				i = -1 ;
	m4int32_t				iFirstVisible = -1 ;
	m4int32_t				iFirstRegister = -1 ;
	m4int32_t				iLastRegister = -1 ;
	long					lPosition = -1 ;
	ClPartialLoadData		*poPartial = NULL ;
	ClNode_Data				*poNodeData = NULL ;
	ClChannel_Data			*poChannelData = NULL ;
	ClNodeDef				*poNodeDef = NULL ;
	ClRamdomFileIODriver	IOD ;

	M4PLFileMap_t::iterator	iterator ;


	CHECK_CHLOG_ERRORF( !(m4bool_t)ai_br, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_br" << "ClRegisterPartialLoad::_StoreWindow" << __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_pap == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_pap" << "ClRegisterPartialLoad::_StoreWindow" << __LINE__ << __FILE__ ) ;


	poPartial = ai_br->GetpPL_Data() ;
	poNodeData = ai_br->GetpNode_Data() ;
	poChannelData = poNodeData->GetpChannel_Data() ;
	poNodeDef = poNodeData->GetpNodeDef() ;

	CHECK_CHLOG_ERRORF( poPartial == NULL || poNodeData == NULL || poChannelData == NULL || poNodeDef == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	M4PLFileMap_t	&roFileMap = poPartial->GetFileMap() ;


	iFirstVisible = poPartial->GetFirstVisible() ;
	iFirstRegister = iFirstVisible + poPartial->GetFirstToWrite() ;
	iLastRegister = iFirstRegister + poPartial->GetNumberToWrite() ;

	iChildCount = poNodeData->GetpExtInfo()->ChildNodeRB.Count() ;

	iResult = _OpenFile( poChannelData, IOD, GEN_IO_MODE_REWRITE, poNodeDef ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = iFirstRegister ; i < iLastRegister ; i++ )
	{
		iterator = roFileMap.find( i ) ;

		if( iterator != roFileMap.end() )
		{
			bExist = M4_TRUE ;
			lPosition = (*iterator).second ;
		}
		else
		{
			bExist = M4_FALSE ;
			lPosition = -1 ;
		}

		iResult = IOD.StartWritting( lPosition ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_WRITE_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		iResult = ai_br->_SerializeRecord( IOD, M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_ALL, NULL, NULL, i - iFirstVisible, M4_TRUE, iChildCount, 0, M4_TRUE ) ; 

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_WRITE_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		iResult = IOD.EndWritting( lPosition ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_WRITE_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		if( bExist == M4_FALSE )
		{
			roFileMap[ i ] = lPosition ;
		}

	}

	IOD.End( M4_TRUE ) ;


    return( M4_SUCCESS ) ;
}


/*
Esta función recupera los registros de un bloque almacenados en el fichero en disco.
Antes que nada comprueba si los registros que se necesitan existen ya en el fichero.
Si no existen se sale de la función marcando que no se han restaurado los registros.
Tiene la función similar a la carga de base de datos, pero de fichero. Al acabar
de recuperar los ficheros tiene que actualizar todos los parámetros de la carga
parcial que se actualizaban en la carga de base de datos.

Pasos:

	Se actualizan las variables de salida
	Se comprueban los argumentos de entrada
	Se obtienen los objetos necesarios a partir de los argumentos y se comprueban
	Se calculan los registros a leer.
		El primero es el primer registro (lógico) a leer, es decir FirstToRead.
		El último es ese primero calculado más el número de ellos que se van a leer,
		de modo que es First + NumberToWrite
		A estos valores hay que hacerles el ajuste de final de registros. Si ya se
		ha llegado a todos los registros que hay en base de datos, puede ser que el último
		registro a leer tenga que ser menor. Entonces, si ya se sabe cual es el número
		total de registros en base de datos y si el valor calculado para el último
		registro es mayor que el real, se pone el calculado al valor del real
	Se guarda el número que originalmente se quería leer para hacer el ajuste de fin de
		registros
	Se itera por los registros
	Para cada iteración se comprueba si el registro existe en el fichero y se va contando
		el número de registros que si están en fichero en la variable de salida.
		Si no existe alguno se sale de la función
	Se activa el modo UseCPDelta, necesario para que se deserialicen correctamente
		los oldvalues
	Se inicializa el driver asociado al fichero de carga parcial en modo lectura
	Se itera por los registros
	Para cada iteración se obtiene la posición del registro dentro del fichero en disco
		y se hace que el driver comience a leer en esa posición. Después se destruyen
		los hijos del registro que se va a restaurar y se deserializa el registro y
		sus hijos a RAM. Para ello, se utiliza el índice físico dado por FirstToWrite
		(que se va avanzando) según se itera. También se incrementa el índice físico
		dado por FirstToWrite y el valor de la carga parcial NextLV, que se asigna. Al
		acabar de tratar cada registro se finaliza la lectura de ese registro en el driver
	Al acabar la iteración se cierra el driver y su fichero
	Se ajusta el final de registros. Si se han cargado menos de los pedidos estamos al final
		y si no no
	Se asigna el FirstVisible al NextFV y el LastVisible al NextLV que se ha ido incrementando
		a cada iteración
	Se realiza la ordenación física de los registros según el PrepareIndexPL. Este código
		está copiado de vm y hay que tener cuidado si se añade algo en un sitio hacerlo en
		el otro
	Se actualiza la variable de salida
*/
m4return_t	ClRegisterPartialLoad::_RestoreWindow( const ClBlockReference &ai_br, ClAccessProvider *ai_pap, m4bool_t& ao_rbRestored, m4int32_t& ao_iStoredRegisters )
{

	m4return_t				iResult = M4_ERROR ;
	m4uint16_t				iChildCount = 0 ;
	m4uint32_t				iNumRows = 0 ;
	m4int32_t				i = -1 ;
	m4int32_t				iPrepareIndex = -1 ;
	m4int32_t				iNextFirstVisible = -1 ;
	m4int32_t				iNextLastVisible = -1 ;
	m4int32_t				iFirstToWrite = -1 ;
	m4int32_t				iFirstRegister = -1 ;
	m4int32_t				iLastRegisterToRead = -1 ;
	m4int32_t				iLastRegister = -1 ;
	m4int32_t				iMaxRegisters = 0 ;
	long					lPosition = -1 ;
    ClAccess				*poAccess = NULL ;
	ClPartialLoadData		*poPartial = NULL ;
	ClNode_Data				*poNodeData = NULL ;
	ClChannel_Data			*poChannelData = NULL ;
	ClNodeDef				*poNodeDef = NULL ;
	ClNotifier_Channel_Data	*poNotifier = NULL ;
	ClNode					*poNode = NULL ;
	ClRamdomFileIODriver	IOD ;

	M4PLFileMap_t::iterator	iterator ;


	ao_rbRestored = M4_FALSE ;
	ao_iStoredRegisters = 0 ;

			
	CHECK_CHLOG_ERRORF( !(m4bool_t)ai_br, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_br" << "ClRegisterPartialLoad::_RestoreWindow" << __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_pap == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_pap" << "ClRegisterPartialLoad::_RestoreWindow" << __LINE__ << __FILE__ ) ;


    poAccess = ai_pap->GetpAccess() ;
	poPartial = ai_br->GetpPL_Data() ;
	poNodeData = ai_br->GetpNode_Data() ;
	poChannelData = poNodeData->GetpChannel_Data() ;
	poNodeDef = poNodeData->GetpNodeDef() ;

	CHECK_CHLOG_ERRORF( poAccess == NULL || poPartial == NULL || poNodeData == NULL || poChannelData == NULL || poNodeDef == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	M4PLFileMap_t	&roFileMap = poPartial->GetFileMap() ;


	iFirstRegister = poPartial->GetFirstToRead() ;
	iLastRegister = iFirstRegister + poPartial->GetNumberToWrite() ;
	iMaxRegisters = poPartial->GetMaxRegisters() ;

	iLastRegisterToRead = iLastRegister ;

	if( iMaxRegisters > -1 && iLastRegister > iMaxRegisters )
	{
		iLastRegister = iMaxRegisters ;
	}

	for( i = iFirstRegister ; i < iLastRegister ; i++ )
	{
		if( roFileMap.find( i ) != roFileMap.end() )
		{
			ao_iStoredRegisters++ ;
		}
	}

	if( ao_iStoredRegisters != iLastRegister - iFirstRegister )
	{
		return( M4_SUCCESS ) ;
	}


	iChildCount = poNodeData->GetpExtInfo()->ChildNodeRB.Count() ;
	poNotifier = poChannelData->GetpNotifier() ;
	ai_br->m_pClBlock->Records.SetUseCPDelta( M4_TRUE ) ;

	iNextFirstVisible = poPartial->GetNextFV() ;
	iNextLastVisible = poPartial->GetNextLV() ;
	iFirstToWrite = poPartial->GetFirstToWrite() ;

	iResult = _OpenFile( poChannelData, IOD, GEN_IO_MODE_READ, poNodeDef ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = iFirstRegister ; i < iLastRegister ; i++ )
	{
		iterator = roFileMap.find( i ) ;

		if( iterator == roFileMap.end() )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			return( M4_ERROR ) ;
		}

		lPosition = (*iterator).second ;

		iResult = IOD.StartReading( lPosition ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		ai_br->R_DestroyRBChilds( iFirstToWrite ) ;

		iResult = ai_br->_DeSerializeRecord( IOD, M4CL_RECORD_SERIAL_ALL, iFirstToWrite, M4_TRUE, iChildCount, M4_FALSE, poNotifier, NULL, 0 ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		/* Bug 0102510
		Hay que decir que el registro que cargamos de fichero genera deltas
		*/
		ai_br->m_pClBlock->Records.OnModifiedByPos( iFirstToWrite ) ;

		iResult = IOD.EndReading() ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		iFirstToWrite++ ;
		iNextLastVisible++ ;

		poPartial->SetNextLV( iNextLastVisible ) ;
	}

	IOD.End( M4_TRUE ) ;


    poPartial->SetReadRegister( iLastRegister - iFirstRegister ) ;

	if( iLastRegister < iLastRegisterToRead )
	{
		poPartial->SetEndOfRegisters( M4_TRUE ) ;
	}
	else
	{
		poPartial->SetEndOfRegisters( M4_FALSE ) ;
	}

    poPartial->SetFirstVisible( iNextFirstVisible ) ;
    poPartial->SetLastVisible( iNextLastVisible ) ;

    iPrepareIndex = poPartial->GetPrepareIndexPL() ;

	if( iPrepareIndex != 0 )
	{
		iNumRows = poNodeData->GetpNodeDef()->NumRows() ;
		poNode = poAccess->Node.Get( poNodeData->GetpNodeDef()->Index ) ;

		if( iPrepareIndex == 1 )
		{
			poPartial->SetLastVisible( iNextFirstVisible + iNumRows ) ;
			iResult = poNode->RecordSet.Register.Filter->PrepareIndexPL( M4_FALSE, ai_br ) ;
		}
		else if( iPrepareIndex == 2 )
		{
			poPartial->SetFirstVisible( iNextLastVisible - iNumRows ) ;
			iResult = poNode->RecordSet.Register.Filter->PrepareIndexPL( M4_TRUE, ai_br ) ;
		}

		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_NO_ORDER_PL_INDEX, M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
	}


	ao_rbRestored = M4_TRUE ;

	return( M4_SUCCESS ) ;
}





/*
Esta función recupera registros individuales almacenados en el fichero en disco.
Sirve para que una vez que se haya realizado una carga de base de datos (porque
no se pudo recuperar la ventana del fichero en disco) se puedan recuperar registros
individuales que si existen en disco (aunque no esté toda la ventana)

Pasos:

	Se comprueban los argumentos de entrada
	Se obtienen los objetos necesarios a partir de los argumentos y se comprueban
	Se calculan los registros a leer.
		El primero es el primer registro (lógico) a leer, es decir FirstToRead.
		El último es ese primero calculado más el número de ellos que se han leido,
		de modo que es First + ReadRegister
	Se itera por los registros
	Para cada iteración se obtiene la posición del registro dentro del fichero en disco.
		Si ese registro no está almacenado se pasa al siguiente.
		Si hay algún registro a tratar, en el primero de ellos el driver asociado
		al fichero de carga parcial en modo	lectura (para optimizar). También se
		activa el modo UseCPDelta, necesario para que se deserialicen correctamente
		los oldvalues
		Se obtiene la posición del registro dentro del fichero en disco y se hace
		que el driver comience a leer en esa posición. Después se destruyen los
		hijos del registro que se va a restaurar y se deserializa el registro y
		sus hijos a RAM. Para ello,	se utiliza el índice físico que es el lógico
		(el iterador) menos	el comienzo de la ventana, es decir, i - FirstVisible.
		Al acabar de tratar cada registro se finaliza la lectura de ese registro
		en el driver
	Al acabar la iteración se cierra el driver y su fichero si se ha utilizado
*/
m4return_t	ClRegisterPartialLoad::_RestoreRegisters( const ClBlockReference &ai_br, ClAccessProvider *ai_pap )
{

	m4return_t				iResult = M4_ERROR ;
	m4bool_t				bFirst = M4_TRUE ;
	m4uint16_t				iChildCount = 0 ;
	m4int32_t				i = -1 ;
	m4int32_t				iFirstVisible = -1 ;
	m4int32_t				iFirstRegister = -1 ;
	m4int32_t				iLastRegister = -1 ;
	long					lPosition = -1 ;
	ClPartialLoadData		*poPartial = NULL ;
	ClNode_Data				*poNodeData = NULL ;
	ClChannel_Data			*poChannelData = NULL ;
	ClNodeDef				*poNodeDef = NULL ;
	ClNotifier_Channel_Data	*poNotifier = NULL ;
	ClRamdomFileIODriver	IOD ;

	M4PLFileMap_t::iterator	iterator ;


	CHECK_CHLOG_ERRORF( !(m4bool_t)ai_br, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_br" << "ClRegisterPartialLoad::_RestoreRegisters" << __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_pap == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_pap" << "ClRegisterPartialLoad::_RestoreRegisters" << __LINE__ << __FILE__ ) ;


	poPartial = ai_br->GetpPL_Data() ;
	poNodeData = ai_br->GetpNode_Data() ;
	poChannelData = poNodeData->GetpChannel_Data() ;
	poNodeDef = poNodeData->GetpNodeDef() ;

	CHECK_CHLOG_ERRORF( poPartial == NULL || poNodeData == NULL || poChannelData == NULL || poNodeDef == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	M4PLFileMap_t	&roFileMap = poPartial->GetFileMap() ;


	iFirstVisible = poPartial->GetFirstVisible() ;
	iFirstRegister = poPartial->GetFirstToRead() ;
	iLastRegister = iFirstRegister + poPartial->GetReadRegister() ;


	bFirst = M4_TRUE ;

	for( i = iFirstRegister ; i < iLastRegister ; i++ )
	{
		iterator = roFileMap.find( i ) ;

		if( iterator == roFileMap.end() )
		{
			continue ;
		}

		if( bFirst == M4_TRUE )
		{
			iResult = _OpenFile( poChannelData, IOD, GEN_IO_MODE_READ, poNodeDef ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iChildCount = poNodeData->GetpExtInfo()->ChildNodeRB.Count() ;
			poNotifier = poChannelData->GetpNotifier() ;
			ai_br->m_pClBlock->Records.SetUseCPDelta( M4_TRUE ) ;

			bFirst = M4_FALSE ;
		}

		lPosition = (*iterator).second ;

		iResult = IOD.StartReading( lPosition ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		ai_br->R_DestroyRBChilds( i - iFirstVisible ) ;

		iResult = ai_br->_DeSerializeRecord( IOD, M4CL_RECORD_SERIAL_ALL, i - iFirstVisible, M4_TRUE, iChildCount, M4_FALSE, poNotifier, NULL, 0 ) ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}

		/* Bug 0102510
		Hay que decir que el registro que cargamos de fichero genera deltas
		*/
		ai_br->m_pClBlock->Records.OnModifiedByPos( i - iFirstVisible ) ;

		iResult = IOD.EndReading() ;

		if( iResult != M4_SUCCESS )
		{
			IOD.End( M4_TRUE ) ;
			DUMP_CHLOG_ERRORF( M4_CH_DM_NO_READ_PL_RECORD, i << poChannelData->GetPLFile() << M4ObjNodeN( poNodeDef->GetpCMCR(), poNodeDef->GetHandle() ) ) ;
			return( M4_ERROR ) ;
		}
	}

	if( bFirst == M4_FALSE )
	{
		IOD.End( M4_TRUE ) ;
	}


	return( M4_SUCCESS ) ;
}



/*
Esta función inicializa el driver de fichero en el modo especificado
Es una función auxiliar.

Pasos:

	Se comprueban los argumentos de entrada
	Si ya existe un fichero no se hace nada
	Se crea el fichero
	Se inicializa el driver
*/
m4return_t	ClRegisterPartialLoad::_OpenFile( ClChannel_Data *ai_poChannelData, ClFileIODriver& ai_roIOD, m4uint32_t ai_iMode, const ClNodeDef *ai_pcoNodeDef )
{

	m4return_t	iResult = M4_ERROR ;
	m4pcchar_t	pccFile = NULL ;


	CHECK_CHLOG_ERRORF( ai_poChannelData == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelData" << "ClRegisterPartialLoad::_OpenFile" << __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_pcoNodeDef == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_pcoNodeDef" << "ClRegisterPartialLoad::_OpenFile" << __LINE__ << __FILE__ ) ;


	pccFile = ai_poChannelData->GetPLFile() ;

	if( pccFile == NULL || *pccFile == '\0' )
	{
		iResult = ai_poChannelData->CreatePLFile( ai_pcoNodeDef, pccFile ) ;

		if( iResult != M4_SUCCESS || pccFile == NULL || *pccFile == '\0' )
		{
			return( M4_ERROR ) ;
		}
	}

	/* Bug 0263238
	Se debe retornar error y no nulo si no se puede abrir el fichero
	*/
	iResult = ai_roIOD.Init( ai_iMode, (m4pchar_t) pccFile ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_DM_NO_OPEN_PL_FILE, pccFile << ai_iMode << strerror( errno ) << M4ObjNodeN( ai_pcoNodeDef->GetpCMCR(), ai_pcoNodeDef->GetHandle() ) ) ;

	return( M4_SUCCESS ) ;
}


