
#include "itempx_c.hpp"
#include "node_knl.hpp"
#include "syncro.hpp"


ClItemProxy_Array::ClItemProxy_Array ()
{
    m_IINode.IncReference ();   //para que ningún patoso (yo mismo) lo borre
    m_IIBlock.IncReference ();
    m_IIRegister.IncReference ();
    m_IIDataNode.IncReference ();
    m_IIDataBlock.IncReference ();
    m_IIDataRegister.IncReference ();

#ifdef _DEBUG
    m_IINode.SetIsStackObject ();   //para detectar si alguien intenta borrar
    m_IIBlock.SetIsStackObject ();
    m_IIRegister.SetIsStackObject ();
    m_IIDataNode.SetIsStackObject ();
    m_IIDataBlock.SetIsStackObject ();
    m_IIDataRegister.SetIsStackObject ();
#endif
}

ClItemProxy_Array::~ClItemProxy_Array ()
{
#ifdef _DEBUG
//    M4_ASSERT (m_IINode.GetReferenceCount () == 1);
//    M4_ASSERT (m_IIBlock.GetReferenceCount () == 1);
//    M4_ASSERT (m_IIRegister.GetReferenceCount () == 1);
//    M4_ASSERT (m_IIDataNode.GetReferenceCount () == 1);
//    M4_ASSERT (m_IIDataBlock.GetReferenceCount () == 1);
//    M4_ASSERT (m_IIDataRegister.GetReferenceCount () == 1);
#endif
}

//inicialización:

m4return_t ClItemProxy_Array::Init (ClNodeDef &ai_nodedef, ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pcidk)
{
    m4return_t res = M4_SUCCESS;

    m_pcmcr = ai_pcmcr;
    M4_ASSERT (m_pcmcr);

    m_pcidk = ai_pcidk;    
    M4_ASSERT (m_pcidk);

    m_IINode.Attach (m_pcmcr, m_pcidk);
    m_IIBlock.Attach (m_pcmcr, m_pcidk);
    m_IIRegister.Attach (m_pcmcr, m_pcidk);
    m_IIDataNode.Attach (m_pcmcr, m_pcidk);
    m_IIDataBlock.Attach (m_pcmcr, m_pcidk);
    m_IIDataRegister.Attach (m_pcmcr, m_pcidk);

    //Refresh (ai_nodedef);

    return res;
}

//llamada en el 'encarrilamiento':

void ClItemProxy_Array::Refresh (ClNode_Kernel *ai_poNodeKernel)
{
    m4uint16_t nin, nib, nir;
	ClMutex	*poMutex = NULL;

	/* Bug 0278825
	Se bloquea la construcción en multithread
	*/
	if( ai_poNodeKernel != NULL )
	{
		poMutex = ai_poNodeKernel->ItemProxyMutex;

		if( poMutex != NULL )
		{
			poMutex->Lock();
		}
	}

    nin = ai_poNodeKernel->NumberOfNodeItems ();
    nib = ai_poNodeKernel->NumberOfBlockItems ();
    nir = ai_poNodeKernel->NumberOfRegisterItems ();

    // precondición: que nodedef esté perfectamente preparado para ser usado
    m_IINode.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_NODE, M4DM_TYPE_ALL, nin);
    m_IIBlock.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK, M4DM_TYPE_ALL, nin + nib);
    m_IIRegister.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_ALL, M4DM_TYPE_ALL, nin + nib + nir);

    m_IIDataNode.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_NODE, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
    m_IIDataBlock.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_BLOCK, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
    m_IIDataRegister.SetModeMandatory (poMutex != NULL, M4DM_SCOPE_REGISTER, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);

	if( poMutex != NULL )
	{
		poMutex->Unlock();
	}
}

