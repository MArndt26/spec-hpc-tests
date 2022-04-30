#pragma once 

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

/*
 *		PROFILING TOOL
 *		Not thread safe.
 */

#define PROFILER_MAX_NAME 128
#define PROFILER_MAX_ENTRIES 2048

struct profile_entry
{
    int calls;
    double time;
    char name[PROFILER_MAX_NAME];
};

struct profile
{
#ifdef __APPLE__
    uint64_t profiler_start;
    uint64_t profiler_end;
#else
    struct timespec profiler_start;
    struct timespec profiler_end;
#endif

    int profiler_entry_count;
    struct profile_entry profiler_entries[PROFILER_MAX_ENTRIES];
};

void profiler_initialise();
void profiler_finalise();

void profiler_start_timer(struct profile* p);
void profiler_end_timer(struct profile* p, const char* entry_name);
void profiler_print_simple_profile(struct profile* p);
void profiler_print_full_profile(struct profile* p);
int profiler_get_profile_entry(struct profile* p, const char* entry_name);

// Allows compile-time optimised conditional profiling
#ifdef ENABLE_PROFILING

    #define START_PROFILING(profile) \
        profiler_start_timer(profile)

    #define STOP_PROFILING(profile, name) \
        profiler_end_timer(profile, name)

    #define PRINT_PROFILING_RESULTS(profile) \
        profiler_print_full_profile(profile)

#else

    #define START_PROFILING(profile) \
        ;

    #define STOP_PROFILING(profile, name) \
        ;   

    #define PRINT_PROFILING_RESULTS(profile) \
        ;

#endif

