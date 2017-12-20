


#include <m4meminfo.hpp>
#include <m4types.hpp>
#include <m4date.hpp>

#include <m4types.hpp>
#include <procfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/swap.h>
#include <stdlib.h>
#include <rpcsvc/rstat.h>
#include <m4date.hpp>

#include <sys/sysinfo.h>
#include <sys/cpuvar.h>
#include <kvm.h>
#include <nlist.h>
#include <kstat.h>

#define MAXSTRSIZE	80
#define CPUSTATES	5
#define CPUSTATE_IOWAIT 3
#define CPUSTATE_SWAP	4


/*
 *  percentages(cnt, out, new, old, diffs) - calculate percentage change
 *	between array "old" and "new", putting the percentages i "out".
 *	"cnt" is size of each array and "diffs" is used for scratch space.
 *	The array "old" is updated on each call.
 *	The routine assumes modulo arithmetic.  This function is especially
 *	useful on BSD mchines for calculating cpu state percentages.
 */

m4int32_t percentages(m4int32_t cnt, m4int32_t * pOut, m4int32_t * pNew, m4int32_t * pOld, m4int32_t * pDiffs)

{
    int i;
    m4int32_t change;
    m4int32_t total_change;
    m4int32_t *dp;
    m4int32_t half_total;

    /* initialization */
    total_change = 0;
    dp = pDiffs;

    /* calculate changes for each state and the overall change */
    for (i = 0; i < cnt; i++)
    {
		if ((change = *pNew - *pOld) < 0)
		{
			/* this only happens when the counter wraps */
			change = (int)
			((unsigned long)*pNew-(unsigned long)*pOld);
		}
	total_change += (*dp++ = change);
	*pOld++ = *pNew++;
    }

    /* avoid divide by zero potential */
    if (total_change == 0)
    {
	total_change = 1;
    }

    /* calculate percentages based on overall change, rounding up */
    half_total = total_change / 2l;
    for (i = 0; i < cnt; i++)
    {
	*pOut++ = (int)((*pDiffs++ * 1000 + half_total) / total_change);
    }

    /* return the total in case the caller wants to use it */
    return(total_change);
};


m4int32_t Update (char *& aio_cpu_stat, int &ao_cpu_stat_data_size, kstat_t **&aio_cpu_ks)
{
	static m4int32_t ncpus = 0 ;
    static int ncpu = 0;
	kstat_named_t * kn ;
	kstat_t * ks ;
	static kstat_ctl_t * kc = NULL ;
    kid_t nkcid;
    static kid_t kcid = 0;
    int changed = 0;
    int i;
	int ncpus_was_updated = 0;


	if (! kc )
	{
		if (! ( kc = kstat_open () ) )
		{
			return M4_ERROR ;
		}
		changed = 1;
	}

    nkcid = kstat_chain_update(kc);
    if (nkcid)
    {
		/* UPDKCID will abort if nkcid is -1, so no need to check */
		changed = 1;
		kcid = nkcid;
    }
    
	if ( nkcid== -1)
		return M4_ERROR ;

	
	ks = kstat_lookup ( kc , "unix" , 0 , "system_misc" ) ;

	if (  kstat_read (kc, ks , 0 ) == -1 )
	{
		return M4_ERROR ;
	}

    if (changed) 
	{
		ncpu = 0;

		kn = (kstat_named_t *) kstat_data_lookup(ks, "ncpus");
		if (kn)
		{
			if ( (kn->value.ui32) > ncpus) 
			{
				ncpus = kn->value.ui32;
				ncpus_was_updated = 1;
				if ( aio_cpu_ks == NULL )
				{
					aio_cpu_ks = (kstat_t **) malloc ( ncpus * sizeof (kstat_t *) ) ;
				}
				else
				{
					aio_cpu_ks = (kstat_t **) realloc (aio_cpu_ks, ncpus * sizeof (kstat_t *));
				}
			}
		}

		for (ks = kc->kc_chain; ks; ks = ks->ks_next)
		{
			if (strncmp(ks->ks_name, "cpu_stat", 8) == 0)
		    {
				nkcid = kstat_read(kc, ks, NULL);
				/* if kcid changed, pointer might be invalid */
				if ( nkcid== -1 )
					return M4_ERROR ;

				aio_cpu_ks[ncpu] = ks;
				ncpu++;
				if (ncpu > ncpus)
				{
					fprintf(stderr, "kstat finds too many cpus: should be %d\n",ncpus);
					return M4_ERROR ;
				}
			}
		}

		/* note that ncpu could be less than ncpus, but that's okay */
		changed = 0;
    }

    /*
     * 3. get data
     */
    for (i = 0; i < ncpu; i++)
    {
		if (ncpus_was_updated)
		{
			ncpus_was_updated = 0;
			ao_cpu_stat_data_size = aio_cpu_ks[i]->ks_data_size;
			if ( aio_cpu_stat == NULL )
			{
				aio_cpu_stat = (char*)malloc  ( ncpus * ao_cpu_stat_data_size ) ;
			}
			else
			{
				aio_cpu_stat = (char*)realloc (aio_cpu_stat, ncpus * ao_cpu_stat_data_size);
			}
		}

		char *cpu_stat = aio_cpu_stat + (i*ao_cpu_stat_data_size);
		nkcid = kstat_read(kc, aio_cpu_ks[i], cpu_stat);

		/* if kcid changed, pointer might be invalid */
		if (nkcid==-1)
			return  M4_ERROR ;
    }

    /* return the number of cpus found */
    return(ncpu);
}



