
#ifndef __FILTER_I__HPP__
#define __FILTER_I__HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include "index.hpp"


class _ClRegisterFilter;
class _ClRegister_Base;
class ClBSearchIndex;
class ClChannel;
class IChangeListener;


class M4_DECL_M4DM ClFilterInterface
{
public:
    ClFilterInterface ();
   ~ClFilterInterface ();

    enum FilterType {StaticFilter, DynamicFilter};

    void Attach (_ClRegister_Base *ai_pregister);
    m4return_t Init (m4bool_t ai_ispartialload, m4uint8_t ai_autoloadmode);

	void	SetChangeListener( IChangeListener *ai_poChangeListener )
	{
		m_poChangeListener = ai_poChangeListener ;
	}

    m4int16_t Count (void);

	m4return_t AddNew (m4char_t * ai_pForm, FilterType ai_Static, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs, m4pchar_t ai_pName = 0);
	m4return_t AddNew (m4int32_t ai_pHandleForm, FilterType ai_Static, m4pchar_t ai_pName = 0);
    m4return_t AddNew (m4uint8_t ai_OpType, FilterType ai_Static, m4pchar_t ai_pName = 0);

    m4return_t AddFastFilter (m4pchar_t ai_pItemName, m4VariantType ai_Value, FilterType ai_Static = StaticFilter, m4pchar_t ai_pName = 0);
    m4return_t AddFastFilter (ClItemIndex ai_idxItem, m4VariantType ai_Value, m4uint32_t ai_iOperation=M4CL_CASE_EQUAL_FUNCTION , FilterType ai_Static = StaticFilter, m4pchar_t ai_pName = 0);

    m4return_t AddSort (m4uint32_t ai_id, m4pchar_t ai_pName = 0);
    m4return_t AddSort (m4uint16_t ai_ipos, m4pchar_t ai_pName = 0);
    m4return_t AddSort (m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders, m4pchar_t ai_pName = 0);

    m4return_t DeleteLast (void);
    m4return_t DeleteAll (void);

    void DestroyChannel_Before (ClChannel * ai_poCh);

    m4return_t UpdateOneIndex(void);
    m4return_t UpdateFilter(void);

    m4return_t CopyFilter (const ClFilterInterface &ai_fi, m4bool_t ai_bTotal = M4_FALSE);

    _ClRegisterFilter * operator -> (void) const
    {
        M4_ASSERT (m_pClFilter);
        return m_pClFilter;
    }

    _ClRegisterFilter & operator [] (m4int16_t ai_iFilter);

    _ClRegisterFilter & operator [] (m4pchar_t ai_pName);

    //pública, se llama directamente desde el iterador de registro
    //que se mueve por los bloques
    m4return_t Add (_ClRegisterFilter * AuxFilter);

    m4bool_t IsInitialized (void);

    ClBSearchIndex * GetSearchIndexAndSorted (m4bool_t & ao_bIsSorted);

    m4bool_t Find (m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions, m4puint32_t ai_piNextIndex = 0);

    void AttachFilterCollection(_ClRegisterFilter * ai_pClFilter);

//------------------------
//autoload
    public:
        m4return_t SetAutoLoadMode (m4uint8_t ai_mode);
    private:
        _ClRegisterFilter *_GetAutoLoadTerminator (m4uint8_t ai_mode);
//------------------------

    public:
        _ClRegister_Base * GetpRegister(void)
        {
            return m_pClRegister;
        }
	m4return_t GetSize(ClSize &ao_size);

private:

    _ClRegisterFilter *m_pClFilter;
    _ClRegister_Base *m_pClRegister;
	IChangeListener	*m_poChangeListener ;
};

#endif //__FILTER_I__HPP__
