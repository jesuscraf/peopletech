#include "val.hpp"
#include "clgeniod.hpp"

_ClGM_Value    ClValue::s_gmval;
_ClGM_OldValue ClOldValue::s_gmoldval;


ClValue::ClValue (const ClValue &ai_value) : _ClValue_ReadWrite_Slices (ai_value)
{
    _Copy (ai_value);
}

ClValue &ClValue::operator= (const ClValue &ai_value)
{
    _Copy (ai_value);
    return *this;
}

ClValue &ClValue::operator[] (ClSliceIndex ai_sliceindex)
{
    _ClValue_Read_Slices::operator[] (ai_sliceindex);
    return *this;
}

ClValue &ClValue::operator[] (m4date_t ai_date)
{
    _ClValue_Read_Slices::operator[] (ai_date);
    return *this;
}

ClValue &ClValue::operator++ ()     //pre
{
    _ClValue_Read_Slices::operator++ ();
    return *this;
}

ClValue &ClValue::operator-- ()     //pre
{
    _ClValue_Read_Slices::operator-- ();
    return *this;
}


//------------

ClOldValue::ClOldValue (const ClValue &ai_value) : _ClValue_Read_Slices (ai_value)
{
    _Copy (ai_value);
}

ClOldValue &ClOldValue::operator= (const ClValue &ai_value)
{
    _Copy (ai_value);
    return *this;
}

ClOldValue &ClOldValue::operator[] (ClSliceIndex ai_sliceindex)
{
    _ClValue_Read_Slices::operator[] (ai_sliceindex);
    return *this;
}

ClOldValue &ClOldValue::operator[] (m4date_t ai_date)
{
    _ClValue_Read_Slices::operator[] (ai_date);
    return *this;
}

ClOldValue &ClOldValue::operator++ ()     //pre
{
    _ClValue_Read_Slices::operator++ ();
    return *this;
}

ClOldValue &ClOldValue::operator-- ()     //pre
{
    _ClValue_Read_Slices::operator-- ();
    return *this;
}


