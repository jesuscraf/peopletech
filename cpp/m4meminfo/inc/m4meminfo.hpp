
#ifndef _HPP_M4MEMINFO_
#define _HPP_M4MEMINFO_

#include <m4meminfo_dll.hpp>
#include <m4types.hpp>



M4_DECL_M4MEMINFO m4int32_t M4GetProcessMem(m4uint64_t& ao_pb) ;
M4_DECL_M4MEMINFO m4return_t M4GetSystemCPU(m4double_t& ao_cpu) ;
M4_DECL_M4MEMINFO m4int32_t M4GetSystemMem(m4uint64_t& ao_lAvailPhysMem, m4uint64_t& ao_lTotalPhysMem, m4uint64_t& ao_lAvailSwapMem, m4uint64_t& ao_lTotalSwapMem) ;

#endif

