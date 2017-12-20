
#include "compst_fac.hpp"

#include "compst_common.hpp"

//#include "compst.hpp"
#include "compst_map.hpp"

#include "node_knl.hpp"
#include "px_block.hpp"
#include "cldefine.hpp"

void ClComponentStore_Factory::CreateComponentStore (ClNode_Data *ai_pnd)
{
	_AClComponentStore *pcs;

    m4bool_t bmono;
    ClNode_Connector *pnc;
	ClNode_Data *pnd = ai_pnd;

    for (;;) {
        if (pnd->GetpExtInfo ()->IsFree ()) {
            bmono = M4_TRUE;
            break;
        }

        //tiene que tener un padre:
        pnc = pnd->GetpExtInfo ()->Parent.GetpNotFree ();
        M4_ASSERT (pnc);
        if (pnc->GetType () != M4CL_NODE_CONNECTOR_TYPE_BB) {
            bmono = M4_FALSE;
            break;
        }

        //continuamos ...
        pnd = pnc->GetAddress ();
        M4_ASSERT (pnd);
    }

    if (bmono) {
        pcs = new ClComponentStore_Mono (ai_pnd);
    }
    else {
        //queda pendiente una optimización, hacer un store especializado en relaciones BB
		//pcs = new ClComponentStore_RB ();
        pcs = new ClComponentStore_RB_map (ai_pnd);
    }

    ai_pnd->SetpBlockStore (pcs);
}

//llamada por el tema del carril:

void ClComponentStore_Factory::Update (ClNode_Data *ai_pnd)
{
	ClBlockReference br;
    ai_pnd->GetpBlockStore ()->Get (0, 0, br);

    if (br) {
        br->DestroySafe (); //le borramos de forma ordenada
	    br.Invalide ();
    }

    delete ai_pnd->GetpBlockStore ();   //sí, no borra los bloques
    ClComponentStore_Factory::CreateComponentStore (ai_pnd);

    //si tiene hijos BB hay que actualizarlos tambien, para estar en coherencia
    //con el algoritmo de creación:
    m4uint16_t i;
    m4uint16_t n = ai_pnd->GetpExtInfo ()->ChildNodeBB.Count ();

    ClNode_Data *pchildnode;

    for (i = 0; i < n; i++) {
        pchildnode = ai_pnd->GetpExtInfo ()->ChildNodeBB[(ClChildIndex)i].GetAddress ();
        M4_ASSERT (pchildnode);

        ClComponentStore_Factory::Update (pchildnode);
    }
}
