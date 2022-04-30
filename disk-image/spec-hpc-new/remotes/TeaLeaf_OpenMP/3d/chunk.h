#pragma once

#include <math.h>
#include "settings.h"
#include "build/chunk_extension.h"

// The core Tealeaf interface class.
typedef struct Chunk
{
    // Solve-wide variables
    double dt_init;

    // Neighbouring ranks
    int* neighbours; 

    // MPI comm buffers
    double* left_send;
    double* left_recv;
    double* right_send;
    double* right_recv;
    double* top_send;
    double* top_recv;
    double* bottom_send;
    double* bottom_recv;
    double* front_send;
    double* front_recv;
    double* back_send;
    double* back_recv;

    // MPI comm buffer page sizes
    int lr_page;
    int tb_page;
    int fb_page;
    
    // Mesh chunks
    int left;
    int right;
    int bottom;
    int top;
    int back;
    int front;

    // Field dimensions
    int x;
    int y;
    int z;

    // Field buffers
    double* density0;
    double* density;
    double* energy0;
    double* energy;

    double* vec_u;
    double* vec_u0;
    double* vec_p;
    double* vec_r;
    double* vec_mi;
    double* vec_w;
    double* vec_kx;
    double* vec_ky;
    double* vec_kz;
    double* vec_sd;
    double* vec_z;

    double* cell_x;
    double* cell_y;
    double* cell_z;
    double* cell_dx;
    double* cell_dy;
    double* cell_dz;

    double* vertex_dx;
    double* vertex_dy;
    double* vertex_dz;
    double* vertex_x;
    double* vertex_y;
    double* vertex_z;

    double* volume;
    double* x_area;
    double* y_area;
    double* z_area;

    // Cheby and PPCG  
    double theta;
    double eigmin;
    double eigmax;

    double* cg_alphas;
    double* cg_betas;
    double* cheby_alphas;
    double* cheby_betas;

    // Extension by composition for implementations
    struct chunk_extension_t* ext;
} Chunk;

struct Settings;

void initialise_chunk(Chunk* chunk, struct Settings* settings, int x, int y, int z);
void finalise_chunk(Chunk* chunk);

