#include "../../shared.h"

/*
 *		CHEBYSHEV SOLVER KERNEL
 */

// Calculates the new value for u.
void cheby_calc_u(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* vec_u,
        double* vec_p)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {	
                const int index = ii*x*y+jj*x+kk;
                vec_u[index] += vec_p[index];
            }
        }
    }
}

// Initialises the Chebyshev solver
void cheby_solver_init(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const double theta,
        double* vec_u,
        double* vec_u0,
        double* vec_p,
        double* vec_r,
        double* vec_w,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                const double smvp = SMVP(vec_u);
                vec_w[index] = smvp;
                vec_r[index] = vec_u0[index]-vec_w[index];
                vec_p[index] = vec_r[index] / theta;
            }
        }
    }

    cheby_calc_u(x, y, z, halo_depth, vec_u, vec_p);
}

// The main chebyshev iteration
void cheby_solver_iterate(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double alpha,
        double beta,
        double* vec_u,
        double* vec_u0,
        double* vec_p,
        double* vec_r,
        double* vec_w,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {	
                const int index = ii*x*y+jj*x+kk;
                const double smvp = SMVP(vec_u);
                vec_w[index] = smvp;
                vec_r[index] = vec_u0[index]-vec_w[index];
                vec_p[index] = alpha*vec_p[index] + beta*vec_r[index];
            }
        }
    }

    cheby_calc_u(x, y, z, halo_depth, vec_u, vec_p);
}

