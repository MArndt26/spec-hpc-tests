#include "../../shared.h"
#include <stdlib.h>

void allocate_buffer(double** a, int x, int y, int z);
void prepare_7pt_stencil_csr_rows(
        int x, int y, int z, int halo_depth, int* a_row_index);

// Allocates all of the field buffers
void kernel_initialise(
        Settings* settings, int x, int y, int z, double** density0, 
        double** density, double** energy0, double** energy, double** vec_u, 
        double** vec_u0, double** vec_p, double** vec_r, double** vec_mi, 
        double** vec_w, double** vec_kx, double** vec_ky, double** vec_kz, 
        double** vec_sd, double** vec_z, double** volume, double** x_area, 
        double** y_area, double** z_area, double** cell_x, double** cell_y, 
        double** cell_z, double** cell_dx, double** cell_dy, double** cell_dz, 
        double** vertex_dx, double** vertex_dy, double** vertex_dz, 
        double** vertex_x, double** vertex_y, double** vertex_z,
        double** cg_alphas, double** cg_betas, double** cheby_alphas,
        double** cheby_betas, int** a_row_index, int** a_col_index,
        double** a_non_zeros)
{
    allocate_buffer(density0, x, y, z);
    allocate_buffer(density, x, y, z);
    allocate_buffer(energy0, x, y, z);
    allocate_buffer(energy, x, y, z);
    allocate_buffer(vec_u, x, y, z);
    allocate_buffer(vec_u0, x, y, z);
    allocate_buffer(vec_p, x, y, z);
    allocate_buffer(vec_r, x, y, z);
    allocate_buffer(vec_mi, x, y, z);
    allocate_buffer(vec_w, x, y, z);
    allocate_buffer(vec_kx, x, y, z);
    allocate_buffer(vec_ky, x, y, z);
    allocate_buffer(vec_kz, x, y, z);
    allocate_buffer(vec_sd, x, y, z);
    allocate_buffer(vec_z, x, y, z);
    allocate_buffer(volume, x, y, z);
    allocate_buffer(x_area, x+1, y, z);
    allocate_buffer(y_area, x, y+1, z);
    allocate_buffer(z_area, x, y, z+1);
    allocate_buffer(cell_x, x, 1, 1);
    allocate_buffer(cell_y, 1, y, 1);
    allocate_buffer(cell_z, 1, 1, z);
    allocate_buffer(cell_dx, x, 1, 1);
    allocate_buffer(cell_dy, 1, y, 1);
    allocate_buffer(cell_dz, 1, 1, z);
    allocate_buffer(vertex_dx, x+1, 1, 1);
    allocate_buffer(vertex_dy, 1, y+1, 1);
    allocate_buffer(vertex_dz, 1, 1, z+1);
    allocate_buffer(vertex_x, x+1, 1, 1);
    allocate_buffer(vertex_y, 1, y+1, 1);
    allocate_buffer(vertex_z, 1, 1, z+1);
    allocate_buffer(cg_alphas, settings->max_iters, 1, 1);
    allocate_buffer(cg_betas, settings->max_iters, 1, 1);
    allocate_buffer(cheby_alphas, settings->max_iters, 1, 1);
    allocate_buffer(cheby_betas, settings->max_iters, 1, 1);

    // Zero-indexed three-array CSR variant
    *a_row_index = (int*)malloc(sizeof(int)*(x*y*z+1));

    prepare_7pt_stencil_csr_rows(x, y, z, settings->halo_depth, *a_row_index);

    int num_non_zeros = (*a_row_index)[x*y*z];
    *a_col_index = (int*)malloc(sizeof(int)*num_non_zeros);
    *a_non_zeros = (double*)malloc(sizeof(double)*num_non_zeros);
}

// Allocates, and zeroes and individual buffer
void allocate_buffer(double** a, int x, int y, int z)
{
    *a = (double*)malloc(sizeof(double)*x*y*z);

    if(*a == NULL) 
    {
        die(__LINE__, __FILE__, "Error allocating buffer %s\n");
    }

#pragma omp parallel for
    for(int ii = 0; ii < z; ++ii)
    {
        for(int jj = 0; jj < y; ++jj)
        {
            for(int kk = 0; kk < x; ++kk)
            {
                const int ind = ii*x*y+jj*x+kk;
                (*a)[ind] = 0.0;
            }
        }
    }
}

// Initialises the row index (CSR) for a 7pt stencil
void prepare_7pt_stencil_csr_rows(
        int x, int y, int z, int halo_depth, int* a_row_index)
{
    // Necessarily serialised row index calculation
    a_row_index[0] = 0;

    for(int ii = 0; ii < z; ++ii)
    {
        for(int jj = 0; jj < y; ++jj)
        {
            for(int kk = 0; kk < x; ++kk)
            {
                int index = kk + jj*x + ii*x*y;

                // Calculate position dependent row count
                int row_count = 
                    1 + (ii >= halo_depth) + (jj >= halo_depth) + 
                    (kk >= halo_depth) + (ii < (z-halo_depth)) + 
                    (jj < (y-halo_depth)) + (kk < (x-halo_depth));
                a_row_index[index+1] = a_row_index[index] + row_count;
            }
        }
    }
}

