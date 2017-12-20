#ifndef __VDELTA__INL__
#define __VDELTA__INL__


// ----------------------------------------------------------------------------
// class ClDeltaInfo
// ----------------------------------------------------------------------------

M4_INLINE void ClDeltaInfo::OnModified(void)  { m_iDeltaInfo |= VD_MODIFIED; }
M4_INLINE void ClDeltaInfo::OnDeleted(void)   { m_iDeltaInfo |= VD_DELETED;  }
M4_INLINE void ClDeltaInfo::OnInvalid(void)   { m_iDeltaInfo |= VD_INVALID; }

M4_INLINE void ClDeltaInfo::OffModified(void) { m_iDeltaInfo &= ~VD_MODIFIED; }
M4_INLINE void ClDeltaInfo::OffDeleted(void)  { m_iDeltaInfo &= ~VD_DELETED;  }
M4_INLINE void ClDeltaInfo::OffInvalid(void)  { m_iDeltaInfo &= ~VD_INVALID; }

M4_INLINE regindex_t ClDeltaInfo::ClientIndex(void) const { return m_iDeltaInfo & VD_CLIENT_INDEX; }

M4_INLINE m4bool_t ClDeltaInfo::IsInserted(void) const
{
   return ( ( ClientIndex() == VD_NEW_RECORD ) ? M4_TRUE : M4_FALSE );
}

M4_INLINE m4bool_t ClDeltaInfo::IsModified(void) const
{ 
   return (m_iDeltaInfo & VD_MODIFIED) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClDeltaInfo::IsDeleted(void) const
{ 
   return (m_iDeltaInfo & VD_DELETED) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClDeltaInfo::IsInvalid(void) const
{ 
   return (m_iDeltaInfo & VD_INVALID) ? M4_TRUE : M4_FALSE;
}

M4_INLINE void ClDeltaInfo::ClientIndex(const regindex_t ai_clientIndex) { m_iDeltaInfo = ai_clientIndex; }


// ----------------------------------------------------------------------------
// class ClVectorDelta
// ----------------------------------------------------------------------------

M4_INLINE void ClVectorDelta::OnModifiedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria en ClVectorDelta::OnFlag");
   M4_MSG_ASSERT(ai_Index < m_iCount && ai_Index >= 0,"Index fuera de rango en ClVectorDelta::OnFlag");
  
   m_index[ai_Index].OnModified();

   m_bAnyChange = M4_TRUE;
}

M4_INLINE void ClVectorDelta::OnDeletedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");
  
   m_index[ai_Index].OnDeleted();

   m_bAnyChange = M4_TRUE;
}

M4_INLINE void ClVectorDelta::OffModifiedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");
  
   m_index[ai_Index].OffModified();

//   m_anyChange=M4_TRUE;       // ???
}

M4_INLINE void ClVectorDelta::OffDeletedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");
  
   m_index[ai_Index].OffDeleted();

//   m_anyChange=M4_TRUE;       // ???
}

M4_INLINE m4bool_t ClVectorDelta::GetInsertedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount,"Index fuera de rango");
  
   return m_index[ai_Index].IsInserted();
}

M4_INLINE m4bool_t ClVectorDelta::GetModifiedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");

   return m_index[ai_Index].IsModified();
}

M4_INLINE m4bool_t ClVectorDelta::GetDeletedByPos(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");

   return m_index[ai_Index].IsDeleted();
}

M4_INLINE regindex_t ClVectorDelta::ClientIndex(regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");
  
   return m_index[ai_Index].ClientIndex();
}

M4_INLINE m4bool_t ClVectorDelta::ThereIsAnyChange(void) const
{
   return m_bAnyChange;
}

M4_INLINE void ClVectorDelta::ThereIsAnyChange(const m4bool_t ai_bAnyChange)
{
   m_bAnyChange = ai_bAnyChange;
}

M4_INLINE ClDeltaInfo& ClVectorDelta::Get(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(m_index, "No se reservo memoria");
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < m_iCount, "Index fuera de rango");

   return m_index[ai_Index];
}

#endif
