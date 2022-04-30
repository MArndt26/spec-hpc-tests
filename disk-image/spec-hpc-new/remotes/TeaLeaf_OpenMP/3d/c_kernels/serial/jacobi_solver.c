#include <stdlib.h>
#include "../../shared.h"

/*
 *		JACOBI SOLVER KERNEL
 */

// Initialises the Jacobi solver
void jacobi_solver_init(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int coefficient,
        double rx,
        double ry,
        double rz,
        double* density,
        double* energy,
        double* vec_u0,
        double* vec_u,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz)
{
    if(coefficient < CONDUCTIVITY && coefficient < RECIP_CONDUCTIVITY)
    {
        die(__LINE__, __FILE__, "Coefficient %d is not valid.\n", coefficient);
    }

    for(int ii = 1; ii < z-1; ++ii)
    {
        for(int jj = 1; jj < y-1; ++jj)
        {
            for(int kk = 1; kk < x-1; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                double temp = energy[index]*density[index];
                vec_u0[index] = temp;
                vec_u[index] = temp;
            }
        }
    }

    for(int ii = halo_depth; ii < z-1; ++ii)
    {
        for(int jj = halo_depth; jj < y-1; ++jj)
        {
            for(int kk = halo_depth; kk < x-1; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                double densityCentre = (coefficient == CONDUCTIVITY) 
                    ? density[index] : 1.0/density[index];
                double densityLeft = (coefficient == CONDUCTIVITY) 
                    ? density[index-1] : 1.0/density[index-1];
                double densityDown = (coefficient == CONDUCTIVITY) 
                    ? density[index-x] : 1.0/density[index-x];
                double densityBack = (coefficient == CONDUCTIVITY) 
                    ? density[index-x*y] : 1.0/density[index-x*y];

                vec_kx[index] = rx*(densityLeft+densityCentre)/(2.0*densityLeft*densityCentre);
                vec_ky[index] = ry*(densityDown+densityCentre)/(2.0*densityDown*densityCentre);
                vec_kz[index] = rz*(densityBack+densityCentre)/(2.0*densityBack*densityCentre);
            }
        }
    }
}

// The main Jacobi solve step
void jacobi_solver_iterate(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* error,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz,
        double* vec_u0,
        double* vec_u,
        double* vec_r)
{
    for(int ii = 0; ii < z; ++ii)
    {
        for(int jj = 0; jj < y; ++jj)
        {
            for(int kk = 0; kk < x; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                vec_r[index] = vec_u[index];	
            }
        }
    }

    double err=0.0;
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                vec_u[index] = (vec_u0[index] 
                        + (vec_kx[index+1]*vec_r[index+1] + vec_kx[index]*vec_r[index-1])
                        + (vec_ky[index+x]*vec_r[index+x] + vec_ky[index]*vec_r[index-x])
                        + (vec_kz[index+x*y]*vec_r[index+x*y] + vec_kz[index]*vec_r[index-x*y]))
                    / (1.0 + (vec_kx[index]+vec_kx[index+1])
                            + (vec_ky[index]+vec_ky[index+x])
                            + (vec_kz[index]+vec_kz[index+x*y]));

                err += fabs(vec_u[index]-vec_r[index]);
            }
        }
    }

    *error = err;
}

