//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:  CVM - DM
// File: slices_variable.cpp
// Project: CVM
// Author:  Meta Software M.S. , S.A
// Date: 29/12/97
// Language:C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
// This module defines...
//
//
//==============================================================================
#include "chlog.hpp"
#include "dmres.hpp"
#include "slices_variable.hpp"
#include "node_knl.hpp"
#include "gm_intrf.hpp"
#include "m4define.hpp"
#include "chlog.hpp"
#include "v_contxt.hpp"

//EDU:
// includes para que reconozca ItemDef_Interface y DatesInfo_Interface
 #include "itemdef.hpp"
 #include "chan_knl.hpp"

#include "m4mdrt.hpp"	// M4MDrt
#include "m4unicode.hpp"	// M4MDrt


#define MinSizeBlock 64 //tamaño minimo del bloque con tramos
#define MinSize 8 //tamaño minimo del bloque sin tramos
#define MinFS 10 //minimo espacio libre permitido absoluto
#define MinFSIns 5 //minimo espacio libre permitido para inserciones
#define BlockFrac 2 //crecimento del bloque
#define SliceHeadInInt32 4 //tamano del offset (4bytes)
#define SliceHeadInDate 2
#define SliceHeadInByte 16
#define DateSize 8
#define Int32Size 4

//******************************************************************
// Funciones que pueden ser llamadas direectamente desde Item.Value

m4return_t _ClSlices_Variable::Count (const StKernelContext &ai_context, m4uint32_t &ao_count, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;

    SliceHeader = *(m4uint32_t **)ao_valueadd;
	ao_count = SliceHeader[1];

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::GetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address)
{
	return M4_ERROR;
}

