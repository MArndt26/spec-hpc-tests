#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "profiler.h"

#define strmatch(a, b) (strcmp(a, b) == 0)

// Internally start the profiling timer
void profiler_start_timer(struct profile* p)
{
#ifdef __APPLE__
    p->profiler_start = mach_absolute_time();
#else
    clock_gettime(CLOCK_MONOTONIC, &p->profiler_start);
#endif
}

// Internally end the profiling timer and store results
void profiler_end_timer(struct profile* p, const char* entry_name)
{
#ifdef __APPLE__
    p->profiler_end = mach_absolute_time();
#else
    clock_gettime(CLOCK_MONOTONIC, &p->profiler_end);
#endif

    // Check if an entry exists
    int ii;
    for(ii = 0; ii < p->profiler_entry_count; ++ii)
    {
        if(strmatch(p->profiler_entries[ii].name, entry_name))
        {
            break;
        }
    }

    // Don't overrun
    if(ii >= PROFILER_MAX_ENTRIES)
    {
        printf("Attempted to profile too many entries, maximum is %d\n",
                PROFILER_MAX_ENTRIES);
        exit(1);
    }

    // Create new entry
    if(ii == p->profiler_entry_count)
    {
        p->profiler_entry_count++;
        strcpy(p->profiler_entries[ii].name, entry_name);
    }

    // Update number of calls and time
#ifdef __APPLE__
    double elapsed = (p->profiler_end-p->profiler_start)*1.0E-9;
#else
    double elapsed = 
        (p->profiler_end.tv_sec - p->profiler_start.tv_sec) + 
        (p->profiler_end.tv_nsec - p->profiler_start.tv_nsec)*1.0E-9;
#endif

    p->profiler_entries[ii].time += elapsed; 
    p->profiler_entries[ii].calls++;
}

// Print the profiling results to output
void profiler_print_full_profile(struct profile* p)
{
    printf("\n-------------------------------------------------------------\n");
    printf("\nProfiling Results:\n\n");
    printf("%-30s%8s%20s\n", "Kernel Name", "Calls", "Runtime (s)");

    double total_elapsed_time = 0.0;
    for(int ii = 0; ii < p->profiler_entry_count; ++ii)
    {
        total_elapsed_time += p->profiler_entries[ii].time;
        printf("%-30s%8d%20.03F\n", p->profiler_entries[ii].name, 
                p->profiler_entries[ii].calls, 
                p->profiler_entries[ii].time);
    }

    printf("\nTotal elapsed time: %.03Fs, entries * are excluded.\n", total_elapsed_time);
    printf("\n-------------------------------------------------------------\n\n");
}

// Prints profile without extra details
void profiler_print_simple_profile(struct profile* p)
{
    for(int ii = 0; ii < p->profiler_entry_count; ++ii)
    {
        printf("\033[1m\033[30m%s\033[0m: %.3lfs (%d calls)\n", 
                p->profiler_entries[ii].name, 
                p->profiler_entries[ii].time,
                p->profiler_entries[ii].calls);
    }
}

// Gets an individual profile entry
int profiler_get_profile_entry(struct profile* p, const char* entry_name)
{
    for(int ii = 0; ii < p->profiler_entry_count; ++ii)
    {
        if(strmatch(p->profiler_entries[ii].name, entry_name))
        {
            return ii;
        }
    }

    printf("Attempted to retrieve missing profile entry %s\n",
            entry_name);
    exit(1);
}

