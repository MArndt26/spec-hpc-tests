#include "shared.h"
#include "comms.h"

// Initialises the log file pointer
void initialise_log(
        struct Settings* settings)
{
    // Only write to log in master rank
    if(settings->rank != MASTER)
    {
        return;
    }

    printf("Opening %s as log file.\n", settings->tea_out_filename);
    settings->tea_out_fp = fopen(settings->tea_out_filename, "w");

    if(!settings->tea_out_fp)
    {
        die(__LINE__, __FILE__,
                "Could not open log %s\n", settings->tea_out_filename);
    }
}

// Prints to stdout and then logs message in log file
void print_and_log(
        struct Settings* settings, const char* format, ...)
{
    // Only master rank should print
    if(settings->rank != MASTER)
    {
        return;
    }

    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);

    if(!settings->tea_out_fp)
    {
        die(__LINE__, __FILE__, 
                "Attempted to write to log before it was initialised\n");
    }

    // Obtuse, but necessary
    va_list arglist2;
    va_start(arglist2, format);
    vfprintf(settings->tea_out_fp, format, arglist2);
    va_end(arglist2);
}

// Logs message in log file
void print_to_log(
        struct Settings* settings, const char* format, ...)
{
    // Only master rank should log
    if(settings->rank != MASTER)
    {
        return;
    }

    if(!settings->tea_out_fp)
    {
        die(__LINE__, __FILE__, 
                "Attempted to write to log before it was initialised\n");
    }

    va_list arglist;
    va_start(arglist, format);
    vfprintf(settings->tea_out_fp, format, arglist);
    va_end(arglist);
}

// Plots a three-dimensional dat file.
void plot_3d(Chunk* chunk, double* buffer, const char* name)
{    
    // Open the plot file
    FILE* fp = fopen("plot3d.dat", "wb");
    if(!fp) { printf("Could not open plot file.\n"); }

    double b_sum = 0.0;

    for(int ii = 0; ii < chunk->z; ++ii)
    {
        for(int jj = 0; jj < chunk->y; ++jj)
        {
            for(int kk = 0; kk < chunk->x; ++kk)
            {
                double val = buffer[kk+jj*chunk->x+ii*chunk->x*chunk->y];
                fprintf(fp, "%d %d %d %.12E\n", kk, jj, ii, val);
                b_sum+=val;
            }
        }
    }

    printf("%s: %.12E\n", name, b_sum);
    fclose(fp);
}

// Aborts the application.
void die(int lineNum, const char* file, const char* format, ...)
{
    // Print location of error
    printf("\x1b[31m");
    printf("\nError at line %d in %s:", lineNum, file);
    printf("\x1b[0m \n");

    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);

    abort_comms();
}