m4return_t _ClSlices_Variable::GetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t *date;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	date = (m4date_t *)(SliceHeader);
	ao_date = date[(ai_index * SliceHeadInDate) + 1];

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::GetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4return_t res;
	m4date_t unitdate, *date;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

    ai_context.GetUnitDate(unitdate);

	date = (m4date_t *)(SliceHeader);
	ao_date = date[(ai_index + 1) * SliceHeadInDate + 1] - unitdate;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::Size (const StKernelContext &ai_context, m4uint32_t ai_index, 
									 m4uint32_t &ao_size, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4return_t res;
	m4bool_t isnull;

	ao_size = 0;
// me parece que en este metodo no va necesitar una validacion del indice.

    res = IsNull(ai_context, ai_index, isnull, ao_valueadd);

	if (!isnull) {
        SliceHeader = *(m4uint32_t **)ao_valueadd;

		ao_size = SliceHeader[(ai_index + 1) * SliceHeadInInt32 + 1];
	}

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::IsNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t &ao_isnull, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, mask, NewOffset;
	m4return_t res;

    ao_isnull = M4_TRUE;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewOffset = SliceHeader[(ai_index + 1) * SliceHeadInInt32];
	mask = 4;// mask = 00000100

	NewOffset = NewOffset & mask;
	ao_isnull = ( NewOffset ? M4_TRUE : M4_FALSE );

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::GetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t &ao_priority, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, mask, NewOffset;
	m4return_t res;

    ao_priority = 3;
    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewOffset = SliceHeader[(ai_index + 1) * SliceHeadInInt32];
	mask = 3;// mask = 00000011

	NewOffset = NewOffset & mask;
	ao_priority = (priority_t)NewOffset;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::SetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t ao_priority, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewOffset = &SliceHeader[(ai_index + 1) * SliceHeadInInt32];
	mask = 3;// mask = 00000011

	*NewOffset = ((*NewOffset) | mask) + ao_priority - 3;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::SetNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t ai_bSetSize, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewOffset = &SliceHeader[(ai_index + 1) * SliceHeadInInt32];
	mask = 4;// mask = 00000100

	*NewOffset = (*NewOffset) | mask;

    if (ai_bSetSize)
        SliceHeader[(ai_index + 1) * SliceHeadInInt32 + 1] = 0;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::SetValue (const StKernelContext &ai_context, m4uint32_t ai_index, const void *ai_paddress, 
									  m4uint32_t ai_size,  m4uint32_t ai_offset, 
									  m4uint32_t &ao_retSize, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, PrevOffset, OffsetDiff, i, MaskedLast, Size8B, MOffset, FS, 
		Size, NewSize, ItemSize, size_to_set;
	m4return_t res, resout = M4_SUCCESS;
	m4uint32_t ActualSlice, NextSlice, sizeslice;
    m4bool_t bhs;
	size_t iChars = 0 ;
	size_t iSize = 0 ;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

// De momento no se me ocurre nada mejor
    if (ai_paddress == 0)
        return SetNull (ai_context, ai_index, M4_TRUE, ao_valueadd);

    res = SetNoNull(ai_context, ai_index, ao_valueadd);

	ItemSize = ai_context.pItemDef_Interface->Precision() + 1;

	if ((ItemSize != 0) && (ai_offset > ItemSize)) {
		return M4_ERROR;
	}

	// UNICODE LN4
	// Hay que truncar por número de caracteres y no por número de bytes
	iChars = M4StrLen( (char*) ai_paddress, ai_size - 1 ) + 1 ;

	if ((ItemSize != 0) && ((iChars + ai_offset) > ItemSize))
	{
		size_to_set = M4CharPosition( (char*) ai_paddress, ai_size - 1, ItemSize - ai_offset - 1, iSize ) + 1;
		resout = M4_WARNING;
	}
	else
	{
		size_to_set = ai_size;
	}

	Size8B = M4_ROUND_TO_8 (size_to_set + ai_offset);

	res = Compact(ai_context, ao_valueadd);//Mejor hacerlo al principio

	res = MaskedOffset(&SliceHeader[(ai_index + 1) * SliceHeadInInt32], &MOffset);
	res = MaskedOffset(&SliceHeader[ai_index * SliceHeadInInt32], &PrevOffset);

	if ((PrevOffset - MOffset) < Size8B ) {
		OffsetDiff = Size8B - (PrevOffset - MOffset);
		Size = SliceHeader[0];
		FS = FreeSpace(ai_context, ao_valueadd);
		if (FS < (OffsetDiff + (Size / MinFSIns)))
        {
			NewSize = M4_ROUND_TO_8 (((BlockFrac * (OffsetDiff + Size))));
			res = ai_context.pMemory_Interface->SetSize(ai_context, NewSize);
			res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&SliceHeader, bhs);
			res = ai_context.pMemory_Interface->GetSize(ai_context, SliceHeader[0]);
			res = Compact(ai_context, (void**)&SliceHeader);
		}

		res = MaskedOffset(&SliceHeader[ai_index * SliceHeadInInt32], &PrevOffset);//han cambiado los offset!!!!
		res = MaskedOffset(&SliceHeader[(SliceHeader[1])*4], &MaskedLast);
		ActualSlice = MaskedLast;
		NextSlice = MaskedLast - OffsetDiff;
		if (PrevOffset > MaskedLast) {
			sizeslice = PrevOffset - MaskedLast;
			memmove((void *)&SliceHeader[NextSlice/Int32Size],
				(void *)&SliceHeader[ActualSlice/Int32Size], sizeslice);
		}
		else
			sizeslice = 0;
		i = ai_index;
		while (i != (SliceHeader[1])) {//i + 1 + 1 uno por la cabecera y otro por que empiezo en el siguiente
				SliceHeader[(i + 1) * SliceHeadInInt32] = SliceHeader[(i + 1) * SliceHeadInInt32] - OffsetDiff;
				i++;
		}
		res = MaskedOffset(&SliceHeader[(ai_index + 1) * SliceHeadInInt32], &MOffset);
	}
	if ((size_to_set + ai_offset) > SliceHeader[(ai_index + 1) * SliceHeadInInt32 + 1]) {
		SliceHeader[(ai_index + 1) * SliceHeadInInt32 + 1] = size_to_set  + ai_offset;
	}
	res = ai_context.pMemory_Interface->SetValueByOffset (ai_context, ai_paddress, size_to_set, (MOffset + ai_offset), NULL, M4_TRUE, M4_TRUE);

	if (res != M4_SUCCESS)
		return M4_ERROR;

	ao_retSize = size_to_set;

    res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, ao_valueadd, bhs);

	return resout;
}