m4return_t  M4GetSystemCPU ( m4double_t &ao_cpu) 
{
  
	static kstat_t **cpu_ks = NULL;
	static m4bool_t bInit = M4_FALSE ;
	
	static char *cpu_stat_array = NULL;
	static cpu_stat_data_size = 0;
	cpu_stat_t *cpu_stat = NULL;

	m4int32_t cpu_states [CPUSTATES] ;
	static struct nlist nlst [] = {
		{"v"},
		{"mpid"}, 
		{"k_anoninfo"},
		{"cpu"},
		{0}
	};
	kvm_t * kd ;
	static m4int32_t cp_time [ CPUSTATES] ;
	static m4int32_t cp_old [ CPUSTATES] ;
	static m4int32_t cp_diff [ CPUSTATES] ;
	m4uint32_t i,j ;
	static m4uint32_t ncpus ;

	if ( ! bInit )
	{
		kd = kvm_open  (NULL , NULL , NULL , O_RDONLY , NULL);
		if ( kd )
		{
			if ( kvm_nlist ( kd , nlst ) > 0 )
			{
				return M4_ERROR ;
			}
		}

		bInit = M4_TRUE ;
	}

	//inicializar el array
	for ( j = 0 ; j < CPUSTATES ; j ++ )
		cp_time [j] = 0L ;

	// ncpus = Update (cpu_stat , cpu_ks ) ;
	ncpus = Update (cpu_stat_array, cpu_stat_data_size, cpu_ks);

	for ( i = 0 ; i< ncpus ; i ++ )
	{
		cpu_stat = (cpu_stat_t*)((char*)cpu_stat_array + (i*cpu_stat_data_size));
		for ( j = 0 ; j < CPU_WAIT ; j++ )
		{
			cp_time [j] += (m4int32_t) cpu_stat->cpu_sysinfo.cpu[j] ;
		}

		cp_time[CPUSTATE_IOWAIT] += (m4int32_t) cpu_stat->cpu_sysinfo.wait[W_IO] +
									(m4int32_t) cpu_stat->cpu_sysinfo.wait[W_PIO] ;

		cp_time[CPUSTATE_SWAP] += (m4int32_t) cpu_stat->cpu_sysinfo.wait[W_SWAP];
	}

	percentages ( CPUSTATES , cpu_states , cp_time , cp_old , cp_diff ) ;
	ao_cpu = 1000 - cpu_states [0] ;
	ao_cpu /= 10 ;

	return M4_SUCCESS ;
}


