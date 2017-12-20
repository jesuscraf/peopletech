
#include "iteminfo.hpp"

#ifndef __ITEMPRX_C_HPP__
#define __ITEMPRX_C_HPP__

class ClColItemDef_Kernel;
class ClCompiledMCR;
class ClNode_Kernel;


class ClItemProxy_Array
{
public:
    ClItemProxy_Array ();
   ~ClItemProxy_Array ();

    m4return_t Init (ClNodeDef &ai_nodedef, ClCompiledMCR *ai_pcmcr, ClColItemDef_Kernel *ai_pcidk);
    void Refresh (ClNode_Kernel *ai_poNodeKernel);

    ClRTItemIterator *GetpRTII (ClNode_Kernel *ai_poNodeKernel, m4uint8_t ai_newscope, m4uint8_t ai_newtype);

	m4return_t GetSize(ClSize &ao_size);

	/**
	 * Hace un reset de los contadores de referencias de cada uno de los ClRTItemIterator.
	 * Este método sólo debe llamarse desde el ClNodeDirectory_Data::ResetRTItemIteratorsCountReferences,
	 * para forzar una desconexión.
	 * Con esto conseguimos que cuando se borre el canal no aparezca un assert indicando que el número
	 * de referencias no es correcto (por no haber destruido los accesos).
	 */
	void ResetRTItemIteratorsCountReferences(void);

private:
    //Subfiltros, según el modo:
    ClRTItemIterator m_IINode;
    ClRTItemIterator m_IIBlock;
    ClRTItemIterator m_IIRegister;
    ClRTItemIterator m_IIDataNode;
    ClRTItemIterator m_IIDataBlock;
    ClRTItemIterator m_IIDataRegister;

    ClCompiledMCR       *m_pcmcr;
    ClColItemDef_Kernel *m_pcidk;
};

#endif //__ITEMPRX_C_HPP__