m4return_t _ClSlices_Variable::SetDescription (const StKernelContext &ai_context,
                                               m4uint32_t ai_index, const void *ai_paddress)
{
    return M4_ERROR;
}

m4return_t _ClSlices_Variable::SetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t NewDate, *EndDate;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewDate = ai_date;
	EndDate = (m4date_t*)(SliceHeader);
	EndDate[(ai_index * SliceHeadInDate) + 1] = NewDate;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::SetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t NewDate, *EndDate, unitdate;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	ai_context.GetUnitDate(unitdate);

	NewDate = ai_date + unitdate;
	EndDate = (m4date_t *)(SliceHeader);
	EndDate[((ai_index + 1) * SliceHeadInDate) + 1] = NewDate;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::Delete (const StKernelContext &ai_context, m4uint32_t &ai_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceToDelete, NextSlice;
	m4uint32_t NS, ai_size;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NS = SliceHeader[1];
	--NS;

	if (NS == 0) {
		return DeleteAll(ai_context, ao_valueadd);
	}

	SliceToDelete = (ai_index * SliceHeadInByte) + DateSize;
	NextSlice = ((ai_index + 1) * SliceHeadInByte) + DateSize;
	ai_size = (NS - ai_index) * SliceHeadInByte + DateSize;//ya se le ha restado uno a NS
	memmove((void *)&SliceHeader[SliceToDelete/Int32Size],
		(void *)&SliceHeader[NextSlice/Int32Size], ai_size);

	if (ai_index == (SliceHeader[1] - 1)) ai_index--;
	SliceHeader[1] = NS;

	return M4_SUCCESS;
}

m4return_t  _ClSlices_Variable::DeleteAll (const StKernelContext &ai_context, void **ao_valueadd)
{
	m4bool_t hasslices = M4_FALSE;
	m4return_t res;

	res = ai_context.pMemory_Interface->SetSize(ai_context, MinSize);
	    res = ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);    //despues de SetSize
	res = ai_context.pMemory_Interface->SetNull(ai_context, M4_TRUE);

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::Insert (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t ai_count, void **ao_valueadd)
{
	m4bool_t hasslices;
	m4uint32_t Size, AddSize,FS, *SliceHeader, NS, i, NewSize;
	m4return_t res;
	m4uint32_t n;
    void * paddress;
    m4bool_t bhs;

    res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, &paddress, hasslices);

	if (!hasslices)
    {
        //EDU:
        hasslices = M4_TRUE;// mejor antes !!

		res = ai_context.pMemory_Interface->SetSize(ai_context, MinSizeBlock);
		    res = ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);    //despues de SetSize
		res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&SliceHeader, bhs);
		SliceHeader[1] = 0;		
	}
    else
    {
        SliceHeader = (m4uint32_t *)paddress;
    }

	res = Count(ai_context, n, (void **) &SliceHeader);
	if (ai_index > n)
    {
        m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
        ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();

        DUMP_CHLOG_WARNINGF (M4_CH_DM_SLICEISEOF, M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }


	res = ai_context.pMemory_Interface->GetSize(ai_context, Size);
	SliceHeader[0] = Size;
	/* Calculo de la cantidad de bytes que van a ocupar los nuevos tramos: AddSize*/
	NS = SliceHeader[1];
	NS += ai_count;
	AddSize = ai_count*24;//fecha(8Bytes)+datos(8Bytes)+offset(4bytes)+longitud(4bytes)
	FS = FreeSpace(ai_context, (void **)&SliceHeader);
	if (FS < AddSize ) {
		res = Compact(ai_context, (void **)&SliceHeader);
		FS =FreeSpace(ai_context, (void **)&SliceHeader);
		if (FS < AddSize ) {
			NewSize = ((BlockFrac * (AddSize + Size))) / DateSize;
			if (((BlockFrac * (AddSize + Size))) % 8)
                NewSize++;
			NewSize *= 8;
			res = ai_context.pMemory_Interface->SetSize(ai_context, NewSize);
			res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&SliceHeader, bhs);
			res = ai_context.pMemory_Interface->GetSize(ai_context, SliceHeader[0]);
			res = Compact(ai_context, (void **)&SliceHeader);
		}
	}
	i = ai_count;
	for (; i!=0; --i) {
		res = SingleInsert(ai_context, SliceHeader, ai_index);
	}

    if (ao_valueadd)
        * ao_valueadd = SliceHeader;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::AllocSlices (const StKernelContext &ai_context, ClDateStack *ai_stack, void **ao_valueadd)
{
	m4uint32_t size, blocksize, PosInBlock8, PosInBlock4, incr4, incr8, *block4;
	m4uint16_t ai_pos, length;
	m4bool_t hasslices = M4_TRUE;
	m4return_t res = 0;
	m4date_t date, *block8;
    m4bool_t bhs;

	length  = ai_stack->GetLength();
	blocksize = M4_ROUND_TO_8(m4uint32_t ((((3 * DateSize) * (length - 1)) + 16) * BlockFrac));
	
	res |= ai_context.pMemory_Interface->SetSize(ai_context, blocksize);
    res |= ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);    //despues de SetSize
	res |= ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&block8, bhs);
	res |= ai_context.pMemory_Interface->GetSize(ai_context, size);

	if (res != M4_SUCCESS) {return M4_ERROR;}

	block4 = (m4uint32_t *)block8;

	block4[1] = m4uint32_t (length - 1);
	block4[0] = m4uint32_t (size);

	ai_pos = 0;
	PosInBlock8 = 1; //se comienza en los segundos 8 bytes
	PosInBlock4 = 4;
	incr8 = 2;
	incr4 = 4;
	size -= 8;

	while (ai_pos < (length - 1)) {
		res = ai_stack->GetElement(ai_pos, date);
		block4[PosInBlock4] = size + 7; //flags de prioridad 3 y nulo
		block4[PosInBlock4 + 1] = 8;
		block8[PosInBlock8] = date;
		PosInBlock8 += incr8;
		PosInBlock4 += incr4;
		ai_pos ++;
		size -= 8;
	}

	res = ai_stack->GetElement(length - 1, date);
	block8[(incr8 * (length -1)) + 1] = date; //fecha final

	return M4_SUCCESS;
}