m4int32_t  M4GetProcessMem ( m4uint64_t &ao_cPrivateBytes )
{
	m4char_t szFilename [1024] ;
	psinfo_t oInfo ;
	m4int_t fd ;


	sprintf ( szFilename , "/proc/%d/psinfo" , getpid() ) ;

	fd = open ( szFilename , O_RDONLY ) ;

	if ( ! fd )
	{
		return M4_ERROR ;
	}

	read ( fd , (void*) &oInfo , sizeof ( psinfo ) ) ;

	close (fd ) ;

	ao_cPrivateBytes = oInfo.pr_size ;

	ao_cPrivateBytes *= 1024 ;


	return M4_SUCCESS ;
}

m4int32_t  M4GetSystemMem (  m4uint64_t & ao_lAvailPhysMem , m4uint64_t & ao_lTotalPhysMem , m4uint64_t & ao_lAvailSwapMem , m4uint64_t & ao_lTotalSwapMem ) 
{
	m4uint32_t pagesize = 0 ;
	m4uint32_t swap_pages = 0;
	m4uint32_t swap_pages_free = 0;
	swaptbl_t * s = NULL;
	m4int_t num, old_num = 0;
	m4int_t n = 0;
	m4int_t i;

	// Fix Bug 0073099
	m4uint64_t lTotalPhysMem = 0,
		lAvailPhysMem = 0,
		lTotalVirtualMem = 0 ,
		lAvailVirtualMem = 0 ;



	pagesize = sysconf ( _SC_PAGESIZE ) ;
	lTotalPhysMem = sysconf ( _SC_PHYS_PAGES) * m4uint64_t(pagesize) ;
	lAvailPhysMem = sysconf ( _SC_AVPHYS_PAGES ) * m4uint64_t(pagesize) ;

	char pBuffer[MAXSTRSIZE];
					
	do{

		if ( M4_ERROR == ( num = swapctl ( SC_GETNSWP , 0) ) )
		{
			return M4_ERROR ;
		}

		if ( num > 0)
		{
			// checks for alrealy allocated memory
			// fix bug 0090983
			if (num > old_num) {
				
				// reset flags
				old_num = num;
				
				if (s != NULL) {
					free (s);
				}
			
				// allocates new chunks
				if ( (s = (swaptbl_t*) malloc ( sizeof (int) + num * sizeof ( swapent_t ) ) ) == NULL )
				{
					return M4_ERROR ;
				}
	
				// initializes references
				for ( i = 0 ; i < num; i ++)
				{
					s ->swt_ent[i].ste_path = pBuffer; 
				}
				
				s-> swt_n = num;
			}
		
			if ( ( n = swapctl (SC_LIST , s) ) < 0 )
			{
				free(s);
				return M4_ERROR ;
			}
		}

	}while ( n > num);

	for ( i = 0 ; i< n ;i++ )
	{

		/* ignore slots being deleted */
		if ((s->swt_ent[i].ste_flags & ST_INDEL) || (s->swt_ent[i].ste_flags & ST_DOINGDEL))
		{
			continue;
		}

		swap_pages += s->swt_ent[i].ste_pages ;

		swap_pages_free += s->swt_ent[i].ste_free ;
	}
	
	if (s != NULL) {
		free (s);
	}

	lTotalVirtualMem = swap_pages * m4uint64_t(pagesize) ;
	lAvailVirtualMem = swap_pages_free * m4uint64_t(pagesize) ;

	ao_lAvailPhysMem = lAvailPhysMem ;
	ao_lTotalPhysMem = lTotalPhysMem ;
	ao_lAvailSwapMem = lAvailVirtualMem ;
	ao_lTotalSwapMem = lTotalVirtualMem ;

	return M4_SUCCESS ;
}


