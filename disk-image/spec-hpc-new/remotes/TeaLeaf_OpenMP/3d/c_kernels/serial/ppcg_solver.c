#include "../../shared.h"

/*
 *		PPCG SOLVER KERNEL
 */

// Initialises the PPCG solver
void ppcg_init(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double theta,
        double* vec_r,
        double* vec_sd)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                vec_sd[index] = vec_r[index] / theta;
            }
        }
    }
}

// The PPCG inner iteration
void ppcg_inner_iteration(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double alpha,
        double beta,
        double* vec_u,
        double* vec_r,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz,
        double* vec_sd)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                const double smvp = SMVP(vec_sd);
                vec_r[index] -= smvp;
                vec_u[index] += vec_sd[index];
            }
        }
    }

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                vec_sd[index] = alpha*vec_sd[index] + beta*vec_r[index];
            }
        }
    }
}

