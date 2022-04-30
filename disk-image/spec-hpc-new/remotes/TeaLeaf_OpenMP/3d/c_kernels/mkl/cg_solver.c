#include <stdlib.h>
#include "../../shared.h"
#include "mkl.h"

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
        double* vec_kz,
        int* a_row_index,
        int* a_col_index,
        double* a_non_zeros)
{
    if(coefficient != CONDUCTIVITY && coefficient != RECIP_CONDUCTIVITY)
    {
        die(__LINE__, __FILE__, "Coefficient %d is not valid.\n", coefficient);
    }

#pragma omp parallel for
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

#pragma omp parallel for
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

#pragma omp parallel for
    for(int ii = halo_depth; ii < z-1; ++ii)
    {
        for(int jj = halo_depth; jj < y-1; ++jj)
        {
            for(int kk = halo_depth; kk < x-1; ++kk)
            {
                const int index = ii*x*y + jj*x + kk;
                vec_kx[index] = rx*(vec_w[index-1]+vec_w[index]) /
                    (2.0*vec_w[index-1]*vec_w[index]);
                vec_ky[index] = ry*(vec_w[index-x]+vec_w[index]) /
                    (2.0*vec_w[index-x]*vec_w[index]);
                vec_kz[index] = rz*(vec_w[index-x*y]+vec_w[index]) /
                    (2.0*vec_w[index-x*y]*vec_w[index]);
            }
        }
    }

    // Initialise the CSR sparse coefficient matrix
    for(int ii = halo_depth; ii < z-1; ++ii)
    {
        for(int jj = halo_depth; jj < y-1; ++jj)
        {
            for(int kk = halo_depth; kk < x-1; ++kk)
            {
                const int index = ii*x*y + jj*x + kk;
                int coef_index = a_row_index[index];

                if(ii >= halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_kz[index];
                    a_col_index[coef_index++] = index-x*y;
                }

                if(jj >= halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_ky[index];
                    a_col_index[coef_index++] = index-x;
                }

                if(kk >= halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_kx[index];
                    a_col_index[coef_index++] = index-1;
                }

                a_non_zeros[coef_index] = (1.0 + 
                        vec_kx[index+1] + vec_kx[index] + 
                        vec_ky[index+x] + vec_ky[index] + 
                        vec_kz[index+x*y] + vec_kz[index]);
                a_col_index[coef_index++] = index;

                if(ii < z-halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_kz[index+x*y];
                    a_col_index[coef_index++] = index+x*y;
                }

                if(jj < y-halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_ky[index+x];
                    a_col_index[coef_index++] = index+x;
                }

                if(kk < x-halo_depth)
                {
                    a_non_zeros[coef_index] = -vec_kx[index+1];
                    a_col_index[coef_index] = index+1;
                }
            }
        }
    }

    double rro_temp = 0.0;

    int m = x*y*z;
    mkl_cspblas_dcsrgemv(
            "n", &m, a_non_zeros, a_row_index, a_col_index, vec_u, vec_w);

    int x_inner = x-2*halo_depth;

#pragma omp parallel for reduction(+:rro_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*y*x + jj*x + halo_depth;
            cblas_dcopy(x_inner, vec_u + offset, 1, vec_r + offset, 1);
            cblas_daxpy(x_inner, -1.0, vec_w + offset, 1, vec_r + offset, 1);
            cblas_dcopy(x_inner, vec_r + offset, 1, vec_p + offset, 1);
            rro_temp += cblas_ddot(x_inner, vec_r + offset, 1, vec_p + offset, 1);
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
        double* vec_kz,
        int* a_row_index,
        int* a_col_index,
        double* a_non_zeros)
{
    double pw_temp = 0.0;

    int m = x*y*z;
    mkl_cspblas_dcsrgemv(
            "n", &m, a_non_zeros, a_row_index, a_col_index, vec_p, vec_w);

    int x_inner = x - 2*halo_depth;
#pragma omp parallel for reduction(+:pw_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            int offset = ii*x*y + jj*x + halo_depth;
            pw_temp += cblas_ddot(x_inner, vec_w + offset, 1, vec_p + offset, 1);
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
    int x_inner = x - 2*halo_depth;

#pragma omp parallel for reduction(+:rrn_temp)
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*x*y + jj*x + halo_depth;
            cblas_daxpy(x_inner, alpha, vec_p + offset, 1, vec_u + offset, 1);
            cblas_daxpy(x_inner, -alpha, vec_w + offset, 1, vec_r + offset, 1);
            rrn_temp += cblas_ddot(x_inner, vec_r + offset, 1, vec_r + offset, 1);
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
    int x_inner = x - 2*halo_depth;

#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*x*y + jj*x + halo_depth;
            cblas_dscal(x_inner, beta, vec_p + offset, 1);
            cblas_daxpy(x_inner, 1.0, vec_r + offset, 1, vec_p + offset, 1);
        }
    }
}

//// Calculates w
//void cg_solver_calc_w(
//        const int x,
//        const int y,
//        const int z,
//        const int halo_depth,
//        double* pw,
//        double* vec_p,
//        double* vec_w,
//        double* vec_kx,
//        double* vec_ky,
//        double* vec_kz)
//{
//    double pw_temp = 0.0;
//
//#pragma omp parallel for reduction(+:pw_temp)
//    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
//    {
//        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
//        {
//            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
//            {
//                const int index = ii*x*y+jj*x+kk;
//                const double smvp = SMVP(vec_p);
//                vec_w[index] = smvp;
//                pw_temp += vec_w[index]*vec_p[index];
//            }
//        }
//    }
//
//    *pw += pw_temp;
//}

//// Calculates u and r
//void cg_solver_calc_ur(
//        const int x,
//        const int y,
//        const int z,
//        const int halo_depth,
//        const double alpha,
//        double* rrn,
//        double* vec_u,
//        double* vec_p,
//        double* vec_r,
//        double* vec_w)
//{
//    double rrn_temp = 0.0;
//
//#pragma omp parallel for reduction(+:rrn_temp)
//    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
//    {
//        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
//        {
//            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
//            {
//                const int index = ii*x*y+jj*x+kk;
//
//                vec_u[index] += alpha*vec_p[index];
//                vec_r[index] -= alpha*vec_w[index];
//                rrn_temp += vec_r[index]*vec_r[index];
//            }
//        }
//    }
//
//    *rrn += rrn_temp;
//}

//// Calculates p
//void cg_solver_calc_p(
//        const int x,
//        const int y,
//        const int z,
//        const int halo_depth,
//        const double beta,
//        double* vec_p,
//        double* vec_r)
//{
//#pragma omp parallel for
//    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
//    {
//        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
//        {
//            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
//            {
//                const int index = ii*x*y+jj*x+kk;
//                vec_p[index] = beta*vec_p[index] + vec_r[index];
//            }
//        }
//    }
//}

