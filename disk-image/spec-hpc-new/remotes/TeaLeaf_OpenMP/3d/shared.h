#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "chunk.h"
#include "profiler.h"

struct Chunk;
struct Settings;

// Shared function declarations
void initialise_log(struct Settings* settings);
void print_to_log(struct Settings* settings, const char* format, ...);
void print_and_log(struct Settings* settings, const char* format, ...);
void plot_3d(struct Chunk* chunk, double* buffer, const char* name);
void die(int lineNum, const char* file, const char* format, ...);

// Global constants
#define MASTER 0

#define NUM_FACES 6
#define CHUNK_LEFT 0
#define CHUNK_RIGHT 1
#define CHUNK_BOTTOM 2
#define CHUNK_TOP 3
#define CHUNK_BACK 4
#define CHUNK_FRONT 5
#define EXTERNAL_FACE -1

#define FIELD_DENSITY 0
#define FIELD_ENERGY0 1
#define FIELD_ENERGY1 2
#define FIELD_U 3
#define FIELD_P 4
#define FIELD_SD 5

#define CONDUCTIVITY 1
#define RECIP_CONDUCTIVITY 2

#define CG_ITERS_FOR_EIGENVALUES 20
#define ERROR_SWITCH_MAX 1.0

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)
#define strmatch(a, b) (strcmp(a, b) == 0)
#define sign(a,b) ((b)<0 ? -fabs(a) : fabs(a))

// Sparse Matrix Vector Product
#define SMVP(a) \
    (1.0 + (vec_kx[index+1]+vec_kx[index])\
     + (vec_ky[index+x]+vec_ky[index])\
     + (vec_kz[index+x*y]+vec_kz[index]))*a[index]\
     - (vec_kx[index+1]*a[index+1]+vec_kx[index]*a[index-1])\
     - (vec_ky[index+x]*a[index+x]+vec_ky[index]*a[index-x])\
     - (vec_kz[index+x*y]*a[index+x*y]+vec_kz[index]*a[index-x*y]);

#define GET_ARRAY_VALUE(len, buffer) \
    temp = 0.0;\
    for(int ii = 0; ii < len; ++ii) {\
        temp += buffer[ii];\
    }\
    printf("%s = %.12E\n", #buffer, temp);

    
