#include "val_ge.hpp"

//*************************************************************************
// FUNCIONES PARA EL MANTENIMIENTO DEL ESTADO y VOLCADO DE ERRORES (INLINE)
//*************************************************************************

M4_INLINE void ClValue_State::_Attach27 (ClItem *ai_pii, ClAccess * ai_poAccess)
{
    m_pItem_Interface = ai_pii;
    m_oContext.m_poAccess = ai_poAccess;
}

M4_INLINE _ClVirtual_Value_R *ClValue_State::GetpKernelR (void)
{
    return (_ClVirtual_Value_R *)m_oContext.pClItemProxy->GetpKernel ();
}

M4_INLINE _ClVirtual_Value_W *ClValue_State::GetpKernelRW (void)
{
    return (_ClVirtual_Value_W *)m_oContext.pClItemProxy->GetpKernel ();
}

M4_INLINE const ClBlockReference &ClValue_State::GetCurrentBlockRef (void) const
{
    return m_oContext.BlockReference;
}

M4_INLINE void ClValue_State::_SetItemProxy (_AClItemProxy *ai_pnewitemproxy)
{
    M4_ASSERT (ai_pnewitemproxy);
    M4_ASSERT (ai_pnewitemproxy != &s_oClItemProxy_Nihil);

    m_oContext.pClItemProxy = ai_pnewitemproxy;

    if (Index == M4DM_EOF_INDEX) {
        // si el indice está a eof (caso inicial), lo llevamos al primer value:
        Index = 0;
    }
    else {
        if (Index) {
            // intentamos quedarnos en el index que teniamos:
			/* Bug 0243222
			Si no hay registros no hay que llamar al Count que solo comprueba la validez en debug
			*/
            if (m_oContext.BlockReference->Count() > 0 && Index < Count()) {
                // ok                
            }
            else {
                // si no lo conseguimos, reseteamos el valor del index (al primer value):
                Index = 0;
            }
        }
    }
}

M4_INLINE ClRegisterIndex ClValue_State::GetRegisterIndex (void) const
{
    return m_oContext.GetRegisterIndex ();
}

M4_INLINE ClMMIndex ClValue_State::GetMMIndex (void) const
{
    return (* m_oContext.m_piMMIndex);
}

M4_INLINE _ClRegister_Base *ClValue_State::GetpRegister () const
{
    return m_oContext.GetpRegister ();
}

