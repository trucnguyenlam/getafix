/* =========================================================================

   Definition allgemeiner Funktionen

   ====================================================================== */


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "common.h"

#define MAX_RESOURCE_MARKS 5

#ifdef WIN32
  #include <sys/timeb.h>
  time_t common_time_offset;
#else
  #include <sys/times.h>
#endif
#include <limits.h>

int mc_stats=1;

/* -------------------------  memory - Functions ------------------------ */

#ifdef MEMORY_USAGE

long memory_current   = 0;
long memory_threshold = 0;
long mark_memory_max[MAX_RESOURCE_MARKS];

void *common_malloc (size_t bytes)
{
	int i;
	void *ret = malloc(bytes);

	if (!ret) common_error("cannot allocate heap-space");

	memory_current += bytes;

	for (i=0; i<MAX_RESOURCE_MARKS; i++)
		if (memory_current > mark_memory_max[i])
			mark_memory_max[i] = memory_current;

	if (memory_current > memory_threshold)
	{
		if (memory_threshold > 0) printf("[>%li]\n", memory_threshold);
		memory_threshold += 1000000;
	}      

	return ret;
}

void common_free (void *ptr, size_t bytes)
{
	memory_current -= bytes;
	free(ptr);
}

char* common_strdup(char *s)
{
	memory_current += strlen(s)+1;
	return strdup(s);
}

void* common_realloc(void *ptr, size_t new_size, size_t diff)
{
	memory_current += diff;
	return realloc(ptr,new_size);
}

void common_check_leak()
{
	if (memory_current > 0)
		printf("\n\n=====> WARNING: Memory leak detected "
			"(%li bytes leaked) ! <=====\n\n",memory_current);
	else if (memory_current < 0)
		printf("\n\n=====> WARNING: internal error "
			"(memory_current = %li) ! <=====\n\n", memory_current);
}

#endif


/* -------------------------  time - Functions ------------------------ */

float common_time_getseconds()
{
#ifdef WIN32
	struct _timeb tbstruct;
	_ftime(&tbstruct);
	return ((float) difftime(tbstruct.time,common_time_offset))
		+ (float) (tbstruct.millitm/1000.0);
#else
	struct tms time_current;
	times(&time_current);
  #ifdef __CYGWIN__
	return (float) time_current.tms_utime * CLK_TCK / 1000000.0;
  #else
	/* return (float) time_current.tms_utime * CLK_TCK / 10000.0; */
  #endif
#endif
}

/* -------------------------  resource - Functions ------------------------ */


#ifdef MEMORY_USAGE
long mark_memory_start[MAX_RESOURCE_MARKS];
long limit_memory = -1;
#endif

float mark_time[MAX_RESOURCE_MARKS];
float limit_time = -1;

void common_resource_init()
{
	int i;

#ifdef WIN32
	time(&common_time_offset);
#endif
	for (i = 0; i < MAX_RESOURCE_MARKS; i++)
	{
#ifdef MEMORY_USAGE
		mark_memory_start[i] = -1;
		mark_memory_max[i] = -1;
#endif
		mark_time[i]       = -1;
	}

	common_strbuf_init();
	common_resource_setmark(0);
}


void common_resource_setmark (int res_mark_nr)
{
	assert((res_mark_nr >= 0) && (res_mark_nr < MAX_RESOURCE_MARKS));
  
#ifdef MEMORY_USAGE
	mark_memory_start[res_mark_nr] = memory_current;
	mark_memory_max[res_mark_nr]   = memory_current;
	memory_threshold = ((int) (memory_current/1000000)) * 1000000;
#endif
	mark_time[res_mark_nr] = common_time_getseconds();
}


void common_resource_displaymark (int res_mark_nr, char *comment, float *tp)
{
	float time;

	if (!mc_stats) return;
	assert((res_mark_nr >= 0) && (res_mark_nr < MAX_RESOURCE_MARKS));

	time = common_time_getseconds() - mark_time[res_mark_nr];
	printf("Statistics for common resources (%s):\n", comment);
	printf("  - Time   : %.2f seconds\n", time); 
	mark_time[res_mark_nr] = -1;
	if (tp) *tp = time;

#ifdef MEMORY_USAGE
	{
	long  memory;
	memory = mark_memory_max[res_mark_nr] - mark_memory_start[res_mark_nr];
	printf( "  - Memory : %lu bytes\n\n", memory );
	mark_memory_start[res_mark_nr] = -1;
	mark_memory_max[res_mark_nr] = -1;
	}
#endif

}


void common_error(char* msg, ...)
{
	va_list args;
	va_start(args,msg);
	fprintf(stderr,"error: ");
	vfprintf(stderr,msg,args);
	fprintf(stderr,"\n");
	va_end(args);
	exit(1);
}


/****************************************************************************/
/* string buffer functions 						    */

char *common_strbuf;
int common_strbuf_alloc;

void common_strbuf_init()
{
	common_strbuf = malloc(256);
	common_strbuf_alloc = 256;
}

void common_strbuf_new()
{
	*common_strbuf = 0;
}

void common_addbuf(char *s, ...)
{
        int len = strlen(common_strbuf);
        va_list args;

        if (len + 128 >= common_strbuf_alloc)
        {
                common_strbuf_alloc += 256;
                common_strbuf = realloc(common_strbuf,common_strbuf_alloc);
        }

        va_start(args,s);
        vsprintf(common_strbuf+len,s,args);
        va_end(args);
}
