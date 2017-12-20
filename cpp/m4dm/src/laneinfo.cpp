#include "laneinfo.hpp"
#include "conitinf.hpp"
#include "itemconn.hpp"

#define DM_INITIAL_SIZE 10

ClLane_Item_Info::ClLane_Item_Info (m4uint32_t ai_icount)
{
    m_pConnectItemInfo = new ClConnectItemInfo [ai_icount];
    m_pHandles = new ClHandle[ai_icount];

	m_iCount = ai_icount;
}

ClLane_Item_Info::~ClLane_Item_Info (void)
{
    delete [] m_pConnectItemInfo;
    delete [] m_pHandles;
}

ClLane_Info::ClLane_Info (void)
{
    m_poLanItInf = new ClLane_Item_Info * [DM_INITIAL_SIZE]; //por defecto creo sitio para DM_INITIAL_SIZE conexiones

    m4uint32_t i;

    for (i = 0; i < DM_INITIAL_SIZE; i ++)
        m_poLanItInf[i] = 0;

    m_iDocs = 0;
    m_iArrayDim = 10;
}

ClLane_Info::~ClLane_Info (void)
{
    m4uint32_t i;

    for (i = 0; i < m_iDocs; i++)
    {
        delete m_poLanItInf[i];
    }

    if (m_poLanItInf)
        delete [] m_poLanItInf;

    m_iDocs = 0;
}

m4uint32_t ClLane_Info::AddNewDoc(m4uint32_t ai_icount)
{
    if (m_iDocs == m_iArrayDim)
        IncreaseArrayDim();

    m_poLanItInf[m_iDocs] = new ClLane_Item_Info (ai_icount);
    m_iDocs ++;
    return m_iDocs - 1;
}

m4return_t ClLane_Info::DeleteDoc(ClNode * ai_pNodeDoc)
{
    m4uint32_t i, j, k;
    m4uint16_t l;

    for (i = 0; i < ChildNodeRB.Count(); i ++)
    {
        if (ChildNodeRB[(ClChildIndex)i].GetAddress() == ai_pNodeDoc)
        {
            delete m_poLanItInf[i];

            memmove (&m_poLanItInf[i], &m_poLanItInf[i + 1], ((m_iDocs - i - 1) * sizeof (ClLane_Item_Info *)));
            m_iDocs --;

            m4uint32_t nic;


// Las posiciones de los nodos hijos cambian!!!!
            for (j = i; j < m_iDocs; j ++)
            {
                for (k = 0; k < m_poLanItInf[j]->m_iCount; k ++)
                {
                    nic = m_poLanItInf[j]->m_pConnectItemInfo[k].Count ();
                    for (l = 0; l < nic; l ++)
                    {
                        m4uint16_t pos    = m_poLanItInf[j]->m_pConnectItemInfo[k][l]->GetChildNodePos ();
                        ClHandle han      = m_poLanItInf[j]->m_pConnectItemInfo[k][l]->GetHandle ();
                        ClNode_Data * pnd = m_poLanItInf[j]->m_pConnectItemInfo[k][l]->GetpNode_Data ();
                        m_poLanItInf[j]->m_pConnectItemInfo[k][l]->Init (pnd, han, --pos);
                    }
                }
            }

            ClChildNodes<ClNode> auxChildNodeRB;
            for (j = 0; j < ChildNodeRB.Count(); j ++)
            {
                if (j != i)
                {
                    auxChildNodeRB.Add (ChildNodeRB[(ClChildIndex)j].GetAddress(),
                        ChildNodeRB[(ClChildIndex)j].GetConnectorHandle());
                }
            }

            ChildNodeRB.Destroy();

            for (j = 0; j < auxChildNodeRB.Count(); j ++)
            {
                ChildNodeRB.Add (auxChildNodeRB[(ClChildIndex)j].GetAddress(),
                    auxChildNodeRB[(ClChildIndex)j].GetConnectorHandle());
            }

            return M4_SUCCESS;
        }
    }

    return M4_WARNING; //no hay conexion
}

ClLane_Item_Info * ClLane_Info::GetpLane_Item_Info_by_Doc(m4uint32_t ai_doc)
{
    if ( ai_doc < m_iDocs)
        return m_poLanItInf[ai_doc];
    else
        return 0;
}

void ClLane_Info::IncreaseArrayDim()
{
    m4uint32_t i;

    ClLane_Item_Info ** auxArray = new ClLane_Item_Info * [2 * m_iArrayDim];

    memcpy (auxArray, m_poLanItInf, m_iArrayDim*sizeof(ClLane_Item_Info *));
    for (i = m_iArrayDim; i < 2 * m_iArrayDim; i ++)
        auxArray[i] = 0;

    delete m_poLanItInf;
    m_poLanItInf = auxArray;
    m_iArrayDim = 2 * m_iArrayDim;
}

m4return_t
ClLane_Info::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLane_Info);

	ao_size-=sizeof(ChildNodeRB);
		if (ChildNodeRB.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	if (m_poLanItInf){
		ao_size += sizeof(*m_poLanItInf)*m_iArrayDim;
		// Iteramos por los que tengamos.
		for (m4uint32_t i = 0; i < m_iDocs; i++)
		{
			if (m_poLanItInf[i]->GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}

	}
	return M4_SUCCESS;
}

m4return_t
ClLane_Item_Info::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLane_Item_Info);
	
	// Iteramos por los que tengamos.
	for (m4uint32_t i = 0; i < m_iCount; i++){
		if (m_pHandles[i].GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
		if (m_pConnectItemInfo[i].GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}
