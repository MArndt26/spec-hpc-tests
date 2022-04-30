#include <stdlib.h>
#include "../../shared.h"

/*
 *		SHARED SOLVER METHODS
 */

// Copies the current u into u0
void solver_copy_u(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* vec_u0,
        double* vec_u)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int index = ii*x*y+jj*x+kk;
                vec_u0[index] = vec_u[index];	
            }
        }
    }
}

// Calculates the current value of r
void calculate_residual(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* vec_u,
        double* vec_u0,
        double* vec_r,
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
                int index = ii*x*y+jj*x+kk;
                const double smvp = SMVP(vec_u);
                vec_r[index] = vec_u0[index] - smvp;
            }
        }
    }
}

// Calculates the 2 norm of a given buffer
void calculate_2norm(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* buffer,
        double* norm)
{
    double norm_temp = 0.0;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int index = ii*x*y+jj*x+kk;
                norm_temp += buffer[index]*buffer[index];			
            }
        }
    }

    *norm += norm_temp;
}

// Finalises the solution
void solver_finalise(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* energy,
        double* density,
        double* u)
{
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int index = ii*x*y+jj*x+kk;
                energy[index] = u[index]/density[index];
            }
        }
    }
}
