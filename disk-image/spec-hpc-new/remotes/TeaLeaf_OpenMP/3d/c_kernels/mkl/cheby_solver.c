#include "../../shared.h"
#include "mkl.h"

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
    int x_inner = x - 2*halo_depth;

#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*x*y + jj*x + halo_depth;
            cblas_daxpy(x_inner, 1.0, vec_p + offset, 1, vec_u + offset, 1);
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
        double* vec_kz,
        int* a_row_index,
        int* a_col_index,
        double* a_non_zeros)
{
    int m = x*y*z;

    mkl_cspblas_dcsrgemv(
            "n", &m, a_non_zeros, a_row_index, a_col_index, vec_u, vec_w);

    int x_inner = x - 2*halo_depth;

#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*x*y + jj*x + halo_depth;
            cblas_dcopy(x_inner, vec_u0 + offset, 1, vec_r + offset, 1);
            cblas_daxpy(x_inner, -1.0, vec_w + offset, 1, vec_r + offset, 1);
            cblas_dscal(x_inner, 1.0/theta, vec_r + offset, 1);
            cblas_dcopy(x_inner, vec_r + offset, 1, vec_p + offset, 1);
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
        double* vec_kz,
        int* a_row_index,
        int* a_col_index,
        double* a_non_zeros)
{
    int m = x*y*z;

    mkl_cspblas_dcsrgemv(
            "n", &m, a_non_zeros, a_row_index, a_col_index, vec_u, vec_w);

    int x_inner = x - 2*halo_depth;
    
#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            const int offset = ii*x*y + jj*x + halo_depth;
            cblas_dcopy(x_inner, vec_u0 + offset, 1, vec_r + offset, 1);
            cblas_daxpy(x_inner, -1.0, vec_w + offset, 1, vec_r + offset, 1);
            cblas_dscal(x_inner, alpha, vec_p + offset, 1);
            cblas_daxpy(x_inner, beta, vec_r + offset, 1, vec_p + offset, 1);
        }
    }

    cheby_calc_u(x, y, z, halo_depth, vec_u, vec_p);
}

