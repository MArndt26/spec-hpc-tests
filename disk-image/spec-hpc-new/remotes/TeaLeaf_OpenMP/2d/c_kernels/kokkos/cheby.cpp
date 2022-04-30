#include "kokkos_shared.hpp"
#include "../../shared.h"

using namespace Kokkos;

// Initialises the Chebyshev solver
void cheby_init(
        const int x, const int y, const int halo_depth, const double theta, 
        KView p, KView r, KView u, KView u0, KView w, 
        KView kx, KView ky) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            const double smvp = SMVP(u);
            w[index] = smvp;
            r[index] = u0[index]-w[index];
            p[index] = r[index]/theta;
        }
    });
}

// Calculates U
void cheby_calc_u(
        const int x, const int y, const int halo_depth, KView p, 
        KView u) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            u[index] += p[index];
        }
    });
}

// The main Cheby iteration step
void cheby_iterate(
        const int x, const int y, const int halo_depth, 
        const double alpha, const double beta, KView p, 
        KView r, KView u, KView u0, KView w, KView kx, 
        KView ky) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            const double smvp = SMVP(u);
            w[index] = smvp;
            r[index] = u0[index]-w[index];
            p[index] = alpha*p[index] + beta*r[index];
        }
    });
}

