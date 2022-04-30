#include "kokkos_shared.hpp"
#include "../../shared.h"

using namespace Kokkos;

// Copies the inner u into u0.
void copy_u(
        const int x, const int y, const int halo_depth, KView u, KView u0) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            u0(index) = u(index);	
        }
    });
}

// Calculates the residual r.
void calculate_residual(
            const int x, const int y, const int halo_depth, KView u, 
            KView u0, KView r, KView kx, KView ky) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            const double smvp = SMVP(u);
            r(index) = u0(index) - smvp;
        }
    });
}

// Calculates the 2 norm of the provided buffer.
void calculate_2norm(
        const int x, const int y, const int halo_depth, KView buffer,
        double* norm) 
{
    parallel_reduce(x*y, KOKKOS_LAMBDA (const int index, double& norm_temp)
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            norm_temp += buffer(index)*buffer(index);			
        }
    }, *norm);
}

// Finalises the energy field.
void finalise(
        const int x, const int y, const int halo_depth, KView u, 
        KView density, KView energy) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            energy(index) = u(index)/density(index);
        }
    });
}     

