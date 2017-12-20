
// Code based on AIX 5L Performance Tools Handbook samples.
// http://www.redbooks.ibm.com/pubs/pdfs/redbooks/sg246039.pdf

// Other monitor program:
// http://www.mesa.nl/pub/monitor/

// Other monitor program, without source code.
// nmon. http://www-106.ibm.com/developerworks/eserver/articles/analyze_aix/

#include "m4meminfo.hpp"
#include "m4types.hpp"
#include "m4trace.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/var.h>
#include <libperfstat.h>

#include <sys/procfs.h>
#include <fcntl.h>


void cpu_total_t(int t, u_longlong_t& ao_total)
{
    perfstat_cpu_total_t ub;
    static u_longlong_t ttime;
    
    static u_longlong_t user = 0;
    static u_longlong_t sys = 0;
    static u_longlong_t idle = 0;
    static u_longlong_t wait = 0;
    int errorCode = 0;
    
    if (t) 
    {
        errorCode = perfstat_cpu_total((perfstat_id_t*)NULL, &ub, sizeof(perfstat_cpu_total_t), 1);
        if (errorCode>=0) 
        {
            ttime = (ub.user-user) + (ub.sys-sys) + (ub.idle-idle) + (ub.wait-wait);

            user = (ub.user-user)*100/ttime;
            m4TraceLevel(2, printf("%3lld ", user));

            sys = (ub.sys-sys)*100/ttime;
            m4TraceLevel(2, printf("%3lld ", sys));

            idle = (ub.idle-idle)*100/ttime;
            m4TraceLevel(2, printf("%3lld ", idle));

            wait = (ub.wait-wait)*100/ttime;
            m4TraceLevel(2, printf("%3lld ", wait));

            m4TraceLevel(2, printf("\n\n"));

            ao_total = user + sys;
        } 
        else 
        {
            ao_total = 0;
        }
    } 
    else 
    {
        errorCode = perfstat_cpu_total((perfstat_id_t*)NULL, &ub, sizeof(perfstat_cpu_total_t), 1);
        if (errorCode>=0) 
        {
            user = ub.user;
            sys = ub.sys;
            idle = ub.idle;
            wait = ub.wait;
        } 
        else 
        {
            user = 0;
            sys = 0;
            idle = 0;
            wait = 0;
        }
    }
}


void memory_total_t(
    u_longlong_t& ao_lAvailPhysMem, 
    u_longlong_t& ao_lTotalPhysMem, 
    u_longlong_t& ao_lAvailSwapMem, 
    u_longlong_t& ao_lTotalSwapMem)
{
    perfstat_memory_total_t ub;

    static u_longlong_t real_free = 0;
    static u_longlong_t real_total = 0;
    static u_longlong_t pgsp_free = 0;
    static u_longlong_t pgsp_total = 0;

    // memory is in units of 4K pages.
    static double toMB = 4.0 / 1024;
    static u_longlong_t toBytes = 4 * 1024;
    int errorCode = 0;

    errorCode = perfstat_memory_total((perfstat_id_t*)NULL, &ub, sizeof(perfstat_memory_total_t), 1);
    if (errorCode>=0) 
    {
        m4TraceLevel(2, printf("%6.6s %6.6s %6.6s %6.6s %6.6s %6.6s %6.6s \n", "total", "free", "use", "total", "free", "rsvd", "active"));            
        m4TraceLevel(2, printf("%.2f ", (double)ub.virt_total*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.real_free*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.real_inuse*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.pgsp_total*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.pgsp_free*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.pgsp_rsvd*toMB));
        m4TraceLevel(2, printf("%.2f ", (double)ub.virt_active*toMB));
        m4TraceLevel(2, printf("\n\n"));

		/* Bug 0138807
		real_free no da la memoria que potencialmente podría llegar a tener disponible una aplicación,
		ya que contabiliza las páginas que se han usado para ficheros aunque su aplicación haya muerto.
		http://publib16.boulder.ibm.com/doc_link/en_US/a_doc_lib/aixbman/prftungd/memperf1.htm
		Hay que usar el valor virt_active que da todas las páginas consumidas, tanto reales como de paginación
		*/
        real_free = ub.virt_total - ub.virt_active - ub.pgsp_free;
        real_total = ub.real_total;
        pgsp_free = ub.pgsp_free;
        pgsp_total = ub.pgsp_total;
    } 

    ao_lAvailPhysMem = real_free * toBytes; 
    ao_lTotalPhysMem = real_total * toBytes;  
    ao_lAvailSwapMem = pgsp_free * toBytes;  
    ao_lTotalSwapMem = pgsp_total * toBytes; 
}



// All output values in Kb.
m4int32_t  M4GetSystemMem (m4uint64_t& ao_lAvailPhysMem, 
                           m4uint64_t& ao_lTotalPhysMem, 
                           m4uint64_t& ao_lAvailSwapMem, 
                           m4uint64_t& ao_lTotalSwapMem) 
{
    static m4uint64_t toMB = 1024 * 1024;

    memory_total_t(ao_lAvailPhysMem, ao_lTotalPhysMem, ao_lAvailSwapMem, ao_lTotalSwapMem);    
    m4TraceLevel(2, printf("%.2f %.2f %.2f %.2f \n", (double)ao_lAvailPhysMem/toMB, (double)ao_lTotalPhysMem/toMB, (double)ao_lAvailSwapMem/toMB, (double)ao_lTotalSwapMem/toMB));

	return M4_SUCCESS ;
}



m4int32_t M4GetProcessMem (m4uint64_t &ao_cPrivateBytes)
{
    // ps info
    // pr_size - process image size in KB (1024) units
    // pr_rssize - resident set size in KB (1024) units
	psinfo_t oInfo ;
    m4uint64_t toBytes = 1024;

	// get the filename for this process in /proc dir.
    m4char_t szFilename [1024] ;
	sprintf ( szFilename , "/proc/%d/psinfo" , getpid() ) ;
	
    // open and read psinfo file.
    int fd = open (szFilename, O_RDONLY);
	if (!fd)
	{
		return M4_ERROR;
	}
	read (fd, (void*) &oInfo, sizeof(psinfo));
	close (fd);

	// calculate memory for this process.
    ao_cPrivateBytes = oInfo.pr_size;
	
	// Bg 0127680
	ao_cPrivateBytes &= 0x00000000FFFFFFFF;
	
    m4TraceLevel(2, cout << "oInfo.pr_size: " << oInfo.pr_size << endl);
    m4TraceLevel(2, cout << "oInfo.pr_rssize: " << oInfo.pr_rssize << endl);
	ao_cPrivateBytes *= toBytes;
    m4TraceLevel(2, cout << "M4GetProcessMem::ao_cPrivateBytes: " << ao_cPrivateBytes << endl);

	return M4_SUCCESS ;
}



m4return_t  M4GetSystemCPU ( m4double_t &ao_cpu) 
{
    u_longlong_t total = 0;

    // get cpu usage.
    cpu_total_t(0, total);
    
    // sleep for a while.
    sleep(1);

    // get cpu usage and calculate porcentages.
    cpu_total_t(1, total);

    // return;
    ao_cpu = total;
    m4TraceLevel(2, cout << "M4GetSystemCPU::ao_cpu: " << ao_cpu << endl);
	return M4_SUCCESS ; 
}


#if 0
main()
{

    m4uint64_t u0, u1, u2, u3;
	m4double_t d0;
    
    while(1)
    {
		//M4GetSystemCPU(d0);
        //M4GetSystemMem(u0, u1, u2, u3);
        M4GetProcessMem(u0);
        
		sleep(1);
    }
	
    return 0;
}

#endif