#include "kokkos_shared.hpp"
#include "../../shared.h"

using namespace Kokkos;

// Initialises Sd
void ppcg_init(
        const int x, const int y, const int halo_depth, const double theta, 
        KView sd, KView r) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            sd[index] = r[index]/theta;
        }
    });
}

// Calculates U and R
void ppcg_calc_ur(
        const int x, const int y, const int halo_depth, KView sd, 
        KView r, KView u, KView kx, KView ky) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            const double smvp = SMVP(sd);
            r[index] -= smvp;
            u[index] += sd[index];
        }
    });
}

// Calculates Sd
void ppcg_calc_sd(
        const int x, const int y, const int halo_depth, const double theta, 
        const double alpha, const double beta, KView sd, KView r) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            sd[index] = alpha*sd[index] + beta*r[index];
        }
    });
}