//******************************************************************
// Funciones llamdas indirectamente

m4return_t _ClSlices_Variable::Serialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice, m4char_t ai_iCppType)
{
	m4uint32_t iNumSlices, iIndex=0;

	if (IOD.Write(*(m4uint32_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;
	iIndex+=sizeof(m4uint32_t);

	iNumSlices=*(m4uint32_t*)&ai_poPtrSlice[iIndex];
	iIndex+=sizeof(m4uint32_t);

	if (IOD.Write(iNumSlices)!=M4_SUCCESS)
		return M4_ERROR;

	for (m4uint32_t i=0; i<iNumSlices; i++)
	{
		// Date
		if (IOD.Write(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4double_t);

		// Offset
		m4uint32_t iOffset;

		iOffset=*(m4uint32_t*)&ai_poPtrSlice[iIndex];

		if (IOD.Write(iOffset)!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4uint32_t);

		if (IOD.Write((void*)&ai_poPtrSlice[iOffset & 0xfffffff8],
			*(m4uint32_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4uint32_t);
	}

	// Last date
	if (IOD.Write(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::DeSerializedSize (ClGenericIODriver& IOD, m4uint32_t&ao_iSize)
{
	if (IOD.Read(ao_iSize)!=M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::DeSerialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice,
											m4char_t  ai_iCppType, m4uint32_t m_Size)
{
	m4uint32_t iNumSlices, iIndex=0;

	*(m4uint32_t*)&ai_poPtrSlice[0]=m_Size;

	iIndex=sizeof(m4uint32_t);

	if (IOD.Read(iNumSlices)!=M4_SUCCESS)
		return M4_ERROR;
	*(m4uint32_t*)&ai_poPtrSlice[iIndex]=iNumSlices;
	iIndex+=sizeof(m4uint32_t);

	for (m4uint32_t i=0; i<iNumSlices; i++)
	{
		// Date
		if (IOD.Read(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4double_t);

		// Offset
		m4uint32_t iOffset;

		if (IOD.Read(iOffset)!=M4_SUCCESS)
			return M4_ERROR;
		*(m4uint32_t*)&ai_poPtrSlice[iIndex]=iOffset;
		iIndex+=sizeof(m4uint32_t);

		if (IOD.Read((void*)&ai_poPtrSlice[iOffset & 0xfffffff8],
			*(m4uint32_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4uint32_t);
	}

	// Last date
	if (IOD.Read(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
}
/*
m4return_t _ClSlices_Variable::CopyItem(const StKernelContext &ai_Dest, StKernelContext &ai_Source, void **ao_valueadd)
{
    return M4_ERROR;
}
*/
m4return_t _ClSlices_Variable::GetAddress_withstartaddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **aio_address, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceAddr;
	m4return_t res;
	m4bool_t isnull;
	
    SliceHeader = *(m4uint32_t **)aio_address;
    *aio_address = 0;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF_GETVALUE);
    if (res != M4_SUCCESS)
        return res;

	res = IsNull(ai_context, ai_index, isnull, (void **)&SliceHeader);
	if (!isnull) {
		res = MaskedOffset(&SliceHeader[(ai_index + 1) * SliceHeadInInt32], &SliceAddr);
		* aio_address = (m4char_t *) &SliceHeader[SliceAddr / SliceHeadInInt32];
	}

	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::GetAddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceAddr;
	m4return_t res;
	m4bool_t isnull;

	*ao_address = 0;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF_GETVALUE);
    if (res != M4_SUCCESS)
        return res;

    res = IsNull(ai_context, ai_index, isnull, (void**)&SliceHeader);
	if (!isnull) {
		res = MaskedOffset(&SliceHeader[(ai_index + 1) * SliceHeadInInt32], &SliceAddr);
		* ao_address = (m4char_t *) &SliceHeader[SliceAddr / SliceHeadInInt32];
	}

	return M4_SUCCESS;
}
	

m4return_t _ClSlices_Variable::GetEndDatePlusOne (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t *date;
	m4return_t res;
    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	date = (m4date_t *)(SliceHeader);
	ao_date = date[(ai_index + 1) * SliceHeadInDate + 1];

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::GetFirstIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd)
{
	ao_index = 0;
	return M4_SUCCESS;
}

m4return_t _ClSlices_Variable::GetLastIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

	ao_index = SliceHeader[1] - 1;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::GetFirstStartDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd)
{
	return GetStartDate (ai_context, 0, ao_date, ao_valueadd);
}

m4return_t _ClSlices_Variable::GetLastEndDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd)
{

	m4uint32_t *SliceHeader;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

	return GetEndDate (ai_context, SliceHeader[1] - 1, ao_date, ao_valueadd);
}


m4return_t _ClSlices_Variable::FindByDate (const StKernelContext &ai_context, m4date_t ai_date, m4uint32_t &ao_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, index;
	m4date_t date;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    index = 0;
	res = GetFirstStartDate(ai_context, date, ao_valueadd);
	if (ai_date < date) {
		ao_index = M4DM_EOF_INDEX;
		return M4_WARNING;
	}

	while (index < SliceHeader[1])
	{
		GetEndDate(ai_context, index, date, ao_valueadd);
        if (ai_date <= date)
            break;
		index++ ;
	}

 //---------->>
 // EDU: en caso de no encontrar el elemento
 // el comportamiento es: devolver en ao_index -1 y como retorno un warning
	//if (index >= (SliceHeader[1])) return M4_ERROR;
 if (index >= (SliceHeader[1])) {
  ao_index = M4DM_EOF_INDEX;
  return M4_WARNING;
 }
 //----------<<
	else ao_index = index;

	return M4_SUCCESS;
}


//******************************************************************
// Funciones internas de la clase

m4return_t _ClSlices_Variable::Compact(const StKernelContext &ai_context,
                                       void **ao_valueadd)
{
	m4uint32_t *SliceHeader, index, *length, MOffset, MOffsetLast, length8B, Flags;
	m4return_t res;
	m4uint32_t Size, ActualSlice, NextSlice, sizeslice;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ai_context.pMemory_Interface->GetSize(ai_context, Size);

	index = 0;
	MOffsetLast = Size;

	while (index < (SliceHeader[1])) {
		length = SliceHeader + ((index + 1) * SliceHeadInInt32) + 1;
		res = MaskedOffset(&SliceHeader[(index + 1) * SliceHeadInInt32], &MOffset);
		res = FlagMaskedOffset(&SliceHeader[(index + 1) * SliceHeadInInt32], &Flags);

		if ((*length) % DateSize) length8B = ((*length) / DateSize) + 1;
		else length8B = ((*length) / DateSize);
		length8B = length8B * DateSize;
		if (MOffset < (MOffsetLast - length8B)) {
			ActualSlice = MOffset;
			NextSlice = MOffsetLast - length8B;
			sizeslice = length8B;
			memmove((void *)&SliceHeader[NextSlice/Int32Size],
				(void *)&SliceHeader[ActualSlice/Int32Size], sizeslice);
			SliceHeader[(index + 1) * SliceHeadInInt32] = MOffsetLast - length8B + Flags;
		}
		MOffsetLast = MOffsetLast - length8B;
		index++ ;
	}
	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::SingleInsert(const StKernelContext &ai_context,
                                            m4uint32_t *SliceHeader, m4uint32_t ai_index,
                                            void **ao_valueadd)
{
	m4uint32_t NS, NextSlice, ai_size, NewSlice, ActualSlice;
	m4date_t *date, newdate;
	m4return_t res;

	NS = SliceHeader[1];

	ai_size = (NS - ai_index) * SliceHeadInByte + DateSize;//tramos + fecha final
	ActualSlice = (ai_index * SliceHeadInByte) + DateSize;
	NextSlice = ((ai_index + 1) * SliceHeadInByte) + DateSize;
	memmove((void *)&SliceHeader[NextSlice/Int32Size],
		(void *)&SliceHeader[ActualSlice/Int32Size], ai_size);
	SliceHeader[1] = ++NS;

	SliceHeader[(ai_index + 1) * SliceHeadInInt32] = SliceHeader[ai_index * SliceHeadInInt32] - (SliceHeader[ai_index * SliceHeadInInt32] % DateSize);
	//tamaño 0 -> offset = offset(previo)!!!
	SliceHeader[(ai_index + 1) * SliceHeadInInt32 + 1] = 0; //tamaño por defecto es 0
	res = SetNull(ai_context, ai_index, M4_FALSE, (void **)&SliceHeader); //Inicializacion normal: nulo y porioridad 3
	res = SetPriority(ai_context, ai_index, 3, (void **)&SliceHeader);
	res = MaskedOffset(&SliceHeader[(ai_index + 1) * SliceHeadInInt32], &NewSlice);
	newdate = 0;
	date = (m4date_t *)(SliceHeader);
	date[(ai_index * SliceHeadInDate) + 1] = newdate;//pongo la fecha a cero

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::FlagMaskedOffset(m4uint32_t *Offset, m4uint32_t *MOffset,
                                                void **ao_valueadd)
{
	m4uint32_t Mask = 7; // 0000 0111

	*MOffset = (*Offset) & Mask;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Variable::MaskedOffset(m4uint32_t *Offset, m4uint32_t *MOffset,
                                            void **ao_valueadd)
{
	m4uint32_t Mask = 7; // 0000 0000 0000 0111

	*MOffset = ((*Offset) | Mask) - 7; //hay que restarle 0000 00000 00000 0111;

	return M4_SUCCESS;
}

m4uint32_t _ClSlices_Variable::FreeSpace(const StKernelContext &ai_context,
                                         void **ao_valueadd)
{
	m4uint32_t *SliceHeader, MLastOffset, Size;
	m4return_t res;
	m4uint32_t FS;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

	res = ai_context.pMemory_Interface->GetSize(ai_context, Size);
	res = MaskedOffset(&SliceHeader[(SliceHeader[1]) * SliceHeadInInt32], &MLastOffset);
	FS = (MLastOffset) - (((SliceHeader[1])+1) * SliceHeadInByte);

	return FS;
}

m4return_t _ClSlices_Variable::SetNoNull (const StKernelContext &ai_context,
                                          m4uint32_t ai_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NewOffset = &SliceHeader[(ai_index + 1) * SliceHeadInInt32];
	mask = 4;// mask = 00000100

	*NewOffset = ((*NewOffset) | mask) - 4;

	return M4_SUCCESS;
}

