//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_load.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             27/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Autoload 'filter' declaration
//
//
//==============================================================================

#include "reg_load.hpp"

#include "px_block.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"

#include "node.hpp"
#include "recorset.hpp"
#include "bookmark.hpp"

#include "accesspr.hpp"
#include "access.hpp"

#include "blockref.hpp"
#include "m4mdrt.hpp"

#include "cldmcontext.hpp"

ClRegisterFilter_AutoLoad::ClRegisterFilter_AutoLoad ()
{
}

ClRegisterFilter_AutoLoad::~ClRegisterFilter_AutoLoad ()
{
}

m4return_t ClRegisterFilter_AutoLoad::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (!ai_br->IsNihil())
    {
        preprocess (ai_br, ai_pap);
        //aunque devuelva un error seguimos adelante, puede haber un método
        //conectado a la load que inserta registros
        ClRegisterFilter_Simple::Begin (ai_Index, aio_MMIndex, ai_br, ai_pap);
    }

    else {
        aio_MMIndex = ClMMIndex_EOF;    //me acongojo y me voy a eof
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_AutoLoad::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (!ai_br->IsNihil())
    {
        preprocess (ai_br, ai_pap);
        //aunque devuelva un error seguimos adelante, puede haber un método
        //conectado a la load que inserta registros
        ClRegisterFilter_Simple::End (ai_Index, aio_MMIndex, ai_br, ai_pap);

    }
    else {
        aio_MMIndex = ClMMIndex_EOF;    //me acongojo y me voy a eof
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_AutoLoad::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (!ai_br->IsNihil())
    {
        preprocess (ai_br, ai_pap);
        //aunque devuelva un error seguimos adelante, puede haber un método
        //conectado a la load que inserta registros
        ClRegisterFilter_Simple::MoveTo (ai_newindex, ai_Index, aio_MMIndex, ai_br, ai_pap);
    }

    else {
        aio_MMIndex = ClMMIndex_EOF;    //me acongojo y me voy a eof
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_AutoLoad::Count (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (!ai_br->IsNihil())
    {
        preprocess (ai_br, ai_pap);
        //aunque devuelva un error seguimos adelante, puede haber un método
        //conectado a la load que inserta registros
        ClRegisterFilter_Simple::Count (ao_icount, ai_br, ai_pap);
    }
    else {
        ao_icount = 0;    //me acongojo y devuelvo 0 registros
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_AutoLoad::preprocess (const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	M4_ASSERT (ai_br);
	
	m4return_t res = M4_SUCCESS;

	if (ai_br->IsLoaded()) return res;
	
	ClAccess* const pOriginalAccess = ai_pap->GetpAccess();
	M4_ASSERT (pOriginalAccess);
	ClChannel* const pChannel = pOriginalAccess->GetpChannel();

	//Bug: 61143   Begin ----------
	// Al dar al enter en el inmediate window, el sesion manager lanzaba un MoveTo(0) en un nodo con autoload = On
	//  de un canal debug en la parte cliente. Esto lo hacía el Sesion Manager cuando el Stepper intentaba moverse a
	//  un item EXECUTEJIT
	if ( !( (pChannel->GetpChannelManager()->GetDMContext()->ExecutingInClient()) &&  ( (pChannel->GetDebugMode() == ClChannel::IS_DEBUG_VIEW) || (pChannel->GetDebugMode() == ClChannel::IS_DEBUG) ) ) ) {
	//Bug: 61143   End   ----------
		// Antes que nada marco el bloque como cargado, 
		// ya que si falla la load me da igual, se ha intentado.
		ai_br->_SetIsLoaded (M4_TRUE);
		
		/* Corrige el bug 51424.
		 * Antes se creaba un bookMark para después de ejecutar volver a colocar el acceso.
		 * En algunos casos esto puede dar problemas (por ejemplo si al hacer la load del hijo se
		 * reordenan los registros del padre).
		 * Ahora para ejecutar se crea un nuevo acceso, de forma que el original no se mueve.
		 * 
		 * ClAccessBookMark oBookMark;
		 * res = oBookMark.GetFrom (*paccess);
		 * if (res != M4_SUCCESS)
		 * 	return res;
		 */
		ClAccess* paccess = 0;
		pChannel->CreateAccess(paccess);
		if (!paccess) return M4_ERROR;


		/* Bug 0087766
		En el caso de que el canal sea proxy se marca para que se borre el acceso
		al acabar la ejecución en la parte server, porque la vamos a destruir en cliente.
		Sólo se hace si estamos en la parte front.
		*/
		if( pChannel->GetpChannelManager()->GetDMContext()->ExecutingInClient() == M4_TRUE )
		{
			if( pChannel->GetpChannelDef()->CsExeType() == M4CL_CSEXETYPE_PROXY )
			{
				m4VariantType	vDelete( M4_TRUE ) ;

				res = paccess->Property.SetOrNewProperty( "DELETE_ON_EXIT", vDelete, M4_PROP_LN4_STATE ) ;

				// Se ignora el retorno. Qué le vamos a hacer.
			}
		}


		paccess->Follow(*pOriginalAccess);
		
		M4_ASSERT (! ai_br->IsNihil ());
		
		ClNode *paccessnode = paccess ->Node.Get (ai_br->GetpNode_Data ()->GetpExtInfo ()->Index);
		M4_ASSERT (paccessnode);
		
		res = paccessnode->RecordSet.Follow (ai_br);
		if ( res != M4_SUCCESS ) return res;
		
		//averiguo el modo de autoload:
		//le pregunto al nodo directamente:
		m4uint8_t iautoloadmode = paccessnode->GetAutoLoadRealMode ();
		M4_ASSERT (iautoloadmode != M4CL_AUTOLOAD_OFF); //pq sino, no debería tener este terminador
		
		//disparamos la autoload:
		
		//comprobación MUY importante:
		M4_ASSERT (paccessnode->RecordSet.GetCurrentBlockRef () == ai_br);	//operator == overloaded

		switch (iautoloadmode) {
		case M4CL_AUTOLOAD_BLOCK:
			//autoload normal:
			res = paccessnode->RecordSet.Load_Block ();
			break;
			
		case M4CL_AUTOLOAD_PRG:
			//autoload 'repelente':
			res = paccessnode->RecordSet.Load_Prg ();
			break;
			
		default:
			//no soportado:
			res = M4_ERROR;
			M4_ASSERT (0);
		}
		
		if (M4_ERROR == res) {
			ClCompiledMCR * pcmcr = paccess->GetpCMCR ();
			ClHandle NodeHandle = ai_br->GetpNode_Data ()->GetpNodeDef()->GetHandle();
			DUMP_CHLOG_DEBUGF(M4_CH_DM_AUTOLOAD_ERROR, iautoloadmode << M4ObjNodeN(pcmcr, NodeHandle));
		}
		
		pOriginalAccess->CopyProperties(*paccess);
		m4return_t res2 = paccess->Destroy();	// Corrige el bug 51424. oBookMark.ApplyTo (*paccess);
		if (res2 != M4_SUCCESS) return res2;
	}

	return res;
}
