#include "m4types.hpp"
#include "register.hpp"
#include "valh_rs.hpp"
#include "totalize.hpp"
#include "cltotacum.hpp"


m4return_t
ClGetValue::operator()(_ClRegister_Base * ai_poReg, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue)
{
	return  ai_poReg->Item.Value.GetpSliceFooter().Get(ao_vTotVal);
}

m4return_t
ClGetValue::operator()(_ClValue_Read_Slices * ai_poSlices, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue)
{
	return ai_poSlices->Get(ao_vTotVal);
}

m4return_t
ClGetValue::operator()(ClRgAc_Base * ai_poAcumTot, ClVectorVariants &ao_vTotVal, m4bool_t ai_bWeightedValue)
{
	m4return_t ret = M4_SUCCESS;
	m4int32_t iNumRows;
	iNumRows =  ai_poAcumTot->GetNumRows();
	if (ao_vTotVal.GetSize() == 0){
		m4VariantType vNull;
		vNull.SetNull ();
		for ( m4int32_t i = 0; i < iNumRows; i++ )	ao_vTotVal.Insert (&vNull);
	}
	ret = ai_poAcumTot->GetValueByIndexPos (ao_vTotVal);
	return ret;
}

m4return_t
ClGetValue::operator()(ClRgAc *pRegAcumTot,ClVectorVariants &ao_vTotVal, m4bool_t ai_bWeightedValue)
{
	return 	pRegAcumTot->m_ptotal->Get (ao_vTotVal);
}

m4return_t
ClGetValue::operator()(ClRgBoundedAc *pRegAcumTot, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue)
{
	if ( ai_bWeightedValue == M4_TRUE )
	{
		return 	pRegAcumTot->GetWeightedValue(ao_vTotVal);
	}
	else
	{
		return 	pRegAcumTot->Get(ao_vTotVal);
	}
}