ClRTItemIterator *ClItemProxy_Array::GetpRTII (ClNode_Kernel *ai_poNodeKernel, m4uint8_t ai_newscope, m4uint8_t ai_newtype)
{
	ClMutex	*poMutex = NULL;
    ClRTItemIterator *pnewrtii = 0;
    m4uint16_t total;

	/* Bug 0278825
	Se bloquea la construcción en multithread
	*/
	if( ai_poNodeKernel != NULL )
	{
		poMutex = ai_poNodeKernel->ItemProxyMutex;

		if( poMutex != NULL )
		{
			poMutex->Lock();
		}
	}

    //cambio: inicializaciones on the fly

    if (M4DM_TYPE_ALL == ai_newtype)
	{
        switch (ai_newscope)
		{
            case M4DM_SCOPE_NODE:
            {
                if (!m_IINode.IsInit ()) {
                    total = ai_poNodeKernel->NumberOfNodeItems ();
                    m_IINode.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype, total);
                }
                pnewrtii = &m_IINode;
                break;
            }
            case (M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK):
            {
                if (!m_IIBlock.IsInit ()) {
                    total = ai_poNodeKernel->NumberOfNodeItems () + ai_poNodeKernel->NumberOfBlockItems ();
                    m_IIBlock.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype, total);
                }
                pnewrtii = &m_IIBlock;
                break;
            }
            case M4DM_SCOPE_ALL:
            {
                if (!m_IIRegister.IsInit ()) {
                    total = ai_poNodeKernel->NumberOfNodeItems () + ai_poNodeKernel->NumberOfBlockItems () + ai_poNodeKernel->NumberOfRegisterItems ();
                    m_IIRegister.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype, total);
                }
                pnewrtii = &m_IIRegister;
                break;
            }
        }
    }
    else if ( (M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT) == ai_newtype )
	{
        switch (ai_newscope)
		{
            case M4DM_SCOPE_NODE:
            {
                if (!m_IIDataNode.IsInit ()) {
                    m_IIDataNode.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype);
                }
                pnewrtii = &m_IIDataNode;
                break;
            }
            case M4DM_SCOPE_BLOCK:
            {
                if (!m_IIDataBlock.IsInit ()) {
                    m_IIDataBlock.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype);
                }
                pnewrtii = &m_IIDataBlock;
                break;
            }
            case M4DM_SCOPE_REGISTER:
            {
                if (!m_IIDataRegister.IsInit ()) {
                    m_IIDataRegister.SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype);
                }
                pnewrtii = &m_IIDataRegister;
                break;
            }
        }
    }

    //no existe, hay que crear el array con los punteros a los items que vemos:
    if (0 == pnewrtii) {
        pnewrtii = new ClRTItemIterator;
        M4_ASSERT (pnewrtii);
        pnewrtii->Attach (m_pcmcr, m_pcidk);
        pnewrtii->SetModeMandatory (poMutex != NULL, ai_newscope, ai_newtype);
    }

	if( poMutex != NULL )
	{
		poMutex->Unlock();
	}

    return pnewrtii;
}

m4return_t ClItemProxy_Array::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClItemProxy_Array);

	ao_size-=sizeof(m_IINode);
	if (m_IINode.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(m_IIBlock);
    if (m_IIBlock.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(m_IIRegister);
    if (m_IIRegister.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(m_IIDataNode);
	if (m_IIDataNode.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(m_IIDataBlock);
    if (m_IIDataBlock.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_size-=sizeof(m_IIDataRegister);
    if (m_IIDataRegister.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;

}

/**
 * Hace un reset de los contadores de referencias de cada uno de los ClRTItemIterator.
 * Este método sólo debe llamarse desde el ClNodeDirectory_Data::ResetRTItemIteratorsCountReferences,
 * para forzar una desconexión.
 * Con esto conseguimos que cuando se borre el canal no aparezca un assert indicando que el número
 * de referencias no es correcto (por no haber destruido los accesos).
 */
void ClItemProxy_Array::ResetRTItemIteratorsCountReferences(void)
{
	m_IINode.ResetCountReferences();
	m_IIBlock.ResetCountReferences();
	m_IIRegister.ResetCountReferences();
	m_IIDataNode.ResetCountReferences();
	m_IIDataBlock.ResetCountReferences();
	m_IIDataRegister.ResetCountReferences();
}

