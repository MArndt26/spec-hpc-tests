#include <stdlib.h>
#include "../../shared.h"

/*
 *		CONJUGATE GRADIENT SOLVER KERNEL
 */

// Initialises the CG solver
void cg_solver_init(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int coefficient,
        double rx,
        double ry,
        double rz,
        double* rro,
        double* density,
        double* energy,
        double* vec_u,
        double* vec_p,
        double* vec_r,
        double* vec_w,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz)
{
    if(coefficient != CONDUCTIVITY && coefficient != RECIP_CONDUCTIVITY)
    {
        die(__LINE__, __FILE__, "Coefficient %d is not valid.\n", coefficient);
    }

#pragma omp target teams distribute
    for(int ii = 0; ii < z; ++ii)
    {
        for(int jj = 0; jj < y; ++jj)
        {
            for(int kk = 0; kk < x; ++kk)
            {
                const int index = ii*y*x+jj*x+kk;
                vec_p[index] = 0.0;
                vec_r[index] = 0.0;
                vec_u[index] = energy[index]*density[index];
            }
        }
    }

#pragma omp target teams distribute
    for(int ii = 1; ii < z-1; ++ii)
    {
        for(int jj = 1; jj < y-1; ++jj)
        {
            for(int kk = 1; kk < x-1; ++kk)
            {
                const int index = ii*y*x+jj*x+kk;
                vec_w[index] = (coefficient == CONDUCTIVITY) 
                    ? density[index] : 1.0/density[index];
            }
        }
    }

#pragma omp target teams distribute
    for(int ii = halo_depth; ii < z-1; ++ii)
    {
        for(int jj = halo_depth; jj < y-1; ++jj)
        {
            for(int kk = halo_depth; kk < x-1; ++kk)
            {
                const int index = ii*y*x+jj*x+kk;
                vec_kx[index] = rx*(vec_w[index-1]+vec_w[index]) /
                    (2.0*vec_w[index-1]*vec_w[index]);
                vec_ky[index] = ry*(vec_w[index-x]+vec_w[index]) /
                    (2.0*vec_w[index-x]*vec_w[index]);
                vec_kz[index] = rz*(vec_w[index-x*y]+vec_w[index]) /
                    (2.0*vec_w[index-x*y]*vec_w[index]);
            }
        }
    }

    double rro_temp = 0.0;

#pragma omp target teams distribute reduction(+:rro_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*y*x+jj*x+kk;
                const double smvp = SMVP(vec_u);
                vec_w[index] = smvp;
                vec_r[index] = vec_u[index]-vec_w[index];
                vec_p[index] = vec_r[index];
                rro_temp += vec_r[index]*vec_p[index];
            }
        }
    }

    // Sum locally
    *rro += rro_temp;
}

// Calculates w
void cg_solver_calc_w(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        double* pw,
        double* vec_p,
        double* vec_w,
        double* vec_kx,
        double* vec_ky,
        double* vec_kz)
{
    double pw_temp = 0.0;

#pragma omp target teams distribute reduction(+:pw_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                const double smvp = SMVP(vec_p);
                vec_w[index] = smvp;
                pw_temp += vec_w[index]*vec_p[index];
            }
        }
    }

    *pw += pw_temp;
}

// Calculates u and r
void cg_solver_calc_ur(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const double alpha,
        double* rrn,
        double* vec_u,
        double* vec_p,
        double* vec_r,
        double* vec_w)
{
    double rrn_temp = 0.0;

#pragma omp target teams distribute reduction(+:rrn_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;

                vec_u[index] += alpha*vec_p[index];
                vec_r[index] -= alpha*vec_w[index];
                rrn_temp += vec_r[index]*vec_r[index];
            }
        }
    }

    *rrn += rrn_temp;
}

// Calculates p
void cg_solver_calc_p(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const double beta,
        double* vec_p,
        double* vec_r)
{
#pragma omp target teams distribute
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                const int index = ii*x*y+jj*x+kk;
                vec_p[index] = beta*vec_p[index] + vec_r[index];
            }
        }
    }
}

